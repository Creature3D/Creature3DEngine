/*
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or (at
 * your option) any later version. The full license is in the LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * OpenSceneGraph Public License for more details.
*/

//
// Copyright(c) 2008 Skew crMatrix Software LLC.
//

#ifndef __FLTEXP_FLT_EXPORT_VISITOR_H__
#define __FLTEXP_FLT_EXPORT_VISITOR_H__ 1

#include <CRCore/crNodeVisitor.h>
#include <CRCore/crPrimitive.h>
#include <Plugin OpenFlight/ExportOptions.h>
#include <Plugin OpenFlight/Types.h>
#include <fstream>
#include <set>
#include <memory>

namespace CRCore {
    class DrawArrays;
    class DrawArrayLengths;
    class DrawElements;
    class crGeometry;
    class crStateSet;
    class crSwitch;
    class crMaterial;
    class crTexture2D;
	class crDOFTransform;
	class crMultiSwitch;
}
namespace CRCore {
    class LightPointNode;
    class ObjectRecordData;
}

namespace flt
{


class ExportOptions;
class DataOutputStream;
class MaterialPaletteManager;
class TexturePaletteManager;
class VertexPaletteManager;
class LightSourcePaletteManager;



/*!
   The main crNodeVisitor for walking the scene graph during export.
   A collection of apply() methods is in FltExportVisitor.cpp.
   The apply() methods call several other methods for writing
   specific FLT record types, most of which are in exp*.cpp.
 */
class FltExportVisitor : public CRCore::crNodeVisitor
{
public:
    FltExportVisitor( DataOutputStream* dos, ExportOptions* fltOpt );
    ~FltExportVisitor(  );

    bool complete( const CRCore::crNode& node );

    virtual void apply( CRCore::crGroup& node );
    virtual void apply( CRCore::crSequence& node );
    virtual void apply( CRCore::crSwitch& node );
    virtual void apply( CRCore::crLod& node );
    virtual void apply( CRCore::crMatrixTransform& node );
    virtual void apply( CRCore::crPositionAttitudeTransform& node );
    virtual void apply( CRCore::crTransform& node );
    virtual void apply( CRCore::crLightSource& node );
    virtual void apply( CRCore::crObject& node );
    virtual void apply( CRCore::crNode& node );
    virtual void apply( CRCore::crProxyNode& node );



    // Primary records
    void writeHeader( const std::string& headerName );
    void writeGroup( const CRCore::crGroup& node );
    void writeGroup( const CRCore::crGroup& group,
                     int32 flags,
                     int32 loopCount,
                     float32 loopDuration,
                     float32 lastFrameDuration);
    void writeSequence( const CRCore::crSequence& node );
    void writeObject( const CRCore::crGroup& node/*, CRCore::ObjectRecordData* ord */);
    void writeDegreeOfFreedom( const CRCore::crDOFTransform* dof );
    void writeExternalReference( const CRCore::crProxyNode& node );
    void writeLevelOfDetail( const CRCore::crLod& lod, const CRCore::crVector3d& center,
                             double switchInDist, double switchOutDist);
    void writeLightSource( const CRCore::crLightSource& ls );
    void writeSwitch( const CRCore::crMultiSwitch* ms );
    void writeSwitch( const CRCore::crSwitch* ms );
    void writeLightPoint( const CRCore::LightPointNode* lpn );

    // Ancillary records
    void writeComment( const CRCore::crNode& node, DataOutputStream* dos=NULL );
    void writeLongID( const std::string& id, DataOutputStream* dos=NULL );
    void writeMatrix( const CRCore::Referenced* ref );
    void writeContinuationRecord( const unsigned short length );

    // Control records
    void writePush();
    void writePop();
    void writePushSubface();
    void writePopSubface();

    // Helper routine for traversing a pushed subtree
    void writePushTraverseWritePop(CRCore::crNode& node)
    {
        writePush();
        traverse(node);
        writePop();
    }

    void writePushTraverseChildWritePop(CRCore::crNode& node)
    {
        writePush();
        node.accept(*this);
        writePop();
    }

    // crGeometry records
    void writeFace( const CRCore::crObject& geode, const CRCore::crGeometry& geom, GLenum mode );
    void writeMesh( const CRCore::crObject& geode, const CRCore::crGeometry& geom );
    int writeVertexList( int first, unsigned int count );
    int writeVertexList( const std::vector<unsigned int>& indices, unsigned int count );
    void writeMeshPrimitive( const std::vector<unsigned int>& indices, GLenum mode );
    void writeLocalVertexPool( const CRCore::crGeometry& geom );
    void writeMultitexture( const CRCore::crGeometry& geom );
    void writeUVList( int numVerts, const CRCore::crGeometry& geom, const std::vector<unsigned int>& indices );
    void writeUVList( int numVerts, const CRCore::crGeometry& geom, unsigned int first=0);

    // crLight Point records
    void writeLightPoint();

    // Exporter doesn't currently support color palette; write a dummy in its place to
    // support loaders that require it.
    void writeColorPalette();

    // crStateSet stack support
    void pushStateSet( const CRCore::crStateSet* rhs );
    void popStateSet();
    const CRCore::crStateSet* getCurrentStateSet() const;
    void clearStateSetStack();

    // Write a .attr file if none exists in the data file path.
    void writeATTRFile( int unit, const CRCore::crTexture2D* texture ) const;


private:
    // Methods for handling different primitive set types.
    //   These are defined in expGeometryRecords.cpp.
    void handleDrawArrays( const CRCore::DrawArrays* da, const CRCore::crGeometry& geom, const CRCore::crObject& geode );
    void handleDrawArrayLengths( const CRCore::DrawArrayLengths* dal, const CRCore::crGeometry& geom, const CRCore::crObject& geode );
    void handleDrawElementsUByte( const CRCore::DrawElementsUByte* de, const CRCore::crGeometry& geom, const CRCore::crObject& geode );
	void handleDrawElementsUShort( const CRCore::DrawElementsUShort* de, const CRCore::crGeometry& geom, const CRCore::crObject& geode );
	void handleDrawElementsUInt( const CRCore::DrawElementsUInt* de, const CRCore::crGeometry& geom, const CRCore::crObject& geode );

    bool isLit( const CRCore::crGeometry& geom ) const;
    bool isTextured( int unit, const CRCore::crGeometry& geom ) const;
    bool isMesh( const GLenum mode ) const;
    bool atLeastOneFace( const CRCore::crGeometry& geom ) const;
    bool atLeastOneMesh( const CRCore::crGeometry& geom ) const;

    CRCore::ref_ptr< ExportOptions > _fltOpt;

    // _dos is the primary output stream, produces the actual .flt file.
    DataOutputStream& _dos;

    // _records is a temp file for most records. After the Header and palette
    // records are written to _dos, _records is copied onto _dos.
    std::ofstream _recordsStr;
    DataOutputStream* _records;
    std::string _recordsTempName;

    // Track state changes during a scene graph walk.
    typedef std::vector< CRCore::ref_ptr<CRCore::crStateSet> > StateSetStack;
    StateSetStack _stateSetStack;

    std::auto_ptr<MaterialPaletteManager>     _materialPalette;
    std::auto_ptr<TexturePaletteManager>      _texturePalette;
    std::auto_ptr<LightSourcePaletteManager>  _lightSourcePalette;
    std::auto_ptr<VertexPaletteManager>       _vertexPalette;

    // Used to avoid duplicate Header/crGroup records at top of output FLT file.
    bool _firstNode;
};

/*!
   Helper class to ensure symmetrical state push/pop behavior.
 */
class ScopedStatePushPop
{
    public:
        ScopedStatePushPop ( FltExportVisitor * fnv, const CRCore::crStateSet *ss ) :
            fnv_( fnv )
        {
            fnv_->pushStateSet( ss );
        }
        virtual ~ScopedStatePushPop ()
        {
            fnv_->popStateSet();
        }

    private:
        FltExportVisitor * fnv_;
};


/*!
   Automatically handles writing the LongID ancillary record.
 */
struct IdHelper
{
    IdHelper(flt::FltExportVisitor& v, const std::string& id)
      : v_(v), id_(id), dos_(NULL)
    { }

    // Write an ancillary ID record upon destruction if name is too long
    ~IdHelper()
    {
        if (id_.length() > 8)
            v_.writeLongID(id_,dos_);
    }

    // Allow implicit conversion to the abbreviated name string
    operator const std::string() const
    {
        return( (id_.length() > 8) ? id_.substr(0, 8) : id_ );
    }

    flt::FltExportVisitor&  v_;
    const std::string        id_;
    DataOutputStream* dos_;

protected:

    IdHelper& operator = (const IdHelper&) { return *this; }

};


/*!
   Supports wrapping subfaces with push/pop records.
 */
struct SubfaceHelper
{
    SubfaceHelper(flt::FltExportVisitor& v, const CRCore::crStateSet* ss )
      : v_(v)
    {
        _polygonOffsetOn = ( ss->getMode( GL_POLYGON_OFFSET_FILL ) == CRCore::crStateAttribute::ON );
        if (_polygonOffsetOn)
            v_.writePushSubface();
    }

    ~SubfaceHelper()
    {
        if (_polygonOffsetOn)
            v_.writePopSubface();
    }

    flt::FltExportVisitor&  v_;
    bool _polygonOffsetOn;

protected:

    SubfaceHelper& operator = (const SubfaceHelper&) { return *this; }
};

}

#endif
