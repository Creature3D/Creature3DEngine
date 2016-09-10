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
//Modified by Wucaihua
#include <CRCore/crIntersectVisitor.h>
#include <CRCore/crTransform.h>
#include <CRCore/crObject.h>
#include <CRCore/crLod.h>
#include <CRCore/crSwitch.h>
#include <CRCore/crBillboard.h>
//#include <CRCore/crDepthSortedObject.h>
#include <CRCore/crNotify.h>
#include <CRCore/crTriangleFunctor.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crBrain.h>

#include <float.h>
#include <algorithm>
#include <map>

using namespace CRCore;
crIntersectVisitor::NodeSet crIntersectVisitor::s_avoidIntersectNodeSet;


crHit::crHit()
{
}


crHit::crHit(const crHit& hit)
{
    // copy data across.
    m_ratio = hit.m_ratio;
    m_originalLineSegment = hit.m_originalLineSegment;
    m_localLineSegment = hit.m_localLineSegment;
    m_nodePath = hit.m_nodePath;
    m_geode = hit.m_geode;
    m_drawable = hit.m_drawable;
    m_matrix = hit.m_matrix;
    m_inverse = hit.m_inverse;

    m_vecIndexList = hit.m_vecIndexList;
    m_primitiveIndex = hit.m_primitiveIndex;
    m_intersectPoint = hit.m_intersectPoint;
    m_intersectNormal = hit.m_intersectNormal;
}


crHit::~crHit()
{
}


crHit& crHit::operator = (const crHit& hit)
{
    if (&hit==this) return *this;

    m_matrix = hit.m_matrix;
    m_inverse = hit.m_inverse;
    m_originalLineSegment = hit.m_originalLineSegment;
    m_localLineSegment = hit.m_localLineSegment;

    // copy data across.
    m_ratio = hit.m_ratio;
    m_originalLineSegment = hit.m_originalLineSegment;
    m_localLineSegment = hit.m_localLineSegment;
    m_nodePath = hit.m_nodePath;
    m_geode = hit.m_geode;
    m_drawable = hit.m_drawable;

    m_vecIndexList = hit.m_vecIndexList;
    m_primitiveIndex = hit.m_primitiveIndex;
    m_intersectPoint = hit.m_intersectPoint;
    m_intersectNormal = hit.m_intersectNormal;

    return *this;
}

const CRCore::crVector3 crHit::getWorldIntersectNormal() const
{
    if (m_inverse.valid()) 
    {
        CRCore::crVector3 norm = CRCore::crMatrix::transform3x3(*m_inverse,m_intersectNormal); 
        norm.normalize();
        return norm;
    }
    else return m_intersectNormal;
}



crIntersectVisitor::IntersectState::IntersectState()
{
    m_segmentMaskStack.push_back(0xffffffff);
}


crIntersectVisitor::IntersectState::~IntersectState()
{
}


bool crIntersectVisitor::IntersectState::isCulled(const crBoundingSphere& bs,LineSegmentMask& segMaskOut)
{
    bool hit = false;
    LineSegmentMask mask = 0x00000001;
    segMaskOut = 0x00000000;
    LineSegmentMask segMaskIn = m_segmentMaskStack.back();
    for(IntersectState::LineSegmentList::iterator sitr=m_segList.begin();
        sitr!=m_segList.end();
        ++sitr)
    {
        if ((segMaskIn & mask) && (sitr->second)->intersect(bs))
        {
            segMaskOut = segMaskOut| mask;
            hit = true;
        }
        mask = mask << 1;
    }
    return !hit;
}


bool crIntersectVisitor::IntersectState::isCulled(const crBoundingBox& bb,LineSegmentMask& segMaskOut)
{
    bool hit = false;
    LineSegmentMask mask = 0x00000001;
    segMaskOut = 0x00000000;
    LineSegmentMask segMaskIn = m_segmentMaskStack.back();
    for(IntersectState::LineSegmentList::iterator sitr=m_segList.begin();
        sitr!=m_segList.end();
        ++sitr)
    {
        if ((segMaskIn & mask) && (sitr->second)->intersect(bb))
        {
            segMaskOut = segMaskOut| mask;
            hit = true;
        }
        mask = mask << 1;
    }
    return !hit;
}

crIntersectVisitor::crIntersectVisitor()
{
    // overide the default node visitor mode.
    setTraversalMode(crNodeVisitor::TRAVERSE_ACTIVE_CHILDREN);

	// Initialize eyepoint to 0,0,0
	setEyePoint(crVector3(0.0f,0.0f,0.0f));

	setLODSelectionMode(USE_HEIGHEST_LEVEL_OF_DETAIL); // orignal IntersectVisitor behavior
	//setLODSelectionMode(USE_SEGMENT_START_POINT_AS_EYE_POINT_FOR_LOD_LEVEL_SELECTION);

    reset();
}


crIntersectVisitor::~crIntersectVisitor()
{
}


void crIntersectVisitor::reset()
{
    m_intersectStateStack.clear();

    // create a empty IntersectState on the the intersectStateStack.
    m_intersectStateStack.push_back(new IntersectState);

    //m_nodePath.clear();
    m_segHitList.clear();

	resetAvoidIntersectNodeSet();
}

float crIntersectVisitor::getDistanceToEyePoint(const crVector3& pos, bool /*withLODScale*/) const
{
	if (m_lodSelectionMode==USE_SEGMENT_START_POINT_AS_EYE_POINT_FOR_LOD_LEVEL_SELECTION)
	{
		// LODScale is not available to IntersectVisitor, so we ignore the withLODScale argument
		//if (withLODScale) return (pos-getEyePoint()).length()*getLODScale();
		//else return (pos-getEyePoint()).length();
		return (pos-getEyePoint()).length();
	}
	else
	{
		return 0.0f;
	}
}

bool crIntersectVisitor::hits()
{
    for(LineSegmentHitListMap::iterator itr = m_segHitList.begin();
        itr != m_segHitList.end();
        ++itr)
    {
        if (!(itr->second.empty())) return true;
    }
    return false;
}


void crIntersectVisitor::addLineSegment(crLineSegment* seg)
{
	if (!seg) return;

	if (!seg->valid())
	{
		notify(WARN)<<"Warning: invalid line segment passed to IntersectVisitor::addLineSegment(..)"<<std::endl;
		notify(WARN)<<"         "<<seg->start()<<" "<<seg->end()<<" segment ignored.."<< std::endl;
		return;
	}

	IntersectState* cis = m_intersectStateStack.back().get();

	if (cis->m_segList.size()>=32)
	{
		notify(WARN)<<"Warning: excessive number of line segmenets passed to IntersectVisitor::addLineSegment(..), maximum permitted is 32 line segments."<<std::endl;
		notify(WARN)<<"         "<<seg->start()<<" "<<seg->end()<<" segment ignored.."<< std::endl;
		return;
	}

	setEyePoint(seg->start()); // set start of line segment to be pseudo EyePoint for billboarding and LOD purposes

	// first check to see if segment has already been added.
	for(IntersectState::LineSegmentList::iterator itr = cis->m_segList.begin();
		itr != cis->m_segList.end();
		++itr)
	{
		if (itr->first == seg) return;
	}

	// create a new segment transformed to local coordintes.
	crLineSegment* ns = new crLineSegment;

	if (cis->m_inverse.valid()) ns->mult(*seg,*(cis->m_inverse));
	else *ns = *seg;

	cis->addLineSegmentPair(seg,ns);

}


void crIntersectVisitor::pushMatrix(const crMatrix& matrix)
{
    IntersectState* nis = new IntersectState;

    IntersectState* cis = m_intersectStateStack.back().get();

    if (cis->m_matrix.valid())
    {
        nis->m_matrix = new RefMatrix;
        nis->m_matrix->mult(matrix,*(cis->m_matrix));
    }
    else
    {
        nis->m_matrix = new RefMatrix(matrix);
    }

    RefMatrix* inverse_world = new RefMatrix;
    inverse_world->invert(*(nis->m_matrix));
    nis->m_inverse = inverse_world;

    IntersectState::LineSegmentMask segMaskIn = cis->m_segmentMaskStack.back();
    IntersectState::LineSegmentMask mask = 0x00000001;
    for(IntersectState::LineSegmentList::iterator sitr=cis->m_segList.begin();
        sitr!=cis->m_segList.end();
        ++sitr)
    {
        if ((segMaskIn & mask))
        {
            crLineSegment* seg = new crLineSegment;
            seg->mult(*(sitr->first),*inverse_world);
            nis->addLineSegmentPair(sitr->first.get(),seg);
        }
        mask = mask << 1;
    }

    m_intersectStateStack.push_back(nis);
}


void crIntersectVisitor::popMatrix()
{
    if (!m_intersectStateStack.empty())
    {
        m_intersectStateStack.pop_back();
    }
}


bool crIntersectVisitor::enterNode(crNode& node)
{
    const crBoundingSphere& bs = node.getBound();
    if (bs.valid() && node.isCullingActive())
    {
        IntersectState* cis = m_intersectStateStack.back().get();
        IntersectState::LineSegmentMask sm=0xffffffff;
        if (cis->isCulled(bs,sm)) return false;
        cis->m_segmentMaskStack.push_back(sm);
        //m_nodePath.push_back(&node);
        return true;
    }
    else
    {
		IntersectState* cis = m_intersectStateStack.back().get();
		if (!cis->m_segmentMaskStack.empty()) 
			cis->m_segmentMaskStack.push_back(cis->m_segmentMaskStack.back());
		else
			cis->m_segmentMaskStack.push_back(0xffffffff);

        return true;
    }
}


void crIntersectVisitor::leaveNode()
{
    IntersectState* cis = m_intersectStateStack.back().get();
    cis->m_segmentMaskStack.pop_back();
    //m_nodePath.pop_back();
}


void crIntersectVisitor::apply(crNode& node)
{
	if( !node.getVisiable()||
		!node.getEnableIntersect() ||
		node.getAvoidIntersect() ||
		avoidIntersectNodeCull(&node) ||
		!enterNode(node) ) return;

    traverse(node);

    leaveNode();
}


struct TriangleHit
{
    TriangleHit(unsigned int index, const CRCore::crVector3& normal, float r1, const CRCore::crVector3* v1, float r2, const CRCore::crVector3* v2, float r3, const CRCore::crVector3* v3):
        m_index(index),
        m_normal(normal),
        m_r1(r1),
        m_v1(v1),        
        m_r2(r2),
        m_v2(v2),        
        m_r3(r3),
        m_v3(v3) {}

    unsigned int        m_index;
    const CRCore::crVector3     m_normal;
    float               m_r1;
    const CRCore::crVector3*    m_v1;        
    float               m_r2;
    const CRCore::crVector3*    m_v2;        
    float               m_r3;
    const CRCore::crVector3*    m_v3;        
};


struct TriangleIntersect
{
    CRCore::ref_ptr<crLineSegment> m_seg;

    crVector3    m_s;
    crVector3    m_d;
    float        m_length;

    int          m_index;
    float        m_ratio;
    bool         m_hit;
    
    

    typedef std::multimap<float,TriangleHit> TriangleHitList;
    
    TriangleHitList m_thl;

    TriangleIntersect()
    {
    }

    TriangleIntersect(const crLineSegment& seg,float ratio=FLT_MAX)
    {
        set(seg,ratio);
    }
    
    void set(const crLineSegment& seg,float ratio=FLT_MAX)
    {
        m_seg=new crLineSegment(seg);
        m_hit=false;
        m_index = 0;
        m_ratio = ratio;

        m_s = m_seg->start();
        m_d = m_seg->end()-m_seg->start();
        m_length = m_d.length();
        m_d /= m_length;
    }

    //   bool intersect(const crVector3& v1,const crVector3& v2,const crVector3& v3,float& r)
    inline void operator () (const crVector3& v1,const crVector3& v2,const crVector3& v3, bool treatVertexDataAsTemporary)
    {
        ++m_index;

        if (v1==v2 || v2==v3 || v1==v3) return;

        crVector3 v12 = v2-v1;
        crVector3 n12 = v12^m_d;
        float ds12 = (m_s-v1)*n12;
        float d312 = (v3-v1)*n12;
        if (d312>=0.0f)
        {
            if (ds12<0.0f) return;
            if (ds12>d312) return;
        }
        else                     // d312 < 0
        {
            if (ds12>0.0f) return;
            if (ds12<d312) return;
        }

        crVector3 v23 = v3-v2;
        crVector3 n23 = v23^m_d;
        float ds23 = (m_s-v2)*n23;
        float d123 = (v1-v2)*n23;
        if (d123>=0.0f)
        {
            if (ds23<0.0f) return;
            if (ds23>d123) return;
        }
        else                     // d123 < 0
        {
            if (ds23>0.0f) return;
            if (ds23<d123) return;
        }

        crVector3 v31 = v1-v3;
        crVector3 n31 = v31^m_d;
        float ds31 = (m_s-v3)*n31;
        float d231 = (v2-v3)*n31;
        if (d231>=0.0f)
        {
            if (ds31<0.0f) return;
            if (ds31>d231) return;
        }
        else                     // d231 < 0
        {
            if (ds31>0.0f) return;
            if (ds31<d231) return;
        }
        

        float r3;
        if (ds12==0.0f) r3=0.0f;
        else if (d312!=0.0f) r3 = ds12/d312;
        else return; // the triangle and the line must be parallel intersection.
        
        float r1;
        if (ds23==0.0f) r1=0.0f;
        else if (d123!=0.0f) r1 = ds23/d123;
        else return; // the triangle and the line must be parallel intersection.
        
        float r2;
        if (ds31==0.0f) r2=0.0f;
        else if (d231!=0.0f) r2 = ds31/d231;
        else return; // the triangle and the line must be parallel intersection.

        float total_r = (r1+r2+r3);
        if (total_r!=1.0f)
        {
            if (total_r==0.0f) return; // the triangle and the line must be parallel intersection.
            float inv_total_r = 1.0f/total_r;
            r1 *= inv_total_r;
            r2 *= inv_total_r;
            r3 *= inv_total_r;
        }
        
        crVector3 in = v1*r1+v2*r2+v3*r3;
		if (!in.valid())
		{
			//CRCore::notify(WARN)<<"Warning:: Picked up error in TriangleIntersect"<<std::endl;
			//CRCore::notify(WARN)<<"   ("<<v1<<",\t"<<v2<<",\t"<<v3<<")"<<std::endl;
			//CRCore::notify(WARN)<<"   ("<<r1<<",\t"<<r2<<",\t"<<r3<<")"<<std::endl;
			return;
		}

        float d = (in-m_s)*m_d;

        if (d<0.0f) return;
        if (d>m_length) return;

        CRCore::crVector3 normal = v12^v23;
        normal.normalize();

        float r = d/m_length;

        if (treatVertexDataAsTemporary)
        {
            m_thl.insert(std::pair<const float,TriangleHit>(r,TriangleHit(m_index-1,normal,r1,0,r2,0,r3,0)));
        }
        else
        {
            m_thl.insert(std::pair<const float,TriangleHit>(r,TriangleHit(m_index-1,normal,r1,&v1,r2,&v2,r3,&v3)));
        }
        m_hit = true;

    }

};

bool crIntersectVisitor::intersect(crDrawable& drawable)
{
    bool hitFlag = false;

    IntersectState* cis = m_intersectStateStack.back().get();

    const crBoundingBox& bb = drawable.getBound();

    for(IntersectState::LineSegmentList::iterator sitr=cis->m_segList.begin();
        sitr!=cis->m_segList.end();
        ++sitr)
    {
        if (sitr->second->intersect(bb))
        {

            crTriangleFunctor<TriangleIntersect> ti;
            ti.set(*sitr->second);
            drawable.accept(ti);
            if (ti.m_hit)
            {
            
                CRCore::crGeometry* geometry = drawable.asGeometry();
                

                for(TriangleIntersect::TriangleHitList::iterator thitr=ti.m_thl.begin();
                    thitr!=ti.m_thl.end();
                    ++thitr)
                {
                
                    crHit hit;
                    hit.m_nodePath = m_nodePath;
                    hit.m_matrix = cis->m_matrix;
                    hit.m_inverse = cis->m_inverse;
                    hit.m_drawable = &drawable;
                    if (m_nodePath.empty()) hit.m_geode = NULL;
                    else hit.m_geode = dynamic_cast<crObject*>(m_nodePath.back());

                    TriangleHit& triHit = thitr->second;
                    
                    hit.m_ratio = thitr->first;
                    hit.m_primitiveIndex = triHit.m_index;
                    hit.m_originalLineSegment = sitr->first;
                    hit.m_localLineSegment = sitr->second;

                    hit.m_intersectPoint = sitr->second->start()*(1.0f-hit.m_ratio)+
                        sitr->second->end()*hit.m_ratio;

                    hit.m_intersectNormal = triHit.m_normal;
                    
                    if (geometry)
                    {
                        CRCore::Vec3Array* vertices = dynamic_cast<CRCore::Vec3Array*>(geometry->getVertexArray());
                        if (vertices)
                        {
                            CRCore::crVector3* first = &(vertices->front());
                            if (triHit.m_v1) hit.m_vecIndexList.push_back(triHit.m_v1-first);
                            if (triHit.m_v2) hit.m_vecIndexList.push_back(triHit.m_v2-first);
                            if (triHit.m_v3) hit.m_vecIndexList.push_back(triHit.m_v3-first);
                        }
                    }
                    

                    m_segHitList[sitr->first.get()].push_back(hit);

                    std::sort(m_segHitList[sitr->first.get()].begin(),m_segHitList[sitr->first.get()].end());

                    hitFlag = true;

                }
            }
        }
    }

    return hitFlag;

}

void crIntersectVisitor::apply(crObject& node)
{
	if( !node.getVisiable()||
		!node.getEnableIntersect() ||
		node.getAvoidIntersect() ||
		avoidIntersectNodeCull(&node) ||
		!enterNode(node) ) return;
	//if(node.getCollideMode() == crNode::CollideNone) return;
    for(unsigned int i = 0; i < node.getNumDrawables(); i++ )
    {
        intersect(*node.getDrawable(i));
    }
    
    leaveNode();
}


void crIntersectVisitor::apply(crBillboard& node)
{//billboard ²»×öÅö×²¼ì²â
	//return;
	if( !node.getVisiable()||
		!node.getEnableIntersect() ||
		node.getAvoidIntersect() ||
		avoidIntersectNodeCull(&node) ||
		!enterNode(node) ) return;

    //intersect(*node.getComposeDrawable());
	CRCore::crDrawable* drawable = node.getComposeDrawable();
	//bool hitFlag = false;
	IntersectState* cis = m_intersectStateStack.back().get();
	crBoundingSphere& bs = node.getBound();
	for(IntersectState::LineSegmentList::iterator sitr=cis->m_segList.begin();
		sitr!=cis->m_segList.end();
		++sitr)
	{
		if (sitr->second->intersect(bs))
		{
			crHit hit;
			hit.m_nodePath = m_nodePath;
			hit.m_matrix = cis->m_matrix;
			hit.m_inverse = cis->m_inverse;
			hit.m_drawable = drawable;
			if (m_nodePath.empty()) hit.m_geode = NULL;
			else hit.m_geode = dynamic_cast<crObject*>(m_nodePath.back());

			hit.m_originalLineSegment = sitr->first;
			hit.m_localLineSegment = sitr->second;

			hit.m_intersectPoint = bs.center();

			m_segHitList[sitr->first.get()].push_back(hit);

			std::sort(m_segHitList[sitr->first.get()].begin(),m_segHitList[sitr->first.get()].end());

			//hitFlag = true;
		}
	}

	//popMatrix();

	leaveNode();
}
void crIntersectVisitor::intersectBox(CRCore::crGroup& node)
{
	if( !node.getVisiable()||
		!node.getEnableIntersect() ||
		node.getAvoidIntersect() ||
		avoidIntersectNodeCull(&node) ||
		!enterNode(node) ) return;

	IntersectState* cis = m_intersectStateStack.back().get();
	const crBoundingBox& bbox = node.getBoundBox();
	for(IntersectState::LineSegmentList::iterator sitr=cis->m_segList.begin();
		sitr!=cis->m_segList.end();
		++sitr)
	{
		if (sitr->second->intersect(bbox))
		{
			crHit hit;
			hit.m_nodePath = m_nodePath;
			hit.m_matrix = cis->m_matrix;
			hit.m_inverse = cis->m_inverse;
			hit.m_drawable = NULL;
			hit.m_geode = NULL;

			hit.m_originalLineSegment = sitr->first;
			hit.m_localLineSegment = sitr->second;

			hit.m_intersectPoint = bbox.center();

			m_segHitList[sitr->first.get()].push_back(hit);

			std::sort(m_segHitList[sitr->first.get()].begin(),m_segHitList[sitr->first.get()].end());

			//hitFlag = true;
		}
	}

	//popMatrix();

	leaveNode();
}
//void crIntersectVisitor::apply(crDepthSortedObject& node)
//{
//	if( !node.getVisiable() ||
//		avoidIntersectNodeCull(&node) ||
//		!enterNode(node) ) return;
//
//	intersect(*node.getComposeDrawable());
//
//	leaveNode();
//}

void crIntersectVisitor::apply(crGroup& node)
{
	if(node.getName().compare("ProxyNode") == 0)
	{
		if(!crBrain::getInstance()->getCameraFrustum().contains(node.getBoundBox()))
			return;
	}
	if( !node.getVisiable()||
		!node.getEnableIntersect() ||
		node.getAvoidIntersect() ||
		avoidIntersectNodeCull(&node) ||
		!enterNode(node) ) return;

	//CRCore::notify(CRCore::ALWAYS)<<"crIntersectVisitor::applycrGroup "<<node.getName()<< std::endl;

    traverse(node);

    leaveNode();
}


void crIntersectVisitor::apply(crTransform& node)
{
	if( !node.getVisiable()||
		!node.getEnableIntersect() ||
		node.getAvoidIntersect() ||
		avoidIntersectNodeCull(&node) ||
		!enterNode(node) ) return;

    CRCore::ref_ptr<RefMatrix> matrix = new RefMatrix;
    node.computeLocalToWorldMatrix(*matrix,this);

    pushMatrix(*matrix);

    traverse(node);

    popMatrix();

    leaveNode();
}

void crIntersectVisitor::apply(crSwitch& node)
{
	apply((crGroup&)node);
}


void crIntersectVisitor::apply(crLod& node)
{
	apply((crGroup&)node);
}

crPickVisitor::crPickVisitor(const CRCore::crViewPort* viewport, const CRCore::crMatrixd& proj, const CRCore::crMatrixd& view, float mx, float my):
m_mx(mx),
m_my(my),
m_lastViewport(viewport),
m_lastProjectionMatrix(proj),
m_lastViewMatrix(view)
{
	if (viewport && 
		mx >= static_cast<float>(viewport->x()) && 
		my >= static_cast<float>(viewport->y()) &&
		mx < static_cast<float>(viewport->x()+viewport->width()) &&
		my < static_cast<float>(viewport->y()+viewport->height()))
	{

		// mouse pointer intersect viewport so we can proceed to set up a line segment
		CRCore::crMatrix MVPW = view * proj * viewport->computeWindowMatrix();
		CRCore::crMatrixd inverseMVPW;
		inverseMVPW.invert(MVPW);

		CRCore::crVector3 nearPoint = CRCore::crVector3(mx,my,0.0f) * inverseMVPW;
		CRCore::crVector3 farPoint = CRCore::crVector3(mx,my,1.0f) * inverseMVPW;
		CRCore::crLineSegment* lineSegment = new CRCore::crLineSegment;
		lineSegment->set(nearPoint, farPoint);

		addLineSegment(lineSegment);
	}

}

void crPickVisitor::runNestedPickVisitor(CRCore::crNode& node, const CRCore::crViewPort* viewport, const CRCore::crMatrix& proj, const CRCore::crMatrix& view, float mx, float my)
{
	crPickVisitor newPickVisitor( viewport, proj, view, mx, my );
	newPickVisitor.setTraversalMask(getTraversalMask());

	// the new pickvisitor over the nodes children.
	node.traverse( newPickVisitor );

	for(LineSegmentHitListMap::iterator itr = newPickVisitor.m_segHitList.begin();
		itr != newPickVisitor.m_segHitList.end();
		++itr)
	{
		m_segHitList.insert(*itr);
	}
}

void crPickVisitor::apply(CRCore::crProjection& projection)
{
	runNestedPickVisitor( projection, 
		m_lastViewport.get(), 
		projection.getMatrix(),
		m_lastViewMatrix,
		m_mx, m_my );
}

void crPickVisitor::apply(CRCore::crCameraNode& camera)
{
	if(camera.getVisiable() && camera.getEnableIntersect() && !camera.getAvoidIntersect() && camera.getReferenceFrame()==CRCore::crCameraNode::RELATIVE_TO_PARENTS)
	    traverse(camera);
	//if (!camera.isRenderToTextureCamera())
	//{
	//	if (camera.getReferenceFrame()==CRCore::crCameraNode::RELATIVE_TO_ABSOLUTE)
	//	{
	//		runNestedPickVisitor( camera,
	//			camera.getViewport() ? camera.getViewport() : m_lastViewport.get(),
	//			*camera.getProjectionMatrix(), 
	//			*camera.getViewMatrix(),
	//			m_mx, m_my );
	//	}
	//	else if (camera.getTransformOrder()==CRCore::crCameraNode::POST_MULTIPLE)
	//	{
	//		runNestedPickVisitor( camera,
	//			camera.getViewport() ? camera.getViewport() : m_lastViewport.get(),
	//			m_lastProjectionMatrix * *camera.getProjectionMatrix(), 
	//			m_lastViewMatrix * *camera.getViewMatrix(),
	//			m_mx, m_my );
	//	}
	//	else // PRE_MULTIPLE
	//	{
	//		runNestedPickVisitor( camera,
	//			camera.getViewport() ? camera.getViewport() : m_lastViewport.get(),
	//			*camera.getProjectionMatrix() * m_lastProjectionMatrix, 
	//			*camera.getViewMatrix() * m_lastViewMatrix,
	//			m_mx, m_my );
	//	}
	//}
}
