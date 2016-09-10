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
#include <Plugin cre/crShader.h>
#include <Plugin cre/crBase.h>

using namespace cre;

void crShader::write(crDataOutputStream* out){
    // Write crShader's identification.
    out->writeInt(CRESHADER);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
    if(obj)
    {
        ((cre::crBase*)(obj))->write(out);
    }
    else
        throw crException("crShader::write(): Could not cast this CRCore::crShader to an CRCore::crBase.");

     out->writeString(getName());
     out->writeInt(getType());
     out->writeString(getShaderSource());

}

void crShader::read(crDataInputStream* in)
{
    // Read crShader's identification.
    int id = in->peekInt();
    if(id == CRESHADER)
    {
        // Read crShader's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
        if(obj)
        {
            ((cre::crBase*)(obj))->read(in);
        }
        else
            throw crException("crShader::read(): Could not cast this CRCore::crShader to an CRCore::crBase.");

    }
    else
    {
        throw crException("crShader::read(): Expected crShader identification.");
    }

     setName(in->readString());
     setType(static_cast<Type>(in->readInt()));
     setShaderSource(in->readString());

}
