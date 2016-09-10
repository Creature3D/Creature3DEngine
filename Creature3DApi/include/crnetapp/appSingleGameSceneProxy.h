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
#ifndef CRNETAPP_SINGLEGAMESCENEPROXY_H
#define CRNETAPP_SINGLEGAMESCENEPROXY_H 1

#include <CRNetApp/appExport.h>
#include <CRNetApp/appNetGameData.h>
#include <CRUtil/crSceneView.h>
namespace CRNetApp{

class CRNETAPP_EXPORT crSingleGameSceneProxy : public CRUtil::crSceneView::ExternUpdater
{
public:
	crSingleGameSceneProxy();
	static crSingleGameSceneProxy *getInstance();
	void clear();
	
	void initScene();
	crScene *getScene();
	virtual void operator()(float dt);
protected:
	virtual ~crSingleGameSceneProxy(){}
	static CRCore::ref_ptr<crSingleGameSceneProxy> m_instance;

	CRCore::ref_ptr<crScene> m_scene;
	float m_reliveInterval;
	float m_reliveTime;
};

}
#endif