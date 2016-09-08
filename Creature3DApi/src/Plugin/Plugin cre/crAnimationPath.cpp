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
#include <Plugin cre/crAnimationPath.h>
#include <Plugin cre/crBase.h>

using namespace cre;

void crAnimationPath::write(crDataOutputStream* out){
	out->writeInt(CREANIMATIONPATH);
	CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
	if(obj){
		((cre::crBase*)(obj))->write(out);
	}
	else
		throw crException("crAnimationPath::write(): Could not cast this CRCore::crAnimationPath to an CRCore::crBase.");

	out->writeInt(getLoopMode());
	crAnimationPath::TimeControlPointMap tcpm = getTimeControlPointMap();
	out->writeInt(tcpm.size());
    for(crAnimationPath::TimeControlPointMap::iterator itr=tcpm.begin(); itr!=tcpm.end(); ++itr){
		out->writeFloat(itr->first);
		out->writeVec3(itr->second.getPosition());
		out->writeQuat(itr->second.getRotation());
		out->writeVec3(itr->second.getScale());
    }
}

void crAnimationPath::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CREANIMATIONPATH){
		id = in->readInt();
		CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
		if(obj){
			((cre::crBase*)(obj))->read(in);
		}
		else
			throw crException("crAnimationPath::read(): Could not cast this CRCore::crAnimationPath to an CRCore::crBase.");
		
		setLoopMode((CRCore::crAnimationPath::LoopMode)in->readInt());

		int size = in->readInt();
		for(int i=0;i<size;i++){
			float time = in->readFloat();
			CRCore::crVector3f pos = in->readVec3();
			CRCore::crQuat rot = in->readQuat();
			CRCore::crVector3f scale = in->readVec3();
			insert(time, CRCore::crAnimationPath::ControlPoint(pos, rot, scale));
		}
	}
	else{
		throw crException("crAnimationPath::read(): Expected crAnimationPath identification.");
	}
}
