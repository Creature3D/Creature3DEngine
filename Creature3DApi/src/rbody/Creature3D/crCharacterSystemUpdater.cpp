/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
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
#include <rbody/Creature3D/crCharacterSystemUpdater.h>
#include <CRCore/crBrain.h>
#include <CRCore/crStatistics.h>
#include "rbody/Creature3D/ReplicantBodyMgr.h"
using namespace rbody;
using namespace CRCore;
//using namespace CRUtil;
ref_ptr<crCharacterSystemUpdater> crCharacterSystemUpdater::m_instance = NULL;

crCharacterSystemUpdater* crCharacterSystemUpdater::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crCharacterSystemUpdater;
		crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}
void crCharacterSystemUpdater::clear()
{
	m_instance = NULL;
}

crCharacterSystemUpdater::crCharacterSystemUpdater()
:m_done(true),
m_time(0L)
{
	m_updateBarrier = new CRCore::crBarrier(2);
	m_frameBarrier = new CRCore::crBarrier(2);
}

crCharacterSystemUpdater::~crCharacterSystemUpdater() 
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

CRCore::crBarrier *crCharacterSystemUpdater::getUpdateBlock()
{
	 return m_updateBarrier;
}

CRCore::crBarrier *crCharacterSystemUpdater::getFrameBlock()
{
	return m_frameBarrier;
}

void crCharacterSystemUpdater::done()
{
	m_done = true;
	//m_cullBarrier->release();
	//m_frameBarrier->release();
	//m_cullBarrier->block(1);
	//m_frameBarrier->block(1);
}

void crCharacterSystemUpdater::run()
{
#if _DEBUG
	try
	{
#endif
	m_done = false;
	CRCore::Timer_t start_tick,end_tick;
	CRCore::Timer_t t1;
	float dt;
	while(!m_done)
	{
		updateBlock();
		//CRCore::notify(CRCore::ALWAYS)<<"m_cullBarrier->block()"<<std::endl;
		frameBlock();
		//CRCore::notify(CRCore::ALWAYS)<<"m_frameBarrier->block()"<<std::endl;
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
			if(dt>0.05f)
			{
				if(crStatistics::getInstance()->getStat())
				{
					start_tick = CRCore::Timer::instance()->tick();
				}
				
				m_time = t1;
				ReplicantBodyMgr::instance()->updateCharacter(dt);

				if(crStatistics::getInstance()->getStat())
				{
					end_tick = CRCore::Timer::instance()->tick();
					float ms = CRCore::Timer::instance()->delta_m(start_tick,end_tick);
					crStatistics::getInstance()->setCharacterUpdTime(ms);
				}
			}
		}
	}
	rbody::ReplicantBodyMgr::instance()->clear();
#if _DEBUG
	CRCore::notify(CRCore::ALWAYS)<<"crCharacterSystemUpdater end"<<std::endl;
	//sprintf(gDebugInfo->buf(),"crCharacterSystemUpdater end\n\0");
	//gDebugInfo->debugInfo(CRCore::ALWAYS);
	}
	catch (...)
	{
		CRCore::notify(CRCore::ALWAYS)<<"crCharacterSystemUpdater::run() error "<<std::endl;
		//sprintf(gDebugInfo->buf(),"crCharacterSystemUpdater::run() error\n\0");
		//gDebugInfo->debugInfo(CRCore::FATAL);
	}
#endif
}