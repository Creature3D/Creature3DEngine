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
#include <CRCore/crBlendColor.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crState.h>
#include <CRCore/crNotify.h>
#include <crgl/gl.h>
#include <crgl/glu.h>
#include <crgl/GLExtensions.h>
using namespace CRCore;

crBlendColor::crBlendColor() :
    m_constantColor(0.0f,0.0f,0.0f,0.0f)
{
}

crBlendColor::~crBlendColor()
{
}

void crBlendColor::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		crArgumentParser::appAtoVec(str,m_constantColor);
	    break;
	}

	return crStateAttribute::addAttributeParam(i-1,str);
}
//#include <Driver/GLDrv/crBlendColorDrv.h>
void crBlendColor::apply(crState& state) const
{
	// get the contextID (user defined ID of 0 upwards) for the
	// current OpenGL context.
	const unsigned int contextID = state.getContextID();

	const Extensions* extensions = getExtensions(contextID,true);

	if (!extensions->isBlendColorSupported())
	{
		notify(CRCore::WARN)<<"Warning: BlendColor::apply(..) failed, BlendColor is not support by OpenGL driver."<<std::endl;
		return;
	}

	extensions->glBlendColor(m_constantColor[0], m_constantColor[1],
		m_constantColor[2], m_constantColor[3]);
#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crBlendColor OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}

typedef buffered_value< ref_ptr<crBlendColor::Extensions> > BufferedExtensions;
static BufferedExtensions s_extensions;
void crBlendColor::Extensions::clearExtensions()
{
	s_extensions.clear();
}

crBlendColor::Extensions* crBlendColor::getExtensions(unsigned int contextID,bool createIfNotInitalized)
{
	if (!s_extensions[contextID] && createIfNotInitalized) s_extensions[contextID] = new Extensions(contextID);
	return s_extensions[contextID].get();
}

void crBlendColor::setExtensions(unsigned int contextID,Extensions* extensions)
{
	s_extensions[contextID] = extensions;
}


crBlendColor::Extensions::Extensions(unsigned int contextID)
{
	setupGLExtensions(contextID);
}

crBlendColor::Extensions::Extensions(const Extensions& rhs):
	Referenced()
{
	_isBlendColorSupported = rhs._isBlendColorSupported;
	_glBlendColor = rhs._glBlendColor;
}

void crBlendColor::Extensions::lowestCommonDenominator(const Extensions& rhs)
{
	if (!rhs._isBlendColorSupported)  _isBlendColorSupported = false;
	if (!rhs._glBlendColor)           _glBlendColor = 0;
}

void crBlendColor::Extensions::setupGLExtensions(unsigned int contextID)
{
	_isBlendColorSupported = /*OSG_GLES2_FEATURES || OSG_GL3_FEATURES ||*/
		isGLExtensionSupported(contextID,"GL_EXT_blend_color") ||
		strncmp((const char*)glGetString(GL_VERSION),"1.2",3)>=0;

	setGLExtensionFuncPtr(_glBlendColor, "glBlendColor", "glBlendColorEXT");
}

void crBlendColor::Extensions::glBlendColor(GLclampf red , GLclampf green , GLclampf blue , GLclampf alpha) const
{
	if (_glBlendColor)
	{
		_glBlendColor(red, green, blue, alpha);
	}
	//else
	//{
	//	notify(CRCore::WARN)<<"Error: glBlendColor not supported by OpenGL driver"<<std::endl;
	//}
#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crBlendColor OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}