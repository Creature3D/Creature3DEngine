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
#include <CRCore/crStencil.h>

using namespace CRCore;

crStencil::crStencil()
{
    // set up same defaults as glStencilFunc.
    m_func = ALWAYS;
    m_funcRef = 0;
    m_funcMask = ~0;
        
    // set up same defaults as glStencilOp.
    m_sfail = KEEP;
    m_zfail = KEEP;
    m_zpass = KEEP;

    // set up same defaults as glStencilMask.
    m_writeMask = ~0;
}


crStencil::~crStencil()
{
}

void crStencil::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		if(str.compare("NEVER")==0)
			m_func = NEVER;
		else if(str.compare("LESS")==0)
			m_func = LESS;
		else if(str.compare("EQUAL")==0)
			m_func = EQUAL;
		else if(str.compare("LEQUAL")==0)
			m_func = LEQUAL;
		else if(str.compare("GREATER")==0)
			m_func = GREATER;
		else if(str.compare("NOTEQUAL")==0)
			m_func = NOTEQUAL;
		else if(str.compare("GEQUAL")==0)
			m_func = GEQUAL;
		else if(str.compare("ALWAYS")==0)
			m_func = ALWAYS;
		break;
	case 1:
		m_funcRef = atoi(str.c_str());
		break;
	case 2:
		m_funcMask = atoi(str.c_str());
		break;
	case 3:
		if(str.compare("KEEP")==0)
			m_sfail = KEEP;
		else if(str.compare("ZERO")==0)
			m_sfail = ZERO;
		else if(str.compare("REPLACE")==0)
			m_sfail = REPLACE;
		else if(str.compare("INCR")==0)
			m_sfail = INCR;
		else if(str.compare("DECR")==0)
			m_sfail = DECR;
		else if(str.compare("INVERT")==0)
			m_sfail = INVERT;
		break;
	case 4:
		if(str.compare("KEEP")==0)
			m_zfail = KEEP;
		else if(str.compare("ZERO")==0)
			m_zfail = ZERO;
		else if(str.compare("REPLACE")==0)
			m_zfail = REPLACE;
		else if(str.compare("INCR")==0)
			m_zfail = INCR;
		else if(str.compare("DECR")==0)
			m_zfail = DECR;
		else if(str.compare("INVERT")==0)
			m_zfail = INVERT;
		break;
	case 5:
		if(str.compare("KEEP")==0)
			m_zpass = KEEP;
		else if(str.compare("ZERO")==0)
			m_zpass = ZERO;
		else if(str.compare("REPLACE")==0)
			m_zpass = REPLACE;
		else if(str.compare("INCR")==0)
			m_zpass = INCR;
		else if(str.compare("DECR")==0)
			m_zpass = DECR;
		else if(str.compare("INVERT")==0)
			m_zpass = INVERT;
		break;
	case 6:
		m_writeMask = atoi(str.c_str());
		break;
	}

	return crStateAttribute::addAttributeParam(i-7,str);
}
#include <Driver/GLDrv/crStencilDrv.h>
