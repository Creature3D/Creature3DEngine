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
#include <CRCore/crNodeTrackerCallback.h>
#include <CRCore/crNodeVisitor.h>
#include <CRCore/crMatrixTransform.h>
#include <CRCore/crPositionAttitudeTransform.h>
#include <CRCore/crCameraView.h>
#include <CRCore/crCameraNode.h>
#include <CRCore/crNotify.h>

using namespace CRCore;

class ApplyMatrixVisitor : public crNodeVisitor
{
    public:

        ApplyMatrixVisitor(const CRCore::crMatrix& matrix):
            m_matrix(matrix) {}

        virtual void apply(crCameraNode& camera)
        {
            camera.setViewMatrix(m_matrix);
        }

        virtual void apply(crCameraView& cv)
        {
            crQuat rotation;
            m_matrix.get(rotation);
            
            cv.setPosition(m_matrix.getTrans());
            cv.setAttitude(rotation);
        }

        virtual void apply(crMatrixTransform& mt)
        {
            mt.setMatrix(m_matrix);
        }
        
        virtual void apply(crPositionAttitudeTransform& pat)
        {
            crQuat rotation;
            m_matrix.get(rotation);
            
            pat.setPosition(m_matrix.getTrans());
            pat.setAttitude(rotation);
        }
        
        CRCore::crMatrix m_matrix;
};


void crNodeTrackerCallback::setTrackNode(CRCore::crNode* node)
{
    if (!node)
    {
        CRCore::notify(CRCore::NOTICE)<<"crNodeTrackerCallback::setTrackNode(crNode*):  Unable to set tracked node due to null crNode*"<<std::endl;
        return;
    }

    NodePathList parentNodePaths = node->getParentalNodePaths();

    if (!parentNodePaths.empty())
    {
        //CRCore::notify(CRCore::INFO)<<"crNodeTrackerCallback::setTrackNode(crNode*): Path set"<<std::endl;
        m_trackNodePath = parentNodePaths[0];
    }
    else
    {
        CRCore::notify(CRCore::NOTICE)<<"crNodeTrackerCallback::setTrackNode(crNode*): Unable to set tracked node due to empty parental path."<<std::endl;
    }
}

void crNodeTrackerCallback::operator()(crNode* node, crNodeVisitor* nv)
{
    if (nv->getVisitorType()==crNodeVisitor::UPDATE_VISITOR)
    {
        update(*node);
    }

    traverse(node,nv);
}

void crNodeTrackerCallback::update(CRCore::crNode& node)
{
    ApplyMatrixVisitor applyMatrix(computeWorldToLocal(NodePath(m_trackNodePath)));
    node.accept(applyMatrix);
}

bool crNodeTrackerCallback::validateNodePath() const
{
    if (!m_trackNodePath.valid())
    {
        if (m_trackNodePath.empty())
        {
            CRCore::notify(CRCore::NOTICE)<<"Warning: tracked node path has been invalidated by changes in the scene graph."<<std::endl;
            crNodeTrackerCallback* non_const_this = const_cast<crNodeTrackerCallback*>(this);
            non_const_this->m_trackNodePath.clear();
        }
        return false;
    }
    return true;
}

