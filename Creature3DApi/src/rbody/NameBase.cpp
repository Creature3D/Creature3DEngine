#include "rbody/NameBase.h"

using namespace rbody;


/*----------------------------------------------------------------------------
  Constructor
  ----------------------------------------------------------------------------*/
NameBase::NameBase()
{

    m_is_default = false;

}


/*----------------------------------------------------------------------------
  Destructor
  ----------------------------------------------------------------------------*/
NameBase::~NameBase()
{
}


/*----------------------------------------------------------------------------
  Set name
  ----------------------------------------------------------------------------*/
void NameBase::setName(const std::string& name)
{

    m_name = name;

}


/*----------------------------------------------------------------------------
  Get name
  ----------------------------------------------------------------------------*/
std::string NameBase::getName()
{

    return m_name;

}


/*----------------------------------------------------------------------------
  Set default or not
  ----------------------------------------------------------------------------*/
void NameBase::setIsDefault(bool is_default)
{

    m_is_default = is_default;

}


/*----------------------------------------------------------------------------
  Return true if used as default, else false
  ----------------------------------------------------------------------------*/
bool NameBase::isDefault()
{

    return m_is_default;

}

/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/src/rbody/NameBase.cpp,v $
 * $Revision: 1.1.1.1 $ 
 * $Date: 2005/02/06 06:52:59 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: NameBase.cpp,v $
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


