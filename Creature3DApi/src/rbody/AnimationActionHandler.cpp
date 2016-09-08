// $Id: AnimationActionHandler.cpp,v 1.2 2005/02/09 17:26:56 deepone Exp $

#include "rbody/AnimationActionHandler.h"
#include "CRCore/crNotify.h"
using namespace rbody;


/*----------------------------------------------------------------------------
  Constructor
  ----------------------------------------------------------------------------*/
AnimationActionHandler::AnimationActionHandler()
{

}


/*----------------------------------------------------------------------------
  Destructor
  ----------------------------------------------------------------------------*/
AnimationActionHandler::~AnimationActionHandler()
{

}

/*----------------------------------------------------------------------------
  Add animation to vector
  ----------------------------------------------------------------------------*/
void AnimationActionHandler::addAnimation(Animation *animation, float start_time, float weight)
{

    // Create new active animation instance
    AnimationActive *active_anim = new AnimationActive();

    // Set attributes of active animations
    active_anim->setActName(m_current_act_name);
    active_anim->setWeight(weight);
    active_anim->setCal3dId(animation->getCal3dId());
    active_anim->setIsLooped(animation->isLooped());

    // Get and set duration
    float duration = m_cal3d_model->getCoreModel()->getCoreAnimation(animation->getCal3dId())->getDuration();
    active_anim->setDuration(duration);

    // Set current time to 0
    active_anim->setCurrentTime(0.0f);

    // Set start time
    active_anim->setStartTime(start_time);

    // Set end time to duration - transition time (must be greater or equal to zero)
    float end_time = duration - m_transition_time;

    if (end_time < 0)
        end_time = 0.0f;

    active_anim->setEndTime(end_time);

    // Add active animation to active animations' list
    m_active_anim_vect.push_back(active_anim);

}


/*----------------------------------------------------------------------------
  Extract real animations from given animation (real, script, blend, degree, speed)
  ----------------------------------------------------------------------------*/
void AnimationActionHandler::extractAnimations(const std::string& anim_name, float start_time, float weight, int recursion_depth)
{

    // Limit recursion depth to find loops
    if (recursion_depth > 10) {
        CRCore::notify(CRCore::WARN) <<"AnimationActionHandler::extractAnimations(): Warning: Recursion depth exceeds 10, probably a loop somewhere" << std::endl;
        return;
    }

    AnimationBase *animation = m_core_body->getAnimation(anim_name);

    if (animation == NULL) {
        CRCore::notify(CRCore::WARN) <<"AnimationActionHandler::extractAnimations(): Warning: Animation with name "<< anim_name.c_str() <<" requested but not loaded" << std::endl;
        return;
    }

    // Real core animation
    if (animation->getType() == AnimationBase::REAL) {

        // Add or update animation
        addAnimation((Animation *) animation, start_time, weight);

        return;
    }

    // Degree or speed animation
    if (animation->getType() == AnimationBase::DEGREE || animation->getType() == AnimationBase::SPEED) {

        AnimationCombine *anim = (AnimationCombine *) animation;

        // If type of activity (not type of this animation, although it most of the time is the same) 
        // is degree or speed, use calculated blend weights
        // Else, set animation weights to 0.5 (temporary solution)

        if (m_anim_type == AnimationBase::DEGREE || m_anim_type == AnimationBase::SPEED) {

            // Recursive call, extract all animations that degree or speed animations contain
            extractAnimations(anim->getAnimationNameAt(0), start_time, m_anim_weights_vect[0] * weight, recursion_depth+1);
            extractAnimations(anim->getAnimationNameAt(1), start_time, m_anim_weights_vect[1] * weight, recursion_depth+1);
        } 

        else {

            // This case can only appear if scripted/blend and degree/speed animations are combined
            // ie, scripted or blended animations that contain degree or speed animations...bad combination

            CRCore::notify(CRCore::WARN) <<"AnimationActionHandler::extractAnimations(): Warning: Degree or speed animation found in another animation type -- bad combination" << std::endl;

            extractAnimations(anim->getAnimationNameAt(0), start_time, 0.5f, recursion_depth+1);
            extractAnimations(anim->getAnimationNameAt(1), start_time, 0.5f, recursion_depth+1);
        }

        return;

    }

    // Blend animation
    if (animation->getType() == AnimationBase::BLEND) {

        AnimationBlend *anim = (AnimationBlend *) animation;

        // Use given blend weights if this is the first animation (not nested within other animation types)
        if (m_anim_type == AnimationBase::BLEND && recursion_depth == 0) {

            // Make sure that blend weights are given
            if (m_anim_weights_vect.size() == anim->getNrOfAnimations()) {

                // Loop through all animations in blended animation and adjust weight
                for (int i =0; i < anim->getNrOfAnimations(); i++)
                    extractAnimations(anim->getAnimationNameAt(i), start_time, m_anim_weights_vect[i] * weight, recursion_depth+1);

                return;
            }
        }

        // If no blend weights are given or if the blended animation is nested with other animation types
        // use default blend weights

        for (int i =0; i < anim->getNrOfAnimations(); i++)
            extractAnimations(anim->getAnimationNameAt(i), start_time, anim->getAnimationWeightAt(i) * weight, recursion_depth+1);

        return;
    }

    // Script animation
    if (animation->getType() == AnimationBase::SCRIPT) {

        AnimationScript *anim = (AnimationScript *) animation;

        // Loop through all animations in scripted animation
        // Update start time accordingly

        for (int i =0; i < anim->getNrOfAnimations(); i++)
            extractAnimations(anim->getAnimationNameAt(i), anim->getAnimationStartTimeAt(i), weight, recursion_depth+1);

        return;
    }

}


/*----------------------------------------------------------------------------
  Add animation(s) connected to action vector of active animations
  ----------------------------------------------------------------------------*/
RequestResult AnimationActionHandler::addAction(const std::string& act_name, ActRequestArg *act_arg, bool force)
{
    std::list<AnimationBase *> act_anim_list;	
    ActStatus action_status = AnimationHandler::ADD;

    m_current_act_name = act_name;

    std::list<std::string> conf_act_list;
    std::list<std::string>::iterator act_list_iter;

    // Check for conflicting active actions
    // If conflicts, check force parameter, if it isn't set, return conflicts
    // else, end all actions that conflicts and move on
    if (!m_active_anim_vect.empty()) {

        if (getConflictingActions(act_name, conf_act_list)) {
            if (!force) 
                return ::rbody::CONFLICTS;
            else {
                for (act_list_iter = conf_act_list.begin(); act_list_iter != conf_act_list.end(); act_list_iter++)				
                    stopAction(*act_list_iter);
            }
        }
    }

    std::vector<AnimationActive *>::iterator anim_vect_iter;

    // Loop through all actions to see if action is active already
    for (anim_vect_iter = m_active_anim_vect.begin(); anim_vect_iter != m_active_anim_vect.end(); anim_vect_iter++)  {

        // If action that animation is connected to is equal to given action
        // set status to update
        if ((*anim_vect_iter)->getActName() == m_current_act_name) {

            action_status = AnimationHandler::UPDATE;

            break;
        }

    }

    // Get animations with given action name
    m_core_body->getAnimationsWithActName(m_current_act_name, act_anim_list);

    // Select one animation with given action name
    AnimationBase *animation = selectAnimationFromAct(act_anim_list, act_arg);

    // Make sure animation exists
    if (!animation) {
        CRCore::notify(CRCore::WARN) << __FILE__ << " Line " << __LINE__ << " Warning: Requested animation does not exist: " << m_current_act_name << std::endl;		
        return ::rbody::UNKNOWN;
    }

    // Make sure the weights' vector is empty
    m_anim_weights_vect.clear();

    // Get type of action to be added
    m_anim_type = animation->getType();


    // Real core animation
    if (m_anim_type == AnimationBase::REAL) {

        // A real animation cannot be updated
        if (action_status == AnimationHandler::ADD)
            extractAnimations(animation->getName(), 0.0f, 1.0f, 0);

        return ::rbody::SUCCESS;
    }

    // Script
    if (m_anim_type == AnimationBase::SCRIPT) {

        // A script animation cannot be updated
        if (action_status == AnimationHandler::ADD)
            extractAnimations(animation->getName(), 0.0f, 1.0f, 0);

        return ::rbody::SUCCESS;
    }

    // Degree
    if (m_anim_type == AnimationBase::DEGREE) {

        AnimationDegree *anim = (AnimationDegree *) animation;

        float degree;

        if (act_arg->hasDirection()) {

            CRCore::crVector3 dir;

            act_arg->getDirection(dir);

            degree = convertDirVectorToDegree(dir);

        } else if (act_arg->hasDegree()) {

            degree = act_arg->getDegree();

        } else {

            // Use default. Temporary solution, set degree to first animation's degree
            degree = anim->getAnimationDegreeAt(0);

        }

        float weight1, weight2;

        // Get weights
        convertValueToWeights(degree, anim->getAnimationDegreeAt(0), anim->getAnimationDegreeAt(1), weight1, weight2);

        // Set weights in weight vector
        m_anim_weights_vect.push_back(weight1);
        m_anim_weights_vect.push_back(weight2);

        // If update, remove old action
        if (action_status == AnimationHandler::UPDATE)
            stopAction(m_current_act_name);

        extractAnimations(anim->getName(), 0.0f, 1.0f, 0);

        return ::rbody::SUCCESS;
    }

    // Speed
    if (m_anim_type == AnimationBase::SPEED) {

        AnimationSpeed *anim = (AnimationSpeed *) animation;

        float speed;

        if (!act_arg->hasSpeed()) {

            // If no speed is given, use default
            speed = anim->getDefaultSpeed();

        } else {

            speed = act_arg->getSpeed();

        }

        float weight1, weight2;

        // Get weights
        convertValueToWeights(act_arg->getSpeed(), anim->getAnimationSpeedAt(0), anim->getAnimationSpeedAt(1), weight1, weight2);

        // Set weights in weight vector
        m_anim_weights_vect.push_back(weight1);
        m_anim_weights_vect.push_back(weight2);

        // If update, remove old action
        if (action_status == AnimationHandler::UPDATE)
            stopAction(m_current_act_name);

        extractAnimations(anim->getName(), 0.0f, 1.0f, 0);

        return ::rbody::SUCCESS;
    }

    // Blend
    if (m_anim_type == AnimationBase::BLEND) {

        AnimationBlend *anim = (AnimationBlend *) animation;


        // If all weights for animation are given, use them
        // else, use default

        if (act_arg->getNrOfWeights() == anim->getNrOfAnimations()) {

            // Add all weights to weight vector
            for (int i = 0; i < act_arg->getNrOfWeights(); i++)
                m_anim_weights_vect.push_back(act_arg->getWeightAt(i));

        }

        // If update, remove old action
        if (action_status == AnimationHandler::UPDATE)
            stopAction(m_current_act_name);

        extractAnimations(anim->getName(), 0.0f, 1.0f, 0);

        return ::rbody::SUCCESS;
    }

    return ::rbody::UNKNOWN;

}


/*----------------------------------------------------------------------------
  Get conflicting actions
  ----------------------------------------------------------------------------*/
bool AnimationActionHandler::getConflictingActions(const std::string& action, std::list<std::string> &action_list)
{

    // Get action conflicts object
    ActionConflicts *action_conflicts = m_core_body->getActionConflicts();

    // Take fast way out if conflicting actions isn't set
    if (action_conflicts == NULL)
        return false;

    std::list<std::string> conf_act_list;

    // Get conflicting actions for given action name
    action_conflicts->getConflictingActions(action, conf_act_list);

    // Exit if conflict list or active action vector are empty
    if (conf_act_list.empty() || m_active_anim_vect.empty())
        return false;

    int nr_of_conflicts = conf_act_list.size();

    // Now check if any of these conflicting actions are active

    std::vector<AnimationActive *>::iterator anim_vect_iter;
    std::list<std::string>::iterator act_list_iter;
    bool b_conflicts = false;

    // Loop through all active animations
    for (anim_vect_iter = m_active_anim_vect.begin(); anim_vect_iter != m_active_anim_vect.end(); anim_vect_iter++)  {

        // Loop through all conflicting actions
        for (act_list_iter = conf_act_list.begin(); act_list_iter != conf_act_list.end(); act_list_iter++)  {

            // If action that animation is connected to is equal to given action
            // add it to conflicting action list
            if ((*anim_vect_iter)->getActName() == (*act_list_iter)) {

                action_list.push_back(action);

                b_conflicts = true;

                // Keep track if we have checked all conflicting actions and can exit
                if (--nr_of_conflicts == 0) 
                    return true;
            }
        }
    }

    // Return true if conflicting actions found
    return b_conflicts;

}


/*----------------------------------------------------------------------------
  Get state of action
  ----------------------------------------------------------------------------*/
RequestResult AnimationActionHandler::getActionState(const std::string& action)
{

    // Make sure action exists  
    if (!m_core_body->actNameExists(action))
        return ::rbody::UNKNOWN;

    std::vector<AnimationActive *>::iterator anim_vect_iter;

    RequestResult temp_state = ::rbody::INACTIVE;

    // Loop through all active animations
    for (anim_vect_iter = m_active_anim_vect.begin(); anim_vect_iter != m_active_anim_vect.end(); anim_vect_iter++)  {

        // If we find animation connected to action we are looking for, check if it's playing or is queued 
        // (ie, current time < start time ). since several animations can be connected to same action
        // we must loop through all animations before we know for sure it isn't active
        // It can actually both be playing and be queued, if so, return playing

        if ((*anim_vect_iter)->getActName() == action) {

            // If animation is currently playing, return active, else set it temporary to queued
            if ((*anim_vect_iter)->isActivated()) {

                return ::rbody::ACTIVE;
            }
            else {

                temp_state = ::rbody::QUEUED;
            }

        }
    }

    // Return temporary state if it isn't active
    return temp_state;

}


/*----------------------------------------------------------------------------
  Stop action 
  ----------------------------------------------------------------------------*/
RequestResult AnimationActionHandler::stopAction(const std::string& action)
{

    // Stop action = set all associated animations to non-looping

    std::vector<AnimationActive *>::iterator anim_vect_iter;

    // Loop through all active animations
    for (anim_vect_iter = m_active_anim_vect.begin(); anim_vect_iter != m_active_anim_vect.end(); anim_vect_iter++)  {

        // If we find animation connected to action we are looking for, set it to non-looping
        if ((*anim_vect_iter)->getActName() == action) {

            if((*anim_vect_iter)->isLooped() == 1) (*anim_vect_iter)->setIsLooped(0);
        }

    }

    // Return success (even if we didn't find any animations connected to given action)
    return ::rbody::SUCCESS;

}


/*----------------------------------------------------------------------------
  Update activity animation vector according to elapsed seconds
  ----------------------------------------------------------------------------*/
bool AnimationActionHandler::update(float elapsed_seconds)
{
	bool stopact = false;
    std::vector<AnimationActive *>::iterator anim_vect_iter;

    AnimationActive *anim;

    float new_cur_time;

    // Loop through all active animations

    anim_vect_iter = m_active_anim_vect.begin();

    while (anim_vect_iter != m_active_anim_vect.end()) {

        anim = *anim_vect_iter;

        new_cur_time = anim->getCurrentTime() + elapsed_seconds;

        // Execute action if it isn't activated (read hasn't started) and current time
        // has passed start time, or if it's loooping and we have passed its end time

        if (((!anim->isActivated()) && (new_cur_time >= anim->getStartTime()))

                || (anim->isLooped()==1 && (new_cur_time >= anim->getEndTime()))) {

            // Execute action
            bool success = m_cal3d_model->getMixer()->executeAction(anim->getCal3dId(), m_transition_time, m_transition_time, anim->getWeight(),anim->isLooped() != 1);
            assert(success);

            // Since we just started the animation its current time is zero
            anim->setCurrentTime(/*0.0f*/new_cur_time);

            // Animation has started - activate
            anim->setIsActivated(true);

            // Go to next animation in vector
            anim_vect_iter++;
        }

        // Remove animation from queue if it's non looping and we have passed its end time

        else if ((anim->isLooped()!=1) && (new_cur_time >= anim->getEndTime()/*-0.05f*/)) {

			//m_cal3d_model->getMixer()->executeAction(anim->getCal3dId(),0.0f, anim->getDuration());
			m_cal3d_model->getMixer()->removeAction(anim->getCal3dId());
			stopact = true;
            // Delete animation
            delete (*anim_vect_iter);

            // Remove animation from vector and go to next element in vector
            anim_vect_iter = m_active_anim_vect.erase(anim_vect_iter);
        } 

        // If none of the conditions above is meet, just update current time

        else {

            // Update current time
            anim->setCurrentTime(new_cur_time);

            // Go to next animation in vector
            anim_vect_iter++;
        }

    } // while
	return stopact;
}

/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/src/rbody/AnimationActionHandler.cpp,v $
 * $Revision: 1.2 $ 
 * $Date: 2005/02/09 17:26:56 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12


 * $Log: AnimationActionHandler.cpp,v $
 * Revision 1.2  2005/02/09 17:26:56  deepone
 * vrutils and cfgscript put into new namespaces
 *
 * Revision 1.1.1.1  2005/02/06 06:52:59  deepone
 * initial sourceforge checkin
 *
 * Revision 1.7  2004/02/03 12:58:04  deepone
 * new Property.h, Body.h and Notify.h in rvrutils
 *
 * Revision 1.6  2003/11/06 08:24:38  andersb
 * Removed the use of rbody::crVector3
 *

 * Revision 1.5  2003/11/03 14:22:43  deepone
 * compiling on linux with CRCore plugin
 *
 * Revision 1.4  2003/10/31 13:21:48  andersb
 * Added ReplicantBodyMgr class to handle instances of CreBody
 * Added .char files for specifying individual characters (CreBody)
 * Added OSG plugin for reading char files
 *
 * Revision 1.3  2003/06/17 08:58:28  Anders Backman
 * *** empty log message ***
 *
 * Revision 1.2  2002/11/11 13:45:38  sunna
 * no message
 *
 * Revision 1.1  2002/06/20 13:23:11  sunna
 * no message
 *
 * Revision 1.6  2002/04/18 10:05:45  sunna
 * getActivityState()/getActionState() is now returning unkonw if act doesn't exist
 *
 * Revision 1.5  2002/04/02 13:44:40  sunna
 * End time greater or equal to zero
 *
 * Revision 1.4  2002/03/21 09:37:28  sunna
 * Small changes in method headers
 *
 * Revision 1.3  2002/03/20 15:07:29  sunna
 * If no animation is connected to a given activity/action, addAction() and addActivity() are now returning UNKNOWN
 *
 * Revision 1.2  2002/03/15 08:55:08  andersb
 * no message
 *
 * Revision 1.1.1.1  2002/03/14 20:48:40  andersb
 * First import as ReplicantBody
 *
 * Revision 1.2  2002/02/19 15:46:12  andersb
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2002/01/04 08:22:40  andersb
 * Initial version
 *
 ----------------------------------------------------------------------------*/
