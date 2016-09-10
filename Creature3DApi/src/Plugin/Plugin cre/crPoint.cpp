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
#include <Plugin cre/crPoint.h>
#include <Plugin cre/crBase.h>

using namespace cre;

void crPoint::write(crDataOutputStream* out){
	// Write crCullFace's identification.
	out->writeInt(CREPOINT);
	// If the CRCore class is inherited by any other class we should also write this to file.
	CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
	if(obj){
		((cre::crBase*)(obj))->write(out);
	}
	else
		throw crException("crPoint::write(): Could not cast this CRCore::crPoint to an CRCore::crBase.");
	// Write crPoint's properties.
	out->writeFloat(getSize());
	out->writeFloat(getFadeThresholdSize());
	out->writeVec3(getDistanceAttenuation());
	out->writeFloat(getMinSize());
	out->writeFloat(getMaxSize());
}

void crPoint::read(crDataInputStream* in){
	// Peek on crPoint's identification.
	int id = in->peekInt();
	if(id == CREPOINT){
		// Read crPoint's identification.
		id = in->readInt();
		// If the CRCore class is inherited by any other class we should also read this from file.
		CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
		if(obj){
			((cre::crBase*)(obj))->read(in);
		}
		else
			throw crException("crPoint::read(): Could not cast this CRCore::crPoint to an CRCore::crBase.");
		// Read crPoint's properties
		setSize(in->readFloat());
		setFadeThresholdSize(in->readFloat());
		setDistanceAttenuation(in->readVec3());
		setMinSize(in->readFloat());
		setMaxSize(in->readFloat());
	}
	else{
		throw crException("crPoint::read(): Expected crPoint identification.");
	}
}
