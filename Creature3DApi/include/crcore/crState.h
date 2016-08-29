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
#ifndef CRCORE_CRSTATE_H
#define CRCORE_CRSTATE_H 1

#include <CRCore/crExport.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crMatrix.h>
#include <CRCore/crUniform.h>
#include <CRCore/crBufferObject.h>

#include <CRCore/crFrameStamp.h>
#include <CRCore/crDisplaySettings.h>
#include <CRCore/crPolytope.h>
#include <CRCore/crViewPort.h>
#include <CRCore/crNotify.h>
#include <vector>
#include <map>

namespace CRCore {

class crGraphicsContext;

/** crState class for managing a state stack.
  * Lazy state updating is used to minimize state changes.
  */
class CR_EXPORT crState : public Referenced
{
    public :
    
        crState();

		/** Set the graphics context associated with that owns this State object.*/        
		void setGraphicsContext(crGraphicsContext* context) { m_graphicsContext = context; }

		/** Get the graphics context associated with that owns this State object.*/        
		crGraphicsContext* getGraphicsContext() { return m_graphicsContext; }

		/** Get the const graphics context associated with that owns this State object.*/        
		const crGraphicsContext* getGraphicsContext() const { return m_graphicsContext; }

        ///** push stateset onto state stack.*/
        void pushStateSet(const crStateSet* dstate);

        ///** pop stateset off state stack.*/
        void popStateSet();
       
        ///** pop all stateset's off state stack, ensuring its empty ready for the next frame.
        //  * note, to return OpenGL to default state, one should do any state.popAllStatSets(); state.apply().*/
        void popAllStateSets();

        ///** copy the modes and attributes which captures the current state.*/
        void captureCurrentState(crStateSet& stateset) const;

        ///** reset the state object to an empty stack.*/
        virtual void reset();
        //
        inline const crViewPort* getCurrentViewport() const
        {
            return static_cast<const crViewPort*>(getLastAppliedAttribute(CRCore::crStateAttribute::VIEWPORT));
        }
      
        virtual void setInitialViewMatrix(const CRCore::RefMatrix* matrix);

        inline const CRCore::crMatrix& getInitialViewMatrix() const { return *m_initialViewMatrix; }
        inline const CRCore::crMatrix& getInitialInverseViewMatrix() const { return m_initialInverseViewMatrix; }

		void applyProjectionMatrix(const CRCore::RefMatrix* matrix);
        
        inline const CRCore::crMatrix& getProjectionMatrix() const
        {
            return *m_projection;
        }

		void applyModelViewMatrix(const CRCore::RefMatrix* matrix);

        const CRCore::crMatrix& getModelViewMatrix() const
        {
            return *m_modelView;
        }

        crPolytope getViewFrustum() const;

        /** Apply stateset.*/
        void apply(const crStateSet* dstate);

        /** Apply the state.*/
        void apply();


        /** Apply an OpenGL mode if required. */
        inline bool applyMode(crStateAttribute::VRMode mode,bool enabled)
        {
            ModeStack& ms = m_modeMap[mode];
            ms.changed = true;
			//in
            return applyMode(mode,enabled,ms);
        }

        inline bool applyTextureMode(unsigned int unit, crStateAttribute::VRMode mode,bool enabled)
        {
            if (setActiveTextureUnit(unit))
            {
                ModeMap& modeMap = getOrCreateTextureModeMap(unit);
                ModeStack& ms = modeMap[mode];
                ms.changed = true;
                return applyMode(mode,enabled,ms);
            }
            else
                return false;
        }

		/** Set whether a particular OpenGL mode is valid in the current graphics context.
		* Use to disable OpenGL modes that are not supported by current graphics drivers/context.*/
		inline void setModeValidity(crStateAttribute::VRMode mode,bool valid)
		{
			ModeStack& ms = m_modeMap[mode];
			ms.valid = valid;
		}

		/** Get whether a particular OpenGL mode is valid in the current graphics context.
		* Use to disable OpenGL modes that are not supported by current graphics drivers/context.*/
		inline bool getModeValidity(crStateAttribute::VRMode mode)
		{
			ModeStack& ms = m_modeMap[mode];
			return ms.valid;
		}

		inline void setGlobalDefaultAttribute( const crStateAttribute* attribute)
		{
			AttributeStack& as = m_attributeMap[attribute->getType()];
			as.global_default_attribute = attribute;
		}

		inline const crStateAttribute* getGlobalDefaultAttribute(crStateAttribute::Type type)
		{
			AttributeStack& as = m_attributeMap[type];
			return as.global_default_attribute.get();
		}

		inline void setGlobalDefaultTextureAttribute(unsigned int unit, const crStateAttribute* attribute)
		{
			AttributeMap& attributeMap = getOrCreateTextureAttributeMap(unit);
			AttributeStack& as = attributeMap[attribute->getType()];
			as.global_default_attribute = attribute;
		}

		inline const crStateAttribute* getGlobalDefaultTextureAttribute(unsigned int unit, crStateAttribute::Type type)
		{
			AttributeMap& attributeMap = getOrCreateTextureAttributeMap(unit);
			AttributeStack& as = attributeMap[type];
			return as.global_default_attribute.get();
		}


        /** Apply an attribute if required. */
        inline bool applyAttribute(const crStateAttribute* attribute)
        {
            AttributeStack& as = m_attributeMap[attribute->getType()];
            as.changed = true;
            return applyAttribute(attribute,as);
        }

        inline bool applyTextureAttribute(unsigned int unit, const crStateAttribute* attribute)
        {
            if (setActiveTextureUnit(unit))
            {
                AttributeMap& attributeMap = getOrCreateTextureAttributeMap(unit);
                AttributeStack& as = attributeMap[attribute->getType()];
                as.changed = true;
                return applyAttribute(attribute,as);
            }
            else
                return false;
        }
       
        /** Mode has been set externally, update state to reflect this setting.*/
        void haveAppliedMode(crStateAttribute::VRMode mode,crStateAttribute::VRModeValue value);
        
        /** Mode has been set externally, therefore dirty the associated mode in CRCore::crState
          * so it is applied on next call to CRCore::crState::apply(..)*/
        void haveAppliedMode(crStateAttribute::VRMode mode);

        /** Attribute has been applied externally, update state to reflect this setting.*/
        void haveAppliedAttribute(const crStateAttribute* attribute);

        /** Attribute has been applied externally, 
          * and therefore this attribute type has been dirtied 
          * and will need to be re-appplied on next CRCore::crState.apply(..).
          * note, if you have an CRCore::crStateAttribute which you have applied externally
          * then use the have_applied(attribute) method as this will the CRCore::crState to
          * track the current state more accuratly and enable lazy state updating such
          * that only changed state will be applied.*/
        void haveAppliedAttribute(crStateAttribute::Type type);

        /** Get whether the current specified mode is enabled (true) or disabled (false).*/ 
        bool getLastAppliedMode(crStateAttribute::VRMode mode) const;
        
        /** Get the current specified attribute, return NULL is one has not yet been applied.*/ 
        const crStateAttribute* getLastAppliedAttribute(crStateAttribute::Type type) const;
        


        /** texture Mode has been set externally, update state to reflect this setting.*/
        void haveAppliedTextureMode(unsigned int unit, crStateAttribute::VRMode mode,crStateAttribute::VRModeValue value);
        
        /** texture Mode has been set externally, therefore dirty the associated mode in CRCore::crState
          * so it is applied on next call to CRCore::crState::apply(..)*/
        void haveAppliedTextureMode(unsigned int unit, crStateAttribute::VRMode mode);

        /** texture Attribute has been applied externally, update state to reflect this setting.*/
        void haveAppliedTextureAttribute(unsigned int unit, const crStateAttribute* attribute);

        /** texture Attribute has been applied externally, 
          * and therefore this attribute type has been dirtied 
          * and will need to be re-appplied on next CRCore::crState.apply(..).
          * note, if you have an CRCore::crStateAttribute which you have applied externally
          * then use the have_applied(attribute) method as this will the CRCore::crState to
          * track the current state more accuratly and enable lazy state updating such
          * that only changed state will be applied.*/
        void haveAppliedTextureAttribute(unsigned int unit, crStateAttribute::Type type);

        /** Get whether the current specified texture mode is enabled (true) or disabled (false).*/ 
        bool getLastAppliedTextureMode(unsigned int unit, crStateAttribute::VRMode mode) const;
        
        /** Get the current specified texture attribute, return NULL is one has not yet been applied.*/ 
        const crStateAttribute* getLastAppliedTextureAttribute(unsigned int unit, crStateAttribute::Type type) const;


        /** Dirty the modes previously applied in CRCore::crState.*/
        void dirtyAllModes(); 

        /** Dirty the modes attributes previously applied in CRCore::crState.*/
        void dirtyAllAttributes();

        /** disable the vertex, normal, color, tex coords, secenday color, fog coord and index arrays.*/
        void disableAllVertexArrays();

        /** dirty the vertex, normal, color, tex coords, secenday color, fog coord and index arrays.*/
        void dirtyAllVertexArrays();

		void setCurrentPixelBufferObject(crPixelBufferObject* pbo) { m_currentPBO = pbo; }
		const crPixelBufferObject* getCurrentPixelBufferObject() { return m_currentPBO; }

		inline void bindPixelBufferObject(const crPixelBufferObject* pbo)
		{
			if (pbo == m_currentPBO) return;

			if (pbo->needsCompile(m_contextID)) pbo->compileBuffer(*this);
			else pbo->bindBuffer(m_contextID);

			m_currentPBO = pbo;
		}

		inline void unbindPixelBufferObject()
		{
			if (!m_currentPBO) return;

			m_currentPBO->unbindBuffer(m_contextID);
			m_currentPBO = 0;
		}

        /** Wrapper around glInterleavedArrays(..).
          * also resets the internal array points and modes within CRCore::crState to keep the other
          * vertex array operations consistent. */
		void setInterleavedArrays( unsigned int format, int stride, const void* pointer);


        /** wrapper around glEnableClientState(GL_VERTEX_ARRAY);glVertexPointer(..);
          * note, only updates values that change.*/
		void setVertexPointer( int size, unsigned int type, int stride, const void *ptr );
        
        /** wrapper glDisableClientState(GL_VERTEX_ARRAY).
          * note, only updates values that change.*/
		void disableVertexPointer();

        inline void dirtyVertexPointer()
        {
            m_vertexArray.m_pointer = 0; 
            m_vertexArray.m_dirty = true; 
        }

        /** wrapper around glEnableClientState(GL_NORMAL_ARRAY);glNormalPointer(..);
          * note, only updates values that change.*/
		void setNormalPointer( unsigned int type, int stride, const void *ptr );

        /** wrapper around glDisableClientState(GL_NORMAL_ARRAY);
          * note, only updates values that change.*/
		void disableNormalPointer();
		
        inline void dirtyNormalPointer()
        {
			m_normalArray.m_pointer = 0;
            m_normalArray.m_dirty = true;
        }

        /** wrapper around glEnableClientState(GL_COLOR_ARRAY);glColorPointer(..);
          * note, only updates values that change.*/
		void setColorPointer( int size, unsigned int type, int stride, const void *ptr );

        /** wrapper around glDisableClientState(GL_COLOR_ARRAY);
          * note, only updates values that change.*/
		void disableColorPointer();

        inline void dirtyColorPointer()
        { 
            m_colorArray.m_pointer = 0;
            m_colorArray.m_dirty = true;
        }

        inline bool isSecondaryColorSupported() const { return m_isSecondaryColorSupportResolved?m_isSecondaryColorSupported:computeSecondaryColorSupported(); }

        /** wrapper around glEnableClientState(GL_SECONDARY_COLOR_ARRAY);glSecondayColorPointer(..);
          * note, only updates values that change.*/
		void setSecondaryColorPointer( int size, unsigned int type, int stride, const void *ptr );

        /** wrapper around glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
          * note, only updates values that change.*/
		void disableSecondaryColorPointer();
     
        inline void dirtySecondaryColorPointer()
        {
            m_secondaryColorArray.m_pointer = 0;
            m_secondaryColorArray.m_dirty = true;
        }

        /** wrapper around glEnableClientState(GL_INDEX_ARRAY);glIndexPointer(..);
          * note, only updates values that change.*/
		void setIndexPointer( unsigned int type, int stride, const void *ptr );

        /** wrapper around glDisableClientState(GL_INDEX_ARRAY);
          * note, only updates values that change.*/
		void disableIndexPointer();

        inline void dirtyIndexPointer()
        {
            m_indexArray.m_pointer = 0;
            m_indexArray.m_dirty = true;
        }

        inline bool isFogCoordSupported() const { return m_isFogCoordSupportResolved?m_isFogCoordSupported:computeFogCoordSupported(); }

        /** wrapper around glEnableClientState(GL_FOG_COORDINATE_ARRAY);glFogCoordPointer(..);
          * note, only updates values that change.*/
		void setFogCoordPointer( unsigned int type, int stride, const void *ptr );

        /** wrapper around glDisableClientState(GL_FOG_COORDINATE_ARRAY);
          * note, only updates values that change.*/
		void disableFogCoordPointer();

        inline void dirtyFogCoordPointer()
        {
            m_fogArray.m_pointer = 0;
            m_fogArray.m_dirty = true;
        }


        /** wrapper around glEnableClientState(GL_TEXTURE_COORD_ARRAY);glTexCoordPointer(..);
          * note, only updates values that change.*/
        void setTexCoordPointer( unsigned int unit,
                                        int size, unsigned int type,
										int stride, const void *ptr );
     
        /** wrapper around glDisableClientState(GL_TEXTURE_COORD_ARRAY);
          * note, only updates values that change.*/
		void disableTexCoordPointer( unsigned int unit );
 
        inline void dirtyTexCoordPointer( unsigned int unit )
        {
            if ( unit >= m_texCoordArrayList.size()) return; // m_texCoordArrayList.resize(unit+1);
            EnabledArrayPair& eap = m_texCoordArrayList[unit];
            eap.m_pointer = 0;
            eap.m_dirty = true;
        }

		void disableTexCoordPointersAboveAndIncluding( unsigned int unit );

        inline void dirtyTexCoordPointersAboveAndIncluding( unsigned int unit )
        {
            while (unit<m_texCoordArrayList.size())
            {
                EnabledArrayPair& eap = m_texCoordArrayList[unit];
                eap.m_pointer = 0;
                eap.m_dirty = true;
                ++unit;
            }
        }

        /** set the current tex coord array texture unit, return true if selected, false if selection failed such as when multitexturing is not supported.
          * note, only updates values that change.*/
	    bool setClientActiveTextureUnit( unsigned int unit );

		/** Get the current tex coord array texture unit.*/
		unsigned int getClientActiveTextureUnit() const { return m_currentClientActiveTextureUnit; }

	   /** set the current texture unit, return true if selected, false if selection failed such as when multitexturing is not supported.
	   * note, only updates values that change.*/
	    bool setActiveTextureUnit( unsigned int unit );

		/** Get the current texture unit.*/
		unsigned int getActiveTextureUnit() const { return m_currentActiveTextureUnit; }

        /** wrapper around glEnableVertexAttribArrayARB(index);glVertexAttribPointerARB(..);
          * note, only updates values that change.*/
        void setVertexAttribPointer( unsigned int index,
                                     int size, unsigned int type, unsigned char normalized, 
                                     int stride, const void *ptr );
        
        /** wrapper around DisableVertexAttribArrayARB(index);
          * note, only updates values that change.*/
        void disableVertexAttribPointer( unsigned int index );
      
        void disableVertexAttribPointersAboveAndIncluding( unsigned int index );
        
        inline void dirtyVertexAttribPointersAboveAndIncluding( unsigned int index )
        {
            while (index<m_vertexAttribArrayList.size())
            {
                EnabledArrayPair& eap = m_vertexAttribArrayList[index];
                eap.m_pointer = 0;
                eap.m_dirty = true;
                ++index;
            }
        }

        bool isVertexBufferObjectSupported() const { return m_isVertexBufferObjectSupportResolved?m_isVertexBufferObjectSupported:computeVertexBufferObjectSupported(); }

		void setLastAppliedProgramObject(const crProgram::PerContextProgram* program) { if (m_lastAppliedProgramObject!=program) { m_lastAppliedProgramObject = program; if (program) m_appliedProgramObjectSet.insert(program); } }
		const crProgram::PerContextProgram* getLastAppliedProgramObject() const { return m_lastAppliedProgramObject; }

		inline int getUniformLocation( const std::string& name ) const { return m_lastAppliedProgramObject ? m_lastAppliedProgramObject->getUniformLocation(name) : -1; }
		inline int getAttribLocation( const std::string& name ) const { return m_lastAppliedProgramObject ? m_lastAppliedProgramObject->getAttribLocation(name) : -1; }

        /** Set the current OpenGL context uniqueID.
            Note, it is the application developers responsibility to
            set up unique ID for each OpenGL context.  This value is
            then used by CRCore::StateAttribure's and CRCore::Drawable's to
            help manage OpenGL display list and texture binds appropriate
            for each context, the contextID simply acts as an index local
            arrays that they maintain for the purpose.
            Typical settings for contextID are 0,1,2,3... up to the maximum
            number of graphics contexts you have setup.
            By default contextID is 0.*/ 
        inline void setContextID(unsigned int contextID) { m_contextID=contextID; }

        /** Get the current OpenGL context unique ID.*/ 
        inline unsigned int getContextID() const { return m_contextID; }
        
        ///** Set the frame stamp for the current frame.*/
        //inline void setFrameStamp(crFrameStamp* fs) { m_frameStamp = fs; }

        ///** Set the frame stamp for the current frame.*/
        //inline const crFrameStamp* getFrameStamp() const { return m_frameStamp.get(); }
                
        /** Set the crDisplaySettings. Note, nothing is applied, the visual settings are just used
          * used in the crState object to pass the current visual settings to Drawables
          * during rendering. */
        inline void setDisplaySettings(crDisplaySettings* vs) { m_displaySettings = vs; }
        
        /** Get the crDisplaySettings */
       inline const crDisplaySettings* getDisplaySettings() const { return m_displaySettings.get(); }

        typedef std::pair<const crStateAttribute*,crStateAttribute::OverrideValue>  AttributePair;
        typedef std::vector<AttributePair>                                      AttributeVec;
        typedef std::vector<crStateAttribute::VRModeValue>                        ValueVec;
        
        //
        ///** Set flag for early termination of the draw traversal.*/
        void setAbortRenderingPtr(bool* abortPtr) { m_abortRenderingPtr = abortPtr; }

        /** Get flag for early termination of the draw traversal, 
          * if true steps should be taken to complete rendering early.*/
        bool getAbortRendering() const { return m_abortRenderingPtr!=0?(*m_abortRenderingPtr):false; }

        //void setReportErrors(bool flag) { m_reportErrors = flag; }
        //bool getReportErrors() const { return m_reportErrors; }

        //virtual bool checkErrors(const char* str) const { return false; };
        //virtual bool checkErrors(crStateAttribute::VRMode mode) const { return false; };
        //virtual bool checkErrors(const crStateAttribute* attribute) const { return false; };
		

		//for render dataList
		typedef std::vector< ref_ptr<const crStateAttribute> > StateAttributeList;
		typedef std::multimap< unsigned int, ref_ptr<const crStateAttribute> > TextureList;
		typedef std::pair< crStateAttribute::VRMode, bool > StateModePair;
		typedef std::vector< StateModePair > StateModeList;

		void getAttributeAndModeList( const crStateSet *dstate, StateAttributeList **, StateModeList **, TextureList **);
		//////
        /////////////////////////////////////////////////////////
		/** Mark all the vertex attributes as being disabled but leave the disabling till a later call to applyDisablingOfVertexAttributes.*/
		void lazyDisablingOfVertexAttributes();
		/** Disable all the vertex attributes that have been marked as to be disabled.*/
		void applyDisablingOfVertexAttributes();

		inline void setIgnoreColor(bool ignore){m_ignorecolor = ignore;}
		inline void colorDirty(){ m_colordirty = true; }
		inline void applyColor(const crVector4 &v)
		{
			if(!m_ignorecolor && (m_colordirty || v != m_lastAppliedColor))
			{
				m_lastAppliedColor = v;
				glColor4fv(v.ptr());
				m_colordirty = false;
			}
		}
		inline void applyCullFace(unsigned int mode)
		{
			if(m_lastmode != mode)
			{
				m_lastmode = mode;
				glCullFace(mode);
			}
		}
		//inline void applyColorMask(const crVector4ub &mask)
		//{
		//	//if(m_lastmask != mask)
		//	//{
		//	//	m_lastmask = mask;
		//	glColorMask(mask.r(),mask.g(),mask.b(),mask.a());
		//	//}
		//}
		void initializeExtensionProcs();
    protected:

		virtual ~crState();

		crGraphicsContext*          m_graphicsContext;
        unsigned int                m_contextID;
        //ref_ptr<crFrameStamp>       m_frameStamp;
        
        ref_ptr<const RefMatrix>    m_identity;
        ref_ptr<const RefMatrix>    m_initialViewMatrix;
        ref_ptr<const RefMatrix>    m_projection;
        ref_ptr<const RefMatrix>    m_modelView;
        
        crMatrix                      m_initialInverseViewMatrix;

        ref_ptr<crDisplaySettings>    m_displaySettings;
        
        bool*                       m_abortRenderingPtr;
        //bool                        m_reportErrors;
       
		
		//for render dataList
		StateAttributeList          m_attributeList;
		StateModeList               m_modeList;
		TextureList                 m_textureList;
		bool                        m_execute;
		unsigned int                m_currentunit;
		///////


        struct ModeStack
        {
            ModeStack()
            {
				valid = true;
                changed = false;
                last_applied_value = false;
                global_default_value = false;
            }

			bool        valid;
            bool        changed;
            bool        last_applied_value;
            bool        global_default_value;
            ValueVec    valueVec;
        };

        struct AttributeStack
        {
            AttributeStack()
            {
                changed = false;
                last_applied_attribute = 0L;
                global_default_attribute = 0L;
            }
			~AttributeStack()
			{
                last_applied_attribute = NULL;
			}

            /** apply an attribute if required, passing in attribute and appropriate attribute stack */
            bool                    changed;
            ref_ptr<const crStateAttribute>   last_applied_attribute;
            ref_ptr<const crStateAttribute> global_default_attribute;
            AttributeVec            attributeVec;
        };
        
		struct UniformStack
		{
			typedef std::pair<const crUniform*,crStateAttribute::OverrideValue>     UniformPair;
			typedef std::vector<UniformPair>                                        UniformVec;

			UniformStack() {}
			~UniformStack() { uniformVec.clear(); }

			UniformVec              uniformVec;
		};

        /** apply an OpenGL mode if required, passing in mode, enable flag and appropriate mode stack */
        bool applyMode(crStateAttribute::VRMode mode,bool enabled,ModeStack& ms );

        /** apply an attribute if required, passing in attribute and appropriate attribute stack */
        inline bool applyAttribute(const crStateAttribute* attribute,AttributeStack& as)
        {
            if (as.last_applied_attribute != attribute)
            {
                if (!as.global_default_attribute.valid()) as.global_default_attribute = dynamic_cast<crStateAttribute*>(attribute->cloneType());

                as.last_applied_attribute = attribute;

				if(m_execute)
                    attribute->apply(*this);
				else
				{
					if(attribute->isTextureAttribute())
					{
						typedef std::pair<unsigned int, ref_ptr<const crStateAttribute> > Temp_TexturePair;
                        m_textureList.insert(Temp_TexturePair( m_currentunit, attribute));  
					}
					else
					    m_attributeList.push_back( attribute );
				}
                
                //if (m_reportErrors) checkErrors(attribute);
                
                return true;
            }
            else
                return false;
        }

        inline bool applyGlobalDefaultAttribute(AttributeStack& as)
        {
            if (as.last_applied_attribute != as.global_default_attribute.get())
            {
                as.last_applied_attribute = as.global_default_attribute.get();
                if (as.global_default_attribute.valid())
                {
                    if(m_execute)
					    as.global_default_attribute->apply(*this);
					else
					{
						if(as.global_default_attribute->isTextureAttribute())
						{
							typedef std::pair<unsigned int, ref_ptr<const crStateAttribute> > Temp_TexturePair;
							m_textureList.insert(Temp_TexturePair( m_currentunit, as.global_default_attribute ));  
						}
						else
							m_attributeList.push_back( as.global_default_attribute );

					}               //     if (m_reportErrors) checkErrors(as.global_default_attribute.get());
                }
                return true;
            }
            else
                return false;
        }
        

        typedef std::map<crStateAttribute::VRMode,ModeStack>      ModeMap;
        typedef std::vector<ModeMap>                              TextureModeMapList;

        typedef std::map<crStateAttribute::Type,AttributeStack>   AttributeMap;
        typedef std::vector<AttributeMap>                         TextureAttributeMapList;

		typedef std::map<std::string,UniformStack>                      UniformMap;

        typedef std::vector<const crStateSet*>                    StateSetStack;
        typedef std::vector<ref_ptr<const crMatrix> >             MatrixStack;

        typedef std::set<CRCore::ref_ptr<const crProgram::PerContextProgram> > AppliedProgramObjectSet;

        ModeMap                                                 m_modeMap;
        AttributeMap                                            m_attributeMap;
        UniformMap                                              m_uniformMap;

        TextureModeMapList                                      m_textureModeMapList;
        TextureAttributeMapList                                 m_textureAttributeMapList;
       
		AppliedProgramObjectSet                                 m_appliedProgramObjectSet;
		const crProgram::PerContextProgram*                     m_lastAppliedProgramObject;

        StateSetStack                                           m_drawStateStack;
        
        struct EnabledArrayPair
        {
            EnabledArrayPair():m_lazy_disable(false),m_dirty(true),m_enabled(false),m_normalized(0),m_pointer(0) {}
            EnabledArrayPair(const EnabledArrayPair& eap):m_lazy_disable(eap.m_lazy_disable),m_dirty(eap.m_dirty),m_enabled(eap.m_enabled),m_normalized(eap.m_normalized),m_pointer(eap.m_pointer) {}
            EnabledArrayPair& operator = (const EnabledArrayPair& eap) { m_lazy_disable = eap.m_lazy_disable; m_dirty=eap.m_dirty; m_enabled=eap.m_enabled; m_normalized=eap.m_normalized;m_pointer=eap.m_pointer; return *this; }
            
			bool            m_lazy_disable;
			bool            m_dirty;
            bool            m_enabled;
            unsigned char   m_normalized;
            const void*     m_pointer;
        };
        
        typedef std::vector<EnabledArrayPair>                   EnabledTexCoordArrayList;
        typedef std::vector<EnabledArrayPair>                   EnabledVertexAttribArrayList;

        EnabledArrayPair                m_vertexArray;
        EnabledArrayPair                m_normalArray;
        EnabledArrayPair                m_colorArray;
        EnabledArrayPair                m_secondaryColorArray;
        EnabledArrayPair                m_indexArray;
        EnabledArrayPair                m_fogArray;
        EnabledTexCoordArrayList        m_texCoordArrayList;
        EnabledVertexAttribArrayList    m_vertexAttribArrayList;

        unsigned int                    m_currentActiveTextureUnit;
        unsigned int                    m_currentClientActiveTextureUnit;

		const crPixelBufferObject*        m_currentPBO;
        
        inline ModeMap& getOrCreateTextureModeMap(unsigned int unit)
        {        
            if (unit>=m_textureModeMapList.size()) m_textureModeMapList.resize(unit+1);
            return m_textureModeMapList[unit];
        }


        inline AttributeMap& getOrCreateTextureAttributeMap(unsigned int unit)
        {        
            if (unit>=m_textureAttributeMapList.size()) m_textureAttributeMapList.resize(unit+1);
            return m_textureAttributeMapList[unit];
        }
        
        inline void pushModeList(ModeMap& modeMap,const crStateSet::ModeList& modeList);
        inline void pushAttributeList(AttributeMap& attributeMap,const crStateSet::AttributeList& attributeList);
		inline void pushUniformList(UniformMap& uniformMap,const crStateSet::UniformList& uniformList);

        inline void popModeList(ModeMap& modeMap,const crStateSet::ModeList& modeList);
        inline void popAttributeList(AttributeMap& attributeMap,const crStateSet::AttributeList& attributeList);
		inline void popUniformList(UniformMap& uniformMap,const crStateSet::UniformList& uniformList);

        inline void applyModeList(ModeMap& modeMap,const crStateSet::ModeList& modeList);
        inline void applyAttributeList(AttributeMap& attributeMap,const crStateSet::AttributeList& attributeList);
		inline void applyUniformList(UniformMap& uniformMap,const crStateSet::UniformList& uniformList);

        inline void applyModeMap(ModeMap& modeMap);
        inline void applyAttributeMap(AttributeMap& attributeMap);
        inline void applyUniformMap(UniformMap& uniformMap);

        void haveAppliedMode(ModeMap& modeMap,crStateAttribute::VRMode mode,crStateAttribute::VRModeValue value);
        void haveAppliedMode(ModeMap& modeMap,crStateAttribute::VRMode mode);
        void haveAppliedAttribute(AttributeMap& attributeMap,const crStateAttribute* attribute);
        void haveAppliedAttribute(AttributeMap& attributeMap,crStateAttribute::Type type);
        bool getLastAppliedMode(const ModeMap& modeMap,crStateAttribute::VRMode mode) const;
        const crStateAttribute* getLastAppliedAttribute(const AttributeMap& attributeMap,crStateAttribute::Type type) const;


        mutable bool m_isSecondaryColorSupportResolved;
        mutable bool m_isSecondaryColorSupported;
        bool computeSecondaryColorSupported() const;

        mutable bool m_isFogCoordSupportResolved;
        mutable bool m_isFogCoordSupported;
        bool computeFogCoordSupported() const;

        mutable bool m_isVertexBufferObjectSupportResolved;
        mutable bool m_isVertexBufferObjectSupported;
        bool computeVertexBufferObjectSupported() const;

		crVector4 m_lastAppliedColor;
		bool m_colordirty;
		bool m_ignorecolor;
		unsigned int m_lastmode;
		//crVector4ub m_lastmask;
		std::set<int> m_shaderTextureSet;

		typedef void (APIENTRY * ActiveTextureProc) (GLenum texture);
		typedef void (APIENTRY * FogCoordPointerProc) (GLenum type, GLsizei stride, const GLvoid *pointer);
		typedef void (APIENTRY * SecondaryColorPointerProc) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
		typedef void (APIENTRY * MultiTexCoord4fProc) (GLenum target, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
		typedef void (APIENTRY * VertexAttrib4fProc)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
		typedef void (APIENTRY * VertexAttrib4fvProc)(GLuint index, const GLfloat *v);
		typedef void (APIENTRY * VertexAttribPointerProc) (unsigned int, GLint, GLenum, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
		typedef void (APIENTRY * VertexAttribIPointerProc) (unsigned int, GLint, GLenum, GLsizei stride, const GLvoid *pointer);
		typedef void (APIENTRY * VertexAttribLPointerProc) (unsigned int, GLint, GLenum, GLsizei stride, const GLvoid *pointer);
		typedef void (APIENTRY * EnableVertexAttribProc) (unsigned int);
		typedef void (APIENTRY * DisableVertexAttribProc) (unsigned int);
		typedef void (APIENTRY * BindBufferProc) (GLenum target, GLuint buffer);

		typedef void (APIENTRY * DrawArraysInstancedProc)( GLenum mode, GLint first, GLsizei count, GLsizei primcount );
		typedef void (APIENTRY * DrawElementsInstancedProc)( GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount );

		//bool                        _extensionProcsInitialized;
		//GLint                       _glMaxTextureCoords;
		//GLint                       _glMaxTextureUnits;
		ActiveTextureProc           _glClientActiveTexture;//
		ActiveTextureProc           _glActiveTexture;//
		//MultiTexCoord4fProc         _glMultiTexCoord4f;
		//VertexAttrib4fProc          _glVertexAttrib4f;
		//VertexAttrib4fvProc         _glVertexAttrib4fv;
		FogCoordPointerProc         _glFogCoordPointer;//
		SecondaryColorPointerProc   _glSecondaryColorPointer;//
		VertexAttribPointerProc     _glVertexAttribPointer;//
		//VertexAttribIPointerProc    _glVertexAttribIPointer;
		//VertexAttribLPointerProc    _glVertexAttribLPointer;
		EnableVertexAttribProc      _glEnableVertexAttribArray;//
		DisableVertexAttribProc     _glDisableVertexAttribArray;//
		//BindBufferProc              _glBindBuffer;
		//DrawArraysInstancedProc     _glDrawArraysInstanced;
		//DrawElementsInstancedProc   _glDrawElementsInstanced;
};

inline void crState::pushModeList(ModeMap& modeMap,const crStateSet::ModeList& modeList)
{
    for(crStateSet::ModeList::const_iterator mitr=modeList.begin();
        mitr!=modeList.end();
        ++mitr)
    {
        // get the mode stack for incomming GLmode {mitr->first}.
        ModeStack& ms = modeMap[mitr->first];
        if (ms.valueVec.empty())
        {
            // first pair so simply push incomming pair to back.
            ms.valueVec.push_back(mitr->second);
        }
        else if ((ms.valueVec.back() & crStateAttribute::OVERRIDE) && !(mitr->second & crStateAttribute::PROTECTED)) // check the existing override flag 
        {
            // push existing back since override keeps the previoius value.
            ms.valueVec.push_back(ms.valueVec.back());
        }
        else 
        {
            // no override on so simply push incomming pair to back.
            ms.valueVec.push_back(mitr->second);
        }
        ms.changed = true;
    }
}

inline void crState::pushAttributeList(AttributeMap& attributeMap,const crStateSet::AttributeList& attributeList)
{
    for(crStateSet::AttributeList::const_iterator aitr=attributeList.begin();
        aitr!=attributeList.end();
        ++aitr)
    {
        // get the attribute stack for incomming type {aitr->first}.
        AttributeStack& as = attributeMap[aitr->first];
        if (as.attributeVec.empty())
        {
            // first pair so simply push incomming pair to back.
            as.attributeVec.push_back(
                AttributePair(aitr->second.first.get(),aitr->second.second));
        }
        else if ((as.attributeVec.back().second & crStateAttribute::OVERRIDE) && !(aitr->second.second & crStateAttribute::PROTECTED)) // check the existing override flag 
        {
            // push existing back since override keeps the previoius value.
            as.attributeVec.push_back(as.attributeVec.back());
        }
        else 
        {
            // no override on so simply push incomming pair to back.
            as.attributeVec.push_back(
                AttributePair(aitr->second.first.get(),aitr->second.second));
        }
        as.changed = true;
    }
}

inline void crState::pushUniformList(UniformMap& uniformMap,const crStateSet::UniformList& uniformList)
{
	for(crStateSet::UniformList::const_iterator aitr=uniformList.begin();
		aitr!=uniformList.end();
		++aitr)
	{
		// get the attribute stack for incoming type {aitr->first}.
		UniformStack& us = uniformMap[aitr->first];
		if (us.uniformVec.empty())
		{
			// first pair so simply push incoming pair to back.
			us.uniformVec.push_back(
				UniformStack::UniformPair(aitr->second.first.get(),aitr->second.second));
		}
		else if ((us.uniformVec.back().second & crStateAttribute::OVERRIDE) && !(aitr->second.second & crStateAttribute::PROTECTED)) // check the existing override flag
		{
			// push existing back since override keeps the previous value.
			us.uniformVec.push_back(us.uniformVec.back());
		}
		else
		{
			// no override on so simply push incoming pair to back.
			us.uniformVec.push_back(
				UniformStack::UniformPair(aitr->second.first.get(),aitr->second.second));
		}
	}
}


inline void crState::popModeList(ModeMap& modeMap,const crStateSet::ModeList& modeList)
{
    for(crStateSet::ModeList::const_iterator mitr=modeList.begin();
        mitr!=modeList.end();
        ++mitr)
    {
        // get the mode stack for incomming GLmode {mitr->first}.
        ModeStack& ms = modeMap[mitr->first];
        if (!ms.valueVec.empty())
        {
            ms.valueVec.pop_back();
        }
        ms.changed = true;
    }
}

inline void crState::popAttributeList(AttributeMap& attributeMap,const crStateSet::AttributeList& attributeList)
{
    for(crStateSet::AttributeList::const_iterator aitr=attributeList.begin();
        aitr!=attributeList.end();
        ++aitr)
    {
        // get the attribute stack for incomming type {aitr->first}.
        AttributeStack& as = attributeMap[aitr->first];
        if (!as.attributeVec.empty())
        {
            as.attributeVec.pop_back();
        }
        as.changed = true;
    }
}

inline void crState::popUniformList(UniformMap& uniformMap,const crStateSet::UniformList& uniformList)
{
	for(crStateSet::UniformList::const_iterator aitr=uniformList.begin();
		aitr!=uniformList.end();
		++aitr)
	{
		// get the attribute stack for incoming type {aitr->first}.
		UniformStack& us = uniformMap[aitr->first];
		if (!us.uniformVec.empty())
		{
			us.uniformVec.pop_back();
		}
	}
}

inline void crState::applyModeList(ModeMap& modeMap,const crStateSet::ModeList& modeList)
{
	ModeMap::iterator this_mitr=modeMap.begin();
	if(modeList.empty())
	{
		// iterator over the remaining state modes to apply any previous changes.
		for(;
			this_mitr!=modeMap.end();
			++this_mitr)
		{
			// note VRMode = this_mitr->first
			ModeStack& ms = this_mitr->second;
			if (ms.changed)
			{
				ms.changed = false;
				if (!ms.valueVec.empty())
				{
					bool new_value = ms.valueVec.back() & crStateAttribute::ON;
					applyMode(this_mitr->first,new_value,ms);
				}
				else
				{
					// assume default of disabled.
					applyMode(this_mitr->first,ms.global_default_value,ms);

				}

			}
		}        
	}
	else
	{
		crStateSet::ModeList::const_iterator ds_mitr = modeList.begin();

		while (this_mitr!=modeMap.end() && ds_mitr!=modeList.end())
		{
			if (this_mitr->first<ds_mitr->first)
			{

				// note VRMode = this_mitr->first
				ModeStack& ms = this_mitr->second;
				if (ms.changed)
				{
					ms.changed = false;
					if (!ms.valueVec.empty())
					{
						bool new_value = ms.valueVec.back() & crStateAttribute::ON;
						applyMode(this_mitr->first,new_value,ms);
					}
					else
					{
						// assume default of disabled.
						applyMode(this_mitr->first,ms.global_default_value,ms);

					}

				}

				++this_mitr;

			}
			else if (ds_mitr->first<this_mitr->first)
			{

				// ds_mitr->first is a new mode, therefore 
				// need to insert a new mode entry for ds_mistr->first.
				ModeStack& ms = modeMap[ds_mitr->first];

				bool new_value = ds_mitr->second & crStateAttribute::ON;
				applyMode(ds_mitr->first,new_value,ms);

				// will need to disable this mode on next apply so set it to changed.
				ms.changed = true;

				++ds_mitr;

			}
			else
			{
				// this_mitr & ds_mitr refer to the same mode, check the overide
				// if any otherwise just apply the incomming mode.

				ModeStack& ms = this_mitr->second;

				if (!ms.valueVec.empty() && (ms.valueVec.back() & crStateAttribute::OVERRIDE) && !(ds_mitr->second & crStateAttribute::PROTECTED))
				{
					// override is on, there just treat as a normal apply on modes.

					if (ms.changed)
					{
						ms.changed = false;
						bool new_value = ms.valueVec.back() & crStateAttribute::ON;
						applyMode(this_mitr->first,new_value,ms);

					}
				}
				else
				{
					// no override on or no previous entry, therefore consider incomming mode.
					bool new_value = ds_mitr->second & crStateAttribute::ON;
					if (applyMode(ds_mitr->first,new_value,ms))
					{
						ms.changed = true;
					}
				}

				++this_mitr;
				++ds_mitr;
			}
		}

		// iterator over the remaining state modes to apply any previous changes.
		for(;
			this_mitr!=modeMap.end();
			++this_mitr)
		{
			// note VRMode = this_mitr->first
			ModeStack& ms = this_mitr->second;
			if (ms.changed)
			{
				ms.changed = false;
				if (!ms.valueVec.empty())
				{
					bool new_value = ms.valueVec.back() & crStateAttribute::ON;
					applyMode(this_mitr->first,new_value,ms);
				}
				else
				{
					// assume default of disabled.
					applyMode(this_mitr->first,ms.global_default_value,ms);

				}

			}
		}        
		// iterator over the remaining incomming modes to apply any new mode.
		for(;
			ds_mitr!=modeList.end();
			++ds_mitr)
		{
			ModeStack& ms = modeMap[ds_mitr->first];

			bool new_value = ds_mitr->second & crStateAttribute::ON;
			applyMode(ds_mitr->first,new_value,ms);

			// will need to disable this mode on next apply so set it to changed.
			ms.changed = true;
		}
	}
}

inline void crState::applyAttributeList(AttributeMap& attributeMap,const crStateSet::AttributeList& attributeList)
{
	AttributeMap::iterator this_aitr=attributeMap.begin();
    if(attributeList.empty())
	{
		// iterator over the remaining state modes to apply any previous changes.
		for(;
			this_aitr!=attributeMap.end();
			++this_aitr)
		{
			// note attribute type = this_aitr->first
			AttributeStack& as = this_aitr->second;
			if (as.changed)
			{
				as.changed = false;
				if (!as.attributeVec.empty())
				{
					const crStateAttribute* new_attr = as.attributeVec.back().first;
					applyAttribute(new_attr,as);
				}
				else
				{
					applyGlobalDefaultAttribute(as);
				}
			}
		}
	}
	else
	{
		crStateSet::AttributeList::const_iterator ds_aitr=attributeList.begin();
		while (this_aitr!=attributeMap.end() && ds_aitr!=attributeList.end())
		{
			if (this_aitr->first<ds_aitr->first)
			{

				// note attribute type = this_aitr->first
				AttributeStack& as = this_aitr->second;
				if (as.changed)
				{
					as.changed = false;
					if (!as.attributeVec.empty())
					{
						const crStateAttribute* new_attr = as.attributeVec.back().first;
						applyAttribute(new_attr,as);
					}
					else
					{
						applyGlobalDefaultAttribute(as);
					}
				}

				++this_aitr;

			}
			else if (ds_aitr->first<this_aitr->first)
			{

				// ds_mitr->first is a new attribute, therefore 
				// need to insert a new attribute entry for ds_aistr->first.
				AttributeStack& as = attributeMap[ds_aitr->first];

				const crStateAttribute* new_attr = ds_aitr->second.first.get();
				applyAttribute(new_attr,as);

				// will need to disable this mode on next apply so set it to changed.
				as.changed = true;

				++ds_aitr;

			}
			else
			{
				// this_mitr & ds_mitr refer to the same attribute, check the overide
				// if any otherwise just apply the incomming attribute

				AttributeStack& as = this_aitr->second;

				if (!as.attributeVec.empty() && (as.attributeVec.back().second & crStateAttribute::OVERRIDE) && !(ds_aitr->second.second & crStateAttribute::PROTECTED))
				{
					// override is on, there just treat as a normal apply on modes.

					if (as.changed)
					{
						as.changed = false;
						const crStateAttribute* new_attr = as.attributeVec.back().first;
						applyAttribute(new_attr,as);
					}
				}
				else
				{
					// no override on or no previous entry, therefore consider incomming mode.
					const crStateAttribute* new_attr = ds_aitr->second.first.get();
					if (applyAttribute(new_attr,as))
					{
						as.changed = true;
					}
				}

				++this_aitr;
				++ds_aitr;
			}
		}

		// iterator over the remaining state modes to apply any previous changes.
		for(;
			this_aitr!=attributeMap.end();
			++this_aitr)
		{
			// note attribute type = this_aitr->first
			AttributeStack& as = this_aitr->second;
			if (as.changed)
			{
				as.changed = false;
				if (!as.attributeVec.empty())
				{
					const crStateAttribute* new_attr = as.attributeVec.back().first;
					applyAttribute(new_attr,as);
				}
				else
				{
					applyGlobalDefaultAttribute(as);
				}
			}
		}
		// iterator over the remaining incomming modes to apply any new mode.
		for(;
			ds_aitr!=attributeList.end();
			++ds_aitr)
		{
			// ds_mitr->first is a new attribute, therefore 
			// need to insert a new attribute entry for ds_aistr->first.
			AttributeStack& as = attributeMap[ds_aitr->first];

			const crStateAttribute* new_attr = ds_aitr->second.first.get();
			applyAttribute(new_attr,as);

			// will need to update this attribute on next apply so set it to changed.
			as.changed = true;
		}
	}
}

inline void crState::applyUniformList(UniformMap& uniformMap,const crStateSet::UniformList& uniformList)
{
	if (!m_lastAppliedProgramObject) return;

	crStateSet::UniformList::const_iterator ds_aitr=uniformList.begin();

	UniformMap::iterator this_aitr=uniformMap.begin();

	while (this_aitr!=uniformMap.end() && ds_aitr!=uniformList.end())
	{
		if (this_aitr->first<ds_aitr->first)
		{
			// note attribute type = this_aitr->first
			UniformStack& as = this_aitr->second;
			if (!as.uniformVec.empty())
			{
				if(as.uniformVec.back().first) m_lastAppliedProgramObject->apply(*as.uniformVec.back().first);
			}

			++this_aitr;

		}
		else if (ds_aitr->first<this_aitr->first)
		{
			if(ds_aitr->second.first.get()) m_lastAppliedProgramObject->apply(*(ds_aitr->second.first.get()));

			++ds_aitr;
		}
		else
		{
			// this_mitr & ds_mitr refer to the same attribute, check the override
			// if any otherwise just apply the incoming attribute

			UniformStack& as = this_aitr->second;

			if (!as.uniformVec.empty() && (as.uniformVec.back().second & crStateAttribute::OVERRIDE) && !(ds_aitr->second.second & crStateAttribute::PROTECTED))
			{
				// override is on, just treat as a normal apply on uniform.
				if(as.uniformVec.back().first) m_lastAppliedProgramObject->apply(*as.uniformVec.back().first);
			}
			else
			{
				// no override on or no previous entry, therefore consider incoming attribute.
				if(ds_aitr->second.first.get()) m_lastAppliedProgramObject->apply(*(ds_aitr->second.first.get()));
			}

			++this_aitr;
			++ds_aitr;
		}
	}

	// iterator over the remaining state attributes to apply any previous changes.
	for(;
		this_aitr!=uniformMap.end();
		++this_aitr)
	{
		// note attribute type = this_aitr->first
		UniformStack& as = this_aitr->second;
		if (!as.uniformVec.empty())
		{
			m_lastAppliedProgramObject->apply(*as.uniformVec.back().first);
		}
	}

	// iterator over the remaining incoming attribute to apply any new attribute.
	for(;
		ds_aitr!=uniformList.end();
		++ds_aitr)
	{
		m_lastAppliedProgramObject->apply(*(ds_aitr->second.first.get()));
	}
}


inline void crState::applyModeMap(ModeMap& modeMap)
{
    for(ModeMap::iterator mitr=modeMap.begin();
        mitr!=modeMap.end();
        ++mitr)
    {
        // note VRMode = mitr->first
        ModeStack& ms = mitr->second;
        if (ms.changed)
        {
            ms.changed = false;
            if (!ms.valueVec.empty())
            {
                bool new_value = ms.valueVec.back() & crStateAttribute::ON;
                applyMode(mitr->first,new_value,ms);
            }
            else
            {
                // assume default of disabled.
                applyMode(mitr->first,ms.global_default_value,ms);
            }
            
        }
    }        
}

inline void crState::applyAttributeMap(AttributeMap& attributeMap)
{
    for(AttributeMap::iterator aitr=attributeMap.begin();
        aitr!=attributeMap.end();
        ++aitr)
    {
        AttributeStack& as = aitr->second;
        if (as.changed)
        {
            as.changed = false;
            if (!as.attributeVec.empty())
            {
                const crStateAttribute* new_attr = as.attributeVec.back().first;
                applyAttribute(new_attr,as);
            }
            else
            {
                applyGlobalDefaultAttribute(as);
            }
        }
    }        
}

inline void crState::applyUniformMap(UniformMap& uniformMap)
{
	if (!m_lastAppliedProgramObject) return;

	for(UniformMap::iterator aitr=uniformMap.begin();
		aitr!=uniformMap.end();
		++aitr)
	{
		UniformStack& as = aitr->second;
		if (!as.uniformVec.empty())
		{
			m_lastAppliedProgramObject->apply(*as.uniformVec.back().first);
		}
	}
}

}

#endif
