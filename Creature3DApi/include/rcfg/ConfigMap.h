// $Id: ConfigMap.h,v 1.1 2005/02/09 17:26:54 deepone Exp $

#ifndef __ConfigMap_h__
#define __ConfigMap_h__

#ifdef _WIN32
#pragma warning(disable:4786) // Disable warnings about long names

#include "rcfg/memory_debug.h"

#endif


#include "rcfg/Item.h"


#include <stack>
#include <string>
#include "rcfg/export.h"



typedef std::pair<std::string, StructMapItem *> StructStackContent;

typedef std::stack<StructStackContent> StructItemStack_t;

class CONFIGSCRIPT_API ConfigElement {
public:
  ConfigElement(ItemMap *im) : item_map(im) {}

  ~ConfigElement();

  StructItemStack_t struct_stack;
  ItemMap *item_map;
};


ConfigElement::~ConfigElement()
{
  if (item_map) {
    delete item_map;
  }
}

typedef std::map<int, ConfigElement *> ConfigMap_t;
typedef ConfigMap_t::iterator ConfigMapIterator;


/// Class for storing pointers to ConfigScript maps
class CONFIGSCRIPT_API ConfigMap {
public:

  /// Returns the pointer to the n:th ConfigScript map
  ConfigElement *operator[](int map_id);
  
  /// Removes the n:th ConfigScript map. Also deallocates the memory
  void erase(int map_id);  

  /// Removes all ConfigScript maps
  void erase_all(void);

  /// Returns the first map id in the map of configmaps, returns 0 if map is empty
  int first();
  
  /// Associate the map with map_id
  void add(int map_id, ItemMap *map);
private:
  ConfigMap_t  map_;
};

// 
inline int ConfigMap::first() 
{
  if (map_.size()) {
    return map_.begin()->first; 
  }
  else
    return 0;
}

//
inline ConfigElement *ConfigMap::operator [](int i)
{

  ConfigMapIterator ci = map_.find(i);
  if (ci != map_.end()) {
    i = ci->first;
    return ci->second;
  }
  
  return 0;
}

//
inline void ConfigMap::erase(int map_id)
{
  ItemMap *item_map = 0;
  
  ConfigMapIterator ci = map_.find(map_id);
  if (ci != map_.end()) {
    delete ci->second;
    map_.erase(map_id);
  }
}

//
inline void ConfigMap::erase_all()
{
  ItemMap *item_map = 0;
  
  while(map_.size()) {
    ConfigMapIterator ci = map_.begin();
    delete ci->second;
    map_.erase(ci->first);
  }
}

//
inline void ConfigMap::add(int map_id, ItemMap *map)
{
 ItemMap *item_map = 0;
  
  ConfigMapIterator ci = map_.find(map_id);
  if (ci != map_.end()) 
    item_map = ci->second->item_map;

  if (item_map) {
    std::cerr << "Warning: Map id " << map_id << " already initialized, previous data overridden" << std::endl;
    delete item_map;
    map_.erase(map_id);
  }

  map_[map_id] = new ConfigElement(map);  // Insert it into the map
}
#endif

/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rcfg/ConfigMap.h,v $
* $Revision: 1.1 $ 
* $Date: 2005/02/09 17:26:54 $
* $Author: deepone $ 
* $Locker:  $

* $Log: ConfigMap.h,v $
* Revision 1.1  2005/02/09 17:26:54  deepone
* vrutils and cfgscript put into new namespaces
*
* Revision 1.1.1.1  2005/02/06 06:53:00  deepone
* initial sourceforge checkin
*
* Revision 1.5  2003/12/17 09:28:50  andersb
* Made configscript a dll library under windows
* Fixed memory leaks.
* Fixed bug in processing included files in wrong order.
*
* Revision 1.4  2003/12/16 11:34:44  andersb
* *** empty log message ***
*
* Revision 1.3  2003/12/05 10:43:37  deepone
* *** empty log message ***
*
* Revision 1.2  2003/02/27 13:27:52  andersb
* Moved over to sstream instead of strstream
*
* Revision 1.1  2002/10/22 12:30:55  Anders Backman
* Moved around include files.
*
* Revision 1.1  2002/02/01 13:35:20  andersb
* Now fully updated to support recursive structs.
* Still problems with case sensitivity.
*
* Revision 1.1.1.1  2002/01/02 13:45:30  andersb
* A new version of Configscript using namespace rcfg.
* Also supporting recursive structures.
*
--------------------------------------------*/
