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
#include <CRCore/crClusterCullingCallback.h>
#include <CRCore/crTriangleFunctor.h>
#include <CRCore/crCullSettings.h>

using namespace CRCore;


///////////////////////////////////////////////////////////////////////////////////////////
//
//  Cluster culling callback
//

crClusterCullingCallback::crClusterCullingCallback():
    m_radius(-1.0f),
    m_deviation(-1.0f)
{
}

crClusterCullingCallback::crClusterCullingCallback(const crClusterCullingCallback& ccc,const crCopyOp& copyop):
    crDrawable::CullCallback(ccc,copyop),
    m_controlPoint(ccc.m_controlPoint),m_normal(ccc.m_normal),m_deviation(ccc.m_deviation)
{
}
crClusterCullingCallback::crClusterCullingCallback(const crClusterCullingCallback& ccc):
    crNodeCallback(ccc),
    m_controlPoint(ccc.m_controlPoint),m_normal(ccc.m_normal),m_deviation(ccc.m_deviation)
{

}

crClusterCullingCallback::crClusterCullingCallback(const CRCore::crVector3f& controlPoint, const CRCore::crVector3f& normal, float deviation):
    m_controlPoint(controlPoint),m_normal(normal), m_deviation(deviation)
{
}

crClusterCullingCallback::crClusterCullingCallback(const CRCore::crDrawable* drawable)
{
    computeFrom(drawable);
}

struct ComputeAveragesFunctor
{

    ComputeAveragesFunctor():
        m_num(0) {}

    inline void operator() ( const CRCore::crVector3f &v1, const CRCore::crVector3f &v2, const CRCore::crVector3f &v3, bool)
    {
        CRCore::crVector3d normal = (v2-v1)^(v3-v1);
        if (normal.length()!=0.0f)
        {
            m_normal += normal.normalize();
        }        
        m_center += v1;
        m_center += v2;
        m_center += v3;
                
        ++m_num;

    }
    
    CRCore::crVector3f center() { return m_center /  (double)(3*m_num); }
    CRCore::crVector3f normal() { m_normal.normalize(); return m_normal; }
    
    unsigned int m_num;
    crVector3d m_center;
    crVector3d m_normal;
};

struct ComputeDeviationFunctor
{

    ComputeDeviationFunctor():
        m_deviation(1.0),
        m_radius2(0.0) {}
        
    void set(const CRCore::crVector3f& center,const CRCore::crVector3f& normal)
    {
        m_center = center;
        m_normal = normal;
    }

    inline void operator() ( const CRCore::crVector3f &v1, const CRCore::crVector3f &v2, const CRCore::crVector3f &v3, bool)
    {
        CRCore::crVector3f normal = (v2-v1)^(v3-v1);
        if (normal.length()!=0.0f)
        {
            m_deviation = CRCore::minimum(m_normal*normal.normalize(),m_deviation);
        }
        m_radius2 = CRCore::maximum((v1-m_center).length2(),m_radius2);
        m_radius2 = CRCore::maximum((v2-m_center).length2(),m_radius2);
        m_radius2 = CRCore::maximum((v3-m_center).length2(),m_radius2);

    }
    CRCore::crVector3f m_center;
    CRCore::crVector3f m_normal;
    float m_deviation;
    float m_radius2;
};


void crClusterCullingCallback::computeFrom(const CRCore::crDrawable* drawable)
{
    crTriangleFunctor<ComputeAveragesFunctor> caf;
    drawable->accept(caf);
    
    m_controlPoint = caf.center();
    m_normal = caf.normal();
    
    crTriangleFunctor<ComputeDeviationFunctor> cdf;
    cdf.set(m_controlPoint,m_normal);
    drawable->accept(cdf);
    
//    CRCore::notify(CRCore::NOTICE)<<"crClusterCullingCallback::computeFrom() m_controlPoint="<<m_controlPoint<<std::endl;
//    CRCore::notify(CRCore::NOTICE)<<"                                      m_normal="<<m_normal<<std::endl;
//    CRCore::notify(CRCore::NOTICE)<<"                                      cdf.m_deviation="<<cdf.m_deviation<<std::endl;
    

    if (m_normal.length2()==0.0) m_deviation = -1.0f;
    else 
    {
        float angle = acosf(cdf.m_deviation)+PI*0.5f;
        if (angle<PI) m_deviation = cosf(angle);
        else m_deviation = -1.0f;
    }
    
    m_radius = sqrtf(cdf.m_radius2);
}

void crClusterCullingCallback::set(const CRCore::crVector3f& controlPoint, const CRCore::crVector3f& normal, float deviation, float radius)
{
    m_controlPoint = controlPoint;
    m_normal = normal;
    m_deviation = deviation;
    m_radius = radius;
}

void crClusterCullingCallback::transform(const CRCore::crMatrixd& matrix)
{
    m_controlPoint = crVector3d(m_controlPoint)*matrix;
    m_normal = crMatrixd::transform3x3(crMatrixd::inverse(matrix),crVector3d(m_normal));
    m_normal.normalize();
}


bool crClusterCullingCallback::cull(CRCore::crNodeVisitor* nv, CRCore::crDrawable* , CRCore::crState*) const
{
    crCullSettings* cs = dynamic_cast<crCullSettings*>(nv);
    if (cs && !(cs->getCullingMode() & crCullSettings::CLUSTER_CULLING))
    {
        return false;
    }

    if (m_deviation<=-1.0f)
    {
        return false;
    }
    
    CRCore::crVector3f eye_cp = nv->getEyePoint() - m_controlPoint;
    float radius = eye_cp.length();
    
    if (radius<m_radius)
    {
        return false;
    }
    
    
    float deviation = (eye_cp * m_normal)/radius;

//    CRCore::notify(CRCore::NOTICE)<<"crClusterCullingCallback::cull() m_normal="<<m_normal<<" _controlPointtest="<<m_controlPoint<<" eye_cp="<<eye_cp<<std::endl;
//    CRCore::notify(CRCore::NOTICE)<<"                             deviation="<<deviation<<" m_deviation="<<m_deviation<<" test="<<(deviation < m_deviation)<<std::endl;

    return deviation < m_deviation;
}


void crClusterCullingCallback::operator()(crNode* node, crNodeVisitor* nv)
{
    if (nv)
    {
        if (cull(nv,0,0)) return;
        
        traverse(node,nv); 
    }
}
