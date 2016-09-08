#include "rbody/MaterialSet.h"

using namespace rbody;


/*----------------------------------------------------------------------------
  Constructor
  ----------------------------------------------------------------------------*/
MaterialSet::MaterialSet()
{
}


/*----------------------------------------------------------------------------
  Destructor
  ----------------------------------------------------------------------------*/
MaterialSet::~MaterialSet()
{
}


/*----------------------------------------------------------------------------
  Set id of material set
  ----------------------------------------------------------------------------*/
void MaterialSet::setCal3dMaterialSetId(int id)
{

    m_cal3d_set_id = id;

}


/*----------------------------------------------------------------------------
  Get id of material set
  ----------------------------------------------------------------------------*/
int MaterialSet::getCal3dMaterialSetId()
{

    return m_cal3d_set_id;

}


/*----------------------------------------------------------------------------
  Add connection between cal3d thread and material
  ----------------------------------------------------------------------------*/
void MaterialSet::addConnection(int thread_id, int material_id)
{

    intint_struct_t connection;

    connection.material_id = material_id;
    connection.thread_id = thread_id;

    m_connection_vect.push_back(connection);

}


/*----------------------------------------------------------------------------
  Get number of connections
  ----------------------------------------------------------------------------*/
int MaterialSet::getNrOfConnections()
{

    return m_connection_vect.size();

}


/*----------------------------------------------------------------------------
  Get connection at given position
  Throws exception out_of_range if nr isn't in range 0 <= nr < size
  ----------------------------------------------------------------------------*/
void MaterialSet::getConnectionAt(unsigned int nr, int& thread_id, int& material_id) throw(std::out_of_range)
{

    // Check that nr is valid and throw out_of range if it isn't
    if ((nr < 0) || (nr >= m_connection_vect.size()))
        throw std::out_of_range("MaterialSet::getConnectionAt()");

    intint_struct_t connection;

    // Get requested connection struct
    connection = m_connection_vect[nr];

    // Set return variables
    thread_id = connection.thread_id;
    material_id = connection.material_id;

}

/*----------------------------------------------------------------------------
  Set cal3d id
  ----------------------------------------------------------------------------*/
void MaterialSet::setCal3dId(int cal3d_id)
{

    m_cal3d_id = cal3d_id;

}


/*----------------------------------------------------------------------------
  Get cal3d id
  ----------------------------------------------------------------------------*/
int MaterialSet::getCal3dId()
{

    return m_cal3d_id;

}

/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/src/rbody/MaterialSet.cpp,v $
 * $Revision: 1.1.1.1 $ 
 * $Date: 2005/02/06 06:52:59 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: MaterialSet.cpp,v $
 * Revision 1.1.1.1  2005/02/06 06:52:59  deepone
 * initial sourceforge checkin
 *
 * Revision 1.2  2003/10/01 09:00:41  sunna
 * rbodyTest now in ReplicantBody directory structure instead of osgReplicant
 *
 * Revision 1.1  2002/06/20 13:23:11  sunna
 * no message
 *
 * Revision 1.3  2002/04/04 14:30:39  sunna
 * Using push_back instead of insert
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


