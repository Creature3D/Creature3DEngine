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
// Win32BarrierPrivateData.h - private data structure for barrier
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
#ifndef CRCORE_CRHandleHolder_H
#define CRCORE_CRHandleHolder_H

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

/************************************************************************/
/* Class that holds HANDLES ensuring proper destruction                 */
/* It is design decision to make this class noncopyable.                */    
/* It makes design much cleaner. If one wants to copy handle than one   */
/* can do hv.set(DuplicateHandle(....))                                 */
/************************************************************************/
namespace CRCore {

    class HandleHolder{
    private:
        HANDLE m_handle;

        inline void close(){
            if( m_handle != INVALID_HANDLE_VALUE) CloseHandle(m_handle);
            m_handle = INVALID_HANDLE_VALUE;
        };

        // copy constructor - disallow
        HandleHolder(const HandleHolder& rhs);

        // assignment operator - disallow
        HandleHolder& operator=(const HandleHolder& rhs );

    public:
        // constructor
        HandleHolder()
            :m_handle(INVALID_HANDLE_VALUE)
        {};
        
        // constructor from HANDLE  
        explicit HandleHolder(HANDLE h)
            :m_handle(h)
        {};

        // destructor - CloseHandle()
        ~HandleHolder()
        {
            close();
        };

        // accessor    
        const HANDLE& get() const { 
            return m_handle;
        }
        
        // mutator
        void set (HANDLE h) { 
            if( m_handle != INVALID_HANDLE_VALUE ) close();
            m_handle = h;
        }

        operator bool (){
            return m_handle != INVALID_HANDLE_VALUE && m_handle != NULL;
        };

    };

} // namespace




#endif
