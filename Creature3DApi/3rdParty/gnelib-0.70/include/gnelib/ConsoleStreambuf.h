#ifndef _CONSOLESTREAMBUF_H_
#define _CONSOLESTREAMBUF_H_

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
#include "OldConsoleStreambuf.h"
#else
#include <streambuf>

namespace GNE {
namespace Console {

/**
 * @ingroup console
 *
 * A class derived from streambuf that uses the functions in the Console class.
 * This allows cin or another istream to work under GNE by using the Console
 * class for multithreaded I/O.  You should use the GNE::Console::gin object
 * rather than using this class directly.
 *
 * For specifics on these functions, consult a standard C++ library reference.
 */
class ginbuf : public std::streambuf {
public:
  ginbuf();
  ~ginbuf();

protected:
  int_type underflow();

private:
  char* buf;
};

/**
 * @ingroup console
 *
 * A class derived from streambuf that uses the functions in the Console class.
 * This allows cout or another ostream to work under GNE by using the Console
 * class for multithreaded I/O.  You should use the GNE::Console::gout object
 * rather than using this class directly.
 *
 * For specifics on these functions, consult a standard C++ library reference.
 */
class goutbuf : public std::streambuf {
public:
  goutbuf();
  ~goutbuf();

  /**
   * If set, the next output will be an mlprintf instead of a mprintf, with
   * the specified coordinates.
   */
  void setNextWriteLoc(int x, int y);

protected:
  int sync();
  void flush_output();

#if (_MSC_VER == 1310 )
  //While still technically correct and valid, it's not quite as robust as the
  //next line because this makes the assumption on what traits_type is defined
  //as.
  int_type overflow(int_type meta = std::char_traits<char>::eof());
#else
  //For some reason, in MSVC.NET 2003 alone (not 2002), this line generates
  //the following error:
  //error C2653: 'char_traits<char>' : is not a class or namespace name
  int_type overflow(int_type meta = traits_type::eof());
#endif   
  
  std::streamsize xsputn(const char_type *ptr, std::streamsize count);

private:
  char* buf;

  int x; //the next coords to write to, if not -1.
  int y;
};

}
}

#endif //#ifdef OLD_CPP #else
#endif //#ifndef _CONSOLESTREAMBUF_H_
