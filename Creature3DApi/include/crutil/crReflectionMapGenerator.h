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
//Modified by Îâ²Æ»ª
#ifndef CRUTIL_REFLECTIONMAPGENERATOR_H
#define CRUTIL_REFLECTIONMAPGENERATOR_H

#include <CRUtil/crCubeMapGenerator.h>

namespace CRUtil
{

	/** This is the most simple cube map generator. It performs a direct association 
	    between reflection vector and RGBA color (C = R).
	*/
	class crReflectionMapGenerator: public crCubeMapGenerator 
	{
	public:
		inline crReflectionMapGenerator(int texture_size = 64);
		inline crReflectionMapGenerator(const crReflectionMapGenerator &copy, const CRCore::crCopyOp &copyop = CRCore::crCopyOp::SHALLOW_COPY);

	protected:
		virtual ~crReflectionMapGenerator() {}
		crReflectionMapGenerator &operator=(const crReflectionMapGenerator &) { return *this; }

		inline virtual CRCore::crVector4 compute_color(const CRCore::crVector3 &R) const;
	};

	// INLINE METHODS

	inline crReflectionMapGenerator::crReflectionMapGenerator(int texture_size)
		: crCubeMapGenerator(texture_size)
	{
	}

	inline crReflectionMapGenerator::crReflectionMapGenerator(const crReflectionMapGenerator &copy, const CRCore::crCopyOp &copyop)
		: crCubeMapGenerator(copy, copyop)
	{
	}

	inline CRCore::crVector4 crReflectionMapGenerator::compute_color(const CRCore::crVector3 &R) const
	{
		return vector_to_color(R / R.length());
	}

}

#endif
