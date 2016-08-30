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
#ifndef CRNETAPP_DATASTREAMTRANSFER_H
#define CRNETAPP_DATASTREAMTRANSFER_H 1

#include <CRNetApp/appExport.h>
#include <CRNetApp/appDataParam.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRCore/crStreamBuf.h>
#include <CRCore/crHandleManager.h>
#include <CRCore/crDataManager.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/thread/crScopedLock.h>
#include <gnelib.h>
#include <list>
#include <string>

namespace CRNetApp{

//enum DownloadFlg
//{
//	DF_None,
//	DF_PreEnterGame,
//	DF_EnterGame,
//	DF_EnterScene,
//	DF_CreateRole,
//	DF_EnterSceneSucess,
//	DF_StartGame,
//	DF_DownLoadItem,
//	DF_DownLoadItem2,
//	DF_DownLoadItemChild,
//	DF_DownLoadItemChild2,
//	DF_CheckRoleUpdatecfg,
//	DF_EnterSceneRoom,
//	DF_InitCheck,
//	DF_InitCheckEnd,
//};
/////////////GameClient
class CRNETAPP_EXPORT crRecvDataStream : public CRCore::Referenced
{
public:
	crRecvDataStream(int streamid, int packetid, int count);
	bool compare(int streamid, int packetid);
	int getCount();
	int getStreamID();
	int getPacketID();
	void recvStream(CRCore::crStreamBuf *stream);
	CRCore::crStreamBuf *getStream();
	bool isRecvComplete();
	int getProgress();
protected:
	int m_streamid;
    int m_packetid;
	int m_count;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class CRNETAPP_EXPORT crRecvFileStream : public CRCore::Referenced
{
public:
	crRecvFileStream(const std::string &fileName, short netType, int cacheid, int count,CRCore::Timer_t mtime,CRCore::crStreamBuf *headStream);
	const std::string &getFileName()const;
	int getCount();
	int getCacheID();
	void recvStream(CRCore::crStreamBuf *stream);
	CRCore::crStreamBuf *getStream();
	bool isRecvComplete();
	int getProgress();
	unsigned short getDownloadFlg();
	short getNetType();
	void writeToFile();
	void requestStream();
protected:
	virtual ~crRecvFileStream();
	void setFileTime(const std::string &file);
	std::string m_fileName;
	short m_netType;
	int m_cacheid;
	int m_count;
	unsigned short m_downloadFlg;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	CRCore::Timer_t m_mtime;
};
class CRNETAPP_EXPORT crDataStreamTransfer : public CRCore::Referenced
{
public:
	crDataStreamTransfer();
	static crDataStreamTransfer *getInstance();
	void clear();

	typedef std::pair<int,float>RecycleIDPair;
	typedef std::deque<RecycleIDPair> RecycleIDDeque;
	int gainStreamID();
	void recycleStreamID(int id,float delay = crGlobalHandle::gData()->gRecycleIDTime());
	void update(float dt);

	typedef std::list< CRCore::ref_ptr<crRecvDataStream> >DataStreamList;
	bool createTransferingStream(int streamid, int packetid, int count);
	void recvStream(CRCore::crStreamBuf *stream);
	crRecvDataStream *findTransferingStream(int streamid, int packetid);
	void removeTransferingStream(crRecvDataStream *stream);

	typedef std::map<int, CRCore::ref_ptr<crRecvFileStream> >FileStreamMap;
	void createFileStream(const std::string &fileName, short netType, int cacheid, int count, CRCore::Timer_t mtime, CRCore::crStreamBuf *headStream);
	void recvFileStream(CRCore::crStreamBuf *stream);
	crRecvFileStream *findFileStream(int cacheid);
	void removeFileStream(int cacheid);
protected:
	virtual ~crDataStreamTransfer(){}
	static CRCore::ref_ptr<crDataStreamTransfer> m_instance;
	RecycleIDDeque m_recycleStreamIDDeque;
	GNE::Mutex m_recycleStreamIDDequeMutex;
	static int s_temporaryStreamID;

	void addTransferingStream(crRecvDataStream *stream);
	void addFileStream(crRecvFileStream *stream);
	CRCore::crMutex m_dataStreamListMutex;
	DataStreamList m_transferingStream;
	CRCore::crMutex m_fileStreamMapMutex;
	FileStreamMap m_fileStreaMap;
};
////////DownloadServer
class CRNETAPP_EXPORT crFileStreamCache : public CRCore::Referenced
{
public:
	crFileStreamCache();
	void setCacheid(int id);
	int getCacheid();
	void setFileName(const std::string &filename);
	const std::string &getFileName() const;
	void setStream(CRCore::crStreamBuf *stream);
	int getStreamSize();
	char *getStream(int pos);
	void update(float dt);
	bool issafe();
	bool timeout();
	void setMTime(CRCore::Timer_t mtime);
	CRCore::Timer_t getMTime();
protected:
	void loadFileStream();
	virtual ~crFileStreamCache();
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_cacheid;
	std::string m_fileName;
	float m_remainTime;//表示在内存里剩余时间，在没有接收者后开始计时
	CRCore::Timer_t m_mtime;
};

class CRNETAPP_EXPORT crFileStreamCacheManager : public CRCore::Referenced
{
public:
	crFileStreamCacheManager();
	static crFileStreamCacheManager *getInstance();
	void clear();

	typedef std::map< int, CRCore::ref_ptr<crFileStreamCache>, std::greater<int> >StreamCacheMap;
	crFileStreamCache *findFileStreamCache(const std::string &fileName);
	crFileStreamCache *getStreamCache(int cacheid);//注意防止cacheid过期，或者设计cacheid永远不过期
	void addStreamCache(crFileStreamCache *streamcache);
	void update(float dt);
protected:
	virtual ~crFileStreamCacheManager(){}
	static CRCore::ref_ptr<crFileStreamCacheManager> m_instance;

	GNE::Mutex m_streamCacheMapMutex;
	StreamCacheMap m_streamCacheMap;
};
}
#endif