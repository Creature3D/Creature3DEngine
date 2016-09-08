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
#include <CRCore/crArgumentParser.h>
#include <CRIOManager/crFileNameUtils.h>
using namespace cre;

void crGroup::write(crDataOutputStream* out){
    // Write crGroup's identification.
    out->writeInt(CREGROUP);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crNode* node = dynamic_cast<CRCore::crNode*>(this);
    if(node){
        static_cast<cre::crNode*>(node)->write(out);
    }
    else
        throw crException("crGroup::write(): Could not cast this CRCore::crGroup to an CRCore::crNode.");

    // Write crGroup's properties.
	bool push_data = false;
	std::string des = getDescriptionsInString();
	if(!des.empty())
	{
		std::string str;
		if(CRCore::crArgumentParser::readKeyValue(des,"DataFile",str))
		{
			str = CRIOManager::convertFileNameToUnixStyle(str);
			if(str.find("./") == 0||str.find(".\\") == 0)
			{
				str.erase(0,2);
			}
			out->pushDataFile(str);
			push_data = true;
		}
	}

    // Write number of children.
    out->writeInt(getNumChildren());
    // Write children.
    for(unsigned int i=0; i<getNumChildren(); i++){
        CRCore::crNode* child = getChild(i);
        out->writeNode(child);
    }
	out->writeUInt(m_attributeMask);

	if(push_data)
		out->popDataFile();
}

void crGroup::read(crDataInputStream* in){
    // Read crGroup's identification.
    int id = in->peekInt();
    if(id == CREGROUP){
        // Read crGroup's identification.
        id = in->readInt();

        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crNode* node = dynamic_cast<CRCore::crNode*>(this);
        if(node){
            ((cre::crNode*)(node))->read(in);
        }
        else
            throw crException("crGroup::read(): Could not cast this CRCore::crGroup to an CRCore::crNode.");

        // Read groups properties.

        // Read number of children.
        int size = in->readInt();
        // Read children.
		if ( in->getVersion() < VERSION_0027 && getName().compare("HDR") == 0 )
		{
			crNode *child;
			for(int i=0; i<size; i++)
			{
				child = in->readNode();
				if(!child)
					continue;
				if(child->getName().compare("CalcShadow") == 0)
				{
					child->setName("Static");
					std::string command = child->getDescriptionsInString();
					CRCore::crArgumentParser::updateOrAddKeyValue(command,"[NameID]","Static");
					child->setDescriptionsInString(command);
					addChild(child);
					CRCore::crGroup *dynamicGroup = new CRCore::crGroup;
					dynamicGroup->setName("Dynamic");
					command = "[NameID] Dynamic";
					dynamicGroup->setDescriptionsInString(command);
					addChild(dynamicGroup);
				}
				else
				{
					addChild(child);
				}
			}
		}
		else
		{
			for(int i=0; i<size; i++)
			{
				addChild(in->readNode());
			}
		}
		m_attributeMask = in->readUInt();
    }
    else{
        throw crException("crGroup::read(): Expected crGroup identification");
    }
}
