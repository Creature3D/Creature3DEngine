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
#include "../include/gnelib/Console.h"
#include "../include/gnelib/ConsoleStreambuf.h"
#include "../include/gnelib/ConsoleStream.h"
#include "../include/gnelib/Mutex.h"
#include "../include/gnelib/Lock.h"

#ifndef OLD_CPP
#include <ostream>
#include <istream>
#else //fix for old libstdc++ used traditionally with gcc 2.9x
#include <iostream>
#endif

#include "../include/gnelib/conioport.h"

namespace GNE {
namespace Console {

static Mutex outSync;
static bool initialized = false;
static bool clearOnEnd = true;

//Global iostreams to replace cout/cin
ConsoleMutex acquire(true);
ConsoleMutex release(false);
static goutbuf outbuf;
GOut gout(&outbuf);
static ginbuf inbuf;
std::istream gin(&inbuf);

//Our global key codes for enter and backspace
int ENTER;
int BACKSPACE;

//MSVC6 has a strange buf about qualifing the std namespace here, so we import
//the std namespace.
using namespace std;

GOut::GOut( goutbuf* buf ) : ostream( (streambuf*)buf ) {
}

GOut::~GOut() {
}

GOut& GOut::operator << ( const ConsoleMutex& cm ) {
  cm.action( *this );
  return *this;
}

GOut& GOut::operator << ( const moveTo& cm ) {
  cm.action( *this );
  return *this;
}

GOut& GOut::operator << ( const ConsoleManipulator& cm ) {
  cm.action( *this );
  return *this;
}

GOut& GOut::operator << ( GOFType f ) {
  f( *this );
  return *this;
}

bool initConsole( bool clearOnExit ) {
  LockMutex lock( outSync );

  if (!initialized) {
    conio_init(&ENTER, &BACKSPACE);
    gin.tie(&gout); //tie the input and output together.
    initialized = true;
    clearOnEnd = clearOnExit;
  }
  return false;
}

bool initConsole(int (*)(void (*)(void))) {
  return initConsole( true );
}

void shutdownConsole() {
  LockMutex lock( outSync );

  if (initialized) {
    gout.flush();

    if ( clearOnEnd ) {
      mclearConsole();
      conio_gotoxy( 0, 0 );
    }

    conio_exit();
    initialized = false;
  }
}

void mclearConsole() {
  LockMutex lock( outSync );
  conio_clear();
}

int kbhit() {
  //We don't lock here since only one thread can ever do input
  assert(initialized);
  return conio_kbhit();
}

int getch() {
  //We don't lock here since only one thread can ever do input
  assert(initialized);
  return conio_getch();
}

int mprintf(const char* format, ...) {
  assert(initialized);
  va_list arg;
  
  va_start(arg, format);
  outSync.acquire();
  int ret = conio_vprintf(format, arg);
  outSync.release();
  va_end(arg);

  return ret;
}

int mlprintf(int x, int y, const char* format, ...) {
  assert(initialized);
  va_list arg;
  
  va_start(arg, format);
  outSync.acquire();
  conio_gotoxy(x, y);
  int ret = conio_vprintf(format, arg);
  outSync.release();
  va_end(arg);

  return ret;
}

void mputchar(int ch) {
  assert(initialized);
  outSync.acquire();
  conio_putchar(ch);
  outSync.release();
}

void mlputchar(int x, int y, int ch) {
  assert(initialized);
  outSync.acquire();
  conio_gotoxy(x, y);
  conio_putchar(ch);
  outSync.release();
}

int lgetString(int x, int y, char* str, int maxlen) {
  assert(initialized);
  int currpos = 0;          //The next char to be typed
  bool exit = false;
  while (!exit) {
    int ch = getch();
    if (ch == ENTER) {      //If user pressed RETURN
      exit = true;
    } else if (ch == BACKSPACE) {//If backspace
      currpos--;
      if (currpos < 0)
        currpos = 0;        //Hit the end
      else {
        mlputchar(currpos+x, y, ' ');

        //Performing this action helps to keep the cursor in the proper
        //position.
        if (currpos > 0)
          mlputchar(currpos-1+x, y, (int)str[currpos-1]);
      }
    } else {
      currpos++;
      if (currpos > maxlen) {
        currpos = maxlen;   //Clip off at end
      } else {
        mlputchar(currpos-1+x, y, ch); //Display input
        str[currpos-1] = (char)ch;     //update input string
      }
    }
    str[currpos] = '\0';
  }
  return currpos;
}

int getString(char* str, int maxlen) {
  assert(initialized);
  int x, y;
  mgetPos(&x, &y);
  return lgetString(x, y, str, maxlen);
}

void setTitle(const char* title) {
  assert(initialized);
  conio_settitle(title);
}

void mgetConsoleSize(int* width, int* height) {
  assert(initialized);
  outSync.acquire();
  conio_getsize(width, height);
  outSync.release();
}

void mgetPos(int* x, int* y) {
  assert(initialized);
  outSync.acquire();
  conio_getxy(x, y);
  outSync.release();
}

}
}





