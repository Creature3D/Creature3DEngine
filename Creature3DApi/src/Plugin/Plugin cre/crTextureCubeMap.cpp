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
#include <Plugin cre/crTextureCubeMap.h>
#include <Plugin cre/crTexture.h>
#include <Plugin cre/crImage.h>
#include <CRCore/crArgumentParser.h>
#include <CRIOManager/crWriteFile.h>

using namespace cre;

void crTextureCubeMap::write(crDataOutputStream* out){
    // Write crTextureCubeMap's identification.
    out->writeInt(CRETEXTURECUBEMAP);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crTexture*  tex = dynamic_cast<CRCore::crTexture*>(this);
    if(tex){
        ((cre::crTexture*)(tex))->write(out);
    }
    else
        throw crException("crTextureCubeMap::write(): Could not cast this CRCore::crTextureCubeMap to an CRCore::crTexture.");
    // Write crTextureCubeMap's properties.

    // Write texture size
    //int width,height;
    //getTextureSize(width, height);
    out->writeInt(m_textureWidth);
    out->writeInt(m_textureHeight);

    // Write number of mipmap levels
    out->writeInt(getNumMipmapLevels());

    // Should we include images date in stream
    bool includeImg = out->getIncludeImageData();
    out->writeBool(includeImg);

    writeImage(out,includeImg,getImage(CRCore::crTextureCubeMap::POSITIVE_X));
    writeImage(out,includeImg,getImage(CRCore::crTextureCubeMap::NEGATIVE_X));
    writeImage(out,includeImg,getImage(CRCore::crTextureCubeMap::POSITIVE_Y));
    writeImage(out,includeImg,getImage(CRCore::crTextureCubeMap::NEGATIVE_Y));
    writeImage(out,includeImg,getImage(CRCore::crTextureCubeMap::POSITIVE_Z));
    writeImage(out,includeImg,getImage(CRCore::crTextureCubeMap::NEGATIVE_Z));

	//for(int i = 0; i<6; ++i)
	//{
	//	if(!m_imageNameID[i].empty())
	//		out->writeString(m_imageNameID[i]);
	//	else
	//		out->writeString("");

	//}
}


void crTextureCubeMap::writeImage(crDataOutputStream* out,bool includeImg,CRCore::crImage* image)
{
    if(includeImg)
    {
        // Write images if any
        out->writeBool(image!=0);
        if(image)
            ((cre::crImage*)(image))->write(out);
    }
    else
    {
        //if (image && !(image->getFileName().empty())){
        //    out->writeString(image->getFileName());
        //}
        //else{ 
        //    out->writeString("");
        //}    
    }

	const std::string &writeImagePath = out->getWriteImagePath();
	if(!writeImagePath.empty() && image)
	{
		CRIOManager::writeImageFile( *image, writeImagePath + CRCore::crArgumentParser::getFileNameEliminateExt(image->getFileName()) + ".img" );
	}
}

void crTextureCubeMap::read(crDataInputStream* in)
{
    // Peek on crTextureCubeMap's identification.
    int id = in->peekInt();
    if(id == CRETEXTURECUBEMAP){
        // Read crTextureCubeMap's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crTexture* tex = dynamic_cast<CRCore::crTexture*>(this);
        if(tex){
            ((cre::crTexture*)(tex))->read(in);
        }
        else
            throw crException("crTextureCubeMap::read(): Could not cast this CRCore::crTextureCubeMap to an CRCore::crTexture.");
        // Read crTextureCubeMap's properties

        // Read texture size
        int width = in->readInt();
        int height = in->readInt();
        setTextureSize(width, height);

        // Read number of mipmap levels
        setNumMipmapLevels((unsigned int)in->readInt());

        // Should we read image data from stream
        bool includeImg = in->readBool();

        setImage(CRCore::crTextureCubeMap::POSITIVE_X,readImage(in, includeImg));
        setImage(CRCore::crTextureCubeMap::NEGATIVE_X,readImage(in, includeImg));
        setImage(CRCore::crTextureCubeMap::POSITIVE_Y,readImage(in, includeImg));
        setImage(CRCore::crTextureCubeMap::NEGATIVE_Y,readImage(in, includeImg));
        setImage(CRCore::crTextureCubeMap::POSITIVE_Z,readImage(in, includeImg));
        setImage(CRCore::crTextureCubeMap::NEGATIVE_Z,readImage(in, includeImg));
    }
    else{
        throw crException("crTextureCubeMap::read(): Expected crTextureCubeMap identification.");
    }
}

CRCore::crImage* crTextureCubeMap::readImage(crDataInputStream* in, bool includeImg)
{        
    if(includeImg)
    {
        // Read image data from stream
        if(in->readBool())
        {
            CRCore::crImage* image = new CRCore::crImage();
            ((cre::crImage*)image)->read(in);
            return image;
        }
    }
    else
    {
        // Only read image name from stream.
        //std::string filename = in->readString();
        //if(filename.compare("")!=0)
        //{
        //    CRCore::crImage* image = in->readImage(filename);
        //    return image;
        //}
    }
    return 0;
}
