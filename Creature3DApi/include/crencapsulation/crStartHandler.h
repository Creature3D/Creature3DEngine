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
#ifndef CRENCAPSULATION_CRSTARTHANDLER_H
#define CRENCAPSULATION_CRSTARTHANDLER_H 1

#include <CREncapsulation/crExport.h>
#include <CRCore/ref_ptr.h>
#include <CRProducer/crViewer.h>
#include <CRCore/crTexture.h>
#include <CRCore/crImage.h>
#include <CRCore/crBrain.h>
#include <CRIOManager/crReadFile.h>
#include <CREncapsulation/crLoader.h>
#include <CRParticle/crParticleEffect.h>
#include <CRUtil/crUpdateVisitor.h>
#include <string>

namespace CREncapsulation{

class crCullAndUpdThread;
class crLoadingDrawThread; 

class CRENCAPSULATION_EXPORT crStartLoader : public CRCore::Referenced
{
public:
	virtual void loading(CRProducer::crViewer *viewer,Producer::Window parentWnd,Producer::Window renderWnd);
};

class CRENCAPSULATION_EXPORT crStartHandler : public CRCore::Referenced
{
public:
	crStartHandler():m_endcode(0){ m_loadingViewer = new CRProducer::crViewer; }
	
    static crStartHandler *getInstance();

	void setLoader(crStartLoader *loader);
	//void setViewer(CRProducer::crViewer* viewer);
	void start(unsigned int options1, unsigned int options2, Producer::Window parentWnd = 0, Producer::Window renderWnd = 0);
	void run(/*CRGUI::crImageStage *main = NULL*/);
	void beginloading();//显示loading画面
	void endloading();//结束loading画面

	void startCullAndUpdThread();
	void startDrawThread();
	void end();

	static bool changeDisplaySettings(int width, int height, int colorbit = 32);
	virtual void clear();

	inline void setEndCode(int code){ m_endcode = code; }
	inline int getEndCode() { return m_endcode; }
	inline bool isRunning(){ return m_running; }
	Producer::Window getWindowHandle(){ return m_viewer.valid()?m_viewer->getCameraWindow():NULL; }
	CRProducer::crViewer *getViewer(){ return m_viewer.get(); }
	void doMessaging();
protected:
	~crStartHandler();
	static CRCore::ref_ptr<crStartHandler> m_instance;

	CRCore::ref_ptr<crStartLoader> m_loader;
	CRCore::ref_ptr<CRProducer::crViewer> m_viewer;
	CRCore::ref_ptr<crCullAndUpdThread>    m_cullAndUpdThread;
	CRCore::ref_ptr<crLoadingDrawThread>    m_loadindDrawThread;
    CRCore::ref_ptr<CRProducer::crViewer> m_loadingViewer;
    int m_endcode;//0表示切换场景，1表示退出游戏
	bool m_running;
};

class CRENCAPSULATION_EXPORT crImageDataRequestClass : public CRCore::ImageDataRequestHandler
{
public:
	inline virtual CRCore::crImage *requestImageFile(const std::string& fileName, CRCore::crTexture *texture)
	{
		if(!texture) return false;
		CRCore::crImage *loadedimg = CRIOManager::readImageFile(fileName);
		if(CRCore::gCredefimage() == loadedimg)
		{
			CRCore::crBrain::getInstance()->pushLoadFailedTexture(texture);
		}
		texture->setImageNameID(CRCore::crArgumentParser::getSimpleFileName(fileName));
		return loadedimg;
	}
	inline virtual bool requestCubeImageFile(const std::string& cubeImageNameID, CRCore::crTextureCubeMap *textureCubeMap)
	{
		if(!textureCubeMap) return false;
		std::string ext = CRCore::crArgumentParser::getFileExtension(cubeImageNameID);
		std::string fileName = CRCore::crArgumentParser::getFileNameEliminateExt(cubeImageNameID);

		bool faild = false;
		CRCore::crImage* imagePosX = CRIOManager::readImageFile(CUBEMAP_FILENAME(fileName,posx,ext));
		if(CRCore::gCredefimage() == imagePosX)
		{
			faild = true;
		}
		CRCore::crImage* imageNegX = CRIOManager::readImageFile(CUBEMAP_FILENAME(fileName,negx,ext));
		if(CRCore::gCredefimage() == imageNegX)
		{
			faild = true;
		}
		CRCore::crImage* imagePosY = CRIOManager::readImageFile(CUBEMAP_FILENAME(fileName,posy,ext));
		if(CRCore::gCredefimage() == imagePosY)
		{
			faild = true;
		}
		CRCore::crImage* imageNegY = CRIOManager::readImageFile(CUBEMAP_FILENAME(fileName,negy,ext));
		if(CRCore::gCredefimage() == imageNegY)
		{
			faild = true;
		}
		CRCore::crImage* imagePosZ = CRIOManager::readImageFile(CUBEMAP_FILENAME(fileName,posz,ext));
		if(CRCore::gCredefimage() == imagePosZ)
		{
			faild = true;
		}
		CRCore::crImage* imageNegZ = CRIOManager::readImageFile(CUBEMAP_FILENAME(fileName,negz,ext));
		if(CRCore::gCredefimage() == imageNegZ)
		{
			faild = true;
		}
		if(faild)
		{
			CRCore::crBrain::getInstance()->pushLoadFailedTexture(textureCubeMap);
		}
		if (imagePosX && imageNegX && imagePosY && imageNegY && imagePosZ && imageNegZ /*&& object*/)
		{
			//imagePosX->flipVertical();
			//imageNegX->flipVertical();
			//imagePosY->flipVertical();
			//imageNegY->flipVertical();
			//imagePosZ->flipVertical();
			//imageNegZ->flipVertical();

			textureCubeMap->setImage(CRCore::crTextureCubeMap::POSITIVE_X, imagePosX);
			textureCubeMap->setImage(CRCore::crTextureCubeMap::NEGATIVE_X, imageNegX);
			textureCubeMap->setImage(CRCore::crTextureCubeMap::POSITIVE_Y, imagePosY);
			textureCubeMap->setImage(CRCore::crTextureCubeMap::NEGATIVE_Y, imageNegY);
			textureCubeMap->setImage(CRCore::crTextureCubeMap::POSITIVE_Z, imagePosZ);
			textureCubeMap->setImage(CRCore::crTextureCubeMap::NEGATIVE_Z, imageNegZ);

			textureCubeMap->setImageNameID(CRCore::crArgumentParser::getSimpleFileName(cubeImageNameID));
			return true;
		}
		return false;
	}

protected:
	virtual ~crImageDataRequestClass() {}
};

class CRENCAPSULATION_EXPORT crLoadNodeCfgClass : public CRParticle::LoadNodeCfgHandler
{
public:            
	inline virtual void loadCfgFile(const std::string &cfgFile, CRCore::crNode *node)
	{
		CREncapsulation::loadCfgFile(cfgFile,node);
	}
protected:
	virtual ~crLoadNodeCfgClass() {}
};

class crCullAndUpdThread : public CRCore::crThread, public CRCore::Referenced
{
public:
	crCullAndUpdThread(CRProducer::crViewer *viewer):m_viewer(viewer),m_done(false){};
	virtual void run();
	void setDone(bool done){ m_done = done; }
	bool done(){ return m_done; }
protected:
	CRProducer::crViewer* m_viewer;
	bool m_done;
};

class crLoadingDrawThread : public CRCore::crThread, public CRCore::Referenced
{
public:
	crLoadingDrawThread(CRProducer::crViewer *viewer):m_loadingViewer(viewer),m_done(false){};
	virtual void run();
	void done(){ m_done = true; }
protected:
	CRCore::ref_ptr<CRProducer::crViewer> m_loadingViewer;
	bool m_done;
};
}
#endif