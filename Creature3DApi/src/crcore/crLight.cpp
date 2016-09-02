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
#include <CRCore/crLight.h>
#include <CRCore/crNotify.h>
#include <CRCore/crBrain.h>
using namespace CRCore;

crLight::crLight( void )
{
  init();

  //     notify(DEBUG) << "_ambient "<<_ambient<<std::endl;
  //     notify(DEBUG) << "_diffuse "<<_diffuse<<std::endl;
  //     notify(DEBUG) << "_specular "<<_specular<<std::endl;
  //     notify(DEBUG) << "_position "<<_position<<std::endl;
  //     notify(DEBUG) << "_direction "<<_direction<<std::endl;
  //     notify(DEBUG) << "_spot_exponent "<<_spot_exponent<<std::endl;
  //     notify(DEBUG) << "_spot_cutoff "<<_spot_cutoff<<std::endl;
  //     notify(DEBUG) << "_constant_attenuation "<<_constant_attenuation<<std::endl;
  //     notify(DEBUG) << "_linear_attenuation "<<_linear_attenuation<<std::endl;
  //     notify(DEBUG) << "_quadratic_attenuation "<<_quadratic_attenuation<<std::endl;
}


crLight::~crLight( void )
{
}


void crLight::init( void )
{
  m_lightNum = 0;
  m_ambient.set(0.05f,0.05f,0.05f,1.0f);
  m_diffuse.set(0.8f,0.8f,0.8f,1.0f);
  m_specular.set(0.8f,0.8f,0.8f,1.0f);
  m_position.set(0.0f,0.0f,1.0f,1.0f);
  m_direction.set(0.0f,0.0f,-1.0f);
  m_spot_Exponent = 0.0f;
  m_spot_Cutoff = 180.0f;
  m_constant_Attenuation = 0.5f;
  m_linear_Attenuation = 0.1f;
  m_quadratic_Attenuation = 0.0f;
  m_lightType = LIGHT_LOCAL;
  m_lowestLum = 0.2f;

  m_distance_computed = false;
  m_maxDistance = 0.0f;
  //m_enable = true;
}

void crLight::dirtyDistance()
{
    m_distance_computed = false;
}
float crLight::getMaxDistance()
{
     if(!m_distance_computed)
		 computeDistance();
	 return m_maxDistance;
}

void crLight::computeDistance()
{
	if(m_lightType == LIGHT_INFINITE)
	{
		m_maxDistance = crBrain::getInstance()->getMaxDepth()*crBrain::getInstance()->getMaxDepth();
	}
	else
	{
		float distance = crBrain::getInstance()->getMaxDepth()*crBrain::getInstance()->getMaxDepth();
		if(m_lowestLum>0.0f)
		{
			if(m_quadratic_Attenuation==0.0f)
			{
				if(m_linear_Attenuation!=0.0f)
				{
					distance = (1.0f/m_lowestLum-m_constant_Attenuation)/m_linear_Attenuation;
				}
			}
			else
			{
				float a = m_linear_Attenuation*m_linear_Attenuation - 4.0f*m_quadratic_Attenuation*(m_constant_Attenuation-1.0f/m_lowestLum);
				if(a>0)
				{
					float x1 = (-m_linear_Attenuation+sqrt(a))/(2.0f*m_quadratic_Attenuation);
					distance = CRCore::maximum(0.0f,x1);
				}
			}
		}
		m_maxDistance = distance;
	}

	m_distance_computed = true;
}

//crImage* crLight::createStdSpotLightImage(float power)
//{
//	crImage* image = new crImage;
//	image->setFileName("StdSpotLightImage");
//	image->allocateImage(64,64,1,
//		GL_RGBA,GL_UNSIGNED_BYTE);
//
//    crVector4 color;
//	crVector4 centerColour(1.0f,1.0f,1.0f,1.0f);
//	crVector4 backgroudColour(0.0f,0.0f,0.0f,1.0f);
//	float mid = (float(64)-1.0f)*0.5f;
//	float div = 2.0f/float(64);
//	for(unsigned int r=0;r<64;++r)
//	{
//		unsigned char* ptr = image->data(0,r,0);
//		for(unsigned int c=0;c<64;++c)
//		{
//			float dx = (float(c) - mid)*div;
//			float dy = (float(r) - mid)*div;
//			float r = powf(1.0f-sqrtf(dx*dx+dy*dy),power);
//			if (r<0.0f) r=0.0f;
//			color = centerColour*r+backgroudColour*(1.0f-r);
//			*ptr++ = (unsigned char)((color[0])*255.0f);
//			*ptr++ = (unsigned char)((color[1])*255.0f);
//			*ptr++ = (unsigned char)((color[2])*255.0f);
//			*ptr++ = (unsigned char)((color[3])*255.0f);
//		}
//	}
//	return image;
//}
//
//crImage* crLight::createSunLightImage()
//{
//	crImage* image = new crImage;
//	image->setFileName("SunLightImage");
//	image->allocateImage(64,64,1,
//		GL_RGBA,GL_UNSIGNED_BYTE);
//
//	for(unsigned int r=0;r<64;++r)
//	{
//		unsigned char* ptr = image->data(0,r,0);
//		for(unsigned int c=0;c<64;++c)
//		{
//			*ptr++ = (unsigned char)(255.0f);
//			*ptr++ = (unsigned char)(255.0f);
//			*ptr++ = (unsigned char)(255.0f);
//			*ptr++ = (unsigned char)(255.0f);
//		}
//	}
//	return image;
//}
//
//void crLight::createLightTexture(crImage *image)
//{
//	if(!m_lightMap.valid())
//	{
//        m_lightMap = new crTexture2D;
//	}
//	m_lightMap->setImage(image);
//}
//
//const crTexture2D *crLight::getLightTexture() const
//{
//	return m_lightMap.get();
//}
//
//crTexture2D *crLight::getLightTexture()
//{
//	return m_lightMap.get();
//}
#include <Driver/GLDrv/crLightDrv.h>