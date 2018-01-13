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
#include <CRCore/crGuid.h>
#include <CRCore/crStreamBuf.h>
#include <CRCore/crTimer.h>
using namespace CRCore;
crGuid::crGuid(_crInt32 inA, _crInt32 inB, _crInt32 inC, _crInt32 inD):
A(inA),
B(inB),
C(inC),
D(inD)
{
}

crGuid::~crGuid()
{
}

crGuid crGuid::newGuid()
{
	static _crInt16 IncrementCounter = 0;

	_crInt32 Year = 0, Month = 0, DayOfWeek = 0, Day = 0, Hour = 0, Min = 0, Sec = 0, MSec = 0; // Use real time for baseline uniqueness
	_crInt32 SequentialBits = static_cast<_crInt32>(IncrementCounter++); // Add sequential bits to ensure sequentially generated guids are unique even if Cycles is wrong
	_crInt32 RandBits = rand() & 0xFFFF; // Add randomness to improve uniqueness across machines

	time_t nowTime_t = time(0);
	struct  tm  nowTm = *localtime(&nowTime_t);
	struct tm  startTm;
	memset(&startTm, 0, sizeof(startTm));
	Timer::SystemTime(Year, Month, DayOfWeek, Day, Hour, Min, Sec, MSec);
	return crGuid(RandBits | (SequentialBits << 16), Day | (Hour << 8) | (Month << 16) | (Sec << 24), MSec | (Min << 16), Year ^ Timer::instance()->runtime());
}
std::string crGuid::toString(GuidFormats format) const
{
	char buf[256];
	switch (format)
	{
	case DigitsWithHyphens:
		sprintf_s(buf, "%08X-%04X-%04X-%04X-%04X%08X", A, B >> 16, B & 0xFFFF, C >> 16, C & 0xFFFF, D);
		break;
	case DigitsWithHyphensInBraces:
		sprintf_s(buf, "{%08X-%04X-%04X-%04X-%04X%08X}", A, B >> 16, B & 0xFFFF, C >> 16, C & 0xFFFF, D);
		break;
	case DigitsWithHyphensInParentheses:
		sprintf_s(buf, "(%08X-%04X-%04X-%04X-%04X%08X)", A, B >> 16, B & 0xFFFF, C >> 16, C & 0xFFFF, D);
		break;
	case HexValuesInBraces:
		sprintf_s(buf, "{0x%08X,0x%04X,0x%04X,{0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X}}", A, B >> 16, B & 0xFFFF, C >> 24, (C >> 16) & 0xFF, (C >> 8) & 0xFF, C & 0XFF, D >> 24, (D >> 16) & 0XFF, (D >> 8) & 0XFF, D & 0XFF);
		break;
	case UniqueObjectGuid:
		sprintf_s(buf, "%08X-%08X-%08X-%08X", A, B, C, D);
		break;
	default:
		sprintf_s(buf, "%08X%08X%08X%08X", A, B, C, D);
		break;
	}
	return buf;
}
unsigned short crGuid::GetTypeHash()
{
	ref_ptr<crStreamBuf> buf = new crStreamBuf;
	buf->createBuf(16);
	buf->_writeGuid(*this);
	return buf->getMBCRC16(buf->getBuf(),buf->getBufSize());
}
