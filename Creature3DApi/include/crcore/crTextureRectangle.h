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
#ifndef CRCORE_CRTEXTURERECTANGLE_H
#define CRCORE_CRTEXTURERECTANGLE_H 1

#include <CRCore/crTexture.h>

#ifndef GL_TEXTURE_RECTANGLE_NV
#define GL_TEXTURE_RECTANGLE_NV           0x84F5
#endif

#ifndef GL_TEXTURE_RECTANGLE
#define GL_TEXTURE_RECTANGLE GL_TEXTURE_RECTANGLE_NV
#endif

namespace CRCore {

/** crTexture state class which encapsulates OpenGL texture functionality. */
class CR_EXPORT crTextureRectangle : public crTexture
{

    public :

        crTextureRectangle();

        crTextureRectangle(crImage* image);

        /** Copy constructor using crCopyOp to manage deep vs shallow copy. */
        crTextureRectangle(const crTextureRectangle& text,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_StateAttribute(CRCore, crTextureRectangle, TEXTURE);

        /** Return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs. */
        virtual int compare(const crStateAttribute& rhs) const;
		virtual int compare(const crStateAttribute& rhs);

        virtual GLenum getTextureTarget() const { return GL_TEXTURE_RECTANGLE; }

        /** Set the texture image. */
        void setImage(crImage* image);

        /** Get the texture image. */
        crImage* getImage() { return m_image.get(); }

        /** Get the const texture image. */
        inline const crImage* getImage() const { return m_image.get(); }

        inline unsigned int& getModifiedCount(unsigned int contextID) const
        {
            // get the modified count for the current contextID.
            return m_modifiedCount[contextID];
        }


        /** Set the texture image, ignoring face value as there is only one image. */
        virtual void setImage(unsigned int, crImage* image) { setImage(image); }

        /** Get the texture image, ignoring face value as there is only one image. */
        virtual crImage* getImage(unsigned int) { return m_image.get(); }

        /** Get the const texture image, ignoring face value as there is only one image. */
        virtual const crImage* getImage(unsigned int) const { return m_image.get(); }

        /** Get the number of images that can be assigned to the crTexture. */
        virtual unsigned int getNumImages() const { return 1; }


        /** Set the texture width and height. If width or height are zero then
          * the respective size value is calculated from the source image sizes.
        */
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

        class SubloadCallback : public Referenced
        {
        public:
            virtual void load(const crTextureRectangle&, crState&) const = 0;
            virtual void subload(const crTextureRectangle&, crState&) const = 0;
        };

        void setSubloadCallback(SubloadCallback* cb) { m_subloadCallback = cb;; }
        SubloadCallback* getSubloadCallback() { return m_subloadCallback.get(); }
        const SubloadCallback* getSubloadCallback() const { return m_subloadCallback.get(); }

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

        /** On first apply (unless already compiled), create and bind the 
          * texture, subsequent apply will simply bind to texture.
        */
        virtual void apply(crState& state) const;

    protected :

        virtual ~crTextureRectangle();

        virtual void computeInternalFormat() const;
        void allocateMipmap(crState& state) const;

        void applyTexParameters(GLenum target, crState& state) const;

        void applyTexImage_load(GLenum target, crImage* image, crState& state, GLsizei& inwidth, GLsizei& inheight) const;
        void applyTexImage_subload(GLenum target, crImage* image, crState& state, GLsizei& inwidth, GLsizei& inheight, GLint& inInternalFormat) const;

        mutable ref_ptr<crImage> m_image;

        // subloaded images can have different texture and image sizes.
        mutable GLsizei m_textureWidth, m_textureHeight;
        
        ref_ptr<SubloadCallback> m_subloadCallback;

        typedef buffered_value<unsigned int> ImageModifiedCount;
        mutable ImageModifiedCount m_modifiedCount;
};

}

#endif
