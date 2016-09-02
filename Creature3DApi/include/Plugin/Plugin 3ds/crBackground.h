/* -*- c -*- */
#ifndef INCLUDED_CR3DS_BACKGROUND_H
#define INCLUDED_CR3DS_BACKGROUND_H
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
 * $Id: crBackground.h,v 1.4 2003/01/24 15:12:52 robert Exp $
 */

#ifndef INCLUDED_CR3DS_TYPES_H
#include <Plugin 3ds\crTypes.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Bitmap background settings
 * \ingroup background
 */
typedef struct _cr3dsBitmap {
    cr3dsBool use;
    char name[64];
} cr3dsBitmap;

/*!
 * Solid color background settings
 * \ingroup background
 */
typedef struct _cr3dsSolid {
    cr3dsBool use;
    cr3dsRgb col;
} cr3dsSolid;

/*!
 * Gradient background settings
 * \ingroup background
 */
typedef struct _cr3dsGradient {
    cr3dsBool use;
    cr3dsFloat percent;
    cr3dsRgb top;
    cr3dsRgb middle;
    cr3dsRgb bottom;
} cr3dsGradient;

/*!
 * Background settings
 * \ingroup background
 */
struct _cr3dsBackground {
    cr3dsBitmap bitmap;
    cr3dsSolid solid;
    cr3dsGradient gradient;
};

extern CR3DSAPI cr3dsBool cr_3ds_background_read(cr3dsBackground *background, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_background_write(cr3dsBackground *background, FILE *f);

#ifdef __cplusplus
};
#endif
#endif





