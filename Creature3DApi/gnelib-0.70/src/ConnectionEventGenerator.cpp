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
#include "../include/gnelib/ConnectionEventGenerator.h"
#include "../include/gnelib/ReceiveEventListener.h"
#include "../include/gnelib/ConditionVariable.h"
#include "../include/gnelib/Connection.h"
#include "../include/gnelib/Errors.h"
#include "../include/gnelib/Lock.h"
#include "../include/nl.h"
#include "../include/gnelib/Timer.h"
//#include <CRCore/thread/crScopedLock.h>
namespace GNE {

#define MAX_GROUP_SOCKETS 1000

ConnectionEventGenerator::ConnectionEventGenerator() 
: Thread("EventGen", Thread::DEF_PRI/*HIGH_PRI*/), group(NL_INVALID) {
  group = nlGroupCreate();
  assert(group != NL_INVALID);
  sockBuf = new NLsocket[MAX_GROUP_SOCKETS];
  setType( SYSTEM );
  gnedbgo(5, "created");
}

ConnectionEventGenerator::sptr ConnectionEventGenerator::create() {
  sptr ret( new ConnectionEventGenerator() );
  ret->setThisPointer( ret );
  return ret;
}

ConnectionEventGenerator::~ConnectionEventGenerator() {
  nlGroupDestroy(group);
  delete[] sockBuf;
  gnedbgo(5, "destroyed");
}

/**
 * \bug If nlPollGroup fails, the error is not reported.  Instead the results
 *      are ignored, and the call is made again (or in debug mode, the
 *      assert fails).
 */
void ConnectionEventGenerator::run() 
{
	while (!shutdown) 
	{
		mapCtrl.acquire();
		while (connections.empty() /*&& m_connections_reg.empty()*/ && !shutdown) 
		{
		    mapCtrl.wait();
			//sleep(100);
		}
		mapCtrl.release();

		if (!shutdown) 
		{	
			//try 
			//{
				//m_unregMutex.lock();
				//ConnectionsMap::iterator itr;
				//for( ConnectionsQueue::iterator uitr = m_connections_unreg.begin();
				//	 uitr != m_connections_unreg.end();
				//	 ++uitr )
				//{
				//	itr = connections.find(*uitr);
				//	if(itr != connections.end())
				//	{
				//	    if(!nlGroupDeleteSocket(group, *uitr))
				//		{
				//			NLenum err = nlGetError();
    //                        printf("nlGroupDeleteSocket HawkNL error: %s\n", nlGetErrorStr(err));
				//		}
				//		nlClose(*uitr);
				//	    connections.erase(*uitr);
				//	}
				//}
				//m_connections_unreg.clear();
				//m_unregMutex.unlock();
				//
				//m_regMutex.lock();
				//for( ConnectionsMap::iterator ritr = m_connections_reg.begin();
				//	ritr != m_connections_reg.end();
				//	++ritr )
				//{
				//	if ( connections.find(ritr->first) == connections.end() ) 
				//	{
				//	    if(!nlGroupAddSocket(group, ritr->first))
				//		{
				//			NLenum err = nlGetError();
				//			printf("nlGroupAddSocket HawkNL error: %s\n", nlGetErrorStr(err));
				//		}
				//		else
				//		{
				//			connections[ritr->first] = ritr->second;
				//		}
				//	}
				//}
				//m_connections_reg.clear();
				//m_regMutex.unlock();
				//Time t1 = Timer::getCurrentTime();
				int numsockets = nlPollGroup(group, NL_READ_STATUS, sockBuf, MAX_GROUP_SOCKETS, 250);
				//int dt = (Timer::getCurrentTime() - t1).getTotalmSec();
				//if(dt<9)
				//	printf("nlPollGroup dt %d\n", dt);
				if (numsockets != NL_INVALID) 
				{
					numsockets--;
					for (; numsockets >= 0; numsockets--) 
					{
						LockCVEx lock( mapCtrl );
						ConnectionsMapIter iter = connections.find(sockBuf[numsockets]);
						ReceiveEventListener::sptr listener;

						//Check to make sure the listener didn't unregister while we were waiting.
						if (iter != connections.end())
							listener = iter->second;

						//Release mapCtrl, so we don't cause any deadlocks
						lock.release();
						if ( listener )
						{
							listener->onReceive();
						}
					}
				}
				else
				{
					NLenum err = nlGetError();

					if(err == NL_SYSTEM_ERROR)
					{
						printf("nlPollGroup system error: %s\n", nlGetSystemErrorStr(nlGetSystemError()));
					}
					else
					{
						printf("nlPollGroup HawkNL error: %s\n", nlGetErrorStr(err));
					}
				}
		 //   }
			//catch (...) 
			//{
			//	gnedbg(1, "ConnectionEventGenerator thread terminated. Error");
			//	printf(" ConnectionEventGenerator thread Error\n");
			//}
		} 
		else 
		{
			//The only valid error is NL_INVALID_SOCKET which happens if we close
			//a socket while nlPollGroup is using it.
			if (nlGetError() != NL_INVALID_SOCKET) 
			{
				gnedbgo1(1, "%s", LowLevelError().toString().c_str());
				assert(false);
			}
		}
	}
}

void ConnectionEventGenerator::reg(NLsocket socket, const ReceiveEventListener::sptr& listener) {
	assert(socket != NL_INVALID);

	LockCV lock( mapCtrl );
	if ( connections.find(socket) == connections.end() ) {
		nlGroupAddSocket(group, socket);
		connections[socket] = listener;
	}
	mapCtrl.signal(); //This will wake up the daemon thread if this is the first
	// registered, and it is sleeping.
 //   m_regMutex.lock();
	//m_connections_reg[socket] = listener;
	//m_regMutex.unlock();
}

void ConnectionEventGenerator::unreg(NLsocket socket) {
	assert(socket != NL_INVALID);

	LockCV lock( mapCtrl );
	if(connections.find(socket) != connections.end()) {
		nlGroupDeleteSocket(group, socket);
		connections.erase(socket);
	}
	nlClose(socket);
 //   m_unregMutex.lock();
 //   m_connections_unreg.push_back(socket);
	//m_unregMutex.unlock();
}

void ConnectionEventGenerator::shutDown() {
  Thread::shutDown();
  mapCtrl.signal();
}

}





