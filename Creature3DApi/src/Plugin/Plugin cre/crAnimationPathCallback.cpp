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
#include <Plugin cre/crAnimationPathCallback.h>
#include <Plugin cre/crAnimationPath.h>
#include <Plugin cre/crBase.h>

using namespace cre;

void crAnimationPathCallback::write(crDataOutputStream* out){
	out->writeInt(CREANIMATIONPATHCALLBACK);
	CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
	if(obj){
		((cre::crBase*)(obj))->write(out);
	}
	else
		throw crException("crAnimationPathCallback::write(): Could not cast this CRCore::crAnimationPathCallback to an CRCore::crBase.");

	out->writeVec3(m_pivotPoint);
	out->writeDouble(m_timeOffset);
	out->writeDouble(m_timeMultiplier);
	out->writeDouble(m_firstTime);
	out->writeDouble(m_pauseTime);

	if(getAnimationPath())
        {
        	out->writeInt(1);
		((cre::crAnimationPath*)(getAnimationPath()))->write(out);
	}
        else
        {
        	out->writeInt(0);
        }
}

void crAnimationPathCallback::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CREANIMATIONPATHCALLBACK){
		id = in->readInt();
		CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
		if(obj){
			((cre::crBase*)(obj))->read(in);
		}
		else
			throw crException("crAnimationPathCallback::read(): Could not cast this CRCore::crAnimationPathCallback to an CRCore::crBase.");

		m_pivotPoint = in->readVec3();
		m_timeOffset = in->readDouble();
		m_timeMultiplier = in->readDouble();
		m_firstTime = in->readDouble();
		m_pauseTime = in->readDouble();

		if(in->readInt())
                {
			CRCore::crAnimationPath* path = new CRCore::crAnimationPath();
			((cre::crAnimationPath*)(path))->read(in);
			setAnimationPath(path);
		}
	}
	else{
		throw crException("crAnimationPathCallback::read(): Expected crAnimationPathCallback identification.");
	}
}
