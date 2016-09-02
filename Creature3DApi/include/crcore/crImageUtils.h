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
#ifndef CRCORE_IMAGEUTILS
#define CRCORE_IMAGEUTILS 1

#include <CRCore/crExport.h>

#include <CRCore/crImage.h>

namespace CRCore {

template <typename T, class O>    
void _readRow(unsigned int num, GLenum pixelFormat, const T* data,float scale, O& operation)
{
    switch(pixelFormat)
    {
        case(GL_LUMINANCE):         { for(unsigned int i=0;i<num;++i) { float l = float(*data++)*scale; operation.luminance(l); } }  break;
        case(GL_ALPHA):             { for(unsigned int i=0;i<num;++i) { float a = float(*data++)*scale; operation.alpha(a); } }  break;
        case(GL_LUMINANCE_ALPHA):   { for(unsigned int i=0;i<num;++i) { float l = float(*data++)*scale; float a = float(*data++)*scale; operation.luminance_alpha(l,a); } }  break;
        case(GL_RGB):               { for(unsigned int i=0;i<num;++i) { float r = float(*data++)*scale; float g = float(*data++)*scale; float b = float(*data++)*scale; operation.rgb(r,g,b); } }  break;
        case(GL_RGBA):              { for(unsigned int i=0;i<num;++i) { float r = float(*data++)*scale; float g = float(*data++)*scale; float b = float(*data++)*scale; float a = float(*data++)*scale; operation.rgba(r,g,b,a); } }  break;
        case(GL_BGR):               { for(unsigned int i=0;i<num;++i) { float b = float(*data++)*scale; float g = float(*data++)*scale; float r = float(*data++)*scale; operation.rgb(r,g,b); } }  break;
        case(GL_BGRA):              { for(unsigned int i=0;i<num;++i) { float b = float(*data++)*scale; float g = float(*data++)*scale; float r = float(*data++)*scale; float a = float(*data++)*scale; operation.rgba(r,g,b,a); } }  break;
    }
}

template <class O>    
void readRow(unsigned int num, GLenum pixelFormat, GLenum dataType, const unsigned char* data, O& operation)
{
	if(num == 0)
		num = 1;
    switch(dataType)
    {
        case(GL_BYTE):              _readRow(num,pixelFormat, (const char*)data,            1.0f/128.0f,        operation); break;
        case(GL_UNSIGNED_BYTE):     _readRow(num,pixelFormat, (const unsigned char*)data,   1.0f/255.0f,        operation); break;
        case(GL_SHORT):             _readRow(num,pixelFormat, (const short*) data,          1.0f/32768.0f,      operation); break;
        case(GL_UNSIGNED_SHORT):    _readRow(num,pixelFormat, (const unsigned short*)data,  1.0f/65535.0f,      operation); break;
        case(GL_INT):               _readRow(num,pixelFormat, (const int*) data,            1.0f/2147483648.0f, operation); break;
        case(GL_UNSIGNED_INT):      _readRow(num,pixelFormat, (const unsigned int*) data,   1.0f/4294967295.0f, operation); break;
        case(GL_FLOAT):             _readRow(num,pixelFormat, (const float*) data,          1.0f,               operation); break;
    }
}

template <class O>    
void readImage(const CRCore::crImage* image, O& operation)
{
    if (!image) return;
    
    //for(int r=0;r<image->r();++r)
    //{
    //    for(int t=0;t<image->t();++t)
    //    {
    //        readRow(image->s(), image->getPixelFormat(), image->getDataType(), image->data(0,t,r), operation);
    //    }
    //}

	for(int s=0;s<image->s();++s)
	{
		for(int t=0;t<image->t();++t)
		{
			readRow(image->r(), image->getPixelFormat(), image->getDataType(), image->data(s,t,0), operation);
		}
	}
}

// example ModifyOperator
// struct ModifyOperator
// {
//     inline void luminance(float& l) const {} 
//     inline void alpha(float& a) const {} 
//     inline void luminance_alpha(float& l,float& a) const {} 
//     inline void rgb(float& r,float& g,float& b) const {}
//     inline void rgba(float& r,float& g,float& b,float& a) const {}
// };


template <typename T, class M>    
void _modifyRow(unsigned int num, GLenum pixelFormat, T* data,float scale, const M& operation)
{
    float inv_scale = 1.0f/scale;
    switch(pixelFormat)
    {
        case(GL_LUMINANCE):         { for(unsigned int i=0;i<num;++i) { float l = float(*data)*scale; operation.luminance(l); *data++ = T(l*inv_scale); } }  break;
        case(GL_ALPHA):             { for(unsigned int i=0;i<num;++i) { float a = float(*data)*scale; operation.alpha(a); *data++ = T(a*inv_scale); } }  break;
        case(GL_LUMINANCE_ALPHA):   { for(unsigned int i=0;i<num;++i) { float l = float(*data)*scale; float a = float(*(data+1))*scale; operation.luminance_alpha(l,a); *data++ = T(l*inv_scale); *data++ = T(a*inv_scale); } }  break;
        case(GL_RGB):               { for(unsigned int i=0;i<num;++i) { float r = float(*data)*scale; float g = float(*(data+1))*scale; float b = float(*(data+2))*scale; operation.rgb(r,g,b); *data++ = T(r*inv_scale); *data++ = T(g*inv_scale); *data++ = T(b*inv_scale); } }  break;
        case(GL_RGBA):              { for(unsigned int i=0;i<num;++i) { float r = float(*data)*scale; float g = float(*(data+1))*scale; float b = float(*(data+2))*scale; float a = float(*(data+3))*scale; operation.rgba(r,g,b,a); *data++ = T(r*inv_scale); *data++ = T(g*inv_scale); *data++ = T(b*inv_scale); *data++ = T(a*inv_scale); } }  break;
        case(GL_BGR):               { for(unsigned int i=0;i<num;++i) { float b = float(*data)*scale; float g = float(*(data+1))*scale; float r = float(*(data+2))*scale; operation.rgb(r,g,b); *data++ = T(b*inv_scale); *data++ = T(g*inv_scale); *data++ = T(r*inv_scale); } }  break;
        case(GL_BGRA):              { for(unsigned int i=0;i<num;++i) { float b = float(*data)*scale; float g = float(*(data+1))*scale; float r = float(*(data+2))*scale; float a = float(*(data+3))*scale; operation.rgba(r,g,b,a); *data++ = T(b*inv_scale); *data++ = T(g*inv_scale); *data++ = T(r*inv_scale); *data++ = T(a*inv_scale); } }  break;
    }
}

template <class M>    
void modifyRow(unsigned int num, GLenum pixelFormat, GLenum dataType, unsigned char* data, const M& operation)
{
	if(num == 0)
		num = 1;
    switch(dataType)
    {
        case(GL_BYTE):              _modifyRow(num,pixelFormat, (char*)data,            1.0f/128.0f,        operation); break;
        case(GL_UNSIGNED_BYTE):     _modifyRow(num,pixelFormat, (unsigned char*)data,   1.0f/255.0f,        operation); break;
        case(GL_SHORT):             _modifyRow(num,pixelFormat, (short*) data,          1.0f/32768.0f,      operation); break;
        case(GL_UNSIGNED_SHORT):    _modifyRow(num,pixelFormat, (unsigned short*)data,  1.0f/65535.0f,      operation); break;
        case(GL_INT):               _modifyRow(num,pixelFormat, (int*) data,            1.0f/2147483648.0f, operation); break;
        case(GL_UNSIGNED_INT):      _modifyRow(num,pixelFormat, (unsigned int*) data,   1.0f/4294967295.0f, operation); break;
        case(GL_FLOAT):             _modifyRow(num,pixelFormat, (float*) data,          1.0f,               operation); break;
    }
}

template <class M>    
void modifyImage(CRCore::crImage* image, const M& operation)
{
    if (!image) return;
    
    //for(int r=0;r<image->r();++r)
    //{
    //    for(int t=0;t<image->t();++t)
    //    {
    //        modifyRow(image->s(), image->getPixelFormat(), image->getDataType(), image->data(0,t,r), operation);
    //    }
    //}

	for(int s=0;s<image->s();++s)
	{
	    for(int t=0;t<image->t();++t)
	    {
	        modifyRow(image->r(), image->getPixelFormat(), image->getDataType(), image->data(s,t,0), operation);
	    }
	}
}

/** Compute the min max colour values in the image.*/
extern CR_EXPORT bool computeMinMax(const CRCore::crImage* image, CRCore::crVector4& min, CRCore::crVector4& max);

/** Compute the min max colour values in the image.*/
extern CR_EXPORT bool offsetAndScaleImage(CRCore::crImage* image, const CRCore::crVector4& offset, const CRCore::crVector4& scale);

/** Compute source image to destination image.*/
extern CR_EXPORT bool copyImage(const CRCore::crImage* srcImage, int src_s, int src_t, int src_r, int width, int height, int depth,
                                       CRCore::crImage* destImage, int dest_s, int dest_t, int dest_r, bool doRescale = false);

/** Compute the min max colour values in the image.*/
extern CR_EXPORT bool clearImageToColor(CRCore::crImage* image, const CRCore::crVector4& colour);

typedef std::vector< CRCore::ref_ptr<CRCore::crImage> > ImageList;

/** Search through the list of Images and find the maximum number of components used amoung the images.*/
extern CR_EXPORT unsigned int maximimNumOfComponents(const ImageList& imageList);

/** create a 3D CRCore::crImage from a list of CRCore::crImage.*/
extern CR_EXPORT CRCore::crImage* createImage3D(const ImageList& imageList,
            GLenum desiredPixelFormat,
            int s_maximumImageSize = 1024,
            int t_maximumImageSize = 1024,
            int r_maximumImageSize = 1024,
            bool resizeToPowerOfTwo = false);

/** create a 3D CRCore::crImage from a list of CRCore::crImage.*/
extern CR_EXPORT CRCore::crImage* createImage3DWithAlpha(const ImageList& imageList,
            int s_maximumImageSize = 1024,
            int t_maximumImageSize = 1024,
            int r_maximumImageSize = 1024,
            bool resizeToPowerOfTwo = false);

    
}


#endif
