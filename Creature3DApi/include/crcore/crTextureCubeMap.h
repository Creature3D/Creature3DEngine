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
#ifndef CRCORE_CRTEXTURECUBEMAP_H
#define CRCORE_CRTEXTURECUBEMAP_H 1

#include <CRCore/crTexture.h>

#ifndef GL_TEXTURE_CUBE_MAP
    #define GL_TEXTURE_CUBE_MAP             0x8513
    #define GL_TEXTURE_BINDING_CUBE_MAP     0x8514
    #define GL_TEXTURE_CUBE_MAP_POSITIVE_X  0x8515
    #define GL_TEXTURE_CUBE_MAP_NEGATIVE_X  0x8516
    #define GL_TEXTURE_CUBE_MAP_POSITIVE_Y  0x8517
    #define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y  0x8518
    #define GL_TEXTURE_CUBE_MAP_POSITIVE_Z  0x8519
    #define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z  0x851A
    #define GL_PROXY_TEXTURE_CUBE_MAP       0x851B
    #define GL_MAX_CUBE_MAP_TEXTURE_SIZE    0x851C
#endif

#define CUBEMAP_FILENAME(cupmap,face,ext) cupmap + "_" #face +"."+ext

namespace CRCore {

/** crTextureCubeMap state class which encapsulates OpenGL texture cubemap functionality. */
class CR_EXPORT crTextureCubeMap : public crTexture
{

    public :
        
        crTextureCubeMap();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy. */
        crTextureCubeMap(const crTextureCubeMap& cm,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_StateAttribute(CRCore, crTextureCubeMap,TEXTURE);
        
        /** Return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs. */
        virtual int compare(const crStateAttribute& rhs) const;
		virtual int compare(const crStateAttribute& rhs);

        virtual GLenum getTextureTarget() const { return GL_TEXTURE_CUBE_MAP; }

        enum Face {
            POSITIVE_X=0,
            NEGATIVE_X=1,
            POSITIVE_Y=2,
            NEGATIVE_Y=3,
            POSITIVE_Z=4,
            NEGATIVE_Z=5
        };

		std::string getCupMapNameID();
        /** Set the texture image for specified face. */
        virtual void setImage(unsigned int face, crImage* image);

        /** Get the texture image for specified face. */
        virtual crImage* getImage(unsigned int face);

        /** Get the const texture image for specified face. */
        virtual const crImage* getImage(unsigned int face) const;

        /** Get the number of images that can be assigned to the crTexture. */
        virtual unsigned int getNumImages() const { return 6; }

        inline unsigned int& getModifiedCount(unsigned int face,unsigned int contextID) const
        {
            // get the modified count for the current contextID.
            return m_modifiedCount[face][contextID];
        }

        /** Set the texture width and height. If width or height are zero then
          * the repsective size value is calculated from the source image sizes.
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

        class CR_EXPORT SubloadCallback : public Referenced
        {
            public:
                virtual void load(const crTextureCubeMap& texture,crState& state) const = 0;
                virtual void subload(const crTextureCubeMap& texture,crState& state) const = 0;
        };
        
        void setSubloadCallback(SubloadCallback* cb) { m_subloadCallback = cb;; }
        
        SubloadCallback* getSubloadCallback() { return m_subloadCallback.get(); }

        const SubloadCallback* getSubloadCallback() const { return m_subloadCallback.get(); }


        /** Set the number of mip map levels the the texture has been created with.
          * Should only be called within an CRCore::Texuture::apply() and custom OpenGL texture load.
        */
        void setNumMipmapLevels(unsigned int num) const { m_numMipmapLevels=num; }

        /** Get the number of mip map levels the the texture has been created with. */
        unsigned int getNumMipmapLevels() const { return m_numMipmapLevels; }        

        /** Copies a two-dimensional texture subimage, as per
          * glCopyTexSubImage2D. Updates a portion of an existing OpenGL
          * texture object from the current OpenGL background framebuffer
          * contents at position \a x, \a y with width \a width and height
          * \a height. Loads framebuffer data into the texture using offsets
          * \a xoffset and \a yoffset. \a width and \a height must be powers
          * of two. */
        void copyTexSubImageCubeMap(crState& state, int face, int xoffset, int yoffset, int x, int y, int width, int height );


        /** On first apply (unless already compiled), create the mipmapped
          * texture and bind it. Subsequent apply will simple bind to texture.
        */
        virtual void apply(crState& state) const;
        

        /** Extensions class which encapsulates the querying of extensions and
          * associated function pointers, and provides convinience wrappers to 
          * check for the extensions or use the associated functions.
        */
        class Extensions : public CRCore::Referenced
        {
            public:
                Extensions(unsigned int contextID);

                Extensions(const Extensions& rhs);
                
                void lowestCommonDenominator(const Extensions& rhs);
                
                void setupGLExtensions(unsigned int contextID);

                void setCubeMapSupported(bool flag) { m_isCubeMapSupported=flag; }
                bool isCubeMapSupported() const { return m_isCubeMapSupported; }
				static void clearExtensions();
            protected:

                ~Extensions() {}
                
                bool    m_isCubeMapSupported;

        };
        
        /** Function to call to get the extension of a specified context.
          * If the Exentsion object for that context has not yet been created
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

        virtual ~crTextureCubeMap();
        
        bool imagesValid() const;
        
        virtual void computeInternalFormat() const;
        void allocateMipmap(crState& state) const;

        mutable ref_ptr<crImage> m_images[6];

        // subloaded images can have different texture and image sizes.
        mutable GLsizei m_textureWidth, m_textureHeight;
        
        // number of mip map levels the the texture has been created with,        
        mutable GLsizei m_numMipmapLevels;

        ref_ptr<SubloadCallback> m_subloadCallback;

        typedef buffered_value<unsigned int> ImageModifiedCount;
        mutable ImageModifiedCount m_modifiedCount[6];
};

}

#endif
