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
#include <Plugin cre/crTexture2D.h>
#include <Plugin cre/crTexture.h>
#include <Plugin cre/crImage.h>
#include <CRIOManager/crFileNameUtils.h>
#include <CRIOManager/crWriteFile.h>
#include <CRCore/crArgumentParser.h>
using namespace cre;

void crTexture2D::write(crDataOutputStream* out){
    // Write crTexture2D's identification.
    out->writeInt(CRETEXTURE2D);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crTexture*  tex = dynamic_cast<CRCore::crTexture*>(this);
    if(tex){
        ((cre::crTexture*)(tex))->write(out);
    }
    else
        throw crException("crTexture2D::write(): Could not cast this CRCore::crTexture2D to an CRCore::crTexture.");
    // Write crTexture2D's properties.
    // Write image.

    // Should we include images date in stream
    bool includeImg = out->getIncludeImageData();
	if(!includeImg&&getImage())
	{
		const std::string& fileName = getImage()->getFileName();
		if(!fileName.empty())
		{
			std::string ext = CRIOManager::getLowerCaseFileExtension(fileName);
			if(ext.compare("")==0)
                includeImg = true;
		}
	}
    out->writeBool(includeImg);

    // Include image data in stream
    if(includeImg){
        out->writeBool(getImage()!=0);
        if(getImage())
            ((cre::crImage*)getImage())->write(out);
    }

	const std::string &writeImagePath = out->getWriteImagePath();
	if(!writeImagePath.empty() && getImage())
	{
		CRIOManager::writeImageFile( *getImage(), writeImagePath + CRCore::crArgumentParser::getFileNameEliminateExt(getImage()->getFileName()) + ".img" );
	}
    // Only include image name in stream
    //else{
    //    if (getImage() && !(getImage()->getFileName().empty())){
    //        out->writeString(getImage()->getFileName());
    //    }
    //    else{ 
    //        out->writeString("");
    //    }    
    //}
}

void crTexture2D::read(crDataInputStream* in){
    // Read crTexture2D's identification.
    int id = in->peekInt();
    if(id == CRETEXTURE2D){
        // Code to read crTexture2D's properties.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crTexture*  tex = dynamic_cast<CRCore::crTexture*>(this);
        if(tex){
            ((cre::crTexture*)(tex))->read(in);
        }
        else
            throw crException("crTexture2D::read(): Could not cast this CRCore::crTexture2D to an CRCore::crTexture.");
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
 //       else
	//	{
 ////           std::string filename = in->readString();
 ////           if(filename.compare("")!=0)
	////		{
	////			setImageNameID(filename);
 /////*               CRCore::crImage* image = in->readImage(filename);
 ////               if (image){
 ////                   setImage(image);
 ////               }*/
 ////           }
 //       }
    }
    else{
        throw crException("crTexture2D::read(): Expected crTexture2D identification.");
    }
}
