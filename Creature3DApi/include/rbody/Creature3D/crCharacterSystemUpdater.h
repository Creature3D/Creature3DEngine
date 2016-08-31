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

#ifndef CREBODY_CRCHARACTERSYSTEMUPDATER
#define CREBODY_CRCHARACTERSYSTEMUPDATER 1

#include <rbody/export.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/Referenced.h>
#include <CRCore/thread/crThread.h>
#include <CRCore/thread/crBarrier.h>
#include <CRCore/crTimer.h>
namespace rbody
{
	class REPLICANTBODY_API crCharacterSystemUpdater: public CRCore::crThread, public CRCore::Referenced
	{
    public:
		crCharacterSystemUpdater();
		static crCharacterSystemUpdater *getInstance();

		virtual void clear();

		virtual void run();

		inline void updateBlock() { if(m_updateBarrier) m_updateBarrier->block(); }
		inline void frameBlock() { if(m_frameBarrier) m_frameBarrier->block(); }
		CRCore::crBarrier *getUpdateBlock();
		CRCore::crBarrier *getFrameBlock();
		void done();
	protected:
		virtual ~crCharacterSystemUpdater();
		static CRCore::ref_ptr<crCharacterSystemUpdater> m_instance;
	private:
		CRCore::crBarrier *m_updateBarrier;
		CRCore::crBarrier *m_frameBarrier;
		bool m_done;
		CRCore::Timer_t m_time;
	};	
}

#endif
