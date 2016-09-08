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

//
// OpenFlight?loader for OpenSceneGraph
//
//  Copyright (C) 2005-2007  Brede Johansen
//

#include <CRCore/crNotify.h>
#include <Plugin OpenFlight/Registry.h>

using namespace flt;

crRegistry::crRegistry()
{
}

crRegistry::~crRegistry()
{
}

crRegistry* crRegistry::instance()
{
    static crRegistry s_registry;
    return &s_registry;
}

void crRegistry::addPrototype(int opcode, Record* prototype)
{
    if (prototype==0L)
    {
        CRCore::notify(CRCore::WARN) << "Not a record." << std::endl;
        return;
    }

    if (_recordProtoMap.find(opcode) != _recordProtoMap.end())
        CRCore::notify(CRCore::WARN) << "crRegistry already contains prototype for opcode " << opcode << "." << std::endl;

    _recordProtoMap[opcode] = prototype;
}

Record* crRegistry::getPrototype(int opcode)
{
    RecordProtoMap::iterator itr = _recordProtoMap.find(opcode);
    if (itr != _recordProtoMap.end())
        return (*itr).second.get();

    return NULL;
}
