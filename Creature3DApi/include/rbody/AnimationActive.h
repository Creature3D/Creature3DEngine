#ifndef _CANIMATIONACTIVE_H
#define _CANIMATIONACTIVE_H

#include "rbody/global.h"
#include <string>

namespace rbody {

/// Contains information of animation currently in queue that is being or is about to be executed
class REPLICANTBODY_API AnimationActive {

    public:

        /// Constructor
        AnimationActive();

        /// Destructor
        virtual ~AnimationActive();

        /// Set start time of animation
        /**
         * \param start_time is time for animation to start
         **/
        void setStartTime(float start_time);

        /// Get end time of animation
        float getEndTime();

        /// Set start time of animation
        /**
         * \param end_time is time for animation to end
         **/
        void setEndTime(float end_time);

        /// Get start time of animation
        float getStartTime();

        /// Set current time of animation
        /**
         * \param current_time is number of seconds that has elapsed in animation
         **/
        void setCurrentTime(float current_time);

        /// Get current time of animation
        float getCurrentTime();

        /// Set duration of animation
        /**
         * \param duration is length of animation in seconds
         **/
        void setDuration(float duration);

        /// Get duration of animation
        float getDuration();

        /// Set weight of animation - used for blending
        /**
         * \param weight is weight of animation
         **/
        void setWeight(float weight);

        /// Get weight of animation
        float getWeight();

        /// Set Cal3d id
        /**
         * \param cal3d_id is id assigned to in cal3d
         **/
        void setCal3dId(int cal3d_id);

        /// Get Cal3d id
        int getCal3dId();

        /// Set group id - used to bundle animations
        /**
         * \param group_id is id of animation
         **/
        void setGroupId(int group_id);

        /// Get group id of animation - used to bundle animations
        int getGroupId();

        /// Set name of action/activity that animation is connected to
        /**
         * \param act_name is name of action/activity
         **/
        void setActName(const std::string& act_name);

        /// Get action/activity name that animation is connected to
        std::string getActName();

        /// Set whether animation is looped or not
        /**
         * \param is_looped is boolean which tells if animation is looped or not
         **/
        void setIsLooped(unsigned char is_looped);

        /// Return true if animation is looped, else false
        unsigned char isLooped();

        /// Set whether animation is activated (has started) or not
        /**
         * \param is_activated is boolean which tells if animation has started
         **/
        void setIsActivated(bool is_activated);

        /// Return true if animation is activated, else false
        bool isActivated();

        /// Set speed
        void setSpeed(float speed) { m_speed = speed; };

        /// Get speed
        float getSpeed() { return m_speed; };

    private:

        /// Time for animation to start
        float m_start_time;

        /// Time for animation to end
        float m_end_time;

        /// Current time for animation
        float m_current_time;

        /// Duration of animation
        float m_duration;

        /// Cal3d animation id
        int m_cal3d_id;

        /// Name of action/activity that amination is connected to
        std::string m_act_name;

        /// Tells whether animation is looped or not
        unsigned char m_is_looped;//1循环，其它值：结束时候停止动作（0，2）

        /// Weight of animation (used for blending)
        float m_weight;

        /// Id (used to bundle animations)
        int m_group_id;

        /// Activated (used to decide if animation has started)
        bool m_is_activated;

        /// Speed (if speed animation this speed is the resulting speed of the animations
        /// else it is the default speed of the animation)
        float m_speed;

};

} // namespace end

#endif // _CANIMATIONACTIVE_H
/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rbody/AnimationActive.h,v $
* $Revision: 1.1.1.1 $ 
* $Date: 2005/02/06 06:52:59 $
* $Author: deepone $ 
* $Locker:  $

* Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

 Created: 2001-12-12

* $Log: AnimationActive.h,v $
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.2  2003/10/31 13:21:45  andersb
* Added ReplicantBodyMgr class to handle instances of CreBody
* Added .char files for specifying individual characters (CreBody)
* Added OSG plugin for reading char files
*
* Revision 1.1  2002/06/20 12:51:50  sunna
* Restructured directories
*
* Revision 1.3  2002/04/10 06:54:52  sunna
* Added get and set speed
*
* Revision 1.2  2002/03/15 08:55:07  andersb
* no message
*
* Revision 1.1.1.1  2002/03/14 20:48:40  andersb
* First import as ReplicantBody
*
* Revision 1.1.1.1  2002/01/04 08:22:34  andersb
* Initial version
*
----------------------------------------------------------------------------*/



