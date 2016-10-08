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
#ifndef CRCORE_CRSHADERMANAGER
#define CRCORE_CRSHADERMANAGER 1

#include <CRCore/crBase.h>
#include <CRCore/crBrain.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crDrawable.h>
#include <CRCore/crObject.h>
#include <CRCore/crLightSource.h>
#include <CRCore/crMaterial.h>
#include <CRCore/crAlphaFunc.h>
#include <CRCore/crBlendFunc.h>
#include <CRCore/crNotify.h>
#include <CRCore/crDisplaySettings.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/thread/crScopedLock.h>
#include <map>
#include <hash_map>
#include <vector>
#include <string>
#include <deque>

namespace CRCore {

class CR_EXPORT crShaderManager : public CRCore::Referenced
{
public :
	crShaderManager();
	static crShaderManager *getInstance();
    
	typedef std::map< std::string, CRCore::ref_ptr<CRCore::crStateSet> > ShaderStateSetMap;
	typedef stdext::hash_map< std::string, CRCore::ref_ptr<CRCore::crStateSet> > ShaderStateSetHashMap;

	void insertShaderStateSet( crStateSet *stateset );
    ShaderStateSetMap &getShaderStateSetMap();

    typedef std::pair< CRCore::ref_ptr<CRCore::crStateSet>, CRCore::ref_ptr<CRCore::crStateSet> > Shader_UniformPair;
    typedef std::vector< Shader_UniformPair > StateSetVec;
	inline bool getShaderStateSetVec(const std::string &ts, const crObject *obj,crDrawable* drawable, const crBoundingBox &bbox, float depth, bool effectByShadow, crShaderManager::StateSetVec &statesetVec)
	{
		Callback *callback = getCallback(ts);
		if(callback) 
			return (*callback)(obj,drawable,bbox,depth,effectByShadow,statesetVec);
		return false;
	}
	
	bool getShadowShaderStateSetVec(const crObject *obj, crDrawable* drawable, crShaderManager::StateSetVec &statesetVec );
	
	inline crStateSet *getShaderStateSet( const std::string& name )
	{
		//CRCore::ScopedLock<crMutex> lock(m_mutex);
		if(!m_sssmapbuf.empty())
		{
			ShaderStateSetHashMap::iterator itr = m_sssmapbuf.find(name);
			if(itr != m_sssmapbuf.end()) return itr->second.get();
		}
        ShaderStateSetMap::iterator itr = m_shaderStateSetMap.find(name);
		if(itr != m_shaderStateSetMap.end())
		{
			//m_sssmapbuf.insert(std::make_pair(itr->first,itr->second));
			m_sssmapbuf[itr->first] = itr->second;
			//CRCore::notify(CRCore::WARN) << "crShaderManager m_sssmapbuf = "<<m_sssmapbuf.size()<<std::endl;
			//CRCore::notify(CRCore::WARN) << "crShaderManager m_shaderStateSetMap = "<<m_shaderStateSetMap.size()<<std::endl;
            return itr->second.get();
		}
		//CRCore::notify(CRCore::WARN)<<"crShaderManager::getShaderStateSet(): not find shaderstr = "<<name<<std::endl;
		//needload
		return loadShader(name);
	}

	void releaseObjects(crState* state);
	class Callback : public virtual CRCore::Referenced 
	{
	public:
		Callback() {}
		virtual bool operator()(const crObject *obj,crDrawable* drawable, const crBoundingBox &bbox, float depth, bool effectByShadow, crShaderManager::StateSetVec &statesetVec) = 0;
		virtual bool giMapRender(const crObject *obj, crDrawable* drawable, crShaderManager::StateSetVec &statesetVec){ return false; }
	protected:
		virtual ~Callback(){}
	    virtual inline void getProgramString(std::string &str,bool effectByShadow, bool effectByShadow2 = false, char acceptGI = 0, bool fadeIn = false, bool fadeOut = false)
		{
			if(effectByShadow)
			{
				str += "_rts";
				if(effectByShadow2)
					str += "_rts2";
				if(crDisplaySettings::instance()->getShadowDetail()>=3)
					str += "_h";
			}
			if(acceptGI==1)
			{
				if(crShaderManager::getInstance()->getGiTexture())
					str += "_gi";
				if(crShaderManager::getInstance()->getLightMapTexture())
					str += "_sgi";
			}
			else if(acceptGI==2)
			{
				if(crShaderManager::getInstance()->getGiTexture())
					str += "_gi";
				if (crShaderManager::getInstance()->getHeightMapTexture())
					str += "_hgi";
				//if(crShaderManager::getInstance()->getHeightMapTexture())
				//	str += "_hgi";
				//else if(crShaderManager::getInstance()->getLightMapTexture())
				//	str += "_sgi";
			}
			else if(acceptGI!=-2 && crShaderManager::getInstance()->getLightMapTexture())
			{
				str += "_sgi";
			}
			if(fadeIn)
				str += "_fi";
			if(fadeOut)
				str += "_fo";
		}
	};
    typedef stdext::hash_map< std::string, CRCore::ref_ptr<Callback> > CallbackMap;//可以改成hashmap
	inline Callback *getCallback(const std::string &ts)
	{
		CallbackMap::iterator itr = m_shaderCallbackMap.find(ts);
		if(itr != m_shaderCallbackMap.end()) return itr->second.get();
		else return NULL;
	}
	void insertCallback(const std::string &ts, Callback *callback);

	inline void resetCulling()
	{//每帧culling之前都需调用一次
		//m_currentActiveStateSets->clear();
        //CRCore::notify(CRCore::WARN) << "crShaderManager currentActiveStateSets = "<<m_currentActiveStateSets->size()<<std::endl;
		m_currentActiveStateSets->resize(0);
		m_currentReuseStateSetIndex = 0;//delete 20140421
	}

	typedef std::vector< CRCore::ref_ptr<CRCore::crStateSet> > StateSetBuf;
	inline void swapBuffers(int frameNumber)
	{
		//crStateSet::UniformList::iterator uitr;
		//for( StateSetBuf::iterator itr = m_currentActiveStateSets->begin();
		//	itr != m_currentActiveStateSets->end();
		//	++itr )
		//{
		//	(*itr)->swapBuffers(frameNumber);
		//}

		//StateSetBuf *temp;
		StateSetBuf *temp;
		temp = m_currentActiveStateSets;
		m_currentActiveStateSets = m_currentActiveStateSets_buf;
		m_currentActiveStateSets_buf = temp;
	}

	inline crStateSet *getOrCreateIdleStateSet()
	{//复用有可能造成不稳定
		//modify 20140421
		while (m_currentReuseStateSetIndex<m_buf.size() && 
			m_buf[m_currentReuseStateSetIndex]->referenceCount()>1)
		{
			//CRCore::notify(CRCore::WARN)<<"Warning:createOrReuseRenderLeaf() skipping multiply refrenced entry."<< std::endl;
			++m_currentReuseStateSetIndex;
		}
		if (m_currentReuseStateSetIndex<m_buf.size())
		{
			crStateSet *idle = m_buf[m_currentReuseStateSetIndex++].get();
			idle->reset();
			return idle;
		}

		crStateSet *idle = new crStateSet;
		idle->setName("Shader");
		m_buf.push_back(idle);
		++m_currentReuseStateSetIndex;
		//CRCore::notify(CRCore::WARN) << "getOrCreateIdleStateSet = "<<m_buf.size()<<" currentReuseStateSetIndex = "<<m_currentReuseStateSetIndex<<std::endl;
		return idle;

		//return new crStateSet;
	}

	inline /*crBrain::StateSetSet*/StateSetBuf& getCurrentActiveStateSets() { return *m_currentActiveStateSets; }
	//inline crMaterial *getDefaultMaterial() { return m_defaultMaterial.get(); }
	void registAlphaFunc(crAlphaFunc *alphaFunc);
	void registBlendFunc(crBlendFunc *blendFunc);
	inline crAlphaFunc *getAlphaFunc() { return m_alphaFunc.get(); }
    inline crBlendFunc *getBlendFunc() { return m_blendFunc.get(); }

	virtual void clear();
	inline void pushActiveStateSet(crStateSet *ss) {m_currentActiveStateSets->push_back(ss);}

	void setSunShadowDistanceOffset( const crVector2 &offset );
	inline const crVector2& getSunShadowDistanceOffset() { return m_sunShadowDistanceOffset; }

	void setSunShadowDistance(const crVector2 &sunShadowDistance);
	inline const crVector2 &getSunShadowDistance(){ return m_sunShadowDistance; }

	typedef std::vector<std::string> StrVec;
	void pushString(const std::string &str);

	typedef std::pair< CRCore::ref_ptr<crStateSet>, CRCore::ref_ptr<crStateSet> > ShadowStateSetPair;
	typedef std::map< ShadowStateSetPair, ShadowStateSetPair > ShadowStateSetMap;//objss,drawabless,uniform_ss,shader_ss
	void setGIParam(const crVector4& giparam);
	//void setGIMVP(const crMatrix& gimvp);
	void setGITexture(crTexture *tex);
	//void setUpperSkyTexture(crTexture *tex);
	void setLightMapTexture(crTexture *tex);
	void setHeightMapTexture(crTexture *tex);
	inline crTexture *getGiTexture(){ return m_giTexture.get(); }
	//inline crTexture *getUpperSkyTexture(){ return m_upperSkyTexture.get(); }
	inline crTexture *getLightMapTexture(){ return m_lightMapTexture.get(); }
	inline crTexture *getHeightMapTexture(){ return m_heightMapTexture.get(); }
	inline const crVector4& getGIParam(){ return m_giparam; }
	//inline const crMatrix& getGIMVP(){ return m_giMVP; }
protected :

    virtual ~crShaderManager();
	static ref_ptr<crShaderManager> m_instance;

	crStateSet* loadShader(const std::string &name);
	crStateSet* createShader(const std::string& shaderName,const std::string& vp, const std::string& fp, bool addT, bool addB);
	std::string getLightHead(const std::string &name, int mode = 0);//0:rtglm 1:rtplm 2:nolight
	std::string getShadowName(const std::string &statesetShaderStr,crStateSet *shadow_ss,crStateSet *drawable_ss, crStateSet *obj_ss);
	std::string getShadowHead(const std::string &name);

	ShaderStateSetMap m_shaderStateSetMap;
	ShaderStateSetHashMap m_sssmapbuf;

	CallbackMap m_shaderCallbackMap;

	StateSetBuf *m_currentActiveStateSets;
	StateSetBuf *m_currentActiveStateSets_buf;
	//ref_ptr<crMaterial> m_defaultMaterial;
	ref_ptr<crAlphaFunc> m_alphaFunc;
	ref_ptr<crBlendFunc> m_blendFunc;

	StateSetBuf m_buf;//getOrCreateIdleStateSet //delete 20140421
	int m_currentReuseStateSetIndex;//delete 20140421
	CRCore::crVector2 m_sunShadowDistanceOffset;
	CRCore::crVector2 m_sunShadowDistance;
	StrVec m_stringVec;
	crMutex                  m_mutex;
	ShadowStateSetMap m_shadowStateSetMap;
	ref_ptr<crTexture> m_giTexture;
	//ref_ptr<crTexture> m_upperSkyTexture;
	ref_ptr<crTexture> m_lightMapTexture;
	ref_ptr<crTexture> m_heightMapTexture;
	crVector4 m_giparam;
	//crMatrix m_giMVP;
};

class SunCallback : public crShaderManager::Callback
{
public:
	SunCallback();
	virtual bool operator()(const crObject *obj,crDrawable* drawable, const crBoundingBox &bbox, float depth, bool effectByShadow, crShaderManager::StateSetVec &statesetVec);
	virtual bool giMapRender(const crObject *obj, crDrawable* drawable, crShaderManager::StateSetVec &statesetVec);
protected:
	void getUniforms_sun(crStateSet *uniform_ss,crLightSource *sun);
	void getUniforms_sun(crStateSet *uniform_ss,crLightSource *sun, const crVector3 &center);
	void getUniforms_spot(crStateSet *uniform_ss,crLightSource *ls,int lightNum);
	void getUniforms_direct(crStateSet *uniform_ss,crLightSource *ls,int lightNum);
	void getUniforms_rts(crStateSet *uniform_ss,crLightSource *ls,bool acceptShadow2);
	void getUniforms_gi(crStateSet *uniform_ss);
	void getUniforms_sgi(crStateSet *uniform_ss);
	void getUniforms_height(crStateSet *uniform_ss);
	//void getUniforms_material(crStateSet *uniform_ss,crMaterial *material);
	//typedef std::map< std::string, CRCore::ref_ptr<CRCore::crUniform> > UniformMap;
	//UniformMap m_reuseUniformList;
	CRCore::ref_ptr<Callback> m_appendLightCallback;
};

class NeedLightCallback : public crShaderManager::Callback
{
public:
	virtual bool operator()(const crObject *obj,crDrawable* drawable, const crBoundingBox &bbox, float depth, bool effectByShadow, crShaderManager::StateSetVec &statesetVec);
protected:
	void getUniforms_spot(crStateSet *uniform_ss,crLightSource *ls,int lightNum);
	void getUniforms_direct(crStateSet *uniform_ss,crLightSource *ls,int lightNum);
};

//class LightCallback : public crShaderManager::Callback
//{
//public:
//	virtual bool operator()(const crObject *obj,crDrawable* drawable, const crBoundingBox &bbox, float depth, bool effectByShadow, crShaderManager::StateSetVec &statesetVec);
//protected:
//	void getUniforms_sun(crStateSet *uniform_ss,crLightSource *sun);
//	void getUniforms_direct(crStateSet *uniform_ss,crLightSource *ls,bool calcRts);
//	void getUniforms_spot(crStateSet *uniform_ss,crLightSource *ls);
//	void getUniforms_rts(crStateSet *uniform_ss,crLightSource *ls);
//	//void getUniforms_material(crStateSet *uniform_ss,crMaterial *material);
//};

//class WaterCallback : public crShaderManager::Callback
//{
//public:
//	virtual bool operator()(const crObject *obj,crDrawable* drawable, const crBoundingBox &bbox, float depth, bool effectByShadow, crShaderManager::StateSetVec &statesetVec);
//protected:
//	void getProgramString(std::string &str,bool hasColor,bool hasLightmap,bool effectByShadow,bool hasEnvmap,bool hasEmissive,bool hasDiffuse,int specular,bool fadeIn, bool fadeOut);
//	//void getUniforms_water(crStateSet *uniform_ss,crLightSource *ls);
//	void getUniforms_sun(crStateSet *uniform_ss,crLightSource *sun);
//	void getUniforms_rts(crStateSet *uniform_ss,crLightSource *ls);
//	//void getUniforms_material(crStateSet *uniform_ss,crMaterial *material);
//};

class NoLightCallback : public crShaderManager::Callback
{
public:
	virtual bool operator()(const crObject *obj,crDrawable* drawable, const crBoundingBox &bbox, float depth, bool effectByShadow, crShaderManager::StateSetVec &statesetVec);
	virtual bool giMapRender(const crObject *obj, crDrawable* drawable, crShaderManager::StateSetVec &statesetVec);
protected:
	void getUniforms_gi(crStateSet *uniform_ss);
	void getUniforms_sgi(crStateSet *uniform_ss);
	void getUniforms_height(crStateSet *uniform_ss);
	//void getProgramString(std::string & shaderName,bool isEmissive, bool hasDetal,bool hasLightmap,bool hasEnvmap,bool fadeIn, bool fadeOut);
};

}

#endif
