/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield
* Copyright (C) 2003-2005 3Dlabs Inc. Ltd.
* Copyright (C) 2004-2005 Nathan Cournia
* Copyright (C) 2007 Art Tevs
* Copyright (C) 2008 Zebra Imaging
* Copyright (C) 2010 VIRES Simulationstechnologie GmbH
* Copyright (C) 2012 David Callu
*
* This application is open source and may be redistributed and/or modified
* freely and without restriction, both in commercial and non commercial
* applications, as long as this copyright notice is maintained.
*
* This application is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include <CRCore/crNotify.h>
#include <CRCore/buffered_value.h>
#include <CRCore/ref_ptr.h>
#include <crgl/GL2Extensions.h>
#include <crgl/GLExtensions.h>
#include <CRCore/crStreamBuf.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crDisplaySettings.h>
#include <CRCore/crFrameBufferObject.h>
#include <CRCore/crTexture2DArray.h>
#include <CRCore/crStencilTwoSided.h>
#include <CRCore/crTexture3D.h>
#include <CRCore/crTextureCubeMap.h>
#include <CRCore/crGeometry.h>

#include <CRCore/crBlendColor.h>
#include <CRCore/crBlendEquation.h>
#include <CRCore/crBlendFunc.h>
#include <CRCore/crClampColor.h>
#include <CRCore/crMultisample.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
using namespace CRCore;


///////////////////////////////////////////////////////////////////////////
// Extension function pointers for OpenGL v2.x
bool GL2Extensions::s_isShaderEnabled = true;
GL2Extensions::GL2Extensions(unsigned int contextID)
{
    setupGL2Extensions(contextID);
}


GL2Extensions::GL2Extensions(const GL2Extensions& rhs) : CRCore::Referenced()
{
    m_glVersion = rhs.m_glVersion;
    m_glslLanguageVersion = rhs.m_glslLanguageVersion;

    m_isShaderObjectsSupported = rhs.m_isShaderObjectsSupported;
    m_isVertexShaderSupported = rhs.m_isVertexShaderSupported;
    m_isFragmentShaderSupported = rhs.m_isFragmentShaderSupported;
    m_isLanguage100Supported = rhs.m_isLanguage100Supported;
    m_isGeometryShader4Supported = rhs.m_isGeometryShader4Supported;
    m_isGpuShader4Supported = rhs.m_isGpuShader4Supported;

    m_glBlendEquationSeparate = rhs.m_glBlendEquationSeparate;
    m_glDrawBuffers = rhs.m_glDrawBuffers;
    m_glStencilOpSeparate = rhs.m_glStencilOpSeparate;
    m_glStencilFuncSeparate = rhs.m_glStencilFuncSeparate;
    m_glStencilMaskSeparate = rhs.m_glStencilMaskSeparate;
    m_glAttachShader = rhs.m_glAttachShader;
    m_glBindAttribLocation = rhs.m_glBindAttribLocation;
    m_glCompileShader = rhs.m_glCompileShader;
    m_glCreateProgram = rhs.m_glCreateProgram;
    m_glCreateShader = rhs.m_glCreateShader;
    m_glDeleteProgram = rhs.m_glDeleteProgram;
    m_glDeleteShader = rhs.m_glDeleteShader;
    m_glDetachShader = rhs.m_glDetachShader;
    m_glDisableVertexAttribArray = rhs.m_glDisableVertexAttribArray;
    m_glEnableVertexAttribArray = rhs.m_glEnableVertexAttribArray;
    m_glGetActiveAttrib = rhs.m_glGetActiveAttrib;
    m_glGetActiveUniform = rhs.m_glGetActiveUniform;
    m_glGetAttachedShaders = rhs.m_glGetAttachedShaders;
    m_glGetAttribLocation = rhs.m_glGetAttribLocation;
    m_glGetProgramiv = rhs.m_glGetProgramiv;
    m_glGetProgramInfoLog = rhs.m_glGetProgramInfoLog;
    m_glGetShaderiv = rhs.m_glGetShaderiv;
    m_glGetShaderInfoLog = rhs.m_glGetShaderInfoLog;
    m_glGetShaderSource = rhs.m_glGetShaderSource;
    m_glGetUniformLocation = rhs.m_glGetUniformLocation;
    m_glGetUniformfv = rhs.m_glGetUniformfv;
    m_glGetUniformiv = rhs.m_glGetUniformiv;
    m_glGetVertexAttribdv = rhs.m_glGetVertexAttribdv;
    m_glGetVertexAttribfv = rhs.m_glGetVertexAttribfv;
    m_glGetVertexAttribiv = rhs.m_glGetVertexAttribiv;
    m_glGetVertexAttribPointerv = rhs.m_glGetVertexAttribPointerv;
    m_glIsProgram = rhs.m_glIsProgram;
    m_glIsShader = rhs.m_glIsShader;
    m_glLinkProgram = rhs.m_glLinkProgram;
    m_glShaderSource = rhs.m_glShaderSource;
    m_glUseProgram = rhs.m_glUseProgram;
    m_glUniform1f = rhs.m_glUniform1f;
    m_glUniform2f = rhs.m_glUniform2f;
    m_glUniform3f = rhs.m_glUniform3f;
    m_glUniform4f = rhs.m_glUniform4f;
    m_glUniform1i = rhs.m_glUniform1i;
    m_glUniform2i = rhs.m_glUniform2i;
    m_glUniform3i = rhs.m_glUniform3i;
    m_glUniform4i = rhs.m_glUniform4i;
    m_glUniform1fv = rhs.m_glUniform1fv;
    m_glUniform2fv = rhs.m_glUniform2fv;
    m_glUniform3fv = rhs.m_glUniform3fv;
    m_glUniform4fv = rhs.m_glUniform4fv;
    m_glUniform1iv = rhs.m_glUniform1iv;
    m_glUniform2iv = rhs.m_glUniform2iv;
    m_glUniform3iv = rhs.m_glUniform3iv;
    m_glUniform4iv = rhs.m_glUniform4iv;
    m_glUniformMatrix2fv = rhs.m_glUniformMatrix2fv;
    m_glUniformMatrix3fv = rhs.m_glUniformMatrix3fv;
    m_glUniformMatrix4fv = rhs.m_glUniformMatrix4fv;
    m_glValidateProgram = rhs.m_glValidateProgram;
    m_glVertexAttrib1d = rhs.m_glVertexAttrib1d;
    m_glVertexAttrib1dv = rhs.m_glVertexAttrib1dv;
    m_glVertexAttrib1f = rhs.m_glVertexAttrib1f;
    m_glVertexAttrib1fv = rhs.m_glVertexAttrib1fv;
    m_glVertexAttrib1s = rhs.m_glVertexAttrib1s;
    m_glVertexAttrib1sv = rhs.m_glVertexAttrib1sv;
    m_glVertexAttrib2d = rhs.m_glVertexAttrib2d;
    m_glVertexAttrib2dv = rhs.m_glVertexAttrib2dv;
    m_glVertexAttrib2f = rhs.m_glVertexAttrib2f;
    m_glVertexAttrib2fv = rhs.m_glVertexAttrib2fv;
    m_glVertexAttrib2s = rhs.m_glVertexAttrib2s;
    m_glVertexAttrib2sv = rhs.m_glVertexAttrib2sv;
    m_glVertexAttrib3d = rhs.m_glVertexAttrib3d;
    m_glVertexAttrib3dv = rhs.m_glVertexAttrib3dv;
    m_glVertexAttrib3f = rhs.m_glVertexAttrib3f;
    m_glVertexAttrib3fv = rhs.m_glVertexAttrib3fv;
    m_glVertexAttrib3s = rhs.m_glVertexAttrib3s;
    m_glVertexAttrib3sv = rhs.m_glVertexAttrib3sv;
    m_glVertexAttrib4Nbv = rhs.m_glVertexAttrib4Nbv;
    m_glVertexAttrib4Niv = rhs.m_glVertexAttrib4Niv;
    m_glVertexAttrib4Nsv = rhs.m_glVertexAttrib4Nsv;
    m_glVertexAttrib4Nub = rhs.m_glVertexAttrib4Nub;
    m_glVertexAttrib4Nubv = rhs.m_glVertexAttrib4Nubv;
    m_glVertexAttrib4Nuiv = rhs.m_glVertexAttrib4Nuiv;
    m_glVertexAttrib4Nusv = rhs.m_glVertexAttrib4Nusv;
    m_glVertexAttrib4bv = rhs.m_glVertexAttrib4bv;
    m_glVertexAttrib4d = rhs.m_glVertexAttrib4d;
    m_glVertexAttrib4dv = rhs.m_glVertexAttrib4dv;
    m_glVertexAttrib4f = rhs.m_glVertexAttrib4f;
    m_glVertexAttrib4fv = rhs.m_glVertexAttrib4fv;
    m_glVertexAttrib4iv = rhs.m_glVertexAttrib4iv;
    m_glVertexAttrib4s = rhs.m_glVertexAttrib4s;
    m_glVertexAttrib4sv = rhs.m_glVertexAttrib4sv;
    m_glVertexAttrib4ubv = rhs.m_glVertexAttrib4ubv;
    m_glVertexAttrib4uiv = rhs.m_glVertexAttrib4uiv;
    m_glVertexAttrib4usv = rhs.m_glVertexAttrib4usv;
    m_glVertexAttribPointer = rhs.m_glVertexAttribPointer;

    m_glGetInfoLogARB = rhs.m_glGetInfoLogARB;
    m_glGetObjectParameterivARB = rhs.m_glGetObjectParameterivARB;
    m_glDeleteObjectARB = rhs.m_glDeleteObjectARB;
    m_glGetHandleARB = rhs.m_glGetHandleARB;

    // GL 2.1
    m_glUniformMatrix2x3fv = rhs.m_glUniformMatrix2x3fv;
    m_glUniformMatrix3x2fv = rhs.m_glUniformMatrix3x2fv;
    m_glUniformMatrix2x4fv = rhs.m_glUniformMatrix2x4fv;
    m_glUniformMatrix4x2fv = rhs.m_glUniformMatrix4x2fv;
    m_glUniformMatrix3x4fv = rhs.m_glUniformMatrix3x4fv;
    m_glUniformMatrix4x3fv = rhs.m_glUniformMatrix4x3fv;

    // EXT_geometry_shader4
    m_glProgramParameteri = rhs.m_glProgramParameteri;
    m_glFramebufferTexture = rhs.m_glFramebufferTexture;
    m_glFramebufferTextureLayer = rhs.m_glFramebufferTextureLayer;
    m_glFramebufferTextureFace = rhs.m_glFramebufferTextureFace;

    // EXT_gpu_shader4
    m_glGetUniformuiv = rhs.m_glGetUniformuiv;
    m_glBindFragDataLocation = rhs.m_glBindFragDataLocation;
    m_glGetFragDataLocation = rhs.m_glGetFragDataLocation;
    m_glUniform1ui = rhs.m_glUniform1ui;
    m_glUniform2ui = rhs.m_glUniform2ui;
    m_glUniform3ui = rhs.m_glUniform3ui;
    m_glUniform4ui = rhs.m_glUniform4ui;
    m_glUniform1uiv = rhs.m_glUniform1uiv;
    m_glUniform2uiv = rhs.m_glUniform2uiv;
    m_glUniform3uiv = rhs.m_glUniform3uiv;
    m_glUniform4uiv = rhs.m_glUniform4uiv;
}


void GL2Extensions::lowestCommonDenominator(const GL2Extensions& rhs)
{
    if (rhs.m_glVersion < m_glVersion) m_glVersion = rhs.m_glVersion;
    if (rhs.m_glslLanguageVersion < m_glslLanguageVersion)
               m_glslLanguageVersion = rhs.m_glslLanguageVersion;

    if (!rhs.m_isShaderObjectsSupported) m_isShaderObjectsSupported = false;
    if (!rhs.m_isVertexShaderSupported) m_isVertexShaderSupported = false;
    if (!rhs.m_isFragmentShaderSupported) m_isFragmentShaderSupported = false;
    if (!rhs.m_isLanguage100Supported) m_isLanguage100Supported = false;
    if (!rhs.m_isGeometryShader4Supported) m_isGeometryShader4Supported = false;
    if (!rhs.m_isGpuShader4Supported) m_isGpuShader4Supported = false;

    if (!rhs.m_glBlendEquationSeparate) m_glBlendEquationSeparate = 0;
    if (!rhs.m_glDrawBuffers) m_glDrawBuffers = 0;
    if (!rhs.m_glStencilOpSeparate) m_glStencilOpSeparate = 0;
    if (!rhs.m_glStencilFuncSeparate) m_glStencilFuncSeparate = 0;
    if (!rhs.m_glStencilMaskSeparate) m_glStencilMaskSeparate = 0;
    if (!rhs.m_glAttachShader) m_glAttachShader = 0;
    if (!rhs.m_glBindAttribLocation) m_glBindAttribLocation = 0;
    if (!rhs.m_glCompileShader) m_glCompileShader = 0;
    if (!rhs.m_glCreateProgram) m_glCreateProgram = 0;
    if (!rhs.m_glCreateShader) m_glCreateShader = 0;
    if (!rhs.m_glDeleteProgram) m_glDeleteProgram = 0;
    if (!rhs.m_glDeleteShader) m_glDeleteShader = 0;
    if (!rhs.m_glDetachShader) m_glDetachShader = 0;
    if (!rhs.m_glDisableVertexAttribArray) m_glDisableVertexAttribArray = 0;
    if (!rhs.m_glEnableVertexAttribArray) m_glEnableVertexAttribArray = 0;
    if (!rhs.m_glGetActiveAttrib) m_glGetActiveAttrib = 0;
    if (!rhs.m_glGetActiveUniform) m_glGetActiveUniform = 0;
    if (!rhs.m_glGetAttachedShaders) m_glGetAttachedShaders = 0;
    if (!rhs.m_glGetAttribLocation) m_glGetAttribLocation = 0;
    if (!rhs.m_glGetProgramiv) m_glGetProgramiv = 0;
    if (!rhs.m_glGetProgramInfoLog) m_glGetProgramInfoLog = 0;
    if (!rhs.m_glGetShaderiv) m_glGetShaderiv = 0;
    if (!rhs.m_glGetShaderInfoLog) m_glGetShaderInfoLog = 0;
    if (!rhs.m_glGetShaderSource) m_glGetShaderSource = 0;
    if (!rhs.m_glGetUniformLocation) m_glGetUniformLocation = 0;
    if (!rhs.m_glGetUniformfv) m_glGetUniformfv = 0;
    if (!rhs.m_glGetUniformiv) m_glGetUniformiv = 0;
    if (!rhs.m_glGetVertexAttribdv) m_glGetVertexAttribdv = 0;
    if (!rhs.m_glGetVertexAttribfv) m_glGetVertexAttribfv = 0;
    if (!rhs.m_glGetVertexAttribiv) m_glGetVertexAttribiv = 0;
    if (!rhs.m_glGetVertexAttribPointerv) m_glGetVertexAttribPointerv = 0;
    if (!rhs.m_glIsProgram) m_glIsProgram = 0;
    if (!rhs.m_glIsShader) m_glIsShader = 0;
    if (!rhs.m_glLinkProgram) m_glLinkProgram = 0;
    if (!rhs.m_glShaderSource) m_glShaderSource = 0;
    if (!rhs.m_glUseProgram) m_glUseProgram = 0;
    if (!rhs.m_glUniform1f) m_glUniform1f = 0;
    if (!rhs.m_glUniform2f) m_glUniform2f = 0;
    if (!rhs.m_glUniform3f) m_glUniform3f = 0;
    if (!rhs.m_glUniform4f) m_glUniform4f = 0;
    if (!rhs.m_glUniform1i) m_glUniform1i = 0;
    if (!rhs.m_glUniform2i) m_glUniform2i = 0;
    if (!rhs.m_glUniform3i) m_glUniform3i = 0;
    if (!rhs.m_glUniform4i) m_glUniform4i = 0;
    if (!rhs.m_glUniform1fv) m_glUniform1fv = 0;
    if (!rhs.m_glUniform2fv) m_glUniform2fv = 0;
    if (!rhs.m_glUniform3fv) m_glUniform3fv = 0;
    if (!rhs.m_glUniform4fv) m_glUniform4fv = 0;
    if (!rhs.m_glUniform1iv) m_glUniform1iv = 0;
    if (!rhs.m_glUniform2iv) m_glUniform2iv = 0;
    if (!rhs.m_glUniform3iv) m_glUniform3iv = 0;
    if (!rhs.m_glUniform4iv) m_glUniform4iv = 0;
    if (!rhs.m_glUniformMatrix2fv) m_glUniformMatrix2fv = 0;
    if (!rhs.m_glUniformMatrix3fv) m_glUniformMatrix3fv = 0;
    if (!rhs.m_glUniformMatrix4fv) m_glUniformMatrix4fv = 0;
    if (!rhs.m_glValidateProgram) m_glValidateProgram = 0;
    if (!rhs.m_glVertexAttrib1d) m_glVertexAttrib1d = 0;
    if (!rhs.m_glVertexAttrib1dv) m_glVertexAttrib1dv = 0;
    if (!rhs.m_glVertexAttrib1f) m_glVertexAttrib1f = 0;
    if (!rhs.m_glVertexAttrib1fv) m_glVertexAttrib1fv = 0;
    if (!rhs.m_glVertexAttrib1s) m_glVertexAttrib1s = 0;
    if (!rhs.m_glVertexAttrib1sv) m_glVertexAttrib1sv = 0;
    if (!rhs.m_glVertexAttrib2d) m_glVertexAttrib2d = 0;
    if (!rhs.m_glVertexAttrib2dv) m_glVertexAttrib2dv = 0;
    if (!rhs.m_glVertexAttrib2f) m_glVertexAttrib2f = 0;
    if (!rhs.m_glVertexAttrib2fv) m_glVertexAttrib2fv = 0;
    if (!rhs.m_glVertexAttrib2s) m_glVertexAttrib2s = 0;
    if (!rhs.m_glVertexAttrib2sv) m_glVertexAttrib2sv = 0;
    if (!rhs.m_glVertexAttrib3d) m_glVertexAttrib3d = 0;
    if (!rhs.m_glVertexAttrib3dv) m_glVertexAttrib3dv = 0;
    if (!rhs.m_glVertexAttrib3f) m_glVertexAttrib3f = 0;
    if (!rhs.m_glVertexAttrib3fv) m_glVertexAttrib3fv = 0;
    if (!rhs.m_glVertexAttrib3s) m_glVertexAttrib3s = 0;
    if (!rhs.m_glVertexAttrib3sv) m_glVertexAttrib3sv = 0;
    if (!rhs.m_glVertexAttrib4Nbv) m_glVertexAttrib4Nbv = 0;
    if (!rhs.m_glVertexAttrib4Niv) m_glVertexAttrib4Niv = 0;
    if (!rhs.m_glVertexAttrib4Nsv) m_glVertexAttrib4Nsv = 0;
    if (!rhs.m_glVertexAttrib4Nub) m_glVertexAttrib4Nub = 0;
    if (!rhs.m_glVertexAttrib4Nubv) m_glVertexAttrib4Nubv = 0;
    if (!rhs.m_glVertexAttrib4Nuiv) m_glVertexAttrib4Nuiv = 0;
    if (!rhs.m_glVertexAttrib4Nusv) m_glVertexAttrib4Nusv = 0;
    if (!rhs.m_glVertexAttrib4bv) m_glVertexAttrib4bv = 0;
    if (!rhs.m_glVertexAttrib4d) m_glVertexAttrib4d = 0;
    if (!rhs.m_glVertexAttrib4dv) m_glVertexAttrib4dv = 0;
    if (!rhs.m_glVertexAttrib4f) m_glVertexAttrib4f = 0;
    if (!rhs.m_glVertexAttrib4fv) m_glVertexAttrib4fv = 0;
    if (!rhs.m_glVertexAttrib4iv) m_glVertexAttrib4iv = 0;
    if (!rhs.m_glVertexAttrib4s) m_glVertexAttrib4s = 0;
    if (!rhs.m_glVertexAttrib4sv) m_glVertexAttrib4sv = 0;
    if (!rhs.m_glVertexAttrib4ubv) m_glVertexAttrib4ubv = 0;
    if (!rhs.m_glVertexAttrib4uiv) m_glVertexAttrib4uiv = 0;
    if (!rhs.m_glVertexAttrib4usv) m_glVertexAttrib4usv = 0;
    if (!rhs.m_glVertexAttribPointer) m_glVertexAttribPointer = 0;

    if (!rhs.m_glGetInfoLogARB) m_glGetInfoLogARB = 0;
    if (!rhs.m_glGetObjectParameterivARB) m_glGetObjectParameterivARB = 0;
    if (!rhs.m_glDeleteObjectARB) m_glDeleteObjectARB = 0;
    if (!rhs.m_glGetHandleARB) m_glGetHandleARB = 0;

    // GL 2.1
    if (!rhs.m_glUniformMatrix2x3fv) m_glUniformMatrix2x3fv = 0;
    if (!rhs.m_glUniformMatrix3x2fv) m_glUniformMatrix3x2fv = 0;
    if (!rhs.m_glUniformMatrix2x4fv) m_glUniformMatrix2x4fv = 0;
    if (!rhs.m_glUniformMatrix4x2fv) m_glUniformMatrix4x2fv = 0;
    if (!rhs.m_glUniformMatrix3x4fv) m_glUniformMatrix3x4fv = 0;
    if (!rhs.m_glUniformMatrix4x3fv) m_glUniformMatrix4x3fv = 0;

    // EXT_geometry_shader4
    if (!rhs.m_glProgramParameteri) m_glProgramParameteri = 0;
    if (!rhs.m_glFramebufferTexture) m_glFramebufferTexture = 0;
    if (!rhs.m_glFramebufferTextureLayer) m_glFramebufferTextureLayer = 0;
    if (!rhs.m_glFramebufferTextureFace) m_glFramebufferTextureFace = 0;

    // EXT_gpu_shader4
    if (!rhs.m_glGetUniformuiv) m_glGetUniformuiv = 0;
    if (!rhs.m_glBindFragDataLocation) m_glBindFragDataLocation = 0;
    if (!rhs.m_glGetFragDataLocation) m_glGetFragDataLocation = 0;
    if (!rhs.m_glUniform1ui) m_glUniform1ui = 0;
    if (!rhs.m_glUniform2ui) m_glUniform2ui = 0;
    if (!rhs.m_glUniform3ui) m_glUniform3ui = 0;
    if (!rhs.m_glUniform4ui) m_glUniform4ui = 0;
    if (!rhs.m_glUniform1uiv) m_glUniform1uiv = 0;
    if (!rhs.m_glUniform2uiv) m_glUniform2uiv = 0;
    if (!rhs.m_glUniform3uiv) m_glUniform3uiv = 0;
    if (!rhs.m_glUniform4uiv) m_glUniform4uiv = 0;
}


void GL2Extensions::setupGL2Extensions(unsigned int contextID)
{
    const char* version = (const char*) glGetString( GL_VERSION );
    if (!version)
    {
		MessageBox(::GetActiveWindow(),"未检测到显卡OpenGL版本信息，请尝试重新安装显卡驱动！","Creature3D",MB_OK);
        CRCore::notify(CRCore::FATAL)<<"Error: OpenGL version test failed, requires valid graphics context."<<std::endl;
        return;
    }
	s_isShaderEnabled = !crDisplaySettings::instance()->getDisableShader();

    m_glVersion = atof( version );
    m_glslLanguageVersion = 0.0f;

    m_isShaderObjectsSupported = CRCore::isGLExtensionSupported(contextID,"GL_ARB_shader_objects");
    m_isVertexShaderSupported = CRCore::isGLExtensionSupported(contextID,"GL_ARB_vertex_shader");
    m_isFragmentShaderSupported = CRCore::isGLExtensionSupported(contextID,"GL_ARB_fragment_shader");
    m_isLanguage100Supported = CRCore::isGLExtensionSupported(contextID,"GL_ARB_shading_language_100");
    m_isGeometryShader4Supported = CRCore::isGLExtensionSupported(contextID,"GL_EXT_geometry_shader4");
    m_isGpuShader4Supported = CRCore::isGLExtensionSupported(contextID,"GL_EXT_gpu_shader4");

    if( isGlslSupported() )
    {
        // If glGetString raises an error, assume initial release "1.00"
        while(glGetError() != GL_NO_ERROR) {}        // reset error flag
        const char* langVerStr = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
        if( (glGetError() == GL_NO_ERROR) && langVerStr )
            m_glslLanguageVersion = atof( langVerStr );
        else
            m_glslLanguageVersion = 1.0f;
    }

    //CRCore::notify(CRCore::INFO)
    //        << "glVersion=" << getGlVersion() << ", "
    //        << "isGlslSupported=" << (isGlslSupported() ? "YES" : "NO") << ", "
    //        << "glslLanguageVersion=" << getLanguageVersion()
    //        << std::endl;


    m_glBlendEquationSeparate = CRCore::getGLExtensionFuncPtr("glBlendEquationSeparate");
    m_glDrawBuffers = CRCore::getGLExtensionFuncPtr("glDrawBuffers", "glDrawBuffersARB");
    m_glStencilOpSeparate = CRCore::getGLExtensionFuncPtr("glStencilOpSeparate");
    m_glStencilFuncSeparate = CRCore::getGLExtensionFuncPtr("glStencilFuncSeparate");
    m_glStencilMaskSeparate = CRCore::getGLExtensionFuncPtr("glStencilMaskSeparate");
    m_glAttachShader = CRCore::getGLExtensionFuncPtr("glAttachShader", "glAttachObjectARB");
    m_glBindAttribLocation = CRCore::getGLExtensionFuncPtr("glBindAttribLocation", "glBindAttribLocationARB");
    m_glCompileShader = CRCore::getGLExtensionFuncPtr("glCompileShader", "glCompileShaderARB");
    m_glCreateProgram = CRCore::getGLExtensionFuncPtr("glCreateProgram", "glCreateProgramObjectARB");
    m_glCreateShader = CRCore::getGLExtensionFuncPtr("glCreateShader", "glCreateShaderObjectARB");
    m_glDeleteProgram = CRCore::getGLExtensionFuncPtr("glDeleteProgram");
    m_glDeleteShader = CRCore::getGLExtensionFuncPtr("glDeleteShader");
    m_glDetachShader = CRCore::getGLExtensionFuncPtr("glDetachShader", "glDetachObjectARB");
    m_glDisableVertexAttribArray = CRCore::getGLExtensionFuncPtr("glDisableVertexAttribArray");
    m_glEnableVertexAttribArray = CRCore::getGLExtensionFuncPtr("glEnableVertexAttribArray");
    m_glGetActiveAttrib = CRCore::getGLExtensionFuncPtr("glGetActiveAttrib", "glGetActiveAttribARB");
    m_glGetActiveUniform = CRCore::getGLExtensionFuncPtr("glGetActiveUniform", "glGetActiveUniformARB");
    m_glGetAttachedShaders = CRCore::getGLExtensionFuncPtr("glGetAttachedShaders", "glGetAttachedObjectsARB");
    m_glGetAttribLocation = CRCore::getGLExtensionFuncPtr("glGetAttribLocation", "glGetAttribLocationARB");
    m_glGetProgramiv = CRCore::getGLExtensionFuncPtr("glGetProgramiv");
    m_glGetProgramInfoLog = CRCore::getGLExtensionFuncPtr("glGetProgramInfoLog");
    m_glGetShaderiv = CRCore::getGLExtensionFuncPtr("glGetShaderiv");
    m_glGetShaderInfoLog = CRCore::getGLExtensionFuncPtr("glGetShaderInfoLog");
    m_glGetShaderSource = CRCore::getGLExtensionFuncPtr("glGetShaderSource", "glGetShaderSourceARB");
    m_glGetUniformLocation = CRCore::getGLExtensionFuncPtr("glGetUniformLocation", "glGetUniformLocationARB");
    m_glGetUniformfv = CRCore::getGLExtensionFuncPtr("glGetUniformfv", "glGetUniformfvARB");
    m_glGetUniformiv = CRCore::getGLExtensionFuncPtr("glGetUniformiv", "glGetUniformivARB");
    m_glGetVertexAttribdv = CRCore::getGLExtensionFuncPtr("glGetVertexAttribdv");
    m_glGetVertexAttribfv = CRCore::getGLExtensionFuncPtr("glGetVertexAttribfv");
    m_glGetVertexAttribiv = CRCore::getGLExtensionFuncPtr("glGetVertexAttribiv");
    m_glGetVertexAttribPointerv = CRCore::getGLExtensionFuncPtr("glGetVertexAttribPointerv");
    m_glIsProgram = CRCore::getGLExtensionFuncPtr("glIsProgram");
    m_glIsShader = CRCore::getGLExtensionFuncPtr("glIsShader");
    m_glLinkProgram = CRCore::getGLExtensionFuncPtr("glLinkProgram", "glLinkProgramARB");
    m_glShaderSource = CRCore::getGLExtensionFuncPtr("glShaderSource", "glShaderSourceARB");
    m_glUseProgram = CRCore::getGLExtensionFuncPtr("glUseProgram", "glUseProgramObjectARB");
    m_glUniform1f = CRCore::getGLExtensionFuncPtr("glUniform1f", "glUniform1fARB");
    m_glUniform2f = CRCore::getGLExtensionFuncPtr("glUniform2f", "glUniform2fARB");
    m_glUniform3f = CRCore::getGLExtensionFuncPtr("glUniform3f", "glUniform3fARB");
    m_glUniform4f = CRCore::getGLExtensionFuncPtr("glUniform4f", "glUniform4fARB");
    m_glUniform1i = CRCore::getGLExtensionFuncPtr("glUniform1i", "glUniform1iARB");
    m_glUniform2i = CRCore::getGLExtensionFuncPtr("glUniform2i", "glUniform2iARB");
    m_glUniform3i = CRCore::getGLExtensionFuncPtr("glUniform3i", "glUniform3iARB");
    m_glUniform4i = CRCore::getGLExtensionFuncPtr("glUniform4i", "glUniform4iARB");
    m_glUniform1fv = CRCore::getGLExtensionFuncPtr("glUniform1fv", "glUniform1fvARB");
    m_glUniform2fv = CRCore::getGLExtensionFuncPtr("glUniform2fv", "glUniform2fvARB");
    m_glUniform3fv = CRCore::getGLExtensionFuncPtr("glUniform3fv", "glUniform3fvARB");
    m_glUniform4fv = CRCore::getGLExtensionFuncPtr("glUniform4fv", "glUniform4fvARB");
    m_glUniform1iv = CRCore::getGLExtensionFuncPtr("glUniform1iv", "glUniform1ivARB");
    m_glUniform2iv = CRCore::getGLExtensionFuncPtr("glUniform2iv", "glUniform2ivARB");
    m_glUniform3iv = CRCore::getGLExtensionFuncPtr("glUniform3iv", "glUniform3ivARB");
    m_glUniform4iv = CRCore::getGLExtensionFuncPtr("glUniform4iv", "glUniform4ivARB");
    m_glUniformMatrix2fv = CRCore::getGLExtensionFuncPtr("glUniformMatrix2fv", "glUniformMatrix2fvARB");
    m_glUniformMatrix3fv = CRCore::getGLExtensionFuncPtr("glUniformMatrix3fv", "glUniformMatrix3fvARB");
    m_glUniformMatrix4fv = CRCore::getGLExtensionFuncPtr("glUniformMatrix4fv", "glUniformMatrix4fvARB");
    m_glValidateProgram = CRCore::getGLExtensionFuncPtr("glValidateProgram", "glValidateProgramARB");
    m_glVertexAttrib1d = CRCore::getGLExtensionFuncPtr("glVertexAttrib1d");
    m_glVertexAttrib1dv = CRCore::getGLExtensionFuncPtr("glVertexAttrib1dv");
    m_glVertexAttrib1f = CRCore::getGLExtensionFuncPtr("glVertexAttrib1f");
    m_glVertexAttrib1fv = CRCore::getGLExtensionFuncPtr("glVertexAttrib1fv");
    m_glVertexAttrib1s = CRCore::getGLExtensionFuncPtr("glVertexAttrib1s");
    m_glVertexAttrib1sv = CRCore::getGLExtensionFuncPtr("glVertexAttrib1sv");
    m_glVertexAttrib2d = CRCore::getGLExtensionFuncPtr("glVertexAttrib2d");
    m_glVertexAttrib2dv = CRCore::getGLExtensionFuncPtr("glVertexAttrib2dv");
    m_glVertexAttrib2f = CRCore::getGLExtensionFuncPtr("glVertexAttrib2f");
    m_glVertexAttrib2fv = CRCore::getGLExtensionFuncPtr("glVertexAttrib2fv");
    m_glVertexAttrib2s = CRCore::getGLExtensionFuncPtr("glVertexAttrib2s");
    m_glVertexAttrib2sv = CRCore::getGLExtensionFuncPtr("glVertexAttrib2sv");
    m_glVertexAttrib3d = CRCore::getGLExtensionFuncPtr("glVertexAttrib3d");
    m_glVertexAttrib3dv = CRCore::getGLExtensionFuncPtr("glVertexAttrib3dv");
    m_glVertexAttrib3f = CRCore::getGLExtensionFuncPtr("glVertexAttrib3f");
    m_glVertexAttrib3fv = CRCore::getGLExtensionFuncPtr("glVertexAttrib3fv");
    m_glVertexAttrib3s = CRCore::getGLExtensionFuncPtr("glVertexAttrib3s");
    m_glVertexAttrib3sv = CRCore::getGLExtensionFuncPtr("glVertexAttrib3sv");
    m_glVertexAttrib4Nbv = CRCore::getGLExtensionFuncPtr("glVertexAttrib4Nbv");
    m_glVertexAttrib4Niv = CRCore::getGLExtensionFuncPtr("glVertexAttrib4Niv");
    m_glVertexAttrib4Nsv = CRCore::getGLExtensionFuncPtr("glVertexAttrib4Nsv");
    m_glVertexAttrib4Nub = CRCore::getGLExtensionFuncPtr("glVertexAttrib4Nub");
    m_glVertexAttrib4Nubv = CRCore::getGLExtensionFuncPtr("glVertexAttrib4Nubv");
    m_glVertexAttrib4Nuiv = CRCore::getGLExtensionFuncPtr("glVertexAttrib4Nuiv");
    m_glVertexAttrib4Nusv = CRCore::getGLExtensionFuncPtr("glVertexAttrib4Nusv");
    m_glVertexAttrib4bv = CRCore::getGLExtensionFuncPtr("glVertexAttrib4bv");
    m_glVertexAttrib4d = CRCore::getGLExtensionFuncPtr("glVertexAttrib4d");
    m_glVertexAttrib4dv = CRCore::getGLExtensionFuncPtr("glVertexAttrib4dv");
    m_glVertexAttrib4f = CRCore::getGLExtensionFuncPtr("glVertexAttrib4f");
    m_glVertexAttrib4fv = CRCore::getGLExtensionFuncPtr("glVertexAttrib4fv");
    m_glVertexAttrib4iv = CRCore::getGLExtensionFuncPtr("glVertexAttrib4iv");
    m_glVertexAttrib4s = CRCore::getGLExtensionFuncPtr("glVertexAttrib4s");
    m_glVertexAttrib4sv = CRCore::getGLExtensionFuncPtr("glVertexAttrib4sv");
    m_glVertexAttrib4ubv = CRCore::getGLExtensionFuncPtr("glVertexAttrib4ubv");
    m_glVertexAttrib4uiv = CRCore::getGLExtensionFuncPtr("glVertexAttrib4uiv");
    m_glVertexAttrib4usv = CRCore::getGLExtensionFuncPtr("glVertexAttrib4usv");
    m_glVertexAttribPointer = CRCore::getGLExtensionFuncPtr("glVertexAttribPointer");

    // v1.5-only ARB entry points, in case they're needed for fallback
    m_glGetInfoLogARB = CRCore::getGLExtensionFuncPtr("glGetInfoLogARB");
    m_glGetObjectParameterivARB = CRCore::getGLExtensionFuncPtr("glGetObjectParameterivARB");
    m_glDeleteObjectARB = CRCore::getGLExtensionFuncPtr("glDeleteObjectARB");
    m_glGetHandleARB = CRCore::getGLExtensionFuncPtr("glGetHandleARB");

    // GL 2.1
    m_glUniformMatrix2x3fv = CRCore::getGLExtensionFuncPtr( "glUniformMatrix2x3fv" );
    m_glUniformMatrix3x2fv = CRCore::getGLExtensionFuncPtr( "glUniformMatrix3x2fv" );
    m_glUniformMatrix2x4fv = CRCore::getGLExtensionFuncPtr( "glUniformMatrix2x4fv" );
    m_glUniformMatrix4x2fv = CRCore::getGLExtensionFuncPtr( "glUniformMatrix4x2fv" );
    m_glUniformMatrix3x4fv = CRCore::getGLExtensionFuncPtr( "glUniformMatrix3x4fv" );
    m_glUniformMatrix4x3fv = CRCore::getGLExtensionFuncPtr( "glUniformMatrix4x3fv" );

    // EXT_geometry_shader4
    m_glProgramParameteri = CRCore::getGLExtensionFuncPtr( "glProgramParameteri", "glProgramParameteriEXT" );
    m_glFramebufferTexture = CRCore::getGLExtensionFuncPtr( "glFramebufferTexture", "glFramebufferTextureEXT" );
    m_glFramebufferTextureLayer = CRCore::getGLExtensionFuncPtr( "glFramebufferTextureLayer", "glFramebufferTextureLayerEXT" );
    m_glFramebufferTextureFace = CRCore::getGLExtensionFuncPtr( "glFramebufferTextureFace", "glFramebufferTextureFaceEXT" );

    // EXT_gpu_shader4
    m_glGetUniformuiv = CRCore::getGLExtensionFuncPtr( "glGetUniformuiv", "glGetUniformuivEXT" );
    m_glBindFragDataLocation = CRCore::getGLExtensionFuncPtr( "glBindFragDataLocation", "glBindFragDataLocationEXT" );
    m_glGetFragDataLocation = CRCore::getGLExtensionFuncPtr( "glGetFragDataLocation", "glGetFragDataLocationEXT" );
    m_glUniform1ui = CRCore::getGLExtensionFuncPtr( "glUniform1ui", "glUniform1uiEXT" );
    m_glUniform2ui = CRCore::getGLExtensionFuncPtr( "glUniform2ui", "glUniform2uiEXT" );
    m_glUniform3ui = CRCore::getGLExtensionFuncPtr( "glUniform3ui", "glUniform3uiEXT" );
    m_glUniform4ui = CRCore::getGLExtensionFuncPtr( "glUniform4ui", "glUniform4uiEXT" );
    m_glUniform1uiv = CRCore::getGLExtensionFuncPtr( "glUniform1uiv", "glUniform1uivEXT" );
    m_glUniform2uiv = CRCore::getGLExtensionFuncPtr( "glUniform2uiv", "glUniform2uivEXT" );
    m_glUniform3uiv = CRCore::getGLExtensionFuncPtr( "glUniform3uiv", "glUniform3uivEXT" );
    m_glUniform4uiv = CRCore::getGLExtensionFuncPtr( "glUniform4uiv", "glUniform4uivEXT" );

	//////////////////////////////
	m_glDeleteBuffers = CRCore::getGLExtensionFuncPtr("glDeleteBuffers","glDeleteBuffersARB");

	ref_ptr<crStreamBuf> stream = new crStreamBuf;
	stream->createBuf(512);
	const char* renderer = (const char*) glGetString(GL_RENDERER);
	std::string str = "RenderInfo:";
	str +=renderer;
	//if(str.find("Intel")!=std::string::npos)
	//{
	//	s_isShaderEnabled = false;
	//}
	//output to file
	str += "\n";
	stream->write(str.c_str(),str.length());
	str = "glVersion="+crArgumentParser::appFtoa(m_glVersion) + "\n";
	stream->write(str.c_str(),str.length());
	str = "glslLanguageVersion="+crArgumentParser::appFtoa(m_glslLanguageVersion) + "\n";
	stream->write(str.c_str(),str.length());
	str = "ShaderObjectsSupported="+crArgumentParser::appItoa(m_isShaderObjectsSupported?1:0) + "\n";
	stream->write(str.c_str(),str.length());
	str = "VertexShaderSupported="+crArgumentParser::appItoa(m_isVertexShaderSupported?1:0) + "\n";
	stream->write(str.c_str(),str.length());
	str = "FragmentShaderSupported="+crArgumentParser::appItoa(m_isFragmentShaderSupported?1:0) + "\n";
	stream->write(str.c_str(),str.length());
	str = "Language100Supported="+crArgumentParser::appItoa(m_isLanguage100Supported?1:0) + "\n";
	stream->write(str.c_str(),str.length());
	str = "GeometryShader4Supported="+crArgumentParser::appItoa(m_isGeometryShader4Supported?1:0) + "\n";
	stream->write(str.c_str(),str.length());
	str = "GpuShader4Supported="+crArgumentParser::appItoa(m_isGpuShader4Supported?1:0) + "\n";
	stream->write(str.c_str(),str.length());
	stream->seekBegin();
	stream->saveToFile2("glinfo.txt",true);
	std::string rendererString(renderer ? renderer : "");
	if (rendererString.find("GDI")!=std::string::npos || rendererString.find("gdi")!=std::string::npos)
	{
		MessageBox(::GetActiveWindow(),"OpenGL未检测显卡驱动信息，请安装显卡驱动！","Creature3D",MB_OK);
	}
	else if(!isGlslSupported())
	{
		MessageBox(::GetActiveWindow(),"您的显卡不支持Directx 9c标准!","Creature3D",MB_OK);
	}
}


bool GL2Extensions::isGlslSupported() const
{
    return s_isShaderEnabled && (( m_glVersion >= 2.0f ) ||
           ( m_isShaderObjectsSupported &&
             m_isVertexShaderSupported &&
             m_isFragmentShaderSupported &&
             m_isLanguage100Supported ));
}


///////////////////////////////////////////////////////////////////////////
// Static array of per-context CRCore::GL2Extensions instances

typedef CRCore::buffered_ptrobject< CRCore::ref_ptr<GL2Extensions> > BufferedExtensions;
static BufferedExtensions s_extensions;
void GL2Extensions::clearExtensions()
{
	s_extensions.clear();
	crGeometry::clearExtensions();
	crBufferObject::Extensions::clearExtensions();
	crFBOExtensions::clearExtensions();
	crStencilTwoSided::Extensions::clearExtensions();
	crTexture::Extensions::clearExtensions();
	crTexture2DArray::Extensions::clearExtensions();
	crTexture3D::Extensions::clearExtensions();
	crTextureCubeMap::Extensions::clearExtensions();
	crBlendColor::Extensions::clearExtensions();
	crBlendEquation::Extensions::clearExtensions();
	crBlendFunc::Extensions::clearExtensions();
	crClampColor::Extensions::clearExtensions();
	crMultisample::Extensions::clearExtensions();

}
GL2Extensions* GL2Extensions::Get(unsigned int contextID, bool createIfNotInitalized)
{
    if (!s_extensions[contextID] && createIfNotInitalized)
            s_extensions[contextID] = new GL2Extensions(contextID);

    return s_extensions[contextID].get();
}

void GL2Extensions::Set(unsigned int contextID, GL2Extensions* extensions)
{
    s_extensions[contextID] = extensions;
}

//void GL2Extensions::Clear()
//{
//	s_extensions.clear();
//}
///////////////////////////////////////////////////////////////////////////

static void NotSupported( const char* funcName )
{
    CRCore::notify(CRCore::WARN)
        <<"Error: "<<funcName<<" not supported by OpenGL driver"<<std::endl;
}



void GL2Extensions::glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha) const
{
    if (m_glBlendEquationSeparate)
    {
        typedef void (APIENTRY * BlendEquationSeparateProc)(GLenum modeRGB, GLenum modeAlpha);
        ((BlendEquationSeparateProc)m_glBlendEquationSeparate)(modeRGB, modeAlpha);
    }
    else
    {
        NotSupported( "glBlendEquationSeparate" );
    }
}


void GL2Extensions::glDrawBuffers(GLsizei n, const GLenum *bufs) const
{
    if (m_glDrawBuffers)
    {
        typedef void (APIENTRY * DrawBuffersProc)(GLsizei n, const GLenum *bufs);
        ((DrawBuffersProc)m_glDrawBuffers)(n, bufs);
    }
    else
    {
        NotSupported( "glDrawBuffers" );
    }
}


void GL2Extensions::glStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass) const
{
    if (m_glStencilOpSeparate)
    {
        typedef void (APIENTRY * StencilOpSeparateProc)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
        ((StencilOpSeparateProc)m_glStencilOpSeparate)(face, sfail, dpfail, dppass);
    }
    else
    {
        NotSupported( "glStencilOpSeparate" );
    }
}


void GL2Extensions::glStencilFuncSeparate(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask) const
{
    if (m_glStencilFuncSeparate)
    {
        typedef void (APIENTRY * StencilFuncSeparateProc)(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
        ((StencilFuncSeparateProc)m_glStencilFuncSeparate)(frontfunc, backfunc, ref, mask);
    }
    else
    {
        NotSupported( "glStencilFuncSeparate" );
    }
}


void GL2Extensions::glStencilMaskSeparate(GLenum face, GLuint mask) const
{
    if (m_glStencilMaskSeparate)
    {
        typedef void (APIENTRY * StencilMaskSeparateProc)(GLenum face, GLuint mask);
        ((StencilMaskSeparateProc)m_glStencilMaskSeparate)(face, mask);
    }
    else
    {
        NotSupported( "glStencilMaskSeparate" );
    }
}


void GL2Extensions::glAttachShader(GLuint program, GLuint shader) const
{
    if (m_glAttachShader)
    {
        typedef void (APIENTRY * AttachShaderProc)(GLuint program, GLuint shader);
        ((AttachShaderProc)m_glAttachShader)(program, shader);
    }
    else
    {
        NotSupported( "glAttachShader" );
    }
}


void GL2Extensions::glBindAttribLocation(GLuint program, GLuint index, const GLchar *name) const
{
    if (m_glBindAttribLocation)
    {
        typedef void (APIENTRY * BindAttribLocationProc)(GLuint program, GLuint index, const GLchar *name);
        ((BindAttribLocationProc)m_glBindAttribLocation)(program, index, name);
    }
    else
    {
        NotSupported( "glBindAttribLocation" );
    }
}


void GL2Extensions::glCompileShader(GLuint shader) const
{
    if (m_glCompileShader)
    {
        typedef void (APIENTRY * CompileShaderProc)(GLuint shader);
        ((CompileShaderProc)m_glCompileShader)(shader);
    }
    else
    {
        NotSupported( "glCompileShader" );
    }
}


GLuint GL2Extensions::glCreateProgram(void) const
{
    if (m_glCreateProgram)
    {
        typedef GLuint (APIENTRY * CreateProgramProc)(void);
        return ((CreateProgramProc)m_glCreateProgram)();
    }
    else
    {
        NotSupported( "glCreateProgram" );
        return 0;
    }
}


GLuint GL2Extensions::glCreateShader(GLenum type) const
{
    if (m_glCreateShader)
    {
        typedef GLuint (APIENTRY * CreateShaderProc)(GLenum type);
        return ((CreateShaderProc)m_glCreateShader)(type);
    }
    else
    {
        NotSupported( "glCreateShader" );
        return 0;
    }
}


void GL2Extensions::glDeleteProgram(GLuint program) const
{
    if (m_glDeleteProgram)
    {
        typedef void (APIENTRY * DeleteProgramProc)(GLuint program);
        ((DeleteProgramProc)m_glDeleteProgram)(program);
    }
    else if (m_glDeleteObjectARB)
    {
        typedef void (APIENTRY * DeleteObjectARBProc)(GLuint program);
        ((DeleteObjectARBProc)m_glDeleteObjectARB)(program);
    }
    else
    {
        NotSupported( "glDeleteProgram" );
    }
}


void GL2Extensions::glDeleteShader(GLuint shader) const
{
    if (m_glDeleteShader)
    {
        typedef void (APIENTRY * DeleteShaderProc)(GLuint shader);
        ((DeleteShaderProc)m_glDeleteShader)(shader);
    }
    else if (m_glDeleteObjectARB)
    {
        typedef void (APIENTRY * DeleteObjectARBProc)(GLuint shader);
        ((DeleteObjectARBProc)m_glDeleteObjectARB)(shader);
    }
    else
    {
        NotSupported( "glDeleteShader" );
    }
}


void GL2Extensions::glDetachShader(GLuint program, GLuint shader) const
{
    if (m_glDetachShader)
    {
        typedef void (APIENTRY * DetachShaderProc)(GLuint program, GLuint shader);
        ((DetachShaderProc)m_glDetachShader)(program, shader);
    }
    else
    {
        NotSupported( "glDetachShader" );
    }
}


void GL2Extensions::glDisableVertexAttribArray(GLuint index) const
{
    if (m_glDisableVertexAttribArray)
    {
        typedef void (APIENTRY * DisableVertexAttribArrayProc)(GLuint index);
        ((DisableVertexAttribArrayProc)m_glDisableVertexAttribArray)(index);
    }
    else
    {
        NotSupported( "glDisableVertexAttribArray" );
    }
}


void GL2Extensions::glEnableVertexAttribArray(GLuint index) const
{
    if (m_glEnableVertexAttribArray)
    {
        typedef void (APIENTRY * EnableVertexAttribArrayProc)(GLuint index);
        ((EnableVertexAttribArrayProc)m_glEnableVertexAttribArray)(index);
    }
    else
    {
        NotSupported( "glEnableVertexAttribArray" );
    }
}


void GL2Extensions::glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) const
{
    if (m_glGetActiveAttrib)
    {
        typedef void (APIENTRY * GetActiveAttribProc)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
        ((GetActiveAttribProc)m_glGetActiveAttrib)(program, index, bufSize, length, size, type, name);
    }
    else
    {
        NotSupported( "glGetActiveAttrib" );
    }
}


void GL2Extensions::glGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) const
{
    if (m_glGetActiveUniform)
    {
        typedef void (APIENTRY * GetActiveUniformProc)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
        ((GetActiveUniformProc)m_glGetActiveUniform)(program, index, bufSize, length, size, type, name);
    }
    else
    {
        NotSupported( "glGetActiveUniform" );
    }
}


void GL2Extensions::glGetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *obj) const
{
    if (m_glGetAttachedShaders)
    {
        typedef void (APIENTRY * GetAttachedShadersProc)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *obj);
        ((GetAttachedShadersProc)m_glGetAttachedShaders)(program, maxCount, count, obj);
    }
    else
    {
        NotSupported( "glGetAttachedShaders" );
    }
}


GLint GL2Extensions::glGetAttribLocation(GLuint program, const GLchar *name) const
{
    if (m_glGetAttribLocation)
    {
        typedef GLint (APIENTRY * GetAttribLocationProc)(GLuint program, const GLchar *name);
        return ((GetAttribLocationProc)m_glGetAttribLocation)(program, name);
    }
    else
    {
        NotSupported( "glGetAttribLocation" );
        return 0;
    }
}


void GL2Extensions::glGetProgramiv(GLuint program, GLenum pname, GLint *params) const
{
    if (m_glGetProgramiv)
    {
        typedef void (APIENTRY * GetProgramivProc)(GLuint program, GLenum pname, GLint *params);
        ((GetProgramivProc)m_glGetProgramiv)(program, pname, params);
    }
    else if (m_glGetObjectParameterivARB)
    {
        typedef void (APIENTRY * GetObjectParameterivARBProc)(GLuint program, GLenum pname, GLint *params);
        ((GetObjectParameterivARBProc)m_glGetObjectParameterivARB)(program, pname, params);
    }
    else
    {
        NotSupported( "glGetProgramiv" );
    }
}


void GL2Extensions::glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) const
{
    if (m_glGetProgramInfoLog)
    {
        typedef void (APIENTRY * GetProgramInfoLogProc)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
        ((GetProgramInfoLogProc)m_glGetProgramInfoLog)(program, bufSize, length, infoLog);
    }
    else if (m_glGetInfoLogARB)
    {
        typedef void (APIENTRY * GetInfoLogARBProc)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
        ((GetInfoLogARBProc)m_glGetInfoLogARB)(program, bufSize, length, infoLog);
    }
    else
    {
        NotSupported( "glGetProgramInfoLog" );
    }
}


void GL2Extensions::glGetShaderiv(GLuint shader, GLenum pname, GLint *params) const
{
    if (m_glGetShaderiv)
    {
        typedef void (APIENTRY * GetShaderivProc)(GLuint shader, GLenum pname, GLint *params);
        ((GetShaderivProc)m_glGetShaderiv)(shader, pname, params);
    }
    else if (m_glGetObjectParameterivARB)
    {
        typedef void (APIENTRY * GetObjectParameterivARBProc)(GLuint shader, GLenum pname, GLint *params);
        ((GetObjectParameterivARBProc)m_glGetObjectParameterivARB)(shader, pname, params);
    }
    else
    {
        NotSupported( "glGetShaderiv" );
    }
}


void GL2Extensions::glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) const
{
    if (m_glGetShaderInfoLog)
    {
        typedef void (APIENTRY * GetShaderInfoLogProc)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
        ((GetShaderInfoLogProc)m_glGetShaderInfoLog)(shader, bufSize, length, infoLog);
    }
    else if (m_glGetInfoLogARB)
    {
        typedef void (APIENTRY * GetInfoLogARBProc)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
        ((GetInfoLogARBProc)m_glGetInfoLogARB)(shader, bufSize, length, infoLog);
    }
    else
    {
        NotSupported( "glGetShaderInfoLog" );
    }
}


void GL2Extensions::glGetShaderSource(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source) const
{
    if (m_glGetShaderSource)
    {
        typedef void (APIENTRY * GetShaderSourceProc)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
        ((GetShaderSourceProc)m_glGetShaderSource)(shader, bufSize, length, source);
    }
    else
    {
        NotSupported( "glGetShaderSource" );
    }
}


GLint GL2Extensions::glGetUniformLocation(GLuint program, const GLchar *name) const
{
    if (m_glGetUniformLocation)
    {
        typedef GLint (APIENTRY * GetUniformLocationProc)(GLuint program, const GLchar *name);
        return ((GetUniformLocationProc)m_glGetUniformLocation)(program, name);
    }
    else
    {
        NotSupported( "glGetUniformLocation" );
        return 0;
    }
}


void GL2Extensions::glGetUniformfv(GLuint program, GLint location, GLfloat *params) const
{
    if (m_glGetUniformfv)
    {
        typedef void (APIENTRY * GetUniformfvProc)(GLuint program, GLint location, GLfloat *params);
        ((GetUniformfvProc)m_glGetUniformfv)(program, location, params);
    }
    else
    {
        NotSupported( "glGetUniformfv" );
    }
}


void GL2Extensions::glGetUniformiv(GLuint program, GLint location, GLint *params) const
{
    if (m_glGetUniformiv)
    {
        typedef void (APIENTRY * GetUniformivProc)(GLuint program, GLint location, GLint *params);
        ((GetUniformivProc)m_glGetUniformiv)(program, location, params);
    }
    else
    {
        NotSupported( "glGetUniformiv" );
    }
}


void GL2Extensions::glGetVertexAttribdv(GLuint index, GLenum pname, GLdouble *params) const
{
    if (m_glGetVertexAttribdv)
    {
        typedef void (APIENTRY * GetVertexAttribdvProc)(GLuint index, GLenum pname, GLdouble *params);
        ((GetVertexAttribdvProc)m_glGetVertexAttribdv)(index, pname, params);
    }
    else
    {
        NotSupported( "glGetVertexAttribdv" );
    }
}


void GL2Extensions::glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params) const
{
    if (m_glGetVertexAttribfv)
    {
        typedef void (APIENTRY * GetVertexAttribfvProc)(GLuint index, GLenum pname, GLfloat *params);
        ((GetVertexAttribfvProc)m_glGetVertexAttribfv)(index, pname, params);
    }
    else
    {
        NotSupported( "glGetVertexAttribfv" );
    }
}


void GL2Extensions::glGetVertexAttribiv(GLuint index, GLenum pname, GLint *params) const
{
    if (m_glGetVertexAttribiv)
    {
        typedef void (APIENTRY * GetVertexAttribivProc)(GLuint index, GLenum pname, GLint *params);
        ((GetVertexAttribivProc)m_glGetVertexAttribiv)(index, pname, params);
    }
    else
    {
        NotSupported( "glGetVertexAttribiv" );
    }
}


void GL2Extensions::glGetVertexAttribPointerv(GLuint index, GLenum pname, GLvoid* *pointer) const
{
    if (m_glGetVertexAttribPointerv)
    {
        typedef void (APIENTRY * GetVertexAttribPointervProc)(GLuint index, GLenum pname, GLvoid* *pointer);
        ((GetVertexAttribPointervProc)m_glGetVertexAttribPointerv)(index, pname, pointer);
    }
    else
    {
        NotSupported( "glGetVertexAttribPointerv" );
    }
}


GLboolean GL2Extensions::glIsProgram(GLuint program) const
{
    if (m_glIsProgram)
    {
        typedef GLboolean (APIENTRY * IsProgramProc)(GLuint program);
        return ((IsProgramProc)m_glIsProgram)(program);
    }
    else
    {
        NotSupported( "glIsProgram" );
        return 0;
    }
}


GLboolean GL2Extensions::glIsShader(GLuint shader) const
{
    if (m_glIsShader)
    {
        typedef GLboolean (APIENTRY * IsShaderProc)(GLuint shader);
        return ((IsShaderProc)m_glIsShader)(shader);
    }
    else
    {
        NotSupported( "glIsShader" );
        return 0;
    }
}


void GL2Extensions::glLinkProgram(GLuint program) const
{
    if (m_glLinkProgram)
    {
        typedef void (APIENTRY * LinkProgramProc)(GLuint program);
        ((LinkProgramProc)m_glLinkProgram)(program);
    }
    else
    {
        NotSupported( "glLinkProgram" );
    }
}


void GL2Extensions::glShaderSource(GLuint shader, GLsizei count, const GLchar* *string, const GLint *length) const
{
    if (m_glShaderSource)
    {
        typedef void (APIENTRY * ShaderSourceProc)(GLuint shader, GLsizei count, const GLchar* *string, const GLint *length);
        ((ShaderSourceProc)m_glShaderSource)(shader, count, string, length);
    }
    else
    {
        NotSupported( "glShaderSource" );
    }
}


void GL2Extensions::glUseProgram(GLuint program) const
{
    if (m_glUseProgram)
    {
        typedef void (APIENTRY * UseProgramProc)(GLuint program);
        ((UseProgramProc)m_glUseProgram)(program);
    }
    else
    {
        NotSupported( "glUseProgram" );
    }
}


void GL2Extensions::glUniform1f(GLint location, GLfloat v0) const
{
    if (m_glUniform1f)
    {
        typedef void (APIENTRY * Uniform1fProc)(GLint location, GLfloat v0);
        ((Uniform1fProc)m_glUniform1f)(location, v0);
    }
    else
    {
        NotSupported( "glUniform1f" );
    }
}


void GL2Extensions::glUniform2f(GLint location, GLfloat v0, GLfloat v1) const
{
    if (m_glUniform2f)
    {
        typedef void (APIENTRY * Uniform2fProc)(GLint location, GLfloat v0, GLfloat v1);
        ((Uniform2fProc)m_glUniform2f)(location, v0, v1);
    }
    else
    {
        NotSupported( "glUniform2f" );
    }
}


void GL2Extensions::glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) const
{
    if (m_glUniform3f)
    {
        typedef void (APIENTRY * Uniform3fProc)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
        ((Uniform3fProc)m_glUniform3f)(location, v0, v1, v2);
    }
    else
    {
        NotSupported( "glUniform3f" );
    }
}


void GL2Extensions::glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const
{
    if (m_glUniform4f)
    {
        typedef void (APIENTRY * Uniform4fProc)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
        ((Uniform4fProc)m_glUniform4f)(location, v0, v1, v2, v3);
    }
    else
    {
        NotSupported( "glUniform4f" );
    }
}


void GL2Extensions::glUniform1i(GLint location, GLint v0) const
{
    if (m_glUniform1i)
    {
        typedef void (APIENTRY * Uniform1iProc)(GLint location, GLint v0);
        ((Uniform1iProc)m_glUniform1i)(location, v0);
    }
    else
    {
        NotSupported( "glUniform1i" );
    }
}


void GL2Extensions::glUniform2i(GLint location, GLint v0, GLint v1) const
{
    if (m_glUniform2i)
    {
        typedef void (APIENTRY * Uniform2iProc)(GLint location, GLint v0, GLint v1);
        ((Uniform2iProc)m_glUniform2i)(location, v0, v1);
    }
    else
    {
        NotSupported( "glUniform2i" );
    }
}


void GL2Extensions::glUniform3i(GLint location, GLint v0, GLint v1, GLint v2) const
{
    if (m_glUniform3i)
    {
        typedef void (APIENTRY * Uniform3iProc)(GLint location, GLint v0, GLint v1, GLint v2);
        ((Uniform3iProc)m_glUniform3i)(location, v0, v1, v2);
    }
    else
    {
        NotSupported( "glUniform3i" );
    }
}


void GL2Extensions::glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3) const
{
    if (m_glUniform4i)
    {
        typedef void (APIENTRY * glUniform4iProc)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
        ((glUniform4iProc)m_glUniform4i)(location, v0, v1, v2, v3);
    }
    else
    {
        NotSupported( "glUniform4i" );
    }
}


void GL2Extensions::glUniform1fv(GLint location, GLsizei count, const GLfloat *value) const
{
    if (m_glUniform1fv)
    {
        typedef void (APIENTRY * Uniform1fvProc)(GLint location, GLsizei count, const GLfloat *value);
        ((Uniform1fvProc)m_glUniform1fv)(location, count, value);
    }
    else
    {
        NotSupported( "glUniform1fv" );
    }
}


void GL2Extensions::glUniform2fv(GLint location, GLsizei count, const GLfloat *value) const
{
    if (m_glUniform2fv)
    {
        typedef void (APIENTRY * Uniform2fvProc)(GLint location, GLsizei count, const GLfloat *value);
        ((Uniform2fvProc)m_glUniform2fv)(location, count, value);
    }
    else
    {
        NotSupported( "glUniform2fv" );
    }
}


void GL2Extensions::glUniform3fv(GLint location, GLsizei count, const GLfloat *value) const
{
    if (m_glUniform3fv)
    {
        typedef void (APIENTRY * Uniform3fvProc)(GLint location, GLsizei count, const GLfloat *value);
        ((Uniform3fvProc)m_glUniform3fv)(location, count, value);
    }
    else
    {
        NotSupported( "glUniform3fv" );
    }
}


void GL2Extensions::glUniform4fv(GLint location, GLsizei count, const GLfloat *value) const
{
    if (m_glUniform4fv)
    {
        typedef void (APIENTRY * Uniform4fvProc)(GLint location, GLsizei count, const GLfloat *value);
        ((Uniform4fvProc)m_glUniform4fv)(location, count, value);
    }
    else
    {
        NotSupported( "glUniform4fv" );
    }
}


void GL2Extensions::glUniform1iv(GLint location, GLsizei count, const GLint *value) const
{
    if (m_glUniform1iv)
    {
        typedef void (APIENTRY * Uniform1ivProc)(GLint location, GLsizei count, const GLint *value);
        ((Uniform1ivProc)m_glUniform1iv)(location, count, value);
    }
    else
    {
        NotSupported( "glUniform1iv" );
    }
}


void GL2Extensions::glUniform2iv(GLint location, GLsizei count, const GLint *value) const
{
    if (m_glUniform2iv)
    {
        typedef void (APIENTRY * Uniform2ivProc)(GLint location, GLsizei count, const GLint *value);
        ((Uniform2ivProc)m_glUniform2iv)(location, count, value);
    }
    else
    {
        NotSupported( "glUniform2iv" );
    }
}


void GL2Extensions::glUniform3iv(GLint location, GLsizei count, const GLint *value) const
{
    if (m_glUniform3iv)
    {
        typedef void (APIENTRY * glUniform3ivProc)(GLint location, GLsizei count, const GLint *value);
        ((glUniform3ivProc)m_glUniform3iv)(location, count, value);
    }
    else
    {
        NotSupported( "glUniform3iv" );
    }
}


void GL2Extensions::glUniform4iv(GLint location, GLsizei count, const GLint *value) const
{
    if (m_glUniform4iv)
    {
        typedef void (APIENTRY * Uniform4ivProc)(GLint location, GLsizei count, const GLint *value);
        ((Uniform4ivProc)m_glUniform4iv)(location, count, value);
    }
    else
    {
        NotSupported( "glUniform4iv" );
    }
}


void GL2Extensions::glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) const
{
    if (m_glUniformMatrix2fv)
    {
        typedef void (APIENTRY * UniformMatrix2fvProc)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
        ((UniformMatrix2fvProc)m_glUniformMatrix2fv)(location, count, transpose, value);
    }
    else
    {
        NotSupported( "glUniformMatrix2fv" );
    }
}


void GL2Extensions::glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) const
{
    if (m_glUniformMatrix3fv)
    {
        typedef void (APIENTRY * glUniformMatrix3fvProc)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
        ((glUniformMatrix3fvProc)m_glUniformMatrix3fv)(location, count, transpose, value);
    }
    else
    {
        NotSupported( "glUniformMatrix3fv" );
    }
}


void GL2Extensions::glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) const
{
    if (m_glUniformMatrix4fv)
    {
        typedef void (APIENTRY * UniformMatrix4fvProc)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
        ((UniformMatrix4fvProc)m_glUniformMatrix4fv)(location, count, transpose, value);
    }
    else
    {
        NotSupported( "glUniformMatrix4fv" );
    }
}


void GL2Extensions::glValidateProgram(GLuint program) const
{
    if (m_glValidateProgram)
    {
        typedef void (APIENTRY * ValidateProgramProc)(GLuint program);
        ((ValidateProgramProc)m_glValidateProgram)(program);
    }
    else
    {
        NotSupported( "glValidateProgram" );
    }
}


void GL2Extensions::glVertexAttrib1d(GLuint index, GLdouble x) const
{
    if (m_glVertexAttrib1d)
    {
        typedef void (APIENTRY * VertexAttrib1dProc)(GLuint index, GLdouble x);
        ((VertexAttrib1dProc)m_glVertexAttrib1d)(index, x);
    }
    else
    {
        NotSupported( "glVertexAttrib1d" );
    }
}


void GL2Extensions::glVertexAttrib1dv(GLuint index, const GLdouble *v) const
{
    if (m_glVertexAttrib1dv)
    {
        typedef void (APIENTRY * glVertexAttrib1dvProc)(GLuint index, const GLdouble *v);
        ((glVertexAttrib1dvProc)m_glVertexAttrib1dv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib1dv" );
    }
}


void GL2Extensions::glVertexAttrib1f(GLuint index, GLfloat x) const
{
    if (m_glVertexAttrib1f)
    {
        typedef void (APIENTRY * VertexAttrib1fProc)(GLuint index, GLfloat x);
        ((VertexAttrib1fProc)m_glVertexAttrib1f)(index, x);
    }
    else
    {
        NotSupported( "glVertexAttrib1f" );
    }
}


void GL2Extensions::glVertexAttrib1fv(GLuint index, const GLfloat *v) const
{
    if (m_glVertexAttrib1fv)
    {
        typedef void (APIENTRY * VertexAttrib1fvProc)(GLuint index, const GLfloat *v);
        ((VertexAttrib1fvProc)m_glVertexAttrib1fv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib1fv" );
    }
}


void GL2Extensions::glVertexAttrib1s(GLuint index, GLshort x) const
{
    if (m_glVertexAttrib1s)
    {
        typedef void (APIENTRY * VertexAttrib1sProc)(GLuint index, GLshort x);
        ((VertexAttrib1sProc)m_glVertexAttrib1s)(index, x);
    }
    else
    {
        NotSupported( "glVertexAttrib1s" );
    }
}


void GL2Extensions::glVertexAttrib1sv(GLuint index, const GLshort *v) const
{
    if (m_glVertexAttrib1sv)
    {
        typedef void (APIENTRY * VertexAttrib1svProc)(GLuint index, const GLshort *v);
        ((VertexAttrib1svProc)m_glVertexAttrib1sv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib1sv" );
    }
}


void GL2Extensions::glVertexAttrib2d(GLuint index, GLdouble x, GLdouble y) const
{
    if (m_glVertexAttrib2d)
    {
        typedef void (APIENTRY * VertexAttrib2dProc)(GLuint index, GLdouble x, GLdouble y);
        ((VertexAttrib2dProc)m_glVertexAttrib2d)(index, x, y);
    }
    else
    {
        NotSupported( "glVertexAttrib2d" );
    }
}


void GL2Extensions::glVertexAttrib2dv(GLuint index, const GLdouble *v) const
{
    if (m_glVertexAttrib2dv)
    {
        typedef void (APIENTRY * VertexAttrib2dvProc)(GLuint index, const GLdouble *v);
        ((VertexAttrib2dvProc)m_glVertexAttrib2dv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib2dv" );
    }
}


void GL2Extensions::glVertexAttrib2f(GLuint index, GLfloat x, GLfloat y) const
{
    if (m_glVertexAttrib2f)
    {
        typedef void (APIENTRY * VertexAttrib2fProc)(GLuint index, GLfloat x, GLfloat y);
        ((VertexAttrib2fProc)m_glVertexAttrib2f)(index, x, y);
    }
    else
    {
        NotSupported( "glVertexAttrib2f" );
    }
}


void GL2Extensions::glVertexAttrib2fv(GLuint index, const GLfloat *v) const
{
    if (m_glVertexAttrib2fv)
    {
        typedef void (APIENTRY * VertexAttrib2fvProc)(GLuint index, const GLfloat *v);
        ((VertexAttrib2fvProc)m_glVertexAttrib2fv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib2fv" );
    }
}


void GL2Extensions::glVertexAttrib2s(GLuint index, GLshort x, GLshort y) const
{
    if (m_glVertexAttrib2s)
    {
        typedef void (APIENTRY * VertexAttrib2sProc)(GLuint index, GLshort x, GLshort y);
        ((VertexAttrib2sProc)m_glVertexAttrib2s)(index, x, y);
    }
    else
    {
        NotSupported( "glVertexAttrib2s" );
    }
}


void GL2Extensions::glVertexAttrib2sv(GLuint index, const GLshort *v) const
{
    if (m_glVertexAttrib2sv)
    {
        typedef void (APIENTRY * VertexAttrib2svProc)(GLuint index, const GLshort *v);
        ((VertexAttrib2svProc)m_glVertexAttrib2sv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib2sv" );
    }
}


void GL2Extensions::glVertexAttrib3d(GLuint index, GLdouble x, GLdouble y, GLdouble z) const
{
    if (m_glVertexAttrib3d)
    {
        typedef void (APIENTRY * VertexAttrib3dProc)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
        ((VertexAttrib3dProc)m_glVertexAttrib3d)(index, x, y, z);
    }
    else
    {
        NotSupported( "glVertexAttrib3d" );
    }
}


void GL2Extensions::glVertexAttrib3dv(GLuint index, const GLdouble *v) const
{
    if (m_glVertexAttrib3dv)
    {
        typedef void (APIENTRY * VertexAttrib3dvProc)(GLuint index, const GLdouble *v);
        ((VertexAttrib3dvProc)m_glVertexAttrib3dv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib3dv" );
    }
}


void GL2Extensions::glVertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z) const
{
    if (m_glVertexAttrib3f)
    {
        typedef void (APIENTRY * VertexAttrib3fProc)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
        ((VertexAttrib3fProc)m_glVertexAttrib3f)(index, x, y, z);
    }
    else
    {
        NotSupported( "glVertexAttrib3f" );
    }
}


void GL2Extensions::glVertexAttrib3fv(GLuint index, const GLfloat *v) const
{
    if (m_glVertexAttrib3fv)
    {
        typedef void (APIENTRY * VertexAttrib3fvProc)(GLuint index, const GLfloat *v);
        ((VertexAttrib3fvProc)m_glVertexAttrib3fv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib3fv" );
    }
}


void GL2Extensions::glVertexAttrib3s(GLuint index, GLshort x, GLshort y, GLshort z) const
{
    if (m_glVertexAttrib3s)
    {
        typedef void (APIENTRY * VertexAttrib3sProc)(GLuint index, GLshort x, GLshort y, GLshort z);
        ((VertexAttrib3sProc)m_glVertexAttrib3s)(index, x, y, z);
    }
    else
    {
        NotSupported( "glVertexAttrib3s" );
    }
}


void GL2Extensions::glVertexAttrib3sv(GLuint index, const GLshort *v) const
{
    if (m_glVertexAttrib3sv)
    {
        typedef void (APIENTRY * VertexAttrib3svProc)(GLuint index, const GLshort *v);
        ((VertexAttrib3svProc)m_glVertexAttrib3sv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib3sv" );
    }
}


void GL2Extensions::glVertexAttrib4Nbv(GLuint index, const GLbyte *v) const
{
    if (m_glVertexAttrib4Nbv)
    {
        typedef void (APIENTRY * VertexAttrib4NbvProc)(GLuint index, const GLbyte *v);
        ((VertexAttrib4NbvProc)m_glVertexAttrib4Nbv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib4Nbv" );
    }
}


void GL2Extensions::glVertexAttrib4Niv(GLuint index, const GLint *v) const
{
    if (m_glVertexAttrib4Niv)
    {
        typedef void (APIENTRY * VertexAttrib4NivProc)(GLuint index, const GLint *v);
        ((VertexAttrib4NivProc)m_glVertexAttrib4Niv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib4Niv" );
    }
}


void GL2Extensions::glVertexAttrib4Nsv(GLuint index, const GLshort *v) const
{
    if (m_glVertexAttrib4Nsv)
    {
        typedef void (APIENTRY * VertexAttrib4NsvProc)(GLuint index, const GLshort *v);
        ((VertexAttrib4NsvProc)m_glVertexAttrib4Nsv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib4Nsv" );
    }
}


void GL2Extensions::glVertexAttrib4Nub(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w) const
{
    if (m_glVertexAttrib4Nub)
    {
        typedef void (APIENTRY * VertexAttrib4NubProc)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
        ((VertexAttrib4NubProc)m_glVertexAttrib4Nub)(index, x, y, z, w);
    }
    else
    {
        NotSupported( "glVertexAttrib4Nub" );
    }
}


void GL2Extensions::glVertexAttrib4Nubv(GLuint index, const GLubyte *v) const
{
    if (m_glVertexAttrib4Nubv)
    {
        typedef void (APIENTRY * VertexAttrib4NubvProc)(GLuint index, const GLubyte *v);
        ((VertexAttrib4NubvProc)m_glVertexAttrib4Nubv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib4Nubv" );
    }
}


void GL2Extensions::glVertexAttrib4Nuiv(GLuint index, const GLuint *v) const
{
    if (m_glVertexAttrib4Nuiv)
    {
        typedef void (APIENTRY * VertexAttrib4NuivProc)(GLuint index, const GLuint *v);
        ((VertexAttrib4NuivProc)m_glVertexAttrib4Nuiv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib4Nuiv" );
    }
}


void GL2Extensions::glVertexAttrib4Nusv(GLuint index, const GLushort *v) const
{
    if (m_glVertexAttrib4Nusv)
    {
        typedef void (APIENTRY * VertexAttrib4NusvProc)(GLuint index, const GLushort *v);
        ((VertexAttrib4NusvProc)m_glVertexAttrib4Nusv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib4Nusv" );
    }
}


void GL2Extensions::glVertexAttrib4bv(GLuint index, const GLbyte *v) const
{
    if (m_glVertexAttrib4bv)
    {
        typedef void (APIENTRY * VertexAttrib4bvProc)(GLuint index, const GLbyte *v);
        ((VertexAttrib4bvProc)m_glVertexAttrib4bv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib4bv" );
    }
}


void GL2Extensions::glVertexAttrib4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) const
{
    if (m_glVertexAttrib4d)
    {
        typedef void (APIENTRY * VertexAttrib4dProc)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
        ((VertexAttrib4dProc)m_glVertexAttrib4d)(index, x, y, z, w);
    }
    else
    {
        NotSupported( "glVertexAttrib4d" );
    }
}


void GL2Extensions::glVertexAttrib4dv(GLuint index, const GLdouble *v) const
{
    if (m_glVertexAttrib4dv)
    {
        typedef void (APIENTRY * VertexAttrib4dvProc)(GLuint index, const GLdouble *v);
        ((VertexAttrib4dvProc)m_glVertexAttrib4dv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib4dv" );
    }
}


void GL2Extensions::glVertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const
{
    if (m_glVertexAttrib4f)
    {
        typedef void (APIENTRY * VertexAttrib4fProc)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
        ((VertexAttrib4fProc)m_glVertexAttrib4f)(index, x, y, z, w);
    }
    else
    {
        NotSupported( "glVertexAttrib4f" );
    }
}


void GL2Extensions::glVertexAttrib4fv(GLuint index, const GLfloat *v) const
{
    if (m_glVertexAttrib4fv)
    {
        typedef void (APIENTRY * VertexAttrib4fvProc)(GLuint index, const GLfloat *v);
        ((VertexAttrib4fvProc)m_glVertexAttrib4fv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib4fv" );
    }
}


void GL2Extensions::glVertexAttrib4iv(GLuint index, const GLint *v) const
{
    if (m_glVertexAttrib4iv)
    {
        typedef void (APIENTRY * VertexAttrib4ivProc)(GLuint index, const GLint *v);
        ((VertexAttrib4ivProc)m_glVertexAttrib4iv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib4iv" );
    }
}


void GL2Extensions::glVertexAttrib4s(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w) const
{
    if (m_glVertexAttrib4s)
    {
        typedef void (APIENTRY * VertexAttrib4sProc)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
        ((VertexAttrib4sProc)m_glVertexAttrib4s)(index, x, y, z, w);
    }
    else
    {
        NotSupported( "glVertexAttrib4s" );
    }
}


void GL2Extensions::glVertexAttrib4sv(GLuint index, const GLshort *v) const
{
    if (m_glVertexAttrib4sv)
    {
        typedef void (APIENTRY * VertexAttrib4svProc)(GLuint index, const GLshort *v);
        ((VertexAttrib4svProc)m_glVertexAttrib4sv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib4sv" );
    }
}


void GL2Extensions::glVertexAttrib4ubv(GLuint index, const GLubyte *v) const
{
    if (m_glVertexAttrib4ubv)
    {
        typedef void (APIENTRY * VertexAttrib4ubvProc)(GLuint index, const GLubyte *v);
        ((VertexAttrib4ubvProc)m_glVertexAttrib4ubv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib4ubv" );
    }
}


void GL2Extensions::glVertexAttrib4uiv(GLuint index, const GLuint *v) const
{
    if (m_glVertexAttrib4uiv)
    {
        typedef void (APIENTRY * VertexAttrib4uivProc)(GLuint index, const GLuint *v);
        ((VertexAttrib4uivProc)m_glVertexAttrib4uiv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib4uiv" );
    }
}


void GL2Extensions::glVertexAttrib4usv(GLuint index, const GLushort *v) const
{
    if (m_glVertexAttrib4usv)
    {
        typedef void (APIENTRY * VertexAttrib4usvProc)(GLuint index, const GLushort *v);
        ((VertexAttrib4usvProc)m_glVertexAttrib4usv)(index, v);
    }
    else
    {
        NotSupported( "glVertexAttrib4usv" );
    }
}


void GL2Extensions::glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) const
{
    if (m_glVertexAttribPointer)
    {
        typedef void (APIENTRY * VertexAttribPointerProc)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
        ((VertexAttribPointerProc)m_glVertexAttribPointer)(index, size, type, normalized, stride, pointer);
    }
    else
    {
        NotSupported( "glVertexAttribPointer" );
    }
}


void GL2Extensions::glUniformMatrix2x3fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value ) const
{
    if (m_glUniformMatrix2x3fv)
    {
        typedef void (APIENTRY * UniformMatrix2x3fvProc)( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
        ((UniformMatrix2x3fvProc)m_glUniformMatrix2x3fv)( location, count, transpose, value );
    }
    else
    {
        NotSupported( "glUniformMatrix2x3fv" );
    }
}

void GL2Extensions::glUniformMatrix3x2fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value ) const
{
    if (m_glUniformMatrix3x2fv)
    {
        typedef void (APIENTRY * UniformMatrix3x2fvProc)( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
        ((UniformMatrix3x2fvProc)m_glUniformMatrix3x2fv)( location, count, transpose, value );
    }
    else
    {
        NotSupported( "glUniformMatrix3x2fv" );
    }
}

void GL2Extensions::glUniformMatrix2x4fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value ) const
{
    if (m_glUniformMatrix2x4fv)
    {
        typedef void (APIENTRY * UniformMatrix2x4fvProc)( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
        ((UniformMatrix2x4fvProc)m_glUniformMatrix2x4fv)( location, count, transpose, value );
    }
    else
    {
        NotSupported( "glUniformMatrix2x4fv" );
    }
}

void GL2Extensions::glUniformMatrix4x2fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value ) const
{
    if (m_glUniformMatrix4x2fv)
    {
        typedef void (APIENTRY * UniformMatrix4x2fvProc)( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
        ((UniformMatrix4x2fvProc)m_glUniformMatrix4x2fv)( location, count, transpose, value );
    }
    else
    {
        NotSupported( "glUniformMatrix4x2fv" );
    }
}

void GL2Extensions::glUniformMatrix3x4fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value ) const
{
    if (m_glUniformMatrix3x4fv)
    {
        typedef void (APIENTRY * UniformMatrix3x4fvProc)( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
        ((UniformMatrix3x4fvProc)m_glUniformMatrix3x4fv)( location, count, transpose, value );
    }
    else
    {
        NotSupported( "glUniformMatrix3x4fv" );
    }
}

void GL2Extensions::glUniformMatrix4x3fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value ) const
{
    if (m_glUniformMatrix4x3fv)
    {
        typedef void (APIENTRY * UniformMatrix4x3fvProc)( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
        ((UniformMatrix4x3fvProc)m_glUniformMatrix4x3fv)( location, count, transpose, value );
    }
    else
    {
        NotSupported( "glUniformMatrix4x3fv" );
    }
}


void GL2Extensions::glProgramParameteri( GLuint program, GLenum pname, GLint value ) const
{
    if (m_glProgramParameteri)
    {
        typedef void (APIENTRY * ProgramParameteriProc)( GLuint program, GLenum pname, GLint value );
        ((ProgramParameteriProc)m_glProgramParameteri)( program, pname, value );
    }
    else
    {
        NotSupported( "glProgramParameteri" );
    }
}

void GL2Extensions::glFramebufferTexture( GLenum target, GLenum attachment, GLuint texture, GLint level ) const
{
    if (m_glFramebufferTexture)
    {
        typedef void (APIENTRY * FramebufferTextureProc)( GLenum target, GLenum attachment, GLuint texture, GLint level );
        ((FramebufferTextureProc)m_glFramebufferTexture)( target, attachment, texture, level );
    }
    else
    {
        NotSupported( "glFramebufferTexture" );
    }
}

void GL2Extensions::glFramebufferTextureLayer( GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer ) const
{
    if (m_glFramebufferTextureLayer)
    {
        typedef void (APIENTRY * FramebufferTextureLayerProc)( GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer );
        ((FramebufferTextureLayerProc)m_glFramebufferTextureLayer)( target, attachment, texture, level, layer );
    }
    else
    {
        NotSupported( "glFramebufferTextureLayer" );
    }
}

void GL2Extensions::glFramebufferTextureFace( GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face ) const
{
    if (m_glFramebufferTextureFace)
    {
        typedef void (APIENTRY * FramebufferTextureFaceProc)( GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face );
        ((FramebufferTextureFaceProc)m_glFramebufferTextureFace)( target, attachment, texture, level, face );
    }
    else
    {
        NotSupported( "glFramebufferTextureFace" );
    }
}


void GL2Extensions::glGetUniformuiv( GLuint program, GLint location, GLuint* params ) const
{
    if (m_glGetUniformuiv)
    {
        typedef void (APIENTRY * GetUniformuivProc)( GLuint program, GLint location, GLuint* params );
        ((GetUniformuivProc)m_glGetUniformuiv)( program, location, params );
    }
    else
    {
        NotSupported( "glGetUniformuiv" );
    }
}

void GL2Extensions::glBindFragDataLocation( GLuint program, GLuint color, const GLchar* name ) const
{
    if (m_glBindFragDataLocation)
    {
        typedef void (APIENTRY * BindFragDataLocationProc)( GLuint program, GLuint color, const GLchar* name );
        ((BindFragDataLocationProc)m_glBindFragDataLocation)( program, color, name );
    }
    else
    {
        NotSupported( "glBindFragDataLocation" );
    }
}

GLint GL2Extensions::glGetFragDataLocation( GLuint program, const GLchar* name ) const
{
    if (m_glGetFragDataLocation)
    {
        typedef GLint (APIENTRY * GetFragDataLocationProc)( GLuint program, const GLchar* name );
        return ((GetFragDataLocationProc)m_glGetFragDataLocation)( program, name );
    }
    else
    {
        NotSupported( "glGetFragDataLocation" );
        return -1;
    }
}


void GL2Extensions::glUniform1ui( GLint location, GLuint v0 ) const
{
    if (m_glUniform1ui)
    {
        typedef void (APIENTRY * Uniform1uiProc)( GLint location, GLuint v0 );
        ((Uniform1uiProc)m_glUniform1ui)( location, v0 );
    }
    else
    {
        NotSupported( "glUniform1ui" );
    }
}

void GL2Extensions::glUniform2ui( GLint location, GLuint v0, GLuint v1 ) const
{
    if (m_glUniform2ui)
    {
        typedef void (APIENTRY * Uniform2uiProc)( GLint location, GLuint v0, GLuint v1 );
        ((Uniform2uiProc)m_glUniform2ui)( location, v0, v1 );
    }
    else
    {
        NotSupported( "glUniform2ui" );
    }
}

void GL2Extensions::glUniform3ui( GLint location, GLuint v0, GLuint v1, GLuint v2 ) const
{
    if (m_glUniform3ui)
    {
        typedef void (APIENTRY * Uniform3uiProc)( GLint location, GLuint v0, GLuint v1, GLuint v2 );
        ((Uniform3uiProc)m_glUniform3ui)( location, v0, v1, v2 );
    }
    else
    {
        NotSupported( "glUniform3ui" );
    }
}

void GL2Extensions::glUniform4ui( GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3 ) const
{
    if (m_glUniform4ui)
    {
        typedef void (APIENTRY * Uniform4uiProc)( GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3 );
        ((Uniform4uiProc)m_glUniform4ui)( location, v0, v1, v2, v3 );
    }
    else
    {
        NotSupported( "glUniform4ui" );
    }
}

void GL2Extensions::glUniform1uiv( GLint location, GLsizei count, const GLuint *value ) const
{
    if (m_glUniform1uiv)
    {
        typedef void (APIENTRY * Uniform1uivProc)( GLint location, GLsizei count, const GLuint *value );
        ((Uniform1uivProc)m_glUniform1uiv)( location, count, value );
    }
    else
    {
        NotSupported( "glUniform1uiv" );
    }
}

void GL2Extensions::glUniform2uiv( GLint location, GLsizei count, const GLuint *value ) const
{
    if (m_glUniform2uiv)
    {
        typedef void (APIENTRY * Uniform2uivProc)( GLint location, GLsizei count, const GLuint *value );
        ((Uniform2uivProc)m_glUniform2uiv)( location, count, value );
    }
    else
    {
        NotSupported( "glUniform2uiv" );
    }
}

void GL2Extensions::glUniform3uiv( GLint location, GLsizei count, const GLuint *value ) const
{
    if (m_glUniform3uiv)
    {
        typedef void (APIENTRY * Uniform3uivProc)( GLint location, GLsizei count, const GLuint *value );
        ((Uniform3uivProc)m_glUniform3uiv)( location, count, value );
    }
    else
    {
        NotSupported( "glUniform3uiv" );
    }
}

void GL2Extensions::glUniform4uiv( GLint location, GLsizei count, const GLuint *value ) const
{
    if (m_glUniform4uiv)
    {
        typedef void (APIENTRY * Uniform4uivProc)( GLint location, GLsizei count, const GLuint *value );
        ((Uniform4uivProc)m_glUniform4uiv)( location, count, value );
    }
    else
    {
        NotSupported( "glUniform4uiv" );
    }
}


///////////////////////////////////////////////////////////////////////////
// C++-friendly convenience methods

GLuint GL2Extensions::getCurrentProgram() const
{
    if( m_glVersion >= 2.0f )
    {
        // GLSL as GL v2.0 core functionality
        GLint result = 0;
        glGetIntegerv( GL_CURRENT_PROGRAM, &result );
        return static_cast<GLuint>(result);
    }
    else if (m_glGetHandleARB)
    {
        // fallback for GLSL as GL v1.5 ARB extension
#ifndef GL_PROGRAM_OBJECT_ARB
#define GL_PROGRAM_OBJECT_ARB 0x8B40
#endif
        typedef GLuint (APIENTRY * GetHandleProc) (GLenum pname);
        return ((GetHandleProc)m_glGetHandleARB)( GL_PROGRAM_OBJECT_ARB );
    }
    else
    {
        NotSupported( "getCurrentProgram" );
        return 0;
    }
}


bool GL2Extensions::getProgramInfoLog( GLuint program, std::string& result ) const
{
    GLsizei bufLen = 0;        // length of buffer to allocate
    GLsizei strLen = 0;        // strlen GL actually wrote to buffer

    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &bufLen );
    if( bufLen > 1 )
    {
        GLchar* infoLog = new GLchar[bufLen];
        glGetProgramInfoLog( program, bufLen, &strLen, infoLog );
        if( strLen > 0 ) result = infoLog;
        delete [] infoLog;
    }
    return (strLen > 0);
}


bool GL2Extensions::getShaderInfoLog( GLuint shader, std::string& result ) const
{
    GLsizei bufLen = 0;        // length of buffer to allocate
    GLsizei strLen = 0;        // strlen GL actually wrote to buffer

    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &bufLen );
    if( bufLen > 1 )
    {
        GLchar* infoLog = new GLchar[bufLen];
        glGetShaderInfoLog( shader, bufLen, &strLen, infoLog );
        if( strLen > 0 ) result = infoLog;
        delete [] infoLog;
    }
    return (strLen > 0);
}


bool GL2Extensions::getAttribLocation( const char* attribName, GLuint& location ) const
{
    // is there an active GLSL program?
    GLuint program = getCurrentProgram();
    if( glIsProgram(program) == GL_FALSE ) return false;

    // has that program been successfully linked?
    GLint linked = GL_FALSE;
    glGetProgramiv( program, GL_LINK_STATUS, &linked );
    if( linked == GL_FALSE ) return false;

    // is there such a named attribute?
    GLint loc = glGetAttribLocation( program, attribName );
    if( loc < 0 ) return false;

    location = loc;
    return true;
}


bool GL2Extensions::getFragDataLocation( const char* fragDataName, GLuint& location ) const
{
    // is there an active GLSL program?
    GLuint program = getCurrentProgram();
    if( glIsProgram(program) == GL_FALSE ) return false;

    // has that program been successfully linked?
    GLint linked = GL_FALSE;
    glGetProgramiv( program, GL_LINK_STATUS, &linked );
    if( linked == GL_FALSE ) return false;

    // check if supported
    if (m_glGetFragDataLocation == NULL) return false;

    // is there such a named attribute?
    GLint loc = glGetFragDataLocation( program, fragDataName );
    if( loc < 0 ) return false;

    location = loc;
    return true;
}
///////////////////////////////////////////
void GL2Extensions::glDeleteBuffers( GLsizei n, const GLuint *buffers ) const
{
	if (m_glDeleteBuffers)
	{
		typedef void (APIENTRY * DeleteBuffersProc) (GLsizei n, const GLuint *buffers);
		((DeleteBuffersProc)m_glDeleteBuffers)( n, buffers );
	}
	else
	{
		NotSupported( "glDeleteBuffers" );
	}
}