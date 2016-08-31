// $Id: utils.h,v 1.1 2005/02/09 17:26:55 deepone Exp $

#ifndef __UTILS_H__
#define __UTILS_H__

#ifdef _WIN32
#pragma warning(disable:4786) // Disable warnings about long names
#include "rvrutils/os/memory_debug.h"
#endif

#include "rcfg/export.h"

#ifndef MIN
//#ifdef _WIN32
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
//#endif
#endif

#ifndef MAX
#ifdef _WIN32
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif
#endif

//#ifdef _WIN32
//#define PATH_DELIMITER  "\\"
//#else
#define PATH_DELIMITER   "/"
//#endif


// Check which compiler we are using
// If it is a gnu compiler of version lower than 3 than use old iostream libs.
#ifdef __GNUC__
#if __GNUC__ <= 2
#define HAVE_NEW_IOSTREAM 0
#else
#define HAVE_NEW_IOSTREAM 1
#endif

#else
#define HAVE_NEW_IOSTREAM 1 // Default
#endif

#include <string>
///

#include "rcfg/ConfigValue.h"


///
std::string CONFIGSCRIPT_API GetWord( std::string& line );
///
std::string CONFIGSCRIPT_API Trim( const std::string& str);

///
// Moved to ConfigIO::
//void ValidValue( const std::string& value, ConfigValue::ValueType &type );
///
// Moved to ConfigIO::
void CONFIGSCRIPT_API ValidKey( const std::string& key );

///
bool CONFIGSCRIPT_API StringToFloat(const std::string& word, float& val);


///
void CONFIGSCRIPT_API RemoveCntrlChar(std::string& line);
///
int CONFIGSCRIPT_API cmp_nocase( const std::string& s1, const std::string&s2);

void CONFIGSCRIPT_API StripPathDelimiter(std::string& model_path);

std::string CONFIGSCRIPT_API CompoundPath(const std::string& path, const std::string& file);
std::string CONFIGSCRIPT_API AddPathDelimiter(const std::string &path);


/// Return a unique filename that can be used for opening temporary files.
std::string CONFIGSCRIPT_API getTempFileName();

/// Returns a random number in the interval low to high
double CONFIGSCRIPT_API RandInterval(double low=0, double high=1);

#endif

/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rcfg/utils.h,v $
* $Revision: 1.1 $ 
* $Date: 2005/02/09 17:26:55 $
* $Author: deepone $ 
* $Locker:  $

* $Log: utils.h,v $
* Revision 1.1  2005/02/09 17:26:55  deepone
* vrutils and cfgscript put into new namespaces
*
* Revision 1.1.1.1  2005/02/06 06:53:00  deepone
* initial sourceforge checkin
*
* Revision 1.8  2003/12/17 09:28:50  andersb
* Made configscript a dll library under windows
* Fixed memory leaks.
* Fixed bug in processing included files in wrong order.
*
* Revision 1.7  2003/12/16 11:34:44  andersb
* *** empty log message ***
*
* Revision 1.6  2003/12/05 10:43:37  deepone
* *** empty log message ***
*
* Revision 1.5  2003/02/27 14:02:57  andersb
* Works under gcc3.2
*
* Revision 1.4  2003/02/27 13:27:53  andersb
* Moved over to sstream instead of strstream
*
* Revision 1.3  2002/12/17 16:55:10  Anders Backman
* no message
*
* Revision 1.2  2002/11/26 15:25:40  Anders Backman
* no message
*
* Revision 1.1  2002/10/22 12:30:56  Anders Backman
* Moved around include files.
*
* Revision 1.11  2002/05/29 06:28:25  andersb
* Removed the use of the \ path delimiter under Windows, seems to work anyway.
*
* Revision 1.10  2002/05/21 22:04:57  andersb
* Fixed bug when reading a line that ends with a comment.
*
* Revision 1.9  2002/04/18 06:23:26  andersb
* Minor error in GCC version check.
*
* Revision 1.8  2002/04/18 06:16:31  andersb
* Finally got into the mood to rewrite the ConfigIO so it supports both
* gcc of versions 2.x and 3.x. No more need of g++-3.0!!
*
* Revision 1.7  2002/04/09 09:04:48  andersb
* Major rewrite of ConfigIO.cxx.
* Now supports multiline strings and arrays.
*
* Revision 1.6  2002/02/01 13:35:21  andersb
* Now fully updated to support recursive structs.
* Still problems with case sensitivity.
*
* Revision 1.1.1.1  2002/01/02 13:45:30  andersb
* A new version of Configscript using namespace rcfg.
* Also supporting recursive structures.
*
* Revision 1.5  2001/11/14 13:12:47  andersb
* New features added:
* Hiearchial structures supported.
* Possible to use a mapID when opening to
* support multiple open configscripts.
*
* Revision 1.4  2001/08/16 06:41:36  andersb
* Functions in the ConfigScript utility is now thread safe using CommonC++  Mutex locks.
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
* Revision 1.12  2000/11/23 08:09:35  andersb
* Working SGI version
*
* Revision 1.11  2000/11/14 16:04:21  andersb
* Getting there...
*
* Revision 1.10  2000/11/09 15:21:20  andersb
* no message
*
* Revision 1.9  2000/11/03 14:47:45  andersb
* no message
*
* Revision 1.8  2000/10/16 08:43:34  andersb
* no message
*
* Revision 1.7  2000/10/05 09:56:04  andersb
* no message
*
* Revision 1.6  2000/10/03 07:44:36  andersb
* Using DOC++
*
* Revision 1.5  2000/10/03 07:02:38  andersb
* no message
*
* Revision 1.4  2000/10/02 09:31:14  andersb
* A Working SGI version of the ConfigIO files.
*
* Revision 1.3  2000/10/02 09:08:34  andersb
* no message
*
* Revision 1.2  2000/09/26 14:14:36  andersb
* no message
*
* Revision 1.1.1.1  2000/09/26 09:05:46  andersb
* no message
*

--------------------------------------------*/
