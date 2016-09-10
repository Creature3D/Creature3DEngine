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
#include <CRCore/crStreamBuf.h>
#include <CRCore/crEndian.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRCore/crRange.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "windows.h"
using namespace CRCore;
#define CookSize 4
crStreamBuf::crStreamBuf(int cook):
m_buf(NULL),
m_ptr(NULL),
m_maxSize(0),
m_bufSize(0),
m_endian(cook)
{
	//createBuf(256);
}

crStreamBuf::~crStreamBuf()
{
	//CRCore::notify(CRCore::FATAL)<<"~crStreamBuf() "<<m_maxSize<<std::endl;
	if(m_buf) delete [] m_buf;
	m_buf = NULL;
	m_ptr = NULL;
}
bool crStreamBuf::createBuf(int bufSize)
{
	if(bufSize<=0) return false;

	if(m_buf) delete [] m_buf;
	bufSize+=CookSize;
	m_buf = new char[bufSize];//(char*)malloc(bufSize);
	if(m_buf) 
	{
		memset(m_buf,0,bufSize);
		m_ptr = m_buf;
		m_maxSize = bufSize;
		m_bufSize = 0;
		return true;
	}
	else
	{
		CRCore::notify(CRCore::FATAL)<<"crStreamBuf::createBuf error"<<std::endl;
	}
	return false;
}
int crStreamBuf::getRelMaxBufSize() const
{
	return m_maxSize - CookSize;
}
bool crStreamBuf::appendBuf(int size)
{
	if(size<=0) return true;
	char *buf = new char[m_maxSize + size];
	if(buf)
	{
        m_maxSize += size;
		memset(buf,0,m_maxSize);
		if(m_buf) 
		{
            memcpy(buf,m_buf,m_bufSize);		
			delete [] m_buf;
		}
		m_buf = buf;
		m_ptr = m_buf + m_bufSize;
		return true;
	}
	else
	{
		CRCore::notify(CRCore::FATAL)<<"crStreamBuf::appendBuf error"<<std::endl;
	}
	return false;
}
//void crStreamBuf::releaseUselessBuf()
//{
//	if(m_buf)
//	{
//		if(m_bufSize<=0) clearBuf();
//		else
//		{
//			char *buf = new char[m_bufSize];
//			if(buf)
//			{
//				memcpy(buf,m_buf,m_bufSize);		
//				delete [] m_buf;
//
//				m_buf = buf;
//				m_ptr = m_buf;
//			}
//		}
//	}
//}
void crStreamBuf::resetBuf()
{
    m_ptr = m_buf;
	m_bufSize = 0;
	//memset(m_ptr,0,m_maxSize);
}

void crStreamBuf::clearBuf()
{
	if(m_buf) delete [] m_buf;
	m_buf = NULL;
	m_ptr = NULL;
    m_bufSize = 0;
	m_maxSize = 0;
}

//std::string crStreamBuf::makeString() const
//{
//	CRCore::notify(CRCore::NOTICE)<<"crStreamBuf::makeString():"<<std::endl;
//
//	for(int i = 0; i< m_bufSize; ++i)
//		CRCore::notify(CRCore::NOTICE)<<(int)(m_buf[i]);
//	CRCore::notify(CRCore::NOTICE)<<std::endl;
//
//
//	std::string str;
//	////str.copy(m_buf,m_bufSize);
//    str.insert(0,m_buf,m_bufSize);
//	return str;
//}

//void crStreamBuf::setString(const std::string &str)
//{
//	if(m_bufSize>0) clearBuf();
//	createBuf(str.size());
//	memcpy(m_buf,str.c_str(),str.size());
//}

void crStreamBuf::write(const char *buf, int size)
{
	if(m_ptr - m_buf + size > m_maxSize)
	{
		//CRCore::notify(CRCore::FATAL)<<"crStreamBuf::write 缓存溢出 "<<std::endl;
		////_asm   int   3   //只是为了让程序崩溃
		////////抛出异常
		//////int *e;
		//////*e = 0;
		//return;
		if(m_maxSize == 0)
			createBuf(CRCore::maximum(32,size));
		else
			appendBuf(CRCore::maximum(m_maxSize,size+CookSize));
	}

    memcpy(m_ptr,buf,size);
	if (m_endian && size>0) 
	{
		for(char *p = m_ptr; p < m_ptr+size; p++)
		{
			ENDIAN(*p);
		}
	}
    m_ptr += size;
	m_bufSize = m_ptr - m_buf;
}

void crStreamBuf::read(char *buf, int size)
{
	if(m_ptr - m_buf + size > m_maxSize)
	{
		CRCore::notify(CRCore::FATAL)<<"crStreamBuf::read 缓存溢出 "<<std::endl;
		//char gbuf[256];
		//sprintf(gbuf,"crStreamBuf::read 缓存溢出\n\0");
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		//_asm   int   3   //只是为了让程序崩溃
		//////抛出异常
		////int *e;
		////*e = 0;
		return;
	}

	memcpy(buf,m_ptr,size);
	if (m_endian && size>0)
	{
		for(char *p = m_ptr; p < m_ptr+size; p++)
		{
			ENDIAN(*p);
		}
	}
	m_ptr += size;
}

void crStreamBuf::peek(char *buf, int size)
{
	if(m_ptr - m_buf + size > m_maxSize)
	{
		CRCore::notify(CRCore::FATAL)<<"crStreamBuf::peek 缓存溢出 "<<std::endl;
		//抛出异常
		//int *e;
		//*e = 0;
		return;
	}

	memcpy(buf,m_ptr,size);
	if (m_endian && size>0) 
	{
		for(char *p = m_ptr; p < m_ptr+size; p++)
		{
			ENDIAN(*p);
		}
	}
}

void crStreamBuf::_writeBool(bool b)
{
	char c = b?1:0;
	write(&c, CHARSIZE);
}

void crStreamBuf::_writeChar(char c)
{
	write(&c, CHARSIZE);
}

void crStreamBuf::_writeUChar(unsigned char c)
{
	write((char*)&c, CHARSIZE);
}

void crStreamBuf::_writeUShort(unsigned short s)
{
	write((char*)&s, SHORTSIZE);
}

void crStreamBuf::_writeShort(short s)
{
	write((char*)&s, SHORTSIZE);
}

void crStreamBuf::_writeUInt(unsigned int s)
{
	write((char*)&s, INTSIZE);
}

void crStreamBuf::_writeInt(int i)
{
	write((char*)&i, INTSIZE);
}

void crStreamBuf::_writeInt64(_crInt64 i)
{
	write((char*)&i, INT64SIZE);
}

void crStreamBuf::_writeFloat(float f)
{
	write((char*)&f, FLOATSIZE);
}

void crStreamBuf::_writeLong(long l)
{
	write((char*)&l, LONGSIZE);
}

void crStreamBuf::_writeULong(unsigned long l)
{
	write((char*)&l, LONGSIZE);
}

void crStreamBuf::_writeDouble(double d)
{
	write((char*)&d, DOUBLESIZE);
}

void crStreamBuf::_writeString(const std::string& s)
{
	_writeInt(s.size());
	write(s.c_str(), s.size());
}

void crStreamBuf::_writeCharArray(const char* data, int size)
{
	write(data, size);
}

void crStreamBuf::_writeVec2(const CRCore::crVector2f& v)
{
	_writeFloat(v.x());
	_writeFloat(v.y());
}

void crStreamBuf::_writeVec3(const CRCore::crVector3f& v)
{
	_writeFloat(v.x());
	_writeFloat(v.y());
	_writeFloat(v.z());
}

void crStreamBuf::_writeVec4(const CRCore::crVector4f& v)
{
	_writeFloat(v.x());
	_writeFloat(v.y());
	_writeFloat(v.z());
	_writeFloat(v.w());
}

void crStreamBuf::_writeVec2d(const CRCore::crVector2d& v)
{
	_writeDouble(v.x());
	_writeDouble(v.y());
}

void crStreamBuf::_writeVec3d(const CRCore::crVector3d& v)
{
	_writeDouble(v.x());
	_writeDouble(v.y());
	_writeDouble(v.z());
}

void crStreamBuf::_writeVec4d(const CRCore::crVector4d& v)
{
	_writeDouble(v.x());
	_writeDouble(v.y());
	_writeDouble(v.z());
	_writeDouble(v.w());
}

void crStreamBuf::_writeVec2i(const CRCore::crVector2i& v)
{
	_writeInt(v.x());
	_writeInt(v.y());
}

void crStreamBuf::_writeVec3i(const CRCore::crVector3i& v)
{
	_writeInt(v.x());
	_writeInt(v.y());
	_writeInt(v.z());
}

void crStreamBuf::_writeVec4i(const CRCore::crVector4i& v)
{
	_writeInt(v.x());
	_writeInt(v.y());
	_writeInt(v.z());
	_writeInt(v.w());
}

void crStreamBuf::_writeVec2s(const CRCore::crVector2s& v)
{
	_writeShort(v.x());
	_writeShort(v.y());
}

void crStreamBuf::_writeVec3s(const CRCore::crVector3s& v)
{
	_writeShort(v.x());
	_writeShort(v.y());
	_writeShort(v.z());
}

void crStreamBuf::_writeVec4s(const CRCore::crVector4s& v)
{
	_writeShort(v.x());
	_writeShort(v.y());
	_writeShort(v.z());
	_writeShort(v.w());
}

void crStreamBuf::_writeVec2b(const CRCore::crVector2b& v)
{
	_writeChar(v.x());
	_writeChar(v.y());
}

void crStreamBuf::_writeVec3b(const CRCore::crVector3b& v)
{
	_writeChar(v.x());
	_writeChar(v.y());
	_writeChar(v.z());
}

void crStreamBuf::_writeVec4b(const CRCore::crVector4b& v)
{
	_writeChar(v.x());
	_writeChar(v.y());
	_writeChar(v.z());
	_writeChar(v.w());
}

void crStreamBuf::_writeVec2ub(const CRCore::crVector2ub& v)
{
	_writeUChar(v.x());
	_writeUChar(v.y());
}

void crStreamBuf::_writeVec3ub(const CRCore::crVector3ub& v)
{
	_writeUChar(v.x());
	_writeUChar(v.y());
	_writeUChar(v.z());
}

void crStreamBuf::_writeVec4ub(const CRCore::crVector4ub& v)
{
	_writeUChar(v.x());
	_writeUChar(v.y());
	_writeUChar(v.z());
	_writeUChar(v.w());
}

void crStreamBuf::_writePlane(const CRCore::crPlane& v)
{
	_writeFloat(v[0]);
	_writeFloat(v[1]);
	_writeFloat(v[2]);
	_writeFloat(v[3]);
}

void crStreamBuf::_writeUByte4(const CRCore::crVector4ub& v)
{
	_writeChar(v.r());
	_writeChar(v.g());
	_writeChar(v.b());
	_writeChar(v.a());
}

void crStreamBuf::_writeQuat(const CRCore::crQuat& q)
{
	_writeFloat(q.x());
	_writeFloat(q.y());
	_writeFloat(q.z());
	_writeFloat(q.w());
}

void crStreamBuf::_writeArray(const CRCore::Array* a)
{
	switch(a->getType())
	{
		case CRCore::Array::IntArrayType: 
			_writeChar((char)0);
			_writeIntArray(static_cast<const CRCore::IntArray*>(a));
			break;
		case CRCore::Array::UByteArrayType:
			_writeChar((char)1);
			_writeUByteArray(static_cast<const CRCore::UByteArray*>(a));
			break;
		case CRCore::Array::UShortArrayType:
			_writeChar((char)2);
			_writeUShortArray(static_cast<const CRCore::UShortArray*>(a));
			break;
		case CRCore::Array::UIntArrayType:
			_writeChar((char)3);
			_writeUIntArray(static_cast<const CRCore::UIntArray*>(a));
			break;
		case CRCore::Array::Vec4ubArrayType:
			_writeChar((char)4);
			_writeUByte4Array(static_cast<const CRCore::Vec4ubArray*>(a));
			break;
		case CRCore::Array::FloatArrayType:
			_writeChar((char)5);
			_writeFloatArray(static_cast<const CRCore::FloatArray*>(a));
			break;
		case CRCore::Array::Vec2ArrayType: 
			_writeChar((char)6);
			_writeVec2Array(static_cast<const CRCore::Vec2Array*>(a));
			break;
		case CRCore::Array::Vec3ArrayType: 
			_writeChar((char)7);
			_writeVec3Array(static_cast<const CRCore::Vec3Array*>(a));
			break;
		case CRCore::Array::Vec4ArrayType: 
			_writeChar((char)8);
			_writeVec4Array(static_cast<const CRCore::Vec4Array*>(a));
			break;
		default: 
			CRCore::notify(CRCore::FATAL)<<"Unknown array type in crStreamBuf::_writeArray()"<<std::endl;
	}
}

void crStreamBuf::_writeIntArray(const CRCore::IntArray* a)
{
	int size = a->getNumElements();
	_writeInt(size);
	for(int i =0; i<size ;i++)
	{
		_writeInt(a->index(i));
	}
}

void crStreamBuf::_writeUByteArray(const CRCore::UByteArray* a)
{
	int size = a->getNumElements(); 
	_writeInt(size);
	for(int i =0; i<size ;i++)
	{
		_writeChar((*a)[i]);
	}
}

void crStreamBuf::_writeUShortArray(const CRCore::UShortArray* a)
{
	int size = a->getNumElements(); 
	_writeInt(size);
	for(int i =0; i<size ;i++)
	{
		_writeUShort((*a)[i]);
	}
}

void crStreamBuf::_writeUIntArray(const CRCore::UIntArray* a)
{
	int size = a->getNumElements(); 
	_writeInt(size);
	for(int i =0; i<size ;i++)
	{
		_writeInt((*a)[i]);
	}
}

void crStreamBuf::_writeUByte4Array(const CRCore::Vec4ubArray* a)
{
	int size = a->getNumElements(); 
	_writeInt(size);
	for(int i =0; i<size ;i++)
	{
		_writeUByte4((*a)[i]);
	}
}

void crStreamBuf::_writeFloatArray(const CRCore::FloatArray* a)
{
	int size = a->getNumElements(); 
	_writeInt(size);
	for(int i =0; i<size ;i++)
	{
		_writeFloat((*a)[i]);
	}
}

void crStreamBuf::_writeVec2Array(const CRCore::Vec2Array* a)
{
	int size = a->size();
	_writeInt(size);
	for(int i=0;i<size;i++)
	{
		_writeVec2((*a)[i]);
	}
}

void crStreamBuf::_writeVec3Array(const CRCore::Vec3Array* a)
{
	int size = a->size();
	_writeInt(size);
	for(int i = 0; i < size; i++)
	{
		_writeVec3((*a)[i]);
	}
}

void crStreamBuf::_writeVec4Array(const CRCore::Vec4Array* a)
{
	int size = a->size();
	_writeInt(size);
	for(int i=0;i<size;i++)
	{
		_writeVec4((*a)[i]);
	}
}

void crStreamBuf::_writeMatrixf(const CRCore::crMatrixf& mat)
{
	for(int r=0;r<4;r++)
	{
		for(int c=0;c<4;c++)
		{
			_writeFloat(mat(r,c));
		}
	}
}

void crStreamBuf::_writeMatrixd(const CRCore::crMatrixd& mat)
{
	for(int r=0;r<4;r++)
	{
		for(int c=0;c<4;c++)
		{
			_writeDouble(mat(r,c));
		}
	}
}

bool crStreamBuf::_readBool()
{
	char c = 0;
	read(&c, CHARSIZE);
	return c!=0;
}

char crStreamBuf::_readChar()
{
	char c = 0;
	read(&c, CHARSIZE);
	return c;
}

unsigned char crStreamBuf::_readUChar()
{
	unsigned char c = 0;
	read((char*)&c, CHARSIZE);
	return c;
}

unsigned short crStreamBuf::_readUShort()
{
	unsigned short s = 0;
	read((char*)&s, SHORTSIZE);
	return s;
}

short crStreamBuf::_readShort()
{
	short s = 0;
	read((char*)&s, SHORTSIZE);
	return s;
}

unsigned int crStreamBuf::_readUInt()
{
	unsigned int s = 0;
	read((char*)&s, INTSIZE);
	return s;
}

int crStreamBuf::_readInt()
{
	int i = 0;
	read((char*)&i, INTSIZE);
	return i;
}

_crInt64 crStreamBuf::_readInt64()
{
	_crInt64 i = 0;
	read((char*)&i, INT64SIZE);

	return i;
}

/**
* Read an integer from the stream, but
* save it such that the next _readInt call will
* return the same integer.
*/
bool crStreamBuf::peekBool()
{
	char c = 0;
	peek(&c, CHARSIZE);
	return c!=0;
}
char crStreamBuf::peekChar()
{
	char c = 0;
	peek(&c, CHARSIZE);
	return c;
}
int crStreamBuf::peekInt()
{
	int i = 0;
	peek((char*)&i, INTSIZE);
	//CRCore::notify(CRCore::FATAL)<<"crStreamBuf::peekInt = "<<i<<std::endl;
	return i;
}

float crStreamBuf::_readFloat()
{
	float f = 0.0f;
	read((char*)&f, FLOATSIZE);
	return f;
}

long crStreamBuf::_readLong()
{
	long l = 0;
	read((char*)&l, LONGSIZE);
	return l;
}

unsigned long crStreamBuf::_readULong()
{
	unsigned long l = 0;
	read((char*)&l, LONGSIZE);
	return l;
}

double crStreamBuf::_readDouble()
{
	double d = 0;
	read((char*)&d, DOUBLESIZE);
	return d;
}

std::string crStreamBuf::_readString()
{
	std::string s;
	int size = _readInt();
	s.resize(size);
	read((char*)s.c_str(), size);
	return s;
}

void crStreamBuf::_readCharArray(char* data, int size)
{
	read(data, size);
}

CRCore::crVector2f crStreamBuf::_readVec2()
{
	CRCore::crVector2f v;
	v.x()=_readFloat();
	v.y()=_readFloat();
	return v;
}

CRCore::crVector3f crStreamBuf::_readVec3()
{
	CRCore::crVector3f v;
	v.x()=_readFloat();
	v.y()=_readFloat();
	v.z()=_readFloat();
	return v;
}

CRCore::crVector4f crStreamBuf::_readVec4()
{
	CRCore::crVector4f v;
	v.x()=_readFloat();
	v.y()=_readFloat();
	v.z()=_readFloat();
	v.w()=_readFloat();
	return v;
}
CRCore::crVector2d crStreamBuf::_readVec2d()
{
	CRCore::crVector2d v;
	v.x()=_readDouble();
	v.y()=_readDouble();
	return v;
}

CRCore::crVector3d crStreamBuf::_readVec3d()
{
	CRCore::crVector3d v;
	v.x()=_readDouble();
	v.y()=_readDouble();
	v.z()=_readDouble();
	return v;
}

CRCore::crVector4d crStreamBuf::_readVec4d()
{
	CRCore::crVector4d v;
	v.x()=_readDouble();
	v.y()=_readDouble();
	v.z()=_readDouble();
	v.w()=_readDouble();
	return v;
}

CRCore::crVector2i crStreamBuf::_readVec2i()
{
	CRCore::crVector2i v;
	v.x()=_readInt();
	v.y()=_readInt();
	return v;
}

CRCore::crVector3i crStreamBuf::_readVec3i()
{
	CRCore::crVector3i v;
	v.x()=_readInt();
	v.y()=_readInt();
	v.z()=_readInt();
	return v;
}

CRCore::crVector4i crStreamBuf::_readVec4i()
{
	CRCore::crVector4i v;
	v.x()=_readInt();
	v.y()=_readInt();
	v.z()=_readInt();
	v.w()=_readInt();
	return v;
}

CRCore::crVector2s crStreamBuf::_readVec2s()
{
	CRCore::crVector2s v;
	v.x()=_readShort();
	v.y()=_readShort();
	return v;
}

CRCore::crVector3s crStreamBuf::_readVec3s()
{
	CRCore::crVector3s v;
	v.x()=_readShort();
	v.y()=_readShort();
	v.z()=_readShort();
	return v;
}

CRCore::crVector4s crStreamBuf::_readVec4s()
{
	CRCore::crVector4s v;
	v.x()=_readShort();
	v.y()=_readShort();
	v.z()=_readShort();
	v.w()=_readShort();
	return v;
}

CRCore::crVector2b crStreamBuf::_readVec2b()
{
	CRCore::crVector2b v;
	v.x()=_readChar();
	v.y()=_readChar();
	return v;
}

CRCore::crVector3b crStreamBuf::_readVec3b()
{
	CRCore::crVector3b v;
	v.x()=_readChar();
	v.y()=_readChar();
	v.z()=_readChar();
	return v;
}

CRCore::crVector4b crStreamBuf::_readVec4b()
{
	CRCore::crVector4b v;
	v.x()=_readChar();
	v.y()=_readChar();
	v.z()=_readChar();
	v.w()=_readChar();
	return v;
}

CRCore::crVector2ub crStreamBuf::_readVec2ub()
{
	CRCore::crVector2ub v;
	v.x()=_readUChar();
	v.y()=_readUChar();
	return v;
}

CRCore::crVector3ub crStreamBuf::_readVec3ub()
{
	CRCore::crVector3ub v;
	v.x()=_readUChar();
	v.y()=_readUChar();
	v.z()=_readUChar();
	return v;
}

CRCore::crVector4ub crStreamBuf::_readVec4ub()
{
	CRCore::crVector4ub v;
	v.x()=_readUChar();
	v.y()=_readUChar();
	v.z()=_readUChar();
	v.w()=_readUChar();
	return v;
}

CRCore::crPlane crStreamBuf::_readPlane()
{
	CRCore::crPlane v;
	v[0]=_readFloat();
	v[1]=_readFloat();
	v[2]=_readFloat();
	v[3]=_readFloat();
	return v;
}

CRCore::crVector4ub crStreamBuf::_readUByte4()
{
	CRCore::crVector4ub v;
	v.r()=_readChar();
	v.g()=_readChar();
	v.b()=_readChar();
	v.a()=_readChar();
	return v;
}

CRCore::crQuat crStreamBuf::_readQuat()
{
	CRCore::crQuat q;
	q.x()=_readFloat();
	q.y()=_readFloat();
	q.z()=_readFloat();
	q.w()=_readFloat();
	return q;
}

CRCore::Array* crStreamBuf::_readArray()
{
	char c = _readChar();

	switch((int)c){
		case 0: return _readIntArray();
		case 1: return _readUByteArray();
		case 2: return _readUShortArray();
		case 3: return _readUIntArray();
		case 4: return _readUByte4Array();
		case 5: return _readFloatArray();
		case 6:    return _readVec2Array();
		case 7:    return _readVec3Array();
		case 8:    return _readVec4Array();
		default: 
			CRCore::notify(CRCore::FATAL)<<"Unknown array type in crStreamBuf::_readArray()"<<std::endl;
			return 0;
	}
}

CRCore::IntArray* crStreamBuf::_readIntArray()
{
	int size = _readInt();
	CRCore::IntArray* a = new CRCore::IntArray(size);

	read((char*)&((*a)[0]), INTSIZE*size);

	return a;
}

CRCore::UByteArray* crStreamBuf::_readUByteArray()
{
	int size = _readInt();
	CRCore::UByteArray* a = new CRCore::UByteArray(size);

	read((char*)&((*a)[0]), CHARSIZE*size);

	return a;
}

CRCore::UShortArray* crStreamBuf::_readUShortArray()
{
	int size = _readInt();
	CRCore::UShortArray* a = new CRCore::UShortArray(size);

	read((char*)&((*a)[0]), SHORTSIZE*size);

	return a;
}

CRCore::UIntArray* crStreamBuf::_readUIntArray()
{
	int size = _readInt();
	CRCore::UIntArray* a = new CRCore::UIntArray(size);

	read((char*)&((*a)[0]), INTSIZE*size);

	return a;
}

CRCore::Vec4ubArray* crStreamBuf::_readUByte4Array()
{
	int size = _readInt();
	CRCore::Vec4ubArray* a = new CRCore::Vec4ubArray(size);

	read((char*)&((*a)[0]), INTSIZE*size);

	return a;
}

CRCore::FloatArray* crStreamBuf::_readFloatArray()
{
	int size = _readInt();

	CRCore::FloatArray* a = new CRCore::FloatArray(size);

	read((char*)&((*a)[0]), FLOATSIZE*size);

	return a;
}

CRCore::Vec2Array* crStreamBuf::_readVec2Array()
{
	int size = _readInt();
	CRCore::Vec2Array* a = new CRCore::Vec2Array(size);

	read((char*)&((*a)[0]), FLOATSIZE*2*size);

	return a;
}

CRCore::Vec3Array* crStreamBuf::_readVec3Array()
{
	int size = _readInt();
	CRCore::Vec3Array* a = new CRCore::Vec3Array(size);

	read((char*)&((*a)[0]), FLOATSIZE*3*size);

	return a;
}

CRCore::Vec4Array* crStreamBuf::_readVec4Array()
{
	int size = _readInt();
	CRCore::Vec4Array* a = new CRCore::Vec4Array(size);

	read((char*)&((*a)[0]), FLOATSIZE*4*size);

	return a;
}

CRCore::crMatrixf crStreamBuf::_readMatrixf()
{
	CRCore::crMatrixf mat;
	for(int r=0;r<4;r++)
	{
		for(int c=0;c<4;c++)
		{
			mat(r,c) = _readFloat();
		}
	}

	return mat;
}

CRCore::crMatrixd crStreamBuf::_readMatrixd()
{
	CRCore::crMatrixd mat;
	for(int r=0;r<4;r++)
	{
		for(int c=0;c<4;c++)
		{
			mat(r,c) = _readDouble();
		}
	}

	return mat;
}

//bool crStreamBuf::saveToFile(const std::string &filename,bool app)
//{
//	int size = getBufSize();
//	if(size>0)
//	{
//		
//		std::ofstream fout(filename.c_str(), std::ios::binary|(app ? std::ios::app : std::ios::out));
//		if(!fout) return false;
//
//		fout.write((char*)&m_endian, INTSIZE);
//		fout.write((char*)&size, INTSIZE);
//		fout.write(m_buf,size);
//		fout.close();
//
//		return true;
//	}
//
//	return false;
//}
//
//bool crStreamBuf::loadFromFile(const std::string &filename,LoadStreamVec& loadStreamVec)
//{
//	std::ifstream fin(filename.c_str(), std::ios::in | std::ios::binary);
//	if(!fin) return false;
//
//	//fin.seekg(0, std::ios::end); 
//	//int length  =  fin.tellg(); 
//	//fin.seekg(0, std::ios::beg);
//
//	crStreamBuf *stream;
//	int endian,size;
//	while (fin.peek() != EOF)
//	{
//		fin.read((char*)&endian, INTSIZE);
//		stream = new crStreamBuf(endian);
//		fin.read((char*)&size, INTSIZE);
//		stream->createBuf(size);
//		fin.read(stream->getBuf(),size);
//		stream->m_bufSize = size;
//		loadStreamVec.push_back(stream);
//	}
//	//createBuf(length);
//	//if(m_buf)
//	//{
//	//	fin->read((char*)&m_endian, INTSIZE);
//
//	//    fin.read(m_buf,length);
//	//	m_bufSize = length;
//	//}
//	//else
//	//{
//	//	fin.clear();
//	//	return false;
//	//}
//	fin.clear();
//	return true;
//}

bool crStreamBuf::saveToFile2(const std::string &filename,bool app)
{
	int size = getRemainSize();
	if(size>0)
	{

		std::ofstream fout(filename.c_str(), std::ios::binary|(app ? std::ios::app : std::ios::out));
		if(!fout) return false;

		fout.write(m_ptr,size);
		fout.close();

		return true;
	}

	return false;
}

bool crStreamBuf::loadFromFile2(const std::string &filename)
{
	std::ifstream fin(filename.c_str(), std::ios::in | std::ios::binary);
	if(!fin)
	{
		//CRCore::notify(CRCore::NOTICE)<<"crStreamBuf::loadFromFile2 文件没有找到: "<<filename<<std::endl;
		return false;
	}

	fin.seekg(0, std::ios::end); 
	int count  =  fin.tellg();
	if(count>0)
	{
		fin.seekg(0, std::ios::beg);
		createBuf(count);
		fin.read(getBuf(),count);
		setBufSize(count);
	}
	else
	{
		CRCore::notify(CRCore::NOTICE)<<"crStreamBuf::loadFromFile2 文件打开出错: "<<filename<<std::endl;
	}
	return true;
}
void crStreamBuf::endian()
{
	for(char *p = m_buf; p < m_buf+m_bufSize; p++)
	{
		ENDIAN(*p);
	}
}
int calcByte(int crc, char b)//这个函数是用来进行位运算的，主要为下面的计算CRC来服务
{
	int i;
	crc = crc ^ (int)b << 8;

	for ( i = 0; i < 8; i++)
	{
		if ((crc & 0x8000) == 0x8000)
			crc = crc << 1 ^ 0x1021;
		else
			crc = crc << 1;
	}

	return crc & 0xffff;
}
//count crc-16, length in byte
//这个函数是计算16位CRC码的函数，只要给出要校验的数组，以及数组长度，就可以返回16位CRC校验码
int CRC16(char *pBuffer, int length)
{
	int wCRC16=0;
	int i;
	if (( pBuffer==0 )||( length==0 ))
	{
		return 0;
	}
	for ( i = 0; i < length; i++)
	{
		wCRC16 = calcByte(wCRC16, pBuffer[i]);
	}
	return wCRC16;
}
//下面这个函数是计算32位CRC码的函数，传入参数是数组，以及校验的数据数量，返回32位的CRC校验码
long CRC32(char*  pBuffer, long length)
{
	long dwCRC32=0;
	if ((NULL == pBuffer)||(0 == length))
	{
		return 0;
	}
	for (int i = 0; i < length; i++)
	{
		dwCRC32 -= *pBuffer++;
	}
	return dwCRC32;
}
static const unsigned char aucCRCHi[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40
};
static const unsigned char aucCRCLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
	0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
	0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
	0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
	0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
	0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
	0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
	0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
	0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
	0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
	0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
	0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
	0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
	0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
	0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
	0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
	0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
	0x41, 0x81, 0x80, 0x40
};
unsigned short usMBCRC16( char *pucFrame, unsigned short usLen )
{
	if ((NULL == pucFrame)||(usLen==0))
	{
		return 0;
	}
	unsigned char ucCRCHi = 0xFF;
	unsigned char ucCRCLo = 0xFF;
	int iIndex;
	while( usLen-- )
	{
		iIndex = ucCRCLo ^ (unsigned char)(*( pucFrame++ ));
		ucCRCLo = ( unsigned char )( ucCRCHi ^ aucCRCHi[iIndex] );
		ucCRCHi = aucCRCLo[iIndex];
	}
	return ( ucCRCHi << 8 | ucCRCLo );
}
void crStreamBuf::cook()
{
	static unsigned short mask = 0;
	unsigned short code = usMBCRC16(m_buf,m_bufSize);
	if(getRemainCapacity()<CookSize)
		appendBuf(CookSize);
	seekEnd();
	_writeUShort(code);
	//crRange<unsigned short> rnd(0,RAND_MAX);
	_writeUShort(mask++);
	for(char *p = m_ptr-CookSize; p < m_ptr; p++)
	{
		ENDIAN(*p);
	}
}
bool crStreamBuf::uncook(int &cookcode)
{
	if(m_bufSize>=CookSize)
	{
		char *ptr = m_ptr;
		seekEnd();
		for(char *p = m_ptr-CookSize; p < m_ptr; p++)
		{
			ENDIAN(*p);
		}
		seek(-CookSize);
		unsigned short code = _readUShort();
		unsigned short _code = usMBCRC16(m_buf,m_bufSize-CookSize);
		if(code == _code)
		{
			unsigned short rndcode = _readUShort();
			cookcode = MAKEINT32(code,rndcode);
			m_ptr = ptr;
			m_bufSize -= CookSize;
			return true;
		}
		else
		{
			CRCore::notify(CRCore::ALWAYS)<<"code:"<<code<<" _code:"<<_code<<std::endl;
		}
	}
	return false;
}
//void crStreamBuf::writeNode(CRCore::crEventGroup &eg)
//{
//
//}
//
//void crStreamBuf::writeNode(CRPhysics::crMatterGroup &mg)
//{
//
//}
//
//void crStreamBuf::readNode(CRCore::crEventGroup &eg)
//{
//
//}
//
//void crStreamBuf::readNode(CRPhysics::crMatterGroup &mg)
//{
//
//}
ref_ptr<crDebugInfo> crDebugInfo::m_instance = NULL;
crDebugInfo *crDebugInfo::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crDebugInfo;
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}
void crDebugInfo::init(const std::string &info)
{
	m_filename = info;
	DeleteFileA(m_filename.c_str());
	m_stream = new crStreamBuf;
	m_stream->createBuf(10260);
}
//void crDebugInfo::debugInfo(const NotifySeverity severity)
//{
//	notify(severity)<<m_buf;
//	if(m_stream.valid())
//	{
//		m_stream->write(m_buf,strlen(m_buf));
//		if(m_stream->getBufSize()>10000)
//			writeToFile();
//	}
//}
void crDebugInfo::debugInfo(const NotifySeverity severity,char *str)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	notify(severity)<<str;
	if(m_stream.valid())
	{
		m_stream->write(str,strlen(str));
		if(m_stream->getBufSize()>10000)
			writeToFile();
	}
}
void crDebugInfo::writeToFile()
{
	if(m_stream.valid() && m_stream->getBufSize()>0)
	{
		m_stream->seekBegin();
		m_stream->saveToFile2(m_filename,true);
	}
}
crDebugInfo::~crDebugInfo()
{
	writeToFile();
}

typedef unsigned int uint32;
#ifdef sgi
#define HIGHFIRST
#endif

#ifdef sun
#define HIGHFIRST
#endif

#ifndef HIGHFIRST
#define byteReverse(buf, len)	/* Nothing */
#else
/*
 * Note: this code is harmless on little-endian machines.
 */
void byteReverse(buf, longs)
    unsigned char *buf; unsigned longs;
{
    uint32 t;
    do {
	t = (uint32) ((unsigned) buf[3] << 8 | buf[2]) << 16 |
	    ((unsigned) buf[1] << 8 | buf[0]);
	*(uint32 *) buf = t;
	buf += 4;
    } while (--longs);
}
#endif

struct MD5Context {
	uint32 buf[4];
	uint32 bits[2];
	unsigned char in[64];
};

/*
 * Start MD5 accumulation.  Set bit count to 0 and buffer to mysterious
 * initialization constants.
 */
void MD5Init(struct MD5Context *ctx)
{
	ctx->buf[0] = 0x67452301;
	ctx->buf[1] = 0xefcdab89;
	ctx->buf[2] = 0x98badcfe;
	ctx->buf[3] = 0x10325476;

	ctx->bits[0] = 0;
	ctx->bits[1] = 0;
}
/* The four core functions - F1 is optimized somewhat */

/* #define F1(x, y, z) (x & y | ~x & z) */
#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

/* This is the central step in the MD5 algorithm. */
#define MD5STEP(f, w, x, y, z, data, s) ( w += f(x, y, z) + data,  w = w<<s | w>>(32-s),  w += x )

/*
 * The core of the MD5 algorithm, this alters an existing MD5 hash to
 * reflect the addition of 16 longwords of new data.  MD5Update blocks
 * the data and converts bytes into longwords for this routine.
 */
void MD5Transform(unsigned int buf[4], unsigned int in[16])
{
	register uint32 a, b, c, d;

	a = buf[0];
	b = buf[1];
	c = buf[2];
	d = buf[3];

	MD5STEP(F1, a, b, c, d, in[0] + 0xd76aa478, 7);
	MD5STEP(F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
	MD5STEP(F1, c, d, a, b, in[2] + 0x242070db, 17);
	MD5STEP(F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
	MD5STEP(F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
	MD5STEP(F1, d, a, b, c, in[5] + 0x4787c62a, 12);
	MD5STEP(F1, c, d, a, b, in[6] + 0xa8304613, 17);
	MD5STEP(F1, b, c, d, a, in[7] + 0xfd469501, 22);
	MD5STEP(F1, a, b, c, d, in[8] + 0x698098d8, 7);
	MD5STEP(F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
	MD5STEP(F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
	MD5STEP(F1, b, c, d, a, in[11] + 0x895cd7be, 22);
	MD5STEP(F1, a, b, c, d, in[12] + 0x6b901122, 7);
	MD5STEP(F1, d, a, b, c, in[13] + 0xfd987193, 12);
	MD5STEP(F1, c, d, a, b, in[14] + 0xa679438e, 17);
	MD5STEP(F1, b, c, d, a, in[15] + 0x49b40821, 22);

	MD5STEP(F2, a, b, c, d, in[1] + 0xf61e2562, 5);
	MD5STEP(F2, d, a, b, c, in[6] + 0xc040b340, 9);
	MD5STEP(F2, c, d, a, b, in[11] + 0x265e5a51, 14);
	MD5STEP(F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
	MD5STEP(F2, a, b, c, d, in[5] + 0xd62f105d, 5);
	MD5STEP(F2, d, a, b, c, in[10] + 0x02441453, 9);
	MD5STEP(F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
	MD5STEP(F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
	MD5STEP(F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
	MD5STEP(F2, d, a, b, c, in[14] + 0xc33707d6, 9);
	MD5STEP(F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
	MD5STEP(F2, b, c, d, a, in[8] + 0x455a14ed, 20);
	MD5STEP(F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
	MD5STEP(F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
	MD5STEP(F2, c, d, a, b, in[7] + 0x676f02d9, 14);
	MD5STEP(F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

	MD5STEP(F3, a, b, c, d, in[5] + 0xfffa3942, 4);
	MD5STEP(F3, d, a, b, c, in[8] + 0x8771f681, 11);
	MD5STEP(F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
	MD5STEP(F3, b, c, d, a, in[14] + 0xfde5380c, 23);
	MD5STEP(F3, a, b, c, d, in[1] + 0xa4beea44, 4);
	MD5STEP(F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
	MD5STEP(F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
	MD5STEP(F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
	MD5STEP(F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
	MD5STEP(F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
	MD5STEP(F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
	MD5STEP(F3, b, c, d, a, in[6] + 0x04881d05, 23);
	MD5STEP(F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
	MD5STEP(F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
	MD5STEP(F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
	MD5STEP(F3, b, c, d, a, in[2] + 0xc4ac5665, 23);

	MD5STEP(F4, a, b, c, d, in[0] + 0xf4292244, 6);
	MD5STEP(F4, d, a, b, c, in[7] + 0x432aff97, 10);
	MD5STEP(F4, c, d, a, b, in[14] + 0xab9423a7, 15);
	MD5STEP(F4, b, c, d, a, in[5] + 0xfc93a039, 21);
	MD5STEP(F4, a, b, c, d, in[12] + 0x655b59c3, 6);
	MD5STEP(F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
	MD5STEP(F4, c, d, a, b, in[10] + 0xffeff47d, 15);
	MD5STEP(F4, b, c, d, a, in[1] + 0x85845dd1, 21);
	MD5STEP(F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
	MD5STEP(F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
	MD5STEP(F4, c, d, a, b, in[6] + 0xa3014314, 15);
	MD5STEP(F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
	MD5STEP(F4, a, b, c, d, in[4] + 0xf7537e82, 6);
	MD5STEP(F4, d, a, b, c, in[11] + 0xbd3af235, 10);
	MD5STEP(F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
	MD5STEP(F4, b, c, d, a, in[9] + 0xeb86d391, 21);

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}
/*
 * Update context to reflect the concatenation of another buffer full
 * of bytes.
 */
void MD5Update(struct MD5Context *ctx, char const *buf, unsigned len)
{
	uint32 t;

	/* Update bitcount */

	t = ctx->bits[0];
	if ((ctx->bits[0] = t + ((uint32) len << 3)) < t)
		ctx->bits[1]++; 	/* Carry from low to high */
	ctx->bits[1] += len >> 29;

	t = (t >> 3) & 0x3f;	/* Bytes already in shsInfo->data */

	/* Handle any leading odd-sized chunks */

	if (t != 0) {
		unsigned char *p = (unsigned char *) ctx->in + t;

		t = 64 - t;
		if ((uint32)len < t) {
			memcpy(p, buf, (size_t)len);
			return;
		}
		memcpy(p, buf, (size_t)t);
		byteReverse(ctx->in, 16);
		MD5Transform(ctx->buf, (uint32 *) ctx->in);
		buf += t;
		len -= t;
	}
	/* Process data in 64-byte chunks */

	while (len >= 64) {
		memcpy(ctx->in, buf, 64);
		byteReverse(ctx->in, 16);
		MD5Transform(ctx->buf, (uint32 *) ctx->in);
		buf += 64;
		len -= 64;
	}

	/* Handle any remaining bytes of data. */

	memcpy(ctx->in, buf, (size_t)len);
}

/*
 * Final wrapup - pad to 64-byte boundary with the bit pattern 
 * 1 0* (64-bit count of bits processed, MSB-first)
 */
void MD5Final(unsigned char digest[16], struct MD5Context *ctx)
{
	unsigned int count;
    unsigned char *p;

    /* Compute number of bytes mod 64 */
    count = (unsigned int)(ctx->bits[0] >> 3) & 0x3F;

    /* Set the first char of padding to 0x80.  This is safe since there is
       always at least one byte free */
    p = ctx->in + count;
    *p++ = (unsigned char)0x80;

    /* Bytes of padding needed to make 64 bytes */
    count = 64 - 1 - count;

    /* Pad out to 56 mod 64 */
    if (count < 8) {
	/* Two lots of padding:  Pad the first block to 64 bytes */
	memset(p, 0, (size_t)count);
	byteReverse(ctx->in, 16);
	MD5Transform(ctx->buf, (uint32 *) ctx->in);

	/* Now fill the next block with 56 bytes */
	memset(ctx->in, 0, 56);
    } else {
	/* Pad block to 56 bytes */
	memset(p, 0, count - 8);
    }
    byteReverse(ctx->in, 14);

    /* Append length in bits and transform */
    ((uint32 *) ctx->in)[14] = ctx->bits[0];
    ((uint32 *) ctx->in)[15] = ctx->bits[1];

    MD5Transform(ctx->buf, (uint32 *) ctx->in);
    byteReverse((unsigned char *) ctx->buf, 4);
    memcpy(digest, ctx->buf, 16);
    memset(ctx, 0, sizeof(ctx));        /* In case it's sensitive */
}

std::string crStreamBuf::getMD5()
{
	std::string str;
	if(m_buf && m_bufSize>0)
	{
		struct MD5Context   md5c;
		unsigned char       sig[16];

		MD5Init(&md5c);
		MD5Update(&md5c, m_buf, m_bufSize);
		MD5Final(sig, &md5c);

		char buf[3];
		for( int i = 0; i<16; i++)
		{
			sprintf(buf,"%02X\0",sig[i]);
			str+=buf;
		}
	}
	return str;
}