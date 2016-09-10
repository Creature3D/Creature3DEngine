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
#include <Plugin cre/crPolygonMode.h>
#include <Plugin cre/crBase.h>

using namespace cre;

void crPolygonMode::write(crDataOutputStream* out){
	// Write crPolygonMode's identification.
	out->writeInt(CREPOLYGONMODE);
	// If the CRCore class is inherited by any other class we should also write this to file.
	CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
	if(obj){
		((cre::crBase*)(obj))->write(out);
	}
	else
		throw crException("crPolygonMode::write(): Could not cast this CRCore::crPolygonMode to an CRCore::crBase.");
	// Write crPolygonMode's properties.
	out->writeBool(getFrontAndBack());
	if(getFrontAndBack())
	{
		out->writeInt(getMode(FRONT_AND_BACK));
	}
	else
	{
		out->writeInt(getMode(FRONT));
		out->writeInt(getMode(BACK));
	}
}

void crPolygonMode::read(crDataInputStream* in){
	// Peek on crPolygonMode's identification.
	int id = in->peekInt();
	if(id == CREPOLYGONMODE){
		// Read crPolygonMode's identification.
		id = in->readInt();
		// If the CRCore class is inherited by any other class we should also read this from file.
		CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
		if(obj){
			((cre::crBase*)(obj))->read(in);
		}
		else
			throw crException("crPolygonMode::read(): Could not cast this CRCore::crPolygonMode to an CRCore::crBase.");
		// Read crPolygonMode's properties
		if(in->readBool())
		{
			setMode(FRONT_AND_BACK,(Mode)(in->readInt()));
		}
		else
		{
			setMode(FRONT,(Mode)(in->readInt()));
			setMode(BACK,(Mode)(in->readInt()));
		}

	}
	else{
		throw crException("crPolygonMode::read(): Expected crPolygonMode identification.");
	}
}
