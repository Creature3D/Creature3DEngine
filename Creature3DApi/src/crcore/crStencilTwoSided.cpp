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
#include <CRCore/crStencilTwoSided.h>
#include <CRCore/crState.h>
#include <crgl/GLExtensions.h>
#include <CRCore/crNotify.h>

using namespace CRCore;

crStencilTwoSided::crStencilTwoSided()
{
    // set up same defaults as glStencilFunc.
    m_func[FRONT] = m_func[BACK] = ALWAYS;
    m_funcRef[FRONT] = m_funcRef[BACK] = 0;
    m_funcMask[FRONT] = m_funcMask[BACK] = ~0u;
        
    // set up same defaults as glStencilOp.
    m_sfail[FRONT] = m_sfail[BACK] = KEEP;
    m_zfail[FRONT] = m_zfail[BACK] = KEEP;
    m_zpass[FRONT] = m_zpass[BACK] = KEEP;

    m_writeMask[FRONT] = m_writeMask[BACK] = ~0u;
}

crStencilTwoSided::crStencilTwoSided(const crStencilTwoSided& stencil,const crCopyOp& copyop):
    crStateAttribute(stencil,copyop)
{
    m_func[FRONT] = stencil.m_func[FRONT];
    m_funcRef[FRONT] = stencil.m_funcRef[FRONT];
    m_funcMask[FRONT] = stencil.m_funcMask[FRONT];
    m_sfail[FRONT] = stencil.m_sfail[FRONT];
    m_zfail[FRONT] = stencil.m_zfail[FRONT];
    m_zpass[FRONT] = stencil.m_zpass[FRONT];
    m_writeMask[FRONT] = stencil.m_writeMask[FRONT];

    m_func[BACK] = stencil.m_func[BACK];
    m_funcRef[BACK] = stencil.m_funcRef[BACK];
    m_funcMask[BACK] = stencil.m_funcMask[BACK];
    m_sfail[BACK] = stencil.m_sfail[BACK];
    m_zfail[BACK] = stencil.m_zfail[BACK];
    m_zpass[BACK] = stencil.m_zpass[BACK];
    m_writeMask[BACK] = stencil.m_writeMask[BACK];
}

crStencilTwoSided::~crStencilTwoSided()
{
}

int crStencilTwoSided::compare(const crStateAttribute& sa) const
{
    // check the types are equal and then create the rhs variable
    // used by the COMPARE_StateAttribute_Parameter macro's below.
    COMPARE_StateAttribute_Types(crStencilTwoSided,sa)

    // compare each parameter in turn against the rhs.
    COMPARE_StateAttribute_Parameter(m_func[FRONT])
    COMPARE_StateAttribute_Parameter(m_funcRef[FRONT])
    COMPARE_StateAttribute_Parameter(m_funcMask[FRONT])
    COMPARE_StateAttribute_Parameter(m_sfail[FRONT])
    COMPARE_StateAttribute_Parameter(m_zfail[FRONT])
    COMPARE_StateAttribute_Parameter(m_zpass[FRONT])
    COMPARE_StateAttribute_Parameter(m_writeMask[FRONT])

    COMPARE_StateAttribute_Parameter(m_func[BACK])
    COMPARE_StateAttribute_Parameter(m_funcRef[BACK])
    COMPARE_StateAttribute_Parameter(m_funcMask[BACK])
    COMPARE_StateAttribute_Parameter(m_sfail[BACK])
    COMPARE_StateAttribute_Parameter(m_zfail[BACK])
    COMPARE_StateAttribute_Parameter(m_zpass[BACK])
    COMPARE_StateAttribute_Parameter(m_writeMask[BACK])

    return 0; // passed all the above comparison macro's, must be equal.
}

int crStencilTwoSided::compare(const crStateAttribute& sa)
{
	// check the types are equal and then create the rhs variable
	// used by the COMPARE_StateAttribute_Parameter macro's below.
	COMPARE_StateAttribute_Types(crStencilTwoSided,sa)

	// compare each parameter in turn against the rhs.
	COMPARE_StateAttribute_Parameter(m_func[FRONT])
	COMPARE_StateAttribute_Parameter(m_funcRef[FRONT])
	COMPARE_StateAttribute_Parameter(m_funcMask[FRONT])
	COMPARE_StateAttribute_Parameter(m_sfail[FRONT])
	COMPARE_StateAttribute_Parameter(m_zfail[FRONT])
	COMPARE_StateAttribute_Parameter(m_zpass[FRONT])
	COMPARE_StateAttribute_Parameter(m_writeMask[FRONT])

	COMPARE_StateAttribute_Parameter(m_func[BACK])
	COMPARE_StateAttribute_Parameter(m_funcRef[BACK])
	COMPARE_StateAttribute_Parameter(m_funcMask[BACK])
	COMPARE_StateAttribute_Parameter(m_sfail[BACK])
	COMPARE_StateAttribute_Parameter(m_zfail[BACK])
	COMPARE_StateAttribute_Parameter(m_zpass[BACK])
	COMPARE_StateAttribute_Parameter(m_writeMask[BACK])

	return 0; // passed all the above comparison macro's, must be equal.
}

void crStencilTwoSided::apply(crState& state) const
{
    const unsigned int contextID = state.getContextID();
    const Extensions* extensions = getExtensions(contextID,true);

    if (!extensions->isStencilTwoSidedSupported())
        return;
        
    extensions->glActiveStencilFace(GL_BACK);
    glStencilOp((GLenum)m_sfail[BACK],(GLenum)m_zfail[BACK],(GLenum)m_zpass[BACK]);
    glStencilMask(m_writeMask[BACK]);
    glStencilFunc((GLenum)m_func[BACK],m_funcRef[BACK],m_funcMask[BACK]);

    extensions->glActiveStencilFace(GL_FRONT);
    glStencilOp((GLenum)m_sfail[FRONT],(GLenum)m_zfail[FRONT],(GLenum)m_zpass[FRONT]);
    glStencilMask(m_writeMask[FRONT]);
    glStencilFunc((GLenum)m_func[FRONT],m_funcRef[FRONT],m_funcMask[FRONT]);
}


typedef buffered_value< ref_ptr<crStencilTwoSided::Extensions> > BufferedExtensions;
static BufferedExtensions s_extensions;
void crStencilTwoSided::Extensions::clearExtensions()
{
	s_extensions.clear();
}
crStencilTwoSided::Extensions* crStencilTwoSided::getExtensions(unsigned int contextID,bool createIfNotInitalized)
{
    if (!s_extensions[contextID] && createIfNotInitalized) s_extensions[contextID] = new Extensions(contextID);
    return s_extensions[contextID].get();
}

void crStencilTwoSided::setExtensions(unsigned int contextID,Extensions* extensions)
{
    s_extensions[contextID] = extensions;
}

crStencilTwoSided::Extensions::Extensions(unsigned int contextID)
{
    setupGLExtensions(contextID);
}

crStencilTwoSided::Extensions::Extensions(const Extensions& rhs):
    Referenced()
{
    m_isStencilTwoSidedSupported = rhs.m_isStencilTwoSidedSupported;
    m_glActiveStencilFace = rhs.m_glActiveStencilFace;
}


void crStencilTwoSided::Extensions::lowestCommonDenominator(const Extensions& rhs)
{
    if (!rhs.m_isStencilTwoSidedSupported) m_isStencilTwoSidedSupported = false;

    if (!rhs.m_glActiveStencilFace) m_glActiveStencilFace = 0;

}

void crStencilTwoSided::Extensions::setupGLExtensions(unsigned int contextID)
{
    m_isStencilTwoSidedSupported = isGLExtensionSupported(contextID,"GL_EXT_stencil_two_side");

    m_glActiveStencilFace = CRCore::getGLExtensionFuncPtr("glActiveStencilFace","glActiveStencilFaceEXT");
}

void crStencilTwoSided::Extensions::glActiveStencilFace(GLenum face) const
{
    if (m_glActiveStencilFace)
    {
        typedef void (APIENTRY * ActiveStencilFaceProc) (GLenum);
        ((ActiveStencilFaceProc)m_glActiveStencilFace)(face);
    }
    else
    {
        notify(WARN)<<"Error: glActiveStencilFace not supported by OpenGL driver"<<std::endl;
    }    
}

void crStencilTwoSided::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		if(str.compare("NEVER")==0)
			m_func[0] = NEVER;
		else if(str.compare("LESS")==0)
			m_func[0] = LESS;
		else if(str.compare("EQUAL")==0)
			m_func[0] = EQUAL;
		else if(str.compare("LEQUAL")==0)
			m_func[0] = LEQUAL;
		else if(str.compare("GREATER")==0)
			m_func[0] = GREATER;
		else if(str.compare("NOTEQUAL")==0)
			m_func[0] = NOTEQUAL;
		else if(str.compare("GEQUAL")==0)
			m_func[0] = GEQUAL;
		else if(str.compare("ALWAYS")==0)
			m_func[0] = ALWAYS;
		break;
	case 1:
		m_funcRef[0] = atoi(str.c_str());
		break;
	case 2:
		m_funcMask[0] = atoi(str.c_str());
		break;
	case 3:
		if(str.compare("KEEP")==0)
			m_sfail[0] = KEEP;
		else if(str.compare("ZERO")==0)
			m_sfail[0] = ZERO;
		else if(str.compare("REPLACE")==0)
			m_sfail[0] = REPLACE;
		else if(str.compare("INCR")==0)
			m_sfail[0] = INCR;
		else if(str.compare("DECR")==0)
			m_sfail[0] = DECR;
		else if(str.compare("INVERT")==0)
			m_sfail[0] = INVERT;
		else if(str.compare("INCR_WRAP")==0)
			m_sfail[0] = INCR_WRAP;
		else if(str.compare("DECR_WRAP")==0)
			m_sfail[0] = DECR_WRAP;
		break;
	case 4:
		if(str.compare("KEEP")==0)
			m_zfail[0] = KEEP;
		else if(str.compare("ZERO")==0)
			m_zfail[0] = ZERO;
		else if(str.compare("REPLACE")==0)
			m_zfail[0] = REPLACE;
		else if(str.compare("INCR")==0)
			m_zfail[0] = INCR;
		else if(str.compare("DECR")==0)
			m_zfail[0] = DECR;
		else if(str.compare("INVERT")==0)
			m_zfail[0] = INVERT;
		else if(str.compare("INCR_WRAP")==0)
			m_zfail[0] = INCR_WRAP;
		else if(str.compare("DECR_WRAP")==0)
			m_zfail[0] = DECR_WRAP;
		break;
	case 5:
		if(str.compare("KEEP")==0)
			m_zpass[0] = KEEP;
		else if(str.compare("ZERO")==0)
			m_zpass[0] = ZERO;
		else if(str.compare("REPLACE")==0)
			m_zpass[0] = REPLACE;
		else if(str.compare("INCR")==0)
			m_zpass[0] = INCR;
		else if(str.compare("DECR")==0)
			m_zpass[0] = DECR;
		else if(str.compare("INVERT")==0)
			m_zpass[0] = INVERT;
		else if(str.compare("INCR_WRAP")==0)
			m_zpass[0] = INCR_WRAP;
		else if(str.compare("DECR_WRAP")==0)
			m_zpass[0] = DECR_WRAP;
		break;
	case 6:
		m_writeMask[0] = atoi(str.c_str());
		break;

	case 7:
		if(str.compare("NEVER")==0)
			m_func[1] = NEVER;
		else if(str.compare("LESS")==0)
			m_func[1] = LESS;
		else if(str.compare("EQUAL")==0)
			m_func[1] = EQUAL;
		else if(str.compare("LEQUAL")==0)
			m_func[1] = LEQUAL;
		else if(str.compare("GREATER")==0)
			m_func[1] = GREATER;
		else if(str.compare("NOTEQUAL")==0)
			m_func[1] = NOTEQUAL;
		else if(str.compare("GEQUAL")==0)
			m_func[1] = GEQUAL;
		else if(str.compare("ALWAYS")==0)
			m_func[1] = ALWAYS;
		break;
	case 8:
		m_funcRef[1] = atoi(str.c_str());
		break;
	case 9:
		m_funcMask[1] = atoi(str.c_str());
		break;
	case 10:
		if(str.compare("KEEP")==0)
			m_sfail[1] = KEEP;
		else if(str.compare("ZERO")==0)
			m_sfail[1] = ZERO;
		else if(str.compare("REPLACE")==0)
			m_sfail[1] = REPLACE;
		else if(str.compare("INCR")==0)
			m_sfail[1] = INCR;
		else if(str.compare("DECR")==0)
			m_sfail[1] = DECR;
		else if(str.compare("INVERT")==0)
			m_sfail[1] = INVERT;
		else if(str.compare("INCR_WRAP")==0)
			m_sfail[1] = INCR_WRAP;
		else if(str.compare("DECR_WRAP")==0)
			m_sfail[1] = DECR_WRAP;
		break;
	case 11:
		if(str.compare("KEEP")==0)
			m_zfail[1] = KEEP;
		else if(str.compare("ZERO")==0)
			m_zfail[1] = ZERO;
		else if(str.compare("REPLACE")==0)
			m_zfail[1] = REPLACE;
		else if(str.compare("INCR")==0)
			m_zfail[1] = INCR;
		else if(str.compare("DECR")==0)
			m_zfail[1] = DECR;
		else if(str.compare("INVERT")==0)
			m_zfail[1] = INVERT;
		else if(str.compare("INCR_WRAP")==0)
			m_zfail[1] = INCR_WRAP;
		else if(str.compare("DECR_WRAP")==0)
			m_zfail[1] = DECR_WRAP;
		break;
	case 12:
		if(str.compare("KEEP")==0)
			m_zpass[1] = KEEP;
		else if(str.compare("ZERO")==0)
			m_zpass[1] = ZERO;
		else if(str.compare("REPLACE")==0)
			m_zpass[1] = REPLACE;
		else if(str.compare("INCR")==0)
			m_zpass[1] = INCR;
		else if(str.compare("DECR")==0)
			m_zpass[1] = DECR;
		else if(str.compare("INVERT")==0)
			m_zpass[1] = INVERT;
		else if(str.compare("INCR_WRAP")==0)
			m_zpass[1] = INCR_WRAP;
		else if(str.compare("DECR_WRAP")==0)
			m_zpass[1] = DECR_WRAP;
		break;
	case 13:
		m_writeMask[1] = atoi(str.c_str());
		break;
	}

	return crStateAttribute::addAttributeParam(i-14,str);
}