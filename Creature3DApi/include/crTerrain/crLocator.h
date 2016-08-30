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
#ifndef CRTERRAIN_CRLOCATOR_H
#define CRTERRAIN_CRLOCATOR_H 1

#include <CRCore/crBase.h>
#include <CRCore/crVector3.h>
#include <CRCore/crCoordinateSystemNode.h>

#include <CRTerrain/crExport.h>

namespace CRTerrain {

class CRTERRAIN_EXPORT crLocator : public CRCore::crBase
{
    public:

        crLocator();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crLocator(const crLocator&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
        
        META_Base(CRTerrain, crLocator);

        /** CoordinateSystemType provides the classification of the type coordinate system represented.*/
        enum CoordinateSystemType
        {
            /** GEOCENTRIC coordinate systems are ones mapped to the around the ellipsoid, i.e. whole earth.*/
            GEOCENTRIC,

            /** GEOGRAPHIC coordinate systems are ones mapped to latitude and longitude.*/
            GEOGRAPHIC,

            /** PROJECTED coordinate systems are ones projected to a local projected coordinate system i.e. UTMs.*/
            PROJECTED
        };

        /** Set the CoordinatesSyetemType.
          * Note, the user must keep the CoordinateSystemString consistent with the type of the CoordinateSystem.*/
        void setCoordinateSystemType(CoordinateSystemType type) { m_coordinateSystemType = type; }

        /** Get the CoordinatesSyetemType.*/
        CoordinateSystemType getCoordinateSystemType() const { return m_coordinateSystemType; }

        /** Set the coordinate system format string. Typical values would be WKT, PROJ4, USGS etc.*/
        void setFormat(const std::string& format) { m_format = format; }
        
        /** Get the coordinate system format string.*/
        const std::string& getFormat() const { return m_format; }

        /** Set the CoordinateSystem reference string, should be stored in a form consistent with the Format.*/
        void setCoordinateSystem(const std::string& cs) { m_cs = cs; }
        
        /** Get the CoordinateSystem reference string.*/
        const std::string& getCoordinateSystem() const { return m_cs; }


        /** Set crEllipsoidModel to describe the model used to map lat, long and height into geocentric XYZ and back. */
        void setEllipsoidModel(CRCore::crEllipsoidModel* ellipsode) { m_ellipsoidModel = ellipsode; }
        
        /** Get the crEllipsoidModel.*/
        CRCore::crEllipsoidModel* getEllipsoidModel() { return m_ellipsoidModel.get(); }
        
        /** Get the const crEllipsoidModel.*/
        const CRCore::crEllipsoidModel* getEllipsoidModel() const { return m_ellipsoidModel.get(); }


        /** Set the transformation from local coordinates to model coordinates.*/
        void setTransform(const CRCore::crMatrixd& transform) { m_transform = transform; m_inverse.invert(m_transform); }

        /** Set the transformation from local coordinates to model coordinates.*/
        const CRCore::crMatrixd& getTransform() const { return m_transform; }

        /** Set the extents of the local coords.*/
        void setTransformAsExtents(double minX, double minY, double maxX, double maxY);

        
        virtual bool orientationOpenGL() const;

        virtual bool convertLocalToModel(const CRCore::crVector3d& /*local*/, CRCore::crVector3d& /*world*/) const;

        virtual bool convertModelToLocal(const CRCore::crVector3d& /*world*/, CRCore::crVector3d& /*local*/) const;

        static bool convertLocalCoordBetween(const crLocator& source, const CRCore::crVector3d& sourceNDC, 
                                             const crLocator& destination, CRCore::crVector3d& destinationNDC)
        {
            CRCore::crVector3d model;
            if (!source.convertLocalToModel(sourceNDC, model)) return false;
            if (!destination.convertModelToLocal(model, destinationNDC)) return false;
            return true;
        }
        
        bool computeLocalBounds(crLocator& source, CRCore::crVector3d& bottomLeft, CRCore::crVector3d& topRight) const;
        
        void setDefinedInFile(bool flag) { m_definedInFile = flag; }
        bool getDefinedInFile() const { return m_definedInFile; }
        
        void setTransformScaledByResolution(bool scaledByResolution) { m_transformScaledByResolution = scaledByResolution; }
        bool getTransformScaledByResolution() const { return m_transformScaledByResolution; }

    protected:

        virtual ~crLocator();
        
        CoordinateSystemType                m_coordinateSystemType;

        std::string                         m_format;
        std::string                         m_cs;
        CRCore::ref_ptr<CRCore::crEllipsoidModel>   m_ellipsoidModel;

        CRCore::crMatrixd                        m_transform;
        CRCore::crMatrixd                        m_inverse;
        
        bool                                m_definedInFile;
        bool                                m_transformScaledByResolution;

};

}

#endif
