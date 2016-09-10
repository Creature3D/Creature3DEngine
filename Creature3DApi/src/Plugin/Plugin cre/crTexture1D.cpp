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
#include <Plugin cre/crTexture1D.h>
#include <Plugin cre/crTexture.h>
#include <Plugin cre/crImage.h>

using namespace cre;

void crTexture1D::write(crDataOutputStream* out){
    // Write crTexture1D's identification.
    out->writeInt(CRETEXTURE1D);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crTexture*  tex = dynamic_cast<CRCore::crTexture*>(this);
    if(tex){
        ((cre::crTexture*)(tex))->write(out);
    }
    else
        throw crException("crTexture1D::write(): Could not cast this CRCore::crTexture1D to an CRCore::crTexture.");
    // Write crTexture1D's properties.
    // Write image.

    // Should we include images date in stream
    bool includeImg = out->getIncludeImageData();
    out->writeBool(includeImg);

    // Include image data in stream
    if(includeImg){
        out->writeBool(getImage()!=0);
        if(getImage())
            ((cre::crImage*)getImage())->write(out);
    }
    // Only include image name in stream
    else{
        if (getImage() && !(getImage()->getFileName().empty())){
            out->writeString(getImage()->getFileName());
        }
        else{ 
            out->writeString("");
        }    
    }
}

void crTexture1D::read(crDataInputStream* in){
    // Read crTexture1D's identification.
    int id = in->peekInt();
    if(id == CRETEXTURE1D){
        // Code to read crTexture1D's properties.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crTexture*  tex = dynamic_cast<CRCore::crTexture*>(this);
        if(tex){
            ((cre::crTexture*)(tex))->read(in);
        }
        else
            throw crException("crTexture1D::read(): Could not cast this CRCore::crTexture1D to an CRCore::crTexture.");
        // Read image.
        
        // Should we read image data from stream
        bool includeImg = in->readBool();

        // Read image data from stream
        if(includeImg)
        {
            if(in->readBool())
            {
                CRCore::crImage* image = new CRCore::crImage();
                ((cre::crImage*)image)->read(in);
                setImage(image);
            }
        }
        // Only read image name from stream.
        else{
            std::string filename = in->readString();
            if(filename.compare("")!=0){
                CRCore::crImage* image = in->readImage(filename);
                if (image){
                    setImage(image);
                }
            }
        }
    }
    else{
        throw crException("crTexture1D::read(): Expected crTexture1D identification.");
    }
}
