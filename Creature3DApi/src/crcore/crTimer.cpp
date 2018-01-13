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
//Modified by Wucaihua
//#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <CRCore/crTimer.h>

using namespace CRCore;

Timer_t Timer::STARTTICK = Timer::instance()->tick();
const Timer* Timer::instance()
{
	static Timer s_timer;
	return &s_timer;
}

#ifdef WIN32

#include <iostream>

#include <sys/types.h>
#include <fcntl.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winbase.h>
Timer::Timer()
{
	LARGE_INTEGER frequency;
	if(QueryPerformanceFrequency(&frequency))
	{
		m_secsPerTick = 1.0/(double)frequency.QuadPart;
	}
	else
	{
		m_secsPerTick = 1.0;
		std::cerr <<"Error: Timer::Timer() unable to use QueryPerformanceFrequency, "<<std::endl;
		std::cerr <<"timing code will be wrong, Windows error code: "<<GetLastError()<<std::endl;
	}
}

Timer_t Timer::tick() const
{
	LARGE_INTEGER qpc;
	if (QueryPerformanceCounter(&qpc))
	{
		return qpc.QuadPart;
	}
	else
	{
		std::cerr <<"Error: Timer::Timer() unable to use QueryPerformanceCounter, "<<std::endl;
		std::cerr <<"timing code will be wrong, Windows error code: "<<GetLastError()<<std::endl;
		return 0;
	}
}

void Timer::SystemTime(int& Year, int& Month, int& DayOfWeek, int& Day, int& Hour, int& Min, int& Sec, int& MSec)
{
	time_t nowTime_t = time(0);
	struct  tm  nowTm = *localtime(&nowTime_t);
	// pull out data/time
	Year = nowTm.tm_year + 1900;
	Month = nowTm.tm_mon + 1;
	DayOfWeek = nowTm.tm_wday;
	Day = nowTm.tm_mday;
	Hour = nowTm.tm_hour;
	Min = nowTm.tm_min;
	Sec = nowTm.tm_sec;
	LARGE_INTEGER t, freq;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&t);
	int tv_usec = int((t.QuadPart % freq.QuadPart) * 1000000 / freq.QuadPart);
	MSec = tv_usec / 1000;
}
#else

#include <sys/time.h>

Timer::Timer( void )
{
	m_secsPerTick = (1.0 / (double) 1000000);
}

Timer_t Timer::tick() const
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((Producer::Timer_t)tv.tv_sec)*1000000+(Producer::Timer_t)tv.tv_usec;
}

#endif
