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
#ifndef CRUTIL_HIGHLIGHTMAPGENERATOR_
#define CRUTIL_HIGHLIGHTMAPGENERATOR_

#include <CRUtil/crExport.h>

#include <CRUtil/crCubeMapGenerator.h>

namespace CRUtil
{

    /** This cube map generator produces a specular highlight map. 
        The vector-color association is: C = (R dot (-L)) ^ n, where C is the 
        resulting color, R is the reflection vector, L is the light direction 
        and n is the specular exponent.
    */
    class CRUTIL_EXPORT crHighlightMapGenerator: public crCubeMapGenerator 
	{
    public:
        crHighlightMapGenerator(
            const CRCore::crVector3 &light_direction, 
            const CRCore::crVector4 &light_color, 
            float specular_exponent, 
            int texture_size = 64);

        crHighlightMapGenerator(const crHighlightMapGenerator &copy, const CRCore::crCopyOp &copyop = CRCore::crCopyOp::SHALLOW_COPY);

    protected:
        virtual ~crHighlightMapGenerator() {}
        crHighlightMapGenerator &operator=(const crHighlightMapGenerator &) { return *this; }

        inline virtual CRCore::crVector4 compute_color(const CRCore::crVector3 &R) const;

    private:
        CRCore::crVector3 m_ldir;
        CRCore::crVector4 m_lcol;
        float m_sexp;
    };

    // INLINE METHODS

    inline CRCore::crVector4 crHighlightMapGenerator::compute_color(const CRCore::crVector3 &R) const
    {
        float v = -m_ldir * (R / R.length());
        if (v < 0) v = 0;
        CRCore::crVector4 color(m_lcol * powf(v, m_sexp));
        color.w() = 1;
        return color;
    }

}

#endif
