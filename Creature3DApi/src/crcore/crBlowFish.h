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

#ifndef CRCORE_BLOWFISH_H
#define CRCORE_BLOWFISH_H

#include <CRCore/crExport.h>
#define NUM_SUBKEYS  18
#define NUM_S_BOXES  4
#define NUM_ENTRIES  256

#define MAX_STRING   256
#define MAX_PASSWD   56  // 448bits
#define LOW_PASSWD   16  // 128bits
namespace CRCore {
// #define BIG_ENDIAN
// #define LITTLE_ENDIAN

//BIG_ENDIAN
//struct WordByte
//{
//	unsigned int zero:8;
//	unsigned int one:8;
//	unsigned int two:8;
//	unsigned int three:8;
//};

//LITTLE_ENDIAN
struct WordByte
{
	unsigned int three:8;
	unsigned int two:8;
	unsigned int one:8;
	unsigned int zero:8;
};

union Word
{
	unsigned int word;
	WordByte byte;
};

struct DWord
{
	Word word0;
	Word word1;
};

class CR_EXPORT crBlowfish
{
public:
    crBlowfish();
    ~crBlowfish();

    void reset();
    void setPasswd(char * passwd= NULL);
    void encrypt(void *,unsigned int);
    bool decrypt(void *,unsigned int);
private:
	unsigned int PA[NUM_SUBKEYS];
	unsigned int SB[NUM_S_BOXES][NUM_ENTRIES];

	void genSubkeys(char *);
	void BF_En(Word *,Word *);
	void BF_De(Word *,Word *);
};
}
#endif