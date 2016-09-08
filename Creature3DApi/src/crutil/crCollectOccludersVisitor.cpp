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
#include <CRUtil/crCollectOccludersVisitor.h>

#include <CRCore/crConvexPlanarOccluder.h>
#include <CRCore/crTransform.h>
#include <CRCore/crSwitch.h>
#include <CRCore/crLod.h>
#include <CRCore/crBoundingBox.h>
#include <CRCore/crObject.h>
#include <CRCore/crOccluderNode.h>
#include <CRCore/crProjection.h>
#include <CRCore/crMatrix.h>

#include <algorithm>
#include <stdio.h>

using namespace CRCore;
using namespace CRUtil;

crCollectOccludersVisitor::crCollectOccludersVisitor():
    crNodeVisitor(COLLECT_OCCLUDER_VISITOR,TRAVERSE_ACTIVE_CHILDREN)
{

    setCullingMode(VIEW_FRUSTUM_CULLING|
                   NEAR_PLANE_CULLING|
                   FAR_PLANE_CULLING|
                   SMALL_FEATURE_CULLING);
    
//    m_minCalculateValue = 300.0f;
	m_minShadowOccluderVolume = 0.005f;
    m_maxNumberOfActiveOccluders = 10;
//	m_maxNumberOfCollectOccluders = 15;
    m_createDrawables = false;
    
}

crCollectOccludersVisitor::~crCollectOccludersVisitor()
{
}

void crCollectOccludersVisitor::reset()
{
    crCullStack::reset();
    m_occluderSet.clear();
}

float crCollectOccludersVisitor::getDistanceToEyePoint(const crVector3& pos, bool withLODScale) const
{
    if (withLODScale) return (pos-getEyeLocal()).length()*getLODScale();
    else return (pos-getEyeLocal()).length();
}

float crCollectOccludersVisitor::getDistanceFromEyePoint(const crVector3& pos, bool withLODScale) const
{
    const crMatrix& matrix = *m_modelviewStack.back();
    float dist = -(pos[0]*matrix(0,2)+pos[1]*matrix(1,2)+pos[2]*matrix(2,2)+matrix(3,2));
    
    if (withLODScale) return dist*getLODScale();
    else return dist*getLODScale();
}

void crCollectOccludersVisitor::apply(CRCore::crNode& node)
{
//	if(m_occluderSet.size()>m_maxNumberOfCollectOccluders) return;

	if ( isCulled(node)|| 
		(node.getStateSet()&&
		node.getStateSet()->getRenderingHint() == crStateSet::TRANSPARENT_BIN) ) return;

    // push the culling mode.
    pushCurrentMask();
    
    traverse(node);
    
    // pop the culling mode.
    popCurrentMask();
}

void crCollectOccludersVisitor::apply(CRCore::crTransform& node)
{
 //   if(m_occluderSet.size()>m_maxNumberOfCollectOccluders) return;

	if ( isCulled(node)|| 
		(node.getStateSet()&&
		node.getStateSet()->getRenderingHint() == crStateSet::TRANSPARENT_BIN) ) return;

    // push the culling mode.
    pushCurrentMask();

    CRCore::RefMatrix* matrix = createOrReuseMatrix(getModelViewMatrix());
    node.computeLocalToWorldMatrix(*matrix,this);
    pushModelViewMatrix(matrix);
    
    traverse(node);

    popModelViewMatrix();

    // pop the culling mode.
    popCurrentMask();
}

void crCollectOccludersVisitor::apply(CRCore::crProjection& node)
{
    if (isCulled(node)) return;

    // push the culling mode.
    pushCurrentMask();

    CRCore::RefMatrix* matrix = createOrReuseMatrix(node.getMatrix());
    pushProjectionMatrix(matrix);
    
    traverse(node);

    popProjectionMatrix();

    // pop the culling mode.
    popCurrentMask();
}

void crCollectOccludersVisitor::apply(CRCore::crSwitch& node)
{
//	if(m_occluderSet.size()>m_maxNumberOfCollectOccluders) return;

	apply((crGroup&)node);
}

void crCollectOccludersVisitor::apply(CRCore::crLod& node)
{
//	if(m_occluderSet.size()>m_maxNumberOfCollectOccluders) return;
 
	if ( isCulled(node) || 
		(node.getStateSet()&&
		node.getStateSet()->getRenderingHint() == crStateSet::TRANSPARENT_BIN) ) return;

    // push the culling mode.
    pushCurrentMask();

    traverse(node);

    // pop the culling mode.
    popCurrentMask();
}

//void crCollectOccludersVisitor::creatOccluder(const crVector3& v1,const crVector3& v2,const crVector3& v3,const crVector3& v4 )
//{	
//	//根据该物体生成crConvexPlanarOccluder
//	crConvexPlanarOccluder cpo;
//	crConvexPlanarPolygon& occluder = cpo.getOccluder();
//	occluder.add(v1);
//	occluder.add(v2);
//	occluder.add(v3);
//	occluder.add(v4);
//
//	//生成该遮挡物的阴影集
//	crShadowVolumeOccluder svo;
//
//	if ( svo.computeOccluder( m_nodePath, cpo, *this,m_createDrawables ) )
//	{   
//		if ( svo.getVolume() > m_minShadowOccluderVolume )
//		{//该遮挡物的阴影集>可为遮挡物的要求
//			// need to test occluder against view frustum.
//			m_occluderSet.insert(svo);
//		}
//	}
//}
//
//void crCollectOccludersVisitor::apply(CRCore::crObject& node)
//{
//	if(m_occluderSet.size()>m_maxNumberOfCollectOccluders) return;
//
//	// need to check if occlusion node is in the occluder
//    //list, if so disable the appropriate ShadowOccluderVolume
//    disableAndPushOccludersCurrentMask(m_nodePath);
//
//	if ( isCulled(node)|| 
//		(node.getStateSet()&&
//		node.getStateSet()->getRenderingHint() == crStateSet::TRANSPARENT_BIN) )
//    {
//        popOccludersCurrentMask(m_nodePath);
//        return;
//    }
//		
//	//估算该物体的计算价值
//    const crVector3 &eyeLocal = getEyeLocal();
//    const crBoundingBox &bb = node.getBoundBox();
//    
//	float boxLength= bb.xMax()-bb.xMin();
//	float boxWidth = bb.yMax()-bb.yMin();
//	float boxHeight= bb.zMax()-bb.zMin();
//    
//	float distanceBetweenObjectAndEye = (bb.center() - eyeLocal).length();
//	float boxVolume = boxLength * boxWidth * boxHeight;
//	float sixSideArea = 2 *( boxLength * boxWidth + boxLength * boxHeight + boxWidth * boxHeight );
//
//	float value = ( boxVolume + sixSideArea ) / distanceBetweenObjectAndEye;
//
//	if ( value < m_minCalculateValue )
//	{
//		popOccludersCurrentMask(m_nodePath);
//		return;
//	}
//	// push the culling mode.
//	pushCurrentMask();
//	switch( getOccluderFace(bb) ) 
//	{
//	case LEFT:
//	// left side
//		creatOccluder(bb.corner(2),
//			bb.corner(0),
//			bb.corner(4),
//			bb.corner(6));
//		break;
//	case RIGHT:
//		// right side
//		creatOccluder(bb.corner(1),
//			bb.corner(3),
//			bb.corner(7),
//			bb.corner(5));
//		break;
//	case FRONT:
//		//front
//		creatOccluder(bb.corner(0),
//			bb.corner(1),
//			bb.corner(5),
//			bb.corner(4));
//		break;
//	case FRONTLEFT:
//	//front
//		creatOccluder(bb.corner(0),
//			bb.corner(1),
//			bb.corner(5),
//			bb.corner(4));
//		// left side
//		creatOccluder(bb.corner(2),
//			bb.corner(0),
//			bb.corner(4),
//			bb.corner(6));
//		break;
//	case FRONTRIGHT:
//	//front
//		creatOccluder(bb.corner(0),
//			bb.corner(1),
//			bb.corner(5),
//			bb.corner(4));
//		// right side
//		creatOccluder(bb.corner(1),
//			bb.corner(3),
//			bb.corner(7),
//			bb.corner(5));
//		break;
//	case BACK:
//	// back side
//		creatOccluder(bb.corner(3),
//			bb.corner(2),
//			bb.corner(6),
//			bb.corner(7));
//		break;
//	case BACKLEFT:
//	// back side
//		creatOccluder(bb.corner(3),
//			bb.corner(2),
//			bb.corner(6),
//			bb.corner(7));
//		// left side
//		creatOccluder(bb.corner(2),
//			bb.corner(0),
//			bb.corner(4),
//			bb.corner(6));
//		break;
//	case BACKRIGHT:
//	// back side
//		creatOccluder(bb.corner(3),
//			bb.corner(2),
//			bb.corner(6),
//			bb.corner(7));
//		// right side
//		creatOccluder(bb.corner(1),
//			bb.corner(3),
//			bb.corner(7),
//			bb.corner(5));
//		break;
//	case ALL:
//	//front
//		creatOccluder(bb.corner(0),
//			bb.corner(1),
//			bb.corner(5),
//			bb.corner(4));
//
//		// right side
//		creatOccluder(bb.corner(1),
//			bb.corner(3),
//			bb.corner(7),
//			bb.corner(5));
//
//		// left side
//		creatOccluder(bb.corner(2),
//			bb.corner(0),
//			bb.corner(4),
//			bb.corner(6));
//
//		// back side
//		creatOccluder(bb.corner(3),
//			bb.corner(2),
//			bb.corner(6),
//			bb.corner(7));
//		break;
//	}
//
//	//traverse(node);
//
//    // pop the culling mode.
//    popCurrentMask();
//	    
//	// pop the current mask for the disabled occluder
//	popOccludersCurrentMask(m_nodePath);
//}

void crCollectOccludersVisitor::apply(CRCore::crOccluderNode& node)
{
    // need to check if occlusion node is in the occluder
    // list, if so disable the appropriate ShadowOccluderVolume
    disableAndPushOccludersCurrentMask(m_nodePath);
    

    if (isCulled(node))
    {
        popOccludersCurrentMask(m_nodePath);
        return;
    }

    // push the culling mode.
    pushCurrentMask();


    if (node.getOccluder())
    {
        // computeOccluder will check if the occluder is the view frustum,
        // if it ins't then the it will return false, when in it will
        // clip the occluder's polygons in clip space, then create occluder
        // planes, all with their normals facing inward towards the volume,
        // and then transform them back into projection space.
        crShadowVolumeOccluder svo;
        if (svo.computeOccluder(m_nodePath, *node.getOccluder(), *this,m_createDrawables))
        {
        
            if (svo.getVolume()>m_minShadowOccluderVolume)
            {
                // need to test occluder against view frustum.
                m_occluderSet.insert(svo);
            }
        }
    }

    traverse(node);

    // pop the culling mode.
    popCurrentMask();
    
    // pop the current mask for the disabled occluder
    popOccludersCurrentMask(m_nodePath);
}

void crCollectOccludersVisitor::removeOccludedOccluders()
{
    if (m_occluderSet.empty()) return;
        
    ShadowVolumeOccluderSet::iterator occludeeItr=m_occluderSet.begin();
    
    // skip the first element as this can't be occluded by anything else.
    occludeeItr++;

    // step through the rest of the occluders, remove occluders which are themselves occluded.
    for(;
        occludeeItr!=m_occluderSet.end();
        ++occludeeItr)
    {
        
        // search for any occluders that occlude the current occluder,
        // we only need to test any occluder near the front of the set since
        // you can't be occluder by something smaller than you.
        crShadowVolumeOccluder& occludee = const_cast<crShadowVolumeOccluder&>(*occludeeItr);
        crShadowVolumeOccluder::HoleList& holeList = occludee.getHoleList();

        for(ShadowVolumeOccluderSet::iterator occluderItr=m_occluderSet.begin();
            occluderItr!=occludeeItr;
            ++occluderItr)
        {
            // cast away constness of the std::set element since crShadowVolumeOccluder::contains() is non const,
            // and the std::set is a const, just for the invariance of the operator <!! Ahhhhh. oh well the below
            // should be robust since contains won't change the getVolume which is used by the operator <.  Honest,  :-)
            crShadowVolumeOccluder* occluder = const_cast<crShadowVolumeOccluder*>(&(*occluderItr));
            if (occluder->contains(occludee.getOccluder().getReferenceVertexList()))
            {
                // erase occluder from set.
                // take a copy of the iterator then rewind it one element so to prevent invalidating the occludeeItr.
                ShadowVolumeOccluderSet::iterator eraseItr = occludeeItr--;
                m_occluderSet.erase(eraseItr);
                break;
            }
            
            // now check all the holes in the occludee against the occluder, 
            // do so in reverse order so that the iterators remain valid.
            for(crShadowVolumeOccluder::HoleList::reverse_iterator holeItr=holeList.rbegin();
                holeItr!=holeList.rend();
                )
            {
                if (occluder->contains((*holeItr).getReferenceVertexList()))
                {
                    holeList.erase((++holeItr).base());
                }
                else
                {
                    ++holeItr;
                }
                
            }
            
        }
    }
    

    if (m_occluderSet.size()<=m_maxNumberOfActiveOccluders) return;
    
    // move the iterator to the m_maxNumberOfActiveOccluders th occluder.
    occludeeItr = m_occluderSet.begin();
    for(unsigned int i=0;i<m_maxNumberOfActiveOccluders;++i)
        ++occludeeItr;
        
    // discard last occluders.
    m_occluderSet.erase(occludeeItr,m_occluderSet.end());
    
}
