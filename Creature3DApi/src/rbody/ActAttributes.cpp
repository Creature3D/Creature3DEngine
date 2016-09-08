#include "rbody/ActAttributes.h"

using namespace rbody;

/*----------------------------------------------------------------------------
  Constructor
  ----------------------------------------------------------------------------*/
ActAttributes::ActAttributes()
{

    m_has_speed = false;
    m_default_speed = 0;
    m_lowest_speed = 0;
    m_highest_speed = 0;

}


/*----------------------------------------------------------------------------
  Destructor
  ----------------------------------------------------------------------------*/
ActAttributes::~ActAttributes()
{

}


/*----------------------------------------------------------------------------
  Set default speed
  ----------------------------------------------------------------------------*/
void ActAttributes::setDefaultSpeed(float default_speed)
{

    m_default_speed = default_speed;

    // If the activity/action has a default speed it has speed as an attribute
    m_has_speed = true;

}


/*----------------------------------------------------------------------------
  Set lowest possible speed
  ----------------------------------------------------------------------------*/
void ActAttributes::setLowestSpeed(float lowest_speed)
{

    m_lowest_speed = lowest_speed;

}


/*----------------------------------------------------------------------------
  Set highest possible speed
  ----------------------------------------------------------------------------*/
void ActAttributes::setHighestSpeed(float highest_speed)
{

    m_highest_speed = highest_speed;

}


/*----------------------------------------------------------------------------
  Return true if speed is used
  ----------------------------------------------------------------------------*/
bool ActAttributes::hasSpeed()
{

    return m_has_speed;

}


/*----------------------------------------------------------------------------
  Get default speed
  ----------------------------------------------------------------------------*/
float ActAttributes::getDefaultSpeed()
{

    return m_default_speed;

}


/*----------------------------------------------------------------------------
  Get lowest possible speed
  ----------------------------------------------------------------------------*/
float ActAttributes::getLowestSpeed()
{

    return m_lowest_speed;

}


/*----------------------------------------------------------------------------
  Get highest possible speed
  ----------------------------------------------------------------------------*/
float ActAttributes::getHighestSpeed()
{

    return m_highest_speed;

}

/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/src/rbody/ActAttributes.cpp,v $
 * $Revision: 1.1.1.1 $ 
 * $Date: 2005/02/06 06:52:59 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: ActAttributes.cpp,v $
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


