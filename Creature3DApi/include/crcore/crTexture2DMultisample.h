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

#ifndef CRCORE_TEXTURE2DMS_H
#define CRCORE_TEXTURE2DMS_H 1

#include <CRCore/crTexture.h>

namespace CRCore {

 /** crTexture2DMultisample state class which encapsulates OpenGL 2D multisampled texture functionality. 
 * Multisampled texture were introduced with OpenGL 3.1 and extension GL_ARB_texture_multisample.
 * See http://www.opengl.org/registry/specs/ARB/texture_multisample.txt for more info.
 */

class CR_EXPORT crTexture2DMultisample : public crTexture
{
    public :
        
        crTexture2DMultisample();

        crTexture2DMultisample(GLsizei numSamples, GLboolean fixedsamplelocations);

        /** Copy constructor using crCopyOp to manage deep vs shallow copy. */
        crTexture2DMultisample(const crTexture2DMultisample& text,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);
        
        META_StateAttribute(CRCore, crTexture2DMultisample,TEXTURE);

        /** Return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs. */
        virtual int compare(const crStateAttribute& rhs) const;
        virtual int compare(const crStateAttribute& rhs);

        virtual GLenum getTextureTarget() const 
        { 
          return GL_TEXTURE_2D_MULTISAMPLE; 
        }

        /** Sets the texture width and height. If width or height are zero,
          * calculate the respective value from the source image size. */
        inline void setTextureSize(int width, int height) const
        {
            m_textureWidth = width;
            m_textureHeight = height;
        }

        inline void setNumSamples( int samples ) { m_numSamples = samples; }

        // unnecessary for crTexture2DMultisample
        virtual void setImage(unsigned int /*face*/, crImage* /*image*/) {}
        virtual crImage* getImage(unsigned int /*face*/) { return NULL; }
        virtual const crImage* getImage(unsigned int /*face*/) const { return NULL; }
        virtual unsigned int getNumImages() const {return 0; }
        virtual void allocateMipmap(crState& /*state*/) const {}

        void setTextureWidth(int width) { m_textureWidth=width; }
        void setTextureHeight(int height) { m_textureHeight=height; }

        virtual int getTextureWidth() const { return m_textureWidth; }
        virtual int getTextureHeight() const { return m_textureHeight; }
        virtual int getTextureDepth() const { return 1; }

        /** Bind the texture object. If the texture object hasn't already been
          * compiled, create the texture mipmap levels. */
        virtual void apply(crState& state) const;

    protected :

        virtual ~crTexture2DMultisample();

        virtual void computeInternalFormat() const;

        /** Subloaded images can have different texture and image sizes. */
        mutable GLsizei m_textureWidth, m_textureHeight;

        mutable GLsizei m_numSamples;

        mutable GLboolean m_fixedsamplelocations;

};

}

#endif
