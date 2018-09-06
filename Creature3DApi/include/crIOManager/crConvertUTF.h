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
#ifndef CRIOMANAGER_CRCONVERTUTF
#define CRIOMANAGER_CRCONVERTUTF 1
#include <CRIOManager/crExport.h>

#include <string>

#ifdef __CYGWIN__
namespace std
{
typedef basic_string<wchar_t> wstring;
}
#endif

namespace CRIOManager
{

extern CRIOMANAGER_EXPORT std::string convertUTF16toUTF8(const wchar_t* source, unsigned sourceLength);
extern CRIOMANAGER_EXPORT std::wstring convertUTF8toUTF16(const char* source, unsigned sourceLength);

extern CRIOMANAGER_EXPORT std::string convertUTF16toUTF8(const std::wstring& s);
extern CRIOMANAGER_EXPORT std::string convertUTF16toUTF8(const wchar_t* s);

extern CRIOMANAGER_EXPORT std::wstring convertUTF8toUTF16(const std::string& s);
extern CRIOMANAGER_EXPORT std::wstring convertUTF8toUTF16(const char* s);

extern CRIOMANAGER_EXPORT std::string convertTCHARToWebUTF8(const std::wstring& s);

}

#endif
