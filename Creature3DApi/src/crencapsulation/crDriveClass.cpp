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
#include <CRGUI/crDriveClass.h>
#include <CRGUI/crImageStage.h>
#include <CRGUI/cr2DStageManager.h>
#include <CRGUI/crComputePosENVisitor.h>
#include <CRGUI/crDrawTreeENVisitor.h>
#include <CRGUI/crElementNodeManager.h>
#include <CRGUI/crWindowContainer.h>

#include <CRAI/crAIGroup.h>
#include <CRAI/crKeyboardMouseEventGroup.h>
//#include <CRAI/crAI.h>
#include <CRProducer/crViewer.h>
#include <CRCore/crBrain.h>
#include <CRCore/crSearchNodeBYNameVisitor.h>
#include <CRAL/crSoundManager.h>
//#include <CRNet/crNetManager.h>
#include <CRPhysics/crBotManager.h>
#include <CRPhysics/crViewMatterObject.h>

#include "rcfg/ConfigScriptOO.h"
#include <rcfg/ItemMap.h>

#include <sys/stat.h>
#include <stdio.h>
using namespace CRGUI;
using namespace CRCore;

///////////////////////////////////////////
////
////crOneSlotDrive
////
///////////////////////////////////////////
//crOneSlotDrive::crOneSlotDrive(){}
//crOneSlotDrive::crOneSlotDrive(const crOneSlotDrive& handle):
//crDrive(handle)
//{
//}
//
//void crOneSlotDrive::inputHandle(int i, void *param)
//{
//	if(param!=NULL) 
//		m_handle = (CRCore::crHandle *)param;
//}
//void crOneSlotDrive::outputParam(int i, void *param)
//{
//}
//void crOneSlotDrive::getOutputParam(int i, void*& param)
//{
//}
//void crOneSlotDrive::inputParam(int i, void *param)
//{
//}
//void crOneSlotDrive::operator()(crData &data);
//{
//	(*m_handle)(*this);
//}
///////////////////////////////////////////
////
////crMoreSlotDrive
////
///////////////////////////////////////////
//void crMoreSlotDrive::operator()(crData &data)
//{
//	for( HandleVec::iterator itr = m_handleVec.begin();
//		 itr != m_handleVec.end();
//		 ++itr )
//	{
//		(*(itr->get()))(*this);
//	}
//}
//void crMoreSlotDrive::inputHandle(int i, void *param)
//{
//	if(param!=NULL) 
//        m_handleVec.insert((CRCore::crHandle *)param)
//}
//void crMoreSlotDrive::outputParam(int i, void *param)
//{
//}
//void crMoreSlotDrive::getOutputParam(int i, void*& param)
//{
//}
