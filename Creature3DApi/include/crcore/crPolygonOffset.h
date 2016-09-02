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
#ifndef CRCORE_POLYGONOFFSET
#define CRCORE_POLYGONOFFSET 1

#include <CRCore/crStateAttribute.h>

namespace CRCore {

/** crPolygonOffset - encapsulates the OpenGL glPolygonOffset state.*/
class CR_EXPORT crPolygonOffset : public crStateAttribute
{
    public :

        crPolygonOffset();

        crPolygonOffset(float factor, float units);

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crPolygonOffset(const crPolygonOffset& po,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(po,copyop),
            m_factor(po.m_factor),
            m_units(po.m_units) {}

        META_StateAttribute(CRCore, crPolygonOffset, POLYGONOFFSET);

        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crPolygonOffset,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_factor)
            COMPARE_StateAttribute_Parameter(m_units)

            return 0; // passed all the above comparison macro's, must be equal.
        }

		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crPolygonOffset,sa)

				// compare each paramter in turn against the rhs.
				COMPARE_StateAttribute_Parameter(m_factor)
				COMPARE_StateAttribute_Parameter(m_units)

				return 0; // passed all the above comparison macro's, must be equal.
		}

		virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
		{
			// Have to think about the role of m_colorMode
			// in setting the colormaterial... also need to take the
			// color material enable/disable out of the the apply()...
			modes.push_back(GL_POLYGON_OFFSET_FILL);
			modes.push_back(GL_POLYGON_OFFSET_LINE);
			modes.push_back(GL_POLYGON_OFFSET_POINT);
		}

        inline void setFactor(float factor) { m_factor = factor; }
        inline float getFactor() const { return m_factor; }
        
        inline void setUnits(float units) { m_units = units; }
        inline float getUnits() const { return m_units; }

        virtual void apply(crState& state) const;
		virtual void addAttributeParam(int i, const std::string &str);
    protected :

        virtual ~crPolygonOffset();

        float       m_factor;
        float       m_units;

};

}

#endif
