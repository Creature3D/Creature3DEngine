/* -*- c -*- */
#ifndef INCLUDED_CR3DS_CAMERA_H
#define INCLUDED_CR3DS_CAMERA_H
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
 * $Id: crCamera.h,v 1.4 2003/01/24 15:12:52 robert Exp $
 */

#ifndef INCLUDED_CR3DS_TYPES_H
#include <Plugin 3ds\crTypes.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Camera object
 * \ingroup camera
 */
struct _cr3dsCamera {
    cr3dsCamera *next;
    char name[64];
    cr3dsVector position;
    cr3dsVector target;
    cr3dsFloat roll;
    cr3dsFloat fov;
    cr3dsBool see_cone;
    cr3dsFloat near_range;
    cr3dsFloat far_range;
}; 

extern CR3DSAPI cr3dsCamera* cr_3ds_camera_new(const char *name);
extern CR3DSAPI void cr_3ds_camera_free(cr3dsCamera *mesh);
extern CR3DSAPI void cr_3ds_camera_dump(cr3dsCamera *camera);
extern CR3DSAPI cr3dsBool cr_3ds_camera_read(cr3dsCamera *camera, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_camera_write(cr3dsCamera *camera, FILE *f);

#ifdef __cplusplus
};
#endif
#endif

