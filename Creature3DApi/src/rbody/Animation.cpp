#include "rbody/Animation.h"

using namespace rbody;


/*----------------------------------------------------------------------------
  Constructor
  ----------------------------------------------------------------------------*/
Animation::Animation()
{

    m_type = AnimationBase::REAL;
    m_has_speed = false;
    m_speed = 0.0f;

}


/*----------------------------------------------------------------------------
  Destructor
  ----------------------------------------------------------------------------*/
Animation::~Animation()
{
}


/*----------------------------------------------------------------------------
  Set speed of animation
  ----------------------------------------------------------------------------*/
void Animation::setSpeed(float speed)
{

    m_speed = speed;
    m_has_speed = true;

}


/*----------------------------------------------------------------------------
  Get speed of animation
  ----------------------------------------------------------------------------*/
float Animation::getSpeed()
{

    return m_speed;

}


/*----------------------------------------------------------------------------
  Return true if speed is set
  ----------------------------------------------------------------------------*/
bool Animation::hasSpeed()
{

    return m_has_speed;

}


/*----------------------------------------------------------------------------
  Set animation to looped or non-looped
  ----------------------------------------------------------------------------*/
void Animation::setIsLooped(unsigned char is_looped)
{

    m_is_looped = is_looped;

}


/*----------------------------------------------------------------------------
  Return true if animation is looped, else false
  ----------------------------------------------------------------------------*/
unsigned char Animation::isLooped()
{

    return m_is_looped;

}

/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/src/rbody/Animation.cpp,v $
 * $Revision: 1.1.1.1 $ 
 * $Date: 2005/02/06 06:52:59 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: Animation.cpp,v $
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
 * Revision 1.2  2002/02/19 15:46:12  andersb
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2002/01/04 08:22:40  andersb
 * Initial version
 *
 ----------------------------------------------------------------------------*/


