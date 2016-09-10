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
#include <Plugin cre/crTexMat.h>
#include <Plugin cre/crBase.h>

using namespace cre;

void crTexMat::write(crDataOutputStream* out){
	// Write crTexMat's identification.
	out->writeInt(CRETEXMAT);
	// If the CRCore class is inherited by any other class we should also write this to file.
	CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
	if(obj){
		((cre::crBase*)(obj))->write(out);
	}
	else
		throw crException("crTexMat::write(): Could not cast this CRCore::crTexMat to an CRCore::crBase.");
	// Write crTexMat's properties.

	// Write mode
	out->writeMatrixf(getMatrix());
}

void crTexMat::read(crDataInputStream* in){
	// Peek on crTexMat's identification.
	int id = in->peekInt();
	if(id == CRETEXMAT){
		// Read crTexMat's identification.
		id = in->readInt();
		// If the CRCore class is inherited by any other class we should also read this from file.
		CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
		if(obj){
			((cre::crBase*)(obj))->read(in);
		}
		else
			throw crException("crTexMat::read(): Could not cast this CRCore::crTexMat to an CRCore::crBase.");
		// Read crTexMat's properties

		// Read matrix
		setMatrix(in->readMatrixf());

	}
	else{
		throw crException("crTexMat::read(): Expected crTexMat identification.");
	}
}
