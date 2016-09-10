/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
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
#include <Plugin cre/crLightModel.h>
#include <Plugin cre/crBase.h>

using namespace cre;

void crLightModel::write(crDataOutputStream* out){

  // write crLightModel's identification
  out->writeInt(CRELIGHTMODEL);

  // if the CRCore class is inherited by any other class we should also write this to file
  CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
  if(obj)
    ((cre::crBase*)(obj))->write(out);
  else
    throw crException("crLightModel::write(): Could not cast this CRCore::crLightModel to an CRCore::crBase.");

  // write crLightModel's properties
  out->writeBool(getTwoSided());
  out->writeBool(getLocalViewer());
  out->writeVec4(getAmbientIntensity());
  out->writeInt( getColorControl());
}

void crLightModel::read(crDataInputStream* in){

  // peek on crLightModel's identification
  int id = in->peekInt();
  if(id == CRELIGHTMODEL)
    {
      // read crLightModel's identification
      id = in->readInt();

      // if the CRCore class is inherited by any other class we should also read this from file
      CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
      if(obj)
        ((cre::crBase*)(obj))->read(in);
      else
        throw crException("crLightModel::read(): Could not cast this CRCore::crLightModel to an CRCore::crBase.");

      // Read crLightModel's properties
      setTwoSided(in->readBool());
	  setLocalViewer(in->readBool());
	  setAmbientIntensity(in->readVec4());
	  setColorControl((ColorControl)in->readInt());
    }
  else{
    throw crException("crLightModel::read(): Expected crLightModel identification.");
  }
}
