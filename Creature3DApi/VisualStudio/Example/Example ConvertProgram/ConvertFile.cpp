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
#include <creature3d.h>
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <vector>
#include <string>
#include <commdlg.h>
using namespace CRCore;

void convert(const std::string &fileName)
{
	CRCore::ref_ptr<CRCore::crNode> cookObj = new CRCore::crNode;
	CRIOManager::writeObjectFile(*(cookObj.get()), fileName + ".shd");
}

int main( int argc, char **argv )
{
	_putenv( "CRE_LIBRARY_PATH=./lib/" );

	OPENFILENAME ofn;       // common dialog box structure
	char szFile[102400];       // buffer for file name

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ZeroMemory(szFile, sizeof(szFile));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "GLSL(.glsl)\0*.glsl\0All(*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;

	if (GetOpenFileName(&ofn)==TRUE) 
	{
		std::string filePath;
		std::string fileName;
		std::vector<std::string> fileNameVec;

		filePath = ofn.lpstrFile;
		int offset = ofn.nFileOffset;
		while(strlen(ofn.lpstrFile + offset) > 1)
		{
			fileName = ofn.lpstrFile + offset;
			fileNameVec.push_back(fileName);
			offset += fileName.length() + 1;
		}

		if(fileNameVec.size()>1)
		{
			for( std::vector<std::string>::iterator itr = fileNameVec.begin();
				itr != fileNameVec.end();
				++itr )
			{
				fileName = filePath + "/" + *itr;
                convert(fileName);
			}
		}
		else
		{
			fileName = filePath;
			convert(fileName);
		}
	}
    
	return 0;
}

