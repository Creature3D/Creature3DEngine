/*
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or (at
 * your option) any later version. The full license is in the LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * OpenSceneGraph Public License for more details.
*/

//
// Copyright(c) 2008 Skew crMatrix Software LLC.
//

#include <Plugin OpenFlight/LightSourcePaletteManager.h>
#include <Plugin OpenFlight/DataOutputStream.h>
#include <Plugin OpenFlight/Opcodes.h>
#include <CRCore/crNotify.h>
#include <CRCore/crLight.h>
#include <CRCore/crVector4.h>
#include <cassert>
#include <sstream>
#include <stdio.h>

namespace flt
{


LightSourcePaletteManager::LightSourcePaletteManager( ExportOptions& fltOpt )
  : _currIndex( -1 ),
    _fltOpt( fltOpt )

{
    // TODO: Pay attention to the version here(?)
}


int LightSourcePaletteManager::add(CRCore::crLight const* light)
{
    int index = -1;
    if (light == NULL) return -1;


    // If this light has already been cached, set 'index' to the cached value
    LightPalette::const_iterator it = _lightPalette.find(light);
    if ( it != _lightPalette.end() )
    {
        index = it->second.Index;
    }

    // New light? Add it to the cache...
    else
    {
        index = ++_currIndex;
        _lightPalette.insert(std::make_pair(light,
                                               LightRecord(light, index) ) );
    }

    return index;
}

void
LightSourcePaletteManager::write( DataOutputStream& dos ) const
{
    using CRCore::crVector4;

    static int const INFINITE_LIGHT = 0;
    static int const LOCAL_LIGHT    = 1;
    static int const SPOT_LIGHT     = 2;

    LightPalette::const_iterator it = _lightPalette.begin();
    for ( ; it != _lightPalette.end(); ++it)
    {
        LightRecord m = it->second;

        static char lightName[64];
        sprintf(lightName, "crLight%02d\0", m.crLight->getLightNum() );

        int lightType = INFINITE_LIGHT;
        crVector4 const& lightPos = m.crLight->getPosition();
        if (lightPos.w() != 0)
        {
            if (m.crLight->getSpotCutoff() < 180)
                lightType = SPOT_LIGHT;
            else
                lightType = LOCAL_LIGHT;
        }

        dos.writeInt16( (int16) LIGHT_SOURCE_PALETTE_OP );
        dos.writeInt16( 240 );
        dos.writeInt32( m.Index );
        dos.writeFill(2*4, '\0');                     // Reserved
        dos.writeString( lightName, 20 );
        dos.writeFill(4, '\0');                       // Reserved

        dos.writeVec4f(m.crLight->getAmbient() );
        dos.writeVec4f(m.crLight->getDiffuse() );
        dos.writeVec4f(m.crLight->getSpecular() );
        dos.writeInt32(lightType);
        dos.writeFill(4*10, '\0');                     // Reserved
        dos.writeFloat32(m.crLight->getSpotExponent() );
        dos.writeFloat32(m.crLight->getSpotCutoff() );
        dos.writeFloat32(0);                           // Yaw (N/A)
        dos.writeFloat32(0);                           // Pitch (N/A)
        dos.writeFloat32(m.crLight->getConstantAttenuation() );
        dos.writeFloat32(m.crLight->getLinearAttenuation() );
        dos.writeFloat32(m.crLight->getQuadraticAttenuation() );
        dos.writeInt32(0);                             // Modeling flag (N/A)
        dos.writeFill(4*19, '\0');                     // Reserved

    }
}



}  // End namespace fltexp
