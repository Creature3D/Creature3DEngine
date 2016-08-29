
#ifndef CRCORE_CRSTATICSHADOWMANAGER
#define CRCORE_CRSTATICSHADOWMANAGER 1
#include <CRCore/crGroup.h>
#include <CRCore/crLightSource.h>
#include <CRCore/crCameraNode.h>
//#include <CRCore/crViewPort.h>

#include <map>

namespace CRCore {

class CR_EXPORT crStaticShadow : public CRCore::crBase
{
public:
	crStaticShadow();
	crStaticShadow(const crStaticShadow&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);
	META_Base(CRCore, crStaticShadow);

    void addShadowerNode(crNode *shadower);
	void removeShadowerNode(int index);
	void removeShadowerNode(const std::string &nodeName);
	void removeShadowerNode(crNode *shadower);
	crGroup *getShadower() { return shadowerGroup.get(); }

	void addShadowedNode(crNode *shadowed);
	void removeShadowedNode(int index);
	void removeShadowedNode(const std::string &nodeName);
	void removeShadowedNode(crNode *shadowed);
	crGroup *getShadowed() { return shadowedGroup.get(); }

	void setMapSize(const crVector2i &mapSize);
	void setMapSize(int width, int height);
	const crVector2i &getMapSize(){ return m_mapSize; }

	void setBlurSize(const crVector2f &size);
	void setBlurSize(float h, float v);
	const crVector2f &getBlurSize(){ return m_blurSize; }

	void setOutPutImageName(const std::string &outPutImageName) { m_outPutImageName = outPutImageName; }
	const std::string &getOutPutImageName() { return m_outPutImageName; }

	void setLightSource(crLightSource *ls) { m_lightSource = ls; }
	crLightSource *getLightSource(){ return m_lightSource.get(); }

	void setLightPos(const crVector3 &lightPos) { m_lightPos = lightPos; }
	void setCameraPos(const crVector3 &cameraPos) { setLightPos(cameraPos); }
	
    enum StaticShadowMode
	{
		RenderToImage = 0x1,
        RenderToTexture = 0x2,
		RenderShadowMap = 0x4,
		RenderPhoto = 0x8,
        NeedBlur = 0x10
	};
	void setMode(StaticShadowMode mode) { m_mode = mode; }
	StaticShadowMode getMode() { return m_mode; }

	void setNeedRender(bool needRender) { m_needRender = needRender; }
	bool getNeedRender() { return m_needRender; }

	void init();
	void computeLightMapMVP();
protected:
	virtual ~crStaticShadow() {}

	ref_ptr<crGroup> shadowerGroup;
	ref_ptr<crGroup> shadowedGroup;
	ref_ptr<crLightSource> m_lightSource;
	crVector3 m_lightPos;
	//crVector3 m_dir;
	//float m_fov;
	crVector2i m_mapSize;
	crVector2f m_blurSize;
	StaticShadowMode m_mode;
	std::string m_outPutImageName;
	bool m_needRender;
}; 

class CR_EXPORT crStaticShadowManager : public crGroup
{
public :
	crStaticShadowManager();
	static crStaticShadowManager *getInstance();

    crStaticShadowManager(const crStaticShadowManager&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

    META_Node(CRCore, crStaticShadowManager);
    
    virtual void traverse(crNodeVisitor& nv);

	virtual void releaseObjects(crState* = 0) const;
	crCameraNode *getShadowCameraNode(){ return m_shadowCamera.get(); }
	crCameraNode *getPhotoCameraNode(){ return m_photoimageCamera.get(); }

	typedef std::map<std::string, ref_ptr<crStaticShadow> > StaticShadowMap;
    void addStaticShadow(crStaticShadow *staticShadow);

protected :
    virtual ~crStaticShadowManager() {}
	void createCameras();

	StaticShadowMap m_staticShadowMap;
	ref_ptr<crCameraNode> m_shadowCamera;
    ref_ptr<crCameraNode> m_photoCamera;
	ref_ptr<crCameraNode> m_hgaussianCamera;
	ref_ptr<crCameraNode> m_vgaussianCamera;
};

}

#endif
