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
#include <Plugin cre/crCameraView.h>
#include <Plugin cre/crTransform.h>
#include <CRCore/crNotify.h>

using namespace cre;

void crCameraView::write(crDataOutputStream* out)
{
    // Write crCameraView's identification.
    out->writeInt(CRECAMERAVIEW);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crTransform*  trans = dynamic_cast<CRCore::crTransform*>(this);
    if(trans)
    {
        ((cre::crTransform*)(trans))->write(out);
    }
    else
        throw crException("crCameraView::write(): Could not cast this CRCore::crCameraView to an CRCore::crTransform.");

    // Write crCameraView's properties.      
    out->writeVec3(getPosition());
    out->writeQuat(getAttitude());
    out->writeDouble(getFieldOfView());
    out->writeInt(getFieldOfViewMode());
    out->writeDouble(getFocalLength());
}

void crCameraView::read(crDataInputStream* in){
    // Peek on crCameraView's identification.
    int id = in->peekInt();
    if(id == CRECAMERAVIEW)
    {
        // Read crCameraView's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crTransform*  trans = dynamic_cast<CRCore::crTransform*>(this);
        if(trans)
        {
            ((cre::crTransform*)(trans))->read(in);
        }
        else
            throw crException("crCameraView::read(): Could not cast this CRCore::crCameraView to an CRCore::crTransform.");
        
        // Read crCameraView's properties
        setPosition(in->readVec3());
        setAttitude(in->readQuat());
        setFieldOfView(in->readDouble());
        setFieldOfViewMode((FieldOfViewMode)in->readInt());
        setFocalLength(in->readDouble());
    }
    else
    {
        throw crException("crCameraView::read(): Expected crCameraView identification.");
    }
}
