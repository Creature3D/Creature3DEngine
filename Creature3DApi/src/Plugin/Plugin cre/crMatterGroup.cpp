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
#include <Plugin cre/crEventGroup.h>
#include <Plugin cre/crMatterGroup.h>
using namespace cre;

void crMatterGroup::write(crDataOutputStream* out)
{
    out->writeInt(CREMATTERGROUP);

    CRCore::crEventGroup* node = dynamic_cast<CRCore::crEventGroup*>(this);
    if(node)
	{
        static_cast<cre::crEventGroup*>(node)->write(out);
    }
    else
        throw crException("crMatterGroup::write(): Could not cast this CRPhysics::crMatterGroup to an CRCore::crEventGroup.");

	//从脚本解析
 //   out->writeInt(m_joints.size());
	//for( JointMap::iterator itr = m_joints.begin();
	//	 itr != m_joints.end();
	//	 ++itr )
	//{
	//	out->writeUInt( itr->first );
	//	out->writeJoint( itr->second.get() );
	//}
   
}

void crMatterGroup::read(crDataInputStream* in)
{
    int id = in->peekInt();
    if(id == CREMATTERGROUP)
	{
        id = in->readInt();
        CRCore::crEventGroup* node = dynamic_cast<CRCore::crEventGroup*>(this);
        if(node)
		{
            ((cre::crEventGroup*)(node))->read(in);
        }
        else
            throw crException("crMatterGroup::read(): Could not cast this CRPhysics::crMatterGroup to an CRCore::crEventGroup.");

		//从脚本解析
  //      int size,i,msgid;
		//size = in->readInt();
		//CRPhysics::crJoint *joint;
		//for( i = 0; i<size; i++)
		//{
		//	msgid = in->readUInt();
		//	joint = in->readJoint();
		//	m_joints.insert(crMatterGroup::JointMapPair(msgid,joint));
		//}

    }
    else{
        throw crException("crGroup::read(): Expected crGroup identification");
    }
}
