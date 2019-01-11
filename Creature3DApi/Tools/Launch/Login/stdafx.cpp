// stdafx.cpp : source file that includes just the standard includes
// Launcher.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#include <fstream>
#include <string>
#include <stdarg.h>

using namespace std;

void debuglog(const char *logFileDir,const char * format,...)
{
#ifdef _TEST_DEBUG_
	char buffer[256];
	va_list varptr;
	va_start(varptr, format);

	vsnprintf(buffer,256 ,format, varptr);

	fstream file;
	std::string fileName =string(logFileDir)+std::string("WebLauncherLog.Log");
	file.open(fileName,ios::in|ios::out|ios::app);
	if(file.is_open())
	{
		file.write(buffer,strlen(buffer));
	}
	file.close();

	va_end(varptr);

#endif
}


