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
#ifndef CRCORE_CRSCISSOR
#define CRCORE_CRSCISSOR 1

#include <CRCore/crStateAttribute.h>

namespace CRCore {

/** Encapsulate OpenGL glScissor. */     
class CR_EXPORT crScissor : public crStateAttribute
{
    public :
        crScissor();
        
        crScissor(int x,int y,int width,int height):
            m_x(x),
            m_y(y),
            m_width(width),
            m_height(height) {}
            

        /** Copy constructor using crCopyOp to manage deep vs shallow copy. */
        crScissor(const crScissor& vp,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(vp,copyop),
            m_x(vp.m_x),
            m_y(vp.m_y),
            m_width(vp.m_width),
            m_height(vp.m_height) {}

        META_StateAttribute(CRCore, crScissor, SCISSOR);
        
        /** Return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs. */
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crScissor,sa)

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
			COMPARE_StateAttribute_Types(crScissor,sa)

			// compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_x)
			COMPARE_StateAttribute_Parameter(m_y)
			COMPARE_StateAttribute_Parameter(m_width)
			COMPARE_StateAttribute_Parameter(m_height)

			return 0; // passed all the above comparison macro's, must be equal.
		}

		virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
		{
			// Have to think about the role of m_colorMode
			// in setting the colormaterial... also need to take the
			// color material enable/disable out of the the apply()...
			modes.push_back(GL_SCISSOR_TEST);
		}

        inline void setScissor(int x,int y,int width,int height)
        {
            m_x = x;
            m_y = y;
            m_width = width;
            m_height = height;
        }
        
        void getScissor(int& x,int& y,int& width,int& height) const
        {
            x = m_x;
            y = m_y;
            width = m_width;
            height = m_height;
        }

        inline int& x() { return m_x; }
        inline int x() const { return m_x; }
        
        inline int& y() { return m_y; }
        inline int y() const { return m_y; }

        inline int& width() { return m_width; }
        inline int width() const { return m_width; }

        inline int& height() { return m_height; }
        inline int height() const { return m_height; }

        virtual void apply(crState& state) const;
		virtual void addAttributeParam(int i, const std::string &str);
    protected:
    
        virtual ~crScissor();

        int m_x;
        int m_y;
        int m_width;
        int m_height;

};

}

#endif
