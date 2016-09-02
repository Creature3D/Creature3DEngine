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

#ifndef CRCORE_CRBLENDEQUATION
#define CRCORE_CRBLENDEQUATION 1

#include <CRCore/crStateAttribute.h>

namespace CRCore {

/** Encapsulates OpenGL crBlendEquation state. */
class CR_EXPORT crBlendEquation : public crStateAttribute
{
    public :

        enum Equation {
            RGBA_MIN,
            RGBA_MAX,
            ALPHA_MIN,
            ALPHA_MAX,
            LOGIC_OP,
            FUNC_ADD,
            FUNC_SUBTRACT,
            FUNC_REVERSE_SUBTRACT
        };

        crBlendEquation();
        
        crBlendEquation(Equation equation);

        /** Copy constructor using crCopyOp to manage deep vs shallow copy. */
        crBlendEquation(const crBlendEquation& trans,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(trans,copyop),
            m_equation(trans.m_equation){}

        META_StateAttribute(CRCore, crBlendEquation,BLENDEQUATION);
        
        /** Return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs. */
        virtual int compare(const crStateAttribute& sa) const
        {
            // Check for equal types, then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macros below.
            COMPARE_StateAttribute_Types(crBlendEquation,sa)

            // Compare each parameter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_equation)

            return 0; // Passed all the above comparison macros, so must be equal.
        }

		virtual int compare(const crStateAttribute& sa)
		{
			// Check for equal types, then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macros below.
			COMPARE_StateAttribute_Types(crBlendEquation,sa)

			// Compare each parameter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_equation)

			return 0; // Passed all the above comparison macros, so must be equal.
		}

        //virtual bool getModeUsage(ModeUsage& usage) const
        //{
        //    usage.usesMode(GL_BLEND);
        //    return true;
        //}
		virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
		{
			modes.push_back(GL_BLEND);
		}

        inline void setEquation(Equation equation)
        {
            m_equation = equation;
        }

        inline Equation getEquation() const { return m_equation; }
		virtual void addAttributeParam(int i, const std::string &str);
		virtual void apply(crState& state) const;

		class Extensions : public Referenced
		{
		public:
			Extensions(unsigned int contextID);

			Extensions(const Extensions& rhs);

			void lowestCommonDenominator(const Extensions& rhs);

			void setupGLExtensions(unsigned int contextID);

			bool isBlendEquationSupported() const { return _isBlendEquationSupported; }
			bool isBlendEquationSeparateSupported() const { return _isBlendEquationSeparateSupported; }
			bool isSGIXMinMaxSupported() const { return _isSGIXMinMaxSupported; }
			bool isLogicOpSupported() const { return _isLogicOpSupported; }

			void glBlendEquation(GLenum mode) const;
			void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha) const;
			static void clearExtensions();
		protected:

			~Extensions() {}

			typedef void (APIENTRY * GLBlendEquationProc)(GLenum mode);
			typedef void (APIENTRY * GLBlendEquationSeparateProc)(GLenum modeRGB, GLenum modeAlpha);

			bool                _isBlendEquationSupported;
			bool                _isBlendEquationSeparateSupported;
			bool                _isSGIXMinMaxSupported;
			bool                _isLogicOpSupported;

			GLBlendEquationProc _glBlendEquation;
			GLBlendEquationSeparateProc _glBlendEquationSeparate;
		};
		static Extensions* getExtensions(unsigned int contextID,bool createIfNotInitalized);
		static void setExtensions(unsigned int contextID,Extensions* extensions);
    protected :
        virtual ~crBlendEquation();
        Equation m_equation;
};

}

#endif
