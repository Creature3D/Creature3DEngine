/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/

#include <Plugin cre/crException.h>
#include <Plugin cre/crOccluderNode.h>
#include <Plugin cre/crGroup.h>
#include <Plugin cre/crConvexPlanarOccluder.h>

using namespace cre;

void crOccluderNode::write(crDataOutputStream* out){
    // Write crOccluderNode's identification.
    out->writeInt(CREOCCLUDERNODE);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
    if(group){
        ((cre::crGroup*)(group))->write(out);
    }
    else
        throw crException("crOccluderNode::write(): Could not cast this CRCore::crOccluderNode to an CRCore::crGroup.");
    // Write crOccluderNode's properties.
    out->writeBool(getOccluder()!=0);
    if(getOccluder())
        ((cre::crConvexPlanarOccluder*)(getOccluder()))->write(out);
}

void crOccluderNode::read(crDataInputStream* in)
{
    // Peek on crOccluderNode's identification.
    int id = in->peekInt();
    if(id == CREOCCLUDERNODE)
    {
        // Read crOccluderNode's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
        if(group){
            ((cre::crGroup*)(group))->read(in);
        }
        else
            throw crException("crOccluderNode::read(): Could not cast this CRCore::crOccluderNode to an CRCore::crGroup.");
        // Read crOccluderNode's properties
        if(in->readBool()){
            CRCore::crConvexPlanarOccluder* cpo = new CRCore::crConvexPlanarOccluder();
            ((cre::crConvexPlanarOccluder*)(cpo))->read(in);
            setOccluder(cpo);
        }
    }
    else{
        throw crException("crOccluderNode::read(): Expected crOccluderNode identification.");
    }
}
