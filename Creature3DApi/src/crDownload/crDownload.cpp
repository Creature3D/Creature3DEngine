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
#if defined(WIN32) && !defined(__CYGWIN__)
#include <Io.h>
#include <Windows.h>
#include <Winbase.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h> // for _mkdir

#define mkdir(x,y) _mkdir((x))
#define stat64 _stati64

// set up for windows so acts just like unix access().
#define F_OK 4


#else // unix

#if defined( __APPLE__ ) || defined(__CYGWIN__) || defined(__FreeBSD__)
#define stat64 stat
#endif

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

// set up _S_ISDIR()
#if !defined(S_ISDIR)
#  if defined( _S_IFDIR) && !defined( __S_IFDIR)
#    define __S_IFDIR _S_IFDIR
#  endif
#  define S_ISDIR(mode)    (mode&__S_IFDIR)
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <WinInet.h>
#include <CRDownload/crDownload.h>
#include <CRDownload/sockinet.h>
#include <CRCore/crBrain.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crNotify.h>
#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crFileNameUtils.h>
#include <CRIOManager/crConvertUTF.h>
#include <CRNet/crStreamPacket.h>
using namespace CRDownload;
using namespace CRCore;
using namespace CRIOManager;
using namespace CREncapsulation;
///////////////////////////////////
//
//crDownload
//
/////////////////////////////////////
crDownload::crDownload(const std::string &filename)
:m_filename(filename),
m_downloadState(Wait),
m_size(0)
{
}
crDownload::~crDownload()
{
}
void crDownload::setSize(int size)
{
	m_size = size;
	m_recvstream = new crStreamBuf;
	m_recvstream->createBuf(m_size);
	m_downloadState = Downloading;
}
const std::string &crDownload::getFileName()
{
	return m_filename;
}
std::string crDownload::getPkgFileName()
{
	if(isinifile() || is7zfile())
		return crDownloadManager::getInstance()->getFileRootDir()+"/"+m_filename;
	else
		return crDownloadManager::getInstance()->getFileRootDir()+"/"+m_filename+".pkg";
}
int crDownload::getSize()
{
	return m_size;
}
void crDownload::setDownloadState(DownloadState state)
{
	m_downloadState = state;
}
crDownload::DownloadState crDownload::getDownloadState()
{
	return m_downloadState;
}
float crDownload::getProgress()
{
	if(m_downloadState == Downloading)
	{
		return (float)m_recvstream->getBufSize()/(float)m_size;
	}
	else if( m_downloadState == Wait )
	{
		return 0.0f;
	}
	else
	{
		return 1.0f;
	}
}
void crDownload::saveFile()
{
	if(m_recvstream.valid())
	{
		std::string destFile = crDownloadManager::getInstance()->getFileRootDir()+"/"+getFileName();
		CRIOManager::makeDirectoryForFile(destFile);
		m_recvstream->seekBegin();
		m_recvstream->saveToFile2(destFile);
		m_downloadState = Done;
		m_recvstream = NULL;
	}
}
bool crDownload::moveFileToDirectory()
{
	std::string file = crDownloadManager::getInstance()->getFileRootDir()+"/"+getFileName();
	if(is7zfile())
	{
		char buf[256];
		memset(buf,0,256);
		sprintf_s(buf,"7zr.exe x -r %s -y",file.c_str());
		PROCESS_INFORMATION pi;
		STARTUPINFO si; //This is an [in] parameter
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof si; //Only compulsory field
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		if(CreateProcess(NULL, buf, NULL,NULL,FALSE,0,NULL, NULL,&si,&pi))
		{
			WaitForSingleObject(pi.hProcess,INFINITE);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
		else
		{
			int err = GetLastError();
			sprintf_s(buf,"解压缩[%s]出错!%d\0",file.c_str(),err);
			MessageBox(::GetActiveWindow(),buf,"Creature3D",MB_OK);
			return false;
		}
	}
	else
	{
		CRIOManager::copyFile(file,getFileName());
		DeleteFileA(file.c_str());
	}
	return true;
}
bool crDownload::isinifile()
{
	std::string ext = getFileExtension(m_filename);
	return equalCaseInsensitive(ext,"ini");
}
bool crDownload::is7zfile()
{
	std::string ext = getFileExtension(m_filename);
	return equalCaseInsensitive(ext,"7z");
}
///////////////////////////////////
//
//crDownloadThread
//
/////////////////////////////////////
#define BUFSIZE 1024
#define VALID_TIME 120
crDownloadThread::crDownloadThread():
m_done(false)
{
	m_buf = new char[BUFSIZE];
}
crDownloadThread::~crDownloadThread() 
{
	m_done = true;
	while (isRunning()) 
	{
		yieldCurrentThread();
	}
	delete [] m_buf;
}
 void crDownloadThread::downloadImplementation()
 {//下载的文件名中只能使用英文，不能有空格
	 if(m_download->is7zfile())
	 {//检查目录下是否已经存在
		 std::string destFile = crDownloadManager::getInstance()->getFileRootDir()+"/"+m_download->getFileName();
		 if(CRIOManager::fileExists(destFile))
		 {
			 m_download->setDownloadState(crDownload::Done);
			 crDownloadManager::getInstance()->complete(m_download.get());
			 m_download = NULL;
			 return;
		 }
	 }
 	ref_ptr<iosockinet> sio;
	std::string host;
 	try {
 		sio = new iosockinet(sockbuf::sock_stream);
		if(m_download->getDownloadState() == crDownload::Faild)
			host = crDownloadManager::getInstance()->getHost1();
		else
			host = crDownloadManager::getInstance()->getHost();
 		sio->rdbuf()->connect( host.c_str(), 80 );
		sio->rdbuf()->recvtimeout(250);

 		std::string requestAdd ="/" + CRIOManager::convertFileNameToUnixStyle(crDownloadManager::getInstance()->getHostDir()+m_download->getPkgFileName());
 		//requestAdd = CRIOManager::convertUTF8toUTF16(requestAdd);
 		*sio << 
         "GET " << requestAdd << " HTTP/1.1\r\n" 
         "User-Agent: Creature3D\r\n"
         "Accept: */*\r\n"
         "Host: " << host << "\r\n"
         "Connection: close\r\n"
         "\r\n";
 		sio->flush();
 	}
 	catch( sockerr e )
 	{//无法链接下载服务器
 		//CRCore::notify(CRCore::WARN) << "crDownloadManager无法链接服务器：" << crDownloadManager::getInstance()->getHost() << std::endl;
		char gbuf[256];
		sprintf(gbuf,"crDownloadManager无法链接服务器：%s\n\0",host.c_str());
		gDebugInfo->debugInfo(CRCore::WARN,gbuf);
 		//int texid = 16;
 		//crHandle *noticeHandle = crBrain::getInstance()->getDataClass()->getHandle(WCH_USER+310);
 		//noticeHandle->inputParam(387,&texid);
 		//int mode = 0;
 		//noticeHandle->inputParam(388,&mode);
 		//crBrain::getInstance()->doEvent(WCH_USER+310);//WCH_GlobalNotice
 		//CRCore::notify(CRCore::WARN)<<m_download->getPkgFileName()<<" 下载失败"<<std::endl;
		sprintf(gbuf,"%s 下载失败\n\0",m_download->getPkgFileName().c_str());
		gDebugInfo->debugInfo(CRCore::WARN,gbuf);
		if(m_download->getDownloadState() == crDownload::Faild)
 			m_download->setDownloadState(crDownload::Wait);
		else
			m_download->setDownloadState(crDownload::Faild);
		crDownloadManager::getInstance()->addDownloadRequest(m_download.get());
 		m_download = NULL;
 		sio = NULL;
 		//sleep(10);
 		return;
 	}
 	bool success = false;
try {
 	do
 	{
 		if(!sio.valid())
 			break;
 		CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();
 		int count;
 		//read head
 		count = sio->rdbuf()->read(m_buf,BUFSIZE);
 		if(count<=0)
 			break;
 		unsigned int front_of_line = 0;
 		unsigned int end_of_line = 0;
 		std::vector<std::string> StrVec;
 		while (end_of_line<count && m_buf[front_of_line]!='\r')
 		{
 			if (m_buf[end_of_line]=='\n')
 			{
 				StrVec.push_back(std::string( m_buf, front_of_line, end_of_line-front_of_line) );
 				++end_of_line;
 				front_of_line = end_of_line;
 			}
 			else ++end_of_line;
 		}
 		if(StrVec.empty())
 			break;
 		std::string str;
 		int value = 0;
 		int i = StrVec[0].find(' ');
 		if(i>-1)
 		{
 			str = StrVec[0].substr(0,i);
 			value = atoi(StrVec[0].substr(i,StrVec[0].size()-i).c_str());
 		}
 		if(value == 200)
 			;
 		else if(value == 400)
 		{
 			//CRCore::notify(CRCore::WARN)<<"crDownloadThread: http server response 400 - Bad Request : "<<m_download->getPkgFileName()<< std::endl;
			char gbuf[256];
			sprintf(gbuf,"crDownloadThread: http server response 400 - Bad Request : %s\n\0",m_download->getPkgFileName().c_str());
			gDebugInfo->debugInfo(CRCore::WARN,gbuf);
 			break;
 		}
 		else if(value == 401)
 		{
 			//CRCore::notify(CRCore::WARN)<<"crDownloadThread: http server response 401 - Unauthorized Access : "<<m_download->getPkgFileName()<< std::endl;
			char gbuf[256];
			sprintf(gbuf,"crDownloadThread: http server response 401 - Unauthorized Access : %s\n\0",m_download->getPkgFileName().c_str());
			gDebugInfo->debugInfo(CRCore::WARN,gbuf);
 			break;
 		}
 		else if(value == 403)
 		{
 			//CRCore::notify(CRCore::WARN)<<"crDownloadThread: http server response 403 - Access forbidden : "<<m_download->getPkgFileName()<< std::endl;
			char gbuf[256];
			sprintf(gbuf,"crDownloadThread: http server response 403 - Access forbidden : %s\n\0",m_download->getPkgFileName().c_str());
			gDebugInfo->debugInfo(CRCore::WARN,gbuf);
 			break;
 		}
 		else if(value == 404)
 		{
 			//CRCore::notify(CRCore::WARN)<<"crDownloadThread: http server response 404 - File not found : "<<m_download->getPkgFileName()<< std::endl;
			char gbuf[256];
			sprintf(gbuf,"crDownloadThread: http server response 404 - File not found : %s\n\0",m_download->getPkgFileName().c_str());
			gDebugInfo->debugInfo(CRCore::WARN,gbuf);
 			break;
 		}
 		else if(value == 405)
 		{
 			//CRCore::notify(CRCore::WARN)<<"crDownloadThread: http server response 405 - Method Not Allowed : "<<m_download->getPkgFileName()<< std::endl;
			char gbuf[256];
			sprintf(gbuf,"crDownloadThread: http server response 405 - Method Not Allowed : %s\n\0",m_download->getPkgFileName().c_str());
			gDebugInfo->debugInfo(CRCore::WARN,gbuf);
 			break;
 		}
 		int filesize = 0;
 		crArgumentParser::readKeyValue(StrVec,"Content-Length",filesize);
		if(filesize<=0)
			break;
 		m_download->setSize(filesize);
 
 		front_of_line += 2;
 		m_download->recvstream()->write(m_buf+front_of_line,count-front_of_line);
 		while (!m_done && m_download->recvstream()->getBufSize() < filesize)
 		{
 			count = sio->rdbuf()->read(m_buf,BUFSIZE);
 			if(count>0)
 				m_download->recvstream()->write(m_buf,count);
 			else
 			{
 				break;
 			}
 		}
 		success = m_download->recvstream()->getBufSize() == filesize;
 		if(success)
 		{
			m_download->recvstream()->seekBegin();
 			CRCore::Timer_t end_tick = CRCore::Timer::instance()->tick();
 			//CRCore::notify(CRCore::ALWAYS)<<m_download->getPkgFileName()<<" 下载耗时:"<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
			char gbuf[256];
			sprintf(gbuf,"%s 下载耗时:%f\n\0",m_download->getPkgFileName().c_str(),CRCore::Timer::instance()->delta_s(start_tick,end_tick));
			gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
 			m_download->setDownloadState(crDownload::UnCompress);
 			//解压缩
 			if(!m_download->isinifile() && !m_download->is7zfile())
 				CRNet::creUnCompress(m_download->recvstream());
 			//完成
 			m_download->saveFile();
 		}
		else
		{
			int recvdsize = m_download->recvstream()->getBufSize();
			if(recvdsize>filesize)
			{
				char gbuf[256];
				sprintf(gbuf,"%s recvdsize大于filesize recvdsize=%d,filesize=%d\n\0",m_download->getPkgFileName().c_str(),recvdsize,filesize);
				gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
				//m_download->recvstream()->setBufSize(filesize);
			}
		}
 	} while (0);
}
catch( sockerr e )
{
	char gbuf[256];
	sprintf(gbuf,"%s 下载失败,错误码：%d,%s\n\0",m_download->getPkgFileName().c_str(),e.serrno(),e.errstr());
	gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
}
 	if(!success)
 	{
 		//CRCore::notify(CRCore::ALWAYS)<<m_download->getPkgFileName()<<" 下载失败"<<std::endl;
		char gbuf[256];
		sprintf(gbuf,"%s 下载失败,重新下载!\n\0",m_download->getPkgFileName().c_str());
		gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
		if(m_download->getDownloadState() == crDownload::Faild)
			m_download->setDownloadState(crDownload::Wait);
		else
			m_download->setDownloadState(crDownload::Faild);
		crDownloadManager::getInstance()->addDownloadRequest(m_download.get());
 	}
	else
 		crDownloadManager::getInstance()->complete(m_download.get());
 	m_download = NULL;
 	sio = NULL;
 }
//void crDownloadThread::downloadImplementation()
//{
//	char buffer[BUFSIZE];
//	DWORD bufferSize = BUFSIZE;
//	DWORD index = 0;
//	bool success = false;
//	CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();
//	//打开Internet连接
//	HINTERNET hOpen = NULL;
//	HINTERNET hConnect = NULL;
//	HINTERNET hHttp = NULL;
//	do 
//	{
//		hOpen = ::InternetOpen("Creature3D",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
//		if(hOpen == NULL)
//		{
//			CRCore::notify(CRCore::WARN)<<"Internet Open Fail: "<<m_download->getPkgFileName()<< std::endl;
//			break;
//		}
//		//创建Internet连接
//		std::string strHostName = crDownloadManager::getInstance()->getHost();
//		hConnect = ::InternetConnect(hOpen,strHostName.c_str(),INTERNET_DEFAULT_HTTP_PORT,NULL,NULL,INTERNET_SERVICE_HTTP,0,0);
//		if(NULL == hConnect)
//		{
//			CRCore::notify(CRCore::WARN)<<"Internet Connect Fail: "<<m_download->getPkgFileName()<< std::endl;
//			break;
//		}
//		//http请求
//		std::string requestAdd = "/" + CRIOManager::convertFileNameToUnixStyle(m_download->getPkgFileName());
//		const char * pFileType = "*/*";
//		hHttp = ::HttpOpenRequest(hConnect,"GET",requestAdd.c_str(),HTTP_VERSION,NULL,&pFileType,INTERNET_FLAG_RELOAD,NULL);
//		if(NULL == hHttp)
//		{
//			CRCore::notify(CRCore::WARN)<<"Http OpenRequest Fail: "<<m_download->getPkgFileName()<< std::endl;
//			break;
//		}
//		//测试Http连接
//		bool bSendRequest = ::HttpSendRequest(hHttp,NULL,0,NULL,0);
//		if(!bSendRequest)
//		{
//			break;
//		}
//		//获取文件大小
//		bool bQueryInfo = ::HttpQueryInfo(hHttp,HTTP_QUERY_CONTENT_LENGTH,buffer,&bufferSize,&index);
//		if(!bQueryInfo)
//		{
//			break;
//		}
//		int fileSize = atoi(buffer);
//		m_download->setSize(fileSize);
//		//读取写入文件
//		DWORD dwReadSize = 0;
//		while(true)
//		{
//			memset( m_buf, 0, BUFSIZE );
//			bool ReadReturn = InternetReadFile(hHttp,	(LPVOID)m_buf,bufferSize,&dwReadSize);
//			if(!ReadReturn)break;
//			if(dwReadSize==0)break;
//			m_download->recvstream()->write(m_buf,dwReadSize);
//		}
//		success = m_download->recvstream()->getBufSize() == fileSize;
//		if(success)
//		{
//			CRCore::Timer_t end_tick = CRCore::Timer::instance()->tick();
//			CRCore::notify(CRCore::ALWAYS)<<m_download->getPkgFileName()<<" 下载耗时:"<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
//			m_download->setDownloadState(crDownload::UnCompress);
//			//解压缩
//			if(!m_download->isinifile())
//				CRNet::creUnCompress(m_download->recvstream());
//			//完成
//			m_download->saveFile();
//		}
//		else
//		{
//			CRCore::notify(CRCore::ALWAYS)<<m_download->getPkgFileName()<<" 下载失败"<<std::endl;
//			m_download->setDownloadState(crDownload::Faild);
//		}
//	} while (0);
//	m_download = NULL;
//	crDownloadManager::getInstance()->complete(m_download.get());
//	if(hHttp) InternetCloseHandle(hHttp);
//	if(hConnect) InternetCloseHandle(hConnect);
//	if(hOpen) InternetCloseHandle(hOpen);
//}
void crDownloadThread::run()
{
#if _DEBUG
	try
	{
#endif
		m_done = false;
		//setSchedulePriority(THREAD_PRIORITY_HIGH);
		while(!m_done)
		{
			if(!m_download.valid())
			{
				crDownloadManager::getInstance()->getTask(this);
			}
			if(m_download.valid())
			{
				try
				{
					downloadImplementation();
				}
				catch (...)
				{
					
				}
				//yieldCurrentThread();
			}
			else
			{
				sleep(250);
			}
		}
#if _DEBUG
		CRCore::notify(CRCore::ALWAYS)<<"crDownload end"<<std::endl;
	}
	catch (...)
	{
		CRCore::notify(CRCore::ALWAYS)<<"crDownload::run() error "<<std::endl;
	}
#endif
}
///////////////////////////////////
//
//crDownloadManager
//
/////////////////////////////////////
ref_ptr<crDownloadManager> crDownloadManager::m_instance = NULL;

crDownloadManager* crDownloadManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crDownloadManager;
	}
	return m_instance.get();
}
void crDownloadManager::clear()
{
	m_downloadThreads.clear();
	m_instance = NULL;
}
crDownloadManager::crDownloadManager():
m_fileRootDir("jxjpkg"),
m_taskCount(1),
m_completeCount(0),
m_dynamicDownloadRecord(false)
{
}

crDownloadManager::~crDownloadManager() 
{
	if(m_dynamicDownloadTab.valid())
	{
		m_dynamicDownloadTab->saveToFileNoCook("dynamicRec.tab");
	}
}
void crDownloadManager::start(int threadcount)
{
	int curcount = m_downloadThreads.size();
	if(curcount<threadcount)
	{
		CRCore::ref_ptr<crDownloadThread> downloadThread;
		for (int i = curcount; i<threadcount; i++)
		{
			downloadThread = new crDownloadThread;
			downloadThread->start();
			m_downloadThreads.push_back(downloadThread);
		}
	}
	else if(curcount>threadcount)
	{
		while (curcount>threadcount)
		{
			m_downloadThreads.pop_back();
			curcount--;
		}
	}
}
bool crDownloadManager::addDownloadRequest(CRCore::ref_ptr<crDownload> download)
{
	if(m_dynamicDownloadRecord)
		return false;
	CRCore::ScopedLock<crCriticalMutex> lock1(m_downloadMapMutex);
	DownloadMap::iterator itr = m_downloadMap.find(download->getFileName());
	if(itr == m_downloadMap.end())
	{
		CRCore::ScopedLock<crCriticalMutex> lock2(m_requestDequeMutex);
		m_requestDeque.push_back(download);
		m_downloadMap[download->getFileName()] = download;
		return true;
	}
	else if(itr->second == download && itr->second->getDownloadState() == crDownload::Faild)
	{
		CRCore::ScopedLock<crCriticalMutex> lock2(m_requestDequeMutex);
		m_requestDeque.push_back(download);
		return true;
	}
	return false;
}
void crDownloadManager::addImmediatelyDownloadRequest(CRCore::ref_ptr<crDownload> download)
{
	CRCore::ScopedLock<crCriticalMutex> lock1(m_downloadMapMutex);
	DownloadMap::iterator itr = m_downloadMap.find(download->getFileName());
	if(itr == m_downloadMap.end()/* || (itr->second->getDownloadState() == crDownload::Faild && itr->second == download) */)
	{
		CRCore::ScopedLock<crCriticalMutex> lock2(m_immediatelyRequestDequeMutex);
		m_immediatelyRequestDeque.push_back(download);
		m_downloadMap[download->getFileName()] = download;
		if(m_dynamicDownloadRecord)
		{
			CREncapsulation::crTableIO::StrVec record;
			record.push_back(download->getFileName());
			m_dynamicDownloadTab->addData(record);
		}
	}
	else
	{//检查是否存在requestDeque里
		bool found = false;
		CRCore::ScopedLock<crCriticalMutex> lock3(m_immediatelyRequestDequeMutex);
		for( DownloadDeque::iterator ditr = m_immediatelyRequestDeque.begin();
			ditr != m_immediatelyRequestDeque.end();
			++ditr )
		{
			if(ditr->get() == itr->second.get())
			{
				found = true;
				break;
			}
		}
		if(!found)
		{
			CRCore::ScopedLock<crCriticalMutex> lock4(m_requestDequeMutex);
			for( DownloadDeque::iterator ditr = m_requestDeque.begin();
				ditr != m_requestDeque.end();
				++ditr )
			{
				if(ditr->get() == itr->second.get())
				{
					found = true;
					m_immediatelyRequestDeque.push_back(ditr->get());
					m_requestDeque.erase(ditr);
					break;
				}
			}
		}
	}
}
void crDownloadManager::getDownload(CRCore::ref_ptr<crDownload> &download)
{
	CRCore::ScopedLock<crCriticalMutex> lock(m_requestDequeMutex);
	if(!m_requestDeque.empty())
	{
		download = m_requestDeque.front();
		pushDownloadingDeque(download);
		m_requestDeque.pop_front();
	}
}
void crDownloadManager::getImmediatelyDownload(CRCore::ref_ptr<crDownload> &download)
{
	CRCore::ScopedLock<crCriticalMutex> lock(m_immediatelyRequestDequeMutex);
	if(!m_immediatelyRequestDeque.empty())
	{
		download = m_immediatelyRequestDeque.front();
		pushDownloadingDeque(download);
		m_immediatelyRequestDeque.pop_front();
	}
}
void crDownloadManager::pushDownloadingDeque(CRCore::ref_ptr<crDownload> download)
{
	CRCore::ScopedLock<crCriticalMutex> lock(m_downloadingDequeMutex);
	m_downloadingDeque.push_back(download);
}
void crDownloadManager::complete(crDownload *download)
{
	CRCore::ScopedLock<crCriticalMutex> lock(m_downloadingDequeMutex);
	for( DownloadDeque::iterator itr = m_downloadingDeque.begin();
		 itr != m_downloadingDeque.end();
		 ++itr )
	{
		if((*itr) == download)
		{
			m_downloadingDeque.erase(itr);
			break;
		}
	}
	m_completeCount++;
}
void crDownloadManager::setHost(const std::string &host)
{
	m_hostName = host;
}
const std::string &crDownloadManager::getHost()
{
	return m_hostName;
}
void crDownloadManager::setHost1(const std::string &host1)
{
	m_hostName1 = host1;
}
const std::string &crDownloadManager::getHost1()
{
	return m_hostName1;
}
void crDownloadManager::setHostDir(const std::string &dir)
{
	m_hostDir = dir;
}
const std::string &crDownloadManager::getHostDir()
{
	return m_hostDir;
}
void crDownloadManager::setFileRootDir(const std::string &dir)
{
	m_fileRootDir = dir;
}
const std::string &crDownloadManager::getFileRootDir()
{
	return m_fileRootDir;
}
void crDownloadManager::getTask(crDownloadThread *thread)
{
	//if(m_immDownloadThread == thread)
	//{
	//	getImmediatelyDownload(thread->m_download);
	//}
	//else if(m_downloadThread == thread)
	//{
	//	getImmediatelyDownload(thread->m_download);
	//	if(!thread->m_download.valid())
	//		getDownload(thread->m_download);
	//}
	getImmediatelyDownload(thread->m_download);
	if(!thread->m_download.valid())
		getDownload(thread->m_download);
}
crDownload *crDownloadManager::getCurrentDownload()
{
	CRCore::ScopedLock<crCriticalMutex> lock(m_downloadingDequeMutex);
	if(m_downloadingDeque.empty())
		return NULL;
	else
	{
		return m_downloadingDeque.back().get();
	}
}
//crDownload *crDownloadManager::getCurrentImmediatelyDownload()
//{
//	return m_immDownloadThread->m_download.get();
//}
void crDownloadManager::insertDownloadMap(crDownload *download)
{
	CRCore::ScopedLock<crCriticalMutex> lock(m_downloadMapMutex);
	m_downloadMap[download->getFileName()] = download;
}
crDownload *crDownloadManager::getDownload(const std::string &filename)
{
	CRCore::ScopedLock<crCriticalMutex> lock(m_downloadMapMutex);
	std::string file = CRIOManager::convertFileNameToUnixStyle(filename);
	DownloadMap::iterator itr = m_downloadMap.find(file);
	return itr == m_downloadMap.end()?NULL:itr->second.get();
}
void crDownloadManager::removeDownload(const std::string &filename)
{
	CRCore::ScopedLock<crCriticalMutex> lock(m_downloadMapMutex);
	m_downloadMap.erase(filename);
}
int crDownloadManager::getDownloadCount()
{
	CRCore::ScopedLock<crCriticalMutex> lock(m_downloadMapMutex);
	return m_downloadMap.size();
}
void crDownloadManager::lockDownloadMap()
{
	m_downloadMapMutex.lock();
}
crDownloadManager::DownloadMap &crDownloadManager::getDownloadMap()
{
	return m_downloadMap;
}
void crDownloadManager::unlockDownloadMap()
{
	m_downloadMapMutex.unlock();
}
bool crDownloadManager::isIdle()
{
	{
		CRCore::ScopedLock<crCriticalMutex> lock(m_immediatelyRequestDequeMutex);
		if(!m_immediatelyRequestDeque.empty())
			return false;
	}
	{
		CRCore::ScopedLock<crCriticalMutex> lock(m_requestDequeMutex);
		if(!m_requestDeque.empty())
			return false;
	}
	return true;
}
void crDownloadManager::setDynamicDownloadRecord(bool record)
{
	m_dynamicDownloadRecord = record;
	if(m_dynamicDownloadRecord)
	{
		m_dynamicDownloadTab = new crTableIO;
		CREncapsulation::crTableIO::StrVec title;
		title.push_back("file");
		m_dynamicDownloadTab->setTitleVec(title);
	}
}
bool crDownloadManager::getDynamicDownloadRecord()
{
	return m_dynamicDownloadRecord;
}