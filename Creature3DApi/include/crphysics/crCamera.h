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
#ifndef CRPHYSICS_CRCAMERAMATTEROBJECT_H
#define CRPHYSICS_CRCAMERAMATTEROBJECT_H 1

#include <CRPhysics/crExport.h>
#include <CRCore/crNode.h>
#include <CRCore/crWidgetNode.h>
#include <CRCore/crIntersectVisitor.h>
namespace CRPhysics{

class CRPHYSICS_EXPORT crCamera : public CRCore::crBase
{
public:
	crCamera();
	crCamera(const crCamera&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Base(CRPhysics,crCamera)

	CRCore::crMatrix getCameraMatrix();
	CRCore::crVector3 getCameraDir();
	//enum KeyboardMouseMode
	//{
	//	KM_None = 0x0,
	//	KM_Keyboard = 0x1,
	//	KM_LeftMouse = 0x2,
	//	KM_MiddleMouse = 0x4,
	//	KM_RightMouse = 0x8,
	//	KM_Mouse = KM_LeftMouse | KM_MiddleMouse | KM_RightMouse,
	//	KM_AllBufRM = KM_Keyboard | KM_LeftMouse | KM_MiddleMouse,
	//	KM_All = KM_Keyboard | KM_Mouse,
	//};
    void setKeyboardMouseMode(unsigned short mode);
	unsigned short getKeyboardMouseMode();

	void setViewPitch(float viewPitch);
	void setViewRoll(float viewRoll);
	void setViewOffset(float viewOffset);
	void setViewHeight(float viewHeight);
	//void setViewOffsetRange( const CRCore::crVector2& range ) { m_viewOffsetRange = range; }
	void attachNode(CRCore::crNode *node);
	inline CRCore::crNode *getAttachedNode() { return m_attachedNode.get(); }
protected:
	CRCore::KeyboardMouseMode m_keyboardMouseMode;
	CRCore::ref_ptr<CRCore::crNode> m_attachedNode;
	float      m_viewPitch;
	float      m_viewRoll;
	float      m_viewHeight;
	float      m_viewOffset;
	//float      m_currentViewOffset;
	//float      m_viewVelocity;
	//CRCore::crVector2f m_viewOffsetRange;
	//CRCore::crIntersectVisitor               m_iv;
	//CRCore::ref_ptr<CRCore::crLineSegment>   m_lineSegment;
};

class CRPHYSICS_EXPORT crCameraManager : public CRCore::Referenced
{
public :
	crCameraManager();
	static crCameraManager *getInstance();

	typedef std::map< std::string, CRCore::ref_ptr<crCamera> > CameraMap;
	void removeCamera(crCamera *camera);
	crCamera *getCamera(const std::string &name);
	void addCamera(crCamera *camera);
	virtual void clear();
	void setCurrentCamera(crCamera *cam);
	crCamera *getCurrentCamera();
	crCamera *getMainCamera();
protected:
	virtual ~crCameraManager(){}
	static CRCore::ref_ptr<crCameraManager> m_instance;
	CameraMap m_cameraMap;
	CRCore::ref_ptr<crCamera> m_currentCamera;
	CRCore::ref_ptr<crCamera> m_mainCamera;
};
}
#endif