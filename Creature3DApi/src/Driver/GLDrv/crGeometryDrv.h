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
#include <crgl/gl.h>
#include <crgl/GLExtensions.h>
#include <crgl/glu.h>
#include <CRCore/crPrimitive.h>
#include <CRCore/crArray.h>
#include <CRCore/crNotify.h>
#include <CRCore/crImage.h>
#include <CRCore/crTexture2D.h>
#include <CRCore/crStreamBuf.h>
//////////////////////////////////////////////////////////////////////////////
//
//  Extension support
//
//////////////////////////////////////////////////////////////////////////////
class Extensions : public CRCore::Referenced
{
public:
	Extensions(unsigned int contextID)
	{
		m_isVertexProgramSupported = isGLExtensionSupported(contextID,"GL_ARB_vertex_program");
		m_isSecondaryColorSupported = isGLExtensionSupported(contextID,"GL_EXT_secondary_color");
		m_isFogCoordSupported = isGLExtensionSupported(contextID,"GL_EXT_fog_coord");
		m_isMultiTexSupported = isGLExtensionSupported(contextID,"GL_ARB_multitexture");
		m_isOcclusionQuerySupported = isGLExtensionSupported(contextID, "GL_NV_occlusion_query" );
		m_isARBOcclusionQuerySupported = isGLExtensionSupported(contextID, "GL_ARB_occlusion_query" );

		m_glFogCoordfv = ((FogCoordProc)getGLExtensionFuncPtr("glFogCoordfv","glFogCoordfvEXT"));
		m_glSecondaryColor3ubv = ((SecondaryColor3ubvProc)getGLExtensionFuncPtr("glSecondaryColor3ubv","glSecondaryColor3ubvEXT"));
		m_glSecondaryColor3fv = ((SecondaryColor3fvProc)getGLExtensionFuncPtr("glSecondaryColor3fv","glSecondaryColor3fvEXT"));
		m_glMultiTexCoord1f = ((MultiTexCoord1fProc)getGLExtensionFuncPtr("glMultiTexCoord1f","glMultiTexCoord1fARB"));
		m_glMultiTexCoord2fv = ((MultiTexCoordfvProc)getGLExtensionFuncPtr("glMultiTexCoord2fv","glMultiTexCoord2fvARB"));
		m_glMultiTexCoord3fv = ((MultiTexCoordfvProc)getGLExtensionFuncPtr("glMultiTexCoord3fv","glMultiTexCoord3fvARB"));
		m_glMultiTexCoord4fv = ((MultiTexCoordfvProc)getGLExtensionFuncPtr("glMultiTexCoord4fv","glMultiTexCoord4fvARB"));

		m_glVertexAttrib1s = ((VertexAttrib1sProc)getGLExtensionFuncPtr("glVertexAttrib1s","glVertexAttrib1sARB"));
		m_glVertexAttrib1f = ((VertexAttrib1fProc)getGLExtensionFuncPtr("glVertexAttrib1f","glVertexAttrib1fARB"));
		m_glVertexAttrib2fv = ((VertexAttribfvProc)getGLExtensionFuncPtr("glVertexAttrib2fv","glVertexAttrib2fvARB"));
		m_glVertexAttrib3fv = ((VertexAttribfvProc)getGLExtensionFuncPtr("glVertexAttrib3fv","glVertexAttrib3fvARB"));
		m_glVertexAttrib4fv = ((VertexAttribfvProc)getGLExtensionFuncPtr("glVertexAttrib4fv","glVertexAttrib4fvARB"));
		m_glVertexAttrib4ubv = ((VertexAttribubvProc)getGLExtensionFuncPtr("glVertexAttrib4ubv","glVertexAttrib4ubvARB"));
		m_glVertexAttrib4Nubv = ((VertexAttribubvProc)getGLExtensionFuncPtr("glVertexAttrib4Nubv","glVertexAttrib4NubvARB"));

		m_glGenBuffers = ((GenBuffersProc)getGLExtensionFuncPtr("glGenBuffers","glGenBuffersARB"));
		m_glBindBuffer = ((BindBufferProc)getGLExtensionFuncPtr("glBindBuffer","glBindBufferARB"));
		m_glBufferData = ((BufferDataProc)getGLExtensionFuncPtr("glBufferData","glBufferDataARB"));
		m_glBufferSubData = ((BufferSubDataProc)getGLExtensionFuncPtr("glBufferSubData","glBufferSubDataARB"));
		m_glDeleteBuffers = ((DeleteBuffersProc)getGLExtensionFuncPtr("glDeleteBuffers","glDeleteBuffersARB"));
		m_glIsBuffer = ((IsBufferProc)getGLExtensionFuncPtr("glIsBuffer","glIsBufferARB"));
		m_glGetBufferSubData = ((GetBufferSubDataProc)getGLExtensionFuncPtr("glGetBufferSubData","glGetBufferSubDataARB"));
		m_glMapBuffer = ((MapBufferProc)getGLExtensionFuncPtr("glMapBuffer","glMapBufferARB"));
		m_glUnmapBuffer = ((UnmapBufferProc)getGLExtensionFuncPtr("glUnmapBuffer","glUnmapBufferARB"));
		m_glGetBufferParameteriv = ((GetBufferParameterivProc)getGLExtensionFuncPtr("glGetBufferParameteriv","glGetBufferParameterivARB"));
		m_glGetBufferPointerv = ((GetBufferPointervProc)getGLExtensionFuncPtr("glGetBufferPointerv","glGetBufferPointervARB"));

		m_glGenOcclusionQueries = ((GenOcclusionQueriesProc)getGLExtensionFuncPtr("glGenOcclusionQueries","glGenOcclusionQueriesNV"));
		m_glDeleteOcclusionQueries = ((DeleteOcclusionQueriesProc)getGLExtensionFuncPtr("glDeleteOcclusionQueries","glDeleteOcclusionQueriesNV"));
		m_glIsOcclusionQuery = ((IsOcclusionQueryProc)getGLExtensionFuncPtr("glIsOcclusionQuery","_glIsOcclusionQueryNV"));
		m_glBeginOcclusionQuery = ((BeginOcclusionQueryProc)getGLExtensionFuncPtr("glBeginOcclusionQuery","glBeginOcclusionQueryNV"));
		m_glEndOcclusionQuery = ((EndOcclusionQueryProc)getGLExtensionFuncPtr("glEndOcclusionQuery","glEndOcclusionQueryNV"));
		m_glGetOcclusionQueryiv = ((GetOcclusionQueryivProc)getGLExtensionFuncPtr("glGetOcclusionQueryiv","glGetOcclusionQueryivNV"));
		m_glGetOcclusionQueryuiv = ((GetOcclusionQueryuivProc)getGLExtensionFuncPtr("glGetOcclusionQueryuiv","glGetOcclusionQueryuivNV"));

		m_gl_gen_queries_arb = (GenQueriesProc)getGLExtensionFuncPtr("glGenQueries", "glGenQueriesARB");
		m_gl_delete_queries_arb = (DeleteQueriesProc)getGLExtensionFuncPtr("glDeleteQueries", "glDeleteQueriesARB");
		m_gl_is_query_arb = (IsQueryProc)getGLExtensionFuncPtr("glIsQuery", "glIsQueryARB");
		m_gl_begin_query_arb = (BeginQueryProc)getGLExtensionFuncPtr("glBeginQuery", "glBeginQueryARB");
		m_gl_end_query_arb = (EndQueryProc)getGLExtensionFuncPtr("glEndQuery", "glEndQueryARB");
		m_gl_get_queryiv_arb = (GetQueryivProc)getGLExtensionFuncPtr("glGetQueryiv", "glGetQueryivARB");
		m_gl_get_query_objectiv_arb = (GetQueryObjectivProc)getGLExtensionFuncPtr("glGetQueryObjectiv","glGetQueryObjectivARB");
		m_gl_get_query_objectuiv_arb = (GetQueryObjectuivProc)getGLExtensionFuncPtr("glGetQueryObjectuiv","glGetQueryObjectuivARB");
	}

	void setVertexProgramSupported(bool flag) { m_isVertexProgramSupported=flag; }
	bool isVertexProgramSupported() const { return m_isVertexProgramSupported; }

	void setSecondaryColorSupported(bool flag) { m_isSecondaryColorSupported=flag; }
	bool isSecondaryColorSupported() const { return m_isSecondaryColorSupported; }

	void setFogCoordSupported(bool flag) { m_isFogCoordSupported=flag; }
	bool isFogCoordSupported() const { return m_isFogCoordSupported; }

	void setMultiTexSupported(bool flag) { m_isMultiTexSupported=flag; }
	bool isMultiTexSupported() const { return m_isMultiTexSupported; }

	void setOcclusionQuerySupported(bool flag) { m_isOcclusionQuerySupported=flag; }
	bool isOcclusionQuerySupported() const { return m_isOcclusionQuerySupported; }

	void setARBOcclusionQuerySupported(bool flag) { m_isARBOcclusionQuerySupported=flag; }
	bool isARBOcclusionQuerySupported() const { return m_isARBOcclusionQuerySupported; }

	void glSecondaryColor3ubv(const GLubyte* coord) const;
	void glSecondaryColor3fv(const GLfloat* coord) const;

	void glFogCoordfv(const GLfloat* coord) const;

	void glMultiTexCoord1f(GLenum target,GLfloat coord) const;
	void glMultiTexCoord2fv(GLenum target,const GLfloat* coord) const;
	void glMultiTexCoord3fv(GLenum target,const GLfloat* coord) const;
	void glMultiTexCoord4fv(GLenum target,const GLfloat* coord) const;

	void glVertexAttrib1s(unsigned int index, GLshort s) const;
	void glVertexAttrib1f(unsigned int index, GLfloat f) const;
	void glVertexAttrib2fv(unsigned int index, const GLfloat * v) const;
	void glVertexAttrib3fv(unsigned int index, const GLfloat * v) const;
	void glVertexAttrib4fv(unsigned int index, const GLfloat * v) const;
	void glVertexAttrib4ubv(unsigned int index, const GLubyte * v) const;
	void glVertexAttrib4Nubv(unsigned int index, const GLubyte * v) const;


	void glGenBuffers (GLsizei n, GLuint *buffers) const;
	void glBindBuffer (GLenum target, GLuint buffer) const;
	void glBufferData (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage) const;
	void glBufferSubData (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data) const;
	void glDeleteBuffers (GLsizei n, const GLuint *buffers) const;
	GLboolean glIsBuffer (GLuint buffer) const;
	void glGetBufferSubData (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data) const;
	GLvoid* glMapBuffer (GLenum target, GLenum access) const;
	GLboolean glUnmapBuffer (GLenum target) const;
	void glGetBufferParameteriv (GLenum target, GLenum pname, GLint *params) const;
	void glGetBufferPointerv (GLenum target, GLenum pname, GLvoid* *params) const;


	void glGenOcclusionQueries( GLsizei n, GLuint *ids ) const;
	void glDeleteOcclusionQueries( GLsizei n, const GLuint *ids ) const;
	GLboolean glIsOcclusionQuery( GLuint id ) const;
	void glBeginOcclusionQuery( GLuint id ) const;
	void glEndOcclusionQuery() const;
	void glGetOcclusionQueryiv( GLuint id, GLenum pname, GLint *params ) const;
	void glGetOcclusionQueryuiv( GLuint id, GLenum pname, GLuint *params ) const;

	void glGetQueryiv(GLenum target, GLenum pname, GLint *params) const;
	void glGenQueries(GLsizei n, GLuint *ids) const;
	void glBeginQuery(GLenum target, GLuint id) const;
	void glEndQuery(GLenum target) const;
	GLboolean glIsQuery(GLuint id) const;
	void glGetQueryObjectiv(GLuint id, GLenum pname, GLint *params) const;
	void glGetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params) const;
	//static void clearExtensions();
protected:

	typedef void (APIENTRY * FogCoordProc) (const GLfloat* coord);

	typedef void (APIENTRY * VertexAttrib1sProc) (unsigned int index, GLshort s);
	typedef void (APIENTRY * VertexAttrib1fProc) (unsigned int index, GLfloat f);
	typedef void (APIENTRY * VertexAttribfvProc) (unsigned int index, const GLfloat * v);
	typedef void (APIENTRY * VertexAttribubvProc) (unsigned int index, const GLubyte * v);

	typedef void (APIENTRY * SecondaryColor3ubvProc) (const GLubyte* coord);
	typedef void (APIENTRY * SecondaryColor3fvProc) (const GLfloat* coord);

	typedef void (APIENTRY * MultiTexCoord1fProc) (GLenum target,GLfloat coord);
	typedef void (APIENTRY * MultiTexCoordfvProc) (GLenum target,const GLfloat* coord);


	typedef void (APIENTRY * GenBuffersProc) (GLsizei n, GLuint *buffers);
	typedef void (APIENTRY * BindBufferProc) (GLenum target, GLuint buffer);
	typedef void (APIENTRY * BufferDataProc) (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
	typedef void (APIENTRY * BufferSubDataProc) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
	typedef void (APIENTRY * DeleteBuffersProc) (GLsizei n, const GLuint *buffers);
	typedef GLboolean (APIENTRY * IsBufferProc) (GLuint buffer);
	typedef void (APIENTRY * GetBufferSubDataProc) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data);
	typedef GLvoid* (APIENTRY * MapBufferProc) (GLenum target, GLenum access);
	typedef GLboolean (APIENTRY * UnmapBufferProc) (GLenum target);
	typedef void (APIENTRY * GetBufferParameterivProc) (GLenum target, GLenum pname, GLint *params);
	typedef void (APIENTRY * GetBufferPointervProc) (GLenum target, GLenum pname, GLvoid* *params);

	typedef void (APIENTRY * GenOcclusionQueriesProc) ( GLsizei n, GLuint *ids );
	typedef void (APIENTRY * DeleteOcclusionQueriesProc) ( GLsizei n, const GLuint *ids );
	typedef GLboolean (APIENTRY * IsOcclusionQueryProc) ( GLuint id );
	typedef void (APIENTRY * BeginOcclusionQueryProc) ( GLuint id );
	typedef void (APIENTRY * EndOcclusionQueryProc) ();
	typedef void (APIENTRY * GetOcclusionQueryivProc) ( GLuint id, GLenum pname, GLint *params );
	typedef void (APIENTRY * GetOcclusionQueryuivProc) ( GLuint id, GLenum pname, GLuint *params );

	typedef void (APIENTRY *GenQueriesProc) (GLsizei n, GLuint *ids);
	typedef void (APIENTRY *DeleteQueriesProc) (GLsizei n, const GLuint *ids);
	typedef GLboolean (APIENTRY *IsQueryProc) (GLuint id);
	typedef void (APIENTRY *BeginQueryProc) (GLenum target, GLuint id);
	typedef void (APIENTRY *EndQueryProc) (GLenum target);
	typedef void (APIENTRY *GetQueryivProc) (GLenum target, GLenum pname, GLint *params);
	typedef void (APIENTRY *GetQueryObjectivProc) (GLuint id, GLenum pname, GLint *params);
	typedef void (APIENTRY *GetQueryObjectuivProc) (GLuint id, GLenum pname, GLuint *params);

	~Extensions() {}

	bool m_isVertexProgramSupported;
	bool m_isSecondaryColorSupported;
	bool m_isFogCoordSupported;
	bool m_isMultiTexSupported;
	bool m_isOcclusionQuerySupported;
	bool m_isARBOcclusionQuerySupported;

	FogCoordProc m_glFogCoordfv;

	SecondaryColor3ubvProc m_glSecondaryColor3ubv;
	SecondaryColor3fvProc m_glSecondaryColor3fv;

	VertexAttrib1sProc m_glVertexAttrib1s;
	VertexAttrib1fProc m_glVertexAttrib1f;
	VertexAttribfvProc m_glVertexAttrib2fv;
	VertexAttribfvProc m_glVertexAttrib3fv;
	VertexAttribfvProc m_glVertexAttrib4fv;
	VertexAttribubvProc m_glVertexAttrib4ubv;
	VertexAttribubvProc m_glVertexAttrib4Nubv;

	MultiTexCoord1fProc m_glMultiTexCoord1f;
	MultiTexCoordfvProc m_glMultiTexCoord2fv;
	MultiTexCoordfvProc m_glMultiTexCoord3fv;
	MultiTexCoordfvProc m_glMultiTexCoord4fv;

	GenBuffersProc          m_glGenBuffers;
	BindBufferProc          m_glBindBuffer;
	BufferDataProc          m_glBufferData;
	BufferSubDataProc       m_glBufferSubData;
	DeleteBuffersProc       m_glDeleteBuffers;
	IsBufferProc            m_glIsBuffer;
	GetBufferSubDataProc    m_glGetBufferSubData;
	MapBufferProc           m_glMapBuffer;
	UnmapBufferProc         m_glUnmapBuffer;
	GetBufferParameterivProc m_glGetBufferParameteriv;
	GetBufferPointervProc   m_glGetBufferPointerv;

	GenOcclusionQueriesProc m_glGenOcclusionQueries;
	DeleteOcclusionQueriesProc m_glDeleteOcclusionQueries;
	IsOcclusionQueryProc m_glIsOcclusionQuery;
	BeginOcclusionQueryProc m_glBeginOcclusionQuery;
	EndOcclusionQueryProc m_glEndOcclusionQuery;
	GetOcclusionQueryivProc m_glGetOcclusionQueryiv;
	GetOcclusionQueryuivProc m_glGetOcclusionQueryuiv;

	GenQueriesProc m_gl_gen_queries_arb;
	DeleteQueriesProc m_gl_delete_queries_arb;
	IsQueryProc m_gl_is_query_arb;
	BeginQueryProc m_gl_begin_query_arb;
	EndQueryProc m_gl_end_query_arb;
	GetQueryivProc m_gl_get_queryiv_arb;
	GetQueryObjectivProc m_gl_get_query_objectiv_arb;
	GetQueryObjectuivProc  m_gl_get_query_objectuiv_arb;
};
typedef buffered_value< ref_ptr<Extensions> > BufferedExtensions;
static BufferedExtensions s_extensions;
Extensions* getExtensions(unsigned int contextID,bool createIfNotInitalized)
{
	if (!s_extensions[contextID] && createIfNotInitalized) s_extensions[contextID] = new Extensions(contextID);
	return s_extensions[contextID].get();
}
void crGeometry::clearExtensions()
{
	s_extensions.clear();
}
void Extensions::glFogCoordfv(const GLfloat* coord) const
{
    if (m_glFogCoordfv)
    {
        m_glFogCoordfv(coord);
    }
    else
    {
        notify(WARN)<<"Error: glFogCoordfv not supported by OpenGL driver"<<std::endl;
    }    
}

void Extensions::glSecondaryColor3ubv(const GLubyte* coord) const
{
    if (m_glSecondaryColor3ubv)
    {
        m_glSecondaryColor3ubv(coord);
    }
    else
    {
        notify(WARN)<<"Error: glSecondaryColor3ubv not supported by OpenGL driver"<<std::endl;
    }
}

void Extensions::glSecondaryColor3fv(const GLfloat* coord) const
{
    if (m_glSecondaryColor3fv)
    {
        m_glSecondaryColor3fv(coord);
    }
    else
    {
        notify(WARN)<<"Error: glSecondaryColor3fv not supported by OpenGL driver"<<std::endl;
    }
}

void Extensions::glMultiTexCoord1f(GLenum target,GLfloat coord) const
{
    if (m_glMultiTexCoord1f)
    {
        m_glMultiTexCoord1f(target,coord); 
    }
    else
    {
        notify(WARN)<<"Error: glMultiTexCoord1f not supported by OpenGL driver"<<std::endl;
    }
}

void Extensions::glMultiTexCoord2fv(GLenum target,const GLfloat* coord) const
{
    if (m_glMultiTexCoord2fv)
    {
        m_glMultiTexCoord2fv(target,coord); 
    }
    else
    {
        notify(WARN)<<"Error: glMultiTexCoord2fv not supported by OpenGL driver"<<std::endl;
    }
}

void Extensions::glMultiTexCoord3fv(GLenum target,const GLfloat* coord) const
{
    if (m_glMultiTexCoord3fv)
    {
        m_glMultiTexCoord3fv(target,coord); 
    }
    else
    {
        notify(WARN)<<"Error: m_glMultiTexCoord3fv not supported by OpenGL driver"<<std::endl;
    }
}

void Extensions::glMultiTexCoord4fv(GLenum target,const GLfloat* coord) const
{
    if (m_glMultiTexCoord4fv)
    {
        m_glMultiTexCoord4fv(target,coord); 
    }
    else
    {
        notify(WARN)<<"Error: glMultiTexCoord4fv not supported by OpenGL driver"<<std::endl;
    }
}

void Extensions::glVertexAttrib1s(unsigned int index, GLshort s) const
{
    if (m_glVertexAttrib1s)
    {
        m_glVertexAttrib1s(index,s); 
    }
    else
    {
        notify(WARN)<<"Error: glVertexAttrib1s not supported by OpenGL driver"<<std::endl;
    }
}

void Extensions::glVertexAttrib1f(unsigned int index, GLfloat f) const
{
    if (m_glVertexAttrib1f)
    {
        m_glVertexAttrib1f(index,f); 
    }
    else
    {
        notify(WARN)<<"Error: glVertexAttrib1f not supported by OpenGL driver"<<std::endl;
    }
}

void Extensions::glVertexAttrib2fv(unsigned int index, const GLfloat * v) const
{
    if (m_glVertexAttrib2fv)
    {
        m_glVertexAttrib2fv(index,v); 
    }
    else
    {
        notify(WARN)<<"Error: glVertexAttrib2fv not supported by OpenGL driver"<<std::endl;
    }
}

void Extensions::glVertexAttrib3fv(unsigned int index, const GLfloat * v) const
{
    if (m_glVertexAttrib3fv)
    {
        m_glVertexAttrib3fv(index,v); 
    }
    else
    {
        notify(WARN)<<"Error: glVertexAttrib3fv not supported by OpenGL driver"<<std::endl;
    }
}

void Extensions::glVertexAttrib4fv(unsigned int index, const GLfloat * v) const
{
    if (m_glVertexAttrib4fv)
    {
        m_glVertexAttrib4fv(index,v); 
    }
    else
    {
        notify(WARN)<<"Error: glVertexAttrib4fv not supported by OpenGL driver"<<std::endl;
    }
}

void Extensions::glVertexAttrib4ubv(unsigned int index, const GLubyte * v) const
{
    if (m_glVertexAttrib4ubv)
    {
        m_glVertexAttrib4ubv(index,v); 
    }
    else
    {
        notify(WARN)<<"Error: glVertexAttrib4ubv not supported by OpenGL driver"<<std::endl;
    }
}

void Extensions::glVertexAttrib4Nubv(unsigned int index, const GLubyte * v) const
{
    if (m_glVertexAttrib4Nubv)
    {
        m_glVertexAttrib4Nubv(index,v); 
    }
    else
    {
        notify(WARN)<<"Error: glVertexAttrib4Nubv not supported by OpenGL driver"<<std::endl;
    }
}

void Extensions::glGenBuffers(GLsizei n, GLuint *buffers) const
{
    if (m_glGenBuffers) m_glGenBuffers(n, buffers); 
    else notify(WARN)<<"Error: glGenBuffers not supported by OpenGL driver"<<std::endl;
}

void Extensions::glBindBuffer(GLenum target, GLuint buffer) const
{
    if (m_glBindBuffer) m_glBindBuffer(target, buffer); 
    else notify(WARN)<<"Error: glBindBuffer not supported by OpenGL driver"<<std::endl;
}

void Extensions::glBufferData(GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage) const
{
    if (m_glBufferData) m_glBufferData(target, size, data, usage); 
    else notify(WARN)<<"Error: glBufferData not supported by OpenGL driver"<<std::endl;
}

void Extensions::glBufferSubData(GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data) const
{
    if (m_glBufferSubData) m_glBufferSubData(target, offset, size, data); 
    else notify(WARN)<<"Error: glBufferData not supported by OpenGL driver"<<std::endl;
}

void Extensions::glDeleteBuffers(GLsizei n, const GLuint *buffers) const
{
    if (m_glDeleteBuffers) m_glDeleteBuffers(n, buffers); 
    else notify(WARN)<<"Error: glBufferData not supported by OpenGL driver"<<std::endl;
}

GLboolean Extensions::glIsBuffer (GLuint buffer) const
{
    if (m_glIsBuffer) return m_glIsBuffer(buffer);
    else 
    {
        notify(WARN)<<"Error: glIsBuffer not supported by OpenGL driver"<<std::endl;
        return GL_FALSE;
    }
}

void Extensions::glGetBufferSubData (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data) const
{
    if (m_glGetBufferSubData) m_glGetBufferSubData(target,offset,size,data);
    else notify(WARN)<<"Error: glGetBufferSubData not supported by OpenGL driver"<<std::endl;
}

GLvoid* Extensions::glMapBuffer (GLenum target, GLenum access) const
{
    if (m_glMapBuffer) return m_glMapBuffer(target,access);
    else 
    {
        notify(WARN)<<"Error: glMapBuffer not supported by OpenGL driver"<<std::endl;
        return 0;
    }
}

GLboolean Extensions::glUnmapBuffer (GLenum target) const
{
    if (m_glUnmapBuffer) return m_glUnmapBuffer(target);
    else 
    {
        notify(WARN)<<"Error: glUnmapBuffer not supported by OpenGL driver"<<std::endl;
        return GL_FALSE;
    }
}

void Extensions::glGetBufferParameteriv (GLenum target, GLenum pname, GLint *params) const
{
    if (m_glGetBufferParameteriv) m_glGetBufferParameteriv(target,pname,params);
    else notify(WARN)<<"Error: glGetBufferParameteriv not supported by OpenGL driver"<<std::endl;
}

void Extensions::glGetBufferPointerv (GLenum target, GLenum pname, GLvoid* *params) const
{
    if (m_glGetBufferPointerv) m_glGetBufferPointerv(target,pname,params);
    else notify(WARN)<<"Error: glGetBufferPointerv not supported by OpenGL driver"<<std::endl;
}


void Extensions::glGenOcclusionQueries( GLsizei n, GLuint *ids ) const
{
    if (m_glGenOcclusionQueries)
    {
        m_glGenOcclusionQueries( n, ids );
    }
    else
    {
        CRCore::notify(CRCore::WARN)<<"Error: glGenOcclusionQueries not supported by OpenGL driver"<<std::endl;
    }    
}

void Extensions::glDeleteOcclusionQueries( GLsizei n, const GLuint *ids ) const
{
    if (m_glDeleteOcclusionQueries)
    {
        m_glDeleteOcclusionQueries( n, ids );
    }
    else
    {
        CRCore::notify(CRCore::WARN)<<"Error: glDeleteOcclusionQueries not supported by OpenGL driver"<<std::endl;
    }    
}

GLboolean Extensions::glIsOcclusionQuery( GLuint id ) const
{
    if (m_glIsOcclusionQuery)
    {
        return m_glIsOcclusionQuery( id );
    }
    else
    {
        CRCore::notify(CRCore::WARN)<<"Error: glIsOcclusionQuery not supported by OpenGL driver"<<std::endl;
    }    

    return GLboolean( 0 );
}

void Extensions::glBeginOcclusionQuery( GLuint id ) const
{
    if (m_glBeginOcclusionQuery)
    {
        m_glBeginOcclusionQuery( id );
    }
    else
    {
        CRCore::notify(CRCore::WARN)<<"Error: glBeginOcclusionQuery not supported by OpenGL driver"<<std::endl;
    }    
}

void Extensions::glEndOcclusionQuery() const
{
    if (m_glEndOcclusionQuery)
    {
        m_glEndOcclusionQuery();
    }
    else
    {
        CRCore::notify(CRCore::WARN)<<"Error: glEndOcclusionQuery not supported by OpenGL driver"<<std::endl;
    }    
}

void Extensions::glGetOcclusionQueryiv( GLuint id, GLenum pname, GLint *params ) const
{
    if (m_glGetOcclusionQueryiv)
    {
        m_glGetOcclusionQueryiv( id, pname, params );
    }
    else
    {
        CRCore::notify(CRCore::WARN)<<"Error: glGetOcclusionQueryiv not supported by OpenGL driver"<<std::endl;
    }    
}

void Extensions::glGetOcclusionQueryuiv( GLuint id, GLenum pname, GLuint *params ) const
{
    if (m_glGetOcclusionQueryuiv)
    {
        m_glGetOcclusionQueryuiv( id, pname, params );
    }
    else
    {
        CRCore::notify(CRCore::WARN)<<"Error: glGetOcclusionQueryuiv not supported by OpenGL driver"<<std::endl;
    }    
}

void Extensions::glGetQueryiv(GLenum target, GLenum pname, GLint *params) const
{
  if (m_gl_get_queryiv_arb)
    m_gl_get_queryiv_arb(target, pname, params);
  else
    CRCore::notify(CRCore::WARN) << "Error: glGetQueryiv not supported by OpenGL driver" << std::endl;
}

void Extensions::glGenQueries(GLsizei n, GLuint *ids) const
{
  if (m_gl_gen_queries_arb)
    m_gl_gen_queries_arb(n, ids);
  else
    CRCore::notify(CRCore::WARN) << "Error: glGenQueries not supported by OpenGL driver" << std::endl;
}

void Extensions::glBeginQuery(GLenum target, GLuint id) const
{
  if (m_gl_begin_query_arb)
    m_gl_begin_query_arb(target, id);
  else
    CRCore::notify(CRCore::WARN) << "Error: glBeginQuery not supported by OpenGL driver" << std::endl;
}

void Extensions::glEndQuery(GLenum target) const
{
  if (m_gl_end_query_arb)
    m_gl_end_query_arb(target);
  else
    CRCore::notify(CRCore::WARN) << "Error: glEndQuery not supported by OpenGL driver" << std::endl;
}

GLboolean Extensions::glIsQuery(GLuint id) const
{
  if (m_gl_is_query_arb) return m_gl_is_query_arb(id);

  CRCore::notify(CRCore::WARN) << "Error: glIsQuery not supported by OpenGL driver" << std::endl;
  return false;
}

void Extensions::glGetQueryObjectiv(GLuint id, GLenum pname, GLint *params) const
{
	if (m_gl_get_query_objectiv_arb)
		m_gl_get_query_objectiv_arb(id, pname, params);
	else
		CRCore::notify(CRCore::WARN) << "Error: glGetQueryObjectiv not supported by OpenGL driver" << std::endl;
}

void Extensions::glGetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params) const
{
	if (m_gl_get_query_objectuiv_arb)
		m_gl_get_query_objectuiv_arb(id, pname, params);
	else
		CRCore::notify(CRCore::WARN) << "Error: glGetQueryObjectuiv not supported by OpenGL driver" << std::endl;
}

#if 1
class DrawVertex
{
public:

	DrawVertex(const CRCore::Array* vertices,const CRCore::IndexArray* indices):
	  m_vertices(vertices),
	  m_indices(indices)
	  {
		  m_verticesType = m_vertices?m_vertices->getType():Array::ArrayType;
		  m_indicesType = m_indices?m_indices->getType():Array::ArrayType;
	  }

	  inline unsigned int index(unsigned int pos)
	  {
		  switch(m_indicesType)
		  {
		  case(Array::ByteArrayType): return (*static_cast<const ByteArray*>(m_indices))[pos];
		  case(Array::ShortArrayType): return (*static_cast<const ShortArray*>(m_indices))[pos];
		  case(Array::IntArrayType): return (*static_cast<const IntArray*>(m_indices))[pos];
		  case(Array::UByteArrayType): return (*static_cast<const UByteArray*>(m_indices))[pos];
		  case(Array::UShortArrayType): return (*static_cast<const UShortArray*>(m_indices))[pos];
		  case(Array::UIntArrayType): return (*static_cast<const UIntArray*>(m_indices))[pos];
		  default: return 0;
		  }
	  }

	  inline void operator () (unsigned int pos)
	  {
		  if (m_indices) pos = index(pos);

		  switch(m_verticesType)
		  {
		  case(Array::Vec3ArrayType): 
			  apply((*(static_cast<const Vec3Array*>(m_vertices)))[pos]);
			  break;
		  case(Array::Vec2ArrayType): 
			  apply((*(static_cast<const Vec2Array*>(m_vertices)))[pos]);
			  break;
		  case(Array::Vec4ArrayType): 
			  apply((*(static_cast<const Vec4Array*>(m_vertices)))[pos]);
			  break;
		  default:
			  break;
		  }

	  }

	  inline void apply(const crVector2& v)   { glVertex2fv(v.ptr()); }
	  inline void apply(const crVector3& v)   { glVertex3fv(v.ptr()); }
	  inline void apply(const crVector4& v)   { glVertex4fv(v.ptr()); }

	  const Array*        m_vertices;
	  const IndexArray*   m_indices;
	  Array::Type         m_verticesType;
	  Array::Type         m_indicesType;
};
#else
class DrawVertex : public CRCore::ConstValueVisitor
{
public:

	DrawVertex(const Array* vertices,const IndexArray* indices):
	      m_vertices(vertices),
		  m_indices(indices) {}

		  inline void operator () (unsigned int pos)
		  {
			  if (m_indices) m_vertices->accept(m_indices->index(pos),*this);
			  else m_vertices->accept(pos,*this);
		  }

		  virtual void apply(const crVector2& v)   { glVertex2fv(v.ptr()); }
		  virtual void apply(const crVector3& v)   { glVertex3fv(v.ptr()); }
		  virtual void apply(const crVector4& v)   { glVertex4fv(v.ptr()); }

		  const Array*        m_vertices;
		  const IndexArray*   m_indices;
};
#endif

class DrawNormal
{
public:

	DrawNormal(const Vec3Array* normals,const IndexArray* indices):
	      m_normals(normals),
		  m_indices(indices) {}

		  void operator () (unsigned int pos)
		  {
			  if (m_indices) glNormal3fv((*m_normals)[m_indices->index(pos)].ptr());
			  else glNormal3fv((*m_normals)[pos].ptr());
		  }

		  const Vec3Array*   m_normals;
		  const IndexArray*  m_indices;
};

#if 1
class DrawColor
{
public:

	DrawColor(const Array* colors,const IndexArray* indices):
	  m_colors(colors),
      m_indices(indices)
	  {
		  m_colorsType = m_colors?m_colors->getType():Array::ArrayType;
		  m_indicesType = m_indices?m_indices->getType():Array::ArrayType;
	  }

	  inline unsigned int index(unsigned int pos)
	  {
		  switch(m_indicesType)
		  {
		  case(Array::ByteArrayType): return (*static_cast<const ByteArray*>(m_indices))[pos];
		  case(Array::ShortArrayType): return (*static_cast<const ShortArray*>(m_indices))[pos];
		  case(Array::IntArrayType): return (*static_cast<const IntArray*>(m_indices))[pos];
		  case(Array::UByteArrayType): return (*static_cast<const UByteArray*>(m_indices))[pos];
		  case(Array::UShortArrayType): return (*static_cast<const UShortArray*>(m_indices))[pos];
		  case(Array::UIntArrayType): return (*static_cast<const UIntArray*>(m_indices))[pos];
		  default: return 0;
		  }
	  }

	  inline void operator () (unsigned int pos)
	  {
		  if (m_indices) pos = index(pos);

		  switch(m_colorsType)
		  {
		  case(Array::Vec4ArrayType):
			  apply((*static_cast<const Vec4Array*>(m_colors))[pos]);
			  break;
		  case(Array::Vec4ubArrayType):
			  apply((*static_cast<const Vec4ubArray*>(m_colors))[pos]);
			  break;
		  case(Array::Vec3ArrayType):
			  apply((*static_cast<const Vec3Array*>(m_colors))[pos]);
			  break;
		  default:
			  break;
		  }
	  }

	  inline void apply(const crVector4ub& v) { glColor4ubv(v.ptr()); }
	  inline void apply(const crVector3& v)   { glColor3fv(v.ptr()); }
	  inline void apply(const crVector4& v)   { glColor4fv(v.ptr()); }

	  const Array*        m_colors;
	  const IndexArray*   m_indices;
	  Array::Type         m_colorsType;
	  Array::Type         m_indicesType;
};
#else
class DrawColor : public CRCore::ConstValueVisitor
{
public:

	DrawColor(const Array* colors,const IndexArray* indices):
	      m_colors(colors),
		  m_indices(indices) {}

		  inline void operator () (unsigned int pos)
		  {
			  if (m_indices) m_colors->accept(m_indices->index(pos),*this);
			  else m_colors->accept(pos,*this);
		  }

		  virtual void apply(const crVector4ub& v) { glColor4ubv(v.ptr()); }
		  virtual void apply(const crVector3& v)   { glColor3fv(v.ptr()); }
		  virtual void apply(const crVector4& v)   { glColor4fv(v.ptr()); }

		  const Array*        m_colors;
		  const IndexArray*   m_indices;
};
#endif
class DrawVertexAttrib : public CRCore::Referenced, public CRCore::ConstValueVisitor
{
public:

	DrawVertexAttrib(const Extensions * extensions,unsigned int vertAttribIndex,unsigned char  normalized,const Array* attribcoords,const IndexArray* indices):    
	  m_vertAttribIndex(vertAttribIndex),
		  m_normalized(normalized),
		  m_extensions(extensions),
		  m_attribcoords(attribcoords),
		  m_indices(indices),
		  m_index(0) {;}

		  inline void applyAndIncrement() { operator()(m_index++); }

		  inline void operator () (unsigned int pos)
		  {
			  if (m_indices) m_attribcoords->accept(m_indices->index(pos),*this);
			  else m_attribcoords->accept(pos,*this);
		  }

		  virtual void apply(const GLshort& s) 
		  {
			  m_extensions->glVertexAttrib1s( m_vertAttribIndex, s );
		  }
		  virtual void apply(const float& f) 
		  {
			  m_extensions->glVertexAttrib1f( m_vertAttribIndex, f );
		  }
		  virtual void apply(const crVector4ub& v) 
		  {
			  if( m_normalized )
			  {
				  m_extensions->glVertexAttrib4Nubv( m_vertAttribIndex, v.ptr() );
			  }
			  else
			  {
				  m_extensions->glVertexAttrib4ubv( m_vertAttribIndex, v.ptr() );
			  }
		  }
		  virtual void apply(const crVector2& v) 
		  {
			  m_extensions->glVertexAttrib2fv( m_vertAttribIndex, v.ptr() );
		  }
		  virtual void apply(const crVector3& v) 
		  {
			  m_extensions->glVertexAttrib3fv( m_vertAttribIndex, v.ptr() );
		  }
		  virtual void apply(const crVector4& v) 
		  {
			  m_extensions->glVertexAttrib4fv( m_vertAttribIndex, v.ptr() );
		  }

		  unsigned int                    m_vertAttribIndex;
		  unsigned char                        m_normalized;
		  const Extensions*     m_extensions;
		  const Array*                    m_attribcoords;
		  const IndexArray*               m_indices;
		  unsigned int                    m_index;
};

class DrawTexCoord : public CRCore::Referenced, public CRCore::ConstValueVisitor
{
public:

	DrawTexCoord(const Array* texcoords,const IndexArray* indices):
	  m_texcoords(texcoords),
		  m_indices(indices) {}

		  inline void operator () (unsigned int pos)
		  {
			  if (m_indices) m_texcoords->accept(m_indices->index(pos),*this);
			  else m_texcoords->accept(pos,*this);
		  }

		  virtual void apply(const float& v){ glTexCoord1f(v); }
		  virtual void apply(const crVector2& v)   { glTexCoord2fv(v.ptr()); }
		  virtual void apply(const crVector3& v)   { glTexCoord3fv(v.ptr()); }
		  virtual void apply(const crVector4& v)   { glTexCoord4fv(v.ptr()); }

		  const Array*        m_texcoords;
		  const IndexArray*   m_indices;
};

class DrawMultiTexCoord : public CRCore::Referenced, public CRCore::ConstValueVisitor
{
public:

	DrawMultiTexCoord(unsigned int  target,const Array* texcoords,const IndexArray* indices,
		const Extensions * extensions):
	    m_target(target),
		m_texcoords(texcoords),
		m_indices(indices),
		m_extensions(extensions) {}

		inline void operator () (unsigned int pos)
		{
			if (m_indices) m_texcoords->accept(m_indices->index(pos),*this);
			else m_texcoords->accept(pos,*this);
		}

		virtual void apply(const float& v){ m_extensions->glMultiTexCoord1f(m_target,v); }
		virtual void apply(const crVector2& v)   { m_extensions->glMultiTexCoord2fv(m_target,v.ptr()); }
		virtual void apply(const crVector3& v)   { m_extensions->glMultiTexCoord3fv(m_target,v.ptr()); }
		virtual void apply(const crVector4& v)   { m_extensions->glMultiTexCoord4fv(m_target,v.ptr()); }

		unsigned int  m_target;
		const Array*        m_texcoords;
		const IndexArray*   m_indices;

		const Extensions * m_extensions;
};


class DrawSecondaryColor : public CRCore::ConstValueVisitor
{
public:

	DrawSecondaryColor(const Array* colors,const IndexArray* indices,
		const Extensions * extensions):
	    m_colors(colors),
		m_indices(indices),
		m_extensions(extensions)
	{}

	inline void operator () (unsigned int pos)
	{
		if (m_indices) m_colors->accept(m_indices->index(pos),*this);
		else m_colors->accept(pos,*this);
	}

	virtual void apply(const crVector4ub& v) { m_extensions->glSecondaryColor3ubv(v.ptr()); }
	virtual void apply(const crVector3& v)   { m_extensions->glSecondaryColor3fv(v.ptr()); }
	virtual void apply(const crVector4& v)   { m_extensions->glSecondaryColor3fv(v.ptr()); }

	const Array*        m_colors;
	const IndexArray*   m_indices;

	const Extensions * m_extensions;
};

class DrawFogCoord : public CRCore::ConstValueVisitor
{
public:

	DrawFogCoord(const Array* fogcoords,const IndexArray* indices,const Extensions * extensions):
	      m_fogcoords(fogcoords),
		  m_indices(indices),
		  m_extensions(extensions) {}

		  inline void operator () (unsigned int pos)
		  {
			  if (m_indices) m_fogcoords->accept(m_indices->index(pos),*this);
			  else m_fogcoords->accept(pos,*this);
		  }

		  virtual void apply(const float& v) { m_extensions->glFogCoordfv(&v); }

		  const Array*        m_fogcoords;
		  const IndexArray*   m_indices;

		  const Extensions * m_extensions;
};

void crGeometry::drawImplementation(CRCore::crState& state ) const
{
	if (m_internalOptimizedGeometry.valid())
	{
		m_internalOptimizedGeometry->drawImplementation(state);
		return;
	}

	const Extensions* extensions = getExtensions(state.getContextID(),true);

	//const crGeometry::ArrayData &m_vertexData = m_pgeometry->getVertexData();
	//const crGeometry::ArrayData &m_colorData = m_pgeometry->getColorData();
	//const crGeometry::ArrayData &m_secondaryColorData = m_pgeometry->getSecondaryColorData();
	//const crGeometry::ArrayData &m_fogCoordData = m_pgeometry->getFogCoordData();
	//const crGeometry::Vec3ArrayData &m_normalData = m_pgeometry->getNormalData();

	//const crGeometry::ArrayList &m_vertexAttribList = m_pgeometry->getVertexAttribArrayList();
 //   const crGeometry::ArrayList& m_texCoordList = m_pgeometry->getTexCoordArrayList();
	//const crGeometry::PrimitiveList& m_primitives = m_pgeometry->getPrimitiveList();

	if( !( ( m_vertexData.array.valid() && m_vertexData.array->getNumElements() != 0 ) ||
		( m_vertexAttribList.size() > 0 && 
		m_vertexAttribList[0].array.valid() && 
		m_vertexAttribList[0].array->getNumElements() != 0 ) ) )
	{
		return;
	}

	if( ( m_vertexData.indices.valid() && m_vertexData.indices->getNumElements() == 0 ) ||
		( m_vertexAttribList.size() > 0 && 
		m_vertexAttribList[0].indices.valid() && 
		m_vertexAttribList[0].indices->getNumElements() == 0 ) )
	{
		return;
	}

	//DrawNormal         drawNormal(m_normalData.array.get(),m_normalData.indices.get());
	//DrawColor          drawColor(m_colorData.array.get(),m_colorData.indices.get());
	//DrawSecondaryColor drawSecondaryColor(m_secondaryColorData.array.get(),m_secondaryColorData.indices.get(),extensions);

	//DrawFogCoord       drawFogCoord(m_fogCoordData.array.get(),m_fogCoordData.indices.get(),extensions);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Set up secondary color if required.
	//
	crGeometry::AttributeBinding secondaryColorBinding = m_secondaryColorData.binding;
	if (secondaryColorBinding!=crGeometry::BIND_OFF && !extensions->isSecondaryColorSupported())
	{
		// switch off if not supported or have a valid data.
		secondaryColorBinding = crGeometry::BIND_OFF;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Set up fog coord if required.
	//
	crGeometry::AttributeBinding fogCoordBinding = m_fogCoordData.binding;
	if (fogCoordBinding!=crGeometry::BIND_OFF && !extensions->isFogCoordSupported())
	{
		// switch off if not supported or have a valid data.
		fogCoordBinding = crGeometry::BIND_OFF;
	}

	unsigned int normalIndex = 0;
	unsigned int colorIndex = 0;
	unsigned int secondaryColorIndex = 0;
	unsigned int fogCoordIndex = 0;

#define USE_DEFAULT_NORMAL 1
#if USE_DEFAULT_NORMAL
	// if no values are defined for normal and color provide some defaults...
	if (m_normalData.binding==crGeometry::BIND_OFF) glNormal3f(0.0f,0.0f,1.0f);
#endif

//#define USE_DEFAULT_COLOUR 1
//#if USE_DEFAULT_COLOUR
	if (m_colorData.binding==crGeometry::BIND_OFF) 
		state.applyColor(m_color);//glColor4fv(m_pgeometry->getColor().ptr());
	else
		state.colorDirty();
//#ifdef _DEBUG
//	else
//	    CRCore::notify(WARN)<<"crGeometry::drawImplementation m_colorData.binding = "<<m_colorData.binding<< std::endl;
//#endif
//#endif

	typedef std::vector< ref_ptr<DrawVertexAttrib> > DrawVertexAttribList;
	typedef std::map< crGeometry::AttributeBinding, DrawVertexAttribList> DrawVertexAttribMap;
	DrawVertexAttribMap drawVertexAttribMap;

	bool vertexVertexAttributesSupported = extensions->isVertexProgramSupported();
	bool handleVertexAttributes = (!m_vertexAttribList.empty() && vertexVertexAttributesSupported);

	bool usingVertexBufferObjects = m_useVertexBufferObjects && state.isVertexBufferObjectSupported();
	//bool usingIndexBufferObjects = m_useIndexBufferObjects;

	if (m_fastPath && m_fastPathHint)
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// fast path.        
		//
		if (usingVertexBufferObjects)
		{
			//
			// Vertex Buffer Object path for defining vertex arrays.
			// 

			unsigned int & buffer = m_vboList[state.getContextID()];
			if (!buffer)
			{

				//CRCore::notify(ALWAYS) << "creating VertexBuffer "<<buffer<<std::endl;

				extensions->glGenBuffers(1, &buffer);
				extensions->glBindBuffer(GL_ARRAY_BUFFER_ARB,buffer);

				//CRCore::notify(ALWAYS)<< "  gen VertexBuffer "<<buffer<< std::endl;
				// compute total size and offsets required.
				unsigned int totalSize = 0;

				m_vertexData.offset = 0;
				if (m_vertexData.array.valid()) totalSize += m_vertexData.array->getTotalDataSize();

				m_normalData.offset = totalSize; 
				if (m_normalData.array.valid()) totalSize += m_normalData.array->getTotalDataSize();

				m_colorData.offset = totalSize;
				if (m_colorData.array.valid()) totalSize += m_colorData.array->getTotalDataSize();
				
				m_secondaryColorData.offset = totalSize;
				if (m_secondaryColorData.array.valid()) totalSize += m_secondaryColorData.array->getTotalDataSize();

				m_fogCoordData.offset = totalSize;
				if (m_fogCoordData.array.valid()) totalSize += m_fogCoordData.array->getTotalDataSize();

				unsigned int unit;
				for(unit=0;unit<m_texCoordList.size();++unit)
				{
					m_texCoordList[unit].offset = totalSize;
					const Array* array = m_texCoordList[unit].array.get();
					if (array)
						totalSize += array->getTotalDataSize();

				}

				if( handleVertexAttributes )
				{
					unsigned int index;
					for( index = 0; index < m_vertexAttribList.size(); ++index )
					{
						m_vertexAttribList[index].offset = totalSize;
						const Array* array = m_vertexAttribList[index].array.get();
						const crGeometry::AttributeBinding ab = m_vertexAttribList[index].binding;
						if( ab == crGeometry::BIND_PER_VERTEX && array )
						{
							totalSize += array->getTotalDataSize();           
						}
					}
				}

				// allocated the buffer space, but leave the copy to be done per vertex array below
				extensions->glBufferData(GL_ARRAY_BUFFER_ARB,totalSize, 0, /*GL_DYNAMIC_DRAW_ARB*/GL_STATIC_DRAW_ARB);

				//CRCore::notify(ALWAYS)<< "   Created VertexBuffer"<<buffer<<" size="<<totalSize<<std::endl;

				//
				// copy the data
				//
				if( m_vertexData.array.valid() )
					extensions->glBufferSubData(GL_ARRAY_BUFFER_ARB, m_vertexData.offset, m_vertexData.array->getTotalDataSize(),m_vertexData.array->getDataPointer());

				if (m_normalData.binding==crGeometry::BIND_PER_VERTEX)
					extensions->glBufferSubData(GL_ARRAY_BUFFER_ARB, m_normalData.offset, m_normalData.array->getTotalDataSize(),m_normalData.array->getDataPointer());

				if (m_colorData.binding==crGeometry::BIND_PER_VERTEX)
					extensions->glBufferSubData(GL_ARRAY_BUFFER_ARB, m_colorData.offset, m_colorData.array->getTotalDataSize(),m_colorData.array->getDataPointer());
				
				if (secondaryColorBinding==crGeometry::BIND_PER_VERTEX)
					extensions->glBufferSubData(GL_ARRAY_BUFFER_ARB, m_secondaryColorData.offset, m_secondaryColorData.array->getTotalDataSize(),m_secondaryColorData.array->getDataPointer());

				if (fogCoordBinding==crGeometry::BIND_PER_VERTEX)
					extensions->glBufferSubData(GL_ARRAY_BUFFER_ARB, m_fogCoordData.offset, m_fogCoordData.array->getTotalDataSize(),m_fogCoordData.array->getDataPointer());

				for(unit=0;unit<m_texCoordList.size();++unit)
				{
					const Array* array = m_texCoordList[unit].array.get();
					if (array)
						extensions->glBufferSubData(GL_ARRAY_BUFFER_ARB, m_texCoordList[unit].offset, array->getTotalDataSize(), array->getDataPointer());
				}

				if( handleVertexAttributes )
				{
					unsigned int index;
					for( index = 0; index < m_vertexAttribList.size(); ++index )
					{
						const Array* array = m_vertexAttribList[index].array.get();
						const crGeometry::AttributeBinding ab = m_vertexAttribList[index].binding;

						if( ab == crGeometry::BIND_PER_VERTEX && array )
						{
							extensions->glBufferSubData(GL_ARRAY_BUFFER_ARB, m_vertexAttribList[index].offset, array->getTotalDataSize(), array->getDataPointer());
						}
					}
				}

			}

			//std::cout << "binding VertexBuffer "<<buffer<<std::endl;

			extensions->glBindBuffer(GL_ARRAY_BUFFER_ARB,buffer);

			if( m_vertexData.array.valid() )
				state.setVertexPointer(m_vertexData.array->getDataSize(),m_vertexData.array->getDataType(),0,(const void*)m_vertexData.offset);
			else
				state.disableVertexPointer();

			if (m_normalData.binding==crGeometry::BIND_PER_VERTEX)
				state.setNormalPointer(GL_FLOAT,0,(const void*)m_normalData.offset);
			else
				state.disableNormalPointer();


			if (m_colorData.binding==crGeometry::BIND_PER_VERTEX)
				state.setColorPointer(m_colorData.array->getDataSize(),m_colorData.array->getDataType(),0,(const void*)m_colorData.offset);
			else
				state.disableColorPointer();

			if (secondaryColorBinding==crGeometry::BIND_PER_VERTEX)
				state.setSecondaryColorPointer(m_secondaryColorData.array->getDataSize(),m_secondaryColorData.array->getDataType(),0,(const void*)m_secondaryColorData.offset);
			else
				state.disableSecondaryColorPointer();

			if (fogCoordBinding==crGeometry::BIND_PER_VERTEX)
				state.setFogCoordPointer(GL_FLOAT,0,(const void*)m_fogCoordData.offset);
			else
				state.disableFogCoordPointer();

			unsigned int unit;
			for(unit=0;unit<m_texCoordList.size();++unit)
			{
				const Array* array = m_texCoordList[unit].array.get();
				if (array)
					state.setTexCoordPointer(unit,array->getDataSize(),array->getDataType(),0,(const void*)m_texCoordList[unit].offset);
				else
					state.disableTexCoordPointer(unit);
			}

			state.disableTexCoordPointersAboveAndIncluding(unit);

			if( handleVertexAttributes )
			{
				unsigned int index;
				for( index = 0; index < m_vertexAttribList.size(); ++index )
				{
					const Array* array = m_vertexAttribList[index].array.get();
					const crGeometry::AttributeBinding ab = m_vertexAttribList[index].binding;

					if( ab == crGeometry::BIND_PER_VERTEX && array )
					{
						state.setVertexAttribPointer( index, array->getDataSize(), array->getDataType(), 
							m_vertexAttribList[index].normalize, 0, (const void*)m_vertexAttribList[index].offset );
					}
					else
					{
						if( array )
						{
							const IndexArray* indexArray = m_vertexAttribList[index].indices.get();

							if( indexArray && indexArray->getNumElements() > 0 )
							{
								drawVertexAttribMap[ab].push_back( 
									new DrawVertexAttrib(extensions,index,m_vertexAttribList[index].normalize,array,indexArray) );
							}
							else
							{
								drawVertexAttribMap[ab].push_back( 
									new DrawVertexAttrib(extensions,index,m_vertexAttribList[index].normalize,array,0) );
							}
						}

						state.disableVertexAttribPointer( index );
					}
				}
				state.disableVertexAttribPointersAboveAndIncluding( index );

			}
			else if (vertexVertexAttributesSupported)
			{
				state.disableVertexAttribPointersAboveAndIncluding( 0 );
			}

		}
		else
		{
			//std::cout << "none VertexBuffer path"<<std::endl;

			//CRCore::notify(CRCore::WARN)<<"VertexBufferObjectSupported is being disabled,Now useing none VertexBuffer path"<<std::endl;

			//
			// None Vertex Buffer Object path for defining vertex arrays.
			//  
			if( m_vertexData.array.valid() )
				state.setVertexPointer(m_vertexData.array->getDataSize(),m_vertexData.array->getDataType(),0,m_vertexData.array->getDataPointer());
			else
				state.disableVertexPointer();

			if (m_normalData.binding==crGeometry::BIND_PER_VERTEX)
				state.setNormalPointer(GL_FLOAT,0,m_normalData.array->getDataPointer());
			else
				state.disableNormalPointer();

			if (m_colorData.binding==crGeometry::BIND_PER_VERTEX)
				state.setColorPointer(m_colorData.array->getDataSize(),m_colorData.array->getDataType(),0,m_colorData.array->getDataPointer());
			else
				state.disableColorPointer();

			if (secondaryColorBinding==crGeometry::BIND_PER_VERTEX)
				state.setSecondaryColorPointer(m_secondaryColorData.array->getDataSize(),m_secondaryColorData.array->getDataType(),0,m_secondaryColorData.array->getDataPointer());
			else
				state.disableSecondaryColorPointer();

			if (fogCoordBinding==crGeometry::BIND_PER_VERTEX)
				state.setFogCoordPointer(GL_FLOAT,0,m_fogCoordData.array->getDataPointer());
			else
				state.disableFogCoordPointer();

			unsigned int unit;
			for(unit=0;unit<m_texCoordList.size();++unit)
			{
				const Array* array = m_texCoordList[unit].array.get();
				if (array)
					state.setTexCoordPointer(unit,array->getDataSize(),array->getDataType(),0,array->getDataPointer());
				else
					state.disableTexCoordPointer(unit);
			}
			state.disableTexCoordPointersAboveAndIncluding(unit);

			if( handleVertexAttributes )
			{
				unsigned int index;
				for( index = 0; index < m_vertexAttribList.size(); ++index )
				{
					const Array* array = m_vertexAttribList[index].array.get();
					const crGeometry::AttributeBinding ab = m_vertexAttribList[index].binding;

					if( ab == crGeometry::BIND_PER_VERTEX && array )
					{
						state.setVertexAttribPointer( index, array->getDataSize(), array->getDataType(), 
							m_vertexAttribList[index].normalize, 0, array->getDataPointer() );
					}
					else
					{
						if( array )
						{
							const IndexArray* indexArray = m_vertexAttribList[index].indices.get();

							if( indexArray && indexArray->getNumElements() > 0 )
							{
								drawVertexAttribMap[ab].push_back( 
									new DrawVertexAttrib(extensions,index,m_vertexAttribList[index].normalize,array,indexArray) );
							}
							else
							{
								drawVertexAttribMap[ab].push_back( 
									new DrawVertexAttrib(extensions,index,m_vertexAttribList[index].normalize,array,0) );
							}
						}

						state.disableVertexAttribPointer( index );
					}
				}
				state.disableVertexAttribPointersAboveAndIncluding( index );

			}
			else if (vertexVertexAttributesSupported)
			{
				state.disableVertexAttribPointersAboveAndIncluding( 0 );
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// pass the overall binding values onto OpenGL.
		//
		//if (m_normalData.binding==crGeometry::BIND_OVERALL)           drawNormal(normalIndex++);
		//if (m_colorData.binding==crGeometry::BIND_OVERALL)            drawColor(colorIndex++);
		//if (secondaryColorBinding==crGeometry::BIND_OVERALL)    drawSecondaryColor(secondaryColorIndex++);
		//if (fogCoordBinding==crGeometry::BIND_OVERALL)          drawFogCoord(fogCoordIndex++);
		//if (handleVertexAttributes)
		//{
		//	DrawVertexAttribList &list = drawVertexAttribMap[crGeometry::BIND_OVERALL];

		//	for( unsigned int i = 0; i < list.size(); ++i )
		//	{
		//		list[i]->applyAndIncrement();
		//	}
		//}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// draw the m_primitives themselves.
		//
		for(crGeometry::PrimitiveList::const_iterator itr=m_primitives.begin();
			itr!=m_primitives.end();
			++itr)
		{

			//if (m_normalData.binding==crGeometry::BIND_PER_PRIMITIVE_SET)       drawNormal(normalIndex++);
			//if (m_colorData.binding==crGeometry::BIND_PER_PRIMITIVE_SET)        drawColor(colorIndex++);
			//if (secondaryColorBinding==crGeometry::BIND_PER_PRIMITIVE_SET)    drawSecondaryColor(secondaryColorIndex++);
			//if (fogCoordBinding==crGeometry::BIND_PER_PRIMITIVE_SET)          drawFogCoord(fogCoordIndex++);
			//if (handleVertexAttributes)
			//{
			//	DrawVertexAttribList &list = drawVertexAttribMap[crGeometry::BIND_PER_PRIMITIVE_SET];

			//	for( unsigned int i = 0; i < list.size(); ++i )
			//	{
			//		list[i]->applyAndIncrement();
			//	}
			//}
			//if(pgeometry->getName().compare("Create")==0)
			//{
			//	CRCore::notify(CRCore::FATAL)<<"_drawImplementation PrimitiveList  "<<(*itr)->getDrawCallback()<<std::endl;
			//}
			(*itr)->draw(state, usingVertexBufferObjects/*m_useIndexBufferObjects*/);
		}
		if (usingVertexBufferObjects)
		{
			extensions->glBindBuffer(GL_ARRAY_BUFFER_ARB,0);
		}
    }
	else
	{   

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// slow path.        
		//
//#ifdef _DEBUG
		//CRCore::notify(CRCore::WARN)<<"using slow path"<<std::endl;
		char gbuf[128];
		sprintf(gbuf,"using slow path %s\n\0",getParent(0)->getName().c_str());
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
//#endif
		DrawNormal         drawNormal(m_normalData.array.get(),m_normalData.indices.get());
		DrawColor          drawColor(m_colorData.array.get(),m_colorData.indices.get());
		DrawSecondaryColor drawSecondaryColor(m_secondaryColorData.array.get(),m_secondaryColorData.indices.get(),extensions);

		DrawFogCoord       drawFogCoord(m_fogCoordData.array.get(),m_fogCoordData.indices.get(),extensions);

		typedef std::vector< ref_ptr<DrawMultiTexCoord> > DrawTexCoordList;
		DrawTexCoordList drawTexCoordList;
		drawTexCoordList.reserve(m_texCoordList.size());

		// fallback if multitexturing not supported.
		ref_ptr<DrawTexCoord> drawTextCoord;

		if (extensions->isMultiTexSupported() && m_texCoordList.size()>1)
		{
			// multitexture supported..
			for(unsigned int unit=0;unit!=m_texCoordList.size();++unit)
			{
				const crGeometry::ArrayData& texcoordData = getTexCoordData(unit);
				if (texcoordData.array.valid() && texcoordData.array->getNumElements()>0)
				{
					if (texcoordData.indices.valid() && texcoordData.indices->getNumElements()>0)
					{
						drawTexCoordList.push_back(new DrawMultiTexCoord(GL_TEXTURE0+unit,texcoordData.array.get(),texcoordData.indices.get(),
							extensions));
					}
					else
					{
						drawTexCoordList.push_back(new DrawMultiTexCoord(GL_TEXTURE0+unit,texcoordData.array.get(),0,
							extensions));
					}
				}
			}
		}
		else
		{
			if (!m_texCoordList.empty())
			{
				const crGeometry::ArrayData& texcoordData = m_texCoordList[0];
				if (texcoordData.array.valid() && texcoordData.array->getNumElements()>0)
				{
					if (texcoordData.indices.valid())
					{
						if (texcoordData.indices->getNumElements()>0)
						{
							drawTextCoord = new DrawTexCoord(texcoordData.array.get(),texcoordData.indices.get());
						}
					}
					else
					{
						drawTextCoord = new DrawTexCoord(texcoordData.array.get(),0);
					}
				}
			}
		}

		if(handleVertexAttributes)
		{
			unsigned int index;
			for( index = 1; index < m_vertexAttribList.size(); ++index )
			{
				const crGeometry::ArrayData& vertAttribData = m_vertexAttribList[index];

				if( vertAttribData.array.valid() && vertAttribData.array->getNumElements() > 0 )
				{
					if( vertAttribData.indices.valid() && vertAttribData.indices->getNumElements() > 0 )
					{
						drawVertexAttribMap[vertAttribData.binding].push_back( 
							new DrawVertexAttrib(extensions,index,vertAttribData.normalize,vertAttribData.array.get(),vertAttribData.indices.get() ));
					}
					else
					{
						drawVertexAttribMap[vertAttribData.binding].push_back( 
							new DrawVertexAttrib(extensions,index,vertAttribData.normalize,vertAttribData.array.get(),0) );
					}            
				}
			}
		}

		// disable all the vertex arrays in the slow path as we are
		// sending everything using glVertex etc.
		state.disableAllVertexArrays();


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// pass the overall binding values onto OpenGL.
		//
		if (m_normalData.binding==crGeometry::BIND_OVERALL)      drawNormal(normalIndex++);
		if (m_colorData.binding==crGeometry::BIND_OVERALL)       drawColor(colorIndex++);
		if (secondaryColorBinding==crGeometry::BIND_OVERALL)    drawSecondaryColor(secondaryColorIndex++);
		if (fogCoordBinding==crGeometry::BIND_OVERALL)          drawFogCoord(fogCoordIndex++);
		if (handleVertexAttributes)
		{
			DrawVertexAttribList &list = drawVertexAttribMap[crGeometry::BIND_OVERALL];

			for( unsigned int i = 0; i < list.size(); ++i )
			{
				list[i]->applyAndIncrement();
			}
		}

		// set up vertex functor.
		DrawVertex drawVertex(m_vertexData.array.get(),m_vertexData.indices.get());

		bool useVertexAttrib =  m_vertexAttribList.size() > 0 &&
			m_vertexAttribList[0].array.valid() && 
			m_vertexAttribList[0].indices->getNumElements();

		ref_ptr<DrawVertexAttrib> drawVertexAttribZero;
		if( useVertexAttrib )
		{
			drawVertexAttribZero = new DrawVertexAttrib(extensions,0,
				m_vertexAttribList[0].normalize,m_vertexAttribList[0].array.get(),
				m_vertexAttribList[0].indices.get()); 
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// draw the m_primitives themselves.
		//
		for(crGeometry::PrimitiveList::const_iterator itr=m_primitives.begin();
			itr!=m_primitives.end();
			++itr)
		{
			if (m_normalData.binding==crGeometry::BIND_PER_PRIMITIVE_SET)       drawNormal(normalIndex++);
			if (m_colorData.binding==crGeometry::BIND_PER_PRIMITIVE_SET)        drawColor(colorIndex++);
			if (secondaryColorBinding==crGeometry::BIND_PER_PRIMITIVE_SET)    drawSecondaryColor(secondaryColorIndex++);
			if (fogCoordBinding==crGeometry::BIND_PER_PRIMITIVE_SET)          drawFogCoord(fogCoordIndex++);
			if (handleVertexAttributes)
			{
				DrawVertexAttribList &list = drawVertexAttribMap[crGeometry::BIND_PER_PRIMITIVE_SET];

				for( unsigned int i = 0; i < list.size(); ++i )
				{
					list[i]->applyAndIncrement();
				}
			}

			const crPrimitive* primitive = itr->get();
			unsigned int  mode=primitive->getMode();
			unsigned int primLength;
			switch(mode)
			{
			case crPrimitive::PT_POINTS:
				mode = GL_POINTS;
				primLength = 1; 
				break;
			case crPrimitive::PT_LINES:
				mode = GL_LINES;
				primLength = 2; 
				break;
			case crPrimitive::PT_TRIANGLES:
				mode = GL_TRIANGLES;
				primLength = 3; 
				break;
			case crPrimitive::PT_QUADS:
				mode = GL_QUADS;
				primLength = 4; 
				break;
			case crPrimitive::PT_LINE_STRIP:
				mode = GL_LINE_STRIP;
				primLength = 0; 
				break;
			case crPrimitive::PT_LINE_LOOP:
				mode = GL_LINE_LOOP;
				primLength = 0; 
				break;
			case crPrimitive::PT_TRIANGLE_STRIP:
				mode = GL_TRIANGLE_STRIP;
				primLength = 0; 
				break;
			case crPrimitive::PT_TRIANGLE_FAN:
				mode = GL_TRIANGLE_FAN;
				primLength = 0; 
				break;
			case crPrimitive::PT_QUAD_STRIP:
				mode = GL_QUAD_STRIP;
				primLength = 0; 
				break;
			case crPrimitive::PT_POLYGON:
				mode = GL_POLYGON;
				primLength = 0; 
				break;
			}


			// draw primtives by the more flexible "slow" path,
			// sending OpenGL glBegin/glVertex.../glEnd().
			switch(primitive->getType())
			{
			case(crPrimitive::DrawArraysPrimitiveType):
				{
					if (primLength==0) primLength=primitive->getNumIndices();

					const DrawArrays* drawArray = static_cast<const DrawArrays*>(primitive);
					glBegin(mode);

					unsigned int primCount=0;
					unsigned int indexEnd = drawArray->getFirst()+drawArray->getCount();
					for(unsigned int vindex=drawArray->getFirst();
						vindex<indexEnd;
						++vindex,++primCount)
					{

						if ((primCount%primLength)==0)
						{
							if (m_normalData.binding==crGeometry::BIND_PER_PRIMITIVE)           drawNormal(normalIndex++);
							if (m_colorData.binding==crGeometry::BIND_PER_PRIMITIVE)            drawColor(colorIndex++);
							if (secondaryColorBinding==crGeometry::BIND_PER_PRIMITIVE)    drawSecondaryColor(secondaryColorIndex++);
							if (fogCoordBinding==crGeometry::BIND_PER_PRIMITIVE)          drawFogCoord(fogCoordIndex++);
							if (handleVertexAttributes)
							{
								DrawVertexAttribList &list = drawVertexAttribMap[crGeometry::BIND_PER_PRIMITIVE];

								for( unsigned int i = 0; i < list.size(); ++i )
								{
									list[i]->applyAndIncrement();
								}
							}                        
						}

						if (m_normalData.binding==crGeometry::BIND_PER_VERTEX)           drawNormal(vindex);
						if (m_colorData.binding==crGeometry::BIND_PER_VERTEX)            drawColor(vindex);
						if (secondaryColorBinding==crGeometry::BIND_PER_VERTEX)    drawSecondaryColor(vindex);
						if (fogCoordBinding==crGeometry::BIND_PER_VERTEX)          drawFogCoord(vindex);
						if (handleVertexAttributes)
						{
							DrawVertexAttribList &list = drawVertexAttribMap[crGeometry::BIND_PER_VERTEX];

							for( unsigned int i = 0; i < list.size(); ++i )
							{
								list[i]->applyAndIncrement();
							}
						}  

						for(DrawTexCoordList::iterator texItr=drawTexCoordList.begin();
							texItr!=drawTexCoordList.end();
							++texItr)
						{
							(*(*texItr))(vindex);
						}
						if (drawTextCoord.valid()) (*drawTextCoord)(vindex);

						if( useVertexAttrib )
						{
							(*drawVertexAttribZero)(vindex);
						}
						else
						{
							drawVertex(vindex);
						}
					}

					glEnd();
					break;
				}
			case(crPrimitive::DrawArrayLengthsPrimitiveType):
				{

					const DrawArrayLengths* drawArrayLengths = static_cast<const DrawArrayLengths*>(primitive);
					const DrawArrayLengths::PT_IndexArray &lhs = drawArrayLengths->getIndexArray();
					unsigned int vindex=drawArrayLengths->getFirst();
					for(DrawArrayLengths::PT_IndexArray::const_iterator primItr=lhs.begin();
						primItr!=lhs.end();
						++primItr)
					{
						unsigned int localPrimLength;
						if (primLength==0) localPrimLength=*primItr;
						else localPrimLength=primLength;

						glBegin(mode);

						for(GLsizei primCount=0;primCount<*primItr;++primCount)
						{
							if ((primCount%localPrimLength)==0)
							{
								if (m_normalData.binding==crGeometry::BIND_PER_PRIMITIVE)           drawNormal(normalIndex++);
								if (m_colorData.binding==crGeometry::BIND_PER_PRIMITIVE)            drawColor(colorIndex++);
								if (secondaryColorBinding==crGeometry::BIND_PER_PRIMITIVE)    drawSecondaryColor(secondaryColorIndex++);
								if (fogCoordBinding==crGeometry::BIND_PER_PRIMITIVE)          drawFogCoord(fogCoordIndex++);
								if (handleVertexAttributes)
								{
									DrawVertexAttribList &list = drawVertexAttribMap[crGeometry::BIND_PER_PRIMITIVE];

									for( unsigned int i = 0; i < list.size(); ++i )
									{
										list[i]->applyAndIncrement();
									}
								}  
							}

							if (m_normalData.binding==crGeometry::BIND_PER_VERTEX)           drawNormal(vindex);
							if (m_colorData.binding==crGeometry::BIND_PER_VERTEX)            drawColor(vindex);
							if (secondaryColorBinding==crGeometry::BIND_PER_VERTEX)    drawSecondaryColor(vindex);
							if (fogCoordBinding==crGeometry::BIND_PER_VERTEX)          drawFogCoord(vindex);
							if (handleVertexAttributes)
							{
								DrawVertexAttribList &list = drawVertexAttribMap[crGeometry::BIND_PER_VERTEX];

								for( unsigned int i = 0; i < list.size(); ++i )
								{
									list[i]->applyAndIncrement();
								}
							}  
							for(DrawTexCoordList::iterator texItr=drawTexCoordList.begin();
								texItr!=drawTexCoordList.end();
								++texItr)
							{
								(*(*texItr))(vindex);
							}
							if (drawTextCoord.valid()) (*drawTextCoord)(vindex);

							if( useVertexAttrib )
							{
								(*drawVertexAttribZero)(vindex);
							}
							else
							{
								drawVertex(vindex);
							}

							++vindex;
						}

						glEnd();

					}
					break;
				}
			case(crPrimitive::DrawElementsUBytePrimitiveType):
				{
					if (primLength==0) primLength=primitive->getNumIndices();

					const DrawElementsUByte* drawElements = static_cast<const DrawElementsUByte*>(primitive);
					glBegin(mode);

					unsigned int primCount=0;
					for(DrawElementsUByte::PT_IndexArray::const_iterator primItr=drawElements->getIndexArray().begin();
						primItr!=drawElements->getIndexArray().end();
						++primCount,++primItr)
					{

						if ((primCount%primLength)==0)
						{
							if (m_normalData.binding==crGeometry::BIND_PER_PRIMITIVE)           drawNormal(normalIndex++);
							if (m_colorData.binding==crGeometry::BIND_PER_PRIMITIVE)            drawColor(colorIndex++);
							if (secondaryColorBinding==crGeometry::BIND_PER_PRIMITIVE)    drawSecondaryColor(secondaryColorIndex++);
							if (fogCoordBinding==crGeometry::BIND_PER_PRIMITIVE)          drawFogCoord(fogCoordIndex++);
							if (handleVertexAttributes)
							{
								DrawVertexAttribList &list = drawVertexAttribMap[crGeometry::BIND_PER_PRIMITIVE];

								for( unsigned int i = 0; i < list.size(); ++i )
								{
									list[i]->applyAndIncrement();
								}
							}  
						}

						unsigned int vindex=*primItr;

						if (m_normalData.binding==crGeometry::BIND_PER_VERTEX)           drawNormal(vindex);
						if (m_colorData.binding==crGeometry::BIND_PER_VERTEX)            drawColor(vindex);
						if (secondaryColorBinding==crGeometry::BIND_PER_VERTEX)    drawSecondaryColor(vindex);
						if (fogCoordBinding==crGeometry::BIND_PER_VERTEX)          drawFogCoord(vindex);
						if ( extensions->isVertexProgramSupported() )
						{
							DrawVertexAttribList &list = drawVertexAttribMap[crGeometry::BIND_PER_VERTEX];

							for( unsigned int i = 0; i < list.size(); ++i )
							{
								list[i]->applyAndIncrement();
							}
						}  

						for(DrawTexCoordList::iterator texItr=drawTexCoordList.begin();
							texItr!=drawTexCoordList.end();
							++texItr)
						{
							(*(*texItr))(vindex);
						}
						if (drawTextCoord.valid()) (*drawTextCoord)(vindex);

						if( useVertexAttrib )
						{
							(*drawVertexAttribZero)(vindex);
						}
						else
						{
							drawVertex(vindex);
						}
					}

					glEnd();
					break;
				}
			case(crPrimitive::DrawElementsUShortPrimitiveType):
				{
					if (primLength==0) primLength=primitive->getNumIndices();

					const DrawElementsUShort* drawElements = static_cast<const DrawElementsUShort*>(primitive);
					glBegin(mode);

					unsigned int primCount=0;
					for(DrawElementsUShort::PT_IndexArray::const_iterator primItr=drawElements->getIndexArray().begin();
						primItr!=drawElements->getIndexArray().end();
						++primCount,++primItr)
					{

						if ((primCount%primLength)==0)
						{
							if (m_normalData.binding==crGeometry::BIND_PER_PRIMITIVE)           drawNormal(normalIndex++);
							if (m_colorData.binding==crGeometry::BIND_PER_PRIMITIVE)            drawColor(colorIndex++);
							if (secondaryColorBinding==crGeometry::BIND_PER_PRIMITIVE)    drawSecondaryColor(secondaryColorIndex++);
							if (fogCoordBinding==crGeometry::BIND_PER_PRIMITIVE)          drawFogCoord(fogCoordIndex++);
							if (handleVertexAttributes)
							{
								DrawVertexAttribList &list = drawVertexAttribMap[crGeometry::BIND_PER_PRIMITIVE];

								for( unsigned int i = 0; i < list.size(); ++i )
								{
									list[i]->applyAndIncrement();
								}
							}  
						}

						unsigned int vindex=*primItr;

						if (m_normalData.binding==crGeometry::BIND_PER_VERTEX)           drawNormal(vindex);
						if (m_colorData.binding==crGeometry::BIND_PER_VERTEX)            drawColor(vindex);
						if (secondaryColorBinding==crGeometry::BIND_PER_VERTEX)    drawSecondaryColor(vindex);
						if (fogCoordBinding==crGeometry::BIND_PER_VERTEX)          drawFogCoord(vindex);
						if (handleVertexAttributes)
						{
							DrawVertexAttribList &list = drawVertexAttribMap[crGeometry::BIND_PER_VERTEX];

							for( unsigned int i = 0; i < list.size(); ++i )
							{
								list[i]->applyAndIncrement();
							}
						}  

						for(DrawTexCoordList::iterator texItr=drawTexCoordList.begin();
							texItr!=drawTexCoordList.end();
							++texItr)
						{
							(*(*texItr))(vindex);
						}
						if (drawTextCoord.valid()) (*drawTextCoord)(vindex);

						if( useVertexAttrib )
						{
							(*drawVertexAttribZero)(vindex);
						}
						else
						{
							drawVertex(vindex);
						}
					}

					glEnd();
					break;
				}
			case(crPrimitive::DrawElementsUIntPrimitiveType):
				{
					if (primLength==0) primLength=primitive->getNumIndices();

					const DrawElementsUInt* drawElements = static_cast<const DrawElementsUInt*>(primitive);
					glBegin(mode);

					unsigned int primCount=0;
					for(DrawElementsUInt::PT_IndexArray::const_iterator primItr=drawElements->getIndexArray().begin();
						primItr!=drawElements->getIndexArray().end();
						++primCount,++primItr)
					{

						if ((primCount%primLength)==0)
						{
							if (m_normalData.binding==crGeometry::BIND_PER_PRIMITIVE)           drawNormal(normalIndex++);
							if (m_colorData.binding==crGeometry::BIND_PER_PRIMITIVE)            drawColor(colorIndex++);
							if (secondaryColorBinding==crGeometry::BIND_PER_PRIMITIVE)    drawSecondaryColor(secondaryColorIndex++);
							if (fogCoordBinding==crGeometry::BIND_PER_PRIMITIVE)          drawFogCoord(fogCoordIndex++);
							if ( extensions->isVertexProgramSupported() )
							{
								DrawVertexAttribList &list = drawVertexAttribMap[crGeometry::BIND_PER_PRIMITIVE];

								for( unsigned int i = 0; i < list.size(); ++i )
								{
									list[i]->applyAndIncrement();
								}
							}  
						}

						unsigned int vindex=*primItr;

						if (m_normalData.binding==crGeometry::BIND_PER_VERTEX)           drawNormal(vindex);
						if (m_colorData.binding==crGeometry::BIND_PER_VERTEX)            drawColor(vindex);
						if (secondaryColorBinding==crGeometry::BIND_PER_VERTEX)    drawSecondaryColor(vindex);
						if (fogCoordBinding==crGeometry::BIND_PER_VERTEX)          drawFogCoord(vindex);
						if ( extensions->isVertexProgramSupported() )
						{
							DrawVertexAttribList &list = drawVertexAttribMap[crGeometry::BIND_PER_VERTEX];

							for( unsigned int i = 0; i < list.size(); ++i )
							{
								list[i]->applyAndIncrement();
							}
						}  

						for(DrawTexCoordList::iterator texItr=drawTexCoordList.begin();
							texItr!=drawTexCoordList.end();
							++texItr)
						{
							(*(*texItr))(vindex);
						}
						if (drawTextCoord.valid()) (*drawTextCoord)(vindex);

						if( useVertexAttrib )
						{
							(*drawVertexAttribZero)(vindex);
						}
						else
						{
							drawVertex(vindex);
						}
					}

					glEnd();
					break;
				}
			default:
				{
					break;
				}
			}
		}
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crGeometry OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}
void crGeometry::compileImplementation(CRCore::crState& state ) const
{
	if (m_internalOptimizedGeometry.valid())
	{
		m_internalOptimizedGeometry->compileImplementation(state);
		return;
	}

	const Extensions* extensions = getExtensions(state.getContextID(),true);

	//const crGeometry::ArrayData &m_vertexData = m_pgeometry->getVertexData();
	//const crGeometry::ArrayData &m_colorData = m_pgeometry->getColorData();
	//const crGeometry::ArrayData &m_secondaryColorData = m_pgeometry->getSecondaryColorData();
	//const crGeometry::ArrayData &m_fogCoordData = m_pgeometry->getFogCoordData();
	//const crGeometry::Vec3ArrayData &m_normalData = m_pgeometry->getNormalData();

	//const crGeometry::ArrayList &m_vertexAttribList = m_pgeometry->getVertexAttribArrayList();
	//   const crGeometry::ArrayList& m_texCoordList = m_pgeometry->getTexCoordArrayList();
	//const crGeometry::PrimitiveList& m_primitives = m_pgeometry->getPrimitiveList();

	if( !( ( m_vertexData.array.valid() && m_vertexData.array->getNumElements() != 0 ) ||
		( m_vertexAttribList.size() > 0 && 
		m_vertexAttribList[0].array.valid() && 
		m_vertexAttribList[0].array->getNumElements() != 0 ) ) )
	{
		return;
	}

	if( ( m_vertexData.indices.valid() && m_vertexData.indices->getNumElements() == 0 ) ||
		( m_vertexAttribList.size() > 0 && 
		m_vertexAttribList[0].indices.valid() && 
		m_vertexAttribList[0].indices->getNumElements() == 0 ) )
	{
		return;
	}

	//DrawNormal         drawNormal(m_normalData.array.get(),m_normalData.indices.get());
	//DrawColor          drawColor(m_colorData.array.get(),m_colorData.indices.get());
	//DrawSecondaryColor drawSecondaryColor(m_secondaryColorData.array.get(),m_secondaryColorData.indices.get(),extensions);

	//DrawFogCoord       drawFogCoord(m_fogCoordData.array.get(),m_fogCoordData.indices.get(),extensions);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Set up secondary color if required.
	//
	crGeometry::AttributeBinding secondaryColorBinding = m_secondaryColorData.binding;
	if (secondaryColorBinding!=crGeometry::BIND_OFF && !extensions->isSecondaryColorSupported())
	{
		// switch off if not supported or have a valid data.
		secondaryColorBinding = crGeometry::BIND_OFF;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Set up fog coord if required.
	//
	crGeometry::AttributeBinding fogCoordBinding = m_fogCoordData.binding;
	if (fogCoordBinding!=crGeometry::BIND_OFF && !extensions->isFogCoordSupported())
	{
		// switch off if not supported or have a valid data.
		fogCoordBinding = crGeometry::BIND_OFF;
	}

	unsigned int normalIndex = 0;
	unsigned int colorIndex = 0;
	unsigned int secondaryColorIndex = 0;
	unsigned int fogCoordIndex = 0;

#define USE_DEFAULT_NORMAL 1
#if USE_DEFAULT_NORMAL
	// if no values are defined for normal and color provide some defaults...
	if (m_normalData.binding==crGeometry::BIND_OFF) glNormal3f(0.0f,0.0f,1.0f);
#endif

	//#define USE_DEFAULT_COLOUR 1
	//#if USE_DEFAULT_COLOUR
	if (m_colorData.binding==crGeometry::BIND_OFF) 
		state.applyColor(m_color);//glColor4fv(m_pgeometry->getColor().ptr());
	else
		state.colorDirty();
	//#ifdef _DEBUG
	//	else
	//	    CRCore::notify(WARN)<<"crGeometry::drawImplementation m_colorData.binding = "<<m_colorData.binding<< std::endl;
	//#endif
	//#endif

	typedef std::vector< ref_ptr<DrawVertexAttrib> > DrawVertexAttribList;
	typedef std::map< crGeometry::AttributeBinding, DrawVertexAttribList> DrawVertexAttribMap;
	DrawVertexAttribMap drawVertexAttribMap;

	bool vertexVertexAttributesSupported = extensions->isVertexProgramSupported();
	bool handleVertexAttributes = (!m_vertexAttribList.empty() && vertexVertexAttributesSupported);

	bool usingVertexBufferObjects = m_useVertexBufferObjects && state.isVertexBufferObjectSupported();
	//bool usingIndexBufferObjects = m_useIndexBufferObjects;

	if (m_fastPath && m_fastPathHint)
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// fast path.        
		//
		if (usingVertexBufferObjects)
		{
			//
			// Vertex Buffer Object path for defining vertex arrays.
			// 

			unsigned int & buffer = m_vboList[state.getContextID()];
			if (!buffer)
			{

				//CRCore::notify(ALWAYS) << "creating VertexBuffer "<<buffer<<std::endl;

				extensions->glGenBuffers(1, &buffer);
				extensions->glBindBuffer(GL_ARRAY_BUFFER_ARB,buffer);

				//CRCore::notify(ALWAYS)<< "  gen VertexBuffer "<<buffer<< std::endl;
				// compute total size and offsets required.
				unsigned int totalSize = 0;

				m_vertexData.offset = 0;
				if (m_vertexData.array.valid()) totalSize += m_vertexData.array->getTotalDataSize();

				m_normalData.offset = totalSize; 
				if (m_normalData.array.valid()) totalSize += m_normalData.array->getTotalDataSize();

				m_colorData.offset = totalSize;
				if (m_colorData.array.valid()) totalSize += m_colorData.array->getTotalDataSize();

				m_secondaryColorData.offset = totalSize;
				if (m_secondaryColorData.array.valid()) totalSize += m_secondaryColorData.array->getTotalDataSize();

				m_fogCoordData.offset = totalSize;
				if (m_fogCoordData.array.valid()) totalSize += m_fogCoordData.array->getTotalDataSize();

				unsigned int unit;
				for(unit=0;unit<m_texCoordList.size();++unit)
				{
					m_texCoordList[unit].offset = totalSize;
					const Array* array = m_texCoordList[unit].array.get();
					if (array)
						totalSize += array->getTotalDataSize();

				}

				if( handleVertexAttributes )
				{
					unsigned int index;
					for( index = 0; index < m_vertexAttribList.size(); ++index )
					{
						m_vertexAttribList[index].offset = totalSize;
						const Array* array = m_vertexAttribList[index].array.get();
						const crGeometry::AttributeBinding ab = m_vertexAttribList[index].binding;
						if( ab == crGeometry::BIND_PER_VERTEX && array )
						{
							totalSize += array->getTotalDataSize();           
						}
					}
				}

				// allocated the buffer space, but leave the copy to be done per vertex array below
				extensions->glBufferData(GL_ARRAY_BUFFER_ARB,totalSize, 0, /*GL_DYNAMIC_DRAW_ARB*/GL_STATIC_DRAW_ARB);

				//CRCore::notify(ALWAYS)<< "   Created VertexBuffer"<<buffer<<" size="<<totalSize<<std::endl;

				//
				// copy the data
				//
				if( m_vertexData.array.valid() )
					extensions->glBufferSubData(GL_ARRAY_BUFFER_ARB, m_vertexData.offset, m_vertexData.array->getTotalDataSize(),m_vertexData.array->getDataPointer());

				if (m_normalData.binding==crGeometry::BIND_PER_VERTEX)
					extensions->glBufferSubData(GL_ARRAY_BUFFER_ARB, m_normalData.offset, m_normalData.array->getTotalDataSize(),m_normalData.array->getDataPointer());

				if (m_colorData.binding==crGeometry::BIND_PER_VERTEX)
					extensions->glBufferSubData(GL_ARRAY_BUFFER_ARB, m_colorData.offset, m_colorData.array->getTotalDataSize(),m_colorData.array->getDataPointer());

				if (secondaryColorBinding==crGeometry::BIND_PER_VERTEX)
					extensions->glBufferSubData(GL_ARRAY_BUFFER_ARB, m_secondaryColorData.offset, m_secondaryColorData.array->getTotalDataSize(),m_secondaryColorData.array->getDataPointer());

				if (fogCoordBinding==crGeometry::BIND_PER_VERTEX)
					extensions->glBufferSubData(GL_ARRAY_BUFFER_ARB, m_fogCoordData.offset, m_fogCoordData.array->getTotalDataSize(),m_fogCoordData.array->getDataPointer());

				for(unit=0;unit<m_texCoordList.size();++unit)
				{
					const Array* array = m_texCoordList[unit].array.get();
					if (array)
						extensions->glBufferSubData(GL_ARRAY_BUFFER_ARB, m_texCoordList[unit].offset, array->getTotalDataSize(), array->getDataPointer());
				}

				if( handleVertexAttributes )
				{
					unsigned int index;
					for( index = 0; index < m_vertexAttribList.size(); ++index )
					{
						const Array* array = m_vertexAttribList[index].array.get();
						const crGeometry::AttributeBinding ab = m_vertexAttribList[index].binding;

						if( ab == crGeometry::BIND_PER_VERTEX && array )
						{
							extensions->glBufferSubData(GL_ARRAY_BUFFER_ARB, m_vertexAttribList[index].offset, array->getTotalDataSize(), array->getDataPointer());
						}
					}
				}

			}

			//std::cout << "binding VertexBuffer "<<buffer<<std::endl;

			extensions->glBindBuffer(GL_ARRAY_BUFFER_ARB,buffer);

			if( m_vertexData.array.valid() )
				state.setVertexPointer(m_vertexData.array->getDataSize(),m_vertexData.array->getDataType(),0,(const void*)m_vertexData.offset);
			else
				state.disableVertexPointer();

			if (m_normalData.binding==crGeometry::BIND_PER_VERTEX)
				state.setNormalPointer(GL_FLOAT,0,(const void*)m_normalData.offset);
			else
				state.disableNormalPointer();


			if (m_colorData.binding==crGeometry::BIND_PER_VERTEX)
				state.setColorPointer(m_colorData.array->getDataSize(),m_colorData.array->getDataType(),0,(const void*)m_colorData.offset);
			else
				state.disableColorPointer();

			if (secondaryColorBinding==crGeometry::BIND_PER_VERTEX)
				state.setSecondaryColorPointer(m_secondaryColorData.array->getDataSize(),m_secondaryColorData.array->getDataType(),0,(const void*)m_secondaryColorData.offset);
			else
				state.disableSecondaryColorPointer();

			if (fogCoordBinding==crGeometry::BIND_PER_VERTEX)
				state.setFogCoordPointer(GL_FLOAT,0,(const void*)m_fogCoordData.offset);
			else
				state.disableFogCoordPointer();

			unsigned int unit;
			for(unit=0;unit<m_texCoordList.size();++unit)
			{
				const Array* array = m_texCoordList[unit].array.get();
				if (array)
					state.setTexCoordPointer(unit,array->getDataSize(),array->getDataType(),0,(const void*)m_texCoordList[unit].offset);
				else
					state.disableTexCoordPointer(unit);
			}

			state.disableTexCoordPointersAboveAndIncluding(unit);

			if( handleVertexAttributes )
			{
				unsigned int index;
				for( index = 0; index < m_vertexAttribList.size(); ++index )
				{
					const Array* array = m_vertexAttribList[index].array.get();
					const crGeometry::AttributeBinding ab = m_vertexAttribList[index].binding;

					if( ab == crGeometry::BIND_PER_VERTEX && array )
					{
						state.setVertexAttribPointer( index, array->getDataSize(), array->getDataType(), 
							m_vertexAttribList[index].normalize, 0, (const void*)m_vertexAttribList[index].offset );
					}
					else
					{
						if( array )
						{
							const IndexArray* indexArray = m_vertexAttribList[index].indices.get();

							if( indexArray && indexArray->getNumElements() > 0 )
							{
								drawVertexAttribMap[ab].push_back( 
									new DrawVertexAttrib(extensions,index,m_vertexAttribList[index].normalize,array,indexArray) );
							}
							else
							{
								drawVertexAttribMap[ab].push_back( 
									new DrawVertexAttrib(extensions,index,m_vertexAttribList[index].normalize,array,0) );
							}
						}

						state.disableVertexAttribPointer( index );
					}
				}
				state.disableVertexAttribPointersAboveAndIncluding( index );

			}
			else if (vertexVertexAttributesSupported)
			{
				state.disableVertexAttribPointersAboveAndIncluding( 0 );
			}
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//
			// draw the m_primitives themselves.
			//
			for(crGeometry::PrimitiveList::const_iterator itr=m_primitives.begin();
				itr!=m_primitives.end();
				++itr)
			{
				(*itr)->compile(state, usingVertexBufferObjects/*m_useIndexBufferObjects*/);
			}
			extensions->glBindBuffer(GL_ARRAY_BUFFER_ARB,0);
		}
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crGeometry OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}
///////////////////////////////////////////////////////////////////////////////
CRCore::crObject* CRCore::createObjectForImage(CRCore::crImage* image)
{
	return createObjectForImage(image,image->s(),image->t());
}

CRCore::crObject* CRCore::createObjectForImage(CRCore::crImage* image,float s,float t)
{
	if (image)
	{
		if (s>0 && t>0)
		{

			float y = 1.0;
			float x = y*(s/t);

			// set up the texture.
			CRCore::crTexture2D* texture = new CRCore::crTexture2D;
			//texture->setDrawCallback(crOglTexture2DDraw::getInstance());
			texture->setImage(image);

			// set up the drawstate.
			CRCore::crStateSet* dstate = new CRCore::crStateSet;
			dstate->setMode(GL_CULL_FACE,CRCore::crStateAttribute::OFF);
			dstate->setMode(GL_LIGHTING,CRCore::crStateAttribute::OFF);
			dstate->setTextureAttributeAndModes(0, texture,CRCore::crStateAttribute::ON);

			// set up the geoset.
			crGeometry* geom = new crGeometry;
			//geom->setDrawCallback(crOglGeometryDraw::getInstance());
			geom->setStateSet(dstate);

			Vec3Array* coords = new Vec3Array(4);
			(*coords)[0].set(-x,0.0f,y);
			(*coords)[1].set(-x,0.0f,-y);
			(*coords)[2].set(x,0.0f,-y);
			(*coords)[3].set(x,0.0f,y);
			geom->setVertexArray(coords);

			Vec2Array* tcoords = new Vec2Array(4);
			(*tcoords)[0].set(0.0f,1.0f);
			(*tcoords)[1].set(0.0f,0.0f);
			(*tcoords)[2].set(1.0f,0.0f);
			(*tcoords)[3].set(1.0f,1.0f);
			geom->setTexCoordArray(0,tcoords);

			//CRCore::Vec4Array* colours = new CRCore::Vec4Array(1);
			//(*colours)[0].set(1.0f,1.0f,1.0,1.0f);
			//geom->setColorArray(colours);
			//geom->setColorBinding(crGeometry::BIND_OVERALL);

			DrawArrays *drawArrays = new DrawArrays(crPrimitive::PT_QUADS,0,4);
			//drawArrays->setDrawCallback(crOglDrawArrays::getInstance());
			geom->addPrimitive(drawArrays);

			// set up the geode.
			CRCore::crObject* geode = new CRCore::crObject;
			geode->addDrawable(geom);

			return geode;

		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}
