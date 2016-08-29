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
#ifndef CRDOWNLOAD_CRDOWNLOAD_H
#define CRDOWNLOAD_CRDOWNLOAD_H 1

#include <CRDownload/crExport.h>
#include <CRCore/Referenced.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/thread/crThread.h>
#include <CRCore/thread/crCriticalMutex.h>
#include <CRCore/crStreamBuf.h>
#include <CREncapsulation/crTableIO.h>
#include <deque>
namespace CRDownload
{

class CRDOWNLOAD_EXPORT crDownload: public CRCore::Referenced
{
public:
	crDownload(const std::string &filename);
	enum DownloadState
	{
		Wait,
		Downloading,
		UnCompress,
		Faild,
		Done,
	};
	const std::string &getFileName();
	std::string getPkgFileName();
	void setSize(int size);
	int getSize();
	void setDownloadState(DownloadState state);
	DownloadState getDownloadState();
	inline CRCore::ref_ptr<CRCore::crStreamBuf> &recvstream(){ return m_recvstream; }
	float getProgress();
	//float getSpeed();
	void saveFile();
	bool moveFileToDirectory();
	bool isinifile();
	bool is7zfile();
protected:
	virtual ~crDownload();
	std::string m_filename;
	int m_size;//文件大小
	DownloadState m_downloadState;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_recvstream;
};
class crDownloadManager;
class crDownloadThread: public CRCore::crThread, public CRCore::Referenced
{
friend class crDownloadManager;
public:
	crDownloadThread();
	virtual void run();
protected:
	virtual ~crDownloadThread();
	void downloadImplementation();
	bool m_done;
	char* m_buf;
	CRCore::ref_ptr<crDownload> m_download;
};
class CRDOWNLOAD_EXPORT crDownloadManager: public CRCore::Referenced
{
public:
	crDownloadManager();
	static crDownloadManager *getInstance();
	virtual void clear();
	void start(int threadcount);
	void setHost(const std::string &host);
	const std::string &getHost();
	void setHost1(const std::string &host1);
	const std::string &getHost1();
	void setHostDir(const std::string &dir);
	const std::string &getHostDir();
	void setFileRootDir(const std::string &dir);
	const std::string &getFileRootDir();
	typedef std::deque< CRCore::ref_ptr<crDownload> > DownloadDeque;
	bool addDownloadRequest(CRCore::ref_ptr<crDownload> download);
	void addImmediatelyDownloadRequest(CRCore::ref_ptr<crDownload> download);
	void getTask(crDownloadThread *thread);
	crDownload *getCurrentDownload();
	//crDownload *getCurrentImmediatelyDownload();

	typedef std::map< std::string,CRCore::ref_ptr<crDownload>,std::less<std::string> >DownloadMap;
	crDownload *getDownload(const std::string &filename);
	void removeDownload(const std::string &filename);
	int getDownloadCount();
	void lockDownloadMap();
	DownloadMap &getDownloadMap();
	void unlockDownloadMap();
	void setTaskCount(int count){ m_taskCount = count; }
	void setCompleteCount(int count){ m_completeCount = count; }
	void complete(crDownload *download);
	inline float getProgress(){ return (float)m_completeCount/(float)m_taskCount; }
	inline bool isComplete(){ return m_completeCount == m_taskCount; }
	inline int getTaskCount(){ return m_taskCount; }
	inline int getCompleteCount(){ return m_completeCount; }
	bool isIdle();
	void setDynamicDownloadRecord(bool record);
	bool getDynamicDownloadRecord();
protected:
	virtual ~crDownloadManager();
	void insertDownloadMap(crDownload *download);
	void getDownload( CRCore::ref_ptr<crDownload> &download );
	void getImmediatelyDownload(CRCore::ref_ptr<crDownload> &download);
	void pushDownloadingDeque(CRCore::ref_ptr<crDownload> download);
	static CRCore::ref_ptr<crDownloadManager> m_instance;
	std::string m_hostName;
	std::string m_hostName1;
	std::string m_hostDir;
	std::string m_fileRootDir;
	typedef std::deque< CRCore::ref_ptr<crDownloadThread> >DownloadThreadDeq;
	DownloadThreadDeq m_downloadThreads;
	//CRCore::ref_ptr<crDownloadThread> m_immDownloadThread;

	DownloadDeque m_requestDeque;
	DownloadDeque m_immediatelyRequestDeque;
	DownloadDeque m_downloadingDeque;
	CRCore::crCriticalMutex m_requestDequeMutex;
	CRCore::crCriticalMutex m_immediatelyRequestDequeMutex;
	CRCore::crCriticalMutex m_downloadingDequeMutex;
	DownloadMap m_downloadMap;
	CRCore::crCriticalMutex m_downloadMapMutex;
	int m_taskCount;
	int m_completeCount;
	bool m_dynamicDownloadRecord;//动态下载录制
	CRCore::ref_ptr<CREncapsulation::crTableIO> m_dynamicDownloadTab;//动态下载录制
};
}

#endif
