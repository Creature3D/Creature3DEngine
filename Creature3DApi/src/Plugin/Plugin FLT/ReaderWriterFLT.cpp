// ReaderWriterFLT.cpp

#if defined(_MSC_VER)
#pragma warning( disable : 4786 )
#endif

#include <Plugin flt\ReaderWriterFLT.h>
#include <Plugin flt\Registry.h>
#include <Plugin flt\FltFile.h>

#include <CRCore/crBase.h>
#include <CRCore/crNode.h>
#include <CRCore/crNotify.h>

#include <CRIOManager/crRegistry.h>
#include <CRIOManager/crFileUtils.h>

using namespace flt;

CRIOManager::crReaderWriter::ReadResult ReaderWriterFLT::readObject(const std::string& fileName, const CRIOManager::crReaderWriter::Options* opt)const
{
    return readNode(fileName,opt);
}


CRIOManager::crReaderWriter::ReadResult ReaderWriterFLT::readNode(const std::string& file, const CRIOManager::crReaderWriter::Options* options)const
{
	// serialize all access to the OpenFlight plugin as its not thread safe by itself.
	CRCore::ScopedLock<CRIOManager::crReentrantMutex> lock(_serializerMutex);

    std::string ext = CRIOManager::getLowerCaseFileExtension(file);
    if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

    std::string fileName = CRIOManager::findDataFile( file );
    if (fileName.empty()) return ReadResult::FILE_NOT_FOUND;

    CRCore::ref_ptr<FltFile> read = new FltFile;

    if (options)
    {
        read->setUseTextureAlphaForTransparancyBinning(options->getOptionString().find("noTextureAlphaForTransparancyBinning")==std::string::npos);
        //CRCore::notify(CRCore::DEBUG_INFO) << "FltFile.getUseTextureAlphaForTransparancyBinning()=" << read->getUseTextureAlphaForTransparancyBinning() << std::endl;
        read->setDoUnitsConversion((options->getOptionString().find("noUnitsConversion")==std::string::npos)); // default to true, unless noUnitsConversion is specified.o
        //CRCore::notify(CRCore::DEBUG_INFO) << "FltFile.getDoUnitsConversion()=" << read->getDoUnitsConversion() << std::endl;

        if (read->getDoUnitsConversion())
        {
            if (options->getOptionString().find("convertToFeet")!=std::string::npos)
                read->setDesiredUnits( FltFile::ConvertToFeet );
            else if (options->getOptionString().find("convertToInches")!=std::string::npos)
                read->setDesiredUnits( FltFile::ConvertToInches );
            else if (options->getOptionString().find("convertToMeters")!=std::string::npos)
                read->setDesiredUnits( FltFile::ConvertToMeters );
            else if (options->getOptionString().find("convertToKilometers")!=std::string::npos)
                read->setDesiredUnits( FltFile::ConvertToKilometers );
            else if (options->getOptionString().find("convertToNauticalMiles")!=std::string::npos)
                read->setDesiredUnits( FltFile::ConvertToNauticalMiles );
            //CRCore::notify(CRCore::DEBUG_INFO) << "FltFile.getDesiredUnits()=" << read->getDesiredUnitsString() << std::endl;
        }
    }

    CRCore::crNode* node = read->readNode(fileName);

    flt::Registry::instance()->clearObjectCache();

    if (node) return node;
    else return ReadResult::FILE_NOT_HANDLED;
}


// now register with Registry to instantiate the above
// reader/writer.
static CRIOManager::RegisterReaderWriterProxy<ReaderWriterFLT> g_fltReaderWriterProxy;
