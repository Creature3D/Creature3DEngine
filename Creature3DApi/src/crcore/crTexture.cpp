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
#include <crgl/GLExtensions.h>
#include <CRCore/crImage.h>
#include <CRCore/crTexture.h>
#include <CRCore/crState.h>
#include <CRCore/crNotify.h>
#include <CRCore/crBrain.h>
#include <crgl/GLU.h>
#include <CRCore/crTimer.h>
#include <CRCore/crFrameBufferObject.h>
#include <CRCore/crStreamBuf.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRCore/thread/crMutex.h>

using namespace CRCore;

#ifndef GL_TEXTURE_WRAP_R
#define GL_TEXTURE_WRAP_R                 0x8072
#endif

#ifndef GL_TEXTURE_MAX_LEVEL
#define GL_TEXTURE_MAX_LEVEL              0x813D
#endif


#ifndef GL_UNPACK_CLIENT_STORAGE_APPLE
#define GL_UNPACK_CLIENT_STORAGE_APPLE    0x85B2
#endif

#ifndef GL_APPLE_vertex_array_range
#define GL_VERTEX_ARRAY_RANGE_APPLE       0x851D
#define GL_VERTEX_ARRAY_RANGE_LENGTH_APPLE 0x851E
#define GL_VERTEX_ARRAY_STORAGE_HINT_APPLE 0x851F
#define GL_VERTEX_ARRAY_RANGE_POINTER_APPLE 0x8521
#define GL_STORAGE_CACHED_APPLE           0x85BE
#define GL_STORAGE_SHARED_APPLE           0x85BF
#endif

//#define DO_TIMING

unsigned int crTexture::s_numberTextureReusedLastInLastFrame = 0;
unsigned int crTexture::s_numberTextures = 0;
//unsigned int crTexture::s_numberDeletedTextureInLastFrame = 0;
unsigned int crTexture::s_minimumNumberOfTextureObjectsToRetainInCache = 0;
ref_ptr<ImageDataRequestHandler> crTexture::s_imageDataRequestHandler = NULL;

typedef buffered_value< ref_ptr<crTexture::Extensions> > BufferedExtensions;
static BufferedExtensions s_extensions;
void crTexture::Extensions::clearExtensions()
{
	s_extensions.clear();
}
void crTexture::setMinimumNumberOfTextureObjectsToRetainInCache(unsigned int minimum)
{
    s_minimumNumberOfTextureObjectsToRetainInCache = minimum;
}

unsigned int crTexture::getMinimumNumberOfTextureObjectsToRetainInCache()
{
    return s_minimumNumberOfTextureObjectsToRetainInCache;
}
////////////////////////////////////////
//
//crTextureObjectManager
//
ref_ptr<crTextureObjectManager> crTextureObjectManager::m_instance = NULL;

crTextureObjectManager* crTextureObjectManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crTextureObjectManager;
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}

void crTextureObjectManager::clear()
{
	//CRCore::ScopedLock<crMutex> lock(m_mutex);
	//m_instance->m_textureObjectListMap.clear();
	//m_instance = NULL;
}

crTexture::TextureObject* crTextureObjectManager::generateTextureObject(unsigned int /*contextID*/,GLenum target)
{
	//CRCore::notify(ALWAYS)<<"creating Textures"<< std::endl;
	++crTexture::s_numberTextures;
	GLuint id;
	glGenTextures( 1L, &id );

	//CRCore::notify(ALWAYS)<< "  gen Textures "<<id<< std::endl;
	return new crTexture::TextureObject(id,target);
}

//int crTextureObjectManager::s_number = 0;

crTexture::TextureObject* crTextureObjectManager::generateTextureObject(unsigned int /*contextID*/,
																			   GLenum    target,
																			   GLint     numMipmapLevels,
																			   GLenum    internalFormat,
																			   GLsizei   width,
																			   GLsizei   height,
																			   GLsizei   depth,
																			   GLint     border)
{
	//++m_number;
	++crTexture::s_numberTextures;
	// notify(NOTICE)<<"creating new texture object "<<s_number<<std::endl;

	// no useable texture object found so return 0
	//CRCore::notify(ALWAYS)<<"creating Textures"<< std::endl;
	GLuint id;
	glGenTextures( 1L, &id );
	//test
	//GLenum errorNo = glGetError();
	//if (errorNo!=GL_NO_ERROR)
	//{
	//	CRCore::notify(WARN)<<"crTexture2D::drawImplementation<Warning>: detected OpenGL error '"<<gluErrorString(errorNo)<<"'"<< std::endl;
	//}
	//CRCore::notify(ALWAYS)<< "  gen Textures "<<id<< std::endl;

	return new crTexture::TextureObject(id,target,numMipmapLevels,internalFormat,width,height,depth,border);
}

crTexture::TextureObject* crTextureObjectManager::reuseTextureObject(unsigned int contextID,
																			GLenum    target,
																			GLint     numMipmapLevels,
																			GLenum    internalFormat,
																			GLsizei   width,
																			GLsizei   height,
																			GLsizei   depth,
																			GLint     border)
{
	CRCore::ScopedLock<crMutex> lock(m_mutex);

	crTexture::TextureObjectList& tol = m_textureObjectListMap[contextID];
	CRCore::ref_ptr<crTexture::TextureObject> setItr;
	for(crTexture::TextureObjectList::iterator itr = tol.begin();
		itr != tol.end();
		++itr)
	{
		setItr = *itr;
		if (setItr->match(target,numMipmapLevels,internalFormat,width,height,depth,border))
		{
			// found usable texture object.
			//crTexture::TextureObject* textureObject = (*itr).release();
			tol.erase(itr);
			setItr->setStateInfo(0);
			// notify(NOTICE)<<"reusing texture object "<<std::endl;

			++crTexture::s_numberTextureReusedLastInLastFrame;

			return setItr.get();
		}
	}

	return 0;
}

bool crTextureObjectManager::reuseTextureObject(unsigned int contextID, crTexture::TextureObject *to)
{
	CRCore::ScopedLock<crMutex> lock(m_mutex);

	crTexture::TextureObjectList& tol = m_textureObjectListMap[contextID];
	crTexture::TextureObjectList::iterator itr = tol.find(to);
	if(itr!=tol.end())
	{
		tol.erase(itr);
		to->setStateInfo(0);
		++crTexture::s_numberTextureReusedLastInLastFrame;
		return true;
	}

	return false;
}


void crTextureObjectManager::addTextureObjects(crTexture::TextureObjectListMap& toblm)
{
	CRCore::ScopedLock<crMutex> lock(m_mutex);

	for(crTexture::TextureObjectListMap::iterator itr = toblm.begin();
		itr != toblm.end();
		++itr)
	{
		crTexture::TextureObjectList& tol = m_textureObjectListMap[itr->first];
		tol.insert(itr->second.begin(),itr->second.end());
	}
}

void crTextureObjectManager::addTextureObjectsFrom(crTexture& texture)
{
	CRCore::ScopedLock<crMutex> lock(m_mutex);

	texture.takeTextureObjects(m_textureObjectListMap);
}

void crTextureObjectManager::flushAllTextureObjects(unsigned int contextID)
{
	CRCore::ScopedLock<crMutex> lock(m_mutex);

	crTexture::TextureObjectListMap::iterator tmitr = m_textureObjectListMap.find(contextID);
	if (tmitr!=m_textureObjectListMap.end())
	{
		crTexture::TextureObjectList& tol = tmitr->second;

		CRCore::ref_ptr<crTexture::TextureObject> setItr;
		for(crTexture::TextureObjectList::iterator itr=tol.begin();
			itr!=tol.end();
			++itr)
		{
			//CRCore::notify(CRCore::ALWAYS)<<"flushAllTextureObjects "<< std::endl;
			setItr = *itr;
			glDeleteTextures( 1L, &(setItr->m_id));
			setItr->setStateInfo(2);
		}
		int delcount = tol.size();
		crTexture::s_numberTextures -= delcount;
		char gbuf[256];
		sprintf(gbuf,"É¾³ýÌùÍ¼Êý:%d,Ê£ÓàÌùÍ¼Êý:%d\n\0",delcount,crTexture::s_numberTextures);
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		tol.clear();
	}
}

void crTextureObjectManager::flushTextureObjects(unsigned int contextID,double currentTime, double& availableTime)
{
	// if no time available don't try to flush objects.
	if (availableTime<=0.0) return;

	unsigned int numObjectsDeleted = 0;
	unsigned int maxNumObjectsToDelete = 4;

	const CRCore::Timer& timer = *CRCore::Timer::instance();
	CRCore::Timer_t start_tick = timer.tick();
	double elapsedTime = 0.0;

	unsigned int numTexturesDeleted = 0;
	{    
		CRCore::ScopedLock<crMutex> lock(m_mutex);

		crTexture::TextureObjectListMap::iterator tmitr = m_textureObjectListMap.find(contextID);

		if (tmitr!=m_textureObjectListMap.end())
		{
			crTexture::TextureObjectList& tol = tmitr->second;

			// reset the time of any uninitialized objects.
			CRCore::ref_ptr<crTexture::TextureObject> setItr;
			crTexture::TextureObjectList::iterator itr;
			for(itr=tol.begin();
				itr!=tol.end();
				++itr)
			{
				setItr = *itr;
				if (setItr->m_timeStamp==0.0) setItr->m_timeStamp=currentTime;
			}

			double expiryTime = currentTime-m_expiryDelay;

			for(itr=tol.begin();
				itr!=tol.end() && elapsedTime<availableTime && tol.size()>crTexture::s_minimumNumberOfTextureObjectsToRetainInCache && numObjectsDeleted<maxNumObjectsToDelete;
				)
			{
				setItr = *itr;
				if (setItr->m_timeStamp<=expiryTime)
				{
					//--m_number;
					//++crTexture::s_numberDeletedTextureInLastFrame;
					--crTexture::s_numberTextures;
					//CRCore::notify(CRCore::ALWAYS)<<"flushTextureObjects "<< std::endl;

					glDeleteTextures( 1L, &(setItr->m_id));
					setItr->setStateInfo(2);
					itr = tol.erase(itr);
					++numTexturesDeleted;
					++numObjectsDeleted;
				}
				else
				{
					++itr;
				}
				elapsedTime = timer.delta_s(start_tick,timer.tick());
			}

		}
	}
	elapsedTime = timer.delta_s(start_tick,timer.tick());
	availableTime -= elapsedTime;
}

static crTextureObjectManager* getTextureObjectManager()
{
	//static ref_ptr<crTextureObjectManager> s_textureObjectManager;
	//if (!s_textureObjectManager) s_textureObjectManager = new crTextureObjectManager;
	//return s_textureObjectManager.get();
	return crTextureObjectManager::getInstance();
}

////////////////////////////////////////
//
//TextureObject
//
void crTexture::TextureObject::bind(double timeStamp)
{
	m_timeStamp = timeStamp;
	glBindTexture( m_target, m_id);
}
////////////////////////////////////////
//
//crTexture
//
crTexture::TextureObject* crTexture::generateTextureObject(unsigned int contextID,GLenum target)
{
    if (getTextureObjectManager()) return getTextureObjectManager()->generateTextureObject(contextID,target);
    else return 0;
}

crTexture::TextureObject* crTexture::generateTextureObject(unsigned int contextID,
                                             GLenum    target,
                                             GLint     numMipmapLevels,
                                             GLenum    internalFormat,
                                             GLsizei   width,
                                             GLsizei   height,
                                             GLsizei   depth,
                                             GLint     border)
{
    if (getTextureObjectManager())    
        return getTextureObjectManager()->reuseOrGenerateTextureObject(contextID,
                                             target,
                                             numMipmapLevels,
                                             internalFormat,
                                             width,
                                             height,
                                             depth,
                                             border);
    else
        return 0;
}                                             

void crTexture::flushAllDeletedTextureObjects(unsigned int contextID)
{
    if (getTextureObjectManager()) getTextureObjectManager()->flushAllTextureObjects(contextID);
}

void crTexture::flushDeletedTextureObjects(unsigned int contextID,double currentTime, double& availbleTime)
{
#if 0
	if (getTextureObjectManager() && crDisplaySettings::instance()->getRunMode()>crDisplaySettings::Editor) 
		getTextureObjectManager()->flushTextureObjects(contextID, currentTime, availbleTime);
#endif
}

crTexture::crTexture():
            m_wrap_s(REPEAT),
            m_wrap_t(REPEAT),
            m_wrap_r(REPEAT),
            m_min_filter(LINEAR_MIPMAP_LINEAR), // trilinear
            m_mag_filter(LINEAR),
            m_maxAnisotropy(1.0f),
            m_useHardwareMipMapGeneration(true),
            m_unrefImageDataAfterApply(true),
            m_clientStorageHint(false),
            m_resizeNonPowerOfTwoHint(false),
            m_borderColor(0.0, 0.0, 0.0, 0.0),
            m_borderWidth(0),
            m_internalFormatMode(USE_IMAGE_DATA_FORMAT),
            m_internalFormat(0),
            m_sourceFormat(0),
            m_sourceType(0),
            m_use_shadow_comparison(false),
            m_shadow_compare_func(LEQUAL),
            m_shadow_texture_mode(LUMINANCE),
            m_shadow_ambient(0),
            m_internalFormatType(NORMALIZED),
			m_needCompile(true),
			m_blendShadowEnable(false),
			m_alphaShadowEnable(false),
			m_useParallax(false),
			m_externTextureID(EXT_NONE)
{
}

crTexture::crTexture(const crTexture& text,const crCopyOp& copyop):
            crStateAttribute(text,copyop),
            m_wrap_s(text.m_wrap_s),
            m_wrap_t(text.m_wrap_t),
            m_wrap_r(text.m_wrap_r),
            m_min_filter(text.m_min_filter),
            m_mag_filter(text.m_mag_filter),
            m_maxAnisotropy(text.m_maxAnisotropy),
            m_useHardwareMipMapGeneration(text.m_useHardwareMipMapGeneration),
            m_unrefImageDataAfterApply(text.m_unrefImageDataAfterApply),
            m_clientStorageHint(text.m_clientStorageHint),
            m_resizeNonPowerOfTwoHint(text.m_resizeNonPowerOfTwoHint),
            m_borderColor(text.m_borderColor),
            m_borderWidth(text.m_borderWidth),
            m_internalFormatMode(text.m_internalFormatMode),
            m_internalFormat(text.m_internalFormat),
            m_sourceFormat(text.m_sourceFormat),
            m_sourceType(text.m_sourceType),
            m_use_shadow_comparison(text.m_use_shadow_comparison),
            m_shadow_compare_func(text.m_shadow_compare_func),
            m_shadow_texture_mode(text.m_shadow_texture_mode),
            m_shadow_ambient(text.m_shadow_ambient),
            m_internalFormatType(text.m_internalFormatType),
			m_imageNameID(text.m_imageNameID),
			m_blendShadowEnable(text.m_blendShadowEnable),
			m_alphaShadowEnable(text.m_alphaShadowEnable),
			m_useParallax(text.m_useParallax),
			m_externTextureID(text.m_externTextureID)
{
}

crTexture::~crTexture()
{
    // delete old texture objects.
	dirtyTextureObject();
}

int crTexture::compareTexture(const crTexture& rhs) const
{
    COMPARE_StateAttribute_Parameter(m_wrap_s)
    COMPARE_StateAttribute_Parameter(m_wrap_t)
    COMPARE_StateAttribute_Parameter(m_wrap_r)
    COMPARE_StateAttribute_Parameter(m_min_filter)
    COMPARE_StateAttribute_Parameter(m_mag_filter)
    COMPARE_StateAttribute_Parameter(m_maxAnisotropy)
    COMPARE_StateAttribute_Parameter(m_useHardwareMipMapGeneration)
    COMPARE_StateAttribute_Parameter(m_internalFormatMode)

    // only compare _internalFomat is it has alrady been set in both lhs, and rhs
    if (m_internalFormat!=0 && rhs.m_internalFormat!=0)
    {
        COMPARE_StateAttribute_Parameter(m_internalFormat)
    }

    COMPARE_StateAttribute_Parameter(m_sourceFormat)
    COMPARE_StateAttribute_Parameter(m_sourceType)

    COMPARE_StateAttribute_Parameter(m_use_shadow_comparison)
    COMPARE_StateAttribute_Parameter(m_shadow_compare_func)
    COMPARE_StateAttribute_Parameter(m_shadow_texture_mode)
    COMPARE_StateAttribute_Parameter(m_shadow_ambient)

    COMPARE_StateAttribute_Parameter(m_unrefImageDataAfterApply)
    COMPARE_StateAttribute_Parameter(m_clientStorageHint)
    COMPARE_StateAttribute_Parameter(m_resizeNonPowerOfTwoHint)

    COMPARE_StateAttribute_Parameter(m_internalFormatType)

	COMPARE_StateAttribute_Parameter(m_blendShadowEnable)
	COMPARE_StateAttribute_Parameter(m_alphaShadowEnable)
	COMPARE_StateAttribute_Parameter(m_useParallax)
	COMPARE_StateAttribute_Parameter(m_externTextureID)
    return 0;
}

int crTexture::compareTexture(const crTexture& rhs)
{
	COMPARE_StateAttribute_Parameter(m_wrap_s)
	COMPARE_StateAttribute_Parameter(m_wrap_t)
	COMPARE_StateAttribute_Parameter(m_wrap_r)
	COMPARE_StateAttribute_Parameter(m_min_filter)
	COMPARE_StateAttribute_Parameter(m_mag_filter)
	COMPARE_StateAttribute_Parameter(m_maxAnisotropy)
	COMPARE_StateAttribute_Parameter(m_useHardwareMipMapGeneration)
	COMPARE_StateAttribute_Parameter(m_internalFormatMode)

	// only compare _internalFomat is it has alrady been set in both lhs, and rhs
	if (m_internalFormat!=0 && rhs.m_internalFormat!=0)
	{
		COMPARE_StateAttribute_Parameter(m_internalFormat)
	}

	COMPARE_StateAttribute_Parameter(m_sourceFormat)
	COMPARE_StateAttribute_Parameter(m_sourceType)

	COMPARE_StateAttribute_Parameter(m_use_shadow_comparison)
	COMPARE_StateAttribute_Parameter(m_shadow_compare_func)
	COMPARE_StateAttribute_Parameter(m_shadow_texture_mode)
	COMPARE_StateAttribute_Parameter(m_shadow_ambient)

	COMPARE_StateAttribute_Parameter(m_unrefImageDataAfterApply)
	COMPARE_StateAttribute_Parameter(m_clientStorageHint)
	COMPARE_StateAttribute_Parameter(m_resizeNonPowerOfTwoHint)

	COMPARE_StateAttribute_Parameter(m_internalFormatType)

	COMPARE_StateAttribute_Parameter(m_blendShadowEnable)
	COMPARE_StateAttribute_Parameter(m_alphaShadowEnable)
	COMPARE_StateAttribute_Parameter(m_useParallax)
	COMPARE_StateAttribute_Parameter(m_externTextureID)
	return 0;
}

int crTexture::compareTextureObjects(const crTexture& rhs) const
{
    if (m_textureObjectBuffer.size()<rhs.m_textureObjectBuffer.size()) return -1; 
    if (rhs.m_textureObjectBuffer.size()<m_textureObjectBuffer.size()) return 1;
    for(unsigned int i=0; i<m_textureObjectBuffer.size(); ++i)
    {
        if (m_textureObjectBuffer[i] < rhs.m_textureObjectBuffer[i]) return -1;
        else if (rhs.m_textureObjectBuffer[i] < m_textureObjectBuffer[i]) return 1;
    }
    return 0;
}

int crTexture::compareTextureObjects(const crTexture& rhs)
{
	if (m_textureObjectBuffer.size()<rhs.m_textureObjectBuffer.size()) return -1; 
	if (rhs.m_textureObjectBuffer.size()<m_textureObjectBuffer.size()) return 1;
	for(unsigned int i=0; i<m_textureObjectBuffer.size(); ++i)
	{
		if (m_textureObjectBuffer[i] < rhs.m_textureObjectBuffer[i]) return -1;
		else if (rhs.m_textureObjectBuffer[i] < m_textureObjectBuffer[i]) return 1;
	}
	return 0;
}


void crTexture::setWrap(WrapParameter which, WrapMode wrap)
{
    switch( which )
    {
        case WRAP_S : m_wrap_s = wrap; dirtyTextureParameters(); break;
        case WRAP_T : m_wrap_t = wrap; dirtyTextureParameters(); break;
        case WRAP_R : m_wrap_r = wrap; dirtyTextureParameters(); break;
        default : notify(WARN)<<"Error: invalid 'which' passed crTexture::setWrap("<<(unsigned int)which<<","<<(unsigned int)wrap<<")"<<std::endl; break;
    }
    
}


crTexture::WrapMode crTexture::getWrap(WrapParameter which) const
{
    switch( which )
    {
        case WRAP_S : return m_wrap_s;
        case WRAP_T : return m_wrap_t;
        case WRAP_R : return m_wrap_r;
        default : notify(WARN)<<"Error: invalid 'which' passed crTexture::getWrap(which)"<<std::endl; return m_wrap_s;
    }
}


void crTexture::setFilter(FilterParameter which, FilterMode filter)
{
    switch( which )
    {
        case MIN_FILTER : m_min_filter = filter; dirtyTextureParameters(); break;
        case MAG_FILTER : m_mag_filter = filter; dirtyTextureParameters(); break;
        default : notify(WARN)<<"Error: invalid 'which' passed crTexture::setFilter("<<(unsigned int)which<<","<<(unsigned int)filter<<")"<<std::endl; break;
    }
}


crTexture::FilterMode crTexture::getFilter(FilterParameter which) const
{
    switch( which )
    {
        case MIN_FILTER : return m_min_filter;
        case MAG_FILTER : return m_mag_filter;
        default : notify(WARN)<<"Error: invalid 'which' passed crTexture::getFilter(which)"<< std::endl; return m_min_filter;
    }
}

void crTexture::setMaxAnisotropy(float anis)
{
    if (m_maxAnisotropy!=anis)
    {
		//notify(WARN)<<"crTexture::setMaxAnisotropy "<<anis<< std::endl;
        m_maxAnisotropy = anis; 
        dirtyTextureParameters();
    }
}


/** Force a recompile on next apply() of associated OpenGL texture objects.*/
void crTexture::dirtyTextureObject()
{
    if (getTextureObjectManager()) getTextureObjectManager()->addTextureObjectsFrom(*this);
}

bool crTexture::reuseTextureObject(unsigned int contextID, TextureObject *textureObject) const
{
	switch(textureObject->getStateInfo()) 
	{
	case 0:
		break;
	case 1:
		getTextureObjectManager()->reuseTextureObject(contextID, textureObject);
		break;
	case 2:
		// m_textureObjectBuffer[contextID] = 0;	
		return false;
	}
	return true;
}

void crTexture::takeTextureObjects(crTexture::TextureObjectListMap& toblm)
{
	if(!m_textureObjectBuffer.empty())
	{
		TextureObject *to;
		for(int i = 0; i<m_textureObjectBuffer.size();++i)
		{
			to = m_textureObjectBuffer[i].get();
			if (to) 
			{
				//notify(INFO) << "releasing texure "<<toblm[i].size()<<std::endl;
				toblm[i].insert(to);
				to->setStateInfo(1);
			}
		}
		m_textureObjectBuffer.clear();
	}
}

void crTexture::dirtyTextureParameters()
{
    m_texParametersDirtyList.setAllElementsTo(1);
}

void crTexture::allocateMipmapLevels()
{
    m_texMipmapGenerationDirtyList.setAllElementsTo(1);
}

void crTexture::computeInternalFormatWithImage(const CRCore::crImage& image) const
{
    GLint internalFormat = image.getInternalTextureFormat();

    if (m_internalFormatMode==USE_IMAGE_DATA_FORMAT)
    {
        internalFormat = image.getInternalTextureFormat();
    }
    else if (m_internalFormatMode==USE_USER_DEFINED_FORMAT)
    {
        internalFormat = m_internalFormat;
    }
    else
    {

        const unsigned int contextID = 0; // state.getContextID();  // set to 0 right now, assume same parameters for each graphics context...
        const Extensions* extensions = getExtensions(contextID,true);

        switch(m_internalFormatMode)
        {
        case(USE_ARB_COMPRESSION):
            if (extensions->isTextureCompressionARBSupported())
            {
                switch(image.getPixelFormat())
                {
                    case(1): internalFormat = GL_COMPRESSED_ALPHA_ARB; break;
                    case(2): internalFormat = GL_COMPRESSED_LUMINANCE_ALPHA_ARB; break;
                    case(3): internalFormat = GL_COMPRESSED_RGB_ARB; break;
                    case(4): internalFormat = GL_COMPRESSED_RGBA_ARB; break;
                    case(GL_RGB): internalFormat = GL_COMPRESSED_RGB_ARB; break;
                    case(GL_RGBA): internalFormat = GL_COMPRESSED_RGBA_ARB; break;
                    case(GL_ALPHA): internalFormat = GL_COMPRESSED_ALPHA_ARB; break;
                    case(GL_LUMINANCE): internalFormat = GL_COMPRESSED_LUMINANCE_ARB; break;
                    case(GL_LUMINANCE_ALPHA): internalFormat = GL_COMPRESSED_LUMINANCE_ALPHA_ARB; break;
                    case(GL_INTENSITY): internalFormat = GL_COMPRESSED_INTENSITY_ARB; break;
                }
            }
            else internalFormat = image.getInternalTextureFormat();
            break;

        case(USE_S3TC_DXT1_COMPRESSION):
            if (extensions->isTextureCompressionS3TCSupported())
            {
                switch(image.getPixelFormat())
                {
                    case(3):        internalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT; break;
                    case(4):        internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; break;
                    case(GL_RGB):   internalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT; break;
                    case(GL_RGBA):  internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; break;
                    default:        internalFormat = image.getInternalTextureFormat(); break;
                }
            }
            else internalFormat = image.getInternalTextureFormat();
            break;

        case(USE_S3TC_DXT3_COMPRESSION):
            if (extensions->isTextureCompressionS3TCSupported())
            {
                switch(image.getPixelFormat())
                {
                    case(3):
                    case(GL_RGB):   internalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT; break;
                    case(4):
                    case(GL_RGBA):  internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; break;
                    default:        internalFormat = image.getInternalTextureFormat(); break;
                }
            }
            else internalFormat = image.getInternalTextureFormat();
            break;

        case(USE_S3TC_DXT5_COMPRESSION):
            if (extensions->isTextureCompressionS3TCSupported())
            {
                switch(image.getPixelFormat())
                {
                    case(3):
                    case(GL_RGB):   internalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT; break;
                    case(4):
                    case(GL_RGBA):  internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break;
                    default:        internalFormat = image.getInternalTextureFormat(); break;
                }
            }
            else internalFormat = image.getInternalTextureFormat();
            break;
        default:
            break;
        }
    }
    
    m_internalFormat = internalFormat;
    computeInternalFormatType();
    
    //CRCore::notify(CRCore::NOTICE)<<"Internal format="<<std::hex<<internalFormat<<std::dec<<std::endl;
}

void crTexture::computeInternalFormatType() const
{
    // Here we could also precompute the m_sourceFormat if it is not set,
    // since it is different for different internal formats
    // (i.e. rgba integer texture --> m_sourceFormat = GL_RGBA_INTEGER_EXT)
    // Should we do this?  ( Art, 09. Sept. 2007)
    
    // compute internal format type based on the internal format
    switch(m_internalFormat)
    {
        case GL_RGBA32UI_EXT:
        case GL_RGBA16UI_EXT:
        case GL_RGBA8UI_EXT:

        case GL_RGB32UI_EXT:
        case GL_RGB16UI_EXT:
        case GL_RGB8UI_EXT:

        case GL_LUMINANCE32UI_EXT:  
        case GL_LUMINANCE16UI_EXT:   
        case GL_LUMINANCE8UI_EXT:    

        case GL_INTENSITY32UI_EXT:    
        case GL_INTENSITY16UI_EXT:    
        case GL_INTENSITY8UI_EXT:   

        case GL_LUMINANCE_ALPHA32UI_EXT:    
        case GL_LUMINANCE_ALPHA16UI_EXT:    
        case GL_LUMINANCE_ALPHA8UI_EXT :   
            m_internalFormatType = UNSIGNED_INTEGER;
            break;

        case GL_RGBA32I_EXT:
        case GL_RGBA16I_EXT:
        case GL_RGBA8I_EXT:

        case GL_RGB32I_EXT:
        case GL_RGB16I_EXT:
        case GL_RGB8I_EXT:

        case GL_LUMINANCE32I_EXT:    
        case GL_LUMINANCE16I_EXT:    
        case GL_LUMINANCE8I_EXT:    

        case GL_INTENSITY32I_EXT:    
        case GL_INTENSITY16I_EXT:    
        case GL_INTENSITY8I_EXT:    

        case GL_LUMINANCE_ALPHA32I_EXT:    
        case GL_LUMINANCE_ALPHA16I_EXT:    
        case GL_LUMINANCE_ALPHA8I_EXT:    
            m_internalFormatType = SIGNED_INTEGER;
            break;
        
        case GL_RGBA32F_ARB:
        case GL_RGBA16F_ARB:

        case GL_RGB32F_ARB:
        case GL_RGB16F_ARB:

        case GL_LUMINANCE32F_ARB:
        case GL_LUMINANCE16F_ARB:    

        case GL_INTENSITY32F_ARB:
        case GL_INTENSITY16F_ARB:    

        case GL_LUMINANCE_ALPHA32F_ARB:
        case GL_LUMINANCE_ALPHA16F_ARB:    
            m_internalFormatType = FLOAT;
            break;
            
        default:
            m_internalFormatType = NORMALIZED;
            break;
    };

	crTexture* non_const_tex = const_cast<crTexture*>(this);
	switch(m_internalFormat) 
	{
	case GL_INTENSITY8:
		non_const_tex->setSourceFormat(GL_LUMINANCE);
		non_const_tex->setSourceType(GL_UNSIGNED_BYTE);
		break;
	case GL_LUMINANCE8_ALPHA8:
		non_const_tex->setSourceFormat(GL_LUMINANCE_ALPHA);
		non_const_tex->setSourceType(GL_UNSIGNED_BYTE);
		break;
	case GL_RGB8:
		non_const_tex->setSourceFormat(GL_RGB);
		non_const_tex->setSourceType(GL_UNSIGNED_BYTE);
		break;
	case GL_RGBA8:
		non_const_tex->setSourceFormat(GL_RGBA);
		non_const_tex->setSourceType(GL_UNSIGNED_BYTE);
		break;
	case GL_INTENSITY16:
		non_const_tex->setSourceFormat(GL_LUMINANCE);
		non_const_tex->setSourceType(GL_UNSIGNED_SHORT);
		break;
	case GL_LUMINANCE16_ALPHA16:
		non_const_tex->setSourceFormat(GL_LUMINANCE_ALPHA);
		non_const_tex->setSourceType(GL_UNSIGNED_SHORT);
		break;
	case GL_RGB16:
		non_const_tex->setSourceFormat(GL_RGB);
		non_const_tex->setSourceType(GL_UNSIGNED_SHORT);
		break;
	case GL_RGBA16:
		non_const_tex->setSourceFormat(GL_RGBA);
		non_const_tex->setSourceType(GL_UNSIGNED_SHORT);
		break;
	//case GL_R_FLOAT16_ATI:
	//	non_const_tex->setSourceFormat(GL_RED);
	//	non_const_tex->setSourceType(GL_HALF_FLOAT_ARB);
	//	break;
	//case GL_RG_FLOAT16_ATI:
	//	non_const_tex->setSourceFormat(GL_RG_ATI);
	//	non_const_tex->setSourceType(GL_HALF_FLOAT_ARB);
	//	break;
	case GL_RGB_FLOAT16_ATI:
		non_const_tex->setSourceFormat(GL_RGB);
		non_const_tex->setSourceType(GL_HALF_FLOAT_ARB);
		break;
	case GL_RGBA_FLOAT16_ATI:
		non_const_tex->setSourceFormat(GL_RGBA);
		non_const_tex->setSourceType(GL_HALF_FLOAT_ARB);
		break;
	case GL_INTENSITY_FLOAT16_ATI:
		non_const_tex->setSourceFormat(GL_LUMINANCE);
		non_const_tex->setSourceType(GL_HALF_FLOAT_ARB);
		break;
	case GL_LUMINANCE_ALPHA_FLOAT16_ATI:
		non_const_tex->setSourceFormat(GL_LUMINANCE_ALPHA);
		non_const_tex->setSourceType(GL_HALF_FLOAT_ARB);
		break;
	//case GL_R_FLOAT32_ATI:
	//	non_const_tex->setSourceFormat(GL_RED);
	//	non_const_tex->setSourceType(GL_FLOAT);
	//	break;
	//case GL_RG_FLOAT32_ATI:
	//	non_const_tex->setSourceFormat(GL_RG_ATI);
	//	non_const_tex->setSourceType(GL_FLOAT);
	//	break;
	case GL_RGB_FLOAT32_ATI:
		non_const_tex->setSourceFormat(GL_RGB);
		non_const_tex->setSourceType(GL_FLOAT);
		break;
	case GL_RGBA_FLOAT32_ATI:
		non_const_tex->setSourceFormat(GL_RGBA);
		non_const_tex->setSourceType(GL_FLOAT);
		break;
	case GL_INTENSITY_FLOAT32_ATI:
		non_const_tex->setSourceFormat(GL_LUMINANCE);
		non_const_tex->setSourceType(GL_FLOAT);
		break;
	case GL_LUMINANCE_ALPHA_FLOAT32_ATI:
		non_const_tex->setSourceFormat(GL_LUMINANCE_ALPHA);
		non_const_tex->setSourceType(GL_FLOAT);
		break;
	case GL_DEPTH_COMPONENT16:
		non_const_tex->setSourceFormat(GL_DEPTH_COMPONENT);
		non_const_tex->setSourceType(GL_UNSIGNED_SHORT);
		break;
	case GL_DEPTH_COMPONENT24:
		non_const_tex->setSourceFormat(GL_DEPTH_COMPONENT);
		non_const_tex->setSourceType(GL_UNSIGNED_INT);
		break;
	case GL_R3_G3_B2:
		non_const_tex->setSourceFormat(GL_RGB);
		non_const_tex->setSourceType(GL_UNSIGNED_BYTE_3_3_2);
		break;
	case GL_RGB10_A2:
		non_const_tex->setSourceFormat(GL_RGBA);
		non_const_tex->setSourceType(GL_UNSIGNED_INT_2_10_10_10_REV);
		break;
	case GL_COMPRESSED_RGB_S3TC_DXT1_EXT://// Compressed formats don't need a source type
	case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
	case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
	//case GL_COMPRESSED_LUMINANCE_ALPHA_3DC_ATI: // ATI2N
		break;
	}

	if(non_const_tex->getSourceType()==GL_HALF_FLOAT_ARB)
		non_const_tex->setSourceType(GL_FLOAT);
}

bool crTexture::isCompressedInternalFormat() const
{
    return isCompressedInternalFormat(getInternalFormat());
}

bool crTexture::isCompressedInternalFormat(GLint internalFormat)
{
    switch(internalFormat)
    {
        case(GL_COMPRESSED_ALPHA_ARB):
        case(GL_COMPRESSED_INTENSITY_ARB):
        case(GL_COMPRESSED_LUMINANCE_ALPHA_ARB):
        case(GL_COMPRESSED_LUMINANCE_ARB):
        case(GL_COMPRESSED_RGBA_ARB):
        case(GL_COMPRESSED_RGB_ARB):
        case(GL_COMPRESSED_RGB_S3TC_DXT1_EXT):
        case(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT):
        case(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT):
        case(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT):
            return true;
        default:
            return false;
    }
}

void crTexture::getCompressedSize(GLenum internalFormat, GLint width, GLint height, GLint depth, GLint& blockSize, GLint& size)
{
    if (internalFormat == GL_COMPRESSED_RGB_S3TC_DXT1_EXT || internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
        blockSize = 8;
    else if (internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
        blockSize = 16;
    else
    {
        notify(WARN)<<"crTexture::getCompressedSize(...) : cannot compute correct size of compressed format ("<<internalFormat<<") returning 0."<<std::endl;
        blockSize = 0;
    }
         
    size = ((width+3)/4)*((height+3)/4)*depth*blockSize;        
}

void crTexture::applyTexParameters(GLenum target, crState& state) const
{
    // get the contextID (user defined ID of 0 upwards) for the 
    // current OpenGL context.
    const unsigned int contextID = state.getContextID();
    const Extensions* extensions = getExtensions(contextID,true);

    WrapMode ws = m_wrap_s, wt = m_wrap_t, wr = m_wrap_r;
	//notify(NOTICE)<<"ws = "<<ws<<" wt = "<<wt<<" wr = "<<wr<<std::endl;

    // GL_IBM_texture_mirrored_repeat, fall-back REPEAT
    if (!extensions->isTextureMirroredRepeatSupported())
    {
        if (ws == MIRROR)
            ws = REPEAT;
        if (wt == MIRROR)
            wt = REPEAT;
        if (wr == MIRROR)
            wr = REPEAT;
    }

    // GL_EXT_texture_edge_clamp, fall-back CLAMP
    if (!extensions->isTextureEdgeClampSupported())
    {
        if (ws == CLAMP_TO_EDGE)
            ws = CLAMP;
        if (wt == CLAMP_TO_EDGE)
            wt = CLAMP;
        if (wr == CLAMP_TO_EDGE)
            wr = CLAMP;
    }

    if(!extensions->isTextureBorderClampSupported())
    {
        if(ws == CLAMP_TO_BORDER)
            ws = CLAMP;
        if(wt == CLAMP_TO_BORDER)
            wt = CLAMP;
        if(wr == CLAMP_TO_BORDER)
            wr = CLAMP;
    }

    glTexParameteri( target, GL_TEXTURE_WRAP_S, ws );
    
    if (target!=GL_TEXTURE_1D) glTexParameteri( target, GL_TEXTURE_WRAP_T, wt );
    
    if (target==GL_TEXTURE_3D) glTexParameteri( target, GL_TEXTURE_WRAP_R, wr );

    glTexParameteri( target, GL_TEXTURE_MIN_FILTER, m_min_filter);
    glTexParameteri( target, GL_TEXTURE_MAG_FILTER, m_mag_filter);
	//notify(NOTICE)<<"m_min_filter = "<<m_min_filter<<" m_mag_filter = "<<m_mag_filter<<std::endl;

    // Art: I think anisotropic filtering is not supported by the integer textures
    if (extensions->isTextureFilterAnisotropicSupported() &&
        m_internalFormatType != SIGNED_INTEGER && m_internalFormatType != UNSIGNED_INTEGER)
    {
        // note, GL_TEXTURE_MAX_ANISOTROPY_EXT will either be defined
        // by gl.h (or via glext.h) or by include/CRCore/crTexture.
        glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, /*m_maxAnisotropy*/crDisplaySettings::instance()->getMaxAnisotropy());
    }

    if (extensions->isTextureBorderClampSupported())
    {
        if (m_internalFormatType == SIGNED_INTEGER)
        {
            GLint color[4] = {(GLint)m_borderColor.r(), (GLint)m_borderColor.g(), (GLint)m_borderColor.b(), (GLint)m_borderColor.a()};
            extensions->glTexParameterIiv(target, GL_TEXTURE_BORDER_COLOR, color);
        }else if (m_internalFormatType == UNSIGNED_INTEGER)
        {
            GLuint color[4] = {(GLuint)m_borderColor.r(), (GLuint)m_borderColor.g(), (GLuint)m_borderColor.b(), (GLuint)m_borderColor.a()};
            extensions->glTexParameterIuiv(target, GL_TEXTURE_BORDER_COLOR, color);
        }else{
            GLfloat color[4] = {(GLfloat)m_borderColor.r(), (GLfloat)m_borderColor.g(), (GLfloat)m_borderColor.b(), (GLfloat)m_borderColor.a()};
            glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, color);
        }
    }

    // integer texture are not supported by the shadow
    if (extensions->isShadowSupported() && target == GL_TEXTURE_2D &&
        m_internalFormatType != SIGNED_INTEGER && m_internalFormatType != UNSIGNED_INTEGER)
    {
        if (m_use_shadow_comparison)
        {
            glTexParameteri(target, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
            glTexParameteri(target, GL_TEXTURE_COMPARE_FUNC_ARB, m_shadow_compare_func);
            glTexParameteri(target, GL_DEPTH_TEXTURE_MODE_ARB, m_shadow_texture_mode);

            // if ambient value is 0 - it is default behaviour of GL_ARB_shadow
            // no need for GL_ARB_shadow_ambient in this case
            if (extensions->isShadowAmbientSupported() && m_shadow_ambient > 0)
            {
                glTexParameterf(target, TEXTURE_COMPARE_FAIL_VALUE_ARB, m_shadow_ambient);
            }
        }
        else 
        {
            glTexParameteri(target, GL_TEXTURE_COMPARE_MODE_ARB, GL_NONE);
        }
    }

    getTextureParameterDirty(state.getContextID()) = false;

}

void crTexture::computeRequiredTextureDimensions(crState& state, const CRCore::crImage& image,GLsizei& inwidth, GLsizei& inheight,GLsizei& numMipmapLevels) const
{
    const unsigned int contextID = state.getContextID();
    const Extensions* extensions = getExtensions(contextID,true);

    int width,height;

    if( !m_resizeNonPowerOfTwoHint && extensions->isNonPowerOfTwoTextureSupported(m_min_filter) )
    {
        width = image.s();
        height = image.t();
    }
    else
    {
        width = crImage::computeNearestPowerOfTwo(image.s()-2*m_borderWidth)+2*m_borderWidth;
        height = crImage::computeNearestPowerOfTwo(image.t()-2*m_borderWidth)+2*m_borderWidth;
    }

    // cap the size to what the graphics hardware can handle.
    if (width>extensions->maxTextureSize()) width = extensions->maxTextureSize();
    if (height>extensions->maxTextureSize()) height = extensions->maxTextureSize();

    inwidth = width;
    inheight = height;
    
    bool useHardwareMipMapGeneration = !image.isMipmap() && isHardwareMipmapGenerationEnabled(state);

    if( m_min_filter == LINEAR || m_min_filter == NEAREST || useHardwareMipMapGeneration )
    {
        numMipmapLevels = 1;
    }
    else if( image.isMipmap() )
    {
        numMipmapLevels = image.getNumMipmapLevels();
    }
    else
    {
        numMipmapLevels = 0;
        for( ; (width || height) ;++numMipmapLevels)
        {

            if (width == 0)
                width = 1;
            if (height == 0)
                height = 1;

            width >>= 1;
            height >>= 1;
        }    
    }
}

void crTexture::computeRequiredTextureDimensions(crState& state, GLsizei& inwidth, GLsizei& inheight,GLsizei& numMipmapLevels ) const
{
	unsigned int contextID = state.getContextID();
	const Extensions* extensions = getExtensions(contextID,true);

	int width = inwidth;
	int height = inheight;
	int borderWidth = this->getBorderWidth();
	crTexture::FilterMode min_filter = this->getFilter(crTexture::MIN_FILTER);

	// cap the size to what the graphics hardware can handle.
	if (width>extensions->maxTextureSize()) width = extensions->maxTextureSize();
	if (height>extensions->maxTextureSize()) height = extensions->maxTextureSize();

	inwidth = width;
	inheight = height;

	bool useHardwareMipMapGeneration = this->getUseHardwareMipMapGeneration() && extensions->isGenerateMipMapSupported();

	if( min_filter == crTexture::LINEAR || min_filter == crTexture::NEAREST /*|| useHardwareMipMapGeneration*/ )
	{
		numMipmapLevels = 1;
	}
	else if(!extensions->isGenerateMipMapSupported())
	{
		numMipmapLevels = 1;	
	}
	else
	{
		numMipmapLevels = 0;
		for( ; (width || height) ;++numMipmapLevels)
		{

			if (width == 0)
				width = 1;
			if (height == 0)
				height = 1;

			width >>= 1;
			height >>= 1;
		}    
	}
}


bool crTexture::areAllTextureObjectsLoaded() const
{
    for(unsigned int i=0;i<crDisplaySettings::instance()->getMaxNumberOfGraphicsContexts();++i)
    {
        if (m_textureObjectBuffer[i]==0) return false;
    }
    return true;
}


void crTexture::applyTexImage2D_load(crState& state, GLenum target, const crImage* image, GLsizei inwidth, GLsizei inheight,GLsizei numMipmapLevels) const
{
    // if we don't have a valid image we can't create a texture!
    if (!image || !image->data())
        return;

#ifdef DO_TIMING
    CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();
    CRCore::notify(CRCore::NOTICE)<<"glTexImage2D pixelFormat = "<<std::hex<<image->getPixelFormat()<<std::dec<<std::endl;
#endif

    // get the contextID (user defined ID of 0 upwards) for the 
    // current OpenGL context.
    const unsigned int contextID = state.getContextID();
    const Extensions* extensions = getExtensions(contextID,true);

    // select the internalFormat required for the texture.
    bool compressed_image = isCompressedInternalFormat((GLenum)image->getPixelFormat());

    // If the texture's internal format is a compressed type, then the
    // user is requesting that the graphics card compress the image if it's
    // not already compressed. However, if the image is not a multiple of 
    // four in each dimension the subsequent calls to glTexSubImage* will
    // fail. Revert to uncompressed format in this case.
    if (isCompressedInternalFormat(m_internalFormat) &&
        (((inwidth >> 2) << 2) != inwidth ||
         ((inheight >> 2) << 2) != inheight))
    {
        //CRCore::notify(CRCore::NOTICE)<<"Received a request to compress an image, but image size is not a multiple of four ("<<inwidth<<"x"<<inheight<<"). Reverting to uncompressed.\n";
        switch(m_internalFormat)
        {
            case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
            case GL_COMPRESSED_RGB: m_internalFormat = GL_RGB; break;
            case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
            case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
            case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
            case GL_COMPRESSED_RGBA: m_internalFormat = GL_RGBA; break;
            case GL_COMPRESSED_ALPHA: m_internalFormat = GL_ALPHA; break;
            case GL_COMPRESSED_LUMINANCE: m_internalFormat = GL_LUMINANCE; break;
            case GL_COMPRESSED_LUMINANCE_ALPHA: m_internalFormat = GL_LUMINANCE_ALPHA; break;
            case GL_COMPRESSED_INTENSITY: m_internalFormat = GL_INTENSITY; break;
        }
    }
    
    glPixelStorei(GL_UNPACK_ALIGNMENT,image->getPacking());
    
    bool useClientStorage = extensions->isClientStorageSupported() && getClientStorageHint();
    if (useClientStorage)
    {
        glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE,GL_TRUE);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_PRIORITY,0.0f);
        
        #ifdef GL_TEXTURE_STORAGE_HINT_APPLE    
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_STORAGE_HINT_APPLE , GL_STORAGE_CACHED_APPLE);
        #endif
    }
    
    unsigned char* data = (unsigned char*)image->data();
 

    bool needImageRescale = inwidth!=image->s() || inheight!=image->t();
    if (needImageRescale)
    {

        // resize the image to power of two.
        
        if (image->isMipmap())
        {
            notify(WARN)<<"Warning:: Mipmapped CRCore::crImage not a power of two, cannot apply to texture."<<std::endl;
            return;
        }
        else if (compressed_image)
        {
            notify(WARN)<<"Warning:: Compressed CRCore::crImage not a power of two, cannot apply to texture."<<std::endl;
            return;
        }
        
        unsigned int newTotalSize = CRCore::crImage::computeRowWidthInBytes(inwidth,image->getPixelFormat(),image->getDataType(),image->getPacking())*inheight;
        data = new unsigned char [newTotalSize];
        
        if (!data)
        {
            notify(WARN)<<"Warning:: Not enough memory to resize image, cannot apply to texture."<<std::endl;
            return;
        }

        if (!image->getFileName().empty()) notify(NOTICE) << "Scaling image '"<<image->getFileName()<<"' from ("<<image->s()<<","<<image->t()<<") to ("<<inwidth<<","<<inheight<<")"<<std::endl;
        else notify(NOTICE) << "Scaling image from ("<<image->s()<<","<<image->t()<<") to ("<<inwidth<<","<<inheight<<")"<<std::endl;

        // rescale the image to the correct size.
        glPixelStorei(GL_PACK_ALIGNMENT,image->getPacking());
        gluScaleImage(image->getPixelFormat(),
                      image->s(),image->t(),image->getDataType(),image->data(),
                      inwidth,inheight,image->getDataType(),data);
        
    }    

    bool mipmappingRequired = m_min_filter != LINEAR && m_min_filter != NEAREST;
    bool useHardwareMipMapGeneration = mipmappingRequired && (!image->isMipmap() && isHardwareMipmapGenerationEnabled(state));
    bool useGluBuildMipMaps = mipmappingRequired && (!useHardwareMipMapGeneration && !image->isMipmap());

    unsigned char* dataMinusOffset = 0;
    unsigned char* dataPlusOffset = 0;

    const crPixelBufferObject* pbo = image->getPixelBufferObject();
    if (pbo && pbo->isPBOSupported(contextID) && !needImageRescale && !useGluBuildMipMaps)
    {
        state.bindPixelBufferObject(pbo);
        dataMinusOffset = data;
        dataPlusOffset = reinterpret_cast<unsigned char*>(pbo->offset());
#ifdef DO_TIMING
        CRCore::notify(CRCore::NOTICE)<<"after PBO "<<CRCore::Timer::instance()->delta_m(start_tick,CRCore::Timer::instance()->tick())<<"ms"<<std::endl;
#endif
    }
    else
    {
        pbo = 0;
    }

    if( !mipmappingRequired || useHardwareMipMapGeneration)
    {

        GenerateMipmapMode mipmapResult = mipmapBeforeTexImage(state, useHardwareMipMapGeneration);

        if ( !compressed_image)
        {
            numMipmapLevels = 1;

            glTexImage2D( target, 0, m_internalFormat,
                inwidth, inheight, m_borderWidth,
                (GLenum)image->getPixelFormat(),
                (GLenum)image->getDataType(),
                data -dataMinusOffset+dataPlusOffset);

        }
        else if (extensions->isCompressedTexImage2DSupported())
        {
            numMipmapLevels = 1;

            GLint blockSize, size;
            getCompressedSize(m_internalFormat, inwidth, inheight, 1, blockSize,size);

            extensions->glCompressedTexImage2D(target, 0, m_internalFormat, 
                inwidth, inheight,0, 
                size, 
                data-dataMinusOffset+dataPlusOffset);                
        }

        mipmapAfterTexImage(state, mipmapResult);
    }
    else
    {
        // we require mip mapping.
        if(image->isMipmap())
        {

            // image is mip mapped so we take the mip map levels from the image.
        
            numMipmapLevels = image->getNumMipmapLevels();

            int width  = inwidth;
            int height = inheight;

            if( !compressed_image )
            {
                for( GLsizei k = 0 ; k < numMipmapLevels  && (width || height) ;k++)
                {

                    if (width == 0)
                        width = 1;
                    if (height == 0)
                        height = 1;

                    glTexImage2D( target, k, m_internalFormat,
                         width, height, m_borderWidth,
                        (GLenum)image->getPixelFormat(),
                        (GLenum)image->getDataType(),
                        image->getMipmapData(k)-dataMinusOffset+dataPlusOffset);

                    width >>= 1;
                    height >>= 1;
                }
            }
            else if (extensions->isCompressedTexImage2DSupported())
            {
                GLint blockSize, size;

                for( GLsizei k = 0 ; k < numMipmapLevels  && (width || height) ;k++)
                {
                    if (width == 0)
                        width = 1;
                    if (height == 0)
                        height = 1;

                    getCompressedSize(m_internalFormat, width, height, 1, blockSize,size);
                    
                    extensions->glCompressedTexImage2D(target, k, m_internalFormat, 
                                                       width, height, m_borderWidth, 
                                                       size, image->getMipmapData(k)-dataMinusOffset+dataPlusOffset);                

                    width >>= 1;
                    height >>= 1;
                }
            }
        }
        else
        {
        
            if ( !compressed_image)
            {
                numMipmapLevels = 0;

                gluBuild2DMipmaps( target, m_internalFormat,
                    inwidth,inheight,
                    (GLenum)image->getPixelFormat(), (GLenum)image->getDataType(),
                    data);

                int width  = image->s();
                int height = image->t();
                for( numMipmapLevels = 0 ; (width || height) ; ++numMipmapLevels)
                {
                    width >>= 1;
                    height >>= 1;
                }
            }
            else 
            {
                notify(WARN)<<"Warning:: Compressed image cannot be mip mapped"<<std::endl;
            }

        }

    }

    if (pbo)
    {
        state.unbindPixelBufferObject();
    }
    
#ifdef DO_TIMING
    static double s_total_time = 0.0;
    double delta_time = CRCore::Timer::instance()->delta_m(start_tick,CRCore::Timer::instance()->tick());
    s_total_time += delta_time;
    CRCore::notify(CRCore::NOTICE)<<"glTexImage2D "<<delta_time<<"ms  total "<<s_total_time<<"ms"<<std::endl;
#endif

    if (needImageRescale)
    {
        // clean up the resized image.
        delete [] data;
    }
    
    if (useClientStorage)
    {
        glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE,GL_FALSE);
    }
}



void crTexture::applyTexImage2D_subload(crState& state, GLenum target, const crImage* image, GLsizei& inwidth, GLsizei& inheight, GLint& inInternalFormat, GLint& numMipmapLevels) const
{
    // if we don't have a valid image we can't create a texture!
    if (!image || !image->data())
        return;

    // image size has changed so we have to re-load the image from scratch.
    if (image->s()!=inwidth || image->t()!=inheight || image->getInternalTextureFormat()!=inInternalFormat ) 
    {
		computeInternalFormat();
		computeRequiredTextureDimensions(state,*image,inwidth, inheight, numMipmapLevels);
        applyTexImage2D_load(state, target, image, inwidth, inheight,numMipmapLevels); 
        return;
    }
    // else image size the same as when loaded so we can go ahead and subload
    
    // If the texture's internal format is a compressed type, then the
    // user is requesting that the graphics card compress the image if it's
    // not already compressed. However, if the image is not a multiple of 
    // four in each dimension the subsequent calls to glTexSubImage* will
    // fail. Revert to uncompressed format in this case.
    if (isCompressedInternalFormat(m_internalFormat) &&
        (((inwidth >> 2) << 2) != inwidth ||
         ((inheight >> 2) << 2) != inheight))
    {
		computeRequiredTextureDimensions(state,*image,inwidth, inheight, numMipmapLevels);
        applyTexImage2D_load(state, target, image, inwidth, inheight, numMipmapLevels);
        return;
    }

#ifdef DO_TIMING
    CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();
    CRCore::notify(CRCore::NOTICE)<<"glTexSubImage2D pixelFormat = "<<std::hex<<image->getPixelFormat()<<std::dec<<std::endl;
#endif
   

    // get the contextID (user defined ID of 0 upwards) for the 
    // current OpenGL context.
    const unsigned int contextID = state.getContextID();
    const Extensions* extensions = getExtensions(contextID,true);

    // select the internalFormat required for the texture.
    bool compressed_image = isCompressedInternalFormat((GLenum)image->getPixelFormat());
    
    glPixelStorei(GL_UNPACK_ALIGNMENT,image->getPacking());
    
    unsigned char* data = (unsigned char*)image->data();
 

    bool needImageRescale = inwidth!=image->s() || inheight!=image->t();
    if (needImageRescale)
    {

        // resize the image to power of two.
        
        if (image->isMipmap())
        {
            notify(WARN)<<"Warning:: Mipmapped CRCore::crImage not a power of two, cannot apply to texture."<<std::endl;
            return;
        }
        else if (compressed_image)
        {
            notify(WARN)<<"Warning:: Compressed CRCore::crImage not a power of two, cannot apply to texture."<<std::endl;
            return;
        }
        
        unsigned int newTotalSize = CRCore::crImage::computeRowWidthInBytes(inwidth,image->getPixelFormat(),image->getDataType(),image->getPacking())*inheight;
        data = new unsigned char [newTotalSize];
        
        if (!data)
        {
            notify(WARN)<<"Warning:: Not enough memory to resize image, cannot apply to texture."<<std::endl;
            return;
        }

        if (!image->getFileName().empty()) notify(NOTICE) << "Scaling image '"<<image->getFileName()<<"' from ("<<image->s()<<","<<image->t()<<") to ("<<inwidth<<","<<inheight<<")"<<std::endl;
        else notify(NOTICE) << "Scaling image from ("<<image->s()<<","<<image->t()<<") to ("<<inwidth<<","<<inheight<<")"<<std::endl;

        // rescale the image to the correct size.
        glPixelStorei(GL_PACK_ALIGNMENT,image->getPacking());
        gluScaleImage(image->getPixelFormat(),
                      image->s(),image->t(),image->getDataType(),image->data(),
                      inwidth,inheight,image->getDataType(),data);
        
    }    


    bool mipmappingRequired = m_min_filter != LINEAR && m_min_filter != NEAREST;
    bool useHardwareMipMapGeneration = mipmappingRequired && (!image->isMipmap() && isHardwareMipmapGenerationEnabled(state));
    bool useGluBuildMipMaps = mipmappingRequired && (!useHardwareMipMapGeneration && !image->isMipmap());

    unsigned char* dataMinusOffset = 0;
    unsigned char* dataPlusOffset = 0;
    
    const crPixelBufferObject* pbo = image->getPixelBufferObject();
    if (pbo && pbo->isPBOSupported(contextID) && !needImageRescale && !useGluBuildMipMaps)
    {
        state.bindPixelBufferObject(pbo);
        dataMinusOffset = data;
        dataPlusOffset = reinterpret_cast<unsigned char*>(pbo->offset());
#ifdef DO_TIMING
        CRCore::notify(CRCore::NOTICE)<<"after PBO "<<CRCore::Timer::instance()->delta_m(start_tick,CRCore::Timer::instance()->tick())<<"ms"<<std::endl;
#endif
    }
    else
    {
        pbo = 0;
    }

    if( !mipmappingRequired || useHardwareMipMapGeneration)
    {

        GenerateMipmapMode mipmapResult = mipmapBeforeTexImage(state, useHardwareMipMapGeneration);

        if (!compressed_image)
        {
            glTexSubImage2D( target, 0, 
                0, 0,
                inwidth, inheight,
                (GLenum)image->getPixelFormat(),
                (GLenum)image->getDataType(),
                data - dataMinusOffset + dataPlusOffset);

        }
        else if (extensions->isCompressedTexImage2DSupported())
        {        
            GLint blockSize,size;
            getCompressedSize(image->getInternalTextureFormat(), inwidth, inheight, 1, blockSize,size);

            extensions->glCompressedTexSubImage2D(target, 0, 
                0,0,
                inwidth, inheight,
                (GLenum)image->getPixelFormat(),
                size,
                data - dataMinusOffset + dataPlusOffset );                
        }

        mipmapAfterTexImage(state, mipmapResult);
    }
    else
    {
        if (image->isMipmap())
        {
            numMipmapLevels = image->getNumMipmapLevels();

            int width  = inwidth;
            int height = inheight;

            if( !compressed_image )
            {
                for( GLsizei k = 0 ; k < numMipmapLevels  && (width || height) ;k++)
                {

                    if (width == 0)
                        width = 1;
                    if (height == 0)
                        height = 1;

                    glTexSubImage2D( target, k, 
                        0, 0,
                        width, height,
                        (GLenum)image->getPixelFormat(),
                        (GLenum)image->getDataType(),
                        image->getMipmapData(k) - dataMinusOffset + dataPlusOffset);

                    width >>= 1;
                    height >>= 1;
                }
            }
            else if (extensions->isCompressedTexImage2DSupported())
            {
                GLint blockSize,size;
                for( GLsizei k = 0 ; k < numMipmapLevels  && (width || height) ;k++)
                {
                    if (width == 0)
                        width = 1;
                    if (height == 0)
                        height = 1;

                    getCompressedSize(image->getInternalTextureFormat(), width, height, 1, blockSize,size);

                    //state.checkGLErrors("before extensions->glCompressedTexSubImage2D(");

                    extensions->glCompressedTexSubImage2D(target, k,  
                                                       0, 0,
                                                       width, height, 
                                                       (GLenum)image->getPixelFormat(),
                                                       size,
                                                        image->getMipmapData(k) - dataMinusOffset + dataPlusOffset);                

                    //state.checkGLErrors("after extensions->glCompressedTexSubImage2D(");

                    width >>= 1;
                    height >>= 1;
                }

            }
        }
        else
        {
            //notify(WARN)<<"Warning:: cannot subload mip mapped texture from non mipmapped image."<<std::endl;
            applyTexImage2D_load(state, target, image, inwidth, inheight,numMipmapLevels); 
            return;
        }
    }
    
    if (pbo)
    {
        state.unbindPixelBufferObject();
    }
#ifdef DO_TIMING
    CRCore::notify(CRCore::NOTICE)<<"glTexSubImage2D "<<CRCore::Timer::instance()->delta_m(start_tick,CRCore::Timer::instance()->tick())<<"ms"<<std::endl;
#endif

    if (needImageRescale)
    {
        // clean up the resized image.
        delete [] data;
    }
}

bool crTexture::isHardwareMipmapGenerationEnabled(const crState& state) const
{
    if (m_useHardwareMipMapGeneration)
    {
        unsigned int contextID = state.getContextID();
        const Extensions* extensions = getExtensions(contextID,true);

        if (extensions->isGenerateMipMapSupported())
        {
            return true;
        }

        const crFBOExtensions* fbo_ext = crFBOExtensions::instance(contextID,true);

        if (fbo_ext->glGenerateMipmapEXT)
        {
            return true;
        }
    }

    return false;
}

crTexture::GenerateMipmapMode crTexture::mipmapBeforeTexImage(const crState& state, bool hardwareMipmapOn) const
{
    if (hardwareMipmapOn)
    {
        int width = getTextureWidth();
        int height = getTextureHeight();

        //quick bithack to determine whether width or height are non-power-of-two
        if ((width & (width - 1)) || (height & (height - 1)))
        {
            //GL_GENERATE_MIPMAP_SGIS with non-power-of-two textures on NVIDIA hardware
            //is extremely slow. Use glGenerateMipmapEXT() instead if supported.
            if (m_internalFormatType != SIGNED_INTEGER &&
                m_internalFormatType != UNSIGNED_INTEGER)
            {
                if (crFBOExtensions::instance(state.getContextID(), true)->glGenerateMipmapEXT)
                {
                    return GENERATE_MIPMAP;
                }
            }
        }

        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
        return GENERATE_MIPMAP_TEX_PARAMETER;
    }
    return GENERATE_MIPMAP_NONE;
}

void crTexture::mipmapAfterTexImage(crState& state, GenerateMipmapMode beforeResult) const
{
    switch (beforeResult)
    {
    case GENERATE_MIPMAP:
        {
            unsigned int contextID = state.getContextID();
            TextureObject* textureObject = getTextureObject(contextID);
            if (textureObject)
            {
                CRCore::crFBOExtensions* fbo_ext = CRCore::crFBOExtensions::instance(contextID, true);
                fbo_ext->glGenerateMipmapEXT(
					textureObject->m_target);
            }
        }
        break;
    case GENERATE_MIPMAP_TEX_PARAMETER:
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_FALSE);
        break;
    }
}

void crTexture::generateMipmap(crState& state) const
{
    const unsigned int contextID = state.getContextID();

    // get the texture object for the current contextID.
    TextureObject* textureObject = getTextureObject(contextID);

    // if not initialized before, then do nothing
    if (textureObject == NULL) return;

    m_texMipmapGenerationDirtyList[contextID] = 0;
    
    // if internal format does not provide automatic mipmap generation, then do manual allocation
    if (m_internalFormatType == SIGNED_INTEGER || m_internalFormatType == UNSIGNED_INTEGER)
    {
        allocateMipmap(state);
        return;
    }
    
    // get fbo extension which provides us with the glGenerateMipmapEXT function
    CRCore::crFBOExtensions* fbo_ext = CRCore::crFBOExtensions::instance(state.getContextID(), true);

    // check if the function is supported
    if (fbo_ext->glGenerateMipmapEXT)
    {
        textureObject->bind(crFrameStamp::getInstance()->getReferenceTime());
        fbo_ext->glGenerateMipmapEXT(textureObject->m_target);
        
        // inform state that this texture is the current one bound.
        state.haveAppliedTextureAttribute(state.getActiveTextureUnit(), this);
    
    // if the function is not supported, then do manual allocation
    }else
    {
        allocateMipmap(state);
    }
    
}


///////////////////////////////////////////////////////////////////////////////////////////////
//  Static map to manage the deletion of texture objects are the right time.
//////////////////////////////////////////////////////////////////////////////////////////////
#include <map>
#include <set>


void crTexture::compile(crState& state) const
{
    apply(state);
	m_needCompile = false;
}

void crTexture::setAlphaShadowEnable(bool enableAlphaShadow)
{
	m_alphaShadowEnable = enableAlphaShadow;
}

void crTexture::setBlendShadowEnable(bool enableAlphaShininess)
{
	m_blendShadowEnable = enableAlphaShininess;
}

void crTexture::setUseParallax(bool useParallax)
{
	m_useParallax = useParallax;
}

void crTexture::setExternTextureID(ExternTextureID id)
{
    m_externTextureID = id;
}
//void crTexture::resizeGLObjectBuffers(unsigned int maxSize)
//{
//    m_textureObjectBuffer.resize(maxSize);
//}

void crTexture::releaseObjects(crState* state)
{
//    if (state) CRCore::notify(CRCore::NOTICE)<<"crTexture::releaseGLObjects contextID="<<state->getContextID()<<std::endl;
//    else CRCore::notify(CRCore::NOTICE)<<"crTexture::releaseGLObjects no crState "<<std::endl;
	//this->dirtyTextureObject();
	//if (!state) this->dirtyTextureObject();
	//else
	//{
	//	unsigned int contextID = state->getContextID();
	//	TextureObject *to = m_textureObjectBuffer[contextID].get();
	//	if (to) 
	//	{
	//		CRCore::ScopedLock<crMutex> lock(getTextureObjectManager()->m_mutex);

	//		getTextureObjectManager()->m_textureObjectListMap[contextID].insert(to);
	//		to->setStateInfo(1);
	//		//m_textureObjectBuffer[contextID] = 0;
	//	}
	//}
}

crTexture::Extensions* crTexture::getExtensions(unsigned int contextID,bool createIfNotInitalized)
{
    if (!s_extensions[contextID] && createIfNotInitalized) s_extensions[contextID] = new Extensions(contextID);
    return s_extensions[contextID].get();
}

void crTexture::setExtensions(unsigned int contextID,Extensions* extensions)
{
    s_extensions[contextID] = extensions;
}

crTexture::Extensions::Extensions(unsigned int contextID)
{
    setupGLExtensions(contextID);
}

crTexture::Extensions::Extensions(const Extensions& rhs):
    Referenced()
{
    m_isMultiTexturingSupported = rhs.m_isMultiTexturingSupported;
    m_isTextureFilterAnisotropicSupported = rhs.m_isTextureFilterAnisotropicSupported;
    m_isTextureCompressionARBSupported = rhs.m_isTextureCompressionARBSupported;
    m_isTextureCompressionS3TCSupported = rhs.m_isTextureCompressionS3TCSupported;
    m_isTextureMirroredRepeatSupported = rhs.m_isTextureMirroredRepeatSupported;
    m_isTextureEdgeClampSupported = rhs.m_isTextureEdgeClampSupported;
    m_isTextureBorderClampSupported = rhs.m_isTextureBorderClampSupported;
    m_isGenerateMipMapSupported = rhs.m_isGenerateMipMapSupported;

    m_maxTextureSize = rhs.m_maxTextureSize;

    m_glCompressedTexImage2D = rhs.m_glCompressedTexImage2D;

    m_isShadowSupported = rhs.m_isShadowSupported;
    m_isShadowAmbientSupported = rhs.m_isShadowAmbientSupported;

    m_isClientStorageSupported = rhs.m_isClientStorageSupported;

    m_isNonPowerOfTwoTextureMipMappedSupported = rhs.m_isNonPowerOfTwoTextureMipMappedSupported;
    m_isNonPowerOfTwoTextureNonMipMappedSupported = rhs.m_isNonPowerOfTwoTextureNonMipMappedSupported;

    m_isTextureIntegerEXTSupported = rhs.m_isTextureIntegerEXTSupported;
	m_isTextureMultisampledSupported = rhs.m_isTextureMultisampledSupported;
}

void crTexture::Extensions::lowestCommonDenominator(const Extensions& rhs)
{
    if (!rhs.m_isMultiTexturingSupported) m_isMultiTexturingSupported = false;
    
    if (!rhs.m_isTextureFilterAnisotropicSupported) m_isTextureFilterAnisotropicSupported = false;
    if (!rhs.m_isTextureMirroredRepeatSupported) m_isTextureMirroredRepeatSupported = false;
    if (!rhs.m_isTextureEdgeClampSupported) m_isTextureEdgeClampSupported = false;
    if (!rhs.m_isTextureBorderClampSupported) m_isTextureBorderClampSupported = false;
    
    if (!rhs.m_isTextureCompressionARBSupported) m_isTextureCompressionARBSupported = false;
    if (!rhs.m_isTextureCompressionS3TCSupported) m_isTextureCompressionS3TCSupported = false;
    
    if (!rhs.m_isGenerateMipMapSupported) m_isGenerateMipMapSupported = false;

    if (rhs.m_maxTextureSize<m_maxTextureSize) m_maxTextureSize = rhs.m_maxTextureSize;
    if (rhs.m_numTextureUnits<m_numTextureUnits) m_numTextureUnits = rhs.m_numTextureUnits;

    if (!rhs.m_glCompressedTexImage2D) m_glCompressedTexImage2D = 0;
    if (!rhs.m_glCompressedTexSubImage2D) m_glCompressedTexSubImage2D = 0;
    if (!rhs.m_glGetCompressedTexImage) m_glGetCompressedTexImage = 0;

    if (!rhs.m_isShadowSupported) m_isShadowSupported = false;
    if (!rhs.m_isShadowAmbientSupported) m_isShadowAmbientSupported = false;
    
    if (!rhs.m_isClientStorageSupported) m_isClientStorageSupported = false;

    if (!rhs.m_isNonPowerOfTwoTextureMipMappedSupported) m_isNonPowerOfTwoTextureMipMappedSupported = false;
    if (!rhs.m_isNonPowerOfTwoTextureNonMipMappedSupported) m_isNonPowerOfTwoTextureNonMipMappedSupported = false;

    if (!rhs.m_isTextureIntegerEXTSupported) m_isTextureIntegerEXTSupported = false;
	if (!rhs.m_isTextureMultisampledSupported) m_isTextureMultisampledSupported = false;
}

void crTexture::Extensions::setupGLExtensions(unsigned int contextID)
{
    const char* version = (const char*) glGetString( GL_VERSION );
    if (!version)
    {
        CRCore::notify(CRCore::FATAL)<<"Error: In crTexture::Extensions::setupGLExtensions(..) OpenGL version test failed, requires valid graphics context."<<std::endl;
        return;
    }
    

    float glVersion = atof( (const char *)version );
    const char* renderer = (const char*) glGetString(GL_RENDERER);
    std::string rendererString(renderer ? renderer : "");
    
    m_isMultiTexturingSupported = ( glVersion >= 1.3 ) ||
                                 isGLExtensionSupported(contextID,"GL_ARB_multitexture") ||
                                 isGLExtensionSupported(contextID,"GL_EXT_multitexture");
    m_isTextureFilterAnisotropicSupported = isGLExtensionSupported(contextID,"GL_EXT_texture_filter_anisotropic");
    m_isTextureCompressionARBSupported = ( glVersion >= 1.3 ) ||
                                        isGLExtensionSupported(contextID,"GL_ARB_texture_compression");
    m_isTextureCompressionS3TCSupported = isGLExtensionSupported(contextID,"GL_EXT_texture_compression_s3tc");
    m_isTextureMirroredRepeatSupported = ( glVersion >= 1.4 ) || 
                                    isGLExtensionSupported(contextID,"GL_IBM_texture_mirrored_repeat") ||
                                    isGLExtensionSupported(contextID,"GL_ARB_texture_mirrored_repeat");
    m_isTextureEdgeClampSupported = ( glVersion >= 1.2 ) ||
                                    isGLExtensionSupported(contextID,"GL_EXT_texture_edge_clamp") || 
                                    isGLExtensionSupported(contextID,"GL_SGIS_texture_edge_clamp");
    m_isTextureBorderClampSupported = ( glVersion >= 1.3 ) ||
                                    isGLExtensionSupported(contextID,"GL_ARB_texture_border_clamp");
    m_isGenerateMipMapSupported = (strncmp((const char*)glGetString(GL_VERSION),"1.4",3)>=0) ||
                                  isGLExtensionSupported(contextID,"GL_SGIS_generate_mipmap");
    m_isShadowSupported = isGLExtensionSupported(contextID,"GL_ARB_shadow");
    m_isShadowAmbientSupported = isGLExtensionSupported(contextID,"GL_ARB_shadow_ambient");

    m_isClientStorageSupported = isGLExtensionSupported(contextID,"GL_APPLE_client_storage");

    m_isNonPowerOfTwoTextureNonMipMappedSupported = ( glVersion >= 2.0 ) ||
                                       isGLExtensionSupported(contextID,"GL_ARB_texture_non_power_of_two");

    m_isNonPowerOfTwoTextureMipMappedSupported = m_isNonPowerOfTwoTextureNonMipMappedSupported;
    
    m_isTextureIntegerEXTSupported = isGLExtensionSupported(contextID, "GL_EXT_texture_integer");

	m_isTextureMultisampledSupported = isGLExtensionSupported(contextID,"GL_ARB_texture_multisample");

    if (rendererString.find("Radeon")!=std::string::npos || rendererString.find("RADEON")!=std::string::npos)
    {
        m_isNonPowerOfTwoTextureMipMappedSupported = false;
        //CRCore::notify(CRCore::INFO)<<"Disabling m_isNonPowerOfTwoTextureMipMappedSupported for ATI hardware."<<std::endl;
    }

    if (rendererString.find("GeForce FX")!=std::string::npos)
    {
        m_isNonPowerOfTwoTextureMipMappedSupported = false;
        //CRCore::notify(CRCore::INFO)<<"Disabling m_isNonPowerOfTwoTextureMipMappedSupported for GeForce FX hardware."<<std::endl;
    }

    glGetIntegerv(GL_MAX_TEXTURE_SIZE,&m_maxTextureSize);

    //char *ptr;
    //if( (ptr = getenv("CRE_MAX_TEXTURE_SIZE")) != 0)
    //{
    //    GLint osg_max_size = atoi(ptr);

    //    if (osg_max_size<m_maxTextureSize)
    //    {

    //        m_maxTextureSize = osg_max_size;
    //    }
    //}

    if( m_isMultiTexturingSupported )
    {
       glGetIntegerv(GL_MAX_TEXTURE_UNITS,&m_numTextureUnits);
    }
    else
    {
       m_numTextureUnits = 1;
    }

    setGLExtensionFuncPtr(m_glCompressedTexImage2D,"glCompressedTexImage2D","glCompressedTexImage2DARB");
    setGLExtensionFuncPtr(m_glCompressedTexSubImage2D,"glCompressedTexSubImage2D","glCompressedTexSubImage2DARB");
    setGLExtensionFuncPtr(m_glGetCompressedTexImage,"glGetCompressedTexImage","glGetCompressedTexImageARB");;
	setGLExtensionFuncPtr(m_glTexImage2DMultisample, "glTexImage2DMultisample", "glTexImage2DMultisampleARB");

    setGLExtensionFuncPtr(m_glTexParameterIiv, "glTexParameterIiv", "glTexParameterIivARB");
    setGLExtensionFuncPtr(m_glTexParameterIuiv, "glTexParameterIuiv", "glTexParameterIuivARB");
    
    if (m_glTexParameterIiv == NULL) setGLExtensionFuncPtr(m_glTexParameterIiv, "glTexParameterIivEXT");
    if (m_glTexParameterIuiv == NULL) setGLExtensionFuncPtr(m_glTexParameterIuiv, "glTexParameterIuivEXT");
}


void crTexture::Extensions::glTexParameterIiv(GLenum target, GLenum pname, const GLint* data) const
{
    if (m_glTexParameterIiv)
    {
        m_glTexParameterIiv(target, pname, data);
    }
    else
    {
        notify(WARN)<<"Error: glTexParameterIiv not supported by OpenGL driver"<<std::endl;
    }
}

void crTexture::Extensions::glTexParameterIuiv(GLenum target, GLenum pname, const GLuint* data) const
{
    if (m_glTexParameterIuiv)
    {
        m_glTexParameterIuiv(target, pname, data);
    }
    else
    {
        notify(WARN)<<"Error: glTexParameterIuiv not supported by OpenGL driver"<<std::endl;
    }
}

void crTexture::Extensions::glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data) const
{
    if (m_glCompressedTexImage2D)
    {
        m_glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
    }
    else
    {
        notify(WARN)<<"Error: glCompressedTexImage2D not supported by OpenGL driver"<<std::endl;
    }
    
}

void crTexture::Extensions::glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data) const
{
    if (m_glCompressedTexSubImage2D)
    {
        m_glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
    }
    else
    {
        notify(WARN)<<"Error: glCompressedTexImage2D not supported by OpenGL driver"<<std::endl;
    }
    
}
void crTexture::Extensions::glGetCompressedTexImage(GLenum target, GLint level, GLvoid *data) const
{
    if (m_glGetCompressedTexImage)
    {
        m_glGetCompressedTexImage(target, level, data);
    }
    else
    {
        notify(WARN)<<"Error: glGetCompressedTexImage not supported by OpenGL driver"<<std::endl;
    }
}
void crTexture::Extensions::glTexImage2DMultisample(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) const
{
	m_glTexImage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations);
}
