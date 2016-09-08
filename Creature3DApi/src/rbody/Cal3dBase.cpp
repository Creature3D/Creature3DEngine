#include "rbody/Cal3dBase.h"

using namespace rbody;


/*----------------------------------------------------------------------------
  Constructor
  ----------------------------------------------------------------------------*/
Cal3dBase::Cal3dBase()
{
}


/*----------------------------------------------------------------------------
  Destructor
  ----------------------------------------------------------------------------*/
Cal3dBase::~Cal3dBase()
{
}


/*----------------------------------------------------------------------------
  Set filename
  ----------------------------------------------------------------------------*/
void Cal3dBase::setFilename(const std::string& filename)
{

    m_cal3d_filename = filename;

}


/*----------------------------------------------------------------------------
  Get filename
  ----------------------------------------------------------------------------*/
std::string Cal3dBase::getFilename()
{

    return m_cal3d_filename;

}


/*----------------------------------------------------------------------------
  Set cal3d id
  ----------------------------------------------------------------------------*/
void Cal3dBase::setCal3dId(int cal3d_id)
{

    m_cal3d_id = cal3d_id;

}


/*----------------------------------------------------------------------------
  Get cal3d id
  ----------------------------------------------------------------------------*/
int Cal3dBase::getCal3dId()
{

    return m_cal3d_id;

}

/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/src/rbody/Cal3dBase.cpp,v $
 * $Revision: 1.1.1.1 $ 
 * $Date: 2005/02/06 06:52:59 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: Cal3dBase.cpp,v $
 * Revision 1.1.1.1  2005/02/06 06:52:59  deepone
 * initial sourceforge checkin
 *
 * Revision 1.1  2002/06/20 13:23:11  sunna
 * no message
 *
 * Revision 1.2  2002/03/15 08:55:08  andersb
 * no message
 *
 * Revision 1.1.1.1  2002/03/14 20:48:40  andersb
 * First import as ReplicantBody
 *
 * Revision 1.1.1.1  2002/01/04 08:22:40  andersb
 * Initial version
 *
 ----------------------------------------------------------------------------*/


