/* -*- c -*- */
#ifndef INCLUDED_CR3DS_MATERIAL_H
#define INCLUDED_CR3DS_MATERIAL_H
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
 * $Id: crMaterial.h,v 1.4 2003/01/24 15:12:53 robert Exp $
 */

#ifndef INCLUDED_CR3DS_TYPES_H
#include <Plugin 3ds\crTypes.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \ingroup material 
 */
typedef enum _cr3dsTextureMapFlags {
  CR3DS_DECALE       =0x0001,
  CR3DS_MIRROR       =0x0002,
  CR3DS_NEGATE       =0x0004,
  CR3DS_NO_TILE      =0x0008,
  CR3DS_SUMMED_AREA  =0x0010,
  CR3DS_ALPHA_SOURCE =0x0020,
  CR3DS_TINT         =0x0040,
  CR3DS_IGNORE_ALPHA =0x0080,
  CR3DS_RGB_TINT     =0x0100
} cr3dsTextureMapFlags;

/*!
 * Mateial texture map
 * \ingroup material 
 */
typedef struct _cr3dsTextureMap {
    char name[64];
    cr3dsDword flags;
    cr3dsFloat percent;
    cr3dsFloat blur;
    cr3dsFloat scale[2];
    cr3dsFloat offset[2];
    cr3dsFloat rotation;
    cr3dsRgb tint_1;
    cr3dsRgb tint_2;
    cr3dsRgb tint_r;
    cr3dsRgb tint_g;
    cr3dsRgb tint_b;
} cr3dsTextureMap;

/*!
 * \ingroup material 
 */
typedef enum _cr3dsAutoReflMapFlags {
  CR3DS_USE_REFL_MAP          =0x0001,
  CR3DS_READ_FIRST_FRAME_ONLY =0x0004,
  CR3DS_FLAT_MIRROR           =0x0008 
} cr3dsAutoReflectionMapFlags;

/*!
 * \ingroup material 
 */
typedef enum _cr3dsAutoReflMapAntiAliasLevel {
  CR3DS_ANTI_ALIAS_NONE   =0,
  CR3DS_ANTI_ALIAS_LOW    =1,
  CR3DS_ANTI_ALIAS_MEDIUM =2,
  CR3DS_ANTI_ALIAS_HIGH   =3
} cr3dsAutoReflMapAntiAliasLevel;

/*!
 * Auto reflection map settings
 * \ingroup material 
 */
typedef struct _cr3dsAutoReflMap {
    cr3dsDword flags;
    cr3dsIntd level;
    cr3dsIntd size;
    cr3dsIntd frame_step;
} cr3dsAutoReflMap;

/*!
 * \ingroup material 
 */
typedef enum _cr3dsMaterialShading {
  CR3DS_WIRE_FRAME =0,
  CR3DS_FLAT       =1, 
  CR3DS_GOURAUD    =2, 
  CR3DS_PHONG      =3, 
  CR3DS_METAL      =4
} cr3dsMaterialShading; 

/*!
 * Material
 * \ingroup material 
 */
struct _cr3dsMaterial {
    cr3dsUserData user;
    cr3dsMaterial *next;
    char name[64];
    cr3dsRgba ambient;
    cr3dsRgba diffuse;
    cr3dsRgba specular;
    cr3dsFloat shininess;
    cr3dsFloat shin_strength;
    cr3dsBool use_blur;
    cr3dsFloat blur;
    cr3dsFloat transparency;
    cr3dsFloat falloff;
    cr3dsBool additive;
    cr3dsBool use_falloff;
    cr3dsBool self_illum;
    cr3dsIntw shading;
    cr3dsBool soften;
    cr3dsBool face_map;
    cr3dsBool two_sided;
    cr3dsBool map_decal;
    cr3dsBool use_wire;
    cr3dsBool use_wire_abs;
    cr3dsFloat wire_size;
    cr3dsTextureMap texture1_map;
    cr3dsTextureMap texture1_mask;
    cr3dsTextureMap texture2_map;
    cr3dsTextureMap texture2_mask;
    cr3dsTextureMap opacity_map;
    cr3dsTextureMap opacity_mask;
    cr3dsTextureMap bump_map;
    cr3dsTextureMap bump_mask;
    cr3dsTextureMap specular_map;
    cr3dsTextureMap specular_mask;
    cr3dsTextureMap shininess_map;
    cr3dsTextureMap shininess_mask;
    cr3dsTextureMap self_illum_map;
    cr3dsTextureMap self_illum_mask;
    cr3dsTextureMap reflection_map;
    cr3dsTextureMap reflection_mask;
    cr3dsAutoReflMap autorefl_map;
};

extern CR3DSAPI cr3dsMaterial* cr_3ds_material_new();
extern CR3DSAPI void cr_3ds_material_free(cr3dsMaterial *material);
extern CR3DSAPI void cr_3ds_material_dump(cr3dsMaterial *material);
extern CR3DSAPI cr3dsBool cr_3ds_material_read(cr3dsMaterial *material, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_material_write(cr3dsMaterial *material, FILE *f);

#ifdef __cplusplus
};
#endif
#endif



