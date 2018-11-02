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
#include <CRCore/crBlockDetectThread.h>
#include <CRCore/thread/crCondition.h>
#include "windows.h" 
using namespace CRCore;
ref_ptr<crBlockDetectThread> crBlockDetectThread::m_instance = NULL;

crBlockDetectThread* crBlockDetectThread::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crBlockDetectThread;
	}
	return m_instance.get();
}
void crBlockDetectThread::clear()
{
	m_instance = NULL;
}

crBlockDetectThread::crBlockDetectThread()
:m_done(true),
m_timeout(300),
m_timer(0),
m_pause(false)
{
}

crBlockDetectThread::~crBlockDetectThread() 
{
	m_done = true;
	while (isRunning()) 
	{
		CRCore::crThread::sleep(10);
	}
}
void crBlockDetectThread::done()
{
	m_done = true;
}
void crBlockDetectThread::update()
{
	m_timer = CRCore::Timer::instance()->tick();
}
void crBlockDetectThread::setTimeout(int timeout)
{
	m_timeout = timeout;
}
void crBlockDetectThread::pause()
{
	m_pause = false;
}
void crBlockDetectThread::resume()
{
	m_pause = true;
}
void crBlockDetectThread::run()
{
	m_done = false;
	m_timer = CRCore::Timer::instance()->tick();
	CRCore::Timer_t t1;
	float dt;
	setSchedulePriority(THREAD_PRIORITY_NOMINAL);
	while(!m_done)
	{
		if (!m_pause)
		{
			t1 = CRCore::Timer::instance()->tick();
			dt = CRCore::Timer::instance()->delta_s(m_timer, t1);
			if (dt > m_timeout)
			{
				_asm   int   3   //只是为了让程序崩溃
			}
		}
		crThread::sleep(1000);
	}
}