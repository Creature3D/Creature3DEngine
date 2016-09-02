/* -*- c -*- */
#ifndef INCLUDED_CR3DS_LIGHT_H
#define INCLUDED_CR3DS_LIGHT_H
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
 * $Id: crLight.h,v 1.4 2003/01/24 15:12:53 robert Exp $
 */

#ifndef INCLUDED_CR3DS_TYPES_H
#include <Plugin 3ds\crTypes.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Light
 * \ingroup light
 */
struct _cr3dsLight {
    cr3dsLight *next;
    char name[64];
    cr3dsBool spot_light;
    cr3dsBool see_cone;
    cr3dsRgb color;
    cr3dsVector position;
    cr3dsVector spot;
    cr3dsFloat roll;
    cr3dsBool off;
    cr3dsFloat outer_range;
    cr3dsFloat inner_range;
    cr3dsFloat multiplier;
    /*const char** excludes;*/
    cr3dsFloat attenuation;
    cr3dsBool rectangular_spot;
    cr3dsBool shadowed;
    cr3dsFloat shadow_bias;
    cr3dsFloat shadow_filter;
    cr3dsIntw shadow_size;
    cr3dsFloat spot_aspect;
    cr3dsBool use_projector;
    char projector[64];
    cr3dsIntd spot_overshoot;
    cr3dsBool ray_shadows;
    cr3dsFloat ray_bias;
    cr3dsFloat hot_spot;
    cr3dsFloat fall_off;
}; 

extern CR3DSAPI cr3dsLight* cr_3ds_light_new(const char *name);
extern CR3DSAPI void cr_3ds_light_free(cr3dsLight *mesh);
extern CR3DSAPI void cr_3ds_light_dump(cr3dsLight *light);
extern CR3DSAPI cr3dsBool cr_3ds_light_read(cr3dsLight *light, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_light_write(cr3dsLight *light, FILE *f);

#ifdef __cplusplus
};
#endif
#endif

