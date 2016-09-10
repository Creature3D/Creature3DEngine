/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/
#include <Plugin cre/crException.h>
#include <Plugin cre/crClusterCullingCallback.h>
#include <Plugin cre/crBase.h>

using namespace cre;

void crClusterCullingCallback::write(crDataOutputStream* out)
{
    // Write crClusterCullingCallback's identification.
    out->writeInt(CRECLUSTERCULLINGCALLBACK);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
    if(obj)
    {
        ((cre::crBase*)(obj))->write(out);
    }
    else
        throw crException("crClusterCullingCallback::write(): Could not cast this CRCore::crClusterCullingCallback to an CRCore::crBase.");
    // Write crClusterCullingCallback's properties.

    out->writeVec3(m_controlPoint);
    out->writeVec3(m_normal);
    out->writeFloat(m_radius);
    out->writeFloat(m_deviation);
}

void crClusterCullingCallback::read(crDataInputStream* in)
{
    // Peek on crClusterCullingCallback's identification.
    int id = in->peekInt();
    if(id == CRECLUSTERCULLINGCALLBACK)
    {
        // Read crClusterCullingCallback's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
        if(obj)
        {
            ((cre::crBase*)(obj))->read(in);
        }
        else
            throw crException("crClusterCullingCallback::read(): Could not cast this CRCore::crClusterCullingCallback to an CRCore::crBase.");
    
        // Read crClusterCullingCallback's properties
        
        m_controlPoint = in->readVec3();
        m_normal = in->readVec3();
        m_radius = in->readFloat();
        m_deviation = in->readFloat();
        
    }
    else
    {
        throw crException("crClusterCullingCallback::read(): Expected crClusterCullingCallback identification.");
    }
}
