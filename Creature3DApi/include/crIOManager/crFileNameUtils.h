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
#ifndef CRIOMANAGER_FILENAMEUTILS
#define CRIOMANAGER_FILENAMEUTILS 1

#include <CRIOManager/crExport.h>

#include <string>

namespace CRIOManager {

extern CRIOMANAGER_EXPORT std::string getFilePath(const std::string& filename);
extern CRIOMANAGER_EXPORT std::string getFileExtension(const std::string& filename);
extern CRIOMANAGER_EXPORT std::string getLowerCaseFileExtension(const std::string& filename);
extern CRIOMANAGER_EXPORT std::string getSimpleFileName(const std::string& fileName);
extern CRIOMANAGER_EXPORT std::string getNameLessExtension(const std::string& fileName);
extern CRIOMANAGER_EXPORT std::string getStrippedName(const std::string& fileName);


extern CRIOMANAGER_EXPORT std::string convertFileNameToWindowsStyle(const std::string& fileName);
extern CRIOMANAGER_EXPORT std::string convertFileNameToUnixStyle(const std::string& fileName);

extern CRIOMANAGER_EXPORT bool isFileNameNativeStyle(const std::string& fileName);
extern CRIOMANAGER_EXPORT std::string convertFileNameToNativeStyle(const std::string& fileName);

extern CRIOMANAGER_EXPORT bool equalCaseInsensitive(const std::string& lhs,const std::string& rhs);
extern CRIOMANAGER_EXPORT bool equalCaseInsensitive(const std::string& lhs,const char* rhs);

extern CRIOMANAGER_EXPORT bool containsServerAddress(const std::string& filename);
extern CRIOMANAGER_EXPORT std::string getServerAddress(const std::string& filename);
extern CRIOMANAGER_EXPORT std::string getServerFileName(const std::string& filename);

}

#endif
