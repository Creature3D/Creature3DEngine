/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/

//
// OpenFlight?loader for OpenSceneGraph
//
//  Copyright (C) 2005-2007  Brede Johansen
//

#ifndef FLT_POOLS_H
#define FLT_POOLS_H 1

#include <vector>
#include <map>
#include <sstream>
#include <CRCore/crVector4.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crMaterial.h>
#include <CRCore/crLight.h>
#include <CRCore/crProgram.h>
#include <Plugin OpenFlight/Types.h>


namespace flt {

class VertexPool : public CRCore::Referenced, public std::istringstream
{
public:

    explicit VertexPool( const std::string& str) :
        std::istringstream(str,std::istringstream::in|std::istringstream::binary) {}

protected:

    virtual ~VertexPool() {}
};


class ColorPool : public CRCore::Referenced , public std::vector<CRCore::crVector4>
{
public:

    explicit ColorPool(bool old,int size) :
        std::vector<CRCore::crVector4>(size),
        _old(old) {}

    CRCore::crVector4 getColor(int indexIntensity) const;

protected:

    virtual ~ColorPool() {}

    bool _old; // true if version <= 13
};


class TexturePool : public CRCore::Referenced , public std::map<int,CRCore::ref_ptr<CRCore::crStateSet> >
{
public:

    TexturePool() {}

    CRCore::crStateSet* get(int index)
    {
        iterator itr = find(index);
        if (itr != end())
            return (*itr).second.get();
        return NULL;
    }

protected:

    virtual ~TexturePool() {}

};


class MaterialPool : public CRCore::Referenced , public std::map<int,CRCore::ref_ptr<CRCore::crMaterial> >
{
public:

    MaterialPool();

    // Get material, return default material if not found in palette.
    CRCore::crMaterial* get(int index);

    // Get or create material based on
    // index: face material index
    // color: face color with alpha set to 1-face transparency.
    CRCore::crMaterial* getOrCreateMaterial(int index, const CRCore::crVector4& faceColor);

protected:

    virtual ~MaterialPool() {}

    CRCore::ref_ptr<CRCore::crMaterial> _defaultMaterial;

    struct MaterialParameters
    {
        int index;              // face index to material pool
        CRCore::crVector4 color;        // face color with alpha set to 1-face transparency.

        MaterialParameters():
            index(-1) {}

        MaterialParameters(int i, const CRCore::crVector4& c):
            index(i),
            color(c) {}

        bool operator < (const MaterialParameters& rhs) const
        {
            if (index < rhs.index) return true;
            else if (index > rhs.index) return false;
            else return (color < rhs.color);
        }
    };

    // crMaterial from palette combined with face color stored here for reuse.
    typedef std::map<MaterialParameters,CRCore::ref_ptr<CRCore::crMaterial> > FinalMaterialMap;
    FinalMaterialMap _finalMaterialMap;
};


class LightSourcePool : public CRCore::Referenced , public std::map<int,CRCore::ref_ptr<CRCore::crLight> >
{
public:

    LightSourcePool() {}

    CRCore::crLight* get(int index)
    {
        iterator itr = find(index);
        if (itr != end())
            return (*itr).second.get();
        return NULL;
    }

protected:

    virtual ~LightSourcePool() {}
};


struct LPAppearance : public CRCore::Referenced
{
    std::string name;
    int32 index;
    int16 materialCode;
    int16 featureID;
    CRCore::crVector4 backColor;
    int32 displayMode;
    float32 intensityFront;
    float32 intensityBack;
    float32 minDefocus;
    float32 maxDefocus;
    int32 fadingMode;
    int32 fogPunchMode;
    int32 directionalMode;
    int32 rangeMode;
    float32 minPixelSize;
    float32 maxPixelSize;
    float32 actualPixelSize;
    float32 transparentFalloffPixelSize;
    float32 transparentFalloffExponent;
    float32 transparentFalloffScalar;
    float32 transparentFalloffClamp;
    float32 fogScalar;
    float32 fogIntensity;
    float32 sizeDifferenceThreshold;
    int32 directionality;
    float32 horizontalLobeAngle;
    float32 verticalLobeAngle;
    float32 lobeRollAngle;
    float32 directionalFalloffExponent;
    float32 directionalAmbientIntensity;
    float32 significance;
    uint32 flags;
    float32 visibilityRange;
    float32 fadeRangeRatio;
    float32 fadeInDuration;
    float32 fadeOutDuration;
    float32 LODRangeRatio;
    float32 LODScale;
    int16 texturePatternIndex;
};


class LightPointAppearancePool : public CRCore::Referenced , public std::map<int,CRCore::ref_ptr<LPAppearance> >
{
public:

    LightPointAppearancePool() {}

    LPAppearance* get(int index)
    {
        iterator itr = find(index);
        if (itr != end())
            return (*itr).second.get();
        return NULL;
    }

protected:

    virtual ~LightPointAppearancePool() {}

};

struct LPAnimation : public CRCore::Referenced
{
    enum AnimationType
    {
        FLASHING_SEQUENCE = 0,
        ROTATING = 1,
        STROBE = 2,
        MORSE_CODE = 3
    };

    enum State
    {
        ON = 0,
        OFF = 1,
        COLOR_CHANGE = 2
    };

    struct Pulse
    {
        uint32 state;
        float32 duration;
        CRCore::crVector4 color;
    };

    typedef std::vector<Pulse>  PulseArray;

    std::string name;                        // animation name
    int32 index;                            // animation index
    float32 animationPeriod;                // animation period, in seconds
    float32 animationPhaseDelay;            // animation phase delay, in seconds from start of period
    float32 animationEnabledPeriod;            // animation enabled period (time on), in seconds
    CRCore::crVector3 axisOfRotation;                // axis of rotation for rotating animation (i, j, k)
    uint32 flags;                            // flags (bits, from left to right)
                                            //     0 = flashing
                                            //   1 = rotating
                                            //   2 = rotate counter clockwise
                                            //   3-31 = spare
    int32 animationType;                    // animation type
                                            //     0 = flashing sequence
                                            //   1 = rotating
                                            //   2 = strobe
                                            //   3 = morse code
    int32 morseCodeTiming;                    // morse code timing
                                            //     0 = standard timing
                                            //   1 = Farnsworth timing
    int32 wordRate;                            // word rate (for Farnsworth timing)
    int32 characterRate;                    // character rate (for Farnsworth timing)
    std::string morseCodeString;            // morse code string
    PulseArray sequence;
};


class LightPointAnimationPool : public CRCore::Referenced , public std::map<int,CRCore::ref_ptr<LPAnimation> >
{
public:

    LightPointAnimationPool() {}

    LPAnimation* get(int index)
    {
        iterator itr = find(index);
        if (itr != end())
            return (*itr).second.get();
        return NULL;
    }

protected:

    virtual ~LightPointAnimationPool() {}

};

class ShaderPool : public CRCore::Referenced , public std::map<int,CRCore::ref_ptr<CRCore::crProgram> >
{
public:

	ShaderPool() {}

	CRCore::crProgram* get(int index)
	{
		iterator itr = find(index);
		if (itr != end())
			return (*itr).second.get();
		return NULL;
	}

protected:

	virtual ~ShaderPool() {}
};


// This object records parent palettes for external record support.
// When an external record is parsed, this object is instantiated and populated with
// the parent model's palettes, then stored as UserData on the crProxyNode.
// When the ReadExternalsVisitor hits the crProxyNode, it moves this object
// into the crReaderWriter Options' UserData before calling CRIOManager::ReadNode,
// enabling  access to the parent palettes during load of the ext ref model.
class ParentPools : public CRCore::Referenced
{
public:

    ParentPools() {}

    void setColorPool(ColorPool* pool) { _colorPool=pool; }
    ColorPool* getColorPool() { return _colorPool.get(); }

    void setTexturePool(TexturePool* pool) { _texturePool=pool; }
    TexturePool* getTexturePool() { return _texturePool.get(); }

    void setMaterialPool(MaterialPool* pool) { _materialPool=pool; }
    MaterialPool* getMaterialPool() { return _materialPool.get(); }

    void setLightSourcePool(LightSourcePool* pool) { _lightSourcePool=pool; }
    LightSourcePool* getLightSourcePool() { return _lightSourcePool.get(); }

    void setLPAppearancePool(LightPointAppearancePool* pool) { _lpAppearancePool=pool; }
    LightPointAppearancePool* getLPAppearancePool() { return _lpAppearancePool.get(); }

    void setLPAnimationPool(LightPointAnimationPool* pool) { _lpAnimationPool=pool; }
    LightPointAnimationPool* getLPAnimationPool() { return _lpAnimationPool.get(); }

    //void setShaderPool(ShaderPool* pool) { _shaderPool=pool; }
    //ShaderPool* getShaderPool() const { return _shaderPool.get(); }

protected:

    virtual ~ParentPools() {}

    CRCore::ref_ptr<ColorPool> _colorPool;
    CRCore::ref_ptr<MaterialPool> _materialPool;
    CRCore::ref_ptr<TexturePool> _texturePool;
    CRCore::ref_ptr<LightSourcePool> _lightSourcePool;
    CRCore::ref_ptr<LightPointAppearancePool> _lpAppearancePool;
    CRCore::ref_ptr<LightPointAnimationPool> _lpAnimationPool;
  //  CRCore::ref_ptr<ShaderPool> _shaderPool;
};


} // end namespace

#endif
