#ifndef _TEXTCONSOLE_H_
#define _TEXTCONSOLE_H_

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

#include "ConsoleBuffer.h"

namespace GNE {

namespace Console {

/**
 * @ingroup console
 *
 * Provides a text-based window view of a buffer of text, similar to a textbox
 * object in a GUI.  This allows one to provide a text-based console which
 * would be useful in a dedicated server running in a console, or in a UNIX
 * prompt environment.
 *
 * The console needs a rectangle on the screen for which it can have exclusive
 * access.  Other threads may use located versions of text output (like
 * Console::mlprintf) outside of the specified window area.  The window
 * provides word wrapping and software-based scrolling.
 */
class TextConsole : public ConsoleBuffer {
public:
  TextConsole( int xoffset, int yoffset, int width, int height );

  virtual void erase();

protected:
  virtual void render( int x, int y, std::string text, int renderHints );
};

} //namespace Console

} //namespace GNE

#endif
