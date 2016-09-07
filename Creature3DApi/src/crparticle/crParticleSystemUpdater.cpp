/* Creature3D - Online Game Engine, Copyright (C) 2005 Îâ²Æ»ª(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/
#include <CRParticle/crParticleSystemUpdater.h>
#include <CRCore/crBrain.h>
#include <CRCore/crStatistics.h>
using namespace CRCore;
using namespace CRParticle;
//using namespace CRUtil;
ref_ptr<crParticleSystemUpdater> crParticleSystemUpdater::m_instance = NULL;

crParticleSystemUpdater* crParticleSystemUpdater::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crParticleSystemUpdater;
		crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}
void crParticleSystemUpdater::clear()
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_mutex);
	m_instance = NULL;
}

crParticleSystemUpdater::crParticleSystemUpdater()
:m_time(0L),m_done(true)
{
    m_updateBarrier = new CRCore::crBarrier(2);
	m_frameBarrier = new CRCore::crBarrier(2);
}

crParticleSystemUpdater::~crParticleSystemUpdater() 
{
	m_done = true;
	if(m_updateBarrier) m_updateBarrier->invalidate();
	if(m_frameBarrier) m_frameBarrier->invalidate();
	while (isRunning()) 
	{
		CRCore::crThread::sleep(10);
	}

	if(m_updateBarrier)
	{
		delete m_updateBarrier; 
		m_updateBarrier = NULL;
	}
	if(m_frameBarrier)
	{
		delete m_frameBarrier;
		m_frameBarrier = NULL;
	}
}

void crParticleSystemUpdater::done()
{
	m_done = true;
}

CRCore::crBarrier *crParticleSystemUpdater::getUpdateBlock()
{
	return m_updateBarrier;
}

CRCore::crBarrier *crParticleSystemUpdater::getFrameBlock()
{
	return m_frameBarrier;
}

void crParticleSystemUpdater:: run()
{
	//try
	//{
	m_done = false;
	CRCore::Timer_t start_tick,end_tick;
	CRCore::Timer_t t1;
	float dt;
	CRCore::ref_ptr<crParticleEffect> setItr;
	while(!m_done)
	{
		updateBlock();
		frameBlock();
        if(!m_done)
		{
			t1 = CRCore::Timer::instance()->tick();
			if(m_time == 0)
			{
				m_time = t1;
				CRCore::crThread::yieldCurrentThread();
				continue;
			}
			dt = CRCore::Timer::instance()->delta_s( m_time, t1 );
			if(dt>0.04f)
			{
				if(crStatistics::getInstance()->getStat())
				{
					start_tick = CRCore::Timer::instance()->tick();
				}

				m_time = t1;
				CRCore::ScopedLock<CRCore::crMutex> lock(m_mutex);
				for( ParticleEffectSet::iterator itr = m_particleEffectSet.begin();
					itr!=m_particleEffectSet.end();
					++itr)
				{
					setItr = *itr;
					//if(setItr->getVisiable())
					//{
						//setItr->needUpdate() = false;
						if (setItr->getVisiable() && !setItr->getParticleSystem()->isFrozen() && !setItr->getParticleSystem()->getFreezeOnCull())
						{
							setItr->getEmitter()->update(dt);
							setItr->getProgram()->update(dt);

							setItr->getParticleSystem()->update(dt);
						}
					//}
				}

				if(crStatistics::getInstance()->getStat())
				{
					end_tick = CRCore::Timer::instance()->tick();
					float ms = CRCore::Timer::instance()->delta_m(start_tick,end_tick);
					crStatistics::getInstance()->setParticleUpdTime(ms);
				}
			}
		}
	}
#if _DEBUG
    CRCore::notify(CRCore::ALWAYS)<<"crParticleSystemUpdater end"<<std::endl;
#endif
	//}
	//catch (...)
	//{
	//	CRCore::notify(CRCore::ALWAYS)<<"crParticleSystemUpdater::run() error "<<std::endl;
	//}
}
void crParticleSystemUpdater::insertParticleEffect(crParticleEffect *effect)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_mutex);
	m_particleEffectSet.insert(effect);
}
void crParticleSystemUpdater::removeParticleEffect(crParticleEffect *effect)
{
    CRCore::ScopedLock<CRCore::crMutex> lock(m_mutex);
	m_particleEffectSet.erase(effect);
}
CRCore::crGroup *crParticleSystemUpdater::findEffectGroup(const std::string &nodeName)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_mutex);
	CRCore::ref_ptr<crParticleEffect> setItr;
	for( ParticleEffectSet::iterator itr = m_particleEffectSet.begin();
		itr!=m_particleEffectSet.end();
		++itr)
	{
		setItr = *itr;
		if(setItr->getParent(0)->getName().compare(nodeName) == 0)
			return setItr->getParent(0);
	}
	return NULL;
}
void crParticleSystemUpdater::lock()
{
	m_mutex.lock();
}
crParticleSystemUpdater::ParticleEffectSet &crParticleSystemUpdater::getParticleEffectSet()
{
	return m_particleEffectSet;
}
void crParticleSystemUpdater::unlock()
{
	m_mutex.unlock();
}