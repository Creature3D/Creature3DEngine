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
#include "../include/gnelib/TextConsole.h"
#include "../include/gnelib/Console.h"
#include "../include/gnelib/ConsoleStream.h"

using namespace std;
using namespace GNE::Console;

namespace GNE {

namespace Console {

TextConsole::TextConsole( int xoffset, int yoffset, int width, int height )
: ConsoleBuffer( xoffset, yoffset, width, height, 1 ) {
};

void TextConsole::erase() {
  for ( int i = 0; i < getHeight(); ++i )
    render( getXOffset(), getYOffset() + i, string( getWidth(), ' ' ), 0 );
}

void TextConsole::render( int x, int y, string text, int renderHints ) {
  assert( text.find( '\n' ) == string::npos );
  if ( renderHints & REDRAW_HINT ) {
    //We want to fill the rest of the line with spaces to redraw.
    int numSpaces = getWidth() - x + getXOffset() - stringWidth( text );
    text.append( string( numSpaces, ' ' ) );
  }
  gout << Console::acquire;
  gout << moveTo( x, y ) << text << flush << Console::release;
}

} //namespace Console

} //namespace GNE
