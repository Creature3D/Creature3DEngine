// FltFile.h

#ifndef __FLT_FILE_H
#define __FLT_FILE_H

#include <CRCore/crGroup.h>

#include <map>
#include <string>


#include <Plugin flt/Pool.h>
#include <Plugin flt/HeaderRecord.h>

namespace flt {

class Record;


class FltFile : public CRCore::Referenced
{

public:
    FltFile(
        ColorPool* pColorPool = NULL,
        TexturePool* pTexturePool = NULL,
        MaterialPool* pMaterialPool = NULL,
        LtPtAppearancePool* pLtPtAppearancePool = NULL,
        LtPtAnimationPool* pLtPtAnimationPool = NULL);

    virtual CRCore::crBase* readObject(const std::string& fileName);
    virtual CRCore::crNode* readNode(const std::string& fileName);
    CRCore::crGroup* convert();
    bool readModel(const std::string& fileName);

    ColorPool*      getColorPool()      { return m_colorPool.get(); }
    TexturePool*    getTexturePool()    { return m_texturePool.get(); }
    LightPool*      getLightPool()   { return m_lightPool.get(); }
    MaterialPool*   getMaterialPool()   { return m_materialPool.get(); }
    InstancePool*   getInstancePool()   { return m_instancePool.get(); }
    LtPtAppearancePool* getLtPtAppearancePool() { return m_ltPtAppearancePool.get(); }
    LtPtAnimationPool* getLtPtAnimationPool() { return m_ltPtAnimationPool.get(); }

    void setColorPool(ColorPool* colorPool)         { m_colorPool = colorPool; }
    void setTexturePool(TexturePool* texturePool)   { m_texturePool = texturePool; }
    void setLightPool(LightPool* lightPool){ m_lightPool = lightPool; }
    void setMaterialPool(MaterialPool* materialPool){ m_materialPool = materialPool; }
    void setInstancePool(InstancePool* instancePool){ m_instancePool = instancePool; }
    void setLtPtAppearancePool(LtPtAppearancePool* ltPtAppearancePool){ m_ltPtAppearancePool = ltPtAppearancePool; }
    void setLtPtAnimationPool(LtPtAnimationPool* ltPtAnimationPool){ m_ltPtAnimationPool = ltPtAnimationPool; }

    inline bool useInternalColorPalette() const    { return m_useInternalColorPalette; }
    inline bool useInternalTexturePalette() const  { return m_useInternalTexturePalette; }
    inline bool useInternalMaterialPalette() const { return m_useInternalMaterialPalette; }
    inline bool useInternalLtPtPalettes() const { return m_useInternalLtPtPalettes; }

    void setUseTextureAlphaForTransparancyBinning(bool flag) { m_useTextureAlphaForTransparancyBinning=flag; }
    bool getUseTextureAlphaForTransparancyBinning() const { return m_useTextureAlphaForTransparancyBinning; }
    void setDoUnitsConversion(bool flag) { m_doUnitsConversion=flag; }
    bool getDoUnitsConversion() const { return m_doUnitsConversion; }

    typedef enum { 
        ConvertToMeters, 
        ConvertToKilometers, 
        ConvertToFeet,
        ConvertToInches, 
        ConvertToNauticalMiles 
    } ConvertUnits;
    void setDesiredUnits( FltFile::ConvertUnits units ) { m_desiredUnits=units; }
    FltFile::ConvertUnits getDesiredUnits() const { return m_desiredUnits; }
    std::string getDesiredUnitsString() const;

    int getFlightVersion() const;
    inline HeaderRecord* getHeaderRecord() { return m_headerRecord.get(); }
    void getOrigin( double& latitude, double& longitude ) const;

protected:

    virtual ~FltFile() {}

    bool readFile(const std::string& fileName);
    void readExternals();

private:

    CRCore::ref_ptr<HeaderRecord>        m_headerRecord;

    bool                        m_useInternalColorPalette;
    bool                        m_useInternalTexturePalette;
    bool                        m_useInternalMaterialPalette;
    bool                        m_useInternalLtPtPalettes;
    bool                        m_useTextureAlphaForTransparancyBinning;
    bool                        m_doUnitsConversion;
    ConvertUnits                m_desiredUnits;

    std::string                 m_directory;
    
    CRCore::ref_ptr<ColorPool>     m_colorPool;
    CRCore::ref_ptr<TexturePool>   m_texturePool;
    CRCore::ref_ptr<LightPool>     m_lightPool;
    CRCore::ref_ptr<MaterialPool>  m_materialPool;
    CRCore::ref_ptr<InstancePool>  m_instancePool;
    CRCore::ref_ptr<LtPtAppearancePool> m_ltPtAppearancePool;
    CRCore::ref_ptr<LtPtAnimationPool> m_ltPtAnimationPool;
};



}; // end namespace flt

#endif
