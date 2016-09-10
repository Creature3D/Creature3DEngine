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
#ifndef CRCORE_CRSTATESET_H
#define CRCORE_CRSTATESET_H 1

#include <CRCore/crBase.h>
#include <CRCore/crStateAttribute.h>
#include <CRCore/ref_ptr.h>

#include <CRCore/crUniform.h>
#include <CRCore/crProgram.h>

#include <map>
#include <vector>
#include <string>

namespace CRCore {

class crNodeVisitor;
class CR_EXPORT crStateSet : public crBase
{
    public :
        crStateSet();
        crStateSet(const crStateSet&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        virtual crBase* cloneType() const { return new crStateSet(); }
        virtual crBase* clone(const crCopyOp& copyop) const { return new crStateSet(*this,copyop); }
        virtual bool isSameKindAs(const crBase* obj) const { return dynamic_cast<const crStateSet*>(obj)!=NULL; }
        virtual const char* libraryName() const { return "CRCore"; }
        virtual const char* className() const { return "crStateSet"; }

        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        int compare(const crStateSet& rhs,bool compareAttributeContents=false) const;
		int compare(const crStateSet& rhs,bool compareAttributeContents=false);

  //      bool operator <  (const crStateSet& rhs) const { return compare(rhs)<0; }
  //      bool operator == (const crStateSet& rhs) const { return compare(rhs)==0; }
  //      bool operator != (const crStateSet& rhs) const { return compare(rhs)!=0; }

		//bool operator <  (const crStateSet& rhs) { return compare(rhs)<0; }
		//bool operator == (const crStateSet& rhs) { return compare(rhs)==0; }
		//bool operator != (const crStateSet& rhs) { return compare(rhs)!=0; }

		/** A vector of osg::Object pointers which is used to store the parent(s) of this Stateset, the parents could be osg::Node or osg::Drawable.*/
		typedef std::vector<crBase*> ParentList;

		/** Get the parent list of this StateSet. */
		inline const ParentList& getParents() const { return m_parents; }

		/** Get the a copy of parent list of node. A copy is returned to 
		* prevent modification of the parent list.*/
		inline ParentList getParents() { return m_parents; }

		inline crBase* getParent(unsigned int i)  { return m_parents[i]; }
		/**
		* Get a single const parent of this StateSet.
		* @param i index of the parent to get.
		* @return the parent i.
		*/
		inline const crBase* getParent(unsigned int i) const  { return m_parents[i]; }

		/**
		* Get the number of parents of this StateSet.
		* @return the number of parents of this StateSet.
		*/
		inline unsigned int getNumParents() const { return m_parents.size(); }

        /** set all the modes to on or off so that it defines a 
            complete state, typically used for a default global state.*/
        void setGlobalDefaults();

		void clear();
		void reset();
        /** set all the modes to inherit, typically used to signify
            nodes which inherit all of their modes for the global state.*/
        void setAllToInherit();
        
       /** merge this stateset with stateset rhs, this overrides
          * the rhs if OVERRIDE is specified, otherwise rhs takes precedence.*/
        void merge(const crStateSet& rhs);

        /** a container to map GLModes to their respective GLModeValues.*/
        typedef std::map<crStateAttribute::VRMode,crStateAttribute::VRModeValue>  ModeList;

        /** set this crStateSet to contain specified VRMode and value.*/
        void setMode(crStateAttribute::VRMode mode, crStateAttribute::VRModeValue value);
        
        /** set this crStateSet to inherit specified VRMode type from parents.
          * has the effect of deleting any GlMode of specified type from crStateSet.*/
        void setModeToInherit(crStateAttribute::VRMode mode);

		void removeMode(crStateAttribute::VRMode mode);

        /** get specified VRModeValue for specified VRMode.
          * returns INHERIT if no VRModeValue is contained within crStateSet.*/
        crStateAttribute::VRModeValue getMode(crStateAttribute::VRMode mode) const;

        /** return the list of all GLModes contained in this crStateSet.*/
        inline ModeList& getModeList() { return m_modeList; }

        /** return the const list of all GLModes contained in this const crStateSet.*/
        inline const ModeList& getModeList() const { return m_modeList; }



        /** simple pairing between an attribute and its override flag.*/
        typedef std::pair<ref_ptr<crStateAttribute>,crStateAttribute::OverrideValue> RefAttributePair;
        /** a container to map StateAttribyte::Types to their respective RefAttributePair.*/
        typedef std::map<crStateAttribute::Type,RefAttributePair>                  AttributeList;

        /** set this crStateSet to contain specified attribute and override flag.*/
        void setAttribute(crStateAttribute *attribute, crStateAttribute::OverrideValue value=crStateAttribute::OFF);
        /** set this crStateSet to contain specified attribute and set the associated VRMode's to specified value.*/
        void setAttributeAndModes(crStateAttribute *attribute, crStateAttribute::VRModeValue value=crStateAttribute::ON);
		
		/** remove attribute of specified type from StateSet.*/
		void removeAttribute(crStateAttribute::Type type);
		/** remove attribute from StateSet.*/
		void removeAttribute(crStateAttribute *attribute);

		///** set this crStateSet to inherit specified attribute type from parents.
  //        * has the effect of deleting any state attributes of specified type from crStateSet.*/
  //      void setAttributeToInherit(crStateAttribute::Type type);
		//void setAttributeToInherit(crStateAttribute::Type type)const;

        /** get specified crStateAttribute for specified type.
          * returns NULL if no type is contained within crStateSet.*/
        crStateAttribute* getAttribute(crStateAttribute::Type type);

        /** get specified const crStateAttribute for specified type.
          * returns NULL if no type is contained within const crStateSet.*/
        const crStateAttribute* getAttribute(crStateAttribute::Type type) const;

        /** get specified RefAttributePair for specified type.
          * returns NULL if no type is contained within crStateSet.*/
        const RefAttributePair* getAttributePair(crStateAttribute::Type type) const;
        
        /** return the list of all crStateAttributes contained in this crStateSet.*/
        inline AttributeList& getAttributeList() { return m_attributeList; }

        /** return the const list of all crStateAttributes contained in this const crStateSet.*/
        inline const AttributeList& getAttributeList() const { return m_attributeList; }

        typedef std::vector<ModeList>       TextureModeList;

        /** set this crStateSet to contain specified VRMode and value.*/
        void setTextureMode(unsigned int unit,crStateAttribute::VRMode mode, crStateAttribute::VRModeValue value);
        ///** set this crStateSet to inherit specified VRMode type from parents.
        //  * has the effect of deleting any GlMode of specified type from crStateSet.*/
        //void setTextureModeToInherit(unsigned int unit,crStateAttribute::VRMode mode);
		/** Remove texture mode from StateSet.*/
		void removeTextureMode(unsigned int unit,crStateAttribute::VRMode mode);

        /** get specified VRModeValue for specified VRMode.
          * returns INHERIT if no VRModeValue is contained within crStateSet.*/
        crStateAttribute::VRModeValue getTextureMode(unsigned int unit,crStateAttribute::VRMode mode) const;

        /** return the list of all Texture related GLModes contained in this crStateSet.*/
        inline TextureModeList& getTextureModeList() { return m_textureModeList; }

        /** return the const list of all Texture related GLModes contained in this const crStateSet.*/
        inline const TextureModeList& getTextureModeList() const  { return m_textureModeList; }


        typedef std::vector<AttributeList>  TextureAttributeList;

        /** set this crStateSet to contain specified attribute and override flag.*/
        void setTextureAttribute(unsigned int unit,crStateAttribute *attribute, crStateAttribute::OverrideValue value=crStateAttribute::OFF);
     
		/** set this crStateSet to contain specified attribute and set the associated VRMode's to specified value.*/
        void setTextureAttributeAndModes(unsigned int unit,crStateAttribute *attribute, crStateAttribute::VRModeValue value=crStateAttribute::ON);
 
		///** set this crStateSet to inherit specified attribute type from parents.
  //        * has the effect of deleting any state attributes of specified type from crStateSet.*/
  //      void setTextureAttributeToInherit(unsigned int unit,crStateAttribute::Type type);
		//void setTextureAttributeToInherit(unsigned int unit,crStateAttribute::Type type)const;

		/** remove texture attribute of specified type from StateSet.*/
		void removeTextureAttribute(unsigned int unit, crStateAttribute::Type type);

		/** remove texture attribute from StateSet.*/
		void removeTextureAttribute(unsigned int unit, crStateAttribute *attribute);

        /** get specified Texture related crStateAttribute for specified type.
          * returns NULL if no type is contained within crStateSet.*/
        crStateAttribute* getTextureAttribute(unsigned int unit,crStateAttribute::Type type);

        /** get specified Texture related const crStateAttribute for specified type.
          * returns NULL if no type is contained within const crStateSet.*/
        const crStateAttribute* getTextureAttribute(unsigned int unit,crStateAttribute::Type type) const;

        /** get specified Texture related RefAttributePair for specified type.
          * returns NULL if no type is contained within crStateSet.*/
        const RefAttributePair* getTextureAttributePair(unsigned int unit,crStateAttribute::Type type) const;
        
        /** return the list of all Texture related crStateAttributes contained in this crStateSet.*/
        inline TextureAttributeList& getTextureAttributeList() { return m_textureAttributeList; }

        /** return the const list of all Texture related crStateAttributes contained in this const crStateSet.*/
        inline const TextureAttributeList& getTextureAttributeList() const { return m_textureAttributeList; }

        void setAssociatedModes(const crStateAttribute* attribute, crStateAttribute::VRModeValue value);
        
        void setAssociatedTextureModes(unsigned int unit,const crStateAttribute* attribute, crStateAttribute::VRModeValue value);


		/** Simple pairing between a Uniform and its override flag.*/
		typedef std::pair<ref_ptr<crUniform>,crStateAttribute::OverrideValue>  RefUniformPair;

		/** a container to map Uniform name to its respective RefUniformPair.*/
		typedef std::map<std::string,RefUniformPair> UniformList;
		typedef std::map< std::string,ref_ptr<crUniform> > DirtyUniformList;

		/** Set this StateSet to contain specified uniform and override flag.*/
		void addUniform(crUniform* uniform, crStateAttribute::OverrideValue value=crStateAttribute::ON);

		/** remove uniform of specified name from StateSet.*/
		void removeUniform(const std::string& name);

		/** remove Uniform from StateSet.*/
		void removeUniform(crUniform* uniform);

		/** Get Uniform for specified name.
		* Returns NULL if no matching Uniform is contained within StateSet.*/
		crUniform* getUniform(const std::string& name);

		/** Get Uniform for specified name, if one is not available create it, add it to this StateSet and return a pointer to it.*/
		crUniform* getOrCreateUniform(const std::string& name, crUniform::Type type);

		/** Get const Uniform for specified name.
		* Returns NULL if no matching Uniform is contained within StateSet.*/
		const crUniform* getUniform(const std::string& name) const;

		/** Get specified RefUniformPair for specified Uniform name.
		* Returns NULL if no Uniform is contained within crStateSet.*/
		const RefUniformPair* getUniformPair(const std::string& name) const;

		/** set the list of all Uniforms contained in this crStateSet.*/
		inline void setUniformList(UniformList& al) { m_uniformList=al; }

		/** return the list of all Uniforms contained in this StateSet.*/
		inline UniformList& getUniformList() { return m_uniformList; }

		/** return the const list of all Uniforms contained in this const crStateSet.*/
		inline const UniformList& getUniformList() const { return m_uniformList; }

		enum RenderBinNum
		{
			ClearNode = -20,//StateSortedBin
			MirrorFirst = -9,//StateSortedBin
			MirrorSecond = -8,//StateSortedBin
			MirrorReflect = -7,//RenderBin
			MirrorEnd = -6,//StateSortedBin
			Opaque = 0,//RenderBin
			ForMultiRender = 1,//StateSortedBin
			Sky = 2,
			AlphaTest = 5,//RenderBin
			PreTrans = 9,
			Trans = 10,//DepthSortedBin
			PostTrans = 11,
			OutLine = 6,
			FX = 7,
			UI = 20,//RenderBin
			GridDraw = 30,//StateSortedBin
		};

        enum RenderingHint
        {
            DEFAULT_BIN = 0,
            OPAQUE_BIN = 1,
            TRANSPARENT_BIN = 2,
			ALPHATEST_BIN = 3,
            CLEARNODE_BIN = 4,
			MIRRORFIRST_BIN = 5,
			MIRRORSECOND_BIN = 6,
			MIRRORREFLECT_BIN = 7,
			MIRROREND_BIN = 8,
			MULTIRENDER_BIN = 9,
			SKY_BIN = 10,
			GRIDDRAW_BIN = 11,
			UI_BIN = 12,
			PRETRANS_BIN = 13,
			POSTTRANS_BIN = 14,
			OUTLINE_BIN = 15,
			FX_BIN = 16,
        };

        /** set the RenderingHint of the crStateSet.
          * RenderingHint is used by CRCoreUtil::Renderer to determine which
          * draw bin to drop associated CRCore::Drawables in. For opaque
          * objects OPAQUE_BIN would typical used, which TRANSPARENT_BIN
          * should be used for objects which need to be depth sorted.*/
        void setRenderingHint(RenderingHint hint);

        /** get the RenderingHint of the crStateSet.*/
        inline RenderingHint getRenderingHint() const { return m_renderingHint; }

        enum RenderBinMode
        {
            INHERIT_RENDERBIN_DETAILS,
            USE_RENDERBIN_DETAILS,
            OVERRIDE_RENDERBIN_DETAILS,
            ENCLOSE_RENDERBIN_DETAILS
        };

        /** set the render bin details.*/
        void setRenderBinDetails(int binNum,const std::string& binName,RenderBinMode mode=USE_RENDERBIN_DETAILS);
                
        /** set the render bin details to inherit.*/
        void setRenderBinToInherit();
        
        /** get the render bin mode.*/
        inline RenderBinMode getRenderBinMode() const { return m_binMode; }

        /** get whether the render bin details are set and should be used.*/
        inline bool useRenderBinDetails() const { return m_binMode!=INHERIT_RENDERBIN_DETAILS; }
		inline bool useRenderBinDetails() { return m_binMode!=INHERIT_RENDERBIN_DETAILS; }

		/** Set the render bin number.*/
		inline void setBinNumber(int num) { m_binNum=num; }
		/** get the render bin number.*/
        inline int getBinNumber() const { return m_binNum; }
		inline int getBinNumber() { return m_binNum; }		

		inline void setBinName(const std::string& name) { m_binName=name; }
        /** get the render bin name.*/
        inline const std::string& getBinName() const { return m_binName; }
		inline const std::string& getBinName() { return m_binName; }

		//struct CompileCallback : public virtual crBase
		//{
		//	CompileCallback() {}

		//	CompileCallback(const CompileCallback&,const crCopyOp&) {}
		//	META_Base(CRCore,CompileCallback);
		//	/** do customized cull code.*/
		//	virtual void compile(crStateSet *stateset, crState &state) const {}
		//};

		//void setComileCallback(CompileCallback *callback){ m_compileCallback = callback; }
		//const CompileCallback *getCompileCallback()const { return m_compileCallback.get(); }
		
		//void setLight( int lightid );  
		//int getLightID()const;
		//
		//void setMaterial( int matid );
		//int getMaterialID()const;

		//void setTexture( unsigned int unit, int texid );
		//int getTextureID( unsigned int unit );

		void setNeedComplie( bool bln){ m_needCompile = bln; }
		bool getNeedComplie()const{ return m_needCompile; }
		bool getNeedComplie(){ return m_needCompile; }

        /** call compile on all crStateAttributes contained within this crStateSet.*/
        void compile(crState& state) const;

        virtual void releaseObjects(crState* state=0);

		struct Callback : public virtual CRCore::crBase
		{
			Callback() {}

			Callback(const Callback&,const crCopyOp&) {}

			META_Base(CRCore,Callback);

			/** do customized callback code.*/
			virtual void operator() (crStateSet*, crNodeVisitor*) {}
		};

		/** Set the Update Callback which allows users to attach customize the updating of an object during the update traversal.*/
		void setUpdateCallback(Callback* ac);

		/** Get the non const Update Callback.*/
		Callback* getUpdateCallback() { return m_updateCallback.get(); }

		/** Get the const Update Callback.*/
		const Callback* getUpdateCallback() const { return m_updateCallback.get(); }

		/** Return whether this StateSet has update callbacks associated with it, and therefore must be traversed.*/
		bool requiresUpdateTraversal() const { return m_updateCallback.valid() || getNumChildrenRequiringUpdateTraversal()!=0; }

		/** Get the number of Objects of this StateSet which require Update traversal,
		* since they have an Update Callback attached to them or their children.*/
		inline unsigned int getNumChildrenRequiringUpdateTraversal() const { return m_numChildrenRequiringUpdateTraversal; }

		/** Run the update callbacks attached directly to this StateSet or to its children.*/
		void runUpdateCallbacks(CRCore::crNodeVisitor* nv);

		///** Set the Event Callback which allows users to attach customize the updating of an object during the event traversal.*/
		//void setEventCallback(Callback* ac);

		///** Get the non const Event Callback.*/
		//Callback* getEventCallback() { return m_eventCallback.get(); }

		///** Get the const Event Callback.*/
		//const Callback* getEventCallback() const { return m_eventCallback.get(); }

		///** Return whether this StateSet has event callbacks associated with it, and therefore must be traversed.*/
		//bool requiresEventTraversal() const { return m_eventCallback.valid() || getNumChildrenRequiringEventTraversal()!=0; }

		///** Get the number of Objects of this StateSet which require Event traversal,
		//* since they have an Eevnt Callback attached to them or their children.*/
		//inline unsigned int getNumChildrenRequiringEventTraversal() const { return m_numChildrenRequiringEventTraversal; }

		///** Run the event callbacks attached directly to this StateSet or to its children.*/
		//void runEventCallbacks(CRCore::crNodeVisitor* nv);
        
		/** Check the modes associated with this StateSet are supported by current OpenGL drivers,
		* and if not set the associated mode in osg::State to be black listed/invalid.
		* Return true if all associated modes are valid.*/
		bool checkValidityOfAssociatedModes(crState& state) const;

		inline void swapBuffers(int frameNumber)
		{
			if(m_swapFrameNumber != frameNumber)
			{
				for( UniformList::iterator itr = m_uniformList.begin();
					itr != m_uniformList.end();
					++itr )
				{
					//if(itr->second.first->getDataVariance()==crBase::DYNAMIC)
					itr->second.first->swapBuffers(frameNumber);
				}
				m_modeList_buf = m_modeList;
				m_attributeList_buf = m_attributeList;
				m_textureModeList_buf = m_textureModeList;
				m_textureAttributeList_buf = m_textureAttributeList;
				m_uniformList_buf = m_uniformList;
				m_swapFrameNumber = frameNumber;
			}
		}
		inline AttributeList& getAttributeListBuf() { return m_swapFrameNumber>0?m_attributeList_buf:m_attributeList; }
		inline const AttributeList& getAttributeListBuf()const { return m_swapFrameNumber>0?m_attributeList_buf:m_attributeList; }
		inline TextureAttributeList& getTextureAttributeListBuf() { return m_swapFrameNumber>0?m_textureAttributeList_buf:m_textureAttributeList; }
		inline const TextureAttributeList& getTextureAttributeListBuf() const { return m_swapFrameNumber>0?m_textureAttributeList_buf:m_textureAttributeList; }
		inline UniformList& getUniformListBuf() { return m_swapFrameNumber>0?m_uniformList_buf:m_uniformList; }
		inline const UniformList& getUniformListBuf()const { return m_swapFrameNumber>0?m_uniformList_buf:m_uniformList; }
		inline ModeList& getModeListBuf() { return m_swapFrameNumber>0?m_modeList_buf:m_modeList; }
		inline const ModeList& getModeListBuf() const { return m_swapFrameNumber>0?m_modeList_buf:m_modeList; }
		inline TextureModeList& getTextureModeListBuf() { return m_swapFrameNumber>0?m_textureModeList_buf:m_textureModeList; }
		inline const TextureModeList& getTextureModeListBuf() const { return m_swapFrameNumber>0?m_textureModeList_buf:m_textureModeList; }

		void dirtyShaderStr();
		const std::string &getRtglmShaderStr(const crStateSet *obj_ss = NULL) const;
		//const std::string &getRtplmShaderStr(const crStateSet *drawable_ss = NULL) const;

		void setTextureOperate(int i, int operate) { if(i>=0 && i<16) m_textureOperate[i] = operate; }
		inline int getTextureOperate(int i) const { return i>=0 && i<16 ? m_textureOperate[i] : 0; }

		void setAlphaOperate(int i, int operate) { if(i>=0 && i<16) m_alphaOperate[i] = operate; }
		inline int getAlphaOperate(int i) const { return i>=0 && i<16 ? m_alphaOperate[i] : 0; }
		void setTextureUV(int i,unsigned char uv){ if(i>=0 && i<16) m_textureUV[i] = uv; }
		inline unsigned char getTextureUV(int i) const { return i>=0 && i<16 ? m_textureUV[i] : 0; }
		//void setRenderInited(bool bln);
		//bool getRenderInited();
		enum StateSetMode
		{
			SS_Normal,
			SS_ShadowMapRender,
			SS_CollideMapRender,
			SS_NRPRender,
		};
		inline void setStateSetMode(unsigned char mode) { m_statesetMode = mode; }
		inline unsigned char getStateSetMode()const { return m_statesetMode; }
    protected :

        virtual ~crStateSet();

        crStateSet& operator = (const crStateSet&) { return *this; }

		void addParent(crBase* object);
		void removeParent(crBase* object);

		ParentList m_parents;
		friend class CRCore::crNode;
		friend class CRCore::crDrawable;
		friend class CRCore::crUniform;
		friend class CRCore::crStateAttribute;

        mutable ModeList                            m_modeList;
		mutable ModeList                            m_modeList_buf;
        mutable AttributeList                       m_attributeList;
		mutable AttributeList                       m_attributeList_buf;

        mutable TextureModeList                     m_textureModeList;
		mutable TextureModeList                     m_textureModeList_buf;
        mutable TextureAttributeList                m_textureAttributeList;
        mutable TextureAttributeList                m_textureAttributeList_buf;

		UniformList                                 m_uniformList;
		UniformList                                 m_uniformList_buf;

        inline ModeList& getOrCreateTextureModeList(unsigned int unit)
        {
            if (unit>=m_textureModeList.size()) m_textureModeList.resize(unit+1);
            return m_textureModeList[unit];
        }

		inline ModeList& getOrCreateTextureModeList(unsigned int unit) const
		{
			if (unit>=m_textureModeList.size()) m_textureModeList.resize(unit+1);
			return m_textureModeList[unit];
		}

        inline AttributeList& getOrCreateTextureAttributeList(unsigned int unit)
        {
            if (unit>=m_textureAttributeList.size()) m_textureAttributeList.resize(unit+1);
            return m_textureAttributeList[unit];
        }

		inline AttributeList& getOrCreateTextureAttributeList(unsigned int unit) const
		{
			if (unit>=m_textureAttributeList.size()) m_textureAttributeList.resize(unit+1);
			return m_textureAttributeList[unit];
		}

        int compareModes(const ModeList& lhs,const ModeList& rhs);
        int compareAttributePtrs(const AttributeList& lhs,const AttributeList& rhs);
        int compareAttributeContents(const AttributeList& lhs,const AttributeList& rhs);

        void setMode(ModeList& modeList,crStateAttribute::VRMode mode, crStateAttribute::VRModeValue value);
		void setMode(ModeList& modeList,crStateAttribute::VRMode mode, crStateAttribute::VRModeValue value)const;

        void setModeToInherit(ModeList& modeList,crStateAttribute::VRMode mode);
		void setModeToInherit(ModeList& modeList,crStateAttribute::VRMode mode)const;

        crStateAttribute::VRModeValue getMode(const ModeList& modeList,crStateAttribute::VRMode mode) const;
        void setAssociatedModes(ModeList& modeList,const crStateAttribute* attribute, crStateAttribute::VRModeValue value);

        void setAttribute(AttributeList& attributeList,crStateAttribute *attribute, const crStateAttribute::OverrideValue value=crStateAttribute::OFF);

        crStateAttribute* getAttribute(AttributeList& attributeList,const crStateAttribute::Type type);
        const crStateAttribute* getAttribute(const AttributeList& attributeList,const crStateAttribute::Type type) const;
        const RefAttributePair* getAttributePair(const AttributeList& attributeList,const crStateAttribute::Type type) const;

        RenderingHint                       m_renderingHint;

        RenderBinMode                       m_binMode;
        int                                 m_binNum;
        std::string                         m_binName;

		//ref_ptr<CompileCallback>            m_compileCallback;

		mutable bool                        m_needCompile;

		ref_ptr<Callback> m_updateCallback;
		unsigned int m_numChildrenRequiringUpdateTraversal;
		void setNumChildrenRequiringUpdateTraversal(unsigned int num);

		//ref_ptr<Callback> m_eventCallback;
		//unsigned int m_numChildrenRequiringEventTraversal;
		//void setNumChildrenRequiringEventTraversal(unsigned int num);

		//bool                               m_isShadowStateSet;
		unsigned char                        m_statesetMode;

		//mutable std::string                        m_getRtglmShaderStr;
		typedef std::map<const crStateSet*,std::string> ShaderStrMap;
		mutable ShaderStrMap                      m_shaderStrMap;
		//mutable std::string                        m_getRtplmShaderStr;
		unsigned char                      m_textureOperate[16];//0:add, 1:mult, 2:Replace, 3:Ignore, 4:AddETTo, 5:MultETTo, 6:ET3ToLM
		unsigned char                      m_alphaOperate[16];//0:Ignore,1:add, 2:mult, 3:Replace, 
		unsigned char                      m_textureUV[16];//0Ä¬ÈÏ£¬1£ºUV1
		//bool m_renderInited;
};

}

#endif
