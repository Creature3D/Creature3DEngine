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
#include <Plugin cre/crTexGen.h>
#include <Plugin cre/crBase.h>

using namespace cre;

void crTexGen::write(crDataOutputStream* out){
	// Write crTexGen's identification.
	out->writeInt(CRETEXGEN);
	// If the CRCore class is inherited by any other class we should also write this to file.
	CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
	if(obj){
		((cre::crBase*)(obj))->write(out);
	}
	else
		throw crException("crTexGen::write(): Could not cast this CRCore::crTexGen to an CRCore::crBase.");
	// Write crTexGen's properties.

	// Write mode
	out->writeInt(getMode());

        if ( out->getVersion() >= VERSION_0005 )
        {
            out->writePlane(getPlane(CRCore::crTexGen::S));
            out->writePlane(getPlane(CRCore::crTexGen::T));
            out->writePlane(getPlane(CRCore::crTexGen::R));
            out->writePlane(getPlane(CRCore::crTexGen::Q));
        }
}

void crTexGen::read(crDataInputStream* in){
	// Peek on crTexGen's identification.
	int id = in->peekInt();
	if(id == CRETEXGEN){
		// Read crTexGen's identification.
		id = in->readInt();
		// If the CRCore class is inherited by any other class we should also read this from file.
		CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
		if(obj){
			((cre::crBase*)(obj))->read(in);
		}
		else
			throw crException("crTexGen::read(): Could not cast this CRCore::crTexGen to an CRCore::crBase.");
		// Read crTexGen's properties
		setMode((CRCore::crTexGen::Mode)in->readInt());
                
                if ( in->getVersion() >= VERSION_0005 )
                {
                    setPlane(CRCore::crTexGen::S, in->readPlane());
                    setPlane(CRCore::crTexGen::T, in->readPlane());
                    setPlane(CRCore::crTexGen::R, in->readPlane());
                    setPlane(CRCore::crTexGen::Q, in->readPlane());
                }

	}
	else{
		throw crException("crTexGen::read(): Expected crTexGen identification.");
	}
}
