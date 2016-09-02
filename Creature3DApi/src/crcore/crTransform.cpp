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
#include <CRCore/crTransform.h>

using namespace CRCore;

class crTransformVisitor : public crNodeVisitor
{
    public:
    
        enum CoordMode
        {
            WORLD_TO_LOCAL,
            LOCAL_TO_WORLD
        };
        

        CoordMode       m_CoordMode;
        crMatrix&       m_matrix;

        crTransformVisitor(crMatrix& crMatrix,CoordMode coordMode):
            crNodeVisitor(),
            m_CoordMode(coordMode),
            m_matrix(crMatrix)
            {}

        virtual void apply(crTransform& crTransform)
        {
            if (m_CoordMode==LOCAL_TO_WORLD)
            {
                crTransform.computeLocalToWorldMatrix(m_matrix,this);
            }
            else // worldToLocal
            {
                crTransform.computeWorldToLocalMatrix(m_matrix,this);
            }
        }
        
        void accumulate(NodePath& nodePath)
        {
            for(NodePath::iterator itr=nodePath.begin();
                itr!=nodePath.end();
                ++itr)
            {
                (*itr)->accept(*this);
            }
        }
    
};

crMatrix CRCore::computeLocalToWorld(NodePath& nodePath)
{
    crMatrix crMatrix;
    crTransformVisitor tv(crMatrix,crTransformVisitor::LOCAL_TO_WORLD);
    tv.accumulate(nodePath);
    return crMatrix;
}

crMatrix CRCore::computeWorldToLocal(NodePath& nodePath)
{
    CRCore::crMatrix crMatrix;
    crTransformVisitor tv(crMatrix,crTransformVisitor::WORLD_TO_LOCAL);
    tv.accumulate(nodePath);
    return crMatrix;
}

crMatrix CRCore::computeLocalToEye(const crMatrix& modelview,NodePath& nodePath)
{
    crMatrix crMatrix(modelview);
    crTransformVisitor tv(crMatrix,crTransformVisitor::LOCAL_TO_WORLD);
    tv.accumulate(nodePath);
    return crMatrix;
}

crMatrix CRCore::computeEyeToLocal(const crMatrix& modelview,NodePath& nodePath)
{
    crMatrix crMatrix;
    crMatrix.invert(modelview);
    crTransformVisitor tv(crMatrix,crTransformVisitor::WORLD_TO_LOCAL);
    tv.accumulate(nodePath);
    return crMatrix;
}





crTransform::crTransform()
{
    m_referenceFrame = RELATIVE_TO_PARENTS;
	m_dataVariance = DYNAMIC;
}

crTransform::crTransform(const crTransform& crTransform,const crCopyOp& copyop):
    crGroup(crTransform,copyop),
    m_referenceFrame(crTransform.m_referenceFrame)
{    
}

crTransform::~crTransform()
{
}

void crTransform::setReferenceFrame(ReferenceFrame rf)
{
    if (m_referenceFrame == rf) return;
    
    m_referenceFrame = rf;
    
    // switch off culling if crTransform is absolute.
    if (m_referenceFrame==RELATIVE_TO_ABSOLUTE) setCullingActive(false);
    else setCullingActive(true);
}

bool crTransform::computeBound() const
{
    if (!crGroup::computeBound()) return false;
    
    // note, NULL pointer for NodeVisitor, so compute's need
    // to handle this case gracefully, normally this should not be a problem.
    crMatrix l2w;

    computeLocalToWorldMatrix(l2w,NULL);

	m_bbox.m_max = m_bbox.m_max * l2w;
	m_bbox.m_min = m_bbox.m_min * l2w;

	m_bbox.correct();

    crVector3 xdash = m_boundSphere.m_center;
    xdash.x() += m_boundSphere.m_fRadius;
    xdash = xdash*l2w;

    crVector3 ydash = m_boundSphere.m_center;
    ydash.y() += m_boundSphere.m_fRadius;
    ydash = ydash*l2w;

    crVector3 zdash = m_boundSphere.m_center;
    zdash.z() += m_boundSphere.m_fRadius;
    zdash = zdash*l2w;

    m_boundSphere.m_center = m_boundSphere.m_center*l2w;

    xdash -= m_boundSphere.m_center;
    float len_xdash = xdash.length();

    ydash -= m_boundSphere.m_center;
    float len_ydash = ydash.length();

    zdash -= m_boundSphere.m_center;
    float len_zdash = zdash.length();

    m_boundSphere.m_fRadius = len_xdash;
    if (m_boundSphere.m_fRadius<len_ydash) m_boundSphere.m_fRadius = len_ydash;
    if (m_boundSphere.m_fRadius<len_zdash) m_boundSphere.m_fRadius = len_zdash;

    return true;

}
