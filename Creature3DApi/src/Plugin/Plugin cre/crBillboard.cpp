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
#include <Plugin cre/crBillboard.h>
#include <Plugin cre/crObject.h>
#include <Plugin cre/crGeometry.h>
using namespace cre;

void crBillboard::write(crDataOutputStream* out){
	out->writeInt(CREBILLBOARD);
	CRCore::crObject*  geode = dynamic_cast<CRCore::crObject*>(this);
	if(geode){
		((cre::crObject*)(geode))->write(out);
	}
	else
		throw crException("crBillboard::write(): Could not cast this CRCore::crBillboard to an CRCore::crObject.");

	out->writeInt(getMode());
	out->writeVec3(getAxis());
	out->writeVec3(getNormal());
	int size = m_positionList.size();
	out->writeInt(size);
	for(int i=0;i<size;i++){
		out->writeVec3(m_positionList[i]);
	}

	size = m_indexOffsetList.size();
	out->writeInt(size);
	for(int i=0;i<size;i++){
		out->writeInt(m_indexOffsetList[i]);
	}

	if ( out->getVersion() < VERSION_0023 )
	{
		out->writeBool(m_init);
		out->writeBool(m_composeGeo.valid());
		if(m_composeGeo.valid())
			out->writeDrawable(m_composeGeo.get());
	}

	if ( out->getVersion() >= VERSION_0017 )
	{
		out->writeBool( getStateSet()!=0);
		if(getStateSet())
			out->writeStateSet(getStateSet());
	}
}

void crBillboard::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CREBILLBOARD){
		id = in->readInt();
		CRCore::crObject*  geode = dynamic_cast<CRCore::crObject*>(this);
		if(geode){
			((cre::crObject*)(geode))->read(in);
		}
		else
			throw crException("crBillboard::read(): Could not cast this CRCore::crBillboard to an CRCore::crObject.");

		setMode((CRCore::crBillboard::Mode)in->readInt());
		setAxis(in->readVec3());
		setNormal(in->readVec3());
		int size = in->readInt();
		m_positionList.reserve(size);
		for(int i=0;i<size;i++){
			m_positionList.push_back(in->readVec3());
		}

		size = in->readInt();
		m_indexOffsetList.reserve(size);
		for(int i=0;i<size;i++){
			m_indexOffsetList.push_back(in->readInt());
		}

		if ( in->getVersion() < VERSION_0023 )
		{
			m_init = in->readBool();
			if(in->readBool())
			{
				m_composeGeo = dynamic_cast<CRCore::crGeometry *>(in->readDrawable());
			}
		}
		m_init = false;

		setEnableIntersect(false);

		if ( in->getVersion() >= VERSION_0017 )
		{
			if(in->readBool())
				setStateSet(in->readStateSet());
		}
	}
	else{
		throw crException("crBillboard::read(): Expected crBillboard identification.");
	}
}
