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
//Modified by Wucaihua
#include <stdio.h>

#include <CRCore/crNotify.h>
#include <CRCore/crTexture1D.h>
#include <CRCore/crTexture2D.h>
#include <CRCore/crTexture2DMultisample.h>
#include <CRCore/crTexture3D.h>
#include <CRCore/crTextureRectangle.h>
#include <CRCore/crTextureCubeMap.h>
#include <CRCore/crDisplaySettings.h>
#include <CRCore/crStreamBuf.h>

#include <crgl/GLExtensions.h>
#include <crgl/GLU.h>

#include <CRCore/crStatistics.h>

#include <CRUtil/crRenderStage.h>


using namespace CRCore;
using namespace CRUtil;

// register a crRenderStage prototype with the crRenderBin prototype list.
//RegisterRenderBinProxy<crRenderStage> s_registerRenderStageProxy;

crRenderStage::crRenderStage():
    crRenderBin(getDefaultRenderBinSortMode()),
    m_disableFboAfterRender(true)
{
    // point crRenderBin's m_stage to this to ensure that references to
    // stage don't go tempted away to any other stage.
    m_stage = this;
    m_stageDrawnThisFrame = false;

    m_drawBuffer = GL_NONE;
    m_readBuffer = GL_NONE;

	m_clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT/* | GL_STENCIL_BUFFER_BIT*/;

    m_clearColor.set(0.0f,0.0f,0.0f,1.0f);
    m_clearAccum.set(0.0f,0.0f,0.0f,1.0f);
    m_clearDepth = 1.0;
    m_clearStencil = 0;

    m_cameraRequiresSetUp = false;
    m_camera = 0;
    
    m_level = 0;
    m_face = 0;
    
    m_imageReadPixelFormat = GL_RGBA;
    m_imageReadPixelDataType = GL_UNSIGNED_BYTE;
}

crRenderStage::crRenderStage(SortMode mode):
    crRenderBin(mode),
    m_disableFboAfterRender(true)
{
    // point crRenderBin's m_stage to this to ensure that references to
    // stage don't go tempted away to any other stage.
    m_stage = this;
    m_stageDrawnThisFrame = false;

    m_drawBuffer = GL_NONE;
    m_readBuffer = GL_NONE;

	m_clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

    m_clearColor.set(0.0f,0.0f,0.0f,0.0f);
    m_clearAccum.set(0.0f,0.0f,0.0f,0.0f);
    m_clearDepth = 1.0;
    m_clearStencil = 0;

    m_cameraRequiresSetUp = false;
    m_camera = 0;
    
    m_level = 0;
    m_face = 0;
    
    m_imageReadPixelFormat = GL_RGBA;
    m_imageReadPixelDataType = GL_UNSIGNED_BYTE;
}

crRenderStage::crRenderStage(const crRenderStage& rhs,const CRCore::crCopyOp& copyop):
        crRenderBin(rhs,copyop),
        m_stageDrawnThisFrame(false),
        m_preRenderList(rhs.m_preRenderList),
        m_postRenderList(rhs.m_postRenderList),
        m_viewport(rhs.m_viewport),
        m_drawBuffer(rhs.m_drawBuffer),
        m_readBuffer(rhs.m_readBuffer),
        m_clearMask(rhs.m_clearMask),
        m_colorMask(rhs.m_colorMask),
        m_clearColor(rhs.m_clearColor),
        m_clearAccum(rhs.m_clearAccum),
        m_clearDepth(rhs.m_clearDepth),
        m_clearStencil(rhs.m_clearStencil),
        m_cameraRequiresSetUp(rhs.m_cameraRequiresSetUp),
        m_camera(rhs.m_camera),
        m_level(rhs.m_level),
        m_face(rhs.m_face),
        m_imageReadPixelFormat(rhs.m_imageReadPixelFormat),
        m_imageReadPixelDataType(rhs.m_imageReadPixelDataType),
        m_renderStageLighting(rhs.m_renderStageLighting),
        m_disableFboAfterRender(rhs.m_disableFboAfterRender)
{
    m_stage = this;
}


crRenderStage::~crRenderStage()
{
}

void crRenderStage::reset()
{
    m_stageDrawnThisFrame = false;
    
    if (m_renderStageLighting.valid()) m_renderStageLighting->reset();

    for(RenderStageList::iterator pre_itr = m_preRenderList.begin();
        pre_itr != m_preRenderList.end();
        ++pre_itr)
    {
        (*pre_itr)->reset();
    }

    crRenderBin::reset();

    for(RenderStageList::iterator post_itr = m_postRenderList.begin();
        post_itr != m_postRenderList.end();
        ++post_itr)
    {
        (*post_itr)->reset();
    }

    //m_preRenderList.clear();
    //m_postRenderList.clear();
	m_preRenderList.resize(0);
	m_postRenderList.resize(0);
}

void crRenderStage::sort()
{
    for(RenderStageList::iterator pre_itr = m_preRenderList.begin();
        pre_itr != m_preRenderList.end();
        ++pre_itr)
    {
        (*pre_itr)->sort();
    }

    crRenderBin::sort();

    for(RenderStageList::iterator post_itr = m_postRenderList.begin();
        post_itr != m_postRenderList.end();
        ++post_itr)
    {
        (*post_itr)->sort();
    }
}

void crRenderStage::drawZprePass(CRCore::crState& state,crRenderLeaf*& previous)
{
	if (!m_viewport)
	{
		notify(FATAL) << "Error: cannot draw stage due to undefined viewport."<< std::endl;
		return;
	}
	// set up the back buffer.
	state.applyAttribute(m_viewport.get());

	if(crDisplaySettings::instance()->useHardWareAA())
	{
		glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
		glClearDepth( m_clearDepth);
		return;
	}

#define USE_SISSOR_TEST
#ifdef USE_SISSOR_TEST
	glScissor( static_cast<int>(m_viewport->x()),
		static_cast<int>(m_viewport->y()),
		static_cast<int>(m_viewport->width()),
		static_cast<int>(m_viewport->height()) );
	//cout << "    clearing "<<this<< "  "<<m_viewport->x()<<","<< m_viewport->y()<<","<< m_viewport->width()<<","<< m_viewport->height()<<std::endl;

	glEnable( GL_SCISSOR_TEST );
#endif

	glClearColor( m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
	glClearDepth( m_clearDepth);
	glClearStencil( m_clearStencil);
	glClearAccum( m_clearAccum[0], m_clearAccum[1], m_clearAccum[2], m_clearAccum[3]);

	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	//state.applyColorMask(crVector4ub(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE));

#ifdef USE_SISSOR_TEST
	glDisable( GL_SCISSOR_TEST );
#endif

	//glEnable(GL_POLYGON_OFFSET_FILL);
	//glPolygonOffset(0.0f,1.0f);
	glDisable( GL_DEPTH_TEST );
	glDepthFunc(GL_LESS);
	glDepthRange(0.0,1.0);
	glDepthMask(GL_TRUE);

	state.applyCullFace(GL_BACK);
	//glDisable( GL_MULTISAMPLE_ARB );
	//glDisable( GL_STENCIL_TEST );
	//glDisable( GL_NORMALIZE );
	glDisable( GL_ALPHA_TEST );
	glDisable( GL_BLEND );
    //glDisable( GL_POLYGON_OFFSET_FILL );
    //glDisable( GL_TEXTURE_GEN_S );
    //glDisable( GL_TEXTURE_GEN_T );
    //glDisable( GL_TEXTURE_GEN_R );
    //glDisable( GL_TEXTURE_GEN_Q );
	//glDisable( GL_LIGHTING );
	//for(int i = 0; i<8; ++i)
	//{
 //       glDisable( GL_CLIP_PLANE0+i );
	//}
	//for(int i = 0; i<6; ++i)
	//{
	//	glDisable( GL_LIGHT0+i );
	//}
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	//glMatrixMode( GL_TEXTURE );
	//glLoadIdentity();
	//glMatrixMode( GL_MODELVIEW );

	//// apply the positional state.
	//if (m_inheritedPositionalStateContainer.valid())
	//{
	//	m_inheritedPositionalStateContainer->draw(state, previous, &m_inheritedPositionalStateContainerMatrix);
	//}

	//// apply the positional state.
	//if (m_renderStageLighting.valid())
	//{
	//	m_renderStageLighting->draw(state, previous, 0);
	//}

	state.setIgnoreColor(true);
	for(RenderStageList::iterator pre_itr = m_preRenderList.begin();
		pre_itr != m_preRenderList.end();
		++pre_itr)
	{
		(*pre_itr)->drawZprePassImplementation(state,previous);
	}

	// draw the children and local.
	crRenderBin::drawZprePassImplementation(state,previous);

	for(RenderStageList::iterator post_itr = m_postRenderList.begin();
		post_itr != m_postRenderList.end();
		++post_itr)
	{
		(*post_itr)->drawZprePassImplementation(state,previous);
	}

	////state.apply();
	if (previous)
	{
		crRenderGraph::moveToRootRenderGraph(state,previous->m_parent);
		state.apply();
		previous = NULL;
	}
	//glDisable(GL_POLYGON_OFFSET_FILL);
#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crRenderStage::drawZprePass OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}

void crRenderStage::setCameraNode(CRCore::crCameraNode* camera) 
{
	if (m_camera!=camera) 
	{ 
		m_camera = camera; 
		m_cameraRequiresSetUp = true; 
	}
}

void crRenderStage::addPreRenderStage(crRenderStage* rs, int order)
{
    //if (rs)
    //{
    //    RenderStageList::iterator itr;
    //    for(itr = m_preRenderList.begin(); itr != m_preRenderList.end(); ++itr) {
    //        if(order < itr->first) {
    //            break;
    //        }
    //    }
    //    if(itr == m_preRenderList.end()) {
    //        m_preRenderList.push_back(RenderStageOrderPair(order,rs));
    //    } else {
    //        m_preRenderList.insert(itr,RenderStageOrderPair(order,rs));
    //    }
    //}
	if (rs) m_preRenderList.push_back(rs);
}

void crRenderStage::addPostRenderStage(crRenderStage* rs, int order)
{
    //if (rs)
    //{
    //    RenderStageList::iterator itr;
    //    for(itr = m_postRenderList.begin(); itr != m_postRenderList.end(); ++itr) {
    //        if(order < itr->first) {
    //            break;
    //        }
    //    }
    //    if(itr == m_postRenderList.end()) {
    //        m_postRenderList.push_back(RenderStageOrderPair(order,rs));
    //    } else {
    //        m_postRenderList.insert(itr,RenderStageOrderPair(order,rs));
    //    }
    //}
	if (rs) m_postRenderList.push_back(rs);
}

void crRenderStage::drawPreRenderStages(CRCore::crState& state,crRenderLeaf*& previous)
{
    //if (m_preRenderList.empty()) return;
    //
    ////cout << "Drawing prerendering stages "<<this<< "  "<<m_viewport->x()<<","<< m_viewport->y()<<","<< m_viewport->width()<<","<< m_viewport->height()<<std::endl;
    //for(RenderStageList::iterator itr=m_preRenderList.begin();
    //    itr!=m_preRenderList.end();
    //    ++itr)
    //{
    //    itr->second->draw(renderInfo,previous);
    //}
    ////cout << "Done Drawing prerendering stages "<<this<< "  "<<m_viewport->x()<<","<< m_viewport->y()<<","<< m_viewport->width()<<","<< m_viewport->height()<<std::endl;

	if (m_preRenderList.empty()) return;

	//cout << "Drawing prerendering stages "<<this<< "  "<<m_viewport->x()<<","<< m_viewport->y()<<","<< m_viewport->width()<<","<< m_viewport->height()<<std::endl;
	for(RenderStageList::iterator itr=m_preRenderList.begin();
		itr!=m_preRenderList.end();
		++itr)
	{
		(*itr)->draw(state,previous);
	}
}


void crRenderStage::runCameraSetUp(CRCore::crState& state)
{
    if (!m_camera)
	{
		m_cameraRequiresSetUp = false;
		return;
	}
    
    CRCore::crCameraNode::RenderTargetImplementation renderTargetImplementation = m_camera->getRenderTargetImplementation();
    CRCore::crCameraNode::RenderTargetImplementation renderTargetFallback = m_camera->getRenderTargetFallback();

    CRCore::crCameraNode::BufferAttachmentMap& bufferAttachments = m_camera->getBufferAttachmentMap();

    // compute the required dimensions
    int width = m_viewport->width();//static_cast<int>(m_viewport->x() + m_viewport->width());
    int height = m_viewport->height();//static_cast<int>(m_viewport->y() + m_viewport->height());
    int depth = 1;
    CRCore::crCameraNode::BufferAttachmentMap::iterator itr;
    //for(itr = bufferAttachments.begin();
    //    itr != bufferAttachments.end();
    //    ++itr)
    //{
    //    width = CRCore::maximum(width,itr->second.width());
    //    height = CRCore::maximum(height,itr->second.height());
    //    depth = CRCore::maximum(depth,itr->second.depth());
    //}
    
     //CRCore::notify(CRCore::NOTICE)<<"crRenderStage::runCameraSetUp viewport "<<m_viewport->x()<<" "<<m_viewport->y()<<" "<<m_viewport->width()<<" "<<m_viewport->height()<<std::endl;
     //CRCore::notify(CRCore::NOTICE)<<"crRenderStage::runCameraSetUp computed "<<width<<" "<<height<<" "<<depth<<std::endl;
	//CRCore::notify(CRCore::NOTICE)<<"crRenderStage::runCameraSetUp "<<m_camera->getName()<<std::endl;

    // attach images that need to be copied after the stage is drawn.
	CRCore::crImage* image;
	GLenum pixelFormat;
	GLenum dataType;
    for(itr = bufferAttachments.begin();
        itr != bufferAttachments.end();
        ++itr)
    {
        // if one exist attach image to the crRenderStage.
        if (itr->second.m_image.valid())
        {
            image = itr->second.m_image.get();
            pixelFormat = image->getPixelFormat();
            dataType = image->getDataType();

            if (image->data()==0)
            {
                if (pixelFormat==0) pixelFormat = itr->second.m_internalFormat;
                if (pixelFormat==0) pixelFormat = m_imageReadPixelFormat;
                if (pixelFormat==0) pixelFormat = GL_RGBA;

                if (dataType==0) dataType = m_imageReadPixelDataType;
                if (dataType==0) dataType = GL_UNSIGNED_BYTE;
            }

            m_bufferAttachmentMap[itr->first].m_imageReadPixelFormat = pixelFormat;
            m_bufferAttachmentMap[itr->first].m_imageReadPixelDataType = dataType;
            m_bufferAttachmentMap[itr->first].m_image = image;
        }
        
        if (itr->second.m_texture.valid())
        {
            CRCore::crTexture* texture = itr->second.m_texture.get();
            CRCore::crTexture1D* texture1D = 0;
            CRCore::crTexture2D* texture2D = 0;
			CRCore::crTexture2DMultisample* texture2DMS = 0;
            CRCore::crTexture3D* texture3D = 0;
            CRCore::crTextureCubeMap* textureCubeMap = 0;
            CRCore::crTextureRectangle* textureRectangle = 0;
            if (0 != (texture1D=dynamic_cast<CRCore::crTexture1D*>(texture)))
            {
                if (texture1D->getTextureWidth()==0)
                {
                    texture1D->setTextureWidth(width);
                }
            }
            else if (0 != (texture2D = dynamic_cast<CRCore::crTexture2D*>(texture)))
            {
                if (texture2D->getTextureWidth()==0 || texture2D->getTextureHeight()==0)
                {
                    texture2D->setTextureSize(width,height);
                }
            }
			else if (0 != (texture2DMS = dynamic_cast<CRCore::crTexture2DMultisample*>(texture)))
			{
				if (texture2DMS->getTextureWidth()==0 || texture2DMS->getTextureHeight()==0)
				{
					texture2DMS->setTextureSize(width,height);
				}
			}
            else if (0 != (texture3D = dynamic_cast<CRCore::crTexture3D*>(texture)))
            {
                if (texture3D->getTextureWidth()==0 || texture3D->getTextureHeight()==0 || texture3D->getTextureDepth()==0 )
                {
                    // note we dont' have the depth here, so we'll heave to assume that height and depth are the same..
                    texture3D->setTextureSize(width,height,height);
                }
            }
            else if (0 != (textureCubeMap = dynamic_cast<CRCore::crTextureCubeMap*>(texture)))
            {
                if (textureCubeMap->getTextureWidth()==0 || textureCubeMap->getTextureHeight()==0)
                {
                    textureCubeMap->setTextureSize(width,height);
                }
            }
            else if (0 != (textureRectangle = dynamic_cast<CRCore::crTextureRectangle*>(texture)))
            {
                if (textureRectangle->getTextureWidth()==0 || textureRectangle->getTextureHeight()==0)
                {
                    textureRectangle->setTextureSize(width,height);
                }
            }

        }
    }
    
	unsigned int contextID = state.getContextID();

    if (renderTargetImplementation==CRCore::crCameraNode::FRAME_BUFFER_OBJECT)
    {
        CRCore::crFBOExtensions* fbo_ext = CRCore::crFBOExtensions::instance(state.getContextID(),true);
        bool fbo_supported = fbo_ext && fbo_ext->isSupported();
        
        if (fbo_supported/* && !m_fbo*/)
        {
            //CRCore::notify(CRCore::INFO)<<"Setting up CRCore::crCameraNode::FRAME_BUFFER_OBJECT"<<std::endl;
			//CRUtil::crRenderStage* rtts_buf = dynamic_cast<CRUtil::crRenderStage*>(m_camera->getBufRenderingCache(contextID));
			//if(rtts_buf)
			//{
			//	//CRCore::notify(CRCore::WARN)<<"setFrameBufferObject0 = "<<rtts_buf->getFrameBufferObject()<<std::endl;
			//	setFrameBufferObject(rtts_buf->getFrameBufferObject());
			//}
			//if(!m_fbo)
			//{
			//	rtts_buf = dynamic_cast<CRUtil::crRenderStage*>(m_camera->getBufRenderingCache(contextID));
			//	if(rtts_buf)
			//	{
			//		//CRCore::notify(CRCore::WARN)<<"setFrameBufferObject1 = "<<rtts_buf->getFrameBufferObject()<<std::endl;
			//		setFrameBufferObject(rtts_buf->getFrameBufferObject());
			//	}
			//}
			char gbuf[256];
			sprintf(gbuf,"New crFrameBufferObject camera:%s\n\0",m_camera->getName().c_str());
			gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
			CRCore::ref_ptr<CRCore::crFrameBufferObject> fbo = new CRCore::crFrameBufferObject;
			CRCore::ref_ptr<CRCore::crFrameBufferObject> fbo_multisample;
			//if(!fbo.valid())
			//{
			//	CRCore::ScopedLock<CRCore::crMutex> lock(*(m_camera->getDataChangeMutex()));
			//	CRUtil::crRenderStage* rtts = dynamic_cast<CRUtil::crRenderStage*>(m_camera->getRenderingCache(contextID));
			//	if(rtts)
			//	{
			//		fbo = rtts->getFrameBufferObject();
			//	}
			//	if(!fbo.valid())
			//	{
			//		fbo = new CRCore::crFrameBufferObject;
			//	}
			//	m_fbo = fbo;
			//}
				bool colorAttached = false;
				bool depthAttached = false;
				bool stencilAttached = false;
				unsigned samples = 0;
				unsigned colorSamples = 0;

				if (fbo_ext->isMultisampleSupported())
				{
					for(CRCore::crCameraNode::BufferAttachmentMap::iterator itr = bufferAttachments.begin();
						itr != bufferAttachments.end();
						++itr)
					{
						CRCore::crCameraNode::Attachment& attachment = itr->second;
						samples = maximum(samples, attachment.m_multisampleSamples);
						colorSamples = maximum(samples, attachment.m_multisampleColorSamples);
					}

					if (colorSamples > samples)
					{
						CRCore::notify(WARN) << "Multisample color samples must be less than or "
							"equal to samples. Setting color samples equal to samples." << std::endl;
						colorSamples = samples;
					}

					if (samples>1)
					{
						fbo_multisample = new CRCore::crFrameBufferObject;
					}
				}
	            
				CRCore::crCameraNode::BufferComponent buffer;
				for(CRCore::crCameraNode::BufferAttachmentMap::iterator itr = bufferAttachments.begin();
					itr != bufferAttachments.end();
					++itr)
				{

					buffer = itr->first;
					CRCore::crCameraNode::Attachment& attachment = itr->second;
	                
					if (attachment.m_texture.valid() || attachment.m_image.valid())
						fbo->setAttachment(buffer, CRCore::crFrameBufferAttachment(attachment));
					else
						fbo->setAttachment(buffer, CRCore::crFrameBufferAttachment(new CRCore::crRenderBuffer(width, height, attachment.m_internalFormat)));

					if (fbo_multisample.valid()/* && attachment.m_multisampleSamples > 1*/)
					{//注：fbo_multisample会导致第0个后面的color buffer的颜色被第0个color buffer覆盖。
						//mrt multisample除了colorbuffer0，其它都无法获得正确的颜色
						GLenum internalFormat = attachment.m_internalFormat;
						if (!internalFormat)
						{
							switch (buffer)
							{
							case crCameraNode::DEPTH_BUFFER:
								internalFormat = GL_DEPTH_COMPONENT24;
								break;
							case crCameraNode::STENCIL_BUFFER:
								internalFormat = GL_STENCIL_INDEX8_EXT;
								break;
							case crCameraNode::PACKED_DEPTH_STENCIL_BUFFER:
								internalFormat = GL_DEPTH_STENCIL_EXT;
								break;
								// all other buffers are color buffers
							default:
								// setup the internal format based on attached texture if such exists, otherwise just default format
								if (attachment.m_texture.valid())
									internalFormat = attachment.m_texture->getInternalFormat();
								else
									internalFormat = GL_RGBA;
								break;
							}
						}
						fbo_multisample->setAttachment(buffer,
							CRCore::crFrameBufferAttachment(new CRCore::crRenderBuffer(
							width, height, internalFormat,
							samples, colorSamples)));
					}
	                
					if (buffer==CRCore::crCameraNode::DEPTH_BUFFER) depthAttached = true;
					else if (buffer==CRCore::crCameraNode::STENCIL_BUFFER) stencilAttached = true;
					else if (buffer==CRCore::crCameraNode::PACKED_DEPTH_STENCIL_BUFFER)
					{
						depthAttached = true;
						stencilAttached = true;
					}
					else if (buffer>=CRCore::crCameraNode::COLOR_BUFFER) colorAttached = true;
				}

				if (!depthAttached)
				{
					// If doing MSFBO (and therefore need two FBOs, one for multisampled rendering and one for 
					// final resolve), then configure "fbo" as the resolve FBO, and When done 
					// configuring, swap it into "_resolveFbo" (see line 554). But, if not 
					// using MSFBO, then "fbo" is just the render fbo.
					// If using MSFBO, then resolveBuffersMask 
					// is the value set by the app for the resolve buffers. But if not using 
					// MSFBO, then resolveBuffersMask is the value set by the app for render 
					// buffers. In both cases, resolveBuffersMask is used to configure "fbo".
					fbo->setAttachment(CRCore::crCameraNode::DEPTH_BUFFER, CRCore::crFrameBufferAttachment(new CRCore::crRenderBuffer(width, height, GL_DEPTH_COMPONENT24)));
					depthAttached = true;
					if (fbo_multisample.valid())
					{
						fbo_multisample->setAttachment(CRCore::crCameraNode::DEPTH_BUFFER,
							CRCore::crFrameBufferAttachment(new CRCore::crRenderBuffer(width,
							height, GL_DEPTH_COMPONENT24, samples, colorSamples)));
					}
				}
				//if (!stencilAttached)
				//{
				//	fbo->setAttachment(CRCore::crCameraNode::STENCIL_BUFFER, CRCore::crFrameBufferAttachment(new CRCore::crRenderBuffer(width, height, GL_STENCIL_INDEX8_EXT)));
				//	stencilAttached = true;

				//	if (fbo_multisample.valid())
				//	{
				//		fbo_multisample->setAttachment(CRCore::crCameraNode::STENCIL_BUFFER,
				//			CRCore::crFrameBufferAttachment(new CRCore::crRenderBuffer(width,
				//			height, GL_STENCIL_INDEX8_EXT, samples, colorSamples)));
				//	}
				//}

				if (!colorAttached)
				{                
					fbo->setAttachment(CRCore::crCameraNode::COLOR_BUFFER, CRCore::crFrameBufferAttachment(new CRCore::crRenderBuffer(width, height, GL_RGB)));
					colorAttached = true;
					if (fbo_multisample.valid())
					{
						fbo_multisample->setAttachment(CRCore::crCameraNode::COLOR_BUFFER,
							CRCore::crFrameBufferAttachment(new CRCore::crRenderBuffer(width,
							height, GL_RGB, samples, colorSamples)));
					}
				}

				fbo->apply(state);
				// If no color attachment make sure to set glDrawBuffer/glReadBuffer to none 
				// otherwise glCheckFramebufferStatus will fail
				// It has to be done after call to glBindFramebuffer (fbo->apply) 
				// and before call to glCheckFramebufferStatus
				if ( !colorAttached )
				{
					setDrawBuffer( GL_NONE );
					setReadBuffer( GL_NONE );
					glDrawBuffer( GL_NONE );
					glReadBuffer( GL_NONE );
				}
				GLenum status = fbo_ext->glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
#if _DEBUG
				switch(status) 
				{
				case GL_FRAMEBUFFER_COMPLETE_EXT:
					CRCore::notify(CRCore::ALWAYS)<<"FBO SUSESS"<<std::endl;
					break;
				case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
					CRCore::notify(CRCore::ALWAYS)<<"Unsupported framebuffer format"<<std::endl;
					break;
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
					CRCore::notify(CRCore::ALWAYS)<<"Framebuffer incomplete, missing attachment"<<std::endl;
					break;
				case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
					CRCore::notify(CRCore::ALWAYS)<<"Framebuffer incomplete, duplicate attachment"<<std::endl;
					break;
				case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
					CRCore::notify(CRCore::ALWAYS)<<"Framebuffer incomplete, attached images must have same dimensions"<<std::endl;
					break;
				case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
					CRCore::notify(CRCore::ALWAYS)<<"Framebuffer incomplete, attached images must have same format"<<std::endl;
					break;
				case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
					CRCore::notify(CRCore::ALWAYS)<<"Framebuffer incomplete, missing draw buffer"<<std::endl;
					break;
				case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
					CRCore::notify(CRCore::ALWAYS)<<"Framebuffer incomplete, missing read buffer"<<std::endl;
					break;
				}
#endif          
				if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
				{
					CRCore::notify(CRCore::FATAL)<<"crRenderStage::runCameraSetUp(), FBO setup failed, FBO status= 0x"<<std::hex<<status<<std::endl;
                    
					fbo_supported = false;
					fbo_ext->glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
					fbo = 0;
	                
					// clean up.
					double availableTime = 100.0f;
					double currentTime = crFrameStamp::getInstance()->getReferenceTime();
					CRCore::crRenderBuffer::flushDeletedRenderBuffers(state.getContextID(),currentTime,availableTime);
					CRCore::crFrameBufferObject::flushDeletedFrameBufferObjects(state.getContextID(),currentTime,availableTime);

					//exit(0);
				}
				else
				{
					setDrawBuffer(GL_NONE);
					setReadBuffer(GL_NONE);
	       
					m_fbo = fbo;

					if (fbo_multisample.valid())
					{
						fbo_multisample->apply(state);

						GLenum status = fbo_ext->glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

						if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
						{
							notify(NOTICE) << "crRenderStage::runCameraSetUp(), "
								"multisample FBO setup failed, FBO status = 0x"
								<< std::hex << status << std::endl;

							fbo->apply(state);
							fbo_multisample = 0;
							m_resolveFbo = 0;

							// clean up.
							double availableTime = 100.0f;
							double currentTime = crFrameStamp::getInstance()->getReferenceTime();
							CRCore::crRenderBuffer::flushDeletedRenderBuffers(state.getContextID(),currentTime,availableTime);
							CRCore::crFrameBufferObject::flushDeletedFrameBufferObjects(state.getContextID(),currentTime,availableTime);
						}
						else
						{
							m_resolveFbo.swap(m_fbo);
							m_fbo = fbo_multisample;
						}
					}
					else
					{
						m_resolveFbo = 0;
					}
				}
			//}
        }
        
        if (!fbo_supported)
        {
            if (renderTargetImplementation<renderTargetFallback)
                renderTargetImplementation = renderTargetFallback;
            else
                renderTargetImplementation = CRCore::crCameraNode::PIXEL_BUFFER_RTT;
        }
    }
    
    // check whether PBuffer-RTT is supported or not
    if (renderTargetImplementation==CRCore::crCameraNode::PIXEL_BUFFER_RTT && 
        !CRCore::isGLExtensionSupported(state.getContextID(), "WGL_ARB_render_texture"))
    {    
        if (renderTargetImplementation<renderTargetFallback)
            renderTargetImplementation = renderTargetFallback;
        else
            renderTargetImplementation = CRCore::crCameraNode::PIXEL_BUFFER;
    }

    // if any of the renderTargetImplementations require a separate graphics context such as with pbuffer try in turn to
    // set up, but if each level fails then resort to the next level down.
	CRCore::ref_ptr<CRCore::crGraphicsContext> context;
	CRCore::ref_ptr<CRCore::crGraphicsContext::Traits> traits;
	CRCore::crTexture* pBufferTexture = 0;
	GLenum bufferFormat = GL_NONE;
	unsigned int level = 0; 
	unsigned int face = 0; 

	bool colorAttached = false;
	bool depthAttached = false;
	bool stencilAttached = false;
	CRCore::crCameraNode::BufferComponent buffer;
    while (!getGraphicsContext() &&
           (renderTargetImplementation==CRCore::crCameraNode::PIXEL_BUFFER_RTT ||
            renderTargetImplementation==CRCore::crCameraNode::PIXEL_BUFFER ||
            renderTargetImplementation==CRCore::crCameraNode::SEPERATE_WINDOW) )
    {
        context = getGraphicsContext();
        if (!context)
        {

            // set up the traits of the graphics context that we want
            traits = new CRCore::crGraphicsContext::Traits;

            traits->m_width = width;
            traits->m_height = height;

            // CRCore::notify(CRCore::NOTICE)<<"traits = "<<traits->width<<" "<<traits->height<<std::endl;

            traits->m_pbuffer = (renderTargetImplementation==CRCore::crCameraNode::PIXEL_BUFFER || renderTargetImplementation==CRCore::crCameraNode::PIXEL_BUFFER_RTT);
            traits->m_windowDecoration = (renderTargetImplementation==CRCore::crCameraNode::SEPERATE_WINDOW);
            traits->m_doubleBuffer = (renderTargetImplementation==CRCore::crCameraNode::SEPERATE_WINDOW);

            for(CRCore::crCameraNode::BufferAttachmentMap::iterator itr = bufferAttachments.begin();
                itr != bufferAttachments.end();
                ++itr)
            {

                buffer = itr->first;
                CRCore::crCameraNode::Attachment& attachment = itr->second;
                switch(buffer)
                {
                    case(CRCore::crCameraNode::DEPTH_BUFFER):
                    {
                        traits->m_depth = 24;
                        depthAttached = true;
                        break;
                    }
                    case(CRCore::crCameraNode::STENCIL_BUFFER):
                    {
                        traits->m_stencil = 8;
                        stencilAttached = true;
                        break;
                    }
                    case(CRCore::crCameraNode::COLOR_BUFFER):
                    {
                        if (attachment.m_internalFormat!=GL_NONE)
                        {
                            bufferFormat = attachment.m_internalFormat;
                        }
                        else
                        {
                            if (attachment.m_texture.valid())
                            {
                                pBufferTexture = attachment.m_texture.get();
                                bufferFormat = attachment.m_texture->getInternalFormat();
                            }
                            else if (attachment.m_image.valid())
                            {
                                bufferFormat = attachment.m_image->getInternalTextureFormat();
                            }
                            else
                            {
                                bufferFormat = GL_RGBA;
                            }
                        }

                        level = attachment.m_level;
                        face = attachment.m_face;

                        if (renderTargetImplementation==CRCore::crCameraNode::PIXEL_BUFFER_RTT)
                        {
                            traits->m_target = attachment.m_texture.valid() ? attachment.m_texture->getTextureTarget() : 0;
                            traits->m_level = level;
                            traits->m_face = face;
                            traits->m_mipMapGeneration = attachment.m_mipMapGeneration;
                        }
                        break;
                    }
                    default:
                    {
                        if (renderTargetImplementation==CRCore::crCameraNode::SEPERATE_WINDOW)
                            CRCore::notify(CRCore::NOTICE)<<"Warning: crRenderStage::runCameraSetUp(crState&) Window ";
                        else
                            CRCore::notify(CRCore::NOTICE)<<"Warning: crRenderStage::runCameraSetUp(crState&) Pbuffer ";

                        CRCore::notify(CRCore::NOTICE)<<"does not support multiple color outputs."<<std::endl;
                        break;
                    }

                }
            }

            if (!depthAttached)
            {                
                traits->m_depth = 24;
            }

            if (!colorAttached)
            {                
                if (bufferFormat == GL_NONE) bufferFormat = GL_RGB;

                traits->m_red = 8;
                traits->m_green = 8;
                traits->m_blue = 8;
                traits->m_alpha = (bufferFormat==GL_RGBA) ? 8 : 0;
            }

            // share OpenGL objects if possible...
            if (state.getGraphicsContext())
            {
                traits->m_sharedContext = state.getGraphicsContext();
                
                const CRCore::crGraphicsContext::Traits* sharedTraits = traits->m_sharedContext->getTraits();
                if (sharedTraits)
                {
                    traits->m_hostName = sharedTraits->m_hostName;
                    traits->m_displayNum = sharedTraits->m_displayNum;
                    traits->m_screenNum = sharedTraits->m_screenNum;
                }
            }

            // create the graphics context according to these traits.
            context = CRCore::crGraphicsContext::createGraphicsContext(traits.get());

            if (context.valid() && context->realize())
            {
                //CRCore::notify(CRCore::INFO)<<"crRenderStage::runCameraSetUp(crState&) Context has been realized "<<std::endl;

                // successfully set up graphics context as requested,
                // will assign this graphics context to the crRenderStage and 
                // associated parameters.  Setting the graphics context will
                // single this while loop to exit successful.
                setGraphicsContext(context.get());
                
                // how to do we detect that an attempt to set up RTT has failed??

                setDrawBuffer(GL_FRONT);
                setReadBuffer(GL_FRONT);

                if (pBufferTexture && renderTargetImplementation==CRCore::crCameraNode::PIXEL_BUFFER_RTT)
                {
                   //CRCore::notify(CRCore::INFO)<<"crRenderStage::runCameraSetUp(crState&) Assign graphis context to crTexture"<<std::endl;
                   pBufferTexture->setReadPBuffer(context.get());
                }
                else
                {
                    //CRCore::notify(CRCore::INFO)<<"crRenderStage::runCameraSetUp(crState&) Assigning texture to crRenderStage so that it does the copy"<<std::endl;
                    setTexture(pBufferTexture, level, face);
                }
            }
            else
            {
                //CRCore::notify(CRCore::INFO)<<"Failed to acquire Graphics Context"<<std::endl;
                
                if (renderTargetImplementation==CRCore::crCameraNode::PIXEL_BUFFER_RTT)
                {
                    // fallback to using standard PBuffer, this will allow this while loop to continue
                    if (renderTargetImplementation<renderTargetFallback)
                        renderTargetImplementation = renderTargetFallback;
                    else
                        renderTargetImplementation = CRCore::crCameraNode::PIXEL_BUFFER;
                }
                else 
                {
                    renderTargetImplementation = CRCore::crCameraNode::FRAME_BUFFER;
                }
            }

        }
    }
    
    // finally if all else has failed, then the frame buffer fallback will come in to play.
    if (renderTargetImplementation==CRCore::crCameraNode::FRAME_BUFFER)
    {
        //CRCore::notify(CRCore::INFO)<<"Setting up CRCore::crCameraNode::FRAME_BUFFER"<<std::endl;

        for(CRCore::crCameraNode::BufferAttachmentMap::iterator itr = bufferAttachments.begin();
            itr != bufferAttachments.end();
            ++itr)
        {
            // assign the texture... 
            if (itr->second.m_texture.valid()) setTexture(itr->second.m_texture.get(), itr->second.m_level, itr->second.m_face);
        }
    }
	m_cameraRequiresSetUp = false;
}

void crRenderStage::copyTexture(CRCore::crState& state)
{
    if (m_readBuffer != GL_NONE)
    {
        glReadBuffer(m_readBuffer);
    }

    // need to implement texture cube map etc...
    CRCore::crTexture1D* texture1D = 0;
    CRCore::crTexture2D* texture2D = 0;
    CRCore::crTexture3D* texture3D = 0;
    CRCore::crTextureRectangle* textureRec = 0;
    CRCore::crTextureCubeMap* textureCubeMap = 0;

    // use TexCopySubImage with the offset of the viewport into the texture
    // note, this path mirrors the pbuffer and fbo means for updating the texture.
    // Robert Osfield, 3rd August 2006.
    if ((texture2D = dynamic_cast<CRCore::crTexture2D*>(m_texture.get())) != 0)
    {
        texture2D->copyTexSubImage2D(state,
                                     static_cast<int>(m_viewport->x()),
                                     static_cast<int>(m_viewport->y()),
                                     static_cast<int>(m_viewport->x()),
                                     static_cast<int>(m_viewport->y()),
                                     static_cast<int>(m_viewport->width()),
                                     static_cast<int>(m_viewport->height()));
    }
    else if ((textureRec = dynamic_cast<CRCore::crTextureRectangle*>(m_texture.get())) != 0)
    {
        textureRec->copyTexSubImage2D(state,
                                     static_cast<int>(m_viewport->x()),
                                     static_cast<int>(m_viewport->y()),
                                     static_cast<int>(m_viewport->x()),
                                     static_cast<int>(m_viewport->y()),
                                     static_cast<int>(m_viewport->width()),
                                     static_cast<int>(m_viewport->height()));
    }
    else if ((texture1D = dynamic_cast<CRCore::crTexture1D*>(m_texture.get())) != 0)
    {
        // need to implement
        texture1D->copyTexSubImage1D(state,
                                     static_cast<int>(m_viewport->x()),
                                     static_cast<int>(m_viewport->x()),
                                     static_cast<int>(m_viewport->y()),
                                     static_cast<int>(m_viewport->width()));
    }
    else if ((texture3D = dynamic_cast<CRCore::crTexture3D*>(m_texture.get())) != 0)
    {
        // need to implement
        texture3D->copyTexSubImage3D(state, 
                                     static_cast<int>(m_viewport->x()),
                                     static_cast<int>(m_viewport->y()),
                                     m_face,
                                     static_cast<int>(m_viewport->x()),
                                     static_cast<int>(m_viewport->y()),
                                     static_cast<int>(m_viewport->width()),
                                     static_cast<int>(m_viewport->height()));
    }
    else if ((textureCubeMap = dynamic_cast<CRCore::crTextureCubeMap*>(m_texture.get())) != 0)
    {
        // need to implement
        textureCubeMap->copyTexSubImageCubeMap(state, m_face, 
                                     static_cast<int>(m_viewport->x()),
                                     static_cast<int>(m_viewport->y()),
                                     static_cast<int>(m_viewport->x()),
                                     static_cast<int>(m_viewport->y()),
                                     static_cast<int>(m_viewport->width()),
                                     static_cast<int>(m_viewport->height()));
    }
}

void crRenderStage::drawInner(CRCore::crState& state,crRenderLeaf*& previous, bool& doCopyTexture)
{
    struct SubFunc
    {
        static void applyReadFBO(bool& apply_read_fbo,
            const crFrameBufferObject* read_fbo, CRCore::crState& state)
        {
            if (read_fbo->isMultisample())
            {
                CRCore::notify(CRCore::WARN) << "Attempting to read from a"
                    " multisampled framebuffer object. Set a resolve"
                    " framebuffer on the crRenderStage to fix this." << std::endl;
            }

            if (apply_read_fbo)
            {
                // Bind the monosampled FBO to read from
				read_fbo->apply(state, crFrameBufferObject::READ_FRAMEBUFFER);
                apply_read_fbo = false;
            }
        }
    };

    CRCore::crFBOExtensions* fbo_ext = m_fbo.valid() ? CRCore::crFBOExtensions::instance(state.getContextID(),true) : 0;
    bool fbo_supported = fbo_ext && fbo_ext->isSupported();

    bool using_multiple_render_targets = fbo_supported && m_fbo->hasMultipleRenderingTargets();
    
    if (!using_multiple_render_targets)
    {
        if (m_drawBuffer != GL_NONE)
        {    
            glDrawBuffer(m_drawBuffer);
        }

        if (m_readBuffer != GL_NONE)
        {
            glReadBuffer(m_readBuffer);
        }
    }

    if (fbo_supported)
    {
        m_fbo->apply(state);
    }

//#define ZperPass
//#ifdef ZperPass
	if(crDisplaySettings::instance()->getUseZperPass())
	{
		if(crDisplaySettings::instance()->getHDR()>0)
		{
			if(m_camera && m_camera->getName().compare("SceneRender") == 0)
				drawZprePass(state,previous);
		}
		else if(!m_camera || m_camera->getName().compare("SceneRender") == 0)
			drawZprePass(state,previous);
	}
//#endif

    // do the drawing itself.    
    crRenderBin::draw(state,previous);


    //if(state.getCheckForGLErrors()!=CRCore::crState::NEVER_CHECK_GL_ERRORS)
    //{
    //    GLenum errorNo = glGetError();
    //    if (errorNo!=GL_NO_ERROR)
    //    {
    //        const char* error = (char*)gluErrorString(errorNo);
    //        if (error)  CRCore::notify(CRCore::NOTICE)<<"Warning: detected OpenGL error '"<<error<<"' after crRenderBin::draw(,)"<<std::endl;
    //        else        CRCore::notify(CRCore::NOTICE)<<"Warning: detected OpenGL errorNo= 0x"<<std::hex<<errorNo<<" after crRenderBin::draw(,)"<<std::endl;

    //        if (fbo_ext) CRCore::notify(CRCore::NOTICE)<<"crRenderStage::drawInner(,) FBO status= 0x"<<std::hex<<fbo_ext->glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)<<std::endl;
    //    }
    //}

    const crFrameBufferObject* read_fbo = fbo_supported ? m_fbo.get() : 0;
    bool apply_read_fbo = false;

    if (fbo_supported && m_resolveFbo.valid() && fbo_ext->glBlitFramebufferEXT)
    {
        GLbitfield blitMask = 0;
        bool needToBlitColorBuffers = false;

        //find which buffer types should be copied
        for (crFrameBufferObject::AttachmentMap::const_iterator
            it = m_resolveFbo->getAttachmentMap().begin(),
            end =m_resolveFbo->getAttachmentMap().end(); it != end; ++it)
        {
            switch (it->first)
            {
            case crCameraNode::DEPTH_BUFFER:
                blitMask |= GL_DEPTH_BUFFER_BIT;
                break;
            case crCameraNode::STENCIL_BUFFER:
                blitMask |= GL_STENCIL_BUFFER_BIT;
                break;
			case crCameraNode::PACKED_DEPTH_STENCIL_BUFFER:
				blitMask |= GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
				break;
			case crCameraNode::COLOR_BUFFER:
				blitMask |= GL_COLOR_BUFFER_BIT;
				break;
            default:
				needToBlitColorBuffers = true;
				break;
			}
		}

		////Bind the MS FBO 
		//glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, multisample_fboID); 
		////Bind the standard FBO 
		//glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, fboID); 
		////Let's say I want to copy the entire surface 
		////Let's say I only want to copy the color buffer only 
		////Let's say I don't need the GPU to do filtering since both surfaces have the same dimension 
		//glBlitFramebufferEXT(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST); 
		////-------------------- 
		////Bind the standard FBO for reading 
		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID); 
		//glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, pixels); 

        // Bind the resolve framebuffer to blit into.
		m_fbo->apply(state, crFrameBufferObject::READ_FRAMEBUFFER);//new add
		//glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
        m_resolveFbo->apply(state, crFrameBufferObject::DRAW_FRAMEBUFFER);
        //glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
        // Blit to the resolve framebuffer.
        // Note that (with nvidia 175.16 windows drivers at least) if the read
        // framebuffer is multisampled then the dimension arguments are ignored
        // and the whole framebuffer is always copied.
		if (blitMask)
		{
			fbo_ext->glBlitFramebufferEXT(
				0, 0, static_cast<GLint>(m_viewport->width()), static_cast<GLint>(m_viewport->height()),
				0, 0, static_cast<GLint>(m_viewport->width()), static_cast<GLint>(m_viewport->height()),
				blitMask, GL_NEAREST);
		}
		if (needToBlitColorBuffers)
		{
			for (crFrameBufferObject::AttachmentMap::const_iterator
				it = m_resolveFbo->getAttachmentMap().begin(),
				end =m_resolveFbo->getAttachmentMap().end(); it != end; ++it)
			{
				crCameraNode::BufferComponent attachment = it->first;
				if (attachment >=crCameraNode::COLOR_BUFFER0)
				{
					glReadBuffer(GL_COLOR_ATTACHMENT0_EXT + (attachment - crCameraNode::COLOR_BUFFER0));
					glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + (attachment - crCameraNode::COLOR_BUFFER0));

					fbo_ext->glBlitFramebufferEXT(
						0, 0, static_cast<GLint>(m_viewport->width()), static_cast<GLint>(m_viewport->height()),
						0, 0, static_cast<GLint>(m_viewport->width()), static_cast<GLint>(m_viewport->height()),
						GL_COLOR_BUFFER_BIT, GL_NEAREST);
				}
			}
			// reset the read and draw buffers?  will comment out for now with the assumption that
			// the buffers will be set explictly when needed elsewhere.
			// glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
			// glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		}
        apply_read_fbo = true;
		read_fbo = m_resolveFbo.get();

        using_multiple_render_targets = read_fbo->hasMultipleRenderingTargets();
    }

    // now copy the rendered image to attached texture.
    if (doCopyTexture)
    {
        if (read_fbo) SubFunc::applyReadFBO(apply_read_fbo, read_fbo, state);
        copyTexture(state);
    }

    std::map< CRCore::crCameraNode::BufferComponent, Attachment>::const_iterator itr;
	GLenum pixelFormat;
	GLenum dataType;
    for(itr = m_bufferAttachmentMap.begin();
        itr != m_bufferAttachmentMap.end();
        ++itr)
    {
        if (itr->second.m_image.valid())
        {
            if (read_fbo) SubFunc::applyReadFBO(apply_read_fbo, read_fbo, state);

            if (using_multiple_render_targets)
            {
                int attachment=itr->first;
                if (attachment==CRCore::crCameraNode::DEPTH_BUFFER || attachment==CRCore::crCameraNode::STENCIL_BUFFER) 
				{
                    // assume first buffer rendered to is the one we want
                    glReadBuffer(read_fbo->getMultipleRenderingTargets()[0]);
                } 
				else 
				{
                    glReadBuffer(GL_COLOR_ATTACHMENT0_EXT + (attachment - CRCore::crCameraNode::COLOR_BUFFER0));
                }
            } 
			else 
			{
                if (m_readBuffer != GL_NONE)
                {
                    glReadBuffer(m_readBuffer);
                }
            }

            pixelFormat = itr->second.m_image->getPixelFormat();
            if (pixelFormat==0) pixelFormat = m_imageReadPixelFormat;
            if (pixelFormat==0) pixelFormat = GL_RGB;

            dataType = itr->second.m_image->getDataType();
            if (dataType==0) dataType = m_imageReadPixelDataType;
            if (dataType==0) dataType = GL_UNSIGNED_BYTE;       

            (const_cast<crImage *>(itr->second.m_image.get()))->readPixels(static_cast<int>(m_viewport->x()),
                                           static_cast<int>(m_viewport->y()),
                                           static_cast<int>(m_viewport->width()),
                                           static_cast<int>(m_viewport->height()), 
                                           pixelFormat, dataType);
        }
    }

    if (fbo_supported)
    {
        if (getDisableFboAfterRender())
        {
            // switch off the frame buffer object
            fbo_ext->glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        }

        doCopyTexture = true;
    }

    if (fbo_supported && m_camera)
    {
        // now generate mipmaps if they are required.
        const CRCore::crCameraNode::BufferAttachmentMap& bufferAttachments = m_camera->getBufferAttachmentMap();
        for(CRCore::crCameraNode::BufferAttachmentMap::const_iterator itr = bufferAttachments.begin();
            itr != bufferAttachments.end();
            ++itr)
        {
            if (itr->second.m_texture.valid() && itr->second.m_mipMapGeneration) 
            {
                state.setActiveTextureUnit(0);
                state.applyTextureAttribute(0, itr->second.m_texture.get());
                fbo_ext->glGenerateMipmapEXT(itr->second.m_texture->getTextureTarget());
            }
        }
    }
}

struct DrawInnerOperation : public CRCore::crGraphicsThread::Operation
{
    DrawInnerOperation(crRenderStage* stage) : 
		CRCore::crGraphicsThread::Operation("DrawInnerStage",false),
		m_stage(stage) {}

    virtual void operator () (CRCore::crGraphicsContext* context)
    {
		if (m_stage && context)
		{
			crRenderLeaf* previous = 0;
			bool doCopyTexture = false;
			m_stage->drawInner(*(context->getState()), previous, doCopyTexture);
		}
    }
    
    crRenderStage* m_stage;
};


void crRenderStage::draw(CRCore::crState& state,crRenderLeaf*& previous)
{
    if (m_stageDrawnThisFrame) return;

    m_stageDrawnThisFrame = true;

    if (m_camera && m_camera->getInitialDrawCallback())
    {
        // if we have a camera with a intial draw callback invoke it.
        (*(m_camera->getInitialDrawCallback()))(*m_camera);
    }

    // note, SceneView does call to drawPreRenderStages explicitly
    // so there is no need to call it here.
    drawPreRenderStages(state,previous);
    
    if (m_cameraRequiresSetUp)
    {
        runCameraSetUp(state);
    }
	else if(m_camera && m_camera->getExportImageBuf())
	{
		m_bufferAttachmentMap.clear();
		CRCore::crCameraNode::BufferAttachmentMap& bufferAttachments = m_camera->getBufferAttachmentMap();
		CRCore::crCameraNode::BufferAttachmentMap::iterator itr;
		// attach images that need to be copied after the stage is drawn.
		CRCore::crImage* image;
		GLenum pixelFormat;
		GLenum dataType;
		for(itr = bufferAttachments.begin();
			itr != bufferAttachments.end();
			++itr)
		{
			// if one exist attach image to the crRenderStage.
			if (itr->second.m_image.valid())
			{
				image = itr->second.m_image.get();
				pixelFormat = image->getPixelFormat();
				dataType = image->getDataType();

				if (image->data()==0)
				{
					if (pixelFormat==0) pixelFormat = itr->second.m_internalFormat;
					if (pixelFormat==0) pixelFormat = m_imageReadPixelFormat;
					if (pixelFormat==0) pixelFormat = GL_RGBA;

					if (dataType==0) dataType = m_imageReadPixelDataType;
					if (dataType==0) dataType = GL_UNSIGNED_BYTE;
				}

				m_bufferAttachmentMap[itr->first].m_imageReadPixelFormat = pixelFormat;
				m_bufferAttachmentMap[itr->first].m_imageReadPixelDataType = dataType;
				m_bufferAttachmentMap[itr->first].m_image = image;
			}
		}
		m_camera->exportImageBuf(m_camera->getExportImageBuf()+1);
	}

    //CRCore::crState* useState = &state;
    CRCore::crGraphicsContext* callingContext = state.getGraphicsContext();
    CRCore::crGraphicsContext* useContext = callingContext;
	CRCore::crGraphicsThread* useThread = 0;
    
	crRenderLeaf* saved_previous = previous;	

    //if (m_graphicsContext.valid() && m_graphicsContext != callingContext)
    //{
    //    // show we release the context so that others can use it?? will do so right
    //    // now as an experiment.
    //    callingContext->releaseContext();
    //
    //    // CRCore::notify(CRCore::NOTICE)<<"  enclosing state before - "<<state.getStateSetStackSize()<<std::endl;

    //    useState = m_graphicsContext->getState();
    //    useContext = m_graphicsContext.get();
    //    useThread = useContext->getGraphicsThread();
    //            
    //    if (!useThread) 
    //    {
    //        previous = 0;
    //        useContext->makeCurrent();
    //        // CRCore::notify(CRCore::NOTICE)<<"  nested state before - "<<useState->getStateSetStackSize()<<std::endl;
    //    }
    //}

    if (m_camera && m_camera->getPreDrawCallback())
    {
        // if we have a camera with a pre draw callback invoke it.
        (*(m_camera->getPreDrawCallback()))(*m_camera);
    }
    
    bool doCopyTexture = m_texture.valid() ? 
                        (callingContext != useContext) :
                        false;
	//if (useThread)
	//{
	//	useThread->add(new DrawInnerOperation( this ), true);

	//	doCopyTexture = false;
	//}
	//else
	//{
		drawInner( /**useState*/state, previous, doCopyTexture);
	//}
//    if (useThread)
//    {
//#if 1
//        ref_ptr<CRCore::BlockAndFlushOperation> block = new CRCore::BlockAndFlushOperation;
//    
//        useThread->add(new DrawInnerOperation( this, renderInfo ));
//        
//        useThread->add(block.get());
//        
//        // wait till the DrawInnerOperations is complete.
//        block->block();
//        
//        doCopyTexture = false;
//        
//#else
//        useThread->add(new DrawInnerOperation( this, renderInfo ), true);
//        
//        doCopyTexture = false;
//#endif        
//    }
//    else
//    {
//        drawInner( useRenderInfo, previous, doCopyTexture);
//    }

    // now copy the rendered image to attached texture.
    if (m_texture.valid() && !doCopyTexture)
    {
        //if (callingContext && useContext!= callingContext)
        //{
        //    // make the calling context use the pbuffer context for reading.
        //    callingContext->makeContextCurrent(useContext);
        //}

        copyTexture(state);
    }

    if (m_camera && m_camera->getPostDrawCallback())
    {
        // if we have a camera with a post draw callback invoke it.
        (*(m_camera->getPostDrawCallback()))(*m_camera);
    }

    //if (m_graphicsContext.valid() && m_graphicsContext != callingContext)
    //{
    //    if (!useThread)
    //    {
    //        // flush any command left in the useContex's FIFO
    //        // to ensure that textures are updated before main thread commenses.
    //        glFlush();
    //        useContext->releaseContext();
    //    }
    //}

    //if (callingContext && useContext != callingContext)
    //{
    //    // restore the graphics context.
    //    
    //    previous = saved_previous;
    //    
    //    // CRCore::notify(CRCore::NOTICE)<<"  nested state after - "<<useState->getStateSetStackSize()<<std::endl;
    //    // CRCore::notify(CRCore::NOTICE)<<"  enclosing state after - "<<state.getStateSetStackSize()<<std::endl;

    //    callingContext->makeCurrent();
    //}

    // render all the post draw callbacks
    drawPostRenderStages(state,previous);

    if (m_camera && m_camera->getFinalDrawCallback())
    {
        // if we have a camera with a final callback invoke it.
        (*(m_camera->getFinalDrawCallback()))(*m_camera);
    }
}

void crRenderStage::drawImplementation(CRCore::crState& state,crRenderLeaf*& previous)
{
    if (!m_viewport)
    {
        notify(FATAL) << "Error: cannot draw stage due to undefined viewport."<< std::endl;
        return;
    }
    // set up the back buffer.
    state.applyAttribute(m_viewport.get());
#define USE_SISSOR_TEST
#ifdef USE_SISSOR_TEST
    glScissor( static_cast<int>(m_viewport->x()),
               static_cast<int>(m_viewport->y()),
               static_cast<int>(m_viewport->width()),
               static_cast<int>(m_viewport->height()) );
    //cout << "    clearing "<<this<< "  "<<m_viewport->x()<<","<< m_viewport->y()<<","<< m_viewport->width()<<","<< m_viewport->height()<<std::endl;
    
    glEnable( GL_SCISSOR_TEST );
#endif


    // glEnable( GL_DEPTH_TEST );

    // set which color planes to operate on.
    if (m_colorMask.valid()) m_colorMask->apply(state);
	else glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

    if (m_clearMask & GL_COLOR_BUFFER_BIT)
        glClearColor( m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);

    if (m_clearMask & GL_DEPTH_BUFFER_BIT)
        glClearDepth( m_clearDepth);

    if (m_clearMask & GL_STENCIL_BUFFER_BIT)
        glClearStencil( m_clearStencil);

    if (m_clearMask & GL_ACCUM_BUFFER_BIT)
        glClearAccum( m_clearAccum[0], m_clearAccum[1], m_clearAccum[2], m_clearAccum[3]);


    if(m_clearMask != GL_NONE) glClear( m_clearMask );
    
#ifdef USE_SISSOR_TEST
    glDisable( GL_SCISSOR_TEST );
#endif

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

	state.setIgnoreColor(false);

    // apply the positional state.
    if (m_inheritedPositionalStateContainer.valid())
    {
        m_inheritedPositionalStateContainer->draw(state, previous, &m_inheritedPositionalStateContainerMatrix);
    }

    // apply the positional state.
    if (m_renderStageLighting.valid())
    {
        m_renderStageLighting->draw(state, previous, 0);
    }

    // draw the children and local.
    crRenderBin::drawImplementation(state,previous);

    ////state.apply();
	if (previous)
	{
		crRenderGraph::moveToRootRenderGraph(state,previous->m_parent);
		state.apply();
		previous = NULL;
	}
#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crRenderStage::drawImplementation OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		//CRCore::notify(WARN)<<"crRenderStage::drawImplementation<Warning>: detected OpenGL error '"<<gluErrorString(errorNo)<<"'"<< std::endl;
	}
#endif

}

void crRenderStage::drawPostRenderStages(CRCore::crState& state,crRenderLeaf*& previous)
{
    if (m_postRenderList.empty()) return;
    
    //cout << "Drawing prerendering stages "<<this<< "  "<<m_viewport->x()<<","<< m_viewport->y()<<","<< m_viewport->width()<<","<< m_viewport->height()<<std::endl;
    for(RenderStageList::iterator itr=m_postRenderList.begin();
        itr!=m_postRenderList.end();
        ++itr)
    {
        (*itr)->draw(state,previous);
    }
    //cout << "Done Drawing prerendering stages "<<this<< "  "<<m_viewport->x()<<","<< m_viewport->y()<<","<< m_viewport->width()<<","<< m_viewport->height()<<std::endl;
}

void crRenderStage::releaseObjects(CRCore::crState* state)
{
	if(m_fbo.valid())
	{
		m_fbo->releaseObjects(state);
		m_fbo = NULL;
	}
	if(m_texture.valid())
	{
		m_texture->releaseObjects(state);
		m_texture = NULL;
	}
	RenderStageList::iterator itr;

	for( itr = m_preRenderList.begin();
		itr != m_preRenderList.end();
		++itr )
	{
		(*itr)->releaseObjects(state);
	}
	m_preRenderList.clear();
	for( itr = m_postRenderList.begin();
		itr != m_postRenderList.end();
		++itr )
	{
		(*itr)->releaseObjects(state);
	}
	m_postRenderList.clear();
}

//// Statistics features
//bool crRenderStage::getStats(Statistics& stats) const
//{
//    bool statsCollected = false;
//
//    for(RenderStageList::const_iterator pre_itr = m_preRenderList.begin();
//        pre_itr != m_preRenderList.end();
//        ++pre_itr)
//    {
//        if ((*pre_itr)->getStats(stats))
//        {
//            statsCollected = true;
//        }
//    }
//
//    for(RenderStageList::const_iterator post_itr = m_postRenderList.begin();
//        post_itr != m_postRenderList.end();
//        ++post_itr)
//    {
//        if ((*post_itr)->getStats(stats))
//        {
//            statsCollected = true;
//        }
//    }
//        
//    if (crRenderBin::getStats(stats))
//    {
//        statsCollected = true;
//    }
//    return statsCollected;
//}

void crRenderStage::attach(CRCore::crCameraNode::BufferComponent buffer, CRCore::crImage* image)
{
    m_bufferAttachmentMap[buffer].m_image = image;
}

//unsigned int crRenderStage::computeNumberOfDynamicRenderLeaves() const
//{
//    unsigned int count = 0;
//
//    for(RenderStageList::const_iterator pre_itr = m_preRenderList.begin();
//        pre_itr != m_preRenderList.end();
//        ++pre_itr)
//    {
//        count += (*pre_itr)->computeNumberOfDynamicRenderLeaves();
//    }
//
//    count += crRenderBin::computeNumberOfDynamicRenderLeaves();
//
//    for(RenderStageList::const_iterator post_itr = m_postRenderList.begin();
//        post_itr != m_postRenderList.end();
//        ++post_itr)
//    {
//        count += (*post_itr)->computeNumberOfDynamicRenderLeaves();
//    }
//    
//    return count;
//}


void CRUtil::crRenderStage::setMultisampleResolveFramebufferObject(CRCore::crFrameBufferObject* fbo)
{
    if (fbo && fbo->isMultisample())
    {
        CRCore::notify(CRCore::WARN) << "Resolve framebuffer must not be"
            " multisampled." << std::endl;
    }
    m_resolveFbo = fbo;
}
