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
#include <Plugin cre/crGeometry.h>
#include <Plugin cre/crDrawable.h>
#include <Plugin cre/crDrawArrays.h>
#include <Plugin cre/crDrawArrayLengths.h>
#include <Plugin cre/crDrawElementsUByte.h>
#include <Plugin cre/crDrawElementsUShort.h>
#include <Plugin cre/crDrawElementsUInt.h>
#include <CRCore/crNotify.h>

using namespace cre;

void crGeometry::write(crDataOutputStream* out){
    // Write crGeometry's identification.
    out->writeInt(CREGEOMETRY);

    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crDrawable*  drawable = dynamic_cast<CRCore::crDrawable*>(this);
    if(drawable){
        ((cre::crDrawable*)(drawable))->write(out);
    }
    else
        throw crException("crGeometry::write(): Could not cast this CRCore::crGeometry to an CRCore::crDrawable.");


    // Write crGeometry's properties.

    // Write primitiveset list.
    int size = getNumPrimitives();
    out->writeInt(size);
    for(int i=0;i<size;i++){
        if(dynamic_cast<CRCore::DrawArrays*>(getPrimitive(i)))
            ((cre::crDrawArrays*)(getPrimitive(i)))->write(out);
        else if(dynamic_cast<CRCore::DrawArrayLengths*>(getPrimitive(i)))
            ((cre::crDrawArrayLengths*)(getPrimitive(i)))->write(out);
        else if(dynamic_cast<CRCore::DrawElementsUByte*>(getPrimitive(i)))
            ((cre::crDrawElementsUByte*)(getPrimitive(i)))->write(out);
        else if(dynamic_cast<CRCore::DrawElementsUShort*>(getPrimitive(i)))
            ((cre::crDrawElementsUShort*)(getPrimitive(i)))->write(out);
        else if(dynamic_cast<CRCore::DrawElementsUInt*>(getPrimitive(i)))
            ((cre::crDrawElementsUInt*)(getPrimitive(i)))->write(out);
        else
            throw crException("Unknown PrimitivSet in crGeometry::write()");
    }

    // Write vertex array if any
    out->writeBool(getVertexArray()!=0);
    if (getVertexArray())
    {
		//out->writeInt(getVertexBindMode());
		out->writeArray(getVertexArray());
		//out->writeULong(getVertexStride());
    }
    // Write vertex indices if any
    out->writeBool(getVertexIndices()!=0);
    if (getVertexIndices())
	{
        out->writeArray(getVertexIndices());
    }

    // Write normal array if any
    out->writeBool(getNormalArray()!=0);
    if (getNormalArray())
	{
        out->writeBinding(getNormalBinding());
        out->writeVec3Array(getNormalArray());
    }
    // Write normal indices if any
    out->writeBool(getNormalIndices()!=0);
    if (getNormalIndices())
	{
        out->writeArray(getNormalIndices());        
    }
    // Write color array if any.
	if ( out->getVersion() >= VERSION_0036 )
	{
		if(getColorBinding() != CRCore::crGeometry::BIND_PER_VERTEX)
		{
			setColorArray(NULL);
			setColorIndices(NULL);
			setColorBinding(CRCore::crGeometry::BIND_OFF);
		}
		out->writeBool(getColorArray()!=0);
		if (getColorArray()){
			out->writeBinding(getColorBinding());
			out->writeArray(getColorArray());
		}
		//Write color indices if any
		out->writeBool(getColorIndices()!=0);
		if (getColorIndices()){
			out->writeArray(getColorIndices());        
		}
	}
    // Write secondary color array if any
    out->writeBool(getSecondaryColorArray()!=0);
    if (getSecondaryColorArray())
	{
        out->writeBinding(getSecondaryColorBinding());
        out->writeArray(getSecondaryColorArray());
    }
    // Write second color indices if any
    out->writeBool(getSecondaryColorIndices()!=0);
    if (getSecondaryColorIndices())
	{
        out->writeArray(getSecondaryColorIndices());
    }
    // Write fog coord array if any
    out->writeBool(getFogCoordArray()!=0);
    if (getFogCoordArray())
	{
        out->writeBinding(getFogCoordBinding());
        out->writeArray(getFogCoordArray());
    }
    // Write fog coord indices if any
    out->writeBool(getFogCoordIndices()!=0);
    if (getFogCoordIndices())
	{
        out->writeArray(getFogCoordIndices());
    }
    // Write texture coord arrays
    crGeometry::ArrayList& tcal = getTexCoordArrayList();
	if(tcal.size()>TEXTURE_LIGHTMAP+1)
	{
		CRCore::notify(CRCore::ALWAYS) << "crGeometry::write TexCoordArraySize = " <<tcal.size()<<" ObjectName: "<<getParent(0)->getName()<< std::endl;
	}
    out->writeInt(tcal.size());
    unsigned int j;
    for(j=0;j<tcal.size();j++)
    {
        // Write coords if valid
        out->writeBool(tcal[j].array.valid());
        if (tcal[j].array.valid()){
            out->writeArray(tcal[j].array.get());
        }
        // Write indices if valid
        out->writeBool(tcal[j].indices.valid());
        if (tcal[j].indices.valid()){
            out->writeArray(tcal[j].indices.get());
        }
    }

    // Write vertex attributes
    crGeometry::ArrayList& vaal = getVertexAttribArrayList();
    out->writeInt(vaal.size());
    for(j=0;j<vaal.size();j++)
    {
        // Write coords if valid
        const CRCore::crGeometry::ArrayData& arrayData = vaal[j];
        out->writeBinding(arrayData.binding);
        out->writeBool(arrayData.normalize==GL_TRUE);
        out->writeBool(arrayData.array.valid());
        if (arrayData.array.valid()){
            out->writeArray(arrayData.array.get());
        }
        // Write indices if valid
        out->writeBool(arrayData.indices.valid());
        if (arrayData.indices.valid()){
            out->writeArray(arrayData.indices.get());
        }
    }

	out->writeBool(getFastPathHint());


}

void crGeometry::read(crDataInputStream* in){
    // Read crGeometry's identification.
    int id = in->peekInt();
    if(id == CREGEOMETRY){
        // Code to read crGeometry's properties.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crDrawable*  drawable = dynamic_cast<CRCore::crDrawable*>(this);
        if(drawable){
            ((cre::crDrawable*)(drawable))->read(in);
        }
        else
            throw crException("crGeometry::read(): Could not cast this CRCore::crGeometry to an CRCore::crDrawable.");


        // Read geoemtry properties

        // Read primitiveset list.
        int size = in->readInt();
        int i;
        for(i=0;i<size;i++){
            CRCore::crPrimitive* prim;
            int primID = in->peekInt();
            if(primID==CREDRAWARRAYS){
                prim = new CRCore::DrawArrays();
                ((cre::crDrawArrays*)(prim))->read(in);
                addPrimitive(prim);
            }
            else if(primID==CREDRAWARRAYLENGTHS){
                prim = new CRCore::DrawArrayLengths();
                ((cre::crDrawArrayLengths*)(prim))->read(in);
                addPrimitive(prim);
            }
            else if(primID==CREDRAWELEMENTSUBYTE){
                prim = new CRCore::DrawElementsUByte();
                ((cre::crDrawElementsUByte*)(prim))->read(in);
                addPrimitive(prim);
            }
            else if(primID==CREDRAWELEMENTSUSHORT){
                prim = new CRCore::DrawElementsUShort();
                ((cre::crDrawElementsUShort*)(prim))->read(in);
                addPrimitive(prim);
            }
            else if(primID==CREDRAWELEMENTSUINT){
                prim = new CRCore::DrawElementsUInt();
                ((cre::crDrawElementsUInt*)(prim))->read(in);
                addPrimitive(prim);
            }
            else{
				//throw crException("Unknown crPrimitive in crGeometry::read() ID:");
				CRCore::notify(CRCore::FATAL)<<"Unknown crPrimitive in crGeometry::read() ID:"<<primID<<std::endl;
            }
        }
    
        // Read vertex array if any
        bool va=in->readBool();
        if (va){
			if ( in->getVersion() < VERSION_0041 )
			{
				//setVertexBindMode((BindMode)in->readInt());
				in->readInt();
			}
            setVertexArray(in->readArray());
			if ( in->getVersion() < VERSION_0041 )
			{
				//setVertexStride(in->readULong());
				in->readULong();
			}
        }
        // Read vertex indices if any
        bool vi = in->readBool();
        if (vi){
            setVertexIndices(static_cast<CRCore::IndexArray*>(in->readArray()));
        }
        // Read normal array if any
        bool na =in->readBool();
        if(na){
            setNormalBinding(in->readBinding());
            setNormalArray(in->readVec3Array());
        }
        // Read normal indices if any
        bool ni = in->readBool();
        if(ni){
            setNormalIndices(static_cast<CRCore::IndexArray*>(in->readArray()));
        }
        // Read color array if any.
		if ( in->getVersion() >= VERSION_0036 )
		{
			if(in->readBool()){
				setColorBinding(in->readBinding());
				setColorArray(in->readArray());
			}
			// Read color indices if any
			if(in->readBool()){
				setColorIndices(static_cast<CRCore::IndexArray*>(in->readArray()));
			}

			if(getColorBinding() != CRCore::crGeometry::BIND_PER_VERTEX)
			{
				setColorArray(NULL);
				setColorIndices(NULL);
				setColorBinding(CRCore::crGeometry::BIND_OFF);
			}
		}
        // Read secondary color array if any
        if(in->readBool()){
            setSecondaryColorBinding(in->readBinding());
            setSecondaryColorArray(in->readArray());
        }
        // Read second color indices if any
        if(in->readBool()){
            setSecondaryColorIndices(static_cast<CRCore::IndexArray*>(in->readArray()));
        }
        // Read fog coord array if any
        if(in->readBool()){
            setFogCoordBinding(in->readBinding());
            setFogCoordArray(in->readArray());
        }
        // Read fog coord indices if any
        if(in->readBool()){
            setFogCoordIndices(static_cast<CRCore::IndexArray*>(in->readArray()));
        }
        // Read texture coord arrays
        size = in->readInt();
		if(size>TEXTURE_LIGHTMAP+1)
		{
			CRCore::notify(CRCore::ALWAYS) << "crGeometry::read TexCoordArraySize = " <<size<< std::endl;
		}
        for(i =0;i<size;i++)
        {
            // Read coords if valid
            bool coords_valid = in->readBool();
            if(coords_valid)
			{
				CRCore::Array* array = in->readArray();
                if(i==0||i==TEXTURE_LIGHTMAP)
                    setTexCoordArray(i, array);
				else if(array)
					delete array;
			}
            // Read Indices if valid
            bool indices_valid = in->readBool();
            if(indices_valid)
			{
				CRCore::Array* array = in->readArray();
				if(i==0||i==TEXTURE_LIGHTMAP)
                    setTexCoordIndices(i, static_cast<CRCore::IndexArray*>(array));
				else if(array)
					delete array;
			}
        }

        // Read vertex attrib arrays
        size = in->readInt();
        for(i =0;i<size;i++)
        {
            setVertexAttribBinding(i,in->readBinding());
            setVertexAttribNormalize(i,in->readBool()?GL_TRUE:GL_FALSE);
            
            // Read coords if valid
            bool coords_valid = in->readBool();
            if(coords_valid)
                setVertexAttribArray(i, in->readArray());
                
            // Read Indices if valid
            bool indices_valid = in->readBool();
            if(indices_valid)
                setVertexAttribIndices(i, static_cast<CRCore::IndexArray*>(in->readArray()));
        }

		setFastPathHint(in->readBool());

    }
    else{
        throw crException("crGeometry::read(): Expected crGeometry identification.");
    }
}
