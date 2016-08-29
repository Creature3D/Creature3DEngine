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
#ifndef CRCORE_CRTEXTURE_H
#define CRCORE_CRTEXTURE_H 1

#include <crgl/GL.h>
#include <CRCore/crImage.h>
#include <CRCore/crStateAttribute.h>
#include <CRCore/crGraphicsContext.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/crVector4.h>
#include <CRCore/buffered_value.h>

#include <list>
#include <map>

// If not defined by gl.h use the definition found in:
// http://oss.sgi.com/projects/ogl-sample/registry/EXT/texture_filter_anisotropic.txt
#ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
    #define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#endif

#ifndef GL_ARB_texture_compression
    #define GL_COMPRESSED_ALPHA_ARB                 0x84E9
    #define GL_COMPRESSED_LUMINANCE_ARB             0x84EA
    #define GL_COMPRESSED_LUMINANCE_ALPHA_ARB       0x84EB
    #define GL_COMPRESSED_INTENSITY_ARB             0x84EC
    #define GL_COMPRESSED_RGB_ARB                   0x84ED
    #define GL_COMPRESSED_RGBA_ARB                  0x84EE
    #define GL_TEXTURE_COMPRESSION_HINT_ARB         0x84EF
    #define GL_TEXTURE_COMPRESSED_ARB               0x86A1
    #define GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB   0x86A2
    #define GL_COMPRESSED_TEXTURE_FORMATS_ARB       0x86A3
#endif

#ifndef GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB
    #define GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB    0x86A0
#endif

#ifndef GL_EXT_texture_compression_s3tc
    #define GL_COMPRESSED_RGB_S3TC_DXT1_EXT         0x83F0
    #define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT        0x83F1
    #define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT        0x83F2
    #define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT        0x83F3
#endif

#ifndef GL_EXT_texture_compression_rgtc
  #define GL_COMPRESSED_RED_RGTC1_EXT                0x8DBB
  #define GL_COMPRESSED_SIGNED_RED_RGTC1_EXT         0x8DBC
  #define GL_COMPRESSED_RED_GREEN_RGTC2_EXT          0x8DBD
  #define GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT   0x8DBE
#endif

#ifndef GL_IMG_texture_compression_pvrtc
    #define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG      0x8C00
    #define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG      0x8C01
    #define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG     0x8C02
    #define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG     0x8C03
#endif

#ifndef GL_OES_compressed_ETC1_RGB8_texture
    #define GL_ETC1_RGB8_OES                            0x8D64   
#endif

#ifndef GL_ARB_INTERNAL_TEXTURE_FORMAT
    #define GL_RGBA32F_ARB                           0x8814
    #define GL_RGB32F_ARB                            0x8815
    #define GL_ALPHA32F_ARB                          0x8816
    #define GL_INTENSITY32F_ARB                      0x8817
    #define GL_LUMINANCE32F_ARB                      0x8818
    #define GL_LUMINANCE_ALPHA32F_ARB                0x8819
    #define GL_RGBA16F_ARB                           0x881A
    #define GL_RGB16F_ARB                            0x881B
    #define GL_ALPHA16F_ARB                          0x881C
    #define GL_INTENSITY16F_ARB                      0x881D
    #define GL_LUMINANCE16F_ARB                      0x881E
    #define GL_LUMINANCE_ALPHA16F_ARB                0x881F
#endif

#ifndef GL_ARB_PIXEL_DATA
    #define GL_HALF_FLOAT_ARB                        0x140B
#endif

#ifndef GL_NV_texture_shader
    #define GL_HILO_NV                              0x86F4
    #define GL_DSDT_NV                              0x86F5
    #define GL_DSDT_MAG_NV                          0x86F6
    #define GL_DSDT_MAG_VIB_NV                      0x86F7
    #define GL_HILO16_NV                            0x86F8
    #define GL_SIGNED_HILO_NV                       0x86F9
    #define GL_SIGNED_HILO16_NV                     0x86FA
    #define GL_SIGNED_RGBA_NV                       0x86FB
    #define GL_SIGNED_RGBA8_NV                      0x86FC
    #define GL_SIGNED_RGB_NV                        0x86FE
    #define GL_SIGNED_RGB8_NV                       0x86FF
    #define GL_SIGNED_LUMINANCE_NV                  0x8701
    #define GL_SIGNED_LUMINANCE8_NV                 0x8702
    #define GL_SIGNED_LUMINANCE_ALPHA_NV            0x8703
    #define GL_SIGNED_LUMINANCE8_ALPHA8_NV          0x8704
    #define GL_SIGNED_ALPHA_NV                      0x8705
    #define GL_SIGNED_ALPHA8_NV                     0x8706
    #define GL_SIGNED_INTENSITY_NV                  0x8707
    #define GL_SIGNED_INTENSITY8_NV                 0x8708
    #define GL_DSDT8_NV                             0x8709
    #define GL_DSDT8_MAG8_NV                        0x870A
    #define GL_DSDT8_MAG8_INTENSITY8_NV             0x870B
    #define GL_SIGNED_RGB_UNSIGNED_ALPHA_NV         0x870C
    #define GL_SIGNED_RGB8_UNSIGNED_ALPHA8_NV       0x870D
#endif

#ifndef GL_NV_float_buffer
    #define GL_FLOAT_R_NV                           0x8880
    #define GL_FLOAT_RG_NV                          0x8881
    #define GL_FLOAT_RGB_NV                         0x8882
    #define GL_FLOAT_RGBA_NV                        0x8883
    #define GL_FLOAT_R16_NV                         0x8884
    #define GL_FLOAT_R32_NV                         0x8885
    #define GL_FLOAT_RG16_NV                        0x8886
    #define GL_FLOAT_RG32_NV                        0x8887
    #define GL_FLOAT_RGB16_NV                       0x8888
    #define GL_FLOAT_RGB32_NV                       0x8889
    #define GL_FLOAT_RGBA16_NV                      0x888A
    #define GL_FLOAT_RGBA32_NV                      0x888B
#endif

#ifndef GL_NV_half_float
    #define GL_HALF_FLOAT_NV                        0x140B
#endif

#ifndef GL_ATI_texture_float
    #define GL_RGBA_FLOAT32_ATI                     0x8814
    #define GL_RGB_FLOAT32_ATI                      0x8815
    #define GL_ALPHA_FLOAT32_ATI                    0x8816
    #define GL_INTENSITY_FLOAT32_ATI                0x8817
    #define GL_LUMINANCE_FLOAT32_ATI                0x8818
    #define GL_LUMINANCE_ALPHA_FLOAT32_ATI          0x8819
    #define GL_RGBA_FLOAT16_ATI                     0x881A
    #define GL_RGB_FLOAT16_ATI                      0x881B
    #define GL_ALPHA_FLOAT16_ATI                    0x881C
    #define GL_INTENSITY_FLOAT16_ATI                0x881D
    #define GL_LUMINANCE_FLOAT16_ATI                0x881E
    #define GL_LUMINANCE_ALPHA_FLOAT16_ATI          0x881F
#endif

#ifndef GL_MIRRORED_REPEAT_IBM
    #define GL_MIRRORED_REPEAT_IBM            0x8370
#endif

#ifndef GL_CLAMP_TO_EDGE
    #define GL_CLAMP_TO_EDGE                  0x812F
#endif

#ifndef GL_CLAMP
    #define GL_CLAMP                          0x2900  
#endif

#ifndef GL_CLAMP_TO_BORDER_ARB
    #define GL_CLAMP_TO_BORDER_ARB            0x812D
#endif

#ifndef GL_INTENSITY
    // OpenGL ES1 and ES2 doesn't provide GL_INTENSITY
    #define GL_INTENSITY 0x8049
#endif

#ifndef GL_GENERATE_MIPMAP_SGIS
    #define GL_GENERATE_MIPMAP_SGIS           0x8191
    #define GL_GENERATE_MIPMAP_HINT_SGIS      0x8192
#endif

#ifndef GL_TEXTURE_3D
    #define GL_TEXTURE_3D                     0x806F
#endif

#ifndef GL_TEXTURE_2D_ARRAY_EXT
    #define GL_TEXTURE_2D_ARRAY_EXT           0x8C1A
    #define GL_TEXTURE_2D_ARRAY_EXT                        0x8C1A
    #define GL_PROXY_TEXTURE_2D_ARRAY_EXT                  0x8C1B
    #define GL_TEXTURE_BINDING_2D_ARRAY_EXT                0x8C1D
    #define GL_MAX_ARRAY_TEXTURE_LAYERS_EXT                0x88FF
    #define GL_COMPARE_REF_DEPTH_TO_TEXTURE_EXT            0x884E
    #define GL_SAMPLER_2D_ARRAY_EXT                        0x8DC1
    #define GL_SAMPLER_2D_ARRAY_SHADOW_EXT                 0x8DC4
    #define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER_EXT    0x8CD4
#endif

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

#ifndef GL_TEXTURE_BINDING_3D
    #define GL_TEXTURE_BINDING_3D             0x806A
#endif

#ifndef GL_DEPTH_TEXTURE_MODE_ARB
    #define GL_DEPTH_TEXTURE_MODE_ARB         0x884B
#endif

#ifndef GL_TEXTURE_COMPARE_MODE_ARB
    #define GL_TEXTURE_COMPARE_MODE_ARB       0x884C
#endif
#ifndef GL_TEXTURE_COMPARE_FUNC_ARB
    #define GL_TEXTURE_COMPARE_FUNC_ARB       0x884D
#endif
#ifndef GL_COMPARE_R_TO_TEXTURE_ARB
    #define GL_COMPARE_R_TO_TEXTURE_ARB       0x884E
#endif

#ifndef TEXTURE_COMPARE_FAIL_VALUE_ARB
    #define TEXTURE_COMPARE_FAIL_VALUE_ARB    0x80BF
#endif

#if !defined( GL_MAX_TEXTURE_UNITS )
    #define GL_MAX_TEXTURE_UNITS              0x84E2
#endif

#ifndef  GL_TEXTURE_DEPTH
    #define GL_TEXTURE_DEPTH                  0x8071
#endif

#ifndef GL_TEXTURE_2D_MULTISAMPLE
    #define GL_TEXTURE_2D_MULTISAMPLE         0x9100
#endif

// Integer texture extension as in http://www.opengl.org/registry/specs/EXT/texture_integer.txt
#ifndef GL_EXT_texture_integer
    #define GL_RGBA32UI_EXT                                    0x8D70
    #define GL_RGB32UI_EXT                                     0x8D71
    #define GL_ALPHA32UI_EXT                                   0x8D72
    #define GL_INTENSITY32UI_EXT                               0x8D73
    #define GL_LUMINANCE32UI_EXT                               0x8D74
    #define GL_LUMINANCE_ALPHA32UI_EXT                         0x8D75

    #define GL_RGBA16UI_EXT                                    0x8D76
    #define GL_RGB16UI_EXT                                     0x8D77
    #define GL_ALPHA16UI_EXT                                   0x8D78
    #define GL_INTENSITY16UI_EXT                               0x8D79
    #define GL_LUMINANCE16UI_EXT                               0x8D7A
    #define GL_LUMINANCE_ALPHA16UI_EXT                         0x8D7B

    #define GL_RGBA8UI_EXT                                     0x8D7C
    #define GL_RGB8UI_EXT                                      0x8D7D
    #define GL_ALPHA8UI_EXT                                    0x8D7E
    #define GL_INTENSITY8UI_EXT                                0x8D7F
    #define GL_LUMINANCE8UI_EXT                                0x8D80
    #define GL_LUMINANCE_ALPHA8UI_EXT                          0x8D81

    #define GL_RGBA32I_EXT                                     0x8D82
    #define GL_RGB32I_EXT                                      0x8D83
    #define GL_ALPHA32I_EXT                                    0x8D84
    #define GL_INTENSITY32I_EXT                                0x8D85
    #define GL_LUMINANCE32I_EXT                                0x8D86
    #define GL_LUMINANCE_ALPHA32I_EXT                          0x8D87

    #define GL_RGBA16I_EXT                                     0x8D88
    #define GL_RGB16I_EXT                                      0x8D89
    #define GL_ALPHA16I_EXT                                    0x8D8A
    #define GL_INTENSITY16I_EXT                                0x8D8B
    #define GL_LUMINANCE16I_EXT                                0x8D8C
    #define GL_LUMINANCE_ALPHA16I_EXT                          0x8D8D

    #define GL_RGBA8I_EXT                                      0x8D8E
    #define GL_RGB8I_EXT                                       0x8D8F
    #define GL_ALPHA8I_EXT                                     0x8D90
    #define GL_INTENSITY8I_EXT                                 0x8D91
    #define GL_LUMINANCE8I_EXT                                 0x8D92
    #define GL_LUMINANCE_ALPHA8I_EXT                           0x8D93

    #define GL_RED_INTEGER_EXT                                 0x8D94
    #define GL_GREEN_INTEGER_EXT                               0x8D95
    #define GL_BLUE_INTEGER_EXT                                0x8D96
    #define GL_ALPHA_INTEGER_EXT                               0x8D97
    #define GL_RGB_INTEGER_EXT                                 0x8D98
    #define GL_RGBA_INTEGER_EXT                                0x8D99
    #define GL_BGR_INTEGER_EXT                                 0x8D9A
    #define GL_BGRA_INTEGER_EXT                                0x8D9B
    #define GL_LUMINANCE_INTEGER_EXT                           0x8D9C
    #define GL_LUMINANCE_ALPHA_INTEGER_EXT                     0x8D9D

    #define GL_RGBA_INTEGER_MODE_EXT                           0x8D9E
#endif
namespace CRCore {
class crTexture;
class crTextureCubeMap;
class CR_EXPORT ImageDataRequestHandler : public CRCore::Referenced
{
public:            
	virtual crImage *requestImageFile(const std::string& fileName, crTexture *texture){ return NULL; }
	virtual bool requestCubeImageFile(const std::string& fileName, crTextureCubeMap *textureCubeMap){ return false; }
protected:
	virtual ~ImageDataRequestHandler() {}
};
/** crTexture pure virtual base class that encapsulates OpenGl texture
  * functionality common to the various types of OSG textures.
*/
class CR_EXPORT crTexture : public CRCore::crStateAttribute
{

    public :
    
        static unsigned int s_numberTextureReusedLastInLastFrame;
        //static unsigned int s_numberNewTextureInLastFrame;
        //static unsigned int s_numberDeletedTextureInLastFrame;
		static unsigned int s_minimumNumberOfTextureObjectsToRetainInCache;
		static unsigned int s_numberTextures;

        crTexture();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy. */
        crTexture(const crTexture& text,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        virtual CRCore::crBase* cloneType() const = 0;
        virtual CRCore::crBase* clone(const crCopyOp& copyop) const = 0;
        virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crTexture *>(obj)!=NULL; }
        virtual const char* libraryName() const { return "CRCore"; }
        virtual const char* className() const { return "crTexture"; }

        virtual Type getType() const { return TEXTURE; }

        virtual bool isTextureAttribute() const { return true; }

        virtual GLenum getTextureTarget() const = 0;

		//virtual bool getModeUsage(crStateAttribute::ModeUsage& usage) const
		//{
		//	usage.usesTextureMode(getTextureTarget());
		//	return true;
		//}
		virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
		{
			modes.push_back(getTextureTarget());
		}

        virtual int getTextureWidth() const { return 0; }
        virtual int getTextureHeight() const { return 0; }
        virtual int getTextureDepth() const { return 0; }

        enum WrapParameter {
            WRAP_S,
            WRAP_T,
            WRAP_R
        };

        enum WrapMode {
            CLAMP  = GL_CLAMP,
            CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
            CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER_ARB,
            REPEAT = GL_REPEAT,
            MIRROR = GL_MIRRORED_REPEAT_IBM
        };

        /** Sets the texture wrap mode. */
        void setWrap(WrapParameter which, WrapMode wrap);
        /** Gets the texture wrap mode. */
        WrapMode getWrap(WrapParameter which) const;


        /** Sets the border color. Only used when wrap mode is CLAMP_TO_BORDER.
         * The border color will be casted to the appropriate type to match the
         * internal pixel format of the texture. */
        void setBorderColor(const crVector4d& color) { m_borderColor = color; dirtyTextureParameters(); }

        /** Gets the border color. */
        const crVector4d& getBorderColor() const { return m_borderColor; }

        /** Sets the border width. */
        void setBorderWidth(GLint width) { m_borderWidth = width; dirtyTextureParameters(); }
        
        GLint getBorderWidth() const { return m_borderWidth; }

        enum FilterParameter {
            MIN_FILTER,
            MAG_FILTER
        };

        enum FilterMode {
            LINEAR                    = GL_LINEAR,
            LINEAR_MIPMAP_LINEAR      = GL_LINEAR_MIPMAP_LINEAR,
            LINEAR_MIPMAP_NEAREST     = GL_LINEAR_MIPMAP_NEAREST,
            NEAREST                   = GL_NEAREST,
            NEAREST_MIPMAP_LINEAR     = GL_NEAREST_MIPMAP_LINEAR,
            NEAREST_MIPMAP_NEAREST    = GL_NEAREST_MIPMAP_NEAREST
        };


        /** Sets the texture filter mode. */
        void setFilter(FilterParameter which, FilterMode filter);

        /** Gets the texture filter mode. */
        FilterMode getFilter(FilterParameter which) const;

        /** Sets the maximum anisotropy value, default value is 1.0 for no
          * anisotropic filtering. If hardware does not support anisotropic
          * filtering, use normal filtering (equivalent to a max anisotropy
          * value of 1.0. Valid range is 1.0f upwards.  The maximum value
          * depends on the graphics system. */
        void setMaxAnisotropy(float anis);
        
        /** Gets the maximum anisotropy value. */
        inline float getMaxAnisotropy() const { return m_maxAnisotropy; }

        /** Sets the hardware mipmap generation hint. If enabled, it will
          * only be used if supported in the graphics system. */
        inline void setUseHardwareMipMapGeneration(bool useHardwareMipMapGeneration) { m_useHardwareMipMapGeneration = useHardwareMipMapGeneration; }

        /** Gets the hardware mipmap generation hint. */
        inline bool getUseHardwareMipMapGeneration() const { return m_useHardwareMipMapGeneration; }

        /** Sets whether or not the apply() function will unreference the image
          * data. If enabled, and the image data is only referenced by this
          * crTexture, apply() will delete the image data. */
        inline void setUnRefImageDataAfterApply(bool flag) { m_unrefImageDataAfterApply = flag; }
        
        /** Gets whether or not apply() unreferences image data. */
        inline bool getUnRefImageDataAfterApply() const { return m_unrefImageDataAfterApply; }

        /** Sets whether to use client storage for the texture, if supported
          * by the graphics system. Note: If enabled, and the graphics system
          * supports it, the CRCore::crImage(s) associated with this texture cannot
          * be deleted, so the UnRefImageDataAfterApply flag would be ignored. */
        inline void setClientStorageHint(bool flag) { m_clientStorageHint = flag; }

        /** Gets whether to use client storage for the texture. */
        inline bool getClientStorageHint() const { return m_clientStorageHint; }

        /** Sets whether to force the texture to resize images that have dimensions 
          * that are not a power of two. If enabled, NPOT images will be resized,
          * whether or not NPOT textures are supported by the hardware. If disabled,
          * NPOT images will not be resized if supported by hardware. */
        inline void setResizeNonPowerOfTwoHint(bool flag) { m_resizeNonPowerOfTwoHint = flag; }

        /** Gets whether texture will force non power to two images to be resized. */
        inline bool getResizeNonPowerOfTwoHint() const { return m_resizeNonPowerOfTwoHint; }

        enum InternalFormatMode {
            USE_IMAGE_DATA_FORMAT,
            USE_USER_DEFINED_FORMAT,
            USE_ARB_COMPRESSION,
            USE_S3TC_DXT1_COMPRESSION,
            USE_S3TC_DXT3_COMPRESSION,
            USE_S3TC_DXT5_COMPRESSION
        };

        /** Sets the internal texture format mode. Note: If the texture format is
          * USE_IMAGE_DATA_FORMAT, USE_ARB_COMPRESSION, or USE_S3TC_COMPRESSION,
          * the internal format mode is set automatically and will overwrite the
          * previous m_internalFormat. */
        inline void setInternalFormatMode(InternalFormatMode mode) { m_internalFormatMode = mode; }

        /** Gets the internal texture format mode. */
        inline InternalFormatMode getInternalFormatMode() const { return m_internalFormatMode; }

        /** Sets the internal texture format. Implicitly sets the
          * internalFormatMode to USE_USER_DEFINED_FORMAT.
          * The corresponding internal format type will be computed. */
        inline void setInternalFormat(GLint internalFormat)
        {
            m_internalFormatMode = USE_USER_DEFINED_FORMAT;
            m_internalFormat = internalFormat;
            computeInternalFormatType();
        }


        /** Gets the internal texture format. */
        inline GLint getInternalFormat() const { if (m_internalFormat==0) computeInternalFormat(); return m_internalFormat; }
        
        /** Return true if the internal format is one of the compressed formats.*/
        bool isCompressedInternalFormat() const;

        /** Sets the external source image format, used as a fallback when no CRCore::crImage is attached to provide the source image format. */
        inline void setSourceFormat(GLenum sourceFormat) { m_sourceFormat = sourceFormat; }

        /** Gets the external source image format. */
        inline GLenum getSourceFormat() const { return m_sourceFormat; }

        /** Sets the external source data type, used as a fallback when no CRCore::crImage is attached to provide the source image format.*/
        inline void setSourceType(GLenum sourceType) { m_sourceType = sourceType; }

        /** Gets the external source data type.*/
        inline GLenum getSourceType() const { return m_sourceType; }

        /** crTexture type determined by the internal texture format */
        enum InternalFormatType{

            //! default OpenGL format (clamped values to [0,1) or [0,255])
            NORMALIZED = 0x0,

            //! float values, Shader Model 3.0 (see ARB_texture_float)
            FLOAT = 0x1,

            //! Signed integer values (see EXT_texture_integer)
            SIGNED_INTEGER = 0x2,

            //! Unsigned integer value (see EXT_texture_integer)
            UNSIGNED_INTEGER = 0x4
        };
        
        /** Get the internal texture format type. */
        inline InternalFormatType getInternalFormatType() const { return m_internalFormatType; }
    
        /** Gets the dirty flag for the current contextID. */
        inline unsigned int& getTextureParameterDirty(unsigned int contextID) const
        {
            return m_texParametersDirtyList[contextID];
        }


        /** Force a reset on next apply() of associated OpenGL texture
          * parameters. */
        void dirtyTextureParameters();

        /** Force a manual allocation of the mipmap levels on the next apply() call.
          * User is responsible for filling the mipmap levels with valid data.
          * The OpenGL's glGenerateMipmapEXT function is used to generate the mipmap levels.
          * If glGenerateMipmapEXT is not supported or texture's internal format is not supported
          * by the glGenerateMipmapEXT, then empty mipmap levels will
          * be allocated manually. The mipmap levels are also allocated if a non-mipmapped
          * min filter is used. */
        void allocateMipmapLevels();


        /** Sets GL_TEXTURE_COMPARE_MODE_ARB to GL_COMPARE_R_TO_TEXTURE_ARB
          * See http://oss.sgi.com/projects/ogl-sample/registry/ARB/shadow.txt. */
        void setShadowComparison(bool flag) { m_use_shadow_comparison = flag; }

        enum ShadowCompareFunc {
            LEQUAL = GL_LEQUAL,
            GEQUAL = GL_GEQUAL
        };

        /** Sets shadow texture comparison function. */
        void setShadowCompareFunc(ShadowCompareFunc func) { m_shadow_compare_func = func; }
        ShadowCompareFunc getShadowCompareFunc() const { return m_shadow_compare_func; }

        enum ShadowTextureMode {
            LUMINANCE = GL_LUMINANCE,
            INTENSITY = GL_INTENSITY,
            ALPHA = GL_ALPHA
        };

        /** Sets shadow texture mode after comparison. */
        void setShadowTextureMode(ShadowTextureMode mode) { m_shadow_texture_mode = mode; }
        ShadowTextureMode getShadowTextureMode() const { return m_shadow_texture_mode; }

        /** Sets the TEXTURE_COMPARE_FAIL_VALUE_ARB texture parameter. See
          * http://oss.sgi.com/projects/ogl-sample/registry/ARB/shadow_ambient.txt. */
        void setShadowAmbient(float shadow_ambient) { m_shadow_ambient = shadow_ambient; }
        float getShadowAmbient() const { return m_shadow_ambient; }
        

		void setImageNameID(const std::string &imageNameID){ m_imageNameID = imageNameID; }
		const std::string &getImageNameID()const { return m_imageNameID; }

        /** Sets the texture image for the specified face. */
        virtual void setImage(unsigned int face, crImage* image) = 0;

        /** Gets the texture image for the specified face. */
        virtual crImage* getImage(unsigned int face) = 0;

        /** Gets the const texture image for specified face. */
        virtual const crImage* getImage(unsigned int face) const = 0;

        /** Gets the number of images that can be assigned to this crTexture. */
        virtual unsigned int getNumImages() const = 0;


        /** Set the PBuffer graphics context to read from when using PBuffers for RenderToTexture.*/
        void setReadPBuffer(crGraphicsContext* context) { m_readPBuffer = context; }

        /** Get the PBuffer graphics context to read from when using PBuffers for RenderToTexture.*/
        crGraphicsContext* getReadPBuffer() { return m_readPBuffer.get(); }

        /** Get the const PBuffer graphics context to read from when using PBuffers for RenderToTexture.*/
        const crGraphicsContext* getReadPBuffer() const { return m_readPBuffer.get(); }

        /** crTexture is a pure virtual base class, apply must be overridden. */
        virtual void apply(crState& state) const = 0;

        /** Calls apply(state) to compile the texture. */
        void compile(crState& state) const;

        ///** Resize any per context GLObject buffers to specified size. */
        //virtual void resizeGLObjectBuffers(unsigned int maxSize);

        /** If crState is non-zero, this function releases OpenGL objects for
          * the specified graphics context. Otherwise, releases OpenGL objects
          * for all graphics contexts. */
        virtual void releaseObjects(crState* state);
		
		void setNeedCompile(bool bln) { m_needCompile = bln; }
		bool getNeedCompile()const { return m_needCompile; }

		/** Set the minimum number of texture objects to retain in the deleted display list cache. */
		static void setMinimumNumberOfTextureObjectsToRetainInCache(unsigned int minimum);

		/** Get the minimum number of display lists to retain in the deleted display list cache. */
		static unsigned int getMinimumNumberOfTextureObjectsToRetainInCache();

		/** Set the handler for database requests.*/
		static void setImageDataRequestHandler(ImageDataRequestHandler* handler) { s_imageDataRequestHandler = handler; }

		/** Get the handler for database requests.*/
		static ImageDataRequestHandler* getImageDataRequestHandler() { return s_imageDataRequestHandler.get(); }

		inline bool getAlphaShadowEnable()const { return m_alphaShadowEnable; }
		void setAlphaShadowEnable(bool enableAlphaShadow);

		inline bool getBlendShadowEnable()const { return m_blendShadowEnable; }
		void setBlendShadowEnable(bool blendShadowEnable);

		inline bool getUseParallax() const { return m_useParallax; }
		void setUseParallax(bool useParallax);

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

                void setMultiTexturingSupported(bool flag) { m_isMultiTexturingSupported=flag; }
                bool isMultiTexturingSupported() const { return m_isMultiTexturingSupported; }

                void setTextureFilterAnisotropicSupported(bool flag) { m_isTextureFilterAnisotropicSupported=flag; }
                bool isTextureFilterAnisotropicSupported() const { return m_isTextureFilterAnisotropicSupported; }
                
                void setTextureCompressionARBSupported(bool flag) { m_isTextureCompressionARBSupported=flag; }
                bool isTextureCompressionARBSupported() const { return m_isTextureCompressionARBSupported; }

                void setTextureCompressionS3TCSupported(bool flag) { m_isTextureCompressionS3TCSupported=flag; }
                bool isTextureCompressionS3TCSupported() const { return m_isTextureCompressionS3TCSupported; }

                void setTextureMirroredRepeatSupported(bool flag) { m_isTextureMirroredRepeatSupported=flag; }
                bool isTextureMirroredRepeatSupported() const { return m_isTextureMirroredRepeatSupported; }

                void setTextureEdgeClampSupported(bool flag) { m_isTextureEdgeClampSupported=flag; }
                bool isTextureEdgeClampSupported() const { return m_isTextureEdgeClampSupported; }

                void setTextureBorderClampSupported(bool flag) { m_isTextureBorderClampSupported=flag; }
                bool isTextureBorderClampSupported() const { return m_isTextureBorderClampSupported; }

                void setGenerateMipMapSupported(bool flag) { m_isGenerateMipMapSupported=flag; }
                bool isGenerateMipMapSupported() const { return m_isGenerateMipMapSupported; }

                void setShadowSupported(bool flag) { m_isShadowSupported = flag; }
                bool isShadowSupported() const { return m_isShadowSupported; }

                void setShadowAmbientSupported(bool flag) { m_isShadowAmbientSupported = flag; }
                bool isShadowAmbientSupported() const { return m_isShadowAmbientSupported; }

				void setTextureMultisampledSupported(bool flag) { m_isTextureMultisampledSupported=flag; }
				bool isTextureMultisampledSupported() const { return m_isTextureMultisampledSupported; }

                void setMaxTextureSize(GLint maxsize) { m_maxTextureSize=maxsize; }
                GLint maxTextureSize() const { return m_maxTextureSize; }

                void setNumTextureUnits(GLint nunits ) { m_numTextureUnits=nunits; }
                GLint numTextureUnits() const { return m_numTextureUnits; }

                bool isCompressedTexImage2DSupported() const { return m_glCompressedTexImage2D!=0; }
                bool isCompressedTexSubImage2DSupported() const { return m_glCompressedTexSubImage2D!=0; }
                
                void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data) const;
                void glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei type, const GLvoid *data) const;
                void glGetCompressedTexImage(GLenum target, GLint level, GLvoid *data) const;

                bool isClientStorageSupported() const { return m_isClientStorageSupported; }

                bool isNonPowerOfTwoTextureSupported(GLenum filter) const
                {
                    return (filter==GL_LINEAR || filter==GL_NEAREST) ? 
                            m_isNonPowerOfTwoTextureNonMipMappedSupported :
                            m_isNonPowerOfTwoTextureMipMappedSupported;
                }

                void setTextureIntegerSupported(bool flag) { m_isTextureIntegerEXTSupported=flag; }
                bool isTextureIntegerSupported() const { return m_isTextureIntegerEXTSupported; }

                void glTexParameterIiv(GLenum target, GLenum pname, const GLint* data) const;
                void glTexParameterIuiv(GLenum target, GLenum pname, const GLuint* data) const;

				void glTexImage2DMultisample(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) const;
				static void clearExtensions();
            protected:

                ~Extensions() {}
                
                bool    m_isMultiTexturingSupported;
                bool    m_isTextureFilterAnisotropicSupported;
                bool    m_isTextureCompressionARBSupported;
                bool    m_isTextureCompressionS3TCSupported;
                bool    m_isTextureMirroredRepeatSupported;
                bool    m_isTextureEdgeClampSupported;
                bool    m_isTextureBorderClampSupported;
                bool    m_isGenerateMipMapSupported;
                bool    m_isShadowSupported;
                bool    m_isShadowAmbientSupported;
                bool    m_isClientStorageSupported;
                bool    m_isNonPowerOfTwoTextureMipMappedSupported;
                bool    m_isNonPowerOfTwoTextureNonMipMappedSupported;
                bool    m_isTextureIntegerEXTSupported;
				bool    m_isTextureMultisampledSupported;

                GLint   m_maxTextureSize;
                GLint   m_numTextureUnits;

                typedef void (APIENTRY * CompressedTexImage2DArbProc) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
                typedef void (APIENTRY * CompressedTexSubImage2DArbProc) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
                typedef void (APIENTRY * GetCompressedTexImageArbProc) (GLenum target, GLint level, GLvoid *data);
                typedef void (APIENTRY * TexParameterIivProc)(GLenum target, GLenum pname, const GLint* data);
                typedef void (APIENTRY * TexParameterIuivProc)(GLenum target, GLenum pname, const GLuint* data);
				typedef void (APIENTRY * TexImage2DMultisample)(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);

                CompressedTexImage2DArbProc     m_glCompressedTexImage2D;
                CompressedTexSubImage2DArbProc  m_glCompressedTexSubImage2D;
                GetCompressedTexImageArbProc    m_glGetCompressedTexImage;
                TexParameterIivProc             m_glTexParameterIiv;
                TexParameterIuivProc            m_glTexParameterIuiv;
				TexImage2DMultisample           m_glTexImage2DMultisample;
        };
        
        /** Gets the extension for the specified context. Creates the
          * Extensions object for that context if it doesn't exist.
          * Returns NULL if the Extensions object for the context doesn't
          * exist and the createIfNotInitalized flag is false. */
        static Extensions* getExtensions(unsigned int contextID,bool createIfNotInitalized);

        /** Overrides Extensions objects across graphics contexts. Typically
          * used to ensure the same lowest common denominator of extensions
          * on systems with different graphics pipes. */
        static void setExtensions(unsigned int contextID,Extensions* extensions);

        /** Determine whether the given internalFormat is a compressed
          * image format. */
        static bool isCompressedInternalFormat(GLint internalFormat);
        
        /** Determine the size of a compressed image, given the internalFormat,
          * the width, the height, and the depth of the image. The block size
          * and the size are output parameters. */
        static void getCompressedSize(GLenum internalFormat, GLint width, GLint height, GLint depth, GLint& blockSize, GLint& size);


        /** Helper method. Creates the texture, but doesn't set or use a
          * texture binding. Note: Don't call this method directly unless
          * you're implementing a subload callback. */
        void applyTexImage2D_load(crState& state, GLenum target, const crImage* image, GLsizei width, GLsizei height,GLsizei numMipmapLevels) const;
        
        /** Helper method. Subloads images into the texture, but doesn't set
          * or use a texture binding. Note: Don't call this method directly
          * unless you're implementing a subload callback. */
        void applyTexImage2D_subload(crState& state, GLenum target, const crImage* image, GLsizei& width, GLsizei& height, GLint& inInternalFormat, GLsizei& numMipmapLevels) const;

        /** Returned by mipmapBeforeTexImage() to indicate what
          * mipmapAfterTexImage() should do */
        enum GenerateMipmapMode
        {
            GENERATE_MIPMAP_NONE,
            GENERATE_MIPMAP,
            GENERATE_MIPMAP_TEX_PARAMETER
        };

		enum ExternTextureID
		{
			EXT_NONE,
			UVSCRAMBLER,
		    COLORWEIGHT,
			LIGHTMAP,
			ENVMAP,
		};
		void setExternTextureID(ExternTextureID id);
		inline ExternTextureID getExternTextureID()const { return m_externTextureID; }
    protected :

        virtual ~crTexture();

        virtual void computeInternalFormat() const = 0;
        
        void computeInternalFormatWithImage(const CRCore::crImage& image) const;

        void computeRequiredTextureDimensions(crState& state, const CRCore::crImage& image,GLsizei& width, GLsizei& height,GLsizei& numMipmapLevels) const;
		void computeRequiredTextureDimensions(crState& state, GLsizei& inwidth, GLsizei& inheight,GLsizei& numMipmapLevels ) const;

        void computeInternalFormatType() const;

        /** Helper method. Sets texture parameters. */
        void applyTexParameters(GLenum target, crState& state) const;

        /** Returns true if m_useHardwareMipMapGeneration is true and either
          * glGenerateMipmapEXT() or GL_GENERATE_MIPMAP_SGIS are supported. */
        bool isHardwareMipmapGenerationEnabled(const crState& state) const;

        /** Helper methods to be called before and after calling
          * gl[Compressed][Copy]Tex[Sub]Image2D to handle generating mipmaps. */
        GenerateMipmapMode mipmapBeforeTexImage(const crState& state, bool hardwareMipmapOn) const;
        void mipmapAfterTexImage(crState& state, GenerateMipmapMode beforeResult) const;

        /** Helper method to generate mipmap levels by calling of glGenerateMipmapEXT.
          * If it is not supported, then call the virtual allocateMipmap() method */
        void generateMipmap(crState& state) const;

        /** Allocate mipmap levels of the texture by subsequent calling of glTexImage* function. */
        virtual void allocateMipmap(crState& state) const = 0;
        
        /** Returns -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs. */
        int compareTexture(const crTexture& rhs) const;
		int compareTexture(const crTexture& rhs);

        /** Returns -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs. */
        int compareTextureObjects(const crTexture& rhs) const;
		int compareTextureObjects(const crTexture& rhs);

        typedef buffered_value<unsigned int> TexParameterDirtyList;
        mutable TexParameterDirtyList m_texParametersDirtyList;
        mutable TexParameterDirtyList m_texMipmapGenerationDirtyList;
        
        WrapMode m_wrap_s;
        WrapMode m_wrap_t;
        WrapMode m_wrap_r;

        FilterMode      m_min_filter;
        FilterMode      m_mag_filter;
        float           m_maxAnisotropy;
        bool            m_useHardwareMipMapGeneration;
        bool            m_unrefImageDataAfterApply;
        bool            m_clientStorageHint;
        bool            m_resizeNonPowerOfTwoHint;

        crVector4d           m_borderColor;
        GLint           m_borderWidth;

        InternalFormatMode          m_internalFormatMode;
        mutable InternalFormatType  m_internalFormatType;
        mutable GLint       m_internalFormat;
        mutable GLenum      m_sourceFormat;
        mutable GLenum      m_sourceType;
      
        bool                m_use_shadow_comparison;
        ShadowCompareFunc   m_shadow_compare_func;
        ShadowTextureMode   m_shadow_texture_mode;
        float               m_shadow_ambient;

		mutable bool                        m_needCompile;

		static ref_ptr<ImageDataRequestHandler> s_imageDataRequestHandler;

		std::string m_imageNameID;

		bool m_blendShadowEnable;
		bool m_alphaShadowEnable;
		bool m_useParallax;
		ExternTextureID m_externTextureID;
    public:
    
		class CR_EXPORT TextureObject : public CRCore::Referenced
		{
		public:

			inline TextureObject(unsigned int    id, unsigned int target):
			    m_id(id),
				m_target(target),
				m_numMipmapLevels(0),
				m_internalFormat(0),
				m_width(0),
				m_height(0),
				m_depth(0),
				m_border(0),
				m_allocated(false),
				m_timeStamp(0),
				m_stateinfo(0){}

				inline TextureObject(unsigned int    id,
					unsigned int    target,
					int     numMipmapLevels,
					unsigned int    internalFormat,
					int   width,
					int   height,
					int   depth,
					int     border):
				m_id(id),
					m_target(target),
					m_numMipmapLevels(numMipmapLevels),
					m_internalFormat(internalFormat),
					m_width(width),
					m_height(height),
					m_depth(depth),
					m_border(border),
					m_allocated(false),
					m_timeStamp(0),
					m_stateinfo(0){}

					inline bool match(unsigned int    target,
						int     numMipmapLevels,
						unsigned int    internalFormat,
						int   width,
						int   height,
						int   depth,
						int     border)
					{
						return isReusable() &&
							(m_target == target) &&
							(m_numMipmapLevels == numMipmapLevels) &&
							(m_internalFormat == internalFormat) &&
							(m_width == width) &&
							(m_height == height) &&
							(m_depth == depth) &&
							(m_border == border);
					}

					inline void bind(double timeStamp);

					inline void setAllocated(bool allocated=true) { m_allocated = allocated; }

					inline void setAllocated(int     numMipmapLevels,
						unsigned int    internalFormat,
						int   width,
						int   height,
						int   depth,
						int     border)
					{
						m_allocated=true;
						m_numMipmapLevels = numMipmapLevels;
						m_internalFormat = internalFormat;
						m_width = width;
						m_height = height;
						m_depth = depth;
						m_border = border;
					}

					inline bool isAllocated() const { return m_allocated; }

					inline bool isReusable() const { return m_allocated && m_width!=0 && m_stateinfo!=2; }

					inline void setTimeStamp(double timeStamp) { m_timeStamp = timeStamp; }

					inline void setStateInfo(int stateinfo) { m_stateinfo = stateinfo; }
					inline int getStateInfo() { return m_stateinfo; }

					unsigned int      m_id;
					unsigned int      m_target;
					int       m_numMipmapLevels;
					unsigned int      m_internalFormat;
					int       m_width;
					int       m_height;
					int       m_depth;
					int       m_border;

					bool      m_allocated;
					double    m_timeStamp;
					int       m_stateinfo;//0:正常，可使用 1：存在删除队列里 2：已经被删除
		};

		typedef std::set< CRCore::ref_ptr<TextureObject> > TextureObjectList;
		typedef std::map<unsigned int, TextureObjectList > TextureObjectListMap; 

		/** take the active texture objects from the CRCore::crTexture and place them in the specified TextureObjectListMap.*/
		void takeTextureObjects(TextureObjectListMap& toblm);

		static TextureObject* generateTextureObject(unsigned int contextID,GLenum target);

		static TextureObject* generateTextureObject(unsigned int contextID,
			GLenum    target,
			GLint     numMipmapLevels,
			GLenum    internalFormat,
			GLsizei   width,
			GLsizei   height,
			GLsizei   depth,
			GLint     border);

		static void flushAllDeletedTextureObjects(unsigned int contextID);

		static void flushDeletedTextureObjects(unsigned int contextID,double currentTime, double& availableTime);

		/** Returns a pointer to the texture object for the current context. */
		inline TextureObject* getTextureObject(unsigned int contextID) const
		{
			return m_textureObjectBuffer[contextID].get();
		}
		/** Forces a recompile on next apply() of associated OpenGL texture
		* objects. */
		void dirtyTextureObject();

		/** Returns true if the texture objects for all the required graphics
		* contexts are loaded. */
		bool areAllTextureObjectsLoaded() const;
		bool reuseTextureObject(unsigned int contextID, TextureObject *textureObject)const;
    protected:

        typedef buffered_ptrobject< ref_ptr<TextureObject> >  TextureObjectBuffer;
        mutable TextureObjectBuffer         m_textureObjectBuffer;
        mutable ref_ptr<crGraphicsContext>    m_readPBuffer;
};

class CR_EXPORT crTextureObjectManager : public CRCore::Referenced
{
public:

	crTextureObjectManager():
	  m_expiryDelay(10.0) {}
	  static crTextureObjectManager *getInstance();

	  virtual crTexture::TextureObject* generateTextureObject(unsigned int contextID,GLenum target);

	  virtual crTexture::TextureObject* generateTextureObject(unsigned int contextID,
		  GLenum    target,
		  GLint     numMipmapLevels,
		  GLenum    internalFormat,
		  GLsizei   width,
		  GLsizei   height,
		  GLsizei   depth,
		  GLint     border);

	  virtual crTexture::TextureObject* reuseTextureObject(unsigned int contextID,
		  GLenum    target,
		  GLint     numMipmapLevels,
		  GLenum    internalFormat,
		  GLsizei   width,
		  GLsizei   height,
		  GLsizei   depth,
		  GLint     border);

	  inline crTexture::TextureObject* reuseOrGenerateTextureObject(unsigned int contextID,
		  GLenum    target,
		  GLint     numMipmapLevels,
		  GLenum    internalFormat,
		  GLsizei   width,
		  GLsizei   height,
		  GLsizei   depth,
		  GLint     border)
	  {
		  /*crTexture::TextureObject* to = reuseTextureObject(contextID,target,numMipmapLevels,internalFormat,width,height,depth,border);
		  if (to) return to;
		  else */return generateTextureObject(contextID,target,numMipmapLevels,internalFormat,width,height,depth,border);
	  }   

	  bool reuseTextureObject(unsigned int contextID, crTexture::TextureObject *to);

	  void addTextureObjects(crTexture::TextureObjectListMap& toblm);

	  void addTextureObjectsFrom(crTexture& texture);

	  void flushAllTextureObjects(unsigned int contextID);

	  void flushTextureObjects(unsigned int contextID,double currentTime, double& availableTime);

	  void setExpiryDelay(double expiryDelay) { m_expiryDelay = expiryDelay; }

	  double getExpiryDelay() const { return m_expiryDelay; }

	  /** How long to keep unused texture objects before deletion. */
	  double                  m_expiryDelay;

	  crTexture::TextureObjectListMap    m_textureObjectListMap;

	  // mutex to keep access serialized.
	  crMutex      m_mutex;
	  //static int s_number;
	  //int m_number;
	  virtual void clear();
protected :
	virtual ~crTextureObjectManager() {}
	static CRCore::ref_ptr<crTextureObjectManager> m_instance;
};

}

#endif
