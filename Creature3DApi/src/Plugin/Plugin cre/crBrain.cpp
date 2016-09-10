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
//#include <Plugin cre/crResource.h>
#include <Plugin cre/crDB.h>
#include <Plugin cre/crGroup.h>
#include <Plugin cre/crBrain.h>
#include <Plugin cre/crBase.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crTraverseStringNode.h>
#include <CRCore/crPagedLOD.h>
#include <CRIOManager/crWriteFile.h>
#include <CRIOManager/crLoadManager.h>
using namespace cre;

/////////////////////////////////////////
//
//crCollectObjectLightVisitor
//
/////////////////////////////////////////
class crCollectObjectLightVisitor : public CRCore::crNodeVisitor
{
public:
	crCollectObjectLightVisitor():
	  CRCore::crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN),
	  m_i(0){}

	  virtual ~crCollectObjectLightVisitor(){}
	  virtual void apply(CRCore::crPagedLOD &node)
	  {
		  //fullload
		    const CRCore::crLod::RangeList& rangeList = node.getRangeList();
			int numChild = node.getNumChildren();
			int i;
			for( i = 0; i<rangeList.size(); ++i )
			{
				if(i>=numChild)
				{
					CRIOManager::crLoadManager::getInstance()->requestNode(node.getFileName(i),&node,false);
				}
			}
			m_plodObjectNameChanged = false;
			traverse(node);
			if(m_plodObjectNameChanged)
			{
				//其下的Object可能有被改名。
				for(i=0; i<(int)node.getNumFileNames();++i)
				{
					if (!node.getFileName(i).empty())
					{//write pagelod file
						CRCore::crNode* child = node.getChild(i);
						if(child) CRIOManager::writeNodeFile(*child,node.getFileName(i));
					}
				}
			}
	  }
	  virtual void apply(CRCore::crTraverseStringNode &node)
	  {
		  std::string str = m_trasvelstr;
		  m_trasvelstr = node.getName();
		  traverse(node);
		  m_trasvelstr = str;
	  }
	  virtual void apply(CRCore::crObject& node)
	  {
		  //static i = 0;
		  if(m_trasvelstr.compare("PhoneLight") == 0)
		  {
			ObjectSet::iterator itr = m_objectSet.find(&node);
			if(itr == m_objectSet.end())
			{
				m_objectSet.insert(&node);
				std::string objName = node.getName()/*+"$"*/;
				int i = objName.find("$");
				if(i != std::string::npos)
					objName = objName.substr(0,i);
				CRCore::crObject::LightNameSet &avoidLightSet = node.getAvoidLightSet();
				CRCore::crObject::LightNameSet &needLightSet = node.getNeedLightSet();
				if(!avoidLightSet.empty()||!needLightSet.empty())
				{
					std::string newObjName;
					if(objName.empty()) 
					{
						newObjName = CRCore::crArgumentParser::appItoa(m_i++);
						m_plodObjectNameChanged = true;
					}
					CRCore::crBrain *brain = CRCore::crBrain::getInstance();
					newObjName = objName+newObjName;
					while(brain->findObjectLight(newObjName) != NULL)
					{
						newObjName = objName + CRCore::crArgumentParser::appItoa(m_i++);
						m_plodObjectNameChanged = true;
					}
					node.setName(newObjName);

					CRCore::ref_ptr<CRCore::crBrain::ObjectLight> objLight = new CRCore::crBrain::ObjectLight;
					objLight->m_objectName = objName;
					for( CRCore::crObject::LightNameSet::iterator itr = avoidLightSet.begin();
						itr != avoidLightSet.end();
						++itr )
					{
						objLight->m_avoidLight.insert(*itr);
					}

					for( CRCore::crObject::LightNameSet::iterator itr = needLightSet.begin();
						itr != needLightSet.end();
						++itr )
					{
						objLight->m_needLight.insert(*itr);
					}
					brain->insertObjectLight(objLight.get());
				}
			}
		  }
		  traverse(node);
	  }
protected:
	int m_i;
	std::string m_trasvelstr;
	bool m_plodObjectNameChanged;
	typedef std::set< CRCore::ref_ptr<CRCore::crObject> > ObjectSet;
	ObjectSet m_objectSet;
};

//void crAITreeHead::write(crDataOutputStream* out){
//	out->writeInt(CREAITREEHEAD);
//    CRCore::crGroup* node = dynamic_cast<CRCore::crGroup*>(this);
//    if(node){
//        static_cast<cre::crGroup*>(node)->write(out);
//    }
//    else
//        throw crException("crAITreeHead::write(): Could not cast this CRCore::crAITreeHead to an CRCore::crGroup.");
//
//}
//
//void crAITreeHead::read(crDataInputStream* in){
//	int id = in->peekInt();
//	if(id == CREAITREEHEAD){
//		id = in->readInt();
//
//		CRCore::crGroup* node = dynamic_cast<CRCore::crGroup*>(this);
//		if(node){
//			((cre::crGroup*)(node))->read(in);
//		}
//		else
//			throw crException("crEventGroup::read(): Could not cast this CRCore::crEventGroup to an CRCore::crGroup.");
//
//	}
//	else{
//		throw crException("crGroup::read(): Expected crGroup identification");
//	}
//}

void crBrain::write(crDataOutputStream* out){
    out->writeInt(CREBRAIN);
	CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
	if(obj){
		((cre::crBase*)(obj))->write(out);
	}
	else
		throw crException("crBrain::write(): Could not cast this CRCore::crBrain to an CRCore::crBase.");

	//此处写入注册码信息或应用程序的唯一序列号，可保证每版本都只能备使用于一个项目
	//设计方法如下，可设计一个长整型数据掩码，该掩码信息可由文件打包时候的日期和时间设定

    if ( out->getVersion() >= VERSION_0016 )
	{
		clearObjectLightMap();//输出的时候重建
		crCollectObjectLightVisitor collectObjectLightVisitor;
		this->accept(collectObjectLightVisitor);
	}

    out->writeInt(getDBCount());
	for( DBArray::const_iterator itr = m_dbArray.begin();
		 itr != m_dbArray.end();
		 ++itr )
	{
		out->writeNode(itr->get());
	}

	if ( out->getVersion() >= VERSION_0016 )
	{
		const ObjectLight *objLight;
        const ObjectLightMap &objectLightMap = getObjectLightMap();
		out->writeInt(objectLightMap.size());
		for( ObjectLightMap::const_iterator itr = objectLightMap.begin();
			 itr != objectLightMap.end();
			 ++itr )
		{
			objLight = itr->second.get();
			out->writeString(objLight->m_objectName);
			out->writeInt(objLight->m_avoidLight.size());
			for( ObjectLight::LightNameSet::const_iterator aitr = objLight->m_avoidLight.begin();
				 aitr != objLight->m_avoidLight.end();
				 ++aitr )
			{
                out->writeString(*aitr);
			}
			out->writeInt(objLight->m_needLight.size());
			for( ObjectLight::LightNameSet::const_iterator nitr = objLight->m_needLight.begin();
				 nitr != objLight->m_needLight.end();
				 ++nitr )
			{
				out->writeString(*nitr);
			}
		}
	}
	//out->writeInt(getGlobalLightNum());

	//out->writeBool(getResource()!=0);
	//if(getResource()!=0)
	//{
 //       ((cre::crResource*)(getResource()))->write(out);
	//}
    
	//out->writeBool(getAITreeHead()!=0);
	//if(getAITreeHead()!=0)
	//{
	//	((cre::crAITreeHead*)(getAITreeHead()))->write(out);
	//}
    
	out->writeString(m_description);

	out->writeUInt(m_currentActiveDB);

	//////////////////////postprocess effect
	out->writeVec2(m_clip);
	out->writeVec4(m_ambientIntensity);
	out->writeFloat(m_brightness);
	out->writeInt(m_fog);
	out->writeVec3(m_fogColor);
	if ( out->getVersion() >= VERSION_0030 )
		out->writeVec4(m_fogParams);
	//else
	//	out->writeVec3(m_fogParams);
	out->writeFloat(m_maxDepth);
	if ( out->getVersion() >= VERSION_0018 )
		out->writeFloat(m_dofBlurStart);
	out->writeFloat(m_dofBlurFactor);
	out->writeFloat(m_motionBlurFactor);
	out->writeInt(m_initLoadPage.size());
	for( StringSet::iterator itr = m_initLoadPage.begin();
		 itr != m_initLoadPage.end();
		 ++itr )
	{
		out->writeString(*itr);
	}
	if ( out->getVersion() < VERSION_0034 )
	{
		out->writeInt(0/*m_initRenderVecBuf.size()*/);
		//for( InitRenderVecBuf::iterator itr = m_initRenderVecBuf.begin();
		//	itr != m_initRenderVecBuf.end();
		//	++itr )
		//{
		//	out->writeVec3(*itr);
		//}
	}
	if ( out->getVersion() >= VERSION_0012 )
	{
		out->writeVec3(m_upperSkyColor);
		out->writeVec3(m_lowerSkyColor);
	}
	if ( out->getVersion() >= VERSION_0027 )
	{
		out->writeBool(getCalcStaticMeshShadow());
	}
	if ( out->getVersion() >= VERSION_0035 )
	{
		out->writeVec3i(m_worldSize);
		out->writeVec2i(m_mapSize);
	}
}

void crBrain::read(crDataInputStream* in){
    int id = in->peekInt();
    if(id == CREBRAIN){
        id = in->readInt();

        CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
        if( obj )
        {
            ( ( cre::crBase* )( obj ) )->read( in );
        }
        else
		{
			throw crException( "crBrain::read(): Could not cast this CRCore::crBrain to an CRCore::crBase." );
		}

		int size = in->readInt();
		int i;
		for(i = 0; i<size; i++)
		{
			insertDB(dynamic_cast<CRCore::crDB*>(in->readNode()));
		}

		if ( in->getVersion() >= VERSION_0016 )
		{
			size = in->readInt();
			int asize,nsize;
			int j;
			CRCore::ref_ptr<CRCore::crBrain::ObjectLight> objLight;
			for(i = 0; i<size; i++)
			{
                objLight = new CRCore::crBrain::ObjectLight;
				objLight->m_objectName = in->readString();
                asize = in->readInt();
				for(j = 0; j<asize; j++)
				{
				    objLight->m_avoidLight.insert(in->readString());
				}
				nsize = in->readInt();
				for(j = 0; j<nsize; j++)
				{
					objLight->m_needLight.insert(in->readString());
				}
				insertObjectLight(objLight.get());
			}
		}

		//m_globalLightNum = in->readInt();

		//if(in->readBool())
		//{
		//	((cre::crResource*)(getResource()))->read(in);
		//}
		
		//if(in->readBool())
		//{
		//	((cre::crAITreeHead*)(getAITreeHead()))->read(in);
		//}

		m_description = in->readString();

		m_currentActiveDB = in->readUInt();

		//////////////////////postprocess effect
		setClip(in->readVec2());
        m_ambientIntensity = in->readVec4();
		m_brightness = in->readFloat();
        m_fog = in->readInt();
        m_fogColor = in->readVec3();
		if ( in->getVersion() >= VERSION_0030 )
			m_fogParams = in->readVec4();
		else
		{
			CRCore::crVector3 fog = in->readVec3();
			m_fogParams[0] = fog[0];
			m_fogParams[1] = fog[1];
			m_fogParams[2] = fog[2];
		}
        m_maxDepth = in->readFloat();
		if ( in->getVersion() >= VERSION_0018 )
			m_dofBlurStart = in->readFloat();
        m_dofBlurFactor = in->readFloat();
        m_motionBlurFactor = in->readFloat();
        int initLoadPageSize = in->readInt();
		for(int i = 0; i<initLoadPageSize; ++i)
		{
            m_initLoadPage.insert(in->readString());
		}
		if ( in->getVersion() < VERSION_0034 )
		{
			int initRenderVecBufSize = in->readInt();
			for(int i = 0; i<initRenderVecBufSize; ++i)
			{
				in->readVec3();
				/*m_initRenderVecBuf.push_back(in->readVec3());*/
			}
		}
		if ( in->getVersion() >= VERSION_0012 )
		{
			m_upperSkyColor = in->readVec3();
			m_lowerSkyColor = in->readVec3();
		}
		if ( in->getVersion() >= VERSION_0027 )
		{
			setCalcStaticMeshShadow(in->readBool());
		}
		if ( in->getVersion() >= VERSION_0035 )
		{
			m_worldSize = in->readVec3i();
			m_mapSize = in->readVec2i();
		}
    }
    else{
        throw crException("crGroup::read(): Expected crGroup identification");
    }
}
