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
#ifndef CRCORE_COORDINATESYSTEMNODE
#define CRCORE_COORDINATESYSTEMNODE 1

#include <CRCore/crGroup.h>
#include <CRCore/crMatrixd.h>

namespace CRCore
{

const double WGS_84_RADIUS_EQUATOR = 6378137.0;
const double WGS_84_RADIUS_POLAR = 6356752.3142;

/** crEllipsoidModel encapsulates the ellipsoid used to model astral bodies such as plants, moon etc.*/
class crEllipsoidModel : public crBase
{
    public:


        crEllipsoidModel(double radiusEquator = WGS_84_RADIUS_EQUATOR,
                           double radiusPolar = WGS_84_RADIUS_POLAR):
            m_radiusEquator(radiusEquator),
            m_radiusPolar(radiusPolar) { computeCoefficients(); }

        crEllipsoidModel(const crEllipsoidModel& et,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crBase(et,copyop),
            m_radiusEquator(et.m_radiusEquator),
            m_radiusPolar(et.m_radiusPolar) { computeCoefficients(); }

        META_Base(CRCore,crEllipsoidModel);

        void setRadiusEquator(double radius) { m_radiusEquator = radius; computeCoefficients(); }
        double getRadiusEquator() const { return m_radiusEquator; }

        void setRadiusPolar(double radius) { m_radiusPolar = radius; computeCoefficients(); }
        double getRadiusPolar() const { return m_radiusPolar; }

        inline void convertLatLongHeightToXYZ(double latitude, double longitude, double height,
                                              double& X, double& Y, double& Z) const;

        inline void convertXYZToLatLongHeight(double X, double Y, double Z,
                                              double& latitude, double& longitude, double& height) const;

        inline void computeLocalToWorldTransformFromLatLongHeight(double latitude, double longitude, double height, CRCore::crMatrixd& localToWorld) const;

        inline void computeLocalToWorldTransformFromXYZ(double X, double Y, double Z, CRCore::crMatrixd& localToWorld) const;

        inline CRCore::crVector3d computeLocalUpVector(double X, double Y, double Z) const;

    protected:

        void computeCoefficients()
        {
            double flattening = (m_radiusEquator-m_radiusPolar)/m_radiusEquator;
            m_eccentricitySquared = 2*flattening - flattening*flattening;
        }

        double m_radiusEquator;
        double m_radiusPolar;
        double m_eccentricitySquared;

};

/** CoordinateFrame encapsulate the orientiation of east, north and up.*/ 
typedef crMatrixd CoordinateFrame;

/** CoordinateSystem encapsulate the coordinate system that associated with objects in a scene.
    For an overview of common earth bases coordinate systems see http://www.colorado.edu/geography/gcraft/notes/coordsys/coordsys_f.html */
class CR_EXPORT crCoordinateSystemNode : public crGroup
{
    public:

        crCoordinateSystemNode();

        crCoordinateSystemNode(const std::string& format, const std::string& cs);

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crCoordinateSystemNode(const crCoordinateSystemNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
        
        META_Node(CRCore,crCoordinateSystemNode);
        
        
        /** Set the coordinate system node up by copy the format, coordinate system string, and ellipsoid model of another coordinate system node.*/
        void set(const crCoordinateSystemNode& csn);
                
        /** Set the coordinate system format string. Typical values would be WKT, PROJ4, USGS etc.*/
        void setFormat(const std::string& format) { m_format = format; }
        
        /** Get the coordinate system format string.*/
        const std::string& getFormat() const { return m_format; }

        /** Set the CoordinateSystem reference string, should be stored form consistent with the Format.*/
        void setCoordinateSystem(const std::string& cs) { m_cs = cs; }
        
        /** Get the CoordinateSystem reference string.*/
        const std::string& getCoordinateSystem() const { return m_cs; }
        
        
        /** set crEllipsoidModel to describe the model used to map lat, long and height into geocentric XYZ and back. */
        void setEllipsoidModel(crEllipsoidModel* ellipsode) { m_ellipsoidModel = ellipsode; }
        
        /** get the crEllipsoidModel.*/
        crEllipsoidModel* getEllipsoidModel() { return m_ellipsoidModel.get(); }
        
        /** get the const crEllipsoidModel.*/
        const crEllipsoidModel* getEllipsoidModel() const { return m_ellipsoidModel.get(); }
        
        /** compute the local coorindate frame for specified point.*/
        CoordinateFrame computeLocalCoordinateFrame(const crVector3d& position) const;
        
        /** compute the local coorindate frame for specified point.*/
        CRCore::crVector3d computeLocalUpVector(const crVector3d& position) const;

    protected:

        virtual ~crCoordinateSystemNode() {}
                
        std::string             m_format;
        std::string             m_cs;
        ref_ptr<crEllipsoidModel> m_ellipsoidModel;

};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// implement inline methods.
//
inline void crEllipsoidModel::convertLatLongHeightToXYZ(double latitude, double longitude, double height,
                                      double& X, double& Y, double& Z) const
{
    // for details on maths see http://www.colorado.edu/geography/gcraft/notes/datum/gif/llhxyz.gif
    double sin_latitude = sin(latitude);
    double cos_latitude = cos(latitude);
    double N = m_radiusEquator / sqrt( 1.0 - m_eccentricitySquared*sin_latitude*sin_latitude);
    X = (N+height)*cos_latitude*cos(longitude);
    Y = (N+height)*cos_latitude*sin(longitude);
    Z = (N*(1-m_eccentricitySquared)+height)*sin_latitude;
}


inline void crEllipsoidModel::convertXYZToLatLongHeight(double X, double Y, double Z,
                                      double& latitude, double& longitude, double& height) const
{
    // http://www.colorado.edu/geography/gcraft/notes/datum/gif/xyzllh.gif
    double p = sqrt(X*X + Y*Y);
    double theta = atan(Z*m_radiusEquator/ (p*m_radiusPolar));
    double eDashSquared = (m_radiusEquator*m_radiusEquator - m_radiusPolar*m_radiusPolar)/
                          (m_radiusPolar*m_radiusPolar);

    double sin_theta = sin(theta);
    double cos_theta = cos(theta);

    latitude = atan( (Z + eDashSquared*m_radiusPolar*sin_theta*sin_theta*sin_theta) /
                     (p - m_eccentricitySquared*m_radiusEquator*cos_theta*cos_theta*cos_theta) );
    longitude = atan2(Y,X);

    double sin_latitude = sin(latitude);
    double N = m_radiusEquator / sqrt( 1.0 - m_eccentricitySquared*sin_latitude*sin_latitude);

    height = p/cos(latitude) - N;
}

inline void crEllipsoidModel::computeLocalToWorldTransformFromLatLongHeight(double latitude, double longitude, double height, CRCore::crMatrixd& localToWorld) const
{
    double X, Y, Z;
    convertLatLongHeightToXYZ(latitude,longitude,height,X,Y,Z);
    computeLocalToWorldTransformFromXYZ(X,Y,Z,localToWorld);
}

inline void crEllipsoidModel::computeLocalToWorldTransformFromXYZ(double X, double Y, double Z, CRCore::crMatrixd& localToWorld) const
{
    localToWorld.makeTranslate(X,Y,Z);


    // normalize X,Y,Z
    double inverse_length = 1.0/sqrt(X*X + Y*Y + Z*Z);
    
    X *= inverse_length;
    Y *= inverse_length;
    Z *= inverse_length;

    double length_XY = sqrt(X*X + Y*Y);
    double inverse_length_XY = 1.0/length_XY;

    // Vx = |(-Y,X,0)|
    localToWorld(0,0) = -Y*inverse_length_XY;
    localToWorld(0,1) = X*inverse_length_XY;
    localToWorld(0,2) = 0.0;

    // Vy = /(-Z*X/(sqrt(X*X+Y*Y), -Z*Y/(sqrt(X*X+Y*Y),sqrt(X*X+Y*Y))| 
    double Vy_x = -Z*X*inverse_length_XY;
    double Vy_y = -Z*Y*inverse_length_XY;
    double Vy_z = length_XY;
    inverse_length = 1.0/sqrt(Vy_x*Vy_x + Vy_y*Vy_y + Vy_z*Vy_z);            
    localToWorld(1,0) = Vy_x*inverse_length;
    localToWorld(1,1) = Vy_y*inverse_length;
    localToWorld(1,2) = Vy_z*inverse_length;

    // Vz = (X,Y,Z)
    localToWorld(2,0) = X;
    localToWorld(2,1) = Y;
    localToWorld(2,2) = Z;
}

inline CRCore::crVector3d crEllipsoidModel::computeLocalUpVector(double X, double Y, double Z) const
{
    CRCore::crVector3 normal(X,Y,Z);
    normal.normalize();
    return normal;
}

}
#endif
