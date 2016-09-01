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
#include <CRCore/crNotify.h>
#include <string>

using namespace std;

CRCore::NotifySeverity g_NotifyLevel = CRCore::NOTICE;

void CRCore::setNotifyLevel(CRCore::NotifySeverity severity)
{
	CRCore::initNotifyLevel();
	g_NotifyLevel = severity;
}


CRCore::NotifySeverity CRCore::getNotifyLevel()
{
	CRCore::initNotifyLevel();
	return g_NotifyLevel;
}


bool CRCore::initNotifyLevel()
{
	static bool s_NotifyInit = false;

	if (s_NotifyInit) return true;

	s_NotifyInit = true;

	// g_NotifyLevel
	// =============

	g_NotifyLevel = CRCore::NOTICE; // Default value

	char* CRNOTIFYLEVEL=getenv("CR_NOTIFY_LEVEL");
	if (!CRNOTIFYLEVEL) CRNOTIFYLEVEL=getenv("CRNOTIFYLEVEL");
	if(CRNOTIFYLEVEL)
	{

		std::string stringCRNOTIFYLEVEL(CRNOTIFYLEVEL);

		// Convert to upper case
		for(std::string::iterator i=stringCRNOTIFYLEVEL.begin();
			i!=stringCRNOTIFYLEVEL.end();
			++i)
		{
			*i=toupper(*i);
		}

		if(stringCRNOTIFYLEVEL.find("ALWAYS")!=std::string::npos)          g_NotifyLevel=CRCore::ALWAYS;
		else if(stringCRNOTIFYLEVEL.find("FATAL")!=std::string::npos)      g_NotifyLevel=CRCore::FATAL;
		else if(stringCRNOTIFYLEVEL.find("WARN")!=std::string::npos)       g_NotifyLevel=CRCore::WARN;
		else if(stringCRNOTIFYLEVEL.find("NOTICE")!=std::string::npos)     g_NotifyLevel=CRCore::NOTICE;
		else if(stringCRNOTIFYLEVEL.find("DEBUG_INFO")!=std::string::npos) g_NotifyLevel=CRCore::DEBUG_INFO;
		else if(stringCRNOTIFYLEVEL.find("DEBUG_FP")!=std::string::npos)   g_NotifyLevel=CRCore::DEBUG_FP;
		else if(stringCRNOTIFYLEVEL.find("DEBUG")!=std::string::npos)      g_NotifyLevel=CRCore::DEBUG_INFO;
		else if(stringCRNOTIFYLEVEL.find("INFO")!=std::string::npos)       g_NotifyLevel=CRCore::INFO;
		//else std::cout << "Warning: invalid CR_NOTIFY_LEVEL set ("<<stringCRNOTIFYLEVEL<<")"<<std::endl;

	}

	return true;

}

#if defined(WIN32) && !(defined(__CYGWIN__) || defined(__MINGW32__))
const char* NullStreamName = "nul";
#else
const char* NullStreamName = "/dev/null";
#endif

std::ostream& CRCore::notify(const CRCore::NotifySeverity severity)
{
	//// set up global notify null stream for inline notify
	//static std::ofstream s_NotifyNulStream(NullStreamName);

	//static bool initialized = false;
	//if (!initialized) 
	//{
	//	std::cerr<<""; // dummy op to force construction of cerr, before a reference is passed back to calling code.
	//	//std::cout<<""; // dummy op to force construction of cout, before a reference is passed back to calling code.
	//	initialized = CRCore::initNotifyLevel();
	//}

	//if (severity<=g_NotifyLevel)
	//{
	//	if (severity<=CRCore::WARN) return std::cerr;
	//	else return std::cout;
	//}
	//return s_NotifyNulStream;
	return std::cerr;
}
