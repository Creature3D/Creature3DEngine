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
#include <CRCore/crRenderToTextureNode.h>
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

#include <CRCore/crCullFace.h>

#include <CRCore/crBrain.h>

using namespace CRCore;

//////////////////////////////////////////////////////////////////
crRenderToTextureNode::crRenderToTextureNode()
{
	m_indoor = true;
	setNumChildrenRequiringUpdateUniformTraversal(1);
}

crRenderToTextureNode::crRenderToTextureNode(const crRenderToTextureNode& shadowNode,const crCopyOp& copyop):
    crGroup(shadowNode,copyop),
	m_indoor(shadowNode.m_indoor),
    m_ls_cameraMap(shadowNode.m_ls_cameraMap)
{
}

void crRenderToTextureNode::insertLightSource( CRCore::crLightSource *ls,  ref_ptr<crCameraNode> &shadow_camera, ref_ptr<crCameraNode> &lighting_camera )
{
	if(m_ls_cameraMap.find(ls)!=m_ls_cameraMap.end()) return;
	std::string name;

	//crCullFace *cullFace = new crCullFace(crCullFace::FRONT);
	shadow_camera = new crCameraNode;
	crStateSet *shadow_stateset = shadow_camera->getOrCreateStateSet();
    //CRCore::crClipPlane *clipplane;
	//crClipNode *clipNode;

	ref_ptr<crTexture2D> shadowMap = new crTexture2D;
	shadowMap->setInternalFormat(GL_RGB16F_ARB);//GL_RGB16F_ARB
	shadowMap->setSourceFormat(GL_RGB);
	shadowMap->setSourceType(GL_FLOAT);
	shadowMap->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
	shadowMap->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
    //shadowMap->setMaxAnisotropy(16.0);
	shadowMap->setWrap(crTexture2D::WRAP_S, crTexture2D::CLAMP);
	shadowMap->setWrap(crTexture2D::WRAP_T, crTexture2D::CLAMP);
	if(ls->getLightIsSunLight())
	{
		setIndoorFlag(false);
		shadowMap->setTextureSize(c_sunShadowMapSize[0], c_sunShadowMapSize[1]);
		shadow_camera->setViewport(0,0,c_sunShadowMapSize[0],c_sunShadowMapSize[1]);
		shadow_camera->setRenderMode(crCameraNode::SunShadowMapRenderForBig);

  //      shadow_camera->addChild(m_childArray[0].get());
		//shadow_camera->addChild(m_childArray[1].get());
		//shadow_camera->addChild(m_childArray[2].get());
	}
	else
	{	
		shadowMap->setTextureSize(c_shadowMapSize[0], c_shadowMapSize[1]);
		shadow_camera->setViewport(0,0,c_shadowMapSize[0],c_shadowMapSize[1]);
		shadow_camera->setRenderMode(crCameraNode::ShadowMapRender);

		//shadow_camera->addChild(m_childArray[0].get());
		//shadow_camera->addChild(m_childArray[2].get());
		//shadow_camera->addChild(m_childArray[3].get());
		//shadow_camera->addChild(m_childArray[5].get());
		//shadow_camera->addChild(m_childArray[7].get());
	}

	shadow_camera->setClearColor(crVector4(1.0f, 1.0f, 1.0f, 1.0f));
	shadow_camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shadow_camera->setComputeNearFarMode(crCameraNode::DO_NOT_COMPUTE_NEAR_FAR);

	shadow_camera->setRenderOrder(crCameraNode::PRE_RENDER);
	shadow_camera->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER_OBJECT);
	shadow_camera->attach(crCameraNode::COLOR_BUFFER, shadowMap.get(),0,0,false);
	shadow_camera->setDrawBuffer(GL_BACK);
	shadow_camera->setReadBuffer(GL_BACK);
	shadow_camera->setReferenceFrame(crCameraNode::RELATIVE_TO_ABSOLUTE);
	shadow_camera->setProjectionMatrix(crMatrix::identity());
	shadow_camera->setBufProjectionMatrix(crMatrix::identity());  
	shadow_camera->setViewMatrix(crMatrix::identity());
	shadow_camera->setBufViewMatrix(crMatrix::identity());

	shadow_camera->setCullingMode( crCameraNode::VIEW_FRUSTUM_CULLING |
		                           crCameraNode::SMALL_FEATURE_CULLING |
		                           crCameraNode::CLUSTER_CULLING );

	this->addChild(shadow_camera.get());

	shadow_stateset->setMode(GL_LIGHTING, crStateAttribute::OVERRIDE|crStateAttribute::OFF);
	//shadow_stateset->setAttributeAndModes(cullFace,crStateAttribute::OVERRIDE|crStateAttribute::ON);

	//shadow_stateset->setMode(GL_BLEND, crStateAttribute::OFF);
	//shadow_stateset->setTextureMode(0,GL_TEXTURE_2D,crStateAttribute::OVERRIDE|crStateAttribute::OFF);
	//ati fbo 对以下三个不能硬件支持
	//shadow_stateset->setMode(GL_BLEND, crStateAttribute::OFF);
	//shadow_stateset->setMode(GL_ALPHA_TEST, crStateAttribute::OFF);
	//shadow_stateset->setMode(GL_FOG, crStateAttribute::OFF);

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

	uniform = new CRCore::crUniform(crUniform::FLOAT_MAT4,"lightViewInverse");
	shadow_stateset->addUniform(uniform.get());

	uniform = new CRCore::crUniform(crUniform::BOOL,"computeAlphaShadow");
	uniform->set(false);
	shadow_stateset->addUniform(uniform.get());



	//uniform = new CRCore::crUniform(crUniform::FLOAT_VEC3,"lightPos");
	//shadow_stateset->addUniform(uniform.get());

//////////////create lighting camera
	if(ls->getLightIsSunLight())
	{
		lighting_camera = new crCameraNode;
		crCameraNode *detal_shadow_camera = lighting_camera.get();
		crStateSet *detal_shadow_stateset = detal_shadow_camera->getOrCreateStateSet();
		shadow_camera->setDependCameraNode(detal_shadow_camera,false);

		//detal_shadow_camera->addChild(m_childArray[3].get());
		//detal_shadow_camera->addChild(m_childArray[4].get());
		//detal_shadow_camera->addChild(m_childArray[5].get());

		shadowMap = new crTexture2D;
		shadowMap->setInternalFormat(GL_RGB16F_ARB);//GL_RGB16
		shadowMap->setSourceFormat(GL_RGB);
		shadowMap->setSourceType(GL_FLOAT);
		shadowMap->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
		shadowMap->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
		shadowMap->setWrap(crTexture2D::WRAP_S, crTexture2D::CLAMP);
		shadowMap->setWrap(crTexture2D::WRAP_T, crTexture2D::CLAMP);
		//shadowMap->setMaxAnisotropy(16.0);
		shadowMap->setTextureSize(c_shadowMapSize[0], c_shadowMapSize[1]);
		detal_shadow_camera->setViewport(0,0,c_shadowMapSize[0],c_shadowMapSize[1]);

		detal_shadow_camera->setClearColor(crVector4(1.0f, 1.0f, 1.0f, 1.0f));
		detal_shadow_camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		detal_shadow_camera->setComputeNearFarMode(crCameraNode::DO_NOT_COMPUTE_NEAR_FAR);
		detal_shadow_camera->setRenderMode(crCameraNode::SunShadowMapRenderForSmall);

		detal_shadow_camera->setRenderOrder(crCameraNode::PRE_RENDER);
		detal_shadow_camera->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER_OBJECT);
		detal_shadow_camera->attach(crCameraNode::COLOR_BUFFER, shadowMap.get(),0,0,false);
		detal_shadow_camera->setDrawBuffer(GL_BACK);
		detal_shadow_camera->setReadBuffer(GL_BACK);
		detal_shadow_camera->setReferenceFrame(crCameraNode::RELATIVE_TO_ABSOLUTE);
		shadow_camera->setProjectionMatrix(crMatrix::identity());
		shadow_camera->setBufProjectionMatrix(crMatrix::identity());
		shadow_camera->setViewMatrix(crMatrix::identity());
		shadow_camera->setBufViewMatrix(crMatrix::identity());

		detal_shadow_camera->setCullingMode( crCameraNode::VIEW_FRUSTUM_CULLING |
			                                 crCameraNode::SMALL_FEATURE_CULLING |
			                                 crCameraNode::CLUSTER_CULLING );

		detal_shadow_stateset->setMode(GL_LIGHTING, crStateAttribute::OVERRIDE|crStateAttribute::OFF);
		//detal_shadow_stateset->setAttributeAndModes(cullFace,crStateAttribute::OVERRIDE|crStateAttribute::ON);

		//detal_shadow_stateset->setMode(GL_BLEND, crStateAttribute::OFF);
		//detal_shadow_stateset->setTextureMode(0,GL_TEXTURE_2D,crStateAttribute::OVERRIDE|crStateAttribute::OFF);
		////ati fbo 对以下三个不能硬件支持
		//detal_shadow_stateset->setMode(GL_BLEND, crStateAttribute::OFF);
		//detal_shadow_stateset->setMode(GL_ALPHA_TEST, crStateAttribute::OFF);
		//detal_shadow_stateset->setMode(GL_FOG, crStateAttribute::OFF);

		///////setup shader
		//program = new CRCore::crProgram;
		detal_shadow_stateset->setAttributeAndModes(program.get(),CRCore::crStateAttribute::ON);
		shadowMap->setImageNameID("detalShadowMap_"+ls->getName());

		uniform = new CRCore::crUniform(crUniform::FLOAT_MAT4,"lightViewInverse");
		detal_shadow_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::BOOL,"computeAlphaShadow");
		uniform->set(false);
		detal_shadow_stateset->addUniform(uniform.get());

		//uniform = new CRCore::crUniform(crUniform::FLOAT_VEC3,"lightPos");
		//detal_shadow_stateset->addUniform(uniform.get());
	}
	else
	{
		lighting_camera = new crCameraNode;
		crStateSet *lighting_stateset = lighting_camera->getOrCreateStateSet();

		shadow_camera->setDependCameraNode(lighting_camera.get(),false);
		
		//lighting_camera->addChild(m_childArray[0].get());
		//lighting_camera->addChild(m_childArray[3].get());
		//lighting_camera->addChild(m_childArray[7].get());

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
		lighting_camera->setCullingMode( crCameraNode::ENABLE_ALL_CULLING |
			                             crCameraNode::EXTERN_FRUSTUM_CULLING );

		lighting_stateset->setMode(GL_LIGHTING, crStateAttribute::OVERRIDE|crStateAttribute::OFF);

		//clipNode = new crClipNode;
		//clipNode->setCullingActive(false);

		//for (int i = 0; i < 6; i++)
		//{
		//	clipplane = new crClipPlane;
		//	clipplane->setClipPlaneNum(i);
		//	clipNode->addClipPlane(clipplane);
		//}

		//for(int i = 0; i < lighting_camera->getNumChildren(); ++i)
		//{
		//	clipNode->addChild(lighting_camera->getChild(i)); 
		//}

		//lighting_camera->removeChild(0,lighting_camera->getNumChildren());

		//lighting_camera->addChild(clipNode);

		lighting_stateset->setMode(crStateAttribute::VRMode(GL_POLYGON_OFFSET_FILL,NULL), crStateAttribute::OFF);
		crBlendFunc *blend = new crBlendFunc(crBlendFunc::ONE,crBlendFunc::ONE);
		lighting_stateset->setAttributeAndModes(blend);
		crDepth *depth = new crDepth;
		depth->setFunction(crDepth::LEQUAL);
		depth->setWriteMask(false);
		lighting_stateset->setAttributeAndModes(depth);

		uniform = new CRCore::crUniform(crUniform::FLOAT_MAT4,"lightMVP");
		lighting_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::FLOAT_VEC3,"lightPos");
		lighting_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::FLOAT_VEC4,"lightColor");
		lighting_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::FLOAT_VEC4,"params");
		crVector4f params(0.1f,0.05f,0.5f,1.0f);
		uniform->set(params);
		lighting_stateset->addUniform(uniform.get());

		//uniform = new CRCore::crUniform(crUniform::BOOL,"needRotTBN");
		//uniform->set(0);
		//lighting_stateset->addUniform(uniform.get());
	}

	//CameraPair camerapair(shadow_camera,lighting_camera);
	m_ls_cameraMap[ls] = shadow_camera;
	
	//if(ls->getName().compare("ls2")==0)
	//    renderShadowMap(this,shadowMap.get()/*dynamic_cast<crTexture2D *>(crBrain::getInstance()->getResource()->findOrLoadTexture(261))*/);
}

void crRenderToTextureNode::createShader(const crVector4& fogColor)
{
	CRCore::ref_ptr<CRCore::crUniform> uniform;

	//ref_ptr<crGroup> group;
	crStateSet *this_stateset = getOrCreateStateSet();
	if(m_indoor)
	{
		CRCore::ref_ptr<CRCore::crProgram> program;
		CRCore::ref_ptr<CRCore::crShader> shader;

		//group = new crGroup;
		//group->addChild(m_childArray[0].get());
		//group->addChild(m_childArray[3].get());
		//group->addChild(m_childArray[7].get());

		//for( NodeArray::iterator itr = group->getChildArray().begin(); 
		//	itr != group->getChildArray().end();
		//	++itr )
		//{
		//	removeChild(itr->get());
		//}
		//this->removeChild(0,1);
  //      this->removeChild(2);
		//this->removeChild(4);
        //insertChild(0,group.get());

        //lighting_stateset = this->getOrCreateStateSet();

		program = new CRCore::crProgram;
		this_stateset->setAttributeAndModes(program.get(),CRCore::crStateAttribute::ON);
		program->setName("ambient");
		program->addBindAttribLocation("tangent",ATTRIBUTE_TANGENT);
		program->addBindAttribLocation("binormal",ATTRIBUTE_BINORMAL);

	    shader = new CRCore::crShader(CRCore::crShader::VERTEX,ambient_vp_indoor);
		shader->setName("ambient_vp_indoor");
		program->addShader(shader.get());
		shader = new CRCore::crShader(CRCore::crShader::FRAGMENT,ambient_fp_indoor);
		shader->setName("ambient_fp_indoor");
		program->addShader(shader.get());
	}
	else
	{
		//group = new crGroup;
		//group->addChild(m_childArray[0].get());
		//group->addChild(m_childArray[1].get());
		//group->addChild(m_childArray[3].get());
		//group->addChild(m_childArray[4].get());
		//group->addChild(m_childArray[6].get());
        //m_noteffectwithsunlightnode = m_childArray[7].get();

		//for( NodeArray::iterator itr = group->getChildArray().begin(); 
		//	 itr != group->getChildArray().end();
		//	 ++itr )
		//{
		//	removeChild(itr->get());
		//}
		//this->removeChild(0,1);
		//this->removeChild(0,1);
		//this->removeChild(1);
		//this->removeChild(1);
		//this->removeChild(2);
		//insertChild(0,group.get());

		//lighting_stateset = group->getOrCreateStateSet();

		uniform = new CRCore::crUniform(crUniform::FLOAT_MAT4,"lightMVP");
		this_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::FLOAT_MAT4,"detailLightMVP");
		this_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::FLOAT_VEC3,"lightPos");
		this_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::FLOAT_VEC4,"lightColor");
		this_stateset->addUniform(uniform.get());
		
		//uniform = new CRCore::crUniform(crUniform::FLOAT_VEC4,"fogColor");
  //      uniform->set(fogColor);
		//lighting_stateset->addUniform(uniform.get());

		uniform = new CRCore::crUniform(crUniform::FLOAT_VEC4,"params");
		crVector4f params(0.1f,0.05f,0.5f,1.0f);
		uniform->set(params);
		this_stateset->addUniform(uniform.get());

		//uniform = new CRCore::crUniform(crUniform::BOOL,"needRotTBN");
		//uniform->set(0);
		//this_stateset->addUniform(uniform.get());

		/////临时
		//uniform = new CRCore::crUniform(crUniform::BOOL,"softShadow");
		//uniform->set(true);
		//lighting_stateset->addUniform(uniform.get(),crStateAttribute::OVERRIDE);
		//uniform = new CRCore::crUniform(crUniform::BOOL,"fog");
		//uniform->set(true);
		//lighting_stateset->addUniform(uniform.get(),crStateAttribute::OVERRIDE);
		////临时end
	}

	crPolygonOffset *polygonOffset = new crPolygonOffset(1.0f,1.0f);
	this_stateset->setAttribute(polygonOffset);
	this_stateset->setMode(GL_POLYGON_OFFSET_FILL,crStateAttribute::ON);
	this_stateset->setMode(GL_LIGHTING, crStateAttribute::OVERRIDE|crStateAttribute::OFF);
}

bool crRenderToTextureNode::computeBound() const
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
		if(typeid(*(itr->get()))==typeid(CRCore::crCameraNode)) continue;	
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

void crRenderToTextureNode::updateUniform(crViewPort *sceneViewPort)
{
	crStateSet *this_stateset = this->getStateSet();
	crStateSet *shadow_stateset,*lighting_stateset,*detal_shadow_stateset;
	const crLightSource *light_source = NULL;
	const crLight *light = NULL;
	crCameraNode *shadow_camera = NULL;
	crCameraNode *lighting_camera = NULL;
	crCameraNode *detal_shadow_camera = NULL;
	crClipNode *clipNode;
	crPlane plane;
    int lightRenderMode = None;

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

		if(lightRenderMode == SunLight)
		{
			lighting_stateset = this_stateset;//m_childArray[0]->getStateSet();
			detal_shadow_camera = shadow_camera->getDependCameraNode();
			detal_shadow_stateset = detal_shadow_camera->getStateSet();
		}
		else
		{
			lighting_camera = shadow_camera->getDependCameraNode();
			lighting_stateset = lighting_camera->getStateSet();
		}

		light = dynamic_cast<const crLight *>(light_source->getLight());
		uniform = lighting_stateset->getUniform("lightPos");
		uniform->set(activeCameraParams.m_lightPos);

		const CRCore::crVector4& lightColor = light->getDiffuse();
		uniform = lighting_stateset->getUniform("lightColor");
		uniform->set(lightColor);

		if(lightRenderMode == SunLight)
		{
			uniform = lighting_stateset->getUniform("lightMVP");
			uniform->set(activeCameraParams.m_lightMVP);
			
			uniform = lighting_stateset->getUniform("detailLightMVP");
			uniform->set( activeCameraParams.m_detailLightMVP );
		}
		else
		{
			lighting_camera->setViewport(sceneViewPort);

			uniform = lighting_stateset->getUniform("lightMVP");
			uniform->set(activeCameraParams.m_lightMVP);
			//uniform = lighting_stateset->getUniform("switchOff");
			//uniform->set(!(lightRenderMode & ParallaxMapping),!(lightRenderMode & ShadowMapping));

			//计算lighting_stateset的ClipPlane
			light_frustum.clear();
			light_frustum.setToUnitFrustum(true,true);
			light_frustum.transformProvidingInverse( activeCameraParams.m_lightMVP );
			crPolytope::PlaneList& pl = light_frustum.getPlaneList();

			clipNode = dynamic_cast<crClipNode *>(lighting_camera->getChild(0));
			if(clipNode)
			{
				for(int i = 0; i<6; i++)
				{
					clipNode->getClipPlane(i)->setClipPlane(pl[i]);
				}
			}

			lighting_stateset->setTextureAttributeAndModes(TEXTURE_LIGHTMAP,const_cast<crTexture2D *>(light->getLightTexture()),crStateAttribute::ON/*|crStateAttribute::OVERRIDE*/);

			depth = dynamic_cast<crDepth *>(lighting_stateset->getAttribute(crStateAttribute::DEPTH));
			if(depth)
			{
				if(i==m_active_ls_cameraMap_buf.size())
					depth->setWriteMask(true);
				else
					depth->setWriteMask(false);
			}
		}
		//if(lightRenderMode & ShadowMapping)
		{
			uniform = shadow_stateset->getUniform("lightViewInverse");
			uniform->set(*shadow_camera->getBufViewInverseMatrix());
			
			//uniform = shadow_stateset->getUniform("lightPos");
			//uniform->set(activeCameraParams.m_lightPos);

			crCameraNode::BufferAttachmentMap& bufferAttachmentMap = shadow_camera->getBufferAttachmentMap();
			shadowMap = bufferAttachmentMap[crCameraNode::COLOR_BUFFER].m_texture.get();
			
			lighting_stateset->setTextureAttributeAndModes(TEXTURE_SHADOWMAP,shadowMap,crStateAttribute::ON/*|crStateAttribute::OVERRIDE*/);

			if(lightRenderMode == SunLight)
			{
				uniform = detal_shadow_stateset->getUniform("lightViewInverse");
				uniform->set(*detal_shadow_camera->getBufViewInverseMatrix());

				//uniform = detal_shadow_stateset->getUniform("lightPos");
				//uniform->set(activeCameraParams.m_lightPos);

				crCameraNode::BufferAttachmentMap& bufferAttachmentMap1 = detal_shadow_camera->getBufferAttachmentMap();
				shadowMap = bufferAttachmentMap1[crCameraNode::COLOR_BUFFER].m_texture.get();

				lighting_stateset->setTextureAttributeAndModes(TEXTURE_DETAILSHADOWMAP,shadowMap,crStateAttribute::ON/*|crStateAttribute::OVERRIDE*/);
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
		//else
		//{
		//	lighting_stateset->removeTextureAttribute(TEXTURE_SHADOWMAP,crStateAttribute::TEXTURE);
		//	if(lightRenderMode & SunLight)
		//	{
		//		lighting_stateset->removeTextureAttribute(TEXTURE_DETAILSHADOWMAP,crStateAttribute::TEXTURE);
		//	}
		//}

		//////////临时
		//uniform = lighting_stateset->getUniform("parallaxParam");
		//if(crBrain::getInstance()->getDisableParallax())
		//{
		//	uniform->set(0.0f);
		//}
		//else
		//{
		//	uniform->set(0.05f);
		//}

		//uniform = lighting_stateset->getUniform("specularParam");
		//if(crBrain::getInstance()->getDisableNormalMapping())
		//{	
		//	uniform->set(0.0f);
		//}
		//else
		//{
		//	uniform->set(1.0f);
		//}

		//uniform = lighting_stateset->getUniform("shadowBias");
		//if(crBrain::getInstance()->getDisableVSM())
		//{	
		//	uniform->set(0.0005f);
		//}
		//else
		//{
		//	uniform->set(0.00025f);
		//}
	
		//uniform = lighting_stateset->getUniform("softShadow");
		//if(crBrain::getInstance()->getDisableSoftShadow())
		//{	
		//	uniform->set(false);
		//}
		//else
		//{
		//	uniform->set(true);
		//}

		//uniform = lighting_stateset->getUniform("fog");
		//if(crBrain::getInstance()->getDisableFog())
		//{	
		//	uniform->set(false);
		//}
		//else
		//{
		//	uniform->set(true);
		//}
		///////临时 end
	}
}

void crRenderToTextureNode::swapBuffers()
{
    m_active_ls_cameraMap_buf.swap(m_active_ls_cameraMap);
}

void crRenderToTextureNode::traverse(crNodeVisitor& nv)
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
			
			//lighting_camera->setProjectionMatrix(m_currentProjectMatrix);
			//lighting_camera->setViewMatrix(m_currentViewMatrix);
			//shadow_camera->setProjectionMatrix(m_currentProjectMatrix);
			//shadow_camera->setViewMatrix(m_currentViewMatrix);

			light = dynamic_cast<const crLight *>(light_source->getLight());
			const CRCore::crVector4& lightPosition = light->getPosition();
			const crMatrix &lightMat = light->getLightMatrix();  
			lightPos.set(lightPosition[0],lightPosition[1],lightPosition[2]);
			lightPos = lightPos * lightMat;
			
			lightRenderMode = None;

            if(light_source->getLightIsSunLight())
			{				 
				//LightMVPS lightmvps;

				lightRenderMode = SunLight;
                //计算shadow_camera的视锥
				maxlight = eye + lv * c_sunViewRangeForBig;
                lightDir1 = eye - lightPos;
				lightDir2 = maxlight - lightPos;
				lightDir1.normalize();
				lightDir2.normalize();
                lightFov = acos(lightDir1 * lightDir2);
                lightDir = (lightDir1 + lightDir2) * 0.5f;
				
				shadow_camera->setProjectionMatrixAsPerspective(RadiansToDegrees(lightFov),1.0,1.0f,SUNLIGHT_RANGE);
				shadow_camera->setViewMatrixAsLookAt(lightPos,lightPos+lightDir,Y_AXIS);

				activeCameraParams.m_lightMVP = *shadow_camera->getViewMatrix() * *shadow_camera->getProjectionMatrix();

				maxlight = eye + lv * c_sunViewRangeForSmall;
				lightDir2 = maxlight - lightPos;
				lightDir2.normalize();
				lightFov = acos(lightDir1 * lightDir2);
				lightDir = (lightDir1 + lightDir2) * 0.5f;

				lighting_camera->setProjectionMatrixAsPerspective(RadiansToDegrees(lightFov),1.0,1.0f,SUNLIGHT_RANGE);
				lighting_camera->setViewMatrixAsLookAt(lightPos,lightPos+lightDir,Y_AXIS);

				activeCameraParams.m_detailLightMVP =  *lighting_camera->getViewMatrix() * *lighting_camera->getProjectionMatrix();

				activeCameraParams.m_lightPos = lightPos;//.set(lightPos[0],lightPos[1],lightPos[2],SUNLIGHTSCALE);
			}
			else
			{//室内场景
				lighting_camera->setProjectionMatrix(m_currentProjectMatrix);
				lighting_camera->setViewMatrix(m_currentViewMatrix);

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
                            lightRenderMode = SpotLight;

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
							shadow_camera->setProjectionMatrixAsPerspective(2.0f*cutoff,1.0f,1.0f,SPOTLIGHT_RANGE);
							shadow_camera->setViewMatrixAsLookAt(lightPos,lightPos+spotLightDir,Y_AXIS);
							activeCameraParams.m_lightMVP = *shadow_camera->getViewMatrix() * *shadow_camera->getProjectionMatrix();

							lighting_camera->clearExternFrustum();
							light_frustum.clear();
							light_frustum.setToUnitFrustum(true,true);
							light_frustum.transformProvidingInverse( activeCameraParams.m_lightMVP );
							lighting_camera->addExternFrustum(light_frustum);

							activeCameraParams.m_lightPos = lightPos;//.set(lightPos[0],lightPos[1],lightPos[2],SPOTLIGHTSCALE);
						}
					}
					break;
				}
			}

			shadow_camera->setShallRender(lightRenderMode!=None);

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

		if(!m_indoor) nv.setShadowMappingNodeMainTraverse(true);
		for(NodeArray::iterator nitr=m_childArray.begin();
			nitr!=m_childArray.end();
			++nitr)
		{
			if(typeid(*(nitr->get()))==typeid(CRCore::crCameraNode)) continue;
			(*nitr)->accept(nv);
		}
		if(!m_indoor) nv.setShadowMappingNodeMainTraverse(false);

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
			if(typeid(*(itr->get()))==typeid(CRCore::crCameraNode)) continue;
			(*itr)->accept(nv);
		}
	}
}

void crRenderToTextureNode::releaseObjects(crState* state) const
{
	crGroup::releaseObjects(state);
}
