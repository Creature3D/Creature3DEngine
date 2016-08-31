// $Id: ItemMap.h,v 1.1 2005/02/09 17:26:55 deepone Exp $

#ifndef __ITEMMAP_H__
#define __ITEMMAP_H__


#ifdef _WIN32
#pragma warning(disable:4786)

#include "rvrutils/os/memory_debug.h"

#endif

#include <string>
#include <map>

#include "rcfg/ConfigValue.h"
#include "rcfg/utils.h"
#include "rcfg/export.h"


//#define __MMMAP_STRING_CONFIG std::multimap<std::string, ConfigValue *>
//typedef __MMMAP_STRING_CONFIG::iterator ItemMapIterator;

typedef std::multimap<std::string, ConfigValue *> ItemMap_t;
typedef ItemMap_t::iterator ItemMapIterator;


/**
ItemMap is a class for storing items read from setupfile.
Derived from multimap, ie. it can contain multiple of occurences with the 
same key */
class  CONFIGSCRIPT_API ItemMapError {
public:
  ///
  ItemMapError(std::string m ) : message(m) {};
  ///
  std::string message;

};



/// A map for storing Items, derived from multimap
class  CONFIGSCRIPT_API ItemMap : public ItemMap_t {
public:

  ///
  virtual ~ItemMap();

  ///
  ItemMap& operator= ( ConfigValue * );

  friend std::ostream& operator <<(std::ostream& os, const ItemMap& item_map);
  friend std::ostream& operator << (std::ostream& os, const ItemMapIterator& ii);

  /*!
    Locates a literal in the item map named name. It should be a float, integer or an expression
    \return true if name is found as the right type, otherwise false
  */
  bool getLiteral(const std::string& name, float& value);
  ///
  //ConfigValue *& operator[]( const string& key );

  /// Returns the first matching item
  ItemMapIterator find(const std::string& key) {
    return ItemMap_t::find(key);
  }
  
  /// Searches in a limited interval (from start to stop) after key
  /*ItemMapIterator find(ItemMapIterator start, ItemMapIterator stop, const std::string& key);*/
  
  /** Makes a two level search, starts to search in this map after key.
      If there was a match it continous to the second level map (if the stored value
      is a struct. It searches in the 2:nd level map after sndKey.
      If there was a match it continous to check the value (if it was a string value) for a match. */
  ItemMapIterator find(const std::string& key, const std::string& sndKey, const std::string& sndValue);

  
  /*std::pair<ItemMapIterator, ItemMapIterator> equal_range(ItemMapIterator start, ItemMapIterator stop, const std::string& key);

  std::pair<ItemMapIterator, ItemMapIterator> equal_range(const std::string& key) { return equal_range(begin(), end(), key); }
*/

  /** Replaces the item pointed by p with the value.
     If the values are not of the same type a ItemMapError is thrown  */
  ItemMapIterator replace(ItemMapIterator p, ConfigValue *value);

  /** Replaces the item identified with key with the value.
     If the values are not of the same type a ItemMapError is thrown  */
  ItemMapIterator replace(const std::string& key, ConfigValue *value) { return insert_item(key, value); };
  

  /// Inserts a new item, replaces the first occurance if it already exists.
  ItemMapIterator insert_item(const std::string& key, ConfigValue *item);

 };


#endif

/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rcfg/ItemMap.h,v $
* $Revision: 1.1 $ 
* $Date: 2005/02/09 17:26:55 $
* $Author: deepone $ 
* $Locker:  $

* $Log: ItemMap.h,v $
* Revision 1.1  2005/02/09 17:26:55  deepone
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
* Revision 1.2  2003/02/27 13:27:53  andersb
* Moved over to sstream instead of strstream
*
* Revision 1.1  2002/10/22 12:30:56  Anders Backman
* Moved around include files.
*
* Revision 1.9  2002/10/14 14:22:31  Anders Backman
* Added VC6 project files again,
*
* Revision 1.8  2002/07/04 23:08:47  andersb
* Added expressions.
* This helps alot writing scripts, where math expressions is available.
* Most standard functions: ceil, floor, sin, cos, etc is available.
* Also pi (3.1415...) and e ( 2.718 ) as constants.
* Scope is also implemented. (a variable name is backtraced upwards from the current
* scope.
*
* Revision 1.7  2002/04/09 09:04:48  andersb
* Major rewrite of ConfigIO.cxx.
* Now supports multiline strings and arrays.
*
* Revision 1.6  2002/03/13 09:03:55  andersb
* Compile with -D_THREAD_SAFE if thread safety is required. This also requires
* CommonC++ version 1.9.3 or higher.
*
* Also now Case sensitive for all items.
*
* Revision 1.5  2002/02/01 13:35:21  andersb
* Now fully updated to support recursive structs.
* Still problems with case sensitivity.
*
* Revision 1.1.1.1  2002/01/02 13:45:30  andersb
* A new version of Configscript using namespace rcfg.
* Also supporting recursive structures.
*
* Revision 1.4  2001/08/29 09:23:35  andersb
* Now using the old iostream.h library to get it to work together with
* CommonC++.
*
* Revision 1.3  2001/08/01 07:11:20  andersb
* Major update
*
* Revision 1.2  2001/04/25 12:45:01  andersb
* Added makefiles for the IRIX platform.
*
* Revision 1.1.1.1  2001/04/25 10:56:27  andersb
* no message
*
* Revision 1.14  2000/11/03 14:48:04  andersb
* no message
*
* Revision 1.13  2000/10/26 15:02:18  andersb
* no message
*
* Revision 1.12  2000/10/16 08:43:40  andersb
* no message
*
* Revision 1.11  2000/10/09 13:02:47  andersb
* no message
*
* Revision 1.10  2000/10/05 13:59:59  andersb
* no message
*
* Revision 1.9  2000/10/05 10:27:09  andersb
* no message
*
* Revision 1.8  2000/10/03 09:37:26  andersb
* *** empty log message ***
*
* Revision 1.7  2000/10/03 09:21:26  andersb
* no message
*
* Revision 1.6  2000/10/03 07:44:39  andersb
* Using DOC++
*
* Revision 1.5  2000/10/03 07:36:56  andersb
* no message
*
* Revision 1.4  2000/10/03 07:33:04  andersb
* no message
*
* Revision 1.3  2000/10/03 07:25:05  andersb
* no message
*
* Revision 1.2  2000/10/03 07:20:33  andersb
* no message
*

--------------------------------------------*/
