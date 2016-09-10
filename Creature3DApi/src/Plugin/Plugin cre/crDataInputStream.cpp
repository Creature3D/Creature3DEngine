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
#include <Plugin cre/crDataInputStream.h>
#include <Plugin cre/crStateSet.h>
#include <Plugin cre/crAlphaFunc.h>
#include <Plugin cre/crBlendFunc.h>
#include <Plugin cre/crDepth.h>
#include <Plugin cre/crMaterial.h>
#include <Plugin cre/crCullFace.h>
#include <Plugin cre/crClipPlane.h>
#include <Plugin cre/crPolygonOffset.h>
#include <Plugin cre/crPolygonMode.h>
//#include "PolygonMode.h"
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
#include <Plugin cre/crLight.h>
#include <Plugin cre/crProxyNode.h>
#include <Plugin cre/crFrontFace.h>
#include <Plugin cre/crProgram.h>
#include <Plugin cre/crFog.h>
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
#include <Plugin cre/crUniform.h>
#include <Plugin cre/crShader.h>

#include <Plugin cre/crCameraView.h>
#include <Plugin cre/crCameraNode.h>

//#include <Plugin cre/crLightPointNode.h>
#include <Plugin cre/crMultiSwitch.h>
//#include <Plugin cre/crVisibilityGroup.h>

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
//#include <Plugin cre/crNerveCellGroup.h>
//#include <Plugin cre/crAIGroup.h>
#include <Plugin cre/crEventGroup.h>
#include <Plugin cre/crJoint.h>

//#include <Plugin cre/crParseParamCallback.h>

//#include <Plugin cre/crKeyboardMouseEventGroup.h>
//#include <Plugin cre/crSoundState.h>
//#include <Plugin cre/crSoundNode.h>
#include <Plugin cre/crCollideCallback.h>
#include <Plugin cre/crTerrainTile.h>
//#include <Plugin cre/crSoundCollideCallback.h>
//#include <Plugin cre/crSoundOccludeCallback.h>

#include <CRCore/crEndian.h>
#include <CRCore/crNotify.h>
//#include <CRCore/io_utils.h>
#include <CRIOManager/crReadFile.h>
#include <CRCore/crBrain.h>
#include <Plugin cre/crTraverseStringNode.h>
#include <Plugin cre/crWidgetNode.h>
#include <Plugin cre/crParticleEffect.h>
#include <CRCore/crFilterRenderManager.h>
#include <CRUtil/crOptimizer.h>

using namespace cre;
using namespace std;

void crDataInputStream::setOptions(const CRIOManager::crReaderWriter::Options* options) 
{ 
    m_options = options; 

    if (m_options.get())
    {
        setLoadExternalReferenceFiles(m_options->getOptionString().find("noLoadExternalReferenceFiles")==std::string::npos);
        //CRCore::notify(CRCore::DEBUG_INFO) << "cre::crDataInputStream.setLoadExternalReferenceFiles()=" << getLoadExternalReferenceFiles() << std::endl;
    }
}

crDataInputStream::crDataInputStream(std::istream* istream)
{
    unsigned int endianType ;

    m_loadExternalReferenceFiles = false;
    
    m_verboseOutput = false;

    m_istream = istream;
    m_peeking = false;
    m_peekValue = 0;
    m_byteswap = 0;

    if(!istream){
        throw crException("crDataInputStream::crDataInputStream(): null pointer exception in argument.");    
    }

    endianType = readUInt() ;
    
    if ( endianType != ENDIAN_TYPE) {
      // Make sure the file is simply swapped
      if ( endianType != OPPOSITE_ENDIAN_TYPE ) {
         throw crException("crDataInputStream::crDataInputStream(): This file has an unreadable endian type.") ;
      }
      //CRCore::notify(CRCore::INFO)<<"crDataInputStream::crDataInputStream: Reading a byteswapped file" << std::endl ;
      m_byteswap = 1 ;
   }
    
    m_version = readUInt();
        
    // Are we trying to open a binary .cre file which version are newer than this library.
    if(m_version>VERSION){
        throw crException("crDataInputStream::crDataInputStream(): The version found in the file is newer than this library can handle.");
    }
        
}

crDataInputStream::~crDataInputStream()
{
}

bool crDataInputStream::readBool(){
    char c;
    m_istream->read(&c, CHARSIZE);
    
    if (m_istream->rdstate() & m_istream->failbit)
        throw crException("crDataInputStream::readBool(): Failed to read boolean value.");

    //if (m_verboseOutput) std::cout<<"read/writeBool() ["<<(int)c<<"]"<<std::endl;
    
    return c!=0;
}

unsigned int crDataInputStream::getVersion()
{
    return( m_version );
}

char crDataInputStream::readChar(){
    char c;
    m_istream->read(&c, CHARSIZE);

    if (m_istream->rdstate() & m_istream->failbit)
        throw crException("crDataInputStream::readChar(): Failed to read char value.");

    //if (m_verboseOutput) std::cout<<"read/writeChar() ["<<(int)c<<"]"<<std::endl;
    
    return c;
}

unsigned char crDataInputStream::readUChar(){
    unsigned char c;
    m_istream->read((char*)&c, CHARSIZE);

    if (m_istream->rdstate() & m_istream->failbit)
        throw crException("crDataInputStream::readUChar(): Failed to read unsigned char value.");

    //if (m_verboseOutput) std::cout<<"read/writeUChar() ["<<(int)c<<"]"<<std::endl;
    
    return c;
}

unsigned short crDataInputStream::readUShort(){
    unsigned short s;
    m_istream->read((char*)&s, SHORTSIZE);
    if (m_istream->rdstate() & m_istream->failbit)
        throw crException("crDataInputStream::readUShort(): Failed to read unsigned short value.");

    //if (m_verboseOutput) std::cout<<"read/writeUShort() ["<<s<<"]"<<std::endl;
    
    if (m_byteswap) CRCore::swapBytes((char *)&s,SHORTSIZE);
    
    return s;
}

unsigned int crDataInputStream::readUInt(){
    unsigned int s;
    m_istream->read((char*)&s, INTSIZE);

    if (m_istream->rdstate() & m_istream->failbit)
	{
        throw crException("crDataInputStream::readUInt(): Failed to read unsigned int value.");
	}

    //if (m_verboseOutput) std::cout<<"read/writeUInt() ["<<s<<"]"<<std::endl;
    
    if (m_byteswap) CRCore::swapBytes((char *)&s,INTSIZE);
    
    return s;
}

int crDataInputStream::readInt(){
    if(m_peeking){
        m_peeking = false;
        return m_peekValue;
    }
    int i;
    m_istream->read((char*)&i, INTSIZE);

    // comment out for time being as this check seems to eroneously cause a
    // premature exit when reading .cre files under OSX!#?:!
    // Robet Osfield, September 12th 2003.
    // if (m_istream->rdstate() & m_istream->failbit)
    //    throw crException("crDataInputStream::readInt(): Failed to read int value.");

    //if (m_verboseOutput) std::cout<<"read/writeInt() ["<<i<<"]"<<std::endl;
    
    if (m_byteswap) CRCore::swapBytes((char *)&i,INTSIZE);

    return i;
}

/**
 * Read an integer from the stream, but
 * save it such that the next readInt call will
 * return the same integer.
 */
int crDataInputStream::peekInt(){
    if(m_peeking){
        return m_peekValue;
    }
    m_peekValue  = readInt();
    m_peeking = true;
    return m_peekValue;
}

float crDataInputStream::readFloat(){
    float f;
    m_istream->read((char*)&f, FLOATSIZE);
    if (m_istream->rdstate() & m_istream->failbit)
        throw crException("crDataInputStream::readFloat(): Failed to read float value.");

    //if (m_verboseOutput) std::cout<<"read/writeFloat() ["<<f<<"]"<<std::endl;
    
    if (m_byteswap) CRCore::swapBytes((char *)&f,FLOATSIZE);
    return f;
}

long crDataInputStream::readLong(){
    long l;
    m_istream->read((char*)&l, LONGSIZE);
    if (m_istream->rdstate() & m_istream->failbit)
        throw crException("crDataInputStream::readLong(): Failed to read long value.");

    //if (m_verboseOutput) std::cout<<"read/writeLong() ["<<l<<"]"<<std::endl;
    
    if (m_byteswap) CRCore::swapBytes((char *)&l,LONGSIZE);
    return l;
}

unsigned long crDataInputStream::readULong(){
    unsigned long l;
    m_istream->read((char*)&l, LONGSIZE);
    if (m_istream->rdstate() & m_istream->failbit)
        throw crException("crDataInputStream::readULong(): Failed to read unsigned long value.");

    //if (m_verboseOutput) std::cout<<"read/writeULong() ["<<l<<"]"<<std::endl;
    
    return l;
}

_crInt64 crDataInputStream::readInt64()
{
	_crInt64 i;
	m_istream->read((char*)&i, INT64SIZE);
	if (m_istream->rdstate() & m_istream->failbit)
		throw crException("crDataInputStream::readULong(): Failed to read unsigned long value.");

	//if (m_verboseOutput) std::cout<<"read/writeInt64() ["<<i<<"]"<<std::endl;

	return i;
}

double crDataInputStream::readDouble(){
    double d;
    m_istream->read((char*)&d, DOUBLESIZE);
    if (m_istream->rdstate() & m_istream->failbit)
        throw crException("crDataInputStream::readDouble(): Failed to read double value.");

    //if (m_verboseOutput) std::cout<<"read/writeDouble() ["<<d<<"]"<<std::endl;
    
    if (m_byteswap) CRCore::swapBytes((char *)&d,DOUBLESIZE);
    return d;
}

std::string crDataInputStream::readString(){
    std::string s;
    int size = readInt();
    s.resize(size);
    m_istream->read((char*)s.c_str(), size);
    //if (m_istream->rdstate() & m_istream->failbit)
    //   throw crException("crDataInputStream::readString(): Failed to read string value.");

    //if (m_verboseOutput) std::cout<<"read/writeString() ["<<s<<"]"<<std::endl;
    
    return s;
}

void crDataInputStream::readCharArray(char* data, int size){
    m_istream->read(data, size);
    if (m_istream->rdstate() & m_istream->failbit)
        throw crException("crDataInputStream::readCharArray(): Failed to read char value.");

    //if (m_verboseOutput) std::cout<<"read/writeCharArray() ["<<data<<"]"<<std::endl;
}

CRCore::crVector2f crDataInputStream::readVec2()
{
    CRCore::crVector2f v;
    v.x()=readFloat();
    v.y()=readFloat();

    //if (m_verboseOutput) std::cout<<"read/writeVec2() ["<<v<<"]"<<std::endl;
    
    return v;
}

CRCore::crVector3f crDataInputStream::readVec3(){
    CRCore::crVector3f v;
    v.x()=readFloat();
    v.y()=readFloat();
    v.z()=readFloat();

    //if (m_verboseOutput) std::cout<<"read/writeVec3() ["<<v<<"]"<<std::endl;

    return v;
}

CRCore::crVector4f crDataInputStream::readVec4(){
    CRCore::crVector4f v;
    v.x()=readFloat();
    v.y()=readFloat();
    v.z()=readFloat();
    v.w()=readFloat();

    //if (m_verboseOutput) std::cout<<"read/writeVec4() ["<<v<<"]"<<std::endl;
    
    return v;
}
CRCore::crVector2d crDataInputStream::readVec2d()
{
    CRCore::crVector2d v;
    v.x()=readDouble();
    v.y()=readDouble();

    //if (m_verboseOutput) std::cout<<"read/writeVec2d() ["<<v<<"]"<<std::endl;
    
    return v;
}

CRCore::crVector3d crDataInputStream::readVec3d(){
    CRCore::crVector3d v;
    v.x()=readDouble();
    v.y()=readDouble();
    v.z()=readDouble();

    //if (m_verboseOutput) std::cout<<"read/writeVec3d() ["<<v<<"]"<<std::endl;

    return v;
}

CRCore::crVector4d crDataInputStream::readVec4d(){
    CRCore::crVector4d v;
    v.x()=readDouble();
    v.y()=readDouble();
    v.z()=readDouble();
    v.w()=readDouble();

    //if (m_verboseOutput) std::cout<<"read/writeVec4d() ["<<v<<"]"<<std::endl;
    
    return v;
}

CRCore::crVector2i crDataInputStream::readVec2i()
{
	CRCore::crVector2i v;
	v.x()=readInt();
	v.y()=readInt();

	//if (m_verboseOutput) std::cout<<"read/writeVec2i() ["<<v<<"]"<<std::endl;

	return v;
}

CRCore::crVector3i crDataInputStream::readVec3i(){
	CRCore::crVector3i v;
	v.x()=readInt();
	v.y()=readInt();
	v.z()=readInt();

	//if (m_verboseOutput) std::cout<<"read/writeVec3i() ["<<v<<"]"<<std::endl;

	return v;
}

CRCore::crVector4i crDataInputStream::readVec4i(){
	CRCore::crVector4i v;
	v.x()=readInt();
	v.y()=readInt();
	v.z()=readInt();
	v.w()=readInt();

	//if (m_verboseOutput) std::cout<<"read/writeVec4i() ["<<v<<"]"<<std::endl;

	return v;
}
CRCore::crVector2b crDataInputStream::readVec2b()
{
	CRCore::crVector2b v;
	v.x()=readBool();
	v.y()=readBool();

	//if (m_verboseOutput) std::cout<<"read/writeVec2b() ["<<v<<"]"<<std::endl;

	return v;
}

CRCore::crVector3b crDataInputStream::readVec3b(){
	CRCore::crVector3b v;
	v.x()=readBool();
	v.y()=readBool();
	v.z()=readBool();

	//if (m_verboseOutput) std::cout<<"read/writeVec3b() ["<<v<<"]"<<std::endl;

	return v;
}

CRCore::crVector4b crDataInputStream::readVec4b(){
	CRCore::crVector4b v;
	v.x()=readBool();
	v.y()=readBool();
	v.z()=readBool();
	v.w()=readBool();

	//if (m_verboseOutput) std::cout<<"read/writeVec4b() ["<<v<<"]"<<std::endl;

	return v;
}

CRCore::crPlane crDataInputStream::readPlane(){
    CRCore::crPlane v;
    v[0]=readFloat();
    v[1]=readFloat();
    v[2]=readFloat();
    v[3]=readFloat();

    //if (m_verboseOutput) std::cout<<"read/writePlane() ["<<v<<"]"<<std::endl;
    
    return v;
}

CRCore::crVector4ub crDataInputStream::readUByte4(){
    CRCore::crVector4ub v;
    v.r()=readChar();
    v.g()=readChar();
    v.b()=readChar();
    v.a()=readChar();

    //if (m_verboseOutput) std::cout<<"read/writeUByte4() ["<<v<<"]"<<std::endl;
    
    return v;
}

CRCore::crQuat crDataInputStream::readQuat(){
    CRCore::crQuat q;
    q.x()=readFloat();
    q.y()=readFloat();
    q.z()=readFloat();
    q.w()=readFloat();

    //if (m_verboseOutput) std::cout<<"read/writeQuat() ["<<q<<"]"<<std::endl;
    
    return q;
}




CRCore::crGeometry::AttributeBinding crDataInputStream::readBinding(){
    char c = readChar();

    //if (m_verboseOutput) std::cout<<"read/writeBinding() ["<<(int)c<<"]"<<std::endl;
    
    switch((int)c){
        case 0:    return CRCore::crGeometry::BIND_OFF;
        case 1: return CRCore::crGeometry::BIND_OVERALL;
        case 2: return CRCore::crGeometry::BIND_PER_PRIMITIVE_SET;
        case 3: return CRCore::crGeometry::BIND_PER_PRIMITIVE;
        case 4: return CRCore::crGeometry::BIND_PER_VERTEX;
        default: throw crException("Unknown binding type in crDataInputStream::readBinding()");
    }
}

CRCore::Array* crDataInputStream::readArray(){
    char c = readChar();

    switch((int)c){
        case 0: return readIntArray();
        case 1: return readUByteArray();
        case 2: return readUShortArray();
        case 3: return readUIntArray();
        case 4: return readUByte4Array();
        case 5: return readFloatArray();
        case 6:    return readVec2Array();
        case 7:    return readVec3Array();
        case 8:    return readVec4Array();
        default: throw crException("Unknown array type in crDataInputStream::readArray()");
    }
}

CRCore::IntArray* crDataInputStream::readIntArray(){
    int size = readInt();
    CRCore::IntArray* a = new CRCore::IntArray(size);
    
    m_istream->read((char*)&((*a)[0]), INTSIZE*size);

    if (m_istream->rdstate() & m_istream->failbit)
        throw crException("crDataInputStream::readIntArray(): Failed to read Int array.");

   // if (m_verboseOutput) std::cout<<"read/writeIntArray() ["<<size<<"]"<<std::endl;  

    if (m_byteswap) {
       for (int  i = 0 ; i < size ; i++ ) CRCore::swapBytes((char *)&(a[i]),INTSIZE) ;
    }
       
    return a;
}

CRCore::UByteArray* crDataInputStream::readUByteArray(){
    int size = readInt();
    CRCore::UByteArray* a = new CRCore::UByteArray(size);

    m_istream->read((char*)&((*a)[0]), CHARSIZE*size);

    if (m_istream->rdstate() & m_istream->failbit)
        throw crException("crDataInputStream::readUByteArray(): Failed to read UByte array.");

    //if (m_verboseOutput) std::cout<<"read/writeUByteArray() ["<<size<<"]"<<std::endl;
    
    return a;
}

CRCore::UShortArray* crDataInputStream::readUShortArray(){
    int size = readInt();
    CRCore::UShortArray* a = new CRCore::UShortArray(size);

    m_istream->read((char*)&((*a)[0]), SHORTSIZE*size);

    if (m_istream->rdstate() & m_istream->failbit)
        throw crException("crDataInputStream::readUShortArray(): Failed to read UShort array.");

   // if (m_verboseOutput) std::cout<<"read/writeUShortArray() ["<<size<<"]"<<std::endl;
    
    if (m_byteswap)
    {
       for (int i = 0 ; i < size ; i++ ) CRCore::swapBytes((char *)&(a[i]),SHORTSIZE) ;
    }
    return a;
}

CRCore::UIntArray* crDataInputStream::readUIntArray()
{
    int size = readInt();
    CRCore::UIntArray* a = new CRCore::UIntArray(size);

    m_istream->read((char*)&((*a)[0]), INTSIZE*size);

    if (m_istream->rdstate() & m_istream->failbit)
        throw crException("crDataInputStream::readUIntArray(): Failed to read UInt array.");

    //if (m_verboseOutput) std::cout<<"read/writeUIntArray() ["<<size<<"]"<<std::endl;
    
    if (m_byteswap)
    {
       for (int i = 0 ; i < size ; i++ ) CRCore::swapBytes((char *)&(a[i]),INTSIZE) ;
    }
    return a;
}

CRCore::Vec4ubArray* crDataInputStream::readUByte4Array()
{
    int size = readInt();
    CRCore::Vec4ubArray* a = new CRCore::Vec4ubArray(size);

    m_istream->read((char*)&((*a)[0]), INTSIZE*size);

    if (m_istream->rdstate() & m_istream->failbit)
        throw crException("crDataInputStream::readUbyte4Array(): Failed to read crVector4ub array.");

   // if (m_verboseOutput) std::cout<<"read/writeUByte4Array() ["<<size<<"]"<<std::endl;
    
    return a;
}

CRCore::FloatArray* crDataInputStream::readFloatArray()
{
    int size = readInt();
    
    CRCore::FloatArray* a = new CRCore::FloatArray(size);
    
    m_istream->read((char*)&((*a)[0]), FLOATSIZE*size);

    if (m_istream->rdstate() & m_istream->failbit)
        throw crException("crDataInputStream::readFloatArray(): Failed to read float array.");

    //if (m_verboseOutput) std::cout<<"read/writeFloatArray() ["<<size<<"]"<<std::endl;
    
    if (m_byteswap)
    {
       for (int i = 0 ; i < size ; i++ ) CRCore::swapBytes((char *)&(a[i]),FLOATSIZE) ;
    }
    return a;
}

CRCore::Vec2Array* crDataInputStream::readVec2Array()
{
    int size = readInt();
    CRCore::Vec2Array* a = new CRCore::Vec2Array(size);
    
    m_istream->read((char*)&((*a)[0]), FLOATSIZE*2*size);

    if (m_istream->rdstate() & m_istream->failbit)
        throw crException("crDataInputStream::readVec2Array(): Failed to read crVector2f array.");

    //if (m_verboseOutput) std::cout<<"read/writeVec2Array() ["<<size<<"]"<<std::endl;
    
    if (m_byteswap)
    {
       float *ptr = (float*)&((*a)[0]) ;
       for (int i = 0 ; i < size*2 ; i++ )
       {
          CRCore::swapBytes((char *)&(ptr[i]), FLOATSIZE) ;
       }
    }
    return a;
}

CRCore::Vec3Array* crDataInputStream::readVec3Array()
{
    int size = readInt();
    CRCore::Vec3Array* a = new CRCore::Vec3Array(size);

    m_istream->read((char*)&((*a)[0]), FLOATSIZE*3*size);
    
    if (m_istream->rdstate() & m_istream->failbit)
        throw crException("crDataInputStream::readVec3Array(): Failed to read crVector3f array.");

    //if (m_verboseOutput) std::cout<<"read/writeVec3Array() ["<<size<<"]"<<std::endl;
    

    if (m_byteswap)
    {
       float *ptr = (float*)&((*a)[0]) ;
       for (int i = 0 ; i < size*3 ; i++ ) {
          CRCore::swapBytes((char *)&(ptr[i]),FLOATSIZE) ;
       }
    }
    return a;
}

CRCore::Vec4Array* crDataInputStream::readVec4Array(){
    int size = readInt();
    CRCore::Vec4Array* a = new CRCore::Vec4Array(size);

    m_istream->read((char*)&((*a)[0]), FLOATSIZE*4*size);

    if (m_istream->rdstate() & m_istream->failbit)
        throw crException("crDataInputStream::readVec4Array(): Failed to read crVector4f array.");

    //if (m_verboseOutput) std::cout<<"read/writeVec4Array() ["<<size<<"]"<<std::endl;
    
    if (m_byteswap) {
       float *ptr = (float*)&((*a)[0]) ;
       for (int i = 0 ; i < size*4 ; i++ ) {
          CRCore::swapBytes((char *)&(ptr[i]),FLOATSIZE) ;
       }
    }
    return a;
}

CRCore::crMatrixf crDataInputStream::readMatrixf()
{
    CRCore::crMatrixf mat;
    for(int r=0;r<4;r++)
    {
        for(int c=0;c<4;c++)
        {
            mat(r,c) = readFloat();
        }
    }

    if (m_istream->rdstate() & m_istream->failbit)
        throw crException("crDataInputStream::readMatrix(): Failed to read crMatrix array.");

    //if (m_verboseOutput) std::cout<<"read/writeMatrix() ["<<mat<<"]"<<std::endl;
    

    return mat;
}

CRCore::crMatrixd crDataInputStream::readMatrixd()
{
    CRCore::crMatrixd mat;
    for(int r=0;r<4;r++)
    {
        for(int c=0;c<4;c++)
        {
            mat(r,c) = readDouble();
        }
    }

    if (m_istream->rdstate() & m_istream->failbit)
        throw crException("crDataInputStream::readMatrix(): Failed to read crMatrix array.");

    //if (m_verboseOutput) std::cout<<"read/writeMatrix() ["<<mat<<"]"<<std::endl;
    

    return mat;
}

CRCore::crImage* crDataInputStream::readImage(std::string filename)
{
    // If image is already read and in list 
    // then just return pointer to this.
    ImageMap::iterator mitr=m_imageMap.find(filename);    
    if (mitr!=m_imageMap.end()) return mitr->second.get();
        
    // crImage is not in list. 
    // Read it from disk, 
    CRCore::crImage* image = CRIOManager::readImageFile(filename.c_str(),m_options.get());
        
    // add it to the imageList,
    m_imageMap[filename] = image;
    // and return image pointer.

    //if (m_verboseOutput) std::cout<<"read/writeImage() ["<<image<<"]"<<std::endl;
    
    return image;
}

CRCore::crStateSet* crDataInputStream::readStateSet()
{
    // Read statesets unique ID.
    int id = readInt();
    // See if stateset is already in the list.
    StateSetMap::iterator itr= m_statesetMap.find(id);
    if (itr!=m_statesetMap.end()) return itr->second.get();

    // crStateSet is not in list.
    // Create a new stateset,
	CRCore::ref_ptr<CRCore::crStateSet> stateset = new CRCore::crStateSet();

    // read its properties from stream
    ((cre::crStateSet*)(stateset.get()))->read(this);
    
	CRCore::ref_ptr<CRCore::crStateSet> exist_stateset = CRCore::crBrain::getInstance()->checkStateSetExistAndGet(stateset.get());
	if(exist_stateset.valid())
	{
		stateset = exist_stateset;
	}
	else
		CRCore::crBrain::getInstance()->insertStateSet(stateset.get());

    // and add it to the stateset map,
    m_statesetMap[id] = stateset.get();
        

    //if (m_verboseOutput) std::cout<<"read/writeStateSet() ["<<id<<"]"<<std::endl;
    
    return stateset.get();
}

CRCore::crStateAttribute* crDataInputStream::readStateAttribute()
{
    // Read stateattributes unique ID.
    int id = readInt();
    // See if stateattribute is already in the list.
    StateAttributeMap::iterator itr= m_stateAttributeMap.find(id);
    if (itr!=m_stateAttributeMap.end()) return itr->second.get();

    // stateattribute is not in list.
    // Create a new stateattribute,


	CRCore::ref_ptr<CRCore::crStateAttribute> attribute;
    int attributeID = peekInt();
    if(attributeID == CREALPHAFUNC){
        attribute = new CRCore::crAlphaFunc();
        ((cre::crAlphaFunc*)(attribute.get()))->read(this);
    }
    else if(attributeID == CREBLENDFUNC){
        attribute = new CRCore::crBlendFunc();
        ((cre::crBlendFunc*)(attribute.get()))->read(this);
    }
	else if(attributeID == CREDEPTH){
		attribute = new CRCore::crDepth();
		((cre::crDepth*)(attribute.get()))->read(this);
	}
    else if(attributeID == CREMATERIAL){
        attribute = new CRCore::crMaterial();
        ((cre::crMaterial*)(attribute.get()))->read(this);
    }
    else if(attributeID == CRECULLFACE){
        attribute = new CRCore::crCullFace();
        ((cre::crCullFace*)(attribute.get()))->read(this);
    }
    else if(attributeID == CRECLIPPLANE){
        attribute = new CRCore::crClipPlane();
        ((cre::crClipPlane*)(attribute.get()))->read(this);
    }
    else if(attributeID == CREPOLYGONOFFSET){
        attribute = new CRCore::crPolygonOffset();
        ((cre::crPolygonOffset*)(attribute.get()))->read(this);
    }
    else if(attributeID == CREPOLYGONMODE){
        attribute = new CRCore::crPolygonMode();
        ((cre::crPolygonMode*)(attribute.get()))->read(this);
    }
    else if(attributeID == CRESHADEMODEL){
        attribute = new CRCore::crShadeModel();
        ((cre::crShadeModel*)(attribute.get()))->read(this);
    }
    else if(attributeID == CREPOINT){
        attribute = new CRCore::crPoint();
        ((cre::crPoint*)(attribute.get()))->read(this);
    }
    else if(attributeID == CRELINEWIDTH){
        attribute = new CRCore::crLineWidth();
        ((cre::crLineWidth*)(attribute.get()))->read(this);
    }
    else if(attributeID == CRETEXTURE1D){
        attribute = new CRCore::crTexture1D();
        ((cre::crTexture1D*)(attribute.get()))->read(this);
    }
    else if(attributeID == CRETEXTURE2D){
        attribute = new CRCore::crTexture2D();
        ((cre::crTexture2D*)(attribute.get()))->read(this);
		CRCore::crTexture2D *loadedTex2d = dynamic_cast<CRCore::crTexture2D*>(attribute.get());
		const std::string &imageNameID = loadedTex2d->getImageNameID();
		std::string extImageNameID = imageNameID;
		switch(loadedTex2d->getExternTextureID())
		{
		case CRCore::crTexture::EXT_NONE:
			break;
		case CRCore::crTexture::UVSCRAMBLER:
            extImageNameID += "_UVS";
			break;
		case CRCore::crTexture::COLORWEIGHT:
			extImageNameID += "_CW";
			break;
		}
		//if(imageNameID.find("qnyhui") == 0)
		//{
		//	int i = 0;
		//}
		CRCore::crTexture2D *tex2d = CRCore::crBrain::getInstance()->getTexture2D(extImageNameID);
		if(tex2d)
		{
			if(CRCore::crBrain::getInstance()->getNeedLoadImage2D(tex2d))
			{
				if(loadedTex2d->getImage()&&loadedTex2d->getImage()->data())
					tex2d->setImage(loadedTex2d->getImage());
				else
					tex2d->setImage(loadedTex2d->getImageDataRequestHandler()->requestImageFile(imageNameID,tex2d));
			}

			attribute = tex2d;
		}
		else
		{
			if(!loadedTex2d->getImage())
				loadedTex2d->setImage(loadedTex2d->getImageDataRequestHandler()->requestImageFile(imageNameID,loadedTex2d));

			CRCore::crBrain::getInstance()->insertTexture2D(extImageNameID,loadedTex2d);
		}
    }
    else if(attributeID == CRETEXTURE3D){
        attribute = new CRCore::crTexture3D();
        ((cre::crTexture3D*)(attribute.get()))->read(this);
    }
    else if(attributeID == CRETEXTURECUBEMAP){
		attribute = new CRCore::crTextureCubeMap();
		((cre::crTextureCubeMap*)(attribute.get()))->read(this);
		CRCore::crTextureCubeMap *loadedTexCubeMap = dynamic_cast<CRCore::crTextureCubeMap*>(attribute.get());
		const std::string& imageNameID = loadedTexCubeMap->getImageNameID();
		CRCore::crTextureCubeMap *texCubeMap = CRCore::crBrain::getInstance()->getTextureCubeMap(imageNameID);
		bool needReloadImage =false;
		if(texCubeMap)
		{
			if(CRCore::crBrain::getInstance()->getNeedLoadImageCubeMap(texCubeMap))
			{
				for(int face = 0; face < 6; face++)
				{
					if(loadedTexCubeMap->getImage(face)&&loadedTexCubeMap->getImage(face)->data())
				       texCubeMap->setImage(face,loadedTexCubeMap->getImage(face));
					else 
					{
						needReloadImage = true;
						break;
					}
				}
			    if(needReloadImage)
				    texCubeMap->getImageDataRequestHandler()->requestCubeImageFile(imageNameID,texCubeMap);
			}

			attribute = texCubeMap;
		}
		else
		{
			for(int face = 0; face < 6; face++)
			{
				if(!(loadedTexCubeMap->getImage(face)&&loadedTexCubeMap->getImage(face)->data()))
				{
					needReloadImage = true;
					break;
				}
			}
			if(needReloadImage)
			    loadedTexCubeMap->getImageDataRequestHandler()->requestCubeImageFile(imageNameID,loadedTexCubeMap);

			CRCore::crBrain::getInstance()->insertTextureCubeMap(imageNameID,loadedTexCubeMap);
		}
    }
    else if(attributeID == CRETEXENV){
        attribute = new CRCore::crTexEnv();
        ((cre::crTexEnv*)(attribute.get()))->read(this);
    }
    else if(attributeID == CRETEXENVCOMBINE){
        attribute = new CRCore::crTexEnvCombine();
        ((cre::crTexEnvCombine*)(attribute.get()))->read(this);
    }
    else if(attributeID == CRETEXGEN){
        attribute = new CRCore::crTexGen();
        ((cre::crTexGen*)(attribute.get()))->read(this);
    }
    else if(attributeID == CRETEXMAT){
        attribute = new CRCore::crTexMat();
        ((cre::crTexMat*)(attribute.get()))->read(this);
    }
    //else if(attributeID == CREFRAGMENTPROGRAM){
    //    attribute = new CRCore::crFragmentProgram();
    //    ((cre::crFragmentProgram*)(attribute.get()))->read(this);
    //}
    //else if(attributeID == CREVERTEXPROGRAM){
    //    attribute = new CRCore::crVertexProgram();
    //    ((cre::crVertexProgram*)(attribute.get()))->read(this);
    //}
    else if(attributeID == CRELIGHTMODEL){
        attribute = new CRCore::crLightModel();
        ((cre::crLightModel*)(attribute.get()))->read(this);
    }
	else if(attributeID == CRELIGHT){
		attribute = new CRCore::crLight();
		((cre::crLight*)(attribute.get()))->read(this);
	}
    else if(attributeID == CREFRONTFACE){
        attribute = new CRCore::crFrontFace();
        ((cre::crFrontFace*)(attribute.get()))->read(this);
    }
    else if(attributeID == CREPROGRAM){
        attribute = new CRCore::crProgram();
        ((cre::crProgram*)(attribute.get()))->read(this);
    }
	else if(attributeID == CREFOG){
		attribute = new CRCore::crFog();
		((cre::crFog*)(attribute.get()))->read(this);
	}
    else{
        throw crException("Unknown crStateAttribute in crStateSet::read()");
    }
       
    // and add it to the stateattribute map,
    m_stateAttributeMap[id] = attribute.get();
        

    //if (m_verboseOutput) std::cout<<"read/writeStateAttribute() ["<<id<<"]"<<std::endl;
    
    return attribute.get();
}

CRCore::crUniform* crDataInputStream::readUniform()
{
    // Read uniforms unique ID.
    int id = readInt();
    // See if uniform is already in the list.
    UniformMap::iterator itr= m_uniformMap.find(id);
    if (itr!=m_uniformMap.end()) return itr->second.get();

    // crUniform is not in list.
    // Create a new uniform,
    CRCore::crUniform* uniform = new CRCore::crUniform();

    // read its properties from stream
    ((cre::crUniform*)(uniform))->read(this);
        
    // and add it to the uniform map,
    m_uniformMap[id] = uniform;
        

    //if (m_verboseOutput) std::cout<<"read/writeUniform() ["<<id<<"]"<<std::endl;
    
    return uniform;
}


CRCore::crShader* crDataInputStream::readShader()
{
    // Read shaders unique ID.
    int id = readInt();
    // See if shader is already in the list.
    ShaderMap::iterator itr= m_shaderMap.find(id);
    if (itr!=m_shaderMap.end()) return itr->second.get();

    // crShader is not in list.
    // Create a new shader,
    CRCore::crShader* shader = new CRCore::crShader();

    // read its properties from stream
    ((cre::crShader*)(shader))->read(this);
        
    // and add it to the shader map,
    m_shaderMap[id] = shader;
        

    //if (m_verboseOutput) std::cout<<"read/writeShader() ["<<id<<"]"<<std::endl;
    
    return shader;
}

CRCore::crDrawable* crDataInputStream::readDrawable()
{
    // Read stateattributes unique ID.
    int id = readInt();
    // See if stateattribute is already in the list.
    DrawableMap::iterator itr= m_drawableMap.find(id);
    if (itr!=m_drawableMap.end()) return itr->second.get();

    // stateattribute is not in list.
    // Create a new stateattribute,

    int drawableTypeID = peekInt();
    CRCore::crDrawable* drawable = NULL;
    if(drawableTypeID == CREGEOMETRY)
    {
        drawable = new CRCore::crGeometry();                
        ((crGeometry*)(drawable))->read(this);
    }
    else if(drawableTypeID == CRESHAPEDRAWABLE)
    {
        drawable = new CRCore::crShapeDrawable();                
        ((crShapeDrawable*)(drawable))->read(this);
    }
    else if(drawableTypeID == CRETEXT){
        drawable = new CRText::crText();
        ((crText*)(drawable))->read(this);
    }    
 //   else
	//{
	//	std::cout<<"readDrawable error"<<std::endl;
 //       //throw crException("Unknown drawable drawableTypeIDentification in crObject::read()");
	//}

       
    // and add it to the stateattribute map,
	if(drawable)
        m_drawableMap[id] = drawable;
        
    //if (m_verboseOutput) std::cout<<"read/writeDrawable() ["<<id<<"]"<<std::endl;
    
    return drawable;
}

CRCore::crShape* crDataInputStream::readShape()
{
    // Read stateattributes unique ID.
    int id = readInt();
    // See if stateattribute is already in the list.
    ShapeMap::iterator itr= m_shapeMap.find(id);
    if (itr!=m_shapeMap.end()) return itr->second.get();

    // stateattribute is not in list.
    // Create a new stateattribute,

    int shapeTypeID = peekInt();
    CRCore::crShape* shape;
    if(shapeTypeID == CRESPHERE)
    {
        shape = new CRCore::crSphere();                
        ((crSphere*)(shape))->read(this);
    }
    else if(shapeTypeID == CREBOX)
    {
        shape = new CRCore::crBox();
        ((crBox*)(shape))->read(this);
    }
    else if(shapeTypeID == CRECONE)
    {
        shape = new CRCore::crCone();
        ((crCone*)(shape))->read(this);
    }
    else if(shapeTypeID == CRECYLINDER)
    {
        shape = new CRCore::crCylinder();
        ((crCylinder*)(shape))->read(this);
    }
    else if(shapeTypeID == CRECAPSULE)
    {
        shape = new CRCore::crCapsule();
        ((crCapsule*)(shape))->read(this);
    }
    else if(shapeTypeID == CREHEIGHTFIELD)
    {
        shape = new CRCore::crHeightField();                
        ((crHeightField*)(shape))->read(this);
    }
    else
        throw crException("Unknown shape shapeTypeIDentification in crShape::read()");

       
    // and add it to the stateattribute map,
    m_shapeMap[id] = shape;
        

    //if (m_verboseOutput) std::cout<<"read/writeShape() ["<<id<<"]"<<std::endl;
    
    return shape;
}

CRCore::crNode* crDataInputStream::readNode()
{
    // Read node unique ID.
    int id = readInt();
	if(id == CRESCRIPTDATA)
	{
		return NULL;
	}
    // See if node is already in the list.
    NodeMap::iterator itr= m_nodeMap.find(id);
    if (itr!=m_nodeMap.end())
	{
		//std::cout<<"readNode() find ["<<itr->second->getName()<<"] id = "<<id<<std::endl;
		return itr->second.get();
	}

    // stateattribute is not in list.
    // Create a new node,

    CRCore::crNode* node;
    int nodeTypeID= peekInt();
    
    if(nodeTypeID== CREMATRIXTRANSFORM){
        node = new CRCore::crMatrixTransform();
		m_nodeMap[id] = node;
        ((cre::crMatrixTransform*)(node))->read(this);
    }
    else if(nodeTypeID== CREPOSITIONATTITUDETRANSFORM){
        node = new CRCore::crPositionAttitudeTransform();
		m_nodeMap[id] = node;
        ((cre::crPositionAttitudeTransform*)(node))->read(this);
    }
    else if(nodeTypeID== CREDOFTRANSFORM){
        node = new CRCore::crDOFTransform();
		m_nodeMap[id] = node;
        ((cre::crDOFTransform*)(node))->read(this);
    }
	else if(nodeTypeID== CRECAMERAVIEW){
		node = new CRCore::crCameraView();
		m_nodeMap[id] = node;
		((cre::crCameraView*)(node))->read(this);
	}
	else if(nodeTypeID== CRECAMERANODE){
		node = new CRCore::crCameraNode();
		m_nodeMap[id] = node;
		((cre::crCameraNode*)(node))->read(this);
	}
    else if(nodeTypeID== CRETRANSFORM){
        node = new CRCore::crTransform();
		m_nodeMap[id] = node;
        ((cre::crTransform*)(node))->read(this);
    }
    else if(nodeTypeID== CRELIGHTSOURCE){
        node = new CRCore::crLightSource();
		m_nodeMap[id] = node;
        ((cre::crLightSource*)(node))->read(this);
    }
    else if(nodeTypeID== CRETEXGENNODE){
        node = new CRCore::crTexGenNode();
		m_nodeMap[id] = node;
        ((cre::crTexGenNode*)(node))->read(this);
    }
    else if(nodeTypeID== CRECLIPNODE){
        node = new CRCore::crClipNode();
		m_nodeMap[id] = node;
        ((cre::crClipNode*)(node))->read(this);
    }
    else if(nodeTypeID== CRESEQUENCE){
        node = new CRCore::crSequence();
		m_nodeMap[id] = node;
        ((cre::crSequence*)(node))->read(this);
    }
	else if(nodeTypeID== CREMATRIXSEQUENCE){
		node = new CRCore::crMatrixSequence();
		m_nodeMap[id] = node;
		((cre::crMatrixSequence*)(node))->read(this);
	}
    else if(nodeTypeID== CRELOD){
        node = new CRCore::crLod();
		m_nodeMap[id] = node;
        ((cre::crLod*)(node))->read(this);
    }
    else if(nodeTypeID== CREPAGEDLOD){
        node = new CRCore::crPagedLOD();
		m_nodeMap[id] = node;
        ((cre::crPagedLOD*)(node))->read(this);
    }
    else if(nodeTypeID== CRECOORDINATESYSTEMNODE){
        node = new CRCore::crCoordinateSystemNode();
		m_nodeMap[id] = node;
        ((cre::crCoordinateSystemNode*)(node))->read(this);
    }
    else if(nodeTypeID== CRESWITCH){
        node = new CRCore::crSwitch();
		m_nodeMap[id] = node;
        ((cre::crSwitch*)(node))->read(this);
    }
    else if(nodeTypeID== CREMULTISWITCH){
        node = new CRCore::crMultiSwitch();
		m_nodeMap[id] = node;
        ((cre::crMultiSwitch*)(node))->read(this);
    }
  //  else if(nodeTypeID== CREIMPOSTOR){
  //      //node = new CRSim::crImpostor();
		////m_nodeMap[id] = node;
  //      //((cre::crImpostor*)(node))->read(this);
  //  }
    else if(nodeTypeID== CREOCCLUDERNODE){
        node = new CRCore::crOccluderNode();
		m_nodeMap[id] = node;
        ((cre::crOccluderNode*)(node))->read(this);
    }
  //  else if(nodeTypeID== CREVISIBILITYGROUP){
  //      node = new CRSim::crVisibilityGroup();
		//m_nodeMap[id] = node;
  //      ((cre::crVisibilityGroup*)(node))->read(this);
  //  }
    else if(nodeTypeID== CREPROXYNODE){
        node = new CRCore::crProxyNode();
		m_nodeMap[id] = node;
        ((cre::crProxyNode*)(node))->read(this);
    }
	else if(nodeTypeID== CREEXTERNNODE){
		node = new CRCore::crExternNode();
		m_nodeMap[id] = node;
		((cre::crExternNode*)(node))->read(this);
	}
	else if(nodeTypeID== CRETERRAINTILE){
		node = new CRTerrain::crTerrainTile();
		m_nodeMap[id] = node;
		((cre::crTerrainTile*)(node))->read(this);
	}
	else if(nodeTypeID== CREDB){
		node = new CRCore::crDB();
		m_nodeMap[id] = node;
		((cre::crDB*)(node))->read(this);
	}
	else if(nodeTypeID== CRTRAVERSESTRINGNODE){
		node = new CRCore::crTraverseStringNode();
		m_nodeMap[id] = node;
		((cre::crTraverseStringNode*)(node))->read(this);
	}
	else if(nodeTypeID== CRWIDGETNODE){
		node = new CRCore::crWidgetNode();
		m_nodeMap[id] = node;
		((cre::crWidgetNode*)(node))->read(this);
	}
	else if(nodeTypeID== CRCANVASNODE){
		node = new CRCore::crCanvasNode();
		m_nodeMap[id] = node;
		((cre::crCanvasNode*)(node))->read(this);
		if(CRCore::crDisplaySettings::instance()->getRunMode() > 0)
			node->setVisiable(false);
		//CRCore::crFilterRenderManager::getInstance()->addCanvas((CRCore::crCanvasNode*)node);
		//CRUtil::crOptimizer::crTextureVisitor tv(true,true, // unref image 
		//	false,false, // client storage
		//	true,1.0, // anisotropic filtering
		//	NULL );
		//tv.setChangeFilter(CRCore::crTexture::LINEAR,CRCore::crTexture::LINEAR);
		//node->accept(tv);
		//CRCore::crFilterRenderManager::getInstance()->showCanvas((CRCore::crCanvasNode*)node,true);
	}
	else if(nodeTypeID== CRBUTTONWIDGET){
		node = new CRUI::crButtonWidgetNode();
		m_nodeMap[id] = node;
		((cre::crButtonWidgetNode*)(node))->read(this);
	}
	else if(nodeTypeID== CRSCROLLBARWIDGET){
		node = new CRUI::crScrollBarWidgetNode();
		m_nodeMap[id] = node;
		((cre::crScrollBarWidgetNode*)(node))->read(this);
	}
	else if(nodeTypeID== CRSTATICTEXTWIDGET){
		node = new CRUI::crStaticTextWidgetNode();
		m_nodeMap[id] = node;
		((cre::crStaticTextWidgetNode*)(node))->read(this);
	}
	else if(nodeTypeID== CRHYPERTEXTWIDGET){
		node = new CRUI::crHypertextWidgetNode();
		m_nodeMap[id] = node;
		((cre::crHypertextWidgetNode*)(node))->read(this);
	}
	else if(nodeTypeID== CREDITWIDGET){
		node = new CRUI::crEditWidgetNode();
		m_nodeMap[id] = node;
		((cre::crEditWidgetNode*)(node))->read(this);
	}
	else if(nodeTypeID== CRELISTBOXWIDGET){
		node = new CRUI::crListBoxWidgetNode();
		m_nodeMap[id] = node;
		((cre::crListBoxWidgetNode*)(node))->read(this);
	}
	else if(nodeTypeID== CREIMAGEBOXWIDGET){
		node = new CRUI::crImageBoxWidgetNode();
		m_nodeMap[id] = node;
		((cre::crImageBoxWidgetNode*)(node))->read(this);
	}
	else if(nodeTypeID== CRECHECKBOXWIDGET){
		node = new CRUI::crCheckBoxWidgetNode();
		m_nodeMap[id] = node;
		((cre::crCheckBoxWidgetNode*)(node))->read(this);
	}
	else if(nodeTypeID== CRERADIOWIDGET){
		node = new CRUI::crRadioWidgetNode();
		m_nodeMap[id] = node;
		((cre::crRadioWidgetNode*)(node))->read(this);
	}
	else if(nodeTypeID== CRERADIOGROUPWIDGET){
		node = new CRUI::crRadioGroupWidgetNode();
		m_nodeMap[id] = node;
		((cre::crRadioGroupWidgetNode*)(node))->read(this);
	}
	else if(nodeTypeID== CREPROGRESSWIDGET){
		node = new CRUI::crProgressWidgetNode();
		m_nodeMap[id] = node;
		((cre::crProgressWidgetNode*)(node))->read(this);
	}
	else if(nodeTypeID== CRESLIDERWIDGET){
		node = new CRUI::crSliderWidgetNode();
		m_nodeMap[id] = node;
		((cre::crSliderWidgetNode*)(node))->read(this);
	}
	else if(nodeTypeID== CRECOMBOBOXWIDGET){
		node = new CRUI::crComboBoxWidgetNode();
		m_nodeMap[id] = node;
		((cre::crComboBoxWidgetNode*)(node))->read(this);
	}
	else if(nodeTypeID== CRECHOOSECOLORWIDGET){
		node = new CRUI::crChooseColorWidgetNode();
		m_nodeMap[id] = node;
		((cre::crChooseColorWidgetNode*)(node))->read(this);
	}
	else if(nodeTypeID== CRETABLEWIDGET){
		node = new CRUI::crTableWidgetNode();
		m_nodeMap[id] = node;
		((cre::crTableWidgetNode*)(node))->read(this);
	}
	else if(nodeTypeID== CRELISTCONTROLWIDGET){
		node = new CRUI::crListControlWidgetNode();
		m_nodeMap[id] = node;
		((cre::crListControlWidgetNode*)(node))->read(this);
	}
	else if(nodeTypeID== CRESCISSORWIDGET){
		node = new CRUI::crScissorWidgetNode();
		m_nodeMap[id] = node;
		((cre::crScissorWidgetNode*)(node))->read(this);
	}
	else if(nodeTypeID== CREPARTICLEEFFECT){
		node = new CRParticle::crParticleEffect();
		m_nodeMap[id] = node;
		((cre::crParticleEffect*)(node))->read(this);
	}
    else if(nodeTypeID== CREGROUP){
        node = new CRCore::crGroup();
		m_nodeMap[id] = node;
        ((cre::crGroup*)(node))->read(this);
    }
    else if(nodeTypeID== CREBILLBOARD){
        node = new CRCore::crBillboard();
		m_nodeMap[id] = node;
        ((cre::crBillboard*)(node))->read(this);
    }
	//else if(nodeTypeID== CREDEPTHSORTEDOBJECT){
	//	node = new CRCore::crDepthSortedObject();
	//	m_nodeMap[id] = node;
	//	((cre::crDepthSortedObject*)(node))->read(this);
	//}
    else if(nodeTypeID== CREOBJECT){
        node = new CRCore::crObject();
		m_nodeMap[id] = node;
        ((cre::crObject*)(node))->read(this);
    }
  //  else if(nodeTypeID== CRELIGHTPOINTNODE){
  //      node = new CRSim::crLightPointNode();
		//m_nodeMap[id] = node;
  //      ((cre::crLightPointNode*)(node))->read(this);
  //  }
    //else if(nodeTypeID== CREMULTITEXTURECONTROL){
    //    node = new CRFX::crMultiTextureControl();
    //    ((cre::crMultiTextureControl*)(node))->read(this);
    //}
	else if(nodeTypeID== CREMATTERGROUP){
		node = new CRPhysics::crMatterGroup();
		m_nodeMap[id] = node;
		((cre::crMatterGroup*)(node))->read(this);
	}
	//else if(nodeTypeID== CREBULLETMATTEROBJECT){
	//	node = new CRPhysics::crBulletMatterObject();
	//	m_nodeMap[id] = node;
	//	((cre::crBulletMatterObject*)(node))->read(this);
	//}
	////else if(nodeTypeID== CREWEAPONMATTEROBJECT){
	////	node = new CRPhysics::crWeaponMatterObject();
	////	m_nodeMap[id] = node;
	////	((cre::crWeaponMatterObject*)(node))->read(this);
	////}
	else if(nodeTypeID== CREBOXMATTEROBJECT){
		node = new CRPhysics::crBoxMatterObject();
		m_nodeMap[id] = node;
		((cre::crBoxMatterObject*)(node))->read(this);
	}
	else if(nodeTypeID== CRESPHEREMATTEROBJECT){
		node = new CRPhysics::crSphereMatterObject();
		m_nodeMap[id] = node;
		((cre::crSphereMatterObject*)(node))->read(this);
	}
	else if(nodeTypeID== CRECCYLINDERMATTEROBJECT){
		node = new CRPhysics::crCCylinderMatterObject();
		m_nodeMap[id] = node;
		((cre::crCCylinderMatterObject*)(node))->read(this);
	}
	else if(nodeTypeID== CREMESHMATTEROBJECT){
		node = new CRPhysics::crMeshMatterObject();
		m_nodeMap[id] = node;
		((cre::crMeshMatterObject*)(node))->read(this);
	}
	//else if(nodeTypeID== CRERAYMATTEROBJECT){
	//	node = new CRPhysics::crRayMatterObject();
	//	m_nodeMap[id] = node;
	//	((cre::crRayMatterObject*)(node))->read(this);
	//}
	//else if(nodeTypeID== CREWEAPONNODE){
	//	node = new CRPhysics::crWeaponNode();
	//	m_nodeMap[id] = node;
	//	((cre::crWeaponNode*)(node))->read(this);
	//}
	else if(nodeTypeID== CREEVENTGROUP){
		node = new CRCore::crEventGroup();
		m_nodeMap[id] = node;
		((cre::crEventGroup*)(node))->read(this);
	}
	//else if(nodeTypeID== CRENERVECELLGROUP){
	//	node = new CRAI::crNerveCellGroup();
	//	m_nodeMap[id] = node;
	//	((cre::crNerveCellGroup*)(node))->read(this);
	//}
	//else if(nodeTypeID== CREAIGROUP){
	//	node = new CRAI::crAIGroup();
	//	m_nodeMap[id] = node;
	//	((cre::crAIGroup*)(node))->read(this);
	//}
	//else if(nodeTypeID== CREKEYBOARDMOUSEEVENTGROUP){
	//	node = new CRAI::crKeyboardMouseEventGroup();
	//	m_nodeMap[id] = node;
	//	((cre::crKeyboardMouseEventGroup*)(node))->read(this);
	//}
	//else if(nodeTypeID== CRESOUNDNODE){
	//	node = new CRAL::crSoundNode();
	//	m_nodeMap[id] = node;
	//	((cre::crSoundNode*)(node))->read(this);
	//}
    else{
        //throw crException("Unknown node identification in crDataInputStream::readNode()");
		//std::cout<<"Unknown node identification in crDataInputStream::readNode()"<<std::endl;
		return NULL;
    }
        
    //if (m_verboseOutput) std::cout<<"read/writeNode() ["<<id<<"]"<<std::endl;
	//std::cout<<"readNode() ["<<node->getName()<<"] id = "<<id<<std::endl;
    
    return node;
}

CRCore::Condition* crDataInputStream::readCondition()
{
	// Read crEventMsg unique ID.
	int id = readInt();
	// See if crEventMsg is already in the list.
	ConditionMap::iterator itr= m_conditionMap.find(id);
	if (itr!=m_conditionMap.end()) return itr->second.get();

	// stateattribute is not in list.
	// Create a new stateattribute,

	int conditionTypeID = peekInt();
	CRCore::Condition* condition;
	if(conditionTypeID == CRECONDITION)
	{
		condition = new CRCore::Condition();                
		((Condition*)(condition))->read(this);
	}
	else
		throw crException("Unknown shape shapeTypeIDentification in crShape::read()");


	// and add it to the stateattribute map,
	m_conditionMap[id] = condition;

	//if (m_verboseOutput) std::cout<<"read/readCondition() ["<<id<<"]"<<std::endl;

	return condition;
}

CRCore::SendMsg* crDataInputStream::readSendMsg()
{
	// Read crEventMsg unique ID.
	int id = readInt();
	// See if crEventMsg is already in the list.
	SendMsgMap::iterator itr= m_sendMsgMap.find(id);
	if (itr!=m_sendMsgMap.end()) return itr->second.get();

	// stateattribute is not in list.
	// Create a new stateattribute,

	int sendMsgTypeID = peekInt();
	CRCore::SendMsg* sendMsg;
	if(sendMsgTypeID == CRESENDMSG)
	{
		sendMsg = new CRCore::SendMsg();                
		((SendMsg*)(sendMsg))->read(this);
	}
	else
		throw crException("Unknown shape sendMsgTypeIDentification in SendMsg::read()");


	// and add it to the stateattribute map,
	m_sendMsgMap[id] = sendMsg;

	//if (m_verboseOutput) std::cout<<"read/readSendMsg() ["<<id<<"]"<<std::endl;

	return sendMsg;
}

CRCore::ECondition* crDataInputStream::readECondition()
{
	// Read crEventMsg unique ID.
	int id = readInt();
	// See if crEventMsg is already in the list.
	EConditionMap::iterator itr= m_econditionMap.find(id);
	if (itr!=m_econditionMap.end()) return itr->second.get();

	// stateattribute is not in list.
	// Create a new stateattribute,

	int econditionTypeID = peekInt();
	CRCore::ECondition* econdition;
	if(econditionTypeID == CREECONDITION)
	{
		econdition = new CRCore::ECondition();                
		((ECondition*)(econdition))->read(this);
	}
	else
		throw crException("Unknown shape econditionTypeIDentification in ECondition::read()");


	// and add it to the stateattribute map,
	m_econditionMap[id] = econdition;

	//if (m_verboseOutput) std::cout<<"read/readECondition() ["<<id<<"]"<<std::endl;

	return econdition;
}

CRCore::crEvent* crDataInputStream::readEvent()
{
	// Read crEventMsg unique ID.
	int id = readInt();
	// See if crEventMsg is already in the list.
	EventMap::iterator itr= m_eventMap.find(id);
	if (itr!=m_eventMap.end()) return itr->second.get();

	// stateattribute is not in list.
	// Create a new stateattribute,

	int eventTypeID = peekInt();
	CRCore::crEvent* event;
	if(eventTypeID == CREEVENT)
	{
		event = new CRCore::crEvent();                
		((crEvent*)(event))->read(this);
	}
	else
		throw crException("Unknown shape eventTypeIDentification in crEvent::read()");


	// and add it to the stateattribute map,
	m_eventMap[id] = event;

	//if (m_verboseOutput) std::cout<<"read/writeEvent() ["<<id<<"]"<<std::endl;

	return event;
}

CRCore::crEventMsg* crDataInputStream::readEventMsg()
{
	// Read crEventMsg unique ID.
	int id = readInt();
	// See if crEventMsg is already in the list.
	EventMsgMap::iterator itr= m_eventMsgMap.find(id);
	if (itr!=m_eventMsgMap.end()) return itr->second.get();

	// stateattribute is not in list.
	// Create a new stateattribute,

	int eventMsgTypeID = peekInt();
	CRCore::crEventMsg* eventMsg;
	if(eventMsgTypeID == CREEVENTMSG)
	{
		eventMsg = new CRCore::crEventMsg();                
		((crEventMsg*)(eventMsg))->read(this);
	}
	else if(eventMsgTypeID == CREIFELSEEVENTMSG)
	{
		eventMsg = new CRCore::crIF_ELSE_EventMsg();
		((crIF_ELSE_EventMsg*)(eventMsg))->read(this);
	}
	else if(eventMsgTypeID == CRESWITCHEVENTMSG)
	{
		eventMsg = new CRCore::crSwitch_EventMsg();
		((crSwitch_EventMsg*)(eventMsg))->read(this);
	}
	else if(eventMsgTypeID == CREWHILEEVENTMSG)
	{
		eventMsg = new CRCore::crWhile_EventMsg();
		((crWhile_EventMsg*)(eventMsg))->read(this);
	}
	else
		throw crException("Unknown shape eventMsgTypeIDentification in crEventMsg::read()");


	// and add it to the stateattribute map,
	m_eventMsgMap[id] = eventMsg;


	//if (m_verboseOutput) std::cout<<"read/writeEventMsg() ["<<id<<"]"<<std::endl;

	return eventMsg;
}

//CRCore::ParseParamCallback* crDataInputStream::readPPC()
//{
//	// Read crEventMsg unique ID.
//	int id = readInt();
//	// See if crEventMsg is already in the list.
//	PPCMap::iterator itr= m_ppcMap.find(id);
//	if (itr!=m_ppcMap.end()) return itr->second.get();
//
//	// stateattribute is not in list.
//	// Create a new stateattribute,
//
//	int ppcTypeID = peekInt();
//	CRCore::ParseParamCallback* ppc;
///*	if(ppcTypeID == CREPPC)
//	{
//		ppc = new CRCore::ParseParamCallback();                
//		((ParseParamCallback*)(ppc))->read(this);
//	}
//	//else *///if(ppcTypeID == CRESETFORCEANDTORQUEPPC)
//	//{
//	//	ppc = new CRPhysics::crSetForceAndTorquePPC();
//	//	((crSetForceAndTorquePPC*)(ppc))->read(this);
//	//}
//	//else if(ppcTypeID == CRESETGRAVITYMODEPPC)
//	//{
//	//	ppc = new CRPhysics::crSetGravityModePPC();
//	//	((crSetGravityModePPC*)(ppc))->read(this);
//	//}
//	//else if(ppcTypeID == CREJOINTSETPARAMPPC)
//	//{
//	//	ppc = new CRPhysics::crJointSetParamPPC();
//	//	((crJointSetParamPPC*)(ppc))->read(this);
//	//}
//	//else if(ppcTypeID == CREPHYSICSRESETPPC)
//	//{
//	//	ppc = new CRPhysics::crPhysicsResetPPC();
//	//	((crPhysicsResetPPC*)(ppc))->read(this);
//	//}
//	//else if(ppcTypeID == CRESETGEOMORBODYINFOTPPC)
//	//{
//	//	ppc = new CRPhysics::crSetGeomorBodyInfoPPC();
//	//	((crSetGeomorBodyInfoPPC*)(ppc))->read(this);
//	//}
//	//else if(ppcTypeID == CREGENERALPARAMTRANSFERPPC)
//	//{
//	//	ppc = new CRPhysics::crGeneralParamTransferPPC();
//	//	((crGeneralParamTransferPPC*)(ppc))->read(this);
//	//}
//	//else if(ppcTypeID == CREFXEVENTPPC)
//	//{
//	//	ppc = new CRPhysics::crFXEventPPC();
//	//	((crFXEventPPC*)(ppc))->read(this);
//	//}
//	//else if(ppcTypeID == CREKEYBOARDMOUSEPPC)
//	//{
//	//	ppc = new CRAI::crKeyboardMousePPC();
//	//	((crKeyboardMousePPC*)(ppc))->read(this);
//	//}
//	//else if(ppcTypeID == CRECARMANIPULATORPPC)
//	//{
//	//	ppc = new CRAI::crCarManipulatorPPC();
//	//	((crCarManipulatorPPC*)(ppc))->read(this);
//	//}
//	//else if(ppcTypeID == CREROBOTMANIPULATORPPC)
//	//{
//	//	ppc = new CRAI::crRobotManipulatorPPC();
//	//	((crRobotManipulatorPPC*)(ppc))->read(this);
//	//}
//	//else if(ppcTypeID == CRECHARACTERMANIPULATORPPC)
//	//{
//	//	ppc = new CRAI::crCharacterManipulatorPPC();
//	//	((crCharacterManipulatorPPC*)(ppc))->read(this);
//	//}
//	//else if(ppcTypeID == CRERADARCOLLIDEPPC)
//	//{
//	//	ppc = new CRAI::crRadarCollidePPC();
//	//	((crRadarCollidePPC*)(ppc))->read(this);
//	//}
//	//else if(ppcTypeID == CREDISCOVERAIPPC)
//	//{
//	//	ppc = new CRAI::crDiscoverAIPPC();
//	//	((crDiscoverAIPPC*)(ppc))->read(this);
//	//}
//	//else if(ppcTypeID == CRECOLLIDEPPC)
//	//{
//	//	ppc = new CRAI::crCollidePPC();
//	//	((crCollidePPC*)(ppc))->read(this);
//	//}
//	else if(ppcTypeID == CREWEAPONEVENTPPC)
//	{
//		ppc = new CRPhysics::crWeaponEventPPC();
//		((crWeaponEventPPC*)(ppc))->read(this);
//	}
//	else
//		throw crException("Unknown ppc ppcTypeIDentification in ParseParamCallback::read()");
//
//
//	// and add it to the stateattribute map,
//	m_ppcMap[id] = ppc;
//
//
//	if (m_verboseOutput) std::cout<<"read/writePPC() ["<<id<<"]"<<std::endl;
//
//	return ppc;
//}
//
CRPhysics::crJointParameter* crDataInputStream::readJointParam()
{
	// Read crEventMsg unique ID.
	int id = readInt();
	// See if crEventMsg is already in the list.
	JointParamMap::iterator itr= m_jointParamMap.find(id);
	if (itr!=m_jointParamMap.end()) return itr->second.get();

	// stateattribute is not in list.
	// Create a new stateattribute,

	int jointTypeID = peekInt();
	CRPhysics::crJointParameter* jointParam;
	if(jointTypeID == CREJOINTPARAMETER)
	{
		jointParam = new CRPhysics::crJointParameter();
		((crJointParameter*)(jointParam))->read(this);
	}
	else
		throw crException("Unknown crJointParameter jointTypeIDentification in crJointParameter::read()");


	// and add it to the stateattribute map,
	m_jointParamMap[id] = jointParam;

	//if (m_verboseOutput) std::cout<<"read/readJointParam() ["<<id<<"]"<<std::endl;

	return jointParam;
}

CRPhysics::crJoint* crDataInputStream::readJoint()
{
	// Read crEventMsg unique ID.
	int id = readInt();
	// See if crEventMsg is already in the list.
	JointMap::iterator itr= m_jointMap.find(id);
	if (itr!=m_jointMap.end()) return itr->second.get();

	// stateattribute is not in list.
	// Create a new stateattribute,

	int jointTypeID = peekInt();
	CRPhysics::crJoint* joint;
/*	if(jointTypeID == CREJOINT)
	{
		joint = new CRPhysics::crJoint();                
		((crJoint*)(joint))->read(this);
	}
	else*/ if(jointTypeID == CREJOINTBALL)
	{
		joint = new CRPhysics::crJointBall();
		((crJointBall*)(joint))->read(this);
	}
	else if(jointTypeID == CREJOINTHINGE)
	{
		joint = new CRPhysics::crJointHinge();
		((crJointHinge*)(joint))->read(this);
	}
	else if(jointTypeID == CREJOINTSLIDER)
	{
		joint = new CRPhysics::crJointSlider();
		((crJointSlider*)(joint))->read(this);
	}
	else if(jointTypeID == CREJOINTUNIVERSAL)
	{
		joint = new CRPhysics::crJointUniversal();
		((crJointUniversal*)(joint))->read(this);
	}
	else if(jointTypeID == CREJOINTCONTACT)
	{
		joint = new CRPhysics::crJointContact();
		((crJointContact*)(joint))->read(this);
	}
	else if(jointTypeID == CREJOINTHINGE2)
	{
		joint = new CRPhysics::crJointHinge2();
		((crJointHinge2*)(joint))->read(this);
	}
	else if(jointTypeID == CREJOINTFIXED)
	{
		joint = new CRPhysics::crJointFixed();
		((crJointFixed*)(joint))->read(this);
	}
	else if(jointTypeID == CREJOINTAMOTOR)
	{
		joint = new CRPhysics::crJointAMotor();
		((crJointAMotor*)(joint))->read(this);
	}
	else if(jointTypeID == CREJOINTNULL)
	{
		joint = new CRPhysics::crJointNull();
		((crJointNull*)(joint))->read(this);
	}
	else
		throw crException("Unknown crJoint jointTypeIDentification in readJoint::read()");


	// and add it to the stateattribute map,
	m_jointMap[id] = joint;

	//if (m_verboseOutput) std::cout<<"read/readJoint() ["<<id<<"]"<<std::endl;

	return joint;
}

//CRAL::crSoundState* crDataInputStream::readSoundState()
//{
//	int id = readInt();
//
//	SoundStateMap::iterator itr= m_soundStateMap.find(id);
//	if (itr!=m_soundStateMap.end()) return itr->second.get();
//
//	int soundStateID = peekInt();
//	CRAL::crSoundState* soundState;
//    if(soundStateID == CRESOUNDSTATE)
//	{
//		soundState = new CRAL::crSoundState();
//		((crSoundState*)(soundState))->read(this);
//	}
//	else
//		throw crException("Unknown crSoundState TypeIDentification in readSoundState::read()");
//
//	m_soundStateMap[id] = soundState;
//
//	//if (m_verboseOutput) std::cout<<"read/readSoundState() ["<<id<<"]"<<std::endl;
//
//	return soundState;
//}

//CRAL::crOccludeCallback* crDataInputStream::readSoundOccludeCallback()
//{
//	int id = readInt();
//
//	SoundOccludeCallbackMap::iterator itr= m_soundOccludeCallbackMap.find(id);
//	if (itr!=m_soundOccludeCallbackMap.end()) return itr->second.get();
//
//	int ID = peekInt();
//	CRAL::crOccludeCallback* occ;
//	if(ID == CRESOUNDOCCLUDECALLBACK)
//	{
//		occ = new CRAL::crOccludeCallback();
//		((crSoundOccludeCallback*)(occ))->read(this);
//	}
//	else
//		throw crException("Unknown crOccludeCallback TypeIDentification in readSoundOccludeCallback::read()");
//
//	m_soundOccludeCallbackMap[id] = occ;
//
//	if (m_verboseOutput) std::cout<<"read/readSoundOccludeCallback() ["<<id<<"]"<<std::endl;
//
//	return occ;
//}

CRCore::crCollideCallback* crDataInputStream::readCollideCallback()
{
	int id = readInt();

	CollideCallbackMap::iterator itr= m_collideCallbackMap.find(id);
	if (itr!=m_collideCallbackMap.end()) return itr->second.get();

	int ID = peekInt();
	CRCore::crCollideCallback* temp;
	if(ID == CRECOLLIDECALLBACK)
	{
		temp = new CRCore::crCollideCallback();
		((crCollideCallback*)(temp))->read(this);
	}
	//else if(ID == CRESOUNDCOLLIDECALLBACK)
	//{
	//	temp = new CRAL::crSoundCollideCallback();
	//	((crSoundCollideCallback*)(temp))->read(this);
	//}
	else
		throw crException("Unknown crCollideCallback TypeIDentification in readCollideCallback::read()");

	m_collideCallbackMap[id] = temp;

	//if (m_verboseOutput) std::cout<<"read/readCollideCallback() ["<<id<<"]"<<std::endl;

	return temp;
}

