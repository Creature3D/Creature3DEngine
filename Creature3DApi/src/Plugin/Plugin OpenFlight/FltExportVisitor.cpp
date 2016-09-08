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

#include <Plugin OpenFlight/FltExportVisitor.h>
#include <Plugin OpenFlight/ExportOptions.h>
#include <Plugin OpenFlight/FltWriteResult.h>
#include <Plugin OpenFlight/DataOutputStream.h>
#include <Plugin OpenFlight/Opcodes.h>
#include <Plugin OpenFlight/LightSourcePaletteManager.h>
#include <Plugin OpenFlight/MaterialPaletteManager.h>
#include <Plugin OpenFlight/TexturePaletteManager.h>
#include <Plugin OpenFlight/VertexPaletteManager.h>
#include <Plugin OpenFlight/AttrData.h>
#include <Plugin OpenFlight/Utils.h>

#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crWriteFile.h>
#include <CRCore/crObject.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crLightSource.h>
#include <CRCore/crLod.h>
#include <CRCore/crMatrixTransform.h>
#include <CRCore/crPositionAttitudeTransform.h>
#include <CRCore/crPrimitive.h>
#include <CRCore/crProxyNode.h>
#include <CRCore/crQuat.h>
#include <CRCore/crSequence.h>
#include <CRCore/crTexture2D.h>
#include <CRCore/crTexEnv.h>
#include <CRCore/crSwitch.h>
#include <CRCore/crMaterial.h>
#include <CRCore/crCullFace.h>
#include <CRCore/crBlendFunc.h>
#include <CRCore/crPolygonOffset.h>
#include <CRCore/crDOFTransform.h>
#include <CRCore/crMultiSwitch.h>
//#include <CRCore/LightPointNode.h>
//#include <CRCore/ObjectRecordData>

#ifdef _MSC_VER
// Disable this warning. It's OK for us to use 'this' in initializer list,
// as the texturePaletteManager merely stores a ref to it.
#pragma warning( disable : 4355 )
#endif

namespace flt
{


FltExportVisitor::FltExportVisitor( DataOutputStream* dos,
                                ExportOptions* fltOpt )

  : CRCore::crNodeVisitor( CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN ),
    _fltOpt( fltOpt ),
    _dos( *dos ),
    _materialPalette( new MaterialPaletteManager( *fltOpt ) ),
    _texturePalette( new TexturePaletteManager( *this, *fltOpt ) ),
    _lightSourcePalette( new LightSourcePaletteManager( *fltOpt ) ),
    _vertexPalette( new VertexPaletteManager( *fltOpt ) ),
    _firstNode( true )
{
    // Init the crStateSet stack.
    CRCore::crStateSet* ss = new CRCore::crStateSet;

    int unit;
    for(unit=0; unit<8; unit++)
    {
        CRCore::crTexEnv* texenv = new CRCore::crTexEnv;
        ss->setTextureAttributeAndModes( unit, texenv, CRCore::crStateAttribute::OFF );
        // TBD other texture state?
    }

    CRCore::crMaterial* material = new CRCore::crMaterial;
    ss->setAttribute( material, CRCore::crStateAttribute::OFF );
    if (fltOpt->getLightingDefault())
        ss->setMode( GL_LIGHTING, CRCore::crStateAttribute::ON );
    else
        ss->setMode( GL_LIGHTING, CRCore::crStateAttribute::OFF );

    CRCore::crCullFace* cf = new CRCore::crCullFace;
    ss->setAttributeAndModes( cf, CRCore::crStateAttribute::OFF );

    CRCore::crBlendFunc* bf = new CRCore::crBlendFunc;
    ss->setAttributeAndModes( bf, CRCore::crStateAttribute::OFF );

    CRCore::crPolygonOffset* po = new CRCore::crPolygonOffset;
    ss->setAttributeAndModes( po, CRCore::crStateAttribute::OFF );

    _stateSetStack.push_back( ss );


    // Temp file for storing records. Need a temp file because we don't
    // write header and palette until FltExportVisitor completes traversal.
    _recordsTempName = fltOpt->getTempDir() + "/ofw_temp_records";
    _recordsStr.open( _recordsTempName.c_str(), std::ios::out | std::ios::binary );
    _records = new DataOutputStream( _recordsStr.rdbuf(), fltOpt->getValidateOnly() );

    // Always write initial push level
    writePush();
}

FltExportVisitor::~FltExportVisitor()
{
    // Delete our temp file.
    if (_recordsStr.is_open())
    {
        CRCore::notify(CRCore::WARN) << "fltexp: FltExportVisitor destructor has an open temp file." << std::endl;
        // This should not happen. FltExportVisitor::complete should close
        // this file before we get to this destructor.
        return;
    }
    CRCore::notify(CRCore::INFO) << "fltexp: Deleting temp file " << _recordsTempName << std::endl;
    FLTEXP_DELETEFILE( _recordsTempName.c_str() );
}


void
FltExportVisitor::apply( CRCore::crGroup& node )
{
    ScopedStatePushPop guard( this, node.getStateSet() );

    if (_firstNode)
    {
        // On input, a FLT header creates a crGroup node.
        // On export, we always write a Header record, but then the first crNode
        //   we export is the crGroup that was created from the original input Header.
        // On successive roundtrips, this results in increased redundant top-level crGroup nodes/records.
        // Avoid this by NOT outputting anything for a top-level crGroup node.
        _firstNode = false;
        traverse( node );
        return;
    }

    // A crGroup node could indicate one of many possible records.
    //   Header record -- Don't need to support this here. We always output a header.
    //   crGroup record -- HIGH
    //   Child of an crLod node -- HIGH Currently write out a crGroup record regardless.
    //   InstanceDefinition/InstanceReference -- MED --  multiparented crGroup is an instance
    //   Extension record -- MED
    //   crBase record -- MED
    //   LightPointSystem record (if psgSim::crMultiSwitch) -- LOW

    CRCore::crMultiSwitch* multiSwitch = dynamic_cast<CRCore::crMultiSwitch*>( &node );
    if (multiSwitch)
    {
        writeSwitch( multiSwitch );
    }

    else
    {
        //CRCore::ObjectRecordData* ord =
        //    dynamic_cast< CRCore::ObjectRecordData* >( node.getUserData() );
        if (dynamic_cast<CRCore::crObject*>( &node ))
        {
            // This crGroup should write an crBase Record.
            writeObject( node/*, ord*/ );
        }
        else
        {
            // Handle other cases here.
            // For now, just output a crGroup record.
            writeGroup( node );
        }
    }
	CRCore::crMatrix mat;
    writeMatrix( &mat/*node.getUserData()*/ );
    writeComment( node );
    writePushTraverseWritePop( node );
}

void
FltExportVisitor::apply( CRCore::crSequence& node )
{
    _firstNode = false;
    ScopedStatePushPop guard( this, node.getStateSet() );

    writeSequence( node );
	CRCore::crMatrix mat;
	writeMatrix( &mat/*node.getUserData()*/ );
    writeComment( node );
    writePushTraverseWritePop( node );
}

void
FltExportVisitor::apply( CRCore::crSwitch& node )
{
    _firstNode = false;
    ScopedStatePushPop guard( this, node.getStateSet() );

    writeSwitch( &node );

	CRCore::crMatrix mat;
	writeMatrix( &mat/*node.getUserData()*/ );
    writeComment( node );
    writePushTraverseWritePop( node );
}

void
FltExportVisitor::apply( CRCore::crLod& lodNode )
{
    _firstNode = false;
    ScopedStatePushPop guard( this, lodNode.getStateSet() );

    // crLod center - same for all children
    CRCore::crVector3d center = lodNode.getCenter();

    // Iterate children of the crLod and write a separate crLod record for each,
    // with that child's individual switchIn and switchOut properties
    for ( size_t i = 0; i < lodNode.getNumChildren(); ++i )
    {
        CRCore::crNode* lodChild = lodNode.getChild(i);

        // crSwitch-in/switch-out distances may vary per child
        double switchInDist = lodNode.getMaxRange(i);
        double switchOutDist = lodNode.getMinRange(i);

        writeLevelOfDetail( lodNode, center, switchInDist, switchOutDist);
		CRCore::crMatrix mat;
		writeMatrix( &mat/*node.getUserData()*/ );
        writeComment( lodNode );

        // Traverse each child of the crLod
        writePushTraverseChildWritePop( *lodChild );
    }

}

void
FltExportVisitor::apply( CRCore::crMatrixTransform& node )
{
    // Importer reads a crMatrix record and inserts a crMatrixTransform above
    //   the current node. We need to do the opposite: Write a crMatrix record
    //   as an ancillary record for each child. We do that by storing the
    //   crMatrixTransform in each child's UserData. Each child then checks
    //   UserData and writes a crMatrix record if UserData is a crMatrixTransform.

    _firstNode = false;
    ScopedStatePushPop guard( this, node.getStateSet() );

    CRCore::ref_ptr< CRCore::RefMatrix > m = new CRCore::RefMatrix;
    m->set( node.getMatrix() );
    //if (node.getUserData())
    //{
    //    const CRCore::RefMatrix* rm = dynamic_cast<const CRCore::RefMatrix*>( node.getUserData() );
    //    if (rm)
    //        (*m) *= *rm;
    //}

    typedef std::vector< CRCore::ref_ptr< CRCore::Referenced > > UserDataList;

    UserDataList saveUserDataList( node.getNumChildren() );

    //unsigned int idx;
    //for( idx=0; idx<node.getNumChildren(); ++idx )
    //{
    //    saveUserDataList[ idx ] = node.getChild( idx )->getUserData();
    //    node.getChild( idx )->setUserData( m.get() );
    //}

    traverse( (CRCore::crNode&)node );

    //// Restore saved UserData.
    //for( idx=0; idx< node.getNumChildren(); ++idx )
    //{
    //  node.getChild( idx )->setUserData( saveUserDataList[ idx ].get() );
    //}
}

void
FltExportVisitor::apply( CRCore::crPositionAttitudeTransform& node )
{
    _firstNode = false;
    ScopedStatePushPop guard( this, node.getStateSet() );

    CRCore::ref_ptr<CRCore::RefMatrix> m = new CRCore::RefMatrix(
        CRCore::crMatrix::translate( -node.getPivotPoint() ) *
        CRCore::crMatrix::scale( node.getScale() ) *
        CRCore::crMatrix::rotate( node.getAttitude() ) *
        CRCore::crMatrix::translate( node.getPosition() ) );

    typedef std::vector< CRCore::ref_ptr< CRCore::Referenced > > UserDataList;
    UserDataList saveUserDataList( node.getNumChildren() );

    //unsigned int idx;
    //for( idx=0; idx<node.getNumChildren(); ++idx )
    //{
    //    saveUserDataList[ idx ] = node.getChild( idx )->getUserData();
    //    node.getChild( idx )->setUserData( m.get() );
    //}

    traverse( (CRCore::crNode&)node );

    //// Restore saved UserData.
    //for( idx=0; idx<node.getNumChildren(); ++idx )
    //{
    //    node.getChild( idx )->setUserData( saveUserDataList[ idx ].get() );
    //}

}


void
FltExportVisitor::apply( CRCore::crTransform& node )
{
    _firstNode = false;
    ScopedStatePushPop guard( this, node.getStateSet() );

    CRCore::crDOFTransform* dof = dynamic_cast<CRCore::crDOFTransform*>( &node );

    if (dof)
    {
        writeDegreeOfFreedom( dof);
    }
	CRCore::crMatrix mat;
    writeMatrix( &mat );
    writeComment( node );
    writePushTraverseWritePop( node );
}

void
FltExportVisitor::apply( CRCore::crLightSource& node )
{
    _firstNode = false;
    ScopedStatePushPop guard( this, node.getStateSet() );

    writeLightSource( node );
	CRCore::crMatrix mat;
	writeMatrix( &mat );
    writeComment( node );
    writePushTraverseWritePop( node );
}

// Billboards also go through this code. The crObject is passed
// to writeFace and writeMesh. If those methods successfully cast
// the crObject to a crBillboard, then they set the template mode
// bit accordingly.
void
FltExportVisitor::apply( CRCore::crObject& node )
{
    _firstNode = false;
    ScopedStatePushPop guard( this, node.getStateSet() );

    unsigned int idx;
    for (idx=0; idx<node.getNumDrawables(); idx++)
    {
        CRCore::crGeometry* geom = node.getDrawable( idx )->asGeometry();
        if (!geom)
        {
            std::string warning( "fltexp: Non-crGeometry Drawable encountered. Ignoring." );
            CRCore::notify(CRCore::WARN) << warning << std::endl;
            _fltOpt->getWriteResult().warn( warning );
            continue;
        }

        ScopedStatePushPop drawableGuard( this, geom->getStateSet() );

        // Push and pop subfaces if polygon offset is on.
        SubfaceHelper subface( *this, getCurrentStateSet() );

        if (atLeastOneFace( *geom ))
        {
            // If at least one record will be a Face record, then we
            //   need to write to the vertex palette.
            _vertexPalette->add( *geom );

            // Iterate over all PrimitiveSets and output Face records.
            unsigned int jdx;
            for (jdx=0; jdx < geom->getNumPrimitives(); jdx++)
            {
                CRCore::crPrimitive* prim = geom->getPrimitive( jdx );
                if ( isMesh( prim->getMode() ) )
                    continue;

                if (prim->getType() == CRCore::crPrimitive::DrawArraysPrimitiveType)
                    handleDrawArrays( dynamic_cast<CRCore::DrawArrays*>( prim ), *geom, node );
                else if (prim->getType() == CRCore::crPrimitive::DrawArrayLengthsPrimitiveType)
                    handleDrawArrayLengths( dynamic_cast<CRCore::DrawArrayLengths*>( prim ), *geom, node );
				else if(prim->getType() == CRCore::crPrimitive::DrawElementsUBytePrimitiveType)
					handleDrawElementsUByte( dynamic_cast<CRCore::DrawElementsUByte*>( prim ), *geom, node );
				else if(prim->getType() == CRCore::crPrimitive::DrawElementsUShortPrimitiveType)
					handleDrawElementsUShort( dynamic_cast<CRCore::DrawElementsUShort*>( prim ), *geom, node );
				else if(prim->getType() == CRCore::crPrimitive::DrawElementsUIntPrimitiveType)
					handleDrawElementsUInt( dynamic_cast<CRCore::DrawElementsUInt*>( prim ), *geom, node );
                //else if ( (prim->getType() == CRCore::crPrimitive::DrawElementsUBytePrimitiveType) ||
                //        (prim->getType() == CRCore::crPrimitive::DrawElementsUShortPrimitiveType) ||
                //        (prim->getType() == CRCore::crPrimitive::DrawElementsUIntPrimitiveType) )
                //    handleDrawElements( dynamic_cast<CRCore::DrawElements*>( prim ), *geom, node );
                else
                {
                    std::string warning( "fltexp: Unknown crPrimitive type." );
                    CRCore::notify(CRCore::WARN) << warning << std::endl;
                    _fltOpt->getWriteResult().warn( warning );
                    return;
                }
            }
        }

        if (atLeastOneMesh( *geom ))
        {
            // If at least one Mesh record, write out preamble mesh records
            //   followed by a Mesh Primitive record per crPrimitive.
            writeMesh( node, *geom );

			CRCore::crMatrix mat;
			writeMatrix( &mat );
            writeComment( node );
            writeMultitexture( *geom );
            writeLocalVertexPool( *geom );

            writePush();

            unsigned int jdx;
            for (jdx=0; jdx < geom->getNumPrimitives(); jdx++)
            {
                CRCore::crPrimitive* prim = geom->getPrimitive( jdx );
                if ( !isMesh( prim->getMode() ) )
                    continue;

                if (prim->getType() == CRCore::crPrimitive::DrawArraysPrimitiveType)
                    handleDrawArrays( dynamic_cast<CRCore::DrawArrays*>( prim ), *geom, node );
                else if (prim->getType() == CRCore::crPrimitive::DrawArrayLengthsPrimitiveType)
                    handleDrawArrayLengths( dynamic_cast<CRCore::DrawArrayLengths*>( prim ), *geom, node );
				else if(prim->getType() == CRCore::crPrimitive::DrawElementsUBytePrimitiveType)
					handleDrawElementsUByte( dynamic_cast<CRCore::DrawElementsUByte*>( prim ), *geom, node );
				else if(prim->getType() == CRCore::crPrimitive::DrawElementsUShortPrimitiveType)
					handleDrawElementsUShort( dynamic_cast<CRCore::DrawElementsUShort*>( prim ), *geom, node );
				else if(prim->getType() == CRCore::crPrimitive::DrawElementsUIntPrimitiveType)
					handleDrawElementsUInt( dynamic_cast<CRCore::DrawElementsUInt*>( prim ), *geom, node );
                else
                {
                    std::string warning( "fltexp: Unknown crPrimitive type." );
                    CRCore::notify(CRCore::WARN) << warning << std::endl;
                    _fltOpt->getWriteResult().warn( warning );
                    return;
                }
            }

            writePop();
        }
    }

    // Would traverse here if this node could have children.
    //   traverse( (CRCore::crNode&)node );
}

void
FltExportVisitor::apply( CRCore::crNode& node )
{
    _firstNode = false;
    ScopedStatePushPop guard( this, node.getStateSet() );

    //CRCore::LightPointNode* lpn = dynamic_cast< CRCore::LightPointNode* >( &node );
    //if (lpn)
    //{
    //    writeLightPoint( lpn );
    //}
    //else
    {
        // Unknown crNode. Warn and return.
        // (Note, if the base class of this crNode was a crGroup, then apply(crGroup&)
        //   would export a crGroup record then continue traversal. Because we are
        //   a crNode, there's no way to continue traversal, so just return.)
        std::string warning( "fltexp: Unknown crNode in OpenFlight export." );
        CRCore::notify(CRCore::WARN) << warning << std::endl;
        _fltOpt->getWriteResult().warn( warning );
        return;
    }
}

void
FltExportVisitor::apply( CRCore::crProxyNode& node )
{
    //_firstNode = false;
    //ScopedStatePushPop guard( this, node.getStateSet() );

    //writeExternalReference( node );
    //writeMatrix( node.getUserData() );
    //writeComment( node );
	apply((CRCore::crGroup&)node);
}





bool
FltExportVisitor::complete( const CRCore::crNode& node )
{
    // Always write final pop level
    writePop();
    // Done writing records, close the record data temp file.
    _recordsStr.close();

    // Write OpenFlight file front matter: header, vertex palette, etc.
    writeHeader( node.getName() );

    writeColorPalette();
    _materialPalette->write( _dos );
    _texturePalette->write( _dos );
    _lightSourcePalette->write( _dos );
    _vertexPalette->write( _dos );

    // Write Comment ancillary record and specify the _dos DataOutputStream.
    writeComment( node, &_dos );

    // Copy record data temp file into final OpenFlight file.
    // Yee-uck. TBD need better stream copy routine.
    char buf;
    std::ifstream recIn;
    recIn.open( _recordsTempName.c_str(), std::ios::in | std::ios::binary );
    while (!recIn.eof() )
    {
        recIn.read( &buf, 1 );
        if (recIn.good())
            _dos << buf;
    }
    recIn.close();

    return true;
}



//
// crStateSet stack support

void
FltExportVisitor::pushStateSet( const CRCore::crStateSet* rhs )
{
    CRCore::crStateSet* ss = new CRCore::crStateSet( *( _stateSetStack.back().get() ) );

    if (rhs)
        ss->merge( *rhs );

    _stateSetStack.push_back( ss );
}

void
FltExportVisitor::popStateSet()
{
    _stateSetStack.pop_back();
}

const CRCore::crStateSet*
FltExportVisitor::getCurrentStateSet() const
{
    return _stateSetStack.back().get();
}

void
FltExportVisitor::clearStateSetStack()
{
    _stateSetStack.clear();
}


void
FltExportVisitor::writeATTRFile( int unit, const CRCore::crTexture2D* texture ) const
{
    std::string name;
    if (_fltOpt->getStripTextureFilePath())
        name = CRIOManager::getSimpleFileName( texture->getImage()->getFileName() );
    else
        name = texture->getImage()->getFileName();
    name += std::string( ".attr" );

    if ( CRIOManager::findDataFile( name ).empty() )
    {
        // No .attr file found. We should write one out.
        // Fill AttrData fields from current state.
        AttrData ad;

        ad.texels_u = texture->getImage()->s();
        ad.texels_v = texture->getImage()->t();

        switch( texture->getFilter( CRCore::crTexture::MIN_FILTER ) )
        {
        case CRCore::crTexture::LINEAR:
            ad.minFilterMode = AttrData::MIN_FILTER_BILINEAR;
            break;
        case CRCore::crTexture::LINEAR_MIPMAP_LINEAR:
            ad.minFilterMode = AttrData::MIN_FILTER_MIPMAP_TRILINEAR;
            break;
        case CRCore::crTexture::LINEAR_MIPMAP_NEAREST:
            ad.minFilterMode = AttrData::MIN_FILTER_MIPMAP_BILINEAR;
            break;
        case CRCore::crTexture::NEAREST:
            ad.minFilterMode = AttrData::MIN_FILTER_POINT;
            break;
        case CRCore::crTexture::NEAREST_MIPMAP_LINEAR:
            ad.minFilterMode = AttrData::MIN_FILTER_MIPMAP_LINEAR;
            break;
        case CRCore::crTexture::NEAREST_MIPMAP_NEAREST:
            ad.minFilterMode = AttrData::MIN_FILTER_MIPMAP_POINT;
            break;
        default:
            ad.minFilterMode = AttrData::MIN_FILTER_MIPMAP_TRILINEAR;
            break;
        }
        switch( texture->getFilter( CRCore::crTexture::MAG_FILTER ) )
        {
        case CRCore::crTexture::NEAREST:
            ad.magFilterMode = AttrData::MAG_FILTER_POINT;
            break;
        default:
            ad.magFilterMode = AttrData::MAG_FILTER_BILINEAR;
            break;
        }

        // Repeat and Clamp
        switch( texture->getWrap( CRCore::crTexture::WRAP_S ) )
        {
        case CRCore::crTexture::CLAMP:
        case CRCore::crTexture::CLAMP_TO_EDGE:
        case CRCore::crTexture::CLAMP_TO_BORDER:
            ad.wrapMode_u = AttrData::WRAP_CLAMP;
            break;
        case CRCore::crTexture::REPEAT:
        default:
            ad.wrapMode_u = AttrData::WRAP_REPEAT;
            break;
        case CRCore::crTexture::MIRROR:
            if (_fltOpt->getFlightFileVersionNumber() >= 1610)
                ad.wrapMode_u = AttrData::WRAP_MIRRORED_REPEAT;
            else
                ad.wrapMode_u = AttrData::WRAP_REPEAT;
            break;
        }
        switch( texture->getWrap( CRCore::crTexture::WRAP_T ) )
        {
        case CRCore::crTexture::CLAMP:
        case CRCore::crTexture::CLAMP_TO_EDGE:
        case CRCore::crTexture::CLAMP_TO_BORDER:
            ad.wrapMode_v = AttrData::WRAP_CLAMP;
            break;
        case CRCore::crTexture::REPEAT:
        default:
            ad.wrapMode_v = AttrData::WRAP_REPEAT;
            break;
        case CRCore::crTexture::MIRROR:
            if (_fltOpt->getFlightFileVersionNumber() >= 1610)
                ad.wrapMode_v = AttrData::WRAP_MIRRORED_REPEAT;
            else
                ad.wrapMode_v = AttrData::WRAP_REPEAT;
            break;
        }

        const CRCore::crStateSet* ss = getCurrentStateSet();
        const CRCore::crTexEnv* texenv = dynamic_cast<const CRCore::crTexEnv*>(
            ss->getTextureAttribute( unit, CRCore::crStateAttribute::TEXENV ) );
        if (texenv)
        {
            switch( texenv->getMode())
            {
            case CRCore::crTexEnv::DECAL:
                ad.texEnvMode = AttrData::TEXENV_DECAL;
                break;
            case CRCore::crTexEnv::MODULATE:
            default:
                ad.texEnvMode = AttrData::TEXENV_MODULATE;
                break;
            case CRCore::crTexEnv::BLEND:
                ad.texEnvMode = AttrData::TEXENV_BLEND;
                break;
            case CRCore::crTexEnv::REPLACE:
                ad.texEnvMode = AttrData::TEXENV_COLOR;
                break;
            case CRCore::crTexEnv::ADD:
                ad.texEnvMode = AttrData::TEXENV_ADD;
                break;
            }
        }

        CRIOManager::writeObjectFile( ad, name/*, _fltOpt.get() */);
    }
}


}

