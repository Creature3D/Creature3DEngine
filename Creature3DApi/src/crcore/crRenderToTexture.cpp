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
#include <CRCore/crRenderToTexture.h>
#include <CRCore/crRenderToTextureManager.h>
#include <CRCore/crShaderManager.h>
#include <CRCore/crMath.h>
#include <CRCore/crBrain.h>
#include <CRCore/crLightSourceManager.h>
#include <CRCore/crNotify.h>
#include <CRCore/crDepth.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crBrain.h>
using namespace CRCore;

crPolytope crRenderToTexture::m_currentViewFrustum;

crRenderToTexture::crRenderToTexture():
    m_needUpd(true),
	m_lastUpdFrameNumber(-1),
	m_lightRange(100.0f),
	m_cameraFov(45.0),
	m_lightSource(NULL),
	m_shadowRadius(0.0f),
	m_shadowOffset(0.0f),
	m_lastUpdateTime(0.0f),
	m_updateInterval(0.04f),
	m_enable(false)
{
    m_cameraDir.set(0.0,0.0,-1.0);
	m_cameraUp = Y_AXIS;
	m_mapSize.set(256,256);
}

crRenderToTexture::crRenderToTexture(const crRenderToTexture&,const crCopyOp& copyop)
{

}

crRenderToTexture::~crRenderToTexture()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crRenderToTexture()"<< std::endl;
}
void crRenderToTexture::setEnable(bool enable)
{
	if(m_enable != enable)
	{
		m_enable = enable;
	}
	if(m_enable && m_texturedGroup.valid())
	{
		crStateSet *ss = m_texturedGroup->getStateSet();
		for( NodeArray::iterator itr = m_texturedGroup->getChildArray().begin();
			itr != m_texturedGroup->getChildArray().end();
			++itr )
		{
			(*itr)->setStateSet(ss);
		}
	}
}
bool crRenderToTexture::getEnable()
{
	return m_enable;
}
void crRenderToTexture::setShadowRadius(float shadowRadius)
{
    m_shadowRadius = shadowRadius;
}

void crRenderToTexture::setShadowOffset(float shadowOffset)
{
	m_shadowOffset = shadowOffset;
}

//void crRenderToTexture::setRenderProgrom(const std::string& filename, bool addT, bool addB)
//{
//	if(!m_renderGroup.valid())
//		return;
//	crStateSet *ss = crBrain::getInstance()->getProgramStateSet(filename,addT,addB);
//	m_renderGroup->setStateSet(ss);
//
//	ss->setMode(GL_LIGHTING, crStateAttribute::OVERRIDE|crStateAttribute::OFF);
//	if(m_mode & RenderDepthMap)
//		ss->setIsShadowStateSet(true);
//	if(m_mode & RenderShadowMap)
//		ss->setIsShadowStateSet(true);
//}
void crRenderToTexture::setProjectionMatrix(const crMatrix &proj)
{
	m_projection = proj;
}
void crRenderToTexture::setViewMatrix(const crMatrix &view)
{
	m_view = view;
}
void crRenderToTexture::computeMVP(/*const crVector3 & eyePoint,const crVector3 & center*/)
{
	if(m_mode & CalcMVPByRenderData)
	{
		const crBoundingSphere& bs = m_renderGroup->getBound();
		crVector3f center = bs.center();
		double r = bs.radius();
		//crVector3f dir = m_cameraPos;
		//dir.normalize();
  //      dir[2] = 0.0;
  //      dir.normalize();
		crVector3f dir = Y_AXIS;
		crVector3f pos = center + dir * r;
		crVector3f dir1 = pos - m_cameraPos;
		pos = center - dir * r;
		crVector3f dir2 = pos - m_cameraPos;
		dir1.normalize();
		dir2.normalize();
		m_cameraFov = RadiansToDegrees(acos(dir1 * dir2));
		m_cameraDir = (dir1 + dir2) * 0.5;
		if(m_lightSource)
		{
			m_lightSource->setCosLightMapFov(m_cameraDir * dir1);
            m_lightSource->setLSDirection(m_cameraDir);
		}
	}
	else if(m_mode & CalcMVPByShadowRadius)
	{
		const crMatrix &viewMatrix = crRenderToTextureManager::getInstance()->getCurrentViewMatrix();
		crVector3 eye,center,up,lv;
		viewMatrix.getLookAt(eye,center,up);
		lv = center - eye;
		lv.normalize();
		m_cameraUp = lv;

		const crVector3 &nearPoint = crRenderToTextureManager::getInstance()->getNearPoint();
        center = nearPoint + lv * (m_shadowRadius - m_shadowOffset);
		//crVector3f dir = m_cameraPos;
		//dir.normalize();
		//dir[2] = 0.0;
		//dir.normalize();
        crVector3f dir = Y_AXIS;
		crVector3f pos = center + dir * m_shadowRadius;
		crVector3f dir1 = pos - m_cameraPos;
		pos = center - dir * m_shadowRadius;
		crVector3f dir2 = pos - m_cameraPos;
		dir1.normalize();
		dir2.normalize();
		m_cameraFov = RadiansToDegrees(acos(dir1 * dir2));
		m_cameraDir = (dir1 + dir2) * 0.5;
		if(m_lightSource)
		{
			m_lightSource->setCosLightMapFov(m_cameraDir * dir1);
			m_lightSource->setLSDirection(m_cameraDir);
		}
	}
	else if(m_mode & CalcMVPByEyePoint)
	{//用于计算sun的shadow
		//计算shadow_camera的视锥
		const crMatrix &viewMatrix = crRenderToTextureManager::getInstance()->getCurrentViewMatrix();
		crVector3 eye,center,up,lv;
		viewMatrix.getLookAt(eye,center,up);
		//eye[2] = 0;
		lv = center - eye;
        lv.normalize();
		//m_cameraUp = lv;//(Y_AXIS*viewMatrix).normalize();
		if(fabsf(lv[2]) != 1.0f)
		{
			lv[2] = 0.0f;
			lv.normalize();
		}
		//center[2] = eye[2];
		const crVector3 &nearPoint = crRenderToTextureManager::getInstance()->getNearPoint();
		crVector3f spos = nearPoint - lv * crShaderManager::getInstance()->getSunShadowDistanceOffset()[0];
		crVector3f dir1 = spos - m_cameraPos;
		crVector3f epos = nearPoint + lv * crShaderManager::getInstance()->getSunShadowDistance()[0];
		crVector3f dir2 = epos - m_cameraPos;
		dir1.normalize();
		dir2.normalize();

		m_cameraFov = RadiansToDegrees(acos(dir1 * dir2));
		m_cameraDir = (dir1 + dir2) * 0.5;
		//m_cameraUp
        crVector3 lv2 = lv*crMatrix::rotate(CRCore::PI_2,CRCore::Z_AXIS);
		crVector3 centerpos = (epos + spos) * 0.5f;
		crVector3 hpos0 = centerpos + lv2;
		crVector3 hpos1 = centerpos - lv2;
		crVector3 hdir1 = hpos0 - m_cameraPos;
		hdir1.normalize();
		crVector3 hdir2 = hpos1 - m_cameraPos;
		hdir2.normalize();
		m_cameraUp = hdir2^hdir1;
		//m_cameraUp = lv;//lv2^m_cameraDir;

		if(m_lightSource)
		{
			m_lightSource->setCosLightMapFov(m_cameraDir * dir1);
			m_lightSource->setLSDirection(m_cameraDir);
		}

		//crMatrix rotmat = viewMatrix;
  //      rotmat.setTrans(0.0f,0.0f,0.0f);
		//m_cameraUp = (Z_AXIS*rotmat).normalize();

	 //   CRCore::notify(CRCore::ALWAYS)<<"m_cameraPos:"<<m_cameraPos<<std::endl;
		//CRCore::notify(CRCore::ALWAYS)<<"m_cameraFov:"<<m_cameraFov<<std::endl;
		//CRCore::notify(CRCore::ALWAYS)<<"m_cameraDir:"<<m_cameraDir<<std::endl;
	}
	else if(m_mode & CalcMVPByEyePoint2)
	{//用于计算sun的shadow
		//计算shadow_camera的视锥
		const crMatrix &viewMatrix = crRenderToTextureManager::getInstance()->getCurrentViewMatrix();
		crVector3d eye,center,up,lv;
		viewMatrix.getLookAt(eye,center,up);

		//eye[2] = 0;
		lv = center - eye;
		lv.normalize();
		//m_cameraUp = lv;//(Y_AXIS*viewMatrix).normalize();
		if(fabsf(lv[2]) != 1.0f)
		{
			lv[2] = 0.0f;
			lv.normalize();
		}
		//center[2] = eye[2];
		const crVector3 &nearPoint = crRenderToTextureManager::getInstance()->getNearPoint();
		crVector3f spos = nearPoint + lv * crShaderManager::getInstance()->getSunShadowDistance()[0] - lv * crShaderManager::getInstance()->getSunShadowDistanceOffset()[1];
		crVector3f dir1 = spos - m_cameraPos;
		crVector3f epos = nearPoint + lv * crShaderManager::getInstance()->getSunShadowDistance()[1];
		crVector3f dir2 = epos - m_cameraPos;
		dir1.normalize();
		dir2.normalize();

		m_cameraFov = RadiansToDegrees(acos(dir1 * dir2));
		m_cameraDir = (dir1 + dir2) * 0.5;
		//m_cameraUp
		crVector3 lv2 = lv*crMatrix::rotate(CRCore::PI_2,CRCore::Z_AXIS);
		crVector3 centerpos = (epos + spos) * 0.5f;
		crVector3 hpos0 = centerpos + lv2;
		crVector3 hpos1 = centerpos - lv2;
		crVector3 hdir1 = hpos0 - m_cameraPos;
		hdir1.normalize();
		crVector3 hdir2 = hpos1 - m_cameraPos;
		hdir2.normalize();
		m_cameraUp = hdir2^hdir1;


		if(m_lightSource)
		{
			m_lightSource->setCosLightMapFov(m_cameraDir * dir1);
			m_lightSource->setLSDirection(m_cameraDir);
		}

		//crMatrix rotmat = viewMatrix;
		//      rotmat.setTrans(0.0f,0.0f,0.0f);
		//m_cameraUp = (Z_AXIS*rotmat).normalize();

		//   CRCore::notify(CRCore::ALWAYS)<<"m_cameraPos:"<<m_cameraPos<<std::endl;
		//CRCore::notify(CRCore::ALWAYS)<<"m_cameraFov:"<<m_cameraFov<<std::endl;
		//CRCore::notify(CRCore::ALWAYS)<<"m_cameraDir:"<<m_cameraDir<<std::endl;
	}
	else
	{//CalcMVPByEyeSetting
		if(m_lightSource)
		{
			m_lightSource->setCosLightMapFov(cos(DegreesToRadians(m_cameraFov * 0.5f)));
		}
	}
	m_projection.makePerspective(m_cameraFov,m_mapSize[0]/m_mapSize[1]/*crDisplaySettings::instance()->getAspectRatio()*/,1.0f,m_lightRange);
	m_view.makeLookAt(m_cameraPos,m_cameraPos + m_cameraDir,m_cameraUp);

	if(m_lightSource)
	{
		if(m_mode & CalcMVPByEyePoint2)
	        m_lightSource->setLightMapMVP2(m_view*m_projection);
		else
			m_lightSource->setLightMapMVP(m_view*m_projection);
	}
}

void crRenderToTexture::createCamera()
{
	m_camera = new crCameraNode;
	m_camera->setName(getName());
	m_camera->setEnableIntersect(false);

	if(m_mode & RenderDepthMap)
	{
		m_camera->setRenderMode(crCameraNode::ShadowMapRender);
		m_camera->setClearColor(crVector4(1.0f, 1.0f, 1.0f, 1.0f));
		m_camera->getOrCreateStateSet()->setStateSetMode(crStateSet::SS_ShadowMapRender);
	}
	else if(m_mode & RenderCollideMap)
	{
		m_camera->setRenderMode(crCameraNode::CollideMapRender);
		m_camera->setClearColor(crVector4(0.0f, 0.0f, 0.0f, 1.0f));
		m_camera->getOrCreateStateSet()->setStateSetMode(crStateSet::SS_CollideMapRender);
	}
	else if(m_mode & NRPRender)
	{
		m_camera->setRenderMode(crCameraNode::NRPRender);
		m_camera->setClearColor(crVector4(0.0f, 0.0f, 0.0f, 1.0f));
		float w = crDisplaySettings::instance()->getViewWidth();
		float h = crDisplaySettings::instance()->getViewHeight();
		m_camera->setViewport(0,0,w,h);
	}
	else if (m_mode & RenderGIMap)
	{
		m_camera->setRenderMode(crCameraNode::GIMapRender);
		m_camera->setClearColor(crVector4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		m_camera->setRenderMode(crCameraNode::RTTRender);
		m_camera->setClearColor(crVector4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	m_camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_camera->setComputeNearFarMode(crCameraNode::DO_NOT_COMPUTE_NEAR_FAR);

    m_camera->setRenderOrder(crCameraNode::PRE_RENDER);

	if(m_mode & crRenderToTexture::RenderToFrame)
		m_camera->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER);
	else
		m_camera->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER_OBJECT);

	m_camera->setDrawBuffer(GL_BACK);
	m_camera->setReadBuffer(GL_BACK);
	m_camera->setReferenceFrame(crCameraNode::RELATIVE_TO_ABSOLUTE);

	m_camera->setCullingMode( crCameraNode::VIEW_FRUSTUM_CULLING |
		crCameraNode::SMALL_FEATURE_CULLING/* |
		crCameraNode::CLUSTER_CULLING */);

	m_camera->setProjectionMatrix(crMatrix::identity());
	m_camera->setBufProjectionMatrix(crMatrix::identity());
	m_camera->setViewMatrix(crMatrix::identity());
	m_camera->setBufViewMatrix(crMatrix::identity());

    m_camera->setViewport(0,0,m_mapSize[0],m_mapSize[1]);

	if(m_mode & crRenderToTexture::RenderToImage)
	{
		m_camera->attach(crCameraNode::COLOR_BUFFER, getImage());
		m_camera->setPostDrawCallback(m_writeImageCallback.get());
	}
	if(m_mode & crRenderToTexture::RenderToTexture)
	{
		m_camera->attach(crCameraNode::COLOR_BUFFER, getTexture(),0,0,false);
	}

	m_camera->addChild(m_renderGroup.get());
}
crCameraNode *crRenderToTexture::getCameraNode()
{
	return m_camera.get();
}
//void renderTexture(crGroup *node,crTexture2D *texture,int x,int y)
//{
//	crObject* object = new crObject;
//	crGroup *group = new crGroup;
//	crGeometry* geom = createTexturedQuadGeometry(crVector3(0,0,0),crVector3(400.0,0.0,0.0),crVector3(0.0,400.0,0.0));
//	geom->getOrCreateStateSet()->setTextureAttributeAndModes(0,texture,crStateAttribute::ON);
//	geom->getOrCreateStateSet()->setMode(GL_LIGHTING,crStateAttribute::OFF);
//	object->addDrawable(geom);
//
//	crCameraNode* camera = new crCameraNode;
//
//	camera->setRenderMode(crCameraNode::NormalRender);
//	camera->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER);
//	camera->setComputeNearFarMode(crCameraNode::DO_NOT_COMPUTE_NEAR_FAR);
//	// set the projection matrix
//	camera->setProjectionMatrix(crMatrix::ortho2D(0,400,0,400));
//	camera->setBufProjectionMatrix(crMatrix::ortho2D(0,400,0,400));
//	// set the view matrix    
//	camera->setReferenceFrame(crTransform::RELATIVE_TO_ABSOLUTE);
//	camera->setViewMatrix(crMatrix::identity());
//	camera->setBufViewMatrix(crMatrix::identity());
//
//	camera->setViewport(x,y,400,400);
//
//	// only clear the depth buffer
//	camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	// draw subgraph after main camera view.
//	camera->setRenderOrder(crCameraNode::POST_RENDER);
//
//	camera->addChild(object);
//
//	group->addChild(camera);
//	group->setEnableIntersect(false);
//	node->addChild(group);
//}
void crRenderToTexture::createBlurCamera(float blurSize)
{
	float glowSize = blurSize;
	m_texture_vblur = new crTexture2D;
	float w = m_mapSize[0];
	float h = m_mapSize[1];
	int maxAnisotropy = crDisplaySettings::instance()->getMaxAnisotropy();
	//CRCore::notify(CRCore::FATAL)<<" crRenderToTexture::createBlurCamera: w = "<<w<<" h = "<<h<<std::endl;
	m_texture_vblur->setTextureSize(w/glowSize,h/glowSize);
	m_texture_vblur->setInternalFormat(m_texture->getInternalFormat());//GL_RGB16F_ARB
	//m_texture_vblur->setSourceFormat(GL_RGBA);
	//m_texture_vblur->setSourceType(GL_FLOAT);
	m_texture_vblur->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
	m_texture_vblur->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
	m_texture_vblur->setMaxAnisotropy(16.0);
	m_texture_vblur->setWrap(crTexture2D::WRAP_S, crTexture2D::CLAMP_TO_EDGE);
	m_texture_vblur->setWrap(crTexture2D::WRAP_T, crTexture2D::CLAMP_TO_EDGE);

	m_vblurCamera = new crCameraNode;
	m_vblurCamera->setName(getName()+"_vblur");
	m_vblurCamera->setEnableIntersect(false);
	m_vblurCamera->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER_OBJECT);
	m_vblurCamera->setComputeNearFarMode(crCameraNode::DO_NOT_COMPUTE_NEAR_FAR);
	m_vblurCamera->setRenderOrder(crCameraNode::POST_RENDER);
	m_vblurCamera->setDrawBuffer(GL_BACK);
	m_vblurCamera->setReadBuffer(GL_BACK);
	m_vblurCamera->setReferenceFrame(crCameraNode::RELATIVE_TO_ABSOLUTE);
	m_vblurCamera->setCullingMode(crCameraNode::NO_CULLING);

	m_vblurCamera->setProjectionMatrix(crMatrix::identity()/*crMatrix::ortho2D(0,w,0,h)*/);
	m_vblurCamera->setBufProjectionMatrix(crMatrix::identity()/*crMatrix::ortho2D(0,w,0,h)*/);
	m_vblurCamera->setViewMatrix(crMatrix::identity());
	m_vblurCamera->setBufViewMatrix(crMatrix::identity());

	m_vblurCamera->setViewport(0,0,w/glowSize,h/glowSize);

	m_vblurCamera->attach(crCameraNode::COLOR_BUFFER, m_texture_vblur.get(),0,0,false);

	ref_ptr<crStateSet> vblur_ss = new crStateSet;
	//vblur_ss->setMode(GL_LIGHTING,crStateAttribute::OFF);
	crStateSet *shader = crShaderManager::getInstance()->getShaderStateSet("gaussian");
	crUniform *uniform = vblur_ss->getOrCreateUniform("invSize",CRCore::crUniform::FLOAT_VEC2);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(crVector2(glowSize / w, 0));
	vblur_ss->setTextureAttributeAndModes(0,m_texture.get(),crStateAttribute::ON);
	vblur_ss->merge(*shader);
	//m_vblurCamera->setStateSet(vblur_ss.get());

	ref_ptr<crObject> object = new crObject;
	object->setName("rttvblur");
	object->setEnableIntersect(false);
	ref_ptr<crGeometry> quad = createTexturedQuadGeometry(crVector3(-1,-1,0),crVector3(2,0.0,0.0),crVector3(0.0,2,0.0));
	object->addDrawable(quad.get());
	object->setStateSet(vblur_ss.get());

	m_vblurCamera->addChild(object.get());

	//////h_blur
	m_texture_hblur = new crTexture2D;
	m_texture_hblur->setTextureSize(w/glowSize,h/glowSize);
	m_texture_hblur->setInternalFormat(m_texture->getInternalFormat());//GL_RGB16F_ARB
	//m_texture_hblur->setSourceFormat(GL_RGBA);
	//m_texture_hblur->setSourceType(GL_FLOAT);
	m_texture_hblur->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
	m_texture_hblur->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
	m_texture_hblur->setMaxAnisotropy(maxAnisotropy);
	m_texture_hblur->setWrap(crTexture2D::WRAP_S, crTexture2D::CLAMP_TO_EDGE);
	m_texture_hblur->setWrap(crTexture2D::WRAP_T, crTexture2D::CLAMP_TO_EDGE);

	m_hblurCamera = new crCameraNode;
	m_hblurCamera->setName(getName()+"_hblur");
	m_hblurCamera->setEnableIntersect(false);
	m_hblurCamera->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER_OBJECT);
	m_hblurCamera->setComputeNearFarMode(crCameraNode::DO_NOT_COMPUTE_NEAR_FAR);
	m_hblurCamera->setRenderOrder(crCameraNode::POST_RENDER);
	m_hblurCamera->setDrawBuffer(GL_BACK);
	m_hblurCamera->setReadBuffer(GL_BACK);
	m_hblurCamera->setReferenceFrame(crCameraNode::RELATIVE_TO_ABSOLUTE);
	m_hblurCamera->setCullingMode( crCameraNode::NO_CULLING );

	m_hblurCamera->setProjectionMatrix(crMatrix::identity()/*crMatrix::ortho2D(0,w,0,h)*/);
	m_hblurCamera->setBufProjectionMatrix(crMatrix::identity()/*crMatrix::ortho2D(0,w,0,h)*/);
	m_hblurCamera->setViewMatrix(crMatrix::identity());
	m_hblurCamera->setBufViewMatrix(crMatrix::identity());

	m_hblurCamera->setViewport(0,0,w/glowSize,h/glowSize);

	m_hblurCamera->attach(crCameraNode::COLOR_BUFFER, m_texture_hblur.get(),0,0,false);

	ref_ptr<crStateSet> hblur_ss = new crStateSet;
	//hblur_ss->setMode(GL_LIGHTING,crStateAttribute::OFF);
	uniform = hblur_ss->getOrCreateUniform("invSize",CRCore::crUniform::FLOAT_VEC2);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(crVector2(0, glowSize / h));
	hblur_ss->setTextureAttributeAndModes(0,m_texture_vblur.get(),crStateAttribute::ON);
	hblur_ss->merge(*shader);
	//m_hblurCamera->setStateSet(hblur_ss.get());

	object = new crObject;
	object->setName("rtthblur");
	object->setEnableIntersect(false);
	//quad = createTexturedQuadGeometry(crVector3(-1,-1,0),crVector3(2,0.0,0.0),crVector3(0.0,2,0.0));
	object->addDrawable(quad.get());
	object->setStateSet(hblur_ss.get());

	m_hblurCamera->addChild(object.get());

	m_vblurCamera->addDescription("#ScriptAddNode");
	//m_vblurCamera->setName("vblur_camera");
	m_hblurCamera->addDescription("#ScriptAddNode");
	//m_hblurCamera->setName("hblur_camera");

	m_camera->addChild(m_vblurCamera.get());
	m_camera->addChild(m_hblurCamera.get());

//renderTexture(crBrain::getInstance()->getCurrentActiveDB(),m_texture.get(),0,0);
//renderTexture(crBrain::getInstance()->getCurrentActiveDB(),m_texture_vblur.get(),405,0);
//renderTexture(crBrain::getInstance()->getCurrentActiveDB(),m_texture_hblur.get(),405,405);
}

void crRenderToTexture::init(/*float bias,*/ float blursize)
{
	if(m_lightSource)
	{
		crLight *light = dynamic_cast<crLight *>(m_lightSource->getLight());
		if(light->getLightType() == crLight::LIGHT_SPOT)
		{
			m_cameraFov = light->getSpotCutoff() * 2.0;
		}
	}

	crStateSet *ss;
    ref_ptr<crUniform> uniform;
	if(m_mode & RenderToImage)
	{
        if(!m_image.valid())
		{
			m_image = new crImage;
			m_image->setFileName(m_outPutImageName);
			m_image->allocateImage(m_mapSize[0], m_mapSize[1], 1, GL_RGB, GL_UNSIGNED_BYTE);
		}
	}
    if(m_mode & RenderToTexture)
	{
		if(!m_texture.valid())
		{
			m_texture = new crTexture2D;
			m_texture->setImageNameID("rtt"/*getName()*/);
			m_texture->setTextureSize(m_mapSize[0], m_mapSize[1]);
            if(m_mode & RenderDepthMap)
			{
				m_texture->setInternalFormat(GL_RGB16F_ARB/*crDisplaySettings::instance()->getShadowDetail()>2?GL_RGB32F_ARB:GL_RGB16F_ARB*/);//GL_RGB16F_ARB
				//m_texture->setSourceFormat(GL_RGB);
				//m_texture->setSourceType(GL_FLOAT);
				m_texture->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
				m_texture->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
				//m_texture->setMaxAnisotropy(16.0);
				m_texture->setWrap(crTexture2D::WRAP_S, crTexture2D::CLAMP_TO_EDGE);
				m_texture->setWrap(crTexture2D::WRAP_T, crTexture2D::CLAMP_TO_EDGE);
			}
			else if(m_mode & RenderCollideMap)
			{
				m_texture->setInternalFormat(GL_RGB16);//GL_LUMINANCE16_ALPHA16
				m_texture->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
				m_texture->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
				//m_texture->setMaxAnisotropy(16.0);
				m_texture->setWrap(crTexture2D::WRAP_S, crTexture2D::CLAMP_TO_EDGE);
				m_texture->setWrap(crTexture2D::WRAP_T, crTexture2D::CLAMP_TO_EDGE);
			}
			else if (m_mode & RenderGIMap)
			{
				m_texture->setInternalFormat(GL_RGBA16F_ARB);//GL_LUMINANCE16_ALPHA16
				m_texture->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
				m_texture->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
				//m_texture->setMaxAnisotropy(16.0);
				m_texture->setWrap(crTexture2D::WRAP_S, crTexture2D::CLAMP_TO_EDGE);
				m_texture->setWrap(crTexture2D::WRAP_T, crTexture2D::CLAMP_TO_EDGE);
			}
			else//RenderColorMap
			{
				m_texture->setInternalFormat(GL_RGB8);
				//m_texture->setSourceFormat(GL_RGB);
				//m_texture->setSourceType(GL_UNSIGNED_BYTE);
				m_texture->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
				m_texture->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
				//m_texture->setMaxAnisotropy(16.0);
				m_texture->setWrap(crTexture2D::WRAP_S, crTexture2D::CLAMP_TO_EDGE);
				m_texture->setWrap(crTexture2D::WRAP_T, crTexture2D::CLAMP_TO_EDGE);
			}
		}

		//if(m_lightSource)
		//	m_lightSource->setLightMap(m_texture.get());
	}

	if(m_mode & RenderDepthMap)
	{
		ss = m_renderGroup->getOrCreateStateSet();
		ss->setStateSetMode(crStateSet::SS_ShadowMapRender);
		//uniform = ss->getOrCreateUniform("computeAlphaShadow",crUniform::BOOL);
		//uniform->setDataVariance(crBase::STATIC);
		//uniform->set(false);

		uniform = ss->getOrCreateUniform("scale",crUniform::FLOAT);
		uniform->setDataVariance(crBase::STATIC);
		uniform->set(1.0f/m_lightRange);

		//uniform = ss->getOrCreateUniform("bias",crUniform::FLOAT);
		//uniform->setDataVariance(crBase::STATIC);
		//uniform->set(bias);
		//CRCore::notify(CRCore::FATAL)<<"crRenderToTexture::init: "<<bias<<std::endl;

		uniform = ss->getOrCreateUniform("lightViewInverse",crUniform::FLOAT_MAT4);

		if(m_lightSource)
		{
			m_lightSource->setLightRangeScale(1.0f/m_lightRange);
		    m_lightSource->setInvShadowmapSize(blursize/double(maximum(m_mapSize[0],m_mapSize[1])));
		}

		ss->setMode(GL_LIGHTING,crStateAttribute::OFF);
	 //   ss->setMode(GL_BLEND,CRCore::crStateAttribute::OFF);
		//ss->setMode(GL_ALPHA_TEST,CRCore::crStateAttribute::OFF);
		//ss->setRenderBinDetails(1,"RenderBin");
		//crCullFace* cullFace = new crCullFace(crCullFace::FRONT_AND_BACK);
		//ss->setAttributeAndModes(cullFace,crStateAttribute::ON);

		//ss->setMode(GL_DEPTH_TEST, CRCore::crStateAttribute::OFF);
		//crDepth* depth = new crDepth;
		//depth->setFunction(crDepth::LEQUAL);
		//ss->setAttribute(depth,crStateAttribute::ON);
	}
	else if(!(m_mode & NRPRender))
	{
		if(!m_renderGroup)
			m_renderGroup = new crGroup;
		ss = m_renderGroup->getOrCreateStateSet();
		ss->getOrCreateUniform("CRE_InverseViewMatrix",crUniform::FLOAT_MAT4);
		if(m_mode & RenderCollideMap)
			ss->setStateSetMode(crStateSet::SS_CollideMapRender);
	}

	if(!(m_mode & RenderCubeMap) && !(m_mode & ExternMVP) && !(m_mode & CameraMVP))
	    computeMVP();

	//////for 室外 sunlight
	//if(m_texturedGroup.valid())
	//{
	//	if(m_mode & RenderDepthMap)
	//	{
	//		uniform = new CRCore::crUniform(crUniform::FLOAT_MAT4,"lightMapMVP");
	//		//uniform->set( m_view * m_projection );
	//	}
	//	for( NodeArray::iterator itr = m_texturedGroup->getChildArray().begin();
	//		itr != m_texturedGroup->getChildArray().end();
	//		++itr )
	//	{
	//		ss = (*itr)->getOrCreateStateSet();
	//		if(m_mode & RenderDepthMap)
	//		{
	//			if(!ss->getUniform("lightMapMVP"))
	//				ss->addUniform(uniform.get());
	//			ss->setTextureAttributeAndModes(TEXTURE_LIGHTMAP,m_texture.get(),crStateAttribute::ON);
	//		}
	//		else
	//		{
	//			ss->setTextureAttributeAndModes(TEXTURE_BASEMAP,m_texture.get(),crStateAttribute::ON);
	//		}
	//	}
	//}

	if((m_mode & RenderToTexture) || (m_mode & RenderToImage) || (m_mode & RenderToFrame))
		createCamera();
	if(m_mode & NeedBlur)
	{
        createBlurCamera(blursize);
		if(m_lightSource)
			m_lightSource->setLightMap(m_texture_hblur.get());
	}
	else
	{
		if(m_lightSource)
		{
			if(m_mode & CalcMVPByEyePoint2)
				m_lightSource->setLightMap2(m_texture.get());
			else
				m_lightSource->setLightMap(m_texture.get());
		}
	}

	//////////
	if(m_texturedGroup.valid())
	{
		crStateSet *ss;
		ref_ptr<crUniform> uniform;
		if(m_mode & RenderDepthMap)
		{
			uniform = new CRCore::crUniform(crUniform::FLOAT_MAT4,"lightMapMVP");
		}
		crTexture2D *tex = getBlurTexture();
		if(!tex) tex = getTexture();
		ss = m_texturedGroup->getOrCreateStateSet();
		if(m_mode & RenderDepthMap)
		{
			if(!ss->getUniform("lightMapMVP"))
				ss->addUniform(uniform.get());
			ss->setTextureAttributeAndModes(TEXTURE_LIGHTMAP,tex,crStateAttribute::ON);
		}
		else
		{
			ss->setTextureAttributeAndModes(TEXTURE_BASEMAP,tex,crStateAttribute::ON);
		}
	}
	//if(m_mode & CalcMVPByShadowRadius)
	//{
 //       crLightSourceManager::getInstance()->setSunShadowDistance(getShadowLength());
	//}
	setEnable(true);
}

void crRenderToTexture::addRenderNode(crNode *render)
{
	if(!render) return;

	if(!m_renderGroup.valid())
		m_renderGroup = new crGroup;
	m_renderGroup->addChild(render);
}

void crRenderToTexture::removeRenderNode(int index)
{
	if(!m_renderGroup.valid()) return;
	m_renderGroup->removeChild(index);
}

void crRenderToTexture::removeRenderNode(const std::string &nodeName)
{
	if(!m_renderGroup.valid()) return;
	for( NodeArray::iterator itr = m_renderGroup->getChildArray().begin();
		itr != m_renderGroup->getChildArray().end();
		++itr )
	{
		if(nodeName.compare((*itr)->getName()) == 0)
		{
			m_renderGroup->removeChild(itr->get());
			break;
		}
	}
}
crNode *crRenderToTexture::findRenderNode(const std::string &nodeName)
{
	if(!m_renderGroup.valid()) return NULL;
	for( NodeArray::iterator itr = m_renderGroup->getChildArray().begin();
		itr != m_renderGroup->getChildArray().end();
		++itr )
	{
		if(nodeName.compare((*itr)->getName()) == 0)
		{
			return itr->get();
		}
	}
	return NULL;
}
void crRenderToTexture::removeRenderNode(crNode *render)
{
	if(!m_renderGroup.valid()) return;
	m_renderGroup->removeChild(render);
}

void crRenderToTexture::createTexturedRoot()
{
	if(!m_texturedGroup.valid())
		m_texturedGroup = new crGroup;
}
void crRenderToTexture::addTexturedNode(crNode *textured)
{
	if(!textured) return;

	if(!m_texturedGroup.valid())
		m_texturedGroup = new crGroup;
	if(m_texturedGroup->addChild(textured))
		textured->setStateSet(m_texturedGroup->getStateSet());
}

void crRenderToTexture::removeTexturedNode(int index)
{
	if(!m_texturedGroup.valid()) return;
	m_texturedGroup->removeChild(index);
}

void crRenderToTexture::removeTexturedNode(const std::string &nodeName)
{
	if(!m_texturedGroup.valid()) return;
	for( NodeArray::iterator itr = m_texturedGroup->getChildArray().begin();
		itr != m_texturedGroup->getChildArray().end();
		++itr )
	{
		if(nodeName.compare((*itr)->getName()) == 0)
		{
			m_texturedGroup->removeChild(itr->get());
			break;
		}
	}
}

void crRenderToTexture::removeTexturedNode(crNode *textured)
{
	if(!m_texturedGroup.valid()) return;
	m_texturedGroup->removeChild(textured);
}

void crRenderToTexture::setMapSize(const crVector2i &mapSize)
{
	m_mapSize = mapSize;
}

void crRenderToTexture::setMapSize(int width, int height)
{
	m_mapSize.set(width,height);
}

void crRenderToTexture::setBlurSize(const crVector2f &size)
{
	m_blurSize = size;
}

void crRenderToTexture::setBlurSize(float h, float v)
{
	m_blurSize.set(h,v);
}

bool crRenderToTexture::getNeedUpd(/* const crFrameStamp *fs */)
{
	int frameNumber = crFrameStamp::getInstance()->getFrameNumber();
	if(m_mode & RealTimeUpd)
	{
		if(crFrameStamp::getInstance()->getReferenceTime() - m_lastUpdateTime > m_updateInterval)
		{
			m_lastUpdateTime = crFrameStamp::getInstance()->getReferenceTime();
			m_lastUpdFrameNumber = frameNumber;
            return true;
		}
		return false;
	}
	else if(m_mode & UpdOnce)
	{
		if(m_lastUpdFrameNumber<0)
		{
            m_lastUpdFrameNumber = frameNumber;
			return true;
		}
		else if(m_lastUpdFrameNumber < frameNumber)
		{
			m_needUpd = false;
		}
		return m_needUpd;
	}
	else if(m_mode & UpdByExtern)
	{
		if(m_needUpd)
		{ 
			m_lastUpdFrameNumber = frameNumber;
			return true;
		}
	}
	return false;
}

void crRenderToTexture::setUpdateInterval(float interval)
{//给一个最小的0.001s间隔，可以防止在VR模式下，右眼重复渲染，优化性能
	m_updateInterval = CRCore::maximum(0.001f,interval);
}
float crRenderToTexture::getUpdateInterval() const
{
    return m_updateInterval;
}

void crRenderToTexture::update(/*const crVector3 & eyePoint,const crVector3 & center*/)
{
    if(m_lightSource)
	{
		m_cameraPos = m_lightSource->getLSPosition();
		//CRCore::notify(CRCore::FATAL)<<"m_cameraPos = "<<m_cameraPos<<std::endl;
    	m_cameraDir = m_lightSource->getLSDirection();

			//const CRCore::crVector4& lightPosition = light->getPosition();
			//m_cameraPos.set(lightPosition[0],lightPosition[1],lightPosition[2]);
			//m_cameraPos = m_cameraPos * lightMat;

			//////针对点光源阴影成像计算问题，需要取得计算点光源阴影方向。
			////if(light->getLightType() == crLight::LIGHT_SPOT)
			//lightMat.setTrans(0.0f,0.0f,0.0f);
			//m_cameraDir = light->getDirection() * lightMat;
			//m_cameraDir.normalize();


			//以下可根据光强与衰减计算。
			//m_lightRange
			//m_cameraFov
	}

	if(m_mode<CameraMVP)
		computeMVP();
	else if(m_mode & CameraMVP)
	{
		setProjectionMatrix(crRenderToTextureManager::getInstance()->getCurrentProjectMatrix());
		setViewMatrix(crRenderToTextureManager::getInstance()->getCurrentViewMatrix());
	}
}

bool crRenderToTexture::culled(/*crPolytope& view_frustum*/)
{
	return !m_enable;
	//判断rtt视锥是否与视锥相交
	
	std::vector<crVector3> frustumVecList;
	crVector3 A,B,C,D,R,R1;
    float d;
    bool culled;

	float fov = DegreesToRadians(m_cameraFov * 0.5);
	d = tan(fov) * m_lightRange;
	R = (m_cameraDir^Y_AXIS) * d;
	R1 = Y_AXIS * d;

	A = m_cameraDir * m_lightRange + R1 + R;
	B = m_cameraDir * m_lightRange - R1 + R;
	C = m_cameraDir * m_lightRange + R1 - R;
	D = m_cameraDir * m_lightRange - R1 - R;

	frustumVecList.push_back(m_cameraPos);
	frustumVecList.push_back(A);
	frustumVecList.push_back(B);
	frustumVecList.push_back(C);
	frustumVecList.push_back(D);

	m_currentViewFrustum.pushCurrentMask();
	culled = !m_currentViewFrustum.contains(frustumVecList);
	m_currentViewFrustum.popCurrentMask();

	if(m_lightSource)
        m_lightSource->setLightShadowCulled(culled);

	return culled;
}

crImage *crRenderToTexture::getImage()
{
 //   if(!m_image.valid())
	//{
	//	if(m_mode & RenderToImage)
	//	{
	//		m_image = new crImage;
	//		m_image->setFileName(m_outPutImageName);
	//		m_image->allocateImage(m_mapSize[0], m_mapSize[1], 1, GL_RGB, GL_UNSIGNED_BYTE);
	//	}
	//}
	return m_image.get();
}

crTexture2D *crRenderToTexture::getTexture()
{
	return m_texture.get();
}
crTexture2D *crRenderToTexture::getBlurTexture()
{
	return m_texture_hblur.get();
}
//void crRenderToTexture::updateUniform(/*crViewPort *sceneViewPort*/)
//{
//	if(!getNeedUpd()) return;
//	//CRCore::notify(CRCore::FATAL)<<"crRenderToTexture::updateUniform() "<<std::endl;
//
//	crStateSet *stateset;
//	crUniform *uniform;
//	if(m_renderGroup.valid())
//	{
//        stateset = m_renderGroup->getStateSet();
//		if(stateset&&m_camera.valid())
//		{
//			uniform = stateset->getUniform("lightViewInverse");
//			if(uniform)
//			{
//				//CRCore::notify(CRCore::FATAL)<<" crRenderToTexture::updateUniform: getViewInverseMatrix:"<<*m_camera->getBufViewInverseMatrix()<<std::endl;
//				uniform->set(*m_camera->getBufViewInverseMatrix());
//				//CRCore::notify(CRCore::FATAL)<<" crRenderToTexture::updateUniform: getViewInverseMatrix end"<<std::endl;
//
//			}
//		}
//	}
//
//	if(m_texturedGroup.valid())
//	{
//		for( NodeArray::iterator itr = m_texturedGroup->getChildArray().begin();
//			itr != m_texturedGroup->getChildArray().end();
//			++itr )
//		{
//			stateset = (*itr)->getStateSet();
//			if(stateset&&m_camera.valid())
//			{
//				uniform = stateset->getUniform("lightMapMVP");
//				if(uniform)
//					uniform->set(*m_camera->getBufViewMatrix() * *m_camera->getBufProjectionMatrix());
//			}
//		}
//	}
//
//	//CRCore::notify(CRCore::FATAL)<<"crRenderToTexture::updateUniform() End"<<std::endl;
//
//}
//
//void crRenderToTexture::swapBuffers()
//{
//	//if(getNeedUpd())
//	//{
//		if(m_camera.valid())
//			m_camera->swapBuffers();
//		if(m_hblurCamera.valid())
//			m_hblurCamera->swapBuffers();
//		if(m_vblurCamera.valid())
//			m_vblurCamera->swapBuffers();
//		crStateSet *stateset;
//		if(m_renderGroup.valid())
//		{
//			stateset = m_renderGroup->getOrCreateStateSet();
//			stateset->swapBuffers();
//		}
//		if(m_texturedGroup.valid())
//		{
//			for( NodeArray::iterator itr = m_texturedGroup->getChildArray().begin();
//				itr != m_texturedGroup->getChildArray().end();
//				++itr )
//			{
//				stateset = (*itr)->getStateSet();
//                stateset->swapBuffers();
//			}
//		}
//	//}
//}

void crRenderToTexture::releaseObjects(crState* state)
{
	if(m_renderGroup.valid())
	{
	    m_renderGroup->removeChild(0,m_renderGroup->getNumChildren());
	}
	if(m_camera.valid())
		m_camera->releaseObjects(state);
	//if(m_hblurCamera.valid())
	//	m_hblurCamera->releaseObjects(state);
	//if(m_vblurCamera.valid())
	//	m_vblurCamera->releaseObjects(state);
	if(m_texturedGroup.valid())
		m_texturedGroup->releaseObjects(state);
	if(m_texture.valid())
		m_texture->releaseObjects(state);
	if(m_texture_vblur.valid())
		m_texture_vblur->releaseObjects(state);
	if(m_texture_hblur.valid())
		m_texture_hblur->releaseObjects(state);
	//if(m_lightSource)
	//	m_lightSource->releaseObjects(state);
}

void crRenderToTexture::traverse(crNodeVisitor& nv)
{
	crNodeVisitor::VisitorType vt = nv.getVisitorType();
	if (vt==CRCore::crNodeVisitor::CULL_VISITOR)
	{
		//CRCore::notify(CRCore::FATAL)<<"crRenderToTexture "<<m_lightSource->getName()<<std::endl;
		std::string str = nv.getTraverseString();
		nv.setTraverseString("RenderToTexture");
		if(m_camera.valid())
		{
		    m_camera->setProjectionMatrix(m_projection);
		    m_camera->setViewMatrix(m_view);
		    m_camera->accept(nv);
			crStateSet *stateset;
			crUniform *uniform;
			if(m_renderGroup.valid())
			{
				stateset = m_renderGroup->getStateSet();
				if(stateset)
				{
					uniform = stateset->getUniform(m_mode & RenderDepthMap?"lightViewInverse":"CRE_InverseViewMatrix");
					if(uniform)	uniform->set(*m_camera->getViewInverseMatrix());
					//            crMatrixd *viewInverseMatrix = m_camera->getViewInverseMatrix();
					//crVector3 pos = crVector3((*viewInverseMatrix)(3,0),(*viewInverseMatrix)(3,1),(*viewInverseMatrix)(3,2));
					//CRCore::notify(CRCore::ALWAYS)<<"crRenderToTexture::traverse:m_cameraPos = "<<pos<<std::endl;
				}
			}
			if(m_mode & RenderDepthMap)
			{
				if(m_texturedGroup.valid())
				{
					crMatrix lightMapMVP = *m_camera->getViewMatrix() * *m_camera->getProjectionMatrix();
					for( NodeArray::iterator itr = m_texturedGroup->getChildArray().begin();
						itr != m_texturedGroup->getChildArray().end();
						++itr )
					{
						stateset = (*itr)->getStateSet();
						if(stateset)
						{
							uniform = stateset->getUniform("lightMapMVP");
							if(uniform)	uniform->set(lightMapMVP);
						}
					}
				}
			}
		}
		//if(m_mode & NeedBlur)
		//{
		//	m_vblurCamera->accept(nv);
		//	m_hblurCamera->accept(nv);
		//}
		nv.setTraverseString(str);
	}
	//else if(vt==CRCore::crNodeVisitor::INIT_VISITOR)
	//{
		////CRCore::notify(CRCore::FATAL)<<"crRenderToTexture INIT_VISITOR "<<m_lightSource->getName()<<std::endl;

		//if(m_texturedGroup.valid())
		//	m_texturedGroup->accept(nv);
		//if(m_camera.valid())
		//	m_camera->accept(nv);
		////if(m_hblurCamera.valid())
		////	m_hblurCamera->accept(nv);
		////if(m_vblurCamera.valid())
		////	m_vblurCamera->accept(nv);
	//}
	else if(vt==CRCore::crNodeVisitor::SWAPBUFFER_VISITOR)
	{//
		//CRCore::notify(CRCore::FATAL)<<"crRenderToTexture SWAPBUFFER_VISITOR "<<m_lightSource->getName()<<std::endl;
		int frameNumber = crFrameStamp::getInstance()->getFrameNumber();
		if(m_swapFrameNumber != frameNumber)
		{
			m_swapFrameNumber = frameNumber;
			//if(m_lastUpdFrameNumber == frameNumber)
			//{
				//if(m_texturedGroup.valid())
				//	m_texturedGroup->accept(nv);
				//if(m_camera.valid())
				//	m_camera->accept(nv);

					//crStateSet *stateset;
					if(m_texturedGroup.valid())
					{
						m_texturedGroup->swapBuffers(frameNumber);
					}
					//if(m_renderGroup.valid())
					//{
					//	m_renderGroup->swapBuffers(frameNumber);
					//}
					if(m_camera.valid())
					{
						//m_camera->swapBuffers(frameNumber);
						m_camera->accept(nv);
					}
					//if(m_hblurCamera.valid())
					//	m_hblurCamera->accept(nv);
					//if(m_vblurCamera.valid())
					//	m_vblurCamera->accept(nv);
			//}
		}
	}
}

/////////////////////////////////////
//
//crRenderToCubeTexture
//
////////////////////////////////////
crImage *crRenderToCubeTexture::getImage(int i)
{
	return m_image[i].get();
}

crTextureCubeMap *crRenderToCubeTexture::getCubeTexture()
{
	return m_cubeTexture.get();
}
//
void crRenderToCubeTexture::createCamera()
{
	crStateSet *ss;
	for(int i = 0; i<6; i++)
	{
		m_camera[i] = new crCameraNode;
		m_camera[i]->setEnableIntersect(false);
		m_camera[i]->setName("RTCT");
        m_camera[i]->setRenderMode(crCameraNode::NormalRender);
		m_camera[i]->setClearColor(crVector4(1.0f, 1.0f, 1.0f, 1.0f));
		m_camera[i]->setComputeNearFarMode(crCameraNode::DO_NOT_COMPUTE_NEAR_FAR);
	    m_camera[i]->setRenderOrder(crCameraNode::PRE_RENDER);
		if(m_mode & crRenderToTexture::RenderToFrame)
			m_camera[i]->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER);
		else
			m_camera[i]->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER_OBJECT);

		m_camera[i]->setDrawBuffer(GL_BACK);
		m_camera[i]->setReadBuffer(GL_BACK);
		m_camera[i]->setReferenceFrame(crCameraNode::RELATIVE_TO_ABSOLUTE);
		m_camera[i]->setCullingMode( crCameraNode::VIEW_FRUSTUM_CULLING |
			crCameraNode::SMALL_FEATURE_CULLING |
			crCameraNode::CLUSTER_CULLING );

		m_camera[i]->setProjectionMatrix(crMatrix::identity());
		m_camera[i]->setBufProjectionMatrix(crMatrix::identity());
		m_camera[i]->setViewMatrix(crMatrix::identity());
		m_camera[i]->setBufViewMatrix(crMatrix::identity());

		m_camera[i]->setViewport(0,0,m_mapSize[0],m_mapSize[1]);

		if(m_mode & crRenderToTexture::RenderToImage)
		{
			m_camera[i]->attach(crCameraNode::COLOR_BUFFER, m_image[i].get());
			m_camera[i]->setPostDrawCallback(m_writeImageCallback.get());
		}
		if(m_mode & crRenderToTexture::RenderToTexture)
		{
			m_camera[i]->attach(crCameraNode::COLOR_BUFFER, m_image[i].get());
		}

		ss = m_camera[i]->getOrCreateStateSet();
		ss->getOrCreateUniform("CRE_InverseViewMatrix",CRCore::crUniform::FLOAT_MAT4);
		m_camera[i]->addChild(m_renderGroup.get());
	}
}

void crRenderToCubeTexture::init(float bias)
{
	const std::string _fn[] = 
	{
	    "_posx","_negx",
	    "_posz","_negz",
	    "_posy","_negy"
	};
	std::string ext = crArgumentParser::getFileExtension(m_outPutImageName);
	std::string fn = crArgumentParser::getFileNameEliminateExt(m_outPutImageName);
	for(int i = 0; i<6; i++)
	{
		if(!m_image[i].valid())
		{
			m_image[i] = new crImage;
			m_image[i]->setFileName(fn + _fn[i] + "." + ext);
			m_image[i]->allocateImage(m_mapSize[0], m_mapSize[1], 1, GL_RGB, GL_UNSIGNED_BYTE);
		}
	}
	if(m_mode & RenderToTexture)
	{
		if(!m_texture.valid())
		{
			m_cubeTexture = new crTextureCubeMap;
			m_cubeTexture->setTextureSize(m_mapSize[0], m_mapSize[1]);
			for(int i = 0; i<6; i++)
			{
                m_cubeTexture->setImage(i,m_image[i].get());
			}
			
			m_cubeTexture->setInternalFormat(GL_RGB8);//GL_RGB16F_ARB
			m_cubeTexture->setSourceFormat(GL_RGB);
			m_cubeTexture->setSourceType(GL_UNSIGNED_BYTE);
			m_cubeTexture->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
			m_cubeTexture->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
			//m_texture->setMaxAnisotropy(16.0);
			m_cubeTexture->setWrap(crTexture2D::WRAP_S, crTexture2D::CLAMP_TO_EDGE);
			m_cubeTexture->setWrap(crTexture2D::WRAP_T, crTexture2D::CLAMP_TO_EDGE);
		}
	}

	computeMVP();

	if((m_mode & RenderToTexture) || (m_mode & RenderToImage) || (m_mode & RenderToFrame))
		createCamera();

	if(m_texturedGroup.valid())
	{
		for( NodeArray::iterator itr = m_texturedGroup->getChildArray().begin();
			itr != m_texturedGroup->getChildArray().end();
			++itr )
		{
			if(m_mode & RenderToTexture)
				(*itr)->getOrCreateStateSet()->setTextureAttributeAndModes(TEXTURE_ENVMAP,m_cubeTexture.get(),crStateAttribute::ON);
		}
	}
}

void crRenderToCubeTexture::computeMVP(/*const crVector3 & eyePoint*/)
{
	typedef std::pair<crVector3f, crVector3f> ImageData;
	static const ImageData id[] =
	{
		ImageData( crVector3f( 1,  0,  0), /*crVector3f( 0, 1, 0 )*/crVector3f( 0,-1, 0 ) ), // +X  RT
		ImageData( crVector3f(-1,  0,  0), /*crVector3f( 0, 1, 0 )*/crVector3f( 0,-1, 0 ) ), // -X  LF
		ImageData( crVector3f( 0,  1,  0), /*crVector3f( 0, 0,-1 )*/crVector3f( 0, 0, 1 ) ), // +Y  FR
		ImageData( crVector3f( 0, -1,  0), /*crVector3f( 0, 0, 1 )*/crVector3f( 0, 0,-1 ) ), // -Y  BK
		ImageData( crVector3f( 0,  0,  1), /*crVector3f( 0, 1, 0 )*/crVector3f( 0, -1, 0 ) ), // +Z   UP
		ImageData( crVector3f( 0,  0, -1), /*crVector3f( 0, 1, 0 )*/crVector3f( 0, -1, 0 ) )  // -Z  DN
	};
	
    for(int i = 0; i < 6; i++)
	{
	    m_projection[i].makePerspective(m_cameraFov,1.0,0.01,m_lightRange);//makeFrustum(-1.0,1.0,-1.0,1.0,1.0,m_lightRange);
	    m_view[i].makeLookAt(m_cameraPos,m_cameraPos + id[i].first,id[i].second);
	}
}

//void crRenderToCubeTexture::updateUniform(/*crViewPort *sceneViewPort*/)
//{
//	if(!getNeedUpd()) return;
//
//	crUniform *uniform;
//	for(int i = 0; i<6; i++)
//	{
//		uniform = m_camera[i]->getStateSet()->getUniform("CRE_InverseViewMatrix");
//		uniform->set(*m_camera[i]->getViewInverseMatrix());
//	}
//}

//void crRenderToCubeTexture::swapBuffers()
//{
//	if(getNeedUpd())
//	{
//		for(int i = 0; i<6; i++)
//		{
//			if(m_camera[i].valid())
//			    m_camera[i]->swapBuffers();
//		}
//	}
//}

void crRenderToCubeTexture::releaseObjects(crState* state)
{
	if(m_renderGroup.valid())
	{
		m_renderGroup->removeChild(0,m_renderGroup->getNumChildren());
	}
	for(int i = 0; i<6; i++)
	{
		if(m_camera[i].valid())
			m_camera[i]->releaseObjects(state);
	}
	//if(m_renderGroup.valid())
	//	m_renderGroup->releaseObjects(state);
	if(m_texturedGroup.valid())
		m_texturedGroup->releaseObjects(state);
	else if(m_texture.valid())
		m_texture->releaseObjects(state);
}

void crRenderToCubeTexture::traverse(crNodeVisitor& nv)
{
	crNodeVisitor::VisitorType vt = nv.getVisitorType();
	if (vt==CRCore::crNodeVisitor::CULL_VISITOR)
	{
		crUniform *uniform;
		for(int i = 0; i<6; i++)
		{
			if(m_camera[i].valid())
			{
				m_camera[i]->setProjectionMatrix(m_projection[i]);
				m_camera[i]->setViewMatrix(m_view[i]);
				m_camera[i]->accept(nv);
          
				uniform = m_camera[i]->getStateSet()->getUniform("CRE_InverseViewMatrix");
        		uniform->set(*m_camera[i]->getViewInverseMatrix());
			}
		}
	}
	else if(vt==CRCore::crNodeVisitor::INIT_VISITOR)
	{
		if(m_texturedGroup.valid())
			m_texturedGroup->accept(nv);
		for(int i = 0; i<6; i++)
		{
			if(m_camera[i].valid())
				m_camera[i]->accept(nv);
		}
	}
	else if(vt==CRCore::crNodeVisitor::SWAPBUFFER_VISITOR)
	{//if(currentframe need upd)
		int frameNumber = crFrameStamp::getInstance()->getFrameNumber();
		if(m_swapFrameNumber != frameNumber)
		{
			m_swapFrameNumber = frameNumber;
			//if(m_lastUpdFrameNumber == frameNumber)
			//{
				if(m_texturedGroup.valid())
					m_texturedGroup->swapBuffers(frameNumber);
				for(int i = 0; i<6; i++)
				{
					if(m_camera[i].valid())
						m_camera[i]->accept(nv);
				}
			//}
		}
	}
}
