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
#ifndef CRCORE_CRLINESTIPPLE_H
#define CRCORE_CRLINESTIPPLE_H 1

#include <CRCore/crStateAttribute.h>

namespace CRCore {

class CR_EXPORT crLineStipple : public crStateAttribute
{
    public :

        crLineStipple();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crLineStipple(const crLineStipple& lw,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
          crStateAttribute(lw,copyop),
          m_factor(lw.m_factor),
          m_pattern(lw.m_pattern) {}

        META_StateAttribute(CRCore, crLineStipple, LINESTIPPLE);

        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crLineStipple,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_factor)
            COMPARE_StateAttribute_Parameter(m_pattern)

            return 0; // passed all the above comparison macro's, must be equal.
        }

		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crLineStipple,sa)

			// compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_factor)
			COMPARE_StateAttribute_Parameter(m_pattern)

			return 0; // passed all the above comparison macro's, must be equal.
		}

 /*       virtual bool getModeUsage(ModeUsage& usage) const
        {
            usage.usesMode(GL_LINE_STIPPLE);
            return true;
        }*/

		virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
		{
			// Have to think about the role of m_colorMode
			// in setting the colormaterial... also need to take the
			// color material enable/disable out of the the apply()...
			modes.push_back(GL_LINE_STIPPLE);
		}

        void setFactor(int factor);
        inline int getFactor() const { return m_factor; }

        void setPattern(unsigned short pattern);
        inline unsigned short getPattern() const { return m_pattern; }

        virtual void apply(crState& state) const;
    protected :

        virtual ~crLineStipple();

        int             m_factor;
        unsigned short	m_pattern;

};

}

#endif
