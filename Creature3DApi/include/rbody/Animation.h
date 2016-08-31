#ifndef _CANIMATION_H
#define _CANIMATION_H

#include "rbody/global.h"
#include "rbody/AnimationBase.h"
#include "rbody/Cal3dBase.h"

namespace rbody {

/// Animation Class
class REPLICANTBODY_API Animation : public AnimationBase, public Cal3dBase {

    public:

        /// Constructor
        Animation();

        /// Destructor
        virtual ~Animation();

        /// Set speed of animation
        /**
         * \param speed is speed in which to play animation
         **/
        void setSpeed(float speed);

        /// Get speed of animation
        float getSpeed();

        /// Return true if speed is set, else false
        bool hasSpeed();

        /// Set animation to looped or non-looped
        /**
         * \param is_looped is set to true if animation should loop, else false
         **/
        void setIsLooped(unsigned char is_looped);

        /// Returns true if animation is looped, else false
        unsigned char isLooped();	

    private:

        /// tells if animation should loop
        unsigned char m_is_looped;//0不循环，1循环，2结束时候停止动作

        /// speed of animation
        float m_speed;

        /// tells if speed is set
        bool m_has_speed;

};

} // namespace end

#endif // _CANIMATION_H
/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rbody/Animation.h,v $
* $Revision: 1.1.1.1 $ 
* $Date: 2005/02/06 06:52:59 $
* $Author: deepone $ 
* $Locker:  $

* Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

 Created: 2001-12-12

* $Log: Animation.h,v $
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.2  2003/10/31 13:21:45  andersb
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
* Revision 1.1.1.1  2002/01/04 08:22:34  andersb
* Initial version
*
----------------------------------------------------------------------------*/


