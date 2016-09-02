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
#include <CRCore/crState.h>
#include <CRCore/crNotify.h>
using namespace std;
using namespace CRCore;

//std::set<int> crState::s_shaderTextureSet;
crState::crState()
{
	m_shaderTextureSet.insert(TEXTURE_BUMPMAP);
	m_shaderTextureSet.insert(TEXTURE_SHADOWMAP);
	m_shaderTextureSet.insert(TEXTURE_SHADOWMAP2);
	m_shaderTextureSet.insert(TEXTURE_SPECULARMAP);
	m_shaderTextureSet.insert(TEXTURE_EMISSIVEMAP);
	m_shaderTextureSet.insert(TEXTURE_NORMALMAP2);
	m_shaderTextureSet.insert(TEXTURE_SPECULARMAP2);
	m_shaderTextureSet.insert(TEXTURE_NORMALMAP3);
	m_shaderTextureSet.insert(TEXTURE_SPECULARMAP3);
	m_shaderTextureSet.insert(TEXTURE_EMISSIVEMAP3);

	m_graphicsContext = 0;
	m_contextID = 0;
    m_identity = new CRCore::RefMatrix(); // default RefMatrix constructs to identity.
    m_initialViewMatrix = m_identity;
    m_projection = m_identity;
    m_modelView = m_identity;

    m_abortRenderingPtr = 0;    
    //m_reportErrors = false;

    m_currentActiveTextureUnit=0;
    m_currentClientActiveTextureUnit=0;

	m_currentPBO = 0;

    m_isSecondaryColorSupportResolved = false;
    m_isSecondaryColorSupported = false;

    m_isFogCoordSupportResolved = false;
    m_isFogCoordSupported = false;

    m_isVertexBufferObjectSupportResolved = false;
    m_isVertexBufferObjectSupported = false;

	m_lastAppliedProgramObject = 0;

	m_execute = true;
	m_currentunit = 0;
	m_colordirty = true;
	m_ignorecolor = false;
	m_lastmode = 0;
}

crState::~crState()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crState()"<< std::endl;
}

void crState::reset()
{
	disableAllVertexArrays();
#if 1
	for(ModeMap::iterator mitr=m_modeMap.begin();
		mitr!=m_modeMap.end();
		++mitr)
	{
		ModeStack& ms = mitr->second;
		ms.valueVec.clear();
		ms.last_applied_value = !ms.global_default_value;
		ms.changed = true;
	}
	m_modeMap.clear();
#else
	m_modeMap.clear();
#endif

	m_modeMap[GL_DEPTH_TEST].global_default_value = true;
	m_modeMap[GL_DEPTH_TEST].changed = true;

	// go through all active crStateAttribute's, setting to change to force update,
	// the idea is to leave only the global defaults left.
	for(AttributeMap::iterator aitr=m_attributeMap.begin();
		aitr!=m_attributeMap.end();
		++aitr)
	{
		AttributeStack& as = aitr->second;
		as.attributeVec.clear();
		as.last_applied_attribute = NULL;
		as.changed = true;
	}
	m_attributeMap.clear();
	// we can do a straight clear, we arn't intrested in GL_DEPTH_TEST defaults in texture modes.
	for(TextureModeMapList::iterator tmmItr=m_textureModeMapList.begin();
		tmmItr!=m_textureModeMapList.end();
		++tmmItr)
	{
		tmmItr->clear();
	}
	m_textureModeMapList.clear();
	// empty all the texture attributes as per normal attributes, leaving only the global defaults left.
	for(TextureAttributeMapList::iterator tamItr=m_textureAttributeMapList.begin();
		tamItr!=m_textureAttributeMapList.end();
		++tamItr)
	{
		AttributeMap& attributeMap = *tamItr;
		// go through all active crStateAttribute's, setting to change to force update.
		for(AttributeMap::iterator aitr=attributeMap.begin();
			aitr!=attributeMap.end();
			++aitr)
		{
			AttributeStack& as = aitr->second;
			as.attributeVec.clear();
			as.last_applied_attribute = NULL;
			as.changed = true;
		}
	}
	m_textureAttributeMapList.clear();

	m_drawStateStack.clear();

	m_modelView = m_identity;
	m_projection = m_identity;

#if 1
	// reset active texture unit values and call OpenGL
	// note, this OpenGL op precludes the use of State::reset() without a
	// valid graphics context, therefore the new implementation below 
	// is preferred.
	setActiveTextureUnit(0);
#else
	// reset active texture unit values without calling OpenGL
	m_currentActiveTextureUnit = 0;
	m_currentClientActiveTextureUnit = 0;
#endif
	m_currentunit = 0;
	m_lastAppliedProgramObject = 0;

	for(AppliedProgramObjectSet::iterator apitr=m_appliedProgramObjectSet.begin();
		apitr!=m_appliedProgramObjectSet.end();
		++apitr)
	{
		(*apitr)->resetAppliedUniforms();
	}

	m_appliedProgramObjectSet.clear();


	// what about uniforms??? need to clear them too...
	// go through all active Unfirom's, setting to change to force update,
	// the idea is to leave only the global defaults left.
	for(UniformMap::iterator uitr=m_uniformMap.begin();
		uitr!=m_uniformMap.end();
		++uitr)
	{
		UniformStack& us = uitr->second;
		us.uniformVec.clear();
	}
	m_uniformMap.clear();

	m_currentPBO = NULL;
	m_colordirty = true;
	m_ignorecolor = false;
}

void crState::setInitialViewMatrix(const CRCore::RefMatrix* matrix)
{
    if (matrix) m_initialViewMatrix = matrix;
    else m_initialViewMatrix = m_identity;

    m_initialInverseViewMatrix.invert(*m_initialViewMatrix);
}

void crState::pushStateSet(const crStateSet* dstate)
{
    m_drawStateStack.push_back(dstate);
    if (dstate)
    {

        pushModeList(m_modeMap,dstate->getModeList());

        // iterator through texture modes.        
        unsigned int unit;
        const crStateSet::TextureModeList& ds_textureModeList = dstate->getTextureModeList();
        for(unit=0;unit<ds_textureModeList.size();++unit)
        {
            pushModeList(getOrCreateTextureModeMap(unit),ds_textureModeList[unit]);
        }

        pushAttributeList(m_attributeMap,dstate->getAttributeList());

        // iterator through texture attributes.
        const crStateSet::TextureAttributeList& ds_textureAttributeList = dstate->getTextureAttributeList();
        for(unit=0;unit<ds_textureAttributeList.size();++unit)
        {
            pushAttributeList(getOrCreateTextureAttributeMap(unit),ds_textureAttributeList[unit]);
        }

		pushUniformList(m_uniformMap,dstate->getUniformList());
    }
}

void crState::popAllStateSets()
{
    while (!m_drawStateStack.empty()) popStateSet();
    
    applyProjectionMatrix(0);
    applyModelViewMatrix(0);

	m_lastAppliedProgramObject = 0;
}

void crState::popStateSet()
{
    if (m_drawStateStack.empty()) return;
    
    const crStateSet* dstate = m_drawStateStack.back();

    if (dstate)
    {

        popModeList(m_modeMap,dstate->getModeList());

        // iterator through texture modes.        
        unsigned int unit;
        const crStateSet::TextureModeList& ds_textureModeList = dstate->getTextureModeList();
        for(unit=0;unit<ds_textureModeList.size();++unit)
        {
            popModeList(getOrCreateTextureModeMap(unit),ds_textureModeList[unit]);
        }

        popAttributeList(m_attributeMap,dstate->getAttributeList());

        // iterator through texture attributes.
        const crStateSet::TextureAttributeList& ds_textureAttributeList = dstate->getTextureAttributeList();
        for(unit=0;unit<ds_textureAttributeList.size();++unit)
        {
            popAttributeList(getOrCreateTextureAttributeMap(unit),ds_textureAttributeList[unit]);
        }

		popUniformList(m_uniformMap,dstate->getUniformList());
    }
    
    // remove the top draw state from the stack.
    m_drawStateStack.pop_back();
}

void crState::captureCurrentState(crStateSet& stateset) const
{
    // empty the stateset first.
    stateset.setAllToInherit();
    
    for(ModeMap::const_iterator mitr=m_modeMap.begin();
        mitr!=m_modeMap.end();
        ++mitr)
    {
        // note VRMode = mitr->first
        const ModeStack& ms = mitr->second;
        if (!ms.valueVec.empty())
        {
            stateset.setMode(mitr->first,ms.valueVec.back());
        }
    }        

    for(AttributeMap::const_iterator aitr=m_attributeMap.begin();
        aitr!=m_attributeMap.end();
        ++aitr)
    {
        const AttributeStack& as = aitr->second;
        if (!as.attributeVec.empty())
        {
            stateset.setAttribute(const_cast<crStateAttribute*>(as.attributeVec.back().first));
        }
    }        

}

// revert to using maximum for consistency, maximum should be defined by STLport on VS.
// // visual studio 6.0 doesn't appear to define maximum?!? So do our own here.. 
// template<class T>
// T mymax(const T& a,const T& b)
// {
//     return (((a) > (b)) ? (a) : (b));
// }

void crState::getAttributeAndModeList( const crStateSet *dstate,StateAttributeList **out_attributeList, StateModeList **out_modeList, TextureList **out_textureList )
{
	m_attributeList.clear();
	m_modeList.clear();
	m_textureList.clear();
	m_execute = false;

	apply(dstate);

	*out_attributeList = &m_attributeList;
	*out_modeList = &m_modeList;
	*out_textureList = &m_textureList;

	m_execute = true;

}

void crState::apply(const crStateSet* dstate)
{
    //if (m_reportErrors) checkErrors("start of crState::apply(crStateSet*)");

    // equivilant to:
    //pushStateSet(dstate);
    //apply();
    //popStateSet();
    //return;

    if (dstate)
    {
		//try{
        applyModeList(m_modeMap,dstate->getModeListBuf());
        applyAttributeList(m_attributeMap,dstate->getAttributeListBuf());
		//}
		//catch (...) {
		//	CRCore::notify(CRCore::ALWAYS)<<"crState::apply(const crStateSet* dstate) error"<< std::endl;
		//	const crStateSet::ModeList& modeList = dstate->getModeList();
		//	const crStateSet::AttributeList& attributeList = dstate->getAttributeList();
  //          CRCore::notify(CRCore::ALWAYS)<<"ModeList size = "<<modeList.size()<< std::endl;
		//	CRCore::notify(CRCore::ALWAYS)<<"AttributeList size = "<<attributeList.size()<< std::endl;
		//}
        const crStateSet::TextureModeList& ds_textureModeList = dstate->getTextureModeListBuf();
        const crStateSet::TextureAttributeList& ds_textureAttributeList = dstate->getTextureAttributeListBuf();

        unsigned int unit;
        unsigned int unitMax = maximum(static_cast<unsigned int>(ds_textureModeList.size()),static_cast<unsigned int>(ds_textureAttributeList.size()));
        unitMax = maximum(static_cast<unsigned int>(unitMax),static_cast<unsigned int>(m_textureModeMapList.size()));
        unitMax = maximum(static_cast<unsigned int>(unitMax),static_cast<unsigned int>(m_textureAttributeMapList.size()));
      
		//const GL2Extensions* extensions = GL2Extensions::Get(0,false);
		//bool isGlslSupported = extensions && extensions->isGlslSupported();
		bool isGlslSupported = true;
		for(unit=0;unit<unitMax;++unit)
        {
			if(!isGlslSupported && m_shaderTextureSet.find(unit) != m_shaderTextureSet.end()/* > TEXTURE_LIGHTMAP*/)
			{
				break;
			}
            if (setActiveTextureUnit(unit))
            {
				m_currentunit = unit;
				//typedef std::pair<unsigned int, ref_ptr<crStateAttribute> > Temp_TexturePair;
    //            
				//crStateSet::AttributeList::const_iterator ds_aitr=ds_textureAttributeList[unit].begin();
				//for( ; ds_aitr != ds_textureAttributeList[unit].end(); ds_aitr++ )
				//{
				//    m_textureList.insert(Temp_TexturePair( unit, const_cast<crStateAttribute*>(ds_aitr->second.first.get())));  
				//}

				if (unit<ds_textureModeList.size()) applyModeList(getOrCreateTextureModeMap(unit),ds_textureModeList[unit]);
				else if (unit<m_textureModeMapList.size()) applyModeMap(m_textureModeMapList[unit]);
                
                if (unit<ds_textureAttributeList.size()) applyAttributeList(getOrCreateTextureAttributeMap(unit),ds_textureAttributeList[unit]);
                else if (unit<m_textureAttributeMapList.size()) applyAttributeMap(m_textureAttributeMapList[unit]);
			}
        }
#if 1   
		if(isGlslSupported)
		    applyUniformList(m_uniformMap,dstate->getUniformListBuf());
#else                
		if (m_lastAppliedProgramObject)
		{
			for(StateSetStack::iterator sitr=m_drawStateStack.begin();
				sitr!=m_drawStateStack.end();
				++sitr)
			{
				const crStateSet* stateset = *sitr;
				const crStateSet::UniformList& uniformList = stateset->getUniformListBuf();
				for(crStateSet::UniformList::const_iterator itr=uniformList.begin();
					itr!=uniformList.end();
					++itr)
				{
					m_lastAppliedProgramObject->apply(*(itr->second.first));
				}
			}

			const crStateSet::UniformList& uniformList = dstate->getUniformListBuf();
			for(crStateSet::UniformList::const_iterator itr=uniformList.begin();
				itr!=uniformList.end();
				++itr)
			{
				m_lastAppliedProgramObject->apply(*(itr->second.first));
			}
		}
#endif    
    }
    else
    {
        // no incomming stateset, so simply apply state.
        apply();
    }
    //if (m_reportErrors) checkErrors("end of crState::apply(crStateSet*)");
}

void crState::apply()
{
    //if (m_reportErrors) checkErrors("start of crState::apply()");

    // go through all active OpenGL modes, enabling/disable where
    // appropriate.
    applyModeMap(m_modeMap);

    // go through all active crStateAttribute's, applying where appropriate.
    applyAttributeMap(m_attributeMap);
       
    unsigned int unit;
    unsigned int unitMax = maximum(m_textureModeMapList.size(),m_textureAttributeMapList.size());
    for(unit=0;unit<unitMax;++unit)
    {
        if (setActiveTextureUnit(unit))
        {
	/*		typedef std::pair<unsigned int, ref_ptr<crStateAttribute> > Temp_TexturePair;

			AttributeMap::iterator aitr=m_textureAttributeMapList[unit].begin();
			for( ; aitr != m_textureAttributeMapList[unit].end(); aitr++ )
			{
				m_textureList.insert(Temp_TexturePair( unit, const_cast<crStateAttribute*>(aitr->second.attributeVec.back().first)));  
			}*/
			m_currentunit = unit;

            if (unit<m_textureModeMapList.size()) applyModeMap(m_textureModeMapList[unit]);
            if (unit<m_textureAttributeMapList.size()) applyAttributeMap(m_textureAttributeMapList[unit]);
        }
#if 1        
		applyUniformMap(m_uniformMap);
#else        
		if (m_lastAppliedProgramObject && !m_drawStateStack.empty())
		{
			for(StateSetStack::iterator sitr=m_drawStateStack.begin();
				sitr!=m_drawStateStack.end();
				++sitr)
			{
				const crStateSet* stateset = *sitr;
				const crStateSet::UniformList& uniformList = stateset->getUniformListBuf();
				for(crStateSet::UniformList::const_iterator itr=uniformList.begin();
					itr!=uniformList.end();
					++itr)
				{
					m_lastAppliedProgramObject->apply(*(itr->second.first));
				}
			}
		}
#endif
	}
    //if (m_reportErrors) checkErrors("end of crState::apply()");

	dirtyAllVertexArrays();
	m_colordirty = true;
	m_ignorecolor = false;
}

void crState::haveAppliedMode(crStateAttribute::VRMode mode,crStateAttribute::VRModeValue value)
{
    haveAppliedMode(m_modeMap,mode,value);
}

void crState::haveAppliedMode(crStateAttribute::VRMode mode)
{
    haveAppliedMode(m_modeMap,mode);
}

void crState::haveAppliedAttribute(const crStateAttribute* attribute)
{
    haveAppliedAttribute(m_attributeMap,attribute);
}

void crState::haveAppliedAttribute(crStateAttribute::Type type)
{
    haveAppliedAttribute(m_attributeMap,type);
}

bool crState::getLastAppliedMode(crStateAttribute::VRMode mode) const
{
    return getLastAppliedMode(m_modeMap,mode);
}

const crStateAttribute* crState::getLastAppliedAttribute(crStateAttribute::Type type) const
{
    return getLastAppliedAttribute(m_attributeMap,type);
}


void crState::haveAppliedTextureMode(unsigned int unit,crStateAttribute::VRMode mode,crStateAttribute::VRModeValue value)
{
    haveAppliedMode(getOrCreateTextureModeMap(unit),mode,value);
}

void crState::haveAppliedTextureMode(unsigned int unit,crStateAttribute::VRMode mode)
{
    haveAppliedMode(getOrCreateTextureModeMap(unit),mode);
}

void crState::haveAppliedTextureAttribute(unsigned int unit,const crStateAttribute* attribute)
{
    haveAppliedAttribute(getOrCreateTextureAttributeMap(unit),attribute);
}

void crState::haveAppliedTextureAttribute(unsigned int unit,crStateAttribute::Type type)
{
    haveAppliedAttribute(getOrCreateTextureAttributeMap(unit),type);
}

bool crState::getLastAppliedTextureMode(unsigned int unit,crStateAttribute::VRMode mode) const
{
    if (unit>=m_textureModeMapList.size()) return false;
    return getLastAppliedMode(m_textureModeMapList[unit],mode);
}

const crStateAttribute* crState::getLastAppliedTextureAttribute(unsigned int unit,crStateAttribute::Type type) const
{
    if (unit>=m_textureAttributeMapList.size()) return false;
    return getLastAppliedAttribute(m_textureAttributeMapList[unit],type);
}


void crState::haveAppliedMode(ModeMap& modeMap,crStateAttribute::VRMode mode,crStateAttribute::VRModeValue value)
{
    ModeStack& ms = modeMap[mode];

    ms.last_applied_value = value & crStateAttribute::ON;

    // will need to disable this mode on next apply so set it to changed.
    ms.changed = true;    
}

/** mode has been set externally, update state to reflect this setting.*/
void crState::haveAppliedMode(ModeMap& modeMap,crStateAttribute::VRMode mode)
{
    ModeStack& ms = modeMap[mode];

    // don't know what last applied value is can't apply it.
    // assume that it has changed by toggle the value of last_applied_value.
    ms.last_applied_value = !ms.last_applied_value;

    // will need to disable this mode on next apply so set it to changed.
    ms.changed = true;    
}

/** attribute has been applied externally, update state to reflect this setting.*/
void crState::haveAppliedAttribute(AttributeMap& attributeMap,const crStateAttribute* attribute)
{
    if (attribute)
    {
        AttributeStack& as = attributeMap[attribute->getType()];

        as.last_applied_attribute = attribute;

        // will need to update this attribute on next apply so set it to changed.
        as.changed = true;
    }
}

void crState::haveAppliedAttribute(AttributeMap& attributeMap,crStateAttribute::Type type)
{
    
    AttributeMap::iterator itr = attributeMap.find(type);
    if (itr!=attributeMap.end())
    {
        AttributeStack& as = itr->second;
        as.last_applied_attribute = 0L;

        // will need to update this attribute on next apply so set it to changed.
        as.changed = true;
    }
}

bool crState::getLastAppliedMode(const ModeMap& modeMap,crStateAttribute::VRMode mode) const
{
    ModeMap::const_iterator itr = modeMap.find(mode);
    if (itr!=modeMap.end())
    {
        const ModeStack& ms = itr->second;
        return ms.last_applied_value;
    }
    else
    {
        return false;
    }
}

const crStateAttribute* crState::getLastAppliedAttribute(const AttributeMap& attributeMap,crStateAttribute::Type type) const
{
    AttributeMap::const_iterator itr = attributeMap.find(type);
    if (itr!=attributeMap.end())
    {
        const AttributeStack& as = itr->second;
        return as.last_applied_attribute.get();
    }
    else
    {
        return NULL;
    }
}

void crState::dirtyAllModes()
{
    for(ModeMap::iterator mitr=m_modeMap.begin();
        mitr!=m_modeMap.end();
        ++mitr)
    {
        ModeStack& ms = mitr->second;
        ms.last_applied_value = !ms.last_applied_value;
        ms.changed = true;    

    }        

    for(TextureModeMapList::iterator tmmItr=m_textureModeMapList.begin();
        tmmItr!=m_textureModeMapList.end();
        ++tmmItr)
    {
        for(ModeMap::iterator mitr=tmmItr->begin();
            mitr!=tmmItr->end();
            ++mitr)
        {
            ModeStack& ms = mitr->second;
            ms.last_applied_value = !ms.last_applied_value;
            ms.changed = true;    

        }        
    }
}

void crState::dirtyAllAttributes()
{
    for(AttributeMap::iterator aitr=m_attributeMap.begin();
        aitr!=m_attributeMap.end();
        ++aitr)
    {
        AttributeStack& as = aitr->second;
        as.last_applied_attribute = 0;
        as.changed = true;
    }
    

    for(TextureAttributeMapList::iterator tamItr=m_textureAttributeMapList.begin();
        tamItr!=m_textureAttributeMapList.end();
        ++tamItr)
    {
        AttributeMap& attributeMap = *tamItr;
        for(AttributeMap::iterator aitr=attributeMap.begin();
            aitr!=attributeMap.end();
            ++aitr)
        {
            AttributeStack& as = aitr->second;
            as.last_applied_attribute = 0;
            as.changed = true;
        }
    }

}

crPolytope crState::getViewFrustum() const
{
    crPolytope cv;
    cv.setToUnitFrustum();
    cv.transformProvidingInverse((*m_modelView)*(*m_projection));
    return cv;
}

void crState::disableAllVertexArrays()
{
    disableVertexPointer();
    disableTexCoordPointersAboveAndIncluding(0);
    disableVertexAttribPointersAboveAndIncluding(0);
    disableColorPointer();
    disableFogCoordPointer();
    disableIndexPointer();
    disableNormalPointer();
    disableSecondaryColorPointer();
}

void crState::dirtyAllVertexArrays()
{
    dirtyVertexPointer();
    dirtyTexCoordPointersAboveAndIncluding(0);
    dirtyVertexAttribPointersAboveAndIncluding(0);
    dirtyColorPointer();
    dirtyFogCoordPointer();
    dirtyIndexPointer();
    dirtyNormalPointer();
    dirtySecondaryColorPointer();
}
void crState::lazyDisablingOfVertexAttributes()
{
	//if (!m_useVertexAttributeAliasing)
	{
		m_vertexArray.m_lazy_disable = true;
		m_normalArray.m_lazy_disable = true;
		m_colorArray.m_lazy_disable = true;
		m_secondaryColorArray.m_lazy_disable = true;
		m_fogArray.m_lazy_disable = true;
		for(EnabledTexCoordArrayList::iterator itr = m_texCoordArrayList.begin();
			itr != m_texCoordArrayList.end();
			++itr)
		{
			itr->m_lazy_disable = true;
		}
	}

	for(EnabledVertexAttribArrayList::iterator itr = m_vertexAttribArrayList.begin();
		itr != m_vertexAttribArrayList.end();
		++itr)
	{
		itr->m_lazy_disable = true;
	}
}

void crState::applyDisablingOfVertexAttributes()
{
	//if (!m_useVertexAttributeAliasing)
	{
		if (m_vertexArray.m_lazy_disable) disableVertexPointer();
		if (m_normalArray.m_lazy_disable) disableNormalPointer();
		if (m_colorArray.m_lazy_disable) disableColorPointer();
		if (m_secondaryColorArray.m_lazy_disable) disableSecondaryColorPointer();
		if (m_fogArray.m_lazy_disable) disableFogCoordPointer();
		for(unsigned int i=0; i<m_texCoordArrayList.size(); ++i)
		{
			if (m_texCoordArrayList[i].m_lazy_disable) disableTexCoordPointer(i);
		}
	}
	for(unsigned int i=0; i<m_vertexAttribArrayList.size(); ++i)
	{
		if (m_vertexAttribArrayList[i].m_lazy_disable) disableVertexAttribPointer(i);
	}
}
#include <Driver/GLDrv/crStateDrv.h>