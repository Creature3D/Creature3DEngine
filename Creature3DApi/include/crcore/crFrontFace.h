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
#ifndef CRCORE_CRFRONTFACE_H
#define CRCORE_CRFRONTFACE_H 1

#include <CRCore/crStateAttribute.h>

namespace CRCore {

/** Class to specifies the orientation of front-facing polygons.
*/     
class CR_EXPORT crFrontFace : public crStateAttribute
{
    public :
    
        crFrontFace();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crFrontFace(const crFrontFace& ff,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(ff,copyop),
            m_mode(ff.m_mode) {}

        META_StateAttribute(CRCore, crFrontFace, FRONTFACE);
        
        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crFrontFace,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_mode)

            return 0; // passed all the above comparison macro's, must be equal.
        }

		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crFrontFace,sa)

			// compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_mode)

			return 0; // passed all the above comparison macro's, must be equal.
		}

        enum Mode {
            CLOCKWISE,
            COUNTER_CLOCKWISE
        };
    
        inline void setMode(Mode mode) { m_mode = mode; }
        inline const Mode getMode() const    { return m_mode; }
        
		virtual void addAttributeParam(int i, const std::string &str);
		virtual void apply(crState& state) const;
    protected:
    
        virtual ~crFrontFace();
        
        Mode m_mode;

};

}

#endif
