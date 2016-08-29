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
#ifndef CRCORE_CRIMAGE_H
#define CRCORE_CRIMAGE_H 1

#include <CRCore/crBase.h>
#include <CRCore/crBufferObject.h>
#include <CRCore/crFrameStamp.h>
#include <CRCore/crVector2.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crStateAttribute.h>

#include <string>
#include <vector>

#ifndef GL_VERSION_1_2
    // 1.2 definitions...
    #define GL_BGR                          0x80E0
    #define GL_BGRA                         0x80E1
    #define GL_UNSIGNED_BYTE_3_3_2          0x8032
    #define GL_UNSIGNED_BYTE_2_3_3_REV      0x8362
    #define GL_UNSIGNED_SHORT_5_6_5         0x8363
    #define GL_UNSIGNED_SHORT_5_6_5_REV     0x8364
    #define GL_UNSIGNED_SHORT_4_4_4_4       0x8033
    #define GL_UNSIGNED_SHORT_4_4_4_4_REV   0x8365
    #define GL_UNSIGNED_SHORT_5_5_5_1       0x8034
    #define GL_UNSIGNED_SHORT_1_5_5_5_REV   0x8366
    #define GL_UNSIGNED_INT_8_8_8_8         0x8035
    #define GL_UNSIGNED_INT_8_8_8_8_REV     0x8367
    #define GL_UNSIGNED_INT_10_10_10_2      0x8036
    #define GL_UNSIGNED_INT_2_10_10_10_REV  0x8368
#endif

#ifndef GL_COMPRESSED_ALPHA
    #define GL_COMPRESSED_ALPHA             0x84E9
    #define GL_COMPRESSED_LUMINANCE         0x84EA
    #define GL_COMPRESSED_LUMINANCE_ALPHA   0x84EB
    #define GL_COMPRESSED_INTENSITY         0x84EC
    #define GL_COMPRESSED_RGB               0x84ED
    #define GL_COMPRESSED_RGBA              0x84EE
#endif


#ifndef GL_ABGR_EXT
#define GL_ABGR_EXT                         0x8000
#endif

#if defined(GLES1_AVAILABLE) || defined(GLES2_AVAILABLE)
    #define GL_RED                  0x1903
    #define GL_GREEN                0x1904
    #define GL_BLUE                 0x1905
    #define GL_DEPTH_COMPONENT      0x1902
    #define GL_STENCIL_INDEX        0x1901
#endif

#if defined(GLES1_AVAILABLE) || defined(GLES2_AVAILABLE) || defined(GL3_AVAILABLE)
    #define GL_BITMAP               0x1A00
    #define GL_COLOR_INDEX          0x1900
    #define GL_INTENSITY12          0x804C
    #define GL_INTENSITY16          0x804D
    #define GL_INTENSITY4           0x804A
    #define GL_INTENSITY8           0x804B
    #define GL_LUMINANCE12          0x8041
    #define GL_LUMINANCE12_ALPHA4   0x8046
    #define GL_LUMINANCE12_ALPHA12  0x8047
    #define GL_LUMINANCE16          0x8042
    #define GL_LUMINANCE16_ALPHA16  0x8048
    #define GL_LUMINANCE4           0x803F
    #define GL_LUMINANCE4_ALPHA4    0x8043
    #define GL_LUMINANCE6_ALPHA2    0x8044
    #define GL_LUMINANCE8           0x8040
    #define GL_LUMINANCE8_ALPHA8    0x8045
    #define GL_RGBA8                0x8058
    #define GL_PACK_ROW_LENGTH      0x0D02
#endif

#ifndef GL_PACK_SKIP_IMAGES
    #define GL_PACK_SKIP_IMAGES     0x806B
    #define GL_PACK_IMAGE_HEIGHT    0x806C
    #define GL_UNPACK_SKIP_IMAGES   0x806D
    #define GL_UNPACK_IMAGE_HEIGHT  0x806E
#endif

namespace CRCore {

// forward declare
class crNodeVisitor;

/** crImage class for encapsulating the storage texture image data. */
class CR_EXPORT crImage : public crBase
{

    public :

        crImage();
        
        /** Copy constructor using CopyOp to manage deep vs shallow copy. */
        crImage(const crImage& image,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

		virtual crBase* cloneType() const { return new crImage(); }
		virtual crBase* clone(const crCopyOp& copyop) const { return new crImage(*this,copyop); }
		virtual bool isSameKindAs(const crBase* obj) const { return dynamic_cast<const crImage*>(obj)!=0; }
		virtual const char* libraryName() const { return "CRCore"; }
		virtual const char* className() const { return "crImage"; }

        virtual const GLvoid*   getDataPointer() const { return data(); }
        virtual unsigned int    getTotalDataSize() const { return getTotalSizeInBytesIncludingMipmaps(); }

        /** Return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs. */
        virtual int compare(const crImage& rhs) const;

        void setFileName(const std::string& fileName);
        inline const std::string& getFileName() const { return m_fileName; }
        
        enum WriteHint {
            NO_PREFERENCE,
            STORE_INLINE,
            EXTERNAL_FILE
        };
        
        void setWriteHint(WriteHint writeHint) { m_writeHint = writeHint; }
        WriteHint getWriteHint() const { return m_writeHint; }
        
        enum AllocationMode {
            NO_DELETE,
            USE_NEW_DELETE,
            USE_MALLOC_FREE
        };
        
        /** Set the method used for deleting data once it goes out of scope. */
        void setAllocationMode(AllocationMode mode) { m_allocationMode = mode; }

        /** Get the method used for deleting data once it goes out of scope. */
        AllocationMode getAllocationMode() const { return m_allocationMode; }


        /** Allocate a pixel block of specified size and type. */
        virtual void allocateImage(int s,int t,int r,
                           GLenum pixelFormat,GLenum type,
                           int packing=1);
        
        
        /** Set the image dimensions, format and data. */
        virtual void setImage(int s,int t,int r,
                      GLint internalTextureformat,
                      GLenum pixelFormat,GLenum type,
                      unsigned char* data,
                      AllocationMode mode,
                      int packing=1);
            
        /** Read pixels from current frame buffer at specified position and size, using glReadPixels.
          * Create memory for storage if required, reuse existing pixel coords if possible.
        */
        virtual void readPixels(int x,int y,int width,int height,
                        GLenum pixelFormat,GLenum type);
            

        /** Read the contents of the current bound texture, handling compressed pixelFormats if present.
          * Create memory for storage if required, reuse existing pixel coords if possible.
        */
        virtual void readImageFromCurrentTexture(unsigned int contextID, bool copyMipMapsIfAvailable, GLenum type = GL_UNSIGNED_BYTE);


        /** Scale image to specified size. */
        void scaleImage(int s,int t,int r) { scaleImage(s,t,r, getDataType()); }

        /** Scale image to specified size and with specified data type. */
        virtual void scaleImage(int s,int t,int r, GLenum newDataType);

        /** Copy a source crImage into a subpart of this crImage at specified position.
          * Typically used to copy to an already allocated image, such as creating
          * a 3D image from a stack 2D images.
          * If this crImage is empty then image data is created to
          * accomodate the source image in its offset position.
          * If source is NULL then no operation happens, this crImage is left unchanged.
        */
        virtual void copySubImage(int s_offset, int t_offset, int r_offset, const CRCore::crImage* source);


        enum Origin 
        {
            BOTTOM_LEFT,
            TOP_LEFT
        };
        
        /** Set the origin of the image.
          * The default value is BOTTOM_LEFT and is consistent with OpenGL.
          * TOP_LEFT is used for imagery that follows standard Imagery convention, such as movies,
          * and hasn't been flipped yet.  For such images one much flip the t axis of the tex coords.
          * to handle this origin position. */
        void setOrigin(Origin origin) { m_origin = origin; }
        
        /** Get the origin of the image.*/
        Origin getOrigin() const { return m_origin; }
        

        /** Width of image. */
        inline int s() const { return m_s; }

        /** Height of image. */
        inline int t() const { return m_t; }
        
        /** Depth of image. */
        inline int r() const { return m_r; }
        
        void setInternalTextureFormat(GLint internalFormat);
        inline GLint getInternalTextureFormat() const { return m_internalTextureFormat; }
        
        void setPixelFormat(GLenum pixelFormat);
        inline GLenum getPixelFormat() const { return m_pixelFormat; }
        
        void setDataType(GLenum dataType);
        inline GLenum getDataType() const { return m_dataType; }        
        
        void setPacking(unsigned int packing) { m_packing = packing; }
        inline unsigned int getPacking() const { return m_packing; }

        /** return true of the pixel format is an OpenGL compressed pixel format.*/
        bool isCompressed() const;

        /** Set the pixel aspect ratio, defined as the pixel width divided by the pixel height.*/
        inline void setPixelAspectRatio(float pixelAspectRatio) { m_pixelAspectRatio = pixelAspectRatio; }

        /** Get the pixel aspect ratio.*/
        inline float getPixelAspectRatio() const { return m_pixelAspectRatio; }
        
        /** Return the number of bits required for each pixel. */
        inline unsigned int getPixelSizeInBits() const { return computePixelSizeInBits(m_pixelFormat,m_dataType); }

        /** Return the number of bytes each row of pixels occupies once it has been packed. */
        inline unsigned int getRowSizeInBytes() const { return computeRowWidthInBytes(m_s,m_pixelFormat,m_dataType,m_packing); }

        /** Return the number of bytes each image (m_s*m_t) of pixels occupies. */
        inline unsigned int getImageSizeInBytes() const { return getRowSizeInBytes()*m_t; }
        
        /** Return the number of bytes the whole row/image/volume of pixels occupies. */
        inline unsigned int getTotalSizeInBytes() const { return getImageSizeInBytes()*m_r; }

        /** Return the number of bytes the whole row/image/volume of pixels occupies, including all mip maps if included. */
        unsigned int getTotalSizeInBytesIncludingMipmaps() const;

        /** Return true if the crImage represent a valid and usable imagery.*/
        bool valid() const { return m_s!=0 && m_t!=0 && m_r!=0 && m_data!=0 && m_dataType!=0; }

        /** Raw image data. */
        inline unsigned char* data() { return m_data; }
        
        /** Raw const image data. */
        inline const unsigned char* data() const { return m_data; }


        inline unsigned char* data(int column, int row=0,int image=0)
        {
            if (!m_data) return NULL;
            return m_data+(column*getPixelSizeInBits())/8+row*getRowSizeInBytes()+image*getImageSizeInBytes();
        }
        
        inline const unsigned char* data(int column, int row=0,int image=0) const
        {
            if (!m_data) return NULL;
            return m_data+(column*getPixelSizeInBits())/8+row*getRowSizeInBytes()+image*getImageSizeInBytes();
        }

        /** Get the color value for specified texcoord.*/
        crVector4 getColor(unsigned int s,unsigned t=0,unsigned r=0) const;

        /** Get the color value for specified texcoord.*/
        crVector4 getColor(const crVector2& texcoord) const { return getColor(crVector3(texcoord.x(),texcoord.y(),0.0f)); }

        /** Get the color value for specified texcoord.*/
        crVector4 getColor(const crVector3& texcoord) const;


        /** Flip the image horizontally. */
        void flipHorizontal();
        
        /** Flip the image vertically. */
        void flipVertical();


        /** Ensure image dimensions are a power of two.
          * Mipmapped textures require the image dimensions to be
          * power of two and are within the maxiumum texture size for
          * the host machine.
        */
        void ensureValidSizeForTexturing(GLint maxTextureSize);

		/** Dirty the image, which increments the modified count, to force CRCore::Texture to reload the image. */
		inline void dirty() { ++m_modifiedCount; }      

		/** Set the modified count value. Used by CRCore::Texture when using texture subloading. */
		inline void setModifiedCount(unsigned int value) { m_modifiedCount=value; }

		/** Get modified count value. Used by CRCore::Texture when using texture subloading. */
		inline unsigned int getModifiedCount() const { return m_modifiedCount; }

        static bool isPackedType(GLenum type);
        static GLenum computePixelFormat(GLenum pixelFormat);
        static GLenum computeFormatDataType(GLenum pixelFormat);
        static unsigned int computeNumComponents(GLenum pixelFormat);
        static unsigned int computePixelSizeInBits(GLenum pixelFormat,GLenum type);
        static unsigned int computeRowWidthInBytes(int width,GLenum pixelFormat,GLenum type,int packing);
        static int computeNearestPowerOfTwo(int s,float bias=0.5f);
        static int computeNumberOfMipmapLevels(int s,int t = 1, int r = 1);

		unsigned int getNumComponents();

        /** Precomputed mipmaps stuff. */
        typedef std::vector< unsigned int > MipmapDataType;

        inline bool isMipmap() const {return !m_mipmapData.empty();};

        unsigned int getNumMipmapLevels() const
        {
            return static_cast<unsigned int>(m_mipmapData.size())+1;
        };

        /** Send offsets into data. It is assumed that first mipmap offset (index 0) is 0.*/
        inline void setMipmapLevels(const MipmapDataType& mipmapDataVector) { m_mipmapData = mipmapDataVector; }
        
        inline const MipmapDataType& getMipmapLevels() const { return m_mipmapData; }

        inline unsigned int getMipmapOffset(unsigned int mipmapLevel) const
        {
            if(mipmapLevel == 0)
                return 0;
            else if (mipmapLevel < getNumMipmapLevels())
               return m_mipmapData[mipmapLevel-1];
            return 0;
        };
        
        inline unsigned char* getMipmapData(unsigned int mipmapLevel)
        {
           return m_data+getMipmapOffset(mipmapLevel);
        }

        inline const unsigned char* getMipmapData(unsigned int mipmapLevel) const
        {
           return m_data+getMipmapOffset(mipmapLevel);
        }

        /*inline const unsigned char* getMipmapData(unsigned int row, unsigned int column, unsigned int mipmapLevel) const
        {
           if (!m_data) return NULL;
           return getMipmapData(mipmapLevel) + (column*getPixelSizeInBits())/8+row*getRowSizeInBytes();
        }*/

        /** Return true if this image is translucent - i.e. it has alpha values that are less 1.0 (when normalized). */
        virtual bool isImageTranslucent() const;

        /** Set the optional crPixelBufferObject used to map the image memory efficiently to graphics memory. */ 
        void setPixelBufferObject(crPixelBufferObject* buffer) { m_bufferObject = buffer; if (m_bufferObject.valid()) m_bufferObject->setImage(this); }

        /** Get the crPixelBufferObject.*/
        crPixelBufferObject* getPixelBufferObject() { return dynamic_cast<crPixelBufferObject*>(m_bufferObject.get()); }

        /** Get the const crPixelBufferObject.*/
        const crPixelBufferObject* getPixelBufferObject() const { return dynamic_cast<const crPixelBufferObject*>(m_bufferObject.get()); }

        /** return whether the update(crNodeVisitor* nv) should be required on each frame to enable proper working of CRCore::crImage.*/
        virtual bool requiresUpdateCall() const { return false; }

        /** update method for CRCore::crImage subclasses that update themselves during the update traversal.*/
        virtual void update(crNodeVisitor* /*nv*/) {}

		struct CR_EXPORT UpdateCallback : public CRCore::crStateAttribute::Callback
		{
			virtual void operator () (CRCore::crStateAttribute* attr, CRCore::crNodeVisitor* nv);
		};

        /** method for hinting whether to enable or disable focus to images acting as front ends to interactive surfaces such as a vnc or browser window.  Return true if handled. */
        virtual bool sendFocusHint(bool /*focus*/) { return false; }

        /** method for sending pointer events to images that are acting as front ends to interactive surfaces such as a vnc or browser window.  Return true if handled. */
        virtual bool sendPointerEvent(int /*x*/, int /*y*/, int /*buttonMask*/) { return false; }

        /** method for sending key events to images that are acting as front ends to interactive surfaces such as a vnc or browser window.  Return true if handled.*/
        virtual bool sendKeyEvent(int /*key*/, bool /*keyDown*/) { return false; }

        /** method for passing frame information to the custom crImage classes, to be called only when objects associated with imagery are not culled.*/
        virtual void setFrameLastRendered(const CRCore::crFrameStamp* /*frameStamp*/) {}

    protected :

        virtual ~crImage();

        crImage& operator = (const crImage&) { return *this; }

        std::string m_fileName;
        WriteHint   m_writeHint;


        Origin m_origin;

        int m_s, m_t, m_r;
        GLint m_internalTextureFormat;
        GLenum m_pixelFormat;
        GLenum m_dataType;
        unsigned int m_packing;
        float m_pixelAspectRatio;

        AllocationMode m_allocationMode;
        unsigned char* m_data;
        
        void deallocateData();
        
        void setData(unsigned char* data,AllocationMode allocationMode);

		unsigned int m_modifiedCount;

        MipmapDataType m_mipmapData;
        
        ref_ptr<crPixelBufferObject> m_bufferObject;
};

//class crObject;
//CRRender::createObjectForImage   crOglGeometryDraw.h
//extern CR_EXPORT crObject* createObjectForImage(crImage* image);
//extern CR_EXPORT crObject* createObjectForImage(crImage* image,float s,float t);

}

#endif                                            // __SG_IMAGE_H
