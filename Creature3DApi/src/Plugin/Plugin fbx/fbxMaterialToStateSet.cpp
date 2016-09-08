#include "fbxMaterialToStateSet.h"
#include <sstream>
#include <CRIOManager/crReadFile.h>
#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crFileNameUtils.h>


static CRCore::crTexture::WrapMode convertWrap(FbxFileTexture::EWrapMode wrap)
{
    return wrap == FbxFileTexture::eRepeat ?
        CRCore::crTexture2D::REPEAT : CRCore::crTexture2D::CLAMP_TO_EDGE;
}

StateSetContent
FbxMaterialToOsgStateSet::convert(const FbxSurfaceMaterial* pFbxMat)
{
    FbxMaterialMap::const_iterator it = _fbxMaterialMap.find(pFbxMat);
    if (it != _fbxMaterialMap.end())
        return it->second;

    CRCore::ref_ptr<CRCore::crMaterial> pOsgMat = new CRCore::crMaterial;
    pOsgMat->setName(pFbxMat->GetName());

    StateSetContent result;

    result.material = pOsgMat;

    FbxString shadingModel = pFbxMat->ShadingModel.Get();

    const FbxSurfaceLambert* pFbxLambert = FbxCast<FbxSurfaceLambert>(pFbxMat);

    // diffuse map...
    const FbxProperty lProperty = pFbxMat->FindProperty(FbxSurfaceMaterial::sDiffuse);
    if (lProperty.IsValid())
    {
        int lNbTex = lProperty.GetSrcObjectCount<FbxFileTexture>();
        for (int lTextureIndex = 0; lTextureIndex < lNbTex; lTextureIndex++)
        {
            FbxFileTexture* lTexture = FbxCast<FbxFileTexture>(lProperty.GetSrcObject<FbxFileTexture>(lTextureIndex));
            if (lTexture)
            {
                result.diffuseTexture = fbxTextureToOsgTexture(lTexture);
                result.diffuseChannel = lTexture->UVSet.Get();
                result.diffuseScaleU = lTexture->GetScaleU();
                result.diffuseScaleV = lTexture->GetScaleV();
            }

            //For now only allow 1 texture
            break;
        }
    }

    // opacity map...
    const FbxProperty lOpacityProperty = pFbxMat->FindProperty(FbxSurfaceMaterial::sTransparentColor);
    if (lOpacityProperty.IsValid())
    {
        int lNbTex = lOpacityProperty.GetSrcObjectCount<FbxFileTexture>();
        for (int lTextureIndex = 0; lTextureIndex < lNbTex; lTextureIndex++)
        {
            FbxFileTexture* lTexture = FbxCast<FbxFileTexture>(lOpacityProperty.GetSrcObject<FbxFileTexture>(lTextureIndex));
            if (lTexture)
            {
                // TODO: if texture image does NOT have an alpha channel, should it be added?

                result.opacityTexture = fbxTextureToOsgTexture(lTexture);
                result.opacityChannel = lTexture->UVSet.Get();
                result.opacityScaleU = lTexture->GetScaleU();
                result.opacityScaleV = lTexture->GetScaleV();
            }

            //For now only allow 1 texture
            break;
        }
    }

    // reflection map...
    const FbxProperty lReflectionProperty = pFbxMat->FindProperty(FbxSurfaceMaterial::sReflection);
    if (lReflectionProperty.IsValid())
    {
        int lNbTex = lReflectionProperty.GetSrcObjectCount<FbxFileTexture>();
        for (int lTextureIndex = 0; lTextureIndex < lNbTex; lTextureIndex++)
        {
            FbxFileTexture* lTexture = FbxCast<FbxFileTexture>(lReflectionProperty.GetSrcObject<FbxFileTexture>(lTextureIndex));
            if (lTexture)
            {
                // support only spherical reflection maps...
                if (FbxFileTexture::eUMT_ENVIRONMENT == lTexture->CurrentMappingType.Get())
                {
                    result.reflectionTexture = fbxTextureToOsgTexture(lTexture);
                    result.reflectionChannel = lTexture->UVSet.Get();
                }
            }

            //For now only allow 1 texture
            break;
        }
    }

    // emissive map...
    const FbxProperty lEmissiveProperty = pFbxMat->FindProperty(FbxSurfaceMaterial::sEmissive);
    if (lEmissiveProperty.IsValid())
    {
        int lNbTex = lEmissiveProperty.GetSrcObjectCount<FbxFileTexture>();
        for (int lTextureIndex = 0; lTextureIndex < lNbTex; lTextureIndex++)
        {
            FbxFileTexture* lTexture = FbxCast<FbxFileTexture>(lEmissiveProperty.GetSrcObject<FbxFileTexture>(lTextureIndex));
            if (lTexture)
            {
                result.emissiveTexture = fbxTextureToOsgTexture(lTexture);
                result.emissiveChannel = lTexture->UVSet.Get();
                result.emissiveScaleU = lTexture->GetScaleU();
                result.emissiveScaleV = lTexture->GetScaleV();
            }

            //For now only allow 1 texture
            break;
        }
    }

    if (pFbxLambert)
    {
        FbxDouble3 color = pFbxLambert->Diffuse.Get();
        double factor = pFbxLambert->DiffuseFactor.Get();
        pOsgMat->setDiffuse(CRCore::crMaterial::FRONT_AND_BACK, CRCore::crVector4(
            static_cast<float>(color[0] * factor),
            static_cast<float>(color[1] * factor),
            static_cast<float>(color[2] * factor),
            static_cast<float>(1.0 - pFbxLambert->TransparencyFactor.Get())));

        color = pFbxLambert->Ambient.Get();
        factor = pFbxLambert->AmbientFactor.Get();
        pOsgMat->setAmbient(CRCore::crMaterial::FRONT_AND_BACK, CRCore::crVector4(
            static_cast<float>(color[0] * factor),
            static_cast<float>(color[1] * factor),
            static_cast<float>(color[2] * factor),
            1.0f));

        color = pFbxLambert->Emissive.Get();
        factor = pFbxLambert->EmissiveFactor.Get();
        pOsgMat->setEmission(CRCore::crMaterial::FRONT_AND_BACK, CRCore::crVector4(
            static_cast<float>(color[0] * factor),
            static_cast<float>(color[1] * factor),
            static_cast<float>(color[2] * factor),
            1.0f));

        // get maps factors...
        result.diffuseFactor = pFbxLambert->DiffuseFactor.Get();

        if (const FbxSurfacePhong* pFbxPhong = FbxCast<FbxSurfacePhong>(pFbxLambert))
        {
            color = pFbxPhong->Specular.Get();
            factor = pFbxPhong->SpecularFactor.Get();
            pOsgMat->setSpecular(CRCore::crMaterial::FRONT_AND_BACK, CRCore::crVector4(
                static_cast<float>(color[0] * factor),
                static_cast<float>(color[1] * factor),
                static_cast<float>(color[2] * factor),
                1.0f));

            pOsgMat->setShininess(CRCore::crMaterial::FRONT_AND_BACK,
                static_cast<float>(pFbxPhong->Shininess.Get()));

            // get maps factors...
            result.reflectionFactor = pFbxPhong->ReflectionFactor.Get();
            // get more factors here...
        }
    }

    if (_lightmapTextures)
    {
        // if using an emission map then adjust material properties accordingly...
        if (result.emissiveTexture)
        {
            CRCore::crVector4 diffuse = pOsgMat->getDiffuse(CRCore::crMaterial::FRONT_AND_BACK);
            pOsgMat->setEmission(CRCore::crMaterial::FRONT_AND_BACK, diffuse);
            pOsgMat->setDiffuse(CRCore::crMaterial::FRONT_AND_BACK, CRCore::crVector4(0,0,0,diffuse.a()));
            pOsgMat->setAmbient(CRCore::crMaterial::FRONT_AND_BACK, CRCore::crVector4(0,0,0,diffuse.a()));
        }
    }

    _fbxMaterialMap.insert(FbxMaterialMap::value_type(pFbxMat, result));
    return result;
}

CRCore::ref_ptr<CRCore::crTexture2D>
FbxMaterialToOsgStateSet::fbxTextureToOsgTexture(const FbxFileTexture* fbx)
{
    ImageMap::iterator it = _imageMap.find(fbx->GetFileName());
    if (it != _imageMap.end())
        return it->second;
    CRCore::ref_ptr<CRCore::crImage> pImage = NULL;

    // Warning: fbx->GetRelativeFileName() is relative TO EXECUTION DIR
    //          fbx->GetFileName() is as stored initially in the FBX
    if ((pImage = CRIOManager::readImageFile(CRIOManager::concatPaths(_dir, fbx->GetFileName()), _options)) ||                // First try "export dir/name"
        (pImage = CRIOManager::readImageFile(fbx->GetFileName(), _options)) ||                                        // Then try  "name" (if absolute)
        (pImage = CRIOManager::readImageFile(CRIOManager::concatPaths(_dir, fbx->GetRelativeFileName()), _options)))        // Else try  "current dir/name"
    {
        CRCore::ref_ptr<CRCore::crTexture2D> pOsgTex = new CRCore::crTexture2D;
        pOsgTex->setImage(pImage.get());
        pOsgTex->setWrap(CRCore::crTexture2D::WRAP_S, convertWrap(fbx->GetWrapModeU()));
        pOsgTex->setWrap(CRCore::crTexture2D::WRAP_T, convertWrap(fbx->GetWrapModeV()));
        _imageMap.insert(std::make_pair(fbx->GetFileName(), pOsgTex.get()));
        return pOsgTex;
    }
    else
    {
        return NULL;
    }
}

void FbxMaterialToOsgStateSet::checkInvertTransparency()
{
    int zeroAlpha = 0, oneAlpha = 0;
    for (FbxMaterialMap::const_iterator it = _fbxMaterialMap.begin(); it != _fbxMaterialMap.end(); ++it)
    {
        const CRCore::crMaterial* pMaterial = it->second.material.get();
        float alpha = pMaterial->getDiffuse(CRCore::crMaterial::FRONT).a();
        if (alpha > 0.999f)
        {
            ++oneAlpha;
        }
        else if (alpha < 0.001f)
        {
            ++zeroAlpha;
        }
    }

    if (zeroAlpha > oneAlpha)
    {
        //Transparency values seem to be back to front so invert them.

        for (FbxMaterialMap::const_iterator it = _fbxMaterialMap.begin(); it != _fbxMaterialMap.end(); ++it)
        {
            CRCore::crMaterial* pMaterial = it->second.material.get();
            CRCore::crVector4 diffuse = pMaterial->getDiffuse(CRCore::crMaterial::FRONT);
            diffuse.a() = 1.0f - diffuse.a();
            pMaterial->setDiffuse(CRCore::crMaterial::FRONT_AND_BACK, diffuse);
        }
    }
}
