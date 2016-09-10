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
#include <CREncapsulation/crLoader.h>
#include <CREncapsulation/crStartHandler.h>
//#include <CREncapsulation/crAIMoviesParsePPC.h>
#include <CREncapsulation/crMethodClass.h>
#include <CREncapsulation/crMethodClass2.h>
#include <CREncapsulation/crDataClass.h>
#include <CREncapsulation/crLogicClass.h>
#include <CRCore/crNodeVisitor.h>
#include <CRCore/crNodeCallback.h>
#include <CRCore/crGroup.h>
#include <CRCore/crObject.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crBrain.h>
#include <CRCore/crNotify.h>
#include <CRCore/crTextureCubeMap.h>
#include <CRCore/crLightSource.h>
#include <CRCore/crLight.h>
#include <CRCore/crProxyNode.h>
#include <CRCore/crSearchNodeBYNameVisitor.h>
#include <CRCore/crShapeDrawable.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crClipNode.h>
#include <CRCore/crClipPlane.h>
#include <CRCore/crTimer.h>
#include <CRCore/crDB.h>
#include <CRCore/crProgram.h>
#include <CRCore/crShader.h>
#include <CRCore/crUniform.h>
#include <CRCore/crCameraNode.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/crTraverseStringNode.h>
#include <CRCore/crTexGen.h>
#include <CRCore/crMatrix.h>
#include <CRCore/crMath.h>
#include <CRCore/crVector2.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crTexMat.h>
#include <CRCore/crProgram.h>
#include <CRCore/crShader.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crAnimationPath.h>
#include <CRCore/crEffectSequenceManager.h>
//#include <CRCore/crDoubleStateNode.h>
//#include <CRCore/crWaterSimulationNode.h>
//#include <CRCore/crWaterReflectNode.h>
//#include <CRCore/crDepthSortedObject.h>
#include <CRCore/crLightSourceManager.h>
#include <CRCore/crRenderToTexture.h>
#include <CRCore/crRenderToTextureManager.h>
#include <CRCore/crFilterRenderManager.h>
#include <CRCore/crDrawPixels.h>
#include <CRCore/crMaterialStateSetManager.h>
#include <CRCore/crIntersectVisitor.h>
#include <CRCore/crEndian.h>
#include <CRCore/crFog.h>
#include <CRCore/crPlane.h>
#include <CRCore/crQuat.h>
#include <CRCore/crDataManager.h>
#include <CRCore/crLineWidth.h>
#include <CRCore/crUniversalHandles.h>

//#include <CREncapsulation/crImageStreamManager.h>

#include <CRUtil/crOptimizer.h>
#include <CRUtil/crSmoothingVisitor.h>
#include <CRUtil/crSimplifier.h>
#include <CRUtil/crTesselator.h>
#include <CRUtil/crTangentSpaceGenerator.h>
#include <CRUtil/crNoise.h>
#include <CRUtil/crUpdateVisitor.h>

#include <CRPhysics/crMatterObject.h>
#include <CRPhysics/crMatterGroup.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRPhysics/crCharacterMatterObject.h>
#include <CRPhysics/crRadarMatterObject.h>
#include <CRPhysics/crJointSetParamPPC.h>
#include <CRPhysics/crSetForceAndTorquePPC.h>
#include <CRPhysics/crSetGravityModePPC.h>
#include <CRPhysics/crPhysicsResetPPC.h>
#include <CRPhysics/crSetGeomorBodyInfoPPC.h>
#include <CRPhysics/crGeneralParamTransferPPC.h>
#include <CRPhysics/crWeaponEventPPC.h>
#include <CRPhysics/crWeaponManager.h>
#include <CRPhysics/crWeaponNode.h>
#include <CRPhysics/crDetectCallback.h>
#include <CRPhysics/crBodyCaseMatterObject.h>

#include <CREncapsulation/crCollideCallback.h>
//#include <CREncapsulation/crEventCallbacks.h>

#include <CRCore/thread/crMutex.h>
#include <CRCore/thread/crBarrier.h>
#include <CRUtil/crPrepareVisitor.h>
//#include <CRFX/crSpecularHighlights.h>
//#include <CRFX/crCupMapping.h>
//#include <CRFX/crAnisotropicLighting.h>
//#include <CRFX/crBumpMapping.h>
//#include <CRFX/crCartoon.h>
//#include <CRFX/crScribe.h>

//#include <CRAI/crAIGroup.h>
//#include <CRAI/crKeyboardMouseEventGroup.h>
//#include <CRAI/crKeyboardMousePPC.h>
//#include <CRAI/crCarManipulatorPPC.h>
//#include <CRAI/crVisionSenseInformationParsePPC.h>
//#include <CRAI/crCharacterManipulatorPPC.h>
//#include <CRAI/crFlyManipulatorPPC.h>
//#include <CRAI/crObjectEventManager.h>
//#include <CRAI/crMoveTaskManager.h>

//#include <CRAL/crSoundManager.h>
////#include <CRAL/crSoundCollideCallback.h>
//#include <CRAL/crSoundEventPPC.h>
//#include <CRAL/crSoundRoot.h>

#include <CRParticle/crParticleSystemUpdater.h>
#include <CRParticle/crMultiSegmentPlacer.h>
#include <CRParticle/crSegmentPlacer.h>
#include <CRParticle/crSectorPlacer.h>
#include <CRParticle/crPointPlacer.h>
#include <CRParticle/crModularEmitter.h>
//#include <CRParticle/crEffectParticleManager.h>
#include <CRCore/crRange.h>
#include <CRParticle/crOperator.h>
#include <CRParticle/crAccelOperator.h>
#include <CRParticle/crAngularAccelOperator.h>
#include <CRParticle/crFluidFrictionOperator.h>
#include <CRParticle/crForceOperator.h>
#include <CRParticle/crParticleSystem.h>
#include <CRParticle/crParticle.h>
#include <CRParticle/crBoxPlacer.h>
#include <CRParticle/crConstantRateCounter.h>
#include <CRParticle/crFluidProgram.h>
#include <CRParticle/crRadialShooter.h>
#include <CRParticle/crParticleSystemUpdater.h>
#include <CRParticle/crConnectedParticleSystem.h>

#include <CRCore/crDOFTransform.h>

#include <rbody/Creature3D/CreBody.h>
#include <rvrutils/Property.h>
#include <rvrutils/utils.h>
#include <rvrutils/CodeLineException.h>
#include <cal3d/loader.h>
#include <rcfg/ItemMap.h>

#include <CRIOManager/crReadFile.h>
#include <CRIOManager/crWriteFile.h>
#include <CRIOManager/crFileutils.h>
#include <CRIOManager/crFileNameUtils.h>
#include <CRIOManager/crLoadManager.h>

#include <CRProducer/crViewer.h>
#include <CRProducer/crSceneHandler.h>

#include <CRText/crText.h>
#include <CRText/crFont.h>
#include <CRText/crText.h>
#include <CRText/crFont.h>

#include <CRCore/thread/crThread.h>

#include <CRCore/crVector2.h>

//#include <CREncapsulation/crDlgEventPPC.h>
//#include <CREncapsulation/crExitCarEventPPC.h>
//#include <CREncapsulation/crGeneralCharacterSoundEventPPC.h>
//#include <CREncapsulation/crGeneralCarSoundEventPPC.h>
//#include <CREncapsulation/crSoundManagerCallbacks.h>
//#include <CREncapsulation/crTalkEventPPC.h>
//#include <CREncapsulation/crKeyEventMapPPC.h>
//#include <CREncapsulation/crObjectEventCallback.h>
//#include <CREncapsulation/crAILoader.h>
#include <CREncapsulation/crNodeCallbacks.h>

#include <CRGUI/cr2DStageManager.h>
#include <CRGUI/crImageStage.h>
//#include <CRGUI/crEventCallback.h>
#include <CRGUI/crElement.h>

#include <CRProducer/crGUIEventAdapter.h>
//#include <CREncapsulation/crNetManager.h>

#include <CRCore/crAlphaFunc.h>
#include <CRCore/crBlendColor.h>
#include <CRCore/crBlendEquation.h>
#include <CRCore/crClampColor.h>
#include <CRCore/crClipPlane.h>
#include <CRCore/crColorMask.h>
#include <CRCore/crCullFace.h>
#include <CRCore/crDepth.h>
#include <CRCore/crFog.h>
#include <CRCore/crFrontFace.h>
#include <CRCore/crHint.h>
#include <CRCore/crLightModel.h>
#include <CRCore/crLogicOp.h>
#include <CRCore/crMultisample.h>
#include <CRCore/crPolygonMode.h>
#include <CRCore/crPolygonOffset.h>
#include <CRCore/crScissor.h>
#include <CRCore/crShadeModel.h>
#include <CRCore/crStencil.h>
#include <CRCore/crStencilTwoSided.h>
#include <CRCore/crTexEnv.h>
#include <CRCore/crTexEnvCombine.h>
#include <CRCore/crTexEnvFilter.h>
#include <CRCore/crTexGen.h>

//#include <CREncapsulation/crDBQuery.h>
#include <CRIOManager/crCookFile.h>
#include <CRTerrain/crTerrainTile.h>

#include "rcfg/ConfigScriptOO.h"

#include <sys/stat.h>

#include <string>
#include <vector>
#include <stack>
#include <set>
#include <map>
#include <iostream>

using namespace CREncapsulation;
using namespace CRCore;
using namespace CRPhysics;
using namespace CRParticle;
using namespace CRProducer;
using namespace CRTerrain;
using namespace CRIOManager;
//using namespace CRDataBase;

_crInt32 CREncapsulation::parseEventMsg( const std::string &str )
{
	if(str.compare("NULL") == 0)
		return NULL;
	if(str.compare("WCH_KEYBOARDMOUSE") == 0)
		return WCH_KEYBOARDMOUSE;
	if(str.compare("WCH_INPUTPARAM") == 0)
		return WCH_INPUTPARAM;
	if(str.compare("WCH_MSGCONTAINER") == 0)
		return WCH_MSGCONTAINER;
	if(str.compare("WCH_UPDATEVISITOR") == 0)
		return WCH_UPDATEVISITOR;
	if(str.compare("WCH_COLLIDE") == 0)
		return WCH_COLLIDE;
	if(str.compare("WCH_COLLIDETEST") == 0)
		return WCH_COLLIDETEST;
	//Editor
	if(str.compare("WCH_ShowTerrainEditDecal") == 0)
		return WCH_ShowTerrainEditDecal;
	//LenUI
	if(str.compare("WCH_LenFadeInit") == 0)
		return WCH_LenFadeInit;
	if(str.compare("WCH_LenFade") == 0)
		return WCH_LenFade;
	if(str.compare("WCH_AnimationEvent") == 0)
		return WCH_AnimationEvent;
	if(str.compare("WCH_SceneStart") == 0)
		return WCH_SceneStart;
	if(str.compare("WCH_RunGame") == 0)
		return WCH_RunGame;
	if(str.compare("WCH_GameStart") == 0)
		return WCH_GameStart;
	if(str.compare("WCH_GameEnd") == 0)
		return WCH_GameEnd;
	if(str.compare("WCH_DOWNLOAD") == 0)
		return WCH_DOWNLOAD;
	if(str.compare("WCH_Startup") == 0)
		return WCH_Startup;
	if(str.compare("WCH_ShowCanvasFaild") == 0)
		return WCH_ShowCanvasFaild;
	if(str.compare("WCH_ShowCanvasRecover") == 0)
		return WCH_ShowCanvasRecover;
	/////////////////////////////////////////////

	if(str.compare("WCH_AITASK") == 0)
		return WCH_AITASK;
	if(str.compare("WCH_INPUTAITASK") == 0)
		return WCH_INPUTAITASK;
	if(str.compare("WCH_DOAITASK") == 0)
		return WCH_DOAITASK;
	if(str.compare("WCH_TASKEND") == 0)
		return WCH_TASKEND;
	if(str.compare("WCH_INVOLUME") == 0)
		return WCH_INVOLUME;
	if(str.compare("WCH_OUTVOLUME") == 0)
		return WCH_OUTVOLUME;
	if(str.compare("WCH_ENABLEVOLUME") == 0)
		return WCH_ENABLEVOLUME;

	//if(str.compare("WCH_KEYDOWN") == 0)
	//	return WCH_KEYDOWN;
	//if(str.compare("WCH_KEYUP") == 0)
	//	return WCH_KEYUP;
	//if(str.compare("WCH_KEY_UP") == 0)
	//	return WCH_KEY_UP;
	//if(str.compare("WCH_KEY_DOWN") == 0)
	//	return WCH_KEY_DOWN;
	//if(str.compare("WCH_KEY_LEFT") == 0)
	//	return WCH_KEY_LEFT;
	//if(str.compare("WCH_KEY_RIGHT") == 0)
	//	return WCH_KEY_RIGHT;

	//if(str.compare("WCH_KEY_W") == 0)
	//	return WCH_KEY_W;
	//if(str.compare("WCH_KEY_S") == 0)
	//	return WCH_KEY_S;
	//if(str.compare("WCH_KEY_A") == 0)
	//	return WCH_KEY_A;
	//if(str.compare("WCH_KEY_D") == 0)
	//	return WCH_KEY_D;

	//if(str.compare("WCH_MOUSEPUSH") == 0)
	//	return WCH_MOUSEPUSH;
	//if(str.compare("WCH_MOUSERELEASE") == 0)
	//	return WCH_MOUSERELEASE;
	//if(str.compare("WCH_MOUSEMOVE") == 0)
	//	return WCH_MOUSEMOVE;
	//if(str.compare("WCH_MOUSEDRAG") == 0)
	//	return WCH_MOUSEDRAG;
	//if(str.compare("LEFT_MOUSE_BUTTON") == 0)
	//	return crGUIEventAdapter::LEFT_MOUSE_BUTTON;
	//if(str.compare("RIGHT_MOUSE_BUTTON") == 0)
	//	return crGUIEventAdapter::RIGHT_MOUSE_BUTTON;
	//if(str.compare("MIDDLE_MOUSE_BUTTON") == 0)
	//	return crGUIEventAdapter::MIDDLE_MOUSE_BUTTON;
	//if(str.compare("WCH_SCROLLUP") == 0)
	//	return WCH_SCROLLUP;
	//if(str.compare("WCH_SCROLLDOWN") == 0)
	//	return WCH_SCROLLDOWN;

	//if(str.compare("WCH_KEY_C") == 0)
	//	return WCH_KEY_C;
	//if(str.compare("WCH_KEY_Q") == 0)
	//	return WCH_KEY_Q;
	//if(str.compare("WCH_KEY_E") == 0)
	//	return WCH_KEY_E;
	//if(str.compare("WCH_KEY_I") == 0)
	//	return WCH_KEY_I;
	//if(str.compare("WCH_KEY_P") == 0)
	//	return WCH_KEY_P;
	//if(str.compare("WCH_KEY_O") == 0)//'o'
	//	return WCH_KEY_O;
	//if(str.compare("WCH_KEY_SPACE") == 0)
	//	return WCH_KEY_SPACE;
	//if(str.compare("WCH_KEY_Control_L") == 0)
	//	return WCH_KEY_Control_L;
	//if(str.compare("WCH_KEY_V") == 0)
	//	return WCH_KEY_V;
	//if(str.compare("WCH_KEY_H") == 0)
	//	return WCH_KEY_H;
	//if(str.compare("WCH_KEY_F") == 0)
	//	return WCH_KEY_F;

	if(str.compare("WCH_WEAPON_FIRE") == 0)
		return WCH_WEAPON_FIRE;
	if(str.compare("WCH_WEAPON_REPLACECLIP") == 0)
		return WCH_WEAPON_REPLACECLIP;
	if(str.compare("WCH_WEAPON_SELECT") == 0)
		return WCH_WEAPON_SELECT;
	if(str.compare("WCH_WEAPON_UNSELECT") == 0)
		return WCH_WEAPON_UNSELECT;
	if(str.compare("WCH_BULLET_FLY") == 0)
		return WCH_BULLET_FLY;
	if(str.compare("WCH_BULLET_EXPLODE") == 0)
		return WCH_BULLET_EXPLODE;
	if(str.compare("WCH_BULLET_COLLIDE") == 0)
		return WCH_BULLET_COLLIDE;
	if(str.compare("WCH_BULLET_FIRE") == 0)
		return WCH_BULLET_FIRE;

	//if(str.compare("WCH_KEY_0") == 0)//0
	//	return WCH_KEY_0;
	//if(str.compare("WCH_KEY_1") == 0)
	//	return WCH_KEY_1;
	//if(str.compare("WCH_KEY_2") == 0)
	//	return WCH_KEY_2;
	//if(str.compare("WCH_KEY_3") == 0)
	//	return WCH_KEY_3;
	//if(str.compare("WCH_KEY_4") == 0)
	//	return WCH_KEY_4;
	//if(str.compare("WCH_KEY_5") == 0)
	//	return WCH_KEY_5;
	//if(str.compare("WCH_KEY_6") == 0)
	//	return WCH_KEY_6;
	//if(str.compare("WCH_KEY_7") == 0)
	//	return WCH_KEY_7;
	//if(str.compare("WCH_KEY_8") == 0)
	//	return WCH_KEY_8;
	//if(str.compare("WCH_KEY_9") == 0)
	//	return WCH_KEY_9;

	//if(str.compare("WCH_KEY_F1") == 0)
	//	return WCH_KEY_F1;
	//if(str.compare("WCH_KEY_F2") == 0)
	//	return WCH_KEY_F2;
	//if(str.compare("WCH_KEY_F3") == 0)
	//	return WCH_KEY_F3;
	//if(str.compare("WCH_KEY_F4") == 0)
	//	return WCH_KEY_F4;
	//if(str.compare("WCH_KEY_F5") == 0)
	//	return WCH_KEY_F5;
	//if(str.compare("WCH_KEY_F6") == 0)
	//	return WCH_KEY_F6;
	//if(str.compare("WCH_KEY_F7") == 0)
	//	return WCH_KEY_F7;
	//if(str.compare("WCH_KEY_F8") == 0)
	//	return WCH_KEY_F8;
	//if(str.compare("WCH_KEY_F9") == 0)
	//	return WCH_KEY_F9;
	//if(str.compare("WCH_KEY_F10") == 0)//0
	//	return WCH_KEY_F10;
	//if(str.compare("WCH_KEY_F11") == 0)//0
	//	return WCH_KEY_F11;
	//if(str.compare("WCH_KEY_F12") == 0)//0
	//	return WCH_KEY_F12;

	if(str.compare("WCH_STALL") == 0)
		return WCH_STALL;
	if(str.compare("WCH_UNSTALL") == 0)
		return WCH_UNSTALL;
	if(str.compare("WCH_DEAD") == 0)
		return WCH_DEAD;
	if(str.compare("WCH_RENASCENCE") == 0)
		return WCH_RENASCENCE;
	if(str.compare("WCH_AIMOVIES") == 0)
		return WCH_AIMOVIES;
	if(str.compare("WCH_AIMOVBEGIN") == 0)
		return WCH_AIMOVBEGIN;
	if(str.compare("WCH_AIMOVEND") == 0)
		return WCH_AIMOVEND;
	if(str.compare("WCH_CHANGECAMERA") == 0)
		return WCH_CHANGECAMERA;
	if(str.compare("WCH_HIDEBODY") == 0)
		return WCH_HIDEBODY;
	if(str.compare("WCH_STOPBODY") == 0)
		return WCH_STOPBODY;
	if(str.compare("WCH_CHANGECHBODY") == 0)
		return WCH_CHANGECHBODY;
	if(str.compare("WCH_WIEWFILTEREVENT") == 0)
		return WCH_WIEWFILTEREVENT;
	if(str.compare("WCH_SHOWWIEWFILTER") == 0)
		return WCH_SHOWWIEWFILTER;
	if(str.compare("WCH_BOTCONTROLLER") == 0)
		return WCH_BOTCONTROLLER;
	if(str.compare("WCH_LOADPROGRESS") == 0)
		return WCH_LOADPROGRESS;
	if(str.compare("WCH_NEXTPROGRESS") == 0)
		return WCH_NEXTPROGRESS;
	if(str.compare("WCH_NEXTGAME") == 0)
		return WCH_NEXTGAME;
	if(str.compare("WCH_EXITGAME") == 0)
		return WCH_EXITGAME;
	if(str.compare("WCH_WINRESTORED") == 0)
		return WCH_WINRESTORED;
	if(str.compare("WCH_PLAYSOUND") == 0)
		return WCH_PLAYSOUND;
	if(str.compare("WCH_PLAYSEQUENCE") == 0)
		return WCH_PLAYSEQUENCE;
	if(str.compare("WCH_ADDACTOR") == 0)
		return WCH_ADDACTOR;
	if(str.compare("WCH_PLAYAMBSOUND") == 0)
		return WCH_PLAYAMBSOUND;
	if(str.compare("WCH_STOPAMBSOUND") == 0)
		return WCH_STOPAMBSOUND;
	if(str.compare("WCH_DOMODALDLG") == 0)
		return WCH_DOMODALDLG;
	if(str.compare("WCH_SHOWDLG") == 0)
		return WCH_SHOWDLG;
	if(str.compare("WCH_LOADBOT") == 0)
		return WCH_LOADBOT;
	if(str.compare("WCH_SELECTWEAPON") == 0)
		return WCH_SELECTWEAPON;
	if(str.compare("WCH_FRONTCONTACT") == 0)
		return WCH_FRONTCONTACT;
	if(str.compare("WCH_UPDATE") == 0)
		return WCH_UPDATE;
	if(str.compare("WCH_SHOWCURSOR") == 0)
		return WCH_SHOWCURSOR;
	if(str.compare("WCH_HIDECURSOR") == 0)
		return WCH_HIDECURSOR;
	if(str.compare("WCH_FILTERCAUTION") == 0)
		return WCH_FILTERCAUTION;
	if(str.compare("WCH_FILTERINFO") == 0)
		return WCH_FILTERINFO;
	if(str.compare("WCH_SELECTNODE") == 0)
		return WCH_SELECTNODE;
	if(str.compare("WCH_UNSELECTNODE") == 0)
		return WCH_UNSELECTNODE;
	if(str.compare("WCH_FILTERSWITCH") == 0)
		return WCH_FILTERSWITCH;
	if(str.compare("WCH_NODESWITCH") == 0)
		return WCH_NODESWITCH;
	if(str.compare("WCH_FILTERGETFOCUS") == 0)
		return WCH_FILTERGETFOCUS;
	if(str.compare("WCH_FILTERLOSTFOCUS") == 0)
		return WCH_FILTERLOSTFOCUS;
	if(str.compare("WCH_LISTEVENT") == 0)
		return WCH_LISTEVENT;
	if(str.compare("WCH_NODEGETFOCUS") == 0)
		return WCH_NODEGETFOCUS;
	if(str.compare("WCH_NODELOSTFOCUS") == 0)
		return WCH_NODELOSTFOCUS;
	if(str.compare("WCH_WIDGETLOSTCAPTURE") == 0)
		return WCH_WIDGETLOSTCAPTURE;
	if(str.compare("WCH_WIDGETGETCAPTURE") == 0)
		return WCH_WIDGETGETCAPTURE;
	//if(str.compare("WCH_WINFOCUS") == 0)
	//	return WCH_WINFOCUS;
	
	if(str.compare("WCH_TALK") == 0)
		return WCH_TALK;
	if(str.compare("WCH_GETTASK") == 0)
		return WCH_GETTASK;
	if(str.compare("WCH_ADVANCETASK") == 0)
		return WCH_ADVANCETASK;
	if(str.compare("WCH_REFERTASK") == 0)
		return WCH_REFERTASK;
	if(str.compare("WCH_TASKAWARD") == 0)
		return WCH_TASKAWARD;
    ////////////UI消息
	if(str.compare("NULL") == 0)
		return NULL;
	if(str.compare("LBtnUp") == 0)
		return WCH_UI_LBtnUp;
	if(str.compare("LBtnDown") == 0)
		return WCH_UI_LBtnDown;
	if(str.compare("LBtnDblClk") == 0)
		return WCH_UI_LBtnDblClk;
	if(str.compare("RBtnUp") == 0)
		return WCH_UI_RBtnUp;
	if(str.compare("RBtnDown") == 0)
		return WCH_UI_RBtnDown;
	if(str.compare("RBtnDblClk") == 0)
		return WCH_UI_RBtnDblClk;
	if(str.compare("MBtnUp") == 0)
		return WCH_UI_MBtnUp;
	if(str.compare("MBtnDown") == 0)
		return WCH_UI_MBtnDown;
	if(str.compare("MBtnDblClk") == 0)
		return WCH_UI_MBtnDblClk;
	if(str.compare("MouseMove") == 0)
		return WCH_UI_MouseMove;
	if(str.compare("MouseWheel") == 0)
		return WCH_UI_MouseWheel;
	if(str.compare("MouseOn") == 0)
		return WCH_UI_MouseOn;

	if(str.compare("GetFocus") == 0)
		return WCH_UI_GetFocus;
	if(str.compare("LostFocus") == 0)
		return WCH_UI_LostFocus;
	if(str.compare("InitWindow") == 0)
		return WCH_UI_InitWindow;
	if(str.compare("DestroyWindow") == 0)
		return WCH_UI_DestroyWindow;
	if(str.compare("InputKey") == 0)
		return WCH_UI_InputKey;
	if(str.compare("InputChar") == 0)
		return WCH_UI_InputChar;
	if(str.compare("UpdateData") == 0)
		return WCH_UI_UpdateData;
	if(str.compare("PreDraw") == 0)
		return WCH_UI_PreDraw;
	if(str.compare("Frame") == 0)
		return WCH_UI_Frame;
	if(str.compare("SelectChange") == 0)
		return WCH_UI_SelectChange;
	if(str.compare("ValueChange") == 0)
		return WCH_UI_ValueChange;
	if(str.compare("ListItemDrag") == 0)
		return WCH_UI_ListItemDrag;
	if(str.compare("OnShow") == 0)
		return WCH_UI_OnShow;
	if(str.compare("WCH_OK") == 0)
		return WCH_OK;
	if(str.compare("WCH_CANCEL") == 0)
		return WCH_CANCEL;
	if(str.compare("WCH_ABORT") == 0)
		return WCH_ABORT;
	if(str.compare("WCH_RETRY") == 0)
		return WCH_RETRY;
	if(str.compare("WCH_IGNORE") == 0)
		return WCH_IGNORE;
	if(str.compare("WCH_YES") == 0)
		return WCH_YES;
	if(str.compare("WCH_NO") == 0)
		return WCH_NO;
	if(str.compare("WCH_CLOSE") == 0)
		return WCH_CLOSE;
	if(str.compare("WCH_HELP") == 0)
		return WCH_HELP;
	if(str.compare("WCH_TRYAGAIN") == 0)
		return WCH_TRYAGAIN;
	if(str.compare("WCH_CONTINUE") == 0)
		return WCH_CONTINUE;
	if(str.compare("WCH_TIMEOUT") == 0)
		return WCH_TIMEOUT;
    ////////////////////////////////////////
	if(str.compare("WCH_USER") == 0)
		return WCH_USER;
	if(str.find("WCH_USER+")==0)
	{
		int keylen = strlen("WCH_USER+");
		std::string user = str;
		user.erase(0,keylen);
		return WCH_USER + atoi(user.c_str());
	}
	if(str.compare("WCHNETID") == 0)
		return 5;
	if(str.find("WCHNETID+")==0)
	{
		int keylen = strlen("WCHNETID+");
		std::string user = str;
		user.erase(0,keylen);
		return 5 + atoi(user.c_str());
	}

	//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::parseEventMsg(): 该消息目前不支持 "<<str<<std::endl;
	return atoi(str.c_str());
}

_crInt16 CREncapsulation::parseEventMsg16( const std::string &str )
{
	if(str.compare("NULL") == 0)
		return NULL;
	//MouseButtonMask
	if(str.compare("LEFT_MOUSE_BUTTON") == 0)
		return crGUIEventAdapter::LEFT_MOUSE_BUTTON;
	if(str.compare("RIGHT_MOUSE_BUTTON") == 0)
		return crGUIEventAdapter::RIGHT_MOUSE_BUTTON;
	if(str.compare("MIDDLE_MOUSE_BUTTON") == 0)
		return crGUIEventAdapter::MIDDLE_MOUSE_BUTTON;
	//EventType
	if(str.compare("MOUSEPUSH") == 0)
		return crGUIEventAdapter::PUSH;
	if(str.compare("MOUSERELEASE") == 0)
		return crGUIEventAdapter::RELEASE;
	if(str.compare("MOUSEDOUBLECLICK") == 0)
		return crGUIEventAdapter::DOUBLECLICK;
	if(str.compare("MOUSEDRAG") == 0)
		return crGUIEventAdapter::DRAG;
	if(str.compare("MOUSEMOVE") == 0)
		return crGUIEventAdapter::MOVE;
	if(str.compare("KEYDOWN") == 0)
		return crGUIEventAdapter::KEYDOWN;
	if(str.compare("KEYUP") == 0)
		return crGUIEventAdapter::KEYUP;
	if(str.compare("INPUTCHAR") == 0)
		return crGUIEventAdapter::INPUTCHAR;
	if(str.compare("FRAME") == 0)
		return crGUIEventAdapter::FRAME;
	if(str.compare("RESIZE") == 0)
		return crGUIEventAdapter::RESIZE;
	if(str.compare("SCROLLUP") == 0)
		return crGUIEventAdapter::SCROLLUP;
	if(str.compare("SCROLLDOWN") == 0)
		return crGUIEventAdapter::SCROLLDOWN;
	if(str.compare("SCROLLLEFT") == 0)
		return crGUIEventAdapter::SCROLLLEFT;
	if(str.compare("SCROLLRIGHT") == 0)
		return crGUIEventAdapter::SCROLLRIGHT;
	//if(str.compare("WINFOCUS") == 0)
	//	return crGUIEventAdapter::WINFOCUS;
    //基础交互，使得不与键盘输入冲突
	if(str.compare("WCH_UP") == 0)
		return WCH_UP;
	if(str.compare("WCH_DOWN") == 0)
		return WCH_DOWN;
	if(str.compare("WCH_LEFT") == 0)
		return WCH_LEFT;
	if(str.compare("WCH_RIGHT") == 0)
		return WCH_RIGHT;
	if(str.compare("WCH_TURNLEFT") == 0)
		return WCH_TURNLEFT;
	if(str.compare("WCH_TURNRIGHT") == 0)
		return WCH_TURNRIGHT;
	if(str.compare("WCH_TURNUP") == 0)
		return WCH_TURNUP;
	if(str.compare("WCH_TURNDOWN") == 0)
		return WCH_TURNDOWN;
	if(str.compare("WCH_JUMP") == 0)
		return WCH_JUMP;
	///////////////AITask
	if(str.compare("WCHAI_Begin") == 0)
		return WCHAI_Begin;
	if(str.compare("WCHAI_Pause") == 0)
		return WCHAI_Pause;
	if(str.compare("WCHAI_Resume") == 0)
		return WCHAI_Resume;
	if(str.compare("WCHAI_End") == 0)
		return WCHAI_End;
	if(str.compare("WCHAI_SetBotToCP") == 0)
		return WCHAI_SetBotToCP;
	if(str.compare("WCHAI_SetCPToBot") == 0)
		return WCHAI_SetCPToBot;
	if(str.compare("WCHAI_RecBot") == 0)
		return WCHAI_RecBot;
	//if(str.compare("WCHAI_TakeoverControl") == 0)
	//	return WCHAI_TakeoverControl;

	////////////////////////////////////////
	if(str.compare("WCH_USER16") == 0)
		return WCH_USER16;
	if(str.find("WCH_USER16+")==0)
	{
		int keylen = strlen("WCH_USER16+");
		std::string user = str;
		user.erase(0,keylen);
		return WCH_USER16 + atoi(user.c_str());
	}
	//////////////////////////////////////
	//////////////////////KeySymbol
	if(str.compare("KEY_Space") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_Space;
	if(str.compare("KEY_BackSpace") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_BackSpace;
	if(str.compare("KEY_Tab") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_Tab;
	if(str.compare("KEY_Linefeed") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_Linefeed;
	if(str.compare("KEY_Clear") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_Clear;
	if(str.compare("KEY_Return") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_Return;
	if(str.compare("KEY_Pause") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_Pause;
	if(str.compare("KEY_Scroll_Lock") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_Scroll_Lock;
	if(str.compare("KEY_Sys_Req") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_Sys_Req;
	if(str.compare("KEY_Escape") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_Escape;
	if(str.compare("KEY_Delete") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_Delete;
	if(str.compare("KEY_Home") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_Home;
	if(str.compare("KEY_Left") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_Left;
	if(str.compare("KEY_Up") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_Up;
	if(str.compare("KEY_Right") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_Right;
	if(str.compare("KEY_Down") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_Down;
	if(str.compare("KEY_Prior") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_Prior;
	if(str.compare("KEY_Page_Up") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_Page_Up;
	if(str.compare("KEY_Next") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_Next;
	if(str.compare("KEY_Page_Down") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_Page_Down;
	if(str.compare("KEY_End") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_End;
	if(str.compare("KEY_Begin") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_Begin;

	if(str.compare("KEY_F1") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_F1;
	if(str.compare("KEY_F2") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_F2;
	if(str.compare("KEY_F3") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_F3;
	if(str.compare("KEY_F4") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_F4;
	if(str.compare("KEY_F5") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_F5;
	if(str.compare("KEY_F6") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_F6;
	if(str.compare("KEY_F7") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_F7;
	if(str.compare("KEY_F8") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_F8;
	if(str.compare("KEY_F9") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_F9;
	if(str.compare("KEY_F10") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_F10;
	if(str.compare("KEY_F11") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_F11;
	if(str.compare("KEY_F12") == 0)
		return (_crInt16)crGUIEventAdapter::KEY_F12;
	return atoi(str.c_str());
}
//class crLightBindToMoCallback : public CRCore::crNodeCallback
//{
//public:
//
//	crLightBindToMoCallback()
//	{
//	}
//	void setLightMO(CRPhysics::crMatterObject *mo) { m_lightMO = mo;}
//
//	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
//	{
//		CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
//		crLightSource* ls = dynamic_cast<crLightSource*>(node);
//		if (cv && ls)
//		{
//			ls->getLightParamStruct()->m_lsPosition = m_lightMO->getMatterObjectPosition();
//			crLight* light = dynamic_cast<crLight*>(ls->getLight());
//            ls->getLightParamStruct()->m_lsDirection = light->getDirection() * m_lightMO->getMatterObjectRotation();
//		}
//      crNodeCallback::traverse(node,nv);
//	}
//  virtual void addEventParam(int i, const std::string &str){}
//protected:
//	CRCore::ref_ptr<CRPhysics::crMatterObject> m_lightMO;
//};


//class MoveLightWithEyePointTransform : public crTransform
//{
//public:
//	MoveLightWithEyePointTransform(){}
//	virtual bool computeLocalToWorldMatrix(crMatrix& matrix,crNodeVisitor* nv) const 
//	{
//		CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
//		if (cv)
//		{
//			matrix.preMult(crMatrix::translate(cv->getEyeLocal()));
//			//CRCore::notify(CRCore::ALWAYS)<<"MoveLightWithEyePointTransform:"<<cv->getEyeLocal()<<std::endl;
//		}
//		return true;
//	}
//
//	/** Get the transformation matrix which moves from world coords to local coords.*/
//	virtual bool computeWorldToLocalMatrix(crMatrix& matrix,crNodeVisitor* nv) const
//	{
//		CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
//		if (cv)
//		{
//			matrix.postMult(crMatrix::translate(-cv->getEyeLocal()));
//		}
//		return true;
//	}
//};

//
struct MyCameraPostDrawCallback : public crCameraNode::DrawCallback
{
	MyCameraPostDrawCallback():
m_count(0){}

virtual void operator () (crCameraNode& camera)
{
	crCameraNode::BufferAttachmentMap& bufferAttachmentMap = camera.getBufferAttachmentMap();
	crImage *image = bufferAttachmentMap[crCameraNode::COLOR_BUFFER].m_image.get();
	if(image)
	{
		const_cast<MyCameraPostDrawCallback *>(this)->m_count++;

		std::string fileName = image->getFileName();
		std::string file = crArgumentParser::getFileNameEliminateExt(fileName);
		std::string ext = crArgumentParser::getFileExtension(fileName);
		char filename[4];
		sprintf(filename,"%d\0",m_count);
		fileName = file + filename + "." + ext;
		CRIOManager::writeImageFile(*image,fileName);
	}
}
int m_count;
};

//void CREncapsulation::cfgloading()
//{
//	//CRAI::crAIGroup::loadKeyeventmap("script/keyeventmap.cfg");
//}

//void CREncapsulation::reLoadDisplaySettingCfg()
//{
//	rcfg::ConfigScript cfg_script;
//
//	struct stat buf;
//	if (stat( "script/init.cfg", &buf )) 
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::reLoadDisplaySettingCfg(): file open error"<<std::endl;
//		return;
//	}
//
//	if (!cfg_script.Open("script/init.cfg"))
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::reLoadDisplaySettingCfg(): "<<cfg_script.GetLastError()<<std::endl;
//		return;
//	}
//
//	int int1;
//	float flt1;
//	std::string str;
//	std::vector<float> v_i;
//	if(cfg_script.Push("DisplaySetting"))
//	{
//		crDisplaySettings *ds = CRCore::crDisplaySettings::instance();
//		if(cfg_script.Get("screenWidth", int1))
//		{
//			ds->setScreenWidth(int1);
//		}
//		if(cfg_script.Get("screenHeight", int1))
//		{
//			ds->setScreenHeight(int1);
//		}
//		if(cfg_script.Get("screenDistance", int1))
//		{
//			ds->setScreenDistance(int1);
//		}
//		if(cfg_script.Get("displayType", str))
//		{
//			if(str.compare("MONITOR")==0)
//			{
//				ds->setDisplayType(crDisplaySettings::MONITOR);
//			}
//			else if(str.compare("POWERWALL")==0)
//			{
//				ds->setDisplayType(crDisplaySettings::POWERWALL);
//			}
//			else if(str.compare("REALITY_CENTER")==0)
//			{
//				ds->setDisplayType(crDisplaySettings::REALITY_CENTER);
//			}
//			else if(str.compare("HEAD_MOUNTED_DISPLAY")==0)
//			{
//				ds->setDisplayType(crDisplaySettings::HEAD_MOUNTED_DISPLAY);
//			}
//		}
//		if(cfg_script.Get("stereo", int1))
//		{
//			ds->setStereo(int1);
//		}
//		if(cfg_script.Get("stereoMode", str))
//		{
//			if(str.compare("QUAD_BUFFER")==0)
//			{
//				ds->setStereoMode(crDisplaySettings::QUAD_BUFFER);
//			}
//			else if(str.compare("ANAGLYPHIC")==0)
//			{
//				ds->setStereoMode(crDisplaySettings::ANAGLYPHIC);
//			}
//			else if(str.compare("HORIZONTAL_SPLIT")==0)
//			{
//				ds->setStereoMode(crDisplaySettings::HORIZONTAL_SPLIT);
//			}
//			else if(str.compare("VERTICAL_SPLIT")==0)
//			{
//				ds->setStereoMode(crDisplaySettings::VERTICAL_SPLIT);
//			}
//			else if(str.compare("LEFT_EYE")==0)
//			{
//				ds->setStereoMode(crDisplaySettings::LEFT_EYE);
//			}
//			else if(str.compare("RIGHT_EYE")==0)
//			{
//				ds->setStereoMode(crDisplaySettings::RIGHT_EYE);
//			}
//			else if(str.compare("HORIZONTAL_INTERLACE")==0)
//			{
//				ds->setStereoMode(crDisplaySettings::HORIZONTAL_INTERLACE);
//			}
//			else if(str.compare("VERTICAL_INTERLACE")==0)
//			{
//				ds->setStereoMode(crDisplaySettings::VERTICAL_INTERLACE);
//			}
//		}
//		if(cfg_script.Get("eyeSeparation", flt1))
//		{
//			ds->setEyeSeparation(flt1);
//		}
//		if(cfg_script.Get("splitStereoHorizontalEyeMapping", str))
//		{
//			if(str.compare("LEFT_EYE_LEFT_VIEWPORT")==0)
//			{
//				ds->setSplitStereoHorizontalEyeMapping(crDisplaySettings::LEFT_EYE_LEFT_VIEWPORT);
//			}
//			else if(str.compare("LEFT_EYE_RIGHT_VIEWPORT")==0)
//			{
//				ds->setSplitStereoHorizontalEyeMapping(crDisplaySettings::LEFT_EYE_RIGHT_VIEWPORT);
//			}
//		}
//		if(cfg_script.Get("splitStereoHorizontalSeparation", int1))
//		{
//			ds->setSplitStereoHorizontalSeparation(int1);
//		}
//		if(cfg_script.Get("splitStereoVerticalEyeMapping", str))
//		{
//			if(str.compare("LEFT_EYE_TOP_VIEWPORT")==0)
//			{
//				ds->setSplitStereoVerticalEyeMapping(crDisplaySettings::LEFT_EYE_TOP_VIEWPORT);
//			}
//			else if(str.compare("LEFT_EYE_BOTTOM_VIEWPORT")==0)
//			{
//				ds->setSplitStereoVerticalEyeMapping(crDisplaySettings::LEFT_EYE_BOTTOM_VIEWPORT);
//			}
//		}
//		if(cfg_script.Get("splitStereoVerticalSeparation", int1))
//		{
//			ds->setSplitStereoVerticalSeparation(int1);
//		}
//		if(cfg_script.Get("splitStereoAutoAdjustAspectRatio", int1))
//		{
//			ds->setSplitStereoAutoAjustAspectRatio(int1);
//		}
//		if(cfg_script.Get("doubleBuffer", int1))
//		{
//			ds->setDoubleBuffer(int1);
//		}
//		if(cfg_script.Get("RGB", int1))
//		{
//			ds->setRGB(int1);
//		}
//		if(cfg_script.Get("depthBuffer", int1))
//		{
//			ds->setDepthBuffer(int1);
//		}
//		if(cfg_script.Get("minimumNumberAlphaBits", int1))
//		{
//			ds->setMinimumNumAlphaBits(int1);
//		}
//		if(cfg_script.Get("minimumNumberStencilBits", int1))
//		{
//			ds->setMinimumNumStencilBits(int1);
//		}
//		if(cfg_script.Get("minimumNumberAccumBits",v_i))
//		{
//			if(v_i.size()==4)
//				ds->setMinimumNumAccumBits(v_i[0],v_i[1],v_i[2],v_i[3]);
//		}
//		if(cfg_script.Get("maxNumOfGraphicsContexts", int1))
//		{
//			ds->setMaxNumberOfGraphicsContexts(int1);
//		}
//		if(cfg_script.Get("colorBits", int1))
//		{
//			ds->setColorBits(int1);
//		}
//		if(cfg_script.Get("depthBits", int1))
//		{
//			ds->setDepthBits(int1);
//		}
//		if(cfg_script.Get("numMultiSamples", int1))
//		{
//			ds->setNumMultiSamples(int1);
//		}
//		if(cfg_script.Get("MaxAnisotropy", int1))
//		{
//			ds->setMaxAnisotropy(int1);
//		}
//		if(cfg_script.Get("AmbientIntensity", v_i))
//		{
//			if(v_i.size()==4)
//				ds->setAmbientIntensity(crVector4(v_i[0],v_i[1],v_i[2],v_i[3]));
//		}
//		if(cfg_script.Get("HDR", int1))
//		{
//			ds->setHDR(int1);
//		}
//		if(cfg_script.Get("GlowSize", flt1))
//		{
//			ds->setGlowSize(flt1);
//		}
//		if(cfg_script.Get("EnableAlphaShadow", int1))
//		{
//			ds->setEnableAlphaShadow(int1);
//		}
//		if(cfg_script.Get("Fog", int1))
//		{
//			ds->setFog(int1);
//		}
//		if(cfg_script.Get("FogColor", v_i))
//		{
//			if(v_i.size()==3)
//				ds->setFogColor(crVector3(v_i[0],v_i[1],v_i[2]));
//		}
//		if(cfg_script.Get("FogStartEndDensity", v_i))
//		{
//			if(v_i.size()==3)
//				ds->setFogParams(crVector3(v_i[0],v_i[1],v_i[2]));
//		}
//		if(cfg_script.Get("FogScaleSpeed", v_i))
//		{
//			if(v_i.size()==2)
//				ds->setFogScaleSpeed(crVector2(v_i[0],v_i[1]));
//		}
//		if(cfg_script.Get("NoiseSample", v_i))
//		{
//			if(v_i.size()==4)
//				ds->setNoiseSample(crVector4(v_i[0],v_i[1],v_i[2],v_i[3]));
//		}
//		if(cfg_script.Get("NeedPixelDepth", int1))
//		{
//			ds->setNeedPixelDepth(int1);
//		}
//		if(cfg_script.Get("MaxDepth", flt1))
//		{
//			ds->setMaxDepth(flt1);
//		}
//		if(cfg_script.Get("DepthOfField", int1))
//		{
//			ds->setDepthOfField(int1);
//		}
//		if(cfg_script.Get("DofBlurFactor", flt1))
//		{
//			ds->setDofBlur(flt1);
//		}
//		if(cfg_script.Get("MotionBlur", int1))
//		{
//			ds->setMotionBlur(int1);
//		}
//		if(cfg_script.Get("MotionBlurFactor", flt1))
//		{
//			ds->setMotionBlurFactor(flt1);
//		}
//		if(cfg_script.Get("ShadowDetail", int1))
//		{
//			ds->setShadowDetail(int1);
//		}
//		if(cfg_script.Get("TextureDetail", int1))
//		{
//			ds->setTextureDetail(int1);
//		}
//		cfg_script.Pop();
//	}
//}
//
void CREncapsulation::loadInitCfg(bool wantScreanSize,const std::string &path)
{
	std::string filename;
	if(path.empty())
		filename = "script/init.cfg";
	else
        filename = path + "/script/init.cfg";
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename,true))
	{
		CRCore::notify(CRCore::FATAL)<<"loadInitCfg(): file open error,"<<filename<<std::endl;
		return;
	}

	int int1;
	float flt1;
	std::string str;
	std::vector<float> v_i;
	crDisplaySettings *ds = CRCore::crDisplaySettings::instance();
	if(cfg_script.Push("DisplaySetting"))
	{
		if(cfg_script.Get("UIWindowPixel",int1))
			ds->setUIWindowPixel(int1);
		if(ds->getRunMode()!=crDisplaySettings::WebGame)
		{
			if(cfg_script.Get("FullScreen", int1))
			{
				ds->setFullScreen(int1);
			}
			DEVMODE dm;
			memset(&dm,0,sizeof(dm));
			dm.dmSize = sizeof(dm);
			EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
			int screenWidth = dm.dmPelsWidth;
			int screenHeight = dm.dmPelsHeight;
			if(cfg_script.Get("WindowMaxSize", int1))
			{
				ds->setWindowMaxsize(int1);
			}
			if(!ds->getWindowMaxsize())
			{
				if(cfg_script.Get("screenWidth", int1))
				{
					ds->setScreenWidth(int1);
				}
				if(cfg_script.Get("screenHeight", int1))
				{
					ds->setScreenHeight(int1);
				}
			}
			else
			{
				ds->setScreenWidth(screenWidth);
				ds->setScreenHeight(screenHeight);
			}
#ifdef _WIN32

			if(ds->getFullScreen())
			{
				if(!CREncapsulation::crStartHandler::changeDisplaySettings(ds->getScreenWidth(),ds->getScreenHeight(),ds->getColorBits()))
					ds->setFullScreen(false);
			}
			else
			{
				ChangeDisplaySettings(NULL,0);
			}
#endif
			EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
			screenWidth = dm.dmPelsWidth;
			screenHeight = dm.dmPelsHeight;
			if(ds->getWindowMaxsize())
			{
				ds->setScreenWidth(screenWidth);
				ds->setScreenHeight(screenHeight);
			}
			if(!ds->getFullScreen())
			{
				int dsWidth = ds->getScreenWidth();
				int dsHeight = ds->getScreenHeight();
				int shellH = 0;
				RECT   Rect; 
				HWND   hWnd = FindWindow("Shell_TrayWnd", NULL);
				if(GetWindowRect(hWnd,&Rect))   
				{ 
					shellH = Rect.bottom-Rect.top;//   就是任务栏的高度 
					if(shellH >= screenHeight)
						shellH = 0;
				}
				if(dsHeight + shellH >= screenHeight)
				{
					dsHeight = screenHeight - shellH;
					ds->setScreenHeight(dsHeight);
				}
				if(dsWidth > screenWidth)
				{
					dsWidth = screenWidth;
					ds->setScreenWidth(dsWidth);
				}
				if(ds->getUseBoard())
				{
					int h = GetSystemMetrics(SM_CYCAPTION);
					int xFrame = GetSystemMetrics(SM_CXFRAME);
					int yFrame = GetSystemMetrics(SM_CYFRAME);
#if(_WIN32_WINNT >= 0x0600)
					yFrame += GetSystemMetrics(SM_CXPADDEDBORDER);
					xFrame += GetSystemMetrics(SM_CXPADDEDBORDER);
#endif
					dsHeight -= h+yFrame*2;
					dsWidth -= xFrame*2;
					char gbuf[256];
					sprintf(gbuf,"xFrame=%d dsHeight=%d dsWidth=%d\n\0",xFrame,dsHeight,dsWidth);
					gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
					//CRCore::notify(CRCore::FATAL)<<"xFrame="<<xFrame<<" dsHeight="<<dsHeight<<" dsWidth="<<dsWidth<<std::endl;
				}
				ds->setViewSize(dsWidth,dsHeight);
			}
			else
			{
				ds->setViewSize(ds->getScreenWidth(),ds->getScreenHeight());
			}
		}
		if(cfg_script.Get("Vsync", int1))
		{
			ds->setVsync(int1);
		}
		if(cfg_script.Get("FPSControl", flt1))
		{//60 30 0
			if(ds->getRunMode() >= crDisplaySettings::Robot)
				ds->initFpsControl(10.0f);
			else
				ds->initFpsControl(flt1);
			ds->restoreFpsControl();
		}
		if(cfg_script.Get("UIWidth", flt1))
		{
			ds->setUIDesignWidth(flt1);
		}
		if(cfg_script.Get("UIHeight", flt1))
		{
			ds->setUIDesignHeight(flt1);
		}
		if(cfg_script.Get("UIScaleMode", int1))
		{
			ds->setUIScaleMode(int1);
		}
		if(cfg_script.Get("UIScaleFactor", flt1))
		{
			ds->setUIScaleFactor(flt1);
		}
		if(cfg_script.Get("screenDistance", flt1))
		{
			ds->setScreenDistance(flt1);
		}
		if(cfg_script.Get("Fovy", flt1))
		{
			ds->setFovy(flt1);
		}
		if(cfg_script.Get("AspectRatio", flt1))
		{
			ds->setAspectRatio(flt1);
		}
		//if(cfg_script.Get("Clip", v_i))
		//{
		//	ds->setClip(crVector2(v_i[0],v_i[1]));
		//}
		//if(cfg_script.Get("Shear", v_i))
		//{
		//	ds->setShear(crVector2(v_i[0],v_i[1]));
		//}
		if(cfg_script.Get("displayType", str))
		{
			if(str.compare("MONITOR")==0)
			{
				ds->setDisplayType(crDisplaySettings::MONITOR);
			}
			else if(str.compare("POWERWALL")==0)
			{
				ds->setDisplayType(crDisplaySettings::POWERWALL);
			}
			else if(str.compare("REALITY_CENTER")==0)
			{
				ds->setDisplayType(crDisplaySettings::REALITY_CENTER);
			}
			else if(str.compare("HEAD_MOUNTED_DISPLAY")==0)
			{
				ds->setDisplayType(crDisplaySettings::HEAD_MOUNTED_DISPLAY);
			}
		}
		if(cfg_script.Get("stereo", int1))
		{
			ds->setStereo(int1);
		}
		if(cfg_script.Get("stereoMode", str))
		{
			if(str.compare("QUAD_BUFFER")==0)
			{
				ds->setStereoMode(crDisplaySettings::QUAD_BUFFER);
			}
			else if(str.compare("ANAGLYPHIC")==0)
			{
				ds->setStereoMode(crDisplaySettings::ANAGLYPHIC);
			}
			else if(str.compare("HORIZONTAL_SPLIT")==0)
			{
				ds->setStereoMode(crDisplaySettings::HORIZONTAL_SPLIT);
			}
			else if(str.compare("VERTICAL_SPLIT")==0)
			{
				ds->setStereoMode(crDisplaySettings::VERTICAL_SPLIT);
			}
			else if(str.compare("LEFT_EYE")==0)
			{
				ds->setStereoMode(crDisplaySettings::LEFT_EYE);
			}
			else if(str.compare("RIGHT_EYE")==0)
			{
				ds->setStereoMode(crDisplaySettings::RIGHT_EYE);
			}
			else if(str.compare("HORIZONTAL_INTERLACE")==0)
			{
				ds->setStereoMode(crDisplaySettings::HORIZONTAL_INTERLACE);
			}
			else if(str.compare("VERTICAL_INTERLACE")==0)
			{
				ds->setStereoMode(crDisplaySettings::VERTICAL_INTERLACE);
			}
		}
		if(cfg_script.Get("eyeSeparation", flt1))
		{
			ds->setEyeSeparation(flt1);
		}
		if(cfg_script.Get("splitStereoHorizontalEyeMapping", str))
		{
			if(str.compare("LEFT_EYE_LEFT_VIEWPORT")==0)
			{
				ds->setSplitStereoHorizontalEyeMapping(crDisplaySettings::LEFT_EYE_LEFT_VIEWPORT);
			}
			else if(str.compare("LEFT_EYE_RIGHT_VIEWPORT")==0)
			{
				ds->setSplitStereoHorizontalEyeMapping(crDisplaySettings::LEFT_EYE_RIGHT_VIEWPORT);
			}
		}
		if(cfg_script.Get("splitStereoHorizontalSeparation", int1))
		{
			ds->setSplitStereoHorizontalSeparation(int1);
		}
		if(cfg_script.Get("splitStereoVerticalEyeMapping", str))
		{
			if(str.compare("LEFT_EYE_TOP_VIEWPORT")==0)
			{
				ds->setSplitStereoVerticalEyeMapping(crDisplaySettings::LEFT_EYE_TOP_VIEWPORT);
			}
			else if(str.compare("LEFT_EYE_BOTTOM_VIEWPORT")==0)
			{
				ds->setSplitStereoVerticalEyeMapping(crDisplaySettings::LEFT_EYE_BOTTOM_VIEWPORT);
			}
		}
		if(cfg_script.Get("splitStereoVerticalSeparation", int1))
		{
			ds->setSplitStereoVerticalSeparation(int1);
		}
		if(cfg_script.Get("splitStereoAutoAdjustAspectRatio", int1))
		{
			ds->setSplitStereoAutoAjustAspectRatio(int1);
		}
		if(cfg_script.Get("doubleBuffer", int1))
		{
			ds->setDoubleBuffer(int1);
		}
		if(cfg_script.Get("RGB", int1))
		{
			ds->setRGB(int1);
		}
		if(cfg_script.Get("depthBuffer", int1))
		{
			ds->setDepthBuffer(int1);
		}
		if(cfg_script.Get("minimumNumberAlphaBits", int1))
		{
			ds->setMinimumNumAlphaBits(int1);
		}
		if(cfg_script.Get("minimumNumberStencilBits", int1))
		{
			ds->setMinimumNumStencilBits(int1);
		}
		if(cfg_script.Get("minimumNumberAccumBits",v_i))
		{
			if(v_i.size()==4)
				ds->setMinimumNumAccumBits(v_i[0],v_i[1],v_i[2],v_i[3]);
		}
		if(cfg_script.Get("maxNumOfGraphicsContexts", int1))
		{
			ds->setMaxNumberOfGraphicsContexts(int1);
		}
		if(cfg_script.Get("colorBits", int1))
		{
			ds->setColorBits(int1);
		}
		if(cfg_script.Get("depthBits", int1))
		{
			ds->setDepthBits(int1);
		}
		if(cfg_script.Get("numMultiSamples", int1))
		{
			ds->setNumMultiSamples(int1);
		}
		if(cfg_script.Get("MaxAnisotropy", int1))
		{
			ds->setMaxAnisotropy(int1);
		}
		if(cfg_script.Get("Brightness", flt1))
		{
			ds->setBrightness(flt1);
		}
		//if(cfg_script.Get("AmbientIntensity", v_i))
		//{
		//	if(v_i.size()==4)
		//		ds->setAmbientIntensity(crVector4(v_i[0],v_i[1],v_i[2],v_i[3]));
		//}
		if(cfg_script.Get("HDR", int1))
		{
			ds->setHDR(int1);
		}
		//if(cfg_script.Get("SeparateFilter", int1))
		//{
		//	ds->setSeparateFilter(int1);
		//}
		if(cfg_script.Get("GlowSize", flt1))
		{
			ds->setGlowSize(flt1);
		}
		if(cfg_script.Get("EnableAlphaShadow", int1))
		{
			ds->setEnableAlphaShadow(int1);
		}
		if(cfg_script.Get("NeedPixelDepth", int1))
		{
			ds->setNeedPixelDepth(int1);
		}
		//if(cfg_script.Get("NeedNormalDistortion", int1))
		//{
		//	ds->setNeedNormalDistortionBuffer(int1);
		//}
		if(cfg_script.Get("DepthOfField", int1))
		{
			ds->setDepthOfField(int1);
		}
		//if(cfg_script.Get("DofBlurFactor", flt1))
		//{
		//	ds->setDofBlur(flt1);
		//}
		if(cfg_script.Get("OutLine", int1))
		{
			ds->setOutLine(int1);
		}
		if(cfg_script.Get("OutLineColor", v_i))
		{
			if(v_i.size()==3)
				ds->setOutLineColor(crVector3(v_i[0]/255.0f,v_i[1]/255.0f,v_i[2]/255.0f));
		}
		if(cfg_script.Get("OutLineWidth", flt1))
		{
			ds->setOutLineWidth(flt1);
		}
		if(cfg_script.Get("OutLineThreshold", flt1))
		{
			ds->setOutLineThreshold(flt1);
		}
		if(cfg_script.Get("OutLineDispDistance", flt1))
		{
			ds->setOutLineDispDistance(flt1);
		}
		if(cfg_script.Get("SelectOutLineColor", v_i))
		{
			if(v_i.size()==3)
				ds->setSelectOutLineColor(crVector3(v_i[0]/255.0f,v_i[1]/255.0f,v_i[2]/255.0f));
		}
		if(cfg_script.Get("CharacterDispDistance", flt1))
		{
			ds->setCharacterDispDistance(flt1);
		}
		if(cfg_script.Get("CharacterShadowDistance", flt1))
		{
			ds->setCharacterShadowDistance(flt1);
		}
		if(cfg_script.Get("CharacterUpdateFactor", flt1))
		{
			ds->setCharacterUpdateFactor(flt1);
		}
		if(cfg_script.Get("CharacterNearDistance", flt1))
		{
			ds->setCharacterNearDistance(flt1);
		}
		if(cfg_script.Get("CharacterFarDistance", flt1))
		{
			ds->setCharacterFarDistance(flt1);
		}
		if(cfg_script.Get("MotionBlur", int1))
		{
			ds->setMotionBlur(int1);
		}
		//if(cfg_script.Get("MotionBlurFactor", flt1))
		//{
		//	ds->setMotionBlurFactor(flt1);
		//}
		if(cfg_script.Get("ShadowDetail", int1))
		{
			ds->setShadowDetail(int1);
		}
		if(cfg_script.Get("TextureDetail", int1))
		{
			ds->setTextureDetail(int1);
		}
		//if(cfg_script.Get("WaveBlend", int1))
		//{
		//	ds->setWaveBlend(int1);
		//}
		//if(cfg_script.Get("WaveAmp", flt1))
		//{
		//	ds->setWaveAmp(flt1);
		//}
		//if(cfg_script.Get("WaveScaleSpeed", v_i))
		//{
		//	if(v_i.size()==2)
		//		ds->setWaveScaleSpeed(crVector2(v_i[0],v_i[1]));
		//}
		//if(cfg_script.Get("NoiseSample", v_i))
		//{
		//	if(v_i.size()==4)
		//		ds->setNoiseSample(crVector4(v_i[0],v_i[1],v_i[2],v_i[3]));
		//}
		//if(cfg_script.Get("MaxDepth", flt1))
		//{
		//	ds->setMaxDepth(flt1);
		//}
		//if(cfg_script.Get("NPR", int1))
		//{
		//	ds->setNPR(int1);
		//}
		//if(cfg_script.Get("NprParam", v_i))
		//{
		//	if(v_i.size()==3)
		//		ds->setNprParam(crVector3(v_i[0],v_i[1],v_i[2]));
		//}
		//if(cfg_script.Get("HdrParam", v_i))
		//{
		//	if(v_i.size()==4)
		//		ds->setHdrParam(crVector4(v_i[0],v_i[1],v_i[2],v_i[3]));
		//}
		if(cfg_script.Get("MouseSpeed", flt1))
		{
			ds->setMouseSpeed(flt1);
		}
		if(cfg_script.Get("DisableShader", int1))
		{
			ds->setDisableShader(int1);
		}
		///声音设置
		if(cfg_script.Get("DisableSound", int1))
		{
			ds->setDisableSound(int1);
		}
		if(cfg_script.Get("AmbSoundVolume", flt1))
		{
			ds->setAmbSoundVolume(flt1);
		}
		if(cfg_script.Get("FxSoundVolume", flt1))
		{
			ds->setFxSoundVolume(flt1);
		}
		//if(cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName()))
		//{
		//	//if(cfg_script.Get("Clip", v_i))
		//	//{
		//	//	ds->setClip(crVector2(v_i[0],v_i[1]));
		//	//}
		//	//if(cfg_script.Get("AmbientIntensity", v_i))
		//	//{
		//	//	if(v_i.size()==4)
		//	//		ds->setAmbientIntensity(crVector4(v_i[0],v_i[1],v_i[2],v_i[3]));
		//	//}
		//	//if(cfg_script.Get("Fog", int1))
		//	//{
		//	//	ds->setFog(int1);
		//	//}
		//	//if(cfg_script.Get("FogColor", v_i))
		//	//{
		//	//	if(v_i.size()==3)
		//	//		ds->setFogColor(crVector3(v_i[0],v_i[1],v_i[2]));
		//	//}
		//	//if(cfg_script.Get("FogStartEndDensity", v_i))
		//	//{
		//	//	if(v_i.size()==3)
		//	//		ds->setFogParams(crVector3(v_i[0],v_i[1],v_i[2]));
		//	//}
		//	//if(cfg_script.Get("WaveBlend", int1))
		//	//{
		//	//	ds->setWaveBlend(int1);
		//	//}
		//	//if(cfg_script.Get("WaveAmp", flt1))
		//	//{
		//	//	ds->setWaveAmp(flt1);
		//	//}
		//	//if(cfg_script.Get("WaveScaleSpeed", v_i))
		//	//{
		//	//	if(v_i.size()==2)
		//	//		ds->setWaveScaleSpeed(crVector2(v_i[0],v_i[1]));
		//	//}
		//	//if(cfg_script.Get("NoiseSample", v_i))
		//	//{
		//	//	if(v_i.size()==4)
		//	//		ds->setNoiseSample(crVector4(v_i[0],v_i[1],v_i[2],v_i[3]));
		//	//}
		//	//if(cfg_script.Get("MaxDepth", flt1))
		//	//{
		//	//	ds->setMaxDepth(flt1);
		//	//}
		//	//if(cfg_script.Get("DofBlurFactor", flt1))
		//	//{
		//	//	ds->setDofBlur(flt1);
		//	//}
		//	//if(cfg_script.Get("MotionBlurFactor", flt1))
		//	//{
		//	//	ds->setMotionBlurFactor(flt1);
		//	//}
		//	if(cfg_script.Get("GlowSize", flt1))
		//	{
		//		ds->setGlowSize(flt1);
		//	}
		//	//if(cfg_script.Get("NprParam", v_i))
		//	//{
		//	//	if(v_i.size()==3)
		//	//		ds->setNprParam(crVector3(v_i[0],v_i[1],v_i[2]));
		//	//}
		//	if(cfg_script.Get("HdrParam", v_i))
		//	{
		//		if(v_i.size()==4)
		//			ds->setHdrParam(crVector4(v_i[0],v_i[1],v_i[2],v_i[3]));
		//	}
		//	//int initLoadPage = 1;
		//	//while(cfg_script.Get("InitLoadPage", str,initLoadPage++))
		//	//{
		//	//	CRCore::crBrain::getInstance()->addInitLoadPage(str);
		//	//}
		//	if (!cfg_script.Pop())
		//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadInitCfg(): "<<cfg_script.GetLastError()<<std::endl;
		//}

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadInitCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}
	//if(!wantScreanSize)
	//{
	//	CREncapsulation::createStatsText();
	//	//CREncapsulation::createLogoText();
	//}
}

void CREncapsulation::loadStageCfg(const std::string &filename)
{
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
	{
		CRCore::notify(CRCore::FATAL)<<"loadStageCfg(): file open error,"<<filename<<std::endl;
		return;
	}

	std::string str;
	int nstage = 1;
	CRCore::ref_ptr<CRGUI::crImageStage> stage;
	while (cfg_script.Push("Stage", nstage++)) 
	{
		if (cfg_script.Get("ImageStage", str))
		{
			if(str.find("loading")==std::string::npos && crDisplaySettings::instance()->getRunMode()!=crDisplaySettings::Editor)
			{
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadStageCfg(): "<<cfg_script.GetLastError()<<std::endl;
				continue;
			}
			stage = CRGUI::crImageStage::testLoad(str);
			if(!stage.valid())
			{
				stage  = new CRGUI::crImageStage;
				stage->load(str);
			}
			if(stage.valid()) CRGUI::cr2DStageManager::getInstance()->addStage(stage.get());
		}

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadStageCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}

	CRGUI::crElement::clearBmpCache();
}

void CREncapsulation::loadGameStageCfg(const std::string &path)
{
	std::string filename;
	if(path.empty())
		filename = "script/gamestage.cfg";
	else
		filename = path + "/script/gamestage.cfg";

	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
	{
		CRCore::notify(CRCore::FATAL)<<"loadGameStageCfg(): file open error,"<<filename<<std::endl;
		return;
	}

	//cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());
	std::string str;
	int nstage = 1;
	CRCore::ref_ptr<CRGUI::crImageStage> stage;
	while (cfg_script.Push("Stage", nstage)) 
	{
		if (cfg_script.Get("ImageStage", str))
		{
			//stage = CRGUI::crImageStage::testLoad(str);
			//if(!stage.valid())
			//{
			//	stage  = new CRGUI::crImageStage;
			//	stage->load(str);
			//}
			stage  = new CRGUI::crImageStage;
			stage->load(str);
		}

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadGameStageCfg(): "<<cfg_script.GetLastError()<<std::endl;

		if(stage.valid()) CRGUI::cr2DStageManager::getInstance()->addStage(stage.get());
		nstage++;
	}

	//cfg_script.Pop();
	CRGUI::crElement::clearBmpCache();
}

//void CREncapsulation::loadSoundCfg()
//{
//	CRAL::crSoundManager::instance()->loadSound("script/sounds.cfg");
//}

//void CREncapsulation::loadLightCfg()
//{///////注:如果实时阴影在相机运动的时候产生不正常的抖动，可能时因为光源节点没有在LSGroup节点下。
//	rcfg::ConfigScript cfg_script;
//
//	struct stat buf;
//	if (stat( "script/light.cfg", &buf )) 
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadLightCfg(): file open error"<<std::endl;
//		return;
//	}
//
//	if (!cfg_script.Open("script/light.cfg"))
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadLightCfg(): "<<cfg_script.GetLastError()<<std::endl;
//		return;
//	}
//	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());
//	std::string str;
//	std::vector<float> v_i;
//	int int1;
//	//float flt1;
//	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
//	crLightSource *lightSource;
//	crNodeCallback *callback;
//	int nLight = 1;
//	int nAvoidLightObj = 1;
//	int nLightCullback = 1;
//	int nParam = 1;
//	crObject *obj;
//	crTransform* transform;
//	crGroup *lsGroup;
//	//crLightSource::ShadowLightProxy *slp;
//	//if(cfg_script.Get("SunShadowDistance", flt1))
//	//	crLightSourceManager::getInstance()->setSunShadowDistance(flt1);
//	while(cfg_script.Push("light",nLight++))
//	{
//		cfg_script.Get("lightNameID", str);
//		searchByNameVisitor.reset();
//		searchByNameVisitor.setNameId(str);
//		searchByNameVisitor.setSearchNodeType(crSearchNodeBYNameVisitor::LIGHTSOURCE);
//		CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
//		lightSource = dynamic_cast<crLightSource*>(searchByNameVisitor.getResult());
//		if( !lightSource)
//		{
//			if (!cfg_script.Pop())
//				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadLightCfg(): "<<cfg_script.GetLastError()<<std::endl;
//			continue;
//		}
//		crLight *light = dynamic_cast<crLight *>(lightSource->getLight());
//		if(!light)//平行光不计算阴影
//		{
//			continue;
//		}
//		//else
//		//	crLightSourceManager::getInstance()->insertLightSource(lightSource);
//
//		//lightSource->initLightParamStruct();
//
//		if(cfg_script.Get("MoveLightWithEyePoint",int1) && int1)
//		{
//			transform = new MoveLightWithEyePointTransform();
//			transform->setName("EyePointTransform");
//			transform->addDescription("#ScriptAddGroup");
//			transform->addChild(lightSource);
//			lsGroup = lightSource->getParent(0);
//			if(lsGroup->getName() != "LSGroup")
//			{
//				CRCore::notify(CRCore::WARN)<<"CREncapsulation::loadLightCfg(): 光源不在LSGroup节点下，检查LSGroup节点有没有设定[NameID]"<<cfg_script.GetLastError()<<std::endl; 
//			}
//			lsGroup->replaceChild(lightSource,transform);
//		}
//		//if(cfg_script.Push("ShadowLightProxy"))
//		//{
//		//	slp = new crLightSource::ShadowLightProxy;
//		//	if(cfg_script.Get("ShadowLightPos", v_i))
//		//		slp->m_shadowLightPos.set(v_i[0],v_i[1],v_i[2]);
//		//	if(cfg_script.Get("ShadowLightLength", flt1))
//		//		slp->m_shadowLightLength = flt1;
//		//	
//		//	lightSource->setShadowLightProxy(slp);
//
//		//	cfg_script.Pop();
//		//}
//		if(cfg_script.Get("lightEnable", int1))
//			lightSource->setEnable(int1);
//		if(cfg_script.Get("lightStdModeEnable", int1))
//			lightSource->setStdModeEnable(int1);
//		if(cfg_script.Get("lightStdModeNumber", int1))
//			light->setLightNum(int1);
//
//		if(cfg_script.Get("lightCalcShadow", int1))
//			lightSource->setCalcShadow(int1 && crDisplaySettings::instance()->getShadowDetail());
//
//		if(cfg_script.Get("StaticShadowMapChannel", int1))
//			lightSource->setStaticShadowMapChannel(int1);
//
//		nLightCullback = 1;
//		while (cfg_script.Push("lightCullback", nLightCullback++))
//		{
//			cfg_script.Get("Type", str);
//			callback = dynamic_cast<crNodeCallback *>(crEventCallbackManager::getInstance()->getEventCallback(str));
//			if(!callback) 
//			{
//				CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadLightCfg(): 未找到NodeCallback: "<<str<<std::endl;
//				continue;
//			}
//			//if(str.compare("LightlumCallback") == 0)
//			//{
//			//	callback = crLightlumCallback::getInstance();
//			//}
//			nParam = 1;
//			while (cfg_script.Get("Param", str,nParam++)) 
//			{
//				callback->addEventParam(nParam-1,str);
//			}
//
//			if (!cfg_script.Pop())
//				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadLightCfg(): "<<cfg_script.GetLastError()<<std::endl;
//			lightSource->addCullCallback(callback);
//		}
//
//		nAvoidLightObj = 1;
//		while(cfg_script.Get("AvoidLightObject", str,nAvoidLightObj++))
//		{
//			searchByNameVisitor.reset();
//			searchByNameVisitor.setNameId(str);
//			searchByNameVisitor.setSearchNodeType(crSearchNodeBYNameVisitor::OBJECT);
//			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
//			obj = dynamic_cast<crObject*>(searchByNameVisitor.getResult());
//			if(obj)
//			{
//				//lightSource->addAvoidLightObject(obj);
//				obj->addAvoidLight(lightSource->getName());
//			}
//		}
//
//		nAvoidLightObj = 1;
//		while(cfg_script.Get("NeedLightObject", str,nAvoidLightObj++))
//		{
//			searchByNameVisitor.reset();
//			searchByNameVisitor.setNameId(str);
//			searchByNameVisitor.setSearchNodeType(crSearchNodeBYNameVisitor::OBJECT);
//			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
//			obj = dynamic_cast<crObject*>(searchByNameVisitor.getResult());
//			if(obj)
//			{
//				//lightSource->addNeedLightObject(obj);
//				obj->addNeedLight(lightSource->getName());
//			}
//		}
//
//		if (!cfg_script.Pop())
//			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadLightCfg(): "<<cfg_script.GetLastError()<<std::endl;
//	}
//
//	//int ndata = 1;
//	//crCameraNode *shadow_camera = crLightSourceManager::getInstance()->getShadowCameraNode();
//	//while(cfg_script.Get("shadowCameraChildData", str,ndata++))
//	//{
//	//    searchByNameVisitor.reset();
//	//    searchByNameVisitor.setNameId(str);
//	//   	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
//	//   	shadow_camera->addChild(searchByNameVisitor.getResult());
//	//}
//	cfg_script.Pop();
//}

void CREncapsulation::loadRenderToTextureCfg(const std::string &path)
{
	std::string filename;
	if(path.empty())
		filename = "script/renderToTexture.cfg";
	else
		filename = path + "/script/renderToTexture.cfg";
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
	{
		CRCore::notify(CRCore::FATAL)<<"loadRenderToTextureCfg(): file open error,"<<filename<<std::endl;
		return;
	}

	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());
	std::string str;
	std::vector<float> v_i;
	float flt1;
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
	ref_ptr<crRenderToTexture> rtt;
	crLightSource *lightSource;
	int nRtt = 1;
	int ndata;
	//float bias = 0.0f;
	float blursize = 10.0f; 
	while(cfg_script.Push("RenderToTexture",nRtt++))
	{
		int mode = 0;
		if(cfg_script.Get("RenderToTextureMode", str))
		{
			if(str.find("RenderToImage") != -1)
				mode |= crRenderToTexture::RenderToImage;
			if(str.find("RenderToTexture") != -1)
				mode |= crRenderToTexture::RenderToTexture;
			if(str.find("RenderToFrame") != -1)
				mode |= crRenderToTexture::RenderToFrame;
			if(str.find("RenderDepthMap") != -1)
			{
				if(crDisplaySettings::instance()->getShadowDetail() == 0)
				{
					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadRenderToTextureCfg(): "<<cfg_script.GetLastError()<<std::endl;
					continue;
				}
				mode |= crRenderToTexture::RenderDepthMap;
			}
			if(str.find("RenderColorMap") != -1)
				mode |= crRenderToTexture::RenderColorMap;
			if(str.find("RenderCubeMap") != -1)
			{
				rtt = new crRenderToCubeTexture;
				mode |= crRenderToTexture::RenderCubeMap;
			}
			else
			{
				rtt = new crRenderToTexture;
			}
			if(str.find("NeedBlur") != -1)
				mode |= crRenderToTexture::NeedBlur;
			if(str.find("RealTimeUpd") != -1)
				mode |= crRenderToTexture::RealTimeUpd;
			if(str.find("UpdOnce") != -1)
				mode |= crRenderToTexture::UpdOnce;
			if(str.find("UpdByExtern") != -1)
				mode |= crRenderToTexture::UpdByExtern;
			if(str.find("CalcMVPBySetting") != -1)
				mode |= crRenderToTexture::CalcMVPBySetting;
			else if(str.find("CalcMVPByRenderData") != -1)
				mode |= crRenderToTexture::CalcMVPByRenderData;
			else if(str.find("CalcMVPByEyePoint2") != -1)
				mode |= crRenderToTexture::CalcMVPByEyePoint2;
			else if(str.find("CalcMVPByEyePoint") != -1)
				mode |= crRenderToTexture::CalcMVPByEyePoint;
			else if(str.find("CalcMVPByShadowRadius") != -1)
				mode |= crRenderToTexture::CalcMVPByShadowRadius;
			else if(str.find("ExternMVP") != -1)
				mode |= crRenderToTexture::ExternMVP;
		}

		rtt->setMode((crRenderToTexture::RenderToTextureMode)mode);

		cfg_script.Get("Name", str);
		rtt->setName(str);

		if(cfg_script.Get("OutPutImageFileName", str))
		{
			rtt->setOutPutImageName(str);
			rtt->setWriteImageCallback(new MyCameraPostDrawCallback);
		}
		if(cfg_script.Get("lightNameID", str))
		{
			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId(str);
			searchByNameVisitor.setSearchNodeType(LIGHTSOURCE);
			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
			lightSource = dynamic_cast<crLightSource*>(searchByNameVisitor.getResult());
			if( !lightSource)
			{
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadRenderToTextureCfg(): "<<cfg_script.GetLastError()<<std::endl;
				continue;
			}
			crLight *light = dynamic_cast<crLight *>(lightSource->getLight());
			if(!light||light->getLightType()==crLight::LIGHT_INFINITE)//平行光不计算阴影
			{
				CRCore::notify(CRCore::WARN)<<"CREncapsulation::loadRenderToTextureCfg(): 平行光不计算阴影"<<std::endl;
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadRenderToTextureCfg(): "<<cfg_script.GetLastError()<<std::endl;
				continue;
			}

			rtt->setLightSource(lightSource);
			lightSource->addCullCallback(rtt.get());			
		}
		/*else
		{
			crBrain::getInstance()->getCurrentActiveDB()->addCullCallback(rtt.get());
		}*/
		if(crDisplaySettings::instance()->getShadowDetail()<=1)
		{
			if(cfg_script.Get("LowMapSize", v_i))
			{
				rtt->setMapSize(v_i[0],v_i[1]);
			}
			else
			{
				if(cfg_script.Get("MapSize", v_i))
				{
					rtt->setMapSize(v_i[0],v_i[1]);
				}
			}
		}
		else
		{
			if(cfg_script.Get("MapSize", v_i))
			{
				rtt->setMapSize(v_i[0],v_i[1]);
			}
		}
		if(cfg_script.Get("CameraPos", v_i))
		{
			rtt->setCameraPos(crVector3(v_i[0],v_i[1],v_i[2]));
		}
		if(cfg_script.Get("CameraDir", v_i))
		{
			rtt->setCameraDir(crVector3(v_i[0],v_i[1],v_i[2]));
		}
		if(cfg_script.Get("CameraUp", v_i))
		{
			rtt->setCameraUp(crVector3(v_i[0],v_i[1],v_i[2]));
		}
		if(cfg_script.Get("CameraFov", flt1))
		{
			rtt->setCameraFov(flt1);
		}
		if(cfg_script.Get("LightRange", flt1))
		{
			rtt->setLightRange(flt1);
		}
		if(cfg_script.Get("ShadowRadius", flt1))
		{
			rtt->setShadowRadius(flt1);
		}
		if(cfg_script.Get("ShadowOffset", flt1))
		{
			rtt->setShadowOffset(flt1);
		}
		if(cfg_script.Get("UpdateInterval", flt1))
		{
			rtt->setUpdateInterval(flt1);
		}
		ndata = 1;
		while (cfg_script.Get("RenderData", str,ndata++))
		{
			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId(str);
			searchByNameVisitor.setSearchNodeType(ALLNODE);
			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
			if(!searchByNameVisitor.getResult())
			{
				CRCore::notify(CRCore::FATAL)<<"error to find RenderData str =  "<<str<<std::endl;
				continue;
			}
			//CRCore::notify(CRCore::FATAL)<<"find RenderData str =  "<<str<<std::endl;
			rtt->addRenderNode(searchByNameVisitor.getResult());
		}

		ndata = 1;
		while (cfg_script.Get("TexturedData", str,ndata++))
		{
			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId(str);
			searchByNameVisitor.setSearchNodeType(ALLNODE);
			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
			if(!searchByNameVisitor.getResult())
				CRCore::notify(CRCore::FATAL)<<"error to find TexturedData str =  "<<str<<std::endl;
			rtt->addTexturedNode(searchByNameVisitor.getResult());
		}

		//if (cfg_script.Push("RenderMaterial", 1)) 
		//{   
		//	int addT = 0;
		//	int addB = 0;
		//	cfg_script.Get("needTangent", addT);
		//	cfg_script.Get("needBinormal", addB);
		//	if(cfg_script.Get("glsl", str))
		//	{
		//		//CRCore::notify(CRCore::FATAL)<<"RenderMaterial:glsl= "<<str<<std::endl;
		//		rtt->setRenderProgrom(str,addT,addB);
		//	}
		//	if (!cfg_script.Pop())
		//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadRenderToTextureCfg(): "<<cfg_script.GetLastError()<<std::endl;
		//}

		//cfg_script.Get("ShadowBias", bias);
		cfg_script.Get("BlurSize", blursize);
		if(rtt->getName().compare("GITexture")==0)
		{
			const crVector2i &mapSize = crBrain::getInstance()->getMapSize();
			rtt->setMapSize(mapSize);
		}
		rtt->init(/*bias,*/blursize);
		if(rtt->getName().compare("GITexture")==0)
		{
			if(!crDisplaySettings::instance()->getHDR())
			{
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadRenderToTextureCfg(): "<<cfg_script.GetLastError()<<std::endl;
				continue;
			}
			/*crVector3i worldSize(256,256,10);
			if(cfg_script.Get("WorldSize", v_i))
			{
				worldSize.set(v_i[0],v_i[1],v_i[2]);
			}*/
			const crVector3i &worldSize = crBrain::getInstance()->getWorldSize();
			rtt->setProjectionMatrix(CRCore::crMatrix::ortho(-worldSize[0]*0.5f,worldSize[0]*0.5f,-worldSize[1]*0.5f,worldSize[1]*0.5f,-worldSize[2],worldSize[2]));
			rtt->setViewMatrix(crMatrix::identity());
			crShaderManager::getInstance()->setGITexture(rtt->getTexture());
			crShaderManager::getInstance()->setGIParam(crVector4(worldSize[0]*0.5f,worldSize[1]*0.5f,1.0f/float(worldSize[0]),1.0f/float(worldSize[1])));
			//crShaderManager::getInstance()->setGIMVP(rtt->getView() * rtt->getProjection());
			if(crBrain::getInstance()->getFog()==1)
			{
				crStateSet *ss = rtt->getRender()->getOrCreateStateSet();
				crUniform *uniform = ss->getOrCreateUniform("fog_params",CRCore::crUniform::FLOAT_VEC4);
				uniform->setDataVariance(crBase::STATIC);
				uniform->set(crVector4(0,0,0,0));
			}
		}
		crRenderToTextureManager::getInstance()->addRenderToTexture(rtt.get());

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadRenderToTextureCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}
	cfg_script.Pop();
}

void CREncapsulation::createLogoText()
{
	float leftPos = 10.0f;
	float characterSize = 15.0f;
	CRCore::crVector3 pos(leftPos,100.0f,0.0f);
	CRCore::crVector4 color(1.0f,1.0f,0.0f,1.0f);

	CRText::crText* text = new CRText::crText;
	text->setFont("arial.ttf");
	text->setColor(color);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("=========================");
	//text->setText("Creature3D");
	crFilterRenderManager::getInstance()->addDrawable(text);

	pos.y()-=characterSize;
	text = new CRText::crText;
	text->setFont("arial.ttf");//fonts/SIMSUN.TTC
	text->setColor(color);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("Creature3D Game Engine");//L"Copyright(C) 2005 Wucaihua"
	crFilterRenderManager::getInstance()->addDrawable(text);

	pos.y()-=characterSize;
	text = new CRText::crText;
	text->setFont("arial.ttf");
	text->setColor(color);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("Copyright(C) 2005 WuCaihua");
	crFilterRenderManager::getInstance()->addDrawable(text);

	pos.y()-=characterSize;
	text = new CRText::crText;
	text->setFont("arial.ttf");
	text->setColor(color);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("http://blog.sina.com.cn/creature3d");
	crFilterRenderManager::getInstance()->addDrawable(text);


	pos.y()-=characterSize;
	text = new CRText::crText;
	text->setFont("arial.ttf");
	text->setColor(color);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("Email:caitht@sina.com");
	crFilterRenderManager::getInstance()->addDrawable(text);


	pos.y()-=characterSize;
	text = new CRText::crText;
	text->setFont("arial.ttf");
	text->setColor(color);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("QQ:26756325");
	crFilterRenderManager::getInstance()->addDrawable(text);

	pos.y()-=characterSize;
	text = new CRText::crText;
	text->setFont("arial.ttf");
	text->setColor(color);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("=========================");
	crFilterRenderManager::getInstance()->addDrawable(text);
}

void CREncapsulation::createStatsText()
{
	float leftPos = 10.0f;
	float characterSize = 15.0f;
	float screenHeight = crDisplaySettings::instance()->getViewHeight();
	//if(!crDisplaySettings::instance()->getFullScreen())
	//    screenHeight -= GetSystemMetrics(SM_CYSIZE);
	CRCore::crVector3 pos(leftPos,screenHeight - 200.0f,0.0f);

	CRCore::crVector4 colorFR(1.0f,0.0f,0.0f,1.0f);
	CRCore::crVector4 colorFrameTimes( 0.0f,1.0f,0.0f,1.0f);
	CRCore::crVector4 colorDraw( 0.0f,1.0f,1.0f,1.0f);
	CRCore::crVector4 colorReferDraw( 1.0f,1.0f,0.0f,1.0f);

	//////////////////////////
	//frameRateLabelText
	/////////////////////////
	CRText::crText* text = new CRText::crText;
	text->setName("frameRateLabelText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("Frame Rate: ");
	text->setVisiable(false);
	text->swapBuffers();//为获得正确的BoundBox
	crFilterRenderManager::getInstance()->addDrawable(text);

	//////////////////////////
	//frameRateCounterText
	/////////////////////////
	pos.x() = text->getBound().xMax();
	text = new CRText::crText;
	text->setName("frameRateCounterText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("0123456789.");
	text->setVisiable(false);
	crFilterRenderManager::getInstance()->addDrawable(text);

	//////////////////////////
	//frameTimeLabelText
	/////////////////////////
/*	pos.x() = leftPos;
	pos.y() -= characterSize;
	text = new CRText::crText;
	text->setName("frameTimeLabelText");
	text->setFont("arial.ttf");
	text->setColor(colorFrameTimes);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("Frame Time: ");
	text->setVisiable(false);
	crFilterRenderManager::getInstance()->addDrawable(text);

	//////////////////////////
	//frameTimeText
	/////////////////////////
	pos.x() = text->getBound().xMax();
	text = new CRText::crText;
	text->setName("frameTimeText");
	text->setFont("arial.ttf");
	text->setColor(colorFrameTimes);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("0123456789.");
	text->setVisiable(false);
	crFilterRenderManager::getInstance()->addDrawable(text);

	//////////////////////////
	//drawLabelText
	/////////////////////////
	pos.x() = leftPos;
	pos.y() -= characterSize;
	text = new CRText::crText;
	text->setName("drawLabelText");
	text->setFont("arial.ttf");
	text->setColor(colorDraw);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("Draw: ");
	text->setVisiable(false);
	crFilterRenderManager::getInstance()->addDrawable(text);

	//////////////////////////
	//drawTimeText
	/////////////////////////
	pos.x() = text->getBound().xMax();
	text = new CRText::crText;
	text->setName("drawTimeText");
	text->setFont("arial.ttf");
	text->setColor(colorDraw);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("1000.00");
	text->setVisiable(false);
	crFilterRenderManager::getInstance()->addDrawable(text);

	//////////////////////////
	//ReferDrawLabelText
	/////////////////////////
	pos.x() = text->getBound().xMax();
	text = new CRText::crText;
	text->setName("referDrawLabelText");
	text->setFont("arial.ttf");
	text->setColor(colorReferDraw);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("ReferDraw: ");
	text->setVisiable(false);
	crFilterRenderManager::getInstance()->addDrawable(text);

	//////////////////////////
	//ReferDrawTimeText
	/////////////////////////
	pos.x() = text->getBound().xMax();
	text = new CRText::crText;
	text->setName("referDrawTimeText");
	text->setFont("arial.ttf");
	text->setColor(colorReferDraw);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("1000.00");
	text->setVisiable(false);
	crFilterRenderManager::getInstance()->addDrawable(text);
*/
	//////////////////////////
	//stateLabelText
	/////////////////////////
	pos.x() = leftPos;
	pos.y() -= characterSize;
	text = new CRText::crText;
	text->setName("stateLabelText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("Number of state: ");
	text->setVisiable(false);
	text->swapBuffers();//为获得正确的BoundBox
	crFilterRenderManager::getInstance()->addDrawable(text);

	//////////////////////////
	//stateCounterText
	/////////////////////////
	pos.x() = text->getBound().xMax();
	text = new CRText::crText;
	text->setName("stateCounterText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("0123456789.");
	text->setVisiable(false);
	crFilterRenderManager::getInstance()->addDrawable(text);

	//////////////////////////
	//drawableLabelText
	/////////////////////////
	pos.x() = leftPos;
	pos.y() -= characterSize;
	text = new CRText::crText;
	text->setName("drawableLabelText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("Number of drawables: ");
	text->setVisiable(false);
	text->swapBuffers();//为获得正确的BoundBox
	crFilterRenderManager::getInstance()->addDrawable(text);

	//////////////////////////
	//drawableCounterText
	/////////////////////////
	pos.x() = text->getBound().xMax();
	text = new CRText::crText;
	text->setName("drawableCounterText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("0123456789.");
	text->setVisiable(false);
	crFilterRenderManager::getInstance()->addDrawable(text);

	//////////////////////////
	//vertexLabelText
	/////////////////////////
	pos.x() = leftPos;
	pos.y() -= characterSize;
	text = new CRText::crText;
	text->setName("vertexLabelText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("Number of vertex: ");
	text->setVisiable(false);
	text->swapBuffers();//为获得正确的BoundBox
	crFilterRenderManager::getInstance()->addDrawable(text);

	//////////////////////////
	//vertexCounterText
	/////////////////////////
	pos.x() = text->getBound().xMax();
	text = new CRText::crText;
	text->setName("vertexCounterText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("0123456789.");
	text->setVisiable(false);
	crFilterRenderManager::getInstance()->addDrawable(text);

	//////////////////////////
	//primLabelText
	/////////////////////////
	pos.x() = leftPos;
	pos.y() -= characterSize;
	text = new CRText::crText;
	text->setName("primLabelText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("Number of Primitive: ");
	text->setVisiable(false);
	text->swapBuffers();//为获得正确的BoundBox
	crFilterRenderManager::getInstance()->addDrawable(text);

	//////////////////////////
	//primCounterText
	/////////////////////////
	pos.x() = text->getBound().xMax();
	text = new CRText::crText;
	text->setName("primCounterText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("0123456789.");
	text->setVisiable(false);
	crFilterRenderManager::getInstance()->addDrawable(text);

	//////////////////////////
	//ParticleLabelText
	/////////////////////////
	pos.x() = leftPos;
	pos.y() -= characterSize;
	text = new CRText::crText;
	text->setName("particleLabelText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("Number of Particle: ");
	text->setVisiable(false);
	text->swapBuffers();//为获得正确的BoundBox
	crFilterRenderManager::getInstance()->addDrawable(text);

	//////////////////////////
	//ParticleCounterText
	/////////////////////////
	pos.x() = text->getBound().xMax();
	text = new CRText::crText;
	text->setName("particleCounterText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("0123456789.");
	text->setVisiable(false);
	crFilterRenderManager::getInstance()->addDrawable(text);
//#define ShowTime
//#ifdef ShowTime
	//////////////////////////
	//CullLabelText
	/////////////////////////
	pos.x() = leftPos;
	pos.y() -= characterSize;
	text = new CRText::crText;
	text->setName("cullLabelText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("CullTime: ");
	text->setVisiable(false);
	text->swapBuffers();//为获得正确的BoundBox
	crFilterRenderManager::getInstance()->addDrawable(text);
	//////////////////////////
	//CullTimeText
	/////////////////////////
	pos.x() = text->getBound().xMax();
	text = new CRText::crText;
	text->setName("cullTimeText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("0123456789.");
	text->setVisiable(false);
	crFilterRenderManager::getInstance()->addDrawable(text);

	//////////////////////////
	//DrawLabelText
	/////////////////////////
	pos.x() = leftPos;
	pos.y() -= characterSize;
	text = new CRText::crText;
	text->setName("drawLabelText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("DrawTime: ");
	text->setVisiable(false);
	text->swapBuffers();//为获得正确的BoundBox
	crFilterRenderManager::getInstance()->addDrawable(text);
	//////////////////////////
	//DrawTimeText
	/////////////////////////
	pos.x() = text->getBound().xMax();
	text = new CRText::crText;
	text->setName("drawTimeText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("0123456789.");
	text->setVisiable(false);
	crFilterRenderManager::getInstance()->addDrawable(text);

	//////////////////////////
	//UpdLabelText
	/////////////////////////
	pos.x() = leftPos;
	pos.y() -= characterSize;
	text = new CRText::crText;
	text->setName("updLabelText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("UpdTime: ");
	text->setVisiable(false);
	text->swapBuffers();//为获得正确的BoundBox
	crFilterRenderManager::getInstance()->addDrawable(text);
	//////////////////////////
	//UpdTimeText
	/////////////////////////
	pos.x() = text->getBound().xMax();
	text = new CRText::crText;
	text->setName("updTimeText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("0123456789.");
	text->setVisiable(false);
	crFilterRenderManager::getInstance()->addDrawable(text);

	//////////////////////////
	//PhyUpdLabelText
	/////////////////////////
	pos.x() = leftPos;
	pos.y() -= characterSize;
	text = new CRText::crText;
	text->setName("phyUpdLabelText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("PhysicsTime: ");
	text->setVisiable(false);
	text->swapBuffers();//为获得正确的BoundBox
	crFilterRenderManager::getInstance()->addDrawable(text);
	//////////////////////////
	//PhyTimeText
	/////////////////////////
	pos.x() = text->getBound().xMax();
	text = new CRText::crText;
	text->setName("phyTimeText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("0123456789.");
	text->setVisiable(false);
	crFilterRenderManager::getInstance()->addDrawable(text);
	//////////////////////////
	//ParticleUpdLabelText
	/////////////////////////
	pos.x() = leftPos;
	pos.y() -= characterSize;
	text = new CRText::crText;
	text->setName("partUpdLabelText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("ParticleTime: ");
	text->setVisiable(false);
	text->swapBuffers();//为获得正确的BoundBox
	crFilterRenderManager::getInstance()->addDrawable(text);
	//////////////////////////
	//PartTimeText
	/////////////////////////
	pos.x() = text->getBound().xMax();
	text = new CRText::crText;
	text->setName("phyTimeText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("0123456789.");
	text->setVisiable(false);
	crFilterRenderManager::getInstance()->addDrawable(text);
	//////////////////////////
	//CharacterUpdLabelText
	/////////////////////////
	pos.x() = leftPos;
	pos.y() -= characterSize;
	text = new CRText::crText;
	text->setName("charaUpdLabelText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("ParticleTime: ");
	text->setVisiable(false);
	text->swapBuffers();//为获得正确的BoundBox
	crFilterRenderManager::getInstance()->addDrawable(text);
	//////////////////////////
	//CharacterTimeText
	/////////////////////////
	pos.x() = text->getBound().xMax();
	text = new CRText::crText;
	text->setName("charaTimeText");
	text->setFont("arial.ttf");
	text->setColor(colorFR);
	text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
	text->setCharacterSize(characterSize);
	text->setPosition(pos);
	text->setAlignment(CRText::crText::BASE_LINE);
	text->setText("0123456789.");
	text->setVisiable(false);
	crFilterRenderManager::getInstance()->addDrawable(text);
//#endif
}

class crTime_VisableDCallback : public CRCore::crDrawable::UpdateCallback
{
public:

	crTime_VisableDCallback():
	  m_startFrameTime(0.0),
		  m_interval(1.0),
		  m_state(0)
	  {
	  }
	  void setInterval(float interval) { m_interval = interval; }

	  virtual void update(crNodeVisitor *visitor,crDrawable* drawable)
	  {
		  CRUtil::crUpdateVisitor* uv = dynamic_cast<CRUtil::crUpdateVisitor*>(visitor);
		  if (uv)
		  {
			  if(drawable->getVisiable())
			  {
				  double referenceTime = crFrameStamp::getInstance()->getReferenceTime();
				  if(m_state==0)
				  {
					  m_startFrameTime = referenceTime;
					  m_state++;
				  }
				  else if(referenceTime - m_startFrameTime > m_interval)
				  {
					  drawable->setVisiable(false);
					  m_state = 0;
				  }
			  }
		  }
	  }
protected:
	int m_state;
	double m_startFrameTime;
	float m_interval;
};

void CREncapsulation::loadFilterCfg(const std::string &path)
{
//#define DisplayLogo 1
#ifdef DisplayLogo
	CREncapsulation::createLogoText();
#endif
	std::string filename;
	if(path.empty())
		filename = "script/filter.cfg";
	else
		filename = path + "/script/filter.cfg";
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
	{
		CRCore::notify(CRCore::FATAL)<<"loadFilterCfg(): file open error,"<<filename<<std::endl;
		return;
	}

	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());

	std::string str;
	std::vector<float> v_i;
	float flt1;
	int int1;
	ref_ptr<CRText::crText> text;
	ref_ptr<crDrawPixels> drawPixels;
	int nText = 1;
	int nDrawPixels = 1;
	crTime_VisableDCallback *time_vDcallback;

	int screenWidth = crDisplaySettings::instance()->getViewWidth();
	int screenHeight = crDisplaySettings::instance()->getViewHeight();
	while(cfg_script.Push("Text",nText++))
	{
		text = new CRText::crText;
		cfg_script.Get("Name", str);
		text->setName(str);
		if(cfg_script.Get("Type", str))
		{
			if(str.compare("2D")==0)
				crFilterRenderManager::getInstance()->addDrawable(text.get());
		}
		if(cfg_script.Get("Font", str))
		{
			text->setFont(str);
		}
		if(cfg_script.Get("Color", v_i))
		{
			crVector4 color(v_i[0],v_i[1],v_i[2],v_i[3]);
			color /= 255.0f; 
			text->setColor(color);
		}
		if(cfg_script.Get("FontResolution", v_i))
		{
			text->setFontResolution((unsigned int)v_i[0],(unsigned int)v_i[1]);
		}
		if(cfg_script.Get("CharacterSize", int1))
		{
			text->setCharacterSize(int1);
		}
		if(cfg_script.Get("Alignment", str))
		{
			if(str.compare("LEFT_TOP")==0)
				text->setAlignment(CRText::crText::LEFT_TOP);
			else if(str.compare("LEFT_CENTER")==0)
				text->setAlignment(CRText::crText::LEFT_CENTER);
			else if(str.compare("LEFT_BOTTOM")==0)
				text->setAlignment(CRText::crText::LEFT_BOTTOM);
			else if(str.compare("CENTER_TOP")==0)
				text->setAlignment(CRText::crText::CENTER_TOP);
			else if(str.compare("CENTER_CENTER")==0)
				text->setAlignment(CRText::crText::CENTER_CENTER);
			else if(str.compare("CENTER_BOTTOM")==0)
				text->setAlignment(CRText::crText::CENTER_BOTTOM);
			else if(str.compare("RIGHT_TOP")==0)
				text->setAlignment(CRText::crText::RIGHT_TOP);
			else if(str.compare("RIGHT_CENTER")==0)
				text->setAlignment(CRText::crText::RIGHT_CENTER);
			else if(str.compare("RIGHT_BOTTOM")==0)
				text->setAlignment(CRText::crText::RIGHT_BOTTOM);
			else if(str.compare("LEFT_BASE_LINE")==0)
				text->setAlignment(CRText::crText::LEFT_BASE_LINE);
			else if(str.compare("CENTER_BASE_LINE")==0)
				text->setAlignment(CRText::crText::CENTER_BASE_LINE);
			else if(str.compare("RIGHT_BASE_LINE")==0)
				text->setAlignment(CRText::crText::RIGHT_BASE_LINE);
			else if(str.compare("BASE_LINE")==0)
				text->setAlignment(CRText::crText::BASE_LINE);
		}
		if(cfg_script.Get("BackdropType", str))
		{
			if(str.compare("BOTTOM_RIGHT")==0)
				text->setBackdropType(CRText::crText::DROP_SHADOW_BOTTOM_RIGHT);
			else if(str.compare("CENTER_RIGHT")==0)
				text->setBackdropType(CRText::crText::DROP_SHADOW_CENTER_RIGHT);
			else if(str.compare("TOP_RIGHT")==0)
				text->setBackdropType(CRText::crText::DROP_SHADOW_TOP_RIGHT);
			else if(str.compare("BOTTOM_CENTER")==0)
				text->setBackdropType(CRText::crText::DROP_SHADOW_BOTTOM_CENTER);
			else if(str.compare("TOP_CENTER")==0)
				text->setBackdropType(CRText::crText::DROP_SHADOW_TOP_CENTER);
			else if(str.compare("BOTTOM_LEFT")==0)
				text->setBackdropType(CRText::crText::DROP_SHADOW_BOTTOM_LEFT);
			else if(str.compare("CENTER_LEFT")==0)
				text->setBackdropType(CRText::crText::DROP_SHADOW_CENTER_LEFT);
			else if(str.compare("TOP_LEFT")==0)
				text->setBackdropType(CRText::crText::DROP_SHADOW_TOP_LEFT);
			else if(str.compare("OUTLINE")==0)
				text->setBackdropType(CRText::crText::OUTLINE);
			else if(str.compare("NONE")==0)
				text->setBackdropType(CRText::crText::NONE);
		}
		if(cfg_script.Get("BackdropOffset", v_i))
		{
			text->setBackdropOffset(v_i[0],v_i[1]);
		}
		if(cfg_script.Get("BackdropColor", v_i))
		{
			crVector4 color(v_i[0],v_i[1],v_i[2],v_i[3]);
			color /= 255.0f; 
			text->setBackdropColor(color);
		}
		if(cfg_script.Get("DrawMode", str))
		{
			int1 = 0;
			if(str.find("TEXT")!=std::string::npos)
				int1 |= CRText::crText::TEXT;
			if(str.find("BOUNDINGBOX")!=std::string::npos)
				int1 |= CRText::crText::BOUNDINGBOX;
			if(str.find("FILLEDBOUNDINGBOX")!=std::string::npos)
				int1 |= CRText::crText::FILLEDBOUNDINGBOX;
			if(str.find("UNDERLINE")!=std::string::npos)
				int1 |= CRText::crText::UNDERLINE;
			text->setDrawMode(int1);
		}
		if(cfg_script.Get("LineSpacing", flt1))
			text->setLineSpacing(flt1);
		if(cfg_script.Get("UnderLineSpacing", flt1))
			text->setUnderLineSpacing(flt1);
		crVector3 pos(screenWidth * 0.5f, screenHeight * 0.5f,0.0f);
		if(cfg_script.Get("Position", v_i))
		{
			pos[0] = v_i[0] == 0.010f ? pos[0] : (v_i[0] < 0 ? screenWidth + v_i[0] : v_i[0]);
			pos[1] = v_i[1] == 0.010f ? pos[1] : (v_i[1] < 0 ? screenHeight + v_i[1] : v_i[1]);
			pos[2] = v_i[2];
		}
		//if(cfg_script.Get("IdenticPosition", v_i))
		//{
		//	pos.set(v_i[0],v_i[1],v_i[2]);
		//	pos = pos * MVPW;
		//}
		if(cfg_script.Push("AdjustPositionForImage"))
		{
            int w,h;
            if(cfg_script.Get("ImageSize", v_i))
			{
                w = v_i[0];
				h = v_i[1];
			}
			float scale_w,scale_h;
			if(cfg_script.Get("ScaleWithScreen", str))
			{
				if(str.compare("Scale_Width")==0)
				{
					scale_w = float(screenWidth) / float(w);
                    pos[0] *= scale_w;
				}
				else if(str.compare("Scale_Height")==0)
				{
					scale_h = float(screenHeight) / float(h);
					pos[1] *= scale_h;
				}
				else if(str.compare("Scale_Both")==0)
				{
					scale_w = float(screenWidth) / float(w);
					scale_h = float(screenHeight) / float(h);
					pos[0] *= scale_w;
					pos[1] *= scale_h;
				}
			}
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadFilterCfg(): "<<cfg_script.GetLastError()<<std::endl;
		}
		text->setPosition( pos );

		if(cfg_script.Get("Data", str))
		{
			text->setText(str);
		}
		if(cfg_script.Get("Visiable", int1))
		{
			text->setVisiable(int1);
		}
		if(cfg_script.Get("Interval", flt1) && flt1>0.0f)
		{
			time_vDcallback = new crTime_VisableDCallback;
			time_vDcallback->setInterval(flt1);
			text->setUpdateCallback(time_vDcallback);
		}
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadFilterCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}

	CRCore::crImage* image;
	//crImageStream* imageStream;
	//crPlayStreamCallback *playStream;
	int width,height;
	while(cfg_script.Push("DrawPixels",nDrawPixels++))
	{
		drawPixels = new crDrawPixels;
		cfg_script.Get("Name", str);
		drawPixels->setName(str);
		if(cfg_script.Get("Type", str))
		{
			if(str.compare("2D")==0)
				crFilterRenderManager::getInstance()->addDrawable(drawPixels.get());
			//else if(str.compare("Mouse")==0)
			//	crFilterRenderManager::getInstance()->addMouse(drawPixels.get());
			//else if(str.compare("MousePicMsg")==0)
			//	crFilterRenderManager::getInstance()->addMousePicMsg(drawPixels.get());
		}
		if(cfg_script.Get("Visiable", int1))
		{
			drawPixels->setVisiable(int1);
		}
		if(cfg_script.Get("Image", str))
		{
			image = CRIOManager::readImageFile(str);
			drawPixels->setImage(image);
	        if(cfg_script.Get("ScaleWithScreen", str))
			{
				if(str.compare("Scale_Width")==0)
				{
					drawPixels->setScaleWithScreen(crDrawPixels::Scale_Width);
				}
				else if(str.compare("Scale_Height")==0)
				{
					drawPixels->setScaleWithScreen(crDrawPixels::Scale_Height);
				}
				else if(str.compare("Scale_Both")==0)
				{
					drawPixels->setScaleWithScreen(crDrawPixels::Scale_Both);
				}
			}
			width = image->s();
			height = image->t();
		}
		//if(cfg_script.Get("Stream", str))
		//{
		//	imageStream = crImageStreamManager::getInstance()->getImageStream(str);
		//	if(!imageStream) 
		//	{
		//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadFilterCfg(): not find ImageStream ID = "<<str.c_str()<<std::endl;
		//		if (!cfg_script.Pop())
		//			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadFilterCfg(): "<<cfg_script.GetLastError()<<std::endl;
		//		continue;
		//	}
		//	playStream = new crPlayStreamCallback;
		//	playStream->setStream(imageStream);
		//	drawPixels->setUpdateCallback(playStream);
		//	imageStream->getStreamSize(width,height);
		//	drawPixels->setImage(imageStream->getStreamImage(0.0f));
		//}

		crVector3 pos((screenWidth - width) * 0.5, (screenHeight - height) * 0.5,0.0f);
		if(cfg_script.Get("Position", v_i))
		{
			pos[0] = v_i[0] == 0.010f ? pos[0] : (v_i[0] < 0 ? screenWidth + v_i[0] : v_i[0]);
			pos[1] = v_i[1] == 0.010f ? pos[1] : (v_i[1] < 0 ? screenHeight + v_i[1] : v_i[1]);
			pos[2] = v_i[2];
		}
		if(cfg_script.Push("AdjustPositionForImage"))
		{
			int w,h;
			if(cfg_script.Get("ImageSize", v_i))
			{
				w = v_i[0];
				h = v_i[1];
			}
			float scale_w,scale_h;
			if(cfg_script.Get("ScaleWithScreen", str))
			{
				if(str.compare("Scale_Width")==0)
				{
					scale_w = float(screenWidth) / float(w);
					pos[0] *= scale_w;
				}
				else if(str.compare("Scale_Height")==0)
				{
					scale_h = float(screenHeight) / float(h);
					pos[1] *= scale_h;
				}
				else if(str.compare("Scale_Both")==0)
				{
					scale_w = float(screenWidth) / float(w);
					scale_h = float(screenHeight) / float(h);
					pos[0] *= scale_w;
					pos[1] *= scale_h;
				}
			}
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadFilterCfg(): "<<cfg_script.GetLastError()<<std::endl;
		}
		drawPixels->setPosition( pos );

		if(cfg_script.Get("Interval", flt1) && flt1>0.0f)
		{
			time_vDcallback = new crTime_VisableDCallback;
			time_vDcallback->setInterval(flt1);
			drawPixels->setUpdateCallback(time_vDcallback);
		}
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadFilterCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}
	//int nFilterNode = 1;
	//crSearchNodeBYNameVisitor searchByNameVisitor;
	//while(cfg_script.Push("FilterNode",nFilterNode++))
	//{
 //       cfg_script.Get("NodeName", str);
	//	searchByNameVisitor.reset();
	//	searchByNameVisitor.setNameId(str);
	//	searchByNameVisitor.setSearchNodeType(crSearchNodeBYNameVisitor::ALLNODE);
	//	crBrain::getInstance()->accept(searchByNameVisitor);
	//	crNode *node = searchByNameVisitor.getResult();
	//	if(node)
	//	{
	//		crFilterRenderManager::getInstance()->addFilterNode(node);
	//		if(cfg_script.Get("Visiable", int1))
	//		{
	//			node->setVisiable(int1);
	//		}
	//	}

	//	if (!cfg_script.Pop())
	//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadFilterCfg(): "<<cfg_script.GetLastError()<<std::endl;
	//}
	cfg_script.Pop();
}
void CREncapsulation::loadUIFile(const std::string &uifile)
{
	if(!uifile.empty())
	{
		//CRIOManager::readNodeFile(uifile);
		ref_ptr<crGroup> uiroot = dynamic_cast<crGroup *>(CRIOManager::readNodeFile(uifile));
		addUINodeToScene(uiroot.get());
	}
}
void CREncapsulation::addUINodeToScene(CRCore::crGroup *uiroot,bool async)
{
	if(uiroot)
	{
		CRUtil::crOptimizer::crTextureVisitor tv(true,true, // unref image 
			false,false, // client storage
			true,1.0, // anisotropic filtering
			NULL );
		tv.setChangeFilter(crTexture::LINEAR,crTexture::LINEAR);
		//tv.setChangeWrap(crTexture::CLAMP_TO_EDGE);
		uiroot->accept(tv);
		CRUtil::crRemoveTransStateVisitor stateVisitor;
		uiroot->accept(stateVisitor);

		bool isEditor = crDisplaySettings::instance()->getRunMode() == 0;
		crCanvasNode *canvas;
		CRCore::crCollectNodeBYClassNameVisitor searchVisitor;
		searchVisitor.insertClassNameId("crCanvasNode");
		uiroot->accept(searchVisitor);
		crCanvasNode *mainUI = crFilterRenderManager::getInstance()->getMainCanvas();
		crGroup *canvasRoot = crFilterRenderManager::getInstance()->getCanvasRoot();
		NodeArray &canvasArray = searchVisitor.getResult();
		for( NodeArray::iterator itr = canvasArray.begin();
			itr != canvasArray.end();
			++itr )
		{
			canvas = dynamic_cast<crCanvasNode *>(itr->get());
			if(canvas->getName().compare("MainUI")==0)
			{
				NodeArray &childArray = canvas->getChildArray();
				for( NodeArray::iterator nitr = childArray.begin();
					 nitr != childArray.end();
					 ++nitr )
				{
					if(!isEditor) (*nitr)->setVisiable(false);
					if(async)CRIOManager::crLoadManager::getInstance()->requestAddNode(mainUI,nitr->get(),false);
					else mainUI->addChild(nitr->get());
				}
			}
			else if(canvas->getName().compare("MouseUI")==0)
			{
				canvas->setEnableIntersect(false);
				canvas->setCanFocus(false);
				crFilterRenderManager::getInstance()->setMouseCanvas(canvas);
			}
			else if(canvas->getName().compare("LoadingUI")==0)
			{
				canvas->setEnableIntersect(false);
				canvas->setCanFocus(false);
				crFilterRenderManager::getInstance()->setLoadingCanvas(canvas);
			}
			else
			{
				if(async)CRIOManager::crLoadManager::getInstance()->requestAddNode(canvasRoot,canvas,false);
				else canvasRoot->addChild(canvas);
				if (isEditor)
				{
					crFilterRenderManager::getInstance()->showCanvas(canvas,canvas->getName(),SH_Show);
				}
			}
		}
	}
}
void CREncapsulation::loadEngineUI()
{
	if(CRCore::crDisplaySettings::instance()->getRunMode() != 0)
	{
		//return;
 		ref_ptr<crGroup> uiroot = dynamic_cast<crGroup *>(CRIOManager::readNodeFile("uidata/Engine/lenui.cre"));
		if(uiroot.valid())
		{
			addUINodeToScene(uiroot.get());
			//ref_ptr<crCanvasNode>lenCanvas = crFilterRenderManager::getInstance()->findCanvas("LenCanvas");
			//if(lenCanvas.valid())
			//{
			//	crVector4 color(0.0f,0.0f,0.0f,1.0f);
			//	lenCanvas->doEvent(WCH_LenFadeInit,MAKEINT64(&color,NULL));
			//}
			//CRCore::crSearchNodeBYClassNameVisitor searchVisitor;
			//searchVisitor.insertClassNameId("crCanvasNode");
			//uiroot->accept(searchVisitor);
			//crCanvasNode *canvas = dynamic_cast<crCanvasNode *>(searchVisitor.getResult());
			//if(canvas)
			//{
			//	crFilterRenderManager::getInstance()->showCanvas(canvas,true);
			//	canvas->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_LenFadeInit,MAKERGBA(0,0,0,255)));
			//	short speed = 200;
			//	short timeDelay = 1000;
			//	canvas->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_LenFade,MAKEINT32(speed,timeDelay)));
			//}
		}
	}
	else
	{
		ref_ptr<crGroup> uiroot = dynamic_cast<crGroup *>(CRIOManager::readNodeFile("uidata/Engine/rttui.cre"));
		if(uiroot.valid())
		{
			addUINodeToScene(uiroot.get());
			//CRCore::crSearchNodeBYClassNameVisitor searchVisitor;
			//searchVisitor.insertClassNameId("crCanvasNode");
			//uiroot->accept(searchVisitor);
			//crCanvasNode *canvas = dynamic_cast<crCanvasNode *>(searchVisitor.getResult());
			//if(canvas)
			//{
			//	CRIOManager::crLoadManager::getInstance()->requestAddNode(crFilterRenderManager::getInstance()->getCanvasRoot(),canvas,true);
			//	crFilterRenderManager::getInstance()->showCanvas(canvas,true);
			//}
		}
	}
}
void CREncapsulation::loadLoadingUI()
{
	ref_ptr<crGroup> uiroot = dynamic_cast<crGroup *>(CRIOManager::readNodeFile("uidata/Engine/UI_Loading.cre"));
	if(uiroot.valid())
	{
		addUINodeToScene(uiroot.get());
	}
}
//void CREncapsulation::loadSceneNodeEventCfg()
//{
//	rcfg::ConfigScript cfg_script;

//	CRIOManager::crLoadCookFile scopedLoad("script/SceneNodeEvent.cfg");

//	struct stat buf;
//	if (stat( "script/SceneNodeEvent.cfg", &buf )) 
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSceneNodeEventCfg(): file open error"<<std::endl;
//		return;
//	}
//
//	if (!cfg_script.Open("script/SceneNodeEvent.cfg"))
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSceneNodeEventCfg(): "<<cfg_script.GetLastError()<<std::endl;
//		return;
//	}
//
//	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());
//
//	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
//	CRCore::ref_ptr<crNode> node;
//	unsigned int typeParam1,typeParam2;
//	ref_ptr<crEventCallback> event;
//	int nNode = 1;
//	int nObjectEvent = 1;
//	int nEvent = 1;
//	int nEventParam = 1;
//	std::string str;
//	while (cfg_script.Push("PhysicsObject", nNode++)) 
//	{
//		cfg_script.Get("NodeName", str);
//		searchByNameVisitor.reset();
//		searchByNameVisitor.setNameId(str);
//		searchByNameVisitor.setSearchNodeType(crSearchNodeBYNameVisitor::ALLNODE);
//		CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
//		node = searchByNameVisitor.getResult();
//		if(!node)
//		{
//			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSceneNodeEventCfg(): NodeName error "<<str<<std::endl;
//			if (!cfg_script.Pop())
//				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSceneNodeEventCfg(): "<<cfg_script.GetLastError()<<std::endl;
//			continue;
//		}
//
//		nObjectEvent = 1;
//		while (cfg_script.Push("PhysicsObjectEvent", nObjectEvent++)) 
//		{
//			cfg_script.Get("TypeParam", str,1);
//			typeParam1 = parseEventMsg(str);
//			cfg_script.Get("TypeParam", str,2);
//			typeParam2 = parseEventMsg(str);
//
//			nEvent = 1;
//			while (cfg_script.Push("Event", nEvent++))
//			{
//				cfg_script.Get("EventFunction", str);
//				event = crEventCallbackManager::getInstance()->getEventCallback(str);
//				if(!event) 
//				{
//					CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadSceneNodeEventCfg(): 未找到EventFunction: "<<str<<std::endl;
//					continue;
//				}
//				nEventParam = 1;
//				while (cfg_script.Get("EventParam", str,nEventParam++)) 
//				{
//					event->addEventParam(nEventParam-2,str);
//				}
//
//				if (!cfg_script.Pop())
//					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSceneNodeEventCfg(): "<<cfg_script.GetLastError()<<std::endl;
//				node->insertEvent(MAKEINT64(typeParam1,typeParam2),event.get());
//			}
//			if (!cfg_script.Pop())
//				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSceneNodeEventCfg(): "<<cfg_script.GetLastError()<<std::endl;
//		}
//
//		///load data handle
//		ref_ptr<crData> data;
//		int nParam;
//		if(cfg_script.Push("Data"))
//		{
//			cfg_script.Get("Class", str);
//			data = crDataManager::getInstance()->getData(str);
//			if(!data.valid())
//			{
//				CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadSceneNodeEventCfg(): 未找到EventFunction: "<<str<<std::endl;
//			}
//			else
//			{
//				nParam = 1;
//				while (cfg_script.Get("Param", str,nParam++)) 
//				{
//					data->addParam(nParam-2,str);
//				}
//
//				loadDrivers(cfg_script,data.get());
//				node->setDataClass(data.get());
//			}
//			if (!cfg_script.Pop())
//				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSceneNodeEventCfg(): "<<cfg_script.GetLastError()<<std::endl;
//		}
//		if (!cfg_script.Pop())
//			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSceneNodeEventCfg(): "<<cfg_script.GetLastError()<<std::endl;
//	}
//    cfg_script.Pop();
//	//CRCore::ref_ptr<crObject::crObjectEvent> objectEvent;
//	//CRCore::ref_ptr<CRCore::crEventCallback> eventCallback;
//	//int i = 1;
//	//int j,k;
//	//std::string str;
//	//crObject::crObjectEvent::EventType et;
//	//while (cfg_script.Push("ObjectEvent", i++)) 
//	//{
//	//	cfg_script.Get("ObjectName", str);
//	//	searchByNameVisitor.reset();
//	//	searchByNameVisitor.setNameId(str);
//	//	searchByNameVisitor.setSearchNodeType(crSearchNodeBYNameVisitor::OBJECT);
//	//	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
//	//	object = dynamic_cast<crObject *>(searchByNameVisitor.getResult());
//	//	//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadObjectEventCfg(): ObjectName = "<<object->getName()<<" objectID = "<<object.get()<<std::endl;
//	//	if(!object)
//	//	{
//	//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadObjectEventCfg(): ObjectName error "<<str<<std::endl;
//	//		if (!cfg_script.Pop())
//	//			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadObjectEventCfg(): "<<cfg_script.GetLastError()<<std::endl;
//	//		continue;
//	//	}
//
//	//	cfg_script.Get("EventType", str);
//	//	if(str.compare("LBtnUp") == 0)
//	//		et = crObject::crObjectEvent::LBtnUp;
//	//	else if(str.compare("LBtnDown") == 0)
//	//		et = crObject::crObjectEvent::LBtnDown;
//	//	else if(str.compare("LBtnDblClk") == 0)
//	//		et = crObject::crObjectEvent::LBtnDblClk;
//	//	else if(str.compare("RBtnUp") == 0)
//	//		et = crObject::crObjectEvent::RBtnUp;
//	//	else if(str.compare("RBtnDown") == 0)
//	//		et = crObject::crObjectEvent::RBtnDown;
//	//	else if(str.compare("RBtnDblClk") == 0)
//	//		et = crObject::crObjectEvent::RBtnDblClk;
//	//	else if(str.compare("MBtnUp") == 0)
//	//		et = crObject::crObjectEvent::MBtnUp;
//	//	else if(str.compare("MBtnDown") == 0)
//	//		et = crObject::crObjectEvent::MBtnDown;
//	//	else if(str.compare("MBtnDblClk") == 0)
//	//		et = crObject::crObjectEvent::MBtnDblClk;
//	//	else if(str.compare("MouseMove") == 0)
//	//		et = crObject::crObjectEvent::MouseMove;
//	//	else if(str.compare("MouseWheel") == 0)
//	//		et = crObject::crObjectEvent::MouseWheel;
//	//	else
//	//		et = crObject::crObjectEvent::MouseMove;
//	//	objectEvent = new crObject::crObjectEvent(et);
//
//	//	j = 1;
//	//	while (cfg_script.Push("Event", j++))
//	//	{
//	//		cfg_script.Get("EventFunction", str);
//	//		eventCallback = crEventCallbackManager::getInstance()->getEventCallback(str);
//	//		if(!eventCallback) 
//	//		{
//	//			CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadObjectEventCfg(): 未找到EventFunction: "<<str<<std::endl;
//	//			continue;
//	//		}
//	//		k = 1;
//	//		while (cfg_script.Get("EventParam", str,k++)) 
//	//		{
//	//			eventCallback->addEventParam(k-2,str);
//	//		}
//
//	//	    objectEvent->addObjectEventCallback(eventCallback.get());
//
//	//		if (!cfg_script.Pop())
//	//			CRCore::notify(CRCore::FATAL)<<"crImageStage::loadElementEvents(): "<<cfg_script.GetLastError()<<std::endl;
//	//	}
//
//	//	object->addObjectEvent(objectEvent.get());
//
//	//	if (!cfg_script.Pop())
//	//		CRCore::notify(CRCore::FATAL)<<"crImageStage::loadElementEvents(): "<<cfg_script.GetLastError()<<std::endl;
//	//}
//}
//
//void CREncapsulation::loadNodeEventCfg(CRCore::crNode *node)
//{
//	if(!node) return;
//	rcfg::ConfigScript cfg_script;

//	CRIOManager::crLoadCookFile scopedLoad("script/NodeEvent.cfg");

//	struct stat buf;
//	if (stat( "script/NodeEvent.cfg", &buf )) 
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNodeEventCfg(): file open error"<<std::endl;
//		return;
//	}
//
//	if (!cfg_script.Open("script/NodeEvent.cfg"))
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNodeEventCfg(): "<<cfg_script.GetLastError()<<std::endl;
//		return;
//	}
//	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());
//
//	if(cfg_script.Push(node->getName()))
//	{
//		//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNodeEventCfg(): "<<node->getName()<<std::endl;
//		unsigned int typeParam1,typeParam2;
//		ref_ptr<crEventCallback> event;
//		int nObjectEvent = 1;
//		int nEvent = 1;
//		int nEventParam = 1;
//		std::string str;
//
//		nObjectEvent = 1;
//		while (cfg_script.Push("PhysicsObjectEvent", nObjectEvent++)) 
//		{
//			cfg_script.Get("TypeParam", str,1);
//			typeParam1 = parseEventMsg(str);
//			cfg_script.Get("TypeParam", str,2);
//			typeParam2 = parseEventMsg(str);
//
//			nEvent = 1;
//			while (cfg_script.Push("Event", nEvent++))
//			{
//				cfg_script.Get("EventFunction", str);
//				event = crEventCallbackManager::getInstance()->getEventCallback(str);
//				if(!event) 
//				{
//					CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadNodeEventCfg(): 未找到EventFunction: "<<str<<std::endl;
//					if (!cfg_script.Pop())
//						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNodeEventCfg(): "<<cfg_script.GetLastError()<<std::endl;
//					continue;
//				}
//				nEventParam = 1;
//				while (cfg_script.Get("EventParam", str,nEventParam++)) 
//				{
//					event->addEventParam(nEventParam-2,str);
//				}
//
//				if (!cfg_script.Pop())
//					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNodeEventCfg(): "<<cfg_script.GetLastError()<<std::endl;
//				node->insertEvent(MAKEINT64(typeParam1,typeParam2),event.get());
//				//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNodeEventCfg(): insertEvent "<<node->getName()<<" classname = "<<node->className()<<std::endl;
//			}
//			if (!cfg_script.Pop())
//				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNodeEventCfg(): "<<cfg_script.GetLastError()<<std::endl;
//		}
//
//        ///load data handle
//		ref_ptr<crData> data;
//		int nParam;
//		if(cfg_script.Push("Data"))
//		{
//			cfg_script.Get("Class", str);
//			data = crDataManager::getInstance()->getData(str);
//			if(!data.valid())
//			{
//				CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadNodeEventCfg(): 未找到EventFunction: "<<str<<std::endl;
//			}
//			else
//			{
//				nParam = 1;
//				while (cfg_script.Get("Param", str,nParam++)) 
//				{
//					data->addParam(nParam-2,str);
//				}
//
//				loadDrivers(cfg_script,data.get());
//				node->setDataClass(data.get());
//			}
//			if (!cfg_script.Pop())
//				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNodeEventCfg(): "<<cfg_script.GetLastError()<<std::endl;
//		}
//
//		cfg_script.Pop();
//	}
//	cfg_script.Pop();
//}

void CREncapsulation::loadEventScript(const std::string &scriptFile, CRCore::crNode *node)
{
	if(!node) return;
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,scriptFile))
	{
		CRCore::notify(CRCore::FATAL)<<"loadEventScript(): file open error,"<<scriptFile<<std::endl;
		return;
	}

	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());

	//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEventScript(): "<<node->getName()<<std::endl;
	_crInt32 typeParam1,typeParam2;
	ref_ptr<crEventCallback> event;
	int nObjectEvent = 1;
	int nEvent = 1;
	int nEventParam = 1;
	std::string str;

	nObjectEvent = 1;
	while (cfg_script.Push("PhysicsObjectEvent", nObjectEvent++)) 
	{
		cfg_script.Get("TypeParam", str,1);
		typeParam1 = parseEventMsg(str);
		cfg_script.Get("TypeParam", str,2);
		typeParam2 = parseEventMsg(str);

		nEvent = 1;
		while (cfg_script.Push("Event", nEvent++))
		{
			cfg_script.Get("EventFunction", str);
			event = crEventCallbackManager::getInstance()->getEventCallback(str);
			if(!event) 
			{
				CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadEventScript(): 未找到EventFunction: "<<str<<std::endl;
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEventScript(): "<<cfg_script.GetLastError()<<std::endl;
				continue;
			}
			nEventParam = 1;
			while (cfg_script.Get("EventParam", str,nEventParam++)) 
			{
				event->addEventParam(nEventParam-2,str);
			}

			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEventScript(): "<<cfg_script.GetLastError()<<std::endl;
			node->insertEvent(MAKEINT64(typeParam1,typeParam2),event.get());
			//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEventScript(): insertEvent "<<node->getName()<<" classname = "<<node->className()<<std::endl;
		}
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEventScript(): "<<cfg_script.GetLastError()<<std::endl;
	}

	///load data handle
	crData *data = loadData(cfg_script);
	node->setDataClass(data);
	//ref_ptr<crData> data;
	//int nParam;
	//if(cfg_script.Push("Data"))
	//{
	//	cfg_script.Get("Class", str);
	//	data = crDataManager::getInstance()->getData(str);
	//	if(!data.valid())
	//	{
	//		CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadEventScript(): 未找到EventFunction: "<<str<<std::endl;
	//	}
	//	else
	//	{
	//		nParam = 1;
	//		while (cfg_script.Get("Param", str,nParam++)) 
	//		{
	//			data->addParam(nParam-2,str);
	//		}

	//		loadDrivers(cfg_script,data.get());
	//		node->setDataClass(data.get());
	//	}
	//	if (!cfg_script.Pop())
	//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEventScript(): "<<cfg_script.GetLastError()<<std::endl;
	//}
	cfg_script.Pop();
}
crData *CREncapsulation::loadData(const std::string &scriptFile)
{
	//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadData(): "<<scriptFile<<std::endl;
	
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,scriptFile))
	{
		CRCore::notify(CRCore::FATAL)<<"loadData(): file open error,"<<scriptFile<<std::endl;
		return NULL;
	}

	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());

	crData *data = loadData(cfg_script);

	cfg_script.Pop();

	return data;
}
crData *CREncapsulation::loadData(rcfg::ConfigScript& cfg_script)
{
	crData* data = NULL;
	std::string str;
	int nParam;
	if(cfg_script.Push("Data"))
	{
		cfg_script.Get("Class", str);
		data = crDataManager::getInstance()->getData(str);
		if(!data)
		{
			CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadData(): 未找到data: "<<str<<std::endl;
		}
		else
		{
			int nInclude = 1;
			while(cfg_script.Get("IncludeData", str,nInclude++))
			{
				loadIncludeData(str,data);
			}
			nParam = 1;
			while (cfg_script.Get("Param", str,nParam++)) 
			{
				data->addParam(nParam-2,str);
			}
			loadDrivers(cfg_script,data);
		}
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadData(): "<<cfg_script.GetLastError()<<std::endl;
	}
	return data;
}
void CREncapsulation::loadIncludeData(const std::string &scriptFile,crData *data)
{
	if(!data) return;
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,scriptFile))
	{
		char gbuf[256];
		sprintf(gbuf,"loadIncludeData(): file open error,%s\n\0",scriptFile.c_str());
		gDebugInfo->debugInfo(CRCore::WARN,gbuf);
		//CRCore::notify(CRCore::FATAL)<<"loadIncludeData(): file open error,"<<scriptFile<<std::endl;
		return;
	}

	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());

	std::string str;
	int nParam;
	if(cfg_script.Push("Data"))
	{
		//cfg_script.Get("Class", str);
		//
		//if(str.compare(data->_name()) != 0)
		//{
		//	CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadIncludeData(): Data类型不匹配:"<<str<<std::endl;
		//}
		//else
		//{
		int nInclude = 1;
		while(cfg_script.Get("IncludeData", str,nInclude++))
		{
			loadIncludeData(str,data);
		}
		nParam = 1;
		while (cfg_script.Get("Param", str,nParam++)) 
		{
			data->addParam(nParam-2,str);
		}
		loadDrivers(cfg_script,data);
		//}
		//if (!cfg_script.Pop())
		//	CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadData(): "<<cfg_script.GetLastError()<<std::endl;
	}
	cfg_script.Pop();
}
void CREncapsulation::loadExternData(rcfg::ConfigScript& cfg_script,CRCore::crData *data)
{
	if(!data) return;
	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());

	std::string str;
	int nParam;
	if(cfg_script.Push("Data"))
	{
		int nInclude = 1;
		while(cfg_script.Get("IncludeData", str,nInclude++))
		{
			loadIncludeData(str,data);
		}
		nParam = 1;
		while (cfg_script.Get("Param", str,nParam++)) 
		{
			data->addParam(nParam-2,str);
		}
		loadDrivers(cfg_script,data);

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadData(): "<<cfg_script.GetLastError()<<std::endl;
	}
	cfg_script.Pop();
}
void CREncapsulation::loadHandles(rcfg::ConfigScript& cfg_script,crHandle *handle)
{
	if(!handle)
		return;
	ref_ptr<CRCore::crHandle> logic,method;
	int nLogic,nMethod,/*nMethod2,*/nParam;
	std::string str;

	nMethod = 1;
	while (cfg_script.Push("Method", nMethod++))
	{
		cfg_script.Get("Class", str);
		method = crHandleManager::getInstance()->getHandle(str);
		if(!method) 
		{
			//CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadHandles: 未找到Handle: "<<str<<std::endl;
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadHandles: "<<cfg_script.GetLastError()<<std::endl;
			continue;
		}
		nParam = 1;
		while (cfg_script.Get("Param", str,nParam++)) 
		{
			method->addParam(nParam-2,str);
		}

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadHandles: "<<cfg_script.GetLastError()<<std::endl;

		handle->inputHandle(nMethod-2,method.get());
	}

	nLogic = 1;
	while (cfg_script.Push("Logic", nLogic++))
	{
		cfg_script.Get("Class", str);
		logic = crHandleManager::getInstance()->getHandle(str);
		if(!logic) 
		{
			//CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadHandles: 未找到Handle: "<<str<<std::endl;
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadHandles: "<<cfg_script.GetLastError()<<std::endl;
			continue;
		}
		nParam = 1;
		while (cfg_script.Get("Param", str,nParam++)) 
		{
			logic->addParam(nParam-2,str);
		}

		//nMethod2 = 1;
		//while (cfg_script.Push("Method", nMethod2++))
		//{
		//	cfg_script.Get("Class", str);
		//	method = crHandleManager::getInstance()->getHandle(str);
		//	if(!method) 
		//	{
		//		CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadHandles: 未找到Handle: "<<str<<std::endl;
		//		if (!cfg_script.Pop())
		//			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadHandles: "<<cfg_script.GetLastError()<<std::endl;
		//		continue;
		//	}
		//	nParam = 1;
		//	while (cfg_script.Get("Param", str,nParam++)) 
		//	{
		//		method->addParam(nParam-2,str);
		//	}

		//	if (!cfg_script.Pop())
		//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadHandles: "<<cfg_script.GetLastError()<<std::endl;

		//	logic->inputHandle(nMethod2-2,method.get());
		//}

		loadHandles(cfg_script,logic.get());

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadHandles: "<<cfg_script.GetLastError()<<std::endl;

		handle->inputHandle(nLogic+nMethod-4,logic.get());
	}
}
void CREncapsulation::loadDrivers(rcfg::ConfigScript& cfg_script,crData *data)
{
	if(!data)
		return;
	ref_ptr<CRCore::crHandle> driver;
	int nHandle = 1;
	int nParam;
	std::string str;
	_crInt32 msgParam32[2] = {0,0};
	_crInt16 msgParam16[4] = {0,0,0,0};
	_crInt64 msg64 = 0;
	while (cfg_script.Push("Handle", nHandle++)) 
	{
		memset(msgParam32,0,8);
		memset(msgParam16,0,8);
		if(cfg_script.Get("MsgParam", str,1))
		{
			msgParam32[0] = parseEventMsg(str);
		}
		if(cfg_script.Get("MsgParam", str,2))
		{
		    msgParam32[1] = parseEventMsg(str);
		}
        for(int i = 1; i<5; i++)
		{
			if(cfg_script.Get("MsgParam16", str,i))
                msgParam16[i-1] = parseEventMsg16(str);
		}
        if(msgParam32[0]==0)
		{
			msgParam32[0] = MAKEINT32(msgParam16[0],msgParam16[1]);
		}
		if(msgParam32[1]==0)
		{
            msgParam32[1] = MAKEINT32(msgParam16[2],msgParam16[3]);
		}
		msg64 = MAKEINT64(msgParam32[0],msgParam32[1]);
		//if(data->getHandle(msg64))
		//{
		//	CRCore::notify(CRCore::INFO)<<"CREncapsulation::loadData() error 该消息ID已经存在 "<<cfg_script.GetLastError()<<std::endl;
		//}
		cfg_script.Push("Driver");
		cfg_script.Get("Class", str);
		driver = crHandleManager::getInstance()->getHandle(str);
		if(driver.valid())
		{
			nParam = 1;
			while (cfg_script.Get("Param", str,nParam++)) 
			{
				driver->addParam(nParam-2,str);
			}
			loadHandles(cfg_script,driver.get());
		}
	//	nLogic = 1;
	//	while (cfg_script.Push("Logic", nLogic++))
	//	{
	//		cfg_script.Get("Class", str);
	//		logic = crHandleManager::getInstance()->getHandle(str);
	//		if(!logic) 
	//		{
	//			CRCore::notify(CRCore::NOTICE)<<"crImageStage::loadElementHandles(): 未找到Handle: "<<str<<std::endl;
	//			if (!cfg_script.Pop())
	//				CRCore::notify(CRCore::FATAL)<<"crImageStage::loadElementHandles(): "<<cfg_script.GetLastError()<<std::endl;
	//			continue;
	//		}
	//		nParam = 1;
	//		while (cfg_script.Get("Param", str,nParam++)) 
	//		{
	//			logic->addParam(nParam-2,str);
	//		}

	//		nMethod = 1;
	//		while (cfg_script.Push("Method", nMethod++))
	//		{
	//			cfg_script.Get("Class", str);
	//			method = crHandleManager::getInstance()->getHandle(str);
	//			if(!method) 
	//			{
	//				CRCore::notify(CRCore::NOTICE)<<"crImageStage::loadElementHandles(): 未找到Handle: "<<str<<std::endl;
	//				if (!cfg_script.Pop())
	//					CRCore::notify(CRCore::FATAL)<<"crImageStage::loadElementHandles(): "<<cfg_script.GetLastError()<<std::endl;
	//				continue;
	//			}
	//			nParam = 1;
	//			while (cfg_script.Get("Param", str,nParam++)) 
	//			{
	//				method->addParam(nParam-2,str);
	//			}

	//			if (!cfg_script.Pop())
	//				CRCore::notify(CRCore::FATAL)<<"crImageStage::loadElementHandles(): "<<cfg_script.GetLastError()<<std::endl;

	//			logic->inputHandle(nMethod-2,method.get());
	//		}

	//		if (!cfg_script.Pop())
	//			CRCore::notify(CRCore::FATAL)<<"crImageStage::loadElementHandles(): "<<cfg_script.GetLastError()<<std::endl;

	//		driver->inputHandle(nLogic-2,logic.get());
	//	}

	//	nMethod = 1;
	//	while (cfg_script.Push("Method", nMethod++))
	//	{
	//		cfg_script.Get("Class", str);
	//		method = crHandleManager::getInstance()->getHandle(str);
	//		if(!method) 
	//		{
	//			CRCore::notify(CRCore::NOTICE)<<"crImageStage::loadElementHandles(): 未找到Handle: "<<str<<std::endl;
	//			if (!cfg_script.Pop())
	//				CRCore::notify(CRCore::FATAL)<<"crImageStage::loadElementHandles(): "<<cfg_script.GetLastError()<<std::endl;
	//			continue;
	//		}
	//		nParam = 1;
	//		while (cfg_script.Get("Param", str,nParam++)) 
	//		{
	//			method->addParam(nParam-2,str);
	//		}

	//		if (!cfg_script.Pop())
	//			CRCore::notify(CRCore::FATAL)<<"crImageStage::loadElementHandles(): "<<cfg_script.GetLastError()<<std::endl;

	//		driver->inputHandle(nLogic+nMethod-4,method.get());
	//	}
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadHandles(): "<<cfg_script.GetLastError()<<std::endl;
		
		data->insertHandle(msg64,driver.get());

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadHandles(): "<<cfg_script.GetLastError()<<std::endl;
	}
}
//void CREncapsulation::loadObjectLightCfg(crObject *object)
//{
//	if(!object) return;
//	rcfg::ConfigScript cfg_script;
//
//	struct stat buf;
//	if (stat( "script/ObjectLight.cfg", &buf )) 
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNodeEventCfg(): file open error"<<std::endl;
//		return;
//	}
//
//	if (!cfg_script.Open("script/ObjectLight.cfg"))
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNodeEventCfg(): "<<cfg_script.GetLastError()<<std::endl;
//		return;
//	}
//	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());
//
//	if(cfg_script.Push(object->getName()))
//	{
//		int nObjLight = 1;
//	    std::string str;
//		while(cfg_script.Get("AvoidLight", str,nObjLight++))
//		{
//			object->addAvoidLight(str);
//		}
//		nObjLight = 1;
//		while(cfg_script.Get("NeedLight", str,nObjLight++))
//		{
//			object->addNeedLight(str);
//		}
//		cfg_script.Pop();
//	}
//	cfg_script.Pop();
//}
//
//
void CREncapsulation::loadEffectSequenceManagerCfg(const std::string &path)
{
	std::string filename;
	if(path.empty())
		filename = "script/EffectSequenceManager.cfg";
	else
		filename = path + "/script/EffectSequenceManager.cfg";
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
	{
		CRCore::notify(CRCore::FATAL)<<"loadEffectSequenceManagerCfg(): file open error,"<<filename<<std::endl;
		return;
	}

	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());

	std::string name,str;
	int nNode = 1;
	int nNodeName = 1;
	if(cfg_script.Push("EffectSequenceManager"))
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		ref_ptr<crSequence> sequence;
		nNode = 1;
		while (cfg_script.Push("EffectSequence",nNode++))
		{
			cfg_script.Get("Name",name);
			nNodeName = 1;
			while(cfg_script.Get("NodeName",str,nNodeName++))
			{
				searchByNameVisitor.reset();
				searchByNameVisitor.setNameId(str);
				searchByNameVisitor.setSearchNodeType(GROUP);
				CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
				sequence = dynamic_cast<crSequence *>(searchByNameVisitor.getResult());
				if(!sequence)
				{
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEffectSequenceManagerCfg(): EffectSequence Name error "<<str<<std::endl;
					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEffectSequenceManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
					continue;
				}
				crEffectSequenceManager::getInstance()->addEffectNodes(name,sequence.get());
				CRIOManager::crLoadManager::getInstance()->requestAddNode(crEffectSequenceManager::getInstance()->getEffectRoot(),sequence.get(),true);
			}
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEffectSequenceManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
		}
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEffectSequenceManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}

	cfg_script.Pop();
}

void CREncapsulation::loadWeaponManagerCfg(const std::string &path)
{
	std::string filename;
	if(path.empty())
		filename = "script/WeaponManager.cfg";
	else
		filename = path + "/script/WeaponManager.cfg";
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
	{
		CRCore::notify(CRCore::FATAL)<<"loadWeaponManagerCfg(): file open error,"<<filename<<std::endl;
		return;
	}

	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());

	std::string str;
	std::vector<float> v_i;
	float flt1;
	int int1;
	unsigned int attrmask = 0;
	ref_ptr<CRPhysics::crWeaponObject> weaponObject;
	ref_ptr<CRPhysics::crBulletObject> bulletObject;
	ref_ptr<CRPhysics::crCaissonObject> caissonObject;
	ref_ptr<CRPhysics::crHpObject> hpObject;
	ref_ptr<CRPhysics::crArmorObject> armorObject;
	
	int nWeaponMatterObject = 1;
	ref_ptr<CRPhysics::crWeaponMatterObject> weaponMatterObject;
	ref_ptr<CRPhysics::crBulletMatterObject> bulletMatterObject;
	ref_ptr<crCollideCallback> bulletCollideCallback;
	ref_ptr<crCollideCallback> weaponCollideCallback;
	ref_ptr<crGroup> bulletParentGroup;
	float bulletMass = 0.1f;
	int bulletObjectIndex = 0;

	int nWeapon = 1;
	int nWeaponModel = 1;
	int nBulletModel = 1;
	int nCaissonModel = 1;
	int nHpModel = 1;
	int nArmorModel = 1;
	int nDescription = 1;
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
	crNode *node;
	while(cfg_script.Push("Weapon",nWeapon++))
	{
		nWeaponModel = 1;
		while(cfg_script.Push("WeaponModel",nWeaponModel++))
		{
			weaponObject = new CRPhysics::crWeaponObject;
			if(cfg_script.Get("GroupName", str))
			{
				searchByNameVisitor.reset();
				searchByNameVisitor.setNameId(str);
				searchByNameVisitor.setSearchNodeType(ALLNODE);
				CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
				node = searchByNameVisitor.getResult();
				if(node)
				{
				    weaponObject->addChild(node);
					//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): GroupName error "<<str<<std::endl;
					//if (!cfg_script.Pop())
					//	CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
					//continue;
				}
			}

			if(cfg_script.Get("MaxBulletsInOneClip", int1))
			{
				weaponObject->setMaxBulletsInOneClip(int1);
			}
			if(cfg_script.Get("TimeInterval", flt1))
			{
				weaponObject->setInterval(flt1);
			}
			if(cfg_script.Get("ReplaceClipTime", flt1))
			{
				weaponObject->setReplaceClipTime(flt1);
			}
			if(cfg_script.Get("AttackDistance", flt1))
			{
				weaponObject->setAttackDistance(flt1);
			}
			if(cfg_script.Get("GunPoint", v_i))
			{
				weaponObject->setGunPoint(crVector3(v_i[0],v_i[1],v_i[2]));
			}
			if(cfg_script.Get("ClipPos", v_i))
			{
				weaponObject->setClipPos(crVector3(v_i[0],v_i[1],v_i[2]));
			}
			if(cfg_script.Get("EffectAttrMask", str))
			{
				attrmask = 0;
				if(str.find("Fire_Particle") != -1)
					attrmask |= CRPhysics::crWeaponObject::Fire_Particle;
				if(str.find("Fire_Sequence") != -1)
					attrmask |= CRPhysics::crWeaponObject::Fire_Sequence;
				if(str.find("Fire_Sound") != -1)
					attrmask |= CRPhysics::crWeaponObject::Fire_Sound;
				if(str.find("Clip_Particle") != -1)
					attrmask |= CRPhysics::crWeaponObject::Clip_Particle;
				if(str.find("Clip_Sequence") != -1)
					attrmask |= CRPhysics::crWeaponObject::Clip_Sequence;
				if(str.find("Clip_Sound") != -1)
					attrmask |= CRPhysics::crWeaponObject::Clip_Sound;
				if(str.find("Collide_Particle") != -1)
					attrmask |= CRPhysics::crWeaponObject::Collide_Particle;
				if(str.find("Collide_Sequence") != -1)
					attrmask |= CRPhysics::crWeaponObject::Collide_Sequence;
				if(str.find("Collide_Sound") != -1)
					attrmask |= CRPhysics::crWeaponObject::Collide_Sound;

				weaponObject->setEffectAttrMask((CRPhysics::crWeaponObject::EffectAttrMask)attrmask);
			}
			if(cfg_script.Get("AttackMask", str))
			{
				attrmask = 0;
				if(str.find("AttackAll") != -1)
					attrmask |= CRPhysics::crWeaponObject::AttackAll;
				if(str.find("AttackGround") != -1)
					attrmask |= CRPhysics::crWeaponObject::AttackGround;
				if(str.find("AttackAir") != -1)
					attrmask |= CRPhysics::crWeaponObject::AttackAir;
				if(str.find("AttackDeepSea") != -1)
					attrmask |= CRPhysics::crWeaponObject::AttackDeepSea;

				weaponObject->setAttackMask(attrmask);
			}
			//if(cfg_script.Get("ShotSoundName", str))
			//{
			//	weaponObject->setShotSoundName(str);
			//}
			//if(cfg_script.Get("CollideSoundName", str))
			//{
			//	weaponObject->setCollideSoundName(str);
			//}
			//if(cfg_script.Get("ReplaceClipSoundName", str))
			//{
			//	weaponObject->setReplaceClipSoundName(str);
			//}
			//if(cfg_script.Get("FireEffectString", str))
			//{
			//	weaponObject->setFireEffectTypeString(str);
			//}
			CRPhysics::crWeaponManager::getInstance()->addWeapon(weaponObject.get());

			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
		}
		nBulletModel = 1;
		while(cfg_script.Push("BulletModel",nBulletModel++))
		{
			bulletObject = new CRPhysics::crBulletObject;
			if(cfg_script.Get("Name", str))
			{
				bulletObject->setName(str);
				cfg_script.Get("GroupName", str);
				searchByNameVisitor.reset();
				searchByNameVisitor.setNameId(str);
				searchByNameVisitor.setSearchNodeType(ALLNODE);
				CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
				node = searchByNameVisitor.getResult();
				if(node)
				{
					bulletObject->addChild(node);
					//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): GroupName error "<<str<<std::endl;
					//if (!cfg_script.Pop())
					//	CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
					//continue;
				}
			}

			if(cfg_script.Get("DamageType", str))
			{
				if(str.compare("Direct")==0)
					bulletObject->setDamageType(CRPhysics::crBulletObject::Direct);
				else if(str.compare("Explode")==0)
					bulletObject->setDamageType(CRPhysics::crBulletObject::Explode);
				else if(str.compare("Extra")==0)
					bulletObject->setDamageType(CRPhysics::crBulletObject::Extra);
			}
			if(cfg_script.Get("ExecutionRang", v_i))
			{
				bulletObject->setExecutionRang(v_i[0],v_i[1]);
			}
			if(cfg_script.Get("ExplodeRadius", flt1))
			{
				bulletObject->setExplodeRadius(flt1);
			}
			if(cfg_script.Get("K", flt1))
			{
				bulletObject->setK(flt1);
			}
			if(cfg_script.Get("Impetus", flt1))
			{
				bulletObject->setImpetus(flt1);
			}
			if(cfg_script.Get("Force", v_i))
			{
				bulletObject->setForce(CRCore::crVector3(v_i[0],v_i[1],v_i[2]));
			}
			if(cfg_script.Get("LinearVel", flt1))
			{
				bulletObject->setLinearVel(flt1);
			}
			if(cfg_script.Get("Duration", flt1))
			{
				bulletObject->setDuration(flt1);
			}
			if(cfg_script.Get("MissilePrecision", flt1))
			{
				bulletObject->setMissilePrecision(CRCore::rangef(-flt1,flt1));
			}
			if(cfg_script.Get("MissileFlexibility", flt1))
			{
				bulletObject->setMissileFlexibility(flt1);
			}
			if(cfg_script.Get("ParticleFlyScale", flt1))
			{
				bulletObject->setParticleFlyScale(flt1);
			}
			if(cfg_script.Get("ParticleFlyOffset", flt1))
			{
				bulletObject->setParticleFlyOffset(flt1);
			}
			if(cfg_script.Get("AttrMask", str))
			{
				attrmask = 0;
				if(str.find("GravityEnable") != -1)
					attrmask |= CRPhysics::crBulletObject::GravityEnable;
				if(str.find("VanishWhenCollide") != -1)
					attrmask |= CRPhysics::crBulletObject::VanishWhenCollide;
				if(str.find("Track") != -1)
					attrmask |= CRPhysics::crBulletObject::Track;
				
				bulletObject->setAttrMask((CRPhysics::crBulletObject::AttrMask)attrmask);
			}
			if(cfg_script.Get("EffectAttrMask", str))
			{
				attrmask = 0;
				if(str.find("Fly_Particle") != -1)
					attrmask |= CRPhysics::crBulletObject::Fly_Particle;
				if(str.find("Fly_Sequence") != -1)
					attrmask |= CRPhysics::crBulletObject::Fly_Sequence;
				if(str.find("Fly_Sound") != -1)
					attrmask |= CRPhysics::crBulletObject::Fly_Sound;
				if(str.find("Collide_Particle") != -1)
					attrmask |= CRPhysics::crBulletObject::Collide_Particle;
				if(str.find("Collide_Particle2") != -1)
					attrmask |= CRPhysics::crBulletObject::Collide_Particle2;
				if(str.find("Collide_Particle3") != -1)
					attrmask |= CRPhysics::crBulletObject::Collide_Particle3;
				if(str.find("Collide_Sequence") != -1)
					attrmask |= CRPhysics::crBulletObject::Collide_Sequence;
				if(str.find("Collide_Sound") != -1)
					attrmask |= CRPhysics::crBulletObject::Collide_Sound;
				if(str.find("Explode_Particle") != -1)
					attrmask |= CRPhysics::crBulletObject::Explode_Particle;
				if(str.find("Explode_Sequence") != -1)
					attrmask |= CRPhysics::crBulletObject::Explode_Sequence;
				if(str.find("Explode_Sound") != -1)
					attrmask |= CRPhysics::crBulletObject::Explode_Sound;

				bulletObject->setEffectAttrMask((CRPhysics::crBulletObject::EffectAttrMask)attrmask);
			}
			//if(cfg_script.Get("CollideSoundName", str))
			//{
			//	bulletObject->setCollideSoundName(str);
			//}
			if(cfg_script.Get("ExecutionPhysicsType", str))
			{
				if(str.compare("Collide")==0)
					bulletObject->setExecutionPhysicsType(crBulletObject::Type_Collide);
				else if(str.compare("Explode")==0)
					bulletObject->setExecutionPhysicsType(crBulletObject::Type_Explode);
				else if(str.compare("TimeExplode")==0)
					bulletObject->setExecutionPhysicsType(crBulletObject::Type_TimeExplode);
				else if(str.compare("Sphere")==0)
					bulletObject->setExecutionPhysicsType(crBulletObject::Type_Sphere);
				else if(str.compare("Ray")==0)
					bulletObject->setExecutionPhysicsType(crBulletObject::Type_Ray);
				else if(str.compare("Weapon")==0)
					bulletObject->setExecutionPhysicsType(crBulletObject::Type_Weapon);
				else if(str.compare("Shield_Box")==0)
					bulletObject->setExecutionPhysicsType(crBulletObject::Type_Shield_Box);
				else if(str.compare("Shield_Sphere")==0)
					bulletObject->setExecutionPhysicsType(crBulletObject::Type_Shield_Sphere);
				else if(str.compare("Volume")==0)
					bulletObject->setExecutionPhysicsType(crBulletObject::Type_Volume);
				else if(str.compare("ExplodeVolume")==0)
					bulletObject->setExecutionPhysicsType(crBulletObject::Type_ExplodeVolume);
			}
			if(cfg_script.Get("ExecutionPhysicsLength", flt1))
			{
				bulletObject->setExecutionPhysicsLength(flt1);
			}
			if(cfg_script.Get("ModelPhysicsRadius", flt1))
			{
				bulletObject->setModelPhysicsRadius(flt1);
			}
			if(cfg_script.Get("VolumeName", str))
			{
				bulletObject->setVolumeName(str);
			}
			if(cfg_script.Get("VolumeLength", v_i))
			{
				bulletObject->setVolumeLength(crVector3(v_i[0],v_i[1],v_i[2]));
			}
			if(cfg_script.Get("MaxTriggerCount", int1))
			{
				bulletObject->setMaxTriggerCount(int1);
			}
			if(cfg_script.Get("ReTriggerDelay", flt1))
			{
				bulletObject->setReTriggerDelay(flt1);
			}
			nDescription = 1;
			while (cfg_script.Get("addDescription", str,nDescription++))
			{
				bulletObject->addDescription(str);
			}
			CRPhysics::crWeaponManager::getInstance()->addBullet(bulletObject.get());
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
		}
		nCaissonModel = 1;
		while(cfg_script.Push("CaissonModel",nCaissonModel++))
		{
			caissonObject = new CRPhysics::crCaissonObject;
			cfg_script.Get("GroupName", str);
			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId(str);
			searchByNameVisitor.setSearchNodeType(GROUP);
			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
			node = searchByNameVisitor.getResult();
			if(!node)
			{
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): GroupName error "<<str<<std::endl;
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
				continue;
			}
			caissonObject->addChild(node);

			if(cfg_script.Get("WeaponName", str))
			{
				caissonObject->setWeaponName(str);
			}
			if(cfg_script.Get("EffectAttrMask", str))
			{
				attrmask = 0;
				if(str.find("Collide_Particle") != -1)
					attrmask |= CRPhysics::crCaissonObject::Collide_Particle;
				if(str.find("Collide_Sequence") != -1)
					attrmask |= CRPhysics::crCaissonObject::Collide_Sequence;
				if(str.find("Collide_Sound") != -1)
					attrmask |= CRPhysics::crCaissonObject::Collide_Sound;

				caissonObject->setEffectAttrMask((CRPhysics::crCaissonObject::EffectAttrMask)attrmask);
			}
			//if(cfg_script.Get("CollideSoundName", str))
			//{
			//	caissonObject->setCollideSoundName(str);
			//}
			CRPhysics::crWeaponManager::getInstance()->addCaisson(caissonObject.get());
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
		}
		nHpModel = 1;
		while(cfg_script.Push("HpModel",nHpModel++))
		{
			hpObject = new CRPhysics::crHpObject;
			cfg_script.Get("GroupName", str);
			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId(str);
			searchByNameVisitor.setSearchNodeType(GROUP);
			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
			node = searchByNameVisitor.getResult();
			if(!node)
			{
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): GroupName error "<<str<<std::endl;
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
				continue;
			}
			hpObject->addChild(node);

			if(cfg_script.Get("HpValue", int1))
			{
				hpObject->setHpValue(int1);
			}
			if(cfg_script.Get("EffectAttrMask", str))
			{
				attrmask = 0;
				if(str.find("Collide_Particle") != -1)
					attrmask |= CRPhysics::crHpObject::Collide_Particle;
				if(str.find("Collide_Sequence") != -1)
					attrmask |= CRPhysics::crHpObject::Collide_Sequence;
				if(str.find("Collide_Sound") != -1)
					attrmask |= CRPhysics::crHpObject::Collide_Sound;

				hpObject->setEffectAttrMask((CRPhysics::crHpObject::EffectAttrMask)attrmask);
			}
			//if(cfg_script.Get("CollideSoundName", str))
			//{
			//	hpObject->setCollideSoundName(str);
			//}
			CRPhysics::crWeaponManager::getInstance()->addHp(hpObject.get());
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
		}

		nArmorModel = 1;
		while(cfg_script.Push("ArmorModel",nArmorModel++))
		{
			armorObject = new CRPhysics::crArmorObject;
			cfg_script.Get("GroupName", str);
			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId(str);
			searchByNameVisitor.setSearchNodeType(GROUP);
			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
			node = searchByNameVisitor.getResult();
			if(!node)
			{
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadObjectEventCfg(): GroupName error "<<str<<std::endl;
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
				continue;
			}
			armorObject->addChild(node);

			if(cfg_script.Get("ArmorValue", int1))
			{
				armorObject->setArmorValue(int1);
			}
			if(cfg_script.Get("EffectAttrMask", str))
			{
				attrmask = 0;
				if(str.find("Collide_Particle") != -1)
					attrmask |= CRPhysics::crArmorObject::Collide_Particle;
				if(str.find("Collide_Sequence") != -1)
					attrmask |= CRPhysics::crArmorObject::Collide_Sequence;
				if(str.find("Collide_Sound") != -1)
					attrmask |= CRPhysics::crArmorObject::Collide_Sound;

				armorObject->setEffectAttrMask((CRPhysics::crArmorObject::EffectAttrMask)attrmask);
			}
			//if(cfg_script.Get("CollideSoundName", str))
			//{
			//	armorObject->setCollideSoundName(str);
			//}
			CRPhysics::crWeaponManager::getInstance()->addArmor(armorObject.get());
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
		}

		nWeaponMatterObject = 1;
		while(cfg_script.Push("WeaponMatterObject",nWeaponMatterObject++))
		{
			weaponMatterObject = new CRPhysics::crWeaponMatterObject;
			cfg_script.Get("Name", str);
			weaponMatterObject->setName(str);
			cfg_script.Get("BulletInsertIn", str);
			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId(str);
			searchByNameVisitor.setSearchNodeType(GROUP);
			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
			bulletParentGroup = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
			if(!bulletParentGroup)
			{
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): parentGroup not found "<<std::endl;
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
				continue;
			}

			if(cfg_script.Get("BulletCount", int1))
			{
				weaponMatterObject->setBulletCount(int1);
			}
			if(cfg_script.Get("BulletsInWeapon", int1))
			{
				weaponMatterObject->setBulletsInWeapon(int1);
			}
			if(cfg_script.Get("WeaponObjectIndexID", int1))
			{
				weaponMatterObject->addChild(CRPhysics::crWeaponManager::getInstance()->getWeapon(int1));
			}
			if(cfg_script.Get("BulletObjectIndexID", bulletObjectIndex))
			{
				bulletObject = CRPhysics::crWeaponManager::getInstance()->getBullet(bulletObjectIndex);
				//weaponMatterObject->setBulletObjectIndex(bulletObjectIndex);delete at 2011-2-26
			}
			//if(cfg_script.Get("BulletCollide", str))
			//{
			//	bulletCollideCallback = dynamic_cast<crCollideCallback *>(crEventCallbackManager::getInstance()->getEventCallback(str));
			//	if(!bulletCollideCallback.valid()) 
			//	{
			//		CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadWeaponManagerCfg: 未找到NodeCallback: "<<str<<std::endl;
			//	}
			//}
			//if(cfg_script.Get("WeaponCollide", str))
			//{
			//	weaponCollideCallback = dynamic_cast<crCollideCallback *>(crEventCallbackManager::getInstance()->getEventCallback(str));
			//	if(!weaponCollideCallback.valid()) 
			//	{
			//		CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadWeaponManagerCfg: 未找到NodeCallback: "<<str<<std::endl;
			//	}
			//	weaponMatterObject->addCollideCallback(weaponCollideCallback.get());
			//}
			if(cfg_script.Get("WeaponMass", flt1))
			{
				weaponMatterObject->setMass(flt1);
			}
			//if(cfg_script.Get("WeaponRackName", str))
			//{
			//	weaponMatterObject->setWeaponRackName(str);
			//}
			
			bulletMatterObject = new CRPhysics::crBulletMatterObject;
			//bulletMatterObject->addCollideCallback(bulletCollideCallback.get());
			bulletMatterObject->addChild(bulletObject.get());
			bulletMatterObject->setDescriptionsInString(bulletObject->getDescriptionsInString());
			bulletMatterObject->setName(bulletObject->getName());
			bulletMatterObject->setFlying(false);
			if(cfg_script.Get("BulletMass", bulletMass))
				bulletMatterObject->setMass(bulletMass);
			bulletParentGroup->addChild(bulletMatterObject.get());

			nDescription = 1;
			while (cfg_script.Get("addDescription", str,nDescription++))
			{
				weaponMatterObject->addDescription(str);
			}

			int nNodeEvent = 1;
			int nEvent = 1;
			int nEventParam = 1;
			_crInt32 typeParam1,typeParam2;
			ref_ptr<crEventCallback> event;
			while (cfg_script.Push("WeaponEvent", nNodeEvent++)) 
			{
				cfg_script.Get("TypeParam", str,1);
				typeParam1 = parseEventMsg(str);
				cfg_script.Get("TypeParam", str,2);
				typeParam2 = parseEventMsg(str);

				nEvent = 1;
				while (cfg_script.Push("Event", nEvent++))
				{
					cfg_script.Get("EventFunction", str);
					event = crEventCallbackManager::getInstance()->getEventCallback(str);
					if(!event) 
					{
						CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadCfgFile(): 未找到EventFunction: "<<str<<std::endl;
						continue;
					}
					nEventParam = 1;
					while (cfg_script.Get("EventParam", str,nEventParam++)) 
					{
						event->addEventParam(nEventParam-2,str);
					}

					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
					weaponMatterObject->insertEvent(MAKEINT64(typeParam1,typeParam2),event.get());
				}
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
			}
			nNodeEvent = 1;
			nEvent = 1;
			nEventParam = 1;
			while (cfg_script.Push("BulletEvent", nNodeEvent++)) 
			{
				cfg_script.Get("TypeParam", str,1);
				typeParam1 = parseEventMsg(str);
				cfg_script.Get("TypeParam", str,2);
				typeParam2 = parseEventMsg(str);

				nEvent = 1;
				while (cfg_script.Push("Event", nEvent++))
				{
					cfg_script.Get("EventFunction", str);
					event = crEventCallbackManager::getInstance()->getEventCallback(str);
					if(!event) 
					{
						CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadCfgFile(): 未找到EventFunction: "<<str<<std::endl;
						continue;
					}
					nEventParam = 1;
					while (cfg_script.Get("EventParam", str,nEventParam++)) 
					{
						event->addEventParam(nEventParam-2,str);
					}

					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
					bulletMatterObject->insertEvent(MAKEINT64(typeParam1,typeParam2),event.get());
				}
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
			}
			ref_ptr<crCollideCallback> collideCallback;
			int nCollideCallback = 1;
			int nParam = 1;
			while(cfg_script.Push("WeaponCollide", nCollideCallback++))
			{
				cfg_script.Get("Class", str);
				collideCallback = dynamic_cast<crCollideCallback *>(crEventCallbackManager::getInstance()->getEventCallback(str));
				if(!collideCallback) 
				{
					CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadCfgFile: 未找到CollideCallback: "<<str<<std::endl;
					continue;
				}
				if(collideCallback.valid())
				{
					weaponMatterObject->addCollideCallback(collideCallback.get());
					nParam = 1;
					while (cfg_script.Get("Param", str,nParam++))
					{
						collideCallback->addEventParam(nParam-2,str);
					}
				}
				else
				{
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): not find CollideCallback of "<<str<<std::endl;
				}
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
			}
			nCollideCallback = 1;
			nParam = 1;
			while(cfg_script.Push("BulletCollide", nCollideCallback++))
			{
				cfg_script.Get("Class", str);
				collideCallback = dynamic_cast<crCollideCallback *>(crEventCallbackManager::getInstance()->getEventCallback(str));
				if(!collideCallback) 
				{
					CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadCfgFile: 未找到CollideCallback: "<<str<<std::endl;
					continue;
				}
				if(collideCallback.valid())
				{
					bulletMatterObject->addCollideCallback(collideCallback.get());
					nParam = 1;
					while (cfg_script.Get("Param", str,nParam++))
					{
						collideCallback->addEventParam(nParam-2,str);
					}
				}
				else
				{
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): not find CollideCallback of "<<str<<std::endl;
				}
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
			}
			crWeaponManager::getInstance()->addBulletMatterObject(bulletObjectIndex,bulletMatterObject.get());
			crWeaponManager::getInstance()->insertWeaponMatterObject(weaponMatterObject.get());

			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
		}

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}

	/////////////////////////////////////
	ref_ptr<crGroup> weaponParentGroup;
	nWeaponMatterObject = 1;
	while(cfg_script.Push("WeaponMatterObject",nWeaponMatterObject++))
	{
		cfg_script.Get("Name", str);
        weaponMatterObject = crWeaponManager::getInstance()->getWeaponMatterObject(str);
        if(weaponMatterObject.valid())
		{
			weaponMatterObject = crWeaponManager::createWeaponMatterObject(weaponMatterObject.get());
			weaponMatterObject->setName(str);

			cfg_script.Get("WeaponInsertIn", str);
			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId(str);
			searchByNameVisitor.setSearchNodeType(GROUP);
			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
			weaponParentGroup = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
			if(!weaponParentGroup)
			{
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): parentGroup not found "<<std::endl;
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
				continue;
			}
			weaponParentGroup->addChild(weaponMatterObject.get());
			if(cfg_script.Get("Position", v_i))
			{
				weaponMatterObject->setInitPosition(crVector3(v_i[0],v_i[1],v_i[2]));
                //weaponMatterObject->addWeaponMatrix(crMatrix::translate(v_i[0],v_i[1],v_i[2]));
			}
			if(cfg_script.Get("ComeForthInterval", flt1))
			{
				weaponMatterObject->setComeForthInterval(flt1);
			}
			if(cfg_script.Get("Visiable", int1))
			{
				weaponMatterObject->setVisiable(int1);
			}
			if(cfg_script.Get("EnableIntersect", int1))
			{
				weaponMatterObject->setEnableIntersect(int1);
			}
		}
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}

	int nCaissonMatterObject = 1;
	ref_ptr<CRPhysics::crCaissonMatterObject> caissonMatterObject;
	ref_ptr<crGroup> caissonParentGroup;
	while(cfg_script.Push("CaissonMatterObject",nCaissonMatterObject++))
	{
		caissonMatterObject = new CRPhysics::crCaissonMatterObject;
		cfg_script.Get("Name", str);
		caissonMatterObject->setName(str);

		cfg_script.Get("CaissonInsertIn", str);
		searchByNameVisitor.reset();
		searchByNameVisitor.setNameId(str);
		searchByNameVisitor.setSearchNodeType(GROUP);
		CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
		caissonParentGroup = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
		if(!caissonParentGroup)
		{
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): parentGroup not found "<<std::endl;
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
			continue;
		}
		caissonParentGroup->addChild(caissonMatterObject.get());

		if(cfg_script.Get("BulletCount", int1))
		{
			caissonMatterObject->setBulletCount(int1);
		}
		if(cfg_script.Get("CaissonObjectIndexID", int1))
		{
			caissonMatterObject->addChild(CRPhysics::crWeaponManager::getInstance()->getCaisson(int1));
		}
		nDescription = 1;
		while (cfg_script.Get("addDescription", str,nDescription++))
		{
			caissonMatterObject->addDescription(str);
		}
		if(cfg_script.Get("CaissonCollide", str))
		{
			caissonMatterObject->setCollideCallback(dynamic_cast<crCollideCallback *>(crEventCallbackManager::getInstance()->getEventCallback(str)));
			//if(str.compare("CaissonCollideCallback") == 0)
			//{
			//	caissonMatterObject->setCollideCallback(crCaissonCollideCallback::getInstance());
			//}
		}
		if(cfg_script.Get("Position", v_i))
		{
			caissonMatterObject->setInitPosition(crVector3(v_i[0],v_i[1],v_i[2]));
		}
		if(cfg_script.Get("ComeForthInterval", flt1))
		{
			caissonMatterObject->setComeForthInterval(flt1);
		}
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}

	int nHpMatterObject = 1;
	ref_ptr<CRPhysics::crHpMatterObject> hpMatterObject;
	ref_ptr<crGroup> hpParentGroup;
	while(cfg_script.Push("HpMatterObject",nHpMatterObject++))
	{
		hpMatterObject = new CRPhysics::crHpMatterObject;
		cfg_script.Get("Name", str);
		hpMatterObject->setName(str);

		cfg_script.Get("HpInsertIn", str);
		searchByNameVisitor.reset();
		searchByNameVisitor.setNameId(str);
		searchByNameVisitor.setSearchNodeType(GROUP);
		CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
		hpParentGroup = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
		if(!hpParentGroup)
		{
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): parentGroup not found "<<std::endl;
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
			continue;
		}
		hpParentGroup->addChild(hpMatterObject.get());

		if(cfg_script.Get("HpObjectIndexID", int1))
		{
			hpMatterObject->addChild(CRPhysics::crWeaponManager::getInstance()->getHp(int1));
		}

		nDescription = 1;
		while (cfg_script.Get("addDescription", str,nDescription++))
		{
			hpMatterObject->addDescription(str);
		}
		if(cfg_script.Get("HpCollide", str))
		{
			//if(str.compare("HpCollideCallback") == 0)
			//{
			//	hpMatterObject->setCollideCallback(crHpCollideCallback::getInstance());
			//}
			hpMatterObject->setCollideCallback(dynamic_cast<crCollideCallback *>(crEventCallbackManager::getInstance()->getEventCallback(str)));
		}
		if(cfg_script.Get("Position", v_i))
		{
			hpMatterObject->setInitPosition(crVector3(v_i[0],v_i[1],v_i[2]));
		}
		if(cfg_script.Get("ComeForthInterval", flt1))
		{
			hpMatterObject->setComeForthInterval(flt1);
		}
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}

	int nArmorMatterObject = 1;
	ref_ptr<CRPhysics::crArmorMatterObject> armorMatterObject;
	ref_ptr<crGroup> armorParentGroup;
	while(cfg_script.Push("ArmorMatterObject",nArmorMatterObject++))
	{
		armorMatterObject = new CRPhysics::crArmorMatterObject;
		cfg_script.Get("Name", str);
		armorMatterObject->setName(str);

		cfg_script.Get("ArmorInsertIn", str);
		searchByNameVisitor.reset();
		searchByNameVisitor.setNameId(str);
		searchByNameVisitor.setSearchNodeType(GROUP);
		CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
		armorParentGroup = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
		if(!armorParentGroup)
		{
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): parentGroup not found "<<std::endl;
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
			continue;
		}
		armorParentGroup->addChild(armorMatterObject.get());

		if(cfg_script.Get("ArmorObjectIndexID", int1))
		{
			armorMatterObject->addChild(CRPhysics::crWeaponManager::getInstance()->getArmor(int1));
		}
		nDescription = 1;
		while (cfg_script.Get("addDescription", str,nDescription++))
		{
			armorMatterObject->addDescription(str);
		}
		if(cfg_script.Get("ArmorCollide", str))
		{
			//if(str.compare("ArmorCollideCallback") == 0)
			//{
			//	armorMatterObject->setCollideCallback(crArmorCollideCallback::getInstance());
			//}
			armorMatterObject->setCollideCallback(dynamic_cast<crCollideCallback *>(crEventCallbackManager::getInstance()->getEventCallback(str)));
		}
		if(cfg_script.Get("Position", v_i))
		{
			armorMatterObject->setInitPosition(crVector3(v_i[0],v_i[1],v_i[2]));
		}
		if(cfg_script.Get("ComeForthInterval", flt1))
		{
			armorMatterObject->setComeForthInterval(flt1);
		}
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}
	//int nEquip = 1;
	//CRPhysics::crViewMatterObject *vo;
	//ref_ptr<CRPhysics::crWeaponMatterObject> newWeapon;
	//while(cfg_script.Push("WeaponEquip",nEquip++))
	//{
	//	cfg_script.Get("VONameID", str);
	//	searchByNameVisitor.reset();
	//	searchByNameVisitor.setNameId(str);
	//	searchByNameVisitor.setSearchNodeType(crSearchNodeBYNameVisitor::MATRIXTRANSFORM);
	//	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	//	vo = dynamic_cast<CRPhysics::crViewMatterObject *>(searchByNameVisitor.getResult());
	//	if(!vo) 
	//	{
	//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponEquipCfg(): VONameID error "<<str<<std::endl;
	//		if (!cfg_script.Pop())
	//			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
	//		continue;
	//	}

	//	cfg_script.Get("WeaponNameID", str);
	//	searchByNameVisitor.reset();
	//	searchByNameVisitor.setNameId(str);
	//	searchByNameVisitor.setSearchNodeType(crSearchNodeBYNameVisitor::MATRIXTRANSFORM);
	//	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	//	weaponMatterObject = dynamic_cast<CRPhysics::crWeaponMatterObject *>(searchByNameVisitor.getResult());
	//	if(!weaponMatterObject.valid())
	//	{
	//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponEquipCfg(): WeaponNameID error "<<str<<std::endl;
	//		if (!cfg_script.Pop())
	//			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
	//		continue;
	//	}

	//	if(!vo->testToAddWeaponMatterObject(weaponMatterObject.get()))
	//	{
	//		newWeapon = new CRPhysics::crWeaponMatterObject;
	//		newWeapon->setName(weaponMatterObject->getName());
	//		newWeapon->setBulletCount(weaponMatterObject->getBulletCount());
	//		newWeapon->setBulletsInWeapon(weaponMatterObject->getBulletsInWeapon());
	//		newWeapon->addChild(weaponMatterObject->getChild(0));
	//		newWeapon->setMass(weaponMatterObject->getMass());
	//		//newWeapon->getBulletMatterObjectVec() = weaponMatterObject->getBulletMatterObjectVec();
	//		newWeapon->setBulletObjectIndex(weaponMatterObject->getBulletObjectIndex());
	//		vo->addWeaponMatterObject(newWeapon.get());
	//	}

	//	if(cfg_script.Get("HideBody", int1))
	//	{
	//		vo->setHideBody(int1);
	//        vo->setVisiable(!int1);
	//	}
	//	if (!cfg_script.Pop())
	//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadWeaponManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
	//}

	cfg_script.Pop();
}

//void CREncapsulation::loadMaterialCfg()
//{
//	rcfg::ConfigScript cfg_script;
//
//	struct stat buf;
//	if (stat( "script/material.cfg", &buf )) 
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadMaterialCfg(): file open error"<<std::endl;
//		return;
//	}
//
//	if (!cfg_script.Open("script/material.cfg"))
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadMaterialCfg(): "<<cfg_script.GetLastError()<<std::endl;
//		return;
//	}
//	std::string matName,uniformName,str;
//	std::vector<float> v_i;
//	float flt1;
//	float int1;
//	int nMat = 1;
//	int nUniform = 1;
//	ref_ptr<crStateSet> stateset;
//	ref_ptr<crUniform> uniform;
//	while(cfg_script.Push("Material",nMat++))
//	{
//		cfg_script.Get("Name", matName);
//        stateset = new crStateSet;
//		stateset->setName(matName);
//		nUniform = 1;
//		while(cfg_script.Push("UNIFORM",nUniform++))
//		{		
//			cfg_script.Get("name", uniformName);
//			cfg_script.Get("type", str);
//			if(str.compare("FLOAT")==0)
//			{
//				cfg_script.Get("value", flt1);
//				uniform = new crUniform(crUniform::FLOAT,uniformName);
//				uniform->set(flt1);
//			}
//			else if(str.compare("FLOAT_VEC2")==0)
//			{
//				cfg_script.Get("value", v_i);
//				uniform = new crUniform(crUniform::FLOAT_VEC2,uniformName);
//				uniform->set(crVector2(v_i[0],v_i[1]));
//			}
//			else if(str.compare("FLOAT_VEC3")==0)
//			{
//				cfg_script.Get("value", v_i);
//				uniform = new crUniform(crUniform::FLOAT_VEC3,uniformName);
//				uniform->set(crVector3(v_i[0],v_i[1],v_i[2]));
//				//CRCore::notify(CRCore::FATAL)<<"uniform FLOAT_VEC3: "<<uniformName<<" x="<<v_i[0]<<" y="<<v_i[1]<<" z="<<v_i[2]<<std::endl;
//			}
//			else if(str.compare("FLOAT_VEC4")==0)
//			{
//				cfg_script.Get("value", v_i);
//				uniform = new crUniform(crUniform::FLOAT_VEC4,uniformName);
//				uniform->set(crVector4(v_i[0],v_i[1],v_i[2],v_i[3]));
//				//CRCore::notify(CRCore::FATAL)<<"uniform FLOAT_VEC4: "<<uniformname<<" x="<<v_i[0]<<" y="<<v_i[1]<<" z="<<v_i[2]<<" w="<<v_i[3]<<std::endl;
//			}
//			else if(str.compare("INT")==0)
//			{
//				cfg_script.Get("value", int1);
//				uniform = new crUniform(crUniform::INT,uniformName);
//				uniform->set(int1);
//				//CRCore::notify(CRCore::FATAL)<<"uniform INT: "<<uniformname<<" x="<<int1<<std::endl;
//			}
//			else if(str.compare("INT_VEC2")==0)
//			{
//				cfg_script.Get("value", v_i);
//				uniform = new crUniform(crUniform::INT_VEC2,uniformName);
//				uniform->set((int)(v_i[0]),(int)(v_i[1]));
//			}
//			else if(str.compare("INT_VEC3")==0)
//			{
//				cfg_script.Get("value", v_i);
//				uniform = new crUniform(crUniform::INT_VEC3,uniformName);
//				uniform->set((int)(v_i[0]),(int)(v_i[1]),(int)(v_i[2]));
//			}
//			else if(str.compare("INT_VEC4")==0)
//			{
//				cfg_script.Get("value", v_i);
//				uniform = new crUniform(crUniform::INT_VEC4,uniformName);
//				uniform->set((int)(v_i[0]),(int)(v_i[1]),(int)(v_i[2]),(int)(v_i[3]));
//			}
//			else if(str.compare("BOOL")==0)
//			{
//				cfg_script.Get("value", int1);
//				uniform = new crUniform(crUniform::BOOL,uniformName);
//				uniform->set((bool)int1);
//			}
//			else if(str.compare("BOOL_VEC2")==0)
//			{
//				cfg_script.Get("value", v_i);
//				uniform = new crUniform(crUniform::BOOL_VEC2,uniformName);
//				uniform->set((bool)(v_i[0]),(bool)(v_i[1]));
//			}
//			else if(str.compare("BOOL_VEC3")==0)
//			{
//				cfg_script.Get("value", v_i);
//				uniform = new crUniform(crUniform::BOOL_VEC3,uniformName);
//				uniform->set((bool)(v_i[0]),(bool)(v_i[1]),(bool)(v_i[2]));
//			}
//			else if(str.compare("BOOL_VEC4")==0)
//			{
//				cfg_script.Get("value", v_i);
//				uniform = new crUniform(crUniform::BOOL_VEC4,uniformName);
//				uniform->set((bool)(v_i[0]),(bool)(v_i[1]),(bool)(v_i[2]),(bool)(v_i[3]));
//			}
//			else if(str.compare("FLOAT_MAT2")==0)
//			{
//				cfg_script.Get("value", v_i);
//				uniform = new crUniform(crUniform::FLOAT_MAT2,uniformName);
//				uniform->set(crMatrix2( v_i[0],v_i[1],
//					v_i[2],v_i[3]));
//			}
//			else if(str.compare("FLOAT_MAT3")==0)
//			{
//				cfg_script.Get("value", v_i);
//				uniform = new crUniform(crUniform::FLOAT_MAT3,uniformName);
//				uniform->set(crMatrix3( v_i[0],v_i[1],v_i[2],
//					v_i[3],v_i[4],v_i[5],
//					v_i[6],v_i[7],v_i[8]));
//			}
//			else if(str.compare("FLOAT_MAT4")==0)
//			{
//				cfg_script.Get("value", v_i);
//				uniform = new crUniform(crUniform::FLOAT_MAT4,uniformName);
//				uniform->set(crMatrixf( v_i[ 0],v_i[ 1],v_i[ 2],v_i[ 3],
//					v_i[ 4],v_i[ 5],v_i[ 6],v_i[ 7],
//					v_i[ 8],v_i[ 9],v_i[10],v_i[11],
//					v_i[12],v_i[13],v_i[14],v_i[15]));
//			}
//			else if(str.compare("SAMPLER_1D")==0)
//			{
//				cfg_script.Get("value", int1);
//				uniform = new crUniform(crUniform::SAMPLER_1D,uniformName);
//				uniform->set(int1);
//			}
//			else if(str.compare("SAMPLER_2D")==0)
//			{
//				cfg_script.Get("value", int1);
//				uniform = new crUniform(crUniform::SAMPLER_2D,uniformName);
//				uniform->set(int1);
//			}
//			else if(str.compare("SAMPLER_3D")==0)
//			{
//				cfg_script.Get("value", int1);
//				uniform = new crUniform(crUniform::SAMPLER_3D,uniformName);
//				uniform->set(int1);
//			}
//			else if(str.compare("SAMPLER_CUBE")==0)
//			{
//				cfg_script.Get("value", int1);
//				uniform = new crUniform(crUniform::SAMPLER_CUBE,uniformName);
//				uniform->set(int1);
//			}
//			else if(str.compare("SAMPLER_1D_SHADOW")==0)
//			{
//				cfg_script.Get("value", int1);
//				uniform = new crUniform(crUniform::SAMPLER_1D_SHADOW,uniformName);
//				uniform->set(int1);
//			}
//			else if(str.compare("SAMPLER_2D_SHADOW")==0)
//			{
//				cfg_script.Get("value", int1);
//				uniform = new crUniform(crUniform::SAMPLER_2D_SHADOW,uniformName);
//				uniform->set(int1);
//			}
//			stateset->addUniform(uniform.get());
//			if (!cfg_script.Pop())
//				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadMaterialCfg(): "<<cfg_script.GetLastError()<<std::endl;
//		}
//
//		crMaterialStateSetManager::getInstance()->insertMaterial(stateset.get());
//		if (!cfg_script.Pop())
//			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadMaterialCfg(): "<<cfg_script.GetLastError()<<std::endl;
//	}
//}
//
//void CREncapsulation::loadEffectParticleManagerCfg(const std::string &path)
//{
//	rcfg::ConfigScript cfg_script;
//
//	std::string filename;
//	if(path.empty())
//		filename = "script/EffectParticleManager.cfg";
//	else
//		filename = path + "/script/EffectParticleManager.cfg";
//	CRIOManager::crLoadCookFile scopedLoad(filename);
//
//	struct stat buf;
//	if (stat( filename.c_str(), &buf )) 
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEffectParticleManagerCfg(): file open error"<<std::endl;
//		return;
//	}
//
//	if (!cfg_script.Open(filename))
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEffectParticleManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
//		return;
//	}
//	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());
//	std::string str;
//	if(cfg_script.Push("EffectParticleManager"))
//	{
//		int nParticle = 1;
//        while(cfg_script.Get("FileName",str,nParticle++))
//		{
//            loadParticle(str);
//		}
//		if (!cfg_script.Pop())
//			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEffectParticleManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
//	}
//
//	CRParticle::crEffectParticleManager::getInstance()->setCullingActive(false);
//	cfg_script.Pop();
//}
void CREncapsulation::loadParticle(const std::string &fileName,crGroup *group,float loddist)
{
	if(!group) return;
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,fileName))
	{
		CRCore::notify(CRCore::FATAL)<<"loadParticle(): file open error,"<<fileName<<std::endl;
		return;
	}

	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());

	std::string str;
	std::string name;
	//std::string typeStr;
	std::vector<float> v_i;
	float flt1;
	int int1;
	ref_ptr<CRParticle::crParticleEffect> particleEffect;
	int nEffectNodesManager = 1;
	//ref_ptr<crObject> object = new crObject;
	int nCount = 1;
	int _count = 0;
	int nOperator = 1;
	int k = 1;
	float radius = 0.25;
	float density  = 1.0;
	ref_ptr<crOperator> _operator;
	ref_ptr<crParticleSystem> ps;
	ref_ptr<crEmitter> emitter;
	ref_ptr<CRParticle::crProgram> program;
	ref_ptr<crCounter> counter;
	ref_ptr<crPlacer> placer;
	ref_ptr<crShooter> shooter;
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
	ref_ptr<crGroup> particleModelGroup;
	ref_ptr<crObject> particleModelObject;
	ref_ptr<crObject> childObject;
	ref_ptr<crDrawable> drawable;
	ref_ptr<crStateSet> stateset;
	while(cfg_script.Push("Particle",nEffectNodesManager))
	{
		if(cfg_script.Get("Count", nCount))
			_count = _count == 0? nCount : _count;
		if(_count <= 1)
		{
			nEffectNodesManager++;
			_count = 0;
		}
		else _count--;
		name.clear();
		//typeStr.clear();
		cfg_script.Get("Name", name);
		//cfg_script.Get("TypeStr", typeStr);

		if(cfg_script.Push("ParticleEffect"))
		{
			particleEffect = new crParticleEffect;

			particleEffect->setName(name);
			if(cfg_script.Get("EnableCalcShadow",int1))
			{
				particleEffect->setCalcShadow(int1);
			}
			else
			{
                particleEffect->setCalcShadow(false);
			}
			if(cfg_script.Push("ParticleSystem"))
			{
				cfg_script.Get("Type", str);
				if(str.compare("ConnectedParticleSystem") == 0)
                    ps = new CRParticle::crConnectedParticleSystem;
				else ps = new CRParticle::crParticleSystem;

				particleEffect->setParticleSystem(ps.get());

				if(cfg_script.Get("TextureFileName", str))
				{
					ps->setDefaultAttributes(str, false, false);
				}
				//if(cfg_script.Get("DistortParam", v_i))
				//{
				//	ps->setDistortParam(crVector2(v_i[0],v_i[1]));
				//}
				if(cfg_script.Get("Alignment", str))
				{
					if(str.compare("BILLBOARD")==0)
					{
						ps->setParticleAlignment(crParticleSystem::BILLBOARD);
					}
					else if(str.compare("FIXED")==0)
					{
						ps->setParticleAlignment(crParticleSystem::FIXED);
					}
				}
                if(cfg_script.Get("TraverseString",str))
				{
                    ps->setTraverseString(str);
				}
				else
				{
					ps->setTraverseString("NoLight");
				}
				if(cfg_script.Get("ParticleModelObject",str))
				{
					searchByNameVisitor.reset();
					searchByNameVisitor.setNameId(str);
					searchByNameVisitor.setSearchNodeType(GROUP);
					CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
					particleModelGroup = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
					if(!particleModelGroup.valid())
					{
						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEffectParticleManagerCfg(): ParticleModelGroup not found "<<std::endl;
						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEffectParticleManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
						continue;
					}
					particleModelObject = new crObject;
					particleModelObject->setName(particleModelGroup->getName());
					particleModelObject->setStateSet(particleModelGroup->getStateSet());
					particleModelObject->setVisiable(particleModelGroup->getVisiable());
					particleModelObject->setCalcShadow(particleModelGroup->isCalcShadow());
					particleModelObject->setEnableIntersect(particleModelGroup->getEnableIntersect());
					particleModelObject->setRenderable(particleModelGroup->getRenderable());
					particleModelObject->setCollideMode(particleModelGroup->getCollideMode());
					particleModelObject->setCullingActive(particleModelGroup->getCullingActive());
					particleModelObject->setDescriptionsInString(particleModelGroup->getDescriptionsInString());
					stateset = particleModelObject->getOrCreateStateSet();
					NodeArray& childArray = particleModelGroup->getChildArray();
					for( CRCore::NodeArray::iterator itr = childArray.begin();
						itr != childArray.end();
						++itr)
					{//particleModelGroup下的所有Drawable共用相同的stateset
						childObject = dynamic_cast<crObject *>(itr->get());
						if(!childObject.valid()) childObject = dynamic_cast<crObject *>((dynamic_cast<crGroup *>(itr->get()))->getChild(0));
						if(!childObject.valid()) 
						{
							CRCore::notify(CRCore::ALWAYS)<<"CREncapsulation::loadEffectParticleManagerCfg(): error "<<particleModelGroup->getName()<< std::endl;
							continue;
						}
					    drawable = childObject->getDrawable(0);

						if(itr==childArray.begin())
						{
		                    if(drawable->getStateSet()) stateset->merge(*drawable->getStateSet());
							if(childObject->getStateSet()) stateset->merge(*childObject->getStateSet());
						}
					    drawable->setStateSet(0);
						particleModelObject->addDrawable(drawable.get());
					}
					ps->setParticleModelObject(particleModelObject.get());
				}
				if(cfg_script.Push("Particle"))
				{
					crParticle& def_particle = ps->getDefaultParticleTemplate();
					if(cfg_script.Get("Shape", str))
					{
						if(str.compare("QUAD")==0)
						{
							def_particle.setShape(crParticle::QUAD);
						}
						else if(str.compare("QUAD2")==0)
						{
							def_particle.setShape(crParticle::QUAD2);
						}
						else if(str.compare("POINT")==0)
						{
							def_particle.setShape(crParticle::POINT);
						}
						else if(str.compare("QUAD_TRIANGLESTRIP")==0)
						{
							def_particle.setShape(crParticle::QUAD_TRIANGLESTRIP);
						}
						else if(str.compare("HEXAGON")==0)
						{
							def_particle.setShape(crParticle::HEXAGON);
						}
						else if(str.compare("LINE")==0)
						{
							def_particle.setShape(crParticle::LINE);
						}
						else if(str.compare("Object")==0)	
						{
							def_particle.setShape(crParticle::Object);
							def_particle.setDrawableIdRange(CRCore::rangei(0,particleModelObject->getNumDrawables()));
						}
					}
					radius = 0.25;
					density  = 1.0;
					cfg_script.Get("Density", density);
					cfg_script.Get("Radius", radius);
					def_particle.setRadius(radius);
					if(cfg_script.Get("Mass", flt1))
					{
						def_particle.setMass(flt1);
					}
					else
					{
					    def_particle.setMass(density*radius*radius*radius*CRCore::PI*4.0f/3.0f);
					}
					if(cfg_script.Get("SizeRange", v_i))
					{
						if(v_i.size() == 2)
						{
							float minsize = radius*v_i[0];
							float maxsize = radius*v_i[1];
							def_particle.setSizeRange(CRCore::rangev3(crVector3(minsize,minsize,minsize),crVector3(maxsize,maxsize,maxsize)));
						}
						else
						{
							crVector3 minsize(v_i[0], v_i[1], v_i[2]);
							crVector3 maxsize(v_i[3], v_i[4], v_i[5]);
							def_particle.setSizeRange(CRCore::rangev3(minsize,maxsize));
						}
					}
					if(cfg_script.Get("AlphaRange", v_i))
					{
						def_particle.setAlphaRange(CRCore::rangef(v_i[0], v_i[1]));
					}
					if(cfg_script.Get("ColorRange", v_i))
					{
						crVector4 color1(v_i[0], v_i[1], v_i[2], v_i[3]);
						color1 /= 255.0f;
						crVector4 color2(v_i[4], v_i[5], v_i[6], v_i[7]);
						color2 /= 255.0f;
						def_particle.setColorRange(CRCore::rangev4(color1,color2));
					}
					if(cfg_script.Get("LifeTime", flt1))
					{
						def_particle.setLifeTime(flt1);
					}
					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEffectParticleManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
				} 

				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEffectParticleManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
			}

			if(cfg_script.Push("Emitter"))
			{
				emitter = NULL;
				if(cfg_script.Get("EmitterType", str))
				{
					if(str.compare("ModularEmitter") == 0)
						emitter = new crModularEmitter;
				}
				if(!emitter.valid())
				{
					emitter = new crModularEmitter;
				}

				if(cfg_script.Get("Enable", int1))
				{
					emitter->setEnabled(int1);
				}
				if(cfg_script.Get("Endless", int1))
				{///Endless = 1表示该粒子系统一直处于发射状态，不停止
					emitter->setEndless(int1);
				}
				if(cfg_script.Get("EmitterDuration", flt1))
				{
					emitter->setLifeTime(flt1);
				}
				if(cfg_script.Get("StartTime", flt1))
				{
					emitter->setStartTime(flt1);
				}
				if(cfg_script.Get("ResetTime", flt1))
				{
					emitter->setResetTime(flt1);
				}
				if(cfg_script.Get("CompenstationRatio", flt1))
				{
					if(dynamic_cast<crModularEmitter *>(emitter.get()))
						(dynamic_cast<crModularEmitter *>(emitter.get()))->setNumParticlesToCreateMovementCompenstationRatio(flt1);
				}
				if(cfg_script.Get("UseLocalParticleSystem", int1))
				{
					emitter->setReferenceFrame( int1?
						crParticleProcessor::ABSOLUTE_RF:
					crParticleProcessor::RELATIVE_RF);
				}
				if(cfg_script.Push("Counter"))
				{
					cfg_script.Get("CounterType", str);
					counter = 0;
					if(str.compare("RandomRateCounter") == 0)
					{
						counter = new crRandomRateCounter;
					}
					else if(str.compare("ConstantRateCounter") == 0)
					{
						counter = new crConstantRateCounter;
					}
					if(!counter.valid())
					{
						counter = new crRandomRateCounter;
					}
					k = 1;
					while (cfg_script.Get("CounterParam", str,k++)) 
					{
						counter->addCounterParam(k-2,str);
					}
					(dynamic_cast<CRParticle::crModularEmitter *>(emitter.get()))->setCounter(counter.get());
					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEffectParticleManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
				}
				if(cfg_script.Push("Placer"))
				{
					cfg_script.Get("PlacerType", str);
					placer = 0;
					if(str.compare("SectorPlacer") == 0)
					{
						placer = new crSectorPlacer;
					}
					else if(str.compare("BoxPlacer") == 0)
					{
						placer = new crBoxPlacer;
					}
					else if(str.compare("PointPlacer") == 0)
					{
						placer = new crPointPlacer;
					}
					else if(str.compare("MultiSegmentPlacer") == 0)
					{
						placer = new crMultiSegmentPlacer;
					}
					else if(str.compare("SegmentPlacer") == 0)
					{
						placer = new crSegmentPlacer;
					}
					if(!placer.valid())
					{
						placer = new crSectorPlacer;
					}
					k = 1;
					while (cfg_script.Get("PlacerParam", str,k++)) 
					{
						placer->addPlacerParam(k-2,str);
					}
					(dynamic_cast<CRParticle::crModularEmitter *>(emitter.get()))->setPlacer(placer.get());
					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEffectParticleManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
				}
				if(cfg_script.Push("Shooter"))
				{
					cfg_script.Get("ShooterType", str);
					shooter = 0;
					if(str.compare("RadialShooter") == 0)
					{
						shooter = new crRadialShooter;
					}
					if(!shooter.valid())
					{
						shooter = new crRadialShooter;
					}
					k = 1;
					while (cfg_script.Get("ShooterParam", str,k++)) 
					{
						shooter->addShooterParam(k-2,str);
					}
					(dynamic_cast<CRParticle::crModularEmitter *>(emitter.get()))->setShooter(shooter.get());
					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEffectParticleManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
				}

				particleEffect->setEmitter(emitter.get());
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEffectParticleManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
			}

			if(cfg_script.Push("Program"))
			{
				program = NULL;
				if(cfg_script.Get("ProgramType", str))
				{
					if(str.compare("ModularProgram") == 0)
						program = new crModularProgram;
					else if(str.compare("FluidProgram") == 0)
						program = new crFluidProgram;
				}
				if(!program.valid())
				{
					program = new crModularProgram;
				}
				if(dynamic_cast<CRParticle::crModularProgram *>(program.get()))
				{
					nOperator = 1;
					while(cfg_script.Push("Operator",nOperator++))
					{
						cfg_script.Get("OperatorType", str);
						_operator = 0;
						if(str.compare("AccelOperator") == 0)
						{
							_operator = new CRParticle::crAccelOperator;
						}
						else if(str.compare("AngularAccelOperator") == 0)
						{
							_operator = new CRParticle::crAngularAccelOperator;
						}
						else if(str.compare("FluidFrictionOperator") == 0)
						{
							_operator = new CRParticle::crFluidFrictionOperator;
						}
						else if(str.compare("ForceOperator") == 0)
						{
							_operator = new CRParticle::crForceOperator;
						}
						if(_operator.valid())
						{
							k = 1;
							while (cfg_script.Get("OperatorParam", str,k++)) 
							{
								//CRCore::notify(CRCore::FATAL)<<"addOperatorParam(): "<<str<<" k-2 = "<<k-2<<std::endl;
								_operator->addOperatorParam(k-2,str);
							}
							(dynamic_cast<CRParticle::crModularProgram *>(program.get()))->addOperator(_operator.get());
						}
						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEffectParticleManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
					}
				}
				else if(dynamic_cast<CRParticle::crFluidProgram *>(program.get()))
				{
					if(cfg_script.Get("Wind", v_i))
					{
						dynamic_cast<CRParticle::crFluidProgram *>(program.get())->setWind(crVector3(v_i[0],v_i[1],v_i[2]));
					}
					if(cfg_script.Get("FluidType", str))
					{
						if(str.compare("FluidToAir") == 0)
						{
							dynamic_cast<CRParticle::crFluidProgram *>(program.get())->setFluidType(crFluidProgram::Air);
						}
						else if(str.compare("FluidToWater") == 0)
						{
							dynamic_cast<CRParticle::crFluidProgram *>(program.get())->setFluidType(crFluidProgram::Water);
						}
					}
					if(cfg_script.Get("ToGravity", flt1))
					{
						dynamic_cast<CRParticle::crFluidProgram *>(program.get())->setToGravity(flt1);
					}
					if(cfg_script.Get("FluidDensity", flt1))
					{
						dynamic_cast<CRParticle::crFluidProgram *>(program.get())->setFluidDensity(flt1);
					}
					if(cfg_script.Get("FluidViscosity", flt1))
					{
						dynamic_cast<CRParticle::crFluidProgram *>(program.get())->setFluidViscosity(flt1);
					}
				}

				particleEffect->setProgram(program.get());
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEffectParticleManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
			}

			//particleEffect->buildEffect();
			//CRCore::notify(CRCore::FATAL)<<"crParticleEffect::loadEffectParticleManagerCfg(): "<<std::endl;
			group->addChild(particleEffect.get());
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEffectParticleManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
		}
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadEffectParticleManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}

	cfg_script.Pop();

	if(loddist>0.0f)
	{
		ref_ptr<crLod> lod = new crLod;
		lod->setName(group->getName());
		lod->addChild(group,0.0f,loddist);
		CRIOManager::crLoadManager::getInstance()->requestAddNode(crSceneManager::getInstance()->getDynamicRoot(),lod.get(),true);
	}
	else
	{
		CRIOManager::crLoadManager::getInstance()->requestAddNode(crSceneManager::getInstance()->getDynamicRoot(),group,true);
	}
}

void CREncapsulation::loadCfgMaterial(const std::string &cfgFile, CRCore::crStateSet *nodestateset)
{
	if(!nodestateset) return;

	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,cfgFile))
	{
		CRCore::notify(CRCore::FATAL)<<"loadCfgMaterial(): file open error,"<<cfgFile<<std::endl;
		return;
	}

	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());

	std::string str,str1;
	std::vector<float> v_i;
	int int1;
	float flt1;

	int nUniform = 1;
	if(cfg_script.Get("Shader", str))
	{
		crStateSet *shader = crShaderManager::getInstance()->getShaderStateSet(str);
		nodestateset->merge(*shader);
	}

	int nTexture = 1;
	int tex_unit = 0;
	ref_ptr<crTexture> texture;
	crTexture2D *tex2d;
	crTexture1D *tex1d;
	crTexture3D *tex3d;
	crTextureCubeMap *texCubeMap;
	while(cfg_script.Push("Texture", nTexture++))
	{		
		cfg_script.Get("tex_name", str);
		if(str.compare("envmap")==0)
		{
			//texture = crBrain::getInstance()->getCurrentActiveDB()->getDBEnvironmentMap();
		}
		else if(str.find("Noise3D")!=-1)
		{
			tex3d = CRCore::crBrain::getInstance()->getTexture3D(str);
			if(!cfg_script.Get("NoiseSample", v_i))
			{
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): error has not set NoiseSample, name = "<<str<<std::endl;
				if(v_i.size()!=4)
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): error NoiseSample need for size, name = "<<str<<std::endl;
			}
			if(!tex3d)
			{
				tex3d = CRUtil::crNoise::instance()->make3DNoiseTexture(v_i[0],v_i[1],v_i[2],v_i[3]);
				CRCore::crBrain::getInstance()->insertTexture3D(str,tex3d);
			}
			else
			{
				if(CRCore::crBrain::getInstance()->getNeedLoadImage3D(tex3d))
				{
					tex3d = CRUtil::crNoise::instance()->make3DNoiseTexture(v_i[0],v_i[1],v_i[2],v_i[3]);
					CRCore::crBrain::getInstance()->insertTexture3D(str,tex3d);
				}
			}

			texture = tex3d;
		}

		if(!texture.valid())
		{
			cfg_script.Get("tex_type", str1);

			if(str1.compare("TEXTURE1D")==0)
			{
				tex1d = CRCore::crBrain::getInstance()->getTexture1D(str);
				if(!tex1d)
				{
					tex1d = new crTexture1D;
					tex1d->setImage(0,tex1d->getImageDataRequestHandler()->requestImageFile(str,tex1d));
					CRCore::crBrain::getInstance()->insertTexture1D(tex1d->getImageNameID(),tex1d);
				}
				else
				{
					if(CRCore::crBrain::getInstance()->getNeedLoadImage1D(tex1d))
						tex1d->setImage(0,tex1d->getImageDataRequestHandler()->requestImageFile(str,tex1d));
				}

				texture = tex1d;
			}
			else if(str1.compare("TEXTURE2D")==0)
			{
				tex2d = CRCore::crBrain::getInstance()->getTexture2D(str);
				if(!tex2d)
				{
					tex2d = new crTexture2D;
					tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(str,tex2d));
					CRCore::crBrain::getInstance()->insertTexture2D(tex2d->getImageNameID(),tex2d);
				}
				else
				{
					if(CRCore::crBrain::getInstance()->getNeedLoadImage2D(tex2d))
						tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(str,tex2d));
				}

				texture = tex2d;
			}
			else if(str1.compare("TEXTURE3D")==0)
			{
				tex3d = CRCore::crBrain::getInstance()->getTexture3D(str);
				if(!tex3d)
				{
					tex3d = new crTexture3D;
					tex3d->setImage(0,tex3d->getImageDataRequestHandler()->requestImageFile(str,tex3d));
					CRCore::crBrain::getInstance()->insertTexture3D(tex3d->getImageNameID(),tex3d);
				}
				else
				{
					if(CRCore::crBrain::getInstance()->getNeedLoadImage3D(tex3d))
						tex3d->setImage(0,tex3d->getImageDataRequestHandler()->requestImageFile(str,tex3d));
				}

				texture = tex3d;
			}
			else if(str1.compare("TEXTURE_CUBE_MAP")==0)
			{
				texCubeMap = CRCore::crBrain::getInstance()->getTextureCubeMap(str);
				if(!texCubeMap)
				{
					texCubeMap = new crTextureCubeMap;
					texCubeMap->getImageDataRequestHandler()->requestCubeImageFile(str,texCubeMap);
					CRCore::crBrain::getInstance()->insertTextureCubeMap(texCubeMap->getImageNameID(),texCubeMap);
				}
				else
				{
					if(CRCore::crBrain::getInstance()->getNeedLoadImageCubeMap(texCubeMap))
					{
						texCubeMap->getImageDataRequestHandler()->requestCubeImageFile(str,texCubeMap);
					}
				}

				texture = texCubeMap;
			}
			else if(str1.compare("TEXTURE_RECTANGLE_NV")==0)
			{
				//texture = CRCore::crBrain::getInstance()->getTexture(str);
			}
		}

		if(!texture.valid()) 
		{
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): texture error"<<std::endl;		
			continue;
		}

		cfg_script.Get("tex_coord_set", tex_unit);

		cfg_script.Get("tex_wrap_mode", str);

		if(str.compare("CLAMP")==0)
		{
			texture->setWrap(crTexture::WRAP_S,crTexture::CLAMP);
			texture->setWrap(crTexture::WRAP_T,crTexture::CLAMP);
			texture->setWrap(crTexture::WRAP_R,crTexture::CLAMP);
		}
		else if(str.compare("CLAMP_TO_EDGE")==0)
		{
			texture->setWrap(crTexture::WRAP_S,crTexture::CLAMP_TO_EDGE);
			texture->setWrap(crTexture::WRAP_T,crTexture::CLAMP_TO_EDGE);
			texture->setWrap(crTexture::WRAP_R,crTexture::CLAMP_TO_EDGE);
		}
		else if(str.compare("CLAMP_TO_BORDER")==0)
		{
			texture->setWrap(crTexture::WRAP_S,crTexture::CLAMP_TO_BORDER);
			texture->setWrap(crTexture::WRAP_T,crTexture::CLAMP_TO_BORDER);
			texture->setWrap(crTexture::WRAP_R,crTexture::CLAMP_TO_BORDER);
		}
		else if(str.compare("REPEAT")==0)
		{
			texture->setWrap(crTexture::WRAP_S,crTexture::REPEAT);
			texture->setWrap(crTexture::WRAP_T,crTexture::REPEAT);
			texture->setWrap(crTexture::WRAP_R,crTexture::REPEAT);
		}
		else if(str.compare("MIRROR")==0)
		{
			texture->setWrap(crTexture::WRAP_S,crTexture::MIRROR);
			texture->setWrap(crTexture::WRAP_T,crTexture::MIRROR);
			texture->setWrap(crTexture::WRAP_R,crTexture::MIRROR);
		}

		cfg_script.Get("min_filter", str);
		if(str.compare("LINEAR")==0)
		{
			texture->setFilter(crTexture::MIN_FILTER,crTexture::LINEAR);
		}
		else if(str.compare("LINEAR_MIPMAP_LINEAR")==0)
		{
			texture->setFilter(crTexture::MIN_FILTER,crTexture::LINEAR_MIPMAP_LINEAR);
		}
		else if(str.compare("LINEAR_MIPMAP_NEAREST")==0)
		{
			texture->setFilter(crTexture::MIN_FILTER,crTexture::LINEAR_MIPMAP_NEAREST);
		}
		else if(str.compare("NEAREST")==0)
		{
			texture->setFilter(crTexture::MIN_FILTER,crTexture::NEAREST);
		}
		else if(str.compare("NEAREST_MIPMAP_LINEAR")==0)
		{
			texture->setFilter(crTexture::MIN_FILTER,crTexture::NEAREST_MIPMAP_LINEAR);
		}
		else if(str.compare("NEAREST_MIPMAP_NEAREST")==0)
		{
			texture->setFilter(crTexture::MIN_FILTER,crTexture::NEAREST_MIPMAP_NEAREST);
		}

		cfg_script.Get("mag_filter", str);
		if(str.compare("LINEAR")==0)
		{
			texture->setFilter(crTexture::MAG_FILTER,crTexture::LINEAR);
		}
		else if(str.compare("NEAREST")==0)
		{
			texture->setFilter(crTexture::MAG_FILTER,crTexture::NEAREST);
		}

		crStateAttribute::VRModeValue value = crStateAttribute::ON;
		if(cfg_script.Get("Mode", str))
		{
			if(str.find("OFF") != -1)
				value |= crStateAttribute::OFF;
			if(str.find("ON") != -1)
				value |= crStateAttribute::ON;
			if(str.find("OVERRIDE") != -1)
				value |= crStateAttribute::OVERRIDE;
			if(str.find("PROTECTED") != -1)
				value |= crStateAttribute::PROTECTED;
			if(str.find("INHERIT") != -1)
				value |= crStateAttribute::INHERIT;
		}

		nodestateset->setTextureAttributeAndModes(tex_unit,texture.get(),value);

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
	}

	ref_ptr<crUniform> uniform;
	std::string uniformname;
	std::string variance;
	while(cfg_script.Push("UNIFORM", nUniform++))
	{		
		cfg_script.Get("name", uniformname);
		cfg_script.Get("type", str);
		if(str.compare("FLOAT")==0)
		{
			cfg_script.Get("value", flt1);
			uniform = new crUniform(crUniform::FLOAT,uniformname);
			uniform->set(flt1);
		}
		else if(str.compare("FLOAT_VEC2")==0)
		{
			cfg_script.Get("value", v_i);
			uniform = new crUniform(crUniform::FLOAT_VEC2,uniformname);
			uniform->set(crVector2(v_i[0],v_i[1]));
		}
		else if(str.compare("FLOAT_VEC3")==0)
		{
			cfg_script.Get("value", v_i);
			uniform = new crUniform(crUniform::FLOAT_VEC3,uniformname);
			uniform->set(crVector3(v_i[0],v_i[1],v_i[2]));
			//CRCore::notify(CRCore::FATAL)<<"uniform FLOAT_VEC3: "<<uniformname<<" x="<<v_i[0]<<" y="<<v_i[1]<<" z="<<v_i[2]<<std::endl;
		}
		else if(str.compare("FLOAT_VEC4")==0)
		{
			cfg_script.Get("value", v_i);
			uniform = new crUniform(crUniform::FLOAT_VEC4,uniformname);
			uniform->set(crVector4(v_i[0],v_i[1],v_i[2],v_i[3]));
			//CRCore::notify(CRCore::FATAL)<<"uniform FLOAT_VEC4: "<<uniformname<<" x="<<v_i[0]<<" y="<<v_i[1]<<" z="<<v_i[2]<<" w="<<v_i[3]<<std::endl;
		}
		else if(str.compare("INT")==0)
		{
			cfg_script.Get("value", int1);
			uniform = new crUniform(crUniform::INT,uniformname);
			uniform->set(int1);
			//CRCore::notify(CRCore::FATAL)<<"uniform INT: "<<uniformname<<" x="<<int1<<std::endl;
		}
		else if(str.compare("INT_VEC2")==0)
		{
			cfg_script.Get("value", v_i);
			uniform = new crUniform(crUniform::INT_VEC2,uniformname);
			uniform->set((int)(v_i[0]),(int)(v_i[1]));
		}
		else if(str.compare("INT_VEC3")==0)
		{
			cfg_script.Get("value", v_i);
			uniform = new crUniform(crUniform::INT_VEC3,uniformname);
			uniform->set((int)(v_i[0]),(int)(v_i[1]),(int)(v_i[2]));
		}
		else if(str.compare("INT_VEC4")==0)
		{
			cfg_script.Get("value", v_i);
			uniform = new crUniform(crUniform::INT_VEC4,uniformname);
			uniform->set((int)(v_i[0]),(int)(v_i[1]),(int)(v_i[2]),(int)(v_i[3]));
		}
		else if(str.compare("BOOL")==0)
		{
			cfg_script.Get("value", int1);
			uniform = new crUniform(crUniform::BOOL,uniformname);
			uniform->set((bool)int1);
		}
		else if(str.compare("BOOL_VEC2")==0)
		{
			cfg_script.Get("value", v_i);
			uniform = new crUniform(crUniform::BOOL_VEC2,uniformname);
			uniform->set((bool)(v_i[0]),(bool)(v_i[1]));
		}
		else if(str.compare("BOOL_VEC3")==0)
		{
			cfg_script.Get("value", v_i);
			uniform = new crUniform(crUniform::BOOL_VEC3,uniformname);
			uniform->set((bool)(v_i[0]),(bool)(v_i[1]),(bool)(v_i[2]));
		}
		else if(str.compare("BOOL_VEC4")==0)
		{
			cfg_script.Get("value", v_i);
			uniform = new crUniform(crUniform::BOOL_VEC4,uniformname);
			uniform->set((bool)(v_i[0]),(bool)(v_i[1]),(bool)(v_i[2]),(bool)(v_i[3]));
		}
		else if(str.compare("FLOAT_MAT2")==0)
		{
			cfg_script.Get("value", v_i);
			uniform = new crUniform(crUniform::FLOAT_MAT2,uniformname);
			if(cfg_script.Get("value", v_i))
			{
				uniform->set(crMatrix2( v_i[0],v_i[1],
					v_i[2],v_i[3]));
			}
			else
			{
				uniform->set(crMatrix2());
			}
		}
		else if(str.compare("FLOAT_MAT3")==0)
		{
			uniform = new crUniform(crUniform::FLOAT_MAT3,uniformname);
			if(cfg_script.Get("value", v_i))
			{
				uniform->set(crMatrix3( v_i[0],v_i[1],v_i[2],
					v_i[3],v_i[4],v_i[5],
					v_i[6],v_i[7],v_i[8]));
			}
			else
			{
				uniform->set(crMatrix3());
			}
		}
		else if(str.compare("FLOAT_MAT4")==0)
		{
			uniform = new crUniform(crUniform::FLOAT_MAT4,uniformname);
			if(cfg_script.Get("value", v_i))
			{
				uniform->set(crMatrixf( v_i[ 0],v_i[ 1],v_i[ 2],v_i[ 3],
					v_i[ 4],v_i[ 5],v_i[ 6],v_i[ 7],
					v_i[ 8],v_i[ 9],v_i[10],v_i[11],
					v_i[12],v_i[13],v_i[14],v_i[15]));
			}
			else
			{
				uniform->set(crMatrixf::identity());
			}
		}
		else if(str.compare("SAMPLER_1D")==0)
		{
			cfg_script.Get("value", int1);
			uniform = new crUniform(crUniform::SAMPLER_1D,uniformname);
			uniform->set(int1);
		}
		else if(str.compare("SAMPLER_2D")==0)
		{
			cfg_script.Get("value", int1);
			uniform = new crUniform(crUniform::SAMPLER_2D,uniformname);
			uniform->set(int1);
		}
		else if(str.compare("SAMPLER_3D")==0)
		{
			cfg_script.Get("value", int1);
			uniform = new crUniform(crUniform::SAMPLER_3D,uniformname);
			uniform->set(int1);
		}
		else if(str.compare("SAMPLER_CUBE")==0)
		{
			cfg_script.Get("value", int1);
			uniform = new crUniform(crUniform::SAMPLER_CUBE,uniformname);
			uniform->set(int1);
		}
		else if(str.compare("SAMPLER_1D_SHADOW")==0)
		{
			cfg_script.Get("value", int1);
			uniform = new crUniform(crUniform::SAMPLER_1D_SHADOW,uniformname);
			uniform->set(int1);
		}
		else if(str.compare("SAMPLER_2D_SHADOW")==0)
		{
			cfg_script.Get("value", int1);
			uniform = new crUniform(crUniform::SAMPLER_2D_SHADOW,uniformname);
			uniform->set(int1);
		}
		if(uniform.valid())
		{
			variance = "STATIC";
			cfg_script.Get("variance", variance);
			if(variance.compare("DYNAMIC") == 0)
				uniform->setDataVariance(crBase::DYNAMIC);
			else
				uniform->setDataVariance(crBase::STATIC);
		}

		crStateAttribute::VRModeValue value = crStateAttribute::ON;
		if(cfg_script.Get("Mode", str))
		{
			if(str.find("OFF") != -1)
				value |= crStateAttribute::OFF;
			if(str.find("ON") != -1)
				value |= crStateAttribute::ON;
			if(str.find("OVERRIDE") != -1)
				value |= crStateAttribute::OVERRIDE;
			if(str.find("PROTECTED") != -1)
				value |= crStateAttribute::PROTECTED;
			if(str.find("INHERIT") != -1)
				value |= crStateAttribute::INHERIT;
		}
		nodestateset->addUniform(uniform.get(),value);

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
	}

	ref_ptr<crStateAttribute> stateAttribute;
	int nAttribute = 1;
	int nParam = 1;
	while(cfg_script.Push("StateAttribute", nAttribute++))
	{
		cfg_script.Get("Type", str);
		if(str.compare("AlphaFunc") == 0)
		{
			stateAttribute = new crAlphaFunc;
		}
		else if(str.compare("BlendColor") == 0)
		{
			stateAttribute = new crBlendColor;
		}
		else if(str.compare("BlendEquation") == 0)
		{
			stateAttribute = new crBlendEquation;
		}
		else if(str.compare("BlendFunc") == 0)
		{
			stateAttribute = new crBlendFunc;
		}
		else if(str.compare("ClampColor") == 0)
		{
			stateAttribute = new crClampColor;
		}
		else if(str.compare("ClipPlane") == 0)
		{
			stateAttribute = new crClipPlane;
		}
		else if(str.compare("ColorMask") == 0)
		{
			stateAttribute = new crColorMask;
		}
		else if(str.compare("CullFace") == 0)
		{
			stateAttribute = new crCullFace;
		}
		else if(str.compare("Depth") == 0)
		{
			stateAttribute = new crDepth;
		}
		else if(str.compare("Fog") == 0)
		{
			stateAttribute = new crFog;
		}
		else if(str.compare("FrontFace") == 0)
		{
			stateAttribute = new crFrontFace;
		}
		else if(str.compare("Hint") == 0)
		{
			stateAttribute = new crHint;
		}
		else if(str.compare("LightModel") == 0)
		{
			stateAttribute = new crLightModel;
		}
		else if(str.compare("LogicOp") == 0)
		{
			stateAttribute = new crLogicOp;
		}
		else if(str.compare("Multisample") == 0)
		{
			stateAttribute = new crMultisample;
		}
		else if(str.compare("PolygonMode") == 0)
		{
			stateAttribute = new crPolygonMode;
		}
		else if(str.compare("PolygonOffset") == 0)
		{
			stateAttribute = new crPolygonOffset;
		}
		else if(str.compare("Scissor") == 0)
		{
			stateAttribute = new crScissor;
		}
		else if(str.compare("ShadeModel") == 0)
		{
			stateAttribute = new crShadeModel;
		}
		else if(str.compare("Stencil") == 0)
		{
			stateAttribute = new crStencil;
		}
		else if(str.compare("StencilTwoSided") == 0)
		{
			stateAttribute = new crStencilTwoSided;
		}
		else if(str.compare("TexEnv") == 0)
		{
			stateAttribute = new crTexEnv;
		}
		else if(str.compare("TexEnvCombine") == 0)
		{
			stateAttribute = new crTexEnvCombine;
		}
		else if(str.compare("TexEnvFilter") == 0)
		{
			stateAttribute = new crTexEnvFilter;
		}
		else if(str.compare("TexGen") == 0)
		{
			stateAttribute = new crTexGen;
		}
		else if(str.compare("ViewPort") == 0)
		{
			stateAttribute = new crViewPort;
		}
		else if(str.compare("Material") == 0)
		{
			stateAttribute = new crMaterial;
		}
		nParam = 1;
		while (cfg_script.Get("AttributeParam", str,nParam++))
		{
			stateAttribute->addAttributeParam(nParam-2,str);
			//CRCore::notify(CRCore::FATAL)<<stateAttribute->className()<<" "<<str<<std::endl;
		}
		crStateAttribute::VRModeValue value = crStateAttribute::ON;
		if(cfg_script.Get("Mode", str))
		{
			if(str.find("OFF") != -1)
				value |= crStateAttribute::OFF;
			if(str.find("ON") != -1)
				value |= crStateAttribute::ON;
			if(str.find("OVERRIDE") != -1)
				value |= crStateAttribute::OVERRIDE;
			if(str.find("PROTECTED") != -1)
				value |= crStateAttribute::PROTECTED;
			if(str.find("INHERIT") != -1)
				value |= crStateAttribute::INHERIT;
		}
		nodestateset->setAttributeAndModes(stateAttribute.get(),value);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
	}
	if(cfg_script.Push("RenderBinDetails"))
	{
		cfg_script.Get("BinNum", int1);
		cfg_script.Get("BinName", str);
		nodestateset->setRenderBinDetails(int1,str);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
	}
	cfg_script.Pop();
}

void CREncapsulation::loadCfgFile(const std::string &cfgFile, CRCore::crNode *node)
{
	if(!node) return;

	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,cfgFile))
	{
		CRCore::notify(CRCore::FATAL)<<"loadCfgFile(): file open error,"<<cfgFile<<std::endl;
		return;
	}

	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());

	std::string str,str1;
	std::vector<float> v_i;
	int int1;
	float flt1;

	int nUniform = 1;
	ref_ptr<crStateSet> nodestateset;

	if(cfg_script.Get("Shader", str))
	{
		nodestateset = node->getOrCreateStateSet();
		crStateSet *shader = crShaderManager::getInstance()->getShaderStateSet(str);
		nodestateset->merge(*shader);
	}

	int nTexture = 1;
	int tex_unit = 0;
	ref_ptr<crTexture> texture;
	crTexture2D *tex2d;
	crTexture1D *tex1d;
	crTexture3D *tex3d;
	crTextureCubeMap *texCubeMap;
	while(cfg_script.Push("Texture", nTexture++))
	{		
		cfg_script.Get("tex_name", str);
		if(str.compare("envmap")==0)
		{
			//texture = crBrain::getInstance()->getCurrentActiveDB()->getDBEnvironmentMap();
		}
		else if(str.find("Noise3D")!=-1)
		{
			tex3d = CRCore::crBrain::getInstance()->getTexture3D(str);
			if(!cfg_script.Get("NoiseSample", v_i))
			{
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): error has not set NoiseSample, name = "<<str<<std::endl;
				if(v_i.size()!=4)
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): error NoiseSample need for size, name = "<<str<<std::endl;
			}
			if(!tex3d)
			{
				tex3d = CRUtil::crNoise::instance()->make3DNoiseTexture(v_i[0],v_i[1],v_i[2],v_i[3]);
				CRCore::crBrain::getInstance()->insertTexture3D(str,tex3d);
			}
			else
			{
				if(CRCore::crBrain::getInstance()->getNeedLoadImage3D(tex3d))
				{
					tex3d = CRUtil::crNoise::instance()->make3DNoiseTexture(v_i[0],v_i[1],v_i[2],v_i[3]);
					CRCore::crBrain::getInstance()->insertTexture3D(str,tex3d);
				}
			}

			texture = tex3d;
		}

		if(!texture.valid())
		{
			cfg_script.Get("tex_type", str1);

			if(str1.compare("TEXTURE1D")==0)
			{
				tex1d = CRCore::crBrain::getInstance()->getTexture1D(str);
				if(!tex1d)
				{
					tex1d = new crTexture1D;
					tex1d->setImage(0,tex1d->getImageDataRequestHandler()->requestImageFile(str,tex1d));
					CRCore::crBrain::getInstance()->insertTexture1D(tex1d->getImageNameID(),tex1d);
				}
				else
				{
					if(CRCore::crBrain::getInstance()->getNeedLoadImage1D(tex1d))
						tex1d->setImage(0,tex1d->getImageDataRequestHandler()->requestImageFile(str,tex1d));
				}

				texture = tex1d;
			}
			else if(str1.compare("TEXTURE2D")==0)
			{
				tex2d = CRCore::crBrain::getInstance()->getTexture2D(str);
				if(!tex2d)
				{
					tex2d = new crTexture2D;
					tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(str,tex2d));
					CRCore::crBrain::getInstance()->insertTexture2D(tex2d->getImageNameID(),tex2d);
				}
				else
				{
					if(CRCore::crBrain::getInstance()->getNeedLoadImage2D(tex2d))
						tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(str,tex2d));
				}

				texture = tex2d;
			}
			else if(str1.compare("TEXTURE3D")==0)
			{
				tex3d = CRCore::crBrain::getInstance()->getTexture3D(str);
				if(!tex3d)
				{
					tex3d = new crTexture3D;
					tex3d->setImage(0,tex3d->getImageDataRequestHandler()->requestImageFile(str,tex3d));
					CRCore::crBrain::getInstance()->insertTexture3D(tex3d->getImageNameID(),tex3d);
				}
				else
				{
					if(CRCore::crBrain::getInstance()->getNeedLoadImage3D(tex3d))
						tex3d->setImage(0,tex3d->getImageDataRequestHandler()->requestImageFile(str,tex3d));
				}

				texture = tex3d;
			}
			else if(str1.compare("TEXTURE_CUBE_MAP")==0)
			{
				texCubeMap = CRCore::crBrain::getInstance()->getTextureCubeMap(str);
				if(!texCubeMap)
				{
					texCubeMap = new crTextureCubeMap;
					texCubeMap->getImageDataRequestHandler()->requestCubeImageFile(str,texCubeMap);
					CRCore::crBrain::getInstance()->insertTextureCubeMap(texCubeMap->getImageNameID(),texCubeMap);
				}
				else
				{
					if(CRCore::crBrain::getInstance()->getNeedLoadImageCubeMap(texCubeMap))
					{
						texCubeMap->getImageDataRequestHandler()->requestCubeImageFile(str,texCubeMap);
					}
				}

				texture = texCubeMap;
			}
			else if(str1.compare("TEXTURE_RECTANGLE_NV")==0)
			{
				//texture = CRCore::crBrain::getInstance()->getTexture(str);
			}
		}

		if(!texture.valid()) 
		{
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): texture error"<<std::endl;		
			continue;
		}

		cfg_script.Get("tex_coord_set", tex_unit);

		cfg_script.Get("tex_wrap_mode", str);

		if(str.compare("CLAMP")==0)
		{
			texture->setWrap(crTexture::WRAP_S,crTexture::CLAMP);
			texture->setWrap(crTexture::WRAP_T,crTexture::CLAMP);
			texture->setWrap(crTexture::WRAP_R,crTexture::CLAMP);
		}
		else if(str.compare("CLAMP_TO_EDGE")==0)
		{
			texture->setWrap(crTexture::WRAP_S,crTexture::CLAMP_TO_EDGE);
			texture->setWrap(crTexture::WRAP_T,crTexture::CLAMP_TO_EDGE);
			texture->setWrap(crTexture::WRAP_R,crTexture::CLAMP_TO_EDGE);
		}
		else if(str.compare("CLAMP_TO_BORDER")==0)
		{
			texture->setWrap(crTexture::WRAP_S,crTexture::CLAMP_TO_BORDER);
			texture->setWrap(crTexture::WRAP_T,crTexture::CLAMP_TO_BORDER);
			texture->setWrap(crTexture::WRAP_R,crTexture::CLAMP_TO_BORDER);
		}
		else if(str.compare("REPEAT")==0)
		{
			texture->setWrap(crTexture::WRAP_S,crTexture::REPEAT);
			texture->setWrap(crTexture::WRAP_T,crTexture::REPEAT);
			texture->setWrap(crTexture::WRAP_R,crTexture::REPEAT);
		}
		else if(str.compare("MIRROR")==0)
		{
			texture->setWrap(crTexture::WRAP_S,crTexture::MIRROR);
			texture->setWrap(crTexture::WRAP_T,crTexture::MIRROR);
			texture->setWrap(crTexture::WRAP_R,crTexture::MIRROR);
		}

		cfg_script.Get("min_filter", str);
		if(str.compare("LINEAR")==0)
		{
			texture->setFilter(crTexture::MIN_FILTER,crTexture::LINEAR);
		}
		else if(str.compare("LINEAR_MIPMAP_LINEAR")==0)
		{
			texture->setFilter(crTexture::MIN_FILTER,crTexture::LINEAR_MIPMAP_LINEAR);
		}
		else if(str.compare("LINEAR_MIPMAP_NEAREST")==0)
		{
			texture->setFilter(crTexture::MIN_FILTER,crTexture::LINEAR_MIPMAP_NEAREST);
		}
		else if(str.compare("NEAREST")==0)
		{
			texture->setFilter(crTexture::MIN_FILTER,crTexture::NEAREST);
		}
		else if(str.compare("NEAREST_MIPMAP_LINEAR")==0)
		{
			texture->setFilter(crTexture::MIN_FILTER,crTexture::NEAREST_MIPMAP_LINEAR);
		}
		else if(str.compare("NEAREST_MIPMAP_NEAREST")==0)
		{
			texture->setFilter(crTexture::MIN_FILTER,crTexture::NEAREST_MIPMAP_NEAREST);
		}

		cfg_script.Get("mag_filter", str);
		if(str.compare("LINEAR")==0)
		{
			texture->setFilter(crTexture::MAG_FILTER,crTexture::LINEAR);
		}
		else if(str.compare("NEAREST")==0)
		{
			texture->setFilter(crTexture::MAG_FILTER,crTexture::NEAREST);
		}

		crStateAttribute::VRModeValue value = crStateAttribute::ON;
		if(cfg_script.Get("Mode", str))
		{
			if(str.find("OFF") != -1)
				value |= crStateAttribute::OFF;
			if(str.find("ON") != -1)
				value |= crStateAttribute::ON;
			if(str.find("OVERRIDE") != -1)
				value |= crStateAttribute::OVERRIDE;
			if(str.find("PROTECTED") != -1)
				value |= crStateAttribute::PROTECTED;
			if(str.find("INHERIT") != -1)
				value |= crStateAttribute::INHERIT;
		}

		nodestateset = node->getOrCreateStateSet();
		nodestateset->setTextureAttributeAndModes(tex_unit,texture.get(),value);

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
	}

	ref_ptr<crUniform> uniform;
	std::string uniformname;
	std::string variance;
	while(cfg_script.Push("UNIFORM", nUniform++))
	{		
		cfg_script.Get("name", uniformname);
		cfg_script.Get("type", str);
		if(str.compare("FLOAT")==0)
		{
			uniform = new crUniform(crUniform::FLOAT,uniformname);
			if(cfg_script.Get("value", flt1))
			    uniform->set(flt1);
			else
				uniform->set(0.0f);
		}
		else if(str.compare("FLOAT_VEC2")==0)
		{
			uniform = new crUniform(crUniform::FLOAT_VEC2,uniformname);
			if(cfg_script.Get("value", v_i))
				uniform->set(crVector2(v_i[0],v_i[1]));
			else
				uniform->set(crVector2(0.0f,0.0f));
		}
		else if(str.compare("FLOAT_VEC3")==0)
		{
			uniform = new crUniform(crUniform::FLOAT_VEC3,uniformname);
			if(cfg_script.Get("value", v_i))
			    uniform->set(crVector3(v_i[0],v_i[1],v_i[2]));
			else
				uniform->set(crVector3(0.0f,0.0f,0.0f));
			//CRCore::notify(CRCore::FATAL)<<"uniform FLOAT_VEC3: "<<uniformname<<" x="<<v_i[0]<<" y="<<v_i[1]<<" z="<<v_i[2]<<std::endl;
		}
		else if(str.compare("FLOAT_VEC4")==0)
		{
			uniform = new crUniform(crUniform::FLOAT_VEC4,uniformname);
            if(cfg_script.Get("value", v_i))
			    uniform->set(crVector4(v_i[0],v_i[1],v_i[2],v_i[3]));
			else
			    uniform->set(crVector4(0.0f,0.0f,0.0f,0.0f));
			//CRCore::notify(CRCore::FATAL)<<"uniform FLOAT_VEC4: "<<uniformname<<" x="<<v_i[0]<<" y="<<v_i[1]<<" z="<<v_i[2]<<" w="<<v_i[3]<<std::endl;
		}
		else if(str.compare("INT")==0)
		{
			uniform = new crUniform(crUniform::INT,uniformname);
			if(cfg_script.Get("value", int1))		
			    uniform->set(int1);
			else
				uniform->set(0);
			//CRCore::notify(CRCore::FATAL)<<"uniform INT: "<<uniformname<<" x="<<int1<<std::endl;
		}
		else if(str.compare("INT_VEC2")==0)
		{
			uniform = new crUniform(crUniform::INT_VEC2,uniformname);
			if(cfg_script.Get("value", v_i))
			    uniform->set((int)(v_i[0]),(int)(v_i[1]));
			else
			    uniform->set(0,0);
		}
		else if(str.compare("INT_VEC3")==0)
		{
			uniform = new crUniform(crUniform::INT_VEC3,uniformname);
			if(cfg_script.Get("value", v_i))
			    uniform->set((int)(v_i[0]),(int)(v_i[1]),(int)(v_i[2]));
			else
                uniform->set(0,0,0);
		}
		else if(str.compare("INT_VEC4")==0)
		{
			uniform = new crUniform(crUniform::INT_VEC4,uniformname);
			if(cfg_script.Get("value", v_i))
			    uniform->set((int)(v_i[0]),(int)(v_i[1]),(int)(v_i[2]),(int)(v_i[3]));
			else
                uniform->set(0,0,0,0);
		}
		else if(str.compare("BOOL")==0)
		{
			uniform = new crUniform(crUniform::BOOL,uniformname);
			if(cfg_script.Get("value", int1))
			    uniform->set((bool)int1);
			else
				uniform->set(0);
		}
		else if(str.compare("BOOL_VEC2")==0)
		{
			uniform = new crUniform(crUniform::BOOL_VEC2,uniformname);
			if(cfg_script.Get("value", v_i))
			    uniform->set((bool)(v_i[0]),(bool)(v_i[1]));
			else
				uniform->set(0,0);
		}
		else if(str.compare("BOOL_VEC3")==0)
		{
			uniform = new crUniform(crUniform::BOOL_VEC3,uniformname);
			if(cfg_script.Get("value", v_i))
			    uniform->set((bool)(v_i[0]),(bool)(v_i[1]),(bool)(v_i[2]));
			else
				uniform->set(0,0,0);
		}
		else if(str.compare("BOOL_VEC4")==0)
		{
			uniform = new crUniform(crUniform::BOOL_VEC4,uniformname);
			if(cfg_script.Get("value", v_i))
			    uniform->set((bool)(v_i[0]),(bool)(v_i[1]),(bool)(v_i[2]),(bool)(v_i[3]));
			else
				uniform->set(0,0,0,0);
		}
		else if(str.compare("FLOAT_MAT2")==0)
		{
			cfg_script.Get("value", v_i);
			uniform = new crUniform(crUniform::FLOAT_MAT2,uniformname);
			if(cfg_script.Get("value", v_i))
			{
				uniform->set(crMatrix2( v_i[0],v_i[1],
					v_i[2],v_i[3]));
			}
			else
			{
				uniform->set(crMatrix2());
			}
		}
		else if(str.compare("FLOAT_MAT3")==0)
		{
			uniform = new crUniform(crUniform::FLOAT_MAT3,uniformname);
			if(cfg_script.Get("value", v_i))
			{
				uniform->set(crMatrix3( v_i[0],v_i[1],v_i[2],
					v_i[3],v_i[4],v_i[5],
					v_i[6],v_i[7],v_i[8]));
			}
			else
			{
				uniform->set(crMatrix3());
			}
		}
		else if(str.compare("FLOAT_MAT4")==0)
		{
			uniform = new crUniform(crUniform::FLOAT_MAT4,uniformname);
			if(cfg_script.Get("value", v_i))
			{
				uniform->set(crMatrixf( v_i[ 0],v_i[ 1],v_i[ 2],v_i[ 3],
					v_i[ 4],v_i[ 5],v_i[ 6],v_i[ 7],
					v_i[ 8],v_i[ 9],v_i[10],v_i[11],
					v_i[12],v_i[13],v_i[14],v_i[15]));
			}
			else
			{
				uniform->set(crMatrixf::identity());
			}
		}
		else if(str.compare("SAMPLER_1D")==0)
		{
			cfg_script.Get("value", int1);
			uniform = new crUniform(crUniform::SAMPLER_1D,uniformname);
			uniform->set(int1);
		}
		else if(str.compare("SAMPLER_2D")==0)
		{
			cfg_script.Get("value", int1);
			uniform = new crUniform(crUniform::SAMPLER_2D,uniformname);
			uniform->set(int1);
		}
		else if(str.compare("SAMPLER_3D")==0)
		{
			cfg_script.Get("value", int1);
			uniform = new crUniform(crUniform::SAMPLER_3D,uniformname);
			uniform->set(int1);
		}
		else if(str.compare("SAMPLER_CUBE")==0)
		{
			cfg_script.Get("value", int1);
			uniform = new crUniform(crUniform::SAMPLER_CUBE,uniformname);
			uniform->set(int1);
		}
		else if(str.compare("SAMPLER_1D_SHADOW")==0)
		{
			cfg_script.Get("value", int1);
			uniform = new crUniform(crUniform::SAMPLER_1D_SHADOW,uniformname);
			uniform->set(int1);
		}
		else if(str.compare("SAMPLER_2D_SHADOW")==0)
		{
			cfg_script.Get("value", int1);
			uniform = new crUniform(crUniform::SAMPLER_2D_SHADOW,uniformname);
			uniform->set(int1);
		}
		if(uniform.valid())
		{
			variance = "STATIC";
			cfg_script.Get("variance", variance);
			if(variance.compare("DYNAMIC") == 0)
				uniform->setDataVariance(crBase::DYNAMIC);
			else
				uniform->setDataVariance(crBase::STATIC);
		}

		crStateAttribute::VRModeValue value = crStateAttribute::ON;
		if(cfg_script.Get("Mode", str))
		{
			if(str.find("OFF") != -1)
				value |= crStateAttribute::OFF;
			if(str.find("ON") != -1)
				value |= crStateAttribute::ON;
			if(str.find("OVERRIDE") != -1)
				value |= crStateAttribute::OVERRIDE;
			if(str.find("PROTECTED") != -1)
				value |= crStateAttribute::PROTECTED;
			if(str.find("INHERIT") != -1)
				value |= crStateAttribute::INHERIT;
		}
		nodestateset = node->getOrCreateStateSet();
		nodestateset->addUniform(uniform.get(),value);

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
	}

	ref_ptr<crStateAttribute> stateAttribute;
	int nAttribute = 1;
	int nParam = 1;
	while(cfg_script.Push("StateAttribute", nAttribute++))
	{
		cfg_script.Get("Type", str);
		if(str.compare("AlphaFunc") == 0)
		{
			stateAttribute = new crAlphaFunc;
		}
		else if(str.compare("BlendColor") == 0)
		{
			stateAttribute = new crBlendColor;
		}
		else if(str.compare("BlendEquation") == 0)
		{
			stateAttribute = new crBlendEquation;
		}
		else if(str.compare("BlendFunc") == 0)
		{
			stateAttribute = new crBlendFunc;
		}
		else if(str.compare("ClampColor") == 0)
		{
			stateAttribute = new crClampColor;
		}
		else if(str.compare("ClipPlane") == 0)
		{
			stateAttribute = new crClipPlane;
		}
		else if(str.compare("ColorMask") == 0)
		{
			stateAttribute = new crColorMask;
		}
		else if(str.compare("CullFace") == 0)
		{
			stateAttribute = new crCullFace;
		}
		else if(str.compare("Depth") == 0)
		{
			stateAttribute = new crDepth;
		}
		else if(str.compare("Fog") == 0)
		{
			stateAttribute = new crFog;
		}
		else if(str.compare("FrontFace") == 0)
		{
			stateAttribute = new crFrontFace;
		}
		else if(str.compare("Hint") == 0)
		{
			stateAttribute = new crHint;
		}
		else if(str.compare("LightModel") == 0)
		{
			stateAttribute = new crLightModel;
		}
		else if(str.compare("LogicOp") == 0)
		{
			stateAttribute = new crLogicOp;
		}
		else if(str.compare("Multisample") == 0)
		{
			stateAttribute = new crMultisample;
		}
		else if(str.compare("PolygonMode") == 0)
		{
			stateAttribute = new crPolygonMode;
		}
		else if(str.compare("PolygonOffset") == 0)
		{
			stateAttribute = new crPolygonOffset;
		}
		else if(str.compare("Scissor") == 0)
		{
			stateAttribute = new crScissor;
		}
		else if(str.compare("ShadeModel") == 0)
		{
			stateAttribute = new crShadeModel;
		}
		else if(str.compare("Stencil") == 0)
		{
			stateAttribute = new crStencil;
		}
		else if(str.compare("StencilTwoSided") == 0)
		{
			stateAttribute = new crStencilTwoSided;
		}
		else if(str.compare("TexEnv") == 0)
		{
			stateAttribute = new crTexEnv;
		}
		else if(str.compare("TexEnvCombine") == 0)
		{
			stateAttribute = new crTexEnvCombine;
		}
		else if(str.compare("TexEnvFilter") == 0)
		{
			stateAttribute = new crTexEnvFilter;
		}
		else if(str.compare("TexGen") == 0)
		{
			stateAttribute = new crTexGen;
		}
		else if(str.compare("ViewPort") == 0)
		{
			stateAttribute = new crViewPort;
		}
		else if(str.compare("Material") == 0)
		{
			stateAttribute = new crMaterial;
		}
		nParam = 1;
		while (cfg_script.Get("AttributeParam", str,nParam++))
		{
			stateAttribute->addAttributeParam(nParam-2,str);
			//CRCore::notify(CRCore::FATAL)<<stateAttribute->className()<<" "<<str<<std::endl;
		}
		crStateAttribute::VRModeValue value = crStateAttribute::ON;
		if(cfg_script.Get("Mode", str))
		{
			if(str.find("OFF") != -1)
				value |= crStateAttribute::OFF;
			if(str.find("ON") != -1)
				value |= crStateAttribute::ON;
			if(str.find("OVERRIDE") != -1)
				value |= crStateAttribute::OVERRIDE;
			if(str.find("PROTECTED") != -1)
				value |= crStateAttribute::PROTECTED;
			if(str.find("INHERIT") != -1)
				value |= crStateAttribute::INHERIT;
		}
		nodestateset = node->getOrCreateStateSet();
		nodestateset->setAttributeAndModes(stateAttribute.get(),value);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
	}
	if(cfg_script.Push("RenderBinDetails"))
	{
		cfg_script.Get("BinNum", int1);
		cfg_script.Get("BinName", str);
		nodestateset = node->getOrCreateStateSet();
		nodestateset->setRenderBinDetails(int1,str);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
	}
	ref_ptr<crNodeCallback> nodeCallback;
	int nCallback = 1;
	while(cfg_script.Push("NodeCallback", nCallback++))
	{
		cfg_script.Get("Class", str);
		nodeCallback = dynamic_cast<crNodeCallback *>(crEventCallbackManager::getInstance()->getEventCallback(str));
		if(!nodeCallback.valid()) 
		{
			CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadCfgFile: 未找到NodeCallback: "<<str<<std::endl;
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
			continue;
		}
		nParam = 1;
		while (cfg_script.Get("Param", str,nParam++))
		{
			nodeCallback->addEventParam(nParam-2,str);
		}
		nodeCallback->setCfgFile(cfgFile);
		cfg_script.Get("Type", str);
		if(dynamic_cast<crDofUniformTexMatCallback *>(nodeCallback.get()))
			str = "CullCallback";
		if(str.compare("CullCallback")==0)
		{
			if(crDisplaySettings::instance()->getRunMode()>0)
			{
				if(dynamic_cast<crDofUniformTexMatCallback *>(nodeCallback.get()))
				{
					crDofUniformTexMatCallback *dofUniformTex = dynamic_cast<crDofUniformTexMatCallback *>(nodeCallback.get());
					if(!dofUniformTex->getDofTransform()->getAnimationOn())
					{
						node->getStateSet()->removeUniform(dofUniformTex->getUniformID());
						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
						continue;
					}
				}
			}
			node->addCullCallback(nodeCallback.get());
		}
		else if(str.compare("UpdateCallback")==0)
		{
			node->addUpdateCallback(nodeCallback.get());
		}
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
	}

	ref_ptr<crCollideCallback> collideCallback;
	int nCollideCallback = 1;
	while(cfg_script.Push("CollideCallback", nCollideCallback++))
	{
		cfg_script.Get("Class", str);
		collideCallback = dynamic_cast<crCollideCallback *>(crEventCallbackManager::getInstance()->getEventCallback(str));
		if(!collideCallback.valid()) 
		{
			CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadCfgFile: 未找到CollideCallback: "<<str<<std::endl;
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
			continue;
		}
		node->addCollideCallback(collideCallback.get());
		nParam = 1;
		while (cfg_script.Get("Param", str,nParam++))
		{
			collideCallback->addEventParam(nParam-2,str);
		}
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
	}

	crObject *object = dynamic_cast<crObject *>(node);
	if(object)
	{
		int nObjLight = 1;
		while(cfg_script.Get("AvoidLight", str,nObjLight++))
		{
			object->addAvoidLight(str);
		}
		nObjLight = 1;
		while(cfg_script.Get("NeedLight", str,nObjLight++))
		{
			object->addNeedLight(str);
		}
	}

	if(cfg_script.Push("NodeParam"))
	{
		nParam = 1;
		while (cfg_script.Get("Param", str,nParam++))
		{
			node->addParam(nParam-2,str);
		}
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
	}

	cfg_script.Pop();
}

//CRPhysics::crViewMatterObject* CREncapsulation::loadBotCfg(const char* sceneName,CRCore::crGroup *botParent,const char* fileName, const char* botName, bool isNetBot, int netTeam)
//{
//	rcfg::ConfigScript cfg_script;
//
//	struct stat buf;
//	std::string file = fileName?fileName:"script/bot.cfg";
//	if (stat( file.c_str(), &buf )) 
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotCfg(): file open error "<<file<<std::endl;
//		return NULL;
//	}
//
//	if (!cfg_script.Open(file.c_str()))
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotCfg(): "<<cfg_script.GetLastError()<<std::endl;
//		return NULL;
//	}
//	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());
//
//	std::string str,str1;
//	int int1;
//	float flt1;
//	std::vector<float> v_i;
//	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
//	CRCore::ref_ptr<CRPhysics::crMatterGroup> mg;
//	CRCore::ref_ptr<CRCore::crGroup> externNode;
//	CRCore::ref_ptr<rbody::CreBodyNode> creBodyNode;
//	CRCore::ref_ptr<CRPhysics::crViewMatterObject> viewMatterObject;
//	CRCore::ref_ptr<crGroup> parent;
//	ref_ptr<CRPhysics::crRadarMatterObject> radar;
//	ref_ptr<CRPhysics::crMatterGroup> radarMg;
//	ref_ptr<CRPhysics::crWeaponMatterObject> weaponMatterObject;
//	//ref_ptr<CRPhysics::crWeaponMatterObject> newWeapon;
//	//crGroup *radarDetect;
//	int nBot = 1;
//	int nDescription = 1;
//	int nppc = 1;
//	int nParam = 1;
//	int nCfg = 1;
//	CRCore::ParseParamCallback *ppc;
//	crEventMsg *emsg;
//	crSave *save = crSceneManager::getInstance()->getSave(crSceneManager::getInstance()->getCurrentSaveIndex());
//	if(sceneName)
//	{
//		if(!cfg_script.Push(sceneName)) return NULL;
//	}
//	while(cfg_script.Push("Bot",nBot++))
//	{
//		radarMg = NULL;
//		mg = new CRPhysics::crMatterGroup;
//		mg->setDataVariance(CRCore::crBase::DYNAMIC);
//		if(botName)
//			mg->setName(botName);
//		else
//		{
//			cfg_script.Get("Name", str);
//			mg->setName(str);
//		}
//  //      if(save)
//		//{
//		//	if(save->isBotKilled(str))
//		//	{
//		//		if (!cfg_script.Pop())
//		//			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotCfg(): "<<cfg_script.GetLastError()<<std::endl;
//		//		continue;
//		//	}
//		//}
//		nppc = 1;
//		while (cfg_script.Push("PPC",nppc++)) 
//		{
//			cfg_script.Get("Type",str);
//			if(str.compare("crGeneralParamTransferPPC")==0)
//			{
//				emsg = crGeneralParamTransferPPC::makeEventMsg();
//				ppc = emsg->m_event->getParseParamCallback();
//			}
//			else if(str.compare("crWeaponEventPPC")==0)
//			{
//				emsg = crWeaponEventPPC::makeEventMsg();
//				ppc = emsg->m_event->getParseParamCallback();
//			}
//			else if(str.compare("crWeaponFiringPPC")==0)
//			{
//				emsg = crWeaponFiringPPC::makeEventMsg();
//				ppc = emsg->m_event->getParseParamCallback();
//			}
//			else if(str.compare("crJointSetParamPPC")==0)
//			{
//				emsg = crJointSetParamPPC::makeEventMsg();
//				ppc = emsg->m_event->getParseParamCallback();
//			}
//			else if(str.compare("crPhysicsResetPPC")==0)
//			{
//				emsg = crPhysicsResetPPC::makeEventMsg();
//				ppc = emsg->m_event->getParseParamCallback();
//			}
//			else if(str.compare("crSetForceAndTorquePPC")==0)
//			{
//				emsg = crSetForceAndTorquePPC::makeEventMsg();
//				ppc = emsg->m_event->getParseParamCallback();
//			}
//			else if(str.compare("crSetGeomorBodyInfoPPC")==0)
//			{
//				emsg = crSetGeomorBodyInfoPPC::makeEventMsg();
//				ppc = emsg->m_event->getParseParamCallback();
//			}
//			else if(str.compare("crSetGravityModePPC")==0)
//			{
//				emsg = crSetGravityModePPC::makeEventMsg();
//				ppc = emsg->m_event->getParseParamCallback();
//			}
//			else if(str.compare("crAsyncDataTransferPPC")==0)
//			{
//				//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotCfg(): crAsyncDataTransferPPC "<<std::endl;
//				emsg = crAsyncDataTransferPPC::makeEventMsg();
//				ppc = emsg->m_event->getParseParamCallback();
//			}
//
//			nParam = 1;
//			while (cfg_script.Get("Param", str,nParam++))
//			{
//				ppc->addParam(nParam-2,str);
//			}
//			mg->addEventMsgMap(emsg);
//			if (!cfg_script.Pop())
//				CRCore::notify(CRCore::FATAL)<<"crCreateAiVisitor::loadAI(): "<<cfg_script.GetLastError()<<std::endl;
//		}
//
//		if(cfg_script.Get("matterGroupMaterialString", str))
//		{
//			mg->setMatterGroupMaterialString(str);
//		}
//		if(cfg_script.Push("Body"))
//		{
//			cfg_script.Get("Type", str);
//			//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotCfg(): Type "<<str<<std::endl;
//
//			if(str.compare("crViewMatterObject")==0)
//			{
//                cfg_script.Get("CreBotModel", str);
//				viewMatterObject = new crViewMatterObject;
//				viewMatterObject->setBotModelName(str);
//				//CRCore::notify(CRCore::ALWAYS)<<" setBotModelName "<<str.c_str()<< std::endl;
//				CRIOManager::crLoadManager::getInstance()->requestCreBot(str,viewMatterObject.get(),false);
//			}
//			else if(str.compare("crCharacterMatterObject")==0)
//			{
//				cfg_script.Get("CreBotModel", str);
//				viewMatterObject = new crCharacterMatterObject;
//				viewMatterObject->setBotModelName(str);
//				if(cfg_script.Get("BlockRadius", flt1))
//					(dynamic_cast<crCharacterMatterObject *>(viewMatterObject.get()))->setBlockRadius(flt1);
//				//CRCore::notify(CRCore::ALWAYS)<<" setBotModelName "<<str.c_str()<< std::endl;
//				CRIOManager::crLoadManager::getInstance()->requestCreBot(str,viewMatterObject.get(),false);
//			}
//			else if(str.compare("crMissileSetOrFlackMatterObject") == 0)
//			{
//				cfg_script.Get("CreBotModel", str);
//				viewMatterObject = new crMissileSetOrFlackMatterObject;
//				viewMatterObject->setBotModelName(str);
//				CRIOManager::crLoadManager::getInstance()->requestCreBot(str,viewMatterObject.get(),false);
//			}
//			else if(str.compare("crBodyCaseMatterObject") == 0)
//			{
//				cfg_script.Get("CreBotModel", str);
//				viewMatterObject = new crBodyCaseMatterObject;
//				viewMatterObject->setBotModelName(str);
//				CRIOManager::crLoadManager::getInstance()->requestCreBot(str,viewMatterObject.get(),false);
//			}
//			//else if(str.compare("CreBodyNode")==0)
//			//{
//			//	cfg_script.Get("CharacterBotModel", str);
//			//	externNode = dynamic_cast<crGroup *>(CRIOManager::readNodeFile(str));
//   //             if(externNode.valid()) creBodyNode = dynamic_cast<rbody::CreBodyNode *>(externNode->getChild(0));
//			//	if(!creBodyNode.valid()) return NULL;
//			//	externNode->removeChild(creBodyNode.get());
//			//	//creBodyNode->setUpdateMode(creBodyNode->getUpdateMode() ^ rbody::CreBodyNode::UPDATE_GROUND_FOLLOWING);
//			//	//creBodyNode->setEnableIntersect(false);///角色暂不做物理计算
//			//	rbody::ActionRequest *action = creBodyNode->getBody()->getActionPrototype( creBodyNode->getBody()->getCoreBody()->getDefaultActivity() );
//			//	if (action) 
//			//	{
//			//		action->setPrioritized(false);
//			//		creBodyNode->getBody()->executeAction(action, false);
//			//		creBodyNode->getBody()->update(rvrutils::randInterval(0,2));
//			//	}
//   //             viewMatterObject = dynamic_cast<CRPhysics::crViewMatterObject *>(creBodyNode.get());
//			//	viewMatterObject->setBotModelName(str);
//			//}
//			if(viewMatterObject.valid())
//			{
//				if(cfg_script.Get("PhysicsBox", v_i))
//				{
//					viewMatterObject->setPhysicsBox(crVector3(v_i[0],v_i[1],v_i[2]));
//				}
//				if(cfg_script.Get("NeedViewPitch", int1))
//				{
//					viewMatterObject->setNeedViewPitch(int1);
//				}
//				if(cfg_script.Get("AttributeMask", str))
//				{
//					unsigned int attrmask = 0;
//					if(str.find("AttrAll") != -1)
//						attrmask |= CRCore::crGroup::AttrAll;
//					if(str.find("AttrGround") != -1)
//						attrmask |= CRCore::crGroup::AttrGround;
//					if(str.find("AttrAir") != -1)
//						attrmask |= CRCore::crGroup::AttrAir;
//					if(str.find("AttrDeepSea") != -1)
//						attrmask |= CRCore::crGroup::AttrDeepSea;
//
//					viewMatterObject->setAttributeMask(attrmask);
//				}
//				ref_ptr<crNodeCallback> nodeCallback;
//				int nCallback = 1;
//				int nParam = 1;
//				while(cfg_script.Push("NodeCallback", nCallback++))
//				{
//					cfg_script.Get("Class", str);
//					if(str.compare("Detect") == 0)
//					{
//						nodeCallback = new CREncapsulation::crDetectCallback;
//					}
//					else
//					{
//						nodeCallback = dynamic_cast<crNodeCallback *>(crEventCallbackManager::getInstance()->getEventCallback(str));
//						if(!nodeCallback) 
//						{
//							CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadCfgFile: 未找到NodeCallback: "<<str<<std::endl;
//							continue;
//						}
//					}
//					//else if(str.compare("FlyGameViewFilter") == 0)
//					//{
//					//	nodeCallback = new CREncapsulation::crFlyGameViewFilterCallback;
//					//}
//					//else if(str.compare("ParticleBind") == 0)
//					//{
//					//	nodeCallback = new CREncapsulation::crParticleBindCallback;
//					//}
//					//else if(str.compare("SequenceBind") == 0)
//					//{
//					//	nodeCallback = new CREncapsulation::crSequenceBindCallback;
//					//}
//					cfg_script.Get("Type", str);
//					if(str.compare("CullCallback")==0)
//					{
//						viewMatterObject->addCullCallback(nodeCallback.get());
//					}
//					else if(str.compare("UpdateCallback")==0)
//					{
//						viewMatterObject->addUpdateCallback(nodeCallback.get());
//					}
//					nParam = 1;
//					while (cfg_script.Get("Param", str,nParam++))
//					{
//						nodeCallback->addEventParam(nParam-2,str);
//					}
//					if (!cfg_script.Pop())
//						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotCfg(): "<<cfg_script.GetLastError()<<std::endl;
//				}
//                int nRack = 1;
//				while (cfg_script.Push("WeaponRack", nRack++))
//				{
//					cfg_script.Get("RackName", str);
//                    cfg_script.Get("RackNode", str1);
//					viewMatterObject->insertWeaponRackName(str,str1);
//
//					if (!cfg_script.Pop())
//						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotCfg(): "<<cfg_script.GetLastError()<<std::endl;
//				}
//				nRack = 1;
//				while (cfg_script.Push("BodyRack", nRack++))
//				{
//					cfg_script.Get("RackNode", str);
//					cfg_script.Get("BodyName", str1);
//					crBodyCaseMatterObject *bodyCase = dynamic_cast<crBodyCaseMatterObject *>(viewMatterObject.get());
//					if(bodyCase)
//						bodyCase->insertBodyRackName(str,str1);
//					else
//						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotCfg(): BodyRack 被配置到非crBodyCaseMatterObject上"<<std::endl;
//
//					if (!cfg_script.Pop())
//						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotCfg(): "<<cfg_script.GetLastError()<<std::endl;
//				}
//				nDescription = 1;
//				while (cfg_script.Get("addDescription", str,nDescription++))
//				{
//					viewMatterObject->addDescription(str);
//				}
//				nCfg = 1;
//				while (cfg_script.Get("cfgFile", str,nCfg++))
//				{
//					loadCfgFile(str,viewMatterObject.get());
//				}
//
//				mg->addChild(viewMatterObject.get());
//				viewMatterObject->setName(mg->getName());
//				ref_ptr<crCollideCallback> collideCallback;
//				int nCollideCallback = 1;
//				while(cfg_script.Push("CollideCallback", nCollideCallback++))
//				{
//					cfg_script.Get("Class", str);
//					collideCallback = dynamic_cast<crCollideCallback *>(crEventCallbackManager::getInstance()->getEventCallback(str));
//					if(!collideCallback) 
//					{
//						CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadBotCfg: 未找到CollideCallback: "<<str<<std::endl;
//					}
//					if(collideCallback.valid())
//					{
//						viewMatterObject->addCollideCallback(collideCallback.get());
//						nParam = 1;
//						while (cfg_script.Get("Param", str,nParam++))
//						{
//							collideCallback->addEventParam(nParam-2,str);
//						}
//					}
//					else
//					{
//						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotCfg(): not find CollideCallback of "<<str<<std::endl;
//					}
//					if (!cfg_script.Pop())
//						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotCfg(): "<<cfg_script.GetLastError()<<std::endl;
//				}
//				//if(cfg_script.Get("ViewObjectCollide", str))
//				//{
//				//	if(str.compare("crViewObjectCollideCallback") == 0)
//				//		viewMatterObject->setCollideCallback(crViewObjectCollideCallback::getInstance());
//				//	else if(str.compare("crAircraftCollideCallback") == 0)
//				//		viewMatterObject->setCollideCallback(crAircraftCollideCallback::getInstance());
//				//	//else if(str.compare("crBodyCaseCollideCallback") == 0)
//				//	//	viewMatterObject->setCollideCallback(CRPhysics::crBodyCaseCollideCallback::getInstance());
//				//}
//				//if(cfg_script.Get("MouseManipularCollide", str))
//				//{
//				//	if(str.compare("crMouseManipularCollideCallback") == 0)
//				//		viewMatterObject->addCollideCallback(crMouseManipularCollideCallback::getInstance());
//				//}
//				if(cfg_script.Get("AttachRadar", v_i))
//				{
//					radar = new CRPhysics::crRadarMatterObject;
//					radar->setDetectRaySizeTo(v_i[0],v_i[1],v_i[2]);
//					radarMg = new CRPhysics::crMatterGroup;
//					radarMg->setMatterGroupMaterialString("RadarCollideDetect");
//					radarMg->addChild(radar.get());
//					viewMatterObject->attachRadar(radar.get());
//
//					//crBrain::getInstance()->getCurrentActiveDB()->addChild(radarMg.get());
//					//mg->addChild(radarMg.get());
//				}
//				//if(cfg_script.Get("AttachRadar", str))
//				//{
//				//	if(str.compare("Radar") == 0)
//				//	{
//				//		radar = new CRPhysics::crRadarMatterObject;
//				//		radarMg = new CRPhysics::crMatterGroup;
//				//		radarMg->setMatterGroupMaterialString("RadarCollideDetect");
//				//		radarMg->addChild(radar.get());
//				//		viewMatterObject->attachRadar(radar.get());
//
//				//		mg->addChild(radarMg.get());
//				//	}
//				//	//else if(str.compare("Detect") == 0)
//				//	//{
//				//	//	viewMatterObject->setUpdateCallback(CRPhysics::crDetectCallback::getInstance());
//				//	//}
//				//}
//
//				int nWE = 1;
//				while(cfg_script.Push("WeaponEquip",nWE++))
//				{
//					cfg_script.Get("Weapon", str);
//					weaponMatterObject = CRPhysics::crWeaponManager::getInstance()->getWeaponMatterObject(str);
//					if(weaponMatterObject.valid() && !viewMatterObject->testToAddWeaponMatterObject(weaponMatterObject.get()))
//					{
//						weaponMatterObject = CRPhysics::crWeaponManager::createWeaponMatterObject(weaponMatterObject.get());
//						weaponMatterObject->setName(str);
//						viewMatterObject->addWeaponMatterObject(weaponMatterObject.get());
//
//						//if(cfg_script.Get("BulletCount", int1))
//						//{
//                        //  weaponMatterObject->setBulletCount(int1);
//						//}
//						//if(cfg_script.Get("BulletsInWeapon", int1))
//						//{
//						//	weaponMatterObject->setBulletsInWeapon(int1);
//						//}
//						if(cfg_script.Get("IsCurrentWeaponMatterObject", int1) && int1)
//						{
//							viewMatterObject->selectWeaponMatterObject(weaponMatterObject.get());
//						}
//					}
//					if (!cfg_script.Pop())
//						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotCfg(): "<<cfg_script.GetLastError()<<std::endl;
//				}
//
//				if(cfg_script.Get("HideBody", int1))
//				{
//					viewMatterObject->setHideBody(int1);
//					viewMatterObject->setVisiable(!int1);
//					viewMatterObject->setEnableIntersect(!int1);
//				}
//				if(cfg_script.Get("MaxSpeed", v_i))
//                    viewMatterObject->setMaxSpeed(crVector3(v_i[0],v_i[1],v_i[2]));
//				if(cfg_script.Get("MinSpeed", v_i))
//					viewMatterObject->setMinSpeed(crVector3(v_i[0],v_i[1],v_i[2]));
//				if(cfg_script.Get("Velocity", v_i))
//					viewMatterObject->setVelocity(crVector3(v_i[0],v_i[1],v_i[2]));
//				if(cfg_script.Get("GroundDetect", int1))
//					viewMatterObject->setGroundDetect((int1));
//				if(cfg_script.Get("CanFallDown", int1))
//					viewMatterObject->setCanFalldown((int1));
//				if(cfg_script.Get("ViewOffset", flt1))
//					viewMatterObject->setViewOffset(flt1);
//				if(cfg_script.Get("ViewHeight", flt1))
//					viewMatterObject->setViewHeight(flt1);
//				if(cfg_script.Get("Orientation", v_i))
//				{
//					CRCore::crQuat q1(DegreesToRadians(v_i[0]), CRCore::crVector3(1, 0, 0)), q2(DegreesToRadians(v_i[1]), CRCore::crVector3(0, 1, 0)), q3(DegreesToRadians(v_i[2]), CRCore::crVector3(0, 0, 1));
//					CRCore::crMatrix mrot;
//					mrot.set(q1*q2*q3);
//					viewMatterObject->setMatrix(mrot);
//
//					//crQuat orientation;
//					//orientation.set(v_i[0],v_i[1],v_i[2],v_i[3]);
//					//CRCore::crMatrix mrot(orientation);
//					//viewMatterObject->setMatrix(mrot);
//				}
//				if(cfg_script.Get("Position", v_i))
//					viewMatterObject->setPosition(crVector3f(v_i[0],v_i[1],v_i[2]));
//				//if(cfg_script.Get("Mass", flt1))
//				//	viewMatterObject->setMass(flt1);
//
//				ref_ptr<crGameBodyInfo> gameBodyInfo;
//				ref_ptr<crData> data;
//				if(cfg_script.Push("GameBodyInfo"))
//				{
//					cfg_script.Get("Type", str);
//					if(str.compare("GameBodyInfo") == 0)
//					{
//						gameBodyInfo = new crGameBodyInfo;
//					}
//					int nParam = 1;
//					while (cfg_script.Get("Param", str,nParam++))
//					{
//						gameBodyInfo->addParam(nParam-2,str);
//					}
//					if(isNetBot) gameBodyInfo->setTeam(netTeam);
//
//					if(cfg_script.Push("Data"))
//					{
//						cfg_script.Get("Class", str);
//						data = dynamic_cast<crData *>(crDataManager::getInstance()->getData(str));
//						if(!data.valid()) 
//						{
//							CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadSaveCfg: 未找到DataClass: "<<str<<std::endl;
//						}
//						else
//						{
//							nParam = 1;
//							while (cfg_script.Get("Param", str,nParam++))
//							{
//								data->addParam(nParam-2,str);
//							}
//
//							loadDrivers(cfg_script,data.get());
//							gameBodyInfo->setDataClass(data.get());
//						}
//						if (!cfg_script.Pop())
//							CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
//					}
//
//					viewMatterObject->setGameBodyInfo(gameBodyInfo.get());
//					if (!cfg_script.Pop())
//						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
//				}
//
//				int nMatterObjectEvent = 1;
//				int nEvent = 1;
//				int nEventParam = 1;
//				unsigned int typeParam1,typeParam2;
//				ref_ptr<crEventCallback> event;
//				while (cfg_script.Push("PhysicsObjectEvent", nMatterObjectEvent++)) 
//				{
//					cfg_script.Get("TypeParam", str,1);
//					typeParam1 = parseEventMsg(str);
//					cfg_script.Get("TypeParam", str,2);
//					typeParam2 = parseEventMsg(str);
//
//					nEvent = 1;
//					while (cfg_script.Push("Event", nEvent++))
//					{
//						cfg_script.Get("EventFunction", str);
//						event = crEventCallbackManager::getInstance()->getEventCallback(str);
//						if(!event) 
//						{
//							CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadCfgFile(): 未找到EventFunction: "<<str<<std::endl;
//							continue;
//						}
//						nEventParam = 1;
//						while (cfg_script.Get("EventParam", str,nEventParam++)) 
//						{
//							event->addEventParam(nEventParam-2,str);
//						}
//
//						if (!cfg_script.Pop())
//							CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
//						viewMatterObject->insertEvent(MAKEINT64(typeParam1,typeParam2),event.get());
//					}
//					if (!cfg_script.Pop())
//						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
//				}
//				///load data handle
//				//ref_ptr<crData> data;
//				//int nParam;
//				if(cfg_script.Push("Data"))
//				{
//					cfg_script.Get("Class", str);
//					data = crDataManager::getInstance()->getData(str);
//					if(!data.valid())
//					{
//						CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadNodeEventCfg(): 未找到EventFunction: "<<str<<std::endl;
//					}
//					else
//					{
//						nParam = 1;
//						while (cfg_script.Get("Param", str,nParam++)) 
//						{
//							data->addParam(nParam-2,str);
//						}
//
//						loadDrivers(cfg_script,data.get());
//						viewMatterObject->setDataClass(data.get());
//					}
//					if (!cfg_script.Pop())
//						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNodeEventCfg(): "<<cfg_script.GetLastError()<<std::endl;
//				}
//				//if(!isNetBot)
//				//{
//				//	if(cfg_script.Get("CameraBot", int1) && int1)
//				//	{
//				//		CRPhysics::crBotManager::getInstance()->setFirstPlayer(viewMatterObject.get());
//				//	}
//				//	else
//				//	{
//				//		CRPhysics::crBotManager::getInstance()->addBot(viewMatterObject.get());
//				//	}
//				//}
//			}
//			if (!cfg_script.Pop())
//				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotCfg(): "<<cfg_script.GetLastError()<<std::endl;
//		}
//
//		if(cfg_script.Get("createAi", str))
//		{
//			CREncapsulation::loadAI(str,*mg,NULL);
//		}
//
//		if(cfg_script.Get("InsertIn", str))
//		{
//			searchByNameVisitor.reset();
//			searchByNameVisitor.setNameId(str);
//			searchByNameVisitor.setSearchNodeType(crSearchNodeBYNameVisitor::GROUP);
//			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
//			parent = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
//			if(!parent)
//			{
//				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotCfg(): Parent not found "<<std::endl;
//				if (!cfg_script.Pop())
//					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotCfg(): "<<cfg_script.GetLastError()<<std::endl;
//				continue;
//			}
//			if(radarMg.valid()) CRIOManager::crLoadManager::getInstance()->requestAddNode(parent.get(),radarMg.get(),true);
//			//parent->addChild(mg.get());
//			CRIOManager::crLoadManager::getInstance()->requestAddNode(parent.get(),mg.get(),true);
//			//CRIOManager::crLoadManager::getInstance()->requestAddNode(parent.get(),radarMg.get(),true);
//			//parent->addChild(mg.get());
//		}
//		else if(botParent)
//		{
//			//botParent->addChild(mg.get());
//			if(radarMg.valid()) CRIOManager::crLoadManager::getInstance()->requestAddNode(parent.get(),radarMg.get(),true);
//			CRIOManager::crLoadManager::getInstance()->requestAddNode(botParent,mg.get(),true);
//		}
//		else
//		{
//		    CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotCfg(): BotParent not found "<<std::endl;
//		}
//		if (!cfg_script.Pop())
//			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotCfg(): "<<cfg_script.GetLastError()<<std::endl;
//	}
//
//	int nNeutrals = 1;
//	crVector3 pos;
//	while(cfg_script.Get("StaticNeutral",v_i,nNeutrals++))
//	{
//		pos.set(v_i[0],v_i[1],v_i[2]);
//		CRPhysics::crBotManager::getInstance()->addNeutralStaticPositions(pos);
//	}
//	if(sceneName)
//	{
//		if (!cfg_script.Pop())
//			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotCfg(): "<<cfg_script.GetLastError()<<std::endl;
//	}
//
//	cfg_script.Pop();
//
//	return viewMatterObject.get();
//}
//
//void CREncapsulation::loadBotIDCfg(const char* fileName)
//{
//	rcfg::ConfigScript cfg_script;
//
//	struct stat buf;
//	std::string file = fileName?fileName:"script/bot.cfg";
//	if (stat( file.c_str(), &buf )) 
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotCfg(): file open error "<<file<<std::endl;
//		return;
//	}
//
//	if (!cfg_script.Open(file.c_str()))
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotCfg(): "<<cfg_script.GetLastError()<<std::endl;
//		return;
//	}
//	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());
//
//	std::string str;
//	int id;
//	int nID = 1;
//	int nBotName;
//	while(cfg_script.Push("IDBot",nID++))
//	{
//		cfg_script.Get("ID", id);
//		nBotName = 1;
//		while(cfg_script.Get("BotName",str,nBotName++))
//		{
//			crBotManager::getInstance()->addIDBot(id,str);
//			//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotIDCfg(): id = "<<id<<" str = "<<str<<std::endl;
//		}
//		if (!cfg_script.Pop())
//			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadBotIDCfg(): "<<cfg_script.GetLastError()<<std::endl;
//	}
//    cfg_script.Pop();
//}
//
void CREncapsulation::loadCursorCfg(CRProducer::crViewer *viewer,const std::string &path)
{
	std::string filename;
	if(path.empty())
		filename = "script/cursor.cfg";
	else
		filename = path + "/script/cursor.cfg";
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
	{
		CRCore::notify(CRCore::FATAL)<<"loadCursorCfg(): file open error,"<<filename<<std::endl;
		return;
	}

	std::string name,fileName;
	int nCursor = 1;
	HCURSOR cursor;
	while(cfg_script.Push("Cursor",nCursor++))
	{
		cfg_script.Get("Name", name);
		cfg_script.Get("FileName", fileName);

		cursor = (HCURSOR)LoadImage(GetModuleHandle(NULL),fileName.c_str(),IMAGE_CURSOR,0,0,LR_LOADFROMFILE);
        viewer->insertCursor(name,cursor);

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCursorCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}
}

//void CREncapsulation::loadPagedLodCfg(CRCore::crPagedLOD *pagedLOD, rcfg::ConfigScript& cfg_script)
//{
//	//rcfg::ConfigScript cfg_script;
//
//	if(!cfg_script.IsOpen())
//	{
//		std::string pagefile ;
//		char *ptr;
//		if( (ptr = getenv( "CRE_RunMode" )) && strcmp(ptr,"Editor")==0 )
//		{
//            pagefile = "script/editorpage.cfg";
//		}
//		else pagefile = "script/pagedLod.cfg";
//
//		struct stat buf;
//		if (stat( pagefile.c_str(), &buf )) 
//		{
//			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadPagedLodCfg(): file open error"<<std::endl;
//			return;
//		}
//
//		if (!cfg_script.Open(pagefile.c_str()))
//		{
//			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadPagedLodCfg(): "<<cfg_script.GetLastError()<<std::endl;
//			return;
//		}
//	}
//
//	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());
//
//	std::string str;
//	float flt1;
//	int nScene = 1;
//    std::vector<float> v_i;
//	if(cfg_script.Push(pagedLOD->getName()))
//	{
//		if(cfg_script.Get("Center", v_i))
//		{
//			pagedLOD->setCenter(crVector3(v_i[0],v_i[1],v_i[2]));
//		}
//		if(cfg_script.Get("Radius", flt1))
//		{
//			pagedLOD->setRadius(flt1);
//		}
//		if(cfg_script.Get("RangeMode", str))
//		{
//			if(str.compare("DISTANCE_FROM_EYE_POINT") == 0)
//			{
//				pagedLOD->setRangeMode(crPagedLOD::DISTANCE_FROM_EYE_POINT);
//			}
//			else if(str.compare("PIXEL_SIZE_ON_SCREEN") == 0)
//			{
//				pagedLOD->setRangeMode(crPagedLOD::PIXEL_SIZE_ON_SCREEN);
//			}
//		}
//		while(cfg_script.Push("Scene",nScene))
//		{
//			if(cfg_script.Get("FileName", str))
//                pagedLOD->setFileName(nScene-1,str);
//			if(cfg_script.Get("Range", v_i))
//				pagedLOD->setRange(nScene-1,v_i[0],v_i[1]);
//			if(cfg_script.Get("TimeStamp", flt1))
//				pagedLOD->setTimeStamp(nScene-1,flt1);
//			if(cfg_script.Get("PriorityOffset", flt1))
//				pagedLOD->setPriorityOffset(nScene-1,flt1);
//			if(cfg_script.Get("PriorityScale", flt1))
//				pagedLOD->setPriorityScale(nScene-1,flt1);
//
//			if (!cfg_script.Pop())
//				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadShaderManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
//
//			nScene++;
//		}
//
//		if (!cfg_script.Pop())
//			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadShaderManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
//	}
//	cfg_script.Pop();
//}

crNeedInitLoadPageVisitor::crNeedInitLoadPageVisitor():
    CRCore::crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN)
{
}

crNeedInitLoadPageVisitor::~crNeedInitLoadPageVisitor()
{
}

void crNeedInitLoadPageVisitor::apply(CRCore::crPagedLOD& node)
{
	ref_ptr<crNode> loadedScene;
	CRIOManager::crDatabasePager* dp = CRIOManager::crRegistry::instance()->getOrCreateDatabasePager();
	CRIOManager::crDatabasePager::LoadedMap &loadedMap = dp->getLoadedMap();
	CRIOManager::crDatabasePager::LoadedMap::iterator loadedMapItr;
	if(CRCore::crBrain::getInstance()->checkPageNeedInitLoad(node.getName()))
	{
		for(int i=0; i<(int)node.getNumFileNames();++i)
		{
			loadedMapItr = loadedMap.find(node.getFileName(i));
			if(loadedMapItr == loadedMap.end())
			{
				loadedScene = CRIOManager::readNodeFile(node.getFileName(i));
				node.addChild(loadedScene.get());
				loadedMap[node.getFileName(i)] = loadedScene.get();
			}
			else node.addChild(loadedMap[node.getFileName(i)].get());
		}
	}
	traverse(node);
}

//void CREncapsulation::loadImageStreamManagerCfg(const std::string &path)
//{
//	rcfg::ConfigScript cfg_script;
//
//	std::string filename;
//	if(path.empty())
//		filename = "script/ImageStreamManager.cfg";
//	else
//		filename = path + "/script/ImageStreamManager.cfg";
//	CRIOManager::crLoadCookFile scopedLoad(filename);
//
//	struct stat buf;
//	if (stat( filename.c_str(), &buf )) 
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadImageStreamManagerCfg(): file open error"<<std::endl;
//		return;
//	}
//
//	if (!cfg_script.Open(filename))
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadImageStreamManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
//		return;
//	}
//	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());
//
//	std::string str;
//	float flt1;
//	int int1;
//	int nImage = 1;
//	ref_ptr<crImage> image;
//	while(cfg_script.Get("Image",str,nImage++))
//	{
//		image = CRIOManager::readImageFile(str);
//		if(image.valid())
//		    crImageStreamManager::getInstance()->insertImage(image.get());
//	}
//
//	int nStream = 1;
//	int nKeyFrame = 1;
//	int width,height;
//	ref_ptr<crImageStream> imageStream;
//	while(cfg_script.Push("Stream",nStream++))
//	{
//		imageStream = new crImageStream;
//		cfg_script.Get("Name", str);
//		imageStream->setName(str);
//		cfg_script.Get("Width", width);
//		cfg_script.Get("Height", height);
//        imageStream->setStreamSize(width,height);
//
//		if(cfg_script.Get("Looping", int1))
//			imageStream->setLooping(int1);
//		
//		if(cfg_script.Get("SoundStream", str))
//			imageStream->setSoundStream(str);
//
//		cfg_script.Get("FrameLength", flt1);
//		imageStream->setFrameLength(flt1);
//        
//		nKeyFrame = 1;
//		while (cfg_script.Push("KeyFrame",nKeyFrame++))
//		{
//			cfg_script.Get("FrameTime", flt1);
//			cfg_script.Get("ID", str);
//	        imageStream->insertKeyFrame(flt1,str);
//			if (!cfg_script.Pop())
//				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadImageStreamManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
//		}
//
//		crImageStreamManager::getInstance()->insertImageStream(imageStream.get());
//		if (!cfg_script.Pop())
//			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadImageStreamManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
//	}
//    cfg_script.Pop();
//}
//
void CREncapsulation::loadLoadManagerCfg(const std::string &path)
{
	std::string filename;
	if(path.empty())
		filename = "script/LoadManager.cfg";
	else
		filename = path + "/script/LoadManager.cfg";
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
	{
		CRCore::notify(CRCore::FATAL)<<"loadLoadManagerCfg(): file open error,"<<filename<<std::endl;
		return;
	}

	std::string str,str1;
	float flt1;
	int nModel = 1;
	int nComponent = 1;
	int nParam = 1;
	std::vector<float> v_i;
	ref_ptr<CRIOManager::crCreBotModel> creBot;
	ref_ptr<CRCore::crTransform> transform;
	CRCore::crVector3 centerPos;
	while(cfg_script.Push("CreBotModel",nModel++))
	{
		creBot = new CRIOManager::crCreBotModel;
		cfg_script.Get("ID", str);
		creBot->setName(str);
		if(cfg_script.Get("Mass", flt1))
			creBot->setMass(flt1);
        nComponent = 1;
		while (cfg_script.Push("Component",nComponent++))
		{
			cfg_script.Get("File", str);
			transform = NULL;
			if(cfg_script.Push("Matrix"))
			{
				if(cfg_script.Get("Type", str1))
				{
                    if(str1.compare("DOF") == 0)
					{
						transform = new CRCore::crDOFTransform;
					}
					
					nParam = 1;
					while (cfg_script.Get("Param", str,nParam++))
					{
						transform->addParam(nParam-2,str);
					}
				}
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadLoadManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
			}
			
			creBot->insertComponent(str,transform.get());
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadLoadManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
		}

		CRIOManager::crLoadManager::getInstance()->insertCreBotModel(creBot.get());
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadLoadManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}
}

void CREncapsulation::loadAITaskManagerCfg(const std::string &path)
{
	std::string filename;
	if(path.empty())
		filename = "script/AITaskManager.cfg";
	else
		filename = path + "/script/AITaskManager.cfg";
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
	{
		CRCore::notify(CRCore::FATAL)<<"loadAITaskManagerCfg(): file open error,"<<filename<<std::endl;
		return;
	}

	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());

	std::string str;
    std::vector<float> v_i;
	float flt1;
	float probability;
	int int1;
	int nTask = 1;
	int nCP = 1;
	int nPMsg = 1;
	ref_ptr<CRCore::crAITask> aiTask;
	ref_ptr<CRCore::crControlPoint> cp;
	CRCore::crProbabilityMsgControlPoint *pmcp;
    CRCore::crOrdinalMsgControlPoint *omcp;
	CRCore::Msg msg;
	_crInt32 param32[2] = {0,0};
	_crInt16 param16[4] = {0,0,0,0};
	while(cfg_script.Push("AITask",nTask++))
	{
		aiTask = new CRCore::crAITask;
		cfg_script.Get("Name", str);
		aiTask->setName(str);
		if(cfg_script.Get("LoopMode", str))
		{
            if(str.compare("SWING") == 0)
			{
				aiTask->setLoopMode(crAITask::SWING);
			}
			else if(str.compare("LOOP") == 0)
			{
				aiTask->setLoopMode(crAITask::LOOP);
			}
			else if(str.compare("NO_LOOPING") == 0)
			{
				aiTask->setLoopMode(crAITask::NO_LOOPING);
			}
			else if(str.compare("RANDOM") == 0)
			{
				aiTask->setLoopMode(crAITask::RANDOM);
			}
		}
		
		nCP = 1;
		while (cfg_script.Push("ControlPoint",nCP++))
		{
			cfg_script.Get("Type", str);
			if(str.compare("CP") == 0)
        		cp = new crControlPoint;
			else if(str.compare("PMCP") == 0)
				cp = new crProbabilityMsgControlPoint;
			else if(str.compare("OMCP") == 0)
				cp = new crOrdinalMsgControlPoint;

			cfg_script.Get("Position", v_i);
            cp->setPosition(crVector3f(v_i[0],v_i[1],v_i[2]));

			if(cfg_script.Get("Orientation", v_i))
			    cp->setOrientation(crVector3f(v_i[0],v_i[1],v_i[2]));

			if(cfg_script.Get("Velocity", v_i))
			    cp->setVelocity(crVector3f(v_i[0],v_i[1],v_i[2]));
			if(cfg_script.Get("NeedVelocity", int1))
				cp->setNeedVelocity(int1);
			if(cfg_script.Get("IgnoreZ", int1))
				cp->setIgnoreZ(int1);

			nPMsg = 1;
			while(cfg_script.Push("ProbabilityMsg",nPMsg++))
			{
				memset(param32,0,8);
				memset(param16,0,8);
				if(cfg_script.Get("MsgParam", str,1))
				{
					param32[0] = parseEventMsg(str);
				}
				if(cfg_script.Get("MsgParam", str,2))
				{
					param32[1] = parseEventMsg(str);
				}
				for(int i = 1; i<5; i++)
				{
					if(cfg_script.Get("MsgParam16", str,i))
						param16[i-1] = parseEventMsg16(str);
				}
				if(param32[0]==0)
				{
					param32[0] = MAKEINT32(param16[0],param16[1]);
				}
				if(param32[1]==0)
				{
					param32[1] = MAKEINT32(param16[2],param16[3]);
				}
				msg.m_msg = MAKEINT64(param32[0],param32[1]);

				memset(param32,0,8);
				memset(param16,0,8);
				if(cfg_script.Get("Param", str,1))
				{
					param32[0] = parseEventMsg(str);
				}
				if(cfg_script.Get("Param", str,2))
				{
					param32[1] = parseEventMsg(str);
				}
				for(int i = 1; i<5; i++)
				{
					if(cfg_script.Get("Param16", str,i))
						param16[i-1] = parseEventMsg16(str);
				}
				if(param32[0]==0)
				{
					param32[0] = MAKEINT32(param16[0],param16[1]);
				}
				if(param32[1]==0)
				{
					param32[1] = MAKEINT32(param16[2],param16[3]);
				}
				msg.m_param = MAKEINT64(param32[0],param32[1]);

				cfg_script.Get("Probability", probability);
				cfg_script.Get("Interval", flt1);
				msg.m_interval = flt1;
				pmcp = (dynamic_cast<crProbabilityMsgControlPoint *>(cp.get()));
				if(pmcp) pmcp->addMsgProbability(msg,probability);
				else CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadAITaskManagerCfg(): Type error"<<std::endl;

				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadAITaskManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
			}

			nPMsg = 1;
			while(cfg_script.Push("OrdinalMsg",nPMsg++))
			{
				memset(param32,0,8);
				memset(param16,0,8);
				if(cfg_script.Get("MsgParam", str,1))
				{
					param32[0] = parseEventMsg(str);
				}
				if(cfg_script.Get("MsgParam", str,2))
				{
					param32[1] = parseEventMsg(str);
				}
				for(int i = 1; i<5; i++)
				{
					if(cfg_script.Get("MsgParam16", str,i))
						param16[i-1] = parseEventMsg16(str);
				}
				if(param32[0]==0)
				{
					param32[0] = MAKEINT32(param16[0],param16[1]);
				}
				if(param32[1]==0)
				{
					param32[1] = MAKEINT32(param16[2],param16[3]);
				}
				msg.m_msg = MAKEINT64(param32[0],param32[1]);

				memset(param32,0,8);
				memset(param16,0,8);
				if(cfg_script.Get("Param", str,1))
				{
					param32[0] = parseEventMsg(str);
				}
				if(cfg_script.Get("Param", str,2))
				{
					param32[1] = parseEventMsg(str);
				}
				for(int i = 1; i<5; i++)
				{
					if(cfg_script.Get("Param16", str,i))
						param16[i-1] = parseEventMsg16(str);
				}
				if(param32[0]==0)
				{
					param32[0] = MAKEINT32(param16[0],param16[1]);
				}
				if(param32[1]==0)
				{
					param32[1] = MAKEINT32(param16[2],param16[3]);
				}
				msg.m_param = MAKEINT64(param32[0],param32[1]);

				cfg_script.Get("Probability", probability);
				cfg_script.Get("Interval", flt1);
				msg.m_interval = flt1;
				omcp = dynamic_cast<crOrdinalMsgControlPoint *>(cp.get());
				if(omcp) omcp->addOrdinalMsg(msg);
				else CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadAITaskManagerCfg(): Type error"<<std::endl;

				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadAITaskManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
			}
			aiTask->pushTaskPoint(cp.get());
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadAITaskManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
		}

		crAITaskManager::getInstance()->insertAITask(aiTask.get());
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadAITaskManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}
    cfg_script.Pop();
}

void CREncapsulation::loadSaveCfg(const std::string &path)
{
	std::string filename;
	if(path.empty())
		filename = "script/save.cfg";
	else
		filename = path + "/script/save.cfg";
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
	{
		CRCore::notify(CRCore::FATAL)<<"loadSaveCfg(): file open error,"<<filename<<std::endl;
		return;
	}

	std::string str;
	int nSave = 1;
	CRCore::ref_ptr<crSave> save;
	//ref_ptr<crData> dataClass;
    int nParam = 1;
	while(cfg_script.Push("Save",nSave++))
	{
		save = new crSave;
        nParam = 1;
		while (cfg_script.Get("Param", str,nParam++))
		{
			save->addParam(nParam-2,str);
		}

		//if(cfg_script.Push("DataClass"))
		//{
		//	cfg_script.Get("Class", str);
		//	dataClass = dynamic_cast<crData *>(crDataManager::getInstance()->getData(str));
		//	if(!dataClass.valid()) 
		//	{
		//		CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadSaveCfg: 未找到DataClass: "<<str<<std::endl;
		//		continue;
		//	}
		//	if(dataClass.valid())
		//	{
		//		nParam = 1;
		//		while (cfg_script.Get("DataParam", str,nParam++))
		//		{
		//			dataClass->addParam(nParam-2,str);
		//		}

		//		save->setDataClass(dataClass.get());
		//	}
		//	else
		//	{
		//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSaveCfg(): not find DataClass of "<<str<<std::endl;
		//	}
		//}

		//cfg_script.Get("Name", str);
		//save->setName(str);
		//cfg_script.Get("SceneID", int1);
		//save->m_sceneid = int1;
		//cfg_script.Get("SaveTime", str);
		//save->m_saveTime = str;

		crSceneManager::getInstance()->pushSave(save.get());
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSaveCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}
}

bool CREncapsulation::wantToLoadSave(int index)
{
    crSceneManager::getInstance()->setCurrentSaveIndex(index);
	crSave *save = crSceneManager::getInstance()->getSave(index);
	if(save)
	{
        //loadSceneManagerCfg(save,false);
		return true;
	}
	return false;
}

bool CREncapsulation::loadSave(crSave *save)
{
    if(save) 
	{
		//crViewMatterObject *bot = loadBotCfg(NULL,CRCore::crBrain::getInstance()->getCurrentActiveDB(),save->getBotFile().c_str(),save->getName().c_str());
		CRCore::ref_ptr<crNPC> npc = new crNPC;
		npc->setName(save->getName());
		npc->setScriptFileName(save->getBotFile());
		loadNPCCfg(npc->getName(),npc->getScriptFileName(),npc.get());
		crSceneManager::getInstance()->setPlayer(npc->getNPCBot());
	}
	//	CRPhysics::crMatterGroup *bodyMG = dynamic_cast<CRPhysics::crMatterGroup *>(npc->getNPCBody());
	//	if(!bodyMG)
	//	{
	//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSave(): cameraBot的父节点类型不正确"<<std::endl;
	//		return false;
	//	}
	//	crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(bodyMG->getChild(0));
	//	if(!bot)
	//	{
	//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSave(): error"<<std::endl;
	//		return false;
	//	}
	//	CRAI::crNerveCellGroup *bodyNC = dynamic_cast<CRAI::crNerveCellGroup *>(bodyMG->getAttachedEventGroup());
	//	if(!bodyNC)
	//	{
	//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSave(): bodyMG的AttachedEventGroup类型不正确"<<std::endl;
	//		return false;
	//	}
	//	CRAI::crAIGroup *ai = bodyNC->getAI();
	//	if(!ai)
	//	{
	//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSave(): 智能节点不正确"<<std::endl;
	//		return false;
	//	}

	//	ref_ptr<CRPhysics::crWeaponMatterObject> weaponMatterObject;
	//	CRPhysics::crWeaponManager *weaponManager = CRPhysics::crWeaponManager::getInstance();
	//	const std::string &weapon1 = save->getWeapon1();
	//	//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSave(): weapon1 = "<<weapon1<<std::endl;
	//	if(weapon1.compare("NULL") != 0)
	//	{
	//		ai->addKeyEvent("3",weapon1);
	//		weaponMatterObject = weaponManager->getWeaponMatterObject(weapon1);
	//		if(weaponMatterObject.valid() && !bot->testToAddWeaponMatterObject(weaponMatterObject.get()))
	//		{
	//			weaponMatterObject = CRPhysics::crWeaponManager::createWeaponMatterObject(weaponMatterObject.get());
	//			weaponMatterObject->setName(weapon1);
	//			bot->addWeaponMatterObject(weaponMatterObject.get());
	//		}
	//	}
	//	const std::string &weapon2 = save->getWeapon2();
	//	//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSave(): weapon2 = "<<weapon2<<std::endl;
	//	if(weapon2.compare("NULL") != 0)
	//	{
	//		ai->addKeyEvent("4",weapon2);
	//		weaponMatterObject = weaponManager->getWeaponMatterObject(weapon2);
	//		if(weaponMatterObject.valid() && !bot->testToAddWeaponMatterObject(weaponMatterObject.get()))
	//		{
	//			weaponMatterObject = CRPhysics::crWeaponManager::createWeaponMatterObject(weaponMatterObject.get());
	//			weaponMatterObject->setName(weapon2);
	//			bot->addWeaponMatterObject(weaponMatterObject.get());
	//		}
	//	}
	//	const std::string &weapon3 = save->getWeapon3();
	//	//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSave(): weapon3 = "<<weapon3<<std::endl;
	//	if(weapon3.compare("NULL") != 0)
	//	{
	//		ai->addKeyEvent("5",weapon3);
	//		weaponMatterObject = weaponManager->getWeaponMatterObject(weapon3);
	//		if(weaponMatterObject.valid() && !bot->testToAddWeaponMatterObject(weaponMatterObject.get()))
	//		{
	//			weaponMatterObject = CRPhysics::crWeaponManager::createWeaponMatterObject(weaponMatterObject.get());
	//			weaponMatterObject->setName(weapon3);
	//			bot->addWeaponMatterObject(weaponMatterObject.get());
	//		}
	//	}

	//	//bot->setName(save->getName());
	//	//bodyMG->setName(save->getName());
	//	//ai->setName(save->getName());

	//	crSave::GameState gameState = save->getGameState();
	//	//crFlyGameViewFilterCallback ViewFilterCallback;
	//	crFlyGameViewFilterCallback *vf = dynamic_cast<crFlyGameViewFilterCallback *>(bot->getCullCallback("FlyGameViewFilter"));
	//	if(gameState == crSave::SingleInit)
	//	{
	//		save->setGameState(crSave::SingleRunning);
 //           bodyNC->acceptEventMessage(WCH_AIMOVIES,MAKEINT64(WCH_AIMOVBEGIN,0),MAKEINT64(NULL,NULL));//0:init movetask index
 //           bot->setVisiable(false);
	//		if(vf) vf->setShow(2);
	//		save->setProgress(0);
	//	}
	//	else if(gameState == crSave::NetInit)
	//	{
	//		save->setGameState(crSave::NetRunning);
	//		bodyNC->acceptEventMessage(WCH_AIMOVIES,MAKEINT64(WCH_AIMOVBEGIN,3),MAKEINT64(NULL,NULL));//
	//		bot->setVisiable(false);
	//		if(vf) vf->setShow(2);
	//		save->setProgress(0);
	//	}
	//	else if(gameState == crSave::SingleRunning)
	//	{
	//		int hp = save->getHp();
	//		hp = hp<=0?1:hp;
	//		bot->getGameBodyInfo()->setHP(hp);
	//		bot->getGameBodyInfo()->setDefend(save->getDefend());
	//		bot->getGameBodyInfo()->setTeam(save->getTeam());
	//		bot->getGameBodyInfo()->setExperience(save->getExperience());

	//		//const crQuat &orientation = save->getOrientation();
	//		////CRCore::crQuat q1(DegreesToRadians(orientation[0]), crVector3(1, 0, 0)), q2(DegreesToRadians(orientation[1]), crVector3(0, 1, 0)), q3(DegreesToRadians(orientation[2]), crVector3(0, 0, 1));
	//		//CRCore::crMatrix mrot;
	//		//mrot.set(orientation/*q1*q2*q3*/);
	//		//bot->setMatrix(mrot);

	//		//CRCore::notify(CRCore::ALWAYS)<<"orientation = "<<orientation<<std::endl;
	//		//CRCore::notify(CRCore::ALWAYS)<<"mrot = "<<mrot<<std::endl;
	//		//crQuat o;
	//		//o.set(mrot);
	//		////float x,y,z;
	//		////o.getAngles(x,y,z);
	//		//CRCore::notify(CRCore::ALWAYS)<<"o = "<<o/*crVector3f(x,z,y)*/<<std::endl;
	//		const crVector3 &position = save->getPosition();
	//		bot->setPosition(position/*+crVector3(0.0f,0.0f,1.5f)*/);
	//		bodyNC->acceptEventMessage(WCH_AIMOVIES,MAKEINT64(WCH_AIMOVBEGIN,0),MAKEINT64(1,NULL));//0:init movetask index
	//		//bodyNC->acceptEventMessage(WCH_AIMOVIES,MAKEINT64(WCH_BOTCONTROLLER,WCH_LOADPROGRESS),MAKEINT64(NULL,NULL));

	//		bot->setVisiable(false);
	//		if(vf) vf->setShow(2);

	//		//CRCore::notify(CRCore::ALWAYS)<<"SingleRunning getProgress = "<<save->getProgress()<<std::endl;

	//		//bot->setHideBody(false);
	//		//bot->changeCamera(0);
	//		//if(vf) vf->setShow(1);
	//		//bot->setVisiable(false);
	//		//if(vf) vf->setShow(2);
	//		//save->setProgress(0);
	//	}
	//	return true;
	//}
    return false;
}

//void CREncapsulation::writeSaveCfg(int index)
//{
//	crSave *save = CRCore::crSceneManager::getInstance()->getSave(index);
//	if(save) 
//	{
//		std::vector<float> v_i;
//
//		crViewMatterObject *bodyVO = CRPhysics::crSceneManager::getInstance()->getPlayer()->getNPCBot();
//        crGameBodyInfo *bodyInfo = bodyVO->getGameBodyInfo();
//		crMatterGroup *bodyMG = dynamic_cast<crMatterGroup *>(bodyVO->getParent(0));
//		save->m_sceneid = crSceneManager::getInstance()->getCurrentSceneIndex();//bodyInfo->getSceneID();
//        save->m_saveTime = "time";
//
//		rcfg::ConfigScript cfg_script;
//		cfg_script.Add("Bot");
//		cfg_script.Push("Bot");
//		cfg_script.Add("Name",bodyVO->getName());
//		cfg_script.Add("matterGroupMaterialString",bodyMG->getMatterGroupMaterialString());
//		crEventGroup::EventMsgMap &EMM = bodyMG->getEventMsgMap();
//		for( crEventGroup::EventMsgMap::iterator itr = EMM.begin();
//			itr != EMM.end();
//			++itr )
//		{
//			cfg_script.Add("PPC",itr->second->m_event->getParseParamCallback()->className());
//		}
//		cfg_script.Add("InsertIn",bodyMG->getParent(0)->getName());
//		
//		cfg_script.Add("Body");
//		cfg_script.Push("Body");
//		cfg_script.Add("Type",bodyVO->className());
//		cfg_script.Add("CreBotModel",bodyVO->getBotModelName());
//		cfg_script.Add("ViewObjectCollide",bodyVO->getCollideCallback()->className());
//		
//		cfg_script.Add("WeaponEquip");
//		cfg_script.Push("WeaponEquip");
//		crViewMatterObject::WeaponMatterObjectMap &weapons = bodyVO->getWeaponMap();
//		for( crViewMatterObject::WeaponMatterObjectMap::iterator itr = weapons.begin();
//			itr != weapons.end();
//			++itr )
//		{
//			cfg_script.Add("Weapon",itr->first);
//			cfg_script.Add("BulletCount",itr->second->getBulletCount());
//			cfg_script.Add("BulletsInWeapon",itr->second->getBulletsInWeapon());
//			cfg_script.Add("IsCurrentWeaponMatterObject",itr->second == bodyVO->getCurrentWeaponMatterObject()?1:0);
//		}
//		cfg_script.Pop();
//
//		cfg_script.Add("HideBody",bodyVO->getHideBody());
//        v_i.clear();
//		v_i.push_back(bodyVO->getMaxSpeed()[0]);
//		v_i.push_back(bodyVO->getMaxSpeed()[1]);
//		cfg_script.Add("MaxSpeed",v_i);
//		v_i.clear();
//		v_i.push_back(bodyVO->getMinSpeed()[0]);
//		v_i.push_back(bodyVO->getMinSpeed()[1]);
//		cfg_script.Add("MinSpeed",v_i);
//		v_i.clear();
//		v_i.push_back(bodyVO->getVelocity()[0]);
//		v_i.push_back(bodyVO->getVelocity()[1]);
//		cfg_script.Add("Velocity",v_i);
//		cfg_script.Add("ViewOffset",bodyVO->getViewOffset());
//		cfg_script.Add("ViewHeight",bodyVO->getViewHeight());
//		cfg_script.Add("GroundDetect",bodyVO->getGroundDetect());
//		
//		crQuat orientation;
//		orientation.set(bodyVO->getMatrix());
//		float x,y,z;
//        orientation.getAngles(x,y,z);
//		v_i.clear();
//		v_i.push_back(x);
//		v_i.push_back(y);
//		v_i.push_back(z);
//		cfg_script.Add("Orientation",v_i);
//		v_i.clear();
//		v_i.push_back(bodyVO->getMatrix().getTrans()[0]);
//		v_i.push_back(bodyVO->getMatrix().getTrans()[1]);
//		v_i.push_back(bodyVO->getMatrix().getTrans()[2]);
//		cfg_script.Add("Position",v_i);
//
//		cfg_script.Add("GameBodyInfo");
//		cfg_script.Push("GameBodyInfo");
//		cfg_script.Add("Type","GameBodyInfo");
//		char tmpText[15];
//		//sprintf(tmpText,"%d\0",bodyInfo->getSceneID());
//		//cfg_script.Add("Param",tmpText);
//		sprintf(tmpText,"%d\0",bodyInfo->getHP());
//        cfg_script.Add("Param",tmpText);
//		sprintf(tmpText,"%d\0",bodyInfo->getDefend());
//		cfg_script.Add("Param",tmpText);
//		sprintf(tmpText,"%d\0",bodyInfo->getTeam());
//		cfg_script.Add("Param",tmpText);
//		sprintf(tmpText,"%d\0",bodyInfo->getExperience());
//		cfg_script.Add("Param",tmpText);
//		sprintf(tmpText,"%d\0",bodyInfo->getMaxDetectCount());
//		cfg_script.Add("Param",tmpText);
//		sprintf(tmpText,"%f\0",bodyInfo->getMaxDetectRange());
//		cfg_script.Add("Param",tmpText);
//		sprintf(tmpText,"%f\0",bodyInfo->getTaskPointRange());
//		cfg_script.Add("Param",tmpText);
//		cfg_script.Pop();
//
//	    cfg_script.Add("CameraBot",1);
//		cfg_script.Pop();
//
//		cfg_script.Add("createAi","script/aircraftbotai0.cfg");
//        cfg_script.Pop();
//
//		cfg_script.Write(save->getBotFile());
//	}
//	
//	rcfg::ConfigScript cfg_script;
//	crSceneManager::SaveVec &saveVec = crSceneManager::getInstance()->getSaveVec();
//	for( crSceneManager::SaveVec::iterator itr = saveVec.begin();
//		itr != saveVec.end();
//		++itr )
//	{
//		cfg_script.Add("Save");
//		cfg_script.Push("Save");
//		cfg_script.Add("Name",(*itr)->getName());
//		cfg_script.Add("SceneID",(*itr)->m_sceneid);
//		cfg_script.Add("SaveTime",(*itr)->m_saveTime);
//		cfg_script.Pop();
//	}
//	cfg_script.Write("save/save.cfg");
//}
//
void CREncapsulation::writeSaveCfg(const std::string &path)
{//////需要cook
	ItemMap itemMap;
	rcfg::ConfigScript cfg_script(&itemMap);
	crSceneManager *sceneManager = crSceneManager::getInstance();
	crSceneManager::SaveVec &saveVec = sceneManager->getSaveVec();
	crSave *save;
	char tmpText[256];
	int i = 1;
	int hp;
	crSave::GameState gameState;
	int saveCount = 0;
	for( crSceneManager::SaveVec::iterator itr = saveVec.begin();
		 itr != saveVec.end() && saveCount<10;
		 ++itr, saveCount++)
	{
		save = itr->get();
		gameState = save->getGameState();
		if(gameState<crSave::NetInit)
		{
			cfg_script.Add("Save");
			cfg_script.Push("Save",i++);
			cfg_script.Add("Param",save->getName());
			cfg_script.Add("Param",save->getBotName());
			sprintf(tmpText,"%d\0",save->getSceneID());
			cfg_script.Add("Param",tmpText);
			cfg_script.Add("Param",save->getSaveTime());
			hp = save->getHp();
			hp = hp<=0?1:hp;
			sprintf(tmpText,"%d\0",hp);
			cfg_script.Add("Param",tmpText);
			sprintf(tmpText,"%d\0",save->getDefend());
			cfg_script.Add("Param",tmpText);
			sprintf(tmpText,"%d\0",save->getTeam());
			cfg_script.Add("Param",tmpText);
			sprintf(tmpText,"%d\0",save->getExperience());
			cfg_script.Add("Param",tmpText);
			const crQuat &orientation = save->getOrientation();
			sprintf(tmpText,"VEC4 %f, %f, %f, %f\0",orientation[0],orientation[1],orientation[2],orientation[3]);
			cfg_script.Add("Param",tmpText);
			const crVector3 &position = save->getPosition();
			sprintf(tmpText,"VEC3 %f, %f, %f\0",position[0],position[1],position[2]);
			cfg_script.Add("Param",tmpText);
			cfg_script.Add("Param",save->getWeapon1());
			cfg_script.Add("Param",save->getWeapon2());
			cfg_script.Add("Param",save->getWeapon3());
			sprintf(tmpText,"%d\0",save->getProgress());
			cfg_script.Add("Param",tmpText);
			sprintf(tmpText,"%d\0",(int)gameState);
			cfg_script.Add("Param",tmpText);
			cfg_script.Pop();
		}
	}
	std::string filename;
	if(path.empty())
		filename = "script/save.cfg";
	else
		filename = path + "/script/save.cfg";
	cfg_script.Write(filename);
	cfg_script.Close();

	CRIOManager::crWriteCookFile scopedWrite(filename);

}
//void CREncapsulation::deleteSaveCfg(int index)
//{
//	//delete
//	crSave *save = CRCore::crSceneManager::getInstance()->getSave(index);
//	if(save) 
//	{
//        DeleteFile(save->getBotFile().c_str());
//        crSceneManager::getInstance()->deleteSave(index);
//	}
//}
//
//
//////////////////////CheckSN
std::string basecode = "A0BD1JCF2L4RNETS3XW8VZ6Y7QGKU9MP5";
bool CREncapsulation::incorrectRegistrationCode(std::string &code)
{
	char abc[4];
	char end;
	int i[4];
	int iabc[3];
	int j,k;
	for(j = 0; j<3; j++)
	{
		for(k = 0; k<4; k++)
		{
			abc[k] = code[(j*6+k)];
			i[k] = basecode.find(abc[k]);
			if(i[k] == -1) return false;
		}

		iabc[j] = (i[0]+i[1]+i[2]+i[3])%21;
		if(j == 0)
		{    
			end = code[4];
			if(code[5] != '-') 
				return false;
		}
		else if(j == 1)
		{
			end = code[10];
			if(code[11] != '-') 
				return false;
		}
		else if(j == 2)
		{
			end = code[16];
			if(code[17] != '-') 
				return false;
		}
		if(iabc[j]!=basecode.find(end))
			return false;
	}


	int iend[5];
	iend[0] = iabc[0] + iabc[1] + iabc[2];
	iend[1] = iabc[1] + iabc[2] + iend[0];
	iend[2] = iabc[2] + iend[0] + iend[1];
	iend[3] = iend[0] + iend[1] + iend[2];
	iend[4] = iend[1] + iend[2] + iend[3];

	for(j = 0; j<5; j++)
	{
		if(iend[j] % 21 != basecode.find(code[18+j]))
			return false;
	}

	return true;
}

//void CREncapsulation::registerDefUIEventCallback()
//{
//	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crPlayUISoundCallback);
//	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crCloseDlgCallback);
//	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crResumeCallback);
//	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crExitCallback);
//	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crApplyCallback);
//	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crCancelCallback);
//	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crShowStageCallback);
//	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crScrollBarInitCallback);
//	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crScrollBarUpdateCallback);
//	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crListBoxImageUpdateCallback);
//	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crMultiSwitchListBoxUpdateCallback);
//	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crListBoxImageInitCallback);
//	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crCheckBoxPlayPreDrawCallback);
//	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crButtonBoxNextPageCallback);
//	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crButtonBoxPrePageCallback);
//	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crLoadingProgressInitCallback);
//	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crLoadingProgressCallback);
//	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crListBoxImageUpdateExCallback);
//	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crListBoxTextUpdateExCallback);
//	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crValidEditStrCallback);
//
//	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crHideCursorCallback);
//}
//
void CREncapsulation::registerEventCallback()
{
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRCore::crAnimationPathCallback);

	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crGndReflectMaterialCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crMirrorCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crSetEndCodeCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crNodeEventCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crFilterNodeEventCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crPitchDofCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crRollDofCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crAircraftSequenceCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crAircraftWeaponSequenceCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crAircraftWeaponSequenceExCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crZTransDofCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crYTransDofCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crStopDofCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crXTransTMDofCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crYTransTMDofCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crParticleMEventCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crSequenceMEventCallback);

	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crDeadCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crPlayMoviesCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crCameraBotMoviesCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crPlaySequenceCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crPlaySoundCallback);

	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crCharacterActionCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crGndActionEffectCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crStopActionCallback);
	//
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crFpsCharacterUpdateCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crFpsCharacterStateChangeCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crFpsCharacterJumpCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crFpsCharacterMoveChangeCallback);

	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crDialogTalkCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crCloseAllDialogCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crShowManipulatorDialogsCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crShowMouseCallback);

	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crEffectParticleCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crEffectSequenceCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crEffectLightCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crEffectSoundCallback);

	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crPlayUISoundCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crCloseDlgCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crResumeCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crExitCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crApplyCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crCancelCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crShowStageCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crScrollBarInitCallback);
	////CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crScrollBarUpdateCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crListBoxImageUpdateCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crMultiSwitchListBoxUpdateCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crListBoxImageInitCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crCheckBoxPlayPreDrawCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crButtonBoxNextPageCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crButtonBoxPrePageCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crLoadingProgressInitCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crLoadingProgressCallback);

 //   CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crResolutionComboInitCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crBrightnessSliderInitCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crTextureDetailComboInitCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crTextureAnisotropyComboInitCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crShadowDetailComboInitCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crAlphaShadowComboInitCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crMotionBlurComboInitCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crHDRComboInitCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crDepthOfFieldComboInitCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crAntialiasingComboInitCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crNPRComboInitCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crResetDisplaySettingCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crUpdateInitCfgCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crSaveListInitCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crSaveListUpdateCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crSceneMapInitCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crSceneInfoInitCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crSceneListUpdateCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crAircraftListInitCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crWeaponListInitCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crListBoxImageUpdateExCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crListBoxTextUpdateExCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crAircraftListUpdateCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crInitGameCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crCreateSaveCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crDeleteSaveCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crValidEditStrCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crSaveCallback);

	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crHideCursorCallback);

	////CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crExpandCallback);
	////CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crContractCallback);
	////CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crSelectNodeCallback);
	////CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crMoveingNodeCallback);
	////CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crEndMoveNodeCallback);
	////CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crBuildSceneGraphCallback);
	////CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crEditTreePreDrawCallback);
	////CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crBeginRectSelectCallback);
	////CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crResizeRectSelectCallback);
	////CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CRGUI::crEndRectSelectCallback);

	//////////////////Register NodeCallbacks
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crCreBodyCullCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crDuplicateCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crSunInEyesightCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crSunFlaresCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crMoveBillboardWithEyeCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crDofTexMatCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crDofUniformTexMatCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crUniformParam1SinCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crUniformParam1CosCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crUniformParam1RndCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crUniformParam3RndCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crUniformParam4RndCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crParticleBindCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crSequenceBindCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crVisableTimerCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crUnloadTimerCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crFlashTimerCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crBoneMatrixCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crCompassHCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crCompassPCallback);

	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crCompassRCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crScalarBarCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crFlyGameViewFilterCallback);
	
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crLightLumRandomCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crLightPositionRandomCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crRenascenceCallback);

	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crLightEnableTimerCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crSunSimulationCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crTerrainDecalCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crSeqTextureTileCallback);

	//////////////////Register CollideCallbacks
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crCommonMatterObjectCollideCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crBulletCollideCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crWeaponCollideCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crCaissonCollideCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crHpCollideCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crArmorCollideCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crViewObjectCollideCallback);
	//CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crAircraftCollideCallback);
	CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crGroundBoxCollideCallback);
    CRCore::crEventCallbackManager::RegisterEventCallbackProxy(new CREncapsulation::crMouseManipularCollideCallback);
}

void CREncapsulation::registerDataClass()
{
	//CRCore::crDataManager::RegisterDataProxy(new CREncapsulation::crTestData);
}

void CREncapsulation::registerHandleClass()
{
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crExcEventContainerMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crAITaskMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crInputAITaskMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDoAITaskMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crGetRadarInfoMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crAttachNodeUpdateMethod);
	//DisplaySetting Method
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDSFullScreenInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDSVsyncInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDSShaderInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDSResolutionComboInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDSBrightnessSliderInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDSMouseSpeedSliderInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDSTextureDetailComboInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDSTextureAnisotropyComboInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDSShadowDetailComboInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDSAlphaShadowComboInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDSMotionBlurComboInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDSHDRComboInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDSDepthOfFieldComboInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDSAntialiasingComboInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDSResetDisplaySettingMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDSUpdateInitCfgMethod);
	///Editor Method
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crEditorMoveCameraMethod);
	//////////MethodClass
	//CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crPlayAmbSoundMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crCloseDlgMethod);
    CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crHideDlgMethod);
    CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crCloseStageMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crPauseMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crResumeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crExitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crHideCursorMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crShowStageMethod);
    CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crCloseAllModelessStageMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crRedrawStageMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crUpdateAndRedrawStageMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crInitStageMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crScrollBarInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crMultiSwitchListBoxUpdateMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crListBoxFileInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crListBoxTextUpdateExMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crListBoxImageInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crListBoxImageUpdateMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crListBoxImageUpdateExMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crCheckBoxPlayPreDrawMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crButtonBoxNextPageMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crButtonBoxPrePageMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crLoadingProgressInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crClearStaticTextMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crClearEditTextMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSaveListInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSaveListUpdateMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crValidEditStrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crCreateSaveMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDeleteSaveMethod);
	//CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSaveMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSceneMapInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSceneInfoInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSceneListUpdateMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crAircraftListInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crAircraftListUpdateMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crWeaponListInitMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crInitGameMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crPreviewImageMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crAddListDataMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crInitEditorMethod);
	//CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetRunModeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crAddModelMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crAddUIModelMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crReloadUIModelMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crLoadUISceneMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSaveUISceneMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crCreateModelMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crInitSceneNameMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSaveSceneMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSaveScene2Method);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crUpdateSelectNodeListMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDisplayNodeAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifyNodeAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crShowScaleInfoMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crShowBoundLengthMethod);
	//CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSaveNodeAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crInitSelectTypeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crUpdateSelectTypeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetSelectTypeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crLoadDesMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crInitTransformNodeModeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crInitTransformNodeSpeedMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetTransformNodeModeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crInitPanNodeModeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetPanNodeModeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crWalkPanNodeModeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crInitRotNodeModeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetRotNodeModeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crWalkRotNodeModeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crInitScaleNodeModeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetScaleNodeModeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crWalkScaleNodeModeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetTransformNodeSpeedMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crResetMatrixLogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crNewLightMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDeleteLightMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDeleteParticleMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crUpdateLightListMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crUpdateParticleListMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSelectEditorNodeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSelectEditorNodeExMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDisplayLightAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifyLightAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crChooseColorMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crUpdateElementMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetChooseColorMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetComponentColorMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDisplayCharacterAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDoAnimMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifyCharacterAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crAddToListMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crListDeleteCurrentDataMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crUpdateObjectAvoidLightListMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crUpdateObjectNeedLightListMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDisplayDrawableMaterialMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crFocusTextureAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crGetImageNameMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifyMaterialMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifyObjectDrawableMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSaveNodeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSnapMaterialMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crPickNeedLightMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crCheckBoxSetSelectMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crCheckBoxSetSelectExMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crGetSaveFileNameMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDisplayVolumeAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifyVolumeAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDisplayMoviesAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crAddKeyFrameMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifyMoviesAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDelKeyFrameMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetAnimationPauseMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crResetAnimationMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSaveMoviesAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crCreateMoviePathMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifyKeyFrameMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSelectKeyFrameMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crExcElementHandleMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crInitAnimationModeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crMovePathMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crRadioGroupSelectMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifyGridMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crBuildTerrainMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifyTerrainMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crShowTerrainGridMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crHideAllTerrainsGridMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDisplayTerrainAttrMethod);

	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDisplayPostProcessAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetEnableIntersectMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetGroundDetectMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifyPostProcessAttrMethod);

	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDisplayMultiSwitchAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetActiveSwitchSetMethod);
    CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crMultiSwitchPreMaskMethod);
    CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crMultiSwitchNextMaskMethod);
    CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crMultiSwitchAllOnMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crMultiSwitchAllOffMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crPickChildMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crAddChildMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crRemoveChildMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crRemovePagedLodChildMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifyMultiSwitchAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crAddSwitchSetMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDelSwitchSetMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crUpdateNodeListMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSelectNodeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDisplayLodAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifyLodAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDisplayPagedLodAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifyPagedLodAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crFullLoadPagedLodMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crHiddenOrUnHiddenNodes2Method);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetLayeredWindowAttributesMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crCombineObjectsMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crCombineDrawablesMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crBuildNodeTreeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSeletNodeByNodeTreeMethod);
	
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crInitTerraformModeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetTerraformModeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crInitCurrentTerToolMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetCurrentTerToolMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetTerEditParamMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDisplayDofParamMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifyDofParamMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crFocusDofTexMatMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSaveDofTexMatMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crNextDofTexMatMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crShowEditorMenuMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crNewParticleMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDisplayParticleParamMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifyParticleParamMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crAddParticleMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDelParticleMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crNextParticleMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crCopyParticleMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crPastParticleMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crResetParticleIndexMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crElementDoEventMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSelectParticleGroupMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSelectParticleEffectMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDisplaySequenceAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifySequenceAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSelectNodeIsObjectMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSaveSequenceMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crEditChangeCameraMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crEditReturnCameraMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crFreezeModelMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crFreezeModel2Method);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDisconnectDrawableMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crRunSingleGameMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDisplayDuplicateAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifyDuplicateAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSaveDuplicateMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crUpdateNodeList2Method);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSelectNode2Method);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDisplayInitSettingMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifyInitSettingMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSaveInitSettingMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDisplayBrainAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifyBrainAttrMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crGetModKeyMaskMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSceneModifiedMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crNodeUpdateMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crAutoSaveSceneMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crUnSelectAllMethod);
	//////////MethodClass2
    CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crPitchDofMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crRollDofMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crZTransDofMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crYTransDofMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crStopDofMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crAircraftSequenceMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crAircraftWeaponSequenceMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crAircraftWeaponSequenceExMethod);
	//CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDeadMethod);
	//CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crPlayMoviesMethod);
	//CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crCameraBotMoviesMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crPlaySequenceMethod);
	//CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crPlaySoundMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crCharacterActionMethod);
	//CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crGndActionEffectMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crStopActionMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crFpsCharacterUpdateMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crFpsCharacterStateChangeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crFpsCharacterJumpMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crFpsCharacterMoveChangeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crRpgCharacterUpdateMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crRpgCharacterStateChangeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crRpgCharacterJumpMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crRpgCharacterMoveChangeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDialogTalkMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crCloseAllDialogMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crShowManipulatorDialogsMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crFilterSwitchMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crShowFilterNodeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crNodeSwitchMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSwitchActiveMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crFilterNodeEventMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crNodeEventMethod);
	//CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetEndCodeMethod);
	//CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crEffectParticleMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crEffectSequenceMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crEffectLightMethod);
	//CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crEffectSoundMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crXTransTMDofMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crYTransTMDofMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crMoveMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crPanMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crRaiseMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crRotateMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crPointSelectMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crShowCursorMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crMouseControlViewMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetEditorCoordinateMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crConvertToTransformMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crPanNodeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crRotNodeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crScaleNodeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSpawnNodeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDeleteNodeMethod);
    CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crAdjustEditorNodeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crFocusNodeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSelectNodeMaterialMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crHiddenOrUnHiddenNodesMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crHideNodeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crUnHideAllMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crShowAllUnRenderableNodeMethod);
    //CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crLightListSelectMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crNodeListSelectMethod);
	//CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crPlaceLoadedNodeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crEnablePhysicsMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crRemoveMatrixMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crFreezeMatrixMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crInsertNodeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crRemoveNodeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crShowCameraPosMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crPickTargetMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crMouseMoveCursorMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crTurnToTargetMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crMoveToTargetMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crBotAISelectTargetMethod);
	//CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crBotAIPatrolMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crShowBotHeadInfoMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crChangeViewOffsetMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crChangeMoveSpeedMethod);
	//CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crBotAIAvoidCollideMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crTalkWithBotMethod);
	//CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crTalkEndMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDoEventMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crTargetDoEventMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crMudererDoEventMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crReviveMosterMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crIsStageOnShowMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crRackCharacterMeshMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crLoadAnimMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSelectNodeTypeCaseMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crTerrainEditMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crColorWeightEditMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crTerrainEditDecalMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSaveHeightMapMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSaveColorWeightMapMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crShowTerrainEditDecalMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crHideTerrainEditDecalMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crMoveWidgetMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crMoveParentCanvasMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crCloseParentCanvasMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crCloseMeMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crWidgetShowMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crShowCanvasMethod);

	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crWidgetUpdateMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crWidgetUpdate2Method);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crWidgetUpdate3Method);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crCanvasUpdateMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crParentCanvasUpdateMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crUpdateMeMethod);
	
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crShowMouseCanvasMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crChangeMouseMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crAddTabWidgetMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetMsgTraverseMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crWidgetDoEventMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crWidgetDoEvent2Method);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crCanvasDoEventMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crParentCanvasDoEventMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crChangeCharacterSettingMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSetWidgetEnableMethod);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crIsRenderRunningMethod);
    //////////////LogicClass
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crTransformNodeLogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crPlaceLoadedNodeLogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crSpawnSelectNodesLogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crCenterModelLogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crDeleteSelectNodesLogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crLoadModelLogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crGetListBoxStrLogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crNewEditorModelLogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crFocusSelectNodeLogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crInitLightEditorModelLogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crInitParticleEditorModelLogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crGetModelFileNameLogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crModifyDrawableMaterialLogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crProxyElementLogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crMousePickMoveLogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crRpgFireAndFollowLogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crBotAILogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crPlayerAILogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crNetPlayerAILogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crShowTalkStageLogic);
	CRCore::crHandleManager::RegisterHandleProxy(new CREncapsulation::crUpdateModelLogic);
	//////////////DataClass
    CRCore::crDataManager::RegisterDataProxy(new CREncapsulation::crEditorInfoData);
}

bool CREncapsulation::registrationCodeInput(std::string computerName, std::string userName)
{
	CRGUI::crImageStage *stage = CRGUI::cr2DStageManager::getInstance()->findStage("SN");
	CRGUI::crEditBoxElement *snEditBox = dynamic_cast<CRGUI::crEditBoxElement *>(stage->getElement("SnEditBox"));
	std::ofstream fout;
	fout.open("script/key.cfg", std::ios::binary);
	if(fout.is_open() && stage && snEditBox)
	{
		std::string sn;
		while(CRGUI::cr2DStageManager::getInstance()->doModal(stage).compare("OK") == 0)
		{
			sn = snEditBox->getStringArrayInString();
			if(incorrectRegistrationCode(sn))
			{
				int length = computerName.length();
				char *text = (char*)malloc(length);
				strcpy(text,computerName.c_str());
				for(int i = 0; i<length; i++)
				{
					ENDIAN(text[i]);
				}
				fout.write(text,length);
				free(text);

				fout.write("\n",1);

				length = userName.length();
				text = (char*)malloc(length);
				strcpy(text,userName.c_str());
				for(int i = 0; i<length; i++)
				{
					ENDIAN(text[i]);
				}
				fout.write(text,length);
				free(text);

				fout.write("\n",1);

				length = sn.length();
				text = (char*)malloc(length);
				strcpy(text,sn.c_str());
				for(int i = 0; i<length; i++)
				{
					ENDIAN(text[i]);
				}
				fout.write(text,length);
				free(text);

				fout.close();
				return true;
			}

		}
	}
	if(fout.is_open()) fout.close();
	return false;
}

bool CREncapsulation::checkSN()
{
	const int bufsize = 1024;
	LPTSTR lpszSystemInfo;
	DWORD cchBuff = bufsize;
	TCHAR _computername[bufsize];
	TCHAR _username[bufsize];
	//TCHAR _temppath[bufsize];
	//DWORD dwResult;
	lpszSystemInfo = _computername; 
	if( !GetComputerName(lpszSystemInfo, &cchBuff) ) 
		CRCore::notify(CRCore::FATAL)<<"main(): get computer name error"<<std::endl;
	cchBuff = bufsize;
	lpszSystemInfo = _username; 
	if( !GetUserName(lpszSystemInfo, &cchBuff) )
		CRCore::notify(CRCore::FATAL)<<"main(): get user name error"<<std::endl;
	//lpszSystemInfo = _temppath; 
	//dwResult = ExpandEnvironmentStrings( 
	//	"TEMP=%TEMP%",
	//	lpszSystemInfo, 
	//	bufsize);
	//if( dwResult > bufsize )
	//	CRCore::notify(CRCore::FATAL)<<"main(): get TMP environment error"<<std::endl;

	std::string computerName = _computername;
	std::string userName = _username;
	//CRCore::notify(CRCore::FATAL)<<"main():computerName = "<<computerName<<std::endl;
	//CRCore::notify(CRCore::FATAL)<<"main():userName = "<<userName<<std::endl;

	std::ifstream fin;
	fin.open("script/key.cfg");
	if(!fin.is_open())
	{
		if(!registrationCodeInput(computerName,userName))
		{
            return false;
		}
	}
	else
	{
		fin.seekg(0, std::ios::end);
		int length = fin.tellg();
		char *text = new char[length+1];
		fin.seekg(0, std::ios::beg);
		fin.read(text, length);
		fin.close();
		text[length] = '\0';
		int _itext[2] = {0,0};
		int j = 0;
		for(int i = 0; i<length; i++)
		{
			if(text[i]!='\n')
				ENDIAN(text[i]);
			else if(j<2) 
			{
				text[i] = '\0';
				_itext[j++] = i+1;
			}
		}

		//CRCore::notify(CRCore::FATAL)<<"main():_itext[0] = "<<_itext[0]<<std::endl;
		//CRCore::notify(CRCore::FATAL)<<"main():_itext[1] = "<<_itext[1]<<std::endl;

		bool same = computerName.compare(text) == 0;
		if(same) same = userName.compare(text+_itext[0]) == 0;

		//CRCore::notify(CRCore::FATAL)<<"main():computerName = "<<computerName<<std::endl;
		//CRCore::notify(CRCore::FATAL)<<"main():userName = "<<userName<<std::endl;
		//CRCore::notify(CRCore::FATAL)<<"main():computerName2 = "<<text<<std::endl;
		//CRCore::notify(CRCore::FATAL)<<"main():userName2= "<<text+_itext[0]<<std::endl;


		std::string code = text+_itext[1];
		//CRCore::notify(CRCore::FATAL)<<"main():code= "<<code<<std::endl;
		free(text);

		if(!same || !CREncapsulation::incorrectRegistrationCode(code))
		{
			if(!CREncapsulation::registrationCodeInput(computerName,userName))
			{
                return false;
			}
		}
	}
	
	return true;
}

CRCore::crMatrixTransform *CREncapsulation::createCoordinate()
{
	crObject* object = new crObject;
	crGeometry* geom = new crGeometry;
	crStateSet *stateset = geom->getOrCreateStateSet(); 
	stateset->setMode(GL_LIGHTING,crStateAttribute::OFF);
	crDepth *depth = new crDepth(crDepth::ALWAYS,0.0,1.0,true);
	stateset->setAttributeAndModes(depth,crStateAttribute::ON);
	crLineWidth *lineWidth = new crLineWidth(2.0f);
	stateset->setAttributeAndModes(lineWidth,crStateAttribute::ON);
	//crPoint *point = new crPoint;
	//point->setSize(5.0f);
	//stateset->setAttributeAndModes(point,crStateAttribute::ON);
	//stateset->setRenderBinDetails(100,"StateSortedBin");
	stateset->setRenderingHint(crStateSet::GRIDDRAW_BIN);
	crVector3 origin;
	Vec3Array* coords = new Vec3Array(22);
	(*coords)[0] = origin;
	(*coords)[1] = X_AXIS;
	(*coords)[2] = crVector3( 0.9, 0.0, 0.1);
	(*coords)[3] = crVector3( 1.1, 0.0,-0.1);
	(*coords)[4] = crVector3( 0.9, 0.0,-0.1);
	(*coords)[5] = crVector3( 1.1, 0.0, 0.1);

	(*coords)[6] = origin;
	(*coords)[7] = Y_AXIS;
	(*coords)[8] = crVector3(-0.1, 1.0, 0.1);
	(*coords)[9] = crVector3( 0.0, 1.0, 0.0);
	(*coords)[10] = crVector3( 0.1, 1.0, 0.1);
	(*coords)[11] = crVector3( 0.0, 1.0, 0.0);
	(*coords)[12] = crVector3( 0.0, 1.0, 0.0);
	(*coords)[13] = crVector3( 0.0, 1.0,-0.1);

	(*coords)[14] = origin;
	(*coords)[15] = Z_AXIS;
	(*coords)[16] = crVector3(-0.1, 0.0, 1.1);
	(*coords)[17] = crVector3( 0.1, 0.0, 1.1);
	(*coords)[18] = crVector3( 0.1, 0.0, 1.1);
	(*coords)[19] = crVector3(-0.1, 0.0, 0.9);
	(*coords)[20] = crVector3(-0.1, 0.0, 0.9);
	(*coords)[21] = crVector3( 0.1, 0.0, 0.9);
	geom->setVertexArray(coords);

	CRCore::Vec4Array* colours = new CRCore::Vec4Array(22);
	(*colours)[0].set(1.0f,0.0f,0.0,1.0f);
	(*colours)[1].set(1.0f,0.0f,0.0,1.0f);
	(*colours)[2].set(1.0f,0.0f,0.0,1.0f);
	(*colours)[3].set(1.0f,0.0f,0.0,1.0f);
	(*colours)[4].set(1.0f,0.0f,0.0,1.0f);
	(*colours)[5].set(1.0f,0.0f,0.0,1.0f);

	(*colours)[6].set(0.0f,1.0f,0.0,1.0f);
	(*colours)[7].set(0.0f,1.0f,0.0,1.0f);
	(*colours)[8].set(0.0f,1.0f,0.0,1.0f);
	(*colours)[9].set(0.0f,1.0f,0.0,1.0f);
	(*colours)[10].set(0.0f,1.0f,0.0,1.0f);
	(*colours)[11].set(0.0f,1.0f,0.0,1.0f);
	(*colours)[12].set(0.0f,1.0f,0.0,1.0f);
	(*colours)[13].set(0.0f,1.0f,0.0,1.0f);

	(*colours)[14].set(0.0f,0.0f,1.0,1.0f);
	(*colours)[15].set(0.0f,0.0f,1.0,1.0f);
	(*colours)[16].set(0.0f,0.0f,1.0,1.0f);
	(*colours)[17].set(0.0f,0.0f,1.0,1.0f);
	(*colours)[18].set(0.0f,0.0f,1.0,1.0f);
	(*colours)[19].set(0.0f,0.0f,1.0,1.0f);
	(*colours)[20].set(0.0f,0.0f,1.0,1.0f);
	(*colours)[21].set(0.0f,0.0f,1.0,1.0f);
	geom->setColorArray(colours);
	geom->setColorBinding(crGeometry::BIND_PER_VERTEX);

	geom->setNormalBinding(crGeometry::BIND_OFF);
	geom->addPrimitive(new DrawArrays(crPrimitive::PT_LINES,0,22));

	object->addDrawable(geom);
	object->setCollideMode(crNode::CollideNone);
	object->setName("EditorAxis");
	//object->setEnableIntersect(false);
	crMatrixTransform *matrixTransform = new crMatrixTransform;
	matrixTransform->addChild(object);
	matrixTransform->setName("EditorAxis");
	return matrixTransform;
}

CRCore::crObject *CREncapsulation::createGrid(int rows, float interval, float zoffset)
{
	crObject* object = new crObject;
	crHeightField *heightField = new crHeightField;
	crShapeDrawable *gridshape = new crShapeDrawable(heightField);
	//crTessellationHints *hint = new crTessellationHints;
	//hint->setCreateBackFace(true);
	//hint->setCreateFrontFace(false);
	//hint->setCreateNormals(false);
	//hint->setDetailRatio(0.5);
	//gridshape->setTessellationHints(hint);
	gridshape->setColor(crVector4(0.7f,0.7f,0.7f,1.0f));
	heightField->allocate(rows+1,rows+1);
	heightField->setXInterval(interval);
	heightField->setYInterval(interval);
	float size = rows * interval;
	crVector3 origin(-size*0.5f,-size*0.5f,zoffset);
	heightField->setOrigin(origin);

	crStateSet *stateset = gridshape->getOrCreateStateSet();
	stateset->setRenderingHint(crStateSet::GRIDDRAW_BIN);
	stateset->setMode(GL_LIGHTING,crStateAttribute::OFF);
	//crDepth *depth = new crDepth(crDepth::ALWAYS,0.0,1.0,true);
	//stateset->setAttributeAndModes(depth,crStateAttribute::ON);
	crLineWidth *lineWidth = new crLineWidth(1.0f);
	stateset->setAttributeAndModes(lineWidth,crStateAttribute::ON);
	crPolygonMode *polygonMode = new crPolygonMode;
	polygonMode->setMode(crPolygonMode::FRONT_AND_BACK,crPolygonMode::LINE);
	stateset->setAttributeAndModes(polygonMode,crStateAttribute::ON);
	stateset->setMode(GL_CULL_FACE,crStateAttribute::OFF);
	//stateset->setRenderingHint(crStateSet::TRANSPARENT_BIN);

	//crObject* object2 = new crObject;
	crGeometry* geom = new crGeometry;
	Vec3Array* coords = new Vec3Array(4);
	(*coords)[0] = crVector3( -size*0.5f, 0.0, zoffset);
	(*coords)[1] = crVector3( size*0.5f, 0.0, zoffset);
	(*coords)[2] = crVector3( 0.0f, -size*0.5f, zoffset);
	(*coords)[3] = crVector3( 0.0f, size*0.5f, zoffset);
	geom->setVertexArray(coords);
	CRCore::Vec4Array* colours = new CRCore::Vec4Array(4);
	(*colours)[0].set(1.0f,0.0f,0.0,1.0f);
	(*colours)[1].set(1.0f,0.0f,0.0,1.0f);
	(*colours)[2].set(0.0f,1.0f,0.0,1.0f);
	(*colours)[3].set(0.0f,1.0f,0.0,1.0f);
	geom->setColorArray(colours);
	geom->setColorBinding(crGeometry::BIND_PER_VERTEX);
	geom->setNormalBinding(crGeometry::BIND_OFF);
	geom->addPrimitive(new DrawArrays(crPrimitive::PT_LINES,0,4));

	stateset = geom->getOrCreateStateSet(); 
	stateset->setMode(GL_LIGHTING,crStateAttribute::OFF);
	crDepth *depth = new crDepth(crDepth::ALWAYS,0.0,1.0,true);
	stateset->setAttributeAndModes(depth,crStateAttribute::ON);
	lineWidth = new crLineWidth(2.0f);
	stateset->setAttributeAndModes(lineWidth,crStateAttribute::ON);
	stateset->setRenderingHint(crStateSet::TRANSPARENT_BIN);

	object->addDrawable(gridshape);
	object->addDrawable(geom);
	object->setCollideMode(crNode::CollideNone);
	object->setName("ViewGrid");
	object->setEditorIntersectable(false);

	return object;
}

void CREncapsulation::loadSceneManagerCfg(crSave *save,bool loadDetail)
{
	rcfg::ConfigScript cfg_script;
	std::string filename = "script/SceneManager.cfg";
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
	{
		CRCore::notify(CRCore::FATAL)<<"loadSceneManagerCfg(): file open error,"<<filename<<std::endl;
		return;
	}

	int nSceneMap = 1;
	int nMapArea = 1;
	int nNPCTroop = 1;
	int nNPC = 1;
	std::string str;
	int int1;
	std::vector<float> v_i;
	ref_ptr<crScene> scene;
	ref_ptr<crSceneMap> sceneMap;
	ref_ptr<crMapArea> mapArea;
	ref_ptr<crNPC> npc;
	ref_ptr<crNPCTroop> npcTroop;
	//ref_ptr<crNPCQueryData> npcQueryData;
	//ref_ptr<crNPCQueryData> queryResult;
	//crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
	crSceneManager *sceneManager = crSceneManager::getInstance();
	if(cfg_script.Push("Scene",save->getSceneID()+1))
	{
		if(!loadDetail)
		{
			scene = new crScene;
			cfg_script.Get("SceneID", str);
			scene->setSceneNameID(str);
			cfg_script.Get("SceneFile", str);
			scene->setSceneFileName(str);
		}
		else
		{
			scene = sceneManager->getCurrentScene();
			if(!scene)
			{
				scene = new crScene;
				cfg_script.Get("SceneID", str);
				scene->setSceneNameID(str);
				cfg_script.Get("SceneFile", str);
				scene->setSceneFileName(str);
			}

			nSceneMap = 1;
			sceneMap = NULL;
			while(cfg_script.Push("SceneMap",nSceneMap++))
			{
				//if(cfg_script.Get("File",str))
				//{
				//	sceneMap = dynamic_cast<crSceneMap *>(CRIOManager::readObjectFile(str));
				//	if(sceneMap.valid())
				//	{
				//		sceneMap->setFileName(str);
				//	}
				//}
				//else
				//{
					sceneMap = new crSceneMap;
				//}
				if(cfg_script.Get("Name", str))
					sceneMap->setName(str);

				nMapArea = 1;
				while(cfg_script.Push("MapArea",nMapArea++))
				{
					mapArea = new crMapArea;
					if(cfg_script.Get("Name", str))
						mapArea->setName(str);
					sceneMap->addMapArea(mapArea.get());
					if(cfg_script.Get("Area",v_i))
					{
						if(v_i.size()==3)
							mapArea->setMapArea(crVector3(v_i[0],v_i[1],v_i[2]));
					}
					nNPCTroop = 1;
					while(cfg_script.Push("NPCTroop",nNPCTroop++))
					{
						npcTroop = new crNPCTroop;
						nNPC = 1;
						while(cfg_script.Get("NPC",int1,nNPC++))
						{
							npc = new crNPC;
							npc->setNPCID(int1);
							///load npc npcid = int1
							//if(db)
							//{//数据库检索登陆账户，如果密码正确，则返回账户信息
							//	npcQueryData = new crNPCQueryData;
							//	npcQueryData->buildQuerySql(int1);

							//	db->executeQuery(npcQueryData.get());
							//	crDataBase::QueryResultVec &queryResultVec = db->getQueryResult();
							//	if(queryResultVec.size()==1)
							//	{
							//		queryResult = dynamic_cast<crNPCQueryData *>(queryResultVec[0].get());
							//		npc->setName(queryResult->getName());
							//		npc->setReviveTaskByName(queryResult->getReviveTaskName());
							//		npc->setScriptFileName(queryResult->getScript());
							//		npc->setMaxRange(queryResult->getMaxRange());
							//	}
							//	db->releaseQuery();
							//}
							//else
							{//load from NPC.cfg
								NPCQueryFromCfg(int1,npc.get());
							}
							loadNPCCfg(npc->getName(),npc->getScriptFileName(),npc.get());
							//scene->getNetSceneRoot()->addChild(npc->getNPCBody());

							npcTroop->addNPC(npc.get());
							//npc->addToScene();
						}
						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNetSceneManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
						mapArea->addNPCTroop(npcTroop.get());
					}
					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNetSceneManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
				}
				scene->addSceneMap(sceneMap.get());
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNetSceneManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
			}
		}

		crSceneManager::getInstance()->setCurrentScene(scene.get());
		//CRCore::crSceneManager::getInstance()->pushSceneID(sceneid);
		//CRCore::crSceneManager::getInstance()->insertSceneMap(sceneid,scenefile);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSceneManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}
}

void CREncapsulation::NPCQueryFromCfg(int npcid,crNPC *npc)
{
	rcfg::ConfigScript cfg_script;
	std::string filename = "script/NPC.cfg";
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
	{
		CRCore::notify(CRCore::FATAL)<<"NPCQueryFromCfg(): file open error,"<<filename<<std::endl;
		return;
	}

	std::string str;
	float flt1;
	if(cfg_script.Push("NPC",npcid+1))
	{
		if(cfg_script.Get("Name", str))
		{
            npc->setName(str);
		}
		if(cfg_script.Get("ReviveTask", str))
		{
			npc->setReviveTaskByName(str);
		}
		if(cfg_script.Get("Script", str))
		{
			npc->setScriptFileName(str);
		}
		if(cfg_script.Get("MaxRange", flt1))
		{
			npc->setMaxRange(flt1);
		}
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSceneManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}
}

void CREncapsulation::loadNPCCfg(const std::string &npcName, const std::string &scriptFile, crNPC *npc,const std::string &botFile)
{
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,scriptFile))
	{
		CRCore::notify(CRCore::FATAL)<<"loadNPCCfg(): file open error,"<<scriptFile<<std::endl;
		return;
	}

	std::string str,str1;
	int int1;
	float flt1;
	std::vector<float> v_i;
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
	CRCore::ref_ptr<CRPhysics::crMatterGroup> mg;
	CRCore::ref_ptr<CRCore::crGroup> externNode;
	CRCore::ref_ptr<rbody::CreBodyNode> creBodyNode;
	CRCore::ref_ptr<CRPhysics::crViewMatterObject> viewMatterObject;
	ref_ptr<CRPhysics::crRadarMatterObject> radar;
	ref_ptr<CRPhysics::crMatterGroup> radarMg;
	ref_ptr<CRPhysics::crWeaponMatterObject> weaponMatterObject;
	//ref_ptr<CRPhysics::crWeaponMatterObject> newWeapon;
	//crGroup *radarDetect;
	int nBot = 1;
	int nDescription = 1;
	int nppc = 1;
	int nParam = 1;
	int nCfg = 1;
	ref_ptr<CRCore::ParseParamCallback> ppc;
	ref_ptr<crEventMsg> emsg;
	CRCore::ref_ptr<crGroup> parent = crSceneManager::getInstance()->getBotRoot();
	if(!parent.valid())
		return;
	while(cfg_script.Push("Bot",nBot++))
	{
		radarMg = NULL;
		mg = new CRPhysics::crMatterGroup;
		npc->setNPCBody(mg.get());
		mg->setDataVariance(CRCore::crBase::DYNAMIC);
		if(!npcName.empty())
			mg->setName(npcName);
		else
		{
			cfg_script.Get("Name", str);
			mg->setName(str);
		}
		nppc = 1;
		while (cfg_script.Push("PPC",nppc++)) 
		{
			cfg_script.Get("Type",str);
			if(str.compare("crGeneralParamTransferPPC")==0)
			{
				emsg = crGeneralParamTransferPPC::makeEventMsg();
				ppc = emsg->m_event->getParseParamCallback();
			}
			else if(str.compare("crWeaponEventPPC")==0)
			{
				emsg = crWeaponEventPPC::makeEventMsg();
				ppc = emsg->m_event->getParseParamCallback();
			}
			else if(str.compare("crWeaponFiringPPC")==0)
			{
				emsg = crWeaponFiringPPC::makeEventMsg();
				ppc = emsg->m_event->getParseParamCallback();
			}
			else if(str.compare("crJointSetParamPPC")==0)
			{
				emsg = crJointSetParamPPC::makeEventMsg();
				ppc = emsg->m_event->getParseParamCallback();
			}
			else if(str.compare("crPhysicsResetPPC")==0)
			{
				emsg = crPhysicsResetPPC::makeEventMsg();
				ppc = emsg->m_event->getParseParamCallback();
			}
			else if(str.compare("crSetForceAndTorquePPC")==0)
			{
				emsg = crSetForceAndTorquePPC::makeEventMsg();
				ppc = emsg->m_event->getParseParamCallback();
			}
			else if(str.compare("crSetGeomorBodyInfoPPC")==0)
			{
				emsg = crSetGeomorBodyInfoPPC::makeEventMsg();
				ppc = emsg->m_event->getParseParamCallback();
			}
			else if(str.compare("crSetGravityModePPC")==0)
			{
				emsg = crSetGravityModePPC::makeEventMsg();
				ppc = emsg->m_event->getParseParamCallback();
			}
			//else if(str.compare("crAsyncDataTransferPPC")==0)
			//{
			//	emsg = crAsyncDataTransferPPC::makeEventMsg();
			//	ppc = emsg->m_event->getParseParamCallback();
			//}

			nParam = 1;
			while (cfg_script.Get("Param", str,nParam++))
			{
				ppc->addParam(nParam-2,str);
			}
			mg->addEventMsgMap(emsg.get());
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"crCreateAiVisitor::loadAI(): "<<cfg_script.GetLastError()<<std::endl;
		}

		if(cfg_script.Get("matterGroupMaterialString", str))
		{
			mg->setMatterGroupMaterialString(str);
		}
		if(cfg_script.Push("Body"))
		{
			cfg_script.Get("Type", str);

			if(str.compare("crViewMatterObject")==0)
			{
				cfg_script.Get("CreBotModel", str);
				viewMatterObject = new crViewMatterObject;
				viewMatterObject->setBotModelName(str);
				//CRCore::notify(CRCore::ALWAYS)<<" setBotModelName "<<str.c_str()<< std::endl;
				CRIOManager::crLoadManager::getInstance()->requestCreBot(str,viewMatterObject.get(),false);
			}
			else if(str.compare("crCharacterMatterObject")==0)
			{
				cfg_script.Get("CreBotModel", str);
				viewMatterObject = new crCharacterMatterObject;
				viewMatterObject->setBotModelName(str);
				if(cfg_script.Get("BlockRadius", flt1))
					(dynamic_cast<crCharacterMatterObject *>(viewMatterObject.get()))->setBlockRadius(flt1);
				//CRCore::notify(CRCore::ALWAYS)<<" setBotModelName "<<str.c_str()<< std::endl;
				CRIOManager::crLoadManager::getInstance()->requestCreBot(str,viewMatterObject.get(),false);
			}
			else if(str.compare("crMissileSetOrFlackMatterObject") == 0)
			{
				cfg_script.Get("CreBotModel", str);
				viewMatterObject = new crMissileSetOrFlackMatterObject;
				viewMatterObject->setBotModelName(str);
				CRIOManager::crLoadManager::getInstance()->requestCreBot(str,viewMatterObject.get(),false);
			}
			else if(str.compare("crBodyCaseMatterObject") == 0)
			{
				cfg_script.Get("CreBotModel", str);
				viewMatterObject = new crBodyCaseMatterObject;
				viewMatterObject->setBotModelName(str);
				CRIOManager::crLoadManager::getInstance()->requestCreBot(str,viewMatterObject.get(),false);
			}
			else if(str.compare("crNetCharacterMatterObject") == 0)
			{
				viewMatterObject = new crCharacterMatterObject;
				viewMatterObject->setBotModelName(npcName);
				if(cfg_script.Get("BlockRadius", flt1))
					(dynamic_cast<crCharacterMatterObject *>(viewMatterObject.get()))->setBlockRadius(flt1);
				//CRCore::notify(CRCore::ALWAYS)<<" setBotModelName "<<str.c_str()<< std::endl;

				CRCore::ref_ptr<crGroup> proxyNode = new crGroup;
				CRIOManager::crLoadManager::getInstance()->requestNode(botFile,proxyNode.get(),false);
				if(proxyNode->getNumChildren()==1)
				{
					crNode *loadedNode = proxyNode->getChild(0);
					loadedNode = dynamic_cast<crNode *>(loadedNode->clone(crCopyOp::DEEP_COPY_NODES|
						crCopyOp::DEEP_COPY_STATESETS|crCopyOp::DEEP_COPY_UNIFORMS|crCopyOp::DEEP_COPY_Callbacks|
						crCopyOp::DEEP_COPY_DATAS));
					viewMatterObject->addChild(loadedNode);
				}
				//CRIOManager::crLoadManager::getInstance()->requestNode(botFile,viewMatterObject.get(),false);
			}
			else if(str.compare("crNetViewMatterObject") == 0)
			{
				viewMatterObject = new crViewMatterObject;
				viewMatterObject->setBotModelName(npcName);

				CRCore::ref_ptr<crGroup> proxyNode = new crGroup;
				CRIOManager::crLoadManager::getInstance()->requestNode(botFile,proxyNode.get(),false);
				if(proxyNode->getNumChildren()==1)
				{
					crNode *loadedNode = proxyNode->getChild(0);
					loadedNode = dynamic_cast<crNode *>(loadedNode->clone(crCopyOp::DEEP_COPY_NODES|
						crCopyOp::DEEP_COPY_STATESETS|crCopyOp::DEEP_COPY_UNIFORMS|crCopyOp::DEEP_COPY_Callbacks|
						crCopyOp::DEEP_COPY_DATAS));
					viewMatterObject->addChild(loadedNode);
				}
				//CRIOManager::crLoadManager::getInstance()->requestNode(botFile,viewMatterObject.get(),false);
			}
			//else if(str.compare("CreBodyNode")==0)
			//{
			//	cfg_script.Get("CharacterBotModel", str);
			//	externNode = dynamic_cast<crGroup *>(CRIOManager::readNodeFile(str));
			//             if(externNode.valid()) creBodyNode = dynamic_cast<rbody::CreBodyNode *>(externNode->getChild(0));
			//	if(!creBodyNode.valid()) return NULL;
			//	externNode->removeChild(creBodyNode.get());
			//	//creBodyNode->setUpdateMode(creBodyNode->getUpdateMode() ^ rbody::CreBodyNode::UPDATE_GROUND_FOLLOWING);
			//	//creBodyNode->setEnableIntersect(false);///角色暂不做物理计算
			//	rbody::ActionRequest *action = creBodyNode->getBody()->getActionPrototype( creBodyNode->getBody()->getCoreBody()->getDefaultActivity() );
			//	if (action) 
			//	{
			//		action->setPrioritized(false);
			//		creBodyNode->getBody()->executeAction(action, false);
			//		creBodyNode->getBody()->update(rvrutils::randInterval(0,2));
			//	}
			//             viewMatterObject = dynamic_cast<CRPhysics::crViewMatterObject *>(creBodyNode.get());
			//	viewMatterObject->setBotModelName(str);
			//}
			if(viewMatterObject.valid())
			{
				viewMatterObject->setVisiable(false);
				viewMatterObject->setEnableIntersect(true);

				if(cfg_script.Get("PhysicsBox", v_i))
				{
					viewMatterObject->setPhysicsBox(crVector3(v_i[0],v_i[1],v_i[2]));
				}
				if(cfg_script.Get("AttributeMask", str))
				{
					unsigned int attrmask = 0;
					if(str.find("AttrAll") != -1)
						attrmask |= CRCore::crGroup::AttrAll;
					if(str.find("AttrGround") != -1)
						attrmask |= CRCore::crGroup::AttrGround;
					if(str.find("AttrAir") != -1)
						attrmask |= CRCore::crGroup::AttrAir;
					if(str.find("AttrDeepSea") != -1)
						attrmask |= CRCore::crGroup::AttrDeepSea;

					viewMatterObject->setAttributeMask(attrmask);
				}
				ref_ptr<crNodeCallback> nodeCallback;
				int nCallback = 1;
				int nParam = 1;
				while(cfg_script.Push("NodeCallback", nCallback++))
				{
					cfg_script.Get("Class", str);
					//if(str.compare("Detect") == 0)
					//{
					//	nodeCallback = new CREncapsulation::crDetectCallback;
					//}
					//else
					//{
						nodeCallback = dynamic_cast<crNodeCallback *>(crEventCallbackManager::getInstance()->getEventCallback(str));
						if(!nodeCallback) 
						{
							CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadCfgFile: 未找到NodeCallback: "<<str<<std::endl;
							continue;
						}
					//}
					//else if(str.compare("FlyGameViewFilter") == 0)
					//{
					//	nodeCallback = new CREncapsulation::crFlyGameViewFilterCallback;
					//}
					//else if(str.compare("ParticleBind") == 0)
					//{
					//	nodeCallback = new CREncapsulation::crParticleBindCallback;
					//}
					//else if(str.compare("SequenceBind") == 0)
					//{
					//	nodeCallback = new CREncapsulation::crSequenceBindCallback;
					//}
					cfg_script.Get("Type", str);
					if(str.compare("CullCallback")==0)
					{
						viewMatterObject->addCullCallback(nodeCallback.get());
					}
					else if(str.compare("UpdateCallback")==0)
					{
						viewMatterObject->addUpdateCallback(nodeCallback.get());
					}
					nParam = 1;
					while (cfg_script.Get("Param", str,nParam++))
					{
						nodeCallback->addEventParam(nParam-2,str);
					}
					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNPCCfg(): "<<cfg_script.GetLastError()<<std::endl;
				}
				int nRack = 1;
				while (cfg_script.Push("WeaponRack", nRack++))
				{
					cfg_script.Get("RackName", str);
					cfg_script.Get("RackNode", str1);
					viewMatterObject->insertWeaponRackName(str,str1);

					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNPCCfg(): "<<cfg_script.GetLastError()<<std::endl;
				}
				nRack = 1;
				while (cfg_script.Push("BodyRack", nRack++))
				{
					cfg_script.Get("RackNode", str);
					cfg_script.Get("BodyName", str1);
					crBodyCaseMatterObject *bodyCase = dynamic_cast<crBodyCaseMatterObject *>(viewMatterObject.get());
					if(bodyCase)
						bodyCase->insertBodyRackName(str,str1);
					else
						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNPCCfg(): BodyRack 被配置到非crBodyCaseMatterObject上"<<std::endl;

					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNPCCfg(): "<<cfg_script.GetLastError()<<std::endl;
				}
				nDescription = 1;
				while (cfg_script.Get("addDescription", str,nDescription++))
				{
					viewMatterObject->addDescription(str);
				}
				nCfg = 1;
				while (cfg_script.Get("cfgFile", str,nCfg++))
				{
					loadCfgFile(str,viewMatterObject.get());
				}

				mg->addChild(viewMatterObject.get());
				viewMatterObject->setName(mg->getName());
				ref_ptr<crCollideCallback> collideCallback;
				int nCollideCallback = 1;
				while(cfg_script.Push("CollideCallback", nCollideCallback++))
				{
					cfg_script.Get("Class", str);
					collideCallback = dynamic_cast<crCollideCallback *>(crEventCallbackManager::getInstance()->getEventCallback(str));
					if(!collideCallback) 
					{
						CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadNPCCfg: 未找到CollideCallback: "<<str<<std::endl;
					}
					if(collideCallback.valid())
					{
						viewMatterObject->addCollideCallback(collideCallback.get());
						nParam = 1;
						while (cfg_script.Get("Param", str,nParam++))
						{
							collideCallback->addEventParam(nParam-2,str);
						}
					}
					else
					{
						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNPCCfg(): not find CollideCallback of "<<str<<std::endl;
					}
					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNPCCfg(): "<<cfg_script.GetLastError()<<std::endl;
				}
				if(cfg_script.Get("AttachRadar", v_i))
				{
					radar = new CRPhysics::crRadarMatterObject;
					radar->setDetectRaySizeTo(v_i[0],v_i[1],v_i[2]);
					radarMg = new CRPhysics::crMatterGroup;
					radarMg->setMatterGroupMaterialString("RadarCollideDetect");
					radarMg->addChild(radar.get());
					viewMatterObject->attachRadar(radar.get());

					npc->setNPCRadar(radarMg.get());
					//crBrain::getInstance()->getCurrentActiveDB()->addChild(radarMg.get());
					//mg->addChild(radarMg.get());
				}

				int nWE = 1;
				while(cfg_script.Push("WeaponEquip",nWE++))
				{
					cfg_script.Get("Weapon", str);
					weaponMatterObject = CRPhysics::crWeaponManager::getInstance()->getWeaponMatterObject(str);
					if(weaponMatterObject.valid() && !viewMatterObject->testToAddWeaponMatterObject(weaponMatterObject.get()))
					{
						weaponMatterObject = CRPhysics::crWeaponManager::createWeaponMatterObject(weaponMatterObject.get());
						weaponMatterObject->setName(str);
						viewMatterObject->addWeaponMatterObject(weaponMatterObject.get());

						//if(cfg_script.Get("BulletCount", int1))
						//{
						//  weaponMatterObject->setBulletCount(int1);
						//}
						//if(cfg_script.Get("BulletsInWeapon", int1))
						//{
						//	weaponMatterObject->setBulletsInWeapon(int1);
						//}
						if(cfg_script.Get("IsCurrentWeaponMatterObject", int1) && int1)
						{
							viewMatterObject->selectWeaponMatterObject(weaponMatterObject.get());
						}
					}
					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNPCCfg(): "<<cfg_script.GetLastError()<<std::endl;
				}

				//if(cfg_script.Get("HideBody", int1))
				//{
				//	viewMatterObject->setHideBody(int1);
					viewMatterObject->setVisiable(false);
					viewMatterObject->setEnableIntersect(false);
				//}
				if(cfg_script.Get("MaxSpeed", v_i))
					viewMatterObject->setMaxSpeed(crVector3(v_i[0],v_i[1],v_i[2]));
				if(cfg_script.Get("MinSpeed", v_i))
					viewMatterObject->setMinSpeed(crVector3(v_i[0],v_i[1],v_i[2]));
				if(cfg_script.Get("Velocity", v_i))
					viewMatterObject->setVelocity(crVector3(v_i[0],v_i[1],v_i[2]));
				if(cfg_script.Get("GroundDetect", int1))
					viewMatterObject->setGroundDetect((int1));
				if(cfg_script.Get("CanFallDown", int1))
					viewMatterObject->setCanFalldown((int1));
				if(cfg_script.Get("ViewOffsetRange", v_i))
					viewMatterObject->setViewOffsetRange(crVector2(v_i[0],v_i[1]));
				if(cfg_script.Get("ViewOffset", flt1))
					viewMatterObject->setViewOffset(flt1);
				if(cfg_script.Get("ViewPitchRange", v_i))
					viewMatterObject->setViewPitchRange(crVector2(v_i[0],v_i[1]));
				if(cfg_script.Get("ViewPitch", flt1))
					viewMatterObject->setViewPitch(flt1);
				if(cfg_script.Get("ViewRoll", flt1))
					viewMatterObject->setViewRoll(flt1);
				if(cfg_script.Get("ViewHeight", flt1))
					viewMatterObject->setViewHeight(flt1);

				if(cfg_script.Get("LockViewPitch", int1))
				{
					viewMatterObject->setLockViewPitch(int1);
				}
				if(cfg_script.Get("LockViewRoll", int1))
				{
					viewMatterObject->setLockViewRoll(int1);
				}
				if(cfg_script.Get("LockViewOffset", int1))
				{
					viewMatterObject->setLockViewOffset(int1);
				}
				if(cfg_script.Get("FreeCamera", int1))
				{
					viewMatterObject->setFreeCamera(int1);
				}
				if(cfg_script.Get("CameraPitchDelta", flt1))
				{
					viewMatterObject->setCameraPitchDelta(flt1);
				}
				if(cfg_script.Get("ViewOffsetVelocity", flt1))
				{
					viewMatterObject->setViewOffsetVelocity(flt1);
				}
				if(cfg_script.Get("ViewPitchVelocity", flt1))
				{
					viewMatterObject->setViewPitchVelocity(flt1);
				}
				if(cfg_script.Get("Orientation", v_i))
				{
					CRCore::crQuat q1(DegreesToRadians(v_i[0]), CRCore::crVector3(1, 0, 0)), q2(DegreesToRadians(v_i[1]), CRCore::crVector3(0, 1, 0)), q3(DegreesToRadians(v_i[2]), CRCore::crVector3(0, 0, 1));
					CRCore::crMatrix mrot;
					mrot.set(q1*q2*q3);
					viewMatterObject->setMatrix(mrot);

					//crQuat orientation;
					//orientation.set(v_i[0],v_i[1],v_i[2],v_i[3]);
					//CRCore::crMatrix mrot(orientation);
					//viewMatterObject->setMatrix(mrot);
				}
				if(cfg_script.Get("Position", v_i))
					viewMatterObject->setPosition(crVector3f(v_i[0],v_i[1],v_i[2]));
				//if(cfg_script.Get("Mass", flt1))
				//	viewMatterObject->setMass(flt1);

				ref_ptr<crGameBodyInfo> gameBodyInfo;
				ref_ptr<crData> data;
				if(cfg_script.Push("GameBodyInfo"))
				{
					cfg_script.Get("Type", str);
					if(str.compare("GameBodyInfo") == 0)
					{
						gameBodyInfo = new crGameBodyInfo;
					}
					int nParam = 1;
					while (cfg_script.Get("Param", str,nParam++))
					{
						gameBodyInfo->addParam(nParam-2,str);
					}
					//if(isNetBot) gameBodyInfo->setTeam(netTeam);

					data=loadData(cfg_script);
					gameBodyInfo->setDataClass(data.get());

					viewMatterObject->setGameBodyInfo(gameBodyInfo.get());
					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
				}

				int nMatterObjectEvent = 1;
				int nEvent = 1;
				int nEventParam = 1;
				_crInt32 typeParam1,typeParam2;
				ref_ptr<crEventCallback> event;
				while (cfg_script.Push("PhysicsObjectEvent", nMatterObjectEvent++)) 
				{
					cfg_script.Get("TypeParam", str,1);
					typeParam1 = parseEventMsg(str);
					cfg_script.Get("TypeParam", str,2);
					typeParam2 = parseEventMsg(str);

					nEvent = 1;
					while (cfg_script.Push("Event", nEvent++))
					{
						cfg_script.Get("EventFunction", str);
						event = crEventCallbackManager::getInstance()->getEventCallback(str);
						if(!event) 
						{
							CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadCfgFile(): 未找到EventFunction: "<<str<<std::endl;
							continue;
						}
						nEventParam = 1;
						while (cfg_script.Get("EventParam", str,nEventParam++)) 
						{
							event->addEventParam(nEventParam-2,str);
						}

						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
						viewMatterObject->insertEvent(MAKEINT64(typeParam1,typeParam2),event.get());
					}
					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
				}
				///load data handle
				//ref_ptr<crData> data;
				//int nParam;
				data = loadData(cfg_script);
				viewMatterObject->setDataClass(data.get());
			}
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNPCCfg(): "<<cfg_script.GetLastError()<<std::endl;
		}

		//if(cfg_script.Get("createAi", str))
		//{
		//	CREncapsulation::loadAI(str,*mg,npc);
		//}
		//if(viewMatterObject.valid())
		//    CRIOManager::crLoadManager::getInstance()->requestAddNode(CRCore::crBrain::getInstance()->getAITreeHead(),viewMatterObject.get(),false);

		npc->setNPCParent(parent.get());
		if(radarMg.valid()) CRIOManager::crLoadManager::getInstance()->requestAddNode(parent.get(),radarMg.get(),true);
		CRIOManager::crLoadManager::getInstance()->requestAddNode(parent.get(),mg.get(),true);

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadNPCCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}

	//int nNeutrals = 1;
	//crVector3 pos;
	//while(cfg_script.Get("StaticNeutral",v_i,nNeutrals++))
	//{
	//	pos.set(v_i[0],v_i[1],v_i[2]);
	//	CRPhysics::crBotManager::getInstance()->addNeutralStaticPositions(pos);
	//}
	//return viewMatterObject.get();
}

void CREncapsulation::loadSkillCfg(const std::string &scriptFile, CRCore::crGroup *loadedNode)
{
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,scriptFile))
	{
		CRCore::notify(CRCore::FATAL)<<"loadSkillCfg(): file open error,"<<scriptFile<<std::endl;
		return;
	}

	std::string str;
	std::vector<float> v_i;
	float flt1;
	int int1;
	unsigned int attrmask = 0;
	ref_ptr<CRPhysics::crWeaponObject> weaponObject;
	ref_ptr<CRPhysics::crBulletObject> bulletObject;

	int nWeaponMatterObject = 1;
	ref_ptr<CRPhysics::crWeaponMatterObject> weaponMatterObject;
	ref_ptr<CRPhysics::crBulletMatterObject> bulletMatterObject;
	float bulletMass = 0.1f;

	int nWeapon = 1;
	int nDescription = 1;
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
	crNode *node;
	while(cfg_script.Push("Weapon",nWeapon++))
	{
		if(cfg_script.Push("WeaponModel"))
		{
			weaponObject = new CRPhysics::crWeaponObject;
			if(cfg_script.Get("GroupName", str))
			{
				if(!str.empty() && str.compare("NULL")!=0)
				{
					searchByNameVisitor.reset();
					searchByNameVisitor.setNameId(str);
					searchByNameVisitor.setSearchNodeType(ALLNODE);
					loadedNode->accept(searchByNameVisitor);
					node = searchByNameVisitor.getResult();
					if(node)
					{
						weaponObject->addChild(node);
					}
				}
			}
			if(cfg_script.Get("MaxBulletsInOneClip", int1))
			{
				weaponObject->setMaxBulletsInOneClip(int1);
			}
			if(cfg_script.Get("TimeInterval", flt1))
			{
				weaponObject->setInterval(flt1);
			}
			if(cfg_script.Get("ReplaceClipTime", flt1))
			{
				weaponObject->setReplaceClipTime(flt1);
			}
			if(cfg_script.Get("AttackDistance", flt1))
			{
				weaponObject->setAttackDistance(flt1);
			}
			if(cfg_script.Get("GunPoint", v_i))
			{
				weaponObject->setGunPoint(crVector3(v_i[0],v_i[1],v_i[2]));
			}
			if(cfg_script.Get("ClipPos", v_i))
			{
				weaponObject->setClipPos(crVector3(v_i[0],v_i[1],v_i[2]));
			}
			if(cfg_script.Get("EffectAttrMask", str))
			{
				attrmask = 0;
				if(str.find("Fire_Particle") != -1)
					attrmask |= CRPhysics::crWeaponObject::Fire_Particle;
				if(str.find("Fire_Sequence") != -1)
					attrmask |= CRPhysics::crWeaponObject::Fire_Sequence;
				if(str.find("Fire_Sound") != -1)
					attrmask |= CRPhysics::crWeaponObject::Fire_Sound;
				if(str.find("Clip_Particle") != -1)
					attrmask |= CRPhysics::crWeaponObject::Clip_Particle;
				if(str.find("Clip_Sequence") != -1)
					attrmask |= CRPhysics::crWeaponObject::Clip_Sequence;
				if(str.find("Clip_Sound") != -1)
					attrmask |= CRPhysics::crWeaponObject::Clip_Sound;
				if(str.find("Collide_Particle") != -1)
					attrmask |= CRPhysics::crWeaponObject::Collide_Particle;
				if(str.find("Collide_Sequence") != -1)
					attrmask |= CRPhysics::crWeaponObject::Collide_Sequence;
				if(str.find("Collide_Sound") != -1)
					attrmask |= CRPhysics::crWeaponObject::Collide_Sound;

				weaponObject->setEffectAttrMask((CRPhysics::crWeaponObject::EffectAttrMask)attrmask);
			}
			if(cfg_script.Get("AttackMask", str))
			{
				attrmask = 0;
				if(str.find("AttackAll") != -1)
					attrmask |= CRPhysics::crWeaponObject::AttackAll;
				if(str.find("AttackGround") != -1)
					attrmask |= CRPhysics::crWeaponObject::AttackGround;
				if(str.find("AttackAir") != -1)
					attrmask |= CRPhysics::crWeaponObject::AttackAir;
				if(str.find("AttackDeepSea") != -1)
					attrmask |= CRPhysics::crWeaponObject::AttackDeepSea;

				weaponObject->setAttackMask(attrmask);
			}
			//CRPhysics::crWeaponManager::getInstance()->addWeapon(weaponObject.get());

			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSkillCfg(): "<<cfg_script.GetLastError()<<std::endl;
		}
		if(cfg_script.Push("BulletModel"))
		{
			bulletObject = new CRPhysics::crBulletObject;
			if(cfg_script.Get("Name", str))
			{
				bulletObject->setName(str);
				if(cfg_script.Get("GroupName", str))
				{
					if(!str.empty() && str.compare("NULL")!=0)
					{
						cfg_script.Get("GroupName", str);
						searchByNameVisitor.reset();
						searchByNameVisitor.setNameId(str);
						searchByNameVisitor.setSearchNodeType(ALLNODE);
						loadedNode->accept(searchByNameVisitor);
						node = searchByNameVisitor.getResult();
						if(node)
						{
							bulletObject->addChild(node);
						}
					}
				}
			}

			if(cfg_script.Get("DamageType", str))
			{
				if(str.compare("Direct")==0)
					bulletObject->setDamageType(CRPhysics::crBulletObject::Direct);
				else if(str.compare("Explode")==0)
					bulletObject->setDamageType(CRPhysics::crBulletObject::Explode);
				else if(str.compare("Extra")==0)
					bulletObject->setDamageType(CRPhysics::crBulletObject::Extra);
			}
			if(cfg_script.Get("ExecutionRang", v_i))
			{
				bulletObject->setExecutionRang(v_i[0],v_i[1]);
			}
			if(cfg_script.Get("ExplodeRadius", flt1))
			{
				bulletObject->setExplodeRadius(flt1);
			}
			if(cfg_script.Get("K", flt1))
			{
				bulletObject->setK(flt1);
			}
			if(cfg_script.Get("Impetus", flt1))
			{
				bulletObject->setImpetus(flt1);
			}
			if(cfg_script.Get("Force", v_i))
			{
				bulletObject->setForce(CRCore::crVector3(v_i[0],v_i[1],v_i[2]));
			}
			if(cfg_script.Get("LinearVel", flt1))
			{
				bulletObject->setLinearVel(flt1);
			}
			if(cfg_script.Get("Duration", flt1))
			{
				bulletObject->setDuration(flt1);
			}
			if(cfg_script.Get("MissilePrecision", flt1))
			{
				bulletObject->setMissilePrecision(CRCore::rangef(-flt1,flt1));
			}
			if(cfg_script.Get("MissileFlexibility", flt1))
			{
				bulletObject->setMissileFlexibility(flt1);
			}
			if(cfg_script.Get("ParticleFlyScale", flt1))
			{
				bulletObject->setParticleFlyScale(flt1);
			}
			if(cfg_script.Get("ParticleFlyOffset", flt1))
			{
				bulletObject->setParticleFlyOffset(flt1);
			}
			if(cfg_script.Get("AttrMask", str))
			{
				attrmask = 0;
				if(str.find("GravityEnable") != -1)
					attrmask |= CRPhysics::crBulletObject::GravityEnable;
				if(str.find("VanishWhenCollide") != -1)
					attrmask |= CRPhysics::crBulletObject::VanishWhenCollide;
				if(str.find("Track") != -1)
					attrmask |= CRPhysics::crBulletObject::Track;

				bulletObject->setAttrMask((CRPhysics::crBulletObject::AttrMask)attrmask);
			}
			if(cfg_script.Get("EffectAttrMask", str))
			{
				attrmask = 0;
				if(str.find("Fly_Particle") != -1)
					attrmask |= CRPhysics::crBulletObject::Fly_Particle;
				if(str.find("Fly_Sequence") != -1)
					attrmask |= CRPhysics::crBulletObject::Fly_Sequence;
				if(str.find("Fly_Sound") != -1)
					attrmask |= CRPhysics::crBulletObject::Fly_Sound;
				if(str.find("Collide_Particle") != -1)
					attrmask |= CRPhysics::crBulletObject::Collide_Particle;
				if(str.find("Collide_Particle2") != -1)
					attrmask |= CRPhysics::crBulletObject::Collide_Particle2;
				if(str.find("Collide_Particle3") != -1)
					attrmask |= CRPhysics::crBulletObject::Collide_Particle3;
				if(str.find("Collide_Sequence") != -1)
					attrmask |= CRPhysics::crBulletObject::Collide_Sequence;
				if(str.find("Collide_Sound") != -1)
					attrmask |= CRPhysics::crBulletObject::Collide_Sound;
				if(str.find("Explode_Particle") != -1)
					attrmask |= CRPhysics::crBulletObject::Explode_Particle;
				if(str.find("Explode_Sequence") != -1)
					attrmask |= CRPhysics::crBulletObject::Explode_Sequence;
				if(str.find("Explode_Sound") != -1)
					attrmask |= CRPhysics::crBulletObject::Explode_Sound;

				bulletObject->setEffectAttrMask((CRPhysics::crBulletObject::EffectAttrMask)attrmask);
			}
			if(cfg_script.Get("ExecutionPhysicsType", str))
			{
				if(str.compare("Collide")==0)
					bulletObject->setExecutionPhysicsType(crBulletObject::Type_Collide);
				else if(str.compare("Explode")==0)
					bulletObject->setExecutionPhysicsType(crBulletObject::Type_Explode);
				else if(str.compare("TimeExplode")==0)
					bulletObject->setExecutionPhysicsType(crBulletObject::Type_TimeExplode);
				else if(str.compare("Sphere")==0)
					bulletObject->setExecutionPhysicsType(crBulletObject::Type_Sphere);
				else if(str.compare("Ray")==0)
					bulletObject->setExecutionPhysicsType(crBulletObject::Type_Ray);
				else if(str.compare("Weapon")==0)
					bulletObject->setExecutionPhysicsType(crBulletObject::Type_Weapon);
				else if(str.compare("Shield_Box")==0)
					bulletObject->setExecutionPhysicsType(crBulletObject::Type_Shield_Box);
				else if(str.compare("Shield_Sphere")==0)
					bulletObject->setExecutionPhysicsType(crBulletObject::Type_Shield_Sphere);
				else if(str.compare("Volume")==0)
					bulletObject->setExecutionPhysicsType(crBulletObject::Type_Volume);
				else if(str.compare("ExplodeVolume")==0)
					bulletObject->setExecutionPhysicsType(crBulletObject::Type_ExplodeVolume);
			}
			if(cfg_script.Get("ExecutionPhysicsLength", flt1))
			{
				bulletObject->setExecutionPhysicsLength(flt1);
			}
			if(cfg_script.Get("ModelPhysicsRadius", flt1))
			{
				bulletObject->setModelPhysicsRadius(flt1);
			}
			if(cfg_script.Get("VolumeName", str))
			{
				bulletObject->setVolumeName(str);
			}
			if(cfg_script.Get("VolumeLength", v_i))
			{
				bulletObject->setVolumeLength(crVector3(v_i[0],v_i[1],v_i[2]));
			}
			if(cfg_script.Get("MaxTriggerCount", int1))
			{
				bulletObject->setMaxTriggerCount(int1);
			}
			if(cfg_script.Get("ReTriggerDelay", flt1))
			{
				bulletObject->setReTriggerDelay(flt1);
			}
			nDescription = 1;
			while (cfg_script.Get("addDescription", str,nDescription++))
			{
				bulletObject->addDescription(str);
			}
			//CRPhysics::crWeaponManager::getInstance()->addBullet(bulletObject.get());
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSkillCfg(): "<<cfg_script.GetLastError()<<std::endl;
		}
		if(cfg_script.Push("WeaponMatterObject"))
		{
			weaponMatterObject = new CRPhysics::crWeaponMatterObject;
			cfg_script.Get("Name", str);
			weaponMatterObject->setName(str);
			if(cfg_script.Get("BulletCount", int1))
			{
				weaponMatterObject->setBulletCount(int1);
			}
			if(cfg_script.Get("BulletsInWeapon", int1))
			{
				weaponMatterObject->setBulletsInWeapon(int1);
			}
			if(cfg_script.Get("WeaponMass", flt1))
			{
				weaponMatterObject->setMass(flt1);
			}
			//if(cfg_script.Get("WeaponRackName", str))
			//{
			//	weaponMatterObject->setWeaponRackName(str);
			//}

			bulletMatterObject = new CRPhysics::crBulletMatterObject;
			bulletMatterObject->addChild(bulletObject.get());
			bulletMatterObject->setDescriptionsInString(bulletObject->getDescriptionsInString());
			bulletMatterObject->setName(bulletObject->getName());
			bulletMatterObject->setFlying(false);
			if(cfg_script.Get("BulletMass", bulletMass))
				bulletMatterObject->setMass(bulletMass);

			weaponMatterObject->addChild(weaponObject.get());
            weaponMatterObject->setBullet(bulletMatterObject.get());

			nDescription = 1;
			while (cfg_script.Get("addDescription", str,nDescription++))
			{
				weaponMatterObject->addDescription(str);
			}
			//crWeaponManager::getInstance()->addBulletMatterObject(bulletObjectIndex,bulletMatterObject.get());
			//crWeaponManager::getInstance()->insertWeaponMatterObject(weaponMatterObject.get());

			weaponMatterObject->setVisiable(false);
			bulletMatterObject->setVisiable(false);
			
			//bulletMatterObject->addCollideCallback(new crBulletCollideCallback());///for test

			loadedNode->removeChild(0,loadedNode->getNumChildren());
			loadedNode->addChild(weaponMatterObject.get());
			loadedNode->addChild(bulletMatterObject.get());

			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSkillCfg(): "<<cfg_script.GetLastError()<<std::endl;
		}
		if(weaponMatterObject.valid() && bulletMatterObject.valid())
		{
			crData *data;
			if (cfg_script.Push("WeaponData"))
			{
				data = loadData(cfg_script);
				weaponMatterObject->setDataClass(data);
				//data->inputParam(16,&itemid);//WCHDATA_ItemID
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSkillCfg(): "<<cfg_script.GetLastError()<<std::endl;
			}
			if (cfg_script.Push("BulletData"))
			{
				data = loadData(cfg_script);
				bulletMatterObject->setDataClass(data);
				//data->inputParam(16,&itemid);//WCHDATA_ItemID
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSkillCfg(): "<<cfg_script.GetLastError()<<std::endl;
			}
		}
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSkillCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}
}
void CREncapsulation::loadItemCfg(const std::string &scriptFile, CRCore::crGroup *loadedNode/*,bool staticRoot*/)
{
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,scriptFile))
	{
		CRCore::notify(CRCore::FATAL)<<"loadItemCfg(): file open error,"<<scriptFile<<std::endl;
		return;
	}

	std::string str;
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
	ref_ptr<crGroup> parent = crSceneManager::getInstance()->getBotRoot();//staticRoot?crSceneManager::getInstance()->getStaticRoot():crSceneManager::getInstance()->getSceneRoot();
    if(!parent.valid())
		return;
	if(cfg_script.Push("Item"))
	{
		if(cfg_script.Get("CollideMode", str))
		{
			if(str.compare("CollideNone")==0)
			{
				loadedNode->setCollideMode(crNode::CollideNone);
			}
			else if(str.compare("CollideWithBoundBox")==0)
			{
				loadedNode->setCollideMode(crNode::CollideWithBoundBox);
			}
			else if(str.compare("CollideWithBoundSphere")==0)
			{
				loadedNode->setCollideMode(crNode::CollideWithBoundSphere);
			}
			else if(str.compare("PrecisionCollide")==0)
			{
				loadedNode->setCollideMode(crNode::PrecisionCollide);
			}
			else if(str.compare("AvoidCollide")==0)
			{
				loadedNode->setCollideMode(crNode::AvoidCollide);
			}
		}
		CRIOManager::crLoadManager::getInstance()->requestAddNode(parent.get(),loadedNode,true);
		crData *data = loadData(cfg_script);
		loadedNode->setDataClass(data);

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadItemCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}
}
void CREncapsulation::loadCameraManagerCfg(const std::string &path)
{
	std::string filename;
	if(path.empty())
		filename = "script/CameraManager.cfg";
	else
		filename = path + "/script/CameraManager.cfg";
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
	{
		CRCore::notify(CRCore::FATAL)<<"loadCameraManagerCfg(): file open error,"<<filename<<std::endl;
		return;
	}

	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
	std::string str;
    ref_ptr<crCamera> camera;
	float flt1;
	int int1;
	int nCamera = 1;
	crNode *node;
	while(cfg_script.Push("Camera",nCamera++))
	{
        camera = new crCamera;
		if(cfg_script.Get("Name", str))
			camera->setName(str);
		if(cfg_script.Get("ViewPitch", flt1))
			camera->setViewPitch(flt1);
		if(cfg_script.Get("ViewRoll", flt1))
			camera->setViewRoll(flt1);
		if(cfg_script.Get("ViewHeight", flt1))
			camera->setViewHeight(flt1);
		if(cfg_script.Get("ViewOffset", flt1))
			camera->setViewOffset(flt1);
		if(cfg_script.Get("KeyboardMouseMode", int1))
			camera->setKeyboardMouseMode(int1);
		if(cfg_script.Get("AttachedNode", str))
		{
			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId(str);
			searchByNameVisitor.setSearchNodeType(ALLNODE);
			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
			node = searchByNameVisitor.getResult();
			if(node)
			{
				camera->attachNode(node);
			}
		}
		crCameraManager::getInstance()->addCamera(camera.get());
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadCameraManagerCfg(): "<<cfg_script.GetLastError()<<std::endl;
	}
}
////释放所有资源，这些资源将在系统退出时自动释放，无须手动释放
//rbody::ReplicantBodyMgr::instance()->clear();
//crBrain::getInstance()->clear();
//crDisplaySettings::instance()->clear();
//crEnvironmentManager::getInstance()->clear();
////crEventCallbackManager::getInstance()->clear();
//crLightSourceManager::getInstance()->clear();
//crMaterialStateSetManager::getInstance()->clear();
//crRenderToTextureManager::getInstance()->clear();
//crShaderManager::getInstance()->clear();
//crFilterRenderManager::getInstance()->clear();
////crSoundManager::getInstance()->clear();
////cr2DStageManager::instance()->clear();
//CRIOManager::crRegistry::instance()->setDatabasePager(0);
//CRIOManager::crRegistry::instance()->setSharedStateManager(0);	
//CRIOManager::crRegistry::instance()->clearObjectCache();
//crEffectParticleManager::getInstance()->clear();
//crOdeWorld::clear();
//crWeaponManager::getInstance()->clear();
//CRFX::crRegistry::instance()->clear();
//CRText::crDefaultFont::instance()->clear();

/////*
////3Dmark2003注册号的算法研究
////
////先在字符串B2CD6FHJ1LN4RSTE3VWX8YZA0Q7MUK9P5(33个字符)中任取12位 
////组成XXXXa-XXXXb-XXXXc-defgh形的字串，X为任取的字符， 
////a,b,c位对应的前面4个字符在字符串B2CD6FHJ1LN4RSTE3VWX8YZA0Q7MUK9P5中的位置编号（16进制）相加再对21（16进制）取余得到数字对应位置的字符，d为a，b，c位置编号相加再对21取余得到数字对应位置的字符,e为b，c,d位置编号相加再对21取余得到数字对应位置的字符,f为c,d,e位置编号相加再对21取余得到数字对应位置的字符,g为d,e,f位置编号相加再对21取余得到数字对应位置的字符,h为e，f，g位置编号相加再对21取余得到数字对应位置的字符。 
////*/
////#define Rnd(i) i * rand() / RAND_MAX
////std::string basecode = "A0BD1JCF2L4RNETS3XW8VZ6Y7QGKU9MP5";
////std::string getRegistrationCode()
////{
////	std::string code;
////	char abc[4];
////	int i[4];
////	int iabc[3];
////	int j,k;
////	for(j = 0; j<3; j++)
////	{
////		for(k = 0; i<4; k++)
////		{
////			i[k] = Rnd(32);
////			abc[k] = basecode[i[k]];
////
////			code.push_back(abc[k]);
////		}
////		iabc[j] = (i[0]+i[1]+i[2]+i[3])%21;
////		code.push_back(basecode[iabc[j]]);
////		code.push_back('-');
////	}
////
////	int iend[5];
////	iend[0] = iabc[0] + iabc[1] + iabc[2];
////	iend[1] = iabc[1] + iabc[2] + iend[0];
////	iend[2] = iabc[2] + iend[0] + iend[1];
////	iend[3] = iend[0] + iend[1] + iend[2];
////	iend[4] = iend[1] + iend[2] + iend[3];
////
////	for(j = 0; j<5; j++)
////	{
////		code.push_back(basecode[iend[j] % 21]);
////	}
////
////	return code;
////}
////
//