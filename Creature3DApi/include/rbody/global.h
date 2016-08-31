// $Id: global.h,v 1.1.1.1 2005/02/06 06:53:00 deepone Exp $

#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "rbody/export.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#pragma warning (disable: 4786) // Disable warnings about long names
#pragma warning (disable: 4788) // Disable warnings about ...
#pragma warning( disable : 4290 ) // Disable warnings about C++ exception specification
#pragma warning(disable: 4251) // Disable warnings about exporting functions
#pragma warning(disable: 4275) // Disable warnings about exporting functions
#endif // _WIN32

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif


#endif // _GLOBAL_H

/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rbody/global.h,v $
* $Revision: 1.1.1.1 $ 
* $Date: 2005/02/06 06:53:00 $
* $Author: deepone $ 
* $Locker:  $

* Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

 Created: 2001-12-12

* $Log: global.h,v $
* Revision 1.1.1.1  2005/02/06 06:53:00  deepone
* initial sourceforge checkin
*
* Revision 1.3  2003/11/12 10:12:11  andersb
* Body inherits from PropertyContainer again.
* Moved Direction property out of Body.
*
* Revision 1.2  2003/10/31 13:21:47  andersb
* Added ReplicantBodyMgr class to handle instances of CreBody
* Added .char files for specifying individual characters (CreBody)
* Added OSG plugin for reading char files
*
* Revision 1.1  2002/06/20 12:51:50  sunna
* Restructured directories
*
* Revision 1.1.1.1  2002/03/14 20:48:40  andersb
* First import as ReplicantBody
*
* Revision 1.2  2002/03/01 10:26:36  andersb
* no message
*
* Revision 1.1.1.1  2002/01/04 08:22:35  andersb
* Initial version
*
----------------------------------------------------------------------------*/
