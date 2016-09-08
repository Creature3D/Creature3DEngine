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
#include <Plugin cre/crBase.h>

using namespace cre;

void crBase::write(crDataOutputStream* out){
	out->writeInt(CREBASE);

	switch(getDataVariance()){
        case(CRCore::crBase::STATIC):	out->writeChar((char)0); break;
		case(CRCore::crBase::DYNAMIC): out->writeChar((char)1); break;	
    }
}

void crBase::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CREBASE){
		id = in->readInt();

		char c = in->readChar();
		switch((int)c){
			case 0: setDataVariance(CRCore::crBase::STATIC);break;
			case 1: setDataVariance(CRCore::crBase::DYNAMIC);break;
		}
	}
	else{
		throw crException("crBase::read(): Expected crBase identification");
	}
}
