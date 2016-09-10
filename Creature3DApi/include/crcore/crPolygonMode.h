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
#ifndef CR_POLYGONMODE
#define CR_POLYGONMODE 1

#include <CRCore/crStateAttribute.h>

namespace CRCore{

  /** Class to for setting polygon culling mode.
  */     
class CR_EXPORT crPolygonMode : public crStateAttribute
{
public :

    crPolygonMode();

    /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
    crPolygonMode(const crPolygonMode& pm,const crCopyOp& copyop = crCopyOp::SHALLOW_COPY):
    crStateAttribute(pm,copyop),
    m_frontAndBack(pm.m_frontAndBack),
    m_frontMode(pm.m_frontMode),
    m_backMode(pm.m_backMode) {}

    META_StateAttribute(CRCore, crPolygonMode, POLYGONMODE);

      /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
    virtual int compare(const crStateAttribute& sa) const
	{ 
        // check the types are equal and then create the rhs variable
        // used by the COMPARE_StateAttribute_Paramter macro's below.
        COMPARE_StateAttribute_Types(crPolygonMode,sa)

        // compare each paramter in turn against the rhs.
        COMPARE_StateAttribute_Parameter(m_frontAndBack)
        COMPARE_StateAttribute_Parameter(m_frontMode)
        COMPARE_StateAttribute_Parameter(m_backMode)

        return 0; // passed all the above comparison macro's, must be equal.
	}
    
    virtual int compare(const crStateAttribute& sa)
	{
		  // check the types are equal and then create the rhs variable
		  // used by the COMPARE_StateAttribute_Paramter macro's below.
		COMPARE_StateAttribute_Types(crPolygonMode,sa)

		 // compare each paramter in turn against the rhs.
        COMPARE_StateAttribute_Parameter(m_frontAndBack)
		COMPARE_StateAttribute_Parameter(m_frontMode)
		COMPARE_StateAttribute_Parameter(m_backMode)

		return 0; // passed all the above comparison macro's, must be equal.
	}

    enum Face
	{
        FRONT,
        BACK,
        FRONT_AND_BACK
    };

    enum Mode 
	{
        POINT,//GL_POINT
        LINE,//GL_LINE,
        FILL//GL_FILL
    };

    void setMode(Face face,Mode mode);

    Mode getMode(Face face) const;

    inline bool getFrontAndBack() const { return m_frontAndBack; }

    virtual void apply(crState& state) const;
	virtual void addAttributeParam(int i, const std::string &str);
  protected:

    virtual ~crPolygonMode();

    bool m_frontAndBack;
    Mode m_frontMode;
    Mode m_backMode;

  };

}

#endif