//
// OpenFlight?loader for OpenSceneGraph
//
//  Copyright (C) 2005-2006  Brede Johansen
//

#include <stdexcept>
#include <CRCore/crNotify.h>
#include <CRCore/crProxyNode.h>
#include <CRIOManager/crFileNameUtils.h>
#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crRegistry.h>
#include <CRIOManager/crReadFile.h>
#include <CRIOManager/crReentrantMutex.h>
#include <CRUtil/crOptimizer.h>


#include <Plugin OpenFlight\crRegistry.h>
#include <Plugin OpenFlight\Document.h>
#include <Plugin OpenFlight\RecordInputStream.h>


#define SERIALIZER() CRCore::ScopedLock<CRIOManager::crReentrantMutex> lock(m_serializerMutex)  

using namespace flt;
using namespace CRCore;
using namespace CRIOManager;


class crReadExternalsVisitor : public CRCore::crNodeVisitor
{
    CRCore::ref_ptr<crReaderWriter::Options> m_options;

public:

    crReadExternalsVisitor(crReaderWriter::Options* options) :
        CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN),
        m_options(options)
    {
    }
        
    virtual ~crReadExternalsVisitor() {}

    virtual void apply(crProxyNode& node)
    {
        // Transfer ownership of pools.
        //m_options->setUserData( node.getUserData() );
        //node.setUserData(NULL);

        for (unsigned int pos=0; pos<node.getNumFileNames(); pos++)
        {
            std::string filename = node.getFileName(pos);

            // read external
            CRCore::crNode* external = CRIOManager::readNodeFile(filename,m_options.get());
            if (external)
                node.addChild(external);
        }
    }
};


class crFLTReaderWriter : public crReaderWriter
{
    public:
        virtual const char* className() const { return "FLT Reader/Writer"; }

        virtual bool acceptsExtension(const std::string& extension) const
        {
            return equalCaseInsensitive(extension,"fltw");
        }

        virtual ReadResult readObject(const std::string& file, const Options* options) const
        {
            return readNode(file, options);
        }
        
        virtual ReadResult readNode(const std::string& file, const Options* options) const
        {
            SERIALIZER();

            std::string ext = CRIOManager::getLowerCaseFileExtension(file);
            if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

            std::string fileName = CRIOManager::findDataFile(file, options);
            if (fileName.empty()) return ReadResult::FILE_NOT_FOUND;

            // in local cache?
            {
                CRCore::crNode* node = flt::crRegistry::instance()->getFromLocalCache(fileName);
                if (node)
                    return ReadResult(node, crReaderWriter::ReadResult::FILE_LOADED_FROM_CACHE);
            }

            // setting up the database path so that internally referenced file are searched for on relative paths. 
            CRCore::ref_ptr<Options> local_opt = options ? static_cast<Options*>(options->clone(CRCore::crCopyOp::SHALLOW_COPY)) : new Options;
            local_opt->setDatabasePath(CRIOManager::getFilePath(fileName));

            ReadResult rr;

            // read file
            {
                std::ifstream istream;
                istream.imbue(std::locale::classic());
                istream.open(fileName.c_str(), std::ios::in | std::ios::binary);

                if (istream)
                {
                    rr = readNode(istream,local_opt.get());
                }
            }

            static int nestedExternalsLevel = 0;
            if (rr.success())
            {
                // add to local cache.
                flt::crRegistry::instance()->addToLocalCache(fileName,rr.getNode());
        
                bool keepExternalReferences = false;
                if (options)
                    keepExternalReferences = (options->getOptionString().find("keepExternalReferences")!=std::string::npos);


                if ( !keepExternalReferences )
                {
                    CRCore::notify(CRCore::DEBUG_INFO) << "keepExternalReferences not found, so externals will be re-readed"<<std::endl;
                    // read externals.
                    if (rr.getNode())
                    {
                        nestedExternalsLevel++;
                        crReadExternalsVisitor visitor(local_opt.get());
                        rr.getNode()->accept(visitor);
                        nestedExternalsLevel--;
                    }
                }
                else
                {
                    CRCore::notify(CRCore::DEBUG_INFO) << "keepExternalReferences found, so externals will be left as ProxyNodes"<<std::endl;    
                }
            }

            // clear local cache.
            if (nestedExternalsLevel==0)
                flt::crRegistry::instance()->clearLocalCache();

            return rr;
        }
        
        virtual ReadResult readObject(std::istream& fin, const Options* options) const
        {
            return readNode(fin, options);
        }
        
        virtual ReadResult readNode(std::istream& fin, const Options* options) const
        {
            Document document;
            document.setOptions(options);

            // option string and parent pools
            if (options)
            {
                const char readerMsg[] = "flt reader option: ";
                
                document.setKeepExternalReferences((options->getOptionString().find("keepExternalReferences")!=std::string::npos));
                CRCore::notify(CRCore::DEBUG_INFO) << readerMsg << "keepExternalReferences=" << document.getKeepExternalReferences() << std::endl;

                document.setPreserveFace((options->getOptionString().find("preserveFace")!=std::string::npos));
                CRCore::notify(CRCore::DEBUG_INFO) << readerMsg << "preserveFace=" << document.getPreserveFace() << std::endl;

                document.setPreserveObject((options->getOptionString().find("preserveObject")!=std::string::npos));
                CRCore::notify(CRCore::DEBUG_INFO) << readerMsg << "preserveObject=" << document.getPreserveObject() << std::endl;

                document.setDefaultDOFAnimationState((options->getOptionString().find("dofAnimation")!=std::string::npos));
                CRCore::notify(CRCore::DEBUG_INFO) << readerMsg << "dofAnimation=" << document.getDefaultDOFAnimationState() << std::endl;

                document.setUseTextureAlphaForTransparancyBinning(options->getOptionString().find("noTextureAlphaForTransparancyBinning")==std::string::npos);
                CRCore::notify(CRCore::DEBUG_INFO) << readerMsg << "noTextureAlphaForTransparancyBinning=" << document.getUseTextureAlphaForTransparancyBinning() << std::endl;

                document.setDoUnitsConversion((options->getOptionString().find("noUnitsConversion")==std::string::npos)); // default to true, unless noUnitsConversion is specified.
                CRCore::notify(CRCore::DEBUG_INFO) << readerMsg << "noUnitsConversion=" << document.getDoUnitsConversion() << std::endl;

                if (document.getDoUnitsConversion())
                {
                    if (options->getOptionString().find("convertToFeet")!=std::string::npos)
                        document.setDesiredUnits(FEET);
                    else if (options->getOptionString().find("convertToInches")!=std::string::npos)
                        document.setDesiredUnits(INCHES);
                    else if (options->getOptionString().find("convertToMeters")!=std::string::npos)
                        document.setDesiredUnits(METERS);
                    else if (options->getOptionString().find("convertToKilometers")!=std::string::npos)
                        document.setDesiredUnits(KILOMETERS);
                    else if (options->getOptionString().find("convertToNauticalMiles")!=std::string::npos)
                        document.setDesiredUnits(NAUTICAL_MILES);
                }

      //          const ParentPools* pools = dynamic_cast<const ParentPools*>( options->getUserData() );
      //          if (pools)
      //          {
      //              // This file is an external reference. The individual pools will
      //              // be non-NULL if the parent is overriding the ext ref model's pools.
      //              if (pools->getColorPool())
						//document.setColorPool( const_cast<flt::ColorPool *>(pools->getColorPool()), true );
      //              if (pools->getTexturePool())
      //                  document.setTexturePool( const_cast<flt::TexturePool *>(pools->getTexturePool()), true );
      //              if (pools->getMaterialPool())
      //                  document.setMaterialPool( const_cast<flt::MaterialPool *>(pools->getMaterialPool()), true );
      //              if (pools->getLPAppearancePool())
      //                  document.setLightPointAppearancePool( const_cast<flt::LightPointAppearancePool *>(pools->getLPAppearancePool()), true );
      //              if (pools->getShaderPool())
      //                  document.setShaderPool( const_cast<flt::ShaderPool *>(pools->getShaderPool()), true );
      //          }
            }

            {
                // read records
                flt::RecordInputStream recordStream(fin.rdbuf());
                while (recordStream.good() && !document.done())
                {
                    recordStream.readRecord(document);
                }
            }

            if (!document.getHeaderNode())
                return ReadResult::ERROR_IN_READING_FILE;

            //if (!document.getPreserveFace())
            //{
            //    CRUtil::crOptimizer optimizer;
            //    optimizer.optimize(document.getHeaderNode(),  CRUtil::crOptimizer::SHARE_DUPLICATE_STATE | CRUtil::crOptimizer::MERGE_GEOMETRY | CRUtil::crOptimizer::MERGE_GEODES | CRUtil::crOptimizer::TESSELATE_GEOMETRY );
            //}

            return document.getHeaderNode();
        }

        virtual WriteResult writeObject(const crBase& object,const std::string& fileName, const CRIOManager::crReaderWriter::Options* options) const
        {
            const crNode* node = dynamic_cast<const crNode*>(&object);
            if (node) return writeNode( *node, fileName, options );
            return WriteResult::FILE_NOT_HANDLED;
        }

        virtual WriteResult writeNode(const crNode& node,const std::string& fileName, const CRIOManager::crReaderWriter::Options* options) const
        {
            std::string ext = getFileExtension(fileName);
            if (!acceptsExtension(ext)) return WriteResult::FILE_NOT_HANDLED;
			std::string _fileName = CRCore::crArgumentParser::getFileNameEliminateExt(fileName) + ".flt";
            // code for setting up the database path so that internally referenced file are searched for on relative paths. 
            CRCore::ref_ptr<Options> local_opt = options ? static_cast<Options*>(options->clone(CRCore::crCopyOp::SHALLOW_COPY)) : new Options;
            if(local_opt->getDatabasePathList().empty())
			{
				std::string filepath = CRIOManager::getFilePath(_fileName);
                local_opt->setDatabasePath(filepath);
			}
            std::ofstream fout(_fileName.c_str(), std::ios::out | std::ios::binary);
            WriteResult result = writeNode(node, fout, local_opt.get());
            fout.close();
            return result;
        }
        
        virtual WriteResult writeObject(const crBase& object,std::ostream& fout, const CRIOManager::crReaderWriter::Options* options) const
        {
            const crNode* node = dynamic_cast<const crNode*>(&object);
            if (node) return writeNode( *node, fout, options );
            return WriteResult::FILE_NOT_HANDLED;
        }

        virtual WriteResult writeNode(const crNode& /*node*/,std::ostream& /*fout*/, const CRIOManager::crReaderWriter::Options* /*options*/) const
        {
            return WriteResult::FILE_NOT_HANDLED;

        }

    protected:

        mutable CRIOManager::crReentrantMutex m_serializerMutex;
};

// now register with crRegistry to instantiate the above
// reader/writer.
RegisterReaderWriterProxy<crFLTReaderWriter> g_FLTReaderWriterProxy;















