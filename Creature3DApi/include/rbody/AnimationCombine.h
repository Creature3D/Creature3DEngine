#ifndef _CANIMATIONCOMBINE_H
#define _CANIMATIONCOMBINE_H

#include <string>
#include <vector>
#include <exception>
#include <stdexcept>

#include "rbody/global.h"
#include "rbody/AnimationBase.h"

namespace rbody {

/// Combined animations' class
class REPLICANTBODY_API AnimationCombine : public AnimationBase {
protected:

	/// Struct with string (animation name) and float (weight, time or degree)
	typedef struct strfloat_struct_t {
		/// string used as animation name
		std::string animation_name;

		/// float used as weight, time or degree depending on instance
		float f;
	};

	/// Animation vector with name and float
	std::vector<strfloat_struct_t> m_animation_vect; 

protected:

	/// Constructor
	AnimationCombine();

	/// Get float associated with animation at given place in vector
        /**
 		 * Throws exception out_of_range if nr isn't in range 0 <= nr < size
         * \param nr is number of animation in vector
         **/
	float getAnimationFloatAt(unsigned int nr);

public:

	/// Destructor
	virtual ~AnimationCombine();

	/// Add animation
        /**
         * \param animation_name is name of animation to add
		 * \param f is either weight, time or degree depending on instance
         **/
	void addAnimation(const std::string& animation_name, float f);

	/// Get number of animations in vector
	int getNrOfAnimations();

	/// Get name of animation at given place in vector
        /**
 		 * Throws exception out_of_range if nr isn't in range 0 <= nr < size
         * \param nr is number of animation in vector
         **/
	std::string getAnimationNameAt(unsigned int nr);


};

} // namespace end

#endif // _CANIMATIONCOMBINE_H
/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rbody/AnimationCombine.h,v $
* $Revision: 1.1.1.1 $ 
* $Date: 2005/02/06 06:52:59 $
* $Author: deepone $ 
* $Locker:  $

* Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

 Created: 2001-12-12

* $Log: AnimationCombine.h,v $
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.3  2003/10/31 13:21:46  andersb
* Added ReplicantBodyMgr class to handle instances of CreBody
* Added .char files for specifying individual characters (CreBody)
* Added OSG plugin for reading char files
*
* Revision 1.2  2003/10/01 09:00:40  sunna
* rbodyTest now in ReplicantBody directory structure instead of osgReplicant
*
* Revision 1.1  2002/06/20 12:51:50  sunna
* Restructured directories
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


