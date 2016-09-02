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
#ifndef CRCORE_CRVIEWPORT_H
#define CRCORE_CRVIEWPORT_H 1

#include <CRCore/crStateAttribute.h>
#include <CRCore/crMatrix.h>

namespace CRCore {

/** Encapsulte OpenGL glViewport.
*/     
class CR_EXPORT crViewPort : public crStateAttribute
{
    public :
    
    
        crViewPort();
        
        crViewPort(int x,int y,int width,int height):
            m_x(x),
            m_y(y),
            m_width(width),
            m_height(height) {}
            

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crViewPort(const crViewPort& vp,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(vp,copyop),
            m_x(vp.m_x),
            m_y(vp.m_y),
            m_width(vp.m_width),
            m_height(vp.m_height) {}

        META_StateAttribute(CRCore, crViewPort,VIEWPORT);
        
        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crViewPort,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_x)
            COMPARE_StateAttribute_Parameter(m_y)
            COMPARE_StateAttribute_Parameter(m_width)
            COMPARE_StateAttribute_Parameter(m_height)
            
            return 0; // passed all the above comparison macro's, must be equal.
        }

		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crViewPort,sa)

			// compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_x)
			COMPARE_StateAttribute_Parameter(m_y)
			COMPARE_StateAttribute_Parameter(m_width)
			COMPARE_StateAttribute_Parameter(m_height)

			return 0; // passed all the above comparison macro's, must be equal.
		}

        inline void setViewport(int x,int y,int width,int height)
        {
            m_x = x;
            m_y = y;
            m_width = width;
            m_height = height;
        }
        
        void getViewport(int& x,int& y,int& width,int& height) const
        {
            x = m_x;
            y = m_y;
            width = m_width;
            height = m_height;
        }

        inline int x() const { return m_x; }
        inline int y() const { return m_y; }
        inline int width() const { return m_width; }
        inline int height() const { return m_height; }

        inline bool valid() const { return m_width!=0 && m_height!=0; }

        /** Return the aspcetRatio of the viewport, which is equal to width/height.
          * If height is zero, the potental division by zero is avoid by simply returning 1.0f.*/
        inline float aspectRatio() const { if (m_height!=0) return (float)m_width/(float)m_height; else return 1.0f; }
        
        /** Compute the Window crMatrix which takes projected coords into Window coordinates.
          * To converted local coodinates into window coordinates use v_window = v_local * MVPW matrix, 
          * where the MVPW matrix is ModelViewMatrix * ProjectionMatrix * WindowMatrix, the later supplied by 
          * viewport::computeWindowMatrix(), the ModelView and Projection crMatrix can either be sourced from the
          * current CRCore::crState object, via osgUtil::SceneView or CullVisitor.*/
        inline const CRCore::crMatrix computeWindowMatrix() const
        {
            return CRCore::crMatrix::translate(1.0f,1.0f,1.0f)*CRCore::crMatrix::scale(0.5f*width(),0.5f*height(),0.5f);
        }
        virtual void apply(crState& state) const;
		virtual void addAttributeParam(int i, const std::string &str);

    protected:
    
        virtual ~crViewPort();

        int m_x;
        int m_y;
        int m_width;
        int m_height;

};

}

#endif
