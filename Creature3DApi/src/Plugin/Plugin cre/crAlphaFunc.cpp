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
#include <Plugin cre/crAlphaFunc.h>
#include <Plugin cre/crBase.h>

using namespace cre;

void crAlphaFunc::write(crDataOutputStream* out){

  out->writeInt(CREALPHAFUNC);

  CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
  if(obj)
    ((cre::crBase*)(obj))->write(out);
  else
    throw crException("crAlphaFunc::write(): Could not cast this CRCore::crAlphaFunc to an CRCore::crBase.");

  out->writeInt(getFunction());
  out->writeFloat(getReferenceValue());
}

void crAlphaFunc::read(crDataInputStream* in){

  int id = in->peekInt();
  if(id == CREALPHAFUNC)
    {
      id = in->readInt();

      CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
      if(obj)
        ((cre::crBase*)(obj))->read(in);
      else
        throw crException("crAlphaFunc::read(): Could not cast this CRCore::crAlphaFunc to an CRCore::crBase.");

      CRCore::crAlphaFunc::ComparisonFunction comparison_funtion = CRCore::crAlphaFunc::ComparisonFunction(in->readInt());
      float reference_value = in->readFloat();
      setFunction(comparison_funtion, reference_value);
    }
  else{
    throw crException("crAlphaFunc::read(): Expected crAlphaFunc identification.");
  }
}
