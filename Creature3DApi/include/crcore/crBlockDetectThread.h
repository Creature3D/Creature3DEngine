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

#ifndef CRCORE_CRBLOCKDETECTTHREAD
#define CRCORE_CRBLOCKDETECTTHREAD 1

#include <CRCore/crExport.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/Referenced.h>
#include <CRCore/thread/crThread.h>
namespace CRCore
{
	class CR_EXPORT crBlockDetectThread: public CRCore::crThread, public CRCore::Referenced
	{
	public:
		crBlockDetectThread();
		static crBlockDetectThread *getInstance();

		virtual void clear();

		virtual void run();
		void update();
		void done();
		void setTimeout(int timeout);
		void pause();
		void resume();
	protected:
		virtual ~crBlockDetectThread();
		static CRCore::ref_ptr<crBlockDetectThread> m_instance;
	private:
		bool m_done;
		int m_timer;
		int m_timeout;
		bool m_pause;
	};
}

#endif
