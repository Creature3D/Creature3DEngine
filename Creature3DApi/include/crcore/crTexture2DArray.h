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
#ifndef CRCORE_CRTEXTURE2DARRAY_H
#define CRCORE_CRTEXTURE2DARRAY_H 1

#include <CRCore/crTexture.h>
#include <list>

namespace CRCore {

/** crTexture2DArray state class which encapsulates OpenGL 2D array texture functionality. 
 * crTexture arrays were introduced with Shader Model 4.0 hardware.
 * 
 * A 2D texture array does contain textures sharing the same properties (e.g. size, bitdepth,...)
 * in a layered structure. See http://www.opengl.org/registry/specs/EXT/texture_array.txt for more info.
 */
class CR_EXPORT crTexture2DArray : public crTexture
{

    public :
        
        crTexture2DArray();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy. */
        crTexture2DArray(const crTexture2DArray& cm,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_StateAttribute(CRCore, crTexture2DArray, TEXTURE);
        
        /** Return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs. */
        virtual int compare(const crStateAttribute& rhs) const;
		virtual int compare(const crStateAttribute& rhs);

        virtual GLenum getTextureTarget() const { return GL_TEXTURE_2D_ARRAY_EXT; }

        /** Set the texture image for specified layer. */
        virtual void setImage(unsigned int layer, crImage* image);

        /** Get the texture image for specified layer. */
        virtual crImage* getImage(unsigned int layer);

        /** Get the const texture image for specified layer. */
        virtual const crImage* getImage(unsigned int layer) const;

        /** Get the number of images that are assigned to the crTexture. 
         * The number is equal to the texture depth. To get the maximum possible
         * image/layer count, you have to use the extension subclass, since it provides
         * graphic context dependent information.
         */
        virtual unsigned int getNumImages() const { return getTextureDepth(); } 

        /** Check how often was a certain layer in the given context modified */
        inline unsigned int& getModifiedCount(unsigned int layer, unsigned int contextID) const
        {
            // get the modified count for the current contextID.
            return m_modifiedCount[layer][contextID];
        }

        /** Set the texture width and height. If width or height are zero then
          * the respective size value is calculated from the source image sizes.
          * Depth parameter specifies the number of layers to be used.
        */
        void setTextureSize(int width, int height, int depth);

        void setTextureWidth(int width) { m_textureWidth=width; }
        void setTextureHeight(int height) { m_textureHeight=height; }
        void setTextureDepth(int depth);

        virtual int getTextureWidth() const { return m_textureWidth; }
        virtual int getTextureHeight() const { return m_textureHeight; }
        virtual int getTextureDepth() const { return m_textureDepth; }

        class CR_EXPORT SubloadCallback : public Referenced
        {
            public:
                virtual void load(const crTexture2DArray& texture,crState& state) const = 0;
                virtual void subload(const crTexture2DArray& texture,crState& state) const = 0;
        };
        
        
        void setSubloadCallback(SubloadCallback* cb) { m_subloadCallback = cb;; }
        
        SubloadCallback* getSubloadCallback() { return m_subloadCallback.get(); }

        const SubloadCallback* getSubloadCallback() const { return m_subloadCallback.get(); }
        
        

        /** Set the number of mip map levels the the texture has been created with.
          * Should only be called within an CRCore::crTexture::apply() and custom OpenGL texture load.
        */
        void setNumMipmapLevels(unsigned int num) const { m_numMipmapLevels=num; }

        /** Get the number of mip map levels the the texture has been created with. */
        unsigned int getNumMipmapLevels() const { return m_numMipmapLevels; }        

        /** Copies a two-dimensional texture subimage, as per
          * glCopyTexSubImage3D. Updates a portion of an existing OpenGL
          * texture object from the current OpenGL background framebuffer
          * contents at position \a x, \a y with width \a width and height
          * \a height. Loads framebuffer data into the texture using offsets
          * \a xoffset and \a yoffset. \a zoffset specifies the layer of the texture 
          * array to which the result is copied. 
          */
        void copyTexSubImage2DArray(crState& state, int xoffset, int yoffset, int zoffset, int x, int y, int width, int height );

        /** Bind the texture if already compiled. Otherwise recompile.
        */
        virtual void apply(crState& state) const;
        

        /** Extensions class which encapsulates the querying of extensions and
          * associated function pointers, and provides convenience wrappers to 
          * check for the extensions or use the associated functions.
        */
        class Extensions : public CRCore::Referenced
        {
            public:
                Extensions(unsigned int contextID);

                Extensions(const Extensions& rhs);
                
                void lowestCommonDenominator(const Extensions& rhs);
                
                void setupGLExtensions(unsigned int contextID);

                void setTexture2DArraySupported(bool flag) { m_isTexture2DArraySupported=flag; }
                bool isTexture2DArraySupported() const { return m_isTexture2DArraySupported; }
                
                void setTexture3DSupported(bool flag) { m_isTexture3DSupported=flag; }
                bool isTexture3DSupported() const { return m_isTexture3DSupported; }

                void setMaxLayerCount(GLint count) { m_maxLayerCount = count; }
                GLint maxLayerCount() const { return m_maxLayerCount; }
                
                void setMax2DSize(GLint size) { m_max2DSize = size; }
                GLint max2DSize() const { return m_max2DSize; }
                
                void glTexImage3D( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels) const;

                void glTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels) const;
                
                void glCopyTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height ) const;
                
                bool isCompressedTexImage3DSupported() const { return m_glCompressedTexImage3D!=0; }
                void glCompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data) const;

                bool isCompressedTexSubImage3DSupported() const { return m_glCompressedTexSubImage3D!=0; }
                void glCompressedTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data ) const;
				
				static void clearExtensions();
            protected:

                ~Extensions() {}
                
                bool    m_isTexture2DArraySupported;
                bool    m_isTexture3DSupported;
                
                GLint   m_maxLayerCount;
                GLint   m_max2DSize;

                typedef void (APIENTRY * GLTexImage3DProc)      ( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
                typedef void (APIENTRY * GLTexSubImage3DProc)   ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
                typedef void (APIENTRY * CompressedTexImage3DArbProc) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
                typedef void (APIENTRY * CompressedTexSubImage3DArbProc) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
                typedef void (APIENTRY * GLCopyTexSubImageProc) ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height );

                GLTexImage3DProc                m_glTexImage3D;
                GLTexSubImage3DProc             m_glTexSubImage3D;
                CompressedTexImage3DArbProc     m_glCompressedTexImage3D;
                CompressedTexSubImage3DArbProc  m_glCompressedTexSubImage3D;
                GLCopyTexSubImageProc           m_glCopyTexSubImage3D;
                
        };
        
        /** Function to call to get the extension of a specified context.
          * If the Extension object for that context has not yet been created
          * and the 'createIfNotInitalized' flag been set to false then returns NULL.
          * If 'createIfNotInitalized' is true then the Extensions object is 
          * automatically created. However, in this case the extension object will
          * only be created with the graphics context associated with ContextID.
        */
        static Extensions* getExtensions(unsigned int contextID,bool createIfNotInitalized);

        /** The setExtensions method allows users to override the extensions across graphics contexts.
          * Typically used when you have different extensions supported across graphics pipes
          * but need to ensure that they all use the same low common denominator extensions.
        */
        static void setExtensions(unsigned int contextID,Extensions* extensions);


    protected :

        virtual ~crTexture2DArray();
        
        bool imagesValid() const;
        
        virtual void computeInternalFormat() const;
        void allocateMipmap(crState& state) const;

        void applyTexImage2DArray_subload(crState& state, crImage* image, GLsizei inwidth, GLsizei inheight, GLsizei indepth, GLenum inInternalFormat, GLsizei& numMipmapLevels) const;

        /**
         * Use std::vector to encapsulate referenced pointers to images of different layers.
         * Vectors gives us a random access iterator. The overhead of non-used elements is negligeable */
        mutable std::vector<ref_ptr<crImage> > m_images;

        // subloaded images can have different texture and image sizes.
        mutable GLsizei m_textureWidth, m_textureHeight, m_textureDepth;
        
        // number of mip map levels the the texture has been created with,        
        mutable GLsizei m_numMipmapLevels;

        ref_ptr<SubloadCallback> m_subloadCallback;

        typedef buffered_value<unsigned int> ImageModifiedCount;
        mutable std::vector<ImageModifiedCount> m_modifiedCount;
};

}

#endif
