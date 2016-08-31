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

#ifndef _GNELIB_H_
#define _GNELIB_H_

#ifdef _MSC_VER
/*
 * Disable following warnings:
 *
 * warning C4786: identifier was truncated to '255' characters in the debug information
 */
#pragma warning( disable : 4786)
#endif
#include "gnelib/Address.h"
#include "gnelib/Buffer.h"
#include "gnelib/ClientConnection.h"
#include "gnelib/ConnectionListener.h"
#include "gnelib/ConditionVariable.h"
#include "gnelib/Connection.h"
#include "gnelib/ConnectionEventGenerator.h"
#include "gnelib/ConnectionParams.h"
#include "gnelib/ConnectionStats.h"
#include "gnelib/Console.h"
#include "gnelib/ConsoleBuffer.h"
#include "gnelib/ConsoleStream.h"
#include "gnelib/Counter.h"
//#include "gnelib/CustomPacket.h"
#include "gnelib/Error.h"
#include "gnelib/Errors.h"
#include "gnelib/GNE.h"
#include "gnelib/GNEDebug.h"
#include "gnelib/ListServerConnection.h"
#include "gnelib/Lock.h"
#include "gnelib/Mutex.h"
//#include "gnelib/NetworkObject.h"
//#include "gnelib/ObjectBroker.h"
//#include "gnelib/ObjectBrokerClient.h"
//#include "gnelib/ObjectBrokerPacket.h"
//#include "gnelib/ObjectBrokerServer.h"
//#include "gnelib/ObjectCreationPacket.h"
//#include "gnelib/ObjectDeathPacket.h"
//#include "gnelib/ObjectUpdatePacket.h"
#include "gnelib/Packet.h"
#include "gnelib/PacketFeeder.h"
#include "gnelib/PacketStream.h"
#include "gnelib/PacketParser.h"
#include "gnelib/PingPacket.h"
#include "gnelib/ExitPacket.h"
#include "gnelib/RateAdjustPacket.h"
#include "gnelib/ReceiveEventListener.h"
#include "gnelib/ServerConnectionListener.h"
#include "gnelib/SmartPtr.h"
#include "gnelib/SyncConnection.h"
#include "gnelib/SynchronizedObject.h"
#include "gnelib/ServerConnection.h"
#include "gnelib/TextConsole.h"
#include "gnelib/Thread.h"
#include "gnelib/Time.h"
#include "gnelib/Timer.h"
#include "gnelib/TimerCallback.h"
#include "gnelib/WeakPtr.h"

#endif
