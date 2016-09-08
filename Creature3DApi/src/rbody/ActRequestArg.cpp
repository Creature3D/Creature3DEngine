//  $Id: ActRequestArg.cpp,v 1.1.1.1 2005/02/06 06:52:59 deepone Exp $

#include "rbody/ActRequestArg.h"

using namespace rbody;


/*----------------------------------------------------------------------------
  Constructor
  ----------------------------------------------------------------------------*/
ActRequestArg::ActRequestArg()
{

    m_has_direction = false;
    m_has_degree = false;
    m_has_speed = false;

}


/*----------------------------------------------------------------------------
  Destructor
  ----------------------------------------------------------------------------*/
ActRequestArg::~ActRequestArg()
{

}


/*----------------------------------------------------------------------------
  Set direction
  ----------------------------------------------------------------------------*/
void ActRequestArg::setDirection(CRCore::crVector3 direction)
{

    m_direction = direction;
    m_has_direction = true;

}


/*----------------------------------------------------------------------------
  Return true if direction is used
  ----------------------------------------------------------------------------*/
bool ActRequestArg::hasDirection()
{

    return m_has_direction;

}


/*----------------------------------------------------------------------------
  Get direction
  ----------------------------------------------------------------------------*/
void ActRequestArg::getDirection(CRCore::crVector3 &direction)
{

    direction = m_direction;

}


/*----------------------------------------------------------------------------
  Set degree
  ----------------------------------------------------------------------------*/
void ActRequestArg::setDegree(float degree)
{

    m_degree = degree;

    m_has_degree = true;

}


/*----------------------------------------------------------------------------
  Return true if degree is used
  ----------------------------------------------------------------------------*/
bool ActRequestArg::hasDegree()
{

    return m_has_degree;

}


/*----------------------------------------------------------------------------
  Get degree
  ----------------------------------------------------------------------------*/
float ActRequestArg::getDegree()
{

    return m_degree;

}


/*----------------------------------------------------------------------------
  Set speed
  ----------------------------------------------------------------------------*/
void ActRequestArg::setSpeed(float speed)
{

    m_speed = speed;

    m_has_speed = true;

}


/*----------------------------------------------------------------------------
  Return true if speed is used
  ----------------------------------------------------------------------------*/
bool ActRequestArg::hasSpeed()
{

    return m_has_speed;

}


/*----------------------------------------------------------------------------
  Get speed
  ----------------------------------------------------------------------------*/
float ActRequestArg::getSpeed()
{

    return m_speed;

}


/*----------------------------------------------------------------------------
  Add animation weight
  ----------------------------------------------------------------------------*/
void ActRequestArg::addWeight(float weight)
{

    m_weights_vect.push_back(weight);

}


/*----------------------------------------------------------------------------
  Get weight at postition
  ----------------------------------------------------------------------------*/
float ActRequestArg::getWeightAt(int nr)
{

    return m_weights_vect[nr];

}


/*----------------------------------------------------------------------------
  Get number of weights
  ----------------------------------------------------------------------------*/
int ActRequestArg::getNrOfWeights()
{

    return m_weights_vect.size();

}

/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/src/rbody/ActRequestArg.cpp,v $
 * $Revision: 1.1.1.1 $ 
 * $Date: 2005/02/06 06:52:59 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: ActRequestArg.cpp,v $
 * Revision 1.1.1.1  2005/02/06 06:52:59  deepone
 * initial sourceforge checkin
 *
 * Revision 1.2  2003/11/06 08:24:37  andersb
 * Removed the use of rbody::crVector3
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

