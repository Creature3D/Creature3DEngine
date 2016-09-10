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
#include <Plugin cre/crDepth.h>
#include <Plugin cre/crBase.h>

using namespace cre;

void crDepth::write(crDataOutputStream* out){
	out->writeInt(CREDEPTH);
	CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
	if(obj){
		((cre::crBase*)(obj))->write(out);
	}
	else
		throw crException("crDepth::write(): Could not cast this CRCore::crBlendFunc to an CRCore::crBase.");

	out->writeInt(getFunction());
	out->writeDouble(getZNear());
	out->writeDouble(getZFar());
	out->writeBool(getWriteMask());
}

void crDepth::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CREDEPTH){
		id = in->readInt();
		CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
		if(obj){
			((cre::crBase*)(obj))->read(in);
		}
		else
			throw crException("crDepth::read(): Could not cast this CRCore::crBlendFunc to an CRCore::crBase.");

		setFunction((CRCore::crDepth::Function)in->readInt());
		m_zNear = in->readDouble();
		m_zFar = in->readDouble();
		setWriteMask(in->readBool());

	}
	else{
		throw crException("crDepth::read(): Expected crBlendFunc identification.");
	}
}
