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
#ifndef CRIOMANAGER_CRCOOKFILE_H
#define CRIOMANAGER_CRCOOKFILE_H 1

#include <CRIOManager/crExport.h>
#include <CRCore/crStreamBuf.h>
#include <string>
namespace CRIOManager {

extern CRIOMANAGER_EXPORT void SetCooked(bool cook);
extern CRIOMANAGER_EXPORT void SetRetainSourceFile(bool retain);
extern CRIOMANAGER_EXPORT void SetNetCooked(bool cook);
extern CRIOMANAGER_EXPORT bool GetNetCooked();
extern CRIOMANAGER_EXPORT void SetPublishRun(bool bln);
extern CRIOMANAGER_EXPORT bool GetPublishRun();
extern CRIOMANAGER_EXPORT bool GetCookedFileName(std::string &fileName);
//调用方法示例：CRIOManager::crLoadCookFile scopedLoad(filename);
class CRIOMANAGER_EXPORT crLoadCookFile
{
public:
	crLoadCookFile(const std::string &fileName);
	~crLoadCookFile();
	inline std::string &getFileName(){ return m_fileName; }
private:
	std::string m_fileName;
};

class CRIOMANAGER_EXPORT crWriteCookFile
{
public:
	crWriteCookFile(const std::string &fileName);
	~crWriteCookFile();
private:
	std::string m_fileName;
};

class CRIOMANAGER_EXPORT crLoadCookFileM
{
public:
	crLoadCookFileM(const std::string &fileName, CRCore::crStreamBuf *stream);
	~crLoadCookFileM();
private:
	std::string m_fileName;
};

class CRIOMANAGER_EXPORT crWriteCookFileM
{
public:
	crWriteCookFileM(const std::string &fileName, CRCore::crStreamBuf *stream);
	~crWriteCookFileM();
private:
	std::string m_fileName;
};

class CRIOMANAGER_EXPORT crLoadNetCookFileM
{
public:
	crLoadNetCookFileM(const std::string &fileName, CRCore::crStreamBuf *stream);
	~crLoadNetCookFileM();
private:
	std::string m_fileName;
};

class CRIOMANAGER_EXPORT crWriteNetCookFileM
{
public:
	crWriteNetCookFileM(const std::string &fileName, CRCore::crStreamBuf *stream);
	~crWriteNetCookFileM();
private:
	std::string m_fileName;
};

}

#endif