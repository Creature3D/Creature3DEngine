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
#include <CRCore/crTexture1D.h>
#include <CRCore/crImageSequence.h>
#include <CRCore/crState.h>
#include <crgl/GLU.h>

typedef void (APIENTRY * MyCompressedTexImage1DArbProc) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);

using namespace CRCore;

crTexture1D::crTexture1D():
            m_textureWidth(0),
            m_numMipmapLevels(0)
{
}

crTexture1D::crTexture1D(const crTexture1D& text,const crCopyOp& copyop):
            crTexture(text,copyop),
            m_image(copyop(text.m_image.get())),
            m_textureWidth(text.m_textureWidth),
            m_numMipmapLevels(text.m_numMipmapLevels),
            m_subloadCallback(text.m_subloadCallback)
{
}

crTexture1D::~crTexture1D()
{
}

int crTexture1D::compare(const crStateAttribute& sa) const
{
    // check the types are equal and then create the rhs variable
    // used by the COMPARE_StateAttribute_Parameter macro's below.
    COMPARE_StateAttribute_Types(crTexture1D,sa)
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
    COMPARE_StateAttribute_Parameter(m_subloadCallback)

    return 0; // passed all the above comparison macro's, must be equal.
}

int crTexture1D::compare(const crStateAttribute& sa)
{
	// check the types are equal and then create the rhs variable
	// used by the COMPARE_StateAttribute_Parameter macro's below.
	COMPARE_StateAttribute_Types(crTexture1D,sa)
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
	COMPARE_StateAttribute_Parameter(m_subloadCallback)

	return 0; // passed all the above comparison macro's, must be equal.
}


void crTexture1D::setImage(crImage* image)
{
    if (m_image == image) return;

	//if(image) m_imageNameID = image->getFileName();

    if (dynamic_cast<CRCore::crImageSequence*>(m_image.get()))
    {
        setUpdateCallback(0);
        setDataVariance(CRCore::crBase::STATIC);
    }

    // delete old texture objects.
    dirtyTextureObject();

    m_image = image;
    m_modifiedCount.setAllElementsTo(0);
    
    if (dynamic_cast<CRCore::crImageSequence*>(m_image.get()))
    {
        setUpdateCallback(new crImageSequence::UpdateCallback());
        setDataVariance(CRCore::crBase::DYNAMIC);
    }
}


void crTexture1D::apply(crState& state) const
{

    // get the contextID (user defined ID of 0 upwards) for the 
    // current OpenGL context.
    const unsigned int contextID = state.getContextID();

    // get the texture object for the current contextID.
	TextureObject* textureObject = getTextureObject(contextID);

	double referenceTime = crFrameStamp::getInstance()->getReferenceTime();
	if (textureObject != 0)
	{
		CRCore::ref_ptr<CRCore::crImage> image = m_image;
		int tostate = textureObject->getStateInfo();
		if(tostate != 0)
		{
			reuseTextureObject(contextID, textureObject);
		}
		if(tostate==1&&getNeedCompile())
		{
			crTexture1D* non_const_tex = const_cast<crTexture1D*>(this);
			non_const_tex->setNeedCompile(false);
			if(image.valid() && image->data())
			{
				getModifiedCount(contextID) = image->getModifiedCount();
				if (m_unrefImageDataAfterApply && areAllTextureObjectsLoaded() && image->getDataVariance()==crBase::STATIC)
				{
					non_const_tex->setImage(NULL);
				}
			}
		}
		if(tostate!=2)
		{
			textureObject->bind(referenceTime);

			if (getTextureParameterDirty(state.getContextID()))
				applyTexParameters(GL_TEXTURE_1D,state);

			if (m_subloadCallback.valid())
			{
				m_subloadCallback->subload(*this,state);
			}
			else if (image.valid() && image->data() && getModifiedCount(contextID) != image->getModifiedCount())
			{
				applyTexImage1D(GL_TEXTURE_1D,image.get(),state, m_textureWidth, m_numMipmapLevels);

				// update the modified tag to show that it is up to date.
				getModifiedCount(contextID) = image->getModifiedCount();

			}
		}
		else 
		{
			crTexture1D* non_const_tex = const_cast<crTexture1D*>(this);
			if(!image||!image->data())
			{
				image = getImageDataRequestHandler()->requestImageFile(getImageNameID(),non_const_tex);
				non_const_tex->m_image = image;
			}
			if(image.valid() && image->data())
			{
				GLuint id;
				glGenTextures( 1L, &id );
				textureObject->m_id = id;
				textureObject->bind(referenceTime);
				textureObject->setStateInfo(0);
				applyTexParameters(GL_TEXTURE_1D,state);

				//std::cout<<"Creating new texture object"<<std::endl;
				applyTexImage1D(GL_TEXTURE_1D,m_image.get(),state, m_textureWidth, m_numMipmapLevels);

				textureObject->setAllocated(m_numMipmapLevels,m_internalFormat,m_textureWidth,1,1,0);

				// update the modified tag to show that it is upto date.
				getModifiedCount(contextID) = m_image->getModifiedCount();

				if (m_unrefImageDataAfterApply && areAllTextureObjectsLoaded() && m_image->getDataVariance()==crBase::STATIC)
				{
					non_const_tex->setImage(NULL);
				}
			}
		}
	}
    else if (m_subloadCallback.valid())
    {

        // we don't have a applyTexImage1D_subload yet so can't reuse.. so just generate a new texture object.        
        m_textureObjectBuffer[contextID] = textureObject = generateTextureObject(contextID,GL_TEXTURE_1D);

        textureObject->bind(referenceTime);

        applyTexParameters(GL_TEXTURE_1D,state);

        m_subloadCallback->load(*this,state);

        textureObject->setAllocated(m_numMipmapLevels,m_internalFormat,m_textureWidth,1,1,0);

        // in theory the following line is redundent, but in practice
        // have found that the first frame drawn doesn't apply the textures
        // unless a second bind is called?!!
        // perhaps it is the first glBind which is not required...
        //glBindTexture( GL_TEXTURE_1D, handle );

    }
    else if (m_image.valid() && m_image->data())
    {
		CRCore::ref_ptr<CRCore::crImage> image = m_image;
        // we don't have a applyTexImage1D_subload yet so can't reuse.. so just generate a new texture object.        
        textureObject = generateTextureObject(contextID,GL_TEXTURE_1D);

        textureObject->bind(referenceTime);

        applyTexParameters(GL_TEXTURE_1D,state);

        applyTexImage1D(GL_TEXTURE_1D,image.get(),state, m_textureWidth, m_numMipmapLevels);

        textureObject->setAllocated(m_numMipmapLevels,m_internalFormat,m_textureWidth,1,1,0);

        // update the modified count to show that it is upto date.
        getModifiedCount(contextID) = image->getModifiedCount();
    
        m_textureObjectBuffer[contextID] = textureObject;
    
        if (m_unrefImageDataAfterApply && areAllTextureObjectsLoaded() && image->getDataVariance()==STATIC)
        {
            crTexture1D* non_const_this = const_cast<crTexture1D*>(this);
            non_const_this->m_image = 0;
        }
        
    }
    else if ( (m_textureWidth!=0) && (m_internalFormat!=0) )
    {
        m_textureObjectBuffer[contextID] = textureObject = generateTextureObject(
                contextID,GL_TEXTURE_1D,m_numMipmapLevels,m_internalFormat,m_textureWidth,1,1,0);
        
        textureObject->bind(referenceTime);

        applyTexParameters(GL_TEXTURE_1D,state);

        // no image present, but dimensions are set so lets create the texture
        glTexImage1D( GL_TEXTURE_1D, 0, m_internalFormat,
                     m_textureWidth, m_borderWidth,
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
        glBindTexture( GL_TEXTURE_1D, 0 );
    }

    // if texture object is now valid and we have to allocate mipmap levels, then
    if (textureObject != 0 && m_texMipmapGenerationDirtyList[contextID])
    {
        generateMipmap(state);
    }
}

void crTexture1D::computeInternalFormat() const
{
    if (m_image.valid()) computeInternalFormatWithImage(*m_image); 
    else computeInternalFormatType();
}

void crTexture1D::applyTexImage1D(GLenum target, crImage* image, crState& state, GLsizei& inwidth, GLsizei& numMipmapLevels) const
{
    // if we don't have a valid image we can't create a texture!
    if (!image || !image->data())
        return;

    // get the contextID (user defined ID of 0 upwards) for the 
    // current OpenGL context.
    const unsigned int contextID = state.getContextID();
    const Extensions* extensions = getExtensions(contextID,true);


    // compute the internal texture format, this set the m_internalFormat to an appropriate value.
    computeInternalFormat();

    // select the internalFormat required for the texture.
    bool compressed = isCompressedInternalFormat(m_internalFormat);
    
    //Rescale if resize hint is set or NPOT not supported or dimension exceeds max size
    if( m_resizeNonPowerOfTwoHint || !extensions->isNonPowerOfTwoTextureSupported(m_min_filter) || inwidth > extensions->maxTextureSize() )
    {
        // this is not thread safe... should really create local image data and rescale to that as per Texture2D.
        image->ensureValidSizeForTexturing(extensions->maxTextureSize());
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT,image->getPacking());

    static MyCompressedTexImage1DArbProc glCompressedTexImage1D_ptr = 
        (MyCompressedTexImage1DArbProc)getGLExtensionFuncPtr("glCompressedTexImage1DARB");

    if( m_min_filter == LINEAR || m_min_filter == NEAREST )
    {
        if ( !compressed )
        {
            numMipmapLevels = 1;
            glTexImage1D( target, 0, m_internalFormat,
                image->s(), m_borderWidth,
                (GLenum)image->getPixelFormat(),
                (GLenum)image->getDataType(),
                image->data() );

        }
        else if(glCompressedTexImage1D_ptr)
        {
            numMipmapLevels = 1;
            GLint blockSize = ( m_internalFormat == GL_COMPRESSED_RGB_S3TC_DXT1_EXT ? 8 : 16 ); 
            GLint size = ((image->s()+3)/4)*((image->t()+3)/4)*blockSize;
            glCompressedTexImage1D_ptr(target, 0, m_internalFormat, 
                  image->s(), m_borderWidth, 
                  size, 
                  image->data());                

        }

    }
    else
    {
        if(!image->isMipmap())
        {

            numMipmapLevels = 1;

            gluBuild1DMipmaps( target, m_internalFormat,
                image->s(),
                (GLenum)image->getPixelFormat(), (GLenum)image->getDataType(),
                image->data() );

        }
        else
        {
            numMipmapLevels = image->getNumMipmapLevels();

            int width  = image->s();

            if( !compressed )
            {
                for( GLsizei k = 0 ; k < numMipmapLevels  && width ;k++)
                {

                    glTexImage1D( target, k, m_internalFormat,
                         width,m_borderWidth,
                        (GLenum)image->getPixelFormat(),
                        (GLenum)image->getDataType(),
                        image->getMipmapData(k));

                    width >>= 1;
                }
            }
            else if(glCompressedTexImage1D_ptr)
            {
                GLint blockSize = ( m_internalFormat == GL_COMPRESSED_RGB_S3TC_DXT1_EXT ? 8 : 16 ); 
                GLint size = 0; 
                for( GLsizei k = 0 ; k < numMipmapLevels  && width ;k++)
                {

                    size = ((width+3)/4)*blockSize;
                    glCompressedTexImage1D_ptr(target, k, m_internalFormat, 
                        width,  m_borderWidth, size, image->getMipmapData(k));                

                    width >>= 1;
                }
            }
        }

    }

    inwidth = image->s();
}

void crTexture1D::copyTexImage1D(crState& state, int x, int y, int width)
{
    const unsigned int contextID = state.getContextID();

    // get the texture object for the current contextID.
    TextureObject* textureObject = getTextureObject(contextID);

    if (textureObject != 0)
    {
        if (width==(int)m_textureWidth)
        {
            // we have a valid texture object which is the right size
            // so lets play clever and use copyTexSubImage1D instead.
            // this allows use to reuse the texture object and avoid
            // expensive memory allocations.
            copyTexSubImage1D(state,0 ,x, y, width);
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
    m_min_filter = LINEAR;
    m_mag_filter = LINEAR;

    m_textureObjectBuffer[contextID] = textureObject = generateTextureObject(contextID,GL_TEXTURE_1D);

    textureObject->bind(crFrameStamp::getInstance()->getReferenceTime());


    applyTexParameters(GL_TEXTURE_1D,state);
    glCopyTexImage1D( GL_TEXTURE_1D, 0, GL_RGBA, x, y, width, 0 );

    m_textureWidth = width;
    m_numMipmapLevels = 1;
    
    textureObject->setAllocated(m_numMipmapLevels,m_internalFormat,m_textureWidth,1,1,0);

    // inform state that this texture is the current one bound.
    state.haveAppliedTextureAttribute(state.getActiveTextureUnit(), this);
}

void crTexture1D::copyTexSubImage1D(crState& state, int xoffset, int x, int y, int width)
{
    const unsigned int contextID = state.getContextID();

    // get the texture object for the current contextID.
    TextureObject* textureObject = getTextureObject(contextID);

    if (textureObject != 0)
    {

        textureObject->bind(crFrameStamp::getInstance()->getReferenceTime());

        // we have a valid image
        applyTexParameters(GL_TEXTURE_1D,state);
        glCopyTexSubImage1D( GL_TEXTURE_1D, 0, xoffset, x, y, width);

        // inform state that this texture is the current one bound.
        state.haveAppliedTextureAttribute(state.getActiveTextureUnit(), this);

    }
    else
    {
        // no texture object already exsits for this context so need to
        // create it upfront - simply call copyTexImage1D.
        copyTexImage1D(state,x,y,width);
    }
}

void crTexture1D::allocateMipmap(crState& state) const
{
    const unsigned int contextID = state.getContextID();

    // get the texture object for the current contextID.
    TextureObject* textureObject = getTextureObject(contextID);
    
    if (textureObject && m_textureWidth != 0)
    {
        // bind texture
        textureObject->bind(crFrameStamp::getInstance()->getReferenceTime());

        // compute number of mipmap levels
        int width = m_textureWidth;
        int numMipmapLevels = crImage::computeNumberOfMipmapLevels(width);

        // we do not reallocate the level 0, since it was already allocated
        width >>= 1;
        
        for( GLsizei k = 1; k < numMipmapLevels  && width; k++)
        {
            if (width == 0)
                width = 1;

            glTexImage1D( GL_TEXTURE_1D, k, m_internalFormat,
                     width, m_borderWidth,
                     m_sourceFormat ? m_sourceFormat : m_internalFormat,
                     m_sourceType ? m_sourceType : GL_UNSIGNED_BYTE, NULL);

            width >>= 1;
        }
                
        // inform state that this texture is the current one bound.
        state.haveAppliedTextureAttribute(state.getActiveTextureUnit(), this);        
    }
}
