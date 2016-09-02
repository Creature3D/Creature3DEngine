/* -*- c -*- */
#ifndef INCLUDED_CR3DS_SHADOW_H
#define INCLUDED_CR3DS_SHADOW_H
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
 * $Id: crShadow.h,v 1.4 2003/01/24 15:12:54 robert Exp $
 */

#ifndef INCLUDED_CR3DS_TYPES_H
#include <Plugin 3ds\crTypes.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Shadow map settings
 * \ingroup shadow
 */
struct _cr3dsShadow {
    cr3dsIntw map_size;
    cr3dsFloat lo_bias;
    cr3dsFloat hi_bias;
    cr3dsIntw samples;
    cr3dsIntd range;
    cr3dsFloat filter;
    cr3dsFloat ray_bias;
};

extern CR3DSAPI cr3dsBool cr_3ds_shadow_read(cr3dsShadow *shadow, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_shadow_write(cr3dsShadow *shadow, FILE *f);

#ifdef __cplusplus
};
#endif
#endif





