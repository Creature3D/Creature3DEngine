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
#include <Plugin cre/crDataOutputStream.h>
#include <Plugin cre/crException.h>

#include <Plugin cre/crStateSet.h>
#include <Plugin cre/crAlphaFunc.h>
#include <Plugin cre/crBlendFunc.h>
#include <Plugin cre/crDepth.h>
#include <Plugin cre/crMaterial.h>
#include <Plugin cre/crCullFace.h>
#include <Plugin cre/crClipPlane.h>
#include <Plugin cre/crPolygonOffset.h>
#include <Plugin cre/crPolygonMode.h>
#include <Plugin cre/crShadeModel.h>
#include <Plugin cre/crPoint.h>
#include <Plugin cre/crLineWidth.h>
#include <Plugin cre/crTexture1D.h>
#include <Plugin cre/crTexture2D.h>
#include <Plugin cre/crTexture3D.h>
#include <Plugin cre/crTextureCubeMap.h>
#include <Plugin cre/crTexEnv.h>
#include <Plugin cre/crTexEnvCombine.h>
#include <Plugin cre/crTexGen.h>
#include <Plugin cre/crTexMat.h>
//#include <Plugin cre/crFragmentProgram.h>
//#include <Plugin cre/crVertexProgram.h>
#include <Plugin cre/crLightModel.h>
#include <Plugin cre/crFog.h>
#include <Plugin cre/crLight.h>
#include <Plugin cre/crProxyNode.h>
#include <Plugin cre/crFrontFace.h>
#include <Plugin cre/crProgram.h>
#include <Plugin cre/crUniform.h>
#include <Plugin cre/crShader.h>
#include <Plugin cre/crDB.h>
#include <Plugin cre/crGroup.h>
#include <Plugin cre/crMatrixTransform.h>
#include <Plugin cre/crObject.h>
#include <Plugin cre/crLightSource.h>
#include <Plugin cre/crTexGenNode.h>
#include <Plugin cre/crClipNode.h>
#include <Plugin cre/crBillboard.h>
//#include <Plugin cre/crDepthSortedObject.h>
#include <Plugin cre/crSequence.h>
#include <Plugin cre/crLod.h>
#include <Plugin cre/crPagedLOD.h>
#include <Plugin cre/crPositionAttitudeTransform.h>
#include <Plugin cre/crDOFTransform.h>
#include <Plugin cre/crTransform.h>
#include <Plugin cre/crSwitch.h>
#include <Plugin cre/crOccluderNode.h>
//#include <Plugin cre/crImpostor.h>
#include <Plugin cre/crCoordinateSystemNode.h>
//#include <Plugin cre/crNerveCellGroup.h>
//#include <Plugin cre/crAIGroup.h>
#include <Plugin cre/crCameraView.h>
#include <Plugin cre/crCameraNode.h>

//#include <Plugin cre/crLightPointNode.h>
#include <Plugin cre/crMultiSwitch.h>
//#include <Plugin cre/crVisibilityGroup.h>

//#include <Plugin cre/crParseParamCallback.h>

//#include <Plugin cre/crMultiTextureControl.h>

#include <Plugin cre/crGeometry.h>
#include <Plugin cre/crShapeDrawable.h>

#include <Plugin cre/crShape.h>

#include <Plugin cre/crText.h>

#include <Plugin cre/crEventMsg.h>
#include <Plugin cre/crMatterObject.h>
//#include <Plugin cre/crWeaponMatterObject.h>
//#include <Plugin cre/crWeaponNode.h>
#include <Plugin cre/crMatterGroup.h>
#include <Plugin cre/crEventGroup.h>
#include <Plugin cre/crJoint.h>
//#include <Plugin cre/crSoundState.h>
//#include <Plugin cre/crKeyboardMouseEventGroup.h>
#include <Plugin cre/crSoundOccludeCallback.h>
#include <Plugin cre/crCollideCallback.h>
//#include <Plugin cre/crSoundCollideCallback.h>
//#include <Plugin cre/crSoundNode.h>
#include <Plugin cre/crTerrainTile.h>
#include <CRCore/crNotify.h>
//#include <CRCore/io_utils.h>

//#include <CRAL/crSoundRoot.h>
#include <CRCore/crRenderToTextureManager.h>
#include <CRPhysics/crVolumeNode.h>
#include <CRPhysics/crCamera.h>
#include <CRCore/crFilterRenderManager.h>
#include <CRCore/crEffectSequenceManager.h>
#include <Plugin cre/crTraverseStringNode.h>
#include <Plugin cre/crWidgetNode.h>
#include <Plugin cre/crParticleEffect.h>
#include <CRIOManager/crFileNameUtils.h>
#include <CRIOManager/crCookFile.h>
#include <CRIOManager/crFileUtils.h>
#include <CRCore/crTextureCubeMap.h>
#include <sys/stat.h>
#include <windows.h>
using namespace cre;


void crDataOutputStream::setOptions(const CRIOManager::crReaderWriter::Options* options) 
{ 
    m_options = options; 

    if (m_options.get())
    {
        setIncludeImageData(false/*m_options->getOptionString().find("noTexturesInCREFile")==std::string::npos*/);
        //CRCore::notify(CRCore::DEBUG_INFO) << "cre::DataOutpouStream.setIncludeImageData()=" << getIncludeImageData() << std::endl;

        setIncludeExternalReferences(m_options->getOptionString().find("inlineExternalReferencesInCREFile")!=std::string::npos);
        //CRCore::notify(CRCore::DEBUG_INFO) << "cre::DataOutpouStream.setIncludeExternalReferences()=" << getIncludeExternalReferences() << std::endl;

        setWriteExternalReferenceFiles(m_options->getOptionString().find("noWriteExternalReferenceFiles")==std::string::npos);
        //CRCore::notify(CRCore::DEBUG_INFO) << "cre::DataOutpouStream.setWriteExternalReferenceFiles()=" << getWriteExternalReferenceFiles() << std::endl;

        setUseOriginalExternalReferences(m_options->getOptionString().find("useOriginalExternalReferences")!=std::string::npos);
        //CRCore::notify(CRCore::DEBUG_INFO) << "cre::DataOutpouStream.setUseOriginalExternalReferences()=" << getUseOriginalExternalReferences() << std::endl;
    }
	char *ptr;
	if((ptr = getenv("CRE_WRITEIMAGE_PATH")) != 0)
	    m_writeImagePath = ptr;
}

bool crDataOutputStream::acceptsExtension(const std::string& extension)
{
	return  CRIOManager::equalCaseInsensitive(extension,"cre") ||
			CRIOManager::equalCaseInsensitive(extension,"crs") ||
			CRIOManager::equalCaseInsensitive(extension,"cru");
}

crDataOutputStream::crDataOutputStream(std::ostream * ostream, const std::string &filename)
{
    m_verboseOutput = false;

    m_includeImageData= false;//true;

    m_includeExternalReferences     = false;
    m_writeExternalReferenceFiles   = false;
    m_useOriginalExternalReferences = true;
    
    m_ostream = ostream;
    if(!m_ostream)
        throw crException("crDataOutputStream::crDataOutputStream(): null pointer exception in argument.");

    writeUInt(ENDIAN_TYPE) ;
    writeUInt(getVersion());

	m_writeFileName = filename;

	//delete by wuch 20140125
	if(!m_writeFileName.empty() && m_writeFileName.compare("tmp.cre") != 0)
	{
		std::string ext = CRIOManager::getLowerCaseFileExtension(m_writeFileName);
		if (acceptsExtension(ext)) 
		{
			m_filesTab = new CREncapsulation::crTableIO;
			CREncapsulation::crTableIO::StrVec title;
			title.push_back("file");
			//title.push_back("mtime");
			m_filesTab->setTitleVec(title);
		}
		//std::string matFilePath = CRCore::crArgumentParser::getFileNameEliminateExt(m_writeFileName) + ".mat";
		//CRIOManager::removeDirectory(matFilePath);
	}
}

crDataOutputStream::~crDataOutputStream()
{
	if(!m_writeFileName.empty() && m_writeFileName.compare("tmp.cre") != 0)
	{
		std::string matFilePath = CRCore::crArgumentParser::getFileNameEliminateExt(m_writeFileName) + ".mat";
		CRIOManager::removeDirectory(matFilePath);
		//文件夹改名
		std::string path = CRIOManager::getFilePath(m_writeFileName);
		std::string name = CRCore::crArgumentParser::getFileNameEliminateExt(CRIOManager::getSimpleFileName(m_writeFileName))+".mat";
		std::string xname = name+"$";

		char programDir[256];
		GetCurrentDirectory(256,programDir);
		std::string dir = programDir;
		dir = dir + "/" + path;
		SetCurrentDirectory(dir.c_str());
		char buf[256];
		sprintf(buf,"ren %s %s\0",xname.c_str(),name.c_str());
		system(buf);
		//WinExec(buf,SW_HIDE);
		SetCurrentDirectory(programDir);
	}
}
//bool crDataOutputStream::addFiles(const std::string &filename)
//{
//	//wuch 20130626，只需要输出img
//	if(m_filesTab.valid())
//	{					
//		std::string cookfilename = filename;
//		CRIOManager::GetCookedFileName(cookfilename);
//		struct stat buf;
//		if(!stat( cookfilename.c_str(), &buf )) 
//		{
//			CREncapsulation::crTableIO::StrVec record;
//			cookfilename = CRIOManager::convertFileNameToUnixStyle(cookfilename);
//			record.push_back(cookfilename);
//			//record.push_back(CRCore::crArgumentParser::appI64toa(buf.st_mtime));
//			m_filesTab->addData(record);
//			return true;
//		}
//	}
//	return false;
//}
//void crDataOutputStream::addImageFiles(const std::string &imageid)
//{//需要用全路径
//	if(m_filesTab.valid())
//	{					
//		std::string imagefile = CRIOManager::findDataFile( imageid,CRIOManager::crRegistry::instance()->getOptions() );
//		if(!imagefile.empty())
//		{
//			struct stat buf;
//			if(!stat( imagefile.c_str(), &buf )) 
//			{
//				CREncapsulation::crTableIO::StrVec record;
//				imagefile = CRIOManager::convertFileNameToUnixStyle(imagefile);
//				record.push_back(imagefile);
//				//record.push_back(CRCore::crArgumentParser::appI64toa(buf.st_mtime));
//				m_filesTab->addData(record);
//			}
//		}
//	}
//}
//void crDataOutputStream::addCubeImageFiles(const std::string &cubeImageNameID)
//{//需要用全路径
//	if(m_filesTab.valid())
//	{
//		std::string ext = CRCore::crArgumentParser::getFileExtension(cubeImageNameID);
//		std::string fileName = CRCore::crArgumentParser::getFileNameEliminateExt(cubeImageNameID);
//
//		std::string file = CUBEMAP_FILENAME(fileName,posx,ext);
//		std::string imagefile = CRIOManager::findDataFile( file,CRIOManager::crRegistry::instance()->getOptions() );
//		if(!imagefile.empty())
//		{
//			struct stat buf;
//			if(!stat( imagefile.c_str(), &buf )) 
//			{
//				CREncapsulation::crTableIO::StrVec record;
//				imagefile = CRIOManager::convertFileNameToUnixStyle(imagefile);
//				record.push_back(imagefile);
//				//record.push_back(CRCore::crArgumentParser::appI64toa(buf.st_mtime));
//				m_filesTab->addData(record);
//			}
//		}
//		file = CUBEMAP_FILENAME(fileName,negx,ext);
//		imagefile = CRIOManager::findDataFile( file,CRIOManager::crRegistry::instance()->getOptions() );
//		if(!imagefile.empty())
//		{
//			struct stat buf;
//			if(!stat( imagefile.c_str(), &buf )) 
//			{
//				CREncapsulation::crTableIO::StrVec record;
//				imagefile = CRIOManager::convertFileNameToUnixStyle(imagefile);
//				record.push_back(imagefile);
//				//record.push_back(CRCore::crArgumentParser::appI64toa(buf.st_mtime));
//				m_filesTab->addData(record);
//			}
//		}
//		file = CUBEMAP_FILENAME(fileName,posy,ext);
//		imagefile = CRIOManager::findDataFile( file,CRIOManager::crRegistry::instance()->getOptions() );
//		if(!imagefile.empty())
//		{
//			struct stat buf;
//			if(!stat( imagefile.c_str(), &buf )) 
//			{
//				CREncapsulation::crTableIO::StrVec record;
//				imagefile = CRIOManager::convertFileNameToUnixStyle(imagefile);
//				record.push_back(imagefile);
//				//record.push_back(CRCore::crArgumentParser::appI64toa(buf.st_mtime));
//				m_filesTab->addData(record);
//			}
//		}
//		file = CUBEMAP_FILENAME(fileName,negy,ext);
//		imagefile = CRIOManager::findDataFile( file,CRIOManager::crRegistry::instance()->getOptions() );
//		if(!imagefile.empty())
//		{
//			struct stat buf;
//			if(!stat( imagefile.c_str(), &buf )) 
//			{
//				CREncapsulation::crTableIO::StrVec record;
//				imagefile = CRIOManager::convertFileNameToUnixStyle(imagefile);
//				record.push_back(imagefile);
//				//record.push_back(CRCore::crArgumentParser::appI64toa(buf.st_mtime));
//				m_filesTab->addData(record);
//			}
//		}
//		file = CUBEMAP_FILENAME(fileName,posz,ext);
//		imagefile = CRIOManager::findDataFile( file,CRIOManager::crRegistry::instance()->getOptions() );
//		if(!imagefile.empty())
//		{
//			struct stat buf;
//			if(!stat( imagefile.c_str(), &buf )) 
//			{
//				CREncapsulation::crTableIO::StrVec record;
//				imagefile = CRIOManager::convertFileNameToUnixStyle(imagefile);
//				record.push_back(imagefile);
//				//record.push_back(CRCore::crArgumentParser::appI64toa(buf.st_mtime));
//				m_filesTab->addData(record);
//			}
//		}
//		file = CUBEMAP_FILENAME(fileName,negz,ext);
//		imagefile = CRIOManager::findDataFile( file,CRIOManager::crRegistry::instance()->getOptions() );
//		if(!imagefile.empty())
//		{
//			struct stat buf;
//			if(!stat( imagefile.c_str(), &buf )) 
//			{
//				CREncapsulation::crTableIO::StrVec record;
//				imagefile = CRIOManager::convertFileNameToUnixStyle(imagefile);
//				record.push_back(imagefile);
//				//record.push_back(CRCore::crArgumentParser::appI64toa(buf.st_mtime));
//				m_filesTab->addData(record);
//			}
//		}
//	}
//}
void crDataOutputStream::addTabFiles(const std::string &filename)
{
	if(m_filesTab.valid())
	{					
		CREncapsulation::crTableIO::StrVec record;
		record.push_back(filename);
		m_filesTab->addData(record);
	}
}
void crDataOutputStream::saveFilesTab()
{
	//delete by wuch 20140125
	if(m_filesTab.valid())
	{
		std::string str = CRCore::crArgumentParser::getFileNameEliminateExt(m_writeFileName) + ".crb";
		m_filesTab->saveToFileStream(str);
	}
}
void crDataOutputStream::writeBool(bool b)
{
    char c = b?1:0;
    m_ostream->write(&c, CHARSIZE);
    
    //if (m_verboseOutput) std::cout<<"read/writeBool() ["<<(int)c<<"]"<<std::endl;
}

void crDataOutputStream::writeChar(char c){
    m_ostream->write(&c, CHARSIZE);
    
    //if (m_verboseOutput) std::cout<<"read/writeChar() ["<<(int)c<<"]"<<std::endl;
}

void crDataOutputStream::writeUChar(unsigned char c){
    m_ostream->write((char*)&c, CHARSIZE);
    
    //if (m_verboseOutput) std::cout<<"read/writeUChar() ["<<(int)c<<"]"<<std::endl;
}

void crDataOutputStream::writeUShort(unsigned short s){
    m_ostream->write((char*)&s, SHORTSIZE);
    
    //if (m_verboseOutput) std::cout<<"read/writeUShort() ["<<s<<"]"<<std::endl;
}

void crDataOutputStream::writeUInt(unsigned int s){
    m_ostream->write((char*)&s, INTSIZE);
    
    //if (m_verboseOutput) std::cout<<"read/writeUInt() ["<<s<<"]"<<std::endl;
}

void crDataOutputStream::writeInt(int i){
    m_ostream->write((char*)&i, INTSIZE);
    
    //if (m_verboseOutput) std::cout<<"read/writeInt() ["<<i<<"]"<<std::endl;
}

void crDataOutputStream::writeFloat(float f){
    m_ostream->write((char*)&f, FLOATSIZE);
    
    //if (m_verboseOutput) std::cout<<"read/writeFloat() ["<<f<<"]"<<std::endl;
}

void crDataOutputStream::writeLong(long l){
    m_ostream->write((char*)&l, LONGSIZE);
    
    //if (m_verboseOutput) std::cout<<"read/writeLong() ["<<l<<"]"<<std::endl;
}

void crDataOutputStream::writeULong(unsigned long l){
    m_ostream->write((char*)&l, LONGSIZE);
    
    //if (m_verboseOutput) std::cout<<"read/writeULong() ["<<l<<"]"<<std::endl;
}

void crDataOutputStream::writeInt64(_crInt64 i)
{
	m_ostream->write((char*)&i, INT64SIZE);
	//if (m_verboseOutput) std::cout<<"read/writeInt64() ["<<i<<"]"<<std::endl;
}

void crDataOutputStream::writeDouble(double d){
    m_ostream->write((char*)&d, DOUBLESIZE);
    
    //if (m_verboseOutput) std::cout<<"read/writeDouble() ["<<d<<"]"<<std::endl;
}

void crDataOutputStream::writeString(const std::string& s){
    writeInt(s.size());
    m_ostream->write(s.c_str(), s.size());
    
    //if (m_verboseOutput) std::cout<<"read/writeString() ["<<s<<"]"<<std::endl;
}

void crDataOutputStream::writeCharArray(const char* data, int size){
    m_ostream->write(data, size);
    
    //if (m_verboseOutput) std::cout<<"read/writeCharArray() ["<<data<<"]"<<std::endl;
}

void crDataOutputStream::writeVec2(const CRCore::crVector2f& v){
    writeFloat(v.x());
    writeFloat(v.y());
    
    //if (m_verboseOutput) std::cout<<"read/writeVec2() ["<<v<<"]"<<std::endl;
}

void crDataOutputStream::writeVec3(const CRCore::crVector3f& v){
    writeFloat(v.x());
    writeFloat(v.y());
    writeFloat(v.z());
    
    //if (m_verboseOutput) std::cout<<"read/writeVec3() ["<<v<<"]"<<std::endl;
}

void crDataOutputStream::writeVec4(const CRCore::crVector4f& v){
    writeFloat(v.x());
    writeFloat(v.y());
    writeFloat(v.z());
    writeFloat(v.w());
    
    //if (m_verboseOutput) std::cout<<"read/writeVec4() ["<<v<<"]"<<std::endl;
}

void crDataOutputStream::writeVec2d(const CRCore::crVector2d& v){
    writeDouble(v.x());
    writeDouble(v.y());
    
    //if (m_verboseOutput) std::cout<<"read/writeVec2() ["<<v<<"]"<<std::endl;
}

void crDataOutputStream::writeVec3d(const CRCore::crVector3d& v){
    writeDouble(v.x());
    writeDouble(v.y());
    writeDouble(v.z());
    
    //if (m_verboseOutput) std::cout<<"read/writeVec3d() ["<<v<<"]"<<std::endl;
}

void crDataOutputStream::writeVec4d(const CRCore::crVector4d& v){
    writeDouble(v.x());
    writeDouble(v.y());
    writeDouble(v.z());
    writeDouble(v.w());
    
    //if (m_verboseOutput) std::cout<<"read/writeVec4d() ["<<v<<"]"<<std::endl;
}

void crDataOutputStream::writeVec2i(const CRCore::crVector2i& v){
	writeInt(v.x());
	writeInt(v.y());

	//if (m_verboseOutput) std::cout<<"read/writeVec2i() ["<<v<<"]"<<std::endl;
}

void crDataOutputStream::writeVec3i(const CRCore::crVector3i& v){
	writeInt(v.x());
	writeInt(v.y());
	writeInt(v.z());

	//if (m_verboseOutput) std::cout<<"read/writeVec3i() ["<<v<<"]"<<std::endl;
}

void crDataOutputStream::writeVec4i(const CRCore::crVector4i& v){
	writeInt(v.x());
	writeInt(v.y());
	writeInt(v.z());
	writeInt(v.w());

	//if (m_verboseOutput) std::cout<<"read/writeVec4i() ["<<v<<"]"<<std::endl;
}

void crDataOutputStream::writeVec2b(const CRCore::crVector2b& v){
	writeBool(v.x());
	writeBool(v.y());

	//if (m_verboseOutput) std::cout<<"read/writeVec2b() ["<<v<<"]"<<std::endl;
}

void crDataOutputStream::writeVec3b(const CRCore::crVector3b& v){
	writeBool(v.x());
	writeBool(v.y());
	writeBool(v.z());

	//if (m_verboseOutput) std::cout<<"read/writeVec3b() ["<<v<<"]"<<std::endl;
}

void crDataOutputStream::writeVec4b(const CRCore::crVector4b& v){
	writeBool(v.x());
	writeBool(v.y());
	writeBool(v.z());
	writeBool(v.w());

	//if (m_verboseOutput) std::cout<<"read/writeVec4b() ["<<v<<"]"<<std::endl;
}

void crDataOutputStream::writePlane(const CRCore::crPlane& v)
{
    writeFloat(v[0]);
    writeFloat(v[1]);
    writeFloat(v[2]);
    writeFloat(v[3]);
    
    //if (m_verboseOutput) std::cout<<"read/writePlane() ["<<v<<"]"<<std::endl;
}

void crDataOutputStream::writeUByte4(const CRCore::crVector4ub& v){
    writeChar(v.r());
    writeChar(v.g());
    writeChar(v.b());
    writeChar(v.a());
    
    //if (m_verboseOutput) std::cout<<"read/writeUByte4() ["<<v<<"]"<<std::endl;
}

void crDataOutputStream::writeQuat(const CRCore::crQuat& q){
    writeFloat(q.x());
    writeFloat(q.y());
    writeFloat(q.z());
    writeFloat(q.w());
    
    //if (m_verboseOutput) std::cout<<"read/writeQuat() ["<<q<<"]"<<std::endl;
}

void crDataOutputStream::writeBinding(CRCore::crGeometry::AttributeBinding b){
    switch(b){
        case CRCore::crGeometry::BIND_OFF:                writeChar((char) 0); break;
        case CRCore::crGeometry::BIND_OVERALL:            writeChar((char) 1); break;
        case CRCore::crGeometry::BIND_PER_PRIMITIVE_SET:  writeChar((char) 2); break;
        case CRCore::crGeometry::BIND_PER_PRIMITIVE:     writeChar((char) 3); break;
        case CRCore::crGeometry::BIND_PER_VERTEX:        writeChar((char) 4); break;
        default: throw crException("Unknown binding in crDataOutputStream::writeBinding()");
    }
    
    //if (m_verboseOutput) std::cout<<"read/writeBinding() ["<<b<<"]"<<std::endl;
}

void crDataOutputStream::writeArray(const CRCore::Array* a){
    switch(a->getType()){
        case CRCore::Array::IntArrayType: 
            writeChar((char)0);
            writeIntArray(static_cast<const CRCore::IntArray*>(a));
            break;
        case CRCore::Array::UByteArrayType:
            writeChar((char)1);
            writeUByteArray(static_cast<const CRCore::UByteArray*>(a));
            break;
        case CRCore::Array::UShortArrayType:
            writeChar((char)2);
            writeUShortArray(static_cast<const CRCore::UShortArray*>(a));
            break;
        case CRCore::Array::UIntArrayType:
            writeChar((char)3);
            writeUIntArray(static_cast<const CRCore::UIntArray*>(a));
            break;
        case CRCore::Array::Vec4ubArrayType:
            writeChar((char)4);
            writeUByte4Array(static_cast<const CRCore::Vec4ubArray*>(a));
            break;
        case CRCore::Array::FloatArrayType:
            writeChar((char)5);
            writeFloatArray(static_cast<const CRCore::FloatArray*>(a));
            break;
        case CRCore::Array::Vec2ArrayType: 
            writeChar((char)6);
            writeVec2Array(static_cast<const CRCore::Vec2Array*>(a));
            break;
        case CRCore::Array::Vec3ArrayType: 
            writeChar((char)7);
            writeVec3Array(static_cast<const CRCore::Vec3Array*>(a));
            break;
         case CRCore::Array::Vec4ArrayType: 
            writeChar((char)8);
            writeVec4Array(static_cast<const CRCore::Vec4Array*>(a));
            break;
        default: throw crException("Unknown array type in crDataOutputStream::writeArray()");
    }
}


void crDataOutputStream::writeIntArray(const CRCore::IntArray* a)
{
    int size = a->getNumElements();
    writeInt(size);
    for(int i =0; i<size ;i++){
        writeInt(a->index(i));
    }
    
    //if (m_verboseOutput) std::cout<<"read/writeIntArray() ["<<size<<"]"<<std::endl;
}

void crDataOutputStream::writeUByteArray(const CRCore::UByteArray* a)
{
    int size = a->getNumElements(); 
    writeInt(size);
    for(int i =0; i<size ;i++){
        writeChar((*a)[i]);
    }
    
    //if (m_verboseOutput) std::cout<<"read/writeUByteArray() ["<<size<<"]"<<std::endl;
}

void crDataOutputStream::writeUShortArray(const CRCore::UShortArray* a)
{
    int size = a->getNumElements(); 
    writeInt(size);
    for(int i =0; i<size ;i++){
        writeUShort((*a)[i]);
    }
    
    //if (m_verboseOutput) std::cout<<"read/writeUShortArray() ["<<size<<"]"<<std::endl;
}

void crDataOutputStream::writeUIntArray(const CRCore::UIntArray* a)
{
    int size = a->getNumElements(); 
    writeInt(size);
    for(int i =0; i<size ;i++){
        writeInt((*a)[i]);
    }
    
    //if (m_verboseOutput) std::cout<<"read/writeUIntArray() ["<<size<<"]"<<std::endl;
}

void crDataOutputStream::writeUByte4Array(const CRCore::Vec4ubArray* a)
{
    int size = a->getNumElements(); 
    writeInt(size);
    for(int i =0; i<size ;i++){
        writeUByte4((*a)[i]);
    }
    
    //if (m_verboseOutput) std::cout<<"read/writeUByte4Array() ["<<size<<"]"<<std::endl;
}

void crDataOutputStream::writeFloatArray(const CRCore::FloatArray* a)
{
    int size = a->getNumElements(); 
    writeInt(size);
    for(int i =0; i<size ;i++){
        writeFloat((*a)[i]);
    }
    
    //if (m_verboseOutput) std::cout<<"read/writeFloatArray() ["<<size<<"]"<<std::endl;
}


void crDataOutputStream::writeVec2Array(const CRCore::Vec2Array* a)
{
    int size = a->size();
    writeInt(size);
    for(int i=0;i<size;i++){
        writeVec2((*a)[i]);
    }
    
    //if (m_verboseOutput) std::cout<<"read/writeVec2Array() ["<<size<<"]"<<std::endl;
}

void crDataOutputStream::writeVec3Array(const CRCore::Vec3Array* a)
{
    int size = a->size();
    writeInt(size);
    for(int i = 0; i < size; i++){
        writeVec3((*a)[i]);
    }
    
    //if (m_verboseOutput) std::cout<<"read/writeVec3Array() ["<<size<<"]"<<std::endl;
}

void crDataOutputStream::writeVec4Array(const CRCore::Vec4Array* a)
{
    int size = a->size();
    writeInt(size);
    for(int i=0;i<size;i++){
        writeVec4((*a)[i]);
    }
    
    //if (m_verboseOutput) std::cout<<"read/writeVec4Array() ["<<size<<"]"<<std::endl;
}

void crDataOutputStream::writeMatrixf(const CRCore::crMatrixf& mat)
{
    for(int r=0;r<4;r++)
    {
        for(int c=0;c<4;c++)
        {
            writeFloat(mat(r,c));
        }
    }
    
    //if (m_verboseOutput) std::cout<<"read/writeMatrix() ["<<mat<<"]"<<std::endl;
}

void crDataOutputStream::writeMatrixd(const CRCore::crMatrixd& mat)
{
    for(int r=0;r<4;r++)
    {
        for(int c=0;c<4;c++)
        {
            writeDouble(mat(r,c));
        }
    }
    
    //if (m_verboseOutput) std::cout<<"read/writeMatrix() ["<<mat<<"]"<<std::endl;
}


void crDataOutputStream::writeStateSet(const CRCore::crStateSet* stateset)
{
    StateSetMap::iterator itr = m_stateSetMap.find(stateset);
    if (itr!=m_stateSetMap.end())
    {
        // Id already exists so just write ID.
        writeInt(itr->second);

        //if (m_verboseOutput) std::cout<<"read/writeStateSet() ["<<itr->second<<"]"<<std::endl;
    }
    else
    {
        // id doesn't exist so create a new ID and
        // register the stateset.

        int id = m_stateSetMap.size();
        m_stateSetMap[stateset] = id;

        // write the id.
        writeInt(id);

        // write the stateset.
        ((cre::crStateSet*)(stateset))->write(this);

        //if (m_verboseOutput) std::cout<<"read/writeStateSet() ["<<id<<"]"<<std::endl;

    }
}

void crDataOutputStream::writeStateAttribute(const CRCore::crStateAttribute* attribute)
{
    StateAttributeMap::iterator itr = m_stateAttributeMap.find(attribute);
    if (itr!=m_stateAttributeMap.end())
    {
        // Id already exists so just write ID.
        writeInt(itr->second);
        //if (m_verboseOutput) std::cout<<"read/writeStateAttribute() ["<<itr->second<<"]"<<std::endl;
    }
    else
    {
        // id doesn't exist so create a new ID and
        // register the stateset.

        int id = m_stateAttributeMap.size();
        m_stateAttributeMap[attribute] = id;

        // write the id.
        writeInt(id);

        // write the stateset.
        if(dynamic_cast<const CRCore::crAlphaFunc*>(attribute)){
            ((cre::crAlphaFunc*)(attribute))->write(this);
        }
        else if(dynamic_cast<const CRCore::crBlendFunc*>(attribute)){
            ((cre::crBlendFunc*)(attribute))->write(this);
        }
		else if(dynamic_cast<const CRCore::crDepth*>(attribute)){
			((cre::crDepth*)(attribute))->write(this);
		}
        // This is a crMaterial
        else if(dynamic_cast<const CRCore::crMaterial*>(attribute)){
            ((cre::crMaterial*)(attribute))->write(this);
        }
        // This is a crCullFace
        else if(dynamic_cast<const CRCore::crCullFace*>(attribute)){
            ((cre::crCullFace*)(attribute))->write(this);
        }
        // this is a Cliplane
        else if(dynamic_cast<const CRCore::crClipPlane*>(attribute)){
            ((cre::crClipPlane*)(attribute))->write(this);
        }
        // This is a crPolygonOffset
        else if(dynamic_cast<const CRCore::crPolygonOffset*>(attribute)){
            ((cre::crPolygonOffset*)(attribute))->write(this);
        }
        // This is a PolygonMode
        else if(dynamic_cast<const CRCore::crPolygonMode*>(attribute)){
            ((cre::crPolygonMode*)(attribute))->write(this);
        }
        else if(dynamic_cast<const CRCore::crShadeModel*>(attribute)){
            ((cre::crShadeModel*)(attribute))->write(this);
        }
        else if(dynamic_cast<const CRCore::crPoint*>(attribute)){
            ((cre::crPoint*)(attribute))->write(this);
        }
        else if(dynamic_cast<const CRCore::crLineWidth*>(attribute)){
            ((cre::crLineWidth*)(attribute))->write(this);
        }
        else if(dynamic_cast<const CRCore::crLineWidth*>(attribute)){
            ((cre::crLineWidth*)(attribute))->write(this);
        }
        // This is a crTexture1D
        else if(dynamic_cast<const CRCore::crTexture1D*>(attribute)){
            ((cre::crTexture1D*)(attribute))->write(this);
        }
        // This is a crTexture2D
        else if(dynamic_cast<const CRCore::crTexture2D*>(attribute)){
            ((cre::crTexture2D*)(attribute))->write(this);
        }
        // This is a crTexture2D
        else if(dynamic_cast<const CRCore::crTexture3D*>(attribute)){
            ((cre::crTexture3D*)(attribute))->write(this);
        }
        // This is a crTextureCubeMap
        else if(dynamic_cast<const CRCore::crTextureCubeMap*>(attribute)){
            ((cre::crTextureCubeMap*)(attribute))->write(this);
        }
        // This is a crTexEnv
        else if(dynamic_cast<const CRCore::crTexEnv*>(attribute)){
            ((cre::crTexEnv*)(attribute))->write(this);
        }
        // This is a crTexEnvCombine
        else if(dynamic_cast<const CRCore::crTexEnvCombine*>(attribute)){
            ((cre::crTexEnvCombine*)(attribute))->write(this);
        }
        // This is a crTexGen
        else if(dynamic_cast<const CRCore::crTexGen*>(attribute)){
            ((cre::crTexGen*)(attribute))->write(this);
        }
        // This is a crTexMat
        else if(dynamic_cast<const CRCore::crTexMat*>(attribute)){
            ((cre::crTexMat*)(attribute))->write(this);
        }
        //// This is a crFragmentProgram
        //else if(dynamic_cast<const CRCore::crFragmentProgram*>(attribute)){
        //    ((cre::crFragmentProgram*)(attribute))->write(this);
        //}
        // This is a crVertexProgram
        //else if(dynamic_cast<const CRCore::crVertexProgram*>(attribute)){
        //    ((cre::crVertexProgram*)(attribute))->write(this);
        //}
        // This is a crLightModel
        else if(dynamic_cast<const CRCore::crLightModel*>(attribute)){
            ((cre::crLightModel*)(attribute))->write(this);
        }
		else if(dynamic_cast<const CRCore::crLight*>(attribute)){
			((cre::crLight*)(attribute))->write(this);
		}
        // This is a crFrontFace
        else if(dynamic_cast<const CRCore::crFrontFace*>(attribute)){
            ((cre::crFrontFace*)(attribute))->write(this);
        }
		else if(dynamic_cast<const CRCore::crFog*>(attribute)){
			((cre::crFog*)(attribute))->write(this);
		}
        // This is a crFrontFace
        else if(dynamic_cast<const CRCore::crProgram*>(attribute)){
            ((cre::crProgram*)(attribute))->write(this);
        }

        else{
            std::string className = attribute->className();
            throw crException(std::string("crStateSet::write(): Unknown crStateAttribute: ").append(className));
        }
        //if (m_verboseOutput) std::cout<<"read/writeStateAttribute() ["<<id<<"]"<<std::endl;
    }
}

void crDataOutputStream::writeUniform(const CRCore::crUniform* uniform)
{
    UniformMap::iterator itr = m_uniformMap.find(uniform);
    if (itr!=m_uniformMap.end())
    {
        // Id already exists so just write ID.
        writeInt(itr->second);

        //if (m_verboseOutput) std::cout<<"read/writeUniform() ["<<itr->second<<"]"<<std::endl;
    }
    else
    {
        // id doesn't exist so create a new ID and
        // register the uniform.

        int id = m_uniformMap.size();
        m_uniformMap[uniform] = id;

        // write the id.
        writeInt(id);

        // write the stateset.
        ((cre::crUniform*)(uniform))->write(this);

        //if (m_verboseOutput) std::cout<<"read/writeUniform() ["<<id<<"]"<<std::endl;

    }
}

void crDataOutputStream::writeShader(const CRCore::crShader* shader)
{
    ShaderMap::iterator itr = m_shaderMap.find(shader);
    if (itr!=m_shaderMap.end())
    {
        // Id already exists so just write ID.
        writeInt(itr->second);

        //if (m_verboseOutput) std::cout<<"read/writeShader() ["<<itr->second<<"]"<<std::endl;
    }
    else
    {
        // id doesn't exist so create a new ID and
        // register the shader.

        int id = m_shaderMap.size();
        m_shaderMap[shader] = id;

        // write the id.
        writeInt(id);

        // write the stateset.
        ((cre::crShader*)(shader))->write(this);

        //if (m_verboseOutput) std::cout<<"read/writeShader() ["<<id<<"]"<<std::endl;

    }
}

void crDataOutputStream::writeDrawable(const CRCore::crDrawable* drawable)
{
    DrawableMap::iterator itr = m_drawableMap.find(drawable);
    if (itr!=m_drawableMap.end())
    {
        // Id already exists so just write ID.
        writeInt(itr->second);

        //if (m_verboseOutput) std::cout<<"read/writeDrawable() ["<<itr->second<<"]"<<std::endl;
    }
    else
    {
        // id doesn't exist so create a new ID and
        // register the stateset.

        int id = m_drawableMap.size();
        m_drawableMap[drawable] = id;

        // write the id.
        writeInt(id);
        
        if(dynamic_cast<const CRCore::crGeometry*>(drawable))
            ((cre::crGeometry*)(drawable))->write(this);
        else if(dynamic_cast<const CRCore::crShapeDrawable*>(drawable))
            ((cre::crShapeDrawable*)(drawable))->write(this);
        else if(dynamic_cast<const CRText::crText*>(drawable))
            ((cre::crText*)(drawable))->write(this);
        //else
        //{
            //throw crException("Unknown drawable in crDataOutputStream::writeDrawable()");
       // }
        //if (m_verboseOutput) std::cout<<"read/writeDrawable() ["<<id<<"]"<<std::endl;
    }
}

void crDataOutputStream::writeShape(const CRCore::crShape* shape)
{
    ShapeMap::iterator itr = m_shapeMap.find(shape);
    if (itr!=m_shapeMap.end())
    {
        // Id already exists so just write ID.
        writeInt(itr->second);

        //if (m_verboseOutput) std::cout<<"read/writeShape() ["<<itr->second<<"]"<<std::endl;
    }
    else
    {
        // id doesn't exist so create a new ID and
        // register the stateset.

        int id = m_shapeMap.size();
        m_shapeMap[shape] = id;

        // write the id.
        writeInt(id);
        
        if(dynamic_cast<const CRCore::crSphere*>(shape))
            ((cre::crSphere*)(shape))->write(this);
        else if(dynamic_cast<const CRCore::crBox*>(shape))
            ((cre::crBox*)(shape))->write(this);
        else if(dynamic_cast<const CRCore::crCone*>(shape))
            ((cre::crCone*)(shape))->write(this);
        else if(dynamic_cast<const CRCore::crCylinder*>(shape))
            ((cre::crCylinder*)(shape))->write(this);
        else if(dynamic_cast<const CRCore::crCapsule*>(shape))
            ((cre::crCapsule*)(shape))->write(this);
        else if(dynamic_cast<const CRCore::crHeightField*>(shape))
            ((cre::crHeightField*)(shape))->write(this);
        else
        {
            throw crException("Unknown shape in crDataOutputStream::writeShape()");
        }
        //if (m_verboseOutput) std::cout<<"read/writeShape() ["<<id<<"]"<<std::endl;
    }
}

void crDataOutputStream::writeNode(const CRCore::crNode* node)
{
	bool tryFindNode = false;
	CRCore::crGroup* replaceGroup = NULL;
	CRCore::crNode* replaceNode = NULL;
	std::string command = node->getDescriptionsInString();
	//if(dynamic_cast<const CRAL::crSoundRoot*>(node))
	//{
	//	writeInt(CRESCRIPTDATA);
	//	return;
	//}
	//else if(dynamic_cast<const CRAL::crSoundNode*>(node))
	//{
	//	writeInt(CRESCRIPTDATA);
	//	return;
	//}
	/*else */if(dynamic_cast<const CRCore::crRenderToTextureManager *>(node))
	{
		writeInt(CRESCRIPTDATA);
		return;
	}
	else if(dynamic_cast<const CRPhysics::crVolumeNodeManager *>(node))
	{
		writeInt(CRESCRIPTDATA);
		return;
	}
	else if(dynamic_cast<const CRCore::crFilterRenderManager *>(node))
	{
		writeInt(CRESCRIPTDATA);
		return;
	}
	//else if(dynamic_cast<const CRPhysics::crCameraManager *>(node))
	//{
	//	writeInt(CRESCRIPTDATA);
	//	return;
	//}
	//else if(dynamic_cast<const CRParticle::crEffectParticleManager *>(node))
	//{
	//	replaceGroup = new CRCore::crGroup;
	//	replaceGroup->setName(node->getName());
	//	replaceGroup->setDescriptionsInString(command);
	//	node = replaceGroup;
	//}
	else if(dynamic_cast<const CRCore::crEffectSequenceManager *>(node))
	{
		replaceGroup = new CRCore::crGroup;
		replaceGroup->setName(node->getName());
		replaceGroup->setDescriptionsInString(command);
		node = replaceGroup;
	}
	else
	{
		if(command.find("#ScriptData")!=-1)
		{//用Group节点写该节点，不写其子节点
			replaceGroup = new CRCore::crGroup;
			replaceGroup->setName(node->getName());
			replaceGroup->setDescriptionsInString(command);
			node = replaceGroup;
		}
		else if(command.find("#ScriptAddGroup")!=-1)
		{//不写该节点，但写其子节点
			replaceGroup = dynamic_cast<CRCore::crGroup *>(const_cast<CRCore::crNode *>(node));
			replaceNode = replaceGroup->getChild(0);
			node = replaceNode;
			replaceGroup = NULL;
		}
		else if(command.find("#ScriptAddNode")!=-1)
		{//不写
			writeInt(CRESCRIPTDATA);
			return;
		}
		else
			tryFindNode = true;
	}
	if(replaceGroup) m_tempNodeStack.push(replaceGroup);
	if(replaceNode) m_tempNodeStack.push(replaceNode);

	if(tryFindNode)
	{
		NodeMap::iterator itr = m_nodeMap.find(node);
		if (itr!=m_nodeMap.end())
		{
			// Id already exists so just write ID.
			writeInt(itr->second);
			//if (m_verboseOutput) std::cout<<"read/writeNode() ["<<itr->second<<"]"<<std::endl;
			//std::cout<<"writeNode() find ["<<node->getName()<<"] id = "<<itr->second<<std::endl;
			return;
		}
	}

    // id doesn't exist so create a new ID and
    // register the stateset.
    int id = m_nodeMap.size();
    m_nodeMap[node] = id;

    // write the id.
    writeInt(id);

	if(command.find("#ExternNode")!=-1)
	{
		if(dynamic_cast<const CRCore::crGroup*>(node))
		{
			((cre::crGroup*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crObject*>(node))
		{
			((cre::crObject*)(node))->write(this);
		}
	}
	else
	{
		if(dynamic_cast<const CRPhysics::crBoxMatterObject*>(node)){
			((cre::crBoxMatterObject*)(node))->write(this);
		}
		else if(dynamic_cast<const CRPhysics::crSphereMatterObject*>(node)){
			((cre::crSphereMatterObject*)(node))->write(this);
		}
		else if(dynamic_cast<const CRPhysics::crCCylinderMatterObject*>(node)){
			((cre::crCCylinderMatterObject*)(node))->write(this);
		}
		else if(dynamic_cast<const CRPhysics::crMeshMatterObject*>(node)){
			((cre::crMeshMatterObject*)(node))->write(this);
		}
		//else if(dynamic_cast<const CRPhysics::crRayMatterObject*>(node)){
		//	((cre::crRayMatterObject*)(node))->write(this);
		//}
		else if(dynamic_cast<const CRPhysics::crMatterGroup*>(node)){
			((cre::crMatterGroup*)(node))->write(this);
		}
		else if(dynamic_cast<const CRUI::crScissorWidgetNode*>(node)){
			((cre::crScissorWidgetNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRUI::crListControlWidgetNode*>(node)){
			((cre::crListControlWidgetNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRUI::crTableWidgetNode*>(node)){
			((cre::crTableWidgetNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRUI::crChooseColorWidgetNode*>(node)){
			((cre::crChooseColorWidgetNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRUI::crComboBoxWidgetNode*>(node)){
			((cre::crComboBoxWidgetNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRUI::crSliderWidgetNode*>(node)){
			((cre::crSliderWidgetNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRUI::crProgressWidgetNode*>(node)){
			((cre::crProgressWidgetNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRUI::crRadioGroupWidgetNode*>(node)){
			((cre::crRadioGroupWidgetNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRUI::crRadioWidgetNode*>(node)){
			((cre::crRadioWidgetNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRUI::crCheckBoxWidgetNode*>(node)){
			((cre::crCheckBoxWidgetNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRUI::crImageBoxWidgetNode*>(node)){
			((cre::crImageBoxWidgetNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRUI::crListBoxWidgetNode*>(node)){
			((cre::crListBoxWidgetNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRUI::crEditWidgetNode*>(node)){
			((cre::crEditWidgetNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRUI::crStaticTextWidgetNode*>(node)){
			((cre::crStaticTextWidgetNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRUI::crHypertextWidgetNode*>(node)){
			((cre::crHypertextWidgetNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRUI::crScrollBarWidgetNode*>(node)){
			((cre::crScrollBarWidgetNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRUI::crButtonWidgetNode*>(node)){
			((cre::crButtonWidgetNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crCanvasNode*>(node)){
			((cre::crCanvasNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crCanvasNode*>(node)){
			((cre::crCanvasNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crWidgetNode*>(node)){
			((cre::crWidgetNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crMatrixTransform*>(node)){
			((cre::crMatrixTransform*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crPositionAttitudeTransform*>(node)){
			((cre::crPositionAttitudeTransform*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crDOFTransform*>(node)){
			((cre::crDOFTransform*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crCameraView*>(node)){
			((cre::crCameraView*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crCameraNode*>(node)){
			((cre::crCameraNode*)(node))->write(this);
		}
		//else if(dynamic_cast<const CRAI::crAIGroup*>(node)){
		//	((cre::crAIGroup*)(node))->write(this);
		//}
		//else if(dynamic_cast<const CRAI::crNerveCellGroup*>(node)){
		//	((cre::crNerveCellGroup*)(node))->write(this);
		//}
		//else if(dynamic_cast<const CRAI::crKeyboardMouseEventGroup*>(node)){
		//	((cre::crKeyboardMouseEventGroup*)(node))->write(this);
		//}
		else if(dynamic_cast<const CRCore::crEventGroup*>(node)){
			((cre::crEventGroup*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crLightSource*>(node)){
			((cre::crLightSource*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crTexGenNode*>(node)){
			((cre::crTexGenNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crClipNode*>(node)){
			((cre::crClipNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crMatrixSequence*>(node)){
			((cre::crMatrixSequence*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crSequence*>(node)){
			((cre::crSequence*)(node))->write(this);
		}
		//else if(dynamic_cast<const CRSim::crImpostor*>(node)){
		//    ((cre::crImpostor*)(node))->write(this);
		//}
		else if(dynamic_cast<const CRCore::crPagedLOD*>(node)){
			((cre::crPagedLOD*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crLod*>(node)){
			((cre::crLod*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crSwitch*>(node)){
			((cre::crSwitch*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crCoordinateSystemNode*>(node)){
			((cre::crCoordinateSystemNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crMultiSwitch*>(node)){
			((cre::crMultiSwitch*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crOccluderNode*>(node)){
			((cre::crOccluderNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crTransform*>(node)){
			((cre::crTransform*)(node))->write(this);
		}
		//else if(dynamic_cast<const CRSim::crVisibilityGroup*>(node)){
		//	((cre::crVisibilityGroup*)(node))->write(this);
		//}
		else if(dynamic_cast<const CRCore::crProxyNode*>(node)){
			((cre::crProxyNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crExternNode*>(node)){
			((cre::crExternNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRTerrain::crTerrainTile*>(node)){
			((cre::crTerrainTile*)(node))->write(this);
		}
		//else if(dynamic_cast<const CRFX::crMultiTextureControl*>(node)){
		//    ((cre::crMultiTextureControl*)(node))->write(this);
		//}
		else if(dynamic_cast<const CRCore::crDB*>(node)){
			((cre::crDB*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crTraverseStringNode*>(node)){
			((cre::crTraverseStringNode*)(node))->write(this);
		}
		else if(dynamic_cast<const CRParticle::crParticleEffect*>(node)){
			((cre::crParticleEffect*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crGroup*>(node)){
			((cre::crGroup*)(node))->write(this);
		}
		else if(dynamic_cast<const CRCore::crBillboard*>(node)){
			((cre::crBillboard*)(node))->write(this);
		}
		//else if(dynamic_cast<const CRCore::crDepthSortedObject*>(node)){
		//	((cre::crDepthSortedObject*)(node))->write(this);
		//}
		else if(dynamic_cast<const CRCore::crObject*>(node)){
			((cre::crObject*)(node))->write(this);
		}
		//else if(dynamic_cast<const CRSim::crLightPointNode*>(node)){
		//	((cre::crLightPointNode*)(node))->write(this);
		//}
		else
		{
			//throw crException("Unknown node in crGroup::write()");
			writeInt(CRESCRIPTDATA);
			return;
		}
	}

    //if (m_verboseOutput) std::cout<<"read/writeNode() ["<<id<<"]"<<std::endl;
	//std::cout<<"writeNode() ["<<node->getName()<<"] id = "<<id<<std::endl;
}

void crDataOutputStream::writeCondition(const CRCore::Condition* condition)
{
	ConditionMap::iterator itr = m_conditionMap.find(condition);
	if (itr!=m_conditionMap.end())
	{
		// Id already exists so just write ID.
		writeInt(itr->second);

		//if (m_verboseOutput) std::cout<<"read/writeCondition() ["<<itr->second<<"]"<<std::endl;
	}
	else
	{
		// id doesn't exist so create a new ID and
		// register the stateset.

		int id = m_conditionMap.size();
		m_conditionMap[condition] = id;

		// write the id.
		writeInt(id);

		if(dynamic_cast<const CRCore::Condition*>(condition))
			((cre::Condition*)(condition))->write(this);
		else
		{
			throw crException("Unknown Event in crDataOutputStream::writeCondition()");
		}
		//if (m_verboseOutput) std::cout<<"read/writeCondition() ["<<id<<"]"<<std::endl;
	}
}

void crDataOutputStream::writeSendMsg(const CRCore::SendMsg* sendMsg)
{
	SendMsgMap::iterator itr = m_sendMsgMap.find(sendMsg);
	if (itr!=m_sendMsgMap.end())
	{
		// Id already exists so just write ID.
		writeInt(itr->second);

		//if (m_verboseOutput) std::cout<<"read/writeSendMsg() ["<<itr->second<<"]"<<std::endl;
	}
	else
	{
		// id doesn't exist so create a new ID and
		// register the stateset.

		int id = m_sendMsgMap.size();
		m_sendMsgMap[sendMsg] = id;

		// write the id.
		writeInt(id);

		if(dynamic_cast<const CRCore::SendMsg*>(sendMsg))
			((cre::SendMsg*)(sendMsg))->write(this);
		else
		{
			throw crException("Unknown Event in crDataOutputStream::writeSendMsg()");
		}
		//if (m_verboseOutput) std::cout<<"read/writeSendMsg() ["<<id<<"]"<<std::endl;
	}
}

void crDataOutputStream::writeECondition(const CRCore::ECondition* econdition)
{
	EConditionMap::iterator itr = m_econditionMap.find(econdition);
	if (itr!=m_econditionMap.end())
	{
		// Id already exists so just write ID.
		writeInt(itr->second);

		//if (m_verboseOutput) std::cout<<"read/writeECondition() ["<<itr->second<<"]"<<std::endl;
	}
	else
	{
		// id doesn't exist so create a new ID and
		// register the stateset.

		int id = m_econditionMap.size();
		m_econditionMap[econdition] = id;

		// write the id.
		writeInt(id);

		if(dynamic_cast<const CRCore::ECondition*>(econdition))
			((cre::ECondition*)(econdition))->write(this);
		else
		{
			throw crException("Unknown Event in crDataOutputStream::writeECondition()");
		}
		//if (m_verboseOutput) std::cout<<"read/writeECondition() ["<<id<<"]"<<std::endl;
	}
}

void crDataOutputStream::writeEvent(const CRCore::crEvent* event)
{
	EventMap::iterator itr = m_eventMap.find(event);
	if (itr!=m_eventMap.end())
	{
		// Id already exists so just write ID.
		writeInt(itr->second);

		//if (m_verboseOutput) std::cout<<"read/writeEvent() ["<<itr->second<<"]"<<std::endl;
	}
	else
	{
		// id doesn't exist so create a new ID and
		// register the stateset.

		int id = m_eventMap.size();
		m_eventMap[event] = id;

		// write the id.
		writeInt(id);

		if(dynamic_cast<const CRCore::crEvent*>(event))
			((cre::crEvent*)(event))->write(this);
		else
		{
			throw crException("Unknown Event in crDataOutputStream::writeEvent()");
		}
		//if (m_verboseOutput) std::cout<<"read/writeEvent() ["<<id<<"]"<<std::endl;
	}
}

void crDataOutputStream::writeEventMsg(const CRCore::crEventMsg* eventMsg)
{
	EventMsgMap::iterator itr = m_eventMsgMap.find(eventMsg);
	if (itr!=m_eventMsgMap.end())
	{
		// Id already exists so just write ID.
		writeInt(itr->second);

		//if (m_verboseOutput) std::cout<<"read/writeEventMsg() ["<<itr->second<<"]"<<std::endl;
	}
	else
	{
		// id doesn't exist so create a new ID and
		// register the stateset.

		int id = m_eventMsgMap.size();
		m_eventMsgMap[eventMsg] = id;

		// write the id.
		writeInt(id);

		if(dynamic_cast<const CRCore::crWhile_EventMsg*>(eventMsg))
			((cre::crWhile_EventMsg*)(eventMsg))->write(this);
		else if(dynamic_cast<const CRCore::crSwitch_EventMsg*>(eventMsg))
			((cre::crSwitch_EventMsg*)(eventMsg))->write(this);
		else if(dynamic_cast<const CRCore::crIF_ELSE_EventMsg*>(eventMsg))
			((cre::crIF_ELSE_EventMsg*)(eventMsg))->write(this);
		else if(dynamic_cast<const CRCore::crEventMsg*>(eventMsg))
			((cre::crEventMsg*)(eventMsg))->write(this);
		else
		{
			throw crException("Unknown EventMsg in crDataOutputStream::writeEventMsg()");
		}
		//if (m_verboseOutput) std::cout<<"read/writeEventMsg() ["<<id<<"]"<<std::endl;
	}
}

//void crDataOutputStream::writePPC(const CRCore::ParseParamCallback* ppc)
//{
//	PPCMap::iterator itr = m_ppcMap.find(ppc);
//	if (itr!=m_ppcMap.end())
//	{
//		// Id already exists so just write ID.
//		writeInt(itr->second);
//
//		if (m_verboseOutput) std::cout<<"read/writePPC() ["<<itr->second<<"]"<<std::endl;
//	}
//	else
//	{
//		// id doesn't exist so create a new ID and
//		// register the stateset.
//
//		int id = m_ppcMap.size();
//		m_ppcMap[ppc] = id;
//
//		// write the id.
//		writeInt(id);
//
//		if(dynamic_cast<const CRPhysics::crSetForceAndTorquePPC*>(ppc))
//			((cre::crSetForceAndTorquePPC*)(ppc))->write(this);
//		else if(dynamic_cast<const CRPhysics::crSetGravityModePPC*>(ppc))
//			((cre::crSetGravityModePPC*)(ppc))->write(this);
//		else if(dynamic_cast<const CRPhysics::crJointSetParamPPC*>(ppc))
//			((cre::crJointSetParamPPC*)(ppc))->write(this);
//		else if(dynamic_cast<const CRPhysics::crPhysicsResetPPC*>(ppc))
//			((cre::crPhysicsResetPPC*)(ppc))->write(this);
//		else if(dynamic_cast<const CRPhysics::crSetGeomorBodyInfoPPC*>(ppc))
//			((cre::crSetGeomorBodyInfoPPC*)(ppc))->write(this);
//		else if(dynamic_cast<const CRPhysics::crGeneralParamTransferPPC*>(ppc))
//			((cre::crGeneralParamTransferPPC*)(ppc))->write(this);
//		//else if(dynamic_cast<const CRPhysics::crFXEventPPC*>(ppc))
//		//	((cre::crFXEventPPC*)(ppc))->write(this);
//		//else if(dynamic_cast<const CRAI::crKeyboardMousePPC*>(ppc))
//		//    ((cre::crKeyboardMousePPC*)(ppc))->write(this);
//		//else if(dynamic_cast<const CRAI::crCarManipulatorPPC*>(ppc))
//		//	((cre::crCarManipulatorPPC*)(ppc))->write(this);
//		//else if(dynamic_cast<const CRAI::crRobotManipulatorPPC*>(ppc))
//		//	((cre::crRobotManipulatorPPC*)(ppc))->write(this);
//		//else if(dynamic_cast<const CRAI::crCharacterManipulatorPPC*>(ppc))
//		//	((cre::crCharacterManipulatorPPC*)(ppc))->write(this);
//		//else if(dynamic_cast<const CRAI::crRadarCollidePPC*>(ppc))
//		//	((cre::crRadarCollidePPC*)(ppc))->write(this);
//		//else if(dynamic_cast<const CRAI::crDiscoverAIPPC*>(ppc))
//		//	((cre::crDiscoverAIPPC*)(ppc))->write(this);
///*		else if(dynamic_cast<const CRAI::crCollidePPC*>(ppc))
//			((cre::crCollidePPC*)(ppc))->write(this);*/
//		else if(dynamic_cast<const CRPhysics::crWeaponEventPPC*>(ppc))
//			((cre::crWeaponEventPPC*)(ppc))->write(this);
//		else
//		{
//			throw crException("Unknown ParseParamCallback in crDataOutputStream::writePPC()");
//		}
//		if (m_verboseOutput) std::cout<<"read/writeEventMsg() ["<<id<<"]"<<std::endl;
//	}
//}
//
void crDataOutputStream::writeJointParam(const CRPhysics::crJointParameter* jointParam)
{
	JointParamMap::iterator itr = m_jointParamMap.find(jointParam);
	if (itr!=m_jointParamMap.end())
	{
		// Id already exists so just write ID.
		writeInt(itr->second);

		//if (m_verboseOutput) std::cout<<"read/writeJointParam() ["<<itr->second<<"]"<<std::endl;
	}
	else
	{
		// id doesn't exist so create a new ID and
		// register the stateset.

		int id = m_jointParamMap.size();
		m_jointParamMap[jointParam] = id;

		// write the id.
		writeInt(id);

		if(dynamic_cast<const CRPhysics::crJointParameter*>(jointParam))
			((cre::crJointParameter*)(jointParam))->write(this);
		else
		{
			throw crException("Unknown Event in crDataOutputStream::writeJointParam()");
		}
		//if (m_verboseOutput) std::cout<<"read/writeJointParam() ["<<id<<"]"<<std::endl;
	}
}

void crDataOutputStream::writeJoint(const CRPhysics::crJoint* joint)
{
	JointMap::iterator itr = m_jointMap.find(joint);
	if (itr!=m_jointMap.end())
	{
		// Id already exists so just write ID.
		writeInt(itr->second);

		//if (m_verboseOutput) std::cout<<"read/writeJoint() ["<<itr->second<<"]"<<std::endl;
	}
	else
	{
		// id doesn't exist so create a new ID and
		// register the stateset.

		int id = m_jointMap.size();
		m_jointMap[joint] = id;

		// write the id.
		writeInt(id);

		if(dynamic_cast<const CRPhysics::crJointNull*>(joint))
			((cre::crJointNull*)(joint))->write(this);
		else if(dynamic_cast<const CRPhysics::crJointAMotor*>(joint))
			((cre::crJointAMotor*)(joint))->write(this);
		else if(dynamic_cast<const CRPhysics::crJointFixed*>(joint))
			((cre::crJointFixed*)(joint))->write(this);
		else if(dynamic_cast<const CRPhysics::crJointHinge2*>(joint))
			((cre::crJointHinge2*)(joint))->write(this);
		else if(dynamic_cast<const CRPhysics::crJointContact*>(joint))
			((cre::crJointContact*)(joint))->write(this);
		else if(dynamic_cast<const CRPhysics::crJointUniversal*>(joint))
			((cre::crJointUniversal*)(joint))->write(this);
		else if(dynamic_cast<const CRPhysics::crJointSlider*>(joint))
			((cre::crJointSlider*)(joint))->write(this);
		else if(dynamic_cast<const CRPhysics::crJointHinge*>(joint))
			((cre::crJointHinge*)(joint))->write(this);
		else if(dynamic_cast<const CRPhysics::crJointBall*>(joint))
			((cre::crJointBall*)(joint))->write(this);
		//else if(dynamic_cast<const CRPhysics::crJoint*>(joint))
		//	((cre::crJoint*)(joint))->write(this);
		else
		{
			throw crException("Unknown Joint in crDataOutputStream::writeJoint()");
		}
		//if (m_verboseOutput) std::cout<<"read/writeJoint() ["<<id<<"]"<<std::endl;
	}
}

//void crDataOutputStream::writeSoundState(const CRAL::crSoundState* soundState)
//{
//	SoundStateMap::iterator itr = m_soundStateMap.find(soundState);
//	if (itr!=m_soundStateMap.end())
//	{
//		// Id already exists so just write ID.
//		writeInt(itr->second);
//
//		//if (m_verboseOutput) std::cout<<"read/writeSoundState() ["<<itr->second<<"]"<<std::endl;
//	}
//	else
//	{
//		// id doesn't exist so create a new ID and
//		// register the stateset.
//
//		int id = m_soundStateMap.size();
//		m_soundStateMap[soundState] = id;
//
//		// write the id.
//		writeInt(id);
//
//		((cre::crSoundState*)(soundState))->write(this);
//
//		//if (m_verboseOutput) std::cout<<"read/writeJoint() ["<<id<<"]"<<std::endl;
//	}
//}

//void crDataOutputStream::writeSoundOccludeCallback(const CRAL::crOccludeCallback* soundocc)
//{
//	SoundOccludeCallbackMap::iterator itr = m_soundOccludeCallbackMap.find(soundocc);
//	if (itr!=m_soundOccludeCallbackMap.end())
//	{
//		// Id already exists so just write ID.
//		writeInt(itr->second);
//
//		if (m_verboseOutput) std::cout<<"read/writeSoundOccludeCallback() ["<<itr->second<<"]"<<std::endl;
//	}
//	else
//	{
//		// id doesn't exist so create a new ID and
//		// register the stateset.
//
//		int id = m_soundOccludeCallbackMap.size();
//		m_soundOccludeCallbackMap[soundocc] = id;
//
//		// write the id.
//		writeInt(id);
//
//		((cre::crSoundOccludeCallback*)(soundocc))->write(this);
//
//		if (m_verboseOutput) std::cout<<"read/writeSoundOccludeCallback() ["<<id<<"]"<<std::endl;
//	}
//}

void crDataOutputStream::writeCollideCallback(const CRCore::crCollideCallback* collideCallback)
{
	CollideCallbackMap::iterator itr = m_collideCallbackMap.find(collideCallback);
	if (itr!=m_collideCallbackMap.end())
	{
		// Id already exists so just write ID.
		writeInt(itr->second);

		//if (m_verboseOutput) std::cout<<"read/writeCollideCallback() ["<<itr->second<<"]"<<std::endl;
	}
	else
	{
		// id doesn't exist so create a new ID and
		// register the stateset.

		int id = m_collideCallbackMap.size();
		m_collideCallbackMap[collideCallback] = id;

		// write the id.
		writeInt(id);

/*		if(dynamic_cast<const CRAL::crSoundCollideCallback*>(collideCallback))
			((cre::crSoundCollideCallback*)(collideCallback))->write(this);
		else */if(dynamic_cast<const CRCore::crCollideCallback*>(collideCallback))
			((cre::crCollideCallback*)(collideCallback))->write(this);
		else
		{
			throw crException("Unknown crCollideCallback in crDataOutputStream::writeCollideCallback()");
		}
		//if (m_verboseOutput) std::cout<<"read/writeCollideCallback() ["<<id<<"]"<<std::endl;
	}
}
void crDataOutputStream::pushDataFile(const std::string &datafile)
{
	m_dataFileStack.push(datafile);
}
void crDataOutputStream::getCurrentDataFile(std::string &datafile)
{
	if(!m_dataFileStack.empty())
		datafile = m_dataFileStack.top(); 
}
void crDataOutputStream::popDataFile()
{
	if(!m_dataFileStack.empty())
		m_dataFileStack.pop();
}