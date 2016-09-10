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
#include <CRCore/crTexEnvCombine.h>
#include <CRCore/crNotify.h>
#include <CRCore/crArgumentParser.h>
using namespace CRCore;

crTexEnvCombine::crTexEnvCombine():
            m_needsTexEnvCrossbar(false),
            m_combine_RGB(MODULATE),
            m_combine_Alpha(MODULATE),
            m_source0_RGB(TEXTURE),
            m_source1_RGB(PREVIOUS),
            m_source2_RGB(CONSTANT),
            m_source0_Alpha(TEXTURE),
            m_source1_Alpha(PREVIOUS),
            m_source2_Alpha(CONSTANT),
            m_operand0_RGB(SRC_COLOR),
            m_operand1_RGB(SRC_COLOR),
            m_operand2_RGB(SRC_ALPHA),
            m_operand0_Alpha(SRC_ALPHA),
            m_operand1_Alpha(SRC_ALPHA),
            m_operand2_Alpha(SRC_ALPHA),
            m_scale_RGB(1.0),
            m_scale_Alpha(1.0),
            m_constantColor(0.0f,0.0f,0.0f,0.0f)
{
}

crTexEnvCombine::~crTexEnvCombine()
{
}

//void crTexEnvCombine::apply(State&) const
//{
//    static bool s_isTexEnvCombineSupported =
//        isGLExtensionSupported("GL_ARB_texture_env_combine");
//
//    static bool s_isTexEnvCrossbarSupported =
//        isGLExtensionSupported("GL_ARB_texture_env_crossbar");
//
//    static bool s_isNVTexEnvCrossbarSupported =
//        isGLExtensionSupported("GL_NV_texture_env_combine4");
//
//    static bool s_isTexEnvDot3Supported = 
//        isGLExtensionSupported("GL_ARB_texture_env_dot3");
//
//
//    bool needsTexEnvDot3 = (_combine_RGB==DOT3_RGB) ||
//                           (_combine_RGB==DOT3_RGBA);
//
//    bool supported = s_isTexEnvCombineSupported;
//    if (_needsTexEnvCrossbar && !(s_isTexEnvCrossbarSupported || s_isNVTexEnvCrossbarSupported)) supported = false;
//    if (needsTexEnvDot3 && !s_isTexEnvDot3Supported) supported = false;
//
//    if (supported)
//    {
//        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
//
//        glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, _combine_RGB);
//        
//        if (_combine_RGB!=DOT3_RGBA)
//            glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, _combine_Alpha);
//
//        glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB,_source0_RGB );
//        glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, _source1_RGB);
//        glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB,_source2_RGB );
//
//        glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, _source0_Alpha);
//        glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB, _source1_Alpha);
//        glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE2_ALPHA_ARB, _source2_Alpha);
//
//        glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, _operand0_RGB);
//        glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, _operand1_RGB);
//        glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, _operand2_RGB);
//
//        glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, _operand0_Alpha);
//        glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND1_ALPHA_ARB, _operand1_Alpha);
//        glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND2_ALPHA_ARB, _operand2_Alpha);
//
//        glTexEnvf( GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, _scale_RGB);
//        glTexEnvf( GL_TEXTURE_ENV, GL_ALPHA_SCALE, _scale_Alpha);
//
//        glTexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, _constantColor.ptr());
//    }
//    else
//    {
//        // what is the best fallback when the tex env combine is not supported??
//        // we will resort the settung the OpenGL default of GL_MODULATE.
//        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//    }
//}
void crTexEnvCombine::setCombine_RGB(CombineParam cm) { m_combine_RGB = cm; }
void crTexEnvCombine::setCombine_Alpha(CombineParam cm) { m_combine_Alpha = cm; }

void crTexEnvCombine::setSource0_RGB(SourceParam sp) { m_source0_RGB = sp; computeNeedoForTexEnvCombiners(); }
void crTexEnvCombine::setSource1_RGB(SourceParam sp) { m_source1_RGB = sp; computeNeedoForTexEnvCombiners(); }
void crTexEnvCombine::setSource2_RGB(SourceParam sp) { m_source2_RGB = sp; computeNeedoForTexEnvCombiners(); }

void crTexEnvCombine::setSource0_Alpha(SourceParam sp) { m_source0_Alpha = sp; computeNeedoForTexEnvCombiners(); }
void crTexEnvCombine::setSource1_Alpha(SourceParam sp) { m_source1_Alpha = sp; computeNeedoForTexEnvCombiners(); }
void crTexEnvCombine::setSource2_Alpha(SourceParam sp) { m_source2_Alpha = sp; computeNeedoForTexEnvCombiners(); }

void crTexEnvCombine::setOperand0_RGB(OperandParam op) { m_operand0_RGB = op; }
void crTexEnvCombine::setOperand1_RGB(OperandParam op) { m_operand1_RGB = op; }
void crTexEnvCombine::setOperand2_RGB(OperandParam op) { m_operand2_RGB = op; }

static crTexEnvCombine::OperandParam Valid_Operand_Alpha(crTexEnvCombine::OperandParam op, const char* functionName)
{
    if (op==crTexEnvCombine::SRC_ALPHA || op==crTexEnvCombine::ONE_MINUS_SRC_ALPHA) return op;

    notify(WARN)<<"Warning:: crTexEnvCombine::"<<functionName<<"("<<op<<") invalid parameter value,"<<std::endl<<
                  "          must be SRC_ALPHA or ONE_MINUS_SRC_ALPHA, resetting to SRC_ALPHA."<<std::endl;
    return crTexEnvCombine::SRC_ALPHA;
}

void crTexEnvCombine::setOperand0_Alpha(OperandParam op)
{
    m_operand0_Alpha = Valid_Operand_Alpha(op,"setOperand0_Alpha");
}
void crTexEnvCombine::setOperand1_Alpha(OperandParam op)
{
    m_operand1_Alpha = Valid_Operand_Alpha(op,"setOperand1_Alpha");
}
void crTexEnvCombine::setOperand2_Alpha(OperandParam op)
{ 
    m_operand2_Alpha = Valid_Operand_Alpha(op,"setOperand2_Alpha");
}

void crTexEnvCombine::setScale_RGB(float scale) { m_scale_RGB = scale; }
void crTexEnvCombine::setScale_Alpha(float scale) { m_scale_Alpha = scale; }

void crTexEnvCombine::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		m_needsTexEnvCrossbar = atoi(str.c_str());
		break;
	case 1:
		if(str.compare("REPLACE")==0)
			m_combine_RGB = REPLACE;
		else if(str.compare("MODULATE")==0)
			m_combine_RGB = MODULATE;
		else if(str.compare("ADD")==0)
			m_combine_RGB = ADD;
		else if(str.compare("ADD_SIGNED")==0)
			m_combine_RGB = ADD_SIGNED;
		else if(str.compare("INTERPOLATE")==0)
			m_combine_RGB = INTERPOLATE;
		else if(str.compare("SUBTRACT")==0)
			m_combine_RGB = SUBTRACT;
		else if(str.compare("DOT3_RGB")==0)
			m_combine_RGB = DOT3_RGB;
		else if(str.compare("DOT3_RGBA")==0)
			m_combine_RGB = DOT3_RGBA;
		break;
	case 2:
		if(str.compare("REPLACE")==0)
			m_combine_Alpha = REPLACE;
		else if(str.compare("MODULATE")==0)
			m_combine_Alpha = MODULATE;
		else if(str.compare("ADD")==0)
			m_combine_Alpha = ADD;
		else if(str.compare("ADD_SIGNED")==0)
			m_combine_Alpha = ADD_SIGNED;
		else if(str.compare("INTERPOLATE")==0)
			m_combine_Alpha = INTERPOLATE;
		else if(str.compare("SUBTRACT")==0)
			m_combine_Alpha = SUBTRACT;
		else if(str.compare("DOT3_RGB")==0)
			m_combine_Alpha = DOT3_RGB;
		else if(str.compare("DOT3_RGBA")==0)
			m_combine_Alpha = DOT3_RGBA;
		break;
	case 3:
		if(str.compare("CONSTANT")==0)
			m_source0_RGB = CONSTANT;
		else if(str.compare("PRIMARY_COLOR")==0)
			m_source0_RGB = PRIMARY_COLOR;
		else if(str.compare("PREVIOUS")==0)
			m_source0_RGB = PREVIOUS;
		else if(str.compare("TEXTURE")==0)
			m_source0_RGB = TEXTURE;
		else if(str.compare("TEXTURE0")==0)
			m_source0_RGB = TEXTURE0;
		else if(str.compare("TEXTURE1")==0)
			m_source0_RGB = TEXTURE1;
		else if(str.compare("TEXTURE2")==0)
			m_source0_RGB = TEXTURE2;
		else if(str.compare("TEXTURE3")==0)
			m_source0_RGB = TEXTURE3;
		else if(str.compare("TEXTURE4")==0)
			m_source0_RGB = TEXTURE4;
		else if(str.compare("TEXTURE5")==0)
			m_source0_RGB = TEXTURE5;
		else if(str.compare("TEXTURE6")==0)
			m_source0_RGB = TEXTURE6;
		else if(str.compare("TEXTURE7")==0)
			m_source0_RGB = TEXTURE7;
		break;
	case 4:
		if(str.compare("CONSTANT")==0)
			m_source1_RGB = CONSTANT;
		else if(str.compare("PRIMARY_COLOR")==0)
			m_source1_RGB = PRIMARY_COLOR;
		else if(str.compare("PREVIOUS")==0)
			m_source1_RGB = PREVIOUS;
		else if(str.compare("TEXTURE")==0)
			m_source1_RGB = TEXTURE;
		else if(str.compare("TEXTURE0")==0)
			m_source1_RGB = TEXTURE0;
		else if(str.compare("TEXTURE1")==0)
			m_source1_RGB = TEXTURE1;
		else if(str.compare("TEXTURE2")==0)
			m_source1_RGB = TEXTURE2;
		else if(str.compare("TEXTURE3")==0)
			m_source1_RGB = TEXTURE3;
		else if(str.compare("TEXTURE4")==0)
			m_source1_RGB = TEXTURE4;
		else if(str.compare("TEXTURE5")==0)
			m_source1_RGB = TEXTURE5;
		else if(str.compare("TEXTURE6")==0)
			m_source1_RGB = TEXTURE6;
		else if(str.compare("TEXTURE7")==0)
			m_source1_RGB = TEXTURE7;
		break;
	case 5:
		if(str.compare("CONSTANT")==0)
			m_source2_RGB = CONSTANT;
		else if(str.compare("PRIMARY_COLOR")==0)
			m_source2_RGB = PRIMARY_COLOR;
		else if(str.compare("PREVIOUS")==0)
			m_source2_RGB = PREVIOUS;
		else if(str.compare("TEXTURE")==0)
			m_source2_RGB = TEXTURE;
		else if(str.compare("TEXTURE0")==0)
			m_source2_RGB = TEXTURE0;
		else if(str.compare("TEXTURE1")==0)
			m_source2_RGB = TEXTURE1;
		else if(str.compare("TEXTURE2")==0)
			m_source2_RGB = TEXTURE2;
		else if(str.compare("TEXTURE3")==0)
			m_source2_RGB = TEXTURE3;
		else if(str.compare("TEXTURE4")==0)
			m_source2_RGB = TEXTURE4;
		else if(str.compare("TEXTURE5")==0)
			m_source2_RGB = TEXTURE5;
		else if(str.compare("TEXTURE6")==0)
			m_source2_RGB = TEXTURE6;
		else if(str.compare("TEXTURE7")==0)
			m_source2_RGB = TEXTURE7;
		break;
	case 6:
		if(str.compare("CONSTANT")==0)
			m_source0_Alpha = CONSTANT;
		else if(str.compare("PRIMARY_COLOR")==0)
			m_source0_Alpha = PRIMARY_COLOR;
		else if(str.compare("PREVIOUS")==0)
			m_source0_Alpha = PREVIOUS;
		else if(str.compare("TEXTURE")==0)
			m_source0_Alpha = TEXTURE;
		else if(str.compare("TEXTURE0")==0)
			m_source0_Alpha = TEXTURE0;
		else if(str.compare("TEXTURE1")==0)
			m_source0_Alpha = TEXTURE1;
		else if(str.compare("TEXTURE2")==0)
			m_source0_Alpha = TEXTURE2;
		else if(str.compare("TEXTURE3")==0)
			m_source0_Alpha = TEXTURE3;
		else if(str.compare("TEXTURE4")==0)
			m_source0_Alpha = TEXTURE4;
		else if(str.compare("TEXTURE5")==0)
			m_source0_Alpha = TEXTURE5;
		else if(str.compare("TEXTURE6")==0)
			m_source0_Alpha = TEXTURE6;
		else if(str.compare("TEXTURE7")==0)
			m_source0_Alpha = TEXTURE7;
		break;
	case 7:
		if(str.compare("CONSTANT")==0)
			m_source1_Alpha = CONSTANT;
		else if(str.compare("PRIMARY_COLOR")==0)
			m_source1_Alpha = PRIMARY_COLOR;
		else if(str.compare("PREVIOUS")==0)
			m_source1_Alpha = PREVIOUS;
		else if(str.compare("TEXTURE")==0)
			m_source1_Alpha = TEXTURE;
		else if(str.compare("TEXTURE0")==0)
			m_source1_Alpha = TEXTURE0;
		else if(str.compare("TEXTURE1")==0)
			m_source1_Alpha = TEXTURE1;
		else if(str.compare("TEXTURE2")==0)
			m_source1_Alpha = TEXTURE2;
		else if(str.compare("TEXTURE3")==0)
			m_source1_Alpha = TEXTURE3;
		else if(str.compare("TEXTURE4")==0)
			m_source1_Alpha = TEXTURE4;
		else if(str.compare("TEXTURE5")==0)
			m_source1_Alpha = TEXTURE5;
		else if(str.compare("TEXTURE6")==0)
			m_source1_Alpha = TEXTURE6;
		else if(str.compare("TEXTURE7")==0)
			m_source1_Alpha = TEXTURE7;
		break;
	case 8:
		if(str.compare("CONSTANT")==0)
			m_source2_Alpha = CONSTANT;
		else if(str.compare("PRIMARY_COLOR")==0)
			m_source2_Alpha = PRIMARY_COLOR;
		else if(str.compare("PREVIOUS")==0)
			m_source2_Alpha = PREVIOUS;
		else if(str.compare("TEXTURE")==0)
			m_source2_Alpha = TEXTURE;
		else if(str.compare("TEXTURE0")==0)
			m_source2_Alpha = TEXTURE0;
		else if(str.compare("TEXTURE1")==0)
			m_source2_Alpha = TEXTURE1;
		else if(str.compare("TEXTURE2")==0)
			m_source2_Alpha = TEXTURE2;
		else if(str.compare("TEXTURE3")==0)
			m_source2_Alpha = TEXTURE3;
		else if(str.compare("TEXTURE4")==0)
			m_source2_Alpha = TEXTURE4;
		else if(str.compare("TEXTURE5")==0)
			m_source2_Alpha = TEXTURE5;
		else if(str.compare("TEXTURE6")==0)
			m_source2_Alpha = TEXTURE6;
		else if(str.compare("TEXTURE7")==0)
			m_source2_Alpha = TEXTURE7;
		break;
	case 9:
		if(str.compare("SRC_COLOR")==0)
			m_operand0_RGB = SRC_COLOR;
		else if(str.compare("ONE_MINUS_SRC_COLOR")==0)
			m_operand0_RGB = ONE_MINUS_SRC_COLOR;
		else if(str.compare("SRC_ALPHA")==0)
			m_operand0_RGB = SRC_ALPHA;
		else if(str.compare("ONE_MINUS_SRC_ALPHA")==0)
			m_operand0_RGB = ONE_MINUS_SRC_ALPHA;
		break;
	case 10:
		if(str.compare("SRC_COLOR")==0)
			m_operand1_RGB = SRC_COLOR;
		else if(str.compare("ONE_MINUS_SRC_COLOR")==0)
			m_operand1_RGB = ONE_MINUS_SRC_COLOR;
		else if(str.compare("SRC_ALPHA")==0)
			m_operand1_RGB = SRC_ALPHA;
		else if(str.compare("ONE_MINUS_SRC_ALPHA")==0)
			m_operand1_RGB = ONE_MINUS_SRC_ALPHA;
		break;
	case 11:
		if(str.compare("SRC_COLOR")==0)
			m_operand2_RGB = SRC_COLOR;
		else if(str.compare("ONE_MINUS_SRC_COLOR")==0)
			m_operand2_RGB = ONE_MINUS_SRC_COLOR;
		else if(str.compare("SRC_ALPHA")==0)
			m_operand2_RGB = SRC_ALPHA;
		else if(str.compare("ONE_MINUS_SRC_ALPHA")==0)
			m_operand2_RGB = ONE_MINUS_SRC_ALPHA;
		break;
	case 12:
		if(str.compare("SRC_COLOR")==0)
			m_operand0_Alpha = SRC_COLOR;
		else if(str.compare("ONE_MINUS_SRC_COLOR")==0)
			m_operand0_Alpha = ONE_MINUS_SRC_COLOR;
		else if(str.compare("SRC_ALPHA")==0)
			m_operand0_Alpha = SRC_ALPHA;
		else if(str.compare("ONE_MINUS_SRC_ALPHA")==0)
			m_operand0_Alpha = ONE_MINUS_SRC_ALPHA;
		break;
	case 13:
		if(str.compare("SRC_COLOR")==0)
			m_operand1_Alpha = SRC_COLOR;
		else if(str.compare("ONE_MINUS_SRC_COLOR")==0)
			m_operand1_Alpha = ONE_MINUS_SRC_COLOR;
		else if(str.compare("SRC_ALPHA")==0)
			m_operand1_Alpha = SRC_ALPHA;
		else if(str.compare("ONE_MINUS_SRC_ALPHA")==0)
			m_operand1_Alpha = ONE_MINUS_SRC_ALPHA;
		break;
	case 14:
		if(str.compare("SRC_COLOR")==0)
			m_operand2_Alpha = SRC_COLOR;
		else if(str.compare("ONE_MINUS_SRC_COLOR")==0)
			m_operand2_Alpha = ONE_MINUS_SRC_COLOR;
		else if(str.compare("SRC_ALPHA")==0)
			m_operand2_Alpha = SRC_ALPHA;
		else if(str.compare("ONE_MINUS_SRC_ALPHA")==0)
			m_operand2_Alpha = ONE_MINUS_SRC_ALPHA;
		break;
	case 15:
		m_scale_RGB = atof(str.c_str());
		break;
	case 16:
		m_scale_Alpha = atof(str.c_str());
		break;
	case 17:
		crArgumentParser::appAtoVec(str,m_constantColor);
		break;
	}

	return crStateAttribute::addAttributeParam(i-18,str);
}
#include <Driver/GLDrv/crTexEnvCombineDrv.h>