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
#ifndef CRCORE_CRCLUSTERCULLINGCALLBACK
#define CRCORE_CRCLUSTERCULLINGCALLBACK 1

#include <CRCore/crDrawable.h>
#include <CRCore/crNodeCallback.h>

namespace CRCore {

/** Implements cluster culling to cull back facing drawables. Derived from
  * crDrawable::CullCallback.
*/
class CR_EXPORT crClusterCullingCallback : virtual public crDrawable::CullCallback, virtual public crNodeCallback
{
    public:

        crClusterCullingCallback();
        crClusterCullingCallback(const crClusterCullingCallback& ccc,const crCopyOp& copyop);
		crClusterCullingCallback(const crClusterCullingCallback& ccc);
        crClusterCullingCallback(const CRCore::crVector3f& controlPoint, const CRCore::crVector3f& normal, float deviation);
        crClusterCullingCallback(const CRCore::crDrawable* drawable);

        //META_Base(CRCore,crClusterCullingCallback);

        /** Computes the control point, normal, and deviation from the
		  * given drawable contents. */
        void computeFrom(const CRCore::crDrawable* drawable);
        
        /** Transform the crClusterCullingCallback's positional members to a new coordinate frame.*/
        void transform(const CRCore::crMatrixd& matrix);

        void set(const CRCore::crVector3f& controlPoint, const CRCore::crVector3f& normal, float deviation, float radius);

        void setControlPoint(const CRCore::crVector3f& controlPoint) { m_controlPoint = controlPoint; }
        const CRCore::crVector3f& getControlPoint() const { return m_controlPoint; }

        void setNormal(const CRCore::crVector3f& normal) { m_normal = normal; }
        const CRCore::crVector3f& getNormal() const { return m_normal; }

        void setRadius(float radius) { m_radius = radius; }
        float getRadius() const { return m_radius; }

        void setDeviation(float deviation) { m_deviation = deviation; }
        float getDeviation() const { return m_deviation; }

        virtual bool cull(CRCore::crNodeVisitor*, CRCore::crDrawable*, CRCore::crState*) const;

        /** Callback method called by the crNodeVisitor when visiting a node.*/
        virtual void operator()(crNode* node, crNodeVisitor* nv);

    protected:

        virtual ~crClusterCullingCallback() {}

        CRCore::crVector3f    m_controlPoint;
        CRCore::crVector3f    m_normal;
        float        m_radius;
        float        m_deviation;
};


}

#endif
