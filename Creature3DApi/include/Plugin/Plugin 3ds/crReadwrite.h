/* -*- c -*- */
#ifndef INCLUDED_CR3DS_READWRITE_H
#define INCLUDED_CR3DS_READWRITE_H
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
 * $Id: crReadwrite.h,v 1.4 2003/01/24 15:12:54 robert Exp $
 */

#ifndef INCLUDED_CR3DS_TYPES_H
#include <Plugin 3ds\crTypes.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern CR3DSAPI cr3dsByte cr_3ds_byte_read(FILE *f);
extern CR3DSAPI cr3dsWord cr_3ds_word_read(FILE *f);
extern CR3DSAPI cr3dsDword cr_3ds_dword_read(FILE *f);
extern CR3DSAPI cr3dsIntb cr_3ds_intb_read(FILE *f);
extern CR3DSAPI cr3dsIntw cr_3ds_intw_read(FILE *f);
extern CR3DSAPI cr3dsIntd cr_3ds_intd_read(FILE *f);
extern CR3DSAPI cr3dsFloat cr_3ds_float_read(FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_vector_read(cr3dsVector v, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_rgb_read(cr3dsRgb rgb, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_string_read(char *s, int buflen, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_byte_write(cr3dsByte b, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_word_write(cr3dsWord w, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_dword_write(cr3dsDword d, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_intb_write(cr3dsIntb b, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_intw_write(cr3dsIntw w, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_intd_write(cr3dsIntd d, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_float_write(cr3dsFloat l, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_vector_write(cr3dsVector v, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_rgb_write(cr3dsRgb rgb, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_string_write(const char *s, FILE *f);

#ifdef __cplusplus
};
#endif
#endif

