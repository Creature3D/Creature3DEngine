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
#ifndef CRNETAPP_SINGLEGAMELOGICCLASS_H
#define CRNETAPP_SINGLEGAMELOGICCLASS_H 1

#include <CRNetApp/appExport.h>
#include <CRNetApp/appDataClass.h>
#include <CRCore/crHandleManager.h>
#include <CRGUI/cr2DStageManager.h>
#include <CRNetApp/appClientPlayerData.h>
#include <CRProducer/crGUIEventAdapter.h>
#include <CRCore/crLightSource.h>
//#include <CRAL/crSoundState.h>
#include <ode/ode.h>
#include <string>
#include <vector>
#include <list>

namespace CRNetApp{
class CRNETAPP_EXPORT crVR_ICBCMousePickLogic : public CRCore::crLogic
{//
public:
	crVR_ICBCMousePickLogic();
	crVR_ICBCMousePickLogic(const crVR_ICBCMousePickLogic& handle);
	LogicClass(CRNetApp, VR_ICBCMousePick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	crRole* m_this;
	CREPARAM m_param;
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::ref_ptr<CRCore::crHandle> m_pickTarget;
	unsigned char m_targetType;
	CRCore::ref_ptr<CRCore::crNode> m_targetNode;
	CRCore::crVector3 m_targetPosition;
};
class CRNETAPP_EXPORT crShowShelfUILogic : public CRCore::crLogic
{//Client Method
public:
	crShowShelfUILogic();
	crShowShelfUILogic(const crShowShelfUILogic& handle);
	LogicClass(CRNetApp, ShowShelfUI)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	std::map<std::string,int> m_nameid;
    CRCore::ref_ptr<CRCore::crHandle> m_filterSwitch;
};
}
#endif