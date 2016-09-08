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
#include <Plugin cre/crTransform.h>
#include <Plugin cre/crGroup.h>

using namespace cre;

void crTransform::write(crDataOutputStream* out){
	// Write crTransform's identification.
	out->writeInt(CRETRANSFORM);
	// If the CRCore class is inherited by any other class we should also write this to file.
	CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
	if(group){
		((cre::crGroup*)(group))->write(out);
	}
	else
		throw crException("crTransform::write(): Could not cast this CRCore::crTransform to an CRCore::crGroup.");
	// Write crTransform's properties.
	out->writeInt(getReferenceFrame());
}

void crTransform::read(crDataInputStream* in){
	// Peek on crTransform's identification.
	int id = in->peekInt();
	if(id == CRETRANSFORM){
		// Read crTransform's identification.
		id = in->readInt();
		// If the CRCore class is inherited by any other class we should also read this from file.
		CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
		if(group){
			((cre::crGroup*)(group))->read(in);
		}
		else
			throw crException("crTransform::read(): Could not cast this CRCore::crTransform to an CRCore::crGroup.");
		// Read crTransform's properties
		setReferenceFrame((CRCore::crTransform::ReferenceFrame)in->readInt());
	}
	else{
		throw crException("crTransform::read(): Expected crTransform identification.");
	}
}
