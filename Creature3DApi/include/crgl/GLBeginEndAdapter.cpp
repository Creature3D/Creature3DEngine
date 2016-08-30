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
#include <crgl/GLBeginEndAdapter.h>
#include <CRCore/crState.h>

#include <CRCore/crNotify.h>
#include <CRCore/io_utils.h>

using namespace CRCore;

GLBeginEndAdapter::GLBeginEndAdapter(crState* state):
    m_state(state),
    m_mode(APPLY_LOCAL_MATRICES_TO_VERTICES),
    m_normalAssigned(false),
    m_normal(0.0f,0.0f,1.0f),
    m_colorAssigned(false),
    m_color(1.0f,1.0f,1.0f,1.0f),
    m_primitiveMode(0)
{
}

void GLBeginEndAdapter::PushMatrix()
{
    if (m_matrixStack.empty())
    {
        if (m_mode==APPLY_LOCAL_MATRICES_TO_VERTICES) m_matrixStack.push_back(crMatrixd());
        else m_matrixStack.push_back(m_state->getModelViewMatrix());
    }
    else m_matrixStack.push_back(m_matrixStack.back());
}

void GLBeginEndAdapter::PopMatrix()
{
    if (!m_matrixStack.empty()) m_matrixStack.pop_back();
}


void GLBeginEndAdapter::LoadIdentity()
{
    if (m_matrixStack.empty()) m_matrixStack.push_back(crMatrixd::identity());
    else m_matrixStack.back().makeIdentity();
}

void GLBeginEndAdapter::LoadMatrixd(const GLdouble* m)
{
    if (m_matrixStack.empty()) m_matrixStack.push_back(crMatrixd(m));
    else m_matrixStack.back().set(m);
}

void GLBeginEndAdapter::MultMatrixd(const GLdouble* m)
{
    if (m_matrixStack.empty())
    {
        if (m_mode==APPLY_LOCAL_MATRICES_TO_VERTICES) m_matrixStack.push_back(crMatrixd());
        else m_matrixStack.push_back(m_state->getModelViewMatrix());
    }
    m_matrixStack.back().preMult(crMatrixd(m));
}


void GLBeginEndAdapter::Translated(GLdouble x, GLdouble y, GLdouble z)
{
    if (m_matrixStack.empty())
    {
        if (m_mode==APPLY_LOCAL_MATRICES_TO_VERTICES) m_matrixStack.push_back(crMatrixd());
        else m_matrixStack.push_back(m_state->getModelViewMatrix());
    }
    m_matrixStack.back().preMultTranslate(crVector3d(x,y,z));
}

void GLBeginEndAdapter::Scaled(GLdouble x, GLdouble y, GLdouble z)
{
    if (m_matrixStack.empty())
    {
        if (m_mode==APPLY_LOCAL_MATRICES_TO_VERTICES) m_matrixStack.push_back(crMatrixd());
        else m_matrixStack.push_back(m_state->getModelViewMatrix());
    }
    m_matrixStack.back().preMultScale(crVector3d(x,y,z));
}

void GLBeginEndAdapter::Rotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
    if (m_matrixStack.empty())
    {
        if (m_mode==APPLY_LOCAL_MATRICES_TO_VERTICES) m_matrixStack.push_back(crMatrixd());
        else m_matrixStack.push_back(m_state->getModelViewMatrix());
    }
    m_matrixStack.back().preMultRotate(crQuat(DegreesToRadians(angle), crVector3d(x,y,z)));
}

void GLBeginEndAdapter::MultiTexCoord4f(GLenum target, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    unsigned int unit = static_cast<unsigned int>(target-GL_TEXTURE0);

    if (unit>=m_texCoordAssignedList.size()) m_texCoordAssignedList.resize(unit+1, false);
    if (unit>=m_texCoordList.size()) m_texCoordList.resize(unit+1, CRCore::crVector4(0.0f,0.0f,0.0f,0.0f));

    m_texCoordAssignedList[unit] = true;
    m_texCoordList[unit].set(x,y,z,w);
}

void GLBeginEndAdapter::VertexAttrib4f(GLuint unit, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    if (unit>=m_vertexAttribAssignedList.size()) m_vertexAttribAssignedList.resize(unit+1, false);
    if (unit>=m_vertexAttribList.size()) m_vertexAttribList.resize(unit+1, CRCore::crVector4(0.0f,0.0f,0.0f,0.0f));

    m_vertexAttribAssignedList[unit] = true;
    m_vertexAttribList[unit].set(x,y,z,w);
}

void GLBeginEndAdapter::Vertex3f(GLfloat x, GLfloat y, GLfloat z)
{
    CRCore::crVector3 vertex(x,y,z);

    if (!m_vertices) m_vertices = new CRCore::Vec3Array;

    if (m_normalAssigned)
    {
        if (!m_normals) m_normals = new CRCore::Vec3Array;
        if (m_normals->size()<m_vertices->size()) m_normals->resize(m_vertices->size(), m_overallNormal);

        m_normals->push_back(m_normal);
    }

    if (m_colorAssigned)
    {
        if (!m_colors) m_colors = new CRCore::Vec4Array;
        if (m_colors->size()<m_vertices->size()) m_colors->resize(m_vertices->size(), m_overallColor);

        m_colors->push_back(m_color);
    }

    if (!m_texCoordAssignedList.empty())
    {
        for(unsigned int unit=0; unit<m_texCoordAssignedList.size(); ++unit)
        {
            if (m_texCoordAssignedList[unit])
            {
                if (unit>=m_texCoordsList.size()) m_texCoordsList.resize(unit+1);
                if (!m_texCoordsList[unit]) m_texCoordsList[unit] = new CRCore::Vec4Array;
                if (m_texCoordsList[unit]->size()<m_vertices->size()) m_texCoordsList[unit]->resize(m_vertices->size(), CRCore::crVector4(0.0,0.0f,0.0f,0.0f));

                m_texCoordsList[unit]->push_back(m_texCoordList[unit]);
            }
        }
    }


    if (!m_vertexAttribAssignedList.empty())
    {
        for(unsigned int unit=0; unit<m_vertexAttribAssignedList.size(); ++unit)
        {
            if (m_vertexAttribAssignedList[unit])
            {
                if (unit>=m_vertexAttribsList.size()) m_vertexAttribsList.resize(unit+1);
                if (!m_vertexAttribsList[unit]) m_vertexAttribsList[unit] = new CRCore::Vec4Array;
                if (m_vertexAttribsList[unit]->size()<m_vertices->size()) m_vertexAttribsList[unit]->resize(m_vertices->size(), CRCore::crVector4(0.0,0.0f,0.0f,0.0f));

                m_vertexAttribsList[unit]->push_back(m_vertexAttribList[unit]);
            }
        }
    }

    m_vertices->push_back(vertex);
}

void GLBeginEndAdapter::Begin(GLenum mode)
{
    m_overallNormal = m_normal;
    m_overallColor = m_color;

    // reset geometry
    m_primitiveMode = mode;
    if (m_vertices.valid()) m_vertices->clear();

    m_normalAssigned = false;
    if (m_normals.valid()) m_normals->clear();

    m_colorAssigned = false;
    if (m_colors.valid()) m_colors->clear();

    m_texCoordAssignedList.clear();
    m_texCoordList.clear();
    for(VertexArrayList::iterator itr = m_texCoordsList.begin();
        itr != m_texCoordsList.end();
        ++itr)
    {
        if (itr->valid()) (*itr)->clear();
    }

    m_vertexAttribAssignedList.clear();
    m_vertexAttribList.clear();
}

void GLBeginEndAdapter::End()
{
    if (!m_vertices || m_vertices->empty()) return;

    if (!m_matrixStack.empty())
    {
        const CRCore::crMatrixd& matrix = m_matrixStack.back();
        if (m_mode==APPLY_LOCAL_MATRICES_TO_VERTICES)
        {
            CRCore::crMatrix inverse;
            inverse.invert(matrix);

            for(Vec3Array::iterator itr = m_vertices->begin();
                itr != m_vertices->end();
                ++itr)
            {
                *itr = *itr * matrix;
            }

            if (m_normalAssigned && m_normals.valid())
            {
                for(Vec3Array::iterator itr = m_normals->begin();
                    itr != m_normals->end();
                    ++itr)
                {
                    *itr = CRCore::crMatrixd::transform3x3(inverse, *itr);
                    (*itr).normalize();
                }
            }
            else
            {
                m_overallNormal = CRCore::crMatrixd::transform3x3(inverse, m_overallNormal);
                m_overallNormal.normalize();
            }
        }
        else
        {
            m_state->applyModelViewMatrix(new RefMatrix(matrix));
        }
    }

    m_state->lazyDisablingOfVertexAttributes();

    if (m_colorAssigned)
    {
        m_state->setColorPointer(m_colors.get());
    }
    else
    {
        Color(m_overallColor.r(), m_overallColor.g(), m_overallColor.b(), m_overallColor.a());
    }

    if (m_normalAssigned)
    {
         m_state->setNormalPointer(m_normals.get());
    }
    else
    {
        Normal(m_overallNormal.x(), m_overallNormal.y(), m_overallNormal.z());
    }

    for(unsigned int unit=0; unit<m_texCoordAssignedList.size(); ++unit)
    {
        if (m_texCoordAssignedList[unit] && m_texCoordsList[unit].valid())
        {
            m_state->setTexCoordPointer(unit, m_texCoordsList[unit].get());
        }
    }


    for(unsigned int unit=0; unit<m_vertexAttribAssignedList.size(); ++unit)
    {
        if (m_vertexAttribAssignedList[unit] && m_vertexAttribsList[unit].valid())
        {
            m_state->setVertexAttribPointer(unit, m_vertexAttribsList[unit].get(), false);
        }
    }

    m_state->setVertexPointer(m_vertices.get());

    m_state->applyDisablingOfVertexAttributes();

    if (m_primitiveMode==GL_QUADS) 
    {
        m_state->drawQuads(0, m_vertices->size());
    }
    else if (m_primitiveMode==GL_QUAD_STRIP)
    {
        // will the winding be wrong? Do we need to swap it?
        glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertices->size());
    }
    else if (m_primitiveMode==GL_POLYGON) glDrawArrays(GL_TRIANGLE_FAN, 0, m_vertices->size());
    else glDrawArrays(m_primitiveMode, 0, m_vertices->size());
}
void GLBeginEndAdapter::initializeExtensionProcs()
{
	if (m_extensionProcsInitialized) return;

	setGLExtensionFuncPtr(m_glClientActiveTexture,"glClientActiveTexture","glClientActiveTextureARB");
	setGLExtensionFuncPtr(m_glActiveTexture, "glActiveTexture","glActiveTextureARB");
	setGLExtensionFuncPtr(m_glFogCoordPointer, "glFogCoordPointer","glFogCoordPointerEXT");
	setGLExtensionFuncPtr(m_glSecondaryColorPointer, "glSecondaryColorPointer","glSecondaryColorPointerEXT");
	setGLExtensionFuncPtr(m_glVertexAttribPointer, "glVertexAttribPointer","glVertexAttribPointerARB");
	setGLExtensionFuncPtr(m_glEnableVertexAttribArray, "glEnableVertexAttribArray","glEnableVertexAttribArrayARB");
	setGLExtensionFuncPtr(m_glMultiTexCoord4f, "glMultiTexCoord4f","glMultiTexCoord4fARB");
	setGLExtensionFuncPtr(m_glVertexAttrib4f, "glVertexAttrib4f");
	setGLExtensionFuncPtr(m_glVertexAttrib4fv, "glVertexAttrib4fv");
	setGLExtensionFuncPtr(m_glDisableVertexAttribArray, "glDisableVertexAttribArray","glDisableVertexAttribArrayARB");
	setGLExtensionFuncPtr(m_glBindBuffer, "glBindBuffer","glBindBufferARB");

	setGLExtensionFuncPtr(m_glDrawArraysInstanced, "glDrawArraysInstanced","glDrawArraysInstancedARB","glDrawArraysInstancedEXT");
	setGLExtensionFuncPtr(m_glDrawElementsInstanced, "glDrawElementsInstanced","glDrawElementsInstancedARB","glDrawElementsInstancedEXT");

	if ( osg::getGLVersionNumber() >= 2.0 || osg::isGLExtensionSupported(_contextID,"GL_ARB_vertex_shader") || OSG_GLES2_FEATURES)
	{
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&m_glMaxTextureUnits);
		glGetIntegerv(GL_MAX_TEXTURE_COORDS,&m_glMaxTextureCoords);
	}
	else if ( osg::getGLVersionNumber() >= 1.3 ||
		osg::isGLExtensionSupported(_contextID,"GL_ARB_multitexture") ||
		osg::isGLExtensionSupported(_contextID,"GL_EXT_multitexture") ||
		OSG_GLES1_FEATURES)
	{
		GLint maxTextureUnits = 0;
		glGetIntegerv(GL_MAX_TEXTURE_UNITS,&maxTextureUnits);
		m_glMaxTextureUnits = maxTextureUnits;
		m_glMaxTextureCoords = maxTextureUnits;
	}
	else
	{
		m_glMaxTextureUnits = 1;
		m_glMaxTextureCoords = 1;
	}

	m_extensionProcsInitialized = true;
}

void GLBeginEndAdapter::drawQuads(GLint first, GLsizei count, GLsizei primCount)
{
	// OSG_NOTICE<<"State::drawQuads("<<first<<", "<<count<<")"<<std::endl;

	unsigned int array = first % 4;
	unsigned int offsetFirst = ((first-array) / 4) * 6;
	unsigned int numQuads = (count/4);
	unsigned int numIndices = numQuads * 6;
	unsigned int endOfIndices = offsetFirst+numIndices;

	if (endOfIndices<65536)
	{
		IndicesGLushort& indices = m_quadIndicesGLushort[array];

		if (endOfIndices >= indices.size())
		{
			// we need to expand the _indexArray to be big enough to cope with all the quads required.
			unsigned int numExistingQuads = indices.size()/6;
			unsigned int numRequiredQuads = endOfIndices/6;
			indices.reserve(endOfIndices);
			for(unsigned int i=numExistingQuads; i<numRequiredQuads; ++i)
			{
				unsigned int base = i*4 + array;
				indices.push_back(base);
				indices.push_back(base+1);
				indices.push_back(base+3);

				indices.push_back(base+1);
				indices.push_back(base+2);
				indices.push_back(base+3);

				// OSG_NOTICE<<"   adding quad indices ("<<base<<")"<<std::endl;
			}
		}

		// if (array!=0) return;

		// OSG_NOTICE<<"  glDrawElements(GL_TRIANGLES, "<<numIndices<<", GL_UNSIGNED_SHORT, "<<&(indices[base])<<")"<<std::endl;
		glDrawElementsInstanced(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, &(indices[offsetFirst]), primCount);
	}
	else
	{
		IndicesGLuint& indices = m_quadIndicesGLuint[array];

		if (endOfIndices >= indices.size())
		{
			// we need to expand the _indexArray to be big enough to cope with all the quads required.
			unsigned int numExistingQuads = indices.size()/6;
			unsigned int numRequiredQuads = endOfIndices/6;
			indices.reserve(endOfIndices);
			for(unsigned int i=numExistingQuads; i<numRequiredQuads; ++i)
			{
				unsigned int base = i*4 + array;
				indices.push_back(base);
				indices.push_back(base+1);
				indices.push_back(base+3);

				indices.push_back(base+1);
				indices.push_back(base+2);
				indices.push_back(base+3);

				// OSG_NOTICE<<"   adding quad indices ("<<base<<")"<<std::endl;
			}
		}

		// if (array!=0) return;

		// OSG_NOTICE<<"  glDrawElements(GL_TRIANGLES, "<<numIndices<<", GL_UNSIGNED_SHORT, "<<&(indices[base])<<")"<<std::endl;
		glDrawElementsInstanced(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, &(indices[offsetFirst]), primCount);
	}
}

