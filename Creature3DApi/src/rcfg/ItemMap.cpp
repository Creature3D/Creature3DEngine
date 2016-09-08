// $Id: ItemMap.cpp,v 1.1 2005/02/09 17:26:57 deepone Exp $

#ifdef _WIN32
#pragma warning(disable:4786)
#endif
#include <algorithm>
#include "rcfg/Item.h"

#include "rcfg/ItemMap.h"
#include "rcfg/utils.h"



//bool nocase_compare(std::pair<std::string, ConfigValue *> a, const std::string b) { return a < b; }

ItemMap::~ItemMap( void )
{
  for(ItemMapIterator ii=begin(); ii != end(); ii++)
    delete ii->second;
}

///
/*ItemMapIterator ItemMap::find(const std::string& key)
{
  for(ItemMapIterator ii=begin(); ii != end(); ii++)
    if (!cmp_nocase(key, ii->first))
      return ii;

  return end();
}
*/
/*std::pair<ItemMapIterator, ItemMapIterator> ItemMap::equal_range(ItemMapIterator start, ItemMapIterator stop, const std::string& key)
{
  std::pair<ItemMapIterator, ItemMapIterator> range;
  return range;
  range = std::equal_range(start, stop, key, nocase_compare);
}
*/

std::ostream& operator << (std::ostream& os, const ItemMap& item_map)
{
 
  ItemMap::const_iterator ci;

  for(ci=item_map.begin(); ci != item_map.end(); ci++) {
    os  << ci->first.c_str() << " \t ";
    os << *(ci->second) << "\n\n";
  }

  return os;  
}

std::ostream& operator << (std::ostream& os, const ItemMapIterator& ii)
{
 
  os  << ii->first.c_str() << " \t ";
  os << *(ii->second) << "\n\n";

  return os;  
}


///
/*ItemMapIterator ItemMap::find(ItemMapIterator start, ItemMapIterator stop, const std::string& key) 
{
  for(ItemMapIterator ii=start; ii != stop; ii++)
    if (!cmp_nocase(key, ii->first))
      return ii;
  return end();
}*/

///
ItemMapIterator ItemMap::find(const std::string& key, 
                              const std::string& sndKey, const std::string& sndValue)
{

  std::pair<ItemMapIterator, ItemMapIterator> g = equal_range(key);

  ItemMapIterator p = g.first;

  if (p == end())
    return p;

  if ((p->second)->Type() == ConfigValue::VALUE_STRUCT ) {
    for(; p != g.second; p++) {
      StructMap *smptr = ((StructItem *)p->second)->GetVal();
      StructMapIterator smi;

      
      smi = smptr->find(sndKey);
      if ( smi  == smptr->end() )
        return end();

      if ((smi->second)->Type() != ConfigValue::VALUE_STRING )
        return end();

      std::string s = ((StringItem *)smi->second)->GetVal();
      if (!cmp_nocase(s, sndValue))
        return p;
    }
  }

  return end();

}

bool ItemMap::getLiteral(const std::string& name, float& value)
{
  ItemMapIterator imi;
  imi= find(name);
  
  // Did we find the key?
  if (imi == end()) 
    return false;

  bool flag;

  flag = ((FloatItem *)imi->second)->GetVal(value);
	
  if (!flag) {
    int i_val;
		flag = ((IntItem *)imi->second)->GetVal(i_val);
		if (flag)
		 value = (float)i_val;
  }

  // Still nothing, hm, lets try to search for an expression instead?
  if (!flag) {
    Expression e;
    flag = ((ExpressionItem *)imi->second)->GetVal(e);
    if (flag)
      value = (float)e.GetVal();
  }

  return flag;
}

///
ItemMapIterator ItemMap::replace(ItemMapIterator p, ConfigValue *item)
{
  
  if (p == end() | !item)
    return end();

  std::string key = p->first; // Save the key

  erase(p); // Remove the previous item

  return insert(std::make_pair(key, item));

}

// Inserts/replaces a key with a value.
/// Only the first occurance is replaced if it exists previously
ItemMapIterator ItemMap::insert_item(const std::string& key, ConfigValue *item)
{
  
  if (!key.length() || !item)
    return end();

  ItemMapIterator p = find(key);  // Find the key
  
  // Does the key exists already?
  if (p != end()) {

    // Check so we are replacing with the same value type
    if ( item->Type() != (p->second)->Type() )
      throw std::string("Replacing an existing item with a different base type");

    erase(p); // Remove the previous item
  }

  return insert(std::make_pair(key, item));  // Insert it into the map

}
/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/src/rcfg/ItemMap.cpp,v $
* $Revision: 1.1 $ 
* $Date: 2005/02/09 17:26:57 $
* $Author: deepone $ 
* $Locker:  $

* $Log: ItemMap.cpp,v $
* Revision 1.1  2005/02/09 17:26:57  deepone
* vrutils and cfgscript put into new namespaces
*
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.3  2003/12/16 11:34:45  andersb
* *** empty log message ***
*
* Revision 1.2  2003/12/05 10:43:37  deepone
* *** empty log message ***
*
* Revision 1.1  2003/10/27 13:45:49  andersb
* Moved .cxx to .cpp
*
* Revision 1.11  2002/11/18 13:19:49  Anders Backman
* no message
*
* Revision 1.10  2002/10/22 12:30:57  Anders Backman
* Moved around include files.
*
* Revision 1.9  2002/07/09 21:28:57  andersb
* Fixed case bug in Expressions allowing only lowercase variables.
* Added expressions in arrays too.
* Added randInterval(min,max) function in Expressions allowing expressions like:
*
* Position [randInterval(-10,10) randInterval(-10,10) randInterval(0,3)]
*
* Revision 1.8  2002/07/04 23:08:08  andersb
* Added expressions.
* This helps alot writing scripts, where math expressions is available.
* Most standard functions: ceil, floor, sin, cos, etc is available.
* Also pi (3.1415...) and e ( 2.718 ) as constants.
* Scope is also implemented. (a variable name is backtraced upwards from the current
* scope.
*
* Revision 1.7  2002/04/09 09:04:56  andersb
* Major rewrite of ConfigIO.cxx.
* Now supports multiline strings and arrays.
*
* Revision 1.6  2002/03/13 09:04:03  andersb
* Compile with -D_THREAD_SAFE if thread safety is required. This also requires
* CommonC++ version 1.9.3 or higher.
*
* Also now Case sensitive for all items.
*
* Revision 1.5  2002/02/01 13:35:30  andersb
* Now fully updated to support recursive structs.
* Still problems with case sensitivity.
*
* Revision 1.1.1.1  2002/01/02 13:45:31  andersb
* A new version of Configscript using namespace rcfg.
* Also supporting recursive structures.
*
* Revision 1.4  2001/08/29 09:23:37  andersb
* Now using the old istd::ostream.h library to get it to work together with
* CommonC++.
*
* Revision 1.3  2001/08/01 07:11:23  andersb
* Major update
*
* Revision 1.2  2001/04/25 12:45:04  andersb
* Added makefiles for the IRIX platform.
*
* Revision 1.1.1.1  2001/04/25 10:56:28  andersb
* no message
*
* Revision 1.10  2000/11/22 15:24:42  andersb
* Working in Windows
*
* Revision 1.9  2000/11/03 14:48:04  andersb
* no message
*
* Revision 1.8  2000/10/09 13:02:46  andersb
* no message
*
* Revision 1.7  2000/10/05 13:59:58  andersb
* no message
*
* Revision 1.6  2000/10/05 10:27:08  andersb
* no message
*
* Revision 1.5  2000/10/03 09:37:25  andersb
* *** empty log message ***
*
* Revision 1.4  2000/10/03 09:21:25  andersb
* no message
*
* Revision 1.3  2000/10/03 07:44:38  andersb
* Using DOC++
*
* Revision 1.2  2000/10/03 07:20:32  andersb
* no message
*

--------------------------------------------*/
