#ifndef FBXRANIMATION_H
#define FBXRANIMATION_H

#include <fbxsdk.h>
#include <CRIOManager/crReaderWriter.h>
#include <osgAnimation/BasicAnimationManager>
#include "fbxMaterialToStateSet.h"

namespace osgAnimation
{
    class AnimationManagerBase;
    class RigGeometry;
    class Skeleton;
}

typedef std::map<std::pair<FbxNode*, osgAnimation::RigGeometry*>, CRCore::Matrix> BindMatrixMap;

class OsgFbxReader
{
public:
    FbxManager& pSdkManager;
    FbxScene& fbxScene;
    CRCore::ref_ptr<osgAnimation::AnimationManagerBase> pAnimationManager;
    FbxMaterialToOsgStateSet& fbxMaterialToOsgStateSet;
    std::map<FbxNode*, CRCore::crNode*> nodeMap;
    BindMatrixMap boneBindMatrices;
    const std::set<const FbxNode*>& fbxSkeletons;
    std::map<FbxNode*, osgAnimation::Skeleton*> skeletonMap;
    const CRIOManager::Options& options;
    bool lightmapTextures, tessellatePolygons;

    enum AuthoringTool
    {
        UNKNOWN,
        OPENSCENEGRAPH,
        AUTODESK_3DSTUDIO_MAX
    } authoringTool;

    OsgFbxReader(
        FbxManager& pSdkManager1,
        FbxScene& fbxScene1,
        FbxMaterialToOsgStateSet& fbxMaterialToOsgStateSet1,
        const std::set<const FbxNode*>& fbxSkeletons1,
        const CRIOManager::Options& options1,
        AuthoringTool authoringTool1,
        bool lightmapTextures1,
        bool tessellatePolygons1)
        : pSdkManager(pSdkManager1),
        fbxScene(fbxScene1),
        fbxMaterialToOsgStateSet(fbxMaterialToOsgStateSet1),
        fbxSkeletons(fbxSkeletons1),
        options(options1),
        lightmapTextures(lightmapTextures1),
        tessellatePolygons(tessellatePolygons1),
        authoringTool(authoringTool1)
    {}

    CRIOManager::crReaderWriter::ReadResult readFbxNode(
        FbxNode*, bool& bIsBone, int& nLightCount);

    std::string readFbxAnimation(
        FbxNode*, const char* targetName);

    CRIOManager::crReaderWriter::ReadResult readFbxCamera(
        FbxNode* pNode);

    CRIOManager::crReaderWriter::ReadResult readFbxLight(
        FbxNode* pNode, int& nLightCount);

    CRIOManager::crReaderWriter::ReadResult readMesh(
        FbxNode* pNode, FbxMesh* fbxMesh,
        std::vector<StateSetContent>& stateSetList,
        const char* szName);

    CRIOManager::crReaderWriter::ReadResult readFbxMesh(
        FbxNode* pNode,
        std::vector<StateSetContent>&);
};

osgAnimation::Skeleton* getSkeleton(FbxNode*,
    const std::set<const FbxNode*>& fbxSkeletons,
    std::map<FbxNode*, osgAnimation::Skeleton*>&);

#endif
