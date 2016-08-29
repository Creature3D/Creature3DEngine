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
#ifndef CRCORE_CRCOLORMASK_H
#define CRCORE_CRCOLORMASK_H 1

#include <CRCore/crExport.h>
#include <CRCore/crStateAttribute.h>

namespace CRCore {

/** Encapsulate OpenGL glColorMaskFunc/Op/Mask functions.
*/     
class CR_EXPORT crColorMask : public crStateAttribute
{
    public :
    
    
        crColorMask();

        crColorMask(bool red,bool green,bool blue,bool alpha):
            m_red(red),
            m_green(green),
            m_blue(blue),
            m_alpha(alpha) {}
            

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crColorMask(const crColorMask& cm,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(cm,copyop),
            m_red(cm.m_red),
            m_green(cm.m_green),
            m_blue(cm.m_blue),
            m_alpha(cm.m_alpha) {}
        
        META_StateAttribute(CRCore, crColorMask, COLORMASK);
        
        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crColorMask,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_red)
            COMPARE_StateAttribute_Parameter(m_green)
            COMPARE_StateAttribute_Parameter(m_blue)
            COMPARE_StateAttribute_Parameter(m_alpha)

            return 0; // passed all the above comparison macro's, must be equal.
        }

		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crColorMask,sa)

				// compare each paramter in turn against the rhs.
				COMPARE_StateAttribute_Parameter(m_red)
				COMPARE_StateAttribute_Parameter(m_green)
				COMPARE_StateAttribute_Parameter(m_blue)
				COMPARE_StateAttribute_Parameter(m_alpha)

				return 0; // passed all the above comparison macro's, must be equal.
		}

        inline void setMask(bool red,bool green,bool blue,bool alpha)
        {
            m_red = red;
            m_green = green;
            m_blue = blue;
            m_alpha = alpha;
            
        }
        
        inline bool getRedMask() const { return m_red; }

        inline bool getGreenMask() const { return m_green; }

        inline bool getBlueMask() const { return m_blue; }
        
        inline bool getAlphaMask() const { return m_alpha; }

        virtual void apply(crState& state) const;
		virtual void addAttributeParam(int i, const std::string &str);
    protected:
    
        virtual ~crColorMask();

        bool                m_red;
        bool                m_green;
        bool                m_blue;
        bool                m_alpha;

};

}

#endif
