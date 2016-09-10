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
#include <CRCore/crLod.h>
#include <CRCore/crCullStack.h>
#include <CRCore/crNotify.h>

#include <algorithm>

using namespace CRCore;

crLod::crLod():
    m_centerMode(USE_BOUNDING_SPHERE_CENTER),
    m_radius(-1.0f),
    m_rangeMode(DISTANCE_FROM_EYE_POINT),
	m_frameNumber(0)
{
	m_thisActiveChildVec = new ActiveChildVec;
	m_futureActiveChildVec = new ActiveChildVec;
}
crLod::~crLod()
{
	delete m_thisActiveChildVec;
	delete m_futureActiveChildVec;
}

crLod::crLod(const crLod& lod,const crCopyOp& copyop):
        crGroup(lod,copyop),
        m_centerMode(lod.m_centerMode),
        m_userDefinedCenter(lod.m_userDefinedCenter),
        m_radius(lod.m_radius),
        m_rangeMode(lod.m_rangeMode),
        m_rangeList(lod.m_rangeList),
		m_frameNumber(0)
{
	m_thisActiveChildVec = new ActiveChildVec;
	m_futureActiveChildVec = new ActiveChildVec;
}

void crLod::swapActiveChildVec()
{
	m_activeChildVecMutex.acquire();
    ActiveChildVec *temp = m_thisActiveChildVec;
	m_thisActiveChildVec = m_futureActiveChildVec;
	m_futureActiveChildVec = temp;
	m_activeChildVecMutex.release();
}

void crLod::traverse(crNodeVisitor& nv)
{
	bool needRecordActiveChildren = false;
	if( nv.getVisitorType() == CRCore::crNodeVisitor::CULL_VISITOR )
	{
		CRCore::crCullStack* cullStack = dynamic_cast<CRCore::crCullStack*>(&nv);
        if(cullStack->getRenderMode() == crCullStack::NormalRender)
		{
			int thisFrameNumber = crFrameStamp::getInstance()->getFrameNumber();
			if(m_frameNumber<thisFrameNumber)
			{
				m_activeChildVecMutex.acquire();
				m_futureActiveChildVec->resize(0);
				//m_futureActiveChildVec->clear();
				m_activeChildVecMutex.release();
		        needRecordActiveChildren = true;
				m_frameNumber = thisFrameNumber;
			}
		}
		//else
		//{
		//	//std::for_each(m_childArray.begin(),m_childArray.end(),crNodeAcceptOp(nv));
		//	for( ActiveChildVec::iterator itr = m_thisActiveChildVec->begin();
		//		itr != m_thisActiveChildVec->end();
		//		++itr )
		//	{
		//		m_childArray[*itr]->accept(nv);
		//	}
		//	return;
		//}
	}
    switch(nv.getTraversalMode())
    {
        case(crNodeVisitor::TRAVERSE_ALL_CHILDREN):
            std::for_each(m_childArray.begin(),m_childArray.end(),crNodeAcceptOp(nv));
            break;
        case(crNodeVisitor::TRAVERSE_ACTIVE_CHILDREN):
        {
			if(crDisplaySettings::instance()->getDisableLOD())
				 std::for_each(m_childArray.begin(),m_childArray.end(),crNodeAcceptOp(nv));
			else
			{
				CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_activeChildVecMutex);
				if(!needRecordActiveChildren)
				{
					crNode *child;
					for( ActiveChildVec::iterator itr = m_thisActiveChildVec->begin();
						itr != m_thisActiveChildVec->end();
						++itr )
					{
						child = getChild(*itr);
						if(child) child->accept(nv);
					}
				}
				else
				{
					float required_range = 0;
					//if(!crDisplaySettings::instance()->getDisableLOD())
					//{
					if (m_rangeMode==DISTANCE_FROM_EYE_POINT)
					{
						required_range = nv.getDistanceToEyePoint(getCenter(),true);
					}
					else
					{
						CRCore::crCullStack* cullStack = dynamic_cast<CRCore::crCullStack*>(&nv);
						if (cullStack)
						{
							required_range = cullStack->pixelSize(getBound());
						}
						else
						{
							// fallback to selecting the highest res tile by
							// finding out the max range
							for(unsigned int i=0;i<m_rangeList.size();++i)
							{
								required_range = CRCore::minimum(required_range,m_rangeList[i].first);
							}
						}
					}
					//}

					unsigned int numChildren = m_childArray.size();
					if (m_rangeList.size()<numChildren) numChildren=m_rangeList.size();

					for(unsigned int i=0;i<numChildren;++i)
					{    
						if (m_rangeList[i].first<=required_range && required_range<m_rangeList[i].second)
						{
							//if(needRecordActiveChildren)
							//{
							m_futureActiveChildVec->push_back(i);
							//}
							//m_childArray[i]->accept(nv);
						}
					}
					crNode *child;
					for( ActiveChildVec::iterator itr = m_thisActiveChildVec->begin();
						itr != m_thisActiveChildVec->end();
						++itr )
					{
						//m_childArray[*itr]->accept(nv);
						child = getChild(*itr);
						if(child) child->accept(nv);
					}
					//if(needRecordActiveChildren)
					//{
					swapActiveChildVec();
					//}
				}
			}
           break;
        }
        default:
            break;
    }
}

bool crLod::computeBound() const
{
	m_boundSphere.init();
	m_bbox.init();

    if (m_centerMode==USER_DEFINED_CENTER && m_radius>=0.0f)
    {
        m_boundSphere.set(m_userDefinedCenter,m_radius);
		m_bbox.expandBy(m_boundSphere);
		m_bBoundSphere_computed = true;
		return true;
    }
    else
    {
		//CRCore::notify(CRCore::ALWAYS)<<" crLod::computeBound() = "<<m_boundSphere.radius()<<std::endl;
        return crGroup::computeBound();
    }
}

void crLod::dirtyBound()
{
	if (m_centerMode==USER_DEFINED_CENTER && m_radius>=0.0f)
		return;

	if (m_bBoundSphere_computed)
	{
		m_bBoundSphere_computed = false;

		// dirty parent bounding sphere's to ensure that all are valid.
		for(ParentArray::iterator itr=m_parentArray.begin();
			itr!=m_parentArray.end();
			++itr)
		{
			(*itr)->dirtyBound();
		}

	}
}

bool crLod::addChild( crNode *child )
{
	if (crGroup::addChild(child))
	{
		if (m_childArray.size() > m_rangeList.size()) 
		{
			//float minRange = !m_rangeList.empty()?
			//	minRange=m_rangeList.back().first : 0.0f;

			//float maxRange = !m_rangeList.empty()?
			//	maxRange=m_rangeList.back().second : 0.0f;
   //         
			//m_rangeList.resize(m_childArray.size(),MinMaxPair(minRange,maxRange));

			//float maxRange = !m_rangeList.empty()?
			//	m_rangeList.back().second : 0.0f;

			//m_rangeList.resize(m_childArray.size(),MinMaxPair(maxRange,maxRange));
			m_rangeList.resize(m_childArray.size(),MinMaxPair(0.0f,100.0f));
		}

		//m_radius = CRCore::maximum(child->getBound().radius(),m_radius);
		return true;
	}
	return false;
}

void crLod::childRemoved(unsigned int /*pos*/, unsigned int /*numChildrenToRemove*/)
{
	//std::cout<<"crLod::childRemoved("<<pos<<","<<numChildrenToRemove<<")"<<std::endl;
}

void crLod::childInserted(unsigned int /*pos*/)
{
	//std::cout<<"crLod::childInserted("<<pos<<")"<<std::endl;
}

bool crLod::addChild(crNode *child, float min, float max)
{
	if (crGroup::addChild(child))
	{
		if (m_childArray.size() > m_rangeList.size()) 
			m_rangeList.resize(m_childArray.size(),MinMaxPair(min,max));
		m_rangeList[m_childArray.size()-1].first = min;
		m_rangeList[m_childArray.size()-1].second = max;

		//m_radius = CRCore::maximum(m_radius,child->getBound().radius());
		return true;
	}
	return false;
}

//bool crLod::removeChild( crNode *child )
//{
//	// find the child's position.
//	unsigned int pos=getChildIndex(child);
//	if (pos == m_childArray.size()) return false;
//
//	m_rangeList.erase(m_rangeList.begin()+pos);
//
//	return crGroup::removeChild(child);    
//}

bool crLod::removeChild(unsigned int pos,unsigned int numChildrenToRemove)
{
	if (pos<m_childArray.size() && numChildrenToRemove>0)
	{
		unsigned int endOfRemoveRange = pos+numChildrenToRemove;
		if (endOfRemoveRange>m_childArray.size())
		{
			//notify(DEBUG_INFO)<<"Warning: crLod::removeChild(i,numChildrenToRemove) has been passed an excessive number"<<std::endl;
			//notify(DEBUG_INFO)<<"         of chilren to remove, trimming just to end of child list."<<std::endl;
			endOfRemoveRange=m_childArray.size();
		}

		m_rangeList.erase(m_rangeList.begin()+pos,m_rangeList.begin()+endOfRemoveRange);
	}

	return crGroup::removeChild(pos,numChildrenToRemove);    
}

void crLod::setRange(unsigned int childNo, float min,float max)
{
	if (childNo >= m_rangeList.size()) 
		m_rangeList.resize(childNo+1,MinMaxPair(min,max));
	m_rangeList[childNo].first=min;
	m_rangeList[childNo].second=max;

	//m_radius = (m_radius>max) ? m_radius : max;
}
