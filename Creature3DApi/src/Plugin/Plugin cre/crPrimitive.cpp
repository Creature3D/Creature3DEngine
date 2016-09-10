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
#include <Plugin cre/crBase.h>
#include <Plugin cre/crPrimitive.h>

using namespace cre;

void crPrimitive::write(crDataOutputStream* out){
	// Write crPrimitive's identification.
	out->writeInt(CREPRIMITCRESET);
	// If the CRCore class is inherited by any other class we should also write this to file.
	CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
	if(obj){
		((cre::crBase*)(obj))->write(out);
	}
	else
		throw crException("crPrimitive::write(): Could not cast this CRCore::crPrimitive to an CRCore::crBase.");


	// Write crPrimitive's properties.
	out->writeInt(getMode());
}

void crPrimitive::read(crDataInputStream* in){
	// Read crPrimitive's identification.
	int id = in->peekInt();
	if(id == CREPRIMITCRESET){
		// Code to read crPrimitive's properties.
		id = in->readInt();
		// If the CRCore class is inherited by any other class we should also read this from file.
		CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
		if(obj){
			((cre::crBase*)(obj))->read(in);
		}
		else
			throw crException("crPrimitive::read(): Could not cast this CRCore::crPrimitive to an CRCore::crBase.");


		// Read in primitiveset properties.
		setMode(in->readInt());
	}
	else{
		throw crException("crPrimitive::read(): Expected crPrimitive identification.");
	}
}
