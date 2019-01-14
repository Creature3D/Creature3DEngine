// Modify ConvertFromFLT::setTexture to create a new CRCore::TexEnvCombiner in texture stateset to handle 
// detail texture
// Julian Ortiz, June 18th 2003.

#include <stdio.h>
#include <string.h>
#include <crgl/gl.h>

#include <CRCore/crBrain.h>
//#include <CRCore/crResource.h>
#include <CRCore/crGroup.h>
#include <CRCore/crClearNode.h>
#include <CRCore/crLod.h>
#include <CRCore/crMatrixTransform.h>
#include <CRCore/crOccluderNode.h>
#include <CRCore/crConvexPlanarOccluder.h>
#include <CRCore/crConvexPlanarPolygon.h>
#include <CRCore/crObject.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crCullFace.h>
#include <CRCore/crTexEnv.h>
#include <CRCore/crTexEnvCombine.h>
#include <CRCore/crTexGen.h>
//#include <CRCore/crVertexProgram.h>
//#include <CRCore/crFragmentProgram.h>
//#include <CRCore/crViewMatrixExtractor.h>
#include <CRCore/crAlphaFunc.h>
#include <CRCore/crBlendFunc.h>
#include <CRCore/crPoint.h>
#include <CRCore/crMaterial.h>
#include <CRCore/crPolygonOffset.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crBillboard.h>
//#include <CRCore/crDepthSortedObject.h>
#include <CRCore/crTexture2D.h>
#include <CRCore/crTextureCubeMap.h>
#include <CRCore/crLightSource.h>
#include <CRCore/crImage.h>
#include <CRCore/crNotify.h>
#include <CRCore/crSequence.h>
#include <CRCore/crShapeDrawable.h>
#include <CRCore/crQuat.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crAnimationPath.h>
#include <CRCore/crEventGroup.h>
#include <CRCore/crDepth.h>
#include <CRCore/crProxyNode.h>
#include <CRCore/crPolygonMode.h>

#include <CRPhysics/crMatterGroup.h>
#include <CRPhysics/crMatterObject.h>
#include <CRPhysics/crWeaponNode.h>
#include <CRPhysics/crSetForceAndTorquePPC.h>
#include <CRPhysics/crJointSetParamPPC.h>
#include <CRPhysics/crSetGravityModePPC.h>
#include <CRPhysics/crPhysicsResetPPC.h>
#include <CRPhysics/crSetGeomorBodyInfoPPC.h>
#include <CRPhysics/crGeneralParamTransferPPC.h>
#include <CRPhysics/crWeaponEventPPC.h>
#include <CRPhysics/crCarMatterObject.h>

//#include <CRAI/crNerveCellGroup.h>
//#include <CRAI/crCarManipulatorPPC.h>///////////////////////
////#include <CRAI/crRobotManipulatorPPC.h>///////////////////////
//#include <CRAI/crCharacterManipulatorPPC.h>///////////////////////

#include <CRCore/crMultiSwitch.h>
#include <CRCore/crDOFTransform.h>
//#include <CRSim/crLightPointNode.h>
//#include <CRSim/crLightPointSystem.h>
//#include <CRSim/crSector.h>
//#include <CRSim/crBlinkSequence.h>

#include <CRUtil/crHighlightMapGenerator.h>

#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crFileNameUtils.h>
#include <CRIOManager/crReadFile.h>
#include <CRIOManager/crRegistry.h>

//#include <CRFX/crSpecularHighlights.h>
//#include <CRFX/crCupMapping.h>
//#include <CRFX/crAnisotropicLighting.h>
//#include <CRFX/crBumpMapping.h>
//#include <CRFX/crCartoon.h>
//#include <CRFX/crScribe.h>

#include <Plugin flt/opcodes.h>
#include <Plugin flt/flt.h>
#include <Plugin flt/flt2osg.h>
#include <Plugin flt/FltFile.h>
#include <Plugin flt/Registry.h>
#include <Plugin flt/Record.h>
#include <Plugin flt/HeaderRecord.h>
#include <Plugin flt/ColorPaletteRecord.h>
#include <Plugin flt/MaterialPaletteRecord.h>
#include <Plugin flt/OldMaterialPaletteRecord.h>
#include <Plugin flt/TexturePaletteRecord.h>
#include <Plugin flt/LightPointPaletteRecords.h>
#include <Plugin flt/VertexPoolRecords.h>
#include <Plugin flt/OldVertexRecords.h>
#include <Plugin flt/LodRecord.h>
#include <Plugin flt/DofRecord.h>
#include <Plugin flt/SwitchRecord.h>
#include <Plugin flt/ObjectRecord.h>
#include <Plugin flt/GroupRecord.h>
#include <Plugin flt/FaceRecord.h>
#include <Plugin flt/MeshRecord.h>
#include <Plugin flt/MeshPrimitiveRecord.h>
#include <Plugin flt/TransformationRecords.h>
#include <Plugin flt/ExternalRecord.h>
#include <Plugin flt/LightPointRecord.h>
#include <Plugin flt/LightPointSystemRecord.h>
#include <Plugin flt/Input.h>
#include <Plugin flt/GeoSetBuilder.h>
#include <Plugin flt/LongIDRecord.h>
#include <Plugin flt/CommentRecord.h>
#include <Plugin flt/InstanceRecords.h>
#include <Plugin flt/LocalVertexPoolRecord.h>
#include <Plugin flt/MultiTextureRecord.h>
#include <Plugin flt/UVListRecord.h>
#include <Plugin flt/LightSourceRecord.h>
#include <Plugin flt/LightSourcePaletteRecord.h>
#include <Plugin flt/AttrData.h>
#include <Plugin flt/BSPRecord.h>
#include <sstream>
static int dprint = 0 ;
#define DPRINT if(dprint)fprintf
//#define CUBEMAP_FILENAME(cupmap,face) cupmap + "_" #face ".jpg"

using namespace flt;

typedef std::map<std::string,CRCore::ref_ptr<CRCore::crImage> > ImageMap;
typedef std::map<std::string,CRCore::ref_ptr<CRCore::crTextureCubeMap> > CupMap;
static ImageMap s_imageMap;
static CupMap s_cupMap;

CRCore::crImage *get_loadImage(std::string fileName)
{
	ImageMap::iterator itr = s_imageMap.find(fileName);

	if(itr!=s_imageMap.end()) 
		return itr->second.get();

	CRCore::crImage *image = CRIOManager::readImageFile(fileName);
	s_imageMap[fileName] = image;

	return image;
}

unsigned int mystrnlen(char *s, unsigned int maxLen)
{
	for (unsigned int i = 0; i < maxLen; i++)
	{
		if (!s[i]) return i;
	}
	return maxLen;
}

ConvertFromFLT::ConvertFromFLT() :
m_faceColor(1,1,1,1)
{
	m_diOpenFlightVersion = 0;
	m_diCurrentOffset = 0;
	m_wObjTransparency = 0;
	m_nSubfaceLevel = 0;
	m_unitScale = 1.0;
	m_useTextureAlphaForTranspancyBinning = true;
	m_bHdrRgbMode = false;
	m_currentLocalVertexPool = 0;
	m_doUnitsConversion = true;
}


ConvertFromFLT::~ConvertFromFLT()
{
}


CRCore::crGroup* ConvertFromFLT::convert(HeaderRecord* rec)
{
	if (rec==NULL) return NULL;
	return visitHeader(rec);
}


////////////////////////////////////////////////////////////////////

Record* ConvertFromFLT::getVertexFromPool(int nOffset)
{
	VertexPaletteOffsetMap::iterator fitr = m_VertexPaletteOffsetMap.find(nOffset);
	if (fitr != m_VertexPaletteOffsetMap.end())
	{
		return (*fitr).second;
	}
	else return NULL;
}


void ConvertFromFLT::regisiterVertex(int nOffset, Record* pRec)
{
	m_VertexPaletteOffsetMap[nOffset] = pRec;
}


////////////////////////////////////////////////////////////////////


CRCore::crGroup* ConvertFromFLT::visitInstanceDefinition(CRCore::crGroup& outParent,InstanceDefinitionRecord* rec)
{
	CRCore::crGroup* group = new CRCore::crGroup;
	InstancePool* pInstancePool = rec->getFltFile()->getInstancePool();

	visitAncillary(outParent, *group, rec);

	pInstancePool->addInstance((int)rec->getData()->iInstDefNumber,group);
	visitPrimaryNode(*group, (PrimNodeRecord*)rec);

	return group;
}

CRCore::crGroup* ConvertFromFLT::visitInstanceReference(CRCore::crGroup& outParent,InstanceReferenceRecord* rec)
{
	CRCore::crGroup* group;
	InstancePool* pInstancePool = rec->getFltFile()->getInstancePool();

	group = pInstancePool->getInstance((int)rec->getData()->iInstDefNumber);
	if (group)
		outParent.addChild( group );
	//else
	//	CRCore::notify(CRCore::INFO) << "Warning: cannot find the instance definition in flt file."<<std::endl;
	return group;
}


CRCore::crGroup* ConvertFromFLT::visitAncillary(CRCore::crGroup& outParent, CRCore::crGroup& osgPrimary, PrimNodeRecord* rec)
{
	// Note: There are databases that contains nodes with both crMatrix and GeneralMatrix
	// ancillary records. We need just one of these to put into the scenegraph
	// Nick.
	bool mxFound  = false;

	CRCore::crGroup* parent = &outParent;
	// Visit ancillary records
	for(int i=0; i < rec->getNumChildren(); i++)
	{
		Record* child = rec->getChild(i);
		if (!child->isAncillaryRecord())
			break;

		switch (child->getOpcode())
		{
		case LONG_ID_OP:
			visitLongID(osgPrimary, (LongIDRecord*)child);
			break;

		case GENERAL_MATRIX_OP:
			// Note: Ancillary record creates CRCore node
			if (!mxFound)
			{
				parent = visitGeneralMatrix(*parent, osgPrimary, (GeneralMatrixRecord*)child);
				mxFound = true;
			}
			break;

		case MATRIX_OP:
			// Note: Ancillary record creates CRCore node
			if (!mxFound)
			{
				parent = visitMatrix(*parent, osgPrimary, (MatrixRecord*)child);
				mxFound = true;
			}
			break;

		case COMMENT_OP:
			visitComment(osgPrimary, (CommentRecord*)child);
			break;

		case COLOR_PALETTE_OP:
			visitColorPalette(osgPrimary, (ColorPaletteRecord*)child);
			break;

		case LIGHT_SOURCE_PALETTE_OP:
			visitLightSourcePalette(osgPrimary, (LightSourcePaletteRecord*)child);
			break;

		case MATERIAL_PALETTE_OP:
			visitMaterialPalette(osgPrimary, (MaterialPaletteRecord*)child);
			break;

		case OLD_MATERIAL_PALETTE_OP:
			visitOldMaterialPalette(osgPrimary, (OldMaterialPaletteRecord*)child);
			break;

		case TEXTURE_PALETTE_OP:
			visitTexturePalette(osgPrimary, (TexturePaletteRecord*)child);
			break;

		case LIGHT_PT_APPEARANCE_PALETTE_OP:
			visitLtPtAppearancePalette(osgPrimary, (LtPtAppearancePaletteRecord*)child);
			break;

		case LIGHT_PT_ANIMATION_PALETTE_OP:
			visitLtPtAnimationPalette(osgPrimary, (LtPtAnimationPaletteRecord*)child);
			break;

		case VERTEX_PALETTE_OP:
			visitVertexPalette(osgPrimary, (VertexPaletteRecord*)child);
			break;

		case VERTEX_C_OP:
			visitVertex(osgPrimary, (VertexRecord*)child);
			break;

		case VERTEX_CN_OP:
			visitNormalVertex(osgPrimary, (NormalVertexRecord*)child);
			break;

		case VERTEX_CNT_OP:
			visitNormalTextureVertex(osgPrimary, (NormalTextureVertexRecord*)child);
			break;

		case VERTEX_CT_OP:
			visitTextureVertex(osgPrimary, (TextureVertexRecord*)child);
			break;

		default:

#ifdef _DEBUG

			CRCore::notify( CRCore::INFO ) << "flt::ConvertFromFLT::visitAncillary: "
				<< "Unknown opcode: " << child->getOpcode() << "\n";

#endif

			break;
		}
	}
	return parent;
}


CRCore::crGroup* ConvertFromFLT::visitPrimaryNode(CRCore::crGroup& outParent, PrimNodeRecord* rec)
{
	CRCore::crGroup* outPrim = NULL;
	GeoSetBuilder   geoSetBuilder;
	GeoSetBuilder   axisRoteBillboardBuilder;
    GeoSetBuilder   pointRoteBillboardBuilder;
	// Visit
	bool bBlend = false;
	for(int i=0; i < rec->getNumChildren(); i++)
	{
		Record* child = rec->getChild(i);

		if (child && child->isPrimaryNode())
		{
			DPRINT(stderr, "**************************************\nvisitPrimaryNode: Got child opcode %d\n", child->getOpcode()) ;

			switch (child->getOpcode())
			{
			case MESH_OP:

				if( ((MeshRecord*)child)->getData()->swTemplateTrans == 2 )  //Axis type rotate
					visitMesh(outParent, &axisRoteBillboardBuilder, (MeshRecord*)child,true);
				else if( ((MeshRecord*)child)->getData()->swTemplateTrans == 4 ) //Point rotate
					visitMesh(outParent, &pointRoteBillboardBuilder, (MeshRecord*)child,true);
				else
					bBlend = visitMesh(outParent, &geoSetBuilder, (MeshRecord*)child);
				break;

			case FACE_OP:
				{
					FaceRecord* fr = (FaceRecord*)child;
					if( fr->getData()->swTemplateTrans == 2 )  //Axis type rotate
						visitFace(&axisRoteBillboardBuilder, outParent, fr, true);
					else if( fr->getData()->swTemplateTrans == 4 )  //Point type rotate
						visitFace(&pointRoteBillboardBuilder, outParent, fr, true);
					else
						bBlend = visitFace(&geoSetBuilder, outParent, fr);
				}
				break;
			case LIGHT_POINT_OP:
				visitLightPoint(outParent, (LightPointRecord*)child);
				break;
			case INDEXED_LIGHT_PT_OP:
				visitLightPointIndex(outParent, (LightPointIndexRecord*)child);
				break;
			case LIGHT_PT_SYSTEM_OP:
				outPrim = visitLightPointSystem(outParent, (LightPointSystemRecord*)child);
				break;
			case GROUP_OP:
				outPrim = visitGroup(outParent, (GroupRecord*)child);
				break;
			case LIGHT_SOURCE_OP:
				outPrim = visitLightSource(outParent, (LightSourceRecord*)child);
				break;
			case LOD_OP:
				outPrim = visitLOD(outParent, (LodRecord*)child);
				break;
			case OLD_LOD_OP:
				outPrim = visitOldLOD(outParent, (OldLodRecord*)child);
				break;
			case DOF_OP:
				outPrim = visitDOF(outParent, (DofRecord*)child);
				break;
			case BSP_OP:
				outPrim = visitBSP(outParent, (BSPRecord*)child);
				break;
			case SWITCH_OP:
				outPrim = visitSwitch(outParent, (SwitchRecord*)child);
				break;
			case OBJECT_OP:
				outPrim = visitObject(outParent, (ObjectRecord*)child);
				break;
			case INSTANCE_REFERENCE_OP:
				outPrim = visitInstanceReference(outParent, (InstanceReferenceRecord*)child);
				break;
			case INSTANCE_DEFINITION_OP:
				outPrim = visitInstanceDefinition(outParent, (InstanceDefinitionRecord*)child);
				break;
			case EXTERNAL_REFERENCE_OP:
				outPrim = visitExternal(outParent, (ExternalRecord*)child);
				break;
			case ROAD_CONSTRUCTION_OP:
				// treat road construction record as a group record for now
				outPrim = visitRoadConstruction(outParent, (GroupRecord*)child);
				break;

			default:

#ifdef _DEBUG

				CRCore::notify(CRCore::INFO) << "In ConvertFromFLT::visitPrimaryNode(), unknown opcode: " << child->getOpcode() << std::endl;

#endif

				break;
			}
		}
	}

	
	if( !geoSetBuilder.empty() )
	{
		//if(!bBlend)
		{
			CRCore::crObject* geode = new CRCore::crObject;        
			geoSetBuilder.createOsgGeoSets(geode );

			if (geode->getNumDrawables() > 0)
				outParent.addChild( geode );
			geode->setName(outParent.getName());
		}
		//else
		//{
		//	CRCore::crDepthSortedObject* geode = new CRCore::crDepthSortedObject;        
		//	geoSetBuilder.createOsgGeoSets(geode );
		//	if (geode->getNumDrawables() > 0)
		//	{
		//		outParent.addChild( geode );
		//	}
		//}
	}

	if( !axisRoteBillboardBuilder.empty() )
	{
		CRCore::crBillboard* billboard = new CRCore::crBillboard;
		outParent.addChild( billboard );
		axisRoteBillboardBuilder.createOsgBillboardSets(billboard);
		billboard->setName(outParent.getName());
	}

	if( !pointRoteBillboardBuilder.empty() )
	{
		CRCore::crBillboard* billboard = new CRCore::crBillboard;
		billboard->setMode(CRCore::crBillboard::POINT_ROT_EYE);
		outParent.addChild( billboard );
		pointRoteBillboardBuilder.createOsgBillboardSets(billboard);
		billboard->setName(outParent.getName());
	}

	return outPrim;
}


void ConvertFromFLT::visitLongID(CRCore::crGroup& outParent, LongIDRecord* rec)
{
	SLongID *pSLongID = (SLongID*)rec->getData();

	unsigned int stringLength = mystrnlen(pSLongID->szIdent,rec->getBodyLength());
	outParent.setName(std::string(pSLongID->szIdent,stringLength));

	//CRCore::notify(CRCore::INFO)<<"ConvertFromFLT::visitLongID: "<<outParent.getName()<<std::endl;
}


void ConvertFromFLT::visitComment(CRCore::crNode& outParent, CommentRecord* rec)
{
	SComment *pSComment = (SComment*)rec->getData();

	//std::cout << "ConvertFromFLT::visitComment '"<<std::string(pSComment->szComment,pSComment->RecHeader.length()-4)<<"'"<<std::endl;
	//std::cout << "ConvertFromFLT::visitComment cstyle string '"<<pSComment->szComment<<"'"<<std::endl;

	unsigned int stringLength = mystrnlen(pSComment->szComment,rec->getBodyLength());
	std::string commentfield(pSComment->szComment,stringLength);
	unsigned int front_of_line = 0;
	unsigned int end_of_line = 0;
	while (end_of_line<commentfield.size())
	{
		/*if (commentfield[end_of_line]=='\r')
		{
			outParent.addDescription( std::string( commentfield, front_of_line, end_of_line-front_of_line) );

			if (end_of_line+1<commentfield.size() &&
				commentfield[end_of_line+1]=='\n') ++end_of_line;

			++end_of_line;
			front_of_line = end_of_line;
		}
		else */if (commentfield[end_of_line]=='\n')
		{
		    ++end_of_line;
			outParent.addDescription( std::string( commentfield, front_of_line, end_of_line-front_of_line) );
			
			front_of_line = end_of_line;
		}
		else ++end_of_line;
	}
	if (front_of_line<end_of_line)
	{
		outParent.addDescription( std::string( commentfield, front_of_line, end_of_line-front_of_line) );
	}

}


CRCore::crGroup* ConvertFromFLT::visitHeader(HeaderRecord* rec)
{
	SHeader *pSHeader = (SHeader*)rec->getData();

	// Version
	m_diOpenFlightVersion = pSHeader->diFormatRevLev;
	//CRCore::notify(CRCore::INFO) << "Version " << m_diOpenFlightVersion << std::endl;

	CRCore::crGroup* group = new CRCore::crGroup;
	group->setName(pSHeader->szIdent);
	visitAncillary(*group, *group, rec);

	//CRCore::crDB *db = CRCore::crBrain::getInstance()->getCurrentActiveDB();
	//db->getDescriptions().insert(db->getDescriptions().end(),group->getDescriptions().begin(),group->getDescriptions().end());

	std::string strbuf;
	bool exteriorCoordUnit = CRCore::crArgumentParser::readKeyValue(group->getDescriptions(),"DesiredUnits",strbuf);

	// Unit scale
	if ( m_doUnitsConversion ) 
	{
		switch (rec->getFltFile()->getDesiredUnits())
		{
		case FltFile::ConvertToMeters:
			if(exteriorCoordUnit)
			{
				if(strbuf.compare("centimeter")==0)
				{
					m_unitScale = 0.01f;
				}

				break;
			}
			switch (pSHeader->swVertexCoordUnit)
			{
			case HeaderRecord::METERS:
				m_unitScale = 1.0;
				break;
			case HeaderRecord::KILOMETERS:
				m_unitScale = 1000.0;
				break;
			case HeaderRecord::FEET:
				m_unitScale = 0.3048;
				break;
			case HeaderRecord::INCHES:
				m_unitScale = 0.02540;
				break;
			case HeaderRecord::NAUTICAL_MILES:
				m_unitScale = 1852.0;
				break;
			default:
				m_unitScale = 1.0;
			}
			break;

		case FltFile::ConvertToKilometers:
			if(exteriorCoordUnit)
			{
				if(strbuf.compare("centimeter")==0)
				{
					m_unitScale = 0.00001;
				}

				break;
			}
			switch (pSHeader->swVertexCoordUnit)
			{
			case HeaderRecord::METERS:
				m_unitScale = 0.001;
				break;
			case HeaderRecord::KILOMETERS:
				m_unitScale = 1.0;
				break;
			case HeaderRecord::FEET:
				m_unitScale = 0.0003048;
				break;
			case HeaderRecord::INCHES:
				m_unitScale = 0.0000254;
				break;
			case HeaderRecord::NAUTICAL_MILES:
				m_unitScale = 1.852;
				break;
			default:
				m_unitScale = 1.0;
			}
			break;

		case FltFile::ConvertToFeet:
			switch (pSHeader->swVertexCoordUnit)
			{
			case HeaderRecord::METERS:
				m_unitScale = 3.2808399;
				break;
			case HeaderRecord::KILOMETERS:
				m_unitScale = 3280.839895;
				break;
			case HeaderRecord::FEET:
				m_unitScale = 1.0;
				break;
			case HeaderRecord::INCHES:
				m_unitScale = 0.0833333;
				break;
			case HeaderRecord::NAUTICAL_MILES:
				m_unitScale = 6076.1154856 ;
				break;
			default:
				m_unitScale = 1.0;
			}
			break;

		case FltFile::ConvertToInches:
			switch (pSHeader->swVertexCoordUnit)
			{
			case HeaderRecord::METERS:
				m_unitScale = 39.3700787;
				break;
			case HeaderRecord::KILOMETERS:
				m_unitScale = 39370.0787402;
				break;
			case HeaderRecord::FEET:
				m_unitScale = 12.0;
				break;
			case HeaderRecord::INCHES:
				m_unitScale = 1.0;
				break;
			case HeaderRecord::NAUTICAL_MILES:
				m_unitScale = 72913.3858268;
				break;
			default:
				m_unitScale = 1.0;
			}
			break;

		case FltFile::ConvertToNauticalMiles:
			switch (pSHeader->swVertexCoordUnit)
			{
			case HeaderRecord::METERS:
				m_unitScale = 0.0005399568;
				break;
			case HeaderRecord::KILOMETERS:
				m_unitScale = 0.5399568;
				break;
			case HeaderRecord::FEET:
				m_unitScale = 0.0001646;
				break;
			case HeaderRecord::INCHES:
				m_unitScale = 0.0000137;
				break;
			case HeaderRecord::NAUTICAL_MILES:
				m_unitScale = 1.0;
				break;
			default:
				m_unitScale = 1.0;
			}
			break;

		default:
			m_unitScale = 1.0;
			break;
		}
	}
	else
	{    
		m_unitScale = 1.0;
	}

	// Flight v.11 & v.12 use integer coordinates
	if (rec->getFlightVersion() < 13)
	{
		if (pSHeader->iMultDivUnit >= 0)
			m_unitScale *= (double)pSHeader->iMultDivUnit;
		else
			m_unitScale /= (double)(-pSHeader->iMultDivUnit);
	}

	m_bHdrRgbMode = (pSHeader->dwFlags & 0x40000000) ? true : false;    // RGB space (=packed color)

	visitPrimaryNode(*group, rec);

	return group;
}


/*outParent*/
void ConvertFromFLT::visitColorPalette(CRCore::crGroup& , ColorPaletteRecord* rec)
{
	if (!rec->getFltFile()->useInternalColorPalette()) return;

	ColorPool* pColorPool = rec->getFltFile()->getColorPool();
	int flightVersion = rec->getFlightVersion();

	if (flightVersion > 13)
	{
		SColorPalette* pCol = (SColorPalette*)rec->getData();
		int colors = (flightVersion >= 1500) ? 1024 : 512;

		for (int i=0; i < colors; i++)
		{
			CRCore::crVector4 color(pCol->Colors[i].get());
			color[3] = 1.0f;    // Force alpha to one
			pColorPool->addColor(i, color);
		}
	}
	else    // version 11, 12 & 13
	{
		SOldColorPalette* pCol = (SOldColorPalette*)rec->getData();
		unsigned int i;
		for (i=0; i < sizeof(pCol->Colors)/sizeof(pCol->Colors[0]); i++)
		{
			CRCore::crVector4 color(pCol->Colors[i].get());
			color[3] = 1.0f;    // Force alpha to one
			pColorPool->addColor(i, color);
		}

		for (i=0; i < sizeof(pCol->FixedColors)/sizeof(pCol->FixedColors[0]); i++)
		{
			CRCore::crVector4 color(pCol->FixedColors[i].get());
			color[3] = 1.0f;    // Force alpha to one
			pColorPool->addColor(i+(4096>>7), color);
		}
	}
}

/*outParent*/
void ConvertFromFLT::visitLightSourcePalette(CRCore::crGroup& , LightSourcePaletteRecord* rec)
{

	SLightSourcePalette* pLight = (SLightSourcePalette*)rec->getData();

	CRCore::crLight* light = new CRCore::crLight();

	
	switch(pLight->diLightType) 
	{
	case 0:
		light->setLightType(CRCore::crLight::LIGHT_INFINITE);
		break;
	case 16777216:
		light->setLightType(CRCore::crLight::LIGHT_LOCAL);
		break;
	case 33554432:
		light->setLightType(CRCore::crLight::LIGHT_SPOT);
		break;
	}

	light->setAmbient( CRCore::crVector4(
		pLight->sfAmbientRGBA[0], pLight->sfAmbientRGBA[1],
		pLight->sfAmbientRGBA[2], pLight->sfAmbientRGBA[3] ) );
	light->setDiffuse( CRCore::crVector4(
		pLight->sfDiffuseRGBA[0], pLight->sfDiffuseRGBA[1],
		pLight->sfDiffuseRGBA[2], pLight->sfDiffuseRGBA[3] ) );
	light->setSpecular( CRCore::crVector4(
		pLight->sfSpecularRGBA[0], pLight->sfSpecularRGBA[1],
		pLight->sfSpecularRGBA[2], pLight->sfSpecularRGBA[3] ) );
	light->setConstantAttenuation( pLight->sfConstantAttuenation );
	light->setLinearAttenuation( pLight->sfLinearAttuenation );
	light->setQuadraticAttenuation( pLight->sfQuadraticAttuenation );
	light->setSpotExponent( pLight->sfDropoff );
	light->setSpotCutoff( pLight->sfCutoff );

	LightPool* pLightPool = rec->getFltFile()->getLightPool();
	pLightPool->addLight( pLight->diIndex, light );
}

/*outParent*/
void ConvertFromFLT::visitMaterialPalette(CRCore::crGroup&, MaterialPaletteRecord* rec)
{
	if (!rec->getFltFile()->useInternalMaterialPalette()) return;

	SMaterial* pSMaterial = (SMaterial*)rec->getData();
	MaterialPool* pMaterialPool = rec->getFltFile()->getMaterialPool();
	if (pSMaterial && pMaterialPool)
	{
		MaterialPool::PoolMaterial* pPoolMat = new MaterialPool::PoolMaterial;

		pPoolMat->Ambient   = pSMaterial->Ambient;
		pPoolMat->Diffuse   = pSMaterial->Diffuse;
		pPoolMat->Specular  = pSMaterial->Specular;
		pPoolMat->Emissive  = pSMaterial->Emissive;
		pPoolMat->sfShininess = pSMaterial->sfShininess;
		pPoolMat->sfAlpha   = pSMaterial->sfAlpha;

		pMaterialPool->addMaterial((int)pSMaterial->diIndex, pPoolMat);
	}
}

/*outParent*/
void ConvertFromFLT::visitOldMaterialPalette(CRCore::crGroup& , OldMaterialPaletteRecord* rec)
{
	if (!rec->getFltFile()->useInternalMaterialPalette()) return;

	SOldMaterial* pSMaterial = (SOldMaterial*)rec->getData();
	MaterialPool* pMaterialPool = rec->getFltFile()->getMaterialPool();

	if (pSMaterial && pMaterialPool )
	{
		for (int i=0; i < 64; i++)
		{
			MaterialPool::PoolMaterial* pPoolMat = new MaterialPool::PoolMaterial;

			pPoolMat->Ambient   = pSMaterial->mat[i].Ambient;
			pPoolMat->Diffuse   = pSMaterial->mat[i].Diffuse;
			pPoolMat->Specular  = pSMaterial->mat[i].Specular;
			pPoolMat->Emissive  = pSMaterial->mat[i].Emissive;
			pPoolMat->sfShininess = pSMaterial->mat[i].sfShininess;
			pPoolMat->sfAlpha   = pSMaterial->mat[i].sfAlpha;

			pMaterialPool->addMaterial(i, pPoolMat);
		}
	}
}

/*outParent*/
void ConvertFromFLT::visitTexturePalette(CRCore::crGroup& , TexturePaletteRecord* rec)
{
	int nIndex;
	char* pFilename;

	if (!rec->getFltFile()->useInternalTexturePalette()) return;


	if (rec->getFlightVersion() > 13)
	{
		STexturePalette* pTexture = (STexturePalette*)rec->getData();
		pFilename = pTexture->szFilename;
		nIndex = pTexture->diIndex;
	}
	else // version 11, 12 & 13
	{
		SOldTexturePalette* pOldTexture = (SOldTexturePalette*)rec->getData();
		pFilename = pOldTexture->szFilename;
		nIndex = pOldTexture->diIndex;
	}

	TexturePool* pTexturePool = rec->getFltFile()->getTexturePool();
	if (pTexturePool == NULL) return;


	std::string textureName(pFilename);
	pTexturePool->addTextureName(nIndex, textureName);

	//CERR<<"pTexturePool->addTextureName("<<nIndex<<", "<<textureName<<")"<<std::endl;
}


void ConvertFromFLT::visitLtPtAppearancePalette(CRCore::crGroup& /*outParent*/, LtPtAppearancePaletteRecord* rec)
{
	SLightPointAppearancePalette* ltPtApp = (SLightPointAppearancePalette*)rec->getData();
	LtPtAppearancePool* pool = rec->getFltFile()->getLtPtAppearancePool();
	assert( pool );
	if (ltPtApp && pool)
	{
		LtPtAppearancePool::PoolLtPtAppearance* entry = new LtPtAppearancePool::PoolLtPtAppearance;

		entry->m_iBackColorIdx = ltPtApp->backColor;
		entry->m_bIntensity = ltPtApp->intensity;
		entry->m_sfMinPixelSize = ltPtApp->minPixelSize;
		entry->m_sfMaxPixelSize = ltPtApp->maxPixelSize;
		entry->m_sfActualSize = ltPtApp->actualSize;
		entry->m_iDirectionality = ltPtApp->directionality;
		entry->m_sfHLobeAngle = ltPtApp->horizLobeAngle;
		entry->m_sfVLobeAngle = ltPtApp->vertLobeAngle;
		entry->m_sfLobeRollAngle = ltPtApp->lobeRollAngle;

		pool->add(ltPtApp->index, entry);
	}
}

void ConvertFromFLT::visitLtPtAnimationPalette(CRCore::crGroup& /*outParent*/, LtPtAnimationPaletteRecord* rec)
{
	//SLightPointAnimationPalette* ltPtAnim = (SLightPointAnimationPalette*)rec->getData();
	//LtPtAnimationPool* pool = rec->getFltFile()->getLtPtAnimationPool();
	//assert( pool );
	//if (ltPtAnim && pool)
	//{
	//	CRCore::ref_ptr<LtPtAnimationPool::PoolLtPtAnimation> entry = new LtPtAnimationPool::PoolLtPtAnimation;

	//	entry->m_name = std::string( ltPtAnim->name );

	//	// Support sequenced animations
	//	if ( (ltPtAnim->animType == LtPtAnimationPaletteRecord::SEQ_TYPE) &&
	//		(ltPtAnim->numSequences > 0) )
	//	{
	//		CRCore::ref_ptr<CRSim::crBlinkSequence> b = new CRSim::crBlinkSequence;
	//		for (int idx=0; idx<ltPtAnim->numSequences; idx++)
	//		{
	//			SLightPointAnimationSequence* seq = rec->sequence( idx );
	//			CRCore::crVector4 color( 0.f, 0.f, 0.f, 0.f );
	//			if (seq->seqState != LtPtAnimationPaletteRecord::SEQ_OFF)
	//			{
	//				// crSequence state is On or Color Change, so set the color to non-black
	//				ColorPool* pColorPool = rec->getFltFile()->getColorPool();
	//				color = pColorPool->getColor( seq->seqColor );
	//			}
	//			b->addPulse( seq->duration, color );
	//		}
	//		entry->m_blink = b;
	//	}
	//	// Support strobe animations
	//	else if (ltPtAnim->animType == LtPtAnimationPaletteRecord::STROBE_TYPE)
	//	{
	//		CRCore::ref_ptr<CRSim::crBlinkSequence> b = new CRSim::crBlinkSequence;
	//		const float duration = .5f / ltPtAnim->period;
	//		b->addPulse( duration, CRCore::crVector4( 0.f, 0.f, 0.f, 0.f ) );
	//		b->addPulse( duration, CRCore::crVector4( 1.f, 1.f, 1.f, 1.f ) );
	//		entry->m_blink = b;
	//	}

	//	pool->add( ltPtAnim->index, entry.get() );
	//}
}

/*outParent*/
void ConvertFromFLT::visitVertexPalette(CRCore::crGroup& , VertexPaletteRecord* rec)
{
	m_diCurrentOffset = rec->getSize();
}

/*outParent*/
void ConvertFromFLT::visitVertex(CRCore::crGroup& , VertexRecord* rec)
{
	regisiterVertex(m_diCurrentOffset, rec);
	m_diCurrentOffset += rec->getSize();
}

/*outParent*/
void ConvertFromFLT::visitNormalVertex(CRCore::crGroup& , NormalVertexRecord* rec)
{
	regisiterVertex(m_diCurrentOffset, rec);
	m_diCurrentOffset += rec->getSize();
}

/*outParent*/
void ConvertFromFLT::visitTextureVertex(CRCore::crGroup& , TextureVertexRecord* rec)
{
	regisiterVertex(m_diCurrentOffset, rec);
	m_diCurrentOffset += rec->getSize();
}

/*outParent*/
void ConvertFromFLT::visitNormalTextureVertex(CRCore::crGroup& , NormalTextureVertexRecord* rec)
{
	regisiterVertex(m_diCurrentOffset, rec);
	m_diCurrentOffset += rec->getSize();
}


CRCore::crGroup* ConvertFromFLT::visitBSP(CRCore::crGroup& outParent, BSPRecord* rec)
{
	// create group node for the time being
	CRCore::crGroup* group = new CRCore::crGroup;
	group->setName(rec->getData()->szIdent);

	visitAncillary(outParent, *group, rec)->addChild( group );
	visitPrimaryNode(*group, rec);

	return group;
}

CRCore::crGroup* ConvertFromFLT::visitGroup(CRCore::crGroup& outParent, GroupRecord* rec)
{
	const int fltVer = rec->getFltFile()->getFlightVersion();

	SGroup* currentGroup = (SGroup*) rec->getData();

	const bool forwardAnim = (currentGroup->dwFlags & GroupRecord::FORWARD_ANIM)!=0;
	// OpenFlight 15.8 adds backwards animations
	const bool backwardAnim = ( (fltVer >= 1580) &&
		((currentGroup->dwFlags & GroupRecord::BACKWARD_ANIM) != 0) );
	// Regardless of forwards or backwards, animation could have swing bit set
	const CRCore::crSequence::LoopMode loopMode = ( (currentGroup->dwFlags & GroupRecord::SWING_ANIM) == 0 ) ?
		CRCore::crSequence::LOOP : CRCore::crSequence::SWING;

	if( forwardAnim || backwardAnim)
	{
		CRCore::ref_ptr<CRCore::crSequence> animSeq = new CRCore::crSequence;

		CRCore::crGroup *parent = visitAncillary(outParent, *animSeq, rec);
		std::string str;
		std::string comment = animSeq->getDescriptionsInString();
		if(CRCore::crArgumentParser::readKeyValue(comment,"Type",str))
		{
			if(str.compare("MatrixSequence")==0 || str.compare("MatrixSequenceStateSet")==0)
			{
                CRCore::ref_ptr<CRCore::crMatrixSequence> animSeq2 = new CRCore::crMatrixSequence;
                animSeq2->setDescriptionsInString(comment);
				animSeq2->setName(animSeq->getName());
				animSeq = animSeq2.get();
//				CRCore::notify(CRCore::ALWAYS)<<"visitGroup "<<animSeq->className()<< std::endl;
			}
		}
		parent->addChild( animSeq.get() );
		visitPrimaryNode(*animSeq, rec);

		const int numReps = (fltVer >= 1580) ?
			currentGroup->iLoopCount : 0;
		const float frameDuration = (fltVer >= 1580) ?
			currentGroup->fLoopDuration / (float)animSeq->getNumChildren() : 0.f;
		animSeq->setDuration( frameDuration, numReps==0?-1:numReps );

		if ( forwardAnim )
			animSeq->setInterval( loopMode, 0, -1 );
		else // Backwards animation
			animSeq->setInterval( loopMode, -1, 0 );

        animSeq->setMode( CRCore::crSequence::START );

		// Only set the name from the normal record ID if the visitAncillary()
		// call didn't find a Long ID record on this group
		if (animSeq->getName().length() == 0)
			animSeq->setName(rec->getData()->szIdent);

		return animSeq.get();
	}

	else
	{
		CRCore::ref_ptr<CRCore::crGroup> group = new CRCore::crGroup;
		group->setName(rec->getData()->szIdent);
		CRCore::crGroup *parent = visitAncillary(outParent, *group, rec);
		group = parseComment(*group, rec);
        parent->addChild( group.get() );
		visitPrimaryNode(*group, rec);
		return group.get();
	}
}

CRCore::crGroup* ConvertFromFLT::visitLightSource(CRCore::crGroup& outParent, LightSourceRecord* rec)
{
	static int lightnum = 0;

	LightPool* pLightPool = rec->getFltFile()->getLightPool();
	SLightSource* pLSource = (SLightSource*) rec->getData();

	//if ( !(pLSource->dwFlags & 0x80000000) ) 
	//{ // enabled
	//	return NULL;
	//}

	CRCore::crLightSource* lightSource = new CRCore::crLightSource();

	CRCore::crLight* pLight = pLightPool->getLight( pLSource->diIndex );
	CRCore::crLight* light = dynamic_cast<CRCore::crLight*>(pLight->clone(CRCore::crCopyOp::SHALLOW_COPY));

	//light->setPosition( CRCore::crVector4(
	//	pLSource->Coord.x(), pLSource->Coord.y(),
	//	pLSource->Coord.z(), 1 ) );
	pLSource->sfPitch = CRCore::inDegrees(pLSource->sfPitch);
	pLSource->sfYaw = CRCore::inDegrees(pLSource->sfYaw);

	switch(pLight->getLightType()) 
	{
	case CRCore::crLight::LIGHT_INFINITE:
		/*light->setPosition( CRCore::crVector4(
			cos(pLSource->sfPitch)*cos(pLSource->sfYaw),
			cos(pLSource->sfPitch)*sin(pLSource->sfYaw),
			sin(pLSource->sfPitch), 0 ) );*/
		light->setPosition( CRCore::crVector4(
			cos(pLSource->sfPitch)*sin(-pLSource->sfYaw),
			cos(pLSource->sfPitch)*cos(-pLSource->sfYaw),
			sin(pLSource->sfPitch),0.0) );
		break;
	case CRCore::crLight::LIGHT_LOCAL:
		light->setPosition( CRCore::crVector4(
			pLSource->Coord.x() * m_unitScale, pLSource->Coord.y() * m_unitScale,
			pLSource->Coord.z() * m_unitScale, 1.0 ) );
		light->setDirection( CRCore::crVector3(
			cos(pLSource->sfPitch)*sin(-pLSource->sfYaw),
			cos(pLSource->sfPitch)*cos(-pLSource->sfYaw),
			sin(pLSource->sfPitch)) );
	case CRCore::crLight::LIGHT_SPOT:
		light->setPosition( CRCore::crVector4(
			pLSource->Coord.x() * m_unitScale, pLSource->Coord.y() * m_unitScale,
			pLSource->Coord.z() * m_unitScale, 1.0 ) );
		light->setDirection( CRCore::crVector3(
			cos(pLSource->sfPitch)*sin(-pLSource->sfYaw),
			cos(pLSource->sfPitch)*cos(-pLSource->sfYaw),
			sin(pLSource->sfPitch)) );
		break;
	}
	//std::cout<<"lightPosx "<<pLSource->Coord.x()<<std::endl;
	light->setLightNum( lightnum++ );
	//lightnum = lightnum>=7?0:lightnum+1;

	////////////int lightid = CRCore::crBrain::getInstance()->getResource()->findOrInsertLight( pLSource->diIndex, light );

	///if(lightid<-1) return NULL;
	//lightSource->setLight( lightid/*, CRCore::crStateSet::StateData::ON */);
	lightSource->setLight(light);
	lightSource->setReferenceFrame(CRCore::crLightSource::RELATIVE_TO_PARENTS);

	if ( !(pLSource->dwFlags & 0x80000000) ) 
	{ // enabled = false
		lightSource->setEnable(false);
	}

	lightSource->setName(((SGroup*)(rec->getData()))->szIdent);
	//visitComment(*lightSource, (CommentRecord*)rec);

	CRCore::crGroup* node = &outParent;
	if ( pLSource->dwFlags & 0x40000000  ) 
	{ // global
		lightSource->setGlobal(true);
		//CRCore::crBrain::getInstance()->increaseGlobalLight();

		//while ( !node->getParents().empty() ) 
		//{
		//	node = *(node->getParents().begin());
		//}
		
		//for(CRCore::NodeArray::iterator itr = node->getChildArray().begin();
		//	itr != node->getChildArray().end();
		//	itr++)
		//{
  //          lightSource->addChild(itr->get());
		//}
		//node->removeChild(0,node->getNumChildren());
		//node->addChild(lightSource);
  //      visitPrimaryNode(outParent, rec);
	}
	else 
	{//
		lightSource->setGlobal(false);
		//outParent.addChild( lightSource );
		//visitPrimaryNode(*lightSource, rec);
	}

	//outParent.addChild( lightSource );
	visitAncillary(outParent, *lightSource, rec)->addChild( lightSource );
	visitPrimaryNode(*lightSource, rec);

	//std::string lightMapName;
	//if(CRCore::crArgumentParser::readKeyValue(lightSource->getDescriptions(),"lightMap",lightMapName))
	//{
	//	CRCore::crImage *lightMapImg = NULL;
	//	if(lightMapName.compare("SunLightMap")==0)
	//	{
	//		ImageMap::iterator itr = s_imageMap.find(lightMapName);
	//	    if(itr!=s_imageMap.end())
	//			lightMapImg = itr->second.get();
	//		else
	//		{
	//			lightMapImg = CRCore::crLight::createSunLightImage();
	//		    s_imageMap[lightMapName] = lightMapImg;
	//		}
	//	}
	//	else if(lightMapName.compare("StdSpotLightMap")==0)
	//	{
	//		ImageMap::iterator itr = s_imageMap.find(lightMapName);
	//		if(itr!=s_imageMap.end())
	//			lightMapImg = itr->second.get();
	//		else
	//		{
	//			lightMapImg = CRCore::crLight::createStdSpotLightImage();
	//		    s_imageMap[lightMapName] = lightMapImg;
	//		}
	//	}
	//	else
	//	{
	//		lightMapImg = get_loadImage(lightMapName);
	//	}

	//	if(lightMapImg)
	//	{
 //           light->setLightMapFileName(lightMapName);
	//		light->createLightTexture(lightMapImg);
	//	}
	//}
    int isSunLight = 0;
	if(CRCore::crArgumentParser::readKeyValue(lightSource->getDescriptions(),"SunLight",isSunLight))
	{
        lightSource->setLightToBeSunLight(isSunLight);
	}
	return lightSource;
}

CRCore::crGroup* ConvertFromFLT::visitRoadConstruction(CRCore::crGroup& outParent, GroupRecord* rec)
{
	CRCore::crGroup* group = new CRCore::crGroup;

	group->setName(rec->getData()->szIdent);
	//cout<<"Converted a road construction node of ID "<<group->getName()<<" to group node."<<endl;
	visitAncillary(outParent, *group, rec)->addChild( group );
	visitPrimaryNode(*group, rec);
	return group;
}

CRCore::crGroup* ConvertFromFLT::visitLOD(CRCore::crGroup& outParent, LodRecord* rec)
{
	SLevelOfDetail* pSLOD = rec->getData();
	CRCore::crLod* lod = new CRCore::crLod;

	float64x3* pCenter = &pSLOD->Center;
	lod->setCenter(CRCore::crVector3(pCenter->x(), pCenter->y(), pCenter->z())*m_unitScale);

	lod->setRange(0, pSLOD->dfSwitchOutDist*m_unitScale,
		pSLOD->dfSwitchInDist*m_unitScale);

	lod->setName(pSLOD->szIdent);

	visitAncillary(outParent, *lod, rec)->addChild( lod );

	CRCore::crGroup* group = new CRCore::crGroup;
	lod->addChild(group);
	visitPrimaryNode(*group, rec);
	float radius;
	if(CRCore::crArgumentParser::readKeyValue(lod->getDescriptions(),"LodRadius",radius))
	{
		//CRCore::notify(CRCore::ALWAYS)<<" visitLOD radius = "<<radius<<std::endl;
        lod->setRadius(radius * m_unitScale);
	}
	return lod;
}


CRCore::crGroup* ConvertFromFLT::visitOldLOD(CRCore::crGroup& outParent, OldLodRecord* rec)
{
	SOldLOD* pSLOD = (SOldLOD*)rec->getData();
	CRCore::crLod* lod = new CRCore::crLod;

	lod->setCenter(CRCore::crVector3(
		(float)pSLOD->Center[0],
		(float)pSLOD->Center[1],
		(float)pSLOD->Center[2])*m_unitScale);

	lod->setRange(0, ((float)pSLOD->dwSwitchOutDist)*m_unitScale,
		((float)pSLOD->dwSwitchInDist)*m_unitScale);

	lod->setName(pSLOD->szIdent);

	visitAncillary(outParent, *lod, rec)->addChild( lod );

	CRCore::crGroup* group = new CRCore::crGroup;
	lod->addChild(group);
	visitPrimaryNode(*group, rec);

	return lod;
}

// TODO: DOF node implemented as crGroup.
// Converted DOF to use transform - jtracy@ist.ucf.edu
CRCore::crGroup* ConvertFromFLT::visitDOF(CRCore::crGroup& outParent, DofRecord* rec)
{
#define USE_DOFTransform

#if defined(USE_DOFTransform)

	CRCore::crDOFTransform* transform = new CRCore::crDOFTransform;
	transform->setName(rec->getData()->szIdent);
	transform->setDataVariance(CRCore::crBase::DYNAMIC);
	visitAncillary(outParent, *transform, rec)->addChild( transform );
	visitPrimaryNode(*transform, (PrimNodeRecord*)rec);

	unsigned short flags = 0xffff;
	unsigned short this_flag;
	std::string command = transform->getDescriptionsInString();
	int int1;
	if(CRCore::crArgumentParser::readKeyValue(command,"XTranSurge",int1))
	{
		this_flag = (unsigned short)1;
		if(int1) flags |= this_flag;
		else flags &= ~this_flag;
	}
	if(CRCore::crArgumentParser::readKeyValue(command,"YTranSurge",int1))
	{
		this_flag = (unsigned short)1<<1;
		if(int1) flags |= this_flag;
		else flags &= ~this_flag;
	}
	if(CRCore::crArgumentParser::readKeyValue(command,"ZTranSurge",int1))
	{
		this_flag = (unsigned short)1<<2;
		if(int1) flags |= this_flag;
		else flags &= ~this_flag;
	}
	if(CRCore::crArgumentParser::readKeyValue(command,"PitchSurge",int1))
	{
		//CRCore::notify(CRCore::FATAL)<<"PitchSurge = "<<int1<<std::endl;
		this_flag = (unsigned short)1<<3;
		if(int1) flags |= this_flag;
		else flags &= ~this_flag;
	}
	if(CRCore::crArgumentParser::readKeyValue(command,"RollSurge",int1))
	{
		//CRCore::notify(CRCore::FATAL)<<"RollSurge = "<<int1<<std::endl;
		this_flag = (unsigned short)1<<4;
		if(int1) flags |= this_flag;
		else flags &= ~this_flag;
	}
	if(CRCore::crArgumentParser::readKeyValue(command,"YawSurge",int1))
	{
		//CRCore::notify(CRCore::FATAL)<<"YawSurge = "<<int1<<std::endl;
		this_flag = (unsigned short)1<<5;
		if(int1) flags |= this_flag;
		else flags &= ~this_flag;
	}
	if(CRCore::crArgumentParser::readKeyValue(command,"XScale",int1))
	{
		this_flag = (unsigned short)1<<6;
		if(int1) flags |= this_flag;
		else flags &= ~this_flag;
	}
	if(CRCore::crArgumentParser::readKeyValue(command,"YScale",int1))
	{
		this_flag = (unsigned short)1<<7;
		if(int1) flags |= this_flag;
		else flags &= ~this_flag;
	}
	if(CRCore::crArgumentParser::readKeyValue(command,"ZScale",int1))
	{
		this_flag = (unsigned short)1<<8;
		if(int1) flags |= this_flag;
		else flags &= ~this_flag;
	}
	transform->setSurgeFlags(flags);

	SDegreeOfFreedom* p_data = rec->getData();

	//now fill up members:

	//tranlsations:
	transform->setMinTranslate(CRCore::crVector3(m_unitScale*p_data->dfX._dfMin,
		m_unitScale*p_data->dfY._dfMin,
		m_unitScale*p_data->dfZ._dfMin));

	transform->setMaxTranslate(CRCore::crVector3(m_unitScale*p_data->dfX._dfMax,
		m_unitScale*p_data->dfY._dfMax,
		m_unitScale*p_data->dfZ._dfMax));

	transform->setCurrentTranslate(CRCore::crVector3(m_unitScale*p_data->dfX._dfCurrent,
		m_unitScale*p_data->dfY._dfCurrent,
		m_unitScale*p_data->dfZ._dfCurrent));

	transform->setIncrementTranslate(CRCore::crVector3(m_unitScale*p_data->dfX._dfIncrement,
		m_unitScale*p_data->dfY._dfIncrement,
		m_unitScale*p_data->dfZ._dfIncrement));

	//rotations:
	transform->setMinHPR(CRCore::crVector3(CRCore::inDegrees(p_data->dfYaw._dfMin),
		CRCore::inDegrees(p_data->dfPitch._dfMin),
		CRCore::inDegrees(p_data->dfRoll._dfMin)));

	transform->setMaxHPR(CRCore::crVector3(CRCore::inDegrees(p_data->dfYaw._dfMax),
		CRCore::inDegrees(p_data->dfPitch._dfMax),
		CRCore::inDegrees(p_data->dfRoll._dfMax)));

	transform->setCurrentHPR(CRCore::crVector3(CRCore::inDegrees(p_data->dfYaw._dfCurrent),
		CRCore::inDegrees(p_data->dfPitch._dfCurrent),
		CRCore::inDegrees(p_data->dfRoll._dfCurrent)));

	transform->setIncrementHPR(CRCore::crVector3(CRCore::inDegrees(p_data->dfYaw._dfIncrement),
		CRCore::inDegrees(p_data->dfPitch._dfIncrement),
		CRCore::inDegrees(p_data->dfRoll._dfIncrement)));

	//scales:
	transform->setMinScale(CRCore::crVector3(p_data->dfXscale._dfMin,
		p_data->dfYscale._dfMin,
		p_data->dfZscale._dfMin));

	transform->setMaxScale(CRCore::crVector3(p_data->dfXscale._dfMax,
		p_data->dfYscale._dfMax,
		p_data->dfZscale._dfMax));

	transform->setCurrentScale(CRCore::crVector3(p_data->dfXscale._dfCurrent,
		p_data->dfYscale._dfCurrent,
		p_data->dfZscale._dfCurrent));

	transform->setIncrementScale(CRCore::crVector3(p_data->dfXscale._dfIncrement,
		p_data->dfYscale._dfIncrement,
		p_data->dfZscale._dfIncrement));

	// default DOF animation state
	if(CRCore::crArgumentParser::readKeyValue(command,"AnimationOn",int1))
	    transform->setAnimationOn(int1);
	else transform->setAnimationOn(true);

	CRCore::crVector3i endStop;
	if(CRCore::crArgumentParser::readKeyValue(command,"EndStopHPR",endStop))
		transform->setEndStopHPR(endStop);
	if(CRCore::crArgumentParser::readKeyValue(command,"EndStopTrans",endStop))
		transform->setEndStopTrans(endStop);
	if(CRCore::crArgumentParser::readKeyValue(command,"EndStopScale",endStop))
		transform->setEndStopScale(endStop);

	if(CRCore::crArgumentParser::readKeyValue(command,"EndLess",int1))
		transform->setEndless(int1);
	else transform->setEndless(true);

	float flt1;
	if(CRCore::crArgumentParser::readKeyValue(command,"LifeTime",flt1))
		transform->setLifeTime(flt1);
	if(CRCore::crArgumentParser::readKeyValue(command,"StartTime",flt1))
		transform->setStartTime(flt1);
	if(CRCore::crArgumentParser::readKeyValue(command,"ResetTime",flt1))
		transform->setResetTime(flt1);

	// compute the put matrix.
	CRCore::crVector3 O ( p_data->OriginLocalDOF.x(),
		p_data->OriginLocalDOF.y(),
		p_data->OriginLocalDOF.z());

	CRCore::crVector3 xAxis ( p_data->PointOnXaxis.x(),
		p_data->PointOnXaxis.y(),
		p_data->PointOnXaxis.z());
	xAxis = xAxis - O;
	xAxis.normalize();

	CRCore::crVector3 xyPlane ( p_data->PointInXYplane.x(),
		p_data->PointInXYplane.y(),
		p_data->PointInXYplane.z());
	xyPlane = xyPlane - O;
	xyPlane.normalize();

	CRCore::crVector3 normalz = xAxis ^ xyPlane;
	normalz.normalize();

	// get X, Y, Z axis of the DOF in terms of global coordinates
	CRCore::crVector3 Rz = normalz;
	if (Rz == CRCore::crVector3(0,0,0)) Rz[2] = 1;
	CRCore::crVector3 Rx = xAxis;
	if (Rx == CRCore::crVector3(0,0,0)) Rx[0] = 1;
	CRCore::crVector3 Ry = Rz ^ Rx;


	O *= m_unitScale;

	// create the putmatrix
	CRCore::crMatrix inv_putmat( Rx.x(), Rx.y(), Rx.z(), 0, 
		Ry.x(), Ry.y(), Ry.z(), 0,
		Rz.x(), Rz.y(), Rz.z(), 0,
		O.x(), O.y(), O.z(), 1);

	transform->setInversePutMatrix(inv_putmat);
	transform->setPutMatrix(CRCore::crMatrix::inverse(inv_putmat));


	/*
	//and now do a little ENDIAN to put flags in ordewr as described in OpenFlight spec
	unsigned long flags = p_data->dwFlags;
	ENDIAN(flags);

	//and setup limitation flags:
	//    transform->setLimitationFlags(flags);
	*/
	transform->setLimitationFlags(p_data->dwFlags);

	return transform;

#else

	CRCore::crMatrixTransform* transform = new CRCore::crMatrixTransform;

	transform->setName(rec->getData()->szIdent);
	transform->setDataVariance(CRCore::crBase::DYNAMIC);
	visitAncillary(outParent, *transform, rec)->addChild( transform );
	visitPrimaryNode(*transform, (PrimNodeRecord*)rec);

	SDegreeOfFreedom* p_data = rec->getData();

	// get transformation to O_world
	CRCore::crVector3 O ( p_data->OriginLocalDOF.x(),
		p_data->OriginLocalDOF.y(),
		p_data->OriginLocalDOF.z());

	CRCore::crVector3 xAxis ( p_data->PointOnXaxis.x(),
		p_data->PointOnXaxis.y(),
		p_data->PointOnXaxis.z());
	xAxis = xAxis - O;
	xAxis.normalize();

	CRCore::crVector3 xyPlane ( p_data->PointInXYplane.x(),
		p_data->PointInXYplane.y(),
		p_data->PointInXYplane.z());
	xyPlane = xyPlane - O;
	xyPlane.normalize();

	CRCore::crVector3 normalz = xAxis ^ xyPlane;
	normalz.normalize();

	// get X, Y, Z axis of the DOF in terms of global coordinates
	CRCore::crVector3 Rz = normalz;
	if (Rz == CRCore::crVector3(0,0,0)) Rz[2] = 1;
	CRCore::crVector3 Rx = xAxis;
	if (Rx == CRCore::crVector3(0,0,0)) Rx[0] = 1;
	CRCore::crVector3 Ry = Rz ^ Rx;

	// create the putmatrix
	CRCore::crMatrix putmat( Rx.x(), Rx.y(), Rx.z(), 0, 
		Ry.x(), Ry.y(), Ry.z(), 0,
		Rz.x(), Rz.y(), Rz.z(), 0,
		O.x(), O.y(), O.z(), 1);

	// apply DOF transformation
	CRCore::crVector3 trans(m_unitScale*p_data->dfX._dfCurrent,
		m_unitScale*p_data->dfY._dfCurrent,
		m_unitScale*p_data->dfZ._dfCurrent);

	float roll_rad = CRCore::inDegrees(p_data->dfRoll._dfCurrent);
	float pitch_rad = CRCore::inDegrees(p_data->dfPitch._dfCurrent);
	float yaw_rad = CRCore::inDegrees(p_data->dfYaw._dfCurrent);

	float sx = rec->getData()->dfXscale.current();
	float sy = rec->getData()->dfYscale.current();
	float sz = rec->getData()->dfZscale.current();

	// this is the local DOF transformation
	CRCore::crMatrix dof_matrix = CRCore::crMatrix::scale(sx, sy, sz)*
		CRCore::crMatrix::rotate(yaw_rad,  0.0f,0.0f,1.0f)*
		CRCore::crMatrix::rotate(roll_rad,  0.0f,1.0f,0.0f)*
		CRCore::crMatrix::rotate(pitch_rad, 1.0f,0.0f,0.0f)*
		CRCore::crMatrix::translate(trans);

	// transforming local into global
	dof_matrix.preMult(CRCore::crMatrix::inverse(putmat));

//	cout << "putmat "<< putmat<<endl;

	// transforming global into local
	dof_matrix.postMult(putmat);

	transform->setMatrix(dof_matrix);
	return transform;        

#endif

}


CRCore::crGroup* ConvertFromFLT::visitSwitch(CRCore::crGroup& outParent, SwitchRecord* rec)
{
	SSwitch *pSSwitch = (SSwitch*)rec->getData();
	CRCore::crMultiSwitch* outSwitch = new CRCore::crMultiSwitch;

	outSwitch->setName(pSSwitch->szIdent);
	visitAncillary(outParent, *outSwitch, rec)->addChild( outSwitch );
	visitPrimaryNode(*outSwitch, (PrimNodeRecord*)rec);

	unsigned int totalNumChildren = (unsigned int)rec->getNumChildren();
	if (totalNumChildren!=outSwitch->getNumChildren())
	{
		// only convert the children we agree on,
		// however, there could be a chance that ordering of children might
		// be different if there a children that hanvn't mapped across...
		if (totalNumChildren>outSwitch->getNumChildren()) totalNumChildren=outSwitch->getNumChildren();        
		//CRCore::notify(CRCore::INFO)<<"Warning::OpenFlight loader has come across an incorrectly handled switch."<<std::endl;
		//CRCore::notify(CRCore::INFO)<<"         The number of OpenFlight children ("<<rec->getNumChildren()<<") "<<std::endl;
		//CRCore::notify(CRCore::INFO)<<"         exceeds the number converted to OSG ("<<outSwitch->getNumChildren()<<")"<<std::endl;
	}

	// for each mask in the FLT Switch node
	for (int itMask=0; itMask<pSSwitch->nMasks; ++itMask) 
	{
		// I don't think we need to create a group node for each mask any more
		// create a CRCore group node
		//CRCore::ref_ptr<CRCore::crGroup> group = new CRCore::crGroup;
		//outSwitch->addChild( group.get() );

		// for each child in the FLT Switch node
		for (unsigned int itChild=0; itChild<totalNumChildren; ++itChild)
		{
			// test if this child is active in the current mask (itMask)
			unsigned int nMaskBit = itChild % 32;
			unsigned int nMaskWord = itMask * pSSwitch->nWordsInMask + itChild / 32;
			outSwitch->setValue(itMask, itChild, (pSSwitch->aMask[nMaskWord] & (uint32(1) << nMaskBit))!=0 );
		}
		// now the group contain all the childrens that are active in the current mask (itMask)
	}
	outSwitch->setActiveSwitchSet(pSSwitch->nCurrentMask);
	return outSwitch;
}

CRCore::crGroup* ConvertFromFLT::visitOccluderNode(CRCore::crOccluderNode& occluderNode, ObjectRecord* rec)
{
	if(!rec) return &occluderNode;

	CRCore::crConvexPlanarOccluder* cpo = new CRCore::crConvexPlanarOccluder;
	occluderNode.setOccluder(cpo);

	CRCore::crConvexPlanarPolygon& cpp = cpo->getOccluder();

	for(int i=0; i < rec->getNumChildren(); i++)
	{
		Record* child = rec->getChild(i);

		if (child && child->isPrimaryNode() && child->getOpcode() == FACE_OP)
		{
			FaceRecord* fr = (FaceRecord*)child;
			// Visit comment
			for(int i=0; i < fr->getNumChildren(); i++)
			{
				Record* frchild = fr->getChild(i);
				if (frchild && frchild->isAncillaryRecord() && frchild->getOpcode() == COMMENT_OP)
				{
					SComment *pSComment = (SComment*)frchild->getData();
					unsigned int stringLength = mystrnlen(pSComment->szComment,frchild->getBodyLength());
					std::string commentfield(pSComment->szComment,stringLength);
					if( commentfield.find("#Occluder") != -1 )
					{
						addOccluderVerticies(cpp,fr);
					}
					else if( commentfield.find("#Hole") != -1 )
					{
						CRCore::crConvexPlanarPolygon hole;
						addOccluderVerticies(hole,fr);
						cpo->addHole(hole);
					}
					break;
				}
			}
		}
	}

	return &occluderNode;
}

CRCore::crGroup* ConvertFromFLT::visitObject(CRCore::crGroup& outParent, ObjectRecord* rec)
{
	SObject *pSObject = (SObject*)rec->getData();
	CRCore::ref_ptr<CRCore::crGroup> object = new CRCore::crGroup;

	object->setName(rec->getName());
	CRCore::crGroup *parent = visitAncillary(outParent, *object, rec);
	object = parseComment(*object, rec);
	parent->addChild( object.get() );

	unsigned short  wPrevTransparency = m_wObjTransparency;
	m_wObjTransparency = pSObject->wTransparency;
	visitPrimaryNode(*object, (PrimNodeRecord*)rec);
	m_wObjTransparency = wPrevTransparency;

	if ( pSObject->dwFlags & 0xFC000000) // some of the 6 defined flag bits are set
	{
		std::string desc("flt object flags: 0x");
		char cflags[33];

		sprintf( cflags, "%X\0", (unsigned int)pSObject->dwFlags );
		desc = desc + cflags;

		object->getDescriptions().push_back( desc );
	}

	return object.get();
}

CRCore::crGroup* ConvertFromFLT::parseComment(CRCore::crGroup& group, Record *rec)
{
	CRCore::crNode::DescriptionArray& desArray = group.getDescriptions();
    if(desArray.empty())
		return &group;

	ExternalNode nodetype = GROUPNODE;
	std::string str = group.getDescriptionsInString();
	if(str.find("#SkyNode") != -1) 
	{
		nodetype = CLEARNODE;
		CRCore::crClearNode *clearNode = new CRCore::crClearNode;
		clearNode->setName(group.getName());
		clearNode->getDescriptions().insert(clearNode->getDescriptions().end(),desArray.begin(),desArray.end());
		return clearNode;
	}
	else if(str.find("#OccluderNode") != -1)
	{
		nodetype = OCCLUDERNODE;
		CRCore::crOccluderNode *occluderNode = new CRCore::crOccluderNode;
		occluderNode->setName(group.getName());
		occluderNode->getDescriptions().insert(occluderNode->getDescriptions().end(),desArray.begin(),desArray.end());
		visitOccluderNode(*occluderNode, dynamic_cast<ObjectRecord*>(rec));
		CRCore::crDB *db = CRCore::crBrain::getInstance()->getCurrentActiveDB();
		db->insertChild(0,occluderNode);
		return occluderNode;
	}
	//else if(str.find("#FX_SPECULARHIGHLIGHTS") != -1) 
	//{
	//	nodetype = FX_SPECULARHIGHLIGHTS;
	//	CRFX::crSpecularHighlights *shl = new CRFX::crSpecularHighlights;

	//	shl->setName(group.getName());
	//	shl->getDescriptions().insert(shl->getDescriptions().end(),desArray.begin(),desArray.end());

	//	int enable;
	//	int lightnum;
	//	int unit;
	//	CRCore::crVector4 color;
	//	float sexp;

	//	CRCore::crArgumentParser::readKeyValue(desArray[1],"enable",enable);
	//	CRCore::crArgumentParser::readKeyValue(desArray[2],"light_num",lightnum);
	//	CRCore::crArgumentParser::readKeyValue(desArray[3],"texture_unit",unit);
	//	CRCore::crArgumentParser::readKeyValue(desArray[4],"specular_color",color);
	//	CRCore::crArgumentParser::readKeyValue(desArray[5],"specular_exponent",sexp);
	//	
	//	shl->setEnabled(enable);
	//	shl->setLightNumber(lightnum);
	//	shl->setTextureUnit(unit);
	//	shl->setSpecularColor(color);
	//	shl->setSpecularExponent(sexp);
	//	shl->removeChild(0, shl->getNumChildren());
	//	shl->setUpDemo();

	//	shl->setName(group.getName());
	//	shl->getDescriptions().insert(shl->getDescriptions().end(),desArray.begin(),desArray.end());
	//	return shl;
	//}
	//else if(str.find("#FX_CUPMAPPING") != -1) 
	//{
	//	nodetype = FX_CUPMAPPING;
	//	//newGroup = new CRCore::crOccluderNode;
	//	//newGroup->setName(group.getName());
	//	//newGroup->getDescriptions().insert(newGroup->getDescriptions().end(),desArray.begin(),desArray.end());
	//	return &group;

	//}
	//else if(str.find("#FX_ANISOTROPICLIGHTING") != -1) 
	//{
	//	nodetype = FX_ANISOTROPICLIGHTING;
	//	return &group;

	//}
	//else if(str.find("#FX_BUMPMAPPING") != -1)
	//{
	//	nodetype = FX_BUMPMAPPING;
	//	return &group;

	//}
	//else if(str.find("#FX_CARTOON") != -1) 
	//{
	//	nodetype = FX_CARTOON;
	//	return &group;

	//}
	//else if(str.find("#FX_SCRIBE") != -1)
	//{
	//	nodetype = FX_SCRIBE;
	//	return &group;

	//}
    else if(str.find("#nodemovepath") != -1)
	{
		//CRCore::crAnimationPath *animationPath = new CRCore::crAnimationPath;
		//animationPath->setName(group.getName());
		//animationPath->getDescriptions().insert(animationPath->getDescriptions().end(),desArray.begin(),desArray.end());
		//
		//std::string pathdata;
		//int speed;

		//CRCore::crArgumentParser::readKeyValue(desArray[1],"#pathdata",pathdata);
		//CRCore::crArgumentParser::readKeyValue(desArray[2],"#speed",speed);

  //      PrimNodeRecord *pNodeRec = (PrimNodeRecord *)rec;
		//for(int i=0; i < pNodeRec->getNumChildren(); i++)
		//{
		//	Record* child = pNodeRec->getChild(i);

		//	if (child && child->isPrimaryNode())
		//	{
		//		switch (child->getOpcode())
		//		{
		//		case FACE_OP:
		//			{
		//				FaceRecord* fr = (FaceRecord*)child;
		//				if(strcmp(pathdata.c_str(),fr->getName().c_str()==0)
		//				{//路径节点
  //                          
		//				}
		//			}
		//			break;
		//		}
		//	}
		//}

		//return animationPath;

	}
	else if(str.find("#EventGroup") != -1)
	{
		nodetype = EVENTGROUP;
		CRCore::crEventGroup *eventGroup = new CRCore::crEventGroup;
		eventGroup->setName(group.getName());
		eventGroup->setDataVariance(CRCore::crBase::DYNAMIC);
		eventGroup->getDescriptions().insert(eventGroup->getDescriptions().end(),desArray.begin(),desArray.end());
		return eventGroup;
	}
	else if(str.find("#NotCalcIntersect") != -1)
	{
	    group.setEnableIntersect(false);
		return &group;
	}
	else if(str.find("#BoxMatterObject") != -1)
	{
		nodetype = MATTEROBJECT;
		CRPhysics::crBoxMatterObject *matterObject = new CRPhysics::crBoxMatterObject;
		matterObject->setName(group.getName());
		matterObject->setDataVariance(CRCore::crBase::DYNAMIC);
		matterObject->getDescriptions().insert(matterObject->getDescriptions().end(),desArray.begin(),desArray.end());
		return matterObject;
	}
	else if(str.find("#SphereMatterObject") != -1)
	{
		nodetype = MATTEROBJECT;
		CRPhysics::crSphereMatterObject *matterObject = new CRPhysics::crSphereMatterObject;
		matterObject->setName(group.getName());
		matterObject->setDataVariance(CRCore::crBase::DYNAMIC);
		matterObject->getDescriptions().insert(matterObject->getDescriptions().end(),desArray.begin(),desArray.end());
		return matterObject;
	}
	else if(str.find("#CCylinderMatterObject") != -1)
	{
		nodetype = MATTEROBJECT;
		CRPhysics::crCCylinderMatterObject *matterObject = new CRPhysics::crCCylinderMatterObject;
		matterObject->setName(group.getName());
		matterObject->setDataVariance(CRCore::crBase::DYNAMIC);
		matterObject->getDescriptions().insert(matterObject->getDescriptions().end(),desArray.begin(),desArray.end());
		return matterObject;
	}
	else if(str.find("#MeshMatterObject") != -1)
	{
		nodetype = MATTEROBJECT;
		CRPhysics::crMeshMatterObject *matterObject = new CRPhysics::crMeshMatterObject;
		matterObject->setName(group.getName());
		matterObject->setDataVariance(CRCore::crBase::DYNAMIC);
		matterObject->getDescriptions().insert(matterObject->getDescriptions().end(),desArray.begin(),desArray.end());
		return matterObject;
	}
	//else if(str.find("#RayMatterObject") != -1)
	//{
	//	nodetype = MATTEROBJECT;
	//	CRPhysics::crRayMatterObject *matterObject = new CRPhysics::crRayMatterObject;
	//	matterObject->setName(group.getName());
	//	matterObject->setDataVariance(CRCore::crBase::DYNAMIC);
	//	matterObject->getDescriptions().insert(matterObject->getDescriptions().end(),desArray.begin(),desArray.end());
	//	return matterObject;
	//}
	//else if(str.find("#WeaponMatterObject") != -1)
	//{
	//	nodetype = MATTEROBJECT;
	//	CRPhysics::crWeaponMatterObject *matterObject = new CRPhysics::crWeaponMatterObject;
	//	matterObject->setName(group.getName());
	//	matterObject->setDataVariance(CRCore::crBase::DYNAMIC);
	//	matterObject->getDescriptions().insert(matterObject->getDescriptions().end(),desArray.begin(),desArray.end());
	//	return matterObject;
	//}
	else if(str.find("#MatterGroup") != -1)
	{
		nodetype = MATTERGROUP;
		CRPhysics::crMatterGroup *matterGroup = new CRPhysics::crMatterGroup;
		matterGroup->setName(group.getName());
		matterGroup->setDataVariance(CRCore::crBase::DYNAMIC);
		matterGroup->getDescriptions().insert(matterGroup->getDescriptions().end(),desArray.begin(),desArray.end());

		int int1;
		if(CRCore::crArgumentParser::readKeyValue(matterGroup->getDescriptions(),"GeneralParamTransferPPC",int1) && int1)
		{
			matterGroup->addEventMsgMap(CRPhysics::crGeneralParamTransferPPC::makeEventMsg());
		}
		if(CRCore::crArgumentParser::readKeyValue(matterGroup->getDescriptions(),"JointSetParamPPC",int1) && int1)
		{
			matterGroup->addEventMsgMap(CRPhysics::crJointSetParamPPC::makeEventMsg());
		}
		if(CRCore::crArgumentParser::readKeyValue(matterGroup->getDescriptions(),"SetForceAndTorquePPC",int1) && int1)
		{
			matterGroup->addEventMsgMap(CRPhysics::crSetForceAndTorquePPC::makeEventMsg());
		}
		if(CRCore::crArgumentParser::readKeyValue(matterGroup->getDescriptions(),"SetGravityModePPC",int1) && int1)
		{
			matterGroup->addEventMsgMap(CRPhysics::crSetGravityModePPC::makeEventMsg());
		}
		if(CRCore::crArgumentParser::readKeyValue(matterGroup->getDescriptions(),"PhysicsResetPPC",int1) && int1)
		{
			matterGroup->addEventMsgMap(CRPhysics::crPhysicsResetPPC::makeEventMsg());
		}
		if(CRCore::crArgumentParser::readKeyValue(matterGroup->getDescriptions(),"WeaponEventPPC",int1) && int1)
		{
			matterGroup->addEventMsgMap(CRPhysics::crWeaponEventPPC::makeEventMsg());
		}
	/*	matterGroup->addEventMsgMap(CRPhysics::crJointSetParamPPC::makeEventMsg());
		matterGroup->addEventMsgMap(CRPhysics::crSetForceAndTorquePPC::makeEventMsg());
		matterGroup->addEventMsgMap(CRPhysics::crSetGravityModePPC::makeEventMsg());
		matterGroup->addEventMsgMap(CRPhysics::crPhysicsResetPPC::makeEventMsg());
		matterGroup->addEventMsgMap(CRPhysics::crSetGeomorBodyInfoPPC::makeEventMsg());*/
	    //matterGroup->addEventMsgMap(CRPhysics::crFXEventPPC::makeEventMsg());

		/*std::string temp;
		for( CRCore::crNode::DescriptionArray::iterator itr = desArray.begin();
			itr != desArray.end(); )
		{
			CRCore::crArgumentParser::readKeyValue(*itr++,"#Create",temp);
			if(temp.empty()) continue;
            
			if(temp.compare("NerveCell")==0)
			{
		        CRCore::ref_ptr<CRAI::crNerveCellGroup> nerveCell = new CRAI::crNerveCellGroup;
				CRCore::crBrain::getInstance()->getAITreeHead()->addChild(nerveCell.get());
				nerveCell->attachEventGroup(matterGroup);
				matterGroup->attachEventGroup(nerveCell.get());

				for(;(int)(itr->find("#CreateEnd"))<0; itr++)
				{
					nerveCell->addDescription(*itr);
					temp+='\n';
					temp+=*itr;
				}

				if(temp.find("CarManipulatorPPC")!=-1)
				{
					nerveCell->addEventMsgMap(CRAI::crCarManipulatorPPC::makeEventMsg());
				}

				//if(temp.find("RadarCollidePPC")!=-1)
				//{
				//	nerveCell->addEventMsgMap(CRAI::crRadarCollidePPC::makeEventMsg());
				//}
				//if(temp.find("RobotManipulatorPPC")!=-1)
				//{
				//	nerveCell->addEventMsgMap(CRAI::crRobotManipulatorPPC::makeEventMsg());
				//}
				if(temp.find("CharacterManipulatorPPC")!=-1)
				{
					nerveCell->addEventMsgMap(CRAI::crCharacterManipulatorPPC::makeEventMsg());
				}
			}
		}*/
        
	//	CRCore::ref_ptr<CRAI::crNerveCellGroup> test = new CRAI::crNerveCellGroup;
	////	test->addEventMsgMap(CRAI::crCarManipulatorPPC::makeEventMsg());
	//	test->attachEventGroup(matterGroup);
	//	matterGroup->attachEventGroup(test.get());
	//	CRCore::crBrain::getInstance()->getAITreeHead()->addChild(test.get());

		return matterGroup;
	}
	else if(str.find("#ExternNode") != -1)
	{
		nodetype = EXTERNNODE;
		CRCore::crExternNode *node = new CRCore::crExternNode;
		node->setName(group.getName());
		node->getDescriptions().insert(node->getDescriptions().end(),desArray.begin(),desArray.end());
		std::string filename;
		CRCore::crArgumentParser::readKeyValue(group.getDescriptions(),"FileName",filename);
        node->setfilename(filename);

		return node;
	}
	return &group;

}

void ConvertFromFLT::setCullFaceAndWireframe ( const SFace *pSFace,CRCore::crStateSet* outStateSet, DynGeoSet *dgset )
{
	//
	// Cull face & wireframe
	//

	//   outStateData->setDrawFlag(pSFace->swDrawFlag);

	switch(pSFace->swDrawFlag)
	{
	case FaceRecord::SOLID_BACKFACED:
		// Enable backface culling
	//		outStateSet->setMode(CRCore::crStateAttribute::VRMode(GL_CULL_FACE,NULL),CRCore::crStateAttribute::ON);
		//outStateData->setDrawFlag(CRCore::crStateSet::StateData::SOLID_BACKFACED);
		break;

	case FaceRecord::SOLID_NO_BACKFACE:
		// Disable backface culling
		outStateSet->setMode(GL_CULL_FACE,CRCore::crStateAttribute::OFF);
		//outStateData->setDrawFlag(CRCore::crStateSet::StateData::SOLID_NO_BACKFACE);
		break;

	case FaceRecord::WIREFRAME_NOT_CLOSED:
		//dgset->setPrimType(CRCore::crPrimitive::PT_LINE_STRIP);
		//outStateData->setDrawFlag(CRCore::crStateSet::StateData::WIREFRAME_NOT_CLOSED);
		{
		CRCore::crPolygonMode* polygonMode = new CRCore::crPolygonMode;
		polygonMode->setMode(CRCore::crPolygonMode::FRONT_AND_BACK,CRCore::crPolygonMode::LINE);
		outStateSet->setAttribute(polygonMode, CRCore::crStateAttribute::ON);
		}
		break;

	case FaceRecord::WIREFRAME_CLOSED:
		//dgset->setPrimType(CRCore::crPrimitive::PT_LINE_LOOP);
		//outStateData->setDrawFlag(CRCore::crStateSet::StateData::WIREFRAME_CLOSED);
		{
		CRCore::crPolygonMode* polygonMode = new CRCore::crPolygonMode;
		polygonMode->setMode(CRCore::crPolygonMode::FRONT_AND_BACK,CRCore::crPolygonMode::LINE);
		outStateSet->setAttribute(polygonMode, CRCore::crStateAttribute::ON);
		}
		break;

	case FaceRecord::OMNIDIRECTIONAL_LIGHT:
	case FaceRecord::UNIDIRECTIONAL_LIGHT:
	case FaceRecord::BIDIRECTIONAL_LIGHT:
		//dgset->setPrimType(CRCore::crPrimitive::PT_POINTS);
		//outStateData->setDrawFlag(CRCore::crStateSet::StateData::POINT);
		{
		CRCore::crPolygonMode* polygonMode = new CRCore::crPolygonMode;
		polygonMode->setMode(CRCore::crPolygonMode::FRONT_AND_BACK,CRCore::crPolygonMode::POINT);
		outStateSet->setAttribute(polygonMode, CRCore::crStateAttribute::ON);
		}
		break;
	}
}


void ConvertFromFLT::setDirectionalLight()
{
	/*
	TODO:

	int directionalLight = pSFace->swDrawFlag & (BIT3 | BIT4);
	switch(directionalLight)
	{
	case FaceRecord::OMNIDIRECTIONAL_LIGHT:
	break;
	case FaceRecord::UNIDIRECTIONAL_LIGHT:
	break;
	case FaceRecord::BIDIRECTIONAL_LIGHT:
	break;
	}
	*/
}


void ConvertFromFLT::setLightingAndColorBinding ( const FaceRecord *rec, const SFace *pSFace, CRCore::crStateSet* outStateSet, DynGeoSet *dgset )
{
	//
	// Lighting and color binding
	//
	CRCore::crStateAttribute::VRMode lightmode = GL_LIGHTING;
	if (rec->getFlightVersion() > 13)
	{
		switch(pSFace->swLightMode)
		{
		case FaceRecord::FACE_COLOR:
			// Use face color, not illuminated
			outStateSet->setMode( lightmode, CRCore::crStateAttribute::OFF );
			dgset->setColorBinding( /*osg::Geometry::BIND_OVERALL*/ CRCore::crGeometry::BIND_OVERALL );
			break;

		case FaceRecord::VERTEX_COLOR:
			// Use vertex colors, not illuminated
			outStateSet->setMode( lightmode, CRCore::crStateAttribute::OFF );
			dgset->setColorBinding( CRCore::crGeometry::BIND_PER_VERTEX );
			break;

		case FaceRecord::FACE_COLOR_LIGHTING:
			// Use face color and vertex normal
			dgset->setColorBinding( /*osg::Geometry::BIND_OVERALL*/ CRCore::crGeometry::BIND_PER_PRIMITIVE);
			dgset->setNormalBinding(CRCore::crGeometry::BIND_PER_VERTEX);
			break;

		case FaceRecord::VERTEX_COLOR_LIGHTING:
			// Use vertex color and vertex normal
			dgset->setColorBinding(CRCore::crGeometry::BIND_PER_VERTEX);
			dgset->setNormalBinding(CRCore::crGeometry::BIND_PER_VERTEX);
			break;

		default :
			outStateSet->setMode( lightmode, CRCore::crStateAttribute::OFF );
			dgset->setColorBinding(CRCore::crGeometry::BIND_OVERALL);
			break;
		}
	}
	else // Version 11, 12 & 13
	{
		outStateSet->setMode( lightmode, CRCore::crStateAttribute::OFF );
		dgset->setColorBinding(CRCore::crGeometry::BIND_OVERALL);
	}
////关闭光照
	//outStateData->setLightMode(CRCore::crStateSet::StateData::OFF);
}

void ConvertFromFLT::setColor ( FaceRecord *rec, SFace *pSFace, DynGeoSet *dgset, bool &bBlend)
{
	//
	// Face Color
	//

	if (pSFace->swTexWhite && (pSFace->iTexturePattern != -1))
	{
		// Render textured polygons white
		m_faceColor.set(1, 1, 1, 1);
	}
	else
	{
		ColorPool* pColorPool = rec->getFltFile()->getColorPool();
		m_faceColor.set(1, 1, 1, 1);

		if (rec->getFlightVersion() > 13)
		{
			if (!(pSFace->dwFlags & FaceRecord::NO_COLOR_BIT))
			{
				bool bPackedColor =
					m_bHdrRgbMode ||
					(pSFace->dwFlags & FaceRecord::PACKED_COLOR_BIT) ||
					(pColorPool == NULL);

				if (bPackedColor)
					m_faceColor = pSFace->PrimaryPackedColor.get();
				else
				{
					if (rec->getFlightVersion() >= 1540)
					{
						m_faceColor = 
							pColorPool->getColor(pSFace->dwPrimaryColorIndex);
					}
					else  // Version 14.2, 15.0
					{
						// The field now called wPrimaryNameIndex was
						// originally the primary color/intensity code
						// for OpenFlight v14.2 and v15.0 files
						m_faceColor = 
							pColorPool->getColor(pSFace->wPrimaryNameIndex);
					}
				}
			}
		}
		else // Version 11, 12 & 13
		{
			bool bPackedColor = m_bHdrRgbMode || (pColorPool == NULL);

			if (bPackedColor)
				m_faceColor = pSFace->PrimaryPackedColor.get();
			else
				m_faceColor = pColorPool->getOldColor(pSFace->wPrimaryNameIndex);
		}

	}

	// Face color alpha
	m_faceColor.set(1, 1, 1, 1);//disable face color 2008.7.18
	m_faceColor[3] = 1.0f - ((float)pSFace->wTransparency / 65535.0f);
	if (pSFace->wTransparency > 0)
	{
		bBlend = true;
		dgset->setAlpha(m_faceColor[3]);
	}

	if ((dgset->getColorBinding() == CRCore::crGeometry::BIND_OVERALL)
		||  (dgset->getColorBinding() == CRCore::crGeometry::BIND_PER_PRIMITIVE))
		dgset->addColor(m_faceColor);
}


void ConvertFromFLT::setMaterial ( FaceRecord *rec, SFace *pSFace, CRCore::crStateSet* outStateSet, bool &bBlend )
{
	//
	// crMaterial
	//

	MaterialPool* pMaterialPool = rec->getFltFile()->getMaterialPool();
	if (pMaterialPool)
	{
		float alpha;
		CRCore::crMaterial* outMaterial = pMaterialPool->getCreMaterial((int)pSFace->iMaterial);
		if(outMaterial)
		{
			outStateSet->setAttributeAndModes(outMaterial,CRCore::crStateAttribute::ON);
			alpha = outMaterial->getDiffuse(CRCore::crMaterial::FRONT).w();
			//if (alpha < 1.0f) bBlend = true;
		}
		else
		{
			MaterialPool::PoolMaterial* pSMaterial = pMaterialPool->getMaterial((int)pSFace->iMaterial);
			if (pSMaterial)
			{
				outMaterial= new CRCore::crMaterial;
				CRCore::crVector4 ambient;
				CRCore::crVector4 diffuse;
				CRCore::crVector4 specular;
				CRCore::crVector4 emissiv;

				// In contrast to the OpenFlight Specification this works!
				alpha = pSMaterial->sfAlpha;/* * 
											(1.0f - ((float)pSFace->wTransparency / 65535.0f));*//* *
											(1.0f - ((float)m_wObjTransparency / 65535.0f));*/

				ambient[0] = pSMaterial->Ambient[0] * m_faceColor[0];
				ambient[1] = pSMaterial->Ambient[1] * m_faceColor[1];
				ambient[2] = pSMaterial->Ambient[2] * m_faceColor[2];
				ambient[3] = alpha;

				diffuse[0] = pSMaterial->Diffuse[0] * m_faceColor[0];
				diffuse[1] = pSMaterial->Diffuse[1] * m_faceColor[1];
				diffuse[2] = pSMaterial->Diffuse[2] * m_faceColor[2];
				diffuse[3] = alpha;

				specular[0] = pSMaterial->Specular[0];
				specular[1] = pSMaterial->Specular[1];
				specular[2] = pSMaterial->Specular[2];
				specular[3] = alpha;

				emissiv[0] = pSMaterial->Emissive[0];
				emissiv[1] = pSMaterial->Emissive[1];
				emissiv[2] = pSMaterial->Emissive[2];
				emissiv[3] = 1.0f - ((float)m_wObjTransparency / 65535.0f);//alpha;

				outMaterial->setColorMode(CRCore::crMaterial::OFF);//由OFF该成EMISSION 
				outMaterial->setAmbient(CRCore::crMaterial::FRONT_AND_BACK, CRCore::crVector4(0.0f,0.0f,0.0f,1.0f));
				outMaterial->setDiffuse(CRCore::crMaterial::FRONT_AND_BACK, CRCore::crVector4(0.0f,0.0f,0.0f,1.0f));
				outMaterial->setSpecular(CRCore::crMaterial::FRONT_AND_BACK, CRCore::crVector4(0.0f,0.0f,0.0f,1.0f));
				outMaterial->setEmission(CRCore::crMaterial::FRONT_AND_BACK, CRCore::crVector4(0.0f,0.0f,0.0f,1.0f));
				//outMaterial->setAlpha(CRCore::crMaterial::FRONT_AND_BACK, alpha);
				outMaterial->setShininess(CRCore::crMaterial::FRONT_AND_BACK, 0.0f/*pSMaterial->sfShininess*/);

				//int matid = CRCore::crBrain::getInstance()->getResource()->findOrInsertMaterial((int)pSFace->iMaterial,outMaterial);
				//outStateSet->setAttributeAndModes(outMaterial,CRCore::crStateAttribute::ON);
				outStateSet->setAttribute(outMaterial,CRCore::crStateAttribute::ON);

				pMaterialPool->addCreMaterial((int)pSFace->iMaterial,outMaterial);
				//if (alpha < 1.0f) bBlend = true;
			}
		}
	}
}


void ConvertFromFLT::setTexture ( FaceRecord *rec, SFace *pSFace, CRCore::crStateSet* outStateSet, DynGeoSet *dgset, bool &bBlend )
{
	//
	// Texture
	//

	if (pSFace->iTexturePattern != -1)
	{
		TexturePool* pTexturePool = rec->getFltFile()->getTexturePool();
		if (pTexturePool)
		{
			int nIndex = (int)pSFace->iTexturePattern;
			flt::AttrData *textureAttrData = pTexturePool->getTexture(nIndex,rec->getFlightVersion());

			CRCore::crStateSet *textureStateSet;
			if (textureAttrData)
				textureStateSet = textureAttrData->stateset;
			else
				textureStateSet = NULL;

			if (textureStateSet)
			{
				//We got detail texture, so we got detailTexture stateset and add a crTexEnvCombine attribute
				// To add simple detail texture we just use texture unit 1 to store detail
				//As Creators help says, if Mag. Filter General is set to Modulate Detail we just
				//add the detail, but if it set to Add Detail then we got a lighter image, so we
				//use scale_rgb and scale_alpha of CRCore::crTexEnvCombine to make this effect
				// Julian Ortiz, June 18th 2003.

				flt::AttrData *detailTextureAttrData = NULL;                                
				if (pSFace->iDetailTexturePattern != -1) 
				{                 
					int nIndex2 = (int)pSFace->iDetailTexturePattern;
					detailTextureAttrData = pTexturePool->getTexture(nIndex2,rec->getFlightVersion());
					if (detailTextureAttrData && detailTextureAttrData->stateset) 
					{
						 CRCore::crTexture2D *detTexture = dynamic_cast<CRCore::crTexture2D*>(detailTextureAttrData->stateset->getTextureAttribute( 0, CRCore::crStateAttribute::TEXTURE));		
						 textureStateSet->setTextureAttributeAndModes(1,detTexture,CRCore::crStateAttribute::ON);  
					     CRCore::crTexEnvCombine *tec1 = new CRCore::crTexEnvCombine;
					     float scale = (detailTextureAttrData->modulateDetail==0)?2.0f:4.0f;                     
					     tec1->setScale_RGB(scale);
					     tec1->setScale_Alpha(scale);
						 textureStateSet->setTextureAttribute( 1, tec1,CRCore::crStateAttribute::ON );   
					}                 
				}

				//Now, an ugly thing,... we have detected that in Creator we defined that a texture will we used as
				//detail texture, and we load it as it using texture unit 1,... but we also need to create texture 
				//coordinates to map this detail texture, I found that texture coordinates assigment is made in
				//DynGeoSet::addToGeometry and the easy way I found to create those new coordinates is to add a method
				//to DynGeoSet class named setDetailTextureStatus that pass detail Texture AttrData class, so when 
				//DynGeoSet::addToGeometry runs it reads this class and create new texture coordinates if we got a valid
				//AttrData object. I now this is not a good way to do it, and I expect someone with more CRCore knowledge
				//could make it in a better way.
				// Julian Ortiz, June 18th 2003.                     
				if (pSFace->iDetailTexturePattern != -1 && detailTextureAttrData && detailTextureAttrData->stateset)
					dgset->setDetailTextureAttrData(detailTextureAttrData);
				else 
					dgset->setDetailTextureAttrData(NULL);

				// Merge face stateset with texture stateset
				 outStateSet->merge(*textureStateSet);

				// Alpha channel in texture?
				//CRCore::crTexture2D *outTexture = dynamic_cast<CRCore::crTexture2D*>(CRCore::crBrain::getInstance()->
				//	getResource()->findOrLoadTexture(textureStateSet->getTextureID(0)));
				 ///////////不根据贴图进行透明判断
				 //CRCore::crTexture2D *outTexture = dynamic_cast<CRCore::crTexture2D *>(textureStateSet->getTextureAttribute(0,CRCore::crStateAttribute::TEXTURE));
				//if (outTexture)
				//{
				//	CRCore::crImage* outImage = outTexture->getImage();
				//	if (getUseTextureAlphaForTransparancyBinning() && outImage->isImageTranslucent()) bBlend = true;

				//}

				// Started to experiment with OpenFlight texture mapping modes
				//           if (pSFace->iTextureMapIndex > -1)
				//           {
				//// Visit ancillary records
				//for(int i=0; i < rec->getNumChildren(); i++)
				//{
				//	Record* child = rec->getChild(i);
				//	if (!child->isAncillaryRecord())
				//		break;

				//	switch (child->getOpcode())
				//	{
				//	case TEXTURE_MAPPING_PALETTE_OP:
				//		{
				//			TextureMappingPaletteRecord* tmpr =
				//				dynamic_cast<TextureMappingPaletteRecord*>(child);
				//			if (!tmpr)
				//			{
				//				CRCore::notify( CRCore::WARN ) << "flt::ConvertFromFLT::visitFace(GeoSetBuilder*, FaceRecord*) found invalid TextureMappingPaletteRecord*"<<std::endl;
				//				return;
				//			}
				//			struct STextureMapping
				//			{
				//				SRecHeader    RecHeader;
				//				float32     reserved;           // Reserved
				//				int32       diIndex;            // Texture mapping index
				//				char        szName[20];         // Texture mapping name
				//				int32       diType;             // Texture mapping type
				//				//  0 = None
				//				//  1 = Put
				//				//  2 = 4 Point Put
				//				//  3 = Reserved
				//				//  4 = Spherical Project
				//				//  5 = Radial Project
				//				//  6 = Reserved
				//				int32       diWarpFlag;         // Warped flag; if TRUE, 8 point warp applied
				//				float64     dfMat[4][4];        // Transformation matrix (valid only for Types 1 & 2)
				//				//  Variable    Variable;           // Parameters (see below for parameters for each mapping type)
				//			};

				//                           STextureMapping* stm = reinterpret_cast<STextureMapping*>(tmpr->getData());
				//                           if (stm==0)
				//			{
				//                         CRCore::notify(CRCore::WARN)<<"flt::ConvertFromFLT::visitFace(GeoSetBuilder*, FaceRecord*) stm->getData() not valid STextureMapping*."<<std::endl;
				//                         return;
				//			}
				//                           switch(stm->diType) 
				//			{
				//                           case 0: //None
				//                           	break;
				//                           case 1: //Put
				//				{    enum Mode {
				//					OBJECT_LINEAR,
				//					EYE_LINEAR,
				//					SPHERE_MAP,
				//					NORMAL_MAP,
				//					REFLECTION_MAP};
				//					/// additional texgen coefficents for GL_OBJECT_PLANE or GL_EYE_PLANE,
				//					crPlane m_plane_s, m_plane_t, m_plane_r, m_plane_q;

				//				    SPutTextureMapping *sput = (SPutTextureMapping *)stm;
				//					CRCore::crTexGen* outTexGen = new CRCore::crTexGen;
				//					outTexGen->setMode(CRCore::crTexGen::SPHERE_MAP);
				//					outStateData->getTex(0).texGen = outTexGen;

				//                           	    break;
				//				}
				//                           case 2: //4 Point Put
				//				{
				//					SPutTextureMapping *sput = (SPutTextureMapping *)stm;
				//					break;
				//				}
				//				break;
				//			case 3: //Reserved
				//				break;
				//			case 4: //Spherical Project
				//				break;
				//			case 5: //Radial Project
				//				break;
				//                           case 6: //Reserved
				//                           	break;

				//			}


				//		}
				//		break;
				//	}
				//}


				//           }

				dgset->setTextureBinding(CRCore::crGeometry::BIND_PER_VERTEX);
			}
		}
	}
}


void ConvertFromFLT::setTransparency ( CRCore::crStateSet* outStateSet, bool &bBlend )
{
	//
	// Transparency
	//

	if (bBlend)
	{
		//if(!outStateSet->getAttribute(CRCore::crStateAttribute::ALPHAFUNC))
		//{
			CRCore::crBlendFunc* outBlendFunc = new CRCore::crBlendFunc();
			outBlendFunc->setFunction(CRCore::crBlendFunc::SRC_ALPHA, CRCore::crBlendFunc::ONE_MINUS_SRC_ALPHA);
			outStateSet->setAttribute(outBlendFunc);
			outStateSet->setMode(GL_BLEND, CRCore::crStateAttribute::ON);
			outStateSet->setRenderingHint(CRCore::crStateSet::TRANSPARENT_BIN);
			//CRCore::notify(CRCore::WARN)<<"GL_BLEND "<< std::endl;
		//}
		//else
		//{
		//	CRCore::notify(CRCore::WARN)<<"ALPHAFUNC "<< std::endl;
		//}
	}
}

void  ConvertFromFLT::createCupMap( const std::string *texName, unsigned int uint, CRCore::crStateSet* stateset )
{    
	int i = texName->find("B2");
	if(i<0) return;
    
	std::string cupmapName;
	i = texName->find_last_of(".");
	cupmapName.insert(0,texName->c_str(),i);

	CRCore::crTextureCubeMap* cubemap;
	CupMap::iterator itr = s_cupMap.find(cupmapName);
    if(itr!=s_cupMap.end()) 
	{
		cubemap = itr->second.get();
		stateset->setTextureAttributeAndModes(uint, cubemap);

		CRCore::crTexGen* texgen = new CRCore::crTexGen;
		texgen->setMode(CRCore::crTexGen::NORMAL_MAP);
		stateset->setTextureAttributeAndModes(uint,texgen);

		CRCore::crTexEnv*texEnv = new CRCore::crTexEnv;
		texEnv->setMode(CRCore::crTexEnv::MODULATE);
		stateset->setTextureAttributeAndModes(uint,texEnv);
		return;
	}

	CRCore::crImage* imagePosX = get_loadImage(CUBEMAP_FILENAME(cupmapName,posx,"jpg"));
	CRCore::crImage* imageNegX = get_loadImage(CUBEMAP_FILENAME(cupmapName,negx,"jpg"));
	CRCore::crImage* imagePosY = get_loadImage(CUBEMAP_FILENAME(cupmapName,posy,"jpg"));
	CRCore::crImage* imageNegY = get_loadImage(CUBEMAP_FILENAME(cupmapName,negy,"jpg"));
	CRCore::crImage* imagePosZ = get_loadImage(CUBEMAP_FILENAME(cupmapName,posz,"jpg"));
	CRCore::crImage* imageNegZ = get_loadImage(CUBEMAP_FILENAME(cupmapName,negz,"jpg"));

	if (imagePosX && imageNegX && imagePosY && imageNegY && imagePosZ && imageNegZ)
	{
		cubemap = new CRCore::crTextureCubeMap;
		s_cupMap[cupmapName] = cubemap;

		cubemap->setImage(CRCore::crTextureCubeMap::POSITIVE_X, imagePosX);
		cubemap->setImage(CRCore::crTextureCubeMap::NEGATIVE_X, imageNegX);
		cubemap->setImage(CRCore::crTextureCubeMap::POSITIVE_Y, imagePosY);
		cubemap->setImage(CRCore::crTextureCubeMap::NEGATIVE_Y, imageNegY);
		cubemap->setImage(CRCore::crTextureCubeMap::POSITIVE_Z, imagePosZ);
		cubemap->setImage(CRCore::crTextureCubeMap::NEGATIVE_Z, imageNegZ);
		
		cubemap->setImageNameID(CRCore::crArgumentParser::getSimpleFileName(cubemap->getCupMapNameID()));

		cubemap->setWrap(CRCore::crTexture::WRAP_S, CRCore::crTexture::CLAMP_TO_EDGE);
		cubemap->setWrap(CRCore::crTexture::WRAP_T, CRCore::crTexture::CLAMP_TO_EDGE);
		cubemap->setWrap(CRCore::crTexture::WRAP_R, CRCore::crTexture::CLAMP_TO_EDGE);

		cubemap->setFilter(CRCore::crTexture::MIN_FILTER, CRCore::crTexture::LINEAR_MIPMAP_LINEAR);
		cubemap->setFilter(CRCore::crTexture::MAG_FILTER, CRCore::crTexture::LINEAR);

		//int texid = CRCore::crBrain::getInstance()->getResource()->findOrInsertTexture(0,cupmapName,cubemap);
	    stateset->setTextureAttributeAndModes(uint, cubemap);
        
		//if(texid>-1)
		{
		    CRCore::crTexGen* texgen = new CRCore::crTexGen;
		    texgen->setMode(CRCore::crTexGen::NORMAL_MAP);
		    stateset->setTextureAttributeAndModes(uint,texgen);

			CRCore::crTexEnv*texEnv = new CRCore::crTexEnv;
		    texEnv->setMode(CRCore::crTexEnv::MODULATE);
			stateset->setTextureAttributeAndModes(uint,texEnv);
		}

	}			
}

void  ConvertFromFLT::createSpecularHighlights( std::string &comment, unsigned int uint, CRCore::crStateSet *stateset )
{
	if(comment.find("#SpecularHighlights")==-1) return;
	CRCore::crVector3 direction;
	CRCore::crVector4 color;
	float exponent;
	CRCore::crArgumentParser::readKeyValue(comment,"light_direction",direction);
	CRCore::crArgumentParser::readKeyValue(comment,"light_color",color);
	CRCore::crArgumentParser::readKeyValue(comment,"specular_exponent",exponent);

	// create and setup the texture object
	CRCore::crTextureCubeMap *tcm = new CRCore::crTextureCubeMap;
	tcm->setWrap(CRCore::crTexture::WRAP_S, CRCore::crTexture::CLAMP);
	tcm->setWrap(CRCore::crTexture::WRAP_T, CRCore::crTexture::CLAMP);
	tcm->setWrap(CRCore::crTexture::WRAP_R, CRCore::crTexture::CLAMP);
	tcm->setFilter(CRCore::crTexture::MIN_FILTER, CRCore::crTexture::LINEAR_MIPMAP_LINEAR);
	tcm->setFilter(CRCore::crTexture::MAG_FILTER, CRCore::crTexture::LINEAR);    

	// generate the six highlight map images (light direction = [1, 1, -1])
	CRUtil::crHighlightMapGenerator *mapgen = new CRUtil::crHighlightMapGenerator(
		direction,            // light direction
		color,    // light color
		exponent); // specular exponent

	mapgen->generateMap(false);

	// assign the six images to the texture object
	tcm->setImage(CRCore::crTextureCubeMap::POSITIVE_X, mapgen->getImage(CRCore::crTextureCubeMap::POSITIVE_X));
	tcm->setImage(CRCore::crTextureCubeMap::NEGATIVE_X, mapgen->getImage(CRCore::crTextureCubeMap::NEGATIVE_X));
	tcm->setImage(CRCore::crTextureCubeMap::POSITIVE_Y, mapgen->getImage(CRCore::crTextureCubeMap::POSITIVE_Y));
	tcm->setImage(CRCore::crTextureCubeMap::NEGATIVE_Y, mapgen->getImage(CRCore::crTextureCubeMap::NEGATIVE_Y));
	tcm->setImage(CRCore::crTextureCubeMap::POSITIVE_Z, mapgen->getImage(CRCore::crTextureCubeMap::POSITIVE_Z));
	tcm->setImage(CRCore::crTextureCubeMap::NEGATIVE_Z, mapgen->getImage(CRCore::crTextureCubeMap::NEGATIVE_Z));

	//int texid = CRCore::crBrain::getInstance()->getResource()->findOrInsertTexture(0,"SpecularHighlights",tcm);
    stateset->setTextureAttributeAndModes(uint,tcm);

//	if(texid>-1) 
	{
		CRCore::crTexGen* texgen = new CRCore::crTexGen;
		texgen->setMode(CRCore::crTexGen::NORMAL_MAP);
		stateset->setTextureAttributeAndModes(uint,texgen);

		CRCore::crTexEnv*texEnv = new CRCore::crTexEnv;
		texEnv->setMode(CRCore::crTexEnv::MODULATE);
		stateset->setTextureAttributeAndModes(uint,texEnv);
	}
}

namespace
{
	CRCore::crImage *create_default_anisotropicLighting_image()
	{
		const int texture_size = 16;
		CRCore::ref_ptr<CRCore::crImage> image = new CRCore::crImage;
		image->setImage(texture_size, texture_size, 1, 3, GL_RGB, GL_UNSIGNED_BYTE, new unsigned char[3*texture_size*texture_size], CRCore::crImage::USE_NEW_DELETE);
		for (int i=0; i<texture_size; ++i) 
		{
			for (int j=0; j<texture_size; ++j) 
			{
				float s = static_cast<float>(j) / (texture_size-1);
				float t = static_cast<float>(i) / (texture_size-1);
				float lum = t * 0.75f;
				float red = lum + 0.2f * powf(cosf(s*10), 3.0f);
				float green = lum;
				float blue = lum + 0.2f * powf(sinf(s*10), 3.0f);
				if (red > 1) red = 1;
				if (red < 0) red = 0;
				if (blue > 1) blue = 1;
				if (blue < 0) blue = 0;
				*(image->data(j, i)+0) = static_cast<unsigned char>(red * 255);
				*(image->data(j, i)+1) = static_cast<unsigned char>(green * 255);
				*(image->data(j, i)+2) = static_cast<unsigned char>(blue * 255);
			}
		}
		return image.take();
	}
}

//void  ConvertFromFLT::createAnisotropicLighting( int lightnum, unsigned int uint, CRCore::crStateSet *stateset)
//{
//	//	if(comment.find("#AnisotropicLighting")==-1) return;
//
//	std::ostringstream vp_oss;
//	vp_oss <<
//		"!!ARBvp1.0\n"
//		"PARAM c5 = { 0, 0, 0, 1 };"
//		"PARAM c4 = { 0, 0, 0, 0 };"
//		"TEMP R0, R1, R2, R3, R4, R5, R6, R7, R8, R9;"
//		"ATTRIB v18 = vertex.normal;"
//		"ATTRIB v16 = vertex.position;"
//		"PARAM s259[4] = { state.matrix.mvp };"
//		"PARAM s18 = state.light[" << lightnum << "].position;"
//		"PARAM s223[4] = { state.matrix.modelview[0] };"
//		"PARAM c0[4] = { program.local[0..3] };"
//		"    DP4 result.position.x, s259[0], v16;"
//		"    DP4 result.position.y, s259[1], v16;"
//		"    DP4 result.position.z, s259[2], v16;"
//		"    DP4 result.position.w, s259[3], v16;"
//		"    MOV R9, c0[0];"
//		"    MUL R0, R9.y, s223[1];"
//		"    MAD R0, R9.x, s223[0], R0;"
//		"    MAD R0, R9.z, s223[2], R0;"
//		"    MAD R8, R9.w, s223[3], R0;"
//		"    DP4 R0.x, R8, v16;"
//		"    MOV R7, c0[1];"
//		"    MUL R1, R7.y, s223[1];"
//		"    MAD R1, R7.x, s223[0], R1;"
//		"    MAD R1, R7.z, s223[2], R1;"
//		"    MAD R6, R7.w, s223[3], R1;"
//		"    DP4 R0.y, R6, v16;"
//		"    MOV R5, c0[2];"
//		"    MUL R1, R5.y, s223[1];"
//		"    MAD R1, R5.x, s223[0], R1;"
//		"    MAD R1, R5.z, s223[2], R1;"
//		"    MAD R4, R5.w, s223[3], R1;"
//		"    DP4 R0.z, R4, v16;"
//		"    MOV R3, c0[3];"
//		"    MUL R1, R3.y, s223[1];"
//		"    MAD R1, R3.x, s223[0], R1;"
//		"    MAD R1, R3.z, s223[2], R1;"
//		"    MAD R1, R3.w, s223[3], R1;"
//		"    DP4 R0.w, R1, v16;"
//		"    MOV R1.x, R9.w;"
//		"    MOV R1.y, R7.w;"
//		"    MOV R1.z, R5.w;"
//		"    MOV R1.w, R3.w;"
//		"    ADD R2, R1, -R0;"
//		"    DP4 R0.x, R2, R2;"
//		"    RSQ R1.x, R0.x;"
//		"    DP4 R0.x, R9, s18;"
//		"    DP4 R0.y, R7, s18;"
//		"    DP4 R0.z, R5, s18;"
//		"    DP4 R0.w, R3, s18;"
//		"    DP4 R1.y, R0, R0;"
//		"    RSQ R1.y, R1.y;"
//		"    MUL R3, R1.y, R0;"
//		"    MAD R2, R1.x, R2, R3;"
//		"    DP4 R1.x, R2, R2;"
//		"    RSQ R1.x, R1.x;"
//		"    MUL R1, R1.x, R2;"
//		"    DP3 R2.x, R8.xyzx, v18.xyzx;"
//		"    DP3 R2.y, R6.xyzx, v18.xyzx;"
//		"    DP3 R2.z, R4.xyzx, v18.xyzx;"
//		"    MOV R2.w, c4.x;"
//		"    DP4 R1.x, R1, R2;"
//		"    MAX result.texcoord[" << uint << "].x, R1.x, c4.x;"
//		"    DP4 R0.x, R0, R2;"
//		"    MAX result.texcoord[" << uint << "].y, R0.x, c4.x;"
//		"END\n";
//
//	CRCore::crVertexProgram *vertexProgram = new CRCore::crVertexProgram;
//	vertexProgram->setVertexProgram(vp_oss.str());
//	stateset->setAttributeAndModes(vertexProgram);
//
//	CRCore::crViewMatrixExtractor *viewMatrixExtractor = new CRCore::crViewMatrixExtractor(vertexProgram, 0);
//	stateset->setAttributeAndModes(viewMatrixExtractor);
//
//	// create and setup the texture object
//	CRCore::crTexture2D *texture = new CRCore::crTexture2D;
//	texture->setImage(create_default_anisotropicLighting_image());
//	texture->setWrap(CRCore::crTexture::WRAP_S, CRCore::crTexture::CLAMP);
//	texture->setWrap(CRCore::crTexture::WRAP_T, CRCore::crTexture::CLAMP);
//	texture->setInternalFormatMode(CRCore::crTexture::USE_IMAGE_DATA_FORMAT);
//
//	//int texid = CRCore::crBrain::getInstance()->getResource()->findOrInsertTexture(0,"AnisotropicLighting",texture);
//	stateset->setTextureAttributeAndModes(uint,texture);
////	if(texid>-1) 
//	{
//		CRCore::crTexEnv *texEnv = new CRCore::crTexEnv;
//		texEnv->setMode(CRCore::crTexEnv::ADD);
//		stateset->setTextureAttributeAndModes(uint,texEnv);
//	}
//}
//
//void ConvertFromFLT::createBumpMapping_Normal( int normalunit, std::string &comment, CRCore::crStateSet *stateset )
//{
//	if(comment.find("#BumpMapping")==-1) return;
//	int diffuseunit = normalunit + 1;
//	int lightnum = 0;
//
//	CRCore::crArgumentParser::readKeyValue(comment,"light_num",lightnum);
//	CRCore::crArgumentParser::readKeyValue(comment,"scale",tex.scale);
//
//	// first pass, diffuse bump
//	{
//		std::ostringstream vp_oss;
//		vp_oss <<
//			"!!ARBvp1.0\n"
//			"OPTION ARB_position_invariant;"
//			"PARAM c0 = { 0.5, 1, 0, 0 };"
//			"TEMP R0, R1, R2;"
//			"ATTRIB v5 = vertex.attrib[15];"
//			"ATTRIB v4 = vertex.attrib[7];"
//			"ATTRIB v3 = vertex.attrib[6];"
//			"ATTRIB v24 = vertex.texcoord[" << normalunit << "];"
//			"ATTRIB v25 = vertex.texcoord[" << diffuseunit << "];"
//			"ATTRIB v18 = vertex.normal;"
//			"ATTRIB v16 = vertex.position;"
//			"PARAM s259[4] = { state.matrix.mvp };"
//			"PARAM s18 = state.light[" << lightnum << "].position;"
//			"PARAM s223[4] = { state.matrix.modelview[0] };"
//			"    MOV result.texcoord[" << diffuseunit << "].xy, v25;"
//			"    MOV result.texcoord[" << normalunit << "].xy, v24;"
//			"    DP3 R0.y, s223[0].xyzx, v3.xyzx;"
//			"    DP3 R0.z, s223[1].xyzx, v3.xyzx;"
//			"    DP3 R0.w, s223[2].xyzx, v3.xyzx;"
//			"    DP3 R0.x, s18.xyzx, s18.xyzx;"
//			"    RSQ R0.x, R0.x;"
//			"    MUL R2.xyz, R0.x, s18.xyzx;"
//			"    DP3 R1.x, R0.yzwy, R2.xyzx;"
//			"    DP3 R0.x, s223[0].xyzx, v4.xyzx;"
//			"    DP3 R0.y, s223[1].xyzx, v4.xyzx;"
//			"    DP3 R0.z, s223[2].xyzx, v4.xyzx;"
//			"    DP3 R1.y, R0.xyzx, R2.xyzx;"
//			"    DP3 R0.x, s223[0].xyzx, v5.xyzx;"
//			"    DP3 R0.y, s223[1].xyzx, v5.xyzx;"
//			"    DP3 R0.z, s223[2].xyzx, v5.xyzx;"
//			"    DP3 R1.z, R0.xyzx, R2.xyzx;"
//			"    MAD result.color.front.primary.xyz, c0.x, R1.xyzx, c0.x;"
//			"    MOV result.color.front.primary.w, c0.y;"
//			"END\n";
//
//
//		tex.vertexProgram = new CRCore::crVertexProgram;
//		tex.vertexProgram->setVertexProgram(vp_oss.str());
//
//	    tex.texEnvCombine = new CRCore::crTexEnvCombine;
//		tex.texEnvCombine->setCombine_RGB(CRCore::crTexEnvCombine::DOT3_RGB);
//		tex.texEnvCombine->setSource0_RGB(CRCore::crTexEnvCombine::PRIMARY_COLOR);
//		tex.texEnvCombine->setSource1_RGB(CRCore::crTexEnvCombine::TEXTURE);
//		tex.texEnv = NULL;
//		tex.bumpMapFlg = CRCore::crStateSet::StateData::Tex::NORMAL;
//
//	}
//
//}

//void ConvertFromFLT::createBumpMapping_Normal( std::string &comment, unsigned int normalunit, CRCore::crStateSet *stateset )
//{
//	if(comment.find("#BumpMapping")==-1) return;
//	int diffuseunit = normalunit + 1;
//	int lightnum = 0;
//	float scale;
//    
//	CRCore::crArgumentParser::readKeyValue(comment,"light_num",lightnum);
//	CRCore::crArgumentParser::readKeyValue(comment,"scale",scale);
//    int T = 6;
//	int B = 7;
//	int N = 15;
//    int freeunit = 7;
//  //  for (freeunit=0; freeunit==diffuseunit||freeunit==normalunit; ++freeunit) {}
//
//    // vertex program
//    std::ostringstream vp_oss;
//    vp_oss <<
//        "!!ARBvp1.0\n"
//        "OPTION ARB_position_invariant;"
//        "PARAM c4 = { 0, 0, 0, 1 };"
//        "PARAM c5 = { 0.5, 4, 0, 0 };"
//        "TEMP R0, R1, R2, R3, R4, R5, R6, R7, R8;"
//        "ATTRIB v5 = vertex.attrib[" << N << "];"
//        "ATTRIB v4 = vertex.attrib[" << B << "];"
//        "ATTRIB v3 = vertex.attrib[" << T << "];"
//        "ATTRIB v25 = vertex.texcoord[" << diffuseunit << "];"
//        "ATTRIB v24 = vertex.texcoord[" << normalunit << "];"
//        "ATTRIB v18 = vertex.normal;"
//        "ATTRIB v16 = vertex.position;"
//        "PARAM s259[4] = { state.matrix.mvp };"
//        "PARAM s18 = state.light[0].position;"
//        "PARAM s77 = state.lightprod[0].specular;"
//        "PARAM s4 = state.material.shininess;"
//        "PARAM s75 = state.lightprod[0].ambient;"
//        "PARAM s223[4] = { state.matrix.modelview[0] };"
//        "PARAM c0[4] = { program.local[0..3] };"
//        "    MOV result.texcoord[" << freeunit << "].xyz, s75.xyzx;"
//        "    MOV result.texcoord[" << freeunit << "].w, s4.x;"
//        "    MOV result.texcoord[" << normalunit << "].zw, s77.zwzw;"
//        "    MOV result.texcoord[" << normalunit << "].xy, v24;"
//        "    MOV result.texcoord[" << diffuseunit << "].zw, s77.xyxy;"
//        "    MOV result.texcoord[" << diffuseunit << "].xy, v25;"
//        "    MOV R5, c0[0];"
//        "    MUL R0, R5.y, s223[1];"
//        "    MAD R0, R5.x, s223[0], R0;"
//        "    MAD R0, R5.z, s223[2], R0;"
//        "    MAD R0, R5.w, s223[3], R0;"
//        "    DP4 R1.x, R0, v16;"
//        "    MOV R4, c0[1];"
//		"    MUL R2, R4.y, s223[1];"
//		"    MAD R2, R4.x, s223[0], R2;"
//		"    MAD R2, R4.z, s223[2], R2;"
//		"    MAD R7, R4.w, s223[3], R2;"
//		"    DP4 R1.y, R7, v16;"
//		"    MOV R3, c0[2];"
//		"    MUL R2, R3.y, s223[1];"
//		"    MAD R2, R3.x, s223[0], R2;"
//		"    MAD R2, R3.z, s223[2], R2;"
//		"    MAD R6, R3.w, s223[3], R2;"
//		"    DP4 R1.z, R6, v16;"
//		"    MOV R2, c0[3];"
//		"    MUL R8, R2.y, s223[1];"
//		"    MAD R8, R2.x, s223[0], R8;"
//		"    MAD R8, R2.z, s223[2], R8;"
//		"    MAD R8, R2.w, s223[3], R8;"
//		"    MOV R8.x, R5.w;"
//		"    MOV R8.y, R4.w;"
//		"    MOV R8.z, R3.w;"
//		"    ADD R1.yzw, R8.xxyz, -R1.xxyz;"
//		"    DP3 R1.x, R1.yzwy, R1.yzwy;"
//		"    RSQ R1.x, R1.x;"
//		"    DP4 R5.x, R5, s18;"
//		"    DP4 R5.y, R4, s18;"
//		"    DP4 R5.z, R3, s18;"
//		"    DP3 R2.x, R5.xyzx, R5.xyzx;"
//		"    RSQ R2.x, R2.x;"
//		"    MUL R5.xyz, R2.x, R5.xyzx;"
//		"    MAD R1.yzw, R1.x, R1.yyzw, R5.xxyz;"
//		"    DP3 R1.x, R1.yzwy, R1.yzwy;"
//		"    RSQ R1.x, R1.x;"
//		"    MUL R4.xyz, R1.x, R1.yzwy;"
//		"    DP3 R3.x, R0.xyzx, v3.xyzx;"
//		"    DP3 R3.y, R7.xyzx, v3.xyzx;"
//		"    DP3 R3.z, R6.xyzx, v3.xyzx;"
//		"    DP3 R8.x, R3.xyzx, R4.xyzx;"
//		"    DP3 R2.x, R0.xyzx, v4.xyzx;"
//		"    DP3 R2.y, R7.xyzx, v4.xyzx;"
//		"    DP3 R2.z, R6.xyzx, v4.xyzx;"
//		"    DP3 R8.y, R2.xyzx, R4.xyzx;"
//		"    DP3 R1.x, R0.xyzx, v5.xyzx;"
//		"    DP3 R1.y, R7.xyzx, v5.xyzx;"
//		"    DP3 R1.z, R6.xyzx, v5.xyzx;"
//		"    DP3 R8.z, R1.xyzx, R4.xyzx;"
//		"    MAD result.color.front.secondary.xyz, c5.x, R8.xyzx, c5.x;"
//		"    DP3 R0.y, R0.xyzx, v18.xyzx;"
//		"    DP3 R0.z, R7.xyzx, v18.xyzx;"
//		"    DP3 R0.w, R6.xyzx, v18.xyzx;"
//		"    DP3 R0.x, R0.yzwy, R0.yzwy;"
//		"    RSQ R0.x, R0.x;"
//		"    MUL R6.xyz, R0.x, R0.yzwy;"
//		"    DP3 R0.x, R6.xyzx, R4.xyzx;"
//		"    MUL result.color.front.secondary.w, c5.y, R0.x;"
//		"    DP3 R0.x, R3.xyzx, R5.xyzx;"
//		"    DP3 R0.y, R2.xyzx, R5.xyzx;"
//		"    DP3 R0.z, R1.xyzx, R5.xyzx;"
//		"    MAD result.color.front.primary.xyz, c5.x, R0.xyzx, c5.x;"
//		"    DP3 R0.x, R6.xyzx, R5.xyzx;"
//		"    MUL result.color.front.primary.w, c5.y, R0.x;"
//		"END\n";
//
//	// fragment program
//	std::ostringstream fp_oss;
//	fp_oss <<
//		"!!ARBfp1.0\n"
//		"PARAM c0 = {1, 2, 0.5, 0};"
//		"PARAM c1 = {0, 0, 0, 1};"
//		"TEMP R0;"
//		"TEMP R1;"
//		"TEMP R2;"
//		"TEX R0, fragment.texcoord[" << normalunit << "], texture[" << normalunit << "], 2D;"
//		"TEX R1, fragment.texcoord[" << diffuseunit << "], texture[" << diffuseunit << "], 2D;"
//		"ADD R0, R0, -c0.z;"
//		"MUL R0.xyz, c0.y, R0;"
//		"ADD R2.xyz, fragment.color.primary, -c0.z;"
//		"MUL R2.xyz, c0.y, R2;"
//		"DP3_SAT R0.w, R0, R2;"
//		"ADD R2, fragment.color.secondary, -c0.z;"
//		"MUL R2.xyz, c0.y, R2;"
//		"DP3_SAT R0.x, R0, R2;"
//		"POW R0.x, R0.x, fragment.texcoord[" << freeunit << "].w;"
//		"MOV R2.xyz, fragment.texcoord[" << freeunit << "].xyyx;"
//		"MOV R2.w, c1.w;"
//		"MOV_SAT R0.y, fragment.color.primary.w;"
//		"MUL R0.w, R0.y, R0.w;"
//		"ADD R2, R2, R0.w;"
//		"MUL R1.xyz, R1, R2;"
//		"MOV_SAT R0.y, fragment.color.secondary.w;"
//		"MUL R0.xyz, R0.y, R0.x;"
//		"MOV R2.xy, fragment.texcoord[" << diffuseunit << "].zwzz;"
//		"MOV R2.z, fragment.texcoord[" << normalunit << "].z;"
//		"MUL R2.xyz, R0, R2;"
//		"ADD R2.xyz, R1, R2;"
//		"MOV result.color.xyz, R2;"
//		"MOV result.color.w, c0.x;"
//		"END\n";
//
//
//	CRCore::crVertexProgram *vertexProgram = new CRCore::crVertexProgram;
//	vertexProgram->setVertexProgram(vp_oss.str());
//	stateset->setAttributeAndModes(vertexProgram);
//
//	CRCore::crFragmentProgram *fragmentProgram = new CRCore::crFragmentProgram;
//	fragmentProgram->setFragmentProgram(fp_oss.str());
//	stateset->setAttributeAndModes(fragmentProgram);
//
//	CRCore::crViewMatrixExtractor *viewMatrixExtractor = new CRCore::crViewMatrixExtractor(vertexProgram, 0);
//	stateset->setAttributeAndModes(viewMatrixExtractor);
//	//tex.texEnv = NULL;
//	//tex.bumpMapFlg = CRCore::crStateSet::StateData::Tex::NORMAL;
//
//}
//
//void ConvertFromFLT::createBumpMapping_Diffuse( std::string &comment, unsigned int diffuseunit, CRCore::crStateSet *stateset )
//{
//	if(comment.find("#BumpMapping")==-1) return;
//	int normalunit = diffuseunit - 1;
//    int lightnum = 0;
//    
//	float scale;
//
//	CRCore::crArgumentParser::readKeyValue(comment,"light_num",lightnum);
//	CRCore::crArgumentParser::readKeyValue(comment,"scale",scale);
//
//	//tex.bumpMapFlg = CRCore::crStateSet::StateData::Tex::DIFFUSE;
//	////tex.texEnv = NULL;
//	//tex.texEnv = new CRCore::crTexEnv;
//	//tex.texEnv->setMode(CRCore::crTexEnv::MODULATE);
//
//	// second pass, self-shadowing
//	bool selfShadowing = false; 
//	if (selfShadowing)
//	{
//		std::ostringstream vp_oss;
//		vp_oss <<
//			"!!ARBvp1.0\n"
//			"OPTION ARB_position_invariant;"
//			"PARAM c0 = { 8, 0, 1, 0 };"
//			"TEMP R0;"
//			"ATTRIB v18 = vertex.normal;"
//			"ATTRIB v16 = vertex.position;"
//			"PARAM s259[4] = { state.matrix.mvp };"
//			"PARAM s18 = state.light[" << lightnum << "].position;"
//			"PARAM s631[4] = { state.matrix.modelview[0].invtrans };"
//			"    DP4 R0.x, s631[0], v18;"
//			"    DP4 R0.y, s631[1], v18;"
//			"    DP4 R0.z, s631[2], v18;"
//			"    DP3 R0.x, R0.xyzx, s18.xyzx;"
//			"    MAX R0.x, R0.x, c0.y;"
//			"    MUL R0.x, c0.x, R0.x;"
//			"    MIN result.color.front.primary.xyz, R0.x, c0.z;"
//			"    MOV result.color.front.primary.w, c0.z;"
//			"END\n";
//
//
//		CRCore::ref_ptr<CRCore::crDepth> depth = new CRCore::crDepth;
//		depth->setFunction(CRCore::crDepth::EQUAL);
//		stateset->setAttributeAndModes(depth.get(), CRCore::crStateAttribute::OVERRIDE|CRCore::crStateAttribute::ON);
//
//		CRCore::crVertexProgram *vertexProgram = new CRCore::crVertexProgram;
//		vertexProgram->setVertexProgram(vp_oss.str());
//		stateset->setAttributeAndModes(vertexProgram);
//
//		CRCore::ref_ptr<CRCore::crBlendFunc> bf = new CRCore::crBlendFunc;
//		bf->setFunction(CRCore::crBlendFunc::DST_COLOR, CRCore::crBlendFunc::ZERO);                
//		stateset->setAttributeAndModes(bf.get(), CRCore::crStateAttribute::OVERRIDE|CRCore::crStateAttribute::ON);
//
//		stateset->setTextureMode(diffuseunit, GL_TEXTURE_2D, CRCore::crStateAttribute::OVERRIDE|CRCore::crStateAttribute::OFF);
//		stateset->setTextureMode(normalunit, GL_TEXTURE_2D, CRCore::crStateAttribute::OVERRIDE|CRCore::crStateAttribute::OFF);
//	}
//
//}
//
void ConvertFromFLT::addMultiTexture( DynGeoSet* dgset, MultiTextureRecord* mtr )
{
	CRCore::crGeometry* geom = dgset->getGeometry();

	if (geom==0 || mtr==0 || !mtr->isAncillaryRecord())
	{
		CRCore::notify(CRCore::WARN)<<"ConvertFromFLT::addMultiTexture(DynGeoSet*, MultiTextureRecord*) has been passed invalid paramters."<<std::endl;
		return;
	}

	SMultiTexture* mt = reinterpret_cast<SMultiTexture*>(mtr->getData());
	if (mt==0)
	{
		CRCore::notify(CRCore::WARN)<<"ConvertFromFLT::addMultiTexture(DynGeoSet*, MultiTextureRecord*) mtr->getData() not valid SMultiTexture*."<<std::endl;
		return;
	}


	//CERR << "ConvertFromFLT::addMultiTexture\n";
	CRCore::crStateSet *geom_stateset = geom->getStateSet();
	//CRCore::crResource *resource = CRCore::crBrain::getInstance()->getResource();
	AttrData* temp_attrData = NULL;
	int l = 0;
	for ( int i = 0; i < 8; i++ )
	{
		if ( (1 << (32-i)) & mt->layers )
		{
			//CERR << "Has layer " << i << "\n";
			mt->data[l].endian();
			//CERR << "texture: " << mt->data[l].texture << "\n";
			//CERR << "effect: " << mt->data[l].effect << "\n";
			//CERR << "mapping: " << mt->data[l].mapping << "\n";
			//CERR << "data: " << mt->data[l].data << "\n";

			TexturePool* pTexturePool = mtr->getFltFile()->getTexturePool();

			assert( pTexturePool );
			if (!pTexturePool)
			{
				CRCore::notify(CRCore::WARN)<<"ConvertFromFLT::addMultiTexture(DynGeoSet*, MultiTextureRecord*) pTexturePool invalid."<<std::endl;
				l++;
				continue;
			}

			AttrData* temp_attrData = pTexturePool->getTexture((int)mt->data[l].texture,mtr->getFlightVersion());
			if(!temp_attrData) 
			{
				l++;			
				continue;
			}

			CRCore::crStateSet *textureStateSet = dynamic_cast<CRCore::crStateSet *> (temp_attrData->stateset);

			//CERR << "pTexturePool->getTexture((int)mt->data[l].texture): " << pTexturePool->getTexture((int)mt->data[l].texture,mtr->getFlightVersion()) << "\n";
			//CERR << "textureStateSet: " << textureStateSet << "\n";

			if (!textureStateSet)
			{
				//CERR << "unable to set up multi-texture layer." << std::endl;
				l++;
				continue;
			}

			CRCore::crTexture *texture = dynamic_cast<CRCore::crTexture2D*>(textureStateSet->getTextureAttribute(0, CRCore::crStateAttribute::TEXTURE));
			//CERR << "texture: " << texture << "\n";

			if (!texture)
			{
				CRCore::notify(CRCore::WARN)<<"ConvertFromFLT::addMultiTexture(DynGeoSet*, MultiTextureRecord*) texture invalid."<<std::endl;
				l++;
				continue;
			}
			switch(texture->getTextureTarget())
			{
			case GL_TEXTURE_2D:
			case GL_TEXTURE_1D:
			case GL_TEXTURE_3D:
				break;
			case GL_TEXTURE_CUBE_MAP:
			case GL_TEXTURE_RECTANGLE_NV:
				geom_stateset->setTextureAttributeAndModes(i,texture);
				l++;
				continue;
			}

			//switch(mt->data[l].effect) 
			//{
			//case 0: //texture env 
			//	switch(mt->data[l].data)
			//	{
			//	case 0:
			//		{//cupmap
			//			std::string *texName = pTexturePool->getTextureName((int)mt->data[l].texture);
			//			createCupMap( texName, i, geom_stateset );
			//			continue;
			//		}
			//		break;
			//	case 100:
			//		createSpecularHighlights( temp_attrData->comment, i, geom_stateset );
			//		continue;
			//		break;
			//	}
			//	break;
			//case 1: //bump map
			//	//if(mt->data[l].data == 0)     
			//	//{//m_normalunit
   // //                createBumpMapping_Normal( temp_attrData->comment, i, geom_stateset );
			//	//}
			//	//else if(mt->data[l].data == 1)
			//	//{//m_diffuseunit
			//	//	createBumpMapping_Diffuse( temp_attrData->comment, i, geom_stateset );
			//	//}
			//	break;
			//case 101:
			//	{
			//		//if(!temp_tex.texEnv.valid()&&!temp_tex.texEnvCombine.valid())
			//		//{
			//		//	temp_tex.texEnv = new CRCore::crTexEnv;
			//		//	temp_tex.texEnv->setMode(CRCore::crTexEnv::MODULATE);
			//		//}
			//	}
   //             break;
			////case  111: // custom 自定义贴图处理
			////	{//cupmap
			////		std::string *texName = pTexturePool->getTextureName((int)mt->data[l].texture);
			////		createCupMap( texName, temp_tex );
			////	}
			////	break;
			////case 112:
			////	createSpecularHighlights( temp_attrData->comment, temp_tex );
			////	break;
			//case 111:
			//	createAnisotropicLighting( mt->data[l].data, i, geom_stateset );
			//	continue;
			//	break;
			//}
			geom_stateset->setTextureAttributeAndModes(i,texture);
			CRCore::crStateAttribute* texenv_0 = textureStateSet->getTextureAttribute( 0, CRCore::crStateAttribute::TEXENV );
			if (texenv_0)
			{
				geom_stateset->setTextureAttributeAndModes( i, texenv_0);
			}
			else
			{
				CRCore::crTexEnv* texEnv = new CRCore::crTexEnv;
				texEnv->setMode(CRCore::crTexEnv::MODULATE);
				geom_stateset->setTextureAttributeAndModes( i, texEnv );
			}
          
			//geom_statedata->addTex(temp_tex);

			l++;
		}
	}

	//geom_stateset->getTexArray().reserve(geom_statedata->getTexArray().size());
}

void ConvertFromFLT::addUVList( DynGeoSet* dgset, UVListRecord* uvr )
{
	CRCore::crGeometry* geom = dgset->getGeometry();

	if (!geom || !uvr || !uvr->isAncillaryRecord())
	{
		CRCore::notify(CRCore::WARN)<<"ConvertFromFLT::addUVList( DynGeoSet*, UVListRecord*) has been passed invalid paramters."<<std::endl;
		return;
	}

	SUVList* uvl = reinterpret_cast<SUVList*>(uvr->getData());
	if (!uvl)
	{
		CRCore::notify(CRCore::WARN)<<"ConvertFromFLT::addUVList( DynGeoSet*, UVListRecord*) uvr->getData() is invalid."<<std::endl;
		return;
	}

	//CERR << "ConvertFromFLT::addUVList\n";
	int l = 0;
	int num_coords = dgset->coordListSize();
	int num_tcoords = 0;
	for(int i = 0; i< 8; i++)
	{
		if(1<<(32-i)&uvl->layers)
            num_tcoords++;
	}
	for ( int i = 0; i < 8; i++ )
	{
		if ( (1 << (32-i)) & uvl->layers )
		{
			CRCore::Vec2Array* tcoords = new CRCore::Vec2Array;
			//CERR << "Has layer " << i << "\n";
			// Assume we are working with vertex lists for now
			for ( int v = l; v < num_tcoords*num_coords; v+=num_tcoords )
			{
				uvl->coords.vertex[v].endian();
				//CERR << "( u: " << uvl->coords.vertex[v].coords[1] << ", "
				//	<< "v: " << uvl->coords.vertex[v].coords[0] << ")\n";
				/// FIXME: should be (x,y) instead of (y,x) - ENDIAN problem???
				tcoords->push_back( CRCore::crVector2( uvl->coords.vertex[v].coords[1],
					uvl->coords.vertex[v].coords[0] ) );
			}
			if ( !tcoords->empty() )
			{
				//CERR << "Setting tcoords " << i << ": " << tcoords << "\n";
				geom->setTexCoordArray( i, tcoords );
				//geom->addTexCoordArray(tcoords);
			}

			l++;
		}
	}
}

bool ConvertFromFLT::visitFace(GeoSetBuilder* pBuilder, CRCore::crGroup& outParent, FaceRecord* rec, bool dontMerge)
{
	DynGeoSet* dgset = pBuilder->getDynGeoSet();
	CRCore::crStateSet* outStateSet = dgset->getStateSet();
	SFace *pSFace = (SFace*)rec->getData();
	bool bBlend = false;

	if (rec->getFlightVersion() > 13)
	{
		if (pSFace->dwFlags & FaceRecord::HIDDEN_BIT) //隐藏面不加载
			return false;
	}

	// Various properties.
	setCullFaceAndWireframe ( pSFace, outStateSet, dgset );
	setDirectionalLight();
	setLightingAndColorBinding ( rec, pSFace, outStateSet, dgset );
	setColor ( rec, pSFace, dgset, bBlend );
	setMaterial ( rec, pSFace, outStateSet, bBlend );

	// Subface
	if (rec->getParent()->isOfType(FACE_OP))
	{
		if (m_nSubfaceLevel > 0)
		{
			CRCore::crPolygonOffset* polyoffset = new CRCore::crPolygonOffset;
			if (polyoffset)
			{
				polyoffset->setFactor(-1.0f*m_nSubfaceLevel);
				polyoffset->setUnits(-20.0f*m_nSubfaceLevel);
				outStateSet->setAttributeAndModes(polyoffset,CRCore::crStateAttribute::ON);
			}
		}
	}

	// Texture.
	setTexture ( rec, pSFace, outStateSet, dgset, bBlend );

	// Transparency
	setTransparency ( outStateSet, bBlend );

	// Vertices
	addVertices(pBuilder, outParent, rec);

	// Visit ancillary records
	for(int i=0; i < rec->getNumChildren(); i++)
	{
		Record* child = rec->getChild(i);
		if (!child->isAncillaryRecord())
			break;

		switch (child->getOpcode())
		{
		case MULTI_TEXTURE_OP:
			{
				MultiTextureRecord* mtr =
					dynamic_cast<MultiTextureRecord*>(child);
				if (!mtr)
				{
					CRCore::notify( CRCore::WARN ) << "flt::ConvertFromFLT::visitFace(GeoSetBuilder*, FaceRecord*) found invalid MultiTextureRecord*"<<std::endl;
					return bBlend;
				}

				// original code, but causes crash becayse addPrimitive can invalidate teh dgset pointer.
				// addMultiTexture( dgset, mtr );

				addMultiTexture( dgset, mtr );
			}
			break;

		default:

#ifdef _DEBUG

			//CRCore::notify( CRCore::NOTICE) << "flt::ConvertFromFLT::visitFace: "
			//	<< "Unhandled opcode: " << child->getOpcode() << "\n";
#endif

			break;
		}
	}

	// Add face to builder pool
	pBuilder->addPrimitive(rec->getName(),dontMerge/*||bBlend*/);

	// Look for subfaces
	{
		m_nSubfaceLevel++;
		int n;
		for(n=0; n<rec->getNumChildren(); n++)
		{
			Record* child = rec->getChild(n);

			if (child && child->isOfType(FACE_OP))
				visitFace(pBuilder, outParent, (FaceRecord*)child,dontMerge);
		}
		m_nSubfaceLevel--;
	}

	return bBlend;
}

/* C.Holtz:  These global variables are to support the REPLICATE ancillary record
used for lightpoint strings in 15.7 (it really should support replication of any
node, but I really only needed lightpoint strings and it's such a hack, I don't
want to propagate it anywhere is doesn't really need to be :) */
static CRCore::crMatrix theMatrix ;
static CRCore::crMatrix theGeneralMatrix ;
static CRCore::crVector3 from, delta ;
static int num_replicate ;
static int got_gm, got_m, got_t, got_replicate ;

// Return 1 if record is a known vertex record else return 0.
int ConvertFromFLT::addOccluderVerticie(CRCore::crConvexPlanarPolygon &cpp, Record* rec)
{

	int i ;

	DPRINT(stderr, ">>> addVertex...") ;

	switch(rec->getOpcode())
	{
	case VERTEX_C_OP:
		DPRINT(stderr, "VERTEX_C_OP\n") ;
		{
			SVertex* pVert = (SVertex*)rec->getData();
			CRCore::crVector3 coord(pVert->Coord.x(), pVert->Coord.y(), pVert->Coord.z());
			coord *= (float)m_unitScale;
			cpp.add(coord);
			if ( got_replicate ) 
			{
				/* Handle vertex replication */
				DPRINT(stderr, "      ### addVertex: Replicating (%f,%f,%f) %d times...\n", 
					coord[0], coord[1], coord[2], num_replicate) ;
				for ( i = 0 ; i < num_replicate ; i++ ) 
				{
					if ( got_t ) 
					{
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						cpp.add(coord);
					} 
					else 
					{
						/* If we didn't get a translate record, try to get the delta from the matrix */
						delta = theMatrix.getTrans() ;
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						cpp.add(coord);
					}
				}
			}
		}
		break;

	case VERTEX_CN_OP:
		DPRINT(stderr, "VERTEX_CN_OP\n") ;
		{
			SNormalVertex* pVert = (SNormalVertex*)rec->getData();
			CRCore::crVector3 coord(pVert->Coord.x(), pVert->Coord.y(), pVert->Coord.z());
			coord *= (float)m_unitScale;
			cpp.add(coord);
			if ( got_replicate ) 
			{
				/* Handle vertex replication */
				DPRINT(stderr, "      ### addVertex: Replicating (%f,%f,%f) %d times...\n", 
					coord[0], coord[1], coord[2], num_replicate) ;
				for ( i = 0 ; i < num_replicate ; i++ ) 
				{
					if ( got_t ) 
					{
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						cpp.add(coord);
					} 
					else 
					{
						/* If we didn't get a translate record, try to get the delta from the matrix */
						delta = theMatrix.getTrans() ;
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						cpp.add(coord);
					}
				}
			}
		}
		break;

	case VERTEX_CNT_OP:
		DPRINT(stderr, "VERTEX_CNT_OP\n") ;
		{
			SNormalTextureVertex* pVert = (SNormalTextureVertex*)rec->getData();
			CRCore::crVector3 coord(pVert->Coord.x(), pVert->Coord.y(), pVert->Coord.z());
			coord *= (float)m_unitScale;
			cpp.add(coord);
			if ( got_replicate ) 
			{
				/* Handle vertex replication */
				DPRINT(stderr, "      ### addVertex: Replicating (%f,%f,%f) %d times...\n", 
					coord[0], coord[1], coord[2], num_replicate) ;
				for ( i = 0 ; i < num_replicate ; i++ ) 
				{
					if ( got_t ) 
					{
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						cpp.add(coord);
					} 
					else 
					{
						/* If we didn't get a translate record, try to get the delta from the matrix */
						delta = theMatrix.getTrans() ;
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						cpp.add(coord);
					}
				}
			}
		}
		break;

	case VERTEX_CT_OP:
		DPRINT(stderr, "VERTEX_CT_OP\n") ;
		{
			STextureVertex* pVert = (STextureVertex*)rec->getData();
			CRCore::crVector3 coord(pVert->Coord.x(), pVert->Coord.y(), pVert->Coord.z());
			coord *= (float)m_unitScale;
			cpp.add(coord);
			if ( got_replicate ) 
			{
				/* Handle vertex replication */
				DPRINT(stderr, "      ### addVertex: Replicating (%f,%f,%f) %d times...\n", 
					coord[0], coord[1], coord[2], num_replicate) ;
				for ( i = 0 ; i < num_replicate ; i++ ) 
				{
					if ( got_t ) 
					{
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						cpp.add(coord);
					} 
					else 
					{
						/* If we didn't get a translate record, try to get the delta from the matrix */
						delta = theMatrix.getTrans() ;
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						cpp.add(coord);
					}
				}
			}
		}
		break;

	case OLD_VERTEX_OP:
		DPRINT(stderr, "OLD_VERTEX_OP\n") ;
		{
			SOldVertex* pVert = (SOldVertex*)rec->getData();
			CRCore::crVector3 coord(pVert->v[0], pVert->v[1], pVert->v[2]);
			coord *= (float)m_unitScale;
			cpp.add(coord);
			if ( got_replicate ) 
			{
				/* Handle vertex replication */
				DPRINT(stderr, "      ### addVertex: Replicating (%f,%f,%f) %d times...\n", 
					coord[0], coord[1], coord[2], num_replicate) ;
				for ( i = 0 ; i < num_replicate ; i++ ) 
				{
					if ( got_t ) 
					{
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						cpp.add(coord);
					} 
					else 
					{
						/* If we didn't get a translate record, try to get the delta from the matrix */
						delta = theMatrix.getTrans() ;
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						cpp.add(coord);
					}
				}
			}
		}
		break;

	case OLD_VERTEX_COLOR_OP:
		DPRINT(stderr, "OLD_VERTEX_COLOR_OP\n") ;
		{
			SOldVertexColor* pVert = (SOldVertexColor*)rec->getData();
			CRCore::crVector3 coord(pVert->v[0], pVert->v[1], pVert->v[2]);
			coord *= (float)m_unitScale;
			cpp.add(coord);
			if ( got_replicate ) 
			{
				/* Handle vertex replication */
				DPRINT(stderr, "      ### addVertex: Replicating (%f,%f,%f) %d times...\n", 
					coord[0], coord[1], coord[2], num_replicate) ;
				for ( i = 0 ; i < num_replicate ; i++ ) 
				{
					if ( got_t ) 
					{
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						cpp.add(coord);
					} 
					else 
					{
						/* If we didn't get a translate record, try to get the delta from the matrix */
						delta = theMatrix.getTrans() ;
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						cpp.add(coord);
					}
				}
			}
		}
		break;

	case OLD_VERTEX_COLOR_NORMAL_OP:
		DPRINT(stderr, "OLD_VERTEX_COLOR_NORMAL_OP\n") ;
		{
			SOldVertexColorNormal* pVert = (SOldVertexColorNormal*)rec->getData();
			CRCore::crVector3 coord(pVert->v[0], pVert->v[1], pVert->v[2]);
			coord *= (float)m_unitScale;
			cpp.add(coord);
			if ( got_replicate ) 
			{
				/* Handle vertex replication */
				DPRINT(stderr, "      ### addVertex: Replicating (%f,%f,%f) %d times...\n", 
					coord[0], coord[1], coord[2], num_replicate) ;
				for ( i = 0 ; i < num_replicate ; i++ ) 
				{
					if ( got_t )
					{
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						cpp.add(coord);
					} 
					else 
					{
						/* If we didn't get a translate record, try to get the delta from the matrix */
						delta = theMatrix.getTrans() ;
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						cpp.add(coord);
					}
				}
			}
		}
		break;

	default :
		DPRINT(stderr, "*** UNKNOWN (%d)***\n", rec->getOpcode()) ;
		return 0;
	}

	/* Clear out the replicate stuff so that it doesn't get reused if we
	return to this function from somewhere other than addVerticies() */
	got_gm = got_m = got_t = got_replicate = 0 ;

	return 1;
}

// Return number of vertices added to builder.
int ConvertFromFLT::addOccluderVerticies( CRCore::crConvexPlanarPolygon &cpp, PrimNodeRecord* primRec )
{
	int i;
	int vertices=0;

	/* Clear the replicate stuff each time through */
	got_gm = got_m = got_t = got_replicate = 0 ;

	DPRINT(stderr, ">>> addVerticies...%d children\n", primRec->getNumChildren()) ;
	CRCore::crVector3 pos;
	SMatrix *pSMatrix = NULL;
	SGeneralMatrix *pSGMatrix = NULL;
	for(i=0; i < primRec->getNumChildren(); i++)
	{
		Record* child = primRec->getChild(i);
		if (child == NULL) break;

		DPRINT(stderr, "     child opcode = %d\n", child->getOpcode()) ;

		switch (child->getOpcode())
		{
		case VERTEX_LIST_OP:
			{
				VertexListRecord* rec = (VertexListRecord*) child;
				int n = rec->numberOfVertices();
				// Add vertices to GeoSetBuilder
				for (int j=0; j < n; j++)
				{
					Record* vertex = getVertexFromPool(rec->getVertexPoolOffset(j));
					if (vertex)
						addOccluderVerticie(cpp, vertex);
				}
				vertices += n;
			}
			break;

		case LOCAL_VERTEX_POOL_OP:
			m_currentLocalVertexPool = (LocalVertexPoolRecord *)child;
			break;

		case TRANSLATE_OP:
			if (1) 
			{
				// This will be for replicated verticies
				STranslate *pSTranslate = ((TranslateRecord *)child)->getData() ;
				// scale position.
				from.set(pSTranslate->From[0],pSTranslate->From[1],pSTranslate->From[2]) ;
				from *= m_unitScale ;
				delta.set(pSTranslate->Delta[0],pSTranslate->Delta[1],pSTranslate->Delta[2]) ;
				delta *= m_unitScale ;
				DPRINT(stderr, "   ** addVerticies: Got Translate: F=%lf, %lf, %lf / D=%lf, %lf, %lf\n",
					from[0], from[1], from[2], delta[0], delta[1], delta[2]) ;
				got_t = 1 ;
			}
			break ;

		case MATRIX_OP:
			// This will be for replicated verticies
			pSMatrix = ((MatrixRecord *)child)->getData() ;
			for(int i=0;i<4;++i)
			{
				for(int j=0;j<4;++j)
				{
					theMatrix(i,j) = pSMatrix->sfMat[i][j];
				}
			}
			// scale position.
			pos = theMatrix.getTrans();
			theMatrix *= CRCore::crMatrix::translate(-pos);
			pos *= (float)m_unitScale;
			theMatrix *= CRCore::crMatrix::translate(pos);
			//if(dprint)std::cout << "   ** addVerticies: Got crMatrix: " << theMatrix << std::endl ;
			got_m = 1 ;
			break ;

		case GENERAL_MATRIX_OP:
			// This will be for replicated verticies
			pSGMatrix = ((GeneralMatrixRecord *)child)->getData() ;
			for(int i=0;i<4;++i)
			{
				for(int j=0;j<4;++j)
				{
					theGeneralMatrix(i,j) = pSMatrix->sfMat[i][j];
				}
			}
			// scale position.
			pos = theGeneralMatrix.getTrans();
			theGeneralMatrix *= CRCore::crMatrix::translate(-pos);
			pos *= (float)m_unitScale;
			theGeneralMatrix *= CRCore::crMatrix::translate(pos);
			//if(dprint)std::cout << "   ** addVerticies: Got GeneralMatrix: " << theGeneralMatrix << std::endl ;
			got_gm = 1 ;
			break ;

		case REPLICATE_OP:
			{
				// This will be for replicated verticies
				SReplicate *pSReplicate = (SReplicate *)(child->getData())  ;
				ENDIAN(pSReplicate->iNumber) ;
				num_replicate = pSReplicate->iNumber ;
				DPRINT(stderr, "   ** addVerticies: Got Replicate: %d times\n", num_replicate) ;
				got_replicate = 1 ;
			}
			break ;

		case LIGHT_POINT_OP:
			break ;

		default :
			vertices += addOccluderVerticie(cpp, child);
			break;
		}
	}
	return vertices;
}
// Return number of vertices added to builder.
int ConvertFromFLT::addVertices(GeoSetBuilder* pBuilder, CRCore::crGroup& outParent, PrimNodeRecord* primRec)
{
	int i;
	int vertices=0;
	DynGeoSet* dgset = pBuilder->getDynGeoSet();

	/* Clear the replicate stuff each time through */
	got_gm = got_m = got_t = got_replicate = 0 ;

	DPRINT(stderr, ">>> addVerticies...%d children\n", primRec->getNumChildren()) ;
	CRCore::crVector3 pos;
	SMatrix *pSMatrix = NULL;
	SGeneralMatrix *pSGMatrix = NULL;
	for(i=0; i < primRec->getNumChildren(); i++)
	{
		Record* child = primRec->getChild(i);
		if (child == NULL) break;

		DPRINT(stderr, "     child opcode = %d\n", child->getOpcode()) ;

		switch (child->getOpcode())
		{
		case VERTEX_LIST_OP:
			vertices += visitVertexList(pBuilder, (VertexListRecord*)child);
			break;

		case LOCAL_VERTEX_POOL_OP:
			vertices += visitLocalVertexPool(pBuilder, (LocalVertexPoolRecord *)child);
			break;

		case TRANSLATE_OP:
			if (1) 
			{
				// This will be for replicated verticies
				STranslate *pSTranslate = ((TranslateRecord *)child)->getData() ;
				// scale position.
				from.set(pSTranslate->From[0],pSTranslate->From[1],pSTranslate->From[2]) ;
				from *= m_unitScale ;
				delta.set(pSTranslate->Delta[0],pSTranslate->Delta[1],pSTranslate->Delta[2]) ;
				delta *= m_unitScale ;
				DPRINT(stderr, "   ** addVerticies: Got Translate: F=%lf, %lf, %lf / D=%lf, %lf, %lf\n",
					from[0], from[1], from[2], delta[0], delta[1], delta[2]) ;
				got_t = 1 ;
			}
			break ;

		case MATRIX_OP:
			// This will be for replicated verticies
			pSMatrix = ((MatrixRecord *)child)->getData() ;
			for(int i=0;i<4;++i)
			{
				for(int j=0;j<4;++j)
				{
					theMatrix(i,j) = pSMatrix->sfMat[i][j];
				}
			}
			// scale position.
			pos = theMatrix.getTrans();
			theMatrix *= CRCore::crMatrix::translate(-pos);
			pos *= (float)m_unitScale;
			theMatrix *= CRCore::crMatrix::translate(pos);
			//if(dprint)std::cout << "   ** addVerticies: Got crMatrix: " << theMatrix << std::endl ;
			got_m = 1 ;
			break ;

		case GENERAL_MATRIX_OP:
			// This will be for replicated verticies
			pSGMatrix = ((GeneralMatrixRecord *)child)->getData() ;
			for(int i=0;i<4;++i)
			{
				for(int j=0;j<4;++j)
				{
					theGeneralMatrix(i,j) = pSMatrix->sfMat[i][j];
				}
			}
			// scale position.
			pos = theGeneralMatrix.getTrans();
			theGeneralMatrix *= CRCore::crMatrix::translate(-pos);
			pos *= (float)m_unitScale;
			theGeneralMatrix *= CRCore::crMatrix::translate(pos);
			//if(dprint)std::cout << "   ** addVerticies: Got GeneralMatrix: " << theGeneralMatrix << std::endl ;
			got_gm = 1 ;
			break ;

		case REPLICATE_OP:
			{
				// This will be for replicated verticies
				SReplicate *pSReplicate = (SReplicate *)(child->getData())  ;
				ENDIAN(pSReplicate->iNumber) ;
				num_replicate = pSReplicate->iNumber ;
				DPRINT(stderr, "   ** addVerticies: Got Replicate: %d times\n", num_replicate) ;
				got_replicate = 1 ;
			}
			break ;

		case LIGHT_POINT_OP:
			/* Apparently, lightpoints are allowed to be clildren
			of faces in older versions (<15.4?) */
			DPRINT(stderr, "   ** addVerticies: Got LIGHT_POINT_OP\n") ;
			visitLightPoint(outParent, (LightPointRecord*)child) ;
			break ;

		default :
			vertices += addVertex(pBuilder, child);
			break;
		}
	}

	if (vertices > 0)
	{
		if (dgset->getPrimType() == CRCore::crPrimitive::PT_POINTS)
		{
			for (i=0; i < vertices; i++)
				dgset->addPrimLen(1);
		}
		else
		{
			dgset->addPrimLen(vertices);
		}
	}

	return vertices;
}


int ConvertFromFLT::visitVertexList(GeoSetBuilder* pBuilder, VertexListRecord* rec)
{
	DynGeoSet* dgset = pBuilder->getDynGeoSet();
	int vertices = rec->numberOfVertices();

	DPRINT(stderr, ">>> visitVertexList...%d vertices\n", vertices) ;
	// Add vertices to GeoSetBuilder
	for (int j=0; j < vertices; j++)
	{
		Record* vertex = getVertexFromPool(rec->getVertexPoolOffset(j));
		if (vertex)
			addVertex(pBuilder, vertex);
	}

	// Visit ancillary records
	for(int i=0; i < rec->getNumChildren(); i++)
	{
		Record* child = rec->getChild(i);
		//CERR << "OPCODE: " << child->getOpcode() << "\n";

		if (!child->isAncillaryRecord())
			break;

		switch (child->getOpcode())
		{
		case UV_LIST_OP:
			{
				UVListRecord* uvr =
					dynamic_cast<UVListRecord*>(child);
				assert( uvr );
				addUVList( dgset, uvr );
			}
			break;
		case MULTI_TEXTURE_OP:
			{
				CERR2 << "MULTI_TEXTURE_OP in visitVertexList\n";
				MultiTextureRecord* mtr =
					dynamic_cast<MultiTextureRecord*>(child);
				assert( mtr );
				addMultiTexture( dgset, mtr );
			}
			break;
		default:

#ifdef _DEBUG

			CRCore::notify( CRCore::NOTICE )
				<< "flt::ConvertFromFLT::visitVertexList: "
				<< "Unhandled opcode: " << child->getOpcode() << "\n";

#endif

			break;
		}
	}

	return vertices;
}


// Return 1 if record is a known vertex record else return 0.
int ConvertFromFLT::addVertex(DynGeoSet* dgset, Record* rec)
{

	int i ;

	DPRINT(stderr, ">>> addVertex...") ;

	switch(rec->getOpcode())
	{
	case VERTEX_C_OP:
		DPRINT(stderr, "VERTEX_C_OP\n") ;
		{
			SVertex* pVert = (SVertex*)rec->getData();
			CRCore::crVector3 coord(pVert->Coord.x(), pVert->Coord.y(), pVert->Coord.z());
			coord *= (float)m_unitScale;
			dgset->addCoord(coord);
			if ( got_replicate ) 
			{
				/* Handle vertex replication */
				DPRINT(stderr, "      ### addVertex: Replicating (%f,%f,%f) %d times...\n", 
					coord[0], coord[1], coord[2], num_replicate) ;
				for ( i = 0 ; i < num_replicate ; i++ ) 
				{
					if ( got_t ) 
					{
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						dgset->addCoord(coord) ;
					} 
					else 
					{
						/* If we didn't get a translate record, try to get the delta from the matrix */
						delta = theMatrix.getTrans() ;
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						dgset->addCoord(coord) ;
					}
				}
			}
			if (dgset->getColorBinding() == CRCore::crGeometry::BIND_PER_VERTEX)
				ADD_VERTEX_COLOR(dgset, pVert, rec->getFltFile()->getColorPool())

		}
		break;

	case VERTEX_CN_OP:
		DPRINT(stderr, "VERTEX_CN_OP\n") ;
		{
			SNormalVertex* pVert = (SNormalVertex*)rec->getData();
			CRCore::crVector3 coord(pVert->Coord.x(), pVert->Coord.y(), pVert->Coord.z());
			coord *= (float)m_unitScale;
			dgset->addCoord(coord);
			if ( got_replicate ) 
			{
				/* Handle vertex replication */
				DPRINT(stderr, "      ### addVertex: Replicating (%f,%f,%f) %d times...\n", 
					coord[0], coord[1], coord[2], num_replicate) ;
				for ( i = 0 ; i < num_replicate ; i++ ) 
				{
					if ( got_t ) 
					{
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						dgset->addCoord(coord) ;
					} 
					else 
					{
						/* If we didn't get a translate record, try to get the delta from the matrix */
						delta = theMatrix.getTrans() ;
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						dgset->addCoord(coord) ;
					}
				}
			}
			if (dgset->getNormalBinding() == CRCore::crGeometry::BIND_PER_VERTEX)
				ADD_NORMAL(dgset, pVert)
				if (dgset->getColorBinding() == CRCore::crGeometry::BIND_PER_VERTEX)
					ADD_VERTEX_COLOR(dgset, pVert, rec->getFltFile()->getColorPool())
		}
		break;

	case VERTEX_CNT_OP:
		DPRINT(stderr, "VERTEX_CNT_OP\n") ;
		{
			SNormalTextureVertex* pVert = (SNormalTextureVertex*)rec->getData();
			CRCore::crVector3 coord(pVert->Coord.x(), pVert->Coord.y(), pVert->Coord.z());
			coord *= (float)m_unitScale;
			dgset->addCoord(coord);
			if ( got_replicate ) 
			{
				/* Handle vertex replication */
				DPRINT(stderr, "      ### addVertex: Replicating (%f,%f,%f) %d times...\n", 
					coord[0], coord[1], coord[2], num_replicate) ;
				for ( i = 0 ; i < num_replicate ; i++ ) 
				{
					if ( got_t ) 
					{
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						dgset->addCoord(coord) ;
					} 
					else 
					{
						/* If we didn't get a translate record, try to get the delta from the matrix */
						delta = theMatrix.getTrans() ;
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						dgset->addCoord(coord) ;
					}
				}
			}
			if (dgset->getNormalBinding() == CRCore::crGeometry::BIND_PER_VERTEX)
				ADD_NORMAL(dgset, pVert)
				if (dgset->getTextureBinding() == CRCore::crGeometry::BIND_PER_VERTEX)
					ADD_TCOORD(dgset, pVert)
					if (dgset->getColorBinding() == CRCore::crGeometry::BIND_PER_VERTEX)
						ADD_VERTEX_COLOR(dgset, pVert, rec->getFltFile()->getColorPool())
		}
		break;

	case VERTEX_CT_OP:
		DPRINT(stderr, "VERTEX_CT_OP\n") ;
		{
			STextureVertex* pVert = (STextureVertex*)rec->getData();
			CRCore::crVector3 coord(pVert->Coord.x(), pVert->Coord.y(), pVert->Coord.z());
			coord *= (float)m_unitScale;
			dgset->addCoord(coord);
			if ( got_replicate ) 
			{
				/* Handle vertex replication */
				DPRINT(stderr, "      ### addVertex: Replicating (%f,%f,%f) %d times...\n", 
					coord[0], coord[1], coord[2], num_replicate) ;
				for ( i = 0 ; i < num_replicate ; i++ ) 
				{
					if ( got_t ) 
					{
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						dgset->addCoord(coord) ;
					} 
					else 
					{
						/* If we didn't get a translate record, try to get the delta from the matrix */
						delta = theMatrix.getTrans() ;
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						dgset->addCoord(coord) ;
					}
				}
			}
			if (dgset->getTextureBinding() == CRCore::crGeometry::BIND_PER_VERTEX)
				ADD_TCOORD(dgset, pVert)
				if (dgset->getColorBinding() == CRCore::crGeometry::BIND_PER_VERTEX)
					ADD_VERTEX_COLOR(dgset, pVert, rec->getFltFile()->getColorPool())
		}
		break;

	case OLD_VERTEX_OP:
		DPRINT(stderr, "OLD_VERTEX_OP\n") ;
		{
			SOldVertex* pVert = (SOldVertex*)rec->getData();
			CRCore::crVector3 coord(pVert->v[0], pVert->v[1], pVert->v[2]);
			coord *= (float)m_unitScale;
			dgset->addCoord(coord);
			if ( got_replicate ) 
			{
				/* Handle vertex replication */
				DPRINT(stderr, "      ### addVertex: Replicating (%f,%f,%f) %d times...\n", 
					coord[0], coord[1], coord[2], num_replicate) ;
				for ( i = 0 ; i < num_replicate ; i++ ) 
				{
					if ( got_t ) 
					{
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						dgset->addCoord(coord) ;
					} 
					else 
					{
						/* If we didn't get a translate record, try to get the delta from the matrix */
						delta = theMatrix.getTrans() ;
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						dgset->addCoord(coord) ;
					}
				}
			}
			if ((dgset->getTextureBinding() == CRCore::crGeometry::BIND_PER_VERTEX)
				&&  (rec->getSize() >= sizeof(SOldVertex)))
				ADD_OLD_TCOORD(dgset, pVert)
		}
		break;

	case OLD_VERTEX_COLOR_OP:
		DPRINT(stderr, "OLD_VERTEX_COLOR_OP\n") ;
		{
			SOldVertexColor* pVert = (SOldVertexColor*)rec->getData();
			CRCore::crVector3 coord(pVert->v[0], pVert->v[1], pVert->v[2]);
			coord *= (float)m_unitScale;
			dgset->addCoord(coord);
			if ( got_replicate ) 
			{
				/* Handle vertex replication */
				DPRINT(stderr, "      ### addVertex: Replicating (%f,%f,%f) %d times...\n", 
					coord[0], coord[1], coord[2], num_replicate) ;
				for ( i = 0 ; i < num_replicate ; i++ ) 
				{
					if ( got_t ) 
					{
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						dgset->addCoord(coord) ;
					} 
					else 
					{
						/* If we didn't get a translate record, try to get the delta from the matrix */
						delta = theMatrix.getTrans() ;
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						dgset->addCoord(coord) ;
					}
				}
			}
			if (dgset->getColorBinding() == CRCore::crGeometry::BIND_PER_VERTEX)
				ADD_OLD_COLOR(dgset, pVert, rec->getFltFile()->getColorPool())
				if ((dgset->getTextureBinding() == CRCore::crGeometry::BIND_PER_VERTEX)
					&&  (rec->getSize() >= sizeof(SOldVertexColor)))
					ADD_OLD_TCOORD(dgset, pVert)
		}
		break;

	case OLD_VERTEX_COLOR_NORMAL_OP:
		DPRINT(stderr, "OLD_VERTEX_COLOR_NORMAL_OP\n") ;
		{
			SOldVertexColorNormal* pVert = (SOldVertexColorNormal*)rec->getData();
			CRCore::crVector3 coord(pVert->v[0], pVert->v[1], pVert->v[2]);
			coord *= (float)m_unitScale;
			dgset->addCoord(coord);
			if ( got_replicate ) 
			{
				/* Handle vertex replication */
				DPRINT(stderr, "      ### addVertex: Replicating (%f,%f,%f) %d times...\n", 
					coord[0], coord[1], coord[2], num_replicate) ;
				for ( i = 0 ; i < num_replicate ; i++ ) 
				{
					if ( got_t )
					{
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						dgset->addCoord(coord) ;
					} 
					else 
					{
						/* If we didn't get a translate record, try to get the delta from the matrix */
						delta = theMatrix.getTrans() ;
						coord += delta ;
						DPRINT(stderr, "          >> Replicated vertex as (%f,%f,%f)\n", 
							coord[0], coord[1], coord[2]) ;
						dgset->addCoord(coord) ;
					}
				}
			}
			if (dgset->getNormalBinding() == CRCore::crGeometry::BIND_PER_VERTEX)
			{
				CRCore::crVector3 normal(pVert->n[0], pVert->n[1], pVert->n[2]);
				normal /= (float)(1L<<30);
				dgset->addNormal(normal);
			}
			if (dgset->getColorBinding() == CRCore::crGeometry::BIND_PER_VERTEX)
				ADD_OLD_COLOR(dgset, pVert, rec->getFltFile()->getColorPool())
				if ((dgset->getTextureBinding() == CRCore::crGeometry::BIND_PER_VERTEX)
					&&  (rec->getSize() >= sizeof(SOldVertexColorNormal)))
					ADD_OLD_TCOORD(dgset, pVert)
		}
		break;

	default :
		DPRINT(stderr, "*** UNKNOWN (%d)***\n", rec->getOpcode()) ;
		return 0;
	}

	/* Clear out the replicate stuff so that it doesn't get reused if we
	return to this function from somewhere other than addVerticies() */
	got_gm = got_m = got_t = got_replicate = 0 ;

	return 1;
}

// general matrix
CRCore::crGroup* ConvertFromFLT::visitGeneralMatrix(CRCore::crGroup& outParent, const CRCore::crGroup& /*osgPrimary*/, GeneralMatrixRecord* rec)
{
	SGeneralMatrix* pSMatrix = (SGeneralMatrix*)rec->getData();
	CRCore::crMatrixTransform* transform = new CRCore::crMatrixTransform;

	CRCore::crMatrix m;
	for(int i=0;i<4;++i)
	{
		for(int j=0;j<4;++j)
		{
			m(i,j) = pSMatrix->sfMat[i][j];
		}
	}

	// scale position.
	CRCore::crVector3 pos = m.getTrans();
	m *= CRCore::crMatrix::translate(-pos);
	pos *= (float)m_unitScale;
	m *= CRCore::crMatrix::translate(pos);

	//transform->setDataVariance(CRCore::crBase::STATIC);
	transform->setMatrix(m);

	outParent.addChild(transform);

	return transform;
}

// matrix record
CRCore::crGroup* ConvertFromFLT::visitMatrix(CRCore::crGroup& outParent, const CRCore::crGroup& /*osgPrimary*/, MatrixRecord* rec)
{
	SMatrix* pSMatrix = (SMatrix*)rec->getData();
	CRCore::crMatrixTransform* transform = new CRCore::crMatrixTransform;

	CRCore::crMatrix m;
	for(int i=0;i<4;++i)
	{
		for(int j=0;j<4;++j)
		{
			m(i,j) = pSMatrix->sfMat[i][j];
		}
	}

	// scale position.
	CRCore::crVector3 pos = m.getTrans();
	m *= CRCore::crMatrix::translate(-pos);
	pos *= (float)m_unitScale;
	m *= CRCore::crMatrix::translate(pos);

	//transform->setDataVariance(CRCore::crBase::STATIC);
	transform->setMatrix(m);

	outParent.addChild(transform);

	return transform;
}

CRCore::crGroup* ConvertFromFLT::visitExternal(CRCore::crGroup& outParent, ExternalRecord* rec)
{
	// SExternalReference *pSExternal = (SExternalReference*)rec->getData();

	std::string filePath = CRIOManager::getFilePath(rec->getFilename());
	std::string pushAndPopPath;
	CRIOManager::FilePathList fpl = CRIOManager::getDataFilePathList();
	//If absolute path
	if( (filePath.length()>0 && filePath.find_first_of("/\\")==0) ||
		(filePath.length()>2 && filePath.substr(1,1)==":" && filePath.find_first_of("/\\")==2) )
	{
		pushAndPopPath = filePath;
	}
	else
	{
		pushAndPopPath = fpl.empty() ? "." : fpl.front();
		if(pushAndPopPath.empty()) pushAndPopPath = ".";
		pushAndPopPath += "/" + filePath;
	}

	fpl.push_back(pushAndPopPath);
	//CRIOManager::PushAndPopDataPath tmpfile(pushAndPopPath);
	//CRIOManager::PushAndPopDataPath tmpfile(CRIOManager::getFilePath(rec->getFilename()));


	FltFile* pFile = rec->getExternal();
	CRCore::crGroup* external = NULL;
	if (pFile)
	{
		pFile->setDesiredUnits( rec->getFltFile()->getDesiredUnits() );
		external = pFile->convert();
		if (external)
			visitAncillary(outParent, *external, rec)->addChild(external);
	}

	return external;
}


void ConvertFromFLT::visitLightPoint(GeoSetBuilder* pBuilder,CRCore::crGroup& outParent, LightPointRecord* rec)
{
	DynGeoSet* dgset = pBuilder->getDynGeoSet();
	CRCore::crStateSet *outStateSet = dgset->getStateSet();
	SLightPoint *pSLightPoint = (SLightPoint*)rec->getData();

	dgset->setPrimType(CRCore::crPrimitive::PT_POINTS);
	CRCore::crStateAttribute::VRMode lightmode = GL_LIGHTING;
	outStateSet->setMode(lightmode,CRCore::crStateAttribute::OFF);
	//outStateData->setSmoothFlag(CRCore::crStateSet::StateData::POINT_SMOOTH_TRUE)
	//stateSet->setMode(CRCore::crStateAttribute::VRMode(GL_LIGHTING,NULL), CRCore::crStateAttribute::OFF);
	outStateSet->setMode(GL_POINT_SMOOTH, CRCore::crStateAttribute::ON);
	dgset->setColorBinding(CRCore::crGeometry::BIND_PER_VERTEX);

	CRCore::crPoint* point = new CRCore::crPoint;
	if (point)
	{
		/*
		point->setSize(pSLightPoint->sfSize);
		stateSet->setAttributeAndModes( point, CRCore::crStateAttribute::ON );
		//      point->setFadeThresholdSize(const float fadeThresholdSize);
		//      point->setDistanceAttenuation(const crVector3& distanceAttenuation);
		//      point->setStateSetModes(*stateSet, CRCore::crStateAttribute::ON); // GL_POINT_SMOOTH
		*/
		//change to:
		point->setSize(pSLightPoint->afActualPixelSize);
		point->setFadeThresholdSize(pSLightPoint->sfTranspFalloff);
		//numbers that are going to appear are "experimental"
		point->setDistanceAttenuation(CRCore::crVector3(0.0001, 0.0005, 0.00000025));

		point->setMinSize(pSLightPoint->sfMinPixelSize);
		point->setMaxSize(pSLightPoint->sfMaxPixelSize);


		outStateSet->setAttributeAndModes( point, CRCore::crStateAttribute::ON );
		outStateSet->setMode(GL_POINT_SMOOTH, CRCore::crStateAttribute::ON);
		outStateSet->setAttributeAndModes(new CRCore::crBlendFunc, CRCore::crStateAttribute::ON);

	}

	// Visit vertices
	addVertices(pBuilder, outParent, rec);
	pBuilder->addPrimitive();
}


void ConvertFromFLT::visitLightPoint(CRCore::crGroup& outParent, LightPointRecord* rec)
{
	//SLightPoint *pSLightPoint = (SLightPoint*)rec->getData();

	//GeoSetBuilder pBuilder;
	//DynGeoSet* dgset = pBuilder.getDynGeoSet();
	//dgset->setPrimType(CRCore::crPrimitive::PT_POINTS);
	//dgset->setColorBinding(CRCore::crGeometry::BIND_PER_VERTEX);
	//dgset->setNormalBinding(CRCore::crGeometry::BIND_PER_VERTEX);

	//CRSim::crLightPointNode *lpNode = new CRSim::crLightPointNode();

	//for(int i=0; i < rec->getNumChildren(); i++)
	//{
	//	Record* child = rec->getChild(i);
	//	if( child->classOpcode() == COMMENT_OP) visitComment(*lpNode, (CommentRecord*)child);
	//}

	//DPRINT(stderr, "visitLightPoint: visiting node '%s'...(%d children)\n", pSLightPoint->szIdent, rec->getNumChildren()) ;
	//lpNode->setName(pSLightPoint->szIdent) ;

	//lpNode->setMinPixelSize( pSLightPoint->sfMinPixelSize);
	//lpNode->setMaxPixelSize( pSLightPoint->sfMaxPixelSize);
	//DPRINT(stderr, "   MinPixelSize = %f\n", pSLightPoint->sfMinPixelSize) ;
	//DPRINT(stderr, "   MaxPixelSize = %f\n", pSLightPoint->sfMaxPixelSize) ;

	//addVertices(&pBuilder, outParent, rec);

	//const DynGeoSet::CoordList& coords = dgset->getCoordList();
	//const DynGeoSet::ColorList& colors = dgset->getColorList();
	//const DynGeoSet::NormalList& norms = dgset->getNormalList();

	//DPRINT(stderr, "   Num Coords=%d, Num Colors=%d, Num Norms=%d\n", coords.size(), colors.size(), norms.size()) ;

	//bool directional = false;
	//int numInternalLightPoints = 0; // Number of CRSim::crLightPoint objects to add per OpenFlight light point vertex
	//switch (pSLightPoint->diDirection)
	//{
	//case 0: // Omnidirectional;
	//	DPRINT(stderr, "   OMNIDIRECTIONAL\n") ;
	//	directional = false;
	//	numInternalLightPoints = 1;
	//	break;
	//case 1: // Unidirectional;
	//	DPRINT(stderr, "   UNIDIRECTIONAL\n") ;
	//	directional = true;
	//	numInternalLightPoints = 1;
	//	break;
	//case 2: // Bidirectional;
	//	DPRINT(stderr, "   BIDIRECTIONAL\n") ;
	//	directional = true;
	//	numInternalLightPoints = 2;
	//	break;
	//}

	//float lobeVert=0.f, lobeHorz=0.f, lobeRoll=0.f;
	//if ( directional)
	//{
	//	lobeVert = CRCore::DegreesToRadians( pSLightPoint->sfLobeVert );
	//	lobeHorz = CRCore::DegreesToRadians( pSLightPoint->sfLobeHoriz );
	//	lobeRoll = CRCore::DegreesToRadians( pSLightPoint->sfLobeRoll );
	//}
	//float pointRadius =  pSLightPoint->afActualPixelSize * m_unitScale;

	//DPRINT(stderr, "   Vertical Lobe Angle = %f\n", CRCore::RadiansToDegrees(lobeVert)) ;
	//DPRINT(stderr, "   Horizontal Lobe Angle = %f\n", CRCore::RadiansToDegrees(lobeHorz)) ;
	//DPRINT(stderr, "   Lobe Roll Angle = %f\n", CRCore::RadiansToDegrees(lobeRoll)) ;
	//DPRINT(stderr, "   crPoint Radius = %f\n", pointRadius) ;

	///* From my experience during all this testing, I think it's safe to assume that
	//each light point in a single light point node should share the same color
	//and normal.  Even if multiple normals are found, they seem to be wrong for some
	//reason */
	//CRCore::crVector4 color( 1.0f, 1.0f, 1.0f, 1.0f);
	//CRCore::crVector3 normal( 1.0f, 0.0f, 0.0f);

	//for ( unsigned int nl = 0; nl < coords.size(); nl++)
	//{
	//	//if( nl < colors.size())  color = colors[nl];
	//	if( colors.size()>0)  color = colors[0];
	//	DPRINT(stderr, "   Color = %f, %f, %f, %f\n", color.x(), color.y(), color.z(), color.w()) ;

	//	CRSim::crLightPoint lp( true, coords[ nl], color, pSLightPoint->sfIntensityFront, pointRadius);

	//	if( pSLightPoint->diDirection )
	//	{
	//		DPRINT(stderr, "   LP is directional...\n") ;
	//		if ( !pSLightPoint->diDirectionalMode ) 
	//		{
	//			DPRINT(stderr, "%%%%%%%% WARNING: diDirection is set, but diDirectionalMode is off!!!\n") ;
	//		}

	//		// calc elevation angles
	//		//if( nl < norms.size())  normal = norms[nl];
	//		if( norms.size()>0)  normal = norms[0];
	//		DPRINT(stderr, "   Normal = %f, %f, %f\n", normal.x(), normal.y(), normal.z()) ;

	//		// Verify normal.  If normal is 0,0,0, then LP isn't really directional
	//		if ( (fabsf(normal.x()) < 0.0001) && (fabsf(normal.y()) < 0.0001) && (fabsf(normal.z()) < 0.0001) ) 
	//		{
	//			DPRINT(stderr, "%%%%%%%% WARNING: diDirection is set, but normal is not set!!!\n") ;
	//			DPRINT(stderr, "   ADDING LIGHTPOINT\n") ;
	//			lpNode->addLightPoint( lp);
	//			continue ;
	//		}
	//		if ( normal.isNaN() ) 
	//		{
	//			DPRINT(stderr, "%%%%%%%% WARNING: diDirection is set, but normal is NaN!!!\n") ;
	//			DPRINT(stderr, "   ADDING LIGHTPOINT\n") ;
	//			lpNode->addLightPoint( lp);
	//			continue ;
	//		}


	//		lp.m_sector = new CRSim::crDirectionalSector( normal, lobeHorz, lobeVert, lobeRoll);

	//		if( pSLightPoint->diDirection == 2 )
	//		{
	//			DPRINT(stderr, "   ** LP is BIdirectional...\n") ;

	//			CRCore::crVector4 backcolor = pSLightPoint->dwBackColor.get() ;
	//			if ( backcolor.w() == 0.0 ) backcolor[3] = 1.0 ;
	//			CRSim::crLightPoint lp2( true, coords[ nl], backcolor, 1.0f, pointRadius);
	//			DPRINT(stderr, "   Backface Color = %f, %f, %f, %f\n", backcolor.x(), backcolor.y(), backcolor.z(), backcolor.w()) ;
	//			// calc elevation angles
	//			CRCore::crVector3 backnormal = - normal ;
	//			DPRINT(stderr, "   Normal = %f, %f, %f\n", backnormal.x(), backnormal.y(), backnormal.z()) ;

	//			lp2.m_sector = new CRSim::crDirectionalSector( backnormal, lobeHorz, lobeVert, lobeRoll);

	//			DPRINT(stderr, "   ADDING BACKFACING LIGHTPOINT\n") ;
	//			lpNode->addLightPoint(lp2);
	//		}
	//	}

	//	DPRINT(stderr, "   ADDING LIGHTPOINT\n") ;
	//	lpNode->addLightPoint( lp);

	//}

	//DPRINT (stderr, "lpNode has %d children\n", lpNode->getNumLightPoints()) ;
	//outParent.addChild( lpNode);

}


// OpenFlight 15.8 (1580)
// crLight point records contain indices into appearance and animation palettes.
//   Need to look up the palette entries to determine how the light points
//   look (and behave).
void ConvertFromFLT::visitLightPointIndex(CRCore::crGroup& outParent, LightPointIndexRecord* rec)
{
	//SLightPointIndex *ltPtIdx = (SLightPointIndex*)rec->getData();
	//LtPtAppearancePool* appPool = rec->getFltFile()->getLtPtAppearancePool();
	//LtPtAppearancePool::PoolLtPtAppearance* ltPtApp = appPool->get( ltPtIdx->iAppearanceIndex );
	//if (!ltPtApp)
	//	return; // Appearance index out of range
	//LtPtAnimationPool* animPool = rec->getFltFile()->getLtPtAnimationPool();
	//LtPtAnimationPool::PoolLtPtAnimation* ltPtAnim = NULL;
	//if (ltPtIdx->iAnimationIndex >= 0)
	//{
	//	ltPtAnim = animPool->get( ltPtIdx->iAnimationIndex );
	//	if (!ltPtAnim)
	//		return; // Animation index out of range
	//}

	//GeoSetBuilder pBuilder;
	//DynGeoSet* dgset = pBuilder.getDynGeoSet();
	//dgset->setPrimType(CRCore::crPrimitive::PT_POINTS);
	//dgset->setColorBinding(CRCore::crGeometry::BIND_PER_VERTEX);
	//dgset->setNormalBinding(CRCore::crGeometry::BIND_PER_VERTEX);

	//CRSim::crLightPointNode *lpNode = new CRSim::crLightPointNode();

	//for (int i=0; i < rec->getNumChildren(); i++)
	//{
	//	Record* child = rec->getChild(i);
	//	if( child->classOpcode() == COMMENT_OP) visitComment(*lpNode, (CommentRecord*)child);
	//}

	//lpNode->setMinPixelSize( ltPtApp->m_sfMinPixelSize );
	//lpNode->setMaxPixelSize( ltPtApp->m_sfMaxPixelSize );

	//addVertices(&pBuilder, outParent, rec);

	//const DynGeoSet::CoordList& coords = dgset->getCoordList();
	//const DynGeoSet::ColorList& colors = dgset->getColorList();
	//const DynGeoSet::NormalList& norms = dgset->getNormalList();

	//bool directional = false;
	//int numInternalLightPoints = 0; // Number of CRSim::crLightPoint objects to add per OpenFlight light point vertex
	//switch (ltPtApp->m_iDirectionality)
	//{
	//case 0: // Omnidirectional;
	//	directional = false;
	//	numInternalLightPoints = 1;
	//	break;
	//case 1: // Unidirectional;
	//	directional = true;
	//	numInternalLightPoints = 1;
	//	break;
	//case 2: // Bidirectional;
	//	directional = true;
	//	numInternalLightPoints = 2;
	//	break;
	//}

	//float lobeVert=0.f, lobeHorz=0.f, lobeRoll=0.f;
	//if ( directional)
	//{
	//	lobeVert = CRCore::DegreesToRadians( ltPtApp->m_sfVLobeAngle );
	//	lobeHorz = CRCore::DegreesToRadians( ltPtApp->m_sfHLobeAngle );
	//	lobeRoll = CRCore::DegreesToRadians( ltPtApp->m_sfLobeRollAngle );
	//}
	//float pointRadius =  ltPtApp->m_sfActualSize * m_unitScale;

	//for (unsigned int nl = 0; nl < coords.size(); nl++)
	//{
	//	// Could add 1 or 2 internal light points, 2 for bidirectional
	//	for (int i=0; i<numInternalLightPoints; i++)
	//	{
	//		CRCore::crVector4 color( 1.0f, 1.0f, 1.0f, 1.0f);
	//		if ( (i==0) && (nl < colors.size()) )
	//			color = colors[nl];
	//		else if (i==1)
	//		{
	//			// Get back color
	//			ColorPool* pColorPool = rec->getFltFile()->getColorPool();
	//			color = pColorPool->getColor( ltPtApp->m_iBackColorIdx );
	//		}

	//		CRSim::crBlinkSequence* blink = NULL;
	//		if (ltPtAnim && ltPtAnim->m_blink.valid())
	//			blink = ltPtAnim->m_blink.get();

	//		// note in corbin's code the ltPtApp->m_bIntensity was set to 1.0, however,
	//		// I have left the original setting in place.
	//		CRSim::crLightPoint lp( true, coords[nl], color, ltPtApp->m_bIntensity, pointRadius,
	//			0, blink );

	//		if (directional)
	//		{
	//			// calc elevation angles
	//			CRCore::crVector3 normal( 1.0f, 0.0f, 0.0f);
	//			if (nl < norms.size())
	//				normal = norms[nl];
	//			if (i==1)
	//				// Negate the normal for the back facing internal light point
	//				normal = -normal;

	//			lp.m_sector = new CRSim::crDirectionalSector( normal, lobeHorz, lobeVert, lobeRoll);
	//		}

	//		lpNode->addLightPoint(lp);
	//	}
	//}

	//outParent.addChild(lpNode);
}


// OpenFlight 15.8 (1580)
// crLight point systems allow an application to control intensity, animation
//   state, and on/off state of all child light point nodes from a single node.
// On/off state implemented with an CRSim::crMultiSwitch. Set 0 turns all children
//   off, set 1 turns them on. Applications can define other sets if desired, or
//   redefine these sets.
// Children LightPointNodes all have a reference to a common LightPointState object
//   An application controls intensity and animation state parameters by finding
//   the first child of the crLight crPoint System crMultiSwitch, calling
//   getLightPointState(), and setting intensity and animation state accordingly.
CRCore::crGroup* ConvertFromFLT::visitLightPointSystem(CRCore::crGroup& outParent, LightPointSystemRecord* rec)
{
	//SLightPointSystem *ltPtSys = (SLightPointSystem*)rec->getData();

	//CRCore::crMultiSwitch* system = new CRCore::crMultiSwitch;
	//CRCore::ref_ptr<CRSim::crLightPointSystem> lightState = new CRSim::crLightPointSystem;

	//// Attach children
	//visitAncillary( outParent, *system, rec )->addChild( system );
	//visitPrimaryNode( *system, rec );

	//system->setName( ltPtSys->ident );

	//// Set default sets: 0 for all off, 1 for all on
	//system->setAllChildrenOn( 1 );
	//system->setAllChildrenOff( 0 );

	//// Set initial on/off state
	//unsigned int initialSet = ( (ltPtSys->flags & 0x80000000) != 0 ) ? 1 : 0;
	//system->setActiveSwitchSet( initialSet );


	//lightState->setIntensity( ltPtSys->intensity );
	//switch( ltPtSys->animationState )
	//{
	//	// Note that OpenFlight 15.8 spec says 0 means on and 1 means off.
	//	//   However, if animation is set on in Creator, it stores a 1, and
	//	//   a zero is stored for off! So, for now, we ignore the spec...
	//case 0:
	//	lightState->setAnimationState( CRSim::crLightPointSystem::ANIMATION_OFF );
	//	break;
	//default:
	//case 1:
	//	lightState->setAnimationState( CRSim::crLightPointSystem::ANIMATION_ON );
	//	break;
	//case 2:
	//	lightState->setAnimationState( CRSim::crLightPointSystem::ANIMATION_RANDOM );
	//	break;
	//}

	//// Set light point state in all children
	//int errorChildren = 0;
	//for ( unsigned int i=0; i<system->getNumChildren(); i++)
	//{
	//	CRCore::crNode* child = system->getChild( i );
	//	if (CRSim::crLightPointNode* lpn = dynamic_cast<CRSim::crLightPointNode*>(child))
	//		lpn->setLightPointSystem (lightState.get() );
	//	else
	//		// Should never have a non-crLightPointNode child
	//		errorChildren++;
	//}
	//if (errorChildren > 0)
	//	CRCore::notify( CRCore::WARN ) << "ConvertFromFLT::visitLightPointSystem found " << errorChildren << " non-crLightPointNode child(ren)." << std::endl;

	//return ( (CRCore::crGroup*) system );
	return NULL;
}


bool ConvertFromFLT::visitMesh ( CRCore::crGroup &parent, GeoSetBuilder *pBuilder, MeshRecord *rec, bool dontMerge )
{
	DynGeoSet* dgset = pBuilder->getDynGeoSet();
	CRCore::crStateSet *outStateSet = dgset->getStateSet();
	SFace *pSFace = (SFace *) rec->getData();
	bool bBlend = false;

	// See if it's hidden.
	if ( rec->getFlightVersion() > 13 && flt::hasBits ( pSFace->dwFlags, (uint32) MeshRecord::HIDDEN_BIT ) )
		return bBlend;

	// Set the various properties.
	setCullFaceAndWireframe ( pSFace, outStateSet, dgset );
	setDirectionalLight();
	setLightingAndColorBinding ( rec, pSFace, outStateSet, dgset );
	setColor ( rec, pSFace, dgset, bBlend );
	setMaterial ( rec, pSFace, outStateSet, bBlend );
	setTexture ( rec, pSFace, outStateSet, dgset, bBlend );
	setTransparency ( outStateSet, bBlend );

	// Add the vertices.
	addVertices ( pBuilder, parent, rec );

	// Add the mesh primitives.
	addMeshPrimitives ( parent, pBuilder, rec );

	// Visit ancillary records
	for(int i=0; i < rec->getNumChildren(); i++)
	{
		Record* child = rec->getChild(i);
		if (!child->isAncillaryRecord())
			break;

		switch (child->getOpcode())
		{
		case MULTI_TEXTURE_OP:
			{
				CERR2 << "MULTI_TEXTURE_OP in visitMesh\n";
				MultiTextureRecord* mtr =
					dynamic_cast<MultiTextureRecord*>(child);
				assert( mtr );
				addMultiTexture( dgset, mtr );
			}
			break;
		default:

#ifdef _DEBUG

			CRCore::notify( CRCore::NOTICE ) << "flt::ConvertFromFLT::visitMesh: "
				<< "Unhandled opcode: " << child->getOpcode() << "\n";

#endif

			break;
		}
	}

	return bBlend;

}


int ConvertFromFLT::addMeshPrimitives ( CRCore::crGroup &parent, GeoSetBuilder *pBuilder, MeshRecord *rec )
{
	// The count of the mesh primitives added.
	int count = 0;

	// Loop through all the children.
	for ( int i = 0; i < rec->getNumChildren(); ++i )
	{
		// Get the i'th child.
		Record *child = rec->getChild ( i );

		// If it is a mesh primitive...
		if ( MESH_PRIMITIVE_OP == child->getOpcode() )
		{
			// Visit this mesh primitive.
			visitMeshPrimitive ( parent, pBuilder, (MeshPrimitiveRecord *) child );
			++count;
		}
	}

	// Return the number of mesh primitives added.
	return count;
}


int ConvertFromFLT::visitLocalVertexPool ( GeoSetBuilder *, LocalVertexPoolRecord *rec )
{
	// Make the given instance the current one.
	m_currentLocalVertexPool = rec;

	// We didn't add any vertices.
	return 0;
}


void ConvertFromFLT::visitMeshPrimitive ( CRCore::crGroup &parent, GeoSetBuilder *pBuilder, MeshPrimitiveRecord *mesh )
{
	if ( !mesh )
	{
		CRCore::notify(CRCore::NOTICE)<<"Warning:ConvertFromFLT::visitMeshPrimitive () mesh is 0, unable to process."<<std::endl;
		return;
	}

	CRCore::crObject *geode = new CRCore::crObject();
	CRCore::crGeometry *geometry = new CRCore::crGeometry();
	LocalVertexPoolRecord *pool = m_currentLocalVertexPool;

	if (!pool)
	{
		CRCore::notify(CRCore::NOTICE)<<"Warning:ConvertFromFLT::visitMeshPrimitive () pool is 0, unable to process."<<std::endl;
		return;
	}

	CRCore::DrawElementsUShort* pPrim = NULL;
	// Set the correct primitive type.
	switch ( mesh->getData()->primitiveType )
	{
	case MeshPrimitiveRecord::TRIANGLE_STRIP:
		pPrim = new CRCore::DrawElementsUShort(CRCore::crPrimitive::PT_TRIANGLE_STRIP,mesh->getNumVertices());
		break;
	case MeshPrimitiveRecord::TRIANGLE_FAN:
		pPrim = new CRCore::DrawElementsUShort(CRCore::crPrimitive::PT_TRIANGLE_FAN,mesh->getNumVertices());

		break;
	case MeshPrimitiveRecord::QUADRILATERAL_STRIP:
		pPrim = new CRCore::DrawElementsUShort(CRCore::crPrimitive::PT_QUAD_STRIP,mesh->getNumVertices());
		break;
	case MeshPrimitiveRecord::INDEXED_POLYGON:
		pPrim = new CRCore::DrawElementsUShort(CRCore::crPrimitive::PT_TRIANGLES,mesh->getNumVertices());
		break;
	default:
		CRCore::notify(CRCore::NOTICE)<<"Warning:ConvertFromFLT::visitMeshPrimitive () unknown MeshPrimitiveRecord type."<<std::endl;
		return;
	}

	CRCore::DrawElementsUShort::PT_IndexArray& lhs = pPrim->getIndexArray();
	uint32 index;
	for(int i=0;i<mesh->getNumVertices();i++)
	{   
		mesh->getVertexIndex(i,index);
		lhs.push_back(index);
	}

	geometry->addPrimitive(pPrim);

	// Add the vertex properties.
	setMeshCoordinates    ( mesh->getNumVertices(), pool, mesh, geometry );
	setMeshNormals        ( mesh->getNumVertices(), pool, mesh, geometry );
	setMeshColors         ( mesh->getNumVertices(), pool, mesh, geometry );
	setMeshTexCoordinates ( mesh->getNumVertices(), pool, mesh, geometry );

	DynGeoSet* dgset = pBuilder->getDynGeoSet();
	CRCore::crStateSet* outStateSet = dgset->getStateSet();
	geometry->setStateSet( outStateSet );

	// Add the geometry to the geode.
	geode->addDrawable ( geometry );

	// Add the geode to the parent.
	parent.addChild ( geode );
}


flt::uint32 ConvertFromFLT::setMeshCoordinates ( const flt::uint32 &numVerts, const LocalVertexPoolRecord *pool, MeshPrimitiveRecord *mesh, CRCore::crGeometry *geometry )
{
	if (!pool || !mesh || !geometry)
	{
		CRCore::notify(CRCore::WARN)<<"OpenFlight loader detected error:: ConvertFromFLT::setMeshCoordinates passed null objects."<<std::endl;
	}

	// If there aren't any coordinates...
	if ( false == pool->hasAttribute ( LocalVertexPoolRecord::POSITION ) )
		return 0;

	// Allocate the vertices.
	CRCore::Vec3Array *coords = new CRCore::Vec3Array(numVerts);
	if ( NULL == coords )
	{
		CRCore::notify(CRCore::WARN)<<"OpenFlight loader detected error:: ConvertFromFLT::setMeshCoordinates out of memory."<<std::endl;
		return 0;
	}

	// Declare outside of loop.
	uint32 i ( 0 ), index ( 0 );
	float64 px, py, pz;

	// Loop through all the vertices.
	for ( i = 0; i < numVerts; ++i )
	{
		// Get the i'th index into the vertex pool.
		if ( !mesh->getVertexIndex ( i, index ) )
		{
			CRCore::notify(CRCore::WARN)<<"OpenFlight loader detected error:: ConvertFromFLT::setMeshCoordinates out of bounds."<<std::endl;
			return 0;
		}

		// Get the coordinate (using "index").
		if ( !pool->getPosition ( index, px, py, pz ) )
		{
			CRCore::notify(CRCore::WARN)<<"OpenFlight loader detected error:: ConvertFromFLT::setMeshCoordinates out of bounds."<<std::endl;
			return 0;
		}

		// Add the coordinate.
		(*coords)[i].set ( (float) px, (float) py, (float) pz );
	}

	// Set the mesh coordinates.
	geometry->setVertexArray ( coords );

	// Return the number of coordinates added.
	return i;
}


flt::uint32 ConvertFromFLT::setMeshNormals ( const flt::uint32 &numVerts, const LocalVertexPoolRecord *pool, MeshPrimitiveRecord *mesh, CRCore::crGeometry *geometry )
{
	if (!pool || !mesh || !geometry)
	{
		CRCore::notify(CRCore::WARN)<<"OpenFlight loader detected error:: ConvertFromFLT::setMeshTexCoordinates passed null objects."<<std::endl;
	}

	// If there aren't any coordinates...
	if ( false == pool->hasAttribute ( LocalVertexPoolRecord::NORMAL ) )
		return 0;

	// Allocate the normals.
	CRCore::Vec3Array *normals = new CRCore::Vec3Array(numVerts);
	if ( NULL == normals )
	{
		CRCore::notify(CRCore::WARN)<<"OpenFlight loader detected error:: ConvertFromFLT::setMeshNormals out of memory."<<std::endl;
		return 0;
	}

	// Declare outside of loop.
	uint32 i ( 0 ), index ( 0 );
	float32 x, y, z;

	// Loop through all the vertices.
	for ( i = 0; i < numVerts; ++i )
	{
		// Get the i'th index into the vertex pool.
		if ( !mesh->getVertexIndex ( i, index ) )
		{
			CRCore::notify(CRCore::WARN)<<"OpenFlight loader detected error:: ConvertFromFLT::setMeshNormals out of bounds."<<std::endl;
			return 0;
		}

		// Get the normal (using "index").
		if ( !pool->getNormal ( index, x, y, z ) )
		{
			CRCore::notify(CRCore::WARN)<<"OpenFlight loader detected error:: ConvertFromFLT::setMeshNormals out of bounds."<<std::endl;
			return 0;
		}

		// Add the normal.
		(*normals)[i].set ( x, y, z );
	}

	// Set the mesh normals.
	geometry->setNormalArray( normals );
	geometry->setNormalBinding( CRCore::crGeometry::BIND_PER_VERTEX );

	// Return the number of normals added.
	return i;
}


flt::uint32 ConvertFromFLT::setMeshColors ( const flt::uint32 &numVerts, const LocalVertexPoolRecord *pool, MeshPrimitiveRecord *mesh, CRCore::crGeometry *geometry )
{
	if (!pool || !mesh || !geometry)
	{
		CRCore::notify(CRCore::WARN)<<"OpenFlight loader detected error:: ConvertFromFLT::setMeshTexCoordinates passed null objects."<<std::endl;
	}

	// If there aren't any colors...
	if ( false == pool->hasAttribute ( LocalVertexPoolRecord::RGB_COLOR ) )
		return 0;

	// Allocate the normals.
	CRCore::Vec4Array *colors = new CRCore::Vec4Array(numVerts);
	if ( NULL == colors )
	{
		CRCore::notify(CRCore::WARN)<<"OpenFlight loader detected error:: ConvertFromFLT::setMeshColors out of memory."<<std::endl;
		return 0;
	}

	// Declare outside of loop.
	uint32 i ( 0 ), index ( 0 );
	float32 red, green, blue, alpha;

	// Loop through all the vertices.
	for ( i = 0; i < numVerts; ++i )
	{
		// Get the i'th index into the vertex pool.
		if ( false == mesh->getVertexIndex ( i, index ) )
		{
			CRCore::notify(CRCore::WARN)<<"OpenFlight loader detected error:: ConvertFromFLT::setMeshColors out of bounds."<<std::endl;
			return 0;
		}

		// Get the color (using "index").
		if ( false == pool->getColorRGBA ( index, red, green, blue, alpha ) )
		{
			CRCore::notify(CRCore::WARN)<<"OpenFlight loader detected error:: ConvertFromFLT::setMeshColors out of bounds."<<std::endl;
			return 0;
		}

		// Add the coordinate.
		(*colors)[i].set ( red, green, blue, alpha );
	}

	// Set the mesh coordinates.
	geometry->setColorArray ( colors );
	geometry->setColorBinding( CRCore::crGeometry::BIND_PER_VERTEX );

	// Return the number of colors added.
	return i;
}

void ConvertFromFLT::setMeshTexCoordinates ( const flt::uint32 &numVerts, const LocalVertexPoolRecord *pool, MeshPrimitiveRecord *mesh, CRCore::crGeometry *geometry )
{
	if (!pool || !mesh || !geometry)
	{
		CRCore::notify(CRCore::WARN)<<"OpenFlight loader detected error:: ConvertFromFLT::setMeshTexCoordinates passed null objects."<<std::endl;
	}

	// Don't know the best way to do this in C++ without breaking
	// data encapsulation rules or ending up with silly cut-and-paste
	// code
	//
	std::vector<LocalVertexPoolRecord::AttributeMask>  lAttrList( 8 );
	lAttrList[0] = LocalVertexPoolRecord::BASE_UV;
	lAttrList[1] = LocalVertexPoolRecord::UV_1;
	lAttrList[2] = LocalVertexPoolRecord::UV_2;
	lAttrList[3] = LocalVertexPoolRecord::UV_3;
	lAttrList[4] = LocalVertexPoolRecord::UV_4;
	lAttrList[5] = LocalVertexPoolRecord::UV_5;
	lAttrList[6] = LocalVertexPoolRecord::UV_6;
	lAttrList[7] = LocalVertexPoolRecord::UV_7;

	//CRCore::notify(CRCore::INFO) << "flt2osg::setMeshTexCoordinates() "
	//	<< "Attribute masks in list."
	//	<< std::endl;

	// Check for texture coordinates for each possible texture
	//
	unsigned int   lAttrIdx;
	for (lAttrIdx = 0; lAttrIdx < lAttrList.size(); ++lAttrIdx)
	{
		//CRCore::notify(CRCore::INFO) << "flt2osg::setMeshTexCoordinates() "
		//	<< "Checking texture "
		//	<< lAttrIdx
		//	<< std::endl;

		// If there aren't any coordinates for this texture, skip to next
		//
		if (!pool->hasAttribute ( lAttrList[lAttrIdx] ) )
			continue;

		// Allocate the vertices.
		CRCore::Vec2Array *coords = new CRCore::Vec2Array(numVerts);
		if ( NULL == coords )
		{
			CRCore::notify(CRCore::WARN)<<"OpenFlight loader detected error:: ConvertFromFLT::setMeshTexCoordinates out of memory."<<std::endl;
			return;
		}

		//CRCore::notify(CRCore::INFO) << "flt2osg::setMeshTexCoordinates() "
		//	<< "Getting coords"
		//	<< std::endl;

		// Declare outside of loop.
		uint32 i ( 0 ), index ( 0 );
		float32 pu, pv;

		// Loop through all the vertices.
		for ( i = 0; i < numVerts; ++i )
		{
			// Get the i'th index into the vertex pool.
			if ( !mesh->getVertexIndex ( i, index ) )
			{
				CRCore::notify(CRCore::WARN)<<"OpenFlight loader detected error:: ConvertFromFLT::setMeshTexCoordinates out of bounds."<<std::endl;
				return;
			}

			// Get the coordinate (using "index").
			if ( !pool->getUV ( index, lAttrList[lAttrIdx], pu, pv ) )
			{
				CRCore::notify(CRCore::WARN)<<"OpenFlight loader detected error:: ConvertFromFLT::setMeshTexCoordinates out of bounds."<<std::endl;
				return;
			}

			// Add the coordinate.
			(*coords)[i].set( (float) pu, (float) pv );
		}

		//CRCore::notify(CRCore::INFO) << "flt2osg::setMeshTexCoordinates() "
		//	<< "Adding coords to texture unit "
		//	<< lAttrIdx
		//	<< std::endl;

		// Set the mesh coordinates for this texture layer... use
		// the attribute index as the texture unit.
		//
		geometry->setTexCoordArray ( lAttrIdx, coords );

	}  // check each possible texture

	return;
}


