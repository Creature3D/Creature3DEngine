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
#include <Plugin cre/crUniform.h>
#include <Plugin cre/crBase.h>
#include <CRCore/crNotify.h>

using namespace cre;

void crUniform::write(crDataOutputStream* out){
    // Write crUniform's identification.
    out->writeInt(CREUNIFORM);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
    if(obj)
    {
        ((cre::crBase*)(obj))->write(out);
    }
    else
        throw crException("crUniform::write(): Could not cast this CRCore::crUniform to an CRCore::crBase.");

    out->writeInt(getType());
    out->writeString(getName());

    switch( crUniform::getGlApiType(getType()) )
    {
        case(CRCore::crUniform::FLOAT):
        {
            float value;
            get(value);
            out->writeFloat(value);
            break;
        }
        case(CRCore::crUniform::FLOAT_VEC2):
        {
            CRCore::crVector2f value;
            get(value);
            out->writeVec2(value);
            break;
        }
        case(CRCore::crUniform::FLOAT_VEC3):
        {
            CRCore::crVector3f value;
            get(value);
            out->writeVec3(value);
            break;
        }
        case(CRCore::crUniform::FLOAT_VEC4):
        {
            CRCore::crVector4f value;
            get(value);
            out->writeVec4(value);
            break;
        }
        case(CRCore::crUniform::INT):
        {
            int i0;
            get(i0);
            out->writeInt(i0);
            break;
        }
        case(CRCore::crUniform::INT_VEC2):
        {
            int i0, i1;
            get(i0, i1);
            out->writeInt(i0);
            out->writeInt(i1);
            break;
        }
        case(CRCore::crUniform::INT_VEC3):
        {
            int i0, i1, i2;
            get(i0, i1, i2);
            out->writeInt(i0);
            out->writeInt(i1);
            out->writeInt(i2);
            break;
        }
        case(CRCore::crUniform::INT_VEC4):
        {
            int i0, i1, i2, i3;
            get(i0, i1, i2, i3);
            out->writeInt(i0);
            out->writeInt(i1);
            out->writeInt(i2);
            out->writeInt(i3);
            break;
        }
        case(CRCore::crUniform::FLOAT_MAT2):
        {
            CRCore::notify(CRCore::WARN)<<"Warning : type mat2 not supported for reading."<<std::endl;
            break;
        }
        case(CRCore::crUniform::FLOAT_MAT3):
        {
            CRCore::notify(CRCore::WARN)<<"Warning : type mat3 not supported for reading."<<std::endl;
            break;
        }
        case(CRCore::crUniform::FLOAT_MAT4):
        {
            CRCore::crMatrixf matrix;
            get(matrix);
            out->writeMatrixf(matrix);
            break;
        }
        default:
        {
            CRCore::notify(CRCore::WARN)<<"Warning : uniform "<<getType()<<"type not supported for reading."<<std::endl;
            break;
        }
    }
}

void crUniform::read(crDataInputStream* in)
{
    // Read crUniform's identification.
    int id = in->peekInt();
    if(id == CREUNIFORM)
    {
        // Read crUniform's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
        if(obj)
        {
            ((cre::crBase*)(obj))->read(in);
        }
        else
            throw crException("crUniform::read(): Could not cast this CRCore::crUniform to an CRCore::crBase.");

    }
    else
    {
        throw crException("crUniform::read(): Expected crUniform identification.");
    }
    
    setType(static_cast<Type>(in->readInt()));
    setName(in->readString());
    
    switch( crUniform::getGlApiType(getType()) )
    {
        case(CRCore::crUniform::FLOAT):
        {
            set(in->readFloat());
            break;
        }
        case(CRCore::crUniform::FLOAT_VEC2):
        {
            set(in->readVec2());
            break;
        }
        case(CRCore::crUniform::FLOAT_VEC3):
        {
            set(in->readVec3());
            break;
        }
        case(CRCore::crUniform::FLOAT_VEC4):
        {
            set(in->readVec4());
            break;
        }
        case(CRCore::crUniform::INT):
        {
            set(in->readInt());
            break;
        }
        case(CRCore::crUniform::INT_VEC2):
        {
            int i0 = in->readInt();
            int i1 = in->readInt();
            set(i0,i1);
            break;
        }
        case(CRCore::crUniform::INT_VEC3):
        {
            int i0 = in->readInt();
            int i1 = in->readInt();
            int i2 = in->readInt();
            set(i0,i1,i2);
            break;
        }
        case(CRCore::crUniform::INT_VEC4):
        {
            int i0 = in->readInt();
            int i1 = in->readInt();
            int i2 = in->readInt();
            int i3 = in->readInt();
            set(i0,i1,i2,i3);
            break;
        }
        case(CRCore::crUniform::FLOAT_MAT2):
        {
            CRCore::notify(CRCore::WARN)<<"Warning : type mat2 not supported for reading."<<std::endl;
            break;
        }
        case(CRCore::crUniform::FLOAT_MAT3):
        {
            CRCore::notify(CRCore::WARN)<<"Warning : type mat3 not supported for reading."<<std::endl;
            break;
        }
        case(CRCore::crUniform::FLOAT_MAT4):
        {
            set( in->readMatrixf() );
            break;
        }
        default:
        {
            CRCore::notify(CRCore::WARN)<<"Warning : uniform "<<getType()<<"type not supported for reading."<<std::endl;
            break;
        }
    }

}
