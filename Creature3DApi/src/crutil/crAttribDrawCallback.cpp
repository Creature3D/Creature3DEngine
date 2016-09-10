/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/
#include <CRUtil/crAttribDrawCallback.h>
#include <crgl/gl.h>
#include <crgl/glu.h>
#include <CRCore/crNotify.h>
#include <CRCore/crStreamBuf.h>
using namespace CRCore;
using namespace CRUtil;

void crAttribDrawCallback::drawImplementation( const CRCore::crDrawable* pdrawable, crState& state ) const
{
	glPushAttrib(GL_DEPTH_BUFFER_BIT); 
	glDepthMask(GL_FALSE);
	pdrawable->drawImplementation(state);
	glPopAttrib();
    
	////渲染深度
	//glPushAttrib(GL_COLOR_BUFFER_BIT);
	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	////state.applyColorMask(crVector4ub(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
	//pdrawable->drawImplementation(state);
	//// restore color mask settings
	//glPopAttrib();

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crAttribDrawCallback OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}
////////////////////
//crBillboardDrawCallback
////////////////
void crBillboardDrawCallback::drawImplementation( const CRCore::crDrawable* pdrawable, crState& state ) const
{
	// get the current modelview matrix
	state.applyModelViewMatrix(0);
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	pdrawable->drawImplementation(state);

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crBillboardDrawCallback OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}
////////////////////////////////////////////
//crModelBlendDrawCallback
/////////////////////////////////////////
void crModelBlendDrawCallback::drawImplementation( const CRCore::crDrawable* pdrawable, crState& state ) const
{
	glPushAttrib(GL_DEPTH_BUFFER_BIT); 
	glDepthMask(GL_FALSE);
	pdrawable->drawImplementation(state);
	glPopAttrib();

	//渲染深度
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	//state.applyColorMask(crVector4ub(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
	pdrawable->drawImplementation(state);
	// restore color mask settings
	glPopAttrib();

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crModelBlendDrawCallback OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}