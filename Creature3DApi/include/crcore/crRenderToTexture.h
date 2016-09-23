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
#ifndef CRCORE_CRRENDERTOTEXTURE
#define CRCORE_CRRENDERTOTEXTURE 1

#include <CRCore/crBase.h>
#include <CRCore/crLightSource.h>
#include <CRCore/crCameraNode.h>
#include <CRCore/crPolytope.h>
#include <CRCore/crTextureCubeMap.h>
#include <CRCore/crBrain.h>
#include <map>
#include <vector>

namespace CRCore {

class CR_EXPORT crRenderToTexture : public crNodeCallback
{//CullCallback
public:
	crRenderToTexture();
	crRenderToTexture(const crRenderToTexture&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);
	//META_Base(CRCore, crRenderToTexture);

	inline virtual void operator()(crNode* node, crNodeVisitor* nv)
	{
		if(getNeedUpd(/*nv->getFrameStamp()*/))
		{
			if(m_lightSource && m_lightSource->getCalcShadow())
			{
				if(!(m_mode & CalcMVPByEyePoint2) || (crBrain::getInstance()->getCalcStaticMeshShadow() && m_lightSource->getCalcShadow2()))
				{//如果不计算静态模型阴影则不需要计算远距离阴影
					update();
					if(m_lightSource->getLightIsSunLight() || !culled())
					{
						//CRCore::notify(CRCore::FATAL)<<"crRenderToTexture operator() "<<m_lightSource->getName()<<std::endl;
						traverse(*nv);
					}
				}
			}
			else
			{
				update();
				traverse(*nv);
			}
		}
		crNodeCallback::traverse(node,nv);
	}

	void addRenderNode(crNode *render);
	void removeRenderNode(int index);
	void removeRenderNode(const std::string &nodeName);
	void removeRenderNode(crNode *render);
	crGroup *getRender() { return m_renderGroup.get(); }
	crNode *findRenderNode(const std::string &nodeName);
    //void setRenderProgrom(const std::string& filename, bool addT=false, bool addB=false);

	void createTexturedRoot();
	void addTexturedNode(crNode *textured);
	void removeTexturedNode(int index);
	void removeTexturedNode(const std::string &nodeName);
	void removeTexturedNode(crNode *textured);
	crGroup *getTexturedRoot() { return m_texturedGroup.get(); }

	void setMapSize(const crVector2i &mapSize);
	void setMapSize(int width, int height);
	const crVector2i &getMapSize(){ return m_mapSize; }

	void setBlurSize(const crVector2f &size);
	void setBlurSize(float h, float v);
	const crVector2f &getBlurSize(){ return m_blurSize; }

	void setShadowRadius(float shadowRadius);
	inline float getShadowLength() { return m_shadowRadius * 2.0f - m_shadowOffset; }
	void setShadowOffset(float shadowOffset);

	void setOutPutImageName(const std::string &outPutImageName) { m_outPutImageName = outPutImageName; }
	const std::string &getOutPutImageName() { return m_outPutImageName; }

	void setLightSource(crLightSource *ls) { m_lightSource = ls; }
	crLightSource *getLightSource(){ return m_lightSource; }

	//void setLightPos(const crVector3 &lightPos) { m_lightPos = lightPos; }
	void setCameraPos(const crVector3 &cameraPos) { m_cameraPos = cameraPos; }
	void setCameraDir(const crVector3 &cameraDir) { m_cameraDir = cameraDir; }
	void setCameraUp(const crVector3 &cameraUp) { m_cameraUp = cameraUp; }
	void setCameraFov(float cameraFov) { m_cameraFov = cameraFov; }

	enum RenderToTextureMode
	{
		RenderToImage = 0x1,
		RenderToTexture = 0x2,
		RenderToFrame = 0x4,
		RenderDepthMap = 0x8,//RenderShadowMap
		RenderCollideMap = 0x10,
		RenderColorMap = 0x20,
		RenderCubeMap = 0x40,
		NeedBlur = 0x80,
		RealTimeUpd = 0x100,//实时更新
		UpdOnce = 0x200,//更新一次
		UpdByExtern = 0x400,//根据外部扩展函数更新
		CalcMVPBySetting = 0x800,
		CalcMVPByRenderData = 0x1000,
		CalcMVPByEyePoint = 0x2000,
		CalcMVPByShadowRadius = 0x4000,
		CalcMVPByEyePoint2 = 0x8000,
		CameraMVP = 0x10000,
		ExternMVP = 0x20000,
		NRPRender = 0x40000,
		RenderGIMap = 0x80000,
		Default = RenderToTexture |
		          RenderDepthMap |
                  RealTimeUpd |
                  CalcMVPBySetting
	};
	void setMode(RenderToTextureMode mode) { m_mode = mode; }
	RenderToTextureMode getMode() { return m_mode; }

	bool getNeedUpd(/*const crFrameStamp *fs*/);
	void setNeedUpd(bool needUpd) { m_needUpd = needUpd; }

	//void setNeedRender(bool needRender) { m_needRender = needRender; }
	//bool getNeedRender() { return m_needRender; }

	virtual void init(/*float bias = 0.0f,*/ float blursize = 10.0f);
	virtual void computeMVP( /*const crVector3 & eyePoint = crVector3(0.0,0.0,0.0)*/ );
	crMatrix &getProjection() { return m_projection; }
	const crMatrix &getProjection() const { return m_projection; }
	crMatrix &getView() { return m_view; }
	const crMatrix &getView() const { return m_view; }
	void setProjectionMatrix(const crMatrix &proj);
	void setViewMatrix(const crMatrix &view);

	void setLightRange(float range) { m_lightRange = range; }

	virtual void preUpdate(){}
	virtual void update(/*const crVector3 & eyePoint,const crVector3 & center*/);
    
	virtual bool culled();

	crImage *getImage();
	crTexture2D *getTexture();
	crTexture2D *getBlurTexture();

	//virtual void updateUniform(/*crViewPort *sceneViewPort*/);
	//virtual void swapBuffers();
	virtual void releaseObjects(crState* = 0);
	virtual void traverse(crNodeVisitor& nv);

	void setWriteImageCallback(crCameraNode::DrawCallback *callback){ m_writeImageCallback = callback; }

	static void setCurrentViewFrustum(const crPolytope &currentViewFrustum) { m_currentViewFrustum = currentViewFrustum; }

	inline void setName( const std::string& name ) { m_name = name; }
	inline void setName( const char* name ) { m_name = name; }
	inline const std::string& getName() const { return m_name; }

	void setUpdateInterval(float interval);
	float getUpdateInterval() const;
	void setEnable(bool enable);
	bool getEnable();
	crCameraNode *getCameraNode();
protected:
	virtual ~crRenderToTexture();
	virtual void createCamera();
	virtual void createBlurCamera(float blurSize);

	std::string m_name;

	ref_ptr<crGroup> m_renderGroup;
	ref_ptr<crGroup> m_texturedGroup;
	//ref_ptr<crLightSource> m_lightSource;
	crLightSource *m_lightSource;
	
	crVector3d m_cameraPos;
	crVector3d m_cameraDir;
	crVector3d m_cameraUp;
	double m_cameraFov;
	float m_lightRange;

	crVector2i m_mapSize;
	crVector2f m_blurSize;
	RenderToTextureMode m_mode;
	std::string m_outPutImageName;
	bool m_needUpd;
	int m_lastUpdFrameNumber;

    ref_ptr<crImage> m_image;
	ref_ptr<crTexture2D> m_texture;
	ref_ptr<crTexture2D> m_texture_vblur;
	ref_ptr<crTexture2D> m_texture_hblur;//最终的
    crMatrix     m_projection;
	crMatrix     m_view;
    static crPolytope    m_currentViewFrustum;
	ref_ptr<crCameraNode> m_camera;
	ref_ptr<crCameraNode> m_vblurCamera;
	ref_ptr<crCameraNode> m_hblurCamera;
	ref_ptr<crCameraNode::DrawCallback> m_writeImageCallback;
	float m_shadowRadius;
	float m_shadowOffset;

	float m_lastUpdateTime;
	float m_updateInterval;
	bool m_enable;
};
//
class CR_EXPORT crRenderToCubeTexture : public crRenderToTexture
{
public:
	crRenderToCubeTexture(){};
	crRenderToCubeTexture(const crRenderToTexture&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);
	
	//META_Base(CRCore, crRenderToTexture);
	
	crImage *getImage(int i);
	crTextureCubeMap *getCubeTexture();
	
	virtual void computeMVP( /*const crVector3 & eyePoint = crVector3(0.0,0.0,0.0)*/ );
	virtual void init(float bias = 0.0f);
	virtual inline void update(const crVector3 & eyePoint) {}
	virtual inline bool culled(crPolytope& view_frustum) { return false; }

	//virtual void updateUniform(/*crViewPort *sceneViewPort*/);
	//virtual void swapBuffers();
	virtual void releaseObjects(crState* = 0);
	virtual void traverse(crNodeVisitor& nv);

protected:
	virtual ~crRenderToCubeTexture() {}
	virtual void createCamera();
	int          m_gainCubeImageMask;
	crMatrixd     m_projection[6];
	crMatrixd     m_view[6];
	ref_ptr<crImage> m_image[6];
	ref_ptr<crTextureCubeMap> m_cubeTexture;
	ref_ptr<crCameraNode> m_camera[6];
};


}

#endif
