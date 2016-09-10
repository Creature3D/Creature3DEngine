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
#include <Plugin cre/crLight.h>
#include <Plugin cre/crBase.h>

using namespace cre;

void crLight::write(crDataOutputStream* out){
	// Write crLight's identification.
	out->writeInt(CRELIGHT);
	// If the CRCore class is inherited by any other class we should also write this to file.
	CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
	if(obj){
		((cre::crBase*)(obj))->write(out);
	}
	else
		throw crException("crLight::write(): Could not cast this CRCore::crLight to an CRCore::crBase.");
	// Write crLight's properties.
	out->writeInt(getLightNum());
	out->writeUInt((unsigned int)getLightType());
	out->writeVec4(getAmbient());
	out->writeVec4(getDiffuse());
	out->writeVec4(getSpecular());
	out->writeVec4(getPosition());
	out->writeVec3(getDirection());
	out->writeFloat(getConstantAttenuation());
	out->writeFloat(getLinearAttenuation ());
	out->writeFloat(getQuadraticAttenuation());
	out->writeFloat(getSpotExponent());
	out->writeFloat(getSpotCutoff());

	out->writeFloat(getLoweastLum());
 //   out->writeString(m_lightMapFileName);
	//out->writeBool(m_lightMap.valid());
	//if(m_lightMap.valid())
	//    out->writeStateAttribute(m_lightMap.get());
}

void crLight::read(crDataInputStream* in){
	// Peek on crLight's identification.
	int id = in->peekInt();
	if(id == CRELIGHT){
		// Read crLight's identification.
		id = in->readInt();
		// If the CRCore class is inherited by any other class we should also read this from file.
		CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
		if(obj){
			((cre::crBase*)(obj))->read(in);
		}
		else
			throw crException("crLight::read(): Could not cast this CRCore::crLight to an CRCore::crBase.");
		// Read crLight's properties
		setLightNum(in->readInt());
		setLightType((LightType)(in->readUInt()));
		setAmbient(in->readVec4());
		setDiffuse(in->readVec4());
		setSpecular(in->readVec4());
		setPosition(in->readVec4());
		setDirection(in->readVec3());
		setConstantAttenuation(in->readFloat());
		setLinearAttenuation (in->readFloat());
		setQuadraticAttenuation(in->readFloat());
		setSpotExponent(in->readFloat());
		setSpotCutoff(in->readFloat());
        
		setLoweastLum(in->readFloat());

		//m_lightMapFileName = in->readString();
		//if(in->readBool())
		//    m_lightMap = dynamic_cast<CRCore::crTexture2D *>(in->readStateAttribute());
	}
	else{
		throw crException("crLight::read(): Expected crLight identification.");
	}
}
