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

	return "";
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
	ofn.lpstrFilter = "CRE(.cre)\0*.cre\0";
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

	return "";
}

int main( int argc, char **argv )
{
	int lparam = LOINT64(1376272);
	int hparam = HIINT64(1376272);
	_crInt16 llp = LOINT32(lparam);
	_crInt16 hlp = HIINT32(lparam);
	_putenv( "CRE_WRITEIMAGE_PATH=./textures/" );
    CRIOManager::makeDirectory("./textures/");
	std::string fileName = getOpenFileName();
    
	if(fileName.empty())
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
	
	CRCore::ref_ptr<CRCore::crNode> node = CRIOManager::readNodeFile(fileName);

    if (!node.valid()) 
	{
	  //std::cout << arguments.getApplicationName() <<": No data loaded" << std::endl;
	    return 1;
	}

    CRCore::Timer_t end_tick = CRCore::Timer::instance()->tick();
    //std::cout << "Time to load = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
	CRCore::notify(CRCore::ALWAYS)<<"Time to load = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;

	CRCore::notify(CRCore::ALWAYS)<<"Now Optimizing... "<< std::endl;
	start_tick = CRCore::Timer::instance()->tick();

	//CRUtil::crSimplifier simplifier;
	//node->accept(simplifier);

	CRUtil::crOptimizer optimizer;
	optimizer.optimize(node.get(),CRUtil::crOptimizer::DEFAULT_OPTIMIZATIONS);
	//CRUtil::crSmoothingVisitor smoother;
	//node->accept(smoother);

    //在此加入对BumpMap的支持及特效渲染数据的准备
	CRUtil::crPrepareVisitor prepareVisitor;
	node->accept(prepareVisitor);
  
	end_tick = CRCore::Timer::instance()->tick();
	CRCore::notify(CRCore::ALWAYS)<<"Time to Optimize = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;

	fileName = getSaveFileName();
	start_tick = CRCore::Timer::instance()->tick();
	CRCore::notify(CRCore::ALWAYS)<<"Write file..."<< std::endl;
	if(fileName.empty())
		return 1;
	CRIOManager::writeNodeFile(*node,fileName);
	end_tick = CRCore::Timer::instance()->tick();
	CRCore::notify(CRCore::ALWAYS)<<"Time to Write = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;

	return 0;
}

