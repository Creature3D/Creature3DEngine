#include <CRIOManager/crFileNameUtils.h>
#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crRegistry.h>
#include <CRCore/crNotify.h>

#include <Plugin Freetype/FreeTypeLibrary.h>

class ReaderWriterFreeType : public CRIOManager::crReaderWriter
{
    public:
        virtual const char* className() const { return "FreeType crFont Reader/Writer"; }
		
		virtual bool acceptsExtension(const std::string& extension)const
		{
			return CRIOManager::equalCaseInsensitive(extension,"ttf") ||  // true type
				CRIOManager::equalCaseInsensitive(extension,"ttc") ||  // true type
				CRIOManager::equalCaseInsensitive(extension,"pfb") ||  // type1 binary
				CRIOManager::equalCaseInsensitive(extension,"pfa") ||  // type2 ascii
				CRIOManager::equalCaseInsensitive(extension,"cid") ||  // Postscript CID-Fonts
				CRIOManager::equalCaseInsensitive(extension,"cff") ||  // OpenType
				CRIOManager::equalCaseInsensitive(extension,"cef") ||  // OpenType
				CRIOManager::equalCaseInsensitive(extension,"fon") ||  // Windows bitmap fonts
				CRIOManager::equalCaseInsensitive(extension,"fnt") ||    // Windows bitmap fonts
				CRIOManager::equalCaseInsensitive(extension,"text3d");   // use 3D crFont instead of 2D crFont
		}

        virtual ReadResult readObject(const std::string& fileName, const CRIOManager::crReaderWriter::Options* options) const
        {
			std::string ext = CRIOManager::getFileExtension(fileName);
			if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

			CRText::crFont* font = FreeTypeLibrary::instance()->getFont(fileName,0);

			return font;
        }

};

// now register with Registry to instantiate the above
// reader/writer.
static CRIOManager::RegisterReaderWriterProxy<ReaderWriterFreeType> g_readerWriter_FreeType_Proxy;
