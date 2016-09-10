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
#include <Plugin cre/crLod.h>
#include <Plugin cre/crGroup.h>

using namespace cre;

void crLod::write(crDataOutputStream* out){
	// Write crLod's identification.
	out->writeInt(CRELOD);
	// If the CRCore class is inherited by any other class we should also write this to file.
	CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
	if(group){
		((cre::crGroup*)(group))->write(out);
	}
	else
		throw crException("crLod::write(): Could not cast this CRCore::crLod to an CRCore::crGroup.");
	// Write crLod's properties.
    out->writeFloat(getRadius());
	// Write centermode
	out->writeVec3(getCenter());
	out->writeInt(getCenterMode());

    out->writeInt(getRangeMode());

    // Write rangelist
	int size = getNumRanges();
	out->writeInt(size);
	for(int i=0;i<size;i++){
		out->writeFloat(getMinRange(i));
		out->writeFloat(getMaxRange(i));
	}
}

void crLod::read(crDataInputStream* in){
	// Peek on crLod's identification.
	int id = in->peekInt();
	if(id == CRELOD){
		// Read crLod's identification.
		id = in->readInt();

		// If the CRCore class is inherited by any other class we should also read this from file.
		CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
		if(group){
			((cre::crGroup*)(group))->read(in);
		}
		else
			throw crException("crLod::read(): Could not cast this CRCore::crLod to an CRCore::crGroup.");
		// Read crLod's properties
        setRadius(in->readFloat());
		setCenter(in->readVec3());
		// Read centermode
		setCenterMode((CRCore::crLod::CenterMode)in->readInt());
        setRangeMode((RangeMode)in->readInt());
		// Read rangelist
		int size = in->readInt();
		for(int i=0;i<size;i++){
			float min = in->readFloat();
			float max = in->readFloat();
			setRange(i, min, max);
		}
	}
	else{
		throw crException("crLod::read(): Expected crLod identification.");
	}
}
