#ifndef _CACTATTRIBUTES_H
#define _CACTATTRIBUTES_H

#include "rbody/global.h"

namespace rbody {

/// Activity/action attributes
	/** The purpose of this class is to make it possible to add
	 *  attributes by extending this class without changing the actual
	 *  body interface.
	 **/
class REPLICANTBODY_API ActAttributes {

    public:

        /// Constructor
        ActAttributes();

        /// Destructor
        virtual ~ActAttributes();

        /// Set default speed
        /**
         * \param speed is default speed to set
         **/
        void setDefaultSpeed(float speed);

        /// Set lowest possible speed
        /**
         * \param lowest_speed is lowest possible speed of activity/action
         **/
        void setLowestSpeed(float lowest_speed);

        /// Set highest possible speed
        /**
         * \param highest_speed is highest possible speed of activity/action
         **/
        void setHighestSpeed(float highest_speed);

        /// Return true if speed is an attribute of the action/activity
        bool hasSpeed();

        /// Get default speed
        float getDefaultSpeed();

        /// Get lowest possible speed
        float getLowestSpeed();

        /// Get highest possible speed
        float getHighestSpeed();

    private:

        /// Default speed
        float m_default_speed;

        /// Lowest possible speed
        float m_lowest_speed;

        /// Highest possible speed
        float m_highest_speed;

        /// Has speed as an attribute
        bool m_has_speed;

};


} // namespace end

#endif // _CACTATTRIBUTES_H

/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rbody/ActAttributes.h,v $
* $Revision: 1.1.1.1 $ 
* $Date: 2005/02/06 06:52:59 $
* $Author: deepone $ 
* $Locker:  $

* Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

 Created: 2001-12-12

* $Log: ActAttributes.h,v $
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.3  2003/10/31 13:21:44  andersb
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
* Revision 1.1.1.1  2002/01/04 08:22:34  andersb
* Initial version
*
----------------------------------------------------------------------------*/

