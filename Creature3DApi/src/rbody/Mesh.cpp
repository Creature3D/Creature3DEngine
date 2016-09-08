#include "rbody/Mesh.h"

using namespace rbody;


/*----------------------------------------------------------------------------
  Constructor
  ----------------------------------------------------------------------------*/
Mesh::Mesh():
m_partID(0)
{
}


/*----------------------------------------------------------------------------
  Destructor
  ----------------------------------------------------------------------------*/
Mesh::~Mesh()
{
}

void Mesh::setPartID(int partid)
{
	m_partID = partid;
}
void Mesh::setMaterialFileName(const std::string &matfile)
{
	m_materialFileName = matfile;
}
std::string Mesh::getMaterialFileName()
{
	return m_materialFileName;
}
/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/src/rbody/Mesh.cpp,v $
 * $Revision: 1.1.1.1 $ 
 * $Date: 2005/02/06 06:52:59 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: Mesh.cpp,v $
 * Revision 1.1.1.1  2005/02/06 06:52:59  deepone
 * initial sourceforge checkin
 *
 * Revision 1.1  2002/06/20 13:23:11  sunna
 * no message
 *
 * Revision 1.2  2002/03/15 08:55:09  andersb
 * no message
 *
 * Revision 1.1.1.1  2002/03/14 20:48:40  andersb
 * First import as ReplicantBody
 *
 * Revision 1.1.1.1  2002/01/04 08:22:41  andersb
 * Initial version
 *
 ----------------------------------------------------------------------------*/


