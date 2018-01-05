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
#ifndef CRCORE_CRTIMER_H
#define CRCORE_CRTIMER_H 

#include <CRCore/crExport.h>
#include <time.h>
namespace CRCore {

#if defined(_MSC_VER)
	typedef __int64 Timer_t;
#else
	typedef unsigned long long Timer_t;
#endif

	/** Time stamper. */
	class CR_EXPORT Timer {

	public:

		Timer();
		~Timer() {}

		static const Timer* instance();

		Timer_t tick() const;

		inline double delta_s( Timer_t t1, Timer_t t2 ) const { return (double)(t2 - t1)*m_secsPerTick; }
		inline double delta_m( Timer_t t1, Timer_t t2 ) const { return delta_s(t1,t2)*1e3; }
		inline double delta_u( Timer_t t1, Timer_t t2 ) const { return delta_s(t1,t2)*1e6; }
		inline double delta_n( Timer_t t1, Timer_t t2 ) const { return delta_s(t1,t2)*1e9; }

		inline double getSecondsPerTick() const { return m_secsPerTick; }

		static void SystemTime(int& Year, int& Month, int& DayOfWeek, int& Day, int& Hour, int& Min, int& Sec, int& MSec)
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
		}
	protected :

		double m_secsPerTick;

	};
}
#endif
