#ifndef __FLT_GEOSETBUILDER_H
#define __FLT_GEOSETBUILDER_H

// Added DynGeoSet::setDetailTextureAttrData that is used to store texture Attributes
// Julian Ortiz, June 18th 2003.

#include <CRCore/ref_ptr.h>
#include <CRCore/crVector2.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crObject.h>
//#include <CRCore/crDepthSortedObject.h>
#include <CRCore/crBillboard.h>
#include <CRCore/crMaterial.h>
#include <CRCore/crStateSet.h>

#include <Plugin flt\AttrData.h>

#include <map>
#include <list>


namespace flt {

class Record;
class TmpGeoSet;



////////////////////////////////////////////////////////////////////
//
//                       DynGeoSet
//
////////////////////////////////////////////////////////////////////
#if 1
#  define COMPARE_DynGeoSet_Parameter(parameter) \
        if (parameter<rhs.parameter) return -1; \
        if (rhs.parameter<parameter) return 1;
#else
#  define COMPARE_DynGeoSet_Parameter(parameter) \
        if (parameter != rhs.parameter) return -1;
#endif

/** DynGeoSet - Dynamic GeoSet.
  */ 
class DynGeoSet : public CRCore::Referenced
{
public:

    DynGeoSet();

    int compare(const DynGeoSet& rhs) const
    {
		COMPARE_DynGeoSet_Parameter(m_primtype)
		COMPARE_DynGeoSet_Parameter(m_color_binding)
        COMPARE_DynGeoSet_Parameter(m_normal_binding)
        COMPARE_DynGeoSet_Parameter(m_texture_binding)

        if (m_color_binding == CRCore::crGeometry::BIND_OVERALL)
        {
            if ((m_colorList.size() >= 1) &&  (rhs.m_colorList.size() >= 1))
            {
                if (m_colorList[0]<rhs.m_colorList[0]) return -1;
                if (rhs.m_colorList[0]<m_colorList[0]) return 1;
            }
        }
                
		int result=getStateSet()->compare(*rhs.getStateSet(),true);
        
		return result;

    }
    
    int compatible(const DynGeoSet& rhs) const
    {
    
        COMPARE_DynGeoSet_Parameter(m_color_binding)
        COMPARE_DynGeoSet_Parameter(m_texture_binding)

        int result=getStateSet()->compare(*rhs.getStateSet(),true);
        if (result!=0) return result;

        COMPARE_DynGeoSet_Parameter(m_normal_binding)

        return 0;


        if (m_color_binding == CRCore::crGeometry::BIND_OVERALL)
        {
            if ((m_colorList.size() >= 1) &&  (rhs.m_colorList.size() >= 1))
            {
                if (m_colorList[0]<rhs.m_colorList[0]) return -1;
                if (rhs.m_colorList[0]<m_colorList[0]) return 1;
            }
        }

        return 0;

    }
        
    bool operator <  (const DynGeoSet& rhs) const { return compare(rhs)<0; }
    bool operator == (const DynGeoSet& rhs) const { return compare(rhs)==0; }
    bool operator != (const DynGeoSet& rhs) const { return compare(rhs)!=0; }

    void setStateSet(CRCore::crStateSet* stateset) 
	{
        m_stateset = stateset;
        m_geom->setStateSet( stateset );
	}
    CRCore::crStateSet* getStateSet() { return m_stateset.get(); }
    const CRCore::crStateSet* getStateSet() const { return m_stateset.get(); }
    
    void setColorBinding(CRCore::crGeometry::AttributeBinding bind) { m_color_binding = bind; }
    void setNormalBinding(CRCore::crGeometry::AttributeBinding bind) { m_normal_binding = bind; }
    void setTextureBinding(CRCore::crGeometry::AttributeBinding bind) { m_texture_binding = bind; }

    CRCore::crGeometry::AttributeBinding getColorBinding() const { return m_color_binding; }
    CRCore::crGeometry::AttributeBinding getNormalBinding() const { return m_normal_binding; }
    CRCore::crGeometry::AttributeBinding getTextureBinding() const { return m_texture_binding; }

    void setPrimType(CRCore::crPrimitive::Mode type) { m_primtype=type; }
    CRCore::crPrimitive::Mode getPrimType() const { return m_primtype; }

    inline void addPrimLen(int len)           { m_primLenList.push_back(len); }
    inline void addCoord(const CRCore::crVector3& coord)    { m_coordList.push_back(coord); }
    inline void addNormal(const CRCore::crVector3& normal)  { m_normalList.push_back(normal); }
    inline void addColor(CRCore::crVector4& color)    { color.w() = m_alpha; m_colorList.push_back(color); }
    inline void addTCoord(const CRCore::crVector2& tcoord)  { m_tcoordList.push_back(tcoord); }
	inline void setAlpha( float alpha ){ m_alpha = alpha; }

    typedef std::vector<CRCore::crVector3>  CoordList;
    typedef std::vector<CRCore::crVector3>  NormalList;
    typedef std::vector<CRCore::crVector4>  ColorList;
    typedef std::vector<CRCore::crVector2>  TcoordList;

	const CoordList&  getCoordList()    { return m_coordList; }
	const NormalList& getNormalList()   { return m_normalList; }
	const ColorList&  getColorList()    { return m_colorList; }
	const TcoordList& getTcoordList()   { return m_tcoordList; }

    void append(DynGeoSet* source);
    void setBinding();
    
    void addToGeometry(CRCore::crGeometry* geom);

    inline int primLenListSize() const { return m_primLenList.size(); }
    inline int coordListSize() const { return m_coordList.size(); }
    inline int normalListSize() const { return m_normalList.size(); }
    inline int colorListSize() const { return m_colorList.size(); }
    inline int tcoordListSize() const { return m_tcoordList.size(); }
    inline void setDetailTextureAttrData(AttrData *attrdata) {m_attrdata=attrdata; }

    CRCore::crGeometry* getGeometry() 
	{
        //CERR  << "m_geom.get(): " << m_geom.get()
        //    << "; referenceCount: " << m_geom.get()->referenceCount()<<"\n";
        return m_geom.get();
    };

private:

    typedef std::vector<int>        PrimLenList;

    CRCore::ref_ptr<CRCore::crGeometry>  m_geom;

    CRCore::ref_ptr<CRCore::crStateSet> m_stateset;


    CRCore::crPrimitive::Mode   m_primtype;
    PrimLenList                 m_primLenList;

    CoordList                   m_coordList;

    CRCore::crGeometry::AttributeBinding m_normal_binding;
    NormalList                  m_normalList;

    CRCore::crGeometry::AttributeBinding m_color_binding;
    ColorList                   m_colorList;

    CRCore::crGeometry::AttributeBinding m_texture_binding;
    TcoordList                  m_tcoordList;

    AttrData                    *m_attrdata;

	float                       m_alpha;
};



////////////////////////////////////////////////////////////////////
//
//                       GeoSetBuilder
//
////////////////////////////////////////////////////////////////////

/** GeoSetBuilder - Contains a list of TmpGeoSets to be converted to CRCore::crObject.
  * 
  */

class GeoSetBuilder
{
public:
    GeoSetBuilder(CRCore::crObject* geode = NULL);
    virtual ~GeoSetBuilder() {}

	bool addPrimitive( const std::string &faceID = "", bool dontMerge = false );
    CRCore::crObject* createOsgGeoSets(CRCore::crObject* geode = NULL);
	CRCore::crBillboard* createOsgBillboardSets(CRCore::crBillboard* geode = NULL);

    inline DynGeoSet* getDynGeoSet() { return m_dynGeoSet.get(); }
    inline const DynGeoSet* getDynGeoSet() const { return m_dynGeoSet.get(); }
    inline bool empty()    { return m_dynGeoSetList.empty(); } ;

protected:

    void initPrimData();
    DynGeoSet* findMatchingGeoSet();
    CRCore::crPrimitive::Mode findPrimType(const int nVertices);

private:

    CRCore::ref_ptr<CRCore::crObject>    m_geode;
    CRCore::ref_ptr<DynGeoSet>     m_dynGeoSet;

    typedef std::list<CRCore::ref_ptr<DynGeoSet> > DynGeoSetList;
    DynGeoSetList                  m_dynGeoSetList;
};


}; // end of namespace flt


#endif

