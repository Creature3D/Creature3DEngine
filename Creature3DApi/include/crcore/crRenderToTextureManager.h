/**********************************************************************
*
*	文件:	   crRenderToTextureManager.h
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
*	Copyright 2005 吴财华
**********************************************************************/
#ifndef CRCORE_CRRENDERTOTEXTUREMANAGER
#define CRCORE_CRRENDERTOTEXTUREMANAGER 1
#include <CRCore/crGroup.h>
#include <CRCore/crRenderToTexture.h>
#include <CRCore/crCameraNode.h>
//#include <CRCore/crViewPort.h>

#include <map>

namespace CRCore {

class CR_EXPORT crRenderToTextureManager : public crNode
{
public :
	crRenderToTextureManager();
	static crRenderToTextureManager *getInstance();

    crRenderToTextureManager(const crRenderToTextureManager&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

    META_Node(CRCore, crRenderToTextureManager);
    
    virtual void traverse(crNodeVisitor& nv);

	typedef std::map<std::string, ref_ptr<crRenderToTexture> > RenderToTextureMap;
	//typedef std::vector< ref_ptr<crRenderToTexture> > SyncAddRTTVec;
    void addRenderToTexture(crRenderToTexture *rtt);
	crRenderToTexture *findRenderToTexture(const std::string &name);
	//void removeAllRenderTexture();

	inline void setCurrentProjectMatrix(const crMatrix &prjMat){  m_currentProjectMatrix = prjMat; }
	inline void setCurrentViewMatrix(const crMatrix &viewMat){  m_currentViewMatrix = viewMat; }
	inline const crMatrix &getCurrentProjectMatrix()const {  return m_currentProjectMatrix; }
	inline const crMatrix &getCurrentViewMatrix()const {  return m_currentViewMatrix; }

	//virtual void updateUniform(/*crViewPort *sceneViewPort*/);
	//virtual void swapBuffers();
	virtual void releaseObjects(crState* = 0);
    virtual void clear();
	inline void setNearPoint(const crVector3 &point){ m_nearPoint = point; }
	inline const crVector3 &getNearPoint() { return m_nearPoint; }
	inline int getRttShadowMode(){ return m_rttShadowMode; }
protected :
    virtual ~crRenderToTextureManager();
	static ref_ptr<crRenderToTextureManager> m_instance;

	//crCriticalMutex					m_syncAddRTTVecMutex;
	//SyncAddRTTVec					m_syncAddRTTVec;
	RenderToTextureMap m_renderToTextureMap;
	//ref_ptr<crCameraNode> m_camera;

	crMatrix                        m_currentProjectMatrix;
	crMatrix                         m_currentViewMatrix;
	crVector3 m_nearPoint;
	int m_rttShadowMode;//0:关闭,1:开启一个,2:开启2个
};

}

#endif
