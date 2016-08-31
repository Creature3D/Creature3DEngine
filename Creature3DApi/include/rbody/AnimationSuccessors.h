#ifndef _CANIMATIONSUCCESSORS_H
#define _CANIMATIONSUCCESSORS_H

#include "rbody/global.h"
#include <string>
#include <list>

namespace rbody {

/// Contains all possible successors to an animation
class REPLICANTBODY_API AnimationSuccessors {
private:

	/// Name of animation
	std::string m_animation_name;

	/// Animation list with possible successors to animation
	std::list<std::string> m_animation_list; 

public:

	/// Constructor
	AnimationSuccessors();

	/// Destructor
	~AnimationSuccessors();

	/// Set animation name
        /**
         * \param animation_name is name of animation
         **/
	void setAnimationName(const std::string& animation_name);

	/// Get animation name
	std::string getAnimationName();


	/// Add animation
        /**
         * \param successor_name is name of possible animation successor to add
         **/
	void addSuccessor(const std::string& successor_name);

	/// Get all animation successors as a vector of strings
	std::list<std::string> getSuccessors();

};

} // namespace end

#endif // _CANIMATIONSUCCESSORS_H
/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rbody/AnimationSuccessors.h,v $
* $Revision: 1.1.1.1 $ 
* $Date: 2005/02/06 06:52:59 $
* $Author: deepone $ 
* $Locker:  $

* Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

 Created: 2001-12-12

* $Log: AnimationSuccessors.h,v $
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.2  2003/10/31 13:21:46  andersb
* Added ReplicantBodyMgr class to handle instances of CreBody
* Added .char files for specifying individual characters (CreBody)
* Added OSG plugin for reading char files
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


