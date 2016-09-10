//Creature3D
//Modified by Wucaihua
#ifndef _CANIMATIONHANDLER_H
#define _CANIMATIONHANDLER_H

#include "rbody/global.h"
#include <string>
#include <vector>

#include <cal3d/cal3d.h>

#include "rbody/AnimationActive.h"
#include "rbody/ActRequestArg.h"
#include "rbody/AnimationBase.h"
#include "rbody/AnimationDegree.h"
#include "rbody/AnimationSpeed.h"
#include "rbody/AnimationBlend.h"
#include "rbody/Animation.h"
#include "rbody/CoreBody.h"


#include <CRCore/crVector3.h>


namespace rbody {

/// Handles all active animations
class REPLICANTBODY_API AnimationHandler {

    public:

        /// Activity/action status
        enum ActStatus {
            ADD, /**< means the activty/action should be added */				
            UPDATE /**< means the action/activity should be updated */
        };

    public:

        /// Constructor
        AnimationHandler();

        /// Destructor
        virtual ~AnimationHandler();

        /// Init
        /**
         * \param cal3d_model is pointer to cal3d model
         * \param core_body is pointer to core body
         * \param transition_time is time during the blend in/out process of animations
         **/
        void onInit(CoreBody *core_body, CalModel *cal3d_model, float transition_time);

        /// Set transition time (time when one animation is fading out and another is fading in)
        /**
         * \param transition_time is time during the blend in/out process of animations
         **/
        void setTransitionTime(float transition_time);

        /// Select one animation from animation list (depending on request arguments)
        /**
         * One activity/action can be connected to several animations. This
         * method selects which animation to use. Returns pointer to animation.
         * \param animation_list is list with animations connected to same activity/action to choose from
         * \param act_arg is class with request arguments, for example intensity
         **/
        AnimationBase *selectAnimationFromAct(const std::list<AnimationBase *>& animation_list, ActRequestArg *act_arg);

        /// Convert directional vector to degrees and return it
        /**
         * \param dir is direction vector
         **/
        float convertDirVectorToDegree(CRCore::crVector3 dir);

        /// Convert given value to weights
        /**
         * Degree and speed animations consist of two animations, which represent one 
         * degree (direction) or speed (how fast the animation plays) each. 
         * This method sets the weights of these animations to get the wanted 
         * direction/speed of the resulting blended animation.
         * \param value is the wanted degree/speed of resulting animation
         * \param value_limit1 is representation of animation 1
         * \param value_limit2 is representation of animation 2
         * \param weight1 is resulting weight of animation 1
         * \param weight2 is resulting weight of animation 2
         **/
        void convertValueToWeights(float value, float value_limit1, float value_limit2, float &weight1, float &weight2);

		void deleteAnimation(Animation *animation);
    protected:

        /// Pointer to core body
        CoreBody *m_core_body;

        /// Pointer to cal3d model
        CalModel *m_cal3d_model;

        /// Transition time (used when blending from one animation to another)
        float m_transition_time;

        /// Current activity/action, ie activty/action that is about to be added or updated
        std::string m_current_act_name;

        /// Vector of active animations
        std::vector<AnimationActive *> m_active_anim_vect;

        /// Animation type of current activity/action
        AnimationBase::AnimationType m_anim_type;

        /// List of animation weights
        std::vector<float> m_anim_weights_vect;

        /// Status of activity/action (add or update)
        ActStatus m_act_status;

};

} // namespace end

#endif // _CANIMATIONHANDLER_H
/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/include/rbody/AnimationHandler.h,v $
 * $Revision: 1.1.1.1 $ 
 * $Date: 2005/02/06 06:52:59 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12


 * $Log: AnimationHandler.h,v $
 * Revision 1.1.1.1  2005/02/06 06:52:59  deepone
 * initial sourceforge checkin
 *
 * Revision 1.4  2003/11/12 10:12:11  andersb
 * Body inherits from PropertyContainer again.
 * Moved Direction property out of Body.
 *
 * Revision 1.3  2003/11/11 10:52:06  sunna
 * *** empty log message ***
 *
 * Revision 1.2  2003/10/31 13:21:46  andersb
 * Added ReplicantBodyMgr class to handle instances of CreBody
 * Added .char files for specifying individual characters (CreBody)
 * Added OSG plugin for reading char files
 *
 * Revision 1.1  2002/06/20 12:51:50  sunna
 * Restructured directories
 *
 * Revision 1.5  2002/04/05 17:48:40  deepone
 * include cal3d files as
 * #include <cal3d/cal3d.h>
 * instead of
 * #include "rbody/cal3d.h"
 *
 * Revision 1.4  2002/04/02 13:42:02  sunna
 * Added setTransitionTime()
 *
 * Revision 1.3  2002/03/20 15:04:20  sunna
 * Removed method convertSpeedToWeights() since convertValueToWeights() does the same thing
 *
 * Revision 1.2  2002/03/15 08:55:07  andersb
 * no message
 *
 * Revision 1.1.1.1  2002/03/14 20:48:40  andersb
 * First import as ReplicantBody
 *
 * Revision 1.1.1.1  2002/01/04 08:22:35  andersb
 * Initial version
 *
 ----------------------------------------------------------------------------*/


