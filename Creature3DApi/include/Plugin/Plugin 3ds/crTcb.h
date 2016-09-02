/* -*- c -*- */
#ifndef INCLUDED_CR3DS_TCB_H
#define INCLUDED_CR3DS_TCB_H
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
 * $Id: crTcb.h,v 1.4 2003/01/24 15:12:54 robert Exp $
 */

#ifndef INCLUDED_CR3DS_TYPES_H
#include <Plugin 3ds\crTypes.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _cr3dsTcbFlags{
  CR3DS_USE_TENSION    =0x0001,
  CR3DS_USE_CONTINUITY =0x0002,
  CR3DS_USE_BIAS       =0x0004,
  CR3DS_USE_EASE_TO    =0x0008,
  CR3DS_USE_EASE_FROM  =0x0010
} cr3dsTcbFlags;

typedef struct _cr3dsTcb {
    cr3dsIntd frame;
    cr3dsWord flags;
    cr3dsFloat tens;
    cr3dsFloat cont;
    cr3dsFloat bias;
    cr3dsFloat ease_to;
    cr3dsFloat ease_from;
} cr3dsTcb;

extern CR3DSAPI void cr_3ds_tcb(cr3dsTcb *p, cr3dsTcb *pc, cr3dsTcb *c,
  cr3dsTcb *nc, cr3dsTcb *n, cr3dsFloat *ksm, cr3dsFloat *ksp,
  cr3dsFloat *kdm, cr3dsFloat *kdp);
extern CR3DSAPI cr3dsBool cr_3ds_tcb_read(cr3dsTcb *tcb, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_tcb_write(cr3dsTcb *tcb, FILE *f);

#ifdef __cplusplus
};
#endif
#endif

