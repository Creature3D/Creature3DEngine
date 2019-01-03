#ifndef _OLDCONSOLESTREAMBUF_H_
#define _OLDCONSOLESTREAMBUF_H_

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

//This header is a version of the Console streams to work with the old GNU
//libstdc++ that most people are using now with gcc 2.9x.  MSVC, Sun CC, and
//the newest gcc 3 and libstdc++ can compile the normal code.

#include <streambuf.h>

namespace GNE {
namespace Console {

class ginbuf : public std::streambuf {
public:
  ginbuf();
  ~ginbuf();

protected:
  int underflow();

private:
  char* buf;
};

class goutbuf : public std::streambuf {
public:
  goutbuf();
  ~goutbuf();

  void setNextWriteLoc(int x, int y);

protected:
  int sync();
  void flush_output();
  int overflow(int meta = EOF);
  std::streamsize xsputn(const char *ptr, std::streamsize count);

private:
  char* buf;

  int x;
  int y;
};

}
}

#endif //#ifndef _OLDCONSOLESTREAMBUF_H_
