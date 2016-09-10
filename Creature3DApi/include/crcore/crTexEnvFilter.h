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
#ifndef CRCORE_CRTEXENVFILTER_H
#define CRCORE_CRTEXENVFILTER_H 1

#include <CRCore/crStateAttribute.h>

namespace CRCore {

/** crTexEnvFilter - encapsulates the OpenGL glTexEnv (GL_TEXTURE_FILTER_CONTROL) state.*/
class CR_EXPORT crTexEnvFilter : public crStateAttribute
{
  public:
    crTexEnvFilter(float lodBias = 0.0f);
        
    /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
    crTexEnvFilter(const crTexEnvFilter& texenv,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(texenv,copyop),
            m_lodBias(texenv.m_lodBias) {}

    META_StateAttribute(CRCore, crTexEnvFilter, TEXENVFILTER);

    virtual bool isTextureAttribute() const { return true; }

    /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
    virtual int compare(const crStateAttribute& sa) const
    {
        // check the types are equal and then create the rhs variable
        // used by the COMPARE_StateAttribute_Paramter macro's below.
        COMPARE_StateAttribute_Types(crTexEnvFilter, sa)

        // compare each paramter in turn against the rhs.
        COMPARE_StateAttribute_Parameter(m_lodBias)

        return 0; // passed all the above comparison macro's, must be equal.
    }

	virtual int compare(const crStateAttribute& sa) 
	{
		// check the types are equal and then create the rhs variable
		// used by the COMPARE_StateAttribute_Paramter macro's below.
		COMPARE_StateAttribute_Types(crTexEnvFilter, sa)

		// compare each paramter in turn against the rhs.
		COMPARE_StateAttribute_Parameter(m_lodBias)

		return 0; // passed all the above comparison macro's, must be equal.
	}

    void  setLodBias( float lodBias ) { m_lodBias = lodBias; }
        
    float getLodBias() const { return m_lodBias; }

    virtual void apply(crState& state) const;
	virtual void addAttributeParam(int i, const std::string &str);

  protected:
    virtual ~crTexEnvFilter();

    float   m_lodBias;
};

}

#endif
