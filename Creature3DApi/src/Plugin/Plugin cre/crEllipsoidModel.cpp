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
#include <Plugin cre/crEllipsoidModel.h>
#include <Plugin cre/crConvexPlanarPolygon.h>

using namespace cre;

void crEllipsoidModel::write(crDataOutputStream* out){
    // Write crEllipsoidModel's identification.
    out->writeInt(CREELLIPSOIDMODEL);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
    if(obj){
        ((cre::crBase*)(obj))->write(out);
    }
    else
        throw crException("crEllipsoidModel::write(): Could not cast this CRCore::crEllipsoidModel to an CRCore::crBase.");
    // Write crEllipsoidModel's properties.

    out->writeDouble(getRadiusEquator());
    out->writeDouble(getRadiusPolar());

}

void crEllipsoidModel::read(crDataInputStream* in){
    // Peek on crEllipsoidModel's identification.
    int id = in->peekInt();
    if(id == CREELLIPSOIDMODEL){
        // Read crEllipsoidModel's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
        if(obj){
            ((cre::crBase*)(obj))->read(in);
        }
        else
            throw crException("crEllipsoidModel::read(): Could not cast this CRCore::crEllipsoidModel to an CRCore::crBase.");
        // Read crEllipsoidModel's properties

        setRadiusEquator(in->readDouble());
        setRadiusPolar(in->readDouble());
        

    }
    else{
        throw crException("crEllipsoidModel::read(): Expected crEllipsoidModel identification.");
    }
}
