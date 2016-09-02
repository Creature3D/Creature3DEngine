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
#ifndef CRCORE_CRTEXGENNODE
#define CRCORE_CRTEXGENNODE 1

#include <CRCore/crGroup.h>
#include <CRCore/crTexGen.h>

namespace CRCore {

/** Node for defining the position of crTexGen in the scene. */
class CR_EXPORT crTexGenNode : public crGroup
{

    public:

        crTexGenNode();
        crTexGenNode(crTexGen* texgen);

        crTexGenNode(const crTexGenNode& tgb, const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_Node(CRCore, crTexGenNode);
	
		enum ReferenceFrame
		{
			RELATIVE_RF,
			ABSOLUTE_RF
		};

		/** Set the TexGenNode's ReferenceFrame, either to be relative to its
		* parent reference frame. */
		void setReferenceFrame(ReferenceFrame rf);

		/** Ge thte TexGenNode's ReferenceFrame.*/
		ReferenceFrame getReferenceFrame() const { return m_referenceFrame; }

        void setTextureUnit(unsigned int textureUnit) { m_textureUnit = textureUnit; }

        unsigned int getTextureUnit() const { return m_textureUnit; }

        /** Set the crTexGen. */
        void setTexGen(crTexGen* texgen);
        
        /** Get the crTexGen. */
        inline crTexGen* getTexGen() { return m_texgen.get(); }

        /** Get the const crTexGen. */
        inline const crTexGen* getTexGen() const { return m_texgen.get(); }


    protected:

        virtual ~crTexGenNode();

        unsigned int m_textureUnit;
		crStateAttribute::VRModeValue m_value;
        CRCore::ref_ptr<crTexGen> m_texgen;
		ReferenceFrame                 m_referenceFrame;
};

}

#endif
