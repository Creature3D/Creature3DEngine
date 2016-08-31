#ifndef _CCAL3DBASE_H
#define _CCAL3DBASE_H

#include "rbody/global.h"
#include <string>

namespace rbody {

/// Contains base elements (filename, id) for Cal3d parts, such as animation, mesh and material
class REPLICANTBODY_API Cal3dBase {

    public:

        /// Constructor
        Cal3dBase();

        /// Destructor
        virtual ~Cal3dBase();

        /// Set Cal3d filename
        /**
         * \param filename is name of file containing cal3d information
         **/
        void setFilename(const std::string& filename);

        /// Get Cal3d filename
        std::string getFilename();

        /// Set Cal3d id
        /**
         * \param cal3d_id is id assigned to in cal3d
         **/
        void setCal3dId(int cal3d_id);

        /// Get Cal3d id
        int getCal3dId();

    private:

        /// name of cal3d file
        std::string m_cal3d_filename;

        /// id assigned to in cal3d 
        int m_cal3d_id;

};

} // namespace end

#endif // _CCAL3DBASE_H
/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/include/rbody/Cal3dBase.h,v $
 * $Revision: 1.1.1.1 $ 
 * $Date: 2005/02/06 06:52:59 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: Cal3dBase.h,v $
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

