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
#include <CREncapsulation/crGameDices.h>
#include <CRCore/crRange.h>
using namespace CREncapsulation;
//////////////////////////////////////////////////////////////////////////
//
//crGameDices
//
//////////////////////////////////////////////////////////////////////////
CRCore::ref_ptr<crGameDices> crGameDices::m_instance = NULL;
crGameDices::crGameDices()
{
}
crGameDices::~crGameDices()
{

}
crGameDices *crGameDices::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crGameDices;
	}
	return m_instance.get();
}
int crGameDices::dice(int count,int maximum)
{
	int rnd = 0;
	CRCore::rangei r(1,maximum);
	for( int i = 0; i<count; i++ )
	{
		rnd += r.get_random();
	}
	return rnd;
}
float crGameDices::dicef(int count,float maximum)
{
	float rnd = 0;
	CRCore::rangef r(0.0f,maximum);
	for( int i = 0; i<count; i++ )
	{
		rnd += r.get_random();
	}
	return rnd;
}