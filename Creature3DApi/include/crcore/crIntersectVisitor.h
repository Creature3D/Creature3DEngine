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
#ifndef CRCORE_CRINTERSECTVISITOR
#define CRCORE_CRINTERSECTVISITOR 1

#include <CRCore/crNodeVisitor.h>
#include <CRCore/crLineSegment.h>
#include <CRCore/crObject.h>
#include <CRCore/crMatrix.h>
#include <CRCore/crCameraNode.h>
#include <CRCore/crProjection.h>
#include <CRCore/crExport.h>

#include <map>
#include <set>
#include <vector>

namespace CRCore {


class CR_EXPORT crHit
{
    public:

        crHit();
        crHit(const crHit& hit);
        virtual ~crHit();
        
        crHit& operator = (const crHit& hit);
        
        typedef std::vector<int> VecIndexList;

        bool operator < (const crHit& hit) const
        {
            if (m_originalLineSegment<hit.m_originalLineSegment) return true;
            if (m_originalLineSegment>hit.m_originalLineSegment) return false;
            return m_ratio<hit.m_ratio;
        }
        
        
        const CRCore::crVector3& getLocalIntersectPoint() const { return m_intersectPoint; }
        const CRCore::crVector3& getLocalIntersectNormal() const { return m_intersectNormal; }
        
        const CRCore::crVector3 getWorldIntersectPoint() const { if (m_matrix.valid()) return m_intersectPoint*(*m_matrix); else return m_intersectPoint; }
        const CRCore::crVector3 getWorldIntersectNormal() const ;

		float getRatio() const { return m_ratio; }
		const CRCore::crLineSegment* getOriginalLineSegment() const { return m_originalLineSegment.get(); }
		const CRCore::crLineSegment* getLocalLineSegment() const { return m_localLineSegment.get(); }
		CRCore::NodePath& getNodePath() { return m_nodePath; }
		const CRCore::NodePath& getNodePath() const { return m_nodePath; }
		CRCore::crObject* getObject() { return m_geode.get(); }
		const CRCore::crObject* getObject() const { return m_geode.get(); }
		CRCore::crDrawable* getDrawable() { return m_drawable.get(); }
		const CRCore::crDrawable* getDrawable() const { return m_drawable.get(); }
		const CRCore::RefMatrix* getMatrix() const { return m_matrix.get(); }
		const CRCore::RefMatrix* getInverseMatrix() const { return m_inverse.get(); }
		const VecIndexList& getVecIndexList() const { return m_vecIndexList; }
		int getPrimitiveIndex() const { return m_primitiveIndex; }

		CRCore::crNode* getHitNodeByName(const std::string& name) 
		{ 
			for( CRCore::NodePath::iterator itr = m_nodePath.begin();
				 itr != m_nodePath.end();
				 ++itr )
			{
				if(name.compare((*itr)->getName()) == 0)
					return *itr;
			}
			return NULL;
		}
		const CRCore::crNode* getHitNodeByName(const std::string& name) const
		{
			for( CRCore::NodePath::const_iterator itr = m_nodePath.begin();
				itr != m_nodePath.end();
				++itr )
			{
				if(name.compare((*itr)->getName()) == 0)
					return *itr;
			}
			return NULL;
		}

		CRCore::crNode* getHitNodeByClassName(const std::string& name) 
		{ 
			for( CRCore::NodePath::iterator itr = m_nodePath.begin();
				itr != m_nodePath.end();
				++itr )
			{
				if(name.compare((*itr)->className()) == 0)
					return *itr;
			}
			return NULL;
		}
		const CRCore::crNode* getHitNodeByClassName(const std::string& name) const
		{
			for( CRCore::NodePath::const_iterator itr = m_nodePath.begin();
				itr != m_nodePath.end();
				++itr )
			{
				if(name.compare((*itr)->className()) == 0)
					return *itr;
			}
			return NULL;
		}
		

        float                                   m_ratio;
        CRCore::ref_ptr<CRCore::crLineSegment>  m_originalLineSegment;
        CRCore::ref_ptr<CRCore::crLineSegment>  m_localLineSegment;
        CRCore::NodePath                        m_nodePath;
        CRCore::ref_ptr<CRCore::crObject>       m_geode;
        CRCore::ref_ptr<CRCore::crDrawable>     m_drawable;
        CRCore::ref_ptr<CRCore::RefMatrix>      m_matrix;
        CRCore::ref_ptr<CRCore::RefMatrix>      m_inverse;
        
        VecIndexList                            m_vecIndexList;
        int                                     m_primitiveIndex;
        CRCore::crVector3                       m_intersectPoint;
        CRCore::crVector3                       m_intersectNormal;


};


/** Basic visitor for ray based collisions of a scene.*/
class CR_EXPORT crIntersectVisitor : public CRCore::crNodeVisitor
{
    public:

        crIntersectVisitor();
        virtual ~crIntersectVisitor();

        void reset();
        
        /** Add a line segment to use for intersection testing during scene traversal.*/
        void addLineSegment(CRCore::crLineSegment* seg);

        //typedef std::multiset<crHit> HitList;
        typedef std::vector<crHit> HitList;
        typedef std::map<CRCore::crLineSegment*,HitList > LineSegmentHitListMap;
        HitList& getHitList(CRCore::crLineSegment* seg) { return m_segHitList[seg]; }
        int getNumHits(CRCore::crLineSegment* seg) { return m_segHitList[seg].size(); }
        LineSegmentHitListMap& getSegHitList() { return m_segHitList; }

        bool hits();

		enum LODSelectionMode
		{
			USE_HEIGHEST_LEVEL_OF_DETAIL,
			USE_SEGMENT_START_POINT_AS_EYE_POINT_FOR_LOD_LEVEL_SELECTION
		};

		void setLODSelectionMode(LODSelectionMode mode) { m_lodSelectionMode = mode; }
		LODSelectionMode getLODSelectionMode() const { return m_lodSelectionMode; }
		/** Set the eye point in local coordinates.
		* This is a pseudo-EyePoint for billboarding and LOD purposes.
		* It is copied from the Start point of the most-recently-added segment
		* of the intersection ray set (IntersectState::_segList). */
		void setEyePoint(const CRCore::crVector3& eye) { m_pseudoEyePoint = eye; }

		virtual CRCore::crVector3 getEyePoint() const { return m_pseudoEyePoint; }

		/** Get the distance from a point to the eye point, distance value in local coordinate system.
		* This is calculated using the pseudo-EyePoint (above) when doing LOD calculcations. */
		virtual float getDistanceToEyePoint(const CRCore::crVector3& pos, bool withLODScale) const;

        virtual void apply(CRCore::crNode&);
        virtual void apply(CRCore::crObject& node);
        virtual void apply(CRCore::crBillboard& node);
        //virtual void apply(CRCore::crDepthSortedObject& node);
        virtual void apply(CRCore::crGroup& node);
        virtual void apply(CRCore::crTransform& node);        
		virtual void apply(CRCore::crSwitch& node);
        virtual void apply(CRCore::crLod& node);
		virtual void apply(CRPhysics::crMatterObject& bot){ intersectBox((crGroup&)bot); }
       
		typedef std::set<CRCore::crNode*> NodeSet;
		typedef std::set<std::string> NodeTypeSet;
		void resetAvoidIntersectNodeSet(){ m_avoidIntersectNodeSet.clear(); }
        void insertAvoidIntersectNode(CRCore::crNode *node){ m_avoidIntersectNodeSet.insert(node); }
		void insertAvoidIntersectNodeType(const std::string &nodeType){ m_avoidIntersectNodeTypeSet.insert(nodeType); }

		static void insertStaticAvoidIntersectNode(CRCore::crNode *node){ s_avoidIntersectNodeSet.insert(node); }
		static void clearStaticAvoidIntersectNode(){ s_avoidIntersectNodeSet.clear(); }

		bool avoidIntersectNodeCull(CRCore::crNode *node)
		{ 
			return m_avoidIntersectNodeSet.find(node)!=m_avoidIntersectNodeSet.end() ||
				   m_avoidIntersectNodeTypeSet.find(node->className())!=m_avoidIntersectNodeTypeSet.end() || 
				   s_avoidIntersectNodeSet.find(node)!=s_avoidIntersectNodeSet.end();
		}

    protected:

        class IntersectState : public CRCore::Referenced
        {
            public:

                IntersectState();

                CRCore::ref_ptr<CRCore::RefMatrix> m_matrix;
                CRCore::ref_ptr<CRCore::RefMatrix> m_inverse;

                typedef std::pair<CRCore::ref_ptr<CRCore::crLineSegment>,CRCore::ref_ptr<CRCore::crLineSegment> >   LineSegmentPair;
                typedef std::vector< LineSegmentPair > LineSegmentList;
                LineSegmentList m_segList;

                typedef unsigned int LineSegmentMask;
                typedef std::vector<LineSegmentMask> LineSegmentMaskStack;
                LineSegmentMaskStack m_segmentMaskStack;

                bool isCulled(const CRCore::crBoundingSphere& bs,LineSegmentMask& segMaskOut);
                bool isCulled(const CRCore::crBoundingBox& bb,LineSegmentMask& segMaskOut);

                void addLineSegmentPair(CRCore::crLineSegment* first,CRCore::crLineSegment* second)
                {
                    m_segList.push_back(LineSegmentPair(first,second));
                }

            protected:

                ~IntersectState();

        };

        bool intersect(CRCore::crDrawable& gset);

        void pushMatrix(const CRCore::crMatrix& matrix);
        void popMatrix();

        bool enterNode(CRCore::crNode& node);
        void leaveNode();

		void intersectBox(CRCore::crGroup& bot);

        typedef std::vector<CRCore::ref_ptr<IntersectState> > IntersectStateStack;
        
        IntersectStateStack         m_intersectStateStack;

        LineSegmentHitListMap       m_segHitList;

		LODSelectionMode            m_lodSelectionMode;
		CRCore::crVector3           m_pseudoEyePoint;

		NodeSet m_avoidIntersectNodeSet;
		static NodeSet s_avoidIntersectNodeSet;
		NodeTypeSet m_avoidIntersectNodeTypeSet;
};

/** Picking intersection visitor specialises the IntersectVistor to allow more convinient handling of mouse picking.*/
class CR_EXPORT crPickVisitor : public crIntersectVisitor
{
public:

	crPickVisitor(const CRCore::crViewPort* viewport, const CRCore::crMatrixd& proj, const CRCore::crMatrixd& view, float mx, float my);

	void runNestedPickVisitor(CRCore::crNode& node, const CRCore::crViewPort* viewport, const CRCore::crMatrix& proj, const CRCore::crMatrix& view, float mx, float my);

	void apply(CRCore::crProjection& projection);

	void apply(CRCore::crCameraNode& camera);

protected:

	float m_mx;
	float m_my;

	CRCore::ref_ptr<const CRCore::crViewPort>   m_lastViewport;
	CRCore::crMatrixd                        m_lastProjectionMatrix;
	CRCore::crMatrixd                        m_lastViewMatrix;
};

}

#endif

