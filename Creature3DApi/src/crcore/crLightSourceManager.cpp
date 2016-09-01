/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
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
#include <CRCore/crLightSourceManager.h>
#include <CRCore/crBrain.h>
#include <CRCore/crMath.h>
#include <CRCore\crDisplaySettings.h>
#include <CRCore/thread/crScopedLock.h>

//+1sunlight
#define MaxOnePassLightNum 3

using namespace CRCore;
ref_ptr<crLightSourceManager> crLightSourceManager::m_instance = NULL;

//////////////////////////////////////////////////////////////////
crLightSourceManager::crLightSourceManager():
m_lightValidDistance(50.0f),
m_hasSunLight(true)
{
	//m_currentFrameEyeShadowLightSource = NULL;
	//m_sunShadowDistance = 40.0f;
	//m_hasSunLight = false;
	//createShadowCamera();
}
crLightSourceManager::~crLightSourceManager()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crLightSourceManager()"<< std::endl;
}
crLightSourceManager* crLightSourceManager::getInstance()
{
	if (m_instance==NULL)
	{
		m_instance = new crLightSourceManager;
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
	//static ref_ptr<crLightSourceManager> s_lsManager = new crLightSourceManager;
	//return s_lsManager.get();
}

void crLightSourceManager::clear()
{
	CRCore::ScopedLock<crMutex> lock(m_mutex);
	//m_instance = NULL;
	m_sunLightSource = NULL;
	m_lsVec.clear();
	m_hasSunLight = true;
	m_onePassLightVec.clear();
	m_appendLightVec.clear();
}

void crLightSourceManager::insertLightSource( CRCore::crLightSource *ls )
{
	CRCore::ScopedLock<crMutex> lock(m_mutex);
	crLight *light = dynamic_cast<crLight *>(ls->getLight());
	if(!light||light->getLightType() == crLight::LIGHT_INFINITE)
		return;

	if(ls->getLightIsSunLight())
	{
		m_sunLightSource = ls;
		//m_sunLightSource->setStdModeEnable(true);
		//return;
	}

	for( LSVec::iterator itr = m_lsVec.begin();
		 itr != m_lsVec.end();
		 ++itr )
	{
        if((*itr) == ls)
			return;
	}
  //  if(ls->getLightIsSunLight())
		//m_hasSunLight = true;
    m_lsVec.push_back(ls);
}

int crLightSourceManager::getNumLights()
{
    return m_lsVec.size();
}

crLightSourceManager::LSVec &crLightSourceManager::getLSVec()
{
    return m_lsVec;
}

crLightSource *crLightSourceManager::findLightSource(const std::string &name)
{
  //  if(m_sunLightSource.valid() && name.compare(m_sunLightSource->getName()) == 0)
		//return m_sunLightSource.get();
	CRCore::ScopedLock<crMutex> lock(m_mutex);

	for( LSVec::iterator itr = m_lsVec.begin();
		itr != m_lsVec.end();
		++itr )
	{
		if(name.compare((*itr)->getName()) == 0)
			return itr->get();
	}
	return NULL;
}

void crLightSourceManager::removeLightSource(CRCore::crLightSource *ls)
{
	CRCore::ScopedLock<crMutex> lock(m_mutex);
	for( LSVec::iterator itr = m_lsVec.begin();
		itr != m_lsVec.end();
		++itr )
	{
		if(itr->get() == ls)
		{
			m_lsVec.erase(itr);
			return;
		}
	}
}

void crLightSourceManager::removeLightSource(const std::string &name)
{
	CRCore::ScopedLock<crMutex> lock(m_mutex);
	for( LSVec::iterator itr = m_lsVec.begin();
		itr != m_lsVec.end();
		++itr )
	{
		if(name.compare((*itr)->getName()) == 0)
		{
			m_lsVec.erase(itr);
			return;
		}
	}
}

bool crLightSourceManager::getHasSunLight()
{
	return m_sunLightSource.valid();
}

crLightSource *crLightSourceManager::getSunLightSource()
{
    return m_sunLightSource.get();
}
void crLightSourceManager::setLightValidDistance(float dist)
{
    m_lightValidDistance = dist;
}
float crLightSourceManager::getLightValidDistance()
{
	return m_lightValidDistance;
}
//crLightSource *crLightSourceManager::selectLightByEye(const CRCore::crVector3& eyeLocal)
//{
//    CRCore::ScopedLock<crMutex> lock(m_mutex);
//	//crLight *light;
//	crVector3 /*lightPos,lightDir,*/lightColor,lightCalcedColor,eye_light;
//	float lightLength2,attenuation,cosa;
//    float maxLuminance,temp;
//	crLightSource *ls = NULL;
//	maxLuminance = 0.001;
//
//	//CRCore::crIntersectVisitor iv;
//	//CRCore::ref_ptr<CRCore::crLineSegment> seg = new CRCore::crLineSegment;
//
//	//crMatrix lightMat;
//	crLight *light;
//	for( LSVec::iterator itr = m_lsVec.begin();
//		itr != m_lsVec.end();
//		++itr )
//	{
//		ls = itr->get();
//		if(!ls->getEnable() || !ls->getCalcShadow()) continue;
//		//light = dynamic_cast<crLight *>((*itr)->getLight());
//		
//		//lightMat = (*itr)->getLightMatrix();
//		//const CRCore::crVector4& lightPosition = light->getPosition();
//		const crVector3 &lightPos = ls->getLSPosition();
//		//lightPos.set(light->getPosition()[0],light->getPosition()[1],light->getPosition()[2]);
//		//lightPos = lightPos * lightMat;
//		if(lightPos[2] <= eyeLocal[2] + 0.1) continue;
//
//		//lightColor.set((*itr)->getLSColor()[0],(*itr)->getLSColor()[1],(*itr)->getLSColor()[2]);
//		const crMatrixf &lsParam = ls->getLSParam();
//		lightColor.set(lsParam(1,0),lsParam(1,1),lsParam(1,2));
//		eye_light = eyeLocal - lightPos;
//		lightLength2 = eye_light.length2();
//		
//		//attenuation = (*itr)->getLSAttenuations()[0] + (*itr)->getLSAttenuations()[1] * sqrt(lightLength2) + (*itr)->getLSAttenuations()[2] * lightLength2;
//		attenuation = lsParam(3,0) + lsParam(3,1) * sqrt(lightLength2) + lsParam(3,2) * lightLength2;
//		attenuation = 1.0 / attenuation;
//		
//		light = dynamic_cast<crLight *>(ls->getLight());
//		if(light->getLightType() == crLight::LIGHT_SPOT)
//		{
//		    eye_light.normalize();
//		    cosa = eye_light * ls->getLSDirection();
//		    attenuation *= pow(cosa,lsParam(3,3)/*(*itr)->getLSAttenuations()[3]*/);
//		}
//
//		lightCalcedColor = lightColor * attenuation;
//        temp = lightCalcedColor[0] + lightCalcedColor[1] + lightCalcedColor[2];
//		if(maxLuminance < temp)
//		{
//		 //   iv.reset();
//		 //   seg->set(lightPos,eyeLocal);
//			//iv.addLineSegment(seg.get());
//			//iv.insertAvoidIntersectNode(    );
//			//CRCore::crBrain::getInstance()->getCurrentActiveDB()->accept(iv);
//   //         if (!iv.hits())
//			//{
//				m_currentFrameEyeShadowLightSource = ls;
//				maxLuminance = temp;
//			//}
//		}
//	}
//	return m_currentFrameEyeShadowLightSource.get();
//}
//
//void crLightSourceManager::selectLightByBoundingBox(const CRCore::crObject *obj,const CRCore::crBoundingBox &bbox, ref_ptr<crLightSource> &ls1, ref_ptr<crLightSource> &ls2, ref_ptr<crLightSource> &ls3,bool effectByShadow)
//{
//	CRCore::ScopedLock<crMutex> lock(m_mutex);
//	//crLight *light;
//	crVector3 /*lightPos,lightDir,*/lightColor,lightCalcedColor,center,center_light;
//	float lightLength2,attenuation,cosa;
//	float temp;
//	float maxcosa = 0.0f;
//	crLightSource *ls = NULL;
//	crLightSource *shadowls = NULL;
//
//	m_selectLightMap.clear();
//    
//	center = bbox.center();
//	//CRCore::crIntersectVisitor iv;
//	//CRCore::ref_ptr<CRCore::crLineSegment> seg = new CRCore::crLineSegment;
//
//	//crMatrix lightMat;
//	crLight *light;
//	for( LSVec::iterator itr = m_lsVec.begin();
//		itr != m_lsVec.end();
//		++itr )
//	{
//		ls = itr->get();
//		if(!ls->getEnable()) continue;
//		if(obj->isNeedLight(ls->getName()))
//		{
//			m_selectLightMap.insert(std::make_pair(1.0f,ls));
//			continue;
//		}
//		if(obj->isAvoidLight(ls->getName())) continue;
//
//		//light = dynamic_cast<crLight *>((*itr)->getLight());
//
//		//lightMat = (*itr)->getLightMatrix();
//		//const CRCore::crVector4& lightPosition = light->getPosition();
//		//lightPos.set(light->getPosition()[0],light->getPosition()[1],light->getPosition()[2]);
//		//lightPos = lightPos * lightMat;
//		const crVector3 &lightPos = ls->getLSPosition();
//
//		//lightColor.set((*itr)->getLSColor()[0],(*itr)->getLSColor()[1],(*itr)->getLSColor()[2]);
//		const crMatrixf &lsParam = ls->getLSParam();
//		lightColor.set(lsParam(1,0),lsParam(1,1),lsParam(1,2));
//		center_light = center - lightPos;
//		lightLength2 = center_light.length2();
//		
//		//attenuation = (*itr)->getLSAttenuations()[0] + (*itr)->getLSAttenuations()[1] * sqrt(lightLength2) + (*itr)->getLSAttenuations()[2] * lightLength2;
//		attenuation = lsParam(3,0) + lsParam(3,1) * sqrt(lightLength2) + lsParam(3,2) * lightLength2;
//		attenuation = 1.0 / attenuation;
//
//		light = dynamic_cast<crLight *>(ls->getLight());
//		if(light->getLightType() == crLight::LIGHT_SPOT)
//		{
//			center_light.normalize();
//			cosa = center_light * ls->getLSDirection();
//			attenuation *= pow(cosa,lsParam(3,3)/*(*itr)->getLSAttenuations()[3]*/);
//		}
//
//		lightCalcedColor = lightColor * attenuation;
//		temp = lightCalcedColor[0] + lightCalcedColor[1] + lightCalcedColor[2];
//		if(temp>0.001)
//		{
//			m_selectLightMap.insert(std::make_pair(temp,ls));
//
//			if(effectByShadow && (*itr)->getCalcShadow() && cosa > maxcosa)
//			{
//				shadowls = ls;
//				maxcosa = cosa;
//			}
//		}
//	}
//	//CRCore::notify(CRCore::ALWAYS)<<" m_selectLightMap="<<m_selectLightMap.size()<< std::endl;
//
//	int i = 0;
//	for( SelectLightMap::iterator itr = m_selectLightMap.begin();
//		 i < 3 && itr != m_selectLightMap.end();
//		 ++itr, i++ )
//	{
//        if( i == 0) 
//		{
//			if(shadowls)
//			{
//                ls1 = shadowls;
//				if(shadowls != itr->second)
//				{
//					ls2 = itr->second;
//					i=1;
//					continue;
//				}
//			}
//			else
//			    ls1 = itr->second;
//		}
//		else if( i == 1) 
//		{
//			ls2 = itr->second;
//		}
//		else if( i == 2) 
//		{
//			if(shadowls)
//			{
//				if(shadowls != itr->second)
//			    {
//				    ls3 = itr->second;
//				    break;
//			    }
//				else i = 1;
//			}
//			else
//			{
//				ls3 = itr->second;
//				break;
//			}
//		}
//		else if( i > 2)  break;
//	}
//
//	//if(!ls1->isCalcShadow())
//	//{
//	//	if(ls2.valid() && ls2->isCalcShadow())
//	//	{
//	//		ls = ls1.get();
//	//		ls1 = ls2;
//	//		ls2 = ls;
//	//	}
//	//	else if(ls3.valid() && ls3->isCalcShadow())
//	//	{
//	//		ls = ls1.get();
//	//		ls1 = ls3;
//	//		ls3 = ls;
//	//	}
//	//}
//	//if(ls3.valid())
//	//{
//	//	if((!ls1->isCalcShadow() || !ls2->isCalcShadow()) && ls3->isCalcShadow())
//	//	{
//	//		if(!ls1->isCalcShadow())
//	//		{
//	//			ls = ls1.get();
//	//			ls1 = ls3;
//	//			ls3 = ls;
//	//		}
//	//		else if(!ls2->isCalcShadow())
//	//		{
//	//			ls = ls2.get();
//	//			ls2 = ls3;
//	//			ls3 = ls;
//	//		}
//	//	}
//	//}
//
// //   if(m_hasSunLight && ls1.valid())
//	//{
//	//	if(ls2.valid() && ls2->getLightIsSunLight())
//	//	{
//	//		ls = ls1.get();
//	//		ls1 = ls2;
//	//		ls2 = ls;
//	//	}
//
//	//	if(ls3.valid() && ls3->getLightIsSunLight())
//	//	{
//	//		ls = ls1.get();
//	//		ls1 = ls3;
//	//		ls3 = ls;
//	//	}
//	//}
//}
//
//crLightSourceManager::LSVec &crLightSourceManager::selectLightByNeed(const CRCore::crObject *obj)
//{//根据预设信息选择光源，用于复杂的多光源渲染场合
//    CRCore::ScopedLock<crMutex> lock(m_mutex);
//	m_neededSelectVec.clear();
//	for( LSVec::iterator itr = m_lsVec.begin();
//		itr != m_lsVec.end();
//		++itr )
//	{
//		if(!(*itr)->getEnable()) continue;
//		//if(obj->isAvoidLight((*itr)->getName())) continue;
//		if(obj->isNeedLight((*itr)->getName()))
//		{
//			m_neededSelectVec.push_back(itr->get());
//		}
//	}
//	return m_neededSelectVec;
//}

void crLightSourceManager::selectLightByBoundingBox(const CRCore::crObject *obj,const CRCore::crBoundingBox &bbox)
{
    //CRCore::ScopedLock<crMutex> lock(m_mutex);
	//crVector3 /*lightPos,lightDir,*/lightColor,lightCalcedColor,center,center_light;
	//float lightLength2,attenuation,cosa;
	//float temp;
	crLightSource *ls = NULL;
    m_onePassLightVec.clear();
	m_appendLightVec.clear();
	//center = bbox.center();
	crLight *light;
	crBoundingBox lightBox;
	crVector3 lightPos;
	//float l;
	crObject *non_const_obj = NULL;
	if(crDisplaySettings::instance()->getRunMode()==0) 
	{
		non_const_obj = const_cast<crObject *>(obj);
		non_const_obj->lock();
	    non_const_obj->resetSelectedLightList();
	}
	//std::vector<CRCore::crVector3> pointVec;
	//pointVec.reserve(8);
	//crLight::LightType lightType;
	const crVector3 &eye = crBrain::getInstance()->getCameraPosition();
	float dis = 0;
	bool inlight = false;
	crPolytope light_frustum;
	float cutoff;
	crMatrix lightMVP;
	for( LSVec::iterator itr = m_lsVec.begin();
		itr != m_lsVec.end();
		++itr )
	{
		ls = itr->get();
		if(!ls->getEnable() || ls == m_sunLightSource.get()) continue;
		light = dynamic_cast<crLight *>((*itr)->getLight());
		lightPos = ls->getLSPosition();
		dis = (lightPos - eye).length();
        if(dis<m_lightValidDistance)
		{
			if(obj->isNeedLight(ls->getName()))
			{
				//m_selectLightMap.insert(std::make_pair(1.0f,ls));
				if(m_onePassLightVec.size()<MaxOnePassLightNum)
					m_onePassLightVec.push_back(ls);
				else
					m_appendLightVec.push_back(ls);
				if(non_const_obj) non_const_obj->selectedLight(ls);
				continue;
			}
			if(obj->isAvoidLight(ls->getName())) continue;

			inlight = false;
			//聚光灯的优化问题
			if(light->getLightType() == crLight::LIGHT_SPOT)
			{
				const crVector3 &spotLightDir = ls->getLSDirection();
				cutoff = light->getSpotCutoff();
				lightMVP = crMatrix::lookAt(lightPos,lightPos + spotLightDir,Z_AXIS) * crMatrix::perspective(cutoff*2.0f,1.0f,1.0f,light->getMaxDistance());
				light_frustum.setToUnitFrustum(true,true);
				light_frustum.transformProvidingInverse(lightMVP);
				if(light_frustum.contains(bbox))
				{
                    inlight = true;
				}
			}
			else
			{
				lightBox.init();
				lightBox.expandBy(crBoundingSphere(lightPos,light->getMaxDistance()));
				if(lightBox.intersects(bbox)||lightBox.contains(bbox))
				{
					inlight = true;
				}
			}
            if(inlight)
			{
				if(m_onePassLightVec.size()<MaxOnePassLightNum)
					m_onePassLightVec.push_back(ls);
				else
					m_appendLightVec.push_back(ls);
				if(non_const_obj) non_const_obj->selectedLight(ls);
			}
		}
	}
	if(non_const_obj) non_const_obj->unlock();
}

//void crLightSourceManager::setSunShadowDistance(float sunShadowDistance)
//{
//	m_sunShadowDistance = sunShadowDistance;
//}

crLightSource *crLightSourceManager::reuseOrgetOrCreateAnIdleEffectNodes(const std::string &lsName)
{
	return findLightSource(lsName);
}
