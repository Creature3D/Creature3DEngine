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
#include <Plugin cre/crTexture.h>
#include <Plugin cre/crBase.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crNotify.h>
using namespace cre;

void crTexture::write(crDataOutputStream* out){
    // Write crTexture's identification.
    out->writeInt(CRETEXTURE);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
    if(obj)
    {
        ((cre::crBase*)(obj))->write(out);
    }
    else
        throw crException("crTexture::write(): Could not cast this CRCore::crTexture to an CRCore::crBase.");

    // Write crTexture's properties.
    out->writeInt(m_wrap_s);
    out->writeInt(m_wrap_t);
    out->writeInt(m_wrap_r);
	//CRCore::notify(CRCore::NOTICE)<<"ws = "<<m_wrap_s<<" wt = "<<m_wrap_t<<" wr = "<<m_wrap_r<<std::endl;

    out->writeInt(m_min_filter);
    out->writeInt(m_mag_filter);
    out->writeFloat(m_maxAnisotropy);

    out->writeVec4(m_borderColor);

    out->writeInt(m_internalFormatMode);
    
    if ( out->getVersion() >= VERSION_0008 )
    {
        out->writeInt(m_internalFormat);
    }

    if ( out->getVersion() >= VERSION_0009 )
    {
        out->writeInt(m_borderWidth);
        out->writeBool(m_useHardwareMipMapGeneration);
        out->writeBool(m_unrefImageDataAfterApply);
        out->writeBool(m_clientStorageHint);
        out->writeBool(m_resizeNonPowerOfTwoHint);
    }

	const std::string &writeImagePath = out->getWriteImagePath();
	if(!writeImagePath.empty())
	    out->writeString(CRCore::crArgumentParser::getFileNameEliminateExt(m_imageNameID) + ".img");
	else
		out->writeString(m_imageNameID);

	//if(dynamic_cast<CRCore::crTextureCubeMap *>(this))
	//	out->addCubeImageFiles(m_imageNameID);
	//else
	//	out->addImageFiles(m_imageNameID);

	if(m_imageNameID.find("_as")!=-1)
		m_alphaShadowEnable = true;
	//if(m_imageNameID.find("specular4")!=-1)
	//	m_alphaShininessEnable = true;
	if(m_imageNameID.find("parallax")!=-1)
		m_useParallax = true;

	out->writeBool(m_blendShadowEnable);
	out->writeBool(m_alphaShadowEnable);
	out->writeBool(m_useParallax);

	if ( out->getVersion() >= VERSION_0014 )
	{
		out->writeUShort((unsigned short)m_externTextureID);
	}
	//CRCore::notify(CRCore::NOTICE)<<"m_imageNameID = "<<m_imageNameID<<std::endl;
}

void crTexture::read(crDataInputStream* in)
{
    // Read crTexture's identification.
    int id = in->peekInt();
    if(id == CRETEXTURE)
    {
        // Read crTexture's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
        if(obj)
        {
            ((cre::crBase*)(obj))->read(in);
        }
        else
            throw crException("crTexture::read(): Could not cast this CRCore::crTexture to an CRCore::crBase.");

        // Read properties
        m_wrap_s = (CRCore::crTexture::WrapMode)in->readInt();
        m_wrap_t = (CRCore::crTexture::WrapMode)in->readInt();
        m_wrap_r = (CRCore::crTexture::WrapMode)in->readInt();
		//m_wrap_s = m_wrap_s==REPEAT?CLAMP:m_wrap_s;
		//m_wrap_t = m_wrap_t==REPEAT?CLAMP:m_wrap_t;
		//m_wrap_r = m_wrap_r==REPEAT?CLAMP:m_wrap_r;
		//CRCore::notify(CRCore::NOTICE)<<"ws = "<<ws<<" m_wrap_s = "<<m_wrap_s<<std::endl;

        m_min_filter = (CRCore::crTexture::FilterMode)in->readInt();
		if(m_min_filter == CRCore::crTexture::LINEAR)
			m_min_filter = CRCore::crTexture::LINEAR_MIPMAP_LINEAR;
		//    CRCore::notify(CRCore::ALWAYS)<<"CRCore::crTexture::LINEAR_MIPMAP_LINEAR"<<std::endl;
		//else CRCore::notify(CRCore::ALWAYS)<<m_min_filter<<std::endl;

        m_mag_filter = (CRCore::crTexture::FilterMode)in->readInt();
		if(m_mag_filter != CRCore::crTexture::LINEAR && m_mag_filter != CRCore::crTexture::NEAREST)
			m_mag_filter = CRCore::crTexture::LINEAR;
        m_maxAnisotropy = in->readFloat();
        //CRCore::notify(CRCore::ALWAYS)<<m_maxAnisotropy<<std::endl;

        m_borderColor = in->readVec4();

        m_internalFormatMode = (CRCore::crTexture::InternalFormatMode)in->readInt();
        if ( in->getVersion() >= VERSION_0008 )
        {
            m_internalFormat = in->readInt();
        }

        if ( in->getVersion() >= VERSION_0009 )
        {
            m_borderWidth = in->readInt();
            m_useHardwareMipMapGeneration = in->readBool();
            m_unrefImageDataAfterApply = in->readBool();
            m_clientStorageHint = in->readBool();
            m_resizeNonPowerOfTwoHint = in->readBool();
        }

		m_imageNameID = in->readString();

		m_blendShadowEnable = in->readBool();
        m_alphaShadowEnable = in->readBool();
		m_useParallax = in->readBool();

		if ( in->getVersion() >= VERSION_0014 )
		{
			m_externTextureID = (crTexture::ExternTextureID)(in->readUShort());
		}
		//CRCore::notify(CRCore::NOTICE)<<"m_imageNameID = "<<m_imageNameID<<std::endl;

		//setUnRefImageDataAfterApply(true);
    }
    else
    {
        throw crException("crTexture::read(): Expected crTexture identification.");
    }
}
