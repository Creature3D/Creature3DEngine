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
#include <CRCore/crBase.h>
#include <CRCore/crImage.h>
#include <CRCore/crNode.h>
#include <CRCore/crGroup.h>
#include <CRCore/crObject.h>

#include <CRIOManager/crRegistry.h>
#include <CRIOManager/crWriteFile.h>

using namespace CRCore;
using namespace CRIOManager;

bool CRIOManager::writeObjectFile(const crBase& object,const std::string& filename)
{
    crReaderWriter::WriteResult wr = crRegistry::instance()->writeObject(object,filename);
    if (wr.error()) notify(WARN) << wr.message() << std::endl;
    return wr.success();
}


bool CRIOManager::writeImageFile(const crImage& image,const std::string& filename)
{
    crReaderWriter::WriteResult wr = crRegistry::instance()->writeImage(image,filename);
    if (wr.error()) notify(WARN) << wr.message() << std::endl;
    return wr.success();
}


bool CRIOManager::writeHeightFieldFile(const crHeightField& crHeightField,const std::string& filename)
{
    crReaderWriter::WriteResult wr = crRegistry::instance()->writeHeightField(crHeightField,filename);
    if (wr.error()) notify(WARN) << wr.message() << std::endl;
    return wr.success();
}

bool CRIOManager::writeNodeFile(const crNode& node,const std::string& filename)
{
    crReaderWriter::WriteResult wr = crRegistry::instance()->writeNode(node,filename);
    if (wr.error()) notify(WARN) << wr.message() << std::endl;
    return wr.success();
}
