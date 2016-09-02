/* -*- c -*- */
#ifndef INCLUDED_CR3DS_CHUNK_H
#define INCLUDED_CR3DS_CHUNK_H
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
 * $Id: crChunk.h,v 1.4 2003/01/24 15:12:52 robert Exp $
 */

#ifndef INCLUDED_CR3DS_TYPES_H
#include <Plugin 3ds\crTypes.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _cr3dsChunks {
  CR3DS_NULL_CHUNK             =0x0000,
  CR3DS_M3DMAGIC               =0x4D4D,    /*3DS file*/
  CR3DS_SMAGIC                 =0x2D2D,    
  CR3DS_LMAGIC                 =0x2D3D,    
  CR3DS_MLIBMAGIC              =0x3DAA,    /*MLI file*/
  CR3DS_MATMAGIC               =0x3DFF,    
  CR3DS_CMAGIC                 =0xC23D,    /*PRJ file*/
  CR3DS_M3D_VERSION            =0x0002,
  CR3DS_M3D_KFVERSION          =0x0005,

  CR3DS_COLOR_F                =0x0010,
  CR3DS_COLOR_24               =0x0011,
  CR3DS_LIN_COLOR_24           =0x0012,
  CR3DS_LIN_COLOR_F            =0x0013,
  CR3DS_INT_PERCENTAGE         =0x0030,
  CR3DS_FLOAT_PERCENTAGE       =0x0031,

  CR3DS_MDATA                  =0x3D3D,
  CR3DS_MESH_VERSION           =0x3D3E,
  CR3DS_MASTER_SCALE           =0x0100,
  CR3DS_LO_SHADOW_BIAS         =0x1400,
  CR3DS_HI_SHADOW_BIAS         =0x1410,
  CR3DS_SHADOW_MAP_SIZE        =0x1420,
  CR3DS_SHADOW_SAMPLES         =0x1430,
  CR3DS_SHADOW_RANGE           =0x1440,
  CR3DS_SHADOW_FILTER          =0x1450,
  CR3DS_RAY_BIAS               =0x1460,
  CR3DS_O_CONSTS               =0x1500,
  CR3DS_AMBIENT_LIGHT          =0x2100,
  CR3DS_BIT_MAP                =0x1100,
  CR3DS_SOLID_BGND             =0x1200,
  CR3DS_V_GRADIENT             =0x1300,
  CR3DS_USE_BIT_MAP            =0x1101,
  CR3DS_USE_SOLID_BGND         =0x1201,
  CR3DS_USE_V_GRADIENT         =0x1301,
  CR3DS_FOG                    =0x2200,
  CR3DS_FOG_BGND               =0x2210,
  CR3DS_LAYER_FOG              =0x2302,
  CR3DS_DISTANCE_CUE           =0x2300,
  CR3DS_DCUE_BGND              =0x2310,
  CR3DS_USE_FOG                =0x2201,
  CR3DS_USE_LAYER_FOG          =0x2303,
  CR3DS_USE_DISTANCE_CUE       =0x2301,

  CR3DS_MAT_ENTRY              =0xAFFF,
  CR3DS_MAT_NAME               =0xA000,
  CR3DS_MAT_AMBIENT            =0xA010,
  CR3DS_MAT_DIFFUSE            =0xA020,
  CR3DS_MAT_SPECULAR           =0xA030,
  CR3DS_MAT_SHININESS          =0xA040,
  CR3DS_MAT_SHIN2PCT           =0xA041,
  CR3DS_MAT_TRANSPARENCY       =0xA050,
  CR3DS_MAT_XPFALL             =0xA052,
  CR3DS_MAT_USE_XPFALL         =0xA240,
  CR3DS_MAT_REFBLUR            =0xA053,
  CR3DS_MAT_SHADING            =0xA100,
  CR3DS_MAT_USE_REFBLUR        =0xA250,
  CR3DS_MAT_SELF_ILLUM         =0xA080,
  CR3DS_MAT_TWO_SIDE           =0xA081,
  CR3DS_MAT_DECAL              =0xA082,
  CR3DS_MAT_ADDITIVE           =0xA083,
  CR3DS_MAT_WIRE               =0xA085,
  CR3DS_MAT_FACEMAP            =0xA088,
  CR3DS_MAT_PHONGSOFT          =0xA08C,
  CR3DS_MAT_WIREABS            =0xA08E,
  CR3DS_MAT_WIRE_SIZE          =0xA087,
  CR3DS_MAT_TEXMAP             =0xA200,
  CR3DS_MAT_SXP_TEXT_DATA      =0xA320,
  CR3DS_MAT_TEXMASK            =0xA33E,
  CR3DS_MAT_SXP_TEXTMASK_DATA  =0xA32A,
  CR3DS_MAT_TEX2MAP            =0xA33A,
  CR3DS_MAT_SXP_TEXT2_DATA     =0xA321,
  CR3DS_MAT_TEX2MASK           =0xA340,
  CR3DS_MAT_SXP_TEXT2MASK_DATA =0xA32C,
  CR3DS_MAT_OPACMAP            =0xA210,
  CR3DS_MAT_SXP_OPAC_DATA      =0xA322,
  CR3DS_MAT_OPACMASK           =0xA342,
  CR3DS_MAT_SXP_OPACMASK_DATA  =0xA32E,
  CR3DS_MAT_BUMPMAP            =0xA230,
  CR3DS_MAT_SXP_BUMP_DATA      =0xA324,
  CR3DS_MAT_BUMPMASK           =0xA344,
  CR3DS_MAT_SXP_BUMPMASK_DATA  =0xA330,
  CR3DS_MAT_SPECMAP            =0xA204,
  CR3DS_MAT_SXP_SPEC_DATA      =0xA325,
  CR3DS_MAT_SPECMASK           =0xA348,
  CR3DS_MAT_SXP_SPECMASK_DATA  =0xA332,
  CR3DS_MAT_SHINMAP            =0xA33C,
  CR3DS_MAT_SXP_SHIN_DATA      =0xA326,
  CR3DS_MAT_SHINMASK           =0xA346,
  CR3DS_MAT_SXP_SHINMASK_DATA  =0xA334,
  CR3DS_MAT_SELFIMAP           =0xA33D,
  CR3DS_MAT_SXP_SELFI_DATA     =0xA328,
  CR3DS_MAT_SELFIMASK          =0xA34A,
  CR3DS_MAT_SXP_SELFIMASK_DATA =0xA336,
  CR3DS_MAT_REFLMAP            =0xA220,
  CR3DS_MAT_REFLMASK           =0xA34C,
  CR3DS_MAT_SXP_REFLMASK_DATA  =0xA338,
  CR3DS_MAT_ACUBIC             =0xA310,
  CR3DS_MAT_MAPNAME            =0xA300,
  CR3DS_MAT_MAP_TILING         =0xA351,
  CR3DS_MAT_MAP_TEXBLUR        =0xA353,
  CR3DS_MAT_MAP_USCALE         =0xA354,
  CR3DS_MAT_MAP_VSCALE         =0xA356,
  CR3DS_MAT_MAP_UOFFSET        =0xA358,
  CR3DS_MAT_MAP_VOFFSET        =0xA35A,
  CR3DS_MAT_MAP_ANG            =0xA35C,
  CR3DS_MAT_MAP_COL1           =0xA360,
  CR3DS_MAT_MAP_COL2           =0xA362,
  CR3DS_MAT_MAP_RCOL           =0xA364,
  CR3DS_MAT_MAP_GCOL           =0xA366,
  CR3DS_MAT_MAP_BCOL           =0xA368,

  CR3DS_NAMED_OBJECT           =0x4000,
  CR3DS_N_DIRECT_LIGHT         =0x4600,
  CR3DS_DL_OFF                 =0x4620,
  CR3DS_DL_OUTER_RANGE         =0x465A,
  CR3DS_DL_INNER_RANGE         =0x4659,
  CR3DS_DL_MULTIPLIER          =0x465B,
  CR3DS_DL_EXCLUDE             =0x4654,
  CR3DS_DL_ATTENUATE           =0x4625,
  CR3DS_DL_SPOTLIGHT           =0x4610,
  CR3DS_DL_SPOT_ROLL           =0x4656,
  CR3DS_DL_SHADOWED            =0x4630,
  CR3DS_DL_LOCAL_SHADOW2       =0x4641,
  CR3DS_DL_SEE_CONE            =0x4650,
  CR3DS_DL_SPOT_RECTANGULAR    =0x4651,
  CR3DS_DL_SPOT_ASPECT         =0x4657,
  CR3DS_DL_SPOT_PROJECTOR      =0x4653,
  CR3DS_DL_SPOT_OVERSHOOT      =0x4652,
  CR3DS_DL_RAY_BIAS            =0x4658,
  CR3DS_DL_RAYSHAD             =0x4627,
  CR3DS_N_CAMERA               =0x4700,
  CR3DS_CAM_SEE_CONE           =0x4710,
  CR3DS_CAM_RANGES             =0x4720,
  CR3DS_OBJ_HIDDEN             =0x4010,
  CR3DS_OBJ_VIS_LOFTER         =0x4011,
  CR3DS_OBJ_DOESNT_CAST        =0x4012,
  CR3DS_OBJ_DONT_RECVSHADOW    =0x4017,
  CR3DS_OBJ_MATTE              =0x4013,
  CR3DS_OBJ_FAST               =0x4014,
  CR3DS_OBJ_PROCEDURAL         =0x4015,
  CR3DS_OBJ_FROZEN             =0x4016,
  CR3DS_N_TRI_OBJECT           =0x4100,
  CR3DS_POINT_ARRAY            =0x4110,
  CR3DS_POINT_FLAG_ARRAY       =0x4111,
  CR3DS_FACE_ARRAY             =0x4120,
  CR3DS_MSH_MAT_GROUP          =0x4130,
  CR3DS_SMOOTH_GROUP           =0x4150,
  CR3DS_MSH_BOXMAP             =0x4190,
  CR3DS_TEX_VERTS              =0x4140,
  CR3DS_MESH_MATRIX            =0x4160,
  CR3DS_MESH_COLOR             =0x4165,
  CR3DS_MESH_TEXTURE_INFO      =0x4170,

  CR3DS_KFDATA                 =0xB000,
  CR3DS_KFHDR                  =0xB00A,
  CR3DS_KFSEG                  =0xB008,
  CR3DS_KFCURTIME              =0xB009,
  CR3DS_AMBIENT_NODE_TAG       =0xB001,
  CR3DS_OBJECT_NODE_TAG        =0xB002,
  CR3DS_CAMERA_NODE_TAG        =0xB003,
  CR3DS_TARGET_NODE_TAG        =0xB004,
  CR3DS_LIGHT_NODE_TAG         =0xB005,
  CR3DS_L_TARGET_NODE_TAG      =0xB006,
  CR3DS_SPOTLIGHT_NODE_TAG     =0xB007,
  CR3DS_NODE_ID                =0xB030,
  CR3DS_NODE_HDR               =0xB010,
  CR3DS_PIVOT                  =0xB013,
  CR3DS_INSTANCE_NAME          =0xB011,
  CR3DS_MORPH_SMOOTH           =0xB015,
  CR3DS_BOUNDBOX               =0xB014,
  CR3DS_POS_TRACK_TAG          =0xB020,
  CR3DS_COL_TRACK_TAG          =0xB025,
  CR3DS_ROT_TRACK_TAG          =0xB021,
  CR3DS_SCL_TRACK_TAG          =0xB022,
  CR3DS_MORPH_TRACK_TAG        =0xB026,
  CR3DS_FOV_TRACK_TAG          =0xB023,
  CR3DS_ROLL_TRACK_TAG         =0xB024,
  CR3DS_HOT_TRACK_TAG          =0xB027,
  CR3DS_FALL_TRACK_TAG         =0xB028,
  CR3DS_HIDE_TRACK_TAG         =0xB029,

  CR3DS_POLY_2D                = 0x5000,
  CR3DS_SHAPE_OK               = 0x5010,
  CR3DS_SHAPE_NOT_OK           = 0x5011,
  CR3DS_SHAPE_HOOK             = 0x5020,
  CR3DS_PATH_3D                = 0x6000,
  CR3DS_PATH_MATRIX            = 0x6005,
  CR3DS_SHAPE_2D               = 0x6010,
  CR3DS_M_SCALE                = 0x6020,
  CR3DS_M_TWIST                = 0x6030,
  CR3DS_M_TEETER               = 0x6040,
  CR3DS_M_FIT                  = 0x6050,
  CR3DS_M_BEVEL                = 0x6060,
  CR3DS_XZ_CURVE               = 0x6070,
  CR3DS_YZ_CURVE               = 0x6080,
  CR3DS_INTERPCT               = 0x6090,
  CR3DS_DEFORM_LIMIT           = 0x60A0,

  CR3DS_USE_CONTOUR            = 0x6100,
  CR3DS_USE_TWEEN              = 0x6110,
  CR3DS_USE_SCALE              = 0x6120,
  CR3DS_USE_TWIST              = 0x6130,
  CR3DS_USE_TEETER             = 0x6140,
  CR3DS_USE_FIT                = 0x6150,
  CR3DS_USE_BEVEL              = 0x6160,

  CR3DS_DEFAULT_VIEW           = 0x3000,
  CR3DS_VIEW_TOP               = 0x3010,
  CR3DS_VIEW_BOTTOM            = 0x3020,
  CR3DS_VIEW_LEFT              = 0x3030,
  CR3DS_VIEW_RIGHT             = 0x3040,
  CR3DS_VIEW_FRONT             = 0x3050,
  CR3DS_VIEW_BACK              = 0x3060,
  CR3DS_VIEW_USER              = 0x3070,
  CR3DS_VIEW_CAMERA            = 0x3080,
  CR3DS_VIEW_WINDOW            = 0x3090,

  CR3DS_VIEWPORT_LAYOUT_OLD    = 0x7000,
  CR3DS_VIEWPORT_DATA_OLD      = 0x7010,
  CR3DS_VIEWPORT_LAYOUT        = 0x7001,
  CR3DS_VIEWPORT_DATA          = 0x7011,
  CR3DS_VIEWPORT_DATA_3        = 0x7012,
  CR3DS_VIEWPORT_SIZE          = 0x7020,
  CR3DS_NETWORK_VIEW           = 0x7030
} cr3dsChunks;

typedef struct _cr3dsChunk {
    cr3dsWord chunk;
    cr3dsDword size;
    cr3dsDword end;
    cr3dsDword cur;
} cr3dsChunk; 

extern CR3DSAPI void cr_3ds_chunk_enable_dump(cr3dsBool enable, cr3dsBool unknown);
extern CR3DSAPI cr3dsBool cr_3ds_chunk_read(cr3dsChunk *c, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_chunk_read_start(cr3dsChunk *c, cr3dsWord chunk, FILE *f);
extern CR3DSAPI void cr_3ds_chunk_read_tell(cr3dsChunk *c, FILE *f);
extern CR3DSAPI cr3dsWord cr_3ds_chunk_read_next(cr3dsChunk *c, FILE *f);
extern CR3DSAPI void cr_3ds_chunk_read_reset(cr3dsChunk *c, FILE *f);
extern CR3DSAPI void cr_3ds_chunk_read_end(cr3dsChunk *c, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_chunk_write(cr3dsChunk *c, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_chunk_write_start(cr3dsChunk *c, FILE *f);
extern CR3DSAPI cr3dsBool cr_3ds_chunk_write_end(cr3dsChunk *c, FILE *f);
extern CR3DSAPI const char* cr_3ds_chunk_name(cr3dsWord chunk);
extern CR3DSAPI void cr_3ds_chunk_unknown(cr3dsWord chunk);
extern CR3DSAPI void cr_3ds_chunk_dump_info(const char *format, ...);

#ifdef __cplusplus
};
#endif
#endif


