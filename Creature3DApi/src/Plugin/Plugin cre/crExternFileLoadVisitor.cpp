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
#include <Plugin cre/crExternFileLoadVisitor.h>
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

#include <rbody/Creature3D/crCharacterSystemUpdater.h>
#include <rbody/Creature3D/CreBody.h>
#include <rbody/Creature3D/ReplicantBodyMgr.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRNetApp/appNetGameData.h>
#include <CRNetApp/appClientPlayerData.h>
using namespace CRCore;
using namespace CRGame;
using namespace CRParticle;
using namespace CRPhysics;
using namespace CRUtil;
#define setNewNode(loadedNode) \
{\
	newNode = loadedNode;\
	if(newNode.valid()) \
    {\
	    newNode->setName(node.getName());\
		newNode->setVisiable(node.getVisiable());\
		newNode->setCalcShadow(node.isCalcShadow());\
		newNode->setEnableIntersect(node.getEnableIntersect());\
		newNode->setRenderable(node.getRenderable());\
		newNode->setCollideMode(node.getCollideMode());\
		newNode->setCullingActive(node.getCullingActive());\
	    newNode->setDescriptionsInString(command);\
		crGroup* newGroup = dynamic_cast<crGroup *>(newNode.get());\
		if(newGroup) \
        {\
		    for(int i = 0; i<node.getNumChildren(); i++)\
                newGroup->addChild(node.getChild(i));\
	        node.removeChild(0,node.getNumChildren());\
			node.getDescriptions().clear();\
		}\
		group->replaceChild(&node,newNode.get());\
    }\
}
//
#define setNewNode2(loadedNode) \
{\
	newNode = loadedNode;\
	if(newNode.valid()) \
    {\
	    newNode->setName(node.getName());\
		newNode->setVisiable(node.getVisiable());\
		newNode->setCalcShadow(node.isCalcShadow());\
		newNode->setEnableIntersect(node.getEnableIntersect());\
		newNode->setRenderable(node.getRenderable());\
		newNode->setCollideMode(node.getCollideMode());\
		newNode->setCullingActive(node.getCullingActive());\
	    newNode->setDescriptionsInString(command);\
		crGroup* newGroup = dynamic_cast<crGroup *>(newNode.get());\
		if(newGroup) \
        {\
            newGroup->addChild(&node);\
	        node.getDescriptions().clear();\
		}\
		group->replaceChild(&node,newNode.get());\
    }\
}
//crStateSet *crExternFileLoadVisitor::getProgramStateSet(const std::string& filename, bool addT, bool addB,crStateSet *oldStateset)
//{
//	CRCore::ref_ptr<CRCore::crProgram> program;
//	CRCore::ref_ptr<CRCore::crShader> shader;
//	CRCore::ref_ptr<CRCore::crStateSet> stateset;
//	CRCore::ref_ptr<CRCore::crStateSet> exist_stateset;
//	std::string vp,fp;
//	program = crBrain::getInstance()->getProgram(filename);
//	if(!program.valid())
//	{
//		if(!CRCore::crShader::loadShaderSourceFromFile(filename,vp,fp))
//			return NULL;
//
//		program = new CRCore::crProgram;
//		program->setName(filename);
//		shader = new CRCore::crShader(CRCore::crShader::VERTEX,vp);
//		shader->setName(filename+"_vp");
//		program->addShader(shader.get());
//		shader = new CRCore::crShader(CRCore::crShader::FRAGMENT,fp);
//		shader->setName(filename+"_fp");
//		program->addShader(shader.get());
//		crBrain::getInstance()->insertProgram(filename,program.get());
//
//		if(addT) program->addBindAttribLocation("tangent",ATTRIBUTE_TANGENT);
//		if(addB) program->addBindAttribLocation("binormal",ATTRIBUTE_BINORMAL);
//	}
//	if(oldStateset) stateset = oldStateset;
//	else stateset = new crStateSet;
//	stateset->setAttributeAndModes(program.get(),CRCore::crStateAttribute::ON);
//	exist_stateset = CRCore::crBrain::getInstance()->checkStateSetExistAndGet(stateset.get());
//	if(exist_stateset.valid())
//		stateset = exist_stateset;
//	else
//		CRCore::crBrain::getInstance()->insertStateSet(stateset.get());
//
//	return stateset.get();
//}
//
class MoveEarthySkyWithEyePointTransform : public crTransform
{
public:
	/** Get the transformation matrix which moves from local coords to world coords.*/
	virtual bool computeLocalToWorldMatrix(crMatrix& matrix,crNodeVisitor* nv) const 
	{
		CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
		if (cv)
		{
			m_eyePointLocal = cv->getEyeLocal();
			m_eyePointLocal.z() = 0.0f;
		}
        matrix.preMult(crMatrix::translate(m_eyePointLocal));
		return true;
	}

	/** Get the transformation matrix which moves from world coords to local coords.*/
	virtual bool computeWorldToLocalMatrix(crMatrix& matrix,crNodeVisitor* nv) const
	{
		CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
		if (cv)
		{
			m_eyePointLocal = cv->getEyeLocal();
			m_eyePointLocal.z() = 0.0f;
		}
		matrix.postMult(crMatrix::translate(-m_eyePointLocal));
		return true;
	}
protected:
	mutable crVector3 m_eyePointLocal;
};

class MoveSunWithEyePointTransform : public crTransform
{
public:
	/** Get the transformation matrix which moves from local coords to world coords.*/
	virtual bool computeLocalToWorldMatrix(crMatrix& matrix,crNodeVisitor* nv) const 
	{
		CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
		if (cv)
		{
			matrix.preMult(crMatrix::translate(cv->getEyeLocal()));
		}
		return true;
	}

	/** Get the transformation matrix which moves from world coords to local coords.*/
	virtual bool computeWorldToLocalMatrix(crMatrix& matrix,crNodeVisitor* nv) const
	{
		CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
		if (cv)
		{
			matrix.postMult(crMatrix::translate(-cv->getEyeLocal()));
		}
		return true;
	}
};


//
//class crFarClipCallback : public CRCore::crNodeCallback
//{
//public:
//	crFarClipCallback(float far):m_zFar(far){}
//
//	void init()
//	{
//		m_clipPlane = new CRCore::crClipPlane;
//		m_clipPlane->setClipPlaneNum(0);
//		m_stateset = new crStateSet;
//		m_stateset->setAttributeAndModes(m_clipPlane.get(),crStateAttribute::ON);
//	}
//	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
//	{
//		CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
//		if (cv)
//		{
//			//crMatrixd prjMat = cv->getProjectionMatrix();
//			crMatrixd viewMat = cv->getModelViewMatrix();
//			//double left, right, bottom, top, zNear, zFar;
//			//prjMat.getFrustum(left, right, bottom, top, zNear, zFar);
//			//prjMat.makeFrustum(left, right, bottom, top, zNear, 30000);
//
//			//crPolytope frustum;
//			//frustum.setToUnitFrustum(true,true);
//			//frustum.transformProvidingInverse( viewMat * prjMat );
//			//crPolytope::PlaneList& pl = frustum.getPlaneList();
//   //         m_clipPlane->setClipPlane(pl[4]);
//
//			//CRCore::crFrustum frustum;
//			//frustum.loadFrustum(viewMat * prjMat);
//			crVector4d ffar(0.0,0.0,1.0,10000.0);
//			//ffar.normalize();
//			//ffar = ffar * viewMat;
//			//ffar.normalize();
//			m_clipPlane->setClipPlane(ffar/*frustum.getPlane(FRUSTUM_FAR)*/);
//
//			bool stateset_pushed = cv->pushStateSet(m_stateset.get());
//			crNodeCallback::traverse(node,nv);
//			if(stateset_pushed) cv->popStateSet();	
//
//			//CRCore::notify(CRCore::NOTICE)<<"FRUSTUM_LEFT = "<<frustum.getPlane(FRUSTUM_LEFT).asVec4()<<std::endl;
//			//CRCore::notify(CRCore::NOTICE)<<"FRUSTUM_RIGHT = "<<frustum.getPlane(FRUSTUM_RIGHT).asVec4()<<std::endl;
//			//CRCore::notify(CRCore::NOTICE)<<"FRUSTUM_TOP = "<<frustum.getPlane(FRUSTUM_TOP).asVec4()<<std::endl;
//			//CRCore::notify(CRCore::NOTICE)<<"FRUSTUM_BOTTOM = "<<frustum.getPlane(FRUSTUM_BOTTOM).asVec4()<<std::endl;
//			//CRCore::notify(CRCore::NOTICE)<<"FRUSTUM_FAR = "<<frustum.getPlane(FRUSTUM_FAR).asVec4()<<std::endl;
//			//CRCore::notify(CRCore::NOTICE)<<"FRUSTUM_NEAR = "<<frustum.getPlane(FRUSTUM_NEAR).asVec4()<<std::endl;
//		}
//	}
//protected:
//	double m_zFar;
//	CRCore::ref_ptr<crClipPlane> m_clipPlane;
//	CRCore::ref_ptr<crStateSet> m_stateset;
//};
crExternFileLoadVisitor::crExternFileLoadVisitor(CREncapsulation::crTableIO *scenarioTab):
	CRCore::crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN),
	m_scenarioTab(scenarioTab)
{
}
crExternFileLoadVisitor::~crExternFileLoadVisitor()
{
	m_currentdb = NULL;
	m_pagedLod_cfg_script.Close();
}

void crExternFileLoadVisitor::apply(CRCore::crDB& node)
{
	m_currentdb = &node;
	traverse(node);
}

//void renderTexture(crGroup *node,crTexture2D *texture,int x,int y)
//{
//	crObject* object = new crObject;
//	crGroup *group = new crGroup;
//	crGeometry* geom = createTexturedQuadGeometry(crVector3(0,0,0),crVector3(400.0,0.0,0.0),crVector3(0.0,300.0,0.0));
//	geom->getOrCreateStateSet()->setTextureAttributeAndModes(0,texture,crStateAttribute::ON);
//	geom->getOrCreateStateSet()->setMode(GL_LIGHTING,crStateAttribute::OFF);
//	object->addDrawable(geom);
//
//	crCameraNode* camera = new crCameraNode;
//  camera->setEnableIntersect(false);
//	// set the projection matrix
//	camera->setProjectionMatrix(crMatrix::ortho2D(0,400,0,300));
//	camera->setBufProjectionMatrix(crMatrix::ortho2D(0,400,0,300));
//	// set the view matrix    
//	camera->setReferenceFrame(crTransform::RELATIVE_TO_ABSOLUTE);
//	camera->setViewMatrix(crMatrix::identity());
//	camera->setBufViewMatrix(crMatrix::identity());
//
//	camera->setViewport(x,y,400,300);
//
//	// only clear the depth buffer
//	camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	// draw subgraph after main camera view.
//	camera->setRenderOrder(crCameraNode::POST_RENDER);
//
//	camera->addChild(object);
//
//	group->addChild(camera);
//	group->setEnableIntersect(false);
//	node->addChild(group);
//}

//
void crExternFileLoadVisitor::apply(CRCore::crExternNode& node)
{
	std::string command = node.getDescriptionsInString();
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
	std::string strbuf;
	crVector3 vec3;
	crVector2 vec2;
	float flt1;
	int int1;
	CRCore::crGroup *group = node.getParent(0);
	ref_ptr<crNode> newNode;
	//if(command.find("#SoundNode")!=-1)
	//{
	//	setNewNode(CRAL::crSoundManager::instance()->createSound(command));

	//	//if(dynamic_cast<const CRPhysics::crMatterObject *>(group))
	//	//	((CRPhysics::crMatterObject *)(group))->setCollideCallback(new CRAL::crSoundCollideCallback);
	//}
	//else 
	if(command.find("#CharacterNode")!=-1)
	{
		//CRCore::notify(CRCore::ALWAYS)<<" #CharacterNode "<< std::endl;
		CRCore::crArgumentParser::readKeyValue(command,"FileName",strbuf);
		CRCore::ref_ptr<CRCore::crGroup> externNode = dynamic_cast<crGroup *>(CRIOManager::readNodeFile(strbuf));
		//CRCore::notify(CRCore::ALWAYS)<<" #CharacterNode getfilename "<<node.getfilename().c_str()<<" externNode = "<<externNode.get()<< std::endl;

		if(externNode.valid())
		{
			CRCore::ref_ptr<rbody::CreBodyNode> creBodyNode = dynamic_cast<rbody::CreBodyNode *>(externNode->getChild(0));
			if(!creBodyNode.valid()) return;
			//creBodyNode->setUpdateMode(creBodyNode->getUpdateMode() ^ rbody::CreBodyNode::UPDATE_GROUND_FOLLOWING);
			//creBodyNode->setEnableIntersect(false);

			externNode->removeChild(creBodyNode.get());
			std::string name = creBodyNode->getName();
			setNewNode(creBodyNode.get());
			creBodyNode->setName(name);//恢复creBodyNode的name
			if(command.find("#ScriptData")==std::string::npos)
			    creBodyNode->addDescription("\n#ScriptData");

			std::string default_act;
			if(!CRCore::crArgumentParser::readKeyValue(command,"default_act",default_act))
			{
				default_act = creBodyNode->getBody()->getCoreBody()->getDefaultActivity();
			}
			rbody::ActionRequest* action = creBodyNode->getBody()->getActionPrototype( default_act );
			if (action) 
			{
				action->setPrioritized(false);
				creBodyNode->getBody()->executeAction(action, false);
			}
			//CRCore::crMatrix m;
			//if(!CRCore::crArgumentParser::readKeyValue(command,"scale",flt1))
			//{
   //             flt1 = creBodyNode->getBody()->getScale();
			//}
			//else
			//{
			//	creBodyNode->getBody()->setScale(flt1);
			//}
			//if(!CRCore::crArgumentParser::readKeyValue(command,"trans_offset",vec3))
			//{
			//	vec3 = creBodyNode->getBody()->getCoreBody()->getTransOffset();
			//}
			//else
			//{
			//	creBodyNode->getBody()->getCoreBody()->setTransOffset(vec3);
			//}
			//m.makeScale(flt1, flt1, flt1);
			//if(!CRCore::crArgumentParser::readKeyValue(command,"rot_angle",flt1))
			//{
			//	flt1 = creBodyNode->getBody()->getCoreBody()->getRotAngle();
			//}
			//else
			//{
			//	creBodyNode->getBody()->getCoreBody()->setRotAngle(flt1);
			//}
			//m.postMult(CRCore::crMatrix::rotate(CRCore::DegreesToRadians(flt1),CRCore::Z_AXIS));
			//m.setTrans(vec3);
			//CRCore::crMatrixTransform *bodyTransform = creBodyNode->getBodyTransform();
			//assert(!bodyTransform);
   //         bodyTransform->setMatrix(m);
		}
		else
		{
			return;
		}
	}
	else if(command.find("#TraverseStringNode")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#TraverseStringNode");
		command.erase(startpos,20);
		setNewNode(new crTraverseStringNode);
	}
	//else if(command.find("#PagedLOD")!=-1)
	//{///////////与covertnode 冲突，需要解决
	//	ref_ptr<crPagedLOD> pagedLOD = new crPagedLOD;

	//	setNewNode(pagedLOD.get());

	//	CREncapsulation::loadPagedLodCfg(pagedLOD.get(),m_pagedLod_cfg_script);
	//}
	else if(command.find("#MoveEarthySkyWithEyePointTransform")!=-1)
	{
		ref_ptr<crTransform> transform = new MoveEarthySkyWithEyePointTransform;
		//transform->setCullingActive(false);
		setNewNode(transform.get());
	}
	else if(command.find("#MoveSunWithEyePointTransform")!=-1)
	{
		ref_ptr<crTransform> transform = new MoveSunWithEyePointTransform;
		//transform->setCullingActive(false);
		setNewNode(transform.get());
	}
	else if(command.find("#HDR")!=-1)
	{//hdr
		crDisplaySettings *ds = crDisplaySettings::instance();
		int ihdr = ds->getHDR();
		int maxAnisotropy = ds->getMaxAnisotropy();

		////Z Pre-Pass 
		//float w = ds->getScreenWidth();
		//float h = ds->getScreenHeight();
		//ref_ptr<crCameraNode> prePasscamera = new crCameraNode;
		//prePasscamera->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER_OBJECT);
		//prePasscamera->setRenderOrder(crCameraNode::POST_RENDER);
		//prePasscamera->setComputeNearFarMode(crCameraNode::DO_NOT_COMPUTE_NEAR_FAR);
		//prePasscamera->setDrawBuffer(GL_BACK);
		//prePasscamera->setReadBuffer(GL_BACK);
		//prePasscamera->setRenderMode(crCameraNode::ZprePassRender);
		//prePasscamera->setReferenceFrame(crCameraNode::RELATIVE_TO_PARENTS);
		//prePasscamera->setCullingMode(crCameraNode::VIEW_FRUSTUM_SIDES_CULLING|crCameraNode::SMALL_FEATURE_CULLING|crCameraNode::CLUSTER_CULLING);
		//prePasscamera->setProjectionMatrix(crMatrix::identity());
		//prePasscamera->setBufProjectionMatrix(crMatrix::identity());
		//prePasscamera->setViewMatrix(crMatrix::identity());
		//prePasscamera->setBufViewMatrix(crMatrix::identity());
		//prePasscamera->setViewport(0,0,w,h);
		//prePasscamera->getColorMask()->setMask(false,false,false,false);
		//ref_ptr<crTexture2D> depthSample = new crTexture2D;
		//depthSample->setTextureSize(w,h);
		//depthSample->setInternalFormat(GL_ALPHA32F_ARB);//GL_RGBA16F_ARB
		//depthSample->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
		//depthSample->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
		//depthSample->setMaxAnisotropy(maxAnisotropy);
		//depthSample->setWrap(crTexture2D::WRAP_S, crTexture2D::CLAMP_TO_EDGE);
		//depthSample->setWrap(crTexture2D::WRAP_T, crTexture2D::CLAMP_TO_EDGE);
		//prePasscamera->attach(crCameraNode::COLOR_BUFFER0, depthSample.get(),0,0,false);

		if(ihdr)
		{
			//CRCore::notify(CRCore::FATAL)<<"#HDR... "<<std::endl;
			float w = ds->getViewWidth();
            float h = ds->getViewHeight();
			ref_ptr<crCameraNode> camera = new crCameraNode;
			camera->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER_OBJECT);
			camera->setRenderOrder(crCameraNode::POST_RENDER);
			camera->setComputeNearFarMode(crCameraNode::DO_NOT_COMPUTE_NEAR_FAR);
			camera->setDrawBuffer(GL_BACK);
			camera->setReadBuffer(GL_BACK);
			camera->setReferenceFrame(crCameraNode::RELATIVE_TO_PARENTS);
			camera->setCullingMode(crCameraNode::VIEW_FRUSTUM_SIDES_CULLING|crCameraNode::SMALL_FEATURE_CULLING|crCameraNode::CLUSTER_CULLING);
			camera->setProjectionMatrix(crMatrix::identity());
			camera->setBufProjectionMatrix(crMatrix::identity());
			camera->setViewMatrix(crMatrix::identity());
			camera->setBufViewMatrix(crMatrix::identity());
			camera->setViewport(0,0,w,h);

			ref_ptr<crTexture2D> ldr = new crTexture2D;
			ldr->setTextureSize(w,h);
			ldr->setInternalFormat(ihdr==2?GL_RGBA32F_ARB:GL_RGBA16F_ARB);//GL_RGBA16F_ARB
			//ldr->setSourceFormat(GL_RGB);
			//ldr->setSourceType(GL_FLOAT);
			ldr->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
			ldr->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
			ldr->setMaxAnisotropy(maxAnisotropy);
			ldr->setWrap(crTexture2D::WRAP_S, crTexture2D::CLAMP_TO_EDGE);
			ldr->setWrap(crTexture2D::WRAP_T, crTexture2D::CLAMP_TO_EDGE);
			//ldr->setDrawCallback(crOglTexture2DDraw::getInstance());
//renderTexture(camera.get(),ldr.get(),0,0);

			ref_ptr<crTexture2D> hdr = new crTexture2D;
			hdr->setTextureSize(w,h);
			hdr->setInternalFormat(ihdr==2?GL_RGBA32F_ARB:GL_RGBA16F_ARB);//GL_RGBA16F_ARB
			//hdr->setSourceFormat(GL_RGB);
			//hdr->setSourceType(GL_FLOAT);
			hdr->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
			hdr->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
			hdr->setMaxAnisotropy(maxAnisotropy);
			hdr->setWrap(crTexture2D::WRAP_S, crTexture2D::CLAMP_TO_EDGE);
			hdr->setWrap(crTexture2D::WRAP_T, crTexture2D::CLAMP_TO_EDGE);
			//hdr->setDrawCallback(crOglTexture2DDraw::getInstance());
//renderTexture(camera.get(),hdr.get(),0,305);

			int samples = 0;//= ds->getNumMultiSamples();
            int colorSamples = 0;
			camera->attach(crCameraNode::COLOR_BUFFER0, ldr.get(),0,0,false,samples,colorSamples);
			camera->attach(crCameraNode::COLOR_BUFFER1, hdr.get(),0,0,false,samples,colorSamples);

			ref_ptr<crTexture2D> normalDepth;// = new crTexture2D;
			if(ds->getNeedNormalDepthSample())
			{
				normalDepth = new crTexture2D;
				normalDepth->setTextureSize(w,h);
				normalDepth->setInternalFormat(ihdr==2?GL_RGBA32F_ARB:GL_RGBA16F_ARB);//GL_RGBA16F_ARB
				normalDepth->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
				normalDepth->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
				normalDepth->setMaxAnisotropy(maxAnisotropy);
				normalDepth->setWrap(crTexture2D::WRAP_S, crTexture2D::CLAMP_TO_EDGE);
				normalDepth->setWrap(crTexture2D::WRAP_T, crTexture2D::CLAMP_TO_EDGE);
				camera->attach(crCameraNode::COLOR_BUFFER2, normalDepth.get(),0,0,false,samples,colorSamples);
				//renderTexture(camera.get(),normalDistortion.get(),0,0);
			}

			//////v_blur
			//float bw = w;
            //float bh = h;
			float glowSize = ds->getGlowSize();
			ref_ptr<crTexture2D> vblur_tex = new crTexture2D;
			vblur_tex->setTextureSize(w/glowSize,h/glowSize);
			vblur_tex->setInternalFormat(ihdr==2?GL_RGBA32F_ARB:GL_RGBA16F_ARB);//GL_RGBA16_EXT
			//vblur_tex->setSourceFormat(GL_RGB);
			//vblur_tex->setSourceType(GL_FLOAT);
			vblur_tex->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
			vblur_tex->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
			vblur_tex->setMaxAnisotropy(maxAnisotropy);
			vblur_tex->setWrap(crTexture2D::WRAP_S, crTexture2D::CLAMP_TO_EDGE);
			vblur_tex->setWrap(crTexture2D::WRAP_T, crTexture2D::CLAMP_TO_EDGE);
//renderTexture(camera.get(),vblur_tex.get(),405,0);
			ref_ptr<crCameraNode> vblur_camera = new crCameraNode;
			vblur_camera->setEnableIntersect(false);
			vblur_camera->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER_OBJECT);
			vblur_camera->setComputeNearFarMode(crCameraNode::DO_NOT_COMPUTE_NEAR_FAR);
			vblur_camera->setRenderOrder(crCameraNode::POST_RENDER);
			vblur_camera->setDrawBuffer(GL_BACK);
			vblur_camera->setReadBuffer(GL_BACK);
			vblur_camera->setReferenceFrame(crCameraNode::RELATIVE_TO_ABSOLUTE);
			vblur_camera->setCullingMode(crCameraNode::NO_CULLING);
		
			vblur_camera->setProjectionMatrix(crMatrix::identity()/*crMatrix::ortho2D(0,w,0,h)*/);
			vblur_camera->setBufProjectionMatrix(crMatrix::identity()/*crMatrix::ortho2D(0,w,0,h)*/);
			vblur_camera->setViewMatrix(crMatrix::identity());
			vblur_camera->setBufViewMatrix(crMatrix::identity());
		
			vblur_camera->setViewport(0,0,w/glowSize,h/glowSize);
		
			vblur_camera->attach(crCameraNode::COLOR_BUFFER, vblur_tex.get(),0,0,false);
		
			ref_ptr<crStateSet> vblur_ss = new crStateSet;
			crStateSet *shader = crShaderManager::getInstance()->getShaderStateSet("gaussian");
			
			crUniform *uniform = vblur_ss->getOrCreateUniform("invSize",CRCore::crUniform::FLOAT_VEC2);
			uniform->setDataVariance(crBase::STATIC);
			uniform->set(crVector2(glowSize / w, 0));

			//uniform = vblur_ss->getOrCreateUniform("hdrParam",CRCore::crUniform::FLOAT_VEC4);
			//uniform->setDataVariance(crBase::STATIC);
			//uniform->set(ds->getHdrParam());

			vblur_ss->setTextureAttributeAndModes(0,hdr.get(),crStateAttribute::ON);
			vblur_ss->merge(*shader);
		
			ref_ptr<crObject> object = new crObject;
			object->setEnableIntersect(false);
			ref_ptr<crGeometry> quad = createTexturedQuadGeometry(crVector3(-1,-1,0),crVector3(2,0.0,0.0),crVector3(0.0,2,0.0));
			object->addDrawable(quad.get());
		    object->setStateSet(vblur_ss.get());
			vblur_camera->addChild(object.get());
		
			//////h_blur
			ref_ptr<crTexture2D> hblur_tex = new crTexture2D;
			hblur_tex->setTextureSize(w/glowSize,h/glowSize);
			hblur_tex->setInternalFormat(ihdr==2?GL_RGBA32F_ARB:GL_RGBA16F_ARB);//GL_RGBA16_EXT
			//hblur_tex->setSourceFormat(GL_RGBA);
			//hblur_tex->setSourceType(GL_FLOAT);
			hblur_tex->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
			hblur_tex->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
			hblur_tex->setMaxAnisotropy(maxAnisotropy);
			hblur_tex->setWrap(crTexture2D::WRAP_S, crTexture2D::CLAMP_TO_EDGE);
			hblur_tex->setWrap(crTexture2D::WRAP_T, crTexture2D::CLAMP_TO_EDGE);
//renderTexture(camera.get(),hblur_tex.get(),405,305);
		
			ref_ptr<crCameraNode> hblur_camera = new crCameraNode;
			hblur_camera->setEnableIntersect(false);
			hblur_camera->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER_OBJECT);
			hblur_camera->setComputeNearFarMode(crCameraNode::DO_NOT_COMPUTE_NEAR_FAR);
			hblur_camera->setRenderOrder(crCameraNode::POST_RENDER);
			hblur_camera->setDrawBuffer(GL_BACK);
			hblur_camera->setReadBuffer(GL_BACK);
			hblur_camera->setReferenceFrame(crCameraNode::RELATIVE_TO_ABSOLUTE);
			hblur_camera->setCullingMode( crCameraNode::NO_CULLING );
		
			hblur_camera->setProjectionMatrix(crMatrix::identity()/*crMatrix::ortho2D(0,w,0,h)*/);
			hblur_camera->setBufProjectionMatrix(crMatrix::identity()/*crMatrix::ortho2D(0,w,0,h)*/);
			hblur_camera->setViewMatrix(crMatrix::identity());
			hblur_camera->setBufViewMatrix(crMatrix::identity());
		
			hblur_camera->setViewport(0,0,w/glowSize,h/glowSize);
		
			hblur_camera->attach(crCameraNode::COLOR_BUFFER, hblur_tex.get(),0,0,false);
		
			ref_ptr<crStateSet> hblur_ss = new crStateSet;

			uniform = hblur_ss->getOrCreateUniform("invSize",CRCore::crUniform::FLOAT_VEC2);
			uniform->setDataVariance(crBase::STATIC);
			uniform->set(crVector2(0, glowSize / h));

			//uniform = hblur_ss->getOrCreateUniform("hdrParam",CRCore::crUniform::FLOAT_VEC4);
			//uniform->setDataVariance(crBase::STATIC);
			//uniform->set(ds->getHdrParam());

			hblur_ss->setTextureAttributeAndModes(0,vblur_tex.get(),crStateAttribute::ON);
			hblur_ss->merge(*shader);
		
			object = new crObject;
			object->setEnableIntersect(false);
			object->addDrawable(quad.get());
			object->setStateSet(hblur_ss.get());
			hblur_camera->addChild(object.get());

			//bool needFilter = false;//ds->isNeedFilter();
			//int separateFilter = 0;//ds->getSeparateFilter();
			////////hdrFinal
			ref_ptr<crStateSet> hdr_ss = new crStateSet;
			ref_ptr<crCameraNode> hdrCamera = new crCameraNode;
			hdrCamera->setEnableIntersect(false);

			ref_ptr<crTexture2D> hdrFinal_tex;
			bool useFxaa = ds->useFXAA();
			if(useFxaa)
			{
				//////hdrFinal_tex
				hdrFinal_tex = new crTexture2D;
				hdrFinal_tex->setTextureSize(w,h);
				hdrFinal_tex->setInternalFormat(ihdr==2?GL_RGBA32F_ARB:GL_RGBA16F_ARB);//GL_RGBA16_EXT
				//hdrFinal_tex->setSourceFormat(GL_RGBA);
				//hdrFinal_tex->setSourceType(GL_FLOAT);
				hdrFinal_tex->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
				hdrFinal_tex->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
				hdrFinal_tex->setMaxAnisotropy(maxAnisotropy);
				hdrFinal_tex->setWrap(crTexture2D::WRAP_S, crTexture2D::CLAMP_TO_EDGE);
				hdrFinal_tex->setWrap(crTexture2D::WRAP_T, crTexture2D::CLAMP_TO_EDGE);

				hdrCamera->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER_OBJECT);
				hdrCamera->attach(crCameraNode::COLOR_BUFFER, hdrFinal_tex.get(),0,0,false);
			}
			else
			{
				hdrCamera->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER);
			}

			hdrCamera->setComputeNearFarMode(crCameraNode::DO_NOT_COMPUTE_NEAR_FAR);
			hdrCamera->setRenderOrder(crCameraNode::POST_RENDER);
			hdrCamera->setDrawBuffer(GL_BACK);
			hdrCamera->setReadBuffer(GL_BACK);
			hdrCamera->setReferenceFrame(crCameraNode::RELATIVE_TO_ABSOLUTE);
			hdrCamera->setCullingMode( crCameraNode::NO_CULLING );

			hdrCamera->setProjectionMatrix(crMatrix::identity()/*crMatrix::ortho2D(0,w,0,h)*/);
			hdrCamera->setBufProjectionMatrix(crMatrix::identity()/*crMatrix::ortho2D(0,w,0,h)*/);
			hdrCamera->setViewMatrix(crMatrix::identity());
			hdrCamera->setBufViewMatrix(crMatrix::identity());
			hdrCamera->setViewport(0,0,w,h);

			object = new crObject;
			object->setEnableIntersect(false);
			object->addDrawable(quad.get());
			object->setStateSet(hdr_ss.get());
			hdrCamera->addChild(object.get());

			node.addChild(vblur_camera.get());
			node.addChild(hblur_camera.get());
			node.addChild(hdrCamera.get());
			vblur_camera->addDescription("#ScriptAddNode");
			vblur_camera->setName("vblur_camera");
			hblur_camera->addDescription("#ScriptAddNode");
			hblur_camera->setName("hblur_camera");
			hdrCamera->addDescription("#ScriptAddNode");
			hdrCamera->setName("hdrCamera");

			//crStateSet *npr_ss = hdr_ss.get();
			//crStateSet *dof_ss = hdr_ss.get();
			//crStateSet *distortion_ss = hdr_ss.get();

			hdr_ss->setTextureAttributeAndModes(0,ldr.get(),crStateAttribute::ON);
			hdr_ss->setTextureAttributeAndModes(1,hblur_tex.get(),crStateAttribute::ON);
			uniform = hdr_ss->getOrCreateUniform("CRE_LDR",CRCore::crUniform::SAMPLER_2D);
			uniform->setDataVariance(crBase::STATIC);
			uniform->set(0);
			uniform = hdr_ss->getOrCreateUniform("CRE_HDR",CRCore::crUniform::SAMPLER_2D);
			uniform->setDataVariance(crBase::STATIC);
			uniform->set(1);
			hdr_ss->setTextureAttributeAndModes(2,hdr.get(),crStateAttribute::ON);
			uniform = hdr_ss->getOrCreateUniform("CRE_Distortion",CRCore::crUniform::SAMPLER_2D);
			uniform->setDataVariance(crBase::STATIC);
			uniform->set(2);
			if(ds->getDepthOfField() || ds->getOutLine())
			{
				hdr_ss->setTextureAttributeAndModes(3,normalDepth.get(),crStateAttribute::ON);
				uniform = hdr_ss->getOrCreateUniform("CRE_NormalDepth",CRCore::crUniform::SAMPLER_2D);
				uniform->setDataVariance(crBase::STATIC);
				uniform->set(3);

				if(ds->getDepthOfField())
				{
					uniform = hdr_ss->getOrCreateUniform("dofFactor",CRCore::crUniform::FLOAT_VEC3);
					uniform->setDataVariance(crBase::STATIC);
					uniform->set(crVector3(crBrain::getInstance()->getDofBlurStart(),1.0f/w,crBrain::getInstance()->getDofBlurFactor()));
				}
				if(ds->getOutLine())
				{
					uniform = hdr_ss->getOrCreateUniform("olColor",CRCore::crUniform::FLOAT_VEC3);
					uniform->setDataVariance(crBase::STATIC);
					uniform->set(ds->getOutLineColor());
					uniform = hdr_ss->getOrCreateUniform("olThreshold",CRCore::crUniform::FLOAT);
					uniform->setDataVariance(crBase::STATIC);
					uniform->set(ds->getOutLineThreshold());
				}
			}
			shader = crShaderManager::getInstance()->getShaderStateSet("final");

			hdr_ss->merge(*shader);

			///SSAO
			//crStateSet *ssao = new crStateSet;
			//shader = crShaderManager::getInstance()->getShaderStateSet("ssao");
			//ssao->merge(*shader);
			//std::string texturefile = "noise.img";
			//crTexture2D *rnm = CRCore::crBrain::getInstance()->getTexture2D(texturefile);
			//if(!rnm)
			//{
			//	rnm = new crTexture2D;
			//	rnm->setImage(0,rnm->getImageDataRequestHandler()->requestImageFile(texturefile));
			//	CRCore::crBrain::getInstance()->insertTexture2D(rnm->getImageNameID(),rnm);
			//}
			//else
			//{
			//	if(CRCore::crBrain::getInstance()->getNeedLoadImage2D(rnm))
			//		rnm->setImage(0,rnm->getImageDataRequestHandler()->requestImageFile(texturefile));
			//}
			//ssao->setTextureAttributeAndModes(0,ldr.get(),crStateAttribute::ON);
			//uniform = ssao->getOrCreateUniform("CRE_LDR",CRCore::crUniform::SAMPLER_2D);
			//uniform->setDataVariance(crBase::STATIC);
			//uniform->set(0);
			//ssao->setTextureAttributeAndModes(1,normalDepth.get(),crStateAttribute::ON);
			//uniform = ssao->getOrCreateUniform("CRE_NormalDepth",CRCore::crUniform::SAMPLER_2D);
			//uniform->setDataVariance(crBase::STATIC);
			//uniform->set(1);
			//ssao->setTextureAttributeAndModes(2,rnm,crStateAttribute::ON);
			//uniform = ssao->getOrCreateUniform("rnm",CRCore::crUniform::SAMPLER_2D);
			//uniform->setDataVariance(crBase::STATIC);
			//uniform->set(2);
			//
			//crBlendColor *blendColor = new crBlendColor;
			//blendColor->setConstantColor(crVector4(0.1,0.1,0.1,1.0));
			//crBlendFunc* trans = new crBlendFunc;
			//trans->setFunction(crBlendFunc::CONSTANT_COLOR,crBlendFunc::ONE);
			//ssao->setAttributeAndModes(blendColor,crStateAttribute::ON);
			//ssao->setAttributeAndModes(trans,crStateAttribute::ON);

			//CRCore::crDepth *depthAttr = new CRCore::crDepth;
			//depthAttr->setFunction(CRCore::crDepth::LEQUAL);
			//depthAttr->setWriteMask(false);
			//ssao->setAttribute(depthAttr,CRCore::crStateAttribute::ON);
			//ssao->setRenderingHint(crStateSet::MULTIRENDER_BIN);

			//object = new crObject;
			//object->setEnableIntersect(false);
			//object->addDrawable(quad.get());
			//object->setStateSet(ssao);
			//hdrCamera->addChild(object.get());
            
			if( useFxaa)
			{//HDR 使用FXAA
				ref_ptr<crStateSet> fxaa_ss = new crStateSet;
				ref_ptr<crCameraNode> fxaaCamera = new crCameraNode;
				fxaaCamera->setEnableIntersect(false);
				fxaaCamera->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER);
				fxaaCamera->setComputeNearFarMode(crCameraNode::DO_NOT_COMPUTE_NEAR_FAR);
				fxaaCamera->setRenderOrder(crCameraNode::POST_RENDER);
				fxaaCamera->setDrawBuffer(GL_BACK);
				fxaaCamera->setReadBuffer(GL_BACK);
				fxaaCamera->setReferenceFrame(crCameraNode::RELATIVE_TO_ABSOLUTE);
				fxaaCamera->setCullingMode( crCameraNode::NO_CULLING );

				fxaaCamera->setProjectionMatrix(crMatrix::identity()/*crMatrix::ortho2D(0,w,0,h)*/);
				fxaaCamera->setBufProjectionMatrix(crMatrix::identity()/*crMatrix::ortho2D(0,w,0,h)*/);
				fxaaCamera->setViewMatrix(crMatrix::identity());
				fxaaCamera->setBufViewMatrix(crMatrix::identity());
				fxaaCamera->setViewport(0,0,w,h);

				fxaa_ss->setTextureAttributeAndModes(0,hdrFinal_tex.get(),crStateAttribute::ON);
				uniform = fxaa_ss->getOrCreateUniform("rt_w",CRCore::crUniform::FLOAT);
				uniform->setDataVariance(crBase::STATIC);
				uniform->set(ds->getViewWidth());
				uniform = fxaa_ss->getOrCreateUniform("rt_h",CRCore::crUniform::FLOAT);
				uniform->setDataVariance(crBase::STATIC);
				uniform->set(ds->getViewHeight());

				uniform = fxaa_ss->getOrCreateUniform("FXAA_SPAN_MAX",CRCore::crUniform::FLOAT);
				uniform->set((float)(ds->getNumMultiSamples()));

				shader = crShaderManager::getInstance()->getShaderStateSet("fxaa");
				fxaa_ss->merge(*shader);

				object = new crObject;
				object->setEnableIntersect(false);
				object->addDrawable(quad.get());
				object->setStateSet(fxaa_ss.get());
				fxaaCamera->addChild(object.get());
				
				node.addChild(fxaaCamera.get());
				fxaaCamera->addDescription("#ScriptAddNode");
				fxaaCamera->setName("FXAACamera");
			}
			
			//if(ds->getNPR())
			//{
			//	uniform = npr_ss->getOrCreateUniform("nprParam",CRCore::crUniform::FLOAT_VEC3);
			//	uniform->setDataVariance(crBase::STATIC);
			//	uniform->set(ds->getNprParam());
			//}
			//if(0)//VolumetricLight
			//{
			//	uniform = distortion_ss->getOrCreateUniform("ScreenLightPos",CRCore::crUniform::FLOAT_VEC2);
			//	uniform->setDataVariance(crBase::DYNAMIC);
			//	uniform->set(crVector2(0.8,0.8));
			//	uniform = distortion_ss->getOrCreateUniform("VolumetricParam",CRCore::crUniform::FLOAT_VEC4);
			//	uniform->setDataVariance(crBase::STATIC);
			//	uniform->set(crVector4(2,1.25,0.1,0.1));
			//}
			setNewNode(camera.get());
			camera->setName("SceneRender");

//#define ZperPass
//#ifdef ZperPass
			if(crDisplaySettings::instance()->getUseZperPass())
				camera->setClearMask(GL_COLOR_BUFFER_BIT);
//#endif
		}
		else
		{
			ref_ptr<crGroup> g = new crGroup;
			setNewNode(g.get());
		}
	}
	else if(command.find("#MatrixTransform")!=-1)
	{//CovertNode
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#MatrixTransform");
		command.erase(startpos,17);
		setNewNode(new crMatrixTransform);
	}
	else if(command.find("#DOF")!=-1)
	{//CovertNode
	    int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#DOF");
		command.erase(startpos,5);
		setNewNode(new crDOFTransform);
	}
	else if(command.find("#LOD")!=-1)
	{//CovertNode
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#LOD");
		command.erase(startpos,5);
		setNewNode(new crLod);
	}
	else if(command.find("#MultiSwitch")!=-1)
	{//CovertNode
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#MultiSwitch");
		command.erase(startpos,13);

		crMultiSwitch *sw = new crMultiSwitch;
		setNewNode(sw);
		sw->setAllChildrenOn(0);
	}
	else if(command.find("#PagedLOD")!=-1)
	{//CovertNode
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#PagedLOD");
		command.erase(startpos,10);

		setNewNode(new crPagedLOD);
	}
	else if(command.find("#Group")!=-1)
	{//CovertNode
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#Group");
		command.erase(startpos,7);

		setNewNode(new crGroup);
	}
	else if(command.find("#Sequence")!=-1)
	{//CovertNode
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#Sequence");
		command.erase(startpos,10);

		setNewNode(new crSequence);
		//newNode->getDescriptions().clear();
	}
	else if(command.find("#MatrixSequence")!=-1)
	{//CovertNode
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#MatrixSequence");
		command.erase(startpos,16);

		setNewNode(new crMatrixSequence);
		//newNode->getDescriptions().clear();
	}
	else if(command.find("#BoxMatterObject")!=-1)
	{//CovertNode
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#BoxMatterObject");
		command.erase(startpos,17);

		ref_ptr<CRPhysics::crBoxMatterObject> matterObject = new CRPhysics::crBoxMatterObject;
		setNewNode(matterObject.get());
		CRUtil::crOptimizer optimizer;
		optimizer.optimize(matterObject.get(),CRUtil::crOptimizer::CHECK_PHYSICS);
	}
	else if(command.find("#SphereMatterObject")!=-1)
	{//CovertNode
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#SphereMatterObject");
		command.erase(startpos,20);

		ref_ptr<CRPhysics::crSphereMatterObject> matterObject = new CRPhysics::crSphereMatterObject;
		setNewNode(matterObject.get());
		CRUtil::crOptimizer optimizer;
		optimizer.optimize(matterObject.get(),CRUtil::crOptimizer::CHECK_PHYSICS);
	}
	else if(command.find("#CCylinderMatterObject")!=-1)
	{//CovertNode
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#CCylinderMatterObject");
		command.erase(startpos,22);

		ref_ptr<CRPhysics::crCCylinderMatterObject> matterObject = new CRPhysics::crCCylinderMatterObject;
		setNewNode(matterObject.get());
		CRUtil::crOptimizer optimizer;
		optimizer.optimize(matterObject.get(),CRUtil::crOptimizer::CHECK_PHYSICS);
	}
	else if(command.find("#MeshMatterObject")!=-1)
	{//CovertNode
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#MeshMatterObject");
		command.erase(startpos,18);

		ref_ptr<CRPhysics::crMeshMatterObject> matterObject = new CRPhysics::crMeshMatterObject;
		setNewNode(matterObject.get());
		CRUtil::crOptimizer optimizer;
		optimizer.optimize(matterObject.get(),CRUtil::crOptimizer::CHECK_PHYSICS);
	}
	else if(command.find("#CarMatterObject")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#CarMatterObject");
		command.erase(startpos,17);

		ref_ptr<CRPhysics::crCarMatterObject> matterObject = new CRPhysics::crCarMatterObject;
		setNewNode(matterObject.get());
		CRUtil::crOptimizer optimizer;
		optimizer.optimize(matterObject.get(),CRUtil::crOptimizer::CHECK_PHYSICS);
	}
	//////Volume
	else if(command.find("#VolumeNode")!=-1)
	{
		crVolumeNode *volumeNode = new CRPhysics::crVolumeNode;
		setNewNode(volumeNode);
		if(CRCore::crArgumentParser::readKeyValue(command,"Enable",int1))
		{
			volumeNode->setEnable(int1);
		}
		if(CRCore::crArgumentParser::readKeyValue(command,"MaxTriggerCount",int1))
		{
			volumeNode->setMaxTriggerCount(int1);
		}
		if(CRCore::crArgumentParser::readKeyValue(command,"ReTriggerDelay",flt1))
		{
			volumeNode->setReTriggerDelay(flt1);
		}
		CRPhysics::crVolumeNodeManager::getInstance()->wantAddVolumeNode(volumeNode);
	}
	else if(command.find("#PolytopeVolumeNode")!=-1)
	{
		crVolumeNode *volumeNode = new CRPhysics::crPolytopeVolumeNode;
		setNewNode(volumeNode);
		if(CRCore::crArgumentParser::readKeyValue(command,"Enable",int1))
		{
			volumeNode->setEnable(int1);
		}
		if(CRCore::crArgumentParser::readKeyValue(command,"MaxTriggerCount",int1))
		{
			volumeNode->setMaxTriggerCount(int1);
		}
		if(CRCore::crArgumentParser::readKeyValue(command,"ReTriggerDelay",flt1))
		{
			volumeNode->setReTriggerDelay(flt1);
		}
		CRPhysics::crVolumeNodeManager::getInstance()->wantAddVolumeNode(volumeNode);
	}
	else if(command.find("#MatrixVolumeNode")!=-1)
	{
		crVolumeNode *volumeNode = new CRPhysics::crMatrixVolumeNode;
		setNewNode(volumeNode);
		if(CRCore::crArgumentParser::readKeyValue(command,"Enable",int1))
		{
			volumeNode->setEnable(int1);
		}
		if(CRCore::crArgumentParser::readKeyValue(command,"MaxTriggerCount",int1))
		{
			volumeNode->setMaxTriggerCount(int1);
		}
		if(CRCore::crArgumentParser::readKeyValue(command,"ReTriggerDelay",flt1))
		{
			volumeNode->setReTriggerDelay(flt1);
		}
		CRPhysics::crVolumeNodeManager::getInstance()->wantAddVolumeNode(volumeNode);
	}
	/////////3DUI
	else if(command.find("#Canvas")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#Canvas");
		command.erase(startpos,8);
		crCanvasNode *canvas = new crCanvasNode;
		setNewNode(canvas);
	}
	else if(command.find("#Widget")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#Widget");
		command.erase(startpos,8);
		crWidgetNode *widget = new crWidgetNode;
		setNewNode(widget);
	}
	else if(command.find("#ButtonWidget")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#ButtonWidget");
		command.erase(startpos,14);
		CRUI::crButtonWidgetNode *widget = new CRUI::crButtonWidgetNode;
		setNewNode(widget);
	}
	else if(command.find("#ScrollBarWidget")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#ScrollBarWidget");
		command.erase(startpos,17);
		CRUI::crScrollBarWidgetNode *widget = new CRUI::crScrollBarWidgetNode;
		setNewNode(widget);
	}
	else if(command.find("#StaticTextWidget")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#StaticTextWidget");
		command.erase(startpos,18);
		CRUI::crStaticTextWidgetNode *widget = new CRUI::crStaticTextWidgetNode;
		setNewNode(widget);
	}
	else if(command.find("#HypertextWidget")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#HypertextWidget");
		command.erase(startpos,17);
		CRUI::crHypertextWidgetNode *widget = new CRUI::crHypertextWidgetNode;
		setNewNode(widget);
	}
	else if(command.find("#EditWidget")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#EditWidget");
		command.erase(startpos,12);
		CRUI::crEditWidgetNode *widget = new CRUI::crEditWidgetNode;
		setNewNode(widget);
	}
	else if(command.find("#ListBoxWidget")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#ListBoxWidget");
		command.erase(startpos,15);
		CRUI::crListBoxWidgetNode *widget = new CRUI::crListBoxWidgetNode;
		setNewNode(widget);
	}
	else if(command.find("#ImageBoxWidget")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#ImageBoxWidget");
		command.erase(startpos,16);
		CRUI::crImageBoxWidgetNode *widget = new CRUI::crImageBoxWidgetNode;
		setNewNode(widget);
	}
	else if(command.find("#CheckBoxWidget")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#CheckBoxWidget");
		command.erase(startpos,16);
		CRUI::crCheckBoxWidgetNode *widget = new CRUI::crCheckBoxWidgetNode;
		setNewNode(widget);
	}
	else if(command.find("#RadioWidget")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#RadioWidget");
		command.erase(startpos,13);
		CRUI::crRadioWidgetNode *widget = new CRUI::crRadioWidgetNode;
		setNewNode(widget);
	}
	else if(command.find("#RadioGroupWidget")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#RadioGroupWidget");
		command.erase(startpos,18);
		CRUI::crRadioGroupWidgetNode *widget = new CRUI::crRadioGroupWidgetNode;
		setNewNode(widget);
	}
	else if(command.find("#ProgressWidget")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#ProgressWidget");
		command.erase(startpos,16);
		CRUI::crProgressWidgetNode *widget = new CRUI::crProgressWidgetNode;
		setNewNode(widget);
	}
	else if(command.find("#SliderWidget")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#SliderWidget");
		command.erase(startpos,14);
		CRUI::crSliderWidgetNode *widget = new CRUI::crSliderWidgetNode;
		setNewNode(widget);
	}
	else if(command.find("#ComboBoxWidget")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#ComboBoxWidget");
		command.erase(startpos,16);
		CRUI::crComboBoxWidgetNode *widget = new CRUI::crComboBoxWidgetNode;
		setNewNode(widget);
	}
	else if(command.find("#ChooseColorWidget")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#ChooseColorWidget");
		command.erase(startpos,19);
		CRUI::crChooseColorWidgetNode *widget = new CRUI::crChooseColorWidgetNode;
		setNewNode(widget);
	}
	else if(command.find("#TableWidget")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#TableWidget");
		command.erase(startpos,13);
		CRUI::crTableWidgetNode *widget = new CRUI::crTableWidgetNode;
		setNewNode(widget);
	}
	else if(command.find("#ListControlWidget")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#ListControlWidget");
		command.erase(startpos,19);
		CRUI::crListControlWidgetNode *widget = new CRUI::crListControlWidgetNode;
		setNewNode(widget);
	}
	else if(command.find("#ScissorWidget")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#ScissorWidget");
		command.erase(startpos,15);
		CRUI::crScissorWidgetNode *widget = new CRUI::crScissorWidgetNode;
		setNewNode(widget);
	}
	else if(command.find("#GroupWidget")!=-1)
	{
		int startpos = command.find("#ExternNode");
		command.erase(startpos,12);
		startpos = command.find("#GroupWidget");
		command.erase(startpos,13);
		CRUI::crGroupWidgetNode *widget = new CRUI::crGroupWidgetNode;
		setNewNode(widget);
	}
	////////////////////////////////////////////
	std::string str;
	if(crArgumentParser::readKeyValue(command,"ShaderManager",str))
	{
		newNode->setStateSet(crShaderManager::getInstance()->getShaderStateSet(str));
	}
	char buf[16];
	std::string cfgFile;
	for(int i = 0; i<10; i++)
	{
		sprintf(buf,"cfgFile%d\0",i);
		if(crArgumentParser::readKeyValue(command,buf,cfgFile))
		{
			CREncapsulation::loadCfgFile(cfgFile,newNode.get());
		}
	}
	std::string script;
	if(crArgumentParser::readKeyValue(command,"EventScript",script))
	{
		CREncapsulation::loadEventScript(script,newNode.get());
	}
	if(m_scenarioTab.valid())
	{
		CREncapsulation::crTableIO::StrVec record;
		if(m_scenarioTab->queryOneRecord(0,newNode->getName(),record)>=0)
		{
			std::string scenariodir = CRNetApp::crGlobalHandle::getInstance()->getScenarioDir();
			CREncapsulation::loadEventScript(scenariodir+record[1],newNode.get());
		}
	}
	//if(CRCore::crArgumentParser::readKeyValue(command,"CollideCallback",str))
	//{
	//	if(str.compare("DeepGroundBoxCollide")==0)
	//	{
	//		newNode->setCollideCallback(CREncapsulation::crGroundBoxCollideCallback::getInstance());
	//	}
	//}

	crVector3 boundCenter,boundLength;
	if(crArgumentParser::readKeyValue(command,"BoundCenter",boundCenter) && crArgumentParser::readKeyValue(command,"BoundLength",boundLength))
	{
        newNode->setBoundBox(boundCenter,boundLength);
	}
	//int hasNodeEvent = 0;
	//CRCore::crArgumentParser::readKeyValue(command,"HasNodeEvent",hasNodeEvent);
	//if(hasNodeEvent)
	//{
	//	CREncapsulation::loadNodeEventCfg(newNode.get());
	//}
	if(newNode.valid())
		traverse(*newNode);
	//apply((crNode&)(*newNode));
}

//class MoveSunWithEyePointTransform : public crTransform
//{
//public:
//	/** Get the transformation matrix which moves from local coords to world coords.*/
//	virtual bool computeLocalToWorldMatrix(crMatrix& matrix,crNodeVisitor* nv) const 
//	{
//		CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
//		if (cv)
//		{
//			crVector3 eyePointLocal = cv->getEyeLocal();
//			eyePointLocal.z() = 0.0f;
//			if( (eyePointLocal - m_preEyePointLocal).length() > 50.0f )
//			{
//                m_preEyePointLocal = eyePointLocal;
//			}
//			matrix.preMult(crMatrix::translate(m_preEyePointLocal));
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
//			crVector3 eyePointLocal = cv->getEyeLocal();
//			eyePointLocal.z() = 0.0f;
//			if( (eyePointLocal - m_preEyePointLocal).length() > 50.0f )
//			{
//				m_preEyePointLocal = eyePointLocal;
//			}
//			matrix.postMult(crMatrix::translate(-m_preEyePointLocal));
//		}
//		return true;
//	}
//	mutable crVector3 m_preEyePointLocal;
//};
//
//struct MyCameraPostDrawCallback : public crCameraNode::DrawCallback
//{
//	MyCameraPostDrawCallback(crImage* image):
//	m_image(image),
//    m_count(0)
//	{}
//
//	virtual void operator () (const crCameraNode& /*camera*/) const
//	{
//		const_cast<MyCameraPostDrawCallback *>(this)->m_count++;
//		std::string fileName = m_image->getFileName();
//		std::string file = crArgumentParser::getFileNameEliminateExt(fileName);
//		std::string ext = crArgumentParser::getFileExtension(fileName);
//		char filename[4];
//		sprintf(filename,"%d",m_count);
//		fileName = file + filename + "." + ext;
//	    CRIOManager::writeImageFile(*m_image,fileName);
//	}
//    int m_count;
//	crImage *m_image;
//};

//void hdrblur()
//{
//////blur
//ref_ptr<crTexture2D> blur_tex = new crTexture2D;
//blur_tex->setTextureSize(w,h);
//blur_tex->setInternalFormat(GL_RGB16F_ARB);//GL_RGB16F_ARB
//blur_tex->setSourceFormat(GL_RGB);
//blur_tex->setSourceType(GL_FLOAT);
//blur_tex->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
//blur_tex->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
////hdr_blur0->setMaxAnisotropy(16.0);
//blur_tex->setWrap(crTexture2D::WRAP_S, crTexture2D::CLAMP);
//blur_tex->setWrap(crTexture2D::WRAP_T, crTexture2D::CLAMP);
////renderTexture(camera.get(),blur_tex.get(),250,0);
//ref_ptr<crCameraNode> blurCamera = new crCameraNode;
//blurCamera->setEnableIntersect(false);
//blurCamera->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER_OBJECT);
//blurCamera->setComputeNearFarMode(crCameraNode::DO_NOT_COMPUTE_NEAR_FAR);
//blurCamera->setRenderOrder(crCameraNode::POST_RENDER);
//blurCamera->setDrawBuffer(GL_BACK);
//blurCamera->setReadBuffer(GL_BACK);
//blurCamera->setReferenceFrame(crCameraNode::RELATIVE_TO_ABSOLUTE);
//blurCamera->setCullingMode(crCameraNode::NO_CULLING);
//
//blurCamera->setProjectionMatrix(crMatrix::ortho2D(0,w,0,h));
//blurCamera->setBufProjectionMatrix(crMatrix::ortho2D(0,w,0,h));
//blurCamera->setViewMatrix(crMatrix::identity());
//blurCamera->setBufViewMatrix(crMatrix::identity());
//
//blurCamera->setViewport(0,0,w,h);
//
//blurCamera->attach(crCameraNode::COLOR_BUFFER, blur_tex.get(),0,0,false);
//ref_ptr<crStateSet> blur_ss;
//blur_ss = crShaderManager::getInstance()->getShaderStateSet("blur");
//crUniform *uniform = blur_ss->getOrCreateUniform("blur_offset",CRCore::crUniform::FLOAT_VEC2);
//uniform->setDataVariance(crBase::STATIC);
//uniform->set(crVector2(5.0f / w, 5.0f / h));
//blur_ss->setTextureAttributeAndModes(0,hdr.get(),crStateAttribute::ON);
//blurCamera->setStateSet(blur_ss.get());
//
//ref_ptr<crObject> object = new crObject;
//object->setEnableIntersect(false);
//ref_ptr<crGeometry> quad = createTexturedQuadGeometry(crVector3f(0.0f,0.0f,0.0f),crVector3f(w,0.0f,0.0f),crVector3f(0.0f,h,0.0f));
//object->addDrawable(quad.get());
//
//blurCamera->addChild(object.get());
/////////////////////////////////////////////////////////////
//	ref_ptr<crTexture2D> hdr_blur0 = new crTexture2D;
//	hdr_blur0->setTextureSize(w/4.0f,h/4.0f);
//	hdr_blur0->setInternalFormat(GL_RGB16F_ARB);//GL_RGB16F_ARB
//	hdr_blur0->setSourceFormat(GL_RGB);
//	hdr_blur0->setSourceType(GL_FLOAT);
//	hdr_blur0->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
//	hdr_blur0->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
//	//hdr_blur0->setMaxAnisotropy(16.0);
//	hdr_blur0->setWrap(crTexture2D::WRAP_S, crTexture2D::CLAMP);
//	hdr_blur0->setWrap(crTexture2D::WRAP_T, crTexture2D::CLAMP);
//	//hdr_blur0->setDrawCallback(crOglTexture2DDraw::getInstance());
//	renderTexture(camera.get(),hdr_blur0.get(),250,0);
//	ref_ptr<crCameraNode> blurCamera0 = new crCameraNode;
//	blurCamera0->setEnableIntersect(false);
//	blurCamera0->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER_OBJECT);
//	blurCamera0->setComputeNearFarMode(crCameraNode::DO_NOT_COMPUTE_NEAR_FAR);
//	blurCamera0->setRenderOrder(crCameraNode::POST_RENDER);
//	blurCamera0->setDrawBuffer(GL_BACK);
//	blurCamera0->setReadBuffer(GL_BACK);
//	blurCamera0->setReferenceFrame(crCameraNode::RELATIVE_TO_ABSOLUTE);
//	blurCamera0->setCullingMode(crCameraNode::NO_CULLING);
//
//	blurCamera0->setProjectionMatrix(crMatrix::ortho2D(0,w/4.0f,0,h/4.0f));
//	blurCamera0->setBufProjectionMatrix(crMatrix::ortho2D(0,w/4.0f,0,h/4.0f));
//	blurCamera0->setViewMatrix(crMatrix::identity());
//	blurCamera0->setBufViewMatrix(crMatrix::identity());
//
//	blurCamera0->setViewport(0,0,w/4.0f,h/4.0f);
//
//	blurCamera0->attach(crCameraNode::COLOR_BUFFER, hdr_blur0.get(),0,0,false);
//
//	ref_ptr<crStateSet> blurss0;
//	blurss0 = crShaderManager::getInstance()->getShaderStateSet("gaussian");
//	crUniform *blurUniform0 = blurss0->getOrCreateUniform("invSize",CRCore::crUniform::FLOAT_VEC2);
//	blurUniform0->setDataVariance(crBase::STATIC);
//	blurUniform0->set(crVector2(1.0f / w, 0));
//	blurss0->setTextureAttributeAndModes(0,hdr.get(),crStateAttribute::ON);
//	blurCamera0->setStateSet(blurss0.get());
//
//	ref_ptr<crObject> object = new crObject;
//	object->setEnableIntersect(false);
//	ref_ptr<crGeometry> quad = createTexturedQuadGeometry(crVector3f(0.0f,0.0f,0.0f),crVector3f(w/4.0f,0.0f,0.0f),crVector3f(0.0f,h/4.0f,0.0f));
//	object->addDrawable(quad.get());
//
//	blurCamera0->addChild(object.get());
//	////quad->setDrawCallback( crOglShapeDrawableDraw::getInstance() );
//
//	//////h_blur
//	ref_ptr<crTexture2D> hdr_blur1 = new crTexture2D;
//	hdr_blur1->setTextureSize(w/4.0f,h/4.0f);
//	hdr_blur1->setInternalFormat(GL_RGB16F_ARB);//GL_RGB16F_ARB
//	hdr_blur1->setSourceFormat(GL_RGB);
//	hdr_blur1->setSourceType(GL_FLOAT);
//	hdr_blur1->setFilter(crTexture2D::MIN_FILTER, crTexture2D::LINEAR);
//	hdr_blur1->setFilter(crTexture2D::MAG_FILTER, crTexture2D::LINEAR);
//	//hdr_blur1->setMaxAnisotropy(16.0);
//	hdr_blur1->setWrap(crTexture2D::WRAP_S, crTexture2D::CLAMP);
//	hdr_blur1->setWrap(crTexture2D::WRAP_T, crTexture2D::CLAMP);
//	//hdr_blur1->setDrawCallback(crOglTexture2DDraw::getInstance());
//	renderTexture(camera.get(),hdr_blur1.get(),250,250);
//
//	ref_ptr<crCameraNode> blurCamera1 = new crCameraNode;
//	blurCamera1->setEnableIntersect(false);
//	blurCamera1->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER_OBJECT);
//	blurCamera1->setComputeNearFarMode(crCameraNode::DO_NOT_COMPUTE_NEAR_FAR);
//	blurCamera1->setRenderOrder(crCameraNode::POST_RENDER);
//	blurCamera1->setDrawBuffer(GL_BACK);
//	blurCamera1->setReadBuffer(GL_BACK);
//	blurCamera1->setReferenceFrame(crCameraNode::RELATIVE_TO_ABSOLUTE);
//	blurCamera1->setCullingMode( crCameraNode::NO_CULLING );
//
//	blurCamera1->setProjectionMatrix(crMatrix::ortho2D(0,w/4.0f,0,h/4.0f));
//	blurCamera1->setBufProjectionMatrix(crMatrix::ortho2D(0,w/4.0f,0,h/4.0f));
//	blurCamera1->setViewMatrix(crMatrix::identity());
//	blurCamera1->setBufViewMatrix(crMatrix::identity());
//
//	blurCamera1->setViewport(0,0,w/4.0f,h/4.0f);
//
//	blurCamera1->attach(crCameraNode::COLOR_BUFFER, hdr_blur1.get(),0,0,false);
//
//	ref_ptr<crStateSet> blurss1;
//	blurss1 = crShaderManager::getInstance()->getShaderStateSet("gaussian");
//	crUniform *blurUniform1 = blurss1->getOrCreateUniform("invSize",CRCore::crUniform::FLOAT_VEC2);
//	blurUniform1->setDataVariance(crBase::STATIC);
//	blurUniform1->set(crVector2(0, 1.0f / h));
//	blurss1->setTextureAttributeAndModes(0,hdr_blur0.get(),crStateAttribute::ON);
//	blurCamera1->setStateSet(blurss1.get());
//
//	blurCamera1->addChild(object.get());
//}

//void crExternFileLoadVisitor::loadCfgFile(const std::string &cfgFile, CRCore::crNode *node)
//{
//	rcfg::ConfigScript cfg_script;
//	// Now Open the file
//	if (!cfg_script.Open(cfgFile, c_scriptParserHeader))
//	{
//		CRCore::notify(CRCore::FATAL)<<"crExternFileLoadVisitor::loadCfgFile(): 打开文件"<<cfgFile<<"出错: "<<cfg_script.GetLastError()<<std::endl;
//		return;
//	}
//	cfg_script.Push(crSceneManager::getInstance()->getCurrentSceneName());
//
//	std::string str,str1;
//	std::vector<float> v_i;
//	int int1;
//	float flt1;
// //   CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
//	//crNode *searchedNode = NULL;
//	//if (cfg_script.Push("WaterSimulation", 1)) 
//	//{   
//	//	crWaterSimulationNode *waterSimulationNode = dynamic_cast<crWaterSimulationNode *>(node);
//	//	if(waterSimulationNode)
//	//	{
//	//		int nReflectData = 1;
//	//		crCameraNode *reflectCamera = waterSimulationNode->createReflectCamera();
//
//	//		while(cfg_script.Get("ReflectData", str,nReflectData++))
//	//		{
//	//			searchByNameVisitor.reset();
//	//			searchByNameVisitor.setNameId(str);
//	//			CRCore::crBrain::getInstance()->getCurrentActiveDB()->accept(searchByNameVisitor);
//
//	//			if(searchByNameVisitor.getResult())
//	//			{
// //                   reflectCamera->addChild(searchByNameVisitor.getResult());
//	//			}
//	//		}
//
//	//		waterSimulationNode->createReflectCameraClip(reflectCamera);
// //           cfg_script.Get("material", str);
//	//		if(!str.empty())
// //               loadCfgFile(str,reflectCamera);
//	//	}
//
//	//	if (!cfg_script.Pop())
//	//		CRCore::notify(CRCore::FATAL)<<"crStartHandler::loadInitCfg(): "<<cfg_script.GetLastError()<<std::endl;
//	//}
//	//if (cfg_script.Push("WaterReflectNode", 1)) 
//	//{   
//	//	crWaterReflectNode *waterReflectNode = dynamic_cast<crWaterReflectNode *>(node);
//	//	if(waterReflectNode)
//	//	{
//	//		int nReflectData = 1;
//	//		cfg_script.Get("MirrorPlane", flt1);
//	//		crGroup *new_node = waterReflectNode->create(flt1);
// //           
//	//		while(cfg_script.Get("ReflectData", str,nReflectData++))
//	//		{
//	//			searchByNameVisitor.reset();
//	//			searchByNameVisitor.setNameId(str);
//	//			CRCore::crBrain::getInstance()->getCurrentActiveDB()->accept(searchByNameVisitor);
//
//	//			if(searchByNameVisitor.getResult())
//	//				new_node->addChild(searchByNameVisitor.getResult());
//	//		}
//
//	//		cfg_script.Get("material", str);
//	//		if(!str.empty())
//	//			loadCfgFile(str,waterReflectNode);
//	//	}
//
//	//	if (!cfg_script.Pop())
//	//		CRCore::notify(CRCore::FATAL)<<"crStartHandler::loadInitCfg(): "<<cfg_script.GetLastError()<<std::endl;
//	//}
///*	if (cfg_script.Push("GLSL", 1)) 
//	{   
//		//crDoubleStateNode *doubleStateNode = dynamic_cast<crDoubleStateNode *>(node);
//		int addT = 0;
//		int addB = 0;
//		cfg_script.Get("needTangent", addT);
//		cfg_script.Get("needBinormal", addB);
//		//if(doubleStateNode)
//		//{
//		//	if(cfg_script.Get("sunlight_glsl", str))
//  //              doubleStateNode->setSunLightShaderStateSet(getProgramStateSet(str,addT,addB,doubleStateNode->getSunLightShaderStateSet()));
//
//  //          if(cfg_script.Get("spotlight_glsl", str))
//		//		doubleStateNode->setSpotLightShaderStateSet(getProgramStateSet(str,addT,addB,doubleStateNode->getSpotLightShaderStateSet()));
//		//}
//		//else
//		//{
//            if(cfg_script.Get("glsl", str))
//				node->setStateSet(getProgramStateSet(str,addT,addB,node->getStateSet()));
//		//}
//		if (!cfg_script.Pop())
//			CRCore::notify(CRCore::FATAL)<<"crStartHandler::loadInitCfg(): "<<cfg_script.GetLastError()<<std::endl;
//	}
//	*/
//	int nUniform = 1;
//	ref_ptr<crStateSet> nodestateset;
//	/*ref_ptr<crStateSet> sunlightstateset;
//	ref_ptr<crStateSet> spotlightstateset;*/
//	//crDoubleStateNode *doubleStateNode = dynamic_cast<crDoubleStateNode *>(node);
//	//if(doubleStateNode)
//	//{
// //       sunlightstateset = doubleStateNode->getSunLightShaderStateSet();
// //       spotlightstateset = doubleStateNode->getSpotLightShaderStateSet();
//	//}
//	//else
//    nodestateset = node->getOrCreateStateSet();
//
//	if(cfg_script.Get("Shader", str))
//	{
//        crStateSet *shader = crShaderManager::getInstance()->getShaderStateSet(str);
//        nodestateset->merge(*shader);
//	}
//
//    int nTexture = 1;
//	int tex_unit = 0;
//	ref_ptr<crTexture> texture;
//	crTexture2D *tex2d;
//	crTexture1D *tex1d;
//    crTexture3D *tex3d;
//    crTextureCubeMap *texCubeMap;
//	while(cfg_script.Push("Texture", nTexture++))
//	{		
//		cfg_script.Get("tex_name", str);
//		if(str.compare("envmap")==0)
//		{
//			//texture = crBrain::getInstance()->getCurrentActiveDB()->getDBEnvironmentMap();
//		}
//		else if(str.find("Noise3D")!=-1)
//		{
//			tex3d = CRCore::crBrain::getInstance()->getTexture3D(str);
//			if(!cfg_script.Get("NoiseSample", v_i))
//			{
//				CRCore::notify(CRCore::FATAL)<<"crExternFileLoadVisitor::loadCfgFile(): error has not set NoiseSample, name = "<<str<<std::endl;
//				if(v_i.size()!=4)
//					CRCore::notify(CRCore::FATAL)<<"crExternFileLoadVisitor::loadCfgFile(): error NoiseSample need for size, name = "<<str<<std::endl;
//			}
//			if(!tex3d)
//			{
//				tex3d = CRUtil::crNoise::instance()->make3DNoiseTexture(v_i[0],v_i[1],v_i[2],v_i[3]);
//				CRCore::crBrain::getInstance()->insertTexture3D(str,tex3d);
//			}
//			else
//			{
//				if(CRCore::crBrain::getInstance()->getNeedLoadImage3D(tex3d))
//				{
//					tex3d = CRUtil::crNoise::instance()->make3DNoiseTexture(v_i[0],v_i[1],v_i[2],v_i[3]);
//					CRCore::crBrain::getInstance()->insertTexture3D(str,tex3d);
//				}
//			}
//
//			texture = tex3d;
//		}
//
//		if(!texture.valid())
//		{
//			cfg_script.Get("tex_type", str1);
//
//			if(str1.compare("TEXTURE1D")==0)
//			{
//				tex1d = CRCore::crBrain::getInstance()->getTexture1D(str);
//				if(!tex1d)
//				{
//					tex1d = new crTexture1D;
//					tex1d->setImage(0,tex1d->getImageDataRequestHandler()->requestImageFile(str));
//					CRCore::crBrain::getInstance()->insertTexture1D(tex1d->getImageNameID(),tex1d);
//				}
//				else
//				{
//					if(CRCore::crBrain::getInstance()->getNeedLoadImage1D(tex1d))
//						tex1d->setImage(0,tex1d->getImageDataRequestHandler()->requestImageFile(str));
//				}
//
//				texture = tex1d;
//			}
//			else if(str1.compare("TEXTURE2D")==0)
//			{
//				tex2d = CRCore::crBrain::getInstance()->getTexture2D(str);
//				if(!tex2d)
//				{
//					tex2d = new crTexture2D;
//					tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(str));
//					CRCore::crBrain::getInstance()->insertTexture2D(tex2d->getImageNameID(),tex2d);
//				}
//				else
//				{
//					if(CRCore::crBrain::getInstance()->getNeedLoadImage2D(tex2d))
//						tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(str));
//				}
//
//				texture = tex2d;
//			}
//			else if(str1.compare("TEXTURE3D")==0)
//			{
//				tex3d = CRCore::crBrain::getInstance()->getTexture3D(str);
//				if(!tex3d)
//				{
//					tex3d = new crTexture3D;
//					tex3d->setImage(0,tex3d->getImageDataRequestHandler()->requestImageFile(str));
//					CRCore::crBrain::getInstance()->insertTexture3D(tex3d->getImageNameID(),tex3d);
//				}
//				else
//				{
//					if(CRCore::crBrain::getInstance()->getNeedLoadImage3D(tex3d))
//						tex3d->setImage(0,tex3d->getImageDataRequestHandler()->requestImageFile(str));
//				}
//
//				texture = tex3d;
//			}
//			else if(str1.compare("TEXTURE_CUBE_MAP")==0)
//			{
//				texCubeMap = CRCore::crBrain::getInstance()->getTextureCubeMap(str);
//				if(!texCubeMap)
//				{
//					texCubeMap = new crTextureCubeMap;
//					texCubeMap->getImageDataRequestHandler()->requestCubeImageFile(str,texCubeMap);
//					CRCore::crBrain::getInstance()->insertTextureCubeMap(texCubeMap->getImageNameID(),texCubeMap);
//				}
//				else
//				{
//					if(CRCore::crBrain::getInstance()->getNeedLoadImageCubeMap(texCubeMap))
//					{
//						texCubeMap->getImageDataRequestHandler()->requestCubeImageFile(str,texCubeMap);
//					}
//				}
//
//				texture = texCubeMap;
//			}
//			else if(str1.compare("TEXTURE_RECTANGLE_NV")==0)
//			{
//				//texture = CRCore::crBrain::getInstance()->getTexture(str);
//			}
//		}
//
//		if(!texture.valid()) 
//		{
//			CRCore::notify(CRCore::FATAL)<<"crExternFileLoadVisitor::loadCfgFile(): texture error"<<std::endl;		
//			continue;
//		}
//
//		cfg_script.Get("tex_coord_set", tex_unit);
//
//		cfg_script.Get("tex_wrap_mode", str);
//
//		if(str.compare("CLAMP")==0)
//		{
//			texture->setWrap(crTexture::WRAP_S,crTexture::CLAMP);
//			texture->setWrap(crTexture::WRAP_T,crTexture::CLAMP);
//			texture->setWrap(crTexture::WRAP_R,crTexture::CLAMP);
//		}
//		else if(str.compare("CLAMP_TO_EDGE")==0)
//		{
//			texture->setWrap(crTexture::WRAP_S,crTexture::CLAMP_TO_EDGE);
//			texture->setWrap(crTexture::WRAP_T,crTexture::CLAMP_TO_EDGE);
//			texture->setWrap(crTexture::WRAP_R,crTexture::CLAMP_TO_EDGE);
//		}
//		else if(str.compare("CLAMP_TO_BORDER")==0)
//		{
//			texture->setWrap(crTexture::WRAP_S,crTexture::CLAMP_TO_BORDER);
//			texture->setWrap(crTexture::WRAP_T,crTexture::CLAMP_TO_BORDER);
//			texture->setWrap(crTexture::WRAP_R,crTexture::CLAMP_TO_BORDER);
//		}
//		else if(str.compare("REPEAT")==0)
//		{
//			texture->setWrap(crTexture::WRAP_S,crTexture::REPEAT);
//			texture->setWrap(crTexture::WRAP_T,crTexture::REPEAT);
//			texture->setWrap(crTexture::WRAP_R,crTexture::REPEAT);
//		}
//		else if(str.compare("MIRROR")==0)
//		{
//			texture->setWrap(crTexture::WRAP_S,crTexture::MIRROR);
//			texture->setWrap(crTexture::WRAP_T,crTexture::MIRROR);
//			texture->setWrap(crTexture::WRAP_R,crTexture::MIRROR);
//		}
//
//		cfg_script.Get("min_filter", str);
//		if(str.compare("LINEAR")==0)
//		{
//			texture->setFilter(crTexture::MIN_FILTER,crTexture::LINEAR);
//		}
//		else if(str.compare("LINEAR_MIPMAP_LINEAR")==0)
//		{
//			texture->setFilter(crTexture::MIN_FILTER,crTexture::LINEAR_MIPMAP_LINEAR);
//		}
//		else if(str.compare("LINEAR_MIPMAP_NEAREST")==0)
//		{
//			texture->setFilter(crTexture::MIN_FILTER,crTexture::LINEAR_MIPMAP_NEAREST);
//		}
//		else if(str.compare("NEAREST")==0)
//		{
//			texture->setFilter(crTexture::MIN_FILTER,crTexture::NEAREST);
//		}
//		else if(str.compare("NEAREST_MIPMAP_LINEAR")==0)
//		{
//			texture->setFilter(crTexture::MIN_FILTER,crTexture::NEAREST_MIPMAP_LINEAR);
//		}
//		else if(str.compare("NEAREST_MIPMAP_NEAREST")==0)
//		{
//			texture->setFilter(crTexture::MIN_FILTER,crTexture::NEAREST_MIPMAP_NEAREST);
//		}
//
//		cfg_script.Get("mag_filter", str);
//		if(str.compare("LINEAR")==0)
//		{
//			texture->setFilter(crTexture::MAG_FILTER,crTexture::LINEAR);
//		}
//		else if(str.compare("NEAREST")==0)
//		{
//			texture->setFilter(crTexture::MAG_FILTER,crTexture::NEAREST);
//		}
//
//		crStateAttribute::VRModeValue value = crStateAttribute::ON;
//		if(cfg_script.Get("Mode", str))
//		{
//			if(str.find("OFF") != -1)
//				value |= crStateAttribute::OFF;
//			if(str.find("ON") != -1)
//				value |= crStateAttribute::ON;
//			if(str.find("OVERRIDE") != -1)
//				value |= crStateAttribute::OVERRIDE;
//			if(str.find("PROTECTED") != -1)
//				value |= crStateAttribute::PROTECTED;
//			if(str.find("INHERIT") != -1)
//				value |= crStateAttribute::INHERIT;
//		}
//
//		nodestateset->setTextureAttributeAndModes(tex_unit,texture.get(),value);
//
//		if (!cfg_script.Pop())
//			CRCore::notify(CRCore::FATAL)<<"crExternFileLoadVisitor::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
//	}
//
//    ref_ptr<crUniform> uniform;
//	std::string uniformname;
//	while(cfg_script.Push("UNIFORM", nUniform++))
//	{		
//		cfg_script.Get("name", uniformname);
//		cfg_script.Get("type", str);
//		if(str.compare("FLOAT")==0)
//		{
//            cfg_script.Get("value", flt1);
//			uniform = new crUniform(crUniform::FLOAT,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set(flt1);
//		}
//		else if(str.compare("FLOAT_VEC2")==0)
//		{
//			cfg_script.Get("value", v_i);
//			uniform = new crUniform(crUniform::FLOAT_VEC2,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set(crVector2(v_i[0],v_i[1]));
//		}
//		else if(str.compare("FLOAT_VEC3")==0)
//		{
//			cfg_script.Get("value", v_i);
//			uniform = new crUniform(crUniform::FLOAT_VEC3,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set(crVector3(v_i[0],v_i[1],v_i[2]));
//			//CRCore::notify(CRCore::FATAL)<<"uniform FLOAT_VEC3: "<<uniformname<<" x="<<v_i[0]<<" y="<<v_i[1]<<" z="<<v_i[2]<<std::endl;
//		}
//		else if(str.compare("FLOAT_VEC4")==0)
//		{
//			cfg_script.Get("value", v_i);
//			uniform = new crUniform(crUniform::FLOAT_VEC4,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set(crVector4(v_i[0],v_i[1],v_i[2],v_i[3]));
//			//CRCore::notify(CRCore::FATAL)<<"uniform FLOAT_VEC4: "<<uniformname<<" x="<<v_i[0]<<" y="<<v_i[1]<<" z="<<v_i[2]<<" w="<<v_i[3]<<std::endl;
//		}
//		else if(str.compare("INT")==0)
//		{
//			cfg_script.Get("value", int1);
//			uniform = new crUniform(crUniform::INT,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set(int1);
//			//CRCore::notify(CRCore::FATAL)<<"uniform INT: "<<uniformname<<" x="<<int1<<std::endl;
//		}
//		else if(str.compare("INT_VEC2")==0)
//		{
//			cfg_script.Get("value", v_i);
//			uniform = new crUniform(crUniform::INT_VEC2,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set((int)(v_i[0]),(int)(v_i[1]));
//		}
//		else if(str.compare("INT_VEC3")==0)
//		{
//			cfg_script.Get("value", v_i);
//			uniform = new crUniform(crUniform::INT_VEC3,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set((int)(v_i[0]),(int)(v_i[1]),(int)(v_i[2]));
//		}
//		else if(str.compare("INT_VEC4")==0)
//		{
//			cfg_script.Get("value", v_i);
//			uniform = new crUniform(crUniform::INT_VEC4,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set((int)(v_i[0]),(int)(v_i[1]),(int)(v_i[2]),(int)(v_i[3]));
//		}
//		else if(str.compare("BOOL")==0)
//		{
//			cfg_script.Get("value", int1);
//			uniform = new crUniform(crUniform::BOOL,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set((bool)int1);
//		}
//		else if(str.compare("BOOL_VEC2")==0)
//		{
//			cfg_script.Get("value", v_i);
//			uniform = new crUniform(crUniform::BOOL_VEC2,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set((bool)(v_i[0]),(bool)(v_i[1]));
//		}
//		else if(str.compare("BOOL_VEC3")==0)
//		{
//			cfg_script.Get("value", v_i);
//			uniform = new crUniform(crUniform::BOOL_VEC3,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set((bool)(v_i[0]),(bool)(v_i[1]),(bool)(v_i[2]));
//		}
//		else if(str.compare("BOOL_VEC4")==0)
//		{
//			cfg_script.Get("value", v_i);
//			uniform = new crUniform(crUniform::BOOL_VEC4,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set((bool)(v_i[0]),(bool)(v_i[1]),(bool)(v_i[2]),(bool)(v_i[3]));
//		}
//		else if(str.compare("FLOAT_MAT2")==0)
//		{
//			cfg_script.Get("value", v_i);
//			uniform = new crUniform(crUniform::FLOAT_MAT2,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set(crMatrix2( v_i[0],v_i[1],
//				                    v_i[2],v_i[3]));
//		}
//		else if(str.compare("FLOAT_MAT3")==0)
//		{
//			cfg_script.Get("value", v_i);
//			uniform = new crUniform(crUniform::FLOAT_MAT3,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set(crMatrix3( v_i[0],v_i[1],v_i[2],
//				                    v_i[3],v_i[4],v_i[5],
//									v_i[6],v_i[7],v_i[8]));
//		}
//		else if(str.compare("FLOAT_MAT4")==0)
//		{
//			cfg_script.Get("value", v_i);
//			uniform = new crUniform(crUniform::FLOAT_MAT4,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set(crMatrixf( v_i[ 0],v_i[ 1],v_i[ 2],v_i[ 3],
//				                    v_i[ 4],v_i[ 5],v_i[ 6],v_i[ 7],
//									v_i[ 8],v_i[ 9],v_i[10],v_i[11],
//									v_i[12],v_i[13],v_i[14],v_i[15]));
//		}
//		else if(str.compare("SAMPLER_1D")==0)
//		{
//			cfg_script.Get("value", int1);
//			uniform = new crUniform(crUniform::SAMPLER_1D,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set(int1);
//		}
//		else if(str.compare("SAMPLER_2D")==0)
//		{
//			cfg_script.Get("value", int1);
//			uniform = new crUniform(crUniform::SAMPLER_2D,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set(int1);
//		}
//		else if(str.compare("SAMPLER_3D")==0)
//		{
//			cfg_script.Get("value", int1);
//			uniform = new crUniform(crUniform::SAMPLER_3D,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set(int1);
//		}
//		else if(str.compare("SAMPLER_CUBE")==0)
//		{
//			cfg_script.Get("value", int1);
//			uniform = new crUniform(crUniform::SAMPLER_CUBE,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set(int1);
//		}
//		else if(str.compare("SAMPLER_1D_SHADOW")==0)
//		{
//			cfg_script.Get("value", int1);
//			uniform = new crUniform(crUniform::SAMPLER_1D_SHADOW,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set(int1);
//		}
//		else if(str.compare("SAMPLER_2D_SHADOW")==0)
//		{
//			cfg_script.Get("value", int1);
//			uniform = new crUniform(crUniform::SAMPLER_2D_SHADOW,uniformname);
//			uniform->setDataVariance(crBase::STATIC);
//			uniform->set(int1);
//		}
//		crStateAttribute::VRModeValue value = crStateAttribute::ON;
//		if(cfg_script.Get("Mode", str))
//		{
//			if(str.find("OFF") != -1)
//				value |= crStateAttribute::OFF;
//			if(str.find("ON") != -1)
//				value |= crStateAttribute::ON;
//			if(str.find("OVERRIDE") != -1)
//				value |= crStateAttribute::OVERRIDE;
//			if(str.find("PROTECTED") != -1)
//				value |= crStateAttribute::PROTECTED;
//			if(str.find("INHERIT") != -1)
//				value |= crStateAttribute::INHERIT;
//		}
//		nodestateset->addUniform(uniform.get(),value);
//
//		if (!cfg_script.Pop())
//			CRCore::notify(CRCore::FATAL)<<"crExternFileLoadVisitor::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
//	}
//
//	ref_ptr<crStateAttribute> stateAttribute;
//	int nAttribute = 1;
//	int nParam = 1;
//	while(cfg_script.Push("StateAttribute", nAttribute++))
//	{
//		cfg_script.Get("Type", str);
//		if(str.compare("AlphaFunc") == 0)
//		{
//			stateAttribute = new crAlphaFunc;
//		}
//		else if(str.compare("BlendColor") == 0)
//		{
//			stateAttribute = new crBlendColor;
//		}
//		else if(str.compare("BlendEquation") == 0)
//		{
//			stateAttribute = new crBlendEquation;
//		}
//		else if(str.compare("BlendFunc") == 0)
//		{
//			stateAttribute = new crBlendFunc;
//		}
//		else if(str.compare("ClampColor") == 0)
//		{
//			stateAttribute = new crClampColor;
//		}
//		else if(str.compare("ClipPlane") == 0)
//		{
//			stateAttribute = new crClipPlane;
//		}
//		else if(str.compare("ColorMask") == 0)
//		{
//			stateAttribute = new crColorMask;
//		}
//		else if(str.compare("CullFace") == 0)
//		{
//			stateAttribute = new crCullFace;
//		}
//		else if(str.compare("Depth") == 0)
//		{
//            stateAttribute = new crDepth;
//		}
//		else if(str.compare("Fog") == 0)
//		{
//			stateAttribute = new crFog;
//		}
//		else if(str.compare("FrontFace") == 0)
//		{
//			stateAttribute = new crFrontFace;
//		}
//		else if(str.compare("Hint") == 0)
//		{
//			stateAttribute = new crHint;
//		}
//		else if(str.compare("LightModel") == 0)
//		{
//			stateAttribute = new crLightModel;
//		}
//		else if(str.compare("LogicOp") == 0)
//		{
//			stateAttribute = new crLogicOp;
//		}
//		else if(str.compare("Multisample") == 0)
//		{
//			stateAttribute = new crMultisample;
//		}
//		else if(str.compare("PolygonMode") == 0)
//		{
//			stateAttribute = new crPolygonMode;
//		}
//		else if(str.compare("PolygonOffset") == 0)
//		{
//			stateAttribute = new crPolygonOffset;
//		}
//		else if(str.compare("Scissor") == 0)
//		{
//			stateAttribute = new crScissor;
//		}
//		else if(str.compare("ShadeModel") == 0)
//		{
//			stateAttribute = new crShadeModel;
//		}
//		else if(str.compare("Stencil") == 0)
//		{
//			stateAttribute = new crStencil;
//		}
//		else if(str.compare("StencilTwoSided") == 0)
//		{
//			stateAttribute = new crStencilTwoSided;
//		}
//		else if(str.compare("TexEnv") == 0)
//		{
//			stateAttribute = new crTexEnv;
//		}
//		else if(str.compare("TexEnvCombine") == 0)
//		{
//			stateAttribute = new crTexEnvCombine;
//		}
//		else if(str.compare("TexEnvFilter") == 0)
//		{
//			stateAttribute = new crTexEnvFilter;
//		}
//		else if(str.compare("TexGen") == 0)
//		{
//			stateAttribute = new crTexGen;
//		}
//		else if(str.compare("ViewPort") == 0)
//		{
//			stateAttribute = new crViewPort;
//		}
//		else if(str.compare("Material") == 0)
//		{
//			stateAttribute = new crMaterial;
//		}
//		nParam = 1;
//		while (cfg_script.Get("AttributeParam", str,nParam++))
//		{
//            stateAttribute->addAttributeParam(nParam-2,str);
//			//CRCore::notify(CRCore::FATAL)<<stateAttribute->className()<<" "<<str<<std::endl;
//		}
//		crStateAttribute::VRModeValue value = crStateAttribute::ON;
//        if(cfg_script.Get("Mode", str))
//		{
//			if(str.find("OFF") != -1)
//				value |= crStateAttribute::OFF;
//			if(str.find("ON") != -1)
//				value |= crStateAttribute::ON;
//			if(str.find("OVERRIDE") != -1)
//				value |= crStateAttribute::OVERRIDE;
//			if(str.find("PROTECTED") != -1)
//				value |= crStateAttribute::PROTECTED;
//			if(str.find("INHERIT") != -1)
//				value |= crStateAttribute::INHERIT;
//		}
//		if(nodestateset.valid())
//		{
//			nodestateset->setAttributeAndModes(stateAttribute.get(),value);
//		}
//		if (!cfg_script.Pop())
//			CRCore::notify(CRCore::FATAL)<<"crExternFileLoadVisitor::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
//	}
//	if(cfg_script.Push("RenderBinDetails"))
//	{
//		cfg_script.Get("BinNum", int1);
//        cfg_script.Get("BinName", str);
//		nodestateset->setRenderBinDetails(int1,str);
//		if (!cfg_script.Pop())
//			CRCore::notify(CRCore::FATAL)<<"crExternFileLoadVisitor::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
//	}
//	ref_ptr<crNodeCallback> nodeCallback;
//	int nCallback = 1;
//	while(cfg_script.Push("NodeCallback", nCallback++))
//	{
//		cfg_script.Get("Class", str);
//		if(str.compare("WaveBlend") == 0)
//		{
//			if(crDisplaySettings::instance()->getHDR() && crDisplaySettings::instance()->getWaveBlend())
//			{
//				nodeCallback = new CREncapsulation::crWaveBlendCallback;
//			}
//			else
//			{
//				if (!cfg_script.Pop())
//					CRCore::notify(CRCore::FATAL)<<"crExternFileLoadVisitor::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
//				continue;
//			}
//		}
//		else if(str.compare("ResetWaveBlend") == 0)
//		{
//			if(crDisplaySettings::instance()->getHDR() && crDisplaySettings::instance()->getWaveBlend())
//			{
//				nodeCallback = new CREncapsulation::crResetWaveBlendCallback;
//			}
//			else
//			{
//				if (!cfg_script.Pop())
//					CRCore::notify(CRCore::FATAL)<<"crExternFileLoadVisitor::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
//				continue;
//			}
//		}
//		else
//		{
//			nodeCallback = dynamic_cast<crNodeCallback *>(crEventCallbackManager::getInstance()->getEventCallback(str));
//			if(!nodeCallback) 
//			{
//				CRCore::notify(CRCore::NOTICE)<<"crExternFileLoadVisitor::loadCfgFile: 未找到NodeCallback: "<<str<<std::endl;
//				continue;
//			}
//		}
//		//if(str.compare("SunInEyesight") == 0)
//		//{
//		//	nodeCallback = new CREncapsulation::crSunInEyesightCallback;
//		//}
//		//else if(str.compare("DofTexMat") == 0)
//		//{
//		//	nodeCallback = new CREncapsulation::crDofTexMatCallback;
//		//}
//		//else if(str.compare("SunFlares") == 0)
//		//{
//		//	nodeCallback = new CREncapsulation::crSunFlaresCallback;
//		//}
//		//else if(str.compare("MoveBillboardWithEye") == 0)
//		//{
//		//	nodeCallback = new CREncapsulation::crMoveBillboardWithEyeCallback;
//		//}
//		//else 
//		//else if(str.compare("ParticleBind") == 0)
//		//{
//		//	nodeCallback = new CREncapsulation::crParticleBindCallback;
//		//}
//		//else if(str.compare("SequenceBind") == 0)
//		//{
//		//	nodeCallback = new CREncapsulation::crSequenceBindCallback;
//		//}
//		//else if(str.compare("CompassH") == 0)
//		//{
//		//	nodeCallback = new CREncapsulation::crCompassHCallback;
//		//}
//		//else if(str.compare("CompassP") == 0)
//		//{
//		//	nodeCallback = new CREncapsulation::crCompassPCallback;
//		//}
//		//else if(str.compare("CompassR") == 0)
//		//{
//		//	nodeCallback = new CREncapsulation::crCompassRCallback;
//		//}
//		//else if(str.compare("ScalarBar") == 0)
//		//{
//		//	nodeCallback = new CREncapsulation::crScalarBarCallback;
//		//}
//		//else if(str.compare("VisableTimer") == 0)
//		//{
//		//	nodeCallback = new CREncapsulation::crVisableTimerCallback;
//		//}
//		//else if(str.compare("FlashTimer") == 0)
//		//{
//		//	nodeCallback = new CREncapsulation::crFlashTimerCallback;
//		//}
//		//else if(str.compare("BoneMatrix") == 0)
//		//{
//		//	nodeCallback = new CREncapsulation::crBoneMatrixCallback;
//		//}
//        if(nodeCallback.valid())
//		{
//			cfg_script.Get("Type", str);
//			if(str.compare("CullCallback")==0)
//			{
//				node->addCullCallback(nodeCallback.get());
//			}
//			else if(str.compare("UpdateCallback")==0)
//			{
//				node->addUpdateCallback(nodeCallback.get());
//			}
//			nParam = 1;
//			while (cfg_script.Get("Param", str,nParam++))
//			{
//				nodeCallback->addEventParam(nParam-2,str);
//			}
//		}
//		else
//		{
//			CRCore::notify(CRCore::FATAL)<<"crExternFileLoadVisitor::loadCfgFile(): not find NodeCallback of "<<str<<std::endl;
//		}
//		if (!cfg_script.Pop())
//			CRCore::notify(CRCore::FATAL)<<"crExternFileLoadVisitor::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
//	}
//
//	ref_ptr<crCollideCallback> collideCallback;
//	int nCollideCallback = 1;
//	while(cfg_script.Push("CollideCallback", nCollideCallback++))
//	{
//		cfg_script.Get("Class", str);
//		collideCallback = dynamic_cast<crCollideCallback *>(crEventCallbackManager::getInstance()->getEventCallback(str));
//		if(!collideCallback) 
//		{
//			CRCore::notify(CRCore::NOTICE)<<"crExternFileLoadVisitor::loadCfgFile: 未找到CollideCallback: "<<str<<std::endl;
//			continue;
//		}
//		if(collideCallback.valid())
//		{
//			node->addCollideCallback(collideCallback.get());
//			nParam = 1;
//			while (cfg_script.Get("Param", str,nParam++))
//			{
//				nodeCallback->addEventParam(nParam-2,str);
//			}
//		}
//		else
//		{
//			CRCore::notify(CRCore::FATAL)<<"crExternFileLoadVisitor::loadCfgFile(): not find CollideCallback of "<<str<<std::endl;
//		}
//		if (!cfg_script.Pop())
//			CRCore::notify(CRCore::FATAL)<<"crExternFileLoadVisitor::loadCfgFile(): "<<cfg_script.GetLastError()<<std::endl;
//	}
//
//
//	cfg_script.Pop();
//}
//
struct TexMatCallback : public crNodeCallback
{
public:

	TexMatCallback(crTexMat& tm) :
	  m_texMat(tm)
	  {
	  }
	TexMatCallback(TexMatCallback& tmc) :
      crNodeCallback(tmc),
	  m_texMat(tmc.m_texMat)
	{
	}
	virtual void operator()(crNode* node, crNodeVisitor* nv)
	{
		CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
		if (cv)
		{
			const crMatrix& MV = cv->getModelViewMatrix();
			const crMatrix R = crMatrix::rotate( CRCore::DegreesToRadians(112.0f), 0.0f,0.0f,1.0f)*
				crMatrix::rotate( CRCore::DegreesToRadians(90.0f), 1.0f,0.0f,0.0f);

			crQuat q;
			MV.get(q);
			const crMatrix C = crMatrix::rotate( q.inverse() );

			m_texMat.setMatrix( C * R );
		}

		traverse(node,nv);
	}

	crTexMat& m_texMat;
};

char sky_vp[] = 
"uniform mat4 mvp; \n"
"varying vec3 cubeCoord; \n"
"void main() \n"
"{ \n"
"   gl_Position = mvp * gl_Vertex; \n"
"   cubeCoord = gl_Vertex.xyz; \n"
"} \n";

char sky_fp[] = 
"uniform samplerCube Skybox; \n"
"varying vec3 cubeCoord; \n"
"void main() \n"
"{ \n"
"	gl_FragColor = textureCube(Skybox, cubeCoord); \n"
"} \n";

class crSkyCameraUpdateUniformCallback : public crNodeCallback
{
public:
	virtual void operator()(crNode* node, crNodeVisitor* nv)
	{
		CRCore::notify(CRCore::ALWAYS)<<"crSkyCameraUpdateUniformCallback"<<std::endl;

		crCameraNode *skyCamera = dynamic_cast<crCameraNode *>(node);
		crMatrixd *modelview = skyCamera->getBufViewMatrix();
		crMatrixd *projection = skyCamera->getBufProjectionMatrix();
		modelview->setTrans(0.0,-1.0,1.0);
		crUniform *uniform = skyCamera->getStateSet()->getUniform("mvp");
		uniform->set( crMatrix::pegToFarPlane( *projection * *modelview ) );
		modelview->makeIdentity();
		projection->makeIdentity();
		traverse(node,nv);
	}
};

void crExternFileLoadVisitor::apply(CRCore::crGroup& node)
{
	std::string command = node.getDescriptionsInString();
	if(command.find("#ExternNode")!=-1)
	{
		ref_ptr<crExternNode> newNode = new crExternNode;
		if(node.getNumParents()==1)
		{
			CRCore::crGroup *group = node.getParent(0);		
			setNewNode(newNode.get());
		}
		else
		{
			newNode->setName(node.getName());
			newNode->setVisiable(node.getVisiable());
			newNode->setCalcShadow(node.isCalcShadow());
			newNode->setEnableIntersect(node.getEnableIntersect());
			newNode->setRenderable(node.getRenderable());
			newNode->setCollideMode(node.getCollideMode());
			newNode->setCullingActive(node.getCullingActive());
			newNode->setDescriptionsInString(command);
			crGroup* newGroup = dynamic_cast<crGroup *>(newNode.get());
			if(newGroup) 
			{
			    for(int i = 0; i<node.getNumChildren(); i++)
					newGroup->addChild(node.getChild(i));
				node.removeChild(0,node.getNumChildren());
			}
			node.addChild(newNode.get());
			node.setName("LoadNodeDummy");
			node.getDescriptions().clear();
		}
	    apply(*newNode);
		return;
	}
	std::string str;

	//if( node.getName().compare("skybox") == 0 ||
	//    (crArgumentParser::readKeyValue(command,"[NameID]",str) && str.compare("skybox")==0))
	//{
	//	if(crArgumentParser::readKeyValue(command,"cubemapImageName",str) && !str.empty())
	//	{
	//		CRCore::crTextureCubeMap *texCubeMap = CRCore::crBrain::getInstance()->getTextureCubeMap(str);

	//		if(texCubeMap)
	//		{
	//			if(CRCore::crBrain::getInstance()->getNeedLoadImageCubeMap(texCubeMap))
	//				texCubeMap->getImageDataRequestHandler()->requestCubeImageFile(str,texCubeMap);
	//		}
	//		else
	//		{
	//			texCubeMap = new CRCore::crTextureCubeMap;
	//			texCubeMap->setWrap(CRCore::crTexture::WRAP_S, CRCore::crTexture::CLAMP_TO_EDGE);
	//			texCubeMap->setWrap(CRCore::crTexture::WRAP_T, CRCore::crTexture::CLAMP_TO_EDGE);
	//			texCubeMap->setWrap(CRCore::crTexture::WRAP_R, CRCore::crTexture::CLAMP_TO_EDGE);

	//			texCubeMap->setFilter(CRCore::crTexture::MIN_FILTER, CRCore::crTexture::LINEAR_MIPMAP_NEAREST);
	//			texCubeMap->setFilter(CRCore::crTexture::MAG_FILTER, CRCore::crTexture::LINEAR);

	//			texCubeMap->getImageDataRequestHandler()->requestCubeImageFile(str,texCubeMap);

	//			CRCore::crBrain::getInstance()->insertTextureCubeMap(str,texCubeMap);
	//		}

	//		crObject *object = new crObject;

	//		crShapeDrawable *skyshape = new crShapeDrawable(new /*crSphere*/crBox(crVector3(0.0f,0.0f,0.0f),2.0));
	//		crTessellationHints *hint = new crTessellationHints;
	//		hint->setCreateBackFace(true);
	//		hint->setCreateFrontFace(false);
	//		//hint->setCreateNormals(false);
	//		//hint->setDetailRatio(0.5);
	//		skyshape->setTessellationHints(hint);
	//		object->addDrawable(skyshape);

	//		crStateSet *ss = object->getOrCreateStateSet();

	//		//if(m_currentdb.valid())
	//		//	m_currentdb->setDBEnvironmentMap(texCubeMap);
	//		//else
	//		//	crBrain::getInstance()->getCurrentActiveDB()->setDBEnvironmentMap(texCubeMap);

	//		crTexEnv* te = new crTexEnv;
	//		te->setMode(crTexEnv::REPLACE);
	//		ss->setTextureAttributeAndModes(TEXTURE_ENVMAP, te, crStateAttribute::ON);

	//		CRCore::crTexGen* texgen = new CRCore::crTexGen;
	//		texgen->setMode(CRCore::crTexGen::NORMAL_MAP);
	//		ss->setTextureAttributeAndModes(TEXTURE_ENVMAP,texgen, CRCore::crStateAttribute::ON);

	//		crTexMat *tm = new crTexMat;
	//		ss->setTextureAttribute(TEXTURE_ENVMAP, tm);
	//		node.setCullCallback(new TexMatCallback(*tm));

	//		ss->setTextureAttributeAndModes(TEXTURE_ENVMAP, texCubeMap,CRCore::crStateAttribute::ON);
	//		ss->setMode( GL_LIGHTING, crStateAttribute::OFF );
	//		//ss->setMode( GL_CULL_FACE, crStateAttribute::OFF );

	//		crDepth* depth = new crDepth;
	//		depth->setFunction(crDepth::ALWAYS);
	//		depth->setRange(-1.0,1.0);
	//		depth->setWriteMask(false);
	//		ss->setAttribute(depth, crStateAttribute::ON );
	//		ss->setRenderBinDetails(-10,"StateSortedBin");

	//		crTransform* transform = new MoveEarthySkyWithEyePointTransform;
	//		transform->setCullingActive(false);
	//		transform->addChild(object);
	//		object->setCullingActive(false);
	//		//node.replaceChild(object,sky_camera.get());
	//		//sky_camera->addChild(object);
	//		node.addChild(transform);
	//		node.setCullingActive(false);
	//		transform->addDescription("#ScriptAddNode");
	//	}
	//}
	if(crArgumentParser::readKeyValue(command,"ShaderManager",str))
	{
		node.setStateSet(crShaderManager::getInstance()->getShaderStateSet(str));
	}
	char buf[16];
	std::string cfgFile;
	for(int i = 0; i<10; i++)
	{
		sprintf(buf,"cfgFile%d\0",i);
		if(crArgumentParser::readKeyValue(command,buf,cfgFile))
		{
			CREncapsulation::loadCfgFile(cfgFile,&node);
		}
	}
	std::string script;
	if(crArgumentParser::readKeyValue(command,"EventScript",script))
	{
		CREncapsulation::loadEventScript(script,&node);
	}
	if(m_scenarioTab.valid())
	{
		CREncapsulation::crTableIO::StrVec record;
		if(m_scenarioTab->queryOneRecord(0,node.getName(),record)>=0)
		{
			std::string scenariodir = CRNetApp::crGlobalHandle::getInstance()->getScenarioDir();
			CREncapsulation::loadEventScript(scenariodir+record[1],&node);
		}
	}
	crVector3 boundCenter,boundLength;
	if(crArgumentParser::readKeyValue(node.getDescriptions(),"BoundCenter",boundCenter) && crArgumentParser::readKeyValue(node.getDescriptions(),"BoundLength",boundLength))
	{
		node.setBoundBox(boundCenter,boundLength);
	}

	std::string particleFile;
	if(crArgumentParser::readKeyValue(command,"LoadParticle",particleFile) && CREncapsulation::crSceneManager::getInstance()->getDynamicRoot())
	{
		if(!particleFile.empty())
		{
			ref_ptr<CRCore::crGroup> particleGroup = new crMatrixTransform;//dynamic_cast<crGroup *>(node.clone(CRCore::crCopyOp::SHALLOW_COPY));
			ref_ptr<crData> data = crDataManager::getInstance()->getData("Event");
			ref_ptr<crHandle> driver = crHandleManager::getInstance()->getHandle("OneSlot");
			ref_ptr<crHandle> method = crHandleManager::getInstance()->getHandle("AttachNodeUpdate");
			method->inputParam(3,&node);
			driver->inputHandle(0,method.get());
			data->insertHandle(WCH_UPDATEVISITOR,driver.get());
			particleGroup->setDataClass(data.get());
			particleGroup->setName(node.getName());
			//ref_ptr<CRCore::crNode> particleRoot = CRParticle::crEffectParticleManager::getInstance()->getEffectRoot();
			CRCore::crNode *oldParticle = NULL;
			CRCore::NodeArray &childArray = CREncapsulation::crSceneManager::getInstance()->getDynamicRoot()->getChildArray();
			for( CRCore::NodeArray::iterator itr = childArray.begin();
				 itr != childArray.end();
				 ++itr )
			{
				if((*itr)->getName().compare(particleGroup->getName()) == 0)
				{
					oldParticle = itr->get();
					break;
				}
			}
			//CRCore::crNode *oldParticle =crEffectParticleManager::getInstance()->findEffectNode(particleGroup->getName());
			if(oldParticle)
				CRIOManager::crLoadManager::getInstance()->requestRemoveNode(CREncapsulation::crSceneManager::getInstance()->getDynamicRoot(),oldParticle);

			float loddist = 0.0f;
			crArgumentParser::readKeyValue(command,"ParticleLodDist",loddist);
			CREncapsulation::loadParticle(particleFile,particleGroup.get(),loddist);
		}
	}
	//int hasNodeEvent = 0;
	//CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"HasNodeEvent",hasNodeEvent);
	//if(hasNodeEvent)
	//{
	//	CREncapsulation::loadNodeEventCfg(&node);
	//}
	traverse(node);
}
void crExternFileLoadVisitor::apply(CRCore::crObject& node)
{
	std::string command = node.getDescriptionsInString();
	if(command.find("#ExternNode")!=-1)
	{
		ref_ptr<crExternNode> newNode = new crExternNode;
		if(node.getNumParents()==1)
		{
			CRCore::crGroup *group = node.getParent(0);		
			setNewNode2(newNode.get());
		}
		//CRCore::crNode::ParentArray &parents = node.getParents();
		//for( CRCore::crNode::ParentArray::iterator itr = parents.begin();
		//	itr!=parents.end();
		//	++itr )
		//{
		//	(*itr)->replaceChild(&node,newNode.get());
		//}
		apply(*newNode);
		return;
	}
	std::string str;
	if(crArgumentParser::readKeyValue(node.getDescriptions(),"ShaderManager",str))
	{
		node.setStateSet(crShaderManager::getInstance()->getShaderStateSet(str));
	}
	char buf[16];
	std::string cfgFile;
	for(int i = 0; i<10; i++)
	{
		sprintf(buf,"cfgFile%d\0",i);
		if(crArgumentParser::readKeyValue(node.getDescriptions(),buf,cfgFile))
		{
			CREncapsulation::loadCfgFile(cfgFile,&node);
		}
	}
	std::string script;
	if(crArgumentParser::readKeyValue(node.getDescriptions(),"EventScript",script))
	{
		CREncapsulation::loadEventScript(script,&node);
	}
	if(m_scenarioTab.valid())
	{
		CREncapsulation::crTableIO::StrVec record;
		if(m_scenarioTab->queryOneRecord(0,node.getName(),record)>=0)
		{
			std::string scenariodir = CRNetApp::crGlobalHandle::getInstance()->getScenarioDir();
			CREncapsulation::loadEventScript(scenariodir+record[1],&node);
		}
	}
	crVector3 boundCenter,boundLength;
	if(crArgumentParser::readKeyValue(node.getDescriptions(),"BoundCenter",boundCenter) && crArgumentParser::readKeyValue(node.getDescriptions(),"BoundLength",boundLength))
	{
		node.setBoundBox(boundCenter,boundLength);
	}
	if( node.getName().compare("sky") == 0 )
	{//SkyBin
		crStateSet *ss = node.getOrCreateStateSet();
		ss->setRenderingHint(crStateSet::SKY_BIN);
	}

	//int hasNodeEvent = 0;
	//CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"HasNodeEvent",hasNodeEvent);
	//if(hasNodeEvent)
	//{
	//	//CRCore::notify(CRCore::FATAL)<<"crExternFileLoadVisitor = "<<node.getName()<<std::endl;
	//	CREncapsulation::loadNodeEventCfg(&node);
	//}

	//int hasObjectLight = 0;
	//CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"HasObjectLight",hasObjectLight);
	//if(hasObjectLight)
	//{
	//	//CRCore::notify(CRCore::FATAL)<<"crExternFileLoadVisitor = "<<node.getName()<<std::endl;
	//	CREncapsulation::loadObjectLightCfg(&node);
	//}
}

void crExternFileLoadVisitor::apply(CRCore::crNode& node)
{
	std::string str;
	if(crArgumentParser::readKeyValue(node.getDescriptions(),"ShaderManager",str))
	{
		node.setStateSet(crShaderManager::getInstance()->getShaderStateSet(str));
	}
	char buf[16];
	std::string cfgFile;
	for(int i = 0; i<10; i++)
	{
		sprintf(buf,"cfgFile%d\0",i);
		if(crArgumentParser::readKeyValue(node.getDescriptions(),buf,cfgFile))
		{
			CREncapsulation::loadCfgFile(cfgFile,&node);
		}
	}
	std::string script;
	if(crArgumentParser::readKeyValue(node.getDescriptions(),"EventScript",script))
	{
		CREncapsulation::loadEventScript(script,&node);
	}
	if(m_scenarioTab.valid())
	{
		CREncapsulation::crTableIO::StrVec record;
		if(m_scenarioTab->queryOneRecord(0,node.getName(),record)>=0)
		{
			std::string scenariodir = CRNetApp::crGlobalHandle::getInstance()->getScenarioDir();
			CREncapsulation::loadEventScript(scenariodir+record[1],&node);
		}
	}
	crVector3 boundCenter,boundLength;
	if(crArgumentParser::readKeyValue(node.getDescriptions(),"BoundCenter",boundCenter) && crArgumentParser::readKeyValue(node.getDescriptions(),"BoundLength",boundLength))
	{
		node.setBoundBox(boundCenter,boundLength);
	}
	//int hasNodeEvent = 0;
	//CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"HasNodeEvent",hasNodeEvent);
	//if(hasNodeEvent)
	//{
	//	CREncapsulation::loadNodeEventCfg(&node);
	//}
}
void crExternFileLoadVisitor::apply(CRCore::crSequence& node)
{
    int sz = node.getNumChildren();
	if(sz>0)
	{
		node.setTime(sz-1,1.0f);
		char tmpText[15];
		float frametime;
		std::string comment = node.getDescriptionsInString();
		int vanishWhenStop = 1;
		if(crArgumentParser::readKeyValue(comment,"VanishWhenStop",vanishWhenStop))
			node.setVanishWhenStop(vanishWhenStop);

		std::string str;
		if(CRCore::crArgumentParser::readKeyValue(comment,"Mode",str))
		{
			if(str.compare("Start")==0)
			{
				node.setMode(CRCore::crSequence::START);
			}
			else if(str.compare("Stop")==0)
			{
				node.setMode(CRCore::crSequence::STOP);
			}
		}
		crVector3 pos;
		if(CRCore::crArgumentParser::readKeyValue(comment,"Position",pos))
		{
			node.setEffectPosition(pos);
		}

		for(int i = 0; i<sz; ++i)
		{
			frametime = 1.0f;
			sprintf(tmpText,"FrameTime%d\0",i);
			crArgumentParser::readKeyValue(comment,tmpText,frametime);
			node.setTime(i,frametime);
		}
		crVector3i vec3;
		if(CRCore::crArgumentParser::readKeyValue(comment,"播放参数",vec3))
		{
			node.setInterval((crSequence::LoopMode)vec3[0],vec3[1],vec3[2]);
		}
		crVector2f vec2;
		if(CRCore::crArgumentParser::readKeyValue(comment,"速度与次数",vec2))
		{
			node.setDuration(vec2[0]==0.0f?0.0f:1.0f/vec2[0],vec2[1]);
		}
	}
    apply((crGroup&)node);
}
void crExternFileLoadVisitor::apply(CRCore::crMatrixTransform& node)
{
	std::string str;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"AttachToNodeProxy",str))
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
		searchByNameVisitor.setNameId("ProxyNode");
		searchByNameVisitor.setSearchNodeType(ALLNODE);
		node.accept(searchByNameVisitor);
		crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
		if(proxyNode)
		{
			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId(str);
			searchByNameVisitor.setSearchNodeType(ALLNODE);
			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
			crNode* targetNode = searchByNameVisitor.getResult();
			if(targetNode)
			{
				ref_ptr<crData> data = crDataManager::getInstance()->getData("Event");
				ref_ptr<crHandle> driver = crHandleManager::getInstance()->getHandle("OneSlot");
				ref_ptr<crHandle> method = crHandleManager::getInstance()->getHandle("AttachNodeUpdate");

				if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"BoneName",str))
				{
					method->inputParam(7,&str);

					CRCore::crSearchNodeBYClassNameVisitor searchByClassNameVisitor;
					searchByClassNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN);
					searchByClassNameVisitor.insertClassNameId("CreBodyNode");
					targetNode->accept(searchByClassNameVisitor);
					rbody::CreBodyNode *bodyNode = dynamic_cast<rbody::CreBodyNode *>(searchByClassNameVisitor.getResult());
					if(bodyNode)
					{
						targetNode = bodyNode;
					}
				}
				crVector3 offset;
				CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"AttachOffset",offset);
				method->inputParam(3,targetNode);
				method->inputParam(4,&offset);
				crVector3 scale(1.0f,1.0f,1.0f);
				CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"AttachScale",scale);
				method->inputParam(5,&scale);
				float rotAngle = 0;
				CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"AttachRotAngle",rotAngle);
				rotAngle = CRCore::DegreesToRadians(rotAngle);
				method->inputParam(6,&rotAngle);
				driver->inputHandle(0,method.get());
				data->insertHandle(WCH_UPDATEVISITOR,driver.get());
				node.setDataClass(data.get());
			}
		}
	}
	else if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"AttachToNode",str))
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId(str);
		searchByNameVisitor.setSearchNodeType(ALLNODE);
		CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
		crNode* targetNode = searchByNameVisitor.getResult();
		if(targetNode)
		{
			ref_ptr<crData> data = crDataManager::getInstance()->getData("Event");
			ref_ptr<crHandle> driver = crHandleManager::getInstance()->getHandle("OneSlot");
			ref_ptr<crHandle> method = crHandleManager::getInstance()->getHandle("AttachNodeUpdate");

			if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"BoneName",str))
			{
				method->inputParam(7,&str);

				CRCore::crSearchNodeBYClassNameVisitor searchByClassNameVisitor;
				searchByClassNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN);
				searchByClassNameVisitor.insertClassNameId("CreBodyNode");
				targetNode->accept(searchByClassNameVisitor);
				rbody::CreBodyNode *bodyNode = dynamic_cast<rbody::CreBodyNode *>(searchByClassNameVisitor.getResult());
				if(bodyNode)
				{
					targetNode = bodyNode;
				}
			}
			crVector3 offset;
			CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"AttachOffset",offset);
			method->inputParam(3,targetNode);
			method->inputParam(4,&offset);
			crVector3 scale(1.0f,1.0f,1.0f);
			CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"AttachScale",scale);
			method->inputParam(5,&scale);
			float rotAngle = 0;
			CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"AttachRotAngle",rotAngle);
			rotAngle = CRCore::DegreesToRadians(rotAngle);
			method->inputParam(6,&rotAngle);
			driver->inputHandle(0,method.get());
			data->insertHandle(WCH_UPDATEVISITOR,driver.get());
			node.setDataClass(data.get());
		}
	}
	apply((crGroup&)node);
}
void crExternFileLoadVisitor::apply(CRCore::crLightSource& node)
{
	std::string str;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"AttachToNodeProxy",str))
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
		searchByNameVisitor.setNameId("ProxyNode");
		searchByNameVisitor.setSearchNodeType(ALLNODE);
		node.accept(searchByNameVisitor);
		crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
		if(proxyNode)
		{
			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId(str);
			searchByNameVisitor.setSearchNodeType(ALLNODE);
			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
			crNode* targetNode = searchByNameVisitor.getResult();
			if(targetNode)
			{
				ref_ptr<crData> data = crDataManager::getInstance()->getData("Event");
				ref_ptr<crHandle> driver = crHandleManager::getInstance()->getHandle("OneSlot");
				ref_ptr<crHandle> method = crHandleManager::getInstance()->getHandle("AttachNodeUpdate");

				crVector3 offset;
				CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"AttachOffset",offset);
				method->inputParam(3,targetNode);
				method->inputParam(4,&offset);
				float rotAngle = 0;
				CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"AttachRotAngle",rotAngle);
				rotAngle = CRCore::DegreesToRadians(rotAngle);
				method->inputParam(6,&rotAngle);
				if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"BoneName",str))
				{
					method->inputParam(7,&str);
				}
				driver->inputHandle(0,method.get());
				data->insertHandle(WCH_UPDATEVISITOR,driver.get());
				node.setDataClass(data.get());
			}
		}
	}
	else if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"AttachToNode",str))
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId(str);
		searchByNameVisitor.setSearchNodeType(ALLNODE);
		CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
		crNode* targetNode = searchByNameVisitor.getResult();
		if(targetNode)
		{
			ref_ptr<crData> data = crDataManager::getInstance()->getData("Event");
			ref_ptr<crHandle> driver = crHandleManager::getInstance()->getHandle("OneSlot");
			ref_ptr<crHandle> method = crHandleManager::getInstance()->getHandle("AttachNodeUpdate");

			crVector3 offset;
			CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"AttachOffset",offset);
			method->inputParam(3,targetNode);
			method->inputParam(4,&offset);
			float rotAngle = 0;
			CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"AttachRotAngle",rotAngle);
			rotAngle = CRCore::DegreesToRadians(rotAngle);
			method->inputParam(6,&rotAngle);
			if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"BoneName",str))
			{
				method->inputParam(7,&str);
			}
			driver->inputHandle(0,method.get());
			data->insertHandle(WCH_UPDATEVISITOR,driver.get());
			node.setDataClass(data.get());
		}
	}
	apply((crGroup&)node);
}
void crExternFileLoadVisitor::apply(CRParticle::crParticleEffect& node)
{
	std::string str;
	if(crArgumentParser::readKeyValue(node.getDescriptions(),"ShaderManager",str))
	{
		node.setStateSet(crShaderManager::getInstance()->getShaderStateSet(str));
	}
	//object材质
	//char buf[16];
	//std::string cfgFile;
	//for(int i = 0; i<10; i++)
	//{
	//	sprintf(buf,"cfgFile%d\0",i);
	//	if(crArgumentParser::readKeyValue(node.getDescriptions(),buf,cfgFile))
	//	{
	//		CREncapsulation::loadCfgFile(cfgFile,&node);
	//	}
	//}
	std::string script;
	if(crArgumentParser::readKeyValue(node.getDescriptions(),"EventScript",script))
	{
		CREncapsulation::loadEventScript(script,&node);
	}
	if(m_scenarioTab.valid())
	{
		CREncapsulation::crTableIO::StrVec record;
		if(m_scenarioTab->queryOneRecord(0,node.getName(),record)>=0)
		{
			std::string scenariodir = CRNetApp::crGlobalHandle::getInstance()->getScenarioDir();
			CREncapsulation::loadEventScript(scenariodir+record[1],&node);
		}
	}
}
void crExternFileLoadVisitor::apply(CRCore::crCanvasNode& node)
{
	std::string filename = CRNetApp::crGlobalHandle::getInstance()->getUITableDir()+node.getName()+".crb";
	std::string uidir = CRNetApp::crGlobalHandle::getInstance()->get3DUIDir();
	CRCore::ref_ptr<CREncapsulation::crTableIO> canvasTable = CREncapsulation::crTableIO::openFile(filename);
	if(canvasTable.valid())
	{
		if(CRIOManager::GetPublishRun())
		{
			std::string srcfile = filename;
			std::string destfile = CRNetApp::crGlobalHandle::getInstance()->getUITableDir()+"Publish/"+node.getName()+".crb";
			CRIOManager::copyFile(srcfile,destfile);
		}
		int rowcount = canvasTable->getRowCount();
		std::string widgetname,script;
		crWidgetNode *widget;
		for( int i = 0; i<rowcount; i++ )
		{
			widgetname = (*canvasTable)(i,0);
			if(widgetname == node.getName())
			{
				widget = &node;
			}
			else
			{
				widget = node.getWidget(widgetname);
			}
			if(widget)
			{
				script = (*canvasTable)(i,1);
				if(!script.empty())
				{
					CREncapsulation::loadEventScript(uidir+script,widget);
					if(CRIOManager::GetPublishRun())
					{
						std::string srcfile = uidir+script;
						std::string destfile = uidir+"Publish/"+script;
						CRIOManager::copyFile(srcfile,destfile);
					}
				}
			}
		}
	}
	int int1 = 0;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"ShowTop",int1))
	{
		if(!int1)
			node.setCanvasPos(CRCore::CANVAS_BOTTOM);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"CanvasPos",int1))
	{
		node.setCanvasPos((CRCore::CanvasPos)int1);
	}
	apply((crWidgetNode&)node);
}
void crExternFileLoadVisitor::apply(CRCore::crWidgetNode& node)
{
	int int1 = 0;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"CanFocus",int1))
	{
		node.setCanFocus(int1);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"CanCaptureMouse",int1))
	{
		node.setCanCaptureMouse(int1);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"CanCaptureInput",int1))
	{
		node.setCanCaptureInput(int1);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"MatrixEnable",int1))
	{
		node.setEnableMatrix(int1);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"#Enable",int1))
	{
		node.setEnable(int1);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"Visiable",int1))
	{
		node.setVisiable(int1);
	}
	apply((crGroup&)node);
}
void crExternFileLoadVisitor::apply(CRUI::crScrollBarWidgetNode& node)
{
	crVector2 range;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"Range",range))
	{
		node.setRange(range);
	}
	float lineValue;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"LineValue",lineValue))
	{
		node.setLineValue(lineValue);
	}
	float pageValue;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"PageValue",pageValue))
	{
		node.setPageValue(pageValue);
	}
	std::string str;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"ScrollBarType",str))
	{
		if(str.compare("HORIZONTALBAR") == 0)
			node.setScrollBarType(CRUI::crScrollBarWidgetNode::HORIZONTALBAR);
		else if(str.compare("VERTICALBAR") == 0)
			node.setScrollBarType(CRUI::crScrollBarWidgetNode::VERTICALBAR);
	}
	apply((crWidgetNode&)node);
}
void crExternFileLoadVisitor::apply(CRUI::crTextAttrWidgetNode& node)
{
	std::string str;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"DrawMode",str))
	{
		int int1 = 0;
		if(str.find("TEXT")!=std::string::npos)
			int1 |= CRText::crText::TEXT;
		if(str.find("BOUNDINGBOX")!=std::string::npos)
			int1 |= CRText::crText::BOUNDINGBOX;
		if(str.find("FILLEDBOUNDINGBOX")!=std::string::npos)
			int1 |= CRText::crText::FILLEDBOUNDINGBOX;
		if(str.find("UNDERLINE")!=std::string::npos)
			int1 |= CRText::crText::UNDERLINE;
		node.setDrawMode(int1);
	}
	float flt1;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"LineSpacing",flt1))
		node.setLineSpacing(flt1);
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"UpLineSpacing",flt1))
		node.setUpLineSpacing(flt1);
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"UnderLineSpacing",flt1))
		node.setUnderLineSpacing(flt1);
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"Font",str))
	{
		node.setFont(str);
	}
	float characterSize = 0;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"CharacterSize",characterSize))
	{
		node.setCharacterSize(characterSize);
	}
	CRCore::crVector2s fontSize;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"FontSize",fontSize))
	{
		node.setFontSize(fontSize);
	}
	crVector4 color;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"Color",color))
	{
		color /= 255.0f;
		node.setColor(color);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"Alignment",str))
	{
		if(str.compare("LEFT_TOP") == 0)
			node.setAlignment(CRText::crText::LEFT_TOP);
		else if(str.compare("LEFT_CENTER") == 0)
			node.setAlignment(CRText::crText::LEFT_CENTER);
		else if(str.compare("LEFT_BOTTOM") == 0)
			node.setAlignment(CRText::crText::LEFT_BOTTOM);
		else if(str.compare("CENTER_TOP") == 0)
			node.setAlignment(CRText::crText::CENTER_TOP);
		else if(str.compare("CENTER_CENTER") == 0)
			node.setAlignment(CRText::crText::CENTER_CENTER);
		else if(str.compare("CENTER_BOTTOM") == 0)
			node.setAlignment(CRText::crText::CENTER_BOTTOM);
		else if(str.compare("RIGHT_TOP") == 0)
			node.setAlignment(CRText::crText::RIGHT_TOP);
		else if(str.compare("RIGHT_CENTER") == 0)
			node.setAlignment(CRText::crText::RIGHT_CENTER);
		else if(str.compare("RIGHT_BOTTOM") == 0)
			node.setAlignment(CRText::crText::RIGHT_BOTTOM);
		else if(str.compare("LEFT_BASE_LINE") == 0)
			node.setAlignment(CRText::crText::LEFT_BASE_LINE);
		else if(str.compare("CENTER_BASE_LINE") == 0)
			node.setAlignment(CRText::crText::CENTER_BASE_LINE);
		else if(str.compare("RIGHT_BASE_LINE") == 0)
			node.setAlignment(CRText::crText::RIGHT_BASE_LINE);
		else if(str.compare("LEFT_BOTTOM_BASE_LINE") == 0)
			node.setAlignment(CRText::crText::LEFT_BOTTOM_BASE_LINE);
		else if(str.compare("CENTER_BOTTOM_BASE_LINE") == 0)
			node.setAlignment(CRText::crText::CENTER_BOTTOM_BASE_LINE);
		else if(str.compare("RIGHT_BOTTOM_BASE_LINE") == 0)
			node.setAlignment(CRText::crText::RIGHT_BOTTOM_BASE_LINE);
		else if(str.compare("BASE_LINE") == 0)
			node.setAlignment(CRText::crText::BASE_LINE);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"BackdropType",str))
	{
		if(str.compare("BOTTOM_RIGHT")==0)
			node.setBackdropType(CRText::crText::DROP_SHADOW_BOTTOM_RIGHT);
		else if(str.compare("CENTER_RIGHT")==0)
			node.setBackdropType(CRText::crText::DROP_SHADOW_CENTER_RIGHT);
		else if(str.compare("TOP_RIGHT")==0)
			node.setBackdropType(CRText::crText::DROP_SHADOW_TOP_RIGHT);
		else if(str.compare("BOTTOM_CENTER")==0)
			node.setBackdropType(CRText::crText::DROP_SHADOW_BOTTOM_CENTER);
		else if(str.compare("TOP_CENTER")==0)
			node.setBackdropType(CRText::crText::DROP_SHADOW_TOP_CENTER);
		else if(str.compare("BOTTOM_LEFT")==0)
			node.setBackdropType(CRText::crText::DROP_SHADOW_BOTTOM_LEFT);
		else if(str.compare("CENTER_LEFT")==0)
			node.setBackdropType(CRText::crText::DROP_SHADOW_CENTER_LEFT);
		else if(str.compare("TOP_LEFT")==0)
			node.setBackdropType(CRText::crText::DROP_SHADOW_TOP_LEFT);
		else if(str.compare("OUTLINE")==0)
			node.setBackdropType(CRText::crText::OUTLINE);
		else if(str.compare("NONE")==0)
			node.setBackdropType(CRText::crText::NONE);
	}
	CRCore::crVector2 offset;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"BackdropOffset",offset))
	{
		node.setBackdropOffset(offset);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"BackdropColor",color))
	{
		color /= 255.0f;
		node.setBackdropColor(color);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"VScrollBarName",str))
	{
		node.setVScrollBarName(str);
	}
	CRCore::crVector4f rect;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"RectOffset",rect))
	{
		node.setRectOffset(rect);
	}
	int int1;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"DefaultScrollPos",int1))
	{
		node.setDefaultScrollPos(int1);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"NeedScissor",int1))
	{
		node.setNeedScissor(int1);
	}
	apply((crWidgetNode&)node);
}
void crExternFileLoadVisitor::apply(CRUI::crStaticTextWidgetNode& node)
{
	int int1;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"CanSelectText",int1))
	{
		node.setCanSelectText(int1);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"Password",int1))
		node.setPassword(int1);
	std::string str;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"Text",str))
		node.setString(str);
	apply((CRUI::crTextAttrWidgetNode&)node);
}
void crExternFileLoadVisitor::apply(CRUI::crEditWidgetNode& node)
{
	int int1;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"MultiLine",int1))
		node.setMultiLine(int1);
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"Numeric",int1))
		node.setNumeric(int1);
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"AutoSelectAll",int1))
		node.setAutoSelectAll(int1);
	apply((CRUI::crStaticTextWidgetNode&)node);
}
void crExternFileLoadVisitor::apply(CRUI::crHypertextWidgetNode& node)
{
	apply((CRUI::crTextAttrWidgetNode&)node);
	std::string str;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"Hypertext",str))
	{
		node.setHypertext(str);
	}
}
void crExternFileLoadVisitor::apply(CRUI::crListBoxWidgetNode& node)
{
	apply((CRUI::crStaticTextWidgetNode&)node);
	int int1;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"MouseMoveSelect",int1))
	{
		node.setMouseMoveSelect(int1);
	}
}
void crExternFileLoadVisitor::apply(CRUI::crListControlWidgetNode& node)
{
	int int1;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"DefaultScrollPos",int1))
	{
		node.setDefaultScrollPos(int1);
	}
	CRCore::crVector4f rect;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"RectOffset",rect))
	{
		node.setRectOffset(rect);
	}
	CRCore::crVector2f vec2;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"NodeSize",vec2))
	{
		node.setNodeSize(vec2);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"SpaceBetween",vec2))
	{
		node.setSpaceBetween(vec2);
	}
	crVector4 color;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"SelectColor",color))
	{
		color /= 255.0f;
		node.setSelectColor(color);
	}
	std::string str;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"VScrollBarName",str))
	{
		node.setVScrollBarName(str);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"CanDragNode",int1))
	{
		node.setCanDragNode(int1);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"MouseMoveSelect",int1))
	{
		node.setMouseMoveSelect(int1);
	}
	apply((CRCore::crWidgetNode&)node);
}
void crExternFileLoadVisitor::apply(CRUI::crTableWidgetNode& node)
{
	char buf[16];
	std::string str;
	for(int i = 0; i<10; i++)
	{
		sprintf(buf,"Column%d\0",i);
		if(crArgumentParser::readKeyValue(node.getDescriptions(),buf,str))
		{
			node.addColumn(str);
		}
	}
	int int1;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"HasTitle",int1))
	{
		node.setHasTitle(int1);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"VScrollBarName",str))
	{
		node.setVScrollBarName(str);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"MouseMoveSelect",int1))
	{
		node.setMouseMoveSelect(int1);
	}
	apply((CRCore::crWidgetNode&)node);
}
void crExternFileLoadVisitor::apply(CRUI::crRadioGroupWidgetNode& node)
{
	char buf[16];
	std::string str;
	for(int i = 0; i<10; i++)
	{
		sprintf(buf,"AddRadio%d\0",i);
		if(crArgumentParser::readKeyValue(node.getDescriptions(),buf,str))
		{
			node.addRadioName(str);
		}
	}
	for(int i = 0; i<50; i++)
	{
		sprintf(buf,"[AddRadioEx%d]\0",i);
		if(crArgumentParser::readKeyValue(node.getDescriptions(),buf,str))
		{
			node.addRadioName(str);
		}
	}
	int int1 = 0;
	crArgumentParser::readKeyValue(node.getDescriptions(),"Select",int1);
	node.select(int1);
	apply((CRCore::crWidgetNode&)node);
}
void crExternFileLoadVisitor::apply(CRUI::crRadioWidgetNode& node)
{
	int int1;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"AcceptDrag",int1))
	{
		node.setAcceptDrag(int1);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"MouseMoveSelect",int1))
	{
		node.setMouseMoveSelect(int1);
	}
	apply((CRCore::crWidgetNode&)node);
}
void crExternFileLoadVisitor::apply(CRUI::crComboBoxWidgetNode& node)
{
	std::string str;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"ComboTextBoxName",str))
	{
		node.setComboTextBoxName(str);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"ComboButtonName",str))
	{
		node.setComboButtonName(str);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"ComboListBoxName",str))
	{
		node.setComboListBoxName(str);
	}
	apply((CRCore::crWidgetNode&)node);
}
void crExternFileLoadVisitor::apply(CRUI::crProgressWidgetNode& node)
{
	std::string str;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"ProgressBarType",str))
	{
		if(str.compare("HORIZONTALBAR") == 0)
			node.setProgressBarType(CRUI::crProgressWidgetNode::HORIZONTALBAR);
		else if(str.compare("VERTICALBAR") == 0)
			node.setProgressBarType(CRUI::crProgressWidgetNode::VERTICALBAR);
	}
	apply((crWidgetNode&)node);
}
void crExternFileLoadVisitor::apply(CRUI::crGroupWidgetNode& node)
{
	char buf[16];
	std::string str;
	for(int i = 0; i<10; i++)
	{
		sprintf(buf,"AddWidget%d\0",i);
		if(crArgumentParser::readKeyValue(node.getDescriptions(),buf,str))
		{
			node.addWidgetName(str);
		}
	}
	apply((CRCore::crWidgetNode&)node);
}
////////////
//if(ds->getWaveBlend())
//{
//	CREncapsulation::crWaveBlendCallback::registFinalStaticSet(waveblend_ss);
//	CREncapsulation::crResetWaveBlendCallback::registFinalStaticSet(waveblend_ss);

//	uniform = waveblend_ss->getOrCreateUniform("waveRect",CRCore::crUniform::FLOAT_VEC4);
//	uniform->setDataVariance(crBase::DYNAMIC);
//	uniform->set(crVector4(0.0f,0.0f,0.0f,-3.0f));

//	uniform = waveblend_ss->getOrCreateUniform("wave_scale_speed",CRCore::crUniform::FLOAT_VEC2);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(ds->getWaveScaleSpeed());

//	uniform = waveblend_ss->getOrCreateUniform("waveAmp",CRCore::crUniform::FLOAT);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(ds->getWaveAmp());

//	uniform = waveblend_ss->getOrCreateUniform("Noise3D",CRCore::crUniform::SAMPLER_3D);
//	uniform->setDataVariance(crBase::STATIC);
//	uniform->set(2);

//	const crVector4& ns = ds->getNoiseSample();
//	crTexture3D *tex3d = CRCore::crBrain::getInstance()->getTexture3D("waveblend");
//	if(!tex3d)
//	{
//		tex3d = CRUtil::crNoise::instance()->make3DNoiseTexture(ns[0],ns[1],ns[2],ns[3]);
//		CRCore::crBrain::getInstance()->insertTexture3D("waveblend",tex3d);
//	}
//	else
//	{
//		if(CRCore::crBrain::getInstance()->getNeedLoadImage3D(tex3d))
//		{
//			tex3d = CRUtil::crNoise::instance()->make3DNoiseTexture(ns[0],ns[1],ns[2],ns[3]);
//			CRCore::crBrain::getInstance()->insertTexture3D("waveblend",tex3d);
//		}
//	}
//	waveblend_ss->setTextureAttributeAndModes(2,tex3d,crStateAttribute::ON);
//}

//ref_ptr<crGroup> g = new crGroup;
//for(int i = 0; i<node.getNumChildren(); i++)
//{
//	camera->addChild(node.getChild(i));
// 		    prePasscamera->addChild(node.getChild(i));
//}
//         node.removeChild(0,node.getNumChildren());
//         g->addChild(prePasscamera.get());
//g->addChild(camera.get());

//camera->setName(node.getName());
//camera->setVisiable(node.getVisiable());
//camera->setCalcShadow(node.isCalcShadow());
//camera->setEnableIntersect(node.getEnableIntersect());
//camera->setRenderable(node.getRenderable());
//camera->setCollideMode(node.getCollideMode());
//camera->setCullingActive(node.getCullingActive());
//camera->setDescriptionsInString(command);

//group->replaceChild(&node,camera.get());