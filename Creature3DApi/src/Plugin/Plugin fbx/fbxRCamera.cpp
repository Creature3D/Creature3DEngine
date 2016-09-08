#include <CRCore/CameraView>
#include <CRCore/Notify>

#include <CRIOManager/ReadFile>

#if defined(_MSC_VER)
#pragma warning( disable : 4505 )
#pragma warning( default : 4996 )
#endif
#include <fbxsdk.h>

#include "fbxReader.h"

CRIOManager::crReaderWriter::ReadResult OsgFbxReader::readFbxCamera(FbxNode* pNode)
{
    const FbxCamera* fbxCamera = FbxCast<FbxCamera>(pNode->GetNodeAttribute());

    if (!fbxCamera)
    {
        return CRIOManager::crReaderWriter::ReadResult::ERROR_IN_READING_FILE;
    }

    CRCore::CameraView* osgCameraView = new CRCore::CameraView;

    if (fbxCamera->FieldOfView.IsValid())
    {
        osgCameraView->setFieldOfView(fbxCamera->FieldOfView.Get());
    }

    if (fbxCamera->FocalLength.IsValid())
    {
        osgCameraView->setFocalLength(fbxCamera->FocalLength.Get());
    }

    if (fbxCamera->ApertureMode.IsValid())
    {
        switch (fbxCamera->ApertureMode.Get())
        {
        case FbxCamera::eHorizontal:
            osgCameraView->setFieldOfViewMode(CRCore::CameraView::HORIZONTAL);
            break;
        case FbxCamera::eVertical:
            osgCameraView->setFieldOfViewMode(CRCore::CameraView::VERTICAL);
            break;
        case FbxCamera::eHorizAndVert:
        case FbxCamera::eFocalLength:
        default:
            OSG_WARN << "readFbxCamera: Unsupported Camera aperture mode." << std::endl;
            break;
        }
    }

    return CRIOManager::crReaderWriter::ReadResult(osgCameraView);
}
