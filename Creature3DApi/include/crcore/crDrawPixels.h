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
#ifndef CRCORE_DRAWPIXELS
#define CRCORE_DRAWPIXELS 1

#include <CRCore/crDrawable.h>
#include <CRCore/crVector3.h>
#include <CRCore/crImage.h>

namespace CRCore {

/** crDrawPixels is an CRCore::crDrawable subclass which encapsulates the drawing of
  * images using glDrawPixels.*/
class CR_EXPORT crDrawPixels : public crDrawable
{
    public:

        crDrawPixels();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crDrawPixels(const crDrawPixels& drawimage,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);
            
        virtual crBase* cloneType() const { return new crDrawPixels(); }

        virtual crBase* clone(const crCopyOp& copyop) const { return new crDrawPixels(*this,copyop); }
        
        virtual bool isSameKindAs(const crBase* obj) const { return dynamic_cast<const crDrawPixels*>(obj)!=NULL; }
        
        virtual const char* libraryName() const { return "CRCore"; }
        virtual const char* className() const { return "crDrawPixels"; }


        void setPosition(const CRCore::crVector3& position);
        
        CRCore::crVector3& getPosition() { return m_position; }
        const CRCore::crVector3& getPosition() const { return m_position; }

        void setImage(CRCore::crImage* image) { m_image = image; }
        
        CRCore::crImage* getImage() { return m_image.get(); }
        const CRCore::crImage* getImage() const { return m_image.get(); }
        
        void setUseSubImage(bool useSubImage) { m_useSubImage=useSubImage; }
        bool getUseSubImage() const { return m_useSubImage; }

        void setSubImageDimensions(unsigned int offsetX,unsigned int offsetY,unsigned int width,unsigned int height);
        void getSubImageDimensions(unsigned int& offsetX,unsigned int& offsetY,unsigned int& width,unsigned int& height) const;

        virtual void drawImplementation(crState& state) const;

		enum ScaleWithScreen
		{
			Scale_None,
			Scale_Width,
			Scale_Height,
			Scale_Both
		};

		void setScaleWithScreen(ScaleWithScreen scaleWithScreen);
		ScaleWithScreen getScaleWithScreen();

    protected:

        crDrawPixels& operator = (const crDrawPixels&) { return *this;}

		/** compute the bounding box of the drawable. Method must be 
		implemented by subclasses.*/
		virtual bool computeBound() const;

        virtual ~crDrawPixels();

        crVector3            m_position;
        mutable ref_ptr<crImage>  m_image;
        
        bool            m_useSubImage;
        unsigned int    m_offsetX, m_offsetY, m_width, m_height;
		mutable ScaleWithScreen m_scaleWithScreen;
};

}

#endif
