#ifndef _CMESH_H
#define _CMESH_H

#include "rbody/NameBase.h"
#include "rbody/Cal3dBase.h"
#include "rbody/export.h"

namespace rbody {

/// Contains information needed for a mesh
class REPLICANTBODY_API Mesh : public NameBase, public Cal3dBase 
{
public:

    /// Constructor
    Mesh();

    /// Destructor
    virtual ~Mesh();
	void setPartID(int partid);
	inline int getPartID(){ return m_partID; }
	void setMaterialFileName(const std::string &matfile);
	std::string getMaterialFileName();
protected:
    int m_partID;
	std::string m_materialFileName;
};

} // namespace end

#endif // _CMESH_H
/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/include/rbody/Mesh.h,v $
 * $Revision: 1.1.1.1 $ 
 * $Date: 2005/02/06 06:53:00 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: Mesh.h,v $
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


