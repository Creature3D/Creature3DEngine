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

#ifndef __FLTEXP_VERTEX_PALETTE_MANAGER_H__
#define __FLTEXP_VERTEX_PALETTE_MANAGER_H__ 1


#include <Plugin OpenFlight/DataOutputStream.h>
#include <Plugin OpenFlight/ExportOptions.h>
#include <CRCore/crArray.h>
//#include <CRIOManager/fstream>
#include <map>

namespace CRCore {
    class crGeometry;
}


namespace flt
{


/*!
   Manages writing the Vertex Palette record during export.
   Maintains a map to ensure that instanced VertexArray data is only
   written once to the palette. Writes the palette record to a temp
   file and copies it to FltExportVisitor::_dos after the scene graph
   has been completely walked.
 */
class VertexPaletteManager
{
public:
    VertexPaletteManager( const ExportOptions& fltOpt );
    ~VertexPaletteManager();

    void add( const CRCore::crGeometry& geom );
    void add( const CRCore::Array* key,
        const CRCore::Vec3Array* v, const CRCore::Vec4Array* c,
        const CRCore::Vec3Array* n, const CRCore::Vec2Array* t,
        bool colorPerVertex, bool normalPerVertex, bool allowSharing=true );

    unsigned int byteOffset( unsigned int idx ) const;

    void write( DataOutputStream& dos ) const;

    /*!
       Static utility routines for handling the morass of array
       types that could be found in a crGeometry object's vertex/
       normal/texcoord/color data. */
    static CRCore::ref_ptr< const CRCore::Vec2Array > asVec2Array( const CRCore::Array* in, const unsigned int n );
    static CRCore::ref_ptr< const CRCore::Vec3Array > asVec3Array( const CRCore::Array* in, const unsigned int n );
    static CRCore::ref_ptr< const CRCore::Vec3Array > asVec3dArray( const CRCore::Array* in, const unsigned int n );
    static CRCore::ref_ptr< const CRCore::Vec4Array > asVec4Array( const CRCore::Array* in, const unsigned int n );

protected:
    typedef enum {
        VERTEX_C,
        VERTEX_CN,
        VERTEX_CNT,
        VERTEX_CT
    } PaletteRecordType;

    static PaletteRecordType recordType( const CRCore::Array* v, const CRCore::Array* c,
        const CRCore::Array* n, const CRCore::Array* t );
    unsigned int recordSize( PaletteRecordType recType );

    void writeRecords( const CRCore::Vec3Array* v, const CRCore::Vec4Array* c,
        const CRCore::Vec3Array* n, const CRCore::Vec2Array* t,
        bool colorPerVertex, bool normalPerVertex );

    unsigned int _currentSizeBytes;

    struct ArrayInfo {
        ArrayInfo();

        unsigned int _byteStart;
        unsigned int _idxSizeBytes;
        unsigned int _idxCount;
    };
    ArrayInfo* _current;
    ArrayInfo _nonShared;

    typedef std::map< const CRCore::Array*, ArrayInfo > ArrayMap;
    ArrayMap _arrayMap;

    mutable std::ofstream _verticesStr;
    DataOutputStream* _vertices;
    std::string _verticesTempName;

    const ExportOptions& _fltOpt;
};


}

#endif
