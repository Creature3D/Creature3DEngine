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
#include <Plugin cre/crCollideCallback.h>
using namespace cre;

void crCollideCallback::write(crDataOutputStream* out){

	out->writeInt(CRECOLLIDECALLBACK);

	out->writeBool(crNestedCallbackPtr.valid());
	if(crNestedCallbackPtr.valid())
	{
		out->writeCollideCallback(crNestedCallbackPtr.get());
	}
}

void crCollideCallback::read(crDataInputStream* in){

	int id = in->peekInt();
	if(id == CRECOLLIDECALLBACK)
	{
		id = in->readInt();

		if(in->readBool())
		{
            crNestedCallbackPtr = in->readCollideCallback();
		}
	}
	else
	{
		throw crException("crCollideCallback::read(): Expected crCollideCallback identification");
	}
}
