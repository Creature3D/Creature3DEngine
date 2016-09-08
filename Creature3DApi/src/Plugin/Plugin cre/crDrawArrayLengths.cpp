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
#include <Plugin cre/crDrawArrayLengths.h>
#include <Plugin cre/crPrimitive.h>

using namespace cre;

void crDrawArrayLengths::write(crDataOutputStream* out){
    // Write crDrawArrayLengths's identification.
    out->writeInt(CREDRAWARRAYLENGTHS);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crPrimitive*  prim = dynamic_cast<CRCore::crPrimitive*>(this);
    if(prim){
        ((cre::crPrimitive*)(prim))->write(out);
    }
    else
        throw crException("crDrawArrayLengths::write(): Could not cast this CRCore::DrawArrayLengths to an CRCore::crPrimitive.");

    // Write crDrawArrayLengths's properties.
    out->writeInt(getFirst());

    // Write array length and its elements.
	out->writeUInt(getNumIndices());

	for( PT_IndexArray::iterator itr = getIndexArray().begin();
		itr != getIndexArray().end();
		++itr )
	{
		out->writeInt(*itr);
	}

}

void crDrawArrayLengths::read(crDataInputStream* in){
    // Read crDrawArrayLengths's identification.
    int id = in->peekInt();
    if(id == CREDRAWARRAYLENGTHS){
        // Code to read crDrawArrayLengths's properties.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crPrimitive*  prim = dynamic_cast<CRCore::crPrimitive*>(this);
        if(prim){
            ((cre::crPrimitive*)(prim))->read(in);
        }
        else
            throw crException("crDrawArrayLengths::read(): Could not cast this CRCore::DrawArrayLengths to an CRCore::PrimtiveSet.");

        // Read properties
        setFirst(in->readInt());
    
        // Read array length and its elements.
		unsigned int size = in->readUInt();
		m_indexArray.reserve(size);
		for(int i = 0; i < size; i++ )
		{
			m_indexArray.push_back(in->readInt());
		}

    }
    else{
        throw crException("crDrawArrayLengths::read(): Expected crDrawArrayLengths identification.");
    }
}
