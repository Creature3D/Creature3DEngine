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
#include <CRCore/crNotify.h>
#include <CRCore/crVertexProgram.h>
#include <CRCore/crState.h>
#include <CRCore/crTimer.h>

using namespace CRCore;

OpenThreads::Mutex    crVertexProgram::s_mutex_deletedVertexProgramObjectCache;
crVertexProgram::DeletedVertexProgramObjectCache crVertexProgram::s_deletedVertexProgramObjectCache;

void crVertexProgram::deleteVertexProgramObject(unsigned int contextID,unsigned int handle)
{
    if (handle!=0)
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(s_mutex_deletedVertexProgramObjectCache);

        // insert the handle into the cache for the appropriate context.
        s_deletedVertexProgramObjectCache[contextID].push_back(handle);
    }
}

crVertexProgram::crVertexProgram()
{
}


crVertexProgram::crVertexProgram(const crVertexProgram& vp,const crCopyOp& copyop):
    CRCore::crStateAttribute(vp,copyop)
{
    m_vertexProgram = vp.m_vertexProgram;

    for( LocalParamList::const_iterator itr = vp.m_programLocalParameters.begin(); 
        itr != vp.m_programLocalParameters.end(); ++itr )
    {
        m_programLocalParameters[itr->first] = itr->second;
    }

    for( MatrixList::const_iterator mitr = vp.m_matrixList.begin(); 
        mitr != vp.m_matrixList.end(); ++mitr )
    {
        m_matrixList[mitr->first] = mitr->second;
    }
}


// virtual
crVertexProgram::~crVertexProgram()
{
    dirtyVertexProgramObject();
}

void crVertexProgram::dirtyVertexProgramObject()
{
    for(unsigned int i=0;i<m_vertexProgramIDList.size();++i)
    {
        if (m_vertexProgramIDList[i] != 0)
        {
            crVertexProgram::deleteVertexProgramObject(i,m_vertexProgramIDList[i]);
            m_vertexProgramIDList[i] = 0;
        }
    }
}

void crVertexProgram::releaseGLObjects(crState* state) const
{
    if (!state) const_cast<crVertexProgram*>(this)->dirtyVertexProgramObject();
    else
    {
        unsigned int contextID = state->getContextID();
        if (m_vertexProgramIDList[contextID] != 0)
        {
            crVertexProgram::deleteVertexProgramObject(contextID,m_vertexProgramIDList[contextID]);
            m_vertexProgramIDList[contextID] = 0;
        }
    }
}