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
#include <Plugin cre/crPositionAttitudeTransform.h>
#include <Plugin cre/crTransform.h>

using namespace cre;

void crPositionAttitudeTransform::write(crDataOutputStream* out){
	// Write crPositionAttitudeTransform's identification.
	out->writeInt(CREPOSITIONATTITUDETRANSFORM);
	// If the CRCore class is inherited by any other class we should also write this to file.
	CRCore::crTransform*  trans = dynamic_cast<CRCore::crTransform*>(this);
	if(trans){
		((cre::crTransform*)(trans))->write(out);
	}
	else
		throw crException("crPositionAttitudeTransform::write(): Could not cast this CRCore::crPositionAttitudeTransform to an CRCore::crTransform.");
	// Write crPositionAttitudeTransform's properties.
      
	out->writeVec3(getPosition());
	out->writeQuat(getAttitude());
	out->writeVec3(getScale());
	out->writeVec3(getPivotPoint());
}

void crPositionAttitudeTransform::read(crDataInputStream* in){
	// Peek on crPositionAttitudeTransform's identification.
	int id = in->peekInt();
	if(id == CREPOSITIONATTITUDETRANSFORM){
		// Read crPositionAttitudeTransform's identification.
		id = in->readInt();
		// If the CRCore class is inherited by any other class we should also read this from file.
		CRCore::crTransform*  trans = dynamic_cast<CRCore::crTransform*>(this);
		if(trans){
			((cre::crTransform*)(trans))->read(in);
		}
		else
			throw crException("crPositionAttitudeTransform::read(): Could not cast this CRCore::crPositionAttitudeTransform to an CRCore::crTransform.");
		// Read crPositionAttitudeTransform's properties
		setPosition(in->readVec3());
		setAttitude(in->readQuat());
		setScale(in->readVec3());
		setPivotPoint(in->readVec3());

	}
	else{
		throw crException("crPositionAttitudeTransform::read(): Expected crPositionAttitudeTransform identification.");
	}
}
