#include "rbody/AnimationActivityHandler.h"
#include "CRCore/crNotify.h"

using namespace rbody;
#include <strstream>


/*----------------------------------------------------------------------------
  Constructor
  ----------------------------------------------------------------------------*/
AnimationActivityHandler::AnimationActivityHandler()
{

    // No animations are active
    m_active_group_id = -1;
    m_largest_group_id = -1;

    // Set speed to 0
    m_activity_speed = 0;

}


/*----------------------------------------------------------------------------
  Destructor
  ----------------------------------------------------------------------------*/
AnimationActivityHandler::~AnimationActivityHandler()
{

}

/*----------------------------------------------------------------------------
  Add animation to vector
  ----------------------------------------------------------------------------*/
void AnimationActivityHandler::addAnimation(Animation *animation, int group_id, float weight)
{

    // New animation in vector, animations before with different group id must end as soon as possible...

    if (!m_active_anim_vect.empty()) {

        // Get size of active animations' vector
        int size = m_active_anim_vect.size() -1;

        // Get id of last animation in vector
        int id = m_active_anim_vect[size]->getGroupId();

        // Only animations with different group id should end
        if (id != group_id) {
            // Get end time for animation before in vector
			float end_time;
			//if(!animation->isLooped() || !m_active_anim_vect[size]->isLooped())
			//{//要播放的动作不是循环动作或者当前动作不是循环动作，等待当前动作做完
   //             end_time = m_active_anim_vect[size]->getDuration() - m_transition_time;
			//}
			//else
			//{//立即切换，当前动作的剩余部分做动作融合
                end_time = 0.0f;
			//}
			// Make sure end time is greater or equal to zero
            //if (end_time < 0)
            //    end_time = 0;
            // If several animations have the same group id, set their end time too
            for (int i = size; i >= 0; i--) {

                // Take the fast way out as soon as we found an animation with different different group id
                if (m_active_anim_vect[i]->getGroupId() != id)
                    break;

                // Set end time
                m_active_anim_vect[i]->setEndTime(end_time);
            }
        }
    } 

    // Create new active animation instance
    AnimationActive *active_anim = new AnimationActive();

    // Set attributes of active animations
    active_anim->setActName(m_current_act_name);
    active_anim->setGroupId(group_id);
    active_anim->setWeight(weight);
    active_anim->setCal3dId(animation->getCal3dId());
    active_anim->setIsLooped(animation->isLooped());

    // Set speed, used to make it possbile to get the current speed
    // m_activity_speed is only set if the animation (in current activity) is not of type speed
    if (m_activity_speed != 0)
        active_anim->setSpeed(m_activity_speed);
    else
        active_anim->setSpeed(animation->getSpeed());

    // Get and set duration
    float duration = m_cal3d_model->getCoreModel()->getCoreAnimation(animation->getCal3dId())->getDuration();
    active_anim->setDuration(duration);
    // Set current time to 0
    active_anim->setCurrentTime(0.0f);

    // If animation is looped, set end time to -1, else set end time to duration
    if (active_anim->isLooped()==1)
        active_anim->setEndTime(-1.0f);
    else {
        float end_time = duration - m_transition_time;

        // Make sure end time is greater or equal to zero
        if (end_time < 0)
            end_time = 0.0f;
        active_anim->setEndTime(end_time);
    }

    // Add active animation to active animations' list
    m_active_anim_vect.push_back(active_anim);

    // Update largest_group_id if nescessary
    if (group_id > m_largest_group_id)
        m_largest_group_id = group_id;

}


/*----------------------------------------------------------------------------
  Update currently playing animation with different weight
  If animation isn't playing nothing is done
  ----------------------------------------------------------------------------*/
void AnimationActivityHandler::updateAnimation(Animation *animation, float weight)
{

    std::vector<AnimationActive *>::iterator anim_vect_iter;

    // Loop through all active animations
    for (anim_vect_iter = m_active_anim_vect.begin(); anim_vect_iter != m_active_anim_vect.end(); anim_vect_iter++)  {

        // If we find animation we are looking for, update it with new weight
        if ((*anim_vect_iter)->getCal3dId() == animation->getCal3dId()) {

            // Set new weight of animtion
            (*anim_vect_iter)->setWeight(weight);

            // Transition into animation with new weight
            m_cal3d_model->getMixer()->blendCycle((*anim_vect_iter)->getCal3dId(), weight, m_transition_time);

            // Exit loop
            break;
        }
    }

}


/*----------------------------------------------------------------------------
  Extract real animations from given animation (real, script, blend, degree, speed)
  ----------------------------------------------------------------------------*/
void AnimationActivityHandler::extractAnimations(const std::string& anim_name, int group_id, float weight, int recursion_depth)
{

    // Limit recursion depth to find loops
    if (recursion_depth > 10) {
        CRCore::notify(CRCore::WARN) <<"AnimationActivityHandler::extractAnimations(): Warning: Recursion depth exceeds 10, probably a loop somewhere" << std::endl;
        return;
    }

    AnimationBase *animation = m_core_body->getAnimation(anim_name);

    if (animation == NULL) {
        CRCore::notify(CRCore::WARN) <<"AnimationActivityHandler::extractAnimations(): Warning: Animation with name "<< anim_name.c_str() <<" requested but not loaded" << std::endl;
        return;
    }

    // Real core animation
    if (animation->getType() == AnimationBase::REAL) {

        // Add or update animation
        if (m_act_status == AnimationHandler::ADD)
            addAnimation((Animation *) animation, group_id, weight);
        else
            updateAnimation((Animation *) animation, weight);

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
            extractAnimations(anim->getAnimationNameAt(0), group_id, m_anim_weights_vect[0] * weight, recursion_depth+1);
            extractAnimations(anim->getAnimationNameAt(1), group_id, m_anim_weights_vect[1] * weight, recursion_depth+1);
        } 

        else {

            // This case can only appear if scripted/blend and degree/speed animations are combined
            // ie, scripted or blended animations that contain degree or speed animations...bad combination

            CRCore::notify(CRCore::WARN) <<"AnimationActivityHandler::extractAnimations(): Warning: Degree or speed animation found in another animation type -- bad combination" << std::endl;

            extractAnimations(anim->getAnimationNameAt(0), group_id, 0.5f, recursion_depth+1);
            extractAnimations(anim->getAnimationNameAt(1), group_id, 0.5f, recursion_depth+1);
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
                    extractAnimations(anim->getAnimationNameAt(i), group_id, m_anim_weights_vect[i] * weight, recursion_depth+1);

                return;
            }
        }

        // If no blend weights are given or if the blended animation is nested with other animation types
        // use default blend weights

        for (int i =0; i < anim->getNrOfAnimations(); i++)
            extractAnimations(anim->getAnimationNameAt(i), group_id, anim->getAnimationWeightAt(i) * weight, recursion_depth+1);

        return;
    }

    // Script animation
    if (animation->getType() == AnimationBase::SCRIPT) {

        AnimationCombine *anim = (AnimationCombine *) animation;

        // Loop through all animations in scripted animation
        // Increase group_id since these animation should be queued

        for (int i =0; i < anim->getNrOfAnimations(); i++) {
            extractAnimations(anim->getAnimationNameAt(i), group_id, weight, recursion_depth+1);
            group_id++;
        }

        return;
    }

}


/*----------------------------------------------------------------------------
  Add animation(s) connected to activity to vector of active animations
  ----------------------------------------------------------------------------*/
RequestResult AnimationActivityHandler::addActivity(const std::string& act_name, ActRequestArg *act_arg)
{
    std::list<AnimationBase *> act_anim_list;

    std::vector<AnimationActive *>::iterator anim_vect_iter;

    std::string last_act_name = "", in_between_act = "";

    // Check if this activity is active already - if so, set status to update
    if (!m_active_anim_vect.empty()) {

        // Get last animation in active animation list
        anim_vect_iter = m_active_anim_vect.end();
        anim_vect_iter--;

        last_act_name = (*anim_vect_iter)->getActName();
    }

    if (last_act_name == act_name) {
        m_act_status = AnimationHandler::UPDATE;
    } else {
        m_act_status = AnimationHandler::ADD;
        m_largest_group_id++;
    }

    // Get in-between activity
    if (!last_act_name.empty())
        in_between_act = m_core_body->getInBetweenActivity(last_act_name,act_name);

    RequestResult status;

    int no_act;

    // Set nr of activities to be added
    in_between_act.empty() ? no_act = 1 : no_act = 2; 

    // Loop through all activities that should be added (1 or 2)
    for (int i = 0; i < no_act; i++) {

        // Set speed of activity to zero
        // This tells activity handler to set the speed to the default speed of the animations
        // If the requested activity is a speed activity, the speed instead will be set to the given speed
        // For example, if the activity is a script animation, the speed for each animation in the
        // script will be set to its default speed. The same goes for blend and core animations
        // This makes it possible to always get the correct speed in which the character is traveling
        m_activity_speed = 0;

        // Temporary solution
        // Right now the in-between activity uses the same req_arg as the activity to be added
        // In-between activities should only consist of core animations or scripts that do not
        // take any parameters. Therefore this solution will work in the normal case.

        // Set current activity
        if (i == 0 && no_act == 2)
            m_current_act_name = in_between_act;
        else {
            m_current_act_name = act_name;
            if (no_act == 2)
                m_largest_group_id++;
        }

        // Clear animation list
        act_anim_list.clear();

        // Get animations with given activity name
        m_core_body->getAnimationsWithActName(m_current_act_name,act_anim_list);

        // Select one animation with given activity name
        AnimationBase *animation = selectAnimationFromAct(act_anim_list,act_arg);

        // Make sure animation exists
        if (!animation) {
            CRCore::notify(CRCore::WARN) << __FILE__ << " Line " << __LINE__  << " requested animation does not exist: " << m_current_act_name << std::endl;		
            return ::rbody::UNKNOWN;
        }

        // Make sure the weights' vector is empty
        m_anim_weights_vect.clear();

        // Get type of activity to be added
        m_anim_type = animation->getType();


        // Real core animation
        if (m_anim_type == AnimationBase::REAL) {

            // If status is update nothing needs to be done
            if (m_act_status == AnimationHandler::ADD)
                extractAnimations(animation->getName(), m_largest_group_id, 1.0f, 0);

            status = ::rbody::SUCCESS;
            continue;
        }

        // Script
        if (m_anim_type == AnimationBase::SCRIPT) {

            // Cannot update a script animation, set it to add instead
            if (m_act_status == AnimationHandler::UPDATE) {
                m_act_status = AnimationHandler::ADD;
                m_largest_group_id++;
            }

            extractAnimations(animation->getName(), m_largest_group_id, 1.0f, 0);

            status = ::rbody::SUCCESS;
            continue;
        }

        // Degree (not supposed to be used by activities, but the code exists anyway)
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

            extractAnimations(anim->getName(), m_largest_group_id, 1.0f, 0);

            status = ::rbody::SUCCESS;
            continue;
        }

        // Speed
        if (m_anim_type == AnimationBase::SPEED) {

            AnimationSpeed *anim = (AnimationSpeed *) animation;

            // Set speed
            if (!act_arg->hasSpeed()) {

                // Use default
                m_activity_speed = anim->getDefaultSpeed();

            } else
                m_activity_speed = act_arg->getSpeed();

            float weight1, weight2;

            // Get weights
            convertValueToWeights(m_activity_speed, anim->getAnimationSpeedAt(0), anim->getAnimationSpeedAt(1), weight1, weight2);

            // Set weights in weight vector
            m_anim_weights_vect.push_back(weight1);
            m_anim_weights_vect.push_back(weight2);

            extractAnimations(anim->getName(), m_largest_group_id, 1.0f, 0);

            status = ::rbody::SUCCESS;
            continue;
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

            extractAnimations(anim->getName(), m_largest_group_id, 1.0f, 0);

            status = ::rbody::SUCCESS;
            continue;
        }

        status = ::rbody::UNKNOWN;

    } // for

    return status;

}


/*----------------------------------------------------------------------------
  Update activity animation vector according to elapsed seconds
  ----------------------------------------------------------------------------*/
void AnimationActivityHandler::update(float elapsed_seconds)
{

    std::vector<AnimationActive *>::iterator anim_vect_iter;

    AnimationActive *anim;

    float trans_time=0;

    // Loop through all active animations

    anim_vect_iter = m_active_anim_vect.begin();

    while (anim_vect_iter != m_active_anim_vect.end()) {

        anim = *anim_vect_iter;

        // Is the animation active right now?
        if (anim->getGroupId() == m_active_group_id)	{


            // If end time is set the animation is about to end
            if (anim->getEndTime() >= 0.0f) {

                //std::cerr << std::endl << "crGroup id: " << anim->getGroupId() << std::endl;

                //std::cerr << "End time set: " << anim->getEndTime() << std::endl;

                // Calculate time that is left of animation
                float time_left = anim->getEndTime() - (anim->getCurrentTime() + elapsed_seconds);

                // If there isn't any time left of animation
                if (time_left <= 0.0f ) {

                    //std::cerr << "No time left!" << std::endl;

     //               // Calculate transition time
					//if(anim->getEndTime() == 0.0f)
					//{//被立即切换的动作，剩余时间重新计算
					//    time_left = anim->getDuration() - m_transition_time - (anim->getCurrentTime() + elapsed_seconds);
					//}
     //               trans_time = m_transition_time + time_left;

     //               // Make sure transition time isn't negative
     //               if (trans_time < 0.0f)
     //                   trans_time = 0.0f;

                    // Clear animation from mixer
                     m_cal3d_model->getMixer()->clearCycle(anim->getCal3dId(), /*trans_time*/m_transition_time);//剩余时间做动作融合

                    // Delete animation
                    delete (*anim_vect_iter);

                    // Remove animation from vector and go to next element in vector
                    anim_vect_iter = m_active_anim_vect.erase(anim_vect_iter);

                } else {

                    //std::cerr << "Time left: " << time_left << std::cerr;

                    // Update animation with new time
                    anim->setCurrentTime(anim->getCurrentTime() + elapsed_seconds);

                    // Go to next element in vector
                    anim_vect_iter++;
                }

            } else {

                // No end time set, the animation should continue to loop

                // Calculate new time				
                float new_time = anim->getCurrentTime() + elapsed_seconds;

                // Make sure time is in range
                while (new_time > anim->getDuration())
                    new_time = new_time - anim->getDuration();

                // Update animation with new time				
                anim->setCurrentTime(new_time);

                // Go to next element in vector
                anim_vect_iter++;

            }

        } else {

            // Take the fast way out if group id isn't active id since animations are sorted by group id
            break;
        }
    } // while

    if (!m_active_anim_vect.empty()) {

        anim_vect_iter = m_active_anim_vect.begin();

        // If first animation in vector's group id is bigger than active group id
        // no animation is active and it's time to transition into a new animation

        if ((*anim_vect_iter)->getGroupId() > m_active_group_id) {

            m_active_group_id = (*anim_vect_iter)->getGroupId();

            while (anim_vect_iter != m_active_anim_vect.end()) {

                anim = *anim_vect_iter;

                if (anim->getGroupId() == m_active_group_id) {

                    // Transition into animation (trans_time should be same as used before)
					m_cal3d_model->getMixer()->setAnimationTime(anim->getStartTime());
					//m_cal3d_model->getMixer()->setTimeFactor(-1);//设置到播
                    m_cal3d_model->getMixer()->blendCycle(anim->getCal3dId(), anim->getWeight(), /*trans_time*/m_transition_time);

                } else {

                    // Take fast way out since animations are sorted in group id order
                    break;
                }

                // Go to next animation in vector
                anim_vect_iter++;
            }
        }
    } // if not empty

}


/*----------------------------------------------------------------------------
  Get state of activity
  ----------------------------------------------------------------------------*/
RequestResult AnimationActivityHandler::getActivityState(const std::string &activity)
{

    // Make sure activity exists  
    if (!m_core_body->actNameExists(activity))
        return ::rbody::UNKNOWN;

    std::vector<AnimationActive *>::iterator anim_vect_iter;

    // Loop through all active animations
    for (anim_vect_iter = m_active_anim_vect.begin(); anim_vect_iter != m_active_anim_vect.end(); anim_vect_iter++)  {

        // If we find activity we are looking for, check if it's playing or is queued
        // It can actually both be playing and be queued, if so, return playing

        if ((*anim_vect_iter)->getActName() == activity) {

            // If animation is currently playing, return active, else return queued
            if ((*anim_vect_iter)->getGroupId() == m_active_group_id) {

                return ::rbody::ACTIVE;
            }
            else {

                return ::rbody::QUEUED;
            }

        }
    }

    // Return inactive if we didn't find activity in vector
    return ::rbody::INACTIVE;

}


/*----------------------------------------------------------------------------
  Get current activity
  ----------------------------------------------------------------------------*/
std::string AnimationActivityHandler::getCurrentActivity()
{

    std::vector<AnimationActive *>::iterator anim_vect_iter;

    // Loop through all active animations
    for (anim_vect_iter = m_active_anim_vect.begin(); anim_vect_iter != m_active_anim_vect.end(); anim_vect_iter++)  {

        // If animation is currently playing, return name of activity it's connected to
        if ((*anim_vect_iter)->getGroupId() == m_active_group_id) {

            return (*anim_vect_iter)->getActName();
        }

    }

    // Return empty string if no animation and therefore no activity is active
    return "";

}


/*----------------------------------------------------------------------------
  Get speed of current activity
  ----------------------------------------------------------------------------*/
float AnimationActivityHandler::getCurrentSpeed()
{

    std::vector<AnimationActive *>::iterator anim_vect_iter;

    // Loop through all active animations
    for (anim_vect_iter = m_active_anim_vect.begin(); anim_vect_iter != m_active_anim_vect.end(); anim_vect_iter++)  {

        // If animation is currently playing, return its speed
        if ((*anim_vect_iter)->getGroupId() == m_active_group_id) {

            return (*anim_vect_iter)->getSpeed();
        }

    }

    // Return 0 if no animation and therefore no activity is active
    return 0.0f;

}

/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/src/rbody/AnimationActivityHandler.cpp,v $
 * $Revision: 1.2 $ 
 * $Date: 2005/02/09 17:26:56 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: AnimationActivityHandler.cpp,v $
 * Revision 1.2  2005/02/09 17:26:56  deepone
 * vrutils and cfgscript put into new namespaces
 *
 * Revision 1.1.1.1  2005/02/06 06:52:59  deepone
 * initial sourceforge checkin
 *
 * Revision 1.4  2004/02/03 12:58:04  deepone
 * new Property.h, Body.h and Notify.h in rvrutils
 *
 * Revision 1.3  2003/11/06 08:24:38  andersb
 * Removed the use of rbody::crVector3
 *
 * Revision 1.2  2003/10/31 13:21:48  andersb
 * Added ReplicantBodyMgr class to handle instances of CreBody
 * Added .char files for specifying individual characters (CreBody)
 * Added OSG plugin for reading char files
 *
 * Revision 1.1  2002/06/20 13:23:11  sunna
 * no message
 *
 * Revision 1.9  2002/04/18 10:05:45  sunna
 * getActivityState()/getActionState() is now returning unkonw if act doesn't exist
 *
 * Revision 1.8  2002/04/09 15:31:14  sunna
 * Actual speed of each animation within each activity is set
 * Added getCurrentSpeed()
 *
 * Revision 1.7  2002/04/02 13:44:40  sunna
 * End time greater or equal to zero
 *
 * Revision 1.6  2002/04/02 10:09:52  sunna
 * Fixed: Activitiy animations are now updated (with new weights) if they are active, else they are added to animation queue
 *
 * Revision 1.5  2002/03/28 10:54:48  sunna
 * Fixed a few bugs in the activity in-between handling
 *
 * Revision 1.4  2002/03/27 16:58:46  sunna
 * Possible to load confliting actions and in-between activities from config file
 * In-between activities implemented but not tested
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
 * Revision 1.4  2002/03/04 13:42:42  andersb
 * no message
 *
 * Revision 1.3  2002/03/01 10:26:51  andersb
 * no message
 *
 * Revision 1.2  2002/02/19 15:46:13  andersb
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2002/01/04 08:22:40  andersb
 * Initial version
 *
 ----------------------------------------------------------------------------*/


