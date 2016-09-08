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
#include <Plugin cre/crPagedLOD.h>
#include <Plugin cre/crNode.h>
#include <CRIOManager/crWriteFile.h>

using namespace cre;

void crPagedLOD::write(crDataOutputStream* out){
	// Write crLod's identification.
	out->writeInt(CREPAGEDLOD);
	// If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crNode* node = dynamic_cast<CRCore::crNode*>(this);
    if(node){
        static_cast<cre::crNode*>(node)->write(out);
    }
    else
		throw crException("crPagedLOD::write(): Could not cast this CRCore::crPagedLOD to an CRCore::crLod.");

    out->writeString(getDatabasePath());
    out->writeFloat(getRadius());
    out->writeUInt(getNumChildrenThatCannotBeExpired());

    unsigned int numChildrenToWriteOut = 0;

    int i;
    for(i=0; i<(int)getNumFileNames();++i)
    {
        if (getFileName(i).empty())
        {
            ++numChildrenToWriteOut;
        }
		else
		{//write pagelod file
			//CRCore::crNode* child = getChild(i);
			//if(child) CRIOManager::writeNodeFile(*child,getFileName(i));
		}
    }

// Write crGroup's properties.
    // Write number of children.
    out->writeInt(numChildrenToWriteOut);
    // Write children.
    for(i=0; i<(int)getNumChildren(); i++){
        if (getFileName(i).empty())
        {
            CRCore::crNode* child = getChild(i);
            out->writeBool(child!=NULL);
			if(child) 
			{
				out->writeNode(child);
			}
			else
			{
				CRCore::notify(CRCore::WARN)<<"crPagedLOD::write has emptyNode "<<this->getName()<<std::endl;
			}
        }
    }
        // crLod properties
	// Write centermode
	out->writeVec3(getCenter());
	out->writeInt(getCenterMode());
    out->writeInt(getRangeMode());
        // Write rangelist
	int size = getNumRanges();
	out->writeInt(size);
	for(i=0;i<size;i++){
		out->writeFloat(getMinRange(i));
		out->writeFloat(getMaxRange(i));
	}
    // crPagedLOD properties
	size = getNumFileNames();
	out->writeInt(size);
	for(i=0;i<size;i++){
		out->writeString(getFileName(i));
	}
}

void crPagedLOD::read(crDataInputStream* in){
	// Peek on crLod's identification.
	int id = in->peekInt();
	if(id == CREPAGEDLOD){
		// Read crLod's identification.
		id = in->readInt();
		// If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crNode* node = dynamic_cast<CRCore::crNode*>(this);
        if(node){
            ((cre::crNode*)(node))->read(in);
        }
        else
            throw crException("crGroup::read(): Could not cast this CRCore::crGroup to an CRCore::crNode.");

		setDatabasePath(in->readString());
        if (getDatabasePath().empty() && in->getOptions() && !in->getOptions()->getDatabasePathList().empty())
        {
            const std::string& path = in->getOptions()->getDatabasePathList().front();
            if (!path.empty()) 
            {
                setDatabasePath(path);
            }
        } 

        setRadius(in->readFloat());
        setNumChildrenThatCannotBeExpired(in->readUInt());

        // Read groups properties.
        // Read number of children.
        int size = in->readInt();
        // Read children.
		int i;
        for(i=0; i<size; i++)
        {
			if(in->readBool())
                addChild(in->readNode());
        }

		// Read centermode
		setCenter(in->readVec3());
		setCenterMode((CRCore::crLod::CenterMode)in->readInt());
		setRangeMode((RangeMode)in->readInt());

		// Read rangelist
		size = in->readInt();
		for(i=0;i<size;i++){
			float min = in->readFloat();
			float max = in->readFloat();
			setRange(i, min, max);
		}

		size = in->readInt();
		for(i=0;i<size;i++){
			setFileName(i, in->readString());
		}
	}
	else{
		throw crException("crLod::read(): Expected crLod identification.");
	}
}
