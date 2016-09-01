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
#include <CREncapsulation/crTableIO.h>
#include <CRIOManager/crCookFile.h>
#include <CRIOManager/crFileNameUtils.h>
#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crLoadManager.h>
#include <iostream>
#include <fstream>
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
using namespace CRCore;
using namespace CREncapsulation;
using namespace CRIOManager;
//////////////////////////////////////////////////////////////////////////
//
//crTableIO
//
//////////////////////////////////////////////////////////////////////////
crTableIO::crTableIO()
{
}
crTableIO::~crTableIO()
{

}
crTableIO* crTableIO::openFile(std::string fileName)
{
	crTableIO* loaded = dynamic_cast<crTableIO *>(crScriptLoadManager::getInstance()->findLoadedRefeanced(fileName));
	if(!loaded)
	{
		std::string ext = crArgumentParser::getFileExtension(fileName);
		ref_ptr<crTableIO> newtab = new crTableIO;
		if(equalCaseInsensitive(ext,"tab")||equalCaseInsensitive(ext,"tab1"))
		{
			CRIOManager::crLoadCookFile scopedLoad(fileName);
			if(newtab->openFileNoCook(scopedLoad.getFileName())&&newtab->validTab())
			{
				crScriptLoadManager::getInstance()->insertLoadedRefeanced(fileName,newtab.get());
				return newtab.get();
			}
			else
			{
				return NULL;
			}
		}
		else//crb
		{
			CRIOManager::crLoadCookFile scopedLoad(fileName);
			if(newtab->openFileStream(scopedLoad.getFileName())&&newtab->validTab())
			{
				crScriptLoadManager::getInstance()->insertLoadedRefeanced(fileName,newtab.get());
				return newtab.get();
			}
			else if(newtab->openFileStream(fileName)&&newtab->validTab())
			{
				crScriptLoadManager::getInstance()->insertLoadedRefeanced(fileName,newtab.get());
				return newtab.get();
			}
			else
			{
				std::string fileName2 = crArgumentParser::getFileNameEliminateExt(fileName)+".tab";
				CRIOManager::crLoadCookFile scopedLoad2(fileName2);
				if(newtab->openFileNoCook(scopedLoad2.getFileName())&&newtab->validTab())
				{
					crScriptLoadManager::getInstance()->insertLoadedRefeanced(fileName,newtab.get());
					return newtab.get();
				}
				else
				{
					return NULL;
				}
			}
		}
	}
	return loaded;
	//CRIOManager::crLoadCookFile scopedLoad(fileName);
	//return openFileNoCook(fileName);
}
bool crTableIO::openFileNoCook(std::string fileName)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	std::ifstream fin(fileName.c_str(), std::ios::in);
	if(!fin) return false;

	fin.seekg(0, std::ios::end); 
	int count  =  fin.tellg();
	char *buf = new char[count];//(char*)malloc(bufSize);
	if(buf) 
	{
		try
		{
			memset(buf,0,count);
			fin.seekg(0, std::ios::beg);
			fin.read(buf,count);

			m_titleVec.clear();
			m_dataVec.clear();
			unsigned int front_of_line = 0;
			unsigned int end_of_line = 0;
			bool title = true;
			StrVec readvec;
			while (end_of_line<count)
			{
				if (buf[end_of_line]==9)
				{
					readvec.push_back(std::string( buf, front_of_line, end_of_line-front_of_line) );

					++end_of_line;
					front_of_line = end_of_line;
				}
				else if (buf[end_of_line]=='\n')
				{
					readvec.push_back(std::string( buf, front_of_line, end_of_line-front_of_line) );
					++end_of_line;
					if(title)
					{
						m_titleVec = readvec;
						title = false;
					}
					else
					{
						m_dataVec.push_back(readvec);
					}
					readvec.resize(0);
					front_of_line = end_of_line;
				}
				else ++end_of_line;
			}
			delete [] buf;
			fin.close();
			return true;
		}
		catch (...)
		{
			char gbuf[256];
			sprintf(gbuf,"crTableIO::openFileNoCook Error,File:%s\n\0",fileName.c_str());
			gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
			//DeleteFileA(fileName.c_str());
			delete [] buf;
			fin.close();
			return false;
		}
	}
	else
	{
		fin.close();
		return false;
	}
}
void crTableIO::saveToFile(std::string fileName,SecurityMode security)
{//改成安全存储，先存到临时文件，然后执行拷贝覆盖
	if(fileName.empty())
	{
		CRCore::notify(CRCore::FATAL)<<"crTableIO::saveToFile:文件名不能为空"<< std::endl;
		return;
	}
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	std::string xfilename = fileName;
	if(security>SM_None)
	{
		std::string path = CRIOManager::getFilePath(fileName);
		std::string name = CRIOManager::getSimpleFileName(fileName);
		if(path.empty())
			xfilename = "~"+name;
		else
			xfilename = path+"/"+"~"+name;
	}
	std::ofstream fout(xfilename.c_str(), std::ios::out);
	if(!fout) return;
	int columncount = getColumnCount();
	if(columncount<1) return;
	int rowcount = getRowCount();
	int i,j;
	char tab = 9;
	char n = '\n';
	for(i = 0; i<columncount-1; ++i)
	{
		fout.write(m_titleVec[i].c_str(),m_titleVec[i].length());
		fout.write(&tab,1);
	}
	fout.write(m_titleVec[i].c_str(),m_titleVec[i].length());
	fout.write(&n,1);
	for(i = 0; i<rowcount;++i)
	{
		for(j=0; j<columncount-1; ++j)
		{
			fout.write(m_dataVec[i][j].c_str(),m_dataVec[i][j].length());
			fout.write(&tab,1); 
		}
		fout.write(m_dataVec[i][j].c_str(),m_dataVec[i][j].length());
		fout.write(&n,1);
	}
	//fout.write(&n,1);
	fout.close();
	CRIOManager::crWriteCookFile scopedWrite(xfilename);
	if(security>SM_None)
	{
		CRIOManager::GetCookedFileName(xfilename);
		CRIOManager::GetCookedFileName(fileName);
		CRIOManager::copyFile(xfilename,fileName);
		if(security==SM_Copy)
		{
			DeleteFileA(xfilename.c_str());
		}
	}
}
void crTableIO::saveToFileNoCook(std::string fileName,SecurityMode security)
{//改成安全存储，先存到临时文件，然后执行拷贝覆盖
	if(fileName.empty())
	{
		CRCore::notify(CRCore::FATAL)<<"crTableIO::saveToFile:文件名不能为空"<< std::endl;
		return;
	}
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	std::string xfilename = fileName;
	if(security>SM_None)
	{
		std::string path = CRIOManager::getFilePath(fileName);
		std::string name = CRIOManager::getSimpleFileName(fileName);
		if(path.empty())
			xfilename = "~"+name;
		else
			xfilename = path+"/"+"~"+name;
	}
	std::ofstream fout(xfilename.c_str(), std::ios::out);
	if(!fout) return;
	int columncount = getColumnCount();
	if(columncount<1) return;
	int rowcount = getRowCount();
	int i,j;
	char tab = 9;//'\t'
	char n = '\n';
	for(i = 0; i<columncount-1; ++i)
	{
		fout.write(m_titleVec[i].c_str(),m_titleVec[i].length());
		fout.write(&tab,1);
	}
	fout.write(m_titleVec[i].c_str(),m_titleVec[i].length());
	fout.write(&n,1);
	for(i = 0; i<rowcount;++i)
	{
		for(j=0; j<columncount-1; ++j)
		{
			fout.write(m_dataVec[i][j].c_str(),m_dataVec[i][j].length());
			fout.write(&tab,1); 
		}
		fout.write(m_dataVec[i][j].c_str(),m_dataVec[i][j].length());
		fout.write(&n,1);
	}
	//fout.write(&n,1);
	fout.close();
	if(security>SM_None)
	{
		CRIOManager::copyFile(xfilename,fileName);
		if(security==SM_Copy)
		{
			DeleteFileA(xfilename.c_str());
		}
	}
}
void crTableIO::saveToFileStream(std::string fileName,SecurityMode security)
{//改成安全存储，先存到临时文件，然后执行拷贝覆盖
	if(fileName.empty())
	{
		CRCore::notify(CRCore::FATAL)<<"crTableIO::saveToFile:文件名不能为空"<< std::endl;
		return;
	}
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	std::string xfilename = fileName;
	if(security>SM_None)
	{
		std::string path = CRIOManager::getFilePath(fileName);
		std::string name = CRIOManager::getSimpleFileName(fileName);
		if(path.empty())
			xfilename = "~"+name+crArgumentParser::appItoa(GetCurrentProcessId());
		else
			xfilename = path+"/"+"~"+name+crArgumentParser::appItoa(GetCurrentProcessId());
	}
	ref_ptr<crStreamBuf> stream = new crStreamBuf;
	stream->createBuf(4096);
	stream->_writeUShort(m_titleVec.size());
	StrVec::iterator sitr;
	DataVec::iterator ditr;
	for( sitr = m_titleVec.begin();
		 sitr != m_titleVec.end();
		 ++sitr )
	{
		stream->_writeString(*sitr);
	}
	stream->_writeUInt(m_dataVec.size());
	int size = 0;
	for( ditr = m_dataVec.begin();
		ditr != m_dataVec.end();
		++ditr )
	{
		stream->_writeUShort(ditr->size());
		for( sitr = ditr->begin();
			sitr != ditr->end();
			++sitr )
		{
			size = sitr->size()+4;
			stream->_writeString(*sitr);
		}
	}
	stream->seekBegin();
	stream->saveToFile2(xfilename.c_str());
	CRIOManager::crWriteCookFile scopedWrite(xfilename);
	if(security>SM_None)
	{
		CRIOManager::GetCookedFileName(xfilename);
		CRIOManager::GetCookedFileName(fileName);
		CRIOManager::copyFile(xfilename,fileName);
		if(security==SM_Copy)
		{
			DeleteFileA(xfilename.c_str());
		}
	}
}
void crTableIO::saveToFileStreamNoCook(std::string fileName,SecurityMode security)
{//改成安全存储，先存到临时文件，然后执行拷贝覆盖
	if(fileName.empty())
	{
		CRCore::notify(CRCore::FATAL)<<"crTableIO::saveToFile:文件名不能为空"<< std::endl;
		return;
	}
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	std::string xfilename = fileName;
	if(security>SM_None)
	{
		std::string path = CRIOManager::getFilePath(fileName);
		std::string name = CRIOManager::getSimpleFileName(fileName);
		if(path.empty())
			xfilename = "~"+name+crArgumentParser::appItoa(GetCurrentProcessId());
		else
			xfilename = path+"/"+"~"+name+crArgumentParser::appItoa(GetCurrentProcessId());
	}
	ref_ptr<crStreamBuf> stream = new crStreamBuf;
	stream->createBuf(4096);
	stream->_writeUShort(m_titleVec.size());
	StrVec::iterator sitr;
	DataVec::iterator ditr;
	for( sitr = m_titleVec.begin();
		sitr != m_titleVec.end();
		++sitr )
	{
		stream->_writeString(*sitr);
	}
	stream->_writeUInt(m_dataVec.size());
	int size = 0;
	for( ditr = m_dataVec.begin();
		ditr != m_dataVec.end();
		++ditr )
	{
		stream->_writeUShort(ditr->size());
		for( sitr = ditr->begin();
			sitr != ditr->end();
			++sitr )
		{
			size = sitr->size()+4;
			stream->_writeString(*sitr);
		}
	}
	stream->seekBegin();
	stream->saveToFile2(xfilename.c_str());
	if(security>SM_None)
	{
		CRIOManager::copyFile(xfilename,fileName);
		if(security==SM_Copy)
		{
			DeleteFileA(xfilename.c_str());
		}
	}
}
bool crTableIO::openFileStream(std::string fileName)
{//改成安全存储，先存到临时文件，然后执行拷贝覆盖
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	try
	{
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		if(stream->loadFromFile2(fileName))
		{
			int size = stream->_readUShort();
			int i,j,size2;
			m_titleVec.resize(size);
			for(i = 0; i<size; i++)
			{
				m_titleVec[i] = stream->_readString();
			}
			size = stream->_readUInt();
			//m_dataVec.resize(size);
			StrVec record;
			for( i = 0; i<size; i++ )
			{
				size2 = stream->_readUShort();
				if(size2>0)
				{
					record.resize(size2);
					for(j = 0; j<size2; j++)
					{
						record[j] = stream->_readString();
					}
					m_dataVec.push_back(record);
				}
				//m_dataVec[i].resize(size2);
			}
			return true;
		}
	}
	catch (...)
	{
		char gbuf[256];
		sprintf(gbuf,"crTableIO::openFileStream Error,File:%s\n\0",fileName.c_str());
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		DeleteFileA(fileName.c_str());
	}
	return false;
}
int crTableIO::getRowCount() const
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	return m_dataVec.size();
}
int crTableIO::getColumnCount() const
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	return m_titleVec.size();
}
const std::string &crTableIO::getTitle(int i) const
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	return m_titleVec[i];
}
int crTableIO::getTitleIndex(const std::string &title) const
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	int titlesize = m_titleVec.size();
	for( int i = 0; i<titlesize; i++)
	{
		if(title.compare(m_titleVec[i]) == 0)
			return i;
	}
	return -1;
}

std::string &crTableIO::getData(int row, int col)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	return m_dataVec[row][col];
}
std::string &crTableIO::getData(int row, const std::string &title)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	return getData(row,getTitleIndex(title));
}
crTableIO::DataVec &crTableIO::getDataVec()
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	return m_dataVec;
}
//std::string crTableIO::getData(const std::string &id, int col)
//{
//	int datasize = m_dataVec.size();
//	for( int i = 0; i<datasize; i++)
//	{
//		if(id.compare(m_dataVec[i][0]) == 0)
//			return m_dataVec[i][col];
//	}
//	return "";
//}
//std::string crTableIO::getData(const std::string &id, const std::string &title)
//{
//	return getData(id,getTitleIndex(title));
//}
bool crTableIO::setTitleVec(StrVec &titlevec)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	bool can = true;
	if(!m_dataVec.empty())
	{
		if(m_dataVec[0].size() != m_titleVec.size())
		{
			can = false;
		}
	}
	if(can)
	{
		m_titleVec = titlevec;
		return true;
	}
	return false;
}
bool crTableIO::addData(StrVec &data)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	if(data.size() == m_titleVec.size())
	{
		m_dataVec.push_back(data);
		return true;
	}
	return false;
}
void crTableIO::clearData()
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	m_dataVec.clear();
}
void crTableIO::clearTitle()
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	m_titleVec.clear();
	clearData();
}
int crTableIO::queryOneRecord(int col,const std::string &value,StrVec &record)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	record.clear();
	int row = 0;
	for( DataVec::iterator itr = m_dataVec.begin();
		 itr != m_dataVec.end();
		 ++itr,++row )
	{
		if(CRIOManager::equalCaseInsensitive((*itr)[col],value))
		{
			record = *itr;
			return row;
		}
	}
	return -1;
}
void crTableIO::queryRecords(int col,const std::string &value,DataVec &recordVec)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	recordVec.clear();
	for( DataVec::iterator itr = m_dataVec.begin();
		itr != m_dataVec.end();
		++itr )
	{
		if(CRIOManager::equalCaseInsensitive((*itr)[col],value))
		{
			recordVec.push_back(*itr);
		}
	}
}
void crTableIO::removeRow(int row)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	m_dataVec.erase(m_dataVec.begin()+row);
}
int crTableIO::queryOneRecord(int col1,const std::string &value1,int col2,const std::string &value2,StrVec &record)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	record.clear();
	int row = 0;
	for( DataVec::iterator itr = m_dataVec.begin();
		itr != m_dataVec.end();
		++itr,++row )
	{
		if(CRIOManager::equalCaseInsensitive((*itr)[col1],value1) && CRIOManager::equalCaseInsensitive((*itr)[col2],value2))
		{
			record = *itr;
			return row;
		}
	}
	return -1;
}
void crTableIO::queryRecords(int col1,const std::string &value1,int col2,const std::string &value2,DataVec &recordVec)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	recordVec.clear();
	for( DataVec::iterator itr = m_dataVec.begin();
		itr != m_dataVec.end();
		++itr )
	{
		if(CRIOManager::equalCaseInsensitive((*itr)[col1],value1) && CRIOManager::equalCaseInsensitive((*itr)[col2],value2))
		{
			recordVec.push_back(*itr);
		}
	}
}
int crTableIO::queryOneRecord(int col1,const std::string &value1,int col2,const std::string &value2,int col3,const std::string &value3,StrVec &record)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	record.clear();
	int row = 0;
	for( DataVec::iterator itr = m_dataVec.begin();
		itr != m_dataVec.end();
		++itr,++row )
	{
		if(CRIOManager::equalCaseInsensitive((*itr)[col1],value1) && CRIOManager::equalCaseInsensitive((*itr)[col2],value2)&& CRIOManager::equalCaseInsensitive((*itr)[col3],value3))
		{
			record = *itr;
			return row;
		}
	}
	return -1;
}
void crTableIO::queryRecords(int col1,const std::string &value1,int col2,const std::string &value2,int col3,const std::string &value3,DataVec &recordVec)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	recordVec.clear();
	for( DataVec::iterator itr = m_dataVec.begin();
		itr != m_dataVec.end();
		++itr )
	{
		if(CRIOManager::equalCaseInsensitive((*itr)[col1],value1) && CRIOManager::equalCaseInsensitive((*itr)[col2],value2)&& CRIOManager::equalCaseInsensitive((*itr)[col3],value3))
		{
			recordVec.push_back(*itr);
		}
	}
}