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
#include <Plugin cre/crException.h>
#include <Plugin cre/crClipPlane.h>
#include <Plugin cre/crBase.h>

using namespace cre;

void crClipPlane::write(crDataOutputStream* out){

  // write crClipPlane's identification
  out->writeInt(CRECLIPPLANE);

  // if the CRCore class is inherited by any other class we should also write this to file
  CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
  if(obj)
    ((cre::crBase*)(obj))->write(out);
  else
    throw crException("crClipPlane::write(): Could not cast this CRCore::crClipPlane to an CRCore::crBase.");

  // write crClipPlane's properties
  
  out->writeVec4d(getClipPlane());
  
  out->writeUInt(getClipPlaneNum());

}

void crClipPlane::read(crDataInputStream* in){

  // peek on crClipPlane's identification
  int id = in->peekInt();
  if(id == CRECLIPPLANE)
    {
      // read crClipPlane's identification
      id = in->readInt();

      // if the CRCore class is inherited by any other class we should also read this from file
      CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
      if(obj)
        ((cre::crBase*)(obj))->read(in);
      else
        throw crException("crClipPlane::read(): Could not cast this CRCore::crClipPlane to an CRCore::crBase.");

      setClipPlane(in->readVec4d());

      setClipPlaneNum(in->readUInt());
    }
  else{
    throw crException("crClipPlane::read(): Expected crClipPlane identification.");
  }
}
