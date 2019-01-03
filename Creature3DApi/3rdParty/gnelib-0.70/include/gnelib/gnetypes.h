#ifndef _GNETYPES_H_
#define _GNETYPES_H_

/* GNE - Game Networking Engine, a portable multithreaded networking library.
 * Copyright (C) 2001 Jason Winnebeck (gillius@mail.rit.edu)
 * Project website: http://www.rit.edu/~jpw9607/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

//Header file containing GNE-wide typedefs.

namespace GNE {
  /**
   * Typedefs for the variable types that are common to all GNE
   * implementations (as defined in the GNE protocol document).
   * "u" type variables denote unsigned versions.
   */
  typedef char  gint8;
  typedef unsigned char  guint8;
  typedef gint8          gbyte;

  /**
   * A gbool is 'false' only when its value is 0, else it is 'true'.
   */
  typedef gint8         gbool;

  /**
   * One possible true value for a gbool.
   */
  const gbool gTrue = 1;
  /**
   * The only false value for a gbool.
   */
  const gbool gFalse = 0;
  
  typedef short gint16;
  typedef unsigned short guint16;
  
  typedef int   gint32;
  typedef unsigned int   guint32;
  
  /**
   * A single precision 32-bit IEEE 754 floating point variable.
   */
  typedef float          gsingle;
  /**
   * A double precision 64-bit IEEE 754 floating point variable.
   */
  typedef double         gdouble;
};

#endif
