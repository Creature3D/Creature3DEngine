/* -*- c -*- */
#ifndef INCLUDED_CR3DS_QUAT_H
#define INCLUDED_CR3DS_QUAT_H
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
 * $Id: crQuat.h,v 1.4 2003/01/24 15:12:54 robert Exp $
 */

#ifndef INCLUDED_CR3DS_TYPES_H
#include <Plugin 3ds\crTypes.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern CR3DSAPI void cr_3ds_quat_zero(cr3dsQuat c);
extern CR3DSAPI void cr_3ds_quat_identity(cr3dsQuat c);
extern CR3DSAPI void cr_3ds_quat_copy(cr3dsQuat dest, cr3dsQuat src);
extern CR3DSAPI void cr_3ds_quat_axis_angle(cr3dsQuat c, cr3dsVector axis, cr3dsFloat angle);
extern CR3DSAPI void cr_3ds_quat_neg(cr3dsQuat c);
extern CR3DSAPI void cr_3ds_quat_abs(cr3dsQuat c);
extern CR3DSAPI void cr_3ds_quat_cnj(cr3dsQuat c);
extern CR3DSAPI void cr_3ds_quat_mul(cr3dsQuat c, cr3dsQuat a, cr3dsQuat b);
extern CR3DSAPI void cr_3ds_quat_scalar(cr3dsQuat c, cr3dsFloat k);
extern CR3DSAPI void cr_3ds_quat_normalize(cr3dsQuat c);
extern CR3DSAPI void cr_3ds_quat_inv(cr3dsQuat c);
extern CR3DSAPI cr3dsFloat cr_3ds_quat_dot(cr3dsQuat a, cr3dsQuat b);
extern CR3DSAPI cr3dsFloat cr_3ds_quat_squared(cr3dsQuat c);
extern CR3DSAPI cr3dsFloat cr_3ds_quat_length(cr3dsQuat c);
extern CR3DSAPI void cr_3ds_quat_ln(cr3dsQuat c);
extern CR3DSAPI void cr_3ds_quat_ln_dif(cr3dsQuat c, cr3dsQuat a, cr3dsQuat b);
extern CR3DSAPI void cr_3ds_quat_exp(cr3dsQuat c);
extern CR3DSAPI void cr_3ds_quat_slerp(cr3dsQuat c, cr3dsQuat a, cr3dsQuat b, cr3dsFloat t);
extern CR3DSAPI void cr_3ds_quat_squad(cr3dsQuat c, cr3dsQuat a, cr3dsQuat p, cr3dsQuat q,
  cr3dsQuat b, cr3dsFloat t);
extern CR3DSAPI void cr_3ds_quat_tangent(cr3dsQuat c, cr3dsQuat p, cr3dsQuat q, cr3dsQuat n);
extern CR3DSAPI void cr_3ds_quat_dump(cr3dsQuat q);

#ifdef __cplusplus
};
#endif
#endif

