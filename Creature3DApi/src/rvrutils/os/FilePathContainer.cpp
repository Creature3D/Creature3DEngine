// $Id: FilePathContainer.cpp,v 1.1 2005/02/09 17:26:57 deepone Exp $

#include "rvrutils/os/FilePathContainer.h"
#include <sys/stat.h>

namespace rvrutils {

FilePathContainer::FilePathContainer()
{
  // Set path delimiter for environment variable depending on operative system
  m_delimiter = FP_PATH_DELIMITER;
}

FilePathContainer::FilePathContainer(const std::string& env)
{
  m_delimiter = FP_PATH_DELIMITER;

  setEnvironmentVariable(env);

}


FilePathContainer::~FilePathContainer()
{ 
  m_path_list.clear(); 
}


void FilePathContainer::setFilePathList(const std::string& paths, const std::string& delimitor) 
{ 
  m_path_list.clear(); 
  convertStringToList(paths, m_path_list, delimitor); 
}

std::list<std::string>& FilePathContainer::getFilePathList() 
{ 
  return m_path_list; 
}


bool FilePathContainer::setEnvironmentVariable(const std::string& env)
{

  // Clear path list
  m_path_list.clear();

  const char *path;
  if( (path = getenv(env.c_str()))){

    // Set path list to paths existing in environment variable
    convertStringToList(path, m_path_list, m_delimiter);

    return true;
  }

  return false;
}


void FilePathContainer::setFilePath(const std::string& path)
{

  // Clear path list
  m_path_list.clear();

  // Set path list to paths existing in environment variable
  convertStringToList(path, m_path_list, m_delimiter);

}

void FilePathContainer::addFilePath(const std::string& path)
{

  PathList pl;
  // Set path list to paths existing in environment variable
  convertStringToList(path, pl, m_delimiter);

  // Merge the new with the previous one
  m_path_list.merge(pl);
}



void FilePathContainer::convertStringToList(const std::string& str, std::list<std::string>& lst, const std::string& delimiter)
{

  std::string::size_type start = 0, end;
  std::string sub_str;

  do {

    end = str.find(m_delimiter, start);

    sub_str = str.substr(start, end - start);

    lst.push_back(sub_str);

    start = end + delimiter.size();

  } while (end != std::string::npos);


}


/*----------------------------------------------------------------------------
 Extract directory from path (remove filename)
 ----------------------------------------------------------------------------*/
std::string FilePathContainer::extractDirectory(const std::string& path)
{
  std::string::size_type slash = path.find_last_of('/');

  if (slash == std::string::npos) 
    return std::string("");

  return std::string(path.begin(), path.begin() + slash + 1);
  
}


/*----------------------------------------------------------------------------
 Extract filename from path
 ----------------------------------------------------------------------------*/
std::string FilePathContainer::extractFilename(const std::string& path)
{

  std::string::size_type slash = path.find_last_of('/');

  if (slash == std::string::npos) 
    return path;

  return std::string(path.begin() + slash + 1, path.end());

}


std::string FilePathContainer::findFile(const std::string& filename)
{

  std::list<std::string>::iterator list_it;

  // Loop through all paths and try to find file
  for (list_it = m_path_list.begin(); list_it != m_path_list.end(); list_it++) {

    std::string full_path = (*list_it);

    // Make sure path ends with "/"
    if ((!full_path.empty()) && (full_path[full_path.size() - 1] != '/') && (full_path[full_path.size() - 1] != '\\'))
      full_path.push_back('/');

    // Build path to file
    full_path += filename;
    
    // Debug
    //std::cerr << "Searching path: " << full_path << std::endl;

    /* Ok assume filename is a directory, assume further if
       someone have specified the directory with a / or a \ at the end,
       stat cant find that directory/file. 
       If we remove any existing /\, what will that mean?
       We will find a directory, but if someone by mistake adds a  \/ to
       a path, that error wont be discovered due to this.
       Just something to think about...
    */
    if (full_path.length()) {
      char s = full_path[full_path.length()-1];
      if (s == '\\' || s == '/')
        full_path = full_path.substr(0, full_path.length()-1);
    }

    // Can we access the file with this path?
    struct stat buf;
    // Does the file exist and we can read it?
    if (!stat( full_path.c_str(), &buf )) {
        if (buf.st_mode & S_IREAD ) {
          return std::string(full_path.begin(), full_path.end());
        }
    }

  }
  
  // Return empty string if file wasn't found
  return std::string("");
}

void FilePathContainer::pushbackPath(const std::string& path)
{
  m_path_list.push_back(path);
}

void FilePathContainer::pushfrontPath(const std::string& path)
{
  m_path_list.push_front(path);
}


}// Namespace rvrutils

/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/src/rvrutils/os/FilePathContainer.cpp,v $
* $Revision: 1.1 $ 
* $Date: 2005/02/09 17:26:57 $
* $Author: deepone $ 
* $Locker:  $

* Description: 
 
 Copyright (C) Peter Sunna, VRlab, Ume?University 2002

 Created: 2002-06-26

* $Log: FilePathContainer.cpp,v $
* Revision 1.1  2005/02/09 17:26:57  deepone
* vrutils and cfgscript put into new namespaces
*
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.10.8.1  2004/09/10 13:11:24  andersbl
* *** empty log message ***
*
* Revision 1.10  2003/12/19 16:40:09  deepone
* *** empty log message ***
*
* Revision 1.9  2003/10/31 13:27:14  andersb
* Added utils.cpp
* Changed from static to dynamic library for rvrutils
*
* Revision 1.8  2003/10/17 06:46:21  andersb
* *** empty log message ***
*
* Revision 1.7  2003/09/04 09:44:14  andersb
* Removes any trailing /\ when searching for a given filename.
* This enables it to actually find a directory, not just plain files.
*
* Revision 1.6  2003/06/13 14:52:03  deepone
* warn fix
*
* Revision 1.5  2003/05/16 10:35:48  deepone
* update
*
* Revision 1.4  2003/03/05 14:55:08  Anders Backman
* Changed the test for existing files from iostream::open to stat(). Now works for directories.
*
* Revision 1.3  2003/03/05 07:52:12  Anders Backman
* no message
*
* Revision 1.2  2003/02/28 08:14:13  Anders Backman
* Changed all strstream to sstream
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
* Revision 1.2  2002/08/30 09:50:29  andersb
* no message
*
* Revision 1.1  2002/08/27 06:29:38  andersb
* Rearranged alot.
*
* Revision 1.1.1.1  2002/08/16 13:37:06  andersb
* Initial release
*
* Revision 1.3  2002/08/15 14:34:28  sunna
* Fixed reference bug found by Daniel in convertStringToList()
*
* Revision 1.2  2002/06/27 14:29:33  sunna
* Added destructor, changed increment in findFile() to size of delimiter
*
* Revision 1.1  2002/06/27 10:20:03  sunna
* File path container for handling of several paths to files
*

----------------------------------------------------------------------------*/

