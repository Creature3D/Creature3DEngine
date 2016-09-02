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
#ifndef CRCORE_CRTEXENV_H
#define CRCORE_CRTEXENV_H 1

#include <CRCore/crStateAttribute.h>
#include <CRCore/crVector4.h>

namespace CRCore {

/** crTexEnv - encapsulates the OpenGL glTexEnv (texture environment) state.*/
class CR_EXPORT crTexEnv : public crStateAttribute
{
    public :

       enum Mode 
	   {
           NONE,
           DECAL,
           MODULATE,
           BLEND,
           REPLACE,
           ADD,
		   COLOR
        };

        crTexEnv(Mode mode=MODULATE);
        
        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crTexEnv(const crTexEnv& texenv,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(texenv,copyop),
            m_mode(texenv.m_mode),
            m_color(texenv.m_color) {}


        META_StateAttribute(CRCore, crTexEnv, TEXENV);

        virtual bool isTextureAttribute() const { return true; }

        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crTexEnv,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_mode)
            COMPARE_StateAttribute_Parameter(m_color)

            return 0; // passed all the above comparison macro's, must be equal.
        }

		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crTexEnv,sa)

			// compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_mode)
			COMPARE_StateAttribute_Parameter(m_color)

			return 0; // passed all the above comparison macro's, must be equal.
		}

        void setMode( Mode mode ) { m_mode = mode; }
        
        Mode getMode() const { return m_mode; }
        
        void setColor( const crVector4& color ) { m_color = color; }
        
        crVector4& getColor() { return m_color; }

        const crVector4& getColor() const { return m_color; }

        virtual void apply(crState& state) const;
		virtual void addAttributeParam(int i, const std::string &str);

    protected :

        virtual ~crTexEnv( void );

        Mode        m_mode;
        CRCore::crVector4   m_color;
};

}

#endif
