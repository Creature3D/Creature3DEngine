/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
#ifndef CRPARTICLE_PARTICLESYSTEMUPDATER
#define CRPARTICLE_PARTICLESYSTEMUPDATER 1

#include <CRParticle/crExport.h>
#include <CRParticle/crParticleEffect.h>
#include <set>
#include <CRCore/ref_ptr.h>
#include <CRCore/crTimer.h>
#include <CRCore/thread/crThread.h>
#include <CRCore/thread/crBarrier.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRCore/thread/crMutex.h>

namespace CRParticle
{
	class CRPARTICLE_EXPORT crParticleSystemUpdater: public CRCore::crThread, public CRCore::Referenced
	{
    public:
        crParticleSystemUpdater();
		static crParticleSystemUpdater *getInstance();
		virtual void clear();

		virtual void run();
		inline void updateBlock() { if(m_updateBarrier) m_updateBarrier->block(); }
		inline void frameBlock() { if(m_frameBarrier) m_frameBarrier->block(); }
		CRCore::crBarrier *getUpdateBlock();
		CRCore::crBarrier *getFrameBlock();
		void done();

		void insertParticleEffect(crParticleEffect *effect);
		void removeParticleEffect(crParticleEffect *effect);

		//inline void resetDrawCount(){ m_draw_count = 0; }
		//inline void addDrawCount(int count){ m_draw_count += count; }
		//inline int getDrawCount(){ return m_draw_count; }

		CRCore::crGroup *findEffectGroup(const std::string &nodeName);
		typedef std::set< CRCore::ref_ptr<crParticleEffect> > ParticleEffectSet;
        void lock();
        ParticleEffectSet &getParticleEffectSet();
		void unlock();
    protected:
        virtual ~crParticleSystemUpdater();
        
		static CRCore::ref_ptr<crParticleSystemUpdater> m_instance;

		ParticleEffectSet m_particleEffectSet;

		CRCore::crBarrier *m_updateBarrier;
		CRCore::crBarrier *m_frameBarrier;
		mutable CRCore::crMutex m_mutex;

	    CRCore::Timer_t m_time;
        bool m_done;

		//int m_draw_count;
    };
}

#endif
