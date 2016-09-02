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
#ifndef CRCORE_CRHINTFUNC_H
#define CRCORE_CRHINTFUNC_H 1

#include <CRCore/crStateAttribute.h>

namespace CRCore {

class CR_EXPORT crHint : public crStateAttribute
{
    public :

        crHint();
        
        crHint(unsigned int hintTarget, unsigned int hintMode);

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crHint(const crHint& trans,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(trans,copyop),
            m_hintTarget(trans.m_hintTarget),
            m_hintMode(trans.m_hintMode) {}

        META_StateAttribute(CRCore, crHint,HINT);
        
        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crHint,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_hintTarget)
            COMPARE_StateAttribute_Parameter(m_hintMode)

            return 0; // passed all the above comparison macro's, must be equal.
        }

		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crHint,sa)

			// compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_hintTarget)
			COMPARE_StateAttribute_Parameter(m_hintMode)

			return 0; // passed all the above comparison macro's, must be equal.
		}
   /*     virtual bool getModeUsage(ModeUsage& usage) const
        {
            usage.usesMode(GL_BLEND);
            return true;
        }*/

		enum HintTarget 
		{
			PERSPECTIVE_CORRECTION_HINT,
			POINT_SMOOTH_HINT,
			LINE_SMOOTH_HINT,
			POLYGON_SMOOTH_HINT,
			FOG_HINT,
			PHONG_HINT,
			FRAGMENT_SHADER_DERIVATIVE_HINT
		};

		enum HintMode 
		{
			DONT_CARE,
			FASTEST,
			NICEST
		};

		virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
		{
			switch(m_hintTarget) 
			{
			case POINT_SMOOTH_HINT:
				modes.push_back(GL_POINT_SMOOTH);
				break;
			case LINE_SMOOTH_HINT:
				modes.push_back(GL_LINE_SMOOTH);
				break;
			case POLYGON_SMOOTH_HINT:
				modes.push_back(GL_POLYGON_SMOOTH);
				break;
			}
		}
        
        inline void setHintTarget(unsigned int hintTarget) { m_hintTarget = hintTarget; }
        inline unsigned int getHintTarget() const { return m_hintTarget; }

		inline void setHintMode(unsigned int hintMode) { m_hintMode = hintMode; }
		inline unsigned int getHintMode() const { return m_hintMode; }

        virtual void apply(crState& state) const;
		virtual void addAttributeParam(int i, const std::string &str);
    protected :

        virtual ~crHint();

        unsigned int m_hintTarget;
		unsigned int m_hintMode;

};

}

#endif
