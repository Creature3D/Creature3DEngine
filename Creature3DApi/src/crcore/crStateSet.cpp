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
#include <stdio.h>

#include <CRCore/crStateSet.h>
#include <CRCore/crState.h>
#include <CRCore/crNotify.h>
#include <CRCore/crBrain.h>
//#include <CRCore/crResource.h>
#include <CRCore/crDrawable.h>
#include <CRCore/crNode.h>
#include <CRCore/crProgram.h>
#include <set>


using namespace CRCore;


class TextureModeSet
{

    public:

        TextureModeSet()
        {
            m_textureModeSet.insert(GL_TEXTURE_1D);
            m_textureModeSet.insert(GL_TEXTURE_2D);
            m_textureModeSet.insert(GL_TEXTURE_3D);
            m_textureModeSet.insert(GL_TEXTURE_CUBE_MAP);
	        m_textureModeSet.insert(GL_TEXTURE_RECTANGLE_NV);

            m_textureModeSet.insert(GL_TEXTURE_GEN_Q);
            m_textureModeSet.insert(GL_TEXTURE_GEN_R);
            m_textureModeSet.insert(GL_TEXTURE_GEN_S);
            m_textureModeSet.insert(GL_TEXTURE_GEN_T);
        }
        
        bool isTextureMode(crStateAttribute::VRMode mode) const
        {
            return m_textureModeSet.find(mode)!=m_textureModeSet.end();
        }

    protected:

        std::set<crStateAttribute::VRMode> m_textureModeSet;
        
};

TextureModeSet s_textureModeSet;

/////////////////////////////////////////
//
// crStateSet
/////////////////////////////////////////////
crStateSet::crStateSet()
{
    setDataVariance(CRCore::crStateAttribute::STATIC);

    m_renderingHint = DEFAULT_BIN;
	m_numChildrenRequiringUpdateTraversal = 0;
	//m_numChildrenRequiringEventTraversal = 0;

    setRenderBinToInherit();

	m_needCompile = true;
	
	m_statesetMode = SS_Normal;

	memset(m_textureOperate,0,16);
	setTextureOperate(TEXTURE_BASEMAP,2);
	//setTextureOperate(TEXTURE_SPECULARMAP,2);
	//setTextureOperate(TEXTURE_EMISSIVEMAP,2);

	memset(m_alphaOperate,0,16);
	setAlphaOperate(TEXTURE_BASEMAP,3);
	//m_renderInited = false;
	memset(m_textureUV,0,16);
}

crStateSet::crStateSet(const crStateSet& rhs,const crCopyOp& copyop):crBase(rhs,copyop)
{
    m_modeList = rhs.m_modeList;

    for(AttributeList::const_iterator itr=rhs.m_attributeList.begin();
        itr!=rhs.m_attributeList.end();
        ++itr)
    {
        crStateAttribute::Type type = itr->first;
        const RefAttributePair& rap = itr->second;
        crStateAttribute* attr = copyop(rap.first.get());
        if (attr) 
		{
			m_attributeList[type]=RefAttributePair(attr,rap.second);
		    attr->addParent(this);
		}
    }
    
    // copy texture related modes.
    m_textureModeList = rhs.m_textureModeList;
    
    // set up the size of the texture attribute list.
    m_textureAttributeList.resize(rhs.m_textureAttributeList.size());
    
    // copy the contents across.
    for(unsigned int i=0;i<rhs.m_textureAttributeList.size();++i)
    {
        
        AttributeList& lhs_attributeList = m_textureAttributeList[i];
        const AttributeList& rhs_attributeList = rhs.m_textureAttributeList[i];
        for(AttributeList::const_iterator itr=rhs_attributeList.begin();
            itr!=rhs_attributeList.end();
            ++itr)
        {
            crStateAttribute::Type type = itr->first;
            const RefAttributePair& rap = itr->second;
            crStateAttribute* attr = copyop(rap.first.get());
            if (attr) 
			{
				lhs_attributeList[type]=RefAttributePair(attr,rap.second);
				attr->addParent(this);
			}
        }
    }
    
	for(UniformList::const_iterator rhs_uitr = rhs.m_uniformList.begin();
		rhs_uitr != rhs.m_uniformList.end();
		++rhs_uitr)
	{
		const std::string& name = rhs_uitr->first;
		const RefUniformPair& rup = rhs_uitr->second;
		crUniform* uni = copyop(rup.first.get());
		if (uni)
		{
			m_uniformList[name] = RefUniformPair(uni, rup.second);
			uni->addParent(this);
		}
	}

    m_renderingHint = rhs.m_renderingHint;

    m_binMode = rhs.m_binMode;
    m_binNum = rhs.m_binNum;
    m_binName = rhs.m_binName;

	m_updateCallback = rhs.m_updateCallback;
	m_numChildrenRequiringUpdateTraversal = rhs.m_numChildrenRequiringUpdateTraversal;

	//m_eventCallback = rhs.m_eventCallback;
	//m_numChildrenRequiringEventTraversal = rhs.m_numChildrenRequiringEventTraversal;

	m_needCompile = true;//rhs.m_needCompile;
    m_statesetMode = rhs.m_statesetMode;
	memcpy(m_textureOperate,rhs.m_textureOperate,16);
	memcpy(m_alphaOperate,rhs.m_alphaOperate,16);
	memcpy(m_textureUV,rhs.m_textureUV,16);
	//m_renderInited = false;
}

crStateSet::~crStateSet()
{
    // note, all attached state attributes will be automatically
    // unreferenced by ref_ptr<> and therefore there is no need to
    // delete the memory manually.
	//CRCore::notify(CRCore::ALWAYS)<<"crStateSet delete = "<<getName().c_str()<<std::endl;

	clear();
}

int crStateSet::compare(const crStateSet& rhs,bool compareAttributeContents) const
{
	//CRCore::notify(CRCore::ALWAYS)<<getName().c_str()<<" compare "<<rhs.getName().c_str()<<std::endl;
	//bool report = false;
	//if(getName().compare("smoke.img") == 0 && rhs.getName().compare("smoke.img") == 0) report = true;
	if(m_statesetMode < rhs.m_statesetMode) return -1;
    if(m_statesetMode > rhs.m_statesetMode) return 1;

	if(m_binMode < rhs.m_binMode) return -1;
	if(m_binMode > rhs.m_binMode) return 1;
	if(m_binNum < rhs.m_binNum) return -1;
	if(m_binNum > rhs.m_binNum) return 1;
	if(m_binName < rhs.m_binName) return -1;
	if(m_binName > rhs.m_binName) return 1;

    if (m_textureAttributeList.size()<rhs.m_textureAttributeList.size()) return -1;
    if (m_textureAttributeList.size()>rhs.m_textureAttributeList.size()) return 1;

	int tops = memcmp(m_textureOperate,rhs.m_textureOperate,16);
	if(tops!=0) return tops;
	tops = memcmp(m_alphaOperate,rhs.m_alphaOperate,16);
	if(tops!=0) return tops;
	tops = memcmp(m_textureUV,rhs.m_textureUV,16);
	if(tops!=0) return tops;
	//if(report) CRCore::notify(CRCore::ALWAYS)<<"crStateSet::compare 1"<<std::endl;

    for(unsigned int ai=0;ai<m_textureAttributeList.size();++ai)
    {
        const AttributeList& rhs_attributeList = m_textureAttributeList[ai];
        const AttributeList& lhs_attributeList = rhs.m_textureAttributeList[ai];
        if (compareAttributeContents)
        {
            // now check to see how the attributes compare.
            AttributeList::const_iterator lhs_attr_itr = lhs_attributeList.begin();
            AttributeList::const_iterator rhs_attr_itr = rhs_attributeList.begin();
            while (lhs_attr_itr!=lhs_attributeList.end() && rhs_attr_itr!=rhs_attributeList.end())
            {
                if      (lhs_attr_itr->first<rhs_attr_itr->first) return -1;
                else if (rhs_attr_itr->first<lhs_attr_itr->first) return 1;
                if      (*(lhs_attr_itr->second.first)<*(rhs_attr_itr->second.first)) return -1;
                else if (*(rhs_attr_itr->second.first)<*(lhs_attr_itr->second.first)) return 1;
				//if(lhs_attr_itr->second.first!=rhs_attr_itr->second.first)
				//{//lhs_attr_itr->second.first和rhs_attr_itr->second.first内容相同但指针不同，将其合并
				//	//lhs_attr_itr->second.first = rhs_attr_itr->second.first.get();
				//	const_cast<AttributeList&>(rhs_attributeList)[rhs_attr_itr->first] = RefAttributePair(const_cast<crStateAttribute *>(lhs_attr_itr->second.first->getDrawCallback()?lhs_attr_itr->second.first.get():rhs_attr_itr->second.first.get()),rhs_attr_itr->second.second);
				//}
                if      (lhs_attr_itr->second.second<rhs_attr_itr->second.second) return -1;
                else if (rhs_attr_itr->second.second<lhs_attr_itr->second.second) return 1;
                ++lhs_attr_itr;
                ++rhs_attr_itr;
            }
            if (lhs_attr_itr==lhs_attributeList.end())
            {
                if (rhs_attr_itr!=rhs_attributeList.end()) return -1;
            }
            else if (rhs_attr_itr == rhs_attributeList.end()) return 1;
            //if(report) CRCore::notify(CRCore::ALWAYS)<<"crStateSet::compare 2"<<std::endl;
        }
        else // just compare pointers.
        {
            // now check to see how the attributes compare.
            AttributeList::const_iterator lhs_attr_itr = lhs_attributeList.begin();
            AttributeList::const_iterator rhs_attr_itr = rhs_attributeList.begin();
            while (lhs_attr_itr!=lhs_attributeList.end() && rhs_attr_itr!=rhs_attributeList.end())
            {
                if      (lhs_attr_itr->first<rhs_attr_itr->first) return -1;
                else if (rhs_attr_itr->first<lhs_attr_itr->first) return 1;
                if      (lhs_attr_itr->second.first<rhs_attr_itr->second.first) return -1;
                else if (rhs_attr_itr->second.first<lhs_attr_itr->second.first) return 1;
                if      (lhs_attr_itr->second.second<rhs_attr_itr->second.second) return -1;
                else if (rhs_attr_itr->second.second<lhs_attr_itr->second.second) return 1;
                ++lhs_attr_itr;
                ++rhs_attr_itr;
            }
            if (lhs_attr_itr==lhs_attributeList.end())
            {
                if (rhs_attr_itr!=rhs_attributeList.end()) return -1;
            }
            else if (rhs_attr_itr == rhs_attributeList.end()) return 1;
			//if(report) CRCore::notify(CRCore::ALWAYS)<<"crStateSet::compare 21"<<std::endl;

        }
    }

    
    // now check the rest of the non texture attributes
    if (compareAttributeContents)
    {
        // now check to see how the attributes compare.
        AttributeList::const_iterator lhs_attr_itr = m_attributeList.begin();
        AttributeList::const_iterator rhs_attr_itr = rhs.m_attributeList.begin();
        while (lhs_attr_itr!=m_attributeList.end() && rhs_attr_itr!=rhs.m_attributeList.end())
        {
            if      (lhs_attr_itr->first<rhs_attr_itr->first) return -1;
            else if (rhs_attr_itr->first<lhs_attr_itr->first) return 1;
            if      (*(lhs_attr_itr->second.first)<*(rhs_attr_itr->second.first)) return -1;
            else if (*(rhs_attr_itr->second.first)<*(lhs_attr_itr->second.first)) return 1;
			//if(lhs_attr_itr->second.first!=rhs_attr_itr->second.first)
			//{//lhs_attr_itr->second.first和rhs_attr_itr->second.first内容相同但指针不同，将其合并
			//	const_cast<AttributeList&>(rhs.m_attributeList)[rhs_attr_itr->first] = RefAttributePair(const_cast<crStateAttribute *>(lhs_attr_itr->second.first->getDrawCallback()?lhs_attr_itr->second.first.get():rhs_attr_itr->second.first.get()),rhs_attr_itr->second.second);	
			//}
            if      (lhs_attr_itr->second.second<rhs_attr_itr->second.second) return -1;
            else if (rhs_attr_itr->second.second<lhs_attr_itr->second.second) return 1;
            ++lhs_attr_itr;
            ++rhs_attr_itr;
        }
        if (lhs_attr_itr==m_attributeList.end())
        {
            if (rhs_attr_itr!=rhs.m_attributeList.end()) return -1;
        }
        else if (rhs_attr_itr == rhs.m_attributeList.end()) return 1;
		//if(report) CRCore::notify(CRCore::ALWAYS)<<"crStateSet::compare 3"<<std::endl;

    }
    else // just compare pointers.
    {
        // now check to see how the attributes compare.
        AttributeList::const_iterator lhs_attr_itr = m_attributeList.begin();
        AttributeList::const_iterator rhs_attr_itr = rhs.m_attributeList.begin();
        while (lhs_attr_itr!=m_attributeList.end() && rhs_attr_itr!=rhs.m_attributeList.end())
        {
            if      (lhs_attr_itr->first<rhs_attr_itr->first) return -1;
            else if (rhs_attr_itr->first<lhs_attr_itr->first) return 1;
            if      (lhs_attr_itr->second.first<rhs_attr_itr->second.first) return -1;
            else if (rhs_attr_itr->second.first<lhs_attr_itr->second.first) return 1;
            if      (lhs_attr_itr->second.second<rhs_attr_itr->second.second) return -1;
            else if (rhs_attr_itr->second.second<lhs_attr_itr->second.second) return 1;
            ++lhs_attr_itr;
            ++rhs_attr_itr;
        }
        if (lhs_attr_itr==m_attributeList.end())
        {
            if (rhs_attr_itr!=rhs.m_attributeList.end()) return -1;
        }
        else if (rhs_attr_itr == rhs.m_attributeList.end()) return 1;
		//if(report) CRCore::notify(CRCore::ALWAYS)<<"crStateSet::compare 31"<<std::endl;

    }
    
    // we've got here so attributes must be equal...    

    if (m_textureModeList.size()<rhs.m_textureModeList.size()) return -1;
    if (m_textureModeList.size()>rhs.m_textureModeList.size()) return 1;

    // check to see how the modes compare.
    // first check the rest of the texture modes
    for(unsigned int ti=0;ti<m_textureModeList.size();++ti)
    {
        const ModeList& lhs_modeList = m_textureModeList[ti];
        const ModeList& rhs_modeList = rhs.m_textureModeList[ti];

        ModeList::const_iterator lhs_mode_itr = lhs_modeList.begin();
        ModeList::const_iterator rhs_mode_itr = rhs_modeList.begin();
        while (lhs_mode_itr!=lhs_modeList.end() && rhs_mode_itr!=rhs_modeList.end())
        {
            if      (lhs_mode_itr->first<rhs_mode_itr->first) return -1;
            else if (rhs_mode_itr->first<lhs_mode_itr->first) return 1;
            if      (lhs_mode_itr->second<rhs_mode_itr->second) return -1;
            else if (rhs_mode_itr->second<lhs_mode_itr->second) return 1;
            ++lhs_mode_itr;
            ++rhs_mode_itr;
        }
        if (lhs_mode_itr==lhs_modeList.end())
        {
            if (rhs_mode_itr!=rhs_modeList.end()) return -1;
        }
        else if (rhs_mode_itr == rhs_modeList.end()) return 1;
		//if(report) CRCore::notify(CRCore::ALWAYS)<<"crStateSet::compare 4"<<std::endl;

    }
    // check non texture modes.
    ModeList::const_iterator lhs_mode_itr = m_modeList.begin();
    ModeList::const_iterator rhs_mode_itr = rhs.m_modeList.begin();
    while (lhs_mode_itr!=m_modeList.end() && rhs_mode_itr!=rhs.m_modeList.end())
    {
        if      (lhs_mode_itr->first<rhs_mode_itr->first) return -1;
        else if (rhs_mode_itr->first<lhs_mode_itr->first) return 1;
        if      (lhs_mode_itr->second<rhs_mode_itr->second) return -1;
        else if (rhs_mode_itr->second<lhs_mode_itr->second) return 1;
        ++lhs_mode_itr;
        ++rhs_mode_itr;
    }
    if (lhs_mode_itr==m_modeList.end())
    {
        if (rhs_mode_itr!=rhs.m_modeList.end()) return -1;
    }
    else if (rhs_mode_itr == rhs.m_modeList.end()) return 1;
	//if(report) CRCore::notify(CRCore::ALWAYS)<<"crStateSet::compare 5"<<std::endl;

	// check uniforms.
	// now check the rest of the non texture attributes
	if (compareAttributeContents)
	{
		// now check to see how the attributes compare.
		UniformList::const_iterator lhs_uniform_itr = m_uniformList.begin();
		UniformList::const_iterator rhs_uniform_itr = rhs.m_uniformList.begin();
		while (lhs_uniform_itr!=m_uniformList.end() && rhs_uniform_itr!=rhs.m_uniformList.end())
		{
			if      (lhs_uniform_itr->first<rhs_uniform_itr->first) return -1;
			else if (rhs_uniform_itr->first<lhs_uniform_itr->first) return 1;
			if      (*(lhs_uniform_itr->second.first)<*(rhs_uniform_itr->second.first)) return -1;
			else if (*(rhs_uniform_itr->second.first)<*(lhs_uniform_itr->second.first)) return 1;
			//if(lhs_uniform_itr->second.first!=rhs_uniform_itr->second.first)
			//{//lhs_attr_itr->second.first和rhs_attr_itr->second.first内容相同但指针不同，将其合并
			//	lhs_uniform_itr->second.first = rhs_uniform_itr->second.first;
			//	//const_cast<UniformList&>((rhs.m_uniformList)[rhs_uniform_itr->first]) = RefUniformPair(const_cast<UniformList *>(lhs_uniform_itr->second.first.get(),rhs_uniform_itr->second.second);	
			//}
			if      (lhs_uniform_itr->second.second<rhs_uniform_itr->second.second) return -1;
			else if (rhs_uniform_itr->second.second<lhs_uniform_itr->second.second) return 1;
			++lhs_uniform_itr;
			++rhs_uniform_itr;
		}
		if (lhs_uniform_itr==m_uniformList.end())
		{
			if (rhs_uniform_itr!=rhs.m_uniformList.end()) return -1;
		}
		else if (rhs_uniform_itr == rhs.m_uniformList.end()) return 1;
		//if(report) CRCore::notify(CRCore::ALWAYS)<<"crStateSet::compare 6"<<std::endl;
	}
	else // just compare pointers.
	{
		// now check to see how the attributes compare.
		UniformList::const_iterator lhs_uniform_itr = m_uniformList.begin();
		UniformList::const_iterator rhs_uniform_itr = rhs.m_uniformList.begin();
		while (lhs_uniform_itr!=m_uniformList.end() && rhs_uniform_itr!=rhs.m_uniformList.end())
		{
			if      (lhs_uniform_itr->first<rhs_uniform_itr->first) return -1;
			else if (rhs_uniform_itr->first<lhs_uniform_itr->first) return 1;
			if      (lhs_uniform_itr->second.first<rhs_uniform_itr->second.first) return -1;
			else if (rhs_uniform_itr->second.first<lhs_uniform_itr->second.first) return 1;
			if      (lhs_uniform_itr->second.second<rhs_uniform_itr->second.second) return -1;
			else if (rhs_uniform_itr->second.second<lhs_uniform_itr->second.second) return 1;
			++lhs_uniform_itr;
			++rhs_uniform_itr;
		}
		if (lhs_uniform_itr==m_uniformList.end())
		{
			if (rhs_uniform_itr!=rhs.m_uniformList.end()) return -1;
		}
		else if (rhs_uniform_itr == rhs.m_uniformList.end()) return 1;

		//if(report) CRCore::notify(CRCore::ALWAYS)<<"crStateSet::compare 61"<<std::endl;
	}
	//if(report) CRCore::notify(CRCore::ALWAYS)<<"crStateSet::compare same"<<std::endl;
    return 0;
}

int crStateSet::compare(const crStateSet& rhs,bool compareAttributeContents)
{
	if(m_statesetMode < rhs.m_statesetMode) return -1;
	if(m_statesetMode > rhs.m_statesetMode) return 1;

	if(m_binMode < rhs.m_binMode) return -1;
	if(m_binMode > rhs.m_binMode) return 1;
	if(m_binNum < rhs.m_binNum) return -1;
	if(m_binNum > rhs.m_binNum) return 1;
	if(m_binName < rhs.m_binName) return -1;
	if(m_binName > rhs.m_binName) return 1;

	if (m_textureAttributeList.size()<rhs.m_textureAttributeList.size()) return -1;
	if (m_textureAttributeList.size()>rhs.m_textureAttributeList.size()) return 1;
	
	int tops = memcmp(m_textureOperate,rhs.m_textureOperate,16);
	if(tops!=0) return tops;
	tops = memcmp(m_alphaOperate,rhs.m_alphaOperate,16);
	if(tops!=0) return tops;
	tops = memcmp(m_textureUV,rhs.m_textureUV,16);
	if(tops!=0) return tops;

	for(unsigned int ai=0;ai<m_textureAttributeList.size();++ai)
	{
		const AttributeList& rhs_attributeList = m_textureAttributeList[ai];
		const AttributeList& lhs_attributeList = rhs.m_textureAttributeList[ai];
		if (compareAttributeContents)
		{
			// now check to see how the attributes compare.
			AttributeList::const_iterator lhs_attr_itr = lhs_attributeList.begin();
			AttributeList::const_iterator rhs_attr_itr = rhs_attributeList.begin();
			while (lhs_attr_itr!=lhs_attributeList.end() && rhs_attr_itr!=rhs_attributeList.end())
			{
				if      (lhs_attr_itr->first<rhs_attr_itr->first) return -1;
				else if (rhs_attr_itr->first<lhs_attr_itr->first) return 1;
				if      (*(lhs_attr_itr->second.first)<*(rhs_attr_itr->second.first)) return -1;
				else if (*(rhs_attr_itr->second.first)<*(lhs_attr_itr->second.first)) return 1;
				//if(lhs_attr_itr->second.first!=rhs_attr_itr->second.first)
				//{//lhs_attr_itr->second.first和rhs_attr_itr->second.first内容相同但指针不同，将其合并
				//	const_cast<AttributeList&>(rhs_attributeList)[rhs_attr_itr->first] = RefAttributePair(const_cast<crStateAttribute *>(lhs_attr_itr->second.first->getDrawCallback()?lhs_attr_itr->second.first.get():rhs_attr_itr->second.first.get()),rhs_attr_itr->second.second);
				//}
				if      (lhs_attr_itr->second.second<rhs_attr_itr->second.second) return -1;
				else if (rhs_attr_itr->second.second<lhs_attr_itr->second.second) return 1;
				++lhs_attr_itr;
				++rhs_attr_itr;
			}
			if (lhs_attr_itr==lhs_attributeList.end())
			{
				if (rhs_attr_itr!=rhs_attributeList.end()) return -1;
			}
			else if (rhs_attr_itr == rhs_attributeList.end()) return 1;
		}
		else // just compare pointers.
		{
			// now check to see how the attributes compare.
			AttributeList::const_iterator lhs_attr_itr = lhs_attributeList.begin();
			AttributeList::const_iterator rhs_attr_itr = rhs_attributeList.begin();
			while (lhs_attr_itr!=lhs_attributeList.end() && rhs_attr_itr!=rhs_attributeList.end())
			{
				if      (lhs_attr_itr->first<rhs_attr_itr->first) return -1;
				else if (rhs_attr_itr->first<lhs_attr_itr->first) return 1;
				if      (lhs_attr_itr->second.first<rhs_attr_itr->second.first) return -1;
				else if (rhs_attr_itr->second.first<lhs_attr_itr->second.first) return 1;
				if      (lhs_attr_itr->second.second<rhs_attr_itr->second.second) return -1;
				else if (rhs_attr_itr->second.second<lhs_attr_itr->second.second) return 1;
				++lhs_attr_itr;
				++rhs_attr_itr;
			}
			if (lhs_attr_itr==lhs_attributeList.end())
			{
				if (rhs_attr_itr!=rhs_attributeList.end()) return -1;
			}
			else if (rhs_attr_itr == rhs_attributeList.end()) return 1;
		}
	}


	// now check the rest of the non texture attributes
	if (compareAttributeContents)
	{
		// now check to see how the attributes compare.
		AttributeList::const_iterator lhs_attr_itr = m_attributeList.begin();
		AttributeList::const_iterator rhs_attr_itr = rhs.m_attributeList.begin();
		while (lhs_attr_itr!=m_attributeList.end() && rhs_attr_itr!=rhs.m_attributeList.end())
		{
			if      (lhs_attr_itr->first<rhs_attr_itr->first) return -1;
			else if (rhs_attr_itr->first<lhs_attr_itr->first) return 1;
			if      (*(lhs_attr_itr->second.first)<*(rhs_attr_itr->second.first)) return -1;
			else if (*(rhs_attr_itr->second.first)<*(lhs_attr_itr->second.first)) return 1;
			//if(lhs_attr_itr->second.first!=rhs_attr_itr->second.first)
			//{//lhs_attr_itr->second.first和rhs_attr_itr->second.first内容相同但指针不同，将其合并
			//	const_cast<AttributeList&>(rhs.m_attributeList)[rhs_attr_itr->first] = RefAttributePair(const_cast<crStateAttribute *>(lhs_attr_itr->second.first->getDrawCallback()?lhs_attr_itr->second.first.get():rhs_attr_itr->second.first.get()),rhs_attr_itr->second.second);	
			//}
			if      (lhs_attr_itr->second.second<rhs_attr_itr->second.second) return -1;
			else if (rhs_attr_itr->second.second<lhs_attr_itr->second.second) return 1;
			++lhs_attr_itr;
			++rhs_attr_itr;
		}
		if (lhs_attr_itr==m_attributeList.end())
		{
			if (rhs_attr_itr!=rhs.m_attributeList.end()) return -1;
		}
		else if (rhs_attr_itr == rhs.m_attributeList.end()) return 1;
	}
	else // just compare pointers.
	{
		// now check to see how the attributes compare.
		AttributeList::const_iterator lhs_attr_itr = m_attributeList.begin();
		AttributeList::const_iterator rhs_attr_itr = rhs.m_attributeList.begin();
		while (lhs_attr_itr!=m_attributeList.end() && rhs_attr_itr!=rhs.m_attributeList.end())
		{
			if      (lhs_attr_itr->first<rhs_attr_itr->first) return -1;
			else if (rhs_attr_itr->first<lhs_attr_itr->first) return 1;
			if      (lhs_attr_itr->second.first<rhs_attr_itr->second.first) return -1;
			else if (rhs_attr_itr->second.first<lhs_attr_itr->second.first) return 1;
			if      (lhs_attr_itr->second.second<rhs_attr_itr->second.second) return -1;
			else if (rhs_attr_itr->second.second<lhs_attr_itr->second.second) return 1;
			++lhs_attr_itr;
			++rhs_attr_itr;
		}
		if (lhs_attr_itr==m_attributeList.end())
		{
			if (rhs_attr_itr!=rhs.m_attributeList.end()) return -1;
		}
		else if (rhs_attr_itr == rhs.m_attributeList.end()) return 1;
	}

	// we've got here so attributes must be equal...    

	if (m_textureModeList.size()<rhs.m_textureModeList.size()) return -1;
	if (m_textureModeList.size()>rhs.m_textureModeList.size()) return 1;

	// check to see how the modes compare.
	// first check the rest of the texture modes
	for(unsigned int ti=0;ti<m_textureModeList.size();++ti)
	{
		const ModeList& lhs_modeList = m_textureModeList[ti];
		const ModeList& rhs_modeList = rhs.m_textureModeList[ti];

		ModeList::const_iterator lhs_mode_itr = lhs_modeList.begin();
		ModeList::const_iterator rhs_mode_itr = rhs_modeList.begin();
		while (lhs_mode_itr!=lhs_modeList.end() && rhs_mode_itr!=rhs_modeList.end())
		{
			if      (lhs_mode_itr->first<rhs_mode_itr->first) return -1;
			else if (rhs_mode_itr->first<lhs_mode_itr->first) return 1;
			if      (lhs_mode_itr->second<rhs_mode_itr->second) return -1;
			else if (rhs_mode_itr->second<lhs_mode_itr->second) return 1;
			++lhs_mode_itr;
			++rhs_mode_itr;
		}
		if (lhs_mode_itr==lhs_modeList.end())
		{
			if (rhs_mode_itr!=rhs_modeList.end()) return -1;
		}
		else if (rhs_mode_itr == rhs_modeList.end()) return 1;
	}

	// check non texture modes.
	ModeList::const_iterator lhs_mode_itr = m_modeList.begin();
	ModeList::const_iterator rhs_mode_itr = rhs.m_modeList.begin();
	while (lhs_mode_itr!=m_modeList.end() && rhs_mode_itr!=rhs.m_modeList.end())
	{
		if      (lhs_mode_itr->first<rhs_mode_itr->first) return -1;
		else if (rhs_mode_itr->first<lhs_mode_itr->first) return 1;
		if      (lhs_mode_itr->second<rhs_mode_itr->second) return -1;
		else if (rhs_mode_itr->second<lhs_mode_itr->second) return 1;
		++lhs_mode_itr;
		++rhs_mode_itr;
	}
	if (lhs_mode_itr==m_modeList.end())
	{
		if (rhs_mode_itr!=rhs.m_modeList.end()) return -1;
	}
	else if (rhs_mode_itr == rhs.m_modeList.end()) return 1;

	// check uniforms.
	//UniformList::const_iterator lhs_uniform_itr = m_uniformList.begin();
	//UniformList::const_iterator rhs_uniform_itr = rhs.m_uniformList.begin();
	//while (lhs_uniform_itr!=m_uniformList.end() && rhs_uniform_itr!=rhs.m_uniformList.end())
	//{
	//	if      (lhs_uniform_itr->first<rhs_uniform_itr->first) return -1;
	//	else if (rhs_uniform_itr->first<lhs_uniform_itr->first) return 1;
	//	if      (lhs_uniform_itr->second<rhs_uniform_itr->second) return -1;
	//	else if (rhs_uniform_itr->second<lhs_uniform_itr->second) return 1;
	//	++lhs_uniform_itr;
	//	++rhs_uniform_itr;
	//}
	//if (lhs_uniform_itr==m_uniformList.end())
	//{
	//	if (rhs_uniform_itr!=rhs.m_uniformList.end()) return -1;
	//}
	//else if (rhs_uniform_itr == rhs.m_uniformList.end()) return 1;
	if (compareAttributeContents)
	{
		// now check to see how the attributes compare.
		UniformList::const_iterator lhs_uniform_itr = m_uniformList.begin();
		UniformList::const_iterator rhs_uniform_itr = rhs.m_uniformList.begin();
		while (lhs_uniform_itr!=m_uniformList.end() && rhs_uniform_itr!=rhs.m_uniformList.end())
		{
			if      (lhs_uniform_itr->first<rhs_uniform_itr->first) return -1;
			else if (rhs_uniform_itr->first<lhs_uniform_itr->first) return 1;
			if      (*(lhs_uniform_itr->second.first)<*(rhs_uniform_itr->second.first)) return -1;
			else if (*(rhs_uniform_itr->second.first)<*(lhs_uniform_itr->second.first)) return 1;
			//if(lhs_uniform_itr->second.first!=rhs_uniform_itr->second.first)
			//{//lhs_attr_itr->second.first和rhs_attr_itr->second.first内容相同但指针不同，将其合并
			//	lhs_uniform_itr->second.first = rhs_uniform_itr->second.first;
			//	//const_cast<UniformList&>((rhs.m_uniformList)[rhs_uniform_itr->first]) = RefUniformPair(const_cast<UniformList *>(lhs_uniform_itr->second.first.get(),rhs_uniform_itr->second.second);	
			//}
			if      (lhs_uniform_itr->second.second<rhs_uniform_itr->second.second) return -1;
			else if (rhs_uniform_itr->second.second<lhs_uniform_itr->second.second) return 1;
			++lhs_uniform_itr;
			++rhs_uniform_itr;
		}
		if (lhs_uniform_itr==m_uniformList.end())
		{
			if (rhs_uniform_itr!=rhs.m_uniformList.end()) return -1;
		}
		else if (rhs_uniform_itr == rhs.m_uniformList.end()) return 1;
	}
	else // just compare pointers.
	{
		// now check to see how the attributes compare.
		UniformList::const_iterator lhs_uniform_itr = m_uniformList.begin();
		UniformList::const_iterator rhs_uniform_itr = rhs.m_uniformList.begin();
		while (lhs_uniform_itr!=m_uniformList.end() && rhs_uniform_itr!=rhs.m_uniformList.end())
		{
			if      (lhs_uniform_itr->first<rhs_uniform_itr->first) return -1;
			else if (rhs_uniform_itr->first<lhs_uniform_itr->first) return 1;
			if      (lhs_uniform_itr->second.first<rhs_uniform_itr->second.first) return -1;
			else if (rhs_uniform_itr->second.first<lhs_uniform_itr->second.first) return 1;
			if      (lhs_uniform_itr->second.second<rhs_uniform_itr->second.second) return -1;
			else if (rhs_uniform_itr->second.second<lhs_uniform_itr->second.second) return 1;
			++lhs_uniform_itr;
			++rhs_uniform_itr;
		}
		if (lhs_uniform_itr==m_uniformList.end())
		{
			if (rhs_uniform_itr!=rhs.m_uniformList.end()) return -1;
		}
		else if (rhs_uniform_itr == rhs.m_uniformList.end()) return 1;
	}


	return 0;
}

int crStateSet::compareModes(const ModeList& lhs,const ModeList& rhs)
{
    ModeList::const_iterator lhs_mode_itr = lhs.begin();
    ModeList::const_iterator rhs_mode_itr = rhs.begin();
    while (lhs_mode_itr!=lhs.end() && rhs_mode_itr!=rhs.end())
    {
        if      (lhs_mode_itr->first<rhs_mode_itr->first) return -1;
        else if (rhs_mode_itr->first<lhs_mode_itr->first) return 1;
        if      (lhs_mode_itr->second<rhs_mode_itr->second) return -1;
        else if (rhs_mode_itr->second<lhs_mode_itr->second) return 1;
        ++lhs_mode_itr;
        ++rhs_mode_itr;
    }
    if (lhs_mode_itr==lhs.end())
    {
        if (rhs_mode_itr!=rhs.end()) return -1;
    }
    else if (rhs_mode_itr == rhs.end()) return 1;
    return 0;
}

int crStateSet::compareAttributePtrs(const AttributeList& lhs,const AttributeList& rhs)
{
    AttributeList::const_iterator lhs_attr_itr = lhs.begin();
    AttributeList::const_iterator rhs_attr_itr = rhs.begin();
    while (lhs_attr_itr!=lhs.end() && rhs_attr_itr!=rhs.end())
    {
        if      (lhs_attr_itr->first<rhs_attr_itr->first) return -1;
        else if (rhs_attr_itr->first<lhs_attr_itr->first) return 1;
        if      (lhs_attr_itr->second.first<rhs_attr_itr->second.first) return -1;
        else if (rhs_attr_itr->second.first<lhs_attr_itr->second.first) return 1;
        if      (lhs_attr_itr->second.second<rhs_attr_itr->second.second) return -1;
        else if (rhs_attr_itr->second.second<lhs_attr_itr->second.second) return 1;
        ++lhs_attr_itr;
        ++rhs_attr_itr;
    }
    if (lhs_attr_itr==lhs.end())
    {
        if (rhs_attr_itr!=rhs.end()) return -1;
    }
    else if (rhs_attr_itr == rhs.end()) return 1;
    return 0;
}

int crStateSet::compareAttributeContents(const AttributeList& lhs,const AttributeList& rhs)
{
    AttributeList::const_iterator lhs_attr_itr = lhs.begin();
    AttributeList::const_iterator rhs_attr_itr = rhs.begin();
    while (lhs_attr_itr!=lhs.end() && rhs_attr_itr!=rhs.end())
    {
        if      (lhs_attr_itr->first<rhs_attr_itr->first) return -1;
        else if (rhs_attr_itr->first<lhs_attr_itr->first) return 1;
        if      (*(lhs_attr_itr->second.first)<*(rhs_attr_itr->second.first)) return -1;
        else if (*(rhs_attr_itr->second.first)<*(lhs_attr_itr->second.first)) return 1;
		//if(lhs_attr_itr->second.first!=rhs_attr_itr->second.first)
		//{//lhs_attr_itr->second.first和rhs_attr_itr->second.first内容相同但指针不同，将其合并
		//	const_cast<AttributeList&>(rhs)[rhs_attr_itr->first] = RefAttributePair(const_cast<crStateAttribute *>(lhs_attr_itr->second.first->getDrawCallback()?lhs_attr_itr->second.first.get():rhs_attr_itr->second.first.get()),rhs_attr_itr->second.second);
		//}
        if      (lhs_attr_itr->second.second<rhs_attr_itr->second.second) return -1;
        else if (rhs_attr_itr->second.second<lhs_attr_itr->second.second) return 1;
        ++lhs_attr_itr;
        ++rhs_attr_itr;
    }
    if (lhs_attr_itr==lhs.end())
    {
        if (rhs_attr_itr!=rhs.end()) return -1;
    }
    else if (rhs_attr_itr == rhs.end()) return 1;
    return 0;
}

void crStateSet::setGlobalDefaults()
{ ///////////////////////////////////////////////////////////////   
    m_renderingHint = DEFAULT_BIN;

    setRenderBinToInherit();

    //setMode(crStateAttribute::VRMode(GL_DEPTH_TEST,NULL),crStateAttribute::ON);
    //setAttributeAndModes(new crAlphaFunc,crStateAttribute::OFF);/////////////////////
    //setAttributeAndModes(new crBlendFunc,crStateAttribute::OFF);////////////////////

 //   crMaterial *material       = new crMaterial;
 //   material->setColorMode(crMaterial::AMBIENT_AND_DIFFUSE);
 //   setAttributeAndModes(material,crStateAttribute::ON);
}


void crStateSet::setAllToInherit()
{
    m_renderingHint = DEFAULT_BIN;

    setRenderBinToInherit();

    m_modeList.clear();
    m_attributeList.clear();
    
    m_textureModeList.clear();
    m_textureAttributeList.clear();
    
}

void crStateSet::clear()
{
	m_renderingHint = DEFAULT_BIN;

	setRenderBinToInherit();

	// remove self from as attributes parent
	for(AttributeList::iterator itr=m_attributeList.begin();
		itr!=m_attributeList.end();
		++itr)
	{
		itr->second.first->removeParent(this);
	}

	m_modeList.clear();
	m_attributeList.clear();

	// remove self from as texture attributes parent
	for(unsigned int i=0;i<m_textureAttributeList.size();++i)
	{
		AttributeList& attributeList = m_textureAttributeList[i];
		for(AttributeList::iterator itr=attributeList.begin();
			itr!=attributeList.end();
			++itr)
		{
			itr->second.first->removeParent(this);
		}
	}

	m_textureModeList.clear();
	m_textureAttributeList.clear();

	//// remove self from uniforms parent
	//for(UniformList::iterator uitr = m_dirtyUniformList.begin();
	//	uitr != m_dirtyUniformList.end();
	//	++uitr)
	//{
	//	uitr->second.first->removeParent(this);
	//}
	//m_dirtyUniformList.clear();
	//m_dirtyUniformList.swap(m_uniformList);
	for(UniformList::iterator uitr = m_uniformList.begin();
		uitr != m_uniformList.end();
		++uitr)
	{
		uitr->second.first->removeParent(this);
	}
	m_uniformList.clear();

	m_statesetMode = SS_Normal;
}

void crStateSet::reset()
{
	m_parents.clear();
	// remove self from as attributes parent
	for(AttributeList::iterator itr=m_attributeList.begin();
		itr!=m_attributeList.end();
		++itr)
	{
		itr->second.first->removeParent(this);
	}

	m_modeList.clear();
	m_attributeList.clear();

	// remove self from as texture attributes parent
	for(unsigned int i=0;i<m_textureAttributeList.size();++i)
	{
		AttributeList& attributeList = m_textureAttributeList[i];
		for(AttributeList::iterator itr=attributeList.begin();
			itr!=attributeList.end();
			++itr)
		{
			itr->second.first->removeParent(this);
		}
	}

	m_textureModeList.clear();
	m_textureAttributeList.clear();

	//// remove self from uniforms parent
	//for(UniformList::iterator uitr = m_dirtyUniformList.begin();
	//	uitr != m_dirtyUniformList.end();
	//	++uitr)
	//{
	//	uitr->second.first->removeParent(this);
	//}
	//m_dirtyUniformList.clear();
	//m_dirtyUniformList.swap(m_uniformList);
	for(UniformList::iterator uitr = m_uniformList.begin();
		uitr != m_uniformList.end();
		++uitr)
	{
		uitr->second.first->removeParent(this);
	}
	m_uniformList.clear();
	m_renderingHint = DEFAULT_BIN;
	setRenderBinToInherit();
	m_statesetMode = SS_Normal;
	//delete 20130606
	//setDataVariance(CRCore::crStateAttribute::STATIC);
	//m_renderingHint = DEFAULT_BIN;
	//setRenderBinToInherit();
 //   //m_compileCallback = NULL;
	//m_needCompile = true;
	//m_updateCallback = NULL;
	//m_numChildrenRequiringUpdateTraversal = 0;
	//m_statesetMode = SS_Normal;
	//m_getRtglmShaderStr.clear();
	//memset(m_textureOperate,0,16 * sizeof(int));
	//setTextureOperate(TEXTURE_BASEMAP,2);
	////setTextureOperate(TEXTURE_SPECULARMAP,2);
	////setTextureOperate(TEXTURE_EMISSIVEMAP,2);
	//memset(m_alphaOperate,0,16 * sizeof(int));
	//setAlphaOperate(TEXTURE_BASEMAP,3);
	////m_renderInited = false;
}

void crStateSet::addParent(CRCore::crBase* object)
{
	// CRCore::notify(CRCore::INFO)<<"Adding parent"<<std::endl;

	m_parents.push_back(object);
}

void crStateSet::removeParent(CRCore::crBase* object)
{
	ParentList::iterator pitr = std::find(m_parents.begin(),m_parents.end(),object);
	if (pitr!=m_parents.end()) m_parents.erase(pitr);
}

void crStateSet::merge(const crStateSet& rhs)
{
    // merge the modes of rhs into this, 
    // this overrides rhs if OVERRIDE defined in this.
    for(ModeList::const_iterator rhs_mitr = rhs.m_modeList.begin();
        rhs_mitr != rhs.m_modeList.end();
        ++rhs_mitr)
    {
        ModeList::iterator lhs_mitr = m_modeList.find(rhs_mitr->first);
        if (lhs_mitr!=m_modeList.end())
        {
            if (!(lhs_mitr->second & crStateAttribute::OVERRIDE)) 
            {
                // override isn't on in rhs, so overrite it with incomming
                // value.
                lhs_mitr->second = rhs_mitr->second;
            }
        }
        else
        {
            // entry doesn't exist so insert it.
            m_modeList.insert(*rhs_mitr);
        }
    }

    // merge the attributes of rhs into this, 
    // this overrides rhs if OVERRIDE defined in this.
    for(AttributeList::const_iterator rhs_aitr = rhs.m_attributeList.begin();
        rhs_aitr != rhs.m_attributeList.end();
        ++rhs_aitr)
    {
        AttributeList::iterator lhs_aitr = m_attributeList.find(rhs_aitr->first);
        if (lhs_aitr!=m_attributeList.end())
        {
            if (!(lhs_aitr->second.second & crStateAttribute::OVERRIDE)) 
            {
                // override isn't on in rhs, so overrite it with incomming
                // value.
                if (lhs_aitr->second.first!=rhs_aitr->second.first)
                {
                    // new attribute so need to remove self from outgoing attribute
                    lhs_aitr->second.first->removeParent(this);

                    // override isn't on in rhs, so overrite it with incomming
                    // value.
                    lhs_aitr->second = rhs_aitr->second;
                    lhs_aitr->second.first->addParent(this);

                }
                else
                {
					// same attribute but with override to set.
					lhs_aitr->second = rhs_aitr->second;
				}

			}
		}
		else
		{
			// entry doesn't exist so insert it, and then tell it about self by adding self as parent.
			m_attributeList.insert(*rhs_aitr).first->second.first->addParent(this);
		}
	}


    if (m_textureModeList.size()<rhs.m_textureModeList.size()) m_textureModeList.resize(rhs.m_textureModeList.size());
    for(unsigned int mi=0;mi<rhs.m_textureModeList.size();++mi)
    {
        ModeList& lhs_modeList = m_textureModeList[mi];
        const ModeList& rhs_modeList = rhs.m_textureModeList[mi];
        // merge the modes of rhs into this, 
        // this overrides rhs if OVERRIDE defined in this.
        for(ModeList::const_iterator rhs_mitr = rhs_modeList.begin();
            rhs_mitr != rhs_modeList.end();
            ++rhs_mitr)
        {
            ModeList::iterator lhs_mitr = lhs_modeList.find(rhs_mitr->first);
            if (lhs_mitr!=lhs_modeList.end())
            {
                if (!(lhs_mitr->second & crStateAttribute::OVERRIDE)) 
                {
                    // override isn't on in rhs, so overrite it with incomming
                    // value.
                    lhs_mitr->second = rhs_mitr->second;
                }
            }
            else
            {
                // entry doesn't exist so insert it.
                lhs_modeList.insert(*rhs_mitr);
            }
        }
    }
    
    if (m_textureAttributeList.size()<rhs.m_textureAttributeList.size()) m_textureAttributeList.resize(rhs.m_textureAttributeList.size());
    for(unsigned int ai=0;ai<rhs.m_textureAttributeList.size();++ai)
    {
        AttributeList& lhs_attributeList = m_textureAttributeList[ai];
        const AttributeList& rhs_attributeList = rhs.m_textureAttributeList[ai];
        
        // merge the attributes of rhs into this, 
        // this overrides rhs if OVERRIDE defined in this.
        for(AttributeList::const_iterator rhs_aitr = rhs_attributeList.begin();
            rhs_aitr != rhs_attributeList.end();
            ++rhs_aitr)
        {
            AttributeList::iterator lhs_aitr = lhs_attributeList.find(rhs_aitr->first);
            if (lhs_aitr!=lhs_attributeList.end())
            {
                if (!(lhs_aitr->second.second & crStateAttribute::OVERRIDE)) 
                {
                    // override isn't on in rhs, so overrite it with incomming
                    // value.

					if (lhs_aitr->second.first!=rhs_aitr->second.first)
					{
						lhs_aitr->second.first->removeParent(this);
						lhs_aitr->second = rhs_aitr->second;
						lhs_aitr->second.first->addParent(this);
					}
					else
					{
						lhs_aitr->second = rhs_aitr->second;
					}
                }
            }
            else
            {
                // entry doesn't exist so insert it.
                lhs_attributeList.insert(*rhs_aitr).first->second.first->addParent(this);
            }
        }
    }

	// merge the uniforms of rhs into this, 
	// this overrides rhs if OVERRIDE defined in this.
	for(UniformList::const_iterator rhs_uitr = rhs.m_uniformList.begin();
		rhs_uitr != rhs.m_uniformList.end();
		++rhs_uitr)
	{
		UniformList::iterator lhs_uitr = m_uniformList.find(rhs_uitr->first);
		if (lhs_uitr!=m_uniformList.end())
		{
			if (!(lhs_uitr->second.second & crStateAttribute::OVERRIDE)) 
			{
				// override isn't on in rhs, so overrite it with incomming
				// value.

				if (lhs_uitr->second.first!=rhs_uitr->second.first)
				{
					lhs_uitr->second.first->removeParent(this);
					lhs_uitr->second = rhs_uitr->second;
					lhs_uitr->second.first->addParent(this);
				}
				else
				{
					lhs_uitr->second = rhs_uitr->second;
				}

			}
		}
		else
		{
			// entry doesn't exist so insert it and add self as parent
			m_uniformList.insert(*rhs_uitr).first->second.first->addParent(this);
		}
	}

	// need to merge rendering hints
	// but will need to think how best to do this first
	// RO, Nov. 2001.

}


void crStateSet::setMode(crStateAttribute::VRMode mode, crStateAttribute::VRModeValue value)
{
    if (!s_textureModeSet.isTextureMode(mode))
    {
        setMode(m_modeList,mode,value);
    }
    else
    {
 /*       notify(NOTICE)<<"Warning: texture mode '"<<mode<<"'passed to setMode(mode,value), "<<std::endl;
        notify(NOTICE)<<"         assuming setTextureMode(unit=0,mode,value) instead."<<std::endl;
        notify(NOTICE)<<"         please change calling code to use appropriate call."<<std::endl;*/
        setTextureMode(0,mode,value);
    }
}

void crStateSet::setModeToInherit(crStateAttribute::VRMode mode)
{
  //  if (!s_textureModeSet.isTextureMode(mode))
  //  {
  //      setModeToInherit(m_modeList,mode);
  //  }
  //  else
  //  {
  ///*      notify(NOTICE)<<"Warning: texture mode '"<<mode<<"'passed to setModeToInherit(mode), "<<std::endl;
  //      notify(NOTICE)<<"         assuming setTextureModeToInherit(unit=0,mode) instead."<<std::endl;
  //      notify(NOTICE)<<"         please change calling code to use appropriate call."<<std::endl;*/
  //      setTextureModeToInherit(0,mode);
  //  }

	ModeList::iterator itr = m_modeList.find(mode);
	if (itr!=m_modeList.end())
	{
		m_modeList.erase(itr);
	}

}

void crStateSet::removeMode(crStateAttribute::VRMode mode)
{
	if (!s_textureModeSet.isTextureMode(mode))
	{
		setModeToInherit(m_modeList,mode);
	}
	else
	{
		//notify(NOTICE)<<"Warning: texture mode '"<<mode<<"'passed to setModeToInherit(mode), "<<std::endl;
		//notify(NOTICE)<<"         assuming setTextureModeToInherit(unit=0,mode) instead."<<std::endl;
		//notify(NOTICE)<<"         please change calling code to use appropriate call."<<std::endl;

		removeTextureMode(0,mode);
	}
}

crStateAttribute::VRModeValue crStateSet::getMode(crStateAttribute::VRMode mode) const
{
    if (!s_textureModeSet.isTextureMode(mode))
    {
        return getMode(m_modeList,mode);
    }
    else
    {
        //notify(NOTICE)<<"Warning: texture mode '"<<mode<<"'passed to getMode(mode), "<<std::endl;
        //notify(NOTICE)<<"         assuming getTextureMode(unit=0,mode) instead."<<std::endl;
        //notify(NOTICE)<<"         please change calling code to use appropriate call."<<std::endl;

        return getTextureMode(0,mode);
    }
}

void crStateSet::setAttribute(crStateAttribute *attribute, crStateAttribute::OverrideValue value)
{
    if (attribute)
    {
        if (!attribute->isTextureAttribute())
        {
            setAttribute(m_attributeList,attribute,value);
        }
        else
        {
            //notify(NOTICE)<<"Warning: texture attribute '"<<attribute->className()<<"'passed to setAttribute(attr,value), "<<std::endl;
            //notify(NOTICE)<<"         assuming setTextureAttribute(unit=0,attr,value) instead."<<std::endl;
            //notify(NOTICE)<<"         please change calling code to use appropriate call."<<std::endl;

            setTextureAttribute(0,attribute,value);
        }

		m_needCompile = true;
    }        
}

void crStateSet::setAttributeAndModes(crStateAttribute *attribute, crStateAttribute::VRModeValue value)
{
    if (attribute)
    {
        if (!attribute->isTextureAttribute())
        {
			if (value&crStateAttribute::INHERIT)
			{
				removeAttribute(attribute->getType());
			}    
			else
			{
				setAttribute(m_attributeList,attribute,value);
				setAssociatedModes(attribute,value);
			}
        }
        else
        {
            //notify(NOTICE)<<"Warning: texture attribute '"<<attribute->className()<<"' passed to setAttributeAndModes(attr,value), "<<std::endl;
            //notify(NOTICE)<<"         assuming setTextureAttributeAndModes(unit=0,attr,value) instead."<<std::endl;
            //notify(NOTICE)<<"         please change calling code to use appropriate call."<<std::endl;

            setTextureAttributeAndModes(0,attribute,value);
        }

		m_needCompile = true;
    }
}

void crStateSet::removeAttribute(crStateAttribute::Type type)
{
	AttributeList::iterator itr = m_attributeList.find(type);
	if (itr!=m_attributeList.end())
	{
		if (itr->second.first->getUpdateCallback()) 
		{
			setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()-1);
		}

		//if (itr->second.first->getEventCallback()) 
		//{
		//	setNumChildrenRequiringEventTraversal(getNumChildrenRequiringEventTraversal()-1);
		//}

		itr->second.first->removeParent(this);
		setAssociatedModes(m_modeList,itr->second.first.get(),crStateAttribute::INHERIT);
		m_attributeList.erase(itr);
	}
}

void crStateSet::removeAttribute(crStateAttribute* attribute)
{
	if (!attribute) return;

	AttributeList::iterator itr = m_attributeList.find(attribute->getType());
	if (itr!=m_attributeList.end())
	{
		if (itr->second.first != attribute) return;

		if (itr->second.first->getUpdateCallback()) 
		{
			setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()-1);
		}

		//if (itr->second.first->getEventCallback()) 
		//{
		//	setNumChildrenRequiringEventTraversal(getNumChildrenRequiringEventTraversal()-1);
		//}

		itr->second.first->removeParent(this);

		setAssociatedModes(m_modeList,itr->second.first.get(),crStateAttribute::INHERIT);
		m_attributeList.erase(itr);
	}
}
//
//void crStateSet::setAttributeToInherit(crStateAttribute::Type type)
//{
//    AttributeList::iterator itr = m_attributeList.find(type);
//    if (itr!=m_attributeList.end())
//    {
//        setAssociatedModes(m_modeList,itr->second.first.get(),crStateAttribute::INHERIT);
//        m_attributeList.erase(itr);
//    }
//}

crStateAttribute* crStateSet::getAttribute(crStateAttribute::Type type)
{
    return getAttribute(m_attributeList,type);
}

const crStateAttribute* crStateSet::getAttribute(crStateAttribute::Type type) const
{
    return getAttribute(m_attributeList,type);
}

const crStateSet::RefAttributePair* crStateSet::getAttributePair(crStateAttribute::Type type) const
{
    return getAttributePair(m_attributeList,type);
}

void crStateSet::setAssociatedModes(const crStateAttribute* attribute, crStateAttribute::VRModeValue value)
{
    setAssociatedModes(m_modeList,attribute,value);
}

void crStateSet::addUniform(crUniform* uniform, crStateAttribute::OverrideValue value)
{
	if (uniform)
	{
		int delta_update = 0;
		//int delta_event = 0;

		UniformList::iterator itr=m_uniformList.find(uniform->getName());
		if (itr==m_uniformList.end())
		{
			// new entry.
			RefUniformPair& up = m_uniformList[uniform->getName()];
			up.first = uniform;
			up.second = value&(crStateAttribute::OVERRIDE|crStateAttribute::PROTECTED);

			uniform->addParent(this);

			if (uniform->getUpdateCallback()) 
			{
				delta_update = 1;
			}

			//if (uniform->getEventCallback()) 
			//{
			//	delta_event = 1;
			//}
		}
		else
		{
			if (itr->second.first==uniform)
			{
				// chaning just override
				itr->second.second = value&(crStateAttribute::OVERRIDE|crStateAttribute::PROTECTED);
			}
			else
			{
				itr->second.first->removeParent(this);
				if (itr->second.first->getUpdateCallback()) --delta_update;
				//if (itr->second.first->getEventCallback()) --delta_event;

				uniform->addParent(this);
				itr->second.first = uniform;
				if (itr->second.first->getUpdateCallback()) ++delta_update;
				//if (itr->second.first->getEventCallback()) ++delta_event;

				itr->second.second = value&(crStateAttribute::OVERRIDE|crStateAttribute::PROTECTED);
			}
		}

		if (delta_update!=0) 
		{
			setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+delta_update);
		}

		//if (delta_event!=0) 
		//{
		//	setNumChildrenRequiringEventTraversal(getNumChildrenRequiringEventTraversal()+delta_event);
		//}

	}
}

void crStateSet::removeUniform(const std::string& name)
{
	UniformList::iterator itr = m_uniformList.find(name);
	if (itr!=m_uniformList.end())
	{
		if (itr->second.first->getUpdateCallback()) 
		{
			setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()-1);
		}

		//if (itr->second.first->getEventCallback()) 
		//{
		//	setNumChildrenRequiringEventTraversal(getNumChildrenRequiringEventTraversal()-1);
		//}

		itr->second.first->removeParent(this);

		m_uniformList.erase(itr);
	}
}

void crStateSet::removeUniform(crUniform* uniform)
{
	if (!uniform) return;

	UniformList::iterator itr = m_uniformList.find(uniform->getName());
	if (itr!=m_uniformList.end())
	{
		if (itr->second.first != uniform) return;

		if (itr->second.first->getUpdateCallback()) 
		{
			setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()-1);
		}

		//if (itr->second.first->getEventCallback()) 
		//{
		//	setNumChildrenRequiringEventTraversal(getNumChildrenRequiringEventTraversal()-1);
		//}

		itr->second.first->removeParent(this);
		m_uniformList.erase(itr);
	}
}

crUniform* crStateSet::getUniform(const std::string& name)
{
	UniformList::iterator itr = m_uniformList.find(name);
	if (itr!=m_uniformList.end()) return itr->second.first.get();
	else return 0;
}

crUniform* crStateSet::getOrCreateUniform(const std::string& name, crUniform::Type type)
{
	// for look for an appropriate uniform.
	UniformList::iterator itr = m_uniformList.find(name);
	if (itr!=m_uniformList.end() && 
		itr->second.first->getType()==type)
	{
		return itr->second.first.get();
	}

	//crUniform* uniform;
	//itr = m_dirtyUniformList.find(name);
 //   if ( itr!=m_dirtyUniformList.end() && 
	// 	 itr->second.first->getType()==type)
	//{
	//	uniform = itr->second.first.get();
	//	//m_dirtyUniformList.erase(ditr);
	//}
	//else
	ref_ptr<crUniform>uniform = new crUniform(type,name);

	addUniform(uniform.get());

	return uniform.get();
}


const crUniform* crStateSet::getUniform(const std::string& name) const
{
	UniformList::const_iterator itr = m_uniformList.find(name);
	if (itr!=m_uniformList.end()) return itr->second.first.get();
	else return 0;
}

const crStateSet::RefUniformPair* crStateSet::getUniformPair(const std::string& name) const
{
	UniformList::const_iterator itr = m_uniformList.find(name);
	if (itr!=m_uniformList.end()) return &(itr->second);
	else return 0;
}

void crStateSet::setTextureMode(unsigned int unit,crStateAttribute::VRMode mode, crStateAttribute::VRModeValue value)
{
    if (s_textureModeSet.isTextureMode(mode))
    {
        setMode(getOrCreateTextureModeList(unit),mode,value);
    }
    else
    {
        //notify(NOTICE)<<"Warning: non-texture mode '"<<mode<<"'passed to setTextureMode(unit,mode,value), "<<std::endl;
        //notify(NOTICE)<<"         assuming setMode(mode,value) instead."<<std::endl;
        //notify(NOTICE)<<"         please change calling code to use appropriate call."<<std::endl;

        setMode(mode,value);
    }
}

void crStateSet::removeTextureMode(unsigned int unit,crStateAttribute::VRMode mode)
{
	if (s_textureModeSet.isTextureMode(mode))
	{
		if (unit>=m_textureModeList.size()) return;
		setModeToInherit(m_textureModeList[unit],mode);
	}
	else
	{
		//notify(NOTICE)<<"Warning: non-texture mode '"<<mode<<"'passed to setTextureModeToInherit(unit,mode), "<<std::endl;
		//notify(NOTICE)<<"         assuming setModeToInherit(unit=0,mode) instead."<<std::endl;
		//notify(NOTICE)<<"         please change calling code to use appropriate call."<<std::endl;

		removeMode(mode);
	}
}

crStateAttribute::VRModeValue crStateSet::getTextureMode(unsigned int unit,crStateAttribute::VRMode mode) const
{
    if (s_textureModeSet.isTextureMode(mode))
    {
        if (unit>=m_textureModeList.size()) return crStateAttribute::INHERIT;
        return getMode(m_textureModeList[unit],mode);
    }
    else
    {
 /*       notify(NOTICE)<<"Warning: non-texture mode '"<<mode<<"'passed to geTexturetMode(unit,mode), "<<std::endl;
        notify(NOTICE)<<"         assuming getMode(mode) instead."<<std::endl;
        notify(NOTICE)<<"         please change calling code to use appropriate call."<<std::endl;*/

        return getMode(mode);
    }
}

void crStateSet::setTextureAttribute(unsigned int unit,crStateAttribute *attribute, const crStateAttribute::OverrideValue value)
{
    if (attribute)
    {
        if (attribute->isTextureAttribute())
        {
            setAttribute(getOrCreateTextureAttributeList(unit),attribute,value);
        }
        else
        {
            //notify(NOTICE)<<"Warning: texture attribute '"<<attribute->className()<<"' passed to setTextureAttribute(unit,attr,value), "<<std::endl;
            //notify(NOTICE)<<"         assuming setAttribute(attr,value) instead."<<std::endl;
            //notify(NOTICE)<<"         please change calling code to use appropriate call."<<std::endl;
            setAttribute(attribute,value);
        }
		m_needCompile = true;
    }
}

void crStateSet::setTextureAttributeAndModes(unsigned int unit,crStateAttribute *attribute, crStateAttribute::VRModeValue value)
{
    if (attribute)
    {
    
        if (attribute->isTextureAttribute())
        {
            if (value&crStateAttribute::INHERIT)
            {
                removeTextureAttribute(unit,attribute->getType());
            }
            else
            {
                setAttribute(getOrCreateTextureAttributeList(unit),attribute,value);
                setAssociatedModes(getOrCreateTextureModeList(unit),attribute,value);
            }
        }
        else
        {
            //notify(NOTICE)<<"Warning: non texture attribute '"<<attribute->className()<<"' passed to setTextureAttributeAndModes(unit,attr,value), "<<std::endl;
            //notify(NOTICE)<<"         assuming setAttributeAndModes(attr,value) instead."<<std::endl;
            //notify(NOTICE)<<"         please change calling code to use appropriate call."<<std::endl;
            setAttribute(attribute,value);
        }
		m_needCompile = true;
    }
}

void crStateSet::removeTextureAttribute(unsigned int unit,crStateAttribute::Type type)
{
	if (unit>=m_textureAttributeList.size()) return;
	AttributeList& attributeList = m_textureAttributeList[unit];
	AttributeList::iterator itr = attributeList.find(type);
	if (itr!=attributeList.end())
	{
		if (unit<m_textureModeList.size())
		{
			setAssociatedTextureModes(unit,itr->second.first.get(),crStateAttribute::INHERIT);
		}

		if (itr->second.first->getUpdateCallback()) 
		{
			setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()-1);
		}

		//if (itr->second.first->getEventCallback()) 
		//{
		//	setNumChildrenRequiringEventTraversal(getNumChildrenRequiringEventTraversal()-1);
		//}

		itr->second.first->removeParent(this);
		attributeList.erase(itr);
	}
}

void crStateSet::removeTextureAttribute(unsigned int unit, crStateAttribute* attribute)
{
	if (!attribute) return;
	if (unit>=m_textureAttributeList.size()) return;

	AttributeList& attributeList = m_textureAttributeList[unit];
	AttributeList::iterator itr = attributeList.find(attribute->getType());
	if (itr!=attributeList.end())
	{
		if (itr->second.first != attribute) return;

		setAssociatedTextureModes(unit,itr->second.first.get(),crStateAttribute::INHERIT);

		if (itr->second.first->getUpdateCallback()) 
		{
			setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()-1);
		}

		//if (itr->second.first->getEventCallback()) 
		//{
		//	setNumChildrenRequiringEventTraversal(getNumChildrenRequiringEventTraversal()-1);
		//}

		itr->second.first->removeParent(this);
		attributeList.erase(itr);
	}
}

//void crStateSet::setTextureAttributeToInherit(unsigned int unit,crStateAttribute::Type type)
//{
//    if (unit>=m_textureAttributeList.size()) return;
//    AttributeList& attributeList = m_textureAttributeList[unit];
//    AttributeList::iterator itr = attributeList.find(type);
//    if (itr!=attributeList.end())
//    {
//        if (unit<m_textureModeList.size())
//        {
//            setAssociatedModes(m_textureModeList[unit],itr->second.first.get(),crStateAttribute::INHERIT);
//        }
//        attributeList.erase(itr);
//    }
//}
//
//void crStateSet::setTextureAttributeToInherit(unsigned int unit,crStateAttribute::Type type) const
//{
//	if (unit>=m_textureAttributeList.size()) return;
//	AttributeList& attributeList = m_textureAttributeList[unit];
//	AttributeList::iterator itr = attributeList.find(type);
//	if (itr!=attributeList.end())
//	{
//		if (unit<m_textureModeList.size())
//		{
//			setAssociatedModes(m_textureModeList[unit],itr->second.first.get(),crStateAttribute::INHERIT);
//		}
//		attributeList.erase(itr);
//	}
//}

crStateAttribute* crStateSet::getTextureAttribute(unsigned int unit,crStateAttribute::Type type)
{
    if (unit>=m_textureAttributeList.size()) return 0;
    return getAttribute(m_textureAttributeList[unit],type);
}


const crStateAttribute* crStateSet::getTextureAttribute(unsigned int unit,crStateAttribute::Type type) const
{
    if (unit>=m_textureAttributeList.size()) return 0;
    return getAttribute(m_textureAttributeList[unit],type);
}


const crStateSet::RefAttributePair* crStateSet::getTextureAttributePair(unsigned int unit,crStateAttribute::Type type) const
{
    if (unit>=m_textureAttributeList.size()) return 0;
    return getAttributePair(m_textureAttributeList[unit],type);
}

void crStateSet::setAssociatedTextureModes(unsigned int unit,const crStateAttribute* attribute, crStateAttribute::VRModeValue value)
{
    setAssociatedModes(getOrCreateTextureModeList(unit),attribute,value);
}

void crStateSet::compile(crState& state) const
{
	if(m_needCompile)
	{
		const crProgram *program;
		unsigned int contexid = state.getContextID();
		for(AttributeList::const_iterator itr = m_attributeList.begin();
			itr!=m_attributeList.end();
			++itr)
		{
			program = dynamic_cast<const crProgram *>(itr->second.first.get());
			if(program && program->getPCP(contexid)->needsLink())
				program->compile(state);
		}

		const crTexture *tex;
		for(TextureAttributeList::const_iterator taitr=m_textureAttributeList.begin();
			taitr!=m_textureAttributeList.end();
			++taitr)
		{
			for(AttributeList::const_iterator itr = taitr->begin();
				itr!=taitr->end();
				++itr)
			{
				tex = dynamic_cast<const crTexture *>(itr->second.first.get());
				if(tex && tex->getNeedCompile())
					tex->compile(state);
			}
		}

		m_needCompile = false;
	}
}

void crStateSet::releaseObjects(crState* state)
{
	for(AttributeList::iterator itr = m_attributeList.begin();
		itr!=m_attributeList.end();
		++itr)
	{
		itr->second.first->releaseObjects(state);
	}
	//m_attributeList.clear();
	crTexture *tex;
	for(TextureAttributeList::iterator taitr=m_textureAttributeList.begin();
		taitr!=m_textureAttributeList.end();
		++taitr)
	{
		for(AttributeList::iterator itr = taitr->begin();
			itr!=taitr->end();
			++itr)
		{
			if(referenceCount()<=1)
			{
                itr->second.first->releaseObjects(state);
			}
			else
			{
				tex = dynamic_cast<crTexture *>(itr->second.first.get());
				if(!tex || tex->getImageNameID().empty())
				{
					itr->second.first->releaseObjects(state);
				}
			}
		}
	}
	//m_textureAttributeList.clear();
}

void crStateSet::setRenderingHint(RenderingHint hint)
{
    m_renderingHint = hint;
    // temporary hack to get new render bins working.
    switch(m_renderingHint)
    {
        case(TRANSPARENT_BIN):
        {
            m_binMode = USE_RENDERBIN_DETAILS;
            m_binNum = Trans;
            m_binName = "DepthSortedBin";
            break;
        }
        case(OPAQUE_BIN):
        {
            m_binMode = USE_RENDERBIN_DETAILS;
            m_binNum = Opaque;
            m_binName = "RenderBin";
            break;
        }
		case(ALPHATEST_BIN):
		{
			m_binMode = USE_RENDERBIN_DETAILS;
			m_binNum = AlphaTest;
			m_binName = "RenderBin";
			break;
		}
		case(CLEARNODE_BIN):
		{
			m_binMode = USE_RENDERBIN_DETAILS;
			m_binNum = ClearNode;
			m_binName = "StateSortedBin";
			break;
		}
		case(MIRRORFIRST_BIN):
		{
			m_binMode = USE_RENDERBIN_DETAILS;
			m_binNum = MirrorFirst;
			m_binName = "StateSortedBin";
			break;
		}
		case(MIRRORSECOND_BIN):
		{
			m_binMode = USE_RENDERBIN_DETAILS;
			m_binNum = MirrorSecond;
			m_binName = "StateSortedBin";
			break;
		}
		case(MIRRORREFLECT_BIN):
		{
			m_binMode = USE_RENDERBIN_DETAILS;
			m_binNum = MirrorReflect;
			m_binName = "RenderBin";
			break;
		}
		case(MIRROREND_BIN):
		{
			m_binMode = USE_RENDERBIN_DETAILS;
			m_binNum = MirrorEnd;
			m_binName = "StateSortedBin";
			break;
		}
		case(MULTIRENDER_BIN):
		{
			m_binMode = USE_RENDERBIN_DETAILS;
			m_binNum = ForMultiRender;
			m_binName = "StateSortedBin";
			break;
		}
		case(GRIDDRAW_BIN):
		{
			m_binMode = USE_RENDERBIN_DETAILS;
			m_binNum = GridDraw;
			m_binName = "StateSortedBin";
			break;
		}
		case(SKY_BIN):
		{
			m_binMode = USE_RENDERBIN_DETAILS;
			m_binNum = Sky;
			m_binName = "StateSortedBin";
			break;
		}
		case(UI_BIN):
		{
			m_binMode = USE_RENDERBIN_DETAILS;
			m_binNum = UI;
			m_binName = "UIBin";
			break;
		}
		case(PRETRANS_BIN):
		{
			m_binMode = USE_RENDERBIN_DETAILS;
			m_binNum = PreTrans;
			m_binName = "DepthSortedBin";
			break;
		}
		case(POSTTRANS_BIN):
		{
			m_binMode = USE_RENDERBIN_DETAILS;
			m_binNum = PostTrans;
			m_binName = "DepthSortedBin";
			break;
		}
		case(OUTLINE_BIN):
		{
			m_binMode = USE_RENDERBIN_DETAILS;
			m_binNum = OutLine;//6;
			m_binName = "RenderBin";
			break;
		}
		case(FX_BIN):
		{
			m_binMode = USE_RENDERBIN_DETAILS;
			m_binNum = FX;
			m_binName = "DepthSortedBin";
			break;
		}
        default: // DEFAULT_BIN
        {
			setRenderBinToInherit();
            break;
        }
    }
}

void crStateSet::setRenderBinDetails(int binNum,const std::string& binName,RenderBinMode mode)
{
    m_binMode = mode;
    m_binNum = binNum;
    m_binName = binName;
}

void crStateSet::setRenderBinToInherit()
{
    m_binMode = INHERIT_RENDERBIN_DETAILS;
    m_binNum = 0;
    //m_binName = "";
	if(!m_binName.empty()) m_binName.clear();
}

void crStateSet::setMode(ModeList& modeList,crStateAttribute::VRMode mode, crStateAttribute::VRModeValue value)
{		
    if ((value&crStateAttribute::INHERIT)) setModeToInherit(modeList,mode);
    else modeList[mode] = value;
}

void crStateSet::setMode(ModeList& modeList,crStateAttribute::VRMode mode, crStateAttribute::VRModeValue value) const
{
	if ((value&crStateAttribute::INHERIT)) setModeToInherit(modeList,mode);
	else modeList[mode] = value;
}

void crStateSet::setModeToInherit(ModeList& modeList,crStateAttribute::VRMode mode)
{
    ModeList::iterator itr = modeList.find(mode);
    if (itr!=modeList.end())
    {
        modeList.erase(itr);
    }
}

void crStateSet::setModeToInherit(ModeList& modeList,crStateAttribute::VRMode mode) const
{
	ModeList::iterator itr = modeList.find(mode);
	if (itr!=modeList.end())
	{
		modeList.erase(itr);
	}
}

crStateAttribute::VRModeValue crStateSet::getMode(const ModeList& modeList,crStateAttribute::VRMode mode) const
{
    ModeList::const_iterator itr = modeList.find(mode);
    if (itr!=modeList.end())
    {
        return itr->second;
    }
    else
        return crStateAttribute::INHERIT;
}

void crStateSet::setAssociatedModes(ModeList& modeList,const crStateAttribute* attribute, crStateAttribute::VRModeValue value)
{
    // get the associated modes.
    std::vector<crStateAttribute::VRMode> modes;
    attribute->getAssociatedModes(modes);

    // set the modes on the crStateSet.
    for(std::vector<crStateAttribute::VRMode>::iterator itr=modes.begin();
        itr!=modes.end();
        ++itr)
    {
        setMode(modeList,*itr,value);
    }
}

//void crStateSet::setAssociatedModes(ModeList& modeList,const crStateAttribute* attribute, crStateAttribute::VRModeValue value) const
//{
//	// get the associated modes.
//	std::vector<crStateAttribute::VRMode> modes;
//	attribute->getAssociatedModes(modes);
//
//	// set the modes on the crStateSet.
//	for(std::vector<crStateAttribute::VRMode>::iterator itr=modes.begin();
//		itr!=modes.end();
//		++itr)
//	{
//		setMode(modeList,*itr,value);
//	}
//}

//void crStateSet::setAttribute(AttributeList& attributeList,crStateAttribute *attribute, const crStateAttribute::OverrideValue value)
//{
//    if (attribute)
//    {
//        attributeList[attribute->getType()] = RefAttributePair(attribute,value&crStateAttribute::OVERRIDE);
//    }
//}
//
//void crStateSet::setAttribute(AttributeList& attributeList,crStateAttribute *attribute, const crStateAttribute::OverrideValue value) const
//{
//	if (attribute)
//	{
//		attributeList[attribute->getType()] = RefAttributePair(attribute,value&crStateAttribute::OVERRIDE);
//	}
//}

void crStateSet::setAttribute(AttributeList& attributeList,crStateAttribute *attribute, const crStateAttribute::OverrideValue value)
{
	if (attribute)
	{
		int delta_update = 0;
		//int delta_event = 0;

		AttributeList::iterator itr=attributeList.find(attribute->getType());
		if (itr==attributeList.end())
		{
			// new entry.
			attributeList[attribute->getType()] = RefAttributePair(attribute,value&(crStateAttribute::OVERRIDE|crStateAttribute::PROTECTED));
			attribute->addParent(this);

			if (attribute->getUpdateCallback()) 
			{
				delta_update = 1;
			}

			//if (attribute->getEventCallback()) 
			//{
			//	delta_event = 1;
			//}
		}
		else
		{
			if (itr->second.first==attribute)
			{
				// changing just override
				itr->second.second = value&(crStateAttribute::OVERRIDE|crStateAttribute::PROTECTED);
			}
			else
			{
				itr->second.first->removeParent(this);
				if (itr->second.first->getUpdateCallback()) --delta_update;
				//if (itr->second.first->getEventCallback()) --delta_event;

				attribute->addParent(this);
				itr->second.first = attribute;
				if (itr->second.first->getUpdateCallback()) ++delta_update;
				//if (itr->second.first->getEventCallback()) ++delta_event;

				itr->second.second = value&(crStateAttribute::OVERRIDE|crStateAttribute::PROTECTED);
			}
		}

		if (delta_update!=0) 
		{
			setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+delta_update);
		}

		//if (delta_event!=0) 
		//{
		//	setNumChildrenRequiringEventTraversal(getNumChildrenRequiringEventTraversal()+delta_event);
		//}
	}
}

crStateAttribute* crStateSet::getAttribute(AttributeList& attributeList,crStateAttribute::Type type)
{
    AttributeList::iterator itr = attributeList.find(type);
    if (itr!=attributeList.end())
    {
        return itr->second.first.get();
    }
    else
        return NULL;
}

const crStateAttribute* crStateSet::getAttribute(const AttributeList& attributeList,crStateAttribute::Type type) const
{
    AttributeList::const_iterator itr = attributeList.find(type);
    if (itr!=attributeList.end())
    {
        return itr->second.first.get();
    }
    else
        return NULL;
}

const crStateSet::RefAttributePair* crStateSet::getAttributePair(const AttributeList& attributeList,crStateAttribute::Type type) const
{
    AttributeList::const_iterator itr = attributeList.find(type);
    if (itr!=attributeList.end())
    {
        return &(itr->second);
    }
    else
        return NULL;
}

//void crStateSet::setLight( int lightid )
//{ 
//	crLight *plight = crBrain::getInstance()->getResource()->findOrLoadLight(lightid);
//    this->setAttributeAndModes(plight);
//}  
//
//int crStateSet::getLightID()const
//{ 
//	const crLight *plight = dynamic_cast<const crLight *>(getAttribute(crStateAttribute::LIGHT));
//	if(plight) return crBrain::getInstance()->getResource()->getLightID(plight); 
//	return -1;
//}
//
//void crStateSet::setMaterial( int matid )
//{
//	crMaterial *pmat = crBrain::getInstance()->getResource()->findOrLoadMaterial(matid);
//	this->setAttributeAndModes(pmat);
//}
//
//int crStateSet::getMaterialID()const
//{
//	const crMaterial *pmat = dynamic_cast<const crMaterial *>(getAttribute(crStateAttribute::MATERIAL));
//	if(pmat) return crBrain::getInstance()->getResource()->getMaterialID(pmat); 
//	return -1;
//}
//
//void crStateSet::setTexture( unsigned int unit, int texid )
//{
//	crTexture *ptex = crBrain::getInstance()->getResource()->findOrLoadTexture(texid);
//	this->setTextureAttributeAndModes(unit,ptex);
//}
//
//int crStateSet::getTextureID( unsigned int unit )
//{
//	const crTexture *ptex = dynamic_cast<const crTexture *>(getTextureAttribute(unit,crStateAttribute::TEXTURE));
//	if(ptex) return crBrain::getInstance()->getResource()->getTextureID(ptex); 
//	return -1;
//}

void crStateSet::setUpdateCallback(Callback* ac)
{
	if (m_updateCallback==ac) return;

	int delta = 0;
	if (m_updateCallback.valid()) --delta;
	if (ac) ++delta;

	m_updateCallback = ac;

	if (delta!=0 && m_numChildrenRequiringUpdateTraversal==0)
	{
		for(ParentList::iterator itr=m_parents.begin();
			itr!=m_parents.end();
			++itr)
		{
			crDrawable* drawable = dynamic_cast<crDrawable*>(*itr);
			if (drawable) 
			{
				//drawable->setNumChildrenRequiringUpdateTraversal(drawable->getNumChildrenRequiringUpdateTraversal()+delta);
			}
			else 
			{
				crNode* node = dynamic_cast<crNode*>(*itr);
				if (node) 
				{
					node->setNumChildrenRequiringUpdateTraversal(node->getNumChildrenRequiringUpdateTraversal()+delta);
				}
			}
		}
	}
}

void crStateSet::runUpdateCallbacks(crNodeVisitor* nv)
{
	if (m_updateCallback.valid()) (*m_updateCallback)(this,nv);

	if (m_numChildrenRequiringUpdateTraversal!=0)
	{
		// run attribute callbacks
		for(AttributeList::iterator itr=m_attributeList.begin();
			itr!=m_attributeList.end();
			++itr)
		{
			crStateAttribute::Callback* callback = itr->second.first->getUpdateCallback();
			if (callback) (*callback)(itr->second.first.get(),nv);
		}


		// run texture attribute callbacks.
		for(unsigned int i=0;i<m_textureAttributeList.size();++i)
		{
			AttributeList& attributeList = m_textureAttributeList[i];
			for(AttributeList::iterator itr=attributeList.begin();
				itr!=attributeList.end();
				++itr)
			{
				crStateAttribute::Callback* callback = itr->second.first->getUpdateCallback();
				if (callback) (*callback)(itr->second.first.get(),nv);
			}
		}


		// run uniform callbacks.
		for(UniformList::iterator uitr = m_uniformList.begin();
			uitr != m_uniformList.end();
			++uitr)
		{
			crUniform::Callback* callback = uitr->second.first->getUpdateCallback();
			if (callback) (*callback)(uitr->second.first.get(),nv);
		}
	}
}
//void crStateSet::setEventCallback(Callback* ac)
//{
//	if (m_eventCallback==ac) return;
//
//	int delta = 0;
//	if (m_eventCallback.valid()) --delta;
//	if (ac) ++delta;
//
//	m_eventCallback = ac;
//
//	if (delta!=0 && m_numChildrenRequiringEventTraversal==0)
//	{
//		for(ParentList::iterator itr=m_parents.begin();
//			itr!=m_parents.end();
//			++itr)
//		{
//			CRCore::crDrawable* drawable = dynamic_cast<CRCore::crDrawable*>(*itr);
//			if (drawable) 
//			{
//				//drawable->setNumChildrenRequiringUpdateTraversal(drawable->getNumChildrenRequiringUpdateTraversal()+delta);
//			}
//			else 
//			{
//				CRCore::crNode* node = dynamic_cast<CRCore::crNode*>(*itr);
//				if (node) 
//				{
//					node->setNumChildrenRequiringEventTraversal(node->getNumChildrenRequiringEventTraversal()+delta);
//				}
//			}
//		}
//	}
//}
//
//void crStateSet::runEventCallbacks(CRCore::crNodeVisitor* nv)
//{
//	if (m_eventCallback.valid()) (*m_eventCallback)(this,nv);
//
//	if (m_numChildrenRequiringEventTraversal!=0)
//	{
//		// run attribute callbacks
//		for(AttributeList::iterator itr=m_attributeList.begin();
//			itr!=m_attributeList.end();
//			++itr)
//		{
//			crStateAttribute::Callback* callback = itr->second.first->getEventCallback();
//			if (callback) (*callback)(itr->second.first.get(),nv);
//		}
//
//
//		// run texture attribute callbacks.
//		for(unsigned int i=0;i<m_textureAttributeList.size();++i)
//		{
//			AttributeList& attributeList = m_textureAttributeList[i];
//			for(AttributeList::iterator itr=attributeList.begin();
//				itr!=attributeList.end();
//				++itr)
//			{
//				crStateAttribute::Callback* callback = itr->second.first->getEventCallback();
//				if (callback) (*callback)(itr->second.first.get(),nv);
//			}
//		}
//
//
//		// run uniform callbacks.
//		for(UniformList::iterator uitr = m_uniformList.begin();
//			uitr != m_uniformList.end();
//			++uitr)
//		{
//			crUniform::Callback* callback = uitr->second.first->getEventCallback();
//			if (callback) (*callback)(uitr->second.first.get(),nv);
//		}
//	}
//}
//
void crStateSet::setNumChildrenRequiringUpdateTraversal(unsigned int num)
{
	// if no changes just return.
	if (m_numChildrenRequiringUpdateTraversal==num) return;

	// note, if _updateCallback is set then the
	// parents won't be affected by any changes to
	// m_numChildrenRequiringUpdateTraversal so no need to inform them.
	if (!m_updateCallback && !m_parents.empty())
	{

		// need to pass on changes to parents.        
		int delta = 0;
		if (m_numChildrenRequiringUpdateTraversal>0) --delta;
		if (num>0) ++delta;
		if (delta!=0)
		{
			// the number of callbacks has changed, need to pass this
			// on to parents so they know whether app traversal is
			// reqired on this subgraph.
			for(ParentList::iterator itr = m_parents.begin();
				itr != m_parents.end();
				++itr)
			{    
				crDrawable* drawable = dynamic_cast<crDrawable*>(*itr);
				if (drawable) 
				{
					drawable->setNumChildrenRequiringUpdateTraversal(drawable->getNumChildrenRequiringUpdateTraversal()+delta);
				}
				else 
				{
					crNode* node = dynamic_cast<crNode*>(*itr);
					if (node) 
					{
						node->setNumChildrenRequiringUpdateTraversal(node->getNumChildrenRequiringUpdateTraversal()+delta);
					}
				}
			}
		}
	}

	// finally update this objects value.
	m_numChildrenRequiringUpdateTraversal=num;
}

//void crStateSet::setNumChildrenRequiringEventTraversal(unsigned int num)
//{
//	// if no changes just return.
//	if (m_numChildrenRequiringEventTraversal==num) return;
//
//	// note, if _EventCallback is set then the
//	// parents won't be affected by any changes to
//	// _numChildrenRequiringEventTraversal so no need to inform them.
//	if (!m_eventCallback && !m_parents.empty())
//	{
//
//		// need to pass on changes to parents.        
//		int delta = 0;
//		if (m_numChildrenRequiringEventTraversal>0) --delta;
//		if (num>0) ++delta;
//		if (delta!=0)
//		{
//			// the number of callbacks has changed, need to pass this
//			// on to parents so they know whether app traversal is
//			// reqired on this subgraph.
//			for(ParentList::iterator itr =m_parents.begin();
//				itr != m_parents.end();
//				++itr)
//			{    
//				CRCore::crDrawable* drawable = dynamic_cast<CRCore::crDrawable*>(*itr);
//				if (drawable) 
//				{
//					drawable->setNumChildrenRequiringEventTraversal(drawable->getNumChildrenRequiringEventTraversal()+delta);
//				}
//				else 
//				{
//					CRCore::crNode* node = dynamic_cast<CRCore::crNode*>(*itr);
//					if (node) 
//					{
//						node->setNumChildrenRequiringEventTraversal(node->getNumChildrenRequiringEventTraversal()+delta);
//					}
//				}
//			}
//		}
//	}
//
//	// finally Event this objects value.
//	m_numChildrenRequiringEventTraversal=num;
//}
//
bool crStateSet::checkValidityOfAssociatedModes(crState& state) const
{
	bool modesValid = true;
	for(AttributeList::const_iterator itr = m_attributeList.begin();
		itr!=m_attributeList.end();
		++itr)
	{
		if (!itr->second.first->checkValidityOfAssociatedModes(state)) modesValid = false;
	}

	for(TextureAttributeList::const_iterator taitr=m_textureAttributeList.begin();
		taitr!=m_textureAttributeList.end();
		++taitr)
	{
		for(AttributeList::const_iterator itr = taitr->begin();
			itr!=taitr->end();
			++itr)
		{
			if (!itr->second.first->checkValidityOfAssociatedModes(state)) modesValid = false;
		}
	}

	return modesValid;
}
void crStateSet::dirtyShaderStr() 
{ 
	//m_getRtglmShaderStr.clear();
	m_shaderStrMap.clear();
}
//void crStateSet::setRenderInited(bool bln)
//{
//	m_renderInited = bln;
//}
//bool crStateSet::getRenderInited()
//{
//	return m_renderInited;
//}
const std::string &crStateSet::getRtglmShaderStr(const crStateSet *obj_ss/*drawable_ss*/) const
{
	ShaderStrMap::iterator itr = m_shaderStrMap.find(obj_ss);
	if(itr != m_shaderStrMap.end())
		return itr->second;
	else
	{
		std::string RtglmShaderStr;
		//const crStateSet *this;
		//if(obj_ss)
		//{
		//	this = drawable_ss;
		//	obj_ss = this;
		//}
		//else
		//{
		//	this = this;
		//	obj_ss = NULL;
		//}
        ///////Others
		bool hasLightmap = dynamic_cast<const crTexture2D *>(this->getTextureAttribute(TEXTURE_LIGHTMAP,crStateAttribute::TEXTURE))!=0;
		bool lightMat = false;
		bool hasEnvmap = dynamic_cast<const crTextureCubeMap *>(this->getTextureAttribute(TEXTURE_ENVMAP,crStateAttribute::TEXTURE))!=0;
		if(!hasEnvmap && obj_ss)
		{
			hasEnvmap = dynamic_cast<const crTextureCubeMap *>(obj_ss->getTextureAttribute(TEXTURE_ENVMAP,crStateAttribute::TEXTURE))!=0;
		}
		///////////normal
		const crTexture *texture = dynamic_cast<const crTexture *>(this->getTextureAttribute(TEXTURE_BUMPMAP,crStateAttribute::TEXTURE));
		int bump = 0;
		if(texture)
		{
			bump = texture->getUseParallax()?2:1;
		}
		bool normalMat = false;
		bool NT2 = this->getTextureAttribute(TEXTURE_NORMALMAP2,crStateAttribute::TEXTURE)!=0;
		bool normalMat2 = false;
		bool NT3 = this->getTextureAttribute(TEXTURE_NORMALMAP3,crStateAttribute::TEXTURE)!=0;
		bool normalMat3 = false;

		int OpNT = this->getTextureOperate(TEXTURE_BUMPMAP);
		int OpNT2 = this->getTextureOperate(TEXTURE_NORMALMAP2);
		int OpNT3 = this->getTextureOperate(TEXTURE_NORMALMAP3);

	    ////////////diffuse
		bool haseBase = true;
		bool baseMat = false;
		bool baseMatA = false;
		bool hasDiffuse = this->getTextureAttribute(TEXTURE_DIFFUSEMAP,crStateAttribute::TEXTURE)!=0;
		bool diffuseMat = false;
		bool diffuseMatA = false;
		bool DT2 = this->getTextureAttribute(TEXTURE_DIFFUSEMAP2,crStateAttribute::TEXTURE)!=0;
		bool diffuseMat2 = false;
		bool diffuseMat2A = false;

		int OpBT = this->getTextureOperate(TEXTURE_BASEMAP);
		int OpDT = this->getTextureOperate(TEXTURE_DIFFUSEMAP);
		int OpDT2 = this->getTextureOperate(TEXTURE_DIFFUSEMAP2);

		int OpBTA = this->getAlphaOperate(TEXTURE_BASEMAP);
		int OpDTA = this->getAlphaOperate(TEXTURE_DIFFUSEMAP);
		int OpDT2A = this->getAlphaOperate(TEXTURE_DIFFUSEMAP2);

		///////////specular
		bool hasSpecular = this->getTextureAttribute(TEXTURE_SPECULARMAP,crStateAttribute::TEXTURE)!=0;
		bool specularMat = false;
		bool specularMatA = false;
		bool ST2 = this->getTextureAttribute(TEXTURE_SPECULARMAP2,crStateAttribute::TEXTURE)!=0;
		bool specularMat2 = false;
		bool specularMat2A = false;
		bool ST3 = this->getTextureAttribute(TEXTURE_SPECULARMAP3,crStateAttribute::TEXTURE)!=0;
		bool specularMat3 = false;
		bool specularMat3A = false;

		int OpST = this->getTextureOperate(TEXTURE_SPECULARMAP);
		int OpST2 = this->getTextureOperate(TEXTURE_SPECULARMAP2);
		int OpST3 = this->getTextureOperate(TEXTURE_SPECULARMAP3);

		int OpSTA = this->getAlphaOperate(TEXTURE_SPECULARMAP);
		int OpST2A = this->getAlphaOperate(TEXTURE_SPECULARMAP2);
		int OpST3A = this->getAlphaOperate(TEXTURE_SPECULARMAP3);

		//////////emissive
		bool hasEmissive = this->getTextureAttribute(TEXTURE_EMISSIVEMAP,crStateAttribute::TEXTURE)!=0;
		bool emissiveMat = false;
		bool emissiveMatA = false;
		bool ET2 = this->getTextureAttribute(TEXTURE_EMISSIVEMAP2,crStateAttribute::TEXTURE)!=0;
		bool emissiveMat2 = false;
		bool emissiveMat2A = false;
		bool ET3 = this->getTextureAttribute(TEXTURE_EMISSIVEMAP3,crStateAttribute::TEXTURE)!=0;
		bool emissiveMat3 = false;
		bool emissiveMat3A = false;

		int OpET = this->getTextureOperate(TEXTURE_EMISSIVEMAP);
		int OpET2 = this->getTextureOperate(TEXTURE_EMISSIVEMAP2);
		int OpET3 = this->getTextureOperate(TEXTURE_EMISSIVEMAP3);

		int OpETA = this->getAlphaOperate(TEXTURE_EMISSIVEMAP);
		int OpET2A = this->getAlphaOperate(TEXTURE_EMISSIVEMAP2);
		int OpET3A = this->getAlphaOperate(TEXTURE_EMISSIVEMAP3);

		///////////////////envmap
		bool param1MultENV = false;
		bool param4MultENV = false;
		bool param4AddENV = false;
		if(hasEnvmap)
		{
			param1MultENV = this->getUniform("param1MultENV")!=0;
			param4MultENV = this->getUniform("param4MultENV")!=0;
			param4AddENV = this->getUniform("param4AddENV")!=0;
		}

		///////////////////normalmap
		bool param1MultNT = false;
		bool param3MultNT = false;
		bool param3AddNT = false;
		if(bump>0 && OpNT<3)
		{
			param1MultNT = this->getUniform("param1MultNT")!=0;
			param3MultNT = this->getUniform("param3MultNT")!=0;
			param3AddNT = this->getUniform("param3AddNT")!=0;
		}
		bool param1MultNT2 = false;
		bool param3MultNT2 = false;
		bool param3AddNT2 = false;
		if(NT2 && OpNT2<3)
		{
			param1MultNT2 = this->getUniform("param1MultNT2")!=0;
			param3MultNT2 = this->getUniform("param3MultNT2")!=0;
			param3AddNT2 = this->getUniform("param3AddNT2")!=0;
		}
		bool param1MultNT3 = false;
		bool param3MultNT3 = false;
		bool param3AddNT3 = false;
		if(NT3 && OpNT3<3)
		{
			param1MultNT3 = this->getUniform("param1MultNT3")!=0;
			param3MultNT3 = this->getUniform("param3MultNT3")!=0;
			param3AddNT3 = this->getUniform("param3AddNT3")!=0;
		}
        ////////diffusemap
		bool param1MultBT = false;
		bool param4MultBT = false;
		bool param4AddBT = false;
		bool param1MultBTa = false;
		bool param1AddBTa = false;
        if(haseBase)
		{
			if(OpBT<3)
			{
				param1MultBT = this->getUniform("param1MultBT")!=0;
				param4MultBT = this->getUniform("param4MultBT")!=0;
				param4AddBT = this->getUniform("param4AddBT")!=0;
			}
            if(OpBTA>0)
			{
				param1MultBTa = this->getUniform("param1MultBTa")!=0;
				param1AddBTa = this->getUniform("param1AddBTa")!=0;
			}
		}

        bool param1MultDT = false;
		bool param4MultDT = false;
		bool param4AddDT = false;
		bool param1MultDTa = false;
		bool param1AddDTa = false;
        if(hasDiffuse)
		{
			if(OpDT<3)
			{
				param1MultDT = this->getUniform("param1MultDT")!=0;
				param4MultDT = this->getUniform("param4MultDT")!=0;
				param4AddDT = this->getUniform("param4AddDT")!=0;
			}
            if(OpDTA>0)
			{
				param1MultDTa = this->getUniform("param1MultDTa")!=0;
				param1AddDTa = this->getUniform("param1AddDTa")!=0;
			}
		}
		bool param1MultDT2 = false;
		bool param4MultDT2 = false;
		bool param4AddDT2 = false;
		bool param1MultDT2a = false;
		bool param1AddDT2a = false;
		if(DT2)
		{
			if(OpDT2<3)
			{
				param1MultDT2 = this->getUniform("param1MultDT2")!=0;
				param4MultDT2 = this->getUniform("param4MultDT2")!=0;
				param4AddDT2 = this->getUniform("param4AddDT2")!=0;
			}
            if(OpDT2A>0)
			{
				param1MultDT2a = this->getUniform("param1MultDT2a")!=0;
				param1AddDT2a = this->getUniform("param1AddDT2a")!=0;
			}
		}
		///////////////specular
		bool param1MultST = false;
		bool param4MultST = false;
		bool param4AddST = false;
		bool param1MultSTa = false;
		bool param1AddSTa = false;
		if(hasSpecular)
		{
			if(OpST<3)
			{
				param1MultST = this->getUniform("param1MultST")!=0;
				param4MultST = this->getUniform("param4MultST")!=0;
				param4AddST = this->getUniform("param4AddST")!=0;
			}
			if(OpSTA>0)
			{
				param1MultSTa = this->getUniform("param1MultSTa")!=0;
				param1AddSTa = this->getUniform("param1AddSTa")!=0;
			}
		}
		bool param1MultST2 = false;
		bool param4MultST2 = false;
		bool param4AddST2 = false;
		bool param1MultST2a = false;
		bool param1AddST2a = false;
		if(ST2)
		{
			if(OpST2<3)
			{
				param1MultST2 = this->getUniform("param1MultST2")!=0;
				param4MultST2 = this->getUniform("param4MultST2")!=0;
				param4AddST2 = this->getUniform("param4AddST2")!=0;
			}
			if(OpST3A>0)
			{
				param1MultST2a = this->getUniform("param1MultST2a")!=0;
				param1AddST2a = this->getUniform("param1AddST2a")!=0;
			}
		}
		bool param1MultST3 = false;
		bool param4MultST3 = false;
		bool param4AddST3 = false;
		bool param1MultST3a = false;
		bool param1AddST3a = false;
        if(ST3)
		{
			if(OpST3<3)
			{
				param1MultST3 = this->getUniform("param1MultST3")!=0;
				param4MultST3 = this->getUniform("param4MultST3")!=0;
				param4AddST3 = this->getUniform("param4AddST3")!=0;
			}
            if(OpST3A>0)
			{
				param1MultST3a = this->getUniform("param1MultST3a")!=0;
				param1AddST3a = this->getUniform("param1AddST3a")!=0;
			}
		}
		////////emissivemap
		bool param1MultET = false;
		bool param4MultET = false;
		bool param4AddET = false;
		bool param1MultETa = false;
		bool param1AddETa = false;
		if(hasEmissive)
		{
			if(OpET!=3)
			{
				param1MultET = this->getUniform("param1MultET")!=0;
				param4MultET = this->getUniform("param4MultET")!=0;
				param4AddET = this->getUniform("param4AddET")!=0;
			}
			if(OpETA>0)
			{
				param1MultETa = this->getUniform("param1MultETa")!=0;
				param1AddETa = this->getUniform("param1AddETa")!=0;
			}
		}
		bool param1MultET2 = false;
		bool param4MultET2 = false;
		bool param4AddET2 = false;
		bool param1MultET2a = false;
		bool param1AddET2a = false;
		if(ET2)
		{
			if(OpET2!=3)
			{
				param1MultET2 = this->getUniform("param1MultET2")!=0;
				param4MultET2 = this->getUniform("param4MultET2")!=0;
				param4AddET2 = this->getUniform("param4AddET2")!=0;
			}
			if(OpET2A>0)
			{
				param1MultET2a = this->getUniform("param1MultET2a")!=0;
				param1AddET2a = this->getUniform("param1AddET2a")!=0;
			}
		}
		bool param1MultET3 = false;
		bool param4MultET3 = false;
		bool param4AddET3 = false;
		bool param1MultET3a = false;
		bool param1AddET3a = false;
		if(ET3)
		{
			if(OpET3!=3)
			{
				param1MultET3 = this->getUniform("param1MultET3")!=0;
				param4MultET3 = this->getUniform("param4MultET3")!=0;
				param4AddET3 = this->getUniform("param4AddET3")!=0;
			}
			if(OpET3A>0)
			{
				param1MultET3a = this->getUniform("param1MultET3a")!=0;
				param1AddET3a = this->getUniform("param1AddET3a")!=0;
			}
		}
		////////////ExternTexture
		bool UVS = false;
		bool UVSM = false;
		bool param1MultUVS = false;
        bool param4MultUVS = false;
        bool param4AddUVS = false;
		bool CW = false;
		bool CWM = false;
		bool CWMR = false;
		bool CWMG = false;
		bool CWMB = false;
		bool CWMA = false;
		bool param1MultCW = false;
		bool param4MultCW = false;
		bool param4AddCW = false;
		bool param3MultCWN = false;
        bool param3MultCWD = false;
		bool param3MultCWDa = false;
		bool param3MultCWS = false;
		bool param3MultCWSa = false;
		bool param3MultCWE = false;
		bool param3MultCWEa = false;
		const crTexture2D *externTex2D = dynamic_cast<const crTexture2D *>(this->getTextureAttribute(TEXTURE_UVSCRAMBLER,crStateAttribute::TEXTURE));
		crTexture::ExternTextureID externTextureID = crTexture::EXT_NONE;
		if(externTex2D)
		{
			externTextureID = externTex2D->getExternTextureID();
			switch (externTextureID)
			{
			case crTexture::EXT_NONE:
				break;
			case crTexture::UVSCRAMBLER:
				UVS = true;
				break;
			case crTexture::COLORWEIGHT:
				CW = true;
				break;
			}
		}
        if(UVS)
		{
            param1MultUVS = this->getUniform("param1MultUVS")!=0;
			param4MultUVS = this->getUniform("param4MultUVS")!=0;
			param4AddUVS = this->getUniform("param4AddUVS")!=0;
		}
		else if(CW)
		{
			param1MultCW = this->getUniform("param1MultCW")!=0;
			param4MultCW = this->getUniform("param4MultCW")!=0;
			param4AddCW = this->getUniform("param4AddCW")!=0;

			param3MultCWN = this->getUniform("param3MultCWN")!=0;
			param3MultCWD = this->getUniform("param3MultCWD")!=0;
			param3MultCWDa = this->getUniform("param3MultCWDa")!=0;
			param3MultCWE = this->getUniform("param3MultCWE")!=0;
			param3MultCWEa = this->getUniform("param3MultCWEa")!=0;
			param3MultCWS = this->getUniform("param3MultCWS")!=0;
			param3MultCWSa = this->getUniform("param3MultCWSa")!=0;
		}
		//////////////////////////////
		
		if(obj_ss)
		{
			//////////lightMap
			if(hasLightmap)
			{
				lightMat   = obj_ss->getUniform("lightMat")!=0;
			}
			//////////envmap
			if(hasEnvmap)
			{
				if(!param1MultENV)  param1MultENV  = obj_ss->getUniform("param1MultENV")!=0;
				if(!param4MultENV)  param4MultENV  = obj_ss->getUniform("param4MultENV")!=0;
				if(!param4AddENV)   param4AddENV   = obj_ss->getUniform("param4AddENV")!=0;
			}
            ///////////normal
			if(bump>0 && OpNT<3)
			{
			    normalMat   = obj_ss->getUniform("normalMat")!=0;
				if(!param1MultNT)  param1MultNT  = obj_ss->getUniform("param1MultNT")!=0;
				if(!param3MultNT)  param3MultNT  = obj_ss->getUniform("param3MultNT")!=0;
				if(!param3AddNT)   param3AddNT   = obj_ss->getUniform("param3AddNT")!=0;
			}
			if(NT2 && OpNT2<3)
			{
				normalMat2   = obj_ss->getUniform("normalMat2")!=0;
				if(!param1MultNT2) param1MultNT2 = obj_ss->getUniform("param1MultNT2")!=0;
				if(!param3MultNT2) param3MultNT2 = obj_ss->getUniform("param3MultNT2")!=0;
				if(!param3AddNT2)  param3AddNT2  = obj_ss->getUniform("param3AddNT2")!=0;
			}
			if(NT3 && OpNT3<3)
			{
				normalMat3   = obj_ss->getUniform("normalMat3")!=0;
				if(!param1MultNT3) param1MultNT3 = obj_ss->getUniform("param1MultNT3")!=0;
				if(!param3MultNT3) param3MultNT3 = obj_ss->getUniform("param3MultNT3")!=0;
				if(!param3AddNT3)  param3AddNT3  = obj_ss->getUniform("param3AddNT3")!=0;
			}
			///////////diffuse
			if(haseBase)
			{
				if(OpBT<3)
				{
					baseMat  = obj_ss->getUniform("baseMat")!=0;
					if(!param1MultBT)  param1MultBT  = obj_ss->getUniform("param1MultBT")!=0;
					if(!param4MultBT)  param4MultBT  = obj_ss->getUniform("param4MultBT")!=0;
					if(!param4AddBT)   param4AddBT   = obj_ss->getUniform("param4AddBT")!=0;
				}
				if(OpBTA>0)
				{
					baseMatA  = obj_ss->getUniform("baseMatA")!=0;
					if(!param1MultBTa) param1MultBTa = obj_ss->getUniform("param1MultBTa")!=0;
					if(!param1AddBTa)  param1AddBTa  = obj_ss->getUniform("param1AddBTa")!=0;
				}
			}
			if(hasDiffuse)
			{
				if(OpDT<3)
				{
					diffuseMat = obj_ss->getUniform("diffuseMat")!=0;
					if(!param1MultDT)  param1MultDT  = obj_ss->getUniform("param1MultDT")!=0;
					if(!param4MultDT)  param4MultDT  = obj_ss->getUniform("param4MultDT")!=0;
					if(!param4AddDT)   param4AddDT   = obj_ss->getUniform("param4AddDT")!=0;
				}
				if(OpDTA>0)
				{
					diffuseMatA = obj_ss->getUniform("diffuseMatA")!=0;
					if(!param1MultDTa) param1MultDTa = obj_ss->getUniform("param1MultDTa")!=0;
					if(!param1AddDTa)  param1AddDTa  = obj_ss->getUniform("param1AddDTa")!=0;
				}
			}
			if(DT2)
			{
				if(OpDT2<3)
				{
					diffuseMat2  = obj_ss->getUniform("diffuseMat2")!=0;
					if(!param1MultDT2) param1MultDT2 = obj_ss->getUniform("param1MultDT2")!=0;
					if(!param4MultDT2) param4MultDT2 = obj_ss->getUniform("param4MultDT2")!=0;
					if(!param4AddDT2)  param4AddDT2  = obj_ss->getUniform("param4AddDT2")!=0;
				}
				if(OpDT2A>0)
				{
					diffuseMat2A  = obj_ss->getUniform("diffuseMat2A")!=0;
					if(!param1MultDT2a) param1MultDT2a = obj_ss->getUniform("param1MultDT2a")!=0;
					if(!param1AddDT2a)  param1AddDT2a  = obj_ss->getUniform("param1AddDT2a")!=0;
				}
			}
			/////////specular
			if(hasSpecular)
			{
				if(OpST<3)
				{
					specularMat = obj_ss->getUniform("specularMat")!=0;
					if(!param1MultST)  param1MultST  = obj_ss->getUniform("param1MultST")!=0;
					if(!param4MultST)  param4MultST  = obj_ss->getUniform("param4MultST")!=0;
					if(!param4AddST)   param4AddST   = obj_ss->getUniform("param4AddST")!=0;
				}
                if(OpSTA>0)
				{
					specularMatA = obj_ss->getUniform("specularMatA")!=0;
					if(!param1MultSTa) param1MultSTa = obj_ss->getUniform("param1MultSTa")!=0;
					if(!param1AddSTa)  param1AddSTa  = obj_ss->getUniform("param1AddSTa")!=0;
				}
			}
			if(ST2)
			{
				if(OpST2<3)
				{
					specularMat2 = obj_ss->getUniform("specularMat2")!=0;
					if(!param1MultST2) param1MultST2 = obj_ss->getUniform("param1MultST2")!=0;
					if(!param4MultST2) param4MultST2 = obj_ss->getUniform("param4MultST2")!=0;
					if(!param4AddST2)  param4AddST2  = obj_ss->getUniform("param4AddST2")!=0;
				}
				if(OpST2A>0)
				{
					specularMat2A = obj_ss->getUniform("specularMat2A")!=0;
					if(!param1MultST2a) param1MultST2a = obj_ss->getUniform("param1MultST2a")!=0;
					if(!param1AddST2a)  param1AddST2a  = obj_ss->getUniform("param1AddST2a")!=0;
				}
			}
			if(ST3)
			{
				if(OpST3<3)
				{
					specularMat3 = obj_ss->getUniform("specularMat3")!=0;
					if(!param1MultST3) param1MultST3 = obj_ss->getUniform("param1MultST3")!=0;
					if(!param4MultST3) param4MultST3 = obj_ss->getUniform("param4MultST3")!=0;
					if(!param4AddST3)  param4AddST3  = obj_ss->getUniform("param4AddST3")!=0;
				}
				if(OpST3A>0)
				{
					specularMat3A = obj_ss->getUniform("specularMat3A")!=0;
					if(!param1MultST3a) param1MultST3a = obj_ss->getUniform("param1MultST3a")!=0;
					if(!param1AddST3a)  param1AddST3a  = obj_ss->getUniform("param1AddST3a")!=0;
				}
			}
			////////emissive
			if(hasEmissive)
			{
				if(OpET!=3)
				{
					emissiveMat = obj_ss->getUniform("emissiveMat")!=0;
					if(!param1MultET)  param1MultET  = obj_ss->getUniform("param1MultET")!=0;
					if(!param4MultET)  param4MultET  = obj_ss->getUniform("param4MultET")!=0;
					if(!param4AddET)   param4AddET   = obj_ss->getUniform("param4AddET")!=0;
				}
				if(OpETA>0)
				{
					emissiveMatA = obj_ss->getUniform("emissiveMatA")!=0;
					if(!param1MultETa) param1MultETa = obj_ss->getUniform("param1MultETa")!=0;
					if(!param1AddETa)  param1AddETa  = obj_ss->getUniform("param1AddETa")!=0;
				}
			}
            if(ET2)
			{
				if(OpET2!=3)
				{
					emissiveMat2 = obj_ss->getUniform("emissiveMat2")!=0;
					if(!param1MultET2) param1MultET2 = obj_ss->getUniform("param1MultET2")!=0;
					if(!param4MultET2) param4MultET2 = obj_ss->getUniform("param4MultET2")!=0;
					if(!param4AddET2)  param4AddET2  = obj_ss->getUniform("param4AddET2")!=0;
				}
				if(OpET2A>0)
				{
					emissiveMat2A = obj_ss->getUniform("emissiveMat2A")!=0;
					if(!param1MultET2a) param1MultET2a = obj_ss->getUniform("param1MultET2a")!=0;
					if(!param1AddET2a)  param1AddET2a  = obj_ss->getUniform("param1AddET2a")!=0;
				}
			}
			if(ET3)
			{
				if(OpET3!=3)
				{
					emissiveMat3 = obj_ss->getUniform("emissiveMat3")!=0;
					if(!param1MultET3) param1MultET3 = obj_ss->getUniform("param1MultET3")!=0;
					if(!param4MultET3) param4MultET3 = obj_ss->getUniform("param4MultET3")!=0;
					if(!param4AddET3)  param4AddET3  = obj_ss->getUniform("param4AddET3")!=0;
				}
				if(OpET3A>0)
				{
					emissiveMat3A = obj_ss->getUniform("emissiveMat3A")!=0;
					if(!param1MultET3a) param1MultET3a = obj_ss->getUniform("param1MultET3a")!=0;
					if(!param1AddET3a)  param1AddET3a  = obj_ss->getUniform("param1AddET3a")!=0;
				}
			}
            ////////////ExternTexture
			if(UVS)
			{
				UVSM  = obj_ss->getUniform("UVSMat")!=0;
				if(!param1MultUVS) param1MultUVS = obj_ss->getUniform("param1MultUVS")!=0;
				if(!param4MultUVS) param4MultUVS = obj_ss->getUniform("param4MultUVS")!=0;
				if(!param4AddUVS)  param4AddUVS  = obj_ss->getUniform("param4AddUVS")!=0;
			}
			else if(CW)
			{
				CWMR  = obj_ss->getUniform("CWMatR")!=0;
				CWMG  = obj_ss->getUniform("CWMatG")!=0;
				CWMB  = obj_ss->getUniform("CWMatB")!=0;
				CWMA  = obj_ss->getUniform("CWMatA")!=0;
				if(!(CWMR && CWMG && CWMB && CWMA))
				{
				    CWM  = obj_ss->getUniform("CWMat")!=0;
				}
				if(!param1MultCW) param1MultCW = obj_ss->getUniform("param1MultCW")!=0;
				if(!param4MultCW) param4MultCW = obj_ss->getUniform("param4MultCW")!=0;
				if(!param4AddCW)  param4AddCW  = obj_ss->getUniform("param4AddCW")!=0;

				if(!param3MultCWN) param3MultCWN = obj_ss->getUniform("param3MultCWN")!=0;
				if(!param3MultCWD) param3MultCWD = obj_ss->getUniform("param3MultCWD")!=0;
				if(!param3MultCWDa) param3MultCWDa = obj_ss->getUniform("param3MultCWDa")!=0;
				if(!param3MultCWE) param3MultCWE = obj_ss->getUniform("param3MultCWE")!=0;
				if(!param3MultCWEa) param3MultCWEa = obj_ss->getUniform("param3MultCWEa")!=0;
				if(!param3MultCWS) param3MultCWS = obj_ss->getUniform("param3MultCWS")!=0;
				if(!param3MultCWSa) param3MultCWSa = obj_ss->getUniform("param3MultCWSa")!=0;
			}
			//////////////////////////////
		}
		if(bump>0 && OpNT<3)
		{
			if(!normalMat)
			{
				normalMat = this->getUniform("normalMat")!=0;
			}
		}
		if(NT2 && OpNT2<3)
		{
			if(!normalMat2)
			{
				normalMat2 = this->getUniform("normalMat2")!=0;
			}
		}
		if(NT3 && OpNT3<3)
		{
			if(!normalMat3)
			{
				normalMat3 = this->getUniform("normalMat3")!=0;
			}
		}
		if(haseBase)
		{
			if(!baseMat)
			{
				baseMat = this->getUniform("baseMat")!=0;
			}
			if(!baseMatA)
			{
				baseMatA = this->getUniform("baseMatA")!=0;
			}
		}
		if(hasDiffuse)
		{
			if(!diffuseMat)
			{
				diffuseMat = this->getUniform("diffuseMat")!=0;
			}
			if(!diffuseMatA)
			{
				diffuseMatA = this->getUniform("diffuseMatA")!=0;
			}
		}
		if(DT2)
		{
			if(!diffuseMat2)
			{
				diffuseMat2 = this->getUniform("diffuseMat2")!=0;
			}
			if(!diffuseMat2A)
			{
				diffuseMat2A = this->getUniform("diffuseMat2A")!=0;
			}
		}
		if(hasSpecular)
		{
			if(!specularMat)
			{
				specularMat = this->getUniform("specularMat")!=0;
			}
			if(!specularMatA)
			{
				specularMatA = this->getUniform("specularMatA")!=0;
			}
		}
		if(ST2)
		{
			if(!specularMat2)
			{
				specularMat2 = this->getUniform("specularMat2")!=0;
			}
			if(!specularMat2A)
			{
				specularMat2A = this->getUniform("specularMat2A")!=0;
			}
		}
		if(ST3)
		{
			if(!specularMat3)
			{
				specularMat3 = this->getUniform("specularMat3")!=0;
			}
			if(!specularMat3A)
			{
				specularMat3A = this->getUniform("specularMat3A")!=0;
			}
		}
		if(hasEmissive)
		{
			if(!emissiveMat)
			{
				emissiveMat = this->getUniform("emissiveMat")!=0;
			}
			if(!emissiveMatA)
			{
				emissiveMatA = this->getUniform("emissiveMatA")!=0;
			}
		}
		if(ET2)
		{
			if(!emissiveMat2)
			{
				emissiveMat2 = this->getUniform("emissiveMat2")!=0;
			}
			if(!emissiveMat2A)
			{
				emissiveMat2A = this->getUniform("emissiveMat2A")!=0;
			}
		}
		if(ET3)
		{
			if(!emissiveMat3)
			{
				emissiveMat3 = this->getUniform("emissiveMat3")!=0;
			}
			if(!emissiveMat3A)
			{
				emissiveMat3A = this->getUniform("emissiveMat3A")!=0;
			}
		}
		if(UVS)
		{
			if(!UVSM)
			{
				UVSM = this->getUniform("UVSMat")!=0;
			}
		}
		else if(CW)
		{
			if(!CWMR)
			{
				CWMR = this->getUniform("CWMatR")!=0;
			}
			if(!CWMG)
			{
				CWMG = this->getUniform("CWMatG")!=0;
			}
			if(!CWMB)
			{
				CWMB = this->getUniform("CWMatB")!=0;
			}
			if(!CWMA)
			{
				CWMA = this->getUniform("CWMatA")!=0;
			}
			if(!(CWMR && CWMG && CWMB && CWMA))
			{
				if(!CWM)
				{
					CWM = this->getUniform("CWMat")!=0;
				}
			}
		}
		/////////Others
		if(hasLightmap)
		{
			RtglmShaderStr += "_lm";
			if(lightMat)
			{
				RtglmShaderStr += "_LM";
			}
		}
		if(hasEnvmap)
		{
			RtglmShaderStr += "_env";
			if(param1MultENV)
			{
				RtglmShaderStr += "_Param1MultENV";
			}
			if(param4MultENV)
			{
				RtglmShaderStr += "_Param4MultENV";
			}
			if(param4AddENV)
			{
				RtglmShaderStr += "_Param4AddENV";
			}
		}
        ///////////normal
		if(bump>0 && OpNT<3)
		{
			switch(bump)
			{
			case 1:
				RtglmShaderStr += "_NT";
				break;
			case 2:
				RtglmShaderStr += "_PT";
				break;
			}
			if(normalMat)
			{
				RtglmShaderStr += "_NM";
			}
			//switch(OpNT)
			//{
			//case 0:
			//	m_getRtglmShaderStr += "_IGNORENT";
			//	break;
			//case 1:
			//	m_getRtglmShaderStr += "_REPLACENT";
			//	break;
			//case 2:
			//	m_getRtglmShaderStr += "_ADDNT";
			//	break;
			//case 3:
			//	m_getRtglmShaderStr += "_MULTNT";
			//	break;
			//}

			if(param1MultNT)
			{
				RtglmShaderStr += "_Param1MultNT";
			}
			if(param3MultNT)
			{
				RtglmShaderStr += "_Param3MultNT";
			}
			if(param3AddNT)
			{
				RtglmShaderStr += "_Param3AddNT";
			}
		}
        if(NT2 && OpNT2<3)
		{
			RtglmShaderStr += "_NT2";
			if(this->getTextureUV(TEXTURE_NORMALMAP2) == 1)
			{
				RtglmShaderStr += "_NT2UV1";
			}
			if(normalMat2)
			{
				RtglmShaderStr += "_NM2";
			}
			switch(OpNT2)
			{
			case 0:
				RtglmShaderStr += "_ADDNT2";
				break;
			case 1:
				RtglmShaderStr += "_MULTNT2";
				break;
			case 2:
				RtglmShaderStr += "_REPLACENT2";
				break;
			case 3:
				break;
			}
			if(param1MultNT2)
			{
				RtglmShaderStr += "_Param1MultNT2";
			}
			if(param3MultNT2)
			{
				RtglmShaderStr += "_Param3MultNT2";
			}
			if(param3AddNT2)
			{
				RtglmShaderStr += "_Param3AddNT2";
			}
		}
        if(NT3 && OpNT3<3)
		{
			RtglmShaderStr += "_NT3";
			if(this->getTextureUV(TEXTURE_NORMALMAP3) == 1)
			{
				RtglmShaderStr += "_NT3UV1";
			}
			if(normalMat3)
			{
				RtglmShaderStr += "_NM3";
			}
			switch(OpNT3)
			{
			case 0:
				RtglmShaderStr += "_ADDNT3";
				break;
			case 1:
				RtglmShaderStr += "_MULTNT3";
				break;
			case 2:
				RtglmShaderStr += "_REPLACENT3";
				break;
			case 3:
				break;
			}
			if(param1MultNT3)
			{
				RtglmShaderStr += "_Param1MultNT3";
			}
			if(param3MultNT3)
			{
				RtglmShaderStr += "_Param3MultNT3";
			}
			if(param3AddNT3)
			{
				RtglmShaderStr += "_Param3AddNT3";
			}
		}

        /////////diffuse
		if(haseBase)
		{
			if(OpBT>=3 && OpBTA==0)
				haseBase = false;
			if(haseBase)
			{
	            RtglmShaderStr += "_BT";
				if(this->getTextureUV(TEXTURE_BASEMAP) == 1)
				{
					RtglmShaderStr += "_BTUV1";
				}
				if(baseMat)
				{
					RtglmShaderStr += "_BM";
				}
				if(baseMatA)
				{
					RtglmShaderStr += "_BMa";
				}

				switch(OpBT)
				{
				case 0:
					RtglmShaderStr += "_ADDBT";
					break;
				case 1:
					RtglmShaderStr += "_MULTBT";
					break;
				case 2:
					RtglmShaderStr += "_REPLACEBT";
					break;
				case 3:
					break;
				}

				switch(OpBTA)
				{
				case 1:
					RtglmShaderStr += "_ADDBTa";
					break;
				case 2:
					RtglmShaderStr += "_MULTBTa";
					break;
				case 3:
					RtglmShaderStr += "_REPLACEBTa";
					break;
				}

				if(param1MultBT)
				{
					RtglmShaderStr += "_Param1MultBT";
				}
				if(param4MultBT)
				{
					RtglmShaderStr += "_Param4MultBT";
				}
				if(param4AddBT)
				{
					RtglmShaderStr += "_Param4AddBT";
				}
				if(param1MultBTa)
				{
					RtglmShaderStr += "_Param1MultBTa";
				}
				if(param1AddBTa)
				{
					RtglmShaderStr += "_Param1AddBTa";
				}
			}
		}
		if(hasDiffuse)
		{
			if(OpDT>=3 && OpDTA==0)
				hasDiffuse = false;
			if(hasDiffuse)
			{
			    RtglmShaderStr += "_DT";
				if(this->getTextureUV(TEXTURE_DIFFUSEMAP) == 1)
				{
					RtglmShaderStr += "_DTUV1";
				}
				if(diffuseMat)
				{
					RtglmShaderStr += "_DM";
				}
				if(diffuseMatA)
				{
					RtglmShaderStr += "_DMa";
				}

				switch(OpDT)
				{
				case 0:
					RtglmShaderStr += "_ADDDT";
					break;
				case 1:
					RtglmShaderStr += "_MULTDT";
					break;
				case 2:
					RtglmShaderStr += "_REPLACEDT";
					break;
				case 3:
					break;
				}

				switch(OpDTA)
				{
				case 1:
					RtglmShaderStr += "_ADDDTa";
					break;
				case 2:
					RtglmShaderStr += "_MULTDTa";
					break;
				case 3:
					RtglmShaderStr += "_REPLACEDTa";
					break;
				}

				if(param1MultDT)
				{
					RtglmShaderStr += "_Param1MultDT";
				}
				if(param4MultDT)
				{
					RtglmShaderStr += "_Param4MultDT";
				}
				if(param4AddDT)
				{
					RtglmShaderStr += "_Param4AddDT";
				}
				if(param1MultDTa)
				{
					RtglmShaderStr += "_Param1MultDTa";
				}
				if(param1AddDTa)
				{
					RtglmShaderStr += "_Param1AddDTa";
				}
			}
		}
		if(DT2)
		{
			if(OpDT2>=3 && OpDT2A==0)
				DT2 = false;
			if(DT2)
			{
			    RtglmShaderStr += "_DT2";
				if(this->getTextureUV(TEXTURE_DIFFUSEMAP2) == 1)
				{
					RtglmShaderStr += "_DT2UV1";
				}
				if(diffuseMat2)
				{
					RtglmShaderStr += "_DM2";
				}
				if(diffuseMat2A)
				{
					RtglmShaderStr += "_DM2a";
				}

				switch(OpDT2)
				{
				case 0:
					RtglmShaderStr += "_ADDDT2";
					break;
				case 1:
					RtglmShaderStr += "_MULTDT2";
					break;
				case 2:
					RtglmShaderStr += "_REPLACEDT2";
					break;
				case 3:
					break;
				}

				switch(OpDT2A)
				{
				case 1:
					RtglmShaderStr += "_ADDDT2a";
					break;
				case 2:
					RtglmShaderStr += "_MULTDT2a";
					break;
				case 3:
					RtglmShaderStr += "_REPLACEDT2a";
					break;
				}

				if(param1MultDT2)
				{
					RtglmShaderStr += "_Param1MultDT2";
				}
				if(param4MultDT2)
				{
					RtglmShaderStr += "_Param4MultDT2";
				}
				if(param4AddDT2)
				{
					RtglmShaderStr += "_Param4AddDT2";
				}
				if(param1MultDT2a)
				{
					RtglmShaderStr += "_Param1MultDT2a";
				}
				if(param1AddDT2a)
				{
					RtglmShaderStr += "_Param1AddDT2a";
				}
			}
		}
		////////specular
		if(hasSpecular)
		{
			if(OpST>=3 && OpSTA==0)
				hasSpecular = false;
			if(hasSpecular)
			{
				RtglmShaderStr += "_ST";
				if(this->getTextureUV(TEXTURE_SPECULARMAP) == 1)
				{
					RtglmShaderStr += "_STUV1";
				}
				if(specularMat)
				{
					RtglmShaderStr += "_SM";
				}
				if(specularMatA)
				{
					RtglmShaderStr += "_SMa";
				}

				switch(OpST)
				{
				case 0:
					RtglmShaderStr += "_ADDST";
					break;
				case 1:
					RtglmShaderStr += "_MULTST";
					break;
				case 2:
					RtglmShaderStr += "_REPLACEST";
					break;
				case 3:
					break;
				}

				switch(OpSTA)
				{
				case 1:
					RtglmShaderStr += "_ADDSTa";
					break;
				case 2:
					RtglmShaderStr += "_MULTSTa";
					break;
				case 3:
					RtglmShaderStr += "_REPLACESTa";
					break;
				}

				if(param1MultST)
				{
					RtglmShaderStr += "_Param1MultST";
				}
				if(param4MultST)
				{
					RtglmShaderStr += "_Param4MultST";
				}
				if(param4AddST)
				{
					RtglmShaderStr += "_Param4AddST";
				}
				if(param1MultSTa)
				{
					RtglmShaderStr += "_Param1MultSTa";
				}
				if(param1AddSTa)
				{
					RtglmShaderStr += "_Param1AddSTa";
				}
			}
		}
		if(ST2)
		{
			if(OpST2>=3 && OpST2A==0)
				ST2 = false;
			if(ST2)
			{
			    RtglmShaderStr += "_ST2";
				if(this->getTextureUV(TEXTURE_SPECULARMAP2) == 1)
				{
					RtglmShaderStr += "_ST2UV1";
				}
				if(specularMat2)
				{
					RtglmShaderStr += "_SM2";
				}
				if(specularMat2A)
				{
					RtglmShaderStr += "_SM2a";
				}

				switch(OpST2)
				{
				case 0:
					RtglmShaderStr += "_ADDST2";
					break;
				case 1:
					RtglmShaderStr += "_MULTST2";
					break;
				case 2:
					RtglmShaderStr += "_REPLACEST2";
					break;
				case 3:
					break;
				}

				switch(OpST2A)
				{
				case 1:
					RtglmShaderStr += "_ADDST2a";
					break;
				case 2:
					RtglmShaderStr += "_MULTST2a";
					break;
				case 3:
					RtglmShaderStr += "_REPLACEST2a";
					break;
				}

				if(param1MultST2)
				{
					RtglmShaderStr += "_Param1MultST2";
				}
				if(param4MultST2)
				{
					RtglmShaderStr += "_Param4MultST2";
				}
				if(param4AddST2)
				{
					RtglmShaderStr += "_Param4AddST2";
				}
				if(param1MultST2a)
				{
					RtglmShaderStr += "_Param1MultST2a";
				}
				if(param1AddST2a)
				{
					RtglmShaderStr += "_Param1AddST2a";
				}
			}
		}
		if(ST3)
		{
			if(OpST3>=3 && OpST3A==0)
				ST3 = false;
			if(ST3)
			{
			    RtglmShaderStr += "_ST3";
				if(this->getTextureUV(TEXTURE_SPECULARMAP3) == 1)
				{
					RtglmShaderStr += "_ST3UV1";
				}
				if(specularMat3)
				{
					RtglmShaderStr += "_SM3";
				}
				if(specularMat3A)
				{
					RtglmShaderStr += "_SM3a";
				}

				switch(OpST3)
				{
				case 0:
					RtglmShaderStr += "_ADDST3";
					break;
				case 1:
					RtglmShaderStr += "_MULTST3";
					break;
				case 2:
					RtglmShaderStr += "_REPLACEST3";
					break;
				case 3:
					break;
				}

				switch(OpST3A)
				{
				case 1:
					RtglmShaderStr += "_ADDST3a";
					break;
				case 2:
					RtglmShaderStr += "_MULTST3a";
					break;
				case 3:
					RtglmShaderStr += "_REPLACEST3a";
					break;
				}

				if(param1MultST3)
				{
					RtglmShaderStr += "_Param1MultST3";
				}
				if(param4MultST3)
				{
					RtglmShaderStr += "_Param4MultST3";
				}
				if(param4AddST3)
				{
					RtglmShaderStr += "_Param4AddST3";
				}
				if(param1MultST3a)
				{
					RtglmShaderStr += "_Param1MultST3a";
				}
				if(param1AddST3a)
				{
					RtglmShaderStr += "_Param1AddST3a";
				}
			}
		}
		////////emissive
		if(hasEmissive)
		{
			if(OpET==3 && OpETA==0)
				hasEmissive = false;
			if(hasEmissive)
			{
			    RtglmShaderStr += "_ET";
				if(this->getTextureUV(TEXTURE_EMISSIVEMAP) == 1)
				{
					RtglmShaderStr += "_ETUV1";
				}
				if(emissiveMat)
				{
					RtglmShaderStr += "_EM";
				}
				if(emissiveMatA)
				{
					RtglmShaderStr += "_EMa";
				}

				switch(OpET)
				{
				case 0:
					RtglmShaderStr += "_ADDET";
					break;
				case 1:
					RtglmShaderStr += "_MULTET";
					break;
				case 2:
					RtglmShaderStr += "_REPLACEET";
					break;
				case 3:
					break;
				case 4:
					RtglmShaderStr += "_ADDETTON";
					break;
				case 5:
					RtglmShaderStr += "_MULTETTON";
					break;
				}

				switch(OpETA)
				{
				case 1:
					RtglmShaderStr += "_ADDETa";
					break;
				case 2:
					RtglmShaderStr += "_MULTETa";
					break;
				case 3:
					RtglmShaderStr += "_REPLACEETa";
					break;
				}

				if(param1MultET)
				{
					RtglmShaderStr += "_Param1MultET";
				}
				if(param4MultET)
				{
					RtglmShaderStr += "_Param4MultET";
				}
				if(param4AddET)
				{
					RtglmShaderStr += "_Param4AddET";
				}
				if(param1MultETa)
				{
					RtglmShaderStr += "_Param1MultETa";
				}
				if(param1AddETa)
				{
					RtglmShaderStr += "_Param1AddETa";
				}
			}
		}
		if(ET2)
		{
			if(OpET2==3 && OpET2A==0)
				ET2 = false;
			if(ET2)
			{
			    RtglmShaderStr += "_ET2";
				if(this->getTextureUV(TEXTURE_EMISSIVEMAP2) == 1)
				{
					RtglmShaderStr += "_ET2UV1";
				}
				if(emissiveMat2)
				{
					RtglmShaderStr += "_EM2";
				}
				if(emissiveMat2A)
				{
					RtglmShaderStr += "_EM2a";
				}

				switch(OpET2)
				{
				case 0:
					RtglmShaderStr += "_ADDET2";
					break;
				case 1:
					RtglmShaderStr += "_MULTET2";
					break;
				case 2:
					RtglmShaderStr += "_REPLACEET2";
					break;
				case 3:
					break;
				case 4:
					RtglmShaderStr += "_ADDET2TOD";
					break;
				case 5:
					RtglmShaderStr += "_MULTET2TOD";
					break;
				}

				switch(OpET2A)
				{
				case 1:
					RtglmShaderStr += "_ADDET2a";
					break;
				case 2:
					RtglmShaderStr += "_MULTET2a";
					break;
				case 3:
					RtglmShaderStr += "_REPLACEET2a";
					break;
				}

				if(param1MultET2)
				{
					RtglmShaderStr += "_Param1MultET2";
				}
				if(param4MultET2)
				{
					RtglmShaderStr += "_Param4MultET2";
				}
				if(param4AddET2)
				{
					RtglmShaderStr += "_Param4AddET2";
				}
				if(param1MultET2a)
				{
					RtglmShaderStr += "_Param1MultET2a";
				}
				if(param1AddET2a)
				{
					RtglmShaderStr += "_Param1AddET2a";
				}
			}
		}
		if(ET3)
		{
			if(OpET3 == 6)
			{//ET3ToLM
				RtglmShaderStr += "_lm2";
				if(emissiveMat3)
				{
					RtglmShaderStr += "_LM2";
				}
			}
			else
			{
				if(OpET3==3 && OpET3A==0)
					ET3 = false;
				if(ET3)
				{
					RtglmShaderStr += "_ET3";
					if(this->getTextureUV(TEXTURE_EMISSIVEMAP3) == 1)
					{
						RtglmShaderStr += "_ET3UV1";
					}
					if(emissiveMat3)
					{
						RtglmShaderStr += "_EM3";
					}
					if(emissiveMat3A)
					{
						RtglmShaderStr += "_EM3a";
					}

					switch(OpET3)
					{
					case 0:
						RtglmShaderStr += "_ADDET3";
						break;
					case 1:
						RtglmShaderStr += "_MULTET3";
						break;
					case 2:
						RtglmShaderStr += "_REPLACEET3";
						break;
					case 3:
						break;
					case 4:
						RtglmShaderStr += "_ADDET3TOS";
						break;
					case 5:
						RtglmShaderStr += "_MULTET3TOS";
						break;
					}

					switch(OpET3A)
					{
					case 1:
						RtglmShaderStr += "_ADDET3a";
						break;
					case 2:
						RtglmShaderStr += "_MULTET3a";
						break;
					case 3:
						RtglmShaderStr += "_REPLACEET3a";
						break;
					}

					if(param1MultET3)
					{
						RtglmShaderStr += "_Param1MultET3";
					}
					if(param4MultET3)
					{
						RtglmShaderStr += "_Param4MultET3";
					}
					if(param4AddET3)
					{
						RtglmShaderStr += "_Param4AddET3";
					}
					if(param1MultET3a)
					{
						RtglmShaderStr += "_Param1MultET3a";
					}
					if(param1AddET3a)
					{
						RtglmShaderStr += "_Param1AddET3a";
					}
				}
			}
		}
		////////////ExternTexture
		if(UVS)
		{
			RtglmShaderStr += "_UVS";
			if(this->getTextureUV(TEXTURE_UVSCRAMBLER) == 1)
			{
				RtglmShaderStr += "_UVSUV1";
			}
			if(UVSM)
			{
				RtglmShaderStr += "_UVSM";
			}
			if(param1MultUVS)
			{
				RtglmShaderStr += "_Param1MultUVS";
			}
			if(param4MultUVS)
			{
				RtglmShaderStr += "_Param4MultUVS";
			}
			if(param4AddUVS)
			{
				RtglmShaderStr += "_Param4AddUVS";
			}
		}
		else if(CW)
		{
			RtglmShaderStr += "_CW";
			if(this->getTextureUV(TEXTURE_COLORWEIGHT) == 1)
			{
				RtglmShaderStr += "_CWUV1";
			}
			if(CWM)
			{
				RtglmShaderStr += "_CWM";
			}
			if(CWMR)
			{
				RtglmShaderStr += "_CWMR";
			}
			if(CWMG)
			{
				RtglmShaderStr += "_CWMG";
			}
			if(CWMB)
			{
				RtglmShaderStr += "_CWMB";
			}
			if(CWMA)
			{
				RtglmShaderStr += "_CWMA";
			}
			if(param1MultCW)
			{
				RtglmShaderStr += "_Param1MultCW";
			}
			if(param4MultCW)
			{
				RtglmShaderStr += "_Param4MultCW";
			}
			if(param4AddCW)
			{
				RtglmShaderStr += "_Param4AddCW";
			}

			if(param3MultCWN) 
			{
				RtglmShaderStr += "_Param3MultCWN";
			}
			if(param3MultCWD) 
			{
				RtglmShaderStr += "_Param3MultCWD";
			}
			if(param3MultCWDa) 
			{
				RtglmShaderStr += "_Param3MultCWDa";
			}
			if(param3MultCWE) 
			{
				RtglmShaderStr += "_Param3MultCWE";
			}
			if(param3MultCWEa) 
			{
				RtglmShaderStr += "_Param3MultCWEa";
			}
			if(param3MultCWS) 
			{
				RtglmShaderStr += "_Param3MultCWS";
			}
			if(param3MultCWSa) 
			{
				RtglmShaderStr += "_Param3MultCWSa";
			}
		}

		///////////////////////////////
		RtglmShaderStr += "_";
		m_shaderStrMap[obj_ss] = RtglmShaderStr;
		return m_shaderStrMap[obj_ss]; 
	}
}

//const std::string &crStateSet::getRtplmShaderStr(const crStateSet *drawable_ss) const
//{
//	if(m_getRtplmShaderStr.empty())
//	{
//		const crStateSet *this, *obj_ss;
//		if(drawable_ss)
//		{
//			this = drawable_ss;
//			obj_ss = this;
//		}
//		else
//		{
//			this = this;
//			obj_ss = NULL;
//		}
//
//		const crTexture *texture = dynamic_cast<const crTexture *>(this->getTextureAttribute(TEXTURE_BUMPMAP,crStateAttribute::TEXTURE));
//		int bump = 0;
//		if(texture)
//		{
//			bump = texture->getUseParallax()?2:1;
//		}
//		bool hasEnvmap = this->getTextureAttribute(TEXTURE_ENVMAP,crStateAttribute::TEXTURE)!=0;
//		if(!hasEnvmap && obj_ss)
//		{
//			hasEnvmap = obj_ss->getTextureAttribute(TEXTURE_ENVMAP,crStateAttribute::TEXTURE)!=0;
//		}
//		bool hasDiffuse = this->getTextureAttribute(TEXTURE_DIFFUSEMAP,crStateAttribute::TEXTURE)!=0;
//		texture = dynamic_cast<const crTexture *>(this->getTextureAttribute(TEXTURE_SPECULARMAP,crStateAttribute::TEXTURE));
//		int specular = 0;
//		if(texture)
//		{
//			specular = texture->getAlphaShininessEnable()?2:1;
//		}
//
//		bool baseMat = false;
//		bool baseMatA = false;
//		bool normalMat = false;
//		bool diffuseMat = false;
//		bool specularMat = false;
//
//		bool NT2 = this->getTextureAttribute(TEXTURE_NORMALMAP2,crStateAttribute::TEXTURE)!=0;
//		bool normalMat2 = false;
//		int OpNT2 = this->getTextureOperate(TEXTURE_NORMALMAP2);
//		bool NT3 = this->getTextureAttribute(TEXTURE_NORMALMAP3,crStateAttribute::TEXTURE)!=0;
//		bool normalMat3 = false;
//		int OpNT3 = this->getTextureOperate(TEXTURE_NORMALMAP3);
//
//		int OpDT = this->getTextureOperate(TEXTURE_DIFFUSEMAP);
//
//		bool DT2 = this->getTextureAttribute(TEXTURE_DIFFUSEMAP2,crStateAttribute::TEXTURE)!=0;
//		bool diffuseMat2 = false;
//		int OpDT2 = this->getTextureOperate(TEXTURE_DIFFUSEMAP2);
//		bool DT3 = this->getTextureAttribute(TEXTURE_DIFFUSEMAP3,crStateAttribute::TEXTURE)!=0;
//		bool diffuseMat3 = false;
//		int OpDT3 = this->getTextureOperate(TEXTURE_DIFFUSEMAP3);
//
//		bool ST2 = this->getTextureAttribute(TEXTURE_SPECULARMAP2,crStateAttribute::TEXTURE)!=0;
//		bool specularMat2 = false;
//		int OpST2 = this->getTextureOperate(TEXTURE_SPECULARMAP2);
//		bool ST3 = this->getTextureAttribute(TEXTURE_SPECULARMAP3,crStateAttribute::TEXTURE)!=0;
//		bool specularMat3 = false;
//		int OpST3 = this->getTextureOperate(TEXTURE_SPECULARMAP3);
//
//		bool param1MultENV = this->getUniform("param1MultENV")!=0;
//		bool param4MultENV = this->getUniform("param4MultENV")!=0;
//		bool param4AddENV = this->getUniform("param4AddENV")!=0;
//
//		bool param1MultBT = this->getUniform("param1MultBT")!=0;
//		bool param4MultBT = this->getUniform("param4MultBT")!=0;
//		bool param4AddBT = this->getUniform("param4AddBT")!=0;
//		bool param1MultBTa = this->getUniform("param1MultBTa")!=0;
//		bool param1AddBTa = this->getUniform("param1AddBTa")!=0;
//
//		bool param1MultNT = this->getUniform("param1MultNT")!=0;
//		bool param3MultNT = this->getUniform("param3MultNT")!=0;
//		bool param3AddNT = this->getUniform("param3AddNT")!=0;
//		bool param1MultNT2 = this->getUniform("param1MultNT2")!=0;
//		bool param3MultNT2 = this->getUniform("param3MultNT2")!=0;
//		bool param3AddNT2 = this->getUniform("param3AddNT2")!=0;
//		bool param1MultNT3 = this->getUniform("param1MultNT3")!=0;
//		bool param3MultNT3 = this->getUniform("param3MultNT3")!=0;
//		bool param3AddNT3 = this->getUniform("param3AddNT3")!=0;
//
//		bool param1MultDT = this->getUniform("param1MultDT")!=0;
//		bool param4MultDT = this->getUniform("param4MultDT")!=0;
//		bool param4AddDT = this->getUniform("param4AddDT")!=0;
//		bool param1MultDT2 = this->getUniform("param1MultDT2")!=0;
//		bool param4MultDT2 = this->getUniform("param4MultDT2")!=0;
//		bool param4AddDT2 = this->getUniform("param4AddDT2")!=0;
//		bool param1MultDT3 = this->getUniform("param1MultDT3")!=0;
//		bool param4MultDT3 = this->getUniform("param4MultDT3")!=0;
//		bool param4AddDT3 = this->getUniform("param4AddDT3")!=0;
//
//		bool param1MultST = this->getUniform("param1MultST")!=0;
//		bool param4MultST = this->getUniform("param4MultST")!=0;
//		bool param4AddST = this->getUniform("param4AddST")!=0;
//		bool param1MultST2 = this->getUniform("param1MultST2")!=0;
//		bool param4MultST2 = this->getUniform("param4MultST2")!=0;
//		bool param4AddST2 = this->getUniform("param4AddST2")!=0;
//		bool param1MultST3 = this->getUniform("param1MultST3")!=0;
//		bool param4MultST3 = this->getUniform("param4MultST3")!=0;
//		bool param4AddST3 = this->getUniform("param4AddST3")!=0;
//
//		if(obj_ss)
//		{
//			baseMat  = obj_ss->getUniform("baseMat")!=0;
//			baseMatA  = obj_ss->getUniform("baseMatA")!=0;
//			normalMat   = obj_ss->getUniform("normalMat")!=0;
//			diffuseMat  = obj_ss->getUniform("diffuseMat")!=0;
//			specularMat = obj_ss->getUniform("specularMat")!=0;
//
//			normalMat2   = obj_ss->getUniform("normalMat2")!=0;
//			diffuseMat2  = obj_ss->getUniform("diffuseMat2")!=0;
//			specularMat2 = obj_ss->getUniform("specularMat2")!=0;
//
//			normalMat3   = obj_ss->getUniform("normalMat3")!=0;
//			diffuseMat3  = obj_ss->getUniform("diffuseMat3")!=0;
//			specularMat3 = obj_ss->getUniform("specularMat3")!=0;
//
//			if(!param1MultENV)  param1MultENV  = obj_ss->getUniform("param1MultENV")!=0;
//			if(!param4MultENV)  param4MultENV  = obj_ss->getUniform("param4MultENV")!=0;
//			if(!param4AddENV)   param4AddENV   = obj_ss->getUniform("param4AddENV")!=0;
//
//			if(!param1MultBT)  param1MultBT  = obj_ss->getUniform("param1MultBT")!=0;
//			if(!param4MultBT)  param4MultBT  = obj_ss->getUniform("param4MultBT")!=0;
//			if(!param4AddBT)   param4AddBT   = obj_ss->getUniform("param4AddBT")!=0;
//			if(!param1MultBTa) param1MultBTa = obj_ss->getUniform("param1MultBTa")!=0;
//			if(!param1AddBTa)  param1AddBTa  = obj_ss->getUniform("param1AddBTa")!=0;
//
//			if(!param1MultNT)  param1MultNT  = obj_ss->getUniform("param1MultNT")!=0;
//			if(!param3MultNT)  param3MultNT  = obj_ss->getUniform("param3MultNT")!=0;
//			if(!param3AddNT)   param3AddNT   = obj_ss->getUniform("param3AddNT")!=0;
//			if(!param1MultNT2) param1MultNT2 = obj_ss->getUniform("param1MultNT2")!=0;
//			if(!param3MultNT2) param3MultNT2 = obj_ss->getUniform("param3MultNT2")!=0;
//			if(!param3AddNT2)  param3AddNT2  = obj_ss->getUniform("param3AddNT2")!=0;
//			if(!param1MultNT3) param1MultNT3 = obj_ss->getUniform("param1MultNT3")!=0;
//			if(!param3MultNT3) param3MultNT3 = obj_ss->getUniform("param3MultNT3")!=0;
//			if(!param3AddNT3)  param3AddNT3  = obj_ss->getUniform("param3AddNT3")!=0;
//
//			if(!param1MultDT)  param1MultDT  = obj_ss->getUniform("param1MultDT")!=0;
//			if(!param4MultDT)  param4MultDT  = obj_ss->getUniform("param4MultDT")!=0;
//			if(!param4AddDT)   param4AddDT   = obj_ss->getUniform("param4AddDT")!=0;
//			if(!param1MultDT2) param1MultDT2 = obj_ss->getUniform("param1MultDT2")!=0;
//			if(!param4MultDT2) param4MultDT2 = obj_ss->getUniform("param4MultDT2")!=0;
//			if(!param4AddDT2)  param4AddDT2  = obj_ss->getUniform("param4AddDT2")!=0;
//			if(!param1MultDT3) param1MultDT3 = obj_ss->getUniform("param1MultDT3")!=0;
//			if(!param4MultDT3) param4MultDT3 = obj_ss->getUniform("param4MultDT3")!=0;
//			if(!param4AddDT3)  param4AddDT3  = obj_ss->getUniform("param4AddDT3")!=0;
//
//			if(!param1MultST)  param1MultST  = obj_ss->getUniform("param1MultST")!=0;
//			if(!param4MultST)  param4MultST  = obj_ss->getUniform("param4MultST")!=0;
//			if(!param4AddST)   param4AddST   = obj_ss->getUniform("param4AddST")!=0;
//			if(!param1MultST2) param1MultST2 = obj_ss->getUniform("param1MultST2")!=0;
//			if(!param4MultST2) param4MultST2 = obj_ss->getUniform("param4MultST2")!=0;
//			if(!param4AddST2)  param4AddST2  = obj_ss->getUniform("param4AddST2")!=0;
//			if(!param1MultST3) param1MultST3 = obj_ss->getUniform("param1MultST3")!=0;
//			if(!param4MultST3) param4MultST3 = obj_ss->getUniform("param4MultST3")!=0;
//			if(!param4AddST3)  param4AddST3  = obj_ss->getUniform("param4AddST3")!=0;
//		}
//
//		switch(bump)
//		{
//		case 0:
//			break;
//		case 1:
//			m_getRtplmShaderStr += "_normal";
//			break;
//		case 2:
//			m_getRtplmShaderStr += "_parallax";
//			break;
//		}
//
//		if(hasEnvmap)
//		{
//			m_getRtplmShaderStr += "_env";
//		}
//		if(hasDiffuse)
//		{
//			m_getRtplmShaderStr += "_diffuse";
//		}
//		switch(OpDT)
//		{
//		case 0://Add,默认
//			break;
//		case 1:
//			m_getRtplmShaderStr += "_MULTDT";
//			break;
//		}
//		switch(specular)
//		{
//		case 0:
//			break;
//		case 1:
//			m_getRtplmShaderStr += "_specular3";
//			break;
//		case 2:
//			m_getRtplmShaderStr += "_specular4";
//			break;
//		}
//		if(baseMat)
//		{
//			m_getRtplmShaderStr += "_BM";
//		}
//		if(baseMatA)
//		{
//			m_getRtplmShaderStr += "_BMa";
//		}
//		if(normalMat)
//		{
//			m_getRtplmShaderStr += "_NM";
//		}
//		if(diffuseMat)
//		{
//			m_getRtplmShaderStr += "_DM";
//		}
//		if(specularMat)
//		{
//			m_getRtplmShaderStr += "_SM";
//		}
//		if(NT2)
//		{
//			m_getRtplmShaderStr += "_NT2";
//		}
//		if(normalMat2)
//		{
//			m_getRtplmShaderStr += "_NM2";
//		}
//		switch(OpNT2)
//		{
//		case 0:
//			m_getRtplmShaderStr += "_ADDNT2";
//			break;
//		case 1://Mult,默认
//			break;
//		}
//		if(NT3)
//		{
//			m_getRtplmShaderStr += "_NT3";
//		}
//		if(normalMat3)
//		{
//			m_getRtplmShaderStr += "_NM3";
//		}
//		switch(OpNT3)
//		{
//		case 0:
//			m_getRtplmShaderStr += "_ADDNT3";
//			break;
//		case 1://Mult,默认
//			break;
//		}
//		if(DT2)
//		{
//			m_getRtplmShaderStr += "_DT2";
//		}
//		if(diffuseMat2)
//		{
//			m_getRtplmShaderStr += "_DM2";
//		}
//		switch(OpDT2)
//		{
//		case 0://Add,默认
//			break;
//		case 1:
//			m_getRtplmShaderStr += "_MULTDT2";
//			break;
//		}	
//		if(DT3)
//		{
//			m_getRtplmShaderStr += "_DT3";
//		}
//		if(diffuseMat3)
//		{
//			m_getRtplmShaderStr += "_DM3";
//		}
//		switch(OpDT3)
//		{
//		case 0://Add,默认
//			break;
//		case 1:
//			m_getRtplmShaderStr += "_MULTDT3";
//			break;
//		}
//		if(ST2)
//		{
//			m_getRtplmShaderStr += "_ST2";
//		}
//		if(specularMat2)
//		{
//			m_getRtplmShaderStr += "_SM2";
//		}
//		switch(OpST2)
//		{
//		case 0://Add,默认
//			break;
//		case 1:
//			m_getRtplmShaderStr += "_MULTST2";
//			break;
//		}
//		if(ST3)
//		{
//			m_getRtplmShaderStr += "_ST3";
//		}
//		if(specularMat3)
//		{
//			m_getRtplmShaderStr += "_SM3";
//		}
//		switch(OpST3)
//		{
//		case 0://Add,默认
//			break;
//		case 1:
//			m_getRtplmShaderStr += "_MULTST3";
//			break;
//		}
//
//		if(param1MultENV)
//		{
//			m_getRtplmShaderStr += "_Param1MultENV";
//		}
//		if(param4MultENV)
//		{
//			m_getRtplmShaderStr += "_Param4MultENV";
//		}
//		if(param4AddENV)
//		{
//			m_getRtplmShaderStr += "_Param4AddENV";
//		}
//
//		if(param1MultBT)
//		{
//			m_getRtplmShaderStr += "_Param1MultBT";
//		}
//		if(param4MultBT)
//		{
//			m_getRtplmShaderStr += "_Param4MultBT";
//		}
//		if(param4AddBT)
//		{
//			m_getRtplmShaderStr += "_Param4AddBT";
//		}
//		if(param1MultBTa)
//		{
//			m_getRtplmShaderStr += "_Param1MultBTa";
//		}
//		if(param1AddBTa)
//		{
//			m_getRtplmShaderStr += "_Param1AddBTa";
//		}
//
//		if(param1MultNT)
//		{
//			m_getRtplmShaderStr += "_Param1MultNT";
//		}
//		if(param3MultNT)
//		{
//			m_getRtplmShaderStr += "_Param3MultNT";
//		}
//		if(param3AddNT)
//		{
//			m_getRtplmShaderStr += "_Param3AddNT";
//		}
//		if(param1MultNT2)
//		{
//			m_getRtplmShaderStr += "_Param1MultNT2";
//		}
//		if(param3MultNT2)
//		{
//			m_getRtplmShaderStr += "_Param3MultNT2";
//		}
//		if(param3AddNT2)
//		{
//			m_getRtplmShaderStr += "_Param3AddNT2";
//		}
//		if(param1MultNT3)
//		{
//			m_getRtplmShaderStr += "_Param1MultNT3";
//		}
//		if(param3MultNT3)
//		{
//			m_getRtplmShaderStr += "_Param3MultNT3";
//		}
//		if(param3AddNT3)
//		{
//			m_getRtplmShaderStr += "_Param3AddNT3";
//		}
//
//		if(param1MultDT)
//		{
//			m_getRtplmShaderStr += "_Param1MultDT";
//		}
//		if(param4MultDT)
//		{
//			m_getRtplmShaderStr += "_Param4MultDT";
//		}
//		if(param4AddDT)
//		{
//			m_getRtplmShaderStr += "_Param4AddDT";
//		}
//		if(param1MultDT2)
//		{
//			m_getRtplmShaderStr += "_Param1MultDT2";
//		}
//		if(param4MultDT2)
//		{
//			m_getRtplmShaderStr += "_Param4MultDT2";
//		}
//		if(param4AddDT2)
//		{
//			m_getRtglmShaderStr += "_Param4AddDT2";
//		}
//		if(param1MultDT3)
//		{
//			m_getRtplmShaderStr += "_Param1MultDT3";
//		}
//		if(param4MultDT3)
//		{
//			m_getRtplmShaderStr += "_Param4MultDT3";
//		}
//		if(param4AddDT3)
//		{
//			m_getRtplmShaderStr += "_Param4AddDT3";
//		}
//
//		if(param1MultST)
//		{
//			m_getRtplmShaderStr += "_Param1MultST";
//		}
//		if(param4MultST)
//		{
//			m_getRtplmShaderStr += "_Param4MultST";
//		}
//		if(param4AddST)
//		{
//			m_getRtplmShaderStr += "_Param4AddST";
//		}
//		if(param1MultST2)
//		{
//			m_getRtplmShaderStr += "_Param1MultST2";
//		}
//		if(param4MultST2)
//		{
//			m_getRtplmShaderStr += "_Param4MultST2";
//		}
//		if(param4AddST2)
//		{
//			m_getRtplmShaderStr += "_Param4AddST2";
//		}
//		if(param1MultST3)
//		{
//			m_getRtplmShaderStr += "_Param1MultST3";
//		}
//		if(param4MultST3)
//		{
//			m_getRtplmShaderStr += "_Param4MultST3";
//		}
//		if(param4AddST3)
//		{
//			m_getRtplmShaderStr += "_Param4AddST3";
//		}
//		m_getRtplmShaderStr += "_";
//	}
//	return m_getRtplmShaderStr; 
//}