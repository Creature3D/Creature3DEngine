/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
#include <CRCore/crStateAttribute.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crNotify.h>
#include <algorithm>

using namespace CRCore;

void crStateAttribute::addParent(crStateSet* object)
{
	m_parents.push_back(object);
}

void crStateAttribute::removeParent(crStateSet* object)
{
	ParentList::iterator pitr = std::find(m_parents.begin(),m_parents.end(),object);
	if (pitr!=m_parents.end()) m_parents.erase(pitr);
}

void crStateAttribute::setUpdateCallback(Callback* uc)
{
	//CRCore::notify(CRCore::INFO)<<"crStateAttribute::Setting Update callbacks"<<std::endl;

	if (m_updateCallback==uc) return;

	int delta = 0;
	if (m_updateCallback.valid()) --delta;
	if (uc) ++delta;

	m_updateCallback = uc;

	if (delta!=0)
	{
		//CRCore::notify(CRCore::INFO)<<"Going to set crStateAttribute parents"<<std::endl;

		for(ParentList::iterator itr=m_parents.begin();
			itr!=m_parents.end();
			++itr)
		{
			//CRCore::notify(CRCore::INFO)<<"   Setting crStateAttribute parent"<<std::endl;

			(*itr)->setNumChildrenRequiringUpdateTraversal((*itr)->getNumChildrenRequiringUpdateTraversal()+delta);
		}
	}
}

//void crStateAttribute::setEventCallback(Callback* ec)
//{
//	//CRCore::notify(CRCore::INFO)<<"crStateAttribute::Setting Event callbacks"<<std::endl;
//
//	if (m_eventCallback==ec) return;
//
//	int delta = 0;
//	if (m_eventCallback.valid()) --delta;
//	if (ec) ++delta;
//
//	m_eventCallback = ec;
//
//	if (delta!=0)
//	{
//		for(ParentList::iterator itr=m_parents.begin();
//			itr!=m_parents.end();
//			++itr)
//		{
//			(*itr)->setNumChildrenRequiringEventTraversal((*itr)->getNumChildrenRequiringEventTraversal()+delta);
//		}
//	}
//}

void crStateAttribute::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0://m_drawCallback str
		break;
	case 1://m_updateCallback str
		break;
	case 2://m_eventCallback str
		break;
	}
}