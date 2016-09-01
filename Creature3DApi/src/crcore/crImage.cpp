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
#include <crgl/gl.h>
#include <crgl/glu.h>

#include <CRCore/crImage.h>
#include <CRCore/crNotify.h>
//#include <CRCore/io_utils.h>

#include <CRCore/crObject.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crTexture1D.h>
#include <CRCore/crTexture2D.h>
#include <CRCore/crTexture3D.h>
#include <CRCore/crTexture2DArray.h>
#include <CRCore/crLight.h>

#include <string.h>
#include <stdlib.h>

#include <CRCore/dxtctool.h>

using namespace CRCore;
using namespace std;

void crImage::UpdateCallback::operator () (CRCore::crStateAttribute* attr, CRCore::crNodeVisitor* nv)
{
	CRCore::crTexture* texture = attr ? dynamic_cast<crTexture *>(attr) : 0;

	// CRCore::notify(CRCore::NOTICE)<<"crImage::UpdateCallback::"<<texture<<std::endl;
	if (texture)
	{
		for(unsigned int i=0; i<texture->getNumImages(); ++i)
		{
			texture->getImage(i)->update(nv);
		}
	}
}

crImage::crImage()
:crBase(),
m_fileName(""),
m_writeHint(NO_PREFERENCE),
m_origin(BOTTOM_LEFT),
m_s(0), m_t(0), m_r(0),
m_internalTextureFormat(0),
m_pixelFormat(0),
m_dataType(0),
m_packing(4),
m_pixelAspectRatio(1.0),
m_allocationMode(USE_NEW_DELETE),
m_data(0L)
{
    setDataVariance(STATIC); 
}

crImage::crImage(const crImage& image,const crCopyOp& copyop):
    crBase(image,copyop),
    m_fileName(image.m_fileName),
    m_writeHint(image.m_writeHint),
    m_origin(image.m_origin),
    m_s(image.m_s), m_t(image.m_t), m_r(image.m_r),
    m_internalTextureFormat(image.m_internalTextureFormat),
    m_pixelFormat(image.m_pixelFormat),
    m_dataType(image.m_dataType),
    m_packing(image.m_packing),
    m_pixelAspectRatio(image.m_pixelAspectRatio),
    m_allocationMode(USE_NEW_DELETE),
    m_data(0L),
    m_mipmapData(image.m_mipmapData)
{
    if (image.m_data)
    {
        int size = image.getTotalSizeInBytesIncludingMipmaps();
        setData(new unsigned char [size],USE_NEW_DELETE);
        memcpy(m_data,image.m_data,size);
    }
}

crImage::~crImage()
{
    deallocateData();
}

void crImage::deallocateData()
{
    if (m_data) {
        if (m_allocationMode==USE_NEW_DELETE) delete [] m_data;
        else if (m_allocationMode==USE_MALLOC_FREE) ::free(m_data);
        m_data = 0;
    }
}

int crImage::compare(const crImage& rhs) const
{
    // if at least one filename is empty, then need to test buffer
    // pointers because images could have been created on the fly
    // and therefore we can't rely on file names to get an accurate
    // comparison
    if (getFileName().empty() || rhs.getFileName().empty())
    {
        if (m_data<rhs.m_data) return -1;
        if (m_data>rhs.m_data) return 1;
    }

    // need to test against image contents here...
    COMPARE_StateAttribute_Parameter(m_s)
    COMPARE_StateAttribute_Parameter(m_t)
    COMPARE_StateAttribute_Parameter(m_internalTextureFormat)
    COMPARE_StateAttribute_Parameter(m_pixelFormat)
    COMPARE_StateAttribute_Parameter(m_dataType)
    COMPARE_StateAttribute_Parameter(m_packing)
    COMPARE_StateAttribute_Parameter(m_mipmapData)
    COMPARE_StateAttribute_Parameter(m_modifiedCount)

    // same buffer + same parameters = same image
    if ((m_data || rhs.m_data) && (m_data == rhs.m_data)) return 0;

    // slowest comparison at the bottom!
    COMPARE_StateAttribute_Parameter(getFileName())    

    return 0;
}

void crImage::setFileName(const std::string& fileName)
{
    m_fileName = fileName;
}

void crImage::setData(unsigned char* data, AllocationMode mode)
{
    deallocateData();
    m_data = data;
    m_allocationMode = mode;
}


bool crImage::isPackedType(GLenum type)
{
    switch(type)
    {
        case(GL_UNSIGNED_BYTE_3_3_2):
        case(GL_UNSIGNED_BYTE_2_3_3_REV):
        case(GL_UNSIGNED_SHORT_5_6_5):
        case(GL_UNSIGNED_SHORT_5_6_5_REV):
        case(GL_UNSIGNED_SHORT_4_4_4_4):
        case(GL_UNSIGNED_SHORT_4_4_4_4_REV):
        case(GL_UNSIGNED_SHORT_5_5_5_1):
        case(GL_UNSIGNED_SHORT_1_5_5_5_REV):
        case(GL_UNSIGNED_INT_8_8_8_8):
        case(GL_UNSIGNED_INT_8_8_8_8_REV):
        case(GL_UNSIGNED_INT_10_10_10_2):
        case(GL_UNSIGNED_INT_2_10_10_10_REV): return true;
        default: return false;
    }    
}


GLenum crImage::computePixelFormat(GLenum format)
{
    switch(format)
    {
        case(GL_ALPHA16F_ARB):
        case(GL_ALPHA32F_ARB):
            return GL_ALPHA;
        case(GL_LUMINANCE16F_ARB):
        case(GL_LUMINANCE32F_ARB):
            return GL_LUMINANCE;
        case(GL_INTENSITY16F_ARB):
        case(GL_INTENSITY32F_ARB):
            return GL_INTENSITY;
        case(GL_LUMINANCE_ALPHA16F_ARB):
        case(GL_LUMINANCE_ALPHA32F_ARB):
            return GL_LUMINANCE_ALPHA;
        case(GL_RGB32F_ARB):
        case(GL_RGB16F_ARB):
            return GL_RGB;
        case(GL_RGBA32F_ARB):
        case(GL_RGBA16F_ARB):
            return GL_RGBA;

        case(GL_ALPHA8I_EXT):
        case(GL_ALPHA16I_EXT):
        case(GL_ALPHA32I_EXT):
        case(GL_ALPHA8UI_EXT):
        case(GL_ALPHA16UI_EXT):
        case(GL_ALPHA32UI_EXT):
            return GL_ALPHA_INTEGER_EXT;
        case(GL_LUMINANCE8I_EXT):
        case(GL_LUMINANCE16I_EXT):
        case(GL_LUMINANCE32I_EXT):
        case(GL_LUMINANCE8UI_EXT):
        case(GL_LUMINANCE16UI_EXT):
        case(GL_LUMINANCE32UI_EXT):
            return GL_LUMINANCE_INTEGER_EXT;
        case(GL_INTENSITY8I_EXT):
        case(GL_INTENSITY16I_EXT):
        case(GL_INTENSITY32I_EXT):
        case(GL_INTENSITY8UI_EXT):
        case(GL_INTENSITY16UI_EXT):
        case(GL_INTENSITY32UI_EXT):
            notify(WARN)<<"crImage::computePixelFormat("<<std::hex<<format<<std::dec<<") intensity pixel format is not correctly specified, so assume GL_LUMINANCE_INTEGER."<<std::endl;            
            return GL_LUMINANCE_INTEGER_EXT;
        case(GL_LUMINANCE_ALPHA8I_EXT):
        case(GL_LUMINANCE_ALPHA16I_EXT):
        case(GL_LUMINANCE_ALPHA32I_EXT):
        case(GL_LUMINANCE_ALPHA8UI_EXT):
        case(GL_LUMINANCE_ALPHA16UI_EXT):
        case(GL_LUMINANCE_ALPHA32UI_EXT):
            return GL_LUMINANCE_ALPHA_INTEGER_EXT;
        case(GL_RGB32I_EXT):
        case(GL_RGB16I_EXT):
        case(GL_RGB8I_EXT):
        case(GL_RGB32UI_EXT):
        case(GL_RGB16UI_EXT):
        case(GL_RGB8UI_EXT):
            return GL_RGB_INTEGER_EXT;
        case(GL_RGBA32I_EXT):
        case(GL_RGBA16I_EXT):
        case(GL_RGBA8I_EXT):
        case(GL_RGBA32UI_EXT):
        case(GL_RGBA16UI_EXT):
        case(GL_RGBA8UI_EXT):
            return GL_RGBA_INTEGER_EXT;;

        default:
            return format;
    }
}

GLenum crImage::computeFormatDataType(GLenum pixelFormat)
{
    switch (pixelFormat)
    {
        case GL_LUMINANCE32F_ARB:
        case GL_LUMINANCE16F_ARB: 
        case GL_LUMINANCE_ALPHA32F_ARB:
        case GL_LUMINANCE_ALPHA16F_ARB: 
        case GL_RGB32F_ARB:
        case GL_RGB16F_ARB: 
        case GL_RGBA32F_ARB:
        case GL_RGBA16F_ARB: return GL_FLOAT;

        case GL_RGBA32UI_EXT:
        case GL_RGB32UI_EXT:
        case GL_LUMINANCE32UI_EXT:
        case GL_LUMINANCE_ALPHA32UI_EXT: return GL_UNSIGNED_INT;

        case GL_RGB16UI_EXT:
        case GL_RGBA16UI_EXT:
        case GL_LUMINANCE16UI_EXT: 
        case GL_LUMINANCE_ALPHA16UI_EXT: return GL_UNSIGNED_SHORT;

        case GL_RGBA8UI_EXT:
        case GL_RGB8UI_EXT:
        case GL_LUMINANCE8UI_EXT:
        case GL_LUMINANCE_ALPHA8UI_EXT:  return GL_UNSIGNED_BYTE;

        case GL_RGBA32I_EXT:  
        case GL_RGB32I_EXT:
        case GL_LUMINANCE32I_EXT:
        case GL_LUMINANCE_ALPHA32I_EXT: return GL_INT;

        case GL_RGBA16I_EXT:
        case GL_RGB16I_EXT:
        case GL_LUMINANCE16I_EXT:
        case GL_LUMINANCE_ALPHA16I_EXT: return GL_SHORT;

        case GL_RGB8I_EXT: 
        case GL_RGBA8I_EXT: 
        case GL_LUMINANCE8I_EXT: 
        case GL_LUMINANCE_ALPHA8I_EXT: return GL_BYTE;

        case GL_RGBA:
        case GL_RGB:
        case GL_LUMINANCE:
        case GL_LUMINANCE_ALPHA: return GL_UNSIGNED_BYTE;

        default: 
        {
            notify(WARN)<<"error computeFormatType = "<<std::hex<<pixelFormat<<std::dec<<std::endl;
            return 0;
        }
    }
}

unsigned int crImage::computeNumComponents(GLenum pixelFormat)
{
    switch(pixelFormat)
    {
        case(GL_COMPRESSED_RGB_S3TC_DXT1_EXT): return 3;
        case(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT): return 4;
        case(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT): return 4;
        case(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT): return 4;
        case(GL_COMPRESSED_SIGNED_RED_RGTC1_EXT): return 1;
        case(GL_COMPRESSED_RED_RGTC1_EXT):   return 1;
        case(GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT): return 2;
        case(GL_COMPRESSED_RED_GREEN_RGTC2_EXT): return 2;    
        case(GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG): return 3;
        case(GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG): return 3;
        case(GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG): return 4;
        case(GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG): return 4;
        case(GL_ETC1_RGB8_OES): return 3;
        case(GL_COLOR_INDEX): return 1;
        case(GL_STENCIL_INDEX): return 1;
        case(GL_DEPTH_COMPONENT): return 1;
        case(GL_RED): return 1;
        case(GL_GREEN): return 1;
        case(GL_BLUE): return 1;
        case(GL_ALPHA): return 1;
        case(GL_ALPHA8I_EXT): return 1;
        case(GL_ALPHA8UI_EXT): return 1;
        case(GL_ALPHA16I_EXT): return 1;
        case(GL_ALPHA16UI_EXT): return 1;
        case(GL_ALPHA32I_EXT): return 1;
        case(GL_ALPHA32UI_EXT): return 1;
        case(GL_ALPHA16F_ARB): return 1;
        case(GL_ALPHA32F_ARB): return 1;
        case(GL_RGB): return 3;
        case(GL_BGR): return 3;
        case(GL_RGB8I_EXT): return 3;
        case(GL_RGB8UI_EXT): return 3;
        case(GL_RGB16I_EXT): return 3;
        case(GL_RGB16UI_EXT): return 3;
        case(GL_RGB32I_EXT): return 3;
        case(GL_RGB32UI_EXT): return 3;
        case(GL_RGB16F_ARB): return 3;
        case(GL_RGB32F_ARB): return 3;
        case(GL_RGBA16F_ARB): return 4;
        case(GL_RGBA32F_ARB): return 4;
        case(GL_RGBA): return 4;
        case(GL_BGRA): return 4;
        case(GL_RGBA8): return 4;
        case(GL_LUMINANCE): return 1;
        case(GL_LUMINANCE4): return 1;
        case(GL_LUMINANCE8): return 1;
        case(GL_LUMINANCE12): return 1;
        case(GL_LUMINANCE16): return 1;
        case(GL_LUMINANCE8I_EXT): return 1;
        case(GL_LUMINANCE8UI_EXT): return 1;
        case(GL_LUMINANCE16I_EXT): return 1;
        case(GL_LUMINANCE16UI_EXT): return 1;
        case(GL_LUMINANCE32I_EXT): return 1;
        case(GL_LUMINANCE32UI_EXT): return 1;
        case(GL_LUMINANCE16F_ARB): return 1;
        case(GL_LUMINANCE32F_ARB): return 1;
        case(GL_LUMINANCE4_ALPHA4): return 2;
        case(GL_LUMINANCE6_ALPHA2): return 2;
        case(GL_LUMINANCE8_ALPHA8): return 2;
        case(GL_LUMINANCE12_ALPHA4): return 2;
        case(GL_LUMINANCE12_ALPHA12): return 2;
        case(GL_LUMINANCE16_ALPHA16): return 2;
        case(GL_INTENSITY): return 1;
        case(GL_INTENSITY4): return 1;
        case(GL_INTENSITY8): return 1;
        case(GL_INTENSITY12): return 1;
        case(GL_INTENSITY16): return 1;
        case(GL_INTENSITY8UI_EXT): return 1;
        case(GL_INTENSITY8I_EXT): return 1;
        case(GL_INTENSITY16I_EXT): return 1;
        case(GL_INTENSITY16UI_EXT): return 1;
        case(GL_INTENSITY32I_EXT): return 1;
        case(GL_INTENSITY32UI_EXT): return 1;
        case(GL_INTENSITY16F_ARB): return 1;
        case(GL_INTENSITY32F_ARB): return 1;
        case(GL_LUMINANCE_ALPHA): return 2;
        case(GL_LUMINANCE_ALPHA8I_EXT): return 2;
        case(GL_LUMINANCE_ALPHA8UI_EXT): return 2;
        case(GL_LUMINANCE_ALPHA16I_EXT): return 2;
        case(GL_LUMINANCE_ALPHA16UI_EXT): return 2;
        case(GL_LUMINANCE_ALPHA32I_EXT): return 2;
        case(GL_LUMINANCE_ALPHA32UI_EXT): return 2;
        case(GL_LUMINANCE_ALPHA16F_ARB): return 2;
        case(GL_LUMINANCE_ALPHA32F_ARB): return 2;
        case(GL_HILO_NV): return 2;
        case(GL_DSDT_NV): return 2;
        case(GL_DSDT_MAG_NV): return 3;
        case(GL_DSDT_MAG_VIB_NV): return 4;
        case(GL_RED_INTEGER_EXT): return 1;
        case(GL_GREEN_INTEGER_EXT): return 1;
        case(GL_BLUE_INTEGER_EXT): return 1;
        case(GL_ALPHA_INTEGER_EXT): return 1;
        case(GL_RGB_INTEGER_EXT): return 3;
        case(GL_RGBA_INTEGER_EXT): return 4;
        case(GL_BGR_INTEGER_EXT): return 3;
        case(GL_BGRA_INTEGER_EXT): return 4;
        case(GL_LUMINANCE_INTEGER_EXT): return 1;
        case(GL_LUMINANCE_ALPHA_INTEGER_EXT): return 2;

        default:
        {
            notify(WARN)<<"error pixelFormat = "<<std::hex<<pixelFormat<<std::dec<<std::endl;
            return 0;
        }
    }        
}

unsigned int crImage::getNumComponents()
{
	return crImage::computeNumComponents(m_pixelFormat);
}

unsigned int crImage::computePixelSizeInBits(GLenum format,GLenum type)
{

    switch(format)
    {
        case(GL_COMPRESSED_RGB_S3TC_DXT1_EXT): return 4;
        case(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT): return 4;
        case(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT): return 8;
        case(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT): return 8;
        case(GL_COMPRESSED_SIGNED_RED_RGTC1_EXT): return 4;
        case(GL_COMPRESSED_RED_RGTC1_EXT):   return 4;
        case(GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT): return 8;
        case(GL_COMPRESSED_RED_GREEN_RGTC2_EXT): return 8;
        case(GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG): return 4;
        case(GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG): return 2;
        case(GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG): return 4;
        case(GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG): return 2;
        case(GL_ETC1_RGB8_OES): return 4;
        default: break;
    }

    // note, haven't yet added proper handling of the ARB GL_COMPRESSRED_* pathways
    // yet, no clear size for these since its probably implementation dependent
    // which raises the question of how to actually querry for these sizes...
    // will need to revisit this issue, for now just report an error.
    // this is possible a bit of mute point though as since the ARB compressed formats
    // arn't yet used for storing images to disk, so its likely that users wont have
    // CRCore::crImage's for pixel formats set the ARB compressed formats, just using these
    // compressed formats as internal texture modes.  This is very much speculation though
    // if get the below error then its time to revist this issue :-)
    // Robert Osfield, Jan 2005.
    switch(format)
    {
        case(GL_COMPRESSED_ALPHA):
        case(GL_COMPRESSED_LUMINANCE):
        case(GL_COMPRESSED_LUMINANCE_ALPHA):
        case(GL_COMPRESSED_INTENSITY):
        case(GL_COMPRESSED_RGB):
        case(GL_COMPRESSED_RGBA):
            notify(WARN)<<"crImage::computePixelSizeInBits(format,type) : cannot compute correct size of compressed format ("<<format<<") returning 0."<<std::endl;
            return 0;
        default: break;
    }

    switch(format)
    {
        case(GL_LUMINANCE4): return 4;
        case(GL_LUMINANCE8): return 8;
        case(GL_LUMINANCE12): return 12;
        case(GL_LUMINANCE16): return 16;
        case(GL_LUMINANCE4_ALPHA4): return 8;
        case(GL_LUMINANCE6_ALPHA2): return 8;
        case(GL_LUMINANCE8_ALPHA8): return 16;
        case(GL_LUMINANCE12_ALPHA4): return 16;
        case(GL_LUMINANCE12_ALPHA12): return 24;
        case(GL_LUMINANCE16_ALPHA16): return 32;
        case(GL_INTENSITY4): return 4;
        case(GL_INTENSITY8): return 8;
        case(GL_INTENSITY12): return 12;
        case(GL_INTENSITY16): return 16;
        default: break;
    }

    switch(type)
    {
   
        case(GL_BITMAP): return computeNumComponents(format);
        
        case(GL_BYTE):
        case(GL_UNSIGNED_BYTE): return 8*computeNumComponents(format);
        
        case(GL_HALF_FLOAT_NV):
        case(GL_SHORT):
        case(GL_UNSIGNED_SHORT): return 16*computeNumComponents(format);
        
        case(GL_INT):
        case(GL_UNSIGNED_INT):
        case(GL_FLOAT): return 32*computeNumComponents(format);
    
    
        case(GL_UNSIGNED_BYTE_3_3_2): 
        case(GL_UNSIGNED_BYTE_2_3_3_REV): return 8;
        
        case(GL_UNSIGNED_SHORT_5_6_5):
        case(GL_UNSIGNED_SHORT_5_6_5_REV):
        case(GL_UNSIGNED_SHORT_4_4_4_4):
        case(GL_UNSIGNED_SHORT_4_4_4_4_REV):
        case(GL_UNSIGNED_SHORT_5_5_5_1):
        case(GL_UNSIGNED_SHORT_1_5_5_5_REV): return 16;
        
        case(GL_UNSIGNED_INT_8_8_8_8):
        case(GL_UNSIGNED_INT_10_10_10_2):
        case(GL_UNSIGNED_INT_8_8_8_8_REV):
        case(GL_UNSIGNED_INT_2_10_10_10_REV): return 32;
        default: 
        {
            notify(WARN)<<"error type = "<<type<<std::endl;
            return 0;
        }
    }    

}

unsigned int crImage::computeRowWidthInBytes(int width,GLenum pixelFormat,GLenum type,int packing)
{
	if(packing==0)
		return 0;
    unsigned int pixelSize = computePixelSizeInBits(pixelFormat,type);
    int widthInBits = width*pixelSize;
    int packingInBits = packing*8;
    //notify(INFO) << "width="<<width<<" pixelSize="<<pixelSize<<"  width in bit="<<widthInBits<<" packingInBits="<<packingInBits<<" widthInBits%packingInBits="<<widthInBits%packingInBits<<std::endl;
    return (widthInBits/packingInBits + ((widthInBits%packingInBits)?1:0))*packing;
}

int crImage::computeNearestPowerOfTwo(int s,float bias)
{
    if ((s & (s-1))!=0)
    {
        // it isn't so lets find the closest power of two.
        // yes, logf and powf are slow, but this code should
        // only be called during scene graph initilization,
        // if at all, so not critical in the greater scheme.
        float p2 = logf((float)s)/logf(2.0f);
        float rounded_p2 = floorf(p2+bias);
        s = (int)(powf(2.0f,rounded_p2));
    }
    return s;
}

int crImage::computeNumberOfMipmapLevels(int s,int t, int r)
{
    int w = maximum(s, t);
    w = maximum(w, r);
    return 1 + static_cast<int>(floor(logf(w)/logf(2.0f)));
}

bool crImage::isCompressed() const
{
    switch(m_pixelFormat)
    {
        case(GL_COMPRESSED_ALPHA_ARB):
        case(GL_COMPRESSED_INTENSITY_ARB):
        case(GL_COMPRESSED_LUMINANCE_ALPHA_ARB):
        case(GL_COMPRESSED_LUMINANCE_ARB):
        case(GL_COMPRESSED_RGBA_ARB):
        case(GL_COMPRESSED_RGB_ARB):
        case(GL_COMPRESSED_RGB_S3TC_DXT1_EXT):
        case(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT):
        case(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT):
        case(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT):
        case(GL_COMPRESSED_SIGNED_RED_RGTC1_EXT):
        case(GL_COMPRESSED_RED_RGTC1_EXT):
        case(GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT):
        case(GL_COMPRESSED_RED_GREEN_RGTC2_EXT):
        case(GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG): 
        case(GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG):
        case(GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG):
        case(GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG):
        case(GL_ETC1_RGB8_OES):
            return true;
        default:
            return false;
    }
}

unsigned int crImage::getTotalSizeInBytesIncludingMipmaps() const
{
    if (m_mipmapData.empty()) 
    {
        // no mips so just return size of main image
        return getTotalSizeInBytes();
    }
    
    int s = m_s;
    int t = m_t;
    int r = m_r;
    
    unsigned int maxValue = 0;
    for(unsigned int i=0;i<m_mipmapData.size() && m_mipmapData[i];++i)
    {
        s >>= 1;
        t >>= 1;
        r >>= 1;
        maxValue = maximum(maxValue,m_mipmapData[i]);
   }
   
   if (s==0) s=1;
   if (t==0) t=1;
   if (r==0) r=1;
   
   unsigned int sizeOfLastMipMap = computeRowWidthInBytes(s,m_pixelFormat,m_dataType,m_packing)* r*t;
   switch(m_pixelFormat)
   {
        case(GL_COMPRESSED_RGB_S3TC_DXT1_EXT):
        case(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT):
           sizeOfLastMipMap = maximum(sizeOfLastMipMap, 8u); // block size of 8
           break;
        case(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT):
        case(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT):
        case(GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG):
        case(GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG):
        case(GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG):
        case(GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG):
        case(GL_ETC1_RGB8_OES):
           sizeOfLastMipMap = maximum(sizeOfLastMipMap, 16u); // block size of 16
           break;
        case(GL_COMPRESSED_SIGNED_RED_RGTC1_EXT):
        case(GL_COMPRESSED_RED_RGTC1_EXT):
            sizeOfLastMipMap = maximum(sizeOfLastMipMap, 8u); // block size of 8
            break;
        case(GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT):
        case(GL_COMPRESSED_RED_GREEN_RGTC2_EXT):
            sizeOfLastMipMap = maximum(sizeOfLastMipMap, 16u); // block size of 8
            break;
        default: break;
   }

   // notify(INFO)<<"sizeOfLastMipMap="<<sizeOfLastMipMap<<"\ts="<<s<<"\tt="<<t<<"\tr"<<r<<std::endl;                  

   return maxValue+sizeOfLastMipMap;

}


void crImage::setInternalTextureFormat(GLint internalFormat)
{
    // won't do any sanity checking right now, leave it to 
    // OpenGL to make the call.
    m_internalTextureFormat = internalFormat;
}

void crImage::setPixelFormat(GLenum pixelFormat)
{
    m_pixelFormat = pixelFormat;
}

void crImage::setDataType(GLenum dataType)
{
    if (m_dataType==dataType) return; // do nothing if the same.

    if (m_dataType==0)
    {
        // setting the datatype for the first time
        m_dataType = dataType;
    }
    else
    {
        notify(WARN)<<"crImage::setDataType(..) - warning, attempt to reset the data type not permitted."<<std::endl;
    }
}


void crImage::allocateImage(int s,int t,int r,
                        GLenum format,GLenum type,
                        int packing)
{
    m_mipmapData.clear();

    unsigned int previousTotalSize = 0;
    
    if (m_data) previousTotalSize = computeRowWidthInBytes(m_s,m_pixelFormat,m_dataType,m_packing)*m_t*m_r;
    
    unsigned int newTotalSize = computeRowWidthInBytes(s,format,type,packing)*t*r;

    if (newTotalSize!=previousTotalSize)
    {
        if (newTotalSize)
            setData(new unsigned char [newTotalSize],USE_NEW_DELETE);
        else
            deallocateData(); // and sets it to NULL.
    }

    if (m_data)
    {
        m_s = s;
        m_t = t;
        m_r = r;
        m_pixelFormat = format;
        m_dataType = type;
        m_packing = packing;
        
        // preserve internalTextureFormat if already set, otherwise
        // use the pixelFormat as the source for the format.
        if (m_internalTextureFormat==0) m_internalTextureFormat = format;
    }
    else
    {
    
        // failed to allocate memory, for now, will simply set values to 0.
        m_s = 0;
        m_t = 0;
        m_r = 0;
        m_pixelFormat = 0;
        m_dataType = 0;
        m_packing = 0;
        
        // commenting out reset of m_internalTextureFormat as we are changing
        // policy so that allocateImage honours previous settings of m_internalTextureFormat.
        //m_internalTextureFormat = 0;
    }
    
    dirty();
}

void crImage::setImage(int s,int t,int r,
                     GLint internalTextureFormat,
                     GLenum format,GLenum type,
                     unsigned char *data,
                     AllocationMode mode,
                     int packing)
{
    m_mipmapData.clear();

    m_s = s;
    m_t = t;
    m_r = r;

    m_internalTextureFormat = internalTextureFormat;
    m_pixelFormat    = format;
    m_dataType       = type;

    setData(data,mode);

    m_packing = packing;
        
    dirty();

}

void crImage::readPixels(int x,int y,int width,int height,
                       GLenum format,GLenum type)
{
    allocateImage(width,height,1,format,type);

    glPixelStorei(GL_PACK_ALIGNMENT,m_packing);

    glReadPixels(x,y,width,height,format,type,m_data);
}


void crImage::readImageFromCurrentTexture(unsigned int contextID, bool copyMipMapsIfAvailable, GLenum type)
{
#if !defined(GLES1_AVAILABLE) && !defined(GLES2_AVAILABLE)
    // notify(NOTICE)<<"crImage::readImageFromCurrentTexture()"<<std::endl;

    const CRCore::crTexture::Extensions* extensions = CRCore::crTexture::getExtensions(contextID,true);
    const CRCore::crTexture3D::Extensions* extensions3D = CRCore::crTexture3D::getExtensions(contextID,true);
    const CRCore::crTexture2DArray::Extensions* extensions2DArray = CRCore::crTexture2DArray::getExtensions(contextID,true);

    
    GLboolean binding1D = GL_FALSE, binding2D = GL_FALSE, binding3D = GL_FALSE, binding2DArray = GL_FALSE;

    glGetBooleanv(GL_TEXTURE_BINDING_1D, &binding1D);
    glGetBooleanv(GL_TEXTURE_BINDING_2D, &binding2D);
    glGetBooleanv(GL_TEXTURE_BINDING_3D, &binding3D);

    if (extensions2DArray->isTexture2DArraySupported())
    {
        glGetBooleanv(GL_TEXTURE_BINDING_2D_ARRAY_EXT, &binding2DArray);
    }

    GLenum textureMode = binding1D ? GL_TEXTURE_1D : binding2D ? GL_TEXTURE_2D : binding3D ? GL_TEXTURE_3D : binding2DArray ? GL_TEXTURE_2D_ARRAY_EXT : 0;
    
    if (textureMode==0) return;

    GLint internalformat;
    GLint width;
    GLint height;
    GLint depth;
    GLint packing;

    GLint numMipMaps = 0;
    if (copyMipMapsIfAvailable)
    {
        for(;numMipMaps<20;++numMipMaps)
        {
            glGetTexLevelParameteriv(textureMode, numMipMaps, GL_TEXTURE_WIDTH, &width);
            glGetTexLevelParameteriv(textureMode, numMipMaps, GL_TEXTURE_HEIGHT, &height);
            glGetTexLevelParameteriv(textureMode, numMipMaps, GL_TEXTURE_DEPTH, &depth);
            // notify(NOTICE)<<"   numMipMaps="<<numMipMaps<<" width="<<width<<" height="<<height<<" depth="<<depth<<std::endl;
            if (width==0 || height==0 || depth==0) break;
        }
    }
    else
    {
        numMipMaps = 1;
    }
    
    // notify(NOTICE)<<"crImage::readImageFromCurrentTexture() : numMipMaps = "<<numMipMaps<<std::endl;

        
    GLint compressed = 0;

    if (textureMode==GL_TEXTURE_2D)
    {
        if (extensions->isCompressedTexImage2DSupported())
        {
            glGetTexLevelParameteriv(textureMode, 0, GL_TEXTURE_COMPRESSED_ARB,&compressed);
        }
    }
    else if (textureMode==GL_TEXTURE_3D)
    {
        if (extensions3D->isCompressedTexImage3DSupported())
        {
            glGetTexLevelParameteriv(textureMode, 0, GL_TEXTURE_COMPRESSED_ARB,&compressed);
        }
    }
    else if (textureMode==GL_TEXTURE_2D_ARRAY_EXT)
    {
        if (extensions2DArray->isCompressedTexImage3DSupported())
        {
            glGetTexLevelParameteriv(textureMode, 0, GL_TEXTURE_COMPRESSED_ARB,&compressed);
        }
    }
    
    
        
    /* if the compression has been successful */
    if (compressed == GL_TRUE)
    {

        MipmapDataType mipMapData;
        
        unsigned int total_size = 0;
        GLint i;
        for(i=0;i<numMipMaps;++i)
        {
            if (i>0) mipMapData.push_back(total_size);
            
            GLint compressed_size;
            glGetTexLevelParameteriv(textureMode, i, GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB, &compressed_size);
            
            total_size += compressed_size;
        }
        
        
        unsigned char* data = new unsigned char[total_size];
        if (!data)
        {
            notify(WARN)<<"Warning: crImage::readImageFromCurrentTexture(..) out of memory, now image read."<<std::endl;
            return; 
        }

        deallocateData(); // and sets it to NULL.

        glGetTexLevelParameteriv(textureMode, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalformat);
        glGetTexLevelParameteriv(textureMode, 0, GL_TEXTURE_WIDTH, &width);
        glGetTexLevelParameteriv(textureMode, 0, GL_TEXTURE_HEIGHT, &height);
        glGetTexLevelParameteriv(textureMode, 0, GL_TEXTURE_DEPTH, &depth);
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &packing);
        glPixelStorei(GL_PACK_ALIGNMENT, packing);

        m_data = data;
        m_s = width;
        m_t = height;
        m_r = depth;
        
        m_pixelFormat = internalformat;
        m_dataType = type;
        m_internalTextureFormat = internalformat;
        m_mipmapData = mipMapData;
        m_allocationMode=USE_NEW_DELETE;
        m_packing = packing;
        
        for(i=0;i<numMipMaps;++i)
        {
            extensions->glGetCompressedTexImage(textureMode, i, getMipmapData(i));
        }

        dirty();
    
    }
    else
    {
        MipmapDataType mipMapData;

        // Get the internal texture format and packing value from OpenGL,
        // instead of using possibly outdated values from the class.
        glGetTexLevelParameteriv(textureMode, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalformat);
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &packing);
        glPixelStorei(GL_PACK_ALIGNMENT, packing);

        unsigned int total_size = 0;
        GLint i;
        for(i=0;i<numMipMaps;++i)
        {
            if (i>0) mipMapData.push_back(total_size);
            
            glGetTexLevelParameteriv(textureMode, i, GL_TEXTURE_WIDTH, &width);
            glGetTexLevelParameteriv(textureMode, i, GL_TEXTURE_HEIGHT, &height);
            glGetTexLevelParameteriv(textureMode, i, GL_TEXTURE_DEPTH, &depth);
            
            unsigned int level_size = computeRowWidthInBytes(width,internalformat,type,packing)*height*depth;

            total_size += level_size;
        }
        
        
        unsigned char* data = new unsigned char[total_size];
        if (!data)
        {
            notify(WARN)<<"Warning: crImage::readImageFromCurrentTexture(..) out of memory, now image read."<<std::endl;
            return; 
        }

        deallocateData(); // and sets it to NULL.

        glGetTexLevelParameteriv(textureMode, 0, GL_TEXTURE_WIDTH, &width);
        glGetTexLevelParameteriv(textureMode, 0, GL_TEXTURE_HEIGHT, &height);
        glGetTexLevelParameteriv(textureMode, 0, GL_TEXTURE_DEPTH, &depth);

        m_data = data;
        m_s = width;
        m_t = height;
        m_r = depth;
        
        m_pixelFormat = computePixelFormat(internalformat);
        m_dataType = type;
        m_internalTextureFormat = internalformat;
        m_mipmapData = mipMapData;
        m_allocationMode=USE_NEW_DELETE;
        m_packing = packing;
        
        for(i=0;i<numMipMaps;++i)
        {
            glGetTexImage(textureMode,i,m_pixelFormat,m_dataType,getMipmapData(i));
        }

        dirty();
    }    
#else
    notify(NOTICE)<<"Warning: crImage::readImageFromCurrentTexture() not supported."<<std::endl;
#endif
}

void crImage::scaleImage(int s,int t,int r, GLenum newDataType)
{
    if (m_s==s && m_t==t && m_r==r) return;

    if (m_data==NULL)
    {
        notify(WARN) << "Error crImage::scaleImage() do not succeed : cannot scale NULL image."<<std::endl;
        return;
    }

    if (m_r!=1 || r!=1)
    {
        notify(WARN) << "Error crImage::scaleImage() do not succeed : scaling of volumes not implemented."<<std::endl;
        return;
    }

    unsigned int newTotalSize = computeRowWidthInBytes(s,m_pixelFormat,newDataType,m_packing)*t;

    // need to sort out what size to really use...
    unsigned char* newData = new unsigned char [newTotalSize];
    if (!newData)
    {
        // should we throw an exception???  Just return for time being.
        notify(FATAL) << "Error crImage::scaleImage() do not succeed : out of memory."<<newTotalSize<<std::endl;
        return;
    }

	glPixelStorei(GL_PACK_ALIGNMENT,m_packing);
	glPixelStorei(GL_UNPACK_ALIGNMENT,m_packing);

	GLint status = gluScaleImage(m_pixelFormat,
		m_s,
		m_t,
		m_dataType,
		m_data,
		s,
		t,
		newDataType,
		newData);

    if (status==0)
    {

        // free old image.
        m_s = s;
        m_t = t;
        m_dataType = newDataType;
        setData(newData,USE_NEW_DELETE);
    }
    else
    {
        delete [] newData;

        notify(WARN) << "Error crImage::scaleImage() did not succeed : errorString = "<< gluErrorString((GLenum)status) << ". The rendering context may be invalid." << std::endl;
    }

    dirty();
}

void crImage::copySubImage(int s_offset, int t_offset, int r_offset, const CRCore::crImage* source)
{
    if (!source) return;
    if (s_offset<0 || t_offset<0 || r_offset<0)
    {
        notify(WARN)<<"Warning: negative offsets passed to crImage::copySubImage(..) not supported, operation ignored."<<std::endl;
        return;
    }

    if (!m_data)
    {
        //notify(INFO)<<"allocating image"<<endl;
        allocateImage(s_offset+source->s(),t_offset+source->t(),r_offset+source->r(),
                    source->getPixelFormat(),source->getDataType(),
                    source->getPacking());
    }

    if (s_offset>=m_s || t_offset>=m_t  || r_offset>=m_r)
    {
        notify(WARN)<<"Warning: offsets passed to crImage::copySubImage(..) outside destination image, operation ignored."<<std::endl;
        return;
    }


    if (m_pixelFormat != source->getPixelFormat())
    {
        notify(WARN)<<"Warning: image with an incompatible pixel formats passed to crImage::copySubImage(..), operation ignored."<<std::endl;
        return;
    }

    void* data_destination = data(s_offset,t_offset,r_offset);

	glPixelStorei(GL_PACK_ALIGNMENT,source->getPacking());
	glPixelStorei(GL_PACK_ROW_LENGTH,m_s);

	glPixelStorei(GL_UNPACK_ALIGNMENT,m_packing);

	GLint status = gluScaleImage(m_pixelFormat,
		source->s(),
		source->t(),
		source->getDataType(),
		source->data(),
		source->s(),
		source->t(),
		m_dataType,
		data_destination);

    glPixelStorei(GL_PACK_ROW_LENGTH,0);

    if (status!=0)
    {
        notify(WARN) << "Error crImage::scaleImage() did not succeed : errorString = "<< gluErrorString((GLenum)status) << ". The rendering context may be invalid." << std::endl;
    }
}

void crImage::flipHorizontal()
{
    if (m_data==NULL)
    {
        notify(WARN) << "Error crImage::flipHorizontal() did not succeed : cannot flip NULL image."<<std::endl;
        return;
    }

    unsigned int elemSize = getPixelSizeInBits()/8;

    if (m_mipmapData.empty())
    {

        for(int r=0;r<m_r;++r)
        {
            for (int t=0; t<m_t; ++t)
            {
                unsigned char* rowData = m_data+t*getRowSizeInBytes()+r*getImageSizeInBytes();
                unsigned char* left  = rowData ;
                unsigned char* right = rowData + ((m_s-1)*getPixelSizeInBits())/8;

                while (left < right)
                {
                    char tmp[32];  // max elem size is four floats
                    memcpy(tmp, left, elemSize);
                    memcpy(left, right, elemSize);
                    memcpy(right, tmp, elemSize);
                    left  += elemSize;
                    right -= elemSize;
                }
            }
        }
    }
    else
    {
        notify(WARN) << "Error crImage::flipHorizontal() did not succeed : cannot flip mipmapped image."<<std::endl;
        return;
    }
        
    dirty();
}

void flipImageVertical(unsigned char* top, unsigned char* bottom, unsigned int rowSize)
{
    while(top<bottom)
    {
        for(unsigned int i=0;i<rowSize;++i, ++top,++bottom)
        {
            unsigned char temp=*top;
            *top = *bottom;
            *bottom = temp;
        }
        bottom -= 2*rowSize;
    }
}


void crImage::flipVertical()
{
    if (m_data==NULL)
    {
        notify(WARN) << "Error crImage::flipVertical() do not succeed : cannot flip NULL image."<<std::endl;
        return;
    }

    if (!m_mipmapData.empty() && m_r>1)
    {
        notify(WARN) << "Error crImage::flipVertical() do not succeed : flipping of mipmap 3d textures not yet supported."<<std::endl;
        return;
    }

    if (m_mipmapData.empty())
    {
        // no mipmaps,
        // so we can safely handle 3d textures
        for(int r=0;r<m_r;++r)
        {
            if (!dxtc_tool::VerticalFlip(m_s,m_t,m_pixelFormat,data(0,0,r)))
            {
                // its not a compressed image, so implement flip oursleves.
                
                unsigned int rowSize = computeRowWidthInBytes(m_s,m_pixelFormat,m_dataType,m_packing);
                unsigned char* top = data(0,0,r);
                unsigned char* bottom = top + (m_t-1)*rowSize;
                    
                flipImageVertical(top, bottom, rowSize);
            }
        }
    }
    else if (m_r==1)
    {
        if (!dxtc_tool::VerticalFlip(m_s,m_t,m_pixelFormat,m_data))
        {
            // its not a compressed image, so implement flip oursleves.
            unsigned int rowSize = computeRowWidthInBytes(m_s,m_pixelFormat,m_dataType,m_packing);
            unsigned char* top = data(0,0,0);
            unsigned char* bottom = top + (m_t-1)*rowSize;

            flipImageVertical(top, bottom, rowSize);
        }

        int s = m_s;
        int t = m_t;
        //int r = m_r;

        for(unsigned int i=0;i<m_mipmapData.size() && m_mipmapData[i];++i)
        {
            s >>= 1;
            t >>= 1;
            if (s==0) s=1;
            if (t==0) t=1;
            if (!dxtc_tool::VerticalFlip(s,t,m_pixelFormat,m_data+m_mipmapData[i]))
            {
                // its not a compressed image, so implement flip oursleves.
                unsigned int rowSize = computeRowWidthInBytes(s,m_pixelFormat,m_dataType,m_packing);
                unsigned char* top = m_data+m_mipmapData[i];
                unsigned char* bottom = top + (t-1)*rowSize;

                flipImageVertical(top, bottom, rowSize);
            }
       }
    }   

    dirty();
}



void crImage::ensureValidSizeForTexturing(GLint maxTextureSize)
{
    int new_s = computeNearestPowerOfTwo(m_s);
    int new_t = computeNearestPowerOfTwo(m_t);
    
    if (new_s>maxTextureSize) new_s = maxTextureSize;
    if (new_t>maxTextureSize) new_t = maxTextureSize;
    
    if (new_s!=m_s || new_t!=m_t)
    {
        if (!m_fileName.empty()) { notify(NOTICE) << "Scaling image '"<<m_fileName<<"' from ("<<m_s<<","<<m_t<<") to ("<<new_s<<","<<new_t<<")"<<std::endl; }
        else { notify(NOTICE) << "Scaling image from ("<<m_s<<","<<m_t<<") to ("<<new_s<<","<<new_t<<")"<<std::endl; }

        scaleImage(new_s,new_t,m_r);
    }
}


template <typename T>    
bool _findLowerAlphaValueInRow(unsigned int num, T* data,T value, unsigned int delta)
{
    for(unsigned int i=0;i<num;++i)
    {
        if (*data<value) return true;
        data += delta;
    }
    return false;
}

template <typename T>    
bool _maskedFindLowerAlphaValueInRow(unsigned int num, T* data,T value, T mask, unsigned int delta)
{
    for(unsigned int i=0;i<num;++i)
    {
        if ((*data & mask)<value) return true;
        data += delta;
    }
    return false;
}

bool crImage::isImageTranslucent() const
{
    unsigned int offset = 0;
    unsigned int delta = 1;
    switch(m_pixelFormat)
    {
        case(GL_ALPHA):
            offset = 0;
            delta = 1;
            break;
        case(GL_LUMINANCE_ALPHA):
            offset = 1;
            delta = 2;
            break;
        case(GL_RGBA):
            offset = 3;
            delta = 4;
            break;
        case(GL_BGRA):
            offset = 3;
            delta = 4;
            break;
        case(GL_RGB):
            return false;
        case(GL_BGR):
            return false;
        case(GL_COMPRESSED_RGB_S3TC_DXT1_EXT):
            return false;
        case(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT):
        case(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT):
        case(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT):
            return dxtc_tool::CompressedImageTranslucent(m_s, m_t, m_pixelFormat, m_data);
        default:
            return false;
    }

    for(int ir=0;ir<r();++ir)
    {
        for(int it=0;it<t();++it)
        {
            const unsigned char* d = data(0,it,ir);
            switch(m_dataType)
            {
                case(GL_BYTE):
                    if (_findLowerAlphaValueInRow(s(), (char*)d +offset, (char)127, delta))
                        return true;
                    break;
                case(GL_UNSIGNED_BYTE):
                    if (_findLowerAlphaValueInRow(s(), (unsigned char*)d + offset, (unsigned char)255, delta))
                        return true;
                    break;
                case(GL_SHORT):
                    if (_findLowerAlphaValueInRow(s(), (short*)d + offset, (short)32767, delta))
                        return true;
                    break;
                case(GL_UNSIGNED_SHORT):
                    if (_findLowerAlphaValueInRow(s(), (unsigned short*)d + offset, (unsigned short)65535, delta))
                        return true;
                    break;
                case(GL_INT):
                    if (_findLowerAlphaValueInRow(s(), (int*)d + offset, (int)2147483647, delta))
                        return true;
                    break;
                case(GL_UNSIGNED_INT):
                    if (_findLowerAlphaValueInRow(s(), (unsigned int*)d + offset, 4294967295u, delta))
                        return true;
                    break;
                case(GL_FLOAT):
                    if (_findLowerAlphaValueInRow(s(), (float*)d + offset, 1.0f, delta))
                        return true;
                    break;
                case(GL_UNSIGNED_SHORT_5_5_5_1):
                    if (_maskedFindLowerAlphaValueInRow(s(), (unsigned short*)d,
                                                        (unsigned short)0x0001,
                                                        (unsigned short)0x0001, 1))
                        return true;
                    break;
                case(GL_UNSIGNED_SHORT_1_5_5_5_REV):
                    if (_maskedFindLowerAlphaValueInRow(s(), (unsigned short*)d,
                                                        (unsigned short)0x8000,
                                                        (unsigned short)0x8000, 1))
                        return true;
                    break;
                case(GL_UNSIGNED_SHORT_4_4_4_4):
                    if (_maskedFindLowerAlphaValueInRow(s(), (unsigned short*)d,
                                                        (unsigned short)0x000f,
                                                        (unsigned short)0x000f, 1))
                        return true;
                    break;
                case(GL_UNSIGNED_SHORT_4_4_4_4_REV):
                    if (_maskedFindLowerAlphaValueInRow(s(), (unsigned short*)d,
                                                        (unsigned short)0xf000,
                                                        (unsigned short)0xf000, 1))
                        return true;
                    break;
                case(GL_UNSIGNED_INT_10_10_10_2):
                    if (_maskedFindLowerAlphaValueInRow(s(), (unsigned int*)d,
                                                        0x00000003u,
                                                        0x00000003u, 1))
                        return true;
                    break;                    
                case(GL_UNSIGNED_INT_2_10_10_10_REV):
                    if (_maskedFindLowerAlphaValueInRow(s(), (unsigned int*)d,
                                                        0xc0000000u,
                                                        0xc0000000u, 1))
                        return true;
                    break;
                case(GL_HALF_FLOAT_NV):
                    if (_findLowerAlphaValueInRow(s(), (unsigned short*)d + offset,
                                                  (unsigned short)0x3c00, delta))
                        return true;
                    break;
            }
        }
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////


//crObject* CRCore::createObjectForImage(CRCore::crImage* image)
//{
//    return createObjectForImage(image,image->s(),image->t());
//}
//
//
//#include <CRCore/crTextureRectangle.h> 
//
//
//crObject* CRCore::createObjectForImage(CRCore::crImage* image,float s,float t)
//{
//    if (image)
//    {
//        if (s>0 && t>0)
//        {
//
//            float y = 1.0;
//            float x = y*(s/t);
//
//            float texcoord_y_b = (image->getOrigin() == CRCore::crImage::BOTTOM_LEFT) ? 0.0f : 1.0f;
//            float texcoord_y_t = (image->getOrigin() == CRCore::crImage::BOTTOM_LEFT) ? 1.0f : 0.0f;
//
//            // set up the texture.
//
//#if 0
//            CRCore::crTextureRectangle* texture = new CRCore::crTextureRectangle;
//            texture->setFilter(CRCore::crTexture::MIN_FILTER,CRCore::crTexture::LINEAR);
//            texture->setFilter(CRCore::crTexture::MAG_FILTER,CRCore::crTexture::LINEAR);
//            //texture->setResizeNonPowerOfTwoHint(false);
//            float texcoord_x = image->s();
//            texcoord_y_b *= image->t();
//            texcoord_y_t *= image->t();
//#else
//            CRCore::crTexture2D* texture = new CRCore::crTexture2D;
//            texture->setFilter(CRCore::crTexture::MIN_FILTER,CRCore::crTexture::LINEAR);
//            texture->setFilter(CRCore::crTexture::MAG_FILTER,CRCore::crTexture::LINEAR);
//            texture->setResizeNonPowerOfTwoHint(false);
//            float texcoord_x = 1.0f;
//#endif
//            texture->setImage(image);
//
//            // set up the drawstate.
//            CRCore::crStateSet* dstate = new CRCore::crStateSet;
//            dstate->setMode(GL_CULL_FACE,CRCore::crStateAttribute::OFF);
//            dstate->setMode(GL_LIGHTING,CRCore::crStateAttribute::OFF);
//            dstate->setTextureAttributeAndModes(0, texture,CRCore::crStateAttribute::ON);
//
//            // set up the geoset.
//            crGeometry* geom = new crGeometry;
//            geom->setStateSet(dstate);
//
//            Vec3Array* coords = new Vec3Array(4);
//            (*coords)[0].set(-x,0.0f,y);
//            (*coords)[1].set(-x,0.0f,-y);
//            (*coords)[2].set(x,0.0f,-y);
//            (*coords)[3].set(x,0.0f,y);
//            geom->setVertexArray(coords);
//
//            Vec2Array* tcoords = new Vec2Array(4);
//            (*tcoords)[0].set(0.0f*texcoord_x,texcoord_y_t);
//            (*tcoords)[1].set(0.0f*texcoord_x,texcoord_y_b);
//            (*tcoords)[2].set(1.0f*texcoord_x,texcoord_y_b);
//            (*tcoords)[3].set(1.0f*texcoord_x,texcoord_y_t);
//            geom->setTexCoordArray(0,tcoords);
//
//            //CRCore::Vec4Array* colours = new CRCore::Vec4Array(1);
//            //(*colours)[0].set(1.0f,1.0f,1.0,1.0f);
//            //geom->setColorArray(colours);
//            //geom->setColorBinding(crGeometry::BIND_OVERALL);
//			geom->setColorBinding(crGeometry::BIND_OFF);
//			geom->setNormalBinding(crGeometry::BIND_OFF);
//
//            geom->addPrimitive(new DrawArrays(crPrimitive::PT_QUADS,0,4));
//
//            // set up the geode.
//            CRCore::crObject* geode = new CRCore::crObject;
//            geode->addDrawable(geom);
//
//            return geode;
//
//        }
//        else
//        {
//            return NULL;
//        }
//    }
//    else
//    {
//        return NULL;
//    }
//}

template <typename T>    
crVector4 _readColor(GLenum pixelFormat, T* data,float scale)
{
    switch(pixelFormat)
    {
        case(GL_DEPTH_COMPONENT):   //intentionally fall through and execute the code for GL_LUMINANCE
        case(GL_LUMINANCE):         { float l = float(*data++)*scale; return crVector4(l, l, l, 1.0f); }
        case(GL_ALPHA):             { float a = float(*data++)*scale; return crVector4(1.0f, 1.0f, 1.0f, a); }
        case(GL_LUMINANCE_ALPHA):   { float l = float(*data++)*scale; float a = float(*data++)*scale; return crVector4(l,l,l,a); }
        case(GL_RGB):               { float r = float(*data++)*scale; float g = float(*data++)*scale; float b = float(*data++)*scale; return crVector4(r,g,b,1.0f); }
        case(GL_RGBA):              { float r = float(*data++)*scale; float g = float(*data++)*scale; float b = float(*data++)*scale; float a = float(*data++)*scale; return crVector4(r,g,b,a); }
        case(GL_BGR):               { float b = float(*data++)*scale; float g = float(*data++)*scale; float r = float(*data++)*scale; return crVector4(r,g,b,1.0f); }
        case(GL_BGRA):              { float b = float(*data++)*scale; float g = float(*data++)*scale; float r = float(*data++)*scale; float a = float(*data++)*scale; return crVector4(r,g,b,a); }
    }
    return crVector4(1.0f,1.0f,1.0f,1.0f);
}

crVector4 crImage::getColor(unsigned int s,unsigned t,unsigned r) const
{
    const unsigned char* ptr = data(s,t,r);

    switch(m_dataType)
    {
        case(GL_BYTE):              return _readColor(m_pixelFormat, (char*)ptr,             1.0f/128.0f);
        case(GL_UNSIGNED_BYTE):     return _readColor(m_pixelFormat, (unsigned char*)ptr,    1.0f/255.0f);
        case(GL_SHORT):             return _readColor(m_pixelFormat, (short*)ptr,            1.0f/32768.0f);
        case(GL_UNSIGNED_SHORT):    return _readColor(m_pixelFormat, (unsigned short*)ptr,   1.0f/65535.0f);
        case(GL_INT):               return _readColor(m_pixelFormat, (int*)ptr,              1.0f/2147483648.0f);
        case(GL_UNSIGNED_INT):      return _readColor(m_pixelFormat, (unsigned int*)ptr,     1.0f/4294967295.0f);
        case(GL_FLOAT):             return _readColor(m_pixelFormat, (float*)ptr,            1.0f);
    }
    return crVector4(1.0f,1.0f,1.0f,1.0f);
}

crVector4 crImage::getColor(const crVector3& texcoord) const
{
    int s = int(texcoord.x()*float(m_s-1)) % m_s;
    int t = int(texcoord.y()*float(m_t-1)) % m_t;
    int r = int(texcoord.z()*float(m_r-1)) % m_r;
    //notify(NOTICE)<<"getColor("<<texcoord<<")="<<getColor(s,t,r)<<std::endl;
    return getColor(s,t,r);
}
