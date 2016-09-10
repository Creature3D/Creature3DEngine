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
#ifndef CRCORE_CRBRAIN_H
#define CRCORE_CRBRAIN_H

#include <CRCore/crDB.h>
#include <CRCore/Referenced.h>
#include <CRCore/ref_ptr.h>
//#include <CRCore/crResource.h>
#include <CRCore/crNodeVisitor.h>
#include <CRCore/crNode.h>
#include <CRCore/crTexture2D.h>
#include <CRCore/crTexture1D.h>
#include <CRCore/crTexture3D.h>
#include <CRCore/crTextureCubeMap.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crProgram.h>
#include <CRCore/thread/crThread.h>
#include <CRCore/thread/crBarrier.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/thread/crCriticalMutex.h>
#include <vector>
#include <set>
#include <map>
#include <deque>
namespace CRCore {
class crEventGroup;
class crStreamBuf;
//class CR_EXPORT crAITreeHead : public crGroup
//{
//public:
//	crAITreeHead(){};
//	crAITreeHead(const crAITreeHead,const CRCore::crCopyOp& copyop){}
//
//	META_Node(CRCore, crAITreeHead);
//
//    virtual void traverse(CRCore::crNodeVisitor& nv)
//	{
//		if( nv.getVisitorType() == CRCore::crNodeVisitor::AI_UPDATE_VISITOR||
//			nv.getTraversalMode() == CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN )
//		{
//			crGroup::traverse(nv);
//		}
//	}
//protected:
//	~crAITreeHead(){};
//
//};

class CR_EXPORT crBrain: public crBase
{
public:
  crBrain();
  ~crBrain();

  virtual crBase* cloneType() const{ return NULL; }
  virtual crBase* clone(const crCopyOp&) const{ return NULL; }
  virtual const char* libraryName() const { return "CRCore"; }
  virtual const char* className() const { return "crBrain"; }

  typedef std::vector< ref_ptr<crDB> >        DBArray;

  void  insertDB(crDB* pDB){m_dbArray.push_back(pDB);}
  inline crDB* getDB(int index){return m_dbArray[index].get();}
  inline int getDBCount() { return m_dbArray.size(); }
  inline DBArray &getDBArray(){ return m_dbArray; }
  crDB* getCurrentActiveDB();
  
  void setCurrentActiveDB(unsigned int id);

  //inline crResource* getResource(){ return m_resource.get(); }
  //inline void releaseResource(){ m_resource->unref(); }

  //inline void increaseGlobalLight(){ m_globalLightNum++; }
  //inline int getGlobalLightNum(){ return m_globalLightNum; }
  
  void accept(crNodeVisitor& nv);

  //inline crAITreeHead *getAITreeHead(){ return m_aiTreeHead.get(); }
  
  virtual void clear();
  void prune();
  
  void insertDescription( const std::string &str );
  inline std::string getDescription() const{ return m_description; }
  std::string &getDescription(){ return m_description; }
  
  //void attachKeyboardMouseNode(crNode *keyboardMouseNode);
  //void updateKeyboardMouse(crNodeVisitor &updvisitor);

  typedef std::map<std::string, CRCore::ref_ptr<CRCore::crTexture2D> > Texture2DMap;

  Texture2DMap &getTexture2DMap();
  bool getNeedLoadImage2D(const std::string &imageName);
  bool getNeedLoadImage2D(const crTexture2D *tex2d);
  void insertTexture2D(const std::string &imageName,crTexture2D *tex2d);
  crTexture2D *getTexture2D(const std::string &imageName);
  void removeTexture2D(const std::string &imageName);
  void clearTexture2DMap();

  typedef std::map<std::string, CRCore::ref_ptr<CRCore::crTexture1D> > Texture1DMap;

  Texture1DMap &getTexture1DMap();
  bool getNeedLoadImage1D(const std::string &imageName);
  bool getNeedLoadImage1D(const crTexture1D *tex1d);
  void insertTexture1D(const std::string &imageName,crTexture1D *tex1d);
  crTexture1D *getTexture1D(const std::string &imageName);
  void removeTexture1D(const std::string &imageName);
  void clearTexture1DMap();

  typedef std::map<std::string, CRCore::ref_ptr<CRCore::crTexture3D> > Texture3DMap;

  Texture3DMap &getTexture3DMap();
  bool getNeedLoadImage3D(const std::string &imageName);
  bool getNeedLoadImage3D(const crTexture3D *tex3d);
  void insertTexture3D(const std::string &imageName,crTexture3D *tex3d);
  crTexture3D *getTexture3D(const std::string &imageName);
  void removeTexture3D(const std::string &imageName);
  void clearTexture3DMap();

  typedef std::map<std::string, CRCore::ref_ptr<CRCore::crTextureCubeMap> > TextureCubeMapMap;

  TextureCubeMapMap &getTextureCubeMapMap();
  bool getNeedLoadImageCubeMap(const std::string &cubeMapName);
  bool getNeedLoadImageCubeMap(const crTextureCubeMap *texCubeMap);
  void insertTextureCubeMap(const std::string &cubeMapName,crTextureCubeMap *texCubeMap);
  crTextureCubeMap *getTextureCubeMap(const std::string &cubeMapName);
  void removeTextureCubeMap(const std::string &imageName);
  void clearTextureCubeMapMap();

  //typedef std::map<std::string, CRCore::ref_ptr<CRCore::crProgram> > ProgramMap;

  //ProgramMap &getProgramMapMap();
  //bool getNeedLoadProgram(const std::string &fileName);
  //void insertProgram(const std::string &fileName,crProgram *program);
  //crProgram *getProgram(const std::string &fileName);
  //void removeProgram(const std::string &fileName);
  //void clearProgramMap();

  struct LessStateSetFunctor
  {
	  bool operator () (const CRCore::ref_ptr<CRCore::crStateSet>& lhs,const CRCore::ref_ptr<CRCore::crStateSet>& rhs) const
	  {
		  if (lhs==rhs) return 0;
		  //return lhs->compare(*rhs,true);
		  //if (lhs->compare(*rhs,true) == 0)
			 // return false;
		  //return true;
		  return lhs->compare(*rhs,true)<0;
	  }
  };
  typedef std::set<CRCore::ref_ptr<CRCore::crStateSet>,LessStateSetFunctor> StateSetSet;

  StateSetSet &getStateSetSet();
  crStateSet* checkStateSetExistAndGet(CRCore::crStateSet *stateset);
  void insertStateSet(CRCore::crStateSet *stateset);
  void releaseObjects(CRCore::crState* state);
  //crStateSet *getProgramStateSet(const std::string& filename, bool addT, bool addB,crStateSet *oldStateset = 0);
  CRCore::crVector2s getCoord(CRCore::crVector2 pos) const;
  CRCore::crVector2 getPosXY(const crVector2s& coord) const;
public:
  static crBrain *getInstance();
  void gameLoop(int threadType);//0:cullThread, 1:drawThread
  inline bool isGamePaused(){ return m_gamePaused/*||m_cullThreadPaused||m_drawThreadPaused*/; }
  void pauseGame(int threadType = 2);//0:cullThread, 1:drawThread, 2:otherThread
  void resumeGame();
  //inline void cullThreadPaused(bool paused){ m_cullThreadPaused = paused; }
  //inline void drawThreadPaused(bool paused){ m_drawThreadPaused = paused; }

///////临时
//public:
//	void setDisablePagedLod(bool disable){ m_disablePagedLod = disable; }
//	bool getDisablePagedLod(){ return m_disablePagedLod; }
//
//	void setDisableVSM(bool disable){ m_disableVSM = disable; }
//	bool getDisableVSM(){ return m_disableVSM; }
//
//	void setDisableSoftShadow(bool disable){ m_disableSoftShadow = disable; }
//	bool getDisableSoftShadow(){ return m_disableSoftShadow; }
//
//	void setDisableParallax(bool disable){ m_disableParallax = disable; }
//	bool getDisableParallax(){ return m_disableParallax; }
//
//	void setDisableNormalMapping(bool disable){ m_disableNormalMapping = disable; }
//	bool getDisableNormalMapping(){ return m_disableNormalMapping; }
//
//	void setDisableFog(bool disable){ m_disableFog = disable; }
//	bool getDisableFog(){ return m_disableFog; }
///////临时 end
  typedef std::set<std::string> StringSet;
  void addInitLoadPage(const std::string &page);
  bool checkPageNeedInitLoad(const std::string &page);

  typedef std::vector< ref_ptr<Referenced> > ManagerInstanceVec;
  void pushInstance(Referenced *instance);

  void setClip(const crVector2 &clip) { m_clip = clip; if(m_clip[1]>1e5) m_clip[1] = 1e5; }
  const crVector2 &getClip() const { return m_clip; }
  
  void setAmbientIntensity(const crVector4& color);
  const crVector4 &getAmbientIntensity() const;
  void setBrightness( float brightness );
  float getBrightness() const;

  //typedef std::vector<crVector3f> InitRenderVecBuf;
  //void pushInitRenderVec( const crVector3f &vec);
  //const InitRenderVecBuf &getInitRenderVecBuf() const;

  void setFog( int fog );
  int getFog() const;

  void setFogColor( const crVector3 &color );
  const crVector3& getFogColor() const;

  void setFogParams( const crVector4 &sedh );
  const crVector4& getFogParams() const;

  void setMaxDepth(float maxDepth);
  float getMaxDepth();

  void setDofBlurStart( float start );
  float getDofBlurStart() const;

  void setDofBlurFactor( float blur );
  float getDofBlurFactor() const;

  void setMotionBlurFactor( float blur );
  float getMotionBlurFactor() const;

  void setUpperSkyColor( const crVector3 &color );
  const crVector3& getUpperSkyColor() const;

  void setLowerSkyColor( const crVector3 &color );
  const crVector3& getLowerSkyColor() const;

  void setWorldSize( const crVector3i &worldSize );
  const crVector3i& getWorldSize() const;
  void setMapSize( const crVector2i &mapSize );
  const crVector2i& getMapSize() const;

  void setCameraPosition(const crVector3 &position);
  inline const crVector3 &getCameraPosition() { return m_cameraPosition; }
  void setCameraFrustum(const crPolytope &frustum);
  inline crPolytope &getCameraFrustum(){ return m_cameraFrustum; }
  //void setFrameStamp(crFrameStamp *fs);
  //inline crFrameStamp *getFrameStamp() { return m_frameStamp.get(); }

  struct CR_EXPORT ObjectLight : public Referenced
  {//输入输出时用
	  typedef std::set<std::string> LightNameSet;
	  std::string m_objectName;
	  LightNameSet m_needLight;
	  LightNameSet m_avoidLight;
  };
  typedef std::map< std::string, ref_ptr<ObjectLight> > ObjectLightMap;
  void insertObjectLight(ObjectLight *objectLight);
  const ObjectLightMap &getObjectLightMap();
  void clearObjectLightMap();
  ObjectLight* findObjectLight(const std::string &name);

  typedef std::map< std::string,CRCore::ref_ptr<CRCore::crStreamBuf> > MemFileMap;
  void createMemFile(const std::string &filename,crStreamBuf *stream);
  crStreamBuf *openMemFile(const std::string &filename,int mode);//
  void closeMemFile(CRCore::crStreamBuf *stream);
  void releaseMemFile(const std::string& filename);
  
  void setCalcStaticMeshShadow(bool bln) { m_calcStaticMeshShadow = bln; }
  inline bool getCalcStaticMeshShadow() { return m_calcStaticMeshShadow; }
  typedef std::deque< ref_ptr<crTexture> > LoadFailedTexDeque;
  void pushLoadFailedTexture(crTexture *tex);
  void update(float dt);
protected:
  static ref_ptr<crBrain> m_pBrain;
  crCriticalMutex m_managerInstanceVecMutex;
  ManagerInstanceVec m_managerInstanceVec;
  //crRenderManager *m_pRenderManager;

  // Scene Geometry info
  DBArray                   m_dbArray;
  //ref_ptr<crResource>       m_resource;
  //int                       m_globalLightNum;
  
  //ref_ptr<crAITreeHead>     m_aiTreeHead;

  std::string               m_description;
  unsigned int              m_currentActiveDB;

  //ref_ptr<crNode>     m_keyboardMouseNode;

  Texture2DMap        m_texture2dmap;
  Texture1DMap        m_texture1dmap;
  Texture3DMap        m_texture3dmap;
  TextureCubeMapMap   m_textureCubeMapMap;
  //ProgramMap          m_programMap;

  StateSetSet         m_statesetSet;
  StringSet           m_initLoadPage;
  volatile bool                m_gamePaused;
  volatile bool                m_cullThreadPaused;
  volatile bool                m_drawThreadPaused;
  volatile bool                m_resumeGame;
///////临时
//protected:
//	bool m_disablePagedLod;
//	bool m_disableVSM;
//	bool m_disableSoftShadow;
//	bool m_disableParallax;
//	bool m_disableNormalMapping;
//	bool m_disableFog;
///////临时 end
  //////////////////////postprocess effect
  crVector2                       m_clip;
  crVector4                       m_ambientIntensity;//LightModel AmbientIntensity
  float                           m_brightness;

  int                             m_fog;// = 0:nofog, = 1:DepthFog, = 2:NoiseFog
  crVector3                       m_fogColor;
  crVector4                       m_fogParams;//(Start,End,Density,Height)
  float                           m_maxDepth;
  float                           m_dofBlurStart;
  float                           m_dofBlurFactor;
  float                           m_motionBlurFactor;

  crVector3                       m_upperSkyColor;
  crVector3                       m_lowerSkyColor;
  crVector3i                      m_worldSize;//
  crVector2i                      m_mapSize;

  //InitRenderVecBuf                m_initRenderVecBuf;

  //ref_ptr<crFrameStamp>           m_frameStamp;
  crVector3                       m_cameraPosition;
  crPolytope                      m_cameraFrustum;
  //CRCore::crBarrier *m_gamePausedBarrier;

  ObjectLightMap m_objectLightMap;

  MemFileMap m_memFileMap;
  crMutex m_memFileMutex;

  LoadFailedTexDeque m_loadFailedDeque;
  crMutex m_leadFailedDequeMutex;
  float m_updtimer;

  bool m_calcStaticMeshShadow;
};
extern CR_EXPORT ref_ptr<crImage>& gCredefimage();
}

//#define g_Brain CRCore::crBrain::getInstance();
#endif