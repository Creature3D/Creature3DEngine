#include "rbody/AnimationSpeed.h"

using namespace rbody;

/*----------------------------------------------------------------------------
  Constructor
  ----------------------------------------------------------------------------*/
AnimationSpeed::AnimationSpeed()
{

    m_type = AnimationBase::SPEED;
    m_default_speed = 0;

}


/*----------------------------------------------------------------------------
  Destructor
  ----------------------------------------------------------------------------*/
AnimationSpeed::~AnimationSpeed()
{
}

/*----------------------------------------------------------------------------
  Add animation
  ----------------------------------------------------------------------------*/
void AnimationSpeed::addAnimation(const std::string& animation_name)
{

    // Add animation with speed 0.0
    AnimationCombine::addAnimation(animation_name, 0.0f);

}


/*----------------------------------------------------------------------------
  Get speed that animation at given index is representing
  Throws exception out_of_range if nr isn't in range 0 <= nr < size
  -----------------------------------------------------------------------------*/
float AnimationSpeed::getAnimationSpeedAt(int nr)
{

    return getAnimationFloatAt(nr);

}


/*----------------------------------------------------------------------------
  Set speed of animation at given position in list
  ----------------------------------------------------------------------------*/
void AnimationSpeed::setAnimationSpeedAt(int nr, float speed)
{

    // Check that nr is valid and throw out_of range if it isn't
    if ((nr < 0) || (nr >= getNrOfAnimations()))
        throw std::out_of_range("AnimationCombine::setAnimationSpeedAt()");

    // Set speed
    m_animation_vect[nr].f = speed;

}


/*----------------------------------------------------------------------------
  Set default speed
  ----------------------------------------------------------------------------*/
void AnimationSpeed::setDefaultSpeed(float default_speed)
{

    m_default_speed = default_speed;

}


/*----------------------------------------------------------------------------
  Get default speed
  ----------------------------------------------------------------------------*/
float AnimationSpeed::getDefaultSpeed()
{

    return m_default_speed;

}

/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/src/rbody/AnimationSpeed.cpp,v $
 * $Revision: 1.1.1.1 $ 
 * $Date: 2005/02/06 06:52:59 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: AnimationSpeed.cpp,v $
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
 * Revision 1.2  2002/02/19 15:46:16  andersb
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2002/01/04 08:22:40  andersb
 * Initial version
 *
 ----------------------------------------------------------------------------*/


