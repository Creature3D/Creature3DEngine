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
#include "../include/gnelib/PingPacket.h"
#include "../include/gnelib/Packet.h"
#include "../include/gnelib/Buffer.h"
#include "../include/gnelib/Mutex.h"
#include "../include/gnelib/Time.h"
#include "../include/gnelib/Timer.h"

namespace GNE {

const int PingPacket::ID = 3;//4;
Mutex PingPacket::sync;
guint32 PingPacket::nextReqId = 0;
std::map<guint32, Time> PingPacket::requests;

PingPacket::PingPacket(bool makeReq) : Packet(ID) {
  //makeReq is false when creating a packet just for reading.
  if (makeReq) {
    sync.acquire();
    
    //Get a new, unique request ID.
    reqId = nextReqId++;
    
    //Then register it:
    assert(requests.find(reqId) == requests.end());
    requests[reqId] = Timer::getCurrentTime();
    
    sync.release();	
  }
}

PingPacket::~PingPacket() {
}

bool PingPacket::isRequest() {
  return (T2 == Time());
}

void PingPacket::makeReply() {
  T2 = T3 = Timer::getCurrentTime();
}

PingInformation PingPacket::getPingInformation() {
  assert(!isRequest());
  PingInformation ret;
  sync.acquire();

  std::map<guint32, Time>::iterator iter = requests.find(reqId);
  if (iter != requests.end()) {
    Time T4 = Timer::getCurrentTime();
    const Time& T1(iter->second);
    ret.pingTime = (T4 - T1) - (T3 - T2);
    ret.clockOffset = ( (T2 - T1) + (T3 - T4) ) / 2;
    requests.erase(iter);
  }

  sync.release();
  return ret;
}

int PingPacket::recoverLostRequests(Time limit) {
  int ret = 0;
  Time late = Timer::getCurrentTime() - limit;
  sync.acquire();

  std::map<guint32, Time>::iterator iter = requests.begin();
  while (iter != requests.end()) {
    if (iter->second < late) {
      ++ret;
      //requests.erase(iter++);
	  iter = requests.erase(iter);
    } else
      ++iter;
  }

  sync.release();
  return ret;
}

int PingPacket::reqsPending() {
  sync.acquire();
  int ret = requests.size();
  sync.release();
  return ret;
}

int PingPacket::getSize() const {
  return Packet::getSize() + 2 + Buffer::getSizeOf(reqId) +
    Buffer::getSizeOf(T2) + Buffer::getSizeOf(T3);
}

void PingPacket::writePacket(Buffer& raw) const {
  Packet::writePacket(raw);
  gint16 size = Buffer::getSizeOf(reqId) + Buffer::getSizeOf(T2) + Buffer::getSizeOf(T3);
  raw << size;
  raw << reqId << T2 << T3;
}

void PingPacket::readPacket(Buffer& raw) {
  Packet::readPacket(raw);
  gint16 size;
  raw >> size;
  raw >> reqId >> T2 >> T3;
}

Packet* PingPacket::create() {
  return new PingPacket(false);
}

}
