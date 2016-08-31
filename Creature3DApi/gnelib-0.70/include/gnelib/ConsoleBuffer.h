#ifndef _CONSOLEBUFFER_H_
#define _CONSOLEBUFFER_H_

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

#include "SynchronizedObject.h"

#include <string>
#include <list>
#include <sstream>

namespace GNE {
  namespace Console {
    class ConsoleMutex;
  }

/**
 * @ingroup console
 *
 * An abstracted Console.  A console is defined as a rectangle in units.  What
 * a unit is is entirely arbitrary -- it could be characters or pixels or
 * something else.
 *
 * Essentially, this class abstracts a character (or mostly character) based
 * console that does word-wrapping.  A renderer for this buffer would extend
 * this class.
 *
 * The buffer operates on a single pass -- thus once it writes data, it won't
 * rewrite that data again (assuming the scroll method is supported).  The
 * important thing to note about this fact is that the console is therefore
 * "word buffered" meaning you must output a full word before it is shown --
 * specifically, all text is displayed until the last delimiter element or a
 * newline element.
 *
 * You won't instantiate a ConsoleBuffer directly, but instead will create a
 * instance of a child class of ConsoleBuffer, like TextConsole.
 *
 * ConsoleBuffer provides thread-safety in a similar way used for
 * Console::gout.  You may use the acquirecb and releasecb objects to acquire
 * or release the buffer when using the insertion operators:
 * <code>cb << acquirecb << "Testing " << 123 << endl << releasecb;</code>
 *
 * Calling the ConsoleBuffer functions implicitly lock it, so you need not
 * acquire or release the object when calling the functions.
 */
class ConsoleBuffer : public SynchronizedObject {
public:
  virtual ~ConsoleBuffer();

  /**
   * Sets the continuation indent for the TextConsole.  The default value is
   * 2 spaces.  When the word wrap wraps a line, the next line will be indented
   * with the given string.
   */
  void setContIndent( const std::string& indent );

  /**
   * Sets auto rendering mode on or off.  When auto render mode is enabled,
   * the screen is refreshed immediately whenever an action on the console
   * would change its view on the screen.  The default is true.
   */
  void setAutoRender( bool autoRender );

  /**
   * Returns the x offset.
   */
  int getXOffset();

  /**
   * Returns the y offset.
   */
  int getYOffset();

  /**
   * Returns the current buffer width.
   */
  int getWidth() const;

  /**
   * Returns the current buffer height.
   */
  int getHeight() const;

  /**
   * Tells the renderer for this ConsoleBuffer to completely erase the display.
   * This is different from clear in the fact that erase will "hide" the
   * console.  The behavior of calling update, or adding text during auto
   * render mode, before calling redraw or clear is defined by the renderer,
   * but in general if you want to use the console again, you should call
   * either clear or redraw before inserting text.
   */
  virtual void erase() = 0;

  /**
   * Clears all the text from the console, leaving an empty buffer.  Whether
   * auto render is enabled or disabled, erase is called immedately.
   */
  void clear();

  /**
   * Redraws the console by rerendering all of it.  This method most useful
   * after a call to erase (but not by one to clear).
   */
  void redraw();

  /**
   * Updates the view of this console by having the renderer update the
   * destination device.  If auto render is enabled, calling this method will
   * be a redundant operation.
   */
  void update();

  template <class T>
  ConsoleBuffer& operator << (const T& rhs) {
    input << rhs;
    if (autoRender)
      update();
    return *this;
  }

  //GCC 2 doesn't like member template specialization.  The code compiles
  //with these lines commented in MSVC.NET or less, and GCC2, so I leave
  //them commented out.
  //template <>
  ConsoleBuffer& operator << (const Console::ConsoleMutex& rhs);

  typedef std::ostream& (*CBFType)(std::ostream&);

  ConsoleBuffer& operator << (CBFType f);

protected:
  /**
   * Readies a new TextConsole in the inclusive rectangle from
   * (xoffset, yoffset) to (xoffset+width, yoffset+height).  The TextConsole
   * will not write to the lower right character as not to trigger accidental
   * hardware scrolling on some platforms.  The textHeight parameter denotes
   * the height of the text (and therefore the line height).
   */
  ConsoleBuffer( int xoffset, int yoffset, int width, int height, int textHeight );

  /**
   * Asks the renderer to render the data at the specified location.  There
   * will not be newline characters in the text string.  The renderHints
   * parameter contains hints for the renderer.  At this time the only hint
   * is ConsoleBuffer::REDRAW_HINT, which is set when redrawing to a location.
   * The renderHints field is a bitmask, so use the bitwise and operator to
   * check for hints.
   */
  virtual void render( int x, int y, std::string text, int renderHints ) = 0;

  /**
   * Tells the renderer that the console is scrolling by the specified height
   * in units.  If the renderer supports some sort of scrolling scheme, it
   * should invove it.  If it can do so, the renderer shall return true, and
   * then only the changed and new lines will be sent to render.
   *
   * If the renderer does not support scrolling, it shall return false.  In
   * this case, all lines are resent to the render function, and it will act
   * as a total redraw.  The default behaviour is to return false.
   */
  virtual bool scroll( int height );

  /**
   * Returns the width of the text in width units.  The default
   * implementation returns simply <code>str.length()</code>, so if the
   * renderer uses a proportional font, or more specifically if any letter
   * has a length greater than 1, the renderer should override this method.
   *
   * The given string may contain any character you insert into the buffer,
   * excluding newline characters.
   */
  virtual int stringWidth( const std::string& str );

  /**
   * Sets a string of the set of required delimiter characters.  A delimiter
   * character is required if it should always be displayed, and never left
   * out during a word wrap.  For example a dash character would be a required
   * delimiter whereas a space is not.
   *
   * The default required delimiter set is a dash.
   * Depending on your preference, you may wish to add characters such as ";."
   * or other puncutation marks.
   *
   * Required delimiters are considered to the be the last part of a word,
   * whereas optional delimiters are considered to be the beginning of the
   * next word (and thus are sent to the renderer as such).  The exception to
   * this rule is the newline character.  The newline character is always
   * considered a delimiter.  It is processed differently and is a required
   * delimiter but treated more like an optional one in the fact that it
   * belongs to the start of the next word, meaning the line is not actually
   * advanced until the next word appears.  This is so that there is not
   * always a blank line at the bottom of the console if you use a newline
   * character to end your additions to the console.
   */
  void setRequiredDelimiters( const std::string& delims );

  /**
   * Adds the delimiters in the string to the set of required delimiters.
   * @see #setRequiredDelimiters
   */
  void addRequiredDelimiters( const std::string& delims );

  /**
   * Returns the current set of required delimiter characters.
   */
  std::string getRequiredDelimiters();

  static const int REDRAW_HINT;

private:
  /**
   * Update method, but allows to give hints.  The actual work is done here.
   */
  void doUpdate( int hints );

  void processInput();

  void processWord( std::string currWord );

  /**
   * Pushes the lastLine string into lines, dropping old lines if needed and
   * decrementing currRow if needed as well.
   */
  void pushLastLine( const std::string& newLastLine );

  bool isReqDelim( char ch );
  bool isOptDelim( char ch );

  int xoff;
  int yoff;
  int width;
  int height;
  int textHeight;
  int numLines;

  bool autoRender;

  std::string indent;

  std::string allDelims;
  std::string reqdDelims;
  std::string optDelims;

  /**
   * The input from the user.
   */
  std::ostringstream input;

  /**
   * The current, last line of text, brought in from the input variable.
   */
  std::string lastLine;
  //The current row and column of the "cursor."  Row will be 0 for lastLine,
  //and goes more negative as the buffer scrolls.
  int currCol;
  int currRow;

  std::list<std::string> lines;

  typedef std::list<std::string>::reverse_iterator LineIterator;
};

} //namespace GNE

#endif
