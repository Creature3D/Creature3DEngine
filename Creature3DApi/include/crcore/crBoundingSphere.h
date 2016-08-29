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

#ifndef CRCORE_BOUNDINGSPHERE_H
#define CRCORE_BOUNDINGSPHERE_H 1

#include <CRCore/crExport.h>
#include <CRCore/crVector3.h>

namespace CRCore {

	class crBoundingBox;

	/** General purpose bounding sphere class for enclosing nodes/objects/vertices.
	Used to bound internal CRCore::Node's in the scene,
	to assist in view frustum culling etc.  Similar in function to crBoundingBox
	but is quicker for evaluating culling, but generally encloses a greater volume
	than a crBoundingBox so will not cull so aggressively.
	*/
	class CR_EXPORT crBoundingSphere
	{
	public:

		crVector3 m_center;
		float     m_fRadius;

		/** construct to invalid values to represent an unset bounding sphere.*/ 
		crBoundingSphere() : m_center(0.0f,0.0f,0.0f),m_fRadius(-1.0f) {}

		/** construct to specified bounding sphere.*/ 
		crBoundingSphere(const crVector3& center,float radius) : m_center(center),m_fRadius(radius) {}

		/** initialize to invalid values to represent an unset bounding sphere.*/ 
		inline void init()
		{
			m_center.set(0.0f,0.0f,0.0f);
			m_fRadius = -1.0f;
		}

		/** return true if the bounding sphere contains valid values,
		false if the bounding sphere is effectively unset.*/
		inline bool valid() const { return m_fRadius>=0.0f; }

		/** set bounding sphere.*/ 
		inline void set(const crVector3& center,float radius)
		{
			m_center = center;
			m_fRadius = radius;
		}

		/** return the center of the bounding sphere.*/
		inline crVector3& center() { return m_center; }
		/** return the const center of the bounding sphere.*/
		inline const crVector3& center() const { return m_center; }

		/** return the radius of the bounding sphere.*/
		inline float& radius() { return m_fRadius; }

		/** return the const radius of the bounding sphere.*/
		inline float radius() const { return m_fRadius; }

		/** return the radius squared. 
		Note, for performance reasons, assumes the calling method has ensured 
		that the sphere is valid before calling radius2(), i.e. has m_fRadius>=0.0,
		as it does not check th validity of sphere and will erroneously return a positive value.*/
		inline float radius2() const { return m_fRadius*m_fRadius; }

		/** If the vertex is out-with the sphere expand to encompass vertex.
		Calculates the combination of movement of center and radius which
		minimizes the radius increase. If this sphere is empty then
		move the center to v and set radius to 0.*/
		void expandBy(const crVector3& v);

		/** If the vertex is outwith the sphere expand radius to ecompass vertex.
		Unlike update, does not move the center, just increasing the radius.
		If this sphere is empty then move the centrer to v and set radius to 0 */
		void expandRadiusBy(const crVector3& v);

		/** If incomming sphere is outwith the sphere expand to ecompass incomming sphere.
		calculates the combination of movement of center and radius which
		minimizes the radius increase. If this sphere is empty then
		move the centrer to v and set radius to 0.*/
		void expandBy(const crBoundingSphere& sh);

		/** If incomming sphere is outwith the sphere expand radius to ecompass incomming sphere.
		Unlike update, does not move the center, just increasing the radius.
		If this sphere is empty then move the centrer to v and set radius to 0. */
		void expandRadiusBy(const crBoundingSphere& sh);

		/** If incomming box is outwith the sphere expand to ecompass incomming box.
		calculates the combination of movement of center and radius which
		minimizes the radius increase. If this boz is empty then
		move the centrer to v and set radius to 0.*/
		void expandBy(const crBoundingBox& bb);

		/** If incomming box is outwith the sphere expand radius to ecompass incomming box.
		Unlike update, does not move the center, just increasing the radius.
		If this sphere is empty then move the centrer to v and set radius to 0. */
		void expandRadiusBy(const crBoundingBox& bb);

		/** return true is vertex v is within the sphere.*/
		inline bool contains(const crVector3& v) const
		{
			return valid() && ((v-m_center).length2()<=radius2());
		}

		/** return true if bounding sphere's intersect each other.*/
		inline bool intersects( const crBoundingSphere& bs ) const
		{
			return valid() && bs.valid() &&
				((m_center - bs.m_center).length2() <= (m_fRadius + bs.m_fRadius)*(m_fRadius + bs.m_fRadius));
		}

		inline crBoundingSphere& operator = (const crBoundingSphere& bs) 
		{ m_center = bs.m_center; m_fRadius = bs.m_fRadius; return (*this); }

	};

}

#endif
