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
#include <CREncapsulation/crNodeCallbacks.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crEffectSequenceManager.h>
#include <CRCore/crIntersectVisitor.h>
#include <CRCore/crFilterRenderManager.h>
//#include <CRPhysics/crBotManager.h>
#include <rbody/Creature3D/CreBody.h>
#include <CRCore/crMultiSwitch.h>
#include <CRCore/crClipPlane.h>
#include <CRCore/crCullFace.h>
#include <CRCore/crStencil.h>
#include <CRCore/crBlendColor.h>
#include <CREncapsulation/crSceneManager.h>

using namespace CREncapsulation;
using namespace CRCore;
using namespace CRUtil;
using namespace CRParticle;
using namespace CRPhysics;
using namespace rbody;
using namespace CRTerrain;
using namespace CRIOManager;
////////////////////////////////
//crCreBodyCullCallback
//CullCallback
///////////////////////////////////
crCreBodyCullCallback::crCreBodyCullCallback():
m_nextUpdateFrame(0),
m_previousTraversalNumber(0),
m_fineFactor(1.0f)
{
}
crCreBodyCullCallback::crCreBodyCullCallback(const crCreBodyCullCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_nextUpdateFrame(0),
m_previousTraversalNumber(0),
m_fineFactor(1.0f)
{
}
void crCreBodyCullCallback::setFineFactor(float factor)
{
	m_fineFactor = factor;
}
void crCreBodyCullCallback::operator()(crNode* node, crNodeVisitor* nv)
{
	CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
	CreBodyNode *bodyNode = dynamic_cast<CreBodyNode *>(node);
	if (bodyNode&&cv&&cv->getRenderMode() == CRUtil::crCullVisitor::NormalRender)
	{
		float distance = fabsf(cv->getDistanceToEyePoint(bodyNode->getBound().center(),false));
		crDisplaySettings *ds = crDisplaySettings::instance();
		float disp = ds->getCharacterDispDistance();
		if(distance - disp > ds->getCharacterFarDistance())
		{
			bodyNode->setCalcShadow(false);
			return;
		}
		if(nv->getTraversalNumber() != m_previousTraversalNumber)
		{
			m_previousTraversalNumber = nv->getTraversalNumber();
			if(m_nextUpdateFrame<=0)
			{
				float lodLevel = (disp - distance) / (disp-ds->getCharacterNearDistance());
				bodyNode->setLodLevel(lodLevel);
				bodyNode->setCalcShadow(distance<ds->getCharacterShadowDistance());
				m_nextUpdateFrame = floor(distance*ds->getCharacterUpdateFactor()*m_fineFactor);
				if(m_nextUpdateFrame>3) m_nextUpdateFrame = 3;
				bodyNode->needUpdate();
			}
			else
			{
				m_nextUpdateFrame--;
			}
		}
	}
	crNodeCallback::traverse(node,nv);
}
////////////////////////////////
//crDuplicateCallback
//CullCallback
///////////////////////////////////
crDuplicateCallback::crDuplicateCallback():
	m_duplicateCount(0)
{
}
crDuplicateCallback::crDuplicateCallback(const crDuplicateCallback& callback,const CRCore::crCopyOp&copyop):
	crNodeCallback(callback,copyop),
	m_duplicateCount(callback.m_duplicateCount),
	m_matrixVec(callback.m_matrixVec)
{
}
void crDuplicateCallback::addEventParam(int i, const std::string &str)
{
	if(i == 0)
	{
		m_duplicateCount = atoi(str.c_str());
	}
	else
	{
		if(i%2==1)
		{
			crArgumentParser::appAtoVec(str,m_readTemp);
		}
		else
		{
			crMatrix mat;
			if(m_readTemp == crVector3(0.0f,0.0f,0.0f))
			{
				m_readTemp[1] = -1.0f;
			}
			if(m_readTemp[1] > 0.999)
			{
				mat.makeRotate(-CRCore::Y_AXIS,m_readTemp);
				CRCore::crVector3d ep,center,up;
				mat.getLookAt(ep,center,up);
				if(center[2]>0.0f)
				{
					center = -center;
				}
				mat.makeLookAt(ep,center,up);
			}
			else
				mat.makeRotate(-CRCore::Y_AXIS,m_readTemp);
			crArgumentParser::appAtoVec(str,m_readTemp);
			mat.setTrans(m_readTemp);
			m_matrixVec.push_back(mat);
		}
	}
}
crDuplicateCallback::MatrixVec &crDuplicateCallback::getMatrixVec()
{
	return m_matrixVec;
}
void crDuplicateCallback::setDuplicateCount(int count)
{
	m_duplicateCount = count;
}
int crDuplicateCallback::getDuplicateCount()
{
	return m_duplicateCount;
}
void crDuplicateCallback::operator()(crNode* node, crNodeVisitor* nv)
{
	crNodeCallback::traverse(node,nv);
	CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
	if (cv)
	{
		int count = CRCore::minimum(m_duplicateCount,(int)m_matrixVec.size());
		for(int i = 0; i<m_duplicateCount && i<count; i++)
		{
			// push the culling mode.
			cv->pushCurrentMask();
			RefMatrix* matrix = cv->createOrReuseMatrix(cv->getModelViewMatrix());
			matrix->preMult(m_matrixVec[i]);
			cv->pushModelViewMatrix(matrix);
			crNodeCallback::traverse(node,nv);
			cv->popModelViewMatrix();
			cv->popCurrentMask();
		}
	}
}
////////////////////////////////
//crReflectMaterialCallback
//CullCallback
///////////////////////////////////
crGndReflectMaterialCallback::crGndReflectMaterialCallback():
m_init(false),
m_groundOffset(0.0f),
m_enable(true)
{
}
crGndReflectMaterialCallback::crGndReflectMaterialCallback(const crGndReflectMaterialCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_groundOffset(callback.m_groundOffset),
m_enable(callback.m_enable),
m_init(false)
{
}
void crGndReflectMaterialCallback::init()
{
	m_stateset2 = new crStateSet;
	m_stateset2_buf = new crStateSet;

	//////////2
	//crClipPlane* clipplane = new crClipPlane;
	//clipplane->setClipPlane(0.0,0.0,-1.0,m_groundOffset);
	//clipplane->setClipPlaneNum(0);
	//m_stateset2->setAttributeAndModes(clipplane,crStateAttribute::OVERRIDE|crStateAttribute::ON);
	//m_stateset2_buf->setAttributeAndModes(clipplane,crStateAttribute::OVERRIDE|crStateAttribute::ON);

	crCullFace *cullFace = new crCullFace(crCullFace::FRONT);
	m_stateset2->setAttributeAndModes(cullFace,crStateAttribute::OVERRIDE|crStateAttribute::ON);
	m_stateset2_buf->setAttributeAndModes(cullFace,crStateAttribute::OVERRIDE|crStateAttribute::ON);
	
	crStencil *stencil = new crStencil;
	stencil->setFunction(crStencil::EQUAL,1,~0u);
	stencil->setOperation(crStencil::KEEP, crStencil::KEEP, crStencil::KEEP);
	m_stateset2->setAttributeAndModes(stencil,crStateAttribute::ON);
	m_stateset2_buf->setAttributeAndModes(stencil,crStateAttribute::ON);
	
	m_stateset2->setRenderingHint(crStateSet::MIRRORREFLECT_BIN);
	m_stateset2_buf->setRenderingHint(crStateSet::MIRRORREFLECT_BIN);

    m_init = true;
}

void crGndReflectMaterialCallback::addEventParam(int i, const std::string &str)
{
	if(i == 0) m_groundOffset = atof(str.c_str());
}

void crGndReflectMaterialCallback::operator()(crNode* node, crNodeVisitor* nv)
{
	if(m_enable)
	{
		if(!m_init)
		{
			init();
		}
		CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
		if (cv && cv->getRenderMode() == CRUtil::crCullVisitor::NormalRender)
		{
			//////////////////////////////////2
			const crBoundingBox &bbox = node->getBoundBox();
			float zOffset =m_groundOffset - bbox.zMin()/* - 0.01*/;/*(bbox.zMin() + bbox.zLength() * 0.5f);*/
			//CRCore::notify(CRCore::ALWAYS)<<"zOffset = "<<zOffset<<std::endl;
			crMatrix reflectMatrix = crMatrix::translate(0.0f,0.0f,-zOffset)*
				crMatrix::scale(1.0f,1.0f,-1.0f)*
				crMatrix::translate(0.0f,0.0f,zOffset);
			//crClipPlane* clipplane = dynamic_cast<crClipPlane *>(m_stateset2->getAttribute(crStateAttribute::CLIPPLANE0));
			//if(clipplane) clipplane->setClipPlane(0.0,0.0,-1.0,zOffset);

			cv->pushCurrentMask();
			bool stateset_pushed = cv->pushStateSet(m_stateset2.get());
			RefMatrix* matrix = cv->createOrReuseMatrix(cv->getModelViewMatrix());
			matrix->preMult(reflectMatrix);
			cv->pushModelViewMatrix(matrix);
			crNodeCallback::traverse(node,nv);
			cv->popModelViewMatrix();
			if(stateset_pushed) cv->popStateSet();
			cv->popCurrentMask();
		}
	}
	crNodeCallback::traverse(node,nv);
}

void crGndReflectMaterialCallback::swapBuffers(int frameNumber)
{
	if(m_swapFrameNumber != frameNumber)
	{
		ref_ptr<crStateSet> stateset = m_stateset2;
		m_stateset2 = m_stateset2_buf;
		m_stateset2_buf = stateset;

		crNodeCallback::swapBuffers(frameNumber);
		m_swapFrameNumber = frameNumber;
	}
}
////////////////////////////////
//crMirrorCallback
//CullCallback
///////////////////////////////////
crMirrorCallback::crMirrorCallback():
m_init(false),
m_intensity(0.3f)
{
}
crMirrorCallback::crMirrorCallback(const crMirrorCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_intensity(callback.m_intensity),
m_init(false)
{
}
void crMirrorCallback::init()
{
	m_stateset0 = new crStateSet;
	m_stateset1 = new crStateSet;
	m_stateset3 = new crStateSet;

 //   ////////0
	crStencil* stencil = new crStencil;
	stencil->setFunction(crStencil::ALWAYS,1,~0u);
	stencil->setOperation(crStencil::KEEP, crStencil::KEEP, crStencil::REPLACE);

	crColorMask* colorMask = new crColorMask;
	colorMask->setMask(false,false,false,false);

	m_stateset0->setRenderingHint(crStateSet::MIRRORFIRST_BIN);
	//m_stateset1->setMode(GL_CULL_FACE,crStateAttribute::OFF);
	m_stateset0->setAttributeAndModes(stencil,crStateAttribute::ON);
	m_stateset0->setAttribute(colorMask);
    
	///////////////1
	stencil = new crStencil;
	stencil->setFunction(crStencil::EQUAL,1,~0u);
	stencil->setOperation(crStencil::KEEP, crStencil::KEEP, crStencil::KEEP);

	colorMask = new crColorMask;
	colorMask->setMask(false,false,false,false);

	crDepth* depth = new crDepth;
	depth->setFunction(crDepth::ALWAYS);
	depth->setRange(1.0,1.0);

	m_stateset1->setRenderingHint(crStateSet::MIRRORSECOND_BIN);
	//m_stateset1->setMode(GL_CULL_FACE,crStateAttribute::OFF);
	m_stateset1->setAttributeAndModes(stencil,crStateAttribute::ON);
	m_stateset1->setAttribute(colorMask);
	m_stateset1->setAttribute(depth);

	///////////////3
	crBlendColor *blendColor = new crBlendColor;
	blendColor->setConstantColor(crVector4(m_intensity,m_intensity,m_intensity,0.0));
	crBlendFunc* trans = new crBlendFunc;
	trans->setFunction(crBlendFunc::ONE,crBlendFunc::CONSTANT_COLOR);

	m_stateset3->setRenderingHint(crStateSet::MIRROREND_BIN);
	//m_stateset3->setMode(GL_STENCIL_TEST,crStateAttribute::OFF);
	m_stateset3->setAttributeAndModes(blendColor,crStateAttribute::ON);
	m_stateset3->setAttributeAndModes(trans,crStateAttribute::ON);

	m_init = true;
}

void crMirrorCallback::addEventParam(int i, const std::string &str)
{
	if(i == 0) m_intensity = atof(str.c_str());
}

void crMirrorCallback::operator()(crNode* node, crNodeVisitor* nv)
{
	if(!m_init)
	{
		init();
	}
	CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
    if (cv && cv->getRenderMode() == CRUtil::crCullVisitor::NormalRender)
	{
		bool stateset_pushed = cv->pushStateSet(m_stateset0.get());
		cv->setRenderMode(crCullVisitor::OnlyGeodeRender);
		crNodeCallback::traverse(node,nv);
		cv->setRenderMode(crCullVisitor::NormalRender);
		if(stateset_pushed) cv->popStateSet();

		stateset_pushed = cv->pushStateSet(m_stateset1.get());
		cv->setRenderMode(crCullVisitor::OnlyGeodeRender);
		crNodeCallback::traverse(node,nv);
		cv->setRenderMode(crCullVisitor::NormalRender);
		if(stateset_pushed) cv->popStateSet();

		stateset_pushed = cv->pushStateSet(m_stateset3.get());
		crNodeCallback::traverse(node,nv);
		if(stateset_pushed) cv->popStateSet();
	}
	else crNodeCallback::traverse(node,nv);
}

////////////////////////////////
//crRenascenceCallback
//UpdateCallback
///////////////////////////////////
crRenascenceCallback::crRenascenceCallback():
m_startFrameTime(0.0f),
m_interval(5.0f)
{
}
crRenascenceCallback::crRenascenceCallback(const crRenascenceCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_interval(callback.m_interval)
{
}

void crRenascenceCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		m_interval = atof(str.c_str());
		break;
	}
}

void crRenascenceCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
{
	CRUtil::crUpdateVisitor* uv = dynamic_cast<CRUtil::crUpdateVisitor*>(nv);
	if (uv)
	{
		crViewMatterObject *vo = dynamic_cast<crViewMatterObject *>(node);
		if(vo && vo->isDead())
		{
			crFilterRenderManager *filterRenderManager = crFilterRenderManager::getInstance();
			bool isFirstPlayer = vo == crSceneManager::getInstance()->getPlayer();
			if(!isFirstPlayer || vo->getRequireRenascence())
			{
				double referenceTime = crFrameStamp::getInstance()->getReferenceTime();
				//CRCore::notify(CRCore::FATAL)<<"crDeathCallback(): 1 referenceTime = "<<referenceTime<<std::endl;

				if(vo->getDeadFlag())
				{
					m_startFrameTime = referenceTime;
					if(isFirstPlayer)
					{
						if(!m_dead.valid()) m_dead = crFilterRenderManager::getInstance()->getDrawable("Dead");
						if(m_dead.valid()) m_dead->setVisiable(false);
					}
				}
				else if(referenceTime - m_startFrameTime > m_interval)
				{
					//CRCore::notify(CRCore::FATAL)<<"crDeathCallback(): reference begin "<<std::endl;
					if(isFirstPlayer)
					{
						crDrawable *text = crFilterRenderManager::getInstance()->getDrawable("Renascence");
						if(text) text->setVisiable(false);
					}

					vo->renascence();
					//vo->setPosition(m_renascenecePosVec[m_r.get_random()] + crVector3f(0.0f,0.0f,1.0f));
					//CRCore::notify(CRCore::FATAL)<<"crDeathCallback(): reference end "<<std::endl;
				}
				else if(isFirstPlayer)
				{
					if(!m_renascence.valid()) m_renascence = crFilterRenderManager::getInstance()->getDrawable("Renascence");
					if(m_renascence.valid())
					{
						char tmpText[2];
						sprintf(tmpText,"%1d\0",(int)(m_interval - referenceTime + m_startFrameTime));
						m_renascence->setText(tmpText);
						m_renascence->setVisiable(true);
					}
				}
			}
			else if(isFirstPlayer)
			{
				if(!m_dead.valid()) m_dead = crFilterRenderManager::getInstance()->getDrawable("Dead");
				if(m_dead.valid()) m_dead->setVisiable(true);
			}
		}
	}

	crNodeCallback::traverse(node,nv);
}

////////////////////////////////
//crLightLumRandomCallback
//CullCallback
///////////////////////////////////
crLightLumRandomCallback::crLightLumRandomCallback():
m_lastFrameTime(0.0),
m_interval(1.0f)
{
	m_lumRange.set(0.0f,1.0f);
}
crLightLumRandomCallback::crLightLumRandomCallback(const crLightLumRandomCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_lumRange(callback.m_lumRange),
m_interval(callback.m_interval),
m_lastFrameTime(0.0)
{
}

void crLightLumRandomCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		{
            crVector2 vec2;
			crArgumentParser::appAtoVec(str,vec2);
            m_lumRange.set(vec2[0],vec2[1]);
		}
		break;
	case 1:
		m_interval = atof(str.c_str());
		break;
	}
}

void crLightLumRandomCallback::setInterval(float interval) { m_interval = interval; }
void crLightLumRandomCallback::setLumRange(float min, float max)
{
	m_lumRange.set(min,max);
}

void crLightLumRandomCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
{
	CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
	crLightSource* ls = dynamic_cast<crLightSource*>(node);
	if (cv && ls && cv->getRenderMode() == CRUtil::crCullVisitor::NormalRender)
	{
		double curFrameTime = crFrameStamp::getInstance()->getReferenceTime();
		if(curFrameTime - m_lastFrameTime > m_interval)
		{
			crLight* light = dynamic_cast<crLight*>(ls->getLight());
			//ls->getLightParamStruct()->m_lsColor = light->getDiffuse() * m_lum.get_random();
			float lum = m_lumRange.get_random();
			crVector4 ambient = light->getAmbient() * lum;
			crVector4 diffuse = light->getDiffuse() * lum;
			crVector4 specular = light->getSpecular() * lum;

			ls->lock();
			ls->getLightParamStruct()->m_lsParam.set(ambient[0],ambient[1],ambient[2],ambient[3],
				diffuse[0],diffuse[1],diffuse[2],diffuse[3],
				specular[0],specular[1],specular[2],specular[3],
				light->getConstantAttenuation(),light->getLinearAttenuation(),light->getQuadraticAttenuation(),light->getSpotExponent());
            ls->unlock();
			m_lastFrameTime = curFrameTime;
		}
	}

	crNodeCallback::traverse(node,nv);
}

////////////////////////////////
//crLightPositionRandomCallback
//CullCallback
///////////////////////////////////
crLightPositionRandomCallback::crLightPositionRandomCallback():
m_init(false),
m_lastFrameTime(0.0),
m_interval(1.0f)
{
	m_radiusRange.set(0.0f,0.0f);
}
crLightPositionRandomCallback::crLightPositionRandomCallback(const crLightPositionRandomCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_init(false),
m_radiusRange(callback.m_radiusRange),
m_interval(callback.m_interval),
m_lastFrameTime(0.0)
{
}

void crLightPositionRandomCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		{
			float radius = atof(str.c_str());
			m_radiusRange.set(-radius,radius);
		}
		break;
	case 1:
		m_interval = atof(str.c_str());
		break;
	}
}

void crLightPositionRandomCallback::setInterval(float interval) { m_interval = interval; }
void crLightPositionRandomCallback::setRadiusRange(float radius)
{
	m_radiusRange.set(-radius,radius);
}

void crLightPositionRandomCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
{
	CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
	crLightSource* ls = dynamic_cast<crLightSource*>(node);
	if (cv && ls && cv->getRenderMode() == CRUtil::crCullVisitor::NormalRender)
	{
		double curFrameTime = crFrameStamp::getInstance()->getReferenceTime();
		if(curFrameTime - m_lastFrameTime > m_interval)
		{
			crLight* light = dynamic_cast<crLight*>(ls->getLight());
			if(!m_init)
			{
				const crVector4 &pos = light->getPosition();
				m_origPos.set(pos[0],pos[1],pos[2]);
				m_init = true;
			}
			crVector3 posRandom(m_radiusRange.get_random(),m_radiusRange.get_random(),m_radiusRange.get_random());
			ls->setEffectPosition(posRandom + m_origPos);
			m_lastFrameTime = curFrameTime;
		}
	}
	crNodeCallback::traverse(node,nv);
}

//////////////////////////////////
////crSunSimulationCallback
////CullCallback
/////////////////////////////////////
//crSunSimulationCallback::crSunSimulationCallback():
//m_rotSpeed(5.0f),
//m_currentRot(0.0f)
//{
//
//}
//crSunSimulationCallback::crSunSimulationCallback(const crSunSimulationCallback& callback,const CRCore::crCopyOp&copyop):
//crNodeCallback(callback,copyop),
//m_rotSpeed(callback.m_rotSpeed),
//m_currentRot(callback.m_currentRot)
//{
//}
//
//void crSunSimulationCallback::addEventParam(int i, const std::string &str)
//{
//	switch(i) 
//	{
//	case 0:
//	    m_rotSpeed = atof(str.c_str());
//		break;
//	case 1:
//		m_currentRot = atof(str.c_str());
//		break;
//	}
//}
//void crSunSimulationCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
//{
//	CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
//	crLightSource* ls = dynamic_cast<crLightSource*>(node);
//	if (cv && ls && cv->getRenderMode() == CRUtil::crCullVisitor::NormalRender)
//	{
//		m_currentRot += m_rotSpeed * nv->getFrameStamp()->getFrameInterval();
//		if(m_currentRot>CRCore::PI)
//		{
//			m_currentRot = CRCore::PI;
//			m_rotSpeed *= -1.0f;
//		}
//		else if(m_currentRot<0.0f)
//		{
//			m_currentRot = CRCore::PI;
//			m_rotSpeed *= -1.0f;
//		}
//		m_sunRotMatrix = CRCore::crMatrix::rotate(m_currentRot, 0.0, 0.0, 1.0);
//		crLight* light = dynamic_cast<crLight*>(ls->getLight());
//		const crVector4 &lightpos = light->getPosition();
//		crVector3 newpos = crVector3(lightpos[0],lightpos[1],lightpos[2]) * m_sunRotMatrix;
//		float constantAttenuation = light->getConstantAttenuation();
//		ls->lock();
//		ls->getLightParamStruct()->m_lsPosition = newpos;
//		if(m_rotSpeed<0.0f)
//		{//月亮亮度
//			constantAttenuation *= 5.0;//亮度缩小5倍
//			ls->getLightParamStruct()->m_lsParam(3,0) = constantAttenuation;
//		}
//		else
//		{
//            ls->getLightParamStruct()->m_lsParam(3,0) = constantAttenuation;
//		}
//		ls->unlock();
//	}
//
//	crNodeCallback::traverse(node,nv);
//}

////////////////////////////////
//crSunInEyesightCallback
//CullCallback
///////////////////////////////////
crSunInEyesightCallback::crSunInEyesightCallback(const crSunInEyesightCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_sunDir(callback.m_sunDir),
m_ambientIntensity(callback.m_ambientIntensity),
m_lightModel(callback.m_lightModel),
m_lightModel_buf(callback.m_lightModel_buf),
m_stateset(callback.m_stateset),
m_stateset_buf(callback.m_stateset_buf),
m_scale(callback.m_scale),
m_init(callback.m_init)
{
}
void crSunInEyesightCallback::init()
{
	m_lightModel = new CRCore::crLightModel;
	m_lightModel->setAmbientIntensity(m_ambientIntensity);
	m_stateset = new crStateSet;
	m_stateset->setAttributeAndModes(m_lightModel.get(),crStateAttribute::ON);

	m_lightModel_buf = new CRCore::crLightModel;
	m_lightModel_buf->setAmbientIntensity(m_ambientIntensity);
	m_stateset_buf = new crStateSet;
	m_stateset_buf->setAttributeAndModes(m_lightModel_buf.get(),crStateAttribute::ON);
}

void crSunInEyesightCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		crArgumentParser::appAtoVec(str,m_sunPos);
		m_sunDir = m_sunPos;
		m_sunDir.normalize();
		break;
	case 1:
		crArgumentParser::appAtoVec(str,m_ambientIntensity);
		break;
	case 2:
		m_scale = atof(str.c_str());
		break;
	}
}

void crSunInEyesightCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
{
	if(!m_init)
	{
		init();
		m_init = true;
	}
	CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
	if (cv&&cv->getRenderMode() == CRUtil::crCullVisitor::NormalRender)
	{
		const CRCore::crVector3& eye = cv->getEyeLocal();
		crVector3 lookDir = cv->getLookVectorLocal();//dir
		float cos_a = lookDir * m_sunDir;
		cos_a = (cos_a - 1.0f) * m_scale + 1.0f;
		bool stateset_pushed = false;
		if(cos_a>0)
		{
			CRCore::crIntersectVisitor iv;
			CRCore::ref_ptr<CRCore::crLineSegment> lineSegment = new CRCore::crLineSegment;
			lineSegment->set(eye,m_sunPos);
			iv.addLineSegment(lineSegment.get());

			crBrain::getInstance()->getCurrentActiveDB()->accept(iv);

			if (!iv.hits())
			{
				crVector4 result = m_ambientIntensity * cos_a + crBrain::getInstance()->getAmbientIntensity() * (1.0-cos_a);
				float brightness = crBrain::getInstance()->getBrightness() + crDisplaySettings::instance()->getBrightness();
				m_lightModel->setAmbientIntensity(result + crVector4(brightness,brightness,brightness,1.0f));
				stateset_pushed = cv->pushStateSet(m_stateset.get());
			}  
		}

		crNodeCallback::traverse(node,nv);
		if(stateset_pushed) cv->popStateSet();            
	}
	else crNodeCallback::traverse(node,nv);
}

void crSunInEyesightCallback::swapBuffers(int frameNumber)
{
	if(m_swapFrameNumber != frameNumber)
	{
		ref_ptr<crStateSet> stateset = m_stateset;
		m_stateset = m_stateset_buf;
		m_stateset_buf = stateset;

		ref_ptr<crLightModel> lightModel = m_lightModel;
		m_lightModel = m_lightModel_buf;
		m_lightModel_buf = lightModel;

		crNodeCallback::swapBuffers(frameNumber);
		m_swapFrameNumber = frameNumber;
	}
}

////////////////////////////////
//crSunFlaresCallback
//UpdateCallback
///////////////////////////////////
crSunFlaresCallback::crSunFlaresCallback(const crSunFlaresCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_sunPos(callback.m_sunPos),
m_distance(callback.m_distance),
m_scale(callback.m_scale),
m_distanceRateVec(callback.m_distanceRateVec)
{
}
void crSunFlaresCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		crArgumentParser::appAtoVec(str,m_sunPos);
		break;
	case 1:
		m_distance = atof(str.c_str());
		break;
	case 2:
		m_scale = atof(str.c_str());
		break;
	}
	if(i>2)
	{
		m_distanceRateVec.push_back(atof(str.c_str()));
	}
}

void crSunFlaresCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
{
	CRUtil::crUpdateVisitor* uv = dynamic_cast<CRUtil::crUpdateVisitor*>(nv);
	if (uv)
	{
		CRCore::crMatrixd *viewMatrix = uv->getViewMatrix();
		CRCore::crMatrixd *projectionMatrix = uv->getProjectionMatrix();

		crVector3d eye,origin,up;
		viewMatrix->getLookAt(eye,origin,up,m_distance);

		crVector3 sunPos = m_sunPos + eye;
		crVector3 sunToCenterDir = sunPos - origin;
		sunToCenterDir.normalize();
		sunToCenterDir  = sunToCenterDir * m_scale;

		crPolytope view_frustum;
		view_frustum.setToUnitFrustum(true,true);
		view_frustum.transformProvidingInverse( (*viewMatrix) * (*projectionMatrix) );

		view_frustum.pushCurrentMask();
		if(view_frustum.contains(sunPos))
		{//
			CRCore::crIntersectVisitor iv;
			CRCore::ref_ptr<CRCore::crLineSegment> lineSegment = new CRCore::crLineSegment;
			lineSegment->set(eye,sunPos);
			iv.addLineSegment(lineSegment.get());

			crBrain::getInstance()->getCurrentActiveDB()->accept(iv);

			if (iv.hits())
			{
				node->setVisiable(false);
			}
			else
			{
				node->setVisiable(true);
				crBillboard *sunFlares = dynamic_cast<crBillboard *>(node);
				assert(!sunFlares);
				//CRCore::notify(CRCore::ALWAYS)<<"sunFlares = "<<origin<<std::endl;
				int i = 0;
				for( DistanceRateVec::iterator itr = m_distanceRateVec.begin(); 
					itr != m_distanceRateVec.end();
					++itr, i++ )
				{
					sunFlares->setPos(i,origin + sunToCenterDir * (*itr));
				}
				//sunFlares->setPos(0,origin + sunToCenterDir * 0.78f);
				//sunFlares->setPos(1,origin + sunToCenterDir * 0.56f);
				//sunFlares->setPos(2,origin + sunToCenterDir * 0.33f);
				//sunFlares->setPos(3,origin + sunToCenterDir * 0.11f);
				//sunFlares->setPos(4,origin - sunToCenterDir * 0.11f);
				//sunFlares->setPos(5,origin - sunToCenterDir * 0.33f);
				//sunFlares->setPos(6,origin - sunToCenterDir * 0.56f);
				//sunFlares->setPos(7,origin - sunToCenterDir * 0.78f);
			}
		}
		else
		{
			node->setVisiable(false);
		}
		view_frustum.popCurrentMask();
		crNodeCallback::traverse(node,nv);
	}
}

////////////////////////////////
//crMoveBillboardWithEyeCallback
//UpdateCallback
///////////////////////////////////
crMoveBillboardWithEyeCallback::crMoveBillboardWithEyeCallback(const crMoveBillboardWithEyeCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_referencePos(callback.m_referencePos)
{
}
void crMoveBillboardWithEyeCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		crArgumentParser::appAtoVec(str,m_referencePos);
		break;
	}
}

void crMoveBillboardWithEyeCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
{
	CRUtil::crUpdateVisitor* uv = dynamic_cast<CRUtil::crUpdateVisitor*>(nv);
	if (uv)
	{
		crVector3 position = m_referencePos + uv->getEyePoint();
		crBillboard *billboard = dynamic_cast<crBillboard *>(node);
		crBillboard::PositionList& pl = billboard->getPositionList();
		for( crBillboard::PositionList::iterator bpItr = pl.begin(); 
			bpItr != pl.end();
			++bpItr )
		{
			*bpItr = position;
		}
		crNodeCallback::traverse(node,nv);
	}
}

////////////////////////////////
//crDofTexMatCallback
//CullCallback
///////////////////////////////////
crDofTexMatCallback::crDofTexMatCallback():
m_previousTraversalNumber(0)
{
	m_dof = new CRCore::crDOFTransform;
	m_dof->setReferenceFrame(crDOFTransform::RELATIVE_TO_ABSOLUTE);
}
crDofTexMatCallback::crDofTexMatCallback(const crDofTexMatCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_previousTraversalNumber(0)
{
	if(callback.m_dof.valid()) m_dof = dynamic_cast<crDOFTransform *>(callback.m_dof->clone(crCopyOp::SHALLOW_COPY));
}

crDOFTransform *crDofTexMatCallback::getDofTransform()
{
	return m_dof.get();
}

void crDofTexMatCallback::addEventParam(int i, const std::string &str)
{
	m_dof->addParam(i,str);
}

void crDofTexMatCallback::operator()(crNode* node, crNodeVisitor* nv)
{
	CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
	if (cv&&cv->getRenderMode() == CRUtil::crCullVisitor::NormalRender && nv->getTraversalNumber() != m_previousTraversalNumber)
	{
		m_previousTraversalNumber = nv->getTraversalNumber();
		crStateSet *stateset = node->getOrCreateStateSet();
		if(stateset)
		{
			crTexMat *texMat = dynamic_cast<crTexMat *>(stateset->getTextureAttribute(TEXTURE_BASEMAP,crStateAttribute::TEXMAT));
			if(!texMat)
			{
				texMat = new CRCore::crTexMat;
				stateset->setTextureAttributeAndModes(TEXTURE_BASEMAP,texMat,crStateAttribute::ON);
			}
			m_dof->animate(crFrameStamp::getInstance()->getFrameInterval());
			crMatrix& matrix = texMat->getMatrix();
			m_dof->computeLocalToWorldMatrix(matrix,nv);
		}
	}
	crNodeCallback::traverse(node,nv);
}

////////////////////////////////
//crDofUniformTexMatCallback
//UpdateCallback
///////////////////////////////////
crDofUniformTexMatCallback::crDofUniformTexMatCallback():
m_previousTraversalNumber(0)
{
	m_dof = new CRCore::crDOFTransform;
	m_dof->setReferenceFrame(crDOFTransform::RELATIVE_TO_ABSOLUTE);
}
crDofUniformTexMatCallback::crDofUniformTexMatCallback(const crDofUniformTexMatCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_uniformID(callback.m_uniformID),
m_previousTraversalNumber(0)
{
	if(callback.m_dof.valid())
	{
		m_dof = dynamic_cast<crDOFTransform *>(callback.m_dof->clone(crCopyOp::SHALLOW_COPY));
	}
}

crDOFTransform *crDofUniformTexMatCallback::getDofTransform()
{
	return m_dof.get();
}
const std::string& crDofUniformTexMatCallback::getUniformID()const
{
	return m_uniformID;
}
void crDofUniformTexMatCallback::addEventParam(int i, const std::string &str)
{
	if (i==0) 
	{
		m_uniformID = str;
		if(m_uniformID.compare("baseMat") == 0)
		{
			m_dof->setName("颜色1");
		}
		else if(m_uniformID.compare("baseMatA") == 0)
		{
			m_dof->setName("颜色1Alpha");
		}
		else if(m_uniformID.compare("diffuseMat") == 0)
		{
			m_dof->setName("颜色2");
		}
		else if(m_uniformID.compare("diffuseMatA") == 0)
		{
			m_dof->setName("颜色2Alpha");
		}
		else if(m_uniformID.compare("diffuseMat2") == 0)
		{
			m_dof->setName("颜色3");
		}
		else if(m_uniformID.compare("diffuseMat2A") == 0)
		{
			m_dof->setName("颜色3Alpha");
		}
		else if(m_uniformID.compare("normalMat") == 0)
		{
			m_dof->setName("法线1");
		}
		else if(m_uniformID.compare("normalMat2") == 0)
		{
			m_dof->setName("法线2");
		}
		else if(m_uniformID.compare("normalMat3") == 0)
		{
			m_dof->setName("法线3");
		}
		else if(m_uniformID.compare("specularMat") == 0)
		{
			m_dof->setName("高光1");
		}
		else if(m_uniformID.compare("specularMatA") == 0)
		{
			m_dof->setName("高光指数1Alpha");
		}
		else if(m_uniformID.compare("specularMat2") == 0)
		{
			m_dof->setName("高光2");
		}
		else if(m_uniformID.compare("specularMat2A") == 0)
		{
			m_dof->setName("高光指数2Alpha");
		}
		else if(m_uniformID.compare("specularMat3") == 0)
		{
			m_dof->setName("高光3");
		}
		else if(m_uniformID.compare("specularMat3A") == 0)
		{
			m_dof->setName("高光指数3Alpha");
		}
		else if(m_uniformID.compare("emissiveMat") == 0)
		{
			m_dof->setName("自发光1或法线4");
		}
		else if(m_uniformID.compare("emissiveMatA") == 0)
		{
			m_dof->setName("Alpha扭曲1");
		}
		else if(m_uniformID.compare("emissiveMat2") == 0)
		{
			m_dof->setName("自发光2或颜色4");
		}
		else if(m_uniformID.compare("emissiveMat2A") == 0)
		{
			m_dof->setName("Alpha扭曲2");
		}
		else if(m_uniformID.compare("emissiveMat3") == 0)
		{
			m_dof->setName("自发光3或高光4");
		}
		else if(m_uniformID.compare("emissiveMat3A") == 0)
		{
			m_dof->setName("Alpha扭曲3");
		}
		else if(m_uniformID.compare("UVSMat") == 0)
		{
			m_dof->setName("UV扰动贴图");
		}
		else if(m_uniformID.compare("CWMat") == 0)
		{
			m_dof->setName("通道控制贴图");
		}
		else if(m_uniformID.compare("CWMatR") == 0)
		{
			m_dof->setName("通道控制贴图R");
		}
		else if(m_uniformID.compare("CWMatG") == 0)
		{
			m_dof->setName("通道控制贴图G");
		}
		else if(m_uniformID.compare("CWMatB") == 0)
		{
			m_dof->setName("通道控制贴图B");
		}
		else if(m_uniformID.compare("CWMatA") == 0)
		{
			m_dof->setName("通道控制贴图A");
		}
		else
		{
			m_dof->setName(str);
		}
	}
	else m_dof->addParam(i-1,str);
}

void crDofUniformTexMatCallback::operator()(crNode* node, crNodeVisitor* nv)
{
	CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
	if (cv&&cv->getRenderMode() == CRUtil::crCullVisitor::NormalRender && nv->getTraversalNumber() != m_previousTraversalNumber)
	{
		m_previousTraversalNumber = nv->getTraversalNumber();
		crStateSet *stateset = node->getStateSet();
		if(stateset)
		{
			crUniform *uniform = stateset->getUniform(m_uniformID);
			if(uniform && uniform->getDataVariance() == crBase::DYNAMIC)
			{
				m_dof->animate(crFrameStamp::getInstance()->getFrameInterval());

				crMatrixf m4;
				uniform->get(m4);
				m_dof->computeLocalToWorldMatrix(m4,nv);
				uniform->set(m4);
			}
		}
	}
	crNodeCallback::traverse(node,nv);
}
////////////////////////////////
//crUniformParam1SinCallback
//UpdateCallback
///////////////////////////////////
crUniformParam1SinCallback::crUniformParam1SinCallback():
m_previousTraversalNumber(0),
m_scale(1.0f)
{
}
crUniformParam1SinCallback::crUniformParam1SinCallback(const crUniformParam1SinCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_uniformID(callback.m_uniformID),
m_previousTraversalNumber(0),
m_scale(callback.m_scale)
{
}

void crUniformParam1SinCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		m_uniformID = str;
		break;
	case 1:
		m_scale = atof(str.c_str());
		break;
	}
}

void crUniformParam1SinCallback::operator()(crNode* node, crNodeVisitor* nv)
{
	CRUtil::crCullVisitor* uv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
	if (uv && nv->getTraversalNumber() != m_previousTraversalNumber)
	{
		m_previousTraversalNumber = nv->getTraversalNumber();
		crStateSet *stateset = node->getStateSet();
		if(stateset)
		{
			crUniform *uniform = stateset->getUniform(m_uniformID);
			if(uniform && uniform->getDataVariance() == crBase::DYNAMIC)
			{
				double time = crFrameStamp::getInstance()->getReferenceTime();
				float v = m_scale * sin(time);
				uniform->set(v);
			}
		}
	}
	crNodeCallback::traverse(node,nv);
}
////////////////////////////////
//crUniformParam1CosCallback
//UpdateCallback
///////////////////////////////////
crUniformParam1CosCallback::crUniformParam1CosCallback():
m_previousTraversalNumber(0),
m_scale(1.0f)
{
}
crUniformParam1CosCallback::crUniformParam1CosCallback(const crUniformParam1CosCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_uniformID(callback.m_uniformID),
m_previousTraversalNumber(0),
m_scale(callback.m_scale)
{
}

void crUniformParam1CosCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		m_uniformID = str;
		break;
	case 1:
		m_scale = atof(str.c_str());
		break;
	}
}

void crUniformParam1CosCallback::operator()(crNode* node, crNodeVisitor* nv)
{
	CRUtil::crCullVisitor* uv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
	if (uv && nv->getTraversalNumber() != m_previousTraversalNumber)
	{
		m_previousTraversalNumber = nv->getTraversalNumber();
		crStateSet *stateset = node->getStateSet();
		if(stateset)
		{
			crUniform *uniform = stateset->getUniform(m_uniformID);
			if(uniform && uniform->getDataVariance() == crBase::DYNAMIC)
			{
				double time = crFrameStamp::getInstance()->getReferenceTime();
				float v = m_scale * cos(time);
				uniform->set(v);
			}
		}
	}
	crNodeCallback::traverse(node,nv);
}
////////////////////////////////
//crUniformParam1RndCallback
//UpdateCallback
///////////////////////////////////
crUniformParam1RndCallback::crUniformParam1RndCallback():
m_previousTraversalNumber(0),
m_scale(1.0f)
{
}
crUniformParam1RndCallback::crUniformParam1RndCallback(const crUniformParam1RndCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_uniformID(callback.m_uniformID),
m_previousTraversalNumber(0),
m_scale(callback.m_scale)
{
}

void crUniformParam1RndCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		m_uniformID = str;
		break;
	case 1:
		m_scale = atof(str.c_str());
		break;
	}
}

void crUniformParam1RndCallback::operator()(crNode* node, crNodeVisitor* nv)
{
	CRUtil::crCullVisitor* uv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
	if (uv && nv->getTraversalNumber() != m_previousTraversalNumber)
	{
		m_previousTraversalNumber = nv->getTraversalNumber();
		crStateSet *stateset = node->getStateSet();
		if(stateset)
		{
			crUniform *uniform = stateset->getUniform(m_uniformID);
			if(uniform && uniform->getDataVariance() == crBase::DYNAMIC)
			{
				float v = m_scale * CRCore::rangef(0.0f,1.0f).get_random();
				uniform->set(v);
			}
		}
	}
	crNodeCallback::traverse(node,nv);
}
////////////////////////////////
//crUniformParam3RndCallback
//UpdateCallback
///////////////////////////////////
crUniformParam3RndCallback::crUniformParam3RndCallback():
m_previousTraversalNumber(0),
m_scale(1.0f)
{
}
crUniformParam3RndCallback::crUniformParam3RndCallback(const crUniformParam3RndCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_uniformID(callback.m_uniformID),
m_previousTraversalNumber(0),
m_scale(callback.m_scale)
{
}

void crUniformParam3RndCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		m_uniformID = str;
		break;
	case 1:
		m_scale = atof(str.c_str());
		break;
	}
}

void crUniformParam3RndCallback::operator()(crNode* node, crNodeVisitor* nv)
{
	CRUtil::crCullVisitor* uv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
	if (uv && nv->getTraversalNumber() != m_previousTraversalNumber)
	{
		m_previousTraversalNumber = nv->getTraversalNumber();
		crStateSet *stateset = node->getStateSet();
		if(stateset)
		{
			crUniform *uniform = stateset->getUniform(m_uniformID);
			if(uniform && uniform->getDataVariance() == crBase::DYNAMIC)
			{
				CRCore::rangef rnd(0.0f,1.0f);
				crVector3 v(rnd.get_random(),rnd.get_random(),rnd.get_random());
				uniform->set(v*m_scale);
			}
		}
	}
	crNodeCallback::traverse(node,nv);
}
////////////////////////////////
//crUniformParam4RndCallback
//UpdateCallback
///////////////////////////////////
crUniformParam4RndCallback::crUniformParam4RndCallback():
m_previousTraversalNumber(0),
m_scale(1.0f)
{
}
crUniformParam4RndCallback::crUniformParam4RndCallback(const crUniformParam4RndCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_uniformID(callback.m_uniformID),
m_previousTraversalNumber(0),
m_scale(callback.m_scale)
{
}

void crUniformParam4RndCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		m_uniformID = str;
		break;
	case 1:
		m_scale = atof(str.c_str());
		break;
	}
}

void crUniformParam4RndCallback::operator()(crNode* node, crNodeVisitor* nv)
{
	CRUtil::crCullVisitor* uv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
	if (uv && nv->getTraversalNumber() != m_previousTraversalNumber)
	{
		m_previousTraversalNumber = nv->getTraversalNumber();
		crStateSet *stateset = node->getStateSet();
		if(stateset)
		{
			crUniform *uniform = stateset->getUniform(m_uniformID);
			if(uniform && uniform->getDataVariance() == crBase::DYNAMIC)
			{
				CRCore::rangef rnd(0.0f,1.0f);
				crVector4 v(rnd.get_random(),rnd.get_random(),rnd.get_random(),rnd.get_random());
				uniform->set(v*m_scale);
			}
		}
	}
	crNodeCallback::traverse(node,nv);
}
//////////////////////////////////
////crWaveBlendCallback
////CullCallback
/////////////////////////////////////
//crStateSet* crWaveBlendCallback::m_finalStateset = 0;
//crWaveBlendCallback::crWaveBlendCallback(const crWaveBlendCallback& callback):
//crNodeCallback(callback)
//{
//}
////void crWaveBlendCallback::addEventParam(int i, const std::string &str)
////{
////	switch(i) 
////	{
////	case 0:
////		m_isRect = atoi(str.c_str());
////		break;
////	}
////}
//void crWaveBlendCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
//{
//	CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
//	if (cv&&cv->getRenderMode() == CRUtil::crCullVisitor::NormalRender)
//	{
//		if(m_finalStateset)
//		{
//			static int s_frameNumber = 0;
//			static float s_lastRadius = 0.0f;
//
//			int curFrameNumber = cv->getFrameStamp()->getFrameNumber();
//			if(curFrameNumber>s_frameNumber)
//			{
//				s_frameNumber = curFrameNumber;
//				s_lastRadius = 0.0f;
//			}
//
//			crBoundingSphere& bs = node->getBound();
//			if(!bs.valid() || bs.radius() == 0.0f)
//			{
//				crNodeCallback::traverse(node,nv);
//				return;
//			}
//			crVector3 center = bs.center();
//			float radius = bs.radius();
//			crMatrixf rotation = cv->getModelViewMatrix();
//			rotation.setTrans(0.0f,0.0f,0.0f);
//			crVector3 dirX = (X_AXIS * rotation).normalize();
//			crVector3 dirZ = cv->getUpLocal();
//			dirZ.normalize();
//			crVector3 point1 = center + dirX * radius;
//			crVector3 point2 = center - dirX * radius;
//			crVector3 point3 = center + dirZ * radius;
//			crVector3 point4 = center - dirZ * radius;
//
//			float width = crDisplaySettings::instance()->getScreenWidth();
//			float height = crDisplaySettings::instance()->getScreenHeight();
//			crMatrix MVPW = CRCore::crMatrix::translate(1.0f,1.0f,1.0f)*CRCore::crMatrix::scale(0.5f*width,0.5f*height,0.5f);
//			CRCore::crMatrix inverseMVPW;
//			inverseMVPW.invert(MVPW);
//			crMatrix convertMatrix = cv->getMVPW()*inverseMVPW;
//
//			center = center * convertMatrix;
//			point1 = point1 * convertMatrix;
//			point2 = point2 * convertMatrix;
//			point3 = point3 * convertMatrix;
//			point4 = point4 * convertMatrix;
//
//			center[0] = clampTo(center[0],-1.0f,1.0f);
//			center[1] = clampTo(center[1],-1.0f,1.0f);
//			point1[0] = clampTo(point1[0],-1.0f,1.0f);
//			point1[1] = clampTo(point1[1],-1.0f,1.0f);
//			point2[0] = clampTo(point2[0],-1.0f,1.0f);
//			point2[1] = clampTo(point2[1],-1.0f,1.0f);
//			point3[0] = clampTo(point3[0],-1.0f,1.0f);
//			point3[1] = clampTo(point3[1],-1.0f,1.0f);
//			point4[0] = clampTo(point4[0],-1.0f,1.0f);
//			point4[1] = clampTo(point4[1],-1.0f,1.0f);
//
//			//center[0] = maximum(minimum(1.0f,center[0]),-1.0f);
//			//center[1] = maximum(minimum(1.0f,center[1]),-1.0f);
//			//point1[0] = maximum(minimum(1.0f,point1[0]),-1.0f);
//			//point1[1] = maximum(minimum(1.0f,point1[1]),-1.0f);
//			//point2[0] = maximum(minimum(1.0f,point2[0]),-1.0f);
//			//point2[1] = maximum(minimum(1.0f,point2[1]),-1.0f);
//			//point3[0] = maximum(minimum(1.0f,point3[0]),-1.0f);
//			//point3[1] = maximum(minimum(1.0f,point3[1]),-1.0f);
//			//point4[0] = maximum(minimum(1.0f,point4[0]),-1.0f);
//			//point4[1] = maximum(minimum(1.0f,point4[1]),-1.0f);
//
//			center[2] = 0.0f;
//			point1[2] = (point1 - center).length();
//			point2[2] = (point2 - center).length();
//			point3[2] = (point3 - center).length();
//			point4[2] = (point4 - center).length();
//			radius = maximum(maximum(maximum(point1[2],point2[2]),point3[2]),point4[2]);
//			if(radius >= 2.0f)
//			{
//				radius = 1.0f;
//				center.set(0.0f,0.0f,0.0f);
//			}
//			else if(radius>1.0f)
//			{
//				radius = 1.0f;
//			}
//			center[2] = radius * radius;
//
//			//crDrawable *drawable = crFilterRenderManager::getInstance()->getDrawable("Debug1");
//			//char tmpText[30];
//			//if(drawable)
//			//{
//			//	sprintf(tmpText,"center: %6.2f,%6.2f,%5.2f\0",center[0],center[1],center[2]);
//			//	drawable->setText(tmpText);
//			//}
//			//drawable = crFilterRenderManager::getInstance()->getDrawable("Debug2");
//			//if(drawable)
//			//{
//			//	sprintf(tmpText,"point1: %6.2f,%6.2f,%5.2f\0",point1[0],point1[1],point1[2]);
//			//	drawable->setText(tmpText);
//			//}
//			//drawable = crFilterRenderManager::getInstance()->getDrawable("Debug3");
//			//if(drawable)
//			//{
//			//	sprintf(tmpText,"point2: %6.2f,%6.2f,%5.2f\0",point2[0],point2[1],point2[2]);
//			//	drawable->setText(tmpText);
//			//}
//			//drawable = crFilterRenderManager::getInstance()->getDrawable("Debug4");
//			//if(drawable)
//			//{
//			//	sprintf(tmpText,"point3: %6.2f,%6.2f,%5.2f\0",point3[0],point3[1],point3[2]);
//			//	drawable->setText(tmpText);
//			//}
//			//drawable = crFilterRenderManager::getInstance()->getDrawable("Debug5");
//			//if(drawable)
//			//{
//			//	sprintf(tmpText,"point4: %6.2f,%6.2f,%5.2f\0",point4[0],point4[1],point4[2]);
//			//	drawable->setText(tmpText);
//			//}
//
//			//if(radius > 1.0f)
//			//{
//			//             float d = center.length();
//			//	if(radius>d+1.0f)
//			//	{
//			//                 center.set(0.0f,0.0f,0.0f);
//			//	}
//			//	//else
//			//	//{
//			//	//crVector3 dir = -waveCenter;
//			//	//dir.normalize();
//			// //            waveCenter += dir * (radius-1.0f);
//			//	radius = 1.0f;
//			//}
//			//center[2] = radius * radius;
//			//CRCore::notify(CRCore::NOTICE)<<"Name = "<<node->getName()<<" waveCR = "<<waveCenter<<std::endl;
//			if(radius>s_lastRadius)
//			{//选择最近的
//				s_lastRadius = radius;
//				m_finalStateset->getUniform("waveRect")->set(crVector4(center,-2.0f));
//			}
//			//crUniform *uniform = m_finalStateset->getUniform("waveCR");
//			//crVector3 waveCR;
//			//uniform->get(waveCR);
//			//crVector2 center0(waveCR[0],waveCR[1]);
//			//crVector2 center1(waveCenter[0],waveCenter[1]);
//			//float d0 = center0.length();
//			//float d1 = center1.length();
//			//CRCore::notify(CRCore::NOTICE)<<"waveCenter = "<<waveCenter<<std::endl;
//			//CRCore::notify(CRCore::NOTICE)<<"radius = "<<radius<<std::endl;
//
//			//crBoundingBox &bbox = node->getBoundBox();
//			//if(!bbox.valid())
//			//{
//			//	crNodeCallback::traverse(node,nv);
//			//	return;
//			//}
//			//crVector3 min = bbox.m_min;
//			//crVector3 max = bbox.m_max;
//			//float width = crDisplaySettings::instance()->getScreenWidth();
//			//float height = crDisplaySettings::instance()->getScreenHeight();
//			//crMatrix MVPW = CRCore::crMatrix::translate(1.0f,1.0f,1.0f)*CRCore::crMatrix::scale(0.5f*width,0.5f*height,0.5f);
//			//CRCore::crMatrix inverseMVPW;
//			//inverseMVPW.invert(MVPW);
//			//crMatrix convertMatrix = cv->getMVPW()*inverseMVPW;
//			//min = min * convertMatrix;
//			//max = max * convertMatrix;
//
//			//min[2] = 0.0f;
//			//max[2] = 0.0f;
//			//float temp;
//			//if( min.x() > max.x() )
//			//{
//			//	temp = min.x();
//			//	min.x() = max.x();
//			//	max.x() = temp;
//			//}
//			//if( min.y() > max.y() )
//			//{
//			//	temp = min.y();
//			//	min.y() = max.y();
//			//	max.y() = temp;
//			//}
//			//min[0] = maximum(minimum(1.0f,min[0]),-1.0f);
//			//min[1] = maximum(minimum(1.0f,min[1]),-1.0f);
//			//max[0] = maximum(minimum(1.0f,max[0]),-1.0f);
//			//max[1] = maximum(minimum(1.0f,max[1]),-1.0f);
//			//float radius = sqrtf(0.25f*((max-min).length2()));
//			//if(radius>s_lastRadius)
//			//{//选择最大的
//			//	crVector4 waveRect;
//			//	if(m_isRect)
//			//	{
//			//		waveRect.set(min[0],min[1],max[0],max[1]);
//			//	}
//			//	else
//			//	{
//			//		waveRect = crVector4((min+max)*0.5f,-2.0f);
//			//		waveRect[2] = radius * radius;
//			//	}
//			//	s_lastRadius = radius;
//			//	m_finalStateset->getUniform("waveRect")->set(waveRect);
//			//}
//		}
//		crNodeCallback::traverse(node,nv);
//	}
//	else crNodeCallback::traverse(node,nv);
//}
//
//void crWaveBlendCallback::registFinalStaticSet(crStateSet *stateset) 
//{ 
//	m_finalStateset = stateset; 
//}
//
//////////////////////////////////
////crResetWaveBlendCallback
////UpdateCallback
/////////////////////////////////////
//crStateSet* crResetWaveBlendCallback::m_finalStateset = 0;
//crResetWaveBlendCallback::crResetWaveBlendCallback(const crResetWaveBlendCallback& callback):
//crNodeCallback(callback),
//m_waveRect(callback.m_waveRect)
//{
//}
//void crResetWaveBlendCallback::addEventParam(int i, const std::string &str)
//{
//	switch(i) 
//	{
//	case 0:
//		crArgumentParser::readKeyValue(str,"VEC4",m_waveRect);
//		break;
//	}
//}
//
//void crResetWaveBlendCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
//{
//	CRUtil::crUpdateVisitor* uv = dynamic_cast<CRUtil::crUpdateVisitor*>(nv);
//	if (uv)
//	{
//		if(m_finalStateset)
//			m_finalStateset->getUniform("waveRect")->set(m_waveRect);
//		crNodeCallback::traverse(node,nv);
//	}
//}
//
//void crResetWaveBlendCallback::registFinalStaticSet(crStateSet *stateset) 
//{ 
//	m_finalStateset = stateset; 
//}

//////////////////////////////////
////crDetectCallback
////UpdateCallback
/////////////////////////////////////
//crDetectCallback::crDetectCallback(const crDetectCallback& callback):
//crNodeCallback(callback)
//{
//}
//void crDetectCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
//{
//	CRUtil::crUpdateVisitor* uv = dynamic_cast<CRUtil::crUpdateVisitor*>(nv);
//	if (uv)
//	{
//		crViewMatterObject *vo = dynamic_cast<crViewMatterObject *>(node);
//		crViewMatterObject *cameraNode = crSceneManager::getInstance()->getPlayer();
//		crGameBodyInfo *camera_bodyInfo = cameraNode->getGameBodyInfo();
//		crGameBodyInfo *bodyInfo = vo->getGameBodyInfo();
//		if(vo && camera_bodyInfo && bodyInfo && !vo->isDead() && !cameraNode->isDead())
//		{
//			//crVector3 eyepoint = nv->getEyePoint();
//			if(vo->getTeam() > 0)
//			{
//				CRCore::notify(CRCore::ALWAYS)<<"crDetectCallback::operator() detectCallback 被装配到非enemy上"<<std::endl;
//				crNodeCallback::traverse(node,nv);
//				return;
//			}
//			//CRCore::notify(CRCore::ALWAYS)<<"crDetectCallback::operator() detectCallback "<<vo->getName()<<" "<<nv->getFrameStamp()->getFrameNumber()<<std::endl;
//			crVector3 nodePos = vo->getBound().center();
//			crVector3 eyepoint = cameraNode->getBound().center();
//			float distance = (eyepoint - nodePos).length();
//			crVector3 dir = nodePos-eyepoint;
//			dir.normalize();
//			float cosa;
//			if(distance<bodyInfo->getMaxDetectRange() * camera_bodyInfo->getStealth())
//			{//向enemy发送发现主角的消息
//				//detectMap.clear();放在AIUPD里
//				cosa = dir * vo->getDirection();
//				if(cosa > 0.0f/*crDisplaySettings::instance()->getSightLimit()*/)
//					bodyInfo->getDetectMap().insert(std::make_pair(crGameBodyInfo::DetectParam(cosa,distance),cameraNode));
//			}
//			crBotManager::getInstance()->updateBotDetectMap(vo);
//
//			CRCore::crMatrixd *viewMatrix = uv->getViewMatrix();
//			CRCore::crMatrixd *projectionMatrix = uv->getProjectionMatrix();
//			crPolytope view_frustum;
//			view_frustum.setToUnitFrustum(true,true);
//			view_frustum.transformProvidingInverse( (*viewMatrix) * (*projectionMatrix) );
//			view_frustum.pushCurrentMask();
//			if(view_frustum.contains(nodePos))
//			{
//				if(distance<camera_bodyInfo->getMaxDetectRange() * bodyInfo->getStealth())
//				{
//					cosa = dir * -cameraNode->getDirection();
//					//if(cosa > crDisplaySettings::instance()->getSightLimit())
//					camera_bodyInfo->getDetectMap().insert(std::make_pair(crGameBodyInfo::DetectParam(cosa,distance),vo));
//				}
//			}
//			view_frustum.popCurrentMask();
//		}
//
//		crNodeCallback::traverse(node,nv);
//	}
//}

//////////////////////////////////
////crParticleBindCallback
////CullCallback
/////////////////////////////////////
//crParticleBindCallback::crParticleBindCallback(const crParticleBindCallback& callback):
//crNodeCallback(callback),
//m_effectName(callback.m_effectName),
//m_particleEffectNode(callback.m_particleEffectNode)
//{
//}
//void crParticleBindCallback::addEventParam(int i, const std::string &str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_effectName = str;
//		break;
//	}
//}
//
//void crParticleBindCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
//{
//	CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
//	if(cv&&cv->getRenderMode() == CRUtil::crCullVisitor::NormalRender)
//	{
//		if(!m_particleEffectNode.valid())
//			m_particleEffectNode = crEffectParticleManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(m_effectName);
//		if(m_particleEffectNode.valid())
//		{
//			m_particleEffectNode->setEffectPosition(node->getBound().center());
//			m_particleEffectNode->startEffect();
//		}
//		crNodeCallback::traverse(node,nv);
//	}
//	else crNodeCallback::traverse(node,nv);
//}
////////////////////////////////
//crSequenceBindCallback
//CullCallback
///////////////////////////////////
crSequenceBindCallback::crSequenceBindCallback(const crSequenceBindCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_effectName(callback.m_effectName),
m_sequenceEffectNode(callback.m_sequenceEffectNode)
{
}
void crSequenceBindCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		m_effectName = str;
		break;
	}
}

void crSequenceBindCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
{
	CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
	if(cv&&cv->getRenderMode() == CRUtil::crCullVisitor::NormalRender)
	{
		if(!m_sequenceEffectNode.valid())
			m_sequenceEffectNode = crEffectSequenceManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(m_effectName);
		if(m_sequenceEffectNode.valid())
		{
			m_sequenceEffectNode->setEffectPosition(node->getBound().center());
			m_sequenceEffectNode->startEffect();
		}
		crNodeCallback::traverse(node,nv);
	}
	else crNodeCallback::traverse(node,nv);
}

////////////////////////////////
//crVisableTimerCallback
//UpdateCallback
///////////////////////////////////
crVisableTimerCallback::crVisableTimerCallback():
m_startFrameTime(0.0),
m_interval(1.0f),
m_state(0)
{
}
crVisableTimerCallback::crVisableTimerCallback(const crVisableTimerCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_state(callback.m_state),
m_startFrameTime(callback.m_startFrameTime),
m_interval(callback.m_interval)
{
}
void crVisableTimerCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		m_interval = atof(str.c_str());
		break;
	}
}

void crVisableTimerCallback::setInterval(float interval) { m_interval = interval; m_state = 0; }

void crVisableTimerCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
{
	CRUtil::crUpdateVisitor* uv = dynamic_cast<CRUtil::crUpdateVisitor*>(nv);
	if (uv)
	{
		if(node->getVisiable())
		{
			double referenceTime = crFrameStamp::getInstance()->getReferenceTime();
			if(m_state==0)
			{
				m_startFrameTime = referenceTime;
				m_state++;
			}
			else if(referenceTime - m_startFrameTime > m_interval)
			{
				node->setVisiable(false);
				m_state = 0;
			}
		}
		crNodeCallback::traverse(node,nv);
	}
}
////////////////////////////////
//crUnloadTimerCallback
//UpdateCallback
///////////////////////////////////
crUnloadTimerCallback::crUnloadTimerCallback():
m_interval(1.0f),
m_time(0.0f)
{
}
crUnloadTimerCallback::crUnloadTimerCallback(const crUnloadTimerCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_interval(callback.m_interval),
m_time(0.0f)
{
}
void crUnloadTimerCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		m_interval = atof(str.c_str());
		break;
	}
}

void crUnloadTimerCallback::setInterval(float interval) { m_interval = interval; }

void crUnloadTimerCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
{
	CRUtil::crUpdateVisitor* uv = dynamic_cast<CRUtil::crUpdateVisitor*>(nv);
	if (uv)
	{
		float dt = crFrameStamp::getInstance()->getFrameInterval();
		m_time += dt;
		if(m_time>m_interval)
		{
			m_time = 0.0f;
			crGroup *proxyNode = node->getParent(0);
			crGroup *root = proxyNode->getParent(0);
			CRIOManager::crLoadManager::getInstance()->requestRemoveNode(root,proxyNode);
		}
		crNodeCallback::traverse(node,nv);
	}
}
////////////////////////////////
//crLightEnableTimerCallback
//UpdateCallback
///////////////////////////////////
crLightEnableTimerCallback::crLightEnableTimerCallback():
m_startFrameTime(0.0),
m_interval(1.0f),
m_state(0)
{
}
crLightEnableTimerCallback::crLightEnableTimerCallback(const crLightEnableTimerCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_state(callback.m_state),
m_startFrameTime(callback.m_startFrameTime),
m_interval(callback.m_interval)
{
}
void crLightEnableTimerCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		m_interval = atof(str.c_str());
		break;
	}
}

void crLightEnableTimerCallback::setInterval(float interval) { m_interval = interval; }

void crLightEnableTimerCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
{
	CRUtil::crUpdateVisitor* uv = dynamic_cast<CRUtil::crUpdateVisitor*>(nv);
	crLightSource *ls = dynamic_cast<crLightSource *>(node);
	if (uv && ls)
	{
		if(ls->getEnable())
		{
			double referenceTime = crFrameStamp::getInstance()->getReferenceTime();
			if(m_state==0)
			{
				m_startFrameTime = referenceTime;
				m_state++;
			}
			else if(referenceTime - m_startFrameTime > m_interval)
			{
				ls->setEnable(false);
				m_state = 0;
			}
		}
		crNodeCallback::traverse(node,nv);
	}
}

////////////////////////////////
//crFlashTimerCallback
//UpdateCallback
///////////////////////////////////
crFlashTimerCallback::crFlashTimerCallback():
m_lastFrameTime(0.0),
m_interval(1.0f)
{
}
crFlashTimerCallback::crFlashTimerCallback(const crFlashTimerCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_lastFrameTime(callback.m_lastFrameTime),
m_interval(callback.m_interval)
{
}
void crFlashTimerCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		m_interval = atof(str.c_str());
		break;
	}
}

void crFlashTimerCallback::setInterval(float interval) { m_interval = interval; }

void crFlashTimerCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
{
	CRUtil::crUpdateVisitor* uv = dynamic_cast<CRUtil::crUpdateVisitor*>(nv);
	if (uv)
	{
		double curFrameTime = crFrameStamp::getInstance()->getReferenceTime();
		if(curFrameTime - m_lastFrameTime > m_interval)
		{
			node->setVisiable(!node->getVisiable());
			m_lastFrameTime = curFrameTime;
		}
		crNodeCallback::traverse(node,nv);
	}
}

////////////////////////////////
//crBoneMatrixCallback
//UpdateCallback
///////////////////////////////////
crBoneMatrixCallback::crBoneMatrixCallback():
m_character(NULL)
{
}
crBoneMatrixCallback::crBoneMatrixCallback(const crBoneMatrixCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_boneName(callback.m_boneName),
m_character(NULL)
{
}
void crBoneMatrixCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		m_boneName = str;
		break;
	}
}

void crBoneMatrixCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
{
	CRUtil::crUpdateVisitor* uv = dynamic_cast<CRUtil::crUpdateVisitor*>(nv);
	crMatrixTransform *matNode = dynamic_cast<crMatrixTransform *>(node);
	if (uv && matNode)
	{
		if(!m_character)
		{
			CRCore::crSearchNodeBYClassNameVisitor searchByClassNameVisitor;
			searchByClassNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
			searchByClassNameVisitor.insertClassNameId("crViewMatterObject");
			searchByClassNameVisitor.insertClassNameId("crCharacterMatterObject");
			searchByClassNameVisitor.insertClassNameId("crMissileSetOrFlackMatterObject");
			searchByClassNameVisitor.insertClassNameId("crBodyCaseMatterObject");
			node->accept(searchByClassNameVisitor);
			crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(searchByClassNameVisitor.getResult());
			if(bot)
			{
                m_character = dynamic_cast<rbody::CreBodyNode *>(bot->getBodyChild());
			}
		}
		if(m_character)
		{
			crMatrix mat;
			m_character->getBoneMatrix(m_boneName,mat);
			matNode->setMatrix(mat);
		}
	}
	crNodeCallback::traverse(node,nv);
}

////////////////////////////////
//crCompassHCallback
//CullCallback
///////////////////////////////////
crCompassHCallback::crCompassHCallback():m_init(false),m_scale(0.011)
{
}
crCompassHCallback::crCompassHCallback(const crCompassHCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_textName(callback.m_textName),
m_text(callback.m_text),
m_texMat(callback.m_texMat),
m_texMat_buf(callback.m_texMat_buf),
m_stateset(callback.m_stateset),
m_stateset_buf(callback.m_stateset_buf),
m_init(callback.m_init),
m_scale(callback.m_scale)
{
}
void crCompassHCallback::init()
{
	m_texMat = new CRCore::crTexMat;
	m_stateset = new crStateSet;
	m_stateset->setTextureAttributeAndModes(0,m_texMat.get(),crStateAttribute::ON);

	m_texMat_buf = new CRCore::crTexMat;
	m_stateset_buf = new crStateSet;
	m_stateset_buf->setTextureAttributeAndModes(0,m_texMat_buf.get(),crStateAttribute::ON);
}

void crCompassHCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		m_textName = str;
		break;
	case 1:
		m_scale = atof(str.c_str());
		break;
	}
}

void crCompassHCallback::operator()(crNode* node, crNodeVisitor* nv)
{
	if(!m_init)
	{
		init();
		m_init = true;
	}
	CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
	if (cv&&cv->getRenderMode() == CRUtil::crCullVisitor::NormalRender)
	{
		if(!m_text.valid())
		{
			m_text = dynamic_cast<CRText::crText*>(crFilterRenderManager::getInstance()->getDrawable(m_textName));
		}
		if(!m_text.valid()) 
		{
			CRCore::notify(CRCore::ALWAYS)<<"crCompassHCallback::operator() not find text "<<m_textName.c_str()<<std::endl;
			return;
		}

		crMatrix& texMat = m_texMat->getMatrix();
		texMat.makeIdentity();
		crViewMatterObject *firstPlayer = crSceneManager::getInstance()->getPlayer();
		crQuat orientation;
		orientation.set(firstPlayer->getMatrix());
		float x,y,z;
		orientation.getRadians(x,y,z);
		int H = CRCore::RadiansToDegrees(y);
		texMat.setTrans(((float)H)*m_scale,0.0f,0.0f);
		crVector3 dir = -firstPlayer->getDirection();
		//dir[2] = 0.0f;
		//dir.normalize();
		if(dir[0]>=0.0f && dir[1]>0.0f)
			H = -H;
		else if(dir[0]>=0.0f && dir[1]<0.0f)
			H = 180 - H;
		else if(dir[0]<0.0f && dir[1]<0.0f)
			H = 180 - H;
		else if(dir[0]<0.0f && dir[1]>0.0f)
			H = 360 - H;
		else if(dir[0]>0.0f && dir[1] == 0.0f)
			H = 90;
		else if(dir[0]<0.0f && dir[1] == 0.0f)
			H = 270;

		char tmpText[4];
		sprintf(tmpText,"%d\0",H);
		m_text->setText(tmpText);

		bool stateset_pushed = cv->pushStateSet(m_stateset.get());
		crNodeCallback::traverse(node,nv);
		if(stateset_pushed) cv->popStateSet();
	}
}

void crCompassHCallback::swapBuffers(int frameNumber)
{
	if(m_swapFrameNumber != frameNumber)
	{
		ref_ptr<crStateSet> stateset = m_stateset;
		m_stateset = m_stateset_buf;
		m_stateset_buf = stateset;

		ref_ptr<crTexMat> texMat = m_texMat;
		m_texMat = m_texMat_buf;
		m_texMat_buf = texMat;

		crNodeCallback::swapBuffers(frameNumber); 
		m_swapFrameNumber = frameNumber;
	}
}
////////////////////////////////
//crCompassPCallback
//CullCallback
///////////////////////////////////
crCompassPCallback::crCompassPCallback():m_init(false),m_scale(0.011)
{
}
crCompassPCallback::crCompassPCallback(const crCompassPCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_textName(callback.m_textName),
m_text(callback.m_text),
m_texMat(callback.m_texMat),
m_texMat_buf(callback.m_texMat_buf),
m_stateset(callback.m_stateset),
m_stateset_buf(callback.m_stateset_buf),
m_init(callback.m_init),
m_scale(callback.m_scale)
{
}
void crCompassPCallback::init()
{
	m_texMat = new CRCore::crTexMat;
	m_stateset = new crStateSet;
	m_stateset->setTextureAttributeAndModes(0,m_texMat.get(),crStateAttribute::ON);

	m_texMat_buf = new CRCore::crTexMat;
	m_stateset_buf = new crStateSet;
	m_stateset_buf->setTextureAttributeAndModes(0,m_texMat_buf.get(),crStateAttribute::ON);
}

void crCompassPCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		m_textName = str;
		break;
	case 1:
		m_scale = atof(str.c_str());
		break;
	}
}

void crCompassPCallback::operator()(crNode* node, crNodeVisitor* nv)
{
	if(!m_init)
	{
		init();
		m_init = true;
	}
	CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
	if (cv&&cv->getRenderMode() == CRUtil::crCullVisitor::NormalRender)
	{
		if(!m_text.valid())
		{
			m_text = dynamic_cast<CRText::crText*>(crFilterRenderManager::getInstance()->getDrawable(m_textName));
		}
		if(!m_text.valid()) 
		{
			CRCore::notify(CRCore::ALWAYS)<<"crCompassPCallback::operator() not find text "<<m_textName.c_str()<<std::endl;
			return;
		}

		crMatrix& texMat = m_texMat->getMatrix();
		texMat.makeIdentity();
		crViewMatterObject *firstPlayer = crSceneManager::getInstance()->getPlayer();
		crQuat orientation;
		orientation.set(firstPlayer->getMatrix());
		float x,y,z;
		orientation.getRadians(x,y,z);
		int P = CRCore::RadiansToDegrees(x);
		char tmpText[4];
		sprintf(tmpText,"%d\0",P);
		m_text->setText(tmpText);
		texMat.setTrans(0.0f,((float)P)*m_scale,0.0f);

		bool stateset_pushed = cv->pushStateSet(m_stateset.get());
		crNodeCallback::traverse(node,nv);
		if(stateset_pushed) cv->popStateSet();
	}
}

void crCompassPCallback::swapBuffers(int frameNumber)
{
	if(m_swapFrameNumber != frameNumber)
	{
		ref_ptr<crStateSet> stateset = m_stateset;
		m_stateset = m_stateset_buf;
		m_stateset_buf = stateset;

		ref_ptr<crTexMat> texMat = m_texMat;
		m_texMat = m_texMat_buf;
		m_texMat_buf = texMat;

		crNodeCallback::swapBuffers(frameNumber);
		m_swapFrameNumber = frameNumber;
	}
}
////////////////////////////////
//crCompassRCallback
//UpdateCallback
///////////////////////////////////
crCompassRCallback::crCompassRCallback()
{
}
crCompassRCallback::crCompassRCallback(const crCompassRCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop)
{
}
void crCompassRCallback::operator()(crNode* node, crNodeVisitor* nv)
{
	CRUtil::crUpdateVisitor* uv = dynamic_cast<CRUtil::crUpdateVisitor*>(nv);
	if (uv)
	{
		//CRCore::ref_ptr<CRText::crText> m_text;
		//if(!m_text.valid())
		//{
		//	m_text = dynamic_cast<CRText::crText*>(crFilterRenderManager::getInstance()->getDrawable("CompassR"));
		//}
		//if(!m_text.valid()) 
		//{
		//	CRCore::notify(CRCore::ALWAYS)<<"crCompassRCallback::operator() not find text "<<m_textName.c_str()<<std::endl;
		//	return;
		//}

		//CRCore::crMatrixd *viewMatrix = uv->getViewMatrix();
		//CRCore::crMatrixd *projectionMatrix = uv->getProjectionMatrix();

		//crMatrix MVP = (*viewMatrix) * (*projectionMatrix);

		crViewMatterObject *firstPlayer = crSceneManager::getInstance()->getPlayer();

		//crQuat orientation;
		//orientation.set(currentCameraBindNode->getMatrix());
		//float x,y,z;
		//orientation.getRadians(x,y,z);

		CRCore::crVector3f up = (Z_AXIS*firstPlayer->getMatterObjectRotation()).normalize();
		up[1] = 0.0f;
		up.normalize();
		float z = acos(up * Z_AXIS);

		if(up[0]>=0.0f)
			z = PI * 2.0f -z;
		else if(up[0]<0.0f)
			z = z;
	
		//int R = CRCore::RadiansToDegrees(z);
		//char tmpText[4];
		//sprintf(tmpText,"%d\0",R);
		//m_text->setText(tmpText);

		//CRText::crText *dd = dynamic_cast<CRText::crText*>(crFilterRenderManager::getInstance()->getDrawable("Debug1"));
		//sprintf(tmpText,"%1.3f,%1.3f,%1.3f\0",up[0],up[1],up[2]);
		//dd->setText(tmpText);

		crVector3 dir = -firstPlayer->getDirection();
		if(dir[1]>0.0f)
			z = -z;
		crMatrixTransform *matrixTransNode = dynamic_cast<crMatrixTransform *>(node);
		if(matrixTransNode)
			matrixTransNode->setMatrix(crMatrix::rotate(z,CRCore::Z_AXIS));
		crNodeCallback::traverse(node,nv);
	}
}
////////////////////////////////
//crScalarBarCallback
//CullCallback
///////////////////////////////////
crScalarBarCallback::crScalarBarCallback():m_init(false),m_orientation(HORIZONTAL),m_rate(0.0f)
{
}
crScalarBarCallback::crScalarBarCallback(const crScalarBarCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_orientation(callback.m_orientation),
m_rate(callback.m_rate),
m_texMat(callback.m_texMat),
m_texMat_buf(callback.m_texMat_buf),
m_stateset(callback.m_stateset),
m_stateset_buf(callback.m_stateset_buf),
m_init(callback.m_init)
{
}
void crScalarBarCallback::init()
{
	m_texMat = new CRCore::crTexMat;
	m_stateset = new crStateSet;
	m_stateset->setTextureAttributeAndModes(0,m_texMat.get(),crStateAttribute::ON);

	m_texMat_buf = new CRCore::crTexMat;
	m_stateset_buf = new crStateSet;
	m_stateset_buf->setTextureAttributeAndModes(0,m_texMat_buf.get(),crStateAttribute::ON);
}

void crScalarBarCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		if(str.compare("VERTICAL") == 0)
		    m_orientation = VERTICAL;
		else m_orientation = HORIZONTAL;
		break;
	case 1:
	    m_rate = atof(str.c_str());
		break;
	}
}

void crScalarBarCallback::operator()(crNode* node, crNodeVisitor* nv)
{
	if(!m_init)
	{
		init();
		m_init = true;
	}
	CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
	if (cv&&cv->getRenderMode() == CRUtil::crCullVisitor::NormalRender)
	{
		crMatrix& texMat = m_texMat->getMatrix();
		texMat.makeIdentity();

        //计算比率
		if(m_orientation==HORIZONTAL)
		{
			texMat.setTrans(m_rate,0.0,0.0);
		}
		else
		{
            texMat.setTrans(0.0,m_rate,0.0);
		}

		bool stateset_pushed = cv->pushStateSet(m_stateset.get());
		crNodeCallback::traverse(node,nv);
		if(stateset_pushed) cv->popStateSet();
	}
}

void crScalarBarCallback::swapBuffers(int frameNumber)
{
	if(m_swapFrameNumber != frameNumber)
	{
		ref_ptr<crStateSet> stateset = m_stateset;
		m_stateset = m_stateset_buf;
		m_stateset_buf = stateset;

		ref_ptr<crTexMat> texMat = m_texMat;
		m_texMat = m_texMat_buf;
		m_texMat_buf = texMat;

		crNodeCallback::swapBuffers(frameNumber);
		m_swapFrameNumber = frameNumber;
	}
}
//////////////////////////////////
////crFlyGameViewFilterCallback
////CullCallback
/////////////////////////////////////
//crFlyGameViewFilterCallback::crFlyGameViewFilterCallback():
//m_init(false),
//m_show(2),
//m_radarHeight(100000.0f),
//m_useWeaponEachTarget(false),
////m_previousFrameTime(0.0f),
//m_time_info(5.0f),
//m_interval_info(0.0f)
//{
//	m_lastWeaponName = "Def";
//}
//
//crFlyGameViewFilterCallback::crFlyGameViewFilterCallback(const crFlyGameViewFilterCallback& callback):
//crNodeCallback(callback),
//m_init(callback.m_init),
//m_show(callback.m_show),
//m_radarHeight(callback.m_radarHeight),
//m_useWeaponEachTarget(callback.m_useWeaponEachTarget),
////m_previousFrameTime(callback.m_previousFrameTime),
//m_time_info(callback.m_time_info),
//m_interval_info(callback.m_interval_info)
//{
//	m_lastWeaponName = "Def";
//}
//
//void crFlyGameViewFilterCallback::setRadarHeight(float radarHeight)
//{
//    m_radarHeight = radarHeight;
//}
//
//void crFlyGameViewFilterCallback::addEventParam(int i, const std::string &str)
//{
//	switch(i) 
//	{
//	case 0:
//		crArgumentParser::readKeyValue(str,"VEC3",m_targetDistancePos);
//		break;
//	case 1:
//		crArgumentParser::readKeyValue(str,"VEC3",m_targetNamePos);
//		break;
//	case 2:
//		crArgumentParser::readKeyValue(str,"VEC3",m_targetTypePos);
//		break;
//	case 3:
//		m_show = atoi(str.c_str());
//		break;
//	case 4:
//		m_radarHeight = atof(str.c_str());
//		break;
//	case 5:
//		m_useWeaponEachTarget = atoi(str.c_str());
//		break;
//	case 6:
//		m_time_info = atof(str.c_str());
//		break;
//	default:
//		{
//			std::string typestr,idstr;
//			int id;
//			std::string::size_type comma = str.find_first_of(',');
//			typestr = std::string(str.begin(),str.begin()+comma);
//			idstr = std::string(str.begin()+comma + 1,str.end());
//			id = atoi(idstr.c_str());
//			m_stringidMap.insert(std::make_pair(typestr,id));
//		}
//	//default:
//	//	{
//	//		crVector3 pos;
//	//		crArgumentParser::readKeyValue(str,"VEC3",pos);
//	//		addNeutralBuildPositions(pos);
//	//	}
//	}
//}
//
//void crFlyGameViewFilterCallback::init()
//{
//	crFilterRenderManager *filterRenderManager = crFilterRenderManager::getInstance();
//	m_compassH = dynamic_cast<CRText::crText*>(filterRenderManager->getDrawable("CompassH"));
//	m_time = dynamic_cast<CRText::crText*>(filterRenderManager->getDrawable("Time"));
//	m_alt = dynamic_cast<CRText::crText*>(filterRenderManager->getDrawable("ALT"));
//	m_speed = dynamic_cast<CRText::crText*>(filterRenderManager->getDrawable("Speed"));
//	m_score = dynamic_cast<CRText::crText*>(filterRenderManager->getDrawable("Score"));
//	m_level = dynamic_cast<CRText::crText*>(filterRenderManager->getDrawable("Level"));
//	m_hp = dynamic_cast<CRText::crText*>(filterRenderManager->getDrawable("HP"));
//	m_defend = dynamic_cast<CRText::crText*>(filterRenderManager->getDrawable("Defend"));
//	m_weaponName = dynamic_cast<CRText::crText*>(filterRenderManager->getDrawable("WeaponName"));
//	m_bulletCount = dynamic_cast<CRText::crText*>(filterRenderManager->getDrawable("BulletCount"));
//	/////////////////////target
//	m_targetGroup = dynamic_cast<crGroup *>(filterRenderManager->getFilterNode("TargetGroup"));
//	char tmpText[20];
//	for (int i = 0; i< 10; i++)
//	{
//		sprintf(tmpText,"TargetDistance%d\0",i);
//		m_targetDistance[i] = dynamic_cast<CRText::crText*>(filterRenderManager->getDrawable(tmpText));
//		sprintf(tmpText,"TargetName%d\0",i);
//		m_targetName[i] = dynamic_cast<CRText::crText*>(filterRenderManager->getDrawable(tmpText));
//		sprintf(tmpText,"TargetType%d\0",i);
//		m_targetType[i] = dynamic_cast<CRText::crText*>(filterRenderManager->getDrawable(tmpText));
//	}
//	m_lockedTarget_air = filterRenderManager->getFilterNode("DefLTAir");
//	m_lockedOverTarget_air = filterRenderManager->getFilterNode("DefLTOAir");
//	m_trackTarget_air = filterRenderManager->getFilterNode("DefTTAir");
//
//	m_lockedTarget_gnd = filterRenderManager->getFilterNode("DefLTGnd");
//	m_lockedOverTarget_gnd = filterRenderManager->getFilterNode("DefLTOGnd");
//	m_trackTarget_gnd = filterRenderManager->getFilterNode("DefTTGnd");
//
//	m_invalidTarget_air = filterRenderManager->getFilterNode("InvalidTargetAir");
//	m_invalidTarget_gnd = filterRenderManager->getFilterNode("InvalidTargetGnd");
//
//	m_friendTarget_air = filterRenderManager->getFilterNode("FriendTargetAir");
//	m_friendTarget_gnd = filterRenderManager->getFilterNode("FriendTargetGnd");
//	////////////////////////////
//	m_radarGroup = dynamic_cast<crGroup *>(filterRenderManager->getFilterNode("RadarGroup"));
//
//	m_hpGuage = filterRenderManager->getFilterNode("HPGuage");
//	m_bulletGuage = filterRenderManager->getFilterNode("BulletGuage");
//
//	////////////////////radar
//	m_radar = dynamic_cast<CRCore::crMatrixTransform*>(filterRenderManager->getFilterNode("Radar"));
//	if(m_radar.valid())
//	{//
//		m_radar->setMatrix(crMatrix::scale(1.0,
//			crDisplaySettings::instance()->getScreenWidth()/crDisplaySettings::instance()->getScreenHeight(),1.0));
//	}
//	m_friendAir = filterRenderManager->getFilterNode("FriendAir");
//	m_friendAir_jamming = filterRenderManager->getFilterNode("FriendAirJamming");
//	m_friendGround = filterRenderManager->getFilterNode("FriendGround");
//	m_friendBuild = filterRenderManager->getFilterNode("FriendBuild");
//	m_friendBuild_jamming = filterRenderManager->getFilterNode("FriendBuildJamming");
//
//	m_neutralAir = filterRenderManager->getFilterNode("NeutralAir");
//	m_neutralGround = filterRenderManager->getFilterNode("NeutralGround");
//	m_neutralBuild = filterRenderManager->getFilterNode("NeutralBuild");
//
//	m_enemyAir = filterRenderManager->getFilterNode("EnemyAir");
//	m_enemyAir_jamming = filterRenderManager->getFilterNode("EnemyAirJamming");
//	m_enemyGround = filterRenderManager->getFilterNode("EnemyGround");
//	m_enemyBuild = filterRenderManager->getFilterNode("EnemyBuild");
//	m_enemyBuild_jamming = filterRenderManager->getFilterNode("EnemyBuildJamming");
//	
//	m_radarProjectionMatrix.makePerspective(45,1.0,1.0,1e6);
//
//	float screenWidth = crDisplaySettings::instance()->getScreenWidth();
//	float screenHeight = crDisplaySettings::instance()->getScreenHeight();
//	m_identicMVPW = CRCore::crMatrix::translate(1.0f,1.0f,1.0f)*CRCore::crMatrix::scale(0.5f*screenWidth,0.5f*screenHeight,0.5f);
//	//m_identicMVPW.invert(MVPW);
//
//	m_cautionSW = dynamic_cast<crMultiSwitch *>(crFilterRenderManager::getInstance()->getFilterNode("Caution"));
//	m_viewFilterSwitch = dynamic_cast<crMultiSwitch *>(crFilterRenderManager::getInstance()->getFilterNode("ViewFilterSwitch"));
//	m_targetMovies = dynamic_cast<crMultiSwitch *>(crFilterRenderManager::getInstance()->getFilterNode("TargetMovies"));
//	if(m_targetMovies.valid()) m_targetMovies->setActiveSwitchSet(0);
//}
//
//void crFlyGameViewFilterCallback::setShow(int show)
//{
//    m_show = show;
//}
//
//#define RadarBot(node)\
//	if(m_radarFrustum.contains(radarPos))\
//    {\
//		radarPos = radarPos * radarMVP;\
//		matrixTransform = getOrCreateIdleMatrixTransform();\
//		radarPos[2] = 0.0f;\
//		radarMatrix = (*botItr)->getMatrix();\
//		radarMatrix.setTrans(radarPos);\
//		matrixTransform->setMatrix(radarMatrix);\
//		matrixTransform->addChild(node);\
//		m_radarGroup->addChild(matrixTransform);\
//	}
//
//void crFlyGameViewFilterCallback::operator()(crNode* node, crNodeVisitor* nv)
//{//m_show=0:正常显示，1：正常显示，但不显示导航仪表，2：全部隐藏，3～n：显示动画序列帧
//	if(!m_init)
//	{
//		init();
//		m_init = true;
//	}
//	CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
//	if (cv && cv->getRenderMode() == CRUtil::crCullVisitor::NormalRender)
//	{
//		CRCore::ScopedLock<CRCore::crMutex> lock(crFilterRenderManager::getInstance()->getNodeMutex());
//	//CRCore::notify(CRCore::WARN)<<"crFlyGameViewFilterCallback  "<<nv->getFrameStamp()->getFrameNumber()<<" "<<nv->getTraverseString()<<" m_show = "<<m_show<< std::endl;
//		if(m_viewFilterSwitch.valid()) m_viewFilterSwitch->setActiveSwitchSet(m_show);
//		if(m_show>1)
//		{
//			if(m_compassH.valid()) m_compassH->setVisiable(false);
//			if(m_time.valid()) m_time->setVisiable(false);
//			if(m_alt.valid())  m_alt->setVisiable(false);
//			if(m_speed.valid()) m_speed->setVisiable(false);
//			if(m_score.valid()) m_score->setVisiable(false);
//			if(m_level.valid()) m_level->setVisiable(false);
//			if(m_hp.valid()) m_hp->setVisiable(false);
//			if(m_defend.valid()) m_defend->setVisiable(false);
//			if(m_weaponName.valid()) m_weaponName->setVisiable(false);
//			if(m_bulletCount.valid()) m_bulletCount->setVisiable(false);
//
//			for(int index = 0; index<10; index++)
//			{
//				if(m_targetDistance[index].valid()) m_targetDistance[index]->setVisiable(false);
//				if(m_targetName[index].valid()) m_targetName[index]->setVisiable(false);
//				if(m_targetType[index].valid()) m_targetType[index]->setVisiable(false);
//			}
//			crNodeCallback::traverse(node,nv);
//			return;
//		}
//		if(m_compassH.valid()) m_compassH->setVisiable(true);
//
//		//CRCore::notify(CRCore::WARN)<<"crFlyGameViewFilterCallback  "<<m_show<<" m_viewFilterSwitch.valid() "<<m_viewFilterSwitch.valid()<< std::endl;	
//
//		crViewMatterObject* body = dynamic_cast<crViewMatterObject*>(node);
//		bool isFirstPlayer = body == crSceneManager::getInstance()->getPlayer();
//		if( !body || !isFirstPlayer)
//		{
//			crNodeCallback::traverse(node,nv);
//			return;
//		}
//		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
//		if( !bodyInfo )
//		{
//			crNodeCallback::traverse(node,nv);
//			return;
//		}
//
//		//CRCore::notify(CRCore::WARN)<<"crFlyGameViewFilterCallback  1"<< std::endl;	
//
//		if(m_cautionSW.valid())
//		{
//			if(body->getVelocity()[0] <= bodyInfo->getStallVelocity())
//			{
//				m_cautionSW->setActiveSwitchSet(7);
//			}
//			else if(m_cautionSW->getActiveSwitchSet() == 7)
//			{
//				m_cautionSW->setActiveSwitchSet(0);
//			}
//		}
//		crVector3 bodyPos = body->getBound().center();
//
//		char tmpText[30];
//		//float time = nv->getFrameStamp()->getReferenceTime();
//		//float interval = (m_previousFrameTime != 0.0f) ? (time-m_previousFrameTime) : 0.0f;
//		float interval = nv->getFrameStamp()->getFrameInterval();
//        m_interval_info += interval;
//		//m_previousFrameTime = time;
//        if(m_interval_info>m_time_info)
//		{
//			int numEnemyBot = crBotManager::getInstance()->getNumEnemyBot();
//			sprintf(tmpText,"Enemy remain %d\0",numEnemyBot);
//			crFilterRenderManager::getInstance()->addDequeString(tmpText);
//			if(numEnemyBot == 1)
//			{
//				crViewMatterObject *enemy = (*crBotManager::getInstance()->getEnemyBotVec().begin()).get(); 
//                //crFilterRenderManager::getInstance()->addDequeString("Remain enemy is " + enemy->getName());
//				crVector3 enemyPos = enemy->getBound().center();
//                sprintf(tmpText,"Enemy Distance is %5.2f\0",(bodyPos - enemyPos).length());
//				crFilterRenderManager::getInstance()->addDequeString(tmpText);
//			}
//			m_interval_info = 0.0f;
//		}
//		if(m_time.valid())
//		{
//			float time = nv->getFrameStamp()->getReferenceTime();
//			float th = time / 3600.0f;
//			float h = floor(th);
//			float tm = (th - h) * 60.0f;
//			float m = floor(tm);
//			float s = (tm - m) * 60.0f;
//			sprintf(tmpText,"%d:%d:%d\0",(int)h,(int)m,(int)s);
//			m_time->setText(tmpText);
//			m_time->setVisiable(true);
//		}
//		if(m_alt.valid())
//		{
//			sprintf(tmpText,"%d\0",(int)(bodyPos[2]));
//		    m_alt->setText(tmpText);
//			m_alt->setVisiable(true);
//		}
//		if(m_speed.valid())
//		{
//			crVector3f& speed = body->getVelocity();
//			sprintf(tmpText,"%d\0",(int)(speed[0]));
//			m_speed->setText(tmpText);
//			m_speed->setVisiable(true);
//		}
//		if(m_score.valid())
//		{
//			sprintf(tmpText,"%d\0",bodyInfo->getExperience());
//			m_score->setText(tmpText);
//			m_score->setVisiable(true);
//		}
//		if(m_level.valid())
//		{
//			sprintf(tmpText,"%d\0",bodyInfo->getLevel());
//			m_level->setText(tmpText);
//			m_level->setVisiable(true);
//		}
//		if(m_hp.valid())
//		{
//			sprintf(tmpText,"%d\0",bodyInfo->getHP());
//			m_hp->setText(tmpText);
//			m_hp->setVisiable(true);
//		}
//		if(m_defend.valid())
//		{
//			sprintf(tmpText,"%d\0",bodyInfo->getDefend());
//			m_defend->setText(tmpText);
//			m_defend->setVisiable(true);
//		}
//		crWeaponMatterObject *currentWeapon = body->getCurrentWeaponMatterObject();
//		int bulletsInWeapon = 0;
//		int maxBulletsInOneClip = 1;
//		if(!currentWeapon)
//		{
//			if(m_bulletCount.valid()) m_bulletCount->setVisiable(false);
//			if(m_weaponName.valid()) m_weaponName->setVisiable(false);
//		}
//		else
//		{
//			if(m_weaponName.valid()) 
//			{
//				m_weaponName->setVisiable(true);
//				m_weaponName->setText(currentWeapon->getName());
//			}
//			if(m_bulletCount.valid())
//			{
//				bulletsInWeapon = currentWeapon->getBulletsInWeapon();
//				maxBulletsInOneClip = CRCore::maximum(1,currentWeapon->getWeaponObject()->getMaxBulletsInOneClip());
//				if(bulletsInWeapon > -1)
//				{
//					m_bulletCount->setVisiable(true);
//					sprintf(tmpText,"%3d\0",bulletsInWeapon);
//					m_bulletCount->setText(tmpText);
//				}
//				else
//					m_bulletCount->setVisiable(false);
//			}
//		}
//
//		if(m_useWeaponEachTarget)
//		{
//			std::string currentWeaponName = currentWeapon->getName();
//			if(m_lastWeaponName.compare(currentWeaponName)!=0)
//			{
//				crFilterRenderManager *filterRenderManager = crFilterRenderManager::getInstance();
//				m_lastWeaponName = currentWeaponName;
//
//				m_lockedTarget_air = filterRenderManager->getFilterNode(m_lastWeaponName+"LTAir");
//				m_lockedOverTarget_air = filterRenderManager->getFilterNode(m_lastWeaponName+"LTOAir");
//				m_trackTarget_air = filterRenderManager->getFilterNode(m_lastWeaponName+"TTAir");
//
//				m_lockedTarget_gnd = filterRenderManager->getFilterNode(m_lastWeaponName+"LTGnd");
//				m_lockedOverTarget_gnd = filterRenderManager->getFilterNode(m_lastWeaponName+"LTOGnd");
//				m_trackTarget_gnd = filterRenderManager->getFilterNode(m_lastWeaponName+"TTGnd");
//			}
//		}
//
//		//crScalarBarCallback ScalarBarCallback;
//		crScalarBarCallback *sb;
//		float rate;
//		if(m_hpGuage.valid())
//		{
//            sb = dynamic_cast<crScalarBarCallback *>(m_hpGuage->getCullCallback("ScalarBar"));
//			rate = (float)bodyInfo->getHP() / (float)bodyInfo->getMaxHP();
//			if(sb) sb->updateRate(rate);
//		}
//		if(m_bulletGuage.valid())
//		{
//			sb = dynamic_cast<crScalarBarCallback *>(m_bulletGuage->getCullCallback("ScalarBar"));
//			rate = (float)bulletsInWeapon / (float)maxBulletsInOneClip;
//			//CRCore::notify(CRCore::WARN)<<"crFlyGameViewFilterCallback rate "<<rate<< std::endl;
//			if(sb) sb->updateRate(rate);
//		}
//		crMatrix MVP = cv->getModelViewMatrix() * cv->getProjectionMatrix();
//
//		//////////////////////////
//		//update Locked Info
//		/////////////////////////
//		unsigned int currentWeaponMask = body->getCurrentWeaponMask();
//		//bodyInfo->updateLockedTarget(body->getTargetCount(),currentWeaponMask);
//		crGameBodyInfo::DetectMap &detectMap = bodyInfo->getDetectMap();
//		crViewMatterObject *targetBody;
//		crMatrixTransform* matrixTransform;
//        //bodyInfo->lockBodyInfo();
//		crGameBodyInfo::LockedTargetSet &lockedSet = bodyInfo->getLockedTargetSet();
//	    std::string typeStr;
//		if(m_targetMovies.valid() && !lockedSet.empty())
//		{
//			int currentMovies = m_targetMovies->getActiveSwitchSet();
//			targetBody = lockedSet.begin()->get();
//
//			int currentLock = 0;
//			if(crArgumentParser::readKeyValue(targetBody->getDescriptions(),"TypeString",typeStr))
//			{
//				StringIDMap::iterator itr = m_stringidMap.find(typeStr);
//				if(itr != m_stringidMap.end())
//                    currentLock = itr->second;
//			}
//
//			if(currentLock != currentMovies)
//			{
//				m_targetMovies->setActiveSwitchSet(currentLock);
//                if(currentLock!=0)
//				{
//					crNode *currentActiveNode = m_targetMovies->getFirstActiveChild();
//					crSequence *sequence = dynamic_cast<crSequence *>(currentActiveNode);
//                    if(sequence)
//						sequence->startEffect();
//				}
//			}
//		}
//		//CRCore::notify(CRCore::WARN)<<"crFlyGameViewFilterCallback lockedSet "<<lockedSet.size()<<" detectMap = "<<detectMap.size()<< std::endl;
//
//		m_targetGroup->removeChild(0,m_targetGroup->getNumChildren());
//	    int i = 0;
//		int maxDetectCount = bodyInfo->getMaxDetectCount(); 
//
//		crMatrix invertMat;
//		invertMat.invert(cv->getModelViewMatrix() * cv->getCameraViewInverseMatrix());
//		crMatrix convertMatrix = invertMat * (MVP);
//
//		if(currentWeapon && m_targetGroup.valid() && (!lockedSet.empty() || !detectMap.empty()))
//		{
//			crMatrix matrix;
//			float distance;
//			crVector3 targetPos;
//			crMatrix tmpMVPW;
//
//			float attackDistance = currentWeapon->getWeaponObject()->getAttackDistance();
//
//			unsigned int targetMask;
//			for( crGameBodyInfo::LockedTargetSet::iterator itr = lockedSet.begin();
//				itr != lockedSet.end();
//				++itr,i++ )
//			{
//				targetBody = itr->get();
//
//				matrixTransform = getOrCreateIdleMatrixTransform();
//				targetPos = targetBody->getBound().center();
//				distance = (targetPos - bodyPos).length();
//				targetPos =  targetPos * convertMatrix;
//				//将position转换到Identic的坐标
//				targetPos[2] = 0.0f;
//				matrix = crMatrix::translate(targetPos);
//				//CRCore::notify(CRCore::WARN)<<"crFlyGameViewFilterCallback targetPos "<<targetPos<<" m_lockedTarget_air "<<m_lockedTarget_air.get()<< std::endl;
//
//				matrixTransform->setMatrix(matrix);
//				targetMask = targetBody->getAttributeMask();
//				if(distance>attackDistance)
//				{
//					if(targetMask & crGroup::AttrAir)
//					    matrixTransform->addChild(m_lockedOverTarget_air.get());
//					else if(targetMask & crGroup::AttrGround)
//                        matrixTransform->addChild(m_lockedOverTarget_gnd.get());
//					else matrixTransform->addChild(m_lockedOverTarget_gnd.get());
//				}
//				else
//				{
//					if(targetMask & crGroup::AttrAir)
//						matrixTransform->addChild(m_lockedTarget_air.get());
//					else if(targetMask & crGroup::AttrGround)
//						matrixTransform->addChild(m_lockedTarget_gnd.get());
//					else matrixTransform->addChild(m_lockedTarget_gnd.get());
//				}
//				m_targetGroup->addChild(matrixTransform);
//
//				tmpMVPW = matrix * m_identicMVPW;
//				if(m_targetDistance[i].valid())
//				{
//					m_targetDistance[i]->setPosition(m_targetDistancePos * tmpMVPW);
//					sprintf(tmpText,"%4.2f\0",distance);
//					m_targetDistance[i]->setText(tmpText);
//					m_targetDistance[i]->setVisiable(true);
//				}
//				if(m_targetName[i].valid())
//				{
//					m_targetName[i]->setPosition(m_targetNamePos * tmpMVPW);
//					m_targetName[i]->setText(targetBody->getName());
//					m_targetName[i]->setVisiable(true);
//				}
//				if(m_targetType[i].valid())
//				{
//					if(crArgumentParser::readKeyValue(targetBody->getDescriptions(),"TypeString",typeStr))
//					{
//                        m_targetType[i]->setPosition(m_targetTypePos * tmpMVPW);
//						m_targetType[i]->setText(typeStr);
//						m_targetType[i]->setVisiable(true);
//					}
//					else m_targetType[i]->setVisiable(false);
//				}
//			}
//
//			///////////
//			for( crGameBodyInfo::DetectMap::iterator itr = detectMap.begin();
//				itr != detectMap.end() && i<maxDetectCount;
//				++itr, i++)
//			{
//				targetBody = itr->second.get();
//
//				//if(lockedSet.find(targetBody) != lockedSet.end())
//				//	continue;
//				matrixTransform = getOrCreateIdleMatrixTransform();
//				targetPos = targetBody->getBound().center();
//				distance = (targetPos - bodyPos).length();
//				targetPos = targetPos * convertMatrix;
//				targetPos[2] = 0.0f;
//				//将position转换到Identic的坐标
//				matrix = crMatrix::translate(targetPos);
//				matrixTransform->setMatrix(matrix);
//				targetMask = targetBody->getAttributeMask();
//				if(currentWeaponMask & targetMask)
//				{//
//					if(targetMask & crGroup::AttrAir)
//						matrixTransform->addChild(m_trackTarget_air.get());
//					else if(targetMask & crGroup::AttrGround)
//						matrixTransform->addChild(m_trackTarget_gnd.get());
//					else matrixTransform->addChild(m_trackTarget_gnd.get());
//				}
//				else
//				{//invalidTarget
//					if(targetMask & crGroup::AttrAir)
//						matrixTransform->addChild(m_invalidTarget_air.get());
//					else if(targetMask & crGroup::AttrGround)
//						matrixTransform->addChild(m_invalidTarget_gnd.get());
//					else matrixTransform->addChild(m_invalidTarget_gnd.get());
//				}
//				m_targetGroup->addChild(matrixTransform);
//
//				tmpMVPW = matrix * m_identicMVPW;
//				if(m_targetDistance[i].valid())
//				{
//					m_targetDistance[i]->setPosition(m_targetDistancePos * tmpMVPW);
//					sprintf(tmpText,"%4.2f\0",distance);
//					m_targetDistance[i]->setText(tmpText);
//					m_targetDistance[i]->setVisiable(true);
//				}
//				if(m_targetName[i].valid())
//				{
//					m_targetName[i]->setPosition(m_targetNamePos * tmpMVPW);
//					m_targetName[i]->setText(targetBody->getName());
//					m_targetName[i]->setVisiable(true);
//				}
//				if(m_targetType[i].valid())
//				{
//					if(crArgumentParser::readKeyValue(targetBody->getDescriptions(),"TypeString",typeStr))
//					{
//						m_targetType[i]->setPosition(m_targetTypePos * tmpMVPW);
//						m_targetType[i]->setText(typeStr);
//						m_targetType[i]->setVisiable(true);
//					}
//					else m_targetType[i]->setVisiable(false);
//				}
//			}
//		}
//		if(i < maxDetectCount)
//		{
//			for(int index = i; index<maxDetectCount; index++)
//			{
//				if(m_targetDistance[index].valid()) m_targetDistance[index]->setVisiable(false);
//				if(m_targetName[index].valid()) m_targetName[index]->setVisiable(false);
//				if(m_targetType[index].valid()) m_targetType[index]->setVisiable(false);
//			}
//		}
//		//detectMap.clear();
//		//bodyInfo->unlockBodyInfo();
//
//		/////////队友的显示
//        
//
//		////////////////////////
//        //radar
//		/////////////////////////
//		if(m_radarGroup.valid())
//		{
//			m_radarGroup->removeChild(0,m_radarGroup->getNumChildren());
//
//			crVector3 radarEye = bodyPos;
//			radarEye[2] += m_radarHeight;
//			crVector3 radarLookAt = radarEye;
//			radarLookAt[2] -= 1.0f;
//			m_radarViewMatrix.makeLookAt(radarEye,radarLookAt,CRCore::Y_AXIS);
//			CRCore::crMatrix radarMVP = m_radarViewMatrix * m_radarProjectionMatrix;
//			m_radarFrustum.setToUnitFrustum(true,true);
//			m_radarFrustum.transformProvidingInverse(radarMVP);
//
//			m_radarFrustum.pushCurrentMask();
//			//////////////////////////////////
//			//botManager
//			///////////////////////////////
//			crBotManager *botManager = crBotManager::getInstance();
//			
//			crVector3f radarPos;
//			crPolytope& currentViewFrustum= cv->getCurrentCullingSet().getFrustum();
//			if(m_neutralBuild.valid())
//			{
//				crBotManager::StaticPositionVec::iterator buildItr;
//				crBotManager::StaticPositionVec &staticNeutrals = botManager->getNeutralStaticPositions();
//				for( buildItr = staticNeutrals.begin();
//					buildItr != staticNeutrals.end();
//					++buildItr )
//				{
//					radarPos = (*buildItr);
//					if(m_radarFrustum.contains(radarPos))
//					{
//						//从世界坐标转换到Identic坐标
//						radarPos = radarPos * radarMVP;
//						matrixTransform = getOrCreateIdleMatrixTransform();
//						radarPos[2] = 0.0f;
//						//CRCore::notify(CRCore::WARN)<<"crFlyGameViewFilterCallback radarPos "<<radarPos<< std::endl;
//						matrixTransform->setMatrix(crMatrix::translate(radarPos));
//						matrixTransform->addChild(m_neutralBuild.get());
//						m_radarGroup->addChild(matrixTransform);
//					}
//					//RadarDisplay(radarPos,m_neutralBuild.get())
//				}
//			}
//
//			crBotManager::BotSet& enemyBots = botManager->getEnemyBotVec();
//			crBotManager::BotSet& friendBots = botManager->getFriendBotVec();
//			crBotManager::BotSet& neutralBots = botManager->getNeutralBotVec();
//			crBotManager::BotSet& netEnemyBots = botManager->getNetEnemyBotVec();
//			crBotManager::BotSet& netFriendBots = botManager->getNetFriendBotVec();
//			crBotManager::BotSet& netNeutralBots = botManager->getNetNeutralBotVec();
//			crBotManager::BotSet::iterator botItr;
//			crMatrix radarMatrix;
//			if(m_enemyAir.valid()/*||m_enemyAir_jamming.valid()*/)
//			{
//                for( botItr = enemyBots.begin();
//					 botItr != enemyBots.end();
//					 ++botItr )
//				{
//					if(!(*botItr)->isDead())
//					{
//						radarPos = (*botItr)->getBound().center();
//						RadarBot(m_enemyAir.get())
//					}
//					//CRCore::notify(CRCore::WARN)<<"crFlyGameViewFilterCallback m_enemyAir "<<radarPos<< std::endl;
//					//if(m_radarFrustum.contains(radarPos))
//					//{
//					//	//从世界坐标转换到Identic坐标
//					//	radarPos = radarPos * radarMVP;
//					//	matrixTransform = getOrCreateIdleMatrixTransform();
//					//	radarPos[2] = 0.0f;
//					//	//CRCore::notify(CRCore::WARN)<<"crFlyGameViewFilterCallback radarPos "<<radarPos<< std::endl;
//     //                   radarMatrix = (*botItr)->getMatrix();
//					//	radarMatrix.setTrans(radarPos);
//					//	matrixTransform->setMatrix(radarMatrix);
//					//	matrixTransform->addChild(m_enemyAir.get());
//					//	m_radarGroup->addChild(matrixTransform);
//					//}
//				}
//				for( botItr = netEnemyBots.begin();
//					botItr != netEnemyBots.end();
//					++botItr )
//				{
//					if(!(*botItr)->isDead())
//					{
//						radarPos = (*botItr)->getBound().center();
//						RadarBot(m_enemyAir.get())
//					}
//				}
//			}
//			if(m_friendAir.valid())
//			{
//				crViewMatterObject* cameraBot = crSceneManager::getInstance()->getPlayer();
//				radarPos = cameraBot->getBound().center();
//				radarPos = radarPos * radarMVP;
//				matrixTransform = getOrCreateIdleMatrixTransform();
//				radarPos[2] = 0.0f;
//				//CRCore::notify(CRCore::WARN)<<"crFlyGameViewFilterCallback radarPos "<<radarPos<< std::endl;
//				radarMatrix = cameraBot->getMatrix();
//				radarMatrix.setTrans(radarPos);
//				matrixTransform->setMatrix(radarMatrix);
//				matrixTransform->addChild(m_friendAir.get());
//				m_radarGroup->addChild(matrixTransform);
//
//				for( botItr = friendBots.begin();
//					botItr != friendBots.end();
//					++botItr )
//				{
//					if(!(*botItr)->isDead())
//					{
//						radarPos = (*botItr)->getBound().center();
//						RadarBot(m_friendAir.get())
//					}
//				}
//				for( botItr = netFriendBots.begin();
//					botItr != netFriendBots.end();
//					++botItr )
//				{
//					if(!(*botItr)->isDead())
//					{
//						radarPos = (*botItr)->getBound().center();
//						RadarBot(m_friendAir.get())
//					}
//				}
//			}
//			if(m_neutralAir.valid())
//			{
//				for( botItr = neutralBots.begin();
//					botItr != neutralBots.end();
//					++botItr )
//				{
//					if(!(*botItr)->isDead())
//					{
//						radarPos = (*botItr)->getBound().center();
//						RadarBot(m_neutralAir.get())
//					}
//				}
//				for( botItr = netNeutralBots.begin();
//					botItr != netNeutralBots.end();
//					++botItr )
//				{
//					if(!(*botItr)->isDead())
//					{
//						radarPos = (*botItr)->getBound().center();
//						RadarBot(m_neutralAir.get())
//					}
//				}
//			}
//			m_radarFrustum.popCurrentMask();
//		}
//
//		crNodeCallback::traverse(node,nv);
//	}
//	else crNodeCallback::traverse(node,nv);
//}
//////////////////////////////////
////crTerrainDecalCallback
////UpdateCallback
/////////////////////////////////////
//crTerrainDecalCallback::crTerrainDecalCallback():
//m_frameNumber(0),
//m_width(20.0f),
//m_height(20.0f),
//m_offset(0.1f)
//{
//}
//crTerrainDecalCallback::crTerrainDecalCallback(const crTerrainDecalCallback& callback):
//crNodeCallback(callback),
//m_width(callback.m_width),
//m_height(callback.m_height),
//m_offset(callback.m_offset),
//m_frameNumber(0)
//{
//}
//
//void crTerrainDecalCallback::addEventParam(int i, const std::string &str)
//{
//	switch(i)
//	{
//	case 0:
//		m_width = atof(str.c_str());
//		break;
//	case 1:
//		m_height = atof(str.c_str());
//		break;
//	case 2:
//		m_offset = atof(str.c_str());
//		break;
//	}
//}
//
//void crTerrainDecalCallback::operator()(crNode* node, crNodeVisitor* nv)
//{
//	CRUtil::crUpdateVisitor* uv = dynamic_cast<CRUtil::crUpdateVisitor*>(nv);
//	if (uv)
//	{
//		int frameNumber = nv->getFrameStamp()->getFrameNumber();
//		if(m_frameNumber < frameNumber)
//		{//避免单帧内重复更新
//			m_frameNumber = frameNumber;
//			if(!m_terrain.valid())
//			{
//				CRCore::crSearchNodeBYClassNameVisitor searchByClassNameVisitor;
//				searchByClassNameVisitor.insertClassNameId("crTerrainTile");
//				CRCore::crBrain::getInstance()->accept(searchByClassNameVisitor);
//				m_terrain = dynamic_cast<crTerrainTile *>(searchByClassNameVisitor.getResult());
//			}
//			if(m_terrain.valid())
//			{
//				CRTerrain::crHeightFieldLayer *hfl = dynamic_cast<CRTerrain::crHeightFieldLayer *>(m_terrain->getElevationLayer());
//				if(hfl)
//				{
//					crVector3 centerPoint;
//					crMatrix matrix;
//					if(dynamic_cast<crTransform *>(node))
//					{
//						dynamic_cast<crTransform *>(node)->computeLocalToWorldMatrix(matrix,nv);
//						centerPoint = matrix.getTrans();
//					}
//					else
//					{
//						centerPoint = node->getBound().center();
//					}
//					crHeightField *grid = hfl->getHeightField();
//
//					crVector3 size(m_width / grid->getXInterval(),m_height / grid->getYInterval(),1.0f);
//					//size = size * matrix;
//					int numColumns = size[0];
//					int numRows = size[1];
//					if(numColumns<2) numColumns = 2;
//					if(numRows<2) numRows = 2;
//					int c_2 = numColumns>>1;
//					int r_2 = numRows>>1;
//
//					crVector3 origin = grid->getOrigin();
//					//crVector3 centerPoint = matrix.getTrans();
//					crVector3 pos(centerPoint[0],centerPoint[1],0.0f);
//
//					if(!m_heightField.valid())
//						m_heightField = new crHeightField;
//
//					m_heightField->allocate(numColumns,numRows);
//					m_heightField->setXInterval(grid->getXInterval());
//					m_heightField->setYInterval(grid->getYInterval());
//					m_heightField->setOrigin(crVector3(pos[0] - c_2 * grid->getXInterval(),pos[1] - r_2 * grid->getYInterval(),0.0f));
//
//					pos -= origin;
//					pos[0] /= grid->getXInterval();
//					pos[1] /= grid->getYInterval();
//					//crVector3 terCoord(0.0f,0.0f,1.0f);
//					//matrix.setTrans(crVector3(0.0f,0.0f,0.0f));
//					int s1,t1;
//					for(int s = 0; s<numColumns; ++s)
//					{
//						for(int t=0;t<numRows;++t)
//						{
//							//terCoord[0] = s + pos[0] - c_2;
//							//terCoord[1] = t + pos[1] - r_2;
//							//terCoord = terCoord * matrix;
//							s1 = s + pos[0] - c_2;
//							t1 = t + pos[1] - r_2;
//							m_heightField->setHeight(s,t,grid->getHeight(s1,t1) + m_offset);
//						}
//					}
//					if(!m_shapeDrawable.valid())
//					{
//						m_shapeDrawable = new crShapeDrawable(m_heightField.get());
//						m_shapeDrawable->setColor(crVector4(1.0f,0.0f,0.0f,1.0f));
//
//						crStateSet *stateset = m_shapeDrawable->getOrCreateStateSet();
//						stateset->setMode(GL_LIGHTING,crStateAttribute::OFF);
//						stateset->setMode(GL_CULL_FACE,crStateAttribute::OFF);
//						stateset->setMode(GL_DEPTH_TEST,CRCore::crStateAttribute::OFF);
//
//						stateset->setRenderBinDetails(1,"StateSortedBin");
//						crLineWidth *lineWidth = new crLineWidth(1.0f);
//						stateset->setAttributeAndModes(lineWidth,crStateAttribute::ON);
//						crPolygonMode *polygonMode = new crPolygonMode;
//						polygonMode->setMode(crPolygonMode::FRONT_AND_BACK,crPolygonMode::LINE);
//						stateset->setAttributeAndModes(polygonMode,crStateAttribute::ON);
//						//crDepth *depth = new crDepth(crDepth::ALWAYS,0.0,1.0,true);
//						//stateset->setAttributeAndModes(depth,crStateAttribute::ON);
//
//						//crTexture2D *tex2d = new crTexture2D;
//						//tex2d->setImage(CRIOManager::readImageFile(m_textureName));
//						//stateset->setTextureAttributeAndModes(TEXTURE_BASEMAP,tex2d,crStateAttribute::ON);
//						//crAlphaFunc *alphaFunc = new crAlphaFunc;
//						//alphaFunc->setFunction(crAlphaFunc::GREATER,0.5f);
//						//stateset->setAttributeAndModes(alphaFunc,crStateAttribute::ON);
//						//stateset->setRenderingHint(CRCore::crStateSet::ALPHATEST_BIN);
//						m_shapeDrawable->setUseDisplayList(false);
//					}
//					if(!m_object)
//					{
//						m_object = new crObject;
//						m_object->addDrawable(m_shapeDrawable.get());
//						m_object->setCollideMode(crNode::CollideNone);
//						m_object->setName(node->getName());
//						m_object->setCalcShadow(false);
//						CRCore::crTraverseStringNode *tsNode = new CRCore::crTraverseStringNode;
//						tsNode->setName("NoLight");
//						tsNode->addChild(m_object.get());
//						tsNode->addDescription("#ScriptAddNode");
//
//						CRIOManager::crLoadManager::getInstance()->requestAddNode(crSceneManager::getInstance()->getSceneRoot(),tsNode,true);
//					}
//					m_shapeDrawable->dirtyBound();
//				}
//			}
//		}
//	}
//	crNodeCallback::traverse(node,nv);
//}
////////////////////////////////
//crSeqTextureTileCallback
//UpdateCallback
///////////////////////////////////
crSeqTextureTileCallback::crSeqTextureTileCallback():
m_previousTraversalNumber(0),
m_s_tile(1.0f),
m_t_tile(1.0f),
m_num_tile(1),
m_cur_tile(0),
m_s_coord(0.0f),
m_t_coord(0.0f),
m_object(NULL),
m_drawable(NULL)
{
	m_seq = new CRCore::crSequence;
}
crSeqTextureTileCallback::crSeqTextureTileCallback(const crSeqTextureTileCallback& callback,const CRCore::crCopyOp&copyop):
crNodeCallback(callback,copyop),
m_previousTraversalNumber(0),
m_s_tile(callback.m_s_tile),
m_t_tile(callback.m_t_tile),
m_num_tile(callback.m_num_tile),
m_cur_tile(callback.m_cur_tile),
m_s_coord(callback.m_s_coord),
m_t_coord(callback.m_t_coord),
m_object(NULL),
m_drawable(NULL)
{
	if(callback.m_seq.valid()) m_seq = dynamic_cast<crSequence *>(callback.m_seq->clone(crCopyOp::SHALLOW_COPY));
}

crSequence *crSeqTextureTileCallback::getSequence()
{
	return m_seq.get();
}
void crSeqTextureTileCallback::addEventParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		{
			crVector2i vec;
			crArgumentParser::appAtoVec(str,vec);
			setTextureTile(vec[0],vec[1]);
		}
		break;
	case 1:
		{
			m_num_tile = atoi(str.c_str());
		}
		break;
	}
	if(i>1)
        m_seq->addParam(i-2,str);
}
void crSeqTextureTileCallback::setTextureTile(int sTile, int tTile, int numTiles)
{
	m_s_tile = (sTile>0) ? 1.0f / static_cast<float>(sTile) : 1.0f;
	m_t_tile = (tTile>0) ? 1.0f / static_cast<float>(tTile) : 1.0f;
	if (numTiles <= 0)
	{
		m_num_tile = sTile * tTile;
	}
	else
	{
		m_num_tile = numTiles;
	}
}
int crSeqTextureTileCallback::getTileS() const
{
	return (m_s_tile>0.0f) ? static_cast<int>(1.0f / m_s_tile) : 1;
}
int crSeqTextureTileCallback::getTileT() const
{
	return (m_t_tile>0.0f) ? static_cast<int>(1.0f / m_t_tile) : 1;
}
void crSeqTextureTileCallback::swapBuffers(int frameNumber)
{
	if(m_swapFrameNumber != frameNumber)
	{
		if(m_drawable)
		{
			CRCore::Vec2Array* texCoords = dynamic_cast<CRCore::Vec2Array *>(m_drawable->getTexCoordArray(0));
			(*texCoords)[0].set(m_s_coord,m_t_coord);
			(*texCoords)[1].set(m_s_coord+m_s_tile, m_t_coord);
			(*texCoords)[2].set(m_s_coord+m_s_tile, m_t_coord+m_t_tile);
			(*texCoords)[3].set(m_s_coord, m_t_coord+m_t_tile);
		}
		crNodeCallback::swapBuffers(frameNumber);
		m_swapFrameNumber = frameNumber;
	}
}
void crSeqTextureTileCallback::operator()(crNode* node, crNodeVisitor* nv)
{
	m_object = dynamic_cast<crObject *>(node);
	if(!m_object)
	{
		CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::crSeqTextureTileCallback(): 该callback必须是object类型节点的updatecallback"<<std::endl;
		return;
	}
	if(dynamic_cast<crBillboard *>(node))
		m_drawable = dynamic_cast<crGeometry *>(dynamic_cast<crBillboard *>(node)->getComposeDrawable());
	else
		m_drawable = dynamic_cast<crGeometry *>(m_object->getDrawable(0));
	m_drawable->setUseDisplayList(false);
	m_drawable->setUseVertexBufferObjects(false);
	m_seq->setName(m_object->getName());

	CRUtil::crUpdateVisitor* uv = dynamic_cast<CRUtil::crUpdateVisitor*>(nv);
	if (uv && nv->getTraversalNumber() != m_previousTraversalNumber)
	{
		m_previousTraversalNumber = nv->getTraversalNumber();
		m_object->setVisiable(m_seq->getVisiable());
		if(m_object->getVisiable())
		{
			m_seq->update(/*nv->getFrameStamp()*/);
			m_cur_tile = m_seq->getValue();
			m_s_coord = m_s_tile * fmod(m_cur_tile , 1.0f / m_s_tile);
			m_t_coord = 1.0f - m_t_tile * (static_cast<int>(m_cur_tile * m_s_tile) + 1);
		}
	}
	crNodeCallback::traverse(node,nv);
}