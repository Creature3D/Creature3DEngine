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
#include <Plugin cre/crStateSet.h>
#include <Plugin cre/crBase.h>

#include <CRCore/crStateAttribute.h>

using namespace cre;

void crStateSet::write(crDataOutputStream* out){
    // Write crStateSet's identification.
    out->writeInt(CRESTATESET);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
    if(obj){
         ((cre::crBase*)(obj))->write(out);
    }
    else
        throw crException("crStateSet::write(): Could not cast this CRCore::crStateSet to an CRCore::crBase.");

    // Write render bin bin details.
    switch(getRenderBinMode()){
        case CRCore::crStateSet::INHERIT_RENDERBIN_DETAILS:        out->writeChar((char)0);break;
        case CRCore::crStateSet::USE_RENDERBIN_DETAILS:            out->writeChar((char)1);break;
        case CRCore::crStateSet::OVERRIDE_RENDERBIN_DETAILS:       out->writeChar((char)2);break;
        case CRCore::crStateSet::ENCLOSE_RENDERBIN_DETAILS:        out->writeChar((char)3);break;
        default: throw crException("Unknown RenderBinMode in crStateSet::write()");
    }
    out->writeInt(getBinNumber());
    out->writeString(getBinName());
    
    const crStateSet::ModeList &ml = getModeList();
    const crStateSet::AttributeList &al = getAttributeList();

    // Write stateset modes, this could for instance be GL_NORMALIZE, GL_LIGHT, etc.
    out->writeInt(ml.size());
    for(crStateSet::ModeList::const_iterator mitr=ml.begin(); mitr!=ml.end(); ++mitr)
	{
        out->writeUInt(mitr->first);
        out->writeUInt((unsigned int)mitr->second);
    }

    // Write stateset attributes, this could for instance be alphafunctions, materials, etc.
    out->writeInt(al.size());
    for(crStateSet::AttributeList::const_iterator aitr=al.begin(); aitr!=al.end(); ++aitr)
	{
        out->writeStateAttribute(aitr->second.first.get());
        out->writeInt(aitr->second.second);
    }

    const crStateSet::TextureModeList &tml = getTextureModeList();
    const crStateSet::TextureAttributeList &tal = getTextureAttributeList();
    // Write texture stateset modes, this could for instance be TEXGEN ON, TEXTURECUBEMAP ON, etc.
    int nUnits = tml.size();
    out->writeInt(nUnits);
    int unit;
    for(unit=0;unit<nUnits;unit++)
	{
        const crStateSet::ModeList &ml_t = tml[unit];
        out->writeInt(ml_t.size());
        for(crStateSet::ModeList::const_iterator mitr=ml_t.begin(); mitr!=ml_t.end(); ++mitr)
		{
			out->writeUInt(mitr->first);
            out->writeUInt((unsigned int)mitr->second);
        }
    }

    // Write texture attributes, this could for instance texture2D, texturecubemap
    nUnits = tal.size();
    out->writeInt(nUnits);
    for(unit=0;unit<nUnits;unit++)
	{
        const crStateSet::AttributeList &al_t = tal[unit];
        out->writeInt(al_t.size());
        for(crStateSet::AttributeList::const_iterator aitr=al_t.begin(); aitr!=al_t.end(); ++aitr)
		{
            out->writeStateAttribute(aitr->second.first.get());
            out->writeInt(aitr->second.second);
        }
    }

    // Write stateset uniforms
    const crStateSet::UniformList &ul = getUniformList();
    out->writeInt(ul.size());
    for(crStateSet::UniformList::const_iterator uitr=ul.begin(); uitr!=ul.end(); ++uitr)
    {
        out->writeUniform(uitr->second.first.get());
        out->writeInt(uitr->second.second);
    }

	out->writeUChar(m_statesetMode);

	if(out->getVersion() < VERSION_0039)
	{
		for(int i = 0; i<16; ++i)
		{
			out->writeInt(m_textureOperate[i]);
			out->writeInt(m_alphaOperate[i]);
		}
	}
	else
	{
		for(int i = 0; i<16; ++i)
		{
			out->writeUChar(m_textureOperate[i]);
			out->writeUChar(m_alphaOperate[i]);
			out->writeUChar(m_textureUV[i]);
		}
	}
}

void crStateSet::read(crDataInputStream* in){
    // Read crStateSet's identification.
    int id = in->peekInt();
    if(id == CRESTATESET){
        // Code to read crStateSet's properties.
        id = in->readInt();
    
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
        if(obj){
            ((cre::crBase*)(obj))->read(in);;
        }
        else
            throw crException("crStateSet::read(): Could not cast this CRCore::crStateSet to an CRCore::crBase.");        

        // Read render bin details.
        char c = in->readChar();
        int num = in->readInt();
		if ( num == 9 && in->getVersion() < VERSION_0031 )
		{
			num = CRCore::crStateSet::AlphaTest;
		}
        std::string name = in->readString();
        switch((int)c){
            case 0:        
                setRenderBinDetails(num, name, CRCore::crStateSet::INHERIT_RENDERBIN_DETAILS); 
                break;
            case 1:        
                setRenderBinDetails(num, name, CRCore::crStateSet::USE_RENDERBIN_DETAILS); 
                break;
            case 2:        
                setRenderBinDetails(num, name, CRCore::crStateSet::OVERRIDE_RENDERBIN_DETAILS); 
                break;
            case 3:        
                setRenderBinDetails(num, name, CRCore::crStateSet::ENCLOSE_RENDERBIN_DETAILS); 
                break;
            default: throw crException("Unknown RenderBinMode in crStateSet::read()");
        }

        // Read stateset modes.
        int size = in->readInt();
        int i;
		unsigned int mode;
        for(i=0;i<size;i++)
		{
			mode = in->readUInt();
            setMode(mode, (CRCore::crStateAttribute::VRModeValue)(in->readUInt()));
        }

        // Read state attributes 
        size = in->readInt();
        for(i=0;i<size;i++)
		{
            CRCore::crStateAttribute* attribute = in->readStateAttribute();
            setAttribute(attribute, (CRCore::crStateAttribute::OverrideValue)(in->readInt()));
        }
        
        // Read texture stateset mode.
        int nUnits = in->readInt();
        int unit;
		int oldunit;
        for(unit=0;unit<nUnits;unit++)
		{
            size = in->readInt();
			if ( in->getVersion() < VERSION_0025 )
			{
				oldunit = unit;
				if(unit == TEXTURE_LIGHTMAP)
				{
					oldunit = TEXTURE_BUMPMAP;
				}
				else if(unit == TEXTURE_BUMPMAP)
				{
					oldunit = TEXTURE_ENVMAP;
				}
				for(i=0;i<size;i++)
				{
					mode = in->readUInt();
					setTextureMode(oldunit, mode, (CRCore::crStateAttribute::VRModeValue)(in->readUInt()));
				}
			}
			else
			{
				for(i=0;i<size;i++)
				{
					mode = in->readUInt();
					setTextureMode(unit, mode, (CRCore::crStateAttribute::VRModeValue)(in->readUInt()));
				}
			}
        }

        // Read texture attributes.
        nUnits = in->readInt();
		CRCore::crStateAttribute* attribute = NULL;
        for(unit=0;unit<nUnits;unit++)
        {
            size = in->readInt();
			if ( in->getVersion() < VERSION_0025 )
			{
				oldunit = unit;
				if(unit == TEXTURE_LIGHTMAP)
				{
					oldunit = TEXTURE_BUMPMAP;
				}
				else if(unit == TEXTURE_BUMPMAP)
				{
					oldunit = TEXTURE_ENVMAP;
				}
				for(i=0;i<size;i++)
				{
					attribute = in->readStateAttribute();
					setTextureAttribute(oldunit, attribute,(CRCore::crStateAttribute::OverrideValue)(in->readInt()));
				}
			}
			else
			{
				for(i=0;i<size;i++)
				{
					attribute = in->readStateAttribute();
					setTextureAttribute(unit, attribute,(CRCore::crStateAttribute::OverrideValue)(in->readInt()));
				}
			}
        }

        // Read uniforms 
        size = in->readInt();
        for(i=0;i<size;i++)
        {
            CRCore::crUniform* uniform = in->readUniform();
            addUniform(uniform, (CRCore::crStateAttribute::OverrideValue)in->readInt());
        }

		setStateSetMode(in->readUChar());
		if(in->getVersion() < VERSION_0039)
		{
			for(int i = 0; i<16; ++i)
			{
				m_textureOperate[i] = in->readInt();
				m_alphaOperate[i] = in->readInt();
			}
		}
		else
		{
			for(int i = 0; i<16; ++i)
			{
				m_textureOperate[i] = in->readUChar();
				m_alphaOperate[i] = in->readUChar();
				m_textureUV[i] = in->readUChar();
			}
		}
    }
    else{
        throw crException("crStateSet::read(): Expected crStateSet identification");
    }
}
