// Pool.cpp

// Modify TexturePool to store a flt::AttrData object instead of a CRCore::crStateSet
// Julian Ortiz, June 18th 2003.

#if defined(_MSC_VER)
    #pragma warning( disable : 4786 )
#endif

//#include <CRCore/crBrain.h>
#include <CRCore/crVector4.h>
#include <CRCore/crTexture2D.h>
#include <CRCore/crTexEnv.h>

#include <CRIOManager/crReadFile.h>
#include <CRIOManager/crRegistry.h>

#include <Plugin flt\MaterialPaletteRecord.h>
#include <Plugin flt\OldMaterialPaletteRecord.h>
#include <Plugin flt\Pool.h>
#include <Plugin flt\Registry.h>
#include <Plugin flt\AttrData.h>

#include <stdio.h>


using namespace flt;


CRCore::crVector4 ColorPool::getColor(int nColorIntensity)
{
    // nColorIntensity:
    // bit 0-6:  intensity
    // bit 7-15  color index

    CRCore::crVector4 col(1,1,1,1);

    if (nColorIntensity >= 0)
    {
        ColorName* cn = getColorName(nColorIntensity >> 7);
        if (cn)
            col = cn->getColor();

        float intensity = (float)(nColorIntensity & 0x7f)/127.f;
        col[0] *= intensity;
        col[1] *= intensity;
        col[2] *= intensity;
    }

    return col;
}


// getColor for version 11, 12 & 13.
CRCore::crVector4 ColorPool::getOldColor(int nColorIntensity)
{
    // nColorIntensity:
    // bit 0-6:  intensity
    // bit 7-11  color index
    // bit 12    fixed intensity bit

    CRCore::crVector4 col(1,1,1,1);

    if (nColorIntensity >= 0)
    {
        int nIndex;
        bool bFixedIntensity = (nColorIntensity & 0x1000) ? true : false;

        if (bFixedIntensity)
            nIndex = (nColorIntensity & 0x0fff)+(4096>>7);
        else
            nIndex = nColorIntensity >> 7;

        ColorName* cn = getColorName(nIndex);
        if (cn)
            col = cn->getColor();

        // intensity
        if (!bFixedIntensity)
        {
            float intensity = (float)(nColorIntensity & 0x7f)/127.f;
            col[0] *= intensity;
            col[1] *= intensity;
            col[2] *= intensity;
        }
    }

    return col;
}


void ColorPool::addColor(int nIndex, const CRCore::crVector4& color)
{
    if (nIndex >= 0)
    {
        ColorName* colorname = new ColorName;
        colorname->setColor(color);

        _colorNameMap[nIndex] = colorname;
    }
}


ColorPool::ColorName* ColorPool::getColorName(int nIndex)
{
    ColorNameMap::iterator itr = _colorNameMap.find(nIndex);
    if (itr != _colorNameMap.end())
        return (*itr).second.get();

    return NULL;
}


////////////////////////////////////////////////////////////////////


flt::AttrData* TexturePool::getTexture(int nIndex, int fltVersion)
{
    TexturePaletteMap::iterator fitr = m_textureMap.find(nIndex);
    if (fitr != m_textureMap.end())
    {
        return (*fitr).second.get();
    }
    else
    {
        // no existing texture state set set up so lets look
        // for a file name for this nIndex..
        TextureNameMap::iterator nitr = m_textureNameMap.find(nIndex);
        if (nitr != m_textureNameMap.end())
        {
            const std::string& textureName = (*nitr).second;

            // Valid index, find the texture
            // Get AttrData containing texture from registry pool.
            flt::AttrData* textureAttrData = Registry::instance()->getTexture(textureName);

            if (textureAttrData)
            {
                // Add texture to local pool to be ab121le to get by index.
                addTexture(nIndex, textureAttrData);
            }
            else
            {
                //CERR<<"setTexture attempting to load ("<<textureName<<")"<<std::endl;

                unsigned int unit = 0;
				int texid = -1;
                CRCore::crStateSet *outStateSet = NULL;
				CRCore::crTexture2D* outTexture = NULL;

                // Read texture and attribute file
                CRCore::ref_ptr<CRCore::crImage> image = CRIOManager::readImageFile(textureName);
                if (image.valid())
                {
                    std::string attrName(textureName);
                    attrName += ".attr";

                    // Read attribute file
                    char options[256];
                    sprintf(options,"FLT_VER %d\0",fltVersion);

                    // Add this line to save the existing options
                    CRCore::ref_ptr<CRIOManager::crReaderWriter::Options> oldOptions = CRIOManager::crRegistry::instance()->getOptions();

                    CRIOManager::crRegistry::instance()->setOptions(new CRIOManager::crReaderWriter::Options(options));
                    //textureAttrData = NULL;//dynamic_cast<flt::AttrData*>(CRIOManager::readObjectFile(attrName));
					textureAttrData = dynamic_cast<flt::AttrData*>(CRIOManager::readObjectFile(attrName));
                    
					//if(image->isImageTranslucent())
					//{
					//	CRCore::crAlphaFunc *alphaFunc = new CRCore::crAlphaFunc;
					//	alphaFunc->setFunction(CRCore::crAlphaFunc::GREATER,0.95f);
					//	textureAttrData->statedata->setAlphaFunc(alphaFunc);
					//}

                    // Changed this line to restore the old options
                    CRIOManager::crRegistry::instance()->setOptions( oldOptions.get() );      // Restore options

                    // if not found create default crStateSet for the AttrData
					if(textureAttrData)
					{
						CRCore::crTexture2D* t2d = dynamic_cast<CRCore::crTexture2D*>(textureAttrData->stateset->getTextureAttribute( unit, CRCore::crStateAttribute::TEXTURE));

						if(t2d)
						{
							t2d->setImage(image.get());
							t2d->setImageNameID(CRCore::crArgumentParser::getSimpleFileName(image->getFileName()));
						}
						//texid = CRCore::crBrain::getInstance()->getResource()->findOrInsertTexture(nIndex,textureName,t2d);
					}
                    else
                    {
                        textureAttrData = new flt::AttrData;
                        outStateSet = new CRCore::crStateSet;
						textureAttrData->stateset = outStateSet;

                        outTexture = new CRCore::crTexture2D;
                        outTexture->setImage(image.get());
						outTexture->setImageNameID(CRCore::crArgumentParser::getSimpleFileName(image->getFileName()));
                        outTexture->setWrap(CRCore::crTexture2D::WRAP_S,CRCore::crTexture2D::REPEAT);
                        outTexture->setWrap(CRCore::crTexture2D::WRAP_T,CRCore::crTexture2D::REPEAT);
	                    
						//texid = CRCore::crBrain::getInstance()->getResource()->findOrInsertTexture(nIndex,textureName,outTexture);
						//if(texid>-1) 
						{	
                            CRCore::crTexEnv* outTexEnv = new CRCore::crTexEnv;
                            outTexEnv->setMode(CRCore::crTexEnv::MODULATE);
							outStateSet->setTextureAttributeAndModes(unit,outTexEnv,CRCore::crStateAttribute::ON);  
							outStateSet->setTextureAttributeAndModes(unit,outTexture);
						}

                    }

                    outStateSet = textureAttrData->stateset;
                    if (outStateSet == NULL)
                    {
                        outStateSet = new CRCore::crStateSet;
						textureAttrData->stateset = outStateSet;

						outTexture = new CRCore::crTexture2D;
						outTexture->setImage(image.get());
						outTexture->setImageNameID(CRCore::crArgumentParser::getSimpleFileName(image->getFileName()));
						//texid = CRCore::crBrain::getInstance()->getResource()->findOrInsertTexture(nIndex,textureName,outTexture);

					    outStateSet->setTextureAttributeAndModes(unit,outTexture);
                    }                    
                }
                else
                {
                    // invalid image file, register an empty state set AttrData
                    textureAttrData = new flt::AttrData;
                    textureAttrData->stateset = new CRCore::crStateSet;
                }

                // Add new texture to registry pool
                // ( umm... should this have reference to the texture unit? RO. July2002)
                Registry::instance()->addTexture(textureName, textureAttrData);

                // Also add to local pool to be able to get texture by index.
                // ( umm... should this have reference to the texture unit? RO. July2002)
                addTexture(nIndex, textureAttrData);

                //CERR<<"crRegistry::instance()->addTexture("<<textureName<<", "<<textureAttrData<<")"<<std::endl;
                //CERR<<"pTexturePool->addTexture("<<nIndex<<", "<<textureAttrData<<")"<<std::endl;
            }

            return textureAttrData;
        }
    }
    return NULL;
}

std::string* TexturePool::getTextureName(int nIndex)
{
    TextureNameMap::iterator fitr = m_textureNameMap.find(nIndex);
    if (fitr != m_textureNameMap.end())
        return &(*fitr).second;
    else
        return NULL;
}


void TexturePool::addTexture(int nIndex, flt::AttrData* attrdata)
{
    m_textureMap[nIndex] = attrdata;
}

void TexturePool::addTextureName(int nIndex, const std::string& name)
{
    m_textureNameMap[nIndex] = name;
}

////////////////////////////////////////////////////////////////////


CRCore::crLight* LightPool::getLight(int nIndex)
{
    if (nIndex < 0) return NULL;
    LightPaletteMap::iterator fitr = m_lightMap.find(nIndex);
    if (fitr != m_lightMap.end())
        return (*fitr).second.get();

    return NULL;
}


void LightPool::addLight(int nIndex, CRCore::crLight* light)
{
    m_lightMap[nIndex] = light;
}

CRCore::crMaterial* MaterialPool::getCreMaterial(int nIndex)
{
	if (nIndex < 0) return NULL;
	CreMaterialMap::iterator fitr = m_creMaterialMap.find(nIndex);
	if (fitr != m_creMaterialMap.end())
		return (*fitr).second.get();

	return NULL;
}

void MaterialPool::addCreMaterial(int nIndex, CRCore::crMaterial* material)
{
    m_creMaterialMap[nIndex] = material;
}

MaterialPool::PoolMaterial* MaterialPool::getMaterial(int nIndex)
{
    if (nIndex < 0) return NULL;
    MaterialMap::iterator fitr = m_MaterialMap.find(nIndex);
    if (fitr != m_MaterialMap.end())
        return (*fitr).second.get();

    return NULL;
}


void MaterialPool::addMaterial(int nIndex, PoolMaterial* material)
{
    m_MaterialMap[nIndex] = material;
}

CRCore::crGroup* InstancePool::getInstance(int nIndex)
{
    InstanceMap::iterator fitr = m_instanceMap.find(nIndex);
    if (fitr != m_instanceMap.end())
        return (*fitr).second.get();
    else
        return NULL;
}


void InstancePool::addInstance(int nIndex, CRCore::crGroup* instance)
{
    m_instanceMap[nIndex] = instance;
}


LtPtAppearancePool::PoolLtPtAppearance* LtPtAppearancePool::get(int nIndex)
{
    if (nIndex < 0)
        return NULL;

    AppearanceMap::iterator fitr = m_appearanceMap.find(nIndex);
    if (fitr != m_appearanceMap.end())
        return (*fitr).second.get();

    return NULL;
}


void LtPtAppearancePool::add(int nIndex, PoolLtPtAppearance* appearance)
{
    m_appearanceMap[nIndex] = appearance;
}


LtPtAnimationPool::PoolLtPtAnimation*
LtPtAnimationPool::get( int nIndex )
{
    if (nIndex < 0)
        return NULL;

    AnimationMap::iterator fitr = m_animationMap.find(nIndex);
    if (fitr != m_animationMap.end())
        return (*fitr).second.get();

    return NULL;
}


void
LtPtAnimationPool::add(int nIndex, PoolLtPtAnimation* anim)
{
    m_animationMap[nIndex] = anim;
}




