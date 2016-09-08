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
// OpenFlight loader for OpenSceneGraph
//
//  Copyright (C) 2005-2007  Brede Johansen
//

#include <assert.h>
#include <typeinfo>
#include <CRCore/crNotify.h>
#include <CRCore/crShadeModel.h>
#include <CRCore/crProxyNode.h>
#include <CRCore/crSequence.h>
#include <CRCore/crLod.h>
#include <CRCore/crProxyNode.h>
#include <CRCore/crLightSource.h>
#include <CRIOManager/crFileUtils.h>
#include <CRCore/crDOFTransform.h>
#include <CRCore/crMultiSwitch.h>
//#include <CRCore/GeographicLocation>
//#include <CRCore/ObjectRecordData>

#include <CRCore/crNotify.h>
//#include <CRCore/io_utils.h>

#include <Plugin OpenFlight/Registry.h>
#include <Plugin OpenFlight/Document.h>
#include <Plugin OpenFlight/RecordInputStream.h>

namespace flt {

/** Header
*/
class Header : public PrimaryRecord
{
    static const unsigned int SAVE_VERTEX_NORMALS_BIT = 0x80000000u >> 0;
    static const unsigned int PACKED_COLOR_MODE_BIT   = 0x80000000u >> 1;
    static const unsigned int CAD_VIEW_MODE_BIT       = 0x80000000u >> 2;

    CRCore::ref_ptr<CRCore::crGroup> _header;

public:

    Header() {}

    META_Record(Header)

    META_setID(_header)
    META_setComment(_header)
    META_setMultitexture(_header)
    META_addChild(_header)

protected:

    virtual ~Header() {}

    virtual void readRecord(RecordInputStream& in, Document& document)
    {
        std::string id = in.readString(8);
        CRCore::notify(CRCore::DEBUG_INFO) << "ID: " << id << std::endl;

        uint32 format = in.readUInt32();
        CRCore::notify(CRCore::DEBUG_INFO) << "Format: " << format << std::endl;
        document._version = format;

        /*uint32 revision =*/ in.readUInt32();

        std::string revisionTime = in.readString(32);
        CRCore::notify(CRCore::INFO) << "Last revision: " << revisionTime << std::endl;

        in.forward(4*2);

        // Flight v.11 & 12 use integer coordinates
        int16 multDivUnits = in.readInt16();        // Units multiplier/divisor
        uint8 units = in.readUInt8();               // 0=Meters 1=Kilometers 4=Feet 5=Inches 8=Nautical miles
        /*uint8 textureWhite =*/ in.readUInt8();
        /*uint32 flags =*/ in.readUInt32();

        in.forward( 4*6 );
        /*int32 projectionType =*/ in.readInt32();

        in.forward( 4*7 );
        /*int16 nextDOF =*/ in.readInt16();
        /*int16 vertStorage =*/ in.readInt16();
        /*int32 dbOrigin =*/ in.readInt32();

        /*float64 swX =*/ in.readFloat64();
        /*float64 swY =*/ in.readFloat64();
        /*float64 deltaX =*/ in.readFloat64();
        /*float64 deltaY =*/ in.readFloat64();

        in.forward( 2*2 ); /* some "next bead" IDs */
        in.forward( 4*2 ); /* reserved */
        in.forward( 4*2 ); /* more "next bead" IDs */
        in.forward( 4 ); /* reserved */

        /*float64 swLat =*/ in.readFloat64();
        /*float64 swLong =*/ in.readFloat64();
        /*float64 neLat =*/ in.readFloat64();
        /*float64 neLong =*/ in.readFloat64();
        float64 originLat = in.readFloat64();
        float64 originLong = in.readFloat64();

        if (document.getDoUnitsConversion())
            document._unitScale = unitsToMeters((CoordUnits)units) / unitsToMeters(document.getDesiredUnits());

        if (document._version < VERSION_13)
        {
            if (multDivUnits >= 0)
                document._unitScale *= (double)multDivUnits;
            else
                document._unitScale /= (double)(-multDivUnits);
        }

        _header = new CRCore::crGroup;
        _header->setName(id);

        // Store model origin in returned crNode userData.
        //CRCore::GeographicLocation* loc = new CRCore::GeographicLocation;
        //loc->set( originLat, originLong );
        //_header->setUserData( loc );
        //CRCore::notify(CRCore::INFO) << "DB lat=" << originLat << " lon=" << originLong << std::endl;

        document.setHeaderNode(_header.get());
    }

    virtual void dispose(Document& document)
    {
        if (_header.valid())
        {
            // Preset sampler uniforms.
            ShaderPool* sp = document.getShaderPool();
            if (sp && !sp->empty())
            {
                _header->getOrCreateStateSet()->addUniform( new CRCore::crUniform("TextureUnit0", 0) );
                _header->getOrCreateStateSet()->addUniform( new CRCore::crUniform("TextureUnit1", 1) );
                _header->getOrCreateStateSet()->addUniform( new CRCore::crUniform("TextureUnit2", 2) );
                _header->getOrCreateStateSet()->addUniform( new CRCore::crUniform("TextureUnit3", 3) );
            }
        }
    }
};

REGISTER_FLTRECORD(Header, HEADER_OP)


/** crGroup
*/
class crGroup : public PrimaryRecord
{
    static const unsigned int FORWARD_ANIM     = 0x80000000u >> 1;
    static const unsigned int SWING_ANIM       = 0x80000000u >> 2;
    static const unsigned int BOUND_BOX_FOLLOW = 0x80000000u >> 3;
    static const unsigned int FREEZE_BOUND_BOX = 0x80000000u >> 4;
    static const unsigned int DEFAULT_PARENT   = 0x80000000u >> 5;
    static const unsigned int BACKWARD_ANIM    = 0x80000000u >> 6;

    CRCore::ref_ptr<CRCore::crGroup> _group;
    uint32 _flags;
    bool _forwardAnim;
    bool _backwardAnim;
    int32 _loopCount;
    float32 _loopDuration;
    float32 _lastFrameDuration;

public:

    crGroup():
        _flags(0),
        _forwardAnim(false),
        _backwardAnim(false),
        _loopCount(0),
        _loopDuration(0),
        _lastFrameDuration(0)
    {}

    META_Record(crGroup)

    META_setID(_group)
    META_setComment(_group)
    META_setMultitexture(_group)
    META_addChild(_group)

    bool hasAnimation() const { return _forwardAnim || _backwardAnim; }

protected:

    void readRecord(RecordInputStream& in, Document& document)
    {
        std::string id = in.readString(8);
        CRCore::notify(CRCore::DEBUG_INFO) << "ID: " << id << std::endl;

        /*int16 relativePriority =*/ in.readInt16();
        in.forward(2);
        _flags = in.readUInt32(0);
        /*uint16 specialId0 =*/ in.readUInt16();
        /*uint16 specialId1 =*/ in.readUInt16();
        /*uint16 significance =*/ in.readUInt16();
        /*int8 layer =*/ in.readInt8();
        in.forward(5);
        // version >= VERSION_15_8
        _loopCount = in.readInt32(0);
        _loopDuration = in.readFloat32(0.0f);
        _lastFrameDuration = in.readFloat32(0.0f);

        // Check for forward animation (sequence)
        _forwardAnim = (_flags & FORWARD_ANIM) != 0;

        // For versions prior to 15.8, the swing bit can be set independently
        // of the animation bit.  This implies forward animation (with swing)
        if ((document.version() < VERSION_15_8) && (_flags & SWING_ANIM))
            _forwardAnim = true;

        // OpenFlight 15.8 adds backwards animations
        _backwardAnim = ( (document.version() >= VERSION_15_8) &&
            ((_flags & BACKWARD_ANIM) != 0) );

        if (_forwardAnim || _backwardAnim)
            _group = new CRCore::crSequence;
        else
            _group = new CRCore::crGroup;

        _group->setName(id);

        // Add this implementation to parent implementation.
        if (_parent.valid())
            _parent->addChild(*_group);
    }

    virtual void dispose(Document& document)
    {
        if (!_group.valid()) return;

        // Insert transform(s)
        if (_matrix.valid())
        {
            insertMatrixTransform(*_group,*_matrix,_numberOfReplications);
        }

        // Children are added!
        CRCore::crSequence* sequence = dynamic_cast<CRCore::crSequence*>(_group.get());
        if (sequence && sequence->getNumChildren() > 0)
        {
            // Regardless of forwards or backwards, animation could have swing bit set.
            CRCore::crSequence::LoopMode loopMode = ((_flags & SWING_ANIM) == 0) ?
                CRCore::crSequence::LOOP : CRCore::crSequence::SWING;

            if (_forwardAnim)
                sequence->setInterval(loopMode, 0, -1);
            else
                sequence->setInterval(loopMode, -1, 0);

            // Loop timing available from version 15.8.
            if (document.version() >= VERSION_15_8)
            {
                // Set frame duration.
                float frameDuration = _loopDuration / float(sequence->getNumChildren());
                for (unsigned int i=0; i < sequence->getNumChildren(); i++)
                    sequence->setTime(i, frameDuration);

                // Set number of repetitions.
                if (_loopCount > 0)
                    sequence->setDuration(1.0f, _loopCount);
                else
                    sequence->setDuration(1.0f);        // Run continuously
            }
            else // No timing available.
            {
                // Set frame duration
                float frameDuration = 0.1f;     // 10Hz
                for (unsigned int i=0; i < sequence->getNumChildren(); i++)
                    sequence->setTime(i, frameDuration);

                // Run continuously
                sequence->setDuration(1.0f);
            }

            sequence->setMode(CRCore::crSequence::START);
        }
    }
};

REGISTER_FLTRECORD(crGroup, GROUP_OP)



/** DegreeOfFreedom
*/
class DegreeOfFreedom : public PrimaryRecord
{
    // Flags
    static const unsigned long LIMIT_TRANSLATION_X = 0x80000000u >> 0;
    static const unsigned long LIMIT_TRANSLATION_Y = 0x80000000u >> 1;
    static const unsigned long LIMIT_TRANSLATION_Z = 0x80000000u >> 2;
    static const unsigned long LIMIT_PITCH         = 0x80000000u >> 3;
    static const unsigned long LIMIT_ROLL          = 0x80000000u >> 4;
    static const unsigned long LIMIT_YAW           = 0x80000000u >> 5;
    static const unsigned long LIMIT_SCALE_X       = 0x80000000u >> 6;
    static const unsigned long LIMIT_SCALE_Y       = 0x80000000u >> 7;
    static const unsigned long LIMIT_SCALE_Z       = 0x80000000u >> 8;

    struct Range
    {
        float64 min;            // Minimum value with respect to the local coord system
        float64 max;            // Maximum value with respect to the local coord system
        float64 current;        // Current value with respect to the local coord system
        float64 increment;      // Increment
    };

    CRCore::ref_ptr<CRCore::crDOFTransform> _dof;

public:

    DegreeOfFreedom():
        _dof(new CRCore::crDOFTransform) {}

    META_Record(DegreeOfFreedom)

    META_setID(_dof)
    META_setComment(_dof)
    META_setMultitexture(_dof)
    META_addChild(_dof)
    META_dispose(_dof)

protected:

    virtual ~DegreeOfFreedom() {}
    virtual void readRecord(RecordInputStream& in, Document& document)
    {
        std::string id = in.readString(8);
        in.forward(4);                                  // Reserved
        CRCore::crVector3d localOrigin = in.readVec3d();
        CRCore::crVector3d pointOnXAxis = in.readVec3d();
        CRCore::crVector3d pointInXYPlane = in.readVec3d();
        Range rangeZ = readRange(in);                   // Legal z values with respect to the local coord system
        Range rangeY = readRange(in);                   // Legal y values with respect to the local coord system
        Range rangeX = readRange(in);                   // Legal x values with respect to the local coord system
        Range rangePitch = readRange(in);               // Legal pitch values (rotation about the x-axis)
        Range rangeRoll = readRange(in);                // Legal roll values( rotation about the y-axis)
        Range rangeYaw = readRange(in);                 // Legal yaw values (rotation about the z-axis)
        Range rangeScaleZ = readRange(in);              // Legal z scale values (about local origin)
        Range rangeScaleY = readRange(in);              // Legal y scale values about local origin)
        Range rangeScaleX = readRange(in);              // Legal x scale values (about local origin)
        uint32 flags = in.readUInt32();                 // Flags, bits from left to right (see OF doc)

        // out of range check, required for racecar.flt (Creator Gallery)
        if (!valid(localOrigin))
            localOrigin = CRCore::crVector3d(0,0,0);
        if (!valid(pointOnXAxis))
            pointOnXAxis = CRCore::X_AXIS;
        if (!valid(pointInXYPlane))
            pointInXYPlane = CRCore::Y_AXIS;

        _dof->setName(id);

        //tranlsations:
        _dof->setMinTranslate(CRCore::crVector3(rangeX.min,rangeY.min,rangeZ.min)*document.unitScale());
        _dof->setMaxTranslate(CRCore::crVector3(rangeX.max,rangeY.max,rangeZ.max)*document.unitScale());
        _dof->setCurrentTranslate(CRCore::crVector3(rangeX.current,rangeY.current,rangeZ.current)*document.unitScale());
        _dof->setIncrementTranslate(CRCore::crVector3(rangeX.increment,rangeY.increment,rangeZ.increment)*document.unitScale());

        //rotations:
        _dof->setMinHPR(CRCore::crVector3(CRCore::inDegrees(rangeYaw.min),CRCore::inDegrees(rangePitch.min),CRCore::inDegrees(rangeRoll.min)));
        _dof->setMaxHPR(CRCore::crVector3(CRCore::inDegrees(rangeYaw.max),CRCore::inDegrees(rangePitch.max),CRCore::inDegrees(rangeRoll.max)));
        _dof->setCurrentHPR(CRCore::crVector3(CRCore::inDegrees(rangeYaw.current),CRCore::inDegrees(rangePitch.current),CRCore::inDegrees(rangeRoll.current)));
        _dof->setIncrementHPR(CRCore::crVector3(CRCore::inDegrees(rangeYaw.increment),CRCore::inDegrees(rangePitch.increment),CRCore::inDegrees(rangeRoll.increment)));

        //scales:
        _dof->setMinScale(CRCore::crVector3(rangeScaleX.min,rangeScaleY.min,rangeScaleZ.min));
        _dof->setMaxScale(CRCore::crVector3(rangeScaleX.max,rangeScaleY.max,rangeScaleZ.max));
        _dof->setCurrentScale(CRCore::crVector3(rangeScaleX.current,rangeScaleY.current,rangeScaleZ.current));
        _dof->setIncrementScale(CRCore::crVector3(rangeScaleX.increment,rangeScaleY.increment,rangeScaleZ.increment));

        // compute axis.
        CRCore::crVector3 xAxis = pointOnXAxis - localOrigin;
        CRCore::crVector3 xyPlaneVector = pointInXYPlane - localOrigin;
        CRCore::crVector3 zAxis = xAxis ^ xyPlaneVector;
        CRCore::crVector3 yAxis = zAxis ^ xAxis;

        // normalize
        float length_x = xAxis.length();
		xAxis.normalize();
        float length_y = yAxis.length();
		yAxis.normalize();
        float length_z = zAxis.length();
		zAxis.normalize();

        if ((length_x*length_y*length_z)==0.0f)
        {
            CRCore::notify(CRCore::NOTICE)<<"Warning: OpenFlight DegreeOfFreedom::readRecord() found erroneous axis definition:"<<std::endl;
            CRCore::notify(CRCore::NOTICE)<<"    localOrigin="<<localOrigin<<std::endl;
            CRCore::notify(CRCore::NOTICE)<<"    pointOnXAxis="<<pointOnXAxis<<std::endl;
            CRCore::notify(CRCore::NOTICE)<<"    pointInXYPlane="<<pointInXYPlane<<std::endl;

            xAxis.set(1.0f,0.0f,0.0f);
            yAxis.set(0.0f,1.0f,0.0f);
            zAxis.set(0.0f,0.0f,1.0f);
        }

        // scale origin
        CRCore::crVector3 origin = localOrigin * document.unitScale();

        // create putmatrix
        CRCore::crMatrix inv_putmat(xAxis.x(), xAxis.y(), xAxis.z(), 0.0,
                               yAxis.x(), yAxis.y(), yAxis.z(), 0.0,
                               zAxis.x(), zAxis.y(), zAxis.z(), 0.0,
                               origin.x(), origin.y(), origin.z(), 1.0);


        _dof->setInversePutMatrix(inv_putmat);
        _dof->setPutMatrix(CRCore::crMatrix::inverse(inv_putmat));

        _dof->setLimitationFlags(flags);
        _dof->setAnimationOn(document.getDefaultDOFAnimationState());
//      _dof->setHPRMultOrder(CRCore::crDOFTransform::HPR);

        // Add this implementation to parent implementation.
        if (_parent.valid())
            _parent->addChild(*_dof);
    }

    Range readRange(RecordInputStream& in) const
    {
        Range range;
        range.min = in.readFloat64();
        range.max = in.readFloat64();
        range.current = in.readFloat64();
        range.increment = in.readFloat64();

        // Extend valid range (See Creator help on DOF).
        if (range.current < range.min) range.min = range.current;
        if (range.current > range.max) range.max = range.current;

        const float64 epsilon = 1.0e-7;
        if (fabs(range.max-range.min) < epsilon)
            range.increment = 0;

        return range;
    }

    bool valid(const CRCore::crVector3d& v) const
    {
        const CRCore::crVector3d bad(-1.0e8,-1.0e8,-1.0e8);
        const float64 epsilon = 1.0e-7;

        return (fabs(v.x()-bad.x()) > epsilon) ||
               (fabs(v.y()-bad.y()) > epsilon) ||
               (fabs(v.z()-bad.z()) > epsilon);
    }
};

REGISTER_FLTRECORD(DegreeOfFreedom, DOF_OP)



/** LevelOfDetail - To recreate the LevelOfDetail record in OSG we have to create a crLod node with one crGroup node under it.
 *  OSG representation Children of the LevelOfDetail record will be added to
*/
class LevelOfDetail : public PrimaryRecord
{
    CRCore::ref_ptr<CRCore::crLod> _lod;
    CRCore::ref_ptr<CRCore::crGroup> _impChild0;

public:

    LevelOfDetail() {}

    META_Record(LevelOfDetail)

    META_setID(_lod)
    META_setComment(_lod)
    META_setMultitexture(_lod)
    META_addChild(_impChild0)
    META_dispose(_lod)

protected:

    virtual ~LevelOfDetail() {}
    virtual void readRecord(RecordInputStream& in, Document& document)
    {
        std::string id = in.readString(8);
        in.forward(4);
        float64 switchInDistance = in.readFloat64();
        float64 switchOutDistance = in.readFloat64();
        /*int16 specialEffectID1 =*/ in.readInt16();
        /*int16 specialEffectID2 =*/ in.readInt16();
        /*uint32 flags =*/ in.readUInt32();
        CRCore::crVector3d center = in.readVec3d();

        _lod = new CRCore::crLod;
        _lod->setName(id);
        _lod->setCenter(center*document.unitScale());

        _impChild0 = new CRCore::crGroup;
        _impChild0->setName("crLod child0");

        // Add child to lod.
        _lod->addChild(_impChild0.get(),
             (float)switchOutDistance * document.unitScale(),
             (float)switchInDistance * document.unitScale());

        // Add this implementation to parent implementation.
        if (_parent.valid())
            _parent->addChild(*_lod);
    }

};

REGISTER_FLTRECORD(LevelOfDetail, LOD_OP)



/** OldLevelOfDetail
  */
class OldLevelOfDetail : public PrimaryRecord
{
    CRCore::ref_ptr<CRCore::crLod> _lod;
    CRCore::ref_ptr<CRCore::crGroup> _impChild0;

public:

    OldLevelOfDetail() {}

    META_Record(OldLevelOfDetail)

    META_setID(_lod)
    META_setComment(_lod)
    META_setMultitexture(_lod)
    META_addChild(_impChild0)
    META_dispose(_lod)

protected:

    virtual ~OldLevelOfDetail() {}
    virtual void readRecord(RecordInputStream& in, Document& document)
    {
        std::string id = in.readString(8);
        uint32 switchInDistance = in.readUInt32();
        uint32 switchOutDistance = in.readUInt32();
        /*int16 specialEffectID1 =*/ in.readInt16();
        /*int16 specialEffectID2 =*/ in.readInt16();
        /*uint32 flags =*/ in.readUInt32();

        CRCore::crVector3 center;
        center.x() = (float)in.readInt32();
        center.y() = (float)in.readInt32();
        center.z() = (float)in.readInt32();

        _lod = new CRCore::crLod;
        _lod->setName(id);
        _lod->setCenter(center*document.unitScale());
        _lod->setRange(0, (float)switchOutDistance * document.unitScale(),
                          (float)switchInDistance * document.unitScale());

        // Add child to lod.
        _impChild0 = new CRCore::crGroup;
        _lod->addChild(_impChild0.get());

        // Add this implementation to parent implementation.
        if (_parent.valid())
            _parent->addChild(*_lod);
    }

};

REGISTER_FLTRECORD(OldLevelOfDetail, OLD_LOD_OP)



/** crSwitch
*/
class crSwitch : public PrimaryRecord
{
    uint32 _currentMask;
    uint32 _numberOfMasks;
    uint32 _wordsInMask;
    std::vector<uint32> _masks;

    CRCore::ref_ptr<CRCore::crMultiSwitch> _multiSwitch;

public:

    crSwitch() {}

    META_Record(crSwitch)

    META_setID(_multiSwitch)
    META_setComment(_multiSwitch)
    META_setMultitexture(_multiSwitch)
    META_dispose(_multiSwitch)

    virtual void addChild(CRCore::crNode& child)
    {
        if (_multiSwitch.valid())
        {
            unsigned int nChild = _multiSwitch->getNumChildren();
            for (unsigned int nMask=0; nMask<_numberOfMasks; ++nMask)
            {
                // test if this child is active in the current mask (itMask)
                unsigned int nMaskBit = nChild % 32;
                unsigned int nMaskWord = nMask * _wordsInMask + nChild / 32;
                _multiSwitch->setValue(nMask, nChild, (_masks[nMaskWord] & (uint32(1) << nMaskBit))!=0 );
            }

            _multiSwitch->addChild(&child);
        }
    }

    virtual void setMultiSwitchValueName(unsigned int switchSet, const std::string& name)
    {
        //if (_multiSwitch.valid())
        //{
        //    _multiSwitch->setValueName(switchSet, name);
        //}
    }

protected:

    virtual ~crSwitch() {}
    virtual void readRecord(RecordInputStream& in, Document& /*document*/)
    {
        std::string id = in.readString(8);
        in.forward(4);
        _currentMask = in.readUInt32();
        _numberOfMasks = in.readUInt32();
        _wordsInMask = in.readUInt32();

        _multiSwitch = new CRCore::crMultiSwitch;
        _multiSwitch->setName(id);

        /* Example:
        | word #0 || word #1 || word #2 |     <- Mask #0
        | word #0 || word #1 || word #2 |     <- Mask #1
        In this example numberOfMasks=2 and wordsInMask=3, currentMask can be 0 or 1.
        */

        for (unsigned int n=0; n<_numberOfMasks*_wordsInMask; n++)
        {
            uint32 maskWord = in.readUInt32();
            _masks.push_back(maskWord);
        }

        _multiSwitch->setActiveSwitchSet(_currentMask);

        // Add this implementation to parent implementation.
        if (_parent.valid())
            _parent->addChild(*_multiSwitch);
    }
};

REGISTER_FLTRECORD(crSwitch, SWITCH_OP)



/** ExternalReference
*/
class ExternalReference : public PrimaryRecord
{
    CRCore::ref_ptr<CRCore::crProxyNode> _external;

    // Parent pool override flags
    static const unsigned long COLOR_PALETTE_OVERRIDE        = 0x80000000u >> 0;
    static const unsigned long MATERIAL_PALETTE_OVERRIDE     = 0x80000000u >> 1;
    static const unsigned long TEXTURE_PALETTE_OVERRIDE      = 0x80000000u >> 2;
    static const unsigned long LINE_STYLE_PALETTE_OVERRIDE   = 0x80000000u >> 3;
    static const unsigned long SOUND_PALETTE_OVERRIDE        = 0x80000000u >> 4;
    static const unsigned long LIGHT_SOURCE_PALETTE_OVERRIDE = 0x80000000u >> 5;
    static const unsigned long LIGHT_POINT_PALETTE_OVERRIDE  = 0x80000000u >> 6;
    static const unsigned long SHADER_PALETTE_OVERRIDE       = 0x80000000u >> 7;

public:

    ExternalReference() {}

    META_Record(ExternalReference)

    META_setID(_external)
    META_setComment(_external)
    META_setMultitexture(_external)
    META_addChild(_external)
    META_dispose(_external)

protected:

    virtual ~ExternalReference() {}
    virtual void readRecord(RecordInputStream& in, Document& document)
    {
        std::string strFile = in.readString(200);

        _external = new CRCore::crProxyNode;
        _external->setCenterMode(CRCore::crProxyNode::USE_BOUNDING_SPHERE_CENTER);
        _external->setFileName(0,strFile);

        // Set parent pools as user data
        if (document.version() >= VERSION_14_2)
        {
            in.forward(4);

            uint32 mask = in.readUInt32(~0u);

            // Possible bug in models with version number 15.4.1 ?
            // Symptoms: Black trees in VegaPlayer town.
            if (document.version() == 1541)
                mask = ~0u;

            ParentPools* parentPools = new ParentPools;

            if ((mask & COLOR_PALETTE_OVERRIDE) == 0)
                parentPools->setColorPool(document.getColorPool());

            if ((mask & MATERIAL_PALETTE_OVERRIDE) == 0)
                parentPools->setMaterialPool(document.getMaterialPool());

            if ((mask & TEXTURE_PALETTE_OVERRIDE) == 0)
                parentPools->setTexturePool(document.getTexturePool());

            if ((document.version() >= VERSION_15_1) && ((mask & LIGHT_SOURCE_PALETTE_OVERRIDE) == 0))
                parentPools->setLightSourcePool(document.getLightSourcePool());

            if ((document.version() >= VERSION_15_8) && ((mask & LIGHT_POINT_PALETTE_OVERRIDE) == 0))
                parentPools->setLPAppearancePool(document.getLightPointAppearancePool());

            //if ((document.version() >= VERSION_16_0) && ((mask & SHADER_PALETTE_OVERRIDE) == 0))
            //    parentPools->setShaderPool(document.getShaderPool());

            //_external->setUserData(parentPools);
        }

        // Add this implementation to parent implementation.
        if (_parent.valid())
            _parent->addChild(*_external);
    }

};

REGISTER_FLTRECORD(ExternalReference, EXTERNAL_REFERENCE_OP)



/** InstanceDefinition
*/
class InstanceDefinition : public PrimaryRecord
{
    int _number;
    CRCore::ref_ptr<CRCore::crGroup> _instanceDefinition;

public:

    InstanceDefinition():_number(0) {}

    META_Record(InstanceDefinition)

    META_setID(_instanceDefinition)
    META_setComment(_instanceDefinition)
    META_setMultitexture(_instanceDefinition)
    META_addChild(_instanceDefinition)

protected:

    virtual ~InstanceDefinition() {}
    virtual void readRecord(RecordInputStream& in, Document& document)
    {
        in.forward(2);
        _number = (int)in.readUInt16();

        _instanceDefinition = new CRCore::crGroup;
    }

    virtual void dispose(Document& document)
    {
        // Insert transform(s)
        if (_matrix.valid())
        {
            CRCore::ref_ptr<CRCore::crMatrixTransform> transform = new CRCore::crMatrixTransform(*_matrix);
            transform->setDataVariance(CRCore::crBase::STATIC);
            transform->addChild(_instanceDefinition.get());
            _instanceDefinition = transform.get();
        }

        //  Add to instance definition table.
        document.setInstanceDefinition(_number,_instanceDefinition.get());
    }

};

REGISTER_FLTRECORD(InstanceDefinition, INSTANCE_DEFINITION_OP)



/** InstanceReference
  * The InstanceReference is a leaf record.
  */
class InstanceReference : public PrimaryRecord
{
public:

    InstanceReference() {}

    META_Record(InstanceReference)

protected:

    virtual ~InstanceReference() {}
    virtual void readRecord(RecordInputStream& in, Document& document)
    {
        in.forward(2);
        uint16 number = in.readUInt16();

        // Get definition.
        CRCore::crNode* instance = document.getInstanceDefinition(number);

        // Add this implementation to parent implementation.
        if (_parent.valid() && instance)
            _parent->addChild(*instance);
    }
};

REGISTER_FLTRECORD(InstanceReference, INSTANCE_REFERENCE_OP)



/** Extension
*/
class Extension : public PrimaryRecord
{
    CRCore::ref_ptr<CRCore::crGroup> _extension;

public:

    Extension() {}

    META_Record(Extension)

    META_setID(_extension)
    META_setComment(_extension)
    META_setMultitexture(_extension)
    META_addChild(_extension)
    META_dispose(_extension)

protected:

    virtual ~Extension() {}
    virtual void readRecord(RecordInputStream& in, Document& /*document*/)
    {
        std::string id = in.readString(8);
        std::string siteId = in.readString(8);
        in.forward(1);

        _extension = new CRCore::crGroup;
        _extension->setName(id);

        // Add this implementation to parent implementation.
        if (_parent.valid())
            _parent->addChild(*_extension);
    }
};

REGISTER_FLTRECORD(Extension, EXTENSION_OP)



/** crBase
*/
class crBase : public PrimaryRecord
{
    static const unsigned int HIDE_IN_DAYLIGHT = 0x80000000u >> 0;
    static const unsigned int HIDE_AT_DUSK     = 0x80000000u >> 1;
    static const unsigned int HIDE_AT_NIGHT    = 0x80000000u >> 2;
    static const unsigned int NO_ILLUMINATION  = 0x80000000u >> 3;
    static const unsigned int FLAT_SHADED      = 0x80000000u >> 4;
    static const unsigned int SHADOW_OBJECT    = 0x80000000u >> 5;

    CRCore::ref_ptr<CRCore::crGroup> _object;

public:

    crBase() {}

    META_Record(crBase)

    META_setID(_object)
    META_setComment(_object)
    META_addChild(_object)

protected:

    virtual void readRecord(RecordInputStream& in, Document& document)
    {
        std::string id = in.readString(8);

        _object = new CRCore::crGroup;
        _object->setName(id);

        if (document.getReadObjectRecordData())
        {
           // CRCore::ObjectRecordData* ord = new CRCore::ObjectRecordData;
           /* ord->_flags = */in.readUInt32();
           /* ord->_relativePriority = */in.readInt16();
           /* ord->_transparency = */in.readUInt16();
           /* ord->_effectID1 = */in.readInt16();
           /* ord->_effectID2 = */in.readInt16();
           /* ord->_significance = */in.readInt16();

           // _object->setUserData( ord );
        }
        else
        {
            /*uint32 flags =*/ in.readUInt32();
        }

        // Postpone add-to-parent until we know a bit more.
    }

    virtual void dispose(Document& document)
    {
        if (!_parent.valid() || !_object.valid()) return;

        // Is it safe to remove _object?
        if (!document.getPreserveObject() && isSafeToRemoveObject() && !_matrix.valid())
        {
            // Add children of _object to parent.
            // _object will not be added to graph.
            for (unsigned int i=0; i<_object->getNumChildren(); ++i)
            {
                _parent->addChild(*(_object->getChild(i)));
            }
        }
        else
        {
            _parent->addChild(*_object);
        }

        // Insert transform(s)
        if (_matrix.valid())
        {
            insertMatrixTransform(*_object,*_matrix,_numberOfReplications);
        }
    }

    bool isSafeToRemoveObject() const
    {
        // The following tests need a valid parent.
        if (_parent.valid())
        {
            // LODs adds an empty child group so it is safe to remove this object record.
            if (typeid(*_parent)==typeid(flt::LevelOfDetail))
                return true;

            if (typeid(*_parent)==typeid(flt::OldLevelOfDetail))
                return true;

            // If parent is a crGroup record we have to check for animation.
            crGroup* parentGroup = dynamic_cast<flt::crGroup*>(const_cast<PrimaryRecord*>(_parent.get()));
            if (parentGroup && !parentGroup->hasAnimation())
                return true;
        }

        return false;
    }

};

REGISTER_FLTRECORD(crBase, OBJECT_OP)



/** crLightSource
*/
class crLightSource : public PrimaryRecord
{
    static const unsigned int ENABLED = 0x80000000u >> 0;
    static const unsigned int GLOBAL  = 0x80000000u >> 1;
    static const unsigned int EXPORT  = 0x80000000u >> 3;

    CRCore::ref_ptr<CRCore::crLightSource> _lightSource;

public:

    crLightSource() {}

    META_Record(crLightSource)

    META_setID(_lightSource)
    META_setComment(_lightSource)
    META_dispose(_lightSource)

protected:

    virtual ~crLightSource() {}
    virtual void readRecord(RecordInputStream& in, Document& document)
    {
        std::string id = in.readString(8);
        in.forward(4);
        int32 index = in.readInt32();
        in.forward(4);
        uint32 flags = in.readUInt32();
        in.forward(4);
        CRCore::crVector3d pos = in.readVec3d();
        float32 yaw = in.readFloat32();
        float32 pitch = in.readFloat32();

        _lightSource = new CRCore::crLightSource;
        _lightSource->setName(id);

        LightSourcePool* pool = document.getOrCreateLightSourcePool();
        CRCore::crLight* lightFromPool = pool->get(index);
        if (lightFromPool)
        {
            // Make a clone of light in pool.
            CRCore::crLight* light = new CRCore::crLight(*lightFromPool);

            // TODO: Find a better way to set light number.
            light->setLightNum(1);

            // Position
            float w = lightFromPool->getPosition().w();
            if (w > 0.0) // positional light?
                light->setPosition(CRCore::crVector4(pos,w));

            // Apply yaw and pitch for infinite and spot light.
            if ((w==0.0) || (light->getSpotCutoff()<180.0))
            {
                // assume yaw is zero along y axis, increase positive clockwise
                // assume pitch is zero along xy plane, increase positive upwards
                float cos_yaw = cosf(CRCore::inDegrees(-yaw));
                float sin_yaw = sinf(CRCore::inDegrees(-yaw));
                float cos_pitch = cosf(CRCore::inDegrees(pitch));
                float sin_pitch = sinf(CRCore::inDegrees(pitch));
                light->setDirection(CRCore::crVector3(sin_yaw*cos_pitch, cos_yaw*cos_pitch, sin_pitch));
            }

            _lightSource->setLight(light);
            //_lightSource->setLocalStateSetModes((flags & ENABLED) ? CRCore::crStateAttribute::ON : CRCore::crStateAttribute::OFF);

            // Global light.
            if (flags & GLOBAL)
            {
                // Apply light source to header node.
                CRCore::crNode* header = document.getHeaderNode();
                //if (header)
                  //  _lightSource->setStateSetModes(*(header->getOrCreateStateSet()),CRCore::crStateAttribute::ON);
            }
        }

        if (_parent.valid())
            _parent->addChild(*_lightSource);
    }
};

REGISTER_FLTRECORD(crLightSource, LIGHT_SOURCE_OP)



} // end namespace















