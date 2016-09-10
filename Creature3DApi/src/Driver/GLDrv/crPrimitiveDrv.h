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
#include <crgl/glu.h>
#include <CRCore/crBufferObject.h>
#include <CRCore/crNotify.h>
#include <CRCore/crStreamBuf.h>
void DrawArrays::draw(crState& state, bool useVertexBufferObjects) const
{
	//char gbuf[256];
	//sprintf(gbuf,"DrawArrays mode:%d,first:%d,count:%d\n\0",m_mode,m_first,m_count);
	//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
    glDrawArrays(m_mode,m_first,m_count); 
	//GLenum errorNo = glGetError();
	//sprintf(gbuf,"DrawArrays %d\n\0",errorNo);
	//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	//CRCore::notify(CRCore::ALWAYS)<<"DrawArrays::draw"<<std::endl;
}

void DrawArrayLengths::draw(crState& state, bool useVertexBufferObjects) const
{
	//char gbuf[256];
	//sprintf(gbuf,"DrawArrayLengths mode:%d,first:%d,count:%d\n\0",m_mode,m_first,m_indexArray.size());
	//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	int first = m_first;
	for(DrawArrayLengths::PT_IndexArray::const_iterator itr=m_indexArray.begin();
		itr!=m_indexArray.end();
		++itr)
	{
		glDrawArrays(m_mode,first,*itr);
		first += *itr;
	}
	//GLenum errorNo = glGetError();
	//sprintf(gbuf,"DrawArrayLengths %d\n\0",errorNo);
	//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
}
void DrawElementsUByte::compile(crState& state, bool useVertexBufferObjects) const
{
	if (useVertexBufferObjects)
	{
		//crPrimitive::GLObjectList &vboList = draw->getVBOList();
		//unsigned int modifiedCount = draw->getModifiedCount();

		unsigned int contextID = state.getContextID();
		const crBufferObject::Extensions* extensions = crBufferObject::getExtensions(contextID, true);

		GLuint& buffer = m_vboList[contextID].m_objectID;
		if (!buffer)
		{
			extensions->glGenBuffers(1, &buffer);
			extensions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, buffer);
			extensions->glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, getNumIndices(), getIndeciesAddr(), /*GL_DYNAMIC_DRAW_ARB*/GL_STATIC_DRAW_ARB);

			m_vboList[contextID].m_modifiedCount = m_modifiedCount;
		}
		else
		{
			extensions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, buffer);

			if (m_vboList[contextID].m_modifiedCount != m_modifiedCount) 
			{
				extensions->glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, getNumIndices(), getIndeciesAddr(), /*GL_DYNAMIC_DRAW_ARB*/GL_STATIC_DRAW_ARB);
				m_vboList[contextID].m_modifiedCount = m_modifiedCount;
			}
		}
		//char gbuf[256];
		//sprintf(gbuf,"DrawElementsUByte mode:%d,count:%d,bufid:%d\n\0",m_mode,getNumIndices(),buffer);
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		//glDrawElements(m_mode, getNumIndices(), GL_UNSIGNED_BYTE, 0);
		//GLenum errorNo = glGetError();
		//sprintf(gbuf,"DrawElementsUByte %d\n\0",errorNo);
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);

		extensions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	}
}
void DrawElementsUByte::draw(crState& state, bool useVertexBufferObjects) const
{
	if (useVertexBufferObjects)
	{
		//crPrimitive::GLObjectList &vboList = draw->getVBOList();
		//unsigned int modifiedCount = draw->getModifiedCount();

		unsigned int contextID = state.getContextID();
		const crBufferObject::Extensions* extensions = crBufferObject::getExtensions(contextID, true);

		GLuint& buffer = m_vboList[contextID].m_objectID;
		if (!buffer)
		{
			extensions->glGenBuffers(1, &buffer);
			extensions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, buffer);
			extensions->glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, getNumIndices(), getIndeciesAddr(), /*GL_DYNAMIC_DRAW_ARB*/GL_STATIC_DRAW_ARB);

			m_vboList[contextID].m_modifiedCount = m_modifiedCount;
		}
		else
		{
			extensions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, buffer);

			if (m_vboList[contextID].m_modifiedCount != m_modifiedCount) 
			{
				extensions->glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, getNumIndices(), getIndeciesAddr(), /*GL_DYNAMIC_DRAW_ARB*/GL_STATIC_DRAW_ARB);
				m_vboList[contextID].m_modifiedCount = m_modifiedCount;
			}
		}
		//char gbuf[256];
		//sprintf(gbuf,"DrawElementsUByte mode:%d,count:%d,bufid:%d\n\0",m_mode,getNumIndices(),buffer);
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		glDrawElements(m_mode, getNumIndices(), GL_UNSIGNED_BYTE, 0);
		//GLenum errorNo = glGetError();
		//sprintf(gbuf,"DrawElementsUByte %d\n\0",errorNo);
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);

		extensions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	}
	else 
	{
		glDrawElements(m_mode,getNumIndices(),GL_UNSIGNED_BYTE,getIndeciesAddr());
	}
}

void DrawElementsUByte::releaseObjects(crState* state)
{
	if (state)
	{
		unsigned int contextID = state->getContextID();
		if (m_vboList[contextID].m_objectID != 0)
		{
			crBufferObject::deleteBufferObject(contextID,m_vboList[contextID].m_objectID);
			m_vboList[contextID].m_objectID = 0;
		}
	}
	else
	{
		for(unsigned int i=0;i<m_vboList.size();++i)
		{
			if (m_vboList[i].m_objectID != 0)
			{
				crBufferObject::deleteBufferObject(i,m_vboList[i].m_objectID);
				m_vboList[i].m_objectID = 0;
			}
		}
	}
}
void DrawElementsUShort::compile(crState& state, bool useVertexBufferObjects) const
{
	if (useVertexBufferObjects)
	{
		unsigned int contextID = state.getContextID();
		const crBufferObject::Extensions* extensions = crBufferObject::getExtensions(contextID, true);

		GLuint& buffer = m_vboList[contextID].m_objectID;
		if (!buffer)
		{
			extensions->glGenBuffers(1, &buffer);
			extensions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, buffer);
			extensions->glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, getNumIndices() * 2, getIndeciesAddr(), /*GL_DYNAMIC_DRAW_ARB*/GL_STATIC_DRAW_ARB);

			m_vboList[contextID].m_modifiedCount = m_modifiedCount;
		}
		else
		{
			extensions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, buffer);

			if (m_vboList[contextID].m_modifiedCount != m_modifiedCount) 
			{
				extensions->glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, getNumIndices() * 2, getIndeciesAddr(), /*GL_DYNAMIC_DRAW_ARB*/GL_STATIC_DRAW_ARB);
				m_vboList[contextID].m_modifiedCount = m_modifiedCount;
			}
		}
		//char gbuf[256];
		//sprintf(gbuf,"DrawElementsUShort mode:%d,count:%d,bufid:%d\n\0",m_mode,getNumIndices(),buffer);
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		//GLenum errorNo = glGetError();
		//if(errorNo==GL_NO_ERROR)
		//glDrawElements(m_mode, getNumIndices(), GL_UNSIGNED_SHORT, 0);
		//sprintf(gbuf,"DrawElementsUShort %d\n\0",errorNo);
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);

		extensions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	}
}
void DrawElementsUShort::draw(crState& state, bool useVertexBufferObjects) const
{
	if (useVertexBufferObjects)
	{
		unsigned int contextID = state.getContextID();
		const crBufferObject::Extensions* extensions = crBufferObject::getExtensions(contextID, true);

		GLuint& buffer = m_vboList[contextID].m_objectID;
		if (!buffer)
		{
			extensions->glGenBuffers(1, &buffer);
			extensions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, buffer);
			extensions->glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, getNumIndices() * 2, getIndeciesAddr(), /*GL_DYNAMIC_DRAW_ARB*/GL_STATIC_DRAW_ARB);

			m_vboList[contextID].m_modifiedCount = m_modifiedCount;
		}
		else
		{
			extensions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, buffer);

			if (m_vboList[contextID].m_modifiedCount != m_modifiedCount) 
			{
				extensions->glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, getNumIndices() * 2, getIndeciesAddr(), /*GL_DYNAMIC_DRAW_ARB*/GL_STATIC_DRAW_ARB);
				m_vboList[contextID].m_modifiedCount = m_modifiedCount;
			}
		}
		//char gbuf[256];
		//sprintf(gbuf,"DrawElementsUShort mode:%d,count:%d,bufid:%d\n\0",m_mode,getNumIndices(),buffer);
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		//GLenum errorNo = glGetError();
		//if(errorNo==GL_NO_ERROR)
		glDrawElements(m_mode, getNumIndices(), GL_UNSIGNED_SHORT, 0);
		//sprintf(gbuf,"DrawElementsUShort %d\n\0",errorNo);
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);

		extensions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	}
	else 
	{
		glDrawElements(m_mode,getNumIndices(),GL_UNSIGNED_SHORT,getIndeciesAddr());
	}
}

void DrawElementsUShort::releaseObjects(crState* state)
{
	if (state)
	{
		unsigned int contextID = state->getContextID();
		if (m_vboList[contextID].m_objectID != 0)
		{
			//CRCore::notify(CRCore::ALWAYS)<<"crOglDrawElementsUShort::releaseImplementation "<<vboList[contextID].m_objectID<<std::endl;
			crBufferObject::deleteBufferObject(contextID,m_vboList[contextID].m_objectID);
			m_vboList[contextID].m_objectID = 0;
		}
	}
	else
	{
		for(unsigned int i=0;i<m_vboList.size();++i)
		{
			if (m_vboList[i].m_objectID != 0)
			{
				//CRCore::notify(CRCore::ALWAYS)<<"crOglDrawElementsUShort::releaseImplementation "<<vboList[i].m_objectID<<std::endl;
				crBufferObject::deleteBufferObject(i,m_vboList[i].m_objectID);
				m_vboList[i].m_objectID = 0;
			}
		}
	}
}
void DrawElementsUInt::compile(crState& state, bool useVertexBufferObjects) const
{
	if (useVertexBufferObjects)
	{
		unsigned int contextID = state.getContextID();
		const crBufferObject::Extensions* extensions = crBufferObject::getExtensions(contextID, true);

		GLuint& buffer = m_vboList[contextID].m_objectID;
		if (!buffer)
		{
			extensions->glGenBuffers(1, &buffer);
			extensions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, buffer);
			extensions->glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, getNumIndices() * 4, getIndeciesAddr(), /*GL_DYNAMIC_DRAW_ARB*/GL_STATIC_DRAW_ARB);

			m_vboList[contextID].m_modifiedCount = m_modifiedCount;
		}
		else
		{
			extensions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, buffer);

			if (m_vboList[contextID].m_modifiedCount != m_modifiedCount) 
			{
				extensions->glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, getNumIndices() * 4, getIndeciesAddr(), /*GL_DYNAMIC_DRAW_ARB*/GL_STATIC_DRAW_ARB);
				m_vboList[contextID].m_modifiedCount = m_modifiedCount;
			}
		}
		//char gbuf[256];
		//sprintf(gbuf,"DrawElementsUInt mode:%d,count:%d,bufid:%d\n\0",m_mode,getNumIndices(),buffer);
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		//glDrawElements(m_mode, getNumIndices(), GL_UNSIGNED_INT, 0);
		//GLenum errorNo = glGetError();
		//sprintf(gbuf,"DrawElementsUInt %d\n\0",errorNo);
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);

		extensions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	}
}
void DrawElementsUInt::draw(crState& state, bool useVertexBufferObjects) const
{
	if (useVertexBufferObjects)
	{
		unsigned int contextID = state.getContextID();
		const crBufferObject::Extensions* extensions = crBufferObject::getExtensions(contextID, true);

		GLuint& buffer = m_vboList[contextID].m_objectID;
		if (!buffer)
		{
			extensions->glGenBuffers(1, &buffer);
			extensions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, buffer);
			extensions->glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, getNumIndices() * 4, getIndeciesAddr(), /*GL_DYNAMIC_DRAW_ARB*/GL_STATIC_DRAW_ARB);

			m_vboList[contextID].m_modifiedCount = m_modifiedCount;
		}
		else
		{
			extensions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, buffer);

			if (m_vboList[contextID].m_modifiedCount != m_modifiedCount) 
			{
				extensions->glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, getNumIndices() * 4, getIndeciesAddr(), /*GL_DYNAMIC_DRAW_ARB*/GL_STATIC_DRAW_ARB);
				m_vboList[contextID].m_modifiedCount = m_modifiedCount;
			}
		}
		//char gbuf[256];
		//sprintf(gbuf,"DrawElementsUInt mode:%d,count:%d,bufid:%d\n\0",m_mode,getNumIndices(),buffer);
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		glDrawElements(m_mode, getNumIndices(), GL_UNSIGNED_INT, 0);
		//GLenum errorNo = glGetError();
		//sprintf(gbuf,"DrawElementsUInt %d\n\0",errorNo);
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);

		extensions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	}
	else 
	{
		glDrawElements(m_mode,getNumIndices(),GL_UNSIGNED_INT,getIndeciesAddr());
	}
}

void DrawElementsUInt::releaseObjects(crState* state)
{
	if (state)
	{
		unsigned int contextID = state->getContextID();
		if (m_vboList[contextID].m_objectID != 0)
		{
			crBufferObject::deleteBufferObject(contextID,m_vboList[contextID].m_objectID);
			m_vboList[contextID].m_objectID = 0;
		}
	}
	else
	{
		for(unsigned int i=0;i<m_vboList.size();++i)
		{
			if (m_vboList[i].m_objectID != 0)
			{
				crBufferObject::deleteBufferObject(i,m_vboList[i].m_objectID);
				m_vboList[i].m_objectID = 0;
			}
		}
	}
}