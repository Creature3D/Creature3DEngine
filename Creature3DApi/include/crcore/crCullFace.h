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
#ifndef CRCORE_CRCULLFACE_H
#define CRCORE_CRCULLFACE_H 1

#include <CRCORE/crStateAttribute.h>

namespace CRCore {
  
class CR_EXPORT crCullFace : public crStateAttribute
{
    public :
        enum Mode {
            FRONT,
            BACK,
            FRONT_AND_BACK
        };
    
        crCullFace(Mode mode=BACK):
            m_mode(mode) {}

        crCullFace(const crCullFace& cf,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(cf,copyop),
            m_mode(cf.m_mode) {}

        META_StateAttribute(CRCore, crCullFace, CULLFACE);
        
        virtual int compare(const crStateAttribute& sa) const
        {
            COMPARE_StateAttribute_Types(crCullFace,sa);

            COMPARE_StateAttribute_Parameter(m_mode);

            return 0;
        }

		virtual int compare(const crStateAttribute& sa)
		{
			COMPARE_StateAttribute_Types(crCullFace,sa);

			COMPARE_StateAttribute_Parameter(m_mode);

			return 0;
		}

        virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
        {
			modes.push_back(GL_CULL_FACE);
        }

        inline void setMode(Mode mode) { m_mode = mode; }

        inline Mode getMode() const { return m_mode; }
    
        virtual void apply(crState& state) const;
		virtual void addAttributeParam(int i, const std::string &str);
    protected:
    
        virtual ~crCullFace();
        
        Mode m_mode;

};

}

#endif
