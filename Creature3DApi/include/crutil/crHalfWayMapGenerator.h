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
#ifndef CRUTIL_HALFWAYMAPGENERATOR_H
#define CRUTIL_HALFWAYMAPGENERATOR_H

#include <CRUtil/crExport.h>

#include <CRUtil/crCubeMapGenerator.h>

namespace CRUtil
{

	/** This cube map generator produces an Half-way vector map, useful for 
	    hardware-based specular lighting effects.
	    It computes: C = normalize(R - L), where C is the resulting color, 
	    R is the reflection vector and L is the light direction.
	*/
	class CRUTIL_EXPORT crHalfWayMapGenerator: public crCubeMapGenerator 
	{
	public:
		crHalfWayMapGenerator(const CRCore::crVector3 &light_direction, int texture_size = 64);
		crHalfWayMapGenerator(const crHalfWayMapGenerator &copy, const CRCore::crCopyOp &copyop);

	protected:
		virtual ~crHalfWayMapGenerator() {}
		crHalfWayMapGenerator &operator=(const crHalfWayMapGenerator &) { return *this; }

		inline virtual CRCore::crVector4 compute_color(const CRCore::crVector3 &R) const;

	private:
		CRCore::crVector3 m_ldir;
	};

	// INLINE METHODS

	inline CRCore::crVector4 crHalfWayMapGenerator::compute_color(const CRCore::crVector3 &R) const
	{
		const CRCore::crVector3 V = (R / R.length()) - m_ldir;
		return vector_to_color(V / V.length());
	}

}

#endif
