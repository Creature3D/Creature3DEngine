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
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <vector>
#include <string>
#include<commdlg.h>
using namespace CRCore;

//#define IMGTODDS
//#define IMGTOPNG

void convertImage(const std::string &file, const std::string &ext)
{
	std::string fileName = file;
	CRCore::ref_ptr<CRCore::crImage> image = CRIOManager::readImageFile(fileName);
	if (!image.valid()) 
	{
		//std::cout << arguments.getApplicationName() <<": No data loaded" << std::endl;
		return;
	}

//#ifdef IMGTODDS
//	fileName = CRCore::crArgumentParser::getFileNameEliminateExt(fileName) + ".dds";
//#elif defined IMGTOPNG
//	fileName = CRCore::crArgumentParser::getFileNameEliminateExt(fileName) + ".png";
//#else
//	fileName = CRCore::crArgumentParser::getFileNameEliminateExt(fileName) + ".img";
//#endif
	fileName = CRCore::crArgumentParser::getFileNameEliminateExt(fileName) + ext;
	image->setFileName(CRIOManager::getSimpleFileName(fileName));
	CRIOManager::writeImageFile(*image,fileName);
}

int main( int argc, char **argv )
{
	std::string ext = ".img";
	if(argc>1 && argv[1])
	{
		ext = ".";
		ext += argv[1];
	}
	
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[102400];       // buffer for file name

	// Initialize OPENFILENAME 
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ZeroMemory(szFile, sizeof(szFile));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "bmp(.bmp)\0*.bmp\0jpg(.jpg)\0*.jpg\0jpeg(.jpeg)\0*.jpeg\0tga(.tga)\0*.tga\0rgb(.rgb)\0*.rgb\0sgi(.sgi)\0*.sgi\
					  \0rgba(.rgba)\0*.rgba\0int(.int)\0*.int\0inta(.inta)\0*.inta\0bw(.bw)\0*.bw\0tiff(.tiff)\0*.tiff\0tif(.tif)\0*.tif\
					  \0png(.png)\0*.png\0pnm(.pnm)\0*.pnm\0ppm(.ppm)\0*.ppm\0pgm(.pgm)\0*.pgm\0pbm(.pbm)\0*.pbm\0gif(.gif)\0*.gif\
					  \0dds(.dds)\0*.dds\0hdr(.hdr)\0*.hdr\0img(.img)\0*.img\0All(*.*)\0*.*\0";
	ofn.nFilterIndex = ext == ".img"?19:21;
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

		filePath = CRIOManager::getFilePath(filePath);
		std::string pushAndPopPath;
		CRIOManager::FilePathList fpl = CRIOManager::getDataFilePathList();
		//If absolute path
		if( (filePath.length()>0 && filePath.find_first_of("/\\")==0) ||
			(filePath.length()>2 && filePath.substr(1,1)==":" && filePath.find_first_of("/\\")==2) )
		{
			pushAndPopPath = filePath;
		}
		else
		{
			pushAndPopPath = fpl.empty() ? "." : fpl.front();
			if(pushAndPopPath.empty()) pushAndPopPath = ".";
			pushAndPopPath += "/" + filePath;
		}

		fpl.push_back(pushAndPopPath);

		for( std::vector<std::string>::iterator itr = fileNameVec.begin();
			itr != fileNameVec.end();
			++itr )
		{
            convertImage(filePath + "/" + *itr,ext);
		}
	}
	system("pause");
	return 0;
}

