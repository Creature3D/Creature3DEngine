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

#ifndef CRCORE_CRAUTOTRANSFORM_H
#define CRCORE_CRAUTOTRANSFORM_H 1

#include <CRCore/crGroup.h>
#include <CRCore/crTransform.h>
#include <CRCore/crQuat.h>

namespace CRCore {

class CR_EXPORT crAutoTransform : public crTransform
{
    public :
        crAutoTransform();

        crAutoTransform(const crAutoTransform& pat,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);            

        virtual CRCore::crBase* cloneType() const { return new crAutoTransform (); }
        virtual CRCore::crBase* clone(const CRCore::crCopyOp& copyop) const { return new crAutoTransform (*this,copyop); }
        virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crAutoTransform *>(obj)!=NULL; }
        virtual const char* className() const { return "crAutoTransform"; }
        virtual const char* libraryName() const { return "CRCore"; }

        virtual void accept(crNodeVisitor& nv);

        virtual crAutoTransform* asAutoTransform() { return this; }
        virtual const crAutoTransform* asAutoTransform() const { return this; }

        inline void setPosition(const crVector3& pos) { m_position = pos; m_matrixDirty=true; dirtyBound(); }
        inline const crVector3& getPosition() const { return m_position; }


        inline void setRotation(const crQuat& quat) { m_rotation = quat; m_matrixDirty=true; dirtyBound(); }
        inline const crQuat& getRotation() const { return m_rotation; }
        
        inline void setScale(float scale) { m_scale.set(scale,scale,scale); m_matrixDirty=true; dirtyBound(); }
        inline void setScale(const crVector3& scale) { m_scale = scale; dirtyBound(); }
        inline const crVector3& getScale() const { return m_scale; }
        
        inline void setPivotPoint(const crVector3& pivot) { m_pivotPoint = pivot; m_matrixDirty=true; dirtyBound(); }
        inline const crVector3& getPivotPoint() const { return m_pivotPoint; }
        

        void setAutoUpdateEyeMovementTolerance(float tolerance) { m_autoUpdateEyeMovementTolerance = tolerance; }
        float getAutoUpdateEyeMovementTolerance() const { return m_autoUpdateEyeMovementTolerance; }




        enum AutoRotateMode
        {
            NO_ROTATION,
            ROTATE_TO_SCREEN,
            ROTATE_TO_CAMERA
        };
        
        void setAutoRotateMode(AutoRotateMode mode) { m_autoRotateMode = mode; }

        AutoRotateMode getAutoRotateMode() const { return m_autoRotateMode; }

#ifdef USE_DEPRECATED_API
        void setAutoRotateToScreen(bool autoRotateToScreen)
        {
            setAutoRotateMode(autoRotateToScreen?ROTATE_TO_SCREEN:NO_ROTATION);
		}

        bool getAutoRotateToScreen() const { return m_autoRotateMode==ROTATE_TO_SCREEN; }

        void setAutoRotateToCamera(bool autoRotateToCamera)
		{
            setAutoRotateMode(autoRotateToCamera?ROTATE_TO_CAMERA:NO_ROTATION);
		}
        
        bool getAutoRotateToCamera() const { return m_autoRotateMode==ROTATE_TO_CAMERA; }
#endif



        void setAutoScaleToScreen(bool autoScaleToScreen) { m_autoScaleToScreen = autoScaleToScreen; m_matrixDirty=true; }

        bool getAutoScaleToScreen() const { return m_autoScaleToScreen; }


        virtual bool computeLocalToWorldMatrix(crMatrix& matrix,crNodeVisitor* nv) const;

        virtual bool computeWorldToLocalMatrix(crMatrix& matrix,crNodeVisitor* nv) const;



    protected :
            
        virtual ~crAutoTransform() {}
        

        crVector3                m_position;
        crVector3                m_pivotPoint;
        float               m_autoUpdateEyeMovementTolerance;

        AutoRotateMode      m_autoRotateMode;

        bool                m_autoScaleToScreen;
        
        mutable crQuat        m_rotation;
        mutable crVector3        m_scale;
        mutable bool        m_firstTimeToInitEyePoint;
        mutable CRCore::crVector3   m_previousEyePoint;
        mutable CRCore::crVector3   m_previousLocalUp;
        mutable int         m_previousWidth;
        mutable int         m_previousHeight;        
        mutable CRCore::crMatrix m_previousProjection;
        mutable CRCore::crVector3 m_previousPosition;


        void computeMatrix() const;

        mutable bool        m_matrixDirty;
        mutable CRCore::crMatrix m_cachedMatrix;
};

}

#endif
