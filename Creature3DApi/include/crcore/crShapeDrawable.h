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
//Modified by 吴财华
#ifndef CRCORE_SHAPEDRAWABLE
#define CRCORE_SHAPEDRAWABLE 1

#include <CRCore/crDrawable.h>
#include <CRCore/crVector2.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crArray.h>
#include <CRCore/crPrimitive.h>
#include <CRCore/crShape.h>

namespace CRCore {

class crTessellationHints : public crBase
{
public:

	crTessellationHints():
	    m_TessellationMode(USE_SHAPE_DEFAULTS),
        m_detailRatio(1.0f),
	    m_targetNumFaces(100),
	    m_createFrontFace(true),
	    m_createBackFace(false),
	    m_createNormals(true),
	    m_createTextureCoords(false),
	    m_createTop(true),
	    m_createBody(true),
	    m_createBottom(true) {}
	    

	crTessellationHints(const crTessellationHints& tess, const crCopyOp& copyop=crCopyOp::SHALLOW_COPY): 
    	crBase(tess,copyop),
	    m_TessellationMode(tess.m_TessellationMode),
        m_detailRatio(tess.m_detailRatio),
	    m_targetNumFaces(tess.m_targetNumFaces),
	    m_createFrontFace(tess.m_createFrontFace),
	    m_createBackFace(tess.m_createBackFace),
	    m_createNormals(tess.m_createNormals),
	    m_createTextureCoords(tess.m_createTextureCoords),
	    m_createTop(tess.m_createTop),
	    m_createBody(tess.m_createBody),
	    m_createBottom(tess.m_createBottom) {}

	META_Base(CRCore,crTessellationHints);
	
	
	enum TessellationMode
	{
	    USE_SHAPE_DEFAULTS,
    	USE_TARGET_NUM_FACES
	};

    inline void setTessellationMode(TessellationMode mode) { m_TessellationMode=mode; }
    inline TessellationMode getTessellationMode() const { return m_TessellationMode; }

    inline void setDetailRatio(float ratio) { m_detailRatio = ratio; }
    inline float getDetailRatio() const { return m_detailRatio; }

	inline void setTargetNumFaces(unsigned int target) { m_targetNumFaces=target; }
	inline unsigned int getTargetNumFaces() const { return m_targetNumFaces; }

    inline void setCreateFrontFace(bool on) { m_createFrontFace=on; }
    inline bool getCreateFrontFace() const { return m_createFrontFace; }

    inline void setCreateBackFace(bool on) { m_createBackFace=on; }
    inline bool getCreateBackFace() const { return m_createBackFace; }

    inline void setCreateNormals(bool on) { m_createNormals=on; }
    inline bool getCreateNormals() const { return m_createNormals; }

    inline void setCreateTextureCoords(bool on) { m_createTextureCoords=on; }
    inline bool getCreateTextureCoords() const { return m_createTextureCoords; }

    inline void setCreateTop(bool on) { m_createTop=on; }
    inline bool getCreateTop() const { return m_createTop; }

    inline void setCreateBody(bool on) { m_createBody=on; }
    inline bool getCreateBody() const { return m_createBody; }

    inline void setCreateBottom(bool on) { m_createBottom=on; }
    inline bool getCreateBottom() const { return m_createBottom; }

protected:
    
    ~crTessellationHints() {}

    
    TessellationMode m_TessellationMode;

    float           m_detailRatio;
    unsigned int 	m_targetNumFaces;

	bool 	    	m_createFrontFace;
	bool 	    	m_createBackFace;
	bool 	    	m_createNormals;
	bool 	    	m_createTextureCoords;

	bool 	    	m_createTop;
	bool 	    	m_createBody;
	bool 	    	m_createBottom;

};

class CR_EXPORT crShapeDrawable : public crDrawable
{
    public:

        crShapeDrawable();

        crShapeDrawable(crShape* shape,crTessellationHints* hints=0);

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crShapeDrawable(const crShapeDrawable& pg,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);
            
        virtual crBase* cloneType() const { return new crShapeDrawable(); }
        virtual crBase* clone(const crCopyOp& copyop) const { return new crShapeDrawable(*this,copyop); }        
        virtual bool isSameKindAs(const crBase* obj) const { return dynamic_cast<const crShapeDrawable*>(obj)!=NULL; }
        virtual const char* libraryName() const { return "CRCore"; }
        virtual const char* className() const { return "crShapeDrawable"; }

    	void setTessellationHints(crTessellationHints* hints) { m_tessellationHints = hints; }
        
    	crTessellationHints* getTessellationHints() { return m_tessellationHints.get(); }
    	const crTessellationHints* getTessellationHints() const { return m_tessellationHints.get(); }


        /** return false, CRCore::crShapeDrawable does not support accept(AttributeFunctor&).*/
        virtual bool supports(AttributeFunctor&) const { return false; }

        /** return true, CRCore::crShapeDrawable does support accept(ConstAttributeFunctor&).*/
        virtual bool supports(ConstAttributeFunctor&) const { return true; }

        /** accept an ConstAttributeFunctor and call its methods to tell it about the interal attributes that this crDrawable has.*/
        virtual void accept(ConstAttributeFunctor& af) const;

        /** return true, CRCore::crShapeDrawable does support accept(PrimitiveFunctor&) .*/
        virtual bool supports(PrimitiveFunctor&) const { return true; }

        /** accept a PrimtiveFunctor and call its methods to tell it about the interal primtives that this crDrawable has.*/
        virtual void accept(PrimitiveFunctor& pf) const;

		/** Set the crShape of the drawable. The shape can be used to
		* speed up collision detection or as a guide for produral
		* geometry generation - see CRCore::ProduralGeometry.*/
		inline void setShape(crShape* shape) { m_shape = shape; }

		/** Get the crShape of the Drawable.*/
		inline crShape* getShape() { return m_shape.get(); }

		/** Get the const crShape of the const Drawable.*/
		inline const crShape* getShape() const { return m_shape.get(); }
		virtual void drawImplementation(crState& state) const;
    protected:

        crShapeDrawable& operator = (const crShapeDrawable&) { return *this;}

        virtual ~crShapeDrawable();
       
        virtual bool computeBound() const;

	    ref_ptr<crTessellationHints> m_tessellationHints;
		ref_ptr<crShape>        m_shape;

};


}

#endif
