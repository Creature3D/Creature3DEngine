#ifndef _CACTREQUESTARG_H
#define _CACTREQUESTARG_H

#include "rbody/global.h"
#include <vector>
#include <CRCore/crVector3.h>

namespace rbody {

/// Activity/action request argument class
	/** The purpose of this class is to make it possible to add
	 *  arguments to the different activities/actions by extending 
	 *  this class without changing the actual body interface.
	 **/
class REPLICANTBODY_API ActRequestArg {

    public:

        /// Constructor
        ActRequestArg();

        /// Destructor
        virtual ~ActRequestArg();

        /// Set direction
        /**
         * \param direction is direction to set
         **/
        void setDirection(CRCore::crVector3 direction);

        /// Return true if direction is used
        bool hasDirection();

        /// Get direction
        void getDirection(CRCore::crVector3 &direction);

        /// Set degree
        /**
         * \param degree is degree to set
         **/
        void setDegree(float degree);

        /// Return true if degree is used
        bool hasDegree();

        /// Get degree
        float getDegree();

        /// Set speed
        /**
         * \param speed is speed to set
         **/
        void setSpeed(float speed);

        /// Return true if speed is used
        bool hasSpeed();

        /// Get speed
        float getSpeed();

        /// Add animation weight
        void addWeight(float weight);

        /// Get weight at postition
        /**
         * \param nr is index in weight vector
         **/
        float getWeightAt(int nr);

        /// Get number of weights
        int getNrOfWeights();

    private:

        /// Direction vector
        CRCore::crVector3 m_direction;

        /// Direction vector is set
        bool m_has_direction;

        /// Direction in degrees
        float m_degree;

        /// Direction in degrees is set
        bool m_has_degree;

        /// Speed
        float m_speed;

        /// Speed is set
        bool m_has_speed;

        /// Animation weights
        std::vector<float> m_weights_vect;

};

} // namespace end

#endif // _CACTREQUESTARG_H
/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rbody/ActRequestArg.h,v $
* $Revision: 1.2 $ 
* $Date: 2005/02/09 17:26:54 $
* $Author: deepone $ 
* $Locker:  $

* Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

 Created: 2001-12-12

* $Log: ActRequestArg.h,v $
* Revision 1.2  2005/02/09 17:26:54  deepone
* vrutils and cfgscript put into new namespaces
*
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.5  2004/02/03 12:58:02  deepone
* new Property.h, Body.h and Notify.h in rvrutils
*
* Revision 1.4  2003/11/12 10:12:11  andersb
* Body inherits from PropertyContainer again.
* Moved Direction property out of Body.
*
* Revision 1.3  2003/11/11 10:52:06  sunna
* *** empty log message ***
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
* Revision 1.2  2002/02/28 10:19:14  andersb
* no message
*
* Revision 1.1.1.1  2002/01/04 08:22:34  andersb
* Initial version
*
----------------------------------------------------------------------------*/

