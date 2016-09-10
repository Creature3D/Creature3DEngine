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
//Modified by Wucaihua
#include <CRCore/crSwitch.h>
#include <algorithm>

using namespace CRCore;

crSwitch::crSwitch():
m_bNewChildDefaultValue(true)
{
	m_dataVariance = DYNAMIC;
}

crSwitch::crSwitch(const crSwitch& sw,const crCopyOp& copyop):
crGroup(sw,copyop),
m_bNewChildDefaultValue(sw.m_bNewChildDefaultValue),
m_ValueArray(sw.m_ValueArray)
{
}

void crSwitch::traverse(crNodeVisitor& nv)
{
	if (nv.getTraversalMode()==crNodeVisitor::TRAVERSE_ACTIVE_CHILDREN)
	{
		for(unsigned int pos=0;pos<m_childArray.size();++pos)
		{
			if (m_ValueArray[pos]) m_childArray[pos]->accept(nv);
		}
	}
	else
	{
		crGroup::traverse(nv);
	}
}

bool crSwitch::addChild( crNode *child )
{
	return addChild(child,m_bNewChildDefaultValue);
}

bool crSwitch::addChild( crNode *child, bool value )
{
	unsigned int childPosition = m_childArray.size();
	if (crGroup::addChild(child))
	{
		if (m_childArray.size()>m_ValueArray.size())
		{
			m_ValueArray.resize(m_childArray.size(),m_bNewChildDefaultValue);
			m_ValueArray[childPosition]=value;
		}
		return true;
	}
	return false;
}

bool crSwitch::insertChild( unsigned int index, crNode *child )
{
	return insertChild(index,child,m_bNewChildDefaultValue);
}

bool crSwitch::insertChild( unsigned int index, crNode *child, bool value )
{
	if (crGroup::insertChild(index,child))
	{
		if (index>=m_ValueArray.size())
		{
			m_ValueArray.push_back(value);
		}
		else
		{
			m_ValueArray.insert(m_ValueArray.begin()+index, value);
		}

		return true;
	}
	return false;
}

bool crSwitch::removeChild( crNode *child )
{
	// find the child's position.
	unsigned int pos=getChildIndex(child);
	if (pos==m_childArray.size()) return false;

	m_ValueArray.erase(m_ValueArray.begin()+pos);

	return crGroup::removeChild(child);    
}

void crSwitch::setValue(unsigned int pos,bool value)
{
	if (pos>=m_ValueArray.size()) m_ValueArray.resize(pos+1,m_bNewChildDefaultValue);
	m_ValueArray[pos]=value;
}

void crSwitch::setChildValue(const crNode* child,bool value)
{
	// find the child's position.
	unsigned int pos=getChildIndex(child);
	if (pos==m_childArray.size()) return;

	m_ValueArray[pos]=value;
}

bool crSwitch::getValue(unsigned int pos) const
{
	if (pos>=m_ValueArray.size()) return false;
	return m_ValueArray[pos];
}

bool crSwitch::getChildValue(const crNode* child) const
{
	// find the child's position.
	unsigned int pos=getChildIndex(child);
	if (pos==m_childArray.size()) return false;

	return m_ValueArray[pos];
}

bool crSwitch::setAllChildrenOff()
{
	m_bNewChildDefaultValue = false;
	for(ValueList::iterator itr=m_ValueArray.begin();
		itr!=m_ValueArray.end();
		++itr)
	{
		*itr = false;
	}
	return true;
}

bool crSwitch::setAllChildrenOn()
{
	m_bNewChildDefaultValue = true;
	for(ValueList::iterator itr=m_ValueArray.begin();
		itr!=m_ValueArray.end();
		++itr)
	{
		*itr = true;
	}
	return true;
}

bool crSwitch::setSingleChildOn(unsigned int pos)
{
	for(ValueList::iterator itr=m_ValueArray.begin();
		itr!=m_ValueArray.end();
		++itr)
	{
		*itr = false;
	}
	setValue(pos,true);
	return true;
}

#ifdef USE_DEPRECTATED_API
void crSwitch::setValue(int value)
{
	switch(value)
	{
	case(MULTIPLE_CHILDREN_ON):
		// do nothing...
		break;
	case(ALL_CHILDREN_OFF):
		{
			m_bNewChildDefaultValue = false;
			for(ValueList::iterator itr=m_ValueArray.begin();
				itr!=m_ValueArray.end();
				++itr)
			{
				*itr = false;
			}
			break;
		}
	case(ALL_CHILDREN_ON):
		{
			m_bNewChildDefaultValue = true;
			for(ValueList::iterator itr=m_ValueArray.begin();
				itr!=m_ValueArray.end();
				++itr)
			{
				*itr = true;
			}
			break;
		}
	default:
		{
			for(ValueList::iterator itr=m_ValueArray.begin();
				itr!=m_ValueArray.end();
				++itr)
			{
				*itr = false;
			}
			setValue(value,true);
			break;
		}
	}
}

int crSwitch::getValue() const
{
	if (m_ValueArray.empty()) return ALL_CHILDREN_OFF;

	unsigned int noChildrenSwitchedOn=0;
	int firstChildSelected=ALL_CHILDREN_OFF;
	for(unsigned int i=0; i<m_ValueArray.size();++i)
	{
		if (m_ValueArray[i])
		{
			++noChildrenSwitchedOn;
			if (firstChildSelected==ALL_CHILDREN_OFF) firstChildSelected=i;
		}
	}

	if (noChildrenSwitchedOn>1)
	{
		if (noChildrenSwitchedOn==m_ValueArray.size()) return ALL_CHILDREN_ON;
		else return MULTIPLE_CHILDREN_ON;
	}
	return firstChildSelected;

}
#endif
