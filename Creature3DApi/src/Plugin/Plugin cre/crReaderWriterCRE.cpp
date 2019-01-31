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
#include <rbody/Creature3D/CreBody.h>
#include <Plugin cre/crMatrixTransform.h>
#include <Plugin cre/crGroup.h>
#include <Plugin cre/crBrain.h>
#include <Plugin cre/crImage.h>
#include <Plugin cre/shd>
#include <Plugin cre/cook>
//#include "creature3d.h"
#include <Plugin cre/crExternFileLoadVisitor.h>
#include <CRUtil/crAcceptGIVisitor.h>
#include <CRNetApp/appNetGameData.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRNetApp/appClientPlayerData.h>
//#include <CRNetApp/appGlobalHandle.h>
//#include <CRDownload/crDownload.h>
//#include <CREncapsulation/crTableIO.h>
//#include "crCreateAiVisitor.h"

using namespace CRCore;
using namespace CRIOManager;
using namespace CRUtil;
using namespace CRNetApp;
//using namespace CRDownload;
//using namespace CREncapsulation;
class crCollectImageVisitor : public CRCore::crNodeVisitor
{//收集需要先行动态下载的文件
public:
	crCollectImageVisitor():crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN){};
	virtual void apply(CRCore::crNode& node)
	{
		//if(node.getRenderable2())
		//{
			if(node.getStateSet())
			{
				apply(node.getStateSet());
			}
			traverse(node);
		//}
	}
	virtual void apply(rbody::CreBodyNode& node)
	{//收集CAF,CSF,CMF,(rbody,char)
		//const std::string &filename = node.getFileName();
		//m_imageFileSet.insert(filename);
		//const std::string &core_body_name = node.getBody()->getCoreBodyName();
		//m_imageFileSet.insert(core_body_name);
		const std::string &skeleton = node.getBody()->getCoreBody()->getSkeletonFileName();
		m_imageFileSet.insert(skeleton);
		node.getBody()->getCoreBody()->getMeshFileNames(m_imageFileSet);
		node.getBody()->getCoreBody()->getAnimationFileNames(m_imageFileSet);
		traverse(node);
	}
	virtual void apply(CRCore::crObject& node)
	{
		//if(node.getRenderable2())
		//{
			if(node.getStateSet())
			{
				apply(node.getStateSet());
			}
			CRCore::crObject::DrawableList &drawableList = node.getDrawableList();
			for( CRCore::crObject::DrawableList::iterator itr = drawableList.begin();
				itr != drawableList.end();
				++itr )
			{
				if((*itr)->getStateSet())
				{
					apply((*itr)->getStateSet());
				}
			}
		//}
	}
	virtual void apply(CRCore::crStateSet* ss)
	{
		const crStateSet::TextureAttributeList &tal = ss->getTextureAttributeList();
		int nUnits;
		nUnits = tal.size();
		for(int unit=0;unit<nUnits;unit++)
		{
			const crStateSet::AttributeList &al_t = tal[unit];
			for(crStateSet::AttributeList::const_iterator aitr=al_t.begin(); aitr!=al_t.end(); ++aitr)
			{
				if(dynamic_cast<const crTextureCubeMap *>(aitr->second.first.get()))
				{
					const crTextureCubeMap *texcube = dynamic_cast<const crTextureCubeMap *>(aitr->second.first.get());
					addCubeImageFiles(texcube->getImageNameID());
				}
				else if(dynamic_cast<const crTexture *>(aitr->second.first.get()))
				{
					const crTexture *tex = dynamic_cast<const crTexture *>(aitr->second.first.get());
					addImageFiles(tex->getImageNameID());
				}
			}
		}
	}
	typedef std::set<std::string> ImageFileSet;
	ImageFileSet &getImageFileSet(){ return m_imageFileSet; }
protected:
	void addImageFiles(const std::string &imageid)
	{//需要用全路径
		std::string imagefile = CRIOManager::findDataFile( imageid,CRIOManager::crRegistry::instance()->getOptions() );
		if(!imagefile.empty())
		{
			struct stat buf;
			if(!stat( imagefile.c_str(), &buf )) 
			{
				CREncapsulation::crTableIO::StrVec record;
				imagefile = CRIOManager::convertFileNameToUnixStyle(imagefile);
				m_imageFileSet.insert(imagefile);
			}
		}
	}
	void addCubeImageFiles(const std::string &cubeImageNameID)
	{//需要用全路径
		std::string ext = CRCore::crArgumentParser::getFileExtension(cubeImageNameID);
		std::string fileName = CRCore::crArgumentParser::getFileNameEliminateExt(cubeImageNameID);

		std::string file = CUBEMAP_FILENAME(fileName,posx,ext);
		std::string imagefile = CRIOManager::findDataFile( file,CRIOManager::crRegistry::instance()->getOptions() );
		if(!imagefile.empty())
		{
			struct stat buf;
			if(!stat( imagefile.c_str(), &buf )) 
			{
				CREncapsulation::crTableIO::StrVec record;
				imagefile = CRIOManager::convertFileNameToUnixStyle(imagefile);
				m_imageFileSet.insert(imagefile);
			}
		}
		file = CUBEMAP_FILENAME(fileName,negx,ext);
		imagefile = CRIOManager::findDataFile( file,CRIOManager::crRegistry::instance()->getOptions() );
		if(!imagefile.empty())
		{
			struct stat buf;
			if(!stat( imagefile.c_str(), &buf )) 
			{
				CREncapsulation::crTableIO::StrVec record;
				imagefile = CRIOManager::convertFileNameToUnixStyle(imagefile);
				m_imageFileSet.insert(imagefile);
			}
		}
		file = CUBEMAP_FILENAME(fileName,posy,ext);
		imagefile = CRIOManager::findDataFile( file,CRIOManager::crRegistry::instance()->getOptions() );
		if(!imagefile.empty())
		{
			struct stat buf;
			if(!stat( imagefile.c_str(), &buf )) 
			{
				CREncapsulation::crTableIO::StrVec record;
				imagefile = CRIOManager::convertFileNameToUnixStyle(imagefile);
				m_imageFileSet.insert(imagefile);
			}
		}
		file = CUBEMAP_FILENAME(fileName,negy,ext);
		imagefile = CRIOManager::findDataFile( file,CRIOManager::crRegistry::instance()->getOptions() );
		if(!imagefile.empty())
		{
			struct stat buf;
			if(!stat( imagefile.c_str(), &buf )) 
			{
				CREncapsulation::crTableIO::StrVec record;
				imagefile = CRIOManager::convertFileNameToUnixStyle(imagefile);
				m_imageFileSet.insert(imagefile);
			}
		}
		file = CUBEMAP_FILENAME(fileName,posz,ext);
		imagefile = CRIOManager::findDataFile( file,CRIOManager::crRegistry::instance()->getOptions() );
		if(!imagefile.empty())
		{
			struct stat buf;
			if(!stat( imagefile.c_str(), &buf )) 
			{
				CREncapsulation::crTableIO::StrVec record;
				imagefile = CRIOManager::convertFileNameToUnixStyle(imagefile);
				m_imageFileSet.insert(imagefile);
			}
		}
		file = CUBEMAP_FILENAME(fileName,negz,ext);
		imagefile = CRIOManager::findDataFile( file,CRIOManager::crRegistry::instance()->getOptions() );
		if(!imagefile.empty())
		{
			struct stat buf;
			if(!stat( imagefile.c_str(), &buf )) 
			{
				CREncapsulation::crTableIO::StrVec record;
				imagefile = CRIOManager::convertFileNameToUnixStyle(imagefile);
				m_imageFileSet.insert(imagefile);
			}
		}
	}
	ImageFileSet m_imageFileSet;
};
class crReaderWriterCRE : public crReaderWriter
{
    public:
        virtual const char* className() const { return "CRE Reader/Writer"; }

        virtual bool acceptsExtension(const std::string& extension) const
        {
            return equalCaseInsensitive(extension,"cre") ||
				   equalCaseInsensitive(extension,"crs") ||
				   equalCaseInsensitive(extension,"cru") ||
				   equalCaseInsensitive(extension,"img");
        }
        virtual ReadResult readObject(const std::string& file, const Options* options) const
        {
			std::string ext = CRIOManager::getLowerCaseFileExtension(file);
			if(equalCaseInsensitive(ext,"cook"))
			{
				if(!loadCookedFile(CRCore::crArgumentParser::getFileNameEliminateExt(file)))
				{
					//std::string str = file + "Cook文件没有找到，程序将自动退出！";
					//MessageBox(NULL,str.c_str(),"Creature3D Error",MB_OK);
					//char* ptr;
					//if ((ptr = getenv("CRE_SERVER")))
					//{
					//	exit(1);
					//}
					//else
					//{
					//	CRCore::crBrain::getInstance()->doEvent(MAKEINT64(WCH_EXITGAME,1));
					//}
				}
				return ReadResult::FILE_LOADED;
			}
			else if(equalCaseInsensitive(ext,"mcook"))
			{
				loadCookedFileM(CRCore::crArgumentParser::getFileNameEliminateExt(file));
				return ReadResult::FILE_LOADED;
			}
			else if(equalCaseInsensitive(ext,"shd"))
			{
				loadShaderManagerCfg();
				return ReadResult::FILE_LOADED;
			}
			if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

			std::string fileName = CRIOManager::findDataFile( file, options );
			if (fileName.empty())
			{
				//if(CRNetApp::crGlobalHandle::isClient() && crDisplaySettings::instance()->getRunMode()>0)
				//{
				//	if(equalCaseInsensitive(ext,"crs"))
				//	{
				//		std::string str = CRCore::crArgumentParser::getFileNameEliminateExt(fileName) + ".crb";
				//		ref_ptr<crTableIO> table = new crTableIO;
				//		if(table->openFile(str))
				//		{
				//			int rowcount = table->getRowCount();
				//			for (int i = 0; i < rowcount; i++)
				//			{
				//				crDownloadManager::getInstance()->addImmediatelyDownloadRequest(new crDownload((*table)(i,0)));
				//			}
				//		}
				//		crDownloadManager::getInstance()->addImmediatelyDownloadRequest(new crDownload(fileName));
				//	}
				//}
				if(CRNetApp::crGlobalHandle::isClient() && crDisplaySettings::instance()->getRunMode()>0)
					crBrain::getInstance()->doEvent(WCH_DOWNLOAD,MAKECREPARAM(&file,NULL));
				return ReadResult::FILE_NOT_FOUND;
			}

			// code for setting up the database path so that internally referenced file are searched for on relative paths. 
			CRCore::ref_ptr<Options> local_opt = options ? static_cast<Options*>(options->clone(CRCore::crCopyOp::SHALLOW_COPY)) : new Options;
			local_opt->setDatabasePath(CRIOManager::getFilePath(fileName));

			std::ifstream istream(fileName.c_str(), std::ios::in | std::ios::binary);
			//CRCore::notify(CRCore::ALWAYS)<<"readObject = "<<fileName<<std::endl;
			try {
				return readObject(istream,local_opt.get());
			}
			catch(cre::crException e)
			{
				istream.close();
				//CRCore::notify(CRCore::NOTICE)<<"Error reading file: "<< e.getError()<<std::endl;
				char gbuf[256];
				sprintf(gbuf,"Error readObject:%s,File:%s\n\0",e.getError().c_str(),fileName.c_str());
				gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
				DeleteFileA(fileName.c_str());
				//MessageBox(NULL,"游戏文件错误，请重启游戏客户端，如果还不能解决问题，请重新下载游戏。","Creature3D",MB_OK);
				return ReadResult::FILE_NOT_HANDLED;		
			}
        }
        
        virtual ReadResult readNode(const std::string& file, const Options* options) const
        {
            std::string ext = CRIOManager::getLowerCaseFileExtension(file);
            if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

            std::string fileName = CRIOManager::findDataFile( file, options );
			if (fileName.empty())			
			{
				//if(CRNetApp::crGlobalHandle::isClient() && crDisplaySettings::instance()->getRunMode()>0)
				//{
				//	if(equalCaseInsensitive(ext,"cre"))
				//	{
				//		std::string str = CRCore::crArgumentParser::getFileNameEliminateExt(fileName) + ".crb";
				//		ref_ptr<crTableIO> table = new crTableIO;
				//		if(table->openFile(str))
				//		{
				//			int rowcount = table->getRowCount();
				//			for (int i = 0; i < rowcount; i++)
				//			{
				//				crDownloadManager::getInstance()->addImmediatelyDownloadRequest(new crDownload((*table)(i,0)));
				//			}
				//		}
				//		crDownloadManager::getInstance()->addImmediatelyDownloadRequest(new crDownload(fileName));
				//	}
				//	else if(equalCaseInsensitive(ext,"cru"))
				//	{
				//		std::string str = CRNetApp::crGlobalHandle::getInstance()->getUISceneDir()+CRCore::crArgumentParser::getFileNameEliminateExt(fileName) + ".crb";
				//		ref_ptr<crTableIO> table = new crTableIO;
				//		if(table->openFile(str))
				//		{
				//			int rowcount = table->getRowCount();
				//			for (int i = 0; i < rowcount; i++)
				//			{
				//				crDownloadManager::getInstance()->addImmediatelyDownloadRequest(new crDownload((*table)(i,0)));
				//			}
				//		}
				//		crDownloadManager::getInstance()->addImmediatelyDownloadRequest(new crDownload(CRNetApp::crGlobalHandle::getInstance()->getUISceneDir()+fileName));
				//	}
				//}
				if(CRNetApp::crGlobalHandle::isClient() && crDisplaySettings::instance()->getRunMode()>0)
					crBrain::getInstance()->doEvent(WCH_DOWNLOAD,MAKECREPARAM(&file,NULL));
				return ReadResult::FILE_NOT_FOUND;
			}

            // code for setting up the database path so that internally referenced file are searched for on relative paths. 
            CRCore::ref_ptr<Options> local_opt = options ? static_cast<Options*>(options->clone(CRCore::crCopyOp::SHALLOW_COPY)) : new Options;
            local_opt->setDatabasePath(CRIOManager::getFilePath(fileName));
            
			std::ifstream istream(fileName.c_str(), std::ios::in | std::ios::binary);
			try{
				//CRCore::notify(CRCore::ALWAYS)<<"readNode = "<<fileName<<std::endl;
				return readNode(istream,local_opt.get());
			}
			catch(cre::crException e)
			{
				istream.close();
				//CRCore::notify(CRCore::NOTICE)<<"Error reading file: "<< e.getError()<<std::endl;
				char gbuf[256];
				sprintf(gbuf,"Error readNode:%s,File:%s\n\0",e.getError().c_str(),fileName.c_str());
				gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
				DeleteFileA(fileName.c_str());
				//MessageBox(NULL,"游戏文件错误，请重启游戏客户端，如果还不能解决问题，请重新下载游戏。","Creature3D",MB_OK);
				return ReadResult::FILE_NOT_HANDLED;
			}
        }
        
        virtual ReadResult readObject(std::istream& fin, const Options* options) const
        {
			// Create datainputstream.
			cre::crDataInputStream in(&fin);
			in.setOptions(options);

			crRunGameHandle::getInstance()->setLoadingProgress(0.2f);

			crBrain *brain = crBrain::getInstance();
			((cre::crBrain *)(brain))->read(&in);
                
			crRunGameHandle::getInstance()->setLoadingProgress(0.9f);
			if(in.getVersion() <= VERSION_0027)
			{
				CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
				searchByNameVisitor.setNameId("Wucaihua");
				searchByNameVisitor.setSearchNodeType(CRCore::GROUP);
				brain->accept(searchByNameVisitor);
				CRCore::ref_ptr<CRCore::crNode> wucaihua = searchByNameVisitor.getResult();
				if(wucaihua.get())
				{
					if(wucaihua->getParent(0)->getName().compare("Static") == 0)
					{
						CRCore::crGroup *staticGroup = wucaihua->getParent(0);
						searchByNameVisitor.reset();
						searchByNameVisitor.setNameId("Dynamic");
						searchByNameVisitor.setSearchNodeType(CRCore::GROUP);
						brain->accept(searchByNameVisitor);
						CRCore::crGroup *dynamicGroup = dynamic_cast<CRCore::crGroup *>(searchByNameVisitor.getResult());
						if(dynamicGroup)
						{
							staticGroup->removeChild(wucaihua.get());
							dynamicGroup->insertChild(0,wucaihua.get());
						}
					}
				}
			}

			CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();
			CRCore::Timer_t end_tick;
			CRCore::notify(CRCore::ALWAYS)<<"RUN_OPTIMIZATIONS..."<< std::endl;

			CRCore::crBrain::DBArray &dbArray = brain->getDBArray();
			for( CRCore::crBrain::DBArray::iterator itr = dbArray.begin();
					itr != dbArray.end();
					++itr )
			{
				CRUtil::crOptimizer optimizer;
				if(CRCore::crDisplaySettings::instance()->getRunMode()==0)
				{
					optimizer.optimize(itr->get(),CRUtil::crOptimizer::RUN_OPTIMIZATIONS_EDIT);
				}
				else
				{
					optimizer.optimize(itr->get(),CRUtil::crOptimizer::RUN_OPTIMIZATIONS);
				}
				(*itr)->getBound();
			}
			end_tick = CRCore::Timer::instance()->tick();
			CRCore::notify(CRCore::ALWAYS)<<"Time to RUN_OPTIMIZATIONS = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;

			//CRCore::notify(CRCore::WARN)<<"crBrain loaded"<<std::endl;
			ref_ptr<CREncapsulation::crTableIO> scenarioTab;
			CRNetApp::crScene *scene = CRNetApp::crMyPlayerData::getInstance()->getScene();
			if(scene)
			{
				std::string sceneName = scene->getSceneName();
				std::string filename = CRNetApp::crGlobalHandle::getInstance()->getScenarioTableDir()+sceneName+".crb";
				//m_scenarioTab = new CREncapsulation::crTableIO;
				scenarioTab = CREncapsulation::crTableIO::openFile(filename);
			}

			CRGame::crExternFileLoadVisitor extload(scenarioTab.get());	
			brain->accept(extload);
			crFinalCommandParseVisitor finaParse;
			brain->accept(finaParse);

			//CRCore::notify(CRCore::WARN)<<"crExternFileLoadVisitor loaded"<<std::endl;

			//CRGame::crCreateAiVisitor createAi(brain->getCurrentActiveDB());
			//brain->accept(createAi);

			//CRCore::notify(CRCore::WARN)<<"crCreateAiVisitor loaded"<<std::endl;
            fin.clear();
			crRunGameHandle::getInstance()->setLoadingProgress(1.0f);
			return brain;
        }
        
        virtual ReadResult readNode(std::istream& fin, const Options* options) const
        {
            // Create datainputstream.
            cre::crDataInputStream in(&fin);
            in.setOptions(options);

			CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();
			CRCore::Timer_t end_tick;
			CRCore::notify(CRCore::ALWAYS)<<"RUN_OPTIMIZATIONS..."<< std::endl;

			crNode *node = in.readNode();
			CRUtil::crOptimizer optimizer;
			if(CRCore::crDisplaySettings::instance()->getRunMode()==0)
			{
				optimizer.optimize(node,CRUtil::crOptimizer::RUN_OPTIMIZATIONS_EDIT);
			}
			else
			{
				optimizer.optimize(node,CRUtil::crOptimizer::RUN_OPTIMIZATIONS);
			}
				
			end_tick = CRCore::Timer::instance()->tick();
			CRCore::notify(CRCore::ALWAYS)<<"Time to RUN_OPTIMIZATIONS = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;

			CRGame::crExternFileLoadVisitor extload;
			node->accept(extload);
			crFinalCommandParseVisitor finaParse;
			node->accept(finaParse);
			//CRGame::crCreateAiVisitor createAi((crGroup *)node);
			//node->accept(createAi);
				
			CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
			searchByNameVisitor.setNameId("RemovedGroup");
			searchByNameVisitor.setSearchNodeType(GROUP);
			node->accept(searchByNameVisitor);
			crNode * removedGroup = searchByNameVisitor.getResult();
			if(removedGroup)
			{
				//CRCore::notify(CRCore::FATAL)<<"RemovedGroup: "<<node<<std::endl;
				removedGroup->getParent(0)->removeChild(removedGroup);
			}

			//CRUtil::crPrepareVisitor prepareVisitor;
			//node->accept(prepareVisitor);
			crBrain::getInstance()->prune();
			fin.clear();
			node->getBound();
            return node;
        }
        
		virtual ReadResult readImage(const std::string& file, const CRIOManager::crReaderWriter::Options* options) const
		{
			std::string ext = CRIOManager::getLowerCaseFileExtension(file);
			if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

			std::string fileName = CRIOManager::findDataFile( file, options );
			if (fileName.empty())
			{
				//if(CRNetApp::crGlobalHandle::isClient() && crDisplaySettings::instance()->getRunMode()>0)
				//{
				//	crDownloadManager::getInstance()->addImmediatelyDownloadRequest(new crDownload(CRNetApp::crGlobalHandle::getInstance()->getTextureDir()+fileName));
				//}
				if(CRNetApp::crGlobalHandle::isClient() && crDisplaySettings::instance()->getRunMode()>0)
				{
					if(CRIOManager::getFilePath(file).empty())
						fileName = CRNetApp::crGlobalHandle::getInstance()->getTextureDir() + file;
					else
						fileName = file;
					crBrain::getInstance()->doEvent(WCH_DOWNLOAD,MAKECREPARAM(&fileName,NULL));
				}
				return ReadResult::FILE_NOT_FOUND;
			}

			std::ifstream istream(fileName.c_str(), std::ios::in | std::ios::binary);
			if(!istream) return ReadResult::FILE_NOT_HANDLED;
			
			try{
				return readImage(istream);
			}
			catch(cre::crException e)
			{
				istream.close();
				//CRCore::notify(CRCore::NOTICE)<<"Error reading file: "<< e.getError()<<std::endl;
				char gbuf[256];
				sprintf(gbuf,"Error readImage file:%s\n\0",fileName.c_str());
				gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
				DeleteFileA(fileName.c_str());
				//MessageBox(NULL,"游戏文件错误，请重启游戏客户端，如果还不能解决问题，请重新下载游戏。","Creature3D",MB_OK);
				return ReadResult::FILE_NOT_HANDLED;
			}
		}

		virtual ReadResult readImage(std::istream& fin,const CRIOManager::crReaderWriter::Options* =NULL) const
		{
			cre::crDataInputStream in(&fin);
			in.setOptions(NULL);

			CRCore::crImage* image = new CRCore::crImage();
			((cre::crImage*)image)->read(&in);

			fin.clear();
			return image;
		}

		virtual WriteResult writeImage(const CRCore::crImage &img,const std::string& fileName, const CRIOManager::crReaderWriter::Options *options) const
		{
			std::string ext = CRIOManager::getFileExtension(fileName);
			if (!acceptsExtension(ext)) return WriteResult::FILE_NOT_HANDLED;
			std::string xfilename;
			std::string path = CRIOManager::getFilePath(fileName);
			std::string name = CRIOManager::getSimpleFileName(fileName);
			if(path.empty())
				xfilename = path+"~"+name;
			else
				xfilename = path+"/"+"~"+name;
			std::ofstream fout(xfilename.c_str(), std::ios::out | std::ios::binary);
			if(!fout) return WriteResult::ERROR_IN_WRITING_FILE;
			WriteResult result = writeImage(img,fout,options);
			fout.close();
			if(result.success())
			{
				CRIOManager::copyFile(xfilename,fileName);
				DeleteFileA(xfilename.c_str());
			}
			//CRCore::notify(CRCore::WARN)<<"writeImage "<< fileName.c_str() << std::endl;
			return result;
		}

		virtual WriteResult writeImage(const CRCore::crImage& img,std::ostream& fout,const CRIOManager::crReaderWriter::Options *options) const
		{
			try
			{
				cre::crDataOutputStream out(&fout,"");
				out.setOptions(options);

				((cre::crImage&)(img)).write(&out);
				return WriteResult::FILE_SAVED;
			}
			catch(cre::crException e)
			{
				CRCore::notify(CRCore::WARN)<<"Error writing img file: "<< e.getError() << std::endl;
			}
			return WriteResult::FILE_NOT_HANDLED;
		}

        virtual WriteResult writeObject(const crBase& object,const std::string& fileName, const CRIOManager::crReaderWriter::Options* options) const
        {
			std::string ext = getFileExtension(fileName);
			if(equalCaseInsensitive(ext,"cook"))
			{
				writeCookedFile(CRCore::crArgumentParser::getFileNameEliminateExt(fileName));
				return WriteResult::FILE_SAVED;
			}
			else if(equalCaseInsensitive(ext,"mcook"))
			{
				writeCookedFileM(CRCore::crArgumentParser::getFileNameEliminateExt(fileName));
				return WriteResult::FILE_SAVED;
			}
			else if(equalCaseInsensitive(ext,"shd"))
			{
				convertShader(CRCore::crArgumentParser::getFileNameEliminateExt(fileName));
				return WriteResult::FILE_SAVED;
			}
			if (!acceptsExtension(ext)) return WriteResult::FILE_NOT_HANDLED;

			const crBrain *brain = dynamic_cast<const crBrain *>(&object);
			if(brain) 
			{
				CRCore::ref_ptr<Options> local_opt = options ? static_cast<Options*>(options->clone(CRCore::crCopyOp::SHALLOW_COPY)) : new Options;
				if(local_opt->getDatabasePathList().empty())
					local_opt->setDatabasePath(CRIOManager::getFilePath(fileName));

				std::string xfilename;
				std::string path = CRIOManager::getFilePath(fileName);
				std::string name = CRIOManager::getSimpleFileName(fileName);
				if(path.empty())
					xfilename = path+"~"+name;
				else
					xfilename = path+"/"+"~"+name;

				std::ofstream fout(xfilename.c_str(), std::ios::out | std::ios::binary);
				WriteResult result = writeObject(object, fout, fileName, local_opt.get());
				fout.close();
				if(result.success())
				{
					CRIOManager::copyFile(xfilename,fileName);
					DeleteFileA(xfilename.c_str());
				}
				return result;
			}

            const crNode* node = dynamic_cast<const crNode*>(&object);
            if (node) return writeNode( *node, fileName, options );
            
			return WriteResult::FILE_NOT_HANDLED;
        }

        virtual WriteResult writeNode(const crNode& node,const std::string& fileName, const CRIOManager::crReaderWriter::Options* options) const
        {
            std::string ext = getFileExtension(fileName);
            if (!acceptsExtension(ext)) return WriteResult::FILE_NOT_HANDLED;

            // code for setting up the database path so that internally referenced file are searched for on relative paths. 
            CRCore::ref_ptr<Options> local_opt = options ? static_cast<Options*>(options->clone(CRCore::crCopyOp::SHALLOW_COPY)) : new Options;
            if(local_opt->getDatabasePathList().empty())
                local_opt->setDatabasePath(CRIOManager::getFilePath(fileName));

			std::string xfilename;
			std::string path = CRIOManager::getFilePath(fileName);
			std::string name = CRIOManager::getSimpleFileName(fileName);
			if(path.empty())
				xfilename = path+"~"+name;
			else
				xfilename = path+"/"+"~"+name;
            std::ofstream fout(xfilename.c_str(), std::ios::out | std::ios::binary);
            WriteResult result = writeNode(node, fout, fileName, local_opt.get());
			fout.close();
			if(result.success())
			{
				CRIOManager::copyFile(xfilename,fileName);
				DeleteFileA(xfilename.c_str());
			}
            return result;
        }
        
		virtual WriteResult writeObject(const crBase& object,std::ofstream& fout, const std::string& filename, const CRIOManager::crReaderWriter::Options* options) const
        {
			const crBrain *brain = dynamic_cast<const crBrain *>(&object);
			if(brain) 
			{
				try
				{
					cre::crDataOutputStream out(&fout,filename);

					out.setOptions(options);

					((cre::crBrain*)(const_cast<CRCore::crBrain*>(brain)))->write(&out);
					fout.close();

					crCollectImageVisitor collectImage;
					(const_cast<CRCore::crBrain*>(brain))->accept(collectImage);
					crCollectImageVisitor::ImageFileSet &imageFileSet = collectImage.getImageFileSet();
					for( crCollectImageVisitor::ImageFileSet::iterator itr = imageFileSet.begin();
						 itr != imageFileSet.end();
						 ++itr )
					{
						out.addTabFiles(*itr);
					}
					out.saveFilesTab();
					return WriteResult::FILE_SAVED;
				}
				catch(cre::crException e)
				{		
					CRCore::notify(CRCore::WARN)<<"Error writing CRE file: "<< e.getError() << std::endl;
				}
			}
			else
			{
				const crNode* node = dynamic_cast<const crNode*>(&object);
				if (node) return writeNode( *node, fout, filename, options );
			}
            return WriteResult::FILE_NOT_HANDLED;
        }

        virtual WriteResult writeNode(const crNode& node,std::ofstream& fout, const std::string& filename, const CRIOManager::crReaderWriter::Options* options) const
        {
            try
            {
                cre::crDataOutputStream out(&fout,filename);

                out.setOptions(options);

                out.writeNode(const_cast<CRCore::crNode*>(&node));
				fout.close();

				crCollectImageVisitor collectImage;
				(const_cast<CRCore::crNode&>(node)).accept(collectImage);
				crCollectImageVisitor::ImageFileSet &imageFileSet = collectImage.getImageFileSet();
				for( crCollectImageVisitor::ImageFileSet::iterator itr = imageFileSet.begin();
					itr != imageFileSet.end();
					++itr )
				{
					out.addTabFiles(*itr);
				}
				out.saveFilesTab();
                return WriteResult::FILE_SAVED;
            }
            catch(cre::crException e)
            {
                CRCore::notify(CRCore::WARN)<<"Error writing CRE file: "<< e.getError() << std::endl;
            }
            return WriteResult::FILE_NOT_HANDLED;

        }
};

// now register with crRegistry to instantiate the above
// reader/writer.
static RegisterReaderWriterProxy<crReaderWriterCRE> g_CREReaderWriterProxy;
