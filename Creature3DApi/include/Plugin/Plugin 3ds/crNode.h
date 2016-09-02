/* -*- c -*- */
#ifndef INCLUDED_CR3DS_NODE_H
#define INCLUDED_CR3DS_NODE_H
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
 * $Id: crNode.h,v 1.4 2003/01/24 15:12:54 robert Exp $
 */

#ifndef INCLUDED_CR3DS_TRACKS_H
#include <Plugin 3ds\crTracks.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Scene graph ambient color node data
 * \ingroup node
 */
typedef struct _cr3dsAmbientData {
    cr3dsRgb col;
    cr3dsLin3Track col_track;
} cr3dsAmbientData;

/*!
 * Scene graph object instance node data
 * \ingroup node
 */
typedef struct _cr3dsObjectData {
    cr3dsVector pivot;
    char instance[64];
    cr3dsVector bbox_min;
    cr3dsVector bbox_max;
    cr3dsVector pos;
    cr3dsLin3Track pos_track;
    cr3dsQuat rot;
    cr3dsQuatTrack rot_track;
    cr3dsVector scl;
    cr3dsLin3Track scl_track;
    cr3dsFloat morph_smooth;
    char morph[64];
    cr3dsMorphTrack morph_track;
    cr3dsBool hide;
    cr3dsBoolTrack hide_track;
} cr3dsObjectData;

/*!
 * Scene graph camera node data
 * \ingroup node
 */
typedef struct _cr3dsCameraData {
    cr3dsVector pos;
    cr3dsLin3Track pos_track;
    cr3dsFloat fov;
    cr3dsLin1Track fov_track;
    cr3dsFloat roll;
    cr3dsLin1Track roll_track;
} cr3dsCameraData;

/*!
 * Scene graph camera target node data
 * \ingroup node
 */
typedef struct _cr3dsTargetData {
    cr3dsVector pos;
    cr3dsLin3Track pos_track;
} cr3dsTargetData;

/*!
 * Scene graph light node data
 * \ingroup node
 */
typedef struct _cr3dsLightData {
    cr3dsVector pos;
    cr3dsLin3Track pos_track;
    cr3dsRgb col;
    cr3dsLin3Track col_track;
    cr3dsFloat hotspot;
    cr3dsLin1Track hotspot_track;
    cr3dsFloat falloff;
    cr3dsLin1Track falloff_track;
    cr3dsFloat roll;
    cr3dsLin1Track roll_track;
} cr3dsLightData;

/*!
 * Scene graph spotlight target node data
 * \ingroup node
 */
typedef struct _cr3dsSpotData {
    cr3dsVector pos;
    cr3dsLin3Track pos_track;
} cr3dsSpotData;

/*!
 * Scene graph node data union
 * \ingroup node
 */
typedef union _cr3dsNodeData {
    cr3dsAmbientData ambient;
    cr3dsObjectData object;
    cr3dsCameraData camera;
    cr3dsTargetData target;
    cr3dsLightData light;
    cr3dsSpotData spot;
} cr3dsNodeData;

/*!
 * \ingroup node
 */
#define CR3DS_NO_PARENT 65535

/*!
 * Scene graph node
 * \ingroup node
 */
struct _cr3dsNode {
    cr3dsUserData user;
    cr3dsNode *next;\
    cr3dsNode *childs;\
    cr3dsNode *parent;\
    cr3dsNodeTypes type;\
    cr3dsWord node_id;\
    char name[64];\
    cr3dsWord flags1;\
    cr3dsWord flags2;\
    cr3dsWord parent_id;
    cr3dsMatrix matrix;
    cr3dsNodeData data;
};

extern CR3DSAPI cr3dsNode* cr_3ds_node_new_ambient();
extern CR3DSAPI cr3dsNode* cr_3ds_node_new_object();
extern CR3DSAPI cr3dsNode* cr_3ds_node_new_camera();
extern CR3DSAPI cr3dsNode* cr_3ds_node_new_target();
extern CR3DSAPI cr3dsNode* cr_3ds_node_new_light();
extern CR3DSAPI cr3dsNode* cr_3ds_node_new_spot();
extern CR3DSAPI void cr_3ds_node_free(cr3dsNode *node);
extern CR3DSAPI void cr_3ds_node_eval(cr3dsNode *node, cr3dsFloat t);
extern CR3DSAPI cr3dsNode* cr_3ds_node_by_name(cr3dsNode *node, const char* name,
  cr3dsNodeTypes type);
extern CR3DSAPI cr3dsNode* cr_3ds_node_by_id(cr3dsNode *node, cr3dsWord node_id);
extern CR3DSAPI void cr_3ds_node_dump(cr3dsNode *node, cr3dsIntd level);
extern CR3DSAPI cr3dsBool cr_3ds_node_read(cr3dsNode *node, cr3dsFile *file, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_node_write(cr3dsNode *node, cr3dsFile *file, FILE *f);

#ifdef __cplusplus
};
#endif
#endif

