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
#include <CRIOManager/crFileNameUtils.h>
using namespace CRCore;
using namespace CRIOManager;

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
		|| fileExt.compare("db")==0
		|| fileExt.compare("bat")==0 )
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
void collectStartupFile(const std::string &file,const std::string &filedir,const std::string &strver)
{//
	if(!file.empty())
	{
		std::string pkgfile;
		if(file.find("bin")==0)
		{
			pkgfile = "../"+filedir+"/"+strver+"/jxjpkg/"+file;
		}
		else
		{
			pkgfile = "../"+filedir+"/"+strver+"/"+file;
		}
		CRIOManager::copyFile(file,pkgfile);
	}
}
int readVersion(const std::string &filename)
{
	std::ifstream fin(filename.c_str(), std::ios_base::in|std::ios::binary);
	if(!fin)
		return 0;
	int ver = 0;
	fin.seekg(0, std::ios::end); 
	int count  =  fin.tellg();
	fin.seekg(0, std::ios::beg);
	char *buf = new char[count+1];
	memset(buf,0,count+1);
	fin.read(buf,count);
	crArgumentParser::readKeyValue(buf,"version",ver);
	fin.close();
	delete [] buf;
	return ver;
}
int main( int argc, char **argv )
{
	char programDir[256];
	GetCurrentDirectory(256,programDir);

	std::string newPublish = programDir;
	if( newPublish[newPublish.length() -1] != '\\' )
		newPublish.push_back('\\');
	CRCore::notify(CRCore::WARN)<<"将要发布版本路径："<<newPublish<<std::endl;
	std::string lastPublish = argv[1];
	if( lastPublish[lastPublish.length() -1] != '\\' )
		lastPublish.push_back('\\');
	CRCore::notify(CRCore::WARN)<<"上一次版本路径："<<lastPublish<<std::endl;

	std::vector<std::string> newStartupFileVec,newFileVec;
	//std::vector<std::string> lastStartupFileVec,lastFileVec;
	findFileInDir(newPublish.c_str(),newPublish.size(),argv[2]/*"cre|img"*/,newStartupFileVec,newFileVec);
	//findFileInDir(lastPublish.c_str(),lastPublish.size(),argv[2]/*"cre|img"*/,lastStartupFileVec,lastFileVec);
	
	std::string filedir = argv[3];
	std::string argvstr;
	for( int i = 0; i<argc; i++)
	{
		argvstr += argv[i];
		argvstr += " ";
	}
	bool patch = false;
	//if(argvstr.find("-patch")!=std::string::npos)
	//	patch = true;
	std::vector<std::string>::iterator itr1,itr2;
	bool found,same;
	CREncapsulation::crTableIO::StrVec title;
	title.push_back("file");
	title.push_back("version");
	CREncapsulation::crTableIO::StrVec record;
	record.resize(2);
	CREncapsulation::crTableIO::StrVec qrecord;
	char *buf1,*buf2;
	int ver,count1,count2;
	ref_ptr<crStreamBuf> stream;
	std::string strVersion = crArgumentParser::appItoa(readVersion("jxjversion.ini"));
	{
		ref_ptr<CREncapsulation::crTableIO> startuptab = new CREncapsulation::crTableIO;
		ref_ptr<CREncapsulation::crTableIO> lastStartuptab;// = CREncapsulation::crTableIO::openFile(lastPublish+"startup.crb");
		ref_ptr<CREncapsulation::crTableIO> versiontab;// = CREncapsulation::crTableIO::openFile(lastPublish+"version.crb");
		if(patch)
		{
			//lastStartuptab = CREncapsulation::crTableIO::openFile(lastPublish+"startup.crb");
			versiontab = CREncapsulation::crTableIO::openFile(lastPublish+"version.crb");
		}
		if(!versiontab.valid())
		{
			versiontab = new CREncapsulation::crTableIO;
			CREncapsulation::crTableIO::StrVec title1;
			title1.push_back("version");
			title1.push_back("file");
			versiontab->setTitleVec(title1);
		}
		bool changed = false;
		bool laststartup = lastStartuptab.valid();
		startuptab->setTitleVec(title);
		std::string fontext;
		for( itr1 = newStartupFileVec.begin();
			itr1 != newStartupFileVec.end();
			++itr1 )
		{
			fontext = crArgumentParser::getFileExtension(*itr1);
			if( CRIOManager::equalCaseInsensitive(fontext,"ttf") ||  // true type
				CRIOManager::equalCaseInsensitive(fontext,"ttc") ||  // true type
				CRIOManager::equalCaseInsensitive(fontext,"pfb") ||  // type1 binary
				CRIOManager::equalCaseInsensitive(fontext,"pfa") ||  // type2 ascii
				CRIOManager::equalCaseInsensitive(fontext,"cid") ||  // Postscript CID-Fonts
				CRIOManager::equalCaseInsensitive(fontext,"cff") ||  // OpenType
				CRIOManager::equalCaseInsensitive(fontext,"cef") ||  // OpenType
				CRIOManager::equalCaseInsensitive(fontext,"fon") ||  // Windows bitmap fonts
				CRIOManager::equalCaseInsensitive(fontext,"fnt") )
			{
				compressFile(*itr1,filedir);
				continue;
			}
			if(CRIOManager::equalCaseInsensitive(*itr1,"script/init.cook"))
				continue;
			//found = false;
			//if(patch)
			//{
				//for( itr2 = lastStartupFileVec.begin();
				//	itr2 != lastStartupFileVec.end();
				//	++itr2 )
				//{
				//	if(*itr1 == *itr2)
				//	{
				//		found = true;
				//		break;
				//	}
				//}
			//}
			//if(!found)
			{//新加
				stream = new crStreamBuf;
				stream->loadFromFile2((newPublish+*itr1).c_str());
				record[0] = *itr1;
				record[1] = stream->getMD5();
				startuptab->addData(record);
				//compressFile(*itr1,filedir);
				collectStartupFile(*itr1,filedir,strVersion);
				changed = true;
			}
			//else
			//{//文件比较
			//	same = true;
			//	std::ifstream f1((newPublish+*itr1).c_str(), std::ios::in | std::ios::binary);
			//	f1.seekg(0, std::ios::end); 
			//	count1  =  f1.tellg();
			//	f1.seekg(0, std::ios::beg);
			//	std::ifstream f2((lastPublish+*itr2).c_str(), std::ios::in | std::ios::binary);
			//	f2.seekg(0, std::ios::end); 
			//	count2  =  f2.tellg();
			//	f2.seekg(0, std::ios::beg);
			//	do 
			//	{
			//		if(count1 != count2)
			//		{
			//			same = false;
			//			break;
			//		}
			//		buf1 = new char[count1];
			//		buf2 = new char[count1];
			//		memset(buf1,0,count1);
			//		memset(buf2,0,count1);
			//		f1.read(buf1,count1);
			//		f2.read(buf2,count1);
			//		if(memcmp(buf1,buf2,count1) != 0)
			//		{
			//			same = false;
			//		}
			//		delete [] buf1;
			//		delete [] buf2;
			//	} while (0);
			//	if(!same)
			//	{//
			//		ver = 0;
			//		if(laststartup)
			//		{
			//			if(lastStartuptab->queryOneRecord(0,*itr1,qrecord)>=0)
			//			{
			//				ver = atoi(qrecord[1].c_str());
			//				ver++;
			//			}
			//		}
			//		record[0] = *itr1;
			//		record[1] = crArgumentParser::appItoa(ver);
			//		stream = new crStreamBuf;
			//		stream->loadFromFile2((newPublish+*itr1).c_str());
			//		record[2] = stream->getMD5();
			//		startuptab->addData(record);
			//		//compressFile(*itr1,filedir);
			//		collectStartupFile(*itr1,filedir,strVersion);
			//		changed = true;
			//	}
			//	else
			//	{
			//		ver = 0;
			//		if(laststartup)
			//		{
			//			if(lastStartuptab->queryOneRecord(0,*itr1,qrecord)>=0)
			//			{
			//				//ver = atoi(qrecord[1].c_str());
			//				//record[0] = *itr1;
			//				//record[1] = crArgumentParser::appItoa(ver);
			//				startuptab->addData(qrecord);
			//			}
			//		}
			//	}
			//}
		}
		startuptab->saveToFileStreamNoCook("startup.crb");
		//compressFile("startup.crb",filedir);
		//兼容version:2014022100以前的版本的自动更新
		//startuptab->saveToFile("startup.tab");
		//compressFile("startup.tab",filedir);
		if(changed)
		{
			//collectStartupFile("startup.crb",filedir,strVersion);
			//collectStartupFile("jxjversion.ini",filedir,strVersion);
			record[0] = strVersion;
			record[1] = strVersion+".7z";
			versiontab->addData(record);
			versiontab->saveToFileStreamNoCook("version.crb");
			versiontab->saveToFile("version.tab");
			compressFile("version.crb",filedir);
		}
	}

	{
		ref_ptr<CREncapsulation::crTableIO> dynamictab = new CREncapsulation::crTableIO;
		//ref_ptr<CREncapsulation::crTableIO> lastDynamictab = CREncapsulation::crTableIO::openFile(lastPublish+"dynamic.crb");
		//bool lasttab = lastDynamictab.valid();
		dynamictab->setTitleVec(title);
		for( itr1 = newFileVec.begin();
			itr1 != newFileVec.end();
			++itr1 )
		{
			//found = false;
			//for( itr2 = lastFileVec.begin();
			//	itr2 != lastFileVec.end();
			//	++itr2 )
			//{
			//	if(*itr1 == *itr2)
			//	{
			//		found = true;
			//		break;
			//	}
			//}
			//if(!found)
			{//新加
				record[0] = *itr1;
				stream = new crStreamBuf;
				stream->loadFromFile2((newPublish+*itr1).c_str());
				record[1] = stream->getMD5();
				dynamictab->addData(record);
				compressFile(*itr1,filedir);
			}
			//else
			//{//文件比较
			//	same = true;
			//	std::ifstream f1((newPublish+*itr1).c_str(), std::ios::in | std::ios::binary);
			//	f1.seekg(0, std::ios::end); 
			//	count1  =  f1.tellg();
			//	f1.seekg(0, std::ios::beg);
			//	std::ifstream f2((lastPublish+*itr2).c_str(), std::ios::in | std::ios::binary);
			//	f2.seekg(0, std::ios::end); 
			//	count2  =  f2.tellg();
			//	f2.seekg(0, std::ios::beg);
			//	do 
			//	{
			//		if(count1 != count2)
			//		{
			//			same = false;
			//			break;
			//		}
			//		buf1 = new char[count1];
			//		buf2 = new char[count1];
			//		memset(buf1,0,count1);
			//		memset(buf2,0,count1);
			//		f1.read(buf1,count1);
			//		f2.read(buf2,count1);
			//		if(memcmp(buf1,buf2,count1) != 0)
			//		{
			//			same = false;
			//		}
			//		delete [] buf1;
			//		delete [] buf2;
			//	} while (0);
			//	if(!same)
			//	{//
			//		ver = 0;
			//		if(lasttab && lastDynamictab->queryOneRecord(0,*itr1,qrecord)>=0)
			//		{
			//			ver = atoi(qrecord[1].c_str());
			//			ver++;
			//		}
			//		record[0] = *itr1;
			//		record[1] = crArgumentParser::appItoa(ver);
			//		stream = new crStreamBuf;
			//		stream->loadFromFile2((newPublish+*itr1).c_str());
			//		record[2] = stream->getMD5();
			//		dynamictab->addData(record);
			//		compressFile(*itr1,filedir);
			//	}
			//	else
			//	{
			//		ver = 0;
			//		if(lasttab && lastDynamictab->queryOneRecord(0,*itr1,qrecord)>=0)
			//		{
			//			ver = atoi(qrecord[1].c_str());
			//			compressFile(*itr1,filedir);
			//		}
			//		else
			//		{
			//			compressFile(*itr1,filedir);
			//		}
			//		record[0] = *itr1;
			//		record[1] = crArgumentParser::appItoa(ver);
			//		stream = new crStreamBuf;
			//		stream->loadFromFile2((newPublish+*itr1).c_str());
			//		record[2] = stream->getMD5();
			//		dynamictab->addData(record);
			//	}
			//}
		}
		dynamictab->saveToFileStreamNoCook("dynamic.crb");
		compressFile("dynamic.crb",filedir);
		//兼容version:2014022100以前的版本的自动更新
		//dynamictab->saveToFile("dynamic.tab");
		//compressFile("dynamic.tab",filedir);
	}

	system("pause");
	return 0;
}

