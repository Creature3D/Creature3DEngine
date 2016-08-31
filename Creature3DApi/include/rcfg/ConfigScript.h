// $Id: ConfigScript.h,v 1.1 2005/02/09 17:26:54 deepone Exp $

#ifndef __CONFIGSCRIPT_H__
#define __CONFIGSCRIPT_H__


#ifdef _WIN32
  #pragma warning(disable:4786) // Disable warnings about long names

#include "rvrutils/os/memory_debug.h"
#endif



#include <string>
#include <vector>
#include "rcfg/Item.h"
#include "rcfg/ConfigScriptOO.h"
#include "rcfg/export.h"
namespace rcfg {

  enum MAP_ID { ALL_MAPS=0, CURRENT_MAP=1 };

  /*! 
  * Releases memory associated with open configfile.
  * \param id - Indicates if CURRENT_MAP (set with ConfigSetActive) or ALL_MAPS or given as
  * a integer id opened configfiles should be closed
  */
  bool CONFIGSCRIPT_API ConfigClose(MAP_ID id=ALL_MAPS);

  /// Returns true if the current (set with ConfigSetActive) config file is successfully opened and parsed
bool CONFIGSCRIPT_API  ConfigIsOpen( void );

/*!
 * Opens the ConfigScript file filename.
 * Checks that the first line in the file contains header
 * Parses the file for configuration data
 * \param filename - The file to open
 * \param header - The header of the file that will be opened. This header must match the one in the file
 * \param map_id - Associates an id with the opened configfile for later retrieval with ConfigSetActive
 * \return bool - true if opening and parsing is successful.
 */
bool CONFIGSCRIPT_API  ConfigInit(const std::string& filename, const std::string& header, int map_id=0);

/*!
 * Any error message set during a call to any configfunctions can be retreived with ConfigGetLastError
 * \return std::string - A message containing the last set error message.
*/
std::string CONFIGSCRIPT_API  ConfigGetLastError( void );

/*!
 * Pops the stack of opened structs
 * \return bool - true if stack before the call to ConfigStructPop was not empty otherwise false.
*/
bool CONFIGSCRIPT_API  ConfigStructPop( ConfigScript::StructSelection selection=ConfigScript::ONE ); 

/*!
 * Pushes a given (in key) struct to the opened stack. This will cause all successive data retrieves to 
 * work from the opened struct and beneath.
 * i.e. PushOpen("aaa"); Will work on the struct aaa { ... }
 * \param key - The name of the struct to be opened
 * \param index - If the key exists several times (multiple key) index selects which one to open (1 = first)
 * \return true if the struct key was found.
 */
bool CONFIGSCRIPT_API  ConfigStructPushOpen(const std::string& key, int index=1); 

/*!
 * If several configfiles are opened, selection which one is current is done with ConfigSetActive.
 * After a call to this function the current configfile is set until it is called again.
 * \param int config_file_id - An id of the previously opened configfile
 * \return bool - true if a valid (previously opened) config_id was given, otherwise false.
 */
bool  CONFIGSCRIPT_API ConfigSetActive(int config_file_id);


/*! Overloaded function to retrieve data associated to key read from configfile
    Returns false when key is either missing or of wrong type
 */
bool CONFIGSCRIPT_API  ConfigGetData(std::string key, int& data, int index=1); 

/*! Overloaded function to retrieve data associated to key read from configfile
    Returns false when key is either missing or of wrong type
 */
bool  CONFIGSCRIPT_API ConfigGetData(std::string key, float& data, int index=1); 

/*! Overloaded function to retrieve data associated to key read from configfile
    Returns false when key is either missing or of wrong type
 */
bool CONFIGSCRIPT_API  ConfigGetData(std::string key, std::string&, int index=1); 

/*! Overloaded function to retrieve data associated to key read from configfile
    Returns false when key is either missing or of wrong type
 */
bool CONFIGSCRIPT_API  ConfigGetData(std::string key, std::vector<float> &data, int index=1); 

/*! Overloaded function to retrieve data associated to key read from configfile
    Returns the value if found, otherwise the specified default value will be returned.
 */
std::string CONFIGSCRIPT_API  ConfigReturnData(const std::string& key, const std::string& def, int index=1); 

/*! Overloaded function to retrieve data associated to key read from configfile
    Returns the value if found, otherwise the specified default value will be returned.
 */
int  CONFIGSCRIPT_API ConfigReturnData(const std::string& key, int def, int index=1); 

/*! Overloaded function to retrieve data associated to key read from configfile
    Returns the value if found, otherwise the specified default value will be returned.
 */
float CONFIGSCRIPT_API  ConfigReturnData(const std::string& key, float def, int index=1); 

/*! Overloaded function to retrieve data associated to key read from configfile
    Returns the value if found, otherwise the specified default value will be returned.
 */
std::vector<float> CONFIGSCRIPT_API  ConfigReturnData(const std::string& key, const std::vector<float> &def, int index=1); 

}; //namespace rcfg

#endif // __CONFIGSCRIPT_H__

/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rcfg/ConfigScript.h,v $
* $Revision: 1.1 $ 
* $Date: 2005/02/09 17:26:54 $
* $Author: deepone $ 
* $Locker:  $

* Description: C Wrapper to simplify the use of Config script utility
  VRlab, Ume?University, 2001, Anders Backman

* $Log: ConfigScript.h,v $
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
* Revision 1.6  2002/02/22 10:42:04  andersb
* ConfigScriptOO.h added which now implements a OO version of ConfigScript.
* Please use this in favour from the old C-version.
* The old ConfigScriptGetData-kind of version is now only a wrapper ontop of the OO version.
*
* Revision 1.5  2002/02/01 13:35:20  andersb
* Now fully updated to support recursive structs.
* Still problems with case sensitivity.
*
* Revision 1.1.1.1  2002/01/02 13:45:30  andersb
* A new version of Configscript using namespace rcfg.
* Also supporting recursive structures.
*
* Revision 1.4  2001/11/14 13:12:46  andersb
* New features added:
* Hiearchial structures supported.
* Possible to use a mapID when opening to
* support multiple open configscripts.
*
* Revision 1.3  2001/08/01 07:11:20  andersb
* Major update
*
* Revision 1.2  2001/04/25 12:45:00  andersb
* Added makefiles for the IRIX platform.
*
* Revision 1.1.1.1  2001/04/25 10:56:27  andersb
* no message
*


--------------------------------------------*/
