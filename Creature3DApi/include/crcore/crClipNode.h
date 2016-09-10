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
#ifndef CRCORE_CLIPNODE_H
#define CRCORE_CLIPNODE_H 1

#include <CRCore/crGroup.h>
#include <CRCore/crClipPlane.h>

namespace CRCore {

/** Node for defining the position of ClipPlanes in the scene.*/
class CR_EXPORT crClipNode : public crGroup
{

    public:

        typedef std::vector<ref_ptr<crClipPlane> > ClipPlaneList;


        crClipNode();

        crClipNode(const crClipNode& es, const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_Node(CRCore, crClipNode);
	
	/** Create a 6 clip planes to create a clip box.*/
	void createClipBox(const crBoundingBox& bb,unsigned int clipPlaneNumberBase=0);


        /** Add a crClipPlane to a crClipNode. Return true if plane is added, 
          * return false if plane already exists in crClipNode, or clipplane is false.*/
        bool addClipPlane(crClipPlane* clipplane);

        /** Remove crClipPlane from a crClipNode. Return true if plane is removed, 
          * return false if plane does not exists in crClipNode.*/
        bool removeClipPlane(crClipPlane* clipplane);

        /** Remove crClipPlane, at specified index, from a crClipNode. Return true if plane is removed, 
          * return false if plane does not exists in crClipNode.*/
        bool removeClipPlane(unsigned int pos);

        /** return the number of ClipPlanes.*/
        inline unsigned  int getNumClipPlanes() const { return m_planes.size(); }


        /** Get crClipPlane at specificed index position.*/
        inline crClipPlane* getClipPlane(unsigned int pos) { return m_planes[pos].get(); }

        /** Get const crClipPlane at specificed index position.*/
        inline const crClipPlane* getClipPlane(unsigned int pos) const { return m_planes[pos].get(); }

        /** Get the ClipPlaneList.*/
        inline ClipPlaneList& getClipPlaneList() { return m_planes; }

        /** Get the const ClipPlaneList.*/
        inline const ClipPlaneList& getClipPlaneList() const { return m_planes; }

        /** Set the GLModes on crStateSet associated with the ClipPlanes.*/
        void setStateSetModes(crStateSet&,crStateAttribute::VRModeValue) const;

        /** Set up the local crStateSet */
        void setLocalStateSetModes(crStateAttribute::VRModeValue=crStateAttribute::ON);

    protected:

        virtual ~crClipNode();

        virtual bool computeBound() const;

        crStateAttribute::VRModeValue m_value;
        ClipPlaneList m_planes;
};

}

#endif
