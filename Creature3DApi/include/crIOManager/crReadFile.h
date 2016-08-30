/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
#ifndef CRIOMANAGER_READFILE
#define CRIOMANAGER_READFILE 1

#include <string>

#include <CRCore/crNode.h>
#include <CRCore/crImage.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crDisplaySettings.h>
#include <CRCore/crBrain.h>

#include <CRIOManager/crExport.h>
#include <CRIOManager/crRegistry.h>
#include <CRIOManager/crFileNameUtils.h>
#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crCookFile.h>
namespace CRIOManager {


/** Read an CRCore::crBase from file. 
  * Return valid CRCore::crBase on success,
  * return NULL on failure.
  * Use the Options object to control cache operations and file search paths in CRIOManager::crRegistry.
  * The CRIOManager::crRegistry is used to load the appropriate crReaderWriter plugin
  * for the filename extension, and this plugin then handles the request
  * to read the specified file.*/
extern CRIOMANAGER_EXPORT CRCore::crBase* readObjectFile(const std::string& filename,const crReaderWriter::Options* options);

/** Read an CRCore::crBase from file. 
  * Return valid CRCore::crBase on success,
  * return NULL on failure.
  * The CRIOManager::crRegistry is used to load the appropriate crReaderWriter plugin
  * for the filename extension, and this plugin then handles the request
  * to read the specified file.*/
inline CRCore::crBase* readObjectFile(const std::string& filename)
{
    return readObjectFile(filename,crRegistry::instance()->getOptions());
}


/** Read an CRCore::crImage from file. 
  * Return valid CRCore::crImage on success,
  * return NULL on failure.
  * Use the Options object to control cache operations and file search paths in CRIOManager::crRegistry.
  * The CRIOManager::crRegistry is used to load the appropriate crReaderWriter plugin
  * for the filename extension, and this plugin then handles the request
  * to read the specified file.*/
extern CRIOMANAGER_EXPORT CRCore::crImage*  readImageFile(const std::string& filename,const crReaderWriter::Options* options);

/** Read an CRCore::crImage from file. 
  * Return valid CRCore::crImage on success,
  * return NULL on failure.
  * The CRIOManager::crRegistry is used to load the appropriate crReaderWriter plugin
  * for the filename extension, and this plugin then handles the request
  * to read the specified file.*/
inline CRCore::crImage*  readImageFile(const std::string& filename)
{
	//int textureDetail = crDisplaySettings::instance()->getTextureDetail();
	std::string new_fileName;//(filename);
	//std::string::size_type dot;
	if(filename == "credef.img")
	{
		new_fileName = filename;
	}
	else
	{
		if(CRCore::crDisplaySettings::instance()->getRunMode() == CRCore::crDisplaySettings::RobotNoTexture)
			return CRCore::gCredefimage().get();

		const FilePathList& filepath = crRegistry::instance()->getDataFilePathList();
		std::string dataFile;

		switch(CRCore::crDisplaySettings::instance()->getTextureDetail())
		{
		case 0://Low "_L"
			/*dot = new_fileName.find_last_of('.');
			new_fileName.insert(dot,"_l",2);*/
			new_fileName = getFilePath(filename) + "L_" + getSimpleFileName(filename);
			dataFile = findFileInPath(new_fileName, filepath);
			//CRCore::notify(CRCore::FATAL)<<"CRCore::crImage*  readImageFile(): new_fileName: "<<new_fileName<<std::endl;
			//CRCore::notify(CRCore::FATAL)<<"CRCore::crImage*  readImageFile(): dataFile: "<<dataFile<<std::endl;
			if(dataFile.empty()) new_fileName = filename;
			else new_fileName = dataFile;
    		break;
		case 1://Mid 默认
			new_fileName = filename;
			break;
		case 2://Height "_H"
			//dot = new_fileName.find_last_of('.');
			//new_fileName.insert(dot,"_h",2);
			new_fileName = getFilePath(filename) + "H_" + getSimpleFileName(filename);
			dataFile = findFileInPath(new_fileName, filepath);
			//CRCore::notify(CRCore::FATAL)<<"CRCore::crImage*  readImageFile(): new_fileName: "<<new_fileName<<std::endl;
			if(dataFile.empty()) new_fileName = filename;
			else new_fileName = dataFile;
			break;
		}
	}
	//CRCore::notify(CRCore::FATAL)<<"CRCore::crImage*  readImageFile(): new_fileName: "<<new_fileName<<std::endl;
	//if(getFileExtension(new_fileName)!="img")
	//{//编辑的时候可能用DDS格式，在cook的时候，dds格式需要被转换成img格式，模型文件需要在编辑器里打开重新保存一次
	// //如果文件目录下存在img格式，则使用img格式
	//	std::string imgFile = getNameLessExtension(new_fileName) + "img";
	//	dataFile = findFileInPath(imgFile, filepath);
	//	if(!dataFile.empty())
	//	    new_fileName = dataFile;
	//}
	CRCore::crImage *image = readImageFile(new_fileName,crRegistry::instance()->getOptions());
	if(image)
	{
		image->setFileName(getSimpleFileName(new_fileName));
		if(CRIOManager::GetPublishRun())
		{
			std::string srcfile = CRIOManager::findDataFile( new_fileName, crRegistry::instance()->getOptions() );
			std::string destfile = "media/textures/Publish/"+image->getFileName();
			copyFile(srcfile,destfile);
		}
	}
	else
	{
		image = CRCore::gCredefimage().get();
	}
    return image; 
}

/** Read an CRCore::crHeightField from file. 
  * Return valid CRCore::crHeightField on success,
  * return NULL on failure.
  * Use the Options object to control cache operations and file search paths in CRIOManager::crRegistry.
  * The CRIOManager::crRegistry is used to load the appropriate crReaderWriter plugin
  * for the filename extension, and this plugin then handles the request
  * to read the specified file.*/
extern CRIOMANAGER_EXPORT CRCore::crHeightField*  readHeightFieldFile(const std::string& filename,const crReaderWriter::Options* options);

/** Read an CRCore::crHeightField from file. 
  * Return valid CRCore::crHeightField on success,
  * return NULL on failure.
  * The CRIOManager::crRegistry is used to load the appropriate crReaderWriter plugin
  * for the filename extension, and this plugin then handles the request
  * to read the specified file.*/
inline CRCore::crHeightField*  readHeightFieldFile(const std::string& filename)
{
    return readHeightFieldFile(filename,crRegistry::instance()->getOptions());
}

/** Read an CRCore::crNode from file. 
  * Return valid CRCore::crNode on success,
  * return NULL on failure.
  * Use the Options object to control cache operations and file search paths in CRIOManager::crRegistry.
  * The CRIOManager::crRegistry is used to load the appropriate crReaderWriter plugin
  * for the filename extension, and this plugin then handles the request
  * to read the specified file.*/
extern CRIOMANAGER_EXPORT CRCore::crNode*  readNodeFile(const std::string& filename,const crReaderWriter::Options* options);

/** Read an CRCore::crNode from file. 
  * Return valid CRCore::crNode on success,
  * return NULL on failure.
  * The CRIOManager::crRegistry is used to load the appropriate crReaderWriter plugin
  * for the filename extension, and this plugin then handles the request
  * to read the specified file.*/
inline CRCore::crNode*  readNodeFile(const std::string& filename)
{
    return readNodeFile(filename,crRegistry::instance()->getOptions());
}


/** Read an CRCore::crNode subgraph from files, creating a CRCore::crGroup to contain the nodes if more
  * than one subgraph has been loaded.
  * Use the Options object to control cache operations and file search paths in CRIOManager::crRegistry.*/
extern CRIOMANAGER_EXPORT CRCore::crNode* readNodeFiles(std::vector<std::string>& commandLine,const crReaderWriter::Options* options);

/** Read an CRCore::crNode subgraph from files, creating a CRCore::crGroup to contain the nodes if more
  * than one subgraph has been loaded.*/
inline CRCore::crNode* readNodeFiles(std::vector<std::string>& commandLine)
{
    return readNodeFiles(commandLine,crRegistry::instance()->getOptions());
}


/** Read an CRCore::crNode subgraph from files, creating a CRCore::crGroup to contain the nodes if more
  * than one subgraph has been loaded.
  * Use the Options object to control cache operations and file search paths in CRIOManager::crRegistry.*/
extern CRIOMANAGER_EXPORT CRCore::crNode* readNodeFiles(CRCore::crArgumentParser& parser,const crReaderWriter::Options* options);

/** Read an CRCore::crNode subgraph from files, creating a CRCore::crGroup to contain the nodes if more
  * than one subgraph has been loaded.*/
inline CRCore::crNode* readNodeFiles(CRCore::crArgumentParser& parser)
{
    return readNodeFiles(parser,crRegistry::instance()->getOptions());
}

}

#endif
