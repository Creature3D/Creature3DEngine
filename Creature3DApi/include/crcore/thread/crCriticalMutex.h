/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
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
*/

#ifndef CRCORE_CRITICALMUTEX_H
#define CRCORE_CRITICALMUTEX_H

#include <CRCore/crExport.h>

namespace CRCore {
struct crCriticalData;
class CR_EXPORT crCriticalMutex {
public:
  crCriticalMutex();

  virtual ~crCriticalMutex();
  inline void lock(){acquire();}
  inline void unlock(){release();}
  void acquire();
  void release();
private:
  void vanillaAcquire();
  void vanillaRelease();
  crCriticalData* data;
};

}
#endif
