/* -*- c -*- */
#ifndef INCLUDED_CR3DS_TRACKS_H
#define INCLUDED_CR3DS_TRACKS_H
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
 * $Id: crTracks.h,v 1.4 2003/01/24 15:12:54 robert Exp $
 */

#ifndef INCLUDED_CR3DS_TCB_H
#include <Plugin 3ds\crTcb.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Track flags
 * \ingroup tracks
 */
typedef enum {
  CR3DS_REPEAT    =0x0001,
  CR3DS_SMOOTH    =0x0002,
  CR3DS_LOCK_X    =0x0008,
  CR3DS_LOCK_Y    =0x0010,
  CR3DS_LOCK_Z    =0x0020,
  CR3DS_UNLINK_X  =0x0100,
  CR3DS_UNLINK_Y  =0x0200,
  CR3DS_UNLINK_Z  =0x0400
} cr3dsTrackFlags;

/*!
 * Boolean track key
 * \ingroup tracks
 */
struct _cr3dsBoolKey {
    cr3dsTcb tcb;
    cr3dsBoolKey *next;
};

/*!
 * Boolean track
 * \ingroup tracks
 */
struct _cr3dsBoolTrack {
    cr3dsDword flags;
    cr3dsBoolKey *keyL;
};

/*!
 * Floating-point track key
 * \ingroup tracks
 */
struct _cr3dsLin1Key {
    cr3dsTcb tcb;
    cr3dsLin1Key *next;
    cr3dsFloat value;
    cr3dsFloat dd;
    cr3dsFloat ds;
};
  
/*!
 * Floating-point track
 * \ingroup tracks
 */
struct _cr3dsLin1Track {
    cr3dsDword flags;
    cr3dsLin1Key *keyL;
};

/*!
 * Vector track key
 * \ingroup tracks
 */
struct _cr3dsLin3Key {
    cr3dsTcb tcb;
    cr3dsLin3Key *next;  
    cr3dsVector value;
    cr3dsVector dd;
    cr3dsVector ds;
};
  
/*!
 * Vector track
 * \ingroup tracks
 */
struct _cr3dsLin3Track {
    cr3dsDword flags;
    cr3dsLin3Key *keyL;
};

/*!
 * Rotation track key
 * \ingroup tracks
 */
struct _cr3dsQuatKey {
    cr3dsTcb tcb;
    cr3dsQuatKey *next;  
    cr3dsVector axis;
    cr3dsFloat angle;
    cr3dsQuat q;
    cr3dsQuat dd;
    cr3dsQuat ds;
};
  
/*!
 * Rotation track 
 * \ingroup tracks
 */
struct _cr3dsQuatTrack {
    cr3dsDword flags;
    cr3dsQuatKey *keyL;
};

/*!
 * Morph track key
 * \ingroup tracks
 */
struct _cr3dsMorphKey {
    cr3dsTcb tcb;
    cr3dsMorphKey *next;  
    char name[64];
};
  
/*!
 * Morph track
 * \ingroup tracks
 */
struct _cr3dsMorphTrack {
    cr3dsDword flags;
    cr3dsMorphKey *keyL;
};

extern CR3DSAPI cr3dsBoolKey* cr_3ds_bool_key_new();
extern CR3DSAPI void cr_3ds_bool_key_free(cr3dsBoolKey* key);
extern CR3DSAPI void cr_3ds_bool_track_free_keys(cr3dsBoolTrack *track);
extern CR3DSAPI void cr_3ds_bool_track_insert(cr3dsBoolTrack *track, cr3dsBoolKey* key);
extern CR3DSAPI void cr_3ds_bool_track_remove(cr3dsBoolTrack *track, cr3dsIntd frame);
extern CR3DSAPI void cr_3ds_bool_track_eval(cr3dsBoolTrack *track, cr3dsBool *p, cr3dsFloat t);
extern CR3DSAPI cr3dsBool cr_3ds_bool_track_read(cr3dsBoolTrack *track, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_bool_track_write(cr3dsBoolTrack *track, FILE *f);

extern CR3DSAPI cr3dsLin1Key* cr_3ds_lin1_key_new();
extern CR3DSAPI void cr_3ds_lin1_key_free(cr3dsLin1Key* key);
extern CR3DSAPI void cr_3ds_lin1_track_free_keys(cr3dsLin1Track *track);
extern CR3DSAPI void cr_3ds_lin1_key_setup(cr3dsLin1Key *p, cr3dsLin1Key *cp, cr3dsLin1Key *c,
  cr3dsLin1Key *cn, cr3dsLin1Key *n);
extern CR3DSAPI void cr_3ds_lin1_track_setup(cr3dsLin1Track *track);
extern CR3DSAPI void cr_3ds_lin1_track_insert(cr3dsLin1Track *track, cr3dsLin1Key *key);
extern CR3DSAPI void cr_3ds_lin1_track_remove(cr3dsLin1Track *track, cr3dsIntd frame);
extern CR3DSAPI void cr_3ds_lin1_track_eval(cr3dsLin1Track *track, cr3dsFloat *p, cr3dsFloat t);
extern CR3DSAPI cr3dsBool cr_3ds_lin1_track_read(cr3dsLin1Track *track, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_lin1_track_write(cr3dsLin1Track *track, FILE *f);

extern CR3DSAPI cr3dsLin3Key* cr_3ds_lin3_key_new();
extern CR3DSAPI void cr_3ds_lin3_key_free(cr3dsLin3Key* key);
extern CR3DSAPI void cr_3ds_lin3_track_free_keys(cr3dsLin3Track *track);
extern CR3DSAPI void cr_3ds_lin3_key_setup(cr3dsLin3Key *p, cr3dsLin3Key *cp, cr3dsLin3Key *c,
  cr3dsLin3Key *cn, cr3dsLin3Key *n);
extern CR3DSAPI void cr_3ds_lin3_track_setup(cr3dsLin3Track *track);
extern CR3DSAPI void cr_3ds_lin3_track_insert(cr3dsLin3Track *track, cr3dsLin3Key *key);
extern CR3DSAPI void cr_3ds_lin3_track_remove(cr3dsLin3Track *track, cr3dsIntd frame);
extern CR3DSAPI void cr_3ds_lin3_track_eval(cr3dsLin3Track *track, cr3dsVector p, cr3dsFloat t);
extern CR3DSAPI cr3dsBool cr_3ds_lin3_track_read(cr3dsLin3Track *track, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_lin3_track_write(cr3dsLin3Track *track, FILE *f);

extern CR3DSAPI cr3dsQuatKey* cr_3ds_quat_key_new();
extern CR3DSAPI void cr_3ds_quat_key_free(cr3dsQuatKey* key);
extern CR3DSAPI void cr_3ds_quat_track_free_keys(cr3dsQuatTrack *track);
extern CR3DSAPI void cr_3ds_quat_key_setup(cr3dsQuatKey *p, cr3dsQuatKey *cp, cr3dsQuatKey *c,
  cr3dsQuatKey *cn, cr3dsQuatKey *n);
extern CR3DSAPI void cr_3ds_quat_track_setup(cr3dsQuatTrack *track);
extern CR3DSAPI void cr_3ds_quat_track_insert(cr3dsQuatTrack *track, cr3dsQuatKey *key);
extern CR3DSAPI void cr_3ds_quat_track_remove(cr3dsQuatTrack *track, cr3dsIntd frame);
extern CR3DSAPI void cr_3ds_quat_track_eval(cr3dsQuatTrack *track, cr3dsQuat p, cr3dsFloat t);
extern CR3DSAPI cr3dsBool cr_3ds_quat_track_read(cr3dsQuatTrack *track, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_quat_track_write(cr3dsQuatTrack *track, FILE *f);

extern CR3DSAPI cr3dsMorphKey* cr_3ds_morph_key_new();
extern CR3DSAPI void cr_3ds_morph_key_free(cr3dsMorphKey* key);
extern CR3DSAPI void cr_3ds_morph_track_free_keys(cr3dsMorphTrack *track);
extern CR3DSAPI void cr_3ds_morph_track_insert(cr3dsMorphTrack *track, cr3dsMorphKey *key);
extern CR3DSAPI void cr_3ds_morph_track_remove(cr3dsMorphTrack *track, cr3dsIntd frame);
extern CR3DSAPI void cr_3ds_morph_track_eval(cr3dsMorphTrack *track, char *p, cr3dsFloat t);
extern CR3DSAPI cr3dsBool cr_3ds_morph_track_read(cr3dsMorphTrack *track, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_morph_track_write(cr3dsMorphTrack *track, FILE *f);

#ifdef __cplusplus
};
#endif
#endif

