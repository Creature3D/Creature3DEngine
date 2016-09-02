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
#ifndef CRCORE_STATEATTRIBUTE_H
#define CRCORE_STATEATTRIBUTE_H 1

#include <crgl/gl.h>
#include <crgl/glext.h>
#include <CRCore/crExport.h>
#include <CRCore/crBase.h>

#include <typeinfo>
#include <vector>
#include <map>

namespace CRCore {

class crNodeVisitor;
class crState;
class crStateSet;

#define META_StateAttribute(library,name,type) \
        virtual CRCore::crBase* cloneType() const { return new name(); } \
        virtual CRCore::crBase* clone(const CRCore::crCopyOp& copyop) const { return new name (*this,copyop); } \
        virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const name *>(obj)!=NULL; } \
        virtual const char* libraryName() const { return #library; } \
        virtual const char* className() const { return #name; } \
        virtual Type getType() const { return type; }
		//virtual name *as##name() { return dynamic_cast<name *>(this); }\
		//virtual const name *as##name() const { return dynamic_cast<const name *>(this); }

/** COMPARE_StateAttribute_Types macro is a helper for implementing the StatateAtribute::compare(..) method.*/
#define COMPARE_StateAttribute_Types(TYPE,rhs_attribute) \
            if (this==&rhs_attribute) return 0;\
            const std::type_info* type_lhs = &typeid(*this);\
            const std::type_info* type_rhs = &typeid(rhs_attribute);\
            if (type_lhs->before(*type_rhs)) return -1;\
            if (*type_lhs != *type_rhs) return 1;\
            const TYPE& rhs = static_cast<const TYPE&>(rhs_attribute);


/** COMPARE_StateAttribute_Parameter macro is a helper for implementing the StatateAtribute::compare(..) method.
  * Macro assumes that variable rhs has been corrected defined by code preceesing
  * macro.*/
#define COMPARE_StateAttribute_Parameter(parameter) \
        if (parameter<rhs.parameter) return -1; \
        if (rhs.parameter<parameter) return 1;

#define META_SetAttribute(library,name,type) \
	virtual CRCore::crStateAttribute::DrawCallback* cloneType() const { return new name(); } \
	virtual const char* libraryName() const { return #library; } \
	virtual const char* className() const { return #name; } \
	virtual CRCore::crStateAttribute::Type getType() const { return type; }

// define for the GL_EXT_secondary_color extension, GL_COLOR_SUM is OpenGL
// mode to be used to enable and disable the off the second color.
/*#ifndef GL_COLOR_SUM
#define GL_COLOR_SUM    0x8458
#endif
*/
/** Base class for state attribuets.
*/     
class CR_EXPORT crStateAttribute : public crBase
{
    public :
    
//	    class CR_EXPORT VRMode 
//		{
//		public:
//			unsigned int GLMode;
//			unsigned int D3DMode;
//			VRMode( unsigned int glmode = NULL, unsigned int d3dmode = NULL ):GLMode(glmode),D3DMode(d3dmode) {}
//			inline VRMode& operator = (const VRMode& mode) { GLMode=mode.GLMode; D3DMode=mode.D3DMode; return(*this); }
//			inline VRMode& operator = (unsigned int glmode) { GLMode=glmode; D3DMode=0; return(*this); }
//
//			inline bool operator == (const VRMode& mode) const { return GLMode==mode.GLMode/* && D3DMode==mode.D3DMode*/; }
//
//			inline bool operator != (const VRMode& mode) const { return GLMode!=mode.GLMode/* || D3DMode!=mode.D3DMode*/; }
//
//            inline bool operator <  (const VRMode& mode) const
//			{
//                if (GLMode<mode.GLMode) return true;
///*                else if (GLMode>mode.GLMode)*/ return false;
//              /*  else return (D3DMode<mode.D3DMode);*/
//			}
//			inline bool operator >  (const VRMode& mode) const
//			{
//				return (!(*this<mode)&&(*this!=mode));
//			}
//	    }; 
        /** VRMode is the value used in glEnable/glDisable(mode) */
        typedef unsigned int    VRMode;
        /** VRModeValue is used to specified whether an mode is enabled (ON) or disabled (OFF).
          * VRMoveValue is also used to specify the override behavior of modes from parent to children. 
          * See enum Value description for more details.*/
        typedef unsigned int    VRModeValue;
        /** Override is used to specified the override behavior of StateAttributes
          * from from parent to children. 
          * See enum Value description for more details.*/
        typedef unsigned int    OverrideValue;

        /** list values which can be used in to set either GLModeValues
          * or OverrideValues. When using in conjunction with GLModeValues
          * all Values have meaning.  When using in conjection with 
          * crStateAttribute OverrideValue only OFF,OVERRIDE and INHERIT
          * are meaningful. However, they are useful when using VRModeValue 
          * and OverrideValue in conjunction with each other as when using
          * crStateSet::setAttributeAndModes(..).*/
        enum Values
        {
            /** means that associated VRMode and Override is disabled.*/
            OFF          = 0x0,
            /** means that associated VRMode is enabled and Override is disabled.*/
            ON           = 0x1,
            /** Overriding of VRMode's or StateAttributes is enabled, so that state below it is overriden.*/
            OVERRIDE     = 0x2,
            /** Protecting of VRMode's os StateAttributes is enabled, so that state from above connot override this and below state.*/
            PROTECTED    = 0x4,            
            /** means that VRMode or crStateAttribute should in inherited from above.*/
            INHERIT      = 0x8
        };
        
        /** Type identifier to differentiate between different state types. */
        typedef unsigned int Type;

        /** Values of crStateAttribute::Type used to aid identification
          * of different crStateAttribute subclasses. Each subclass defines
          * it own value in the virtual Type getType() method.  When 
          * extending the CRCore's crStateAttribute's simply define your
          * own Type value which is unique, using the crStateAttribute::Type
          * enum as a guide of what values to use.  If your new subclass
          * needs to override a standard StateAttriubte then simple use 
          * that types value. */
        enum Types
        {
			TEXTURE,

			POLYGONMODE,
			POLYGONOFFSET,
			MATERIAL,
			ALPHAFUNC,
			ANTIALIAS,
			COLORTABLE,
			CULLFACE,
			FOG,
			FRONTFACE,

			LIGHT0,
			LIGHT1,
			LIGHT2,
			LIGHT3,
			LIGHT4,
			LIGHT5,
			LIGHT6,
			LIGHT7,

			POINT,
			LINEWIDTH,
			LINESTIPPLE,
			POLYGONSTIPPLE,
			SHADEMODEL,
			TEXENV,
			TEXENVFILTER,
			TEXGEN,
			TEXMAT,
			LIGHTMODEL,
			BLENDFUNC,
			BLENDEQUATION,
			LOGICOP,
			STENCIL,
			COLORMASK,
			DEPTH,
			VIEWPORT,
			SCISSOR,
			BLENDCOLOR,
			MULTISAMPLE,
            HINT,

			CLIPPLANE0,
			CLIPPLANE1,
			CLIPPLANE2,
			CLIPPLANE3,
			CLIPPLANE4,
			CLIPPLANE5,

			COLORMATRIX,

			VERTEXPROGRAM,
			FRAGMENTPROGRAM,

			POINTSPRITE,

			/// core GLSL support
			PROGRAM,
			FBO,

			/// osgFX namespace
			VALIDATOR,
			VIEWMATRIXEXTRACTOR,

			/// osgNV namespace
			CRNV_PARAMETER_BLOCK,

			// osgNVExt namespace
			CRNVEXT_TEXTURE_SHADER,
			CRNVEXT_VERTEX_PROGRAM,
			CRNVEXT_REGISTER_COMBINERS,

			/// osgNVCg namespace
			CRNVCG_PROGRAM,

			// osgNVSlang namespace
			CRNVSLANG_PROGRAM,

			// osgNVParse
			CRNVPARSE_PROGRAM_PARSER,
			CLAMPCOLOR
        };


        crStateAttribute() { setDataVariance(STATIC); }
        
        crStateAttribute(const crStateAttribute& sa,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY): 
            crBase(sa,copyop) {}
        

        /** Clone the type of an attribute, with crBase* return type.
            Must be defined by derived classes.*/
        virtual crBase* cloneType() const = 0;

        /** Clone an attribute, with crBase* return type.
            Must be defined by derived classes.*/
        virtual crBase* clone(const crCopyOp&) const = 0;

        /** return true if this and obj are of the same kind of object.*/
		virtual bool isSameKindAs(const crBase* obj) const { return dynamic_cast<const crStateAttribute*>(obj)!=NULL; }

        /** return the name of the attribute's library.*/
        virtual const char* libraryName() const { return "CRCore"; }

        /** return the name of the attribute's class type.*/
        virtual const char* className() const { return "crStateAttribute"; }
        
        /** return the Type identifier of the attribute's class type.*/
        virtual Type getType() const = 0;

        /** return true if crStateAttribute is a type which controls texturing and needs to be issued w.r.t to specific texture unit.*/
        virtual bool isTextureAttribute() const { return false; }

        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const = 0;
		virtual int compare(const crStateAttribute& sa) = 0;

        bool operator <  (const crStateAttribute& rhs) const { return compare(rhs)<0; }
        bool operator == (const crStateAttribute& rhs) const { return compare(rhs)==0; }
        bool operator != (const crStateAttribute& rhs) const { return compare(rhs)!=0; }

		bool operator <  (const crStateAttribute& rhs) { return compare(rhs)<0; }
		bool operator == (const crStateAttribute& rhs) { return compare(rhs)==0; }
		bool operator != (const crStateAttribute& rhs) { return compare(rhs)!=0; }

        /** return the modes associated with this crStateSet*/        
        inline virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
        {
            // default to no VRMode's associated with use of the crStateAttribute.
        }
    
        /** apply the OpenGL state attributes. 
          * The global state for the current OpenGL context is passed
          * in to allow the crStateAttribute to obtain details on the
          * the current context and state.
          */
		virtual void apply(crState& state ) const = 0;
		//{
		//  if(m_drawCallback.valid())
		//    m_drawCallback->drawImplementation( this, state );
		//}

        /** default to nothing to compile - all state is applied immediately. */
        //virtual void compile(crState& state) const
		//{
		//	if(m_drawCallback.valid())
		//		m_drawCallback->compileImplementation( this, state );
		//}

		//inline virtual void releaseObjects(crState* state= 0)
		//{
		//	if(m_drawCallback.valid())
		//		m_drawCallback->releaseImplementation( this, state );
		//}

		//virtual bool getNeedCompile()const{ return false; }

  //      class DrawCallback : public virtual Referenced
  //      {
		//public:
		//	DrawCallback(){}
  //          /** Clone the type of an attribute, with crBase* return type.
	 //       Must be defined by derived classes.*/
		//	virtual DrawCallback* cloneType() const { return NULL; };

		//	/** return the name of the attribute's library.*/
		//	virtual const char* libraryName() const { return "CRCore"; }

		//	/** return the name of the attribute's class type.*/
		//	virtual const char* className() const { return "DrawCallback"; }

		//	/** return the Type identifier of the attribute's class type.*/
		//	virtual Type getType() const { return NULL; };

		//	virtual void drawImplementation( const crStateAttribute* sa, crState& ) const{};
		//	virtual void releaseImplementation( const crStateAttribute* sa, crState* ) const{};
		//	virtual void compileImplementation( const crStateAttribute* sa, crState& ) const{};

		//protected:
		//	~DrawCallback(){}
  //      };

  //      void setDrawCallback(DrawCallback* sa) { m_drawCallback=sa; }
  //      
  //      DrawCallback* getDrawCallback() { return m_drawCallback.get(); }
  //      
  //      const DrawCallback* getDrawCallback() const { return m_drawCallback.get(); }

		/** A vector of CRCore::crStateSet pointers which is used to store the parent(s) of this StateAttribute.*/
		typedef std::vector<crStateSet*> ParentList;

		/** Get the parent list of this StateAttribute. */
		inline const ParentList& getParents() const { return m_parents; }

		inline crStateSet* getParent(unsigned int i)  { return m_parents[i]; }
		/**
		* Get a single const parent of this StateAttribute.
		* @param i index of the parent to get.
		* @return the parent i.
		*/
		inline const crStateSet* getParent(unsigned int i) const  { return m_parents[i]; }

		/**
		* Get the number of parents of this StateAttribute.
		* @return the number of parents of this StateAttribute.
		*/
		inline unsigned int getNumParents() const { return m_parents.size(); }

		struct Callback : public virtual CRCore::crBase
		{
			Callback() {}

			Callback(const Callback&,const crCopyOp&) {}

			META_Base(CRCore,Callback);

			/** do customized update code.*/
			virtual void operator () (crStateAttribute*, crNodeVisitor*) {}
		};

		/** Set the UpdateCallback which allows users to attach customize the updating of an object during the update traversal.*/
		void setUpdateCallback(Callback* uc);

		/** Get the non const UpdateCallback.*/
		Callback* getUpdateCallback() { return m_updateCallback.get(); }

		/** Get the const UpdateCallback.*/
		const Callback* getUpdateCallback() const { return m_updateCallback.get(); }

		///** Set the EventCallback which allows users to attach customize the updating of an object during the Event traversal.*/
		//void setEventCallback(Callback* ec);

		///** Get the non const EventCallback.*/
		//Callback* getEventCallback() { return m_eventCallback.get(); }

		///** Get the const EventCallback.*/
		//const Callback* getEventCallback() const { return m_eventCallback.get(); }

		/** Check the modes associated with this StateAttribute are supported by current OpenGL drivers,
		* and if not set the associated mode in osg::State to be black listed/invalid.
		* Return true if all associated modes are valid.*/
		virtual bool checkValidityOfAssociatedModes(crState&) const
		{
			// default to no black listed GLMode's associated with use of the StateAttribute.
			return true;
		}

		virtual void addAttributeParam(int i, const std::string &str);

    protected:	
    
        virtual ~crStateAttribute() { m_parents.clear(); }

		void addParent(crStateSet* object);
		void removeParent(crStateSet* object);

		ParentList m_parents;
		friend class CRCore::crStateSet;

       // ref_ptr<DrawCallback>   m_drawCallback;
		ref_ptr<Callback>   m_updateCallback;
		//ref_ptr<Callback>   m_eventCallback;
};

}

#endif