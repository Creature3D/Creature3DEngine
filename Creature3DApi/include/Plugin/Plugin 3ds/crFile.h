/* -*- c -*- */
#ifndef INCLUDED_CR3DS_FILE_H
#define INCLUDED_CR3DS_FILE_H
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
 * $Id: crFile.h,v 1.4 2003/01/21 21:02:17 robert Exp $
 */

#ifndef INCLUDED_CR3DS_BACKGROUND_H
#include <Plugin 3ds\crBackground.h>
#endif
#ifndef INCLUDED_CR3DS_ATMOSPHERE_H
#include <Plugin 3ds\crAtmosphere.h>
#endif
#ifndef INCLUDED_CR3DS_SHADOW_H
#include <Plugin 3ds\crShadow.h>
#endif
#ifndef INCLUDED_CR3DS_VIEWPORT_H
#include <Plugin 3ds\crViewport.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * 3ds file structure
 * \ingroup file
 */
struct _cr3dsFile {
    cr3dsDword mesh_version;
    cr3dsWord keyf_revision;
    char name[12+1];
    cr3dsFloat master_scale;
    cr3dsVector construction_plane;
    cr3dsRgb ambient;
    cr3dsShadow shadow;
    cr3dsBackground background;
    cr3dsAtmosphere atmosphere;
    cr3dsViewport viewport;
    cr3dsViewport viewport_keyf;
    cr3dsIntd frames;
    cr3dsIntd segment_from;
    cr3dsIntd segment_to;
    cr3dsIntd current_frame;
    cr3dsMaterial *materials;
    cr3dsMesh *meshes;
    cr3dsCamera *cameras;
    cr3dsLight *lights;
    cr3dsNode *nodes;
}; 

extern CR3DSAPI cr3dsFile* cr_3ds_file_load(const char *filename);
extern CR3DSAPI cr3dsBool cr_3ds_file_save(cr3dsFile *file, const char *filename);
extern CR3DSAPI cr3dsFile* cr_3ds_file_new();
extern CR3DSAPI void cr_3ds_file_free(cr3dsFile *file);
extern CR3DSAPI void cr_3ds_file_eval(cr3dsFile *file, cr3dsFloat t);
extern CR3DSAPI cr3dsBool cr_3ds_file_read(cr3dsFile *file, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_file_write(cr3dsFile *file, FILE *f);
extern CR3DSAPI void cr_3ds_file_insert_material(cr3dsFile *file, cr3dsMaterial *material);
extern CR3DSAPI void cr_3ds_file_remove_material(cr3dsFile *file, cr3dsMaterial *material);
extern CR3DSAPI cr3dsMaterial* cr_3ds_file_material_by_name(cr3dsFile *file, const char *name);
extern CR3DSAPI void cr_3ds_file_dump_materials(cr3dsFile *file);
extern CR3DSAPI void cr_3ds_file_insert_mesh(cr3dsFile *file, cr3dsMesh *mesh);
extern CR3DSAPI void cr_3ds_file_remove_mesh(cr3dsFile *file, cr3dsMesh *mesh);
extern CR3DSAPI cr3dsMesh* cr_3ds_file_mesh_by_name(cr3dsFile *file, const char *name);
extern CR3DSAPI void cr_3ds_file_dump_meshes(cr3dsFile *file);
extern CR3DSAPI void cr_3ds_file_dump_instances(cr3dsFile *file);
extern CR3DSAPI void cr_3ds_file_insert_camera(cr3dsFile *file, cr3dsCamera *camera);
extern CR3DSAPI void cr_3ds_file_remove_camera(cr3dsFile *file, cr3dsCamera *camera);
extern CR3DSAPI cr3dsCamera* cr_3ds_file_camera_by_name(cr3dsFile *file, const char *name);
extern CR3DSAPI void cr_3ds_file_dump_cameras(cr3dsFile *file);
extern CR3DSAPI void cr_3ds_file_insert_light(cr3dsFile *file, cr3dsLight *light);
extern CR3DSAPI void cr_3ds_file_remove_light(cr3dsFile *file, cr3dsLight *light);
extern CR3DSAPI cr3dsLight* cr_3ds_file_light_by_name(cr3dsFile *file, const char *name);
extern CR3DSAPI void cr_3ds_file_dump_lights(cr3dsFile *file);
extern CR3DSAPI cr3dsNode* cr_3ds_file_node_by_name(cr3dsFile *file, const char* name,
  cr3dsNodeTypes type);
extern CR3DSAPI cr3dsNode* cr_3ds_file_node_by_id(cr3dsFile *file, cr3dsWord node_id);
extern CR3DSAPI void cr_3ds_file_insert_node(cr3dsFile *file, cr3dsNode *node);
extern CR3DSAPI cr3dsBool cr_3ds_file_remove_node(cr3dsFile *file, cr3dsNode *node);
extern CR3DSAPI void cr_3ds_file_dump_nodes(cr3dsFile *file);

#ifdef __cplusplus
};
#endif
#endif

