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

#ifndef CRCORE_CRARRAY_H
#define CRCORE_CRARRAY_H 1

#include <CRCore/crBase.h>
#include <crgl/gl.h>
#include <CRCore/crVector2.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>

#include <vector>

namespace CRCore {

class ArrayVisitor;
class ConstArrayVisitor;

class ValueVisitor;
class ConstValueVisitor;

#define VR_BYTE                           0x1400
#define VR_UNSIGNED_BYTE                  0x1401
#define VR_SHORT                          0x1402
#define VR_UNSIGNED_SHORT                 0x1403
#define VR_INT                            0x1404
#define VR_UNSIGNED_INT                   0x1405
#define VR_FLOAT                          0x1406
#define VR_2_BYTES                        0x1407
#define VR_3_BYTES                        0x1408
#define VR_4_BYTES                        0x1409
#define VR_DOUBLE                         0x140A

class CR_EXPORT Array : public crBase
{

    public:
    
        enum Type
        {
            ArrayType = 0,
            ByteArrayType     = 1,
            ShortArrayType    = 2,
            IntArrayType      = 3,
            UByteArrayType    = 4,
            UShortArrayType   = 5,
            UIntArrayType     = 6,
            Vec4ubArrayType   = 7,
            FloatArrayType    = 8,
            Vec2ArrayType     = 9,
            Vec3ArrayType     = 10,
            Vec4ArrayType     = 11,
			Vec2sArrayType    = 12,
			Vec3sArrayType    = 13,
			Vec4sArrayType    = 14,
			Vec2bArrayType    = 15,
			Vec3bArrayType    = 16,
			Vec4bArrayType    = 17
        };

        Array(Type arrayType=ArrayType,int dataSize=0,unsigned int dataType=0):
            m_arrayType(arrayType),
            m_dataSize(dataSize),
            m_dataType(dataType) {}
    
        Array(const Array& array,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crBase(array,copyop),
            m_arrayType(array.m_arrayType),
            m_dataSize(array.m_dataSize),
            m_dataType(array.m_dataType) {}

        virtual bool isSameKindAs(const crBase* obj) const { return dynamic_cast<const Array*>(obj)!=NULL; }
        virtual const char* libraryName() const { return "CRCore"; }
        virtual const char* className() const;
        
        virtual void accept(ArrayVisitor&) = 0;
        virtual void accept(ConstArrayVisitor&) const = 0;

        virtual void accept(unsigned int index,ValueVisitor&) = 0;
        virtual void accept(unsigned int index,ConstValueVisitor&) const = 0;

        /** return -1 if lhs element is less than rhs element, 0 is equal, 1 if lhs element is greater than rhs element.*/
        virtual int compare(unsigned int lhs,unsigned int rhs) const = 0;
		virtual int compare(unsigned int lhs,unsigned int rhs) = 0;

        Type                    getType() const { return m_arrayType; }
        int                     getDataSize() const { return m_dataSize; }
        unsigned int            getDataType() const { return m_dataType; }
        virtual const void*     getDataPointer() const = 0;
        virtual unsigned int    getTotalDataSize() const = 0;
        virtual unsigned int    getNumElements() const = 0;
		virtual int             getStride()const = 0;

		Type                    getType() { return m_arrayType; }
		int                     getDataSize() { return m_dataSize; }
		unsigned int            getDataType() { return m_dataType; }
		virtual const void*     getDataPointer() = 0;
		virtual unsigned int    getTotalDataSize() = 0;
		virtual unsigned int    getNumElements() = 0;

		virtual ~Array() {}

    protected:

        Type        m_arrayType;
        int         m_dataSize;
        unsigned int      m_dataType;
};

template<typename T, Array::Type ARRAYTYPE, int DataSize, int DataType>
class TemplateArray : public Array, public std::vector<T>
{
    public:
    
        TemplateArray() : Array(ARRAYTYPE,DataSize,DataType) {}

        TemplateArray(const TemplateArray& ta,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            Array(ta,copyop),
            std::vector<T>(ta) {}
        
        TemplateArray(unsigned int no) : 
            Array(ARRAYTYPE,DataSize,DataType),
            std::vector<T>(no) {}

        TemplateArray(unsigned int no,T* ptr) : 
            Array(ARRAYTYPE,DataSize,DataType),
            std::vector<T>(ptr,ptr+no) {}

        template <class InputIterator>
        TemplateArray(InputIterator first,InputIterator last) : 
            Array(ARRAYTYPE,DataSize,DataType),
            std::vector<T>(first,last) {}        

        TemplateArray& operator = (const TemplateArray& array)
        {
            if (this==&array) return *this;
            assign(array.begin(),array.end());
            return *this;
        } 

        virtual crBase* cloneType() const { return new TemplateArray(); }
        virtual crBase* clone(const crCopyOp& copyop) const { return new TemplateArray(*this,copyop); }

		virtual crBase* cloneType() { return new TemplateArray(); }
		virtual crBase* clone(const crCopyOp& copyop) { return new TemplateArray(*this,copyop); }        

        virtual void accept(ArrayVisitor& av) { av.apply(*this); }
        virtual void accept(ConstArrayVisitor& av) const { av.apply(*this); }

        virtual void accept(unsigned int index,ValueVisitor& vv) { vv.apply( (*this)[index] ); }
        virtual void accept(unsigned int index,ConstValueVisitor& vv) const {  vv.apply( (*this)[index] );}

        virtual int compare(unsigned int lhs,unsigned int rhs) const
        {
            const T& elem_lhs = (*this)[lhs];
            const T& elem_rhs = (*this)[rhs];
            if (elem_lhs<elem_rhs) return -1;
            if (elem_rhs<elem_lhs) return 1;
            return 0;
        }

		virtual int compare(unsigned int lhs,unsigned int rhs)
		{
			const T& elem_lhs = (*this)[lhs];
			const T& elem_rhs = (*this)[rhs];
			if (elem_lhs<elem_rhs) return -1;
			if (elem_rhs<elem_lhs) return 1;
			return 0;
		}      

        virtual const void*     getDataPointer() const { if (!empty()) return &front(); else return 0; }
        virtual unsigned int    getTotalDataSize() const { return size()*sizeof(T); }
        virtual unsigned int    getNumElements() const { return size(); }
		virtual int             getStride()const { return sizeof(T); }

		virtual const void*     getDataPointer() { if (!empty()) return &front(); else return 0; };
		virtual unsigned int    getTotalDataSize(){ return size()*sizeof(T); };
		virtual unsigned int    getNumElements(){ return size(); };

        virtual ~TemplateArray() {}
};

class CR_EXPORT IndexArray : public Array
{

    public:

        IndexArray(Type arrayType=ArrayType,int dataSize=0,unsigned int dataType=0):
            Array(arrayType,dataSize,dataType) {}
    
        IndexArray(const Array& array,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            Array(array,copyop) {}

        virtual bool isSameKindAs(const crBase* obj) const { return dynamic_cast<const IndexArray*>(obj)!=NULL; }
        
        virtual unsigned int index(unsigned int pos) const = 0;
    
        virtual ~IndexArray() {}
};

template<typename T, Array::Type ARRAYTYPE, int DataSize, int DataType>
class TemplateIndexArray : public IndexArray, public std::vector<T>
{
    public:
    
        TemplateIndexArray() : IndexArray(ARRAYTYPE,DataSize,DataType) {}

        TemplateIndexArray(const TemplateIndexArray& ta,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            IndexArray(ta,copyop),
            std::vector<T>(ta) {}
        
        TemplateIndexArray(unsigned int no) : 
            IndexArray(ARRAYTYPE,DataSize,DataType),
            std::vector<T>(no) {}

        TemplateIndexArray(unsigned int no,T* ptr) : 
            IndexArray(ARRAYTYPE,DataSize,DataType),
            std::vector<T>(ptr,ptr+no) {}

        template <class InputIterator>
        TemplateIndexArray(InputIterator first,InputIterator last) : 
            IndexArray(ARRAYTYPE,DataSize,DataType),
            std::vector<T>(first,last) {}        

        TemplateIndexArray& operator = (const TemplateIndexArray& array)
        {
            if (this==&array) return *this;
            assign(array.begin(),array.end());
            return *this;
        } 

        virtual crBase* cloneType() const { return new TemplateIndexArray(); }
        virtual crBase* clone(const crCopyOp& copyop) const { return new TemplateIndexArray(*this,copyop); } 

		virtual crBase* cloneType()  { return new TemplateIndexArray(); }
		virtual crBase* clone(const crCopyOp& copyop)  { return new TemplateIndexArray(*this,copyop); }        

        virtual void accept(ArrayVisitor& av) { av.apply(*this); }
        virtual void accept(ConstArrayVisitor& av) const { av.apply(*this); }

        virtual void accept(unsigned int index,ValueVisitor& vv) { vv.apply( (*this)[index] ); }
        virtual void accept(unsigned int index,ConstValueVisitor& vv) const {  vv.apply( (*this)[index] );}

        virtual int compare(unsigned int lhs,unsigned int rhs) const
        {
            const T& elem_lhs = (*this)[lhs];
            const T& elem_rhs = (*this)[rhs];
            if (elem_lhs<elem_rhs) return -1;
            if (elem_rhs<elem_lhs) return 1;
            return 0;
        }

		virtual int compare(unsigned int lhs,unsigned int rhs)
		{
			const T& elem_lhs = (*this)[lhs];
			const T& elem_rhs = (*this)[rhs];
			if (elem_lhs<elem_rhs) return -1;
			if (elem_rhs<elem_lhs) return 1;
			return 0;
		}

        virtual const void*   getDataPointer() const { if (!empty()) return &front(); else return 0; }
        virtual unsigned int    getTotalDataSize() const { return size()*sizeof(T); }
        virtual unsigned int    getNumElements() const { return size(); }
		virtual int             getStride()const { return DataSize*sizeof(DataType); }

		virtual const void*     getDataPointer() { if (!empty()) return &front(); else return 0; };
		virtual unsigned int    getTotalDataSize(){ return size()*sizeof(T); }
		virtual unsigned int    getNumElements(){ return size(); }

        virtual unsigned int    index(unsigned int pos) const { return (*this)[pos]; }
		virtual unsigned int    index(unsigned int pos) { return (*this)[pos]; }

    virtual ~TemplateIndexArray() {}
    
};

typedef TemplateIndexArray<signed char,Array::ByteArrayType,1,VR_BYTE>          ByteArray;
typedef TemplateIndexArray<short,Array::ShortArrayType,1,VR_SHORT>            ShortArray;
typedef TemplateIndexArray<int,Array::IntArrayType,1,VR_INT>                  IntArray;
typedef TemplateIndexArray<unsigned char,Array::UByteArrayType,1,VR_UNSIGNED_BYTE>    UByteArray;
typedef TemplateIndexArray<unsigned short,Array::UShortArrayType,1,VR_UNSIGNED_SHORT> UShortArray;
typedef TemplateIndexArray<unsigned int,Array::UIntArrayType,1,VR_UNSIGNED_INT>       UIntArray;

typedef TemplateArray<float,Array::FloatArrayType,1,VR_FLOAT>                   FloatArray;
//typedef TemplateArray<crVector4ub,Array::UByte4ArrayType,4,VR_UNSIGNED_BYTE>         Vec4ubArray;
typedef TemplateArray<crVector2,Array::Vec2ArrayType,2,VR_FLOAT>                Vec2Array;
typedef TemplateArray<crVector3,Array::Vec3ArrayType,3,VR_FLOAT>                Vec3Array;
typedef TemplateArray<crVector4,Array::Vec4ArrayType,4,VR_FLOAT>                Vec4Array;

typedef TemplateArray<crVector4ub,Array::Vec4ubArrayType,4,GL_UNSIGNED_BYTE>  Vec4ubArray;

typedef TemplateArray<crVector2s,Array::Vec2sArrayType,2,GL_SHORT>        Vec2sArray;
typedef TemplateArray<crVector3s,Array::Vec3sArrayType,3,GL_SHORT>        Vec3sArray;
typedef TemplateArray<crVector4s,Array::Vec4sArrayType,4,GL_SHORT>        Vec4sArray;

typedef TemplateArray<crVector2b,Array::Vec2bArrayType,2,GL_BYTE>          Vec2bArray;
typedef TemplateArray<crVector3b,Array::Vec3bArrayType,3,GL_BYTE>          Vec3bArray;
typedef TemplateArray<crVector4b,Array::Vec4bArrayType,4,GL_BYTE>          Vec4bArray;

class ArrayVisitor
{
    public:
        ArrayVisitor() {}
        
        virtual void apply(Array&) {}
        virtual void apply(ByteArray&) {}
        virtual void apply(ShortArray&) {}
        virtual void apply(IntArray&) {}
        virtual void apply(UByteArray&) {}
        virtual void apply(UShortArray&) {}
        virtual void apply(UIntArray&) {}
        virtual void apply(Vec4ubArray&) {}
        virtual void apply(FloatArray&) {}
        virtual void apply(Vec2Array&) {}
        virtual void apply(Vec3Array&) {}
        virtual void apply(Vec4Array&) {}

		virtual void apply(Vec2bArray&) {}        
		virtual void apply(Vec3bArray&) {}        
		virtual void apply(Vec4bArray&) {}        

		virtual void apply(Vec2sArray&) {}
		virtual void apply(Vec3sArray&) {}
		virtual void apply(Vec4sArray&) {}
};

class ConstArrayVisitor
{
    public:
        ConstArrayVisitor() {}
        
        virtual void apply(const Array&) {}
        virtual void apply(const ByteArray&) {}
        virtual void apply(const ShortArray&) {}
        virtual void apply(const IntArray&) {}
        virtual void apply(const UByteArray&) {}
        virtual void apply(const UShortArray&) {}
        virtual void apply(const UIntArray&) {}
        virtual void apply(const Vec4ubArray&) {}
        virtual void apply(const FloatArray&) {}
        virtual void apply(const Vec2Array&) {}
        virtual void apply(const Vec3Array&) {}
        virtual void apply(const Vec4Array&) {}

		virtual void apply(const Vec2bArray&) {}        
		virtual void apply(const Vec3bArray&) {}        
		virtual void apply(const Vec4bArray&) {}        

		virtual void apply(const Vec2sArray&) {}
		virtual void apply(const Vec3sArray&) {}
		virtual void apply(const Vec4sArray&) {}
};


class ValueVisitor
{
    public:
        ValueVisitor() {}
        
        virtual void apply(signed char&) {}
        virtual void apply(short&) {}
        virtual void apply(int&) {}
        virtual void apply(unsigned short&) {}
        virtual void apply(unsigned char&) {}
        virtual void apply(unsigned int&) {}
        virtual void apply(float&) {}
        virtual void apply(crVector4ub&) {}
        virtual void apply(crVector2&) {}
        virtual void apply(crVector3&) {}
        virtual void apply(crVector4&) {}

		virtual void apply(const crVector2b&) {}
		virtual void apply(const crVector3b&) {}
		virtual void apply(const crVector4b&) {}        

		virtual void apply(const crVector2s&) {}
		virtual void apply(const crVector3s&) {}
		virtual void apply(const crVector4s&) {}
};

class ConstValueVisitor
{
    public:
        ConstValueVisitor() {}
        
        virtual void apply(const signed char&) {}
        virtual void apply(const short&) {}
        virtual void apply(const int&) {}
        virtual void apply(const unsigned short&) {}
        virtual void apply(const unsigned char&) {}
        virtual void apply(const unsigned int&) {}
        virtual void apply(const float&) {}
        virtual void apply(const crVector4ub&) {}
        virtual void apply(const crVector2&) {}
        virtual void apply(const crVector3&) {}
        virtual void apply(const crVector4&) {}

		virtual void apply(const crVector2b&) {}
		virtual void apply(const crVector3b&) {}
		virtual void apply(const crVector4b&) {}        

		virtual void apply(const crVector2s&) {}
		virtual void apply(const crVector3s&) {}
		virtual void apply(const crVector4s&) {}
};


}

#endif
