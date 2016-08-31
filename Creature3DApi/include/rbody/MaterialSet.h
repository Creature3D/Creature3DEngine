#ifndef _CMATERIALSET_H
#define _CMATERIALSET_H

#include "rbody/global.h"
#include <vector>
#include <exception>
#include <stdexcept>

#include "rbody/NameBase.h"

namespace rbody {

/// Material set with connections between cal3d material threads and cal3d materials
class REPLICANTBODY_API MaterialSet : public NameBase {

    public:

        /// Constructor
        MaterialSet();

        /// Destructor
        virtual ~MaterialSet();

        /// Set id of material set
        /**
         * \param id is cal3d set id
         **/
        void setCal3dMaterialSetId(int id);

        /// Get id of material set
        int getCal3dMaterialSetId();

        /// Add connection between cal3d thread and material
        /**
         * \param thread_id is id of cal3d material thread
         * \param material_id is id of cal3d material
         **/
        void addConnection(int thread_id, int material_id);

        /// Get number of connections
        int getNrOfConnections();

        /// Get connection at given position
        /**
         * Throws exception out_of_range if nr isn't in range 0 <= nr < size
         * \param nr is position of connection in vector
         * \param thread_id is reference where thread material id will be placed
         * \param material_id is reference where material id will be placed
         **/
        void getConnectionAt(unsigned int nr, int& thread_id, int& material_id) throw(std::out_of_range);

        /// Set Cal3d id
        /**
         * \param cal3d_id is id assigned to in cal3d
         **/
        void setCal3dId(int cal3d_id);

        /// Get Cal3d id
        int getCal3dId();

    private:

        /// Struct with two integers
        typedef struct intint_struct_t {
            /// if of material thread
            int thread_id;
            /// id of material
            int material_id;
        };

        /// id of material set in cal3d
        int m_cal3d_set_id;

        /// vector with connections between cal3d threads and materials
        std::vector<intint_struct_t> m_connection_vect;

        /// Cal3d id of material set
        int m_cal3d_id;

};

} // namespace end

#endif // _CMATERIALSET_H
/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/include/rbody/MaterialSet.h,v $
 * $Revision: 1.1.1.1 $ 
 * $Date: 2005/02/06 06:53:00 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: MaterialSet.h,v $
 * Revision 1.1.1.1  2005/02/06 06:53:00  deepone
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


