// $Id: export.h,v 1.1 2005/02/09 17:26:55 deepone Exp $

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LUAPLUGINS_EXPORT
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LUAPLUGINS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifndef __export_rvrutils_h_34_
#define __export_rvrutils_h_34_
#ifdef _WIN32

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#ifdef VRUTILS_EXPORTS
#define VRUTILS_API __declspec(dllexport)
#else
#define VRUTILS_API __declspec(dllimport)
#endif

#if 0
// This class is exported from the osgVortex.dll
class VRUTILS_API CosgVortex {
public:
	CosgVortex(void);
	// TODO: add your methods here.
};

extern VRUTILS_API int nosgVortex;

VRUTILS_API int fnosgVortex(void);
#endif

#else
#define VRUTILS_API
#endif



#endif
