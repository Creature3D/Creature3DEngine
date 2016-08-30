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
#ifndef CRIOMANAGER_REGISTRY
#define CRIOMANAGER_REGISTRY 1

#include <CRCore/ref_ptr.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crKdTree.h>

#include <CRIOManager/crDynamicLibrary.h>
#include <CRIOManager/crReaderWriter.h>
#include <CRIOManager/crDotCreWrapper.h>
#include <CRIOManager/crDatabasePager.h>

#include <vector>
#include <map>
#include <string>


namespace CRIOManager {

/** basic structure for custom runtime inheritance checking */
struct basic_type_wrapper {
    virtual bool matches(const CRCore::crBase *proto) const = 0;
};

/** a class template that checks inheritance between a given
    crBase's class and a class defined at compile time through
        the template parameter T.
        This is used in conjunction with readObjectOfType() to
        specify an abstract class as reference type.
**/
template<class T>
struct type_wrapper: basic_type_wrapper {
    bool matches(const CRCore::crBase *proto) const
    {
        return dynamic_cast<const T*>(proto) != 0;
    }
};

class CRIOMANAGER_EXPORT crScriptLoader : public CRCore::Referenced
{
public:
	virtual void loadScript()=0;
	virtual void unLoadScript()=0;
};
extern CRIOMANAGER_EXPORT void loadScript(const std::string &dllname);

/**
    crRegistry is a singleton factory which stores
    the reader/writers which are linked in
    at runtime for reading non-native file formats.

    The crRegisterDotOsgWrapperProxy can be used to automatically register
    DotOsgWrappers, at runtime with the crRegistry. A crDotCreWrapper encapsulates
    the functions that can read and write to the .CRCore for each CRCore::crBase.

    The RegisterReaderWriterProxy can be used to automatically
    register at runtime a reader/writer with the crRegistry.
*/
class CRIOMANAGER_EXPORT crRegistry : public CRCore::Referenced
{
    public:
        static crRegistry* instance(/*bool erase = false*/);

        /** register an .fileextension alias to mapExt toExt, the later
	  * should the the extension name of the readerwriter plugin library.
	  * For example to map .tif files to the tiff loader, use
	  * addExtAlias("tif","tiff") which will enable .tif to be read
	  * by the libdb_tiff readerwriter plugin.*/
	void addFileExtensionAlias(const std::string mapExt, const std::string toExt);

        void addDotCreWrapper(crDotCreWrapper* wrapper);
        void removeDotCreWrapper(crDotCreWrapper* wrapper);

        void addReaderWriter(crReaderWriter* rw);
        void removeReaderWriter(crReaderWriter* rw);

		void addScriptLoader(crScriptLoader* sl);
		void removeScriptLoader(crScriptLoader* sl);

        /** create the platform specific library name associated with file.*/
        std::string createLibraryNameForFile(const std::string& fileName);

        /** create the platform specific library name associated with file extension.*/
        std::string createLibraryNameForExtension(const std::string& ext);

        /** create the platform specific library name associated with nodekit library name.*/
        std::string createLibraryNameForNodeKit(const std::string& name);

        /** find the library in the SG_LIBRARY_PATH and load it.*/
        bool loadLibrary(const std::string& fileName);
        /** close the attached library with specified name.*/
        bool closeLibrary(const std::string& fileName);
        /** close all libraries.*/
        void closeAllLibraries();

        /** get a reader writer which handles specified extension.*/
        crReaderWriter* getReaderWriterForExtension(const std::string& ext);
 
        CRCore::crBase*         readObjectOfType(const CRCore::crBase& compObj,crInput& fr);       
        CRCore::crBase*         readObjectOfType(const basic_type_wrapper &btw, crInput& fr);

        CRCore::crBase*         readObject(crInput& fr);
        CRCore::crImage*          readImage(crInput& fr);
        CRCore::crDrawable*       readDrawable(crInput& fr);
        CRCore::crUniform*        readUniform(crInput& fr);
        CRCore::crStateAttribute* readStateAttribute(crInput& fr);
        CRCore::crNode*           readNode(crInput& fr);
        
        bool                 writeObject(const CRCore::crBase& obj,crOutput& fw);

        
        class ReadFileCallback : public CRCore::Referenced
        {
        public:
        
            virtual crReaderWriter::ReadResult openArchive(const std::string& filename,crReaderWriter::ArchiveStatus status, unsigned int indexBlockSizeHint, const crReaderWriter::Options* useObjectCache)
            {
                return CRIOManager::crRegistry::instance()->openArchiveImplementation(filename, status, indexBlockSizeHint, useObjectCache);
            }
            
            virtual crReaderWriter::ReadResult readObject(const std::string& filename, const crReaderWriter::Options* options)
            {
                return CRIOManager::crRegistry::instance()->readObjectImplementation(filename,options);
            }

            virtual crReaderWriter::ReadResult readImage(const std::string& filename, const crReaderWriter::Options* options)
            {
                return CRIOManager::crRegistry::instance()->readImageImplementation(filename,options);
            }
            
            virtual crReaderWriter::ReadResult readHeightField(const std::string& filename, const crReaderWriter::Options* options)
            {
                return CRIOManager::crRegistry::instance()->readHeightFieldImplementation(filename,options);
            }
            
            virtual crReaderWriter::ReadResult readNode(const std::string& filename, const crReaderWriter::Options* options)
            {
                return CRIOManager::crRegistry::instance()->readNodeImplementation(filename,options);
            }
            
        protected:
               virtual ~ReadFileCallback() {}
        };

        /** Set the crRegistry callback to use in place of the default readFile calls.*/
        void setReadFileCallback( ReadFileCallback* cb) { m_readFileCallback = cb; }

        /** Get the readFile callback.*/
        ReadFileCallback* getReadFileCallback() { return m_readFileCallback.get(); }
        
        /** Get the const readFile callback.*/
        const ReadFileCallback* getReadFileCallback() const { return m_readFileCallback.get(); }


        crReaderWriter::ReadResult openArchive(const std::string& fileName,crReaderWriter::ArchiveStatus status, unsigned int indexBlockSizeHint, const crReaderWriter::Options* options)
        {
            if (m_readFileCallback.valid()) return m_readFileCallback->openArchive(fileName, status, indexBlockSizeHint, options);
            else return openArchiveImplementation(fileName, status, indexBlockSizeHint, options);
        }
        crReaderWriter::ReadResult openArchiveImplementation(const std::string& fileName, crReaderWriter::ArchiveStatus status, unsigned int indexBlockSizeHint, const crReaderWriter::Options* options);

        crReaderWriter::ReadResult readObject(const std::string& fileName,const crReaderWriter::Options* options)
        {
            if (m_readFileCallback.valid()) return m_readFileCallback->readObject(fileName,options);
            else return readObjectImplementation(fileName,options);
        }
        crReaderWriter::ReadResult readObjectImplementation(const std::string& fileName,const crReaderWriter::Options* options);
        
        crReaderWriter::ReadResult readImage(const std::string& fileName,const crReaderWriter::Options* options)
        {
            if (m_readFileCallback.valid()) return m_readFileCallback->readImage(fileName,options);
            else return readImageImplementation(fileName,options);
        }
        crReaderWriter::ReadResult readImageImplementation(const std::string& fileName,const crReaderWriter::Options* options);

        crReaderWriter::ReadResult readHeightField(const std::string& fileName,const crReaderWriter::Options* options)
        {
            if (m_readFileCallback.valid()) return m_readFileCallback->readHeightField(fileName,options);
            else return readHeightFieldImplementation(fileName,options);
        }
        crReaderWriter::ReadResult readHeightFieldImplementation(const std::string& fileName,const crReaderWriter::Options* options);

        crReaderWriter::ReadResult readNode(const std::string& fileName,const crReaderWriter::Options* options)
        {
            if (m_readFileCallback.valid()) return m_readFileCallback->readNode(fileName,options);
            else return readNodeImplementation(fileName,options);
        }
        crReaderWriter::ReadResult readNodeImplementation(const std::string& fileName,const crReaderWriter::Options* options);




        class WriteFileCallback : public CRCore::Referenced
        {
        public:
        
            virtual crReaderWriter::WriteResult writeObject(const CRCore::crBase& obj, const std::string& fileName)
            {
                return CRIOManager::crRegistry::instance()->writeObjectImplementation(obj,fileName);
            }

            virtual crReaderWriter::WriteResult writeImage(const CRCore::crImage& obj, const std::string& fileName)
            {
                return CRIOManager::crRegistry::instance()->writeImageImplementation(obj,fileName);
            }
            
            virtual crReaderWriter::WriteResult writeHeightField(const CRCore::crHeightField& obj, const std::string& fileName)
            {
                return CRIOManager::crRegistry::instance()->writeHeightFieldImplementation(obj,fileName);
            }
            
            virtual crReaderWriter::WriteResult writeNode(const CRCore::crNode& obj, const std::string& fileName)
            {
                return CRIOManager::crRegistry::instance()->writeNodeImplementation(obj,fileName);
            }
            
        protected:
               virtual ~WriteFileCallback() {}
        };

        /** Set the crRegistry callback to use in place of the default writeFile calls.*/
        void setWriteFileCallback( WriteFileCallback* cb) { m_writeFileCallback = cb; }

        /** Get the writeFile callback.*/
        WriteFileCallback* getWriteFileCallback() { return m_writeFileCallback.get(); }
        
        /** Get the const writeFile callback.*/
        const WriteFileCallback* getWriteFileCallback() const { return m_writeFileCallback.get(); }


        crReaderWriter::WriteResult writeObject(const CRCore::crBase& obj, const std::string& fileName)
        {
            if (m_writeFileCallback.valid()) return m_writeFileCallback->writeObject(obj,fileName);
            else return writeObjectImplementation(obj,fileName);
        }
        crReaderWriter::WriteResult writeObjectImplementation(const CRCore::crBase& obj, const std::string& fileName);

        crReaderWriter::WriteResult writeImage(const CRCore::crImage& obj, const std::string& fileName)
        {
            if (m_writeFileCallback.valid()) return m_writeFileCallback->writeImage(obj,fileName);
            else return writeImageImplementation(obj,fileName);
        }
        crReaderWriter::WriteResult writeImageImplementation(const CRCore::crImage& obj, const std::string& fileName);

        crReaderWriter::WriteResult writeHeightField(const CRCore::crHeightField& obj, const std::string& fileName)
        {
            if (m_writeFileCallback.valid()) return m_writeFileCallback->writeHeightField(obj,fileName);
            else return writeHeightFieldImplementation(obj,fileName);
        }
        crReaderWriter::WriteResult writeHeightFieldImplementation(const CRCore::crHeightField& obj, const std::string& fileName);

        crReaderWriter::WriteResult writeNode(const CRCore::crNode& node, const std::string& fileName)
        {
            if (m_writeFileCallback.valid()) return m_writeFileCallback->writeNode(node,fileName);
            else return writeNodeImplementation(node,fileName);
        }
        crReaderWriter::WriteResult writeNodeImplementation(const CRCore::crNode& node, const std::string& fileName);
        
        

        void setCreateNodeFromImage(bool flag) { m_createNodeFromImage = flag; }
        bool getCreateNodeFromImage() const { return m_createNodeFromImage; }


        void setOptions(crReaderWriter::Options* opt) { m_options = opt; }
        crReaderWriter::Options* getOptions() { return m_options.get(); }
        const crReaderWriter::Options*  getOptions() const { return m_options.get(); }


        /** initilize both the Data and Library FilePaths, by default called by the 
          * constructor, so it should only be required if you want to force
          * the re-reading of environmental variables.*/
        void initFilePathLists() { initDataFilePathList(); initLibraryFilePathList(); }
        
        /** initilize the Data FilePath by reading the OSG_FILE_PATH environmental variable.*/
        void initDataFilePathList();

        /** Set the data file path using a list of paths stored in a FilePath, which is used when search for data files.*/
        void setDataFilePathList(const FilePathList& filepath) { m_dataFilePath = filepath; }

        /** Set the data file path using a single string deliminated either with ';' (Windows) or ':' (All other platforms), which is used when search for data files.*/
        void setDataFilePathList(const std::string& paths);

        /** get the data file path which is used when search for data files.*/
        FilePathList& getDataFilePathList() { return m_dataFilePath; }

        /** get the const data file path which is used when search for data files.*/
        const FilePathList& getDataFilePathList() const { return m_dataFilePath; }

        /** initilize the Library FilePath by reading the OSG_LIBRARY_PATH 
          * and the appropriate system environmental variables*/
        void initLibraryFilePathList();

        /** Set the library file path using a list of paths stored in a FilePath, which is used when search for data files.*/
        void setLibraryFilePathList(const FilePathList& filepath) { m_libraryFilePath = filepath; }

        /** Set the library file path using a single string deliminated either with ';' (Windows) or ':' (All other platforms), which is used when search for data files.*/
        void setLibraryFilePathList(const std::string& paths);

        /** get the library file path which is used when search for library (dso/dll's) files.*/
        FilePathList& getLibraryFilePathList() { return m_libraryFilePath; }
        
        /** get the const library file path which is used when search for library (dso/dll's) files.*/
        const FilePathList& getLibraryFilePathList() const { return m_libraryFilePath; }

        /** For each object in the cache which has an reference count greater than 1 
          * (and therefore referenced by elsewhere in the application) set the time stamp
          * for that object in the cache to specified time.
          * This would typically be called once per frame by applications which are doing database paging,
          * and need to prune objects that are no longer required.
          * Time value is time in sceonds.*/
        void updateTimeStampOfObjectsInCacheWithExtenalReferences(double currentTime);

        /** Removed object in the cache which have a time stamp at or before the specified expiry time.
          * This would typically be called once per frame by applications which are doing database paging,
          * and need to prune objects that are no longer required, and called after the a called
          * after the call to updateTimeStampOfObjectsInCacheWithExtenalReferences(currentTime). 
          * Note, the currentTime is not the expiryTime, one would typically set the expiry time
          * to a fixed amount of time before currentTime, such as expiryTime = currentTime-10.0.
          * Time value is time in sceonds.*/
        void removeExpiredObjectsInCache(double expiryTime);
        
        /** Remove all objects in the cache regardless of having external references or expiry times.*/ 
        void clearObjectCache();

        /** Add a filename,object,timestamp tripple to the crRegistry::ObjectCache.*/
        void addEntryToObjectCache(const std::string& filename, CRCore::crBase* object, double timestamp = 0.0);

	/** Get an object from the object cache*/ 
	CRCore::crBase* getFromObjectCache(const std::string& fileName);
        
        /** Add archive to archive cache so that future calls reference this archive.*/
        void addToArchiveCache(const std::string& fileName, CRIOManager::crArchive* archive);

        /** Remove archive from cache.*/
        void removeFromArchiveCache(const std::string& fileName);
        
        /** Get an archive from the archive cache*/ 
        CRIOManager::crArchive* getFromArchiveCache(const std::string& fileName);
        
        /** Remove all archives from the archive cache.*/
        void clearArchiveCache();
        
         /** If crState is non-zero, this function releases OpenGL objects for
           * the specified graphics context. Otherwise, releases OpenGL objexts
           * for all graphics contexts. */
        void releaseObjects(CRCore::crState* state=0);

        /** get the attached library with specified name.*/
        crDynamicLibrary*              getLibrary(const std::string& fileName);
        
        /** Set the crDatabasePager.*/
        void setDatabasePager(crDatabasePager* databasePager) { m_databasePager = databasePager; }

        /** Get the crDatabasePager, creating one if one is not already created.*/
        crDatabasePager* getOrCreateDatabasePager();
        
        /** Get the crDatabasePager. Return 0 if no crDatabasePager has been assigned.*/
        crDatabasePager* getDatabasePager() { return m_databasePager.get(); }

        
        /** Set the crSharedStateManager.*/
        void setSharedStateManager(crSharedStateManager* crSharedStateManager) { m_sharedStateManager = crSharedStateManager; }

        /** Get the crSharedStateManager, creating one if one is not already created.*/
        crSharedStateManager* getOrCreateSharedStateManager();
        
        /** Get the crSharedStateManager. Return 0 if no crSharedStateManager has been assigned.*/
        crSharedStateManager* getSharedStateManager() { return m_sharedStateManager.get(); }

        virtual void clear();
    protected:

        virtual ~crRegistry();
		static CRCore::ref_ptr<crRegistry> m_instance;
        typedef std::vector< CRCore::ref_ptr<crReaderWriter> >               ReaderWriterList;
		typedef std::vector< CRCore::ref_ptr<crScriptLoader> >               ScriptLoaderList;
        typedef std::map< std::string, CRCore::ref_ptr<crDotCreWrapper> >    DotCreWrapperMap;
        typedef std::vector< CRCore::ref_ptr<crDynamicLibrary> >             DynamicLibraryList;
        typedef std::map< std::string, std::string>                     ExtensionAliasMap;
        
        typedef std::pair<CRCore::ref_ptr<CRCore::crBase>, double >           ObjectTimeStampPair;
        typedef std::map<std::string, ObjectTimeStampPair >             ObjectCache;
        typedef std::map<std::string, CRCore::ref_ptr<CRIOManager::crArchive> >    ArchiveCache;

        /** constructor is private, as its a singleton, preventing
            construction other than via the instance() method and
            therefore ensuring only one copy is ever constructed*/
        crRegistry();
        
        /** get the attached library with specified name.*/
        DynamicLibraryList::iterator getLibraryItr(const std::string& fileName);

        bool m_createNodeFromImage;

        CRCore::crBase*       readObject(DotCreWrapperMap& dowMap,crInput& fr);

        void eraseWrapper(DotCreWrapperMap& wrappermap,crDotCreWrapper* wrapper);

    public:
        /** Functor used in internal implementations.*/
        struct ReadFunctor
        {
            ReadFunctor(const std::string& filename, const crReaderWriter::Options* options):
                m_filename(filename),
                m_options(options) {}

            virtual ~ReadFunctor() {}
            virtual crReaderWriter::ReadResult doRead(crReaderWriter& rw) const = 0;
            virtual bool isValid(crReaderWriter::ReadResult& readResult) const = 0;
            virtual bool isValid(CRCore::crBase* object) const = 0;

            std::string m_filename;
            const crReaderWriter::Options* m_options;
        };

   protected:
        // forward declare helper classes
        struct ReadObjectFunctor;
        struct ReadImageFunctor;
        struct ReadHeightFieldFunctor;
        struct ReadNodeFunctor;
        struct ReadArchiveFunctor;
        
        // make helper classes friends to get round VS6.0 "issues"
        friend struct ReadFunctor;
        friend struct ReadObjectFunctor;
        friend struct ReadImageFunctor;
        friend struct ReadHeightFieldFunctor;
        friend struct ReadNodeFunctor;
        friend struct ReadArchiveFunctor;

        crReaderWriter::ReadResult read(const ReadFunctor& readFunctor);
        crReaderWriter::ReadResult readImplementation(const ReadFunctor& readFunctor,bool useObjectCache);


        // forward declar helper class
        class AvailableReaderWriterIterator;
        friend class AvailableReaderWriterIterator;


        CRCore::ref_ptr<ReadFileCallback>      m_readFileCallback;
        CRCore::ref_ptr<WriteFileCallback>     m_writeFileCallback;

        DotCreWrapperMap   m_objectWrapperMap;
        DotCreWrapperMap   m_imageWrapperMap;
        DotCreWrapperMap   m_drawableWrapperMap;
        DotCreWrapperMap   m_stateAttrWrapperMap;
        DotCreWrapperMap   m_uniformWrapperMap;
        DotCreWrapperMap   m_nodeWrapperMap;
        
        DotCreWrapperMap   m_classNameWrapperMap;

        ReaderWriterList    m_rwList;
		ScriptLoaderList    m_slList;
        DynamicLibraryList  m_dlList;

        bool m_openingLibrary;
    
        // map to alias to extensions to plugins.
        ExtensionAliasMap  m_extAliasMap;
        
        // options to pass to reader writers.
        CRCore::ref_ptr<crReaderWriter::Options>     m_options;
        
        FilePathList                            m_dataFilePath;
        FilePathList                            m_libraryFilePath;

        ObjectCache                             m_objectCache;
        CRCore::crMutex                      m_objectCacheMutex;
        
        ArchiveCache                            m_archiveCache;
        CRCore::crMutex                      m_archiveCacheMutex;
        
        
        CRCore::ref_ptr<crDatabasePager>             m_databasePager;
        CRCore::ref_ptr<crSharedStateManager>        m_sharedStateManager;

};

/**  Proxy class for automatic registration of DotOsgWrappers with the crRegistry.*/
class crRegisterDotCreWrapperProxy
{
    public:
    
        crRegisterDotCreWrapperProxy(CRCore::crBase* proto,
                                   const std::string& name,
                                   const std::string& associates,
                                   crDotCreWrapper::ReadFunc readFunc,
                                   crDotCreWrapper::WriteFunc writeFunc,
                                   crDotCreWrapper::ReadWriteMode readWriteMode=crDotCreWrapper::READ_AND_WRITE)
        {
            if (crRegistry::instance())
            {
                m_wrapper = new crDotCreWrapper(proto,name,associates,readFunc,writeFunc,readWriteMode);
                crRegistry::instance()->addDotCreWrapper(m_wrapper.get());
            }
        }
        
        ~crRegisterDotCreWrapperProxy()
        {
            if (crRegistry::instance())
            {
                crRegistry::instance()->removeDotCreWrapper(m_wrapper.get());
            }
        }
        
    protected:
        CRCore::ref_ptr<crDotCreWrapper> m_wrapper;
};

/** Proxy class for automatic registration of reader/writers with the crRegistry.*/
template<class T>
class RegisterReaderWriterProxy
{
    public:
        RegisterReaderWriterProxy()
        {
            if (crRegistry::instance())
            {
                m_rw = new T;
                crRegistry::instance()->addReaderWriter(m_rw.get());
            }
        }

        ~RegisterReaderWriterProxy()
        {
            if (crRegistry::instance())
            {
                crRegistry::instance()->removeReaderWriter(m_rw.get());
            }
        }
        
        T* get() { return m_rw.get(); }
        
    protected:
        CRCore::ref_ptr<T> m_rw;
};

/** Proxy class for automatic registration of reader/writers with the crRegistry.*/
template<class T>
class RegisterScriptLoaderProxy
{
public:
	RegisterScriptLoaderProxy()
	{
		if (crRegistry::instance())
		{
			m_loader = new T;
			crRegistry::instance()->addScriptLoader(m_loader.get());
		}
	}

	~RegisterScriptLoaderProxy()
	{
		if (crRegistry::instance())
		{
			crRegistry::instance()->removeScriptLoader(m_loader.get());
		}
	}

	T* get() { return m_loader.get(); }

protected:
	CRCore::ref_ptr<T> m_loader;
};

}

#endif
