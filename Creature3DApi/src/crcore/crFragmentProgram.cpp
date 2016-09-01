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
//#include <CRCore/GLExtensions>
#include <CRCore/crFragmentProgram.h>
#include <CRCore/crState.h>
#include <CRCore/crTimer.h>

using namespace CRCore;

OpenThreads::Mutex  crFragmentProgram::s_mutex_deletedFragmentProgramObjectCache;
crFragmentProgram::DeletedFragmentProgramObjectCache crFragmentProgram::s_deletedFragmentProgramObjectCache;

void crFragmentProgram::deleteFragmentProgramObject(unsigned int contextID,unsigned int handle)
{
    if (handle!=0)
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(s_mutex_deletedFragmentProgramObjectCache);

        // insert the handle into the cache for the appropriate context.
        s_deletedFragmentProgramObjectCache[contextID].push_back(handle);
    }
}

crFragmentProgram::crFragmentProgram()
{
}


crFragmentProgram::crFragmentProgram(const crFragmentProgram& vp,const crCopyOp& copyop):
    CRCore::crStateAttribute(vp,copyop)
{
    m_fragmentProgram = vp.m_fragmentProgram;

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
crFragmentProgram::~crFragmentProgram()
{
    dirtyFragmentProgramObject();
}

void crFragmentProgram::dirtyFragmentProgramObject()
{
    for(unsigned int i=0;i<m_fragmentProgramIDList.size();++i)
    {
        if (m_fragmentProgramIDList[i] != 0)
        {
            crFragmentProgram::deleteFragmentProgramObject(i,m_fragmentProgramIDList[i]);
            m_fragmentProgramIDList[i] = 0;
        }
    }
}

void crFragmentProgram::releaseGLObjects(crState* state) const
{
    if (!state) const_cast<crFragmentProgram*>(this)->dirtyFragmentProgramObject();
    else
    {
        unsigned int contextID = state->getContextID();
        if (m_fragmentProgramIDList[contextID] != 0)
        {
            crFragmentProgram::deleteFragmentProgramObject(contextID,m_fragmentProgramIDList[contextID]);
            m_fragmentProgramIDList[contextID] = 0;
        }
    }
}