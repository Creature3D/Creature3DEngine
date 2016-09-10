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
#include <CRCore/crBlendEquation.h>
#include <CRCore/crState.h>
#include <CRCore/crNotify.h>
#include <crgl/gl.h>
#include <crgl/glu.h>
#include <crgl/GLExtensions.h>
#include <CRCore/buffered_value.h>
using namespace CRCore;

crBlendEquation::crBlendEquation():
    m_equation(FUNC_ADD)
{
}

crBlendEquation::crBlendEquation(Equation equation):
    m_equation(equation)
{
}

crBlendEquation::~crBlendEquation()
{
}

void crBlendEquation::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		if(str.compare("RGBA_MIN")==0)
		{
			m_equation = RGBA_MIN;
		}
		else if(str.compare("RGBA_MAX")==0)
		{
			m_equation = RGBA_MAX;
		}
		else if(str.compare("ALPHA_MIN")==0)
		{
			m_equation = ALPHA_MIN;
		}
		else if(str.compare("ALPHA_MAX")==0)
		{
			m_equation = ALPHA_MAX;
		}
		else if(str.compare("LOGIC_OP")==0)
		{
			m_equation = LOGIC_OP;
		}
		else if(str.compare("FUNC_ADD")==0)
		{
			m_equation = FUNC_ADD;
		}
		else if(str.compare("FUNC_SUBTRACT")==0)
		{
			m_equation = FUNC_SUBTRACT;
		}
		else if(str.compare("FUNC_REVERSE_SUBTRACT")==0)
		{
			m_equation = FUNC_REVERSE_SUBTRACT;
		}
		break;
	}

	return crStateAttribute::addAttributeParam(i-1,str);
}
//#include <Driver/GLDrv/crBlendEquationDrv.h>
void crBlendEquation::apply(crState& state) const
{

	// get the contextID (user defined ID of 0 upwards) for the
	// current OpenGL context.
	const unsigned int contextID = state.getContextID();

	const Extensions* extensions = getExtensions(contextID,true);

	if (!extensions->isBlendEquationSupported())
	{
		notify(WARN)<<"Warning: crBlendEquation::apply(..) failed, crBlendEquation is not support by OpenGL driver."<<std::endl;
		return;
	}

	switch(m_equation) 
	{
	case crBlendEquation::RGBA_MIN:
		extensions->glBlendEquation(GL_MIN);
		break;
	case crBlendEquation::RGBA_MAX:
		extensions->glBlendEquation(GL_MAX);
		break;
	case crBlendEquation::ALPHA_MIN:
		if(!extensions->isSGIXMinMaxSupported())
		{
			notify(WARN)<<"Warning: BlendEquation::apply(..) failed, SGIX_blend_alpha_minmax extension is not supported by OpenGL driver." << std::endl;
			return;
		}
		extensions->glBlendEquation(GL_ALPHA_MIN_SGIX);
		break;
	case crBlendEquation::ALPHA_MAX:
		if(!extensions->isSGIXMinMaxSupported())
		{
			notify(WARN)<<"Warning: BlendEquation::apply(..) failed, SGIX_blend_alpha_minmax extension is not supported by OpenGL driver." << std::endl;
			return;
		}
		extensions->glBlendEquation(GL_ALPHA_MAX_SGIX);
		break;
	case crBlendEquation::LOGIC_OP:
		if(!extensions->isLogicOpSupported())
		{
			notify(WARN)<<"Warning: BlendEquation::apply(..) failed, EXT_blend_logic_op extension is not supported by OpenGL driver." << std::endl;
			return;
		}
		extensions->glBlendEquation(GL_LOGIC_OP);
		break;
	case crBlendEquation::FUNC_ADD:
		extensions->glBlendEquation(GL_FUNC_ADD);
		break;
	case crBlendEquation::FUNC_SUBTRACT:
		extensions->glBlendEquation(GL_FUNC_SUBTRACT);
		break;
	case crBlendEquation::FUNC_REVERSE_SUBTRACT:
		extensions->glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
		break;
	}
#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crBlendEquation OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}


typedef buffered_value< ref_ptr<crBlendEquation::Extensions> > BufferedExtensions;
static BufferedExtensions s_extensions;
void crBlendEquation::Extensions::clearExtensions()
{
	s_extensions.clear();
}
crBlendEquation::Extensions* crBlendEquation::getExtensions(unsigned int contextID,bool createIfNotInitalized)
{
	if (!s_extensions[contextID] && createIfNotInitalized) s_extensions[contextID] = new Extensions(contextID);
	return s_extensions[contextID].get();
}

void crBlendEquation::setExtensions(unsigned int contextID,Extensions* extensions)
{
	s_extensions[contextID] = extensions;
}


crBlendEquation::Extensions::Extensions(unsigned int contextID)
{
	setupGLExtensions(contextID);
}

crBlendEquation::Extensions::Extensions(const Extensions& rhs):
	Referenced()
{
	_isBlendEquationSupported = rhs._isBlendEquationSupported;
	_isBlendEquationSeparateSupported = rhs._isBlendEquationSeparateSupported;
	_isSGIXMinMaxSupported = rhs._isSGIXMinMaxSupported;
	_isLogicOpSupported = rhs._isLogicOpSupported;
	_glBlendEquation = rhs._glBlendEquation;
	_glBlendEquationSeparate = rhs._glBlendEquationSeparate;
}

void crBlendEquation::Extensions::lowestCommonDenominator(const Extensions& rhs)
{
	if (!rhs._isBlendEquationSupported)  _isBlendEquationSupported = false;
	if (!rhs._glBlendEquation)           _glBlendEquation = 0;
	if (!rhs._isBlendEquationSeparateSupported)  _isBlendEquationSeparateSupported = false;
	if (!rhs._glBlendEquationSeparate)           _glBlendEquationSeparate = 0;
}

void crBlendEquation::Extensions::setupGLExtensions(unsigned int contextID)
{
	//bool bultInSupport = OSG_GLES2_FEATURES || OSG_GL3_FEATURES;
	_isBlendEquationSupported = /*bultInSupport ||*/
		isGLExtensionSupported(contextID, "GL_EXT_blend_equation") ||
		strncmp((const char*)glGetString(GL_VERSION), "1.2", 3) >= 0;

	_isBlendEquationSeparateSupported = /*bultInSupport ||*/
		isGLExtensionSupported(contextID, "GL_EXT_blend_equation_separate") ||
		strncmp((const char*)glGetString(GL_VERSION), "2.0", 3) >= 0;

	_isSGIXMinMaxSupported = isGLExtensionSupported(contextID, "GL_SGIX_blend_alpha_minmax");
	_isLogicOpSupported = isGLExtensionSupported(contextID, "GL_EXT_blend_logic_op");

	setGLExtensionFuncPtr(_glBlendEquation, "glBlendEquation", "glBlendEquationEXT");
	setGLExtensionFuncPtr(_glBlendEquationSeparate, "glBlendEquationSeparate", "glBlendEquationSeparateEXT");
}

void crBlendEquation::Extensions::glBlendEquation(GLenum mode) const
{
	if (_glBlendEquation)
	{
		_glBlendEquation(mode);
	}
	else
	{
		notify(WARN)<<"Error: glBlendEquation not supported by OpenGL driver"<<std::endl;
	}
}

void crBlendEquation::Extensions::glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha) const
{
	if (_glBlendEquationSeparate)
	{
		_glBlendEquationSeparate(modeRGB, modeAlpha);
	}
	else
	{
		notify(WARN)<<"Error: glBlendEquationSeparate not supported by OpenGL driver"<<std::endl;
	}
}
