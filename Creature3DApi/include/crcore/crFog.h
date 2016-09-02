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
#ifndef CRCORE_CRFOG_H
#define CRCORE_CRFOG_H 1

#include <CRCore/crStateAttribute.h>
#include <CRCore/crVector4.h>

namespace CRCore {
/** crFog - encapsulates OpenGL fog state. */ 
class CR_EXPORT crFog : public crStateAttribute
{
    public :

        crFog();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crFog(const crFog& fog,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(fog,copyop),
            m_mode(fog.m_mode),
            m_density(fog.m_density),
            m_start(fog.m_start),
            m_end(fog.m_end),
            m_color(fog.m_color),
            m_fogCoordinateSource(fog.m_fogCoordinateSource) {}

        META_StateAttribute(CRCore, crFog,FOG);
        
        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crFog,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_mode)
            COMPARE_StateAttribute_Parameter(m_density)
            COMPARE_StateAttribute_Parameter(m_start)
            COMPARE_StateAttribute_Parameter(m_end)
            COMPARE_StateAttribute_Parameter(m_color)
            COMPARE_StateAttribute_Parameter(m_fogCoordinateSource)

            return 0; // passed all the above comparison macro's, must be equal.
        }
		/** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crFog,sa)

			// compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_mode)
			COMPARE_StateAttribute_Parameter(m_density)
			COMPARE_StateAttribute_Parameter(m_start)
			COMPARE_StateAttribute_Parameter(m_end)
			COMPARE_StateAttribute_Parameter(m_color)
			COMPARE_StateAttribute_Parameter(m_fogCoordinateSource)

			return 0; // passed all the above comparison macro's, must be equal.
		}
		virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
		{
			modes.push_back(GL_FOG);
		}

        enum Mode {
            LINEAR,
            EXP,
            EXP2
        };

        inline void  setMode( Mode mode )         { m_mode = mode; }
        inline Mode getMode() const               { return m_mode; }

        inline void  setDensity( float density )  { m_density = density; }
        inline float getDensity() const           { return m_density; }

        inline void  setStart( float start )      { m_start = start; }
        inline float getStart() const             { return m_start; }

        inline void  setEnd( float end )          { m_end = end; }
        inline float getEnd() const               { return m_end; }

        inline void  setColor( const crVector4 &color )      { m_color = color; }
        inline const crVector4& getColor() const             { return m_color; }
        
        enum FogCoordinateSource
        {
            FOG_COORDINATE,
            FRAGMENT_DEPTH
        };
        
        inline void setFogCoordinateSource(GLint source) { m_fogCoordinateSource = source; }
        inline int getFogCoordinateSource() const { return m_fogCoordinateSource; }

		virtual void addAttributeParam(int i, const std::string &str);
		virtual void apply(crState& state) const;
    protected :

        virtual ~crFog();

        Mode    m_mode;
        float   m_density;
        float   m_start;
        float   m_end;
        crVector4    m_color;
        GLint   m_fogCoordinateSource;
};

}

#endif
