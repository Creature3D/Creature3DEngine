/* Creature3D - Online Game Engine, Copyright (C) 2005 Îâ²Æ»ª(26756325@qq.com)
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
#ifndef CRENCAPSULATION_CROBJECTEVENTCALLBACK_H
#define CRENCAPSULATION_CROBJECTEVENTCALLBACK_H 1

#include <CREncapsulation/crExport.h>
#include <CRCore/crNode.h>
//#include <CRCore/crTimer.h>
//#include <CRCore/crObject.h>
#include <CRCore/crEventGroup.h>
//#include <CRCore/crLightSource.h>
//#include <CRPhysics/crMatterObject.h>
namespace CREncapsulation{

//class CRENCAPSULATION_EXPORT crShowMouseCallback : public CRCore::crEventCallback
//{
//public:
//	crShowMouseCallback();
//	crShowMouseCallback(const crShowMouseCallback& callback);
//	META_EventCallback(CREncapsulation, ShowMouse)
//	virtual void addEventParam(int i, const std::string &str);
//	virtual void operator()(void);
//protected:
//	int m_showMouse;
//};
//
//class CRENCAPSULATION_EXPORT crShowMousePickMsgCallback : public CRCore::crEventCallback
//{
//public:
//	crShowMousePickMsgCallback();
//	crShowMousePickMsgCallback(const crShowMousePickMsgCallback& callback);
//	META_EventCallback(CREncapsulation, ShowMousePickMsg)
//	virtual void addEventParam(int i, const std::string &str);
//	virtual void operator()(void);
//protected:
//	int m_showMousePicMsg;
//};
//
//class CRENCAPSULATION_EXPORT crLightSwitchCallback : public CRCore::crEventCallback
//{
//public:
//	crLightSwitchCallback();
//	crLightSwitchCallback(const crLightSwitchCallback& callback);
//	META_EventCallback(CREncapsulation, LightSwitch)
//	virtual void addEventParam(int i, const std::string &str);
//	virtual void operator()(void);
//protected:
//	typedef std::vector< CRCore::ref_ptr<CRCore::crLightSource> > LSVec;
//	LSVec m_lightSourceArray;
//};
//
//class CRENCAPSULATION_EXPORT crParticleEffectCallback : public CRCore::crObject::crObjectEvent::Callback
//{
//public:
//	crParticleEffectCallback();
//	crParticleEffectCallback(const crParticleEffectCallback& callback);
//	META_EventCallback(CREncapsulation, ParticleEffect)
//	virtual void addEventParam(int i, const std::string &str);
//	virtual void operator()(CRCore::crEventGroup &, const CRCore::crObject::crObjectEvent &, const CRCore::crVector3 &);
//protected:
//	std::string                     m_effectTypeString;
//};
//
//class CRENCAPSULATION_EXPORT crDrawableStateSetCallback : public CRCore::crObject::crObjectEvent::Callback
//{
//public:
//	crDrawableStateSetCallback();
//	crDrawableStateSetCallback(const crDrawableStateSetCallback& callback);
//	META_EventCallback(CREncapsulation, DrawableStateSet)
//	virtual void operator()(CRCore::crEventGroup &, const CRCore::crObject::crObjectEvent &, const CRCore::crVector3 &);
//protected:
//};
//
class CRENCAPSULATION_EXPORT crXTransTMDofCallback : public CRCore::crNode::PhysicsObjectCallback
{
public:
	crXTransTMDofCallback();
	crXTransTMDofCallback(const crXTransTMDofCallback& callback);
	META_EventCallback(CREncapsulation, XTransTMDof)
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode &node, _crInt64 param = 0);
protected:
	std::string m_nodeName;
	float m_min;
	float m_max;
	float m_step;
private:
	CRCore::ref_ptr<CRCore::crDOFTransform> m_dof;
};

class CRENCAPSULATION_EXPORT crYTransTMDofCallback : public CRCore::crNode::PhysicsObjectCallback
{
public:
	crYTransTMDofCallback();
	crYTransTMDofCallback(const crYTransTMDofCallback& callback);
	META_EventCallback(CREncapsulation, YTransTMDof)
	virtual void addEventParam(int i, const std::string &str);
	virtual void operator()(CRCore::crNode &node, _crInt64 param = 0);
protected:
	std::string m_nodeName;
	float m_min;
	float m_max;
	float m_step;
private:
	CRCore::ref_ptr<CRCore::crDOFTransform> m_dof;
};
}
#endif