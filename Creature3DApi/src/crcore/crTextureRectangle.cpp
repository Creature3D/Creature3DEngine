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
#include <CRCore/crTextureRectangle.h>
#include <CRCore/crImageSequence.h>
#include <CRCore/crState.h>
#include <crgl/GLU.h>
#include <CRCore/crNotify.h>

#include <CRCore/crTimer.h>

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

// #define DO_TIMING

using namespace CRCore;

crTextureRectangle::crTextureRectangle():
    m_textureWidth(0),
    m_textureHeight(0)
{
    setWrap(WRAP_S, CLAMP);
    setWrap(WRAP_T, CLAMP);

    setFilter(MIN_FILTER, LINEAR);
    setFilter(MAG_FILTER, LINEAR);
}

crTextureRectangle::crTextureRectangle(crImage* image):
            m_textureWidth(0),
            m_textureHeight(0)
{
    setWrap(WRAP_S, CLAMP);
    setWrap(WRAP_T, CLAMP);

    setFilter(MIN_FILTER, LINEAR);
    setFilter(MAG_FILTER, LINEAR);
    
    setImage(image);
}

crTextureRectangle::crTextureRectangle(const crTextureRectangle& text,const crCopyOp& copyop):
    crTexture(text,copyop),
    m_image(copyop(text.m_image.get())),
    m_textureWidth(text.m_textureWidth),
    m_textureHeight(text.m_textureHeight),
    m_subloadCallback(text.m_subloadCallback)
{
}

crTextureRectangle::~crTextureRectangle()
{
}

int crTextureRectangle::compare(const crStateAttribute& sa) const
{
    // check the types are equal and then create the rhs variable
    // used by the COMPARE_StateAttribute_Parameter macro's below.
    COMPARE_StateAttribute_Types(crTextureRectangle,sa)
	int result = m_imageNameID.compare(rhs.m_imageNameID);
	if (result!=0) return result;

    //if (m_image!=rhs.m_image) // smart pointer comparison.
    //{
    //    if (m_image.valid())
    //    {
    //        if (rhs.m_image.valid())
    //        {
    //            int result = m_image->compare(*rhs.m_image);
    //            if (result!=0) return result;
    //        }
    //        else
    //        {
    //            return 1; // valid lhs.m_image is greater than null. 
    //        }
    //    }
    //    else if (rhs.m_image.valid()) 
    //    {
    //        return -1; // valid rhs.m_image is greater than null. 
    //    }
    //}

    if (!m_image && !rhs.m_image)
    {
        // no image attached to either Texture2D
        // but could these textures already be downloaded?
        // check the m_textureObjectBuffer to see if they have been
        // downloaded

        result = compareTextureObjects(rhs);
        if (result!=0) return result;
    }

    result = compareTexture(rhs);
    if (result!=0) return result;

    // compare each parameter in turn against the rhs.
    COMPARE_StateAttribute_Parameter(m_textureWidth)
    COMPARE_StateAttribute_Parameter(m_textureHeight)
    COMPARE_StateAttribute_Parameter(m_subloadCallback)

    return 0; // passed all the above comparison macro's, must be equal.
}

int crTextureRectangle::compare(const crStateAttribute& sa)
{
	// check the types are equal and then create the rhs variable
	// used by the COMPARE_StateAttribute_Parameter macro's below.
	COMPARE_StateAttribute_Types(crTextureRectangle,sa)
	int result = m_imageNameID.compare(rhs.m_imageNameID);
	if (result!=0) return result;

	//if (m_image!=rhs.m_image) // smart pointer comparison.
	//{
	//	if (m_image.valid())
	//	{
	//		if (rhs.m_image.valid())
	//		{
	//			int result = m_image->compare(*rhs.m_image);
	//			if (result!=0) return result;
	//		}
	//		else
	//		{
	//			return 1; // valid lhs.m_image is greater than null. 
	//		}
	//	}
	//	else if (rhs.m_image.valid()) 
	//	{
	//		return -1; // valid rhs.m_image is greater than null. 
	//	}
	//}

	if (!m_image && !rhs.m_image)
	{
		// no image attached to either Texture2D
		// but could these textures already be downloaded?
		// check the m_textureObjectBuffer to see if they have been
		// downloaded

		result = compareTextureObjects(rhs);
		if (result!=0) return result;
	}

	result = compareTexture(rhs);
	if (result!=0) return result;

	// compare each parameter in turn against the rhs.
	COMPARE_StateAttribute_Parameter(m_textureWidth)
	COMPARE_StateAttribute_Parameter(m_textureHeight)
	COMPARE_StateAttribute_Parameter(m_subloadCallback)

	return 0; // passed all the above comparison macro's, must be equal.
}

void crTextureRectangle::setImage(crImage* image)
{
    if (m_image == image) return;

    if (dynamic_cast<CRCore::crImageSequence*>(m_image.get()))
    {
        setUpdateCallback(0);
        setDataVariance(CRCore::crBase::STATIC);
    }

    // delete old texture objects.
    dirtyTextureObject();

    m_image = image;
    
    if (dynamic_cast<CRCore::crImageSequence*>(m_image.get()))
    {
        setUpdateCallback(new crImageSequence::UpdateCallback());
        setDataVariance(CRCore::crBase::DYNAMIC);
    }
}


void crTextureRectangle::apply(crState& state) const
{
    static bool s_rectangleSupported = isGLExtensionSupported(state.getContextID(),"GL_ARB_texture_rectangle")
            || isGLExtensionSupported(state.getContextID(),"GL_EXT_texture_rectangle")
            || isGLExtensionSupported(state.getContextID(),"GL_NV_texture_rectangle");

    if (!s_rectangleSupported)
    {
        notify(WARN)<<"Warning: crTextureRectangle::apply(..) failed, texture rectangle is not support by your OpenGL drivers."<<std::endl;
        return;
    }

    // get the contextID (user defined ID of 0 upwards) for the 
    // current OpenGL context.
    const unsigned int contextID = state.getContextID();


    //static crMutex s_mutex;

    // get the texture object for the current contextID.
    TextureObject* textureObject = getTextureObject(contextID);
    
	double referenceTime = crFrameStamp::getInstance()->getReferenceTime();

    if (textureObject)
    {
		CRCore::ref_ptr<CRCore::crImage> image = m_image;
        textureObject->bind(referenceTime);
        if (getTextureParameterDirty(state.getContextID()))
            applyTexParameters(GL_TEXTURE_RECTANGLE, state);

        if (m_subloadCallback.valid())
        {
            m_subloadCallback->subload(*this, state);
        }
        else if (image.valid() && image->data() && getModifiedCount(contextID) != image->getModifiedCount())
        {
            applyTexImage_subload(GL_TEXTURE_RECTANGLE, image.get(), state, m_textureWidth, m_textureHeight, m_internalFormat);
 
            // update the modified count to show that it is upto date.
            getModifiedCount(contextID) = image->getModifiedCount();
        }
    }
    else if (m_subloadCallback.valid())
    {
        // we don't have a applyTexImage1D_subload yet so can't reuse.. so just generate a new texture object.        
        m_textureObjectBuffer[contextID] = textureObject = generateTextureObject(contextID,GL_TEXTURE_RECTANGLE);

        textureObject->bind(referenceTime);

        applyTexParameters(GL_TEXTURE_RECTANGLE, state);

        m_subloadCallback->load(*this, state);

        textureObject->setAllocated(1,m_internalFormat,m_textureWidth,m_textureHeight,1,0);

        // in theory the following line is redundant, but in practice
        // have found that the first frame drawn doesn't apply the textures
        // unless a second bind is called?!!
        // perhaps it is the first glBind which is not required...
        //glBindTexture(GL_TEXTURE_RECTANGLE, handle);
    }
    else if (m_image.valid() && m_image->data())
    {
		CRCore::ref_ptr<CRCore::crImage> image = m_image;
        //CRCore::ScopedLock<crMutex> lock(s_mutex);

        // we don't have a applyTexImage1D_subload yet so can't reuse.. so just generate a new texture object.        
        textureObject = generateTextureObject(contextID,GL_TEXTURE_RECTANGLE);
        
        textureObject->bind(referenceTime);

        applyTexParameters(GL_TEXTURE_RECTANGLE, state);
     
        applyTexImage_load(GL_TEXTURE_RECTANGLE, image.get(), state, m_textureWidth, m_textureHeight);

        textureObject->setAllocated(1,m_internalFormat,m_textureWidth,m_textureHeight,1,0);
        
        m_textureObjectBuffer[contextID] = textureObject;

        if (m_unrefImageDataAfterApply && areAllTextureObjectsLoaded() && image->getDataVariance()==STATIC)
        {
            crTextureRectangle* non_const_this = const_cast<crTextureRectangle*>(this);
            non_const_this->m_image = 0;
        }


    }
    else if ( (m_textureWidth!=0) && (m_textureHeight!=0) && (m_internalFormat!=0) )
    {
        m_textureObjectBuffer[contextID] = textureObject = generateTextureObject(
                contextID,GL_TEXTURE_RECTANGLE,0,m_internalFormat,m_textureWidth,m_textureHeight,1,0);
        
        textureObject->bind(referenceTime);

        applyTexParameters(GL_TEXTURE_RECTANGLE,state);

        // no image present, but dimensions at set so lets create the texture
        glTexImage2D( GL_TEXTURE_RECTANGLE, 0, m_internalFormat,
                     m_textureWidth, m_textureHeight, m_borderWidth,
                     m_sourceFormat ? m_sourceFormat : m_internalFormat,
                     m_sourceType ? m_sourceType : GL_UNSIGNED_BYTE,
                     0);                
                     
        if (m_readPBuffer.valid())
        {
            m_readPBuffer->bindPBufferToTexture(GL_FRONT);
        }
        
    }
    else
    {
        glBindTexture(GL_TEXTURE_RECTANGLE, 0);
    }
}

void crTextureRectangle::applyTexParameters(GLenum target, crState& state) const
{
    // get the contextID (user defined ID of 0 upwards) for the 
    // current OpenGL context.
    const unsigned int contextID = state.getContextID();

    glTexParameteri( target, GL_TEXTURE_WRAP_S, m_wrap_s );
    glTexParameteri( target, GL_TEXTURE_WRAP_T, m_wrap_t );

    glTexParameteri( target, GL_TEXTURE_MIN_FILTER, m_min_filter);
    glTexParameteri( target, GL_TEXTURE_MAG_FILTER, m_mag_filter);

    getTextureParameterDirty(contextID) = false;
}

void crTextureRectangle::applyTexImage_load(GLenum target, crImage* image, crState& state, GLsizei& inwidth, GLsizei& inheight) const
{
    // if we don't have a valid image we can't create a texture!
    if (!image || !image->data())
        return;

    // get the contextID (user defined ID of 0 upwards) for the 
    // current OpenGL context.
    const unsigned int contextID = state.getContextID();
    const Extensions* extensions = getExtensions(contextID,true);

    // update the modified count to show that it is upto date.
    getModifiedCount(contextID) = image->getModifiedCount();

    // compute the internal texture format, sets m_internalFormat.
    computeInternalFormat();

    glPixelStorei(GL_UNPACK_ALIGNMENT, image->getPacking());

    bool useClientStorage = extensions->isClientStorageSupported() && getClientStorageHint();
    if (useClientStorage)
    {
        glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE,GL_TRUE);
        glTexParameterf(target,GL_TEXTURE_PRIORITY,0.0f);
        
        #ifdef GL_TEXTURE_STORAGE_HINT_APPLE    
            glTexParameteri(target, GL_TEXTURE_STORAGE_HINT_APPLE , GL_STORAGE_CACHED_APPLE);
        #endif
    }

    unsigned char* dataMinusOffset = 0;
    unsigned char* dataPlusOffset = 0;

    const crPixelBufferObject* pbo = image->getPixelBufferObject();
    if (pbo && pbo->isPBOSupported(contextID))
    {
        state.bindPixelBufferObject(pbo);
        dataMinusOffset = image->data();
        dataPlusOffset = reinterpret_cast<unsigned char*>(pbo->offset());
    }
    else
    {
        pbo = 0;
    }

    if(isCompressedInternalFormat(m_internalFormat) && extensions->isCompressedTexImage2DSupported())
    {
        extensions->glCompressedTexImage2D(target, 0, m_internalFormat, 
          image->s(), image->t(), 0,
          image->getImageSizeInBytes(), 
          image->data() - dataMinusOffset + dataPlusOffset);                
    }
    else
    {
        glTexImage2D(target, 0, m_internalFormat,
          image->s(), image->t(), 0,
          (GLenum)image->getPixelFormat(),
          (GLenum)image->getDataType(),
          image->data() - dataMinusOffset + dataPlusOffset );
    }
    

    if (pbo)
    {
        state.unbindPixelBufferObject();
    }

    inwidth = image->s();
    inheight = image->t();

    if (useClientStorage)
    {
        glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE,GL_FALSE);
    }
}

void crTextureRectangle::applyTexImage_subload(GLenum target, crImage* image, crState& state, GLsizei& inwidth, GLsizei& inheight, GLint& inInternalFormat) const
{
    // if we don't have a valid image we can't create a texture!
    if (!image || !image->data())
        return;

    if (image->s()!=inwidth || image->t()!=inheight || image->getInternalTextureFormat()!=inInternalFormat) 
    {
        applyTexImage_load(target, image, state, inwidth, inheight);
        return;
    }


    // get the contextID (user defined ID of 0 upwards) for the 
    // current OpenGL context.
    const unsigned int contextID = state.getContextID();
    const Extensions* extensions = getExtensions(contextID,true);


    // update the modified count to show that it is upto date.
    getModifiedCount(contextID) = image->getModifiedCount();

    // compute the internal texture format, sets m_internalFormat.
    computeInternalFormat();

    glPixelStorei(GL_UNPACK_ALIGNMENT, image->getPacking());

#ifdef DO_TIMING
    CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();
    CRCore::notify(CRCore::NOTICE)<<"glTexSubImage2D pixelFormat = "<<std::hex<<image->getPixelFormat()<<std::dec<<std::endl;
#endif
    unsigned char* dataMinusOffset = 0;
    unsigned char* dataPlusOffset = 0;

    const crPixelBufferObject* pbo = image->getPixelBufferObject();
    if (pbo && pbo->isPBOSupported(contextID))
    {
        state.bindPixelBufferObject(pbo);
        dataMinusOffset = image->data();
        dataPlusOffset = reinterpret_cast<unsigned char*>(pbo->offset()); // -dataMinusOffset+dataPlusOffset

#ifdef DO_TIMING
        CRCore::notify(CRCore::NOTICE)<<"after PBO "<<CRCore::Timer::instance()->delta_m(start_tick,CRCore::Timer::instance()->tick())<<"ms"<<std::endl;
#endif

    }
    else
    {
        pbo = 0;
    }
    

    if(isCompressedInternalFormat(m_internalFormat) && extensions->isCompressedTexSubImage2DSupported())
    {
        extensions->glCompressedTexSubImage2D(target, 0, 
          0,0, 
          image->s(), image->t(),
          (GLenum)image->getPixelFormat(),
          (GLenum)image->getDataType(),
          image->data() - dataMinusOffset + dataPlusOffset);                
    }
    else
    {
        glTexSubImage2D(target, 0, 
          0,0,
          image->s(), image->t(),
          (GLenum)image->getPixelFormat(),
          (GLenum)image->getDataType(),
          image->data() - dataMinusOffset + dataPlusOffset  );
    }

    if (pbo)
    {
        state.unbindPixelBufferObject();
    }

#ifdef DO_TIMING
    CRCore::notify(CRCore::NOTICE)<<"glTexSubImage2D "<<CRCore::Timer::instance()->delta_m(start_tick,CRCore::Timer::instance()->tick())<<"ms"<<std::endl;
#endif
    
}

void crTextureRectangle::computeInternalFormat() const
{
    if (m_image.valid()) computeInternalFormatWithImage(*m_image); 
    else computeInternalFormatType();
}

void crTextureRectangle::copyTexImage2D(crState& state, int x, int y, int width, int height )
{
    const unsigned int contextID = state.getContextID();
    
    if (m_internalFormat==0) m_internalFormat=GL_RGBA;

    // get the globj for the current contextID.
    TextureObject* textureObject = getTextureObject(contextID);
    
    if (textureObject)
    {
        if (width==(int)m_textureWidth && height==(int)m_textureHeight)
        {
            // we have a valid texture object which is the right size
            // so lets play clever and use copyTexSubImage2D instead.
            // this allows use to reuse the texture object and avoid
            // expensive memory allocations.
            copyTexSubImage2D(state,0 ,0, x, y, width, height);
            return;
        }
        // the relevent texture object is not of the right size so
        // needs to been deleted    
        // remove previously bound textures. 
        dirtyTextureObject();
        // note, dirtyTextureObject() dirties all the texture objects for
        // this texture, is this right?  Perhaps we should dirty just the
        // one for this context.  Note sure yet will leave till later.
        // RO July 2001.
    }
    
    
    // remove any previously assigned images as these are nolonger valid.
    m_image = NULL;

    // switch off mip-mapping.
    //
    m_textureObjectBuffer[contextID] = textureObject =
    generateTextureObject(contextID,GL_TEXTURE_RECTANGLE);

    textureObject->bind(crFrameStamp::getInstance()->getReferenceTime());
    
    applyTexParameters(GL_TEXTURE_RECTANGLE,state);


/*    bool needHardwareMipMap = (m_min_filter != LINEAR && m_min_filter != NEAREST);
    bool hardwareMipMapOn = false;
    if (needHardwareMipMap)
    {
        const Extensions* extensions = getExtensions(contextID,true);
        bool generateMipMapSupported = extensions->isGenerateMipMapSupported();

        hardwareMipMapOn = _useHardwareMipMapGeneration && generateMipMapSupported;
        
        if (!hardwareMipMapOn)
        {
            // have to swtich off mip mapping
            notify(NOTICE)<<"Warning: Texture2D::copyTexImage2D(,,,,) switch of mip mapping as hardware support not available."<<std::endl;
            m_min_filter = LINEAR;
        }
    }
*/    
//    if (hardwareMipMapOn) glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS,GL_TRUE);

    glCopyTexImage2D( GL_TEXTURE_RECTANGLE, 0, m_internalFormat, x, y, width, height, 0 );

//    if (hardwareMipMapOn) glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS,GL_FALSE);


    m_textureWidth = width;
    m_textureHeight = height;
//    _numMipmapLevels = 1;
    
    textureObject->setAllocated(1,m_internalFormat,m_textureWidth,m_textureHeight,1,0);


    // inform state that this texture is the current one bound.
    state.haveAppliedTextureAttribute(state.getActiveTextureUnit(), this);
}

void crTextureRectangle::copyTexSubImage2D(crState& state, int xoffset, int yoffset, int x, int y, int width, int height )
{
    const unsigned int contextID = state.getContextID();

    if (m_internalFormat==0) m_internalFormat=GL_RGBA;

    // get the texture object for the current contextID.
    TextureObject* textureObject = getTextureObject(contextID);
    
    if (textureObject)
    {
        // we have a valid image
        textureObject->bind(crFrameStamp::getInstance()->getReferenceTime());
        
        applyTexParameters(GL_TEXTURE_RECTANGLE,state);

/*        bool needHardwareMipMap = (m_min_filter != LINEAR && m_min_filter != NEAREST);
        bool hardwareMipMapOn = false;
        if (needHardwareMipMap)
        {
            const Extensions* extensions = getExtensions(contextID,true);
            bool generateMipMapSupported = extensions->isGenerateMipMapSupported();

            hardwareMipMapOn = _useHardwareMipMapGeneration && generateMipMapSupported;

            if (!hardwareMipMapOn)
            {
                // have to swtich off mip mapping
                notify(NOTICE)<<"Warning: Texture2D::copyTexImage2D(,,,,) switch of mip mapping as hardware support not available."<<std::endl;
                m_min_filter = LINEAR;
            }
        }
*/
//        if (hardwareMipMapOn) glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS,GL_TRUE);

        glCopyTexSubImage2D( GL_TEXTURE_RECTANGLE, 0, xoffset, yoffset, x, y, width, height);

//        if (hardwareMipMapOn) glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS,GL_FALSE);

        // inform state that this texture is the current one bound.
        state.haveAppliedTextureAttribute(state.getActiveTextureUnit(), this);

    }
    else
    {
        // no texture object already exsits for this context so need to
        // create it upfront - simply call copyTexImage2D.
        copyTexImage2D(state,x,y,width,height);
    }
}

void crTextureRectangle::allocateMipmap(crState&) const
{
    CRCore::notify(CRCore::NOTICE)<<"Warning: crTextureRectangle::allocateMipmap(crState&) called eroneously, GL_TEXTURE_RECTANGLE does not support mipmapping."<<std::endl;
}

