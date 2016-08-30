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

#ifndef CRIOMANAGER_WRITEFILE
#define CRIOMANAGER_WRITEFILE 1

#include <CRCore/crImage.h>
#include <CRCore/crShape.h>
#include <CRCore/crNode.h>

#include <CRIOManager/crExport.h>

#include <string>

namespace CRIOManager {

/** Write an CRCore::crBase to file. 
  * Return true on success,
  * return false on failure.
  * The CRIOManager::crRegistry is used to load the appropriate crReaderWriter plugin
  * for the filename extension, and this plugin then handles the request
  * to write the specified file.*/

extern CRIOMANAGER_EXPORT bool writeObjectFile(const CRCore::crBase& object, const std::string& filename);

/** Write an CRCore::crImage to file. 
  * Return true on success,
  * return false on failure.
  * The CRIOManager::crRegistry is used to load the appropriate crReaderWriter plugin
  * for the filename extension, and this plugin then handles the request
  * to write the specified file.*/
extern CRIOMANAGER_EXPORT bool writeImageFile(const CRCore::crImage& image, const std::string& filename);

/** Write an CRCore::crHeightField to file. 
  * Return true on success,
  * return false on failure.
  * The CRIOManager::crRegistry is used to load the appropriate crReaderWriter plugin
  * for the filename extension, and this plugin then handles the request
  * to write the specified file.*/
extern CRIOMANAGER_EXPORT bool writeHeightFieldFile(const CRCore::crHeightField& hf, const std::string& filename);

/** Write an CRCore::crNode to file. 
  * Return true on success,
  * return false on failure.
  * The CRIOManager::crRegistry is used to load the appropriate crReaderWriter plugin
  * for the filename extension, and this plugin then handles the request
  * to write the specified file.*/
extern CRIOMANAGER_EXPORT bool writeNodeFile(const CRCore::crNode& node, const std::string& filename);


}

#endif
