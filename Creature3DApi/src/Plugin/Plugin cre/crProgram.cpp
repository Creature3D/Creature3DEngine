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
#include <Plugin cre/crProgram.h>
#include <Plugin cre/crBase.h>

using namespace cre;

void crProgram::write(crDataOutputStream* out){
    // Write crProgram's identification.
    out->writeInt(CREPROGRAM);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
    if(obj)
    {
        ((cre::crBase*)(obj))->write(out);
    }
    else
        throw crException("crProgram::write(): Could not cast this CRCore::crProgram to an CRCore::crBase.");

    const AttribBindingList& abl = getAttribBindingList();
    out->writeUInt(abl.size());
    for(AttribBindingList::const_iterator itr = abl.begin();
        itr != abl.end();
        ++itr)
    {
        out->writeString(itr->first);
        out->writeUInt(itr->second);
    }

    // Write 
    out->writeUInt(getNumShaders());
    for(unsigned int si=0; si<getNumShaders(); ++si)
    {
        out->writeShader(getShader(si));
    }
}

void crProgram::read(crDataInputStream* in)
{
    // Read crProgram's identification.
    int id = in->peekInt();
    if(id == CREPROGRAM)
    {
        // Read crProgram's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
        if(obj)
        {
            ((cre::crBase*)(obj))->read(in);
        }
        else
            throw crException("crProgram::read(): Could not cast this CRCore::crProgram to an CRCore::crBase.");

    }
    else
    {
        throw crException("crProgram::read(): Expected crProgram identification.");
    }

    // reading in shaders.
    unsigned int size = in->readUInt();
    for(unsigned int ai=0; ai<size; ++ai)
    {
        std::string name = in->readString();
        unsigned int index = in->readUInt();
        addBindAttribLocation(name, index);
    }

    // reading in shaders.
    size = in->readUInt();
    for(unsigned int si=0; si<size; ++si)
    {
        CRCore::crShader* shader = in->readShader();
        addShader(shader);
    }

}
