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
#ifndef CRGAME_CREATURE3D_H
#define CRGAME_CREATURE3D_H 1

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
#include <CRCore/crBlendColor.h>
#include <CRCore/crBlendEquation.h>
#include <CRCore/crClampColor.h>
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
#include <CRCore/crLightSourceManager.h>
#include <CRCore/crRenderToTexture.h>
#include <CRCore/crRenderToTextureManager.h>
#include <CRCore/crFilterRenderManager.h>
#include <CRCore/crDrawPixels.h>
#include <CRCore/crMaterialStateSetManager.h>
#include <CRCore/crIntersectVisitor.h>
#include <CRCore/crFog.h>
#include <CRCore/crEnvironmentManager.h>
#include <CRCore/crSequence.h>
#include <CRCore/crDataManager.h>
#include <CRCore/crFrontFace.h>
#include <CRCore/crLogicOp.h>
#include <CRCore/crMultisample.h>
#include <CRCore/crPolygonOffset.h>
#include <CRCore/crScissor.h>
#include <CRCore/crStencil.h>
#include <CRCore/crStencilTwoSided.h>
#include <CRCore/crTexEnvCombine.h>
#include <CRCore/crTexEnvFilter.h>
#include <CRCore/crLineWidth.h>
#include <CRCore/crPoint.h>
#include <CRCore/crDataManager.h>
#include <CRCore/crHandleManager.h>
#include <CRCore/crShaderManager.h>
#include <CRCore/crDOFTransform.h>
#include <CRCore/crWidgetNode.h>
//#include <CREncapsulation/crImageStreamManager.h>

#include <CRUtil/crOptimizer.h>
#include <CRUtil/crSmoothingVisitor.h>
#include <CRUtil/crSimplifier.h>
#include <CRUtil/crTesselator.h>
#include <CRUtil/crTangentSpaceGenerator.h>
#include <CRUtil/crNoise.h>
#include <CRUtil/crUpdateVisitor.h>
#include <CRUtil/crPrepareVisitor.h>

#include <CRPhysics/crOdeWorld.h>
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
#include <CREncapsulation/crCollideCallback.h>
#include <CRPhysics/crCarMatterObject.h>
//#include <CRPhysics/crBotManager.h>
#include <CRPhysics/crGameBodyInfo.h>
#include <CRPhysics/crVolumeNode.h>
#include <CRPhysics/crCamera.h>

//#include <CRAL/crSoundManager.h>
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

#include <CRIOManager/crReadFile.h>
#include <CRIOManager/crWriteFile.h>
#include <CRIOManager/crFileutils.h>
#include <CRIOManager/crFileNameUtils.h>
#include <CRIOManager/crRegistry.h>
#include <CRIOManager/crLoadManager.h>
#include <CRIOManager/crCookFile.h>

#include <CRProducer/crViewer.h>
#include <CRProducer/crSceneHandler.h>
#include <CRProducer/crKeyboardMouseHandle.h>

#include <CRText/crText.h>
#include <CRText/crFont.h>
#include <CRText/crDefaultFont.h>

#include <Producer/RenderSurface>

#include <CRCore/crVector2.h>

#include <CREncapsulation/crLoader.h>
//#include <CREncapsulation/crObjectEventCallback.h>
//#include <CREncapsulation/crNodeCallbacks.h>

//#include <CRNet/App/crNetSceneUpdater.h>
//#include <CRNet/App/crSceneServerStartHandler.h>
//#include <CRNet/App/crNetLoader.h>


#include <CRGUI/cr2DStageManager.h>
#include <CRGUI/crImageStage.h>
#include <CRGUI/crElement.h>
#include <CRGUI/crWin32.h>

//#include <CRNet/crNetManager.h>
//#include <CRNet/crNetDataManager.h>
//#include <CRNet/crNetCallbackManager.h>
//#include <CRNet/crNetContainer.h>
//#include <CRNet/crConnectServerPacket.h>
//#include <CRNet/crErrorPacket.h>
//#include <CRNet/crNetWorkUpdater.h>

#include <CREncapsulation/crStartHandler.h>
#include <CREncapsulation/crSceneManager.h>
#include <CRUI/crWidgetExtend.h>
#include <CREncapsulation/crGameDices.h>
#include <CREncapsulation/crTableIO.h>
#include <CRDataBase/crDataBaseManager.h>

//#include <rvrutils/Property.h>
//#include <rvrutils/utils.h>
//#include <rvrutils/CodeLineException.h>
//#include <rbody/Creature3D/crCharacterSystemUpdater.h>
//#include <rbody/Creature3D/CreBody.h>
//#include <rbody/Creature3D/ReplicantBodyMgr.h>
#include <cal3d/loader.h>
//#include "rcfg/ConfigScriptOO.h"
#include <sys/stat.h>

#include <string>
#include <vector>
#include <stack>
#include <set>
#include <map>

#endif