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
#ifndef CRE_DRAWABLE
#define CRE_DRAWABLE 1

#include <CRCore/crDrawable.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crDrawable : public crReadWrite, public CRCore::crDrawable{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
