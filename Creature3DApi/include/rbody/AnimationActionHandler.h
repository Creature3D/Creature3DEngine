#ifndef _CANIMATIONACTIONHANDLER_H
#define _CANIMATIONACTIONHANDLER_H

#include "rbody/global.h"
#include <string>

#include "rbody/AnimationHandler.h"
#include "rbody/ActRequestArg.h"
#include "rbody/Animation.h"
#include "rbody/export.h"

#include <rbody/ActionRequest.h>

namespace rbody {

/// Handles all active animations
class  REPLICANTBODY_API AnimationActionHandler : public AnimationHandler {

    public:

        /// Constructor
        AnimationActionHandler();

        /// Destructor
        virtual ~AnimationActionHandler();

        /// Extract real animations from given animation (real, script, blend, degree, speed)
        /**
         * \param anim_name is name of animation
         * \param start_time is current start time for animation
         * \param weight is current weight that should influence animation
         * \param recursion_depth is a counter that keeps track of the number of recursion levels
         **/
        void extractAnimations(const std::string& anim_name, float start_time, float weight, int recursion_depth);

        /// Add animation(s) connected to action to vector of active animations
        /**
         * \param act_name is name of action
         * \param act_arg is action arguments
         * \param force tells whether active conflicting actions should end or not
         **/
        RequestResult addAction(const std::string& act_name, ActRequestArg *act_arg, bool force);

        /// Get conflicting actions
        /**
         * Check if any of the active actions conflicts with given action
         * \param action is action to get conflicting actions for
         * \param action_list is referens list where conflicting actions are placed
         * \return true if any conflicting actions are found
         **/
        bool getConflictingActions(const std::string& action, std::list<std::string> &action_list);

        /// Get state of action
        /**
         * \param action is action to get state for
         **/
        RequestResult getActionState(const std::string& action);

        /// Stop action
        /**
         * \param action is action to stop
         **/
        RequestResult stopAction(const std::string& action);

        /// Update action animation vector according to elapsed seconds
        /**
         * \param elapsed_seconds is time in seconds that has passed since last update
         **/
        bool update(float elapsed_seconds);

    private:

        /// Add animation to list
        /**
         * \param animation is pointer to animation instance
         * \param start_time is time in seconds when animation should start
         * \param weight is weight of animation (used for blending)
         **/
        void addAnimation(Animation *animation, float start_time, float weight);

};

} // namespace end

#endif // _CANIMATIONACTIONHANDLER_H
/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rbody/AnimationActionHandler.h,v $
* $Revision: 1.2 $ 
* $Date: 2005/02/09 17:26:54 $
* $Author: deepone $ 
* $Locker:  $

* Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

 Created: 2001-12-12

* $Log: AnimationActionHandler.h,v $
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


