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
//Modified by 吴财华
#ifndef CRCORE_CRPOSITIONATTITIDETRANSFORM_H
#define CRCORE_CRPOSITIONATTITIDETRANSFORM_H 1

#include <CRCore/crGroup.h>
#include <CRCore/crTransform.h>
#include <CRCore/crAnimationPath.h>
#include <CRCore/crQuat.h>

namespace CRCore {

/** PositionAttitideTransform - is crTransform the set the coordinates crTransform
    up via a crVector3d position and crQuat attitude.
*/
class CR_EXPORT crPositionAttitudeTransform : public crTransform
{
    public :
        crPositionAttitudeTransform();

        crPositionAttitudeTransform(const crPositionAttitudeTransform& pat,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crTransform(pat,copyop),
            m_position(pat.m_position),
            m_attitude(pat.m_attitude),
			m_scale(pat.m_scale),
            m_pivotPoint(pat.m_pivotPoint) {}
            

        META_Node(CRCore, crPositionAttitudeTransform);

        virtual crPositionAttitudeTransform* asPositionAttitudeTransform() { return this; }
        virtual const crPositionAttitudeTransform* asPositionAttitudeTransform() const { return this; }

        inline void setPosition(const crVector3d& pos) { m_position = pos; dirtyBound(); }
        
        inline const crVector3d& getPosition() const { return m_position; }


        inline void setAttitude(const crQuat& crQuat) { m_attitude = crQuat; dirtyBound(); }
        
        inline const crQuat& getAttitude() const { return m_attitude; }
        
		inline void setScale(const crVector3d& scale) { m_scale = scale; dirtyBound(); }
		inline const crVector3d& getScale() const { return m_scale; }

        inline void setPivotPoint(const crVector3d& pivot) { m_pivotPoint = pivot; dirtyBound(); }
    
        inline const crVector3d& getPivotPoint() const { return m_pivotPoint; }
        

        virtual bool computeLocalToWorldMatrix(crMatrix& matrix, crNodeVisitor* nv) const;

        virtual bool computeWorldToLocalMatrix(crMatrix& matrix, crNodeVisitor* nv) const;



    protected :
            
        virtual ~crPositionAttitudeTransform() {}

        crVector3d m_position;
        crQuat    m_attitude;
		crVector3d m_scale;
        crVector3d m_pivotPoint;
};

}

#endif
