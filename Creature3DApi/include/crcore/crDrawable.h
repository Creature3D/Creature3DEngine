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
#ifndef CRCORE_CRDRAWABLE_H
#define CRCORE_CRDRAWABLE_H

#include <CRCore\crVector2.h>
#include <CRCore\crVector3.h>
#include <CRCore\crVector4.h>
#include <CRCore\crBase.h>
#include <CRCore\crBoundingBox.h>
#include <CRCore\crState.h>
#include <CRCore\crObject.h>
#include <CRCore\buffered_value.h>
#include <CRCore\crShape.h>
#include <CRCore/crNotify.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRCore/thread/crMutex.h>

#include <vector>
#include <list>
namespace CRCore{

class crNode;
class crNodeVisitor;
class crGeometry;

#define USE_SEPARATE_COMPILE_AND_EXECUTE

class CR_EXPORT crDrawable : public crBase
{
  public:
    crDrawable();
    virtual ~crDrawable();

	static unsigned int s_numberDrawablesReusedLastInLastFrame;
	static unsigned int s_numberNewDrawablesInLastFrame;
	static unsigned int s_numberDeletedDrawablesInLastFrame;
	static unsigned int s_minimumNumberOfDisplayListsToRetainInCache;

	typedef std::multimap<unsigned int,unsigned int> DisplayListMap;
	typedef std::map<unsigned int,DisplayListMap> DeletedDisplayListCache;

	static crMutex s_mutex_deletedDisplayListCache;
	static DeletedDisplayListCache s_deletedDisplayListCache;

	static crMutex s_mutex_deletedVertexBufferObjectCache;
	static DeletedDisplayListCache s_deletedVertexBufferObjectCache;

	enum Type
	{
		DUMMY,
		GEOMETRY,
        SHAPEDRAWABLE,
        PUPPETGEOMETRY,
		TEXTDRAWABLE
	};

	/** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
	crDrawable(const crDrawable& drawable,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

	virtual bool isSameKindAs(const crBase* obj) const { return dynamic_cast<const crDrawable*>(obj)!=NULL; }
	virtual const char* libraryName() const { return "CRCore"; }
	virtual const char* className() const { return "crDrawable"; }

	Type getType() const { return m_type; }
	Type getType() { return m_type; }

	/** convert 'this' into a Geometry pointer if crDrawable is a Geometry, otherwise return 0.
	* Equivalent to dynamic_cast<Geometry*>(this).*/
	virtual crGeometry* asGeometry() { return 0; }
	/** convert 'const this' into a const Geometry pointer if crDrawable is a Geometry, otherwise return 0.
	* Equivalent to dynamic_cast<const Geometry*>(this).*/
	virtual const crGeometry* asGeometry() const { return 0; }

    typedef std::vector<crNode*>  ParentList;
    inline const ParentList& getParents() const { return m_parents; }
    inline ParentList& getParents() { return m_parents; }

    inline crNode* getParent(unsigned int i)  { return i<m_parents.size()?m_parents[i]:NULL; }
    inline const crNode* getParent(unsigned int i) const  { return i<m_parents.size()?m_parents[i]:NULL; }
    inline unsigned int getNumParents() const { return m_parents.size(); }

    void addParent(crNode* node);
    void removeParent(crNode* node);

	/** Dirty the bounding box, forcing a computeBound() on the next call
	* to getBound(). Should be called in the internal geometry of the crDrawable
	* is modified.*/        
	void dirtyBound();
	/** get bounding box of geoset. 
	* Note, now made virtual to make it possible to implement user-drawn
	* objects albeit so what crudely, to be improved later.
	*/
	inline const crBoundingBox& getBound() const
    {
      if( !m_bbox_computed)
        computeBound();
      return m_bbox;
    }

	inline const crBoundingBox& getBound()
	{
		if( !m_bbox_computed)
			computeBound();
		return m_bbox;
	}

    inline const crBoundingBox& getBoundBox() const
    {
      if( !m_bbox_computed)
        computeBound();
      return m_bbox;
    }

	inline const crBoundingBox& getBoundBox()
	{
		if( !m_bbox_computed)
			computeBound();
		return m_bbox;
	}
    inline void setBoundBox(const crVector3& center,const crVector3& length)
	{
		m_bbox.set(center-length,center+length);
		m_bbox_computed = true;
	}

    struct UpdateCallback : public virtual crBase
    {
		UpdateCallback() {}

		UpdateCallback(const UpdateCallback&,const crCopyOp&) {}
		META_Base(CRCore,UpdateCallback);
      /** do customized app code.*/
		virtual void update(crNodeVisitor *visitor,crDrawable* drawable){ };
    };

    void setUpdateCallback(UpdateCallback* ac);
    UpdateCallback* getUpdateCallback() { return m_updateCallback.get(); }
    const UpdateCallback* getUpdateCallback() const { return m_updateCallback.get(); }
	/** Return whether this Drawable has update callbacks associated with it, and therefore must be traversed.*/
	bool requiresUpdateTraversal() const { return m_updateCallback.valid() || (m_stateset.valid() && m_stateset->requiresUpdateTraversal()); }

	//void setUpdateUniformCallback(UpdateCallback* ac);
	//UpdateCallback* getUpdateUniformCallback() { return m_updateUniformCallback.get(); }
	//const UpdateCallback* getUpdateUniformCallback() const { return m_updateUniformCallback.get(); }
	//bool requiresUpdateUniformTraversal() const { return m_updateUniformCallback.valid(); }

	//struct EventCallback : public virtual crBase
	//{
	//	EventCallback() {}

	//	EventCallback(const EventCallback&,const crCopyOp&) {}

	//	META_Base(CRCore,EventCallback);

	//	/** do customized Event code.*/
	//	virtual void event(crNodeVisitor*, crDrawable*) {}
	//};

	/** Set the EventCallback which allows users to attach customize the updating of an object during the Event traversal.*/
	//virtual void setEventCallback(EventCallback* ac);

	///** Get the non const EventCallback.*/
	//EventCallback* getEventCallback() { return m_eventCallback.get(); }

	///** Get the const EventCallback.*/
	//const EventCallback* getEventCallback() const { return m_eventCallback.get(); }

	/** Return whether this Drawable has event callbacks associated with it, and therefore must be traversed.*/
	//bool requiresEventTraversal() const { return m_eventCallback.valid() || (m_stateset.valid() && m_stateset->requiresEventTraversal()); }

    struct CullCallback : public virtual crBase
    {
	    CullCallback() {}

	    CullCallback(const CullCallback&,const crCopyOp&) {}
        META_Base(CRCore,CullCallback);
        /** do customized cull code.*/
		virtual bool cull(crNodeVisitor *visitor, crDrawable* drawable, crState *state=NULL ) const { return false; };
    };

    void setCullCallback(CullCallback* cc) { m_cullCallback=cc; }
    CullCallback* getCullCallback() { return m_cullCallback.get(); }
    const CullCallback* getCullCallback() const { return m_cullCallback.get(); }

    /** Callback attached to an crDrawable which allows the users to customize the drawing of an exist crDrawable object.
      * The draw callback is implement as a replacement to the crDrawable's own drawImplementation() method, if the
      * the user intends to decorate the exist draw code then simple call the drawable->drawImplementation() from
      * with the callbacks drawImplementation() method.  This allows the users to do both pre and post callbacks
      * without fuss and can even diable the inner draw in required.*/
    struct DrawCallback : public Referenced
    {
		DrawCallback(){}
		//virtual void compileImplementation( const CRCore::crDrawable* pdrawable, crState& state ) const = 0;
	    virtual void drawImplementation( const CRCore::crDrawable* pdrawable, crState& state ) const = 0;
	    //virtual void releaseImplementation( const CRCore::crDrawable* pdrawable, crState* state ) const = 0;
    };

    virtual void setDrawCallback(DrawCallback* dc) { m_drawCallback=dc;}
    DrawCallback* getDrawCallback() { return m_drawCallback.get(); }
    const DrawCallback* getDrawCallback() const { return m_drawCallback.get(); }


    /** draw directly ignoring an OpenGL display list which could be attached.
      * This is the internal draw method which does the drawing itself,
      * and is the method to override when deriving from crDrawable.
      */
	virtual void drawImplementation( crState& state ) const{};
	void compileImplementation( crState& state ) const{};
	//void releaseImplementation( crState& state ) const;
	/** Set the crStateSet attached to the crDrawable.
	Previously attached crStateSet are automatically unreferenced on 
	assignment of a new drawstate.*/
	inline virtual void setStateSet(crStateSet *state);

	/** Get the attached crStateSet.*/
	inline crStateSet* getStateSet() { return m_stateset.get();}

	/** Get the attached const crStateSet.*/
	inline const crStateSet* getStateSet() const { return m_stateset.get();}

	/** Get the attached const crStateSet, 
	* if one is not already attach create one, 
	* attach it to the drawable and return a pointer to it.*/
	crStateSet* getOrCreateStateSet();

	//void setStateSetOptimizerd(bool stateSetOptimizerd) { m_stateset_optimizerd = stateSetOptimizerd; }
 //   void setTextureStateSet(crStateSet *ss) { m_textures_stateset = stateSetOptimizerd; }
 //   void setOthersStateSet(crStateSet *ss) { m_others_stateset = stateSetOptimizerd; }
 //   void setMaterialStateSet(crStateSet *ss) { m_material_stateset = stateSetOptimizerd; }
	//inline getStateSetOptimizerd(){ return m_stateset_optimizerd; }
	//inline crStateSet* getTextureStateSet(){ return m_textures_stateset.get(); }
	//inline crStateSet* getOthersStateSet(){ return m_others_stateset.get(); }
	//inline crStateSet* getMaterialStateSet(){ return m_material_stateset.get(); }

	/** Set the Shape of the \c Drawable. The shape can be used to
	*  speed up collision detection or as a guide for procedural
	*  geometry generation.
	* @see osg::Shape.
	*/
	inline void setShape(crShape* shape) { m_shape = shape; }

	/** Get the Shape of the Drawable.*/
	inline crShape* getShape() { return m_shape.get(); }

	/** Get the const Shape of the const Drawable.*/
	inline const crShape* getShape() const { return m_shape.get(); }

	/** Set the drawable to it can or cannot be used in conjunction with OpenGL
	* display lists.  With set to true, calls to crDrawable::setUseDisplayList,
	* whereas when set to false, no display lists can be created and calls
	* to setUseDisplayList are ignored, and a warning is produced.  The later
	* is typically used to guard against the switching on of display lists
	* on objects with dynamic internal data such as continuous Level of Detail
	* algorithms.*/
	void setSupportsDisplayList(bool flag);

	/** Get whether display lists are supported for this drawable instance.*/  
	inline bool getSupportsDisplayList() const { return m_supportsDisplayList; }

	/** Set the minimum number of display lists to retain in the deleted display list cache. */
	static void setMinimumNumberOfDisplayListsToRetainInCache(unsigned int minimum);

	/** Get the minimum number of display lists to retain in the deleted display list cache. */
	static unsigned int getMinimumNumberOfDisplayListsToRetainInCache();

	/** When set to true, force the draw method to use OpenGL Display List for rendering.
	If false rendering directly.  If the display list has not been already
	compile the next call to draw will automatically create the display list.*/
	void setUseDisplayList(bool flag);

	/** Return whether OpenGL display lists are being used for rendering.*/
	inline bool getUseDisplayList() const { return m_useDisplayList; }

	//void setSupportsVertexBufferObjects(bool flag){ m_supportsVertexBufferObjects = flag; }

	inline bool getSupportsVertexBufferObjects() const { return m_supportsVertexBufferObjects; }

	/** When set to true, ignore the setUseDisplayList() settings, and hints to the drawImplemention 
	method to use OpenGL vertex buffer objects for rendering..*/
	void setUseVertexBufferObjects(bool flag);

	/** Return whether OpenGL vertex buffer objects should be used when supported by OpenGL driver.*/
    inline bool getUseVertexBufferObjects() const { return m_useVertexBufferObjects; }

	void setUseIndexBufferObjects(bool flag);

	/** Return whether OpenGL vertex buffer objects should be used when supported by OpenGL driver.*/
	inline bool getUseIndexBufferObjects() const { return m_useIndexBufferObjects; }

	/** Force a recompile on next draw() of any OpenGL display list associated with this geoset.*/
	void dirtyDisplayList();

	/** use deleteDisplayList instead of glDeleteList to allow
	* OpenGL display list to cached until they can be deleted
	* by the OpenGL context in which they were created, specified
	* by contextID.*/
    static void deleteDisplayList(unsigned int contextID,GLuint globj, unsigned int sizeHint = 0);
	
	virtual unsigned int getObjectSizeHint() const { return 0; }

	/** use deleteVertexBufferObject instead of glDeleteList to allow
	* OpenGL buffer objects to cached until they can be deleted
	* by the OpenGL context in which they were created, specified
	* by contextID.*/
	static void deleteVertexBufferObject(unsigned int contextID,unsigned int globj);

	/** Immediately compile this drawable into an OpenGL Display List.
	Note I, operation is ignored if _useDisplayList to false.
	Note II, compile is not intended to be overridden in subclasses.*/
	void compile(crState& state) const;

	/** If State is non-zero, this function releases OpenGL objects for
	* the specified graphics context. Otherwise, releases OpenGL objexts
	* for all graphics contexts. */
	virtual void releaseObjects(crState* state=0);

	/** draw OpenGL primitives. 
	* If the drawable has _useDisplayList set to true then use an OpenGL display
	* list, automatically compiling one if required.
	* Otherwise call drawImplementation().
	* Note, draw method should *not* be overridden in subclasses as it 
	* manages the optional display list.
	*/
	inline void draw(crState& state) const;
	//inline virtual void draw(crState& state);

	typedef unsigned int AttributeType;

	enum AttributeTypes
	{
		VERTICES            = 0,
		WEIGHTS             = 1,
		NORMALS             = 2,
		COLORS              = 3,
		SECONDARY_COLORS    = 4,
		FOG_COORDS          = 5,
		ATTIBUTE_6          = 6,
		ATTIBUTE_7          = 7,
		TEXTURE_COORDS      = 8,
		TEXTURE_COORDS_0    = TEXTURE_COORDS,
		TEXTURE_COORDS_1    = TEXTURE_COORDS_0+1,
		TEXTURE_COORDS_2    = TEXTURE_COORDS_0+2,
		TEXTURE_COORDS_3    = TEXTURE_COORDS_0+3,
		TEXTURE_COORDS_4    = TEXTURE_COORDS_0+4,
		TEXTURE_COORDS_5    = TEXTURE_COORDS_0+5,
		TEXTURE_COORDS_6    = TEXTURE_COORDS_0+6,
		TEXTURE_COORDS_7    = TEXTURE_COORDS_0+7 
		// only eight texture coord examples provided here, but underlying code can handle any no of texure units,
		// simply co them as (TEXTURE_COORDS_0+unit).
	};

	class AttributeFunctor
	{
	public:

		virtual ~AttributeFunctor() {}

		virtual void apply(AttributeType,unsigned int,signed char*) {}
		virtual void apply(AttributeType,unsigned int,short*) {}
		virtual void apply(AttributeType,unsigned int,signed int*) {}

		virtual void apply(AttributeType,unsigned int,unsigned char*) {}
		virtual void apply(AttributeType,unsigned int,unsigned short*) {}
		virtual void apply(AttributeType,unsigned int,unsigned int*) {}

		virtual void apply(AttributeType,unsigned int,float*) {}
		virtual void apply(AttributeType,unsigned int,crVector2*) {}
		virtual void apply(AttributeType,unsigned int,crVector3*) {}
		virtual void apply(AttributeType,unsigned int,crVector4*) {}
		virtual void apply(AttributeType,unsigned int,crVector4ub*) {}
	};

	/** return true if the crDrawable subclass supports accept(AttributeFunctor&).*/
	virtual bool supports(AttributeFunctor&) const { return false; }

	/** accept an AttributeFunctor and call its methods to tell it about the interal attributes that this crDrawable has.
	* return true if functor handled by drawable, return false on failure of drawable to generate functor calls.*/
	virtual void accept(AttributeFunctor&) {}

	class ConstAttributeFunctor
	{
	public:

		virtual ~ConstAttributeFunctor() {}

		virtual void apply(AttributeType,const unsigned int,const signed char*) {}
		virtual void apply(AttributeType,const unsigned int,const short*) {}
		virtual void apply(AttributeType,const unsigned int,const int*) {}

		virtual void apply(AttributeType,const unsigned int,const unsigned char*) {}
		virtual void apply(AttributeType,const unsigned int,const unsigned short*) {}
		virtual void apply(AttributeType,const unsigned int,const unsigned int*) {}

		virtual void apply(AttributeType,const unsigned int,const float*) {}
		virtual void apply(AttributeType,const unsigned int,const crVector2*) {}
		virtual void apply(AttributeType,const unsigned int,const crVector3*) {}
		virtual void apply(AttributeType,const unsigned int,const crVector4*) {}
		virtual void apply(AttributeType,const unsigned int,const crVector4ub*) {}
	};

	/** return true if the crDrawable subclass supports accept(ConstAttributeFunctor&).*/
	virtual bool supports(ConstAttributeFunctor&) const { return false; }

	/** accept an AttributeFunctor and call its methods to tell it about the interal attributes that this crDrawable has.
	* return true if functor handled by drawable, return false on failure of drawable to generate functor calls.*/
	virtual void accept(ConstAttributeFunctor&) const {}

	class PrimitiveFunctor
	{
	public:

		virtual ~PrimitiveFunctor() {}

		virtual void setVertexArray(unsigned int count,const crVector2* vertices){};
		virtual void setVertexArray(unsigned int count,const crVector3* vertices){};
		virtual void setVertexArray(unsigned int count,const crVector4* vertices){};

		virtual void drawArrays(unsigned int mode,int first,int count){};
		virtual void drawElements(unsigned int mode,int count,const unsigned char* indices){};
		virtual void drawElements(unsigned int mode,int count,const unsigned short* indices){};
		virtual void drawElements(unsigned int mode,int count,const unsigned int* indices){};

		virtual void begin(unsigned int mode){};
		virtual void vertex(const crVector2& vert){};
		virtual void vertex(const crVector3& vert){};
		virtual void vertex(const crVector4& vert){};
		virtual void vertex(float x,float y){};
		virtual void vertex(float x,float y,float z){};
		virtual void vertex(float x,float y,float z,float w){};
		virtual void end(){};

	};

	class PrimitiveIndexFunctor
	{
	public:

		virtual ~PrimitiveIndexFunctor() {}

		virtual void setVertexArray(unsigned int count,const crVector2f* vertices){};
		virtual void setVertexArray(unsigned int count,const crVector3f* vertices){};
		virtual void setVertexArray(unsigned int count,const crVector4f* vertices){};

		virtual void drawArrays(unsigned int mode,int first,int count){};
		virtual void drawElements(unsigned int mode,int count,const unsigned char* indices){};
		virtual void drawElements(unsigned int mode,int count,const unsigned short* indices){};
		virtual void drawElements(unsigned int mode,int count,const unsigned int* indices){};

		virtual void begin(unsigned int mode){};
		virtual void vertex(unsigned int pos){};
		virtual void end(){};

	};

	/** return true if the crDrawable subclass supports accept(PrimitiveFunctor&).*/
	virtual bool supports(PrimitiveFunctor&) const { return false; }

	/** accept a PrimtiveFunctor and call its methods to tell it about the interal primtives that this crDrawable has.
	* return true if functor handled by drawable, return false on failure of drawable to generate functor calls.
	* Note, PrimtiveFunctor only provide const access of the primtives, as primitives may be procedurally generated
	* so one cannot modify it.*/
	virtual void accept(PrimitiveFunctor&) const {}

	/** return true if the Drawable subclass supports accept(PrimitiveIndexFunctor&).*/
	virtual bool supports(PrimitiveIndexFunctor&) const { return false; }

	/** accept a PrimtiveIndexFunctor and call its methods to tell it about the interal primtives that this Drawable has.
	* return true if functor handled by drawable, return false on failure of drawable to generate functor calls.
	* Note, PrimtiveIndexFunctor only provide const access of the primtives, as primitives may be procedurally generated
	* so one cannot modify it.*/
	virtual void accept(PrimitiveIndexFunctor&) const {}

	//// static cache of deleted display lists which can only 
	//// by completely deleted once the appropriate OpenGL context
	//// is set.  Used CRCore::crDrawable::deleteDisplayList(..) and flushDeletedDisplayLists(..) below.
	//typedef std::list<unsigned int> DisplayListList;
	//typedef std::map<unsigned int,DisplayListList> DeletedDisplayListCache;
	//static DeletedDisplayListCache s_deletedDisplayListCache;
	//static DeletedDisplayListCache s_deletedVertexBufferObjectCache;

	typedef CRCore::buffered_value<unsigned int> GLObjectList;

	GLObjectList &getGLObjectList() const{ return m_globjList; }
    GLObjectList &getGLVboList() const{ return m_vboList; }	
	
	//inline void setCulled( unsigned char culled ){ m_culled = culled; }
	//inline unsigned char getCulled() { return m_culled; }
	//inline unsigned char getCulled() const { return m_culled; }

	virtual CRCore::crGeometry *getPuppetGeometry(){ return 0; };
	virtual CRCore::crGeometry *getPuppetBufGeometry(){ return 0; };

	inline void setVisiable( bool visiable ){ m_visiable = visiable; }
	inline bool getVisiable()const{ return m_visiable; }

	inline virtual void setText(const std::string& text){};//对于drawable是无意义的
	inline virtual void setText(const wchar_t* text){};//对于drawable是无意义的
	inline virtual void setPosition(const CRCore::crVector3& position){};//对于drawable是无意义的
	inline virtual const CRCore::crVector3& getPosition() const { return m_bbox.m_max; }//对于drawable是无意义的

	inline virtual void swapBuffers(int frameNumber)
	{
		if(m_swapFrameNumber != frameNumber)
		{
			if(m_stateset.valid()) m_stateset->swapBuffers(frameNumber);
			m_swapFrameNumber = frameNumber;
		}
	}

	inline void setColor(const crVector4& color) { m_color = color; }
	inline void setColor(const crVector4& color)const { m_color = color; }
	inline crVector4& getColor(){ return m_color; }
	inline bool isAbortCurrentRendering() { return m_abortCurrentRendering; }
	inline void resumeRendering() { m_abortCurrentRendering = false; }
	static void flushAllDeletedDisplayLists(unsigned int contextID);
	static void flushDeletedDisplayLists(unsigned int contextID,double& availableTime);
	static void flushDeletedVertexBufferObjects(unsigned int contextID,double currentTime, double& availableTime);
	static void flushAllDeletedVertexBufferObjects(unsigned int contextID);
  protected:
	 crDrawable& operator = (const crDrawable&) { return *this; }

    /** compute the bounding box of the drawable. Method must be 
    implemented by subclasses.*/
    virtual bool computeBound() const;

    friend class crNode;
    friend class crObject;
	friend class crStateSet;

    Type                    m_type;

	ParentList              m_parents;;
    mutable crBoundingBox   m_bbox;
    mutable bool            m_bbox_computed;

    ref_ptr<crShape>        m_shape;

    bool                    m_supportsDisplayList;
    bool                    m_useDisplayList;
    bool                    m_supportsVertexBufferObjects;
    bool                    m_useVertexBufferObjects;
	bool                    m_useIndexBufferObjects;

	bool                    m_visiable;

    mutable GLObjectList    m_globjList;
    mutable GLObjectList    m_vboList;

    ref_ptr<UpdateCallback> m_updateCallback;
    ref_ptr<DrawCallback>   m_drawCallback;
    ref_ptr<CullCallback>   m_cullCallback;

	unsigned int m_numChildrenRequiringUpdateTraversal;
	void setNumChildrenRequiringUpdateTraversal(unsigned int num);
	unsigned int getNumChildrenRequiringUpdateTraversal() const { return m_numChildrenRequiringUpdateTraversal; }

	//ref_ptr<UpdateCallback> m_updateUniformCallback;
	//unsigned int m_numChildrenRequiringUpdateUniformTraversal;
	//void setNumChildrenRequiringUpdateUniformTraversal(unsigned int num);
	//unsigned int getNumChildrenRequiringUpdateUniformTraversal() const { return m_numChildrenRequiringUpdateUniformTraversal; }

	//ref_ptr<EventCallback>  m_eventCallback;
	//unsigned int m_numChildrenRequiringEventTraversal;
	//void setNumChildrenRequiringEventTraversal(unsigned int num);
	//unsigned int getNumChildrenRequiringEventTraversal() const { return m_numChildrenRequiringEventTraversal; }

	bool m_stateset_optimizerd;
    ref_ptr<crStateSet>     m_stateset;
	//ref_ptr<crStateSet>     m_textures_stateset;////优化的stateset 从m_stateset解析出来
	//ref_ptr<crStateSet>     m_others_stateset;////优化的stateset 从m_stateset解析出来
	//ref_ptr<crStateSet>     m_material_stateset;////优化的stateset 从m_stateset解析出来

    //unsigned char m_culled;//－1：初始值，0：没有被剪裁  1：被剪裁
	mutable crVector4 m_color;
	bool m_abortCurrentRendering;
  };

  //inline void crDrawable::draw(crState& state) const
  //{	  
	 // // draw object as nature intended..
	 // if (m_drawCallback.valid())
		//  m_drawCallback->drawImplementation( this, state );
	 // else
		//  drawImplementation(state);
  //};

  //inline void crDrawable::compile(crState& state) const
  //{
	 // if (!m_useDisplayList && !m_useVertexBufferObjects && !m_useIndexBufferObjects) return;
	 // // draw object as nature intended..
	 // if (m_drawCallback.valid())
		//  m_drawCallback->compileImplementation( this, state );
	 // else
		//  compileImplementation(state);
  //};
  //inline void crDrawable::releaseObjects(crState* state)
  //{
	 // if (m_stateset.valid()) m_stateset->releaseObjects(state);

	 // if (!m_useDisplayList&&!m_useVertexBufferObjects) return;

	 // if(!state)
	 //     const_cast<crDrawable*>(this)->dirtyDisplayList();
	 // else
	 // {
	 //  //   if (m_drawCallback.valid())
		//  //    m_drawCallback->releaseImplementation( this, state );
		//  //else
		//	  releaseImplementation(state);
	 // }
  //    m_globjList.clear();
	 // m_vboList.clear();
	 // m_abortCurrentRendering = true;
  //};

}

#endif
