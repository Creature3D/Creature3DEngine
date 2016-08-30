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
#ifndef CRIOMANAGER_READERWRITER
#define CRIOMANAGER_READERWRITER 1

#include <CRCore/Referenced.h>
#include <CRCore/crImage.h>
#include <CRCore/crShape.h>
#include <CRCore/crNode.h>

#include <CRIOManager/crExport.h>

#include <deque>

namespace CRIOManager {

class crArchive;

/** list of directories to search through which searching for files. */
typedef std::deque<std::string> FilePathList;

/** pure virtual base class for reading and writing of non native formats. */
class CRIOMANAGER_EXPORT crReaderWriter : public CRCore::crBase
{
    public:
    
    
        crReaderWriter() {}
        crReaderWriter(const crReaderWriter& rw,const CRCore::crCopyOp copyop=CRCore::crCopyOp::SHALLOW_COPY):
            CRCore::crBase(rw,copyop) {}

        virtual ~crReaderWriter();

        META_Base(CRIOManager,crReaderWriter);

        virtual bool acceptsExtension(const std::string& /*extension*/) const { return false; }

        /** Options base class used for passing options into plugins to control their operation.*/
        class Options : public CRCore::crBase
        {
            public:
            
            
                /// bit mask for setting up which object types get cached by readObject/crImage/crHeightField/crNode(filename) calls
                enum CacheHintOptions
                {   /// do not cache objects of any type
                    CACHE_NONE          = 0,

                    /// cache nodes loaded via readNode(filename)
                    CACHE_NODES         = 1,

                    /// cache images loaded via readImage(filename)
                    CACHE_IMAGES        = 2,

                    /// cache heightfield loaded via readHeightField(filename)
                    CACHE_HEIGHTFIELDS  = 4, 

                    /// cache heightfield loaded via readHeightField(filename)
                    CACHE_ARCHIVES      = 8, 

                    /// cache objects loaded via readObject(filename)
                    CACHE_OBJECTS       = 16, 

                    /// cache on all read*(filename) calls
                    CACHE_ALL           = CACHE_NODES |
                                          CACHE_IMAGES |
                                          CACHE_HEIGHTFIELDS |
                                          CACHE_ARCHIVES |
                                          CACHE_OBJECTS
                };
            

                Options():m_objectCacheHint(CACHE_ARCHIVES) {}
                Options(const std::string& str):m_str(str) {}
                
                Options(const Options& options,const CRCore::crCopyOp copyop=CRCore::crCopyOp::SHALLOW_COPY):
                    CRCore::crBase(options,copyop),
                    m_str(options.m_str),
                    m_databasePaths(options.m_databasePaths),
                    m_objectCacheHint(options.m_objectCacheHint) {}

                META_Base(CRIOManager,Options);

                /** Set the general Options string.*/
                void setOptionString(const std::string& str) { m_str = str; }

                /** Get the general Options string.*/
                const std::string& getOptionString() const { return m_str; }

                /** Set the database path to use a hint of where to look when loading models.*/
                void setDatabasePath(const std::string& str) { m_databasePaths.clear();  m_databasePaths.push_back(str); }

                /** Get the database path which is used a hint of where to look when loading models.*/
                FilePathList& getDatabasePathList() { return m_databasePaths; }

                /** Get the const database path which is used a hint of where to look when loading models.*/
                const FilePathList& getDatabasePathList() const { return m_databasePaths; }

                /** Set whether the crRegistry::ObjectCache should be used by default.*/
                void setObjectCacheHint(CacheHintOptions useObjectCache) { m_objectCacheHint = useObjectCache; }

                /** Get whether the crRegistry::ObjectCache should be used by default.*/
                CacheHintOptions getObjectCacheHint() const { return m_objectCacheHint; }


            protected:

                virtual ~Options() {}

                std::string         m_str;
                FilePathList        m_databasePaths;
                CacheHintOptions    m_objectCacheHint;

        };


        class CRIOMANAGER_EXPORT ReadResult
        {
            public:

                enum ReadStatus
                {
                    FILE_NOT_HANDLED,
                    FILE_NOT_FOUND,
                    FILE_LOADED,
                    FILE_LOADED_FROM_CACHE,
                    ERROR_IN_READING_FILE
                };

                ReadResult(ReadStatus status=FILE_NOT_HANDLED):m_status(status) {}
                ReadResult(const std::string& m):m_status(ERROR_IN_READING_FILE),m_message(m) {}
                ReadResult(CRCore::crBase* obj, ReadStatus status=FILE_LOADED):m_status(status),m_object(obj) {}
                
                ReadResult(const ReadResult& rr):m_status(rr.m_status),m_message(rr.m_message),m_object(rr.m_object) {}
                ReadResult& operator = (const ReadResult& rr) { if (this==&rr) return *this; m_status=rr.m_status; m_message=rr.m_message;m_object=rr.m_object; return *this; }
                
                CRCore::crBase* getObject();
                CRCore::crImage* getImage();
                CRCore::crHeightField* getHeightField();
                CRCore::crNode* getNode();
                CRIOManager::crArchive* getArchive();
                
                bool validObject() { return m_object.valid(); }
                bool validImage() { return getImage()!=0; }
                bool validHeightField() { return getHeightField()!=0; }
                bool validNode() { return getNode()!=0; }
                bool validArchive() { return getArchive()!=0; }

                CRCore::crBase* takeObject();
                CRCore::crImage* takeImage();
                CRCore::crHeightField* takeHeightField();
                CRCore::crNode* takeNode();
                CRIOManager::crArchive* takeArchive();

                const std::string& message() const { return m_message; }

                ReadStatus status() const { return m_status; }
                bool success() const { return m_status==FILE_LOADED || m_status==FILE_LOADED_FROM_CACHE ; }
                bool loadedFromCache() const { return m_status==FILE_LOADED_FROM_CACHE; }
                bool error() const { return m_status==ERROR_IN_READING_FILE; }
                bool notHandled() const { return m_status==FILE_NOT_HANDLED; }
                bool notFound() const { return m_status==FILE_NOT_FOUND; }

            protected:
            
                ReadStatus                  m_status;
                std::string                 m_message;
                CRCore::ref_ptr<CRCore::crBase>   m_object;
        };

        class WriteResult
        {
            public:

                enum WriteStatus
                {
                    FILE_NOT_HANDLED,
                    FILE_SAVED,
                    ERROR_IN_WRITING_FILE
                };

                WriteResult(WriteStatus status=FILE_NOT_HANDLED):m_status(status) {}
                WriteResult(const std::string& m):m_status(ERROR_IN_WRITING_FILE),m_message(m) {}
                
                WriteResult(const WriteResult& rr):m_status(rr.m_status),m_message(rr.m_message) {}
                WriteResult& operator = (const WriteResult& rr) { if (this==&rr) return *this; m_status=rr.m_status; m_message=rr.m_message; return *this; }
                
                const std::string& message() const { return m_message; }

                WriteStatus status() const { return m_status; }
                bool success() const { return m_status==FILE_SAVED; }
                bool error() const { return m_status==ERROR_IN_WRITING_FILE; }
                bool notHandled() const { return m_status==FILE_NOT_HANDLED; }

            protected:
            
                WriteStatus                 m_status;
                std::string                 m_message;
        };

        enum ArchiveStatus
        {
            READ,
            WRITE,
            CREATE
        };

        /** open an archive for reading, writing or or to create an empty archive for writing to.*/ 
        virtual ReadResult openArchive(const std::string& /*fileName*/,ArchiveStatus, unsigned int =4096, const Options* =NULL)const { return ReadResult(ReadResult::FILE_NOT_HANDLED); }

        /** open an archive for reading.*/ 
        virtual ReadResult openArchive(std::istream& /*fin*/,const Options* =NULL)const { return ReadResult(ReadResult::FILE_NOT_HANDLED); }

        virtual ReadResult readObject(const std::string& /*fileName*/,const Options* =NULL)const { return ReadResult(ReadResult::FILE_NOT_HANDLED); }
        virtual ReadResult readImage(const std::string& /*fileName*/,const Options* =NULL)const { return ReadResult(ReadResult::FILE_NOT_HANDLED); }
        virtual ReadResult readHeightField(const std::string& /*fileName*/,const Options* =NULL)const { return ReadResult(ReadResult::FILE_NOT_HANDLED); }
        virtual ReadResult readNode(const std::string& /*fileName*/,const Options* =NULL)const { return ReadResult(ReadResult::FILE_NOT_HANDLED); }

        virtual WriteResult writeObject(const CRCore::crBase& /*obj*/,const std::string& /*fileName*/,const Options* =NULL)const {return WriteResult(WriteResult::FILE_NOT_HANDLED); }
        virtual WriteResult writeImage(const CRCore::crImage& /*image*/,const std::string& /*fileName*/,const Options* =NULL)const {return WriteResult(WriteResult::FILE_NOT_HANDLED); }
        virtual WriteResult writeHeightField(const CRCore::crHeightField& /*heightField*/,const std::string& /*fileName*/,const Options* =NULL)const {return WriteResult(WriteResult::FILE_NOT_HANDLED); }
        virtual WriteResult writeNode(const CRCore::crNode& /*node*/,const std::string& /*fileName*/,const Options* =NULL)const { return WriteResult(WriteResult::FILE_NOT_HANDLED); }

        virtual ReadResult readObject(std::istream& /*fin*/,const Options* =NULL)const { return ReadResult(ReadResult::FILE_NOT_HANDLED); }
        virtual ReadResult readImage(std::istream& /*fin*/,const Options* =NULL)const { return ReadResult(ReadResult::FILE_NOT_HANDLED); }
        virtual ReadResult readHeightField(std::istream& /*fin*/,const Options* =NULL)const { return ReadResult(ReadResult::FILE_NOT_HANDLED); }
        virtual ReadResult readNode(std::istream& /*fin*/,const Options* =NULL)const { return ReadResult(ReadResult::FILE_NOT_HANDLED); }

        virtual WriteResult writeObject(const CRCore::crBase& /*obj*/,std::ostream& /*fout*/,const Options* =NULL)const { return WriteResult(WriteResult::FILE_NOT_HANDLED); }
        virtual WriteResult writeImage(const CRCore::crImage& /*image*/,std::ostream& /*fout*/,const Options* =NULL)const { return WriteResult(WriteResult::FILE_NOT_HANDLED); }
        virtual WriteResult writeHeightField(const CRCore::crHeightField& /*heightField*/,std::ostream& /*fout*/,const Options* =NULL)const { return WriteResult(WriteResult::FILE_NOT_HANDLED); }
        virtual WriteResult writeNode(const CRCore::crNode& /*node*/,std::ostream& /*fout*/,const Options* =NULL)const { return WriteResult(WriteResult::FILE_NOT_HANDLED); }

};

}

#endif // CRIOMANAGER_READERWRITER
