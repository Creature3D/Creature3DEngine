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
#ifndef CRCORE_CRLOGICOP
#define CRCORE_CRLOGICOP 1

#include <CRCore/crStateAttribute.h>

namespace CRCore {

/** Encapsulates OpenGL crLogicOp state. */
class CR_EXPORT crLogicOp : public crStateAttribute
{
    public :

        enum Opcode {
            CLEAR,
            SET,
            COPY,
            COPY_INVERTED,
            NOOP,
            INVERT,
            AND,
            NAND,
            OR,
            NOR,
            XOR,
            EQUIV,
            AND_REVERSE,
            AND_INVERTED,
            OR_REVERSE,
            OR_INVERTED
        };

        crLogicOp();
        
        crLogicOp(Opcode opcode);

        /** Copy constructor using crCopyOp to manage deep vs shallow copy. */
        crLogicOp(const crLogicOp& trans,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(trans,copyop),
            m_opcode(trans.m_opcode){}

        META_StateAttribute(CRCore, crLogicOp,LOGICOP);
        
        /** Return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs. */
        virtual int compare(const crStateAttribute& sa) const
        {
            // Check for equal types, then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macros below.
            COMPARE_StateAttribute_Types(crLogicOp,sa)

            // Compare each parameter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_opcode)

            return 0; // Passed all the above comparison macros, so must be equal.
        }

		virtual int compare(const crStateAttribute& sa)
		{
			// Check for equal types, then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macros below.
			COMPARE_StateAttribute_Types(crLogicOp,sa)

			// Compare each parameter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_opcode)

			return 0; // Passed all the above comparison macros, so must be equal.
		}

		virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
		{
			modes.push_back(GL_COLOR_LOGIC_OP);
		}

        inline void setOpcode(Opcode opcode)
        {
            m_opcode = opcode;
        }

        inline Opcode getOpcode() const { return m_opcode; }
        
		virtual void addAttributeParam(int i, const std::string &str);
		virtual void apply(crState& state) const;
    protected :

        virtual ~crLogicOp();

        Opcode m_opcode;
};

}

#endif
