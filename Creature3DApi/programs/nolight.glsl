/**********************************************************************
 *
 *    FILE:           noLight.glsl
 *
 *    DESCRIPTION:    
 *
 *    CREATED BY:     Îâ²Æ»ª
 *
 *    HISTORY:        Created 2007.2.26
 *
 *    Copyright 2007  Creature3D
 **********************************************************************/
{****Creature3D Vertex shader****}
uniform mat4 CRE_InverseViewMatrix;
varying vec3 vtxPos;
varying vec3 N;
varying vec4 texCoord;
void main(void)
{
    gl_FrontColor = gl_Color;
    gl_Position = ftransform();
    texCoord.xy = (gl_TextureMatrix[0] * gl_MultiTexCoord0).xy;
	texCoord.zw = (gl_TextureMatrix[1] * gl_MultiTexCoord1).xy;
    mat4 mat = CRE_InverseViewMatrix * gl_ModelViewMatrix;
    vec4 pos = mat * gl_Vertex;
    vtxPos = pos.xyz;
    mat3 normalMat = mat3(mat[0].xyz,mat[1].xyz,mat[2].xyz);
	N = normalize(normalMat * gl_Normal);
}

{****Creature3D Fragment shader****}
#if defined(_gi) || defined(_sgi) || defined(_hgi)
uniform vec4 giparam;
#endif
#if defined(_hgi) || defined(_gimap)
uniform float maxheight;
#endif
#ifdef _gi
uniform sampler2D CRE_GiMap;
uniform vec3 sunambient;
#endif
#ifndef _gimap
uniform vec3 UpperSkyColor;
uniform vec3 LowerSkyColor;
#endif

uniform vec3 lightPos;//sun
varying vec3 N;

uniform vec3 CRE_CameraPos;

varying vec3 vtxPos;

#ifdef NeedPixelDepth
    uniform float maxDepth;
#endif

#ifdef DepthFog
    uniform vec3 fog_color;
    uniform vec4 fog_params;
#endif

#ifdef _fi
    uniform vec2 fadeIn;
#endif
#ifdef _fo
    uniform vec2 fadeOut;
#endif

varying vec4 texCoord;

#if defined(_lm) || defined(_sgi) || defined(_hgi)
uniform sampler2D CRE_LightMap;
#if defined(_lm) && defined(_LM)
uniform mat4 lightMat;
#endif
#endif

#if defined(_lm2)
uniform sampler2D CRE_EmissiveMap3;
#if defined(_LM2)
uniform mat4 emissiveMat3;
#endif
#endif

#ifdef _env
uniform samplerCube CRE_EnvMap;
#ifdef _Param1MultENV
uniform float param1MultENV;
#endif
#ifdef _Param4MultENV
uniform vec4 param4MultENV;
#endif
#ifdef _Param4AddENV
uniform vec4 param4AddENV;
#endif
#endif

#ifdef _BT
uniform sampler2D CRE_BaseMap;
#ifdef _BM
uniform mat4 baseMat;
#endif
#ifdef _BMa
uniform mat4 baseMatA;
#endif
#ifdef _Param1MultBT
uniform float param1MultBT;
#endif
#ifdef _Param4MultBT
uniform vec4 param4MultBT;
#endif
#ifdef _Param4AddBT
uniform vec4 param4AddBT;
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
#ifdef _DM
uniform mat4 diffuseMat;
#endif
#ifdef _DMa
uniform mat4 diffuseMatA;
#endif
#ifdef _Param1MultDT
uniform float param1MultDT;
#endif
#ifdef _Param4MultDT
uniform vec4 param4MultDT;
#endif
#ifdef _Param4AddDT
uniform vec4 param4AddDT;
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
#ifdef _DM2
uniform mat4 diffuseMat2;
#endif
#ifdef _DM2a
uniform mat4 diffuseMat2A;
#endif
#ifdef _Param1MultDT2
uniform float param1MultDT2;
#endif
#ifdef _Param4MultDT2
uniform vec4 param4MultDT2;
#endif
#ifdef _Param4AddDT2
uniform vec4 param4AddDT2;
#endif

#ifdef _Param1MultDT2a
uniform float param1MultDT2a;
#endif
#ifdef _Param1AddDT2a
uniform float param1AddDT2a;
#endif
#endif

#ifdef _ET
uniform sampler2D CRE_EmissiveMap;
#ifdef _EM
uniform mat4 emissiveMat;
#endif
#ifdef _EMa
uniform mat4 emissiveMatA;
#endif
#ifdef _Param1MultET
uniform float param1MultET;
#endif
#ifdef _Param4MultET
uniform vec4 param4MultET;
#endif
#ifdef _Param4AddET
uniform vec4 param4AddET;
#endif

#ifdef _Param1MultETa
uniform float param1MultETa;
#endif
#ifdef _Param1AddETa
uniform float param1AddETa;
#endif
#endif

#ifdef _ET2
uniform sampler2D CRE_EmissiveMap2;
#ifdef _EM2
uniform mat4 emissiveMat2;
#endif
#ifdef _EM2a
uniform mat4 emissiveMat2A;
#endif
#ifdef _Param1MultET2
uniform float param1MultET2;
#endif
#ifdef _Param4MultET2
uniform vec4 param4MultET2;
#endif
#ifdef _Param4AddET2
uniform vec4 param4AddET2;
#endif

#ifdef _Param1MultET2a
uniform float param1MultET2a;
#endif
#ifdef _Param1AddET2a
uniform float param1AddET2a;
#endif
#endif

#ifdef _ET3
uniform sampler2D CRE_EmissiveMap3;
#ifdef _EM3
uniform mat4 emissiveMat3;
#endif
#ifdef _EM3a
uniform mat4 emissiveMat3A;
#endif
#ifdef _Param1MultET3
uniform float param1MultET3;
#endif
#ifdef _Param4MultET3
uniform vec4 param4MultET3;
#endif
#ifdef _Param4AddET3
uniform vec4 param4AddET3;
#endif

#ifdef _Param1MultET3a
uniform float param1MultET3a;
#endif
#ifdef _Param1AddET3a
uniform float param1AddET3a;
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
#ifdef _CWMA
uniform mat4 CWMatA;
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
#ifdef _Param3MultCWD
uniform vec3 param3MultCWD;
#endif
#ifdef _Param3MultCWE
uniform vec3 param3MultCWE;
#endif
#ifdef _Param3MultCWDa
uniform vec3 param3MultCWDa;
#endif
#ifdef _Param3MultCWEa
uniform vec3 param3MultCWEa;
#endif
#endif

uniform float Distortion;// = 0.0;

void main(void)
{
    vec4 _texCoord = texCoord;
#if defined(_gi) || defined(_sgi) || defined(_hgi)
    vec2 gicoord = (vtxPos.xy + giparam.xy) * giparam.zw;
#if defined(_gi) || defined(_sgi)
	vec2 _gicoord = gicoord;
#endif
#endif
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
#if defined(_gi) || defined(_sgi)
	_gicoord += uvscrambler.xy*0.2;
#endif
#endif

#ifdef _CW
#ifdef _CWUV1
	_texCoord2 = _texCoord.zw;
#else
	_texCoord2 = _texCoord.xy;
#endif
    vec4 colorWeight;
#if !(defined (_CWMR) && defined (_CWMG) && defined (_CWMB) && defined (_CWMA))
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
#ifdef _CWMA
    colorWeight.a = texture2D(CRE_ColorWeight, (CWMatA * vec4(_texCoord2,0.0,1.0)).xy).a;
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

    vec4 ambientColor = gl_FrontMaterial.ambient;

    vec4 diffuseColor = gl_FrontMaterial.diffuse;
    float alpha = 1.0;    
    float tmpalpha = 0.0;

#ifdef _BT
#ifdef _BTUV1
	_texCoord2 = _texCoord.zw;
#else
	_texCoord2 = _texCoord.xy;
#endif
    vec4 base;
#if defined(_REPLACEBT) || defined(_ADDBT) || defined(_MULTBT)
#ifdef _BM
    base = texture2D(CRE_BaseMap, (baseMat * vec4(_texCoord2,0.0,1.0)).xy);
#else
    base = texture2D(CRE_BaseMap, _texCoord2);
#if defined(_REPLACEBTa) || defined(_ADDBTa) || defined(_MULTBTa)
    tmpalpha = base.w;
#endif
#endif
#ifdef _Param4AddBT
    base += param4AddBT;
#endif
#ifdef _Param1MultBT
    base *= param1MultBT;
#endif
#ifdef _Param4MultBT
    base *= param4MultBT;
#endif
#ifdef _CW
#ifdef _Param3MultCWD
    base = param3MultCWD.x == 0.0 ? base : base * (colorWeight.r * param3MultCWD.x);
    //base *= colorWeight.r * param3MultCWD.x;
#else
    base *= colorWeight.r;
#endif
#endif

#ifdef _REPLACEBT
    diffuseColor = base;
#elif defined (_ADDBT)
    diffuseColor += base;
#elif defined (_MULTBT)
    diffuseColor *= base;
#endif
#endif
#if defined(_REPLACEBTa) || defined(_ADDBTa) || defined(_MULTBTa)
#ifdef _BMa
    tmpalpha = texture2D(CRE_BaseMap, (baseMatA * vec4(_texCoord2,0.0,1.0)).xy).w;
#elif defined(_BM) || !(defined(_REPLACEBT) || defined(_ADDBT) || defined(_MULTBT))
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
    tmpalpha = param3MultCWDa.x == 0.0 ? tmpalpha : tmpalpha * (colorWeight.r * param3MultCWDa.x);
    //tmpalpha *= colorWeight.r * param3MultCWDa.x;
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
    vec4 diffuseColor1;
    tmpalpha = 0.0;
#if defined(_REPLACEDT) || defined(_ADDDT) || defined(_MULTDT)
#ifdef _DM
    diffuseColor1 = texture2D(CRE_DiffuseMap, (diffuseMat * vec4(_texCoord2,0.0,1.0)).xy);
#else
    diffuseColor1 = texture2D(CRE_DiffuseMap, _texCoord2);
#if defined(_REPLACEDTa) || defined(_ADDDTa) || defined(_MULTDTa)
    tmpalpha = diffuseColor1.w;
#endif
#endif
#ifdef _Param4AddDT
    diffuseColor1 += param4AddDT;
#endif
#ifdef _Param1MultDT
    diffuseColor1 *= param1MultDT;
#endif
#ifdef _Param4MultDT
    diffuseColor1 *= param4MultDT;
#endif

#ifdef _CW
#ifdef _Param3MultCWD
    diffuseColor1 = param3MultCWD.y == 0.0 ? diffuseColor1 : diffuseColor1 * (colorWeight.g * param3MultCWD.y);
    //diffuseColor1 *= colorWeight.g * param3MultCWD.y;
#else
    diffuseColor1 *= colorWeight.g;
#endif
#endif

#ifdef _REPLACEDT
    diffuseColor = diffuseColor1;
#elif defined (_ADDDT)
    diffuseColor += diffuseColor1;
#elif defined (_MULTDT)
    diffuseColor *= diffuseColor1;
#endif
#endif
#if defined(_REPLACEDTa) || defined(_ADDDTa) || defined(_MULTDTa)
#ifdef _DMa
    tmpalpha = texture2D(CRE_DiffuseMap, (diffuseMatA * vec4(_texCoord2,0.0,1.0)).xy).w;
#elif defined(_DM) || !(defined(_REPLACEDT) || defined(_ADDDT) || defined(_MULTDT))
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
    tmpalpha = param3MultCWDa.y == 0.0 ? tmpalpha : tmpalpha * (colorWeight.g * param3MultCWDa.y);
    //tmpalpha *= colorWeight.g * param3MultCWDa.y;
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
    vec4 diffuseColor2;
    tmpalpha = 0.0;
#if defined(_REPLACEDT2) || defined(_ADDDT2) || defined(_MULTDT2)
#ifdef _DM2
    diffuseColor2 = texture2D(CRE_DiffuseMap2, (diffuseMat2 * vec4(_texCoord2,0.0,1.0)).xy);
#else
    diffuseColor2 = texture2D(CRE_DiffuseMap2, _texCoord2);
#if defined(_REPLACEDT2a) || defined(_ADDDT2a) || defined(_MULTDT2a)
    tmpalpha = diffuseColor2.w;
#endif
#endif
#ifdef _Param4AddDT2
    diffuseColor2 += param4AddDT2;
#endif
#ifdef _Param1MultDT2
    diffuseColor2 *= param1MultDT2;
#endif
#ifdef _Param4MultDT2
    diffuseColor2 *= param4MultDT2;
#endif
#ifdef _CW
#ifdef _Param3MultCWD
    diffuseColor2 = param3MultCWD.z == 0.0 ? diffuseColor2 : diffuseColor2 * (colorWeight.b * param3MultCWD.z);
    //diffuseColor2 *= colorWeight.b * param3MultCWD.z;
#else
    diffuseColor2 *= colorWeight.b;
#endif
#endif

#ifdef _REPLACEDT2
    diffuseColor = diffuseColor2;
#elif defined (_ADDDT2)
    diffuseColor += diffuseColor2;
#elif defined (_MULTDT2)
    diffuseColor *= diffuseColor2;
#endif
#endif
#if defined(_REPLACEDT2a) || defined(_ADDDT2a) || defined(_MULTDT2a)
#ifdef _DM2a
    tmpalpha = texture2D(CRE_DiffuseMap2, (diffuseMat2A * vec4(_texCoord2,0.0,1.0)).xy).w;
#elif defined(_DM2) || !(defined(_REPLACEDT2) || defined(_ADDDT2) || defined(_MULTDT2))
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
    tmpalpha = param3MultCWDa.z == 0.0 ? tmpalpha : tmpalpha * (colorWeight.b * param3MultCWDa.z);
    //tmpalpha *= colorWeight.b * param3MultCWDa.z;
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

    vec4 emissiveColor =gl_FrontMaterial.emission;
    float distortion = Distortion;
    
#ifdef _ET
#ifdef _ETUV1
	_texCoord2 = _texCoord.zw;
#else
	_texCoord2 = _texCoord.xy;
#endif
    vec4 emissiveColor1;
    tmpalpha = 0.0;
#if defined(_REPLACEET) || defined(_ADDET) || defined(_MULTET)
#ifdef _EM
    emissiveColor1 = texture2D(CRE_EmissiveMap, (emissiveMat * vec4(_texCoord2,0.0,1.0)).xy);
#else
    emissiveColor1 = texture2D(CRE_EmissiveMap, _texCoord2);
#if defined(_REPLACEETa) || defined(_ADDETa) || defined(_MULTETa)
    tmpalpha = emissiveColor1.w;
#endif
#endif
#ifdef _Param4AddET
    emissiveColor1 += param4AddET;
#endif
#ifdef _Param1MultET
    emissiveColor1 *= param1MultET;
#endif
#ifdef _Param4MultET
    emissiveColor1 *= param4MultET;
#endif
#ifdef _CW
#ifdef _Param3MultCWE
    emissiveColor1 = param3MultCWE.x == 0.0 ? emissiveColor1 : emissiveColor1 * (colorWeight.r * param3MultCWE.x);
    //emissiveColor1 *= colorWeight.r * param3MultCWE.x;
#else
    emissiveColor1 *= colorWeight.r;
#endif
#endif

#ifdef _REPLACEET
    emissiveColor = emissiveColor1;
#elif defined (_ADDET)
    emissiveColor += emissiveColor1;
#elif defined (_MULTET)
    emissiveColor *= emissiveColor1;
#endif

#endif
#if defined(_REPLACEETa) || defined(_ADDETa) || defined(_MULTETa)
#ifdef _EMa
    tmpalpha = texture2D(CRE_EmissiveMap, (emissiveMatA * vec4(_texCoord2,0.0,1.0)).xy).w;
#elif defined(_EM) || !(defined(_REPLACEET) || defined(_ADDET) || defined(_MULTET))
    tmpalpha = texture2D(CRE_EmissiveMap, _texCoord2).w;    
#endif
#ifdef _Param1AddETa
    tmpalpha += param1AddETa;
#endif
#ifdef _Param1MultETa
    tmpalpha *= param1MultETa;
#endif
#ifdef _CW
#ifdef _Param3MultCWEa
    tmpalpha = param3MultCWEa.x == 0.0 ? tmpalpha : tmpalpha * (colorWeight.r * param3MultCWEa.x);
    //tmpalpha *= colorWeight.r * param3MultCWEa.x;
#else
    tmpalpha *= colorWeight.r;
#endif
#endif
#ifdef _REPLACEETa
    distortion = tmpalpha;
#elif defined (_ADDETa)
    distortion += tmpalpha;
#elif defined (_MULTETa)
    distortion *= tmpalpha;
#endif
#endif
#endif

#ifdef _ET2
#ifdef _ET2UV1
	_texCoord2 = _texCoord.zw;
#else
	_texCoord2 = _texCoord.xy;
#endif
    vec4 emissiveColor2;
    tmpalpha = 0.0;
#if defined(_REPLACEET2) || defined(_ADDET2) || defined(_MULTET2) || defined(_ADDET2TOD) || defined(_MULTET2TOD)
#ifdef _EM2
    emissiveColor2 = texture2D(CRE_EmissiveMap2, (emissiveMat2 * vec4(_texCoord2,0.0,1.0)).xy);
#else
    emissiveColor2 = texture2D(CRE_EmissiveMap2, _texCoord2);
#if defined(_REPLACEET2a) || defined(_ADDET2a) || defined(_MULTET2a)
    tmpalpha = emissiveColor2.w;
#endif
#endif
#ifdef _Param4AddET2
    emissiveColor2 += param4AddET2;
#endif
#ifdef _Param1MultET2
    emissiveColor2 *= param1MultET2;
#endif
#ifdef _Param4MultET2
    emissiveColor2 *= param4MultET2;
#endif
#ifdef _CW
#if  defined(_ADDET2TOD) || defined(_MULTET2TOD)
#ifdef _Param3MultCWE
    emissiveColor2 = param3MultCWE.y == 0.0 ? emissiveColor2 : emissiveColor2 * (colorWeight.a * param3MultCWE.y);
    //emissiveColor2 *= colorWeight.a * param3MultCWE.y;
#else
    emissiveColor2 *= colorWeight.a;
#endif
#else
#ifdef _Param3MultCWE
    emissiveColor2 = param3MultCWE.y == 0.0 ? emissiveColor2 : emissiveColor2 * (colorWeight.g * param3MultCWE.y);
    //emissiveColor2 *= colorWeight.g * param3MultCWE.y;
#else
    emissiveColor2 *= colorWeight.g;
#endif
#endif
#endif

#ifdef _REPLACEET2
    emissiveColor = emissiveColor2;
#elif defined (_ADDET2)
    emissiveColor += emissiveColor2;
#elif defined (_MULTET2)
    emissiveColor *= emissiveColor2;
#elif defined (_ADDET2TOD)
    diffuseColor += emissiveColor2;
#elif defined (_MULTET2TOD)
    diffuseColor *= emissiveColor2;
#endif

#endif
#if defined(_REPLACEET2a) || defined(_ADDET2a) || defined(_MULTET2a)
#ifdef _EM2a
    tmpalpha = texture2D(CRE_EmissiveMap2, (emissiveMat2A * vec4(_texCoord2,0.0,1.0)).xy).w;
#elif defined(_EM2) || !(defined(_REPLACEET2) || defined(_ADDET2) || defined(_MULTET2) || defined(_ADDET2TOD) || defined(_MULTET2TOD))
    tmpalpha = texture2D(CRE_EmissiveMap2, _texCoord2).w;    
#endif
#ifdef _Param1AddET2a
    tmpalpha += param1AddET2a;
#endif
#ifdef _Param1MultET2a
    tmpalpha *= param1MultET2a;
#endif
#ifdef _CW
#ifdef _Param3MultCWEa
    tmpalpha = param3MultCWEa.y == 0.0 ? tmpalpha : tmpalpha * (colorWeight.g * param3MultCWEa.y);
    //tmpalpha *= colorWeight.g * param3MultCWEa.y;
#else
    tmpalpha *= colorWeight.g;
#endif
#endif
#ifdef _REPLACEET2a
    distortion = tmpalpha;
#elif defined (_ADDET2a)
    distortion += tmpalpha;
#elif defined (_MULTET2a)
    distortion *= tmpalpha;
#endif
#endif
#endif

#ifdef _ET3
#ifdef _ET3UV1
	_texCoord2 = _texCoord.zw;
#else
	_texCoord2 = _texCoord.xy;
#endif
    vec4 emissiveColor3;
    tmpalpha = 0.0;
#if defined(_REPLACEET3) || defined(_ADDET3) || defined(_MULTET3) || defined(_ADDET3TOS) || defined(_MULTET3TOS)
#ifdef _EM3
    emissiveColor3 = texture2D(CRE_EmissiveMap3, (emissiveMat3 * vec4(_texCoord2,0.0,1.0)).xy);
#else
    emissiveColor3 = texture2D(CRE_EmissiveMap3, _texCoord2);
#if defined(_REPLACEET3a) || defined(_ADDET3a) || defined(_MULTET3a)
    tmpalpha = emissiveColor3.w;
#endif
#endif
#ifdef _Param4AddET3
    emissiveColor3 += param4AddET3;
#endif
#ifdef _Param1MultET3
    emissiveColor3 *= param1MultET3;
#endif
#ifdef _Param4MultET3
    emissiveColor3 *= param4MultET3;
#endif
#ifdef _CW
#if  defined(_ADDET3TOS) || defined(_MULTET3TOS)
#ifdef _Param3MultCWE
    emissiveColor3 = param3MultCWE.z == 0.0 ? emissiveColor3 : emissiveColor3 * (colorWeight.a * param3MultCWE.z);
    //emissiveColor3 *= colorWeight.a * param3MultCWE.z;
#else
    emissiveColor3 *= colorWeight.a;
#endif
#else
#ifdef _Param3MultCWE
    emissiveColor3 = param3MultCWE.z == 0.0 ? emissiveColor3 : emissiveColor3 * (colorWeight.b * param3MultCWE.z);
    //emissiveColor3 *= colorWeight.b * param3MultCWE.z;
#else
    emissiveColor3 *= colorWeight.b;
#endif
#endif
#endif
#ifdef _REPLACEET3
    emissiveColor = emissiveColor3;
#elif defined (_ADDET3)
    emissiveColor += emissiveColor3;
#elif defined (_MULTET3)
    emissiveColor *= emissiveColor3;
#elif defined (_ADDET3TOS)
    specularColor += emissiveColor3;
#elif defined (_MULTET3TOS)
    specularColor *= emissiveColor3;
#endif
#endif
#if defined(_REPLACEET3a) || defined(_ADDET3a) || defined(_MULTET3a)
#ifdef _EM3a
    tmpalpha = texture2D(CRE_EmissiveMap3, (emissiveMat3A * vec4(_texCoord2,0.0,1.0)).xy).w;
#elif defined(_EM3) || !(defined(_REPLACEET3) || defined(_ADDET3) || defined(_MULTET3) || defined(_ADDET3TOS) || defined(_MULTET3TOS))
    tmpalpha = texture2D(CRE_EmissiveMap3, _texCoord2).w;    
#endif
#ifdef _Param1AddET3a
    tmpalpha += param1AddET3a;
#endif
#ifdef _Param1MultET3a
    tmpalpha *= param1MultET3a;
#endif
#ifdef _CW
#ifdef _Param3MultCWEa
    tmpalpha = param3MultCWEa.z == 0.0 ? tmpalpha : tmpalpha * (colorWeight.b * param3MultCWEa.z);
    //tmpalpha *= colorWeight.b * param3MultCWEa.z;
#else
    tmpalpha *= colorWeight.b;
#endif
#endif
#ifdef _REPLACEET3a
    distortion = tmpalpha;
#elif defined (_ADDET3a)
    distortion += tmpalpha;
#elif defined (_MULTET3a)
    distortion *= tmpalpha;
#endif
#endif
#endif

  	vec4 color = emissiveColor + ambientColor * diffuseColor + diffuseColor;
  	
#ifdef _env	
	//base = mix(base, textureCube(CRE_EnvMap, reflVec), refParam);
	vec4 env = textureCube(CRE_EnvMap, reflVec);
#ifdef _Param4AddENV
    env += param4AddENV;
#endif
#ifdef _Param1MultENV
    env *= param1MultENV;
#endif
#ifdef _Param4MultENV
    env *= param4MultENV;
#endif
    color += env;
#endif

  	color.a = alpha;
    color *= gl_Color;
#if defined(_lm) || defined(_sgi) || defined(_hgi)
#ifdef _sgi
	color.xyz *= texture2D(CRE_LightMap,_gicoord).xyz;
#elif defined (_hgi)
	if(color.a<1.0)
	{
		vec4 hcolor = texture2D(CRE_LightMap,gicoord);
		float height = (hcolor.y * 2.0 - 1.0) * maxheight;
		float delta_fragz = vtxPos.z - height;
		if(delta_fragz>-1.0 && delta_fragz<=0.0)
			color.a = 0.0;
		else if(delta_fragz>0.0 && delta_fragz<10.0)
			color.a *= pow(delta_fragz * 0.1,0.5);
	}
#elif defined (_LM)
    color.xyz *= texture2D(CRE_LightMap, (lightMat * vec4(_texCoord.zw,0.0,1.0)).xy).xyz;
#else
    color.xyz *= texture2D(CRE_LightMap,_texCoord.zw).xyz;
#endif
#endif
#if defined(_lm2)
#ifdef _LM2
    color.xyz *= texture2D(CRE_EmissiveMap3, (emissiveMat3 * vec4(_texCoord.zw,0.0,1.0)).xy).xyz;
#else
    color.xyz *= texture2D(CRE_EmissiveMap3,_texCoord.zw).xyz;
#endif
#endif

#ifndef _NoSkyLight
#ifndef _gimap
	//HemisphereLightPhong(bump);
	vec3 sunlVec = normalize(lightPos - vtxPos);
    float NdotL = dot(N,sunlVec);
	float influence = NdotL * 0.5 + 0.5;
	vec3 skyLight = mix( LowerSkyColor, UpperSkyColor, influence );
#ifdef _gi
	skyLight += texture2D(CRE_GiMap,_gicoord).xyz*sunambient;
#endif
	color.xyz += (gl_LightModel.ambient.xyz + skyLight) * diffuseColor.xyz;
#endif
#endif

	vec3 tempVec;
    tempVec = CRE_CameraPos - vtxPos;
	float sqrDepth = dot(tempVec,tempVec);
#ifdef NeedPixelDepth
    float depth = min(sqrt(sqrDepth) / maxDepth, 1.0);
    sqrDepth = depth * depth;
#ifdef DepthFog
	float fogheight = vtxPos.z - fog_params.w;   
    if(fogheight<100.0 && sqrDepth >= fog_params.x && sqrDepth <= fog_params.y)
    {
	    float f = exp(-fog_params.z * (sqrDepth-fog_params.x));
	    if(fogheight>0)
	    {
			f = mix(f,1.0,fogheight*0.01);
	    }
	    color.xyz = mix(fog_color,color.xyz,f);
	}
#endif
#ifdef _fi
    if(sqrDepth >= fadeIn.x && sqrDepth <= fadeIn.y)
        color.a *= pow((sqrDepth - fadeIn.x) / (fadeIn.y - fadeIn.x),2.0);        
#endif
#ifdef _fo
    if(sqrDepth >= fadeOut.x && sqrDepth <= fadeOut.y)
        color.a *= pow((fadeOut.y - sqrDepth) / (fadeOut.y-fadeOut.x),2.0);     
#endif
	//if(color.a<1.0)
	//{
	//	float delta_fragz = gl_FragCoord.z / gl_FragCoord.w - depth;
	//	if(delta_fragz>=0.0 && delta_fragz<10.0)
	//		color.a *= delta_fragz * 0.1;
	//}
#endif

#ifdef _gimap
	float h = (vtxPos.z/maxheight)*0.5f + 0.5f;
	gl_FragColor = vec4(color.xyz,h);
#else        
#ifdef HDR
    gl_FragData[0] = min(color, 1.0);
    gl_FragData[1] = max(color - 1.0, 0.0);
    gl_FragData[1].w = distortion * 0.01;
#ifdef NeedNormalDepth
    gl_FragData[2] = vec4(0.0,0.0,1.0,depth);
#endif
#else
	gl_FragColor = color;
#endif
#endif
}
{****end****}