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
#include "rbody/Creature3D/ReplicantBodyMgr.h"
#include "rcfg/ConfigScriptOO.h"
#include "rbody/NamedCoreBodies.h"
#include <CRCore/crNotify.h>
#include <CRCore/crBrain.h>
#include <CRIOManager/crLoadManager.h>
#include <sys/stat.h>
#include <algorithm>

using namespace CRCore;
using namespace rbody;
using namespace CRIOManager;

ref_ptr<ReplicantBodyMgr> ReplicantBodyMgr::m_instance = NULL;

ReplicantBodyMgr::ReplicantBodyMgr( void ):
m_nid(0)
{
}

ReplicantBodyMgr* ReplicantBodyMgr::instance()
{
	if (m_instance==NULL) 
	{
		m_instance = new ReplicantBodyMgr;
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();

    //static CRCore::ref_ptr<ReplicantBodyMgr> s_ReplicantBodyMgr = new ReplicantBodyMgr;
    //return s_ReplicantBodyMgr.get();
}

void ReplicantBodyMgr::clear()
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_characterSetMutex);
	NamedCoreBodies::getInstance()->clear();
    //m_instance=NULL;
	m_characterSet.clear();
}

ReplicantBodyMgr::~ReplicantBodyMgr()
{
  //_clear();
}

//void ReplicantBodyMgr::_clear()
//{
//  m_character_map.clear();
//}

//bool ReplicantBodyMgr::removeCharacter(const std::string& id)
//{
//  // Try to find the named character
//  CharacterIterator it = getIterator(id);
//  
//  // If we didn't find it return false
//  if (it == m_character_map.end())
//    return false;
//
//  // Remove the character from the map
//  m_character_map.erase(it);
//  return true;
//}
//
//
//bool ReplicantBodyMgr::removeCharacter(CreBodyNode *body)
//{
//  // Try to find the character by searching for the address
//  CharacterIterator it = getIterator(body);
//  if (it != m_character_map.end()) {
//    m_character_map.erase(it);
//    return true;
//  }
//
//  return false;
//}

CRCore::crGroup *ReplicantBodyMgr::readCharacterFile(const std::string& fileName, CRCore::crNode *collisionRoot)
{
  rcfg::ConfigScript cfg_script;
  if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,fileName))
  {
	  //CRCore::notify(CRCore::FATAL)<<"readCharacterFile: file open error,"<<fileName<<std::endl;
	  char gbuf[256];
	  sprintf(gbuf,"readCharacterFile: file open error,%s\n\0",fileName);
	  gDebugInfo->debugInfo(CRCore::FATAL,gbuf);
	  return NULL;
  }
  //std::string path;

  // Is the file available?
  //struct stat buf;
  //if (stat( fileName.c_str(), &buf ))
  //{
	 // CRCore::notify(CRCore::FATAL)<<"readCharacterFile: file open error"<<std::endl;
	 // return NULL;
  //}
  //else 
  //{ // Try to locate it with the REPLICANTBODY_ENVIRONMENT_VARIABLE
  //  m_filepathcontainer.setEnvironmentVariable(REPLICANTBODY_ENVIRONMENT_VARIABLE );
  //  path = m_filepathcontainer.findFile(fileName);
  //  if (path.empty())
		//CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::readCharacterFile Unable to locate file: "<<fileName<<std::endl;
  //    //throw std::runtime_error("ReplicantMgr::readCharacterFile: Unable to locate file: " + fileName);
  //}
    
  // Now Open the file
  //if (!rcfg.Open(fileName, header))
  //{
	 // CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::readCharacterFile: 打开文件"<<fileName<<"出错: "<<rcfg.GetLastError()<<std::endl;
	 // return NULL;
  //   // throw std::runtime_error("ReplicantMgr::readCharacterFile: " + rcfg.GetLastError());
  //}
  CRCore::crGroup* group = new CRCore::crGroup;
  ref_ptr<CreBodyNode> body;

  int no_characters=1;
  bool goon = false;
  //do {
    rcfg::ScopedPush sp(cfg_script, "Character", no_characters++);
    goon = sp.valid();
    if (goon) 
	{
      parseCharacter(cfg_script, fileName,body);
      group->addChild(body.get());
    }
  //} while (goon);

  return group;
}

void ReplicantBodyMgr::parseCharacter(rcfg::ConfigScript& rcfg, const std::string &fileName,CRCore::ref_ptr<rbody::CreBodyNode> &body)
{
  //std::string id;
  //if (!rcfg.Get("ID", id))
  //  throw std::runtime_error("ReplicantBodyMgr::parseCharacter(): " + rcfg.CurrentScope() + " struct is missing required field ID");
  
  std::string core_name;
  if (!rcfg.Get("CoreName", core_name))
    throw std::runtime_error("ReplicantBodyMgr::parseCharacter(): " + rcfg.CurrentScope() + " struct is missing required field CoreName");
   
  int enableTangentSpace = 0;
  rcfg.Get("EnableTangentSpace", enableTangentSpace);

  //std::vector<float> v_pos;
  //CRCore::crVector3 pos(0, 0, 0);
  //if (rcfg.Get("Position", v_pos)) {
  //  if (v_pos.size() != 3)
  //    throw std::runtime_error("ReplicantBodyMgr::parseCharacter(): " + rcfg.CurrentScope() + "Position should be a vector with 3 elements");

  //  pos.set(v_pos[0], v_pos[1], v_pos[2]);
  //}


  //CRCore::crMatrix mrot;

  //std::vector<float> dir;

  //if (rcfg.Get("Orientation", dir)) {
  //  if (dir.size() != 3)
  //    throw std::runtime_error("ReplicantBodyMgr::parseCharacter(): " + rcfg.CurrentScope() + "Orientation should be a vector with 3 elements");

  //  CRCore::crQuat q1(DegreesToRadians(dir[0]), CRCore::crVector3(1, 0, 0)), q2(DegreesToRadians(dir[1]), CRCore::crVector3(0, 1, 0)), q3(DegreesToRadians(dir[2]), CRCore::crVector3(0, 0, 1));
  //  mrot.set(q1*q2*q3);
  //}

  createCharacter(core_name, enableTangentSpace, fileName,body);
  rcfg.Pop();
  //body->parseMeshMaterial(rcfg);
  
  //std::string boneName,detailString;
  //int n = 1;
  //while(rcfg.Push("DetailCollideParse ",n++))
  //{
	 // rcfg.Get("BoneName", boneName);
	 // rcfg.Get("DetailString", detailString);
  //    body->insertDetailCollideParseMap(boneName,detailString);
	 // if (!rcfg.Pop())
		//  CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter() error "<<rcfg.GetLastError()<<std::endl;
  //}
  //CRCore::notify(CRCore::WARN) << "parseCharacter" << std::endl;

  //body->setMatrix(mrot * CRCore::crMatrix::translate(pos));

  //return body;
}


void ReplicantBodyMgr::createCharacter(const std::string& coreName,bool enableTangentSpace, const std::string &fileName,CRCore::ref_ptr<rbody::CreBodyNode> &body)
{
  //if (id.empty())
  //  throw std::runtime_error("ReplicantBodyMgr::createCharacter(): Empty string is not valid for an ID of a character");

  if (coreName.empty())
    throw std::runtime_error("ReplicantBodyMgr::createCharacter(): Empty string is not valid for a CoreName of a character");

  // Make sure it isnt taken
  //if (findCharacter(id))
  //  throw std::runtime_error("ReplicantBodyMgr::createCharacter(): ID: " + id + " already taken, needs to be unique");

  // Create a body from the core 
  //char id[128];
  //sprintf(id,"CreBody%d\0",getnid());

  body = new CreBodyNode(coreName, fileName, "CreBodyNode", enableTangentSpace);
  insertCharacter(body.get());
  // Give it the name
  //body->setName(id);

  // And store it in the character map
  //m_character_map[id]= body;

  //return body;
}


//CreBodyNode *ReplicantBodyMgr::findCharacter(const std::string& id)
//{
//
//  CharacterIterator it = getIterator(id);
//  if (it == m_character_map.end())  
//    return 0;
//
//  return (*it).second.get();
//}

//template <class T>
//class SetCollisionRoot {
//public:
//  SetCollisionRoot(CRCore::crNode *root) : m_root(root) {}
//
//  void operator() (T v) {
//    v.second->setCollisionRootNode(m_root);
//  }
//private:
//  CRCore::crNode *m_root;
//};


//void ReplicantBodyMgr::setCollisionRootNode(CharacterIterator begin, CharacterIterator end, CRCore::crNode *collisionRoot)
//{
//  std::for_each(begin, end, SetCollisionRoot<CharacterMap::value_type>(collisionRoot));
//}


//ReplicantBodyMgr::CharacterIterator ReplicantBodyMgr::getIterator(CreBodyNode *body)
//{
//  // Try to find the character by searching for the adress
//  CharacterIterator it;
//  for(it = m_character_map.begin(); it != m_character_map.end(); it++)
//    if ((*it).second == body) 
//      return it;
//
//  return m_character_map.end();
//}

void ReplicantBodyMgr::insertCharacter(rbody::CreBodyNode *character)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_characterSetMutex);
	m_characterSet.insert(character);
}
void ReplicantBodyMgr::removeCharacter(rbody::CreBodyNode *character)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_characterSetMutex);
    m_characterSet.erase(character);
}
int ReplicantBodyMgr::getnid()
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_characterSetMutex);
	return m_nid++;
}
void ReplicantBodyMgr::updateCharacter(float dt)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_characterSetMutex);
	CRCore::ref_ptr<rbody::CreBodyNode> setItr;
	for( CharacterSet::iterator itr = m_characterSet.begin();
		 itr != m_characterSet.end();
		 ++itr )
	{
		setItr = *itr;
		#if _DEBUG
		try
		{
        #endif
        setItr->doUpdate(dt);
		#if _DEBUG
		}
		catch (...)
		{
			//CRCore::notify(CRCore::ALWAYS)<<"doUpdate error "<<std::endl;
			char gbuf[256];
			sprintf(gbuf,"doUpdate error\n\0");
			gDebugInfo->debugInfo(CRCore::FATAL,gbuf);
		}
        #endif
	}
}