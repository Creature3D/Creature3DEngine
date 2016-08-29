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

//游戏随机数D20
#ifndef CRENCAPSULATION_CRGAMEDICES
#define CRENCAPSULATION_CRGAMEDICES 1

#include <CREncapsulation/crExport.h>
#include <CRCore/Referenced.h>
#include <CRCore/ref_ptr.h>
namespace CREncapsulation {
class CRENCAPSULATION_EXPORT crGameDices : public CRCore::Referenced
{
public:
	crGameDices();
	static crGameDices *getInstance();
	int dice(int count,int maximum);
	float dicef(int count,float maximum);
protected:
	virtual ~crGameDices();
	static CRCore::ref_ptr<crGameDices> m_instance;
};
}

#endif