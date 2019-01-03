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
#include "../include/gnelib/ConsoleBuffer.h"
#include "../include/gnelib/ConsoleStream.h"

using namespace std;

namespace GNE {

const int ConsoleBuffer::REDRAW_HINT = 1;

ConsoleBuffer::ConsoleBuffer( int xoffset, int yoffset, int width, int height,
                              int textHeight )
: xoff(xoffset), yoff(yoffset), width(width), height(height),
textHeight(textHeight), numLines(height/textHeight), autoRender(true),
indent("  "), allDelims("\n- "), reqdDelims("-"), optDelims(" "), currCol(0),
currRow(0) {
}

ConsoleBuffer::~ConsoleBuffer() {
}

void ConsoleBuffer::setContIndent( const string& indent ) {
  acquire();
  this->indent = indent;
  release();
}

void ConsoleBuffer::setAutoRender( bool autoRender ) {
  acquire();
  this->autoRender = autoRender;
  release();
}

int ConsoleBuffer::getXOffset() {
  return xoff;
}

int ConsoleBuffer::getYOffset() {
  return yoff;
}

int ConsoleBuffer::getWidth() const {
  return width;
}

int ConsoleBuffer::getHeight() const {
  return height;
}

void ConsoleBuffer::clear() {
  acquire();

  lastLine = "";
  currRow = currCol = 0;
  lines.clear();
  erase();

  release();
}

void ConsoleBuffer::redraw() {
  acquire();

  currCol = 0;
  currRow = -(int)lines.size();
  doUpdate( REDRAW_HINT );

  release();
}

void ConsoleBuffer::update() {
  acquire();
  doUpdate( 0 );
  release();
}

bool ConsoleBuffer::scroll( int height ) {
  return false;
}

int ConsoleBuffer::stringWidth( const string& str ) {
  return (int)str.length();
}

void ConsoleBuffer::setRequiredDelimiters( const string& delims ) {
  acquire();
  reqdDelims = delims;
  allDelims = reqdDelims + optDelims + '\n';
  release();
}

void ConsoleBuffer::addRequiredDelimiters( const string& delims ) {
  acquire();
  setRequiredDelimiters( reqdDelims + delims );
  release();
}

string ConsoleBuffer::getRequiredDelimiters() {
  return reqdDelims;
}

void ConsoleBuffer::doUpdate( int hints ) {
  processInput();
  if ( currRow < 0 ) {
    if ( !scroll( -currRow * textHeight ) ) {
      //If the scroll failed we want to redraw everything.
      currCol = 0;
      currRow = -(int)lines.size();
      hints |= REDRAW_HINT;
    }

    //Move iter back to the last line updated
    LineIterator iter = lines.rend();
    for ( int i=0; i > currRow; --i ) {
      --iter;
    }

    while ( currRow < 0 ) {
      assert( yoff + height + currRow - textHeight >= yoff );
      assert( yoff + height + currRow - textHeight < yoff + height );
      render( xoff + currCol, yoff + height + currRow - textHeight,
        string( *iter, currCol, string::npos ), hints );
      currCol = 0;
      ++currRow;
      assert( iter != lines.rend() );
      ++iter;
    }
    assert( iter == lines.rend() );
  }
  //Now render lastLine
  assert( yoff + height - textHeight >= yoff );
  assert( yoff + height - textHeight < yoff + height );
  render( xoff + currCol, yoff + height - textHeight,
    string( lastLine, currCol, string::npos ), hints );
  currCol = stringWidth( lastLine );
}

void ConsoleBuffer::processInput() {
  //Pull words at a time out of input.
  string currWord;
  string::size_type pos = 0;
  string inputStr = input.str();
  do {
    pos = inputStr.find_first_of( allDelims );
    if (pos == 0 && !isReqDelim( inputStr[0] ) ) {
      //We have an optional delim (or newline) at the start of the word.
      pos = inputStr.find_first_of( allDelims, 1 );
    }
    if (pos != string::npos) {
      //If the delim we found is required, we want that to be part of the word
      //we extract as well.
      if ( isReqDelim( inputStr[pos] ) )
        ++pos;
      //Now let's get our word, and remove it from the input.
      currWord = string( inputStr, 0, pos );
      inputStr = string( inputStr, pos, string::npos );

      //Now we have removed a word from the buffer, possibly with a leading
      //optional delim, and possibly with a trailing required delim.
      processWord( currWord );
    }
  } while ( pos != string::npos );

  //This is a little workaround for a compliance bug in libstdc++-2 (GCC2)
  //and for a crashing bug in MSVC6.
  //Remove what we've used from the input stream.
  input.str( "" );
  input << inputStr;
  assert( input );
}

void ConsoleBuffer::processWord( string currWord ) {
  assert( (int)currWord.length() > 0 );
  int currLen = 0;
  if ( currWord[0] == '\n' ) {
    pushLastLine( string( currWord, 1, string::npos ) );
    assert( stringWidth( lastLine ) <= width );

  } else {
    currLen = stringWidth( lastLine );
    int wordLen = stringWidth( currWord );

    if (currLen + wordLen <= width) {
      lastLine.append( currWord );
    } else {
      //We are word wrapping.  If the starting character of currWord is an
      //optional delimiter (like a space) we want to remove it.
      if ( isOptDelim( currWord[0] ) )
        currWord = string( currWord, 1, string::npos );
      pushLastLine( indent + currWord );
      assert( stringWidth( lastLine ) <= width );
    }
  }
}

void ConsoleBuffer::pushLastLine( const string& newLastLine ) {
  lines.push_front( lastLine );
  //Drop the last line if we have more than the lines in the console.
  if ( (int)lines.size() > numLines - 1 )
    lines.pop_back();
  //Don't move the row cursor off the top when we scroll up.
  if ( -currRow < (numLines - 1) )
    --currRow;
  lastLine = newLastLine;
}

bool ConsoleBuffer::isReqDelim( char ch ) {
  acquire();
  bool ret = ( reqdDelims.find( ch ) != string::npos );
  release();
  return ret;
}

bool ConsoleBuffer::isOptDelim( char ch ) {
  acquire();
  bool ret = ( optDelims.find( ch ) != string::npos );
  release();
  return ret;
}

ConsoleBuffer& ConsoleBuffer::operator << (const Console::ConsoleMutex& rhs) {
  rhs.action( *this );
  return *this;
}

ConsoleBuffer& ConsoleBuffer::operator << (CBFType f) {
  f(input);
  if (autoRender)
    update();
  return *this;
}

} //namespace GNE


