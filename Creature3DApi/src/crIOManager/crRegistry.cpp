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
//Modified by 吴财华
#include <CRCore/crNotify.h>
#include <CRCore/crBase.h>
#include <CRCore/crImage.h>
#include <CRCore/crNode.h>
#include <CRCore/crGroup.h>
#include <CRCore/crObject.h>
#include <CRCore/crBrain.h>
#include <CRCore/crStreamBuf.h>
#include <CRIOManager/crRegistry.h>
#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crFileNameUtils.h>
#include <CRIOManager/crArchive.h>

#include <stdio.h>

#include <algorithm>
#include <set>

using namespace CRCore;
using namespace CRIOManager;
ref_ptr<crRegistry> crRegistry::m_instance = NULL;

class crRegistry::AvailableReaderWriterIterator
{
public:
    AvailableReaderWriterIterator(crRegistry::ReaderWriterList& rwList):
        m_rwList(rwList) {}


    crReaderWriter& operator * () { return *get(); }
    crReaderWriter* operator -> () { return get(); }
    
    bool valid() { return get()!=0; }
    
    void operator ++() 
    {
        m_rwUsed.insert(get());
    }
    

protected:

    crRegistry::ReaderWriterList&     m_rwList;
    std::set<crReaderWriter*>         m_rwUsed;

public:
    crReaderWriter* get() 
    {
        crRegistry::ReaderWriterList::iterator itr=m_rwList.begin();
        for(;itr!=m_rwList.end();++itr)
        {
            if (m_rwUsed.find(itr->get())==m_rwUsed.end())
            {
                return itr->get();
            }
        }
        return 0;
    }

};

#if 0
    // temporary test of autoregistering, not compiled by default.
    enum Methods
    {
        SET_1,
        SET_2,
        END
    };


    typedef std::pair<Methods,std::string> MethodPair;

    class Proxy
    {
    public:
        Proxy(MethodPair* methods)
        {
            std::cout<<"methods "<<methods<<std::endl;
            for(int i=0;methods[i].first!=END;++i)
            {
                std::cout<<"\t"<<methods[i].first<<"\t"<<methods[i].second<<std::endl;
            }
        }
    };


    static MethodPair methods[] =
    {
        MethodPair(SET_1,"SET_1"),
        MethodPair(SET_2,"SET_2"),
        MethodPair(END,"")
    };

    Proxy myproxy(methods);

#endif

void PrintFilePathList(std::ostream& stream,const FilePathList& filepath)
{
    for(FilePathList::const_iterator itr=filepath.begin();
        itr!=filepath.end();
        ++itr)
    {
        stream << "    "<< *itr<<std::endl;
    }
}

//crRegistry* crRegistry::instance(bool erase)
//{
//    static ref_ptr<crRegistry> s_nodeFactory = new crRegistry;
//    if (erase) 
//    {
//        s_nodeFactory->closeAllLibraries();
//        s_nodeFactory = 0;
//    }
//    return s_nodeFactory.get(); // will return NULL on erase
//}
crRegistry* crRegistry::instance(/*bool erase*/)
{
	if (m_instance==NULL)
	{
		m_instance = new crRegistry;
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}

void crRegistry::clear()
{
	//m_instance = NULL;

	//m_databasePager = 0;
	if(m_databasePager.valid()) 
	{
		m_databasePager->cancel();
		m_databasePager->clear();
	}
	m_databasePager = 0;
	m_sharedStateManager = 0;
	clearObjectCache();
	clearArchiveCache();
    //closeLibrary("Plugin_freetype.dll");
	//closeLibrary("Plugin_cred.dll");
	//closeAllLibraries();
	//CRCore::notify(CRCore::ALWAYS)<<"crRegistry::clear()"<< std::endl;
}
// definition of the crRegistry
crRegistry::crRegistry()
{
    // comment out because it was causing problems under OSX - causing it to crash osgconv when constucting ostream in CRCore::notify().
    // notify(INFO) << "Constructing CRCore::crRegistry"<<std::endl;

    m_createNodeFromImage = false;
    m_openingLibrary = false;
    
    initFilePathLists();

    // register file extension alias.
    addFileExtensionAlias("sgi",  "rgb");
    addFileExtensionAlias("rgba", "rgb");
    addFileExtensionAlias("int",  "rgb");
    addFileExtensionAlias("inta", "rgb");
    addFileExtensionAlias("bw",   "rgb");

    addFileExtensionAlias("ivz",   "gz");
    addFileExtensionAlias("ozg",   "gz");
    

#if defined(DARWIN_QUICKTIME)
    addFileExtensionAlias("jpg",  "qt");
    addFileExtensionAlias("jpe",  "qt");
    addFileExtensionAlias("jpeg", "qt");
    addFileExtensionAlias("tif",  "qt");
    addFileExtensionAlias("tiff", "qt");
    addFileExtensionAlias("gif",  "qt");
    addFileExtensionAlias("png",  "qt");
    addFileExtensionAlias("psd",  "qt");
    addFileExtensionAlias("tga",  "qt");
    addFileExtensionAlias("mov",  "qt");
    addFileExtensionAlias("avi",  "qt");
    addFileExtensionAlias("mpg",  "qt");
    addFileExtensionAlias("mpv",  "qt");
    addFileExtensionAlias("dv",   "qt");
    addFileExtensionAlias("mp4",   "qt");
#else
    addFileExtensionAlias("jpg",  "jpeg");
    addFileExtensionAlias("jpe",  "jpeg");
    addFileExtensionAlias("tif",  "tiff");

    // really need to decide this at runtime...
    #if defined(USE_XINE)
        addFileExtensionAlias("mov",  "xine");
        addFileExtensionAlias("mpg",  "xine");
        addFileExtensionAlias("mpv",  "xine");
        addFileExtensionAlias("dv",   "xine");
        addFileExtensionAlias("avi",  "xine");
        addFileExtensionAlias("wmv",  "xine");
    #endif

#endif


    // remove geo to lwo alias as the new Carbon Graphics GEO format
    // also uses the .geo. It is still possible to load light wave .geo
    // files via loading the lwo plugin explicitly and then doing a readNodeFile.
    //addFileExtensionAlias("geo",  "lwo");
    addFileExtensionAlias("lw",   "lwo");

    addFileExtensionAlias("wrl",   "iv");
    
    // add alias for the text/freetype plugin.
    addFileExtensionAlias("ttf",   "freetype");  // true type
    addFileExtensionAlias("ttc",   "freetype");  // true type
    addFileExtensionAlias("cid",   "freetype");  // Postscript CID-Fonts
    addFileExtensionAlias("cff",   "freetype");  // OpenType
    addFileExtensionAlias("cef",   "freetype");  // OpenType
    addFileExtensionAlias("fon",   "freetype");  // Windows bitmap fonts
    addFileExtensionAlias("fnt",   "freetype");    // Windows bitmap fonts
    
    // wont't add type1 and type2 until resolve extension collision with Peformer binary and ascii files.
    // addFileExtensionAlias("pfb",   "freetype");  // type1 binary
    // addFileExtensionAlias("pfa",   "freetype");  // type2 ascii


    // portable bitmap, greyscale and colour/pixmap image formats
    addFileExtensionAlias("pbm", "pnm");
    addFileExtensionAlias("pgm", "pnm");
    addFileExtensionAlias("ppm", "pnm");
	addFileExtensionAlias("img", "cre");
	addFileExtensionAlias("shd", "cre");
	addFileExtensionAlias("crs", "cre");
	addFileExtensionAlias("cook", "cre");
    addFileExtensionAlias("mcook", "cre");
	addFileExtensionAlias("cru", "cre");
}


crRegistry::~crRegistry()
{
    // switch off the pager and its associated thread before we clean up 
    // rest of the crRegistry.
    m_databasePager = 0;
    // clean up the crSharedStateManager 
    m_sharedStateManager = 0;
    // object cache clear needed here to prevent crash in unref() of
    // the objects it contains when running the TXP plugin.
    // Not sure why, but perhaps there is is something in a TXP plugin
    // which is deleted the data before its ref count hits zero, perhaps
    // even some issue with objects be allocated by a plugin that is
    // mainted after that plugin is deleted...  Robert Osfield, Jan 2004.
    clearObjectCache();
    clearArchiveCache();
    // unload all the plugin before we finally destruct.
    closeAllLibraries();
	//CRCore::notify(CRCore::ALWAYS)<<"~crRegistry()"<< std::endl;
}

#if defined(__APPLE__)

//Executable packages should be able to load file resources from inside the packages.
//These resources should be stored in YourProgram.app/Contents/Resources, and so
//should the path list for the data files should include that path by default.
#include <CoreServices/CoreServices.h>

void crRegistry::initDataFilePathList()
{
  
  FilePathList filepath;
  
  //
  // set up data file paths
  //
  char *ptr;
  
  if( (ptr = getenv( "CRE_FILE_PATH" )) )
  {
    //notify(DEBUG_INFO) << "OSG_FILE_PATH("<<ptr<<")"<<std::endl;
    convertStringPathIntoFilePathList(ptr, filepath);
  }
  else if( (ptr = getenv( "CREFILEPATH" )) )
  {
    //notify(DEBUG_INFO) << "OSGFILEPATH("<<ptr<<")"<<std::endl;
    convertStringPathIntoFilePathList(ptr, filepath);
  }
  

  char* buffer = NULL;
  char* bundlePathBuffer = NULL;
  CFURLRef  url;
  CFStringRef pathString;
  CFBundleRef myBundle;
  CFStringRef bundlePathString;
  CFIndex strLen; 
  CFIndex maxBufferSize;
  
  myBundle = CFBundleGetMainBundle();
  if (myBundle != NULL) {
    // Get the URL to the bundle
    url = CFBundleCopyBundleURL( myBundle );
    
    // Convert the URL to a CFString that looks like a Unix file path
    bundlePathString = CFURLCopyFileSystemPath( url, kCFURLPOSIXPathStyle );
    
    // Convert the CFString to a C string
    strLen = CFStringGetLength( bundlePathString );
    maxBufferSize = CFStringGetMaximumSizeForEncoding( strLen, kCFStringEncodingUTF8 );
    bundlePathBuffer = (char*) malloc(maxBufferSize);
    CFStringGetCString( bundlePathString, bundlePathBuffer, maxBufferSize, kCFStringEncodingUTF8 );
    
    CFRelease( url );

    // Now find the resources folder
    url = CFBundleCopyResourcesDirectoryURL( myBundle );
    //pathString = CFURLCopyPath( url );
    // Convert the URL to a CFString that looks like a Unix file path
    pathString = CFURLCopyFileSystemPath( url, kCFURLPOSIXPathStyle );
    
    // Convert the CFString to a C string
    strLen = CFStringGetLength( pathString );
    maxBufferSize = CFStringGetMaximumSizeForEncoding( strLen, kCFStringEncodingUTF8 );
    buffer = (char*) malloc(maxBufferSize);
    CFStringGetCString( pathString, buffer, maxBufferSize, kCFStringEncodingUTF8 );
    
    // Combine the string and copy it into the FilePath list
    filepath.push_back( std::string(bundlePathBuffer) 
                         + std::string("/")
                         + std::string(buffer)
                         );
    
    CFRelease( pathString );
    CFRelease( bundlePathString );
    CFRelease( url );
    
    if (bundlePathBuffer != NULL) free(bundlePathBuffer);
    if (buffer != NULL) free(buffer);
    
    pathString = NULL;
    bundlePathString = NULL;
    url = NULL;
  }
  //else
 // {
 //   CRCore::notify( CRCore::DEBUG_INFO ) << "Couldn't find the Application Bundle" << std::endl;
 // }
  
  setDataFilePathList(filepath);

  //CRCore::notify(INFO)<<"Data FilePathList"<<std::endl;
  //PrintFilePathList(CRCore::notify(INFO),getDataFilePathList());
}

#else

#include <iostream>

void crRegistry::initDataFilePathList()
{
	FilePathList filepath;
    //
    // set up data file paths
    //
    char *ptr;
    if( (ptr = getenv( "CRE_FILE_PATH" )) )
    {
        //notify(DEBUG_INFO) << "OSG_FILE_PATH("<<ptr<<")"<<std::endl;
        //setDataFilePathList(ptr);
		convertStringPathIntoFilePathList(ptr, filepath);
    }
    else if( (ptr = getenv( "CREFILEPATH" )) )
    {
        //notify(DEBUG_INFO) << "OSGFILEPATH("<<ptr<<")"<<std::endl;
        //setDataFilePathList(ptr);
		convertStringPathIntoFilePathList(ptr, filepath);
    }

    //CRCore::notify(INFO)<<"Data FilePathList"<<std::endl;
    //PrintFilePathList(CRCore::notify(INFO),getDataFilePathList());
	CRIOManager::appendPlatformSpecificResourceFilePaths(filepath);
	setDataFilePathList(filepath);
}

#endif

void crRegistry::setDataFilePathList(const std::string& paths)
{
    m_dataFilePath.clear(); 
    convertStringPathIntoFilePathList(paths,m_dataFilePath);
}

void crRegistry::setLibraryFilePathList(const std::string& paths) { m_libraryFilePath.clear(); convertStringPathIntoFilePathList(paths,m_libraryFilePath); }

void crRegistry::initLibraryFilePathList()
{
    //
    // set up library paths
    //
    char* ptr;
    if( (ptr = getenv( "CRE_LIBRARY_PATH")) )
    {
        //notify(DEBUG_INFO) << "CRE_LIBRARY_PATH("<<ptr<<")"<<std::endl;
        setLibraryFilePathList(ptr);
    }
    else if( (ptr = getenv( "CRE_LD_LIBRARY_PATH")) )
    {
        //notify(DEBUG_INFO) << "CRE_LD_LIBRARY_PATH("<<ptr<<")"<<std::endl;
        setLibraryFilePathList(ptr);
    }
    
    appendPlatformSpecificLibraryFilePaths(m_libraryFilePath);

    //CRCore::notify(INFO)<<"Library FilePathList"<<std::endl;
    //PrintFilePathList(CRCore::notify(INFO),getLibraryFilePathList());

}

void crRegistry::addDotCreWrapper(crDotCreWrapper* wrapper)
{
    if (wrapper==0L) return;

    //notify(INFO) << "CRCore::crRegistry::addDotCreWrapper("<<wrapper->getName()<<")"<< std::endl;
    const crDotCreWrapper::Associates& assoc = wrapper->getAssociates();
    
    for(crDotCreWrapper::Associates::const_iterator itr=assoc.begin();
                                                  itr!=assoc.end();
                                                  ++itr)
    {
        //notify(INFO) << "    ("<<*itr<<")"<< std::endl;
    }

    const std::string& name = wrapper->getName();
    const CRCore::crBase* proto = wrapper->getPrototype();

    m_objectWrapperMap[name] = wrapper;
    if (wrapper->getReadWriteMode()==crDotCreWrapper::READ_AND_WRITE) m_classNameWrapperMap[name] = wrapper;
    
    if (proto)
    {
        std::string libraryName = proto->libraryName();
        std::string compositeName = libraryName + "::" + name;

        m_objectWrapperMap[compositeName] = wrapper;
        if (wrapper->getReadWriteMode()==crDotCreWrapper::READ_AND_WRITE) m_classNameWrapperMap[compositeName] = wrapper;

        if (dynamic_cast<const crImage*>(proto))
        {
            m_imageWrapperMap[name] = wrapper;
            m_imageWrapperMap[compositeName] = wrapper;
        }
        if (dynamic_cast<const crDrawable*>(proto))
        {
              m_drawableWrapperMap[name] = wrapper;
              m_drawableWrapperMap[compositeName] = wrapper;
        }
        if (dynamic_cast<const crStateAttribute*>(proto))
        {
            m_stateAttrWrapperMap[name] = wrapper;
            m_stateAttrWrapperMap[compositeName] = wrapper;
        }
        if (dynamic_cast<const crUniform*>(proto))
        {
            m_uniformWrapperMap[name] = wrapper;
            m_uniformWrapperMap[compositeName] = wrapper;
        }
        if (dynamic_cast<const crNode*>(proto))
        {
            m_nodeWrapperMap[name] = wrapper;
            m_nodeWrapperMap[compositeName] = wrapper;
        }
    }
}

// need to change to delete all instances of wrapper, since we
// now can have a wrapper entered twice with the addition of the
// library::class composite name.
void crRegistry::eraseWrapper(DotCreWrapperMap& wrappermap,crDotCreWrapper* wrapper)
{
    typedef std::vector<DotCreWrapperMap::iterator> EraseList;
    EraseList eraseList;
    for(DotCreWrapperMap::iterator witr=wrappermap.begin();
        witr!=wrappermap.end();
        ++witr)
    {
        if (witr->second==wrapper) eraseList.push_back(witr);
    }
    for(EraseList::iterator eitr=eraseList.begin();
        eitr!=eraseList.end();
        ++eitr)
    {
        wrappermap.erase(*eitr);
    }
}

void crRegistry::removeDotCreWrapper(crDotCreWrapper* wrapper)
{
    if (wrapper==0L) return;

    eraseWrapper(m_objectWrapperMap,wrapper);
    eraseWrapper(m_classNameWrapperMap,wrapper);
    eraseWrapper(m_imageWrapperMap,wrapper);
    eraseWrapper(m_drawableWrapperMap,wrapper);
    eraseWrapper(m_uniformWrapperMap,wrapper);
    eraseWrapper(m_stateAttrWrapperMap,wrapper);
    eraseWrapper(m_nodeWrapperMap,wrapper);
}

void crRegistry::addReaderWriter(crReaderWriter* rw)
{
    if (rw==0L) return;
    // notify(INFO) << "CRCore::crRegistry::addReaderWriter("<<rw->className()<<")"<< std::endl;
    m_rwList.push_back(rw);
}

void crRegistry::removeReaderWriter(crReaderWriter* rw)
{
    if (rw==0L) return;
//    notify(INFO) << "CRCore::crRegistry::removeReaderWriter();"<< std::endl;
    ReaderWriterList::iterator rwitr = std::find(m_rwList.begin(),m_rwList.end(),rw);
    if (rwitr!=m_rwList.end())
    {
        m_rwList.erase(rwitr);
    }
}

void crRegistry::addScriptLoader(crScriptLoader* sl)
{
	if (sl==0L) return;
	// notify(INFO) << "CRCore::crRegistry::addReaderWriter("<<rw->className()<<")"<< std::endl;
	sl->loadScript();
	m_slList.push_back(sl);
}

void crRegistry::removeScriptLoader(crScriptLoader* sl)
{
	if (sl==0L) return;
	//    notify(INFO) << "CRCore::crRegistry::removeReaderWriter();"<< std::endl;
	ScriptLoaderList::iterator slitr = std::find(m_slList.begin(),m_slList.end(),sl);
	if (slitr!=m_slList.end())
	{
		sl->unLoadScript();
		m_slList.erase(slitr);
	}
}

void crRegistry::addFileExtensionAlias(const std::string mapExt, const std::string toExt)
{
    if (mapExt!=toExt) m_extAliasMap[mapExt] = toExt;
}

std::string crRegistry::createLibraryNameForFile(const std::string& fileName)
{
    std::string ext = getLowerCaseFileExtension(fileName);
    return createLibraryNameForExtension(ext);
}


std::string crRegistry::createLibraryNameForExtension(const std::string& ext)
{

    ExtensionAliasMap::iterator itr=m_extAliasMap.find(ext);
    if (itr!=m_extAliasMap.end()) return createLibraryNameForExtension(itr->second);

#ifdef CRE_JAVA_BUILD
	static std::string prepend = "java";
#else
	static std::string prepend = "";
#endif

#if defined(WIN32)
    // !! recheck evolving Cygwin DLL extension naming protocols !! NHV
    #ifdef __CYGWIN__
        return "cyg"+prepend+"Plugin_"+ext+".dll";
    #elif defined(__MINGW32__)
        return "lib"+prepend+"Plugin_"+ext+".dll";
    #else
        #ifdef _DEBUG
            return prepend+"Plugin_"+ext+"d.dll";
	        //return prepend+"Plugin_"+ext+".dll";
        #else
            return prepend+"Plugin_"+ext+".dll";
        #endif
    #endif
#elif macintosh
    return prepend+"Plugin_"+ext;
#elif defined(__hpux__)
    // why don't we use PLUGIN_EXT from the makefiles here?
    return prepend+"Plugin_"+ext+".sl";
#else
    return prepend+"Plugin_"+ext+".so";
#endif

}

std::string crRegistry::createLibraryNameForNodeKit(const std::string& name)
{
#if defined(WIN32)
    // !! recheck evolving Cygwin DLL extension naming protocols !! NHV
    #ifdef __CYGWIN__ // [
    return "cyg"+name+".dll";
    #elif defined(__MINGW32__)
        return "lib"+name+".dll";
    #else
        #ifdef _DEBUG
            return name+"d.dll";
        #else
            return name+".dll";
        #endif
    #endif
#elif macintosh
    return name;
#elif defined(__hpux__)
    // why don't we use PLUGIN_EXT from the makefiles here?
    return "lib"+name+".sl";
#else
    return "lib"+name+".so";
#endif
}

bool crRegistry::loadLibrary(const std::string& fileName)
{
    crDynamicLibrary* dl = getLibrary(fileName);
    if (dl) return false;

    m_openingLibrary=true;

    dl = crDynamicLibrary::loadLibrary(fileName);
    m_openingLibrary=false;

    if (dl)
    {
        m_dlList.push_back(dl);
        return true;
    }
    return false;
}


bool crRegistry::closeLibrary(const std::string& fileName)
{
    DynamicLibraryList::iterator ditr = getLibraryItr(fileName);
    if (ditr!=m_dlList.end())
    {
        m_dlList.erase(ditr);
        return true;
    }
    return false;
}

void crRegistry::closeAllLibraries()
{
    m_dlList.clear();
}

crRegistry::DynamicLibraryList::iterator crRegistry::getLibraryItr(const std::string& fileName)
{
    DynamicLibraryList::iterator ditr = m_dlList.begin();
    for(;ditr!=m_dlList.end();++ditr)
    {
        if ((*ditr)->getName()==fileName) return ditr;
    }
    return m_dlList.end();
}

crDynamicLibrary* crRegistry::getLibrary(const std::string& fileName)
{
    DynamicLibraryList::iterator ditr = getLibraryItr(fileName);
    if (ditr!=m_dlList.end()) return ditr->get();
    else return NULL;
}

crReaderWriter* crRegistry::getReaderWriterForExtension(const std::string& ext)
{
    // record the existing reader writer.
    std::set<crReaderWriter*> rwOriginal;

    // first attemt one of the installed loaders
    for(ReaderWriterList::iterator itr=m_rwList.begin();
        itr!=m_rwList.end();
        ++itr)
    {
        rwOriginal.insert(itr->get());
        if((*itr)->acceptsExtension(ext)) return (*itr).get();
    }

    // now look for a plug-in to load the file.
    std::string libraryName = createLibraryNameForExtension(ext);
    //notify(INFO) << "Now checking for plug-in "<<libraryName<< std::endl;
    if (loadLibrary(libraryName))
    {
        for(ReaderWriterList::iterator itr=m_rwList.begin();
            itr!=m_rwList.end();
            ++itr)
        {
            if (rwOriginal.find(itr->get())==rwOriginal.end())
          if((*itr)->acceptsExtension(ext)) return (*itr).get();
        }
    }

    return NULL;

}

struct concrete_wrapper: basic_type_wrapper 
{
    concrete_wrapper(const CRCore::crBase *myobj) : myobj_(myobj) {}
    bool matches(const CRCore::crBase *proto) const
    {
        return myobj_->isSameKindAs(proto);
    }
    const CRCore::crBase *myobj_;
};

CRCore::crBase* crRegistry::readObjectOfType(const CRCore::crBase& compObj,crInput& fr)
{
    return readObjectOfType(concrete_wrapper(&compObj), fr);
}

CRCore::crBase* crRegistry::readObjectOfType(const basic_type_wrapper &btw,crInput& fr)
{
    const char *str = fr[0].getStr();
    if (str==NULL) return NULL;

    if (fr[0].matchWord("Use"))
    {
        if (fr[1].isString())
        {
            crBase* obj = fr.getObjectForUniqueID(fr[1].getStr());
            if (obj && btw.matches(obj))
            {
                fr+=2;
                return obj;
            }
        }
        else return NULL;

    }

    std::string name = str;
    DotCreWrapperMap::iterator itr = m_objectWrapperMap.find(name);
    if (itr==m_objectWrapperMap.end())
    {
        // not found so check if a library::class composite name.
        std::string token = fr[0].getStr();
        std::string::size_type posDoubleColon = token.rfind("::");
        if (posDoubleColon != std::string::npos)
        {
            // we have a composite name so now strip off the library name
            // are try to load it, and then retry the readObject to see
            // if we can recongise the objects.
        
            std::string libraryName = std::string(token,0,posDoubleColon);

            // first try the standard nodekit library.
            std::string nodeKitLibraryName = createLibraryNameForNodeKit(libraryName);
            if (loadLibrary(nodeKitLibraryName)) return readObjectOfType(btw,fr);
            
            // otherwise try the osgdb_ plugin library.
            std::string pluginLibraryName = createLibraryNameForExtension(libraryName);
            if (loadLibrary(pluginLibraryName)) return readObjectOfType(btw,fr);
        }
    }
    else if (fr[1].isOpenBracket())
    {
    
        crDotCreWrapper* wrapper = itr->second.get();
        const CRCore::crBase* proto = wrapper->getPrototype();
        if (proto==NULL)
        {
            CRCore::notify(CRCore::WARN)<<"Token "<<fr[0].getStr()<<" read, but has no prototype, cannot load."<< std::endl;
            return NULL;
        }
        
        if (!btw.matches(proto))
        {
            return NULL;
        }

        // record the number of nested brackets move the input iterator
        // over the name { tokens.
        int entry = fr[0].getNoNestedBrackets();
        fr+=2;

        const crDotCreWrapper::Associates& assoc = wrapper->getAssociates();
        CRCore::crBase* obj = proto->cloneType();

        while(!fr.eof() && fr[0].getNoNestedBrackets()>entry)
        {
            bool iteratorAdvanced = false;
            if (fr[0].matchWord("UniqueID") && fr[1].isString())
            {
                fr.regisiterUniqueIDForObject(fr[1].getStr(),obj);
                fr += 2;
                iteratorAdvanced = true;
            }

            // read the local data by iterating through the associate
            // list, mapping the associate names to crDotCreWrapper's which
            // in turn have the appropriate functions.
            for(crDotCreWrapper::Associates::const_iterator aitr=assoc.begin();
                                                          aitr!=assoc.end();
                                                          ++aitr)
            {
                DotCreWrapperMap::iterator mitr = m_objectWrapperMap.find(*aitr);
                if (mitr==m_objectWrapperMap.end())
                {
                    // not found so check if a library::class composite name.
                    std::string token = *aitr;
                    std::string::size_type posDoubleColon = token.rfind("::");
                    if (posDoubleColon != std::string::npos)
                    {

                        // we have a composite name so now strip off the library name
                        // are try to load it, and then retry the find to see
                        // if we can recongise the objects.

                        std::string libraryName = std::string(token,0,posDoubleColon);

                        // first try the standard nodekit library.
                        std::string nodeKitLibraryName = createLibraryNameForNodeKit(libraryName);
                        if (loadLibrary(nodeKitLibraryName)) 
                        {
                            mitr = m_objectWrapperMap.find(*aitr);
                        }

                        if (mitr==m_objectWrapperMap.end())
                        {
                            // otherwise try the osgdb_ plugin library.
                            std::string pluginLibraryName = createLibraryNameForExtension(libraryName);
                            if (loadLibrary(pluginLibraryName))
                            {
                                mitr = m_objectWrapperMap.find(*aitr);
                            }
                        }

                    }
                }

                if (mitr!=m_objectWrapperMap.end())
                {
                    // get the function to read the data...
                    crDotCreWrapper::ReadFunc rf = mitr->second->getReadFunc();
                    if (rf && (*rf)(*obj,fr)) iteratorAdvanced = true;
                }

            }

            if (!iteratorAdvanced) fr.advanceOverCurrentFieldOrBlock();
        }
        ++fr;                        // step over trailing '}'
        
        return obj;
        
    }
    return 0L;
}

//
// read object from input iterator.
//
CRCore::crBase* crRegistry::readObject(DotCreWrapperMap& dowMap,crInput& fr)
{
    const char *str = fr[0].getStr();
    if (str==NULL) return NULL;

    std::string name = str;
    DotCreWrapperMap::iterator itr = dowMap.find(name);
    if (itr==dowMap.end())
    {
        // not found so check if a library::class composite name.
        std::string token = fr[0].getStr();
        std::string::size_type posDoubleColon = token.rfind("::");
        if (posDoubleColon != std::string::npos)
        {
            // we have a composite name so now strip off the library name
            // are try to load it, and then retry the readObject to see
            // if we can recongise the objects.
        
            std::string libraryName = std::string(token,0,posDoubleColon);

            // first try the standard nodekit library.
            std::string nodeKitLibraryName = createLibraryNameForNodeKit(libraryName);
            if (loadLibrary(nodeKitLibraryName)) return readObject(dowMap,fr);
            
            // otherwise try the osgdb_ plugin library.
            std::string pluginLibraryName = createLibraryNameForExtension(libraryName);
            if (loadLibrary(pluginLibraryName)) return readObject(dowMap,fr);
        }
    }
    else if (fr[1].isOpenBracket())
    {
    
        crDotCreWrapper* wrapper = itr->second.get();
        const CRCore::crBase* proto = wrapper->getPrototype();
        if (proto==NULL)
        {
            CRCore::notify(CRCore::WARN)<<"Token "<<fr[0].getStr()<<" read, but has no prototype, cannot load."<< std::endl;
            return NULL;
        }

        // record the number of nested brackets move the input iterator
        // over the name { tokens.
        int entry = fr[0].getNoNestedBrackets();
        fr+=2;

        const crDotCreWrapper::Associates& assoc = wrapper->getAssociates();
        CRCore::crBase* obj = proto->cloneType();

        while(!fr.eof() && fr[0].getNoNestedBrackets()>entry)
        {
            bool iteratorAdvanced = false;
            if (fr[0].matchWord("UniqueID") && fr[1].isString())
            {
                fr.regisiterUniqueIDForObject(fr[1].getStr(),obj);
                fr += 2;
                iteratorAdvanced = true;
            }

            // read the local data by iterating through the associate
            // list, mapping the associate names to crDotCreWrapper's which
            // in turn have the appropriate functions.
            for(crDotCreWrapper::Associates::const_iterator aitr=assoc.begin();
                                                          aitr!=assoc.end();
                                                          ++aitr)
            {
                DotCreWrapperMap::iterator mitr = m_objectWrapperMap.find(*aitr);
                if (mitr==m_objectWrapperMap.end())
                {
                    // not found so check if a library::class composite name.
                    std::string token = *aitr;
                    std::string::size_type posDoubleColon = token.rfind("::");
                    if (posDoubleColon != std::string::npos)
                    {

                        // we have a composite name so now strip off the library name
                        // are try to load it, and then retry the find to see
                        // if we can recongise the objects.

                        std::string libraryName = std::string(token,0,posDoubleColon);

                        // first try the standard nodekit library.
                        std::string nodeKitLibraryName = createLibraryNameForNodeKit(libraryName);
                        if (loadLibrary(nodeKitLibraryName)) 
                        {
                            mitr = m_objectWrapperMap.find(*aitr);
                        }

                        if (mitr==m_objectWrapperMap.end())
                        {
                            // otherwise try the osgdb_ plugin library.
                            std::string pluginLibraryName = createLibraryNameForExtension(libraryName);
                            if (loadLibrary(pluginLibraryName))
                            {
                                mitr = m_objectWrapperMap.find(*aitr);
                            }
                        }

                    }
                }

                if (mitr!=m_objectWrapperMap.end())
                {
                    // get the function to read the data...
                    crDotCreWrapper::ReadFunc rf = mitr->second->getReadFunc();
                    if (rf && (*rf)(*obj,fr)) iteratorAdvanced = true;
                }

            }

            if (!iteratorAdvanced) fr.advanceOverCurrentFieldOrBlock();
        }
        ++fr;                        // step over trailing '}'
        
        return obj;
        
    }

    return 0L;
}

//
// read object from input iterator.
//
crBase* crRegistry::readObject(crInput& fr)
{
    if (fr[0].matchWord("Use"))
    {
        if (fr[1].isString())
        {
            crBase* obj = fr.getObjectForUniqueID(fr[1].getStr());
            if (obj) fr+=2;
            return obj;
        }
        else return NULL;

    }

    return readObject(m_objectWrapperMap,fr);
}


//
// read image from input iterator.
//
crImage* crRegistry::readImage(crInput& fr)
{
    if (fr[0].matchWord("Use"))
    {
        if (fr[1].isString())
        {
            crImage* image = dynamic_cast<crImage*>(fr.getObjectForUniqueID(fr[1].getStr()));
            if (image) fr+=2;
            return image;
        }
        else return NULL;

    }

    CRCore::crBase* obj = readObject(m_imageWrapperMap,fr);
    CRCore::crImage* image = dynamic_cast<crImage*>(obj);
    if (image) return image;
    else if (obj) obj->unref();
    
    return NULL;
}


//
// read drawable from input iterator.
//
crDrawable* crRegistry::readDrawable(crInput& fr)
{
    if (fr[0].matchWord("Use"))
    {
        if (fr[1].isString())
        {
            crDrawable* drawable = dynamic_cast<crDrawable*>(fr.getObjectForUniqueID(fr[1].getStr()));
            if (drawable) fr+=2;
            return drawable;
        }
        else return NULL;

    }

    CRCore::crBase* obj = readObject(m_drawableWrapperMap,fr);
    CRCore::crDrawable* drawable = dynamic_cast<crDrawable*>(obj);
    if (drawable) return drawable;
    else if (obj) obj->unref();
    
    return NULL;
}

//
// read drawable from input iterator.
//
crStateAttribute* crRegistry::readStateAttribute(crInput& fr)
{

    if (fr[0].matchWord("Use"))
    {
        if (fr[1].isString())
        {
            crStateAttribute* attribute = dynamic_cast<crStateAttribute*>(fr.getObjectForUniqueID(fr[1].getStr()));
            if (attribute) fr+=2;
            return attribute;
        }
        else return NULL;

    }

    return dynamic_cast<crStateAttribute*>(readObject(m_stateAttrWrapperMap,fr));
}

//
// read drawable from input iterator.
//
crUniform* crRegistry::readUniform(crInput& fr)
{

    if (fr[0].matchWord("Use"))
    {
        if (fr[1].isString())
        {
            crUniform* attribute = dynamic_cast<crUniform*>(fr.getObjectForUniqueID(fr[1].getStr()));
            if (attribute) fr+=2;
            return attribute;
        }
        else return NULL;

    }

    return dynamic_cast<crUniform*>(readObject(m_uniformWrapperMap,fr));
}

//
// read node from input iterator.
//
crNode* crRegistry::readNode(crInput& fr)
{
    if (fr[0].matchWord("Use"))
    {
        if (fr[1].isString())
        {
            crNode* node = dynamic_cast<crNode*>(fr.getObjectForUniqueID(fr[1].getStr()));
            if (node) fr+=2;
            return node;
        }
        else return NULL;

    }

    CRCore::crBase* obj = readObject(m_nodeWrapperMap,fr);
    CRCore::crNode* node = dynamic_cast<crNode*>(obj);
    if (node) return node;
    else if (obj) obj->unref();
    
    return NULL;
}

//
// Write object to output 
//
bool crRegistry::writeObject(const CRCore::crBase& obj,crOutput& fw)
{

    if (obj.referenceCount()>1)
    {
        std::string uniqueID;
        if (fw.getUniqueIDForObject(&obj,uniqueID))
        {
            fw.indent() << "Use " << uniqueID << std::endl;
            return true;
        }
    }

    std::string classname = obj.className();
    std::string libraryName = obj.libraryName();
    std::string compositeName = libraryName + "::" + classname;

    // try composite name first
    DotCreWrapperMap::iterator itr = m_classNameWrapperMap.find(compositeName);

    // composite name not found, try simple class name
    if (itr == m_classNameWrapperMap.end()) {
        itr = m_classNameWrapperMap.find(classname);
    }

    if (itr==m_classNameWrapperMap.end())
    {
        // first try the standard nodekit library.
        std::string nodeKitLibraryName = createLibraryNameForNodeKit(obj.libraryName());
        if (loadLibrary(nodeKitLibraryName)) return writeObject(obj,fw);

        // otherwise try the osgdb_ plugin library.
        std::string pluginLibraryName = createLibraryNameForExtension(obj.libraryName());
        if (loadLibrary(pluginLibraryName)) return writeObject(obj,fw);
    }
    else
    {
    
    
        crDotCreWrapper* wrapper = itr->second.get();
        const crDotCreWrapper::Associates& assoc = wrapper->getAssociates();

        if (strcmp(obj.libraryName(),"CRCore")==0)
        {
            // member of the core CRCore, so no need to have composite library::class name.
            fw.indent() << wrapper->getName() << " {"<< std::endl;
            fw.moveIn();
        }
        else
        {
            // member of the node kit so must use composite library::class name.
            std::string::size_type posDoubleColon = wrapper->getName().find("::");
            if (posDoubleColon != std::string::npos)
            {
                fw.indent() << wrapper->getName() << " {"<< std::endl;
            }
            else
            {
                fw.indent() << obj.libraryName()<<"::"<< wrapper->getName() << " {"<< std::endl;
            }

            fw.moveIn();
        }


        // write out the unique ID if required.
        if (obj.referenceCount()>1)
        {
            std::string uniqueID;
            fw.createUniqueIDForObject(&obj,uniqueID);
            fw.registerUniqueIDForObject(&obj,uniqueID);
            fw.indent() << "UniqueID " << uniqueID << std::endl;
        }

        // read the local data by iterating through the associate
        // list, mapping the associate names to crDotCreWrapper's which
        // in turn have the appropriate functions.
        for(crDotCreWrapper::Associates::const_iterator aitr=assoc.begin();
                                                      aitr!=assoc.end();
                                                      ++aitr)
        {
            DotCreWrapperMap::iterator mitr = m_objectWrapperMap.find(*aitr);
            if (mitr==m_objectWrapperMap.end())
            {
                // not found so check if a library::class composite name.
                std::string token = *aitr;
                std::string::size_type posDoubleColon = token.rfind("::");
                if (posDoubleColon != std::string::npos)
                {

                    // we have a composite name so now strip off the library name
                    // are try to load it, and then retry the find to see
                    // if we can recongise the objects.

                    std::string libraryName = std::string(token,0,posDoubleColon);

                    // first try the standard nodekit library.
                    std::string nodeKitLibraryName = createLibraryNameForNodeKit(libraryName);
                    if (loadLibrary(nodeKitLibraryName)) 
                    {
                        mitr = m_objectWrapperMap.find(*aitr);
                    }

                    if (mitr==m_objectWrapperMap.end())
                    {
                        // otherwise try the osgdb_ plugin library.
                        std::string pluginLibraryName = createLibraryNameForExtension(libraryName);
                        if (loadLibrary(pluginLibraryName))
                        {
                            mitr = m_objectWrapperMap.find(*aitr);
                        }
                    }

                }
            }
            if (mitr!=m_objectWrapperMap.end())
            {
                // get the function to read the data...
                crDotCreWrapper::WriteFunc wf = mitr->second->getWriteFunc();
                if (wf) (*wf)(obj,fw);
            }

        }

        fw.moveOut();
        fw.indent() << "}"<< std::endl;

        return true;
    }
    
    return false;
}



struct crRegistry::ReadObjectFunctor : public crRegistry::ReadFunctor
{
    ReadObjectFunctor(const std::string& filename, const crReaderWriter::Options* options):ReadFunctor(filename,options) {}

    virtual crReaderWriter::ReadResult doRead(crReaderWriter& rw) const { return rw.readObject(m_filename, m_options); }    
    virtual bool isValid(crReaderWriter::ReadResult& readResult) const { return readResult.validObject(); }
    virtual bool isValid(CRCore::crBase* object) const { return object!=0;  }
};

struct crRegistry::ReadImageFunctor : public crRegistry::ReadFunctor
{
    ReadImageFunctor(const std::string& filename, const crReaderWriter::Options* options):ReadFunctor(filename,options) {}

    virtual crReaderWriter::ReadResult doRead(crReaderWriter& rw)const  { return rw.readImage(m_filename, m_options); }    
    virtual bool isValid(crReaderWriter::ReadResult& readResult) const { return readResult.validImage(); }
    virtual bool isValid(CRCore::crBase* object) const { return dynamic_cast<CRCore::crImage*>(object)!=0;  }
};

struct crRegistry::ReadHeightFieldFunctor : public crRegistry::ReadFunctor
{
    ReadHeightFieldFunctor(const std::string& filename, const crReaderWriter::Options* options):ReadFunctor(filename,options) {}

    virtual crReaderWriter::ReadResult doRead(crReaderWriter& rw) const { return rw.readHeightField(m_filename, m_options); }    
    virtual bool isValid(crReaderWriter::ReadResult& readResult) const { return readResult.validHeightField(); }
    virtual bool isValid(CRCore::crBase* object) const { return dynamic_cast<CRCore::crHeightField*>(object)!=0;  }
};

struct crRegistry::ReadNodeFunctor : public crRegistry::ReadFunctor
{
    ReadNodeFunctor(const std::string& filename, const crReaderWriter::Options* options):ReadFunctor(filename,options) {}

    virtual crReaderWriter::ReadResult doRead(crReaderWriter& rw) const 
	{ 
		return rw.readNode(m_filename, m_options); 
	}    
    virtual bool isValid(crReaderWriter::ReadResult& readResult) const { return readResult.validNode(); }
    virtual bool isValid(CRCore::crBase* object) const { return dynamic_cast<CRCore::crNode*>(object)!=0;  }

};

struct crRegistry::ReadArchiveFunctor : public crRegistry::ReadFunctor
{
    ReadArchiveFunctor(const std::string& filename, crReaderWriter::ArchiveStatus status, unsigned int indexBlockSizeHint, const crReaderWriter::Options* options):
        ReadFunctor(filename,options),
        m_status(status),
        m_indexBlockSizeHint(indexBlockSizeHint) {}
        
    crReaderWriter::ArchiveStatus m_status;
    unsigned int m_indexBlockSizeHint;

    virtual crReaderWriter::ReadResult doRead(crReaderWriter& rw) const { return rw.openArchive(m_filename, m_status, m_indexBlockSizeHint, m_options); }
    virtual bool isValid(crReaderWriter::ReadResult& readResult) const { return readResult.validArchive(); }
    virtual bool isValid(CRCore::crBase* object) const { return dynamic_cast<CRIOManager::crArchive*>(object)!=0;  }

};

crReaderWriter::ReadResult crRegistry::read(const ReadFunctor& readFunctor)
{
    //std::string archiveName(".osga");

    //std::string::size_type positionArchive = readFunctor.m_filename.find(archiveName+'/');
    //if (positionArchive==std::string::npos) positionArchive = readFunctor.m_filename.find(archiveName+'\\');
    //if (positionArchive!=std::string::npos)
    //{
    //    std::string archiveName(readFunctor.m_filename.substr(0,positionArchive+5));
    //    std::string fileName(readFunctor.m_filename.substr(positionArchive+6,std::string::npos));
    //    CRCore::notify(CRCore::INFO)<<"Contains archive : "<<readFunctor.m_filename<<std::endl;
    //    CRCore::notify(CRCore::INFO)<<"         archive : "<<archiveName<<std::endl;
    //    CRCore::notify(CRCore::INFO)<<"         filename : "<<fileName<<std::endl;
    //    
    //    crReaderWriter::ReadResult result = openArchiveImplementation(archiveName,crReaderWriter::READ, 4096, readFunctor.m_options);
    //    
    //    if (!result.validArchive()) return result;

    //    CRIOManager::crArchive* archive = result.getArchive();
    //    
    //    CRCore::ref_ptr<crReaderWriter::Options> options = new crReaderWriter::Options;
    //    options->setDatabasePath(archiveName);

    //    return archive->readObject(fileName,options.get());
    //}

    //// if filename contains archive
    //// then get archive name
    //// if archive name is not in the cache then do an openArchive on
    //// that archive name
    //// use that archive to read the file.

    //if (containsServerAddress(readFunctor.m_filename))
    //{
    //    std::string serverName = getServerAddress(readFunctor.m_filename);
    //    std::string serverFile = getServerFileName(readFunctor.m_filename);
    //    CRCore::notify(CRCore::INFO)<<"Contains sever address : "<<serverName<<std::endl;
    //    CRCore::notify(CRCore::INFO)<<"         file name on server : "<<serverFile<<std::endl;

    //    if (serverName.empty())
    //    {
    //        return crReaderWriter::ReadResult("Warning: Server address invalid.");
    //    }
    //    
    //    if (serverFile.empty())
    //    {
    //        return crReaderWriter::ReadResult("Warning: Server file name invalid.");
    //    }

    //    crReaderWriter* rw = getReaderWriterForExtension("net");
    //    if (rw)
    //    {
    //        std::string& filename = const_cast<std::string&>(readFunctor.m_filename);
    //        filename = serverName+':'+serverFile;
    //        return readFunctor.doRead(*rw);
    //    }
    //    else
    //    {
    //        return  crReaderWriter::ReadResult("Warning: Could not find the .net plugin to read from server.");
    //    }
    //}
    
    // record the errors reported by readerwriters.
    typedef std::vector<crReaderWriter::ReadResult> Results;
    Results results;

    // first attempt to load the file from existing crReaderWriter's
    AvailableReaderWriterIterator itr(m_rwList);
    for(;itr.valid();++itr)
    {
        crReaderWriter::ReadResult rr = readFunctor.doRead(*itr);
        if (readFunctor.isValid(rr)) return rr;
        else results.push_back(rr);
    }

    if (!results.empty())
    {
        unsigned int num_FILE_NOT_HANDLED = 0;
        unsigned int num_FILE_NOT_FOUND = 0;
        unsigned int num_ERROR_IN_READING_FILE = 0;

        for(Results::iterator ritr=results.begin();
            ritr!=results.end();
            ++ritr)
        {
            if (ritr->status()==crReaderWriter::ReadResult::FILE_NOT_HANDLED) ++num_FILE_NOT_HANDLED;
            else if (ritr->status()==crReaderWriter::ReadResult::FILE_NOT_FOUND) ++num_FILE_NOT_FOUND;
            else if (ritr->status()==crReaderWriter::ReadResult::ERROR_IN_READING_FILE) ++num_ERROR_IN_READING_FILE;
        }
        
        if (num_FILE_NOT_HANDLED!=results.size())
        {
            // we've come across a file not found or error in reading file.
            if (num_ERROR_IN_READING_FILE)
            {
                //CRCore::notify(CRCore::NOTICE)<<"Warning: error reading file \""<<readFunctor.m_filename<<"\""<<std::endl;
				char gbuf[256];
				sprintf(gbuf,"Warning: error reading file %s\n\0",readFunctor.m_filename.c_str());
				gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
                return NULL;
            }
            else if (num_FILE_NOT_FOUND)
            {
                //CRCore::notify(CRCore::NOTICE)<<"Warning: could not find file \""<<readFunctor.m_filename<<"\""<<std::endl;
				char gbuf[256];
				sprintf(gbuf,"Warning: could not find file %s\n\0",readFunctor.m_filename.c_str());
				gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
                return NULL;
            }
        }
    }

    // now look for a plug-in to load the file.
    std::string libraryName = createLibraryNameForFile(readFunctor.m_filename);
    if (loadLibrary(libraryName))
    {
        for(;itr.valid();++itr)
        {
            crReaderWriter::ReadResult rr = readFunctor.doRead(*itr);
            if (readFunctor.isValid(rr)) return rr;
            else results.push_back(rr);
        }
    }
    
    if (results.empty())
    {
        return crReaderWriter::ReadResult("Warning: Could not find plugin to read objects from file \""+readFunctor.m_filename+"\".");
    }
    

    return results.front();
}

crReaderWriter::ReadResult crRegistry::readImplementation(const ReadFunctor& readFunctor, bool useObjectCache)
{
    std::string file(readFunctor.m_filename);

    if (useObjectCache)
    {
        // search for entry in the object cache.
        {
            CRCore::ScopedLock<CRCore::crMutex> lock(m_objectCacheMutex);
			//m_objectCacheMutex.lock();
            ObjectCache::iterator oitr=m_objectCache.find(file);
			//m_objectCacheMutex.unlock();
            if (oitr!=m_objectCache.end())
            {
                //notify(INFO)<<"returning cached instanced of "<<file<<std::endl;
                if (readFunctor.isValid(oitr->second.first.get())) return crReaderWriter::ReadResult(oitr->second.first.get(), crReaderWriter::ReadResult::FILE_LOADED_FROM_CACHE);
                else return crReaderWriter::ReadResult("Error file does not contain an CRCore::crBase");
            }
        }
        
        crReaderWriter::ReadResult rr = read(readFunctor);
        if (rr.validObject()) 
        {
            // update cache with new entry.
            //notify(INFO)<<"Adding to object cache "<<file<<std::endl;
            addEntryToObjectCache(file,rr.getObject());
        }
        //else
        //{
            //notify(INFO)<<"No valid object found for "<<file<<std::endl;
        //}

        return rr;

    }
    else
    {
        ObjectCache tmpObjectCache;
        
        {
            //CRCore::ScopedLock<CRCore::crMutex> lock(m_objectCacheMutex);
			m_objectCacheMutex.lock();
            tmpObjectCache.swap(m_objectCache);
			m_objectCacheMutex.unlock();
        }
        
        crReaderWriter::ReadResult rr = read(readFunctor);

        {
            //CRCore::ScopedLock<CRCore::crMutex> lock(m_objectCacheMutex);
			m_objectCacheMutex.lock();
            tmpObjectCache.swap(m_objectCache);
            m_objectCacheMutex.unlock();
        }
        
        return rr;
    }
}


crReaderWriter::ReadResult crRegistry::openArchiveImplementation(const std::string& fileName, crReaderWriter::ArchiveStatus status, unsigned int indexBlockSizeHint, const crReaderWriter::Options* options)
{

    // default to using chaching archive if no options structure provided, but if options are provided use archives
    // only if supplied.
    if (!options || (options && (options->getObjectCacheHint() & crReaderWriter::Options::CACHE_ARCHIVES)))
    {
        CRIOManager::crArchive* archive = getFromArchiveCache(fileName);
        if (archive) return archive;

        crReaderWriter::ReadResult result = readImplementation(ReadArchiveFunctor(fileName, status, indexBlockSizeHint, options),false);
        if (result.validArchive())
        {
            addToArchiveCache(fileName,result.getArchive());
        }
        return result;
    }
    else
    {
        return readImplementation(ReadArchiveFunctor(fileName, status, indexBlockSizeHint, m_options.get()),false);
    }
}


crReaderWriter::ReadResult crRegistry::readObjectImplementation(const std::string& fileName,const crReaderWriter::Options* options)
{
    return readImplementation(ReadObjectFunctor(fileName, options),
                              options ? (options->getObjectCacheHint()&crReaderWriter::Options::CACHE_OBJECTS)!=0: false);
}

crReaderWriter::WriteResult crRegistry::writeObjectImplementation(const crBase& obj,const std::string& fileName)
{
    // record the errors reported by readerwriters.
    typedef std::vector<crReaderWriter::WriteResult> Results;
    Results results;

    // first attempt to load the file from existing crReaderWriter's
    AvailableReaderWriterIterator itr(m_rwList);
    for(;itr.valid();++itr)
    {
        crReaderWriter::WriteResult rr = itr->writeObject(obj,fileName,m_options.get());
        if (rr.success()) return rr;
        else results.push_back(rr);
    }

    // now look for a plug-in to save the file.
    std::string libraryName = createLibraryNameForFile(fileName);
    if (loadLibrary(libraryName))
    {
        for(;itr.valid();++itr)
        {
            crReaderWriter::WriteResult rr = itr->writeObject(obj,fileName,m_options.get());
            if (rr.success()) return rr;
            else results.push_back(rr);
        }
    }

    if (results.empty())
    {
        return crReaderWriter::WriteResult("Warning: Could not find plugin to write objects to file \""+fileName+"\".");
    }

    return results.front();
}



crReaderWriter::ReadResult crRegistry::readImageImplementation(const std::string& fileName,const crReaderWriter::Options* options)
{
    return readImplementation(ReadImageFunctor(fileName, options),
                              options ? (options->getObjectCacheHint()&crReaderWriter::Options::CACHE_IMAGES)!=0: false);
}

crReaderWriter::WriteResult crRegistry::writeImageImplementation(const crImage& image,const std::string& fileName)
{
    // record the errors reported by readerwriters.
    typedef std::vector<crReaderWriter::WriteResult> Results;
    Results results;

    // first attempt to load the file from existing crReaderWriter's
    AvailableReaderWriterIterator itr(m_rwList);
    for(;itr.valid();++itr)
    {
        crReaderWriter::WriteResult rr = itr->writeImage(image,fileName,m_options.get());
        if (rr.success()) return rr;
        else results.push_back(rr);
    }

    // now look for a plug-in to save the file.
    std::string libraryName = createLibraryNameForFile(fileName);
    if (loadLibrary(libraryName))
    {
        for(;itr.valid();++itr)
        {
            crReaderWriter::WriteResult rr = itr->writeImage(image,fileName,m_options.get());
            if (rr.success()) return rr;
            else results.push_back(rr);
        }
    }

    if (results.empty())
    {
        return crReaderWriter::WriteResult("Warning: Could not find plugin to write image to file \""+fileName+"\".");
    }
    
    return results.front();
}


crReaderWriter::ReadResult crRegistry::readHeightFieldImplementation(const std::string& fileName,const crReaderWriter::Options* options)
{
    return readImplementation(ReadHeightFieldFunctor(fileName, options),
                              options ? (options->getObjectCacheHint()&crReaderWriter::Options::CACHE_HEIGHTFIELDS)!=0: false);
}

crReaderWriter::WriteResult crRegistry::writeHeightFieldImplementation(const crHeightField& crHeightField,const std::string& fileName)
{
    // record the errors reported by readerwriters.
    typedef std::vector<crReaderWriter::WriteResult> Results;
    Results results;

    // first attempt to load the file from existing crReaderWriter's
    AvailableReaderWriterIterator itr(m_rwList);
    for(;itr.valid();++itr)
    {
        crReaderWriter::WriteResult rr = itr->writeHeightField(crHeightField,fileName,m_options.get());
        if (rr.success()) return rr;
        else results.push_back(rr);
    }

    // now look for a plug-in to save the file.
    std::string libraryName = createLibraryNameForFile(fileName);
    if (loadLibrary(libraryName))
    {
        for(;itr.valid();++itr)
        {
            crReaderWriter::WriteResult rr = itr->writeHeightField(crHeightField,fileName,m_options.get());
            if (rr.success()) return rr;
            else results.push_back(rr);
        }
    }

    if (results.empty())
    {
        return crReaderWriter::WriteResult("Warning: Could not find plugin to write crHeightField to file \""+fileName+"\".");
    }
    
    return results.front();
}


crReaderWriter::ReadResult crRegistry::readNodeImplementation(const std::string& fileName,const crReaderWriter::Options* options)
{
    return readImplementation(ReadNodeFunctor(fileName, options),
                              options ? (options->getObjectCacheHint()&crReaderWriter::Options::CACHE_NODES)!=0: false);
}

crReaderWriter::WriteResult crRegistry::writeNodeImplementation(const crNode& node,const std::string& fileName)
{
    // record the errors reported by readerwriters.
    typedef std::vector<crReaderWriter::WriteResult> Results;
    Results results;

    // first attempt to write the file from existing crReaderWriter's
    AvailableReaderWriterIterator itr(m_rwList);
    for(;itr.valid();++itr)
    {
        crReaderWriter::WriteResult rr = itr->writeNode(node,fileName,m_options.get());
        if (rr.success()) return rr;
        else results.push_back(rr);
    }

    // now look for a plug-in to save the file.
    std::string libraryName = createLibraryNameForFile(fileName);
    if (loadLibrary(libraryName))
    {
        for(;itr.valid();++itr)
        {
            crReaderWriter::WriteResult rr = itr->writeNode(node,fileName,m_options.get());
            if (rr.success()) return rr;
            else results.push_back(rr);
        }
    }

    if (results.empty())
    {
        return crReaderWriter::WriteResult("Warning: Could not find plugin to write nodes to file \""+fileName+"\".");
    }

    return results.front();
}


void crRegistry::addEntryToObjectCache(const std::string& filename, CRCore::crBase* object, double timestamp)
{
    CRCore::ScopedLock<CRCore::crMutex> lock(m_objectCacheMutex);
    m_objectCache[filename]=ObjectTimeStampPair(object,timestamp);
}
CRCore::crBase* crRegistry::getFromObjectCache(const std::string& fileName)
{
    CRCore::ScopedLock<CRCore::crMutex> lock(m_objectCacheMutex);
    ObjectCache::iterator itr = m_objectCache.find(fileName);
    if (itr!=m_objectCache.end()) return itr->second.first.get();
    else return 0;
}

void crRegistry::updateTimeStampOfObjectsInCacheWithExtenalReferences(double currentTime)
{
    CRCore::ScopedLock<CRCore::crMutex> lock(m_objectCacheMutex);

    // look for objects with external references and update their time stamp.
    for(ObjectCache::iterator itr=m_objectCache.begin();
        itr!=m_objectCache.end();
        ++itr)
    {
        // if ref count is greater the 1 the object has an external reference.
        if (itr->second.first->referenceCount()>1)
        {
            // so update it time stamp.
            itr->second.second = currentTime;
        }
    }
}

void crRegistry::removeExpiredObjectsInCache(double expiryTime)
{
    CRCore::ScopedLock<CRCore::crMutex> lock(m_objectCacheMutex);

    //typedef std::vector<std::string> ObjectsToRemove;
    //ObjectsToRemove objectsToRemove;

    // first collect all the exprired entries in the ObjectToRemove list.
    for(ObjectCache::iterator oitr=m_objectCache.begin();
        oitr!=m_objectCache.end();
        )
    {
        if (oitr->second.second<=expiryTime)
        {
            //// record the filename of the entry to use as key for deleting
            //// afterwards/
            //objectsToRemove.push_back(oitr->first);
			oitr = m_objectCache.erase(oitr);
        }
		else
		{
			++oitr;
		}
    }
    
    //// remove the entries from the _objectCaache.
    //for(ObjectsToRemove::iterator ritr=objectsToRemove.begin();
    //    ritr!=objectsToRemove.end();
    //    ++ritr)
    //{
    //    m_objectCache.erase(*ritr);
    //}
        
}

void crRegistry::clearObjectCache()
{
    CRCore::ScopedLock<CRCore::crMutex> lock(m_objectCacheMutex);
    m_objectCache.clear();
}

void crRegistry::addToArchiveCache(const std::string& fileName, CRIOManager::crArchive* archive)
{
    CRCore::ScopedLock<CRCore::crMutex> lock(m_archiveCacheMutex);
    m_archiveCache[fileName] = archive;
}

/** Remove archive from cache.*/
void crRegistry::removeFromArchiveCache(const std::string& fileName)
{
    CRCore::ScopedLock<CRCore::crMutex> lock(m_archiveCacheMutex);
    ArchiveCache::iterator itr = m_archiveCache.find(fileName);
    if (itr!=m_archiveCache.end()) 
    {
        m_archiveCache.erase(itr);
    }
}

CRIOManager::crArchive* crRegistry::getFromArchiveCache(const std::string& fileName)
{
    CRCore::ScopedLock<CRCore::crMutex> lock(m_archiveCacheMutex);
    ArchiveCache::iterator itr = m_archiveCache.find(fileName);
    if (itr!=m_archiveCache.end()) return itr->second.get();
    else return 0;
}

void crRegistry::clearArchiveCache()
{
    CRCore::ScopedLock<CRCore::crMutex> lock(m_archiveCacheMutex);
    m_archiveCache.clear();
}

void crRegistry::releaseObjects(CRCore::crState* state)
{
    CRCore::ScopedLock<CRCore::crMutex> lock(m_objectCacheMutex);

    for(ObjectCache::iterator itr = m_objectCache.begin();
        itr != m_objectCache.end();
        ++itr)
    {
        CRCore::crBase* object = itr->second.first.get();
        object->releaseObjects(state);
    }
	m_objectCache.clear();
}

crDatabasePager* crRegistry::getOrCreateDatabasePager()
{
    if (!m_databasePager) m_databasePager = new crDatabasePager;
    
    return m_databasePager.get();
}

crSharedStateManager* crRegistry::getOrCreateSharedStateManager()
{
    if (!m_sharedStateManager) m_sharedStateManager = new crSharedStateManager;
    
    return m_sharedStateManager.get();
}

void CRIOManager::loadScript(const std::string &dllname)
{
	crRegistry::instance()->loadLibrary(dllname);
}