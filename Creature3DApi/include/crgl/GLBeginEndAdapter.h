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
#ifndef CRCORE_GLBeginEndAdapter
#define CRCORE_GLBeginEndAdapter 1

#include <CRCore/ref_ptr.h>
#include <CRCore/crArray.h>
#include <CRCore/crMatrixd.h>
#include <list>

#ifndef GL_TEXTURE0
#define GL_TEXTURE0 0x84C0
#endif

namespace CRCore {

// forward declare
class crState;

/** A class adapting OpenGL 1.0 glBegin()/glEnd() style code to vertex array based code */
class CR_EXPORT GLBeginEndAdapter
{
    public:

        GLBeginEndAdapter(crState* state=0);

        void setState(crState* state) { m_state = state; }
        crState* getState() { return m_state; }
        const crState* getState() const { return m_state; }

        enum MatrixMode
        {
            APPLY_LOCAL_MATRICES_TO_VERTICES,
            APPLY_LOCAL_MATRICES_TO_MODELVIEW
        };

        void setMatrixMode(MatrixMode mode) { m_mode = mode; }
        MatrixMode setMatrixMode() const { return m_mode; }

        void PushMatrix();
        void PopMatrix();

        void LoadIdentity();
        void LoadMatrixd(const GLdouble* m);
        void MultMatrixd(const GLdouble* m);

        void Translatef(GLfloat x, GLfloat y, GLfloat z) { Translated(x,y,z); }
        void Scalef(GLfloat x, GLfloat y, GLfloat z) { Scaled(x,y,z); }
        void Rotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) { Rotated(angle,x,y,z); }

        void Translated(GLdouble x, GLdouble y, GLdouble z);
        void Scaled(GLdouble x, GLdouble y, GLdouble z);
        void Rotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);

        void Vertex3f(GLfloat x, GLfloat y, GLfloat z);
        void Vertex3fv(const GLfloat* v) { Vertex3f(v[0], v[1], v[2]); }

        void Color4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
        {
            m_colorAssigned = true;
            m_color.set(red,green,blue,alpha);
        }

        void Color4fv(const GLfloat* c) { Color4f(c[0], c[1], c[2], c[3]); }
        void Color4ubv(const GLubyte* c) { const float div = 1.0f/255.0f; Color4f(float(c[0])*div, float(c[1])*div, float(c[2])*div, float(c[3])*div); }

        void Normal3f(GLfloat x, GLfloat y, GLfloat z)
        {
            m_normalAssigned = true;
            m_normal.set(x,y,z);
        }

        void Normal3fv(const GLfloat* n) { Normal3f(n[0], n[1], n[2]); }

        void TexCoord1f(GLfloat x) { MultiTexCoord4f(GL_TEXTURE0, x, 0.0f, 0.0f, 1.0f); }
        void TexCoord1fv(const GLfloat* tc)  { MultiTexCoord4f(GL_TEXTURE0, tc[0], 0.0f, 0.0f, 1.0f); }

        void TexCoord2f(GLfloat x, GLfloat y) { MultiTexCoord4f(GL_TEXTURE0, x, y, 0.0f, 1.0f); }
        void TexCoord2fv(const GLfloat* tc) { MultiTexCoord4f(GL_TEXTURE0, tc[0], tc[1], 0.0f, 1.0f); }

        void TexCoord3f(GLfloat x, GLfloat y, GLfloat z) { MultiTexCoord4f(GL_TEXTURE0, x, y, z, 1.0f); }
        void TexCoord3fv(const GLfloat* tc) { MultiTexCoord4f(GL_TEXTURE0, tc[0], tc[1], tc[2], 1.0f); }

        void TexCoord4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w) { MultiTexCoord4f(GL_TEXTURE0, x, y, z, w); }
        void TexCoord4fv(const GLfloat* tc) { MultiTexCoord4f(GL_TEXTURE0, tc[0], tc[1], tc[2], tc[3]); }

        void MultiTexCoord1f(GLenum target, GLfloat x)  { MultiTexCoord4f(target, x, 0.0f, 0.0f, 1.0f); }
        void MultiTexCoord1fv(GLenum target, const GLfloat* tc) { MultiTexCoord4f(target, tc[0], 0.0f, 0.0f, 1.0f); }

        void MultiTexCoord2f(GLenum target, GLfloat x, GLfloat y) { MultiTexCoord4f(target, x, y, 0.0f, 1.0f); }
        void MultiTexCoord2fv(GLenum target, const GLfloat* tc) { MultiTexCoord4f(target, tc[0],tc[1], 0.0f, 1.0f); }

        void MultiTexCoord3f(GLenum target, GLfloat x, GLfloat y, GLfloat z) {MultiTexCoord4f(target, x, y, z, 1.0f); }
        void MultiTexCoord3fv(GLenum target, const GLfloat* tc) { MultiTexCoord4f(target, tc[0], tc[1], tc[2], 1.0f); }

        void MultiTexCoord4f(GLenum target, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
        void MultiTexCoord4fv(GLenum target, const GLfloat* tc) { MultiTexCoord4f(target, tc[0], tc[1], tc[2], tc[3]); }

        void VertexAttrib1f(GLuint unit, GLfloat x)  { VertexAttrib4f(unit, x, 0.0f, 0.0f, 0.0f); }
        void VertexAttrib1fv(GLuint unit, const GLfloat* tc) { VertexAttrib4f(unit, tc[0], 0.0f, 0.0f, 0.0f); }

        void VertexAttrib2f(GLuint unit, GLfloat x, GLfloat y) { VertexAttrib4f(unit, x, y, 0.0f, 0.0f); }
        void VertexAttrib2fv(GLuint unit, const GLfloat* tc) { VertexAttrib4f(unit, tc[0],tc[1], 0.0f, 0.0f); }

        void VertexAttrib3f(GLuint unit, GLfloat x, GLfloat y, GLfloat z) {VertexAttrib4f(unit, x, y, z, 0.0f); }
        void VertexAttrib3fv(GLuint unit, const GLfloat* tc) { VertexAttrib4f(unit, tc[0], tc[1], tc[2], 0.0f); }

        void VertexAttrib4f(GLuint unit, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
        void VertexAttrib4fv(GLuint  unit, const GLfloat* tc) { VertexAttrib4f(unit, tc[0], tc[1], tc[2], tc[3]); }

        void Begin(GLenum mode);
        void End();

    protected:
		typedef std::vector<GLushort> IndicesGLushort;
		IndicesGLushort m_quadIndicesGLushort[4];

		typedef std::vector<GLuint> IndicesGLuint;
		IndicesGLuint m_quadIndicesGLuint[4];

		void drawQuads(GLint first, GLsizei count, GLsizei primCount=0);

		inline void glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei primcount)
		{
			if (primcount>=1 && m_glDrawArraysInstanced!=0) m_glDrawArraysInstanced(mode, first, count, primcount);
			else glDrawArrays(mode, first, count);
		}

		inline void glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount )
		{
			if (primcount>=1 && m_glDrawElementsInstanced!=0) m_glDrawElementsInstanced(mode, count, type, indices, primcount);
			else glDrawElements(mode, count, type, indices);
		}

		inline void Vertex(float x, float y, float z, float w=1.0f)
		{
			glVertex4f(x,y,z,w);
		}
		inline void Color(float r, float g, float b, float a=1.0f)
		{
			glColor4f(r,g,b,a);
		}
		inline void Normal(float x, float y, float z)
		{
			glNormal3f(x,y,z);
		}
		inline void TexCoord(float x, float y=0.0f, float z=0.0f, float w=1.0f)
		{
			glTexCoord4f(x,y,z,w);
		}

		void MultiTexCoord(unsigned int unit, float x, float y=0.0f, float z=0.0f, float w=1.0f)
		{
			m_glMultiTexCoord4f(GL_TEXTURE0+unit,x,y,z,w);
		}

		void VerteAttrib(unsigned int location, float x, float y=0.0f, float z=0.0f, float w=0.0f)
		{
			m_glVertexAttrib4f( location, x,y,z,w);
		}

		void initializeExtensionProcs();
		typedef void (APIENTRY * ActiveTextureProc) (GLenum texture);
		typedef void (APIENTRY * FogCoordPointerProc) (GLenum type, GLsizei stride, const GLvoid *pointer);
		typedef void (APIENTRY * SecondaryColorPointerProc) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
		typedef void (APIENTRY * MultiTexCoord4fProc) (GLenum target, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
		typedef void (APIENTRY * VertexAttrib4fProc)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
		typedef void (APIENTRY * VertexAttrib4fvProc)(GLuint index, const GLfloat *v);
		typedef void (APIENTRY * VertexAttribPointerProc) (unsigned int, GLint, GLenum, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
		typedef void (APIENTRY * EnableVertexAttribProc) (unsigned int);
		typedef void (APIENTRY * DisableVertexAttribProc) (unsigned int);
		typedef void (APIENTRY * BindBufferProc) (GLenum target, GLuint buffer);

		typedef void (APIENTRY * DrawArraysInstancedProc)( GLenum mode, GLint first, GLsizei count, GLsizei primcount );
		typedef void (APIENTRY * DrawElementsInstancedProc)( GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount );

		bool                        m_extensionProcsInitialized;
		GLint                       m_glMaxTextureCoords;
		GLint                       m_glMaxTextureUnits;
		ActiveTextureProc           m_glClientActiveTexture;
		ActiveTextureProc           m_glActiveTexture;
		MultiTexCoord4fProc         m_glMultiTexCoord4f;
		VertexAttrib4fProc          m_glVertexAttrib4f;
		VertexAttrib4fvProc         m_glVertexAttrib4fv;
		FogCoordPointerProc         m_glFogCoordPointer;
		SecondaryColorPointerProc   m_glSecondaryColorPointer;
		VertexAttribPointerProc     m_glVertexAttribPointer;
		EnableVertexAttribProc      m_glEnableVertexAttribArray;
		DisableVertexAttribProc     m_glDisableVertexAttribArray;
		BindBufferProc              m_glBindBuffer;
		DrawArraysInstancedProc     m_glDrawArraysInstanced;
		DrawElementsInstancedProc   m_glDrawElementsInstanced;

        crState*                              m_state;

        MatrixMode                          m_mode;

        typedef std::list<crMatrixd>           MatrixStack;
        MatrixStack                         m_matrixStack;

        bool                                m_normalAssigned;
        CRCore::crVector3                          m_normal;

        bool                                m_colorAssigned;
        CRCore::crVector4                          m_color;

        CRCore::crVector3                          m_overallNormal;
        CRCore::crVector4                          m_overallColor;

        typedef std::vector<bool>           AssignedList;
        typedef std::vector<CRCore::crVector4>     VertexList;

        AssignedList                        m_texCoordAssignedList;
        VertexList                          m_texCoordList;

        AssignedList                        m_vertexAttribAssignedList;
        VertexList                          m_vertexAttribList;


        typedef std::vector< CRCore::ref_ptr<Vec4Array> > VertexArrayList;

        GLenum                              m_primitiveMode;
        CRCore::ref_ptr<CRCore::Vec3Array>        m_vertices;
        CRCore::ref_ptr<CRCore::Vec3Array>        m_normals;
        CRCore::ref_ptr<CRCore::Vec4Array>        m_colors;
        VertexArrayList                     m_texCoordsList;
        VertexArrayList                     m_vertexAttribsList;
};

}

#endif
