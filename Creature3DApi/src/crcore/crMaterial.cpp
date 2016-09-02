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
#include <CRCore/crMaterial.h>
#include <CRCore/crBoundsChecking.h>
#include <CRCore/crArgumentParser.h>
using namespace CRCore;

crMaterial::crMaterial()
{
    m_colorMode = OFF;

    m_ambientFrontAndBack = true;
    m_ambientFront.set(0.2f, 0.2f, 0.2f, 1.0f);
    m_ambientBack.set(0.2f, 0.2f, 0.2f, 1.0f);

    m_diffuseFrontAndBack = true;
    m_diffuseFront.set(0.8f, 0.8f, 0.8f, 1.0f);
    m_diffuseBack.set(0.8f, 0.8f, 0.8f, 1.0f);

    m_specularFrontAndBack = true;
    m_specularFront.set(0.0f, 0.0f, 0.0f, 1.0f);
    m_specularBack.set(0.0f, 0.0f, 0.0f, 1.0f);

    m_emissionFrontAndBack = true;
    m_emissionFront.set(0.0f, 0.0f, 0.0f, 1.0f);
    m_emissionBack.set(0.0f, 0.0f, 0.0f, 1.0f);

    m_shininessFrontAndBack = true;
    m_shininessFront = 0.0f;
    m_shininessBack = 0.0f;
}


crMaterial::~crMaterial()
{
}


void crMaterial::setAmbient(Face face, const crVector4& ambient )
{
    switch(face)
    {
        case(FRONT):
            m_ambientFrontAndBack = false;
            m_ambientFront = ambient;
            //clampArray4BetweenRange(m_ambientFront,0.0f,1.0f,"CRCore::crMaterial::setAmbient(..)");
            break;
        case(BACK):
            m_ambientFrontAndBack = false;
            m_ambientBack = ambient;
            //clampArray4BetweenRange(m_ambientBack,0.0f,1.0f,"crMaterial::setAmbient(..)");
            break;
        case(FRONT_AND_BACK):
            m_ambientFrontAndBack = true;
            m_ambientFront = ambient;
            //clampArray4BetweenRange(m_ambientFront,0.0f,1.0f,"crMaterial::setAmbient(..)");
            m_ambientBack = m_ambientFront;
            break;
        default:
            notify(NOTICE)<<"Notice: invalid Face passed to crMaterial::setAmbient()."<<std::endl;
    }
}


const crVector4& crMaterial::getAmbient(Face face) const
{
    switch(face)
    {
        case(FRONT):
            return m_ambientFront;
        case(BACK):
            return m_ambientBack;
        case(FRONT_AND_BACK):
            if (!m_ambientFrontAndBack)
            {
                notify(NOTICE)<<"Notice: crMaterial::getAmbient(FRONT_AND_BACK) called on material "<< std::endl;
                notify(NOTICE)<<"        with separate FRONT and BACK ambient colors."<< std::endl;
            }
            return m_ambientFront;
    }
    //notify(NOTICE)<<"Notice: invalid Face passed to crMaterial::getAmbient()."<< std::endl;
    return m_ambientFront;
}


void crMaterial::setDiffuse(Face face, const crVector4& diffuse )
{
    switch(face)
    {
        case(FRONT):
            m_diffuseFrontAndBack = false;
            m_diffuseFront = diffuse;
            //clampArray4BetweenRange(m_diffuseFront,0.0f,1.0f,"crMaterial::setDiffuse(..)");
            break;
        case(BACK):
            m_diffuseFrontAndBack = false;
            m_diffuseBack = diffuse;
            //clampArray4BetweenRange(m_diffuseBack,0.0f,1.0f,"crMaterial::setDiffuse(..)");
            break;
        case(FRONT_AND_BACK):
            m_diffuseFrontAndBack = true;
            m_diffuseFront = diffuse;
            //clampArray4BetweenRange(m_diffuseFront,0.0f,1.0f,"crMaterial::setDiffuse(..)");
            m_diffuseBack = m_diffuseFront;
            break;
        default:
            notify(NOTICE)<<"Notice: invalid Face passed to crMaterial::setDiffuse()."<< std::endl;
            break;
    }
}


const crVector4& crMaterial::getDiffuse(Face face) const
{
    switch(face)
    {
        case(FRONT):
            return m_diffuseFront;
        case(BACK):
            return m_diffuseBack;
        case(FRONT_AND_BACK):
            if (!m_diffuseFrontAndBack)
            {
                notify(NOTICE)<<"Notice: crMaterial::getDiffuse(FRONT_AND_BACK) called on material "<< std::endl;
                notify(NOTICE)<<"        with separate FRONT and BACK diffuse colors."<< std::endl;
            }
            return m_diffuseFront;
    }
    //notify(NOTICE)<<"Notice: invalid Face passed to crMaterial::getDiffuse()."<< std::endl;
    return m_diffuseFront;
}


void crMaterial::setSpecular(Face face, const crVector4& specular )
{
    switch(face)
    {
        case(FRONT):
            m_specularFrontAndBack = false;
            m_specularFront = specular;
            //clampArray4BetweenRange(m_specularFront,0.0f,1.0f,"crMaterial::setSpecular(..)");
            break;
        case(BACK):
            m_specularFrontAndBack = false;
            m_specularBack = specular;
            //clampArray4BetweenRange(m_specularBack,0.0f,1.0f,"crMaterial::setSpecular(..)");
            break;
        case(FRONT_AND_BACK):
            m_specularFrontAndBack = true;
            m_specularFront = specular;
            //clampArray4BetweenRange(m_specularFront,0.0f,1.0f,"crMaterial::setSpecular(..)");
            m_specularBack = m_specularFront;
            break;
        default:
            notify(NOTICE)<<"Notice: invalid Face passed to crMaterial::setSpecular()."<< std::endl;
            break;
    }
}


const crVector4& crMaterial::getSpecular(Face face) const
{
    switch(face)
    {
        case(FRONT):
            return m_specularFront;
        case(BACK):
            return m_specularBack;
        case(FRONT_AND_BACK):
            if (!m_specularFrontAndBack)
            {
                notify(NOTICE)<<"Notice: crMaterial::getSpecular(FRONT_AND_BACK) called on material "<< std::endl;
                notify(NOTICE)<<"        with separate FRONT and BACK specular colors."<< std::endl;
            }
            return m_specularFront;
    }
    //notify(NOTICE)<<"Notice: invalid Face passed to crMaterial::getSpecular()."<< std::endl;
    return m_specularFront;
}


void crMaterial::setEmission(Face face, const crVector4& emission )
{
    switch(face)
    {
        case(FRONT):
            m_emissionFrontAndBack = false;
            m_emissionFront = emission;
            //clampArray4BetweenRange(m_emissionFront,0.0f,1.0f,"crMaterial::setEmission(..)");
            break;
        case(BACK):
            m_emissionFrontAndBack = false;
            m_emissionBack = emission;
            //clampArray4BetweenRange(m_emissionBack,0.0f,1.0f,"crMaterial::setEmission(..)");
            break;
        case(FRONT_AND_BACK):
            m_emissionFrontAndBack = true;
            m_emissionFront = emission;
            //clampArray4BetweenRange(m_emissionFront,0.0f,1.0f,"crMaterial::setEmission(..)");
            m_emissionBack = m_emissionFront;
            break;
        default:
            notify(NOTICE)<<"Notice: invalid Face passed to crMaterial::setEmission()."<< std::endl;
            break;
    }
}


const crVector4& crMaterial::getEmission(Face face) const
{
    switch(face)
    {
        case(FRONT):
            return m_emissionFront;
        case(BACK):
            return m_emissionBack;
        case(FRONT_AND_BACK):
            if (!m_emissionFrontAndBack)
            {
                notify(NOTICE)<<"Notice: crMaterial::getEmission(FRONT_AND_BACK) called on material "<< std::endl;
                notify(NOTICE)<<"        with separate FRONT and BACK emission colors."<< std::endl;
            }
            return m_emissionFront;
    }
   // notify(NOTICE)<<"Notice: invalid Face passed to crMaterial::getEmission()."<< std::endl;
    return m_emissionFront;
}


void crMaterial::setShininess(Face face, float shininess )
{
    clampBetweenRange(shininess,0.0f,128.0f,"crMaterial::setShininess()");
    
    switch(face)
    {
        case(FRONT):
            m_shininessFrontAndBack = false;
            m_shininessFront = shininess;
            break;
        case(BACK):
            m_shininessFrontAndBack = false;
            m_shininessBack = shininess;
            break;
        case(FRONT_AND_BACK):
            m_shininessFrontAndBack = true;
            m_shininessFront = shininess;
            m_shininessBack = shininess;
            break;
        default:
            notify(NOTICE)<<"Notice: invalid Face passed to crMaterial::setShininess()."<< std::endl;
            break;
    }
}


float crMaterial::getShininess(Face face) const
{
    switch(face)
    {
        case(FRONT):
            return m_shininessFront;
        case(BACK):
            return m_shininessBack;
        case(FRONT_AND_BACK):
            if (!m_shininessFrontAndBack)
            {
                notify(NOTICE)<<"Notice: crMaterial::getShininess(FRONT_AND_BACK) called on material "<< std::endl;
                notify(NOTICE)<<"        with separate FRONT and BACK shininess colors."<< std::endl;
            }
            return m_shininessFront;
    }
    //notify(NOTICE)<<"Notice: invalid Face passed to crMaterial::getShininess()."<< std::endl;
    return m_shininessFront;
}

void crMaterial::setTransparency(Face face,float transparency)
{
   //clampBetweenRange(transparency,0.0f,1.0f,"crMaterial::setTransparency()");

   if (face==FRONT || face==FRONT_AND_BACK)
   {
        m_ambientFront[3] = 1.0f-transparency;
        m_diffuseFront[3] = 1.0f-transparency;
        m_specularFront[3] = 1.0f-transparency;
        m_emissionFront[3] = 1.0f-transparency;
    }

    if (face==BACK || face==FRONT_AND_BACK)
	{
        m_ambientBack[3] = 1.0f-transparency;
        m_diffuseBack[3] = 1.0f-transparency;
        m_specularBack[3] = 1.0f-transparency;
        m_emissionBack[3] = 1.0f-transparency;
    }
}        

void crMaterial::setAlpha(Face face,float alpha)
{
   clampBetweenRange(alpha,0.0f,1.0f,"crMaterial::setAlpha()");

   if (face==FRONT || face==FRONT_AND_BACK)
   {
        m_ambientFront[3] = alpha;
        m_diffuseFront[3] = alpha;
        m_specularFront[3] = alpha;
        //m_emissionFront[3] = alpha;
    }

    if (face==BACK || face==FRONT_AND_BACK)
    {
        m_ambientBack[3] = alpha;
        m_diffuseBack[3] = alpha;
        m_specularBack[3] = alpha;
        //m_emissionBack[3] = alpha;
    }
}

void crMaterial::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
        if(str.compare("AMBIENT") == 0)
		{
			m_colorMode = AMBIENT;
		}
		else if(str.compare("DIFFUSE") == 0)
		{
			m_colorMode = DIFFUSE;
		}
		else if(str.compare("SPECULAR") == 0)
		{
			m_colorMode = SPECULAR;
		}
		else if(str.compare("EMISSION") == 0)
		{
			m_colorMode = EMISSION;
		}
		else if(str.compare("AMBIENT_AND_DIFFUSE") == 0)
		{
			m_colorMode = AMBIENT_AND_DIFFUSE;
		}
		else if(str.compare("OFF") == 0)
		{
			m_colorMode = OFF;
		}
		break;
	case 1:
		{
			crVector4 vec4;
			crArgumentParser::appAtoVec(str,vec4);
			setAmbient(FRONT_AND_BACK,vec4);
		}
		break;
	case 2:
		{
			crVector4 vec4;
			crArgumentParser::appAtoVec(str,vec4);
			setDiffuse(FRONT_AND_BACK,vec4);
		}
		break;
	case 3:
		{
			crVector4 vec4;
			crArgumentParser::appAtoVec(str,vec4);
			setSpecular(FRONT_AND_BACK,vec4);
		}
		break;
	case 4:
		{
			crVector4 vec4;
			crArgumentParser::appAtoVec(str,vec4);
			setEmission(FRONT_AND_BACK,vec4);
		}
		break;
	case 5:
		{
			setShininess(FRONT_AND_BACK,atof(str.c_str()));
		}
		break;
	}

	return crStateAttribute::addAttributeParam(i-6,str);
}
#include <Driver/GLDrv/crMaterialDrv.h>