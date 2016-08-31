#ifndef _CANIMATIONBASE_H
#define _CANIMATIONBASE_H

#include "rbody/global.h"
#include <string>

#include "rbody/NameBase.h"

namespace rbody {

/// Animation Base Class
class REPLICANTBODY_API AnimationBase : public NameBase {

    public:

        /// Animation type
        enum AnimationType {
            NONE, /**< means the object isn't a valid animation yet */
            REAL, /**< means the object is a Animation */				
            DEGREE, /**< means the object is a AnimationDegree  */
            SPEED, /**< means the object is a AnimationSpeed  */
            BLEND, /**< means the object is a AnimationBlend */
            SCRIPT, /**< means the object is a AnimationScript */
        };

    protected:

        /// Constructor
        AnimationBase();

    public:

        /// Destructor
        virtual ~AnimationBase();

        /// Set activity or action name
        /**
         * \param act_name is activity/action name to be set
         **/
        void setActName(const std::string& act_name);

        /// Get name of activity/action that animation is connected to
        std::string getActName();

        /// Set weight of animation
        /**
         * \param weight is the weight of the animation
         **/
        void setWeight(float weight);

        /// Get weight of animation
        float getWeight();

        /// Get class the instance belongs to
        AnimationType getType();

    private:

        /// Name of activity/action animation is connected to
        std::string m_act_name;

        /// Weight of animation, used if several animations are connected to same activity/action
        float m_weight;

    protected:

        /// Type, tells which class the instance belongs to
        AnimationType m_type;

};

} // namespace end

#endif // _CANIMATIONBASE_H
/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rbody/AnimationBase.h,v $
* $Revision: 1.1.1.1 $ 
* $Date: 2005/02/06 06:52:59 $
* $Author: deepone $ 
* $Locker:  $

* Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

 Created: 2001-12-12

* $Log: AnimationBase.h,v $
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


