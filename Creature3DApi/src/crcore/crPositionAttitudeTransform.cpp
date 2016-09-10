/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
//Modified by Wucaihua
#include <CRCore/crPositionAttitudeTransform.h>

using namespace CRCore;

crPositionAttitudeTransform::crPositionAttitudeTransform():
  m_scale(1.0f,1.0f,1.0f)
{
}

bool crPositionAttitudeTransform::computeLocalToWorldMatrix(crMatrix& crMatrix,crNodeVisitor*) const
{
    if (m_referenceFrame==RELATIVE_TO_PARENTS)
    {
        crMatrix.preMult( CRCore::crMatrix::translate(-m_pivotPoint)*
			              CRCore::crMatrix::scale(m_scale)*
                          CRCore::crMatrix::rotate(m_attitude)*
                          CRCore::crMatrix::translate(m_position));
    }
    else // absolute
    {
        crMatrix = CRCore::crMatrix::translate(-m_pivotPoint)*
                   CRCore::crMatrix::scale(m_scale)*
                   CRCore::crMatrix::rotate(m_attitude)*
                   CRCore::crMatrix::translate(m_position);
    }
    return true;
}


bool crPositionAttitudeTransform::computeWorldToLocalMatrix(crMatrix& crMatrix,crNodeVisitor*) const
{
    if (m_referenceFrame==RELATIVE_TO_PARENTS)
    {
        crMatrix.postMult( CRCore::crMatrix::translate(-m_position)*
                           CRCore::crMatrix::rotate(m_attitude.inverse())*
						   CRCore::crMatrix::scale(1.0f/m_scale.x(),1.0f/m_scale.y(),1.0f/m_scale.z())*
                           CRCore::crMatrix::translate(m_pivotPoint));
    }
    else // absolute
    {
        crMatrix = CRCore::crMatrix::translate(-m_position)*
                   CRCore::crMatrix::rotate(m_attitude.inverse())*
				   CRCore::crMatrix::scale(1.0f/m_scale.x(),1.0f/m_scale.y(),1.0f/m_scale.z())*
                   CRCore::crMatrix::translate(m_pivotPoint);
    }
    return true;
}
