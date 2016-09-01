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
#include <CRCore/crEndian.h>

#include <CRIOManager/crRegistry.h>
#include <CRIOManager/crFileNameUtils.h>
#include <CRIOManager/crArchive.h>

#include <streambuf>

using namespace CRIOManager;

CRIOManager::crArchive* CRIOManager::openArchive(const std::string& filename, crArchive::ArchiveStatus status, unsigned int indexBlockSizeHint)
{
    return openArchive(filename, status, indexBlockSizeHint, crRegistry::instance()->getOptions());
}

CRIOManager::crArchive* CRIOManager::openArchive(const std::string& filename, crArchive::ArchiveStatus status, unsigned int indexBlockSizeHint,crReaderWriter::Options* options)
{
    crReaderWriter::ReadResult result = CRIOManager::crRegistry::instance()->openArchive(filename, status, indexBlockSizeHint, options);
    return result.takeArchive();
}

crArchive::crArchive()
{
    //CRCore::notify(CRCore::INFO)<<"crArchive::crArchive() open"<<std::endl;
}

crArchive::~crArchive()
{
    //CRCore::notify(CRCore::INFO)<<"crArchive::~crArchive() closed"<<std::endl;
}

