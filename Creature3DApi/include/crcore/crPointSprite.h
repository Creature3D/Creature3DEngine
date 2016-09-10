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
//Modified by Wucaihua
#ifndef CRCORE_POINT_SPRITE
#define CRCORE_POINT_SPRITE 1

#include <CRCore/crStateAttribute.h>

#ifndef GL_ARB_point_sprite
#define GL_POINT_SPRITE_ARB               0x8861
#define GL_COORD_REPLACE_ARB              0x8862
#endif

namespace CRCore {

/** crPointSprite base class which encapsulates enabling of point sprites .*/
class CR_EXPORT crPointSprite : public CRCore::crStateAttribute 
{
public:

	crPointSprite() {}

	/** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
	crPointSprite(const crPointSprite& texenv,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY):        
	    crStateAttribute(texenv,copyop) {}


	META_StateAttribute(CRCore, crPointSprite, POINTSPRITE);

	/** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
	virtual int compare(const crStateAttribute& sa) const;
	virtual int compare(const crStateAttribute& sa);
   /*     virtual bool getModeUsage(ModeUsage& usage) const
        {
            usage.usesMode(GL_POINT_SPRITE_ARB);
            return true;
        }*/

	virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
	{
		// Have to think about the role of m_colorMode
		// in setting the colormaterial... also need to take the
		// color material enable/disable out of the the apply()...
		modes.push_back(GL_POINT_SPRITE_ARB);
	}

	virtual bool isTextureAttribute() const { return true; }

	virtual bool checkValidityOfAssociatedModes(crState&) const;
	static bool isPointSpriteSupported(unsigned int context);
	virtual void apply(crState& state) const;
protected:
	virtual ~crPointSprite( void ) {}
};

}

#endif
