// $Id: ConfigScript.cpp,v 1.1 2005/02/09 17:26:57 deepone Exp $

#include "rcfg/ConfigScript.h"
#include "rcfg/ConfigScriptOO.h"
#include "rcfg/ConfigIO.h"
#include "rcfg/ItemMap.h"
#include "rcfg/Item.h"
#include <string>
#include <vector>
#include <stdexcept>
//#include <iostream.h>

#include <stdio.h>

#ifdef _THREAD_SAFE
#include <cc++/thread.h>
#endif

namespace rcfg {
  
  typedef std::map<int, ConfigScript *> ConfigScriptMap;
  typedef ConfigScriptMap::iterator ConfigScriptMapIterator;

  static ConfigScriptMap _script_map;
  static std::string _last_error;

/// Mutex used for secure access from several threads
#ifdef _THREAD_SAFE
static ost::Mutex _io_mutex;
#endif

static int _current_map=0;


inline ConfigScript *GetMap(int map_id)
{
  ConfigScript *map=0;
  
  // Is there a script associated with current map?
  ConfigScriptMapIterator it =_script_map.find(map_id);
  if (it == _script_map.end())
    return 0;
  
  // Is it opened?
  map = it->second;
  if (map->IsOpen())
    return map;
  else
    return 0;
}

inline bool Check(int map_id)
{
  return GetMap(map_id) != 0L;
}

inline ConfigScript *GetCurrentMap()
{
  return GetMap(_current_map);
}



// Set the current map to config_file_id
bool ConfigSetActive(int config_file_id)
{

  // Take exclusive access
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif
  
  if (Check(config_file_id))
    _current_map = config_file_id;
  else {
    char str[100];
    sprintf(str, "Map %d is not initialized\0", config_file_id);
    _last_error = str;
    return false;
  }
  return true;
}

bool ConfigStructPushOpen(const std::string& key, int index)
{

  // Take exclusive access
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  if (!Check(_current_map))
    return false;
  
  ConfigScript *map = GetCurrentMap();

  if (!map->Push(key, index)) {
    _last_error = map->GetLastError();
    return false;
  }

  return true;
}

bool ConfigStructPop( ConfigScript::StructSelection selection)
{
  // Take exclusive access
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  if (!Check(_current_map))
    return false;
  
  ConfigScript *map = GetCurrentMap();

  if (!map->Pop(selection)) {
    _last_error = map->GetLastError();
    return false;
  }

  return true;
}


// Returns the last occurred error message
std::string ConfigGetLastError( void ) 
{ 
  std::string s; 

  // Take exclusive access
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  s = _last_error; 

  return s; 
}



bool ConfigIsOpen( void ) 
{
  // Take exclusive access
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  bool s;
  s= GetCurrentMap() != 0L;
  return s;
}

// 
bool ConfigClose( MAP_ID id )
{

  // Take exclusive access
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  bool status=true;
  ConfigScript *map=0;

  // Close all opened maps
  if (id == ALL_MAPS) {
    _current_map=0;
    ConfigScriptMapIterator it;
    for(it=_script_map.begin(); it != _script_map.end(); it++)
      delete it->second;

    _script_map.clear();

  }
  else {
    int map_id;
    if (id == CURRENT_MAP)
      map_id = _current_map;
    else
      map_id = (int)id;
      
      map = GetMap(map_id);

      if (map) {
        delete map;

        _script_map.erase(map_id);
        ConfigScriptMapIterator it;
        it = _script_map.begin();
        if (it != _script_map.end())
          _current_map = it->first;
        else
          _current_map=0;
      }
      else {
        char str[100];
        sprintf(str, "Map %d is not opened, closing it will be hard\0", _current_map);
        _last_error = str;
        status = false;
      }
  }

  return status;
}


/*
 * Opens the ConfigScript file filename.
 * Checks that the first line in the file contains header
 * Parses the file for configuration data
 * \return bool - true if opening and parsing is successful.
 */
bool ConfigInit(const std::string& filename, const std::string& header, int map_id)  
{
  // Take exclusive access
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  ConfigScript *map = GetMap(map_id);
  if(map) {
    char str[100];
    sprintf(str, "Map %d is already initialized\0", map_id);
    _last_error = str;
    return false;
  }

  ConfigScript *script=0;
  try {
    script = new ConfigScript(filename, header);
  }
  catch(std::exception& e) {
    _last_error = e.what();
    delete script;
	return false;
  }

  // Associate the just opened ConfigScript with _current_map
  _script_map[map_id] = script;

  // Set the current map to be the one we just opened
  _current_map = map_id;

  return true;   
}

bool ConfigGetData(std::string key, int& data, int index) 
{

  // Take exclusive access
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  ConfigScript *map = GetCurrentMap();
  if (!map) {
    _last_error = "Error: Script is not opened. Call ConfigScriptInit first";
    return false;
  }
  
  return map->Get(key, data, index);
}

bool ConfigGetData(std::string key, float& data, int index) 
{

    // Take exclusive access
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  ConfigScript *map = GetCurrentMap();
  if (!map) {
    _last_error = "Error: Script is not opened. Call ConfigScriptInit first";
    return false;
  }
  
  return map->Get(key, data, index);
}

//
bool ConfigGetData(std::string key, std::string& data, int index) 
{

    // Take exclusive access
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  ConfigScript *map = GetCurrentMap();
  if (!map) {
    _last_error = "Error: Script is not opened. Call ConfigScriptInit first";
    return false;
  }
  
  return map->Get(key, data, index);
}


//
bool ConfigGetData(std::string key, FloatVector &data, int index) 
{
     // Take exclusive access
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  ConfigScript *map = GetCurrentMap();
  if (!map) {
    _last_error = "Error: Script is not opened. Call ConfigScriptInit first";
    return false;
  }
  
  return map->Get(key, data, index);

}





std::string ConfigReturnData(const std::string& key, const std::string& def, int index) 
{
     // Take exclusive access
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  ConfigScript *map = GetCurrentMap();
  if (!map) {
    _last_error = "Error: Script is not opened. Call ConfigScriptInit first";
    return def;
  }
  
  return map->Return(key, def, index);

}
 
int ConfigReturnData(const std::string& key, int def, int index) 
{
     // Take exclusive access
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  ConfigScript *map = GetCurrentMap();
  if (!map) {
    _last_error = "Error: Script is not opened. Call ConfigScriptInit first";
    return def;
  }
  
  return map->Return(key, def, index);

}

float ConfigReturnData(const std::string& key, float def, int index) 
{

  // Take exclusive access
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  ConfigScript *map = GetCurrentMap();
  if (!map) {
    _last_error = "Error: Script is not opened. Call ConfigScriptInit first";
    return def;
  }
  
  return map->Return(key, def, index);


}

std::vector<float> ConfigReturnData(const std::string& key, const std::vector<float> &def, int index) 
{

     // Take exclusive access
#ifdef _THREAD_SAFE
  ost::MutexLock mlock(_io_mutex);
#endif

  ConfigScript *map = GetCurrentMap();
  if (!map) {
    _last_error = "Error: Script is not opened. Call ConfigScriptInit first";
    return def;
  }
  
  return map->Return(key, def, index);

}

  
}; // namespace rcfg

/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/src/rcfg/ConfigScript.cpp,v $
* $Revision: 1.1 $ 
* $Date: 2005/02/09 17:26:57 $
* $Author: deepone $ 
* $Locker:  $

* Description: C Wrapper to simplify the use of Config script utility
  VRlab, Ume?University, 2001, Anders Backman

* $Log: ConfigScript.cpp,v $
* Revision 1.1  2005/02/09 17:26:57  deepone
* vrutils and cfgscript put into new namespaces
*
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.3  2003/12/05 10:43:37  deepone
* *** empty log message ***
*
* Revision 1.2  2003/12/02 15:40:14  andersb
* Added scoped struct access within the scripts.
*
* Revision 1.1  2003/10/27 13:45:48  andersb
* Moved .cxx to .cpp
*
* Revision 1.16  2002/10/22 12:30:56  Anders Backman
* Moved around include files.
*
* Revision 1.15  2002/05/21 22:05:08  andersb
* Fixed bug when reading a line that ends with a comment.
*
* Revision 1.14  2002/03/13 14:17:40  andersb
* *** empty log message ***
*
* Revision 1.13  2002/03/13 09:04:02  andersb
* Compile with -D_THREAD_SAFE if thread safety is required. This also requires
* CommonC++ version 1.9.3 or higher.
*
* Also now Case sensitive for all items.
*
* Revision 1.12  2002/03/04 13:02:34  andersb
* Missed a return false during parse error in ConfigScript.cxx
*
* Revision 1.11  2002/02/22 11:04:14  andersb
* Also working under linux
*
* Revision 1.10  2002/02/22 10:42:10  andersb
* ConfigScriptOO.h added which now implements a OO version of ConfigScript.
* Please use this in favour from the old C-version.
* The old ConfigScriptGetData-kind of version is now only a wrapper ontop of the OO version.
*
* Revision 1.9  2002/02/01 13:35:29  andersb
* Now fully updated to support recursive structs.
* Still problems with case sensitivity.
*
* Revision 1.2  2002/01/08 12:24:16  andersb
* *** empty log message ***
*
* Revision 1.1.1.1  2002/01/02 13:45:31  andersb
* A new version of Configscript using namespace rcfg.
* Also supporting recursive structures.
*
* Revision 1.8  2001/11/14 13:48:04  andersb
* *** empty log message ***
*
* Revision 1.7  2001/11/14 13:12:53  andersb
* New features added:
* Hiearchial structures supported.
* Possible to use a mapID when opening to
* support multiple open configscripts.
*
* Revision 1.6  2001/10/23 13:31:01  andersb
* no message
*
* Revision 1.5  2001/10/03 15:27:03  andersb
* Retriving a float will accept an integer (conversion to float)
*
* Revision 1.4  2001/08/16 06:41:38  andersb
* Functions in the ConfigScript utility is now thread safe using CommonC++  Mutex locks.
*
* Revision 1.3  2001/08/01 07:11:22  andersb
* Major update
*
* Revision 1.2  2001/04/25 12:45:04  andersb
* Added makefiles for the IRIX platform.
*
* Revision 1.1.1.1  2001/04/25 10:56:28  andersb
* no message
*

--------------------------------------------*/
