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
#ifndef CRCORE_CRTEXTURE1D_H
#define CRCORE_CRTEXTURE1D_H 1

#include <CRCore/crTexture.h>

namespace CRCore {

/** Encapsulates OpenGl 1D texture functionality. Doesn't support cube maps,
  * so ignore \a face parameters.
*/
class CR_EXPORT crTexture1D : public crTexture
{

    public :
        
        crTexture1D();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy. */
        crTexture1D(const crTexture1D& text,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);
        
        META_StateAttribute(CRCore, crTexture1D,TEXTURE);

        /** Return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs. */
        virtual int compare(const crStateAttribute& rhs) const;
		virtual int compare(const crStateAttribute& rhs);

        virtual GLenum getTextureTarget() const { return GL_TEXTURE_1D; }

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
        virtual void setImage(unsigned int, crImage* image) { setImage(image); }

        /** Gets the texture image, ignoring face. */
        virtual crImage* getImage(unsigned int) { return m_image.get(); }

        /** Gets the const texture image, ignoring face. */
        virtual const crImage* getImage(unsigned int) const { return m_image.get(); }

        /** Gets the number of images that can be assigned to the crTexture. */
        virtual unsigned int getNumImages() const { return 1; }


        /** Sets the texture width. If width is zero, calculate the value
          * from the source image width. */
        inline void setTextureWidth(int width) const { m_textureWidth = width; }

        /** Gets the texture width. */
        virtual int getTextureWidth() const { return m_textureWidth; }
        virtual int getTextureHeight() const { return 1; }
        virtual int getTextureDepth() const { return 1; }


        class CR_EXPORT SubloadCallback : public Referenced
        {
            public:
                virtual void load(const crTexture1D& texture,crState& state) const = 0;
                virtual void subload(const crTexture1D& texture,crState& state) const = 0;
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


        /** Copies pixels into a 1D texture image, as per glCopyTexImage1D.
          * Creates an OpenGL texture object from the current OpenGL background
          * framebuffer contents at position \a x, \a y with width \a width.
          * \a width must be a power of two. */
        void copyTexImage1D(crState& state, int x, int y, int width);

        /** Copies a one-dimensional texture subimage, as per
          * glCopyTexSubImage1D. Updates a portion of an existing OpenGL
          * texture object from the current OpenGL background framebuffer
          * contents at position \a x, \a y with width \a width. */
        void copyTexSubImage1D(crState& state, int xoffset, int x, int y, int width);


        /** Bind the texture object. If the texture object hasn't already been
          * compiled, create the texture mipmap levels. */
        virtual void apply(crState& state) const;

    protected :

        virtual ~crTexture1D();

        virtual void computeInternalFormat() const;
        void allocateMipmap(crState& state) const;

        /** Helper method. Create the texture without setting or using a
          * texture binding. */
        void applyTexImage1D(GLenum target, crImage* image, crState& state, GLsizei& width, GLsizei& numMipmapLevels) const;


        /** It's not ideal that m_image is mutable, but it's required since
          * crImage::ensureDimensionsArePowerOfTwo() can only be called in a
          * valid OpenGL context, and therefore within crTexture::apply, which
          * is const. */
        mutable ref_ptr<crImage> m_image;

        /** Subloaded images can have different texture and image sizes. */
        mutable GLsizei m_textureWidth;
        
        /** Number of mipmap levels created. */
        mutable GLsizei m_numMipmapLevels;

        ref_ptr<SubloadCallback> m_subloadCallback;

        typedef buffered_value<unsigned int> ImageModifiedCount;
        mutable ImageModifiedCount m_modifiedCount;


};

}

#endif
