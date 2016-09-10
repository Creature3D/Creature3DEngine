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
#ifndef CRENCAPSULATION_CRDATACLASS_H
#define CRENCAPSULATION_CRDATACLASS_H 1

#include <CREncapsulation/crExport.h>
#include <CRCore/crHandleManager.h>
#include <CRCore/crDataManager.h>
#include <CRCore/crDrawable.h>
#include <vector>
#include <string>

#define EDP_ -1
#define EDP_SelectType 0
#define EDP_TransformNodeMode 1
#define EDP_PanMode 2
#define EDP_RotMode 3
#define EDP_ScaleMode 4
#define EDP_TransformNodeSpeed 5
#define EDP_SelectClassName 6
#define EDP_CurrentSelectDrawable 7
#define EDP_CanMove 8
#define EDP_TerEditMode 9
#define EDP_TerTools 10
#define EDP_CurrentTerTool 11
#define EDP_TerToolRadius 12
#define EDP_TerToolHeight 13
#define EDP_CurrentFocusTexture 14
#define EDP_CurrentEditorMenu 15
#define EDP_FreezeProxySet 16
#define EDP_SceneModified 17
#define Edit_CopiedParticle 100
namespace CREncapsulation{
class CRENCAPSULATION_EXPORT crEditorInfoData : public CRCore::crData
{
public:
	crEditorInfoData();
	crEditorInfoData(const crEditorInfoData& data);
    DataClass(CREncapsulation, EditorInfo)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	typedef std::vector<std::string> StrVec;
	typedef std::vector< CRCore::ref_ptr<CRCore::crImage> >TerToolVec;
	void init();
	enum TerEditMode
	{
		TER_Pick = 0,
		TER_Raise,
		TER_Lower,
		TER_Lerp,
        TER_ControlR,
		TER_ControlG,
		TER_ControlB,
		TER_ControlA,
	};
	typedef std::set< CRCore::ref_ptr<CRCore::crGroup> > FreezeProxySet;
protected:
    int m_selectType;//-3:PickNeedLight -2:PickChild -1:PickMaterial 0:ProxyNode, 1:PickObject, 2:PickMesh, >=3:ClassName, 21:PickEditorNode, 22:PickWaypoint
	int m_transformNodeMode;//0:Pick, 1:Pan, 2:Rotate, 3:Scale
	int m_panMode;//0:PanXY, 1:PanX, 2:PanY, 3:PanZ
	int m_rotMode;//0:RotX, 1:RotY, 2:RotZ
	int m_scaleMode;//0:ScaleAll, 1:ScaleX, 2:ScaleY, 3:ScaleZ
	//int m_pickMode;//0:NormalPick, 1:PickMaterial
	float m_transformNodeSpeed;
    int m_canmove;
	StrVec m_selectClassName;
	CRCore::ref_ptr<CRCore::crDrawable> m_currentSelectDrawable;
	//地形高度图编辑模式
	unsigned char m_terEditMode;//TerEditMode
	TerToolVec m_terTools;
	unsigned char m_currentTerTool;
	int m_terToolRadius;
	float m_terToolHeight;
	std::string m_currentFocusTexture;
	std::string m_currentEditorMenu;
	FreezeProxySet m_freezeProxySet;
	bool m_sceneModified;
	CRCore::ref_ptr<CRParticle::crParticleEffect> m_copiedParticle;
};

class AddColorOperator
{
public:
	inline void luminance(float& l) const 
	{ 
		l+=m_l; 
		l = CRCore::clampTo(l,0.0f,1.0f); 
	}
	inline void alpha(float& a) const {} 
	inline void luminance_alpha(float& l,float& a) const {} 
	inline void rgb(float& r,float& g,float& b) const {}
	inline void rgba(float& r,float& g,float& b,float& a) const {}
	void setLuminance(float l) { m_l = l; }
protected:
	float m_l;
};

class LerpColorOperator
{
public:
	inline void luminance(float& l) const { l = CRCore::lerp(l,m_pickl,m_tooll); }
	inline void alpha(float& a) const {} 
	inline void luminance_alpha(float& l,float& a) const {} 
	inline void rgb(float& r,float& g,float& b) const {}
	inline void rgba(float& r,float& g,float& b,float& a) const {}
	void setPickl(float pickl) { m_pickl = pickl; }
	void setTooll(float tooll) { m_tooll = tooll; }
protected:
	float m_pickl;
	float m_tooll;
};


class ColorWeightOperator
{
public:
	inline void luminance(float& l) const {}
	inline void alpha(float& a) const {} 
	inline void luminance_alpha(float& l,float& a) const {} 
	inline void rgb(float& r,float& g,float& b) const {}
	inline void rgba(float& r,float& g,float& b,float& a) const 
	{
		CRCore::crVector4 color(r,g,b,a);
		color = CRCore::lerp(color,color+m_weights,m_tooll);
		r = color[0];
		g = color[1];
		b = color[2];
		a = color[3];
        r = CRCore::clampTo(r,0.0f,1.0f);
		g = CRCore::clampTo(g,0.0f,1.0f);
		b = CRCore::clampTo(b,0.0f,1.0f);
		a = CRCore::clampTo(a,0.0f,1.0f);
        float l = r+g+b+a;
		r /= l;
		g /= l;
		b /= l;
		a /= l;
	}
	void setWeight(const CRCore::crVector4& weights) { m_weights = weights; }
	void setTooll(float tooll) { m_tooll = tooll; }
protected:
	CRCore::crVector4 m_weights;
	float m_tooll;
};

}
#endif