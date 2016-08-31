#ifndef _CANIMATIONSPEED_H
#define _CANIMATIONSPEED_H

#include "rbody/global.h"
#include <exception>
#include <stdexcept>

#include "rbody/AnimationCombine.h"

namespace rbody {

/// Handles two animations which represent one speed each
class REPLICANTBODY_API AnimationSpeed : public AnimationCombine {

    public:

        /// Constructor
        AnimationSpeed();

        /// Destructor
        virtual ~AnimationSpeed();

        /// Add animation
        /**
         * \param animation_name is name of animation to add
         **/
        void addAnimation(const std::string& animation_name);

        /// Get speed associated with animation at given place in vector
        /**
         * Throws exception out_of_range if nr isn't in range 0 <= nr < size
         * \param nr is number of animation in list
         **/
        float getAnimationSpeedAt(int nr);



        /// Set speed associated with animation at given place in vector
        /**
         * Throws exception out_of_range if nr isn't in range 0 <= nr < size
         * \param nr is number of animation in list which speed will be set
         * \param speed is speed to set
         **/
        void setAnimationSpeedAt(int nr, float speed);

        /// Set default speed
        /**
         * \param default_speed is new default speed to set
         **/
        void setDefaultSpeed(float default_speed);

        /// Get default speed
        float getDefaultSpeed();

    private:

        /// Default speed
        float m_default_speed;

};

} // namespace end

#endif // _CANIMATIONSPEED_H
/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/include/rbody/AnimationSpeed.h,v $
 * $Revision: 1.1.1.1 $ 
 * $Date: 2005/02/06 06:52:59 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: AnimationSpeed.h,v $
 * Revision 1.1.1.1  2005/02/06 06:52:59  deepone
 * initial sourceforge checkin
 *
 * Revision 1.3  2003/10/31 13:21:46  andersb
 * Added ReplicantBodyMgr class to handle instances of CreBody
 * Added .char files for specifying individual characters (CreBody)
 * Added OSG plugin for reading char files
 *
 * Revision 1.2  2002/11/01 09:26:28  sunna
 * no message
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


