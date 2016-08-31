// $Id: FilePathContainer.h,v 1.1 2005/02/09 17:26:55 deepone Exp $

#ifndef __rvrutils_FILEPATHCONTAINER_H
#define __rvrutils_FILEPATHCONTAINER_H

#ifdef _WIN32
  #pragma warning(disable:4786) // Disable warnings about long names
  #pragma warning(disable: 4251) // Disable warnings about exporting functions
#endif

#include <string>
#include <list>
#include <fstream>
#include <iostream>
#include "rvrutils/export.h"

namespace rvrutils {
class VRUTILS_API FilePathContainer 
{

  #if defined(WIN32) && !defined(__CYGWIN__)
    #define FP_PATH_DELIMITER ";"
  #else
    #define FP_PATH_DELIMITER ":"
  #endif


public:
  /** Constructor */
  FilePathContainer();

  /** Constructor - set the file path found in given environment variable */
  FilePathContainer(const std::string& env);

  /** Destructor */
  ~FilePathContainer();

  /** Set the file path using a single string deliminated with given delimitor */
  void setFilePathList(const std::string& paths, const std::string& delimitor = FP_PATH_DELIMITER); 

  /** Set the file path found in given environment variable - returns true if env var found */
  bool setEnvironmentVariable(const std::string& env);

  /** Set the file path directly */
  void setFilePath(const std::string& path);
  
  /** Adds a list of paths to the previous one */
  void FilePathContainer::addFilePath(const std::string& path);

  /**  Adds a filepath to the end of the list */
  void pushbackPath(const std::string& path);

  /**  Adds a filepath to the beginning of the list */
  void pushfrontPath(const std::string& path);

  /** Get file path list */
  std::list<std::string>& getFilePathList();

  /** Get directory from path */
  std::string extractDirectory(const std::string& path);

  /** Get filename from path */
  std::string extractFilename(const std::string& path);

  /** Return path to given filename if file exists in any directory */
  std::string findFile(const std::string& filename);

private:

  /** Path list */
  typedef  std::list<std::string>  PathList;
  PathList m_path_list;


  /** Convert given string to list with sub strings (seperated by delimitor in string) */
  void convertStringToList(const std::string& str, PathList& lst, const std::string& delimitor);


  /** Delimitor used when parsing environment varaible */
  std::string m_delimiter;

};
} // namespace rvrutils
#endif // __rvrutils_FILEPATHCONTAINER_H

/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rvrutils/os/FilePathContainer.h,v $
* $Revision: 1.1 $ 
* $Date: 2005/02/09 17:26:55 $
* $Author: deepone $ 
* $Locker:  $

* Description: 
 
 Copyright (C) Peter Sunna, VRlab, Ume?University 2002

 Created: 2002-06-26

* $Log: FilePathContainer.h,v $
* Revision 1.1  2005/02/09 17:26:55  deepone
* vrutils and cfgscript put into new namespaces
*
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.8.8.1  2004/09/10 12:43:30  andersbl
* *** empty log message ***
*
* Revision 1.8  2003/10/31 13:27:13  andersb
* Added utils.cpp
* Changed from static to dynamic library for rvrutils
*
* Revision 1.7  2003/10/17 06:46:04  andersb
* *** empty log message ***
*
* Revision 1.6  2003/05/16 10:35:47  deepone
* update
*
* Revision 1.5  2003/03/05 07:52:12  Anders Backman
* no message
*
* Revision 1.4  2003/02/28 08:14:12  Anders Backman
* Changed all strstream to sstream
*
* Revision 1.3  2002/11/18 13:20:50  Anders Backman
* no message
*
* Revision 1.2  2002/10/22 12:33:00  Anders Backman
* no message
*
* Revision 1.1.1.1  2002/09/25 13:14:10  deepone
* initial import
*
* Revision 1.2  2002/09/24 20:55:58  deepone
* fix
*
* Revision 1.1.1.1  2002/09/24 20:51:19  deepone
* initial import
*
* Revision 1.3  2002/08/30 09:50:28  andersb
* no message
*
* Revision 1.2  2002/08/30 09:09:41  andersb
* no message
*
* Revision 1.1  2002/08/27 06:29:36  andersb
* Rearranged alot.
*
* Revision 1.1.1.1  2002/08/16 13:37:06  andersb
* Initial release
*
* Revision 1.3  2002/08/15 14:34:28  sunna
* Fixed reference bug found by Daniel in convertStringToList()
*
* Revision 1.2  2002/06/27 14:29:33  sunna
* Added destructor, changed increment in findFile() to size of delimitor
*
* Revision 1.1  2002/06/27 10:20:03  sunna
* File path container for handling of several paths to files
*

----------------------------------------------------------------------------*/

