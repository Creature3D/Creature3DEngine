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
#include <CRCore/crProxyNode.h>
#include <CRCore/crCullStack.h>
#include <CRCore/crNotify.h>

using namespace CRCore;

crProxyNode::crProxyNode() : 
    m_centerMode(USER_DEFINED_CENTER),
    m_radius(-1)
{
} 

crProxyNode::crProxyNode(const crProxyNode& proxynode,const crCopyOp& copyop):
    crGroup(proxynode,copyop),
    m_filenameList(proxynode.m_filenameList),
    m_centerMode(proxynode.m_centerMode),
    m_userDefinedCenter(proxynode.m_userDefinedCenter),
    m_radius(proxynode.m_radius)
{
}

void crProxyNode::setDatabasePath(const std::string& path)
{
    m_databasePath = path;
    if (!m_databasePath.empty())
    {
        char& lastCharacter = m_databasePath[m_databasePath.size()-1];
        const char unixSlash = '/';
        const char winSlash = '\\';

        if (lastCharacter==winSlash)
        {
            lastCharacter = unixSlash;
        }
        else if (lastCharacter!=unixSlash)
        {
            m_databasePath += unixSlash;
        }
    }
}

void crProxyNode::traverse(crNodeVisitor& nv)
{
    if (m_filenameList.size()>m_childArray.size() && nv.getVisitorType()==crNodeVisitor::CULL_VISITOR)
    {
        for(unsigned int i=m_childArray.size(); i<m_filenameList.size(); ++i)
        {
            nv.getDatabaseRequestHandler()->requestNodeFile(m_databasePath+m_filenameList[i], this, 1.0f/*, nv.getFrameStamp()*/);
        }
    }
    else
    {
        crGroup::traverse(nv);
    }
}

void crProxyNode::expandFileNameListTo(unsigned int pos)
{
    if (pos>=m_filenameList.size()) m_filenameList.resize(pos+1);
}

bool crProxyNode::addChild( crNode *child )
{
    if (crGroup::addChild(child))
    {
        expandFileNameListTo(m_childArray.size()-1);
        return true;
    }
    return false;
}

bool crProxyNode::addChild(crNode *child, const std::string& filename)
{
    if (crGroup::addChild(child))
    {
        setFileName(m_childArray.size()-1,filename);
        return true;
    }
    return false;
}

bool crProxyNode::removeChild( crNode *child )
{
    // find the child's position.
    unsigned int pos=getChildIndex(child);
    if (pos==m_childArray.size()) return false;
    
    if (pos<m_filenameList.size()) m_filenameList.erase(m_filenameList.begin()+pos);
    
    return crGroup::removeChild(child);    
}

bool crProxyNode::computeBound() const
{
    if (m_centerMode==USER_DEFINED_CENTER && m_radius>=0.0f)
    {
        //return crBoundingSphere(m_userDefinedCenter,m_radius);
		return true;
    }
    else
    {
        return crGroup::computeBound();
    }
}


