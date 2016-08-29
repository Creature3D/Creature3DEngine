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
#ifndef CRCORE_LIGHT
#define CRCORE_LIGHT 1

#include <CRCore/crStateAttribute.h>
#include <CRCore/crTexture2D.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crBoundingBox.h>
#include <CRCore/crBoundingSphere.h>
#include <string>
namespace CRCore {

  /** Light state class which encapsulates OpenGL glLight() functionality.*/
  class CR_EXPORT crLight : public crStateAttribute
  {
  public :

    crLight();

    /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
    crLight(const crLight& light,const crCopyOp& copyop = crCopyOp::SHALLOW_COPY):
    crStateAttribute(light,copyop),
      m_lightNum(light.m_lightNum),
      m_ambient(light.m_ambient),
      m_diffuse(light.m_diffuse),
      m_specular(light.m_specular),
      m_position(light.m_position),
      m_direction(light.m_direction),
      m_constant_Attenuation(light.m_constant_Attenuation),
      m_linear_Attenuation(light.m_linear_Attenuation),
      m_quadratic_Attenuation(light.m_quadratic_Attenuation),
      m_spot_Exponent(light.m_spot_Exponent),
      m_spot_Cutoff(light.m_spot_Cutoff),
	  m_lowestLum(light.m_lowestLum),
	  m_lightType(light.m_lightType),
	  m_maxDistance(light.m_maxDistance),
	  m_distance_computed(light.m_distance_computed){}

      META_StateAttribute(CRCore, crLight, LIGHT0+m_lightNum);

      /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
      virtual int compare(const crStateAttribute& sa) const
      {
        // check the types are equal and then create the rhs variable
        // used by the COMPARE_StateAttribute_Paramter macro's below.
          COMPARE_StateAttribute_Types(crLight,sa)

          // compare each paramter in turn against the rhs.
          COMPARE_StateAttribute_Parameter(m_lightNum)
          COMPARE_StateAttribute_Parameter(m_ambient)
          COMPARE_StateAttribute_Parameter(m_diffuse)
          COMPARE_StateAttribute_Parameter(m_specular)
          COMPARE_StateAttribute_Parameter(m_position)
          COMPARE_StateAttribute_Parameter(m_direction)
          COMPARE_StateAttribute_Parameter(m_constant_Attenuation)
          COMPARE_StateAttribute_Parameter(m_linear_Attenuation)
          COMPARE_StateAttribute_Parameter(m_quadratic_Attenuation)
          COMPARE_StateAttribute_Parameter(m_spot_Exponent)
          COMPARE_StateAttribute_Parameter(m_spot_Cutoff)

		  COMPARE_StateAttribute_Parameter(m_lightType)

          return 0; // passed all the above comparison macro's, must be equal.
	  }

	  virtual int compare(const crStateAttribute& sa)
	  {
		  // check the types are equal and then create the rhs variable
		  // used by the COMPARE_StateAttribute_Paramter macro's below.
		  COMPARE_StateAttribute_Types(crLight,sa)

		  // compare each paramter in turn against the rhs.
	      COMPARE_StateAttribute_Parameter(m_lightNum)
		  COMPARE_StateAttribute_Parameter(m_ambient)
	      COMPARE_StateAttribute_Parameter(m_diffuse)
		  COMPARE_StateAttribute_Parameter(m_specular)
		  COMPARE_StateAttribute_Parameter(m_position)
		  COMPARE_StateAttribute_Parameter(m_direction)
		  COMPARE_StateAttribute_Parameter(m_constant_Attenuation)
		  COMPARE_StateAttribute_Parameter(m_linear_Attenuation)
		  COMPARE_StateAttribute_Parameter(m_quadratic_Attenuation)
		  COMPARE_StateAttribute_Parameter(m_spot_Exponent)
		  COMPARE_StateAttribute_Parameter(m_spot_Cutoff)

		  COMPARE_StateAttribute_Parameter(m_lightType)

	      return 0; // passed all the above comparison macro's, must be equal.
	  }

      virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
      {
         modes.push_back((unsigned int)(GL_LIGHT0 + m_lightNum));
      }

      /** Set which OpenGL light to operate on.*/
      void setLightNum(int num) { m_lightNum = num; }

      /** Get which OpenGL light this CRCore::Light operates on.*/
      int getLightNum() const { return m_lightNum; }

      /** Set the ambient component of the light. */
      inline void setAmbient( const crVector4& ambient ) { m_ambient = ambient; }

      /** Get the ambient component of the light. */
      inline const crVector4& getAmbient() const { return m_ambient; }

      /** Set the diffuse component of the light. */
      inline void setDiffuse( const crVector4& diffuse ) { m_diffuse = diffuse; }

      /** Get the diffuse component of the light. */
      inline const crVector4& getDiffuse() const { return m_diffuse; }

      /** Set the specular component of the light. */
      inline void setSpecular( const crVector4& specular ) { m_specular = specular; }

      /** Get the specular component of the light. */
      inline const crVector4& getSpecular() const { return m_specular; }

      /** Set the position of the light. */	
      inline void setPosition( const crVector4& position ) { m_position = position; }

      /** Get the position of the light. */	
	  inline crVector4& getPosition() { return m_position; }
      inline const crVector4& getPosition() const { return m_position; }

      /** Set the direction of the light. */	
      inline void setDirection( const crVector3& direction ) { m_direction = direction; }

      /** Get the direction of the light. */	
      inline const crVector3& getDirection() const { return m_direction; }
	  inline crVector3& getDirection() { return m_direction; }

      /** Set the constant attenuation of the light. */
      inline void setConstantAttenuation( float constant_attenuation )     { m_constant_Attenuation = constant_attenuation; dirtyDistance(); }

      /** Get the constant attenuation of the light. */
      inline float getConstantAttenuation() const { return m_constant_Attenuation; }

      /** Set the linear attenuation of the light. */
      inline void setLinearAttenuation ( float linear_attenuation )        { m_linear_Attenuation = linear_attenuation; dirtyDistance(); }

      /** Get the linear attenuation of the light. */
      inline float getLinearAttenuation () const { return m_linear_Attenuation; }

      /** Set the quadratic attenuation of the light. */
      inline void setQuadraticAttenuation ( float quadratic_attenuation )  { m_quadratic_Attenuation = quadratic_attenuation; dirtyDistance(); }

      /** Get the quadratic attenuation of the light. */
      inline float getQuadraticAttenuation()  const { return m_quadratic_Attenuation; }

      /** Set the spot exponent of the light. */
      inline void setSpotExponent( float spot_exponent )                   { m_spot_Exponent = spot_exponent; }

      /** Get the spot exponent of the light. */
      inline float getSpotExponent() const { return m_spot_Exponent; }

      /** Set the spot cutoff of the light. */
      inline void setSpotCutoff( float spot_cutoff )                       { m_spot_Cutoff = spot_cutoff; }

      /** Get the spot cutoff of the light. */
      inline float getSpotCutoff() const                                   { return m_spot_Cutoff; }

      /**
      * Capture the lighting settings of the current OpenGL state
      * and store them in this object.
      */
      void captureLightState();

      /** Apply the light's state to the OpenGL state machine. */
      virtual void apply(crState& state) const;
      enum LightType
	  {
		  LIGHT_INFINITE,
		  LIGHT_LOCAL,
		  LIGHT_SPOT
	  };
	  inline void setLightType( LightType type ) { m_lightType = type; }
	  inline LightType getLightType() const      { return m_lightType; }
      
	  inline void setLoweastLum( float loweastLum ) { m_lowestLum = loweastLum>0.0f?loweastLum:0.0f; dirtyDistance(); }
	  inline float getLoweastLum() const      { return m_lowestLum; }

	  void dirtyDistance();
	  float getMaxDistance();

	  //void setLightMapFileName(const std::string &fileName){ m_lightMapFileName = fileName; }
	  //const std::string &getLightMapFileName(){ return  m_lightMapFileName; }
	  //void createLightTexture(crImage *image);
	  //const crTexture2D *getLightTexture() const;
	  //crTexture2D *getLightTexture();

	  //static crImage* createStdSpotLightImage(float power = 1.0f);
	  //static crImage* createSunLightImage();

	  //inline void setEnable(bool enable) { m_enable = enable; }
	  //inline bool getEnable() { return m_enable; }
  protected :

    virtual ~crLight();

    /** Initialize the light's settings with some decent defaults. */
    void init();
    void computeDistance();

    //do not know exactly
    int m_lightNum;                           // OpenGL light number
    //
    LightType m_lightType;
    crVector4 m_ambient;                      // r, g, b, w
    crVector4 m_diffuse;                      // r, g, b, w
    crVector4 m_specular;                     // r, g, b, w
    crVector4 m_position;                     // x, y, z, w
    crVector3 m_direction;                    // x, y, z
    float m_constant_Attenuation;             // constant
    float m_linear_Attenuation;               // linear
    float m_quadratic_Attenuation;            // quadratic
    float m_spot_Exponent;                    // exponent
    float m_spot_Cutoff;                      // spreadc
    float m_lowestLum;

	float m_maxDistance;
	bool m_distance_computed;
	//bool m_enable;
	//std::string m_lightMapFileName;
	//ref_ptr<crTexture2D> m_lightMap;
  };

}

#endif