//Creature3D
//modify by Îâ²Æ»ª
#include "rbody/AnimationHandler.h"

using namespace rbody;

/*----------------------------------------------------------------------------
  Constructor
  ----------------------------------------------------------------------------*/
AnimationHandler::AnimationHandler()
{

}


/*----------------------------------------------------------------------------
  Destructor
  ----------------------------------------------------------------------------*/
AnimationHandler::~AnimationHandler()
{
	std::vector<AnimationActive *>::iterator anim_vect_iter;
	for (anim_vect_iter = m_active_anim_vect.begin(); anim_vect_iter != m_active_anim_vect.end(); anim_vect_iter++)  
    {
		//m_cal3d_model->getMixer()->removeAction((*anim_vect_iter)->getCal3dId());
		delete (*anim_vect_iter);
    }
    m_active_anim_vect.clear();
}

void AnimationHandler::deleteAnimation(Animation *animation)
{
	std::vector<AnimationActive *>::iterator anim_vect_iter;

	// Loop through all active animations
	for (anim_vect_iter = m_active_anim_vect.begin(); anim_vect_iter != m_active_anim_vect.end(); anim_vect_iter++)  
	{
		if ((*anim_vect_iter)->getCal3dId() == animation->getCal3dId()) 
		{
			m_cal3d_model->getMixer()->removeAction(animation->getCal3dId());
			delete (*anim_vect_iter);
			m_active_anim_vect.erase(anim_vect_iter);
			break;
		}
	}
}

/*----------------------------------------------------------------------------
  Init
  ----------------------------------------------------------------------------*/
void AnimationHandler::onInit(CoreBody *core_body, CalModel *cal3d_model, float transition_time)
{

    // Set core body
    m_core_body = core_body;

    // Set model
    m_cal3d_model = cal3d_model;

    // Set transition time
    m_transition_time = transition_time;

}


/*----------------------------------------------------------------------------
  Set transition time (time when one animation is fading out and another is 
  fading in)
  ----------------------------------------------------------------------------*/
void AnimationHandler::setTransitionTime(float transition_time)
{

    // Set transition time
    m_transition_time = transition_time;

}


/*----------------------------------------------------------------------------
  Select one animation from animation list (depending on request arguments)
  One activity/action can be connected to several animations. This
  method selects which animation to use. Returns index to that animation in list.
  ----------------------------------------------------------------------------*/
AnimationBase *AnimationHandler::selectAnimationFromAct(const std::list<AnimationBase *>& animation_list, ActRequestArg *act_arg)
{

    // TODO: Now we're just returning first animation in list if list isn't empty

    if (animation_list.empty())
        return NULL;

    return *(animation_list.begin());

}


/*----------------------------------------------------------------------------
  Convert directional vector to degrees and return it
  ----------------------------------------------------------------------------*/
float AnimationHandler::convertDirVectorToDegree(CRCore::crVector3 direction)
{

    // TODO: convert vector to angle in radians between vector and x-axis
    return 0.0f;

}


/*----------------------------------------------------------------------------
  Convert given value to weights
  Degree and speed animations consist of two animations, which represent one 
  degree (direction) or speed (how fast the animation plays) each. 
  This method sets the weights of these animations to get the wanted 
  direction/speed of the resulting blended animation.
  ----------------------------------------------------------------------------*/
void AnimationHandler::convertValueToWeights(float value, float value_limit1, float value_limit2, float &weight1, float &weight2)
{

    float length, sub_length;

    // Make sure value is in interval [value_limit1, value_limit2] or [value_limit2, value_limit1]

    if (value_limit1 < value_limit2) {

        if (value < value_limit1)
            value = value_limit1;
        if (value > value_limit2)
            value = value_limit2;
    }
    else {

        if (value > value_limit1)
            value = value_limit1;
        if (value < value_limit2)
            value = value_limit2;
    }

    // Calculate length of interval
    length = fabs(value_limit1 - value_limit2);

    // Calculte length from limit1 to point
    sub_length = fabs(value_limit1 - value);

    // Calculate weight 1 from lengths
    weight1 = 1 - sub_length/length;

    // Weight 2 is 1 minus weight 1
    weight2 = 1 - weight1;

}

/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/src/rbody/AnimationHandler.cpp,v $
 * $Revision: 1.1.1.1 $ 
 * $Date: 2005/02/06 06:52:59 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: AnimationHandler.cpp,v $
 * Revision 1.1.1.1  2005/02/06 06:52:59  deepone
 * initial sourceforge checkin
 *
 * Revision 1.2  2003/11/06 08:24:38  andersb
 * Removed the use of rbody::crVector3
 *
 * Revision 1.1  2002/06/20 13:23:11  sunna
 * no message
 *
 * Revision 1.5  2002/04/02 13:42:12  sunna
 * Added setTransitionTime()
 *
 * Revision 1.4  2002/03/21 09:37:28  sunna
 * Small changes in method headers
 *
 * Revision 1.3  2002/03/20 15:04:12  sunna
 * Removed method convertSpeedToWeights() since convertValueToWeights() does the same thing
 *
 * Revision 1.2  2002/03/15 08:55:08  andersb
 * no message
 *
 * Revision 1.1.1.1  2002/03/14 20:48:40  andersb
 * First import as ReplicantBody
 *
 * Revision 1.1.1.1  2002/01/04 08:22:40  andersb
 * Initial version
 *
 ----------------------------------------------------------------------------*/


