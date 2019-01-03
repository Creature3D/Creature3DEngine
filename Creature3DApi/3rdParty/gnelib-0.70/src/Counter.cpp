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
#include "../include/gnelib/Counter.h"

namespace GNE {

Counter::Counter() : count(0) {
}

Counter::sptr Counter::create() {
  return sptr( new Counter() );
}

Counter::~Counter() {
}

void Counter::timerCallback() {
  countSync.acquire();
  ++count;
  countSync.broadcast();
  countSync.release();
}

int Counter::getCount() {
  return count;
}

void Counter::setCount(int newCount) {
  countSync.acquire();
  count = newCount;
  countSync.broadcast();
  countSync.release();
}

void Counter::adjustCount(int val) {
  countSync.acquire();
  count += val;
  countSync.broadcast();
  countSync.release();
}

void Counter::waitAndDecrement() {
  countSync.acquire();
  while (count <= 0) {
    countSync.wait();
  }
  --count;
  countSync.release();
}

}
