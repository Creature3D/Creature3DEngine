// $Id: ConfigScriptOO.cpp,v 1.1 2005/02/09 17:26:57 deepone Exp $

#include "rcfg/ConfigScriptOO.h"
#include "rcfg/ConfigIO.h"
#include <vector>

#include <stdio.h>

#ifdef _THREAD_SAFE
#include <cc++/thread.h>
#endif

#include <algorithm>
#include <stdexcept>

using namespace rcfg;

ConfigScript::ConfigScript() : _item_map(0L), _allocated_item_map(false) 
{
  //_item_map = new ItemMap;
  //_allocated_item_map= true;
}

ConfigScript::ConfigScript(const std::string& filename, const std::string& header) : 
  _item_map(0L), _allocated_item_map(false) {

  if (!Open(filename, header))
    throw std::runtime_error(GetLastError().c_str());
}


ConfigScript::ConfigScript(ItemMap *item_map) : _item_map(item_map) ,_allocated_item_map(false)
{

}


void ConfigScript::ReadString(const std::string& title, const std::string& data, const std::string& header) 
{

  // If a script is already opened, close it first
//  if (Check())
//    Close();

// Take exclusive access to the class
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  //_item_map=0;

  // Start by loading the configuration file
  try {
    ConfigIO configFile;


    MacroMapIterator mmi;
    for(mmi = _macro_map.begin(); mmi != _macro_map.end(); mmi++)
      configFile.addMacro(mmi->first, mmi->second);

    configFile.ReadString(title, data, header);
  
    if (!_item_map) {
      _item_map = new ItemMap;
      _allocated_item_map= true;
    }

    // Parses the file and inserts all items into map
    configFile.GetMap( *_item_map ); 
  }
  catch ( std::exception& e ) {
    _last_error = e.what();
    Close();
    throw std::runtime_error(GetLastError().c_str());
  }
}



bool ConfigScript::Open(const std::string& filename, const std::string& header) {
 
  // If a script is already opened, close it first
  if (Check())
    Close();

  // Take exclusive access to the class
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  _item_map=0;

  // Start by loading the configuration file
  try {
    ConfigIO configFile;

    MacroMapIterator mmi;
    for(mmi = _macro_map.begin(); mmi != _macro_map.end(); mmi++)
      configFile.addMacro(mmi->first, mmi->second);

    configFile.OpenRead(filename, header);


    _item_map = new ItemMap;
    _allocated_item_map= true;

    // Parses the file and inserts all items into map
    configFile.GetMap( *_item_map ); 
  }
  catch ( std::exception& e ) {
    Close();
    _last_error = e.what();
    return false;
  }
  return true; 
}


// Local method that takes the given string extract all keys from it
// and place them in an vector of keys
// Example: key1.key2.key3 would result in:
// ["key1", "key2", "key3"] and returns the length 3
std::size_t ConfigScript::CreateKeyVector(std::string str, std::vector<std::string>& vec)
{
  std::size_t pos=0;
  std::string key;
  
  while((pos=str.find(".")) != str.npos) {
    key = str.substr(0, pos);
    str = str.substr(pos+1, str.length());
    vec.push_back(key);
  }
  
  if (str.length())
    vec.push_back(str);
  
  return vec.size();
}



//  
ConfigScript::Status ConfigScript::StructAccess(const std::string& str, std::string& key, std::string& struct_key,
                    ConfigValue* &result, unsigned int nth) 
{
  
  result = 0L;
 
  unsigned int i;
  struct_key = "";
  
  if (!str.length())
    return INVALID_KEY;

  std::string line = str;

  // Take the given string extract all keys from it
  // and place them in an vector of keys
  // Example: key1.key2.key3 would result in:
  // ["key1", "key2", "key3"] and returns the length 3
  std::vector<std::string> keys;
  CreateKeyVector(str, keys);
    
  for(i=0; i < keys.size(); i++) {
    try {
      ValidKey(keys[i]);
    }
    catch (std::string msg) {

      _last_error = msg;
      return INVALID_KEY;
    }
  }

  // if for some strange reason the str was not empty, (should have been catched above)
  // and still the size of the key vector is 0 (no keys)
  if (keys.size() == 0)
    return INVALID_KEY;


  StructMap *smptr=0;
    
  ItemMapIterator ii;
  
  // No structs pushed onto the stack
  if (!_struct_stack.size()) {
    std::pair<ItemMapIterator, ItemMapIterator> range;
    range = _item_map->equal_range(keys[0]);
    
    if (range.first == range.second) {
        std::ostringstream msg_str; msg_str  << "Key " << keys[0] << " of index " << nth << " is missing";
        _last_error = msg_str.str();
      return MISSING_KEY;
    }
    
    // If we are only accessing one key deep. 
    // Then search for the nth  item (if there are more than one matching).
    if (keys.size() == 1) {
      ii=range.first;
      for(i=1; i < nth && ii != range.second;i++, ii++)
        ;
      if (i != nth|| ii == range.second || keys[0] != ii->first  ) { // the n:th item is a nonexisting one
        std::ostringstream msg_str; msg_str  << "Key " << keys[0] << " of index " << nth << " is missing";
        _last_error = msg_str.str();
        return MISSING_KEY;
      }
    }
    else { // keys.size() != 1 
           // We are accessing an item deeper down in a struct (keys.size() > 1)
           // So first try to get us into the first level
      
      // Find a matching struct
      ii= _item_map->find(keys[0]);
  
      // Did we find the key?
      if (ii == _item_map->end()) {
        std::ostringstream msg_str; msg_str  << "Key " << keys[0] << " of index " << nth << " is missing";
        _last_error = msg_str.str();
        return MISSING_KEY;
      }
    }
    
    // Store the result.
    result = ii->second;


    // Are we searching only for one level? Then set the values and return
    if (keys.size() == 1) {
      struct_key = keys[0]; // The key searched for
      return OK;
    }

  } else {// if !_config_maps[_current_map]->struct_stack.size()) {
    
    // There is a struct in the stack. Start from that instead
    result = _struct_stack.top().second;
    if (!result) {
      _last_error = "Internal error, the top of the struct stack contains a NULL struct";
      return INTERNAL_ERROR;
    }    
  }

  // struct_map now contains the struct we should start searching from.
  // Lets descend into the right level of structs
  // Return that struct
  std::string active_key;
  
  // The last is the actual key for accessing values
  // the ones before is just to get into the right structure
  key = keys[keys.size()-1];

  int start_key=0;
  if (_struct_stack.size()) {
    active_key = _struct_stack.top().first;
    start_key = 0;
  }
  else {
    active_key = keys[0];
    active_key += ".";
    start_key = 1;
  }

  bool ok=false; 
  for(i=start_key; i < keys.size(); i++) {
    
    
    active_key += std::string(".") + keys[i];
  
    if (result->Type() == ConfigValue::VALUE_STRUCT) 
      smptr = ((StructItem *)result)->GetVal();
    

    // If we are accessing a multi-existing item and this is the last key, then get the nth item
    if (nth > 1 && i == keys.size()-1) {
      StructMapIterator smi;
      std::pair<StructMapIterator, StructMapIterator> range;

      range = smptr->equal_range(keys[i]);
      if (range.first == range.second) {
        _last_error = "Key ";
        _last_error += keys[i];
        _last_error += " is missing.";
        return MISSING_KEY;
      }

      unsigned int x;
      //      std::string asd = keys[i];
//      std::cerr << "Searching in: " << smptr << std::endl;
//      smi=range.first;
//      for(int x=1; smi != range.second; x++, smi++)
//        std::cerr << x << " " <<  smi->first << " : " << *(smi->second) << std::endl;

      smi=range.first;
      for( x=1; x < nth && smi != range.second; x++, smi++)
        ;

      //std::cerr << "Curent:" << x << " " <<  smi->first << " : " << *(smi->second) << std::endl;
      
      if (smi == range.second || smi->first != keys[i] || x != nth) { // the n:th item is a nonexisting one
        char str[100];
        std::string tmp;
        if (nth ==2)
          tmp = "nd";
        else if(nth ==3)
          tmp= "rd";
        else
          tmp = "th";

        sprintf(str, "There is no %d:%s key of name %s\0", nth, tmp.c_str(), keys[i].c_str());
        _last_error = str;
        result=0;
        return MISSING_KEY;
      }
      result = smi->second;
      ok = true;
    }
    else
      if (smptr)
        ok = smptr->GetVal(keys[i], result);
      else 
        ok = false;

    if (!ok) {
      _last_error = "Key ";
      _last_error += active_key;
      _last_error += " does not exist.";
      return MISSING_KEY;
    } 
  }

  struct_key = key;
  
  return OK;

}

//  
//ConfigScript::Status ConfigScript::StructAccess_I(const std::string& str, std::string& key, std::string& struct_key,
//												ItemMapIterator& result, unsigned int nth) 
//{
//
//	result = 0L;
//
//	unsigned int i;
//	struct_key = "";
//
//	if (!str.length())
//		return INVALID_KEY;
//
//	std::string line = str;
//
//	// Take the given string extract all keys from it
//	// and place them in an vector of keys
//	// Example: key1.key2.key3 would result in:
//	// ["key1", "key2", "key3"] and returns the length 3
//	std::vector<std::string> keys;
//	CreateKeyVector(str, keys);
//
//	for(i=0; i < keys.size(); i++) {
//		try {
//			ValidKey(keys[i]);
//		}
//		catch (std::string msg) {
//
//			_last_error = msg;
//			return INVALID_KEY;
//		}
//	}
//
//	// if for some strange reason the str was not empty, (should have been catched above)
//	// and still the size of the key vector is 0 (no keys)
//	if (keys.size() == 0)
//		return INVALID_KEY;
//
//
//	StructMap *smptr=0;
//
//	ItemMapIterator ii;
//
//	// No structs pushed onto the stack
//	if (!_struct_stack.size()) {
//		std::pair<ItemMapIterator, ItemMapIterator> range;
//		range = _item_map->equal_range(keys[0]);
//
//		if (range.first == range.second) {
//			std::ostringstream msg_str; msg_str  << "Key " << keys[0] << " of index " << nth << " is missing";
//			_last_error = msg_str.str();
//			return MISSING_KEY;
//		}
//
//		// If we are only accessing one key deep. 
//		// Then search for the nth  item (if there are more than one matching).
//		if (keys.size() == 1) {
//			ii=range.first;
//			for(i=1; i < nth && ii != range.second;i++, ii++)
//				;
//			if (i != nth|| ii == range.second || keys[0] != ii->first  ) { // the n:th item is a nonexisting one
//				std::ostringstream msg_str; msg_str  << "Key " << keys[0] << " of index " << nth << " is missing";
//				_last_error = msg_str.str();
//				return MISSING_KEY;
//			}
//		}
//		else { // keys.size() != 1 
//			// We are accessing an item deeper down in a struct (keys.size() > 1)
//			// So first try to get us into the first level
//
//			// Find a matching struct
//			ii= _item_map->find(keys[0]);
//
//			// Did we find the key?
//			if (ii == _item_map->end()) {
//				std::ostringstream msg_str; msg_str  << "Key " << keys[0] << " of index " << nth << " is missing";
//				_last_error = msg_str.str();
//				return MISSING_KEY;
//			}
//		}
//
//		// Store the result.
//		result = ii;
//
//
//		// Are we searching only for one level? Then set the values and return
//		if (keys.size() == 1) {
//			struct_key = keys[0]; // The key searched for
//			return OK;
//		}
//
//	} //else {// if !_config_maps[_current_map]->struct_stack.size()) {
//
//	//	// There is a struct in the stack. Start from that instead
//	//	result = _struct_stack.top();
//	//	if (!result) {
//	//		_last_error = "Internal error, the top of the struct stack contains a NULL struct";
//	//		return INTERNAL_ERROR;
//	//	}    
//	//}
//
//	// struct_map now contains the struct we should start searching from.
//	// Lets descend into the right level of structs
//	// Return that struct
//	std::string active_key;
//
//	// The last is the actual key for accessing values
//	// the ones before is just to get into the right structure
//	key = keys[keys.size()-1];
//
//	int start_key=0;
//	if (_struct_stack.size()) {
//		active_key = _struct_stack.top().first;
//		start_key = 0;
//	}
//	else {
//		active_key = keys[0];
//		active_key += ".";
//		start_key = 1;
//	}
//
//	bool ok=false; 
//	for(i=start_key; i < keys.size(); i++) {
//
//
//		active_key += std::string(".") + keys[i];
//
//		if (result->second->Type() == ConfigValue::VALUE_STRUCT) 
//			smptr = ((StructItem *)(result->second))->GetVal();
//
//
//		// If we are accessing a multi-existing item and this is the last key, then get the nth item
//		if (nth > 1 && i == keys.size()-1) {
//			StructMapIterator smi;
//			std::pair<StructMapIterator, StructMapIterator> range;
//
//			range = smptr->equal_range(keys[i]);
//			if (range.first == range.second) {
//				_last_error = "Key ";
//				_last_error += keys[i];
//				_last_error += " is missing.";
//				return MISSING_KEY;
//			}
//
//			unsigned int x;
//			//      std::string asd = keys[i];
//			//      std::cerr << "Searching in: " << smptr << std::endl;
//			//      smi=range.first;
//			//      for(int x=1; smi != range.second; x++, smi++)
//			//        std::cerr << x << " " <<  smi->first << " : " << *(smi->second) << std::endl;
//
//			smi=range.first;
//			for( x=1; x < nth && smi != range.second; x++, smi++)
//				;
//
//			//std::cerr << "Curent:" << x << " " <<  smi->first << " : " << *(smi->second) << std::endl;
//
//			if (smi == range.second || smi->first != keys[i] || x != nth) { // the n:th item is a nonexisting one
//				char str[100];
//				std::string tmp;
//				if (nth ==2)
//					tmp = "nd";
//				else if(nth ==3)
//					tmp= "rd";
//				else
//					tmp = "th";
//
//				sprintf(str, "There is no %d:%s key of name %s\0", nth, tmp.c_str(), keys[i].c_str());
//				_last_error = str;
//				result=0;
//				return MISSING_KEY;
//			}
//			result = smi;
//			ok = true;
//		}
//		else
//			if (smptr)
//				ok = smptr->GetVal(keys[i], result->second);
//			else 
//				ok = false;
//
//		if (!ok) {
//			_last_error = "Key ";
//			_last_error += active_key;
//			_last_error += " does not exist.";
//			return MISSING_KEY;
//		} 
//	}
//
//	struct_key = key;
//
//	return OK;
//
//}

// Returns the last occurred error message
std::string ConfigScript::GetLastError( void ) 
{ 
  // Take exclusive access to the class
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  std::string s; 
  s = _last_error; 

  return s; 
}


bool ConfigScript::IsOpen(  ) 
{
  // Take exclusive access to the class
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  bool s;
  
  s = _item_map != 0; 
  return s;
}

// Deletes the map associated with _current_map
bool ConfigScript::Close( )
{

  // Take exclusive access to the class
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  bool status=true;
  
  if (Check()) {
    if (_allocated_item_map)
      delete _item_map;
    _item_map=0;
    while(_struct_stack.size())
      _struct_stack.pop();
  }

  return status;
}


bool ConfigScript::Push(const std::string& key, int index)
{
  // Take exclusive access to the class
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif


  if (!Check())
    return false;

  std::string s_key, parent_key;

  ConfigValue *item=0L;
  if (StructAccess(key, s_key, parent_key, item, index))
    return false;
  
  if (!item || item->Type() != ConfigValue::VALUE_STRUCT) {
    char str[100];

    sprintf(str, "Error: Key %s in map is not a struct and can not be opened\0", key.c_str());
    _last_error = str;
    return false;
  }

  // Compound a key indicating where we currently are accessing data
  std::string str;
  if (_struct_stack.size()) 
    str = _struct_stack.top().first + std::string(".") + key;
  else
    str = key;

  StructStackContent ssc;
  
  ssc.first = str;
  ssc.second = (StructMapItem*)item;
  _struct_stack.push(ssc);

  return true;
}

bool ConfigScript::Pop( StructSelection selection )
{
  // Take exclusive access to the class
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  if (!Check())
    return false;

  // Are there any items on the stack?
  if (_struct_stack.size()) {
    if (selection == ALL)  // Should we pop all entries from the stack?
      while(_struct_stack.size())
        _struct_stack.pop();
    else 
      _struct_stack.pop(); // Or only one?

  }
  else {
    char str[100];
    sprintf(str, "Error: struct stack is empty\0");
    _last_error = str;
    return false;
  }

  return true;
}


bool ConfigScript::Exist( std::string key, Type type, int index) 
{

  // Take exclusive access to the class
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  if (!Check())
    return false;
  
  std::string s_key, parent_key;


  ConfigValue *item;
  if (StructAccess(key, s_key, parent_key, item, index)) {
    return false;
  }

  if (item->Type() != ToInternalType(type)) {
    _last_error = "Key ";
    _last_error += s_key + std::string(" exists but is of wrong type (") + item->Description();
    _last_error += ")";
    return false;
  }

  return true;
}


bool ConfigScript::Get(const std::string& key, int& data, int index) 
{

  // Take exclusive access to the class
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  if (!Check())
    return false;
  
  bool result;

  std::string s_key, parent_key;


  ConfigValue *item;
  if (StructAccess(key, s_key, parent_key, item, index)) {
    return false;
  }

  // Check the type of the item
  ConfigValue::ValueType type = item->Type();

  // Get the data if its a float
  if (type == ConfigValue::VALUE_INT) {
    result= ((IntItem *)item)->GetVal(data);
    if (!result) {
      _last_error = "Internal error, key: ";
      _last_error += s_key + std::string(" is a ") + item->Description();
      _last_error += " should be an integer";
      return false;
    }
  }
  else if (type == ConfigValue::VALUE_FLOAT) {

    // Get the data if it is an integer and store it as a float
    float f_data;
    result= ((FloatItem *)item)->GetVal(f_data);
    if (!result) {
      _last_error = "Internal error, key: ";
      _last_error += s_key + std::string(" is a ") + item->Description();
      _last_error += " should be a float";
      return false;
    }
    data = (int)f_data;
  }
  else if (type == ConfigValue::VALUE_EXPRESSION) {
    // Get the data if it is an expression and store it as a float
    Expression exp;
    result= ((ExpressionItem *)item)->GetVal(exp);
    if (!result) {
      _last_error = "Internal error, key: ";
      _last_error += s_key + std::string(" is a ") + item->Description();
      _last_error += " should be an expression";
      return false;
    }
    data = (int)exp.GetVal();
  }
    // it is neither a float, int or an expression
  else {
    _last_error = "Key ";
    _last_error += s_key + std::string(" is a ") + item->Description();
    _last_error += " no valid conversion to integer could be made";
     return false;
  }

  return true;
}





//
bool ConfigScript::Get(const std::string& key, float& data, int index) 
{
  // Take exclusive access to the class
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif


  if (!Check())
    return false;
  
  bool result;

  std::string s_key, parent_key;

   ConfigValue *item;

  if (StructAccess(key, s_key, parent_key, item, index))
    return false;

  // Check the type of the item
  ConfigValue::ValueType type = item->Type();

  // Get the data if its a float
  if (type == ConfigValue::VALUE_FLOAT) {
    result= ((FloatItem *)item)->GetVal(data);
    if (!result) {
      _last_error = "Internal error, key: ";
      _last_error += s_key + " should be a float but is not";
      return false;
    }
  }
  else if (type == ConfigValue::VALUE_INT) {

    // Get the data if it is an integer and store it as a float
    int i_data;
    result= ((IntItem *)item)->GetVal(i_data);
    if (!result) {
      _last_error = "Internal error, key: ";
      _last_error += s_key + " should be a float but is not";
      return false;
    }
    data = (float)i_data;
  }
  else if (type == ConfigValue::VALUE_EXPRESSION) {
    // Get the data if it is an expression and store it as a float
    Expression exp;
    result= ((ExpressionItem *)item)->GetVal(exp);
    if (!result) {
      _last_error = "Internal error, key: ";
      _last_error += s_key + " should be an expression but is not";
      return false;
    }
    data = (float)exp.GetVal();
  }
    // it is neither a float, int or an expression
  else {
    _last_error = "Key ";
    _last_error += s_key + std::string(" is a ") + item->Description();
    _last_error += " no valid conversion to float could be made";
     return false;
  }


  return true;
}

//
bool ConfigScript::Get(const std::string& key, std::string& data, int index) 
{

  // Take exclusive access to the class
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  if (!Check())
    return false;
  
  bool result;

  std::string s_key, parent_key;

  ConfigValue *item;

  if (StructAccess(key, s_key, parent_key, item, index))
    return false;

  if (item->Type() != ConfigValue::VALUE_STRING) {
    _last_error = "Key ";
    _last_error += s_key + std::string(" is a ") + item->Description();
    _last_error += " should be a string";
    return false;
  }

  result= ((StringItem *)item)->GetVal(data);
  if (!result) {
    _last_error = "Internal error, key: ";
    _last_error += s_key + " should be a string but is not";
    return false;
  }

  return true;
}


//
bool ConfigScript::Get(const std::string& key, std::vector<float> &data, int index) 
{
  data.resize(0);  
  // Take exclusive access to the class
  #ifdef _THREAD_SAFE
    ost::MutexLock mlock(_io_mutex);
  #endif

  if (!Check())
    return false;
  
  bool result;

  std::string s_key, parent_key;

   ConfigValue *item;

  if (StructAccess(key, s_key, parent_key, item, index))
    return false;

  if (item->Type() != ConfigValue::VALUE_FLOAT_ARRAY) {
    _last_error = "Key ";
    _last_error += s_key + std::string(" is a ") + item->Description();
    _last_error += " should be a float array";
    return false;
  }

  result= ((FloatVectorItem *)item)->GetVal(data);
  if (!result) {
    _last_error = "Internal error, key: ";
    _last_error += s_key + " should be an array but is not";
    return false;
  }

  return true;
}

//
bool ConfigScript::Get(const std::string& key, std::vector<std::string> &data, int index) 
{
  data.resize(0);  
  // Take exclusive access to the class
  #ifdef _THREAD_SAFE
    ost::MutexLock mlock(_io_mutex);
  #endif

  if (!Check())
    return false;
  
  bool result;

  std::string s_key, parent_key;

   ConfigValue *item;

  if (StructAccess(key, s_key, parent_key, item, index))
    return false;

  if (item->Type() != ConfigValue::VALUE_STRING_ARRAY) {
    _last_error = "Key ";
    _last_error += s_key + std::string(" is a ") + item->Description();
    _last_error += " should be a float array";
    return false;
  }

  result= ((FloatVectorItem *)item)->GetVal(data);
  if (!result) {
    _last_error = "Internal error, key: ";
    _last_error += s_key + " should be an array but is not";
    return false;
  }

  return true;
}

//
bool ConfigScript::Get(const std::string& key, Expression &data, int index) 
{
  // Take exclusive access to the class
  #ifdef _THREAD_SAFE
    ost::MutexLock mlock(_io_mutex);
  #endif

  if (!Check())
    return false;
  
  bool result;

  std::string s_key, parent_key;

   ConfigValue *item;

  if (StructAccess(key, s_key, parent_key, item, index))
    return false;

  if (item->Type() != ConfigValue::VALUE_EXPRESSION) {
    _last_error = "Key ";
    _last_error += s_key + std::string(" is a ") + item->Description();
    _last_error += " should be an expression";
    return false;
  }

  result= ((ExpressionItem *)item)->GetVal(data);
  if (!result) {
    _last_error = "Internal error, key: ";
    _last_error += s_key + " should be an expression but is not";
    return false;
  }

  return true;
}

std::string ConfigScript::Return(const std::string& key, const std::string& def, int index) 
{
  std::string val;

  if (!Get(key, val, index))
    val = def;

  return val;
}
 
int ConfigScript::Return(const std::string& key, int def, int index) 
{
  int val;

  bool b = (bool)Get(key, val, index);
  if (!b)
    val = def;

  return val;
}

float ConfigScript::Return(const std::string& key, float def, int index) 
{
  float val;

  if (!Get(key, val, index))
    val = def;

  return val;
}

std::vector<float> ConfigScript::Return(const std::string& key, const std::vector<float> &def, int index) 
{
  std::vector<float> val;

  if (!Get(key, val, index))
    val = def;

  return val;
}

std::vector<std::string> ConfigScript::Return(const std::string& key, const std::vector<std::string> &def, int index) 
{
  std::vector<std::string> val;

  if (!Get(key, val, index))
    val = def;

  return val;
}


Expression ConfigScript::Return(const std::string& key, const Expression& def, int index) 
{
  Expression val;

  if (!Get(key, val, index))
    val = def;

  return val;
}

ConfigValue::ValueType ConfigScript::ToInternalType(Type type) const
{
  ConfigValue::ValueType int_type;

  switch (type) {
    case(INT):
      int_type = ConfigValue::VALUE_INT;    
      break;
  case(FLOAT):
      int_type = ConfigValue::VALUE_FLOAT;    
      break;
  case(FLOAT_ARRAY):
      int_type = ConfigValue::VALUE_FLOAT_ARRAY;    
      break;

  case(STRING_ARRAY):
      int_type = ConfigValue::VALUE_STRING_ARRAY;    
      break;

  case(STRUCT):
      int_type = ConfigValue::VALUE_STRUCT;    
      break;
  case(STRING):
      int_type = ConfigValue::VALUE_STRING;    
      break;
  default:
    int_type = ConfigValue::VALUE_NONE;
    break;
  }
  return int_type;
}

ConfigScript::Type ConfigScript::FromInternalType(ConfigValue::ValueType type) const 
{
  ConfigScript::Type int_type;

  switch (type) {
    case(ConfigValue::VALUE_INT):
      int_type = INT;    
      break;
  case(ConfigValue::VALUE_FLOAT):
      int_type = FLOAT;    
      break;
  case(ConfigValue::VALUE_FLOAT_ARRAY):
      int_type = FLOAT_ARRAY;    
      break;

  case(ConfigValue::VALUE_STRING_ARRAY):
      int_type = STRING_ARRAY;    
      break;

  case(ConfigValue::VALUE_STRUCT):
      int_type = STRUCT;    
      break;
  case(ConfigValue::VALUE_STRING):
      int_type = STRING;    
      break;
  default:
    int_type = NO_TYPE;
    break;
  }
  return int_type;
}




void ConfigScript::AddMacro(const std::string& macro, const std::string& value)
{
// Take exclusive access to the class
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

 // if (IsOpen()) {
 //   std::cerr << "Warning: addMacro is called after Open is called. It will have no affect what so ever" << std::endl;
 // }
 // else
    _macro_map.insert(MacroMapType(macro, value));
}


std::string ConfigScript::CurrentScope()
{
// Take exclusive access to the class
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  if (!_struct_stack.size())
    return std::string("");

  return _struct_stack.top().first + ".";
}

int ConfigScript::Add(const std::string& id, const std::string& value)
{
// Take exclusive access to the class
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif


  int idx=0;

  idx = addItem<StringItem>(id, new StringItem(value));
  return idx;
}

int ConfigScript::Add(const std::string& id, const float& value)
{
// Take exclusive access to the class
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  int idx=0;

  idx = addItem<FloatItem>(id, new FloatItem(value));
  return idx;
}

int ConfigScript::Add(const std::string& id, const int& value)
{
// Take exclusive access to the class
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  int idx=0;

  idx = addItem<IntItem>(id, new IntItem(value));
  return idx;
}


int ConfigScript::Add(const std::string& id, const std::vector<float>& value)
{
// Take exclusive access to the class
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  int idx=0;

  idx = addItem<FloatVectorItem>(id, new FloatVectorItem(value));
  return idx;
}

int ConfigScript::Add(const std::string& key, const std::vector<std::string> &value)
{
// Take exclusive access to the class
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  int idx=0;
  Array vec;
  for(std::vector<std::string>::const_iterator it=value.begin(); it != value.end(); it++)
    vec.push_back(new StringItem(*it));

  idx = addItem<FloatVectorItem>(key, new FloatVectorItem(vec, ConfigValue::VALUE_STRING_ARRAY));
  return idx;
}

int ConfigScript::Add(const std::string& key, const Expression &value) 
{
// Take exclusive access to the class
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  int idx=0;

  idx = addItem<ExpressionItem>(key, new ExpressionItem(value));
  return idx;
}


// Add a named struct
int ConfigScript::Add(const std::string& id)
{
// Take exclusive access to the class
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif


  int idx=0;
  
  // Create a new empty struct
  StructMap *value = new StructMap;

  // Are we somewhere deep into the tree of the data?
  if (_struct_stack.size()) {
    // Use the current scope
    StructMap *smptr=0; 
    smptr = ((StructItem *)_struct_stack.top().second)->GetVal();

    // Set the parent of the newly created struct the current scope
    value->SetParent(smptr);

    idx = getNumberofItems<StructMap>(id, smptr);
    smptr->insert(std::make_pair(id, new StructMapItem(value)));

  } // No we are at the root
  else {
    idx = getNumberofItems<ItemMap>(id, _item_map);
    _item_map->insert(std::make_pair(id, new StructMapItem(value)));
  }


  return idx+1;
}



bool ConfigScript::setSearchPathEnvironmentVariable(const std::string& env_variable)
{ 
  return ConfigIO::setSearchPathEnvironmentVariable(env_variable); 
}

void ConfigScript::setSearchPathString(const std::string& path_string)
{ 
  ConfigIO::setSearchPathString(path_string); 
}


ConfigScript::Type ConfigScript::GetType(const std::string& key, int index)
{

 // Take exclusive access to the class
  #ifdef _THREAD_SAFE
    ost::MutexLock mlock(_io_mutex);
  #endif

  if (!Check())
    return NO_TYPE;

  std::string s_key, parent_key;

  ConfigValue *item;

  if (StructAccess(key, s_key, parent_key, item, index) != OK)
    return NO_TYPE;

  return FromInternalType(item->Type());
  
}

bool ConfigScript::Write( const std::string& fileName, const std::string& header )
{

  try {
    ConfigIO rcfg;
    rcfg.OpenWrite(fileName, header);

    rcfg.PutMap(*_item_map);
  }
  catch ( std::exception& e ) {
    _last_error = e.what();
    return false;
  }

  return true;
}

/////////ÐÞ¸Ä
bool ConfigScript::Modify(const std::string& key, const int& data, int index) 
{

	// Take exclusive access to the class
#ifdef _THREAD_SAFE
	ost::MutexLock mlock(_io_mutex);
#endif

	if (!Check())
		return false;

	std::string s_key, parent_key;


	ConfigValue *item;
	if (StructAccess(key, s_key, parent_key, item, index)) {
		return false;
	}

	// Check the type of the item
	ConfigValue::ValueType type = item->Type();

	// Get the data if its a float
	if (type == ConfigValue::VALUE_INT) {
		((IntItem *)item)->GetRef() = data;
	}
	else if (type == ConfigValue::VALUE_FLOAT) {
		((FloatItem *)item)->GetRef() = data;
		//Replace(key,data,index);
	}
	//else if (type == ConfigValue::VALUE_EXPRESSION) {
	//	((ExpressionItem *)item)->GetRef() = data;
	//}
	// it is neither a float, int or an expression
	else {
		_last_error = "Key ";
		_last_error += s_key + std::string(" is a ") + item->Description();
		_last_error += " no valid conversion to integer could be made";
		return false;
	}

	return true;
}

//
bool ConfigScript::Modify(const std::string& key, const float& data, int index) 
{
	// Take exclusive access to the class
#ifdef _THREAD_SAFE
	ost::MutexLock mlock(_io_mutex);
#endif


	if (!Check())
		return false;

	std::string s_key, parent_key;

	ConfigValue *item;

	if (StructAccess(key, s_key, parent_key, item, index))
		return false;

	// Check the type of the item
	ConfigValue::ValueType type = item->Type();

	// Get the data if its a float
	if (type == ConfigValue::VALUE_FLOAT) {
		((FloatItem *)item)->GetRef() = data;
	}
	else if (type == ConfigValue::VALUE_INT) {
		((IntItem *)item)->GetRef() = data;
		//Replace(key,data,index);
	}
	//else if (type == ConfigValue::VALUE_EXPRESSION) {
	//	((ExpressionItem *)item)->GetRef() = data;
	//}
	// it is neither a float, int or an expression
	else {
		_last_error = "Key ";
		_last_error += s_key + std::string(" is a ") + item->Description();
		_last_error += " no valid conversion to float could be made";
		return false;
	}


	return true;
}

//
bool ConfigScript::Modify(const std::string& key, const std::string& data, int index) 
{

	// Take exclusive access to the class
#ifdef _THREAD_SAFE
	ost::MutexLock mlock(_io_mutex);
#endif

	if (!Check())
		return false;

	std::string s_key, parent_key;

	ConfigValue *item;

	if (StructAccess(key, s_key, parent_key, item, index))
		return false;

	if (item->Type() != ConfigValue::VALUE_STRING) {
		_last_error = "Key ";
		_last_error += s_key + std::string(" is a ") + item->Description();
		_last_error += " should be a string";
		return false;
	}

	((StringItem *)item)->GetRef() = data;

	return true;
}


//
bool ConfigScript::Modify(const std::string& key, const std::vector<float> &data, int index) 
{
	// Take exclusive access to the class
#ifdef _THREAD_SAFE
	ost::MutexLock mlock(_io_mutex);
#endif

	if (!Check())
		return false;

	std::string s_key, parent_key;

	ConfigValue *item;

	if (StructAccess(key, s_key, parent_key, item, index))
		return false;

	if (item->Type() != ConfigValue::VALUE_FLOAT_ARRAY) {
		_last_error = "Key ";
		_last_error += s_key + std::string(" is a ") + item->Description();
		_last_error += " should be a float array";
		return false;
	}

	((FloatVectorItem *)item)->GetRef() = data;

	return true;
}

//
bool ConfigScript::Modify(const std::string& key, const std::vector<std::string> &data, int index) 
{
	// Take exclusive access to the class
#ifdef _THREAD_SAFE
	ost::MutexLock mlock(_io_mutex);
#endif

	if (!Check())
		return false;

	std::string s_key, parent_key;

	ConfigValue *item;

	if (StructAccess(key, s_key, parent_key, item, index))
		return false;

	if (item->Type() != ConfigValue::VALUE_STRING_ARRAY) {
		_last_error = "Key ";
		_last_error += s_key + std::string(" is a ") + item->Description();
		_last_error += " should be a float array";
		return false;
	}

	((FloatVectorItem *)item)->GetRef() = data;

	return true;
}

//
bool ConfigScript::Modify(const std::string& key, const Expression &data, int index) 
{
	// Take exclusive access to the class
#ifdef _THREAD_SAFE
	ost::MutexLock mlock(_io_mutex);
#endif

	if (!Check())
		return false;

	std::string s_key, parent_key;

	ConfigValue *item;

	if (StructAccess(key, s_key, parent_key, item, index))
		return false;

	if (item->Type() != ConfigValue::VALUE_EXPRESSION) {
		_last_error = "Key ";
		_last_error += s_key + std::string(" is a ") + item->Description();
		_last_error += " should be an expression";
		return false;
	}

	((ExpressionItem *)item)->GetRef() = data;

	return true;
}
ItemMap* ConfigScript::GetItemMap()
{
	_allocated_item_map = false;
	return _item_map;
}
//
//bool ConfigScript::Delete(const std::string& key,int index) 
//{
//	// Take exclusive access to the class
//#ifdef _THREAD_SAFE
//	ost::MutexLock mlock(_io_mutex);
//#endif
//
//	if (!Check())
//		return false;
//
//	std::string s_key, parent_key;
//
//	ItemMapIterator p;
//
//	if (StructAccess_I(key, s_key, parent_key, p, index))
//		return false;
//
//	_item_map->erase(p);
//
//	return true;
//}
//
//bool ConfigScript::Replace(const std::string& key,const int& data,int index)
//{
//	// Take exclusive access to the class
//#ifdef _THREAD_SAFE
//	ost::MutexLock mlock(_io_mutex);
//#endif
//
//	if (!Check())
//		return false;
//
//	std::string s_key, parent_key;
//
//	ItemMapIterator p;
//
//	if (StructAccess_I(key, s_key, parent_key, p, index))
//		return false;
//
//	_item_map->replace(p,new IntItem(data));
//
//	return true;
//}
//
//bool ConfigScript::Replace(const std::string& key,const float& data,int index)
//{
//	// Take exclusive access to the class
//#ifdef _THREAD_SAFE
//	ost::MutexLock mlock(_io_mutex);
//#endif
//
//	if (!Check())
//		return false;
//
//	std::string s_key, parent_key;
//
//	ItemMapIterator p;
//
//	if (StructAccess_I(key, s_key, parent_key, p, index))
//		return false;
//
//	_item_map->replace(p,new FloatItem(data));
//
//	return true;
//}
//
//bool ConfigScript::Replace(const std::string& key,const std::string& data,int index)
//{
//	// Take exclusive access to the class
//#ifdef _THREAD_SAFE
//	ost::MutexLock mlock(_io_mutex);
//#endif
//
//	if (!Check())
//		return false;
//
//	std::string s_key, parent_key;
//
//	ItemMapIterator p;
//
//	if (StructAccess_I(key, s_key, parent_key, p, index))
//		return false;
//
//	_item_map->replace(p,new StringItem(data));
//	return true;
//}
//
//bool ConfigScript::Replace(const std::string& key,const std::vector<float>& data,int index)
//{
//	// Take exclusive access to the class
//#ifdef _THREAD_SAFE
//	ost::MutexLock mlock(_io_mutex);
//#endif
//
//	if (!Check())
//		return false;
//
//	std::string s_key, parent_key;
//
//	ItemMapIterator p;
//
//	if (StructAccess_I(key, s_key, parent_key, p, index))
//		return false;
//
//	_item_map->replace(p,new FloatVectorItem(data));
//	return true;
//}
//
//bool ConfigScript::Replace(const std::string& key,const std::vector<std::string>& data,int index)
//{
//	// Take exclusive access to the class
//#ifdef _THREAD_SAFE
//	ost::MutexLock mlock(_io_mutex);
//#endif
//
//	if (!Check())
//		return false;
//
//	std::string s_key, parent_key;
//
//	ItemMapIterator p;
//
//	if (StructAccess_I(key, s_key, parent_key, p, index))
//		return false;
//
//	int idx=0;
//	Array vec;
//	for(std::vector<std::string>::const_iterator it=data.begin(); it != data.end(); it++)
//		vec.push_back(new StringItem(*it));
//
//	_item_map->replace(p,new FloatVectorItem(vec, ConfigValue::VALUE_STRING_ARRAY));
//	return true;
//}
//
//bool ConfigScript::Replace(const std::string& key,const Expression& data,int index)
//{
//	// Take exclusive access to the class
//#ifdef _THREAD_SAFE
//	ost::MutexLock mlock(_io_mutex);
//#endif
//
//	if (!Check())
//		return false;
//
//	std::string s_key, parent_key;
//
//	ItemMapIterator p;
//
//	if (StructAccess_I(key, s_key, parent_key, p, index))
//		return false;
//
//	_item_map->replace(p,new ExpressionItem(data));
//	return true;
//}

/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/src/rcfg/ConfigScriptOO.cpp,v $
* $Revision: 1.1 $ 
* $Date: 2005/02/09 17:26:57 $
* $Author: deepone $ 
* $Locker:  $

* Description: C Wrapper to simplify the use of Config script utility
  VRlab, Ume?University, 2001, Anders Backman

* $Log: ConfigScriptOO.cpp,v $
* Revision 1.1  2005/02/09 17:26:57  deepone
* vrutils and cfgscript put into new namespaces
*
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.7  2003/12/17 09:28:51  andersb
* Made configscript a dll library under windows
* Fixed memory leaks.
* Fixed bug in processing included files in wrong order.
*
* Revision 1.6  2003/12/16 11:34:45  andersb
* *** empty log message ***
*
* Revision 1.5  2003/12/15 15:31:24  andersb
* Adding write to configscript
*
* Revision 1.4  2003/12/05 10:43:37  deepone
* *** empty log message ***
*
* Revision 1.3  2003/12/02 15:40:14  andersb
* Added scoped struct access within the scripts.
*
* Revision 1.2  2003/11/19 19:14:33  andersb
* Added GetType method for ConfigScript. Returns the type of a named item
*
* Revision 1.1  2003/10/27 13:45:48  andersb
* Moved .cxx to .cpp
*
* Revision 1.29  2003/09/10 18:11:12  andersb
* Added WIN32 and UNIX macros that is available from scripts.
*
* Revision 1.28  2003/04/24 06:23:05  andersb
* Added support for Array of Strings
*
* Revision 1.27  2003/03/04 11:19:53  Anders Backman
* Fixed bug that required data beginning on the same line as the key,
* Added a search path feature. Default the environment variable CFG_FILE_PATH is enquired
* for a list of directories to search config files in.
*
* Revision 1.26  2003/02/28 14:42:53  Anders Backman
* no message
*
* Revision 1.25  2003/02/27 13:27:54  andersb
* Moved over to sstream instead of strstream
*
* Revision 1.24  2003/02/11 07:11:57  andersb
* no message
*
* Revision 1.23  2003/01/27 13:43:51  andersb
* Added ReadString for parsing a string instead of a file.
*
* Revision 1.22  2002/12/03 09:50:13  Anders Backman
* no message
*
* Revision 1.21  2002/12/02 20:33:50  andersb
* *** empty log message ***
*
* Revision 1.3  2002/03/13 09:04:03  andersb
* Compile with -D_THREAD_SAFE if thread safety is required. This also requires
* CommonC++ version 1.9.3 or higher.
*
* Also now Case sensitive for all items.
*
* Revision 1.2  2002/02/22 11:04:15  andersb
* Also working under linux
*
* Revision 1.1  2002/02/22 10:42:11  andersb
* ConfigScriptOO.h added which now implements a OO version of ConfigScript.
* Please use this in favour from the old C-version.
* The old ConfigScriptGetData-kind of version is now only a wrapper ontop of the OO version.
*
--------------------------------------------*/
