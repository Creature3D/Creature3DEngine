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
#include <CRCore/crClampColor.h>
#include <CRCore/crState.h>
#include <CRCore/crNotify.h>
#include <crgl/GLExtensions.h>
#include <CRCore/buffered_value.h>
using namespace CRCore;

#ifndef GL_ARB_color_buffer_float
#define GL_RGBA_FLOAT_MODE_ARB                  0x8820
#define GL_CLAMP_VERTEX_COLOR_ARB               0x891A
#define GL_CLAMP_FRAGMENT_COLOR_ARB             0x891B
#define GL_CLAMP_READ_COLOR_ARB                 0x891C
#define GL_FIXED_ONLY_ARB                       0x891D
#endif

#ifndef GL_FIXED_ONLY
#define GL_FIXED_ONLY                           GL_FIXED_ONLY_ARB
#define GL_CLAMP_VERTEX_COLOR                   GL_CLAMP_VERTEX_COLOR_ARB
#define GL_CLAMP_READ_COLOR                     GL_CLAMP_READ_COLOR_ARB
#define GL_CLAMP_FRAGMENT_COLOR                 GL_CLAMP_FRAGMENT_COLOR_ARB
#endif

crClampColor::crClampColor():
   m_clampVertexColor(GL_FIXED_ONLY),
   m_clampFragmentColor(GL_FIXED_ONLY),
   m_clampReadColor(GL_FIXED_ONLY)
{
}

crClampColor::crClampColor(GLenum vertexMode, GLenum fragmentMode, GLenum readMode):
   m_clampVertexColor(vertexMode),
   m_clampFragmentColor(fragmentMode),
   m_clampReadColor(readMode)
{
}

crClampColor::~crClampColor()
{
}

void crClampColor::apply(crState& state) const
{
	// get the contextID (user defined ID of 0 upwards) for the
	// current OpenGL context.
	const unsigned int contextID = state.getContextID();

	const Extensions* extensions = getExtensions(contextID,true);

	if (!extensions->isClampColorSupported())
	{
		notify(WARN)<<"Warning: crClampColor::apply(..) failed, crClampColor is not support by OpenGL driver."<<std::endl;
		return;
	}

	extensions->glClampColor(GL_CLAMP_VERTEX_COLOR, m_clampVertexColor);
	extensions->glClampColor(GL_CLAMP_FRAGMENT_COLOR, m_clampFragmentColor);
	extensions->glClampColor(GL_CLAMP_READ_COLOR, m_clampReadColor);

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crClampColor OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}


typedef buffered_value< ref_ptr<crClampColor::Extensions> > BufferedExtensions;
static BufferedExtensions s_extensions;
void crClampColor::Extensions::clearExtensions()
{
	s_extensions.clear();
}
crClampColor::Extensions* crClampColor::getExtensions(unsigned int contextID,bool createIfNotInitalized)
{
	if (!s_extensions[contextID] && createIfNotInitalized) s_extensions[contextID] = new Extensions(contextID);
	return s_extensions[contextID].get();
}

void crClampColor::setExtensions(unsigned int contextID,Extensions* extensions)
{
	s_extensions[contextID] = extensions;
}

crClampColor::Extensions::Extensions(unsigned int contextID)
{
	setupGLExtensions(contextID);
}

crClampColor::Extensions::Extensions(const Extensions& rhs):
	Referenced()
{
	_isClampColorSupported = rhs._isClampColorSupported;
	_glClampColor = rhs._glClampColor;
}

void crClampColor::Extensions::lowestCommonDenominator(const Extensions& rhs)
{
	if (!rhs._isClampColorSupported) _isClampColorSupported = false;
	if (!rhs._glClampColor)          _glClampColor = 0;
}

void crClampColor::Extensions::setupGLExtensions(unsigned int contextID)
{
	_isClampColorSupported = /*OSG_GL3_FEATURES ||*/
		isGLExtensionSupported(contextID,"GL_ARB_color_buffer_float") ||
		strncmp((const char*)glGetString(GL_VERSION),"2.0",3)>=0;

	setGLExtensionFuncPtr(_glClampColor, "glClampColor", "glClampColorARB");
}

void crClampColor::Extensions::glClampColor(GLenum target, GLenum mode) const
{
	if (_glClampColor)
	{
		_glClampColor(target,mode);
	}
	else
	{
		notify(WARN)<<"Error: glClampColor not supported by OpenGL driver"<<std::endl;
	}
}

void crClampColor::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		if(str.compare("FIXED_ONLY")==0)
		{
			m_clampVertexColor = GL_FIXED_ONLY_ARB;
		}
		else if(str.compare("RGBA_FLOAT_MODE")==0)
		{
			m_clampVertexColor = GL_RGBA_FLOAT_MODE_ARB;
		}
		break;
	case 1:
		if(str.compare("FIXED_ONLY")==0)
		{
			m_clampFragmentColor = GL_FIXED_ONLY_ARB;
		}
		else if(str.compare("RGBA_FLOAT_MODE")==0)
		{
			m_clampFragmentColor = GL_RGBA_FLOAT_MODE_ARB;
		}
		break;
	case 2:
		if(str.compare("FIXED_ONLY")==0)
		{
			m_clampReadColor = GL_FIXED_ONLY_ARB;
		}
		else if(str.compare("RGBA_FLOAT_MODE")==0)
		{
			m_clampReadColor = GL_RGBA_FLOAT_MODE_ARB;
		}
		break;
	}

	return crStateAttribute::addAttributeParam(i-3,str);
}
