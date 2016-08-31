#ifndef _CANIMATIONBLEND_H
#define _CANIMATIONBLEND_H

#include "rbody/global.h"
#include <exception>
#include <stdexcept>

#include "rbody/AnimationCombine.h"

namespace rbody {

/// Handles blended animations
class REPLICANTBODY_API AnimationBlend : public AnimationCombine {

    public:

        /// Constructor
        AnimationBlend();

        /// Destructor
        virtual ~AnimationBlend();

        /// Get weight (between 0 and 1) associated with animation at given place in vector
        /**
         * Throws exception out_of_range if nr isn't in range 0 <= nr < size
         * \param nr is number of animation in list
         **/
        float getAnimationWeightAt(int nr);

};

} // namespace end

#endif // _CANIMATIONBLEND_H
/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rbody/AnimationBlend.h,v $
* $Revision: 1.1.1.1 $ 
* $Date: 2005/02/06 06:52:59 $
* $Author: deepone $ 
* $Locker:  $

* Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

 Created: 2001-12-12

* $Log: AnimationBlend.h,v $
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
* Revision 1.1.1.1  2002/01/04 08:22:34  andersb
* Initial version
*
----------------------------------------------------------------------------*/


