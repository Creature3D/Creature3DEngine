/**********************************************************************
*
*	文件:	   crLightSourceManager.h
*
*	描述:	  
*
*	作者:	   吴财华
*					
*
*	Copyright 2005 吴财华
**********************************************************************/
#ifndef CRCORE_CRLIGHTSOURCEMANAGER
#define CRCORE_CRLIGHTSOURCEMANAGER 1

#include <CRCore/crBase.h>
#include <CRCore/crLightSource.h>
#include <CRCore/crCameraNode.h>
#include <CRCore/thread/crMutex.h>
#include <map>
#include <vector>

namespace CRCore {

class CR_EXPORT crLightSourceManager : public CRCore::Referenced
{
    public :
		crLightSourceManager();
		static crLightSourceManager *getInstance();
        
		typedef std::vector< CRCore::ref_ptr<CRCore::crLightSource> > LSVec;
		int getNumLights();
		LSVec &getLSVec();
        void insertLightSource( CRCore::crLightSource *ls );
		crLightSource *findLightSource(const std::string &name);
		void removeLightSource(CRCore::crLightSource *ls);
		void removeLightSource(const std::string &name);
		//crLightSource *selectLightByEye(const CRCore::crVector3& eyeLocal);
		//void selectLightByBoundingBox(const CRCore::crObject *obj,const CRCore::crBoundingBox &bbox, ref_ptr<crLightSource> &ls1, ref_ptr<crLightSource> &ls2, ref_ptr<crLightSource> &ls3,bool effectByShadow);
		
		//typedef std::multimap< float, crLightSource *, std::greater<float> > SelectLightMap;
		void selectLightByBoundingBox(const CRCore::crObject *obj,const CRCore::crBoundingBox &bbox);
		inline LSVec &getOnePassLightVec(){ return m_onePassLightVec; }
		inline LSVec &getAppendLightVec(){ return m_appendLightVec; }

		//crCameraNode *getShadowCameraNode(){ return m_shadowCamera.get(); }
		//crLightSource *getCurrentFrameEyeShadowLightSource(){ return m_currentFrameEyeShadowLightSource.get(); }
        
		bool getHasSunLight();
		crLightSource *getSunLightSource();

		crLightSource *reuseOrgetOrCreateAnIdleEffectNodes(const std::string &lsName);
        
		//void releaseObjects(crState* = 0);

		//inline void lock() { m_mutex.lock(); }
		//inline void unlock() { m_mutex.unlock(); }

		//void setSunShadowDistance(float sunShadowDistance);
		//inline float getSunShadowDistance(){ return m_sunShadowDistance; }
		
		virtual void clear();
		inline void lock(){ m_mutex.lock(); }
		inline void unlock(){ m_mutex.unlock(); }

		void setLightValidDistance(float dist);
		float getLightValidDistance();
    protected :
        virtual ~crLightSourceManager();
		static ref_ptr<crLightSourceManager> m_instance;

		//void createShadowCamera();
		//ref_ptr<crLightSource> m_currentFrameEyeShadowLightSource;
		ref_ptr<crLightSource> m_sunLightSource;
		//float                  m_sunShadowDistance;
        LSVec m_lsVec;
		bool m_hasSunLight;

		//SelectLightMap m_selectLightMap;
		LSVec m_onePassLightVec;
		LSVec m_appendLightVec;
        float m_lightValidDistance;
		crMutex m_mutex;
		//ref_ptr<crCameraNode> m_shadowCamera;
};

}

#endif
