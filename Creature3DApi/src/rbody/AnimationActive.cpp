#include "rbody/AnimationActive.h"

using namespace rbody;


/*----------------------------------------------------------------------------
  Constructor
  ----------------------------------------------------------------------------*/
AnimationActive::AnimationActive()
{
    m_start_time = 0.0f;
    m_current_time = 0.0f;
    m_is_activated = 0;
    m_speed = 0;

}


/*----------------------------------------------------------------------------
  Destructor
  ----------------------------------------------------------------------------*/
AnimationActive::~AnimationActive()
{

}


/*----------------------------------------------------------------------------
  Set start time of animation
  ----------------------------------------------------------------------------*/
void AnimationActive::setStartTime(float start_time)
{

    m_start_time = start_time;

}


/*----------------------------------------------------------------------------
  Get start time of animation
  ----------------------------------------------------------------------------*/
float AnimationActive::getStartTime()
{

    return m_start_time;

}


/*----------------------------------------------------------------------------
  Set end time of animation
  ----------------------------------------------------------------------------*/
void AnimationActive::setEndTime(float end_time)
{

    m_end_time = end_time;

}


/*----------------------------------------------------------------------------
  Get end time of animation
  ----------------------------------------------------------------------------*/
float AnimationActive::getEndTime()
{

    return m_end_time;

}


/*----------------------------------------------------------------------------
  Set current time of animation
  ----------------------------------------------------------------------------*/
void AnimationActive::setCurrentTime(float current_time)
{

    m_current_time = current_time;

}


/*----------------------------------------------------------------------------
  Get current time of animation
  ----------------------------------------------------------------------------*/
float AnimationActive::getCurrentTime()
{

    return m_current_time;

}


/*----------------------------------------------------------------------------
  Set duration of animation
  ----------------------------------------------------------------------------*/
void AnimationActive::setDuration(float duration)
{

    m_duration = duration;

}


/*----------------------------------------------------------------------------
  Get duration of animation
  ----------------------------------------------------------------------------*/
float AnimationActive::getDuration()
{

    return m_duration;

}


/*----------------------------------------------------------------------------
  Set weight of animation - used for blending
  ----------------------------------------------------------------------------*/
void AnimationActive::setWeight(float weight)
{

    m_weight = weight;

}


/*----------------------------------------------------------------------------
  Get weight of animation
  ----------------------------------------------------------------------------*/
float AnimationActive::getWeight()
{

    return m_weight;

}


/*----------------------------------------------------------------------------
  Set Cal3d id
  ----------------------------------------------------------------------------*/
void AnimationActive::setCal3dId(int cal3d_id)
{

    m_cal3d_id = cal3d_id;

}


/*----------------------------------------------------------------------------
  Get Cal3d id
  ----------------------------------------------------------------------------*/
int AnimationActive::getCal3dId()
{

    return m_cal3d_id;

}


/*----------------------------------------------------------------------------
  Set group id - used to bundle animations
  ----------------------------------------------------------------------------*/
void AnimationActive::setGroupId(int group_id)
{

    m_group_id = group_id;

}


/*----------------------------------------------------------------------------
  Get id of animation - used to bundle animations
  ----------------------------------------------------------------------------*/
int AnimationActive::getGroupId()
{

    return m_group_id;

}


/*----------------------------------------------------------------------------
  Set name of action/activity that animation is connected to
  ----------------------------------------------------------------------------*/
void AnimationActive::setActName(const std::string& act_name)
{

    m_act_name = act_name;

}


/*----------------------------------------------------------------------------
  Get action/activity name that animation is connected to
  ----------------------------------------------------------------------------*/
std::string AnimationActive::getActName()
{

    return m_act_name;

}


/*----------------------------------------------------------------------------
  Set whether animation is looped or not
  ----------------------------------------------------------------------------*/
void AnimationActive::setIsLooped(unsigned char is_looped)
{

    m_is_looped = is_looped;

}


/*----------------------------------------------------------------------------
  Return true if animation is looped, else false
  ----------------------------------------------------------------------------*/
unsigned char AnimationActive::isLooped()
{

    return m_is_looped;

}


/*----------------------------------------------------------------------------
  Set whether animation is activated or not
  ----------------------------------------------------------------------------*/
void AnimationActive::setIsActivated(bool is_activated)
{

    m_is_activated = is_activated;

}


/*----------------------------------------------------------------------------
  Return true if animation is looped, else false
  ----------------------------------------------------------------------------*/
bool AnimationActive::isActivated()
{

    return m_is_activated;

}

/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/src/rbody/AnimationActive.cpp,v $
 * $Revision: 1.1.1.1 $ 
 * $Date: 2005/02/06 06:52:59 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: AnimationActive.cpp,v $
 * Revision 1.1.1.1  2005/02/06 06:52:59  deepone
 * initial sourceforge checkin
 *
 * Revision 1.1  2002/06/20 13:23:11  sunna
 * no message
 *
 * Revision 1.3  2002/04/10 06:44:23  sunna
 * Added get and set speed
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


