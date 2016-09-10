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

#ifndef CRCORE_CRBLENDCOLOR_H
#define CRCORE_CRBLENDCOLOR_H 1

#include <CRCore/crStateAttribute.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/crVector4.h>

namespace CRCore {

/** crBlendColor - encapsulates the OpenGL blend/transparency state.*/
class CR_EXPORT crBlendColor : public crStateAttribute
{
    public :

        crBlendColor();
        
        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crBlendColor(const crBlendColor& trans,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(trans,copyop),
            m_constantColor(trans.m_constantColor) {}

        META_StateAttribute(CRCore, crBlendColor,BLENDCOLOR);
        
        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crBlendColor,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_constantColor)

            return 0; // passed all the above comparison macro's, must be equal.
        }

		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crBlendColor,sa)

				// compare each paramter in turn against the rhs.
				COMPARE_StateAttribute_Parameter(m_constantColor)

				return 0; // passed all the above comparison macro's, must be equal.
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
        void setConstantColor(const CRCore::crVector4& color) { m_constantColor = color; }
        inline CRCore::crVector4 getConstantColor() const { return m_constantColor; }

		virtual void addAttributeParam(int i, const std::string &str);
		virtual void apply(crState& state) const;
	public:
        class Extensions : public CRCore::Referenced
        {
            public:
				Extensions(unsigned int contextID);

				Extensions(const Extensions& rhs);

				void lowestCommonDenominator(const Extensions& rhs);

				void setupGLExtensions(unsigned int contextID);

				void setBlendColorSupported(bool flag) { _isBlendColorSupported=flag; }
				bool isBlendColorSupported() const { return _isBlendColorSupported; }

				void glBlendColor(GLclampf red , GLclampf green , GLclampf blue , GLclampf alpha) const;
				static void clearExtensions();
            protected:
                ~Extensions() {}
				typedef void (APIENTRY * GLBlendColorProc) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
				bool                _isBlendColorSupported;
				GLBlendColorProc    _glBlendColor;
        };
        static Extensions* getExtensions(unsigned int contextID,bool createIfNotInitalized);
        static void setExtensions(unsigned int contextID,Extensions* extensions);
    protected :

        virtual ~crBlendColor();

        CRCore::crVector4 m_constantColor;
};

}

#endif
