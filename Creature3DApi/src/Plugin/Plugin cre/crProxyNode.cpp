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

#include <CRIOManager/crWriteFile.h>
#include <CRIOManager/crReadFile.h>
#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crFileNameUtils.h>

#include <Plugin cre/crException.h>
#include <Plugin cre/crProxyNode.h>
#include <Plugin cre/crGroup.h>

using namespace cre;
/*
for(CRIOManager::FilePathList::iterator itr=CRIOManager::getDataFilePathList().begin(); itr!=CRIOManager::getDataFilePathList().end(); ++itr)
printf("#1######%s\n", itr->c_str());
for(CRIOManager::FilePathList::const_iterator itrO=in->getOptions()->getDatabasePathList().begin(); itrO!=in->getOptions()->getDatabasePathList().end(); ++itrO)
printf("#2######%s\n", itrO->c_str());
namespace CRIOManager {
class PushAndPopDataPath
{
    public:
        PushAndPopDataPath(const std::string& path)
        {
            getDataFilePathList().push_front(path);
        }

        ~PushAndPopDataPath()
        {
            getDataFilePathList().pop_front();
        }
};
}
*/

void crProxyNode::write(crDataOutputStream* out)
{
    // Write crProxyNode's identification.
    out->writeInt(CREPROXYNODE);

    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crGroup* node = dynamic_cast<CRCore::crGroup*>(this);
    if(node)
    {
        static_cast<cre::crGroup*>(node)->write(out);
    }
    else
        throw crException("crProxyNode::write(): Could not cast this CRCore::crProxyNode to an CRCore::crGroup.");

    out->writeFloat(getRadius());
    out->writeInt(getCenterMode());
    out->writeVec3(getCenter());

    out->writeUInt(getNumFileNames());
    unsigned int numChildrenToWriteOut = 0;
    unsigned int i;
    for(i=0; i<getNumFileNames(); i++)
    {
        if (getFileName(i).empty())
        {
            out->writeString("");
            ++numChildrenToWriteOut;
        }
        else
        {
            if(out->getUseOriginalExternalReferences())
            {
                out->writeString(getFileName(i));
            }
            else
            {
                std::string ivename = CRIOManager::getFilePath(getFileName(i)) +"/"+ CRIOManager::getStrippedName(getFileName(i)) +".cre";
                out->writeString(ivename);
            }
        }
    }

    if(out->getIncludeExternalReferences()) //--------- inlined mode
    {
        out->writeUInt(getNumChildren());
        for(i=0; i<getNumChildren(); i++)
        {
            out->writeNode(getChild(i));
        }
    }
    else //----------------------------------------- no inlined mode
    {
        out->writeUInt(numChildrenToWriteOut);

        for(i=0; i<getNumFileNames(); i++)
        {
            if (getFileName(i).empty())
            {
                out->writeNode(getChild(i));
            }
            else if(out->getWriteExternalReferenceFiles())
            {
                if(out->getUseOriginalExternalReferences())
                {
                    CRIOManager::writeNodeFile(*getChild(i), getFileName(i));
                }
                else
                {
                    std::string ivename = CRIOManager::getFilePath(getFileName(i)) +"/"+ CRIOManager::getStrippedName(getFileName(i)) +".cre";
                    CRIOManager::writeNodeFile(*getChild(i), ivename);
                }
            }
        }
    }
}

void crProxyNode::read(crDataInputStream* in)
{
    // Peek on crProxyNode's identification.
    int id = in->peekInt();
    if(id == CREPROXYNODE)
    {
        // Read crProxyNode's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crGroup* node = dynamic_cast<CRCore::crGroup*>(this);
        if(node)
        {
            ((cre::crGroup*)(node))->read(in);
        }
        else
            throw crException("crProxyNode::read(): Could not cast this CRCore::crProxyNode to an CRCore::crGroup.");

        if (in->getOptions() && !in->getOptions()->getDatabasePathList().empty())
        {
            const std::string& path = in->getOptions()->getDatabasePathList().front();
            if (!path.empty()) 
            {
                setDatabasePath(path);
            }
        }

        setRadius(in->readFloat());
        setCenterMode((CRCore::crProxyNode::CenterMode)in->readInt());
        setCenter(in->readVec3());

        unsigned int numFileNames = in->readUInt();
        unsigned int i;
        for(i=0; i<numFileNames; i++)
        {
            setFileName(i, in->readString());
        }

        unsigned int numChildren = in->readUInt();
        for(i=0; i<numChildren; i++)
        {
            CRIOManager::FilePathList& fpl = ((CRIOManager::crReaderWriter::Options*)in->getOptions())->getDatabasePathList();
            fpl.push_front( fpl.empty() ? CRIOManager::getFilePath(getFileName(i)) : fpl.front()+'/'+ CRIOManager::getFilePath(getFileName(i)));
            addChild(in->readNode());            
            fpl.pop_front();
        }

        if( in->getLoadExternalReferenceFiles() )
        {
            for(i=0; i<numFileNames; i++)
            {
                if(i>=numChildren && !getFileName(i).empty())
                {
                    CRIOManager::FilePathList& fpl = ((CRIOManager::crReaderWriter::Options*)in->getOptions())->getDatabasePathList();
                    fpl.push_front( fpl.empty() ? CRIOManager::getFilePath(getFileName(i)) : fpl.front()+'/'+ CRIOManager::getFilePath(getFileName(i)));
                    CRCore::crNode *node = CRIOManager::readNodeFile(getFileName(i), in->getOptions());
                    fpl.pop_front();

                    if(node)
                    {
                        insertChild(i, node);
                    }
                }
            }
        }
    }
    else
    {
        throw crException("crProxyNode::read(): Expected crProxyNode identification.");
    }
}

void crExternNode::write(crDataOutputStream* out)
{
	out->writeInt(CREEXTERNNODE);

	CRCore::crGroup* node = dynamic_cast<CRCore::crGroup*>(this);
	if(node)
	{
		static_cast<cre::crGroup*>(node)->write(out);
	}
	else
		throw crException("crExternNode::write(): Could not cast this CRCore::crExternNode to an CRCore::crGroup.");

    out->writeString(m_filename);
}

void crExternNode::read(crDataInputStream* in)
{
	int id = in->peekInt();
	if(id == CREEXTERNNODE)
	{
		id = in->readInt();

		CRCore::crGroup* node = dynamic_cast<CRCore::crGroup*>(this);
		if(node)
		{
			((cre::crGroup*)(node))->read(in);
		}
		else
			throw crException("crExternNode::read(): Could not cast this CRCore::crExternNode to an CRCore::crGroup.");

         m_filename = in->readString();
	}
	else
	{
		throw crException("crExternNode::read(): Expected crExternNode identification.");
	}
}

