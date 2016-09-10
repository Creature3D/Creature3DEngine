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
#include <Plugin cre/crPolygonOffset.h>
#include <Plugin cre/crBase.h>

using namespace cre;

void crPolygonOffset::write(crDataOutputStream* out){
	// Write crCullFace's identification.
	out->writeInt(CREPOLYGONOFFSET);
	// If the CRCore class is inherited by any other class we should also write this to file.
	CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
	if(obj){
		((cre::crBase*)(obj))->write(out);
	}
	else
		throw crException("crPolygonOffset::write(): Could not cast this CRCore::crPolygonOffset to an CRCore::crBase.");
	// Write crPolygonOffset's properties.
	out->writeFloat(getFactor());
	out->writeFloat(getUnits());
}

void crPolygonOffset::read(crDataInputStream* in){
	// Peek on crPolygonOffset's identification.
	int id = in->peekInt();
	if(id == CREPOLYGONOFFSET){
		// Read crPolygonOffset's identification.
		id = in->readInt();
		// If the CRCore class is inherited by any other class we should also read this from file.
		CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
		if(obj){
			((cre::crBase*)(obj))->read(in);
		}
		else
			throw crException("crPolygonOffset::read(): Could not cast this CRCore::crPolygonOffset to an CRCore::crBase.");
		// Read crPolygonOffset's properties
		setFactor(in->readFloat());
		setUnits(in->readFloat());
	}
	else{
		throw crException("crPolygonOffset::read(): Expected crPolygonOffset identification.");
	}
}
