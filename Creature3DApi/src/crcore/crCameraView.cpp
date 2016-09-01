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
#include <CRCore/crCameraView.h>

using namespace CRCore;

crCameraView::crCameraView():
    m_fieldOfView(60.0),
    m_fieldOfViewMode(VERTICAL),
    m_focalLength(0.0)
{
}

bool crCameraView::computeLocalToWorldMatrix(crMatrix& matrix,crNodeVisitor*) const
{
    if (m_referenceFrame==RELATIVE_TO_PARENTS)
    {
        matrix.preMult(CRCore::crMatrix::rotate(m_attitude)*
                       CRCore::crMatrix::translate(m_position));
    }
    else // absolute
    {
        matrix = CRCore::crMatrix::rotate(m_attitude)*
                 CRCore::crMatrix::translate(m_position);
    }
    return true;
}


bool crCameraView::computeWorldToLocalMatrix(crMatrix& matrix,crNodeVisitor*) const
{
    if (m_referenceFrame==RELATIVE_TO_PARENTS)
    {
        matrix.postMult(CRCore::crMatrix::translate(-m_position)*
                        CRCore::crMatrix::rotate(m_attitude.inverse()));
    }
    else // absolute
    {
        matrix = CRCore::crMatrix::translate(-m_position)*
                 CRCore::crMatrix::rotate(m_attitude.inverse());
    }
    return true;
}
