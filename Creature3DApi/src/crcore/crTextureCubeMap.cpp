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
//Modified by 吴财华
#include <crgl/GLExtensions.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/crImage.h>
#include <CRCore/crState.h>
#include <CRCore/crTextureCubeMap.h>
#include <CRCore/crImageSequence.h>
#include <CRCore/crNotify.h>
#include <CRCore/crArgumentParser.h>

#include <crgl/GLU.h>

using namespace CRCore;

static GLenum faceTarget[6] =
{
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};


crTextureCubeMap::crTextureCubeMap():
            m_textureWidth(0),
            m_textureHeight(0),
            m_numMipmapLevels(0)
{
    setUseHardwareMipMapGeneration(false);
}

crTextureCubeMap::crTextureCubeMap(const crTextureCubeMap& text,const crCopyOp& copyop):
            crTexture(text,copyop),
            m_textureWidth(text.m_textureWidth),
            m_textureHeight(text.m_textureHeight),
            m_numMipmapLevels(text.m_numMipmapLevels),
            m_subloadCallback(text.m_subloadCallback)
{
    m_images[0] = copyop(text.m_images[0].get());
    m_images[1] = copyop(text.m_images[1].get());
    m_images[2] = copyop(text.m_images[2].get());
    m_images[3] = copyop(text.m_images[3].get());
    m_images[4] = copyop(text.m_images[4].get());
    m_images[5] = copyop(text.m_images[5].get());

    m_modifiedCount[0].setAllElementsTo(0);
    m_modifiedCount[1].setAllElementsTo(0);
    m_modifiedCount[2].setAllElementsTo(0);
    m_modifiedCount[3].setAllElementsTo(0);
    m_modifiedCount[4].setAllElementsTo(0);
    m_modifiedCount[5].setAllElementsTo(0);

}    


crTextureCubeMap::~crTextureCubeMap()
{
}


int crTextureCubeMap::compare(const crStateAttribute& sa) const
{
    // check the types are equal and then create the rhs variable
    // used by the COMPARE_StateAttribute_Paramter macro's below.
    COMPARE_StateAttribute_Types(crTextureCubeMap,sa)

	int result = m_imageNameID.compare(rhs.m_imageNameID);
	if (result!=0) return result;
    
	//bool noImages = true;
 //   for (int n=0; n<6; n++)
 //   {
 //       if (noImages && m_images[n].valid()) noImages = false;
 //       if (noImages && rhs.m_images[n].valid()) noImages = false;
 //   
 //       if (m_images[n]!=rhs.m_images[n]) // smart pointer comparison.
 //       {
 //           if (m_images[n].valid())
 //           {
 //               if (rhs.m_images[n].valid())
 //               {
 //                   int result = m_images[n]->compare(*rhs.m_images[n]);
 //                   if (result!=0) return result;
 //               }
 //               else
 //               {
 //                   return 1; // valid lhs._image is greater than null. 
 //               }
 //           }
 //           else if (rhs.m_images[n].valid()) 
 //           {
 //               return -1; // valid rhs._image is greater than null. 
 //           }
 //       }
 //   }

    if (!imagesValid())
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

    // compare each paramter in turn against the rhs.
    COMPARE_StateAttribute_Parameter(m_textureWidth)
    COMPARE_StateAttribute_Parameter(m_textureHeight)
    COMPARE_StateAttribute_Parameter(m_subloadCallback)

    return 0; // passed all the above comparison macro's, must be equal.
}

int crTextureCubeMap::compare(const crStateAttribute& sa)
{
	// check the types are equal and then create the rhs variable
	// used by the COMPARE_StateAttribute_Paramter macro's below.
	COMPARE_StateAttribute_Types(crTextureCubeMap,sa)
	int result = m_imageNameID.compare(rhs.m_imageNameID);
	if (result!=0) return result;

	//bool noImages = true;
	//for (int n=0; n<6; n++)
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
		// no image attached to either Texture2D
		// but could these textures already be downloaded?
		// check the m_textureObjectBuffer to see if they have been
		// downloaded

		result = compareTextureObjects(rhs);
		if (result!=0) return result;
	}

	result = compareTexture(rhs);
	if (result!=0) return result;

	// compare each paramter in turn against the rhs.
	COMPARE_StateAttribute_Parameter(m_textureWidth)
	COMPARE_StateAttribute_Parameter(m_textureHeight)
	COMPARE_StateAttribute_Parameter(m_subloadCallback)

	return 0; // passed all the above comparison macro's, must be equal.
}

std::string crTextureCubeMap::getCupMapNameID()
{
	std::string fileName;
	for(int face = 0; face < 6; face++)
	{
		if(m_images[face].valid())
		{
			fileName = m_images[face]->getFileName();
			break;
		}
	}

	std::string ext = crArgumentParser::getFileExtension(fileName);

	std::string::size_type underLine = fileName.find_last_of('_');
	if (underLine==std::string::npos) return std::string("");
	std::string nameid = std::string(fileName.begin(),fileName.begin()+underLine) + "." + ext;
	return nameid;
}

void crTextureCubeMap::setImage( unsigned int face, crImage* image)
{
    if (m_images[face] == image) return;

    unsigned numImageSequencesBefore = 0;
    for (unsigned int i=0; i<getNumImages(); ++i)
    {
        CRCore::crImageSequence* is = dynamic_cast<CRCore::crImageSequence*>(m_images[i].get());
        if (is) ++numImageSequencesBefore;
    }

    m_images[face] = image;
    m_modifiedCount[face].setAllElementsTo(0);


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

	//if(face == POSITIVE_X)
	//{
	//	if(image) 
	//		m_imageNameID = getCupMapNameID();
	//}
}

crImage* crTextureCubeMap::getImage(unsigned int face)
{
    return m_images[face].get();
}

const crImage* crTextureCubeMap::getImage(unsigned int face) const
{
    return m_images[face].get();
}

bool crTextureCubeMap::imagesValid() const
{
    for (int n=0; n<6; n++)
    {
        if (!m_images[n].valid() || !m_images[n]->data())
            return false;
    }
    return true;
}

void crTextureCubeMap::computeInternalFormat() const
{
    if (imagesValid()) computeInternalFormatWithImage(*m_images[0]); 
    else computeInternalFormatType();
}

void crTextureCubeMap::apply(crState& state) const
{
    // get the contextID (user defined ID of 0 upwards) for the 
    // current OpenGL context.
    const unsigned int contextID = state.getContextID();
    const Extensions* extensions = getExtensions(contextID,true);

    if (!extensions->isCubeMapSupported())
        return;

    // get the texture object for the current contextID.
    TextureObject* textureObject = getTextureObject(contextID);
	double referenceTime = crFrameStamp::getInstance()->getReferenceTime();

    if (textureObject != 0)
    {
        textureObject->bind(referenceTime);

        if (getTextureParameterDirty(state.getContextID())) applyTexParameters(GL_TEXTURE_CUBE_MAP,state);

        if (m_subloadCallback.valid())
        {
            m_subloadCallback->subload(*this,state);
        }
        else
        {
			ref_ptr<crImage> image;
            for (int n=0; n<6; n++)
            {
                image = m_images[n].get();
                if (image.valid() && image->data() && getModifiedCount((Face)n,contextID) != image->getModifiedCount())
                {
                    applyTexImage2D_subload( state, faceTarget[n], image.get(), m_textureWidth, m_textureHeight, m_internalFormat, m_numMipmapLevels);
                    getModifiedCount((Face)n,contextID) = image->getModifiedCount();
                }
            }
        }

    }
    else if (m_subloadCallback.valid())
    {
        m_textureObjectBuffer[contextID] = textureObject = generateTextureObject(contextID,GL_TEXTURE_CUBE_MAP);

        textureObject->bind(referenceTime);

        applyTexParameters(GL_TEXTURE_CUBE_MAP,state);

        m_subloadCallback->load(*this,state);

        // in theory the following line is redundent, but in practice
        // have found that the first frame drawn doesn't apply the textures
        // unless a second bind is called?!!
        // perhaps it is the first glBind which is not required...
        //glBindTexture( GL_TEXTURE_CUBE_MAP, handle );

    }
    else if (imagesValid())
    {
		ref_ptr<crImage> image = m_images[0];
        // compute the internal texture format, this set the m_internalFormat to an appropriate value.
        computeInternalFormat();

        // compute the dimensions of the texture.
        computeRequiredTextureDimensions(state,*image,m_textureWidth, m_textureHeight, m_numMipmapLevels);

        // cubemap textures must have square dimensions
        if( m_textureWidth != m_textureHeight )
        {
            m_textureWidth = m_textureHeight = minimum( m_textureWidth , m_textureHeight );
        }

        textureObject = generateTextureObject(
                contextID,GL_TEXTURE_CUBE_MAP,m_numMipmapLevels,m_internalFormat,m_textureWidth,m_textureHeight,1,0);
        
        textureObject->bind(referenceTime);

        applyTexParameters(GL_TEXTURE_CUBE_MAP,state);

        for (int n=0; n<6; n++)
        {
            image = m_images[n].get();
            if (image.valid() && image->data())
            {
                if (textureObject->isAllocated())
                {
                    applyTexImage2D_subload( state, faceTarget[n], image.get(), m_textureWidth, m_textureHeight, m_internalFormat, m_numMipmapLevels);
                }
                else
                {
                    applyTexImage2D_load( state, faceTarget[n], image.get(), m_textureWidth, m_textureHeight, m_numMipmapLevels);
                }
                getModifiedCount((Face)n,contextID) = image->getModifiedCount();
            }


        }

        m_textureObjectBuffer[contextID] = textureObject;

        if (m_unrefImageDataAfterApply && areAllTextureObjectsLoaded())
        {
            crTextureCubeMap* non_const_this = const_cast<crTextureCubeMap*>(this);
            for (int n=0; n<6; n++)
            {
				image = m_images[n].get();
                if (image.valid() && image->getDataVariance()==STATIC)
                {
                    non_const_this->m_images[n] = 0;
                }
            }
        }
        
    }
    else if ( (m_textureWidth!=0) && (m_textureHeight!=0) && (m_internalFormat!=0) )
    {
        m_textureObjectBuffer[contextID] = textureObject = generateTextureObject(
                contextID,GL_TEXTURE_CUBE_MAP,m_numMipmapLevels,m_internalFormat,m_textureWidth,m_textureHeight,1,0);
        
        textureObject->bind(referenceTime);

        applyTexParameters(GL_TEXTURE_CUBE_MAP,state);

        for (int n=0; n<6; n++)
        {                
            // no image present, but dimensions at set so less create the texture
            glTexImage2D( faceTarget[n], 0, m_internalFormat,
                         m_textureWidth, m_textureHeight, m_borderWidth,
                         m_sourceFormat ? m_sourceFormat : m_internalFormat,
                         m_sourceType ? m_sourceType : GL_UNSIGNED_BYTE,
                         0);                
        }
        
    }
    else
    {
        glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
    }

    // if texture object is now valid and we have to allocate mipmap levels, then
    if (textureObject != 0 && m_texMipmapGenerationDirtyList[contextID])
    {
        generateMipmap(state);
    }
}

void crTextureCubeMap::copyTexSubImageCubeMap(crState& state, int face, int xoffset, int yoffset, int x, int y, int width, int height )
{
    const unsigned int contextID = state.getContextID();
    const Extensions* extensions = getExtensions(contextID,true);

    if (!extensions->isCubeMapSupported())
        return;

    if (m_internalFormat==0) m_internalFormat=GL_RGBA;

    // get the texture object for the current contextID.
    TextureObject* textureObject = getTextureObject(contextID);

    if (!textureObject)
    {

        if (m_textureWidth==0) m_textureWidth = width;
        if (m_textureHeight==0) m_textureHeight = height;

        // create texture object.
        apply(state);
        
        textureObject = getTextureObject(contextID);
        
        if (!textureObject)
        {
            // failed to create texture object
            CRCore::notify(CRCore::NOTICE)<<"Warning : failed to create crTextureCubeMap texture obeject, copyTexSubImageCubeMap abondoned."<<std::endl;
            return;
        }
        
    }

    GLenum target = faceTarget[face];
    
    if (textureObject)
    {
        // we have a valid image
        textureObject->bind(crFrameStamp::getInstance()->getReferenceTime());
        
        applyTexParameters(GL_TEXTURE_CUBE_MAP, state);

        bool needHardwareMipMap = (m_min_filter != LINEAR && m_min_filter != NEAREST);
        bool hardwareMipMapOn = false;
        if (needHardwareMipMap)
        {
            hardwareMipMapOn = isHardwareMipmapGenerationEnabled(state);

            if (!hardwareMipMapOn)
            {
                // have to switch off mip mapping
                notify(NOTICE)<<"Warning: crTextureCubeMap::copyTexImage2D(,,,,) switch off mip mapping as hardware support not available."<<std::endl;
                m_min_filter = LINEAR;
            }
        }

        GenerateMipmapMode mipmapResult = mipmapBeforeTexImage(state, hardwareMipMapOn);

        glCopyTexSubImage2D( target , 0, xoffset, yoffset, x, y, width, height);

        mipmapAfterTexImage(state, mipmapResult);

        // inform state that this texture is the current one bound.
        state.haveAppliedTextureAttribute(state.getActiveTextureUnit(), this);

    }
}

void crTextureCubeMap::allocateMipmap(crState& state) const
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

            for (int n=0; n<6; n++)
            {
                glTexImage2D( faceTarget[n], k, m_internalFormat,
                            width, height, m_borderWidth,
                            m_sourceFormat ? m_sourceFormat : m_internalFormat,
                            m_sourceType ? m_sourceType : GL_UNSIGNED_BYTE,
                            0);
            }
        
            width >>= 1;
            height >>= 1;
        }
                
        // inform state that this texture is the current one bound.
        state.haveAppliedTextureAttribute(state.getActiveTextureUnit(), this);        
    }
}

typedef buffered_value< ref_ptr<crTextureCubeMap::Extensions> > BufferedExtensions;
static BufferedExtensions s_extensions;
void crTextureCubeMap::Extensions::clearExtensions()
{
	s_extensions.clear();
}
crTextureCubeMap::Extensions* crTextureCubeMap::getExtensions(unsigned int contextID,bool createIfNotInitalized)
{
    if (!s_extensions[contextID] && createIfNotInitalized) s_extensions[contextID] = new Extensions(contextID);
    return s_extensions[contextID].get();
}

void crTextureCubeMap::setExtensions(unsigned int contextID,Extensions* extensions)
{
    s_extensions[contextID] = extensions;
}

crTextureCubeMap::Extensions::Extensions(unsigned int contextID)
{
    setupGLExtensions(contextID);
}

crTextureCubeMap::Extensions::Extensions(const Extensions& rhs):
    Referenced()
{
    m_isCubeMapSupported = rhs.m_isCubeMapSupported;
}

void crTextureCubeMap::Extensions::lowestCommonDenominator(const Extensions& rhs)
{
    if (!rhs.m_isCubeMapSupported) m_isCubeMapSupported = false;
}

void crTextureCubeMap::Extensions::setupGLExtensions(unsigned int contextID)
{
    m_isCubeMapSupported = isGLExtensionSupported(contextID,"GL_ARB_texture_cube_map") ||
                          isGLExtensionSupported(contextID,"GL_EXT_texture_cube_map") ||
                          strncmp((const char*)glGetString(GL_VERSION),"1.3",3)>=0;;
}
