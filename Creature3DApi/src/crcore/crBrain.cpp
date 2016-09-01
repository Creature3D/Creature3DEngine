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

#include <CRCore/crBrain.h>
#include <CRCore/crProgram.h>
#include <CRCore/crShader.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crEventGroup.h>
#include <CRCore/crDisplaySettings.h>
#include <CRCore/crNotify.h>
#include <CRCore/crStreamBuf.h>
#include <CRCore/crIntersectVisitor.h>
#include <crgl/GL2Extensions.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRCore/crBlockDetectThread.h>
using namespace CRCore;
using namespace std;

ref_ptr<crBrain> crBrain::m_pBrain = NULL;

crBrain::crBrain()
{
	//m_pBrain = this;

	//if(!m_resource.valid()) 
	//	m_resource = new crResource;

	//if(!m_aiTreeHead.valid()) 
	//	m_aiTreeHead = new crAITreeHead;

	//m_globalLightNum = 0;
	m_currentActiveDB = 0;
	m_gamePaused = false;
	m_resumeGame = true;

	//m_disablePagedLod = false;
	//m_disableVSM = false;
	//m_disableSoftShadow = false;
	//m_disableParallax = false;
	//m_disableNormalMapping = false;
	//m_disableFog = false;

	m_clip.set(1.0,1e5);
	m_brightness = 0.0f;
	m_fog = 0;
	m_fogParams.set(0.0f,1.0f,10.0f,100.0f);
	m_maxDepth = 500.0f;
	m_dofBlurStart = 0.2f;
	m_dofBlurFactor = 0.003f;
	m_motionBlurFactor = 0.1f;

 //   m_gamePausedBarrier = new CRCore::crBarrier(2);
	m_cullThreadPaused = false;
	m_drawThreadPaused = false;

	m_upperSkyColor.set(0.2f,0.2f,0.2f);
	m_lowerSkyColor.set(0.0f,0.0f,0.0f);

	m_worldSize.set(256,256,200);
	m_mapSize.set(512,512);
	m_calcStaticMeshShadow = true;
	m_updtimer = 0.0f;
}

crBrain::~crBrain()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crBrain()"<< std::endl;
	//delete m_gamePausedBarrier; 
}

crBrain * crBrain::getInstance()
{
  //if (m_pBrain==NULL) 
  //  return new crBrain();
  //return crBrain::m_pBrain.get();
	if (m_pBrain==NULL) 
	{
		m_pBrain = new crBrain;
	}
	return m_pBrain.get();
}

void crBrain::accept(crNodeVisitor& nv)
{
	for( DBArray::iterator itr = m_dbArray.begin();
		itr != m_dbArray.end();
		itr++)
	{
		(*itr)->accept(nv);
	}
}
void crBrain::releaseObjects(crState *state)
{
	m_texture2dmap.clear();
	m_texture1dmap.clear();
	m_texture3dmap.clear();
	m_textureCubeMapMap.clear();
	ref_ptr<CRCore::crStateSet> ss;
	for( StateSetSet::iterator itr = m_statesetSet.begin();
		 itr != m_statesetSet.end();
		 ++itr )
	{
		ss = *itr;
		ss->releaseObjects(state);
	}
	m_statesetSet.clear();
}
void crBrain::clear()
{
	//CRCore::ScopedLock<crMutex> lock(m_memFileMutex);
	//m_keyboardMouseNode = NULL;
	//m_aiTreeHead = NULL;
	CRCore::ScopedLock<crCriticalMutex> lock(m_managerInstanceVecMutex);
	for( ManagerInstanceVec::iterator itr = m_managerInstanceVec.begin();
         itr != m_managerInstanceVec.end(); )
	{
		(*itr)->clear();
        itr = m_managerInstanceVec.erase(itr);
	}
	//CRCore::notify(CRCore::ALWAYS)<<"m_managerInstanceVec = "<<m_managerInstanceVec.size<<std::endl;
	//m_managerInstanceVec.clear();
	//crEventGroup::clearRelayMsgBuffer();
	//m_initRenderVecBuf.clear();
	crIntersectVisitor::clearStaticAvoidIntersectNode();
	m_dbArray.clear();

	////////////////////
	//m_pBrain = NULL;
	m_texture2dmap.clear();
	m_texture1dmap.clear();
	m_texture3dmap.clear();
	m_textureCubeMapMap.clear();
	m_statesetSet.clear();
	m_initLoadPage.clear();
	m_objectLightMap.clear();
	//{
	//	CRCore::ScopedLock<crMutex> lock(m_memFileMutex);
	//	m_memFileMap.clear();
	//}
	{
		CRCore::ScopedLock<crMutex> lock(m_leadFailedDequeMutex);
		m_loadFailedDeque.clear();
	}
	char gbuf[256];
	sprintf(gbuf,"crBrain::clear()\n\0");
	gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
}

void crBrain::pushInstance(Referenced *instance)
{
	CRCore::ScopedLock<crCriticalMutex> lock(m_managerInstanceVecMutex);
    m_managerInstanceVec.push_back(instance);
}

void crBrain::prune()
{
	///////////////////////////
	//typedef std::vector<CRCore::crStateSet *> EraseVec2;
	//EraseVec2 eraseVec2;
	CRCore::ref_ptr<CRCore::crStateSet> setItr;
	for( StateSetSet::iterator itr = m_statesetSet.begin();
		itr != m_statesetSet.end();
		 )
	{
		setItr = *itr;
		if(setItr->referenceCount()==1) 
		{
			//eraseVec2.push_back(setItr.get());
			itr = m_statesetSet.erase(itr);
		}
		else
		{
			++itr;
		}
	}

	//for( EraseVec2::iterator itr = eraseVec2.begin();
	//	itr != eraseVec2.end();
	//	++itr )
	//{
	//	for( StateSetSet::iterator sitr = m_statesetSet.begin();
	//		 sitr != m_statesetSet.end();
	//		 ++sitr )
	//	{
	//		if(*sitr == *itr)
	//		{
	//			m_statesetSet.erase(sitr);
	//			break;
	//		}
	//	}
	//}
	//eraseVec2.clear();
	///////////////////////////
	//typedef std::vector<std::string> EraseVec;
	//EraseVec eraseVec;

	for( Texture2DMap::iterator itr = m_texture2dmap.begin();
		 itr != m_texture2dmap.end();
		 )
	{
        if(itr->second->referenceCount()==1) 
		{
			//eraseVec.push_back(itr->first);
			itr = m_texture2dmap.erase(itr);
		}
		else
		{
			++itr;
		}
	}
	//for( EraseVec::iterator itr = eraseVec.begin();
	//	itr != eraseVec.end();
	//	++itr )
	//{
 //       m_texture2dmap.erase(*itr);
	//}
	//eraseVec.clear();
////////////////////
	for( Texture1DMap::iterator itr = m_texture1dmap.begin();
		itr != m_texture1dmap.end();
		)
	{
		if(itr->second->referenceCount()==1)
		{
			//eraseVec.push_back(itr->first);
			itr = m_texture1dmap.erase(itr);
		}
		else
		{
			++itr;
		}
	}
	//for( EraseVec::iterator itr = eraseVec.begin();
	//	itr != eraseVec.end();
	//	++itr )
	//{
	//	m_texture1dmap.erase(*itr);
	//}
	//eraseVec.clear();
///////////////////////////
	for( Texture3DMap::iterator itr = m_texture3dmap.begin();
		itr != m_texture3dmap.end();
		)
	{
		if(itr->second->referenceCount()==1)
		{
			//eraseVec.push_back(itr->first);
			itr = m_texture3dmap.erase(itr);
		}
		else
		{
			++itr;
		}
	}
	//for( EraseVec::iterator itr = eraseVec.begin();
	//	itr != eraseVec.end();
	//	++itr )
	//{
	//	m_texture3dmap.erase(*itr);
	//}
	//eraseVec.clear();
///////////////////////////
	for( TextureCubeMapMap::iterator itr = m_textureCubeMapMap.begin();
		itr != m_textureCubeMapMap.end();
		)
	{
		if(itr->second->referenceCount()==1)
		{
			//eraseVec.push_back(itr->first);
			itr = m_textureCubeMapMap.erase(itr);
		}
		else
		{
			++itr;
		}
	}
	//for( EraseVec::iterator itr = eraseVec.begin();
	//	itr != eraseVec.end();
	//	++itr )
	//{
	//	m_textureCubeMapMap.erase(*itr);
	//}
	//eraseVec.clear();
}

void crBrain::insertDescription( const std::string &str )
{
    m_description += str;
}

crDB* crBrain::getCurrentActiveDB()
{
	return m_dbArray[m_currentActiveDB].get();
}

void crBrain::setCurrentActiveDB(unsigned int id)
{
	if(id>=0&&id<m_dbArray.size())
	    m_currentActiveDB = id;
}

//void crBrain::attachKeyboardMouseNode(crNode *keyboardMouseNode)
//{ 
//	m_keyboardMouseNode = keyboardMouseNode; 
//}

//void crBrain::updateKeyboardMouse(crNodeVisitor &updvisitor)
//{ 
//	if(m_keyboardMouseNode.valid())
//		m_keyboardMouseNode->accept(updvisitor); 
//}
//////////////////texture 2D
crBrain::Texture2DMap &crBrain::getTexture2DMap() 
{
	//CRCore::notify(CRCore::ALWAYS)<<"m_texture2dmap = "<<m_texture2dmap.size()<< std::endl;
	return m_texture2dmap; 
}

bool crBrain::getNeedLoadImage2D(const std::string &imageName)
{
    crTexture2D *tex2d = getTexture2D(imageName);
	if(!tex2d) return true;
	return tex2d->getNeedCompile()&&!tex2d->getImage();
}

bool crBrain::getNeedLoadImage2D(const crTexture2D *tex2d)
{
	return tex2d->getNeedCompile()&&!tex2d->getImage();
}

void crBrain::insertTexture2D(const std::string &imageName,crTexture2D *tex2d)
{
	//CRCore::notify(CRCore::ALWAYS)<<"insertTexture2D = "<<imageName.c_str()<< std::endl;
    m_texture2dmap[imageName] = tex2d;
}

crTexture2D *crBrain::getTexture2D(const std::string &imageName)
{
	Texture2DMap::iterator itr = m_texture2dmap.find(imageName);
	//if(itr == m_texture2dmap.end())
	//{
	//	CRCore::notify(CRCore::ALWAYS)<<"getTexture2D = "<<imageName.c_str()<< std::endl;
	//}
	return itr != m_texture2dmap.end() ? itr->second.get() : NULL;
}

void crBrain::removeTexture2D(const std::string &imageName)
{
    m_texture2dmap.erase(imageName);
}

void crBrain::clearTexture2DMap()
{
    m_texture2dmap.clear();
}

/////////////////////Texture1D
crBrain::Texture1DMap &crBrain::getTexture1DMap() 
{ 
	return m_texture1dmap; 
}

bool crBrain::getNeedLoadImage1D(const std::string &imageName)
{
	crTexture1D *tex1d = getTexture1D(imageName);
	if(!tex1d) return true;
	return tex1d->getNeedCompile()&&!tex1d->getImage();
}

bool crBrain::getNeedLoadImage1D(const crTexture1D *tex1d)
{
	return tex1d->getNeedCompile()&&!tex1d->getImage();
}

void crBrain::insertTexture1D(const std::string &imageName,crTexture1D *tex1d)
{
	m_texture1dmap[imageName] = tex1d;
}

crTexture1D *crBrain::getTexture1D(const std::string &imageName)
{
	Texture1DMap::iterator itr = m_texture1dmap.find(imageName);
	return itr != m_texture1dmap.end() ? itr->second.get() : NULL;
}

void crBrain::removeTexture1D(const std::string &imageName)
{
	m_texture1dmap.erase(imageName);
}

void crBrain::clearTexture1DMap()
{
	m_texture1dmap.clear();
}

/////////////////////Texture3D
crBrain::Texture3DMap &crBrain::getTexture3DMap() 
{ 
	return m_texture3dmap; 
}

bool crBrain::getNeedLoadImage3D(const std::string &imageName)
{
	crTexture3D *tex3d = getTexture3D(imageName);
	if(!tex3d) return true;
	return tex3d->getNeedCompile()&&!tex3d->getImage();
}

bool crBrain::getNeedLoadImage3D(const crTexture3D *tex3d)
{
	return tex3d->getNeedCompile()&&!tex3d->getImage();
}

void crBrain::insertTexture3D(const std::string &imageName,crTexture3D *tex3d)
{
	m_texture3dmap[imageName] = tex3d;
}

crTexture3D *crBrain::getTexture3D(const std::string &imageName)
{
	Texture3DMap::iterator itr = m_texture3dmap.find(imageName);
	return itr != m_texture3dmap.end() ? itr->second.get() : NULL;
}

void crBrain::removeTexture3D(const std::string &imageName)
{
	m_texture3dmap.erase(imageName);
}

void crBrain::clearTexture3DMap()
{
	m_texture3dmap.clear();
}

/////////////////////////////TextureCubeMap
crBrain::TextureCubeMapMap &crBrain::getTextureCubeMapMap() 
{ 
	return m_textureCubeMapMap; 
}

bool crBrain::getNeedLoadImageCubeMap(const std::string &cubeMapName)
{
	crTextureCubeMap *texCubeMap = getTextureCubeMap(cubeMapName);
	if(!texCubeMap) return true;
	bool isNeedImage = texCubeMap->getNeedCompile();
	int face = crTextureCubeMap::POSITIVE_X;
	while (isNeedImage && face++<=crTextureCubeMap::NEGATIVE_Z)
	{
		isNeedImage = isNeedImage && texCubeMap->getImage(face);
	}
	return isNeedImage;
}

bool crBrain::getNeedLoadImageCubeMap(const crTextureCubeMap *texCubeMap)
{
	bool isNeedImage = texCubeMap->getNeedCompile();
	int face = crTextureCubeMap::POSITIVE_X;
	while (isNeedImage && face++<=crTextureCubeMap::NEGATIVE_Z)
	{
		isNeedImage = isNeedImage && texCubeMap->getImage(face);
	}

	return isNeedImage;
}

void crBrain::insertTextureCubeMap(const std::string &cubeMapName,crTextureCubeMap *texCubeMap)
{
	m_textureCubeMapMap[cubeMapName] = texCubeMap;
}

crTextureCubeMap *crBrain::getTextureCubeMap(const std::string &cubeMapName)
{
	TextureCubeMapMap::iterator itr = m_textureCubeMapMap.find(cubeMapName);
	return itr != m_textureCubeMapMap.end() ? itr->second.get() : NULL;
}

void crBrain::removeTextureCubeMap(const std::string &cubeMapName)
{
	m_textureCubeMapMap.erase(cubeMapName);
}

void crBrain::clearTextureCubeMapMap()
{
	m_textureCubeMapMap.clear();
}

//crBrain::ProgramMap &crBrain::getProgramMapMap() 
//{ 
//	return m_programMap; 
//}
//
//bool crBrain::getNeedLoadProgram(const std::string &fileName)
//{
//	crProgram *program = getProgram(fileName);
//	return !program;
//}
//
//void crBrain::insertProgram(const std::string &fileName,crProgram *program)
//{
//	m_programMap[fileName] = program;
//}
//
//crProgram *crBrain::getProgram(const std::string &fileName)
//{
//	ProgramMap::iterator itr = m_programMap.find(fileName);
//	return itr != m_programMap.end() ? itr->second.get() : NULL;
//}
//
//void crBrain::removeProgram(const std::string &fileName)
//{
//	m_programMap.erase(fileName);
//}

//void crBrain::clearProgramMap()
//{
//	m_programMap.clear();
//}

crBrain::StateSetSet &crBrain::getStateSetSet()
{
    return m_statesetSet;
}

crStateSet* crBrain::checkStateSetExistAndGet(CRCore::crStateSet *stateset)
{
	CRCore::ref_ptr<CRCore::crStateSet> setItr;
	for( StateSetSet::iterator itr = m_statesetSet.begin();
		 itr != m_statesetSet.end();
		 ++itr )
	{
		setItr = *itr;
		if(!m_statesetSet.key_comp()(setItr,stateset) && !m_statesetSet.key_comp()(stateset,setItr))
			return setItr.get();
	}
	return 0;
	//StateSetSet::iterator itr = m_statesetSet.find(stateset);
	//return itr != m_statesetSet.end() ? itr->get() : NULL;
}

void crBrain::insertStateSet(CRCore::crStateSet *stateset)
{
    m_statesetSet.insert(stateset);
}

//crStateSet *crBrain::getProgramStateSet(const std::string& filename, bool addT, bool addB,crStateSet *oldStateset)
//{
//	CRCore::ref_ptr<crProgram> program;
//	CRCore::ref_ptr<crShader> shader;
//	CRCore::ref_ptr<crStateSet> stateset;
//	CRCore::ref_ptr<crStateSet> exist_stateset;
//	std::string vp,fp;
//	program = getProgram(filename);
//	if(!program.valid())
//	{
//		if(!crShader::loadShaderSourceFromFile(filename,vp,fp))
//			return NULL;
//
//		program = new crProgram;
//		program->setName(filename);
//		shader = new crShader(CRCore::crShader::VERTEX,vp);
//		shader->setName(filename+"_vp");
//		program->addShader(shader.get());
//		shader = new crShader(CRCore::crShader::FRAGMENT,fp);
//		shader->setName(filename+"_fp");
//		program->addShader(shader.get());
//		insertProgram(filename,program.get());
//
//		if(addT) program->addBindAttribLocation("tangent",ATTRIBUTE_TANGENT);
//		if(addB) program->addBindAttribLocation("binormal",ATTRIBUTE_BINORMAL);
//	}
//	if(oldStateset) stateset = oldStateset;
//	else stateset = new crStateSet;
//	stateset->setAttributeAndModes(program.get(),CRCore::crStateAttribute::ON);
//	stateset->setMode(GL_LIGHTING, crStateAttribute::OVERRIDE|crStateAttribute::OFF);
//	stateset->setName(crArgumentParser::getFileNameEliminateExt(crArgumentParser::getSimpleFileName(filename)));
//	exist_stateset = checkStateSetExistAndGet(stateset.get());
//	if(exist_stateset.valid())
//		stateset = exist_stateset;
//	else
//		insertStateSet(stateset.get());
//
//	return stateset.get();
//}
//
void crBrain::addInitLoadPage(const std::string &page)
{
    m_initLoadPage.insert(page);
}

bool crBrain::checkPageNeedInitLoad(const std::string &page)
{
    return m_initLoadPage.find(page) != m_initLoadPage.end();
}

//void crBrain::pushInitRenderVec( const crVector3f &vec)
//{
//	m_initRenderVecBuf.push_back(vec);
//}
//
//const crBrain::InitRenderVecBuf &crBrain::getInitRenderVecBuf() const
//{
//	return m_initRenderVecBuf;
//}

void crBrain::setFog( int fog )
{
	m_fog = fog;
}

int crBrain::getFog() const
{
	return m_fog;
}

void crBrain::setFogColor( const crVector3 &color )
{
	m_fogColor = color;
}

const crVector3& crBrain::getFogColor() const
{
	return m_fogColor;
}

void crBrain::setFogParams( const crVector4 &sedh )
{
	m_fogParams = sedh;
}

const crVector4& crBrain::getFogParams() const
{
	return m_fogParams;
}

void crBrain::setMaxDepth(float maxDepth) 
{ 
	m_maxDepth = maxDepth; 
}

float crBrain::getMaxDepth() 
{ 
	return m_maxDepth; 
}

void crBrain::setDofBlurStart( float start )
{
	m_dofBlurStart = start;
}

float crBrain::getDofBlurStart() const
{
	//if(crDisplaySettings::instance()->getNumMultiSamples()>1)
	//	return m_dofBlurFactor * 0.01;
	return m_dofBlurStart;
}

void crBrain::setDofBlurFactor( float blur )
{
	m_dofBlurFactor = blur;
}

float crBrain::getDofBlurFactor() const
{
	//if(crDisplaySettings::instance()->getNumMultiSamples()>1)
	//	return m_dofBlurFactor * 0.01;
	return m_dofBlurFactor;
}

void crBrain::setMotionBlurFactor( float blur )
{
	m_motionBlurFactor = blur;
}

float crBrain::getMotionBlurFactor() const
{
	return m_motionBlurFactor;
}

void crBrain::setUpperSkyColor( const crVector3 &color )
{
	m_upperSkyColor = color;
}

const crVector3& crBrain::getUpperSkyColor() const
{
	return m_upperSkyColor;
}

void crBrain::setLowerSkyColor( const crVector3 &color )
{
	m_lowerSkyColor = color;
}

const crVector3& crBrain::getLowerSkyColor() const
{
	return m_lowerSkyColor;
}

void crBrain::setWorldSize( const crVector3i &worldSize )
{
	m_worldSize = worldSize;
}
const crVector3i& crBrain::getWorldSize() const
{
	return m_worldSize;
}
void crBrain::setMapSize( const crVector2i &mapSize )
{
	m_mapSize = mapSize;
}
const crVector2i& crBrain::getMapSize() const
{
	return m_mapSize;
}

void crBrain::setAmbientIntensity(const crVector4& color)
{
	m_ambientIntensity = color;
}

const crVector4 &crBrain::getAmbientIntensity() const
{
	return m_ambientIntensity;
}
void crBrain::setBrightness( float brightness )
{
	m_brightness = brightness;
}

float crBrain::getBrightness() const
{
	return m_brightness;
}
void crBrain::gameLoop(int threadType)
{
	//if(m_gamePaused)
	//{
	//	switch (threadType)
	//	{
	//	case 0:
	//		m_cullThreadPaused = true;
	//		break;
	//	case 1:
	//		//if(!m_cullThreadPaused)
	//		//{
	//		//	m_drawThreadPaused = false;
	//		//	return;
	//		//}
	//		//if(!m_drawThreadPaused)
	//		//{
	//		//	m_drawThreadPaused = true;
	//		//	return;
	//		//}
	//		//m_gamePausedBarrier->block();
	//		m_drawThreadPaused = true;
	//		break;
	//	}
	//}
	while(m_gamePaused)
	{
		if(m_resumeGame)
		{
			m_gamePaused = false;
			m_cullThreadPaused = false;
			m_drawThreadPaused = false;
			break;
		}
		switch (threadType)
		{
		case 0:
			m_cullThreadPaused = true;
			break;
		case 1:
			if(!m_cullThreadPaused)
			{
				m_drawThreadPaused = false;
				return;
			}
			m_drawThreadPaused = true;
			break;
		}
		CRCore::crThread::sleep(10);
	}
}
void crBrain::pauseGame(int threadType)
{
	if(!isGamePaused())
	{
		m_resumeGame = false;
		m_gamePaused = true;
		CRCore::crBlockDetectThread::getInstance()->pause();
		while(!(m_cullThreadPaused/* && m_drawThreadPaused*/))
		    CRCore::crThread::sleep(10);
	}
}

void crBrain::resumeGame()
{
	m_resumeGame = true;
	CRCore::crBlockDetectThread::getInstance()->resume();
	//while (!m_gamePaused)
	//{
	//	CRCore::crThread::sleep(20);
	//}
	//m_gamePaused = false;
	//m_cullThreadPaused = false;
	//m_drawThreadPaused = false;
}

void crBrain::setCameraPosition(const crVector3 &position)
{
    m_cameraPosition = position;
}
void crBrain::setCameraFrustum(const crPolytope &frustum)
{
	m_cameraFrustum = frustum;
}
//void crBrain::setFrameStamp(crFrameStamp *fs)
//{
//    m_frameStamp = fs;
//}

void crBrain::insertObjectLight(ObjectLight *objectLight)
{
	if(objectLight) m_objectLightMap[objectLight->m_objectName] = objectLight;
}

const crBrain::ObjectLightMap &crBrain::getObjectLightMap()
{
    return m_objectLightMap;
}

void crBrain::clearObjectLightMap()
{
	m_objectLightMap.clear();
}

crBrain::ObjectLight* crBrain::findObjectLight(const std::string &name)
{
	ObjectLightMap::iterator itr = m_objectLightMap.find(name);
	return itr != m_objectLightMap.end() ? itr->second.get() : NULL;
}

void crBrain::createMemFile(const std::string &filename,crStreamBuf *stream)
{
	CRCore::ScopedLock<crMutex> lock(m_memFileMutex);
    m_memFileMap[filename] = stream;
}

crStreamBuf *crBrain::openMemFile(const std::string &filename,int mode)
{
    CRCore::ScopedLock<crMutex> lock(m_memFileMutex);
	crStreamBuf *stream = NULL;
	if(mode == std::ios::out)
	{
		stream = new crStreamBuf;
        m_memFileMap[filename] = stream;
	}
	else if(mode == std::ios::app)
	{
		MemFileMap::iterator itr = m_memFileMap.find(filename);
		if(itr != m_memFileMap.end())
		{
            stream = itr->second.get();
			stream->seekEnd();
		}
		else
		{
			stream = new crStreamBuf;
			m_memFileMap[filename] = stream;
		}
	}
	else if(mode == std::ios::in)
	{
		MemFileMap::iterator itr = m_memFileMap.find(filename);
		stream = itr == m_memFileMap.end() ? NULL : itr->second.get();
		if(stream) stream->seekBegin();
	}

	return stream;
}

void crBrain::closeMemFile(CRCore::crStreamBuf *stream)
{
    stream->seekBegin();
}

void crBrain::releaseMemFile(const std::string& filename)
{
    CRCore::ScopedLock<crMutex> lock(m_memFileMutex);
    m_memFileMap.erase(filename);
}
void crBrain::pushLoadFailedTexture(crTexture *tex)
{
	CRCore::ScopedLock<crMutex> lock(m_leadFailedDequeMutex);
	m_loadFailedDeque.push_back(tex);
}
void crBrain::update(float dt)
{
	m_updtimer += dt;
	if(m_updtimer>0.5f)
	{
		m_updtimer = 0.0f;
		ref_ptr<crTexture> tex;
		m_leadFailedDequeMutex.lock();
		if(!m_loadFailedDeque.empty())
		{
			tex = m_loadFailedDeque.front().get();
			m_loadFailedDeque.pop_front();
		}
		m_leadFailedDequeMutex.unlock();
		if(tex.valid())
		{
			if(dynamic_cast<crTextureCubeMap *>(tex.get()))
			{
				tex->getImageDataRequestHandler()->requestCubeImageFile(tex->getImageNameID(),dynamic_cast<crTextureCubeMap *>(tex.get()));
			}
			else
			{
				tex->setImage(0,tex->getImageDataRequestHandler()->requestImageFile(tex->getImageNameID(),tex.get()));
			}
		}
	}
}
CRCore::crVector2s crBrain::getCoord(CRCore::crVector2 pos) const
{
	const crVector3& wordSize = getWorldSize();
	const crVector2i& mapSize = getMapSize();
	crVector2 origin = crVector2(-wordSize[0]*0.5f,-wordSize[1]*0.5f);
	float dx = wordSize[0] / (float)(mapSize[0]-1); 
	float dy = wordSize[1] / (float)(mapSize[1]-1);

	pos -= origin;
	pos[0] /= dx;
	pos[1] /= dy;
	crVector2s coord(int(pos[0]+0.5f),int(pos[1]+0.5f));//4舍5入取整
	return coord;
}
CRCore::crVector2 crBrain::getPosXY(const crVector2s& coord) const
{
	const crVector3& wordSize = getWorldSize();
	const crVector2i& mapSize = getMapSize();
	crVector2 origin = crVector2(-wordSize[0]*0.5f,-wordSize[1]*0.5f);
	float dx = wordSize[0] / (float)(mapSize[0]-1); 
	float dy = wordSize[1] / (float)(mapSize[1]-1);

	crVector2 pos(coord[0],coord[1]);
	pos[0] *= dx;
	pos[1] *= dy;
	pos += origin;
	return pos;
}
ref_ptr<crImage>& CRCore::gCredefimage()
{
	static ref_ptr<crImage> g_credefimage;
	return g_credefimage;
}