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
#include <Plugin cre/crBase.h>
#include <Plugin cre/crConvexPlanarOccluder.h>
#include <Plugin cre/crConvexPlanarPolygon.h>

using namespace cre;

void crConvexPlanarOccluder::write(crDataOutputStream* out){
    // Write crConvexPlanarOccluder's identification.
    out->writeInt(CRECONVEXPLANAROCCLUDER);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
    if(obj){
        ((cre::crBase*)(obj))->write(out);
    }
    else
        throw crException("crConvexPlanarOccluder::write(): Could not cast this CRCore::crConvexPlanarOccluder to an CRCore::crBase.");
    // Write crConvexPlanarOccluder's properties.

    // Write planar polygon occluder.
    ((cre::crConvexPlanarPolygon*)(&getOccluder()))->write(out);

    // Write hole list.
    HoleList holeList = getHoleList();
    int size = holeList.size();
    out->writeInt(size);
    for(int i=0; i<size; i++){
        ((cre::crConvexPlanarPolygon*)(&holeList[i]))->write(out);
    }
}

void crConvexPlanarOccluder::read(crDataInputStream* in){
    // Peek on crConvexPlanarOccluder's identification.
    int id = in->peekInt();
    if(id == CRECONVEXPLANAROCCLUDER){
        // Read crConvexPlanarOccluder's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
        if(obj){
            ((cre::crBase*)(obj))->read(in);
        }
        else
            throw crException("crConvexPlanarOccluder::read(): Could not cast this CRCore::crConvexPlanarOccluder to an CRCore::crBase.");
        // Read crConvexPlanarOccluder's properties

        // Read planar polygon occluder.
        CRCore::crConvexPlanarPolygon* cpp = &getOccluder();
        ((cre::crConvexPlanarPolygon*)(cpp))->read(in);

        // Read hole list.
        int size = in->readInt();
        for(int i=0; i<size; i++){
            CRCore::crConvexPlanarPolygon* cpp = new CRCore::crConvexPlanarPolygon();
            ((cre::crConvexPlanarPolygon*)(cpp))->read(in);
            addHole(*cpp);
        }

    }
    else{
        throw crException("crConvexPlanarOccluder::read(): Expected crConvexPlanarOccluder identification.");
    }
}
