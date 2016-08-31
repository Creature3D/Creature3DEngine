#ifndef _CONFIG_ERROR_H
#define _CONFIG_ERROR_H

#include <string>
#include <stdexcept>
#include "rbody/export.h"

namespace rbody {

	/// Exception class for errors during body configuration and setup
	class config_error : public std::runtime_error {
	
	public:
		/// Constructor
		config_error(const std::string& what_arg) : runtime_error("Body Configuration Error: " + what_arg) {};

	};

} // namespace end

#endif // _CONFIG_ERROR_H
/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rbody/config_error.h,v $
* $Revision: 1.2 $ 
* $Date: 2005/07/22 06:14:05 $
* $Author: deepone $ 
* $Locker:  $

* Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

 Created: 2001-12-12

* $Log: config_error.h,v $
* Revision 1.2  2005/07/22 06:14:05  deepone
* *** empty log message ***
*
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.2  2003/10/31 13:21:47  andersb
* Added ReplicantBodyMgr class to handle instances of CreBody
* Added .char files for specifying individual characters (CreBody)
* Added OSG plugin for reading char files
*
* Revision 1.1  2002/06/20 12:51:50  sunna
* Restructured directories
*
* Revision 1.2  2002/03/15 08:55:07  andersb
* no message
*
* Revision 1.1.1.1  2002/03/14 20:48:40  andersb
* First import as ReplicantBody
*
* Revision 1.1.1.1  2002/01/04 08:22:35  andersb
* Initial version
*
----------------------------------------------------------------------------*/


