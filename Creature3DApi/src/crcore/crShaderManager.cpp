/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/
#include <CRCore/crShaderManager.h>
#include <CRCore/crBoundingBox.h>
#include <CRCore/crLightSourceManager.h>
#include <CRCore/crUniform.h>
#include <CRCore/crBlendFunc.h>
#include <CRCore/crDepth.h>
#include <CRCore/crDisplaySettings.h>
#include <CRCore/crBrain.h>
#include <CRCore/crRenderToTextureManager.h>
using namespace CRCore;
//#define DISABLE_SHADER
ref_ptr<crShaderManager> crShaderManager::m_instance = NULL;

//////////////////////////////////////////////////////////////////
crShaderManager::crShaderManager()/*:
    m_defaultMaterial(new crMaterial)*/
{
	//m_sunShadowDistanceOffset = 2.0f;
	m_currentActiveStateSets = new StateSetBuf;
	m_currentActiveStateSets_buf = new StateSetBuf;
	m_currentReuseStateSetIndex = 0;//delete 20140421
#ifndef DISABLE_SHADER
	insertCallback("PhoneLight",new SunCallback);
	//insertCallback("AppendLight",new NeedLightCallback);
    //insertCallback("morepass",new LightCallback);
	//insertCallback("water",new WaterCallback);
	insertCallback("NoLight",new NoLightCallback);
#endif
}

crShaderManager::~crShaderManager() 
{
	delete m_currentActiveStateSets;
	delete m_currentActiveStateSets_buf;
}

crShaderManager* crShaderManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crShaderManager;
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}

void crShaderManager::clear()
{//shaderManager 不需要重新载入
	//m_instance = NULL;
	//CRCore::ScopedLock<crMutex> lock(m_mutex);
	//m_sssmapbuf.clear();
	//for( ShaderStateSetMap::iterator itr = m_shaderStateSetMap.begin();
	//	 itr != m_shaderStateSetMap.end();
	//	 ++itr )
	//{
	//	itr->second->releaseObjects();
	//}
	//m_buf.clear();//getOrCreateIdleStateSet
	//m_currentReuseStateSetIndex = 0;
	//m_shadowStateSetMap.clear();
}
void crShaderManager::setGIParam(const crVector4& giparam)
{
	m_giparam = giparam;
}
void crShaderManager::setGITexture(crTexture *tex)
{
	m_giTexture = tex;
}
void crShaderManager::setLightMapTexture(crTexture *tex)
{
	m_lightMapTexture = tex;
}
void crShaderManager::setHeightMapTexture(crTexture *tex)
{
	m_heightMapTexture = tex;
}
crShaderManager::ShaderStateSetMap &crShaderManager::getShaderStateSetMap()
{
	return m_shaderStateSetMap;
}

void crShaderManager::releaseObjects(crState* state)
{
	//CRCore::ScopedLock<crMutex> lock(m_mutex);
	for( ShaderStateSetMap::iterator itr = m_shaderStateSetMap.begin();
		 itr != m_shaderStateSetMap.end();
		 ++itr )
	{
		itr->second->releaseObjects(state);
	}

	m_sssmapbuf.clear();
	
	m_currentActiveStateSets->clear();
	m_currentActiveStateSets_buf->clear();

	//delete 20140421
	for( StateSetBuf::iterator itr = m_buf.begin();
		itr != m_buf.end();
		++itr )
	{
		(*itr)->releaseObjects(state);
	}
    m_buf.clear();//getOrCreateIdleStateSet
    m_currentReuseStateSetIndex = 0;
	m_shadowStateSetMap.clear();
	if(m_giTexture.valid())
	{
		m_giTexture->releaseObjects(state);
		m_giTexture = NULL;
	}
	if(m_lightMapTexture.valid())
	{
		m_lightMapTexture->releaseObjects(state);
		m_lightMapTexture = NULL;
	}
	if(m_heightMapTexture.valid())
	{
		m_heightMapTexture->releaseObjects(state);
		m_heightMapTexture = NULL;
	}
}

void crShaderManager::setSunShadowDistanceOffset( const crVector2 &offset )
{
    m_sunShadowDistanceOffset = offset;
}

void crShaderManager::setSunShadowDistance( const crVector2 &sunShadowDistance )
{
	m_sunShadowDistance = sunShadowDistance;
}

void crShaderManager::insertShaderStateSet( crStateSet *stateset )
{
	if(stateset)
	{
		if(stateset->getName().empty())
		{
			CRCore::notify(CRCore::WARN)<<"crShaderManager::insertShaderStateSet(): 材质没有被命名"<<std::endl;
			return;
		}
		if(m_shaderStateSetMap.find(stateset->getName()) != m_shaderStateSetMap.end())
		{
			CRCore::notify(CRCore::WARN)<<"crShaderManager::insertShaderStateSet(): 同名材质已经存在"<<std::endl;
			return;
		}
		for( ShaderStateSetMap::iterator itr = m_shaderStateSetMap.begin();
			 itr != m_shaderStateSetMap.end();
			 itr++ )
		{
			if(itr->second->compare(*stateset,true) == 0)
			{
				CRCore::notify(CRCore::WARN)<<"crShaderManager::insertShaderStateSet(): 属性相同材质已经存在，已存在的材质名为："<<itr->first<<std::endl;
				return;
			}
		}
	    m_shaderStateSetMap[stateset->getName()] = stateset;
	}
}

void crShaderManager::insertCallback(const std::string &ts, crShaderManager::Callback *callback)
{
	m_shaderCallbackMap[ts] = callback;
}

void crShaderManager::registAlphaFunc(crAlphaFunc *alphaFunc)
{
    m_alphaFunc = alphaFunc;
}

void crShaderManager::registBlendFunc(crBlendFunc *blendFunc)
{
    m_blendFunc = blendFunc;
}

bool crShaderManager::getShadowShaderStateSetVec(const crObject *obj, crDrawable* drawable, crShaderManager::StateSetVec &statesetVec )
{
	crStateSet *drawable_ss = drawable->getStateSet();
	if(drawable_ss == NULL)
		drawable_ss = const_cast<crStateSet *>(obj->getStateSet());
	if(drawable_ss == NULL)
	{
		//CRCore::notify(CRCore::FATAL)<<"crShaderManager::getShadowShaderStateSetVec() not set stateset nodeid = "<<obj->getName()<<std::endl;
		return false;
	}

	std::string str;
	ref_ptr<crStateSet> shader_ss;
	ref_ptr<crStateSet> uniform_ss;

	if(crDisplaySettings::instance()->getEnableAlphaShadow()/*显卡支持FBOBLEND*/)
	{
		crTexture *base = dynamic_cast<crTexture*>(drawable_ss->getTextureAttribute(TEXTURE_BASEMAP,crStateAttribute::TEXTURE));
		if(base)
		{
            bool alphaShadow = base->getAlphaShadowEnable();
			bool blendShadow = base->getBlendShadowEnable();
			if(alphaShadow || blendShadow)
			{
				std::string name;
				crStateSet *obj_ss = const_cast<crStateSet *>(obj->getStateSet());
				ShadowStateSetMap::iterator itr = m_shadowStateSetMap.find(ShadowStateSetPair(obj_ss,drawable_ss));
				if(itr!=m_shadowStateSetMap.end())
				{
                    uniform_ss = itr->second.first.get();
					shader_ss = itr->second.second.get();
				}
				else
				{
					uniform_ss = getOrCreateIdleStateSet();
                    //uniform_ss = new crStateSet;
					uniform_ss->setStateSetMode(crStateSet::SS_ShadowMapRender);
					//uniform_ss->setTextureAttributeAndModes(TEXTURE_BASEMAP,base,crStateAttribute::ON);
					std::string statesetShaderStr;
					statesetShaderStr = drawable_ss->getRtglmShaderStr(obj_ss);

					str = "shadow";
					if(alphaShadow)
					{
						str += "_AlphaShadow";
					}
					if(blendShadow)
					{
						str += "_BlendShadow";
					}
					crAlphaFunc *alphaFunc = dynamic_cast<crAlphaFunc *>(drawable_ss->getAttribute(crStateAttribute::ALPHAFUNC));
					if(alphaFunc)
						uniform_ss->setAttributeAndModes(alphaFunc,crStateAttribute::ON);
					crBlendFunc *blend = dynamic_cast<crBlendFunc *>(drawable_ss->getAttribute(crStateAttribute::BLENDFUNC));
					if(blend)
						uniform_ss->setAttributeAndModes(blend,crStateAttribute::ON);
					uniform_ss->setMode(GL_CULL_FACE,drawable_ss->getMode(GL_CULL_FACE));

					std::string name = getShadowName(statesetShaderStr,uniform_ss.get(),drawable_ss,obj_ss);
					str += name;
					shader_ss = getShaderStateSet(str);
					m_shadowStateSetMap[ShadowStateSetPair(obj_ss,drawable_ss)] = std::make_pair(uniform_ss,shader_ss);
					//if(uniform_ss.valid()) pushActiveStateSet(uniform_ss.get());
					//statesetVec.push_back(std::make_pair(shader_ss,uniform_ss.get()));
				}

				//str += name;
				//shader_ss = getShaderStateSet(str);
				//if(uniform_ss.valid()) crShaderManager::getInstance()->pushActiveStateSet(uniform_ss.get());
				statesetVec.push_back(std::make_pair(shader_ss,uniform_ss));
				return true;
			}
		}
	}

	str = "shadow";
	shader_ss = getShaderStateSet(str);
	statesetVec.push_back(std::make_pair(shader_ss,uniform_ss));
	return true;

	//crTexture *base = dynamic_cast<crTexture*>(drawable_ss->getTextureAttribute(TEXTURE_BASEMAP,crStateAttribute::TEXTURE));
	//if(base)
	//{
	//	//CRCore::notify(CRCore::WARN)<<"crShaderManager::getShadowShaderStateSetVec()：AlphaShadow"<<std::endl;
	//    if(base->getAlphaShadowEnable())
	//	{
	//		if(crDisplaySettings::instance()->getEnableAlphaShadow()/*显卡支持FBOBLEND*/)
	//		{
	//			uniform_ss = getOrCreateIdleStateSet();
	//			uniform_ss->setIsShadowStateSet(true);
	//			////////////ExternTexture
	//			crTexture2D *externTex2D = dynamic_cast<crTexture2D *>(drawable_ss->getTextureAttribute(TEXTURE_UVSCRAMBLER,crStateAttribute::TEXTURE));
	//			crTexture::ExternTextureID externTextureID = crTexture::EXT_NONE;
	//			if(externTex2D)
	//			{
	//				externTextureID = externTex2D->getExternTextureID();
	//			}
	//			if(externTextureID == crTexture::UVSCRAMBLER)
	//			{
	//			    str = "AlphaUVSShadow";
	//				uniform_ss->setTextureAttributeAndModes(TEXTURE_UVSCRAMBLER,externTex2D,crStateAttribute::ON);
	//				crUniform *uniform = drawable_ss->getUniform("param1MultUVS");
	//				if(uniform)
	//					uniform_ss->addUniform(uniform);
	//				else
	//				{
	//					uniform = uniform_ss->getOrCreateUniform("param1MultUVS",crUniform::FLOAT);
	//					uniform->setDataVariance(crBase::STATIC);
	//					uniform->set(1.0f);
	//				}
	//				crStateSet *obj_ss = const_cast<crStateSet *>(obj->getStateSet());
	//				if(obj_ss)
	//				    uniform = obj_ss->getUniform("UVSMat");
	//				if(obj_ss && uniform)
	//					uniform_ss->addUniform(uniform);
	//				else
	//				{
	//					uniform = uniform_ss->getOrCreateUniform("UVSMat",crUniform::FLOAT_MAT4);
	//					uniform->setDataVariance(crBase::STATIC);
	//					uniform->set(crMatrixf::identity());
	//				}
	//			}
	//			else
	//			    str = "AlphaShadow";

	//			uniform_ss->setTextureAttributeAndModes(TEXTURE_BASEMAP,base,crStateAttribute::ON);
	//			uniform_ss->setAttributeAndModes(m_alphaShadowFunc.get(),crStateAttribute::ON);
	//			uniform_ss->setIsShadowStateSet(true);
	//			
	//			uniform_ss->setMode(GL_CULL_FACE,drawable_ss->getMode(GL_CULL_FACE));
	//			//uniform_ss->setRenderingHint(crStateSet::TRANSPARENT_BIN);

	//			//StateSetBuf::iterator itr = m_currentActiveStateSets->find(uniform_ss.get());
	//			//if(itr != m_currentActiveStateSets->end())
	//			//	uniform_ss = itr->get();
	//			//else
	//			//	m_currentActiveStateSets->insert(uniform_ss.get());
	//			if(uniform_ss.valid()) pushActiveStateSet(uniform_ss.get());
	//		}
	//		else return false;
	//	}
	//	else if(base->getBlendShadowEnable())
	//	{
	//		str = "BlendShadow";
	//		uniform_ss = getOrCreateIdleStateSet();
	//		uniform_ss->setTextureAttributeAndModes(TEXTURE_BASEMAP,base,crStateAttribute::ON);
	//		uniform_ss->setIsShadowStateSet(true);

	//		uniform_ss->setMode(GL_CULL_FACE,drawable_ss->getMode(GL_CULL_FACE));
	//		crUniform *uniform = drawable_ss->getUniform("param1MultBTa");
	//		if(uniform)
 //               uniform_ss->addUniform(uniform);
	//		else
	//		{
	//			uniform = uniform_ss->getOrCreateUniform("param1MultBTa",crUniform::FLOAT);
	//			uniform->setDataVariance(crBase::STATIC);
	//			uniform->set(1.0f);
	//		}
	//		uniform = drawable_ss->getUniform("param1AddBTa");
	//		if(uniform)
	//			uniform_ss->addUniform(uniform);
	//		else
	//		{
	//			uniform = uniform_ss->getOrCreateUniform("param1AddBTa",crUniform::FLOAT);
	//			uniform->setDataVariance(crBase::STATIC);
	//			uniform->set(0.0f);
	//		}

	//		//uniform_ss->setRenderingHint(crStateSet::TRANSPARENT_BIN);

	//		//StateSetBuf::iterator itr = m_currentActiveStateSets->find(uniform_ss.get());
	//		//if(itr != m_currentActiveStateSets->end())
	//		//	uniform_ss = itr->get();
	//		//else
	//		//	m_currentActiveStateSets->insert(uniform_ss.get());
	//		if(uniform_ss.valid()) pushActiveStateSet(uniform_ss.get());
	//	}
	//}
	//if(str.empty())
	//    str = "shadow";

	//shader_ss = getShaderStateSet(str);
 //   
	//statesetVec.push_back(std::make_pair(shader_ss,uniform_ss.get()));

	//return true;
}

void crShaderManager::pushString(const std::string &str)
{
    m_stringVec.push_back(str);
}
bool parseLightNum(std::string str, const char *key, int &value )
{
	int i = str.find(key);
	if(i==std::string::npos) return false;
	int keylen = strlen(key);
	str.erase(0,i+keylen);
	i = str.find('_');
	if(i!=std::string::npos)
		str.erase(i,str.size());
	value = atoi(str.c_str());

	return true;
}
std::string crShaderManager::getLightHead(const std::string &name, int mode)
{
	std::string str;
	if(mode!=2)
	{
		int d = 0;
		int s = 0;
		parseLightNum(name,"direct",d);
		parseLightNum(name,"spot",s);
		char buf[8];
		if(d > 0)
		{
			sprintf(buf,"direct%d\0",d);
			str = "#define ";
			str +=  buf;
			str += '\n';

			if(s>0)
			{
				sprintf(buf,"spot%d\0",s);
				str += "#define ";
				str += buf;
				str += '\n';
			}
		}
		else if(s > 0)
		{
			sprintf(buf,"spot%d\0",s);
			str = "#define ";
			str +=  buf;
			str += '\n';
		}
		if(mode == 0 && name.find("sunx")==std::string::npos)
		{
			str += "#define sun\n";
		}
		if(name.find("_rts_")!=std::string::npos)
		{
			str += "#define _rts\n";
			if(name.find("_rts2_")!=std::string::npos)
			{
				str += "#define _rts2\n";
			}
			if(crDisplaySettings::instance()->getShadowDetail()>=3)
				str += "#define _hrts\n";
			else
				str += "#define _lrts\n";
		}
	}
	if(name.find("_gi_")!=std::string::npos)
	{
		str += "#define _gi\n";
	}
	if(name.find("_sgi_")!=std::string::npos)
	{
		str += "#define _sgi\n";
	}
	if(name.find("_hgi_")!=std::string::npos)
	{
		str += "#define _hgi\n";
	}
	if(name.find("_fi_")!=std::string::npos)
	{
		str += "#define _fi\n";
	}
	if(name.find("_fo_")!=std::string::npos)
	{
		str += "#define _fo\n";
	}
	if(name.find("_NoSkyLight_")!=std::string::npos)
	{
		str += "#define _NoSkyLight\n";
	}
	////////////Other
	if(name.find("_lm_")!=std::string::npos)
	{
		str += "#define _lm\n";
		if(name.find("_LM_")!=std::string::npos)
		{
			str += "#define _LM\n";
		}
	}
	if(name.find("_lm2_")!=std::string::npos)
	{
		str += "#define _lm2\n";
		if(name.find("_LM2_")!=std::string::npos)
		{
			str += "#define _LM2\n";
		}
	}
	if(name.find("_env_")!=std::string::npos)
	{
		str += "#define _env\n";

		if(name.find("_Param1MultENV_")!=std::string::npos)
		{
			str += "#define _Param1MultENV\n";
		}
		if(name.find("_Param4MultENV_")!=std::string::npos)
		{
			str += "#define _Param4MultENV\n";
		}
		if(name.find("_Param4AddENV_")!=std::string::npos)
		{
			str += "#define _Param4AddENV\n";
		}
	}
	if (name.find("_gimap_") != std::string::npos)
	{
		str += "#define _gimap\n";
	}
	/////////////////////NT
	if(mode!=2)
	{
		bool hasNormal = false;
		if(name.find("_NT_")!=std::string::npos)
		{
			str += "#define _NT\n";
			hasNormal = true;
		}
		else if(name.find("_PT_")!=std::string::npos)
		{
			str += "#define _PT\n";
			hasNormal = true;
		}
		if(hasNormal)
		{
			if(name.find("_NM_")!=std::string::npos)
			{
				str += "#define _NM\n";
			}

			if(name.find("_Param1MultNT_")!=std::string::npos)
			{
				str += "#define _Param1MultNT\n";
			}
			if(name.find("_Param3MultNT_")!=std::string::npos)
			{
				str += "#define _Param3MultNT\n";
			}
			if(name.find("_Param3AddNT_")!=std::string::npos)
			{
				str += "#define _Param3AddNT\n";
			}
		}
		if(name.find("_NT2_")!=std::string::npos)
		{
			str += "#define _NT2\n";
			if(name.find("_NT2UV1_")!=std::string::npos)
			{
				str += "#define _NT2UV1\n";
			}
			if(name.find("_NM2_")!=std::string::npos)
			{
				str += "#define _NM2\n";
			}

			if(name.find("_REPLACENT2_")!=std::string::npos)
			{
				str += "#define _REPLACENT2\n";
			}
			else if(name.find("_ADDNT2_")!=std::string::npos)
			{
				str += "#define _ADDNT2\n";
			}
			else if(name.find("_MULTNT2_")!=std::string::npos)
			{
				str += "#define _MULTNT2\n";
			}

			if(name.find("_Param1MultNT2_")!=std::string::npos)
			{
				str += "#define _Param1MultNT2\n";
			}
			if(name.find("_Param3MultNT2_")!=std::string::npos)
			{
				str += "#define _Param3MultNT2\n";
			}
			if(name.find("_Param3AddNT2_")!=std::string::npos)
			{
				str += "#define _Param3AddNT2\n";
			}
		}

		if(name.find("_NT3_")!=std::string::npos)
		{
			str += "#define _NT3\n";
			if(name.find("_NT3UV1_")!=std::string::npos)
			{
				str += "#define _NT3UV1\n";
			}
			if(name.find("_NM3_")!=std::string::npos)
			{
				str += "#define _NM3\n";
			}

			if(name.find("_REPLACENT3_")!=std::string::npos)
			{
				str += "#define _REPLACENT3\n";
			}
			else if(name.find("_ADDNT3_")!=std::string::npos)
			{
				str += "#define _ADDNT3\n";
			}
			else if(name.find("_MULTNT3_")!=std::string::npos)
			{
				str += "#define _MULTNT3\n";
			}

			if(name.find("_Param1MultNT3_")!=std::string::npos)
			{
				str += "#define _Param1MultNT3\n";
			}
			if(name.find("_Param3MultNT3_")!=std::string::npos)
			{
				str += "#define _Param3MultNT3\n";
			}
			if(name.find("_Param3AddNT3_")!=std::string::npos)
			{
				str += "#define _Param3AddNT3\n";
			}
		}
	}
	///////////////ET
	bool addETTo = false;
	bool addET2To = false;
	bool addET3To = false;
	bool hasET = false;
	bool hasET2 = false;
	bool hasET3 = false;
	if(name.find("_ADDETTON_")!=std::string::npos)
	{
		str += "#define _ADDETTON\n";
		addETTo = true;
	}
	else if(name.find("_MULTETTON_")!=std::string::npos)
	{
		str += "#define _MULTETTON\n";
		addETTo = true;
	}
	if(name.find("_ADDET2TOD_")!=std::string::npos)
	{
		str += "#define _ADDET2TOD\n";
		addET2To = true;
	}
	else if(name.find("_MULTET2TOD_")!=std::string::npos)
	{
		str += "#define _MULTET2TOD\n";
		addET2To = true;
	}
	if(name.find("_ADDET3TOS_")!=std::string::npos)
	{
		str += "#define _ADDET3TOS\n";
		addET3To = true;
	}
	else if(name.find("_MULTET3TOS_")!=std::string::npos)
	{
		str += "#define _MULTET3TOS\n";
		addET3To = true;
	}
	if(addETTo || mode!=1)
	{
		if(name.find("_ET_")!=std::string::npos)
		{
			hasET = true;
			str += "#define _ET\n";
			if(name.find("_ETUV1_")!=std::string::npos)
			{
				str += "#define _ETUV1\n";
			}
			if(name.find("_EM_")!=std::string::npos)
			{
				str += "#define _EM\n";
			}
			if(name.find("_Param1MultET_")!=std::string::npos)
			{
				str += "#define _Param1MultET\n";
			}
			if(name.find("_Param4MultET_")!=std::string::npos)
			{
				str += "#define _Param4MultET\n";
			}
			if(name.find("_Param4AddET_")!=std::string::npos)
			{
				str += "#define _Param4AddET\n";
			}
		}
	}
	if(addET2To || mode!=1)
	{
		if(name.find("_ET2_")!=std::string::npos)
		{
			hasET2 = true;
			str += "#define _ET2\n";
			if(name.find("_ET2UV1_")!=std::string::npos)
			{
				str += "#define _ET2UV1\n";
			}
			if(name.find("_EM2_")!=std::string::npos)
			{
				str += "#define _EM2\n";
			}
			if(name.find("_Param1MultET2_")!=std::string::npos)
			{
				str += "#define _Param1MultET2\n";
			}
			if(name.find("_Param4MultET2")!=std::string::npos)
			{
				str += "#define _Param4MultET2\n";
			}
			if(name.find("_Param4AddET2_")!=std::string::npos)
			{
				str += "#define _Param4AddET2\n";
			}
		}
	}
	if(addET3To || mode!=1)
	{
		if(name.find("_ET3_")!=std::string::npos)
		{
			hasET3 = true;
			str += "#define _ET3\n";
			if(name.find("_ET3UV1_")!=std::string::npos)
			{
				str += "#define _ET3UV1\n";
			}
			if(name.find("_EM3_")!=std::string::npos)
			{
				str += "#define _EM3\n";
			}
			if(name.find("_Param1MultET3_")!=std::string::npos)
			{
				str += "#define _Param1MultET3\n";
			}
			if(name.find("_Param4MultET3_")!=std::string::npos)
			{
				str += "#define _Param4MultET3\n";
			}
			if(name.find("_Param4AddET3_")!=std::string::npos)
			{
				str += "#define _Param4AddET3\n";
			}
		}
	}
	if(mode!=1)
	{
		if(hasET)
		{
			if(name.find("_EMa_")!=std::string::npos)
			{
				str += "#define _EMa\n";
			}

			if(!addETTo)
			{
				if(name.find("_REPLACEET_")!=std::string::npos)
				{
					str += "#define _REPLACEET\n";
				}
				else if(name.find("_ADDET_")!=std::string::npos)
				{
					str += "#define _ADDET\n";
				}
				else if(name.find("_MULTET_")!=std::string::npos)
				{
					str += "#define _MULTET\n";
				}
			}

			if(name.find("_REPLACEETa_")!=std::string::npos)
			{
				str += "#define _REPLACEETa\n";
			}
			else if(name.find("_ADDETa_")!=std::string::npos)
			{
				str += "#define _ADDETa\n";
			}
			else if(name.find("_MULTETa_")!=std::string::npos)
			{
				str += "#define _MULTETa\n";
			}

			if(name.find("_Param1MultETa_")!=std::string::npos)
			{
				str += "#define _Param1MultETa\n";
			}
			if(name.find("_Param1AddETa_")!=std::string::npos)
			{
				str += "#define _Param1AddETa\n";
			}
		}
        if(hasET2)
		{
			if(name.find("_EM2a_")!=std::string::npos)
			{
				str += "#define _EM2a\n";
			}
           
			if(!addET2To)
			{
				if(name.find("_REPLACEET2_")!=std::string::npos)
				{
					str += "#define _REPLACEET2\n";
				}
				else if(name.find("_ADDET2_")!=std::string::npos)
				{
					str += "#define _ADDET2\n";
				}
				else if(name.find("_MULTET2_")!=std::string::npos)
				{
					str += "#define _MULTET2\n";
				}
			}

			if(name.find("_REPLACEET2a_")!=std::string::npos)
			{
				str += "#define _REPLACEET2a\n";
			}
			else if(name.find("_ADDET2a_")!=std::string::npos)
			{
				str += "#define _ADDET2a\n";
			}
			else if(name.find("_MULTET2a_")!=std::string::npos)
			{
				str += "#define _MULTET2a\n";
			}

			if(name.find("_Param1MultET2a_")!=std::string::npos)
			{
				str += "#define _Param1MultET2a\n";
			}
			if(name.find("_Param1AddET2a_")!=std::string::npos)
			{
				str += "#define _Param1AddET2a\n";
			}
		}
        if(hasET3)
		{
			if(name.find("_EM3a_")!=std::string::npos)
			{
				str += "#define _EM3a\n";
			}

			if(!addET3To)
			{
				if(name.find("_REPLACEET3_")!=std::string::npos)
				{
					str += "#define _REPLACEET3\n";
				}
				else if(name.find("_ADDET3_")!=std::string::npos)
				{
					str += "#define _ADDET3\n";
				}
				else if(name.find("_MULTET3_")!=std::string::npos)
				{
					str += "#define _MULTET3\n";
				}
			}

			if(name.find("_REPLACEET3a_")!=std::string::npos)
			{
				str += "#define _REPLACEET3a\n";
			}
			else if(name.find("_ADDET3a_")!=std::string::npos)
			{
				str += "#define _ADDET3a\n";
			}
			else if(name.find("_MULTET3a_")!=std::string::npos)
			{
				str += "#define _MULTET3a\n";
			}

			if(name.find("_Param1MultET3a_")!=std::string::npos)
			{
				str += "#define _Param1MultET3a\n";
			}
			if(name.find("_Param1AddET3a_")!=std::string::npos)
			{
				str += "#define _Param1AddET3a\n";
			}
		}
	}
    /////////////////////////////////////

	/////////////DT
	if(name.find("_BT_")!=std::string::npos)
	{
		str += "#define _BT\n";
		if(name.find("_BTUV1_")!=std::string::npos)
		{
			str += "#define _BTUV1\n";
		}
		if(name.find("_BM_")!=std::string::npos)
		{
			str += "#define _BM\n";
		}
		if(name.find("_BMa_")!=std::string::npos)
		{
			str += "#define _BMa\n";
		}
		if(name.find("_REPLACEBT_")!=std::string::npos)
		{
			str += "#define _REPLACEBT\n";
		}
		else if(name.find("_ADDBT_")!=std::string::npos)
		{
			str += "#define _ADDBT\n";
		}
		else if(name.find("_MULTBT_")!=std::string::npos)
		{
			str += "#define _MULTBT\n";
		}
		if(name.find("_REPLACEBTa_")!=std::string::npos)
		{
			str += "#define _REPLACEBTa\n";
		}
		else if(name.find("_ADDBTa_")!=std::string::npos)
		{
			str += "#define _ADDBTa\n";
		}
		else if(name.find("_MULTBTa_")!=std::string::npos)
		{
			str += "#define _MULTBTa\n";
		}
		if(name.find("_Param1MultBT_")!=std::string::npos)
		{
			str += "#define _Param1MultBT\n";
		}
		if(name.find("_Param4MultBT_")!=std::string::npos)
		{
			str += "#define _Param4MultBT\n";
		}
		if(name.find("_Param4AddBT_")!=std::string::npos)
		{
			str += "#define _Param4AddBT\n";
		}
		if(name.find("_Param1MultBTa_")!=std::string::npos)
		{
			str += "#define _Param1MultBTa\n";
		}
		if(name.find("_Param1AddBTa_")!=std::string::npos)
		{
			str += "#define _Param1AddBTa\n";
		}
	}

	if(name.find("_DT_")!=std::string::npos)
	{
		str += "#define _DT\n";
		if(name.find("_DTUV1_")!=std::string::npos)
		{
			str += "#define _DTUV1\n";
		}
		if(name.find("_DM_")!=std::string::npos)
		{
			str += "#define _DM\n";
		}
		if(name.find("_DMa_")!=std::string::npos)
		{
			str += "#define _DMa\n";
		}

		if(name.find("_REPLACEDT_")!=std::string::npos)
		{
			str += "#define _REPLACEDT\n";
		}
		else if(name.find("_ADDDT_")!=std::string::npos)
		{
			str += "#define _ADDDT\n";
		}
		else if(name.find("_MULTDT_")!=std::string::npos)
		{
			str += "#define _MULTDT\n";
		}

		if(name.find("_REPLACEDTa_")!=std::string::npos)
		{
			str += "#define _REPLACEDTa\n";
		}
		else if(name.find("_ADDDTa_")!=std::string::npos)
		{
			str += "#define _ADDDTa\n";
		}
		else if(name.find("_MULTDTa_")!=std::string::npos)
		{
			str += "#define _MULTDTa\n";
		}

		if(name.find("_Param1MultDT_")!=std::string::npos)
		{
			str += "#define _Param1MultDT\n";
		}
		if(name.find("_Param4MultDT_")!=std::string::npos)
		{
			str += "#define _Param4MultDT\n";
		}
		if(name.find("_Param4AddDT_")!=std::string::npos)
		{
			str += "#define _Param4AddDT\n";
		}
		if(name.find("_Param1MultDTa_")!=std::string::npos)
		{
			str += "#define _Param1MultDTa\n";
		}
		if(name.find("_Param1AddDTa_")!=std::string::npos)
		{
			str += "#define _Param1AddDTa\n";
		}
	}
	if(name.find("_DT2_")!=std::string::npos)
	{
		str += "#define _DT2\n";
		if(name.find("_DT2UV1_")!=std::string::npos)
		{
			str += "#define _DT2UV1\n";
		}
		if(name.find("_DM2_")!=std::string::npos)
		{
			str += "#define _DM2\n";
		}
		if(name.find("_DM2a_")!=std::string::npos)
		{
			str += "#define _DM2a\n";
		}

		if(name.find("_REPLACEDT2_")!=std::string::npos)
		{
			str += "#define _REPLACEDT2\n";
		}
		else if(name.find("_ADDDT2_")!=std::string::npos)
		{
			str += "#define _ADDDT2\n";
		}
		else if(name.find("_MULTDT2_")!=std::string::npos)
		{
			str += "#define _MULTDT2\n";
		}

		if(name.find("_REPLACEDT2a_")!=std::string::npos)
		{
			str += "#define _REPLACEDT2a\n";
		}
		else if(name.find("_ADDDT2a_")!=std::string::npos)
		{
			str += "#define _ADDDT2a\n";
		}
		else if(name.find("_MULTDT2a_")!=std::string::npos)
		{
			str += "#define _MULTDT2a\n";
		}

		if(name.find("_Param1MultDT2_")!=std::string::npos)
		{
			str += "#define _Param1MultDT2\n";
		}
		if(name.find("_Param4MultDT2_")!=std::string::npos)
		{
			str += "#define _Param4MultDT2\n";
		}
		if(name.find("_Param4AddDT2_")!=std::string::npos)
		{
			str += "#define _Param4AddDT2\n";
		}
		if(name.find("_Param1MultDT2a_")!=std::string::npos)
		{
			str += "#define _Param1MultDT2a\n";
		}
		if(name.find("_Param1AddDT2a_")!=std::string::npos)
		{
			str += "#define _Param1AddDT2a\n";
		}
	}
	/////////////ST
	if(mode!=2)
	{
		if(name.find("_ST_")!=std::string::npos)
		{
			str += "#define _ST\n";
			if(name.find("_STUV1_")!=std::string::npos)
			{
				str += "#define _STUV1\n";
			}
			if(name.find("_SM_")!=std::string::npos)
			{
				str += "#define _SM\n";
			}
			if(name.find("_SMa_")!=std::string::npos)
			{
				str += "#define _SMa\n";
			}

			if(name.find("_REPLACEST_")!=std::string::npos)
			{
				str += "#define _REPLACEST\n";
			}
			else if(name.find("_ADDST_")!=std::string::npos)
			{
				str += "#define _ADDST\n";
			}
			else if(name.find("_MULTST_")!=std::string::npos)
			{
				str += "#define _MULTST\n";
			}

			if(name.find("_REPLACESTa_")!=std::string::npos)
			{
				str += "#define _REPLACESTa\n";
			}
			else if(name.find("_ADDSTa_")!=std::string::npos)
			{
				str += "#define _ADDSTa\n";
			}
			else if(name.find("_MULTSTa_")!=std::string::npos)
			{
				str += "#define _MULTSTa\n";
			}

			if(name.find("_Param1MultST_")!=std::string::npos)
			{
				str += "#define _Param1MultST\n";
			}
			if(name.find("_Param4MultST_")!=std::string::npos)
			{
				str += "#define _Param4MultST\n";
			}
			if(name.find("_Param4AddST_")!=std::string::npos)
			{
				str += "#define _Param4AddST\n";
			}

			if(name.find("_Param1MultSTa_")!=std::string::npos)
			{
				str += "#define _Param1MultSTa\n";
			}
			if(name.find("_Param1AddSTa_")!=std::string::npos)
			{
				str += "#define _Param1AddSTa\n";
			}
		}

		if(name.find("_ST2_")!=std::string::npos)
		{
			str += "#define _ST2\n";
			if(name.find("_ST2UV1_")!=std::string::npos)
			{
				str += "#define _ST2UV1\n";
			}
			if(name.find("_SM2_")!=std::string::npos)
			{
				str += "#define _SM2\n";
			}
			if(name.find("_SM2a_")!=std::string::npos)
			{
				str += "#define _SM2a\n";
			}

			if(name.find("_REPLACEST2_")!=std::string::npos)
			{
				str += "#define _REPLACEST2\n";
			}
			else if(name.find("_ADDST2_")!=std::string::npos)
			{
				str += "#define _ADDST2\n";
			}
			else if(name.find("_MULTST2_")!=std::string::npos)
			{
				str += "#define _MULTST2\n";
			}

			if(name.find("_REPLACEST2a_")!=std::string::npos)
			{
				str += "#define _REPLACEST2a\n";
			}
			else if(name.find("_ADDST2a_")!=std::string::npos)
			{
				str += "#define _ADDST2a\n";
			}
			else if(name.find("_MULTST2a_")!=std::string::npos)
			{
				str += "#define _MULTST2a\n";
			}

			if(name.find("_Param1MultST2_")!=std::string::npos)
			{
				str += "#define _Param1MultST2\n";
			}
			if(name.find("_Param4MultST2_")!=std::string::npos)
			{
				str += "#define _Param4MultST2\n";
			}
			if(name.find("_Param4AddST2_")!=std::string::npos)
			{
				str += "#define _Param4AddST2\n";
			}

			if(name.find("_Param1MultST2a_")!=std::string::npos)
			{
				str += "#define _Param1MultST2a\n";
			}
			if(name.find("_Param1AddST2a_")!=std::string::npos)
			{
				str += "#define _Param1AddST2a\n";
			}
		}

		if(name.find("_ST3_")!=std::string::npos)
		{
			str += "#define _ST3\n";
			if(name.find("_ST3UV1_")!=std::string::npos)
			{
				str += "#define _ST3UV1\n";
			}
			if(name.find("_SM3_")!=std::string::npos)
			{
				str += "#define _SM3\n";
			}
			if(name.find("_SM3a_")!=std::string::npos)
			{
				str += "#define _SM3a\n";
			}

			if(name.find("_REPLACEST3_")!=std::string::npos)
			{
				str += "#define _REPLACEST3\n";
			}
			else if(name.find("_ADDST3_")!=std::string::npos)
			{
				str += "#define _ADDST3\n";
			}
			else if(name.find("_MULTST3_")!=std::string::npos)
			{
				str += "#define _MULTST3\n";
			}

			if(name.find("_REPLACEST3a_")!=std::string::npos)
			{
				str += "#define _REPLACEST3a\n";
			}
			else if(name.find("_ADDST3a_")!=std::string::npos)
			{
				str += "#define _ADDST3a\n";
			}
			else if(name.find("_MULTST3a_")!=std::string::npos)
			{
				str += "#define _MULTST3a\n";
			}

			if(name.find("_Param1MultST3_")!=std::string::npos)
			{
				str += "#define _Param1MultST3\n";
			}
			if(name.find("_Param4MultST3_")!=std::string::npos)
			{
				str += "#define _Param4MultST3\n";
			}
			if(name.find("_Param4AddST3_")!=std::string::npos)
			{
				str += "#define _Param4AddST3\n";
			}

			if(name.find("_Param1MultST3a_")!=std::string::npos)
			{
				str += "#define _Param1MultST3a\n";
			}
			if(name.find("_Param1AddST3a_")!=std::string::npos)
			{
				str += "#define _Param1AddST3a\n";
			}
		}
	}
	////////////ExternTexture
	if(name.find("_UVS_")!=std::string::npos)
	{
		str += "#define _UVS\n";
		if(name.find("_UVSUV1_")!=std::string::npos)
		{
			str += "#define _UVSUV1\n";
		}
		if(name.find("_UVSM_")!=std::string::npos)
		{
			str += "#define _UVSM\n";
		}
		if(name.find("_Param1MultUVS_")!=std::string::npos)
		{
			str += "#define _Param1MultUVS\n";
		}
		if(name.find("_Param4MultUVS_")!=std::string::npos)
		{
			str += "#define _Param4MultUVS\n";
		}
		if(name.find("_Param4AddUVS_")!=std::string::npos)
		{
			str += "#define _Param4AddUVS\n";
		}
	}
	else if(name.find("_CW_")!=std::string::npos)
	{
		str += "#define _CW\n";
		if(name.find("_CWUV1_")!=std::string::npos)
		{
			str += "#define _CWUV1\n";
		}
		int cwm = 0;
		if(name.find("_CWMR_")!=std::string::npos)
		{
			str += "#define _CWMR\n";
			cwm++;
		}
		if(name.find("_CWMG_")!=std::string::npos)
		{
			str += "#define _CWMG\n";
			cwm++;
		}
		if(name.find("_CWMB_")!=std::string::npos)
		{
			str += "#define _CWMB\n";
			cwm++;
		}
		if(name.find("_CWMA_")!=std::string::npos)
		{
			str += "#define _CWMA\n";
			cwm++;
		}
        if(cwm<4)
		{
			if(name.find("_CWM_")!=std::string::npos)
			{
				str += "#define _CWM\n";
			}
		}

		if(name.find("_Param1MultCW_")!=std::string::npos)
		{
			str += "#define _Param1MultCW\n";
		}
		if(name.find("_Param4MultCW_")!=std::string::npos)
		{
			str += "#define _Param4MultCW\n";
		}
		if(name.find("_Param4AddCW_")!=std::string::npos)
		{
			str += "#define _Param4AddCW\n";
		}
		if(name.find("_Param3MultCWD_")!=std::string::npos)
		{
			str += "#define _Param3MultCWD\n";
		}
		if(name.find("_Param3MultCWDa_")!=std::string::npos)
		{
			str += "#define _Param3MultCWDa\n";
		}
		if(mode != 2)
		{
			if(name.find("_Param3MultCWN_")!=std::string::npos)
			{
				str += "#define _Param3MultCWN\n";
			}
			if(name.find("_Param3MultCWS_")!=std::string::npos)
			{
				str += "#define _Param3MultCWS\n";
			}
			if(name.find("_Param3MultCWSa_")!=std::string::npos)
			{
				str += "#define _Param3MultCWSa\n";
			}
		}
		if(mode!=1 || addET3To || addET3To || addET3To)
		{
			if(name.find("_Param3MultCWE_")!=std::string::npos)
			{
				str += "#define _Param3MultCWE\n";
			}

			if(mode != 1)
			{
				if(name.find("_Param3MultCWEa_")!=std::string::npos)
				{
					str += "#define _Param3MultCWEa\n";
				}
			}
		}
	}
	/////////////////////////////
	return str;
}
std::string crShaderManager::getShadowName(const std::string &statesetShaderStr,crStateSet *shadow_ss,crStateSet *drawable_ss, crStateSet *obj_ss)
{
	std::string str;
	crTexture *tex;
	crUniform *uniform;
	bool needDetail = false; 
	bool needExtern = false;
	if(statesetShaderStr.find("_REPLACEBTa_")!=std::string::npos)
	{
		str += "_REPLACEBTa";
		needDetail = true;
	}
	else if(statesetShaderStr.find("_ADDBTa_")!=std::string::npos)
	{
		str += "_ADDBTa";
		needDetail = true;
	}
	else if(statesetShaderStr.find("_MULTBTa_")!=std::string::npos)
	{
		str += "_MULTBTa";
		needDetail = true;
	}
    if(needDetail)
	{
		if(statesetShaderStr.find("_BT_")!=std::string::npos)
		{
			needExtern = true;
			str += "_BT";
			if(statesetShaderStr.find("_BTUV1_")!=std::string::npos)
			{
				str += "_BTUV1";
			}
			tex = dynamic_cast<crTexture *>(drawable_ss->getTextureAttribute(TEXTURE_BASEMAP,crStateAttribute::TEXTURE));
			shadow_ss->setTextureAttributeAndModes(TEXTURE_BASEMAP,tex,crStateAttribute::ON);

			if(statesetShaderStr.find("_BMa_")!=std::string::npos)
			{
				str += "_BMa";
				shadow_ss->addUniform(obj_ss->getUniform("baseMatA"));
			}

			if(statesetShaderStr.find("_Param1MultBTa_")!=std::string::npos)
			{
				str += "_Param1MultBTa";
				uniform = drawable_ss->getUniform("param1MultBTa");
				if(!uniform)
					uniform = obj_ss->getUniform("param1MultBTa");
				shadow_ss->addUniform(uniform);
			}
			if(statesetShaderStr.find("_Param1AddBTa_")!=std::string::npos)
			{
				str += "_Param1AddBTa";
				uniform = drawable_ss->getUniform("param1AddBTa");
				if(!uniform)
					uniform = obj_ss->getUniform("param1AddBTa");
				shadow_ss->addUniform(uniform);
			}
		}
	}

	needDetail = false;
	if(statesetShaderStr.find("_REPLACEDTa_")!=std::string::npos)
	{
		str += "_REPLACEDTa";
		needDetail = true;
	}
	else if(statesetShaderStr.find("_ADDDTa_")!=std::string::npos)
	{
		str += "_ADDDTa";
		needDetail = true;
	}
	else if(statesetShaderStr.find("_MULTDTa_")!=std::string::npos)
	{
		str += "_MULTDTa";
		needDetail = true;
	}
	if(needDetail)
	{
		if(statesetShaderStr.find("_DT_")!=std::string::npos)
		{
			needExtern = true;
			str += "_DT";
			if(statesetShaderStr.find("_DTUV1_")!=std::string::npos)
			{
				str += "_DTUV1";
			}
			tex = dynamic_cast<crTexture *>(drawable_ss->getTextureAttribute(TEXTURE_DIFFUSEMAP,crStateAttribute::TEXTURE));
			shadow_ss->setTextureAttributeAndModes(TEXTURE_DIFFUSEMAP,tex,crStateAttribute::ON);

			if(statesetShaderStr.find("_DMa_")!=std::string::npos)
			{
				str += "_DMa";
				shadow_ss->addUniform(obj_ss->getUniform("diffuseMatA"));
			}

			if(statesetShaderStr.find("_Param1MultDTa_")!=std::string::npos)
			{
				str += "_Param1MultDTa";
				uniform = drawable_ss->getUniform("param1MultDTa");
				if(!uniform)
					uniform = obj_ss->getUniform("param1MultDTa");
				shadow_ss->addUniform(uniform);
			}
			if(statesetShaderStr.find("_Param1AddDTa_")!=std::string::npos)
			{
				str += " _Param1AddDTa";
				uniform = drawable_ss->getUniform("param1AddDTa");
				if(!uniform)
					uniform = obj_ss->getUniform("param1AddDTa");
				shadow_ss->addUniform(uniform);
			}
		}
	}

    needDetail = false;
	if(statesetShaderStr.find("_REPLACEDT2a_")!=std::string::npos)
	{
		str += "_REPLACEDT2a";
		needDetail = true;
	}
	else if(statesetShaderStr.find("_ADDDT2a_")!=std::string::npos)
	{
		str += "_ADDDT2a";
		needDetail = true;
	}
	else if(statesetShaderStr.find("_MULTDT2a_")!=std::string::npos)
	{
		str += "_MULTDT2a";
		needDetail = true;
	}
    if(needDetail)
	{
		if(statesetShaderStr.find("_DT2_")!=std::string::npos)
		{
			needExtern = true;
			str += "_DT2";
			if(statesetShaderStr.find("_DT2UV1_")!=std::string::npos)
			{
				str += "_DT2UV1";
			}
			tex = dynamic_cast<crTexture *>(drawable_ss->getTextureAttribute(TEXTURE_DIFFUSEMAP2,crStateAttribute::TEXTURE));
			shadow_ss->setTextureAttributeAndModes(TEXTURE_DIFFUSEMAP2,tex,crStateAttribute::ON);
		
			if(statesetShaderStr.find("_DM2a_")!=std::string::npos)
			{
				str += "_DM2a";
				shadow_ss->addUniform(obj_ss->getUniform("diffuseMat2A"));
			}

			if(statesetShaderStr.find("_Param1MultDT2a_")!=std::string::npos)
			{
				str += "_Param1MultDT2a";
				uniform = drawable_ss->getUniform("param1MultDT2a");
				if(!uniform)
					uniform = obj_ss->getUniform("param1MultDT2a");
				shadow_ss->addUniform(uniform);
			}
			if(statesetShaderStr.find("_Param1AddDT2a_")!=std::string::npos)
			{
				str += "_Param1AddDT2a";
				uniform = drawable_ss->getUniform("param1AddDT2a");
				if(!uniform)
					uniform = obj_ss->getUniform("param1AddDT2a");
				shadow_ss->addUniform(uniform);
			}
		}
	}
	////////////ExternTexture
	if(needExtern)
	{
		if(statesetShaderStr.find("_UVS_")!=std::string::npos)
		{
			str += "_UVS";
			if(statesetShaderStr.find("_UVSUV1_")!=std::string::npos)
			{
				str += "_UVSUV1";
			}
			tex = dynamic_cast<crTexture2D *>(drawable_ss->getTextureAttribute(TEXTURE_UVSCRAMBLER,crStateAttribute::TEXTURE));
			shadow_ss->setTextureAttributeAndModes(TEXTURE_UVSCRAMBLER,tex,crStateAttribute::ON);

			if(statesetShaderStr.find("_UVSM_")!=std::string::npos)
			{
				str += "_UVSM";
				shadow_ss->addUniform(obj_ss->getUniform("UVSMat"));
			}

			if(statesetShaderStr.find("_Param1MultUVS_")!=std::string::npos)
			{
				str += "_Param1MultUVS";
				uniform = drawable_ss->getUniform("param1MultUVS");
				if(!uniform)
					uniform = obj_ss->getUniform("param1MultUVS");
				shadow_ss->addUniform(uniform);
			}
			if(statesetShaderStr.find("_Param4MultUVS_")!=std::string::npos)
			{
				str += "_Param4MultUVS";
				uniform = drawable_ss->getUniform("param4MultUVS");
				if(!uniform)
					uniform = obj_ss->getUniform("param4MultUVS");
				shadow_ss->addUniform(uniform);
			}
			if(statesetShaderStr.find("_Param4AddUVS_")!=std::string::npos)
			{
				str += "_Param4AddUVS";
				uniform = drawable_ss->getUniform("param4AddUVS");
				if(!uniform)
					uniform = obj_ss->getUniform("param4AddUVS");
				shadow_ss->addUniform(uniform);
			}
		}
		else if(statesetShaderStr.find("_CW_")!=std::string::npos)
		{
			str += "_CW";
			if(statesetShaderStr.find("_CWUV1_")!=std::string::npos)
			{
				str += "_CWUV1";
			}
			tex = dynamic_cast<crTexture2D *>(drawable_ss->getTextureAttribute(TEXTURE_COLORWEIGHT,crStateAttribute::TEXTURE));
			shadow_ss->setTextureAttributeAndModes(TEXTURE_COLORWEIGHT,tex,crStateAttribute::ON);

			int tmp = 0;
			if(statesetShaderStr.find("_CWMR_")!=std::string::npos)
			{
				str += "_CWMR";
				shadow_ss->addUniform(obj_ss->getUniform("CWMatR"));
				tmp++;
			}
			if(statesetShaderStr.find("_CWMG_")!=std::string::npos)
			{
				str += "_CWMG";
				shadow_ss->addUniform(obj_ss->getUniform("CWMatG"));
				tmp++;
			}
			if(statesetShaderStr.find("_CWMB_")!=std::string::npos)
			{
				str += "_CWMB";
				shadow_ss->addUniform(obj_ss->getUniform("CWMatB"));
				tmp++;
			}
            if(tmp<3)
			{
				if(statesetShaderStr.find("_CWM_")!=std::string::npos)
				{
					str += "_CWM";
					shadow_ss->addUniform(obj_ss->getUniform("CWMat"));
				}
			}

			if(statesetShaderStr.find("_Param1MultCW_")!=std::string::npos)
			{
				str += "_Param1MultCW";
				uniform = drawable_ss->getUniform("param1MultCW");
				if(!uniform)
					uniform = obj_ss->getUniform("param1MultCW");
				shadow_ss->addUniform(uniform);
			}
			if(statesetShaderStr.find("_Param4MultCW_")!=std::string::npos)
			{
				str += "_Param4MultCW";
				uniform = drawable_ss->getUniform("param4MultCW");
				if(!uniform)
					uniform = obj_ss->getUniform("param4MultCW");
				shadow_ss->addUniform(uniform);
			}
			if(statesetShaderStr.find("_Param4AddCW_")!=std::string::npos)
			{
				str += "_Param4AddCW";
				uniform = drawable_ss->getUniform("param4AddCW");
				if(!uniform)
					uniform = obj_ss->getUniform("param4AddCW");
				shadow_ss->addUniform(uniform);
			}
			if(statesetShaderStr.find("_Param3MultCWDa_")!=std::string::npos)
			{
				str += "_Param3MultCWDa";
				uniform = drawable_ss->getUniform("param3MultCWDa");
				if(!uniform)
					uniform = obj_ss->getUniform("param3MultCWDa");
				shadow_ss->addUniform(uniform);
			}
		}
	}
	/////////////////////////////
	str += "_";
	return str;
}
std::string crShaderManager::getShadowHead(const std::string &name)
{
	std::string str;
	bool needDetail = false;
	if(name.find("_AlphaShadow_")!=std::string::npos)
	{
		str += "#define _AlphaShadow\n";
		needDetail = true;
	}
	if(name.find("_BlendShadow_")!=std::string::npos)
	{
		str += "#define _BlendShadow\n";
		needDetail = true;
	}
	if(needDetail)
	{
		if(name.find("_BT_")!=std::string::npos)
		{
			str += "#define _BT\n";
			if(name.find("_BTUV1_")!=std::string::npos)
			{
				str += "#define _BTUV1\n";
			}
			if(name.find("_BMa_")!=std::string::npos)
			{
				str += "#define _BMa\n";
			}

			if(name.find("_REPLACEBTa_")!=std::string::npos)
			{
				str += "#define _REPLACEBTa\n";
			}
			else if(name.find("_ADDBTa_")!=std::string::npos)
			{
				str += "#define _ADDBTa\n";
			}
			else if(name.find("_MULTBTa_")!=std::string::npos)
			{
				str += "#define _MULTBTa\n";
			}

			if(name.find("_Param1MultBTa_")!=std::string::npos)
			{
				str += "#define _Param1MultBTa\n";
			}
			if(name.find("_Param1AddBTa_")!=std::string::npos)
			{
				str += "#define _Param1AddBTa\n";
			}
		}

		if(name.find("_DT_")!=std::string::npos)
		{
			str += "#define _DT\n";
			if(name.find("_DTUV1_")!=std::string::npos)
			{
				str += "#define _DTUV1\n";
			}
			if(name.find("_DMa_")!=std::string::npos)
			{
				str += "#define _DMa\n";
			}

			if(name.find("_REPLACEDTa_")!=std::string::npos)
			{
				str += "#define _REPLACEDTa\n";
			}
			else if(name.find("_ADDDTa_")!=std::string::npos)
			{
				str += "#define _ADDDTa\n";
			}
			else if(name.find("_MULTDTa_")!=std::string::npos)
			{
				str += "#define _MULTDTa\n";
			}

			if(name.find("_Param1MultDTa_")!=std::string::npos)
			{
				str += "#define _Param1MultDTa\n";
			}
			if(name.find("_Param1AddDTa_")!=std::string::npos)
			{
				str += "#define _Param1AddDTa\n";
			}
		}
		if(name.find("_DT2_")!=std::string::npos)
		{
			str += "#define _DT2\n";
			if(name.find("_DT2UV1_")!=std::string::npos)
			{
				str += "#define _DT2UV1\n";
			}
			if(name.find("_DM2a_")!=std::string::npos)
			{
				str += "#define _DM2a\n";
			}

			if(name.find("_REPLACEDT2a_")!=std::string::npos)
			{
				str += "#define _REPLACEDT2a\n";
			}
			else if(name.find("_ADDDT2a_")!=std::string::npos)
			{
				str += "#define _ADDDT2a\n";
			}
			else if(name.find("_MULTDT2a_")!=std::string::npos)
			{
				str += "#define _MULTDT2a\n";
			}

			if(name.find("_Param1MultDT2a_")!=std::string::npos)
			{
				str += "#define _Param1MultDT2a\n";
			}
			if(name.find("_Param1AddDT2a_")!=std::string::npos)
			{
				str += "#define _Param1AddDT2a\n";
			}
		}

		////////////ExternTexture
		if(name.find("_UVS_")!=std::string::npos)
		{
			str += "#define _UVS\n";
			if(name.find("_UVSUV1_")!=std::string::npos)
			{
				str += "#define _UVSUV1\n";
			}
			if(name.find("_UVSM_")!=std::string::npos)
			{
				str += "#define _UVSM\n";
			}
			if(name.find("_Param1MultUVS_")!=std::string::npos)
			{
				str += "#define _Param1MultUVS\n";
			}
			if(name.find("_Param4MultUVS_")!=std::string::npos)
			{
				str += "#define _Param4MultUVS\n";
			}
			if(name.find("_Param4AddUVS_")!=std::string::npos)
			{
				str += "#define _Param4AddUVS\n";
			}
		}
		else if(name.find("_CW_")!=std::string::npos)
		{
			str += "#define _CW\n";
			if(name.find("_CWUV1_")!=std::string::npos)
			{
				str += "#define _CWUV1\n";
			}
			int cwm = 0;
			if(name.find("_CWMR_")!=std::string::npos)
			{
				str += "#define _CWMR\n";
				cwm++;
			}
			if(name.find("_CWMG_")!=std::string::npos)
			{
				str += "#define _CWMG\n";
				cwm++;
			}
			if(name.find("_CWMB_")!=std::string::npos)
			{
				str += "#define _CWMB\n";
				cwm++;
			}
			if(cwm<3)
			{
				if(name.find("_CWM_")!=std::string::npos)
				{
					str += "#define _CWM\n";
				}
			}
			
			if(name.find("_Param1MultCW_")!=std::string::npos)
			{
				str += "#define _Param1MultCW\n";
			}
			if(name.find("_Param4MultCW_")!=std::string::npos)
			{
				str += "#define _Param4MultCW\n";
			}
			if(name.find("_Param4AddCW_")!=std::string::npos)
			{
				str += "#define _Param4AddCW\n";
			}
			if(name.find("_Param3MultCWDa_")!=std::string::npos)
			{
				str += "#define _Param3MultCWDa\n";
			}
		}
		/////////////////////////////
	}
	return str;
}


crStateSet* crShaderManager::loadShader(const std::string &name)
{
	std::string vp;
	std::string fp;
	std::string headStr;
	if(name.find("sun")!=std::string::npos)
	{
		vp = m_stringVec[0];
		fp = m_stringVec[1];
		headStr = getLightHead(name,0);
		vp = headStr + vp;
		fp = headStr + fp;
		return createShader(name,vp,fp,true,true);
	}
	else if(name.find("direct")!=std::string::npos || name.find("spot")!=std::string::npos)
	{
		vp = m_stringVec[2];
		fp = m_stringVec[3];
		headStr = getLightHead(name,1);
		vp = headStr + vp;
		fp = headStr + fp;
		return createShader(name,vp,fp,true,true);
	}
	else if(name.find("nolight")!=std::string::npos)
	{
		vp = m_stringVec[4];
		fp = m_stringVec[5];
		headStr = getLightHead(name,2);
		vp = headStr + vp;
		fp = headStr + fp;
		return createShader(name,vp,fp,false,false);
	}
	else if(name.find("shadow")!=std::string::npos)
	{
        vp = m_stringVec[6];
		fp = m_stringVec[7];
		headStr = getShadowHead(name);
		vp = headStr + vp;
		fp = headStr + fp;
		crStateSet* stateset = createShader(name,vp,fp,false,false);
		stateset->setStateSetMode(crStateSet::SS_ShadowMapRender);
		return stateset;
	}
	return NULL;
}
crStateSet* crShaderManager::createShader(const std::string& shaderName,const std::string& vp, const std::string& fp, bool addT, bool addB)
{
	ref_ptr<crStateSet> shader_ss;
	CRCore::ref_ptr<CRCore::crProgram> program;
	CRCore::ref_ptr<CRCore::crShader> shader;

	program = new CRCore::crProgram;
	program->setName(shaderName);

	//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadShaderManagerCfg(): shaderName = "<<shaderName.c_str()<<std::endl;
	//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadShaderManagerCfg(): rtglmHeadStr = "<<rtglmHeadStr.c_str()<<std::endl;
	//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadShaderManagerCfg(): temp_vp = "<<temp_vp.c_str()<<std::endl;
	shader = new CRCore::crShader(CRCore::crShader::VERTEX,vp);
	shader->setName(shaderName+"_cv");
	program->addShader(shader.get());

	//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadShaderManagerCfg(): temp_fp = "<<temp_fp.c_str()<<std::endl;
	shader = new crShader(CRCore::crShader::FRAGMENT,fp);
	shader->setName(shaderName+"_cf");
	program->addShader(shader.get());

	if(addT) program->addBindAttribLocation("tangent",ATTRIBUTE_TANGENT);
	//if(addB) program->addBindAttribLocation("binormal",ATTRIBUTE_BINORMAL);

	shader_ss = new crStateSet;
	shader_ss->setAttributeAndModes(program.get(),CRCore::crStateAttribute::ON);
	//shader_ss->setMode(GL_LIGHTING, crStateAttribute::OVERRIDE|crStateAttribute::OFF);
	shader_ss->setName(shaderName);
	insertShaderStateSet(shader_ss.get());
	return shader_ss.get();
}
///////////////////////
//
//SunCallback
//
////////////
SunCallback::SunCallback()
{
	m_appendLightCallback = new NeedLightCallback;
}
void SunCallback::getUniforms_sun(crStateSet *uniform_ss,crLightSource *sun)
{
	crUniform *uniform;
	//sunlight
	uniform = uniform_ss->getOrCreateUniform("lightPos",crUniform::FLOAT_VEC3);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(sun->getLSPosition());

	//uniform = uniform_ss->getOrCreateUniform("lightColor",crUniform::FLOAT_VEC4);
	//uniform->setDataVariance(crBase::STATIC);
	//uniform->set(sun->getLSColor());
	uniform = uniform_ss->getOrCreateUniform("lightParam",crUniform::FLOAT_MAT4);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(sun->getLSParam());

	uniform = uniform_ss->getOrCreateUniform("loweastLum",crUniform::FLOAT);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(sun->getLoweastLum());
}

void SunCallback::getUniforms_sun(crStateSet *uniform_ss,crLightSource *sun, const crVector3 &center)
{
	crUniform *uniform;
	//sunlight
	uniform = uniform_ss->getOrCreateUniform("lightPos",crUniform::FLOAT_VEC3);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(sun->getLSPosition() + center);///////对于空战里面的BOSS，需要根据其位置调整太阳光源的位置。

	//uniform = uniform_ss->getOrCreateUniform("lightColor",crUniform::FLOAT_VEC4);
	//uniform->setDataVariance(crBase::STATIC);
	//uniform->set(sun->getLSColor());
	uniform = uniform_ss->getOrCreateUniform("lightParam",crUniform::FLOAT_MAT4);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(sun->getLSParam());

	uniform = uniform_ss->getOrCreateUniform("loweastLum",crUniform::FLOAT);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(sun->getLoweastLum());
}

void SunCallback::getUniforms_spot(crStateSet *uniform_ss,crLightSource *ls,int lightNum)
{
	crVector3 temp_vec3;
	crUniform *uniform;
	//l1
	char buf[20];
	sprintf(buf,"lightPoss%d\0",lightNum);
	uniform = uniform_ss->getOrCreateUniform(buf,crUniform::FLOAT_VEC3);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(ls->getLSPosition());

	//sprintf(buf,"lightColor%d\0",lightNum);
	//uniform = uniform_ss->getOrCreateUniform(buf,crUniform::FLOAT_VEC4);
	//uniform->setDataVariance(crBase::STATIC);
	//uniform->set(ls->getLSColor());
	
	sprintf(buf,"lightDir%d\0",lightNum);
	uniform = uniform_ss->getOrCreateUniform(buf,crUniform::FLOAT_VEC3);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(ls->getLSDirection());
	
	//sprintf(buf,"lightAttenuation%d\0",lightNum);
	//uniform = uniform_ss->getOrCreateUniform(buf,crUniform::FLOAT_VEC4);
	//uniform->setDataVariance(crBase::STATIC);
	//uniform->set(ls->getLSAttenuations());
	
	sprintf(buf,"lightParams%d\0",lightNum);
	uniform = uniform_ss->getOrCreateUniform(buf,crUniform::FLOAT_MAT4);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(ls->getLSParam());

	sprintf(buf,"cosLightFov%d\0",lightNum);
	uniform = uniform_ss->getOrCreateUniform(buf,crUniform::FLOAT);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(ls->getCosLightMapFov());

	sprintf(buf,"loweastLums%d\0",lightNum);
	uniform = uniform_ss->getOrCreateUniform(buf,crUniform::FLOAT);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(ls->getLoweastLum());
}

void SunCallback::getUniforms_direct(crStateSet *uniform_ss,crLightSource *ls,int lightNum)
{
	crVector3 temp_vec3;
	crUniform *uniform;
	//l1
	char buf[20];
	sprintf(buf,"lightPosd%d\0",lightNum);
	//CRCore::notify(CRCore::ALWAYS)<<"SunCallback::getUniforms_direct:lightPos1 = "<<str<<std::endl;
	uniform = uniform_ss->getOrCreateUniform(buf,crUniform::FLOAT_VEC3);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(ls->getLSPosition());

	//sprintf(buf,"lightColor%d\0",lightNum);
	////CRCore::notify(CRCore::ALWAYS)<<"SunCallback::getUniforms_direct:lightColor1 = "<<str<<std::endl;
	//uniform = uniform_ss->getOrCreateUniform(buf,crUniform::FLOAT_VEC4);
	//uniform->setDataVariance(crBase::STATIC);
	//uniform->set(ls->getLSColor());

	//sprintf(buf,"lightAttenuation%d\0",lightNum);
	////CRCore::notify(CRCore::ALWAYS)<<"SunCallback::getUniforms_direct:lightAttenuation1 = "<<str<<std::endl;
	//uniform = uniform_ss->getOrCreateUniform(buf,crUniform::FLOAT_VEC3);
	//uniform->setDataVariance(crBase::STATIC);
	//const crVector4f &atten = ls->getLSAttenuations();
	//uniform->set(crVector3(atten[0],atten[1],atten[2]));

	sprintf(buf,"lightParamd%d\0",lightNum);
	uniform = uniform_ss->getOrCreateUniform(buf,crUniform::FLOAT_MAT4);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(ls->getLSParam());

	sprintf(buf,"loweastLumd%d\0",lightNum);
	uniform = uniform_ss->getOrCreateUniform(buf,crUniform::FLOAT);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(ls->getLoweastLum());
}

void SunCallback::getUniforms_rts(crStateSet *uniform_ss,crLightSource *ls, bool acceptShadow2)
{
	crUniform *uniform;
	//sunlight shadow
	uniform = uniform_ss->getOrCreateUniform("lightMVP",crUniform::FLOAT_MAT4);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(ls->getLightMapMVP());
	uniform_ss->setTextureAttributeAndModes(TEXTURE_SHADOWMAP,ls->getLightMap(),crStateAttribute::ON);

	if (acceptShadow2)
	{
		uniform = uniform_ss->getOrCreateUniform("lightMVP2", crUniform::FLOAT_MAT4);
		uniform->setDataVariance(crBase::STATIC);
		uniform->set(ls->getLightMapMVP2());
		uniform_ss->setTextureAttributeAndModes(TEXTURE_SHADOWMAP2, ls->getLightMap2(), crStateAttribute::ON);
	}

	uniform = uniform_ss->getOrCreateUniform("scale",crUniform::FLOAT);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(ls->getLightRangeScale());
	uniform = uniform_ss->getOrCreateUniform("inv_shadowmap_size",crUniform::FLOAT);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(ls->getInvShadowmapSize());
	//if(ls->getLightIsSunLight())
	//{
	//	uniform = uniform_ss->getOrCreateUniform("sqrSunShadowDistance",crUniform::FLOAT_VEC2);
	//	uniform->setDataVariance(crBase::STATIC);
	//	//float offset = crShaderManager::getInstance()->getSunShadowDistanceOffset()[1];
 //       const crVector4& offset = crShaderManager::getInstance()->getSunShadowDistanceOffset();
	//	crVector2 d = crShaderManager::getInstance()->getSunShadowDistance()-crVector2(offset[2],offset[3]);
	//	d[0] *= d[0];
	//	d[1] *= d[1];
	//	uniform->set(d);
	//}
}
void SunCallback::getUniforms_gi(crStateSet *uniform_ss)
{
	crUniform *uniform;
	uniform_ss->setTextureAttributeAndModes(TEXTURE_GIMAP,crShaderManager::getInstance()->getGiTexture(),crStateAttribute::ON);

	uniform = uniform_ss->getOrCreateUniform("giparam",crUniform::FLOAT_VEC4);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(crShaderManager::getInstance()->getGIParam());

	const crVector3i &worldSize = crBrain::getInstance()->getWorldSize();
	uniform = uniform_ss->getOrCreateUniform("maxheight", CRCore::crUniform::FLOAT);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set((float)worldSize[2]);
}
void SunCallback::getUniforms_sgi(crStateSet *uniform_ss)
{
	crUniform *uniform;
	uniform_ss->setTextureAttributeAndModes(TEXTURE_LIGHTMAP,crShaderManager::getInstance()->getLightMapTexture(),crStateAttribute::ON);

	uniform = uniform_ss->getOrCreateUniform("giparam",crUniform::FLOAT_VEC4);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(crShaderManager::getInstance()->getGIParam());
}
void SunCallback::getUniforms_height(crStateSet *uniform_ss)
{
	crUniform *uniform;
	uniform_ss->setTextureAttributeAndModes(TEXTURE_LIGHTMAP,crShaderManager::getInstance()->getHeightMapTexture(),crStateAttribute::ON);

	uniform = uniform_ss->getOrCreateUniform("giparam",crUniform::FLOAT_VEC4);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(crShaderManager::getInstance()->getGIParam());

	const crVector3i &worldSize = crBrain::getInstance()->getWorldSize();
	uniform = uniform_ss->getOrCreateUniform("maxheight",CRCore::crUniform::FLOAT);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set((float)worldSize[2]);
}
//
bool SunCallback::operator()(const crObject *obj,crDrawable* drawable, const crBoundingBox &bbox, float depth, bool effectByShadow, crShaderManager::StateSetVec &statesetVec)
{//需要返回两个stateset，一个是shader一个是该drawable的uniform，将drawable的material元素加入到uniformstateset，避免修改drawable的stateset。
	crLightSourceManager *lightSourceManager = crLightSourceManager::getInstance();
	crLightSource *sun = lightSourceManager->getSunLightSource();
	if(sun == NULL)
	{
		CRCore::notify(CRCore::FATAL)<<"SunCallback::operator() 场景里没有设定太阳光源"<<std::endl;
		return true;
	}
	int rttShadowMode = crRenderToTextureManager::getInstance()->getRttShadowMode();
	effectByShadow &= sun->getCalcShadow() && rttShadowMode>0;

	float radius = bbox.radius();
    if(effectByShadow)
	{
		if(depth - 0.5 * radius/*maximum(bbox.xLength(),bbox.yLength())*/ >= crShaderManager::getInstance()->getSunShadowDistance()[1])
		{
            effectByShadow = false;
		}
	}
	//CRCore::notify(CRCore::FATAL)<<"SunCallback::operator() = "<<std::endl;
	//crLightSourceManager::SelectLightMap &selectLightMap = crLightSourceManager::getInstance()->selectLightByBoundingBox(obj,bbox);
	//int size = selectLightMap.size();
	//CRCore::notify(CRCore::FATAL)<<"SunCallback::operator() size = "<<size<<std::endl;

	crStateSet *drawable_ss = drawable->getStateSet();
	if(drawable_ss == NULL)
		drawable_ss = const_cast<crStateSet *>(obj->getStateSet());
	if(drawable_ss == NULL)
	{
		//CRCore::notify(CRCore::FATAL)<<"SunCallback::operator() not set stateset nodeid = "<<obj->getName()<<std::endl;
		return true;
	}
	const crStateSet *obj_ss = obj->getStateSet();
	std::string statesetShaderStr;
	statesetShaderStr = drawable_ss->getRtglmShaderStr(obj_ss);
	//const std::string &statesetShaderStr = drawable_ss->getRtglmShaderStr(obj_ss);

	crStateSet *shader_ss;
	//ref_ptr<crStateSet> uniform_ss;

	crVector2 fi;
	crVector2 fo;
	bool fadeIn = false;
	bool fadeOut = false;
	crVector2f r(depth-radius * depth-radius,depth+radius);
	r/=crBrain::getInstance()->getMaxDepth();
	r[0] *= r[0];
	r[1] *= r[1];
	if(crArgumentParser::readKeyValue(obj->getDescriptions(),"FadeIn",fi))
	{
		if(r.x() < fi.y() || r.y() > fi.x())
		{
			//CRCore::notify(CRCore::ALWAYS)<<"NoLightCallback::operator(): FadeIn = "<<fi<<obj->getName()<<std::endl;
			fadeIn = true;
		}
	}
	if(crArgumentParser::readKeyValue(obj->getDescriptions(),"FadeOut",fo))
	{
		if(r.x() < fo.y() || r.y() > fo.x())
		{
			//CRCore::notify(CRCore::ALWAYS)<<"NoLightCallback::operator(): FadeOut = "<<fo<<obj->getName()<<std::endl;
			fadeOut = true;
		}
	}
	ref_ptr<crStateSet> uniform_ss;
	crShaderManager *shaderManager = crShaderManager::getInstance();
	//try{
	uniform_ss = shaderManager->getOrCreateIdleStateSet();
	//}
	//catch(...)
	//{
 //        CRCore::notify(CRCore::FATAL)<<"SunCallback::operator() getOrCreateIdleStateSet error "<<std::endl;
	//}
	if(fadeIn||fadeOut)
	{
		crUniform *uniform;
		if(fadeIn)
		{
			uniform = uniform_ss->getOrCreateUniform("fadeIn",crUniform::FLOAT_VEC2);
			uniform->setDataVariance(crBase::STATIC);
			uniform->set(fi);
		}
		if(fadeOut)
		{
			uniform = uniform_ss->getOrCreateUniform("fadeOut",crUniform::FLOAT_VEC2);
			uniform->setDataVariance(crBase::STATIC);
			uniform->set(fo);
		}
		uniform_ss->setAttributeAndModes(crShaderManager::getInstance()->getAlphaFunc(),crStateAttribute::ON);
		uniform_ss->setAttributeAndModes(crShaderManager::getInstance()->getBlendFunc(),crStateAttribute::ON);
		uniform_ss->setRenderingHint(crStateSet::TRANSPARENT_BIN);
	}

	//crMaterial *material = dynamic_cast<crMaterial *>(drawable->getStateSet()->getAttribute(crStateAttribute::MATERIAL));

	//if(str.find("sun")!=-1)
	//{
	std::string str;
	if(sun->getEnable())
	{
		str = "sun";
		//int needTranslateSun = 0;
		//crArgumentParser::readKeyValue(obj->getDescriptions(),"NeedTranslateSun",needTranslateSun);
		//if(needTranslateSun)
		//	getUniforms_sun(uniform_ss.get(),sun,obj->getBound().center());
		//else 
		getUniforms_sun(uniform_ss.get(),sun);
	}
	else
	{
		str = "sunx";
	}
	//}
	crLightSource *ls;
	crLight *light;
    
	if(obj->getNeedSelectLight())
	{
		lightSourceManager->lock();

		lightSourceManager->selectLightByBoundingBox(obj,bbox);
		CRCore::crLightSourceManager::LSVec &appendLightVec = lightSourceManager->getAppendLightVec();
		if(!appendLightVec.empty())
		{
			(*m_appendLightCallback)(obj,drawable,bbox,depth,effectByShadow,statesetVec);
		}
		CRCore::crLightSourceManager::LSVec &onePassLightVec = lightSourceManager->getOnePassLightVec();
		if(!onePassLightVec.empty())
		{
			crLightSourceManager::LSVec dls,sls;
			for( crLightSourceManager::LSVec::iterator slitr = onePassLightVec.begin();
				slitr != onePassLightVec.end(); 
				++slitr)
			{
				ls = slitr->get();
				light = dynamic_cast<crLight *>(ls->getLight());
				if(light->getLightType() == crLight::LIGHT_LOCAL)
					dls.push_back(ls);
				else/* if(light->getLightType() == crLight::LIGHT_SPOT)*/ 
					sls.push_back(ls);
			}

			char buf[8];
			int i = 0;
			int allotd = 0;
			int allots = 0;
			int dcount = dls.size();
			int scount = sls.size();
			if(dcount>0)
			{
				allotd = dcount;
				sprintf(buf,"direct%d\0",allotd);
				str += buf;
				if(scount>0)
				{
					allots = scount;
					sprintf(buf,"spot%d\0",allots);
					str += buf;
				}
			}
			else if(scount>0)
			{
				allots = scount;
				sprintf(buf,"spot%d\0",allots);
				str += buf;
			}

			for(i = 0; i<allotd; i++)
			{
				getUniforms_direct(uniform_ss.get(),dls[i].get(),i+1);
			}
			for(i = 0; i<allots; i++)
			{
				getUniforms_spot(uniform_ss.get(),sls[i].get(),i+1);
			}
		}
		lightSourceManager->unlock();
	}
	char acceptGI = obj->getAcceptGI();
	//if (acceptGI == 0) acceptGI = 1;
	bool effectByShadow2 = effectByShadow && acceptGI<=0 && rttShadowMode==2 && crBrain::getInstance()->getCalcStaticMeshShadow() && sun->getCalcShadow2() && sun->getLightMap2();
	getProgramString(str,effectByShadow,effectByShadow2,acceptGI,fadeIn,fadeOut);
	str += statesetShaderStr;
	//CRCore::notify(CRCore::ALWAYS)<<"SunCallback::operator():"<<str<<std::endl;
	shader_ss = shaderManager->getShaderStateSet(str);
	if(effectByShadow/*str.find("rts")!=std::string::npos*/)
	{
		getUniforms_rts(uniform_ss.get(),sun,effectByShadow2);
	}

	if(acceptGI==1)
	{
		if(crShaderManager::getInstance()->getGiTexture())
			getUniforms_gi(uniform_ss.get());
		if(crShaderManager::getInstance()->getLightMapTexture())
			getUniforms_sgi(uniform_ss.get());
	}
	else if(acceptGI==2)
	{
		if(crShaderManager::getInstance()->getGiTexture())
			getUniforms_gi(uniform_ss.get());
		if(crShaderManager::getInstance()->getHeightMapTexture())
			getUniforms_height(uniform_ss.get());
		//else if(crShaderManager::getInstance()->getLightMapTexture())
		//	getUniforms_sgi(uniform_ss.get());
	}
	else if(crShaderManager::getInstance()->getLightMapTexture())
	{
		getUniforms_sgi(uniform_ss.get());
	}
	shaderManager->pushActiveStateSet(uniform_ss.get());

	statesetVec.push_back(std::make_pair(shader_ss,uniform_ss));
  //  if(!shader_ss || !uniform_ss.valid())
		//CRCore::notify(CRCore::WARN) << "SunCallback::operator() error shader_ss = "<<shader_ss<<" uniform_ss = "<<uniform_ss.valid()<<" shaderStr = "<<str<<std::endl;
	return false;
}
bool SunCallback::giMapRender(const crObject *obj, crDrawable* drawable, crShaderManager::StateSetVec &statesetVec)
{
	crLightSourceManager *lightSourceManager = crLightSourceManager::getInstance();
	crLightSource *sun = lightSourceManager->getSunLightSource();
	if (sun == NULL)
	{
		CRCore::notify(CRCore::FATAL) << "SunCallback::operator() 场景里没有设定太阳光源" << std::endl;
		return true;
	}

	crStateSet *drawable_ss = drawable->getStateSet();
	if (drawable_ss == NULL)
		drawable_ss = const_cast<crStateSet *>(obj->getStateSet());
	if (drawable_ss == NULL)
	{
		//CRCore::notify(CRCore::FATAL)<<"SunCallback::operator() not set stateset nodeid = "<<obj->getName()<<std::endl;
		return true;
	}
	const crStateSet *obj_ss = obj->getStateSet();
	std::string statesetShaderStr;
	statesetShaderStr = drawable_ss->getRtglmShaderStr(obj_ss);

	crStateSet *shader_ss;
	ref_ptr<crStateSet> uniform_ss;
	crShaderManager *shaderManager = crShaderManager::getInstance();
	uniform_ss = shaderManager->getOrCreateIdleStateSet();
	std::string str;
	if (sun->getEnable())
	{
		str = "sun_gimap";
		getUniforms_sun(uniform_ss.get(), sun);
	}
	else
	{
		str = "sunx_gimap";
	}
	getProgramString(str, false, false, -2, false, false);
	str += statesetShaderStr;
	shader_ss = shaderManager->getShaderStateSet(str);
	//if (crShaderManager::getInstance()->getLightMapTexture())
	//{
	//	getUniforms_sgi(uniform_ss.get());
	//}

	uniform_ss->setMode(GL_BLEND, crStateAttribute::OFF | crStateAttribute::OVERRIDE);
	uniform_ss->setMode(GL_ALPHA_TEST, crStateAttribute::OFF | crStateAttribute::OVERRIDE);
	const crVector3i &worldSize = crBrain::getInstance()->getWorldSize();
	crUniform *uniform;
	uniform = uniform_ss->getOrCreateUniform("maxheight", CRCore::crUniform::FLOAT);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set((float)worldSize[2]);

	shaderManager->pushActiveStateSet(uniform_ss.get());
	statesetVec.push_back(std::make_pair(shader_ss, uniform_ss));
	return false;
}
//////////////////////////////
//
//NeedLightCallback
//
/////////////////////////////////
void NeedLightCallback::getUniforms_spot(crStateSet *uniform_ss,crLightSource *ls,int lightNum)
{
	crVector3 temp_vec3;
	crUniform *uniform;
	//l1
	char buf[20];
	sprintf(buf,"lightPoss%d\0",lightNum);
	uniform = uniform_ss->getOrCreateUniform(buf,crUniform::FLOAT_VEC3);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(ls->getLSPosition());

	sprintf(buf,"lightDir%d\0",lightNum);
	uniform = uniform_ss->getOrCreateUniform(buf,crUniform::FLOAT_VEC3);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(ls->getLSDirection());

	sprintf(buf,"lightParams%d\0",lightNum);
	uniform = uniform_ss->getOrCreateUniform(buf,crUniform::FLOAT_MAT4);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(ls->getLSParam());

	sprintf(buf,"cosLightFov%d\0",lightNum);
	uniform = uniform_ss->getOrCreateUniform(buf,crUniform::FLOAT);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(ls->getCosLightMapFov());

	sprintf(buf,"loweastLums%d\0",lightNum);
	uniform = uniform_ss->getOrCreateUniform(buf,crUniform::FLOAT);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(ls->getLoweastLum());
}

void NeedLightCallback::getUniforms_direct(crStateSet *uniform_ss,crLightSource *ls,int lightNum)
{
	crVector3 temp_vec3;
	crUniform *uniform;
	//l1
	char buf[20];
	sprintf(buf,"lightPosd%d\0",lightNum);
	//CRCore::notify(CRCore::ALWAYS)<<"SunCallback::getUniforms_direct:lightPos1 = "<<str<<std::endl;
	uniform = uniform_ss->getOrCreateUniform(buf,crUniform::FLOAT_VEC3);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(ls->getLSPosition());

	sprintf(buf,"lightParamd%d\0",lightNum);
	uniform = uniform_ss->getOrCreateUniform(buf,crUniform::FLOAT_MAT4);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(ls->getLSParam());

	sprintf(buf,"loweastLumd%d\0",lightNum);
	uniform = uniform_ss->getOrCreateUniform(buf,crUniform::FLOAT);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(ls->getLoweastLum());
}
//
bool NeedLightCallback::operator()(const crObject *obj,crDrawable* drawable, const crBoundingBox &bbox, float depth, bool effectByShadow, crShaderManager::StateSetVec &statesetVec)
{//需要返回两个stateset，一个是shader一个是该drawable的uniform，将drawable的material元素加入到uniformstateset，避免修改drawable的stateset。
//不计算淡入淡出
	CRCore::crLightSourceManager::LSVec &appendLightVec = crLightSourceManager::getInstance()->getAppendLightVec();
	if(appendLightVec.empty()) return false;

	crStateSet *drawable_ss = drawable->getStateSet();
	if(drawable_ss == NULL)
		drawable_ss = const_cast<crStateSet *>(obj->getStateSet());
	if(drawable_ss == NULL)
	{
		//CRCore::notify(CRCore::FATAL)<<"NeedLightCallback::operator() not set stateset nodeid = "<<obj->getName()<<std::endl;
		return true;
	}
	const crStateSet *obj_ss = obj->getStateSet();
	std::string statesetShaderStr;
	statesetShaderStr = drawable_ss->getRtglmShaderStr(obj_ss);
	//const std::string &statesetShaderStr = drawable_ss->getRtglmShaderStr(obj_ss);

	crStateSet *shader_ss;
	ref_ptr<crStateSet> uniform_ss;
	crLightSource *ls;
	crLight *light;
	std::string str;

    crLightSourceManager::LSVec dls,sls;
	for( crLightSourceManager::LSVec::iterator litr = appendLightVec.begin();
		litr != appendLightVec.end();
		litr++ )
	{
		ls = litr->get();
		light = dynamic_cast<crLight *>(ls->getLight());
		if(light->getLightType() == crLight::LIGHT_LOCAL)
			dls.push_back(ls);
		else/* if(light->getLightType() == crLight::LIGHT_SPOT)*/ 
			sls.push_back(ls);
	}
	int dcount = dls.size();
	int scount = sls.size();
    char buf[8];
	int i = 0;
    int remain;
	int allotd,allots,offsetd,offsets;
	offsetd = 0;
	offsets = 0;
	crShaderManager *shaderManager = crShaderManager::getInstance();
	while(dcount>0 || scount>0)
	{//均匀分配可以获得更好的优化
		allotd = 0;
		allots = 0;
        if(dcount>0)
		{
            if(dcount<5)
			{
				allotd = dcount;
				sprintf(buf,"direct%d\0",allotd);
				str = buf;
				remain = 5-allotd;
				if(scount>0)
				{
					allots = minimum(scount,remain);
					sprintf(buf,"spot%d\0",allots);
					str += buf;
					scount -= allots;
				}
				dcount = 0;
			}
			else
			{
				allotd = 5;
				sprintf(buf,"direct%d\0",5);
				str = buf;
				dcount -= 5;
			}
		}
		else if(scount>0)
		{
			allots = minimum(scount,5);
			sprintf(buf,"spot%d\0",allots);
			str = buf;
			scount -= allots;
		}

	    str += statesetShaderStr;

		shader_ss = shaderManager->getShaderStateSet(str);
	    uniform_ss = shaderManager->getOrCreateIdleStateSet();
		for(i = 0; i<allotd; i++)
		{
			getUniforms_direct(uniform_ss.get(),dls[offsetd+i].get(),i+1);
		}
		for(i = 0; i<allots; i++)
		{
			getUniforms_spot(uniform_ss.get(),sls[offsets+i].get(),i+1);
		}

		offsetd+=allotd;
        offsets+=allots;
		
		shaderManager->pushActiveStateSet(uniform_ss.get());

		//if(!shader_ss || !uniform_ss.valid())
		//	CRCore::notify(CRCore::WARN) << "LightCallback::operator() error shader_ss = "<<shader_ss<<" uniform_ss = "<<uniform_ss.valid()<<" shaderStr = "<<str<<std::endl;
		statesetVec.push_back(std::make_pair(shader_ss,uniform_ss.get()));
	}
	return false;
}

///////////////////////////////////
//
//LightCallback
//
////////////////////////////////////
//void LightCallback::getUniforms_sun(crStateSet *uniform_ss,crLightSource *sun)
//{
//	crUniform *uniform;
//	//sunlight
//	uniform = uniform_ss->getOrCreateUniform("lightPos",crUniform::FLOAT_VEC3);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(sun->getLSPosition());
//
//	//uniform = uniform_ss->getOrCreateUniform("lightColor",crUniform::FLOAT_VEC4);
//	//uniform->setDataVariance(crBase::STATIC);
//	//uniform->set(sun->getLSColor());
//
//	uniform = uniform_ss->getOrCreateUniform("lightParam",crUniform::FLOAT_MAT4);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(sun->getLSParam());
//}
//
//void LightCallback::getUniforms_direct(crStateSet *uniform_ss,crLightSource *ls,bool calcRts)
//{
//	crUniform *uniform;
//	uniform = uniform_ss->getOrCreateUniform("lightPos",crUniform::FLOAT_VEC3);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(ls->getLSPosition());
//	//uniform = uniform_ss->getOrCreateUniform("lightColor",crUniform::FLOAT_VEC4);
//	//uniform->setDataVariance(crBase::STATIC);
//	//uniform->set(ls->getLSColor());
//	//uniform = uniform_ss->getOrCreateUniform("lightAttenuation",crUniform::FLOAT_VEC3);
//	//uniform->setDataVariance(crBase::STATIC);
//	//const crVector4f &atten = ls->getLSAttenuations();
//	//uniform->set(crVector3f(atten[0],atten[1],atten[2]));
//
//	uniform = uniform_ss->getOrCreateUniform("lightParam",crUniform::FLOAT_MAT4);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(ls->getLSParam());
//
//	uniform = uniform_ss->getOrCreateUniform("loweastLum",crUniform::FLOAT);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(ls->getLoweastLum());
//
//	if(calcRts)
//	{
//		uniform = uniform_ss->getOrCreateUniform("lightDir",crUniform::FLOAT_VEC3);
//		uniform->setDataVariance(crBase::STATIC);
//		uniform->set(ls->getLSDirection());
//		uniform = uniform_ss->getOrCreateUniform("cosLightFov",crUniform::FLOAT);
//		uniform->setDataVariance(crBase::STATIC);
//		uniform->set(ls->getCosLightMapFov());
//	}
//}
//
//void LightCallback::getUniforms_spot(crStateSet *uniform_ss,crLightSource *ls)
//{
//	crUniform *uniform;
//	uniform = uniform_ss->getOrCreateUniform("lightPos",crUniform::FLOAT_VEC3);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(ls->getLSPosition());
//	//uniform = uniform_ss->getOrCreateUniform("lightColor",crUniform::FLOAT_VEC4);
//	//uniform->setDataVariance(crBase::STATIC);
//	//uniform->set(ls->getLSColor());
//	uniform = uniform_ss->getOrCreateUniform("lightDir",crUniform::FLOAT_VEC3);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(ls->getLSDirection());
//	//uniform = uniform_ss->getOrCreateUniform("lightAttenuation",crUniform::FLOAT_VEC4);
//	//uniform->setDataVariance(crBase::STATIC);
//	//uniform->set(ls->getLSAttenuations());
//	uniform = uniform_ss->getOrCreateUniform("lightParam",crUniform::FLOAT_MAT4);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(ls->getLSParam());
//
//	uniform = uniform_ss->getOrCreateUniform("cosLightFov",crUniform::FLOAT);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(ls->getCosLightMapFov());
//
//	uniform = uniform_ss->getOrCreateUniform("loweastLum",crUniform::FLOAT);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(ls->getLoweastLum());
//}
//
//void LightCallback::getUniforms_rts(crStateSet *uniform_ss,crLightSource *ls)
//{
//	crUniform *uniform;
//	//sunlight shadow
//	uniform = uniform_ss->getOrCreateUniform("lightMVP",crUniform::FLOAT_MAT4);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(ls->getLightMapMVP());
//	uniform_ss->setTextureAttributeAndModes(TEXTURE_SHADOWMAP,ls->getLightMap(),crStateAttribute::ON);
//	uniform = uniform_ss->getOrCreateUniform("scale",crUniform::FLOAT);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(ls->getLightRangeScale());
//	uniform = uniform_ss->getOrCreateUniform("inv_shadowmap_size",crUniform::FLOAT);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(ls->getInvShadowmapSize());
//	//CRCore::notify(CRCore::ALWAYS)<<"ls->getInvShadowmapSize() = "<<ls->getInvShadowmapSize()<<std::endl;
//	if(ls->getLightIsSunLight())
//	{
//		uniform = uniform_ss->getOrCreateUniform("sqrSunShadowDistance",crUniform::FLOAT);
//		uniform->setDataVariance(crBase::STATIC);
//		float d = crLightSourceManager::getInstance()->getSunShadowDistance()-crShaderManager::getInstance()->getSunShadowDistanceOffset();
//		uniform->set(d * d);
//	}
//}
//
////
//bool LightCallback::operator()(const crObject *obj,crDrawable* drawable, const crBoundingBox &bbox, float depth, bool effectByShadow, crShaderManager::StateSetVec &statesetVec)
//{//需要返回两个stateset，一个是shader一个是该drawable的uniform，将drawable的material元素加入到uniformstateset，避免修改drawable的stateset。
//
//	bool doNotNeedDrawableStateSet = false;
//
//	crLightSourceManager::SelectLightMap &selectLightMap = crLightSourceManager::getInstance()->selectLightByBoundingBox(obj,bbox);
//
//	crStateSet *drawable_ss = drawable->getStateSet();
//	if(drawable_ss == NULL)
//		drawable_ss = const_cast<crStateSet *>(obj->getStateSet());
//	if(drawable_ss == NULL)
//	{
//		CRCore::notify(CRCore::FATAL)<<"LightCallback::operator() not set stateset nodeid = "<<obj->getName()<<std::endl;
//		return true;
//	}
//
//	crTexture *texture = dynamic_cast<crTexture *>(drawable_ss->getTextureAttribute(TEXTURE_BUMPMAP,crStateAttribute::TEXTURE));
//	int bump = 0;
//	if(texture)
//	{
//		bump = texture->getUseParallax()?2:1;
//	}
//	bool hasLightmap = drawable_ss->getTextureAttribute(TEXTURE_LIGHTMAP,crStateAttribute::TEXTURE)!=0;
//	bool hasEnvmap = drawable_ss->getTextureAttribute(TEXTURE_ENVMAP,crStateAttribute::TEXTURE)!=0;
//	const crStateSet *obj_ss = obj->getStateSet();
//	if(!hasEnvmap && obj_ss)
//	{
//		hasEnvmap = obj_ss->getTextureAttribute(TEXTURE_ENVMAP,crStateAttribute::TEXTURE)!=0;
//	}
//	crTexture *emissiveTexture = dynamic_cast<crTexture*>(drawable_ss->getTextureAttribute(TEXTURE_EMISSIVEMAP,crStateAttribute::TEXTURE));
//	bool hasEmissive = emissiveTexture!=0;
//	bool hasDiffuse = drawable_ss->getTextureAttribute(TEXTURE_DIFFUSEMAP,crStateAttribute::TEXTURE)!=0;
//
//	crTexture *specularTex = dynamic_cast<crTexture *>(drawable_ss->getTextureAttribute(TEXTURE_SPECULARMAP,crStateAttribute::TEXTURE));
//	int specular = 0;
//	if(specularTex)
//	{
//		specular = specularTex->getAlphaShininessEnable()?4:3;
//	}
//
//	crStateSet *shader_ss;
//	ref_ptr<crStateSet> uniform_ss;
//	std::string str;
//
//	crLight *light;
//
//	//crMaterial *material = dynamic_cast<crMaterial *>(drawable->getStateSet()->getAttribute(crStateAttribute::MATERIAL));
//	//StateSetBuf::iterator ssItr;
//	//StateSetBuf& currentActiveStateSets = crShaderManager::getInstance()->getCurrentActiveStateSets();
//
//	int size = selectLightMap.size();
////	crUniform *uniform;
//	if(size == 0)
//	{
//		uniform_ss = crShaderManager::getInstance()->getOrCreateIdleStateSet();
//		doNotNeedDrawableStateSet = true;
//		////push ambient
//		if(hasEmissive)
//		{
//			shader_ss = crShaderManager::getInstance()->getShaderStateSet("ambient_emissive");
//			uniform_ss->setTextureAttributeAndModes(TEXTURE_EMISSIVEMAP,emissiveTexture,crStateAttribute::ON);
//		}
//		else 
//		{
//			shader_ss = crShaderManager::getInstance()->getShaderStateSet("ambient");
//			uniform_ss->setAttributeAndModes(drawable->getStateSet()->getAttribute(crStateAttribute::MATERIAL),crStateAttribute::ON);
//			//uniform = uniform_ss->getOrCreateUniform("emissionColor",crUniform::FLOAT_VEC4);
//			//uniform->set(material->getEmission(crMaterial::FRONT));
//		}
//		//ssItr = currentActiveStateSets.find(uniform_ss.get());
//		//if(ssItr != currentActiveStateSets.end())
//		//	uniform_ss = ssItr->get();
//		//else
//		//	currentActiveStateSets.insert(uniform_ss.get());
//		if(uniform_ss.valid()) crShaderManager::getInstance()->pushActiveStateSet(uniform_ss.get());
//
//		statesetVec.push_back(std::make_pair(shader_ss,uniform_ss.get()));
//	}
//	else
//	{
//		crLightSource *ls;
//		crLightSource *sun = crLightSourceManager::getInstance()->getSunLightSource();
//
//		for( crLightSourceManager::SelectLightMap::iterator litr = selectLightMap.begin();
//			litr != selectLightMap.end();
//			)
//		{
//			ls = litr->second;
//			//effectByShadow = ls->getCalcShadow();//effectByShadow &= ls->getCalcShadow();
//			light = dynamic_cast<crLight *>(ls->getLight());
//			if(light->getLightType() == crLight::LIGHT_LOCAL)
//				str = "direct";
//			else str = "spot";
//
//			getProgramString(str,bump,hasLightmap,effectByShadow & ls->getCalcShadow(),hasEnvmap,hasEmissive,hasDiffuse,specular);
//
//			uniform_ss = crShaderManager::getInstance()->getOrCreateIdleStateSet();
//
//			++litr;
//			if(!sun && litr == selectLightMap.end()) 
//			{//最后一个光源
//				str = "includeAmb_" + str;
//			}
//			
//			shader_ss = crShaderManager::getInstance()->getShaderStateSet(str);
//
//			bool calcRts = str.find("rts")!=-1;
//			if(str.find("direct")!=-1)
//			{
//				getUniforms_direct(uniform_ss.get(),ls,calcRts);
//			}
//			if(str.find("spot")!=-1)
//			{	
//				getUniforms_spot(uniform_ss.get(),ls);
//			}
//			if(calcRts)
//			{
//				getUniforms_rts(uniform_ss.get(),ls);
//			}
//			///build material
//			//getUniforms_material(uniform_ss.get(),material);
//
//			//ssItr = currentActiveStateSets.find(uniform_ss.get());
//			//if(ssItr != currentActiveStateSets.end())
//			//	uniform_ss = ssItr->get();
//			//else
//			//	currentActiveStateSets.insert(uniform_ss.get());
//			if(uniform_ss.valid()) crShaderManager::getInstance()->pushActiveStateSet(uniform_ss.get());
//
//			//if(!shader_ss || !uniform_ss.valid())
//			//	CRCore::notify(CRCore::WARN) << "LightCallback::operator() error shader_ss = "<<shader_ss<<" uniform_ss = "<<uniform_ss.valid()<<" shaderStr = "<<str<<std::endl;
//			statesetVec.push_back(std::make_pair(shader_ss,uniform_ss.get()));
//		}
//        if(sun)
//		{
//            str = "sun";
//			//effectByShadow = sun->getCalcShadow();//effectByShadow &= sun->getCalcShadow();
//			getProgramString(str,bump,hasLightmap,effectByShadow & sun->getCalcShadow(),hasEnvmap,hasEmissive,hasDiffuse,specular);
//			shader_ss = crShaderManager::getInstance()->getShaderStateSet(str);
//			uniform_ss = crShaderManager::getInstance()->getOrCreateIdleStateSet();
//			getUniforms_sun(uniform_ss.get(),sun);
//			if(str.find("rts")!=-1)
//			{
//				getUniforms_rts(uniform_ss.get(),sun);
//			}
//			//ssItr = currentActiveStateSets.find(uniform_ss.get());
//			//if(ssItr != currentActiveStateSets.end())
//			//	uniform_ss = ssItr->get();
//			//else
//			//	currentActiveStateSets.insert(uniform_ss.get());
//			if(uniform_ss.valid()) crShaderManager::getInstance()->pushActiveStateSet(uniform_ss.get());
//
//			//if(!shader_ss || !uniform_ss.valid())
//			//	CRCore::notify(CRCore::WARN) << "LightCallback::operator() error shader_ss = "<<shader_ss<<" uniform_ss = "<<uniform_ss.valid()<<" shaderStr = "<<str<<std::endl;
//			statesetVec.push_back(std::make_pair(shader_ss,uniform_ss.get()));
//		}
//	}
//
//	return doNotNeedDrawableStateSet;
//}
//
//void WaterCallback::getProgramString(std::string &str,bool hasColor,bool hasLightmap,bool effectByShadow,bool hasEnvmap,bool hasEmissive,bool hasDiffuse,int specular,bool fadeIn, bool fadeOut)
//{
//	if(hasColor)
//		str += "_color";
//	if(hasLightmap)
//		str += "_lm";
//	if(effectByShadow)
//		str += "_rts";
//	if(hasEnvmap)
//		str += "_env";
//	if(hasEmissive)
//		str += "_emissive";
//	if(hasDiffuse)
//		str += "_diffuse";
//	if(specular == 3)
//		str += "_specular3";
//	else if(specular==4)
//		str += "_specular4";
//	if(fadeIn)
//		str += "_fi";
//	if(fadeOut)
//		str += "_fo";
//}
//
////void WaterCallback::getUniforms_water(crStateSet *uniform_ss,crLightSource *ls)
////{//从comments加载
////	//uniform vec3 waveScale;
////	//uniform vec3 waveSpeed; //waveSpeed.z = noiseSpeed
////    //uniform vec4 waterColor;
////	//uniform float waveAmp;
////}
//
//void WaterCallback::getUniforms_sun(crStateSet *uniform_ss,crLightSource *sun)
//{
//	crUniform *uniform;
//	//sunlight
//	uniform = uniform_ss->getOrCreateUniform("lightPos",crUniform::FLOAT_VEC3);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(sun->getLSPosition());
//
//	//uniform = uniform_ss->getOrCreateUniform("lightColor",crUniform::FLOAT_VEC4);
//	//uniform->setDataVariance(crBase::STATIC);
//	//uniform->set(sun->getLSColor());
//	uniform = uniform_ss->getOrCreateUniform("lightParam",crUniform::FLOAT_MAT4);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(sun->getLSParam());
//}
//
//void WaterCallback::getUniforms_rts(crStateSet *uniform_ss,crLightSource *ls)
//{
//	crUniform *uniform;
//	//sunlight shadow
//	uniform = uniform_ss->getOrCreateUniform("lightMVP",crUniform::FLOAT_MAT4);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(ls->getLightMapMVP());
//	uniform_ss->setTextureAttributeAndModes(TEXTURE_SHADOWMAP,ls->getLightMap(),crStateAttribute::ON);
//	uniform = uniform_ss->getOrCreateUniform("scale",crUniform::FLOAT);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(ls->getLightRangeScale());
//	uniform = uniform_ss->getOrCreateUniform("inv_shadowmap_size",crUniform::FLOAT);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(ls->getInvShadowmapSize());
//	if(ls->getLightIsSunLight())
//	{
//		uniform = uniform_ss->getOrCreateUniform("sqrSunShadowDistance",crUniform::FLOAT);
//		uniform->setDataVariance(crBase::STATIC);
//		float d = crLightSourceManager::getInstance()->getSunShadowDistance()-crShaderManager::getInstance()->getSunShadowDistanceOffset();
//		uniform->set(d * d);
//	}
//}
//
//bool WaterCallback::operator()(const crObject *obj,crDrawable* drawable, const crBoundingBox &bbox, float depth, bool effectByShadow, crShaderManager::StateSetVec &statesetVec)
//{//需要返回两个stateset，一个是shader一个是该drawable的uniform，将drawable的material元素加入到uniformstateset，避免修改drawable的stateset。
//	float radius = bbox.radius();
//	if(effectByShadow)
//	{
//		if(depth - 0.5 * radius/*maximum(bbox.xLength(),bbox.yLength())*/ >= crLightSourceManager::getInstance()->getSunShadowDistance())
//		{
//			effectByShadow = false;
//		}
//	}
//
//	crStateSet *drawable_ss = drawable->getStateSet();
//	if(drawable_ss == NULL)
//		drawable_ss = const_cast<crStateSet *>(obj->getStateSet());
//	if(drawable_ss == NULL)
//	{
//		CRCore::notify(CRCore::FATAL)<<"LightCallback::operator() not set stateset nodeid = "<<obj->getName()<<std::endl;
//		return true;
//	}
//
//    bool hasColormap = drawable_ss->getTextureAttribute(TEXTURE_BASEMAP,crStateAttribute::TEXTURE)!=0;
//	bool hasEmissive = 0;
//	bool hasDiffuse = 0;
//    int specular = 0;
//	if(hasColormap)
//	{
//		hasEmissive = drawable_ss->getTextureAttribute(TEXTURE_EMISSIVEMAP,crStateAttribute::TEXTURE)!=0;
//		hasDiffuse = drawable_ss->getTextureAttribute(TEXTURE_DIFFUSEMAP,crStateAttribute::TEXTURE)!=0;
//		crTexture *specularTex = dynamic_cast<crTexture *>(drawable_ss->getTextureAttribute(TEXTURE_SPECULARMAP,crStateAttribute::TEXTURE));
//		if(specularTex)
//		{
//			specular = specularTex->getAlphaShininessEnable()?4:3;
//		}
//	}
//	bool hasLightmap = drawable_ss->getTextureAttribute(TEXTURE_LIGHTMAP,crStateAttribute::TEXTURE)!=0;
//	bool hasEnvmap = drawable_ss->getTextureAttribute(TEXTURE_ENVMAP,crStateAttribute::TEXTURE)!=0;
//	const crStateSet *obj_ss = obj->getStateSet();
//	if(!hasEnvmap && obj_ss)
//	{
//		hasEnvmap = obj_ss->getTextureAttribute(TEXTURE_ENVMAP,crStateAttribute::TEXTURE)!=0;
//	}
//
//	crVector2 fi;
//	crVector2 fo;
//	bool fadeIn = false;
//	bool fadeOut = false;
//	crVector2f r(depth-radius,depth+radius);
//	r/=crBrain::getInstance()->getMaxDepth();
//	r[0] *= r[0];
//	r[1] *= r[1];
//	if(crArgumentParser::readKeyValue(obj->getDescriptions(),"FadeIn",fi))
//	{
//		if(r.x() < fi.y() || r.y() > fi.x())
//		{
//			//CRCore::notify(CRCore::ALWAYS)<<"NoLightCallback::operator(): FadeIn = "<<fi<<obj->getName()<<std::endl;
//			fadeIn = true;
//		}
//	}
//	if(crArgumentParser::readKeyValue(obj->getDescriptions(),"FadeOut",fo))
//	{
//		if(r.x() < fo.y() || r.y() > fo.x())
//		{
//			//CRCore::notify(CRCore::ALWAYS)<<"NoLightCallback::operator(): FadeOut = "<<fo<<obj->getName()<<std::endl;
//			fadeOut = true;
//		}
//	}
//	ref_ptr<crStateSet> uniform_ss;
//	if(fadeIn||fadeOut)
//	{
//		crUniform *uniform;
//		uniform_ss = crShaderManager::getInstance()->getOrCreateIdleStateSet();
//		if(fadeIn)
//		{
//			uniform = uniform_ss->getOrCreateUniform("fadeIn",crUniform::FLOAT_VEC2);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set(fi);
//		}
//		if(fadeOut)
//		{
//			uniform = uniform_ss->getOrCreateUniform("fadeOut",crUniform::FLOAT_VEC2);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set(fo);
//		}
//		uniform_ss->setAttributeAndModes(crShaderManager::getInstance()->getBlendFunc(),crStateAttribute::ON);
//		uniform_ss->setRenderingHint(crStateSet::TRANSPARENT_BIN);
//	}
//
//	crStateSet *shader_ss;
//	crLightSource *sun = crLightSourceManager::getInstance()->getSunLightSource();
//	effectByShadow &= sun->getCalcShadow();
//
//	std::string str = "water";
//	getProgramString(str,hasColormap,hasLightmap,effectByShadow,hasEnvmap,hasEmissive,hasDiffuse,specular,fadeIn,fadeOut);
//
//	//CRCore::notify(CRCore::ALWAYS)<<"SunCallback::operator():"<<str<<std::endl;
//
//	shader_ss = crShaderManager::getInstance()->getShaderStateSet(str);
//	if(!uniform_ss.valid()) uniform_ss = crShaderManager::getInstance()->getOrCreateIdleStateSet();
//
//	//crMaterial *material = dynamic_cast<crMaterial *>(drawable->getStateSet()->getAttribute(crStateAttribute::MATERIAL));
//
//	//getUniforms_water(uniform_ss.get(),sun);
//	getUniforms_sun(uniform_ss.get(),sun);
//
//	if(str.find("rts")!=-1)
//	{
//		getUniforms_rts(uniform_ss.get(),sun);
//	}
//	///build material
//	//getUniforms_material(uniform_ss.get(),material);
//
//	//if(uniform_ss.valid())
//	//{
//	//	StateSetBuf& currentActiveStateSets = crShaderManager::getInstance()->getCurrentActiveStateSets();
//	//	StateSetBuf::iterator itr = currentActiveStateSets.find(uniform_ss.get());
//	//	if(itr != currentActiveStateSets.end())
//	//		uniform_ss = itr->get();
//	//	else
//	//		currentActiveStateSets.insert(uniform_ss.get());
//	//}
//	if(uniform_ss.valid()) crShaderManager::getInstance()->pushActiveStateSet(uniform_ss.get());
//
//	statesetVec.push_back(std::make_pair(shader_ss,uniform_ss));
//
//	return false;
//}
///////////////////////
//
//NoLightCallback
//
////////////
//void NoLightCallback::getProgramString(std::string & shaderName,bool isEmissive, bool hasDetal,bool hasLightmap,bool hasEnvmap,bool fadeIn, bool fadeOut)
//{
//	if(isEmissive)
//		shaderName = "emissive";
//	else
//		shaderName = "diffuse";
//
//	if(hasDetal)
//		shaderName += "_detal";
//	if(hasLightmap)
//		shaderName += "_lm";
//	if(hasEnvmap)
//		shaderName += "_env";
//	if(fadeIn)
//		shaderName += "_fi";
//	if(fadeOut)
//		shaderName += "_fo";
//}

bool NoLightCallback::operator()(const crObject *obj,crDrawable* drawable, const crBoundingBox &bbox, float depth, bool effectByShadow, crShaderManager::StateSetVec &statesetVec)
{//需要返回两个stateset，一个是shader一个是该drawable的uniform，将drawable的material元素加入到uniformstateset，避免修改drawable的stateset。
	crStateSet *drawable_ss = drawable->getStateSet();
	if(drawable_ss == NULL)
		drawable_ss = const_cast<crStateSet *>(obj->getStateSet());
	if(drawable_ss == NULL)
	{
		//CRCore::notify(CRCore::FATAL)<<"LightCallback::operator() not set stateset nodeid = "<<obj->getName()<<std::endl;
		return true;
	}

	const crStateSet *obj_ss = obj->getStateSet();
	std::string statesetShaderStr;
	statesetShaderStr = drawable_ss->getRtglmShaderStr(obj_ss);

	//crTexture *texture = dynamic_cast<crTexture *>(drawable_ss->getTextureAttribute(TEXTURE_BASEMAP,crStateAttribute::TEXTURE));
	//bool isEmissive = false;
	//if(!texture)
	//{
	//	CRCore::notify(CRCore::WARN)<<"NoLightCallback::operator(): dose not have base texture obj = "<<obj->getName().c_str()<<std::endl;
	//}
	//if(texture->getImageNameID().find("_ec")!=-1)
 //       isEmissive = true;

	//bool hasDetal = drawable_ss->getTextureAttribute(TEXTURE_BUMPMAP,crStateAttribute::TEXTURE)!=0;
	//bool hasLightmap = drawable_ss->getTextureAttribute(TEXTURE_LIGHTMAP,crStateAttribute::TEXTURE)!=0;
	//bool hasEnvmap = drawable_ss->getTextureAttribute(TEXTURE_ENVMAP,crStateAttribute::TEXTURE)!=0;
	//const crStateSet *obj_ss = obj->getStateSet();
	//if(!hasEnvmap && obj_ss)
	//{
	//	hasEnvmap = obj_ss->getTextureAttribute(TEXTURE_ENVMAP,crStateAttribute::TEXTURE)!=0;
	//}

	crVector2 fi;
	crVector2 fo;
	bool fadeIn = false;
	bool fadeOut = false;
	float radius = bbox.radius();
	crVector2f r(depth-radius,depth+radius);
	r/=crBrain::getInstance()->getMaxDepth();
	r[0] *= r[0];
	r[1] *= r[1];
	if(crArgumentParser::readKeyValue(obj->getDescriptions(),"FadeIn",fi))
	{
		if(r.x() < fi.y() || r.y() > fi.x())
		{
			//CRCore::notify(CRCore::ALWAYS)<<"NoLightCallback::operator(): FadeIn = "<<fi<<obj->getName()<<std::endl;
			fadeIn = true;
		}
	}
	if(crArgumentParser::readKeyValue(obj->getDescriptions(),"FadeOut",fo))
	{
		if(r.x() < fo.y() || r.y() > fo.x())
		{
			//CRCore::notify(CRCore::ALWAYS)<<"NoLightCallback::operator(): FadeOut = "<<fo<<obj->getName()<<std::endl;
			fadeOut = true;
		}
	}
	//int noskylight = 0;
	//crArgumentParser::readKeyValue(obj->getDescriptions(),"NoSkyLight",noskylight);
	ref_ptr<crStateSet> uniform_ss;
	uniform_ss = crShaderManager::getInstance()->getOrCreateIdleStateSet();
	//sunlight
	crLightSource *sun = crLightSourceManager::getInstance()->getSunLightSource();
	crUniform *uniform;
	uniform = uniform_ss->getOrCreateUniform("lightPos",crUniform::FLOAT_VEC3);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(sun->getLSPosition());

    if(fadeIn||fadeOut)
	{
		if(fadeIn)
		{
			uniform = uniform_ss->getOrCreateUniform("fadeIn",crUniform::FLOAT_VEC2);
			uniform->setDataVariance(crBase::STATIC);
			uniform->set(fi);
		}
		if(fadeOut)
		{
			uniform = uniform_ss->getOrCreateUniform("fadeOut",crUniform::FLOAT_VEC2);
			uniform->setDataVariance(crBase::STATIC);
			uniform->set(fo);
		}
		uniform_ss->setAttributeAndModes(crShaderManager::getInstance()->getBlendFunc(),crStateAttribute::ON);
		uniform_ss->setRenderingHint(crStateSet::TRANSPARENT_BIN);
	}
	
	crStateSet *shader_ss;
	std::string str = "nolight";
	//getProgramString(str,isEmissive,hasDetal,hasLightmap,hasEnvmap,fadeIn,fadeOut);
	char acceptGI = obj->getAcceptGI();// && noskylight==0;
	//if (acceptGI == 0) acceptGI = 1;
	getProgramString(str,false,false,acceptGI,fadeIn,fadeOut);
	if(acceptGI==-1)
	{
		str+="_NoSkyLight";
	}
    str += statesetShaderStr;
	//CRCore::notify(CRCore::ALWAYS)<<"NoLightCallback::operator():"<<str<<std::endl;
	shader_ss = crShaderManager::getInstance()->getShaderStateSet(str);
	if(acceptGI==1)
	{
		if(crShaderManager::getInstance()->getGiTexture())
			getUniforms_gi(uniform_ss.get());
		else if(crShaderManager::getInstance()->getLightMapTexture())
			getUniforms_sgi(uniform_ss.get());
	}
	else if(acceptGI == 2)
	{
		if (crShaderManager::getInstance()->getGiTexture())
			getUniforms_gi(uniform_ss.get());
		if(crShaderManager::getInstance()->getHeightMapTexture())
			getUniforms_height(uniform_ss.get());
	}
	else if(crShaderManager::getInstance()->getLightMapTexture())
	{
		getUniforms_sgi(uniform_ss.get());
	}
	//if(uniform_ss.valid())
	//{
	//	StateSetBuf& currentActiveStateSets = crShaderManager::getInstance()->getCurrentActiveStateSets();
	//	StateSetBuf::iterator itr = currentActiveStateSets.find(uniform_ss.get());
	//	if(itr != currentActiveStateSets.end())
	//		uniform_ss = itr->get();
	//	else
	//		currentActiveStateSets.insert(uniform_ss.get());
	//}
	crShaderManager::getInstance()->pushActiveStateSet(uniform_ss.get());
	statesetVec.push_back(std::make_pair(shader_ss,uniform_ss));
	//statesetVec.push_back(crShaderManager::Shader_UniformPair(shader_ss,uniform_ss.get()));
	return false;
}
bool NoLightCallback::giMapRender(const crObject *obj, crDrawable* drawable, crShaderManager::StateSetVec &statesetVec)
{
	crStateSet *drawable_ss = drawable->getStateSet();
	if (drawable_ss == NULL)
		drawable_ss = const_cast<crStateSet *>(obj->getStateSet());
	if (drawable_ss == NULL)
	{
		//CRCore::notify(CRCore::FATAL)<<"LightCallback::operator() not set stateset nodeid = "<<obj->getName()<<std::endl;
		return true;
	}
	const crStateSet *obj_ss = obj->getStateSet();
	std::string statesetShaderStr;
	statesetShaderStr = drawable_ss->getRtglmShaderStr(obj_ss);
	ref_ptr<crStateSet> uniform_ss;
	uniform_ss = crShaderManager::getInstance()->getOrCreateIdleStateSet();
	crLightSource *sun = crLightSourceManager::getInstance()->getSunLightSource();
	crUniform *uniform;
	uniform = uniform_ss->getOrCreateUniform("lightPos", crUniform::FLOAT_VEC3);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(sun->getLSPosition());

	crStateSet *shader_ss;
	std::string str = "nolight_gimap";
	//getProgramString(str,isEmissive,hasDetal,hasLightmap,hasEnvmap,fadeIn,fadeOut);
	//char acceptGI = obj->getAcceptGI();// && noskylight==0;
	getProgramString(str, false, false, -2, false, false);
	str += statesetShaderStr;
	shader_ss = crShaderManager::getInstance()->getShaderStateSet(str);
	//if (crShaderManager::getInstance()->getLightMapTexture())
	//{
	//	getUniforms_sgi(uniform_ss.get());
	//}
	
	uniform_ss->setMode(GL_BLEND, crStateAttribute::OFF | crStateAttribute::OVERRIDE);
	uniform_ss->setMode(GL_ALPHA_TEST, crStateAttribute::OFF | crStateAttribute::OVERRIDE);
	const crVector3i &worldSize = crBrain::getInstance()->getWorldSize();
	uniform = uniform_ss->getOrCreateUniform("maxheight", CRCore::crUniform::FLOAT);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set((float)worldSize[2]);

	crShaderManager::getInstance()->pushActiveStateSet(uniform_ss.get());
	statesetVec.push_back(std::make_pair(shader_ss, uniform_ss));
	return false;
}
void NoLightCallback::getUniforms_gi(crStateSet *uniform_ss)
{
	crUniform *uniform;
	uniform_ss->setTextureAttributeAndModes(TEXTURE_GIMAP,crShaderManager::getInstance()->getGiTexture(),crStateAttribute::ON);

	uniform = uniform_ss->getOrCreateUniform("giparam",crUniform::FLOAT_VEC4);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(crShaderManager::getInstance()->getGIParam());

	crLightSource *sun = crLightSourceManager::getInstance()->getSunLightSource();
	uniform = uniform_ss->getOrCreateUniform("sunambient", crUniform::FLOAT_VEC3);
	uniform->setDataVariance(crBase::STATIC);
	const crMatrixf &lsparam = sun->getLSParam();
	float sunattenuation = 1.0 / lsparam(3, 0);
	crVector3 sunambient = crVector3(lsparam(0, 0), lsparam(0, 1), lsparam(0, 2)) * sunattenuation;
	uniform->set(sunambient);
}
void NoLightCallback::getUniforms_sgi(crStateSet *uniform_ss)
{
	crUniform *uniform;
	uniform_ss->setTextureAttributeAndModes(TEXTURE_LIGHTMAP,crShaderManager::getInstance()->getLightMapTexture(),crStateAttribute::ON);

	uniform = uniform_ss->getOrCreateUniform("giparam",crUniform::FLOAT_VEC4);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(crShaderManager::getInstance()->getGIParam());
}
void NoLightCallback::getUniforms_height(crStateSet *uniform_ss)
{
	crUniform *uniform;
	uniform_ss->setTextureAttributeAndModes(TEXTURE_LIGHTMAP,crShaderManager::getInstance()->getHeightMapTexture(),crStateAttribute::ON);

	uniform = uniform_ss->getOrCreateUniform("giparam",crUniform::FLOAT_VEC4);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(crShaderManager::getInstance()->getGIParam());

	const crVector3i &worldSize = crBrain::getInstance()->getWorldSize();
	uniform = uniform_ss->getOrCreateUniform("maxheight",CRCore::crUniform::FLOAT);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set((float)worldSize[2]);
}