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
#ifndef CRCORE_CRMATERIAL_H
#define CRCORE_CRMATERIAL_H 1

#include <CRCore/crVector4.h>
#include <CRCore/crStateAttribute.h>

namespace CRCore {
/** crMaterial - encapsulates OpenGL glMaterial state.*/
class CR_EXPORT crMaterial : public crStateAttribute
{

    public :


        crMaterial();
        
        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crMaterial(const crMaterial& mat,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(mat,copyop),
            m_colorMode(mat.m_colorMode),
            m_ambientFrontAndBack(mat.m_ambientFrontAndBack),
            m_ambientFront(mat.m_ambientFront),
            m_ambientBack(mat.m_ambientBack),
            m_diffuseFrontAndBack(mat.m_diffuseFrontAndBack),
            m_diffuseFront(mat.m_diffuseFront),
            m_diffuseBack(mat.m_diffuseBack),
            m_specularFrontAndBack(mat.m_specularFrontAndBack),
            m_specularFront(mat.m_specularFront),
            m_specularBack(mat.m_specularBack),
            m_emissionFrontAndBack(mat.m_emissionFrontAndBack),
            m_emissionFront(mat.m_emissionFront),
            m_emissionBack(mat.m_emissionBack),
            m_shininessFrontAndBack(mat.m_shininessFrontAndBack),
            m_shininessFront(mat.m_shininessFront),
            m_shininessBack(mat.m_shininessBack) {}

        META_StateAttribute(CRCore, crMaterial, MATERIAL);
        
        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crMaterial,sa)
			//首字母为*字符，表示该材质不进行相同性判断
            if(m_name.find('*') == 0) return -1;
			if(sa.getName().find('*') == 0) return 1;
            // compare each parameter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_colorMode)
            COMPARE_StateAttribute_Parameter(m_ambientFrontAndBack)
            COMPARE_StateAttribute_Parameter(m_ambientFront)
            COMPARE_StateAttribute_Parameter(m_ambientBack)
            COMPARE_StateAttribute_Parameter(m_diffuseFrontAndBack)
            COMPARE_StateAttribute_Parameter(m_diffuseFront)
            COMPARE_StateAttribute_Parameter(m_diffuseBack)
            COMPARE_StateAttribute_Parameter(m_specularFrontAndBack)
            COMPARE_StateAttribute_Parameter(m_specularFront)
            COMPARE_StateAttribute_Parameter(m_specularBack)
            COMPARE_StateAttribute_Parameter(m_emissionFrontAndBack)
            COMPARE_StateAttribute_Parameter(m_emissionFront)
            COMPARE_StateAttribute_Parameter(m_emissionBack)
            COMPARE_StateAttribute_Parameter(m_shininessFrontAndBack)
            COMPARE_StateAttribute_Parameter(m_shininessFront)
            COMPARE_StateAttribute_Parameter(m_shininessBack)

            return 0; // passed all the above comparison macro's, must be equal.
        }

		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crMaterial,sa)
			//首字母为*字符，表示该材质不进行相同性判断
			if(m_name.find('*') == 0) return -1;
			if(sa.getName().find('*') == 0) return 1;
			// compare each parameter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_colorMode)
			COMPARE_StateAttribute_Parameter(m_ambientFrontAndBack)
			COMPARE_StateAttribute_Parameter(m_ambientFront)
			COMPARE_StateAttribute_Parameter(m_ambientBack)
			COMPARE_StateAttribute_Parameter(m_diffuseFrontAndBack)
			COMPARE_StateAttribute_Parameter(m_diffuseFront)
			COMPARE_StateAttribute_Parameter(m_diffuseBack)
			COMPARE_StateAttribute_Parameter(m_specularFrontAndBack)
			COMPARE_StateAttribute_Parameter(m_specularFront)
			COMPARE_StateAttribute_Parameter(m_specularBack)
			COMPARE_StateAttribute_Parameter(m_emissionFrontAndBack)
			COMPARE_StateAttribute_Parameter(m_emissionFront)
			COMPARE_StateAttribute_Parameter(m_emissionBack)
			COMPARE_StateAttribute_Parameter(m_shininessFrontAndBack)
			COMPARE_StateAttribute_Parameter(m_shininessFront)
			COMPARE_StateAttribute_Parameter(m_shininessBack)
		
			return 0; // passed all the above comparison macro's, must be equal.
		}

        virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
        {
            // Have to think about the role of m_colorMode
            // in setting the colormaterial... also need to take the
            // color material enable/disable out of the the apply()...
            //modes.push_back(GL_COLOR_MATERIAL);
        }

        //virtual void apply( crState& state ) const;

        enum Face {
            FRONT,
            BACK,
            FRONT_AND_BACK
        };

        enum ColorMode {
            AMBIENT,
            DIFFUSE,
            SPECULAR,
            EMISSION,
            AMBIENT_AND_DIFFUSE,
            OFF            
        };

        inline void setColorMode(ColorMode mode) { m_colorMode = mode; }
        inline ColorMode getColorMode() const { return m_colorMode; }

        void setAmbient( Face face, const crVector4& ambient );
        const crVector4& getAmbient(Face face) const;
        inline bool getAmbientFrontAndBack() const { return m_ambientFrontAndBack; }

        void setDiffuse( Face face, const crVector4& diffuse );
        const crVector4& getDiffuse(Face face) const;
        inline bool getDiffuseFrontAndBack() const { return m_diffuseFrontAndBack; }

        /** Set specular value of specified face(s) of the material, 
          * valid specular[0..3] range is 0.0 to 1.0.*/
        void setSpecular( Face face, const crVector4& specular );
        /** Get the specular value for specified face.*/
        const crVector4& getSpecular(Face face) const;
        /** Get the whether specular values are equal for front and back faces.*/
        inline bool getSpecularFrontAndBack() const { return m_specularFrontAndBack; }

        /** Set emission value of specified face(s) of the material, 
          * valid emmison[0..3] range is 0.0 to 1.0.*/
        void setEmission( Face face, const crVector4& emission );
        /** Get the emmsion value for specified face.*/
        const crVector4& getEmission(Face face) const;
        /** Get the whether emission values are equal for front and back faces.*/
        inline bool getEmissionFrontAndBack() const { return m_emissionFrontAndBack; }

        /** Set shininess of specified face(s) of the material, valid shininess range is 0.0 to 128.0.*/
        void setShininess(Face face, float shininess );
        /** Get the shininess value for specified face.*/
        float getShininess(Face face) const;
        /** Get the whether shininess values are equal for front and back faces.*/
        inline bool getShininessFrontAndBack() const { return m_shininessFrontAndBack; }
        
        /** Set the alpha value of ambient,diffuse,specular and emission colors, 
          * of specified face, to 1-transparency. Valid transparency range is 0.0 to 1.0.*/
        void setTransparency(Face face,float trans);

        /** Set the alpha value of ambient,diffuse,specular and emission colors.
          * Valid transparency range is 0.0 to 1.0.*/
        void setAlpha(Face face,float alpha);

		virtual void addAttributeParam(int i, const std::string &str);
		virtual void apply(crState& state) const;
    protected :

        virtual ~crMaterial();

        ColorMode m_colorMode;

        bool m_ambientFrontAndBack;
        crVector4 m_ambientFront;                          // r, g, b, w
        crVector4 m_ambientBack;                           // r, g, b, w

        bool m_diffuseFrontAndBack;
        crVector4 m_diffuseFront;                          // r, g, b, w
        crVector4 m_diffuseBack;                           // r, g, b, w

        bool m_specularFrontAndBack;
        crVector4 m_specularFront;                         // r, g, b, w
        crVector4 m_specularBack;                          // r, g, b, w

        bool m_emissionFrontAndBack;
        crVector4 m_emissionFront;                         // r, g, b, w
        crVector4 m_emissionBack;                          // r, g, b, w

        bool m_shininessFrontAndBack;
        float  m_shininessFront;                      // values 0 - 128.0
        float  m_shininessBack;                       // values 0 - 128.0

};

}

#endif
