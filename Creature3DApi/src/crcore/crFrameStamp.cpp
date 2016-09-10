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
#include <CRCore/crFrameStamp.h>

using namespace CRCore;
ref_ptr<crFrameStamp> crFrameStamp::m_instance = NULL;
crFrameStamp::crFrameStamp():Referenced()
{
    m_frameNumber=0;
    m_referenceTime=0;
	m_frameInterval = 0;

    tm_sec=0;            /* Seconds.        [0-60] (1 leap second) */
    tm_min=0;            /* Minutes.        [0-59] */
    tm_hour=0;           /* Hours.          [0-23] */
    tm_mday=0;           /* Day.            [1-31] */
    tm_mon=0;            /* Month.          [0-11] */
    tm_year=0;           /* Year            - 1900.  */
    tm_wday=0;           /* Day of week.    [0-6] */
    tm_yday=0;           /* Days in year.   [0-365]    */
    tm_isdst=0;           /* DST.           [-1/0/1]*/

	m_time = Timer::instance();
	m_lasttick = m_time->tick();
	m_fps = 0;
	m_length = 0;
	m_fpsFrameCount = 0;
}
crFrameStamp* crFrameStamp::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crFrameStamp;
	}
	return m_instance.get();
}
//crFrameStamp::crFrameStamp(const crFrameStamp& fs):Referenced()
//{
//    m_frameNumber = fs.m_frameNumber;
//    m_referenceTime = fs.m_referenceTime;
//	m_frameInterval = 0;
//    
//    tm_sec = fs.tm_sec;            /* Seconds.    [0-60] (1 leap second) */
//    tm_min = fs.tm_min;            /* Minutes.    [0-59] */
//    tm_hour = fs.tm_hour;            /* Hours.    [0-23] */
//    tm_mday = fs.tm_mday;            /* Day.        [1-31] */
//    tm_mon = fs.tm_mon;            /* Month.    [0-11] */
//    tm_year = fs.tm_year;            /* Year    - 1900.  */
//    tm_wday = fs.tm_wday;            /* Day of week.    [0-6] */
//    tm_yday = fs.tm_yday;            /* Days in year.[0-365]    */
//    tm_isdst = fs.tm_isdst;            /* DST.        [-1/0/1]*/
//
//    m_time = fs.m_time;
//	m_lasttick = fs.m_lasttick;
//    m_fps = fs.m_fps;
//    m_length = fs.m_length;
//	m_fpsFrameCount = 0;
//}

crFrameStamp::~crFrameStamp()
{
}

//crFrameStamp& crFrameStamp::operator = (const crFrameStamp& fs)
//{
//    if (this==&fs) return *this;
//
//    m_frameNumber = fs.m_frameNumber;
//    m_referenceTime = fs.m_referenceTime;
//	m_frameInterval = 0;
//    
//    tm_sec = fs.tm_sec;            /* Seconds.    [0-60] (1 leap second) */
//    tm_min = fs.tm_min;            /* Minutes.    [0-59] */
//    tm_hour = fs.tm_hour;            /* Hours.    [0-23] */
//    tm_mday = fs.tm_mday;            /* Day.        [1-31] */
//    tm_mon = fs.tm_mon;            /* Month.    [0-11] */
//    tm_year = fs.tm_year;            /* Year    - 1900.  */
//    tm_wday = fs.tm_wday;            /* Day of week.    [0-6] */
//    tm_yday = fs.tm_yday;            /* Days in year.[0-365]    */
//    tm_isdst = fs.tm_isdst;            /* DST.        [-1/0/1]*/
//
//	m_time = fs.m_time;
//	m_lasttick = fs.m_lasttick;
//	m_fps = fs.m_fps;
//	m_length = fs.m_length;
//	m_fpsFrameCount = 0;
//    return *this;
//}

void crFrameStamp::setCalendarTime(const tm& ct)
{
    tm_sec = ct.tm_sec;            /* Seconds.    [0-60] (1 leap second) */
    tm_min = ct.tm_min;            /* Minutes.    [0-59] */
    tm_hour = ct.tm_hour;            /* Hours.    [0-23] */
    tm_mday = ct.tm_mday;            /* Day.        [1-31] */
    tm_mon = ct.tm_mon;            /* Month.    [0-11] */
    tm_year = ct.tm_year;            /* Year    - 1900.  */
    tm_wday = ct.tm_wday;            /* Day of week.    [0-6] */
    tm_yday = ct.tm_yday;            /* Days in year.[0-365]    */
    tm_isdst = ct.tm_isdst;            /* DST.        [-1/0/1]*/
}

void crFrameStamp::getCalendarTime(tm& ct) const
{
    ct.tm_sec = tm_sec;            /* Seconds.    [0-60] (1 leap second) */
    ct.tm_min = tm_min;            /* Minutes.    [0-59] */
    ct.tm_hour = tm_hour;            /* Hours.    [0-23] */
    ct.tm_mday = tm_mday;            /* Day.        [1-31] */
    ct.tm_mon = tm_mon;            /* Month.    [0-11] */
    ct.tm_year = tm_year;            /* Year    - 1900.  */
    ct.tm_wday = tm_wday;            /* Day of week.    [0-6] */
    ct.tm_yday = tm_yday;            /* Days in year.[0-365]    */
    ct.tm_isdst = tm_isdst;            /* DST.        [-1/0/1]*/
}

void crFrameStamp::update()
{
	m_fpsFrameCount++;
	if(m_fpsFrameCount>=10)
	{
		Timer_t now = m_time->tick();

		m_length = m_time->delta_s(m_lasttick,now);
		if(m_length<=0.0f)
			m_length=1.0f;

		m_fps=(int)((float)m_fpsFrameCount/m_length);

		m_lasttick = now;
		m_fpsFrameCount = 0;
	}
}