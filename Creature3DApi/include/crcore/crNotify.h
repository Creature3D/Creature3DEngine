/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
#ifndef CRCORE_NOTIFY
#define CRCORE_NOTIFY 1

#include <CRCore/crExport.h>
//#include <CRCore/Referenced.h>	
//#include <CRCore/ref_ptr.h>
//#include <CRCore/crStreamBuf.h>
#include <iostream>
#include <fstream>
#include <memory>

namespace CRCore {

	/** Range of notify levels from DEBUG_FP through to FATAL, ALWAYS
	* is reserved for forcing the absorption of all messages.  The
	* keywords are also used verbatim when specified by the environmental
	* variable CRNOTIFYLEVEL.  See documentation on CRCore::notify() for
	* further details.
	*/
	enum NotifySeverity {
		ALWAYS=0,
		FATAL=1,
		WARN=2,
		NOTICE=3,
		INFO=4,
		DEBUG_INFO=5,
		DEBUG_FP=6
	};

	/** global notify level. */
	//extern CR_EXPORT NotifySeverity g_NotifyLevel;

	/** global notify nul stream. added for Mac OSX */
	//extern CR_EXPORT std::auto_ptr<std::ofstream> g_NotifyNulStream;

	/** global notify nul stream. added for Mac OSX */
	//extern CR_EXPORT bool g_NotifyInit;

	/** set the notify level, overriding the default or value set by
	* the environmental variable OSGNOTIFYLEVEL.
	*/
	extern CR_EXPORT void setNotifyLevel(NotifySeverity severity);

	/** get the notify level. */
	extern CR_EXPORT NotifySeverity getNotifyLevel();

	/** initialize notify level. */
	extern CR_EXPORT bool initNotifyLevel();

	/** notify messaging function for providing fatal through to verbose
	* debugging messages.  Level of messages sent to the console can
	* be controlled by setting the NotifyLevel either within your 
	* application or via the an environmental variable. For instance
	* setenv OSGNOTIFYLEVEL DEBUG (for tsh), export OSGNOTIFYLEVEL=DEBUG
	* (for bourne shell) or set OSGNOTIFYLEVEL=DEBUG (for Windows) all
	* set tell the CRCore to redirect all debugging and more important messages
	* to the console (useful for debugging :-)  setting ALWAYS will force
	* all messages to be absorbed, which might be appropriate for final
	* applications.  Default NotifyLevel is NOTICE.  Check the enum 
	* NotifySeverity for full range of possibilities.  To use the notify
	* with your code simply use the notify function as a normal file
	* stream (like cout) i.e CRCore::notify(CRCore::DEBUG) << "Hello Bugs!"<<endl;
	*/

	//
	// PJA MAC OSX 30-09-01 
	// previous implementation was causing Mac OSX to misbehave. This version
	// places less stress on compiler and runs on Mac

	extern CR_EXPORT std::ostream& notify(const NotifySeverity severity);
	// {
	//     static bool initilized = false;
	//     if (!initilized) initilized = initNotifyLevel();
	// 
	//     if (severity<=g_NotifyLevel)
	//     {
	//         if (severity<=CRCore::WARN) return std::cerr;
	//         else return std::cout;
	//     }
	//     cout << "doing a notify to null stream "<<CRCore::g_NotifyLevel<<" "<<CRCore::g_NotifyNulStream.get()<<" "<<CRCore::g_NotifyInit<<endl;
	//     //if (CRCore::g_NotifyNulStream.get()==0L) abort();
	//     return std::cout;    
	//     //return *CRCore::g_NotifyNulStream;
	// }

	inline std::ostream& notify(void) { return notify(CRCore::INFO); }
}

#endif
