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
#ifndef CRCORE_CLAMPCOLOR_H
#define CRCORE_CLAMPCOLOR_H 1

#include <CRCore/crStateAttribute.h>

namespace CRCore {

/** Encapsulates OpenGL crClampColor state. */
class CR_EXPORT crClampColor : public crStateAttribute
{
    public :

        crClampColor();
        
        crClampColor(GLenum vertexMode, GLenum fragmentMode, GLenum readMode);

        /** Copy constructor using crCopyOp to manage deep vs shallow copy. */
        crClampColor(const crClampColor& rhs,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(rhs,copyop),
            m_clampVertexColor(rhs.m_clampVertexColor),
            m_clampFragmentColor(rhs.m_clampFragmentColor),
            m_clampReadColor(rhs.m_clampReadColor) {}

        META_StateAttribute(CRCore, crClampColor,CLAMPCOLOR);
        
        /** Return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs. */
        virtual int compare(const crStateAttribute& sa) const
        {
            // Check for equal types, then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macros below.
            COMPARE_StateAttribute_Types(crClampColor,sa)

            // Compare each parameter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_clampVertexColor)
            COMPARE_StateAttribute_Parameter(m_clampFragmentColor)
            COMPARE_StateAttribute_Parameter(m_clampReadColor)

            return 0; // Passed all the above comparison macros, so must be equal.
        }
		virtual int compare(const crStateAttribute& sa)
		{
			// Check for equal types, then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macros below.
			COMPARE_StateAttribute_Types(crClampColor,sa)

			// Compare each parameter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_clampVertexColor)
			COMPARE_StateAttribute_Parameter(m_clampFragmentColor)
			COMPARE_StateAttribute_Parameter(m_clampReadColor)

			return 0; // Passed all the above comparison macros, so must be equal.
		}

        void setClampVertexColor(GLenum mode) { m_clampVertexColor = mode; }
        GLenum getClampVertexColor() const { return m_clampVertexColor; }

        void setClampFragmentColor(GLenum mode) { m_clampFragmentColor = mode; }
        GLenum getClampFragmentColor() const { return m_clampFragmentColor; }

        void setClampReadColor(GLenum mode) { m_clampReadColor = mode; }
        GLenum getClampReadColor() const { return m_clampReadColor; }

		virtual void apply(crState& state) const;
        class Extensions : public Referenced
        {
            public:
                Extensions(unsigned int contextID);

                Extensions(const Extensions& rhs);

                void lowestCommonDenominator(const Extensions& rhs);

                void setupGLExtensions(unsigned int contextID);

                void setClampColorSupported(bool flag) { _isClampColorSupported=flag; }
                bool isClampColorSupported() const { return _isClampColorSupported; }

                void glClampColor(GLenum target, GLenum mode) const;
				static void clearExtensions();
            protected:

                ~Extensions() {}

                typedef void (APIENTRY * GLClampColorProc) (GLenum target, GLenum mode);
                bool                _isClampColorSupported;
                GLClampColorProc    _glClampColor;

        };
        static Extensions* getExtensions(unsigned int contextID,bool createIfNotInitalized);
        static void setExtensions(unsigned int contextID,Extensions* extensions);

		virtual void addAttributeParam(int i, const std::string &str);
    protected :
        virtual ~crClampColor();
        GLenum m_clampVertexColor;
        GLenum m_clampFragmentColor;
        GLenum m_clampReadColor;
};

}

#endif
