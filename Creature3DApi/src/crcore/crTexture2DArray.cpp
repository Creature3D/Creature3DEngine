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
#include <CRCore/crTexture2DArray.h>
#include <CRCore/crState.h>
#include <CRCore/crImageSequence.h>
#include <CRCore/crNotify.h>

#include <string.h>


using namespace CRCore;

crTexture2DArray::crTexture2DArray():
            m_textureWidth(0),
            m_textureHeight(0),
            m_textureDepth(0),
            m_numMipmapLevels(0)
{
}

crTexture2DArray::crTexture2DArray(const crTexture2DArray& text,const crCopyOp& copyop):
            crTexture(text,copyop),
            m_textureWidth(text.m_textureWidth),
            m_textureHeight(text.m_textureHeight),
            m_textureDepth(text.m_textureDepth),
            m_numMipmapLevels(text.m_numMipmapLevels),
            m_subloadCallback(text.m_subloadCallback)
{
    // copy all images by iterating through all of them
    for (int i=0; i < text.m_textureDepth; i++)
    {
        m_images.push_back(copyop(text.m_images[i].get()));
        m_modifiedCount.push_back(ImageModifiedCount());
    }
}

crTexture2DArray::~crTexture2DArray()
{
}

int crTexture2DArray::compare(const crStateAttribute& sa) const
{
    // check the types are equal and then create the rhs variable
    // used by the COMPARE_StateAttribute_Parameter macro's below.
    COMPARE_StateAttribute_Types(crTexture2DArray,sa)
	int result = m_imageNameID.compare(rhs.m_imageNameID);
	if (result!=0) return result;

    //bool noImages = true;
    //for (int n=0; n < m_textureDepth; n++)
    //{
    //    if (noImages && m_images[n].valid()) noImages = false;
    //    if (noImages && rhs.m_images[n].valid()) noImages = false;
    //
    //    if (m_images[n]!=rhs.m_images[n]) // smart pointer comparison.
    //    {
    //        if (m_images[n].valid())
    //        {
    //            if (rhs.m_images[n].valid())
    //            {
    //                int result = m_images[n]->compare(*rhs.m_images[n]);
    //                if (result!=0) return result;
    //            }
    //            else
    //            {
    //                return 1; // valid lhs._image is greater than null. 
    //            }
    //        }
    //        else if (rhs.m_images[n].valid()) 
    //        {
    //            return -1; // valid rhs._image is greater than null. 
    //        }
    //    }
    //}

    
	if (!imagesValid())
    {
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

int crTexture2DArray::compare(const crStateAttribute& sa)
{
	// check the types are equal and then create the rhs variable
	// used by the COMPARE_StateAttribute_Parameter macro's below.
	COMPARE_StateAttribute_Types(crTexture2DArray,sa)
	int result = m_imageNameID.compare(rhs.m_imageNameID);
	if (result!=0) return result;

	//bool noImages = true;
	//for (int n=0; n < m_textureDepth; n++)
	//{
	//	if (noImages && m_images[n].valid()) noImages = false;
	//	if (noImages && rhs.m_images[n].valid()) noImages = false;

	//	if (m_images[n]!=rhs.m_images[n]) // smart pointer comparison.
	//	{
	//		if (m_images[n].valid())
	//		{
	//			if (rhs.m_images[n].valid())
	//			{
	//				int result = m_images[n]->compare(*rhs.m_images[n]);
	//				if (result!=0) return result;
	//			}
	//			else
	//			{
	//				return 1; // valid lhs._image is greater than null. 
	//			}
	//		}
	//		else if (rhs.m_images[n].valid()) 
	//		{
	//			return -1; // valid rhs._image is greater than null. 
	//		}
	//	}
	//}


	if (!imagesValid())
	{
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

void crTexture2DArray::setImage(unsigned int layer, crImage* image)
{
    // check if the layer exceeds the texture depth
    if (static_cast<int>(layer) >= m_textureDepth)
    {
        // print warning and do nothing
        notify(WARN)<<"Warning: crTexture2DArray::setImage(..) failed, the given layer number is bigger then the size of the texture array."<<std::endl;
        return;
    }
    
    if (m_images[layer] == image) return;

    unsigned numImageSequencesBefore = 0;
    for (unsigned int i=0; i<getNumImages(); ++i)
    {
        CRCore::crImageSequence* is = dynamic_cast<CRCore::crImageSequence*>(m_images[i].get());
        if (is) ++numImageSequencesBefore;
    }

    // set image
    m_images[layer] = image;
    m_modifiedCount[layer].setAllElementsTo(0);

    // find out if we need to reset the update callback to handle the animation of crImageSequence
    unsigned numImageSequencesAfter = 0;
    for (unsigned int i=0; i<getNumImages(); ++i)
    {
        CRCore::crImageSequence* is = dynamic_cast<CRCore::crImageSequence*>(m_images[i].get());
        if (is) ++numImageSequencesAfter;
    }

    if (numImageSequencesBefore>0)
    {
        if (numImageSequencesAfter==0)
        {
            setUpdateCallback(0);
            setDataVariance(CRCore::crBase::STATIC);
        }
    }
    else if (numImageSequencesAfter>0)
    {
        setUpdateCallback(new crImageSequence::UpdateCallback());
        setDataVariance(CRCore::crBase::DYNAMIC);
    }
}
 
void crTexture2DArray::setTextureSize(int width, int height, int depth)
{
    // set dimensions
    m_textureWidth = width;
    m_textureHeight = height;
    setTextureDepth(depth);
}

void crTexture2DArray::setTextureDepth(int depth)
{    
    // if we decrease the number of layers, then delete non-used
    if (depth < m_textureDepth)
    {
        m_images.resize(depth);
        m_modifiedCount.resize(depth);
    }
    
    // if we increase the array, then add new empty elements
    if (depth > m_textureDepth)
    {
        m_images.resize(depth, ref_ptr<crImage>(0));
        m_modifiedCount.resize(depth, ImageModifiedCount());
    }
        
    // resize the texture array
    m_textureDepth = depth;
}

crImage* crTexture2DArray::getImage(unsigned int layer)
{
    return m_images[layer].get();
}

const crImage* crTexture2DArray::getImage(unsigned int layer) const
{
    return m_images[layer].get();
}

bool crTexture2DArray::imagesValid() const
{
    if (m_textureDepth < 1) return false;
    for (int n=0; n < m_textureDepth; n++)
    {
        if (!m_images[n].valid() || !m_images[n]->data())
            return false;
    }
    return true;
}

void crTexture2DArray::computeInternalFormat() const
{
    if (imagesValid()) computeInternalFormatWithImage(*m_images[0]); 
    else computeInternalFormatType();
}


void crTexture2DArray::apply(crState& state) const
{
    // get the contextID (user defined ID of 0 upwards) for the 
    // current OpenGL context.
    const unsigned int contextID = state.getContextID();
    const Extensions* extensions = getExtensions(contextID,true);

    // if not supported, then return
    if (!extensions->isTexture2DArraySupported() || !extensions->isTexture3DSupported())
    {
        notify(WARN)<<"Warning: crTexture2DArray::apply(..) failed, 2D texture arrays are not support by OpenGL driver."<<std::endl;
        return;
    }
    
    // get the texture object for the current contextID.
    TextureObject* textureObject = getTextureObject(contextID);

	double referenceTime = crFrameStamp::getInstance()->getReferenceTime();
    // if we already have an texture object, then 
    if (textureObject != 0)
    {
        // bind texture object
        textureObject->bind(referenceTime);

        // if texture parameters changed, then reset them
        if (getTextureParameterDirty(state.getContextID())) applyTexParameters(GL_TEXTURE_2D_ARRAY_EXT,state);

        // if subload is specified, then use it to subload the images to GPU memory
        if (m_subloadCallback.valid())
        {
            m_subloadCallback->subload(*this,state);
        }
        else
        {
            // for each image of the texture array do
			ref_ptr<CRCore::crImage> image;
            for (GLsizei n=0; n < m_textureDepth; n++)
            {
                image = m_images[n].get();
				// if image content is modified, then upload it to the GPU memory
                if (image.valid() && getModifiedCount(n,contextID) != image->getModifiedCount())
                {
                    applyTexImage2DArray_subload(state, image.get(), m_textureWidth, m_textureHeight, n, m_internalFormat, m_numMipmapLevels);
                    getModifiedCount(n,contextID) = image->getModifiedCount();
                }
            }
        }

    }
    
    // there is no texture object, but exists a subload callback, so use it to upload images
    else if (m_subloadCallback.valid())
    {
        // generate texture (i.e. glGenTexture) and apply parameters
        m_textureObjectBuffer[contextID] = textureObject = generateTextureObject(contextID, GL_TEXTURE_2D_ARRAY_EXT);
        textureObject->bind(referenceTime);
        applyTexParameters(GL_TEXTURE_2D_ARRAY_EXT, state);
        m_subloadCallback->load(*this,state);
    }
    
    // nothing before, but we have valid images, so do manual upload and create texture object manually
    // TODO: we assume m_images[0] is valid, however this may not be always the case
    //       some kind of checking for the first valid image is required (Art, may 2008)
    else if (imagesValid())
    {
        // compute the internal texture format, this set the m_internalFormat to an appropriate value.
        computeInternalFormat();

        // compute the dimensions of the texture.
        computeRequiredTextureDimensions(state,*m_images[0],m_textureWidth, m_textureHeight, m_numMipmapLevels);

        // create texture object
        textureObject = generateTextureObject(
                contextID,GL_TEXTURE_2D_ARRAY_EXT,m_numMipmapLevels,m_internalFormat,m_textureWidth,m_textureHeight,m_textureDepth,0);
        
        // bind texture
        textureObject->bind(referenceTime);
        applyTexParameters(GL_TEXTURE_2D_ARRAY_EXT, state);

        // first we need to allocate the texture memory
        extensions->glTexImage3D( GL_TEXTURE_2D_ARRAY_EXT, 0, m_internalFormat,
                     m_textureWidth, m_textureHeight, m_textureDepth,
                     m_borderWidth,
                     m_sourceFormat ? m_sourceFormat : m_internalFormat,
                     m_sourceType ? m_sourceType : GL_UNSIGNED_BYTE,
                     0); 

        // now for each layer we upload it into the memory
        for (GLsizei n=0; n<m_textureDepth; n++)
        {
            // if image is valid then upload it to the texture memory
            CRCore::crImage* image = m_images[n].get();
            if (image)
            {
                // now load the image data into the memory, this will also check if image do have valid properties
                applyTexImage2DArray_subload(state, image, m_textureWidth, m_textureHeight, n, m_internalFormat, m_numMipmapLevels);
                getModifiedCount(n,contextID) = image->getModifiedCount();
            }
        }
        textureObject->setAllocated(m_numMipmapLevels,m_internalFormat,m_textureWidth,m_textureHeight,m_textureDepth,0);

        m_textureObjectBuffer[contextID] = textureObject;
        
        // no idea what this for ;-)
        if (m_unrefImageDataAfterApply && areAllTextureObjectsLoaded())
        {
            crTexture2DArray* non_const_this = const_cast<crTexture2DArray*>(this);
            for (int n=0; n<m_textureDepth; n++)
            {                
                if (m_images[n].valid() && m_images[n]->getDataVariance()==STATIC)
                {
                    non_const_this->m_images[n] = 0;
                }
            }
        }
        
    }
    
    // No images present, but dimensions are set. So create empty texture
    else if ( (m_textureWidth > 0) && (m_textureHeight > 0) && (m_textureDepth > 0) && (m_internalFormat!=0) )
    {
        // generate texture 
        m_textureObjectBuffer[contextID] = textureObject = generateTextureObject(
                contextID, GL_TEXTURE_2D_ARRAY_EXT,m_numMipmapLevels,m_internalFormat,m_textureWidth,m_textureHeight,m_textureDepth,0);
        
        textureObject->bind(referenceTime);
        applyTexParameters(GL_TEXTURE_2D_ARRAY_EXT,state);
        
        extensions->glTexImage3D( GL_TEXTURE_2D_ARRAY_EXT, 0, m_internalFormat,
                     m_textureWidth, m_textureHeight, m_textureDepth,
                     m_borderWidth,
                     m_sourceFormat ? m_sourceFormat : m_internalFormat,
                     m_sourceType ? m_sourceType : GL_UNSIGNED_BYTE,
                     0); 
        
    }
    
    // nothing before, so just unbind the texture target
    else
    {
        glBindTexture( GL_TEXTURE_2D_ARRAY_EXT, 0 );
    }

    // if texture object is now valid and we have to allocate mipmap levels, then
    if (textureObject != 0 && m_texMipmapGenerationDirtyList[contextID])
    {
        generateMipmap(state);
    }
}

void crTexture2DArray::applyTexImage2DArray_subload(crState& state, crImage* image, GLsizei inwidth, GLsizei inheight, GLsizei indepth, GLenum inInternalFormat, GLsizei& numMipmapLevels) const
{
    // if we don't have a valid image we can't create a texture!
    if (!imagesValid())
        return;

    // get the contextID (user defined ID of 0 upwards) for the 
    // current OpenGL context.
    const unsigned int contextID = state.getContextID();
    const Extensions* extensions = getExtensions(contextID,true);    
    const crTexture::Extensions* texExtensions = crTexture::getExtensions(contextID,true);
    GLenum target = GL_TEXTURE_2D_ARRAY_EXT;
    
    // compute the internal texture format, this set the m_internalFormat to an appropriate value.
    computeInternalFormat();

    // select the internalFormat required for the texture.
    // bool compressed = isCompressedInternalFormat(m_internalFormat);
    bool compressed_image = isCompressedInternalFormat((GLenum)image->getPixelFormat());

    // if the required layer is exceeds the maximum allowed layer sizes
    if (indepth > extensions->maxLayerCount())
    {
        // we give a warning and do nothing
        notify(WARN)<<"Warning: crTexture2DArray::applyTexImage2DArray_subload(..) the given layer number exceeds the maximum number of supported layers."<<std::endl;
        return;        
    }

    //Rescale if resize hint is set or NPOT not supported or dimensions exceed max size
    if( m_resizeNonPowerOfTwoHint || !texExtensions->isNonPowerOfTwoTextureSupported(m_min_filter)
        || inwidth > extensions->max2DSize()
        || inheight > extensions->max2DSize())
        image->ensureValidSizeForTexturing(extensions->max2DSize());

    // image size or format has changed, this is not allowed, hence return
    if (image->s()!=inwidth || image->t()!=inheight || image->getInternalTextureFormat()!=inInternalFormat ) 
    {
        notify(WARN)<<"Warning: crTexture2DArray::applyTexImage2DArray_subload(..) given image do have wrong dimension or internal format."<<std::endl;
        return;        
    }    
    
    glPixelStorei(GL_UNPACK_ALIGNMENT,image->getPacking());

    // if no special mipmapping is required, then
    if( m_min_filter == LINEAR || m_min_filter == NEAREST )
    {
        numMipmapLevels = 1;

        // upload non-compressed image
        if (!compressed_image)
        {
            extensions->glTexSubImage3D( target, 0,
                                      0, 0, indepth,
                                      inwidth, inheight, 1,
                                      (GLenum)image->getPixelFormat(),
                                      (GLenum)image->getDataType(),
                                      image->data() );
        }
        
        // if we support compression and image is compressed, then
        else if (extensions->isCompressedTexImage3DSupported())
        {
            // notify(WARN)<<"glCompressedTexImage3D "<<inwidth<<", "<<inheight<<", "<<indepth<<std::endl;
            numMipmapLevels = 1;

            GLint blockSize, size;
            getCompressedSize(m_internalFormat, inwidth, inheight, indepth, blockSize,size);

            extensions->glCompressedTexSubImage3D(target, 0,
                0, 0, indepth,  
                inwidth, inheight, 1, 
                (GLenum)image->getPixelFormat(),
                size, 
                image->data());
        }

    // we want to use mipmapping, so enable it
    }else
    {
        // image does not provide mipmaps, so we have to create them
        if(!image->isMipmap())
        {
            notify(WARN)<<"Warning: crTexture2DArray::applyTexImage2DArray_subload(..) automagic mipmap generation is currently not implemented. Check texture's min/mag filters."<<std::endl;

        // the image object does provide mipmaps, so upload the in the certain levels of a layer
        }else
        {
            numMipmapLevels = image->getNumMipmapLevels();

            int width  = image->s();
            int height = image->t();
            
            for( GLsizei k = 0 ; k < numMipmapLevels  && (width || height ) ;k++)
            {

                if (width == 0)
                    width = 1;
                if (height == 0)
                    height = 1;

                extensions->glTexSubImage3D( target, k, 0, 0, indepth,
                                          width, height, 1, 
                                          (GLenum)image->getPixelFormat(),
                                          (GLenum)image->getDataType(),
                                          image->getMipmapData(k));

                width >>= 1;
                height >>= 1;
            }
        }

    }
}


void crTexture2DArray::copyTexSubImage2DArray(crState& state, int xoffset, int yoffset, int zoffset, int x, int y, int width, int height )
{
    const unsigned int contextID = state.getContextID();
    const Extensions* extensions = getExtensions(contextID,true);

    // get the texture object for the current contextID.
    TextureObject* textureObject = getTextureObject(contextID);

    // if texture object is valid
    if (textureObject != 0)
    {
        textureObject->bind(crFrameStamp::getInstance()->getReferenceTime());

        applyTexParameters(GL_TEXTURE_2D_ARRAY_EXT,state);
        extensions->glCopyTexSubImage3D( GL_TEXTURE_2D_ARRAY_EXT, 0, xoffset,yoffset,zoffset, x, y, width, height);

        // inform state that this texture is the current one bound.
        state.haveAppliedTextureAttribute(state.getActiveTextureUnit(), this);

    }
    else
    {
        notify(WARN)<<"Warning: crTexture2DArray::copyTexSubImage2DArray(..) failed, cannot not copy to a non existant texture."<<std::endl;
    }
}

void crTexture2DArray::allocateMipmap(crState& state) const
{
    const unsigned int contextID = state.getContextID();

    // get the texture object for the current contextID.
    TextureObject* textureObject = getTextureObject(contextID);
    
    if (textureObject && m_textureWidth != 0 && m_textureHeight != 0 && m_textureDepth != 0)
    {
        const Extensions* extensions = getExtensions(contextID,true);
        // const crTexture::Extensions* texExtensions = crTexture::getExtensions(contextID,true);
        
        // bind texture
        textureObject->bind(crFrameStamp::getInstance()->getReferenceTime());

        // compute number of mipmap levels
        int width = m_textureWidth;
        int height = m_textureHeight;
        int numMipmapLevels = crImage::computeNumberOfMipmapLevels(width, height);

        // we do not reallocate the level 0, since it was already allocated
        width >>= 1;
        height >>= 1;
        
        for( GLsizei k = 1; k < numMipmapLevels  && (width || height); k++)
        {
            if (width == 0)
                width = 1;
            if (height == 0)
                height = 1;

            extensions->glTexImage3D( GL_TEXTURE_2D_ARRAY_EXT, k, m_internalFormat,
                     width, height, m_textureDepth, m_borderWidth,
                     m_sourceFormat ? m_sourceFormat : m_internalFormat,
                     m_sourceType ? m_sourceType : GL_UNSIGNED_BYTE, NULL);

            width >>= 1;
            height >>= 1;
        }
                
        // inform state that this texture is the current one bound.
        state.haveAppliedTextureAttribute(state.getActiveTextureUnit(), this);        
    }
}

typedef buffered_value< ref_ptr<crTexture2DArray::Extensions> > BufferedExtensions;
static BufferedExtensions s_extensions;
void crTexture2DArray::Extensions::clearExtensions()
{
	s_extensions.clear();
}
crTexture2DArray::Extensions* crTexture2DArray::getExtensions(unsigned int contextID,bool createIfNotInitalized)
{
    if (!s_extensions[contextID] && createIfNotInitalized) s_extensions[contextID] = new Extensions(contextID);
    return s_extensions[contextID].get();
}

void crTexture2DArray::setExtensions(unsigned int contextID,Extensions* extensions)
{
    s_extensions[contextID] = extensions;
}

crTexture2DArray::Extensions::Extensions(unsigned int contextID)
{
    setupGLExtensions(contextID);
}

crTexture2DArray::Extensions::Extensions(const Extensions& rhs):
    Referenced()
{
    m_isTexture3DSupported = rhs.m_isTexture3DSupported;
    m_isTexture2DArraySupported = rhs.m_isTexture2DArraySupported;
    
    m_max2DSize = rhs.m_max2DSize;
    m_maxLayerCount = rhs.m_maxLayerCount;
    
    m_glTexImage3D = rhs.m_glTexImage3D;
    m_glTexSubImage3D = rhs.m_glTexSubImage3D;
    m_glCopyTexSubImage3D = rhs.m_glCopyTexSubImage3D;
    m_glCompressedTexImage3D = rhs.m_glCompressedTexImage3D;
    m_glCompressedTexSubImage3D = rhs.m_glCompressedTexSubImage3D;;
}

void crTexture2DArray::Extensions::lowestCommonDenominator(const Extensions& rhs)
{
    if (!rhs.m_isTexture3DSupported)                 m_isTexture3DSupported = false;
    if (!rhs.m_isTexture2DArraySupported)            m_isTexture2DArraySupported = false;
    if (rhs.m_max2DSize<m_max2DSize)                  m_max2DSize = rhs.m_max2DSize;
    if (rhs.m_maxLayerCount<m_maxLayerCount)          m_maxLayerCount = rhs.m_maxLayerCount;

    if (!rhs.m_glTexImage3D)                         m_glTexImage3D = 0;
    if (!rhs.m_glTexSubImage3D)                      m_glTexSubImage3D = 0;
    if (!rhs.m_glCompressedTexImage3D)               m_glTexImage3D = 0;
    if (!rhs.m_glCompressedTexSubImage3D)            m_glTexSubImage3D = 0;
    if (!rhs.m_glCopyTexSubImage3D)                  m_glCopyTexSubImage3D = 0;
}

void crTexture2DArray::Extensions::setupGLExtensions(unsigned int contextID)
{
    m_isTexture3DSupported = isGLExtensionSupported(contextID,"GL_EXT_texture3D");
    m_isTexture2DArraySupported = isGLExtensionSupported(contextID,"GL_EXT_texture_array");

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_max2DSize);
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS_EXT, &m_maxLayerCount);

    setGLExtensionFuncPtr(m_glTexImage3D, "glTexImage3D","glTexImage3DEXT");
    setGLExtensionFuncPtr(m_glTexSubImage3D, "glTexSubImage3D","glTexSubImage3DEXT");
    setGLExtensionFuncPtr(m_glCompressedTexImage3D, "glCompressedTexImage3D","glCompressedTexImage3DARB");
    setGLExtensionFuncPtr(m_glCompressedTexSubImage3D, "glCompressedTexSubImage3D","glCompressedTexSubImage3DARB");
    setGLExtensionFuncPtr(m_glCopyTexSubImage3D, "glCopyTexSubImage3D","glCopyTexSubImage3DEXT");
}

void crTexture2DArray::Extensions::glTexImage3D( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels) const
{
    if (m_glTexImage3D)
    {
        m_glTexImage3D( target, level, internalFormat, width, height, depth, border, format, type, pixels);
    }
    else
    {
        notify(WARN)<<"Error: glTexImage3D not supported by OpenGL driver"<<std::endl;
    }
}

void crTexture2DArray::Extensions::glTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels) const
{
    if (m_glTexSubImage3D)
    {
        m_glTexSubImage3D( target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
    }
    else
    {
        notify(WARN)<<"Error: glTexSubImage3D not supported by OpenGL driver"<<std::endl;
    }
}

void crTexture2DArray::Extensions::glCompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data) const
{
    if (m_glCompressedTexImage3D)
    {
        m_glCompressedTexImage3D(target, level, internalformat, width, height, depth, border, imageSize, data);
    }
    else
    {
        notify(WARN)<<"Error: glCompressedTexImage3D not supported by OpenGL driver"<<std::endl;
    }
}

void crTexture2DArray::Extensions::glCompressedTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data ) const
{
    if (m_glCompressedTexSubImage3D)
    {
        m_glCompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
    }
    else
    {
        notify(WARN)<<"Error: glCompressedTexImage2D not supported by OpenGL driver"<<std::endl;
    }
}

void crTexture2DArray::Extensions::glCopyTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height ) const
{
    if (m_glCopyTexSubImage3D)
    {
        m_glCopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width, height);
    }
    else
    {
        notify(WARN)<<"Error: glCopyTexSubImage3D not supported by OpenGL driver"<<std::endl;
    }
}

