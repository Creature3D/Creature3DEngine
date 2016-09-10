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
#include <Plugin cre/crClipNode.h>
#include <Plugin cre/crClipPlane.h>
#include <Plugin cre/crGroup.h>
#include <Plugin cre/crLight.h>

using namespace cre;

void crClipNode::write(crDataOutputStream* out){
    // Write crClipNode's identification.
    out->writeInt(CRECLIPNODE);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
    if(group){
        ((cre::crGroup*)(group))->write(out);
    }
    else
        throw crException("crClipNode::write(): Could not cast this CRCore::crClipNode to an CRCore::crGroup.");
    // Write crClipNode's properties.

    out->writeUInt(getNumClipPlanes());

    for (unsigned int i=0;i<getNumClipPlanes();++i)
    {
        ((cre::crClipPlane*)getClipPlane(i))->write(out);
    }

}

void crClipNode::read(crDataInputStream* in){
    // Peek on crClipNode's identification.
    int id = in->peekInt();
    if(id == CRECLIPNODE){
        // Read crClipNode's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
        if(group){
            ((cre::crGroup*)(group))->read(in);
        }
        else
            throw crException("crClipNode::read(): Could not cast this CRCore::crClipNode to an CRCore::crBase.");
        // Read crClipNode's properties

        unsigned int numClipPlanes = in->readUInt();

        for (unsigned int i=0;i<numClipPlanes;++i)
        {
            CRCore::crClipPlane* clipPlane = new CRCore::crClipPlane;
            ((cre::crClipPlane*)clipPlane)->read(in);
            addClipPlane(clipPlane);
        }

    }
    else{
        throw crException("crClipNode::read(): Expected crClipNode identification.");
    }
}
