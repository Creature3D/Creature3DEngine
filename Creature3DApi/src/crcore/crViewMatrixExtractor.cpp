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
#include <CRCore/crViewMatrixExtractor.h>
#include <CRCore/crState.h>

using namespace CRCore;

void crViewMatrixExtractor::apply(crState &state) const
{
	if (m_first_context == -1) 
	{
		m_first_context = state.getContextID();
	}
	if (state.getContextID() == (unsigned int)m_first_context) 
	{
		if (m_vp.valid()) 
		{
			CRCore::crMatrix M = state.getInitialInverseViewMatrix();
			for (int i=0; i<4; ++i) 
			{
				m_vp->setProgramLocalParameter(m_param+i, CRCore::crVector4(M(0, i), M(1, i), M(2, i), M(3, i)));
			}                
		}
	}
}