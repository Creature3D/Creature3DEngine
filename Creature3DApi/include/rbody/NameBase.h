#ifndef _CNAMEBASE_H
#define _CNAMEBASE_H

#include "rbody/global.h"
#include <string>

namespace rbody {

/// Contains information needed (name, is_default) for named cal3d parts
class REPLICANTBODY_API NameBase {

    public:

        /// Constructor
        NameBase();

        /// Destructor
        virtual ~NameBase();

        /// Set name
        /**
         * \param name is name to be set
         **/
        void setName(const std::string& name);

        /// Get name
        std::string getName();

        /// Set to be default or not default
        /**
         * \param is_default is set to true if it should be used as default, else false
         **/
        void setIsDefault(bool is_default);

        /// Returns true if it is used as default, else false
        bool isDefault();

    private:

        /// name
        std::string m_name;

        /// tells whether it should be used as default or not
        bool m_is_default;

};

} // namespace end

#endif // _CNAMEBASE_H
/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/include/rbody/NameBase.h,v $
 * $Revision: 1.1.1.1 $ 
 * $Date: 2005/02/06 06:53:00 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: NameBase.h,v $
 * Revision 1.1.1.1  2005/02/06 06:53:00  deepone
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

