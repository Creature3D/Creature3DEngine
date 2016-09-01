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
#include <CRCore/crProgramNode.h>
#include <CRCore/crLight.h>
#include <CRCore/crProgram.h>
#include <CRCore/crShader.h>
#include <CRCore/crUniform.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crPolygonOffset.h>
#include <CRCore/crClipPlane.h>
//#include <CRCore/crFrameBufferObject.h>
#include <CRCore/crBlendEquation.h>
#include <CRCore/crTextureCubeMap.h>
#include <CRCore/crNotify.h>

#include <CRCore/crObject.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crBrain.h>
#include <CRCore/crMath.h>
#include <CRCore/crVector2.h>
#include <CRCore/crBlendFunc.h>
#include <CRCore/crDepth.h>
#include <CRCore/crClipNode.h>

using namespace CRCore;

const float c_sunViewRangeForBig = 88.0f;//用于渲染大物体的shadowcamera
const float c_sunViewRangeForSmall = 22.0f;//用于渲染小物体的shadowcamera

const crVector2i c_sunShadowMapSize(2048,2048);
const crVector2i c_shadowMapSize(1024,1024);
//#define LIGHT_RANGE 2000.0f //-0.0017 --- -0.01
#define SUNLIGHT_RANGE 1000.0f//450.0f //-0.0017 --- -0.01
#define SPOTLIGHT_RANGE 500.0f //-0.0017 --- -0.01
#define SUNLIGHTSCALE (1.0f / SUNLIGHT_RANGE)
#define SPOTLIGHTSCALE (1.0f / SPOTLIGHT_RANGE)
//////////////////////////////////////////////////////////////////
crProgramNode::crProgramNode()
{
	m_indoor = true;
	setNumChildrenRequiringUpdateUniformTraversal(1);
}

crProgramNode::crProgramNode(const crProgramNode& shadowNode,const crCopyOp& copyop):
    crGroup(shadowNode,copyop),
	m_indoor(shadowNode.m_indoor),
    m_ls_cameraMap(shadowNode.m_ls_cameraMap)
{
}

void renderShadowMap(crProgramNode *node,crTexture2D *shadowMap)
{
	crObject* object = new crObject;
	crGroup *group = new crGroup;
	crGeometry* geom = createTexturedQuadGeometry(crVector3(0,0,0),crVector3(100.0,0.0,0.0),crVector3(0.0,100.0,0.0));
	geom->getOrCreateStateSet()->setTextureAttributeAndModes(0,shadowMap,crStateAttribute::ON);
	geom->getOrCreateStateSet()->setMode(GL_LIGHTING,crStateAttribute::OFF);
	object->addDrawable(geom);

	crCameraNode* camera = new crCameraNode;

	// set the projection matrix
	camera->setProjectionMatrix(crMatrix::ortho2D(0,100,0,100));
    camera->setBufProjectionMatrix(crMatrix::ortho2D(0,100,0,100));
	// set the view matrix    
	camera->setReferenceFrame(crTransform::RELATIVE_TO_ABSOLUTE);
	camera->setViewMatrix(crMatrix::identity());
	camera->setBufViewMatrix(crMatrix::identity());

	camera->setViewport(50,50,100,100);

	// only clear the depth buffer
	camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw subgraph after main camera view.
	camera->setRenderOrder(crCameraNode::POST_RENDER);

	camera->addChild(object);

	group->addChild(camera);
	group->setEnableIntersect(false);
    node->addChild(group);
}

void crProgramNode::insertLightSource( CRCore::crLightSource *ls )
{
	if(m_ls_cameraMap.find(ls)!=m_ls_cameraMap.end()) return;
	std::string name;
	//if(!getStateSet()||!(getOrCreateStateSet()->getAttribute(crStateAttribute::PROGRAM))) createShader();

	ref_ptr<crCameraNode> shadow_camera = new crCameraNode;
	crStateSet *shadow_stateset = shadow_camera->getOrCreateStateSet();
    CRCore::crClipPlane *clipplane;
	crClipNode *clipNode;

	ref_ptr<crTexture2D> shadowMap = new crTexture2D;
	shadowMap->setInternalFormat(GL_RGB);
	shadowMap->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
	shadowMap->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
	//shadowMap->setMaxAnisotropy(16);
	shadowMap->setWrap(crTexture2D::WRAP_S, crTexture2D::REPEAT);
	shadowMap->setWrap(crTexture2D::WRAP_T, crTexture2D::REPEAT);
	if(ls->getLightIsSunLight())
	{
		setIndoorFlag(false);
		shadowMap->setTextureSize(c_sunShadowMapSize[0], c_sunShadowMapSize[1]);
		shadow_camera->setViewport(0,0,c_sunShadowMapSize[0],c_sunShadowMapSize[1]);
		shadow_camera->setRenderMode(crCameraNode::SunShadowMapRenderForBig);

        shadow_camera->addChild(m_childArray[0]);
		shadow_camera->addChild(m_childArray[1]);
	}
	else
	{
		shadowMap->setTextureSize(c_shadowMapSize[0], c_shadowMapSize[1]);
		shadow_camera->setViewport(0,0,c_shadowMapSize[0],c_shadowMapSize[1]);
		shadow_camera->setRenderMode(crCameraNode::ShadowMapRender);

		shadow_camera->addChild(m_childArray[0]);
		shadow_camera->addChild(m_childArray[1]);
		shadow_camera->addChild(m_childArray[2]);
		shadow_camera->addChild(m_childArray[3]);
	}

	shadow_camera->setClearColor(crVector4(1.0f, 1.0f, 1.0f, 1.0f));
	shadow_camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shadow_camera->setComputeNearFarMode(crCameraNode::DO_NOT_COMPUTE_NEAR_FAR);

	shadow_camera->setRenderOrder(crCameraNode::PRE_RENDER);
	shadow_camera->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER_OBJECT);
	shadow_camera->attach(crCameraNode::COLOR_BUFFER, shadowMap.get());
	shadow_camera->setDrawBuffer(GL_BACK);
	shadow_camera->setReadBuffer(GL_BACK);
	shadow_camera->setReferenceFrame(crCameraNode::RELATIVE_TO_ABSOLUTE);
	shadow_camera->setProjectionMatrix(crMatrix::identity());
	shadow_camera->setBufProjectionMatrix(crMatrix::identity());  
	shadow_camera->setViewMatrix(crMatrix::identity());
	shadow_camera->setBufViewMatrix(crMatrix::identity());

	shadow_camera->setCullingMode( crCameraNode::SMALL_FEATURE_CULLING |
		                           crCameraNode::CLUSTER_CULLING |
		                           crCameraNode::EXTERN_FRUSTUM_CULLING );

	this->addChild(shadow_camera.get());

	shadow_stateset->setMode(GL_LIGHTING, crStateAttribute::OFF);
	//ati fbo 对以下三个不能硬件支持
	shadow_stateset->setMode(GL_BLEND, crStateAttribute::OFF);
	shadow_stateset->setMode(GL_ALPHA_TEST, crStateAttribute::OFF);
	shadow_stateset->setMode(GL_FOG, crStateAttribute::OFF);

///////////////////以下为测试代码
	//clipNode = new crClipNode;
	//clipNode->setCullingActive(false);

	//for (int i = 0; i < 6; i++)
	//{
	//	clipplane = new crClipPlane;
	//	clipplane->setClipPlaneNum(i);
	//	clipNode->addClipPlane(clipplane);
	//}

	//for(int i = 0; i < shadow_camera->getNumChildren(); ++i)
	//{
	//	clipNode->addChild(shadow_camera->getChild(i)); 
	//}

	//shadow_camera->removeChild(0,shadow_camera->getNumChildren());

	//shadow_camera->addChild(clipNode);
/////////////////////////////////////////////
	/////////setup shader
	CRCore::ref_ptr<CRCore::crProgram> program;
	CRCore::ref_ptr<CRCore::crShader> shader;
	CRCore::ref_ptr<CRCore::crUniform> uniform;

	program = new CRCore::crProgram;
	shadow_stateset->setAttributeAndModes(program.get(),CRCore::crStateAttribute::ON);
	name.clear();
	name = "shadowMap_";
	name += ls->getName();
	program->setName(name.c_str());
	shadowMap->setImageNameID(name);

	shader = new CRCore::crShader(CRCore::crShader::VERTEX,shadow_vp);
	name.clear();
	name = "shadow_vp_";
	name += ls->getName();
	shader->setName(name.c_str());
	program->addShader(shader.get());
	shader = new CRCore::crShader(CRCore::crShader::FRAGMENT,shadow_fp);
	name.clear();
	name = "shadow_fp_";
	name += ls->getName();
	shader->setName(name.c_str());
	program->addShader(shader.get());

	uniform = new CRCore::crUniform(crUniform::FLOAT_MAT4,"lightMVP");
	shadow_stateset->addUniform(uniform.get());
	uniform = new CRCore::crUniform(crUniform::FLOAT_VEC4,"lightPos");
	shadow_stateset->addUniform(uniform.get());

//////////////create lighting camera
	if(ls->getLightIsSunLight())
	{
		ref_ptr<crCameraNode> shadow_camera1 = new crCameraNode;
		crStateSet *shadow_stateset1 = shadow_camera1->getOrCreateStateSet();
		shadow_camera->setDependCameraNode(shadow_camera1.get(),false);

		shadow_camera->addChild(m_childArray[2]);

		shadowMap = new crTexture2D;
		shadowMap->setInternalFormat(GL_RGB);
		shadowMap->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
		shadowMap->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
		//shadowMap->setMaxAnisotropy(16);
		shadowMap->setWrap(crTexture2D::WRAP_S, crTexture2D::REPEAT);
		shadowMap->setWrap(crTexture2D::WRAP_T, crTexture2D::REPEAT);
		shadowMap->setTextureSize(c_shadowMapSize[0], c_shadowMapSize[1]);
		shadow_camera1->setViewport(0,0,c_shadowMapSize[0],c_shadowMapSize[1]);

		shadow_camera1->setClearColor(crVector4(1.0f, 1.0f, 1.0f, 1.0f));
		shadow_camera1->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shadow_camera1->setComputeNearFarMode(crCameraNode::DO_NOT_COMPUTE_NEAR_FAR);
		shadow_camera1->setRenderMode(crCameraNode::SunShadowMapRenderForSmall);

		shadow_camera1->setRenderOrder(crCameraNode::PRE_RENDER);
		shadow_camera1->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER_OBJECT);
		shadow_camera1->attach(crCameraNode::COLOR_BUFFER, shadowMap.get());
		shadow_camera1->setDrawBuffer(GL_BACK);
		shadow_camera1->setReadBuffer(GL_BACK);
		shadow_camera1->setReferenceFrame(crCameraNode::RELATIVE_TO_ABSOLUTE);
		shadow_camera->setProjectionMatrix(crMatrix::identity());
		shadow_camera->setBufProjectionMatrix(crMatrix::identity());  
		shadow_camera->setViewMatrix(crMatrix::identity());
		shadow_camera->setBufViewMatrix(crMatrix::identity());

		shadow_camera1->setCullingMode( crCameraNode::VIEW_FRUSTUM_CULLING |
			                            crCameraNode::SMALL_FEATURE_CULLING |
			                            crCameraNode::CLUSTER_CULLING |
			                            crCameraNode::EXTERN_FRUSTUM_CULLING );

		shadow_stateset1->setMode(GL_LIGHTING, crStateAttribute::OFF);
		////ati fbo 对以下三个不能硬件支持
		shadow_stateset1->setMode(GL_BLEND, crStateAttribute::OFF);
		shadow_stateset1->setMode(GL_ALPHA_TEST, crStateAttribute::OFF);
		shadow_stateset1->setMode(GL_FOG, crStateAttribute::OFF);

		///////setup shader
		//program = new CRCore::crProgram;
		shadow_stateset1->setAttributeAndModes(program.get(),CRCore::crStateAttribute::ON);
		shadowMap->setImageNameID("shadowMap1_"+ls->getName());

		uniform = new CRCore::crUniform(crUniform::FLOAT_MAT4,"lightMVP");
		shadow_stateset1->addUniform(uniform.get());
		uniform = new CRCore::crUniform(crUniform::FLOAT_VEC4,"lightPos");
		shadow_stateset1->addUniform(uniform.get());
		uniform = new CRCore::crUniform(crUniform::FLOAT,"scale");
		shadow_stateset1->addUniform(uniform.get());
	}
	else
	{
		ref_ptr<crCameraNode> lighting_camera = new crCameraNode;
		crStateSet *lighting_stateset = lighting_camera->getOrCreateStateSet();

		shadow_camera->setDependCameraNode(lighting_camera.get());
		lighting_camera->setClearMask(GL_NONE);
		lighting_camera->setRenderOrder(crCameraNode::POST_RENDER);
		lighting_camera->setReferenceFrame(crTransform::RELATIVE_TO_ABSOLUTE);
		lighting_camera->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER);
		lighting_camera->setProjectionMatrix(crMatrix::identity());
		lighting_camera->setBufProjectionMatrix(crMatrix::identity());  
		lighting_camera->setViewMatrix(crMatrix::identity());
		lighting_camera->setBufViewMatrix(crMatrix::identity());
		lighting_camera->setDrawBuffer(GL_BACK);
		lighting_camera->setReadBuffer(GL_BACK);
		lighting_camera->setColorMask(true,true,true,true);
		lighting_camera->setRenderMode(crCameraNode::LightingRender);
		lighting_camera->setCullingMode( crCameraNode::VIEW_FRUSTUM_CULLING |
			                             crCameraNode::SMALL_FEATURE_CULLING |
			                             crCameraNode::SHADOW_OCCLUSION_CULLING |
			                             crCameraNode::CLUSTER_CULLING |
			                             crCameraNode::EXTERN_FRUSTUM_CULLING );

		lighting_stateset->setMode(GL_LIGHTING, crStateAttribute::OFF);

		clipNode = new crClipNode;
		clipNode->setCullingActive(false);

		for (int i = 0; i < 6; i++)
		{
			clipplane = new crClipPlane;
			clipplane->setClipPlaneNum(i);
			clipNode->addClipPlane(clipplane);
		}

		for(int i = 0; i < lighting_camera->getNumChildren(); ++i)
		{
			clipNode->addChild(lighting_camera->getChild(i)); 
		}

		lighting_camera->removeChild(0,lighting_camera->getNumChildren());

		lighting_camera->addChild(clipNode);

		lighting_stateset->setMode(crStateAttribute::VRMode(GL_POLYGON_OFFSET_FILL,NULL), crStateAttribute::OFF);
		crBlendFunc *blend = new crBlendFunc(crBlendFunc::ONE,crBlendFunc::ONE);
		lighting_stateset->setAttributeAndModes(blend);
		crDepth *depth = new crDepth;
		depth->setFunction(crDepth::LEQUAL);
		depth->setWriteMask(false);
		lighting_stateset->setAttributeAndModes(depth);

		uniform = new CRCore::crUniform(crUniform::SAMPLER_2D,"LightMap");
		uniform->set(TEXTURE_LIGHTMAP0);
		lighting_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::FLOAT_MAT4,"lightMVP");
		lighting_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::FLOAT_VEC4,"lightPos");
		lighting_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::SAMPLER_2D,"BaseMap");
		uniform->set(TEXTURE_BASEMAP);
		lighting_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::SAMPLER_2D,"BumpMap");
		uniform->set(TEXTURE_BUMPMAP);
		lighting_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::SAMPLER_2D,"ShadowMap");
		uniform->set(TEXTURE_SHADOWMAP0);
		lighting_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::FLOAT_VEC4,"lightColor");
		lighting_stateset->addUniform(uniform.get());

		//uniform = new CRCore::crUniform(crUniform::BOOL_VEC2,"switchOff");
		//lighting_stateset->addUniform(uniform.get());

		//uniform = new CRCore::crUniform(crUniform::FLOAT,"specularParam");//放在每个drawable节点上设置
		//uniform->set(1.0f);
		//lighting_stateset->addUniform(uniform.get());

	}

	//CameraPair camerapair(shadow_camera,lighting_camera);
	m_ls_cameraMap[ls] = shadow_camera;
	
	//if(ls->getName().compare("ls2")==0)
	//    renderShadowMap(this,shadowMap.get()/*dynamic_cast<crTexture2D *>(crBrain::getInstance()->getResource()->findOrLoadTexture(261))*/);
}

void crProgramNode::createShader(const crVector4& fogColor)
{
	crStateSet *this_stateset = this->getOrCreateStateSet();
	
	//crTextureCubeMap *envmap = crBrain::getInstance()->getCurrentActiveDB()->getDBEnvironmentMap();
	//if(envmap)
	//{
 //       lighting_stateset->setTextureAttributeAndModes(TEXTURE_REFLECTMAP, envmap,CRCore::crStateAttribute::ON);
	//}

	CRCore::ref_ptr<CRCore::crProgram> program;
	CRCore::ref_ptr<CRCore::crShader> shader;
	CRCore::ref_ptr<CRCore::crUniform> uniform;

	//lighting_stateset->setMode(GL_LIGHTING, crStateAttribute::OFF);//可能存在需要计算阴影但又不受阴影影响不使用shader计算光照的模型
    
	crPolygonOffset *polygonOffset = new crPolygonOffset(1.0f,1.0f);
	lighting_stateset->setAttribute(polygonOffset);
	lighting_stateset->setMode(GL_POLYGON_OFFSET_FILL,crStateAttribute::ON);

	program = new CRCore::crProgram;
	lighting_stateset->setAttributeAndModes(program.get(),CRCore::crStateAttribute::ON);
	program->setName("ambient");
	program->addBindAttribLocation("tangent",ATTRIBUTE_TANGENT);
	program->addBindAttribLocation("binormal",ATTRIBUTE_BINORMAL);

	if(m_indoor)
	{
	    shader = new CRCore::crShader(CRCore::crShader::VERTEX,ambient_vp_indoor);
		shader->setName("ambient_vp_indoor");
		program->addShader(shader.get());
		shader = new CRCore::crShader(CRCore::crShader::FRAGMENT,ambient_fp_indoor);
		shader->setName("ambient_fp_indoor");
		program->addShader(shader.get());

		uniform = new CRCore::crUniform(crUniform::SAMPLER_2D,"BaseMap");
		uniform->set(TEXTURE_BASEMAP);
		lighting_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::SAMPLER_2D,"BumpMap");
		uniform->set(TEXTURE_BUMPMAP);
		lighting_stateset->addUniform(uniform.get());

		//uniform = new CRCore::crUniform(crUniform::SAMPLER_CUBE,"EnvMap");
		//uniform->set(TEXTURE_REFLECTMAP);
		//lighting_stateset->addUniform(uniform.get());
	}
	else
	{
		//shader = new CRCore::crShader(CRCore::crShader::VERTEX,ambient_vp_outdoor);
		//shader->setName("ambient_vp_outdoor");
		//program->addShader(shader.get());
		//shader = new CRCore::crShader(CRCore::crShader::FRAGMENT,ambient_fp_outdoor);
		//shader->setName("ambient_fp_outdoor");
		//program->addShader(shader.get());

		//uniform = new CRCore::crUniform(crUniform::FLOAT_VEC3,"lightPos");
		//lighting_stateset->addUniform(uniform.get());
		//uniform = new CRCore::crUniform(crUniform::FLOAT_VEC4,"lightColor");
		//lighting_stateset->addUniform(uniform.get());
		//uniform = new CRCore::crUniform(crUniform::BOOL,"bumpMapSwitchOff");
		//lighting_stateset->addUniform(uniform.get());

		//CRCore::crClipPlane *clipplane;
		//crClipNode *clipNode;

		//clipNode = new crClipNode;
		//clipNode->setCullingActive(false);

		//for (int i = 0; i < 6; i++)
		//{
		//	clipplane = new crClipPlane;
		//	clipplane->setClipPlaneNum(i);
		//	clipNode->addClipPlane(clipplane);
		//}

		//for(NodeArray::iterator itr=m_childArray.begin();
		//	itr!=m_childArray.end();
		//	++itr)
		//{
		//	if(typeid(*(itr->get()))==typeid(CRCore::crCameraNode)) break;	
		//	clipNode->addChild(itr->get()); 
		//	this->removeChild(itr->get());
		//}

		//this->insertChild(0,clipNode);

		//shader = new CRCore::crShader(CRCore::crShader::VERTEX,sunlighting_vp);
		//shader->setName("sunlighting_vp");
		//program->addShader(shader.get());
		//shader = new CRCore::crShader(CRCore::crShader::FRAGMENT,sunlighting_fp);
		//shader->setName(sunlighting_fp);
		//program->addShader(shader.get());

		uniform = new CRCore::crUniform(crUniform::FLOAT_MAT4,"lightMVP0");
		lighting_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::FLOAT_MAT4,"lightMVP1");
		lighting_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::FLOAT_VEC4,"lightPos");
		lighting_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::SAMPLER_2D,"BaseMap");
		uniform->set(TEXTURE_BASEMAP);
		lighting_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::SAMPLER_2D,"BumpMap");
		uniform->set(TEXTURE_BUMPMAP);
		lighting_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::SAMPLER_2D,"ShadowMap0");
		uniform->set(TEXTURE_SHADOWMAP0);
		lighting_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::SAMPLER_2D,"ShadowMap1");
		uniform->set(TEXTURE_SHADOWMAP1);
		lighting_stateset->addUniform(uniform.get());

		//uniform = new CRCore::crUniform(crUniform::SAMPLER_CUBE,"EnvMap");
		//uniform->set(TEXTURE_REFLECTMAP);
		//lighting_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::FLOAT_VEC4,"lightColor");
		lighting_stateset->addUniform(uniform.get());
		
		//uniform = new CRCore::crUniform(crUniform::FLOAT_VEC4,"fogColor");
  //      uniform->set(fogColor);
		//lighting_stateset->addUniform(uniform.get());

		//uniform = new CRCore::crUniform(crUniform::BOOL_VEC3,"switchOff");
		//lighting_stateset->addUniform(uniform.get());
	}
}

bool crProgramNode::computeBound() const
{
	m_BoundSphere.init();
	m_bbox.init();

	if (m_childArray.empty()) 
	{
		m_bBoundSphere_computed = false;
		return false;
	}

	for(NodeArray::const_iterator itr=m_childArray.begin();
		itr!=m_childArray.end();
		++itr)
	{
		if(typeid(*(itr->get()))==typeid(CRCore::crCameraNode)) break;	
		m_bbox.expandBy((*itr)->getBoundBox());
	}
	
	if (!m_bbox.valid()) 
	{
		m_bBoundSphere_computed = true;
		return false;
	}

	m_BoundSphere.expandBy(m_bbox);

	m_bBoundSphere_computed = true;
	return true;
}

void crProgramNode::updateUniform(crViewPort *sceneViewPort)
{
	crStateSet *this_stateset = this->getStateSet();
	crStateSet *shadow_stateset,*lighting_stateset,*shadow_stateset1;
	const crLightSource *light_source = NULL;
	const crLight *light = NULL;
	crCameraNode *shadow_camera = NULL;
	crCameraNode *lighting_camera = NULL;
	crCameraNode *shadow_camera1 = NULL;
	crClipNode *clipNode;
	crPlane plane;
    int lightRenderMode = None;
    crVector4 lightPos;

	ActiveCameraParams activeCameraParams;

	crPolytope light_frustum;

	crTexture *shadowMap;
	crUniform *uniform;
	crDepth *depth;
   
	int i = 1;
	//SUN先渲染
	for(Active_LS_CameraMap::iterator aitr=m_active_ls_cameraMap_buf.begin();
		aitr!=m_active_ls_cameraMap_buf.end();
		++aitr, ++i)
	{
		lightRenderMode = aitr->first;
		activeCameraParams = aitr->second;
        light_source = activeCameraParams.m_lightSource.get();
		shadow_camera = activeCameraParams.m_cameraNode.get();
		shadow_stateset = shadow_camera->getStateSet();

		if(lightRenderMode & SunLight)
		{
			lighting_stateset = this_stateset;
			shadow_camera1 = shadow_camera->getDependCameraNode();
			shadow_stateset1 = shadow_camera1->getStateSet();
		}
		else
		{
			lighting_camera = shadow_camera->getDependCameraNode();
			lighting_stateset = lighting_camera->getStateSet();
		}

		light = dynamic_cast<const crLight *>(light_source->getLight());
		const CRCore::crVector4& lightPosition = light->getPosition();
		const crMatrix &lightMat = light->getLightMatrix();  
		lightPos.set(lightPosition[0],lightPosition[1],lightPosition[2],1.0);
		lightPos = lightPos * lightMat;
		lightPos.w() = lightRenderMode & SunLight ? SUNLIGHTSCALE : SPOTLIGHTSCALE;
		uniform = lighting_stateset->getUniform("lightPos");
		uniform->set(lightPos);

		const CRCore::crVector4& lightColor = light->getDiffuse();
		uniform = lighting_stateset->getUniform("lightColor");
		uniform->set(lightColor);

		//lightMVP = *shadow_camera->getBufViewMatrix() * 
		//	*shadow_camera->getBufProjectionMatrix();

		if(lightRenderMode & SunLight)
		{
			uniform = lighting_stateset->getUniform("lightMVP0");
			uniform->set(activeCameraParams.m_lightMVP0);

			uniform = lighting_stateset->getUniform("lightMVP1");
			uniform->set( activeCameraParams.m_lightMVP1 );/**shadow_camera1->getBufViewMatrix() * 
				          *shadow_camera1->getBufProjectionMatrix());*/

			uniform = lighting_stateset->getUniform("switchOff");
			uniform->set(!(lightRenderMode & ParallaxMapping),!(lightRenderMode & ShadowMapping),!(lightRenderMode & Fog));
			////计算lighting_stateset的ClipPlane
			//light_frustum.clear();
			//light_frustum.setToUnitFrustum(true,true);
			//light_frustum.transformProvidingInverse( lightMVP );
			//crPolytope::PlaneList& pl = light_frustum.getPlaneList();

			//clipNode = dynamic_cast<crClipNode *>(this->getChild(0));
			//if(clipNode)
			//{
			//	for(int i = 0; i<6; i++)
			//	{
			//		clipNode->getClipPlane(i)->setClipPlane(pl[i]);
			//	}
			//}
		}
		else
		{
			lighting_camera->setViewport(sceneViewPort);

			uniform = lighting_stateset->getUniform("lightMVP");
			uniform->set(activeCameraParams.m_lightMVP0);
			uniform = lighting_stateset->getUniform("switchOff");
			uniform->set(!(lightRenderMode & ParallaxMapping),!(lightRenderMode & ShadowMapping));

			//计算lighting_stateset的ClipPlane
			light_frustum.clear();
			light_frustum.setToUnitFrustum(true,true);
			light_frustum.transformProvidingInverse( activeCameraParams.m_lightMVP0 );
			crPolytope::PlaneList& pl = light_frustum.getPlaneList();

			clipNode = dynamic_cast<crClipNode *>(lighting_camera->getChild(0));
			if(clipNode)
			{
				for(int i = 0; i<6; i++)
				{
					clipNode->getClipPlane(i)->setClipPlane(pl[i]);
				}
			}

			lighting_stateset->setTextureAttributeAndModes(TEXTURE_LIGHTMAP0,const_cast<crTexture2D *>(light->getLightTexture()),crStateAttribute::ON/*|crStateAttribute::OVERRIDE*/);

			depth = dynamic_cast<crDepth *>(lighting_stateset->getAttribute(crStateAttribute::DEPTH));
			if(depth)
			{
				if(i==m_active_ls_cameraMap_buf.size())
					depth->setWriteMask(true);
				else
					depth->setWriteMask(false);
			}
		}
		if(lightRenderMode & ShadowMapping)
		{
			uniform = shadow_stateset->getUniform("lightMVP");
			uniform->set(activeCameraParams.m_lightMVP0);
			//uniform->set(*shadow_camera->getBufViewInverseMatrix()/*crMatrixd::inverse(*shadow_camera->getViewMatrix())*/);
			
			uniform = shadow_stateset->getUniform("lightPos");
			uniform->set(lightPos);

			crCameraNode::BufferAttachmentMap& bufferAttachmentMap = shadow_camera->getBufferAttachmentMap();
			shadowMap = bufferAttachmentMap[crCameraNode::COLOR_BUFFER].m_texture.get();
			
			lighting_stateset->setTextureAttributeAndModes(TEXTURE_SHADOWMAP0,shadowMap,crStateAttribute::ON/*|crStateAttribute::OVERRIDE*/);

			if(lightRenderMode & SunLight)
			{
				//uniform = shadow_stateset1->getUniform("lightViewInverse");
				//uniform->set(*shadow_camera1->getBufViewInverseMatrix());
				uniform = shadow_stateset1->getUniform("lightMVP");
				uniform->set(activeCameraParams.m_lightMVP1);

				uniform = shadow_stateset1->getUniform("lightPos");
				uniform->set(lightPos);

				crCameraNode::BufferAttachmentMap& bufferAttachmentMap1 = shadow_camera1->getBufferAttachmentMap();
				shadowMap = bufferAttachmentMap1[crCameraNode::COLOR_BUFFER].m_texture.get();

				lighting_stateset->setTextureAttributeAndModes(TEXTURE_SHADOWMAP1,shadowMap,crStateAttribute::ON/*|crStateAttribute::OVERRIDE*/);
			}
			//计算shadow_stateset的ClipPlane,测试代码
			//light_frustum.clear();
			//light_frustum.setToUnitFrustum(true,true);
			//light_frustum.transformProvidingInverse( *lighting_camera->getBufViewMatrix() * 
			//	                                             *lighting_camera->getBufProjectionMatrix() );
			//crPolytope::PlaneList& lpl = light_frustum.getPlaneList();
			//clipNode = dynamic_cast<crClipNode *>(shadow_camera->getChild(0));
			//if(clipNode)
			//{
			//	for(int i = 0; i<6; i++)
			//	{
			//		clipNode->getClipPlane(i)->setClipPlane(lpl[i]);
			//	}
			//}
		}
		else
		{
			lighting_stateset->removeTextureAttribute(TEXTURE_SHADOWMAP0,crStateAttribute::TEXTURE);
			if(lightRenderMode & SunLight)
			{
				lighting_stateset->removeTextureAttribute(TEXTURE_SHADOWMAP1,crStateAttribute::TEXTURE);
			}
		}
	}
}

void crProgramNode::swapBuffers()
{
    m_active_ls_cameraMap_buf.swap(m_active_ls_cameraMap);
}

void crProgramNode::traverse(crNodeVisitor& nv)
{
	crCameraNode *shadow_camera = NULL;
	if (nv.getVisitorType()==CRCore::crNodeVisitor::CULL_VISITOR)
	{
		crCameraNode *lighting_camera = NULL;
		crPolytope light_frustum;
		const crLightSource *light_source = NULL;
		const crLight *light = NULL;
		crStateSet *lighting_stateset = getStateSet();
	    int lightRenderMode = None;
		float lightFov;
		crVector3 lightPos,lightDir,lightDir1,lightDir2;
		crVector3d eye,center,up,lv,maxlight,lightPrjCenter;
		//crMatrixd lightMVP;

        /////
		bool culled;
		float cutoff,d;
        std::vector<crVector3> frustumVecList;
        crVector3 A,B,C,D,R,R1;
        crPolytope view_frustum;
		view_frustum.setToUnitFrustum(true,true);
		view_frustum.transformProvidingInverse( m_currentViewMatrix * m_currentProjectMatrix );

		m_currentViewMatrix.getLookAt(eye,center,up);
		eye.z() = 0.0f;
		center.z() = 0.0f;
		lv = center - eye;
		lv.normalize();

		m_active_ls_cameraMap.clear();
		//m_active_ls_lightmvpMap.clear();
		ActiveCameraParams activeCameraParams;
		for( LS_CameraMap::iterator itr = m_ls_cameraMap.begin();
			itr != m_ls_cameraMap.end();
			++itr )
		{//当前将要用于计算阴影的光源的取得方法（平行光不做计算）：
		 //1.太阳光：直接取得
		 //2.点光：
	     //3.spotLit：光源沿其方向的射线与水平面相交，交点落在视点处取景面积为10×20平方米范围内
			light_source = itr->first.get();
            shadow_camera = itr->second.get();
            if(!light_source->getEnable()) continue;

			lighting_camera = shadow_camera->getDependCameraNode();
			
			lighting_camera->setProjectionMatrix(m_currentProjectMatrix);
			lighting_camera->setViewMatrix(m_currentViewMatrix);
			shadow_camera->setProjectionMatrix(m_currentProjectMatrix);
			shadow_camera->setViewMatrix(m_currentViewMatrix);

			light = dynamic_cast<const crLight *>(light_source->getLight());
			const CRCore::crVector4& lightPosition = light->getPosition();
			const crMatrix &lightMat = light->getLightMatrix();  
			lightPos.set(lightPosition[0],lightPosition[1],lightPosition[2]);
			lightPos = lightPos * lightMat;

			lightRenderMode = None;

            if(light_source->getLightIsSunLight())
			{				 
				//LightMVPS lightmvps;

				lightRenderMode = LightMapping| ParallaxMapping | ShadowMapping | Fog | SunLight;
                //计算shadow_camera的视锥
				maxlight = eye + lv * c_sunViewRangeForBig;
                lightDir1 = eye - lightPos;
				lightDir2 = maxlight - lightPos;
				lightDir1.normalize();
				lightDir2.normalize();
                lightFov = acos(lightDir1 * lightDir2);
                lightDir = (lightDir1 + lightDir2) * 0.5f;
				
				activeCameraParams.m_lightMVP0 = crMatrix::lookAt(lightPos,lightPos+lightDir,Y_AXIS)
					                    * crMatrix::perspective(RadiansToDegrees(lightFov),1.0,1.0f,SUNLIGHT_RANGE);
                
				shadow_camera->clearExternFrustum();
				light_frustum.clear();
				light_frustum.setToUnitFrustum(true,true);
				light_frustum.transformProvidingInverse( activeCameraParams.m_lightMVP0 );
				shadow_camera->addExternFrustum(light_frustum);

				//shadow_camera->setProjectionMatrixAsPerspective(RadiansToDegrees(lightFov),1.0,1.0f,SUNLIGHT_RANGE);
				//shadow_camera->setViewMatrixAsLookAt(lightPos,lightPos+lightDir,Y_AXIS);

				maxlight = eye + lv * c_sunViewRangeForSmall;
				lightDir2 = maxlight - lightPos;
				lightDir2.normalize();
				lightFov = acos(lightDir1 * lightDir2);
				lightDir = (lightDir1 + lightDir2) * 0.5f;

				activeCameraParams.m_lightMVP1 = crMatrix::lookAt(lightPos,lightPos+lightDir,Y_AXIS)
					* crMatrix::perspective(RadiansToDegrees(lightFov),1.0,1.0f,SUNLIGHT_RANGE);

				lighting_camera->clearExternFrustum();
				light_frustum.clear();
				light_frustum.setToUnitFrustum(true,true);
				light_frustum.transformProvidingInverse( activeCameraParams.m_lightMVP1 );
				lighting_camera->addExternFrustum(light_frustum);

				//lighting_camera->setProjectionMatrixAsPerspective(RadiansToDegrees(lightFov),1.0,1.0f,SUNLIGHT_RANGE);
				//lighting_camera->setViewMatrixAsLookAt(lightPos,lightPos+lightDir,Y_AXIS);
                //m_active_ls_lightmvpMap.insert(std::make_pair(light_source,lightmvps));
			}
			else
			{//室内场景
				//lighting_camera->setProjectionMatrix(m_currentProjectMatrix);
				//lighting_camera->setViewMatrix(m_currentViewMatrix);

				switch(light->getLightType()) 
				{//点光源默认朝下，当需要计算点光源朝上的阴影成象问题时，可以使用Z_AXIS朝向的聚光灯代替，该聚光灯的lightmap设置为SunLightMap
				case crLight::LIGHT_LOCAL:
					{//
						//lightRenderMode = LightMapping| ParallaxMapping | ShadowMapping;
						////计算shadow_camera的视锥
						//lightFov = 2.0f * atan((0.5f*c_viewLength / lightPos.z()));
						//shadow_camera->setProjectionMatrixAsPerspective(lightFov,1.0,1.0f,LIGHT_RANGE);
						//shadow_camera->setViewMatrixAsLookAt(lightPos,lightCenter,Y_AXIS);
						continue;
					}
					break;
				case crLight::LIGHT_SPOT:
					{//收集光源，室内场景也是一样的收集算法
					 //判断光源视锥是否与视锥相交
						const crVector3 &spotLightDir = light->getDirection();
						cutoff = light->getSpotCutoff();

						lightFov = DegreesToRadians(cutoff);
                        d = tan(lightFov) * SPOTLIGHT_RANGE;
						R = (spotLightDir^Y_AXIS) * d;
						R1 = Y_AXIS * d;

						A = spotLightDir * SPOTLIGHT_RANGE + R1 + R;
						B = spotLightDir * SPOTLIGHT_RANGE - R1 + R;
						C = spotLightDir * SPOTLIGHT_RANGE + R1 - R;
						D = spotLightDir * SPOTLIGHT_RANGE - R1 - R;

						frustumVecList.clear();
						frustumVecList.push_back(lightPos);
						frustumVecList.push_back(A);
						frustumVecList.push_back(B);
						frustumVecList.push_back(C);
						frustumVecList.push_back(D);
						
						view_frustum.pushCurrentMask();
						culled = !view_frustum.contains(frustumVecList);
						view_frustum.popCurrentMask();
                        
						if(culled)
                            lightRenderMode = None;
						else
                            lightRenderMode = LightMapping| ParallaxMapping | ShadowMapping;

						//lightPrjCenter = lightPos + _lightDir * (-lightPos[2] / _lightDir[2]);
						//lightDir1 = lv;
						//lightDir2 = lightPrjCenter - eye;
      //                  lightDir2.normalize();
						///*lightPrjR = (lightPos - lightPrjCenter).length() * tan(DegreesToRadians(cutoff));
						//lightPrjCenter2.set(lightPrjCenter[0],lightPrjCenter[1]);
      //                  light_eye2 = lightPrjCenter2 - eye2;
						//light_eye2.normalize();*/
						//cos_angle = lightDir1 * lightDir2;
						//l = (lightPrjCenter - eye).length();
						//if(cos_angle > 0)
						//{
						//	if(l<c_spotLightViewRange)
						//	{//这个光源将被用于计算阴影
						//		lightRenderMode = LightMapping| ParallaxMapping | ShadowMapping;
						//	}
						//	else if(l<c_spotLightViewRange2)
						//	{//这个光源将被用于计算lightmap，不计算阴影也不计算normalmap
						//		lightRenderMode = LightMapping| ParallaxMapping;
						//	}
						//	else if(l<c_spotLightViewRange3)
						//		lightRenderMode = LightMapping;
						//	else
						//		lightRenderMode = None;
						//}
						//else
						//{
						//	if(l<c_spotLightViewRange)
						//	{//这个光源将被用于计算阴影
						//		lightRenderMode = LightMapping| ParallaxMapping | ShadowMapping;
						//	}
						//	else
						//		lightRenderMode = None;
						//} 
                        //lightRenderMode = None;//调试用
						if(lightRenderMode!=None)
						{//计算spotLit的视锥
							//LightMVPS lightmvps;
							activeCameraParams.m_lightMVP0 = crMatrix::lookAt(lightPos,lightPos+spotLightDir,Y_AXIS)
								* crMatrix::perspective(2.0f*cutoff,1.0f,1.0f,SPOTLIGHT_RANGE);

							shadow_camera->clearExternFrustum();
							lighting_camera->clearExternFrustum();
							light_frustum.clear();
							light_frustum.setToUnitFrustum(true,true);
							light_frustum.transformProvidingInverse( activeCameraParams.m_lightMVP0 );
							shadow_camera->addExternFrustum(light_frustum);
							lighting_camera->addExternFrustum(light_frustum);
							//m_active_ls_lightmvpMap.insert(std::make_pair(light_source,lightmvps));

							//shadow_camera->setProjectionMatrixAsPerspective(2.0f*cutoff,1.0f,1.0f,SPOTLIGHT_RANGE);
							//shadow_camera->setViewMatrixAsLookAt(lightPos,lightPos+spotLightDir,Y_AXIS);
						}
					}
					break;
				}
			}

			shadow_camera->setShallRender(lightRenderMode&ShadowMapping);

			if(lightRenderMode!=None)
			{
    ////            if(lightRenderMode&SunLight)
				////{
				////	nv.clearExternFrustum();
				////	light_frustum.clear();
				////    light_frustum.setToUnitFrustum(true,true);
				////    light_frustum.transformProvidingInverse( lightMVP );
				////    nv.addExternFrustum(light_frustum);
				////}
				//if(!(lightRenderMode&SunLight))
				//{
				//	//lightMVP = *shadow_camera->getViewMatrix() * 
				//	//	*shadow_camera->getProjectionMatrix();

				//	lighting_camera->clearExternFrustum();
				//	light_frustum.clear();
				//	light_frustum.setToUnitFrustum(true,true);
				//	light_frustum.transformProvidingInverse( lightMVP );
				//	lighting_camera->addExternFrustum(light_frustum);
				//}

				////////////////////////////以下为测试代码
				////lightMVP = m_currentViewMatrix * m_currentProjectMatrix;
				////shadow_camera->clearExternFrustum();
				////light_frustum.clear();
				////light_frustum.setToUnitFrustum(true,true);
				////light_frustum.transformProvidingInverse( lightMVP );
				////shadow_camera->addExternFrustum(light_frustum);
    //            //////////////////////////////////

			 //   //LS_CameraPair ls_cameraPair(itr->first,itr->second);
				////Active_LS_CameraMapPair active_ls_cameraMap(lightRenderMode,std::make_pair(itr->first,itr->second));

				activeCameraParams.m_lightSource = itr->first;
				activeCameraParams.m_cameraNode = itr->second;
			    m_active_ls_cameraMap.insert(std::make_pair(lightRenderMode,activeCameraParams));
			}
		}
        
		for(Active_LS_CameraMap::iterator aitr=m_active_ls_cameraMap.begin();
			aitr!=m_active_ls_cameraMap.end();
			++aitr)
		{
			shadow_camera = aitr->second.m_cameraNode.get();
			if(shadow_camera && shadow_camera->getShallRender())
				shadow_camera->accept(nv);
			lighting_camera = shadow_camera->getDependCameraNode();
			if(lighting_camera && lighting_camera->getShallRender())
				lighting_camera->accept(nv);
		}

		nv.setShadowMappingNodeMainTraverse(true);
		for(NodeArray::iterator nitr=m_childArray.begin();
			nitr!=m_childArray.end();
			++nitr)
		{
			if(typeid(*(nitr->get()))==typeid(CRCore::crCameraNode)) break;
			(*nitr)->accept(nv);
		}
		nv.setShadowMappingNodeMainTraverse(false);

	}
	else if(nv.getVisitorType()==CRCore::crNodeVisitor::INIT_VISITOR)
	{
		crGroup::traverse(nv);
	}
	else
	{
		for(NodeArray::iterator itr=m_childArray.begin();
			itr!=m_childArray.end();
			++itr)
		{
			if(typeid(*(itr->get()))==typeid(CRCore::crCameraNode)) break;
			(*itr)->accept(nv);
		}
	}
}

