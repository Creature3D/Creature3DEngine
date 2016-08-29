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
#ifndef CRENCAPSULATION_CRNODECALLBACKS_H
#define CRENCAPSULATION_CRNODECALLBACKS_H 1

#include <CREncapsulation/crExport.h>
#include <CRCore/crNodeCallback.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crLightModel.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crTexMat.h>
#include <CRCore/crDrawable.h>
#include <CRCore/crDOFTransform.h>
#include <CRUtil/crCullVisitor.h>
#include <CRUtil/crUpdateVisitor.h>
#include <CRParticle/crParticleEffect.h>
#include <CRText/crText.h>
#include <rbody/Creature3D/CreBody.h>
#include <CRTerrain/crTerrainTile.h>
#include <CRCore/crShapeDrawable.h>
#include <CRIOManager/crLoadManager.h>
#include <CRCore/crLineWidth.h>
#include <CRCore/crPolygonMode.h>
#include <string>
#include <vector>

namespace CREncapsulation {
////////////////////////////////
//crCreBodyCullCallback
//CullCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crCreBodyCullCallback : public CRCore::crNodeCallback
{
public:
	crCreBodyCullCallback();
	crCreBodyCullCallback(const crCreBodyCullCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, CreBodyCull)
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
	void setFineFactor(float factor);
protected:
	float m_fineFactor;//精细度，0最高
	int m_nextUpdateFrame;
	int m_previousTraversalNumber;
};
////////////////////////////////
//crDuplicateCallback
//CullCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crDuplicateCallback : public CRCore::crNodeCallback
{
public:
	crDuplicateCallback();
	crDuplicateCallback(const crDuplicateCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, Duplicate)
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
	typedef std::vector< CRCore::crMatrix > MatrixVec;
	MatrixVec &getMatrixVec();
	void setDuplicateCount(int count);
	int getDuplicateCount();
protected:
	MatrixVec m_matrixVec;
	int m_duplicateCount;
	CRCore::crVector3 m_readTemp;
};
////////////////////////////////
//crReflectMaterialCallback
//CullCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crGndReflectMaterialCallback : public CRCore::crNodeCallback
{
public:
	crGndReflectMaterialCallback();
	crGndReflectMaterialCallback(const crGndReflectMaterialCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, GndReflectMaterial)
	void init();
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
	virtual void swapBuffers(int frameNumber);
	inline void setGroundOffset(float offset) { m_groundOffset = offset; }
	inline void setEnable(bool enable) { m_enable = enable; }
protected:
	CRCore::ref_ptr<CRCore::crStateSet> m_stateset2;//-7 RenderBinNum
	CRCore::ref_ptr<CRCore::crStateSet> m_stateset2_buf;
	float m_groundOffset;
	bool m_init;
	bool m_enable;
};

////////////////////////////////
//crMirrorCallback
//CullCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crMirrorCallback : public CRCore::crNodeCallback
{
public:
	crMirrorCallback();
	crMirrorCallback(const crMirrorCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, Mirror)
	void init();
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
protected:
	CRCore::ref_ptr<CRCore::crStateSet> m_stateset0;//-9
	CRCore::ref_ptr<CRCore::crStateSet> m_stateset1;//-8 RenderBinNum
	CRCore::ref_ptr<CRCore::crStateSet> m_stateset3;//-1 RenderBinNum
	float m_intensity;
	bool m_init;
};
////////////////////////////////
//crRenascenceCallback
//UpdateCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crRenascenceCallback : public CRCore::crNodeCallback
{
public:
	crRenascenceCallback();
	crRenascenceCallback(const crRenascenceCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, Renascence)
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv);
protected:
	double m_startFrameTime;
	float m_interval;
	CRCore::ref_ptr<CRCore::crDrawable> m_dead;
	CRCore::ref_ptr<CRCore::crDrawable> m_renascence;
};

////////////////////////////////
//crLightLumRandomCallback
//CullCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crLightLumRandomCallback : public CRCore::crNodeCallback
{
public:
	crLightLumRandomCallback();
	crLightLumRandomCallback(const crLightLumRandomCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, LightLumRandom)
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv);
	void setInterval(float interval);
	void setLumRange(float min, float max);
protected:
	CRCore::rangef m_lumRange;
	double m_lastFrameTime;
	float m_interval;
};
////////////////////////////////
//crLightPositionRandomCallback
//CullCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crLightPositionRandomCallback : public CRCore::crNodeCallback
{
public:
	crLightPositionRandomCallback();
	crLightPositionRandomCallback(const crLightPositionRandomCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, LightPositionRandom)
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv);
	void setInterval(float interval);
	void setRadiusRange(float radius);
protected:
	bool m_init;
	CRCore::crVector3f m_origPos;
	CRCore::rangef m_radiusRange;
	double m_lastFrameTime;
	float m_interval;
};
////////////////////////////////////
//////crSunSimulationCallback
//////CullCallback
///////////////////////////////////////
//class CRENCAPSULATION_EXPORT crSunSimulationCallback : public CRCore::crNodeCallback
//{
//public:
//	crSunSimulationCallback();
//	crSunSimulationCallback(const crSunSimulationCallback& callback,const CRCore::crCopyOp&copyop);
//	META_EventCallback(CREncapsulation, SunSimulation)
//	virtual void addEventParam(int i, const std::string &str);
//	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv);
//protected:
//	float m_rotSpeed;
//	float m_currentRot;
//	CRCore::crMatrix m_sunRotMatrix;
//};
////////////////////////////////
//crSunInEyesightCallback
//CullCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crSunInEyesightCallback : public CRCore::crNodeCallback
{
public:
	crSunInEyesightCallback():m_init(false){}
	crSunInEyesightCallback(const crSunInEyesightCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, SunInEyesight)
	void init();
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv);
	virtual void swapBuffers(int frameNumber);
protected:
	CRCore::crVector3 m_sunPos;
	CRCore::crVector3 m_sunDir;
	CRCore::crVector4f m_ambientIntensity;
	CRCore::ref_ptr<CRCore::crLightModel> m_lightModel;
	CRCore::ref_ptr<CRCore::crLightModel> m_lightModel_buf;
	CRCore::ref_ptr<CRCore::crStateSet> m_stateset;
	CRCore::ref_ptr<CRCore::crStateSet> m_stateset_buf;
	float m_scale;
	bool m_init;
};

////////////////////////////////
//crSunFlaresCallback
//UpdateCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crSunFlaresCallback : public CRCore::crNodeCallback
{
public:
	crSunFlaresCallback(){}
	crSunFlaresCallback(const crSunFlaresCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, SunFlares)
	typedef std::vector<float> DistanceRateVec;
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv);
protected:
	CRCore::crVector3 m_sunPos;
	float m_distance;
	float m_scale;
	DistanceRateVec m_distanceRateVec;
};

////////////////////////////////
//crMoveBillboardWithEyeCallback
//UpdateCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crMoveBillboardWithEyeCallback : public CRCore::crNodeCallback
{////////////////////////////////////////
public:
	crMoveBillboardWithEyeCallback(){}
	crMoveBillboardWithEyeCallback(const crMoveBillboardWithEyeCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, MoveBillboardWithEye)
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv);
protected:
	CRCore::crVector3 m_referencePos;
};

////////////////////////////////
//crDofTexMatCallback
//CullCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crDofTexMatCallback : public CRCore::crNodeCallback
{
public:
	crDofTexMatCallback();
	crDofTexMatCallback(const crDofTexMatCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, DofTexMat)
	CRCore::crDOFTransform *getDofTransform();
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
protected:
	CRCore::ref_ptr<CRCore::crDOFTransform> m_dof;
	int m_previousTraversalNumber;
};

////////////////////////////////
//crDofUniformTexMatCallback
//CullCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crDofUniformTexMatCallback : public CRCore::crNodeCallback
{
public:
	crDofUniformTexMatCallback();
	crDofUniformTexMatCallback(const crDofUniformTexMatCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, DofUniformTexMat)
	CRCore::crDOFTransform *getDofTransform();
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
	const std::string& getUniformID()const;
protected:
	std::string m_uniformID;
	CRCore::ref_ptr<CRCore::crDOFTransform> m_dof;
	int m_previousTraversalNumber;
};
class CRENCAPSULATION_EXPORT crUniformParam1SinCallback : public CRCore::crNodeCallback
{
public:
	crUniformParam1SinCallback();
	crUniformParam1SinCallback(const crUniformParam1SinCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, UniformParam1Sin)
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
protected:
	std::string m_uniformID;
	int m_previousTraversalNumber;
	float m_scale;
};
class CRENCAPSULATION_EXPORT crUniformParam1CosCallback : public CRCore::crNodeCallback
{
public:
	crUniformParam1CosCallback();
	crUniformParam1CosCallback(const crUniformParam1CosCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, UniformParam1Cos)
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
protected:
	std::string m_uniformID;
	int m_previousTraversalNumber;
	float m_scale;
};
class CRENCAPSULATION_EXPORT crUniformParam1RndCallback : public CRCore::crNodeCallback
{
public:
	crUniformParam1RndCallback();
	crUniformParam1RndCallback(const crUniformParam1RndCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, UniformParam1Rnd)
		virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
protected:
	std::string m_uniformID;
	int m_previousTraversalNumber;
	float m_scale;
};
class CRENCAPSULATION_EXPORT crUniformParam3RndCallback : public CRCore::crNodeCallback
{
public:
	crUniformParam3RndCallback();
	crUniformParam3RndCallback(const crUniformParam3RndCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, UniformParam3Rnd)
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
protected:
	std::string m_uniformID;
	int m_previousTraversalNumber;
	float m_scale;
};
class CRENCAPSULATION_EXPORT crUniformParam4RndCallback : public CRCore::crNodeCallback
{
public:
	crUniformParam4RndCallback();
	crUniformParam4RndCallback(const crUniformParam4RndCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, UniformParam4Rnd)
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
protected:
	std::string m_uniformID;
	int m_previousTraversalNumber;
	float m_scale;
};
//////////////////////////////////
////crWaveBlendCallback
////CullCallback
/////////////////////////////////////
//class CRENCAPSULATION_EXPORT crWaveBlendCallback : public CRCore::crNodeCallback
//{//不需要注册
//public:
//	crWaveBlendCallback()/*:m_isRect(false)*/{}
//	crWaveBlendCallback(const crWaveBlendCallback& callback);
//	//META_EventCallback(CREncapsulation, WaveBlend)
//	//virtual void addEventParam(int i, const std::string &str);
//	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv);
//	static void registFinalStaticSet(CRCore::crStateSet *stateset);
//protected:
//	static CRCore::crStateSet* m_finalStateset;
//	//bool m_isRect;
//};
//
//////////////////////////////////
////crResetWaveBlendCallback
////UpdateCallback
/////////////////////////////////////
//class CRENCAPSULATION_EXPORT crResetWaveBlendCallback : public CRCore::crNodeCallback
//{//不需要注册
//public:
//	crResetWaveBlendCallback(){}
//	crResetWaveBlendCallback(const crResetWaveBlendCallback& callback);
//	//META_EventCallback(CREncapsulation, ResetWaveBlend)
//	virtual void addEventParam(int i, const std::string &str);
//	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv);
//	static void registFinalStaticSet(CRCore::crStateSet *stateset);
//protected:
//	static CRCore::crStateSet* m_finalStateset;
//	CRCore::crVector4 m_waveRect;
//};
//
//////////////////////////////////
////crDetectCallback
////UpdateCallback
/////////////////////////////////////
//class CRENCAPSULATION_EXPORT crDetectCallback : public CRCore::crNodeCallback
//{//不需要注册 node update callback
//public:
//	crDetectCallback(){}
//	crDetectCallback(const crDetectCallback& callback);
//	//META_EventCallback(CREncapsulation, Detect)
//	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv);
//protected:
//	virtual ~crDetectCallback(){}
//};
//////////////////////////////////
////crParticleBindCallback
////CullCallback
/////////////////////////////////////
//class CRENCAPSULATION_EXPORT crParticleBindCallback : public CRCore::crNodeCallback
//{
//public:
//	crParticleBindCallback(){}
//	crParticleBindCallback(const crParticleBindCallback& callback);
//	META_EventCallback(CREncapsulation, ParticleBind)
//	virtual void addEventParam(int i, const std::string &str);
//	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv);
//protected:
//	std::string m_effectName;
//	CRCore::ref_ptr<CRParticle::crParticleEffect> m_particleEffectNode;
//};
////////////////////////////////
//crSequenceBindCallback
//CullCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crSequenceBindCallback : public CRCore::crNodeCallback
{
public:
	crSequenceBindCallback(){}
	crSequenceBindCallback(const crSequenceBindCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, SequenceBind)
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv);
protected:
	std::string m_effectName;
	CRCore::ref_ptr<CRCore::crSequence> m_sequenceEffectNode;
};

////////////////////////////////
//crVisableTimerCallback
//UpdateCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crVisableTimerCallback : public CRCore::crNodeCallback
{
public:
	crVisableTimerCallback();
	crVisableTimerCallback(const crVisableTimerCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, VisableTimer)
	void setInterval(float interval);
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv);
protected:
	int m_state;
	double m_startFrameTime;
	float m_interval;
};
////////////////////////////////
//crUnloadTimerCallback
//UpdateCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crUnloadTimerCallback : public CRCore::crNodeCallback
{
public:
	crUnloadTimerCallback();
	crUnloadTimerCallback(const crUnloadTimerCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, UnloadTimer)
	void setInterval(float interval);
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv);
protected:
	float m_interval;
	float m_time;
};
////////////////////////////////
//crLightEnableTimerCallback
//UpdateCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crLightEnableTimerCallback : public CRCore::crNodeCallback
{
public:
	crLightEnableTimerCallback();
	crLightEnableTimerCallback(const crLightEnableTimerCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, LightEnableTimer)
	void setInterval(float interval);
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv);
protected:
	int m_state;
	double m_startFrameTime;
	float m_interval;
};

////////////////////////////////
//crFlashTimerCallback
//UpdateCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crFlashTimerCallback : public CRCore::crNodeCallback
{
public:
	crFlashTimerCallback();
	crFlashTimerCallback(const crFlashTimerCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, FlashTimer)
	void setInterval(float interval);
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv);
protected:
	double m_lastFrameTime;
	float m_interval;
};

////////////////////////////////
//crBoneMatrixCallback
//UpdateCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crBoneMatrixCallback : public CRCore::crNodeCallback
{
public:
	crBoneMatrixCallback();
	crBoneMatrixCallback(const crBoneMatrixCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, BoneMatrix)
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv);
protected:
	std::string m_boneName;
	rbody::CreBodyNode* m_character;
};

////////////////////////////////
//crCompassHCallback
//CullCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crCompassHCallback : public CRCore::crNodeCallback
{
public:
	crCompassHCallback();
	crCompassHCallback(const crCompassHCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, CompassH)
	void init();
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
	virtual void swapBuffers(int frameNumber);
protected:
	std::string m_textName;
	CRCore::ref_ptr<CRText::crText> m_text;
	CRCore::ref_ptr<CRCore::crTexMat> m_texMat;
	CRCore::ref_ptr<CRCore::crTexMat> m_texMat_buf;
	CRCore::ref_ptr<CRCore::crStateSet> m_stateset;
	CRCore::ref_ptr<CRCore::crStateSet> m_stateset_buf;
	bool m_init;
	float m_scale;
};
////////////////////////////////
//crCompassPCallback
//CullCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crCompassPCallback : public CRCore::crNodeCallback
{
public:
	crCompassPCallback();
	crCompassPCallback(const crCompassPCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, CompassP)
	void init();
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
	virtual void swapBuffers(int frameNumber);
protected:
	std::string m_textName;
	CRCore::ref_ptr<CRText::crText> m_text;
	CRCore::ref_ptr<CRCore::crTexMat> m_texMat;
	CRCore::ref_ptr<CRCore::crTexMat> m_texMat_buf;
	CRCore::ref_ptr<CRCore::crStateSet> m_stateset;
	CRCore::ref_ptr<CRCore::crStateSet> m_stateset_buf;
	bool m_init;
	float m_scale;
};
////////////////////////////////
//crCompassRCallback
//UpdateCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crCompassRCallback : public CRCore::crNodeCallback
{
public:
	crCompassRCallback();
	crCompassRCallback(const crCompassRCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, CompassR)
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
};
////////////////////////////////
//crScalarBarCallback
//CullCallback
///////////////////////////////////
class CRENCAPSULATION_EXPORT crScalarBarCallback : public CRCore::crNodeCallback
{
public:
	crScalarBarCallback();
	crScalarBarCallback(const crScalarBarCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, ScalarBar)
	enum Orientation
	{
		HORIZONTAL, //(x-axis)
		VERTICAL    //(y-axis)
	};
	void init();
	void setOrientation(crScalarBarCallback::Orientation orientation);
	inline void updateRate(float rate){ m_rate = CRCore::minimum(1.0f,CRCore::maximum(0.0f,rate)); }
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
	virtual void swapBuffers(int frameNumber);
protected:
	Orientation m_orientation;
	float m_rate;
	CRCore::ref_ptr<CRCore::crTexMat> m_texMat;
	CRCore::ref_ptr<CRCore::crTexMat> m_texMat_buf;
	CRCore::ref_ptr<CRCore::crStateSet> m_stateset;
	CRCore::ref_ptr<CRCore::crStateSet> m_stateset_buf;
	bool m_init;
};
//////////////////////////////////
////crFlyGameViewFilterCallback
////CullCallback
/////////////////////////////////////
//class CRENCAPSULATION_EXPORT crFlyGameViewFilterCallback : public CRCore::crNodeCallback
//{
//public:
//	crFlyGameViewFilterCallback();
//	crFlyGameViewFilterCallback(const crFlyGameViewFilterCallback& callback);
//	META_EventCallback(CREncapsulation, FlyGameViewFilter)
//	void init();
//	virtual void addEventParam(int i, const std::string &str);
//	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
//	void setShow(int show);
//    void setRadarHeight(float radarHeight);
//	inline float getRadarHeight(){ return m_radarHeight; }
//	typedef std::map<std::string,int> StringIDMap;//target type string, moviesid
//protected:
//	bool m_init;
//	int m_show;
//	float m_time_info;
//	float m_interval_info;
//	//float m_previousFrameTime;
//	CRCore::ref_ptr<CRText::crText> m_time;
//	CRCore::ref_ptr<CRText::crText> m_alt;//高度
//	CRCore::ref_ptr<CRText::crText> m_speed;
//	CRCore::ref_ptr<CRText::crText> m_score;
//	CRCore::ref_ptr<CRText::crText> m_level;
//	CRCore::ref_ptr<CRText::crText> m_hp;
//	CRCore::ref_ptr<CRText::crText> m_defend;
//	CRCore::ref_ptr<CRText::crText> m_weaponName;
//	CRCore::ref_ptr<CRText::crText> m_bulletCount;
//	CRCore::ref_ptr<CRText::crText> m_targetDistance[10];
//	CRCore::ref_ptr<CRText::crText> m_targetName[10];
//	CRCore::ref_ptr<CRText::crText> m_targetType[10];
//	
//	CRCore::ref_ptr<CRText::crText> m_compassH;
//
//	///////////target
//	CRCore::ref_ptr<CRCore::crNode> m_lockedTarget_air;
//	CRCore::ref_ptr<CRCore::crNode> m_lockedOverTarget_air;
//	CRCore::ref_ptr<CRCore::crNode> m_trackTarget_air;
//	
//	CRCore::ref_ptr<CRCore::crNode> m_lockedTarget_gnd;
//	CRCore::ref_ptr<CRCore::crNode> m_lockedOverTarget_gnd;
//	CRCore::ref_ptr<CRCore::crNode> m_trackTarget_gnd;
//
//	CRCore::ref_ptr<CRCore::crNode> m_invalidTarget_air;
//	CRCore::ref_ptr<CRCore::crNode> m_invalidTarget_gnd;
//
//	CRCore::ref_ptr<CRCore::crNode> m_friendTarget_air;
//	CRCore::ref_ptr<CRCore::crNode> m_friendTarget_gnd;
//    
//	CRCore::ref_ptr<CRCore::crGroup> m_targetGroup;
//
//	std::string m_lastWeaponName;
//    bool m_useWeaponEachTarget;
//	/////////////////////
//	CRCore::ref_ptr<CRCore::crNode> m_hpGuage;
//	CRCore::ref_ptr<CRCore::crNode> m_bulletGuage;
//	CRCore::ref_ptr<CRCore::crMatrixTransform> m_radar;///
//	CRCore::ref_ptr<CRCore::crGroup> m_radarGroup;///
//	CRCore::ref_ptr<CRCore::crNode> m_friendAir;
//	CRCore::ref_ptr<CRCore::crNode> m_friendAir_jamming;//
//	CRCore::ref_ptr<CRCore::crNode> m_friendGround;
//	CRCore::ref_ptr<CRCore::crNode> m_friendBuild;
//	CRCore::ref_ptr<CRCore::crNode> m_friendBuild_jamming;
//	CRCore::ref_ptr<CRCore::crNode> m_neutralAir;
//	CRCore::ref_ptr<CRCore::crNode> m_neutralGround;
//	CRCore::ref_ptr<CRCore::crNode> m_neutralBuild;
//	CRCore::ref_ptr<CRCore::crNode> m_enemyAir;
//	CRCore::ref_ptr<CRCore::crNode> m_enemyAir_jamming;
//	CRCore::ref_ptr<CRCore::crNode> m_enemyGround;
//	CRCore::ref_ptr<CRCore::crNode> m_enemyBuild;
//	CRCore::ref_ptr<CRCore::crNode> m_enemyBuild_jamming;
//
//	CRCore::ref_ptr<CRCore::crMultiSwitch> m_cautionSW;
//    CRCore::ref_ptr<CRCore::crMultiSwitch> m_viewFilterSwitch;
//	CRCore::ref_ptr<CRCore::crMultiSwitch> m_targetMovies;
//    StringIDMap m_stringidMap;
//
//	CRCore::crMatrix m_radarProjectionMatrix;
//	CRCore::crMatrix m_radarViewMatrix;
//	CRCore::crMatrix m_identicMVPW;
//	CRCore::crPolytope m_radarFrustum;
//	float m_radarHeight;
//
//	typedef std::vector< CRCore::ref_ptr<CRCore::crMatrixTransform> > MatrixTransformVec;
//	MatrixTransformVec m_matrixTransformBuf;
//	CRCore::crVector3 m_targetDistancePos;
//	CRCore::crVector3 m_targetNamePos;
//	CRCore::crVector3 m_targetTypePos;
//	inline CRCore::crMatrixTransform *getOrCreateIdleMatrixTransform()
//	{
//		CRCore::crMatrixTransform *idle = 0;
//		for( MatrixTransformVec::iterator itr = m_matrixTransformBuf.begin();
//			itr != m_matrixTransformBuf.end();
//			++itr )
//		{
//			if((*itr)->referenceCount()==1)
//			{
//				idle = itr->get();
//				break;
//			}
//		}
//
//		if(!idle)
//		{
//			idle = new CRCore::crMatrixTransform;
//			m_matrixTransformBuf.push_back(idle);
//		}
//		idle->removeChild(0,idle->getNumChildren());
//		return idle;
//	}
//};

//////////////////////////////////
////crTerrainDecalCallback
////UpdateCallback
/////////////////////////////////////
//class CRENCAPSULATION_EXPORT crTerrainDecalCallback : public CRCore::crNodeCallback
//{//1,材质编辑问题 2.保存读取问题 3.高度与地形偏差问题
//public:
//	crTerrainDecalCallback();
//	crTerrainDecalCallback(const crTerrainDecalCallback& callback);
//	META_EventCallback(CREncapsulation, TerrainDecal)
//	virtual void addEventParam(int i, const std::string &str);
//	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
//protected:
//	int m_frameNumber;
//	float m_width;
//	float m_height;
//	float m_offset;
//	CRCore::ref_ptr<CRTerrain::crTerrainTile> m_terrain;
//	CRCore::ref_ptr<CRCore::crHeightField> m_heightField;
//	CRCore::ref_ptr<CRCore::crShapeDrawable> m_shapeDrawable;
//	CRCore::ref_ptr<CRCore::crObject> m_object;
//};
////////////////////////////////
//crSeqTextureTileCallback
//UpdateCallback 只能对只有一个四边形的drawable的object使用，用于播放序列帧图片阵列
///////////////////////////////////
class CRENCAPSULATION_EXPORT crSeqTextureTileCallback : public CRCore::crNodeCallback
{
public:
	crSeqTextureTileCallback();
	crSeqTextureTileCallback(const crSeqTextureTileCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CREncapsulation, SeqTextureTile)
	CRCore::crSequence *getSequence();
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
	virtual void swapBuffers(int frameNumber);
	int getTileS() const;
	int getTileT() const;
	inline int getNumTiles() const { return m_num_tile; }
	void setTextureTile(int sTile, int tTile, int numTiles = 0);
protected:
	CRCore::ref_ptr<CRCore::crSequence> m_seq;
	int m_previousTraversalNumber;
	float m_s_tile;
	float m_t_tile;
	int m_num_tile;
	int m_cur_tile;
	float m_s_coord;
	float m_t_coord;
	CRCore::crObject* m_object;
	CRCore::crGeometry* m_drawable;
};
}
#endif

