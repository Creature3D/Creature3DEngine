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
#include <CRCore/crCoordinateSystemNode.h>
#include <CRCore/crNotify.h>

using namespace CRCore;


crCoordinateSystemNode::crCoordinateSystemNode()
{
}

crCoordinateSystemNode::crCoordinateSystemNode(const std::string& format, const std::string& cs):
    m_format(format),
    m_cs(cs)
{
}

crCoordinateSystemNode::crCoordinateSystemNode(const crCoordinateSystemNode& csn,const CRCore::crCopyOp& copyop):
    crGroup(csn,copyop),
    m_format(csn.m_format),
    m_cs(csn.m_cs),
    m_ellipsoidModel(csn.m_ellipsoidModel)
{
}

void crCoordinateSystemNode::set(const crCoordinateSystemNode& csn)
{
    m_format = csn.m_format;
    m_cs = csn.m_cs;
    m_ellipsoidModel = csn.m_ellipsoidModel;
}

CoordinateFrame crCoordinateSystemNode::computeLocalCoordinateFrame(const crVector3d& position) const
{
    if (m_ellipsoidModel.valid())
    {
        crMatrixd localToWorld;
        
        double latitude, longitude, height;        
        m_ellipsoidModel->convertXYZToLatLongHeight(position.x(),position.y(),position.z(),latitude, longitude, height);
        m_ellipsoidModel->computeLocalToWorldTransformFromLatLongHeight(latitude, longitude, 0.0f, localToWorld);
        
        //CRCore::notify(CRCore::INFO)<<"lat="<<latitude<<"\tlong="<<longitude<<"\theight"<<height<<std::endl;
    
        //m_ellipsoidModel->computeLocalToWorldTransformFromXYZ(position.x(),position.y(),position.z(), localToWorld);

        return localToWorld;
    }
    else
    {
        return crMatrixd::translate(position.x(),position.y(),0.0f);
    }
}

CRCore::crVector3d crCoordinateSystemNode::computeLocalUpVector(const crVector3d& position) const
{
    if (m_ellipsoidModel.valid())
    {
        return m_ellipsoidModel->computeLocalUpVector(position.x(),position.y(),position.z());
    }
    else
    {
        return CRCore::crVector3d(0.0f,0.0f,1.0f);
    }
}


