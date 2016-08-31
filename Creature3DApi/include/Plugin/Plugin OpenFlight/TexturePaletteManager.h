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

#ifndef __FLTEXP_TEXTURE_PALETTE_MANAGER_H__
#define __FLTEXP_TEXTURE_PALETTE_MANAGER_H__ 1


#include <Plugin OpenFlight/ExportOptions.h>
#include <fstream>
#include <map>

namespace CRCore {
    class crTexture2D;
}


namespace flt
{


class DataOutputStream;
class FltExportVisitor;


class TexturePaletteManager
{
public:
    TexturePaletteManager( const FltExportVisitor& nv, const ExportOptions& fltOpt );

    int add( int unit, const CRCore::crTexture2D* texture );

    void write( DataOutputStream& dos ) const;

protected:

    TexturePaletteManager& operator = (const TexturePaletteManager&) { return *this; }

    int _currIndex;

    typedef std::map< const CRCore::crTexture2D*, int > TextureIndexMap;
    TextureIndexMap _indexMap;

    const FltExportVisitor& _nv;

    const ExportOptions& _fltOpt;

};



}

#endif
