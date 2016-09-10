/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
//Modified by Wucaihua
#include <CRCore/crNotify.h>
#include <CRCore/crUniform.h>
#include <CRCore/crProgram.h>
#include <CRCore/crStateSet.h>

#include <algorithm>

using namespace CRCore;

///////////////////////////////////////////////////////////////////////////
// CRCore::crUniform
///////////////////////////////////////////////////////////////////////////

crUniform::crUniform() :
    m_type(UNDEFINED), /*m_numElements(0),*/ m_modifiedCount(0)
{
	m_uniformData = new UniformData;
	m_uniformData_buf = m_uniformData;
	m_dataVariance = STATIC;
	//m_dataVariance = DYNAMIC;//默认DYNAMIC，需要做swapbuf，如果是STATIC则不需要
}


crUniform::crUniform( Type type, const std::string& name, int numElements ) :
    m_type(type),/* m_numElements(0),*/ m_modifiedCount(0)
{
	m_uniformData = new UniformData;
	m_uniformData_buf = m_uniformData;

    setName(name);
    setNumElements(numElements);
    m_dataVariance = STATIC;
	//allocateDataArray();
}

crUniform::crUniform( const crUniform& rhs, const crCopyOp& copyop ) :
    crBase(rhs,copyop), m_type(rhs.m_type),
    m_uniformData(new UniformData),
	m_uniformData_buf(m_uniformData)
{
	setDataVariance(rhs.getDataVariance());
	setNumElements(CRCore::maximum(rhs.getNumElements(),rhs.getBufNumElements()));
	//allocateDataArray();
    copyData( rhs );
}

void crUniform::setDataVariance(DataVariance dv) 
{ 
	if(m_dataVariance == dv) return;
	m_dataVariance = dv; 
	if(	m_dataVariance == STATIC )
        m_uniformData_buf = m_uniformData;
	else if(!m_uniformData_buf || m_uniformData_buf == m_uniformData)
	{
		m_uniformData_buf = new UniformData(*m_uniformData);
	}
}

crUniform::~crUniform()
{
	m_parents.clear();
}

void crUniform::addParent(CRCore::crStateSet* object)
{
    //CRCore::notify(CRCore::INFO)<<"crUniform Adding parent"<<std::endl;

    m_parents.push_back(object);
}

void crUniform::removeParent(CRCore::crStateSet* object)
{
    ParentList::iterator pitr = std::find(m_parents.begin(),m_parents.end(),object);
    if (pitr!=m_parents.end()) m_parents.erase(pitr);
}

bool crUniform::setType( Type t )
{
    if (m_type==t) return true;

    if( m_type != UNDEFINED )
    {
        CRCore::notify(CRCore::WARN) << "cannot change crUniform type" << std::endl;
        return false;
    }
    m_type = t;
    allocateDataArray();
    return true;
}

void crUniform::setName( const std::string& name )
{
    if( m_name != "" )
    {
        CRCore::notify(CRCore::WARN) << "cannot change crUniform name" << std::endl;
        return;
    }
    m_name = name;
}

void crUniform::setNumElements( unsigned int numElements )
{
    if( numElements < 1 )
    {
        CRCore::notify(CRCore::WARN) << "crUniform numElements < 1 is invalid" << std::endl;
        return;
    }

    if (numElements == m_uniformData->m_numElements) return;

    if( m_uniformData->m_numElements>0 )
    {
        CRCore::notify(CRCore::WARN) << "Warning: crUniform::setNumElements() cannot change crUniform numElements, size already fixed." << std::endl;
        return;
    }

    m_uniformData->m_numElements = numElements;
    allocateDataArray();
}

void crUniform::allocateDataArray()
{
    // if one array is already allocated, the job is done.
    if( m_uniformData->m_floatArray.valid() != m_uniformData->m_intArray.valid() ) return;

    // array cannot be created until m_type and m_numElements are specified
    int arrayNumElements = getInternalArrayNumElements();
    if( arrayNumElements )
    {
        switch( getInternalArrayType(getType()) )
        {
            case GL_FLOAT:
                m_uniformData->m_floatArray = new FloatArray(arrayNumElements);
                m_uniformData->m_intArray = 0;
                return;

            case GL_INT:
                m_uniformData->m_intArray = new IntArray(arrayNumElements);
                m_uniformData->m_floatArray = 0;
                return;

            default:
                break;
        }
    }
    m_uniformData->m_floatArray = 0;
    m_uniformData->m_intArray = 0;
}

bool crUniform::setArray( FloatArray* array )
{
    if( !array ) return false;

    // incoming array must match configuration of the crUniform
    if( getInternalArrayType(getType())!=GL_FLOAT || getInternalArrayNumElements()!=array->getNumElements() )
    {
        CRCore::notify(CRCore::WARN) << "crUniform::setArray : incompatible array" << std::endl;
        return false;
    }

    m_uniformData->m_floatArray = array;
    m_uniformData->m_intArray = 0;
    dirty();
    return true;
}

bool crUniform::setArray( IntArray* array )
{
    if( !array ) return false;

    // incoming array must match configuration of the crUniform
    if( getInternalArrayType(getType())!=GL_INT || getInternalArrayNumElements()!=array->getNumElements() )
    {
        CRCore::notify(CRCore::WARN) << "crUniform::setArray : incompatible array" << std::endl;
        return false;
    }

    m_uniformData->m_intArray = array;
    m_uniformData->m_floatArray = 0;
    dirty();
    return true;
}


///////////////////////////////////////////////////////////////////////////

int crUniform::compare(const crUniform& rhs) const
{
    if( this == &rhs ) return 0;
	//if(m_dataVariance == DYNAMIC) return -1;
	//if(rhs.m_dataVariance == DYNAMIC) return 1;
	if( m_dataVariance < rhs.m_dataVariance ) return -1;
	if( rhs.m_dataVariance < m_dataVariance ) return 1;

    if( m_type < rhs.m_type ) return -1;
    if( rhs.m_type < m_type ) return 1;

    if( m_uniformData->m_numElements < rhs.m_uniformData->m_numElements ) return -1;
    if( rhs.m_uniformData->m_numElements < m_uniformData->m_numElements ) return 1;

    if( m_name < rhs.m_name ) return -1;
    if( rhs.m_name < m_name ) return 1;

    return compareData( rhs );
}

int crUniform::compareData(const crUniform& rhs) const
{
    // caller must ensure that m_type==rhs.m_type

    if( m_uniformData->m_floatArray.valid() )
    {
        if( ! rhs.m_uniformData->m_floatArray ) return 1;
        if( m_uniformData->m_floatArray == rhs.m_uniformData->m_floatArray ) return 0;
        return memcmp( m_uniformData->m_floatArray->getDataPointer(), rhs.m_uniformData->m_floatArray->getDataPointer(),
            m_uniformData->m_floatArray->getTotalDataSize() );
    }

    if( m_uniformData->m_intArray.valid() )
    {
        if( ! rhs.m_uniformData->m_intArray ) return 1;
        if( m_uniformData->m_intArray == rhs.m_uniformData->m_intArray ) return 0;
        return memcmp( m_uniformData->m_intArray->getDataPointer(), rhs.m_uniformData->m_intArray->getDataPointer(),
            m_uniformData->m_intArray->getTotalDataSize() );
    }
    return -1;  // how got here?
}

void crUniform::copyData(const crUniform& rhs)
{
    // caller must ensure that m_type==rhs.m_type
    m_uniformData->m_numElements = rhs.m_uniformData->m_numElements;
    if( m_uniformData->m_floatArray.valid() && rhs.m_uniformData->m_floatArray.valid() ) *m_uniformData->m_floatArray = *rhs.m_uniformData->m_floatArray;
    if( m_uniformData->m_intArray.valid() && rhs.m_uniformData->m_intArray.valid() )     *m_uniformData->m_intArray = *rhs.m_uniformData->m_intArray;
    dirty();
}

bool crUniform::isCompatibleType( Type t ) const
{
    if( (t==UNDEFINED) || (getType()==UNDEFINED) ) return false;
    if( t == getType() ) return true;
    if( getGlApiType(t) == getGlApiType(getType()) ) return true;

    CRCore::notify(CRCore::WARN)
        << "Cannot assign between crUniform types " << getTypename(t)
        << " and " << getTypename(getType()) << std::endl;
    return false;
}

unsigned int crUniform::getInternalArrayNumElements() const
{
    if( getNumElements()<1 || getType()==UNDEFINED ) return 0;
    return getNumElements() * getTypeNumComponents(getType());
}


///////////////////////////////////////////////////////////////////////////
// static methods

const char* crUniform::getTypename( Type t )
{
	switch( t )
	{
	case FLOAT:             return "float";
	case FLOAT_VEC2:        return "vec2";
	case FLOAT_VEC3:        return "vec3";
	case FLOAT_VEC4:        return "vec4";
	case INT:               return "int";
	case INT_VEC2:          return "ivec2";
	case INT_VEC3:          return "ivec3";
	case INT_VEC4:          return "ivec4";
	case BOOL:              return "bool";
	case BOOL_VEC2:         return "bvec2";
	case BOOL_VEC3:         return "bvec3";
	case BOOL_VEC4:         return "bvec4";
	case FLOAT_MAT2:        return "mat2";
	case FLOAT_MAT3:        return "mat3";
	case FLOAT_MAT4:        return "mat4";
	case SAMPLER_1D:        return "sampler1D";
	case SAMPLER_2D:        return "sampler2D";
	case SAMPLER_1D_ARRAY:  return "sampler1DArray";
	case SAMPLER_2D_ARRAY:  return "sampler2DArray";
	case SAMPLER_3D:        return "sampler3D";
	case SAMPLER_CUBE:      return "samplerCube";
	case SAMPLER_1D_SHADOW: return "sampler1DShadow";
	case SAMPLER_2D_SHADOW: return "sampler2DShadow";
	case SAMPLER_1D_ARRAY_SHADOW: return "sampler1DArrayShadow";
	case SAMPLER_2D_ARRAY_SHADOW: return "sampler2DArrayShadow";
	case FLOAT_MAT2x3:  return "mat2x3";
	case FLOAT_MAT2x4:  return "mat2x4";
	case FLOAT_MAT3x2:  return "mat3x2";
	case FLOAT_MAT3x4:  return "mat3x4";
	case FLOAT_MAT4x2:  return "mat4x2";
	case FLOAT_MAT4x3:  return "mat4x3";
	case SAMPLER_BUFFER:       return "samplerBuffer";
	case SAMPLER_CUBE_SHADOW:  return "samplerCubeShadow";
	case UNSIGNED_INT_VEC2:    return "uvec2";
	case UNSIGNED_INT_VEC3:    return "uvec3";
	case UNSIGNED_INT_VEC4:    return "uvec4";
	case INT_SAMPLER_1D:       return "isampler1D";
	case INT_SAMPLER_2D:       return "isampler2D";
	case INT_SAMPLER_3D:       return "isampler3D";
	case INT_SAMPLER_CUBE:     return "isamplerCube";
	case INT_SAMPLER_2D_RECT:  return "isampler2DRect";
	case INT_SAMPLER_1D_ARRAY: return "isampler1DArray";
	case INT_SAMPLER_2D_ARRAY: return "isampler2DArray";
	case INT_SAMPLER_BUFFER:   return "isamplerBuffer";
	case UNSIGNED_INT_SAMPLER_1D:       return "usampler1D";
	case UNSIGNED_INT_SAMPLER_2D:       return "usampler2D";
	case UNSIGNED_INT_SAMPLER_3D:       return "usampler3D";
	case UNSIGNED_INT_SAMPLER_CUBE:     return "usamplerCube";
	case UNSIGNED_INT_SAMPLER_2D_RECT:  return "usampler2DRect";
	case UNSIGNED_INT_SAMPLER_1D_ARRAY: return "usampler1DArray";
	case UNSIGNED_INT_SAMPLER_2D_ARRAY: return "usampler2DArray";
	case UNSIGNED_INT_SAMPLER_BUFFER:   return "usamplerBuffer";
	default: return "UNDEFINED";
	}
}

int crUniform::getTypeNumComponents( Type t )
{
	switch( t )
	{
	case FLOAT:
	case INT:
	case BOOL:
	case SAMPLER_1D:
	case SAMPLER_2D:
	case SAMPLER_1D_ARRAY:
	case SAMPLER_2D_ARRAY:
	case SAMPLER_3D:
	case SAMPLER_CUBE:
	case SAMPLER_1D_SHADOW:
	case SAMPLER_2D_SHADOW:
	case SAMPLER_1D_ARRAY_SHADOW:
	case SAMPLER_2D_ARRAY_SHADOW:
	case SAMPLER_BUFFER:
	case SAMPLER_CUBE_SHADOW:
	case INT_SAMPLER_1D:
	case INT_SAMPLER_2D:
	case INT_SAMPLER_3D:
	case INT_SAMPLER_CUBE:
	case INT_SAMPLER_2D_RECT:
	case INT_SAMPLER_1D_ARRAY:
	case INT_SAMPLER_2D_ARRAY:
	case INT_SAMPLER_BUFFER:
	case UNSIGNED_INT_SAMPLER_1D:
	case UNSIGNED_INT_SAMPLER_2D:
	case UNSIGNED_INT_SAMPLER_3D:
	case UNSIGNED_INT_SAMPLER_CUBE:
	case UNSIGNED_INT_SAMPLER_2D_RECT:
	case UNSIGNED_INT_SAMPLER_1D_ARRAY:
	case UNSIGNED_INT_SAMPLER_2D_ARRAY:
	case UNSIGNED_INT_SAMPLER_BUFFER:
		return 1;

	case FLOAT_VEC2:
	case INT_VEC2:
	case BOOL_VEC2:
	case UNSIGNED_INT_VEC2:
		return 2;

	case FLOAT_VEC3:
	case INT_VEC3:
	case BOOL_VEC3:
	case UNSIGNED_INT_VEC3:
		return 3;

	case FLOAT_VEC4:
	case FLOAT_MAT2:
	case INT_VEC4:
	case BOOL_VEC4:
	case UNSIGNED_INT_VEC4:
		return 4;

	case FLOAT_MAT2x3:
	case FLOAT_MAT3x2:
		return 6;

	case FLOAT_MAT2x4:
	case FLOAT_MAT4x2:
		return 8;

	case FLOAT_MAT3:
		return 9;

	case FLOAT_MAT3x4:
	case FLOAT_MAT4x3:
		return 12;

	case FLOAT_MAT4:
		return 16;

	default:
		return 0;
	}
}

crUniform::Type crUniform::getTypeId( const std::string& tname )
{
	if( tname == "float" )           return FLOAT;
	if( tname == "vec2" )            return FLOAT_VEC2;
	if( tname == "vec3" )            return FLOAT_VEC3;
	if( tname == "vec4" )            return FLOAT_VEC4;
	if( tname == "int" )             return INT;
	if( tname == "ivec2" )           return INT_VEC2;
	if( tname == "ivec3" )           return INT_VEC3;
	if( tname == "ivec4" )           return INT_VEC4;
	if( tname == "bool" )            return BOOL;
	if( tname == "bvec2" )           return BOOL_VEC2;
	if( tname == "bvec3" )           return BOOL_VEC3;
	if( tname == "bvec4" )           return BOOL_VEC4;
	if( tname == "mat2" || tname == "mat2x2" ) return FLOAT_MAT2;
	if( tname == "mat3" || tname == "mat3x3" ) return FLOAT_MAT3;
	if( tname == "mat4" || tname == "mat4x4" ) return FLOAT_MAT4;
	if( tname == "sampler1D" )       return SAMPLER_1D;
	if( tname == "sampler2D" )       return SAMPLER_2D;
	if( tname == "sampler1DArray" )  return SAMPLER_1D_ARRAY;
	if( tname == "sampler2DArray" )  return SAMPLER_2D_ARRAY;
	if( tname == "sampler3D" )       return SAMPLER_3D;
	if( tname == "samplerCube" )     return SAMPLER_CUBE;
	if( tname == "sampler1DShadow" ) return SAMPLER_1D_SHADOW;
	if( tname == "sampler2DShadow" ) return SAMPLER_2D_SHADOW;
	if( tname == "sampler1DArrayShadow" ) return SAMPLER_1D_ARRAY_SHADOW;
	if( tname == "sampler2DArrayShadow" ) return SAMPLER_2D_ARRAY_SHADOW;
	if( tname == "mat2x3" )          return FLOAT_MAT2x3;
	if( tname == "mat2x4" )          return FLOAT_MAT2x4;
	if( tname == "mat3x2" )          return FLOAT_MAT3x2;
	if( tname == "mat3x4" )          return FLOAT_MAT3x4;
	if( tname == "mat4x2" )          return FLOAT_MAT4x2;
	if( tname == "mat4x3" )          return FLOAT_MAT4x3;
	if( tname == "samplerBuffer" )     return SAMPLER_BUFFER;
	if( tname == "samplerCubeShadow" ) return SAMPLER_CUBE_SHADOW;
	if( tname == "uvec2" )           return UNSIGNED_INT_VEC2;
	if( tname == "uvec3" )           return UNSIGNED_INT_VEC3;
	if( tname == "uvec4" )           return UNSIGNED_INT_VEC4;
	if( tname == "isampler1D" )      return INT_SAMPLER_1D;
	if( tname == "isampler2D" )      return INT_SAMPLER_2D;
	if( tname == "isampler3D" )      return INT_SAMPLER_3D;
	if( tname == "isamplerCube" )    return INT_SAMPLER_CUBE;
	if( tname == "isampler2DRect" )  return INT_SAMPLER_2D_RECT;
	if( tname == "isampler1DArray" ) return INT_SAMPLER_1D_ARRAY;
	if( tname == "isampler2DArray" ) return INT_SAMPLER_2D_ARRAY;
	if( tname == "isamplerBuffer" )  return INT_SAMPLER_BUFFER;
	if( tname == "usampler1D" )      return UNSIGNED_INT_SAMPLER_1D;
	if( tname == "usampler2D" )      return UNSIGNED_INT_SAMPLER_2D;
	if( tname == "usampler3D" )      return UNSIGNED_INT_SAMPLER_3D;
	if( tname == "usamplerCube" )    return UNSIGNED_INT_SAMPLER_CUBE;
	if( tname == "usampler2DRect" )  return UNSIGNED_INT_SAMPLER_2D_RECT;
	if( tname == "usampler1DArray" ) return UNSIGNED_INT_SAMPLER_1D_ARRAY;
	if( tname == "usampler2DArray" ) return UNSIGNED_INT_SAMPLER_2D_ARRAY;
	if( tname == "usamplerBuffer" )  return UNSIGNED_INT_SAMPLER_BUFFER;

	return UNDEFINED;
}

crUniform::Type crUniform::getGlApiType( Type t )
{
	switch( t )
	{
	case BOOL:
	case SAMPLER_1D:
	case SAMPLER_2D:
	case SAMPLER_1D_ARRAY:
	case SAMPLER_2D_ARRAY:
	case SAMPLER_3D:
	case SAMPLER_CUBE:
	case SAMPLER_1D_SHADOW:
	case SAMPLER_2D_SHADOW:
	case SAMPLER_1D_ARRAY_SHADOW:
	case SAMPLER_2D_ARRAY_SHADOW:
	case SAMPLER_BUFFER:
	case SAMPLER_CUBE_SHADOW:
	case INT_SAMPLER_1D:
	case INT_SAMPLER_2D:
	case INT_SAMPLER_3D:
	case INT_SAMPLER_CUBE:
	case INT_SAMPLER_2D_RECT:
	case INT_SAMPLER_1D_ARRAY:
	case INT_SAMPLER_2D_ARRAY:
	case INT_SAMPLER_BUFFER:
	case UNSIGNED_INT_SAMPLER_1D:
	case UNSIGNED_INT_SAMPLER_2D:
	case UNSIGNED_INT_SAMPLER_3D:
	case UNSIGNED_INT_SAMPLER_CUBE:
	case UNSIGNED_INT_SAMPLER_2D_RECT:
	case UNSIGNED_INT_SAMPLER_1D_ARRAY:
	case UNSIGNED_INT_SAMPLER_2D_ARRAY:
	case UNSIGNED_INT_SAMPLER_BUFFER:
		return INT;

	case BOOL_VEC2:
		return INT_VEC2;

	case BOOL_VEC3:
		return INT_VEC3;

	case BOOL_VEC4:
		return INT_VEC4;

	default:
		return t;
	}
}

GLenum crUniform::getInternalArrayType( Type t )
{
	switch( t )
	{
	case FLOAT:
	case FLOAT_VEC2:
	case FLOAT_VEC3:
	case FLOAT_VEC4:
	case FLOAT_MAT2:
	case FLOAT_MAT3:
	case FLOAT_MAT4:
	case FLOAT_MAT2x3:
	case FLOAT_MAT2x4:
	case FLOAT_MAT3x2:
	case FLOAT_MAT3x4:
	case FLOAT_MAT4x2:
	case FLOAT_MAT4x3:
		return GL_FLOAT;

	case INT:
	case INT_VEC2:
	case INT_VEC3:
	case INT_VEC4:
	case BOOL:
	case BOOL_VEC2:
	case BOOL_VEC3:
	case BOOL_VEC4:
	case SAMPLER_1D:
	case SAMPLER_2D:
	case SAMPLER_1D_ARRAY:
	case SAMPLER_2D_ARRAY:
	case SAMPLER_3D:
	case SAMPLER_CUBE:
	case SAMPLER_1D_SHADOW:
	case SAMPLER_2D_SHADOW:
	case SAMPLER_1D_ARRAY_SHADOW:
	case SAMPLER_2D_ARRAY_SHADOW:
	case SAMPLER_BUFFER:
	case SAMPLER_CUBE_SHADOW:
	case INT_SAMPLER_1D:
	case INT_SAMPLER_2D:
	case INT_SAMPLER_3D:
	case INT_SAMPLER_CUBE:
	case INT_SAMPLER_2D_RECT:
	case INT_SAMPLER_1D_ARRAY:
	case INT_SAMPLER_2D_ARRAY:
	case INT_SAMPLER_BUFFER:
	case UNSIGNED_INT_SAMPLER_1D:
	case UNSIGNED_INT_SAMPLER_2D:
	case UNSIGNED_INT_SAMPLER_3D:
	case UNSIGNED_INT_SAMPLER_CUBE:
	case UNSIGNED_INT_SAMPLER_2D_RECT:
	case UNSIGNED_INT_SAMPLER_1D_ARRAY:
	case UNSIGNED_INT_SAMPLER_2D_ARRAY:
	case UNSIGNED_INT_SAMPLER_BUFFER:
		return GL_INT;

		// TODO integrate new types
	case UNSIGNED_INT_VEC2:
	case UNSIGNED_INT_VEC3:
	case UNSIGNED_INT_VEC4:
	default:
		return 0;
	}
}


///////////////////////////////////////////////////////////////////////////
// value constructors for single-element (ie: non-array) uniforms

crUniform::crUniform( const char* name, float f ) :
    m_type(FLOAT), /*m_numElements(1),*/ m_modifiedCount(0)
{
    setName(name);
	m_dataVariance = STATIC;//默认DYNAMIC，需要做swapbuf，如果是STATIC则不需要
	m_uniformData = new UniformData(1);
	m_uniformData_buf = m_uniformData;
    allocateDataArray();
    set( f );
}

crUniform::crUniform( const char* name, const CRCore::crVector2& v2 ) :
    m_type(FLOAT_VEC2), /*m_numElements(1),*/ m_modifiedCount(0)
{
	m_uniformData = new UniformData(1);
	m_uniformData_buf = m_uniformData;
    setName(name);
	m_dataVariance = STATIC;//默认DYNAMIC，需要做swapbuf，如果是STATIC则不需要
    allocateDataArray();
    set( v2 );
}

crUniform::crUniform( const char* name, const CRCore::crVector3& v3 ) :
     m_type(FLOAT_VEC3), /*m_numElements(1),*/ m_modifiedCount(0)
{
	m_uniformData = new UniformData(1);
	m_uniformData_buf = m_uniformData;

    setName(name);
	m_dataVariance = STATIC;//默认DYNAMIC，需要做swapbuf，如果是STATIC则不需要
    allocateDataArray();
    set( v3 );
}

crUniform::crUniform( const char* name, const CRCore::crVector4& v4 ) :
    m_type(FLOAT_VEC4), /*m_numElements(1),*/ m_modifiedCount(0)
{
	m_uniformData = new UniformData(1);
	m_uniformData_buf = m_uniformData;

	setName(name);
	m_dataVariance = STATIC;//默认DYNAMIC，需要做swapbuf，如果是STATIC则不需要
    allocateDataArray();
    set( v4 );
}

crUniform::crUniform( const char* name, const CRCore::crMatrix2& m2 ) :
    m_type(FLOAT_MAT2), /*m_numElements(1),*/ m_modifiedCount(0)
{
	m_uniformData = new UniformData(1);
	m_uniformData_buf = m_uniformData;

    setName(name);
	m_dataVariance = STATIC;//默认DYNAMIC，需要做swapbuf，如果是STATIC则不需要
    allocateDataArray();
    set( m2 );
}

crUniform::crUniform( const char* name, const CRCore::crMatrix3& m3 ) :
    m_type(FLOAT_MAT3), /*m_numElements(1),*/ m_modifiedCount(0)
{
	m_uniformData = new UniformData(1);
	m_uniformData_buf = m_uniformData;

    setName(name);
	m_dataVariance = STATIC;//默认DYNAMIC，需要做swapbuf，如果是STATIC则不需要
    allocateDataArray();
    set( m3 );
}

crUniform::crUniform( const char* name, const CRCore::crMatrixf& m4 ) :
    m_type(FLOAT_MAT4), /*m_numElements(1),*/ m_modifiedCount(0)

{
	m_uniformData = new UniformData(1);
	m_uniformData_buf = m_uniformData;

    setName(name);
	m_dataVariance = STATIC;//默认DYNAMIC，需要做swapbuf，如果是STATIC则不需要
    allocateDataArray();
    set( m4 );
}

crUniform::crUniform( const char* name, const CRCore::crMatrixd& m4 ) :
    m_type(FLOAT_MAT4), /*m_numElements(1),*/ m_modifiedCount(0)
{
	m_uniformData = new UniformData(1);
	m_uniformData_buf = m_uniformData;

    setName(name);
	m_dataVariance = STATIC;//默认DYNAMIC，需要做swapbuf，如果是STATIC则不需要
    allocateDataArray();
    set( m4 );
}

crUniform::crUniform( const char* name, int i ) :
    m_type(INT), /*m_numElements(1),*/ m_modifiedCount(0)
{
	m_uniformData = new UniformData(1);
	m_uniformData_buf = m_uniformData;

    setName(name);
	m_dataVariance = STATIC;//默认DYNAMIC，需要做swapbuf，如果是STATIC则不需要
    allocateDataArray();
    set( i );
}

crUniform::crUniform( const char* name, int i0, int i1 ) :
    m_type(INT_VEC2), /*m_numElements(1),*/ m_modifiedCount(0)
{
	m_uniformData = new UniformData(1);
	m_uniformData_buf = m_uniformData;

    setName(name);
	m_dataVariance = STATIC;//默认DYNAMIC，需要做swapbuf，如果是STATIC则不需要
    allocateDataArray();
    set( i0, i1 );
}

crUniform::crUniform( const char* name, int i0, int i1, int i2 ) :
    m_type(INT_VEC3), /*m_numElements(1),*/ m_modifiedCount(0)
{
	m_uniformData = new UniformData(1);
	m_uniformData_buf = m_uniformData;

    setName(name);
	m_dataVariance = STATIC;//默认DYNAMIC，需要做swapbuf，如果是STATIC则不需要
    allocateDataArray();
    set( i0, i1, i2 );
}

crUniform::crUniform( const char* name, int i0, int i1, int i2, int i3 ) :
    m_type(INT_VEC4), /*m_numElements(1),*/ m_modifiedCount(0)
{
	m_uniformData = new UniformData(1);
	m_uniformData_buf = m_uniformData;

    setName(name);
	m_dataVariance = STATIC;//默认DYNAMIC，需要做swapbuf，如果是STATIC则不需要
    allocateDataArray();
    set( i0, i1, i2, i3 );
}

crUniform::crUniform( const char* name, bool b ) :
    m_type(BOOL), /*m_numElements(1),*/ m_modifiedCount(0)
{
	m_uniformData = new UniformData(1);
	m_uniformData_buf = m_uniformData;

    setName(name);
	m_dataVariance = STATIC;//默认DYNAMIC，需要做swapbuf，如果是STATIC则不需要
    allocateDataArray();
    set( b );
}

crUniform::crUniform( const char* name, bool b0, bool b1 ) :
     m_type(BOOL_VEC2), /*m_numElements(1),*/ m_modifiedCount(0)
{
	m_uniformData = new UniformData(1);
	m_uniformData_buf = m_uniformData;

    setName(name);
	m_dataVariance = STATIC;//默认DYNAMIC，需要做swapbuf，如果是STATIC则不需要
    allocateDataArray();
    set( b0, b1 );
}

crUniform::crUniform( const char* name, bool b0, bool b1, bool b2 ) :
    m_type(BOOL_VEC3), /*m_numElements(1),*/ m_modifiedCount(0)
{
	m_uniformData = new UniformData(1);
	m_uniformData_buf = m_uniformData;

    setName(name);
	m_dataVariance = STATIC;//默认DYNAMIC，需要做swapbuf，如果是STATIC则不需要
    allocateDataArray();
    set( b0, b1, b2 );
}

crUniform::crUniform( const char* name, bool b0, bool b1, bool b2, bool b3 ) :
    m_type(BOOL_VEC4), /*m_numElements(1),*/ m_modifiedCount(0)
{
	m_uniformData = new UniformData(1);
	m_uniformData_buf = m_uniformData;

    setName(name);
	m_dataVariance = STATIC;//默认DYNAMIC，需要做swapbuf，如果是STATIC则不需要
    allocateDataArray();
    set( b0, b1, b2, b3 );
}

///////////////////////////////////////////////////////////////////////////
// Value assignment for single-element (ie: non-array) uniforms.
// (For backwards compatability, if not already configured, set the
// crUniform's m_numElements=1)

bool crUniform::set( float f )
{
    if( getNumElements() == 0 ) setNumElements(1);
    return isScalar() ? setElement(0,f) : false;
}

bool crUniform::set( const CRCore::crVector2& v2 )
{
    if( getNumElements() == 0 ) setNumElements(1);
    return isScalar() ? setElement(0,v2) : false;
}

bool crUniform::set( const CRCore::crVector3& v3 )
{	
    if( getNumElements() == 0 ) setNumElements(1);
    return isScalar() ? setElement(0,v3) : false;
}

bool crUniform::set( const CRCore::crVector4& v4 )
{
    if( getNumElements() == 0 ) setNumElements(1);
    return isScalar() ? setElement(0,v4) : false;
}

bool crUniform::set( const CRCore::crMatrix2& m2 )
{
    if( getNumElements() == 0 ) setNumElements(1);
    return isScalar() ? setElement(0,m2) : false;
}

bool crUniform::set( const CRCore::crMatrix3& m3 )
{
    if( getNumElements() == 0 ) setNumElements(1);
    return isScalar() ? setElement(0,m3) : false;
}

bool crUniform::set( const CRCore::crMatrixf& m4 )
{
    if( getNumElements() == 0 ) setNumElements(1);
    return isScalar() ? setElement(0,m4) : false;
}

bool crUniform::set( const CRCore::crMatrixd& m4 )
{
    if( getNumElements() == 0 ) setNumElements(1);
    return isScalar() ? setElement(0,m4) : false;
}

bool crUniform::set( int i )
{
    if( getNumElements() == 0 ) setNumElements(1);
    return isScalar() ? setElement(0,i) : false;
}

bool crUniform::set( int i0, int i1 )
{
    if( getNumElements() == 0 ) setNumElements(1);
    return isScalar() ? setElement(0,i0,i1) : false;
}

bool crUniform::set( int i0, int i1, int i2 )
{
    if( getNumElements() == 0 ) setNumElements(1);
    return isScalar() ? setElement(0,i0,i1,i2) : false;
}

bool crUniform::set( int i0, int i1, int i2, int i3 )
{
    if( getNumElements() == 0 ) setNumElements(1);
    return isScalar() ? setElement(0,i0,i1,i2,i3) : false;
}

bool crUniform::set( bool b )
{
    if( getNumElements() == 0 ) setNumElements(1);
    return isScalar() ? setElement(0,b) : false;
}

bool crUniform::set( bool b0, bool b1 )
{
    if( getNumElements() == 0 ) setNumElements(1);
    return isScalar() ? setElement(0,b0,b1) : false;
}

bool crUniform::set( bool b0, bool b1, bool b2 )
{
    if( getNumElements() == 0 ) setNumElements(1);
    return isScalar() ? setElement(0,b0,b1,b2) : false;
}

bool crUniform::set( bool b0, bool b1, bool b2, bool b3 )
{
    if( getNumElements() == 0 ) setNumElements(1);
    return isScalar() ? setElement(0,b0,b1,b2,b3) : false;
}

///////////////////////////////////////////////////////////////////////////
// Value query for single-element (ie: non-array) uniforms.

bool crUniform::get( float& f ) const
{
    return isScalar() ? getElement(0,f) : false;
}

bool crUniform::get( CRCore::crVector2& v2 ) const
{
    return isScalar() ? getElement(0,v2) : false;
}

bool crUniform::get( CRCore::crVector3& v3 ) const
{
    return isScalar() ? getElement(0,v3) : false;
}

bool crUniform::get( CRCore::crVector4& v4 ) const
{
    return isScalar() ? getElement(0,v4) : false;
}

bool crUniform::get( CRCore::crMatrix2& m2 ) const
{
    return isScalar() ? getElement(0,m2) : false;
}

bool crUniform::get( CRCore::crMatrix3& m3 ) const
{
    return isScalar() ? getElement(0,m3) : false;
}

bool crUniform::get( CRCore::crMatrixf& m4 ) const
{
    return isScalar() ? getElement(0,m4) : false;
}

bool crUniform::get( CRCore::crMatrixd& m4 ) const
{
    return isScalar() ? getElement(0,m4) : false;
}

bool crUniform::get( int& i ) const
{
    return isScalar() ? getElement(0,i) : false;
}

bool crUniform::get( int& i0, int& i1 ) const
{
    return isScalar() ? getElement(0,i0,i1) : false;
}

bool crUniform::get( int& i0, int& i1, int& i2 ) const
{
    return isScalar() ? getElement(0,i0,i1,i2) : false;
}

bool crUniform::get( int& i0, int& i1, int& i2, int& i3 ) const
{
    return isScalar() ? getElement(0,i0,i1,i2,i3) : false;
}

bool crUniform::get( bool& b ) const
{
    return isScalar() ? getElement(0,b) : false;
}

bool crUniform::get( bool& b0, bool& b1 ) const
{
    return isScalar() ? getElement(0,b0,b1) : false;
}

bool crUniform::get( bool& b0, bool& b1, bool& b2 ) const
{
    return isScalar() ? getElement(0,b0,b1,b2) : false;
}

bool crUniform::get( bool& b0, bool& b1, bool& b2, bool& b3 ) const
{
    return isScalar() ? getElement(0,b0,b1,b2,b3) : false;
}

///////////////////////////////////////////////////////////////////////////
// Value assignment for array uniforms.

bool crUniform::setElement( unsigned int index, float f )
{
    if( index>=getNumElements() || !isCompatibleType(FLOAT) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    (*m_uniformData->m_floatArray)[j] = f;
    dirty();
    return true;
}

bool crUniform::setElement( unsigned int index, const CRCore::crVector2& v2 )
{
    if( index>=getNumElements() || !isCompatibleType(FLOAT_VEC2) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    (*m_uniformData->m_floatArray)[j] = v2.x();
    (*m_uniformData->m_floatArray)[j+1] = v2.y();
    dirty();
    return true;
}

bool crUniform::setElement( unsigned int index, const CRCore::crVector3& v3 )
{
    if( index>=getNumElements() || !isCompatibleType(FLOAT_VEC3) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    (*m_uniformData->m_floatArray)[j] = v3.x();
    (*m_uniformData->m_floatArray)[j+1] = v3.y();
    (*m_uniformData->m_floatArray)[j+2] = v3.z();
    dirty();

    return true;
}

bool crUniform::setElement( unsigned int index, const CRCore::crVector4& v4 )
{
    if( index>=getNumElements() || !isCompatibleType(FLOAT_VEC4) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    (*m_uniformData->m_floatArray)[j] = v4.x();
    (*m_uniformData->m_floatArray)[j+1] = v4.y();
    (*m_uniformData->m_floatArray)[j+2] = v4.z();
    (*m_uniformData->m_floatArray)[j+3] = v4.w();
    dirty();
    return true;
}

bool crUniform::setElement( unsigned int index, const CRCore::crMatrix2& m2 )
{
    if( index>=getNumElements() || !isCompatibleType(FLOAT_MAT2) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    for( int i = 0; i < 4; ++i ) (*m_uniformData->m_floatArray)[j+i] = m2[i];
    dirty();
    return true;
}

bool crUniform::setElement( unsigned int index, const CRCore::crMatrix3& m3 )
{
    if( index>=getNumElements() || !isCompatibleType(FLOAT_MAT3) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    for( int i = 0; i < 9; ++i ) (*m_uniformData->m_floatArray)[j+i] = m3[i];
    dirty();
    return true;
}

bool crUniform::setElement( unsigned int index, const CRCore::crMatrixf& m4 )
{
    if( index>=getNumElements() || !isCompatibleType(FLOAT_MAT4) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    const crMatrixf::value_type* p = m4.ptr();
    for( int i = 0; i < 16; ++i ) (*m_uniformData->m_floatArray)[j+i] = p[i];
    dirty();
    return true;
}

bool crUniform::setElement( unsigned int index, const CRCore::crMatrixd& m4 )
{
    if( index>=getNumElements() || !isCompatibleType(FLOAT_MAT4) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    const crMatrixd::value_type* p = m4.ptr();
    for( int i = 0; i < 16; ++i ) (*m_uniformData->m_floatArray)[j+i] = static_cast<float>(p[i]);
    dirty();
    return true;
}

bool crUniform::setElement( unsigned int index, int i )
{
    if( index>=getNumElements() || !isCompatibleType(INT) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    (*m_uniformData->m_intArray)[j] = i;
    dirty();
    return true;
}

bool crUniform::setElement( unsigned int index, int i0, int i1 )
{
    if( index>=getNumElements() || !isCompatibleType(INT_VEC2) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    (*m_uniformData->m_intArray)[j] = i0;
    (*m_uniformData->m_intArray)[j+1] = i1;
    dirty();
    return true;
}

bool crUniform::setElement( unsigned int index, int i0, int i1, int i2 )
{
    if( index>=getNumElements() || !isCompatibleType(INT_VEC3) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    (*m_uniformData->m_intArray)[j] = i0;
    (*m_uniformData->m_intArray)[j+1] = i1;
    (*m_uniformData->m_intArray)[j+2] = i2;
    dirty();
    return true;
}

bool crUniform::setElement( unsigned int index, int i0, int i1, int i2, int i3 )
{
    if( index>=getNumElements() || !isCompatibleType(INT_VEC4) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    (*m_uniformData->m_intArray)[j] = i0;
    (*m_uniformData->m_intArray)[j+1] = i1;
    (*m_uniformData->m_intArray)[j+2] = i2;
    (*m_uniformData->m_intArray)[j+3] = i3;
    dirty();
    return true;
}

bool crUniform::setElement( unsigned int index, bool b )
{
    if( index>=getNumElements() || !isCompatibleType(BOOL) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    (*m_uniformData->m_intArray)[j] = b;
    dirty();
    return true;
}

bool crUniform::setElement( unsigned int index, bool b0, bool b1 )
{
    if( index>=getNumElements() || !isCompatibleType(BOOL_VEC2) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    (*m_uniformData->m_intArray)[j] = b0;
    (*m_uniformData->m_intArray)[j+1] = b1;
    dirty();
    return true;
}

bool crUniform::setElement( unsigned int index, bool b0, bool b1, bool b2 )
{
    if( index>=getNumElements() || !isCompatibleType(BOOL_VEC3) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    (*m_uniformData->m_intArray)[j] = b0;
    (*m_uniformData->m_intArray)[j+1] = b1;
    (*m_uniformData->m_intArray)[j+2] = b2;
    dirty();
    return true;
}

bool crUniform::setElement( unsigned int index, bool b0, bool b1, bool b2, bool b3 )
{
    if( index>=getNumElements() || !isCompatibleType(BOOL_VEC4) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    (*m_uniformData->m_intArray)[j] = b0;
    (*m_uniformData->m_intArray)[j+1] = b1;
    (*m_uniformData->m_intArray)[j+2] = b2;
    (*m_uniformData->m_intArray)[j+3] = b3;
    dirty();
    return true;
}

///////////////////////////////////////////////////////////////////////////
// Value query for array uniforms.

bool crUniform::getElement( unsigned int index, float& f ) const
{
    if( index>=getNumElements() || !isCompatibleType(FLOAT) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    f = (*m_uniformData->m_floatArray)[j];
    return true;
}

bool crUniform::getElement( unsigned int index, CRCore::crVector2& v2 ) const
{
    if( index>=getNumElements() || !isCompatibleType(FLOAT_VEC2) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    v2.x() = (*m_uniformData->m_floatArray)[j];
    v2.y() = (*m_uniformData->m_floatArray)[j+1];
    return true;
}

bool crUniform::getElement( unsigned int index, CRCore::crVector3& v3 ) const
{
    if( index>=getNumElements() || !isCompatibleType(FLOAT_VEC3) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    v3.x() = (*m_uniformData->m_floatArray)[j];
    v3.y() = (*m_uniformData->m_floatArray)[j+1];
    v3.z() = (*m_uniformData->m_floatArray)[j+2];
    return true;
}

bool crUniform::getElement( unsigned int index, CRCore::crVector4& v4 ) const
{
    if( index>=getNumElements() || !isCompatibleType(FLOAT_VEC4) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    v4.x() = (*m_uniformData->m_floatArray)[j];
    v4.y() = (*m_uniformData->m_floatArray)[j+1];
    v4.z() = (*m_uniformData->m_floatArray)[j+2];
    v4.w() = (*m_uniformData->m_floatArray)[j+3];
    return true;
}

bool crUniform::getElement( unsigned int index, CRCore::crMatrix2& m2 ) const
{
    if( index>=getNumElements() || !isCompatibleType(FLOAT_MAT2) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    m2.set( &((*m_uniformData->m_floatArray)[j]) );
    return true;
}

bool crUniform::getElement( unsigned int index, CRCore::crMatrix3& m3 ) const
{
    if( index>=getNumElements() || !isCompatibleType(FLOAT_MAT3) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    m3.set( &((*m_uniformData->m_floatArray)[j]) );
    return true;
}

bool crUniform::getElement( unsigned int index, CRCore::crMatrixf& m4 ) const
{
    if( index>=getNumElements() || !isCompatibleType(FLOAT_MAT4) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    m4.set( &((*m_uniformData->m_floatArray)[j]) );
    return true;
}

bool crUniform::getElement( unsigned int index, CRCore::crMatrixd& m4 ) const
{
    if( index>=getNumElements() || !isCompatibleType(FLOAT_MAT4) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    m4.set( &((*m_uniformData->m_floatArray)[j]) );
    return true;
}

bool crUniform::getElement( unsigned int index, int& i ) const
{
    if( index>=getNumElements() || !isCompatibleType(INT) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    i = (*m_uniformData->m_intArray)[j];
    return true;
}

bool crUniform::getElement( unsigned int index, int& i0, int& i1 ) const
{
    if( index>=getNumElements() || !isCompatibleType(INT_VEC2) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    i0 = (*m_uniformData->m_intArray)[j];
    i1 = (*m_uniformData->m_intArray)[j+1];
    return true;
}

bool crUniform::getElement( unsigned int index, int& i0, int& i1, int& i2 ) const
{
    if( index>=getNumElements() || !isCompatibleType(INT_VEC3) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    i0 = (*m_uniformData->m_intArray)[j];
    i1 = (*m_uniformData->m_intArray)[j+1];
    i2 = (*m_uniformData->m_intArray)[j+2];
    return true;
}

bool crUniform::getElement( unsigned int index, int& i0, int& i1, int& i2, int& i3 ) const
{
    if( index>=getNumElements() || !isCompatibleType(INT_VEC4) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    i0 = (*m_uniformData->m_intArray)[j];
    i1 = (*m_uniformData->m_intArray)[j+1];
    i2 = (*m_uniformData->m_intArray)[j+2];
    i3 = (*m_uniformData->m_intArray)[j+3];
    return true;
}

bool crUniform::getElement( unsigned int index, bool& b ) const
{
    if( index>=getNumElements() || !isCompatibleType(BOOL) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    b = ((*m_uniformData->m_intArray)[j] != 0);
    return true;
}

bool crUniform::getElement( unsigned int index, bool& b0, bool& b1 ) const
{
    if( index>=getNumElements() || !isCompatibleType(BOOL_VEC2) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    b0 = ((*m_uniformData->m_intArray)[j] != 0);
    b1 = ((*m_uniformData->m_intArray)[j+1] != 0);
    return true;
}

bool crUniform::getElement( unsigned int index, bool& b0, bool& b1, bool& b2 ) const
{
    if( index>=getNumElements() || !isCompatibleType(BOOL_VEC3) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    b0 = ((*m_uniformData->m_intArray)[j] != 0);
    b1 = ((*m_uniformData->m_intArray)[j+1] != 0);
    b2 = ((*m_uniformData->m_intArray)[j+2] != 0);
    return true;
}

bool crUniform::getElement( unsigned int index, bool& b0, bool& b1, bool& b2, bool& b3 ) const
{
    if( index>=getNumElements() || !isCompatibleType(BOOL_VEC4) ) return false;
    unsigned int j = index * getTypeNumComponents(getType());
    b0 = ((*m_uniformData->m_intArray)[j] != 0);
    b1 = ((*m_uniformData->m_intArray)[j+1] != 0);
    b2 = ((*m_uniformData->m_intArray)[j+2] != 0);
    b3 = ((*m_uniformData->m_intArray)[j+3] != 0);
    return true;
}

///////////////////////////////////////////////////////////////////////////

void crUniform::apply(const GL2Extensions* ext, GLint location) const
{
	//if(m_name.compare("bias") == 0)
	//	CRCore::notify(CRCore::ALWAYS)<<"crUniform::apply bias "<<getBufNumElements()<<std::endl;
	//else if(m_name.compare("lightViewInverse") == 0)
	//	CRCore::notify(CRCore::ALWAYS)<<"crUniform::apply lightViewInverse "<<getBufNumElements()<<std::endl;

    GLsizei num = getBufNumElements();
    if( num < 1 ) return;

    switch( getGlApiType(getType()) )
    {
    case FLOAT:
        if( m_uniformData_buf->m_floatArray.valid() ) 
		{
			if(num > 1)
                ext->glUniform1fv( location, num, &m_uniformData_buf->m_floatArray->front() );
			else
                ext->glUniform1f( location, (*m_uniformData_buf->m_floatArray)[0]);
		}
        break;

    case FLOAT_VEC2:
        if( m_uniformData_buf->m_floatArray.valid() ) 
		{
			if(num > 1)
			    ext->glUniform2fv( location, num, &m_uniformData_buf->m_floatArray->front() );
			else
				ext->glUniform2f( location, (*m_uniformData_buf->m_floatArray)[0], (*m_uniformData_buf->m_floatArray)[1] );
		}
        break;

    case FLOAT_VEC3:
        if( m_uniformData_buf->m_floatArray.valid() ) 
		{
			if(num > 1)
				ext->glUniform3fv( location, num, &m_uniformData_buf->m_floatArray->front() );
			else
				ext->glUniform3f( location, (*m_uniformData_buf->m_floatArray)[0], (*m_uniformData_buf->m_floatArray)[1], (*m_uniformData_buf->m_floatArray)[2] );
		}
        break;

    case FLOAT_VEC4:
        if( m_uniformData_buf->m_floatArray.valid() )
		{
			if(num > 1)
				ext->glUniform4fv( location, num, &m_uniformData_buf->m_floatArray->front() );
			else
				ext->glUniform4f( location, (*m_uniformData_buf->m_floatArray)[0], (*m_uniformData_buf->m_floatArray)[1], (*m_uniformData_buf->m_floatArray)[2], (*m_uniformData_buf->m_floatArray)[3] );
		}
        break;

    case FLOAT_MAT2:
        if( m_uniformData_buf->m_floatArray.valid() ) ext->glUniformMatrix2fv( location, num, GL_FALSE, &m_uniformData_buf->m_floatArray->front() );
        break;

    case FLOAT_MAT3:
        if( m_uniformData_buf->m_floatArray.valid() ) ext->glUniformMatrix3fv( location, num, GL_FALSE, &m_uniformData_buf->m_floatArray->front() );
        break;

    case FLOAT_MAT4:
        if( m_uniformData_buf->m_floatArray.valid() ) ext->glUniformMatrix4fv( location, num, GL_FALSE, &m_uniformData_buf->m_floatArray->front() );
        break;

    case INT:
        if( m_uniformData_buf->m_intArray.valid() ) 
		{
			if(num > 1)
				ext->glUniform1iv( location, num, &m_uniformData_buf->m_intArray->front() );
			else
				ext->glUniform1i( location, (*m_uniformData_buf->m_intArray)[0] );
		}
		//if( m_uniformData_buf->m_intArray.valid() ) ext->glUniform1i( location, m_uniformData_buf->m_intArray->front() );
        break;

    case INT_VEC2:
        if( m_uniformData_buf->m_intArray.valid() )
		{
			if(num > 1)
				ext->glUniform2iv( location, num, &m_uniformData_buf->m_intArray->front() );
			else
				ext->glUniform2i( location, (*m_uniformData_buf->m_intArray)[0], (*m_uniformData_buf->m_intArray)[1] );
		}
        break;

    case INT_VEC3:
        if( m_uniformData_buf->m_intArray.valid() )
		{
			if(num > 1)
				ext->glUniform3iv( location, num, &m_uniformData_buf->m_intArray->front() );
			else
				ext->glUniform3i( location, (*m_uniformData_buf->m_intArray)[0], (*m_uniformData_buf->m_intArray)[1], (*m_uniformData_buf->m_intArray)[2] );
		}
        break;

    case INT_VEC4:
        if( m_uniformData_buf->m_intArray.valid() ) 
		{
			if(num > 1)
				ext->glUniform4iv( location, num, &m_uniformData_buf->m_intArray->front() );
			else
				ext->glUniform4i( location, (*m_uniformData_buf->m_intArray)[0], (*m_uniformData_buf->m_intArray)[1], (*m_uniformData_buf->m_intArray)[2], (*m_uniformData_buf->m_intArray)[3] );
		}
        break;

    default:
        CRCore::notify(CRCore::FATAL) << "how got here? " __FILE__ ":" << __LINE__ << std::endl;
        break;
    }
}

void crUniform::setUpdateCallback(Callback* uc)
{
    //CRCore::notify(CRCore::INFO)<<"crUniform::Setting Update callbacks"<<std::endl;

    if (m_updateCallback==uc) return;
    
    int delta = 0;
    if (m_updateCallback.valid()) --delta;
    if (uc) ++delta;

    m_updateCallback = uc;
    
    if (delta!=0)
    {
        //CRCore::notify(CRCore::INFO)<<"Going to set crUniform parents"<<std::endl;

        for(ParentList::iterator itr=m_parents.begin();
            itr!=m_parents.end();
            ++itr)
        {
            //CRCore::notify(CRCore::INFO)<<"   setting crUniform parent"<<std::endl;
            (*itr)->setNumChildrenRequiringUpdateTraversal((*itr)->getNumChildrenRequiringUpdateTraversal()+delta);
        }
    }
}

//void crUniform::setEventCallback(Callback* ec)
//{
//    //CRCore::notify(CRCore::INFO)<<"crUniform::Setting Event callbacks"<<std::endl;
//
//    if (m_eventCallback==ec) return;
//    
//    int delta = 0;
//    if (m_eventCallback.valid()) --delta;
//    if (ec) ++delta;
//
//    m_eventCallback = ec;
//    
//    if (delta!=0)
//    {
//        for(ParentList::iterator itr=m_parents.begin();
//            itr!=m_parents.end();
//            ++itr)
//        {            
//            (*itr)->setNumChildrenRequiringEventTraversal((*itr)->getNumChildrenRequiringEventTraversal()+delta);
//        }
//    }
//}

/*EOF*/

