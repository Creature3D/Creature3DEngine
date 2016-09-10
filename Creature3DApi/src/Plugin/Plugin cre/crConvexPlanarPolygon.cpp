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
#include <Plugin cre/crConvexPlanarPolygon.h>

using namespace cre;

void crConvexPlanarPolygon::write(crDataOutputStream* out){
	// Write crConvexPlanarPolygon's identification.
	out->writeInt(CRECONVEXPLANARPOLYGON);
	// If the CRCore class is inherited by any other class we should also write this to file.
	//CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
	//if(obj){
	//	((cre::crBase*)(obj))->write(out);
	//}
	//else
	//	throw crException("crConvexPlanarPolygon::write(): Could not cast this CRCore::crConvexPlanarPolygon to an CRCore::crBase.");
	// Write crConvexPlanarPolygon's properties.

	// Write Vertex list
	VertexList vertexList = getVertexList();
	int size = vertexList.size();
	out->writeInt(size);
	for(int i=0; i<size; i++){
		out->writeVec3(vertexList[i]);
	}

}

void crConvexPlanarPolygon::read(crDataInputStream* in){
	// Peek on crConvexPlanarPolygon's identification.
	int id = in->peekInt();
	if(id == CRECONVEXPLANARPOLYGON){
		// Read crConvexPlanarPolygon's identification.
		id = in->readInt();
		// If the CRCore class is inherited by any other class we should also read this from file.
		//CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
		//if(obj){
		//	((cre::crBase*)(obj))->read(in);
		//}
		//else
		//	throw crException("crConvexPlanarPolygon::read(): Could not cast this CRCore::crConvexPlanarPolygon to an CRCore::crBase.");
		// Read crConvexPlanarPolygon's properties

		// Read Vertex list
		int size = in->readInt();
		for(int i=0; i<size; i++){
			add(in->readVec3());
		}

	}
	else{
		throw crException("crConvexPlanarPolygon::read(): Expected crConvexPlanarPolygon identification.");
	}
}
