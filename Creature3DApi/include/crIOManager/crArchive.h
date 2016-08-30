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
#ifndef CRIOMANAGER_ARCHIVE
#define CRIOMANAGER_ARCHIVE 1

#include <CRIOManager/crReaderWriter.h>
#include <CRIOManager/crRegistry.h>

#include <fstream>
#include <list>

namespace CRIOManager {


/** Base class for implementing database Archives. See src/osgPlugins/osga for an example of a concrete implementation. */
class CRIOMANAGER_EXPORT crArchive : public crReaderWriter
{
    public:
        crArchive();
        virtual ~crArchive(); 

        virtual const char* libraryName() const { return "CRIOManager"; }

        virtual const char* className() const { return "crArchive"; }

        virtual bool acceptsExtension(const std::string& /*extension*/) const { return true; }
        
        /** close the archive.*/
        virtual void close() = 0;

        /** return true if file exists in archive.*/        
        virtual bool fileExists(const std::string& filename) const = 0;
        
        /** Get the file name which represents the master file recorded in the crArchive.*/
        virtual std::string getMasterFileName() const = 0;
        
        typedef std::vector<std::string> FileNameList;
        
        /** Get the full list of file names available in the archive.*/
        virtual bool getFileNames(FileNameList& fileNameList) const = 0;

        virtual ReadResult readObject(const std::string& /*fileName*/,const Options* =NULL) const = 0;
        virtual ReadResult readImage(const std::string& /*fileName*/,const Options* =NULL) const = 0;
        virtual ReadResult readHeightField(const std::string& /*fileName*/,const Options* =NULL) const = 0;
        virtual ReadResult readNode(const std::string& /*fileName*/,const Options* =NULL) const = 0;

        virtual WriteResult writeObject(const CRCore::crBase& /*obj*/,const std::string& /*fileName*/,const Options* =NULL) const = 0;
        virtual WriteResult writeImage(const CRCore::crImage& /*image*/,const std::string& /*fileName*/,const Options* =NULL) const = 0;
        virtual WriteResult writeHeightField(const CRCore::crHeightField& /*heightField*/,const std::string& /*fileName*/,const Options* =NULL) const = 0;
        virtual WriteResult writeNode(const CRCore::crNode& /*node*/,const std::string& /*fileName*/,const Options* =NULL) const = 0;
           


};

/** Open an archive for reading or writing.*/
CRIOMANAGER_EXPORT crArchive* openArchive(const std::string& filename, crArchive::ArchiveStatus status, unsigned int indexBlockSizeHint=4096);

/** Open an archive for reading or writing.*/
CRIOMANAGER_EXPORT crArchive* openArchive(const std::string& filename, crArchive::ArchiveStatus status, unsigned int indexBlockSizeHint,crReaderWriter::Options* options);
}

#endif // CRIOMANAGER_ARCHIVE
