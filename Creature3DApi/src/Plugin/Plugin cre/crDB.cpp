/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
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
#include <Plugin cre/crNode.h>
#include <Plugin cre/crGroup.h>
#include <Plugin cre/crMatrixTransform.h>
#include <Plugin cre/crObject.h>
#include <Plugin cre/crLightSource.h>
#include <Plugin cre/crBillboard.h>
#include <Plugin cre/crSequence.h>
#include <Plugin cre/crLod.h>
#include <Plugin cre/crPositionAttitudeTransform.h>
#include <Plugin cre/crTransform.h>
#include <Plugin cre/crDB.h>
#include <Plugin cre/crBase.h>
using namespace cre;

void crDB::write(crDataOutputStream* out){
    // Write crGroup's identification.
    out->writeInt(CREDB);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crGroup* node = dynamic_cast<CRCore::crGroup*>(this);
    if(node){
        static_cast<cre::crGroup*>(node)->write(out);
    }
    else
        throw crException("crDB::write(): Could not cast this CRCore::crDB to an CRCore::crGroup.");

	out->writeInt(m_maxiterations);
	out->writeVec3(m_gravity);
	out->writePlane(m_groundPlane);
}

void crDB::read(crDataInputStream* in){
    // Read crGroup's identification.
    int id = in->peekInt();
    if(id == CREDB){
        // Read crGroup's identification.
        id = in->readInt();

        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crGroup* node = dynamic_cast<CRCore::crGroup*>(this);
        if(node){
            ((cre::crGroup*)(node))->read(in);
        }
        else
            throw crException("crDB::read(): Could not cast this CRCore::crDB to an CRCore::crGroup.");

		m_maxiterations = in->readInt();
		m_gravity = in->readVec3();
		m_groundPlane = in->readPlane();

    }
    else{
        throw crException("crGroup::read(): Expected crGroup identification");
    }
}
