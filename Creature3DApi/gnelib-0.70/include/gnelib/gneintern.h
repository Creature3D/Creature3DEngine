#ifndef _GNEINTERN_H_
#define _GNEINTERN_H_

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

//This file is included in all .cpps in the library and is meant to be a
//pre-compiled header for compilers that support it.

#ifdef _MSC_VER
/*
 * Disable following warnings:
 *
 * warning C4786: identifier was truncated to '255' characters in the debug information
 */
#pragma warning( disable : 4786)
#endif

//Include system headers
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <sys/timeb.h>
#include <windows.h>
#include <process.h>
#endif

//Standard C includes -- for Console class and debugging functions
#include <cstdio>
#include <cstdarg>

//Include the HawkNL library
#include <nl.h>

//Standard C++ includes
//We don't include the iostream includes because not all compilers support
//pre-compiled headers, and the iostream library is quite large, and is used
//very sparsely (mainly just for the streambuf replacements in
//gstreambufs.cpp)
#include <string>
#include <map>
#include <queue>
#include <algorithm>

//Boost includes
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

//Other standard includes
#include <cassert>

#ifndef WIN32
#include <pthread.h>
#include <sched.h>
#endif

//GNE related includes:
#include "gnetypes.h"
//Define debugging functions
#include "GNEDebug.h"

namespace GNE {
  class ConnectionEventGenerator;
  template <class T> class SmartPtr;

  /**
   * The global event generator.  The library uses this internally to
   * register and unregister connections.  The end-user will not have to use
   * this object under any normal cirumstances.
   */
  extern SmartPtr<ConnectionEventGenerator> eGen;
};

#endif // _GNEINTERN_H_



