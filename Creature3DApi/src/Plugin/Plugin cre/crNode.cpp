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
#include <Plugin cre/crMatrixTransform.h>
#include <Plugin cre/crGroup.h>
#include <Plugin cre/crBase.h>
#include <Plugin cre/crStateSet.h>
#include <Plugin cre/crAnimationPathCallback.h>
#include <Plugin cre/crClusterCullingCallback.h>
#include <CRIOManager/crFileNameUtils.h>
#include <CRIOManager/crFileUtils.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crNodeCallback.h>
#include <CRIOManager/crCookFile.h>
using namespace cre;


void crNode::write(crDataOutputStream* out){

    // Write node identification.
    out->writeInt(CRENODE);

    // Write out any inherited classes.
    CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
    if(obj){
        ((cre::crBase*)(obj))->write(out);
    }
    else
        throw crException("crNode::write(): Could not cast this CRCore::crNode to an CRCore::crBase.");

	std::string des = getDescriptionsInString();
	if(!des.empty())
	{
		bool changed = false;
		std::string matFilePath,matFilePath2;
		std::string writeFileName = out->getWriteFileName();
		if(writeFileName.compare("tmp.cre") != 0)
		{
			std::string datafile;
			out->getCurrentDataFile(datafile);
			if(CRIOManager::equalCaseInsensitive(CRCore::crArgumentParser::getFileExtension(writeFileName),"crs"))
			{//写场景
				matFilePath = CRCore::crArgumentParser::getFileNameEliminateExt(writeFileName) + ".mat";
				matFilePath2 = matFilePath+"$";
				if(!datafile.empty())
				{
				    matFilePath = matFilePath +"/"+datafile;
					matFilePath2 = matFilePath2 +"/"+datafile;
				}
			}
			else
			{
				matFilePath = CRCore::crArgumentParser::getFileNameEliminateExt(writeFileName) + ".mat";
				matFilePath2 = matFilePath+"$";
			}

			char buf[16];
			std::string cfgFile;
			CRCore::crNodeCallback *callback;
			for(int i = 0; i<10; i++)
			{
				sprintf(buf,"cfgFile%d\0",i);
				if(CRCore::crArgumentParser::readKeyValue(des,buf,cfgFile))
				{//将该cfgFile拷贝到模型目录下
					//if(CRIOManager::getFilePath(cfgFile).compare(matFilePath) != 0)
					//{
						std::string newfilename = matFilePath+"/"+CRIOManager::getSimpleFileName(cfgFile);
						std::string newfilename2 = matFilePath2+"/"+CRIOManager::getSimpleFileName(cfgFile);

						std::string cookFile = cfgFile;
						CRIOManager::GetCookedFileName(cookFile);
						std::string cookNewFile = newfilename2;
						CRIOManager::GetCookedFileName(cookNewFile);
						CRIOManager::copyFile(cookFile,cookNewFile);

						CRCore::crArgumentParser::updateOrAddKeyValue(des,buf,newfilename);
						callback = this->getUpdateCallback();
                        while(callback)
						{
							if(callback->getCfgFile().compare(cfgFile) == 0)
							{
								callback->setCfgFile(newfilename);
							}
							callback = callback->getNestedCallback();
						}
						callback = this->getCullCallback();
						while(callback)
						{
							if(callback->getCfgFile().compare(cfgFile) == 0)
							{
								callback->setCfgFile(newfilename);
							}
							callback = callback->getNestedCallback();
						}
						//out->addFiles(newfilename);
						changed = true;
					//}
				}
			}
			if(CRCore::crArgumentParser::readKeyValue(des,"EventScript",cfgFile))
			{//将该cfgFile拷贝到模型目录下
				//out->addFiles(cfgFile);
				//if(CRIOManager::getFilePath(cfgFile).compare(matFilePath) != 0)
				//{
				//	std::string newfilename = matFilePath+"/"+CRIOManager::getSimpleFileName(cfgFile);
				//	CRIOManager::copyFile(cfgFile,newfilename);
				//	CRCore::crArgumentParser::updateOrAddKeyValue(des,"EventScript",newfilename);
				//	changed = true;
				//}
			}
			if(changed)
			{
				setDescriptionsInString(des);
			}
		}
	}
    // Write CRCore::node properties.

    // Write Name
	int i = m_name.find("$");
	if(i != std::string::npos)
		m_name = m_name.substr(0,i);
    out->writeString(getName());
    // Write culling active
    out->writeBool( getCullingActive());
    // Write Descriptions
    int nDesc =  getDescriptions().size();
    out->writeInt(nDesc);
    if(nDesc!=0){
        std::vector<std::string> desc =  getDescriptions();
        for(int i=0;i<nDesc;i++)
            out->writeString(desc[i]); 
    }
    // 从脚本解析
	//out->writeBool( getStateSet()!=0);
	//if(getStateSet())
	//	out->writeStateSet(getStateSet());

    // Write UpdateCallback if any
    //CRCore::crAnimationPathCallback* nc = dynamic_cast<CRCore::crAnimationPathCallback*>(getUpdateCallback());
    //out->writeBool(nc!=0);
    //if(nc)
    //    {
    //    ((cre::crAnimationPathCallback*)(nc))->write(out);
    //}
    
    //if (out->getVersion() >= VERSION_0006)
    //{
    //    CRCore::crClusterCullingCallback* ccc = dynamic_cast<CRCore::crClusterCullingCallback*>(getCullCallback());
    //    out->writeBool(ccc!=0);
    //    if(ccc)
    //    {
    //        ((cre::crClusterCullingCallback*)(ccc))->write(out);
    //    }
    //}

    //if (out->getVersion() >= VERSION_0010)
    //{
    //    const CRCore::crBoundingSphere& bs = getInitialBound();
    //    out->writeBool(bs.valid());
    //    if (bs.valid())
    //    {
    //        out->writeVec3(bs.center());
    //        out->writeFloat(bs.radius());
    //    }
    //}

    // Write NodeMask
    out->writeUInt(getNodeMask());

	out->writeBool(m_visiable);
    out->writeBool(m_renderable);
	out->writeBool(m_enableIntersect);//getEnableIntersect()
    out->writeInt(m_collideMode);
	
	out->writeBool(m_isCalcShadow);
	if (out->getVersion() >= VERSION_0029)
	{
		out->writeBool(m_avoidIntersect);
	}
	//out->writeBool(isShowBoundingBox());
}


void crNode::read(crDataInputStream* in){
    // Peak on the identification id.
    int id = in->peekInt();

    if(id == CRENODE){
        id = in->readInt();
        CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
        if(obj){
            ((cre::crBase*)(obj))->read(in);
        }
        else
            throw crException("crNode::read(): Could not cast this CRCore::crNode to an CRCore::crBase.");
        // Read name
        setName(in->readString());
        // Read Culling active
        setCullingActive(in->readBool());
        // Read descriptions
        int nDesc = in->readInt();
        if(nDesc!=0){
            for(int i=0;i<nDesc;i++)
                 addDescription(in->readString());
        }
        // 从脚本解析
		//if(in->readBool())
		//{
		//	setStateSet(in->readStateSet());
		//}

        //// Read UpdateCallback if any
        //if(in->readBool())
        //{
        //    CRCore::crAnimationPathCallback* nc = new CRCore::crAnimationPathCallback();
        //    ((cre::crAnimationPathCallback*)(nc))->read(in);
        //    setUpdateCallback(nc);
        //}

        //if (in->getVersion() >= VERSION_0006)
        //{
        //    if(in->readBool())
        //    {
        //        CRCore::crClusterCullingCallback* ccc = new CRCore::crClusterCullingCallback();
        //        ((cre::crClusterCullingCallback*)(ccc))->read(in);
        //        setCullCallback(ccc);
        //    }
        //}
        
        //if (in->getVersion() >= VERSION_0010)
        //{
        //    if (in->readBool())
        //    {
        //        CRCore::crBoundingSphere bs;
        //        bs.center() = in->readVec3();
        //        bs.radius() = in->readFloat();
        //        setInitialBound(bs);
        //    }
        //}

        // Read NodeMask
        setNodeMask(in->readUInt());

		setVisiable(in->readBool());
		setRenderable(in->readBool());
        setEnableIntersect(in->readBool());
		setCollideMode((CollideMode)(in->readInt()));

		setCalcShadow(in->readBool());
		if (in->getVersion() >= VERSION_0029)
		{
			setAvoidIntersect(in->readBool());
		}
		//showBoundingBox(in->readBool());

    }
    else{
        throw crException("crNode::read(): Expected crNode identification");
    }
}
