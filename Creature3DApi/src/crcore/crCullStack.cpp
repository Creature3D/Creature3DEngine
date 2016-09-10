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
#include <CRCore/crCullStack.h>
#include <CRCore/crTimer.h>
#include <CRCore/crNotify.h>

using namespace CRCore;
//crCullStack::MatrixList crCullStack::m_reuseMatrixList;
//unsigned int crCullStack::m_currentReuseMatrixIndex = 0;
crCullStack::crCullStack()
{
    m_frustumVolume=-1.0f;
    m_bbCornerNear = 0;
    m_bbCornerFar = 7;
    m_currentReuseMatrixIndex=0;
    m_identity = new RefMatrix();

    m_index_modelviewCullingStack = 0;
    m_back_modelviewCullingStack = 0;
}


crCullStack::~crCullStack()
{
    reset();
}

void crCullStack::reset()
{

    //
    // first unref all referenced objects and then empty the containers.
    //
    m_projectionStack.clear();
    m_modelviewStack.clear();
    m_viewportStack.clear();
    m_eyePointStack.clear();


    m_clipspaceCullingStack.clear();
    m_projectionCullingStack.clear();

    //m_modelviewCullingStack.clear();
    m_index_modelviewCullingStack=0;
    m_back_modelviewCullingStack = 0;

    CRCore::crVector3 lookVector(0.0,0.0,-1.0);
    
    m_bbCornerFar = (lookVector.x()>=0?1:0) |
                   (lookVector.y()>=0?2:0) |
                   (lookVector.z()>=0?4:0);

    m_bbCornerNear = (~m_bbCornerFar)&7;
	m_currentReuseMatrixIndex=0;
}


void crCullStack::pushCullingSet()
{
    m_MVPW_Stack.push_back(0L);

    if (m_index_modelviewCullingStack==0) 
    {
        if (m_modelviewCullingStack.empty())
            m_modelviewCullingStack.push_back(crCullingSet());

        m_modelviewCullingStack[m_index_modelviewCullingStack++].set(m_projectionCullingStack.back());
    }
    else 
    {
        const CRCore::crViewPort& W = *m_viewportStack.back();
        const CRCore::crMatrix& P = *m_projectionStack.back();
        const CRCore::crMatrix& M = *m_modelviewStack.back();
        
		CRCore::crVector4 pixelSizeVector = crCullingSet::computePixelSizeVector(W,P,M);

		if (m_index_modelviewCullingStack>=m_modelviewCullingStack.size()) 
		{
			m_modelviewCullingStack.push_back(crCullingSet());
		}

		m_modelviewCullingStack[m_index_modelviewCullingStack++].set(m_projectionCullingStack.back(),*m_modelviewStack.back(),pixelSizeVector);
        
    }
    
    m_back_modelviewCullingStack = &m_modelviewCullingStack[m_index_modelviewCullingStack-1];

//     const CRCore::Polytope& polytope = m_modelviewCullingStack.back()->getFrustum();
//     const CRCore::Polytope::PlaneList& pl = polytope.getPlaneList();
//     std::cout <<"new cull stack"<<std::endl;
//     for(CRCore::Polytope::PlaneList::const_iterator pl_itr=pl.begin();
//         pl_itr!=pl.end();
//         ++pl_itr)
//     {
//         std::cout << "    plane "<<*pl_itr<<std::endl;
//     }


}

void crCullStack::popCullingSet()
{
    m_MVPW_Stack.pop_back();
    
    --m_index_modelviewCullingStack;
    if (m_index_modelviewCullingStack>0) m_back_modelviewCullingStack = &m_modelviewCullingStack[m_index_modelviewCullingStack-1];

}

void crCullStack::pushViewport(CRCore::crViewPort* viewport)
{
    m_viewportStack.push_back(viewport);
    m_MVPW_Stack.push_back(0L);
}

void crCullStack::popViewport()
{
    m_viewportStack.pop_back();
    m_MVPW_Stack.pop_back();
}

void crCullStack::pushProjectionMatrix(RefMatrix* matrix)
{
    m_projectionStack.push_back(matrix);
    
    m_projectionCullingStack.push_back(crCullingSet());
    crCullingSet& cullingSet = m_projectionCullingStack.back();
    
    // set up view frustum.
    cullingSet.getFrustum().setToUnitFrustum(((m_cullingMode&NEAR_PLANE_CULLING)!=0),((m_cullingMode&FAR_PLANE_CULLING)!=0));
    cullingSet.getFrustum().transformProvidingInverse(*matrix);
    
    // set the culling mask ( There should be a more elegant way!)  Nikolaus H.
    cullingSet.setCullingMask(m_cullingMode);

    // set the small feature culling.
    cullingSet.setSmallFeatureCullingPixelSize(m_smallFeatureCullingPixelSize);
    
    // set up the relevant occluders which a related to this projection.
    for(ShadowVolumeOccluderList::iterator itr=m_occluderList.begin();
        itr!=m_occluderList.end();
        ++itr)
    {
        //std::cout << " ** testing occluder"<<std::endl;
        if (itr->matchProjectionMatrix(*matrix))
        {
            //std::cout << " ** activating occluder"<<std::endl;
            cullingSet.addOccluder(*itr);
        }
    }
    
    

    // need to recompute frustum volume.
    m_frustumVolume = -1.0f;

    pushCullingSet();
}

void crCullStack::popProjectionMatrix()
{

    m_projectionStack.pop_back();

    m_projectionCullingStack.pop_back();

    // need to recompute frustum volume.
    m_frustumVolume = -1.0f;

    popCullingSet();
}

void crCullStack::pushModelViewMatrix(RefMatrix* matrix)
{
    m_modelviewStack.push_back(matrix);
    
    pushCullingSet();
    
#if 1
    CRCore::crVector3f slow_eyepoint = CRCore::crMatrix::inverse(*matrix).getTrans();
    m_eyePointStack.push_back(slow_eyepoint);
#else
    
    // fast method for computing the eye point in local coords which doesn't require the inverse matrix.
    const float x_0 = (*matrix)(0,0);
    const float x_1 = (*matrix)(1,0);
    const float x_2 = (*matrix)(2,0);
    const float x_len2 = (x_0*x_0+x_1*x_1+x_2*x_2);

    const float y_0 = (*matrix)(0,1);
    const float y_1 = (*matrix)(1,1);
    const float y_2 = (*matrix)(2,1);
    const float y_len2 = (y_0*y_0+y_1*y_1+y_2*y_2);
    

    const float z_0 = (*matrix)(0,2);
    const float z_1 = (*matrix)(1,2);
    const float z_2 = (*matrix)(2,2);
    const float z_len2 = (z_0*z_0+z_1*z_1+z_2*z_2);


    bool useFastPath = (CRCore::equivalent(x_len2,y_len2) && 
                        CRCore::equivalent(x_len2,z_len2) &&
                        CRCore::equivalent(y_len2,z_len2));
                        
    // std::cout<<"x_len2 = "<<x_len2 << "\ty_len2 = "<<y_len2 << "\tz_len2 = "<<z_len2 << std::endl;

    if (useFastPath)
    {
        const float xyz_len0 = x_0*x_0 + y_0*y_0 + z_0*z_0;
        const float xyz_len1 = x_1*x_1 + y_1*y_1 + z_1*z_1;
        const float xyz_len2 = x_2*x_2 + y_2*y_2 + z_2*z_2;

        // std::cout<<"xyz_len0 = "<<xyz_len0 << "\txyz_len2 = "<<xyz_len1 << "\txyz_len2 = "<<xyz_len2 << std::endl;

        if (!CRCore::equivalent(xyz_len0,xyz_len1) ||
            !CRCore::equivalent(xyz_len0,xyz_len2) ||
            !CRCore::equivalent(xyz_len1,xyz_len2)) useFastPath = false;

    }                       

    if (useFastPath)
    {
        // compute the eye point in local coords using a fast technique
        // which assumes that only proportional scaling, no shearing, this
        // is satisfied for most scene graph usage.
    
        const float x_scale = (*matrix)(3,0) / -x_len2;
        const float y_scale = (*matrix)(3,1) / -y_len2;
        const float z_scale = (*matrix)(3,2) / -z_len2;

        CRCore::crVector3 fast_eyepoint(x_0*x_scale + y_0*y_scale + z_0*z_scale,
                                x_1*x_scale + y_1*y_scale + z_1*z_scale,
                                x_2*x_scale + y_2*y_scale + z_2*z_scale);

    }
    else
    {
        // shearing or no proportional scaling has been detected so we 
        // callback to compute the inverse of the model view matrix and
        // transforming the eye point into local coords.  This is ten 
        // to thirty times slower than the above fast path.
        CRCore::crVector3 slow_eyepoint(CRCore::crMatrix::inverse(*matrix).getTrans());
        m_eyePointStack.push_back(slow_eyepoint);
        
    }
    
#endif                    
    CRCore::crVector3 lookVector = getLookVectorLocal();                   
    
    m_bbCornerFar = (lookVector.x()>=0?1:0) |
                   (lookVector.y()>=0?2:0) |
                   (lookVector.z()>=0?4:0);

    m_bbCornerNear = (~m_bbCornerFar)&7;
                                       
}

void crCullStack::popModelViewMatrix()
{
    m_modelviewStack.pop_back();
    m_eyePointStack.pop_back();
    popCullingSet();


    CRCore::crVector3 lookVector(0.0f,0.0f,-1.0f);
    if (!m_modelviewStack.empty())
    {
        lookVector = getLookVectorLocal();
    }
    m_bbCornerFar = (lookVector.x()>=0?1:0) |
                   (lookVector.y()>=0?2:0) |
                   (lookVector.z()>=0?4:0);

    m_bbCornerNear = (~m_bbCornerFar)&7;
}

void crCullStack::pushCameraViewInverseMatrix(RefMatrix* matrix)
{
    m_cameraViewInverseStack.push_back(matrix);
}

void crCullStack::popCameraViewInverseMatrix()
{
	m_cameraViewInverseStack.pop_back();
}

void crCullStack::computeFrustumVolume()
{
    CRCore::crMatrix invP;
    invP.invert(getProjectionMatrix());

    CRCore::crVector3 f1(-1,-1,-1); f1 = f1*invP;
    CRCore::crVector3 f2(-1, 1,-1); f2 = f2*invP;
    CRCore::crVector3 f3( 1, 1,-1); f3 = f3*invP;
    CRCore::crVector3 f4( 1,-1,-1); f4 = f4*invP;

    CRCore::crVector3 b1(-1,-1,1); b1 = b1*invP;
    CRCore::crVector3 b2(-1, 1,1); b2 = b2*invP;
    CRCore::crVector3 b3( 1, 1,1); b3 = b3*invP;
    CRCore::crVector3 b4( 1,-1,1); b4 = b4*invP;

    m_frustumVolume = computeVolume(f1,f2,f3,b1,b2,b3)+
                     computeVolume(f2,f3,f4,b1,b3,b4);
        
}

//crCullStack::OCCLUDERFACE crCullStack::getOccluderFace(const crBoundingBox &bb) const
//{
//	const crVector3 &eyeLocal = m_eyePointStack.back();
//
//	if( eyeLocal.y() < bb.yMin() )
//	{
//        if(eyeLocal.x() < bb.xMin())
//            return FRONTLEFT;
//		if(eyeLocal.x() > bb.xMax())
//			return FRONTRIGHT;
//		return FRONT;
//	}
//
//	if( eyeLocal.y() > bb.yMax() )
//	{
//		if(eyeLocal.x() < bb.xMin())
//			return BACKLEFT;
//		if(eyeLocal.x() > bb.xMax())
//			return BACKRIGHT;
//		return BACK;
//	}
//
//	if(eyeLocal.x() < bb.xMin())
//		return LEFT;
//	if(eyeLocal.x() > bb.xMax())
//		return RIGHT;
//	return ALL;
//
//}
