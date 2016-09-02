/* -*- c -*- */
#ifndef INCLUDED_CR3DS_MESH_H
#define INCLUDED_CR3DS_MESH_H
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
 * $Id: crMesh.h,v 1.4 2003/01/24 15:12:53 robert Exp $
 */

#ifndef INCLUDED_CR3DS_TYPES_H
#include <Plugin 3ds\crTypes.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Triangular mesh point
 * \ingroup mesh
 */
typedef struct _cr3dsPoint {
    cr3dsVector pos;
} cr3dsPoint;

/*!
 * Triangular mesh face
 * \ingroup mesh
 */
struct _cr3dsFace {
    cr3dsUserData user;
    char material[64];
    cr3dsWord points[3];
    cr3dsWord flags;
    cr3dsDword smoothing;
    cr3dsVector normal;
};

/*!
 * Triangular mesh box mapping settings
 * \ingroup mesh
 */
struct _cr3dsBoxMap {
    char front[64];
    char back[64];
    char left[64];
    char right[64];
    char top[64];
    char bottom[64];
};

/*!
 * cr3dsMapData maptype
 * \ingroup tracks
 */
typedef enum {
  CR3DS_MAP_NONE        =0xFFFF,
  CR3DS_MAP_PLANAR      =0,
  CR3DS_MAP_CYLINDRICAL =1,
  CR3DS_MAP_SPHERICAL   =2
} cr3dsMapType;

/*!
 * Triangular mesh texture mapping data
 * \ingroup mesh
 */
struct _cr3dsMapData {
    cr3dsWord maptype;
    cr3dsVector pos;
    cr3dsMatrix matrix;
    cr3dsFloat scale;
    cr3dsFloat tile[2];
    cr3dsFloat planar_size[2];
    cr3dsFloat cylinder_height;
};

/*!
 * Triangular mesh object
 * \ingroup mesh
 */
struct _cr3dsMesh {
    cr3dsUserData user;
    cr3dsMesh *next;
    char name[64];
    cr3dsByte color;
    cr3dsMatrix matrix;
    cr3dsDword points;
    cr3dsPoint *pointL;
    cr3dsDword flags;
    cr3dsWord *flagL;
    cr3dsDword texels;
    cr3dsTexel *texelL;
    cr3dsDword faces;
    cr3dsFace *faceL;
    cr3dsBoxMap box_map;
    cr3dsMapData map_data;
}; 

extern CR3DSAPI cr3dsMesh* cr_3ds_mesh_new(const char *name);
extern CR3DSAPI void cr_3ds_mesh_free(cr3dsMesh *mesh);
extern CR3DSAPI cr3dsBool cr_3ds_mesh_new_point_list(cr3dsMesh *mesh, cr3dsDword points);
extern CR3DSAPI void cr_3ds_mesh_free_point_list(cr3dsMesh *mesh);
extern CR3DSAPI cr3dsBool cr_3ds_mesh_new_flag_list(cr3dsMesh *mesh, cr3dsDword flags);
extern CR3DSAPI void cr_3ds_mesh_free_flag_list(cr3dsMesh *mesh);
extern CR3DSAPI cr3dsBool cr_3ds_mesh_new_texel_list(cr3dsMesh *mesh, cr3dsDword texels);
extern CR3DSAPI void cr_3ds_mesh_free_texel_list(cr3dsMesh *mesh);
extern CR3DSAPI cr3dsBool cr_3ds_mesh_new_face_list(cr3dsMesh *mesh, cr3dsDword flags);
extern CR3DSAPI void cr_3ds_mesh_free_face_list(cr3dsMesh *mesh);
extern CR3DSAPI void cr_3ds_mesh_bounding_box(cr3dsMesh *mesh, cr3dsVector min, cr3dsVector max);
extern CR3DSAPI void cr_3ds_mesh_calculate_normals(cr3dsMesh *mesh, cr3dsVector *normalL);
extern CR3DSAPI void cr_3ds_mesh_dump(cr3dsMesh *mesh);
extern CR3DSAPI cr3dsBool cr_3ds_mesh_read(cr3dsMesh *mesh, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_mesh_write(cr3dsMesh *mesh, FILE *f);

#ifdef __cplusplus
};
#endif
#endif

