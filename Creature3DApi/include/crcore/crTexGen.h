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
#ifndef CRCORE_CRTEXGEN_H
#define CRCORE_CRTEXGEN_H 1

#include <CRCore/crPlane.h>
#include <CRCore/crStateAttribute.h>
#include <CRCore/crMatrix.h>
namespace CRCore {

/** crTexGen - encapsulates the OpenGL glTexGen (texture coordinate generation) state.*/
class CR_EXPORT crTexGen : public crStateAttribute
{
    public :
    
        crTexGen();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crTexGen(const crTexGen& texgen,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(texgen,copyop),
            m_mode(texgen.m_mode),
            m_plane_s(texgen.m_plane_s),
            m_plane_t(texgen.m_plane_t),
            m_plane_r(texgen.m_plane_r),
            m_plane_q(texgen.m_plane_q) {}

        META_StateAttribute(CRCore, crTexGen, TEXGEN);
        
        virtual bool isTextureAttribute() const { return true; }

        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crTexGen,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_mode)
            COMPARE_StateAttribute_Parameter(m_plane_s)
            COMPARE_StateAttribute_Parameter(m_plane_t)
            COMPARE_StateAttribute_Parameter(m_plane_r)
            COMPARE_StateAttribute_Parameter(m_plane_q)

            return 0; // passed all the above comparison macro's, must be equal.
        }

		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crTexGen,sa)

			// compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_mode)
			COMPARE_StateAttribute_Parameter(m_plane_s)
			COMPARE_StateAttribute_Parameter(m_plane_t)
			COMPARE_StateAttribute_Parameter(m_plane_r)
			COMPARE_StateAttribute_Parameter(m_plane_q)

			return 0; // passed all the above comparison macro's, must be equal.
		}

        virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
        {
			modes.push_back(GL_TEXTURE_GEN_S);
            modes.push_back(GL_TEXTURE_GEN_T);

            // Not happy with turning all tex gen paramters on
            // as the OSG currently only supports 2D textures and therefore
            // only S and T will only be required, R&Q would be redundant...
            // So commenting out the following until OSG supports 3D textures.
            // I plan to revamp the OpenGL state management later so will
            // tidy up then.  Robert Osfield. Jan 2001.
            
            // The tidy up is now happening, but will have a think before
            // resolving the below parameters.
            modes.push_back(GL_TEXTURE_GEN_R);
            modes.push_back(GL_TEXTURE_GEN_Q);
        }

        virtual void apply(crState& state) const;

        enum Mode {
            OBJECT_LINEAR,
            EYE_LINEAR,
            SPHERE_MAP,
            NORMAL_MAP,
            REFLECTION_MAP
        };

        inline void setMode( Mode mode ) { m_mode = mode; }

        Mode getMode() const { return m_mode; }

        enum Coord {
	       S, T, R, Q
        };

        void setPlane(Coord which, const crPlane& plane);

        crPlane& getPlane(Coord which);

        const crPlane& getPlane(Coord which) const;

		void setPlanesFromMatrix(const crMatrixd& matrix);
		virtual void addAttributeParam(int i, const std::string &str);

    protected :

        virtual ~crTexGen( void );

        Mode m_mode;

        /// additional texgen coefficents for GL_OBJECT_PLANE or GL_EYE_PLANE,
        crPlane m_plane_s, m_plane_t, m_plane_r, m_plane_q;

};

}

#endif
