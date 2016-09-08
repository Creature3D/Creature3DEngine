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
#include <CRTerrain/crLocator.h>
#include <CRCore/crNotify.h>

#include <list>

using namespace CRTerrain;

//////////////////////////////////////////////////////////////////////////////
//
// crLocator
//
crLocator::crLocator():
    m_coordinateSystemType(PROJECTED),
    m_ellipsoidModel(new CRCore::crEllipsoidModel()),
    m_definedInFile(false),
    m_transformScaledByResolution(false)
{
}

crLocator::crLocator(const crLocator& locator,const CRCore::crCopyOp& copyop):
    CRCore::crBase(locator,copyop),
    m_coordinateSystemType(locator.m_coordinateSystemType),
    m_format(locator.m_format),
    m_cs(locator.m_cs),
    m_ellipsoidModel(locator.m_ellipsoidModel),
    m_transform(locator.m_transform),
    m_definedInFile(locator.m_definedInFile),
    m_transformScaledByResolution(locator.m_transformScaledByResolution)
{
}

crLocator::~crLocator()
{
}

void crLocator::setTransformAsExtents(double minX, double minY, double maxX, double maxY)
{
    m_transform.set(maxX-minX, 0.0,       0.0, 0.0,
                   0.0,       maxY-minY, 0.0, 0.0,
                   0.0,       0.0,       1.0, 0.0,
                   minX,      minY,      0.0, 1.0); 

    m_inverse.invert(m_transform);
}

bool crLocator::computeLocalBounds(crLocator& source, CRCore::crVector3d& bottomLeft, CRCore::crVector3d& topRight) const
{
    typedef std::list<CRCore::crVector3d> Corners;
    Corners corners;

    CRCore::crVector3d cornerNDC;
    if (crLocator::convertLocalCoordBetween(source, CRCore::crVector3d(0.0,0.0,0.0), *this, cornerNDC))
    {
        corners.push_back(cornerNDC);
    }

    if (crLocator::convertLocalCoordBetween(source, CRCore::crVector3d(1.0,0.0,0.0), *this, cornerNDC))
    {
        corners.push_back(cornerNDC);
    }

    if (crLocator::convertLocalCoordBetween(source, CRCore::crVector3d(0.0,1.0,0.0), *this, cornerNDC))
    {
        corners.push_back(cornerNDC);
    }

    if (crLocator::convertLocalCoordBetween(source, CRCore::crVector3d(1.0,1.0,0.0), *this, cornerNDC))
    {
        corners.push_back(cornerNDC);
    }

    if (corners.empty()) return false;


    for(Corners::iterator itr = corners.begin();
        itr != corners.end();
        ++itr)
    {
        bottomLeft.x() = CRCore::minimum( bottomLeft.x(), itr->x());
        bottomLeft.y() = CRCore::minimum( bottomLeft.y(), itr->y());
        topRight.x() = CRCore::maximum( topRight.x(), itr->x());
        topRight.y() = CRCore::maximum( topRight.y(), itr->y());
    }
    
    return true;
}

bool crLocator::orientationOpenGL() const
{
    return m_transform(0,0) * m_transform(1,1) >= 0.0;
}

bool crLocator::convertLocalToModel(const CRCore::crVector3d& local, CRCore::crVector3d& world) const
{
    switch(m_coordinateSystemType)
    {
        case(GEOCENTRIC):
        {
            CRCore::crVector3d geographic = local * m_transform;
                
            m_ellipsoidModel->convertLatLongHeightToXYZ(geographic.y(), geographic.x(), geographic.z(),
                                                       world.x(), world.y(), world.z());
            return true;      
        }
        case(GEOGRAPHIC):
        {        
            world = local * m_transform;
            return true;      
        }
        case(PROJECTED):
        {        
            world = local * m_transform;
            return true;      
        }
    }    

    return false;
}

bool crLocator::convertModelToLocal(const CRCore::crVector3d& world, CRCore::crVector3d& local) const
{
    switch(m_coordinateSystemType)
    {
        case(GEOCENTRIC):
        {        
            double longitude, latitude, height;

            m_ellipsoidModel->convertXYZToLatLongHeight(world.x(), world.y(), world.z(),
                                                       latitude, longitude, height );

            local = CRCore::crVector3d(longitude, latitude, height) * m_inverse;

            return true;      
        }
        case(GEOGRAPHIC):
        {        
            local = world * m_inverse;

            return true;      
        }
        case(PROJECTED):
        {        
            local = world * m_inverse;
            return true;      
        }
    }    

    return false;
}
