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
//Modified by 吴财华
#include <CRCore/crMultiSwitch.h>
#include <algorithm>

using namespace CRCore;

crMultiSwitch::crMultiSwitch():
    m_newChildDefaultValue(true),
    m_activeSwitchSet(0)
{
}

crMultiSwitch::crMultiSwitch(const crMultiSwitch& sw,const CRCore::crCopyOp& copyop):
    CRCore::crGroup(sw,copyop),
    m_newChildDefaultValue(sw.m_newChildDefaultValue),
    m_activeSwitchSet(sw.m_activeSwitchSet),
    m_values(sw.m_values)
{
}

void crMultiSwitch::setActiveSwitchSet(unsigned int switchSet) 
{
	if(m_activeSwitchSet!= switchSet)
	{
		m_activeSwitchSet = switchSet;
		dirtyBound();
	}
}

void crMultiSwitch::traverse(CRCore::crNodeVisitor& nv)
{
    if (nv.getTraversalMode()==CRCore::crNodeVisitor::TRAVERSE_ACTIVE_CHILDREN)
    {
        if (m_activeSwitchSet<m_values.size())
        {
            for(unsigned int pos=0;pos<m_childArray.size();++pos)
            {
                if (m_values[m_activeSwitchSet][pos]) m_childArray[pos]->accept(nv);
            }
        }
    }
    else
    {
        crGroup::traverse(nv);
    }
}

bool crMultiSwitch::computeBound() const
{
	m_boundSphere.init();
	m_bbox.init();

	if (m_childArray.empty()) 
	{
		m_bBoundSphere_computed = true;
		return false;
	}

	if (m_activeSwitchSet<m_values.size())
	{
		for(unsigned int pos=0;pos<m_childArray.size();++pos)
		{
			if (m_values[m_activeSwitchSet][pos]) m_bbox.expandBy(m_childArray[pos]->getBoundBox());
		}
	}

	if (!m_bbox.valid()) 
	{
		m_bBoundSphere_computed = true;
		return false;
	}

	m_boundSphere.expandBy(m_bbox);

	m_bBoundSphere_computed = true;
	return true;
}

crNode *crMultiSwitch::getFirstActiveChild()
{
	if (m_activeSwitchSet<m_values.size())
	{
		for(unsigned int pos=0;pos<m_childArray.size();++pos)
		{
			if (m_values[m_activeSwitchSet][pos]) 
			{
				return m_childArray[pos].get();
			}
		}
	}
	return NULL;
}

bool crMultiSwitch::addChild( CRCore::crNode *child)
{
    unsigned int childPosition = m_childArray.size();
    if (crGroup::addChild(child))
    {
        for(SwitchSetList::iterator itr=m_values.begin();
            itr!=m_values.end();
            ++itr)
        {
            ValueList& values = *itr;
            if (m_childArray.size()>values.size())
            {
                values.resize(m_childArray.size(),m_newChildDefaultValue);
                values[childPosition]=m_newChildDefaultValue;
            }
        }
        return true;
    }
    return false;
}

bool crMultiSwitch::insertChild( unsigned int index, CRCore::crNode *child)
{
    if (crGroup::insertChild(index,child))
    {
        for(SwitchSetList::iterator itr=m_values.begin();
            itr!=m_values.end();
            ++itr)
        {
            ValueList& values = *itr;
            if (index>=values.size())
            {
                values.push_back(m_newChildDefaultValue);
            }
            else
            {
                values.insert(values.begin()+index, m_newChildDefaultValue);
            }
        }

        
        return true;
    }
    return false;
}

//bool crMultiSwitch::removeChild( CRCore::crNode *child )
//{
//    return crGroup::removeChild(child);    
//}

bool crMultiSwitch::removeChild(unsigned int pos,unsigned int numChildrenToRemove)
{
	if (pos<m_childArray.size() && numChildrenToRemove>0)
	{
		unsigned int endOfRemoveRange = pos+numChildrenToRemove;
		if (endOfRemoveRange>m_childArray.size())
		{
			//notify(DEBUG_INFO)<<"Warning: crMultiSwitch::removeChild(i,numChildrenToRemove) has been passed an excessive number"<<std::endl;
			//notify(DEBUG_INFO)<<"         of chilren to remove, trimming just to end of child list."<<std::endl;
			endOfRemoveRange=m_childArray.size();
		}

		for(SwitchSetList::iterator itr=m_values.begin();
			itr!=m_values.end();
			++itr)
		{
			ValueList& values = *itr;
			values.erase(values.begin()+pos,values.begin()+endOfRemoveRange);
		}
	}

    return crGroup::removeChild(pos,numChildrenToRemove);
}

void crMultiSwitch::setValue(unsigned int switchSet, unsigned int pos,bool value)
{
    expandToEncompassSwitchSet(switchSet);

    ValueList& values = m_values[switchSet];
    if (pos>=values.size()) values.resize(pos+1,m_newChildDefaultValue);
    values[pos]=value;
}

void crMultiSwitch::setChildValue(const CRCore::crNode* child,unsigned int switchSet, bool value)
{
    expandToEncompassSwitchSet(switchSet);

    // find the child's position.
    unsigned int pos=getChildIndex(child);
    if (pos==m_childArray.size()) return;
    
    ValueList& values = m_values[switchSet];
    values[pos]=value;
}

bool crMultiSwitch::getValue(unsigned int switchSet, unsigned int pos) const
{
    if (switchSet>=m_values.size()) return false;

    const ValueList& values = m_values[switchSet];
    if (pos>=values.size()) return false;

    return values[pos];
}

bool crMultiSwitch::getChildValue(const CRCore::crNode* child, unsigned int switchSet) const
{
    if (switchSet>=m_values.size()) return false;

    // find the child's position.
    unsigned int pos=getChildIndex(child);
    if (pos==m_childArray.size()) return false;
    
    const ValueList& values = m_values[switchSet];
    return values[pos];
}

void crMultiSwitch::expandToEncompassSwitchSet(unsigned int switchSet)
{
    if (switchSet>=m_values.size())
    {
        // need to expand arrays.
        unsigned int originalSize = m_values.size();
        m_values.resize(switchSet+1);
        for(unsigned int i=originalSize;i<=switchSet;++i)
        {
            ValueList& values = m_values[i];
            values.resize(m_childArray.size(),m_newChildDefaultValue);
        }
    }
}

bool crMultiSwitch::setAllChildrenOff(unsigned int switchSet)
{
    m_newChildDefaultValue = false;

    expandToEncompassSwitchSet(switchSet);

    ValueList& values = m_values[switchSet];
    for(ValueList::iterator itr=values.begin();
        itr!=values.end();
        ++itr)
    {
        *itr = false;
    }
    return true;
}

bool crMultiSwitch::setAllChildrenOn(unsigned int switchSet)
{
    m_newChildDefaultValue = true;

    expandToEncompassSwitchSet(switchSet);

    ValueList& values = m_values[switchSet];
    for(ValueList::iterator itr=values.begin();
        itr!=values.end();
        ++itr)
    {
        *itr = true;
    }
    return true;
}

bool crMultiSwitch::setSingleChildOn(unsigned int switchSet, unsigned int pos)
{
    expandToEncompassSwitchSet(switchSet);

    ValueList& values = m_values[switchSet];
    for(ValueList::iterator itr=values.begin();
        itr!=values.end();
        ++itr)
    {
        *itr = false;
    }
    setValue(switchSet, pos,true);
    return true;
}


void crMultiSwitch::setSwitchSetList(const SwitchSetList& switchSetList)
{
    expandToEncompassSwitchSet(switchSetList.size());

    m_values = switchSetList;
}

void crMultiSwitch::setValueList(unsigned int switchSet, const ValueList& values)
{
    expandToEncompassSwitchSet(switchSet);

    m_values[switchSet] = values;
}

bool crMultiSwitch::removeSwitchSet(unsigned int switchSet)
{
	int count = m_values.size();
    if(count<=1 || switchSet>=count)
		return false;
	SwitchSetList::iterator itr = m_values.begin();
	m_values.erase(itr+switchSet);
	count--;
	if(m_activeSwitchSet>=count)
		m_activeSwitchSet = count-1;
	return true;
}

void crMultiSwitch::addParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		m_activeSwitchSet = atoi(str.c_str());
		break;
	}
	crGroup::addParam(i-1,str);
}