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
#include <Plugin cre/crMultiSwitch.h>
#include <Plugin cre/crGroup.h>

using namespace cre;

void crMultiSwitch::write(crDataOutputStream* out){
	// Write crSwitch's identification.
	out->writeInt(CREMULTISWITCH);
	// If the CRCore class is inherited by any other class we should also write this to file.
	CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
	if(group){
		((cre::crGroup*)(group))->write(out);
	}
	else
		throw crException("crSwitch::write(): Could not cast this CRCore::crSwitch to an CRCore::crGroup.");
	// Write crSwitch's properties.

        out->writeBool(getNewChildDefaultValue());
        out->writeUInt(getActiveSwitchSet());

        const CRCore::crMultiSwitch::SwitchSetList& switchset = getSwitchSetList();
        out->writeUInt(switchset.size());
        for(unsigned int s=0;s<switchset.size();++s)
        {
	    // Write childrens value.
	    for(unsigned int i=0; i<getNumChildren();i++)
            out->writeBool(getValue(s,i));
        }
}

void crMultiSwitch::read(crDataInputStream* in){
	// Peek on crSwitch's identification.
	int id = in->peekInt();
	if(id == CREMULTISWITCH){
		// Read crSwitch's identification.
		id = in->readInt();
		// If the CRCore class is inherited by any other class we should also read this from file.
		CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
		if(group){
			((cre::crGroup*)(group))->read(in);
		}
		else
			throw crException("crSwitch::read(): Could not cast this CRCore::crSwitch to an CRCore::crGroup.");
		// Read crSwitch's properties

                setNewChildDefaultValue(in->readBool());
                setActiveSwitchSet(in->readUInt());

                unsigned int numSwitchSets = in->readUInt();
                
                for(unsigned int s=0;s<numSwitchSets;++s)
                {
                    // Read childrens value.
		    for(unsigned int i=0; i<getNumChildren();i++)
			    setValue(s, i, in->readBool());
                }
	}
	else{
		throw crException("crSwitch::read(): Expected crSwitch identification.");
	}
}
