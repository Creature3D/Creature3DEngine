#ifndef _CANIMATIONACTIVITYHANDLER_H
#define _CANIMATIONACTIVITYHANDLER_H

#include "rbody/global.h"
#include <string>
#include <list>

#include "rbody/AnimationHandler.h"
#include "rbody/ActRequestArg.h"
#include "rbody/Animation.h"

namespace rbody {

/// Handles all active animations
class REPLICANTBODY_API AnimationActivityHandler : public AnimationHandler {

    public:

        /// Constructor
        AnimationActivityHandler();

        /// Destructor
        virtual ~AnimationActivityHandler();

        /// Extract real animations from given animation (real, script, blend, degree, speed)
        /**
         * \param anim_name is name of animation
         * \param group_id is used to bundle animations that should be started at the same time
         * \param weight is current weight that should influence animation
         * \param recursion_depth is a counter that keeps track of the number of recursion levels
         **/
        void extractAnimations(const std::string &anim_name, int group_id, float weight, int recursion_depth);

        /// Add animation(s) connected to activity to vector of active animations
        /**
         * \param act_name is name of activity
         * \param act_arg is activity arguments
         **/
        RequestResult addActivity(const std::string& act_name, ActRequestArg *act_arg);

        /// Get current activity
        std::string getCurrentActivity();

        /// Get speed of current activity
        float getCurrentSpeed();

        /// Get state of activity - returns UNKNOWN if not known
        /**
         * \param activity is activity to get state for
         **/
        RequestResult getActivityState(const std::string& activity);

        /// Update activity animation vector according to elapsed seconds
        /**
         * \param elapsed_seconds is time in seconds that has passed since last update
         **/
        void update(float elapsed_seconds);
		
    private:

        /// Id of group with largest id
        int m_largest_group_id;

        /// crGroup id of animations currently being played
        int m_active_group_id;

        /// Activity speed, used to make it possible to get the speed of the currently playing animations
        float m_activity_speed;

        /// Add animation to list
        /**
         * \param animation is pointer to animation instance
         * \param group_id is used to bundle animations that should be started at the same time
         * \param weight is weight of animation (used for blending)
         **/
        void addAnimation(Animation *animation, int group_id, float weight);

        /// Update currently playing animation with different weight, if animation isn't playing nothing is done
        /**
         * \param animation is pointer to animation instance
         * \param weight is weight of animation (used for blending)
         **/
        void updateAnimation(Animation *animation, float weight);

};

} // namespace end

#endif // _CANIMATIONACTIVITYHANDLER_H
/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/include/rbody/AnimationActivityHandler.h,v $
 * $Revision: 1.2 $ 
 * $Date: 2005/02/09 17:26:54 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: AnimationActivityHandler.h,v $
 * Revision 1.2  2005/02/09 17:26:54  deepone
 * vrutils and cfgscript put into new namespaces
 *
 * Revision 1.1.1.1  2005/02/06 06:52:59  deepone
 * initial sourceforge checkin
 *
 * Revision 1.3  2004/02/03 12:58:02  deepone
 * new Property.h, Body.h and Notify.h in rvrutils
 *
 * Revision 1.2  2003/10/31 13:21:45  andersb
 * Added ReplicantBodyMgr class to handle instances of CreBody
 * Added .char files for specifying individual characters (CreBody)
 * Added OSG plugin for reading char files
 *
 * Revision 1.1  2002/06/20 12:51:50  sunna
 * Restructured directories
 *
 * Revision 1.3  2002/04/09 15:31:27  sunna
 * Actual speed of each animation within each activity is set
 * Added getCurrentSpeed()
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



