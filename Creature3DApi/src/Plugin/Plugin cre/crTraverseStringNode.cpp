/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
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
#include <Plugin cre/crGroup.h>
#include <Plugin cre/crTraverseStringNode.h>
using namespace cre;

void crTraverseStringNode::write(crDataOutputStream* out){
    out->writeInt(CRTRAVERSESTRINGNODE);
    CRCore::crGroup* node = dynamic_cast<CRCore::crGroup*>(this);
    if(node){
        static_cast<cre::crGroup*>(node)->write(out);
    }
    else
        throw crException("crTraverseStringNode::write(): Could not cast this crTraverseStringNode to an crGroup.");
}

void crTraverseStringNode::read(crDataInputStream* in){
    int id = in->peekInt();
    if(id == CRTRAVERSESTRINGNODE){
        id = in->readInt();

        CRCore::crGroup* node = dynamic_cast<CRCore::crGroup*>(this);
        if(node){
            ((cre::crGroup*)(node))->read(in);
        }
        else
            throw crException("crTraverseStringNode::read(): Could not cast this crTraverseStringNode to an crGroup.");
    }
    else{
        throw crException("crTraverseStringNode::read(): Expected identification");
    }
}
