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
#include <CRCore/crCameraNode.h>
#include <CRCore/crNotify.h>

using namespace CRCore;

crCameraNode::crCameraNode():
    m_clearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT/* | GL_STENCIL_BUFFER_BIT*/),
    m_clearColor(CRCore::crVector4(0.0f,0.0f,0.0f,1.0f)),
    m_clearAccum(CRCore::crVector4(0.0f,0.0f,0.0f,1.0f)),
    m_clearDepth(1.0),
    m_clearStencil(0),
    m_transformOrder(PRE_MULTIPLE),
    m_renderOrder(POST_RENDER),
    m_drawBuffer(GL_NONE),
    m_readBuffer(GL_NONE),
    m_renderTargetImplementation(FRAME_BUFFER),
    m_renderTargetFallback(FRAME_BUFFER),
	m_shallRender(true),
	m_exportImage(0),
	m_exportImage_buf(0)
{
    setStateSet(new crStateSet);
	m_renderingCache = new buffered_ptrobject< ref_ptr<crBase> >;
	m_renderingCache_buf = new buffered_ptrobject< ref_ptr<crBase> >;
	m_colorMask = new crColorMask(true,true,true,true);
}

crCameraNode::crCameraNode(const crCameraNode& camera,const crCopyOp& copyop):
    crTransform(camera,copyop),
    crCullSettings(camera),
    m_clearMask(camera.m_clearMask),
    m_clearColor(camera.m_clearColor),
    m_clearAccum(camera.m_clearAccum),
    m_clearDepth(camera.m_clearDepth),
    m_clearStencil(camera.m_clearStencil),
    m_colorMask(camera.m_colorMask),
    m_viewport(camera.m_viewport),
    m_transformOrder(camera.m_transformOrder),
    m_renderOrder(camera.m_renderOrder),
    m_drawBuffer(camera.m_drawBuffer),
    m_readBuffer(camera.m_readBuffer),
    m_renderTargetImplementation(camera.m_renderTargetImplementation),
    m_renderTargetFallback(camera.m_renderTargetFallback),
    m_bufferAttachmentMap(camera.m_bufferAttachmentMap),
    m_postDrawCallback(camera.m_postDrawCallback),
	m_shallRender(camera.m_shallRender),
	m_exportImage(camera.m_exportImage),
	m_exportImage_buf(camera.m_exportImage_buf)
{
	if(camera.m_projectionMatrix.valid())
	{
		m_projectionMatrix = new crMatrixd;
		m_projectionMatrix->set(*camera.m_projectionMatrix);
	}
	if(camera.m_viewMatrix.valid())
	{
		m_viewMatrix = new crMatrixd;
		m_viewMatrix->set(*camera.m_viewMatrix);
	}
	if(camera.m_projectionMatrix_buf.valid())
	{
		m_projectionMatrix_buf = new crMatrixd;
		m_projectionMatrix_buf->set(*camera.m_projectionMatrix_buf);
	}
	if(camera.m_viewMatrix_buf.valid())
	{
		m_viewMatrix_buf = new crMatrixd;
		m_viewMatrix_buf->set(*camera.m_viewMatrix_buf);
	}
}


crCameraNode::~crCameraNode()
{    
}

bool crCameraNode::isRenderToTextureCamera() const
{
    return (!m_bufferAttachmentMap.empty());
}

void crCameraNode::setRenderTargetImplementation(RenderTargetImplementation impl)
{
    m_renderTargetImplementation = impl;
    if (impl<FRAME_BUFFER) m_renderTargetFallback = (RenderTargetImplementation)(impl+1);
    else m_renderTargetFallback = impl;
}   

void crCameraNode::setRenderTargetImplementation(RenderTargetImplementation impl, RenderTargetImplementation fallback)
{
    if (impl<fallback || (impl==FRAME_BUFFER && fallback==FRAME_BUFFER))
    {
        m_renderTargetImplementation = impl;
        m_renderTargetFallback = fallback;
    }
    else 
    {
        CRCore::notify(CRCore::NOTICE)<<"Warning: crCameraNode::setRenderTargetImplementation(impl,fallback) must have a lower rated fallback than the main target implementation."<<std::endl;
        setRenderTargetImplementation(impl);
    }
}

void crCameraNode::setColorMask(CRCore::crColorMask* colorMask)
{
    if (m_colorMask == colorMask) return;

    CRCore::crStateSet* stateset = getOrCreateStateSet();
    if (m_colorMask.valid() && stateset)
    {
        stateset->removeAttribute(m_colorMask.get());
    }
    
    m_colorMask = colorMask;
    
    if (m_colorMask.valid() && stateset)
    {
        stateset->setAttribute(m_colorMask.get());
    }
}

void crCameraNode::setColorMask(bool red, bool green, bool blue, bool alpha)
{
    if (!m_colorMask) setColorMask(new CRCore::crColorMask);
    if (m_colorMask.valid()) m_colorMask->setMask(red,green,blue,alpha);
}

void crCameraNode::setViewport(CRCore::crViewPort* viewport)
{
    if (m_viewport == viewport) return;

    CRCore::crStateSet* stateset = getOrCreateStateSet();
    if (m_viewport.valid() && stateset)
    {
        stateset->removeAttribute(m_viewport.get());
    }
    
    m_viewport = viewport;
    
    if (m_viewport.valid() && stateset)
    {
        stateset->setAttribute(m_viewport.get());
    }
}

void crCameraNode::setViewport(int x,int y,int width,int height)
{
    if (!m_viewport) setViewport(new CRCore::crViewPort);
    if (m_viewport.valid()) m_viewport->setViewport(x,y,width,height);
}


void crCameraNode::setProjectionMatrixAsOrtho(double left, double right,
                                           double bottom, double top,
                                           double zNear, double zFar)
{
    setProjectionMatrix(CRCore::crMatrixd::ortho(left, right,
                                           bottom, top,
                                           zNear, zFar));
}                                           

void crCameraNode::setProjectionMatrixAsOrtho2D(double left, double right,
                                             double bottom, double top)
{
    setProjectionMatrix(CRCore::crMatrixd::ortho2D(left, right,
                                             bottom, top));
}

void crCameraNode::setProjectionMatrixAsFrustum(double left, double right,
                                             double bottom, double top,
                                             double zNear, double zFar)
{
    setProjectionMatrix(CRCore::crMatrixd::frustum(left, right,
                                             bottom, top,
                                             zNear, zFar));
}

void crCameraNode::setProjectionMatrixAsPerspective(double fovy,double aspectRatio,
                                                 double zNear, double zFar)
{
    setProjectionMatrix(CRCore::crMatrixd::perspective(fovy,aspectRatio,
                                                 zNear, zFar));
}                                      

bool crCameraNode::getProjectionMatrixAsOrtho(double& left, double& right,
                                           double& bottom, double& top,
                                           double& zNear, double& zFar)
{
    return getProjectionMatrix()->getOrtho(left, right,
                                       bottom, top,
                                       zNear, zFar);
}

bool crCameraNode::getProjectionMatrixAsFrustum(double& left, double& right,
                                             double& bottom, double& top,
                                             double& zNear, double& zFar)
{
    return getProjectionMatrix()->getFrustum(left, right,
                                         bottom, top,
                                         zNear, zFar);
}                                  

bool crCameraNode::getProjectionMatrixAsPerspective(double& fovy,double& aspectRatio,
                                                 double& zNear, double& zFar)
{
    return getProjectionMatrix()->getPerspective(fovy, aspectRatio, zNear, zFar);
}                                                 

void crCameraNode::setViewMatrixAsLookAt(const crVector3& eye,const crVector3& center,const crVector3& up)
{
    setViewMatrix(CRCore::crMatrixd::lookAt(eye,center,up));
}

void crCameraNode::getViewMatrixAsLookAt(crVector3d& eye,crVector3d& center,crVector3d& up,float lookDistance)
{
    getViewMatrix()->getLookAt(eye,center,up,lookDistance);
}

void crCameraNode::attach(BufferComponent buffer, GLenum internalFormat)
{
	switch(buffer)
	{
	case DEPTH_BUFFER:
		if(m_bufferAttachmentMap.find(PACKED_DEPTH_STENCIL_BUFFER) != m_bufferAttachmentMap.end())
		{
			notify(WARN) << "Camera: DEPTH_BUFFER already attached as PACKED_DEPTH_STENCIL_BUFFER !" << std::endl;
		}
		break;

	case STENCIL_BUFFER:
		if(m_bufferAttachmentMap.find(PACKED_DEPTH_STENCIL_BUFFER) != m_bufferAttachmentMap.end())
		{
			notify(WARN) << "Camera: STENCIL_BUFFER already attached as PACKED_DEPTH_STENCIL_BUFFER !" << std::endl;
		}
		break;

	case PACKED_DEPTH_STENCIL_BUFFER:
		if(m_bufferAttachmentMap.find(DEPTH_BUFFER) != m_bufferAttachmentMap.end())
		{
			notify(WARN) << "Camera: DEPTH_BUFFER already attached !" << std::endl;
		}
		if(m_bufferAttachmentMap.find(STENCIL_BUFFER) != m_bufferAttachmentMap.end())
		{
			notify(WARN) << "Camera: STENCIL_BUFFER already attached !" << std::endl;
		}
		break;
	default:
		break;
	}
    m_bufferAttachmentMap[buffer].m_internalFormat = internalFormat;
}

void crCameraNode::attach(BufferComponent buffer, CRCore::crTexture* texture, unsigned int level, unsigned int face, bool mipMapGeneration,
                    unsigned int multisampleSamples, 
                    unsigned int multisampleColorSamples)
{
    m_bufferAttachmentMap[buffer].m_texture = texture;
    m_bufferAttachmentMap[buffer].m_level = level;
    m_bufferAttachmentMap[buffer].m_face = face;
    m_bufferAttachmentMap[buffer].m_mipMapGeneration = mipMapGeneration;
    m_bufferAttachmentMap[buffer].m_multisampleSamples = multisampleSamples;
    m_bufferAttachmentMap[buffer].m_multisampleColorSamples = multisampleColorSamples;
}

void crCameraNode::attach(BufferComponent buffer, CRCore::crImage* image,
                    unsigned int multisampleSamples, 
                    unsigned int multisampleColorSamples)
{
    m_bufferAttachmentMap[buffer].m_image = image;
    m_bufferAttachmentMap[buffer].m_multisampleSamples = multisampleSamples;
    m_bufferAttachmentMap[buffer].m_multisampleColorSamples = multisampleColorSamples;
}

void crCameraNode::detach(BufferComponent buffer)
{
    m_bufferAttachmentMap.erase(buffer);
}

void crCameraNode::releaseObjects(CRCore::crState* state)
{
	//if (state) (*const_cast<crCameraNode*>(this)->m_renderingCache)[state->getContextID()] = 0;
	//else const_cast<crCameraNode*>(this)->m_renderingCache->setAllElementsTo(0);

	//if (state) (*const_cast<crCameraNode*>(this)->m_renderingCache_buf)[state->getContextID()] = 0;
	//else const_cast<crCameraNode*>(this)->m_renderingCache_buf->setAllElementsTo(0);
	for(unsigned i=0; i<m_renderingCache->size(); ++i)
	{
		if ((*m_renderingCache)[i].valid())
			(*m_renderingCache)[i]->releaseObjects(state);
	}
	m_renderingCache->clear();
	for(unsigned i=0; i<m_renderingCache_buf->size(); ++i)
	{
		if ((*m_renderingCache_buf)[i].valid())
			(*m_renderingCache_buf)[i]->releaseObjects(state);
	}
	m_renderingCache_buf->clear();
	crTransform::releaseObjects(state);
}


bool crCameraNode::computeLocalToWorldMatrix(crMatrix& matrix,crNodeVisitor*) const
{
	if (m_referenceFrame==RELATIVE_TO_PARENTS)
	{
		if (m_transformOrder==PRE_MULTIPLE)
		{
			matrix.preMult(*getViewMatrix());
		}
		else
		{
			matrix.postMult(*getViewMatrix());
		}
	}
	else // absolute
	{
		matrix = *getViewMatrix();
	}
	return true;
}

bool crCameraNode::computeWorldToLocalMatrix(crMatrix& matrix,crNodeVisitor*) const
{
	if (m_referenceFrame==RELATIVE_TO_PARENTS)
	{
		if (m_transformOrder==PRE_MULTIPLE)
		{
			// note doing inverse so pre becomes post.
			matrix.postMult(*getViewInverseMatrix());
		}
		else
		{
			// note doing inverse so post becomes pre.
			matrix.preMult(*getViewInverseMatrix());
		}
	}
	else // absolute
	{
		matrix = *getViewInverseMatrix();
	}
	return true;
}

void crCameraNode::swapBuffers(int frameNumber)
{
	//CRCore::notify(CRCore::ALWAYS)<<"crCameraNode::swapBuffers(): name = "<<getName()<<std::endl;
	if(m_swapFrameNumber != frameNumber)
	{
		ref_ptr<crMatrixd> matrix;
		ref_ptr<buffered_ptrobject< ref_ptr<crBase> > > cache;

		matrix = m_projectionMatrix;
		m_projectionMatrix = m_projectionMatrix_buf;
		m_projectionMatrix_buf = matrix;

		matrix = m_viewMatrix;
		m_viewMatrix = m_viewMatrix_buf;
		m_viewMatrix_buf = matrix;

		matrix = m_viewInverseMatrix;
		m_viewInverseMatrix = m_viewInverseMatrix_buf;
		m_viewInverseMatrix_buf = matrix;

		cache = m_renderingCache;
		m_renderingCache = m_renderingCache_buf;
		m_renderingCache_buf = cache;

		char c = m_exportImage;
		m_exportImage = m_exportImage_buf;
		m_exportImage_buf = c;
		crTransform::swapBuffers(frameNumber);
		m_swapFrameNumber = frameNumber;
	}
}
void crCameraNode::exportImage(char progress)
{
	m_exportImage = progress;
	if(m_exportImage == 4)
		m_exportImage = 0;
}
void crCameraNode::exportImageBuf(char progress)
{
	m_exportImage_buf = progress;
	if(m_exportImage_buf == 4)
		m_exportImage_buf = 0;
}