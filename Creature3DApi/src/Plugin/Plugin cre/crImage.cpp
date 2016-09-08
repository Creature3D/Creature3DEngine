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
#include <Plugin cre/crImage.h>
#include <Plugin cre/crBase.h>

#include <CRCore/crNotify.h>

using namespace cre;

void crImage::write(crDataOutputStream* out)
{
    // Write crImage's identification.
    out->writeInt(CREIMAGE);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
    if(obj){
        ((cre::crBase*)(obj))->write(out);
    }
    else
        throw crException("crImage::write(): Could not cast this CRCore::crImage to an CRCore::crBase.");
    // Write crImage's properties.

    // Write name
	if ( out->getVersion() < VERSION_0040 )
		out->writeString(getFileName());
 
    // Write width, height, depth of image.
    out->writeInt(s());
    out->writeInt(t());
    out->writeInt(r());
    
    
	//CRCore::notify(CRCore::DEBUG_INFO) << "image written '" << getFileName()<<"'\t"<<s()<<"\t"<<t()<<std::endl;

    // Write formats, type and packing
    out->writeInt(getInternalTextureFormat());
    out->writeInt(getPixelFormat());
    out->writeInt(getDataType());
    out->writeInt(getPacking());

    // Write modified tag.
    out->writeInt(getModifiedCount()); 

    // Write mipmapdata vector
    int size = m_mipmapData.size();
    out->writeInt(size);
    for(int i=0;i<size;i++)
        out->writeInt(m_mipmapData[i]);

    // Write image data if any
    out->writeBool(data()!=0);
    if(data())
    {
        // Compute the size of image data and write this.
        unsigned int size = getTotalSizeInBytesIncludingMipmaps();
        out->writeInt(size);
        // Write the data
        out->writeCharArray((char*)data(), size);
    }

}

void crImage::read(crDataInputStream* in)
{
    // Peek crImage's identification.
    int id = in->peekInt();
    if(id == CREIMAGE){
        // Read crImage's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
        if(obj){
            ((cre::crBase*)(obj))->read(in);
        }
        else
            throw crException("crImage::read(): Could not cast this CRCore::crImage to an CRCore::crBase.");
        // Read crImage's properties.

        // Read name
		if ( in->getVersion() < VERSION_0040 )
			setFileName(in->readString());

        // Read width, height, depth of image.
        int is=in->readInt();
        int it=in->readInt();
        int ir=in->readInt();

        // Read formats, type and packing
        GLint internalTextureFormat = (GLint) in->readInt();
        GLenum pixelFormat = (GLenum) in->readInt();
        GLenum dataType = (GLenum) in->readInt();;
        unsigned int packing = (unsigned int)in->readInt();

        // Read modified tag.
        setModifiedCount((unsigned int)in->readInt());

        // Read mipmapdata vector        
        int size = in->readInt();
        MipmapDataType mipmapData(size);

        //if (size) std::cout<<"is mip mapped ";

        for(int i=0;i<size;i++)
        {
            mipmapData[i]=(unsigned int)in->readInt();
            //std::cout<<_mipmapData[i]<<"\t";
        }
        //std::cout<<std::endl;
            

        // Read image data if any
        if(in->readBool())
        {
            unsigned int dataSize = (unsigned int)in->readInt();
            
            //static int totalSize = 0;
            
            //totalSize += dataSize;
            //std::cout<<getFileName()<<" "<<is<<" "<<it<<std::endl;
            //std::cout<<" dataSize = "<<dataSize<<"\tTotalSize = "<<totalSize<<std::endl;
            
            
            //char* data = (char*)malloc (dataSize);
            char* data =  new char[dataSize];
            if(!data)
                throw crException("crImage::read(): Unable to allocate memory for image data.");
            in->readCharArray(data,dataSize);
            setImage(is, it, ir, internalTextureFormat, pixelFormat,
                dataType, (unsigned char* ) data, CRCore::crImage::USE_NEW_DELETE, packing);
        }

        m_mipmapData.swap(mipmapData);
    }
    else{
        throw crException("crImage::read(): Expected crImage identification.");
    }
}
