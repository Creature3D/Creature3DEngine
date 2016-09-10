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
#include <CRCore/crMultisample.h>
#include <CRCore/crState.h>
#include <crgl/GLExtensions.h>
#include <CRCore/buffered_value.h>
using namespace CRCore;
 

crMultisample::crMultisample() : m_mode(DONT_CARE)
{
    m_coverage = 1;
    m_invert   = false;
}

crMultisample::~crMultisample()
{
}

void crMultisample::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		m_coverage = atof(str.c_str());
		break;
	case 1:
		m_invert = atoi(str.c_str());
		break;
	case 2:
		if(str.compare("FASTEST") == 0)
			m_mode = FASTEST;
		else if (str.compare("NICEST") == 0)
			m_mode = NICEST;
		else if (str.compare("DONT_CARE") == 0)
			m_mode = DONT_CARE;
		break;
	}

	return crStateAttribute::addAttributeParam(i-3,str);
}
//#include <Driver/GLDrv/crMultisampleDrv.h>
void crMultisample::apply(crState& state) const
{
	// get the contextID (user defined ID of 0 upwards) for the
	// current OpenGL context.
	const unsigned int contextID = state.getContextID();

	const Extensions* extensions = getExtensions(contextID,true);

	if (!extensions->isMultisampleSupported())
	{
		notify(WARN)<<"Warning: crMultisample::apply(..) failed, crMultisample is not support by OpenGL driver."<<std::endl;
		return;
	}

	if(extensions->isMultisampleFilterHintSupported())
		glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
	//{//»áÒýÆðºÚÆÁÉÁË¸µÄäÖÈ¾´íÎó
	//	switch (m_mode)
	//	{
	//	case CRCore::crMultisample::FASTEST:
	//		glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_FASTEST);
	//		break;
	//	case CRCore::crMultisample::NICEST:
	//		glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
	//		break;
	//	case CRCore::crMultisample::DONT_CARE:
	//		glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_DONT_CARE);
	//		break;
	//	}
	//}
	extensions->glSampleCoverage(m_coverage, m_invert);
}

typedef buffered_value< ref_ptr<crMultisample::Extensions> > BufferedExtensions;
static BufferedExtensions s_extensions;
void crMultisample::Extensions::clearExtensions()
{
	s_extensions.clear();
}
crMultisample::Extensions* crMultisample::getExtensions(unsigned int contextID,bool createIfNotInitalized)
{
	if (!s_extensions[contextID] && createIfNotInitalized) s_extensions[contextID] = new Extensions(contextID);
	return s_extensions[contextID].get();
}

void crMultisample::setExtensions(unsigned int contextID,Extensions* extensions)
{
	s_extensions[contextID] = extensions;
}


crMultisample::Extensions::Extensions(unsigned int contextID)
{
	setupGLExtensions(contextID);
}

crMultisample::Extensions::Extensions(const Extensions& rhs):
	Referenced()
{
	_isMultisampleSupported = rhs._isMultisampleSupported;
	_isMultisampleFilterHintSupported = rhs._isMultisampleFilterHintSupported;
}

void crMultisample::Extensions::lowestCommonDenominator(const Extensions& rhs)
{
	if (!rhs._isMultisampleSupported)  _isMultisampleSupported = false;
	if (!rhs._isMultisampleFilterHintSupported)  _isMultisampleFilterHintSupported = false;
	if (!rhs._glSampleCoverage)        _glSampleCoverage = 0;
}

void crMultisample::Extensions::setupGLExtensions(unsigned int contextID)
{
	_isMultisampleSupported = /*OSG_GLES2_FEATURES || OSG_GL3_FEATURES || */isGLExtensionSupported(contextID,"GL_ARB_multisample");
	_isMultisampleFilterHintSupported = isGLExtensionSupported(contextID,"GL_NV_multisample_filter_hint");

	setGLExtensionFuncPtr(_glSampleCoverage, "glSampleCoverageARB");
}

void crMultisample::Extensions::glSampleCoverage(GLclampf value, GLboolean invert) const
{
	if (_glSampleCoverage)
	{
		_glSampleCoverage(value, invert);
	}
	else
	{
		notify(WARN)<<"Error: glSampleCoverage not supported by OpenGL driver"<<std::endl;
	}
}