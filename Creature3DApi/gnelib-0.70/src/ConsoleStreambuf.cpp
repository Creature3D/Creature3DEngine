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

#ifdef OLD_CPP
#include "OldConsoleStreambuf.inc"
#else
#include "../include/gnelib/gneintern.h"
#include "../include/gnelib/ConsoleStreambuf.h"
#include "../include/gnelib/Console.h"
#include <streambuf>

namespace GNE {
namespace Console {

//--------------------------------
//
//    ginbuf implementation
//
//--------------------------------
const int IBUF_LEN = 128;

ginbuf::ginbuf() {
  buf = new char[IBUF_LEN];
  setg(buf, buf + IBUF_LEN, buf + IBUF_LEN);
}

ginbuf::~ginbuf() {
  delete[] buf;
}

ginbuf::int_type ginbuf::underflow() {
  //If there is data still in the input buffer, return it.
  if (gptr() < egptr())
    return traits_type::to_int_type(*gptr());
  
  //else get some more data, and leave room for '\n'
  GNE::Console::getString(buf, IBUF_LEN-2);
  GNE::Console::mputchar('\n'); //Advance the line ourselves, since getString
                                //doesn't.

  //We have to append the newline ourselves
  int x = strlen(buf);
  buf[x] = '\n';
  x++;
  buf[x] = '\0';
  
  //Set the input buffer to the string read:
  setg(buf, buf, buf + x);
  
  //Return the next character ready to be read.
  return traits_type::to_int_type(*buf);
}

//--------------------------------
//
//    goutbuf implementation
//
//--------------------------------
const int OBUF_LEN = 256;

goutbuf::goutbuf() : x(-1), y(-1) {
  buf = new char[OBUF_LEN];
  setp(buf, buf + OBUF_LEN - 1);
  //We want to leave ourselves a character at the end, so that we may null
  //terminate the string during a flush to output a whole string at a time.
}

goutbuf::~goutbuf() {
  //Actually we don't want to flush, because when this dtor is called we can't
  //output.
  flush_output();
  delete[] buf;
}

void goutbuf::setNextWriteLoc(int xLoc, int yLoc) {
  x = xLoc;
  y = yLoc;
}

int goutbuf::sync() {
  flush_output();
  return 0;
}

/**
 * \bug does not handle nulls in the stream properly in all cases although
 *      most of the time it works.  It seems at least for the case of
 *      a null followed by a newline it ignores the newline.  Other cases
 *      seem to work.
 */
void goutbuf::flush_output() {
  //We always have a space for the null pointer because we reserved an
  //extra position when we called setp, and we do so again at the end of
  //this function.
  if (pptr() > pbase()) {
    *pptr() = '\0';
    if (x != -1) { //Do a mlprintf if a location was set.
      assert(y != -1);
      Console::mlprintf(x, y, pbase());
      x = y = -1;  //Unset last location.
    } else
      Console::mprintf(pbase());
    setp(buf, buf + OBUF_LEN - 1);
  }
}

goutbuf::int_type goutbuf::overflow(int_type meta) {
  if (meta != traits_type::eof()) {
    flush_output();
    Console::mputchar(traits_type::to_char_type(meta));
  }
  return traits_type::not_eof(meta);
}
  
std::streamsize goutbuf::xsputn(const char_type *ptr, std::streamsize count) {
  for (int i=0; i<count; i++) {
    sputc(ptr[i]);
    if (ptr[i] == '\n')
      flush_output();
  }
  return count;
}
  
} //namespace Console
} //namespace GNE

#endif //#ifdef OLD_CPP #else
