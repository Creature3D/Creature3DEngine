#ifndef __FLT_POOL_H
#define __FLT_POOL_H

// Modify TexturePool to store a flt::AttrData object instead of a CRCore::crStateSet
// Julian Ortiz, June 18th 2003.

#include <Plugin flt\flt.h>

#include <CRCore/ref_ptr.h>
#include <CRCore/Referenced.h>
#include <CRCore/crVector4.h>
#include <CRCore/crMaterial.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crLight.h>
#include <CRCore/crGroup.h>
//#include <CRSim/crBlinkSequence.h>

#include <Plugin flt\AttrData.h>

#include <string>
#include <algorithm>
#include <map>

namespace flt {


class ColorPool : public CRCore::Referenced
{
    public :

        ColorPool() {}

        CRCore::crVector4 getColor(int nColorIntensity);
        CRCore::crVector4 getOldColor(int nColorIntensity);
        void addColor(int nIndex, const CRCore::crVector4& color);

    protected :

        virtual ~ColorPool() {}

    private :

        class ColorName : public CRCore::Referenced
        {
            public:
                void setName( const std::string& name ) { _name = name; }
                const std::string& getName( void )      { return _name; }
                void setColor(const CRCore::crVector4& color )  { _color = color; }
                CRCore::crVector4 getColor()                    { return _color; }

            private:
                CRCore::crVector4   _color;
                std::string _name;
        };

        ColorName* getColorName(int nIndex);

        typedef std::map<int,CRCore::ref_ptr<ColorName> > ColorNameMap;
        ColorNameMap _colorNameMap;
};


class TexturePool : public CRCore::Referenced
{
    public :

        TexturePool() {}

        flt::AttrData* getTexture(int nIndex, int fltVersion);
        std::string* getTextureName(int nIndex);
        void addTexture(int nIndex, flt::AttrData* attrdata);
        void addTextureName(int nIndex, const std::string& name);

    protected :

        virtual ~TexturePool() {}

    private :

        typedef std::map<int,CRCore::ref_ptr<flt::AttrData> > TexturePaletteMap;
        TexturePaletteMap m_textureMap;
        typedef std::map<int,std::string > TextureNameMap;
        TextureNameMap m_textureNameMap;
};



class LightPool : public CRCore::Referenced
{
    public :

        LightPool() {}

        CRCore::crLight* getLight(int nIndex );
        void addLight(int nIndex, CRCore::crLight* light);

    protected :

        virtual ~LightPool() {}

    private :

        typedef std::map<int,CRCore::ref_ptr<CRCore::crLight> > LightPaletteMap;
        LightPaletteMap m_lightMap;
};



class MaterialPool : public CRCore::Referenced
{
    public:

        struct PoolMaterial : public CRCore::Referenced
        {
            float32x3    Ambient;        // Ambient  component of material
            float32x3    Diffuse;        // Diffuse  component of material
            float32x3    Specular;       // Specular component of material
            float32x3    Emissive;       // Emissive component of material
            float32      sfShininess;    // Shininess. [0.0-128.0]
            float32      sfAlpha;        // Alpha. [0.0-1.0], where 1.0 is opaque
        };
        
        MaterialPool() {}

		CRCore::crMaterial* getCreMaterial(int nIndex);
		void addCreMaterial(int nIndex, CRCore::crMaterial* material);

        PoolMaterial* getMaterial(int nIndex);
        void addMaterial(int nIndex, PoolMaterial* material);
    protected :

        virtual ~MaterialPool() {}

    private:

        typedef std::map<int, CRCore::ref_ptr<PoolMaterial> > MaterialMap;
		typedef std::map<int, CRCore::ref_ptr<CRCore::crMaterial> > CreMaterialMap;
        MaterialMap m_MaterialMap;
		CreMaterialMap m_creMaterialMap;
};


class InstancePool : public CRCore::Referenced
{
    public :

        InstancePool() {}

        CRCore::crGroup* getInstance(int nIndex);
        void addInstance(int nIndex, CRCore::crGroup* instance);

    protected :

        virtual ~InstancePool() {}

    private :

        typedef std::map<int,CRCore::ref_ptr<CRCore::crGroup> > InstanceMap;
        InstanceMap m_instanceMap;
};


class LtPtAppearancePool : public CRCore::Referenced
{
public:
    struct PoolLtPtAppearance : public CRCore::Referenced
    {
        unsigned int m_iBackColorIdx;
        float m_bIntensity;
        float m_sfMinPixelSize;
        float m_sfMaxPixelSize;
        float m_sfActualSize;
        int m_iDirectionality;
        float m_sfHLobeAngle;
        float m_sfVLobeAngle;
        float m_sfLobeRollAngle;
    };

    LtPtAppearancePool()
    {}

    PoolLtPtAppearance* get(int nIndex);
    void add(int nIndex, PoolLtPtAppearance* appearance);

protected:
    ~LtPtAppearancePool() {}

private:
    typedef std::map<int, CRCore::ref_ptr<PoolLtPtAppearance> > AppearanceMap;
    AppearanceMap m_appearanceMap;
};

class LtPtAnimationPool : public CRCore::Referenced
{
public:
    struct PoolLtPtAnimation : public CRCore::Referenced
    {
		std::string m_name;
		//CRCore::ref_ptr<CRSim::crBlinkSequence> m_blink;
    };

    LtPtAnimationPool()
    {}

    PoolLtPtAnimation* get( int nIndex );
    void add( int nIndex, PoolLtPtAnimation* anim );

protected:
    ~LtPtAnimationPool() {}

private:
    typedef std::map<int, CRCore::ref_ptr<PoolLtPtAnimation> > AnimationMap;
    AnimationMap m_animationMap;
};


}; // end namespace flt

#endif

