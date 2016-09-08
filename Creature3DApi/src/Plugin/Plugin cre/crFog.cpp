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
#include <Plugin cre/crFog.h>
#include <Plugin cre/crBase.h>

using namespace cre;

void crFog::write(crDataOutputStream* out){
	out->writeInt(CREFOG);
	CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
	if(obj){
		((cre::crBase*)(obj))->write(out);
	}
	else
		throw crException("crFog::write(): Could not cast this CRCore::crFog to an CRCore::crBase.");

    out->writeUInt(m_mode);
	out->writeFloat(m_density);
	out->writeFloat(m_start);
	out->writeFloat(m_end);
	out->writeVec4(m_color);
	out->writeInt(m_fogCoordinateSource);
}

void crFog::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CREFOG){
		id = in->readInt();
		CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
		if(obj){
			((cre::crBase*)(obj))->read(in);
		}
		else
			throw crException("crFog::read(): Could not cast this CRCore::crFog to an CRCore::crBase.");

		setMode((Mode)(in->readUInt()));
		setDensity(in->readFloat());
		setStart(in->readFloat());
		setEnd(in->readFloat());
		setColor(in->readVec4());
		setFogCoordinateSource(in->readInt());
	}
	else{
		throw crException("crFog::read(): Expected crFog identification.");
	}
}
