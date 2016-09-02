/* -*- c -*- */
#ifndef INCLUDED_CR3DS_VIEWPORT_H
#define INCLUDED_CR3DS_VIEWPORT_H
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
 * $Id: crViewport.h,v 1.4 2003/01/24 15:12:54 robert Exp $
 */

#ifndef INCLUDED_CR3DS_TYPES_H
#include <Plugin 3ds\crTypes.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Layout view types
 * \ingroup viewport
 */
typedef enum _cr3dsViewType {
  CR3DS_VIEW_TYPE_NOT_USED  =0,
  CR3DS_VIEW_TYPE_TOP       =1,
  CR3DS_VIEW_TYPE_BOTTOM    =2,
  CR3DS_VIEW_TYPE_LEFT      =3,
  CR3DS_VIEW_TYPE_RIGHT     =4,
  CR3DS_VIEW_TYPE_FRONT     =5,
  CR3DS_VIEW_TYPE_BACK      =6,
  CR3DS_VIEW_TYPE_USER      =7,
  CR3DS_VIEW_TYPE_SPOTLIGHT =18,
  CR3DS_VIEW_TYPE_CAMERA    =65535
} cr3dsViewType;

/*!
 * Layout view settings
 * \ingroup viewport
 */
typedef struct _cr3dsView {
    cr3dsWord type;
    cr3dsWord axis_lock;
    cr3dsIntw position[2];
    cr3dsIntw size[2];
    cr3dsFloat zoom;
    cr3dsVector center;
    cr3dsFloat horiz_angle;
    cr3dsFloat vert_angle;
    char camera[11];
} cr3dsView;

/*!
 * Layout styles
 * \ingroup viewport
 */
typedef enum _cr3dsLayoutStyle {
  CR3DS_LAYOUT_SINGLE                  =0,
  CR3DS_LAYOUT_TWO_PANE_VERT_SPLIT     =1,
  CR3DS_LAYOUT_TWO_PANE_HORIZ_SPLIT    =2,
  CR3DS_LAYOUT_FOUR_PANE               =3,
  CR3DS_LAYOUT_THREE_PANE_LEFT_SPLIT   =4,
  CR3DS_LAYOUT_THREE_PANE_BOTTOM_SPLIT =5,
  CR3DS_LAYOUT_THREE_PANE_RIGHT_SPLIT  =6,
  CR3DS_LAYOUT_THREE_PANE_TOP_SPLIT    =7,
  CR3DS_LAYOUT_THREE_PANE_VERT_SPLIT   =8,
  CR3DS_LAYOUT_THREE_PANE_HORIZ_SPLIT  =9,
  CR3DS_LAYOUT_FOUR_PANE_LEFT_SPLIT    =10,
  CR3DS_LAYOUT_FOUR_PANE_RIGHT_SPLIT   =11
} cr3dsLayoutStyle;

/*!
 * Viewport layout settings
 * \ingroup viewport
 */
typedef struct _cr3dsLayout {
    cr3dsWord style;
    cr3dsIntw active;
    cr3dsIntw swap;
    cr3dsIntw swap_prior;
    cr3dsIntw swap_view;
    cr3dsWord position[2];
    cr3dsWord size[2];
    cr3dsDword views;
    cr3dsView *viewL;
} cr3dsLayout;

/*!
 * Default view settings
 * \ingroup viewport
 */
typedef struct _cr3dsDefaultView {
    cr3dsWord type;
    cr3dsVector position;
    cr3dsFloat width;
    cr3dsFloat horiz_angle;
    cr3dsFloat vert_angle;
    cr3dsFloat roll_angle;
    char camera[64];
} cr3dsDefaultView;

/*!
 * Viewport and default view settings
 * \ingroup viewport
 */
struct _cr3dsViewport {
    cr3dsLayout layout;
    cr3dsDefaultView default_view;
};

extern CR3DSAPI cr3dsBool cr_3ds_viewport_read(cr3dsViewport *viewport, FILE *f);
extern CR3DSAPI void cr_3ds_viewport_set_views(cr3dsViewport *viewport, cr3dsDword views);
extern CR3DSAPI cr3dsBool cr_3ds_viewport_write(cr3dsViewport *viewport, FILE *f);

#ifdef __cplusplus
};
#endif
#endif




