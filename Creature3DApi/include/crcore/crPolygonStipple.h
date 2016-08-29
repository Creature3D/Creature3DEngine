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
#ifndef CRCORE_CRPOLYGONSTIPPLE_H
#define CRCORE_CRPOLYGONSTIPPLE_H 1

#include <CRCore/crStateAttribute.h>

namespace CRCore
{

class CR_EXPORT crPolygonStipple : public crStateAttribute
{
    public :

        crPolygonStipple();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crPolygonStipple(const crPolygonStipple& lw,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_StateAttribute(CRCore, crPolygonStipple, POLYGONSTIPPLE);

        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const;
		virtual int compare(const crStateAttribute& sa);

        //virtual bool getModeUsage(ModeUsage& usage) const
        //{
        //    usage.usesMode(GL_POLYGON_STIPPLE);
        //    return true;
        //}
		virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
		{
			modes.push_back(GL_POLYGON_STIPPLE);
		}

        /** set the mask up, copying 128 bytes (32x32 bitfield) from mask into the local m_mask.*/
        void setMask(const unsigned char* mask);

        /** get a pointer to the mask.*/
        inline const unsigned char* getMask() const {return m_mask;}
		virtual void apply(crState& state) const;
    protected :

        virtual ~crPolygonStipple();

        unsigned char m_mask[128];

};

}

#endif
