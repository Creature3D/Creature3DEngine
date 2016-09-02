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
#include <CRCore/crLightSource.h>
#include <CRCore/crNotify.h>
#include <CRCore/crBrain.h>
#include <CRCore\crSearchNodeBYNameVisitor.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crLight.h>
#include <CRCore/crBillboard.h>
using namespace CRCore;

crLightSource::crLightSource():
    m_referenceFrame(RELATIVE_TO_PARENTS),
	m_enable(true),
	m_global(true),
	m_sunLight(false),
	m_stdModeEnable(false),
	m_calcShadow(false),
	m_calcShadow2(false),
	//m_cosfov(0.0f),
	//m_lightShadowCulled(false),
	m_lightRangeScale(0.01f),
	m_staticShadowMapChannel(-1),
	m_invShadowMapSize(0.0f),
	m_changeEnableInNextFrame(false),
	m_changeEnable(false)
{
    // switch off culling of light source nodes by default.
    setCullingActive(false);
	m_lightParamStruct = new LightParamStruct;
}


crLightSource::~crLightSource()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crLightSource() "<< std::endl;
    // ref_ptr<> automactially decrements the reference count of attached lights.
}

void crLightSource::setReferenceFrame(ReferenceFrame rf)
{
    m_referenceFrame = rf;
    setCullingActive(m_referenceFrame==RELATIVE_TO_PARENTS);
}

void crLightSource::setLight(crStateAttribute* light)
{
    m_light = light;
}

void crLightSource::initLightParamStruct()
{
	//CRCore::notify(CRCore::ALWAYS)<<"crLightSource::initUniformsStruct0 "<<getName()<<std::endl;
	if(!m_light.valid()) return;
	CRCore::ScopedLock<crMutex> lock(m_mutex);

	//CRCore::notify(CRCore::ALWAYS)<<"crLightSource::initLightParamStruct "<<getName()<<std::endl;

	crLight *light = dynamic_cast<crLight *>(m_light.get());
	const crVector4 &pos = light->getPosition();
	m_lightParamStruct->m_lsPosition.set(pos[0],pos[1],pos[2]);
	m_lightParamStruct->m_lsDirection = light->getDirection();

	//m_lightParamStruct->m_lsColor = light->getDiffuse();
	//m_lightParamStruct->m_lsAmbient = light->getAmbient();
	//m_lightParamStruct->m_lsDiffuse = light->getDiffuse();
	//m_lightParamStruct->m_lsSpecular = light->getSpecular();
	//m_lightParamStruct->m_lsAttenuations.set(light->getConstantAttenuation(),light->getLinearAttenuation(),light->getQuadraticAttenuation(),light->getSpotExponent());
    const crVector4 &ambient = light->getAmbient();
    const crVector4 &diffuse = light->getDiffuse();
	const crVector4 &specular = light->getSpecular();
	//m_lightParamStruct->m_lsParam.set(ambient[0],diffuse[0],specular[0],light->getConstantAttenuation(),
	//	          ambient[1],diffuse[1],specular[1],light->getLinearAttenuation(),
	//			  ambient[2],diffuse[2],specular[2],light->getQuadraticAttenuation(),
	//			  ambient[3],diffuse[3],specular[3],light->getSpotExponent());
	//if(getLightIsSunLight())
	//{
 //       float lum = 1.0f/light->getConstantAttenuation();
	//	m_lightParamStruct->m_lsParam.set(ambient[0]*lum,ambient[1]*lum,ambient[2]*lum,ambient[3]*lum,
	//		diffuse[0]*lum,diffuse[1]*lum,diffuse[2]*lum,diffuse[3]*lum,
	//		specular[0]*lum,specular[1]*lum,specular[2]*lum,specular[3]*lum,
	//		light->getConstantAttenuation(),light->getLinearAttenuation(),light->getQuadraticAttenuation(),light->getSpotExponent());
	//}
	//else
	//{
	    float constantAttenuation = light->getConstantAttenuation();
		//if(crDisplaySettings::instance()->getHDR() && crDisplaySettings::instance()->getNumMultiSamples() > 1)
		//{
		//	constantAttenuation *= 2.0f;
		//}
		m_lightParamStruct->m_lsParam.set(ambient[0],ambient[1],ambient[2],ambient[3],
			diffuse[0],diffuse[1],diffuse[2],diffuse[3],
			specular[0],specular[1],specular[2],specular[3],
			constantAttenuation,light->getLinearAttenuation(),light->getQuadraticAttenuation(),light->getSpotExponent());
	//}

	m_lightParamStruct->m_cosfov = cos(DegreesToRadians(light->getSpotCutoff()));
	m_lightParamStruct->m_loweastLum = light->getLoweastLum();

}

void crLightSource::initLight()
{
	//if(m_light.valid())
	//{
	//	if(!m_stateset.valid())
	//	{
	//		m_stateset = new crStateSet;
	//	}
	//    m_stateset->setAttributeAndModes(m_light.get(),CRCore::crStateAttribute::ON);

	//	crStateSet *exist_stateset = CRCore::crBrain::getInstance()->checkStateSetExistAndGet(m_stateset.get());
	//	if(exist_stateset)
	//		m_stateset = exist_stateset;
	//	else
	//		CRCore::crBrain::getInstance()->insertStateSet(m_stateset.get());
	//}
}

//// Set the GLModes on crStateSet associated with the ClipPlanes.
//void crLightSource::setStateSetModes(crStateSet& stateset,crStateAttribute::VRModeValue value) const
//{
//    if (m_light.valid())
//    {
//        stateset.setAssociatedModes(m_light.get(),value);
//    }
//}
//
//void crLightSource::setLocalStateSetModes(crStateAttribute::VRModeValue value)
//{
//    if (!m_stateset) m_stateset = new crStateSet;
//    m_stateset->setAllToInherit();
//    setStateSetModes(*m_stateset,value);
//}

bool crLightSource::computeBound() const
{
    return crGroup::computeBound();
    
    //if (m_light.valid() && m_referenceFrame==RELATIVE_TO_PARENTS)
    //{
    //    const crLight* light = dynamic_cast<const crLight*>(m_light.get());
    //    if (light)
    //    {
    //        const crVector4& pos = light->getPosition();
    //        if (pos[3]!=0.0f)
    //        {
    //            float div = 1.0f/pos[3];
    //            m_boundSphere.expandBy(crVector3(pos[0]*div,pos[1]*div,pos[2]*div));
    //        }
    //    }
    //}
}

void crLightSource::setEnable( bool enable ){ m_enable = enable; }
void crLightSource::setStdModeEnable( bool enable ){ m_stdModeEnable = enable; }
void crLightSource::setGlobal( bool global ){ m_global = global; }
void crLightSource::setLightToBeSunLight(bool isSunlight){ m_sunLight = isSunlight; }
void crLightSource::setCalcShadow(bool calcShadow){ m_calcShadow = calcShadow; }

//void crLightSource::addAvoidLightObject(const crObject *obj)
//{
//	m_avoidLightObjectSet.insert(obj);
//}

//void crLightSource::addNeedLightObject(const crObject *obj)
//{
//	m_needLightObjectSet.insert(obj);
//}

void crLightSource::setStaticShadowMapChannel(int channel)
{
	m_staticShadowMapChannel = channel;
}

void crLightSource::setEffectPosition(const crVector3 &position)
{
	 CRCore::ScopedLock<crMutex> lock(m_mutex); 
    (dynamic_cast<crLight *>(m_light.get()))->setPosition(crVector4(position,1.0f));
	getLightParamStruct()->m_lsPosition = position;
}