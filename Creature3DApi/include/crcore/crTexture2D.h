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
#ifndef CRCORE_CRTEXTURE2D_H
#define CRCORE_CRTEXTURE2D_H 1

#include <CRCore/crTexture.h>

namespace CRCore {

/** Encapsulates OpenGl 2D texture functionality. Doesn't support cube maps,
  * so ignore \a face parameters.
*/
class CR_EXPORT crTexture2D : public crTexture
{

    public :
        
        crTexture2D();

        crTexture2D(crImage* image);

        /** Copy constructor using crCopyOp to manage deep vs shallow copy. */
        crTexture2D(const crTexture2D& text,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);
        
        META_StateAttribute(CRCore, crTexture2D,TEXTURE);

        /** Return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs. */
        virtual int compare(const crStateAttribute& rhs) const;
		virtual int compare(const crStateAttribute& rhs);

        virtual GLenum getTextureTarget() const { return GL_TEXTURE_2D; }

        /** Sets the texture image. */
        void setImage(crImage* image);

        /** Gets the texture image. */
        crImage* getImage() { return m_image.get(); }

        /** Gets the const texture image. */
        inline const crImage* getImage() const { return m_image.get(); }

        inline unsigned int& getModifiedCount(unsigned int contextID) const
        {
            // get the modified count for the current contextID.
            return m_modifiedCount[contextID];
        }


        /** Sets the texture image, ignoring face. */
        virtual void setImage(unsigned int, crImage* image) { setImage(image);  }

        /** Gets the texture image, ignoring face. */
        virtual crImage* getImage(unsigned int) { return m_image.get(); }

        /** Gets the const texture image, ignoring face. */
        virtual const crImage* getImage(unsigned int) const { return m_image.get(); }

        /** Gets the number of images that can be assigned to the crTexture. */
        virtual unsigned int getNumImages() const { return 1; }


        /** Sets the texture width and height. If width or height are zero,
          * calculate the respective value from the source image size. */
        inline void setTextureSize(int width, int height) const
        {
            m_textureWidth = width;
            m_textureHeight = height;
        }

        void setTextureWidth(int width) { m_textureWidth=width; }
        void setTextureHeight(int height) { m_textureHeight=height; }

        virtual int getTextureWidth() const { return m_textureWidth; }
        virtual int getTextureHeight() const { return m_textureHeight; }
        virtual int getTextureDepth() const { return 1; }

        class CR_EXPORT SubloadCallback : public Referenced
        {
            public:
                virtual void load(const crTexture2D& texture,crState& state) const = 0;
                virtual void subload(const crTexture2D& texture,crState& state) const = 0;
        };
        
        void setSubloadCallback(SubloadCallback* cb) { m_subloadCallback = cb;; }
        
        SubloadCallback* getSubloadCallback() { return m_subloadCallback.get(); }

        const SubloadCallback* getSubloadCallback() const { return m_subloadCallback.get(); }


        /** Helper function. Sets the number of mipmap levels created for this
          * texture. Should only be called within an CRCore::crTexture::apply(), or
          * during a custom OpenGL texture load. */
        void setNumMipmapLevels(unsigned int num) const { m_numMipmapLevels=num; }

        /** Gets the number of mipmap levels created. */
        unsigned int getNumMipmapLevels() const { return m_numMipmapLevels; }
        

        /** Copies pixels into a 2D texture image, as per glCopyTexImage2D.
          * Creates an OpenGL texture object from the current OpenGL background
          * framebuffer contents at position \a x, \a y with width \a width and
          * height \a height. \a width and \a height must be a power of two. */
        void copyTexImage2D(crState& state, int x, int y, int width, int height );

        /** Copies a two-dimensional texture subimage, as per
          * glCopyTexSubImage2D. Updates a portion of an existing OpenGL
          * texture object from the current OpenGL background framebuffer
          * contents at position \a x, \a y with width \a width and height
          * \a height. Loads framebuffer data into the texture using offsets
          * \a xoffset and \a yoffset. \a width and \a height must be powers
          * of two. */
        void copyTexSubImage2D(crState& state, int xoffset, int yoffset, int x, int y, int width, int height );


        /** Bind the texture object. If the texture object hasn't already been
          * compiled, create the texture mipmap levels. */
        virtual void apply(crState& state) const;

    protected :

        virtual ~crTexture2D();

        virtual void computeInternalFormat() const;
        void allocateMipmap(crState& state) const;


        ref_ptr<crImage> m_image;

        /** Subloaded images can have different texture and image sizes. */
        mutable GLsizei m_textureWidth, m_textureHeight;
        
        /** Number of mipmap levels created. */        
        mutable GLsizei m_numMipmapLevels;

        ref_ptr<SubloadCallback> m_subloadCallback;

        typedef buffered_value<unsigned int> ImageModifiedCount;
        mutable ImageModifiedCount m_modifiedCount;

};

}

#endif
