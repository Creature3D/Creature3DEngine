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
#include <crgl/GLExtensions.h>
#include <CRCore/crTexture3D.h>
#include <CRCore/crState.h>
#include <CRCore/crImageSequence.h>
#include <crgl/GLU.h>
#include <CRCore/crNotify.h>

#include <string.h>



using namespace CRCore;

crTexture3D::crTexture3D():
            m_textureWidth(0),
            m_textureHeight(0),
            m_textureDepth(0),
            m_numMipmapLevels(0)
{
}

crTexture3D::crTexture3D(const crTexture3D& text,const crCopyOp& copyop):
            crTexture(text,copyop),
            m_image(copyop(text.m_image.get())),
            m_textureWidth(text.m_textureWidth),
            m_textureHeight(text.m_textureHeight),
            m_textureDepth(text.m_textureDepth),
            m_numMipmapLevels(text.m_numMipmapLevels),
            m_subloadCallback(text.m_subloadCallback)
{
}

crTexture3D::~crTexture3D()
{
}

int crTexture3D::compare(const crStateAttribute& sa) const
{
    // check the types are equal and then create the rhs variable
    // used by the COMPARE_StateAttribute_Parameter macro's below.
    COMPARE_StateAttribute_Types(crTexture3D,sa)
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
    COMPARE_StateAttribute_Parameter(m_textureDepth)
    COMPARE_StateAttribute_Parameter(m_subloadCallback)

    return 0; // passed all the above comparison macro's, must be equal.
}

int crTexture3D::compare(const crStateAttribute& sa)
{
	// check the types are equal and then create the rhs variable
	// used by the COMPARE_StateAttribute_Parameter macro's below.
	COMPARE_StateAttribute_Types(crTexture3D,sa)
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
	COMPARE_StateAttribute_Parameter(m_textureDepth)
	COMPARE_StateAttribute_Parameter(m_subloadCallback)

	return 0; // passed all the above comparison macro's, must be equal.
}


void crTexture3D::setImage(crImage* image)
{
    if (m_image == image) return;

    if (dynamic_cast<CRCore::crImageSequence*>(m_image.get()))
    {
        setUpdateCallback(0);
        setDataVariance(CRCore::crBase::STATIC);
    }

    // delete old texture objects.
    dirtyTextureObject();

    m_modifiedCount.setAllElementsTo(0);

    m_image = image;
    
    if (dynamic_cast<CRCore::crImageSequence*>(m_image.get()))
    {
        setUpdateCallback(new crImageSequence::UpdateCallback());
        setDataVariance(CRCore::crBase::DYNAMIC);
    }
}

void crTexture3D::computeRequiredTextureDimensions(crState& state, const CRCore::crImage& image,GLsizei& inwidth, GLsizei& inheight,GLsizei& indepth, GLsizei& numMipmapLevels) const
{
    const unsigned int contextID = state.getContextID();
    const Extensions* extensions = getExtensions(contextID,true);
    const crTexture::Extensions* texExtensions = crTexture::getExtensions(contextID,true);

    int width,height,depth;

    if( !m_resizeNonPowerOfTwoHint && texExtensions->isNonPowerOfTwoTextureSupported(m_min_filter) )
    {
        width = image.s();
        height = image.t();
        depth = image.r();
    }
    else
    {
        width = crImage::computeNearestPowerOfTwo(image.s()-2*m_borderWidth)+2*m_borderWidth;
        height = crImage::computeNearestPowerOfTwo(image.t()-2*m_borderWidth)+2*m_borderWidth;
        depth = crImage::computeNearestPowerOfTwo(image.r()-2*m_borderWidth)+2*m_borderWidth;
    }

    // cap the size to what the graphics hardware can handle.
    if (width>extensions->maxTexture3DSize()) width = extensions->maxTexture3DSize();
    if (height>extensions->maxTexture3DSize()) height = extensions->maxTexture3DSize();
    if (depth>extensions->maxTexture3DSize()) depth = extensions->maxTexture3DSize();
    
    inwidth = width;
    inheight = height;
    indepth = depth;
    
    bool useHardwareMipMapGeneration = !image.isMipmap() && m_useHardwareMipMapGeneration && texExtensions->isGenerateMipMapSupported();

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
        for( ; (width || height || depth) ;++numMipmapLevels)
        {

            if (width == 0)
                width = 1;
            if (height == 0)
                height = 1;
            if (depth == 0)
                depth = 1;

            width >>= 1;
            height >>= 1;
            depth >>= 1;
        }    
    }
}

void crTexture3D::apply(crState& state) const
{

    // get the contextID (user defined ID of 0 upwards) for the 
    // current OpenGL context.
    const unsigned int contextID = state.getContextID();
    
    const Extensions* extensions = getExtensions(contextID,true);
                                        
    if (!extensions->isTexture3DSupported())
    {
        notify(WARN)<<"Warning: crTexture3D::apply(..) failed, 3D texturing is not support by OpenGL driver."<<std::endl;
        return;
    }

    // get the texture object for the current contextID.
    TextureObject* textureObject = getTextureObject(contextID);
	double referenceTime = crFrameStamp::getInstance()->getReferenceTime();

    if (textureObject)
    {
		CRCore::ref_ptr<CRCore::crImage> image = m_image;
        // we have a valid image
        textureObject->bind(referenceTime);

        if (getTextureParameterDirty(state.getContextID())) applyTexParameters(GL_TEXTURE_3D,state);

        if (m_subloadCallback.valid())
        {
            m_subloadCallback->subload(*this,state);
        }
        else if (image.get() && getModifiedCount(contextID) != image->getModifiedCount())
        {
           computeRequiredTextureDimensions(state,*image,m_textureWidth, m_textureHeight, m_textureDepth,m_numMipmapLevels);

            applyTexImage3D(GL_TEXTURE_3D,image.get(),state, m_textureWidth, m_textureHeight, m_textureDepth,m_numMipmapLevels);

            // update the modified count to show that it is upto date.
            getModifiedCount(contextID) = image->getModifiedCount();
        }

    }
    else if (m_subloadCallback.valid())
    {

        m_textureObjectBuffer[contextID] = textureObject = generateTextureObject(contextID,GL_TEXTURE_3D);

        textureObject->bind(referenceTime);

        applyTexParameters(GL_TEXTURE_3D,state);

        m_subloadCallback->load(*this,state);

        textureObject->setAllocated(m_numMipmapLevels,m_internalFormat,m_textureWidth,m_textureHeight,m_textureDepth,0);

        // in theory the following line is redundent, but in practice
        // have found that the first frame drawn doesn't apply the textures
        // unless a second bind is called?!!
        // perhaps it is the first glBind which is not required...
        //glBindTexture( GL_TEXTURE_3D, handle );

    }
    else if (m_image.valid() && m_image->data())
    {
		CRCore::ref_ptr<CRCore::crImage> image = m_image;
        // compute the internal texture format, this set the m_internalFormat to an appropriate value.
        computeInternalFormat();

        // compute the dimensions of the texture.
        computeRequiredTextureDimensions(state,*image,m_textureWidth, m_textureHeight, m_textureDepth,m_numMipmapLevels);

        textureObject = generateTextureObject(contextID,GL_TEXTURE_3D);

        textureObject->bind(referenceTime);


        applyTexParameters(GL_TEXTURE_3D,state);

        applyTexImage3D(GL_TEXTURE_3D,image.get(),state, m_textureWidth, m_textureHeight, m_textureDepth,m_numMipmapLevels);

        textureObject->setAllocated(m_numMipmapLevels,m_internalFormat,m_textureWidth,m_textureHeight,m_textureDepth,0);

        // update the modified count to show that it is upto date.
        getModifiedCount(contextID) = image->getModifiedCount();

        m_textureObjectBuffer[contextID] = textureObject;

        if (m_unrefImageDataAfterApply && areAllTextureObjectsLoaded() && image->getDataVariance()==STATIC)
        {
            crTexture3D* non_const_this = const_cast<crTexture3D*>(this);
            non_const_this->m_image = 0;
        }

    }
    else if ( (m_textureWidth!=0) && (m_textureHeight!=0) && (m_textureDepth!=0) && (m_internalFormat!=0) )
    {
        m_textureObjectBuffer[contextID] = textureObject = generateTextureObject(
                contextID,GL_TEXTURE_3D,m_numMipmapLevels,m_internalFormat,m_textureWidth,m_textureHeight,m_textureDepth,0);
        
        textureObject->bind(referenceTime);

        applyTexParameters(GL_TEXTURE_3D,state);

        // no image present, but dimensions at set so lets create the texture
        extensions->glTexImage3D( GL_TEXTURE_3D, 0, m_internalFormat,
                     m_textureWidth, m_textureHeight, m_textureDepth,
                     m_borderWidth,
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
        glBindTexture( GL_TEXTURE_3D, 0 );
    }
    
    // if texture object is now valid and we have to allocate mipmap levels, then
    if (textureObject != 0 && m_texMipmapGenerationDirtyList[contextID])
    {
        generateMipmap(state);
    }
}

void crTexture3D::computeInternalFormat() const
{
    if (m_image.valid()) computeInternalFormatWithImage(*m_image); 
    else computeInternalFormatType();
}

void crTexture3D::applyTexImage3D(GLenum target, crImage* image, crState& state, GLsizei& inwidth, GLsizei& inheight, GLsizei& indepth, GLsizei& numMipmapLevels) const
{
    // if we don't have a valid image we can't create a texture!
    if (!image || !image->data())
        return;

    // get the contextID (user defined ID of 0 upwards) for the 
    // current OpenGL context.
    const unsigned int contextID = state.getContextID();
    const Extensions* extensions = getExtensions(contextID,true);    
    const crTexture::Extensions* texExtensions = crTexture::getExtensions(contextID,true);

    // compute the internal texture format, this set the m_internalFormat to an appropriate value.
    computeInternalFormat();

    // select the internalFormat required for the texture.
    bool compressed = isCompressedInternalFormat(m_internalFormat);
    bool compressed_image = isCompressedInternalFormat((GLenum)image->getPixelFormat());

    if (compressed)
    {
        //notify(WARN)<<"Warning::cannot currently use compressed format with 3D textures."<<std::endl;
        //return;
    }    
    
    //Rescale if resize hint is set or NPOT not supported or dimensions exceed max size
    if( m_resizeNonPowerOfTwoHint || !texExtensions->isNonPowerOfTwoTextureSupported(m_min_filter)
        || inwidth > extensions->maxTexture3DSize()
        || inheight > extensions->maxTexture3DSize()
        || indepth > extensions->maxTexture3DSize() )
        image->ensureValidSizeForTexturing(extensions->maxTexture3DSize());

    glPixelStorei(GL_UNPACK_ALIGNMENT,image->getPacking());

    bool useHardwareMipMapGeneration = !image->isMipmap() && m_useHardwareMipMapGeneration && texExtensions->isGenerateMipMapSupported();

    if( m_min_filter == LINEAR || m_min_filter == NEAREST || useHardwareMipMapGeneration )
    {
        bool hardwareMipMapOn = false;
        if (m_min_filter != LINEAR && m_min_filter != NEAREST) 
        {
            if (useHardwareMipMapGeneration) glTexParameteri(GL_TEXTURE_3D, GL_GENERATE_MIPMAP_SGIS,GL_TRUE);
            hardwareMipMapOn = true;
        }

        numMipmapLevels = 1;

        if (!compressed_image)
        {
            //notify(WARN)<<"glTexImage3D '"<<image->getFileName()<<"' data="<<(void*)image->data()<<std::endl;
            extensions->glTexImage3D( target, 0, m_internalFormat,
                                      inwidth, inheight, indepth,
                                      m_borderWidth,
                                      (GLenum)image->getPixelFormat(),
                                      (GLenum)image->getDataType(),
                                      image->data() );
            //notify(WARN)<<"done glTexImage3D"<<std::endl;
        }
        else if (extensions->isCompressedTexImage3DSupported())
        {
            // notify(WARN)<<"glCompressedTexImage3D "<<inwidth<<", "<<inheight<<", "<<indepth<<std::endl;
            numMipmapLevels = 1;

            GLint blockSize, size;
            getCompressedSize(m_internalFormat, inwidth, inheight, indepth, blockSize,size);

            extensions->glCompressedTexImage3D(target, 0, m_internalFormat, 
                inwidth, inheight, indepth, 
                m_borderWidth,
                size, 
                image->data());
        }

        if (hardwareMipMapOn) glTexParameteri(GL_TEXTURE_3D, GL_GENERATE_MIPMAP_SGIS,GL_FALSE);
    }
    else
    {
        if(!image->isMipmap())
        {

            numMipmapLevels = 1;

            extensions->gluBuild3DMipmaps( target, m_internalFormat,
                                           image->s(),image->t(),image->r(),
                                           (GLenum)image->getPixelFormat(), (GLenum)image->getDataType(),
                                           image->data() );

        }
        else
        {
            numMipmapLevels = image->getNumMipmapLevels();

            int width  = image->s();
            int height = image->t();
            int depth = image->r();

            for( GLsizei k = 0 ; k < numMipmapLevels  && (width || height || depth) ;k++)
            {

                if (width == 0)
                    width = 1;
                if (height == 0)
                    height = 1;
                if (depth == 0)
                    depth = 1;

                extensions->glTexImage3D( target, k, m_internalFormat,
                                          width, height, depth, m_borderWidth,
                                          (GLenum)image->getPixelFormat(),
                                          (GLenum)image->getDataType(),
                                          image->getMipmapData(k));

                width >>= 1;
                height >>= 1;
                depth >>= 1;
            }
        }

    }

    inwidth  = image->s();
    inheight = image->t();
    indepth  = image->r();
    
}

void crTexture3D::copyTexSubImage3D(crState& state, int xoffset, int yoffset, int zoffset, int x, int y, int width, int height )
{
    const unsigned int contextID = state.getContextID();
    const Extensions* extensions = getExtensions(contextID,true);

    // get the texture object for the current contextID.
    TextureObject* textureObject = getTextureObject(contextID);

    if (textureObject != 0)
    {
        textureObject->bind(crFrameStamp::getInstance()->getReferenceTime());

        applyTexParameters(GL_TEXTURE_3D,state);
        extensions->glCopyTexSubImage3D( GL_TEXTURE_3D, 0, xoffset,yoffset,zoffset, x, y, width, height);

        /* Redundant, delete later */
        //glBindTexture( GL_TEXTURE_3D, handle );

        // inform state that this texture is the current one bound.
        state.haveAppliedTextureAttribute(state.getActiveTextureUnit(), this);

    }
    else
    {
        notify(WARN)<<"Warning: crTexture3D::copyTexSubImage3D(..) failed, cannot not copy to a non existant texture."<<std::endl;
    }
}

void crTexture3D::allocateMipmap(crState& state) const
{
    const unsigned int contextID = state.getContextID();

    // get the texture object for the current contextID.
    TextureObject* textureObject = getTextureObject(contextID);
    
    if (textureObject && m_textureWidth != 0 && m_textureHeight != 0 && m_textureDepth != 0)
    {
        const Extensions* extensions = getExtensions(contextID,true);
        const crTexture::Extensions* texExtensions = crTexture::getExtensions(contextID,true);
    
        // bind texture
        textureObject->bind(crFrameStamp::getInstance()->getReferenceTime());

        // compute number of mipmap levels
        int width = m_textureWidth;
        int height = m_textureHeight;
        int depth = m_textureDepth;
        int numMipmapLevels = crImage::computeNumberOfMipmapLevels(width, height, depth);

        // we do not reallocate the level 0, since it was already allocated
        width >>= 1;
        height >>= 1;
        depth >>= 1;
                
        for( GLsizei k = 1; k < numMipmapLevels  && (width || height || depth); k++)
        {
            if (width == 0)
                width = 1;
            if (height == 0)
                height = 1;
            if (depth == 0)
                depth = 1;

            extensions->glTexImage3D( GL_TEXTURE_3D, k, m_internalFormat,
                     width, height, depth, m_borderWidth,
                     m_sourceFormat ? m_sourceFormat : m_internalFormat,
                     m_sourceType ? m_sourceType : GL_UNSIGNED_BYTE, NULL);

            width >>= 1;
            height >>= 1;
            depth >>= 1;
        }
                
        // inform state that this texture is the current one bound.
        state.haveAppliedTextureAttribute(state.getActiveTextureUnit(), this);        
    }
}

typedef buffered_value< ref_ptr<crTexture3D::Extensions> > BufferedExtensions;
static BufferedExtensions s_extensions;
void crTexture3D::Extensions::clearExtensions()
{
	s_extensions.clear();
}
crTexture3D::Extensions* crTexture3D::getExtensions(unsigned int contextID,bool createIfNotInitalized)
{
    if (!s_extensions[contextID] && createIfNotInitalized) s_extensions[contextID] = new Extensions(contextID);
    return s_extensions[contextID].get();
}

void crTexture3D::setExtensions(unsigned int contextID,Extensions* extensions)
{
    s_extensions[contextID] = extensions;
}

#ifndef GL_MAX_3D_TEXTURE_SIZE
#define GL_MAX_3D_TEXTURE_SIZE 0x8073
#endif

crTexture3D::Extensions::Extensions(unsigned int contextID)
{
    setupGLExtensions(contextID);
}

crTexture3D::Extensions::Extensions(const Extensions& rhs):
    Referenced()
{
    m_isTexture3DSupported = rhs.m_isTexture3DSupported;
    m_isTexture3DFast = rhs.m_isTexture3DFast;
    m_maxTexture3DSize = rhs.m_maxTexture3DSize;

    m_glTexImage3D = rhs.m_glTexImage3D;
    m_glTexSubImage3D = rhs.m_glTexSubImage3D;
    m_glCopyTexSubImage3D = rhs.m_glCopyTexSubImage3D;
    m_gluBuild3DMipmaps = rhs.m_gluBuild3DMipmaps;
}

void crTexture3D::Extensions::lowestCommonDenominator(const Extensions& rhs)
{
    if (!rhs.m_isTexture3DSupported)                 m_isTexture3DSupported = false;
    if (!rhs.m_isTexture3DFast)                      m_isTexture3DFast = false;
    if (rhs.m_maxTexture3DSize<m_maxTexture3DSize)    m_maxTexture3DSize = rhs.m_maxTexture3DSize;

    if (!rhs.m_glTexImage3D)                         m_glTexImage3D = 0;
    if (!rhs.m_glTexSubImage3D)                      m_glTexSubImage3D = 0;
    if (!rhs.m_glCompressedTexImage3D)               m_glTexImage3D = 0;
    if (!rhs.m_glCompressedTexSubImage3D)            m_glTexSubImage3D = 0;
    if (!rhs.m_glCopyTexSubImage3D)                  m_glCopyTexSubImage3D = 0;
    if (!rhs.m_gluBuild3DMipmaps)                    m_gluBuild3DMipmaps = 0;
}

void crTexture3D::Extensions::setupGLExtensions(unsigned int contextID)
{
    m_isTexture3DFast = isGLExtensionSupported(contextID,"GL_EXT_texture3D");

    if (m_isTexture3DFast) m_isTexture3DSupported = true;
    else m_isTexture3DSupported = strncmp((const char*)glGetString(GL_VERSION),"1.2",3)>=0;
    
    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &m_maxTexture3DSize);

    m_glTexImage3D                     = getGLExtensionFuncPtr("glTexImage3D","glTexImage3DEXT");
    m_glTexSubImage3D                  = getGLExtensionFuncPtr("glTexSubImage3D","glTexSubImage3DEXT");
    m_glCompressedTexImage3D           = getGLExtensionFuncPtr("glCompressedTexImage3D","glCompressedTexImage3DARB");
    m_glCompressedTexSubImage3D        = getGLExtensionFuncPtr("glCompressedTexSubImage3D","glCompressedTexSubImage3DARB");
    m_glCopyTexSubImage3D              = getGLExtensionFuncPtr("glCopyTexSubImage3D","glCopyTexSubImage3DEXT");
    m_gluBuild3DMipmaps                = getGLExtensionFuncPtr("gluBuild3DMipmaps");

}

void crTexture3D::Extensions::glTexImage3D( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels) const
{
//    ::glTexImage3D( target, level, internalFormat, width, height, depth, border, format, type, pixels);
    if (m_glTexImage3D)
    {
        typedef void (APIENTRY * GLTexImage3DProc)      ( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
        ((GLTexImage3DProc)m_glTexImage3D)( target, level, internalFormat, width, height, depth, border, format, type, pixels);
    }
    else
    {
        notify(WARN)<<"Error: glTexImage3D not supported by OpenGL driver"<<std::endl;
    }
}

void crTexture3D::Extensions::glTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels) const
{
//    ::glTexSubImage3D( target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
    if (m_glTexSubImage3D)
    {
        typedef void (APIENTRY * GLTexSubImage3DProc)   ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
        ((GLTexSubImage3DProc)m_glTexSubImage3D)( target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
    }
    else
    {
        notify(WARN)<<"Error: glTexSubImage3D not supported by OpenGL driver"<<std::endl;
    }
}

void crTexture3D::Extensions::glCompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data) const
{
    if (m_glCompressedTexImage3D)
    {
        typedef void (APIENTRY * CompressedTexImage3DArbProc) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
        ((CompressedTexImage3DArbProc)m_glCompressedTexImage3D)(target, level, internalformat, width, height, depth, border, imageSize, data);
    }
    else
    {
        notify(WARN)<<"Error: glCompressedTexImage3D not supported by OpenGL driver"<<std::endl;
    }
}

void crTexture3D::Extensions::glCompressedTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data ) const
{
    if (m_glCompressedTexSubImage3D)
    {
        typedef void (APIENTRY * CompressedTexSubImage3DArbProc) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
        ((CompressedTexSubImage3DArbProc)m_glCompressedTexSubImage3D)(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
    }
    else
    {
        notify(WARN)<<"Error: glCompressedTexImage2D not supported by OpenGL driver"<<std::endl;
    }
}

void crTexture3D::Extensions::glCopyTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height ) const
{
//    ::glCopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width, height);
    if (m_glCopyTexSubImage3D)
    {
        typedef void (APIENTRY * GLCopyTexSubImageProc) ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height );
        ((GLCopyTexSubImageProc)m_glCopyTexSubImage3D)(target, level, xoffset, yoffset, zoffset, x, y, width, height);
    }
    else
    {
        notify(WARN)<<"Error: glCopyTexSubImage3D not supported by OpenGL driver"<<std::endl;
    }
}

void crTexture3D::Extensions::gluBuild3DMipmaps( GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *data) const
{
//    ::gluBuild3DMipmaps(target, internalFormat, width, height, depth, format, type, data);
    if (m_gluBuild3DMipmaps)
    {
        typedef void (APIENTRY * GLUBuild3DMipMapsProc) ( GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *data);
        ((GLUBuild3DMipMapsProc)m_gluBuild3DMipmaps)(target, internalFormat, width, height, depth, format, type, data);
    }
    else
    {
        notify(WARN)<<"Error: gluBuild3DMipmaps not supported by OpenGL driver"<<std::endl;
    }
}
