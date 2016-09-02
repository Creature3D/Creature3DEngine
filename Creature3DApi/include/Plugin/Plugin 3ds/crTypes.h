/* -*- c -*- */
#ifndef INCLUDED_CR3DS_TYPES_H
#define INCLUDED_CR3DS_TYPES_H
/*
 * The 3D Studio File Format Library
 * Copyright (C) 1996-2001 by J.E. Hoffmann <je-h@gmx.net>
 * All rights reserved.
 *
 * This program is  free  software;  you can redistribute it and/or modify it
 * under the terms of the  GNU Lesser General Public License  as published by 
 * the  Free Software Foundation;  either version 2.1 of the License,  or (at 
 * your option) any later version.
 *
 * This  program  is  distributed in  the  hope that it will  be useful,  but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or  FITNESS FOR A  PARTICULAR PURPOSE.  See the  GNU Lesser General Public  
 * License for more details.
 *
 * You should  have received  a copy of the GNU Lesser General Public License
 * along with  this program;  if not, write to the  Free Software Foundation,
 * Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id: crTypes.h,v 1.3 2001/10/31 18:33:26 robert Exp $
 */
#ifdef __cplusplus
extern "C" {
#endif


  #if defined (_CR3DS_DLL) && defined(_WIN32) && (!defined(__GNUC__))
  #ifdef CR3DS_EXPORT
  #define CR3DSAPI __declspec(dllexport)
  #else               
  #define CR3DSAPI __declspec(dllimport)
  #endif           
  #else
  #define CR3DSAPI
  #endif
 
// within the OSG just use the libary inline, no need to export symbols.
//#define CR3DSAPI

#define CR3DS_TRUE 1
#define CR3DS_FALSE 0

typedef int cr3dsBool;
typedef unsigned char cr3dsByte;
typedef unsigned short int cr3dsWord;
typedef unsigned long cr3dsDword;
typedef signed char cr3dsIntb;
typedef signed short int cr3dsIntw;
typedef signed long cr3dsIntd;
typedef float cr3dsFloat;
typedef double cr3dsDouble;

typedef float cr3dsVector[3];
typedef float cr3dsTexel[2];
typedef float cr3dsQuat[4];
typedef float cr3dsMatrix[4][4];
typedef float cr3dsRgb[3];
typedef float cr3dsRgba[4];

#define CR3DS_EPSILON (1e-8)
#define CR3DS_PI 3.14159265358979323846
#define CR3DS_TWOPI (2.0*CR3DS_PI)
#define CR3DS_HALFPI (CR3DS_PI/2.0)
#define CR3DS_DEG(x) ((180.0/CR3DS_PI)*(x))
#define CR3DS_RAD(x) ((CR3DS_PI/180.0)*(x))
  
#ifndef INCLUDED_STDIO_H
#define INCLUDED_STDIO_H
#include <stdio.h>
#endif

#ifdef _DEBUG
  #ifndef ASSERT
  #include <assert.h>
  #define ASSERT(__expr) assert(__expr)
  #endif
  #define CR3DS_ERROR_LOG \
    {printf("\t***CR3DS_ERROR_LOG*** %s : %d\n", __FILE__, __LINE__);}
#else 
  #ifndef ASSERT
  #define ASSERT(__expr)
  #endif
  #define CR3DS_ERROR_LOG
#endif

typedef struct _cr3dsFile cr3dsFile;
typedef struct _cr3dsBackground cr3dsBackground;
typedef struct _cr3dsAtmosphere cr3dsAtmosphere;
typedef struct _cr3dsShadow cr3dsShadow;
typedef struct _cr3dsViewport cr3dsViewport;
typedef struct _cr3dsMaterial cr3dsMaterial;
typedef struct _cr3dsFace cr3dsFace; 
typedef struct _cr3dsBoxMap cr3dsBoxMap; 
typedef struct _cr3dsMapData cr3dsMapData; 
typedef struct _cr3dsMesh cr3dsMesh;
typedef struct _cr3dsCamera cr3dsCamera;
typedef struct _cr3dsLight cr3dsLight;
typedef struct _cr3dsBoolKey cr3dsBoolKey;
typedef struct _cr3dsBoolTrack cr3dsBoolTrack;
typedef struct _cr3dsLin1Key cr3dsLin1Key;
typedef struct _cr3dsLin1Track cr3dsLin1Track;
typedef struct _cr3dsLin3Key cr3dsLin3Key;
typedef struct _cr3dsLin3Track cr3dsLin3Track;
typedef struct _cr3dsQuatKey cr3dsQuatKey;
typedef struct _cr3dsQuatTrack cr3dsQuatTrack;
typedef struct _cr3dsMorphKey cr3dsMorphKey;
typedef struct _cr3dsMorphTrack cr3dsMorphTrack;

typedef enum _cr3dsNodeTypes {
  CR3DS_UNKNOWN_NODE =0,
  CR3DS_AMBIENT_NODE =1,
  CR3DS_OBJECT_NODE  =2,
  CR3DS_CAMERA_NODE  =3,
  CR3DS_TARGET_NODE  =4,
  CR3DS_LIGHT_NODE   =5,
  CR3DS_SPOT_NODE    =6
} cr3dsNodeTypes;

typedef struct _cr3dsNode cr3dsNode;

typedef union _cr3dsUserData {
    void *p;
    cr3dsIntd i;
    cr3dsDword d;
    cr3dsFloat f;
    cr3dsMaterial *material;
    cr3dsMesh *mesh;
    cr3dsCamera *camera;
    cr3dsLight *light;
    cr3dsNode *node;
} cr3dsUserData;

#ifdef __cplusplus
};
#endif
#endif









