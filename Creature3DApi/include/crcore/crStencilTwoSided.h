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
#ifndef CRCORE_CRSTENCILTWOSIDED_H
#define CRCORE_CRSTENCILTWOSIDED_H 1

#include <CRCore/crStencil.h>

namespace CRCore {

#ifndef GL_STENCIL_TEST_TWO_SIDE
#define GL_STENCIL_TEST_TWO_SIDE 0x8910
#endif

/** Encapsulate OpenGL two sided glStencilFunc/Op/Mask functions.
*/     
class CR_EXPORT crStencilTwoSided : public crStateAttribute
{
    public :
    
    
        crStencilTwoSided();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crStencilTwoSided(const crStencilTwoSided& stencil,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_StateAttribute(CRCore, crStencilTwoSided, STENCIL);
        
        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const;
		virtual int compare(const crStateAttribute& sa);

		virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
		{
			modes.push_back(GL_STENCIL_TEST);
			modes.push_back(GL_STENCIL_TEST_TWO_SIDE);
		}

  //      virtual bool getModeUsage(crStateAttribute::ModeUsage& usage) const
  //      {
  //          usage.usesMode(GL_STENCIL_TEST);
  //          usage.usesMode(GL_STENCIL_TEST_TWO_SIDE);
  //          return true;
		//}

        enum Face
        {
            FRONT = 0,
            BACK = 1
        };

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

        inline void setFunction(Face face, Function func,int ref,unsigned int mask)
        {
            m_func[face] = func;
            m_funcRef[face] = ref;
            m_funcMask[face] = mask;
        }
        
        inline void setFunction(Face face, Function func) { m_func[face] = func; }
        inline Function getFunction(Face face) const { return m_func[face]; }
        
        inline void setFunctionRef(Face face, int ref) { m_funcRef[face]=ref; }
        inline int getFunctionRef(Face face) const { return m_funcRef[face]; }

        inline void setFunctionMask(Face face, unsigned int mask) { m_funcMask[face]=mask; }
        inline unsigned int getFunctionMask(Face face) const { return m_funcMask[face]; }
        
        
        enum Operation 
        {
            KEEP = GL_KEEP,
            ZERO = GL_ZERO,
            REPLACE = GL_REPLACE,
            INCR = GL_INCR,
            DECR = GL_DECR,
            INVERT = GL_INVERT,
            INCR_WRAP = GL_INCR_WRAP,
            DECR_WRAP = GL_DECR_WRAP
        };
        
        /** set the operations to apply when the various stencil and depth 
          * tests fail or pass.  First parameter is to control the operation
          * when the stencil test fails.  The second parameter is to control the
          * operation when the stencil test passes, but depth test fails. The
          * third parameter controls the operation when both the stencil test
          * and depth pass.  Ordering of parameter is the same as if using
          * glStencilOp(,,).*/
        inline void setOperation(Face face, Operation sfail, Operation zfail, Operation zpass)
        {
            m_sfail[face] = sfail;
            m_zfail[face] = zfail;
            m_zpass[face] = zpass;
        }
        
        /** set the operation when the stencil test fails.*/
        inline void setStencilFailOperation(Face face, Operation sfail) { m_sfail[face] = sfail; }

        /** get the operation when the stencil test fails.*/
        inline Operation getStencilFailOperation(Face face) const { return m_sfail[face]; }
        
        /** set the operation when the stencil test passes but the depth test fails.*/
        inline void setStencilPassAndDepthFailOperation(Face face, Operation zfail) { m_zfail[face]=zfail; }
        
        /** get the operation when the stencil test passes but the depth test fails.*/
        inline Operation getStencilPassAndDepthFailOperation(Face face) const { return m_zfail[face]; }

        /** set the operation when both the stencil test and the depth test pass.*/
        inline void setStencilPassAndDepthPassOperation(Face face, Operation zpass) { m_zpass[face]=zpass; }

        /** get the operation when both the stencil test and the depth test pass.*/
        inline Operation getStencilPassAndDepthPassOperation(Face face) const { return m_zpass[face]; }
        

        inline void setWriteMask(Face face, unsigned int mask) { m_writeMask[face] = mask; }
        
        inline unsigned int getWriteMask(Face face) const { return m_writeMask[face]; }


        virtual void apply(crState& state) const;
		virtual void addAttributeParam(int i, const std::string &str);
    public:

        /** Extensions class which encapsulates the querying of extensions and
          * associated function pointers, and provide convenience wrappers to 
          * check for the extensions or use the associated functions.
        */
        class Extensions : public CRCore::Referenced
        {
            public:
                Extensions(unsigned int contextID);

                Extensions(const Extensions& rhs);
                
                void lowestCommonDenominator(const Extensions& rhs);
                
                void setupGLExtensions(unsigned int contextID);

                void setStencilTwoSidedSupported(bool flag) { m_isStencilTwoSidedSupported=flag; }
                bool isStencilTwoSidedSupported() const { return m_isStencilTwoSidedSupported; }

                void glActiveStencilFace(GLenum face) const;
				static void clearExtensions();
            protected:

                ~Extensions() {}
                
                bool m_isStencilTwoSidedSupported;
                
                void* m_glActiveStencilFace;
        };
        
        /** Function to call to get the extension of a specified context.
          * If the Extension object for that context has not yet been created
          * and the 'createIfNotInitalized' flag been set to false then returns NULL.
          * If 'createIfNotInitalized' is true then the Extensions object is
          * automatically created. However, in this case the extension object
          * will only be created with the graphics context associated with ContextID.
        */
        static Extensions* getExtensions(unsigned int contextID,bool createIfNotInitalized);

        /** The setExtensions method allows users to override the extensions across graphics contexts.
          * Typically used when you have different extensions supported across graphics pipes
          * but need to ensure that they all use the same low common denominator extensions.
        */
        static void setExtensions(unsigned int contextID,Extensions* extensions);
        

    protected:
    
        virtual ~crStencilTwoSided();

        Function            m_func[2];
        int                 m_funcRef[2];
        unsigned int        m_funcMask[2];
        
        Operation           m_sfail[2];
        Operation           m_zfail[2];
        Operation           m_zpass[2];
        
        unsigned int        m_writeMask[2];

};

}

#endif
