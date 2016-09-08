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
#include <Plugin cre/crCoordinateSystemNode.h>
#include <Plugin cre/crEllipsoidModel.h>
#include <Plugin cre/crGroup.h>

using namespace cre;

void crCoordinateSystemNode::write(crDataOutputStream* out)
{
    // Write crCoordinateSystemNode's identification.
    out->writeInt(CRECOORDINATESYSTEMNODE);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
    if(group){
        ((cre::crGroup*)(group))->write(out);
    }
    else
        throw crException("crCoordinateSystemNode::write(): Could not cast this CRCore::crCoordinateSystemNode to an CRCore::crGroup.");
       // Write crCoordinateSystemNode's properties.

        out->writeString(getFormat());
        out->writeString(getCoordinateSystem());

        out->writeBool(getEllipsoidModel()!=0);
        if(getEllipsoidModel())
        {
            ((cre::crEllipsoidModel*)(getEllipsoidModel()))->write(out);
        }

}

void crCoordinateSystemNode::read(crDataInputStream* in){
    // Peek on crCoordinateSystemNode's identification.
    int id = in->peekInt();
    if(id == CRECOORDINATESYSTEMNODE)
    {
        // Read crCoordinateSystemNode's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
        if(group){
            ((cre::crGroup*)(group))->read(in);
        }
        else
            throw crException("crCoordinateSystemNode::read(): Could not cast this CRCore::crCoordinateSystemNode to an CRCore::crGroup.");
        // Read crCoordinateSystemNode's properties

        // Read format string
        setFormat( in->readString());

        // Read coord string
        setCoordinateSystem( in->readString());

        bool readEllopsoidModel = in->readBool();
        if (readEllopsoidModel)
        {
            CRCore::crEllipsoidModel* em = new CRCore::crEllipsoidModel();
            ((cre::crEllipsoidModel*)(em))->read(in);
            setEllipsoidModel(em);
        }

    }
    else{
        throw crException("crCoordinateSystemNode::read(): Expected crCoordinateSystemNode identification.");
    }
}
