#include "rbody/AnimationBase.h"

using namespace rbody;


/*----------------------------------------------------------------------------
  Constructor
  ----------------------------------------------------------------------------*/
AnimationBase::AnimationBase()
{

    // This instance isn't an animation type
    m_type = AnimationBase::NONE;

}


/*----------------------------------------------------------------------------
  Destructor
  ----------------------------------------------------------------------------*/
AnimationBase::~AnimationBase()
{
}


/*----------------------------------------------------------------------------
  Set activity or action name
  ----------------------------------------------------------------------------*/
void AnimationBase::setActName(const std::string& act_name)
{

    m_act_name = act_name;

}


/*----------------------------------------------------------------------------
  Get name of activity/action that animation is connected to
  ----------------------------------------------------------------------------*/
std::string AnimationBase::getActName()
{

    return m_act_name;

}


/*----------------------------------------------------------------------------
  Set weight of animation
  ----------------------------------------------------------------------------*/
void AnimationBase::setWeight(float weight)
{

    m_weight = weight;

}


/*----------------------------------------------------------------------------
  Get weight of animation
  ----------------------------------------------------------------------------*/
float AnimationBase::getWeight()
{

    return m_weight;

}


/*----------------------------------------------------------------------------
  Get type of instance - that is which class it belongs to
  ----------------------------------------------------------------------------*/
AnimationBase::AnimationType AnimationBase::getType()
{

    return m_type;

}

/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/src/rbody/AnimationBase.cpp,v $
 * $Revision: 1.1.1.1 $ 
 * $Date: 2005/02/06 06:52:59 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: AnimationBase.cpp,v $
 * Revision 1.1.1.1  2005/02/06 06:52:59  deepone
 * initial sourceforge checkin
 *
 * Revision 1.1  2002/06/20 13:23:11  sunna
 * no message
 *
 * Revision 1.2  2002/03/15 08:55:08  andersb
 * no message
 *
 * Revision 1.1.1.1  2002/03/14 20:48:40  andersb
 * First import as ReplicantBody
 *
 * Revision 1.2  2002/02/19 15:46:13  andersb
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2002/01/04 08:22:40  andersb
 * Initial version
 *
 ----------------------------------------------------------------------------*/


