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
#ifndef CRCORE_CRLIGHTMODEL_H
#define CRCORE_CRLIGHTMODEL_H 1

#include <CRCore/crStateAttribute.h>
#include <CRCore/crVector4.h>

namespace CRCore {

class CR_EXPORT crLightModel : public crStateAttribute
{
    public :

        crLightModel();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crLightModel(const crLightModel& lw,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
          crStateAttribute(lw,copyop),
          m_ambient(lw.m_ambient),
          m_colorControl(lw.m_colorControl),
          m_localViewer(lw.m_localViewer),
          m_twoSided(lw.m_twoSided) {}


        META_StateAttribute(CRCore, crLightModel, LIGHTMODEL);

        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crLightModel,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_ambient)
            COMPARE_StateAttribute_Parameter(m_colorControl)
            COMPARE_StateAttribute_Parameter(m_localViewer)
            COMPARE_StateAttribute_Parameter(m_twoSided)

            return 0; // passed all the above comparison macro's, must be equal.
        }

		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crLightModel,sa)

			// compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_ambient)
			COMPARE_StateAttribute_Parameter(m_colorControl)
			COMPARE_StateAttribute_Parameter(m_localViewer)
			COMPARE_StateAttribute_Parameter(m_twoSided)
			
			return 0; // passed all the above comparison macro's, must be equal.
		}

        void setAmbientIntensity(const CRCore::crVector4& ambient) { m_ambient = ambient; }
        const CRCore::crVector4& getAmbientIntensity() const { return m_ambient; }
		CRCore::crVector4& getAmbientIntensity(){ return m_ambient; }

        enum ColorControl
        {
            SEPARATE_SPECULAR_COLOR,
            SINGLE_COLOR
        };
        
        void setColorControl(ColorControl cc) { m_colorControl = cc; }
        inline ColorControl getColorControl() const { return m_colorControl; }


        void setLocalViewer(bool localViewer) { m_localViewer=localViewer; }
        inline bool getLocalViewer() const { return m_localViewer; }


        void setTwoSided(bool twoSided) { m_twoSided = twoSided; }
        inline bool getTwoSided() const { return m_twoSided; }

        virtual void apply(crState& state) const;
		virtual void addAttributeParam(int i, const std::string &str);
    protected :

        virtual ~crLightModel();

        CRCore::crVector4 m_ambient;
        ColorControl m_colorControl;
        bool m_localViewer;
        bool m_twoSided;

};

}

#endif
