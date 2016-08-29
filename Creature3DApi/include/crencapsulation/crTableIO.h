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

//Tab文件的读写
#ifndef CRENCAPSULATION_CRTABLEIO
#define CRENCAPSULATION_CRTABLEIO 1

#include <CREncapsulation/crExport.h>
#include <CRCore/Referenced.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/thread/crCriticalMutex.h>
#include <CRCore/thread/crScopedLock.h>
#include <string>
#include <vector>
namespace CREncapsulation {
enum SecurityMode
{
	SM_None,
	SM_Copy,
	SM_Record
};
class CRENCAPSULATION_EXPORT crTableIO : public CRCore::Referenced
{
public:
	crTableIO();
	typedef std::vector<std::string> StrVec;
	typedef std::vector<StrVec> DataVec;
	static crTableIO* openFile(std::string fileName);
	bool openFileNoCook(std::string fileName);
	bool openFileStream(std::string fileName);
	void saveToFile(std::string fileName,SecurityMode security=SM_None);//0默认，1拷贝模式，2拷贝模式保留临时文件
	void saveToFileNoCook(std::string fileName,SecurityMode security=SM_None);//0默认，1拷贝模式，2拷贝模式保留临时文件
	void saveToFileStream(std::string fileName,SecurityMode security=SM_None);//0默认，1拷贝模式，2拷贝模式保留临时文件
	void saveToFileStreamNoCook(std::string fileName,SecurityMode security=SM_None);//0默认，1拷贝模式，2拷贝模式保留临时文件
	int getRowCount() const;
	int getColumnCount() const;
	const std::string &getTitle(int col) const;
	int getTitleIndex(const std::string &title) const;
	std::string &getData(int row, int col);
	std::string &getData(int row, const std::string &title);
	DataVec &getDataVec();
	//std::string getData(const std::string &id, int col);
	//std::string getData(const std::string &id, const std::string &title);
	inline std::string & operator()(int row, int col) 
	{ CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex); return m_dataVec[row][col]; }
	bool setTitleVec(StrVec &titlevec);
	bool addData(StrVec &data);
	void clearData();
	void clearTitle();
	int queryOneRecord(int col,const std::string &value,StrVec &record);//返回row,返回-1表示没有查到
	void queryRecords(int col,const std::string &value,DataVec &recordVec);	
	inline static int getRowByID(int id){ return id-1; }
	void removeRow(int row);
	int queryOneRecord(int col1,const std::string &value1,int col2,const std::string &value2,StrVec &record);//返回row,返回-1表示没有查到
	void queryRecords(int col1,const std::string &value1,int col2,const std::string &value2,DataVec &recordVec);
	int queryOneRecord(int col1,const std::string &value1,int col2,const std::string &value2,int col3,const std::string &value3,StrVec &record);//返回row,返回-1表示没有查到
	void queryRecords(int col1,const std::string &value1,int col2,const std::string &value2,int col3,const std::string &value3,DataVec &recordVec);
	inline bool validTab(){return !m_titleVec.empty();}
protected:
	virtual ~crTableIO();
	StrVec m_titleVec;
	DataVec m_dataVec;
	mutable CRCore::crCriticalMutex m_mutex;
};
}

#endif