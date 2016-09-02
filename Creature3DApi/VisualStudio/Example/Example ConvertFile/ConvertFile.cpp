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
#include <creature3d.h>
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include<conio.h>
using namespace CRCore;

std::string getOpenFileName()
{
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[256];       // buffer for file name

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ZeroMemory(szFile, sizeof(szFile));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Flt(.flt)\0*.flt\0All(*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER;

	// Display the Open dialog box. 
	if (GetOpenFileName(&ofn)==TRUE) 
	{
		return ofn.lpstrFile;
	}

	return " ";
}

std::string getSaveFileName()
{
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[256];       // buffer for file name

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ZeroMemory(szFile, sizeof(szFile));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "CRS(.crs)\0*.crs\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER;

	// Display the Open dialog box. 
	if (GetSaveFileName(&ofn)==TRUE) 
	{
		return ofn.lpstrFile;
	}

	return " ";
}

int main( int argc, char **argv )
{
	//_putenv( "CRE_LIBRARY_PATH=./lib/" );
	_putenv( "CRE_WRITEIMAGE_PATH=./textures/" );
	std::string fileName = getOpenFileName();

	if(fileName.compare(" ") == 0)
		return 1;

	std::string filePath = CRIOManager::getFilePath(fileName);
	std::string pushAndPopPath;
	CRIOManager::FilePathList fpl = CRIOManager::getDataFilePathList();
	//If absolute path
	if( (filePath.length()>0 && filePath.find_first_of("/\\")==0) ||
		(filePath.length()>2 && filePath.substr(1,1)==":" && filePath.find_first_of("/\\")==2) )
	{
		pushAndPopPath = filePath;
	}
	else
	{
		pushAndPopPath = fpl.empty() ? "." : fpl.front();
		if(pushAndPopPath.empty()) pushAndPopPath = ".";
		pushAndPopPath += "/" + filePath;
	}

	fpl.push_back(pushAndPopPath);

	CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();
    CRCore::notify(CRCore::ALWAYS)<<"Loading..."<< std::endl;

	ref_ptr<CRCore::crDB> pDB = new CRCore::crDB;
	pDB->setName("DB");
	CRCore::crBrain::getInstance()->insertDB(pDB.get());
	CRCore::crBrain::getInstance()->setCurrentActiveDB(CRCore::crBrain::getInstance()->getDBCount()-1);
	
	CRCore::ref_ptr<CRCore::crNode> node = CRIOManager::readNodeFile(fileName);

    if (!node.valid()) 
	{
	  //std::cout << arguments.getApplicationName() <<": No data loaded" << std::endl;
	    return 1;
	}
    
	pDB->addChild(node.get());

	//CRCore::ref_ptr<CRAL::crSoundRoot> sound_root = new CRAL::crSoundRoot;
	//pDB->addChild(sound_root.get());

    CRCore::Timer_t end_tick = CRCore::Timer::instance()->tick();
    //std::cout << "Time to load = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
	CRCore::notify(CRCore::ALWAYS)<<"Time to load = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;

	CRCore::notify(CRCore::ALWAYS)<<"Now Optimizing... "<< std::endl;
	start_tick = CRCore::Timer::instance()->tick();
	
	CRUtil::crOptimizer optimizer;
	optimizer.optimize(node.get(),CRUtil::crOptimizer::DEFAULT_OPTIMIZATIONS/*|CRUtil::crOptimizer::TRISTRIP_GEOMETRY*/);

	//CRUtil::crSimplifier simplifier(0.01,0.0);
	//node->accept(simplifier);

	//CRUtil::crSmoothingVisitor smoother;
	//node->accept(smoother);

    //在此加入对BumpMap的支持及特效渲染数据的准备
    CRUtil::crPrepareVisitor prepareVisitor;
	node->accept(prepareVisitor);
//    CRCore::crBrain::getInstance()->releaseResource();
  
	end_tick = CRCore::Timer::instance()->tick();
	CRCore::notify(CRCore::ALWAYS)<<"Time to Optimize = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
    
	//AITreeVisitor aiv(pDB.get());
	//CRCore::crBrain::getInstance()->getAITreeHead()->accept(aiv);

	//AttachAIGroupVisitor attachAI;
	//CRCore::crBrain::getInstance()->accept(attachAI);

	fileName = getSaveFileName();

	start_tick = CRCore::Timer::instance()->tick();
	CRCore::notify(CRCore::ALWAYS)<<"Write file..."<< std::endl;
	if(fileName.compare(" ") == 0)
		return 1;
	CRIOManager::writeObjectFile((*crBrain::getInstance()),fileName);
	end_tick = CRCore::Timer::instance()->tick();
	CRCore::notify(CRCore::ALWAYS)<<"Time to Write = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
    
	return 0;
}

