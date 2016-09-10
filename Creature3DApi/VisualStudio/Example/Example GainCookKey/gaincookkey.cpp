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
#include <creature3d.h>
#include <CRCore/crBlowFish.h>
#include <CRCore/crEndian.h>
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <vector>
#include <string>
using namespace CRCore;
int main( int argc, char **argv )
{
	char code[MAX_PASSWD+1];
	CRCore::crRange<char> rangec;
	rangec.set(48,122);
	srand(time(0));
	for (int i=0;i<MAX_PASSWD;i++)
	{
		code[i] = rangec.get_random();
	}
	code[MAX_PASSWD-1] = 48;
	code[MAX_PASSWD] = '\0';

	for (int i=0;i<MAX_PASSWD;i++)
	{
		CRCore::ENDIAN(code[i]);
	}
	std::ofstream fout;
	if(strcmp(argv[1],"SHD") == 0)
	{
		fout.open("zhangyanyan", std::ios::binary);
	    fout.write("#define WuCaihua \"",18);
	}
	else if(strcmp(argv[1],"COOK") == 0)
	{
		fout.open("wucaihua", std::ios::binary);
        fout.write("#define ZhangYanyan \"",21);
	}
	fout.write(code,MAX_PASSWD);
	fout.write("\"",1);
	fout.close();
    
	return 0;
}

