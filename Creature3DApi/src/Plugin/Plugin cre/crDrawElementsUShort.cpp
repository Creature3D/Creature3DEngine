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
#include <Plugin cre/crDrawElementsUShort.h>
#include <Plugin cre/crPrimitive.h>
//#include <CRCore/crEndian.h>

using namespace cre;

void crDrawElementsUShort::write(crDataOutputStream* out){
    // Write crDrawElementsUShort's identification.
    out->writeInt(CREDRAWELEMENTSUSHORT);

    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crPrimitive*  prim = dynamic_cast<CRCore::crPrimitive*>(this);
    if(prim){
        ((cre::crPrimitive*)(prim))->write(out);
    }
    else
        throw crException("crDrawElementsUShort::write(): Could not cast this CRCore::DrawElementsUShort to an CRCore::crPrimitive.");
    // Write crDrawElementsUShort's properties.

    // Write array length and its elements.
	out->writeUInt(getNumIndices());

	for( PT_IndexArray::iterator itr = getIndexArray().begin();
		itr != getIndexArray().end();
		++itr )
	{
		out->writeUShort(*itr);
	}
}

void crDrawElementsUShort::read(crDataInputStream* in){
    // Read crDrawElementsUShort's identification.
    int id = in->peekInt();
    if(id == CREDRAWELEMENTSUSHORT){
        // Code to read crDrawElementsUShort's properties.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crPrimitive*  prim = dynamic_cast<CRCore::crPrimitive*>(this);
        if(prim){
            ((cre::crPrimitive*)(prim))->read(in);
        }
        else
            throw crException("crDrawElementsUShort::read(): Could not cast this CRCore::DrawElementsUShort to an CRCore::PrimtiveSet.");

        // Read array length and its elements.
		unsigned int size = in->readUInt();
		m_indexArray.reserve(size);
		for(int i = 0; i < size; i++ )
		{
			m_indexArray.push_back(in->readUShort());
		}    
    }
    else{
        throw crException("crDrawElementsUShort::read(): Expected crDrawElementsUShort identification.");
    }
}
