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
#ifndef CRCORE_CRCAMERAVIEW
#define CRCORE_CRCAMERAVIEW 1

#include <CRCore/crGroup.h>
#include <CRCore/crTransform.h>
#include <CRCore/crAnimationPath.h>
#include <CRCore/crVector3.h>
#include <CRCore/crQuat.h>

namespace CRCore {

/** crCameraView - is a crTransform that is used to specify camera views from within the scene graph.
  * The application must attach a camera to a crCameraView via the NodePath from the top of the scene graph
  * to the crCameraView node itself, and accimulate the view matrix from this NodePath.
*/
class CR_EXPORT crCameraView : public crTransform
{
    public :
        crCameraView();

        crCameraView(const crCameraView& pat,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crTransform(pat,copyop),
            m_position(pat.m_position),
            m_attitude(pat.m_attitude),
            m_fieldOfView(pat.m_fieldOfView),
            m_fieldOfViewMode(pat.m_fieldOfViewMode),
            m_focalLength(pat.m_focalLength) {}
            

        META_Node(CRCore, crCameraView);

        /** Set the position of the camera view.*/
        inline void setPosition(const crVector3d& pos) { m_position = pos; dirtyBound(); }

        /** Get the position of the camera view.*/
        inline const crVector3d& getPosition() const { return m_position; }

        /** Set the attitide of the camera view.*/
        inline void setAttitude(const crQuat& quat) { m_attitude = quat; dirtyBound(); }

        /** Get the attitide of the camera view.*/
        inline const crQuat& getAttitude() const { return m_attitude; }

        /** Set the field of view.
          * The cameras field of view can be contrained to either the horizontal or vertex axis of the camera, or unconstrained
          * in which case the camera/application are left to choose an appropriate field of view.
          * The default value if 60 degrres. */
        inline void setFieldOfView(double fieldOfView) { m_fieldOfView = fieldOfView; }

        /** Get the field of view.*/
        inline double getFieldOfView() const { return m_fieldOfView; }
        
        enum FieldOfViewMode
        {
            UNCONSTRAINED,
            HORIZONTAL,
            VERTICAL
        };

        /** Set the field of view mode - controlling how the field of view of the camera is contrained by the CameaView settings.*/
        inline void setFieldOfViewMode(FieldOfViewMode mode) { m_fieldOfViewMode = mode; }

        /** Get the field of view mode.*/
        inline FieldOfViewMode getFieldOfViewMode() const { return m_fieldOfViewMode; }

        /** Set the focal length of the camera.
          * A focal length of 0.0 indicates that the camera/application should determine the focal length.
          * The default value is 0.0. */
        inline void setFocalLength(double focalLength) { m_focalLength = focalLength; }

        /** Get the focal length of the camera.*/
        inline double getFocalLength() const { return m_focalLength; }
        

        virtual bool computeLocalToWorldMatrix(crMatrix& matrix,crNodeVisitor* nv) const;
        virtual bool computeWorldToLocalMatrix(crMatrix& matrix,crNodeVisitor* nv) const;


    protected :
            
        virtual ~crCameraView() {}

        crVector3d           m_position;
        crQuat            m_attitude;
        double          m_fieldOfView;
        FieldOfViewMode m_fieldOfViewMode;
        double          m_focalLength;
};

}

#endif
