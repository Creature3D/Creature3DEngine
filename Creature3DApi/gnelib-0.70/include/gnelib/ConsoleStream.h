#ifndef _CONSOLESTREAM_H_
#define _CONSOLESTREAM_H_

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

namespace GNE {
class Mutex;
class SynchronizedObject;

namespace Console {
class GOut;

class ConsoleManipulator {
public:
  virtual ~ConsoleManipulator() {};

  virtual void action(GOut& o) const = 0;
};

/**
 * @ingroup console
 *
 * A class for syncronizing the gout stream or a ConsoleBuffer.  You won't
 * create this class directly, but instead will use the GNE::Console::acquire
 * and GNE::Console::release variables.
 */
class ConsoleMutex : public ConsoleManipulator {
public:
  ConsoleMutex(bool isAcquiring);
  virtual ~ConsoleMutex();

  void action(GOut& o) const;

  //Perform a release or an acquire, based on acq.
  void action(SynchronizedObject& o) const;

private:
  //acq is true if we are trying to acquire, false if release.
  bool acq;
};

/**
 * @ingroup console
 *
 * A manipulator for gout essentially meant to facilitate a C++ iostream
 * version of mlprintf.  Using moveTo in the gout stream (and ONLY gout),
 * will set the stream to display the next line-buffered text using an
 * mlprintf rather than a normal mprintf.  gout needs to be acquired if other
 * threads might write to gout.  The following code:
 *
 * <code>gout << acquire << moveTo(10, 15) << "Hello, World!" << flush << release;</code>
 * 
 * works like <code>mlprintf(10, 15, "Hello, World!");</code>.  Other stuff
 * may be before if anything is in the stream -- remember gout only displays
 * when it encounters a newline or is explicitly flushed.
 *
 * The position 0,0 is the upper left corner of the console.
 */
class moveTo : public ConsoleManipulator {
public:
  moveTo(int xLoc, int yLoc);
  virtual ~moveTo();

  void action(GOut& o) const;
private:
  moveTo();
  int x, y;
};

}
}

#endif
