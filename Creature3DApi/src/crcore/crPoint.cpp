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
#include <CRCore/crPoint.h>
#include <CRCore/crNotify.h>

using namespace CRCore;

crPoint::crPoint()
{
    m_size = 1.0f;                // TODO find proper default
    m_fadeThresholdSize = 1.0f;   // TODO find proper default
                                 // TODO find proper default
    m_distanceAttenuation = crVector3(1, 0.0, 0.0);

    m_minSize = 0.0;
    m_maxSize = 100.0;//depends on mulitsampling ... some default necessary
}


crPoint::~crPoint()
{
}

void crPoint::setSize( float size )
{
    m_size = size;
}


void crPoint::setFadeThresholdSize(float fadeThresholdSize)
{
    m_fadeThresholdSize = fadeThresholdSize;
}


void crPoint::setDistanceAttenuation(const crVector3& distanceAttenuation)
{
    m_distanceAttenuation = distanceAttenuation;
}

void crPoint::setMinSize(float minSize)
{
    m_minSize = minSize;
}

void crPoint::setMaxSize(float maxSize)
{
    m_maxSize = maxSize;
}

#include <Driver/GLDrv/crPointDrv.h>
