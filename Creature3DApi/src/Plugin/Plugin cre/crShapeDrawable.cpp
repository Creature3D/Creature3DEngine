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

#include <Plugin cre/crDrawable.h>
#include <Plugin cre/crException.h>
#include <Plugin cre/crShapeDrawable.h>
#include <Plugin cre/crBase.h>

using namespace cre;

void crShapeDrawable::write(crDataOutputStream* out)
{
    // Write crCullFace's identification.
    out->writeInt(CRESHAPEDRAWABLE);

    if ( out->getVersion() >= VERSION_0004 )
    {
        // If the CRCore class is inherited by any other class we should also write this to file.
        CRCore::crDrawable*  drawable = dynamic_cast<CRCore::crDrawable*>(this);
        if(drawable){
            ((cre::crDrawable*)(drawable))->write(out);
        }
        else
            throw crException("crShapeDrawable::write(): Could not cast this CRCore::crShapeDrawable to an CRCore::crBase.");

    }
    else
    {
        CRCore::crBase* obj = dynamic_cast<CRCore::crBase*>(this);
        if(obj){
            ((cre::crBase*)(obj))->write(out);
        }
        else
            throw crException("crShapeDrawable::write(): Could not cast this CRCore::crShapeDrawable to an CRCore::crBase.");
    }

	if ( out->getVersion() < VERSION_0021 )
        out->writeVec4(getColor());

    if (getShape())
    {
        out->writeBool(true);
        out->writeShape(getShape());
    }
    else
    {
        out->writeBool(false);
    }
}

void crShapeDrawable::read(crDataInputStream* in)
{
    // Peek on crShapeDrawable's identification.
    int id = in->peekInt();
    if(id == CRESHAPEDRAWABLE)
    {
        // Read crShapeDrawable's identification.
        id = in->readInt();

        if ( in->getVersion() >= VERSION_0004 )
        {
            // If the CRCore class is inherited by any other class we should also read this from file.
            CRCore::crDrawable*  drawable = dynamic_cast<CRCore::crDrawable*>(this);
            if(drawable){
                ((cre::crDrawable*)(drawable))->read(in);
            }
            else
                throw crException("crShapeDrawable::read(): Could not cast this CRCore::crShapeDrawable to an CRCore::crBase.");
        }
        else
        {
            // If the CRCore class is inherited by any other class we should also read this from file.
            CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
            if(obj){
                ((cre::crBase*)(obj))->read(in);
            }
            else
                throw crException("crShapeDrawable::read(): Could not cast this CRCore::crShapeDrawable to an CRCore::crBase.");
        }
    
		if ( in->getVersion() < VERSION_0021 )
            setColor(in->readVec4());

        if (in->readBool())
        {
            setShape(in->readShape());
        }

    }
    else
    {
        throw crException("crShapeDrawable::read(): Expected crShapeDrawable identification.");
    }
}
