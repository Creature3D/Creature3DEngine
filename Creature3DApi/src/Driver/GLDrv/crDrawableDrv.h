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
#include <CRCore/crTimer.h>
#include <CRCore/crNotify.h>
#include <crgl/GL.h>
#include <crgl/GLExtensions.h>
#include <crgl/GLU.h>
#include <crgl/GL2Extensions.h>
#include <CRCore/crState.h>
#include <CRCore/crStreamBuf.h>
#include <algorithm>

#include <CRCore/thread/crScopedLock.h>
#include <CRCore/thread/crMutex.h>
// this is define to alter the way display lists are compiled inside the
// the draw method, it has been found that the NVidia drivers fail completely
// to optimize COMPILE_AND_EXECUTE in fact make it go slower than for no display 
// lists, but optimize a separate COMPILE very well?!  Define it as default
// the use of a sperate COMPILE, then glCallList rather than use COMPILE_AND_EXECUTE.

#define USE_SEPARATE_COMPILE_AND_EXECUTE

GLuint generateDisplayList(unsigned int contextID, unsigned int sizeHint)
{
	CRCore::ScopedLock<crMutex> lock(crDrawable::s_mutex_deletedDisplayListCache);

	crDrawable::DisplayListMap& dll = crDrawable::s_deletedDisplayListCache[contextID];
	if (dll.empty())
	{
		++crDrawable::s_numberNewDrawablesInLastFrame;
		return  glGenLists( 1 );
	}
	else 
	{
		crDrawable::DisplayListMap::iterator itr = dll.lower_bound(sizeHint);
		if (itr!=dll.end())
		{
			// CRCore::notify(CRCore::NOTICE)<<"Reusing a display list of size = "<<itr->first<<" for requested size = "<<sizeHint<<std::endl;

			++crDrawable::s_numberDrawablesReusedLastInLastFrame;

			GLuint globj = itr->second;
			dll.erase(itr);

			return globj;
		} 
		else
		{
			// CRCore::notify(CRCore::NOTICE)<<"Creating a new display list of size = "<<sizeHint<<" although "<<dll.size()<<" are available"<<std::endl;
			++crDrawable::s_numberNewDrawablesInLastFrame;
			return  glGenLists( 1 );
		}
	}
}

void crDrawable::draw( CRCore::crState& state ) const
{
//	if (m_useDisplayList && !(m_supportsVertexBufferObjects && m_useVertexBufferObjects && state.isVertexBufferObjectSupported()))
//	{
//		// get the contextID (user defined ID of 0 upwards) for the 
//		// current OpenGL context.
//		unsigned int contextID = state.getContextID();
//
//		// get the globj for the current contextID.
//		unsigned int& globj = m_globjList[contextID];
//
//		// call the globj if already set otherwise compile and execute.
//		if( globj != 0 )
//		{
//			//CRCore::notify(CRCore::WARN)<<"CallList:"<<globj<<std::endl;
//
//			glCallList( globj );
//		}
//		else if (m_useDisplayList)
//		{
//#ifdef USE_SEPARATE_COMPILE_AND_EXECUTE
//			globj = generateDisplayList(contextID, getObjectSizeHint());
//			glNewList( globj, GL_COMPILE );
//			if (m_drawCallback.valid())
//				m_drawCallback->drawImplementation( this, state );
//			else
//				drawImplementation(state);
//			glEndList();
//
//			glCallList( globj);
//#else
//			globj = generateDisplayList(contextID, getObjectSizeHint());
//			glNewList( globj, GL_COMPILE_AND_EXECUTE );
//			if (m_drawCallback.valid())
//				m_drawCallback->drawImplementation( this, state );
//			else
//				drawImplementation(state);      
//			glEndList();
//#endif
//		}
//
//		state.colorDirty();
//		return;
//
//	}

	//CRCore::notify(CRCore::WARN)<<"crOglGeoRender::drawImplementation() not use DisplayList "<<std::endl;

	if (m_drawCallback.valid())
		m_drawCallback->drawImplementation( this, state );
	else
		drawImplementation(state);

#ifdef OUTPUTGLERROR
		GLenum errorNo = glGetError();
		if (errorNo!=GL_NO_ERROR)
		{
			char gbuf[256];
			sprintf(gbuf,"crDrawable::draw OpenGL error %s\n\0",gluErrorString(errorNo));
			gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		}
#endif
}

void crDrawable::compile( CRCore::crState& state ) const
{
	//if (!pdrawable->getUseDisplayList()) return;
	
	//if (m_useDisplayList && !(m_supportsVertexBufferObjects && m_useVertexBufferObjects && state.isVertexBufferObjectSupported()))
	//{
	//	// get the contextID (user defined ID of 0 upwards) for the 
	//	// current OpenGL context.
	//	unsigned int contextID = state.getContextID();
	//	// get the globj for the current contextID.
	//	GLuint& globj = m_globjList[contextID];

	//	// call the globj if already set otherwise comple and execute.
	//	if( globj != 0 )
	//	{   
	//		glDeleteLists( globj, 1 );
	//	}

	//	globj = generateDisplayList(contextID, getObjectSizeHint());
	//	//CRCore::notify(ALWAYS)<< "  gen DisplayList "<<globj<< std::endl;
	//	glNewList( globj, GL_COMPILE );
	//	if (m_drawCallback.valid())
	//		m_drawCallback->drawImplementation( this, state );
	//	else
	//		drawImplementation(state);
	//	glEndList();

	//	state.colorDirty();
	//}
	//else 
	if( m_supportsVertexBufferObjects && m_useVertexBufferObjects && state.isVertexBufferObjectSupported() )
	{	
		if (m_drawCallback.valid())
			m_drawCallback->drawImplementation( this, state );
		else
			drawImplementation(state);
	}
}

void crDrawable::releaseObjects(crState* state)
{   
	if (m_stateset.valid())
	{
		m_stateset->releaseObjects(state);
		//m_stateset = NULL;
	}

	if (!m_useDisplayList&&!m_useVertexBufferObjects) return;

	if(!state)
		const_cast<crDrawable*>(this)->dirtyDisplayList();
	else
	{
		// get the contextID (user defined ID of 0 upwards) for the 
		// current OpenGL context.
		unsigned int contextID = state->getContextID();

		// get the globj for the current contextID.
		GLuint& globj = m_globjList[contextID];

		// call the globj if already set otherwise comple and execute.
		if( globj != 0 )
		{
			crDrawable::deleteDisplayList(contextID,globj,getObjectSizeHint());
			//CRCore::notify(CRCore::ALWAYS)<<"crOglGeoRender::releaseImplementation glDeleteLists "<<globj<<std::endl;
			//glDeleteLists( globj, 1 );
			globj = 0;
		} 

		GLuint& glvbo = m_vboList[contextID];

		// call the globj if already set otherwise comple and execute.
		if( glvbo != 0 )
		{
			crDrawable::deleteVertexBufferObject(contextID,glvbo);
			glvbo = 0;
			//const Extensions* extensions = getExtensions(contextID,true);
			////CRCore::notify(CRCore::ALWAYS)<<"crOglGeoRender::releaseImplementation glDeleteBuffers "<<glvbo<<std::endl;
			//extensions->glDeleteBuffers(1,&glvbo);
		}
	}
	//m_globjList.clear();
	//m_vboList.clear();
	m_abortCurrentRendering = true;
}

void crDrawable::flushAllDeletedDisplayLists(unsigned int contextID)
{
	CRCore::ScopedLock<crMutex> lock(crDrawable::s_mutex_deletedDisplayListCache);

	crDrawable::DeletedDisplayListCache::iterator citr = crDrawable::s_deletedDisplayListCache.find(contextID);
	if (citr!=crDrawable::s_deletedDisplayListCache.end())
	{
		crDrawable::DisplayListMap& dll = citr->second;

		for(crDrawable::DisplayListMap::iterator ditr=dll.begin();
			ditr!=dll.end();
			++ditr)
		{
			glDeleteLists(ditr->second,1);
		}

		dll.clear();         
	}
}


void crDrawable::flushDeletedDisplayLists(unsigned int contextID, double& availableTime)
{
	// if no time available don't try to flush objects.
	if (availableTime<=0.0) return;

	const CRCore::Timer& timer = *CRCore::Timer::instance();
	CRCore::Timer_t start_tick = timer.tick();
	double elapsedTime = 0.0;

	unsigned int noDeleted = 0;

	{
		CRCore::ScopedLock<crMutex> lock(crDrawable::s_mutex_deletedDisplayListCache);

		crDrawable::DeletedDisplayListCache::iterator citr = crDrawable::s_deletedDisplayListCache.find(contextID);
		if (citr!=crDrawable::s_deletedDisplayListCache.end())
		{
			crDrawable::DisplayListMap& dll = citr->second;


			bool trimFromFront = true;
			if (trimFromFront)
			{
				unsigned int prev_size = dll.size();

				crDrawable::DisplayListMap::iterator ditr=dll.begin();
				unsigned int maxNumToDelete = (dll.size() > crDrawable::s_minimumNumberOfDisplayListsToRetainInCache) ? dll.size()-crDrawable::s_minimumNumberOfDisplayListsToRetainInCache : 0;
				for(;
					ditr!=dll.end() && elapsedTime<availableTime && noDeleted<maxNumToDelete;
					++ditr)
				{
					glDeleteLists(ditr->second,1);

					elapsedTime = timer.delta_s(start_tick,timer.tick());
					++noDeleted;

					++crDrawable::s_numberDeletedDrawablesInLastFrame;
				}

				if (ditr!=dll.begin()) dll.erase(dll.begin(),ditr);

				if (noDeleted+dll.size() != prev_size)
				{
					CRCore::notify(CRCore::WARN)<<"Error in delete"<<std::endl;
				}    
			}
			else
			{
				unsigned int prev_size = dll.size();

				crDrawable::DisplayListMap::reverse_iterator ditr=dll.rbegin();
				unsigned int maxNumToDelete = (dll.size() > crDrawable::s_minimumNumberOfDisplayListsToRetainInCache) ? dll.size()-crDrawable::s_minimumNumberOfDisplayListsToRetainInCache : 0;
				for(;
					ditr!=dll.rend() && elapsedTime<availableTime && noDeleted<maxNumToDelete;
					++ditr)
				{
					glDeleteLists(ditr->second,1);

					elapsedTime = timer.delta_s(start_tick,timer.tick());
					++noDeleted;

					++crDrawable::s_numberDeletedDrawablesInLastFrame;
				}

				if (ditr!=dll.rbegin()) dll.erase(ditr.base(),dll.end());

				if (noDeleted+dll.size() != prev_size)
				{
					CRCore::notify(CRCore::WARN)<<"Error in delete"<<std::endl;
				}    
			}
		}
	}
	elapsedTime = timer.delta_s(start_tick,timer.tick());

	// if (noDeleted) notify(NOTICE)<<"Number display lists deleted = "<<noDeleted<<" elapsed time"<<elapsedTime<<std::endl;

	availableTime -= elapsedTime;
}


/** flush all the cached display list which need to be deleted
* in the OpenGL context related to contextID.*/
//typedef void (APIENTRY * DeleteBuffersProc) (GLsizei n, const GLuint *buffers);
void crDrawable::flushDeletedVertexBufferObjects(unsigned int contextID,double /*currentTime*/, double& availableTime)
{
	//static DeleteBuffersProc glDeleteBuffers = (DeleteBuffersProc)getGLExtensionFuncPtr("glDeleteBuffers","glDeleteBuffersARB");
	// if no time available don't try to flush objects.
	if (availableTime<=0.0) return;
	const GL2Extensions* extensions = GL2Extensions::Get(contextID,true);
	const CRCore::Timer& timer = *CRCore::Timer::instance();
	CRCore::Timer_t start_tick = timer.tick();
	double elapsedTime = 0.0;

	{
		CRCore::ScopedLock<crMutex> lock(crDrawable::s_mutex_deletedVertexBufferObjectCache);

		crDrawable::DeletedDisplayListCache::iterator citr = crDrawable::s_deletedVertexBufferObjectCache.find(contextID);
		if (citr!=crDrawable::s_deletedVertexBufferObjectCache.end())
		{
			//const Extensions* extensions = getExtensions(contextID,true);

			unsigned int noDeleted = 0;

			crDrawable::DisplayListMap& dll = citr->second;

			crDrawable::DisplayListMap::iterator ditr=dll.begin();
			for(;
				ditr!=dll.end() && elapsedTime<availableTime;
				++ditr)
			{
				extensions->glDeleteBuffers(1,&(ditr->second));
				elapsedTime = timer.delta_s(start_tick,timer.tick());
				++noDeleted;
			}
			if (ditr!=dll.begin()) dll.erase(dll.begin(),ditr);

			if (noDeleted!=0)
			{
				char gbuf[256];
				sprintf(gbuf,"VBOs deleted:%d\n\0",noDeleted);
				gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
				//notify(CRCore::INFO)<<"Number VBOs deleted = "<<noDeleted<<std::endl;
			}
		}
	}    

	availableTime -= elapsedTime;
}
void crDrawable::flushAllDeletedVertexBufferObjects(unsigned int contextID)
{
	//static DeleteBuffersProc glDeleteBuffers = (DeleteBuffersProc)getGLExtensionFuncPtr("glDeleteBuffers","glDeleteBuffersARB");
	const GL2Extensions* extensions = GL2Extensions::Get(contextID,true);
	{
		CRCore::ScopedLock<crMutex> lock(crDrawable::s_mutex_deletedVertexBufferObjectCache);

		crDrawable::DeletedDisplayListCache::iterator citr = crDrawable::s_deletedVertexBufferObjectCache.find(contextID);
		if (citr!=crDrawable::s_deletedVertexBufferObjectCache.end())
		{
			//const Extensions* extensions = getExtensions(contextID,true);

			unsigned int noDeleted = 0;

			crDrawable::DisplayListMap& dll = citr->second;

			crDrawable::DisplayListMap::iterator ditr=dll.begin();
			for(;
				ditr!=dll.end();
				++ditr)
			{
				extensions->glDeleteBuffers(1,&(ditr->second));
				++noDeleted;
			}
			if (ditr!=dll.begin()) dll.erase(dll.begin(),ditr);

			if (noDeleted!=0)
			{
				char gbuf[256];
				sprintf(gbuf,"Number VBOs deleted:%d\n\0",noDeleted);
				gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
				//notify(CRCore::INFO)<<"Number VBOs deleted = "<<noDeleted<<std::endl;
			}
		}
	}    
}