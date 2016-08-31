#ifndef GNE_H_INCLUDED_C51DF1DD
#define GNE_H_INCLUDED_C51DF1DD

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
#include "export.h"
#include <nl.h>
#include "ConnectionStats.h"
#include "Error.h"
#include "gnetypes.h"

/**
 * The namespace in which all of GNE resides in.  The namespace consists of
 * a few global functions, some constants, classes, and the GNE::Console
 * namespace, which holds the console IO functions.
 */
namespace GNE {
  class Address;

  /**
   * Initalizes %GNE and HawkNL.  Call this before using any HawkNL or %GNE
   * functions.  Pass it the atexit function so shutdown will be called on
   * exit.  A call to any other %GNE function before this function succeeds
   * is undefined.
   *
   * Neither initGNE nor shutdownGNE calls are thread-safe, so they should
   * only be called by the main thread.
   *
   * @param networkType a HawkNL network driver, such as NL_IP or NL_IPX.
   *                    I've only ever tested NL_IP.
   * @param timeToClose the amount of time in milliseconds to wait for
   *   connections to finish closing, timers to shut down, and user threads
   *   to close.
   *
   * @return true if %GNE or HawkNL could not be initalized.
   *
   * @see shutdownGNE
   */
  bool GNE_EXPORT initGNE(NLenum networkType, int (*atexit_ptr)(void (*func)(void)), int timeToClose = 10000 );

  /**
   * Shuts down %GNE and HawkNL.  All open connections will be closed, all
   * active timers will be shut down, and the shutDown method of all of the
   * threads you have created will be called.  The method will block for the
   * time passed into the initGNE function.  After that time has passed, the
   * shutdown function exits.  If it is being called as a result of the atexit
   * procedure, the process will be forcefully shutdown when the atexit
   * handlers quit, so a stalled/crashed thread won't keep the program from
   * shutting down, but it will keep destructors from being called.
   *
   * Note that if you have connections, timers, or threads running when main
   * ends, you can be running code or be receiving events after the main
   * function has ended.  If this will be a problem you should explicitly call
   * the shutdownGNE method.
   * 
   * Neither initGNE nor shutdownGNE calls are thread-safe, so they should
   * only be called by the main thread, and most certainly not from any %GNE
   * Thread (this also means you cannot call "exit" from a %GNE Thread).
   */
  void GNE_EXPORT shutdownGNE();

  /**
   * Use this function to get the address of the default networking device on
   * this system, if possible.  The port in the resulting address will be
   * zero.  The returned address is invalid if an error occured.
   */
  Address GNE_EXPORT getLocalAddress();

  /**
   * Gets the global stats.  Very similar to Connection::getStats, but this
   * gets cumulative stats for all sockets, user-created ones and GNE ones
   * too you may not know about.
   */
  ConnectionStats GNE_EXPORT getGlobalStats();

  /**
   * Enables stats gathering, which is off by default.  When stats is not
   * enabled, the get function's results is undefined.
   */
  void GNE_EXPORT enableStats();

  /**
   * Disables stats gathering, which is the default mode.
   */
  void GNE_EXPORT disableStats();

  /**
   * Clears all global stats.
   */
  void GNE_EXPORT clearStats();
  
  /**
   * Returns the number of open low-level connections GNE has opened.
   */
  int GNE_EXPORT getOpenConnections();

  struct GNEProtocolVersionNumber {
    guint8 version;
    guint8 subVersion;
    guint16 build;
  };

  /**
   * Returns the GNE Protocol version number.  This is different from the
   * library version number.  This number is specified by the GNE protocol
   * document on the GNE web site.  Versions of this library obtained from
   * CVS might have the build number non-zero.  Non-beta public releases of
   * GNE will have a build number of zero and a version and subVersion number
   * strictly defined by the GNE protocol specification.  Public alpha or
   * beta releases might have a non-zero build number.
   */
  GNEProtocolVersionNumber GNE_EXPORT getGNEProtocolVersion();

  /**
   * This function is used internally by GNE to get the name that you set in
   * setGameInformation.
   */
  const char* getGameName();

  /**
   * This function is used internally by GNE to get the version that you set
   * in setGameInformation.
   */
  guint32 getUserVersion();

  /**
   * The user's game information.  This information is used during the
   * connection process to check if the versions and games match.  This
   * function should only be called once before making any connections.
   *
   * @param gameName the name of your game, or any other unique string
   *        identifier in ASCII format with a length not exceeding
   *        MAX_GAME_NAME_LEN ASCII characters.
   * @param version a 4-byte value with the current game version in any
   *        format.
   */
  void GNE_EXPORT setGameInformation(std::string gameName, guint32 version);

  /**
   * The maximum number of ASCII characters in game name that is passed into
   * the setGameInformation function.
   */
  const int MAX_GAME_NAME_LEN = 31;

  /**
   * Compares other versions against this library.  This is used internally
   * by GNE to compare versions.  It checks the GNE version numbers first,
   * then the game name, then the user versions.
   *
   * @throw Error if the GNE versions did not match.
   * @throw WrongGame if the game is of the wrong type.
   * @throw UserVersionMismatch if the user versions are different.
   */
  void checkVersions(const GNEProtocolVersionNumber& otherGNE,
    std::string otherName, guint32 otherUser);

  /**
   * A numeric representation of the current version of this library.
   */
  const double VER = 0.74;

  /**
   * A string representation of the name of the library and the current
   * version.
   */
  const std::string VER_STR = "GNE v0.74 CVS";

  /**
   * The low-level header size of a UDP header, which is used with the HawkNL
   * internet driver that GNE uses.  A number that I always have trouble
   * finding and might be useful in getting stats or calculating actual
   * bandwith.
   */
  const int UDP_HEADER_SIZE = 28;

  /**
   * Normally you would pass a network type to GNE::init, but passing this
   * value means that no network should be initalized.  This is useful only
   * in the example programs that don't use networking and do not need to
   * initalize everything.
   * @see initGNE
   */
  const NLenum NO_NET = 128;
}

#endif /* GNE_H_INCLUDED_C51DF1DD */
