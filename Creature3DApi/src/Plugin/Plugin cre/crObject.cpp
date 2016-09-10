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
#include <Plugin cre/crNode.h>
#include <Plugin cre/crObject.h>
#include <Plugin cre/crGeometry.h>

using namespace cre;

void crObject::write(crDataOutputStream* out){
    // Write crObject's identification.
    out->writeInt(CREOBJECT);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crNode*  node = dynamic_cast<CRCore::crNode*>(this);
    if(node){
        ((cre::crNode*)(node))->write(out);
    }
    else
        throw crException("crObject::write(): Could not cast this CRCore::crObject to an CRCore::crNode.");


    // Write crObject's properties.

    // Write number of drawables.
    out->writeInt(getNumDrawables());
    // Write out every drawable.
    for(unsigned int i=0;i<getNumDrawables();i++)
	{
        CRCore::crDrawable* drawable = getDrawable(i);
        out->writeDrawable(drawable);
    }

    //out->writeInt(getCollideMode());
	out->writeBool(m_effectByShadow);
	out->writeBool(m_needSelectLight);
	if (out->getVersion() >= VERSION_0032)
	{
		out->writeChar(m_acceptGI);
	}
	if (out->getVersion() >= VERSION_0033)
	{
		out->writeChar(m_drawOutLine);
	}
}

void crObject::read(crDataInputStream* in){
    // Read crObject's identification.
    int id = in->peekInt();
    if(id == CREOBJECT){
        // Code to read crObject's properties.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crNode*  node = dynamic_cast<CRCore::crNode*>(this);
        if(node){
            ((cre::crNode*)(node))->read(in);
        }
        else
            throw crException("crObject::read(): Could not cast this CRCore::crObject to an CRCore::crNode.");

        // Read number of drawables.
        int size = in->readInt();
        // Read the drawables.
        for(int i = 0; i<size; i++)
		{
            addDrawable(in->readDrawable());
        }
		//setCollideMode((CollideMode)(in->readInt()));

		m_effectByShadow = in->readBool();
		m_needSelectLight = in->readBool();
		if (in->getVersion() >= VERSION_0032)
		{
			m_acceptGI = in->readChar();
		}
		if (in->getVersion() >= VERSION_0033)
		{
			m_drawOutLine = in->readChar();
		}
    }
    else{
        throw crException("crObject::read(): Expected crObject identification.");
    }
}
