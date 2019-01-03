#ifndef _CONNECTIONPARAMS_H_
#define _CONNECTIONPARAMS_H_

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
#include "SmartPointers.h"

namespace GNE {
class PacketFeeder;
class ConnectionListener;

/**
 * @ingroup midlevel
 *
 * A class to encapsulate parameters about a connection.  The parameter list
 * was starting to get very complex, and I knew in the future that I wanted
 * to expand the list of parameters, and perhaps change the defaults.  This
 * class when constructed sets everything to defaults, so change only the
 * parameters that you care about.
 *
 * Many of the methods in this class are not thread-safe, so instances of this
 * class should not be shared between threads.  Copies can be made of objects
 * of this class, though, should sharing be needed.
 */
class GNE_EXPORT ConnectionParams {
public:
  /**
   * Creates a new ConnectionParams object using the default values, and
   * not setting the listener property.  A non-NULL listener is always
   * needed by the time it comes active, so it is almost always specified.
   */
  ConnectionParams();

  /**
   * Creates a new ConnectionParams object using the default values, and
   * setting the listener property.  A non-NULL listener is always
   * needed by the time it comes active, so it is almost always specified.
   */
  ConnectionParams( const SmartPtr<ConnectionListener>& Listener );

  /**
   * Returns true if any of the parameters are invalid or out of range.
   */
  bool checkParams() const;

  /**
   * Returns the result of checkParams.
   */
  operator bool() const { return checkParams(); }

  /**
   * The ConnectionListener that will receive events from the Connection.
   * Valid value is any non-NULL pointer.
   */
  void setListener( const SmartPtr<ConnectionListener>& Listener);

  /**
   * Returns the value set by getListener.
   */
  const SmartPtr<ConnectionListener>& getListener() const;

  /**
   * The PacketFeeder receives onLowPackets events which are entirely
   * independant of the serialized event queue of events sent to the
   * ConnectionListener.  The default is NULL, and any point is valid.
   * @see PacketStream::setFeeder
   */
  void setFeeder(const SmartPtr<PacketFeeder>& Feeder);

  /**
   * Returns the value set by setFeeder.
   */
  const SmartPtr<PacketFeeder>& getFeeder() const;

  /**
   * Sets the timeout of the PacketFeeder for this Connection.  Default is 0
   * meaning no timeouts generated.  Valid values are 0 and positive integers
   * given in milliseconds.
   * @see PacketStream::setFeederTimeout
   */
  void setFeederTimeout(int FeederTimeout);

  /**
   * Returns the value set by setFeederTimeout.
   */
  int getFeederTimeout() const;

  /**
   * The low packet threshold for the PacketFeeder.  Default value is 0, 0 or
   * any positive integer is valid.
   * @see PacketStream::setLowPacketThreshold
   */
  void setLowPacketThreshold(int limit);

  /**
   * Returns the value set by setLowPacketThreshold.
   */
  int getLowPacketThreshold() const;

  /**
   * Sets the timeout for this connection in milliseconds.  A value of 0
   * signifies that no timeouts should occur.  Values less than 0 are invalid.
   * @see Connection::setTimeout
   */
  void setTimeout(int ms);

  /**
   * Gets the timeout.
   */
  int getTimeout() const;

  /**
   * The maximum rate we will transmit in bytes per second.  If this is 0,
   * then the rate is unlimited.  Valid values are 0 or a positive integer.
   *
   * The default out rate is 0 (unlimited).
   */
  void setOutRate(int OutRate);

  /**
   * Returns the value set by setOutRate.
   */
  int getOutRate() const;

  /**
   * The maximum rate we allow the sender to send to us in bytes per second.
   * If this is 0, then the requested incoming rate has no bounds.  Valid
   * values are 0 or a positive integer.
   *
   * The default in rate is 0 (unlimited).
   */
  void setInRate(int InRate);

  /**
   * Returns the value set by setInRate.
   */
  int getInRate() const;

  /**
   * A shortcut to set both rates at the same time.
   */
  void setRates(int OutRate, int InRate);

  /**
   * For client-side connections, this will set a local port, if you desire,
   * although most of the time you will want to keep this at its default value
   * of 0 to let the OS pick the local port for you.  Valid values are
   * [0..65535].
   *
   * This option is ignored for ServerConnection.
   */
  void setLocalPort(int LocalPort);

  /**
   * Returns the value set by setLocalPort.
   */
  int getLocalPort() const;

  /**
   * Set this to true if you want to be able to send unreliable packets over
   * this connection.  Setting this to false will provide a reliable-only
   * connection, so packets sent unreliably will instead be sent reliably.
   *
   * An unreliable connection is created only when both sides agree to open
   * one by setting this parameter to true.  A reliable connection is
   * available regardless of this setting.
   *
   * The default for unreliable is false.
   */
  void setUnrel(bool set);

  /**
   * Returns the value set by setUnrel.
   */
  bool getUnrel() const;

  void setBufSizeScale(int scale);
  int getBufSizeScale() const;
private:
  SmartPtr<ConnectionListener> listener;

  SmartPtr<PacketFeeder> feeder;

  int feederTimeout;

  int feederThresh;

  int timeout;

  int outRate;

  int inRate;

  int localPort;

  bool unrel;

  int m_bufSizeScale;
};

}

#endif
