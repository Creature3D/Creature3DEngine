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
#if defined(WIN32) && !defined(__CYGWIN__)
    #include <Io.h>
    #include <Windows.h>
    #include <Winbase.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <direct.h> // for _mkdir
    
    #define mkdir(x,y) _mkdir((x))
    #define stat64 _stati64

    // set up for windows so acts just like unix access().
    #define F_OK 4


#else // unix

#if defined( __APPLE__ ) || defined(__CYGWIN__) || defined(__FreeBSD__)
    #define stat64 stat
#endif

    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
#endif

    // set up _S_ISDIR()
#if !defined(S_ISDIR)
#  if defined( _S_IFDIR) && !defined( __S_IFDIR)
#    define __S_IFDIR _S_IFDIR
#  endif
#  define S_ISDIR(mode)    (mode&__S_IFDIR)
#endif

#include <CRCore/crNotify.h>

#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crFileNameUtils.h>
#include <CRIOManager/crRegistry.h>

#include <errno.h>
#include <stack>

#include <fstream>
#include <iostream>
#include <cstdio>

bool CRIOManager::copyFile(const std::string &srcFile, const std::string &destFile)
{
	if(fileExists(srcFile))
	{
		std::string src = convertFileNameToUnixStyle(srcFile);
		if(src.find("./") == 0)
		{
			src.erase(0,2);
		}
		std::string dest = convertFileNameToUnixStyle(destFile);
		if(dest.find("./") == 0)
		{
			dest.erase(0,2);
		}
		if(src.compare(dest) != 0)
		{
			makeDirectoryForFile(destFile);
			std::ofstream out(destFile.c_str(),std::ios::binary|std::ios::out);
			if(!out)
			{
				char buf[128];
				sprintf(buf,"错误:文件拷贝失败[%s],请确认文件是否被锁定(比如被杀毒软件错误隔离或者Creature3D进程尚未退出)。\0",destFile.c_str());
				MessageBox(::GetDesktopWindow(),buf,"Creature3D",MB_OK);
				//CRCore::notify(CRCore::WARN) <<buf<< std::endl;
				return false;
			}
			else
			{
				std::ifstream in(srcFile.c_str(),std::ios::binary|std::ios::in);
				out << in.rdbuf();
				out.close();
				in.close();
				return true;
			}
		}
	}
	return false;
}

bool CRIOManager::makeDirectory( const std::string &path )
{
    if (path.empty())
    {
        //CRCore::notify(CRCore::DEBUG_INFO) << "CRIOManager::makeDirectory(): cannot create an empty directory" << std::endl;
        return false;
    }
    
    struct stat64 stbuf;
    if( stat64( path.c_str(), &stbuf ) == 0 )
    {
        if( S_ISDIR(stbuf.st_mode))
            return true;
        else
        {
            //CRCore::notify(CRCore::DEBUG_INFO) << "CRIOManager::makeDirectory(): "  << 
            //        path << " already exists and is not a directory!" << std::endl;
            return false;
        }
    }

    std::string dir = path;
    std::stack<std::string> paths;
    while( true )
    {
        if( dir.empty() )
            break;
 
        if( stat64( dir.c_str(), &stbuf ) < 0 )
        {
            switch( errno )
            {
                case ENOENT:
                case ENOTDIR:
                    paths.push( dir );
                    break;
 
                default:
                    //CRCore::notify(CRCore::DEBUG_INFO) << "CRIOManager::makeDirectory(): "  << strerror(errno) << std::endl;
                    return false;
            }
        }
        dir = getFilePath(std::string(dir));
    }

    while( !paths.empty() )
    {
        std::string dir = paths.top();
 
        if( mkdir( dir.c_str(), 0755 )< 0 )
        {
            //CRCore::notify(CRCore::DEBUG_INFO) << "CRIOManager::makeDirectory(): "  << strerror(errno) << std::endl;
            return false;
        } 
        paths.pop();
    }
    return true;
}

bool CRIOManager::makeDirectoryForFile( const std::string &path )
{
    return makeDirectory( getFilePath( path ));
}

bool CRIOManager::removeDirectory( const std::string &path )
{
	if (!path.empty())
	{
		//rmdir(path.c_str());
		char buf[256];
		sprintf(buf,"rd/s/q %s\0",convertFileNameToNativeStyle(path).c_str());
		system(buf);
		//WinExec(buf,SW_HIDE);
	}
	return true;
}
void CRIOManager::convertStringPathIntoFilePathList(const std::string& paths,FilePathList& filepath)
{
#if defined(WIN32) && !defined(__CYGWIN__)
    char delimitor = ';';
#else
    char delimitor = ':';
#endif

    if (!paths.empty())
    {
        std::string::size_type start = 0;
        std::string::size_type end;
        while ((end = paths.find_first_of(delimitor,start))!=std::string::npos)
        {
            filepath.push_back(std::string(paths,start,end-start));
            start = end+1;
        }

        filepath.push_back(std::string(paths,start,std::string::npos));
    }
 
}

bool CRIOManager::fileExists(const std::string& filename)
{
    return access( filename.c_str(), F_OK ) == 0;
}

CRIOManager::FileType CRIOManager::fileType(const std::string& filename)
{
    struct stat64 fileStat;
    if ( stat64(filename.c_str(), &fileStat) != 0 ) 
    {
        return FILE_NOT_FOUND;
    } // end if

    if ( fileStat.st_mode & S_IFDIR )
        return DIRECTORY;
    else if ( fileStat.st_mode & S_IFREG )
        return REGULAR_FILE;

    return FILE_NOT_FOUND;
}

std::string CRIOManager::findFileInPath(const std::string& filename, const FilePathList& filepath,CaseSensitivity caseSensitivity)
{
    if (filename.empty()) 
        return filename;

    if (!isFileNameNativeStyle(filename)) 
        return findFileInPath(convertFileNameToNativeStyle(filename), filepath, caseSensitivity);


    for(FilePathList::const_iterator itr=filepath.begin();
        itr!=filepath.end();
        ++itr)
    {
        //CRCore::notify(CRCore::DEBUG_INFO) << "itr='" <<*itr<< "'\n";
        std::string path = itr->empty() ? filename : *itr + '/'+ filename;
        //CRCore::notify(CRCore::DEBUG_INFO) << "FindFileInPath() : trying " << path << " ...\n";
        if(fileExists(path)) 
        {
            //CRCore::notify(CRCore::DEBUG_INFO) << "FindFileInPath() : USING " << path << "\n";
            return path;
        }
#ifndef WIN32 
// windows already case insensitive so no need to retry..
        else if (caseSensitivity==CASE_INSENSITIVE) 
        {
            std::string foundfile = findFileInDirectory(filename,*itr,CASE_INSENSITIVE);
            if (!foundfile.empty()) return foundfile;
        }
#endif
            
    }

    return std::string();
}


std::string CRIOManager::findDataFile(const std::string& filename,CaseSensitivity caseSensitivity)
{
    return findDataFile(filename,static_cast<crReaderWriter::Options*>(0),caseSensitivity);
}

CRIOMANAGER_EXPORT std::string CRIOManager::findDataFile(const std::string& filename,const crReaderWriter::Options* options, CaseSensitivity caseSensitivity)
{
    if (filename.empty()) return filename;
    
    if(fileExists(filename)) 
    {
       // CRCore::notify(CRCore::DEBUG_INFO) << "FindFileInPath(" << filename << "): returning " << filename << std::endl;
        return filename;
    }

    std::string fileFound;
    
    if (options && !options->getDatabasePathList().empty())
    {
        fileFound = findFileInPath(filename, options->getDatabasePathList(), caseSensitivity);
        if (!fileFound.empty()) return fileFound;
    }

    const FilePathList& filepath = crRegistry::instance()->getDataFilePathList();
    if (!filepath.empty())
    {
        fileFound = findFileInPath(filename, filepath,caseSensitivity);
        if (!fileFound.empty()) return fileFound;
    }
    

    // if a directory is included in the filename, get just the (simple) filename itself and try that
    std::string simpleFileName = getSimpleFileName(filename);
    if (simpleFileName!=filename)
    {

        if(fileExists(simpleFileName)) 
        {
            //CRCore::notify(CRCore::DEBUG_INFO) << "FindFileInPath(" << filename << "): returning " << filename << std::endl;
            return simpleFileName;
        }

        if (options && !options->getDatabasePathList().empty())
        {
            fileFound = findFileInPath(simpleFileName, options->getDatabasePathList(), caseSensitivity);
            if (!fileFound.empty()) return fileFound;
        }

        if (!filepath.empty())
        {
            fileFound = findFileInPath(simpleFileName, filepath,caseSensitivity);
            if (!fileFound.empty()) return fileFound;
        }

    }

    // return empty string.
    return std::string();
}

std::string CRIOManager::findLibraryFile(const std::string& filename,CaseSensitivity caseSensitivity)
{
    if (filename.empty()) 
        return filename; 

    const FilePathList& filepath = crRegistry::instance()->getLibraryFilePathList();

    std::string fileFound = findFileInPath(filename, filepath,caseSensitivity);
    if (!fileFound.empty()) 
        return fileFound;

    if(fileExists(filename)) 
    {
        //CRCore::notify(CRCore::DEBUG_INFO) << "FindFileInPath(" << filename << "): returning " << filename << std::endl;
        return filename;
    }

    // if a directory is included in the filename, get just the (simple) filename itself and try that
    std::string simpleFileName = getSimpleFileName(filename);
    if (simpleFileName!=filename)
    {
        std::string fileFound = findFileInPath(simpleFileName, filepath,caseSensitivity);
        if (!fileFound.empty()) return fileFound;
    }

    // failed with direct paths,
    // now try prepending the filename with "Plugins/"
    return findFileInPath("Plugins/"+simpleFileName,filepath,caseSensitivity);
}

std::string CRIOManager::findFileInDirectory(const std::string& fileName,const std::string& dirName,CaseSensitivity caseSensitivity)
{
    bool needFollowingBackslash = false;
    bool needDirectoryName = true;
    CRIOManager::DirectoryContents dc;

    if (dirName.empty())
    {
        dc = CRIOManager::getDirectoryContents(".");
        needFollowingBackslash = false;
        needDirectoryName = false;
    }
    else if (dirName=="." || dirName=="./" || dirName==".\\")
    {
        dc = CRIOManager::getDirectoryContents(".");
        needFollowingBackslash = false;
        needDirectoryName = false;
    }
    else
    {
        dc = CRIOManager::getDirectoryContents(dirName);
        char lastChar = dirName[dirName.size()-1];
        if (lastChar=='/') needFollowingBackslash = false;
        else if (lastChar=='\\') needFollowingBackslash = false;
        else needFollowingBackslash = true;
        needDirectoryName = true;
    }

    for(CRIOManager::DirectoryContents::iterator itr=dc.begin();
        itr!=dc.end();
        ++itr)
    {
        if ((caseSensitivity==CASE_INSENSITIVE && CRIOManager::equalCaseInsensitive(fileName,*itr)) ||
            (fileName==*itr))
        {
            if (!needDirectoryName) return *itr;
            else if (needFollowingBackslash) return dirName+'/'+*itr;
            else return dirName+*itr;
        }
    }
    return "";
}

#if defined(WIN32) && !defined(__CYGWIN__)
    #include <io.h>
    #include <direct.h>

    CRIOManager::DirectoryContents CRIOManager::getDirectoryContents(const std::string& dirName)
    {
        CRIOManager::DirectoryContents contents;

        WIN32_FIND_DATA data;
        HANDLE handle = FindFirstFile((dirName + "\\*").c_str(), &data);
        if (handle != INVALID_HANDLE_VALUE)
        {
            do
            {
                contents.push_back(data.cFileName);
            }
            while (FindNextFile(handle, &data) != 0);

            FindClose(handle);
        }
        return contents;
    }

#else

    #include <dirent.h>
    CRIOManager::DirectoryContents CRIOManager::getDirectoryContents(const std::string& dirName)
    {
        CRIOManager::DirectoryContents contents;

        DIR *handle = opendir(dirName.c_str());
        if (handle)
        {
            dirent *rc;
            while((rc = readdir(handle))!=NULL)
            {
                contents.push_back(rc->d_name);
            }
            closedir(handle);
        }

        return contents;
    }

#endif // unix getDirectoryContexts


/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Implementation of appendPlatformSpecificLibraryFilePaths(..)
//
#ifdef __sgi

    void CRIOManager::appendPlatformSpecificLibraryFilePaths(FilePathList& filepath)
    {
        convertStringPathIntoFilePathList("/usr/lib32/:/usr/local/lib32/",filepath);

        // bloody mess see rld(1) man page
        char* ptr;

        #if (_MIPS_SIM == _MIPS_SIM_ABI32)
        if( (ptr = getenv( "LD_LIBRARY_PATH" )))
        {
            convertStringPathIntoFilePathList(ptr,filepath);
        }

        #elif (_MIPS_SIM == _MIPS_SIM_NABI32)

        if( !(ptr = getenv( "LD_LIBRARYN32_PATH" )))
            ptr = getenv( "LD_LIBRARY_PATH" );

        if( ptr )
        {
            convertStringPathIntoFilePathList(ptr,filepath);
        }

        #elif (_MIPS_SIM == _MIPS_SIM_ABI64)

        if( !(ptr = getenv( "LD_LIBRARY64_PATH" )))
            ptr = getenv( "LD_LIBRARY_PATH" );

        if( ptr )
        {
            convertStringPathIntoFilePathList(ptr,filepath);
        }
        #endif
    }


#elif defined(__CYGWIN__)

    void CRIOManager::appendPlatformSpecificLibraryFilePaths(FilePathList& filepath)
    {
        char* ptr;
        if ((ptr = getenv( "PATH" )))
        {
            convertStringPathIntoFilePathList(ptr,filepath);
        }

        convertStringPathIntoFilePathList("/usr/bin/:/usr/local/bin/",filepath);

    }
    
#elif defined(WIN32)

    void CRIOManager::appendPlatformSpecificLibraryFilePaths(FilePathList& filepath)
    {
        char* ptr;
        if ((ptr = getenv( "PATH" )))
        {
            convertStringPathIntoFilePathList(ptr,filepath);
        }

        convertStringPathIntoFilePathList("C:/Windows/System/",filepath);
    }
    
#elif defined(__APPLE__)

    // #define COMPILE_COCOA_VERSION
    #define COMPILE_CARBON_VERSION
    

    // The Cocoa version is about 10 lines of code.
    // The Carbon version is noticably longer.
    // Unfortunately, the Cocoa version requires -lobjc to be 
    // linked in when creating an executable. 
    // Rumor is that this will be done autmatically in gcc 3.5/Tiger,
    // but for now, this will cause a lot of headaches for people
    // who aren't familiar with this concept, so the Carbon version 
    // is preferable.
    // But for the curious, both implementations are here.
    // Note that if the Cocoa version is used, the file should be 
    // renamed to use the .mm extension to denote Objective-C++.
    // And of course, you will need to link against Cocoa

    #ifdef COMPILE_COCOA_VERSION
    #include <Foundation/Foundation.h>
    // OS X has preferred locations for where PlugIns should be located.
    // This function will set this as the order to search:
    // YourProgram.app/Contents/PlugIns
    // ~/Library/Application Support/OpenSceneGraph/PlugIns
    // /Library/Application Support/OpenSceneGraph/PlugIns
    // /Network/Library/Application Support/OpenSceneGraph/PlugIns
    // 
    // As a side effect of this function, if the application is not a 
    // bundle, the first place searched becomes
    // YourProgram/PlugIns
    //
    // In principle, these other directories should be searched:
    // ~/Library/Application Support/YourProgram/PlugIns
    // /Library/Application Support/YourProgram/PlugIns
    // /Network/Library/Application Support/TheProgram/PlugIns 
    // But I'm not going to worry about it for now because the 
    // bundle's PlugIns directory is supposed to be the preferred 
    // place for this anyway.
    //
    // Another directory that might be worth considering is
    // the directory the program resides in,
    // but I'm worried about multiplatform distribution.
    // Because .so is used by other platforms like Linux, we 
    // could end up loading the wrong binary.
    // I'm not sure how robust the current code is for this case.
    // Assuming the program doesn't crash, will OSG move on to the 
    // next search directory, or just give up?
    void CRIOManager::appendPlatformSpecificLibraryFilePaths(FilePathList& filepath)
    {
        char* ptr;
        if ((ptr = getenv( "DYLD_LIBRARY_PATH" )) )
        {
            convertStringPathIntoFilePathList(ptr, filepath);
        }

        // Since this is currently the only Objective-C code in the
        // library, we need an autoreleasepool for obj-c memory management.
        // If more Obj-C is added, we might move this pool to another 
        // location so it can be shared. Pools seem to be stackable,
        // so I don't think there will be a problem if multiple pools
        // exist at a time.
        NSAutoreleasePool* mypool = [[NSAutoreleasePool alloc] init];

        NSString* myBundlePlugInPath;
        NSString* userSupportDir;

        // This will grab the "official" bundle plug in path.
        // It will be YourProgram.app/Contents/PlugIns (for App bundles)
        // or YourProgram/PlugIns (for Unix executables)
        myBundlePlugInPath = [[NSBundle mainBundle] builtInPlugInsPath];

        // Now setup the other search paths
        // Cocoa has a nice method for tilde expansion.
        // There's probably a better way of getting this directory, but I 
        // can't find the call.
        userSupportDir = [@"~/Library/Application Support/Creature3D/PlugIns" stringByExpandingTildeInPath];

        // Can setup the remaining directories directly in C++

        // Since Obj-C and C++ objects don't understand each other,
        // the Obj-C strings must be converted down to C strings so
        // C++ can make them into C++ strings.
        filepath.push_back( [myBundlePlugInPath UTF8String] );
        filepath.push_back( [userSupportDir UTF8String] );

        filepath.push_back( "/Library/Application Support/Creature3D/PlugIns" );
        filepath.push_back( "/Network/Library/Application Support/Creature3D/PlugIns" );

        // Clean up the autorelease pool
        [mypool release];
    }

    #elif defined(COMPILE_CARBON_VERSION)


    #include <CoreServices/CoreServices.h>
    // OS X has preferred locations for where PlugIns should be located.
    // This function will set this as the order to search:
    // YourProgram.app/Contents/PlugIns
    // ~/Library/Application Support/OpenSceneGraph/PlugIns
    // /Library/Application Support/OpenSceneGraph/PlugIns
    // /Network/Library/Application Support/OpenSceneGraph/PlugIns
    // 
    // As a side effect of this function, if the application is not a 
    // bundle, the first place searched becomes
    // YourProgram/PlugIns
    //
    // In principle, these other directories should be searched:
    // ~/Library/Application Support/YourProgram/PlugIns
    // /Library/Application Support/YourProgram/PlugIns
    // /Network/Library/Application Support/TheProgram/PlugIns 
    // But I'm not going to worry about it for now because the 
    // bundle's PlugIns directory is supposed to be the preferred 
    // place for this anyway.
    //
    // Another directory that might be worth considering is
    // the directory the program resides in,
    // but I'm worried about multiplatform distribution.
    // Because .so is used by other platforms like Linux, we 
    // could end up loading the wrong binary.
    // I'm not sure how robust the current code is for this case.
    // Assuming the program doesn't crash, will OSG move on to the 
    // next search directory, or just give up?
    void CRIOManager::appendPlatformSpecificLibraryFilePaths(FilePathList& filepath)
    {
        char* ptr;
        if ((ptr = getenv( "DYLD_LIBRARY_PATH" )) )
        {
            convertStringPathIntoFilePathList(ptr, filepath);
        }

        const int MAX_OSX_PATH_SIZE = 1024;
        const std::string CRE_PLUGIN_PATH("/Creature3D/PlugIns");
        char buffer[MAX_OSX_PATH_SIZE];
        char bundlePathBuffer[MAX_OSX_PATH_SIZE];
        CFURLRef  url;
        CFStringRef pathString;
        CFBundleRef myBundle;
        CFStringRef bundlePathString;
        FSRef     f;
        OSErr	errCode;

        // Start with the the Bundle PlugIns directory.
        // Unlike the Cocoa API, it seems that the PlugIn path is relative
        // and not absolute. So I will need to fetch both the bundle path
        // (which is absolute) and the PlugIn path (which is relative),
        // and combine the two to form a full path.

        // Get the bundle first
        myBundle = CFBundleGetMainBundle();
        if(myBundle != NULL)
        {
            // Get the URL to the bundle
            url = CFBundleCopyBundleURL( myBundle );

            // Convert the URL to a CFString that looks like a Unix file path
            bundlePathString = CFURLCopyFileSystemPath( url, kCFURLPOSIXPathStyle );
            // Convert the CFString to a C string
            CFStringGetCString( bundlePathString, bundlePathBuffer, MAX_OSX_PATH_SIZE, kCFStringEncodingUTF8 );

            CFRelease( url );

            // Now find the PlugIns directory
            // Get the URL to the bundle
            url = CFBundleCopyBuiltInPlugInsURL( myBundle );
            //pathString = CFURLCopyPath( url );
            // Convert the URL to a CFString that looks like a Unix file path
            pathString = CFURLCopyFileSystemPath( url, kCFURLPOSIXPathStyle );
            // Convert the CFString to a C string
            CFStringGetCString( pathString, buffer, MAX_OSX_PATH_SIZE, kCFStringEncodingUTF8 );

            // Combine the string and copy it into the FilePath list
            filepath.push_back( 
                std::string(bundlePathBuffer) 
                + std::string("/")
                + std::string(buffer)
            );

            CFRelease( pathString );
            CFRelease( bundlePathString );
            CFRelease( url );

            // I was getting random crashes which I believe were caused by
            // releasing the bundle. The documentation says I'm responsible
            // for retaining and releasing which didn't immediately register
            // in my head. I never retain the bundle, so I don't release it.
            // CFRelease( myBundle );

            pathString = NULL;
            bundlePathString = NULL;
            url = NULL;
            // myBundle = NULL;
        }
       // else
        //{
            //CRCore::notify( CRCore::DEBUG_INFO ) << "Couldn't find the Application Bundle" << std::endl;
       // }

        // Next, check the User's Application Support folder
        errCode = FSFindFolder( kUserDomain, kApplicationSupportFolderType, kDontCreateFolder, &f );
        if(noErr == errCode)
        {
            // Get the URL
            url = CFURLCreateFromFSRef( 0, &f );
            // Convert the URL to a CFString that looks like a Unix file path
            pathString = CFURLCopyFileSystemPath( url, kCFURLPOSIXPathStyle );
            // Convert the CFString to a C string
            CFStringGetCString( pathString, buffer, MAX_OSX_PATH_SIZE, kCFStringEncodingUTF8 );

            // Add the directory to the FilePathList
            filepath.push_back( 
                std::string(buffer) 
                + CRE_PLUGIN_PATH
            );

            CFRelease( url );
            CFRelease( pathString );
        }
        //else
        //{
        //    CRCore::notify( CRCore::DEBUG_INFO ) << "Couldn't find the User's Application Support Path" << std::endl;
        //}

        // Next, check the Local System's Application Support Folder
        errCode = FSFindFolder( kLocalDomain, kApplicationSupportFolderType, kDontCreateFolder, &f );
        if(noErr == errCode)
        {
            // Get the URL
            url = CFURLCreateFromFSRef( 0, &f );
            // Convert the URL to a CFString that looks like a Unix file path
            pathString = CFURLCopyFileSystemPath( url, kCFURLPOSIXPathStyle );
            // Convert the CFString to a C string
            CFStringGetCString( pathString, buffer, MAX_OSX_PATH_SIZE, kCFStringEncodingUTF8 );

            // Add the directory to the FilePathList
            filepath.push_back( 
                std::string(buffer) 
                + CRE_PLUGIN_PATH
            );

            CFRelease( url );
            CFRelease( pathString );
        }
        //else
        //{
        //    CRCore::notify( CRCore::DEBUG_INFO ) << "Couldn't find the Local System's Application Support Path" << std::endl;
       // }

        // Finally, check the Network Application Support Folder
        // This one has a likely chance of not existing so an error
        // may be returned. Don't panic.
        errCode = FSFindFolder( kNetworkDomain, kApplicationSupportFolderType, kDontCreateFolder, &f );
        if(noErr == errCode)
        {
            // Get the URL
            url = CFURLCreateFromFSRef( 0, &f );
            // Convert the URL to a CFString that looks like a Unix file path
            pathString = CFURLCopyFileSystemPath( url, kCFURLPOSIXPathStyle );
            // Convert the CFString to a C string
            CFStringGetCString( pathString, buffer, MAX_OSX_PATH_SIZE, kCFStringEncodingUTF8 );

            // Add the directory to the FilePathList
            filepath.push_back( 
                std::string(buffer) 
                + CRE_PLUGIN_PATH
            );

            CFRelease( url );
            CFRelease( pathString );
        }
       // else
       // {
	    // had to comment out as it segfauls the OSX app otherwise
            // CRCore::notify( CRCore::DEBUG_INFO ) << "Couldn't find the Network Application Support Path" << std::endl;
       // }
    }
    #else
    void CRIOManager::appendPlatformSpecificLibraryFilePaths(FilePathList& filepath)
    {
        char* ptr;
        if ((ptr = getenv( "DYLD_LIBRARY_PATH" )) )
        {
            convertStringPathIntoFilePathList(ptr, filepath);
        }
    }
    #endif
    
#else   

    void CRIOManager::appendPlatformSpecificLibraryFilePaths(FilePathList& filepath)
    {

       char* ptr;
       if( (ptr = getenv( "LD_LIBRARY_PATH" )) )
        {
            convertStringPathIntoFilePathList(ptr,filepath);
        }

        convertStringPathIntoFilePathList("/usr/lib/:/usr/local/lib/",filepath);

    }

#endif


#ifdef __APPLE__
	void CRIOManager::appendPlatformSpecificResourceFilePaths(FilePathList& filepath)
	{
		// Get the main application bundle
		CFBundleRef mainBundle = CFBundleGetMainBundle();

		if (mainBundle != NULL) {
			// Get the parent directory and the resources directory
			std::string bundlePath = GetApplicationBundlePath(mainBundle);
			std::string resourcesPath = GetApplicationResourcesPath(mainBundle);

			// check if application is really part of a .app bundle
			if(bundlePath.substr(bundlePath.length() - 4, 4) == std::string(".app"))
			{
				if(resourcesPath != std::string(""))
					filepath.push_back( resourcesPath );

				std::string parentPath = GetShortenedPath(bundlePath, 1);
				if(parentPath != std::string(""))
					filepath.push_back( parentPath );
			}
		}
		//else
		//{
		//	CRIOManager::notify( CRIOManager::DEBUG_INFO ) << "Couldn't find the Application Bundle." << std::endl;
		//}
	}
#else
	void CRIOManager::appendPlatformSpecificResourceFilePaths(FilePathList& /*filepath*/)
	{
	}
#endif
