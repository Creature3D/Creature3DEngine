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
//Modified by Wucaihua
#include <stdio.h>
#include <math.h>
#include <float.h>

#include <CRCore/crBufferObject.h>
#include <CRCore/crNotify.h>
#include <CRCore/crTimer.h>
#include <CRCore/crImage.h>
#include <CRCore/crState.h>
#include <crgl/GLExtensions.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/crStreamBuf.h>
using namespace CRCore;

// static cache of deleted display lists which can only 
// by completely deleted once the appropriate OpenGL context
// is set.  Used CRCore::crBufferObject::deleteDisplayList(..) and flushDeletedBufferObjects(..) below.
typedef std::multimap<unsigned int,GLuint> DisplayListMap;
typedef std::map<unsigned int,DisplayListMap> DeletedBufferObjectCache;

static crMutex s_mutex_deletedBufferObjectCache;
static DeletedBufferObjectCache s_deletedBufferObjectCache;

void crBufferObject::deleteBufferObject(unsigned int contextID,GLuint globj)
{
    if (globj!=0)
    {
        CRCore::ScopedLock<crMutex> lock(s_mutex_deletedBufferObjectCache);
        
        // insert the globj into the cache for the appropriate context.
        s_deletedBufferObjectCache[contextID].insert(DisplayListMap::value_type(0,globj));
    }
}

/** flush all the cached display list which need to be deleted
  * in the OpenGL context related to contextID.*/
void crBufferObject::flushDeletedBufferObjects(unsigned int contextID,double /*currentTime*/, double& availableTime)
{
    // if no time available don't try to flush objects.
    if (availableTime<=0.0) return;

    const CRCore::Timer& timer = *CRCore::Timer::instance();
    CRCore::Timer_t start_tick = timer.tick();
    double elapsedTime = 0.0;


    {
        CRCore::ScopedLock<crMutex> lock(s_mutex_deletedBufferObjectCache);

        DeletedBufferObjectCache::iterator citr = s_deletedBufferObjectCache.find(contextID);
        if (citr!=s_deletedBufferObjectCache.end())
        {
            const Extensions* extensions = getExtensions(contextID,true);

            unsigned int noDeleted = 0;

            DisplayListMap& dll = citr->second;

            DisplayListMap::iterator ditr=dll.begin();
            for(;
                ditr!=dll.end() && elapsedTime<availableTime;
                ++ditr)
            {
				//CRCore::notify(CRCore::ALWAYS)<<"crBufferObject::flushDeletedBufferObjects glDeleteBuffers "<<ditr->second<<std::endl;
                extensions->glDeleteBuffers(1,&(ditr->second));
                elapsedTime = timer.delta_s(start_tick,timer.tick());
                ++noDeleted;
            }
            if (ditr!=dll.begin()) dll.erase(dll.begin(),ditr);

            if (noDeleted!=0)
			{
				char gbuf[256];
				sprintf(gbuf,"IBOs deleted:%d\n\0",noDeleted);
				gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
				//notify(CRCore::INFO)<<"Number VBOs deleted = "<<noDeleted<<std::endl;
			}
        }
    }    
    
    availableTime -= elapsedTime;
}
void crBufferObject::flushAllDeletedBufferObjects(unsigned int contextID)
{
	CRCore::ScopedLock<crMutex> lock(s_mutex_deletedBufferObjectCache);

	DeletedBufferObjectCache::iterator citr = s_deletedBufferObjectCache.find(contextID);
	if (citr!=s_deletedBufferObjectCache.end())
	{
		const Extensions* extensions = getExtensions(contextID,true);

		unsigned int noDeleted = 0;

		DisplayListMap& dll = citr->second;

		DisplayListMap::iterator ditr=dll.begin();
		for(;
			ditr!=dll.end();
			++ditr)
		{
			//CRCore::notify(CRCore::ALWAYS)<<"crBufferObject::flushDeletedBufferObjects glDeleteBuffers "<<ditr->second<<std::endl;
			extensions->glDeleteBuffers(1,&(ditr->second));
			++noDeleted;
		}
		if (ditr!=dll.begin()) dll.erase(dll.begin(),ditr);

		if (noDeleted!=0)
		{
			char gbuf[256];
			sprintf(gbuf,"Number IBOs deleted:%d\n\0",noDeleted);
			gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
			//notify(CRCore::INFO)<<"Number VBOs deleted = "<<noDeleted<<std::endl;
		}
	}
}
//void crBufferObject::discardDeletedBufferObjects(unsigned int contextID)
//{
//	CRCore::ScopedLock<crMutex> lock(s_mutex_deletedBufferObjectCache);
//	DisplayListMap& dll = s_deletedBufferObjectCache[contextID];
//	dll.clear();
//}

crBufferObject::crBufferObject():
    m_target(0),
    m_usage(0),
    m_totalSize(0)
{
}

crBufferObject::crBufferObject(const crBufferObject& bo,const crCopyOp& copyop):
    crBase(bo,copyop)
{
}

crBufferObject::~crBufferObject()
{
    releaseBuffer(0);
}

//void crBufferObject::resizeGLObjectBuffers(unsigned int maxSize)
//{
//	m_bufferObjectList.resize(maxSize);
//}

void crBufferObject::releaseBuffer(crState* state) const
{
    if (state)
    {
        unsigned int contextID = state->getContextID();
        if (m_bufferObjectList[contextID])
        {
             deleteBufferObject(contextID,m_bufferObjectList[contextID]);
            m_bufferObjectList[contextID] = 0;
        }
    }
    else
    {
        for(unsigned int contextID=0;contextID<m_bufferObjectList.size();++contextID)
        {
            if (m_bufferObjectList[contextID])
            {
                 deleteBufferObject(contextID,m_bufferObjectList[contextID]);
                m_bufferObjectList[contextID] = 0;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
//
//  Extension support
//

typedef buffered_value< ref_ptr<crBufferObject::Extensions> > BufferedExtensions;
static BufferedExtensions s_extensions;
void crBufferObject::Extensions::clearExtensions()
{
	s_extensions.clear();
}
crBufferObject::Extensions* crBufferObject::getExtensions(unsigned int contextID,bool createIfNotInitalized)
{
    if (!s_extensions[contextID] && createIfNotInitalized) s_extensions[contextID] = new crBufferObject::Extensions(contextID);
    return s_extensions[contextID].get();
}

void crBufferObject::setExtensions(unsigned int contextID,Extensions* extensions)
{
    s_extensions[contextID] = extensions;
}

crBufferObject::Extensions::Extensions(unsigned int contextID)
{
    setupGLExtenions(contextID);
}

crBufferObject::Extensions::Extensions(const Extensions& rhs):
    Referenced()
{
    m_glGenBuffers = rhs.m_glGenBuffers;
    m_glBindBuffer = rhs.m_glBindBuffer;
    m_glBufferData = rhs.m_glBufferData;
    m_glBufferSubData = rhs.m_glBufferSubData;
    m_glDeleteBuffers = rhs.m_glDeleteBuffers;
    m_glIsBuffer = rhs.m_glIsBuffer;
    m_glGetBufferSubData = rhs.m_glGetBufferSubData;
    m_glMapBuffer = rhs.m_glMapBuffer;
    m_glUnmapBuffer = rhs.m_glUnmapBuffer;
    m_glGetBufferParameteriv = rhs.m_glGetBufferParameteriv;
    m_glGetBufferPointerv = rhs.m_glGetBufferPointerv;
}


void crBufferObject::Extensions::lowestCommonDenominator(const Extensions& rhs)
{
    if (!rhs.m_glGenBuffers) m_glGenBuffers = rhs.m_glGenBuffers;
    if (!rhs.m_glBindBuffer) m_glBindBuffer = rhs.m_glBindBuffer;
    if (!rhs.m_glBufferData) m_glBufferData = rhs.m_glBufferData;
    if (!rhs.m_glBufferSubData) m_glBufferSubData = rhs.m_glBufferSubData;
    if (!rhs.m_glDeleteBuffers) m_glDeleteBuffers = rhs.m_glDeleteBuffers;
    if (!rhs.m_glIsBuffer) m_glIsBuffer = rhs.m_glIsBuffer;
    if (!rhs.m_glGetBufferSubData) m_glGetBufferSubData = rhs.m_glGetBufferSubData;
    if (!rhs.m_glMapBuffer) m_glMapBuffer = rhs.m_glMapBuffer;
    if (!rhs.m_glUnmapBuffer) m_glUnmapBuffer = rhs.m_glUnmapBuffer;
    if (!rhs.m_glGetBufferParameteriv) m_glGetBufferParameteriv = rhs.m_glGetBufferParameteriv;
    if (!rhs.m_glGetBufferParameteriv) m_glGetBufferPointerv = rhs.m_glGetBufferPointerv;
}

void crBufferObject::Extensions::setupGLExtenions(unsigned int contextID)
{
    m_glGenBuffers = ((GenBuffersProc)CRCore::getGLExtensionFuncPtr("glGenBuffers","glGenBuffersARB"));
    m_glBindBuffer = ((BindBufferProc)CRCore::getGLExtensionFuncPtr("glBindBuffer","glBindBufferARB"));
    m_glBufferData = ((BufferDataProc)CRCore::getGLExtensionFuncPtr("glBufferData","glBufferDataARB"));
    m_glBufferSubData = ((BufferSubDataProc)CRCore::getGLExtensionFuncPtr("glBufferSubData","glBufferSubDataARB"));
    m_glDeleteBuffers = ((DeleteBuffersProc)CRCore::getGLExtensionFuncPtr("glDeleteBuffers","glDeleteBuffersARB"));
    m_glIsBuffer = ((IsBufferProc)CRCore::getGLExtensionFuncPtr("glIsBuffer","glIsBufferARB"));
    m_glGetBufferSubData = ((GetBufferSubDataProc)CRCore::getGLExtensionFuncPtr("glGetBufferSubData","glGetBufferSubDataARB"));
    m_glMapBuffer = ((MapBufferProc)CRCore::getGLExtensionFuncPtr("glMapBuffer","glMapBufferARB"));
    m_glUnmapBuffer = ((UnmapBufferProc)CRCore::getGLExtensionFuncPtr("glUnmapBuffer","glUnmapBufferARB"));
    m_glGetBufferParameteriv = ((GetBufferParameterivProc)CRCore::getGLExtensionFuncPtr("glGetBufferParameteriv","glGetBufferParameterivARB"));
    m_glGetBufferPointerv = ((GetBufferPointervProc)CRCore::getGLExtensionFuncPtr("glGetBufferPointerv","glGetBufferPointervARB"));
	m_isPBOSupported = isGLExtensionSupported(contextID,"GL_ARB_pixel_buffer_object");
}

void crBufferObject::Extensions::glGenBuffers(GLsizei n, GLuint *buffers) const
{
    if (m_glGenBuffers) m_glGenBuffers(n, buffers); 
    else notify(WARN)<<"Error: glGenBuffers not supported by OpenGL driver"<<std::endl;
}

void crBufferObject::Extensions::glBindBuffer(GLenum target, GLuint buffer) const
{
    if (m_glBindBuffer) m_glBindBuffer(target, buffer); 
    else notify(WARN)<<"Error: glBindBuffer not supported by OpenGL driver"<<std::endl;
}

void crBufferObject::Extensions::glBufferData(GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage) const
{
	//if(size>100000)
	//{
	//	int i = 0;
	//	notify(WARN)<<"Error: glBufferData size:"<<size<<std::endl;
	//}
    if (m_glBufferData) m_glBufferData(target, size, data, usage); 
    else notify(WARN)<<"Error: glBufferData not supported by OpenGL driver"<<std::endl;
}

void crBufferObject::Extensions::glBufferSubData(GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data) const
{
    if (m_glBufferSubData) m_glBufferSubData(target, offset, size, data); 
    else notify(WARN)<<"Error: glBufferData not supported by OpenGL driver"<<std::endl;
}

void crBufferObject::Extensions::glDeleteBuffers(GLsizei n, const GLuint *buffers) const
{
    if (m_glDeleteBuffers) m_glDeleteBuffers(n, buffers); 
    else notify(WARN)<<"Error: glBufferData not supported by OpenGL driver"<<std::endl;
}

GLboolean crBufferObject::Extensions::glIsBuffer (GLuint buffer) const
{
    if (m_glIsBuffer) return m_glIsBuffer(buffer);
    else 
    {
        notify(WARN)<<"Error: glIsBuffer not supported by OpenGL driver"<<std::endl;
        return GL_FALSE;
    }
}

void crBufferObject::Extensions::glGetBufferSubData (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data) const
{
    if (m_glGetBufferSubData) m_glGetBufferSubData(target,offset,size,data);
    else notify(WARN)<<"Error: glGetBufferSubData not supported by OpenGL driver"<<std::endl;
}

GLvoid* crBufferObject::Extensions::glMapBuffer (GLenum target, GLenum access) const
{
    if (m_glMapBuffer) return m_glMapBuffer(target,access);
    else 
    {
        notify(WARN)<<"Error: glMapBuffer not supported by OpenGL driver"<<std::endl;
        return 0;
    }
}

GLboolean crBufferObject::Extensions::glUnmapBuffer (GLenum target) const
{
    if (m_glUnmapBuffer) return m_glUnmapBuffer(target);
    else 
    {
        notify(WARN)<<"Error: glUnmapBuffer not supported by OpenGL driver"<<std::endl;
        return GL_FALSE;
    }
}

void crBufferObject::Extensions::glGetBufferParameteriv (GLenum target, GLenum pname, GLint *params) const
{
    if (m_glGetBufferParameteriv) m_glGetBufferParameteriv(target,pname,params);
    else notify(WARN)<<"Error: glGetBufferParameteriv not supported by OpenGL driver"<<std::endl;
}

void crBufferObject::Extensions::glGetBufferPointerv (GLenum target, GLenum pname, GLvoid* *params) const
{
    if (m_glGetBufferPointerv) m_glGetBufferPointerv(target,pname,params);
    else notify(WARN)<<"Error: glGetBufferPointerv not supported by OpenGL driver"<<std::endl;
}

/*
//////////////////////////////////////////////////////////////////////////////////
//
//  crVertexBufferObject
//
crVertexBufferObject::crVertexBufferObject()
{
	m_target = GL_ARRAY_BUFFER_ARB;
	m_usage = GL_STATIC_DRAW_ARB;
	//    m_usage = GL_DYNAMIC_DRAW_ARB;
	//    m_usage = GL_STREAM_DRAW_ARB;
}

crVertexBufferObject::crVertexBufferObject(const crVertexBufferObject& vbo,const crCopyOp& copyop):
crBufferObject(vbo,copyop)
{
}

crVertexBufferObject::~crVertexBufferObject()
{
}

unsigned int crVertexBufferObject::addArray(CRCore::Array* array)
{
	unsigned int i = m_bufferEntryArrayPairs.size();

	m_bufferEntryArrayPairs.resize(i+1);
	m_bufferEntryArrayPairs[i].second = array;
	m_bufferEntryArrayPairs[i].first.modifiedCount.setAllElementsTo(0xffffffff);
	m_bufferEntryArrayPairs[i].first.offset = 0;

	dirty();

	return i;
}

void crVertexBufferObject::removeArray(CRCore::Array* array)
{
	BufferEntryArrayPairs::iterator itr;
	for(itr = m_bufferEntryArrayPairs.begin();
		itr != m_bufferEntryArrayPairs.end();
		++itr)
	{
		if (itr->second == array) break;
	}
	if (itr != m_bufferEntryArrayPairs.end()) m_bufferEntryArrayPairs.erase(itr);
}

void crVertexBufferObject::setArray(unsigned int i, Array* array)
{
	if (i+1>=m_bufferEntryArrayPairs.size()) m_bufferEntryArrayPairs.resize(i+1);

	m_bufferEntryArrayPairs[i].second = array;
	m_bufferEntryArrayPairs[i].first.modifiedCount.setAllElementsTo(0xffffffff);
	m_bufferEntryArrayPairs[i].first.offset = 0;

	dirty();
}
void crVertexBufferObject::compileBuffer(crState& state) const
{
	unsigned int contextID = state.getContextID();

	m_compiledList[contextID] = 1;

	Extensions* extensions = getExtensions(contextID,true);

	// CRCore::notify(CRCore::NOTICE)<<"crVertexBufferObject::compileBuffer frameNumber="<<state.getFrameStamp()->getFrameNumber()<<std::endl;

	unsigned int totalSizeRequired = 0;
	//    unsigned int numModified = 0;
	//    unsigned int numNotModified = 0;
	for(BufferEntryArrayPairs::const_iterator itr = m_bufferEntryArrayPairs.begin();
		itr != m_bufferEntryArrayPairs.end();
		++itr)
	{
		const BufferEntryArrayPair& bep = *itr;
		if (bep.second)
		{
			totalSizeRequired += bep.second->getTotalDataSize();
		}
	}

	bool copyAll = false;
	GLuint& vbo = buffer(contextID);
	if (vbo==0)
	{
		// building for the first time.

		m_totalSize = totalSizeRequired;

		// don't generate buffer if size is zero.
		if (m_totalSize==0) return;

		extensions->glGenBuffers(1, &vbo);
		extensions->glBindBuffer(m_target, vbo);
		extensions->glBufferData(m_target, m_totalSize, NULL, m_usage);

		copyAll = true;
	}
	else
	{
		extensions->glBindBuffer(m_target, vbo);

		if (m_totalSize != totalSizeRequired)
		{
			// resize vbo.
			m_totalSize = totalSizeRequired;
			extensions->glBufferData(m_target, m_totalSize, NULL, m_usage);

			copyAll = true;
		}
	}

	//    CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();


	void* vboMemory = 0;

#if 0
	vboMemory = extensions->glMapBuffer(m_target, GL_WRITE_ONLY_ARB);
#endif

	unsigned int offset = 0;
	for(BufferEntryArrayPairs::const_iterator itr = m_bufferEntryArrayPairs.begin();
		itr != m_bufferEntryArrayPairs.end();
		++itr)
	{
		const BufferEntryArrayPair& bep = *itr;
		const Array* de = bep.second;
		if (de)
		{
			if (copyAll ||
				bep.first.modifiedCount[contextID] != bep.second->getModifiedCount() ||
				bep.first.dataSize != bep.second->getTotalDataSize())
			{
				// copy data across
				bep.first.dataSize = bep.second->getTotalDataSize();
				bep.first.modifiedCount[contextID] = de->getModifiedCount();
				if (copyAll)
				{
					bep.first.offset = offset;
					de->setVertexBufferObjectOffset((GLvoid*)offset);
					offset += bep.first.dataSize;
				}

				// CRCore::notify(CRCore::NOTICE)<<"   copying vertex buffer data "<<bep.first.dataSize<<" bytes"<<std::endl;

				if (vboMemory)
					memcpy((char*)vboMemory + bep.first.offset, de->getDataPointer(), bep.first.dataSize);
				else
					extensions->glBufferSubData(m_target, bep.first.offset, bep.first.dataSize, de->getDataPointer());

			}
		}
	}


	// Unmap the texture image buffer
	if (vboMemory) extensions->glUnmapBuffer(m_target);

	//    CRCore::notify(CRCore::NOTICE)<<"pbo m_totalSize="<<m_totalSize<<std::endl;
	//    CRCore::notify(CRCore::NOTICE)<<"pbo "<<CRCore::Timer::instance()->delta_m(start_tick,CRCore::Timer::instance()->tick())<<"ms"<<std::endl;
}

void crVertexBufferObject::resizeGLObjectBuffers(unsigned int maxSize)
{
	crBufferObject::resizeGLObjectBuffers(maxSize);

	for(BufferEntryArrayPairs::iterator itr = m_bufferEntryArrayPairs.begin();
		itr != m_bufferEntryArrayPairs.end();
		++itr)
	{
		itr->first.modifiedCount.resize(maxSize);
	}
}

//////////////////////////////////////////////////////////////////////////////////
//
//  crElementBufferObject
//
crElementBufferObject::crElementBufferObject()
{
	m_target = GL_ELEMENT_ARRAY_BUFFER_ARB;
	m_usage = GL_STATIC_DRAW_ARB;
}

crElementBufferObject::crElementBufferObject(const crElementBufferObject& vbo,const crCopyOp& copyop):
crBufferObject(vbo,copyop)
{
}

crElementBufferObject::~crElementBufferObject()
{
}

unsigned int crElementBufferObject::addDrawElements(CRCore::crPrimitive* drawElements)
{
	unsigned int i = m_bufferEntryDrawElementsPairs.size();
	m_bufferEntryDrawElementsPairs.resize(i+1);
	m_bufferEntryDrawElementsPairs[i].second = drawElements;
	m_bufferEntryDrawElementsPairs[i].first.modifiedCount.setAllElementsTo(0xffffffff);
	m_bufferEntryDrawElementsPairs[i].first.dataSize = 0;

	return i;
}

void crElementBufferObject::removeDrawElements(CRCore::crPrimitive* drawElements)
{
	BufferEntryDrawElementsPairs::iterator itr;
	for(itr = m_bufferEntryDrawElementsPairs.begin();
		itr != m_bufferEntryDrawElementsPairs.end();
		++itr)
	{
		if (itr->second == drawElements) break;
	}
	if (itr != m_bufferEntryDrawElementsPairs.end()) m_bufferEntryDrawElementsPairs.erase(itr);
}

void crElementBufferObject::setDrawElements(unsigned int i, crPrimitive* drawElements)
{
	if (i+1>=m_bufferEntryDrawElementsPairs.size()) m_bufferEntryDrawElementsPairs.resize(i+1);

	m_bufferEntryDrawElementsPairs[i].second = drawElements;
	m_bufferEntryDrawElementsPairs[i].first.modifiedCount.setAllElementsTo(0xffffffff);
	m_bufferEntryDrawElementsPairs[i].first.dataSize = 0;
}

void crElementBufferObject::compileBuffer(crState& state) const
{
	unsigned int contextID = state.getContextID();

	m_compiledList[contextID] = 1;

	// CRCore::notify(CRCore::NOTICE)<<"crElementBufferObject::compile"<<std::endl;

	Extensions* extensions = getExtensions(contextID,true);

	unsigned int totalSizeRequired = 0;
	//    unsigned int numModified = 0;
	//    unsigned int numNotModified = 0;
	for(BufferEntryDrawElementsPairs::const_iterator itr = m_bufferEntryDrawElementsPairs.begin();
		itr != m_bufferEntryDrawElementsPairs.end();
		++itr)
	{
		const BufferEntryDrawElementsPair& bep = *itr;
		if (bep.second)
		{
			totalSizeRequired += bep.second->getTotalDataSize();
		}
	}

	bool copyAll = false;
	GLuint& ebo = buffer(contextID);
	if (ebo==0)
	{
		// building for the first time.

		m_totalSize = totalSizeRequired;

		// don't generate buffer if size is zero.
		if (m_totalSize==0) return;

		extensions->glGenBuffers(1, &ebo);
		extensions->glBindBuffer(m_target, ebo);
		extensions->glBufferData(m_target, m_totalSize, NULL, m_usage);

		copyAll = true;
	}
	else
	{
		extensions->glBindBuffer(m_target, ebo);

		if (m_totalSize != totalSizeRequired)
		{
			// resize EBO.
			m_totalSize = totalSizeRequired;
			extensions->glBufferData(m_target, m_totalSize, NULL, m_usage);

			copyAll = true;
		}
	}

	//    CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();


	void* eboMemory = 0;

#if 0
	eboMemory = extensions->glMapBuffer(m_target, GL_WRITE_ONLY_ARB);
#endif

	size_t offset = 0;
	for(BufferEntryDrawElementsPairs::const_iterator itr = m_bufferEntryDrawElementsPairs.begin();
		itr != m_bufferEntryDrawElementsPairs.end();
		++itr)
	{
		const BufferEntryDrawElementsPair& bep = *itr;
		const crPrimitive* de = bep.second;
		if (de)
		{
			if (copyAll ||
				bep.first.modifiedCount[contextID] != bep.second->getModifiedCount() ||
				bep.first.dataSize != bep.second->getTotalDataSize())
			{
				// copy data across
				bep.first.dataSize = bep.second->getTotalDataSize();
				bep.first.modifiedCount[contextID] = de->getModifiedCount();
				if (copyAll)
				{
					bep.first.offset = offset;
					de->setElementBufferObjectOffset((GLvoid*)offset);
					offset += bep.first.dataSize;
				}

				if (eboMemory)
					memcpy((char*)eboMemory + bep.first.offset, de->getDataPointer(), bep.first.dataSize);
				else
					extensions->glBufferSubData(m_target, bep.first.offset, bep.first.dataSize, de->getDataPointer());

			}
		}
	}


	// Unmap the texture image buffer
	if (eboMemory) extensions->glUnmapBuffer(m_target);

	//    CRCore::notify(CRCore::NOTICE)<<"pbo m_totalSize="<<m_totalSize<<std::endl;
	//    CRCore::notify(CRCore::NOTICE)<<"pbo "<<CRCore::Timer::instance()->delta_m(start_tick,CRCore::Timer::instance()->tick())<<"ms"<<std::endl;
}

void crElementBufferObject::resizeGLObjectBuffers(unsigned int maxSize)
{
	crBufferObject::resizeGLObjectBuffers(maxSize);

	for(BufferEntryDrawElementsPairs::iterator itr = m_bufferEntryDrawElementsPairs.begin();
		itr != m_bufferEntryDrawElementsPairs.end();
		++itr)
	{
		itr->first.modifiedCount.resize(maxSize);
	}
}
*/
//////////////////////////////////////////////////////////////////////////////////
//
//  PixelBufferObject
//
crPixelBufferObject::crPixelBufferObject(CRCore::crImage* image):
    crBufferObject()
{
    m_target = GL_PIXEL_UNPACK_BUFFER_ARB;
    m_usage = GL_STREAM_DRAW_ARB;
    m_bufferEntryImagePair.second = image;
}

crPixelBufferObject::crPixelBufferObject(const crPixelBufferObject& buffer,const crCopyOp& copyop):
    crBufferObject(buffer,copyop),
    m_bufferEntryImagePair(buffer.m_bufferEntryImagePair)
{
}

crPixelBufferObject::~crPixelBufferObject()
{
}

void crPixelBufferObject::setImage(CRCore::crImage* image)
{
    m_bufferEntryImagePair.second = image;
}

bool crPixelBufferObject::needsCompile(unsigned int contextID) const
{
    if (!m_bufferEntryImagePair.second) 
        return false;

    if (m_bufferEntryImagePair.first.modifiedCount[contextID]!=m_bufferEntryImagePair.second->getModifiedCount())
        return true;
        
    if (m_bufferObjectList[contextID]==0) return true;

    return false;
}

void crPixelBufferObject::compileBuffer(crState& state) const
{
    unsigned int contextID = state.getContextID();
    if (!needsCompile(contextID)) return;
    
    CRCore::crImage* image = m_bufferEntryImagePair.second;

    m_bufferEntryImagePair.first.modifiedCount[contextID] = image->getModifiedCount();
    if (!image->valid()) return;

    Extensions* extensions = getExtensions(contextID,true);

    GLuint& pbo = buffer(contextID);
    if (pbo==0)
    {
        // building for the first time.

        m_totalSize = image->getTotalSizeInBytes();

        // don't generate buffer if size is zero.        
        if (m_totalSize==0) return;

        extensions->glGenBuffers(1, &pbo);
        extensions->glBindBuffer(m_target, pbo);
        extensions->glBufferData(m_target, m_totalSize, NULL,
                     m_usage);
                    
    }
    else
    {
        extensions->glBindBuffer(m_target, pbo);

        if (m_totalSize != image->getTotalSizeInBytes())
        {

            m_totalSize = image->getTotalSizeInBytes();
            extensions->glBufferData(m_target, m_totalSize, NULL,
                     m_usage);
        }
    }

//    CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();

    void* pboMemory = extensions->glMapBuffer(m_target,
                 GL_WRITE_ONLY_ARB);

    memcpy(pboMemory, image->data(), m_totalSize);
        
    // Unmap the texture image buffer
    extensions->glUnmapBuffer(m_target);

    extensions->glBindBuffer(m_target, 0);
    
    m_bufferEntryImagePair.first.modifiedCount[contextID] = image->getModifiedCount();

//    CRCore::notify(CRCore::NOTICE)<<"pbo m_totalSize="<<m_totalSize<<std::endl;
//    CRCore::notify(CRCore::NOTICE)<<"pbo "<<CRCore::Timer::instance()->delta_m(start_tick,CRCore::Timer::instance()->tick())<<"ms"<<std::endl;
}

//void crPixelBufferObject::resizeGLObjectBuffers(unsigned int maxSize)
//{
//	crBufferObject::resizeGLObjectBuffers(maxSize);
//
//	m_bufferEntryImagePair.first.modifiedCount.resize(maxSize);
//}