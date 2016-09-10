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
#include <Plugin cre/crDrawElementsUByte.h>
#include <Plugin cre/crPrimitive.h>

using namespace cre;

void crDrawElementsUByte::write(crDataOutputStream* out){
    // Write crDrawElementsUByte's identification.
    out->writeInt(CREDRAWELEMENTSUBYTE);

    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crPrimitive*  prim = dynamic_cast<CRCore::crPrimitive*>(this);
    if(prim){
        ((cre::crPrimitive*)(prim))->write(out);
    }
    else
        throw crException("crDrawElementsUByte::write(): Could not cast this CRCore::DrawElementsUByte to an CRCore::crPrimitive.");
    // Write crDrawElementsUByte's properties.

    // Write array length and its elements.
    out->writeUInt(getNumIndices());
	
	for( PT_IndexArray::iterator itr = getIndexArray().begin();
		 itr != getIndexArray().end();
		 ++itr )
	{
        out->writeUChar(*itr);
	}
}

void crDrawElementsUByte::read(crDataInputStream* in){
    // Read crDrawElementsUByte's identification.
    int id = in->peekInt();
    if(id == CREDRAWELEMENTSUBYTE){
        // Code to read crDrawElementsUByte's properties.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crPrimitive*  prim = dynamic_cast<CRCore::crPrimitive*>(this);
        if(prim){
            ((cre::crPrimitive*)(prim))->read(in);
        }
        else
            throw crException("crDrawElementsUByte::read(): Could not cast this CRCore::DrawElementsUByte to an CRCore::PrimtiveSet.");

        // Read array length and its elements.
        unsigned int size = in->readUInt();
		m_indexArray.reserve(size);
		for(int i = 0; i < size; i++ )
		{
            m_indexArray.push_back(in->readUChar());
		}

    }
    else{
        throw crException("crDrawElementsUByte::read(): Expected crDrawElementsUByte identification.");
    }
}
