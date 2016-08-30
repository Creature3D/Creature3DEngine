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
#ifndef CRNETAPP_NODECALLBACKS_H
#define CRNETAPP_NODECALLBACKS_H 1

#include <CRNetApp/appExport.h>
#include <CRCore/crNode.h>
#include <CRNetApp/appNetGameData.h>
namespace CRNetApp{
////////////////////////////////
//crDecalUpdateCallback
//UpdateCallback
///////////////////////////////////
class CRNETAPP_EXPORT crDecalUpdateCallback : public CRCore::crNodeCallback
{
public:
	crDecalUpdateCallback();
	crDecalUpdateCallback(const crDecalUpdateCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CRNetApp, DecalUpdate)
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
	void setPosition(const CRCore::crVector2 &pos);
	void setSize(const CRCore::crVector2 &size);
	void dirty();
	void setColor(const CRCore::crVector4 &color);
	void setVisiable(bool visiable);
protected:
	CRCore::crVector2 m_position;
	CRCore::crVector2 m_size;
	CRCore::crVector4 m_color;
	bool m_dirty;
	bool m_visiable;
};
////////////////////////////////
//crAutoTransCallback
//CullCallback
///////////////////////////////////
class CRNETAPP_EXPORT crAutoTransCallback : public CRCore::crNodeCallback
{
public:
	crAutoTransCallback();
	crAutoTransCallback(const crAutoTransCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CRNetApp, AutoTrans)
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);
	void setAutoTrans(float interval,float minalpha);
	void setSpeed(float speed);
protected:
	float m_minalpha;
	float m_alpha;
	float m_interval;//隐身持续时间，时间到后恢复到原值
	float m_speed;//变化速度
	int m_previousTraversalNumber;
	CRCore::ref_ptr<CRCore::crStateSet> m_stateset;
	CRCore::ref_ptr<CRCore::crStateSet> m_objstateset;
};
////////////////////////////////
//crActFxCallback
//UpdateCallback
///////////////////////////////////
class CRNETAPP_EXPORT crActFxCallback : public CRCore::crNodeCallback
{//当m_actitem的WCHDATA_CurActState != m_act时特效结束
public:
	crActFxCallback();
	crActFxCallback(const crActFxCallback& callback,const CRCore::crCopyOp&copyop);
	META_EventCallback(CRNetApp, ActFx)
	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv);
	void setActInstanceItem(crInstanceItem *item);
	void setAct(unsigned short act);
	void setVisiableOrUnload(bool flg);
protected:
	crInstanceItem* m_actitem;
	unsigned short m_act;
	bool m_visiableOrUnload;//true表示visiable
};
}
#endif