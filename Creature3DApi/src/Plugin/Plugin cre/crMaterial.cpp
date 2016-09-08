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
#include <Plugin cre/crMaterial.h>
#include <Plugin cre/crBase.h>

using namespace cre;

void crMaterial::write(crDataOutputStream* out){
	// Write crMaterial's identification.
	out->writeInt(CREMATERIAL);
	// If the CRCore class is inherited by any other class we should also write this to file.
	CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
	if(obj){
		((cre::crBase*)(obj))->write(out);
	}
	else
		throw crException("crMaterial::write(): Could not cast this CRCore::crMaterial to an CRCore::crBase.");
	// Write crMaterial's properties.

	//	Write color mode
    out->writeInt(m_colorMode);
	// Write ambient
	out->writeBool(m_ambientFrontAndBack);
	out->writeVec4(m_ambientFront);
    out->writeVec4(m_ambientBack);
	// Write diffuse	
    out->writeBool(m_diffuseFrontAndBack);
    out->writeVec4(m_diffuseFront);
    out->writeVec4(m_diffuseBack);
	// Write specular
    out->writeBool(m_specularFrontAndBack);
    out->writeVec4(m_specularFront);
    out->writeVec4(m_specularBack);
	// Write emmision
    out->writeBool(m_emissionFrontAndBack);
    out->writeVec4(m_emissionFront);
    out->writeVec4(m_emissionBack);
	// Write shininess
    out->writeBool(m_shininessFrontAndBack);
	out->writeFloat(m_shininessFront);
	out->writeFloat(m_shininessBack);

}

void crMaterial::read(crDataInputStream* in){
	// Read crMaterial's identification.
	int id = in->peekInt();
	if(id == CREMATERIAL){
		// Code to read crMaterial's properties.
		id = in->readInt();
		// If the CRCore class is inherited by any other class we should also read this from file.
		CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
		if(obj){
			((cre::crBase*)(obj))->read(in);
		}
		else
			throw crException("crMaterial::read(): Could not cast this CRCore::crMaterial to an CRCore::crBase.");

		// Read color mode.
		m_colorMode = (CRCore::crMaterial::ColorMode)in->readInt();
		// Read ambient
		m_ambientFrontAndBack = in->readBool();
        m_ambientFront = in->readVec4();
        m_ambientBack = in->readVec4();
		// Read diffuse
        m_diffuseFrontAndBack = in->readBool();
        m_diffuseFront = in->readVec4();
        m_diffuseBack = in->readVec4();
		// Read specular
        m_specularFrontAndBack = in->readBool();
        m_specularFront = in->readVec4();
        m_specularBack = in->readVec4();
		// Read emission
        m_emissionFrontAndBack = in->readBool();
        m_emissionFront = in->readVec4();
        m_emissionBack = in->readVec4();
		// Read shiniess
        m_shininessFrontAndBack = in->readBool();
        m_shininessFront = in->readFloat();
        m_shininessBack = in->readFloat();
	}
	else{
		throw crException("crMaterial::read(): Expected crMaterial identification.");
	}
}
