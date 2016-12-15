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
#include <CRIOManager/crConvertUTF.h>
#include <CRCore/crNotify.h>

#include <string.h>
#include <wchar.h>

#if defined(WIN32) && !defined(__CYGWIN__)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#define CP_GB2312 936
#define CP_BIG5 950
namespace CRIOManager
{

std::string convertUTF16toUTF8(const std::wstring& s){return convertUTF16toUTF8(s.c_str(), s.length());}
std::string convertUTF16toUTF8(const wchar_t* s){return convertUTF16toUTF8(s, wcslen(s));}

std::wstring convertUTF8toUTF16(const std::string& s){return convertUTF8toUTF16(s.c_str(), s.length());}
std::wstring convertUTF8toUTF16(const char* s){return convertUTF8toUTF16(s, strlen(s));}

std::string convertUTF16toUTF8(const wchar_t* source, unsigned sourceLength)
{
#if defined(WIN32) && !defined(__CYGWIN__)
    if (sourceLength == 0)
    {
        return std::string();
    }

	int destLen = WideCharToMultiByte(/*CP_ACP*/CP_GB2312, 0, source, sourceLength, 0, 0, 0, 0);
    if (destLen <= 0)
    {
        CRCore::notify(CRCore::WARN) << "Cannot convert UTF-16 string to UTF-8." << std::endl;
        return std::string();
    }

    std::string sDest(destLen, '\0');
	//sDest.resize(destLen+1,0);
	destLen = WideCharToMultiByte(/*CP_ACP*/CP_GB2312, 0, source, sourceLength, &sDest[0], destLen, 0, 0);

    if (destLen <= 0)
    {
        CRCore::notify(CRCore::WARN) << "Cannot convert UTF-16 string to UTF-8." << std::endl;
        return std::string();
    }

    return sDest;
#else
    //TODO: Implement for other platforms
    CRCore::notify(CRCore::WARN) << "ConvertUTF16toUTF8 not implemented." << std::endl;
    return std::string();
#endif
}

std::wstring convertUTF8toUTF16(const char* source, unsigned sourceLength)
{
#if defined(WIN32) && !defined(__CYGWIN__)
    if (sourceLength == 0)
    {
        return std::wstring();
    }

	int destLen = MultiByteToWideChar(/*CP_ACP*/CP_GB2312, 0, source, sourceLength, 0, 0);
    if (destLen <= 0)
    {
        CRCore::notify(CRCore::WARN) << "Cannot convert UTF-8 string to UTF-16." << std::endl;
        return std::wstring();
    }

    std::wstring sDest(destLen, L'\0');
	//sDest.resize(destLen+1,0);
	destLen = MultiByteToWideChar(/*CP_ACP*/CP_GB2312, 0, source, sourceLength, &sDest[0], destLen);

    if (destLen <= 0)
    {
        CRCore::notify(CRCore::WARN) << "Cannot convert UTF-8 string to UTF-16." << std::endl;
        return std::wstring();
    }
    return sDest;
#else
    //TODO: Implement for other platforms
    CRCore::notify(CRCore::WARN) << "ConvertUTF8toUTF16 not implemented." << std::endl;
    return std::wstring();
#endif
}

}

