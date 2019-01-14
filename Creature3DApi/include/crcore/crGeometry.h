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
#ifndef   CRCORE_CRGEOMETRY_H
#define   CRCORE_CRGEOMETRY_H

#include <CRCore/crCopyOp.h>
#include <CRCore/crPrimitive.h>
#include <CRCore/crDrawable.h>
//#include <CRCore/crVertexList.h>
#include <CRCore/crVector2.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crArray.h>

namespace CRCore{

  class crPrimitive;
  
  class CR_EXPORT crGeometry : public crDrawable
  {
  public:

    crGeometry();
    
	/** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
    crGeometry(const crGeometry& geometry,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

    virtual crBase* cloneType() const { return new crGeometry(); }
    virtual crBase* clone(const crCopyOp& copyop) const { return new crGeometry(*this,copyop); }        
    virtual bool isSameKindAs(const crBase* base) const { return dynamic_cast<const crGeometry*>(base)!=NULL; }
    virtual const char* libraryName() const { return "CRCore"; }
    virtual const char* className() const { return "crGeometry"; }

    virtual crGeometry* asGeometry() { return this; }
    virtual const crGeometry* asGeometry() const { return this; }
	
	bool empty() const;

	enum BindMode
	{
		BIND_XYZ          = 0x1,
		BIND_NORMAL       = 0x2,
		BIND_DIFFUSE      = 0x4,
		BIND_SPECULAR     = 0x8,

		BIND_TEXCOORD     = 0x100,
		BIND_TEXCOORD1    = BIND_TEXCOORD,
		BIND_TEXCOORD2    = 0x101,
		BIND_TEXCOORD3    = 0x102,
		BIND_TEXCOORD4    = 0x104,
		BIND_TEXCOORD5    = 0x108,
		BIND_TEXCOORD6    = 0x110,
		BIND_TEXCOORD7    = 0x111,
		BIND_TEXCOORD8    = 0x112,

		BIND_TEXSTAGES    = 0x8
	};

	enum AttributeBinding
	{
		BIND_OFF=0,
		BIND_OVERALL,
		BIND_PER_PRIMITIVE_SET,
		BIND_PER_PRIMITIVE,
		BIND_PER_VERTEX
	};

	struct ArrayData
	{
	    ArrayData():
	        binding(BIND_OFF),
		    normalize(false),
		    offset(0) {}
		ArrayData(const ArrayData& data,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

		ArrayData(Array* a, AttributeBinding b, unsigned char n = false):
		    array(a),
			indices(0),
			binding(b),
			normalize(n),
			offset(0) {}

		ArrayData(Array* a, IndexArray* i, AttributeBinding b, unsigned char n = false):
		    array(a),
			indices(i),
			binding(b),
			normalize(n),
			offset(0) {}

		ArrayData& operator = (const ArrayData& rhs)
		{
			array = rhs.array;
			indices = rhs.indices;
			binding = rhs.binding;
			normalize = rhs.normalize;
			offset = rhs.offset;
			return *this;
		}

		inline bool empty() const { return !array.valid(); } 

		ref_ptr<Array>          array;
		ref_ptr<IndexArray>     indices;
		AttributeBinding        binding;
		unsigned char           normalize;
		mutable unsigned int    offset;
	};

	struct Vec3ArrayData
	{
		Vec3ArrayData():
	        binding(BIND_OFF),
		    normalize(false),
		    offset(0) {}

		Vec3ArrayData(const Vec3ArrayData& data,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

		Vec3ArrayData(Vec3Array* a, AttributeBinding b, unsigned char n = false):
		    array(a),
			indices(0),
			binding(b),
			normalize(n),
			offset(0) {}

		Vec3ArrayData(Vec3Array* a, IndexArray* i, AttributeBinding b, unsigned char n = false):
		    array(a),
			indices(i),
			binding(b),
			normalize(n),
			offset(0) {}

		Vec3ArrayData& operator = (const Vec3ArrayData& rhs)
		{
			array = rhs.array;
			indices = rhs.indices;
			binding = rhs.binding;
			normalize = rhs.normalize;
			offset = rhs.offset;
			return *this;
		}

		inline bool empty() const { return !array.valid(); } 

		ref_ptr<Vec3Array>      array;
		ref_ptr<IndexArray>     indices;
		AttributeBinding        binding;
		unsigned char           normalize;
		mutable unsigned int    offset;
	};

	/** static ArrayData which is returned get getTexCoordData(i) const and getVertexAttribData(i) const 
	* when i is out of range.*/
	static const ArrayData s_InvalidArrayData;

	typedef std::vector< ArrayData >  ArrayList;
	typedef std::map<std::string, void*> VertexBufList;

	//VertexArray
	void setVertexArray(Array* array) { m_vertexData.array = array; dirtyDisplayList(); dirtyBound(); }
	//获得Vertex数组	
	Array* getVertexArray() { return m_vertexData.array.get(); }
	const Array* getVertexArray() const  { return m_vertexData.array.get(); }

	void setVertexIndices(IndexArray* array) { m_vertexData.indices = array; computeFastPathsUsed(); dirtyDisplayList(); dirtyBound(); }
	IndexArray* getVertexIndices() { return m_vertexData.indices.get(); }
	const IndexArray* getVertexIndices() const  { return m_vertexData.indices.get(); }

	void setVertexData(const ArrayData& arrayData) { m_vertexData = arrayData; }
	ArrayData& getVertexData() { return m_vertexData; }
	const ArrayData& getVertexData() const { return m_vertexData; }

 ////1, stride
	//void setVertexStride(unsigned long s){m_vertexStride = s;}
	//unsigned long getVertexStride() const {return m_vertexStride;}

	////2, bind mode
	//void setVertexBindMode(BindMode mode = BIND_XYZ) {m_vertexBindMode = mode;}
	//BindMode getVertexBindMode()const {return m_vertexBindMode;}

	// Vertex Buffer List
	//VertexBufList& getVBList(){return m_vertexBufferList;}
	//const VertexBufList& getVBList() const {return m_vertexBufferList;}

	// Vertex Declar
	//void setVertexDeclar(void* p){m_pVertexDeclar = p;}
	//void*& getVertexDeclar(){return m_pVertexDeclar;}
	//const void* getVertexDeclar()const{return m_pVertexDeclar;}

	//NormalArray
	void setNormalBinding(AttributeBinding ab) { m_normalData.binding = ab; dirtyDisplayList(); computeFastPathsUsed(); }
	//获得法线的绑定模式
	AttributeBinding getNormalBinding() const { return m_normalData.binding; }
	AttributeBinding getNormalBinding() { return m_normalData.binding; }

	void setNormalArray(Vec3Array* array) { m_normalData.array = array; if (!m_normalData.array.valid()) m_normalData.binding=BIND_OFF; computeFastPathsUsed(); dirtyDisplayList(); }
	//获得Normal数组
	Vec3Array* getNormalArray() { return m_normalData.array.get(); }
	const Vec3Array* getNormalArray() const { return m_normalData.array.get(); }

	void setNormalIndices(IndexArray* array) { m_normalData.indices = array; computeFastPathsUsed(); dirtyDisplayList(); }
	IndexArray* getNormalIndices() { return m_normalData.indices.get(); }
	const IndexArray* getNormalIndices() const { return m_normalData.indices.get(); }

	void setNormalData(const Vec3ArrayData& arrayData) { m_normalData = arrayData; }
	Vec3ArrayData& getNormalData() { return m_normalData; }
	const Vec3ArrayData& getNormalData() const { return m_normalData; }


	void setColorBinding(AttributeBinding ab) { m_colorData.binding = ab; computeFastPathsUsed();}
	//获得Color的绑定模式
	AttributeBinding getColorBinding() const { return m_colorData.binding; }
	AttributeBinding getColorBinding() { return m_colorData.binding; }

	void setColorArray(Array* array) { m_colorData.array = array;  if (!m_colorData.array.valid()) m_colorData.binding=BIND_OFF; computeFastPathsUsed(); dirtyDisplayList(); }
	//获得Color数组
	Array* getColorArray() { return m_colorData.array.get(); }
	const Array* getColorArray() const { return m_colorData.array.get(); }

	void setColorIndices(IndexArray* array) { m_colorData.indices = array;  computeFastPathsUsed(); dirtyDisplayList(); }
	IndexArray* getColorIndices() { return m_colorData.indices.get(); }
	const IndexArray* getColorIndices() const { return m_colorData.indices.get(); }

	void setColorData(const ArrayData& arrayData) { m_colorData = arrayData; }
	ArrayData& getColorData() { return m_colorData; }
	const ArrayData& getColorData() const { return m_colorData; }



	void setSecondaryColorBinding(AttributeBinding ab) { m_secondaryColorData.binding = ab; computeFastPathsUsed();}
	//获得SecondaryColor的绑定模式	
	AttributeBinding getSecondaryColorBinding() const { return m_secondaryColorData.binding; }
	AttributeBinding getSecondaryColorBinding() { return m_secondaryColorData.binding; }

	void setSecondaryColorArray(Array* array) { m_secondaryColorData.array = array;  if (!m_secondaryColorData.array.valid()) m_secondaryColorData.binding=BIND_OFF; computeFastPathsUsed(); dirtyDisplayList(); }
	//获得SecondaryColor数组
	Array* getSecondaryColorArray() { return m_secondaryColorData.array.get(); }
	const Array* getSecondaryColorArray() const { return m_secondaryColorData.array.get(); }

	void setSecondaryColorIndices(IndexArray* array) { m_secondaryColorData.indices = array;  computeFastPathsUsed(); dirtyDisplayList(); }
	IndexArray* getSecondaryColorIndices() { return m_secondaryColorData.indices.get(); }
	const IndexArray* getSecondaryColorIndices() const { return m_secondaryColorData.indices.get(); }

	void setSecondaryColorData(const ArrayData& arrayData) { m_secondaryColorData = arrayData; }
	ArrayData& getSecondaryColorData() { return m_secondaryColorData; }
	const ArrayData& getSecondaryColorData() const { return m_secondaryColorData; }


	void setFogCoordBinding(AttributeBinding ab) { m_fogCoordData.binding = ab; computeFastPathsUsed();}
	//获得FogCoord的绑定模式	
	AttributeBinding getFogCoordBinding() const { return m_fogCoordData.binding; }
	AttributeBinding getFogCoordBinding() { return m_fogCoordData.binding; }

	void setFogCoordArray(Array* array) { m_fogCoordData.array = array; if (!m_fogCoordData.array.valid()) m_fogCoordData.binding=BIND_OFF; dirtyDisplayList(); }
	Array* getFogCoordArray() { return m_fogCoordData.array.get(); }
	const Array* getFogCoordArray() const { return m_fogCoordData.array.get(); }

	void setFogCoordIndices(IndexArray* array) { m_fogCoordData.indices = array; dirtyDisplayList(); }
	IndexArray* getFogCoordIndices() { return m_fogCoordData.indices.get(); }
	const IndexArray* getFogCoordIndices() const { return m_fogCoordData.indices.get(); }

	void setFogCoordData(const ArrayData& arrayData) { m_fogCoordData = arrayData; }
	ArrayData& getFogCoordData() { return m_fogCoordData; }
	const ArrayData& getFogCoordData() const { return m_fogCoordData; }


	void addTexCoordArray(Array*);
	void setTexCoordArray(unsigned int unit,Array*);
	//获得TexCoord的绑定模式	
	AttributeBinding getTexCoordBinding(unsigned int unit) const 
	{ 
		if(unit>=m_texCoordList.size()) return BIND_OFF;
		else return m_texCoordList[unit].binding; 
	}
	AttributeBinding getTexCoordBinding(unsigned int unit) 
	{ 
		if(unit>=m_texCoordList.size()) return BIND_OFF;
		else return m_texCoordList[unit].binding; 
	}
	//获得unit层的TexCoord数组	传入的参数unit是贴图的层数	
	Array* getTexCoordArray(unsigned int unit);
	const Array* getTexCoordArray(unsigned int unit) const;

	void setTexCoordIndices(unsigned int unit,IndexArray*);
	IndexArray* getTexCoordIndices(unsigned int unit);
	const IndexArray* getTexCoordIndices(unsigned int unit) const;

	void setTexCoordData(unsigned int index,const ArrayData& arrayData);
	ArrayData& getTexCoordData(unsigned int index);
	const ArrayData& getTexCoordData(unsigned int index) const;

	//获得贴图的层数
	unsigned int getNumTexCoordArrays() const {  return m_texCoordList.size(); }
	unsigned int getNumTexCoordArrays() {  return m_texCoordList.size(); }
	ArrayList& getTexCoordArrayList() { return m_texCoordList; }
	const ArrayList& getTexCoordArrayList() const { return m_texCoordList; }


	void setVertexAttribArray(unsigned int index,Array* array);
	Array *getVertexAttribArray(unsigned int index);
	const Array *getVertexAttribArray(unsigned int index) const;

	void setVertexAttribIndices(unsigned int index,IndexArray* array);
	IndexArray* getVertexAttribIndices(unsigned int index);
	const IndexArray* getVertexAttribIndices(unsigned int index) const;

	void setVertexAttribBinding(unsigned int index,AttributeBinding ab);
	AttributeBinding getVertexAttribBinding(unsigned int index) const;
	AttributeBinding getVertexAttribBinding(unsigned int index);

	void setVertexAttribNormalize(unsigned int index,unsigned char norm);
	unsigned char getVertexAttribNormalize(unsigned int index) const;

	void setVertexAttribData(unsigned int index,const ArrayData& arrayData);
	ArrayData& getVertexAttribData(unsigned int index);
	const ArrayData& getVertexAttribData(unsigned int index) const;

	unsigned int getNumVertexAttribArrays() const { return m_vertexAttribList.size(); }
	unsigned int getNumVertexAttribArrays() { return m_vertexAttribList.size(); }
	ArrayList& getVertexAttribArrayList() { return m_vertexAttribList; }
	const ArrayList& getVertexAttribArrayList() const { return m_vertexAttribList; }

    typedef std::vector< ref_ptr<crPrimitive> >     PrimitiveList;

    void setPrimitiveList(const PrimitiveList& primitives) { m_primitives = primitives; }
    PrimitiveList& getPrimitiveList() { return m_primitives; }
    const PrimitiveList& getPrimitiveList() const { return m_primitives; }

    unsigned int getNumPrimitives() const { return m_primitives.size(); }
	unsigned int getNumPrimitives() { return m_primitives.size(); }
    crPrimitive* getPrimitive(unsigned int pos) { return m_primitives[pos].get(); }
    const crPrimitive* getPrimitive(unsigned int pos) const { return m_primitives[pos].get(); }

    /** Add a primitive set to the geometry.*/
    bool addPrimitive(crPrimitive* primitive);

    /** Set a primitive set to the specified position in geometry's primitive set list.*/
    bool setPrimitive(unsigned int i,crPrimitive* primitive);

    /** Insert a primitive set to the specified position in geometry's primitive set list.*/
    bool insertPrimitive(unsigned int i,crPrimitive* primitive);

    /** Remove primitive set(s) from the specified position in geometry's primitive set list.*/
    bool removePrimitive(unsigned int i,unsigned int numElementsToRemove=1);

    /** Get the index number of a primitive set, return a value between
    * 0 and getNumPrimitive()-1 if found, if not found then return getNumPrimitive().
    * When checking for a valid find value use if ((value=geoemtry->getPrimitiveIndex(primitive))!=geometry.getNumPrimitive()) as*/
    unsigned int getPrimitiveIndex(const crPrimitive* primitive) const;

	/** Set whether fast paths should be used when supported.*/
	void setFastPathHint(bool on) {  m_fastPathHint = on; }

	/** Get whether fast paths should be used when supported.*/
	bool getFastPathHint() const { return m_fastPathHint; }

	/** return true if OpenGL fast paths will be used with drawing this crGeometry.
	* Fast paths use vertex arrays, and glDrawArrays/glDrawElements.  Slow paths
	* use glBegin()/glVertex.../glEnd(). Use of per primitive bindings or per vertex indexed
	* arrays will drop the rendering path off the fast path.*/
	inline bool areFastPathsUsed() const { return m_fastPath && m_fastPathHint; }

	bool verifyBindings() const;

	void computeCorrectBindingsAndArraySizes();


	bool suitableForOptimization() const;

	void copyToAndOptimize(crGeometry& target);

	void computeInternalOptimizedGeometry();

	void removeInternalOptimizedGeometry() { m_internalOptimizedGeometry = 0; }

	void setInternalOptimizedGeometry(CRCore::crGeometry* geometry) { m_internalOptimizedGeometry = geometry; }

	CRCore::crGeometry* getInternalOptimizedGeometry() { return m_internalOptimizedGeometry.get(); }

	const CRCore::crGeometry* getInternalOptimizedGeometry() const { return m_internalOptimizedGeometry.get(); }


	/** draw crGeometry directly ignoring an OpenGL display list which could be attached.
	* This is the internal draw method which does the drawing itself,
	* and is the method to override when deriving from crGeometry for user-drawn objects.
	*/
	virtual void drawImplementation(crState& state) const;
	virtual void compileImplementation(crState& state) const;
	/** return true, CRCore::crGeometry does support accept(AttributeFunctor&).*/
	virtual bool supports(AttributeFunctor&) const { return true; }

	/** accept an AttributeFunctor and call its methods to tell it about the interal attributes that this Drawable has.*/
	virtual void accept(AttributeFunctor& af);

	/** return true, CRCore::crGeometry does support accept(ConstAttributeFunctor&).*/
	virtual bool supports(ConstAttributeFunctor&) const { return true; }

	/** accept an ConstAttributeFunctor and call its methods to tell it about the interal attributes that this Drawable has.*/
	virtual void accept(ConstAttributeFunctor& af) const;

	/** return true, CRCore::crGeometry does support accept(PrimitiveFunctor&) .*/
	virtual bool supports(PrimitiveFunctor&) const { return true; }

	/** accept a PrimitiveFunctor and call its methods to tell it about the interal primitives that this Drawable has.*/
	virtual void accept(PrimitiveFunctor& pf) const;
	
	/** return true, CRCore::crGeometrydoes support accept(PrimitiveIndexFunctor&) .*/
	virtual bool supports(PrimitiveIndexFunctor&) const { return true; }

	/** accept a PrimitiveFunctor and call its methods to tell it about the interal primitives that this Drawable has.*/
	virtual void accept(PrimitiveIndexFunctor& pf) const;

    virtual bool expand(const crGeometry& geo,int& offset);
    //// VertexList
    //void setVertexList(crVertexList* pVList);
    //crVertexList* getVertexList(){return m_pVertexList.get();}
    //const crVertexList* getVertexList() const {return m_pVertexList.get();}

    ///** draw crGeometry directly ignoring an OpenGL display list which could be attached.
    //* This is the internal draw method which does the drawing itself,
    //* and is the method to override when deriving from crGeometry for user-drawn objects.
    //*/
    //virtual void drawImplementation(/*crState& state*/) const;

	//typedef std::pair<void *,void *> TriMeshDataPair;
	//typedef std::vector<TriMeshDataPair> TriMeshDataList;
 //   
	//TriMeshDataList &getTriMeshDataList(){ return m_triMeshDataList; }

	//void * getTriMeshID( int pos )const{ return m_triMeshDataList[pos].first; }
	//void * getTriMeshID( int pos ) { return m_triMeshDataList[pos].first; }
	//void * getTriDataID( int pos )const{ return m_triMeshDataList[pos].second; }
	//void * getTriDataID( int pos ){ return m_triMeshDataList[pos].second; }
 //   
	//void addTriMeshData( void * meshID, void * dataID){ m_triMeshDataList.push_back( TriMeshDataPair(meshID,dataID) ); }

	virtual unsigned int getObjectSizeHint() const;
	virtual void releaseObjects(crState* state);

	static void clearExtensions();
	void computeCorrectBindingsAndArraySizes(ArrayData& arrayData,const char* arrayName);
	void computeCorrectBindingsAndArraySizes(Vec3ArrayData& arrayData,const char* arrayName);
  protected:
    crGeometry& operator = (const crGeometry&) { return *this;}
    virtual ~crGeometry();

	bool computeFastPathsUsed();

	bool verifyBindings(const ArrayData& arrayData) const;
	bool verifyBindings(const Vec3ArrayData& arrayData) const;

	ArrayData                       m_vertexData;
	//BindMode                        m_vertexBindMode;
	//unsigned long                   m_vertexStride;   // how many bytes a vertex has

	Vec3ArrayData                   m_normalData;
	ArrayData                       m_colorData;
	ArrayData                       m_secondaryColorData;
	ArrayData                       m_fogCoordData;
	ArrayList                       m_texCoordList;
	ArrayList                       m_vertexAttribList;

	mutable bool                    m_fastPath;
	bool                            m_fastPathHint;

	ref_ptr<crGeometry>             m_internalOptimizedGeometry;

    PrimitiveList                   m_primitives;
	//VertexBufList                   m_vertexBufferList;
	//void*                           m_pVertexDeclar;
    //ref_ptr<crVertexList>     m_pVertexList;

	//TriMeshDataList                 m_triMeshDataList;

  };
  extern CR_EXPORT CRCore::crObject* createObjectForImage(CRCore::crImage* image);
  extern CR_EXPORT CRCore::crObject* createObjectForImage(CRCore::crImage* image,float s,float t);

  extern CR_EXPORT crGeometry* createTexturedQuadGeometry(const crVector3f& corner,const crVector3f& widthVec,const crVector3f& heightVec, float l, float b, float r, float t);

  /** Convenience function to be used for creating quad geometry with texture coords.
  * Tex coords go from bottom left (0,0) to top right (s,t).
  */
  inline crGeometry* createTexturedQuadGeometry(const crVector3f& corner,const crVector3f& widthVec,const crVector3f& heightVec, float s=1.0f, float t=1.0f)
  {
	  return createTexturedQuadGeometry(corner,widthVec,heightVec, 0.0f, 0.0f, s, t);
  }

}

#endif    