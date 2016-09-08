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

#include <Plugin OpenFlight/MaterialPaletteManager.h>
#include <Plugin OpenFlight/DataOutputStream.h>
#include <Plugin OpenFlight/Opcodes.h>
#include <CRCore/crNotify.h>
#include <CRCore/crMaterial.h>
#include <CRCore/crVector4.h>
#include <cassert>
#include <sstream>

namespace flt
{


MaterialPaletteManager::MaterialPaletteManager( ExportOptions& fltOpt )
  : _currIndex( -1 ),
    _fltOpt( fltOpt )
{
    // TODO: Pay attention to the version here(?)
}


int MaterialPaletteManager::add(CRCore::crMaterial const* material)
{
    int index = -1;
    if (material == NULL) return -1;


    // If this material has already been cached, set 'index' to the cached value
    MaterialPalette::const_iterator it = _materialPalette.find(material);
    if ( it != _materialPalette.end() )
    {
        index = it->second.Index;
    }

    // New material? Add it to the cache...
    else
    {
        index = ++_currIndex;
        _materialPalette.insert(std::make_pair(material,
                                               MaterialRecord(material, index) ) );
    }

    return index;
}

void
MaterialPaletteManager::write( DataOutputStream& dos ) const
{
    using CRCore::crVector4;

    MaterialPalette::const_iterator it = _materialPalette.begin();
    for ( ; it != _materialPalette.end(); ++it)
    {
        MaterialRecord m = it->second;
        crVector4 const& ambient = m.crMaterial->getAmbient(CRCore::crMaterial::FRONT);
        crVector4 const& diffuse = m.crMaterial->getDiffuse(CRCore::crMaterial::FRONT);
        crVector4 const& specular = m.crMaterial->getSpecular(CRCore::crMaterial::FRONT);
        crVector4 const& emissive = m.crMaterial->getEmission(CRCore::crMaterial::FRONT);
        float shininess = m.crMaterial->getShininess(CRCore::crMaterial::FRONT);

        dos.writeInt16( (int16) MATERIAL_PALETTE_OP );
        dos.writeInt16( 84 );            // Length - FIXME: hard-code/FLT version?
        dos.writeInt32( m.Index );
        dos.writeString( m.crMaterial->getName(), 12 );
        dos.writeInt32( 0 );             // Flags
        dos.writeFloat32(ambient.r() );
        dos.writeFloat32(ambient.g() );
        dos.writeFloat32(ambient.b() );
        dos.writeFloat32(diffuse.r() );
        dos.writeFloat32(diffuse.g() );
        dos.writeFloat32(diffuse.b() );
        dos.writeFloat32(specular.r() );
        dos.writeFloat32(specular.g() );
        dos.writeFloat32(specular.b() );
        dos.writeFloat32(emissive.r() );
        dos.writeFloat32(emissive.g() );
        dos.writeFloat32(emissive.b() );
        dos.writeFloat32(shininess);
        dos.writeFloat32( diffuse.a() ); // alpha
        dos.writeFloat32(1.0f);       // 'Reserved' - unused

        if (m.crMaterial->getAmbientFrontAndBack()   == false   ||
            m.crMaterial->getDiffuseFrontAndBack()   == false   ||
            m.crMaterial->getSpecularFrontAndBack()  == false   ||
            m.crMaterial->getEmissionFrontAndBack()  == false   ||
            m.crMaterial->getShininessFrontAndBack() == false )

        {
            std::string warning( "fltexp: No support for different front and back material properties." );
            CRCore::notify(CRCore::WARN) << warning << std::endl;
           _fltOpt.getWriteResult().warn( warning );
        }

    }
}



}  // End namespace fltexp
