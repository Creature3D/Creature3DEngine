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
#include <Plugin cre/crDrawArrays.h>
#include <Plugin cre/crPrimitive.h>

using namespace cre;

void crDrawArrays::write(crDataOutputStream* out){
    // Write crDrawArrays's identification.
    out->writeInt(CREDRAWARRAYS);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crPrimitive*  prim = dynamic_cast<CRCore::crPrimitive*>(this);
    if(prim){
        ((cre::crPrimitive*)(prim))->write(out);
    }
    else
        throw crException("crDrawArrays::write(): Could not cast this CRCore::DrawArrays to an CRCore::crPrimitive.");


    // Write crDrawArrays's properties.
    out->writeInt(getFirst());
    out->writeInt(getCount());

	out->writeUInt(getNumIndices());

	for( PT_IndexArray::iterator itr = getIndexArray().begin();
		itr != getIndexArray().end();
		++itr )
	{
		out->writeInt(*itr);
	}
}

void crDrawArrays::read(crDataInputStream* in){
    // Read crDrawArrays's identification.
    int id = in->peekInt();
    if(id == CREDRAWARRAYS){
        // Code to read crDrawArrays's properties.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crPrimitive*  prim = dynamic_cast<CRCore::crPrimitive*>(this);
        if(prim){
            ((cre::crPrimitive*)(prim))->read(in);
        }
        else
            throw crException("crDrawArrays::read(): Could not cast this CRCore::DrawArrays to an CRCore::crPrimitive.");


        // Read crDrawArrays properties
        setFirst(in->readInt());
        setCount(in->readInt());

		unsigned int size = in->readUInt();
		m_indexArray.reserve(size);
		for(int i = 0; i < size; i++ )
		{
			m_indexArray.push_back(in->readInt());
		}
    }
    else{
        throw crException("crDrawArrays::read(): Expected crDrawArrays identification.");
    }
}
