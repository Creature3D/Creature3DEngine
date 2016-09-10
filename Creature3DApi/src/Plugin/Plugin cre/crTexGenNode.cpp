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
#include <Plugin cre/crTexGenNode.h>
#include <Plugin cre/crGroup.h>
#include <Plugin cre/crTexGen.h>

using namespace cre;

void crTexGenNode::write(crDataOutputStream* out){
    // Write crTexGenNode's identification.
    out->writeInt(CRETEXGENNODE);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
    if(group){
        ((cre::crGroup*)(group))->write(out);
    }
    else
        throw crException("crTexGenNode::write(): Could not cast this CRCore::crTexGenNode to an CRCore::crGroup.");
    // Write crTexGenNode's properties.


    out->writeUInt(getTextureUnit());

    // Write out light
    out->writeBool(getTexGen()!=0);
    if(getTexGen())
    {
        ((cre::crTexGen*)(getTexGen()))->write(out);
    }
}

void crTexGenNode::read(crDataInputStream* in){
    // Peek on crTexGenNode's identification.
    int id = in->peekInt();
    if(id == CRETEXGENNODE){
        // Read crTexGenNode's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
        if(group){
            ((cre::crGroup*)(group))->read(in);
        }
        else
            throw crException("crTexGenNode::read(): Could not cast this CRCore::crTexGenNode to an CRCore::crBase.");
        // Read crTexGenNode's properties


        setTextureUnit(in->readUInt());

        // Read texgen
        if(in->readBool()){
            CRCore::crTexGen* texgen = new CRCore::crTexGen();
            ((cre::crTexGen*)(texgen))->read(in);
            setTexGen(texgen);
        }
    }
    else{
        throw crException("crTexGenNode::read(): Expected crTexGenNode identification.");
    }
}
