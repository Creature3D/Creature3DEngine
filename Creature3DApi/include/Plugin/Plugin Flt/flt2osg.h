#ifndef __FLT_2_OSG_H
#define __FLT_2_OSG_H

#include <CRCore/ref_ptr.h>
#include <CRCore/crVector4.h>

#include <map>
#include <vector>
#include <string>


#include <Plugin flt\Record.h>
#include <Plugin flt\GeoSetBuilder.h>


namespace CRCore {
class crBase;
class crGroup;
class crOccluderNode;
class crConvexPlanarPolygon;
class crLod;
class crObject;
class crMaterial;
class crTexture;
};


namespace flt {

class Record;
class HeaderRecord;
class ColorPaletteRecord;
class MaterialPaletteRecord;
class OldMaterialPaletteRecord;
class TexturePaletteRecord;
class LtPtAppearancePaletteRecord;
class LtPtAnimationPaletteRecord;
class VertexPaletteRecord;
class VertexRecord;
class NormalVertexRecord;
class TextureVertexRecord;
class NormalTextureVertexRecord;
class GroupRecord;
class LodRecord;
class OldLodRecord;
class DofRecord;
class SwitchRecord;
class ObjectRecord;
class FaceRecord;
class MeshRecord;
class MeshPrimitiveRecord;
class MatrixRecord;
class GeneralMatrixRecord;
class ExternalRecord;
class LightPointRecord;
class LightPointIndexRecord;
class LightPointSystemRecord;
class VertexListRecord;
class LocalVertexPoolRecord;
class LongIDRecord;
class CommentRecord;
class InstanceDefinitionRecord;
class InstanceReferenceRecord;
class MultiTextureRecord;
class UVListRecord;
class LightSourceRecord;
class LightSourcePaletteRecord;
class BSPRecord;
struct SFace;

//class GeoSetBuilder;

struct SMaterial;

#define ADD_NORMAL(DGSET,VERTEX)                                \
        (DGSET)->addNormal(CRCore::crVector3(                           \
            (float)(VERTEX)->Normal.x(),                        \
            (float)(VERTEX)->Normal.y(),                        \
            (float)(VERTEX)->Normal.z()));

#define ADD_VERTEX_COLOR(DGSET,VERTEX,COLOR_POOL)               \
    {                                                           \
        if ((VERTEX)->swFlags & V_NO_COLOR_BIT)                 \
            (DGSET)->addColor(m_faceColor);                      \
        else                                                    \
        {                                                       \
            if ((VERTEX)->swFlags & V_PACKED_COLOR_BIT)         \
                (DGSET)->addColor(pVert->PackedColor.get());    \
            else                                                \
                (DGSET)->addColor((COLOR_POOL)->getColor((VERTEX)->dwVertexColorIndex)); \
        }                                                       \
    }

#define ADD_TCOORD(DGSET,VERTEX)                                \
        (DGSET)->addTCoord(CRCore::crVector2(                           \
            (float)(VERTEX)->Texture.x(),                       \
            (float)(VERTEX)->Texture.y()));

#define ADD_OLD_COLOR(DGSET,VERTEX,COLOR_POOL)                  \
    {                                                           \
        if (COLOR_POOL)                                         \
            (DGSET)->addColor((COLOR_POOL)->getOldColor((VERTEX)->color_index)); \
        else                                                    \
            (DGSET)->addColor(CRCore::crVector4(1,1,1,1));              \
    }

#define ADD_OLD_TCOORD(DGSET,VERTEX)                            \
        (DGSET)->addTCoord(CRCore::crVector2(                           \
            (float)(VERTEX)->t[0],                              \
            (float)(VERTEX)->t[1]));

#define ADD_OLD_NORMAL(DGSET,VERTEX)                            \
        (DGSET)->addNormal(CRCore::crVector3(                           \
            (float)pVert->n[0] / (1<<30),                       \
            (float)pVert->n[1] / (1<<30),                       \
            (float)pVert->n[2] / (1<<30)));


class ConvertFromFLT
{
    public:

        ConvertFromFLT();
        virtual ~ConvertFromFLT();

        CRCore::crGroup* convert(HeaderRecord* rec);

        CRCore::crGroup* visitAncillary(CRCore::crGroup& osgParent, CRCore::crGroup& osgPrimary, PrimNodeRecord* rec);
        CRCore::crGroup* visitPrimaryNode(CRCore::crGroup& osgParent, PrimNodeRecord* rec);

        // Ancillary records
        CRCore::crGroup* visitMatrix(CRCore::crGroup& osgParent, const CRCore::crGroup& osgPrimary, MatrixRecord* rec);
		CRCore::crGroup* visitGeneralMatrix(CRCore::crGroup& osgParent, const CRCore::crGroup& osgPrimary, GeneralMatrixRecord* rec);
        void visitLongID(CRCore::crGroup& osgParent, LongIDRecord* rec);
        void visitComment(CRCore::crNode& osgParent, CommentRecord* rec);

        // Palette records
        void visitColorPalette(CRCore::crGroup& osgParent, ColorPaletteRecord* rec);
        void visitLightSourcePalette(CRCore::crGroup& osgParent, LightSourcePaletteRecord* rec);
        void visitMaterialPalette(CRCore::crGroup& osgParent, MaterialPaletteRecord* rec);
        void visitOldMaterialPalette(CRCore::crGroup& osgParent, OldMaterialPaletteRecord* rec);
        void visitTexturePalette(CRCore::crGroup& osgParent, TexturePaletteRecord* rec);
        void visitLtPtAppearancePalette(CRCore::crGroup& osgParent, LtPtAppearancePaletteRecord* rec);
        void visitLtPtAnimationPalette(CRCore::crGroup& osgParent, LtPtAnimationPaletteRecord* rec);
        void visitVertexPalette(CRCore::crGroup& osgParent, VertexPaletteRecord* rec);
        void visitVertex(CRCore::crGroup& osgParent, VertexRecord* rec);
        void visitNormalVertex(CRCore::crGroup& osgParent, NormalVertexRecord* rec);
        void visitTextureVertex(CRCore::crGroup& osgParent, TextureVertexRecord* rec);
        void visitNormalTextureVertex(CRCore::crGroup& osgParent, NormalTextureVertexRecord* rec);

        // Primary records
        CRCore::crGroup* visitHeader(HeaderRecord* rec);
        CRCore::crGroup* visitGroup(CRCore::crGroup& osgParent, GroupRecord* rec);
        CRCore::crGroup* visitBSP(CRCore::crGroup& osgParent, BSPRecord* rec);
        CRCore::crGroup* visitLightSource(CRCore::crGroup& osgParent, LightSourceRecord* rec);
        CRCore::crGroup* visitRoadConstruction(CRCore::crGroup& osgParent, GroupRecord* rec);
        CRCore::crGroup* visitLOD(CRCore::crGroup& osgParent, LodRecord* rec);
        CRCore::crGroup* visitOldLOD(CRCore::crGroup& osgParent, OldLodRecord* rec);
        CRCore::crGroup* visitDOF(CRCore::crGroup& osgParent, DofRecord* rec);
        CRCore::crGroup* visitSwitch(CRCore::crGroup& osgParent, SwitchRecord* rec);
        CRCore::crGroup* visitObject(CRCore::crGroup& osgParent, ObjectRecord* rec);
		CRCore::crGroup* visitOccluderNode(CRCore::crOccluderNode& osgParent, ObjectRecord* rec);
        CRCore::crGroup* visitExternal(CRCore::crGroup& osgParent, ExternalRecord* rec);
        CRCore::crGroup* visitInstanceDefinition(CRCore::crGroup& osgParent,InstanceDefinitionRecord* rec);
        CRCore::crGroup* visitInstanceReference(CRCore::crGroup& osgParent,InstanceReferenceRecord* rec);

		CRCore::crGroup* parseComment(CRCore::crGroup& group, Record *rec);

        bool visitFace(GeoSetBuilder* pParent, CRCore::crGroup& osgParent, FaceRecord* rec, bool dontMerge = false);
        bool visitMesh(CRCore::crGroup& osgParent,GeoSetBuilder* pParent, MeshRecord* rec, bool dontMerge = false);
        void visitMeshPrimitive(CRCore::crGroup& osgParent, GeoSetBuilder* pBuilder, MeshPrimitiveRecord* rec);
        void visitLightPoint(GeoSetBuilder* pBuilder, CRCore::crGroup& osgParent, LightPointRecord* rec);
        void visitLightPoint(CRCore::crGroup& osgParent, LightPointRecord* rec);
        void visitLightPointIndex(CRCore::crGroup& osgParent, LightPointIndexRecord* rec);
        CRCore::crGroup* visitLightPointSystem(CRCore::crGroup& osgParent, LightPointSystemRecord* rec);
        int  visitVertexList(GeoSetBuilder* pParent, VertexListRecord* rec);
        int  visitLocalVertexPool(GeoSetBuilder* pBuilder, LocalVertexPoolRecord* rec);


        void setUseTextureAlphaForTransparancyBinning(bool flag) { m_useTextureAlphaForTranspancyBinning=flag; }
        bool getUseTextureAlphaForTransparancyBinning() const { return m_useTextureAlphaForTranspancyBinning; }
        void setDoUnitsConversion(bool flag) { m_doUnitsConversion=flag; }
        bool getDoUnitsConversion() const { return m_doUnitsConversion; }

        enum ExternalNode
		{
			GROUPNODE,
			CLEARNODE,
			OCCLUDERNODE,
			EVENTGROUP,

			FX_SPECULARHIGHLIGHTS,
			FX_CUPMAPPING,
			FX_ANISOTROPICLIGHTING,
			FX_BUMPMAPPING,
			FX_CARTOON,
            FX_SCRIBE,

			MATTEROBJECT,
			MATTERGROUP,

			EXTERNNODE
		};

		//FX
		void createCupMap( const std::string *texName, unsigned int unit, CRCore::crStateSet *stateset );
		void createSpecularHighlights( std::string &comment, unsigned int unit, CRCore::crStateSet *stateset );
		//void createAnisotropicLighting( int lightnum, unsigned int unit, CRCore::crStateSet *stateset );
		//void createBumpMapping_Normal( std::string &comment, unsigned int normalunit, CRCore::crStateSet *stateset );
        //void createBumpMapping_Diffuse( std::string &comment, unsigned int diffuseunit,  CRCore::crStateSet *stateset );

    private:
		int addOccluderVerticie(CRCore::crConvexPlanarPolygon &cpp, Record* rec);
        int addOccluderVerticies( CRCore::crConvexPlanarPolygon &cpp, PrimNodeRecord* primRec );
        int addMeshPrimitives ( CRCore::crGroup &osgParent, GeoSetBuilder *pBuilder, MeshRecord *rec );
        int addVertices(GeoSetBuilder* pBuilder, CRCore::crGroup& osgParent, PrimNodeRecord* primRec);
        int addVertex(DynGeoSet* dgset, Record* rec);
        int addVertex(GeoSetBuilder* pBuilder, Record* rec) {return addVertex( pBuilder->getDynGeoSet(), rec);} ;
        Record* getVertexFromPool(int nOffset);
        void regisiterVertex(int nOffset, Record* pRec);
        void visitFaceOrMeshCommonCode(GeoSetBuilder* pBuilder, FaceRecord* rec);
        uint32 setMeshCoordinates ( const uint32 &numVerts, const LocalVertexPoolRecord *pool, MeshPrimitiveRecord *mesh, CRCore::crGeometry *geometry );
        uint32 setMeshNormals ( const uint32 &numVerts, const LocalVertexPoolRecord *pool, MeshPrimitiveRecord *mesh, CRCore::crGeometry *geometry );
        uint32 setMeshColors ( const uint32 &numVerts, const LocalVertexPoolRecord *pool, MeshPrimitiveRecord *mesh, CRCore::crGeometry *geometry );
        void setMeshTexCoordinates ( const uint32 &numVerts, const LocalVertexPoolRecord *pool, MeshPrimitiveRecord *mesh, CRCore::crGeometry *geometry );

        void setCullFaceAndWireframe ( const SFace *pSFace, CRCore::crStateSet *outStateSet, DynGeoSet *dgset );
        void setDirectionalLight();
        void setLightingAndColorBinding ( const FaceRecord *rec, const SFace *pSFace, CRCore::crStateSet *outStateSet, DynGeoSet *dgset );
        void setColor ( FaceRecord *rec, SFace *pSFace, DynGeoSet *dgset, bool &bBlend );
        void setMaterial ( FaceRecord *rec, SFace *pSFace, CRCore::crStateSet *outStateSet, bool &bBlend );
        void setTexture ( FaceRecord *rec, SFace *pSFace, CRCore::crStateSet *outStateSet, DynGeoSet *dgset, bool &bBlend );
        void setTransparency ( CRCore::crStateSet *outStateSet, bool &bBlend );

        // multitexturing
        void addMultiTexture( DynGeoSet* dgset, MultiTextureRecord* mtr );
        void addUVList( DynGeoSet* dgset, UVListRecord* mtr );

        typedef std::map<int,Record*> VertexPaletteOffsetMap;
        VertexPaletteOffsetMap m_VertexPaletteOffsetMap;

        int                     m_diOpenFlightVersion;
        int                     m_diCurrentOffset;
        unsigned short          m_wObjTransparency;
        int                     m_nSubfaceLevel;
        double                  m_unitScale;
        bool                    m_bHdrRgbMode;
        CRCore::crVector4               m_faceColor;
        bool                    m_useTextureAlphaForTranspancyBinning;
        bool                    m_doUnitsConversion;

        CRCore::crGroup*             m_osgParent;
        
        LocalVertexPoolRecord* m_currentLocalVertexPool;
};

    
}; // end namespace flt

#endif // __FLT_2_OSG_H

