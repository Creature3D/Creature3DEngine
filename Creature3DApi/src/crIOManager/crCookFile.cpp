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
#include <CRIOManager/crCookFile.h>
#include <CRIOManager/crReadFile.h>
#include <CRIOManager/crWriteFile.h>
#include <CRCore/crBrain.h>
#include <sys/stat.h>
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
using namespace CRIOManager;
using namespace CRCore;
static bool Cooked = false;
static bool RetainSourceFile = false;
static bool NetCooked = false;
static bool PublishRun = false;
static std::string FileRoot;
void CRIOManager::SetFileRoot(const std::string &fileRoot) { FileRoot = fileRoot; }
const std::string& CRIOManager::GetFileRoot() { return FileRoot; }
void CRIOManager::SetCooked(bool cook){ Cooked = cook; }
void CRIOManager::SetRetainSourceFile(bool retain){ RetainSourceFile = retain; }
void CRIOManager::SetNetCooked(bool cook){ NetCooked = cook; }
bool CRIOManager::GetNetCooked(){ return NetCooked; }
void CRIOManager::SetPublishRun(bool bln){ PublishRun = bln; }
bool CRIOManager::GetPublishRun(){ return PublishRun; }
bool acceptsExtension(const std::string& extension)
{
	return equalCaseInsensitive(extension,"cfg")||
		   equalCaseInsensitive(extension,"material")||
		   equalCaseInsensitive(extension,"char")||
		   equalCaseInsensitive(extension,"rbody")||
		   equalCaseInsensitive(extension,"tab")||
		   equalCaseInsensitive(extension,"crb")/*||
		   equalCaseInsensitive(extension,"ter")*/;
}
bool CRIOManager::GetCookedFileName(std::string &fileName)
{
	if(Cooked && !fileName.empty())
	{
		std::string ext = CRIOManager::getFileExtension(fileName);
		if (acceptsExtension(ext))
		{
			fileName = CRCore::crArgumentParser::getFileNameEliminateExt(fileName) + ".cook";
			return true;
		}
	}
	return false;
}
/////////////////////////////////////////
//
//crLoadCookFile
//
/////////////////////////////////////////
crLoadCookFile::crLoadCookFile(const std::string &fileName)
{
	//if(Cooked)
	//    CRIOManager::readObjectFile(fileName + ".cook");
	//else
	//{
	//	if(!fileExists(fileName))
	//		CRIOManager::readObjectFile(fileName + ".cook");
	//}
	m_fileName = FileRoot+fileName;
	if(Cooked)
	{
		if(!RetainSourceFile)
		{
			m_fileName += crArgumentParser::appItoa(GetCurrentProcessId());
		}
		CRIOManager::readObjectFile(fileName + ".cook");
	}
	//else
	//{
	//	m_fileName = fileName;
	//	//if(!fileExists(fileName))
	//	//	CRIOManager::readObjectFile(fileName + ".cook");
	//}
}
crLoadCookFile::~crLoadCookFile()
{
//#ifndef _DEBUG
    if(Cooked && !RetainSourceFile)
	    DeleteFileA(m_fileName.c_str());
//#endif
}
/////////////////////////////////////////
//
//crWriteCookFile
//
/////////////////////////////////////////
crWriteCookFile::crWriteCookFile(const std::string &fileName)
{
	m_fileName = FileRoot + fileName;
	if(Cooked)
	{
		CRCore::ref_ptr<CRCore::crNode> cookObj = new CRCore::crNode;
		CRIOManager::writeObjectFile(*(cookObj.get()), fileName + ".cook");
	}
}
crWriteCookFile::~crWriteCookFile()
{
#ifndef _DEBUG
    if(Cooked && !RetainSourceFile)
	    DeleteFileA(m_fileName.c_str());
#endif
}
/////////////////////////////////////////
//
//crLoadCookFileM
//
/////////////////////////////////////////
crLoadCookFileM::crLoadCookFileM(const std::string &fileName, CRCore::crStreamBuf *stream)
:m_fileName(fileName) 
{
	if(Cooked)
	{
		crBrain::getInstance()->createMemFile(fileName + ".mcook",stream);
		CRIOManager::readObjectFile(fileName + ".mcook");
	}
}
crLoadCookFileM::~crLoadCookFileM()
{
	if(Cooked)
	{
		crBrain::getInstance()->releaseMemFile(m_fileName);
		crBrain::getInstance()->releaseMemFile(m_fileName + ".mcook");
	}
}
/////////////////////////////////////////
//
//crWriteCookFileM
//
/////////////////////////////////////////
crWriteCookFileM::crWriteCookFileM(const std::string &fileName, CRCore::crStreamBuf *stream)
:m_fileName(fileName) 
{
	if(Cooked)
	{
		crBrain::getInstance()->createMemFile(fileName,stream);
		CRCore::ref_ptr<CRCore::crNode> cookObj = new CRCore::crNode;
		CRIOManager::writeObjectFile(*(cookObj.get()), fileName + ".mcook");
	}
}
crWriteCookFileM::~crWriteCookFileM()
{
	if(Cooked)
	{
		crBrain::getInstance()->releaseMemFile(m_fileName);
		crBrain::getInstance()->releaseMemFile(m_fileName + ".mcook");
	}
}
/////////////////////////////////////////
//
//crLoadNetCookFileM
//
/////////////////////////////////////////
crLoadNetCookFileM::crLoadNetCookFileM(const std::string &fileName, CRCore::crStreamBuf *stream)
:m_fileName(fileName) 
{
	if(NetCooked)
	{
		crBrain::getInstance()->createMemFile(fileName + ".mcook",stream);
		CRIOManager::readObjectFile(fileName + ".mcook");
	}
}
crLoadNetCookFileM::~crLoadNetCookFileM()
{
	if(NetCooked)
	{
		crBrain::getInstance()->releaseMemFile(m_fileName);
		crBrain::getInstance()->releaseMemFile(m_fileName + ".mcook");
	}
}
/////////////////////////////////////////
//
//crWriteNetCookFileM
//
/////////////////////////////////////////
crWriteNetCookFileM::crWriteNetCookFileM(const std::string &fileName, CRCore::crStreamBuf *stream)
:m_fileName(fileName) 
{
	if(NetCooked)
	{
		crBrain::getInstance()->createMemFile(fileName,stream);
		CRCore::ref_ptr<CRCore::crNode> cookObj = new CRCore::crNode;
		CRIOManager::writeObjectFile(*(cookObj.get()), fileName + ".mcook");
	}
}
crWriteNetCookFileM::~crWriteNetCookFileM()
{
	if(NetCooked)
	{
		crBrain::getInstance()->releaseMemFile(m_fileName);
		crBrain::getInstance()->releaseMemFile(m_fileName + ".mcook");
	}
}