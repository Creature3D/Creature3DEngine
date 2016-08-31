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

#ifndef __FLTEXP_LIGHT_SOURCE_PALETTE_MANAGER_H__
#define __FLTEXP_LIGHT_SOURCE_PALETTE_MANAGER_H__ 1

#include <Plugin OpenFlight/ExportOptions.h>

#include <set>

namespace CRCore {
    class crLight;
}


namespace flt
{


class DataOutputStream;


class LightSourcePaletteManager
{
  public:
      LightSourcePaletteManager( ExportOptions& fltOpt );

    // Add a light to the palette and auto-assign it the next available index
    int add(CRCore::crLight const* light);

    // Write the light palette records out to a DataOutputStream
    void write( DataOutputStream& dos ) const;


  private:
    int _currIndex;

    // Helper struct to hold light palette records
    struct LightRecord
    {
        LightRecord(CRCore::crLight const* light, int i)
            : crLight(light)
            , Index(i) { }

        CRCore::crLight const* crLight;
        int Index;

    };

    // Map to allow lookups by crLight pointer
    typedef std::map<CRCore::crLight const*, LightRecord> LightPalette;
    LightPalette _lightPalette;


    ExportOptions& _fltOpt;

protected:

    LightSourcePaletteManager& operator = (const LightSourcePaletteManager&) { return *this; }
};


}  // End namespace fltexp

#endif  // !__FLTEXP_LIGHT_SOURCE_PALETTE_MANAGER_H__
