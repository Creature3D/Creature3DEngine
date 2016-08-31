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
#include "../include/gnelib/SocketPair.h"
#include "../include/gnelib/Address.h"
#include "../include/gnelib/Error.h"
#include "../include/gnelib/GNEDebug.h"
#include "winsock.h"
namespace GNE {

SocketPair::SocketPair(NLsocket reliable, NLsocket unreliable)
: r(reliable), u(unreliable),releaseudp(false) {
	uRemoteAddr.valid = NL_FALSE;
}

SocketPair::~SocketPair() {
  //disconnect();
}

void SocketPair::disconnect() {
  if (r != NL_INVALID) {
    nlClose(r);
    r = NL_INVALID;
  }
  if (releaseudp && u != NL_INVALID) {
    nlClose(u);
    u = NL_INVALID;
	uRemoteAddr.valid = NL_FALSE;
  }
}

Address SocketPair::getLocalAddress(bool reliable) const {
  NLaddress ret;
  if (reliable) {
    if (r != NL_INVALID)
      nlGetLocalAddr(r, &ret);
    else
      ret.valid = NL_FALSE;
  } else {
    if (u != NL_INVALID)
      nlGetLocalAddr(u, &ret);
    else
      ret.valid = NL_FALSE;
  }
  return Address(ret);
}

Address SocketPair::getRemoteAddress(bool reliable) const {
  NLaddress ret;
  if (reliable) {
    if (r != NL_INVALID)
      nlGetRemoteAddr(r, &ret);
    else
      ret.valid = NL_FALSE;
  } else {
    if (u != NL_INVALID)
      nlGetRemoteAddr(u, &ret);
    else
      ret.valid = NL_FALSE;
  }
  return Address(ret);
}

static void addStats(NLsocket s, ConnectionStats& st) {
  st.packetsSent += nlGetSocketStat(s, NL_PACKETS_SENT);
  st.bytesSent += nlGetSocketStat(s, NL_BYTES_SENT);
  st.avgBytesSent += nlGetSocketStat(s, NL_AVE_BYTES_SENT);
  st.maxAvgBytesSent += nlGetSocketStat(s, NL_HIGH_BYTES_SENT);
  st.packetsRecv += nlGetSocketStat(s, NL_PACKETS_RECEIVED);
  st.bytesRecv += nlGetSocketStat(s, NL_BYTES_RECEIVED);
  st.avgBytesRecv += nlGetSocketStat(s, NL_AVE_BYTES_RECEIVED);
  st.maxAvgBytesRecv += nlGetSocketStat(s, NL_HIGH_BYTES_RECEIVED);
  st.openSockets++;
}

ConnectionStats SocketPair::getStats(int reliable) const {
  ConnectionStats ret;
  memset((void*)&ret, 0, sizeof(ret));
  if (reliable != 0 && r != NL_INVALID) //Add in reliable socket stats
    addStats(r, ret);
  if (reliable <= 0 && u != NL_INVALID) //Add in unrel socket stats
    addStats(u, ret);
  return ret;
}

//modify by wch
int SocketPair::rawRead(bool reliable, Buffer& buf) const {
  NLsocket act;
  if (reliable)
    act = r;
  else
  {//if u is invalid, send over r anyways.
	  act = u;
  }
  int read = 0;
  if(act != NL_INVALID)
  {
	  buf.clear();
	  //try
	  //{
		  read = nlRead( act, (NLvoid*)buf.getData(), (NLint)buf.getCapacity() );
		  //if(read == 0)
		  //{
			 // int error = nlGetError();
			 // std::string str = nlGetErrorStr(error);
		  //}
		  //if(read == NL_INVALID)
		  //{
    //          printf("%d, %s",nlGetError(), nlGetErrorStr(nlGetError()));
		  //}
		  buf.setLimit( read );
		  if(!reliable && !uRemoteAddr.valid) 
		  {
			  Address address = getRemoteAddress(false);
			  uRemoteAddr = address.getAddress();
		  }
	  //}
	  //catch (Error& e)
	  //{
		 // gnedbg2(1, "SocketPair::rawRead Error %d: %s",
			//  e.getCode(), e.toString().c_str());
	  //}
	  //catch (...)
	  //{
		 // Error e(Error::User);
		 // gnedbg2(1, "SocketPair::rawRead Error %d: %s",
			//  e.getCode(), e.toString().c_str());
	  //}
  }
  return read;
}

//modify by wch
int SocketPair::rawWrite(bool reliable, const Buffer& buf) const {
  NLsocket act;
  if (reliable)
  {
      act = r;
  }
  else
  {//if u is invalid, send over r anyways.
    //modify by wch
	if(u != NL_INVALID && uRemoteAddr.valid != NL_FALSE)
	{
		act = u;
		nlSetRemoteAddr(act, &uRemoteAddr);
	}
	else
		act = r;
    //act = (u != NL_INVALID) ? u : r;
  }
  int write = -1;
  if(act != NL_INVALID)
  {
	  //try
	  //{
	  int sockstat;
	  while(1)
	  {
		  sockstat = nlPollSocket(act, NL_WRITE_STATUS, 10);
		  if(sockstat == NL_TRUE)
		  {
			  write = nlWrite(act, (const NLvoid*)buf.getData(), (NLint)buf.getPosition());
			  break;
		  }
		  else if(sockstat == NL_INVALID)
		  {
			  write = -1;
			  break;
		  }
	  }
		  //if(write != buf.getPosition())
		  //{
			 // printf("%d, %s",nlGetError(), nlGetErrorStr(nlGetError()));
		  //}
	  //}
	  //catch (Error& e)
	  //{
		 // gnedbg2(1, "SocketPair::rawWrite Error %d: %s",
			//  e.getCode(), e.toString().c_str());
	  //}
	  //catch (...)
	  //{
		 // Error e(Error::User);
		 // gnedbg2(1, "SocketPair::rawWrite Error %d: %s",
			//  e.getCode(), e.toString().c_str());
	  //}
  }
  return write;
}
//add by wch
void SocketPair::setUDPRemoteAddr(NLaddress addr)
{
    uRemoteAddr = addr;
}
} //Namespace GNE




