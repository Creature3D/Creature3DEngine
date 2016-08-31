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
#include "../include/gnelib/ChannelProvider.h"
#include "../include/gnelib/Packet.h"
#include "../include/gnelib/ChannelPacket.h"
#include "../include/gnelib/Connection.h"
#include "../include/gnelib/Lock.h"

namespace GNE {

typedef std::list<Connection*>::iterator ChannelIterator;

const int ChannelProvider::MIN_CHANNEL = 0;

const int ChannelProvider::MAX_CHANNEL = 255;

static const int NUM_CHANNELS = 255;
  
ChannelProvider::ChannelProvider() {
  for ( int i = 0; i < NUM_CHANNELS; ++i )
    channels[i] = NULL;
}

ChannelProvider::~ChannelProvider() {
  for ( int i = 0; i < NUM_CHANNELS; ++i )
    delete channels[i];
}

void ChannelProvider::addConnection( int channel, Connection* conn ) {
  assert( channel >= MIN_CHANNEL && channel <= MAX_CHANNEL );
  if ( channel >= MIN_CHANNEL && channel <= MAX_CHANNEL ) {
    sync.acquire();

    if ( channels[channel] == NULL )
      channels[channel] = new Channel;
    channels[channel]->conns.push_back( conn );

    sync.release();
  }
}

void ChannelProvider::removeConnection( int channel, Connection* conn ) {
  assert( channel >= MIN_CHANNEL && channel <= MAX_CHANNEL );
  if ( channel >= MIN_CHANNEL && channel <= MAX_CHANNEL ) {
    sync.acquire();
    
    if ( channels[channel] != NULL )
      channels[channel]->conns.remove( conn );

    sync.release();
  }
}

void ChannelProvider::removeFromAll( Connection* conn ) {
  sync.acquire();
  for ( int i = 0; i < NUM_CHANNELS; ++i ) {
    if ( channels[i] != NULL )
      channels[i]->conns.remove( conn );
  }
  sync.release();
}

void ChannelProvider::disbandChannel( int channel ) {
  assert( channel >= MIN_CHANNEL && channel <= MAX_CHANNEL );
  if ( channel >= MIN_CHANNEL && channel <= MAX_CHANNEL ) {
    sync.acquire();
    delete channels[channel];
    channels[channel] = NULL;
    sync.release();
  }
}

int ChannelProvider::numConnections( int channel ) const {
  assert( channel >= MIN_CHANNEL && channel <= MAX_CHANNEL );
  if ( channel >= MIN_CHANNEL && channel <= MAX_CHANNEL ) {
    LockMutex lock( sync );
    if ( channels[channel] == NULL )
      return 0;
    else
      return (int)channels[channel]->conns.size();
  } else
    return 0;
}

void ChannelProvider::sendToChannel( int channel, int from, const Packet& packet,
                                     Connection* exclude, bool reliable ) const {
  assert( channel >= MIN_CHANNEL && channel <= MAX_CHANNEL );
  assert( from >= 0 && from <= 255 );
  if ( channel >= MIN_CHANNEL && channel <= MAX_CHANNEL ) {
    ChannelPacket p( channel, from, packet );
    sendToChannel( p, exclude, reliable );
  }
}

void ChannelProvider::sendToChannel( ChannelPacket& packet, Connection* exclude,
                                    bool reliable ) const {
  int chan = packet.getChannel();
  assert( chan >= MIN_CHANNEL && chan <= MAX_CHANNEL );
  sync.acquire();
  if ( channels[chan] != NULL ) {
    ChannelIterator iter = channels[chan]->conns.begin();
    while ( iter != channels[chan]->conns.end() ) {
      (*iter)->stream().writePacket( packet, reliable );
    }
  }
  sync.release();
}

} //namespace GNE
