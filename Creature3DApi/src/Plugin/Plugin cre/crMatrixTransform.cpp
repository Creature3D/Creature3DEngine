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
#include <Plugin cre/crMatrixTransform.h>
#include <Plugin cre/crGroup.h>

using namespace cre;

void crMatrixTransform::write(crDataOutputStream* out){
    // Write crMatrixTransform's identification.
    out->writeInt(CREMATRIXTRANSFORM);

    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
    if(group){
        ((cre::crGroup*)(group))->write(out);
    }
    else
        throw crException("crMatrixTransform::write(): Could not cast this CRCore::crMatrixTransform to an CRCore::crGroup.");


    // Write crMatrixTransform's properties.
    out->writeMatrixd(getMatrix());
}

void crMatrixTransform::read(crDataInputStream* in){
    // Read crMatrixTransform's identification.
    int id = in->peekInt();
    if(id == CREMATRIXTRANSFORM){
        // Code to read crMatrixTransform's properties.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
        if(group){
            ((cre::crGroup*)(group))->read(in);
        }
        else
            throw crException("crMatrixTransform::read(): Could not cast this CRCore::crMatrixTransform to an CRCore::crGroup.");

        // Read matrix
        setMatrix(in->readMatrixd());
    }
    else{
        throw crException("crMatrixTransform::read(): Expected crMatrixTransform identification");
    }
}
