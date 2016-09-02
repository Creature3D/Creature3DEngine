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
#ifndef CRCORE_CRLINEWIDTH_H
#define CRCORE_CRLINEWIDTH_H 1

#include <CRCore/crStateAttribute.h>

namespace CRCore {

/** crLineWidth - encapsulates the OpenGL glLineWidth for setting the width of lines in pixels.*/
class CR_EXPORT crLineWidth : public crStateAttribute
{
    public :

        crLineWidth(float width=1.0f);

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crLineWidth(const crLineWidth& lw,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(lw,copyop),
            m_width(lw.m_width) {}
            
        META_StateAttribute(CRCore, crLineWidth, LINEWIDTH);
        
        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crLineWidth,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_width)

            return 0; // passed all the above comparison macro's, must be equal.
        }

		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crLineWidth,sa)

			// compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_width)

			return 0; // passed all the above comparison macro's, must be equal.
		}

        void setWidth(float width);
        
        inline float getWidth() const { return m_width; }

        virtual void apply(crState& state) const;

    protected :

        virtual ~crLineWidth();

        float       m_width;

};

}

#endif
