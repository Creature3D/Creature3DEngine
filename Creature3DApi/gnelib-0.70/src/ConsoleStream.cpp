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

#include "../include/gnelib/gneintern.h"
#include "../include/gnelib/ConsoleStream.h"
#include "../include/gnelib/SynchronizedObject.h"
#include "../include/gnelib/Console.h"
#include "../include/gnelib/ConsoleStreambuf.h"
#include "../include/gnelib/Mutex.h"

namespace GNE {
namespace Console {

//--------------------------------
//
//  ConsoleMutex implementation
//
//--------------------------------
ConsoleMutex::ConsoleMutex(bool isAcquiring) 
: acq(isAcquiring) {
}

ConsoleMutex::~ConsoleMutex() {
}

void ConsoleMutex::action(GOut& o) const {
  action( (SynchronizedObject&)o );
}

void ConsoleMutex::action(SynchronizedObject& o) const {
  if ( acq )
    o.acquire();
  else
    o.release();
}

//--------------------------------
//
//     moveTo implementation
//
//--------------------------------
moveTo::moveTo(int xLoc, int yLoc) : x(xLoc), y(yLoc) {
}

moveTo::~moveTo() {
}

void moveTo::action(GOut& o) const {
  ((goutbuf*)(o.rdbuf()))->setNextWriteLoc(x, y);
}

}
}




