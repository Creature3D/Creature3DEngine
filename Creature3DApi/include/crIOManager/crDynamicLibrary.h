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
#ifndef CRIOMANAGER_DYNAMICLIBRARY
#define CRIOMANAGER_DYNAMICLIBRARY 1

#include <CRCore/Referenced.h>
#include <CRIOManager/crExport.h>

#include <string>


namespace CRIOManager {

/** crDynamicLibrary - encapsulates the loading and unloading of dynamic libraries,
    typically used for loading crReaderWriter plug-ins.
*/
class CRIOMANAGER_EXPORT crDynamicLibrary : public CRCore::Referenced
{
    public:

        typedef void*   HANDLE;
        typedef void*   PROC_ADDRESS;
        
        /** returns a pointer to a crDynamicLibrary object on successfully
          * opening of library returns NULL on failure.
          */
        static crDynamicLibrary* loadLibrary(const std::string& libraryName);

        /** return name of library stripped of path.*/
        const std::string& getName() const     { return m_name; }

        /** return name of library including full path to it.*/
        const std::string& getFullName() const { return m_fullName; }

        /** return handle to .dso/.dll dynamic library itself.*/
        HANDLE             getHandle() const   { return m_handle; }

        /** return address of function located in library.*/
        PROC_ADDRESS       getProcAddress(const std::string& procName);

    protected:

	/** get handle to library file */
	static HANDLE getLibraryHandle( const std::string& libraryName);

        /** disallow default constructor.*/
        crDynamicLibrary():CRCore::Referenced() {}
        /** disallow copy constructor.*/
        crDynamicLibrary(const crDynamicLibrary&):CRCore::Referenced() {}
        /** disallow copy operator.*/
        crDynamicLibrary& operator = (const crDynamicLibrary&) { return *this; }
        
        /** Disallow public construction so that users have to go
          * through loadLibrary() above which returns NULL on 
          * failure, a valid crDynamicLibrary object on success.
          */
        crDynamicLibrary(const std::string& name,HANDLE handle);
        ~crDynamicLibrary();

        HANDLE          m_handle;
        std::string     m_name;
        std::string     m_fullName;

};

}

#endif // __DYNAMIC_LIBRARY_H
