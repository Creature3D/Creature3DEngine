/**********************************************************************
 *
 *    FILE:           shadow.glsl
 *
 *    DESCRIPTION:    
 *
 *    CREATED BY:     Îâ²Æ»ª
 *
 *    HISTORY:        Created 2006.5.20
 *
 *    Copyright 2005  Creature3D
 **********************************************************************/
 
{****Creature3D Vertex shader****}
uniform mat4 lightViewInverse;
varying vec3 lightVec;
#if defined (_AlphaShadow) || defined (_BlendShadow)
varying vec4 texCoord;
#endif
void main(void)
{
    gl_Position = ftransform();
    lightVec = lightViewInverse[3].xyz - (lightViewInverse * (gl_ModelViewMatrix * gl_Vertex)).xyz;
#if defined (_AlphaShadow) || defined (_BlendShadow)    
    texCoord.xy = (gl_TextureMatrix[0] * gl_MultiTexCoord0).xy;
	texCoord.zw = (gl_TextureMatrix[1] * gl_MultiTexCoord1).xy;
    gl_FrontColor = gl_Color;
#endif    
}

{****Creature3D Fragment shader****}
uniform float scale;
varying vec3 lightVec;

#if defined (_AlphaShadow) || defined (_BlendShadow)
varying vec4 texCoord;

#ifdef _BT
uniform sampler2D CRE_BaseMap;
#ifdef _BMa
uniform mat4 baseMatA;
#endif
#ifdef _Param1MultBTa
uniform float param1MultBTa;
#endif
#ifdef _Param1AddBTa
uniform float param1AddBTa;
#endif
#endif

#ifdef _DT
uniform sampler2D CRE_DiffuseMap;
#ifdef _DMa
uniform mat4 diffuseMatA;
#endif
#ifdef _Param1MultDTa
uniform float param1MultDTa;
#endif
#ifdef _Param1AddDTa
uniform float param1AddDTa;
#endif
#endif

#ifdef _DT2
uniform sampler2D CRE_DiffuseMap2;
#ifdef _DM2a
uniform mat4 diffuseMat2A;
#endif
#ifdef _Param1MultDT2a
uniform float param1MultDT2a;
#endif
#ifdef _Param1AddDT2a
uniform float param1AddDT2a;
#endif
#endif

#ifdef _UVS
uniform sampler2D CRE_UVScrambler;
#ifdef _UVSM
uniform mat4 UVSMat;
#endif
#ifdef _Param1MultUVS
uniform float param1MultUVS;
#endif
#ifdef _Param4MultUVS
uniform vec4 param4MultUVS;
#endif
#ifdef _Param4AddUVS
uniform vec4 param4AddUVS;
#endif
#endif
#endif

#ifdef _CW
uniform sampler2D CRE_ColorWeight;
#ifdef _CWM
uniform mat4 CWMat;
#endif
#ifdef _CWMR
uniform mat4 CWMatR;
#endif
#ifdef _CWMG
uniform mat4 CWMatG;
#endif
#ifdef _CWMB
uniform mat4 CWMatB;
#endif
#ifdef _Param1MultCW
uniform float param1MultCW;
#endif
#ifdef _Param4MultCW
uniform vec4 param4MultCW;
#endif
#ifdef _Param4AddCW
uniform vec4 param4AddCW;
#endif
#ifdef _Param3MultCWDa
uniform vec3 param3MultCWDa;
#endif
#endif


void main(void)
{
#if defined (_AlphaShadow) || defined (_BlendShadow)
    vec4 _texCoord = texCoord;
	vec2 _texCoord2;
#ifdef _UVS
#ifdef _UVSUV1
	_texCoord2 = _texCoord.zw;
#else
	_texCoord2 = _texCoord.xy;
#endif
    vec4 uvscrambler;
#ifdef _UVSM
    uvscrambler = texture2D(CRE_UVScrambler, (UVSMat * vec4(_texCoord2,0.0,1.0)).xy);
#else
    uvscrambler = texture2D(CRE_UVScrambler, _texCoord2);
#endif
#ifdef _Param4AddUVS
    uvscrambler += param4AddUVS;
#endif
#ifdef _Param1MultUVS
    uvscrambler *= param1MultUVS;
#endif
#ifdef _Param4MultUVS
    uvscrambler *= param4MultUVS;
#endif
    _texCoord.xy += uvscrambler.xy;
	_texCoord.zw += uvscrambler.xy;
#endif

#ifdef _CW
#ifdef _CWUV1
	_texCoord2 = _texCoord.zw;
#else
	_texCoord2 = _texCoord.xy;
#endif
    vec4 colorWeight;
#if !(defined (_CWMR) && defined (_CWMG) && defined (_CWMB))
#ifdef _CWM
    colorWeight = texture2D(CRE_ColorWeight, (CWMat * vec4(_texCoord2,0.0,1.0)).xy);
#else
    colorWeight = texture2D(CRE_ColorWeight, _texCoord2);
#endif
#endif
#ifdef _CWMR
    colorWeight.r = texture2D(CRE_ColorWeight, (CWMatR * vec4(_texCoord2,0.0,1.0)).xy).r;
#endif
#ifdef _CWMG
    colorWeight.g = texture2D(CRE_ColorWeight, (CWMatG * vec4(_texCoord2,0.0,1.0)).xy).g;
#endif
#ifdef _CWMB
    colorWeight.b = texture2D(CRE_ColorWeight, (CWMatB * vec4(_texCoord2,0.0,1.0)).xy).b;
#endif

#ifdef _Param4AddCW
    colorWeight += param4AddCW;
#endif
#ifdef _Param1MultCW
    colorWeight *= param1MultCW;
#endif
#ifdef _Param4MultCW
    colorWeight *= param4MultCW;
#endif
#endif

#endif

    //_texCoord = fract(_texCoord);
    float depth = length(lightVec) * scale;
    float depthEx = fract(depth* 64.0)*0.015625;// / 64.0;

    float alphaBlend = 1.0;
    float alphaTest = 1.0;
    
#if defined (_AlphaShadow) || defined (_BlendShadow)
    float alpha = 1.0;
    float tmpalpha = 0.0;

#ifdef _BT
#ifdef _BTUV1
	_texCoord2 = _texCoord.zw;
#else
	_texCoord2 = _texCoord.xy;
#endif
#if defined(_REPLACEBTa) || defined(_ADDBTa) || defined(_MULTBTa)
#ifdef _BMa
    tmpalpha = texture2D(CRE_BaseMap, (baseMatA * vec4(_texCoord2,0.0,1.0)).xy).w;
#else
    tmpalpha = texture2D(CRE_BaseMap, _texCoord2).w;
#endif
#ifdef _Param1AddBTa
    tmpalpha += param1AddBTa;
#endif
#ifdef _Param1MultBTa
    tmpalpha *= param1MultBTa;
#endif
#ifdef _CW
#ifdef _Param3MultCWDa
    //tmpalpha = param3MultCWDa.x == 0 ? tmpalpha : tmpalpha * (colorWeight.r * param3MultCWDa.x);
    tmpalpha *= colorWeight.r * param3MultCWDa.x;
#else
    tmpalpha *= colorWeight.r;
#endif
#endif
#ifdef _REPLACEBTa
    alpha = tmpalpha;
#elif defined (_ADDBTa)
    alpha += tmpalpha;
#elif defined (_MULTBTa)
    alpha *= tmpalpha;
#endif
#endif
#endif

#ifdef _DT
#ifdef _DTUV1
	_texCoord2 = _texCoord.zw;
#else
	_texCoord2 = _texCoord.xy;
#endif
#if defined(_REPLACEDTa) || defined(_ADDDTa) || defined(_MULTDTa)
#ifdef _DMa
    tmpalpha = texture2D(CRE_DiffuseMap, (diffuseMatA * vec4(_texCoord2,0.0,1.0)).xy).w;
#else
    tmpalpha = texture2D(CRE_DiffuseMap, _texCoord2).w;
#endif
#ifdef _Param1AddDTa
    tmpalpha += param1AddDTa;
#endif
#ifdef _Param1MultDTa
    tmpalpha *= param1MultDTa;
#endif
#ifdef _CW
#ifdef _Param3MultCWDa
    //tmpalpha = param3MultCWDa.y == 0 ? tmpalpha : tmpalpha * (colorWeight.g * param3MultCWDa.y);
    tmpalpha *= colorWeight.g * param3MultCWDa.y;
#else
    tmpalpha *= colorWeight.g;
#endif
#endif
#ifdef _REPLACEDTa
    alpha = tmpalpha;
#elif defined (_ADDDTa)
    alpha += tmpalpha;
#elif defined (_MULTDTa)
    alpha *= tmpalpha;
#endif
#endif
#endif

#ifdef _DT2
#ifdef _DT2UV1
	_texCoord2 = _texCoord.zw;
#else
	_texCoord2 = _texCoord.xy;
#endif
    tmpalpha = 0.0;
#if defined(_REPLACEDT2a) || defined(_ADDDT2a) || defined(_MULTDT2a)
#ifdef _DM2a
    tmpalpha = texture2D(CRE_DiffuseMap2, (diffuseMat2A * vec4(_texCoord2,0.0,1.0)).xy).w;
#else
    tmpalpha = texture2D(CRE_DiffuseMap2, _texCoord2).w;
#endif
#ifdef _Param1AddDT2a
    tmpalpha += param1AddDT2a;
#endif
#ifdef _Param1MultDT2a
    tmpalpha *= param1MultDT2a;
#endif
#ifdef _CW
#ifdef _Param3MultCWDa
    //tmpalpha = param3MultCWDa.z == 0 ? tmpalpha : tmpalpha * (colorWeight.b * param3MultCWDa.z);
    tmpalpha *= colorWeight.b * param3MultCWDa.z;
#else
    tmpalpha *= colorWeight.b;
#endif
#endif
#ifdef _REPLACEDT2a
    alpha = tmpalpha;
#elif defined (_ADDDT2a)
    alpha += tmpalpha;
#elif defined (_MULTDT2a)
    alpha *= tmpalpha;
#endif
#endif
#endif

    alpha *= gl_Color.a;
#ifdef _BlendShadow
    alphaBlend = alpha;
#endif
#ifdef _AlphaShadow
    alphaTest = alpha;
#endif
#endif
    gl_FragColor = vec4(1.0 - alphaBlend,depth - depthEx,depthEx,alphaTest);
}
{****end****}