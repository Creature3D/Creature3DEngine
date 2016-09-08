#include <sstream>
#include <memory>
#ifndef WIN32
#include <strings.h>//for strncasecmp
#endif

#include <CRCore/Notify>
#include <CRCore/crMatrixTransform>
#include <CRCore/crMaterial>
#include <CRCore/PositionAttitudeTransform>
#include <CRCore/crTexture2D>
#include <CRCore/Version>
#include <CRIOManager/ConvertUTF>
#include <CRIOManager/FileNameUtils>
#include <CRIOManager/FileUtils>
#include <CRIOManager/ReadFile>
#include <CRIOManager/Registry>
#include <osgAnimation/AnimationManagerBase>
#include <osgAnimation/Bone>
#include <osgAnimation/RigGeometry>
#include <osgAnimation/Skeleton>
#include <osgAnimation/VertexInfluence>

#if defined(_MSC_VER)
    #pragma warning( disable : 4505 )
    #pragma warning( default : 4996 )
#endif
#include <fbxsdk.h>

#include "ReaderWriterFBX.h"
#include "fbxReader.h"
#include "WriterNodeVisitor.h"

/// Returns true if the given node is a basic root group with no special information.
/// Used in conjunction with UseFbxRoot option.
/// Identity transforms are considered as basic root nodes.
bool isBasicRootNode(const CRCore::crNode& node)
{
    const CRCore::crGroup* osgGroup = node.asGroup();
    if (!osgGroup)
    {
        // Geodes & such are not basic root nodes
        return false;
    }

    // Test if we've got an empty transform (= a group!)
    const CRCore::Transform* transform = osgGroup->asTransform();
    if (transform)
    {
        if (const CRCore::crMatrixTransform* matrixTransform = transform->asMatrixTransform())
        {
            if (!matrixTransform->getMatrix().isIdentity())
            {
                // Non-identity matrix transform
                return false;
            }
        }
        else if (const CRCore::PositionAttitudeTransform* pat = transform->asPositionAttitudeTransform())
        {
            if (pat->getPosition() != CRCore::Vec3d() ||
                pat->getAttitude() != CRCore::Quat() ||
                pat->getScale() != CRCore::Vec3d(1.0f, 1.0f, 1.0f) ||
                pat->getPivotPoint() != CRCore::Vec3d())
            {
                // Non-identity position attribute transform
                return false;
            }
        }
        else
        {
            // Other transform (not identity or not predefined type)
            return false;
        }
    }

    // Test the presence of a non-empty stateset
    if (node.getStateSet())
    {
        CRCore::ref_ptr<CRCore::crStateSet> emptyStateSet = new CRCore::crStateSet;
        if (node.getStateSet()->compare(*emptyStateSet, true) != 0)
        {
            return false;
        }
    }

    return true;
}


class CleanUpFbx
{
    FbxManager* m_pSdkManager;
public:
    explicit CleanUpFbx(FbxManager* pSdkManager) : m_pSdkManager(pSdkManager)
    {}

    ~CleanUpFbx()
    {
        m_pSdkManager->Destroy();
    }
};

//Some files don't correctly mark their skeleton nodes, so this function infers
//them from the nodes that skin deformers linked to.
void findLinkedFbxSkeletonNodes(FbxNode* pNode, std::set<const FbxNode*>& fbxSkeletons)
{
    if (const FbxGeometry* pMesh = FbxCast<FbxGeometry>(pNode->GetNodeAttribute()))
    {
        for (int i = 0; i < pMesh->GetDeformerCount(FbxDeformer::eSkin); ++i)
        {
            const FbxSkin* pSkin = (const FbxSkin*)pMesh->GetDeformer(i, FbxDeformer::eSkin);

            for (int j = 0; j < pSkin->GetClusterCount(); ++j)
            {
                const FbxNode* pSkeleton = pSkin->GetCluster(j)->GetLink();
                fbxSkeletons.insert(pSkeleton);
            }
        }
    }

    for (int i = 0; i < pNode->GetChildCount(); ++i)
    {
        findLinkedFbxSkeletonNodes(pNode->GetChild(i), fbxSkeletons);
    }
}

void resolveBindMatrices(
    CRCore::crNode& root,
    const BindMatrixMap& boneBindMatrices,
    const std::map<FbxNode*, CRCore::crNode*>& nodeMap)
{
    std::set<std::string> nodeNames;
    for (std::map<FbxNode*, CRCore::crNode*>::const_iterator it = nodeMap.begin(); it != nodeMap.end(); ++it)
    {
        nodeNames.insert(it->second->getName());
    }

    for (BindMatrixMap::const_iterator it = boneBindMatrices.begin();
        it != boneBindMatrices.end();)
    {
        FbxNode* const fbxBone = it->first.first;
        std::map<FbxNode*, CRCore::crNode*>::const_iterator nodeIt = nodeMap.find(fbxBone);
        if (nodeIt != nodeMap.end())
        {
            const CRCore::Matrix bindMatrix = it->second;
            osgAnimation::Bone& osgBone = dynamic_cast<osgAnimation::Bone&>(*nodeIt->second);
            osgBone.setInvBindMatrixInSkeletonSpace(bindMatrix);

            ++it;
            for (; it != boneBindMatrices.end() && it->first.first == fbxBone; ++it)
            {
                if (it->second != bindMatrix)
                {
                    std::string name;
                    for (int i = 0;; ++i)
                    {
                        std::stringstream ss;
                        ss << osgBone.getName() << '_' << i;
                        name = ss.str();
                        if (nodeNames.insert(name).second)
                        {
                            break;
                        }
                    }
                    osgAnimation::Bone* newBone = new osgAnimation::Bone(name);
                    newBone->setDefaultUpdateCallback();
                    newBone->setInvBindMatrixInSkeletonSpace(it->second);
                    osgBone.addChild(newBone);

                    osgAnimation::RigGeometry* pRigGeometry = it->first.second;

                    osgAnimation::VertexInfluenceMap* vertexInfluences = pRigGeometry->getInfluenceMap();

                    osgAnimation::VertexInfluenceMap::iterator vimIt = vertexInfluences->find(osgBone.getName());
                    if (vimIt != vertexInfluences->end())
                    {
                        osgAnimation::VertexInfluence vi;
                        vi.swap(vimIt->second);
                        vertexInfluences->erase(vimIt);
                        osgAnimation::VertexInfluence& vi2 = (*vertexInfluences)[name];
                        vi.swap(vi2);
                        vi2.setName(name);
                    }
                    else
                    {
                        OSG_WARN << "No vertex influences found for \"" << osgBone.getName() << "\"" << std::endl;
                    }
                }
            }
        }
        else
        {
            OSG_WARN << "No bone found for \"" << fbxBone->GetName() << "\"" << std::endl;
            ++it;
        }
    }
}

CRIOManager::crReaderWriter::ReadResult
ReaderWriterFBX::readNode(const std::string& filenameInit,
                          const Options* options) const
{
    try
    {
        std::string ext(CRIOManager::getLowerCaseFileExtension(filenameInit));
        if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

        std::string filename(CRIOManager::findDataFile(filenameInit, options));
        if (filename.empty()) return ReadResult::FILE_NOT_FOUND;

        FbxManager* pSdkManager = FbxManager::Create();

        if (!pSdkManager)
        {
            return ReadResult::ERROR_IN_READING_FILE;
        }

        CleanUpFbx cleanUpFbx(pSdkManager);

        pSdkManager->SetIOSettings(FbxIOSettings::Create(pSdkManager, IOSROOT));

        FbxScene* pScene = FbxScene::Create(pSdkManager, "");

        // The FBX SDK interprets the filename as UTF-8
#ifdef OSG_USE_UTF8_FILENAME
        const std::string& utf8filename(filename);
#else
        std::string utf8filename(CRIOManager::convertStringFromCurrentCodePageToUTF8(filename));
#endif

        FbxImporter* lImporter = FbxImporter::Create(pSdkManager, "");

        if (!lImporter->Initialize(utf8filename.c_str(), -1, pSdkManager->GetIOSettings()))
        {
#if FBXSDK_VERSION_MAJOR < 2014
            return std::string(lImporter->GetLastErrorString());
#else
            return std::string(lImporter->GetStatus().GetErrorString());
#endif
        }

        if (!lImporter->IsFBX())
        {
            return ReadResult::ERROR_IN_READING_FILE;
        }

        for (int i = 0; FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i); i++)
        {
            lTakeInfo->mSelect = true;
        }

        if (!lImporter->Import(pScene))
        {
#if FBXSDK_VERSION_MAJOR < 2014
            return std::string(lImporter->GetLastErrorString());
#else 
            return std::string(lImporter->GetStatus().GetErrorString());
#endif
        }

        //FbxAxisSystem::OpenGL.ConvertScene(pScene);        // Doesn't work as expected. Still need to transform vertices.

        if (FbxNode* pNode = pScene->GetRootNode())
        {
            bool useFbxRoot = false;
            bool lightmapTextures = false;
            bool tessellatePolygons = false;
            if (options)
            {
                std::istringstream iss(options->getOptionString());
                std::string opt;
                while (iss >> opt)
                {
                    if (opt == "UseFbxRoot")
                    {
                        useFbxRoot = true;
                    }
                    if (opt == "LightmapTextures")
                    {
                        lightmapTextures = true;
                    }
                    if (opt == "TessellatePolygons")
                    {
                        tessellatePolygons = true;
                    }
                }
            }

            bool bIsBone = false;
            int nLightCount = 0;
            CRCore::ref_ptr<Options> localOptions = NULL;
            if (options)
                localOptions = options->cloneOptions();
            else
                localOptions = new CRIOManager::Options();
            localOptions->setObjectCacheHint(CRIOManager::crReaderWriter::Options::CACHE_IMAGES);

            std::string filePath = CRIOManager::getFilePath(filename);
            FbxMaterialToOsgStateSet fbxMaterialToOsgStateSet(filePath, localOptions.get(), lightmapTextures);

            std::set<const FbxNode*> fbxSkeletons;
            findLinkedFbxSkeletonNodes(pNode, fbxSkeletons);

            OsgFbxReader::AuthoringTool authoringTool = OsgFbxReader::UNKNOWN;
            if (FbxDocumentInfo* pDocInfo = pScene->GetDocumentInfo())
            {
                struct ToolName
                {
                    const char* name;
                    OsgFbxReader::AuthoringTool tool;
                };

                ToolName authoringTools[] = {
                    {"OpenSceneGraph", OsgFbxReader::OPENSCENEGRAPH},
                    {"3ds Max", OsgFbxReader::AUTODESK_3DSTUDIO_MAX}
                };

                FbxString appName = pDocInfo->LastSaved_ApplicationName.Get();

                for (unsigned int i = 0; i < sizeof(authoringTools) / sizeof(authoringTools[0]); ++i)
                {
                    if (0 ==
#ifdef WIN32
                        _strnicmp
#else
                        strncasecmp
#endif
                        (appName, authoringTools[i].name, strlen(authoringTools[i].name)))
                    {
                        authoringTool = authoringTools[i].tool;
                        break;
                    }
                }
            }


            OsgFbxReader reader(*pSdkManager,
                *pScene,
                fbxMaterialToOsgStateSet,
                fbxSkeletons,
                *localOptions,
                authoringTool,
                lightmapTextures,
                tessellatePolygons);

            ReadResult res = reader.readFbxNode(pNode, bIsBone, nLightCount);

            if (res.success())
            {
                fbxMaterialToOsgStateSet.checkInvertTransparency();

                resolveBindMatrices(*res.getNode(), reader.boneBindMatrices, reader.nodeMap);

                CRCore::crNode* osgNode = res.getNode();
                osgNode->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL,CRCore::StateAttribute::ON);
                osgNode->getOrCreateStateSet()->setMode(GL_NORMALIZE,CRCore::StateAttribute::ON);

                if (reader.pAnimationManager.valid())
                {
                    if (osgNode->getUpdateCallback())
                    {
                        CRCore::crGroup* osgGroup = new CRCore::crGroup;
                        osgGroup->addChild(osgNode);
                        osgNode = osgGroup;
                    }

                    //because the animations may be altered after registering
                    reader.pAnimationManager->buildTargetReference();
                    osgNode->setUpdateCallback(reader.pAnimationManager.get());
                }

                FbxAxisSystem fbxAxis = pScene->GetGlobalSettings().GetAxisSystem();

                if (fbxAxis != FbxAxisSystem::OpenGL)
                {
                    int upSign;
                    FbxAxisSystem::EUpVector eUp = fbxAxis.GetUpVector(upSign);
                    bool bLeftHanded = fbxAxis.GetCoorSystem() == FbxAxisSystem::eLeftHanded;
                    float fSign = upSign < 0 ? -1.0f : 1.0f;
                    float zScale = bLeftHanded ? -1.0f : 1.0f;

                    CRCore::Matrix mat;
                    switch (eUp)
                    {
                    case FbxAxisSystem::eXAxis:
                        mat.set(0,fSign,0,0,-fSign,0,0,0,0,0,zScale,0,0,0,0,1);
                        break;
                    case FbxAxisSystem::eYAxis:
                        mat.set(1,0,0,0,0,fSign,0,0,0,0,fSign*zScale,0,0,0,0,1);
                        break;
                    case FbxAxisSystem::eZAxis:
                        mat.set(1,0,0,0,0,0,-fSign*zScale,0,0,fSign,0,0,0,0,0,1);
                        break;
                    }

                    CRCore::Transform* pTransformTemp = osgNode->asTransform();
                    CRCore::crMatrixTransform* pMatrixTransform = pTransformTemp ?
                        pTransformTemp->asMatrixTransform() : NULL;
                    if (pMatrixTransform)
                    {
                        pMatrixTransform->setMatrix(pMatrixTransform->getMatrix() * mat);
                    }
                    else
                    {
                        pMatrixTransform = new CRCore::crMatrixTransform(mat);
                        if (useFbxRoot && isBasicRootNode(*osgNode))
                        {
                            // If root node is a simple group, put all FBX elements under the OSG root
                            CRCore::crGroup* osgGroup = osgNode->asGroup();
                            for(unsigned int i = 0; i < osgGroup->getNumChildren(); ++i)
                            {
                                pMatrixTransform->addChild(osgGroup->getChild(i));
                            }
                            pMatrixTransform->setName(osgGroup->getName());
                        }
                        else
                        {
                            pMatrixTransform->addChild(osgNode);
                        }
                    }
                    osgNode = pMatrixTransform;
                }

                osgNode->setName(filenameInit);
                return osgNode;
            }
        }
    }
    catch (...)
    {
        OSG_WARN << "Exception thrown while importing \"" << filenameInit << '\"' << std::endl;
    }

    return ReadResult::ERROR_IN_READING_FILE;
}

CRIOManager::crReaderWriter::WriteResult ReaderWriterFBX::writeNode(
    const CRCore::crNode& node,
    const std::string& filename,
    const Options* options) const
{
    try
    {
        std::string ext = CRIOManager::getLowerCaseFileExtension(filename);
        if (!acceptsExtension(ext)) return WriteResult::FILE_NOT_HANDLED;

        CRCore::ref_ptr<Options> localOptions = options ?
            static_cast<Options*>(options->clone(CRCore::crCopyOp::SHALLOW_COPY)) : new Options;
        localOptions->getDatabasePathList().push_front(CRIOManager::getFilePath(filename));

        FbxManager* pSdkManager = FbxManager::Create();

        if (!pSdkManager)
        {
            return WriteResult::ERROR_IN_WRITING_FILE;
        }

        CleanUpFbx cleanUpFbx(pSdkManager);

        pSdkManager->SetIOSettings(FbxIOSettings::Create(pSdkManager, IOSROOT));

        bool useFbxRoot = false;
        if (options)
        {
            std::istringstream iss(options->getOptionString());
            std::string opt;
            while (iss >> opt)
            {
                if (opt == "Embedded")
                {
                    pSdkManager->GetIOSettings()->SetBoolProp(EXP_FBX_EMBEDDED, true);
                }
                else if (opt == "UseFbxRoot")
                {
                    useFbxRoot = true;
                }
            }
        }

        FbxScene* pScene = FbxScene::Create(pSdkManager, "");
        pluginfbx::WriterNodeVisitor writerNodeVisitor(pScene, pSdkManager, filename,
            options, CRIOManager::getFilePath(node.getName().empty() ? filename : node.getName()));
        if (useFbxRoot && isBasicRootNode(node))
        {
            // If root node is a simple group, put all elements under the FBX root
            const CRCore::crGroup * osgGroup = node.asGroup();
            for (unsigned int child = 0; child < osgGroup->getNumChildren(); ++child)
            {
                const_cast<CRCore::crNode *>(osgGroup->getChild(child))->accept(writerNodeVisitor);
            }
        }
        else {
            // Normal scene
            const_cast<CRCore::crNode&>(node).accept(writerNodeVisitor);
        }

        FbxDocumentInfo* pDocInfo = pScene->GetDocumentInfo();
        bool needNewDocInfo = pDocInfo != NULL;
        if (needNewDocInfo)
        {
            pDocInfo = FbxDocumentInfo::Create(pSdkManager, "");
        }
        pDocInfo->LastSaved_ApplicationName.Set(FbxString("OpenSceneGraph"));
        pDocInfo->LastSaved_ApplicationVersion.Set(FbxString(osgGetVersion()));
        if (needNewDocInfo)
        {
            pScene->SetDocumentInfo(pDocInfo);
        }

        FbxExporter* lExporter = FbxExporter::Create(pSdkManager, "");
        pScene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::eOpenGL);

        // Ensure the directory exists or else the FBX SDK will fail
        if (!CRIOManager::makeDirectoryForFile(filename)) {
            OSG_NOTICE << "Can't create directory for file '" << filename << "'. FBX SDK may fail creating the file." << std::endl;
        }

        // The FBX SDK interprets the filename as UTF-8
#ifdef OSG_USE_UTF8_FILENAME
        const std::string& utf8filename(filename);
#else
        std::string utf8filename(CRIOManager::convertStringFromCurrentCodePageToUTF8(filename));
#endif

        if (!lExporter->Initialize(utf8filename.c_str()))
        {
#if FBXSDK_VERSION_MAJOR < 2014
            return std::string(lExporter->GetLastErrorString());
#else
            return std::string(lExporter->GetStatus().GetErrorString());
#endif
        }
        if (!lExporter->Export(pScene))
        {
#if FBXSDK_VERSION_MAJOR < 2014
            return std::string(lExporter->GetLastErrorString());
#else
            return std::string(lExporter->GetStatus().GetErrorString());
#endif
        }

        return WriteResult::FILE_SAVED;
    }
    catch (const std::string& s)
    {
        return s;
    }
    catch (const char* s)
    {
        return std::string(s);
    }
    catch (...)
    {
    }

    return WriteResult::ERROR_IN_WRITING_FILE;
}

///////////////////////////////////////////////////////////////////////////
// Add ourself to the Registry to instantiate the reader/writer.

REGISTER_OSGPLUGIN(fbx, ReaderWriterFBX)
