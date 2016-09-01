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
#if defined(WIN32) && !defined(__CYGWIN__)
#include <Io.h>
#include <Windows.h>
#include <Winbase.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#else // all other unix
#include <unistd.h>
#ifdef __hpux__
// Although HP-UX has dlopen() it is broken! We therefore need to stick
// to shl_load()/shl_unload()/shl_findsym()
#include <dl.h>
#include <errno.h>
#else
#include <dlfcn.h>
#endif
#endif

#include <CRCore/crNotify.h>

#include <CRIOManager/crDynamicLibrary.h>
#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crFileNameUtils.h>

using namespace CRCore;
using namespace CRIOManager;

crDynamicLibrary::crDynamicLibrary(const std::string& name,HANDLE handle)
{
    m_name = name;
    m_handle = handle;
    //CRCore::notify(CRCore::NOTICE)<<"Opened crDynamicLibrary "<<m_name<<std::endl;
}

crDynamicLibrary::~crDynamicLibrary()
{
    if (m_handle)
    {
		//CRCore::notify(CRCore::NOTICE)<<"Closing crDynamicLibrary "<<m_name<<std::endl;
#if defined(WIN32) && !defined(__CYGWIN__)
        FreeLibrary((HMODULE)m_handle);
#elif defined(__APPLE__)
        NSUnLinkModule(static_cast<NSModule>(m_handle), FALSE);
#elif defined(__hpux__)
        // fortunately, shl_t is a pointer
        shl_unload (static_cast<shl_t>(m_handle));
#else // other unix
        dlclose(m_handle);
#endif    
    }
}

crDynamicLibrary* crDynamicLibrary::loadLibrary(const std::string& libraryName)
{
	//notify(INFO) << "LoadLibrary\""<<libraryName<<"\""<<std::endl;
    HANDLE handle = NULL;

    std::string fullLibraryName = CRIOManager::findLibraryFile(libraryName);
    if (!fullLibraryName.empty()) handle = getLibraryHandle( fullLibraryName ); // try the lib we have found
    else handle = getLibraryHandle( libraryName ); // havn't found a lib ourselves, see if the OS can find it simply from the library name.
    if (handle) return new crDynamicLibrary(libraryName,handle);

    // else no lib found so report errors.
    //notify(INFO) << "crDynamicLibrary::failed loading \""<<libraryName<<"\""<<std::endl;

    return NULL;
}

crDynamicLibrary::HANDLE crDynamicLibrary::getLibraryHandle( const std::string& libraryName)
{
    HANDLE handle = NULL;

#if defined(WIN32) && !defined(__CYGWIN__)
    handle = LoadLibrary( libraryName.c_str() );
#elif defined(__APPLE__)
    NSObjectFileImage image;
    // NSModule os_handle = NULL;
    if (NSCreateObjectFileImageFromFile(libraryName.c_str(), &image) == NSObjectFileImageSuccess) {
        // os_handle = NSLinkModule(image, libraryName.c_str(), TRUE);
        handle = NSLinkModule(image, libraryName.c_str(), TRUE);
        NSDestroyObjectFileImage(image);
    }
#elif defined(__hpux__)
    // BIND_FIRST is neccessary for some reason
    handle = shl_load ( libraryName.c_str(), BIND_DEFERRED|BIND_FIRST|BIND_VERBOSE, 0);
    return handle;
#else // other unix

    // dlopen will not work with files in the current directory unless
    // they are prefaced with './'  (DB - Nov 5, 2003).
    std::string localLibraryName;
    if( libraryName == CRIOManager::getSimpleFileName( libraryName ) )
        localLibraryName = "./" + libraryName;
    else
        localLibraryName = libraryName;
    handle = dlopen( localLibraryName.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if( handle == NULL )
        notify(NOTICE) << "crDynamicLibrary::getLibraryHandle( "<< libraryName << ") - dlopen(): " << dlerror() << std::endl;
#endif
    return handle;
}

crDynamicLibrary::PROC_ADDRESS crDynamicLibrary::getProcAddress(const std::string& procName)
{
    if (m_handle==NULL) return NULL;
#if defined(WIN32) && !defined(__CYGWIN__)
    return (crDynamicLibrary::PROC_ADDRESS)GetProcAddress( (HMODULE)m_handle,
                                                         procName.c_str() );
#elif defined(__APPLE__)
    std::string temp("_");
    NSSymbol symbol;
    temp += procName;	// Mac OS X prepends an underscore on function names
    symbol = NSLookupAndBindSymbol(temp.c_str());
    return NSAddressOfSymbol(symbol);
#elif defined(__hpux__)
    void* result = NULL;
    if (shl_findsym (reinterpret_cast<shl_t*>(&m_handle), procName.c_str(), TYPE_PROCEDURE, result) == 0)
    {
        return result;
    }
    else
    {
        notify(WARN) << "crDynamicLibrary::failed looking up " << procName << std::endl;
        notify(WARN) << "crDynamicLibrary::error " << strerror(errno) << std::endl;
        return NULL;
    }
#else // other unix
    void* sym = dlsym( m_handle,  procName.c_str() );
    if (!sym) {
        notify(WARN) << "crDynamicLibrary::failed looking up " << procName << std::endl;
        notify(WARN) << "crDynamicLibrary::error " << dlerror() << std::endl;
    }
    return sym;
#endif
    return NULL;
}
