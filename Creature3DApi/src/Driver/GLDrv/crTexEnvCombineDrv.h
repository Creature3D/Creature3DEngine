/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/
#include <crgl/gl.h>
#include <crgl/glu.h>
#include <crgl/GLExtensions.h>
#include <CRCore/crState.h>
#include <CRCore/crStreamBuf.h>
#define parseCombineParam(dest,src)\
{\
    switch(src) \
    {\
    case crTexEnvCombine::REPLACE:\
	    dest = GL_REPLACE;\
		break;\
    case crTexEnvCombine::MODULATE:\
	    dest = GL_MODULATE;\
		break;\
    case crTexEnvCombine::ADD:\
	    dest = GL_ADD;\
		break;\
    case crTexEnvCombine::ADD_SIGNED:\
	    dest = GL_ADD_SIGNED;\
		break;\
    case crTexEnvCombine::INTERPOLATE:\
	    dest = GL_INTERPOLATE;\
		break;\
    case crTexEnvCombine::SUBTRACT:\
	    dest = GL_SUBTRACT;\
		break;\
    case crTexEnvCombine::DOT3_RGB:\
	    dest = GL_DOT3_RGB;\
		break;\
    case crTexEnvCombine::DOT3_RGBA:\
	    dest = GL_DOT3_RGBA;\
		break;\
	}\
}

#define parseSourceParam(dest,src)\
{\
    switch(src) \
    {\
    case crTexEnvCombine::CONSTANT:\
	    dest = GL_CONSTANT_ARB;\
		break;\
    case crTexEnvCombine::PRIMARY_COLOR:\
	    dest = GL_PRIMARY_COLOR_ARB;\
		break;\
    case crTexEnvCombine::PREVIOUS:\
	    dest = GL_PREVIOUS_ARB;\
		break;\
    case crTexEnvCombine::TEXTURE:\
	    dest = GL_TEXTURE;\
		break;\
    case crTexEnvCombine::TEXTURE0:\
	    dest = GL_TEXTURE0;\
		break;\
    case crTexEnvCombine::TEXTURE1:\
	    dest = GL_TEXTURE1;\
		break;\
    case crTexEnvCombine::TEXTURE2:\
	    dest = GL_TEXTURE2;\
		break;\
    case crTexEnvCombine::TEXTURE3:\
	    dest = GL_TEXTURE3;\
		break;\
    case crTexEnvCombine::TEXTURE4:\
	    dest = GL_TEXTURE4;\
		break;\
    case crTexEnvCombine::TEXTURE5:\
	    dest = GL_TEXTURE5;\
		break;\
    case crTexEnvCombine::TEXTURE6:\
	    dest = GL_TEXTURE6;\
		break;\
    case crTexEnvCombine::TEXTURE7:\
	    dest = GL_TEXTURE7;\
		break;\
	}\
}

#define parseOperandParam(dest,src)\
{\
    switch(src) \
    {\
    case crTexEnvCombine::SRC_COLOR:\
	    dest = GL_SRC_COLOR;\
		break;\
    case crTexEnvCombine::ONE_MINUS_SRC_COLOR:\
	    dest = GL_ONE_MINUS_SRC_COLOR;\
		break;\
    case crTexEnvCombine::SRC_ALPHA:\
	    dest = GL_SRC_ALPHA;\
		break;\
    case crTexEnvCombine::ONE_MINUS_SRC_ALPHA:\
	    dest = GL_ONE_MINUS_SRC_ALPHA;\
		break;\
	}\
}
void crTexEnvCombine::apply(crState& state) const
{
	unsigned int contextID = state.getContextID();

    static bool s_isTexEnvCombineSupported =
        isGLExtensionSupported(contextID,"GL_ARB_texture_env_combine");

    static bool s_isTexEnvCrossbarSupported =
        isGLExtensionSupported(contextID,"GL_ARB_texture_env_crossbar");

    static bool s_isNVTexEnvCrossbarSupported =
        isGLExtensionSupported(contextID,"GL_NV_texture_env_combine4");

    static bool s_isTexEnvDot3Supported = 
        isGLExtensionSupported(contextID,"GL_ARB_texture_env_dot3");

    int combine_RGB;
	int combine_Alpha;

	int source0_RGB;
	int source1_RGB;
	int source2_RGB;
	int source0_Alpha;
	int source1_Alpha;
	int source2_Alpha;

	int operand0_RGB;
	int operand1_RGB;
	int operand2_RGB;
	int operand0_Alpha;
	int operand1_Alpha;
	int operand2_Alpha;

	//float  scale_RGB;
	//float  scale_Alpha;

    parseCombineParam(combine_RGB,m_combine_RGB);
    parseCombineParam(combine_Alpha,m_combine_Alpha);

    parseSourceParam(source0_RGB,m_source0_RGB);
	parseSourceParam(source1_RGB,m_source1_RGB);
	parseSourceParam(source2_RGB,m_source2_RGB);
	parseSourceParam(source0_Alpha,m_source0_Alpha);
	parseSourceParam(source1_Alpha,m_source1_Alpha);
	parseSourceParam(source2_Alpha,m_source2_Alpha);

	parseOperandParam(operand0_RGB,m_operand0_RGB);
    parseOperandParam(operand1_RGB,m_operand1_RGB);
    parseOperandParam(operand2_RGB,m_operand2_RGB);
    parseOperandParam(operand0_Alpha,m_operand0_Alpha);
    parseOperandParam(operand1_Alpha,m_operand1_Alpha);
    parseOperandParam(operand2_Alpha,m_operand2_Alpha);

    //scale_RGB = m_ptex->getScale_RGB();
    //scale_Alpha = m_ptex->getScale_Alpha();

	bool needsTexEnvDot3 = (combine_RGB==GL_DOT3_RGB) ||
                           (combine_RGB==GL_DOT3_RGBA);

    bool supported = s_isTexEnvCombineSupported;
    if (m_needsTexEnvCrossbar && !(s_isTexEnvCrossbarSupported || s_isNVTexEnvCrossbarSupported)) supported = false;
    if (needsTexEnvDot3 && !s_isTexEnvDot3Supported) supported = false;

    if (supported)
    {
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);

        glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, combine_RGB);
        
        if (combine_RGB!=GL_DOT3_RGBA)
            glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB,combine_Alpha);

        glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB,source0_RGB);
        glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB,source1_RGB);
        glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB,source2_RGB);

        glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB,source0_Alpha);
        glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB,source1_Alpha);
        glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE2_ALPHA_ARB,source2_Alpha);

        glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, operand0_RGB);
        glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, operand1_RGB);
        glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, operand2_RGB);

        glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB,operand0_Alpha);
        glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND1_ALPHA_ARB,operand1_Alpha);
        glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND2_ALPHA_ARB,operand2_Alpha);


        glTexEnvf( GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, m_scale_RGB);
        glTexEnvf( GL_TEXTURE_ENV, GL_ALPHA_SCALE, m_scale_Alpha);

        glTexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR,m_constantColor.ptr());
    }
    else
    {
        // what is the best fallback when the tex env combine is not supported??
        // we will resort the settung the OpenGL default of GL_MODULATE.
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   }

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crTexEnvCombine OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif

}