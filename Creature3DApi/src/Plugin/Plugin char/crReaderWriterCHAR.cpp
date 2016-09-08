// -*-c++-*-

/*
 * ReplicantBody CHAR loader for Creature3D
 *
 * Copyright (C) 2003 Anders Backman
 *
 * The Open Scene Graph (OSG) is a cross platform C++/OpenGL library for 
 * real-time rendering of large 3D photo-realistic models. 
 * The OSG homepage is http://www.openscenegraph.org/
 */

#if defined(_MSC_VER)
    #pragma warning( disable : 4786 )
#endif

#include <string>

#include <CRCore/crNotify.h>
#include <CRCore/crNode.h>
#include <CRCore/crMatrixTransform.h>

#include <CRIOManager/crRegistry.h>
#include <CRIOManager/crReadFile.h>
#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crFileNameUtils.h>
#include <CRIOManager/crCookFile.h>
#include "rbody/Creature3D/ReplicantBodyMgr.h"



class crReaderWriterCHAR : public CRIOManager::crReaderWriter
{
public:
    crReaderWriterCHAR() { }

    virtual const char* className() { return "ReplicantBody Character file reader"; }
    virtual bool acceptsExtension(const std::string& extension) const {
        return CRIOManager::equalCaseInsensitive(extension,"char");
    }

    virtual ReadResult readNode(const std::string& fileName, const CRIOManager::crReaderWriter::Options*) const;

protected:



};


// register with crRegistry to instantiate the above reader/writer.
static CRIOManager::RegisterReaderWriterProxy<crReaderWriterCHAR> g_objReaderWriterProxy;


// read file and convert to Creatrue3D.
CRIOManager::crReaderWriter::ReadResult crReaderWriterCHAR::readNode(const std::string& file, const CRIOManager::crReaderWriter::Options*) const
{
    std::string ext = CRIOManager::getFileExtension(file);
    if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

	//CRIOManager::crLoadCookFile scopedLoad(file);
 //   std::string fileName = CRIOManager::findDataFile( file );
 //   if (fileName.empty()) return ReadResult::FILE_NOT_FOUND;
  
    // toplevel group or transform
    CRCore::crGroup* root = 0;

    //try {

      root = g_ReplicantBodyMgr->readCharacterFile(file);
    //}
    //catch (std::exception& e) {
    //  CRCore::notify(CRCore::WARN) << e.what() << std::endl;
    //  ReadResult::ERROR_IN_READING_FILE;
    //}

    return root;
}

