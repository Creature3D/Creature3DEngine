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
#include <Plugin cre/crShape.h>
#include <Plugin cre/crBase.h>
#include <CRCore/crEndian.h>

using namespace cre;


////////////////////////////////////////////////////////////////////////////////
//
//  crSphere
//
void crSphere::write(crDataOutputStream* out)
{
    // Write crCullFace's identification.
    out->writeInt(CRESPHERE);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
    if(obj)
    {
        ((cre::crBase*)(obj))->write(out);
    }
    else
        throw crException("crSphere::write(): Could not cast this CRCore::crSphere to an CRCore::crBase.");

    // Write crSphere's properties.
    out->writeVec3(getCenter());
    out->writeFloat(getRadius());
}

void crSphere::read(crDataInputStream* in)
{
    // Peek on crSphere's identification.
    int id = in->peekInt();
    if(id == CRESPHERE)
    {
        // Read crSphere's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
        if(obj)
        {
            ((cre::crBase*)(obj))->read(in);
        }
        else
            throw crException("crSphere::read(): Could not cast this CRCore::crSphere to an CRCore::crBase.");

        // Read crSphere's properties
        setCenter(in->readVec3());
        setRadius(in->readFloat());

    }
    else
    {
        throw crException("crSphere::read(): Expected crSphere identification.");
    }
}


////////////////////////////////////////////////////////////////////////////////
//
//  crBox
//
void crBox::write(crDataOutputStream* out)
{
    // Write crCullFace's identification.
    out->writeInt(CREBOX);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
    if(obj)
    {
        ((cre::crBase*)(obj))->write(out);
    }
    else
        throw crException("crBox::write(): Could not cast this CRCore::crBox to an CRCore::crBase.");

    // Write crBox's properties.
    out->writeVec3(getCenter());
    out->writeVec3(getHalfLengths());
    out->writeQuat(getRotation());
}

void crBox::read(crDataInputStream* in)
{
    // Peek on crBox's identification.
    int id = in->peekInt();
    if(id == CREBOX)
    {
        // Read crBox's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
        if(obj)
        {
            ((cre::crBase*)(obj))->read(in);
        }
        else
            throw crException("crBox::read(): Could not cast this CRCore::crBox to an CRCore::crBase.");

        // Read crBox's properties
        setCenter(in->readVec3());
        setHalfLengths(in->readVec3());
        setRotation(in->readQuat());

    }
    else
    {
        throw crException("crBox::read(): Expected crBox identification.");
    }
}


////////////////////////////////////////////////////////////////////////////////
//
//  crCone
//
void crCone::write(crDataOutputStream* out)
{
    // Write crCullFace's identification.
    out->writeInt(CRECONE);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
    if(obj)
    {
        ((cre::crBase*)(obj))->write(out);
    }
    else
        throw crException("crCone::write(): Could not cast this CRCore::crCone to an CRCore::crBase.");

    // Write crCone's properties.
    out->writeVec3(getCenter());
    out->writeFloat(getRadius());
    out->writeFloat(getHeight());
    out->writeQuat(getRotation());
}

void crCone::read(crDataInputStream* in)
{
    // Peek on crCone's identification.
    int id = in->peekInt();
    if(id == CRECONE)
    {
        // Read crCone's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
        if(obj)
        {
            ((cre::crBase*)(obj))->read(in);
        }
        else
            throw crException("crCone::read(): Could not cast this CRCore::crCone to an CRCore::crBase.");

        // Read crCone's properties
        setCenter(in->readVec3());
        setRadius(in->readFloat());
        setHeight(in->readFloat());
        setRotation(in->readQuat());

    }
    else
    {
        throw crException("crCone::read(): Expected crCone identification.");
    }
}


////////////////////////////////////////////////////////////////////////////////
//
//  crCylinder
//
void crCylinder::write(crDataOutputStream* out)
{
    // Write crCullFace's identification.
    out->writeInt(CRECYLINDER);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
    if(obj)
    {
        ((cre::crBase*)(obj))->write(out);
    }
    else
        throw crException("crCylinder::write(): Could not cast this CRCore::crCylinder to an CRCore::crBase.");

    // Write crCylinder's properties.
    out->writeVec3(getCenter());
    out->writeFloat(getRadius());
    out->writeFloat(getHeight());
    out->writeQuat(getRotation());
}

void crCylinder::read(crDataInputStream* in)
{
    // Peek on crCylinder's identification.
    int id = in->peekInt();
    if(id == CRECYLINDER)
    {
        // Read crCylinder's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
        if(obj)
        {
            ((cre::crBase*)(obj))->read(in);
        }
        else
            throw crException("crCylinder::read(): Could not cast this CRCore::crCylinder to an CRCore::crBase.");

        // Read crCylinder's properties
        setCenter(in->readVec3());
        setRadius(in->readFloat());
        setHeight(in->readFloat());
        setRotation(in->readQuat());

    }
    else
    {
        throw crException("crCylinder::read(): Expected crCylinder identification.");
    }
}


////////////////////////////////////////////////////////////////////////////////
//
//  crCapsule
//
void crCapsule::write(crDataOutputStream* out)
{
    // Write crCullFace's identification.
    out->writeInt(CRECAPSULE);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
    if(obj)
    {
        ((cre::crBase*)(obj))->write(out);
    }
    else
        throw crException("crCapsule::write(): Could not cast this CRCore::crCapsule to an CRCore::crBase.");

    // Write crCapsule's properties.
    out->writeVec3(getCenter());
    out->writeFloat(getRadius());
    out->writeFloat(getHeight());
    out->writeQuat(getRotation());
}

void crCapsule::read(crDataInputStream* in)
{
    // Peek on crCapsule's identification.
    int id = in->peekInt();
    if(id == CRECAPSULE)
    {
        // Read crCapsule's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
        if(obj)
        {
            ((cre::crBase*)(obj))->read(in);
        }
        else
            throw crException("crCapsule::read(): Could not cast this CRCore::crCapsule to an CRCore::crBase.");

        // Read crCapsule's properties
        setCenter(in->readVec3());
        setRadius(in->readFloat());
        setHeight(in->readFloat());
        setRotation(in->readQuat());

    }
    else
    {
        throw crException("crCapsule::read(): Expected crCapsule identification.");
    }
}


////////////////////////////////////////////////////////////////////////////////
//
//  crHeightField
//
void crHeightField::write(crDataOutputStream* out)
{
    // Write crCullFace's identification.
    out->writeInt(CREHEIGHTFIELD);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
    if(obj)
    {
        ((cre::crBase*)(obj))->write(out);
    }
    else
        throw crException("crHeightField::write(): Could not cast this CRCore::crHeightField to an CRCore::crBase.");

    // Write crHeightField's properties.
    out->writeUInt(getNumColumns());
    out->writeUInt(getNumRows());
    out->writeVec3(getOrigin());
    out->writeFloat(getXInterval());
    out->writeFloat(getYInterval());
    out->writeQuat(getRotation());
    out->writeFloat(getSkirtHeight());
    out->writeUInt(getBorderWidth());
    
    unsigned int size = m_heights->size();
    out->writeUInt(size);
    for(unsigned int i = 0; i < size; i++)
    {
        out->writeFloat((*m_heights)[i]);
    }
	if ( out->getVersion() >= VERSION_0015 )
	{
		for(unsigned int i = 0; i < size; i++)
		{
			out->writeFloat((*m_heightsForOde)[i]);
		}
	}

	out->writeString(getHeightMap());
	out->writeFloat(getMaxHeight());
}

void crHeightField::read(crDataInputStream* in)
{
    // Peek on crHeightField's identification.
    int id = in->peekInt();
    if(id == CREHEIGHTFIELD)
    {
        // Read crHeightField's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
        if(obj)
        {
            ((cre::crBase*)(obj))->read(in);
        }
        else
            throw crException("crHeightField::read(): Could not cast this CRCore::crHeightField to an CRCore::crBase.");

        // Read crHeightField's properties
        //setColor(in->readVec4());
        unsigned int col = in->readUInt();
        unsigned int row = in->readUInt();        
        allocate(col,row);

        setOrigin(in->readVec3());
        setXInterval(in->readFloat());
        setYInterval(in->readFloat());
        setRotation(in->readQuat());
        
        setSkirtHeight(in->readFloat());
        setBorderWidth(in->readUInt());
    
        unsigned int size = in->readUInt();
        in->m_istream->read((char*)&((*m_heights)[0]), FLOATSIZE*size);
        if (in->m_istream->rdstate() & in->m_istream->failbit)
            throw crException("crHeightField::read(): Failed to read height array.");
        if (in->m_byteswap) {
           float *ptr = (float*)&((*m_heights)[0]) ;
           for (unsigned int i = 0 ; i < size ; i++ ) {
              CRCore::swapBytes((char *)&(ptr[i]),FLOATSIZE) ;
           }
        }
		//for(unsigned int i = 0; i < size; i++)
		//{
		//	(*m_heights)[i] = in->readFloat();
		//}
		if ( in->getVersion() >= VERSION_0015 )
		{
			//for(unsigned int i = 0; i < size; i++)
			//{
			//	(*m_heightsForOde)[i] = in->readFloat();
			//}
			in->m_istream->read((char*)&((*m_heightsForOde)[0]), FLOATSIZE*size);
			if (in->m_istream->rdstate() & in->m_istream->failbit)
				throw crException("crHeightField::read(): Failed to read height array.");
			if (in->m_byteswap) {
				float *ptr = (float*)&((*m_heightsForOde)[0]) ;
				for (unsigned int i = 0 ; i < size ; i++ ) {
					CRCore::swapBytes((char *)&(ptr[i]),FLOATSIZE) ;
				}
			}
		}

		m_heightMap = in->readString();
		m_maxHeight = in->readFloat();
    }
    else
    {
        throw crException("crHeightField::read(): Expected crHeightField identification.");
    }
}

