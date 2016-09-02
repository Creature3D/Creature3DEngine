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
#ifndef CRCORE_CRVIEWMATRIXEXTRACTOR_H
#define CRCORE_CRVIEWMATRIXEXTRACTOR_H 1

#include <CRCore/crStateAttribute.h>
#include <CRCore/crVertexProgram.h>

namespace CRCore {

// a state attribute class that grabs the initial inverse view matrix
// and sends it to a crVertexProgram.
// NOTE: due to lack of support for per-context parameters in crVertexProgram,
// this class will send the matrix to the vp only while the first context
// is being rendered. All subsequent contexts will use the first context's
// matrix.   
class CR_EXPORT crViewMatrixExtractor : public crStateAttribute
{
    public:
        crViewMatrixExtractor():   
		    CRCore::crStateAttribute(),
            m_vp(0),
            m_param(0),
            m_first_context(-1)
        {
        }

        crViewMatrixExtractor(const crViewMatrixExtractor &copy, const CRCore::crCopyOp &copyop):    
		    CRCore::crStateAttribute(copy, copyop),
            m_vp(static_cast<CRCore::crVertexProgram *>(copyop(copy.m_vp.get()))),
            m_param(copy.m_param),
            m_first_context(-1)
        {
        }

        crViewMatrixExtractor(CRCore::crVertexProgram *vp, int param):    
		    CRCore::crStateAttribute(),
            m_vp(vp),
            m_param(param),
            m_first_context(-1)
        {
        }

        META_StateAttribute(CRCore, crViewMatrixExtractor, VIEWMATRIXEXTRACTOR);

        int compare(const CRCore::crStateAttribute &sa) const
        {
            COMPARE_StateAttribute_Types(crViewMatrixExtractor, sa);
			if (m_vp.get() != rhs.m_vp.get())
			{
				if(*m_vp!=*(rhs.m_vp))			
					return -1;
			}
            if (m_param < rhs.m_param) return -1;
            if (m_param > rhs.m_param) return 1;
            return 0;
        }

		int compare(const CRCore::crStateAttribute &sa)
		{
			COMPARE_StateAttribute_Types(crViewMatrixExtractor, sa);
			if (m_vp.get() != rhs.m_vp.get())
			{
                if(*m_vp!=*(rhs.m_vp))			
				    return -1;
			}
			if (m_param < rhs.m_param) return -1;
			if (m_param > rhs.m_param) return 1;
			return 0;
		}

        virtual void apply(CRCore::crState &state) const;

    private:
        mutable CRCore::ref_ptr<CRCore::crVertexProgram> m_vp;
        int m_param;
        mutable int m_first_context;
};

}

#endif
