#ifndef LISTSERVERCONNECTION_H_INCLUDED_C51B5B1C
#define LISTSERVERCONNECTION_H_INCLUDED_C51B5B1C

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
#include <export.h>
#include "Connection.h"
#include "ClientConnection.h"

namespace GNE {

/**
 * @ingroup highlevel
 *
 * A Client-side connection to a game list server for retriving a list of
 * games.  This stuff is all very preliminary.  List server code will be
 * completed last so don't pay attention to these classes at all.
 *
 * \todo The List server code will be completed only after the rest of GNE
 *       works and has been tested and finalized.
 */
class GNE_EXPORT ListServerConnection : public ClientConnection {
public:
  /**
   * Data about each game.
   */
  struct GameListData {
    std::string gameName;

    std::string modName;

    std::string serverName;

    std::string address;

    int numPlayers;

    int latency;

    void* gameSpecificData;

  };

  ListServerConnection(std::string address);

  virtual ~ListServerConnection();

  void setGame(std::string gameName);

  void getGameList();

};

}
#endif /* LISTSERVERCONNECTION_H_INCLUDED_C51B5B1C */
