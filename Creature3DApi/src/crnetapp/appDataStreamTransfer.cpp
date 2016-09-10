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
#include <CRNetApp/appNetMethod.h>
#include <CRNetApp/appDataStreamTransfer.h>
#include <CRNetApp/appDataClass.h>
#include <CRNetApp/appMsg.h>
#include <CRNetApp/appNetPackets2.h>
#include <CRNet/crNetContainer.h>
#include <CRCore/crNotify.h>
#include <CRIOManager/crCookFile.h>
#include <CRIOManager/crFileUtils.h>
using namespace CRNetApp;
using namespace CRCore;
using namespace CRNet;
/////////////////////////////////////////
//
//crRecvDataStream
//
/////////////////////////////////////////
crRecvDataStream::crRecvDataStream(int streamid, int packetid, int count):
m_streamid(streamid),
m_packetid(packetid),
m_count(count)
{
	m_stream = new CRCore::crStreamBuf;
	m_stream->createBuf(count);
}
bool crRecvDataStream::compare(int streamid, int packetid)
{
    return m_streamid == streamid && m_packetid == packetid;
}
int crRecvDataStream::getCount()
{
	return m_count;
}
int crRecvDataStream::getStreamID()
{
	return m_streamid;
}
int crRecvDataStream::getPacketID()
{
	return m_packetid;
}
void crRecvDataStream::recvStream(CRCore::crStreamBuf *stream)
{
    //m_recvedCount += stream->getRemainSize();
	m_stream->write(stream->getPtr(),stream->getRemainSize());
	crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_RecveDataStream,m_packetid),MAKEINT64(this,NULL));
}
CRCore::crStreamBuf *crRecvDataStream::getStream()
{
	if(m_stream.valid()) m_stream->seekBegin();
	return m_stream.get();
}
bool crRecvDataStream::isRecvComplete()
{
	return m_stream->getBufSize() == m_count;//m_recvedCount==m_count;
}
int crRecvDataStream::getProgress()
{
    return ((float)(m_stream->getBufSize())/(float)(m_count)) * 100.0f;
}
/////////////////////////////////////////
//
//crRecvFileStream
//
/////////////////////////////////////////
crRecvFileStream::crRecvFileStream(const std::string &fileName, short netType, int cacheid, int count,CRCore::Timer_t mtime,CRCore::crStreamBuf *headStream):
m_fileName(fileName),
m_netType(netType),
m_cacheid(cacheid),
m_count(count),
m_mtime(mtime)
{
	m_downloadFlg = headStream->_readUShort();
	m_stream = new CRCore::crStreamBuf;
	m_stream->createBuf(count);
	requestStream();
}
crRecvFileStream::~crRecvFileStream()
{

}
const std::string &crRecvFileStream::getFileName()const
{
	return m_fileName;
}
int crRecvFileStream::getCount()
{
	return m_count;
}
int crRecvFileStream::getCacheID()
{
	return m_cacheid;
}
void crRecvFileStream::requestStream()
{
	crRequestFileStreamPacket packet;
	crRequestFileStreamPacket::buildRequestPacket(packet,m_cacheid,m_stream->getBufSize());
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(m_netType);
	if(netConductor)
		netConductor->getNetManager()->sendPacket("all",packet);
}
void crRecvFileStream::recvStream(CRCore::crStreamBuf *stream)
{
	//m_recvedCount += stream->getRemainSize();
	m_stream->write(stream->getPtr(),stream->getRemainSize());
	crGlobalHandle::getInstance()->doEvent(WCH_RecvingFileStream,MAKEINT64(this,NULL));
}
CRCore::crStreamBuf *crRecvFileStream::getStream()
{
	if(m_stream.valid()) m_stream->seekBegin();
	return m_stream.get();
}
bool crRecvFileStream::isRecvComplete()
{
	return m_stream->getBufSize() == m_count;//m_recvedCount==m_count;
}
int crRecvFileStream::getProgress()
{
	return ((float)(m_stream->getBufSize())/(float)(m_count)) * 100.0f;
}
unsigned short crRecvFileStream::getDownloadFlg()
{
	return m_downloadFlg;
}
short crRecvFileStream::getNetType()
{
	return m_netType;
}
void crRecvFileStream::writeToFile()
{
	if(!m_fileName.empty())
	{
		if(m_stream.valid())
		{
			//CRCore::notify(CRCore::ALWAYS)<<"RecvDownLoadFile "<<m_fileName<<std::endl;
			char gbuf[256];
			sprintf(gbuf,"RecvDownLoadFile %s\n\0",m_fileName.c_str());
			gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
			//std::string cookfilename = m_fileName;
			//CRIOManager::GetCookedFileName(cookfilename);
			//CRIOManager::makeDirectoryForFile(cookfilename);
			m_stream->seekBegin();
			CRNet::creUnCompress(m_stream);
			m_stream->saveToFile2(m_fileName);
			setFileTime(m_fileName);
			//if(cookfilename.find("Creature3DUpdate.exe") != std::string::npos)
			//{
			//	//crInitCheckEndMethod::needUpdate(2);
			//}
		}
	}
}
void crRecvFileStream::setFileTime(const std::string &file)
{
	FILETIME ft;
	LONGLONG ll = Int32x32To64(m_mtime, 10000000) + 116444736000000000;
	ft.dwLowDateTime = (DWORD) ll;
	ft.dwHighDateTime = (DWORD)(ll >> 32);
	HANDLE handle;
	handle=CreateFile(file.c_str(),GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
	SetFileTime(handle,(LPFILETIME) NULL, (LPFILETIME) NULL, &ft);
	CloseHandle(handle);
}
/////////////////////////////////////////
//
//crDataStreamTransfer
//
/////////////////////////////////////////
ref_ptr<crDataStreamTransfer> crDataStreamTransfer::m_instance = NULL;
int crDataStreamTransfer::s_temporaryStreamID = 0;
crDataStreamTransfer::crDataStreamTransfer()
{
}
crDataStreamTransfer* crDataStreamTransfer::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crDataStreamTransfer;
	}
	return m_instance.get();
}

void crDataStreamTransfer::clear()
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataStreamListMutex);
	CRCore::ScopedLock<CRCore::crMutex> lock1(m_fileStreamMapMutex);
	m_instance = NULL;
}
int crDataStreamTransfer::gainStreamID()
{
	int id;
	GNE::LockMutex lock( m_recycleStreamIDDequeMutex );
	if(!m_recycleStreamIDDeque.empty())
	{
		RecycleIDDeque::iterator itr = m_recycleStreamIDDeque.begin();
		if(itr->second>crGlobalHandle::gData()->gRecycleIDTime())
		{
			id = itr->first;
			m_recycleStreamIDDeque.pop_front();
			return id;
		}
	}
	id = s_temporaryStreamID;
	s_temporaryStreamID++;
	return id;
}
void crDataStreamTransfer::recycleStreamID(int id,float delay)
{
	GNE::LockMutex lock( m_recycleStreamIDDequeMutex );
	for( RecycleIDDeque::iterator itr = m_recycleStreamIDDeque.begin();
		itr != m_recycleStreamIDDeque.end();
		++itr )
	{
		if(itr->first == id)
		{
			itr->second = crGlobalHandle::gData()->gRecycleIDTime() - delay;
			return;
		}
	}
	m_recycleStreamIDDeque.push_back(std::make_pair(id,crGlobalHandle::gData()->gRecycleIDTime()-delay));
}
void crDataStreamTransfer::update(float dt)
{
	GNE::LockMutex lock( m_recycleStreamIDDequeMutex );
	for( RecycleIDDeque::iterator itr = m_recycleStreamIDDeque.begin();
		itr != m_recycleStreamIDDeque.end();
		++itr )
	{
		if(itr->second<=crGlobalHandle::gData()->gRecycleIDTime())
			itr->second+=dt;
	}
}
////////////////DataStream
bool crDataStreamTransfer::createTransferingStream(int streamid, int packetid, int count)
{
	if(count>0)
	{
		crRecvDataStream *dataStream = findTransferingStream(streamid,packetid);
		if(!dataStream)
		{
			addTransferingStream(new crRecvDataStream(streamid,packetid,count));
			return true;
		}
	}
	return false;
}

crRecvDataStream *crDataStreamTransfer::findTransferingStream(int streamid, int packetid)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataStreamListMutex);
	for( DataStreamList::iterator itr = m_transferingStream.begin();
		itr != m_transferingStream.end(); 
		++itr )
	{
		if((*itr)->compare(streamid,packetid))
		{
			return itr->get();
		}
	}
	return NULL;
}
void crDataStreamTransfer::addTransferingStream(crRecvDataStream *stream)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataStreamListMutex);
	m_transferingStream.push_back(stream);
}
void crDataStreamTransfer::removeTransferingStream(crRecvDataStream *stream)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataStreamListMutex);
	for( DataStreamList::iterator itr = m_transferingStream.begin();
		itr != m_transferingStream.end(); 
		++itr )
	{
		if(itr->get() == stream)
		{
			m_transferingStream.erase(itr);
			return;
		}
	}
}
void crDataStreamTransfer::recvStream(CRCore::crStreamBuf *stream)
{
	if(stream)
	{		
		//int recvid = stream->_readInt();
		int streamid = stream->_readInt();//streamid = senderid
		int packetid = stream->_readInt();

		CRCore::ref_ptr<crRecvDataStream> dataStream = findTransferingStream(streamid,packetid);
		if(dataStream.valid())
		{
			dataStream->recvStream(stream);
			if(dataStream->isRecvComplete())
			{
				removeTransferingStream(dataStream.get());
				crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_RecvedDataStream,packetid),MAKEINT64(dataStream.get(),streamid));
			}
		}
	}
}
///////////FileStream
void crDataStreamTransfer::createFileStream(const std::string &fileName, short netType, int cacheid, int count, CRCore::Timer_t mtime, CRCore::crStreamBuf *headStream)
{
	crRecvFileStream *fileStream = findFileStream(cacheid);
	if(!fileStream)
	{
		addFileStream(new crRecvFileStream(fileName,netType,cacheid,count,mtime,headStream));
	}
	else
	{
		if(fileStream->getFileName().compare(fileName) != 0)
		{
			addFileStream(new crRecvFileStream(fileName,netType,cacheid,count,mtime,headStream));
		}
	}
}
crRecvFileStream *crDataStreamTransfer::findFileStream(int cacheid)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_fileStreamMapMutex);
	FileStreamMap::iterator itr = m_fileStreaMap.find(cacheid);
	return itr != m_fileStreaMap.end() ? itr->second.get() : NULL;
}
void crDataStreamTransfer::recvFileStream(CRCore::crStreamBuf *stream)
{
	if(stream)
	{		
		int cacheid = stream->_readInt();
		bool sucess = stream->_readBool();
		CRCore::ref_ptr<crRecvFileStream> fileStream = findFileStream(cacheid);
        if (fileStream.valid())
        {
			if(!sucess)
			{
				removeFileStream(cacheid);
				if(fileStream->isRecvComplete())
				{
					fileStream->writeToFile();
					crGameClientData *data = dynamic_cast<crGameClientData *>(crGlobalHandle::getInstance()->getDataClass());
					data->recvedFileStream(fileStream.get());
				}
				//else
				//{//save to file,下次要下载的时候先从硬盘取，然后续传

				//}
			}
			else
			{
				fileStream->recvStream(stream);
				if(fileStream->isRecvComplete())
				{
					fileStream->writeToFile();
					removeFileStream(cacheid);
					crGameClientData *data = dynamic_cast<crGameClientData *>(crGlobalHandle::getInstance()->getDataClass());
					data->recvedFileStream(fileStream.get());
				}
				else
				{
					fileStream->requestStream();
				}
			}
        }
	}
}
void crDataStreamTransfer::addFileStream(crRecvFileStream *stream)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_fileStreamMapMutex);
	m_fileStreaMap[stream->getCacheID()] = stream;
}
void crDataStreamTransfer::removeFileStream(int cacheid)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_fileStreamMapMutex);
	m_fileStreaMap.erase(cacheid);
}
//void crDataStreamTransfer::addCompleteStream(crRecvDataStream *stream)
//{
//}
//void crDataStreamTransfer::removeCompleteStream(crRecvDataStream *stream)
//{
//}
/////////////////////////////////////////
//
//crFileStreamCache
//
/////////////////////////////////////////
crFileStreamCache::crFileStreamCache():
m_cacheid(0),
m_remainTime(crGlobalHandle::gData()->gStreamCacheMaxTime())
{
}
crFileStreamCache::~crFileStreamCache()
{

}
void crFileStreamCache::setCacheid(int id)
{
	m_cacheid = id;
}
int crFileStreamCache::getCacheid()
{
	return m_cacheid;
}
void crFileStreamCache::setFileName(const std::string &filename)
{
	m_fileName = filename;
}
const std::string &crFileStreamCache::getFileName() const
{
	return m_fileName;
}
void crFileStreamCache::setStream(CRCore::crStreamBuf *stream)
{
	m_stream = stream;
}
int crFileStreamCache::getStreamSize()
{
	if(!m_stream.valid())
	{
		if(!m_fileName.empty())
		{
			loadFileStream();
		}
	}
	if (m_stream.valid())
	{
		return m_stream->getBufSize();
	}
	return 0;
}
char *crFileStreamCache::getStream(int pos)
{
    if(!m_stream.valid())
	{
		if(!m_fileName.empty())
		{
			loadFileStream();
		}
	}
	if (m_stream.valid())
	{
		if(m_stream->getBufSize()>pos)
		{
			m_remainTime = crGlobalHandle::gData()->gStreamCacheMaxTime();
			return m_stream->getBuf() + pos;
		}
	}
	return NULL;
}
void crFileStreamCache::update(float dt)
{
	m_remainTime -= dt;
}
bool crFileStreamCache::issafe()
{
	return m_remainTime>1.0f;
}
bool crFileStreamCache::timeout()
{
	return m_remainTime<0.0f;
}
void crFileStreamCache::setMTime(CRCore::Timer_t mtime)
{
	m_mtime = mtime;
}
CRCore::Timer_t crFileStreamCache::getMTime()
{
	return m_mtime;
}
void crFileStreamCache::loadFileStream()
{
	if(!m_fileName.empty())
	{
		std::string cookfilename = m_fileName;
		CRIOManager::GetCookedFileName(cookfilename);
		m_stream = new crStreamBuf;
		if(!m_stream->loadFromFile2(cookfilename))
		{
			CRCore::notify(CRCore::ALWAYS)<<"crFileStreamCache::loadFileStream() "<<m_fileName<<" not found"<<std::endl;
			//sprintf(gDebugInfo->buf(),"crFileStreamCache::loadFileStream() %s not found\n\0",m_fileName.c_str());
			//gDebugInfo->debugInfo(CRCore::ALWAYS);
			m_stream = NULL;
		}
		CRNet::creCompress(m_stream);
	}
}
/////////////////////////////////////////
//
//crFileStreamCacheManager
//
/////////////////////////////////////////
ref_ptr<crFileStreamCacheManager> crFileStreamCacheManager::m_instance = NULL;
crFileStreamCacheManager::crFileStreamCacheManager()
{
}
crFileStreamCacheManager* crFileStreamCacheManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crFileStreamCacheManager;
	}
	return m_instance.get();
}

void crFileStreamCacheManager::clear()
{
	GNE::LockMutex lock( m_streamCacheMapMutex );
	m_instance = NULL;
}
crFileStreamCache *crFileStreamCacheManager::findFileStreamCache(const std::string &fileName)
{
	if(fileName.empty()) return NULL;
	GNE::LockMutex lock( m_streamCacheMapMutex );
	for( StreamCacheMap::iterator itr = m_streamCacheMap.begin();
		itr != m_streamCacheMap.end();
		++itr )
	{
		if(fileName.compare(itr->second->getFileName()) == 0 && itr->second->issafe())
			return itr->second.get();
	}
	return NULL;
}
crFileStreamCache *crFileStreamCacheManager::getStreamCache(int cacheid)
{//设计file cacheid永不过期,临时cacheid采用回收制度
	GNE::LockMutex lock( m_streamCacheMapMutex );
	StreamCacheMap::iterator itr = m_streamCacheMap.find(cacheid);
	if (itr != m_streamCacheMap.end() && itr->second->issafe())
	{
		return itr->second.get();
	}
	return NULL;
}
void crFileStreamCacheManager::addStreamCache(crFileStreamCache *streamcache)
{
	GNE::LockMutex lock( m_streamCacheMapMutex );
	if(m_streamCacheMap.empty())
	{
		streamcache->setCacheid(0);
		m_streamCacheMap[0] = streamcache;
	}
	else
	{
		//for( StreamCacheMap::iterator itr = m_streamCacheMap.begin();
		//	itr != m_streamCacheMap.end();
		//	++itr )
		//{
		//	if(itr->second == streamcache)
		//	{
		//		return;
		//	}
		//}
		int id = m_streamCacheMap.begin()->first + 1;
		streamcache->setCacheid(id);
		m_streamCacheMap[id] = streamcache;
	}
}
void crFileStreamCacheManager::update(float dt)
{
	GNE::LockMutex lock( m_streamCacheMapMutex );
	//std::vector<int> EraseVec;
	for( StreamCacheMap::iterator itr = m_streamCacheMap.begin();
		 itr != m_streamCacheMap.end();
		 )
	{
		itr->second->update(dt);
		if(itr->second->timeout())
		{
			//EraseVec.push_back(itr->first);
			itr = m_streamCacheMap.erase(itr);
		}
		else
		{
			++itr;
		}
	}
	//for( std::vector< int >::iterator itr = EraseVec.begin();
	//	 itr != EraseVec.end();
	//	 ++itr )
	//{
	//	m_streamCacheMap.erase(*itr);
	//}
}

