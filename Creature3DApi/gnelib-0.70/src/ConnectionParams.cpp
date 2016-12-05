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
#include "../include/gnelib/ConnectionParams.h"
#include "../include/gnelib/PacketFeeder.h"
#include "../include/gnelib/ConnectionListener.h"

namespace GNE {

ConnectionParams::ConnectionParams()
: feederTimeout(0), feederThresh(0),
timeout(0), outRate(0), inRate(0), localPort(0), unrel(false),m_bufSizeScale(0) {
}

ConnectionParams::ConnectionParams(const ConnectionListener::sptr& Listener)
: listener(Listener), feederTimeout(0), feederThresh(0),
timeout(0), outRate(0), inRate(0), localPort(0), unrel(false),m_bufSizeScale(0) {
}

bool ConnectionParams::checkParams() const {
  return (outRate < 0 || inRate < 0 || localPort < 0 || localPort > 65535
    || !listener || timeout < 0 || feederTimeout < 0
    || feederThresh < 0 || m_bufSizeScale < 0 || m_bufSizeScale > 1000);
}

void ConnectionParams::setListener( const ConnectionListener::sptr& Listener ) {
  listener = Listener;
}

const ConnectionListener::sptr& ConnectionParams::getListener() const {
  return listener;
}

void ConnectionParams::setFeeder(const PacketFeeder::sptr& Feeder) {
  feeder = Feeder;
}

const PacketFeeder::sptr& ConnectionParams::getFeeder() const {
  return feeder;
}

void ConnectionParams::setFeederTimeout(int FeederTimeout) {
  feederTimeout = feederTimeout;
}

int ConnectionParams::getFeederTimeout() const {
  return feederTimeout;
}

void ConnectionParams::setLowPacketThreshold(int limit) {
  feederThresh = limit;
}

int ConnectionParams::getLowPacketThreshold() const {
  return feederThresh;
}

void ConnectionParams::setTimeout(int ms) {
  timeout = ms;
}

int ConnectionParams::getTimeout() const {
  return timeout;
}

void ConnectionParams::setOutRate(int OutRate) {
  outRate = OutRate;
}

int ConnectionParams::getOutRate() const {
  return outRate;
}

void ConnectionParams::setInRate(int InRate) {
  inRate = InRate;
}

int ConnectionParams::getInRate() const {
  return inRate;
}

void ConnectionParams::setRates(int OutRate, int InRate) {
  setOutRate(OutRate);
  setInRate(InRate);
}

void ConnectionParams::setLocalPort(int LocalPort) {
  localPort = LocalPort;
}

int ConnectionParams::getLocalPort() const {
  return localPort;
}

void ConnectionParams::setUnrel(bool set) {
  unrel = set;
}

bool ConnectionParams::getUnrel() const {
  return unrel;
}

void ConnectionParams::setBufSizeScale(int scale)
{
	m_bufSizeScale = scale;
}
int ConnectionParams::getBufSizeScale() const
{
	return m_bufSizeScale;
}

}
