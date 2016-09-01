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
#include <CRCore/crTexture2DMultisample.h>
#include <CRCore/crState.h>
#include <CRCore/crNotify.h>

using namespace CRCore;

crTexture2DMultisample::crTexture2DMultisample():
            m_textureWidth(0),
            m_textureHeight(0),
            m_numSamples(1),
            m_fixedsamplelocations(GL_FALSE)
{
}

crTexture2DMultisample::crTexture2DMultisample(GLsizei numSamples, GLboolean fixedsamplelocations):
            m_textureWidth(0),
            m_textureHeight(0),
            m_numSamples(numSamples),
            m_fixedsamplelocations(fixedsamplelocations)
{
}

crTexture2DMultisample::crTexture2DMultisample(const crTexture2DMultisample& text,const crCopyOp& copyop):
            crTexture(text,copyop),
            m_textureWidth(text.m_textureWidth),
            m_textureHeight(text.m_textureHeight),
            m_numSamples(text.m_numSamples),
            m_fixedsamplelocations(text.m_fixedsamplelocations)
{
}

crTexture2DMultisample::~crTexture2DMultisample()
{
}

int crTexture2DMultisample::compare(const crStateAttribute& sa) const
{
    // check the types are equal and then create the rhs variable
    // used by the COMPARE_StateAttribute_Parameter macros below.
    COMPARE_StateAttribute_Types(crTexture2DMultisample,sa)
	int result = m_imageNameID.compare(rhs.m_imageNameID);
	if (result!=0) return result;

    result = compareTexture(rhs);
    if (result!=0) return result;

    // compare each parameter in turn against the rhs.
    if (m_textureWidth != 0 && rhs.m_textureWidth != 0)
    {
        COMPARE_StateAttribute_Parameter(m_textureWidth)
    }
    if (m_textureHeight != 0 && rhs.m_textureHeight != 0)
    {
        COMPARE_StateAttribute_Parameter(m_textureHeight)
    }
    if (m_numSamples != 0 && rhs.m_numSamples != 0)
    {
        COMPARE_StateAttribute_Parameter(m_numSamples)
    }
    if (m_fixedsamplelocations != 0 && rhs.m_fixedsamplelocations != 0)
    {
        COMPARE_StateAttribute_Parameter(m_fixedsamplelocations)
    }


    return 0; // passed all the above comparison macros, must be equal.
}

int crTexture2DMultisample::compare(const crStateAttribute& sa)
{
	// check the types are equal and then create the rhs variable
	// used by the COMPARE_StateAttribute_Parameter macros below.
	COMPARE_StateAttribute_Types(crTexture2DMultisample,sa)
	int result = m_imageNameID.compare(rhs.m_imageNameID);
	if (result!=0) return result;

	result = compareTexture(rhs);
	if (result!=0) return result;

	// compare each parameter in turn against the rhs.
	if (m_textureWidth != 0 && rhs.m_textureWidth != 0)
	{
		COMPARE_StateAttribute_Parameter(m_textureWidth)
	}
	if (m_textureHeight != 0 && rhs.m_textureHeight != 0)
	{
		COMPARE_StateAttribute_Parameter(m_textureHeight)
	}
	if (m_numSamples != 0 && rhs.m_numSamples != 0)
	{
		COMPARE_StateAttribute_Parameter(m_numSamples)
	}
	if (m_fixedsamplelocations != 0 && rhs.m_fixedsamplelocations != 0)
	{
		COMPARE_StateAttribute_Parameter(m_fixedsamplelocations)
	}


	return 0; // passed all the above comparison macros, must be equal.
}

void crTexture2DMultisample::apply(crState& state) const
{
    // current OpenGL context.
    const unsigned int contextID = state.getContextID();
    const Extensions* extensions = getExtensions(contextID,true);
    if (!extensions->isTextureMultisampledSupported())
    {
        //notify(INFO)<<"crTexture2DMultisample not supoorted."<<std::endl;
        return;
    }

    //crTexture::crTextureObjectManager* tom = crTexture::getTextureObjectManager(contextID).get();
    //ElapsedTime elapsedTime(&(tom->getApplyTime()));
    //tom->getNumberApplied()++;

    // get the texture object for the current contextID.
    TextureObject* textureObject = getTextureObject(contextID);
	double referenceTime = crFrameStamp::getInstance()->getReferenceTime();
    if (textureObject)
    {
        textureObject->bind(referenceTime);
    }
    else if ( (m_textureWidth!=0) && (m_textureHeight!=0) && (m_numSamples!=0) )
    {
        m_textureObjectBuffer[contextID] = textureObject = 
          generateTextureObject( contextID,
                                 getTextureTarget(),
                                 1,
                                 m_internalFormat,
                                 m_textureWidth,
                                 m_textureHeight,
                                 1,
                                 m_borderWidth);

        textureObject->bind(referenceTime);

        extensions->glTexImage2DMultisample( getTextureTarget(), 
                                             m_numSamples, 
                                             m_internalFormat, 
                                             m_textureWidth, 
                                             m_textureHeight, 
                                             m_fixedsamplelocations );

    }
    else
    {
        glBindTexture( getTextureTarget(), 0 );
    }
}

void crTexture2DMultisample::computeInternalFormat() const
{
    computeInternalFormatType();
}

