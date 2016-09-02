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
#include <CRCore/crAlphaFunc.h>

using namespace CRCore;

crAlphaFunc::crAlphaFunc()
{
    m_comparisonFunc = ALWAYS;
    m_referenceValue = 1.0f;
}


crAlphaFunc::~crAlphaFunc()
{
}

int crAlphaFunc::compare(const crStateAttribute& sa) const
{
	COMPARE_StateAttribute_Types(crAlphaFunc,sa)

	COMPARE_StateAttribute_Parameter(m_comparisonFunc)
	COMPARE_StateAttribute_Parameter(m_referenceValue)

	return 0;
}

int crAlphaFunc::compare(const crStateAttribute& sa)
{
	COMPARE_StateAttribute_Types(crAlphaFunc,sa)

	COMPARE_StateAttribute_Parameter(m_comparisonFunc)
	COMPARE_StateAttribute_Parameter(m_referenceValue)

	return 0;
}

void crAlphaFunc::getAssociatedModes(std::vector<VRMode>& modes) const 
{
	modes.push_back(GL_ALPHA_TEST);
	//modes.push_back(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
}

void crAlphaFunc::setFunction(ComparisonFunction func,float ref)
{
	m_comparisonFunc = func;
	m_referenceValue = ref;
}

void crAlphaFunc::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		if(str.compare("NEVER")==0)
		{
			m_comparisonFunc = NEVER;
		}
		else if(str.compare("LESS")==0)
		{
			m_comparisonFunc = LESS;
		}
		else if(str.compare("EQUAL")==0)
		{
			m_comparisonFunc = EQUAL;
		}
		else if(str.compare("LEQUAL")==0)
		{
			m_comparisonFunc = LEQUAL;
		}
		else if(str.compare("GREATER")==0)
		{
			m_comparisonFunc = GREATER;
		}
		else if(str.compare("NOTEQUAL")==0)
		{
			m_comparisonFunc = NOTEQUAL;
		}
		else if(str.compare("GEQUAL")==0)
		{
			m_comparisonFunc = GEQUAL;
		}
		else if(str.compare("ALWAYS")==0)
		{
			m_comparisonFunc = ALWAYS;
		}
		break;
	case 1:
		m_referenceValue = atof(str.c_str());
		break;
	}

	return crStateAttribute::addAttributeParam(i-2,str);
}
#include <Driver/GLDrv/crAlphaFuncDrv.h>
