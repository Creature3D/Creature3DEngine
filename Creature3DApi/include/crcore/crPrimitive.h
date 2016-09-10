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
#ifndef CRCORE_CRPRIMITIVE_H
#define CRCORE_CRPRIMITIVE_H

#include <CRCore/crBase.h>
#include <CRCore/crDrawable.h>
#include <CRCore/buffered_value.h>
#include <vector>

namespace CRCore{

class CR_EXPORT crPrimitive : public crBase
{
public:

	enum Type
	{
		PrimitiveType,
		DrawArraysPrimitiveType,
		DrawArrayLengthsPrimitiveType,
		DrawElementsUBytePrimitiveType,
		DrawElementsUShortPrimitiveType,
		DrawElementsUIntPrimitiveType
	};

	enum Mode
	{
		PT_POINTS = GL_POINTS,
		PT_LINES = GL_LINES,
		PT_LINE_LOOP = GL_LINE_LOOP,
		PT_LINE_STRIP = GL_LINE_STRIP,
		PT_TRIANGLES = GL_TRIANGLES,
		PT_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
		PT_TRIANGLE_FAN = GL_TRIANGLE_FAN,
		PT_QUADS = GL_QUADS,
		PT_QUAD_STRIP = GL_QUAD_STRIP,
		PT_POLYGON = GL_POLYGON
	};

	crPrimitive(Type primType=PrimitiveType,unsigned int  mode=0):
        m_primitiveType(primType),
	    m_mode(mode), 
		m_pIndexBuffer(NULL),
		m_modifiedCount(0){}

	crPrimitive(const crPrimitive& prim,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
		crBase(prim,copyop),
		m_primitiveType(prim.m_primitiveType),
		m_mode(prim.m_mode),
		m_pIndexBuffer(prim.m_pIndexBuffer),
		//m_drawCallback(prim.m_drawCallback),
		m_modifiedCount(0){}

	virtual ~crPrimitive();

	virtual bool isSameKindAs(const crBase* obj) const { return dynamic_cast<const crPrimitive*>(obj)!=NULL; }
	virtual const char* libraryName() const { return "CRCore"; }
	virtual const char* className() const { return "crPrimitive"; }

	Type getType() const { return m_primitiveType; }
	Type getType() { return m_primitiveType; }

	void setMode(unsigned int  mode) { m_mode = mode; }
	unsigned int  getMode() const { return m_mode; }
	unsigned int  getMode() { return m_mode; }

	virtual void draw(crState& state, bool useVertexBufferObjects) const=0;
	virtual void compile(crState& state, bool useVertexBufferObjects) const{};
	//{ if(m_drawCallback.valid()) m_drawCallback->drawImplementation(this,state,useVertexBufferObjects); }
	/** If State is non-zero, this function releases OpenGL objects for
	* the specified graphics context. Otherwise, releases OpenGL objexts
	* for all graphics contexts. */
	//virtual void releaseObjects(crState* state = 0) 
	//{ if(m_drawCallback.valid()) m_drawCallback->releaseImplementation(this,state); }
	//virtual void releaseObjects(crState* state = 0);

	virtual void accept(CRCore::crDrawable::PrimitiveFunctor& functor) const = 0;
	virtual void accept(CRCore::crDrawable::PrimitiveIndexFunctor& functor) const = 0;

 //   struct DrawCallback : public virtual Referenced
	//{
	//    virtual void drawImplementation( const crPrimitive *primitive, crState& state, bool useVertexBufferObjects) const = 0;
	//	virtual void releaseImplementation(const crPrimitive *primitive, crState* state) const{}
	//};
 //   void setDrawCallback(DrawCallback* dc) { m_drawCallback=dc;}
 //   DrawCallback* getDrawCallback() { return m_drawCallback.get(); }
 //   const DrawCallback* getDrawCallback() const { return m_drawCallback.get(); }

	virtual void replaceIndex(int oldIndex, int newIndex)=0;

	//获得index地址
	virtual const void* getIndeciesAddr()const = 0;
	//return m_indexArray[pos]
	virtual unsigned int index(unsigned int pos) const=0;
	//return m_indexArray.size();
	virtual unsigned int getNumIndices() const = 0;
	virtual unsigned long getTotalDataSize() const = 0;
	//设置index值的偏移量
	virtual void offsetIndices(int offset) = 0;
	//获得面的个数
	virtual unsigned int getNumPrimitives() const = 0; 

	virtual unsigned int index(unsigned int pos) =0;
	virtual unsigned int getNumIndices() = 0;
	virtual unsigned long getTotalDataSize() = 0;
	virtual unsigned int getNumPrimitives() = 0; 
	virtual void reset() = 0; 
	virtual bool empty()const = 0;
	virtual void reserve(int count) = 0;

	virtual int getStride()const = 0;

	void setIndexBuffer(void* p){m_pIndexBuffer = p;}
	void*& getIndexBuffer(){return m_pIndexBuffer;}
	const void* getIndexBuffer()const{return m_pIndexBuffer;}

	virtual bool supportsBufferObject() const { return false; }

	/** Dirty the primitive, which increments the modified count, to force buffer objects to update. */
	inline void dirty() { ++m_modifiedCount; }      

	/** Set the modified count value.*/
	inline void setModifiedCount(unsigned int value) { m_modifiedCount=value; }

	/** Get modified count value.*/
	inline unsigned int getModifiedCount() const { return m_modifiedCount; }

	struct ObjectIDModifiedCountPair
	{
		ObjectIDModifiedCountPair():
	        m_objectID(0),
		    m_modifiedCount(0) {}

		ObjectIDModifiedCountPair(const ObjectIDModifiedCountPair& obj):
	        m_objectID(obj.m_objectID),
		    m_modifiedCount(obj.m_modifiedCount) {}

		ObjectIDModifiedCountPair& operator = (const ObjectIDModifiedCountPair& obj)
		{
			m_objectID = obj.m_objectID;
			m_modifiedCount = obj.m_modifiedCount;
			return *this;
		}

		unsigned int m_objectID;
		unsigned int m_modifiedCount;
	};
	typedef CRCore::buffered_object<ObjectIDModifiedCountPair> GLObjectList;
	GLObjectList& getVBOList()const { return m_vboList; }
protected:
	Type    m_primitiveType;
	unsigned int   m_mode;
    //ref_ptr<DrawCallback> m_drawCallback;
	void*                 m_pIndexBuffer;

	unsigned int    m_modifiedCount;

	mutable GLObjectList    m_vboList;
};

template<class T>
class TemplatePrimitive : public crPrimitive
{
public:
	TemplatePrimitive(Type primType=PrimitiveType,unsigned int  mode=0):
	  crPrimitive(primType,mode){}
	
    TemplatePrimitive(const TemplatePrimitive& prim,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
		crPrimitive(prim,copyop)
	{  setIndexArray(prim.m_indexArray); }

	TemplatePrimitive(Type primType,unsigned int mode,unsigned int no):
		crPrimitive(primType,mode)
		{ m_indexArray.reserve(no); }

	typedef std::vector<T> PT_IndexArray;

	virtual void replaceIndex(int oldIndex, int newIndex)
	{
		for( PT_IndexArray::iterator itr = m_indexArray.begin();
			 itr != m_indexArray.end();
			 ++itr )
		{
			if(*itr == (T)oldIndex)
				*itr = (T)newIndex;
		}
	}

	virtual const void* getIndeciesAddr()const { if (!m_indexArray.empty()) return &m_indexArray.front(); else return 0; }
	virtual PT_IndexArray& getIndexArray(){ return m_indexArray; }
	virtual const PT_IndexArray& getIndexArray()const{ return m_indexArray; }
	virtual void push_back(const T &index){ m_indexArray.push_back(index); }
	virtual void setIndexArray( const PT_IndexArray& array )
	{ 
		//m_indexArray.clear(); 
		m_indexArray.resize(array.size()); 
		std::copy(array.begin(),array.end(),m_indexArray.begin()/*std::back_inserter(m_indexArray)*/);
	}
	virtual void reset(){ int i = m_indexArray.size(); m_indexArray.clear(); m_indexArray.reserve(i); }
	virtual unsigned int index(unsigned int pos) { return m_indexArray[pos]; };
	virtual unsigned int index(unsigned int pos) const{ return m_indexArray[pos]; };
	virtual unsigned int getNumIndices() const { return m_indexArray.size(); };
	virtual unsigned int getNumIndices() { return m_indexArray.size(); };
	inline T& operator [] (unsigned int i) { return m_indexArray[i]; }
	inline T  operator [] (unsigned int i) const { return m_indexArray[i]; }

	virtual unsigned long getTotalDataSize() const { return m_indexArray.size() * sizeof(T); }
	virtual unsigned long getTotalDataSize() { return m_indexArray.size() * sizeof(T); }
	virtual bool empty()const { return m_indexArray.empty(); }
	virtual void reserve(int count) { m_indexArray.reserve(count); }
	virtual int getStride()const
	{
		switch(m_mode)
		{
		case(PT_POINTS): return sizeof(T);
		case(PT_LINES): return 2*sizeof(T);
		case(PT_TRIANGLES): return 3*sizeof(T);
		case(PT_QUADS): return 4*sizeof(T);
		case(PT_LINE_STRIP):
		case(PT_LINE_LOOP):
		case(PT_TRIANGLE_STRIP):
		case(PT_TRIANGLE_FAN):
		case(PT_QUAD_STRIP):
		case(PT_POLYGON): return sizeof(T);
		}
		return 0;
	}

	virtual void offsetIndices(int offset)
	{
		for(PT_IndexArray::iterator itr=m_indexArray.begin();
			itr!=m_indexArray.end();
			++itr)
		{
			*itr += offset;
		}
	}
   
	virtual unsigned int getNumPrimitives() const 
	{
		switch(m_mode)
		{
		case(PT_POINTS): return getNumIndices();
		case(PT_LINES): return getNumIndices()/2;
		case(PT_TRIANGLES): return getNumIndices()/3;
		case(PT_QUADS): return getNumIndices()/4;
		case(PT_LINE_STRIP):
		case(PT_LINE_LOOP):
		case(PT_TRIANGLE_STRIP):
		case(PT_TRIANGLE_FAN):
		case(PT_QUAD_STRIP):
		case(PT_POLYGON): return 1;
		}
		return 0;
	}

	virtual unsigned int getNumPrimitives()
	{
		switch(m_mode)
		{
		case(PT_POINTS): return getNumIndices();
		case(PT_LINES): return getNumIndices()/2;
		case(PT_TRIANGLES): return getNumIndices()/3;
		case(PT_QUADS): return getNumIndices()/4;
		case(PT_LINE_STRIP):
		case(PT_LINE_LOOP):
		case(PT_TRIANGLE_STRIP):
		case(PT_TRIANGLE_FAN):
		case(PT_QUAD_STRIP):
		case(PT_POLYGON): return 1;
		}
		return 0;
	}

protected:
	PT_IndexArray m_indexArray;

};

class CR_EXPORT DrawArrays : public TemplatePrimitive<int>
{
public:
	DrawArrays(unsigned int mode=0):
	    TemplatePrimitive<int>(DrawArraysPrimitiveType,mode),
		m_first(0),
		m_count(0) {}

    DrawArrays(unsigned int mode, int first, int count):
	    TemplatePrimitive<int>(DrawArraysPrimitiveType,mode),
		m_first(first),
		m_count(count) {}

    DrawArrays(const DrawArrays& da,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY):
	    TemplatePrimitive<int>(da,copyop),
		m_first(da.m_first),
		m_count(da.m_count) {}

    virtual ~DrawArrays() {}

    META_Base(CRCore,DrawArrays);

	void set(unsigned int mode,int first, int count)
	{
	    m_mode = mode;
		m_first = first;
		m_count = count;
	}

	void setFirst(int first) { m_first = first; }
	int getFirst() const { return m_first; }
	int getFirst() { return m_first; }

	void setCount(int count) { m_count = count; }
	int getCount() const { return m_count; }
	int getCount() { return m_count; }

	virtual void accept(CRCore::crDrawable::PrimitiveFunctor& functor) const;
	virtual void accept(CRCore::crDrawable::PrimitiveIndexFunctor& functor) const;

	virtual unsigned int getNumIndices() const { return m_count; }
	virtual unsigned int index(unsigned int pos) const { return m_first+pos; }
	virtual unsigned int getNumIndices()  { return m_count; }
	virtual unsigned int index(unsigned int pos)  { return m_first+pos; }
	virtual void offsetIndices(int offset) { m_first += offset; }

	void draw(crState& state, bool useVertexBufferObjects) const;
protected:
	int   m_first;
	int   m_count;
};

class CR_EXPORT DrawArrayLengths : public TemplatePrimitive<int>
{
public:
    DrawArrayLengths(unsigned int mode=0):
	    TemplatePrimitive<int>(DrawArrayLengthsPrimitiveType,mode),
		m_first(0) {}

    DrawArrayLengths(const DrawArrayLengths& dal,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY):
	    TemplatePrimitive<int>(dal,copyop),
		m_first(dal.m_first) {}

  //  DrawArrayLengths(unsigned int mode, int first, unsigned int no, int* ptr) : 
	 //   CRCore::crPrimitive(DrawArrayLengthsPrimitiveType,mode),
		//VectorSizei(ptr,ptr+no),
		//m_first(first) {}

    DrawArrayLengths(unsigned int mode,int first, unsigned int no) : 
	    TemplatePrimitive<int>(DrawArrayLengthsPrimitiveType,mode,no),
			m_first(first){}

	DrawArrayLengths(unsigned int mode,int first) : 
	    TemplatePrimitive<int>(DrawArrayLengthsPrimitiveType,mode),
		m_first(first) {}
    
	virtual ~DrawArrayLengths() {}

	META_Base(CRCore,DrawArrayLengths);

    void setFirst(int first) { m_first = first; }
    int getFirst() const { return m_first; }
	int getFirst() { return m_first; }

    virtual void accept(CRCore::crDrawable::PrimitiveFunctor& functor) const;
	virtual void accept(CRCore::crDrawable::PrimitiveIndexFunctor& functor) const;

	virtual unsigned int getNumIndices() const;
	virtual unsigned int index(unsigned int pos) const { return m_first+pos; }
	virtual unsigned int getNumIndices();
	virtual unsigned int index(unsigned int pos)  { return m_first+pos; }

	virtual void offsetIndices(int offset) { m_first += offset; }

	virtual unsigned int getNumPrimitives() const 
	{
	    switch(m_mode)
		{
		case(PT_POINTS): return getNumIndices();
		case(PT_LINES): return getNumIndices()/2;
		case(PT_TRIANGLES): return getNumIndices()/3;
		case(PT_QUADS): return getNumIndices()/4;
		case(PT_LINE_STRIP):
		case(PT_LINE_LOOP):
		case(PT_TRIANGLE_STRIP):
		case(PT_TRIANGLE_FAN):
		case(PT_QUAD_STRIP):
		case(PT_POLYGON): return m_indexArray.size();
		}
		return 0;
	}

	virtual unsigned int getNumPrimitives() 
	{
		switch(m_mode)
		{
		case(PT_POINTS): return getNumIndices();
		case(PT_LINES): return getNumIndices()/2;
		case(PT_TRIANGLES): return getNumIndices()/3;
		case(PT_QUADS): return getNumIndices()/4;
		case(PT_LINE_STRIP):
		case(PT_LINE_LOOP):
		case(PT_TRIANGLE_STRIP):
		case(PT_TRIANGLE_FAN):
		case(PT_QUAD_STRIP):
		case(PT_POLYGON): return m_indexArray.size();
		}
		return 0;
	}
	void draw(crState& state, bool useVertexBufferObjects) const;
protected:
	int   m_first;
};

class CR_EXPORT DrawElementsUByte : public TemplatePrimitive<unsigned char>
{
public:
    DrawElementsUByte(unsigned int mode=0):
	    TemplatePrimitive<unsigned char>(DrawElementsUBytePrimitiveType,mode) {}

	DrawElementsUByte(const DrawElementsUByte& array,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY):
	    TemplatePrimitive<unsigned char>(array,copyop){}

 /*   DrawElementsUByte(unsigned int mode,unsigned int no,unsigned char* ptr) : 
	    CRCore::crPrimitive(DrawElementsUBytePrimitiveType,mode),
		  VectorUByte(ptr,ptr+no) {}*/

    DrawElementsUByte(unsigned int mode,unsigned int no) : 
		TemplatePrimitive<unsigned char>(DrawElementsUBytePrimitiveType,mode,no){}
	 
    virtual ~DrawElementsUByte() {}

	META_Base(CRCore,DrawElementsUByte);

	virtual void accept(CRCore::crDrawable::PrimitiveFunctor& functor) const;
	virtual void accept(CRCore::crDrawable::PrimitiveIndexFunctor& functor) const;
	void draw(crState& state, bool useVertexBufferObjects) const;
	void compile(crState& state, bool useVertexBufferObjects) const;
	virtual void releaseObjects(crState* state = 0);
};

class CR_EXPORT DrawElementsUShort : public TemplatePrimitive<unsigned short>
{
public:
    DrawElementsUShort(unsigned int mode=0):
	    TemplatePrimitive<unsigned short>(DrawElementsUShortPrimitiveType,mode) {}

	DrawElementsUShort(const DrawElementsUShort& array,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY):
		TemplatePrimitive<unsigned short>(array,copyop){}

   /* DrawElementsUShort(unsigned int mode,unsigned int no,unsigned short* ptr) : 
	    CRCore::crPrimitive(DrawElementsUShortPrimitiveType,mode),
		VectorUShort(ptr,ptr+no) {}*/

	DrawElementsUShort(unsigned int mode,unsigned int no) : 
	    TemplatePrimitive<unsigned short>(DrawElementsUShortPrimitiveType,mode,no){}

	//template <class InputIterator>
	//DrawElementsUShort(unsigned int mode, InputIterator first,InputIterator last) : 
	//	  CRCore::crPrimitive(DrawElementsUShortPrimitiveType,mode),
	//		  VectorUShort(first,last) {}

    virtual ~DrawElementsUShort() {}

	META_Base(CRCore,DrawElementsUShort);

	virtual void accept(CRCore::crDrawable::PrimitiveFunctor& functor) const;
	virtual void accept(CRCore::crDrawable::PrimitiveIndexFunctor& functor) const;
	void draw(crState& state, bool useVertexBufferObjects) const;
	void compile(crState& state, bool useVertexBufferObjects) const;
	virtual void releaseObjects(crState* state = 0);
};

class CR_EXPORT DrawElementsUInt : public TemplatePrimitive<unsigned int>
{
public:
    DrawElementsUInt(unsigned int mode=0):
	    TemplatePrimitive<unsigned int>(DrawElementsUIntPrimitiveType,mode) {}

	DrawElementsUInt(const DrawElementsUInt& array,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY):
		TemplatePrimitive<unsigned int>(array,copyop){}

   //DrawElementsUInt(unsigned int mode,unsigned int no,unsigned int* ptr) : 
	  //CRCore::crPrimitive(DrawElementsUIntPrimitiveType,mode),
		 // VectorUInt(ptr,ptr+no) {}

	DrawElementsUInt(unsigned int mode,unsigned int no) : 
	    TemplatePrimitive<unsigned int>(DrawElementsUIntPrimitiveType,mode,no){}

		  //template <class InputIterator>
			 // DrawElementsUInt(unsigned int mode, InputIterator first,InputIterator last) : 
		  //CRCore::crPrimitive(DrawElementsUIntPrimitiveType,mode),
			 // VectorUInt(first,last) {}
			  
    virtual ~DrawElementsUInt() {}

	META_Base(CRCore,DrawElementsUInt);

	virtual void accept(CRCore::crDrawable::PrimitiveFunctor& functor) const;
	virtual void accept(CRCore::crDrawable::PrimitiveIndexFunctor& functor) const;
	void draw(crState& state, bool useVertexBufferObjects) const;
	void compile(crState& state, bool useVertexBufferObjects) const;
	virtual void releaseObjects(crState* state = 0);
};

}

#endif