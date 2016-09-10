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
#ifndef CRCORE_CRTEXTURE3D_H
#define CRCORE_CRTEXTURE3D_H 1

#include <CRCore/crTexture.h>

namespace CRCore {

/** Encapsulates OpenGl 2D texture functionality. Doesn't support cube maps,
  * so ignore \a face parameters.
*/
class CR_EXPORT crTexture3D : public crTexture
{

    public :
        
        crTexture3D();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy. */
        crTexture3D(const crTexture3D& text,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);
        
        META_StateAttribute(CRCore, crTexture3D,TEXTURE);

        /** Return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs. */
        virtual int compare(const crStateAttribute& rhs) const;
		virtual int compare(const crStateAttribute& rhs);

        virtual GLenum getTextureTarget() const { return GL_TEXTURE_3D; }

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


        /** Sets the texture width, height, and depth. If width, height, or
          * depth are zero, calculate the respective value from the source
          * image size. */
        inline void setTextureSize(int width, int height, int depth) const
        {
            m_textureWidth = width;
            m_textureHeight = height;
            m_textureDepth = depth;
        }

        /** Gets the texture subload width. */
        inline void getTextureSize(int& width, int& height, int& depth) const
        {
            width = m_textureWidth;
            height = m_textureHeight;
            depth = m_textureDepth;
        }

        void setTextureWidth(int width) { m_textureWidth=width; }
        void setTextureHeight(int height) { m_textureHeight=height; }
        void setTextureDepth(int depth) { m_textureDepth=depth; }

        virtual int getTextureWidth() const { return m_textureWidth; }
        virtual int getTextureHeight() const { return m_textureHeight; }
        virtual int getTextureDepth() const { return m_textureDepth; }


        class CR_EXPORT SubloadCallback : public Referenced
        {
            public:
                virtual void load(const crTexture3D& texture,crState& state) const = 0;
                virtual void subload(const crTexture3D& texture,crState& state) const = 0;
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
        

        /** Copies a two-dimensional texture subimage, as per
          * glCopyTexSubImage3D. Updates a portion of an existing OpenGL
          * texture object from the current OpenGL background framebuffer
          * contents at position \a x, \a y with width \a width and height
          * \a height. Loads framebuffer data into the texture using offsets
          * \a xoffset, \a yoffset, and \a zoffset. \a width and \a height
          * must be powers of two. */
        void copyTexSubImage3D(crState& state, int xoffset, int yoffset, int zoffset, int x, int y, int width, int height);


        /** Bind the texture object. If the texture object hasn't already been
          * compiled, create the texture mipmap levels. */
        virtual void apply(crState& state) const;
        

        /** Encapsulates queries of extension availability, obtains extension
          * function pointers, and provides convenience wrappers for
          * calling extension functions. */        
        class Extensions : public CRCore::Referenced
        {
            public:
                Extensions(unsigned int contextID);

                Extensions(const Extensions& rhs);
                
                void lowestCommonDenominator(const Extensions& rhs);
                    
                void setupGLExtensions(unsigned int contextID);

                void setTexture3DSupported(bool flag) { m_isTexture3DSupported=flag; }
                bool isTexture3DSupported() const { return m_isTexture3DSupported; }
                
                void setTexture3DFast(bool flag) { m_isTexture3DFast=flag; }
                bool isTexture3DFast() const { return m_isTexture3DFast; }

                void setMaxTexture3DSize(GLint maxsize) { m_maxTexture3DSize=maxsize; }
                GLint maxTexture3DSize() const { return m_maxTexture3DSize; }

                void setTexImage3DProc(void* ptr) { m_glTexImage3D = ptr; }
                void glTexImage3D( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels) const;

                void setTexSubImage3DProc(void* ptr) { m_glTexSubImage3D = ptr; }
                void glTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels) const;
                
                void setCopyTexSubImage3DProc(void* ptr) { m_glCopyTexSubImage3D = ptr; }
                void glCopyTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height ) const;
                
                bool isCompressedTexImage3DSupported() const { return m_glCompressedTexImage3D!=0; }
                void setCompressedTexImage3DProc(void* ptr) { m_glCompressedTexImage3D = ptr; }
                void glCompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data) const;

                bool isCompressedTexSubImage3DSupported() const { return m_glCompressedTexSubImage3D!=0; }
                void setCompressedTexSubImage3DProc(void* ptr) { m_glCompressedTexSubImage3D = ptr; }
                void glCompressedTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data ) const;

                void setBuild3DMipmapsProc(void* ptr) { m_gluBuild3DMipmaps = ptr; }
                void gluBuild3DMipmaps( GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *data) const;

				static void clearExtensions();
            protected:

                ~Extensions() {}
                
                bool    m_isTexture3DSupported;
                bool    m_isTexture3DFast;
                GLint   m_maxTexture3DSize;

                void*   m_glTexImage3D;
                void*   m_glTexSubImage3D;
                void*   m_glCompressedTexImage3D;
                void*   m_glCompressedTexSubImage3D;
                void*   m_glCopyTexSubImage3D;
                void*   m_gluBuild3DMipmaps;

        };
        
        /** Encapsulates queries of extension availability, obtains extension
          * function pointers, and provides convenience wrappers for
          * calling extension functions. */        
        static Extensions* getExtensions(unsigned int contextID,bool createIfNotInitalized);

        /** Overrides Extensions objects across graphics contexts. Typically
          * used to ensure the same lowest common denominator of extensions
          * on systems with different graphics pipes. */
        static void setExtensions(unsigned int contextID,Extensions* extensions);

    protected :

        virtual ~crTexture3D();

        void computeRequiredTextureDimensions(crState& state, const CRCore::crImage& image,GLsizei& width, GLsizei& height,GLsizei& depth, GLsizei& numMipmapLevels) const;

        virtual void computeInternalFormat() const;
        void allocateMipmap(crState& state) const;

        void applyTexImage3D(GLenum target, crImage* image, crState& state, GLsizei& inwidth, GLsizei& inheight, GLsizei& indepth, GLsizei& numMipmapLevels) const;

        /** It's not ideal that m_image is mutable, but it's required since
          * crImage::ensureDimensionsArePowerOfTwo() can only be called in a
          * valid OpenGL context, and therefore within crTexture::apply, which
          * is const. */
        mutable ref_ptr<crImage> m_image;

        /** Subloaded images can have different texture and image sizes. */
        mutable GLsizei m_textureWidth, m_textureHeight, m_textureDepth;
        
        /** Number of mip map levels the the texture has been created with, */
        mutable GLsizei m_numMipmapLevels;

        ref_ptr<SubloadCallback> m_subloadCallback;

        typedef buffered_value<unsigned int> ImageModifiedCount;
        mutable ImageModifiedCount m_modifiedCount;

};

}

#endif
