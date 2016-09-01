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
#include <CRCore/crBase.h>

#include <CRIOManager/crRegistry.h>
#include <CRIOManager/crReadFile.h>
#include <CRIOManager/crInput.h>

using namespace CRIOManager;

crInput::crInput()
{
}


crInput::~crInput()
{
}


CRCore::crBase* crInput::getObjectForUniqueID(const std::string& uniqueID)
{
    UniqueIDToObjectMapping::iterator fitr = m_uniqueIDToObjectMap.find(uniqueID);
    if (fitr != m_uniqueIDToObjectMap.end()) return (*fitr).second.get();
    else return NULL;
}


void crInput::regisiterUniqueIDForObject(const std::string& uniqueID,CRCore::crBase* obj)
{
    m_uniqueIDToObjectMap[uniqueID] = obj;
}


CRCore::crBase* crInput::readObjectOfType(const CRCore::crBase& compObj)
{
    return crRegistry::instance()->readObjectOfType(compObj,*this);
}

CRCore::crBase* crInput::readObjectOfType(const basic_type_wrapper &btw)
{
    return crRegistry::instance()->readObjectOfType(btw,*this);
}

CRCore::crBase* crInput::readObject()
{
    return crRegistry::instance()->readObject(*this);
}


CRCore::crImage*  crInput::readImage()
{
    return crRegistry::instance()->readImage(*this);
}

CRCore::crDrawable* crInput::readDrawable()
{
    return crRegistry::instance()->readDrawable(*this);
}

CRCore::crStateAttribute* crInput::readStateAttribute()
{
    return crRegistry::instance()->readStateAttribute(*this);
}

CRCore::crUniform* crInput::readUniform()
{
    return crRegistry::instance()->readUniform(*this);
}

CRCore::crNode* crInput::readNode()
{
    return crRegistry::instance()->readNode(*this);
}

CRCore::crBase* crInput::readObject(const std::string& fileName)
{
    return readObjectFile(fileName,m_options.get());
}

CRCore::crImage*  crInput::readImage(const std::string& fileName)
{
    return readImageFile(fileName,m_options.get());
}

CRCore::crNode* crInput::readNode(const std::string& fileName)
{
    return readNodeFile(fileName,m_options.get());
}
