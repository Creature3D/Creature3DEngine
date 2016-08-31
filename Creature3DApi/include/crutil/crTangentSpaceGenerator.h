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
#ifndef CRUTIL_TANGENTSPACEGENERATOR_H
#define CRUTIL_TANGENTSPACEGENERATOR_H

#include <CRUtil/crExport.h>

#include <CRCore/ref_ptr.h>
#include <CRCore/Referenced.h>
#include <CRCore/crArray.h>
#include <CRCore/crGeometry.h>

namespace CRUtil
{

	/**
	 This class generates three arrays containing tangent-space basis vectors. It takes
	 a texture-mapped crGeometry object as input, traverses its primitive sets and computes
	 Tangent, Normal and Binormal vectors for each vertex, storing them into arrays.
	 The resulting arrays can be used as vertex program varying (per-vertex) parameters,
	 enabling advanced effects like bump-mapping.
	 To use this class, simply call the generate() method specifying the crGeometry object
	 you want to process and the texture unit that contains UV mapping for the normal map;
	 then you can retrieve the TBN arrays by calling getTangentArray(), getNormalArray()
	 and getBinormalArray() methods.
	 */
	class CRUTIL_EXPORT crTangentSpaceGenerator: public CRCore::Referenced 
	{
	public:
		crTangentSpaceGenerator();
		crTangentSpaceGenerator(const crTangentSpaceGenerator &copy, const CRCore::crCopyOp &copyop = CRCore::crCopyOp::SHALLOW_COPY);

		bool generate(CRCore::crGeometry *geo, int normal_map_tex_unit = 0);

		inline CRCore::Vec3Array *getTangentArray()               { return m_T.get(); }
		inline const CRCore::Vec3Array *getTangentArray() const   { return m_T.get(); }
		inline void setTangentArray(CRCore::Vec3Array *array)     { m_T = array; }

		inline CRCore::Vec3Array *getNormalArray()                { return m_N.get(); }
		inline const CRCore::Vec3Array *getNormalArray() const    { return m_N.get(); }
		inline void setNormalArray(CRCore::Vec3Array *array)      { m_N = array; }

		inline CRCore::Vec3Array *getBinormalArray()              { return m_B.get(); }
		inline const CRCore::Vec3Array *getBinormalArray() const  { return m_B.get(); }
		inline void setBinormalArray(CRCore::Vec3Array *array)    { m_B = array; }
        
	protected:
		virtual ~crTangentSpaceGenerator() {}
		crTangentSpaceGenerator &operator=(const crTangentSpaceGenerator &) { return *this; }

		void compute_basis_vectors(CRCore::crPrimitive *pset, const CRCore::Array *vx, const CRCore::Array *nx, const CRCore::Array *tx, int iA, int iB, int iC);

	private:
		CRCore::ref_ptr<CRCore::Vec3Array> m_T;
		CRCore::ref_ptr<CRCore::Vec3Array> m_B;
		CRCore::ref_ptr<CRCore::Vec3Array> m_N;
	};

}

#endif
