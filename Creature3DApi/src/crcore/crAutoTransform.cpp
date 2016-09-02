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
#include <CRCore/crAutoTransform.h>
#include <CRCore/crCullStack.h>

using namespace CRCore;

crAutoTransform::crAutoTransform():
    m_autoUpdateEyeMovementTolerance(0.0f),
    m_autoRotateMode(NO_ROTATION),
    m_autoScaleToScreen(false),
    m_scale(1.0f,1.0f,1.0f),
    m_firstTimeToInitEyePoint(true),
    m_matrixDirty(true)
{
//    setNumChildrenRequiringUpdateTraversal(1);
}

crAutoTransform::crAutoTransform(const crAutoTransform& pat,const crCopyOp& copyop):
    crTransform(pat,copyop),
    m_position(pat.m_position),
    m_pivotPoint(pat.m_pivotPoint),
    m_autoUpdateEyeMovementTolerance(pat.m_autoUpdateEyeMovementTolerance),
    m_autoRotateMode(pat.m_autoRotateMode),
    m_autoScaleToScreen(pat.m_autoScaleToScreen),
    m_rotation(pat.m_rotation),
    m_scale(pat.m_scale),
    m_firstTimeToInitEyePoint(true),
    m_matrixDirty(true)
{
//    setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);            
}

bool crAutoTransform::computeLocalToWorldMatrix(crMatrix& matrix,crNodeVisitor*) const
{
    if (m_matrixDirty) computeMatrix();
    
    if (m_referenceFrame==RELATIVE_TO_PARENTS)
    {
        matrix.preMult(m_cachedMatrix);
    }
    else // absolute
    {
        matrix = m_cachedMatrix;
    }
    return true;
}


bool crAutoTransform::computeWorldToLocalMatrix(crMatrix& matrix,crNodeVisitor*) const
{
    if (m_referenceFrame==RELATIVE_TO_PARENTS)
    {
        matrix.postMult(CRCore::crMatrix::translate(-m_position)*
                        CRCore::crMatrix::rotate(m_rotation.inverse())*
                        CRCore::crMatrix::scale(1.0f/m_scale.x(),1.0f/m_scale.y(),1.0f/m_scale.z())*
                        CRCore::crMatrix::translate(m_pivotPoint));
    }
    else // absolute
    {
        matrix = CRCore::crMatrix::translate(-m_position)*
                 CRCore::crMatrix::rotate(m_rotation.inverse())*
                 CRCore::crMatrix::scale(1.0f/m_scale.x(),1.0f/m_scale.y(),1.0f/m_scale.z())*
                 CRCore::crMatrix::translate(m_pivotPoint);
    }
    return true;
}

void crAutoTransform::computeMatrix() const
{
    if (!m_matrixDirty) return;
    
    m_cachedMatrix.set(CRCore::crMatrix::translate(-m_pivotPoint)*
                      CRCore::crMatrix::scale(m_scale)*
                      CRCore::crMatrix::rotate(m_rotation)*
                      CRCore::crMatrix::translate(m_position));
    
    m_matrixDirty = false;
}

void crAutoTransform::accept(crNodeVisitor& nv)
{
    // if app traversal update the frame count.
    if (nv.getVisitorType()==crNodeVisitor::UPDATE_VISITOR)
    {
    }
    else
    if (nv.getVisitorType()==crNodeVisitor::CULL_VISITOR)
    {

        crCullStack* cs = dynamic_cast<crCullStack*>(&nv);
        if (cs)
        {

            int width = m_previousWidth;
            int height = m_previousHeight;

            CRCore::crViewPort* viewport = cs->getViewport();
            if (viewport)
            {
                width = viewport->width();
                height = viewport->height();
            }

            CRCore::crVector3 eyePoint = cs->getEyeLocal(); 
            CRCore::crVector3 localUp = cs->getUpLocal(); 
            CRCore::crVector3 position = getPosition();

            const CRCore::crMatrix& projection = cs->getProjectionMatrix();

            bool doUpdate = m_firstTimeToInitEyePoint;
            if (!m_firstTimeToInitEyePoint)
            {
                CRCore::crVector3 dv = m_previousEyePoint-eyePoint;
                if (dv.length2()>getAutoUpdateEyeMovementTolerance()*(eyePoint-getPosition()).length2())
                {
                    doUpdate = true;
                }
                CRCore::crVector3 dupv = m_previousLocalUp-localUp;
                // rotating the camera only affects ROTATE_TO_*
                if (m_autoRotateMode &&
                    dupv.length2()>getAutoUpdateEyeMovementTolerance())
                {
                    doUpdate = true;
                }
                else if (width!=m_previousWidth || height!=m_previousHeight)
                {
                    doUpdate = true;
                }
                else if (projection != m_previousProjection) 
                {
                    doUpdate = true;
                }                
                else if (position != m_previousPosition) 
                { 
                    doUpdate = true; 
                } 
            }
            m_firstTimeToInitEyePoint = false;

            if (doUpdate)
            {            

                if (getAutoScaleToScreen())
                {
                    float size = 1.0f/cs->pixelSize(getPosition(),1.0f);
                    setScale(size);
                }

                if (m_autoRotateMode==ROTATE_TO_SCREEN)
                {
                    CRCore::crQuat rotation;
                    cs->getModelViewMatrix().get(rotation);            
                    setRotation(rotation.inverse());
                }
                else if (m_autoRotateMode==ROTATE_TO_CAMERA)
                {
                    CRCore::crVector3 PosToEye = m_position - eyePoint;
                    CRCore::crMatrix lookto = CRCore::crMatrix::lookAt(
                        CRCore::crVector3(0,0,0), PosToEye, localUp);
                    crQuat q;
                    q.set(CRCore::crMatrix::inverse(lookto));
                    setRotation(q);
                }

                m_previousEyePoint = eyePoint;
                m_previousLocalUp = localUp;
                m_previousWidth = width;
                m_previousHeight = height;
                m_previousProjection = projection;
                m_previousPosition = position;

                m_matrixDirty = true;
            }

        }
    }
    
    // now do the proper accept
    crTransform::accept(nv);
}
