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
#ifndef CRCORE_TRANSFERFUNCTION
#define CRCORE_TRANSFERFUNCTION 1

#include <CRCore/crTexture.h>
#include <CRCore/crShader.h>

#include <map>

namespace CRCore {


/** crTransferFunction is a class that provide a 1D,2D or 3D colour look up table
  * that can be used on the GPU as a 1D, 2D or 3D texture.  
  * Typically uses include mapping heights to colours when contouring terrain, 
  * or mapping intensities to colours when volume rendering.
*/
class CR_EXPORT crTransferFunction : public CRCore::crBase
{
    public :
    
        crTransferFunction();

       /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crTransferFunction(const crTransferFunction& tf, const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_Base(CRCore, crTransferFunction)

        /** Get the image that is used for passing the transfer function data to the GPU.*/
        CRCore::crImage* getImage() { return m_image.get(); }

        /** Get the const image that is used for passing the transfer function data to the GPU.*/
        const CRCore::crImage* getImage() const { return m_image.get(); }
        
    protected:
    
        virtual ~crTransferFunction();

        CRCore::ref_ptr<CRCore::crImage>    m_image;
};

/** 1D variant of crTransferFunction. */
class CR_EXPORT crTransferFunction1D : public CRCore::crTransferFunction
{
    public:
    
        crTransferFunction1D();
    
       /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crTransferFunction1D(const crTransferFunction1D& tf, const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_Base(CRCore, crTransferFunction1D)
        
        /** Get the mnimum transfer function value.*/
        float getMinimum() const { return m_colorMap.empty() ? 0.0f : m_colorMap.begin()->first; }
        
        /** Get the maximum transfer function value.*/
        float getMaximum() const { return m_colorMap.empty() ? 0.0f : m_colorMap.rbegin()->first; }

        /** allocate the CRCore::crImage with specified dimension.  The crImage tracks the color map, and is used to represent the
          * transfer function when download to GPU.*/
        void allocate(unsigned int numImageCells);

        /** Clear the whole range to just represet a single color.*/
        void clear(const CRCore::crVector4& color = CRCore::crVector4(1.0f,1.0f,1.0f,1.0f));
        
        /** Get pixel value from the image. */
        CRCore::crVector4 getPixelValue(unsigned int i) const
        {
            if (m_image.valid() && i<static_cast<unsigned int>(m_image->s()))
            {   
                return *reinterpret_cast<const CRCore::crVector4*>(m_image->data(i));
            }
            else
            {
                return CRCore::crVector4(1.0f,1.0f,1.0f,1.0f);
            }
        }

        /** Get the number of image cells that are assigned to the represent the transfer function when download to the GPU.*/
        unsigned int getNumberImageCells() const { return m_image.valid() ? m_image->s() : 0; }

        /** Set the color for a specified transfer function value.
          * updateImage defaults to true, and tells the setColor function to update the associate CRCore::crImage that
          * tracks the color map.  Pass in false as the updateImage parameter if you are setting up many values
          * at once to avoid recomputating og the image data, then once all setColor calls are made explictly call
          * updateImage() to bring the CRCore::crImage back into sync with the color map. */ 
        void setColor(float v, const CRCore::crVector4& color, bool updateImage=true);
        
        /** Get the color for a specified transfer function value, interpolating the value if no exact match is found.*/
        CRCore::crVector4 getColor(float v) const;
        
        typedef std::map<float, CRCore::crVector4> ColorMap;
        
        /** Get the color map that stores the mapping between the the tranfser function value and the colour it maps to.*/
        ColorMap& getColorMap() { return m_colorMap; }
        
        /** Get the const color map that stores the mapping between the the tranfser function value and the colour it maps to.*/
        const ColorMap& getColorMap() const { return m_colorMap; }

        /** Assign a color map and automatically update the image to make sure they are in sync.*/
        void assign(const ColorMap& vcm);
        
        /** Manually update the associate CRCore::crImage to represent the colors assigned in the color map.*/
        void updateImage();

    protected:
    
        ColorMap m_colorMap;
        
        void assignToImage(float lower_v, const CRCore::crVector4& lower_c, float upper_v, const CRCore::crVector4& upper_c);
};

}

#endif
