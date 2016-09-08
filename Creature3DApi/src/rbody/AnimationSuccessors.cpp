#include "rbody/AnimationSuccessors.h"

using namespace rbody;


/*----------------------------------------------------------------------------
  Constructor
  ----------------------------------------------------------------------------*/
AnimationSuccessors::AnimationSuccessors()
{
}


/*----------------------------------------------------------------------------
  Destructor
  ----------------------------------------------------------------------------*/
AnimationSuccessors::~AnimationSuccessors()
{
}


/*----------------------------------------------------------------------------
  Set animation name
  ----------------------------------------------------------------------------*/
void AnimationSuccessors::setAnimationName(const std::string& animation_name)
{

    m_animation_name = animation_name;

}


/*----------------------------------------------------------------------------
  Get animation name
  ----------------------------------------------------------------------------*/
std::string AnimationSuccessors::getAnimationName()
{

    return m_animation_name;

}


/*----------------------------------------------------------------------------
  Add animation
  ----------------------------------------------------------------------------*/
void AnimationSuccessors::addSuccessor(const std::string& successor_name)
{

    m_animation_list.push_back(successor_name);

}


/*----------------------------------------------------------------------------
  Get all animation successors as a list of strings
  ----------------------------------------------------------------------------*/
std::list<std::string> AnimationSuccessors::getSuccessors()
{

    return m_animation_list;

}

/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/src/rbody/AnimationSuccessors.cpp,v $
 * $Revision: 1.1.1.1 $ 
 * $Date: 2005/02/06 06:52:59 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: AnimationSuccessors.cpp,v $
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
 * Revision 1.1.1.1  2002/01/04 08:22:40  andersb
 * Initial version
 *
 ----------------------------------------------------------------------------*/


