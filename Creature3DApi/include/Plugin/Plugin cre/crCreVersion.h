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
#ifndef CRE_VERSION
#define CRE_VERSION 1


/* The VERSION tag should be updated any time the
   CRE format changes in order to support backward
   compatibility (if implemented).  VERSION is 
   stored in the 2nd 4 bytes of the file */

#define VERSION_0001 1
#define VERSION_0002 2
#define VERSION_0003 3
#define VERSION_0004 4
#define VERSION_0005 5
#define VERSION_0006 6
#define VERSION_0007 7

#define VERSION_0008 8
#define VERSION_0009 9
#define VERSION_0010 10
#define VERSION_0011 11

#define VERSION_0012 12
#define VERSION_0013 13
#define VERSION_0014 14
#define VERSION_0015 15
#define VERSION_0016 16

#define VERSION_0017 17
#define VERSION_0018 18

#define VERSION_0019 19 //dof

#define VERSION_0020 20 //Particle
#define VERSION_0021 21 //drawable color
#define VERSION_0022 22 //Particle texture tile
#define VERSION_0023 23 //Billboard
#define VERSION_0024 24 //Particle LodValue

#define VERSION_0025 25 //LightMap
#define VERSION_0026 26 //Light CalcShadow2
#define VERSION_0027 27 //CalcShadow->Static/Dynamic
#define VERSION_0028 28 //CalcShadow->Static/Dynamic 2
#define VERSION_0029 29 //AvoidIntersect

#define VERSION_0030 30 //FogHeight ParticleSize
#define VERSION_0031 31 //RenderBin
#define VERSION_0032 32 //GI
#define VERSION_0033 33 //DrawOutLine
#define VERSION_0034 34 //除去InitRenderVecBuf
#define VERSION_0035 35 //Brain WorldSize
#define VERSION_0036 36 //顶点色
#define VERSION_0037 37 //粒子系统旋转角度
#define VERSION_0038 38 //粒子系统Billboard
#define VERSION_0039 39 //uv1
#define VERSION_0040 40 //image filename
#define VERSION_0041 41 //image filename
#define VERSION_0042 42 //SpotLightMap
#define VERSION_0043 43 //GIMapRenderable
#define VERSION      VERSION_0043


/* The BYTE_SEX tag is used to check the endian
   of the CRE file being read in.  The CRE format
   is always written in the native endian of the
   machine to provide optimum reading of the file.
   BYTE_SEX is stored in the first 4 bytes of the 
   file */
#define ENDIAN_TYPE 0x01020304
#define OPPOSITE_ENDIAN_TYPE 0x04030201


#endif
