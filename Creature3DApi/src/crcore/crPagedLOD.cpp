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
#include <CRCore/crPagedLOD.h>
#include <CRCore/crCullStack.h>
#include <CRCore/crNotify.h>
#include <CRCore/crFilterRenderManager.h>

#include <CRCore/crBrain.h>

#include <algorithm>

using namespace CRCore;

crPagedLOD::PerRangeData::PerRangeData():
    m_priorityOffset(0.0f),
    m_priorityScale(1.0f),
    m_timeStamp(0.0f),
	m_frameNumber(0){}

crPagedLOD::PerRangeData::PerRangeData(const PerRangeData& prd):
    m_filename(prd.m_filename),
    m_priorityOffset(prd.m_priorityOffset),
    m_priorityScale(prd.m_priorityScale),
    m_timeStamp(prd.m_timeStamp),
	m_frameNumber(prd.m_frameNumber){}

crPagedLOD::PerRangeData& crPagedLOD::PerRangeData::operator = (const PerRangeData& prd)
{
    if (this==&prd) return *this;
    m_filename = prd.m_filename;
    m_priorityOffset = prd.m_priorityOffset;
    m_priorityScale = prd.m_priorityScale;
    m_timeStamp = prd.m_timeStamp;
	m_frameNumber = prd.m_frameNumber;
    return *this;
}

crPagedLOD::crPagedLOD()
{
    m_frameNumberOfLastTraversal = 0;
    m_centerMode = USER_DEFINED_CENTER;
    m_radius = -1;
    m_numChildrenThatCannotBeExpired = 0;
}

crPagedLOD::crPagedLOD(const crPagedLOD& plod,const crCopyOp& copyop):
    crLod(plod,copyop),
    m_frameNumberOfLastTraversal(plod.m_frameNumberOfLastTraversal),
    m_numChildrenThatCannotBeExpired(plod.m_numChildrenThatCannotBeExpired),
    m_perRangeDataList(plod.m_perRangeDataList)
{
}

crPagedLOD::~crPagedLOD()
{

}

void crPagedLOD::setDatabasePath(const std::string& path)
{
    m_databasePath = path;
    if (!m_databasePath.empty())
    {
        char& lastCharacter = m_databasePath[m_databasePath.size()-1];
        const char unixSlash = '/';
        const char winSlash = '\\';

        if (lastCharacter==winSlash)
        {
            lastCharacter = unixSlash;
        }
        else if (lastCharacter!=unixSlash)
        {
            m_databasePath += unixSlash;
        }

/*        
        // make sure the last character is the appropriate slash 
#ifdef WIN32       
        if (lastCharacter==unixSlash)
        {
            lastCharacter = winSlash;
        }
        else if (lastCharacter!=winSlash)
        {
            m_databasePath += winSlash;
        }
#else
        if (lastCharacter==winSlash)
        {
            lastCharacter = unixSlash;
        }
        else if (lastCharacter!=unixSlash)
        {
            m_databasePath += unixSlash;
        }
#endif
*/
    }
}


void crPagedLOD::traverse(crNodeVisitor& nv)
{
    // set the frame number of the traversal so that external nodes can find out how active this
    // node is.
    //if (nv.getFrameStamp()) setFrameNumberOfLastTraversal(nv.getFrameStamp()->getFrameNumber());

	/////临时
	//if(CRCore::crBrain::getInstance()->getDisablePagedLod())
	//	return;
	/////临时end

    double timeStamp = crFrameStamp::getInstance()->getReferenceTime();
	int frameNumber = crFrameStamp::getInstance()->getFrameNumber();
	bool updateTimeStamp = false;
	bool needRecordActiveChildren = false;
	if( nv.getVisitorType() == CRCore::crNodeVisitor::CULL_VISITOR )
	{
		//try
		//{
		CRCore::crCullStack* cullStack = dynamic_cast<CRCore::crCullStack*>(&nv);
		if(cullStack->getRenderMode() == crCullStack::NormalRender)
		{	
			if(m_frameNumber<frameNumber)
			{
				m_activeChildVecMutex.acquire();
				m_futureActiveChildVec->resize(0);
				//m_futureActiveChildVec->clear();
				m_activeChildVecMutex.release();
				needRecordActiveChildren = true;
				m_frameNumber = frameNumber;
			}
		}
		//else
		//{//
		//	//std::for_each(m_childArray.begin(),m_childArray.end(),crNodeAcceptOp(nv));
		//	for( ActiveChildVec::iterator itr = m_thisActiveChildVec->begin();
		//		itr != m_thisActiveChildVec->end();
		//		++itr )
		//	{
		//		m_childArray[*itr]->accept(nv);
		//	}
		//	return;
		//}
		//}
		//catch (...)
		//{
		//	CRCore::notify(CRCore::ALWAYS)<<"crPagedLOD ShadowMapRender error "<<this->getName()<<std::endl;
		//}
		updateTimeStamp = true;//cullStack->getRenderMode() < crCullStack::ShadowMapRender;
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
					//std::for_each(m_childArray.begin(),m_childArray.end(),crNodeAcceptOp(nv));
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
					//try
					//{			
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
					//int lastChildTraversed = -1;
					bool needToLoadChild = false;
					for(unsigned int i=0;i<m_rangeList.size();++i)
					{   
						if (m_rangeList[i].first<=required_range && required_range<m_rangeList[i].second)
						{
							if (i<m_childArray.size())
							{
								if (updateTimeStamp) 
								{
									m_perRangeDataList[i].m_timeStamp=timeStamp;
									m_perRangeDataList[i].m_frameNumber = frameNumber;
								}

								//m_childArray[i]->accept(nv);
								//lastChildTraversed = (int)i;
								//if(needRecordActiveChildren)
								//{
								m_futureActiveChildVec->push_back(i);
								//}
							}
							else
							{
								needToLoadChild = true;
							}
						}
						//else if(m_perRangeDataList[i].m_frameNumber == frameNumber-1 && frameNumber > 1)
						//{//上一帧还是显示的，采用半透明过渡
						//                m_childArray[i]->accept(nv);
						//}
					}

					crNode *child;
					for( ActiveChildVec::iterator itr = m_thisActiveChildVec->begin();
						itr != m_thisActiveChildVec->end();
						++itr )
					{
						child = getChild(*itr);
						if(child) child->accept(nv);
					}

					//if(nv.getFrameStamp()&&(/*lastChildTraversed>-1*/m_thisActiveChildVec->empty()||needToLoadChild))
					if(m_thisActiveChildVec->empty()||needToLoadChild)
					{
						setFrameNumberOfLastTraversal(crFrameStamp::getInstance()->getFrameNumber());
					}

					if (needToLoadChild)
					{
						unsigned int numChildren = m_childArray.size();

						// select the last valid child.
						//           if (numChildren>0 && ((int)numChildren-1)!=lastChildTraversed)
						//           {
						//               if (updateTimeStamp) m_perRangeDataList[numChildren-1].m_timeStamp=timeStamp;
						//              
						//               //m_childArray[numChildren-1]->accept(nv);
						//if(needRecordActiveChildren)
						//{
						//	m_futureActiveChildVec->push_back(numChildren-1);
						//}
						//           }

						// now request the loading of the next unload child.
						if (nv.getDatabaseRequestHandler() && numChildren<m_perRangeDataList.size())
						{
							// compute priority from where abouts in the required range the distance falls.
							float priority = (m_rangeList[numChildren].second-required_range)/(m_rangeList[numChildren].second-m_rangeList[numChildren].first);

							// modify the priority according to the child's priority offset and scale.
							priority = m_perRangeDataList[numChildren].m_priorityOffset + priority * m_perRangeDataList[numChildren].m_priorityScale;

							//CRCore::notify(CRCore::ALWAYS)<<"crPagedLOD::traverse  requestNodeFile "<<m_perRangeDataList[numChildren].m_filename<<std::endl;

							if (m_databasePath.empty())
							{
								nv.getDatabaseRequestHandler()->requestNodeFile(m_perRangeDataList[numChildren].m_filename,this,priority/*,nv.getFrameStamp()*/);
							}
							else
							{
								// prepend the databasePath to the childs filename.
								nv.getDatabaseRequestHandler()->requestNodeFile(m_databasePath+m_perRangeDataList[numChildren].m_filename,this,priority/*,nv.getFrameStamp()*/);
							}
						}
						/////has something not loaded
						if(/*needRecordActiveChildren && */m_thisActiveChildVec->empty())
						{
							swapActiveChildVec();
						}
					}
					else
					{
						//if(needRecordActiveChildren)
						//{
						swapActiveChildVec();
						//}
					}
					//         }
					//catch (...)
					//{
					//	CRCore::notify(CRCore::ALWAYS)<<"crPagedLOD  error "<<this->getName()<<std::endl;
					//}
				}
			}
            break;
        }
        default:
            break;
    }
}


void crPagedLOD::childRemoved(unsigned int pos, unsigned int numChildrenToRemove)
{
    //crLod::childRemoved(pos, numChildrenToRemove);
}

void crPagedLOD::childInserted(unsigned int pos)
{
    //crLod::childInserted(pos);
}

void crPagedLOD::rangeRemoved(unsigned int pos, unsigned int numChildrenToRemove)
{
    //crLod::rangeRemoved(pos, numChildrenToRemove);
}

void crPagedLOD::rangeInserted(unsigned int pos)
{
    //crLod::rangeInserted(pos);
    //expandPerRangeDataTo(pos);
}

void crPagedLOD::expandPerRangeDataTo(unsigned int pos)
{
    if (pos>=m_perRangeDataList.size()) 
		m_perRangeDataList.resize(pos+1);
}

bool crPagedLOD::addChild( crNode *child )
{
    if (crLod::addChild(child))
    {
        expandPerRangeDataTo(m_childArray.size()-1);
        return true;
    }
    return false;
}

bool crPagedLOD::addChild(crNode *child, float min, float max)
{
    if (crLod::addChild(child,min,max))
    {
        expandPerRangeDataTo(m_childArray.size()-1);
        return true;
    }
    return false;
}


bool crPagedLOD::addChild(crNode *child, float min, float max,const std::string& filename, float priorityOffset, float priorityScale)
{
    if (crLod::addChild(child,min,max))
    {
        setFileName(m_childArray.size()-1,filename);
        setPriorityOffset(m_childArray.size()-1,priorityOffset);
        setPriorityScale(m_childArray.size()-1,priorityScale);
        return true;
    }
    return false;
}

bool crPagedLOD::removeChild(unsigned int pos,unsigned int numChildrenToRemove)
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
		m_perRangeDataList.erase(m_perRangeDataList.begin()+pos,m_perRangeDataList.begin()+endOfRemoveRange);
	}

	return crGroup::removeChild(pos,numChildrenToRemove);    
}

//bool crPagedLOD::removeChild( crNode *child )
//{
//    // find the child's position.
//    unsigned int pos=getChildIndex(child);
//    if (pos==m_childArray.size()) return false;
//    
//    if (pos<m_rangeList.size()) m_rangeList.erase(m_rangeList.begin()+pos);
//    if (pos<m_perRangeDataList.size()) m_perRangeDataList.erase(m_perRangeDataList.begin()+pos);
//    
//    return crGroup::removeChild(child);    
//}

bool crPagedLOD::removeExpiredChildren(double expiryTime,NodeArray& removedChildren,std::map< std::string, CRCore::ref_ptr<CRCore::crNode> > &loadedMap)
{
    if (m_childArray.size()>m_numChildrenThatCannotBeExpired)
    {
		std::string& fileName = m_perRangeDataList[m_childArray.size()-1].m_filename;
        if (!fileName.empty() && m_perRangeDataList[m_childArray.size()-1].m_timeStamp<expiryTime)
        {
            CRCore::crNode* nodeToRemove = m_childArray[m_childArray.size()-1].get();
			if(nodeToRemove->referenceCount()<=2)
			{
                removedChildren.push_back(nodeToRemove);
				loadedMap.erase(fileName);
			}
            return crGroup::removeChild(nodeToRemove);
        }
    }
    return false;
}
