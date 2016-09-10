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
#include <Plugin cre/crDrawable.h>
#include <Plugin cre/crClusterCullingCallback.h>
#include <Plugin cre/crBase.h>
#include <Plugin cre/crStateSet.h>


using namespace cre;

void crDrawable::write(crDataOutputStream* out)
{
    // Write crDrawable's identification.
    out->writeInt(CREDRAWABLE);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
    if(obj){
        ((cre::crBase*)(obj))->write(out);
    }
    else
        throw crException("crDrawable::write(): Could not cast this CRCore::crDrawable to an CRCore::crBase.");

    // Write crDrawable's properties.
    out->writeString(getName());

    // Write stateset if any
    out->writeBool(getStateSet()!=0);
    if (getStateSet())
    {
        out->writeStateSet(getStateSet());
    }
    
      
    CRCore::crClusterCullingCallback* ccc = dynamic_cast<CRCore::crClusterCullingCallback*>(getCullCallback());
    out->writeBool(ccc!=0);
    if(ccc)
    {
        ((cre::crClusterCullingCallback*)(ccc))->write(out);
    }
    

  /*  if (out->getVersion() >= VERSION_0010)
    {
        const CRCore::crBoundingBox& bb = getInitialBound();
        out->writeBool(bb.valid());
        if (bb.valid())
        {
            out->writeFloat(bb.xMin());
            out->writeFloat(bb.yMin());
            out->writeFloat(bb.zMin());
            out->writeFloat(bb.xMax());
            out->writeFloat(bb.yMax());
            out->writeFloat(bb.zMax());
        }
    }*/

    // Write support display list.
    out->writeBool(getSupportsDisplayList());

    // Write use display list.
    out->writeBool(getUseDisplayList());

    // Write use display list.
    out->writeBool(getUseVertexBufferObjects());

	out->writeBool(getSupportsVertexBufferObjects());

	out->writeBool(getUseIndexBufferObjects());

    out->writeBool(getVisiable());

	if (out->getVersion() >= VERSION_0013)
    {
		out->writeBool(m_shape.valid());
		if(m_shape.valid())
		    out->writeShape(m_shape.get());
    }
	if ( out->getVersion() >= VERSION_0021 )
		out->writeVec4(getColor());
}

void crDrawable::read(crDataInputStream* in)
{
    // Read crDrawable's identification.
    int id = in->peekInt();
    if(id == CREDRAWABLE)
    {
        // Code to read crDrawable's properties.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
        if(obj){
            ((cre::crBase*)(obj))->read(in);
        }
        else
            throw crException("crDrawable::read(): Could not cast this CRCore::crDrawable to an CRCore::crBase.");

        // Read in drawable's properties
        setName(in->readString());


        // Read stateset if any
        if(in->readBool())
        {
            setStateSet(in->readStateSet());
        }

        if(in->readBool())
        {
            CRCore::crClusterCullingCallback* ccc = new CRCore::crClusterCullingCallback();
            ((cre::crClusterCullingCallback*)(ccc))->read(in);
            setCullCallback(ccc);
        }

        
  /*      if (in->getVersion() >= VERSION_0010)
        {
            if (in->readBool())
            {
                CRCore::crBoundingBox bb;
                bb.xMin() = in->readFloat();
                bb.yMin() = in->readFloat();
                bb.zMin() = in->readFloat();
                bb.xMax() = in->readFloat();
                bb.yMax() = in->readFloat();
                bb.zMax() = in->readFloat();
                setInitialBound(bb);
            }
        }*/

        // Read support display list
        setSupportsDisplayList(in->readBool());

        // Read use display list
        setUseDisplayList(in->readBool());

        // Read use display list
        setUseVertexBufferObjects(in->readBool());
        
		setSupportsDisplayList(in->readBool());

		setUseIndexBufferObjects(in->readBool());

		setVisiable(in->readBool());

		if (in->getVersion() >= VERSION_0013)
		{
			if(in->readBool())
			{
                setShape(in->readShape());
			}
		}
		if ( in->getVersion() >= VERSION_0021 )
			setColor(in->readVec4());
    }
    else{
        throw crException("crDrawable::read(): Expected crDrawable identification.");
    }
}
