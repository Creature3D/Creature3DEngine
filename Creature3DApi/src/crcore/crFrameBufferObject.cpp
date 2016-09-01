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
// initial FBO support written by Marco Jez, June 2005.

#include <CRCore/crFrameBufferObject.h>
#include <CRCore/crState.h>
#include <crgl/GLExtensions.h>
#include <CRCore/crTexture1D.h>
#include <CRCore/crTexture2D.h>
#include <CRCore/crTexture2DMultisample.h>
#include <CRCore/crTexture3D.h>
#include <CRCore/crTexture2DArray.h>
#include <CRCore/crTextureCubeMap.h>
#include <CRCore/crTextureRectangle.h>
#include <CRCore/crNotify.h>
#include <CRCore/crTimer.h>
#include <CRCore/crStreamBuf.h>
#include <crgl/GLU.h>
using namespace CRCore;

static buffered_ptrobject< ref_ptr<crFBOExtensions> > s_extensions;
crFBOExtensions* crFBOExtensions::instance(unsigned contextID, bool createIfNotInitalized)
{
    if (!s_extensions[contextID] && createIfNotInitalized) s_extensions[contextID] = new crFBOExtensions(contextID);
    return s_extensions[contextID].get();
}
void crFBOExtensions::clearExtensions()
{
	s_extensions.clear();
}
/**************************************************************************
 * crFBOExtensions
 **************************************************************************/
#define LOAD_FBO_EXT(name)    setGLExtensionFuncPtr(name, (#name))

crFBOExtensions::crFBOExtensions(unsigned int contextID)
:   m_supported(false),
    glBindRenderbufferEXT(0),
    glGenRenderbuffersEXT(0),
    glDeleteRenderbuffersEXT(0),
    glRenderbufferStorageEXT(0),
    glRenderbufferStorageMultisampleEXT(0),
    glRenderbufferStorageMultisampleCoverageNV(0),
    glBindFramebufferEXT(0),
    glDeleteFramebuffersEXT(0),
    glGenFramebuffersEXT(0),
    glCheckFramebufferStatusEXT(0),
    glFramebufferTexture1DEXT(0),
    glFramebufferTexture2DEXT(0),
    glFramebufferTexture3DEXT(0),
    glFramebufferTextureLayerEXT(0),
    glFramebufferRenderbufferEXT(0),
    glGenerateMipmapEXT(0),
    glBlitFramebufferEXT(0)
{
    if (!isGLExtensionSupported(contextID, "GL_EXT_framebuffer_object"))
        return;
    
    LOAD_FBO_EXT(glBindRenderbufferEXT);
    LOAD_FBO_EXT(glGenRenderbuffersEXT);
    LOAD_FBO_EXT(glDeleteRenderbuffersEXT);
    LOAD_FBO_EXT(glRenderbufferStorageEXT);
    LOAD_FBO_EXT(glBindFramebufferEXT);
    LOAD_FBO_EXT(glDeleteFramebuffersEXT);
    LOAD_FBO_EXT(glGenFramebuffersEXT);
    LOAD_FBO_EXT(glCheckFramebufferStatusEXT);
    LOAD_FBO_EXT(glFramebufferTexture1DEXT);
    LOAD_FBO_EXT(glFramebufferTexture2DEXT);
    LOAD_FBO_EXT(glFramebufferTexture3DEXT);
    LOAD_FBO_EXT(glFramebufferTextureLayerEXT);
    LOAD_FBO_EXT(glFramebufferRenderbufferEXT);
    LOAD_FBO_EXT(glGenerateMipmapEXT);

    m_supported = 
        glBindRenderbufferEXT != 0 &&
        glDeleteRenderbuffersEXT != 0 &&
        glGenRenderbuffersEXT != 0 &&
        glRenderbufferStorageEXT != 0 &&
        glBindFramebufferEXT != 0 &&
        glDeleteFramebuffersEXT != 0 &&
        glGenFramebuffersEXT != 0 &&
        glCheckFramebufferStatusEXT != 0 &&
        glFramebufferTexture1DEXT != 0 &&
        glFramebufferTexture2DEXT != 0 &&
        glFramebufferTexture3DEXT != 0 &&
        glFramebufferRenderbufferEXT != 0 &&
        glGenerateMipmapEXT != 0;

    if (!isGLExtensionSupported(contextID, "GL_EXT_framebuffer_blit"))
        return;

    LOAD_FBO_EXT(glBlitFramebufferEXT);

    if (isGLExtensionSupported(contextID, "GL_EXT_framebuffer_multisample"))
    {
        LOAD_FBO_EXT(glRenderbufferStorageMultisampleEXT);
    }

    if (isGLExtensionSupported(contextID, "GL_NV_framebuffer_multisample_coverage"))
    {
        LOAD_FBO_EXT(glRenderbufferStorageMultisampleCoverageNV);
    }
}


/**************************************************************************
 * crRenderBuffer
 **************************************************************************/

///////////////////////////////////////////////////////////////////////////
// static cache of glRenderbuffers flagged for deletion, which will actually
// be deleted in the correct GL context.

typedef std::list<GLuint> RenderBufferHandleList;
typedef CRCore::buffered_object<RenderBufferHandleList> DeletedRenderBufferCache;

static crMutex    s_mutex_deletedRenderBufferCache;
static DeletedRenderBufferCache s_deletedRenderBufferCache;

void crRenderBuffer::deleteRenderBuffer(unsigned int contextID, GLuint rb)
{
    if( rb )
    {
        CRCore::ScopedLock<crMutex> lock(s_mutex_deletedRenderBufferCache);

        // add glProgram to the cache for the appropriate context.
        s_deletedRenderBufferCache[contextID].push_back(rb);
    }
}

void crRenderBuffer::flushDeletedRenderBuffers(unsigned int contextID,double /*currentTime*/, double& availableTime)
{
    // if no time available don't try to flush objects.
    if (availableTime<=0.0) return;

    const crFBOExtensions* extensions = crFBOExtensions::instance(contextID,true);
    if(!extensions || !extensions->isSupported() ) return;

    const CRCore::Timer& timer = *CRCore::Timer::instance();
    CRCore::Timer_t start_tick = timer.tick();
    double elapsedTime = 0.0;

    {
        CRCore::ScopedLock<crMutex> lock(s_mutex_deletedRenderBufferCache);

        RenderBufferHandleList& pList = s_deletedRenderBufferCache[contextID];
        for(RenderBufferHandleList::iterator titr=pList.begin();
            titr!=pList.end() && elapsedTime<availableTime;
            )
        {
            extensions->glDeleteRenderbuffersEXT(1, &(*titr) );
            titr = pList.erase( titr );
            elapsedTime = timer.delta_s(start_tick,timer.tick());
        }
    }

    availableTime -= elapsedTime;
}
void crRenderBuffer::flushAllDeletedRenderBuffers(unsigned int contextID)
{
	const crFBOExtensions* extensions = crFBOExtensions::instance(contextID,true);
	if(!extensions || !extensions->isSupported() ) return;

	{
		CRCore::ScopedLock<crMutex> lock(s_mutex_deletedRenderBufferCache);

		RenderBufferHandleList& pList = s_deletedRenderBufferCache[contextID];
		for(RenderBufferHandleList::iterator titr=pList.begin();
			titr!=pList.end();
			)
		{
			extensions->glDeleteRenderbuffersEXT(1, &(*titr) );
			titr = pList.erase( titr );
		}
	}
}

void crRenderBuffer::discardDeletedRenderBuffers(unsigned int contextID)
{
    CRCore::ScopedLock<crMutex> lock(s_mutex_deletedRenderBufferCache);
    RenderBufferHandleList& pList = s_deletedRenderBufferCache[contextID];
    pList.clear();
}


crRenderBuffer::crRenderBuffer()
:    crBase(),
    m_internalFormat(GL_DEPTH_COMPONENT24),
    m_width(512),
    m_height(512),
    m_samples(0),
    m_colorSamples(0)
{
}

crRenderBuffer::crRenderBuffer(int width, int height, GLenum internalFormat, int samples, int colorSamples)
:    crBase(),
    m_internalFormat(internalFormat),
    m_width(width),
    m_height(height),
    m_samples(samples),
    m_colorSamples(colorSamples)
{
}

crRenderBuffer::crRenderBuffer(const crRenderBuffer &copy, const crCopyOp &copyop)
:    crBase(copy, copyop),
    m_internalFormat(copy.m_internalFormat),
    m_width(copy.m_width),
    m_height(copy.m_height),
    m_samples(copy.m_samples),
    m_colorSamples(copy.m_colorSamples)
{
}

crRenderBuffer::~crRenderBuffer()
{
    for(unsigned i=0; i<m_objectID.size(); ++i)
    {
        if (m_objectID[i]) deleteRenderBuffer(i, m_objectID[i]);
    }
}
void crRenderBuffer::releaseObjects(crState *state)
{
	for(unsigned i=0; i<m_objectID.size(); ++i)
	{
		if (m_objectID[i]) deleteRenderBuffer(i, m_objectID[i]);
	}
	m_objectID.clear();
}
int crRenderBuffer::getMaxSamples(unsigned int contextID, const crFBOExtensions *ext)
{
    static CRCore::buffered_value<GLint> maxSamplesList;

    GLint& maxSamples = maxSamplesList[contextID];

    if (!maxSamples && ext->isMultisampleSupported())
    {
        glGetIntegerv(GL_MAX_SAMPLES_EXT, &maxSamples);
    }

    return maxSamples;
}

GLuint crRenderBuffer::getObjectID(unsigned int contextID, const crFBOExtensions *ext) const
{
    GLuint &objectID = m_objectID[contextID];

    int &dirty = m_dirty[contextID];

    if (objectID == 0)
    {
        ext->glGenRenderbuffersEXT(1, &objectID);
        if (objectID == 0) 
            return 0;
        dirty = 1;
    }

    if (dirty)
    {
        // bind and configure
        ext->glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, objectID);

        // framebuffer_multisample_coverage specification requires that coverage
        // samples must be >= color samples.
        if (m_samples < m_colorSamples)
        {
            notify(WARN) << "Coverage samples must be greater than or equal to color samples."
                " Setting coverage samples equal to color samples." << std::endl;
            const_cast<crRenderBuffer*>(this)->setSamples(m_colorSamples);
        }

        if (m_samples > 0 && ext->isMultisampleCoverageSupported())
        {
            int samples = minimum(m_samples, getMaxSamples(contextID, ext));
            int colorSamples = minimum(m_colorSamples, samples);

            ext->glRenderbufferStorageMultisampleCoverageNV(GL_RENDERBUFFER_EXT,
                samples, colorSamples, m_internalFormat, m_width, m_height);
        }
        else if (m_samples > 0 && ext->isMultisampleSupported())
        {
            int samples = minimum(m_samples, getMaxSamples(contextID, ext));

            ext->glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT,
                samples, m_internalFormat, m_width, m_height);
        }
        else
        {
            ext->glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, m_internalFormat, m_width, m_height);
        }
        dirty = 0;
    }

    return objectID;
}

/**************************************************************************
 * crFrameBufferAttachment
 **************************************************************************/

#ifndef GL_TEXTURE_CUBE_MAP_POSITIVE_X
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X  0x8515
#endif

struct crFrameBufferAttachment::Pimpl
{
    enum TargetType
    {
        RENDERBUFFER,
        TEXTURE1D,
        TEXTURE2D,
        TEXTURE3D,
        TEXTURECUBE,
        TEXTURERECT,
        TEXTURE2DARRAY,
		TEXTURE2DMULTISAMPLE
    };
    
    TargetType targetType;
    ref_ptr<crRenderBuffer> renderbufferTarget;
    ref_ptr<crTexture> textureTarget;
    int cubeMapFace;
    int level;
    int zoffset;

    explicit Pimpl(TargetType ttype = RENDERBUFFER, int lev = 0)
    :    targetType(ttype),
        cubeMapFace(0),
        level(lev),
        zoffset(0)
    {
    }

    Pimpl(const Pimpl &copy)
    :    targetType(copy.targetType),
        renderbufferTarget(copy.renderbufferTarget),
        textureTarget(copy.textureTarget),
        cubeMapFace(copy.cubeMapFace),
        level(copy.level),
        zoffset(copy.zoffset)
    {
    }
	void releaseObjects(crState *state)
	{
		if(renderbufferTarget.valid())
			renderbufferTarget->releaseObjects(state);
		if(textureTarget.valid())
			textureTarget->releaseObjects(state);
		renderbufferTarget = NULL;
		textureTarget = NULL;
	}
};

crFrameBufferAttachment::crFrameBufferAttachment()
{
    m_ximpl = new Pimpl;
}

crFrameBufferAttachment::crFrameBufferAttachment(const crFrameBufferAttachment &copy)
{
    m_ximpl = new Pimpl(*copy.m_ximpl);
}

crFrameBufferAttachment::crFrameBufferAttachment(crRenderBuffer* target)
{
    m_ximpl = new Pimpl(Pimpl::RENDERBUFFER);
    m_ximpl->renderbufferTarget = target;
}

crFrameBufferAttachment::crFrameBufferAttachment(crTexture1D* target, int level)
{
    m_ximpl = new Pimpl(Pimpl::TEXTURE1D, level);
    m_ximpl->textureTarget = target;
}

crFrameBufferAttachment::crFrameBufferAttachment(crTexture2D* target, int level)
{
    m_ximpl = new Pimpl(Pimpl::TEXTURE2D, level);
    m_ximpl->textureTarget = target;
}

crFrameBufferAttachment::crFrameBufferAttachment(crTexture2DMultisample* target, unsigned int level)
{
	m_ximpl = new Pimpl(Pimpl::TEXTURE2DMULTISAMPLE, level);
	m_ximpl->textureTarget = target;
}

crFrameBufferAttachment::crFrameBufferAttachment(crTexture3D* target, int zoffset, int level)
{
    m_ximpl = new Pimpl(Pimpl::TEXTURE3D, level);
    m_ximpl->textureTarget = target;
    m_ximpl->zoffset = zoffset;
}

crFrameBufferAttachment::crFrameBufferAttachment(crTexture2DArray* target, int layer, int level)
{
    m_ximpl = new Pimpl(Pimpl::TEXTURE2DARRAY, level);
    m_ximpl->textureTarget = target;
    m_ximpl->zoffset = layer;
}

crFrameBufferAttachment::crFrameBufferAttachment(crTextureCubeMap* target, int face, int level)
{
    m_ximpl = new Pimpl(Pimpl::TEXTURECUBE, level);
    m_ximpl->textureTarget = target;
    m_ximpl->cubeMapFace = face;
}

crFrameBufferAttachment::crFrameBufferAttachment(crTextureRectangle* target)
{
    m_ximpl = new Pimpl(Pimpl::TEXTURERECT);
    m_ximpl->textureTarget = target;
}

crFrameBufferAttachment::crFrameBufferAttachment(crCameraNode::Attachment& attachment)
{
    CRCore::crTexture* texture = attachment.m_texture.get();
    
    if (texture)
    {
        CRCore::crTexture1D* texture1D = dynamic_cast<CRCore::crTexture1D*>(texture);
        if (texture1D)
        {
            m_ximpl = new Pimpl(Pimpl::TEXTURE1D, attachment.m_level);
            m_ximpl->textureTarget = texture1D;
            return;
        }

        CRCore::crTexture2D* texture2D = dynamic_cast<CRCore::crTexture2D*>(texture);
        if (texture2D)
        {
            m_ximpl = new Pimpl(Pimpl::TEXTURE2D, attachment.m_level);
            m_ximpl->textureTarget = texture2D;
            return;
        }

		CRCore::crTexture2DMultisample* texture2DMS = dynamic_cast<CRCore::crTexture2DMultisample*>(texture);
		if (texture2DMS)
		{
			m_ximpl = new Pimpl(Pimpl::TEXTURE2DMULTISAMPLE, attachment.m_level);
			m_ximpl->textureTarget = texture2DMS;
			return;
		}

        CRCore::crTexture3D* texture3D = dynamic_cast<CRCore::crTexture3D*>(texture);
        if (texture3D)
        {
            m_ximpl = new Pimpl(Pimpl::TEXTURE3D, attachment.m_level);
            m_ximpl->textureTarget = texture3D;
            m_ximpl->zoffset = attachment.m_face;
            return;
        }
        
        CRCore::crTexture2DArray* texture2DArray = dynamic_cast<CRCore::crTexture2DArray*>(texture);
        if (texture2DArray)
        {
            m_ximpl = new Pimpl(Pimpl::TEXTURE2DARRAY, attachment.m_level);
            m_ximpl->textureTarget = texture2DArray;
            m_ximpl->zoffset = attachment.m_face;
            return;
        }

        CRCore::crTextureCubeMap* textureCubeMap = dynamic_cast<CRCore::crTextureCubeMap*>(texture);
        if (textureCubeMap)
        {
            m_ximpl = new Pimpl(Pimpl::TEXTURECUBE, attachment.m_level);
            m_ximpl->textureTarget = textureCubeMap;
            m_ximpl->cubeMapFace = attachment.m_face;
            return;
        }

        CRCore::crTextureRectangle* textureRectangle = dynamic_cast<CRCore::crTextureRectangle*>(texture);
        if (textureRectangle)
        {
            m_ximpl = new Pimpl(Pimpl::TEXTURERECT);
            m_ximpl->textureTarget = textureRectangle;
            return;
        }
    }

    CRCore::crImage* image = attachment.m_image.get();
    if (image)
    {
        if (image->s()>0 && image->t()>0)
        {
            GLenum format = attachment.m_image->getInternalTextureFormat();
            if (format == 0)
                format = attachment.m_internalFormat;
            m_ximpl = new Pimpl(Pimpl::RENDERBUFFER);
            m_ximpl->renderbufferTarget = new CRCore::crRenderBuffer(image->s(), image->t(), format);
        }
        else
        {
            CRCore::notify(CRCore::WARN)<<"Error: crFrameBufferAttachment::crFrameBufferAttachment(crCameraNode::Attachment&) passed an empty CRCore::crImage, image must be allocated first."<<std::endl;
        }
        return;
    }

    CRCore::notify(CRCore::WARN)<<"Error: crFrameBufferAttachment::crFrameBufferAttachment(crCameraNode::Attachment&) passed an unrecognised crTexture type."<<std::endl;
}


crFrameBufferAttachment::~crFrameBufferAttachment()
{
    delete m_ximpl;
}
void crFrameBufferAttachment::releaseObjects(crState *state)
{
	if(m_ximpl)
		m_ximpl->releaseObjects(state);
}
crFrameBufferAttachment &crFrameBufferAttachment::operator = (const crFrameBufferAttachment &copy)
{
    delete m_ximpl;
    m_ximpl = new Pimpl(*copy.m_ximpl);
    return *this;
}

bool crFrameBufferAttachment::isMultisample() const
{
    if (m_ximpl->renderbufferTarget.valid())
    {
        return m_ximpl->renderbufferTarget->getSamples() > 0;
    }

    return false;
}

void crFrameBufferAttachment::createRequiredTexturesAndApplyGenerateMipMap(crState &state, const crFBOExtensions* ext) const
{
    unsigned int contextID = state.getContextID();

    // force compile texture if necessary
    crTexture::TextureObject *tobj = 0;
    if (m_ximpl->textureTarget.valid())
    {
        tobj = m_ximpl->textureTarget->getTextureObject(contextID);
        if (!tobj || tobj->m_id == 0)
        {
            m_ximpl->textureTarget->compile(state);
            tobj = m_ximpl->textureTarget->getTextureObject(contextID);
        }
        if (!tobj || tobj->m_id == 0)
            return;

        crTexture::FilterMode minFilter = m_ximpl->textureTarget->getFilter(crTexture::MIN_FILTER);
        if (minFilter==crTexture::LINEAR_MIPMAP_LINEAR || 
            minFilter==crTexture::LINEAR_MIPMAP_NEAREST || 
            minFilter==crTexture::NEAREST_MIPMAP_LINEAR || 
            minFilter==crTexture::NEAREST_MIPMAP_NEAREST)
        {
            state.setActiveTextureUnit(0);
            state.applyTextureAttribute(0, m_ximpl->textureTarget.get());
            ext->glGenerateMipmapEXT(m_ximpl->textureTarget->getTextureTarget());
        }

    }
}

void crFrameBufferAttachment::attach(crState &state, GLenum target, GLenum attachment_point, const crFBOExtensions* ext) const
{
    unsigned int contextID = state.getContextID();

    crTexture::TextureObject *tobj = 0;
    if (m_ximpl->textureTarget.valid())
    {
        tobj = m_ximpl->textureTarget->getTextureObject(contextID);
        if (!tobj || tobj->m_id == 0)
        {
            m_ximpl->textureTarget->compile(state);
            tobj = m_ximpl->textureTarget->getTextureObject(contextID);

        }
        if (!tobj || tobj->m_id == 0)
            return;
    }
    
    switch (m_ximpl->targetType)
    {
    default:
    case Pimpl::RENDERBUFFER:
        ext->glFramebufferRenderbufferEXT(target, attachment_point, GL_RENDERBUFFER_EXT, m_ximpl->renderbufferTarget->getObjectID(contextID, ext));
        break;
    case Pimpl::TEXTURE1D:
        ext->glFramebufferTexture1DEXT(target, attachment_point, GL_TEXTURE_1D, tobj->m_id, m_ximpl->level);
        break;
    case Pimpl::TEXTURE2D:
        ext->glFramebufferTexture2DEXT(target, attachment_point, GL_TEXTURE_2D, tobj->m_id, m_ximpl->level);
        break;
	case Pimpl::TEXTURE2DMULTISAMPLE:
		ext->glFramebufferTexture2DEXT(target, attachment_point, GL_TEXTURE_2D_MULTISAMPLE, tobj->m_id, m_ximpl->level);
		break;
    case Pimpl::TEXTURE3D:
        ext->glFramebufferTexture3DEXT(target, attachment_point, GL_TEXTURE_3D, tobj->m_id, m_ximpl->level, m_ximpl->zoffset);
        break;
    case Pimpl::TEXTURE2DARRAY:
        ext->glFramebufferTextureLayerEXT(target, attachment_point, tobj->m_id, m_ximpl->level, m_ximpl->zoffset);
        break;
    case Pimpl::TEXTURERECT:
        ext->glFramebufferTexture2DEXT(target, attachment_point, GL_TEXTURE_RECTANGLE, tobj->m_id, 0);
        break;
    case Pimpl::TEXTURECUBE:
        ext->glFramebufferTexture2DEXT(target, attachment_point, GL_TEXTURE_CUBE_MAP_POSITIVE_X + m_ximpl->cubeMapFace, tobj->m_id, m_ximpl->level);
        break;
    }
}

int crFrameBufferAttachment::compare(const crFrameBufferAttachment &fa) const
{
    if (&fa == this) return 0;
    if (m_ximpl->targetType < fa.m_ximpl->targetType) return -1;
    if (m_ximpl->targetType > fa.m_ximpl->targetType) return 1;
    if (m_ximpl->renderbufferTarget.get() < fa.m_ximpl->renderbufferTarget.get()) return -1;
    if (m_ximpl->renderbufferTarget.get() > fa.m_ximpl->renderbufferTarget.get()) return 1;
    if (m_ximpl->textureTarget.get() < fa.m_ximpl->textureTarget.get()) return -1;
    if (m_ximpl->textureTarget.get() > fa.m_ximpl->textureTarget.get()) return 1;
    if (m_ximpl->cubeMapFace < fa.m_ximpl->cubeMapFace) return -1;
    if (m_ximpl->cubeMapFace > fa.m_ximpl->cubeMapFace) return 1;
    if (m_ximpl->level < fa.m_ximpl->level) return -1;
    if (m_ximpl->level > fa.m_ximpl->level) return 1;
    if (m_ximpl->zoffset < fa.m_ximpl->zoffset) return -1;
    if (m_ximpl->zoffset > fa.m_ximpl->zoffset) return 1;
    return 0;
}

/**************************************************************************
 * crFrameBufferObject
 **************************************************************************/

///////////////////////////////////////////////////////////////////////////
// static cache of glRenderbuffers flagged for deletion, which will actually
// be deleted in the correct GL context.

typedef std::list<GLuint> FrameBufferObjectHandleList;
typedef CRCore::buffered_object<FrameBufferObjectHandleList> DeletedFrameBufferObjectCache;

static crMutex    s_mutex_deletedFrameBufferObjectCache;
static DeletedFrameBufferObjectCache s_deletedFrameBufferObjectCache;

void crFrameBufferObject::deleteFrameBufferObject(unsigned int contextID, GLuint rb)
{
    if( rb )
    {
        CRCore::ScopedLock<crMutex> lock(s_mutex_deletedFrameBufferObjectCache);

        // add glProgram to the cache for the appropriate context.
        s_deletedFrameBufferObjectCache[contextID].push_back(rb);
    }
}

void crFrameBufferObject::flushDeletedFrameBufferObjects(unsigned int contextID,double /*currentTime*/, double& availableTime)
{
    // if no time available don't try to flush objects.
    if (availableTime<=0.0) return;

    const crFBOExtensions* extensions = crFBOExtensions::instance(contextID,true);
    if(!extensions || !extensions->isSupported() ) return;

    const CRCore::Timer& timer = *CRCore::Timer::instance();
    CRCore::Timer_t start_tick = timer.tick();
    double elapsedTime = 0.0;

    {
        CRCore::ScopedLock<crMutex> lock(s_mutex_deletedFrameBufferObjectCache);

        FrameBufferObjectHandleList& pList = s_deletedFrameBufferObjectCache[contextID];
        for(FrameBufferObjectHandleList::iterator titr=pList.begin();
            titr!=pList.end() && elapsedTime<availableTime;
            )
        {
            extensions->glDeleteFramebuffersEXT(1, &(*titr) );
            titr = pList.erase( titr );
            elapsedTime = timer.delta_s(start_tick,timer.tick());
        }
    }
	//extensions->glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
    availableTime -= elapsedTime;
}
void crFrameBufferObject::flushAllDeletedFrameBufferObjects(unsigned int contextID)
{
	const crFBOExtensions* extensions = crFBOExtensions::instance(contextID,true);
	if(!extensions || !extensions->isSupported() ) return;

	{
		CRCore::ScopedLock<crMutex> lock(s_mutex_deletedFrameBufferObjectCache);

		FrameBufferObjectHandleList& pList = s_deletedFrameBufferObjectCache[contextID];
		for(FrameBufferObjectHandleList::iterator titr=pList.begin();
			titr!=pList.end();
			)
		{
			extensions->glDeleteFramebuffersEXT(1, &(*titr) );
			titr = pList.erase( titr );
		}
	}
	//extensions->glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
}

void crFrameBufferObject::discardDeletedFrameBufferObjects(unsigned int contextID)
{
    CRCore::ScopedLock<crMutex> lock(s_mutex_deletedFrameBufferObjectCache);
    FrameBufferObjectHandleList& pList = s_deletedFrameBufferObjectCache[contextID];

    pList.clear();
}



crFrameBufferObject::crFrameBufferObject()
:    crStateAttribute()
{
	char gbuf[256];
	sprintf(gbuf,"crFrameBufferObject created\n\0");
	gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
}

crFrameBufferObject::crFrameBufferObject(const crFrameBufferObject &copy, const crCopyOp &copyop)
:    crStateAttribute(copy, copyop),
    m_attachments(copy.m_attachments),
    m_drawBuffers(copy.m_drawBuffers)
{
	char gbuf[256];
	sprintf(gbuf,"crFrameBufferObject cloned\n\0");
	gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
}

crFrameBufferObject::~crFrameBufferObject()
{
	for(unsigned i=0; i<m_fboID.size(); ++i)
	{
		if (m_fboID[i]) deleteFrameBufferObject(i, m_fboID[i]);
	}
	char gbuf[256];
	sprintf(gbuf,"crFrameBufferObject deleted\n\0");
	gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
}

void crFrameBufferObject::releaseObjects(CRCore::crState* state)
{
	// TODO
	for (AttachmentMap::iterator i=m_attachments.begin(); i!=m_attachments.end(); ++i)
	{
		crFrameBufferAttachment &fa = i->second;
		fa.releaseObjects(state);
	}
	m_attachments.clear();
	for(unsigned i=0; i<m_fboID.size(); ++i)
	{
		if (m_fboID[i]) deleteFrameBufferObject(i, m_fboID[i]);
	}
	m_fboID.clear();
}

void crFrameBufferObject::setAttachment(BufferComponent attachment_point, const crFrameBufferAttachment &attachment)
{
    GLenum gl_attachment = convertBufferComponentToGLenum(attachment_point);
    m_attachments[attachment_point] = attachment;

    updateDrawBuffers();
    dirtyAll();
}


GLenum crFrameBufferObject::convertBufferComponentToGLenum(BufferComponent attachment_point) const
{
    switch(attachment_point)
    {
        case(crCameraNode::DEPTH_BUFFER): return GL_DEPTH_ATTACHMENT_EXT;
        case(crCameraNode::STENCIL_BUFFER): return GL_STENCIL_ATTACHMENT_EXT;
        case(crCameraNode::COLOR_BUFFER): return GL_COLOR_ATTACHMENT0_EXT;
        default: return GLenum(GL_COLOR_ATTACHMENT0_EXT + (attachment_point-crCameraNode::COLOR_BUFFER0));
    }
}

void crFrameBufferObject::updateDrawBuffers()
{
    m_drawBuffers.clear();

    // create textures and mipmaps before we bind the frame buffer object
    for (AttachmentMap::const_iterator i=m_attachments.begin(); i!=m_attachments.end(); ++i)
    {
        const crFrameBufferAttachment &fa = i->second;

        // setup draw buffers based on the attachment definition
        if (i->first >= crCameraNode::COLOR_BUFFER0 && i->first <= crCameraNode::COLOR_BUFFER15)
            m_drawBuffers.push_back(convertBufferComponentToGLenum(i->first));
    }
}

void crFrameBufferObject::apply(crState &state) const
{
    apply(state, READ_DRAW_FRAMEBUFFER);
}

void crFrameBufferObject::apply(crState &state, BindTarget target) const
{
    unsigned int contextID = state.getContextID();

    if (m_unsupported[contextID])
        return;
        
        
    crFBOExtensions* ext = crFBOExtensions::instance(contextID,true);
    if (!ext->isSupported())
    {
        m_unsupported[contextID] = 1;
        notify(WARN) << "Warning: EXT_framebuffer_object is not supported" << std::endl;
        return;
    }

    if (m_attachments.empty())
    {
        ext->glBindFramebufferEXT(target, 0);
        return;
    }

    int &dirtyAttachmentList = m_dirtyAttachmentList[contextID];

    GLuint &fboID = m_fboID[contextID];
    if (fboID == 0)
    {
        ext->glGenFramebuffersEXT(1, &fboID);
		char gbuf[256];
		sprintf(gbuf,"GenFBO %d\n\0",fboID);
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
        if (fboID == 0)
        {
            notify(WARN) << "Warning: crFrameBufferObject: could not create the FBO" << std::endl;
            return;
        }

        dirtyAttachmentList = 1;
		notify(CRCore::ALWAYS)<<"crFrameBufferObject::apply GenFBO:"<<fboID<< std::endl;
    }

    if (dirtyAttachmentList)
    {
        // the set of of attachments appears to be thread sensitive, it shouldn't be because 
        // OpenGL FBO handles CRCore::crFrameBufferObject has are multi-buffered...
        // so as a temporary fix will stick in a mutex to ensure that only one thread passes through here
        // at one time.
        static crMutex s_mutex;
        CRCore::ScopedLock<crMutex> lock(s_mutex);

        // create textures and mipmaps before we bind the frame buffer object
        for (AttachmentMap::const_iterator i=m_attachments.begin(); i!=m_attachments.end(); ++i)
        {
            const crFrameBufferAttachment &fa = i->second;
            fa.createRequiredTexturesAndApplyGenerateMipMap(state, ext);
        }

    }
    ext->glBindFramebufferEXT(target, fboID);
    // enable drawing buffers to render the result to fbo
    if (m_drawBuffers.size() > 0)
    {
        GL2Extensions *gl2e = GL2Extensions::Get(state.getContextID(), true );
        if (gl2e)
        {
            gl2e->glDrawBuffers(m_drawBuffers.size(), &(m_drawBuffers[0]));
        }
    }

    if (dirtyAttachmentList)
    {
        for (AttachmentMap::const_iterator i=m_attachments.begin(); i!=m_attachments.end(); ++i)
        {
            const crFrameBufferAttachment &fa = i->second;
            fa.attach(state, target, convertBufferComponentToGLenum(i->first), ext);
        }        
        dirtyAttachmentList = 0;
    }

}

bool crFrameBufferObject::isMultisample() const
{
    if (m_attachments.size())
    {
        // If the FBO is correctly set up then all attachments will be either
        // multisampled or single sampled. Therefore we can just return the
        // result of the first attachment.
        return m_attachments.begin()->second.isMultisample();
    }

    return false;
}

int crFrameBufferObject::compare(const crStateAttribute &sa) const
{
    COMPARE_StateAttribute_Types(crFrameBufferObject, sa);
    COMPARE_StateAttribute_Parameter(m_attachments.size());
    AttachmentMap::const_iterator i = m_attachments.begin();
    AttachmentMap::const_iterator j = rhs.m_attachments.begin();
    for (; i!=m_attachments.end(); ++i, ++j)
    {
        int cmp = i->second.compare(j->second);
        if (cmp != 0) return cmp;
    }
    return 0;
}

int crFrameBufferObject::compare(const crStateAttribute &sa)
{
	COMPARE_StateAttribute_Types(crFrameBufferObject, sa);
	COMPARE_StateAttribute_Parameter(m_attachments.size());
	AttachmentMap::const_iterator i = m_attachments.begin();
	AttachmentMap::const_iterator j = rhs.m_attachments.begin();
	for (; i!=m_attachments.end(); ++i, ++j)
	{
		int cmp = i->second.compare(j->second);
		if (cmp != 0) return cmp;
	}
	return 0;
}
