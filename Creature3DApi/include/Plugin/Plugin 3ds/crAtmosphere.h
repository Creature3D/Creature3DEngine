/* -*- c -*- */
#ifndef INCLUDED_CR3DS_ATMOSPHERE_H
#define INCLUDED_CR3DS_ATMOSPHERE_H
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
 * $Id: crAtmosphere.h,v 1.4 2003/01/24 15:12:52 robert Exp $
 */

#ifndef INCLUDED_CR3DS_TYPES_H
#include <Plugin 3ds\crTypes.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Fog atmosphere settings
 * \ingroup atmosphere
 */
typedef struct _cr3dsFog {
    cr3dsBool use;
    cr3dsRgb col;
    cr3dsBool fog_background;
    cr3dsFloat near_plane;
    cr3dsFloat near_density;
    cr3dsFloat far_plane;
    cr3dsFloat far_density;
} cr3dsFog;

/*!
 * Layer fog atmosphere flags
 * \ingroup atmosphere
 */
typedef enum _cr3dsLayerFogFlags {
  CR3DS_BOTTOM_FALL_OFF =0x00000001,
  CR3DS_TOP_FALL_OFF    =0x00000002,
  CR3DS_FOG_BACKGROUND  =0x00100000
} cr3dsLayerFogFlags;

/*!
 * Layer fog atmosphere settings
 * \ingroup atmosphere
 */
typedef struct _cr3dsLayerFog {
    cr3dsBool use;
    cr3dsDword flags;
    cr3dsRgb col;
    cr3dsFloat near_y;
    cr3dsFloat far_y;
    cr3dsFloat density;
} cr3dsLayerFog;

/*!
 * Distance cue atmosphere settings
 * \ingroup atmosphere
 */
typedef struct _cr3dsDistanceCue {
    cr3dsBool use;
    cr3dsBool cue_background;
    cr3dsFloat near_plane;
    cr3dsFloat near_dimming;
    cr3dsFloat far_plane;
    cr3dsFloat far_dimming;
} cr3dsDistanceCue;

/*!
 * Atmosphere settings
 * \ingroup atmosphere
 */
struct _cr3dsAtmosphere {
    cr3dsFog fog;
    cr3dsLayerFog layer_fog;
    cr3dsDistanceCue dist_cue;
};

extern CR3DSAPI cr3dsBool cr_3ds_atmosphere_read(cr3dsAtmosphere *atmosphere, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_atmosphere_write(cr3dsAtmosphere *atmosphere, FILE *f);

#ifdef __cplusplus
};
#endif
#endif
