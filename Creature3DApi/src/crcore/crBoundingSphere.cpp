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
#include <CRCore/crBoundingSphere.h>
#include <CRCore/crBoundingBox.h>

using namespace CRCore;

void crBoundingSphere::expandBy(const crVector3& v)
{
	if (valid())
	{
		crVector3 dv = v-m_center;
		float r = dv.length();
		if (r>m_fRadius)
		{
			float dr = (r-m_fRadius)*0.5f;
			m_center += dv*(dr/r);
			m_fRadius += dr;
		} 
	}
	else
	{
		m_center = v;
		m_fRadius = 0.0f;
	}
}


void crBoundingSphere::expandRadiusBy(const crVector3& v)
{
	if (valid())
	{
		float r = (v - m_center).length();
		if (r > m_fRadius) m_fRadius = r;
	}
	else
	{
		m_center = v;
		m_fRadius = 0.0f;
	}
}


void crBoundingSphere::expandBy(const crBoundingSphere& sh)
{
	if (sh.valid())
	{
		if (valid())
		{
			crVector3 dv = sh.m_center-m_center;
			float dv_len = dv.length();
			if (dv_len+sh.m_fRadius>m_fRadius)
			{
				crVector3 e1 = m_center - ( dv * (m_fRadius/dv_len));
				crVector3 e2 = sh.m_center + (dv*(sh.m_fRadius/dv_len));
				m_center = (e1 + e2)*0.5f;
				m_fRadius = (e2 - m_center).length();

			}
		}
		else
		{
			m_center = sh.m_center;
			m_fRadius = sh.m_fRadius;
		}
	}
}


void crBoundingSphere::expandRadiusBy(const crBoundingSphere& sh)
{
	if (sh.valid())
	{
		if (valid())
		{
			float r = (sh.m_center-m_center).length()+sh.m_fRadius;
			if (r > m_fRadius) m_fRadius = r;
			// else do nothing as vertex is within sphere.
		}
		else
		{
			m_center = sh.m_center;
			m_fRadius = sh.m_fRadius;
		}
	}
}

void crBoundingSphere::expandBy(const crBoundingBox& bb)
{
	if (bb.valid())
	{
		if (valid())
		{
			crBoundingBox newbb(bb);

			for(unsigned int c=0;c<8;++c)
			{
				crVector3 v = bb.corner(c)-m_center; // get the direction vector from corner
				v.normalize(); // normalise it.
				v *= -m_fRadius; // move the vector in the opposite direction distance radius.
				v += m_center; // move to absolute position.
				newbb.expandBy(v); // add it into the new bounding box.
			}

			m_center = newbb.center();
			m_fRadius = newbb.radius();

		}
		else
		{
			m_center = bb.center();
			m_fRadius = bb.radius();
		}
	}
}

void crBoundingSphere::expandRadiusBy(const crBoundingBox& bb)
{
	if (bb.valid())
	{
		if (valid())
		{
			for(unsigned int c=0;c<8;++c)
			{
				expandRadiusBy(bb.corner(c));
			}
		}
		else
		{
			m_center = bb.center();
			m_fRadius = bb.radius();
		}
	}
}
