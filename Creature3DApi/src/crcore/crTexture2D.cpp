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
#include <CRCore/crTexture2D.h>
#include <CRCore/crImageSequence.h>
#include <CRCore/crState.h>
#include <CRCore/crNotify.h>
#include <crgl/GLU.h>
#include <CRCore/crStreamBuf.h>
using namespace CRCore;

crTexture2D::crTexture2D():
            m_textureWidth(0),
            m_textureHeight(0),
            m_numMipmapLevels(0)
{
    setUseHardwareMipMapGeneration(true);
}

crTexture2D::crTexture2D(crImage* image):
            m_textureWidth(0),
            m_textureHeight(0),
            m_numMipmapLevels(0)
{
    setUseHardwareMipMapGeneration(true);
    setImage(image);
}

crTexture2D::crTexture2D(const crTexture2D& text,const crCopyOp& copyop):
            crTexture(text,copyop),
            m_image(copyop(text.m_image.get())),
            m_textureWidth(text.m_textureWidth),
            m_textureHeight(text.m_textureHeight),
            m_numMipmapLevels(text.m_numMipmapLevels),
            m_subloadCallback(text.m_subloadCallback)
{
}

crTexture2D::~crTexture2D()
{
}

int crTexture2D::compare(const crStateAttribute& sa) const
{
    // check the types are equal and then create the rhs variable
    // used by the COMPARE_StateAttribute_Parameter macro's below.
    COMPARE_StateAttribute_Types(crTexture2D,sa)
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
        // no image attached to either crTexture2D
        // but could these textures already be downloaded?
        // check the m_textureObjectBuffer to see if they have been
        // downloaded

        result = compareTextureObjects(rhs);
        if (result!=0) return result;
    }

    result = compareTexture(rhs);
    if (result!=0) return result;

    // compare each parameter in turn against the rhs.
#if 1    
    if (m_textureWidth != 0 && rhs.m_textureWidth != 0)
    {
        COMPARE_StateAttribute_Parameter(m_textureWidth)
    }
    if (m_textureHeight != 0 && rhs.m_textureHeight != 0)
    {
        COMPARE_StateAttribute_Parameter(m_textureHeight)
    }
#endif
    COMPARE_StateAttribute_Parameter(m_subloadCallback)

    return 0; // passed all the above comparison macro's, must be equal.
}

int crTexture2D::compare(const crStateAttribute& sa)
{
	// check the types are equal and then create the rhs variable
	// used by the COMPARE_StateAttribute_Parameter macro's below.
	COMPARE_StateAttribute_Types(crTexture2D,sa)
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
		// no image attached to either crTexture2D
		// but could these textures already be downloaded?
		// check the m_textureObjectBuffer to see if they have been
		// downloaded

		result = compareTextureObjects(rhs);
		if (result!=0) return result;
	}

	result = compareTexture(rhs);
	if (result!=0) return result;

	// compare each parameter in turn against the rhs.
#if 1    
	if (m_textureWidth != 0 && rhs.m_textureWidth != 0)
	{
		COMPARE_StateAttribute_Parameter(m_textureWidth)
	}
	if (m_textureHeight != 0 && rhs.m_textureHeight != 0)
	{
		COMPARE_StateAttribute_Parameter(m_textureHeight)
	}
#endif
	COMPARE_StateAttribute_Parameter(m_subloadCallback)

	return 0; // passed all the above comparison macro's, must be equal.
}


void crTexture2D::setImage(crImage* image)
{
    if (m_image == image) return;

	//if(image) m_imageNameID = image->getFileName();

    if (dynamic_cast<CRCore::crImageSequence*>(m_image.get()))
    {
        setUpdateCallback(0);
        setDataVariance(CRCore::crBase::STATIC);
    }

    m_image = image;
    m_modifiedCount.setAllElementsTo(0);
    
    if (dynamic_cast<CRCore::crImageSequence*>(m_image.get()))
    {
        setUpdateCallback(new crImageSequence::UpdateCallback());
        setDataVariance(CRCore::crBase::DYNAMIC);
    }
	//setNeedCompile(true);
	//m_textureObjectBuffer.clear();
	//dirtyTextureObject();
}


void crTexture2D::apply(crState& state) const
{

    //state.setReportGLErrors(true);

    // get the contextID (user defined ID of 0 upwards) for the 
    // current OpenGL context.
    const unsigned int contextID = state.getContextID();

    // get the texture object for the current contextID.
    TextureObject* textureObject = getTextureObject(contextID);

	double referenceTime = crFrameStamp::getInstance()->getReferenceTime();
    if (textureObject)
    {
		CRCore::ref_ptr<CRCore::crImage> image = m_image;
		int tostate = textureObject->getStateInfo();
		if(tostate != 0)
		{
			reuseTextureObject(contextID, textureObject);
		}
		if(tostate==1&&getNeedCompile())
		{
			crTexture2D* non_const_tex = const_cast<crTexture2D*>(this);
			non_const_tex->setNeedCompile(false);
			if(image.valid() && image->data())
			{
				getModifiedCount(contextID) = image->getModifiedCount();
				if (getUnRefImageDataAfterApply() && areAllTextureObjectsLoaded() && image->getDataVariance()==crBase::STATIC)
				{
					non_const_tex->setImage(NULL);
				}
			}
		}
		if(tostate!=2)
		{
			textureObject->bind(referenceTime);

			if (getTextureParameterDirty(state.getContextID()))
				applyTexParameters(GL_TEXTURE_2D,state);

			if (m_subloadCallback.valid())
			{
				m_subloadCallback->subload(*this,state);
			}
			else if (image.valid() && image->data() && getModifiedCount(contextID) != image->getModifiedCount())
			{
				applyTexImage2D_subload(state,GL_TEXTURE_2D,image.get(),
					m_textureWidth, m_textureHeight, m_internalFormat, m_numMipmapLevels);

				// update the modified tag to show that it is up to date.
				getModifiedCount(contextID) = image->getModifiedCount();
			}
			else if (m_readPBuffer.valid())
			{
				m_readPBuffer->bindPBufferToTexture(GL_FRONT);
			}
		}
		else 
		{
		    crTexture2D* non_const_tex = const_cast<crTexture2D*>(this);
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
				applyTexParameters(GL_TEXTURE_2D,state);

				//std::cout<<"Creating new texture object"<<std::endl;
				applyTexImage2D_load( state, GL_TEXTURE_2D, image.get(),
					m_textureWidth, m_textureHeight, m_numMipmapLevels );

				textureObject->setAllocated(true);

				// update the modified tag to show that it is upto date.
				getModifiedCount(contextID) = image->getModifiedCount();

				if (getUnRefImageDataAfterApply() && areAllTextureObjectsLoaded() && image->getDataVariance()==crBase::STATIC)
				{
					non_const_tex->setImage(NULL);
				}

				if (m_readPBuffer.valid())
				{
					m_readPBuffer->bindPBufferToTexture(GL_FRONT);
				}
			}
		}
    }
    else if (m_subloadCallback.valid())
    {

        m_textureObjectBuffer[contextID] = textureObject = generateTextureObject(contextID,GL_TEXTURE_2D);

		textureObject->bind(referenceTime);

        applyTexParameters(GL_TEXTURE_2D,state);

        m_subloadCallback->load(*this,state);
        
        textureObject->setAllocated(m_numMipmapLevels,m_internalFormat,m_textureWidth,m_textureHeight,1,0);

        // in theory the following line is redundent, but in practice
        // have found that the first frame drawn doesn't apply the textures
        // unless a second bind is called?!!
        // perhaps it is the first glBind which is not required...
        //glBindTexture( GL_TEXTURE_2D, handle );

    }
    else if (m_image.valid() && m_image->data())
    {

        // keep the image around at least till we go out of scope.
        CRCore::ref_ptr<CRCore::crImage> image = m_image;

        // compute the internal texture format, this set the m_internalFormat to an appropriate value.
        computeInternalFormat();
        
        // compute the dimensions of the texture.
        computeRequiredTextureDimensions(state,*image,m_textureWidth, m_textureHeight, m_numMipmapLevels);
        
        
        textureObject = generateTextureObject(
                contextID,GL_TEXTURE_2D,m_numMipmapLevels,m_internalFormat,m_textureWidth,m_textureHeight,1,0);
        
		textureObject->bind(referenceTime);

        applyTexParameters(GL_TEXTURE_2D,state);

        if (textureObject->isAllocated())
        {
            //notify(NOTICE)<<"Reusing texture object"<<std::endl;
            applyTexImage2D_subload(state,GL_TEXTURE_2D,image.get(),
                                 m_textureWidth, m_textureHeight, m_internalFormat, m_numMipmapLevels);
        }
        else
        {
            //notify(NOTICE)<<"Creating new texture object"<<std::endl;
            applyTexImage2D_load(state,GL_TEXTURE_2D,image.get(),
                                 m_textureWidth, m_textureHeight, m_numMipmapLevels);
                                 
            textureObject->setAllocated(true);
        }
        
        
        // update the modified tag to show that it is upto date.
        getModifiedCount(contextID) = image->getModifiedCount();

        m_textureObjectBuffer[contextID] = textureObject;

        if (m_unrefImageDataAfterApply && areAllTextureObjectsLoaded() && image->getDataVariance()==STATIC)
        {
            crTexture2D* non_const_this = const_cast<crTexture2D*>(this);
            non_const_this->m_image = 0;
        }

        // in theory the following line is redundent, but in practice
        // have found that the first frame drawn doesn't apply the textures
        // unless a second bind is called?!!
        // perhaps it is the first glBind which is not required...
        //glBindTexture( GL_TEXTURE_2D, handle );
        
    }
    else if ( (m_textureWidth!=0) && (m_textureHeight!=0) && (m_internalFormat!=0) )
    {
		computeInternalFormat();
        m_textureObjectBuffer[contextID] = textureObject = generateTextureObject(
                contextID,GL_TEXTURE_2D,m_numMipmapLevels,m_internalFormat,m_textureWidth,m_textureHeight,1,0);
        
		textureObject->bind(referenceTime);

        applyTexParameters(GL_TEXTURE_2D,state);

        // no image present, but dimensions at set so lets create the texture
        glTexImage2D( GL_TEXTURE_2D, 0, m_internalFormat,
                     m_textureWidth, m_textureHeight, m_borderWidth,
                     m_sourceFormat ? m_sourceFormat : m_internalFormat,
                     m_sourceType ? m_sourceType : GL_UNSIGNED_BYTE,
                     0);                
                     
        if (m_readPBuffer.valid())
        {
            m_readPBuffer->bindPBufferToTexture(GL_FRONT);
        }

		//computeInternalFormat();
		//computeRequiredTextureDimensions(state, m_textureWidth, m_textureHeight, m_numMipmapLevels );

		//m_textureObjectBuffer[contextID] = textureObject = generateTextureObject(
		//	contextID,GL_TEXTURE_2D,m_numMipmapLevels,m_internalFormat,m_textureWidth,m_textureHeight,1,0);

		//textureObject->bind(crFrameStamp::getInstance()->getReferenceTime());

		//applyTexParameters(GL_TEXTURE_2D,state);

		//glTexImage2D( GL_TEXTURE_2D, 0, m_internalFormat,
		//	m_textureWidth, m_textureHeight, m_borderWidth,
		//	m_sourceFormat ? m_sourceFormat : m_internalFormat,
		//	m_sourceType ? m_sourceType : GL_UNSIGNED_BYTE,
		//	0);                
		//if(m_numMipmapLevels>1)
		//{
		//	int w = m_textureWidth;
		//	int h = m_textureHeight;
		//	int mipMapLevel = 1;
		//	while (w > 1 || h > 1)
		//	{
		//		if (w > 1) w >>= 1;
		//		if (h > 1) h >>= 1;

		//		glTexImage2D( GL_TEXTURE_2D, mipMapLevel, m_internalFormat, 
		//			w, h, 0, m_sourceFormat ? m_sourceFormat : m_internalFormat,
		//			m_sourceType ? m_sourceType : GL_UNSIGNED_BYTE,
		//			0 );
		//		mipMapLevel++;
		//	}
		//}

		//if (m_readPBuffer.valid())
		//{
		//	m_readPBuffer->bindPBufferToTexture(GL_FRONT);
		//}
    }
    else
    {
        glBindTexture( GL_TEXTURE_2D, 0 );
    }

    // if texture object is now valid and we have to allocate mipmap levels, then
    if (textureObject != 0 && m_texMipmapGenerationDirtyList[contextID])
    {
        generateMipmap(state);
    }

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crTexture2D OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}

void crTexture2D::computeInternalFormat() const
{
    if (m_image.valid()) computeInternalFormatWithImage(*m_image); 
    else computeInternalFormatType();
}

void crTexture2D::copyTexImage2D(crState& state, int x, int y, int width, int height )
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
    m_textureObjectBuffer[contextID] = textureObject = generateTextureObject(contextID,GL_TEXTURE_2D);

    textureObject->bind(crFrameStamp::getInstance()->getReferenceTime());
    
    applyTexParameters(GL_TEXTURE_2D,state);


    bool needHardwareMipMap = (m_min_filter != LINEAR && m_min_filter != NEAREST);
    bool hardwareMipMapOn = false;
    if (needHardwareMipMap)
    {
        hardwareMipMapOn = isHardwareMipmapGenerationEnabled(state);
        
        if (!hardwareMipMapOn)
        {
            // have to switch off mip mapping
            notify(NOTICE)<<"Warning: crTexture2D::copyTexImage2D(,,,,) switch off mip mapping as hardware support not available."<<std::endl;
            m_min_filter = LINEAR;
        }
    }
    
    GenerateMipmapMode mipmapResult = mipmapBeforeTexImage(state, hardwareMipMapOn);

    glCopyTexImage2D( GL_TEXTURE_2D, 0, m_internalFormat, x, y, width, height, 0 );

    mipmapAfterTexImage(state, mipmapResult);

    m_textureWidth = width;
    m_textureHeight = height;
    m_numMipmapLevels = 1;
    
    textureObject->setAllocated(m_numMipmapLevels,m_internalFormat,m_textureWidth,m_textureHeight,1,0);


    // inform state that this texture is the current one bound.
    state.haveAppliedTextureAttribute(state.getActiveTextureUnit(), this);
}

void crTexture2D::copyTexSubImage2D(crState& state, int xoffset, int yoffset, int x, int y, int width, int height )
{
    const unsigned int contextID = state.getContextID();

    if (m_internalFormat==0) m_internalFormat=GL_RGBA;

    // get the texture object for the current contextID.
    TextureObject* textureObject = getTextureObject(contextID);
    
    if (textureObject)
    {
        // we have a valid image
        textureObject->bind(crFrameStamp::getInstance()->getReferenceTime());
        
        applyTexParameters(GL_TEXTURE_2D,state);

        bool needHardwareMipMap = (m_min_filter != LINEAR && m_min_filter != NEAREST);
        bool hardwareMipMapOn = false;
        if (needHardwareMipMap)
        {
            hardwareMipMapOn = isHardwareMipmapGenerationEnabled(state);

            if (!hardwareMipMapOn)
            {
                // have to switch off mip mapping
                notify(NOTICE)<<"Warning: crTexture2D::copyTexImage2D(,,,,) switch off mip mapping as hardware support not available."<<std::endl;
                m_min_filter = LINEAR;
            }
        }

        GenerateMipmapMode mipmapResult = mipmapBeforeTexImage(state, hardwareMipMapOn);

        glCopyTexSubImage2D( GL_TEXTURE_2D, 0, xoffset, yoffset, x, y, width, height);

        mipmapAfterTexImage(state, mipmapResult);

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

void crTexture2D::allocateMipmap(crState& state) const
{
    const unsigned int contextID = state.getContextID();

    // get the texture object for the current contextID.
    TextureObject* textureObject = getTextureObject(contextID);
    
    if (textureObject && m_textureWidth != 0 && m_textureHeight != 0)
    {
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

            glTexImage2D( GL_TEXTURE_2D, k, m_internalFormat,
                     width, height, m_borderWidth,
                     m_sourceFormat ? m_sourceFormat : m_internalFormat,
                     m_sourceType ? m_sourceType : GL_UNSIGNED_BYTE, NULL);

            width >>= 1;
            height >>= 1;
        }
                
        // inform state that this texture is the current one bound.
        state.haveAppliedTextureAttribute(state.getActiveTextureUnit(), this);        
    }
}

