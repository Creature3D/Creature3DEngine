/**********************************************************************
 *
 *    FILE:           rtplm.glsl
 *
 *    DESCRIPTION:    用于室内多光源渲染,Blend叠加实现复杂的多光源效果，不计阴影，最多同时处理5个光源, alpha是否可以不用计算
 *
 *    CREATED BY:     吴财华
 *
 *    HISTORY:        Created 2007.2.26
 *
 *    Copyright 2007  Creature3D
 **********************************************************************/
{****Creature3D Vertex shader****}
uniform mat4 CRE_InverseViewMatrix;

attribute vec3 tangent;
//attribute vec3 binormal;

varying vec3 vtxPos;
varying vec3 vVec;
varying vec3 T;
varying vec3 B;
varying vec3 N;
varying vec4 texCoord;

void main(void)
{
    gl_FrontColor = gl_Color;
    texCoord.xy = (gl_TextureMatrix[0] * gl_MultiTexCoord0).xy;
    texCoord.zw = (gl_TextureMatrix[1] * gl_MultiTexCoord1).xy;
    mat4 mat = CRE_InverseViewMatrix * gl_ModelViewMatrix;
    vec4 pos = mat * gl_Vertex;
    vtxPos = pos.xyz;

    mat3 normalMat = mat3(mat[0].xyz,mat[1].xyz,mat[2].xyz);
    T = normalize(normalMat * tangent);
    //B = normalize(normalMat * binormal);
    N = normalize(normalMat * gl_Normal);
	B = cross(N,T);
        
    vec3 temp = CRE_InverseViewMatrix[3].xyz - pos.xyz;
    vVec = vec3(dot(temp,  T), dot(temp,  B), dot(temp,  N));

    gl_Position = ftransform();
}

{****Creature3D Fragment shader****}
#if defined(_sgi)
uniform vec4 giparam;
#endif

uniform vec3 CRE_CameraPos;

#if defined(direct1)
uniform vec3 lightPosd1;
uniform mat4 lightParamd1;//LA,LD,LS,lightAttenuation
uniform float loweastLumd1;
#endif

#if defined(direct2)
uniform vec3 lightPosd1;
uniform mat4 lightParamd1;
uniform float loweastLumd1;

uniform vec3 lightPosd2;
uniform mat4 lightParamd2;
uniform float loweastLumd2;
#endif

#if defined(direct3)
uniform vec3 lightPosd1;
uniform mat4 lightParamd1;
uniform float loweastLumd1;

uniform vec3 lightPosd2;
uniform mat4 lightParamd2;
uniform float loweastLumd2;

uniform vec3 lightPosd3;
uniform mat4 lightParamd3;
uniform float loweastLumd3;
#endif

#if defined(direct4)
uniform vec3 lightPosd1;
uniform mat4 lightParamd1;
uniform float loweastLumd1;

uniform vec3 lightPosd2;
uniform mat4 lightParamd2;
uniform float loweastLumd2;

uniform vec3 lightPosd3;
uniform mat4 lightParamd3;
uniform float loweastLumd3;

uniform vec3 lightPosd4;
uniform mat4 lightParamd4;
uniform float loweastLumd4;
#endif

#if defined(direct5)
uniform vec3 lightPosd1;
uniform mat4 lightParamd1;
uniform float loweastLumd1;

uniform vec3 lightPosd2;
uniform mat4 lightParamd2;
uniform float loweastLumd2;

uniform vec3 lightPosd3;
uniform mat4 lightParamd3;
uniform float loweastLumd3;

uniform vec3 lightPosd4;
uniform mat4 lightParamd4;
uniform float loweastLumd4;

uniform vec3 lightPosd5;
uniform mat4 lightParamd5;
uniform float loweastLumd5;
#endif

#if defined(spot1)
uniform vec3 lightPoss1;
uniform mat4 lightParams1;
uniform vec3 lightDir1;
uniform float cosLightFov1;
uniform float loweastLums1;
#endif

#if defined(spot2)
uniform vec3 lightPoss1;
uniform mat4 lightParams1;
uniform vec3 lightDir1;
uniform float cosLightFov1;
uniform float loweastLums1;

uniform vec3 lightPoss2;
uniform mat4 lightParams2;
uniform vec3 lightDir2;
uniform float cosLightFov2;
uniform float loweastLums2;
#endif

#if defined(spot3)
uniform vec3 lightPoss1;
uniform mat4 lightParams1;
uniform vec3 lightDir1;
uniform float cosLightFov1;
uniform float loweastLums1;

uniform vec3 lightPoss2;
uniform mat4 lightParams2;
uniform vec3 lightDir2;
uniform float cosLightFov2;
uniform float loweastLums2;

uniform vec3 lightPoss3;
uniform mat4 lightParams3;
uniform vec3 lightDir3;
uniform float cosLightFov3;
uniform float loweastLums3;
#endif

#if defined(spot4)
uniform vec3 lightPoss1;
uniform mat4 lightParams1;
uniform vec3 lightDir1;
uniform float cosLightFov1;
uniform float loweastLums1;

uniform vec3 lightPoss2;
uniform mat4 lightParams2;
uniform vec3 lightDir2;
uniform float cosLightFov2;
uniform float loweastLums2;

uniform vec3 lightPoss3;
uniform mat4 lightParams3;
uniform vec3 lightDir3;
uniform float cosLightFov3;
uniform float loweastLums3;

uniform vec3 lightPoss4;
uniform mat4 lightParams4;
uniform vec3 lightDir4;
uniform float cosLightFov4;
uniform float loweastLums4;
#endif

#if defined(spot5)
uniform vec3 lightPoss1;
uniform mat4 lightParams1;
uniform vec3 lightDir1;
uniform float cosLightFov1;
uniform float loweastLums1;

uniform vec3 lightPoss2;
uniform mat4 lightParams2;
uniform vec3 lightDir2;
uniform float cosLightFov2;
uniform float loweastLums2;

uniform vec3 lightPoss3;
uniform mat4 lightParams3;
uniform vec3 lightDir3;
uniform float cosLightFov3;
uniform float loweastLums3;

uniform vec3 lightPoss4;
uniform mat4 lightParams4;
uniform vec3 lightDir4;
uniform float cosLightFov4;
uniform float loweastLums4;

uniform vec3 lightPoss5;
uniform mat4 lightParams5;
uniform vec3 lightDir5;
uniform float cosLightFov5;
uniform float loweastLums5;
#endif

varying vec3 vtxPos;
varying vec3 vVec;
varying vec3 T;
varying vec3 B;
varying vec3 N;

varying vec4 texCoord;

#if defined(_lm) || defined(_sgi)
uniform sampler2D CRE_LightMap;
#ifdef _LM
uniform mat4 lightMat;
#endif
#endif

#if defined(_lm2)
uniform sampler2D CRE_EmissiveMap3;
#if defined(_LM2)
uniform mat4 emissiveMat3;
#endif
#endif

#if defined(_NT) || defined(_PT)
uniform sampler2D CRE_BumpMap;
#ifdef _PT
uniform float parallax;
#endif
#ifdef _NM
uniform mat4 normalMat;
#endif
#ifdef _Param1MultNT
uniform float param1MultNT;
#endif
#ifdef _Param3MultNT
uniform vec3 param3MultNT;
#endif
#ifdef _Param3AddNT
uniform vec3 param3AddNT;
#endif
#endif

#ifdef _NT2
uniform sampler2D CRE_NormalMap2;
#ifdef _NM2
uniform mat4 normalMat2;
#endif
#ifdef _Param1MultNT2
uniform float param1MultNT2;
#endif
#ifdef _Param3MultNT2
uniform vec3 param3MultNT2;
#endif
#ifdef _Param3AddNT2
uniform vec3 param3AddNT2;
#endif
#endif

#ifdef _NT3
uniform sampler2D CRE_NormalMap3;
#ifdef _NM3
uniform mat4 normalMat3;
#endif
#ifdef _Param1MultNT3
uniform float param1MultNT3;
#endif
#ifdef _Param3MultNT3
uniform vec3 param3MultNT3;
#endif
#ifdef _Param3AddNT3
uniform vec3 param3AddNT3;
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

#ifdef _ST
uniform sampler2D CRE_SpecularMap;
#ifdef _SM
uniform mat4 specularMat;
#endif
#ifdef _SMa
uniform mat4 specularMatA;
#endif
#ifdef _Param1MultST
uniform float param1MultST;
#endif
#ifdef _Param4MultST
uniform vec4 param4MultST;
#endif
#ifdef _Param4AddST
uniform vec4 param4AddST;
#endif

#ifdef _Param1MultSTa
uniform float param1MultSTa;
#endif
#ifdef _Param1AddSTa
uniform float param1AddSTa;
#endif
#endif

#ifdef _ST2
uniform sampler2D CRE_SpecularMap2;
#ifdef _SM2
uniform mat4 specularMat2;
#endif
#ifdef _SM2a
uniform mat4 specularMat2A;
#endif
#ifdef _Param1MultST2
uniform float param1MultST2;
#endif
#ifdef _Param4MultST2
uniform vec4 param4MultST2;
#endif
#ifdef _Param4AddST2
uniform vec4 param4AddST2;
#endif

#ifdef _Param1MultST2a
uniform float param1MultST2a;
#endif
#ifdef _Param1AddST2a
uniform float param1AddST2a;
#endif
#endif

#ifdef _ST3
uniform sampler2D CRE_SpecularMap3;
#ifdef _SM3
uniform mat4 specularMat3;
#endif
#ifdef _SM3a
uniform mat4 specularMat3A;
#endif
#ifdef _Param1MultST3
uniform float param1MultST3;
#endif
#ifdef _Param4MultST3
uniform vec4 param4MultST3;
#endif
#ifdef _Param4AddST3
uniform vec4 param4AddST3;
#endif

#ifdef _Param1MultST3a
uniform float param1MultST3a;
#endif
#ifdef _Param1AddST3a
uniform float param1AddST3a;
#endif
#endif

#ifdef _ET
uniform sampler2D CRE_EmissiveMap;
#ifdef _EM
uniform mat4 emissiveMat;
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
#endif

#ifdef _ET2
uniform sampler2D CRE_EmissiveMap2;
#ifdef _EM2
uniform mat4 emissiveMat2;
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
#endif

#ifdef _ET3
uniform sampler2D CRE_EmissiveMap3;
#ifdef _EM3
uniform mat4 emissiveMat3;
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
#ifdef _Param3MultCWN
uniform vec3 param3MultCWN;
#endif
#ifdef _Param3MultCWD
uniform vec3 param3MultCWD;
#endif
#ifdef _Param3MultCWS
uniform vec3 param3MultCWS;
#endif
#ifdef _Param3MultCWE
uniform vec3 param3MultCWE;
#endif
#ifdef _Param3MultCWDa
uniform vec3 param3MultCWDa;
#endif
#ifdef _Param3MultCWSa
uniform vec3 param3MultCWSa;
#endif
#endif

#define saturate(x) clamp(x,0.0,1.0)
#ifdef NV
#define _pow(a,b) (b==0.0?1.0:pow(a,b))
//#define _sqrt sqrt
#elif defined(ATI)
#define _pow pow
//#define _sqrt(x) (x==0.0?0.0:sqrt(x))
#else
#define _pow(a,b) (b==0.0?1.0:pow(a,b))
//#define _sqrt(x) (x==0.0?0.0:sqrt(x))
#endif

void main(void)
{
    vec4 _texCoord = texCoord;
#if defined(_sgi)
	vec2 _gicoord = (vtxPos.xy + giparam.xy) * giparam.zw;
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
#if defined(_sgi)
	_gicoord += uvscrambler.xy;
#endif

#endif

	vec3 n_vVec  = normalize(vVec);
	vec3 bump = vec3(0.0,0.0,1.0);
#if defined(_NT) || defined(_PT)
#if defined(_PT)
#ifdef _NM
	float height = texture2D(CRE_BumpMap, (normalMat * vec4(_texCoord.xy,0.0,1.0)).xy).r;
#else
	float height = texture2D(CRE_BumpMap, _texCoord.xy).r;
#endif
	float offset = parallax * (2.0 * height - 1.0);
	vec2 parallaxTexCoord = _texCoord.xy + offset * n_vVec.xy;
	
	height += texture2D(CRE_BumpMap, parallaxTexCoord).r;
	offset =  parallax * (height - 1.0);
	parallaxTexCoord = _texCoord.xy + offset * n_vVec.xy;
	
	_texCoord.xy = parallaxTexCoord;
#endif
#ifdef _NM
    bump.xy	= (texture2D(CRE_BumpMap, (normalMat * vec4(_texCoord.xy,0.0,1.0)).xy).ag * 2.0) - 1.0;
#else
    bump.xy	= (texture2D(CRE_BumpMap, _texCoord.xy).ag * 2.0) - 1.0;
#endif 
    //bump.z = sqrt(1.0 - dot(bump.xy, bump.xy));
#ifdef _Param3AddNT
    bump += param3AddNT;
#endif
#ifdef _Param1MultNT
    bump *= param1MultNT;
#endif
#ifdef _Param3MultNT
    bump *= param3MultNT;
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

#if defined(_NT) || defined(_PT)
#ifdef _CW
#ifdef _Param3MultCWN
    bump = param3MultCWN.x == 0.0 ? bump : bump * (colorWeight.r * param3MultCWN.x);
    //bump *= colorWeight.r * param3MultCWN.x;
#else
    bump *= colorWeight.r;
#endif
#endif
#endif

#ifdef _NT2
#ifdef _NT2UV1
	_texCoord2 = _texCoord.zw;
#else
	_texCoord2 = _texCoord.xy;
#endif
#if defined(_REPLACENT2) || defined(_ADDNT2) || defined(_MULTNT2)
    vec3 nt2;
#ifdef _NM2
    nt2.xy	= (texture2D(CRE_NormalMap2, (normalMat2 * vec4(_texCoord2,0.0,1.0)).xy).ag * 2.0) - 1.0;
#else
    nt2.xy	= (texture2D(CRE_NormalMap2, _texCoord2).ag * 2.0) - 1.0;
#endif
    //nt2.z = sqrt(1.0 - dot(nt2.xy, nt2.xy));
#ifdef _Param3AddNT2
    nt2 += param3AddNT2;
#endif
#ifdef _Param1MultNT2
    nt2 *= param1MultNT2;
#endif
#ifdef _Param3MultNT2
    nt2 *= param3MultNT2;
#endif

#ifdef _CW
#ifdef _Param3MultCWN
    nt2 = param3MultCWN.y == 0.0 ? nt2 : nt2 * (colorWeight.g * param3MultCWN.y);
    //nt2 *= colorWeight.g * param3MultCWN.y;
#else
    nt2 *= colorWeight.g;
#endif
#endif

#ifdef _REPLACENT2
    bump = nt2;
#elif defined (_ADDNT2)
    bump += nt2;
#elif defined (_MULTNT2)
    bump *= nt2;
#endif
#endif
#endif

#ifdef _NT3
#ifdef _NT3UV1
	_texCoord2 = _texCoord.zw;
#else
	_texCoord2 = _texCoord.xy;
#endif
#if defined(_REPLACENT3) || defined(_ADDNT3) || defined(_MULTNT3)
    vec3 nt3;
#ifdef _NM3
    nt3.xy	= (texture2D(CRE_NormalMap3, (normalMat3 * vec4(_texCoord2,0.0,1.0)).xy).ag * 2.0) - 1.0;
#else
    nt3.xy	= (texture2D(CRE_NormalMap3, _texCoord2).ag * 2.0) - 1.0;
#endif
    //nt3.z = sqrt(1.0 - dot(nt3.xy, nt3.xy));
#ifdef _Param3AddNT3
    nt3 += param3AddNT3;
#endif
#ifdef _Param1MultNT3
    nt3 *= param1MultNT3;
#endif
#ifdef _Param3MultNT3
    nt3 *= param3MultNT3;
#endif

#ifdef _CW
#ifdef _Param3MultCWN
    nt3 = param3MultCWN.z == 0.0 ? nt3 : nt3 * (colorWeight.b * param3MultCWN.z);
    //nt3 *= colorWeight.b * param3MultCWN.z;
#else
    nt3 *= colorWeight.b;
#endif
#endif
   
#ifdef _REPLACENT3
    bump = nt3;
#elif defined (_ADDNT3)
    bump += nt3;
#elif defined (_MULTNT3)
    bump *= nt3;
#endif
#endif
#endif
	
	bump.y = -bump.y;
    vec3 reflVec = reflect(-n_vVec, bump);
    vec4 ambientColor = gl_FrontMaterial.ambient;

    vec4 diffuseColor = gl_FrontMaterial.diffuse;
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
#endif

#ifdef _DT
#ifdef _DTUV1
	_texCoord2 = _texCoord.zw;
#else
	_texCoord2 = _texCoord.xy;
#endif
    vec4 diffuseColor1;
#if defined(_REPLACEDT) || defined(_ADDDT) || defined(_MULTDT)
#ifdef _DM
    diffuseColor1 = texture2D(CRE_DiffuseMap, (diffuseMat * vec4(_texCoord2,0.0,1.0)).xy);
#else
    diffuseColor1 = texture2D(CRE_DiffuseMap, _texCoord2);
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
#endif

#ifdef _DT2
#ifdef _DT2UV1
	_texCoord2 = _texCoord.zw;
#else
	_texCoord2 = _texCoord.xy;
#endif
    vec4 diffuseColor2;
#if defined(_REPLACEDT2) || defined(_ADDDT2) || defined(_MULTDT2)
#ifdef _DM2
    diffuseColor2 = texture2D(CRE_DiffuseMap2, (diffuseMat2 * vec4(_texCoord2,0.0,1.0)).xy);
#else
    diffuseColor2 = texture2D(CRE_DiffuseMap2, _texCoord2);
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
#endif

    vec4 specularColor =gl_FrontMaterial.specular;
    float shininess = gl_FrontMaterial.shininess * 0.0078125;
    float tmpalpha = 0.0;
#ifdef _ST
#ifdef _STUV1
	_texCoord2 = _texCoord.zw;
#else
	_texCoord2 = _texCoord.xy;
#endif
    vec4 specularColor1;
    tmpalpha = 0.0;
#if defined(_REPLACEST) || defined(_ADDST) || defined(_MULTST)
#ifdef _SM
    specularColor1 = texture2D(CRE_SpecularMap, (specularMat * vec4(_texCoord2,0.0,1.0)).xy);
#else
    specularColor1 = texture2D(CRE_SpecularMap, _texCoord2);
#if defined(_REPLACESTa) || defined(_ADDSTa) || defined(_MULTSTa)
    tmpalpha = specularColor1.w;
#endif
#endif
#ifdef _Param4AddST
    specularColor1 += param4AddST;
#endif
#ifdef _Param1MultST
    specularColor1 *= param1MultST;
#endif
#ifdef _Param4MultST
    specularColor1 *= param4MultST;
#endif
#ifdef _CW
#ifdef _Param3MultCWS
    specularColor1 = param3MultCWS.x == 0.0 ? specularColor1 : specularColor1 * (colorWeight.r * param3MultCWS.x);
    //specularColor1 *= colorWeight.r * param3MultCWS.x;
#else
    specularColor1 *= colorWeight.r;
#endif
#endif

#ifdef _REPLACEST
    specularColor = specularColor1;
#elif defined (_ADDST)
    specularColor += specularColor1;
#elif defined (_MULTST)
    specularColor *= specularColor1;
#endif
#endif
#if defined(_REPLACESTa) || defined(_ADDSTa) || defined(_MULTSTa)
#ifdef _SMa
    tmpalpha = texture2D(CRE_SpecularMap, (specularMatA * vec4(_texCoord2,0.0,1.0)).xy).w;
#elif defined(_SM) || !(defined(_REPLACEST) || defined(_ADDST) || defined(_MULTST))
    tmpalpha = texture2D(CRE_SpecularMap, _texCoord2).w;     
#endif
#ifdef _Param1AddSTa
    tmpalpha += param1AddSTa;
#endif
#ifdef _Param1MultSTa
    tmpalpha *= param1MultSTa;
#endif
#ifdef _CW
#ifdef _Param3MultCWSa
    tmpalpha = param3MultCWSa.x == 0.0 ? tmpalpha : tmpalpha * (colorWeight.r * param3MultCWSa.x);
    //tmpalpha *= colorWeight.r * param3MultCWSa.x;
#else
    tmpalpha *= colorWeight.r;
#endif
#endif
#ifdef _REPLACESTa
    shininess = tmpalpha;
#elif defined (_ADDSTa)
    shininess += tmpalpha;
#elif defined (_MULTSTa)
    shininess *= tmpalpha;
#endif
#endif
#endif

#ifdef _ST2
#ifdef _ST2UV1
	_texCoord2 = _texCoord.zw;
#else
	_texCoord2 = _texCoord.xy;
#endif
    vec4 specularColor2;
    tmpalpha = 0.0;
#if defined(_REPLACEST2) || defined(_ADDST2) || defined(_MULTST2)
#ifdef _SM2
    specularColor2 = texture2D(CRE_SpecularMap2, (specularMat2 * vec4(_texCoord2,0.0,1.0)).xy);
#else
    specularColor2 = texture2D(CRE_SpecularMap2, _texCoord2);
#if defined(_REPLACEST2a) || defined(_ADDST2a) || defined(_MULTST2a)
    tmpalpha = specularColor2.w;
#endif
#endif
#ifdef _Param4AddST2
    specularColor2 += param4AddST2;
#endif
#ifdef _Param1MultST2
    specularColor2 *= param1MultST2;
#endif
#ifdef _Param4MultST2
    specularColor2 *= param4MultST2;
#endif
#ifdef _CW
#ifdef _Param3MultCWS
    specularColor2 = param3MultCWS.y == 0.0 ? specularColor2 : specularColor2 * (colorWeight.g * param3MultCWS.y);
    //specularColor2 *= colorWeight.g * param3MultCWS.y;
#else
    specularColor2 *= colorWeight.g;
#endif
#endif

#ifdef _REPLACEST2
    specularColor = specularColor2;
#elif defined (_ADDST2)
    specularColor += specularColor2;
#elif defined (_MULTST2)
    specularColor *= specularColor2;
#endif
#endif
#if defined(_REPLACEST2a) || defined(_ADDST2a) || defined(_MULTST2a)
#ifdef _SM2a
    tmpalpha = texture2D(CRE_SpecularMap2, (specularMat2A * vec4(_texCoord2,0.0,1.0)).xy).w;
#elif defined(_SM2) || !(defined(_REPLACEST2) || defined(_ADDST2) || defined(_MULTST2))
    tmpalpha = texture2D(CRE_SpecularMap2, _texCoord2).w;     
#endif
#ifdef _Param1AddST2a
    tmpalpha += param1AddST2a;
#endif
#ifdef _Param1MultST2a
    tmpalpha *= param1MultST2a;
#endif
#ifdef _CW
#ifdef _Param3MultCWSa
    tmpalpha = param3MultCWSa.y == 0.0 ? tmpalpha : tmpalpha * (colorWeight.g * param3MultCWSa.y);
    //tmpalpha *= colorWeight.g * param3MultCWSa.y;
#else
    tmpalpha *= colorWeight.g;
#endif
#endif
#ifdef _REPLACEST2a
    shininess = tmpalpha;
#elif defined (_ADDST2a)
    shininess += tmpalpha;
#elif defined (_MULTST2a)
    shininess *= tmpalpha;
#endif
#endif
#endif

#ifdef _ST3
#ifdef _ST3UV1
	_texCoord2 = _texCoord.zw;
#else
	_texCoord2 = _texCoord.xy;
#endif
    vec4 specularColor3;
    tmpalpha = 0.0;
#if defined(_REPLACEST3) || defined(_ADDST3) || defined(_MULTST3)
#ifdef _SM3
    specularColor3 = texture2D(CRE_SpecularMap3, (specularMat3 * vec4(_texCoord2,0.0,1.0)).xy);
#else
    specularColor3 = texture2D(CRE_SpecularMap3, _texCoord2);
#if defined(_REPLACEST3a) || defined(_ADDST3a) || defined(_MULTST3a)
    tmpalpha = specularColor3.w;
#endif
#endif
#ifdef _Param4AddST3
    specularColor3 += param4AddST3;
#endif
#ifdef _Param1MultST3
    specularColor3 *= param1MultST3;
#endif
#ifdef _Param4MultST3
    specularColor3 *= param4MultST3;
#endif
#ifdef _CW
#ifdef _Param3MultCWS
    specularColor3 = param3MultCWS.z == 0.0 ? specularColor3 : specularColor3 * (colorWeight.b * param3MultCWS.z);
    //specularColor3 *= colorWeight.b * param3MultCWS.z;
#else
    specularColor3 *= colorWeight.b;
#endif
#endif

#ifdef _REPLACEST3
    specularColor = specularColor3;
#elif defined (_ADDST3)
    specularColor += specularColor3;
#elif defined (_MULTST3)
    specularColor *= specularColor3;
#endif
#endif
#if defined(_REPLACEST3a) || defined(_ADDST3a) || defined(_MULTST3a)
#ifdef _SM3a
    tmpalpha = texture2D(CRE_SpecularMap3, (specularMat3A * vec4(_texCoord2,0.0,1.0)).xy).w;
#elif defined(_SM3) || !(defined(_REPLACEST3) || defined(_ADDST3) || defined(_MULTST3))
    tmpalpha = texture2D(CRE_SpecularMap3, _texCoord2).w;    
#endif
#ifdef _Param1AddST3a
    tmpalpha += param1AddST3a;
#endif
#ifdef _Param1MultST3a
    tmpalpha *= param1MultST3a;
#endif
#ifdef _CW
#ifdef _Param3MultCWSa
    tmpalpha = param3MultCWSa.z == 0.0 ? tmpalpha : tmpalpha * (colorWeight.b * param3MultCWSa.z);
    //tmpalpha *= colorWeight.b * param3MultCWSa.z;
#else
    tmpalpha *= colorWeight.b;
#endif
#endif
#ifdef _REPLACEST3a
    shininess = tmpalpha;
#elif defined (_ADDST3a)
    shininess += tmpalpha;
#elif defined (_MULTST3a)
    shininess *= tmpalpha;
#endif
#endif
#endif
    shininess = floor(saturate(shininess)*128.0);

#ifdef _ET
#ifdef _ETUV1
	_texCoord2 = _texCoord.zw;
#else
	_texCoord2 = _texCoord.xy;
#endif
#if defined(_ADDETTON) || defined(_MULTETTON)
    vec4 emissiveColor1;
#ifdef _EM
    emissiveColor1 = texture2D(CRE_EmissiveMap, (emissiveMat * vec4(_texCoord2,0.0,1.0)).xy);
#else
    emissiveColor1 = texture2D(CRE_EmissiveMap, _texCoord2);
#endif
	emissiveColor1.xy = (emissiveColor1.ag * 2.0) - 1.0;

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
#if  defined(_ADDETTON) || defined(_MULTETTON)
#ifdef _Param3MultCWE
    emissiveColor1 = param3MultCWE.x == 0.0 ? emissiveColor1 : emissiveColor1 * (colorWeight.a * param3MultCWE.x);
    //emissiveColor1 *= colorWeight.a * param3MultCWE.x;
#else
    emissiveColor1 *= colorWeight.a;
#endif
#endif
#endif

#ifdef _ADDETTON
    bump += emissiveColor1.xyz;
#elif defined (_MULTETTON)
    bump *= emissiveColor1.xyz;
#endif

#endif
#endif

#ifdef _ET2
#ifdef _ET2UV1
	_texCoord2 = _texCoord.zw;
#else
	_texCoord2 = _texCoord.xy;
#endif
#if defined(_ADDET2TOD) || defined(_MULTET2TOD)
    vec4 emissiveColor2;
#ifdef _EM2
    emissiveColor2 = texture2D(CRE_EmissiveMap2, (emissiveMat2 * vec4(_texCoord2,0.0,1.0)).xy);
#else
    emissiveColor2 = texture2D(CRE_EmissiveMap2, _texCoord2);
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
#endif
#endif

#ifdef _ADDET2TOD
    diffuseColor += emissiveColor2;
#elif defined (_MULTET2TOD)
    diffuseColor *= emissiveColor2;
#endif
#endif
#endif

#ifdef _ET3
#ifdef _ET3UV1
	_texCoord2 = _texCoord.zw;
#else
	_texCoord2 = _texCoord.xy;
#endif
#if defined(_ADDET3TOS) || defined(_MULTET3TOS)
    vec4 emissiveColor3;
#ifdef _EM3
    emissiveColor3 = texture2D(CRE_EmissiveMap3, (emissiveMat3 * vec4(_texCoord2,0.0,1.0)).xy);
#else
    emissiveColor3 = texture2D(CRE_EmissiveMap3, _texCoord2);
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
#endif
#endif
#ifdef _ADDET3TOS
    specularColor += emissiveColor3;
#elif defined (_MULTET3TOS)
    specularColor *= emissiveColor3;
#endif
#endif
#endif
    //bump = normalize(vec3(bump.xy,0.0));
    bump.z = sqrt(1.0 - saturate(dot(bump.xy, bump.xy)));
    vec4 color = vec4(0.0);

	vec3 n_lVec,tempVec;
	float diffuse,specular;
	
    float attenuation = 0.0;
    float lightLength2;
#if defined(direct1)
    tempVec = lightPosd1 - vtxPos;
    lightLength2 = dot(tempVec,tempVec);
    attenuation = 1.0/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParamd1[3].xyz);
    if(attenuation>loweastLumd1)
    {
		attenuation = attenuation - loweastLumd1;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParamd1[0] + diffuseColor * (lightParamd1[1] * diffuse)) * base + specularColor * (lightParamd1[2] * specular)) * attenuation;
		color += ((ambientColor*lightParamd1[0] + lightParamd1[1] * diffuse) * diffuseColor + specularColor * (lightParamd1[2] * specular)) * attenuation;
	}
#endif

#if defined(direct2)
    tempVec = lightPosd1 - vtxPos;
    lightLength2 = dot(tempVec,tempVec);
    attenuation = 1.0/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParamd1[3].xyz);
    if(attenuation>loweastLumd1)
    {
		attenuation = attenuation - loweastLumd1;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParamd1[0] + diffuseColor * (lightParamd1[1] * diffuse)) * base + specularColor * (lightParamd1[2] * specular)) * attenuation;
		color += ((ambientColor*lightParamd1[0] + lightParamd1[1] * diffuse) * diffuseColor + specularColor * (lightParamd1[2] * specular)) * attenuation;
	}
	
	tempVec = lightPosd2 - vtxPos;
    lightLength2 = dot(tempVec,tempVec);
    attenuation = 1.0/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParamd2[3].xyz);
    if(attenuation>loweastLumd2)
    {
		attenuation = attenuation - loweastLumd2;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParamd2[0] + diffuseColor * (lightParamd2[1] * diffuse)) * base + specularColor * (lightParamd2[2] * specular)) * attenuation;
		color += ((ambientColor*lightParamd2[0] + lightParamd2[1] * diffuse) * diffuseColor + specularColor * (lightParamd2[2] * specular)) * attenuation;
	}
#endif

#if defined(direct3)
    tempVec = lightPosd1 - vtxPos;
    lightLength2 = dot(tempVec,tempVec);
    attenuation = 1.0/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParamd1[3].xyz);
    if(attenuation>loweastLumd1)
    {
		attenuation = attenuation - loweastLumd1;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParamd1[0] + diffuseColor * (lightParamd1[1] * diffuse)) * base + specularColor * (lightParamd1[2] * specular)) * attenuation;
		color += ((ambientColor*lightParamd1[0] + lightParamd1[1] * diffuse) * diffuseColor + specularColor * (lightParamd1[2] * specular)) * attenuation;
	}
	
	tempVec = lightPosd2 - vtxPos;
    lightLength2 = dot(tempVec,tempVec);
    attenuation = 1.0/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParamd2[3].xyz);
    if(attenuation>loweastLumd2)
    {
		attenuation = attenuation - loweastLumd2;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParamd2[0] + diffuseColor * (lightParamd2[1] * diffuse)) * base + specularColor * (lightParamd2[2] * specular)) * attenuation;
		color += ((ambientColor*lightParamd2[0] + lightParamd2[1] * diffuse) * diffuseColor + specularColor * (lightParamd2[2] * specular)) * attenuation;
	}
	
    tempVec = lightPosd3 - vtxPos;
    lightLength2 = dot(tempVec,tempVec);
    attenuation = 1.0/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParamd3[3].xyz);
    if(attenuation>loweastLumd3)
    {
		attenuation = attenuation - loweastLumd3;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParamd3[0] + diffuseColor * (lightParamd3[1] * diffuse)) * base + specularColor * (lightParamd3[2] * specular)) * attenuation;
		color += ((ambientColor*lightParamd3[0] + lightParamd3[1] * diffuse) * diffuseColor + specularColor * (lightParamd3[2] * specular)) * attenuation;
	}
#endif

#if defined(direct4)
    tempVec = lightPosd1 - vtxPos;
    lightLength2 = dot(tempVec,tempVec);
    attenuation = 1.0/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParamd1[3].xyz);
    if(attenuation>loweastLumd1)
    {
		attenuation = attenuation - loweastLumd1;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParamd1[0] + diffuseColor * (lightParamd1[1] * diffuse)) * base + specularColor * (lightParamd1[2] * specular)) * attenuation;
		color += ((ambientColor*lightParamd1[0] + lightParamd1[1] * diffuse) * diffuseColor + specularColor * (lightParamd1[2] * specular)) * attenuation;
	}
	
	tempVec = lightPosd2 - vtxPos;
    lightLength2 = dot(tempVec,tempVec);
    attenuation = 1.0/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParamd2[3].xyz);
    if(attenuation>loweastLumd2)
    {
		attenuation = attenuation - loweastLumd2;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParamd2[0] + diffuseColor * (lightParamd2[1] * diffuse)) * base + specularColor * (lightParamd2[2] * specular)) * attenuation;
		color += ((ambientColor*lightParamd2[0] + lightParamd2[1] * diffuse) * diffuseColor + specularColor * (lightParamd2[2] * specular)) * attenuation;
	}
	
    tempVec = lightPosd3 - vtxPos;
    lightLength2 = dot(tempVec,tempVec);
    attenuation = 1.0/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParamd3[3].xyz);
    if(attenuation>loweastLumd3)
    {
		attenuation = attenuation - loweastLumd3;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParamd3[0] + diffuseColor * (lightParamd3[1] * diffuse)) * base + specularColor * (lightParamd3[2] * specular)) * attenuation;
		color += ((ambientColor*lightParamd3[0] + lightParamd3[1] * diffuse) * diffuseColor + specularColor * (lightParamd3[2] * specular)) * attenuation;
	}
	
	tempVec = lightPosd4 - vtxPos;
    lightLength2 = dot(tempVec,tempVec);
    attenuation = 1.0/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParamd4[3].xyz);
    if(attenuation>loweastLumd4)
    {
		attenuation = attenuation - loweastLumd4;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParamd4[0] + diffuseColor * (lightParamd4[1] * diffuse)) * base + specularColor * (lightParamd4[2] * specular)) * attenuation;
		color += ((ambientColor*lightParamd4[0] + lightParamd4[1] * diffuse) * diffuseColor + specularColor * (lightParamd4[2] * specular)) * attenuation;
	}
#endif

#if defined(direct5)
    tempVec = lightPosd1 - vtxPos;
    lightLength2 = dot(tempVec,tempVec);
    attenuation = 1.0/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParamd1[3].xyz);
    if(attenuation>loweastLumd1)
    {
		attenuation = attenuation - loweastLumd1;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParamd1[0] + diffuseColor * (lightParamd1[1] * diffuse)) * base + specularColor * (lightParamd1[2] * specular)) * attenuation;
		color += ((ambientColor*lightParamd1[0] + lightParamd1[1] * diffuse) * diffuseColor + specularColor * (lightParamd1[2] * specular)) * attenuation;
	}
	
	tempVec = lightPosd2 - vtxPos;
    lightLength2 = dot(tempVec,tempVec);
    attenuation = 1.0/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParamd2[3].xyz);
    if(attenuation>loweastLumd2)
    {
		attenuation = attenuation - loweastLumd2;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParamd2[0] + diffuseColor * (lightParamd2[1] * diffuse)) * base + specularColor * (lightParamd2[2] * specular)) * attenuation;
		color += ((ambientColor*lightParamd2[0] + lightParamd2[1] * diffuse) * diffuseColor + specularColor * (lightParamd2[2] * specular)) * attenuation;
	}
	
    tempVec = lightPosd3 - vtxPos;
    lightLength2 = dot(tempVec,tempVec);
    attenuation = 1.0/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParamd3[3].xyz);
    if(attenuation>loweastLumd3)
    {
		attenuation = attenuation - loweastLumd3;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParamd3[0] + diffuseColor * (lightParamd3[1] * diffuse)) * base + specularColor * (lightParamd3[2] * specular)) * attenuation;
		color += ((ambientColor*lightParamd3[0] + lightParamd3[1] * diffuse) * diffuseColor + specularColor * (lightParamd3[2] * specular)) * attenuation;
	}
	
	tempVec = lightPosd4 - vtxPos;
    lightLength2 = dot(tempVec,tempVec);
    attenuation = 1.0/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParamd4[3].xyz);
    if(attenuation>loweastLumd4)
    {
		attenuation = attenuation - loweastLumd4;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParamd4[0] + diffuseColor * (lightParamd4[1] * diffuse)) * base + specularColor * (lightParamd4[2] * specular)) * attenuation;
		color += ((ambientColor*lightParamd4[0] + lightParamd4[1] * diffuse) * diffuseColor + specularColor * (lightParamd4[2] * specular)) * attenuation;
	}

	tempVec = lightPosd5 - vtxPos;
    lightLength2 = dot(tempVec,tempVec);
    attenuation = 1.0/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParamd5[3].xyz);
    if(attenuation>loweastLumd5)
    {
		attenuation = attenuation - loweastLumd5;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParamd5[0] + diffuseColor * (lightParamd5[1] * diffuse)) * base + specularColor * (lightParamd5[2] * specular)) * attenuation;
		color += ((ambientColor*lightParamd5[0] + lightParamd5[1] * diffuse) * diffuseColor + specularColor * (lightParamd5[2] * specular)) * attenuation;
	}
#endif

#if defined(spot1)
	tempVec = lightPoss1 - vtxPos;
	float cosa = dot(normalize(-tempVec),lightDir1);
	attenuation = 0.0;
	if(cosa > cosLightFov1)
	{
	    lightLength2 = dot(tempVec,tempVec);
		attenuation = _pow(cosa,lightParams1[3].w)/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParams1[3].xyz);
	}
	if(attenuation>loweastLums1)
    {
		attenuation = attenuation - loweastLums1;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParams1[0] + diffuseColor * (lightParams1[1] * diffuse)) * base + specularColor * (lightParams1[2] * specular)) * attenuation;
		color += ((ambientColor*lightParams1[0] + lightParams1[1] * diffuse) * diffuseColor + specularColor * (lightParams1[2] * specular)) * attenuation;
	}
#endif

#if defined(spot2)
	tempVec = lightPoss1 - vtxPos;
	float cosa = dot(normalize(-tempVec),lightDir1);
	attenuation = 0.0;
	if(cosa > cosLightFov1)
	{
	    lightLength2 = dot(tempVec,tempVec);
		attenuation = _pow(cosa,lightParams1[3].w)/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParams1[3].xyz);
	}
	if(attenuation>loweastLums1)
    {
		attenuation = attenuation - loweastLums1;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParams1[0] + diffuseColor * (lightParams1[1] * diffuse)) * base + specularColor * (lightParams1[2] * specular)) * attenuation;
		color += ((ambientColor*lightParams1[0] + lightParams1[1] * diffuse) * diffuseColor + specularColor * (lightParams1[2] * specular)) * attenuation;
	}

	tempVec = lightPoss2 - vtxPos;
	cosa = dot(normalize(-tempVec),lightDir2);
	attenuation = 0.0;
	if(cosa > cosLightFov2)
	{
	    lightLength2 = dot(tempVec,tempVec);
		attenuation = _pow(cosa,lightParams2[3].w)/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParams2[3].xyz);
	}
	if(attenuation>loweastLums2)
    {
		attenuation = attenuation - loweastLums2;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParams2[0] + diffuseColor * (lightParams2[1] * diffuse)) * base + specularColor * (lightParams2[2] * specular)) * attenuation;
		color += ((ambientColor*lightParams2[0] + lightParams2[1] * diffuse) * diffuseColor + specularColor * (lightParams2[2] * specular)) * attenuation;
	}
#endif

#if defined(spot3)
	tempVec = lightPoss1 - vtxPos;
	float cosa = dot(normalize(-tempVec),lightDir1);
	attenuation = 0.0;
	if(cosa > cosLightFov1)
	{
	    lightLength2 = dot(tempVec,tempVec);
		attenuation = _pow(cosa,lightParams1[3].w)/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParams1[3].xyz);
	}
	if(attenuation>loweastLums1)
    {
		attenuation = attenuation - loweastLums1;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParams1[0] + diffuseColor * (lightParams1[1] * diffuse)) * base + specularColor * (lightParams1[2] * specular)) * attenuation;
		color += ((ambientColor*lightParams1[0] + lightParams1[1] * diffuse) * diffuseColor + specularColor * (lightParams1[2] * specular)) * attenuation;
	}

	tempVec = lightPoss2 - vtxPos;
	cosa = dot(normalize(-tempVec),lightDir2);
	attenuation = 0.0;
	if(cosa > cosLightFov2)
	{
	    lightLength2 = dot(tempVec,tempVec);
		attenuation = _pow(cosa,lightParams2[3].w)/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParams2[3].xyz);
	}
	if(attenuation>loweastLums2)
    {
		attenuation = attenuation - loweastLums2;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParams2[0] + diffuseColor * (lightParams2[1] * diffuse)) * base + specularColor * (lightParams2[2] * specular)) * attenuation;
		color += ((ambientColor*lightParams2[0] + lightParams2[1] * diffuse) * diffuseColor + specularColor * (lightParams2[2] * specular)) * attenuation;
	}

	tempVec = lightPoss3 - vtxPos;
	cosa = dot(normalize(-tempVec),lightDir3);
	attenuation = 0.0;
	if(cosa > cosLightFov3)
	{
	    lightLength2 = dot(tempVec,tempVec);
		attenuation = _pow(cosa,lightParams3[3].w)/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParams3[3].xyz);
	}
	if(attenuation>loweastLums3)
    {
		attenuation = attenuation - loweastLums3;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParams3[0] + diffuseColor * (lightParams3[1] * diffuse)) * base + specularColor * (lightParams3[2] * specular)) * attenuation;
		color += ((ambientColor*lightParams3[0] + lightParams3[1] * diffuse) * diffuseColor + specularColor * (lightParams3[2] * specular)) * attenuation;
	}
#endif

#if defined(spot4)
	tempVec = lightPoss1 - vtxPos;
	float cosa = dot(normalize(-tempVec),lightDir1);
	attenuation = 0.0;
	if(cosa > cosLightFov1)
	{
	    lightLength2 = dot(tempVec,tempVec);
		attenuation = _pow(cosa,lightParams1[3].w)/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParams1[3].xyz);
	}
	if(attenuation>loweastLums1)
    {
		attenuation = attenuation - loweastLums1;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParams1[0] + diffuseColor * (lightParams1[1] * diffuse)) * base + specularColor * (lightParams1[2] * specular)) * attenuation;
		color += ((ambientColor*lightParams1[0] + lightParams1[1] * diffuse) * diffuseColor + specularColor * (lightParams1[2] * specular)) * attenuation;
	}

	tempVec = lightPoss2 - vtxPos;
	cosa = dot(normalize(-tempVec),lightDir2);
	attenuation = 0.0;
	if(cosa > cosLightFov2)
	{
	    lightLength2 = dot(tempVec,tempVec);
		attenuation = _pow(cosa,lightParams2[3].w)/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParams2[3].xyz);
	}
	if(attenuation>loweastLums2)
    {
		attenuation = attenuation - loweastLums2;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParams2[0] + diffuseColor * (lightParams2[1] * diffuse)) * base + specularColor * (lightParams2[2] * specular)) * attenuation;
		color += ((ambientColor*lightParams2[0] + lightParams2[1] * diffuse) * diffuseColor + specularColor * (lightParams2[2] * specular)) * attenuation;
	}

	tempVec = lightPoss3 - vtxPos;
	cosa = dot(normalize(-tempVec),lightDir3);
	attenuation = 0.0;
	if(cosa > cosLightFov3)
	{
	    lightLength2 = dot(tempVec,tempVec);
		attenuation = _pow(cosa,lightParams3[3].w)/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParams3[3].xyz);
	}
	if(attenuation>loweastLums3)
    {
		attenuation = attenuation - loweastLums3;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParams3[0] + diffuseColor * (lightParams3[1] * diffuse)) * base + specularColor * (lightParams3[2] * specular)) * attenuation;
		color += ((ambientColor*lightParams3[0] + lightParams3[1] * diffuse) * diffuseColor + specularColor * (lightParams3[2] * specular)) * attenuation;
	}
	
    tempVec = lightPoss4 - vtxPos;
	cosa = dot(normalize(-tempVec),lightDir4);
	attenuation = 0.0;
	if(cosa > cosLightFov4)
	{
	    lightLength2 = dot(tempVec,tempVec);
		attenuation = _pow(cosa,lightParams4[3].w)/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParams4[3].xyz);
	}
	if(attenuation>loweastLums4)
    {
		attenuation = attenuation - loweastLums4;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParams4[0] + diffuseColor * (lightParams4[1] * diffuse)) * base + specularColor * (lightParams4[2] * specular)) * attenuation;
		color += ((ambientColor*lightParams4[0] + lightParams4[1] * diffuse) * diffuseColor + specularColor * (lightParams4[2] * specular)) * attenuation;
	}
#endif

#if defined(spot5)
	tempVec = lightPoss1 - vtxPos;
	float cosa = dot(normalize(-tempVec),lightDir1);
	attenuation = 0.0;
	if(cosa > cosLightFov1)
	{
	    lightLength2 = dot(tempVec,tempVec);
		attenuation = _pow(cosa,lightParams1[3].w)/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParams1[3].xyz);
	}
	if(attenuation>loweastLums1)
    {
		attenuation = attenuation - loweastLums1;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParams1[0] + diffuseColor * (lightParams1[1] * diffuse)) * base + specularColor * (lightParams1[2] * specular)) * attenuation;
		color += ((ambientColor*lightParams1[0] + lightParams1[1] * diffuse) * diffuseColor + specularColor * (lightParams1[2] * specular)) * attenuation;
	}

	tempVec = lightPoss2 - vtxPos;
	cosa = dot(normalize(-tempVec),lightDir2);
	attenuation = 0.0;
	if(cosa > cosLightFov2)
	{
	    lightLength2 = dot(tempVec,tempVec);
		attenuation = _pow(cosa,lightParams2[3].w)/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParams2[3].xyz);
	}
	if(attenuation>loweastLums2)
    {
		attenuation = attenuation - loweastLums2;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParams2[0] + diffuseColor * (lightParams2[1] * diffuse)) * base + specularColor * (lightParams2[2] * specular)) * attenuation;
		color += ((ambientColor*lightParams2[0] + lightParams2[1] * diffuse) * diffuseColor + specularColor * (lightParams2[2] * specular)) * attenuation;
	}

	tempVec = lightPoss3 - vtxPos;
	cosa = dot(normalize(-tempVec),lightDir3);
	attenuation = 0.0;
	if(cosa > cosLightFov3)
	{
	    lightLength2 = dot(tempVec,tempVec);
		attenuation = _pow(cosa,lightParams3[3].w)/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParams3[3].xyz);
	}
	if(attenuation>loweastLums3)
    {
		attenuation = attenuation - loweastLums3;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParams3[0] + diffuseColor * (lightParams3[1] * diffuse)) * base + specularColor * (lightParams3[2] * specular)) * attenuation;
		color += ((ambientColor*lightParams3[0] + lightParams3[1] * diffuse) * diffuseColor + specularColor * (lightParams3[2] * specular)) * attenuation;
	}
	
    tempVec = lightPoss4 - vtxPos;
	cosa = dot(normalize(-tempVec),lightDir4);
	attenuation = 0.0;
	if(cosa > cosLightFov4)
	{
	    lightLength2 = dot(tempVec,tempVec);
		attenuation = _pow(cosa,lightParams4[3].w)/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParams4[3].xyz);
	}
	if(attenuation>loweastLums4)
    {
		attenuation = attenuation - loweastLums4;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParams4[0] + diffuseColor * (lightParams4[1] * diffuse)) * base + specularColor * (lightParams4[2] * specular)) * attenuation;
		color += ((ambientColor*lightParams4[0] + lightParams4[1] * diffuse) * diffuseColor + specularColor * (lightParams4[2] * specular)) * attenuation;
	}

    tempVec = lightPoss5 - vtxPos;
	cosa = dot(normalize(-tempVec),lightDir5);
	attenuation = 0.0;
	if(cosa > cosLightFov5)
	{
	    lightLength2 = dot(tempVec,tempVec);
		attenuation = _pow(cosa,lightParams5[3].w)/dot(vec3(1.0, sqrt(lightLength2), lightLength2),lightParams5[3].xyz);
	}
	if(attenuation>loweastLums5)
    {
		attenuation = attenuation - loweastLums5;
        n_lVec = normalize(vec3(dot(tempVec, T), dot(tempVec, B), dot(tempVec, N)));
		diffuse = saturate(dot(n_lVec, bump));
		specular = _pow(saturate(dot(reflVec, n_lVec)), shininess);
		//color += ((ambientColor*lightParams5[0] + diffuseColor * (lightParams5[1] * diffuse)) * base + specularColor * (lightParams5[2] * specular)) * attenuation;
		color += ((ambientColor*lightParams5[0] + lightParams5[1] * diffuse) * diffuseColor + specularColor * (lightParams5[2] * specular)) * attenuation;
	}
#endif

    color *= gl_Color;
#if defined(_lm) || defined(_sgi)
#ifdef _sgi
	color.xyz *= texture2D(CRE_LightMap,_gicoord).xyz;
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
    color.a = 0;

#ifdef HDR
    gl_FragData[0] = min(color, 1.0);
    gl_FragData[1] = max(color - 1.0, 0.0);
    gl_FragData[1].w = 0.0;
#ifdef NeedNormalDepth
    gl_FragData[2] = vec4(0.0);
#endif
#else
	gl_FragColor = color;
#endif
}
{****end****}
///////////////////////////////////
//距离稍远光源就会被剪裁，所以这里没有必要做雾效