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
#ifndef CRCORE_FRAMESTAMP_H
#define CRCORE_FRAMESTAMP_H 1

#include <CRCore/Referenced.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/crTimer.h>
#if defined(__sgi) || (defined(WIN32) && !defined(__MWERKS__))
#include <time.h>
#else
#include <ctime>
using std::tm;
#endif

namespace CRCore
{

/** Class which encapsulates the frame number, reference time and calander
  * time of specific frame, used to synchonize operations on the scene graph
  * and other machines when using a graphics cluster.  Note the calander
  * time can be an artificial simulation time or capture the real time
  * of day etc.*/ 
class CR_EXPORT crFrameStamp : public Referenced
{
    public:

        crFrameStamp();
		static crFrameStamp *getInstance();
        //crFrameStamp(const crFrameStamp& fs);
        
        //crFrameStamp& operator = (const crFrameStamp& fs);
        
        inline void setFrameNumber(int fnum) { m_frameNumber = fnum; }
        inline int getFrameNumber() const { return m_frameNumber; }
        
        inline void setReferenceTime(double refTime) 
		{
			m_frameInterval = (m_referenceTime != 0.0) ? (refTime-m_referenceTime) : 0.0;
			m_referenceTime = refTime;
		}
        double getReferenceTime() const { return m_referenceTime; }
		inline float getFrameInterval() const { return m_frameInterval; }
        
        void setCalendarTime(const tm& calendarTime);
        void getCalendarTime(tm& calendarTime) const;

		int getFPS()		{ return m_fps; }
		int getFPS()const	{ return m_fps; }
		double getLength()	{ return m_length; }
		void update();

        // keep public to allow it to be permit allocation which is 
        // not on the heap used CRCorecluster
        virtual ~crFrameStamp();

    protected:
		static CRCore::ref_ptr<crFrameStamp> m_instance;

        // note no dynamic memory is used so that data can be passed
        // via a simple memory copy or within a data packet across
        // the network.
    
        int     m_frameNumber;
        double  m_referenceTime;
		float   m_frameInterval;   
	    const Timer	*m_time;		// a timer object
        Timer_t m_lasttick;
		int		m_fps;
		int		m_fpsFrameCount;
		double	m_length;		

        // member varaibles of time.h's tm structure, copied here to
        // ensure that all data is no dynamic. The tm structure itself
        // is not completely consistent betweem implementations, which
        // could be a problem when sending the crFrameStamp across a network
        // with differnt versions of tm (i.e mixing Unix and Windows.)
        int tm_sec;            /* Seconds.        [0-60] (1 leap second) */
        int tm_min;            /* Minutes.        [0-59] */
        int tm_hour;           /* Hours.          [0-23] */
        int tm_mday;           /* Day.            [1-31] */
        int tm_mon;            /* Month.          [0-11] */
        int tm_year;           /* Year            - 1900.  */
        int tm_wday;           /* Day of week.    [0-6] */
        int tm_yday;           /* Days in year.   [0-365]    */
        int tm_isdst;           /* DST.           [-1/0/1]*/
        
        
};

}


#endif
