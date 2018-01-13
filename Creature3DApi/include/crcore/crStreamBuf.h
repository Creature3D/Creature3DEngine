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
#ifndef CRCORE_CRSTREAMBUF_H
#define CRCORE_CRSTREAMBUF_H 1

#include <CRCore\ref_ptr.h>
#include <CRCore\Referenced.h>
#include <CRCore\crWchMsg.h>
#include <CRCore\crVector2.h>
#include <CRCore\crVector3.h>
#include <CRCore\crVector4.h>
#include <CRCore\crPlane.h>
#include <CRCore\crQuat.h>
#include <CRCore\crArray.h>
#include <CRCore\crGuid.h>
#include <CRCore\crExport.h>
#include <CRCore\thread\crCriticalMutex.h>

#include <string>

namespace CRCore {

#define BOOLSIZE	1
#define CHARSIZE	1
#define SHORTSIZE	2
#define INTSIZE		4
#define FLOATSIZE	4
#define LONGSIZE	4
#define DOUBLESIZE	8
#define INT64SIZE	8

class CR_EXPORT crStreamBuf : public CRCore::Referenced
{
public :
	crStreamBuf(int cook = 0);
    bool createBuf(int bufSize);
	bool appendBuf(int size);
	//void releaseUselessBuf();
	inline char *getBuf(){ return m_buf; }
	inline const char *getBuf()const { return m_buf; }
	inline char *getPtr(){ return m_ptr; }
	inline int getMaxBufSize() const { return m_maxSize; }//获得容量
	int getRelMaxBufSize() const;//m_maxSize-CookSize
	inline int getBufSize() const { return m_bufSize; }//获得有效数据字节数
	inline int getRemainSize() const { return m_buf - m_ptr + m_bufSize; }//获得剩余的有效数据字节数
	inline int getRemainCapacity() const { return m_maxSize - m_bufSize; }//获得剩余容量
	inline void setBufSize(int size) { if(size>=0&&size<=m_maxSize) m_bufSize = size;}
    void resetBuf();
	void clearBuf();
	//std::string makeString() const;
	//void setString(const std::string &str);
	void write(const char *buf, int size);
	void read(char *buf, int size);
	void peek(char *buf, int size);
	void seek(int size){ m_ptr += size; }
	void seekBegin(){ m_ptr = m_buf; }
	void seekEnd(){ m_ptr = m_buf + m_bufSize; }

	bool eof(){ return /**m_ptr == 0*/getRemainSize()<=0; }

	void _writeBool(bool b);
	void _writeChar(char c);
	void _writeUChar(unsigned char c);
	void _writeUShort(unsigned short s);
	void _writeShort(short s);
	void _writeUInt(unsigned int s);
	void _writeInt(int i);
	void _writeInt64(_crInt64 i);
	void _writeFloat(float f);
	void _writeLong(long l);
	void _writeULong(unsigned long l);
	void _writeDouble(double d);
	void _writeString(const std::string& s);
	void _writeCharArray(const char* data, int size);
	void _writeVec2(const CRCore::crVector2f& v);
	void _writeVec3(const CRCore::crVector3f& v);
	void _writeVec4(const CRCore::crVector4f& v);
	void _writeVec2d(const CRCore::crVector2d& v);
	void _writeVec3d(const CRCore::crVector3d& v);
	void _writeVec4d(const CRCore::crVector4d& v);
	
	void _writeVec2i(const CRCore::crVector2i& v);
	void _writeVec3i(const CRCore::crVector3i& v);
	void _writeVec4i(const CRCore::crVector4i& v);
	void _writeVec2s(const CRCore::crVector2s& v);
	void _writeVec3s(const CRCore::crVector3s& v);
	void _writeVec4s(const CRCore::crVector4s& v);
	void _writeVec2b(const CRCore::crVector2b& v);
	void _writeVec3b(const CRCore::crVector3b& v);
	void _writeVec4b(const CRCore::crVector4b& v);
	void _writeVec2ub(const CRCore::crVector2ub& v);
	void _writeVec3ub(const CRCore::crVector3ub& v);
	void _writeVec4ub(const CRCore::crVector4ub& v);

	void _writeGuid(const CRCore::crGuid& guid);
	void _writePlane(const CRCore::crPlane& v);
	void _writeUByte4(const CRCore::crVector4ub& v);
	void _writeQuat(const CRCore::crQuat& q);
	void _writeArray(const CRCore::Array* a);
	void _writeIntArray(const CRCore::IntArray* a);
	void _writeUByteArray(const CRCore::UByteArray* a);
	void _writeUShortArray(const CRCore::UShortArray* a);
	void _writeUIntArray(const CRCore::UIntArray* a);
	void _writeUByte4Array(const CRCore::Vec4ubArray* a);
	void _writeFloatArray(const CRCore::FloatArray* a);
	void _writeVec2Array(const CRCore::Vec2Array* a);
	void _writeVec3Array(const CRCore::Vec3Array* a);
	void _writeVec4Array(const CRCore::Vec4Array* a);
	void _writeMatrixf(const CRCore::crMatrixf& mat);
	void _writeMatrixd(const CRCore::crMatrixd& mat);

	bool _readBool();
	char _readChar();
	unsigned char _readUChar();
	unsigned short _readUShort();
	short _readShort();
	unsigned int _readUInt();
	int _readInt();
	bool peekBool();
	char peekChar();
	int peekInt();
	_crInt64 _readInt64();
	float _readFloat();
	long _readLong();
	unsigned long _readULong();
	double _readDouble();
	std::string _readString();
	void _readCharArray(char* data, int size);

	CRCore::crVector2f _readVec2();
	CRCore::crVector3f _readVec3();
	CRCore::crVector4f _readVec4();
	CRCore::crVector2d _readVec2d();
	CRCore::crVector3d _readVec3d();
	CRCore::crVector4d _readVec4d();

	CRCore::crVector2i _readVec2i();
	CRCore::crVector3i _readVec3i();
	CRCore::crVector4i _readVec4i();
	CRCore::crVector2s _readVec2s();
	CRCore::crVector3s _readVec3s();
	CRCore::crVector4s _readVec4s();
	CRCore::crVector2b _readVec2b();
	CRCore::crVector3b _readVec3b();
	CRCore::crVector4b _readVec4b();
	CRCore::crVector2ub _readVec2ub();
	CRCore::crVector3ub _readVec3ub();
	CRCore::crVector4ub _readVec4ub();

	CRCore::crGuid _readGuid();
	CRCore::crPlane _readPlane();
	CRCore::crVector4ub _readUByte4();
	CRCore::crQuat _readQuat();
	CRCore::crMatrixf _readMatrixf();
	CRCore::crMatrixd _readMatrixd();
	CRCore::Array* _readArray();
	CRCore::IntArray* _readIntArray();
	CRCore::UByteArray* _readUByteArray();
	CRCore::UShortArray* _readUShortArray();
	CRCore::UIntArray* _readUIntArray();
	CRCore::Vec4ubArray* _readUByte4Array();
	CRCore::FloatArray* _readFloatArray();
	CRCore::Vec2Array* _readVec2Array();
	CRCore::Vec3Array* _readVec3Array();
	CRCore::Vec4Array* _readVec4Array();
	//bool saveToFile(const std::string &filename,bool app = false);//会写入endian及bufsize
	//typedef std::vector< CRCore::ref_ptr<crStreamBuf> >LoadStreamVec;
	//static bool loadFromFile(const std::string &filename,LoadStreamVec& loadStreamVec);

	///无额外数据输出
	bool saveToFile2(const std::string &filename,bool app = false);
	bool loadFromFile2(const std::string &filename);
	void endian();
	void cook();
	bool uncook(int &code);
	std::string getMD5();
	unsigned short getMBCRC16(char *pucFrame, unsigned short usLen);
protected :
	virtual ~crStreamBuf();

private:
	char *m_buf;//起始地址
	char *m_ptr;//当前读写地址
	int m_maxSize;//总容量
	int m_bufSize;//有效数据字节数
	int m_endian;
};
class CR_EXPORT crDebugInfo : public Referenced
{
public :
	crDebugInfo(){};
	static crDebugInfo *getInstance();
	void init(const std::string &info);
	//void debugInfo(const NotifySeverity severity);
	void debugInfo(const NotifySeverity severity,char *str);
	//char *buf(){return m_buf;}
	void writeToFile();
protected :
	virtual ~crDebugInfo();
	static ref_ptr<crDebugInfo> m_instance;
	//char m_buf[256];
	std::string m_filename;
	ref_ptr<crStreamBuf> m_stream;
	crCriticalMutex m_mutex;
};
#define gDebugInfo CRCore::crDebugInfo::getInstance()
//#define OUTPUTGLERROR
}

#endif
