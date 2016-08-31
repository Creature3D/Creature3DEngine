// $Id: export.h,v 1.1 2005/02/09 17:26:55 deepone Exp $

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CONFIGSCRIPT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CONFIGSCRIPT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifndef __config_script_export_
#define __config_script_export_

#ifdef _WIN32

#pragma once

#pragma warning(disable: 4251) // Disable warnings about exporting functions
#pragma warning(disable: 4996)
#pragma warning(disable: 4189)

#ifdef CONFIGSCRIPT_EXPORTS
#define CONFIGSCRIPT_API __declspec(dllexport)
#else
#define CONFIGSCRIPT_API __declspec(dllimport)
#endif

#else
#define CONFIGSCRIPT_API
#endif


#endif
