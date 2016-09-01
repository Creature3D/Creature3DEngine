/* -*-c++-*- OpenThreads library, Copyright (C) 2002 - 2007  The Open Thread Group
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
//
//
// Win32Condition.c++ - C++ crCondition class built on top of posix threads.
// ~~~~~~~~~~~~~~~~~~~~
//
#include <CRCore/thread/crCondition.h>
#include <CRCore/thread/crThread.h>
#include <CRCore/thread/crWin32ConditionPrivateData.h>

using namespace CRCore;
Win32ConditionPrivateData::~Win32ConditionPrivateData()
{
}

//----------------------------------------------------------------------------
//
// Decription: Constructor
//
// Use: public.
//
crCondition::crCondition() 
{
    Win32ConditionPrivateData *pd =
        new Win32ConditionPrivateData();
    m_prvData = static_cast<void *>(pd);
}
//----------------------------------------------------------------------------
//
// Decription: Destructor
//
// Use: public.
//
crCondition::~crCondition() {
    Win32ConditionPrivateData *pd =
       static_cast<Win32ConditionPrivateData *>(m_prvData);

    delete pd;
}

//----------------------------------------------------------------------------
//
// Decription: wait on a condition
//
// Use: public.
//
int crCondition::wait(crMutex *mutex) {

    Win32ConditionPrivateData *pd =
        static_cast<Win32ConditionPrivateData *>(m_prvData);

    return pd->wait(*mutex, INFINITE);
}
//----------------------------------------------------------------------------
//
// Decription: wait on a condition, for a specified period of time
//
// Use: public.
//
int crCondition::wait(crMutex *mutex, unsigned long ms) 
{
    Win32ConditionPrivateData *pd =
        static_cast<Win32ConditionPrivateData *>(m_prvData);

    return pd->wait(*mutex, ms);
}
//----------------------------------------------------------------------------
//
// Decription: signal a thread to wake up.
//
// Use: public.
//
int crCondition::signal() 
{
    Win32ConditionPrivateData *pd =
        static_cast<Win32ConditionPrivateData *>(m_prvData);
    return pd->signal();
}
//----------------------------------------------------------------------------
//
// Decription: signal many threads to wake up.
//
// Use: public.
//
int crCondition::broadcast() 
{
    Win32ConditionPrivateData *pd =
        static_cast<Win32ConditionPrivateData *>(m_prvData);
    return pd->broadcast();
}
bool crCondition::haswaiters()
{
	Win32ConditionPrivateData *pd =
		static_cast<Win32ConditionPrivateData *>(m_prvData);
	return pd->haswaiters();
}