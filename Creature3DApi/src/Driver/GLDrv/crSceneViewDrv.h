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
#include <CRCore/crCullFace.h>
#include <CRCore/crAlphaFunc.h>
#include <CRCore/crBufferObject.h>
#include <CRCore/crFilterRenderManager.h>
#include <crgl/GL.h>
#include <crgl/GLExtensions.h>
#include <crgl/GLU.h>

#include <CRUtil/crRenderLeaf.h>
#include <CRUtil/crUpdateVisitor.h>
#include <CRCore/crStatistics.h>

#include <CRUtil/crRenderInitVisitor.h>
//#include <CRUtil/crOglRenderStage.h>
#include <CRUtil/crRenderStage.h>
#include <CRUtil/crNoise.h>
//#include <CRCore/crShapeDrawable.h>
#include <CRCore/crBrain.h>
#include <CRCore/crTexture.h>
#include <CRParticle/crParticleSystemUpdater.h>
#include <CRPhysics/crPhysicsInitVisitor.h>

//#define ShowTime
//#ifdef ShowTime
//#include <CRText/crText.h>
//#endif

static const GLubyte patternVertEven[] = {
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};

static const GLubyte patternVertOdd[] = {
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};

static const GLubyte patternHorzEven[] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00};

void crSceneView::setDefaults(unsigned int options)
{
	m_camera->getProjectionMatrix()->makePerspective(50.0f,1.4f,1.0f,10000.0f);
	m_camera->getViewMatrix()->makeIdentity();
	m_camera->getBufProjectionMatrix()->makePerspective(50.0f,1.4f,1.0f,10000.0f);
	m_camera->getBufViewMatrix()->makeIdentity();
	if(crDisplaySettings::instance()->getRunMode() == 0)
		m_camera->setClearColor(CRCore::crVector4(0.27f,0.37f,0.47f,1.0f/*0.2f, 0.2f, 0.4f, 1.0f*/));
	else
		m_camera->setClearColor(CRCore::crVector4(0.0f,0.0f,0.0f,1.0f));

	if (options & USEPHYSICS)
	{
		m_physicsInitVisitor = new CRPhysics::crPhysicsInitVisitor;
		m_physicsUpdateVisitor = new CRPhysics::crPhysicsUpdateVisitor;
	}

	m_globalStateSet = new CRCore::crStateSet;
	m_localStateSet = new CRCore::crStateSet;

	if ((options & HEADLIGHT) || (options & SKY_LIGHT))
	{
		m_lightingMode=(options&HEADLIGHT) ? HEADLIGHT : SKY_LIGHT;
		m_light = new CRCore::crLight;
		m_light->setLightNum(0);
		m_light->setAmbient(crVector4(0.00f,0.0f,0.00f,1.0f));
		m_light->setDiffuse(crVector4(0.8f,0.8f,0.8f,1.0f));
		m_light->setSpecular(crVector4(1.0f,1.0f,1.0f,1.0f));

		//m_globalStateSet->setAttributeAndModes(m_light.get(),CRCore::crStateAttribute::ON);
		m_globalStateSet->setAssociatedModes(m_light.get(),CRCore::crStateAttribute::ON);

		CRCore::crLightModel* lightmodel = new CRCore::crLightModel;
		lightmodel->setAmbientIntensity(CRCore::crVector4(0.1f,0.1f,0.1f,1.0f));
		m_globalStateSet->setAttributeAndModes(lightmodel, CRCore::crStateAttribute::ON);

		// enable lighting by default.
		m_globalStateSet->setMode(GL_LIGHTING, CRCore::crStateAttribute::ON);
	}
	else
	{
		m_lightingMode = NO_SCENEVIEW_LIGHT;
	}    
	
	m_state = new crState;
    m_rendergraph = new crRenderGraph;
    //m_renderStage = new crOglRenderStage;
	m_renderStage = new crRenderStage;

	/*if(m_useRenderDoubleBuf)
	{
		m_cullVisitor_buf = new crCullVisitor;
		m_rendergraph_buf = new crRenderGraph;
	    m_renderStage_buf = new crOglRenderStage;
		m_state_buf = new crState;

		if(!m_projectionMatrix_buf.valid()) m_projectionMatrix_buf = new CRCore::crMatrixd;
		if(!m_viewMatrix_buf.valid()) m_viewMatrix_buf = new CRCore::crMatrixd;

		m_projectionMatrix_buf->makePerspective(50.0f,1.4f,1.0f,10000.0f);
		m_viewMatrix_buf->makeIdentity();
	}*/

	if (options & COMPILE_GLOBJECTS_AT_INIT)
	{
		crRenderInitVisitor::Mode  dlvMode = crRenderInitVisitor::COMPILE_DISPLAY_LISTS |
			                                 crRenderInitVisitor::COMPILE_STATE_ATTRIBUTES |
											 crRenderInitVisitor::CHECK_BLACK_LISTED_MODES;

#ifdef __sgi
		dlvMode = crRenderInitVisitor::COMPILE_STATE_ATTRIBUTES;
#endif

		// sgi's IR graphics has a problem with lighting and display lists, as it seems to store 
		// lighting state with the display list, and the display list visitor doesn't currently apply
		// state before creating display lists. So will disable the init visitor default, this won't
		// affect functionality since the display lists will be created as and when needed.
		crRenderInitVisitor* dlv = new crRenderInitVisitor(dlvMode);
		dlv->setNodeMaskOverride(0xffffffff);
		m_initVisitor = dlv;
	}

    m_updateVisitor = new crUpdateVisitor;
    //m_updateUniformVisitor = new crUpdateUniformVisitor;
    m_cullVisitor = new crCullVisitor;

	//if(m_useRenderDoubleBuf)
	//{
	//	m_cullVisitor->setRenderGraph(m_rendergraph_buf.get());
	//	m_cullVisitor->setRenderStage(m_renderStage_buf.get());
	//}
	//else
	{
		m_cullVisitor->setRenderGraph(m_rendergraph.get());
		m_cullVisitor->setRenderStage(m_renderStage.get());
	}

	m_renderStage->setClearMask(m_camera->getClearMask());
	if( m_useRenderDoubleBuf )
		doubleThreadRenderInit();

	////////////////////////////globalStateSe
    m_globalStateSet->setGlobalDefaults();
    //////////////////////////////////////
	m_globalStateSet->setMode(GL_LIGHTING, CRCore::crStateAttribute::OFF);
	//
	if(m_displaySettings->useHardWareAA())
	{
		m_globalStateSet->setMode(GL_MULTISAMPLE_ARB, CRCore::crStateAttribute::ON);
		crMultisample *multisample = new crMultisample;
		multisample->setHint(crMultisample::NICEST);
		multisample->setSampleCoverage(1.0,true);
		m_globalStateSet->setAttribute(multisample);
	}
	else
		m_globalStateSet->setMode(GL_MULTISAMPLE_ARB, CRCore::crStateAttribute::OFF);

	m_globalStateSet->setMode(GL_DEPTH_TEST, CRCore::crStateAttribute::ON);
    m_globalStateSet->setMode(GL_STENCIL_TEST, CRCore::crStateAttribute::OFF);//默认关闭蒙版测试
	m_globalStateSet->setMode(GL_NORMALIZE, CRCore::crStateAttribute::OFF);
	m_globalStateSet->setMode(GL_ALPHA_TEST, CRCore::crStateAttribute::OFF);
	m_globalStateSet->setMode(GL_BLEND, crStateAttribute::OFF);
	m_globalStateSet->setMode(GL_POLYGON_OFFSET_FILL, crStateAttribute::OFF);

	m_globalStateSet->setMode(GL_TEXTURE_GEN_S, crStateAttribute::OFF);
	m_globalStateSet->setMode(GL_TEXTURE_GEN_T, crStateAttribute::OFF);
	m_globalStateSet->setMode(GL_TEXTURE_GEN_R, crStateAttribute::OFF);
	m_globalStateSet->setMode(GL_TEXTURE_GEN_Q, crStateAttribute::OFF);

	for(int i = 0; i<6; i++)
	{
		m_globalStateSet->setMode((unsigned int)(GL_CLIP_PLANE0+i),CRCore::crStateAttribute::OFF);
	}

	for(int i = 0; i<8; i++)
	{
		m_globalStateSet->setMode((unsigned int)(GL_LIGHT0+i),CRCore::crStateAttribute::OFF);
	}

    ///////////////////////////
	crDepth* depth = new CRCore::crDepth;
	//depth->setFunction(crDepth::LESS);
	//depth->setWriteMask(false);
	m_globalStateSet->setAttribute(depth,crStateAttribute::ON);
    ///////////////////////////
	crCullFace* cullFace = new CRCore::crCullFace;
	m_globalStateSet->setAttributeAndModes(cullFace,crStateAttribute::ON);
    /////////////////////////////
	//crPolygonMode* polygonMode = new CRCore::crPolygonMode;
	//m_globalStateSet->setAttributeAndModes(polygonMode,crStateAttribute::ON);

	//crBlendFunc* blendfunc = new CRCore::crBlendFunc(crBlendFunc::SRC_ALPHA,crBlendFunc::ONE_MINUS_SRC_ALPHA);
	//m_globalStateSet->setAttribute(blendfunc);

	//注：设置默认crTexMat，会引起贴图渲染不正确。
	crTexMat *tm = new crTexMat;
	m_globalStateSet->setTextureAttribute(0, tm);
	m_globalStateSet->setTextureAttribute(1, tm);

	//crTexEnv* texenv = new crTexEnv;
	//texenv->setMode(crTexEnv::MODULATE);
	//m_globalStateSet->setTextureAttribute(0,texenv);

	//m_globalStateSet->setTextureMode(8,GL_TEXTURE_1D,crStateAttribute::OFF);
	//m_globalStateSet->setTextureMode(8,GL_TEXTURE_2D,crStateAttribute::OFF);
	//m_globalStateSet->setTextureMode(8,GL_TEXTURE_3D,crStateAttribute::OFF);
	//m_globalStateSet->setTextureMode(8,GL_TEXTURE_CUBE_MAP,crStateAttribute::OFF);
	//m_globalStateSet->setTextureMode(8,GL_TEXTURE_RECTANGLE_NV,crStateAttribute::OFF);

	//crAlphaFunc* alphafunc = new crAlphaFunc;
	//alphafunc->setFunction(crAlphaFunc::GREATER,0.0f);
	//m_globalStateSet->setAttribute(alphafunc);

	//crMaterial *material       = new crMaterial;
	////material->setDiffuse(crMaterial::FRONT_AND_BACK,crVector4(0.8,0.8,0.8,1.0));
 ////   material->setSpecular(crMaterial::FRONT_AND_BACK,crVector4(0.5,0.5,0.5,1.0));
	////material->setShininess(crMaterial::FRONT_AND_BACK,32.0);
	//material->setColorMode(crMaterial::OFF);
	//m_globalStateSet->setAttribute(material,crStateAttribute::ON);
	////m_globalStateSet->setMode(GL_COLOR_MATERIAL,CRCore::crStateAttribute::OFF);

	crLightModel *lightModel = new CRCore::crLightModel;
	float brightness = crBrain::getInstance()->getBrightness() + crDisplaySettings::instance()->getBrightness();
	//crVector4 amb = crDisplaySettings::instance()->getAmbientIntensity();
	//crVector3 amb3(minimum(amb[0] + brightness,1.0f), minimum(amb[1] + brightness,1.0f), minimum(amb[2] + brightness,1.0f));
	lightModel->setAmbientIntensity(crBrain::getInstance()->getAmbientIntensity() + crVector4(brightness,brightness,brightness,0.0f));
	//lightModel->setColorControl(CRCore::crLightModel::SEPARATE_SPECULAR_COLOR);
	lightModel->setLocalViewer(true);
	m_globalStateSet->setAttribute(lightModel,crStateAttribute::ON);

	///set default uniform
    crUniform *uniform;
	uniform = m_globalStateSet->getOrCreateUniform("CRE_BaseMap",CRCore::crUniform::SAMPLER_2D);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(TEXTURE_BASEMAP);

	uniform = m_globalStateSet->getOrCreateUniform("CRE_BumpMap",CRCore::crUniform::SAMPLER_2D);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(TEXTURE_BUMPMAP);

	uniform = m_globalStateSet->getOrCreateUniform("CRE_ShadowMap",CRCore::crUniform::SAMPLER_2D);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(TEXTURE_SHADOWMAP);

	uniform = m_globalStateSet->getOrCreateUniform("CRE_ShadowMap2",CRCore::crUniform::SAMPLER_2D);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(TEXTURE_SHADOWMAP2);

	//uniform = m_globalStateSet->getOrCreateUniform("CRE_DetailMap",CRCore::crUniform::SAMPLER_2D);
	//uniform->setDataVariance(crBase::STATIC);
	//uniform->set(TEXTURE_DETAILMAP);

	uniform = m_globalStateSet->getOrCreateUniform("CRE_LightMap",CRCore::crUniform::SAMPLER_2D);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(TEXTURE_LIGHTMAP);

	uniform = m_globalStateSet->getOrCreateUniform("CRE_EnvMap",CRCore::crUniform::SAMPLER_CUBE);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(TEXTURE_ENVMAP);

	uniform = m_globalStateSet->getOrCreateUniform("CRE_GiMap",CRCore::crUniform::SAMPLER_2D);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(TEXTURE_GIMAP);
	
	//uniform = m_globalStateSet->getOrCreateUniform("CRE_UpperSky",CRCore::crUniform::SAMPLER_2D);
	//uniform->setDataVariance(crBase::STATIC);
	//uniform->set(TEXTURE_GIMAP);

	uniform = m_globalStateSet->getOrCreateUniform("CRE_EmissiveMap",CRCore::crUniform::SAMPLER_2D);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(TEXTURE_EMISSIVEMAP);

	uniform = m_globalStateSet->getOrCreateUniform("CRE_SpecularMap",CRCore::crUniform::SAMPLER_2D);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(TEXTURE_SPECULARMAP);

	uniform = m_globalStateSet->getOrCreateUniform("CRE_DiffuseMap",CRCore::crUniform::SAMPLER_2D);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(TEXTURE_DIFFUSEMAP);

	/////2
	uniform = m_globalStateSet->getOrCreateUniform("CRE_NormalMap2",CRCore::crUniform::SAMPLER_2D);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(TEXTURE_NORMALMAP2);

	uniform = m_globalStateSet->getOrCreateUniform("CRE_EmissiveMap2",CRCore::crUniform::SAMPLER_2D);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(TEXTURE_EMISSIVEMAP2);

	uniform = m_globalStateSet->getOrCreateUniform("CRE_SpecularMap2",CRCore::crUniform::SAMPLER_2D);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(TEXTURE_SPECULARMAP2);

	uniform = m_globalStateSet->getOrCreateUniform("CRE_DiffuseMap2",CRCore::crUniform::SAMPLER_2D);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(TEXTURE_DIFFUSEMAP2);

	/////3
	uniform = m_globalStateSet->getOrCreateUniform("CRE_NormalMap3",CRCore::crUniform::SAMPLER_2D);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(TEXTURE_NORMALMAP3);

	uniform = m_globalStateSet->getOrCreateUniform("CRE_EmissiveMap3",CRCore::crUniform::SAMPLER_2D);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(TEXTURE_EMISSIVEMAP3);

	uniform = m_globalStateSet->getOrCreateUniform("CRE_SpecularMap3",CRCore::crUniform::SAMPLER_2D);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(TEXTURE_SPECULARMAP3);

	//uniform = m_globalStateSet->getOrCreateUniform("CRE_DiffuseMap3",CRCore::crUniform::SAMPLER_2D);
	//uniform->setDataVariance(crBase::STATIC);
	//uniform->set(TEXTURE_DIFFUSEMAP3);
    
	//////////ExternTexture
	uniform = m_globalStateSet->getOrCreateUniform("CRE_UVScrambler",CRCore::crUniform::SAMPLER_2D);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(TEXTURE_UVSCRAMBLER);
	
	uniform = m_globalStateSet->getOrCreateUniform("CRE_ColorWeight",CRCore::crUniform::SAMPLER_2D);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(TEXTURE_COLORWEIGHT);
    //////////////////////////////////

	//uniform = m_globalStateSet->getOrCreateUniform("CRE_EnvMap",CRCore::crUniform::SAMPLER_3D);
	//uniform->setDataVariance(crBase::STATIC);
	//uniform->set(TEXTURE_NOISEMAP);

	//uniform = m_globalStateSet->getOrCreateUniform("CRE_ReflectMap",CRCore::crUniform::SAMPLER_2D);
	//uniform->setDataVariance(crBase::STATIC);
	//uniform->set(TEXTURE_REFLECTMAP);

	//uniform = m_globalStateSet->getOrCreateUniform("CRE_RefractMap",CRCore::crUniform::SAMPLER_2D);
	//uniform->setDataVariance(crBase::STATIC);
	//uniform->set(TEXTURE_REFRACTMAP);

	//uniform = m_globalStateSet->getOrCreateUniform("parallax",CRCore::crUniform::FLOAT);
	//uniform->setDataVariance(crBase::STATIC);
	//uniform->set(0.035f);

	if(crDisplaySettings::instance()->getNeedPixelDepth())
	{
		uniform = m_globalStateSet->getOrCreateUniform("maxDepth",CRCore::crUniform::FLOAT);
		uniform->setDataVariance(crBase::STATIC);
		uniform->set(crBrain::getInstance()->getMaxDepth());//maxDepth的平方
	}
	if(crBrain::getInstance()->getFog()==1)
	{
		uniform = m_globalStateSet->getOrCreateUniform("fog_color",CRCore::crUniform::FLOAT_VEC3);
		uniform->setDataVariance(crBase::STATIC);
		uniform->set(crBrain::getInstance()->getFogColor());

		uniform = m_globalStateSet->getOrCreateUniform("fog_params",CRCore::crUniform::FLOAT_VEC4);
		uniform->setDataVariance(crBase::STATIC);
		uniform->set(crBrain::getInstance()->getFogParams());
	}

	//uniform = m_globalStateSet->getOrCreateUniform("param1MultBTa",CRCore::crUniform::FLOAT);
	//uniform->setDataVariance(crBase::STATIC);
	//uniform->set(1.0f);
	//uniform = m_globalStateSet->getOrCreateUniform("param1AddBTa",CRCore::crUniform::FLOAT);
	//uniform->setDataVariance(crBase::STATIC);
	//uniform->set(0.0f);
	uniform = m_globalStateSet->getOrCreateUniform("Distortion",CRCore::crUniform::FLOAT);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(0.0f);
	//uniform = m_globalStateSet->getOrCreateUniform("needRotTBN",CRCore::crUniform::BOOL);
	//uniform->setDataVariance(crBase::STATIC);
	//uniform->set(false);
	uniform = m_globalStateSet->getOrCreateUniform("bias",CRCore::crUniform::FLOAT);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(0.5f);//0.15

	uniform = m_globalStateSet->getOrCreateUniform("UpperSkyColor",CRCore::crUniform::FLOAT_VEC3);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(crBrain::getInstance()->getUpperSkyColor());
	uniform = m_globalStateSet->getOrCreateUniform("LowerSkyColor",CRCore::crUniform::FLOAT_VEC3);
	uniform->setDataVariance(crBase::STATIC);
	uniform->set(crBrain::getInstance()->getLowerSkyColor());

	//uniform = m_globalStateSet->getOrCreateUniform("outline_color",crUniform::FLOAT_VEC3);
	//uniform->setDataVariance(crBase::STATIC);
	//uniform->set(crVector3(0.0f,0.0f,0.0f));

	//uniform = m_globalStateSet->getOrCreateUniform("uvw",CRCore::crUniform::BOOL);
	//uniform->setDataVariance(crBase::STATIC);
	//uniform->set(true);


	// Intel HD4000 under <= 10.8.4 requires GL_DITHER disabled or dithering will occur on any channel < 8bits.
	// No other driver does this but we don't need GL_DITHER on anyway.
	glDisable(GL_DITHER);

	//if (SupportsFramebufferSRGBEnable())
	//{
	//	// Render targets with TexCreate_SRGB should do sRGB conversion like in D3D11
	//	glEnable(GL_FRAMEBUFFER_SRGB);
	//}
}
void crSceneView::draw(int vreye)
{//绘制对buf操作
	CRCore::Timer_t start_tick,end_tick;
	start_tick = CRCore::Timer::instance()->tick();
	if(crStatistics::getInstance()->getStat())
		crStatistics::getInstance()->reset();
	if (!m_displaySettings->getFreezeRender())
	{
#ifdef _DEBUG
		try
		{
#endif
			ref_ptr<crCullVisitor> cullVisitor, cullVisitorLeft, cullVisitorRight;
			ref_ptr<crRenderGraph> rg, rgLeft, rgRight;
			ref_ptr<crRenderStage> rs, rsLeft, rsRight;
			ref_ptr<crState> state = getCurrentRenderState();
			ref_ptr<crMatrixd> projectionMatrix;
			ref_ptr<crMatrixd> viewMatrix;
			crViewPort* viewPort = m_camera->getViewport();//getViewport();
			cullVisitor = m_cullVisitor;
			cullVisitorLeft = m_cullVisitorLeft;
			cullVisitorRight = m_cullVisitorRight;
			if (m_useRenderDoubleBuf)
			{
				//cullVisitor = m_cullVisitor_buf;
				rg = m_rendergraph_buf;
				rs = m_renderStage_buf;
				//state = m_state_buf;
				projectionMatrix = m_camera->getBufProjectionMatrix();
				viewMatrix = m_camera->getBufViewMatrix();

				//cullVisitorLeft = m_cullVisitorLeft_buf;
				//cullVisitorRight = m_cullVisitorRight_buf;
				rgLeft = m_rendergraphLeft_buf;
				rgRight = m_rendergraphRight_buf;
				rsLeft = m_renderStageLeft_buf;
				rsRight = m_renderStageRight_buf;
			}
			else
			{
				rg = m_rendergraph;
				rs = m_renderStage;
				//state = m_state;
				projectionMatrix = getProjectionMatrix();
				viewMatrix = getViewMatrix();

				//cullVisitorLeft = m_cullVisitorLeft;
				//cullVisitorRight = m_cullVisitorRight;
				rgLeft = m_rendergraphLeft;
				rgRight = m_rendergraphRight;
				rsLeft = m_renderStageLeft;
				rsRight = m_renderStageRight;
			}

			//CRCore::notify(CRCore::ALWAYS)<<m_frameStamp->getFrameNumber()<<"crSceneView::draw(): m_position = "<<cullVisitor->getEyePoint()<<std::endl;
#if 0    
			if (m_requiresFlush)
			{
				double availableTime = 0.005;
				flushDeletedObjects(availableTime);
			}

			// assume the the draw which is about to happen could generate GL objects that need flushing in the next frame.
			m_requiresFlush = true;
#endif

			state->setInitialViewMatrix(new CRCore::RefMatrix(*viewMatrix));
			state->colorDirty();
			//updateUniforms();

			crRenderLeaf* previous = NULL;
			if (m_displaySettings->getStereo())
			{
				switch (m_displaySettings->getStereoMode())
				{
				case(CRCore::crDisplaySettings::QUAD_BUFFER):
				{
					m_localStateSet->setAttribute(getViewport());

					// ensure that all color planes are active.
					CRCore::crColorMask* cmask = static_cast<CRCore::crColorMask*>(m_localStateSet->getAttribute(CRCore::crStateAttribute::COLORMASK));
					if (cmask)
					{
						cmask->setMask(true, true, true, true);
					}
					else
					{
						cmask = new CRCore::crColorMask(true, true, true, true);
						m_localStateSet->setAttribute(cmask);
					}
					rsLeft->setColorMask(cmask);
					rsRight->setColorMask(cmask);

					rsLeft->setDrawBuffer(GL_BACK_LEFT);
					rsLeft->setReadBuffer(GL_BACK_LEFT);
					rsRight->setDrawBuffer(GL_BACK_RIGHT);
					rsRight->setReadBuffer(GL_BACK_RIGHT);

					//rsLeft->drawPreRenderStages(*state,previous);
					//rsRight->drawPreRenderStages(*state,previous);

					rsLeft->draw(*state, previous);
					rsRight->draw(*state, previous);
				}
				break;
				case(CRCore::crDisplaySettings::ANAGLYPHIC):
				{
					//setDrawBufferMode();
					if (m_drawBufferMode != GL_NONE)
					{
						m_renderStageLeft->setDrawBuffer(m_drawBufferMode);
						m_renderStageLeft->setReadBuffer(m_drawBufferMode);

						m_renderStageRight->setDrawBuffer(m_drawBufferMode);
						m_renderStageRight->setReadBuffer(m_drawBufferMode);
					}

					m_localStateSet->setAttribute(getViewport());

					// rsLeft->drawPreRenderStages(*state,previous);
					 //rsRight->drawPreRenderStages(*state,previous);

					 // ensure that left eye color planes are active.
					CRCore::crColorMask* leftColorMask = rsLeft->getColorMask();
					if (!leftColorMask)
					{
						leftColorMask = new CRCore::crColorMask();
						rsLeft->setColorMask(leftColorMask);
					}

					// red
					leftColorMask->setMask(true, false, false, true);

					// orange
					// leftColorMask->setMask(true,true,false,true);

					m_localStateSet->setAttribute(leftColorMask);

					// draw left eye.
					rsLeft->draw(*state, previous);

					// ensure that right eye color planes are active.
					CRCore::crColorMask* rightColorMask = rsRight->getColorMask();
					if (!rightColorMask)
					{
						rightColorMask = new CRCore::crColorMask();
						rsRight->setColorMask(rightColorMask);
					}

					// cyan
					rightColorMask->setMask(false, true, true, true);

					// blue
					// rightColorMask->setMask(false,false,true,true);

					m_localStateSet->setAttribute(rightColorMask);
					rsRight->setColorMask(rightColorMask);

					// draw right eye.
					rsRight->draw(*state, previous);

				}
				break;
				case(CRCore::crDisplaySettings::HORIZONTAL_SPLIT):
				{
					//setDrawBufferMode();
					if (m_drawBufferMode != GL_NONE)
					{
						m_renderStageLeft->setDrawBuffer(m_drawBufferMode);
						m_renderStageLeft->setReadBuffer(m_drawBufferMode);

						m_renderStageRight->setDrawBuffer(m_drawBufferMode);
						m_renderStageRight->setReadBuffer(m_drawBufferMode);
					}

					// ensure that all color planes are active.
					CRCore::crColorMask* cmask = static_cast<CRCore::crColorMask*>(m_localStateSet->getAttribute(CRCore::crStateAttribute::COLORMASK));
					if (cmask)
					{
						cmask->setMask(true, true, true, true);
					}
					else
					{
						cmask = new CRCore::crColorMask(true, true, true, true);
						m_localStateSet->setAttribute(cmask);
					}
					rsLeft->setColorMask(cmask);
					rsRight->setColorMask(cmask);

					//rsLeft->drawPreRenderStages(*state,previous);
					//rsRight->drawPreRenderStages(*state,previous);

					int separation = m_displaySettings->getSplitStereoHorizontalSeparation();

					int left_half_width = (viewPort->width() - separation) / 2;
					int right_half_begin = (viewPort->width() + separation) / 2;
					int right_half_width = viewPort->width() - right_half_begin;

					CRCore::ref_ptr<CRCore::crViewPort> viewportLeft = new CRCore::crViewPort;
					viewportLeft->setViewport(viewPort->x(), viewPort->y(), left_half_width, viewPort->height());

					CRCore::ref_ptr<CRCore::crViewPort> viewportRight = new CRCore::crViewPort;
					viewportRight->setViewport(viewPort->x() + right_half_begin, viewPort->y(), right_half_width, viewPort->height());


					clearArea(viewPort->x() + left_half_width, viewPort->y(), separation, viewPort->height(), rsLeft->getClearColor());

					if (m_displaySettings->getSplitStereoHorizontalEyeMapping() == CRCore::crDisplaySettings::LEFT_EYE_LEFT_VIEWPORT)
					{
						m_localStateSet->setAttribute(viewportLeft.get());
						rsLeft->setViewport(viewportLeft.get());
						rsLeft->draw(*state, previous);

						m_localStateSet->setAttribute(viewportRight.get());
						rsRight->setViewport(viewportRight.get());
						rsRight->draw(*state, previous);
					}
					else
					{
						m_localStateSet->setAttribute(viewportRight.get());
						rsLeft->setViewport(viewportRight.get());
						rsLeft->draw(*state, previous);

						m_localStateSet->setAttribute(viewportLeft.get());
						rsRight->setViewport(viewportLeft.get());
						rsRight->draw(*state, previous);
					}
				}
				break;
				case(CRCore::crDisplaySettings::VERTICAL_SPLIT):
				{
					//setDrawBufferMode();
					if (m_drawBufferMode != GL_NONE)
					{
						m_renderStageLeft->setDrawBuffer(m_drawBufferMode);
						m_renderStageLeft->setReadBuffer(m_drawBufferMode);

						m_renderStageRight->setDrawBuffer(m_drawBufferMode);
						m_renderStageRight->setReadBuffer(m_drawBufferMode);
					}

					// ensure that all color planes are active.
					CRCore::crColorMask* cmask = static_cast<CRCore::crColorMask*>(m_localStateSet->getAttribute(CRCore::crStateAttribute::COLORMASK));
					if (cmask)
					{
						cmask->setMask(true, true, true, true);
					}
					else
					{
						cmask = new CRCore::crColorMask(true, true, true, true);
						m_localStateSet->setAttribute(cmask);
					}

					rsLeft->setColorMask(cmask);
					rsRight->setColorMask(cmask);
					//rsLeft->drawPreRenderStages(*state,previous);
					//rsRight->drawPreRenderStages(*state,previous);

					int separation = m_displaySettings->getSplitStereoVerticalSeparation();

					int bottom_half_height = (viewPort->height() - separation) / 2;
					int top_half_begin = (viewPort->height() + separation) / 2;
					int top_half_height = viewPort->height() - top_half_begin;

					CRCore::ref_ptr<CRCore::crViewPort> viewportTop = new CRCore::crViewPort;
					viewportTop->setViewport(viewPort->x(), viewPort->y() + top_half_begin, viewPort->width(), top_half_height);

					CRCore::ref_ptr<CRCore::crViewPort> viewportBottom = new CRCore::crViewPort;
					viewportBottom->setViewport(viewPort->x(), viewPort->y(), viewPort->width(), bottom_half_height);

					clearArea(viewPort->x(), viewPort->y() + bottom_half_height, viewPort->width(), separation, rsLeft->getClearColor());

					if (m_displaySettings->getSplitStereoVerticalEyeMapping() == CRCore::crDisplaySettings::LEFT_EYE_TOP_VIEWPORT)
					{
						m_localStateSet->setAttribute(viewportTop.get());
						rsLeft->setViewport(viewportTop.get());
						rsLeft->draw(*state, previous);

						m_localStateSet->setAttribute(viewportBottom.get());
						rsRight->setViewport(viewportBottom.get());
						rsRight->draw(*state, previous);
					}
					else
					{
						m_localStateSet->setAttribute(viewportBottom.get());
						rsLeft->setViewport(viewportBottom.get());
						rsLeft->draw(*state, previous);

						m_localStateSet->setAttribute(viewportTop.get());
						rsRight->setViewport(viewportTop.get());
						rsRight->draw(*state, previous);
					}
				}
				break;
				case(CRCore::crDisplaySettings::RIGHT_EYE):
				case(CRCore::crDisplaySettings::LEFT_EYE):
				{
					//setDrawBufferMode();
					if (m_drawBufferMode != GL_NONE)
					{
						rs->setDrawBuffer(m_drawBufferMode);
						rs->setReadBuffer(m_drawBufferMode);
					}

					m_localStateSet->setAttribute(getViewport());
					// ensure that all color planes are active.
					CRCore::crColorMask* cmask = static_cast<CRCore::crColorMask*>(m_localStateSet->getAttribute(CRCore::crStateAttribute::COLORMASK));
					if (cmask)
					{
						cmask->setMask(true, true, true, true);
					}
					else
					{
						cmask = new CRCore::crColorMask(true, true, true, true);
						m_localStateSet->setAttribute(cmask);
					}
					rs->setColorMask(cmask);
					rs->setClearMask(m_camera->getClearMask());

					//rs->drawPreRenderStages(*state,previous);
					rs->draw(*state, previous);

				}
				break;
				case(CRCore::crDisplaySettings::VERTICAL_INTERLACE):
				{
					m_localStateSet->setAttribute(viewPort);

					// ensure that all color planes are active.
					CRCore::crColorMask* cmask = static_cast<CRCore::crColorMask*>(m_localStateSet->getAttribute(CRCore::crStateAttribute::COLORMASK));
					if (cmask)
					{
						cmask->setMask(true, true, true, true);
					}
					else
					{
						cmask = new CRCore::crColorMask(true, true, true, true);
						m_localStateSet->setAttribute(cmask);
					}
					rsLeft->setColorMask(cmask);
					rsRight->setColorMask(cmask);

					//rsLeft->drawPreRenderStages(*state,previous);
					//rsRight->drawPreRenderStages(*state,previous);

					glEnable(GL_STENCIL_TEST);

					if (m_redrawInterlacedStereoStencilMask ||
						m_interlacedStereoStencilWidth != viewPort->width() ||
						m_interlacedStereoStencilHeight != viewPort->height())
					{
						getViewport()->apply(*state);
						glMatrixMode(GL_PROJECTION);
						glLoadIdentity();
						glOrtho(viewPort->x(), viewPort->width(), viewPort->y(), viewPort->height(), -1.0, 1.0);
						glMatrixMode(GL_MODELVIEW);
						glLoadIdentity();
						glDisable(GL_LIGHTING);
						glDisable(GL_DEPTH_TEST);
						glStencilMask(~0u);
						glClear(GL_STENCIL_BUFFER_BIT);
						glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
						glStencilFunc(GL_ALWAYS, 1, ~0u);
						glPolygonStipple(patternVertEven);
						glEnable(GL_POLYGON_STIPPLE);
						glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
						//state->applyColorMask(crVector4ub(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE));
						glRecti(viewPort->x(), viewPort->y(), viewPort->width(), viewPort->height());
						glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
						//state->applyColorMask(crVector4ub(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE));
						glDisable(GL_POLYGON_STIPPLE);
						glEnable(GL_LIGHTING);
						glEnable(GL_DEPTH_TEST);

						m_redrawInterlacedStereoStencilMask = false;
						m_interlacedStereoStencilWidth = viewPort->width();
						m_interlacedStereoStencilHeight = viewPort->height();
					}

					rsLeft->setClearMask(rsLeft->getClearMask() & ~(GL_STENCIL_BUFFER_BIT));
					rsRight->setClearMask(rsRight->getClearMask() & ~(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

					glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
					glStencilFunc(GL_EQUAL, 0, ~0u);
					rsLeft->draw(*state, previous);

					glStencilFunc(GL_NOTEQUAL, 0, ~0u);
					rsRight->draw(*state, previous);
					glDisable(GL_STENCIL_TEST);
				}
				break;
				case(CRCore::crDisplaySettings::HORIZONTAL_INTERLACE):
				{
					m_localStateSet->setAttribute(viewPort);

					// ensure that all color planes are active.
					CRCore::crColorMask* cmask = static_cast<CRCore::crColorMask*>(m_localStateSet->getAttribute(CRCore::crStateAttribute::COLORMASK));
					if (cmask)
					{
						cmask->setMask(true, true, true, true);
					}
					else
					{
						cmask = new CRCore::crColorMask(true, true, true, true);
						m_localStateSet->setAttribute(cmask);
					}
					rsLeft->setColorMask(cmask);
					rsRight->setColorMask(cmask);

					//rsLeft->drawPreRenderStages(*state,previous);
					//rsRight->drawPreRenderStages(*state,previous);

					glEnable(GL_STENCIL_TEST);

					if (m_redrawInterlacedStereoStencilMask ||
						m_interlacedStereoStencilWidth != viewPort->width() ||
						m_interlacedStereoStencilHeight != viewPort->height())
					{
						getViewport()->apply(*state);
						glMatrixMode(GL_PROJECTION);
						glLoadIdentity();
						glOrtho(viewPort->x(), viewPort->width(), viewPort->y(), viewPort->height(), -1.0, 1.0);
						glMatrixMode(GL_MODELVIEW);
						glLoadIdentity();
						glDisable(GL_LIGHTING);
						glDisable(GL_DEPTH_TEST);
						glStencilMask(~0u);
						glClear(GL_STENCIL_BUFFER_BIT);
						glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
						glStencilFunc(GL_ALWAYS, 1, ~0u);
						glPolygonStipple(patternHorzEven);
						glEnable(GL_POLYGON_STIPPLE);
						glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
						//state->applyColorMask(crVector4ub(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE));
						glRecti(viewPort->x(), viewPort->y(), viewPort->width(), viewPort->height());
						glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
						//state->applyColorMask(crVector4ub(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE));
						glDisable(GL_POLYGON_STIPPLE);
						glEnable(GL_LIGHTING);
						glEnable(GL_DEPTH_TEST);

						m_redrawInterlacedStereoStencilMask = false;
						m_interlacedStereoStencilWidth = viewPort->width();
						m_interlacedStereoStencilHeight = viewPort->height();
					}

					rsLeft->setClearMask(rsLeft->getClearMask() & ~(GL_STENCIL_BUFFER_BIT));
					rsRight->setClearMask(rsRight->getClearMask() & ~(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

					glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
					glStencilFunc(GL_EQUAL, 0, ~0u);
					rsLeft->draw(*state, previous);

					glStencilFunc(GL_NOTEQUAL, 0, ~0u);
					rsRight->draw(*state, previous);
					glDisable(GL_STENCIL_TEST);
				}
				break;
				default:
				{
					CRCore::notify(CRCore::NOTICE) << "Warning: stereo mode not implemented yet." << std::endl;
				}
				break;
				}
			}
			else
			{
				// Need to restore draw buffer when toggling Stereo off.
				//setDrawBufferMode();
				if (m_drawBufferMode != GL_NONE)
				{
					rs->setDrawBuffer(m_drawBufferMode);
					rs->setReadBuffer(m_drawBufferMode);
				}

				//switch (vreye)
				//{
				//case 1:
				//	{
				//		int separation = m_displaySettings->getSplitStereoHorizontalSeparation();

				//		int left_half_width = (viewPort->width() - separation) / 2;
				//		int right_half_begin = (viewPort->width() + separation) / 2;
				//		int right_half_width = viewPort->width() - right_half_begin;

				//		CRCore::ref_ptr<CRCore::crViewPort> viewportLeft = new CRCore::crViewPort;
				//		viewportLeft->setViewport(viewPort->x(), viewPort->y(), left_half_width, viewPort->height());

				//		m_localStateSet->setAttribute(viewportLeft.get());
				//		rs->setViewport(viewportLeft.get());
				//	}
				//	break;
				//case 2:
				//	{
				//		int separation = m_displaySettings->getSplitStereoHorizontalSeparation();

				//		int left_half_width = (viewPort->width() - separation) / 2;
				//		int right_half_begin = (viewPort->width() + separation) / 2;
				//		int right_half_width = viewPort->width() - right_half_begin;

				//		CRCore::ref_ptr<CRCore::crViewPort> viewportRight = new CRCore::crViewPort;
				//		viewportRight->setViewport(viewPort->x() + right_half_begin, viewPort->y(), right_half_width, viewPort->height());

				//		m_localStateSet->setAttribute(viewportRight.get());
				//		rs->setViewport(viewportRight.get());
				//	}
				//	break;
				//default:
				//	m_localStateSet->setAttribute(viewPort);
				//	break;
				//}
				m_localStateSet->setAttribute(viewPort);
				//  ensure that all color planes are active.
				CRCore::crColorMask* cmask = static_cast<CRCore::crColorMask*>(m_localStateSet->getAttribute(CRCore::crStateAttribute::COLORMASK));
				if (cmask)
				{
					cmask->setMask(true, true, true, true);
				}
				else
				{
					cmask = new CRCore::crColorMask(true, true, true, true);
					m_localStateSet->setAttribute(cmask);
				}

				rs->setColorMask(cmask);
				rs->setClearMask(m_camera->getClearMask());
				//rs->drawPreRenderStages(*state,previous);
				//try
				//{
				//CRCore::notify(CRCore::FATAL)<<"rs->draw begin "<<rs->getBinNum()<<std::endl;
				//if(getFrameStamp()->getFrameNumber()>5)
				rs->draw(*state, previous);
				//else
				//{
				//	state->applyAttribute(viewPort);
				//	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
				//	CRCore::crThread::sleep(50);
				//}
				//CRCore::notify(CRCore::FATAL)<<"rs->draw end "<<rs->getBinNum()<<std::endl;
				//}
				//catch (...)
				//{
				//	CRCore::notify(WARN)<<"draw error"<< std::endl;
				//}
				//获得状态参数
				//m_renderStage->getStats(m_statistics.get());
			}

			// re apply the defalt OGL state.	
			state->popAllStateSets();
			state->apply();
			//state->reset();

			if (m_camera->getPostDrawCallback())
			{
				(*(m_camera->getPostDrawCallback()))(*m_camera);
			}
#ifdef OUTPUTGLERROR
			GLenum errorNo = glGetError();
			if (errorNo != GL_NO_ERROR)
			{
				char gbuf[256];
				sprintf(gbuf, "crSceneView draw OpenGL error %s\n\0", gluErrorString(errorNo));
				gDebugInfo->debugInfo(CRCore::NOTICE, gbuf);
			}
#endif
#ifdef _DEBUG
	}
	catch (...)
	{
		CRCore::notify(WARN) << "draw error" << std::endl;
	}
#endif
	}
	if(m_useRenderDoubleBuf)
	{	
		unlockSwapBuffer();
		//CRCore::crThread::sleep(1);//让显卡休息
	}
	end_tick = CRCore::Timer::instance()->tick();
	float drawtime = CRCore::Timer::instance()->delta_m(start_tick,end_tick);
	crStatistics::getInstance()->setDrawTime(drawtime);
	if (vreye == 0)
	{
		float fpsControl = crDisplaySettings::instance()->getFpsControl();
		if (drawtime < fpsControl)
			CRCore::crThread::sleep(fpsControl - drawtime);//让显卡休息
	}
	//if(m_lastDrawTick != 0L)
	//{
	//	float drawtime = CRCore::Timer::instance()->delta_m(m_lastDrawTick,end_tick);
	//	float fpsControl = crDisplaySettings::instance()->getFpsControl();
	//	if(drawtime<fpsControl)
	//		CRCore::crThread::sleep(fpsControl - drawtime);//让显卡休息
	//}
	//m_lastDrawTick = end_tick;
}

void crSceneView::clearArea(int x,int y,int width,int height,const CRCore::crVector4& color)
{
    CRCore::ref_ptr<CRCore::crViewPort> viewport = new CRCore::crViewPort;
    viewport->setViewport(x,y,width,height);

	crState *state = getCurrentRenderState();
    state->applyAttribute(viewport.get());
    
    glScissor( x, y, width, height );
    glEnable( GL_SCISSOR_TEST );
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	//state->applyColorMask(crVector4ub(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE));
    glClearColor( color[0], color[1], color[2], color[3]);
    glClear( GL_COLOR_BUFFER_BIT);
    glDisable( GL_SCISSOR_TEST );
}