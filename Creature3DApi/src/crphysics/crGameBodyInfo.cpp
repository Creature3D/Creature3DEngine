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
#include <CRPhysics/crGameBodyInfo.h>
#include <CRPhysics/crWeaponNode.h>
#include <CRCore/crArgumentParser.h>
//#include <CRCore/crLightSource.h>
//#include <CRCore/thread/crScopedLock.h>
using namespace CRCore;
using namespace CRPhysics;

crGameBodyInfo::crGameBodyInfo():
  //m_sceneid(0),
  m_hp(100),
  m_maxhp(100),
  m_defend(0),
  m_team(1),
  m_experience(0),
  m_maxDetectCount(5),//最多侦测目标的数量
  m_maxDetectRange(100),//最大侦测距离
  m_taskPointRange(100.0f),
  m_stallVelocity(100.0f),
  m_stealth(1.0f),
  m_manipulatorMode(MM_FPS),
  m_killedCount(0),
  m_deathCount(0),
  m_level(0)
{
	setTaskPointRange(0.01f);
}

crGameBodyInfo::crGameBodyInfo(const crGameBodyInfo& gameBodyInfo,const crCopyOp& copyop):
  crBase(gameBodyInfo,copyop),
  //m_sceneid(gameBodyInfo.m_sceneid),
  m_hp(gameBodyInfo.m_hp),
  m_maxhp(gameBodyInfo.m_maxhp),
  m_defend(gameBodyInfo.m_defend),
  m_team(gameBodyInfo.m_team),
  m_killedCount(gameBodyInfo.m_killedCount),
  m_deathCount(gameBodyInfo.m_deathCount),
  m_level(gameBodyInfo.m_level),
  m_experience(gameBodyInfo.m_experience),
  m_maxDetectRange(gameBodyInfo.m_maxDetectRange),//最大侦测距离
  m_maxDetectCount(gameBodyInfo.m_maxDetectCount),//最多侦测目标的数量
  //m_currentLockedTargetIndex(gameBodyInfo.m_currentLockedTargetIndex),
  m_taskPointRange(gameBodyInfo.m_taskPointRange),
  m_stallVelocity(gameBodyInfo.m_stallVelocity),
  m_stealth(gameBodyInfo.m_stealth),
  m_manipulatorMode(gameBodyInfo.m_manipulatorMode)
{

}

void crGameBodyInfo::setTeam(int team)
{
	m_team = team;
}

void crGameBodyInfo::kill()
{
    m_hp = -1;
}

void crGameBodyInfo::addExcution(int excution)
{
	//CRCore::notify(CRCore::FATAL)<<" crGameBodyInfo::addExcution() name = "<<getName()<<" excution = "<<excution<<std::endl;
    if(m_defend==0)
		m_hp -= excution;
	else
	{
		int x = m_defend * 1.25f - excution;
		if(x > 0)
		{
			m_defend -= excution * 0.8f;
			m_hp -= excution * 0.2f;
		}
		else
		{
			m_defend = 0;
            m_hp -= (excution+x) * 0.2f - x;
		}
	}
}



void crGameBodyInfo::renascence()
{
	m_hp = m_maxhp;
	m_defend = 0;
}

void crGameBodyInfo::increaseKilledCount()
{
    m_killedCount++;
}

int crGameBodyInfo::getKilledCount()
{
    return m_killedCount;
}

void crGameBodyInfo::increaseDeathCount()
{
    m_deathCount++;
}

int crGameBodyInfo::getDeathCount()
{
    return m_deathCount;
}

void crGameBodyInfo::setStallVelocity(float stallVelocity)
{
	m_stallVelocity = stallVelocity;
}

void crGameBodyInfo::setStealth(float stealth)
{
	m_stealth = stealth;
}

void crGameBodyInfo::setTargetRange(float range)
{
    m_targetRange.set(-range,range);
}

void crGameBodyInfo::addParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		if(str.compare("MM_EDIT") == 0)
			m_manipulatorMode = MM_FPS;
		else if(str.compare("MM_FPS") == 0)
			m_manipulatorMode = MM_FPS;
		else if(str.compare("MM_RPG") == 0)
			m_manipulatorMode = MM_RPG;
		else if(str.compare("MM_RTS") == 0)
			m_manipulatorMode = MM_RTS;
		else if(str.compare("MM_TANK") == 0)
			m_manipulatorMode = MM_TANK;
		break;
	case 1:
		setTaskPointRange(atof(str.c_str()));
		break;
	case 2:
		setTargetRange(atof(str.c_str()));
		break;
	case 3:
		setMaxDetectCount(atoi(str.c_str()));
		break;
	case 4:
		setMaxDetectRange(atof(str.c_str()));
		break;
	case 5:
		m_hp = atoi(str.c_str());
		break;
	case 6:
		m_maxhp = atoi(str.c_str());
		break;
	case 7:
		m_defend = atoi(str.c_str());
		break;
	case 8:
		m_team = atoi(str.c_str());
		break;
	case 9:
        setInitExperience(atoi(str.c_str()));
		break;
	case 10:
		m_stallVelocity = atof(str.c_str());
		break;
	case 11:
		m_stealth = atof(str.c_str());
		break;
	}
}

void crGameBodyInfo::setTaskPointRange(float taskPointRange)
{
    m_taskPointRange = taskPointRange;
}

void crGameBodyInfo::setInitExperience(int experience)
{
    increaseExperience(experience);
}

void crGameBodyInfo::levelUp()
{//m_level 与 m_experience之间的关系
    
}

void crGameBodyInfo::setMaxDetectCount(int detectCount)
{
	m_maxDetectCount = detectCount;
}

void crGameBodyInfo::setMaxDetectRange(float detectRange)
{
	m_maxDetectRange = detectRange;
}

//void crGameBodyInfo::changeLockedTarget()
//{
//	m_currentLockedTargetIndex++;
//	if( m_currentLockedTargetIndex > m_maxDetectCount )
//		m_currentLockedTargetIndex = 0;
//}

void crGameBodyInfo::updateLockedTarget(int count, unsigned int weaponMask)
{//在bot释放后，会产生内存错误，该函数不再使用
	//CRCore::ScopedLock<CRCore::crMutex> lock(m_bodyInfoMutex);
	//CRCore::notify(CRCore::ALWAYS)<<"crGameBodyInfo::updateLockedTarget count = "<<count<< std::endl;
	//m_bodyInfoMutex.lock();
	m_lockedTargetSet.clear();
	int detectCount = CRCore::minimum((int)(m_detectMap.size()),m_maxDetectCount);
	std::vector<DetectParam> toErase;
	int i = 0;
	if(detectCount < count)
	{
        for( DetectMap::iterator itr = m_detectMap.begin();
			 itr != m_detectMap.end() && i<detectCount;
			 ++itr )
		{
			if(weaponMask & itr->second->getAttributeMask())
			{
				toErase.push_back(itr->first);
				m_lockedTargetSet.insert(itr->second.get());
			    i++;
			}
		}
		//m_detectMap.clear();
	}
	else
	{
		for( DetectMap::iterator itr = m_detectMap.begin();
			 itr != m_detectMap.end() && i<count;
			 ++itr)
		{
			if(weaponMask & itr->second->getAttributeMask())
			{
				toErase.push_back(itr->first);
				m_lockedTargetSet.insert(itr->second.get());
				i++;
			}
		}
		//while(selected < count)
		//{
		//	//m_currentLockedTargetIndex %= size;
		//	for(int i = 0; i<m_currentLockedTargetIndex; ++i)
		//		itr++;
		//	if(itr != m_detectMap.end())
		//	{
		//		m_lockedTargetSet.insert(itr->second.get());
		//		//m_detectMap.erase(itr->first);
		//	}
		//	selected++;
		//	//m_currentLockedTargetIndex++;
		//}
	}
	for( std::vector<DetectParam>::iterator eitr = toErase.begin();
		eitr != toErase.end();
		++eitr )
	{
		m_detectMap.erase(*eitr);
	}
	//m_bodyInfoMutex.unlock();
	//CRCore::notify(CRCore::ALWAYS)<<"crGameBodyInfo::updateLockedTarget size = "<<m_lockedTargetSet.size()<< std::endl;
}

//crViewMatterObject *crGameBodyInfo::getCurrentLockedTarget()
//{
//	return m_lockedTarget.get();
//}

//void crGameBodyInfo::addLockedTargetByName(const std::string &name)
//{
// //   if(name.compare(m_lockedTarget->getName())!=0)
//	//{
//        for( DetectMap::iterator itr = m_detectMap.begin();
//			 itr != m_detectMap.end();
//			 ++itr )
//		{
//			//if(itr->second != m_lockedTarget)
//			//{
//                if(name.compare(itr->second->getName()) == 0)
//				{
//					//m_lockedTarget = itr->second.get();
//					m_lockedTargetSet.insert(itr->second.get());
//					return;
//				}
//			//}
//		}
//	//}
//}

crGameBodyInfo::ManipulatorMode crGameBodyInfo::getManipulatorMode() const 
{
	return m_manipulatorMode; 
}

bool crGameBodyInfo::isNodeSelected(CRCore::crNode *node) const
{
	for( SelectNodeVec::const_iterator itr = m_selectNodeVec.begin();
		itr != m_selectNodeVec.end();
		++itr )
	{
		if((*itr) == node) return true;
	}
	return false;
}
bool crGameBodyInfo::isNodeSelected(const std::string &nodeName) const
{
	for( SelectNodeVec::const_iterator itr = m_selectNodeVec.begin();
		itr != m_selectNodeVec.end();
		++itr )
	{
		if(nodeName.compare((*itr)->getName()) == 0) return true;
	}
	return false;
}
void crGameBodyInfo::selectNode(CRCore::crNode *node)
{
    node->doEvent(WCH_SELECTNODE);
	m_selectNodeVec.push_back(node);
}

void crGameBodyInfo::selectNodeEvent(__int64 kbmsg, __int64 param)
{
	for( SelectNodeVec::iterator itr = m_selectNodeVec.begin();
		 itr != m_selectNodeVec.end();
		 ++itr )
	{
		(*itr)->doEvent(kbmsg, param);
	}
}

void crGameBodyInfo::unSelectNode(CRCore::crNode *node)
{
	for( SelectNodeVec::iterator itr = m_selectNodeVec.begin();
		itr != m_selectNodeVec.end();
		++itr )
	{
		if((*itr) == node)
		{
			node->doEvent(WCH_UNSELECTNODE);
            m_selectNodeVec.erase(itr);
			break;
		}
	}
}

void crGameBodyInfo::unSelectAll()
{
    selectNodeEvent(WCH_UNSELECTNODE);
	m_selectNodeVec.clear();
	if(m_dataClass.valid())
		m_dataClass->inputParam(-1,NULL);
}

void crGameBodyInfo::translateSelectedNodes(CRCore::crMatrix &transMatrix)
{
	crMatrixTransform *matrixTransform;
	crMatterObject *matterObject;
	//crLightSource *ls;
	for( SelectNodeVec::iterator itr = m_selectNodeVec.begin();
		itr != m_selectNodeVec.end();
		++itr )
	{
		matrixTransform = dynamic_cast<crMatrixTransform *>(itr->get());
		if(matrixTransform)
		{
			crMatrix mat = matrixTransform->getMatrix();
			crVector3 trans = matrixTransform->getTrans();//matrixTransform->getBound().center();
			crMatrix put_mat = crMatrix::translate(trans);
			crMatrix inv_put = crMatrix::inverse(put_mat);
			mat.postMult(inv_put);
			mat.postMult(transMatrix);
			mat.postMult(put_mat);

			matterObject = dynamic_cast<crMatterObject *>(itr->get());
			if(matterObject)
			{
				matterObject->setMatterObjectMatrix(mat,CRPhysics::crMatterObject::MD_FullMatrix);

				CRCore::crVector3 pos = mat.getTrans();
				std::string command = matterObject->getDescriptionsInString();
				CRCore::crArgumentParser::updateOrAddKeyValue(command,"GeomPosition",pos);
				CRCore::crQuat q;
				mat.get(q);
				CRCore::crArgumentParser::updateOrAddKeyValue(command,"GeomRotation",q.asVec4());
				matterObject->setDescriptionsInString(command);
			}
			else matrixTransform->setMatrix(mat);
			
			//continue;
		}

		//ls = dynamic_cast<crLightSource *>(itr->get());
		//if(ls)
		//{
		//	crLight *light = dynamic_cast<crLight *>(ls->getLight());
		//	assert(!light);

		//	crMatrix rotMat = transMatrix;
		//	rotMat.setTrans(0.0f,0.0f,0.0f);
		//	crVector3& dir = light->getDirection();
		//	dir = (dir * rotMat).normalize();
		//	crVector4 &pos = light->getPosition();
		//	crVector3 position(pos[0],pos[1],pos[2]);
		//	position = position * transMatrix;
		//	light->setPosition(crVector4(position,1.0f));

		//	ls->getLightParamStruct()->m_lsPosition = position;
		//	ls->getLightParamStruct()->m_lsDirection = dir;			
		//	continue;
		//}

	}
}
void crGameBodyInfo::scaleSelectedNodes(CRCore::crMatrix &scaleMatrix)
{
	crMatrixTransform *matrixTransform;
	crMatterObject *matterObject;
	//crLightSource *ls;
	for( SelectNodeVec::iterator itr = m_selectNodeVec.begin();
		itr != m_selectNodeVec.end();
		++itr )
	{
		if(dynamic_cast<crMatterObject *>(itr->get()))
		{
			matterObject = dynamic_cast<crMatterObject *>(itr->get());
			if(matterObject->getNumChildren() == 1)
			{
				matrixTransform = dynamic_cast<crMatrixTransform *>(matterObject->getChild(0));
			}
			if(!matrixTransform)
			{
				matrixTransform = new crMatrixTransform;
				matrixTransform->setName("ScaleMatrix");
				CRCore::NodeArray &childArray = matterObject->getChildArray();
				for( CRCore::NodeArray::iterator itr = childArray.begin();
					 itr != childArray.end();
					 ++itr )
				{
				    matrixTransform->addChild(itr->get());
				}
				matterObject->removeChild(0,matterObject->getNumChildren());
				matterObject->addChild(matrixTransform);
			}
		}
		else
		{
			matrixTransform = dynamic_cast<crMatrixTransform *>(itr->get());
		}
		if(matrixTransform)
		{
			crMatrix mat = matrixTransform->getMatrix();
			mat.setTrans(0.0f,0.0f,0.0f);
			crVector3 dir = (-Y_AXIS * mat).normalize();
			crVector3 trans = matrixTransform->getTrans();
			crMatrix put_mat = crMatrix::translate(trans);
			//crMatrix inv_put = crMatrix::inverse(put_mat);
			crMatrix rot_mat = crMatrix::rotate(-Y_AXIS,dir);
			crMatrix inv_rot = crMatrix::inverse(rot_mat);
			//mat.postMult(inv_put);
			mat.postMult(inv_rot);
			mat.postMult(scaleMatrix);
			mat.postMult(rot_mat);
			mat.postMult(put_mat);
			
			//crMatrix mat = matrixTransform->getMatrix();
			//crVector3 trans = matrixTransform->getBound().center();
			//crMatrix put_mat = crMatrix::translate(trans);
			//crMatrix inv_put = crMatrix::inverse(put_mat);
			//mat.postMult(inv_put);
			//mat.postMult(scaleMatrix);
			//mat.postMult(put_mat);

			matrixTransform->setMatrix(mat);
		}
	}
}
CRCore::crNode *crGameBodyInfo::getFirstSelectNode()
{
	return m_selectNodeVec.empty()?NULL:m_selectNodeVec[0].get();
}