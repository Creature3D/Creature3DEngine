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
#ifndef CRCORE_CROccluderNode_H
#define CRCORE_CROccluderNode_H 1

#include <CRCore/crGroup.h>
#include <CRCore/crConvexPlanarOccluder.h>

namespace CRCore {

/** crOccluderNode is a crGroup node which allows crOccluderNodeing between children.
    Typical uses would be for objects which might need to be rendered
    differently at different times, for instance a crOccluderNode could be used 
    to represent the different states of a traffic light.
*/
class CR_EXPORT crOccluderNode : public crGroup
{
    public :
        
        crOccluderNode();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crOccluderNode(const crOccluderNode&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_Node(CRCore, crOccluderNode);
        

        /** Attach a crConvexPlanarOccluder to an crOccluderNode.*/            
        void setOccluder(crConvexPlanarOccluder* occluder) { m_occluder = occluder; }
        
        /** Get the crConvexPlanarOccluder* attached to a crOccluderNode. */
        crConvexPlanarOccluder* getOccluder() { return m_occluder.get(); }
        
        /** Get the const crConvexPlanarOccluder* attached to a crOccluderNode.*/            
        const crConvexPlanarOccluder* getOccluder() const { return m_occluder.get(); }


    protected :
    
        virtual ~crOccluderNode() {}
        
        /** Override's crGroup's computeBound.*/
        virtual bool computeBound() const;

        ref_ptr<crConvexPlanarOccluder>   m_occluder;
};

}

#endif
