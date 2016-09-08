// $Id: utils.cpp,v 1.1 2005/02/09 17:26:57 deepone Exp $

#include "rcfg/utils.h"
#include <errno.h>
#include <wctype.h>
#include <time.h>



/// Removes leading and trailing blanks
std::string Trim( const std::string& str)
{
  std::string result(str);
  
  std::size_t startpos = result.find_first_not_of(" ");
  if (startpos != result.npos) // We found a blank
    result.erase(0, startpos);

  
  // Remove trailing blanks
  std::size_t blankpos = result.find_last_not_of(" "); // remove trailing blanks
  if (blankpos != result.npos) // We found a blank
    result.resize(blankpos+1);

  // A string with just blanks, remove the whole line
  if (startpos == result.npos && blankpos == result.npos)
    result.resize(0);

  return result;
}

/// Extracts a word from line, the word is removed from line
std::string GetWord( std::string& line )
{
  std::string word;
  std::size_t stop;

  line = line.substr(0, line.length()); // The word starts here
  line = Trim(line);

  stop = line.find_first_of(" "); // Find the next space, end of word
  if ( stop != line.npos ) {
    word = line.substr(0, stop);
    line = line.substr(stop, line.length());
    line = Trim(line);
  }

  // Is there anything left in the string?
  // that is, there could be only one word in the string
  else if ( line.length() ) {
    word = line;
    line.resize(0);
  }
  

  
  return word; 
}


/// Converts a string to a float, returns true for SUCCESS
bool StringToFloat(const std::string& word, float& val)
{
  float v;
  const char *cstr = word.c_str();

//  for(int i = strlen(cstr) - 1; i >= 0; i--)
 //   if ( !(isdigit(cstr[i]) || cstr[i] == '.' ||cstr[i] == '-' || cstr[i] == '+' ||cstr[i] == 'e' ||cstr[i] == 'E'))
 //     return false;

  char *endptr;
  v = (float)strtod(cstr, &endptr);

  if (endptr ==cstr) return false;  /* no conversion */
  
  while (isspace((unsigned char)*endptr)) endptr++;
  if (*endptr != '\0') return false;  /* invalid trailing characters? */
  val = v;
  
  return true;
}



	

/// Removes any non-printable characters on the line

void RemoveCntrlChar(std::string& line) 
{
#define TAB_LETTER 9
  

  std::string new_string;
  std::string::iterator string_it;
  for(string_it=line.begin(); string_it < line.end(); string_it++) {
    // OOOPPPPSSSS the check a > 0 not directly language independent.
    // The two last is to remove garbage from a OpenFlight reader..... Not GOOD
    if (/*iswprint(*string_it) &&*/ *string_it != TAB_LETTER)// && a != '? && a != 171)//吴财华 注：该函数对中文支持不好
      new_string.insert(new_string.end(),*string_it);
  }

  line = new_string;
}


/// Compares two strings with no case check. Returns -1 if s1 < s2, 1 if s1 > s2, 0 if s1==s2
int cmp_nocase( const std::string& s1, const std::string&s2)
{
  std::string::const_iterator p1 = s1.begin();
  std::string::const_iterator p2 = s2.begin();

  while (p1 != s1.end() && p2 != s2.end()) {
    if ( toupper(*p1) != toupper(*p2) ) 
      return (toupper(*p1) < toupper(*p2)) ? -1 : 1;
    ++p1;
    p2++;
  }

  return (s2.size() == s1.size()) ? 0 : (s1.size() < s2.size()) ? -1 : 1; 
}


/// Compounds a path with a file (portable)
std::string CompoundPath(const std::string& path, const std::string& file)
{
  std::string total = path;

  if (total.length()) {
    total += PATH_DELIMITER;
  }

  total += file;
  return total;
}


/// Remove any trailing path delimiter
void StripPathDelimiter(std::string& model_path)
{
  std::size_t pos;
  pos = model_path.find_last_of("/");
  if (pos != model_path.npos)
    model_path.resize(model_path.length()-1);

  pos = model_path.find_last_of("\\");
  if (pos != model_path.npos)
    model_path.resize(model_path.length()-1);
}


// Adds a path delimiter in a portable way
std::string AddPathDelimiter(const std::string &path)
{
  std::string result;

  if (!path.length())
    return path;

  result = path + PATH_DELIMITER;

  return result;
}

//
void ValidKey( const std::string& key )
{
  if (!key.length())
    throw std::string("zero length key");

  const char *cstr = key.c_str();

  for(int i = (unsigned int)strlen(cstr) - 1; i >= 0; i--)
      // Must be a alphanumeric, _ or begin with a # to be a valid key
    if (!(isalnum(cstr[i]) || cstr[i] == '_') || (cstr[i] == '#' && i != 0))
      throw "key contains invalid letters: \"" + key + "\"";
}



double RandInterval(double low, double high)
{
  static bool first=true;

  if (first) {
    first = false;
    srand((unsigned int) time(0));
  }
  if (low > high) {
    double t = high;
    high = low;
    low = t;
  }

  double length = high - low; // Length of interval

  double a = rand() / (double)RAND_MAX; // // Rand between 0.0 and 1.0
  double b = length * a + low;  // Rand between low and high

  return b;
}



#if 0
std::string getTempFileName()
{

  char temp_path[256];
  char temp_file[256];

#ifdef _WIN32
  GetTempPath(255, temp_path);
    

  UINT GetTempFileName(temp_path,  // pointer to directory name for temporary file
                       "",  // pointer to file name prefix
                       0,        // number used to create temporary file name
                       temp_file);
#else
  tempnam(temp_file);
#endif
  return std::string(tmp_file);
}
#endif
/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/src/rcfg/utils.cpp,v $
* $Revision: 1.1 $ 
* $Date: 2005/02/09 17:26:57 $
* $Author: deepone $ 
* $Locker:  $

* $Log: utils.cpp,v $
* Revision 1.1  2005/02/09 17:26:57  deepone
* vrutils and cfgscript put into new namespaces
*
* Revision 1.2  2005/02/09 10:02:35  vr-anders
* *** empty log message ***
*
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.4  2003/12/17 09:28:51  andersb
* Made configscript a dll library under windows
* Fixed memory leaks.
* Fixed bug in processing included files in wrong order.
*
* Revision 1.3  2003/12/16 11:34:46  andersb
* *** empty log message ***
*
* Revision 1.2  2003/12/05 10:43:37  deepone
* *** empty log message ***
*
* Revision 1.1  2003/10/27 13:45:49  andersb
* Moved .cxx to .cpp
*
* Revision 1.22  2003/02/27 13:08:58  andersb
* *** empty log message ***
*
* Revision 1.21  2003/01/27 13:43:52  andersb
* Added ReadString for parsing a string instead of a file.
*
* Revision 1.20  2002/12/17 16:55:11  Anders Backman
* no message
*
* Revision 1.19  2002/12/03 09:50:13  Anders Backman
* no message
*
* Revision 1.18  2002/11/27 10:56:08  andersb
* *** empty log message ***
*
* Revision 1.17  2002/11/26 15:25:41  Anders Backman
* no message
*
* Revision 1.16  2002/10/22 12:30:57  Anders Backman
* Moved around include files.
*
* Revision 1.15  2002/07/03 10:38:14  andersb
* no message
*
* Revision 1.14  2002/05/29 06:28:37  andersb
* Removed the use of the \ path delimiter under Windows, seems to work anyway.
*
* Revision 1.13  2002/05/21 22:05:08  andersb
* Fixed bug when reading a line that ends with a comment.
*
* Revision 1.12  2002/04/18 06:16:31  andersb
* Finally got into the mood to rewrite the ConfigIO so it supports both
* gcc of versions 2.x and 3.x. No more need of g++-3.0!!
*
* Revision 1.11  2002/04/12 13:51:57  andersb
* no message
*
* Revision 1.10  2002/04/12 08:49:57  andersb
* Peter found a bug in ConfigScriptOO.cxx made it crash when searching for
* indexed items with Get and Return methods.
*
* Revision 1.9  2002/04/09 09:04:56  andersb
* Major rewrite of ConfigIO.cxx.
* Now supports multiline strings and arrays.
*
* Revision 1.8  2002/04/06 21:05:07  andersb
* Found one huge bug for the Struct parser, not solved yet.
* Should rewrite the whole ConfigIO.cxx
*
* Revision 1.7  2002/02/01 13:35:30  andersb
* Now fully updated to support recursive structs.
* Still problems with case sensitivity.
*
* Revision 1.1.1.1  2002/01/02 13:45:31  andersb
* A new version of Configscript using namespace rcfg.
* Also supporting recursive structures.
*
* Revision 1.6  2001/11/14 13:12:54  andersb
* New features added:
* Hiearchial structures supported.
* Possible to use a mapID when opening to
* support multiple open configscripts.
*
* Revision 1.5  2001/10/02 13:12:11  andersb
* Allowed empty strings "" for strings.
*
* Revision 1.4  2001/08/16 06:41:38  andersb
* Functions in the ConfigScript utility is now thread safe using CommonC++  Mutex locks.
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
* Revision 1.11  2000/11/14 16:04:21  andersb
* Getting there...
*
* Revision 1.10  2000/11/09 15:21:19  andersb
* no message
*
* Revision 1.9  2000/11/06 16:04:12  andersb
* Load scenario + load view is working
*
* Revision 1.8  2000/11/03 14:47:45  andersb
* no message
*
* Revision 1.7  2000/10/16 08:43:33  andersb
* no message
*
* Revision 1.6  2000/10/03 07:44:35  andersb
* Using DOC++
*
* Revision 1.5  2000/10/02 09:31:14  andersb
* A Working SGI version of the ConfigIO files.
*
* Revision 1.4  2000/10/02 09:11:50  andersb
* no message
*

--------------------------------------------*/

