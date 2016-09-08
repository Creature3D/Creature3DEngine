// GeoSetBuilder.cpp

// Modify DynGeoSet::addToGeometry to generate texture coordinates for texture unit 1 
// that is used to detail texture
// Julian Ortiz, June 18th 2003.

#if defined(_MSC_VER)
#pragma warning( disable : 4786 )
#endif

#include <Plugin flt\flt.h>
#include <Plugin flt\FltFile.h>
#include <Plugin flt\Pool.h>
#include <Plugin flt\opcodes.h>
#include <Plugin flt\GeoSetBuilder.h>
#include <Plugin flt\AttrData.h>

#include <CRCore/crBase.h>
#include <CRCore/crLod.h>
#include <CRCore/crBlendFunc.h>
#include <CRCore/crObject.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crMaterial.h>
#include <CRCore/crTexture2D.h>
#include <CRCore/crTexEnv.h>
#include <CRCore/crCullFace.h>
#include <CRCore/crPolygonOffset.h>
#include <CRCore/crPoint.h>
#include <CRCore/crNotify.h>
#include <CRCore/crArray.h>
#include <CRCore/crBillboard.h>

#include <CRUtil/crTesselator.h>

#include <map>
#include <algorithm>

using namespace flt;


////////////////////////////////////////////////////////////////////
//
//                       DynGeoSet
//
////////////////////////////////////////////////////////////////////


#define APPEND_DynGeoSet_List(list)                     \
    if (source->list.size() > 0)                        \
        list.insert(list.end(),                         \
            source->list.begin(), source->list.end());

#define APPEND_DynGeoSet_TexCoordList(list)   \
	if (source->list.size() > 0)              \
	    list.insert(list.end(),                   \
	            source->list.begin(), source->list.end());\
	{\
	    CRCore::Vec2Array* lTexCoorArray = NULL;\
	    CRCore::Vec2Array* rTexCoorArray = NULL;\
	    int i = 1;\
	    for(; i<m_geom->getNumTexCoordArrays();i++)\
		{\
	        lTexCoorArray = (CRCore::Vec2Array*)m_geom->getTexCoordArray(i);\
	        rTexCoorArray = (CRCore::Vec2Array*)source->m_geom->getTexCoordArray(i);\
	        if(lTexCoorArray&&rTexCoorArray)\
			{\
	            lTexCoorArray->insert(lTexCoorArray->end(),rTexCoorArray->begin(),rTexCoorArray->end());\
			}\
		}\
	}

#define APPEND_DynGeoSet_TexCoordList2(list)   \
	if (source->list.size() > 0)              \
	    list.insert(list.end(),                   \
	         source->list.begin(), source->list.end());\
	{\
		CRCore::Vec2Array* lTexCoorArray = NULL;\
		CRCore::Vec2Array* rTexCoorArray = NULL;\
		for(int i = 1; i<8; i++)\
		{\
		    lTexCoorArray = (CRCore::Vec2Array*)m_geom->getTexCoordArray(i);\
			rTexCoorArray = (CRCore::Vec2Array*)source->m_geom->getTexCoordArray(i);\
			if(rTexCoorArray)\
            {\
			    if(!lTexCoorArray)\
	                m_geom->setTexCoordArray(i,new CRCore::Vec2Array(*rTexCoorArray));\
				else \
					lTexCoorArray->insert(lTexCoorArray->end(),rTexCoorArray->begin(),rTexCoorArray->end());\
			}\
		}\
	}

void DynGeoSet::append(DynGeoSet* source)
{
    APPEND_DynGeoSet_List(m_primLenList)
    APPEND_DynGeoSet_List(m_coordList)
    if (m_normal_binding==CRCore::crGeometry::BIND_PER_VERTEX || m_normal_binding==CRCore::crGeometry::BIND_PER_PRIMITIVE) APPEND_DynGeoSet_List(m_normalList);
    if (m_color_binding==CRCore::crGeometry::BIND_PER_VERTEX || m_color_binding==CRCore::crGeometry::BIND_PER_PRIMITIVE) APPEND_DynGeoSet_List(m_colorList);
    if (m_texture_binding==CRCore::crGeometry::BIND_PER_VERTEX || m_texture_binding==CRCore::crGeometry::BIND_PER_PRIMITIVE) APPEND_DynGeoSet_TexCoordList(m_tcoordList);
}


#define VERIFY_DynGeoSet_Binding(binding,list)          \
        switch (binding)                                \
        {                                               \
        case CRCore::crGeometry::BIND_PER_VERTEX:               \
            if (list.size() < m_coordList.size()) {      \
                binding = CRCore::crGeometry::BIND_OFF;        \
                list.clear(); }                         \
            break;                                      \
        case CRCore::crGeometry::BIND_PER_PRIMITIVE:                 \
            if (list.size() < m_primLenList.size()) {    \
                binding = CRCore::crGeometry::BIND_OFF;        \
                list.clear(); }                         \
            break;                                      \
        case CRCore::crGeometry::BIND_OVERALL:                 \
            if (list.size() < 1) {                      \
                binding = CRCore::crGeometry::BIND_OFF;        \
                list.clear(); }                         \
            break;                                      \
        default:                                        \
            break;                                      \
        }

const CRCore::crPrimitive::Mode NO_PRIMITIVE_TYPE = (CRCore::crPrimitive::Mode)0xffff;

DynGeoSet::DynGeoSet()
{
    m_primtype = NO_PRIMITIVE_TYPE;
    m_normal_binding = CRCore::crGeometry::BIND_OFF;
    m_color_binding = CRCore::crGeometry::BIND_OFF;
    m_texture_binding = CRCore::crGeometry::BIND_OFF;
    m_alpha = 1.0f; 
    m_geom = new CRCore::crGeometry;

}

void DynGeoSet::setBinding()
{
	//unsigned int i;
    VERIFY_DynGeoSet_Binding(m_normal_binding, m_normalList)
    VERIFY_DynGeoSet_Binding(m_color_binding, m_colorList)
	VERIFY_DynGeoSet_Binding(m_texture_binding, m_tcoordList)
	//for ( i = 0; i < m_tcoordList.size(); i++)
 //       VERIFY_DynGeoSet_Binding(m_texture_binding[i], m_tcoordList[i])

    // Set bindings
    setNormalBinding(m_normal_binding);
    setColorBinding(m_color_binding);
	//for (i = 0; i < m_tcoordList.size(); i++)
	//	setTextureBinding(i, m_texture_binding[i]);
    setTextureBinding(m_texture_binding);

    CRCore::crStateSet* stateset = getStateSet();
    if (stateset)
    {
        if (m_normal_binding == CRCore::crGeometry::BIND_OFF)
			stateset->setMode(GL_LIGHTING,CRCore::crStateAttribute::OFF);
    }
}



void DynGeoSet::addToGeometry(CRCore::crGeometry* geom)
{
    int indexBase = 0;
    int nVertices = m_coordList.size();
    CRCore::Vec3Array* vertices = dynamic_cast<CRCore::Vec3Array*>(geom->getVertexArray());
    if (vertices)
    {
        indexBase = vertices->size();
        vertices->insert(vertices->end(),m_coordList.begin(),m_coordList.end());
    }
    else
    {
        vertices = new CRCore::Vec3Array(m_coordList.begin(),m_coordList.end());
        geom->setVertexArray(vertices);
    }

    if (!m_normalList.empty())
    {
        CRCore::Vec3Array* normals = geom->getNormalArray();
        if (normals)
        {
            if (m_normal_binding==CRCore::crGeometry::BIND_PER_VERTEX || m_normal_binding==CRCore::crGeometry::BIND_PER_PRIMITIVE)
                normals->insert(normals->end(),m_normalList.begin(),m_normalList.end());
        }
        else
        {
            normals = new CRCore::Vec3Array(m_normalList.begin(),m_normalList.end());
            geom->setNormalArray(normals);

            switch(m_normal_binding)
            {
                case(CRCore::crGeometry::BIND_OVERALL):geom->setNormalBinding(CRCore::crGeometry::BIND_OVERALL);break;
                case(CRCore::crGeometry::BIND_PER_VERTEX):geom->setNormalBinding(CRCore::crGeometry::BIND_PER_VERTEX);break;        
                case(CRCore::crGeometry::BIND_PER_PRIMITIVE):geom->setNormalBinding(CRCore::crGeometry::BIND_PER_PRIMITIVE);break;        
                default:geom->setNormalBinding(CRCore::crGeometry::BIND_OFF); break;
            }
        }
    }

    if (!m_tcoordList.empty())
    {
        CRCore::Vec2Array* texcoords = dynamic_cast<CRCore::Vec2Array*>(geom->getTexCoordArray(0));
        if (texcoords)
        {
            texcoords->insert(texcoords->end(),m_tcoordList.begin(),m_tcoordList.end());
        }
        else
        {
            texcoords = new CRCore::Vec2Array(m_tcoordList.begin(),m_tcoordList.end());
            geom->setTexCoordArray(0,texcoords);

            // If we got detail texture defined for this geometry, we need to setup new texcoord 
            // related on base texture ones. Using txDetail_m and  txDetail_n values we read in 
            // ReaderWriterATTR and we got in AttrData class.
            // 
            // We apply those values multiplying original texcoord.x by txDetail_m and texcoord.y 
            // by txDetail_n to get detail texture repeated.
            //
            // Julian Ortiz, June 18th 2003.

            if ( (m_attrdata != NULL) && (m_attrdata->useDetail > 0) ) 
			{
                CRCore::Vec2Array *texcoords2 = new CRCore::Vec2Array(m_tcoordList.begin(),m_tcoordList.end());
                for(unsigned int index=0;index<texcoords2->size();index++) 
				{
                    (*texcoords2)[index][0]=(*texcoords)[index][0]*m_attrdata->txDetail_m;
                    (*texcoords2)[index][1]=(*texcoords)[index][1]*m_attrdata->txDetail_n;
                }                
                geom->setTexCoordArray(1,texcoords2);
            }                                                    
        }
    }
    
    if (!m_colorList.empty())
    {
        CRCore::Vec4Array* colors = dynamic_cast<CRCore::Vec4Array*>(geom->getColorArray());
        if (colors)
        {
            if (m_color_binding==CRCore::crGeometry::BIND_PER_VERTEX || m_color_binding==CRCore::crGeometry::BIND_PER_PRIMITIVE)
                colors->insert(colors->end(),m_colorList.begin(),m_colorList.end());
        }
        else
        {
            colors = new CRCore::Vec4Array(m_colorList.begin(),m_colorList.end());
            geom->setColorArray(colors);

            switch(m_color_binding)
            {
                case(CRCore::crGeometry::BIND_OVERALL):geom->setColorBinding(CRCore::crGeometry::BIND_OVERALL);break;
                case(CRCore::crGeometry::BIND_PER_VERTEX):geom->setColorBinding(CRCore::crGeometry::BIND_PER_VERTEX);break;        
                case(CRCore::crGeometry::BIND_PER_PRIMITIVE):geom->setColorBinding(CRCore::crGeometry::BIND_PER_PRIMITIVE);break;        
                default:geom->setColorBinding(CRCore::crGeometry::BIND_OFF); break;
            }
        }
    }

	//CRCore::DrawElementsUShort* pPrim = NULL;//new CRCore::DrawElementsUInt(CRCore::crPrimitive::PT_TRIANGLES,PosNumber);
	//switch(m_primtype) 
	//{
	//case CRCore::crPrimitive::PT_POINTS:
	//	pPrim = new CRCore::DrawElementsUShort(CRCore::crPrimitive::PT_POINTS,nVertices);
	//	break;
	//case CRCore::crPrimitive::PT_LINES:
	//	pPrim = new CRCore::DrawElementsUShort(CRCore::crPrimitive::PT_LINES,nVertices);
	//	break;
	//case CRCore::crPrimitive::PT_LINE_STRIP:
	//	pPrim = new CRCore::DrawElementsUShort(CRCore::crPrimitive::PT_LINE_STRIP,nVertices);
	//	break;
	//case CRCore::crPrimitive::PT_LINE_LOOP:
	//	pPrim = new CRCore::DrawElementsUShort(CRCore::crPrimitive::PT_LINE_LOOP,nVertices);
	//	break;
	//case CRCore::crPrimitive::PT_TRIANGLES:
 //       pPrim = new CRCore::DrawElementsUShort(CRCore::crPrimitive::PT_TRIANGLES,nVertices);
	//	break;
	//case CRCore::crPrimitive::PT_TRIANGLE_STRIP:
	//	pPrim = new CRCore::DrawElementsUShort(CRCore::crPrimitive::PT_TRIANGLE_STRIP,nVertices);
	//	break;
	//case CRCore::crPrimitive::PT_TRIANGLE_FAN:
	//	pPrim = new CRCore::DrawElementsUShort(CRCore::crPrimitive::PT_TRIANGLE_FAN,nVertices);
	//	break;
	//case CRCore::crPrimitive::PT_QUADS:
 //       pPrim = new CRCore::DrawElementsUShort(CRCore::crPrimitive::PT_QUADS,nVertices);
	//	break;
	//case CRCore::crPrimitive::PT_QUAD_STRIP:
	//	pPrim = new CRCore::DrawElementsUShort(CRCore::crPrimitive::PT_QUAD_STRIP,nVertices);
	//	break;
	//case CRCore::crPrimitive::PT_POLYGON:
	//	pPrim = new CRCore::DrawElementsUShort(CRCore::crPrimitive::PT_POLYGON,nVertices);
	//	break;
	//}

	if((indexBase+nVertices)>=65536)
	{
		CRCore::DrawElementsUInt* pPrim = new CRCore::DrawElementsUInt(m_primtype,nVertices);

		CRCore::DrawElementsUInt::PT_IndexArray& lhs = pPrim->getIndexArray();
		for(int i=indexBase;i<(indexBase+nVertices);i++)
		{   
			lhs.push_back(unsigned int(i));
		}
		geom->addPrimitive(pPrim);
	}
	else
	{
		CRCore::DrawElementsUShort* pPrim = new CRCore::DrawElementsUShort(m_primtype,nVertices);

		CRCore::DrawElementsUShort::PT_IndexArray& lhs = pPrim->getIndexArray();
		for(int i=indexBase;i<(indexBase+nVertices);i++)
		{   
			lhs.push_back(unsigned short(i));
		}
		geom->addPrimitive(pPrim);
	}
}


////////////////////////////////////////////////////////////////////
//
//                       GeoSetBuilder
//
////////////////////////////////////////////////////////////////////

// OpenFlight don't save data in GeoSets.  This class tries to find
// existing GeoSets with matching state before creating a new GeoSet.


GeoSetBuilder::GeoSetBuilder(CRCore::crObject* geode)
{
    m_geode = geode;
    initPrimData();
}


void GeoSetBuilder::initPrimData()
{
    m_dynGeoSet = new DynGeoSet;
	m_dynGeoSet->setStateSet(new CRCore::crStateSet);
}

struct SortDynGeoSet
{
    bool operator () (const CRCore::ref_ptr<DynGeoSet>& lhs,const CRCore::ref_ptr<DynGeoSet>& rhs)
    {
        return *lhs<*rhs;
    }
};

CRCore::crObject* GeoSetBuilder::createOsgGeoSets(CRCore::crObject* geode)
{
    if( geode == NULL) geode = m_geode.get();
    if( geode == NULL) return geode;

	for(DynGeoSetList::iterator itr = m_dynGeoSetList.begin();
        itr!=m_dynGeoSetList.end();
        ++itr)
    {
        DynGeoSet* dgset = itr->get();
        CRCore::crGeometry* geom = dgset->getGeometry();
        dgset->addToGeometry(geom);

		CRCore::crStateSet* stateset = dgset->getStateSet();
        assert( stateset == geom->getStateSet() );

		geode->addDrawable(geom);
    }

    //CRUtil::crTesselator tesselator;
    //for(unsigned int i=0;i<geode->getNumDrawables();++i)
    //{
    //    CRCore::crGeometry* geom = dynamic_cast<CRCore::crGeometry*>(geode->getDrawable(i));
    //    if (geom) tesselator.retesselatePolygons(*geom);
    //}

    return geode;
}

CRCore::crBillboard* GeoSetBuilder::createOsgBillboardSets(CRCore::crBillboard* geode)
{
	if( geode == NULL) geode = (CRCore::crBillboard*)(m_geode.get());
	if( geode == NULL) return geode;
    
	if(m_dynGeoSetList.size() <= 0) return geode;
    geode->setStateSet((*m_dynGeoSetList.begin())->getStateSet());

	for(DynGeoSetList::iterator itr = m_dynGeoSetList.begin();
		itr!=m_dynGeoSetList.end();
		++itr)
	{
		DynGeoSet* dgset = itr->get();
		CRCore::crGeometry* geom = dgset->getGeometry();
		dgset->addToGeometry(geom);

		CRCore::crStateSet* stateset = dgset->getStateSet();
		assert( stateset == geom->getStateSet() );
        //stateset->getStateData()->setDrawFlag(CRCore::crStateSet::StateData::SOLID_NO_BACKFACE);
		//stateset->getStateData()->setLightMode(CRCore::crStateSet::StateData::OFF);
		geode->addBBDrawable(geom);
	}

	//CRUtil::crTesselator tesselator;
	//for(unsigned int i=0;i<geode->getNumDrawables();++i)
	//{
	//    CRCore::crGeometry* geom = dynamic_cast<CRCore::crGeometry*>(geode->getDrawable(i));
	//    if (geom) tesselator.retesselatePolygons(*geom);
	//}

	return geode;
}

bool GeoSetBuilder::addPrimitive(const std::string &faceID, bool dontMerge)
{
	DynGeoSet* dgset = getDynGeoSet();  // This is the new geoset we want to add

	//if(dgset->getStateSet()->getStateData()->getNumTex()>1) //多层贴图不做合并
	    //dontMerge = true;

    if (dgset->getPrimType()==NO_PRIMITIVE_TYPE)
    {
        dgset->setPrimType(findPrimType(dgset->coordListSize()));
    }

    // Still no primitive type?
    if (dgset->getPrimType()==NO_PRIMITIVE_TYPE)
        return false;

	switch(dgset->getPrimType()) 
	{
	case(CRCore::crPrimitive::PT_POINTS):
		CRCore::notify(CRCore::NOTICE)<<"GeoSetBuilder::addPrimitive find PT_POINTS FaceID = "<<faceID<<std::endl;
		break;
	case(CRCore::crPrimitive::PT_LINES):
		CRCore::notify(CRCore::NOTICE)<<"GeoSetBuilder::addPrimitive find PT_LINES FaceID = "<<faceID<<std::endl;
		break;
	case(CRCore::crPrimitive::PT_LINE_STRIP):
		CRCore::notify(CRCore::NOTICE)<<"GeoSetBuilder::addPrimitive find PT_LINE_STRIP FaceID = "<<faceID<<std::endl;
		dontMerge = true;
		break;
	case(CRCore::crPrimitive::PT_LINE_LOOP):
		CRCore::notify(CRCore::NOTICE)<<"GeoSetBuilder::addPrimitive find PT_LINE_LOOP FaceID = "<<faceID<<std::endl;
		dontMerge = true;
		break;
	case(CRCore::crPrimitive::PT_TRIANGLE_STRIP):
	case(CRCore::crPrimitive::PT_TRIANGLE_FAN):
	case(CRCore::crPrimitive::PT_QUAD_STRIP):
	case(CRCore::crPrimitive::PT_POLYGON): 
		dontMerge = true;
		break;
	}

    dgset->setBinding();

    if( dontMerge == true)
    {
        m_dynGeoSetList.push_back(dgset);
    }
    else
    {
        DynGeoSet* match = findMatchingGeoSet();
        if (match)
            match->append(dgset);
        else
            m_dynGeoSetList.push_back(dgset);
    }
    
    initPrimData();     // initialize m_dynGeoSet
    return true;
}


DynGeoSet* GeoSetBuilder::findMatchingGeoSet()
{
    DynGeoSet* new_dgset = getDynGeoSet();
    for(DynGeoSetList::iterator itr=m_dynGeoSetList.begin();
        itr!=m_dynGeoSetList.end();
        ++itr)
    {
        DynGeoSet* dgset = itr->get();
        if (*new_dgset == *dgset)
            return dgset;
    }

    return NULL;
}


CRCore::crPrimitive::Mode GeoSetBuilder::findPrimType(const int nVertices)
{
    switch (nVertices)
    {
        case 1:
			CRCore::notify(CRCore::WARN)<<"Find POINTS"<<std::endl;
			return CRCore::crPrimitive::PT_POINTS;
        case 2: 
			CRCore::notify(CRCore::WARN)<<"Find LINES"<<std::endl;
			return CRCore::crPrimitive::PT_LINES;
        case 3: 
			return CRCore::crPrimitive::PT_TRIANGLES;
        case 4:
			CRCore::notify(CRCore::WARN)<<"Find QUADS, 建议将模型三角化"<<std::endl;
			return CRCore::crPrimitive::PT_QUADS;
    }

    if (nVertices>=5)
	{
		CRCore::notify(CRCore::WARN)<<"Find POLYGON, 建议将模型三角化"<<std::endl;
		return CRCore::crPrimitive::PT_POLYGON;
	}
	CRCore::notify(CRCore::WARN)<<"Find PRIMITIVE TYPE Error"<<std::endl;
    return NO_PRIMITIVE_TYPE;
}