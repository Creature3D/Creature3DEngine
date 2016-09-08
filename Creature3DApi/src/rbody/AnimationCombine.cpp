#include "rbody/AnimationCombine.h"

using namespace rbody;


/*----------------------------------------------------------------------------
 Constructor
 ----------------------------------------------------------------------------*/
AnimationCombine::AnimationCombine()
{

	m_type = AnimationBase::NONE;

}


/*----------------------------------------------------------------------------
 Destructor
 ----------------------------------------------------------------------------*/
AnimationCombine::~AnimationCombine()
{
}


/*----------------------------------------------------------------------------
 Get float associated with animation at given place in vector
 Throws exception out_of_range if nr isn't in range 0 <= nr < size
 ----------------------------------------------------------------------------*/
float AnimationCombine::getAnimationFloatAt(unsigned int nr)
{

	// Check that nr is valid and throw out_of range if it isn't
	if ((nr < 0) || (nr >= m_animation_vect.size()))
		throw std::out_of_range("AnimationCombine::getAnimationFloatAt()");

	// Return float
	return m_animation_vect[nr].f;

}


/*----------------------------------------------------------------------------
 Add animation
 ----------------------------------------------------------------------------*/
void AnimationCombine::addAnimation(const std::string& animation_name, float f)
{
	strfloat_struct_t anim_info;

	// Set variables
	anim_info.animation_name = animation_name;
	anim_info.f = f;

	// Insert into animation vector
	m_animation_vect.push_back(anim_info);

}


/*----------------------------------------------------------------------------
 Get number of animations in vector
 ----------------------------------------------------------------------------*/
int AnimationCombine::getNrOfAnimations()
{

	return m_animation_vect.size();

}


/*----------------------------------------------------------------------------
 Get name of animation at given place in vector
 ----------------------------------------------------------------------------*/
std::string AnimationCombine::getAnimationNameAt(unsigned int nr)
{

	// Check that nr is valid and throw out_of range if it isn't
	if ((nr < 0) || (nr >= m_animation_vect.size()))
		throw std::out_of_range("AnimationCombine::getAnimationNameAt()");

	// Return animation name
	return m_animation_vect[nr].animation_name;

}

/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/src/rbody/AnimationCombine.cpp,v $
* $Revision: 1.1.1.1 $ 
* $Date: 2005/02/06 06:52:59 $
* $Author: deepone $ 
* $Locker:  $

* Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

 Created: 2001-12-12

* $Log: AnimationCombine.cpp,v $
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.2  2003/10/01 09:00:41  sunna
* rbodyTest now in ReplicantBody directory structure instead of osgReplicant
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
* Revision 1.2  2002/02/19 15:46:15  andersb
* *** empty log message ***
*
* Revision 1.1.1.1  2002/01/04 08:22:40  andersb
* Initial version
*
----------------------------------------------------------------------------*/


