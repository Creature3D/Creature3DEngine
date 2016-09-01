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
#include <CRCore/crBlendFunc.h>
#include <CRCore/crState.h>
#include <CRCore/crNotify.h>
#include <crgl/gl.h>
#include <crgl/glu.h>
#include <crgl/GLExtensions.h>
using namespace CRCore;

crBlendFunc::crBlendFunc():
    m_source_factor(SRC_ALPHA),
    m_destination_factor(ONE_MINUS_SRC_ALPHA),
	m_source_factor_alpha(SRC_ALPHA),
	m_destination_factor_alpha(ONE_MINUS_SRC_ALPHA)
{
}

crBlendFunc::crBlendFunc(unsigned int source, unsigned int destination):
    m_source_factor(source),
    m_destination_factor(destination),
	m_source_factor_alpha(source),
    m_destination_factor_alpha(destination)
{
}

crBlendFunc::crBlendFunc(unsigned int source, unsigned int destination, unsigned int source_alpha, unsigned int destination_alpha):
m_source_factor(source),
m_destination_factor(destination),
m_source_factor_alpha(source_alpha),
m_destination_factor_alpha(destination_alpha)
{
}

crBlendFunc::~crBlendFunc()
{
}

void crBlendFunc::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		if(str.compare("DST_ALPHA")==0)
		{
            m_source_factor = DST_ALPHA;
		}
		else if(str.compare("DST_COLOR")==0)
		{
			m_source_factor = DST_COLOR;
		}
		else if(str.compare("ONE")==0)
		{
			m_source_factor = ONE;
		}
		else if(str.compare("ONE_MINUS_DST_ALPHA")==0)
		{
			m_source_factor = ONE_MINUS_DST_ALPHA;
		}
		else if(str.compare("ONE_MINUS_DST_COLOR")==0)
		{
			m_source_factor = ONE_MINUS_DST_COLOR;
		}
		else if(str.compare("ONE_MINUS_SRC_ALPHA")==0)
		{
			m_source_factor = ONE_MINUS_SRC_ALPHA;
		}
		else if(str.compare("ONE_MINUS_SRC_COLOR")==0)
		{
			m_source_factor = ONE_MINUS_SRC_COLOR;
		}
		else if(str.compare("SRC_ALPHA")==0)
		{
			m_source_factor = SRC_ALPHA;
		}
		else if(str.compare("SRC_ALPHA_SATURATE")==0)
		{
			m_source_factor = SRC_ALPHA_SATURATE;
		}
		else if(str.compare("SRC_COLOR")==0)
		{
			m_source_factor = SRC_COLOR;
		}
		else if(str.compare("CONSTANT_COLOR")==0)
		{
			m_source_factor = CONSTANT_COLOR;
		}
		else if(str.compare("ONE_MINUS_CONSTANT_COLOR")==0)
		{
			m_source_factor = ONE_MINUS_CONSTANT_COLOR;
		}
		else if(str.compare("CONSTANT_ALPHA")==0)
		{
			m_source_factor = CONSTANT_ALPHA;
		}
		else if(str.compare("ONE_MINUS_CONSTANT_ALPHA")==0)
		{
			m_source_factor = ONE_MINUS_CONSTANT_ALPHA;
		}
		else if(str.compare("ZERO")==0)
		{
			m_source_factor = ZERO;
		}
		m_source_factor_alpha = m_source_factor;
		break;
	case 1:
		if(str.compare("DST_ALPHA")==0)
		{
			m_destination_factor = DST_ALPHA;
		}
		else if(str.compare("DST_COLOR")==0)
		{
			m_destination_factor = DST_COLOR;
		}
		else if(str.compare("ONE")==0)
		{
			m_destination_factor = ONE;
		}
		else if(str.compare("ONE_MINUS_DST_ALPHA")==0)
		{
			m_destination_factor = ONE_MINUS_DST_ALPHA;
		}
		else if(str.compare("ONE_MINUS_DST_COLOR")==0)
		{
			m_destination_factor = ONE_MINUS_DST_COLOR;
		}
		else if(str.compare("ONE_MINUS_SRC_ALPHA")==0)
		{
			m_destination_factor = ONE_MINUS_SRC_ALPHA;
		}
		else if(str.compare("ONE_MINUS_SRC_COLOR")==0)
		{
			m_destination_factor = ONE_MINUS_SRC_COLOR;
		}
		else if(str.compare("SRC_ALPHA")==0)
		{
			m_destination_factor = SRC_ALPHA;
		}
		else if(str.compare("SRC_ALPHA_SATURATE")==0)
		{
			m_destination_factor = SRC_ALPHA_SATURATE;
		}
		else if(str.compare("SRC_COLOR")==0)
		{
			m_destination_factor = SRC_COLOR;
		}
		else if(str.compare("CONSTANT_COLOR")==0)
		{
			m_destination_factor = CONSTANT_COLOR;
		}
		else if(str.compare("ONE_MINUS_CONSTANT_COLOR")==0)
		{
			m_destination_factor = ONE_MINUS_CONSTANT_COLOR;
		}
		else if(str.compare("CONSTANT_ALPHA")==0)
		{
			m_destination_factor = CONSTANT_ALPHA;
		}
		else if(str.compare("ONE_MINUS_CONSTANT_ALPHA")==0)
		{
			m_destination_factor = ONE_MINUS_CONSTANT_ALPHA;
		}
		else if(str.compare("ZERO")==0)
		{
			m_destination_factor = ZERO;
		}
		m_destination_factor_alpha = m_destination_factor;
		break;
	case 2:
		if(str.compare("DST_ALPHA")==0)
		{
			m_source_factor_alpha = DST_ALPHA;
		}
		else if(str.compare("DST_COLOR")==0)
		{
			m_source_factor_alpha = DST_COLOR;
		}
		else if(str.compare("ONE")==0)
		{
			m_source_factor_alpha = ONE;
		}
		else if(str.compare("ONE_MINUS_DST_ALPHA")==0)
		{
			m_source_factor_alpha = ONE_MINUS_DST_ALPHA;
		}
		else if(str.compare("ONE_MINUS_DST_COLOR")==0)
		{
			m_source_factor_alpha = ONE_MINUS_DST_COLOR;
		}
		else if(str.compare("ONE_MINUS_SRC_ALPHA")==0)
		{
			m_source_factor_alpha = ONE_MINUS_SRC_ALPHA;
		}
		else if(str.compare("ONE_MINUS_SRC_COLOR")==0)
		{
			m_source_factor_alpha = ONE_MINUS_SRC_COLOR;
		}
		else if(str.compare("SRC_ALPHA")==0)
		{
			m_source_factor_alpha = SRC_ALPHA;
		}
		else if(str.compare("SRC_ALPHA_SATURATE")==0)
		{
			m_source_factor_alpha = SRC_ALPHA_SATURATE;
		}
		else if(str.compare("SRC_COLOR")==0)
		{
			m_source_factor_alpha = SRC_COLOR;
		}
		else if(str.compare("CONSTANT_COLOR")==0)
		{
			m_source_factor_alpha = CONSTANT_COLOR;
		}
		else if(str.compare("ONE_MINUS_CONSTANT_COLOR")==0)
		{
			m_source_factor_alpha = ONE_MINUS_CONSTANT_COLOR;
		}
		else if(str.compare("CONSTANT_ALPHA")==0)
		{
			m_source_factor_alpha = CONSTANT_ALPHA;
		}
		else if(str.compare("ONE_MINUS_CONSTANT_ALPHA")==0)
		{
			m_source_factor_alpha = ONE_MINUS_CONSTANT_ALPHA;
		}
		else if(str.compare("ZERO")==0)
		{
			m_source_factor_alpha = ZERO;
		}
		break;
	case 3:
		if(str.compare("DST_ALPHA")==0)
		{
			m_destination_factor_alpha = DST_ALPHA;
		}
		else if(str.compare("DST_COLOR")==0)
		{
			m_destination_factor_alpha = DST_COLOR;
		}
		else if(str.compare("ONE")==0)
		{
			m_destination_factor_alpha = ONE;
		}
		else if(str.compare("ONE_MINUS_DST_ALPHA")==0)
		{
			m_destination_factor_alpha = ONE_MINUS_DST_ALPHA;
		}
		else if(str.compare("ONE_MINUS_DST_COLOR")==0)
		{
			m_destination_factor_alpha = ONE_MINUS_DST_COLOR;
		}
		else if(str.compare("ONE_MINUS_SRC_ALPHA")==0)
		{
			m_destination_factor_alpha = ONE_MINUS_SRC_ALPHA;
		}
		else if(str.compare("ONE_MINUS_SRC_COLOR")==0)
		{
			m_destination_factor_alpha = ONE_MINUS_SRC_COLOR;
		}
		else if(str.compare("SRC_ALPHA")==0)
		{
			m_destination_factor_alpha = SRC_ALPHA;
		}
		else if(str.compare("SRC_ALPHA_SATURATE")==0)
		{
			m_destination_factor_alpha = SRC_ALPHA_SATURATE;
		}
		else if(str.compare("SRC_COLOR")==0)
		{
			m_destination_factor_alpha = SRC_COLOR;
		}
		else if(str.compare("CONSTANT_COLOR")==0)
		{
			m_destination_factor_alpha = CONSTANT_COLOR;
		}
		else if(str.compare("ONE_MINUS_CONSTANT_COLOR")==0)
		{
			m_destination_factor_alpha = ONE_MINUS_CONSTANT_COLOR;
		}
		else if(str.compare("CONSTANT_ALPHA")==0)
		{
			m_destination_factor_alpha = CONSTANT_ALPHA;
		}
		else if(str.compare("ONE_MINUS_CONSTANT_ALPHA")==0)
		{
			m_destination_factor_alpha = ONE_MINUS_CONSTANT_ALPHA;
		}
		else if(str.compare("ZERO")==0)
		{
			m_destination_factor_alpha = ZERO;
		}
		break;
	}

	return crStateAttribute::addAttributeParam(i-4,str);
}
//#include <Driver/GLDrv/crBlendFuncDrv.h>
#define getFactor(factor)\
{\
	switch(factor)\
	{\
	case crBlendFunc::DST_ALPHA:\
	factor = GL_DST_ALPHA;\
	break;\
	case crBlendFunc::DST_COLOR:\
	factor = GL_DST_COLOR;\
	break;\
	case crBlendFunc::ONE:\
	factor = GL_ONE;\
	break;\
	case crBlendFunc::ONE_MINUS_DST_ALPHA:\
	factor = GL_ONE_MINUS_DST_ALPHA;\
	break;\
	case crBlendFunc::ONE_MINUS_DST_COLOR:\
	factor = GL_ONE_MINUS_DST_COLOR;\
	break;\
	case crBlendFunc::ONE_MINUS_SRC_ALPHA:\
	factor = GL_ONE_MINUS_SRC_ALPHA;\
	break;\
	case crBlendFunc::ONE_MINUS_SRC_COLOR:\
	factor = GL_ONE_MINUS_SRC_COLOR;\
	break;\
	case crBlendFunc::SRC_ALPHA:\
	factor = GL_SRC_ALPHA;\
	break;\
	case crBlendFunc::SRC_ALPHA_SATURATE:\
	factor = GL_SRC_ALPHA_SATURATE;\
	break;\
	case crBlendFunc::SRC_COLOR:\
	factor = GL_SRC_COLOR;\
	break;\
	case crBlendFunc::CONSTANT_COLOR:\
	factor = GL_CONSTANT_COLOR;\
	break;\
	case crBlendFunc::ONE_MINUS_CONSTANT_COLOR:\
	factor = GL_ONE_MINUS_CONSTANT_COLOR;\
	break;\
	case crBlendFunc::ONE_MINUS_CONSTANT_ALPHA:\
	factor = GL_ONE_MINUS_CONSTANT_ALPHA;\
	break;\
	case crBlendFunc::ZERO:\
	factor = GL_ZERO;\
	break;\
}\
}
void crBlendFunc::apply(crState& state) const
{
	unsigned int source_factor = m_source_factor;
	unsigned int destination_factor = m_destination_factor;
	unsigned int source_factor_alpha = m_source_factor_alpha;
	unsigned int destination_factor_alpha = m_destination_factor_alpha;

	getFactor(source_factor);
	getFactor(destination_factor);
	getFactor(source_factor_alpha);
	getFactor(destination_factor_alpha);

	if (source_factor != source_factor_alpha ||
		destination_factor != destination_factor_alpha)
	{
		// get the contextID (user defined ID of 0 upwards) for the
		// current OpenGL context.
		const unsigned int contextID = state.getContextID();

		const Extensions* extensions = getExtensions(contextID,true);

		if (!extensions->isBlendFuncSeparateSupported())
		{
			notify(WARN)<<"Warning: crBlendFunc::apply(..) failed, BlendFuncSeparate is not support by OpenGL driver, falling back to crBlendFunc."<<std::endl;
		}
		else
		{
			extensions->glBlendFuncSeparate(source_factor, destination_factor, source_factor_alpha, destination_factor_alpha);
			return;
		}
	}

	glBlendFunc( source_factor, destination_factor );
}


typedef buffered_value< ref_ptr<crBlendFunc::Extensions> > BufferedExtensions;
static BufferedExtensions s_extensions;
void crBlendFunc::Extensions::clearExtensions()
{
	s_extensions.clear();
}
crBlendFunc::Extensions* crBlendFunc::getExtensions(unsigned int contextID,bool createIfNotInitalized)
{
	if (!s_extensions[contextID] && createIfNotInitalized) s_extensions[contextID] = new Extensions(contextID);
	return s_extensions[contextID].get();
}

void crBlendFunc::setExtensions(unsigned int contextID,Extensions* extensions)
{
	s_extensions[contextID] = extensions;
}


crBlendFunc::Extensions::Extensions(unsigned int contextID)
{
	setupGLExtensions(contextID);
}

crBlendFunc::Extensions::Extensions(const Extensions& rhs):
	Referenced()
{
	_isBlendFuncSeparateSupported = rhs._isBlendFuncSeparateSupported;
	_glBlendFuncSeparate = rhs._glBlendFuncSeparate;
}

void crBlendFunc::Extensions::lowestCommonDenominator(const Extensions& rhs)
{
	if (!rhs._isBlendFuncSeparateSupported)  _isBlendFuncSeparateSupported = false;
	if (!rhs._glBlendFuncSeparate)           _glBlendFuncSeparate = 0;
}

void crBlendFunc::Extensions::setupGLExtensions(unsigned int contextID)
{
	_isBlendFuncSeparateSupported = /*OSG_GLES2_FEATURES || OSG_GL3_FEATURES ||*/
		isGLExtensionSupported(contextID, "GL_EXT_blend_func_separate") ||
		strncmp((const char*)glGetString(GL_VERSION), "1.4", 3) >= 0;

	setGLExtensionFuncPtr(_glBlendFuncSeparate, "glBlendFuncSeparate", "glBlendFuncSeparateEXT");
}

void crBlendFunc::Extensions::glBlendFuncSeparate(GLenum sfactorRGB,
												GLenum dfactorRGB,
												GLenum sfactorAlpha,
												GLenum dfactorAlpha) const
{
	if (_glBlendFuncSeparate)
	{
		_glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
	}
	else
	{
		notify(WARN)<<"Error: glBlendFuncSeparate not supported by OpenGL driver"<<std::endl;
	}
}
