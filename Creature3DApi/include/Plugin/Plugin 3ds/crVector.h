/* -*- c -*- */
#ifndef INCLUDED_CR3DS_VECTOR_H
#define INCLUDED_CR3DS_VECTOR_H
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
 * $Id: crVector.h,v 1.4 2003/01/24 15:12:54 robert Exp $
 */

#ifndef INCLUDED_CR3DS_TYPES_H
#include <Plugin 3ds\crTypes.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern CR3DSAPI void cr_3ds_vector_zero(cr3dsVector c);
extern CR3DSAPI void cr_3ds_vector_copy(cr3dsVector dest, cr3dsVector src);
extern CR3DSAPI void cr_3ds_vector_neg(cr3dsVector c);
extern CR3DSAPI void cr_3ds_vector_add(cr3dsVector c, cr3dsVector a, cr3dsVector b);
extern CR3DSAPI void cr_3ds_vector_sub(cr3dsVector c, cr3dsVector a, cr3dsVector b);
extern CR3DSAPI void cr_3ds_vector_scalar(cr3dsVector c, cr3dsFloat k);
extern CR3DSAPI void cr_3ds_vector_cross(cr3dsVector c, cr3dsVector a, cr3dsVector b);
extern CR3DSAPI cr3dsFloat cr_3ds_vector_dot(cr3dsVector a, cr3dsVector b);
extern CR3DSAPI cr3dsFloat cr_3ds_vector_squared(cr3dsVector c);
extern CR3DSAPI cr3dsFloat cr_3ds_vector_length(cr3dsVector c);
extern CR3DSAPI void cr_3ds_vector_normalize(cr3dsVector c);
extern CR3DSAPI void cr_3ds_vector_normal(cr3dsVector n, cr3dsVector a,
  cr3dsVector b, cr3dsVector c);
extern CR3DSAPI void cr_3ds_vector_transform(cr3dsVector c, cr3dsMatrix m, cr3dsVector a);
extern CR3DSAPI void cr_3ds_vector_cubic(cr3dsVector c, cr3dsVector a, cr3dsVector p,
  cr3dsVector q, cr3dsVector b, cr3dsFloat t);
extern CR3DSAPI void cr_3ds_vector_min(cr3dsVector c, cr3dsVector a);
extern CR3DSAPI void cr_3ds_vector_max(cr3dsVector c, cr3dsVector a);
extern CR3DSAPI void cr_3ds_vector_dump(cr3dsVector c);

#ifdef __cplusplus
};
#endif
#endif

