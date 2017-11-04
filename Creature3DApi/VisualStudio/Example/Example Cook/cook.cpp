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
#include <CRNet/crStreamPacket.h>
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <vector>
#include <string>
#include<conio.h>
#include<commdlg.h>
using namespace CRCore;

bool checkExt(const std::string &filename, const std::string &ext)
{
	//std::string fileExt;
	//size_t lastpoint = filename.find_last_of('.');
	//if(lastpoint!= -1)
	//{
	//	fileExt = filename.substr(lastpoint+1,filename.length());
	//}
	std::string fileExt = CRIOManager::getLowerCaseFileExtension(filename);
	return ext.find(fileExt)!=std::string::npos;
}

void findFileInDir(const char* rootDir, const std::string &ext, std::vector<std::string>& fileNameVec)
{
	std::string rdir = rootDir;
	if(rdir.find(".svn") !=  std::string::npos || rdir.find("Des\\") !=  std::string::npos || rdir.find("scriptsrc\\") !=  std::string::npos)
		return;
	char fname[1024];
	ZeroMemory(fname, 1024);

	WIN32_FIND_DATA fd;
	ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));

	HANDLE hSearch;

	char filePathName[1024];
	char tmpPath[1024];
	ZeroMemory(filePathName, 1024);
	ZeroMemory(tmpPath, 1024);

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
		findFileInDir(tmpPath, ext, fileNameVec);
	}
	else if( strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") && checkExt(fd.cFileName,ext) )
	{
		if( rootDir[strlen(rootDir) -1] != '\\' )
			sprintf(fname, "%s\\%s\0",rootDir,fd.cFileName);
		else
			sprintf(fname, "%s%s\0",rootDir,fd.cFileName);
		fileNameVec.push_back(fname);
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
				findFileInDir(tmpPath, ext, fileNameVec);
			}
			else if( strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") && checkExt(fd.cFileName,ext) )
			{
				if( rootDir[strlen(rootDir) -1] != '\\' )
					sprintf(fname, "%s\\%s\0",rootDir,fd.cFileName);
				else
					sprintf(fname, "%s%s\0",rootDir,fd.cFileName);
				fileNameVec.push_back(fname);
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
std::string getOpenFileName()
{
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[1024];       // buffer for file name

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ZeroMemory(szFile, sizeof(szFile));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All(*.*)\0*.*\0";
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
	char szFile[1024];       // buffer for file name

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ZeroMemory(szFile, sizeof(szFile));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All(*.*)\0*.*\0";
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
	if(strcmp(argv[1],"UnCook") == 0)
	{
		char programDir[1024];
		GetCurrentDirectory(1024, programDir);

		std::string filePath = programDir;
		CRCore::notify(CRCore::WARN)<<"Start UnCook "<<filePath<<std::endl;
		if( filePath[filePath.length() -1] != '\\' )
			filePath.push_back('\\');
		std::vector<std::string> fileNameVec;
		findFileInDir(filePath.c_str(),"cook",fileNameVec);

		CRIOManager::SetCooked(true);
		CRIOManager::SetRetainSourceFile(true);
		std::string file;
		char buf[256];
		std::string id = crArgumentParser::appItoa(GetCurrentProcessId());
		std::string cookfilename;
		for( std::vector<std::string>::iterator itr = fileNameVec.begin();
			itr != fileNameVec.end();
			++itr )
		{
			file = *itr;
			CRCore::notify(CRCore::WARN)<<"UnCookFile "<<file<<std::endl;
			file = CRCore::crArgumentParser::getFileNameEliminateExt(file)+"."+argv[2];
			CRIOManager::crLoadCookFile scopedLoad(file.c_str());
			cookfilename = CRCore::crArgumentParser::getFileNameEliminateExt(file) + "." + argv[2] + id;
			sprintf(buf, "ren %s %s\0", cookfilename.c_str(), file.c_str());
			system(buf);
		}
		CRCore::notify(CRCore::WARN)<<"UnCooked "<<filePath<<std::endl;

		system("pause");
		for( std::vector<std::string>::iterator itr = fileNameVec.begin();
			itr != fileNameVec.end();
			++itr )
		{
			file = *itr;
			file = CRCore::crArgumentParser::getFileNameEliminateExt(file)+"."+argv[2];
			CRCore::notify(CRCore::WARN)<<"DeleteFile "<<file<<std::endl;
		    DeleteFileA(file.c_str());
		}
	}
	else if(strcmp(argv[1],"Cook") == 0)
	{
		if(MessageBox(::GetActiveWindow(), "警告：运行COOK，请确认文件已经备份!", "Creature3D Cook", MB_OKCANCEL) == IDOK)
		{
			CRIOManager::SetCooked(true);
			CRIOManager::SetRetainSourceFile(false);
			char programDir[1024];
			GetCurrentDirectory(1024, programDir);

			std::string filePath = programDir;
			CRCore::notify(CRCore::WARN)<<"Start Cook "<<filePath<<std::endl;
			if( filePath[filePath.length() -1] != '\\' )
				filePath.push_back('\\');
			std::vector<std::string> fileNameVec;
			findFileInDir(filePath.c_str(),argv[2]/*"cfg|material|save"*/,fileNameVec);//cfg$material$save

			std::string file;
			for( std::vector<std::string>::iterator itr = fileNameVec.begin();
				itr != fileNameVec.end();
				++itr )
			{
				file = *itr;
				if(file.find("dynamic.crb")!=std::string::npos || file.find("startup.crb")!=std::string::npos)
					continue;
				CRCore::notify(CRCore::WARN)<<"CookFile "<<file<<std::endl;
				CRIOManager::crWriteCookFile scopedWrite(file);
			}
			CRCore::notify(CRCore::WARN)<<"Cooked "<<filePath<<std::endl;
		}
		system("pause");
	}
	else if(strcmp(argv[1],"ConvertTab") == 0)
	{
		if(MessageBox(::GetActiveWindow(),"警告：运行ConvertTabFile，请确认文件已经备份!","Creature3D Cook" ,MB_OKCANCEL)==IDOK)
		{
			char programDir[1024];
			GetCurrentDirectory(1024, programDir);

			std::string filePath = programDir;
			CRCore::notify(CRCore::WARN)<<"Start ConvertTabFile "<<filePath<<std::endl;
			if( filePath[filePath.length() -1] != '\\' )
				filePath.push_back('\\');
			std::vector<std::string> fileNameVec;
			findFileInDir(filePath.c_str(),"tab",fileNameVec);

			bool del = false;
			if(argv[2] && strcmp(argv[2],"Del") == 0)
				del = true;

			std::string file;
			std::string sfilename;
			ref_ptr<CREncapsulation::crTableIO> tabio = new CREncapsulation::crTableIO;
			for( std::vector<std::string>::iterator itr = fileNameVec.begin();
				itr != fileNameVec.end();
				++itr )
			{
				file = *itr;
				sfilename = crArgumentParser::getSimpleFileName(file);
				if(sfilename.compare("gm.tab")==0/* || sfilename.compare("dynamic.tab")==0 || sfilename.compare("startup.tab")==0*/)
					continue;
				CRCore::notify(CRCore::WARN)<<"ConvertTabFile "<<file<<std::endl;
				tabio = CREncapsulation::crTableIO::openFile(file);
				tabio->saveToFileStream(crArgumentParser::getFileNameEliminateExt(file)+".crb");
				CRCore::notify(CRCore::WARN)<<"DeleteFile "<<file<<std::endl;
				if(del) DeleteFileA(file.c_str());
			}
			CRCore::notify(CRCore::WARN)<<"ConvertTabFile End "<<filePath<<std::endl;
		}
		system("pause");
	}
	else if(strcmp(argv[1],"ConvertCrb") == 0)
	{
		if (MessageBox(::GetActiveWindow(), "警告：运行ConvertCrbFile，请确认文件已经备份!", "Creature3D Cook", MB_OKCANCEL) == IDOK)
		{
			char programDir[1024];
			GetCurrentDirectory(1024, programDir);

			std::string filePath = programDir;
			CRCore::notify(CRCore::WARN)<<"Start ConvertCrbFile "<<filePath<<std::endl;
			if( filePath[filePath.length() -1] != '\\' )
				filePath.push_back('\\');
			std::vector<std::string> fileNameVec;
			findFileInDir(filePath.c_str(),"crb",fileNameVec);

			bool del = false;
			if(argv[2] && strcmp(argv[2],"Del") == 0)
				del = true;

			std::string file;
			std::string sfilename;
			ref_ptr<CREncapsulation::crTableIO> tabio = new CREncapsulation::crTableIO;
			for( std::vector<std::string>::iterator itr = fileNameVec.begin();
				itr != fileNameVec.end();
				++itr )
			{
				file = *itr;
				//sfilename = crArgumentParser::getSimpleFileName(file);
				//if(sfilename.compare("gm.tab")==0/* || sfilename.compare("dynamic.tab")==0 || sfilename.compare("startup.tab")==0*/)
				//	continue;
				CRCore::notify(CRCore::WARN)<<"ConvertCrbFile "<<file<<std::endl;
				tabio = CREncapsulation::crTableIO::openFile(file);
				tabio->saveToFileNoCook(crArgumentParser::getFileNameEliminateExt(file)+".tab");
				CRCore::notify(CRCore::WARN)<<"DeleteFile "<<file<<std::endl;
				if(del) DeleteFileA(file.c_str());
			}
			CRCore::notify(CRCore::WARN)<<"ConvertCrbFile End "<<filePath<<std::endl;
		}
		system("pause");
	}
	else if(strcmp(argv[1],"UnCompress") == 0)
	{
		std::string fileName = getOpenFileName();
		if(fileName.empty())
			return 0;
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->loadFromFile2(fileName);
		CRNet::creUnCompress(stream);
		fileName = getSaveFileName();
		if(fileName.empty())
			return 0;
		stream->saveToFile2(fileName);
	}
	else if(strcmp(argv[1],"Compress") == 0)
	{
		std::string fileName = getOpenFileName();
		if(fileName.empty())
			return 0;
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->loadFromFile2(fileName);
		stream->seekBegin();
		CRNet::creCompress(stream);
		fileName = getSaveFileName();
		if(fileName.empty())
			return 0;
		stream->saveToFile2(fileName);
	}
	return 0;
}

