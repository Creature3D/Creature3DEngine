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
//Modified by 吴财华
#include <CRCore/crMatrixTransform.h>

using namespace CRCore;

crMatrixTransform::crMatrixTransform():
    m_inverseDirty(false)
{
}

crMatrixTransform::crMatrixTransform(const crMatrixTransform& transform,const crCopyOp& copyop):
    crTransform(transform,copyop),
    m_matrix(transform.m_matrix),
    m_inverse(transform.m_inverse),
    m_inverseDirty(transform.m_inverseDirty)
{    
}

crMatrixTransform::crMatrixTransform(const crMatrix& mat )
{
    m_referenceFrame = RELATIVE_TO_PARENTS;

    m_matrix = mat;
    m_inverseDirty = false;
}


crMatrixTransform::~crMatrixTransform()
{
}

bool crMatrixTransform::computeLocalToWorldMatrix(crMatrix& matrix,crNodeVisitor*) const
{
    if (m_referenceFrame==RELATIVE_TO_PARENTS)
    {
        matrix.preMult(m_matrix);
    }
    else // absolute
    {
        matrix = m_matrix;
    }
    return true;
}

bool crMatrixTransform::computeWorldToLocalMatrix(crMatrix& matrix,crNodeVisitor*) const
{
    const crMatrix& inverse = getInverseMatrix();

    if (m_referenceFrame==RELATIVE_TO_PARENTS)
    {
        matrix.postMult(inverse);
    }
    else // absolute
    {
        matrix = inverse;
    }
    return true;
}
