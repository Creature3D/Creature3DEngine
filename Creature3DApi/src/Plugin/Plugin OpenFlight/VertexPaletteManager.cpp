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

#include <Plugin OpenFlight/VertexPaletteManager.h>
#include <Plugin OpenFlight/DataOutputStream.h>
#include <Plugin OpenFlight/Opcodes.h>
#include <Plugin OpenFlight/Utils.h>
#include <CRCore/crNotify.h>
#include <CRCore/crGeometry.h>
#include <map>


namespace flt
{


VertexPaletteManager::VertexPaletteManager( const ExportOptions& fltOpt )
  : _currentSizeBytes( 8 ),
    _current( NULL ),
    _vertices( NULL ),
    _fltOpt( fltOpt )
{
}

VertexPaletteManager::~VertexPaletteManager()
{
    if (!_verticesTempName.empty())
    {
        // Delete our temp file.
        if (_verticesStr.is_open())
        {
            CRCore::notify(CRCore::WARN) << "fltexp: VertexPaletteManager destructor has an open temp file." << std::endl;
            // This should not happen. FltExportVisitor::complete should close
            // this file before we get to this destructor.
            return;
        }
        CRCore::notify(CRCore::INFO) << "fltexp: Deleting temp file " << _verticesTempName << std::endl;
        FLTEXP_DELETEFILE( _verticesTempName.c_str() );
    }
}

void
VertexPaletteManager::add( const CRCore::crGeometry& geom )
{
    const CRCore::Array* v = geom.getVertexArray();
    if (!v)
    {
        CRCore::notify(CRCore::WARN) << "fltexp: Attempting to add NULL vertex array in VertexPaletteManager." << std::endl;
        return;
    }
    const CRCore::Array* c = geom.getColorArray();
    const CRCore::Array* n = geom.getNormalArray();
    const CRCore::Array* t = geom.getTexCoordArray( 0 );

    const unsigned int size = v->getNumElements();
    CRCore::ref_ptr< const CRCore::Vec3Array > v3 = asVec3dArray( v, size );
    CRCore::ref_ptr< const CRCore::Vec4Array > c4 = asVec4Array( c, size );
    CRCore::ref_ptr< const CRCore::Vec3Array > n3 = asVec3Array( n, size );
    CRCore::ref_ptr< const CRCore::Vec2Array > t2 = asVec2Array( t, size );
    if (v && !v3)
        return;
    if (c && !c4)
        return;
    if (n && !n3)
        return;
    if (t && !t2)
        return;

    const bool cpv =( geom.getColorBinding() == CRCore::crGeometry::BIND_PER_VERTEX );
    const bool npv =( geom.getNormalBinding() == CRCore::crGeometry::BIND_PER_VERTEX );
    add( v, v3.get(), c4.get(), n3.get(), t2.get(), cpv, npv );
}
void
VertexPaletteManager::add( const CRCore::Array* key,
    const CRCore::Vec3Array* v, const CRCore::Vec4Array* c,
    const CRCore::Vec3Array* n, const CRCore::Vec2Array* t,
    bool colorPerVertex, bool normalPerVertex, bool allowSharing )
{
    bool needsInit( true );

    if (allowSharing)
    {
        ArrayMap::iterator it = _arrayMap.find( key );
        if (it != _arrayMap.end())
            needsInit = false;
        _current = &( _arrayMap[ key ] );
    }
    else
    {
        _current = &( _nonShared );
    }

    if (needsInit)
    {
        _current->_byteStart = _currentSizeBytes;

        _current->_idxCount = v->size();

        _current->_idxSizeBytes = recordSize( recordType( v, c, n, t ) );
        _currentSizeBytes += ( _current->_idxSizeBytes * _current->_idxCount );

        // Next we'll write the vertex palette record data. But,
        //   if we don't have a DataOutputStream yet, open the temp file.
        if (!_vertices)
        {
            _verticesTempName = _fltOpt.getTempDir() + "/ofw_temp_vertices";
            _verticesStr.open( _verticesTempName.c_str(), std::ios::out | std::ios::binary );
            _vertices = new DataOutputStream( _verticesStr.rdbuf(), _fltOpt.getValidateOnly() );
        }
        writeRecords( v, c, n, t, colorPerVertex, normalPerVertex );
    }
}

unsigned int
VertexPaletteManager::byteOffset( unsigned int idx ) const
{
    if (!_current)
    {
        CRCore::notify(CRCore::WARN) << "fltexp: No current vertex array in VertexPaletteManager." << std::endl;
        return 4;
    }
    if (idx >= _current->_idxCount)
    {
        CRCore::notify(CRCore::WARN) << "fltexp: Index out of range in VertexPaletteManager." << std::endl;
        return 4;
    }

    return( _current->_byteStart + (_current->_idxSizeBytes * idx) );
}

void
VertexPaletteManager::write( DataOutputStream& dos ) const
{
    if (_currentSizeBytes == 8)
        // Empty palette. Don't write anything.
        return;

    dos.writeInt16( (int16) VERTEX_PALETTE_OP );
    dos.writeUInt16( 8 );
    dos.writeInt32( _currentSizeBytes );

    // Close the temp file. We're done writing new data to it.
    _verticesStr.close();

    // Open that temp file again, this time for reading.
    //   Then copy to dos.
    char buf;
    std::ifstream vertIn;
    vertIn.open( _verticesTempName.c_str(), std::ios::in | std::ios::binary );
    while (!vertIn.eof() )
    {
        vertIn.read( &buf, 1 );
        if (vertIn.good())
            dos << buf;
    }
    vertIn.close();
}



VertexPaletteManager::PaletteRecordType
VertexPaletteManager::recordType( const CRCore::Array* v, const CRCore::Array* c,
    const CRCore::Array* n, const CRCore::Array* t )
{
    if (t)
    {
        // crTexture coordinates
        if (n)
            return VERTEX_CNT;
        else
            return VERTEX_CT;
    }
    else
    {
        // No texture coordinates
        if (n)
            return VERTEX_CN;
        else
            return VERTEX_C;
    }
}

unsigned int
VertexPaletteManager::recordSize( PaletteRecordType recType )
{
    switch (recType)
    {
    case VERTEX_C:
        return 40;
        break;
    case VERTEX_CN:
        return (_fltOpt.getFlightFileVersionNumber() > ExportOptions::VERSION_15_7) ? 56 : 52;
        break;
    case VERTEX_CT:
        return 48;
        break;
    case VERTEX_CNT:
        return 64;
        break;
    default:
        return 0;
    }
}

void
VertexPaletteManager::writeRecords( const CRCore::Vec3Array* v, const CRCore::Vec4Array* c,
    const CRCore::Vec3Array* n, const CRCore::Vec2Array* t,
    bool colorPerVertex, bool normalPerVertex )
{
    const PaletteRecordType recType = recordType( v, c, n, t );
    const int16 sizeBytes = recordSize( recType );

    int16 opcode = 0;
    switch( recType )
    {
    case VERTEX_C:
        opcode = VERTEX_C_OP;
        break;
    case VERTEX_CN:
        opcode = VERTEX_CN_OP;
        if (!n)
            CRCore::notify(CRCore::WARN) << "fltexp: VPM::writeRecords: no normal array." << std::endl;
        break;
    case VERTEX_CNT:
        opcode = VERTEX_CNT_OP;
        if (!n)
            CRCore::notify(CRCore::WARN) << "fltexp: VPM::writeRecords: no normal array." << std::endl;
        if (!t)
            CRCore::notify(CRCore::WARN) << "fltexp: VPM::writeRecords: no tex coord array." << std::endl;
        break;
    case VERTEX_CT:
        opcode = VERTEX_CT_OP;
        if (!t)
            CRCore::notify(CRCore::WARN) << "fltexp: VPM::writeRecords: no tex coord array." << std::endl;
        break;
    }

    enum FlagBits
    {
        START_HARD_EDGE = (0x8000 >> 0),
        NORMAL_FROZEN   = (0x8000 >> 1),
        NO_COLOR        = (0x8000 >> 2),
        PACKED_COLOR    = (0x8000 >> 3)
    };
    uint32 flags( NO_COLOR );
    if (colorPerVertex)
        flags = PACKED_COLOR;


    size_t idx;
    for( idx=0; idx<v->size(); idx++)
    {
        uint32  packedColor( 0 );
        if (c && colorPerVertex)
        {
            CRCore::crVector4 color = (*c)[ idx ];
            packedColor = (int)(color[3]*255) << 24 |
                (int)(color[2]*255) << 16 | (int)(color[1]*255) << 8 |
                (int)(color[0]*255);
        }

        // Write fields common to all record types.
        _vertices->writeInt16( opcode );
        _vertices->writeUInt16( sizeBytes );
        _vertices->writeUInt16( 0 ); // Color name
        _vertices->writeInt16( flags ); // Flags
        _vertices->writeVec3d( (*v)[ idx ] ); // Vertex

        // Now write record-specific fields.
        switch( recType )
        {
        case VERTEX_C:
        {
            _vertices->writeInt32( packedColor ); // Packed color
            _vertices->writeUInt32( 0 ); // Vertex color index
            break;
        }
        case VERTEX_CN:
        {
            if (!normalPerVertex) // Normal
                _vertices->writeVec3f( (*n)[ 0 ] );
            else
                _vertices->writeVec3f( (*n)[ idx ] );
            _vertices->writeInt32( packedColor ); // Packed color
            _vertices->writeUInt32( 0 ); // Vertex color index
            if (_fltOpt.getFlightFileVersionNumber() > ExportOptions::VERSION_15_7)
                _vertices->writeUInt32( 0 ); // Reserved
            break;
        }
        case VERTEX_CNT:
        {
            if (!normalPerVertex) // Normal
                _vertices->writeVec3f( (*n)[ 0 ] );
            else
                _vertices->writeVec3f( (*n)[ idx ] );
            _vertices->writeVec2f( (*t)[ idx ] ); // Tex coord
            _vertices->writeInt32( packedColor ); // Packed color
            _vertices->writeUInt32( 0 ); // Vertex color index
            _vertices->writeUInt32( 0 ); // Reserved
            break;
        }
        case VERTEX_CT:
        {
            _vertices->writeVec2f( (*t)[ idx ] ); // Tex coord
            _vertices->writeInt32( packedColor ); // Packed color
            _vertices->writeUInt32( 0 ); // Vertex color index
            break;
        }
        }
    }
}



CRCore::ref_ptr< const CRCore::Vec2Array >
VertexPaletteManager::asVec2Array( const CRCore::Array* in, const unsigned int n )
{
    if (!in)
        return NULL;

    CRCore::Array::Type arrayType = in->getType();
    if (arrayType == CRCore::Array::Vec2ArrayType)
    {
        if (n <= in->getNumElements())
        {
            CRCore::ref_ptr< const CRCore::Vec2Array > v2f =
                dynamic_cast< const CRCore::Vec2Array* >( in );
            return v2f;
        }
    }

    const unsigned int nToCopy = ( (n < in->getNumElements()) ? n : in->getNumElements() );
    CRCore::ref_ptr< CRCore::Vec2Array > ret = new CRCore::Vec2Array( n );

    switch( arrayType )
    {
    case CRCore::Array::Vec2ArrayType:
    {
        // No need to convert data, but must copy into correctly-sized array.
        // If the size was correct, we wouldn't be here.
        CRCore::ref_ptr< const CRCore::Vec2Array > v2f =
            dynamic_cast< const CRCore::Vec2Array* >( in );
        ret->assign( v2f->begin(), v2f->end() );;
        ret->resize( n );
        return ret.get();
    }
    //case CRCore::Array::Vec2dArrayType:
    //{
    //    CRCore::ref_ptr< const CRCore::Vec2dArray > v2d =
    //        dynamic_cast< const CRCore::Vec2dArray* >( in );
    //    unsigned int idx;
    //    for (idx=0; idx<nToCopy; idx++ )
    //        (*ret)[ idx ] = (*v2d)[ idx ]; // convert crVector2 double to crVector2 float
    //    return ret.get();
    //}
    default:
    {
        CRCore::notify(CRCore::WARN) << "fltexp: Unsupported array type in conversion to Vec2Array: " << arrayType << std::endl;
        return NULL;
    }
    }
}
CRCore::ref_ptr< const CRCore::Vec3Array >
VertexPaletteManager::asVec3Array( const CRCore::Array* in, const unsigned int n )
{
    if (!in)
        return NULL;

    CRCore::Array::Type arrayType = in->getType();
    if (arrayType == CRCore::Array::Vec3ArrayType)
    {
        if (n <= in->getNumElements())
        {
            CRCore::ref_ptr< const CRCore::Vec3Array > v3f =
                dynamic_cast< const CRCore::Vec3Array* >( in );
            return v3f;
        }
    }

    const unsigned int nToCopy = ( (n < in->getNumElements()) ? n : in->getNumElements() );
    CRCore::ref_ptr< CRCore::Vec3Array > ret = new CRCore::Vec3Array( n );

    switch( arrayType )
    {
    case CRCore::Array::Vec3ArrayType:
    {
        // No need to convert data, but must copy into correctly-sized array.
        // If the size was correct, we wouldn't be here.
        CRCore::ref_ptr< const CRCore::Vec3Array > v3f =
            dynamic_cast< const CRCore::Vec3Array* >( in );
        ret->assign( v3f->begin(), v3f->end() );;
        ret->resize( n );
        return ret.get();
    }
    //case CRCore::Array::Vec3dArrayType:
    //{
    //    CRCore::ref_ptr< const CRCore::Vec3Array > v3d =
    //        dynamic_cast< const CRCore::Vec3Array* >( in );
    //    unsigned int idx;
    //    for (idx=0; idx<nToCopy; idx++ )
    //        (*ret)[ idx ] = (*v3d)[ idx ]; // convert crVector3 double to crVector3 float
    //    return ret.get();
    //}
    default:
    {
        CRCore::notify(CRCore::WARN) << "fltexp: Unsupported array type in conversion to Vec3Array: " << arrayType << std::endl;
        return NULL;
    }
    }
}
CRCore::ref_ptr< const CRCore::Vec3Array >
VertexPaletteManager::asVec3dArray( const CRCore::Array* in, const unsigned int n )
{
    if (!in)
        return NULL;

    CRCore::Array::Type arrayType = in->getType();
    //if (arrayType == CRCore::Array::Vec3dArrayType)
    //{
    //    if (n <= in->getNumElements())
    //    {
    //        CRCore::ref_ptr< const CRCore::Vec3Array > v3d =
    //            dynamic_cast< const CRCore::Vec3Array* >( in );
    //        return v3d;
    //    }
    //}

    const unsigned int nToCopy = ( (n < in->getNumElements()) ? n : in->getNumElements() );
    CRCore::ref_ptr< CRCore::Vec3Array > ret = new CRCore::Vec3Array( n );

    switch( arrayType )
    {
    //case CRCore::Array::Vec3dArrayType:
    //{
    //    // No need to convert data, but must copy into correctly-sized array.
    //    // If the size was correct, we wouldn't be here.
    //    CRCore::ref_ptr< const CRCore::Vec3Array > v3d =
    //        dynamic_cast< const CRCore::Vec3Array* >( in );
    //    ret->assign( v3d->begin(), v3d->end() );;
    //    ret->resize( n );
    //    return ret.get();
    //}
    case CRCore::Array::Vec3ArrayType:
    {
        CRCore::ref_ptr< const CRCore::Vec3Array > v3f =
            dynamic_cast< const CRCore::Vec3Array* >( in );
        unsigned int idx;
        for (idx=0; idx<nToCopy; idx++ )
            (*ret)[ idx ] = (*v3f)[ idx ]; // convert crVector3 float to crVector3 double
        return ret.get();
    }
    default:
    {
        CRCore::notify(CRCore::WARN) << "fltexp: Unsupported array type in conversion to Vec3Array: " << arrayType << std::endl;
        return NULL;
    }
    }
}
CRCore::ref_ptr< const CRCore::Vec4Array >
VertexPaletteManager::asVec4Array( const CRCore::Array* in, const unsigned int n )
{
    if (!in)
        return NULL;

    CRCore::Array::Type arrayType = in->getType();
    if (arrayType == CRCore::Array::Vec4ArrayType)
    {
        if (n <= in->getNumElements())
        {
            CRCore::ref_ptr< const CRCore::Vec4Array > v4f =
                dynamic_cast< const CRCore::Vec4Array* >( in );
            return v4f;
        }
    }

    const unsigned int nToCopy = ( (n < in->getNumElements()) ? n : in->getNumElements() );
    CRCore::ref_ptr< CRCore::Vec4Array > ret = new CRCore::Vec4Array( n );

    switch( arrayType )
    {
    case CRCore::Array::Vec4ArrayType:
    {
        // No need to convert data, but must copy into correctly-sized array.
        // If the size was correct, we wouldn't be here.
        CRCore::ref_ptr< const CRCore::Vec4Array > v4f =
            dynamic_cast< const CRCore::Vec4Array* >( in );
        ret->assign( v4f->begin(), v4f->end() );;
        ret->resize( n );
        return ret.get();
    }
    case CRCore::Array::Vec4ubArrayType:
    {
        CRCore::ref_ptr< const CRCore::Vec4ubArray > v4ub =
            dynamic_cast< const CRCore::Vec4ubArray* >( in );
        unsigned int idx;
        for (idx=0; idx<nToCopy; idx++ )
        {
            // convert crVector4 unsigned byte to crVector4 float
            CRCore::crVector4& dest = (*ret)[ idx ];
            CRCore::crVector4ub src = (*v4ub)[ idx ];
            dest[0] = src[0] / 255.f;
            dest[1] = src[1] / 255.f;
            dest[2] = src[2] / 255.f;
            dest[3] = src[3] / 255.f;
        }
        return ret.get();
    }
    default:
    {
        CRCore::notify(CRCore::WARN) << "fltexp: Unsupported array type in conversion to Vec4Array: " << arrayType << std::endl;
        return NULL;
    }
    }
}



VertexPaletteManager::ArrayInfo::ArrayInfo()
  : _byteStart( 0 ),
    _idxSizeBytes( 0 ),
    _idxCount( 0 )
{
}


}
