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
#ifndef CRCORE_STENCIL_H
#define CRCORE_STENCIL_H 1

#include <CRCore/crStateAttribute.h>

namespace CRCore {

/** Encapsulate OpenGL glStencilFunc/Op/Mask functions.
*/     
class CR_EXPORT crStencil : public crStateAttribute
{
    public :
    
    
        crStencil();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crStencil(const crStencil& stencil,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(stencil,copyop),
            m_func(stencil.m_func),
            m_funcRef(stencil.m_funcRef),
            m_funcMask(stencil.m_funcMask),
            m_sfail(stencil.m_sfail),
            m_zfail(stencil.m_zfail),
            m_zpass(stencil.m_zpass),
            m_writeMask(stencil.m_writeMask) {}


        META_StateAttribute(CRCore, crStencil, STENCIL);
        
        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crStencil,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_func)
            COMPARE_StateAttribute_Parameter(m_funcRef)
            COMPARE_StateAttribute_Parameter(m_funcMask)
            COMPARE_StateAttribute_Parameter(m_sfail)
            COMPARE_StateAttribute_Parameter(m_zfail)
            COMPARE_StateAttribute_Parameter(m_zpass)
            COMPARE_StateAttribute_Parameter(m_writeMask)

            return 0; // passed all the above comparison macro's, must be equal.
        }
		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crStencil,sa)

			// compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_func)
			COMPARE_StateAttribute_Parameter(m_funcRef)
			COMPARE_StateAttribute_Parameter(m_funcMask)
			COMPARE_StateAttribute_Parameter(m_sfail)
			COMPARE_StateAttribute_Parameter(m_zfail)
			COMPARE_StateAttribute_Parameter(m_zpass)
			COMPARE_StateAttribute_Parameter(m_writeMask)

			return 0; // passed all the above comparison macro's, must be equal.
		}
        //virtual bool getModeUsage(ModeUsage& usage) const
        //{
        //    usage.usesMode(GL_STENCIL_TEST);
        //    return true;
        //}
		virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
		{
			modes.push_back(GL_STENCIL_TEST);
		}

        enum Function
        {
            NEVER = GL_NEVER,
            LESS = GL_LESS,
            EQUAL = GL_EQUAL,
            LEQUAL = GL_LEQUAL,
            GREATER = GL_GREATER,
            NOTEQUAL = GL_NOTEQUAL,
            GEQUAL = GL_GEQUAL,
            ALWAYS = GL_ALWAYS
        };

        inline void setFunction(Function func,int ref,unsigned int mask)
        {
            m_func = func;
            m_funcRef = ref;
            m_funcMask = mask;
        }
        
        inline Function getFunction() const { return m_func; }
        
        inline int getFunctionRef() const { return m_funcRef; }

        inline unsigned int getFunctionMask() const { return m_funcMask; }
        
        
        enum Operation 
        {
            KEEP = GL_KEEP,
            ZERO = GL_ZERO,
            REPLACE = GL_REPLACE,
            INCR = GL_INCR,
            DECR = GL_DECR,
            INVERT = GL_INVERT
        };
        
        /** set the operations to apply when the various stencil and depth 
          * tests fail or pass.  First parameter is to control the operation
          * when the stencil test fails.  The second parameter is to control the
          * operation when the stencil test passes, but depth test fails. The
          * third parameter controls the operation when both the stencil test
          * and depth pass.  Ordering of parameter is the same as if using
          * glStencilOp(,,).*/
        inline void setOperation(Operation sfail, Operation zfail, Operation zpass)
        {
            m_sfail = sfail;
            m_zfail = zfail;
            m_zpass = zpass;
        }
        
		/** set the operation when the stencil test fails.*/
		inline void setStencilFailOperation(Operation sfail) { m_sfail = sfail; }

		/** get the operation when the stencil test fails.*/
		inline Operation getStencilFailOperation() const { return m_sfail; }

		/** set the operation when the stencil test passes but the depth test fails.*/
		inline void setStencilPassAndDepthFailOperation(Operation zfail) { m_zfail=zfail; }

		/** get the operation when the stencil test passes but the depth test fails.*/
		inline Operation getStencilPassAndDepthFailOperation() const { return m_zfail; }

		/** set the operation when both the stencil test and the depth test pass.*/
		inline void setStencilPassAndDepthPassOperation(Operation zpass) { m_zpass=zpass; }

		/** get the operation when both the stencil test and the depth test pass.*/
		inline Operation getStencilPassAndDepthPassOperation() const { return m_zpass; }

        inline void setWriteMask(unsigned int mask) { m_writeMask = mask; }
        
        inline unsigned int getWriteMask() const { return m_writeMask; }

        virtual void apply(crState& state) const;
		virtual void addAttributeParam(int i, const std::string &str);
    protected:
    
        virtual ~crStencil();

        Function            m_func;
        int                 m_funcRef;
        unsigned int        m_funcMask;
        
        Operation           m_sfail;
        Operation           m_zfail;
        Operation           m_zpass;
        
        unsigned int        m_writeMask;

};

}

#endif
