#ifndef CRCORE_EXPORT
#define CRCORE_EXPORT 1

// define used to include in API which is being fazed out
// if you can compile your apps with this turned off you are
// well placed for compatablity with future versions.
#define USE_DEPRECATED_API

#if defined(WIN32) && !(defined(__CYGWIN__) || defined(__MINGW32__))
	#pragma warning( disable : 4244 )
	#pragma warning( disable : 4251 )
	#pragma warning( disable : 4267 )
	#pragma warning( disable : 4275 )
	#pragma warning( disable : 4290 )
	#pragma warning( disable : 4786 )
	#pragma warning( disable : 4305 )
	#pragma warning( disable : 4006 )
	#pragma warning( disable : 4312 )
	#pragma warning( disable : 4311 )
	#pragma warning( disable : 4018 )
    #pragma warning( disable : 4800 )
    #pragma warning( disable : 4996 )
#endif

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)

#  ifdef CR_LIBRARY
#    define CR_EXPORT   __declspec(dllexport)
#  else
#    define CR_EXPORT   __declspec(dllimport)
#  endif /* CR_LIBRARY */
#else
#  define CR_EXPORT
#endif  

// set up define for whether member templates are supported by VisualStudio compilers.
#ifdef _MSC_VER
# if (_MSC_VER >= 1300)
#  define __STL_MEMBER_TEMPLATES
# endif
#endif

/* Define NULL pointer value */

#ifndef NULL
#ifdef  __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

///define creature3d texture channels
//0：Basemap
//1：Normalmap
//2：Lightmap//静态的shadowmap
//3：Shadowmap
//4：Envmap
//5：Emissivemap
//6：Specularmap//rgb表示specular color,  a表示shiness
//7：Noisemap

#define TEXTURE_BASEMAP 0//第1层漫反射贴图
#define TEXTURE_BUMPMAP 2//第1层发线贴图
#define TEXTURE_LIGHTMAP 1//3DMAX阴影烘培贴图
#define TEXTURE_SHADOWMAP 3//实时阴影1
#define TEXTURE_SHADOWMAP2 13//实时阴影2
#define TEXTURE_ENVMAP 1//环境贴图CubeMap
#define TEXTURE_DIFFUSEMAP 5//第2层漫反射贴图
#define TEXTURE_SPECULARMAP 6//第1层高光贴图
#define TEXTURE_EMISSIVEMAP 7//自选通道1
//#define TEXTURE_NOISEMAP 1//

#define TEXTURE_NORMALMAP2 8//第2层发线贴图
#define TEXTURE_DIFFUSEMAP2 9//第3层漫反射贴图
#define TEXTURE_SPECULARMAP2 10//第2层高光贴图
#define TEXTURE_EMISSIVEMAP2 11//自选通道2

#define TEXTURE_NORMALMAP3 12//，作废
//#define TEXTURE_DIFFUSEMAP3 13
#define TEXTURE_SPECULARMAP3 14//高光贴图
#define TEXTURE_EMISSIVEMAP3 15//自选通道3

//#define TEXTURE_METALLICMAP 10
//#define TEXTURE_ROUGHNESSMAP 14

#define TEXTURE_UVSCRAMBLER 4//ExternTextureID
#define TEXTURE_COLORWEIGHT 4//ExternTextureID
#define TEXTURE_GIMAP 13
#define TEXTURE_SPOTLIGHTMAP 13//光源贴图IES？
//#define TEXTURE_DETAILMAP 8
//#define TEXTURE_REFLECTMAP 9
//#define TEXTURE_REFRACTMAP 10
//
//#define TEXTURE_DETAILBUMPMAP 3

///define creature3d vertex attributes channels
//#define ATTRIBUTE_VERTEX 0
#define ATTRIBUTE_TANGENT 6   //tangent
#define ATTRIBUTE_BINORMAL 7 //binormal
//#define ATTRIBUTE_NORMAL 8 //normal

#endif
