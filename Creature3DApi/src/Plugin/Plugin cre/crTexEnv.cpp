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
#include <Plugin cre/crTexEnv.h>
#include <Plugin cre/crBase.h>

using namespace cre;

void crTexEnv::write(crDataOutputStream* out){
	// Write crTexEnv's identification.
	out->writeInt(CRETEXENV);
	// If the CRCore class is inherited by any other class we should also write this to file.
	CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
	if(obj){
		((cre::crBase*)(obj))->write(out);
	}
	else
		throw crException("crTexEnv::write(): Could not cast this CRCore::crTexEnv to an CRCore::crBase.");
	// Write crTexEnv's properties.

	// Write mode
	out->writeInt(getMode());
	// Write color
	out->writeVec4(getColor());
}

void crTexEnv::read(crDataInputStream* in){
	// Peek on crTexEnv's identification.
	int id = in->peekInt();
	if(id == CRETEXENV){
		// Read crTexEnv's identification.
		id = in->readInt();
		// If the CRCore class is inherited by any other class we should also read this from file.
		CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
		if(obj){
			((cre::crBase*)(obj))->read(in);
		}
		else
			throw crException("crTexEnv::read(): Could not cast this CRCore::crTexEnv to an CRCore::crBase.");
		// Read crTexEnv's properties

		// Read mode
		setMode((CRCore::crTexEnv::Mode)in->readInt());
		// Read color
		setColor(in->readVec4());
	}
	else{
		throw crException("crTexEnv::read(): Expected crTexEnv identification.");
	}
}
