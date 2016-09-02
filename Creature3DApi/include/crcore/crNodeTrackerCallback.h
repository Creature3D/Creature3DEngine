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
#ifndef CRCORE_CRNODETRACKERCALLBACK
#define CRCORE_CRNODETRACKERCALLBACK 1

#include <CRCore/crNode.h>
#include <CRCore/crNodeCallback.h>
#include <CRCore/RefNodePath.h>

namespace CRCore
{

class CR_EXPORT crNodeTrackerCallback : public crNodeCallback
{
    public:


        void setTrackNodePath(const CRCore::RefNodePath& nodePath) { m_trackNodePath = nodePath; }
        void setTrackNodePath(const CRCore::NodePath& nodePath) { m_trackNodePath = nodePath; }

        CRCore::RefNodePath& getTrackNodePath() { return m_trackNodePath; }
        const CRCore::RefNodePath& getTrackNodePath() const { return m_trackNodePath; }

        void setTrackNode(CRCore::crNode* node);
        CRCore::crNode* getTrackNode() { return m_trackNodePath.empty() ? 0 : m_trackNodePath.back().get(); }
        const CRCore::crNode* getTrackNode() const { return m_trackNodePath.empty() ? 0 : m_trackNodePath.back().get(); }


        /** Implements the callback. */
        virtual void operator()(crNode* node, crNodeVisitor* nv);

        /** Update the node to track the nodepath.*/
        void update(CRCore::crNode& node);

        /** Validate the NodePath by removing any unref'd nodes.*/
        bool validateNodePath() const;

    protected:

        CRCore::RefNodePath        m_trackNodePath;

};

}

#endif
