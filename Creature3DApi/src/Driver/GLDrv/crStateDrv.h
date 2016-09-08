/* Creature3D - Online Game Engine, Copyright (C) 2005 Îâ²Æ»ª(26756325@qq.com)
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
#include <crgl/gl.h>
#include <crgl/glu.h>
#include <crgl/GLExtensions.h>
#include <CRCore/crNotify.h>
#include <CRCore/crStreamBuf.h>

/** macro for use with CRCore::StateAttrbiute::apply methods for detected and 
* reporting OpenGL error messages.*/
#define CRRENDER_GL_DEBUG(message) \
	if (state.getFineGrainedErrorDetection()) \
	{ \
	GLenum errorNo = glGetError(); \
	if (errorNo!=GL_NO_ERROR) \
		{ \
		CRCore::notify(WARN)<<"Warning: detected OpenGL error '"<<gluErrorString(errorNo)<<" "<<message<<endl; \
		}\
	}
//
void crState::initializeExtensionProcs()
{
	setGLExtensionFuncPtr(_glClientActiveTexture,"glClientActiveTexture","glClientActiveTextureARB");
	setGLExtensionFuncPtr(_glActiveTexture, "glActiveTexture","glActiveTextureARB");
	setGLExtensionFuncPtr(_glFogCoordPointer, "glFogCoordPointer","glFogCoordPointerEXT");
	setGLExtensionFuncPtr(_glSecondaryColorPointer, "glSecondaryColorPointer","glSecondaryColorPointerEXT");
	setGLExtensionFuncPtr(_glVertexAttribPointer, "glVertexAttribPointer","glVertexAttribPointerARB");
	//setGLExtensionFuncPtr(_glVertexAttribIPointer, "glVertexAttribIPointer");
	//setGLExtensionFuncPtr(_glVertexAttribLPointer, "glVertexAttribLPointer","glVertexAttribPointerARB");
	setGLExtensionFuncPtr(_glEnableVertexAttribArray, "glEnableVertexAttribArray","glEnableVertexAttribArrayARB");
	//setGLExtensionFuncPtr(_glMultiTexCoord4f, "glMultiTexCoord4f","glMultiTexCoord4fARB");
	//setGLExtensionFuncPtr(_glVertexAttrib4f, "glVertexAttrib4f");
	//setGLExtensionFuncPtr(_glVertexAttrib4fv, "glVertexAttrib4fv");
	setGLExtensionFuncPtr(_glDisableVertexAttribArray, "glDisableVertexAttribArray","glDisableVertexAttribArrayARB");
	//setGLExtensionFuncPtr(_glBindBuffer, "glBindBuffer","glBindBufferARB");

	//setGLExtensionFuncPtr(_glDrawArraysInstanced, "glDrawArraysInstanced","glDrawArraysInstancedARB","glDrawArraysInstancedEXT");
	//setGLExtensionFuncPtr(_glDrawElementsInstanced, "glDrawElementsInstanced","glDrawElementsInstancedARB","glDrawElementsInstancedEXT");
}
void crState::applyProjectionMatrix(const CRCore::RefMatrix* matrix)
{
	if (m_projection!=matrix)
	{
		glMatrixMode( GL_PROJECTION );
		if (matrix) 
		{
			m_projection=matrix;
			glLoadMatrix(matrix->ptr());
		}
		else
		{
			m_projection=m_identity;
			glLoadIdentity();
		}
		glMatrixMode( GL_MODELVIEW );
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::applyProjectionMatrix OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}
        
void crState::applyModelViewMatrix(const CRCore::RefMatrix* matrix)
{
	if (m_modelView!=matrix)
	{
		if (matrix)
		{
			m_modelView=matrix;
			glLoadMatrix(matrix->ptr());
		}
		else 
		{
			m_modelView=m_identity;
			glLoadIdentity();
		}
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::applyModelViewMatrix OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}
        
/** Wrapper around glInterleavedArrays(..).
    * also resets the internal array points and modes within CRCore::crState to keep the other
    * vertex array operations consistent. */
void crState::setInterleavedArrays( unsigned int format, int stride, const void* pointer)
{
    disableAllVertexArrays();

    glInterleavedArrays( format, stride, pointer);

    // the crude way, assume that all arrays have been effected so dirty them and
    // disable them...
	dirtyAllVertexArrays();
}

/** wrapper around glEnableClientState(GL_VERTEX_ARRAY);glVertexPointer(..);
* note, only updates values that change.*/
void crState::setVertexPointer( int size, unsigned int type, int stride, const void *ptr )
{
	if (!m_vertexArray.m_enabled || m_vertexArray.m_dirty)
	{
		m_vertexArray.m_enabled = true;
		glEnableClientState(GL_VERTEX_ARRAY);
	}
	//if (m_vertexArray.m_pointer!=ptr || m_vertexArray.m_dirty)
	{
		m_vertexArray.m_pointer=ptr;
		glVertexPointer( size, type, stride, ptr );
	}
	m_vertexArray.m_dirty = false;

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::setVertexPointer OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif

}

/** wrapper glDisableClientState(GL_VERTEX_ARRAY).
    * note, only updates values that change.*/
void crState::disableVertexPointer()
{
	if (m_vertexArray.m_enabled || m_vertexArray.m_dirty)
	{
		m_vertexArray.m_enabled = false;
		m_vertexArray.m_dirty = false;
		glDisableClientState(GL_VERTEX_ARRAY);
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::disableVertexPointer OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}

/** wrapper around glEnableClientState(GL_NORMAL_ARRAY);glNormalPointer(..);
    * note, only updates values that change.*/
void crState::setNormalPointer( unsigned int type, int stride, const void *ptr )
{
	if (!m_normalArray.m_enabled || m_normalArray.m_dirty)
	{
		m_normalArray.m_enabled = true;
		glEnableClientState(GL_NORMAL_ARRAY);
	}
	//if (m_normalArray.m_pointer!=ptr || m_normalArray.m_dirty)
	{
		m_normalArray.m_pointer=ptr;
		glNormalPointer( type, stride, ptr );
	}
	m_normalArray.m_dirty = false;

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::setNormalPointer OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}

        /** wrapper around glDisableClientState(GL_NORMAL_ARRAY);
          * note, only updates values that change.*/
void crState::disableNormalPointer()
{
	if (m_normalArray.m_enabled || m_normalArray.m_dirty)
	{
		m_normalArray.m_enabled = false;
		m_normalArray.m_dirty = false;
		glDisableClientState(GL_NORMAL_ARRAY);
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::disableNormalPointer OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}
       
/** wrapper around glEnableClientState(GL_COLOR_ARRAY);glColorPointer(..);
    * note, only updates values that change.*/
void crState::setColorPointer( int size, unsigned int type, int stride, const void *ptr )
{
	if (!m_colorArray.m_enabled || m_colorArray.m_dirty)
	{
		m_colorArray.m_enabled = true;
		glEnableClientState(GL_COLOR_ARRAY);
	}
	//if (m_colorArray.m_pointer!=ptr || m_colorArray.m_dirty)
	{
		m_colorArray.m_pointer=ptr;
		glColorPointer( size, type, stride, ptr );
	}
	m_colorArray.m_dirty = false;

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::setColorPointer OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}

/** wrapper around glDisableClientState(GL_COLOR_ARRAY);
    * note, only updates values that change.*/
void crState::disableColorPointer()
{
	if (m_colorArray.m_enabled || m_colorArray.m_dirty)
	{
		m_colorArray.m_enabled = false;
		m_colorArray.m_dirty = false;
		glDisableClientState(GL_COLOR_ARRAY);
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::disableColorPointer OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}
        /** wrapper around glEnableClientState(GL_SECONDARY_COLOR_ARRAY);glSecondayColorPointer(..);
          * note, only updates values that change.*/
//typedef void (APIENTRY * SecondaryColorPointerProc) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void crState::setSecondaryColorPointer( int size, unsigned int type, int stride, const void *ptr )
{
	//static SecondaryColorPointerProc s_glSecondaryColorPointer =
	//	(SecondaryColorPointerProc) getGLExtensionFuncPtr("glSecondaryColorPointer","glSecondaryColorPointerEXT");

	if (_glSecondaryColorPointer)
	{
		if (!m_secondaryColorArray.m_enabled || m_secondaryColorArray.m_dirty)
		{
			m_secondaryColorArray.m_enabled = true;
			glEnableClientState(GL_SECONDARY_COLOR_ARRAY);
		}
		//if (m_secondaryColorArray.m_pointer!=ptr || m_secondaryColorArray.m_dirty)
		{
			m_secondaryColorArray.m_pointer=ptr;
			_glSecondaryColorPointer( size, type, stride, ptr );
		}
		m_secondaryColorArray.m_dirty = false;
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::setSecondaryColorPointer OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}

/** wrapper around glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
    * note, only updates values that change.*/
void crState::disableSecondaryColorPointer()
{
	if (m_secondaryColorArray.m_enabled || m_secondaryColorArray.m_dirty)
	{
		m_secondaryColorArray.m_enabled = false;
		m_secondaryColorArray.m_dirty = false;
		if (isSecondaryColorSupported()) glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::disableSecondaryColorPointer OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}

/** wrapper around glEnableClientState(GL_INDEX_ARRAY);glIndexPointer(..);
    * note, only updates values that change.*/
void crState::setIndexPointer( unsigned int type, int stride, const void *ptr )
{
	if (!m_indexArray.m_enabled || m_indexArray.m_dirty)
	{
		m_indexArray.m_enabled = true;
		glEnableClientState(GL_INDEX_ARRAY);
	}
	//if (m_indexArray.m_pointer!=ptr || m_indexArray.m_dirty)
	{
		m_indexArray.m_pointer=ptr;
		glIndexPointer( type, stride, ptr );
	}
	m_indexArray.m_dirty = false;

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::setIndexPointer OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}

/** wrapper around glDisableClientState(GL_INDEX_ARRAY);
    * note, only updates values that change.*/
void crState::disableIndexPointer()
{
	if (m_indexArray.m_enabled || m_indexArray.m_dirty)
	{
		m_indexArray.m_enabled = false;
		m_indexArray.m_dirty = false;
		glDisableClientState(GL_INDEX_ARRAY);
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::disableIndexPointer OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}

/** wrapper around glEnableClientState(GL_FOG_COORDINATE_ARRAY);glFogCoordPointer(..);
    * note, only updates values that change.*/
//typedef void (APIENTRY * FogCoordPointerProc) (GLenum type, GLsizei stride, const GLvoid *pointer);
void crState::setFogCoordPointer( unsigned int type, int stride, const void *ptr )
{
	//static FogCoordPointerProc s_glFogCoordPointer =
	//	(FogCoordPointerProc) getGLExtensionFuncPtr("glFogCoordPointer","glFogCoordPointerEXT");

	if (_glFogCoordPointer)
	{

		if (!m_fogArray.m_enabled || m_fogArray.m_dirty)
		{
			m_fogArray.m_enabled = true;
			glEnableClientState(GL_FOG_COORDINATE_ARRAY);
		}	
		//if (m_fogArray.m_pointer!=ptr || m_fogArray.m_dirty)
		{
			m_fogArray.m_pointer=ptr;
			_glFogCoordPointer( type, stride, ptr );
		}
		m_fogArray.m_dirty = false;
	}
#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::setFogCoordPointer OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif

}

/** wrapper around glDisableClientState(GL_FOG_COORDINATE_ARRAY);
    * note, only updates values that change.*/
void crState::disableFogCoordPointer()
{
	if (m_fogArray.m_enabled || m_fogArray.m_dirty)
	{
		m_fogArray.m_enabled = false;
		m_fogArray.m_dirty = false;
		if (isFogCoordSupported()) glDisableClientState(GL_FOG_COORDINATE_ARRAY);
	}
#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::disableFogCoordPointer OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif

}

/** wrapper around glEnableClientState(GL_TEXTURE_COORD_ARRAY);glTexCoordPointer(..);
    * note, only updates values that change.*/
void crState::setTexCoordPointer( unsigned int unit,
                                        int size, unsigned int type,
                                        int stride, const void *ptr )
{
	if (setClientActiveTextureUnit(unit))
	{
		if ( unit >= m_texCoordArrayList.size()) m_texCoordArrayList.resize(unit+1);
		EnabledArrayPair& eap = m_texCoordArrayList[unit];

		if (!eap.m_enabled || eap.m_dirty)
		{
			eap.m_enabled = true;
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		//if (eap.m_pointer!=ptr || eap.m_dirty)
		{
			glTexCoordPointer( size, type, stride, ptr );
			eap.m_pointer = ptr;
		}
		eap.m_dirty = false;
	}
#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::setTexCoordPointer OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}

/** wrapper around glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    * note, only updates values that change.*/
void crState::disableTexCoordPointer( unsigned int unit )
{
	if (setClientActiveTextureUnit(unit))
	{
		if ( unit >= m_texCoordArrayList.size()) m_texCoordArrayList.resize(unit+1);
		EnabledArrayPair& eap = m_texCoordArrayList[unit];

		if (eap.m_enabled || eap.m_dirty)
		{
			eap.m_enabled = false;
			eap.m_dirty = false;
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}
#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::disableTexCoordPointer OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}

void crState::disableTexCoordPointersAboveAndIncluding( unsigned int unit )
{
	while (unit<m_texCoordArrayList.size())
	{
		EnabledArrayPair& eap = m_texCoordArrayList[unit];
		if (eap.m_enabled || eap.m_dirty)
		{
			if (setClientActiveTextureUnit(unit))
			{
				eap.m_enabled = false;
				eap.m_dirty = false;
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}
		}
		++unit;
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::disableTexCoordPointersAboveAndIncluding OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}

        /** set the current tex coord array texture unit, return true if selected, false if selection failed such as when multitexturing is not supported.
          * note, only updates values that change.*/
//typedef void (APIENTRY * ActiveTextureProc) (GLenum texture);

bool crState::setClientActiveTextureUnit( unsigned int unit )
{
	if (unit!=m_currentClientActiveTextureUnit)
	{
		//static ActiveTextureProc s_glClientActiveTexture =  
		//	(ActiveTextureProc) getGLExtensionFuncPtr("glClientActiveTexture","glClientActiveTextureARB");

		if (_glClientActiveTexture)
		{
			_glClientActiveTexture(GL_TEXTURE0+unit);
			m_currentClientActiveTextureUnit = unit;
		}
		else
		{
			return unit==0;
		}
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::setClientActiveTextureUnit OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif

	return true;
}

        /** set the current texture unit, return true if selected, false if selection failed such as when multitexturing is not supported.
          * note, only updates values that change.*/
//int numstages;
//glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&numstages);

bool crState::setActiveTextureUnit( unsigned int unit )
{
	if (unit!=m_currentActiveTextureUnit)
	{
		//static ActiveTextureProc s_glActiveTexture =  
		//	(ActiveTextureProc) getGLExtensionFuncPtr("glActiveTexture","glActiveTextureARB");

		if (_glActiveTexture)
		{
			_glActiveTexture(GL_TEXTURE0+unit);
			m_currentActiveTextureUnit = unit;
		}
		else
		{
			return unit==0;
		}
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::setActiveTextureUnit OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
	return true;
}

        /** wrapper around glEnableVertexAttribArrayARB(index);glVertexAttribPointerARB(..);
          * note, only updates values that change.*/
//typedef void (APIENTRY * VertexAttribPointerProc) (unsigned int, GLint, GLenum, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
//typedef void (APIENTRY * EnableVertexAttribProc) (unsigned int);
//typedef void (APIENTRY * DisableVertexAttribProc) (unsigned int);
void crState::setVertexAttribPointer( unsigned int index,
                                     int size, unsigned int type, unsigned char normalized, 
                                     int stride, const void *ptr )
{
	//static VertexAttribPointerProc s_glVertexAttribPointer =  
	//	(VertexAttribPointerProc) getGLExtensionFuncPtr("glVertexAttribPointer","glVertexAttribPointerARB");

	//static EnableVertexAttribProc s_glEnableVertexAttribArray =  
	//	(EnableVertexAttribProc) getGLExtensionFuncPtr("glEnableVertexAttribArray","glEnableVertexAttribArrayARB");

	if( _glVertexAttribPointer )
	{
		if ( index >= m_vertexAttribArrayList.size()) m_vertexAttribArrayList.resize(index+1);
		EnabledArrayPair& eap = m_vertexAttribArrayList[index];

		if (!eap.m_enabled || eap.m_dirty)
		{
			eap.m_enabled = true;
			_glEnableVertexAttribArray( index );
		}
		//if (eap.m_pointer != ptr || eap.m_normalized!=normalized || eap.m_dirty)
		{
			_glVertexAttribPointer( index, size, type, normalized, stride, ptr );
			eap.m_pointer = ptr;
			eap.m_normalized = normalized;
		}
		eap.m_dirty = false;
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::setVertexAttribPointer OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}
        
        /** wrapper around DisableVertexAttribArrayARB(index);
          * note, only updates values that change.*/
void crState::disableVertexAttribPointer( unsigned int index )
{
	//static DisableVertexAttribProc s_glDisableVertexAttribArray =  
	//	(DisableVertexAttribProc) getGLExtensionFuncPtr("glDisableVertexAttribArray","glDisableVertexAttribArrayARB");

	if (_glDisableVertexAttribArray)
	{
		if ( index >= m_vertexAttribArrayList.size()) m_vertexAttribArrayList.resize(index+1);
		EnabledArrayPair& eap = m_vertexAttribArrayList[index];

		if (eap.m_enabled || eap.m_dirty)
		{
			eap.m_enabled = false;
			eap.m_dirty = false;
			_glDisableVertexAttribArray( index );
		}
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::disableVertexAttribPointer OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}
      
void crState::disableVertexAttribPointersAboveAndIncluding( unsigned int index )
{
	//static DisableVertexAttribProc s_glDisableVertexAttribArray =  
	//	(DisableVertexAttribProc) getGLExtensionFuncPtr("glDisableVertexAttribArray","glDisableVertexAttribArrayARB");

	if (_glDisableVertexAttribArray)
	{
		while (index<m_vertexAttribArrayList.size())
		{
			EnabledArrayPair& eap = m_vertexAttribArrayList[index];
			if (eap.m_enabled || eap.m_dirty)
			{
				eap.m_enabled = false;
				eap.m_dirty = false;
				_glDisableVertexAttribArray( index );
			}
			++index;
		}
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::disableVertexAttribPointersAboveAndIncluding OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}
               
//bool crState::checkErrors(const char* str) const 
//{
//	GLenum errorNo = glGetError();
//	if (errorNo!=GL_NO_ERROR)
//	{
//		CRCore::notify(WARN)<<"Warning: detected OpenGL error '"<<gluErrorString(errorNo);
//		if (str) CRCore::notify(WARN)<<"' at "<<str<< std::endl;
//		else CRCore::notify(WARN)<<"' in CRCore::crState."<< std::endl;
//		return true;
//	}
//	return false;
//}
//
//bool crState::checkErrors(crStateAttribute::VRMode mode) const
//{
//	GLenum errorNo = glGetError();
//	if (errorNo!=GL_NO_ERROR)
//	{
//		//CRCore::notify(WARN)<<"Warning: detected OpenGL error '"<<gluErrorString(errorNo)<<"' after applying VRMode 0x"<<hex<<mode<<dec<< std::endl;
//		return true;
//	}
//	return false;
//}
//
//bool crState::checkErrors(const crStateAttribute* attribute) const
//{
//	GLenum errorNo = glGetError();
//	if (errorNo!=GL_NO_ERROR)
//	{
//		//CRCore::notify(WARN)<<"Warning: detected OpenGL error '"<<gluErrorString(errorNo)<<"' after applying attribute "<<attribute->className()<<" "<<attribute<< std::endl;
//		//attribute->apply(*this);
//		return true;
//	}
//	return false;
//}

/** apply an OpenGL mode if required, passing in mode, enable flag and appropriate mode stack */
bool crState::applyMode(crStateAttribute::VRMode mode,bool enabled,ModeStack& ms)
{
	if (ms.last_applied_value != enabled)
	{
		ms.last_applied_value = enabled;
		if (enabled) 
			glEnable(mode);
		else glDisable(mode);

		//if (m_reportErrors) checkErrors(mode.GLMode);
#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crState::applyMode OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
   		return true;
	}
	else
		return false;
}

bool crState::computeSecondaryColorSupported() const
{
	m_isSecondaryColorSupportResolved = true;
	m_isSecondaryColorSupported = isGLExtensionSupported(m_contextID,"GL_EXT_secondary_color");
	return m_isSecondaryColorSupported;
}

bool crState::computeFogCoordSupported() const
{
	m_isFogCoordSupportResolved = true;
	m_isFogCoordSupported = isGLExtensionSupported(m_contextID,"GL_EXT_fog_coord");
	return m_isFogCoordSupported;
}

bool crState::computeVertexBufferObjectSupported() const
{
	m_isVertexBufferObjectSupportResolved = true;
	m_isVertexBufferObjectSupported = isGLExtensionSupported(m_contextID,"GL_ARB_vertex_buffer_object");

	return m_isVertexBufferObjectSupported;
}


