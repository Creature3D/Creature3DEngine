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
#include <vector>
#include <string>
#include<conio.h>
#include <CRNet/crStreamPacket.h>
using namespace CRCore;

char checkExt(const std::string &filename, const std::string &ext)
{//0无效，1动态下载，2启动更新
	//std::string fileExt;
	//size_t lastpoint = filename.find_last_of('.');
	//if(lastpoint!= -1)
	//{
	//	fileExt = filename.substr(lastpoint+1,filename.length());
	//}
	std::string fileExt = CRIOManager::getLowerCaseFileExtension(filename);
	if( fileExt.compare("tab")==0 || fileExt.compare("txt")==0 || fileExt.compare("log")==0 || fileExt.compare("des")==0
		|| fileExt.compare("jpg")==0 || fileExt.compare("tga")==0 || fileExt.compare("png")==0 || fileExt.compare("ini")==0
		|| fileExt.compare("cmd")==0
		|| fileExt.compare("db")==0 )
		return 0;
	if(filename.find("BuildFileUpdateList.exe") != std::string::npos)
		return 0;
	if(filename.find("BuildMiniClientTab.exe") != std::string::npos)
		return 0;
	if(filename.find("BuildPriDynamicTab.exe") != std::string::npos)
		return 0;
	if(filename.find("dynamic.crb") != std::string::npos)
		return 0;
	if(filename.find("startup.crb") != std::string::npos)
		return 0;
	if(filename.find("version.crb") != std::string::npos)
		return 0;
	if(ext.find(fileExt)!=std::string::npos)
		return 1;
	return 2;
}

void findFileInDir(const char* rootDir, int rootlength, const std::string &ext, std::vector<std::string>& startupFileVec,std::vector<std::string>& fileVec)
{
	std::string rdir = rootDir;
	if(rdir.find("textures\\Engine") !=  std::string::npos)
		return;
	char fname[256];
	ZeroMemory(fname, 256);

	WIN32_FIND_DATA fd;
	ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));

	HANDLE hSearch;
	char extcheck;
	char filePathName[256];
	char tmpPath[256];
	ZeroMemory(filePathName, 256);
	ZeroMemory(tmpPath, 256);

	strcpy(filePathName, rootDir);

	bool bSearchFinished = FALSE;

	if( filePathName[strlen(filePathName) -1] != '\\' )
	{
		strcat(filePathName, "\\");
	}
	strcat(filePathName, "*");
	hSearch = FindFirstFile(filePathName, &fd);

	if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		&& strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") )
	{
		strcpy(tmpPath, rootDir);
		strcat(tmpPath, fd.cFileName);
		strcat(tmpPath, "\\");
		findFileInDir(tmpPath, rootlength, ext, startupFileVec, fileVec);
	}
	else if( strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") )
	{
		if( rootDir[strlen(rootDir) -1] != '\\' )
			sprintf(fname, "%s\\%s\0",rootDir,fd.cFileName);
		else
			sprintf(fname, "%s%s\0",rootDir,fd.cFileName);

		extcheck = checkExt(fd.cFileName,ext);
		if(extcheck == 1)
			fileVec.push_back(CRIOManager::convertFileNameToUnixStyle(std::string(fname+rootlength)));
		else if(extcheck == 2)
			startupFileVec.push_back(CRIOManager::convertFileNameToUnixStyle(std::string(fname+rootlength)));
	}

	while( !bSearchFinished )
	{
		if( FindNextFile(hSearch, &fd) )
		{
			if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") )       
			{
				strcpy(tmpPath, rootDir);
				strcat(tmpPath, fd.cFileName);
				strcat(tmpPath, "\\");
				findFileInDir(tmpPath, rootlength, ext, startupFileVec, fileVec);
			}
			else if( strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") )
			{
				if( rootDir[strlen(rootDir) -1] != '\\' )
					sprintf(fname, "%s\\%s\0",rootDir,fd.cFileName);
				else
					sprintf(fname, "%s%s\0",rootDir,fd.cFileName);
				extcheck = checkExt(fd.cFileName,ext);
				if(extcheck == 1)
					fileVec.push_back(CRIOManager::convertFileNameToUnixStyle(std::string(fname+rootlength)));
				else if(extcheck == 2)
					startupFileVec.push_back(CRIOManager::convertFileNameToUnixStyle(std::string(fname+rootlength)));
			}
		}
		else
		{
			if( GetLastError() == ERROR_NO_MORE_FILES )
			{
				bSearchFinished = TRUE;
			}
			else
				bSearchFinished = TRUE;
		}
	}
	FindClose(hSearch);
}
void compressFile(const std::string &file,const std::string &filedir)
{//
	if(!file.empty())
	{
		ref_ptr<crStreamBuf>stream = new crStreamBuf;
		if(!stream->loadFromFile2(file))
		{
			CRCore::notify(CRCore::ALWAYS)<<"文件加载失败："<<file<<std::endl;
		}
		else
		{
			CRNet::creCompress(stream);
			std::string pkgfile = "../"+filedir+"/"+file+".pkg";
			pkgfile = CRIOManager::convertFileNameToUnixStyle(pkgfile);
			CRIOManager::makeDirectoryForFile(pkgfile);
			stream->seekBegin();
			if(stream->saveToFile2(pkgfile))
				CRCore::notify(CRCore::ALWAYS)<<"输出PKG文件："<<pkgfile<<std::endl;
			else
				CRCore::notify(CRCore::ALWAYS)<<"文件输出失败："<<pkgfile<<std::endl;
		}
	}
}
int main( int argc, char **argv )
{
	char programDir[256];
	GetCurrentDirectory(256,programDir);

	std::string newPublish = programDir;
	if( newPublish[newPublish.length() -1] != '\\' )
		newPublish.push_back('\\');
	CRCore::notify(CRCore::WARN)<<"将要发布微端版本路径："<<newPublish<<std::endl;
	std::string lastPublish = argv[1];
	if( lastPublish[lastPublish.length() -1] != '\\' )
		lastPublish.push_back('\\');
	CRCore::notify(CRCore::WARN)<<"上一次版本路径："<<lastPublish<<std::endl;
	
	std::string filedir = argv[3];

	std::vector<std::string>::iterator itr1;
	CREncapsulation::crTableIO::StrVec title;
	title.push_back("file");
	title.push_back("version");
	CREncapsulation::crTableIO::StrVec qrecord;
	{
		ref_ptr<CREncapsulation::crTableIO> priDynamictab = new CREncapsulation::crTableIO;
		ref_ptr<CREncapsulation::crTableIO> dynamictab = CREncapsulation::crTableIO::openFile(newPublish+"dynamic.crb");
		ref_ptr<CREncapsulation::crTableIO> dynamicRectab = CREncapsulation::crTableIO::openFile(newPublish+"dynamicRec.tab");
		priDynamictab->setTitleVec(title);
		int rowcount = dynamicRectab->getRowCount();
		std::string filename;
		int row = 0;
		for(int i = 0; i<rowcount; ++i)
		{
			filename = (*dynamicRectab)(i,0);
			row = dynamictab->queryOneRecord(0,filename,qrecord);
			if(row>=0)
			{
				priDynamictab->addData(qrecord);
				dynamictab->removeRow(row);
			}
		}
		rowcount = dynamictab->getRowCount();
		for(int i = 0; i<rowcount; ++i)
		{
			priDynamictab->addData(dynamictab->getDataVec()[i]);
		}
		priDynamictab->saveToFileNoCook("dynamic.tab");//test
		priDynamictab->saveToFileStreamNoCook("dynamic.crb");
		compressFile("dynamic.crb",filedir);
	}
	system("pause");
	return 0;
}

