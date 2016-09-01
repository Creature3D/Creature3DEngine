/* Creature3D - Online Game Engine, Copyright (C) 2005 Îâ²Æ»ª(26756325@qq.com)
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
#ifndef CRCORE_ENDIAN
#define CRCORE_ENDIAN 1

namespace CRCore {

enum Endian
{
    BigEndian,
    LittleEndian
};

inline extern CR_EXPORT Endian getCpuByteOrder()
{
    static char big_endian_1[2] = { 0, 1 };

    if ( (*((short*) big_endian_1)) == 1)
        return BigEndian;
    else 
        return LittleEndian;
} 

inline extern CR_EXPORT void swapBytes( char* in, unsigned int size )
{
    char* start = in;
    char* end = start+size-1;
    while (start<end)
    {
        std::swap(*start++,*end--);
    }
    
}

inline extern CR_EXPORT void swapBytes2( char* in )
{
    char* start = in;
    char* end = start+1;
    std::swap(*start,*end);
}

inline extern CR_EXPORT void swapBytes4( char* in )
{
    char* start = in;
    char* end = start+3;
    std::swap(*start++,*end--);
    std::swap(*start,*end);
}

inline extern CR_EXPORT void swapBytes8( char* in )
{
    char* start = in;
    char* end = start+7;
    std::swap(*start++,*end--);
    std::swap(*start++,*end--);
    std::swap(*start++,*end--);
    std::swap(*start,*end);
}

inline extern CR_EXPORT void swapBytes16( char* in )
{
    char* start = in;
    char* end = start+15;
    std::swap(*start++,*end--);
    std::swap(*start++,*end--);
    std::swap(*start++,*end--);
    std::swap(*start++,*end--);
    std::swap(*start++,*end--);
    std::swap(*start++,*end--);
    std::swap(*start++,*end--);
    std::swap(*start,*end);
}

#define ENDIAN2(SRC, DST) endian2((void*)&(SRC), sizeof(SRC), (void*)&(DST), sizeof(DST))
#define ENDIAN(A) ENDIAN2((A), (A))

inline extern CR_EXPORT void endian2(void* pSrc, int nSrc, void* pDst, int )
{
	if (nSrc == 1)
	{
		char tmp1;
		tmp1 = *(char *)pSrc;
		tmp1 = (tmp1 << 4) | ((tmp1 >> 4) & 0xf);
		*(char *)pDst = tmp1;
	}
	else if (nSrc == 2)
	{
		short tmp1;
		tmp1 = *(short *)pSrc;
		tmp1 = (tmp1 << 8) | ((tmp1 >> 8) & 0xff);
		*(short *)pDst = tmp1;
	}
	else if (nSrc == 4)
	{
		long tmp1;
		tmp1 = *(long *)pSrc;
		tmp1 = (tmp1 << 24) | ((tmp1 << 8) & 0xff0000) | ((tmp1 >> 8) & 0xff00) | ((tmp1 >> 24) & 0xff);
		*(long *)pDst = tmp1;
	}
	else if (nSrc == 8)
	{
		long tmp1, tmp2;
		tmp1 = *(long *)pSrc;
		tmp2 = *(1 + (long *)pSrc);
		tmp1 = (tmp1 << 24) | ((tmp1 << 8) & 0xff0000) | ((tmp1 >> 8) & 0xff00) | ((tmp1 >> 24) & 0xff);
		tmp2 = (tmp2 << 24) | ((tmp2 << 8) & 0xff0000) | ((tmp2 >> 8) & 0xff00) | ((tmp2 >> 24) & 0xff);
		*(long *)pDst = tmp2;
		*(1 + (long *)pDst) = tmp1;
	}
}

}

#endif
