/* Creature3D - Online Game Engine, Copyright (C) 2005 Îâ²Æ»ª(26756325@qq.com)
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
#include<CRCore/crDB.h>


using namespace CRCore;
using namespace std;

crDB::crDB()
{
  //m_NodeType = NT_DB;
  m_gravity.set(0.0f,0.0f,-9.81f);
  m_groundPlane.set(0,0,1,0);
  m_maxiterations = 10;
}

crDB::crDB(const crDB& db, const crCopyOp& copyop):
crGroup(db,copyop),
m_gravity(db.m_gravity),
m_groundPlane(db.m_groundPlane),
m_maxiterations(db.m_maxiterations)
{
  //m_NodeType = NT_DB;
}
crDB::~crDB()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crDB()"<< std::endl;
}