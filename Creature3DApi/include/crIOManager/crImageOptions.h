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
#ifndef CRIOMANAGER_IMAGEOPTIONS
#define CRIOMANAGER_IMAGEOPTIONS 1

#include <CRIOManager/crReaderWriter.h>

namespace CRIOManager {

class CRIOMANAGER_EXPORT crImageOptions : public CRIOManager::crReaderWriter::Options
{
    public:
    
        crImageOptions();
        
        crImageOptions(const std::string& str);
            
        crImageOptions(const crImageOptions& options,const CRCore::crCopyOp copyop=CRCore::crCopyOp::SHALLOW_COPY):
            CRIOManager::crReaderWriter::Options(options,copyop),
            m_sourceImageSamplingMode(options.m_sourceImageSamplingMode),
            m_sourceImageWindowMode(options.m_sourceImageWindowMode),
            m_sourceRatioWindow(options.m_sourceRatioWindow),
            m_sourcePixelWindow(options.m_sourcePixelWindow),
            m_destinationImage(options.m_destinationImage),
            m_destinationImageWindowMode(options.m_destinationImageWindowMode),
            m_destinationRatioWindow(options.m_destinationRatioWindow),
            m_destinationPixelWindow(options.m_destinationPixelWindow),
            m_destinationDataType(options.m_destinationDataType),
            m_destinationPixelFormat(options.m_destinationPixelFormat) {}
          

        META_Base(CRIOManager,crImageOptions);

        /** RatioWindow stores the window (as ratios of 0.0 to 1.0) from the overall imagery from which to extract the CRCore::crImage*/
        struct RatioWindow
        {
            RatioWindow():
                windowX(0.0),
                windowY(0.0),
                windowWidth(1.0),
                windowHeight(1.0) {}

            void set(double x, double y, double w, double h)
            {
                windowX = x;
                windowY = y;
                windowWidth = w;
                windowHeight = h;
            }
            
            double windowX;
            double windowY;
            double windowWidth;
            double windowHeight;
        };
        
        /** PixelWindow stores the window (in exact pixels) from the overall imagery from which to extract the CRCore::crImage*/
        struct PixelWindow
        {
            PixelWindow():
                windowX(0),
                windowY(0),
                windowWidth(0),
                windowHeight(0) {}
        
            void set(unsigned int x, unsigned int y, unsigned int w, unsigned int h)
            {
                windowX = x;
                windowY = y;
                windowWidth = w;
                windowHeight = h;
            }
            
            unsigned int windowX;
            unsigned int windowY;
            unsigned int windowWidth;
            unsigned int windowHeight;
        };
        
        enum ImageWindowMode
        {
            ALL_IMAGE,
            RATIO_WINDOW,
            PIXEL_WINDOW
        };
        
        enum ImageSamplingMode
        {
            NEAREST,
            LINEAR,
            CUBIC
        };

        /** Used as UserData attached to generated CRCore::crImage's*/
        struct TexCoordRange : public CRCore::Referenced
        {
            TexCoordRange():
                _x(0.0),
                _y(0.0),
                _w(1.0),
                _h(1.0) {}
        
            void set(double x,double y, double w, double h)
            {
                _x = x;
                _y = y;
                _w = w;
                _h = h;
            }
        
            double _x,_y,_w,_h;
        };


        // source        
        ImageSamplingMode   m_sourceImageSamplingMode;
        ImageWindowMode     m_sourceImageWindowMode;
        RatioWindow         m_sourceRatioWindow;
        PixelWindow         m_sourcePixelWindow;

        // destination
        CRCore::ref_ptr<CRCore::crImage> m_destinationImage;
       
        ImageWindowMode     m_destinationImageWindowMode;
        RatioWindow         m_destinationRatioWindow;
        PixelWindow         m_destinationPixelWindow;

        GLenum              m_destinationDataType;
        GLenum              m_destinationPixelFormat;

        void init();
        
};


}

#endif // CRIOMANAGER_IMAGEOPTIONS
