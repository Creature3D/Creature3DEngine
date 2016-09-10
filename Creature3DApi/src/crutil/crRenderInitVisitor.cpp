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
#include <CRUtil/crRenderInitVisitor.h>
#include <CRUtil/crAttribDrawCallback.h>
#include <CRCore/crClusterCullingCallback.h>
#include <CRCore/crNodeCallback.h>
#include <CRCore/crBrain.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crShapeDrawable.h>
#include <CRCore/crBoundingBox.h>
#include <CRCore/crPrimitive.h>
#include <CRCore/crNotify.h>
#include <CRCore/crTexGen.h>
#include <CRCore/crTexEnv.h>
#include <CRCore/crSearchNodeBYNameVisitor.h>
#include <CRCore/crShaderManager.h>
#include <CRCore/crEnvironmentManager.h>
//#include <CRUtil/crCullVisitor.h>

#include <CRCore/crTexture.h>
#include <CRCore/crTexture1D.h>
#include <CRCore/crTexture2D.h>
#include <CRCore/crTexture3D.h>
#include <CRCore/crTextureCubeMap.h>
#include <CRCore/crTextureRectangle.h>

using namespace CRCore;
using namespace CRUtil;

crRenderInitVisitor::crRenderInitVisitor(Mode mode)
    :crNodeVisitor(INIT_VISITOR,TRAVERSE_ALL_CHILDREN)
{
    m_mode = mode;
    m_state = NULL;
}

void crRenderInitVisitor::apply(CRCore::crDB& node)
{
	if(!node.getRenderable() || node.getRenderInited()) return;
	traverse(node);
	node.setRenderInited(true);
}

void crRenderInitVisitor::apply(CRCore::crNode& node)
{
	if(!node.getRenderable() || node.getRenderInited()) return;

	traverse(node);
	node.setRenderInited(true);
}

void crRenderInitVisitor::apply(CRCore::crLightSource& node)
{
	if(!node.getRenderable()) return;
	traverse(node);
}

//#define AttachTextureRenderFunc(func) \
//	if(!texture->getDrawCallback()) \
//	    texture->setDrawCallback(func::getInstance());	

void crRenderInitVisitor::apply(CRCore::crCameraNode& node)
{
	if(!node.getRenderable() || node.getRenderInited()) return;

	if(node.isRenderToTextureCamera())
	{
		crCameraNode::BufferAttachmentMap &bufferAttachmentMap = node.getBufferAttachmentMap();
		crViewPort *viewport = node.getViewport();
		for(CRCore::crCameraNode::BufferAttachmentMap::iterator itr = bufferAttachmentMap.begin();
			itr != bufferAttachmentMap.end();
			++itr)
		{
			if (itr->second.m_texture.valid())
			{
				CRCore::crTexture* texture = itr->second.m_texture.get();
				CRCore::crTexture1D* texture1D = 0;
				CRCore::crTexture2D* texture2D = 0;
				CRCore::crTexture3D* texture3D = 0;
				CRCore::crTextureCubeMap* textureCubeMap = 0;
				CRCore::crTextureRectangle* textureRectangle = 0;
				if (0 != (texture1D=dynamic_cast<CRCore::crTexture1D*>(texture)))
				{
					if (texture1D->getTextureWidth()==0)
					{
						texture1D->setTextureWidth(viewport->width());
					}
				}
				else if (0 != (texture2D = dynamic_cast<CRCore::crTexture2D*>(texture)))
				{
					if (texture2D->getTextureWidth()==0 || texture2D->getTextureHeight()==0)
					{
						texture2D->setTextureSize(viewport->width(),viewport->height());
					}
				}
				else if (0 != (texture3D = dynamic_cast<CRCore::crTexture3D*>(texture)))
				{
					if (texture3D->getTextureWidth()==0 || texture3D->getTextureHeight()==0 || texture3D->getTextureDepth()==0 )
					{
						// note we dont' have the depth here, so we'll heave to assume that height and depth are the same..
						texture3D->setTextureSize(viewport->width(),viewport->height(),viewport->height());
					}
				}
				else if (0 != (textureCubeMap = dynamic_cast<CRCore::crTextureCubeMap*>(texture)))
				{
					if (textureCubeMap->getTextureWidth()==0 || textureCubeMap->getTextureHeight()==0)
					{
						textureCubeMap->setTextureSize(viewport->width(),viewport->height());
					}
				}
				else if (0 != (textureRectangle = dynamic_cast<CRCore::crTextureRectangle*>(texture)))
				{
					if (textureRectangle->getTextureWidth()==0 || textureRectangle->getTextureHeight()==0)
					{
						textureRectangle->setTextureSize(viewport->width(),viewport->height());
					}
				}

				//if(m_mode&COMPILE_STATE_ATTRIBUTES && m_state.valid())
				//	texture->apply(*m_state);
			}
		}

	}

	traverse(node);

	node.setRenderInited(true);
}
void crRenderInitVisitor::apply(CRCore::crCanvasNode& node)
{
	Mode mode = m_mode;
	setMode(NONE);
	traverse(node);
	setMode(mode);
}
void crRenderInitVisitor::apply(CRCore::crWidgetNode& node)
{
	Mode mode = m_mode;
	setMode(NONE);
	traverse(node);
	setMode(mode);
}
void crRenderInitVisitor::apply(CRCore::crObject& node)
{
	if(!node.getRenderable() || node.getRenderInited()) return;

	int needClusterCulling = 0;
	CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"NeedClusterCulling",needClusterCulling);
	int effectRender = 0;
	CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"EffectRender",effectRender);
	int modelBlendRender = 0;
	CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"ModelBlendRender",modelBlendRender);
	//if(attribDrawCallback) node.getOrCreateStateSet()->setRenderBinDetails(10,"StateSortedBin");
	for(unsigned int i=0;i<node.getNumDrawables();++i)
	{
		crDrawable* drawable = node.getDrawable(i);
		if (drawable)
		{
			apply(*drawable,needClusterCulling);
			if(effectRender || modelBlendRender) 
			{
				CRCore::crDrawable::DrawCallback *drawCallback;
				if(drawable->getType() == crDrawable::PUPPETGEOMETRY)
				{
					crGeometry *geometry = drawable->getPuppetGeometry();
					if(geometry)
					{
						drawCallback = geometry->getDrawCallback();
						if(effectRender && !dynamic_cast<crAttribDrawCallback *>(drawCallback))
						{
							geometry->setDrawCallback(new crAttribDrawCallback);
						}
						else if(modelBlendRender && !dynamic_cast<crModelBlendDrawCallback *>(drawCallback))
						{
							geometry->setDrawCallback(new crModelBlendDrawCallback);
						}
					}
					geometry = drawable->getPuppetBufGeometry();
					if(geometry)
					{
						drawCallback = geometry->getDrawCallback();
						if(effectRender && !dynamic_cast<crAttribDrawCallback *>(drawCallback))
						{
							geometry->setDrawCallback(new crAttribDrawCallback);
						}
						else if(modelBlendRender && !dynamic_cast<crModelBlendDrawCallback *>(drawCallback))
						{
							geometry->setDrawCallback(new crModelBlendDrawCallback);
						}
					}
				}
				else
				{
					drawCallback = drawable->getDrawCallback();
					if(effectRender && !dynamic_cast<crAttribDrawCallback *>(drawCallback))
					{
						drawable->setDrawCallback(new crAttribDrawCallback);
					}
					else if(modelBlendRender && !dynamic_cast<crModelBlendDrawCallback *>(drawCallback))
					{
						drawable->setDrawCallback(new crModelBlendDrawCallback);
					}
				}
			}
		}
	}

	const std::string& name = node.getName();
	if(!name.empty())
	{
		crBrain *brain = crBrain::getInstance();
		crBrain::ObjectLight *objLight = brain->findObjectLight(name);
		if(objLight)
		{
			crBrain::ObjectLight::LightNameSet::iterator itr;
			for( itr = objLight->m_avoidLight.begin();
				itr != objLight->m_avoidLight.end();
				++itr )
			{
				node.addAvoidLight(*itr);
			}
			for( itr = objLight->m_needLight.begin();
				itr != objLight->m_needLight.end();
				++itr )
			{
				node.addNeedLight(*itr);
			}
		}
	}
    node.setRenderInited(true);
}

void crRenderInitVisitor::apply(CRCore::crDrawable& drawable,bool needClusterCulling)
{
    CRParticle::crParticleSystem* ps = dynamic_cast<CRParticle::crParticleSystem *>(&drawable);
	if(ps)
	{
		CRCore::crObject *obj = ps->getParticleModelObject();
		if(obj)	apply(*obj);
	    return;
	}
	//crGeometry::PrimitiveList::iterator iter;
	//crGeometry *geometry = NULL;
	//crShapeDrawable *shapeDrawable = NULL;
	switch(drawable.getType()) 
	{
	//case crDrawable::SHAPEDRAWABLE:
	//	shapeDrawable = dynamic_cast<crShapeDrawable *>(&drawable);
	//	if(!shapeDrawable->getDrawCallback())
	//	{
	//		shapeDrawable->setDrawCallback( crOglShapeDrawableDraw::getInstance() );
	//	}
	//	break;
	case crDrawable::GEOMETRY:
		{
			//geometry = drawable.asGeometry();
			//if(!geometry->getDrawCallback())
			//{
			//	geometry->setDrawCallback( crOglGeometryDraw::getInstance() );
			//}
			//crGeometry::PrimitiveList &primitiveList = geometry->getPrimitiveList();
			//for(iter=primitiveList.begin(); iter!=primitiveList.end(); iter++)
			//{
			//	if((*iter)->getDrawCallback()) continue;
			//	switch((*iter)->getType())
			//	{
			//	case crPrimitive::DrawArraysPrimitiveType:
			//		(*iter)->setDrawCallback( crOglDrawArrays::getInstance() );
			//		break;
			//	case crPrimitive::DrawArrayLengthsPrimitiveType:
			//		(*iter)->setDrawCallback( crOglDrawArrayLengths::getInstance() );
			//		break;
			//	case crPrimitive::DrawElementsUBytePrimitiveType:
			//		(*iter)->setDrawCallback( crOglDrawElementsUByte::getInstance() );
			//		break;
			//	case crPrimitive::DrawElementsUShortPrimitiveType:
			//		(*iter)->setDrawCallback( crOglDrawElementsUShort::getInstance() );
			//		break;
			//	case crPrimitive::DrawElementsUIntPrimitiveType:
			//		(*iter)->setDrawCallback( crOglDrawElementsUInt::getInstance() );
			//		break;
			//	case crPrimitive::PrimitiveType:
			//		break;
			//	}
			//}

			if(needClusterCulling)
			{
				if(!drawable.getCullCallback())
				    drawable.setCullCallback(new crClusterCullingCallback(&drawable));
			}
			else
                drawable.setCullCallback(NULL);
		}
		break;
	//case crDrawable::PUPPETGEOMETRY:
	//	{
	//		//geometry = drawable.getPuppetGeometry();
	//		//if(geometry)
	//		//{
	//		//	if(!geometry->getDrawCallback())
	//		//	{
	//		//		geometry->setDrawCallback( crOglGeometryDraw::getInstance() );
	//		//	}
	//		//	crGeometry::PrimitiveList &primitiveList = geometry->getPrimitiveList();
	//		//	for(iter=primitiveList.begin(); iter!=primitiveList.end(); iter++)
	//		//	{
	//		//		if((*iter)->getDrawCallback()) continue;
	//		//		switch((*iter)->getType())
	//		//		{
	//		//		case crPrimitive::DrawArraysPrimitiveType:
	//		//			(*iter)->setDrawCallback( crOglDrawArrays::getInstance() );
	//		//			break;
	//		//		case crPrimitive::DrawArrayLengthsPrimitiveType:
	//		//			(*iter)->setDrawCallback( crOglDrawArrayLengths::getInstance() );
	//		//			break;
	//		//		case crPrimitive::DrawElementsUBytePrimitiveType:
	//		//			(*iter)->setDrawCallback( crOglDrawElementsUByte::getInstance() );
	//		//			break;
	//		//		case crPrimitive::DrawElementsUShortPrimitiveType:
	//		//			(*iter)->setDrawCallback( crOglDrawElementsUShort::getInstance() );
	//		//			break;
	//		//		case crPrimitive::DrawElementsUIntPrimitiveType:
	//		//			(*iter)->setDrawCallback( crOglDrawElementsUInt::getInstance() );
	//		//			break;
	//		//		case crPrimitive::PrimitiveType:
	//		//			break;
	//		//		}
	//		//	}
	//		//}

	//		//geometry = drawable.getPuppetBufGeometry();
	//		//if(geometry)
	//		//{
	//		//	if(!geometry->getDrawCallback())
	//		//	{
	//		//		geometry->setDrawCallback( crOglGeometryDraw::getInstance() );
	//		//	}
	//		//	crGeometry::PrimitiveList &bufPrimitiveList = geometry->getPrimitiveList();
	//		//	for(iter=bufPrimitiveList.begin(); iter!=bufPrimitiveList.end(); iter++)
	//		//	{
	//		//		if((*iter)->getDrawCallback()) continue;
	//		//		switch((*iter)->getType())
	//		//		{
	//		//		case crPrimitive::DrawArraysPrimitiveType:
	//		//			(*iter)->setDrawCallback( crOglDrawArrays::getInstance() );
	//		//			break;
	//		//		case crPrimitive::DrawArrayLengthsPrimitiveType:
	//		//			(*iter)->setDrawCallback( crOglDrawArrayLengths::getInstance() );
	//		//			break;
	//		//		case crPrimitive::DrawElementsUBytePrimitiveType:
	//		//			(*iter)->setDrawCallback( crOglDrawElementsUByte::getInstance() );
	//		//			break;
	//		//		case crPrimitive::DrawElementsUShortPrimitiveType:
	//		//			(*iter)->setDrawCallback( crOglDrawElementsUShort::getInstance() );
	//		//			break;
	//		//		case crPrimitive::DrawElementsUIntPrimitiveType:
	//		//			(*iter)->setDrawCallback( crOglDrawElementsUInt::getInstance() );
	//		//			break;
	//		//		case crPrimitive::PrimitiveType:
	//		//			break;
	//		//		}
	//		//	}
	//		//}

	//		if(!drawable.getCullCallback())
	//			drawable.setCullCallback(new crClusterCullingCallback(&drawable));
	//	}
	//	break;
	//case crDrawable::TEXTDRAWABLE:
	//	return;
	}
	if(m_mode&COMPILE_DISPLAY_LISTS && m_state.valid())
	{
		//crStateSet *ss = drawable.getStateSet();
		//if(ss)
		//	ss->compile(*m_state);
		drawable.compile(*m_state);
	}
	//if (m_mode&SWITCH_OFF_DISPLAY_LISTS)
 //   {
 //       drawable.setUseDisplayList(false);
 //   }

 //   if (m_mode&SWITCH_ON_DISPLAY_LISTS)
 //   {
 //       drawable.setUseDisplayList(true);
 //   }

	//if (m_mode&SWITCH_ON_VERTEX_BUFFER_OBJECTS)
	//{
	//	drawable.setUseVertexBufferObjects(true);
	//}

	//if (m_mode&SWITCH_OFF_VERTEX_BUFFER_OBJECTS)
	//{
	//	drawable.setUseVertexBufferObjects(false);
	//}

	//if (m_mode&SWITCH_ON_INDEX_BUFFER_OBJECTS)
	//{
	//	drawable.setUseIndexBufferObjects(true);
	//}

	//if (m_mode&SWITCH_OFF_INDEX_BUFFER_OBJECTS)
	//{
	//	drawable.setUseIndexBufferObjects(false);
	//}

 //   //if (m_mode&COMPILE_DISPLAY_LISTS && m_state.valid())
 //   //{
 //   //    drawable.compile(*m_state);
 //   //}

 //   if (m_mode&RELEASE_DISPLAY_LISTS)
 //   {
 //       drawable.releaseObjects(m_state.get());
 //   }
}

void crRenderInitVisitor::apply(CRParticle::crParticleSystem& ps,bool needClusterCulling)
{
	CRCore::crObject *obj = ps.getParticleModelObject();
    if(obj)
		apply(*obj);
}

void crRenderInitVisitor::apply(CRCore::crBillboard& node)
{
	if(!node.getRenderable() || node.getRenderInited()) return;
	node.init();
	//if (node.getStateSet())
	//{
	//	apply(*(node.getStateSet()));
	//}
	crDrawable *drawable = node.getComposeDrawable();
	apply(*drawable);
	drawable->setCullCallback(NULL);
	int int1;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"EffectRender",int1)&&int1)
	{
		//node.getOrCreateStateSet()->setRenderBinDetails(10,"StateSortedBin");
		CRCore::crDrawable::DrawCallback *drawCallback = drawable->getDrawCallback();
		if(!dynamic_cast<crAttribDrawCallback *>(drawCallback))
		{
			drawable->setDrawCallback(new crAttribDrawCallback);
		}
	}
	else if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"BillboardDrawCallback",int1)&&int1)
	{
		CRCore::crDrawable::DrawCallback *drawCallback = drawable->getDrawCallback();
		if(!dynamic_cast<crBillboardDrawCallback *>(drawCallback))
		{
			drawable->setDrawCallback(new crBillboardDrawCallback);
		}
	}
	std::string strbuf;
	if(crArgumentParser::readKeyValue(node.getDescriptions(),"AxisAligned",strbuf))
	{
		if(strbuf.compare("POINT_ROT_WORLD_X_AXIS") == 0)
		{
			node.setMode(CRCore::crBillboard::POINT_ROT_EYE);
			node.setAxis(crVector3(1.0f,0.0,0.0f));
			node.setNormal(crVector3(0.0f,-1.0,0.0f));
		}
		else if(strbuf.compare("POINT_ROT_WORLD_Y_AXIS") == 0)
		{
			node.setMode(CRCore::crBillboard::POINT_ROT_EYE);
			node.setAxis(crVector3(0.0f,1.0,0.0f));
			node.setNormal(crVector3(1.0f, 0.0,0.0f));
		}
		else if(strbuf.compare("POINT_ROT_WORLD_Z_AXIS") == 0)
		{
			node.setMode(CRCore::crBillboard::POINT_ROT_WORLD);
			node.setAxis(crVector3(0.0f, 0.0, 1.0f));
			node.setNormal(crVector3(0.0f, -1.0f, 0.0f));
		}
		else if(strbuf.compare("AXIAL_ROT_X_AXIS") == 0)
		{
			node.setMode(CRCore::crBillboard::AXIAL_ROT);
			node.setAxis(crVector3(1.0f,0.0,0.0f));
			node.setNormal(crVector3(0.0f,-1.0,0.0f));
		}
		else if(strbuf.compare("AXIAL_ROT_Y_AXIS") == 0)
		{
			node.setMode(CRCore::crBillboard::AXIAL_ROT);
			node.setAxis(crVector3(0.0f,1.0,0.0f));
			node.setNormal(crVector3(1.0f, 0.0,0.0f));
		}
		else if(strbuf.compare("AXIAL_ROT_Z_AXIS") == 0)
		{
			node.setMode(CRCore::crBillboard::AXIAL_ROT);
			node.setAxis(crVector3(0.0f,0.0,1.0f));
			node.setNormal(crVector3(0.0f,-1.0,0.0f));
		}
	}

	//crGeometry *geometry = node.getComposeDrawable()->asGeometry();
	//if(geometry&&!geometry->getDrawCallback())
	//{
	//	geometry->setCullCallback(NULL);

	//	geometry->setDrawCallback( crOglGeometryDraw::getInstance() );
	//	geometry->setUseDisplayList(false);
	//	geometry->setUseVertexBufferObjects(false);
	//}
	node.setRenderInited(true);
}

//void crRenderInitVisitor::apply(CRCore::crStateSet& stateset)
//{
//	if(m_state.valid())
//		stateset.compile(*m_state);
//	//if(!stateset.getRenderInited())
//	//{
//	//	if(!stateset.getCompileCallback())
//	//		stateset.setComileCallback(crAttachRenderFuncCallback::getInstance());
//	//	if(m_mode&COMPILE_STATE_ATTRIBUTES && m_state.valid())
//	//	{
//	//		stateset.compile(*m_state);
//	//	}
//	//	if (m_mode&RELEASE_STATE_ATTRIBUTES)
//	//	{
//	//		stateset.releaseObjects(m_state.get());
//	//	}
//	//	if (m_mode & CHECK_BLACK_LISTED_MODES)
//	//	{
//	//		stateset.checkValidityOfAssociatedModes(*m_state.get());
//	//	}
//	//	stateset.setRenderInited(true);
//	//}
//}
//
//void crRenderInitVisitor::apply(CRCore::crClipNode& node)
//{
//	if(!node.getRenderable() || node.getRenderInited()) return;
//	
//	for( CRCore::crClipNode::ClipPlaneList::iterator itr = node.getClipPlaneList().begin();
//		itr != node.getClipPlaneList().end();
//		++itr )
//	{
//		if(!(*itr)->getDrawCallback())
//			(*itr)->setDrawCallback(crOglClipPlaneDraw::getInstance());	
//	}
//
//	traverse(node);
//	node.setRenderInited(true);
//}

void crRenderInitVisitor::initExtern()
{//做一些扩展的初始化
	//crShaderManager::ShaderStateSetMap& statesetmap = crShaderManager::getInstance()->getShaderStateSetMap();
	//for( crShaderManager::ShaderStateSetMap::iterator shaderItr = statesetmap.begin();
	//	 shaderItr != statesetmap.end();
	//	 ++shaderItr )
	//{
 //       apply(*(shaderItr->second.get()));
	//}

	//crEnvironmentManager::EnvironmentMap &envMap = crEnvironmentManager::getInstance()->getEnvMap();
	//for( crEnvironmentManager::EnvironmentMap::iterator envItr = envMap.begin();
	//	 envItr != envMap.end();
	//	 ++envItr )
	//{
	//	if(!envItr->second->getDrawCallback())
 //           envItr->second->setDrawCallback(crOglTextureCubeMapDraw::getInstance());
	//}
}