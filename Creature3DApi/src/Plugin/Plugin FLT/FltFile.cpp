#include <Plugin flt/FltFile.h>
#include <Plugin flt/Registry.h>
#include <Plugin flt/Record.h>
#include <Plugin flt/RecordVisitor.h>
#include <Plugin flt/ExternalRecord.h>
#include <Plugin flt/flt2osg.h>        // ConvertFromFLT
#include <Plugin flt/Input.h>

#include <CRCore/crNode.h>
#include <CRCore/crNodeVisitor.h>
#include <CRCore/crNotify.h>

#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crFileNameUtils.h>

//#include <CRSim/crGeographicLocation.h>

#include <string>


using namespace flt;


FltFile::FltFile(
    ColorPool* pColorPool,
    TexturePool* pTexturePool,
    MaterialPool* pMaterialPool,
    LtPtAppearancePool* pLtPtAppearancePool,
    LtPtAnimationPool* pLtPtAnimationPool)
{
    m_useTextureAlphaForTransparancyBinning = true;
    m_doUnitsConversion = true;
    m_desiredUnits = ConvertToMeters;

    if (pColorPool)
    {
        // use external color palette, ignore internal
        m_useInternalColorPalette = false;
        setColorPool( pColorPool );
    }
    else
    {
        // use internal color palette
        m_useInternalColorPalette = true;
        setColorPool( new ColorPool );
    }

    if (pTexturePool)
    {
        // use external texture palette, ignore internal
        m_useInternalTexturePalette = false;
        setTexturePool( pTexturePool );
    }
    else
    {
        // use internal texture palette
        m_useInternalTexturePalette = true;
        setTexturePool( new TexturePool );
    }

    if (pMaterialPool)
    {
        // use external material palette, ignore internal
        m_useInternalMaterialPalette = false;
        setMaterialPool( pMaterialPool );
    }
    else
    {
        // use internal material palette
        m_useInternalMaterialPalette = true;
        setMaterialPool( new MaterialPool );
    }

    if (pLtPtAppearancePool && pLtPtAnimationPool) // Can only be non-NULL if parent is 15.8.
    {
        // use external light point appearance and animation palettes, ignore internal
        m_useInternalLtPtPalettes = false;
        setLtPtAppearancePool( pLtPtAppearancePool );
        setLtPtAnimationPool( pLtPtAnimationPool );
    }
    else
    {
		// If they aren't both set, then they must both be NULL.
		assert( (pLtPtAppearancePool==NULL) && (pLtPtAppearancePool==NULL) );
        // use internal light point palettes
        m_useInternalLtPtPalettes = true;
        setLtPtAppearancePool( new LtPtAppearancePool );
        setLtPtAnimationPool( new LtPtAnimationPool );
    }

    // no support for external light palettes
    setLightPool( new LightPool );

    // instances are always internally defined 
    setInstancePool( new InstancePool );
}


CRCore::crBase* FltFile::readObject(const std::string& fileName)
{
    return readNode(fileName);
}


CRCore::crNode* FltFile::readNode(const std::string& fileName)
{
    m_directory = CRIOManager::getFilePath(fileName);

    if (readModel(fileName))
    {
        // Convert record tree to CRCore scene graph
        CRCore::crNode* model = convert();
        
        if (model)
        {
           // // Store model origin in returned crNode userData.
           // CRCore::ref_ptr<CRSim::crGeographicLocation> loc = new CRSim::crGeographicLocation;
           // double lat, lon;
           // getOrigin( lat, lon );
           // loc->set( lat, lon );
           //// model->setUserData( loc.get() );
           // 
           // CRCore::notify(CRCore::INFO)<<"FltFile::readNode("<<fileName<<") lat="<<lat<<" lon="<<lon<<std::endl;
        
            return model;
        }
    }

    return NULL;
}


CRCore::crGroup* FltFile::convert()
{
    ConvertFromFLT visit;
    visit.setUseTextureAlphaForTransparancyBinning(getUseTextureAlphaForTransparancyBinning());
    visit.setDoUnitsConversion(getDoUnitsConversion());
    return visit.convert(getHeaderRecord());
}


// Read flight model (include externals)
bool FltFile::readModel(const std::string& fileName)
{
    if (readFile(fileName))
    {
        readExternals();
        return getHeaderRecord() ? true : false;
    }
    return false;
}


bool FltFile::readFile(const std::string& fileName)
{

    FileInput fin;
    if (!fin.open(fileName)) 
    {
        // havn't found file, look in OSGFILEPATH
        std::string foundFileName = CRIOManager::findDataFile(fileName);

        if (foundFileName.empty()) return false;
        if (!fin.open(foundFileName)) return false;
    }

    //CRCore::notify(CRCore::INFO) << "Loading " << fileName << " ... " << std::endl;

    Record* pRec = fin.readCreateRecord(this);
    if (pRec == NULL)
    {
        //CRCore::notify(CRCore::WARN) << "File not found " << fileName << std::endl;
        return false;
    }

    m_headerRecord = (HeaderRecord*)pRec;
    if (pRec->isPrimaryNode())   // Header
        pRec->readLocalData(fin);// Read rest of file

    fin.close();

    return true;
}


#define REGISTER_FLT 1

// This class was originally scoped within FltFile::readExternals() function.
// Irix 7.3 compilers hork on this.

    class ReadExternal : public RecordVisitor
    {
        public:
            ReadExternal(FltFile* fltFile)
            {
                _pFltFile = fltFile;
                setTraverseMode(RecordVisitor::TRAVERSE_ALL_CHILDREN);
            }

            virtual void apply(ExternalRecord& rec)
            {
                SExternalReference* pSExternal = (SExternalReference*)rec.getData();

                if (pSExternal)
                {
                    FltFile*        pExternalFltFile = NULL;
                    ColorPool*      pColorPool    = NULL;
                    TexturePool*    pTexturePool  = NULL;
                    MaterialPool*   pMaterialPool = NULL;
                    LtPtAppearancePool* pLtPtAppearancePool = NULL;
                    LtPtAnimationPool* pLtPtAnimationPool = NULL;
                    std::string filename(pSExternal->szPath);

                    //CRCore::notify(CRCore::INFO) << "External=" << filename << std::endl;

                    if (rec.getFlightVersion() > 13)
                    {
                        if (!(pSExternal->dwFlags & ExternalRecord::COLOR_PALETTE_OVERRIDE))
                            pColorPool = _pFltFile->getColorPool();

                        if (!(pSExternal->dwFlags & ExternalRecord::TEXTURE_PALETTE_OVERRIDE))
                            pTexturePool = _pFltFile->getTexturePool();

                        if (!(pSExternal->dwFlags & ExternalRecord::MATERIAL_PALETTE_OVERRIDE))
                            pMaterialPool = _pFltFile->getMaterialPool();

                        if (rec.getFlightVersion() >= 1580)
                        {
                            if (!(pSExternal->dwFlags & ExternalRecord::LIGHT_POINT_PALETTE_OVERRIDE))
							{
                                pLtPtAppearancePool = _pFltFile->getLtPtAppearancePool();
                                pLtPtAnimationPool = _pFltFile->getLtPtAnimationPool();
							}
                        }
                    }

    #if REGISTER_FLT
                    pExternalFltFile = Registry::instance()->getFltFile(filename);
					CRIOManager::FilePathList fpl = CRIOManager::getDataFilePathList();

                    if (pExternalFltFile == NULL)
                    {
                        //Path for Nested external references
                        std::string filePath = CRIOManager::getFilePath(filename);
                        std::string pushAndPopPath;
                        //If absolute path
                        if( (filePath.length()>0 && filePath.find_first_of("/\\")==0) ||
                            (filePath.length()>2 && filePath.substr(1,1)==":" && filePath.find_first_of("/\\")==2) )
                        {
                            pushAndPopPath = filePath;
                        }
                        else
                        {
                            pushAndPopPath = fpl.empty() ? "." : fpl.front();
                            if(pushAndPopPath.empty()) pushAndPopPath = ".";
                            pushAndPopPath += "/" + filePath;
                        }
                        //CRIOManager::PushAndPopDataPath tmpfile(pushAndPopPath);
						fpl.push_back(pushAndPopPath);


                        pExternalFltFile = new FltFile( pColorPool, pTexturePool, pMaterialPool,
                            pLtPtAppearancePool, pLtPtAnimationPool );                        
                        pExternalFltFile->readModel(filename);
                    }
                    Registry::instance()->addFltFile(filename, pExternalFltFile);
    #else
                    pExternalFltFile = new FltFile( pColorPool, pTexturePool, pMaterialPool,
                        pLtPtAppearancePool, pLtPtAnimationPool );
                    pExternalFltFile->readModel(filename);
    #endif
                    rec.setExternal(pExternalFltFile);
                }
            }

        public:

            FltFile* _pFltFile;
    };

void FltFile::readExternals()
{
    ReadExternal visitor(this);
    m_headerRecord->accept(visitor);
}


int FltFile::getFlightVersion() const
{
    if (m_headerRecord.get())
    {
        SHeader* pSHeader = (SHeader*)m_headerRecord.get()->getData();
        if (pSHeader)
            return pSHeader->diFormatRevLev;
    }
    return 0;
}


void FltFile::getOrigin( double& latitude, double& longitude ) const
{
    if (m_headerRecord.get())
    {
        SHeader* pSHeader = (SHeader*)m_headerRecord.get()->getData();
        if (pSHeader)
        {
            latitude = pSHeader->Origin.x();
            longitude = pSHeader->Origin.y();
        }
    }
}


std::string FltFile::getDesiredUnitsString() const
{
    switch (m_desiredUnits)
    {
    case ConvertToMeters:
        return "ConvertToMeters";
        break;
    case ConvertToKilometers:
        return "ConvertToKilometers";
        break;
    case ConvertToFeet:
        return "ConvertToFeet";
        break;
    case ConvertToInches:
        return "ConvertToInches";
        break;
    case ConvertToNauticalMiles:
        return "ConvertToNauticalMiles";
        break;
    default:
        return "Invalid";
        break;
    }
}

