/* -*- c -*- */
#ifndef INCLUDED_CR3DS_MATRIX_H
#define INCLUDED_CR3DS_MATRIX_H
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
 * $Id: crMatrix.h,v 1.4 2003/01/24 15:12:53 robert Exp $
 */

#ifndef INCLUDED_CR3DS_TYPES_H
#include <Plugin 3ds\crTypes.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern CR3DSAPI void cr_3ds_matrix_zero(cr3dsMatrix m);
extern CR3DSAPI void cr_3ds_matrix_identity(cr3dsMatrix m);
extern CR3DSAPI void cr_3ds_matrix_copy(cr3dsMatrix dest, cr3dsMatrix src);
extern CR3DSAPI void cr_3ds_matrix_neg(cr3dsMatrix m);
extern CR3DSAPI void cr_3ds_matrix_abs(cr3dsMatrix m);
extern CR3DSAPI void cr_3ds_matrix_transpose(cr3dsMatrix m);
extern CR3DSAPI void cr_3ds_matrix_add(cr3dsMatrix m, cr3dsMatrix a, cr3dsMatrix b);
extern CR3DSAPI void cr_3ds_matrix_sub(cr3dsMatrix m, cr3dsMatrix a, cr3dsMatrix b);
extern CR3DSAPI void cr_3ds_matrix_mul(cr3dsMatrix m, cr3dsMatrix a, cr3dsMatrix b);
extern CR3DSAPI void cr_3ds_matrix_scalar(cr3dsMatrix m, cr3dsFloat k);
extern CR3DSAPI cr3dsFloat cr_3ds_matrix_det(cr3dsMatrix m);
extern CR3DSAPI void cr_3ds_matrix_adjoint(cr3dsMatrix m);
extern CR3DSAPI cr3dsBool cr_3ds_matrix_inv(cr3dsMatrix m);
void cr_3ds_matrix_translate_xyz(cr3dsMatrix m, cr3dsFloat x,
  cr3dsFloat y, cr3dsFloat z);
extern CR3DSAPI void cr_3ds_matrix_translate(cr3dsMatrix m, cr3dsVector t);
extern CR3DSAPI void cr_3ds_matrix_scale_xyz(cr3dsMatrix m, cr3dsFloat x,
  cr3dsFloat y, cr3dsFloat z);
extern CR3DSAPI void cr_3ds_matrix_scale(cr3dsMatrix m, cr3dsVector s);
extern CR3DSAPI void cr_3ds_matrix_rotate_x(cr3dsMatrix m, cr3dsFloat phi);
extern CR3DSAPI void cr_3ds_matrix_rotate_y(cr3dsMatrix m, cr3dsFloat phi);
extern CR3DSAPI void cr_3ds_matrix_rotate_z(cr3dsMatrix m, cr3dsFloat phi);
extern CR3DSAPI void cr_3ds_matrix_rotate(cr3dsMatrix m, cr3dsQuat q);
extern CR3DSAPI void cr_3ds_matrix_rotate_axis(cr3dsMatrix m,
  cr3dsVector axis, cr3dsFloat angle);
extern CR3DSAPI void cr_3ds_matrix_camera(cr3dsMatrix matrix, cr3dsVector pos,
  cr3dsVector tgt, cr3dsFloat roll);
extern CR3DSAPI void cr_3ds_matrix_dump(cr3dsMatrix matrix);

#ifdef __cplusplus
};
#endif
#endif

