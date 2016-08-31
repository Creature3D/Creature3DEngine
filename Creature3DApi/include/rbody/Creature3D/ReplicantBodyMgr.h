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
#ifndef __ReplicantBodyMgr_h__
#define __ReplicantBodyMgr_h__
#include <cassert>
#include <cal3d/cal3d.h>
#include "rbody/export.h"

#include "rvrutils/os/FilePathContainer.h"

#include <map>
#include "CRCore/Referenced.h"
#include "CRCore/ref_ptr.h"
#include "CRCore/crGroup.h"
#include "rbody/Creature3D/CreBody.h"
#include <CRCore/thread/crScopedLock.h>
#include <CRCore/thread/crMutex.h>

namespace rbody {

class REPLICANTBODY_API ReplicantBodyMgr : public CRCore::Referenced
{
//public:
//  typedef std::map< std::string, CRCore::ref_ptr<rbody::CreBodyNode>  > CharacterMap;
//  typedef CharacterMap::iterator CharacterIterator;
//  typedef CharacterMap::const_iterator CharacterConstIterator;

public:
   
  /// Return the singleton object
  static ReplicantBodyMgr *instance( void );

  ///// Return a pointer to a named character
  //rbody::CreBodyNode *findCharacter(const std::string& name);
  
  //// Return the iterator for the named character
  //CharacterIterator getIterator(const std::string& id) { return m_character_map.find(id); }

  // Return the iterator for the specified character
  //CharacterIterator getIterator(rbody::CreBodyNode *body);

  /// Remove a named character from the map of characters
  //bool removeCharacter(const std::string& name);

  //bool removeCharacter(rbody::CreBodyNode *body);

  /// Return the begin iterator of the map of characters
  //CharacterIterator begin() { return m_character_map.begin(); }

  /// Return the end iterator of the map of characters
  //CharacterIterator end() { return m_character_map.end(); }

  /// Return the number of characters in the map
  //size_t size() { return m_character_map.size(); }

  /// Return true if the map of characters is empty
  //bool empty() { return m_character_map.empty(); }
  /*!
    Open and read a character file. A character file contains instances of bodies.
    It can contain specifications of many characters, therefore it returns a group, containing
    possibly many children with CreBodyNode
    \returns Pointer to a group containing all characters defined in character file fileName
  */
  CRCore::crGroup *readCharacterFile(const std::string& fileName, CRCore::crNode *collisionRoot=0);

  /*!
    Creates a character from the coreName with the name id.
    \returns A ref_ptr to the created character
    \param coreName - Name of the core body
    \param id - Unique id given to the created character
    \param collisionRoot - If specified, the root used for raycast ground following
  */
  void createCharacter(const std::string& coreName, bool enableTangentSpace, const std::string &fileName,CRCore::ref_ptr<rbody::CreBodyNode> &body);

 
  //void setCollisionRootNode(CharacterIterator begin, CharacterIterator end, CRCore::crNode *collisionRoot);
  virtual void clear();

  typedef std::set< CRCore::ref_ptr<rbody::CreBodyNode> > CharacterSet;
  void insertCharacter(rbody::CreBodyNode *character);
  void removeCharacter(rbody::CreBodyNode *character);
  void updateCharacter(float dt);
  int getnid();
private:
	static CRCore::ref_ptr<ReplicantBodyMgr> m_instance;

  void parseCharacter(rcfg::ConfigScript& cfg, const std::string &fileName,CRCore::ref_ptr<rbody::CreBodyNode> &body);

  /// Deallocates any allocated memory
  //virtual void _clear();
  
  //CharacterMap m_character_map;

  /// Destructor
  virtual ~ReplicantBodyMgr();

  /// Constructor
  ReplicantBodyMgr( void );

  rvrutils::FilePathContainer m_filepathcontainer;
  int m_nid;
  CharacterSet m_characterSet;
  CRCore::crMutex m_characterSetMutex;
};

} // namespace rbody

#define g_ReplicantBodyMgr rbody::ReplicantBodyMgr::instance()

#endif

