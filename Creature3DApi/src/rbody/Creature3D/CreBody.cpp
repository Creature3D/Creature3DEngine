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

#include <rbody/Creature3D/CreBody.h>
#include <CRCore/crMaterial.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crTexture2D.h>
#include <CRCore/crVector2.h>
//#include <CRUtil/crOptimizer.h>
//#include <CRUtil/crCullVisitor.h>

#include <CRIOManager/crReadFile.h>
#include <CRIOManager/crLoadManager.h>

#include <CRCore/crArgumentParser.h>
#include <rvrutils/Property.h>
//#include <rbody/Creature3D/CreBodyCallback.h>
//#include <rbody/Creature3D/GeometryContainer.h>
#include <rbody/Creature3D/SubMeshDrawable.h>
#include <rbody/Creature3D/ReplicantBodyMgr.h>
//#include <ode/ode.h>
#include <CRCore/crMath.h>
#include <CRCore/crNotify.h>
#include <CRCore/crBrain.h>
#include <CRCore/crEventCallbackManager.h>
#include <CRCore/crPolygonMode.h>
#include <CRCore/crAlphaFunc.h>
#include <CRCore/crBlendFunc.h>
#include <CRCore/crCullFace.h>
#include <CRCore/crDepth.h>
#include <CRIOManager/crCookFile.h>
//#include <CRAL\crSoundState.h>
//#include <CRAL\crSoundNode.h>
#include <sys/stat.h>
#include <CRCore/crCalcNodeParentsMatrixVisitor.h>
using namespace rbody;
using namespace CRCore;
using namespace CRIOManager;
//using namespace CRPhysics;
//using namespace CRAL;

//GeometryContainer* CreBodyNode::getGeomContainer()
//{
//    //static CRCore::ref_ptr<GeometryContainer> s_geomContainer = new GeometryContainer;
//    //return s_geomContainer.get();
//	return GeometryContainer::getInstance();
//}

CreBodyNode::~CreBodyNode()
{
	m_body = NULL;
	m_requestRemoveDrawableVec.clear();
	m_releaseDrawableVec.clear();
	m_safeReleaseVec.clear();
	m_requestInitDrawables = false;

	m_geomList.clear();
}

void CreBodyNode::releaseObjects(CRCore::crState* state)
{
	crGroup::releaseObjects(state);
	//crCharacterMatterObject::releaseObjects(state);
	//ReplicantBodyMgr::instance()->removeCharacter(const_cast<CreBodyNode *>(this));
	//CRCore::notify(CRCore::FATAL)<<" removeCharacter = "<<this->getName()<<std::endl;
	ReplicantBodyMgr::instance()->removeCharacter(this);
}

void CreBodyNode::setup(bool enableTangentSpace,CRCore::crGroup *copyNode/*,const CRCore::crCopyOp *copyop*/)
{
	setAnimPlaySpeed(1.0f);
	m_enableTangentSpace = enableTangentSpace;
    //m_noNeedTangentSpace = false;
	//m_lodLevel = 1.0f;

    m_update_mode = UPDATE_DEFAULT;
    
	//setCullingActive(false);
	crNodeCallback *nodeCallback = dynamic_cast<crNodeCallback *>(crEventCallbackManager::getInstance()->getEventCallback("CreBodyCull"));
    if(nodeCallback)
		addCullCallback(nodeCallback);
	m_body_scale = new CRCore::crMatrixTransform();
	m_body_scale->setName("body_scale");
    //m_body_scale->setCullingActive(false);
    addChild(m_body_scale);

 //   m_body_geode = new CRCore::crObject();
	//m_body_geode->setCullingActive(false);
 //   m_body_geode->setName(getName());
	//CRCore::notify(CRCore::FATAL)<<"CreBodyNode::setup: name = "<<getName()<<std::endl;

    //m_body_scale->addChild(m_body_geode);

    // Force model to initial pose
    getBody()->getCalModel()->update(0.0f);

    initDrawables(copyNode/*,copyop*/);

    //m_body_geode->setUpdateCallback(new CreBodyCallback(m_body_geode, this));

    //m_body_geode->setNodeMask(0x01);

    CRCore::crMatrix m;
    m.makeIdentity();
	float scale = getBody()->getScale();
	m.makeScale(scale, scale, scale);
	float angle = getBody()->getCoreBody()->getRotAngle();
	m.postMult(CRCore::crMatrix::rotate(CRCore::DegreesToRadians(angle),CRCore::Z_AXIS));
	//m.setTrans(crVector3(0.0f,0.0f,getBody()->getCoreBody()->getFootOffset()));
	m.setTrans(getBody()->getCoreBody()->getTransOffset());

	//m.setTrans(m.getTrans()-crVector3(0.0f,0.0f,m_viewHeight));
    m_body_scale->setMatrix(m);
	//m_body_scale->postMult(crMatrix::translate(0.0f,0.0f,-1.5f));

	setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);

	//m_body_scale->postMult(crMatrix::translate(-m_body_scale->getBound().center()));
	//CRUtil::crOptimizer chkPhysicsOptimizer;
	//chkPhysicsOptimizer.optimize(this,CRUtil::crOptimizer::CHECK_PHYSICS);
	//crVector3 center = -m_body_scale->getBound().center();
 //   center[2] = 0.0f;
	//m_body_scale->postMult(crMatrix::translate(center));
	m_needUpdate = false;
	computeBound();
	//m_last_tick = m_timer.tick();
}

void CreBodyNode::initDrawables(CRCore::crGroup *copyNode/*,const CRCore::crCopyOp *copyop*/)
{
    CalRenderer *cal_renderer = getBody()->getCalModel()->getRenderer();
    // Begin the rendering loop
    assert(cal_renderer->beginRendering());

    // Get the number of meshes
    int mesh_count = cal_renderer->getMeshCount();

    //std::string path = getBody()->getCoreBody()->getPath();

	int submesh_count = 0;
	bool needloadImage = false;
	std::string filename;
	CalSubmesh *core_submesh;
	SubMeshDrawable *submesh_drawable;
	for (int mesh_id = 0; mesh_id < mesh_count; mesh_id++) 
	{
        submesh_count = cal_renderer->getSubmeshCount(mesh_id);

        for(int submesh_id = 0; submesh_id < submesh_count; submesh_id++) 
		{
            if (cal_renderer->selectMeshSubmesh(mesh_id, submesh_id)) 
			{
                CRCore::ref_ptr<CRCore::crGeometry> geom;

                //if ((geom = CreBodyNode::getGeomContainer()->get(/*getBody()->getCoreBody()->getName()*/getName(), mesh_id, submesh_id)) == NULL)
				if ((geom = getGeometry(mesh_id, submesh_id)) == NULL)
				{
                    geom = new CRCore::crGeometry;
                    //CreBodyNode::getGeomContainer()->set(/*getBody()->getCoreBody()->*/getName(), mesh_id, submesh_id, geom.get());
					setGeometry(mesh_id, submesh_id, geom.get());

					submesh_drawable = new SubMeshDrawable(this, geom.get(), mesh_id, submesh_id/*,enableTangentSpace*/);
					//submesh_state_set = new CRCore::crStateSet();
					if(submesh_id>0)
						submesh_drawable->setName(m_body->getCoreBody()->getMeshName(mesh_id)+crArgumentParser::appItoa(submesh_id));
					else
						submesh_drawable->setName(m_body->getCoreBody()->getMeshName(mesh_id));

					std::vector<CalMesh *>& meshes = getBody()->getCalModel()->getVectorMesh();

					core_submesh = meshes[mesh_id]->getSubmesh(submesh_id);

					if(m_enableTangentSpace && !core_submesh->isTangentsEnabled(TEXTURE_BASEMAP))
						core_submesh->enableTangents(TEXTURE_BASEMAP/*TEXTURE_BUMPMAP*/,true);

					crObject *obj;
					if(copyNode)
					{
						crObject* copyObj = dynamic_cast<crObject *>(copyNode->getChild(m_body_scale->getNumChildren()));
						obj = dynamic_cast<crObject *>(copyObj->clone(crCopyOp::DEEP_COPY_NODES|
							crCopyOp::DEEP_COPY_STATESETS|crCopyOp::DEEP_COPY_UNIFORMS|crCopyOp::DEEP_COPY_Callbacks|
							crCopyOp::DEEP_COPY_DATAS));
						//obj = dynamic_cast<crObject *>(copyObj->clone(crCopyOp::SHALLOW_COPY));
						//obj = dynamic_cast<crObject *>((*copyop)(copyObj));
						crStateSet *drawableState = obj->getDrawable(0)->getStateSet();
						submesh_drawable->setStateSet(drawableState);
						obj->removeDrawable(0,obj->getNumDrawables());
						obj->setName(submesh_drawable->getName());
						obj->addDrawable(submesh_drawable);

						m_subMeshDrawableListMutex.lock();
						m_subMeshDrawableList.push_back(submesh_drawable);
						m_subMeshDrawableListMutex.unlock();

					}
					else
					{
						obj = new crObject;
						obj->setName(submesh_drawable->getName());
						obj->addDrawable(submesh_drawable);

						m_subMeshDrawableListMutex.lock();
						m_subMeshDrawableList.push_back(submesh_drawable);
						parseMeshMaterial(submesh_drawable);
						m_subMeshDrawableListMutex.unlock();
					}
					//obj->setCullingActive(false);
					m_body_scale->addChild(obj);
                }
            } // if meshsubmesh select
        } // for submesh
    } // for mesh
    // End the cal3d rendering

	//getBody()->getCalModel()->getSkeleton()->getCoreSkeleton()->calculateBoundingBoxes(getBody()->getCalModel()->getCoreModel());

    cal_renderer->endRendering();
	m_requestInitDrawables = false;
}


CreBodyNode::CreBodyNode(bool enableTangentSpace, bool initiate ) : 
    //m_body_geode(0),
	m_update_mode(UPDATE_DEFAULT),
	m_enableTangentSpace(enableTangentSpace),
	m_needUpdate(false),
	m_swapVaild(false),
	m_animPlaySpeed(1.0f),
	m_timerecord(0.0f),
	m_lodLevel(1.0f)
{ 
    if ( initiate ) 
	{
        m_body = new CreBody(/*this*/);
        setup(enableTangentSpace); 
    }
}

CreBodyNode::CreBodyNode(bool enableTangentSpace):
m_needUpdate(false),
m_swapVaild(false),
m_animPlaySpeed(1.0f),
m_timerecord(0.0f),
m_lodLevel(1.0f)
{
    m_body = new CreBody(/*this*/);

    setup(enableTangentSpace);
}


CreBodyNode::CreBodyNode(const std::string& name, const std::string&fileName, const std::string& id, bool enableTangentSpace):
m_needUpdate(false),
m_swapVaild(false),
m_animPlaySpeed(1.0f),
m_timerecord(0.0f),
m_lodLevel(1.0f)
{
	Body::MeshIDSet meshIDSet;
    m_body = new CreBody(/*this,*/ name,fileName,meshIDSet);
    setName(id);
	setFileName(fileName);
    setup(enableTangentSpace);
}
CreBodyNode::CreBodyNode(const CreBodyNode& body, const CRCore::crCopyOp& copyop):
m_needUpdate(false),
m_swapVaild(false),
m_animPlaySpeed(body.m_animPlaySpeed),
m_timerecord(0.0f),
m_lodLevel(1.0f),
m_fileName(body.m_fileName)
{
	m_descriptionArray = body.m_descriptionArray;

	//换装上去的mesh复制的时候材质有问题，需要重新加载该mesh的材质
	const Body::MeshIDSet& meshIDSet = body.m_body->getBodyMeshSet();
	//try{
	m_body = new CreBody(body.m_body->getCoreBody()->getName(),getFileName(),meshIDSet);
	//}
	//catch(...)
	//{
	//	CRCore::notify(CRCore::FATAL)<<"CreBodyNode 创建出错"<<std::endl;
	//	return;
	//}
	//char id[128];
	//int nid = ReplicantBodyMgr::instance()->getnid();
	//sprintf(id,"CreBody%d\0",nid);
	//setName(id);
	setName("CreBodyNode");
	setup(body.m_enableTangentSpace,body.m_body_scale);
	ReplicantBodyMgr::instance()->insertCharacter(this);

	//std::string filename;
	//if(crArgumentParser::readKeyValue(m_descriptionArray,"FileName",filename))
	//{
	//	rcfg::ConfigScript cfg_script;
	//    CRIOManager::crLoadCookFile scopedLoad(filename);
	//	struct stat buf;
	//	if (stat( filename.c_str(), &buf )) 
	//	{
	//		CRCore::notify(CRCore::FATAL)<<"crModifyDrawableMaterialMethod::operator(): file open error"<<std::endl;
	//		return;
	//	}

	//	if (!cfg_script.Open(filename.c_str(), CreScriptParserHeader))
	//	{
	//		CRCore::notify(CRCore::FATAL)<<"crModifyDrawableMaterialMethod::operator(): "<<cfg_script.GetLastError()<<std::endl;
	//		return;
	//	}

 //       parseMeshMaterial(cfg_script);
	//}
	std::string default_act;
	if(!CRCore::crArgumentParser::readKeyValue(m_descriptionArray,"default_act",default_act))
	{
		default_act = getBody()->getCoreBody()->getDefaultActivity();
	}
	ActionRequest* action = getBody()->getActionPrototype(default_act);
	if (action) 
	{
		action->setPrioritized(0);
		getBody()->executeAction(action, true);
	}
	//////////
	////Object Des拷贝
	//crObject *obj;
	//crObject *myobj;
	//for( int i = 0; i<body.m_body_scale->getNumChildren(); i++ )
	//{
	//	obj = dynamic_cast<crObject *>(body.m_body_scale->getChild(i));
	//	if(obj/* && obj->getNumDescriptions()>0*/)
	//	{
	//		myobj = dynamic_cast<crObject *>(m_body_scale->getChild(i));
	//		if(myobj)
	//		{
	//			myobj->setAcceptGI(obj->getAcceptGI());
	//			myobj->setDrawOutLine(obj->getDrawOutLine());
	//			if(obj->getStateSet())
	//			{
	//				myobj->setStateSet(dynamic_cast<crStateSet *>(copyop(obj->getStateSet())));
	//			}
	//			if(obj->getUpdateCallback())
	//			{
	//				myobj->addUpdateCallback(dynamic_cast<crNodeCallback *>(copyop(obj->getUpdateCallback())));
	//			}
	//			if(obj->getCullCallback())
	//			{
	//				myobj->addCullCallback(dynamic_cast<crNodeCallback *>(copyop(obj->getCullCallback())));
	//			}
	//			if(obj->getCollideCallback())
	//			{
	//				myobj->addCollideCallback(dynamic_cast<crCollideCallback *>(copyop(obj->getCollideCallback())));
	//			}
	//			//for( EventMap::const_iterator itr = node.m_eventMap.begin();
	//			//	itr != node.m_eventMap.end();
	//			//	++itr )
	//			//{
	//			//	myobj->insertEvent(itr->first,dynamic_cast<crEventCallback *>(copyop(itr->second.get())));
	//			//}
	//			if(obj->getDataClass())
	//				myobj->setDataClass(copyop(obj->getDataClass()));
	//			if(myobj->hasHandle(WCH_UPDATEVISITOR))
	//			{
	//				myobj->setNumChildrenRequiringUpdateTraversal(myobj->getNumChildrenRequiringUpdateTraversal()+1);
	//			}
	//		}
	//	}
	//}
}

crTexture::FilterMode getFilterModeFromStr(const std::string &str)
{
	if(str.compare("LINEAR") == 0)
		return crTexture::LINEAR;
	else if(str.compare("LINEAR_MIPMAP_LINEAR") == 0)
		return crTexture::LINEAR_MIPMAP_LINEAR;
	else if(str.compare("LINEAR_MIPMAP_NEAREST") == 0)
		return crTexture::LINEAR_MIPMAP_NEAREST;
	else if(str.compare("NEAREST") == 0)
		return crTexture::NEAREST;
	else if(str.compare("NEAREST_MIPMAP_LINEAR") == 0)
		return crTexture::NEAREST_MIPMAP_LINEAR;
	else if(str.compare("NEAREST_MIPMAP_NEAREST") == 0)
		return crTexture::NEAREST_MIPMAP_NEAREST;
	return crTexture::LINEAR_MIPMAP_LINEAR;
}
crTexture::WrapMode getWrapModeFromStr(const std::string &str)
{
	if(str.compare("CLAMP") == 0)
		return crTexture::CLAMP;
	else if(str.compare("CLAMP_TO_EDGE") == 0)
		return crTexture::CLAMP_TO_EDGE;
	else if(str.compare("CLAMP_TO_BORDER") == 0)
		return crTexture::CLAMP_TO_BORDER;
	else if(str.compare("REPEAT") == 0)
		return crTexture::REPEAT;
	else if(str.compare("MIRROR") == 0)
		return crTexture::MIRROR;
	return crTexture::REPEAT;
}
int getTextureOperateFromStr(const std::string &str)
{
	if(str.compare("Add") == 0)
		return 0;
	else if(str.compare("Mult") == 0)
		return 1;
	else if(str.compare("Replace") == 0)
		return 2;
	else if(str.compare("Ignore") == 0)
		return 3;
	else if(str.compare("AddETTo") == 0)
		return 4;
	else if(str.compare("MultETTo") == 0)
		return 5;
	else if(str.compare("ET3ToLM") == 0)
		return 6;
	return 0;
}
int getAlphaOperateFromStr(const std::string &str)
{
	if(str.compare("Ignore") == 0)
		return 0;
	else if(str.compare("Add") == 0)
		return 1;
	else if(str.compare("Mult") == 0)
		return 2;
	else if(str.compare("Replace") == 0)
		return 3;
	return 0;
}
int getDetailBin(const std::string &str)
{
	if(str.compare("Trans") == 0)
		return crStateSet::TRANSPARENT_BIN;
	else if(str.compare("PreTrans") == 0)
		return crStateSet::PRETRANS_BIN;
	else if(str.compare("PostTrans") == 0)
		return crStateSet::POSTTRANS_BIN;
	else if(str.compare("FXTrans") == 0)
		return crStateSet::FX_BIN;
	return crStateSet::TRANSPARENT_BIN;
}
void CreBodyNode::parseMeshMaterial(SubMeshDrawable *subMeshDrawable)
{
	int nMeshMaterial = 1;
	std::string meshName;
	std::string cfgfile;
	//int nCfgfile = 1;
	//CRCore::crDrawable *subMeshDrawable;
	ref_ptr<crStateSet> stateset;
	crMaterial *creMat;
	crTexture2D *texture;
	crUniform *uniform;
	std::vector<float> v_i;
	float flt1;
	int int1;
	std::string strbuf;
	crVector2 vec2;
	crVector3 vec3;
	crVector4 vec4;
	crVector3 mo3(1.0f,1.0f,1.0f);
	crVector3 ao3(0.0f,0.0f,0.0f);
	crVector4 mo4(1.0f,1.0f,1.0f,1.0f);
	crVector4 ao4(0.0f,0.0f,0.0f,0.0f);
	float maskClipValue;

	std::string fileName = getMeshMaterialFileName(subMeshDrawable->getMeshID());
	if(!fileName.empty()||crArgumentParser::readKeyValue(getDescriptions(),"FileName",fileName))
	{
		rcfg::ConfigScript cfg_script;
		if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,fileName))
		{
			//CRCore::notify(CRCore::FATAL)<<"parseMeshMaterial: file open error,"<<fileName<<std::endl;
			char gbuf[256];
			sprintf(gbuf,"parseMeshMaterial: file open error,%s\n\0",fileName.c_str());
			gDebugInfo->debugInfo(CRCore::FATAL,gbuf);
			return;
		}
		if(cfg_script.Push("Character"))
		{
			while (cfg_script.Push("MeshMaterial",nMeshMaterial++))
			{
				cfg_script.Get("MeshName", meshName);
				if(meshName.compare(subMeshDrawable->getName()) != 0)
				{
					if (!cfg_script.Pop())
					{
						//CRCore::notify(CRCore::FATAL)<<"parseMeshMaterial: "<<cfg_script.GetLastError()<<std::endl;
						char gbuf[256];
						sprintf(gbuf,"parseMeshMaterial:%s\n\0",cfg_script.GetLastError().c_str());
						gDebugInfo->debugInfo(CRCore::FATAL,gbuf);
					}
					continue;
				}
				//subMeshDrawable = meshDrawable;//getSubMeshDrawable(meshName);
					//if(subMeshDrawable)
				{
					stateset = new crStateSet;//subMeshDrawable->getOrCreateStateSet();
					//nCfgfile = 1;
					//while (cfg_script.Get("cfgFile",cfgfile,nCfgfile++))
					//{
					//	loadCfgMaterial(cfgfile,stateset);
					//}

					if(cfg_script.Get("MeshObjDes", strbuf))
					{
						for(std::string::iterator itr = strbuf.begin(); itr != strbuf.end(); ++itr)
						{
							if((*itr) == '@') (*itr) = '\n';
						}
						subMeshDrawable->getParent(0)->setDescriptionsInString(strbuf);
					}

					if(cfg_script.Get("TwoSided", int1))
					{
						if(int1 == 1)
						{
							stateset->setMode(GL_CULL_FACE,CRCore::crStateAttribute::OFF);
						}
					}
					if(cfg_script.Get("Color", v_i))
					{
						crVector4 color(v_i[0],v_i[1],v_i[2],v_i[3]);
						color /= 255.0f;
                        subMeshDrawable->setColor(color);
					}

					maskClipValue = 0.3333;
					cfg_script.Get("MaskClipValue", maskClipValue);
					if(cfg_script.Get("BlendMode", strbuf))
					{
						if(strbuf.compare("Trans") == 0)
						{
							if(maskClipValue>0.0f)
							{
								crAlphaFunc *alphaFunc = new crAlphaFunc;
								alphaFunc->setFunction(crAlphaFunc::GREATER,maskClipValue);
								stateset->setAttributeAndModes(alphaFunc,crStateAttribute::ON);
							}

							crBlendFunc *blendFunc = new crBlendFunc(crBlendFunc::SRC_ALPHA,crBlendFunc::ONE_MINUS_SRC_ALPHA);
							stateset->setAttributeAndModes(blendFunc,crStateAttribute::ON);

							if(cfg_script.Get("RenderOrder", strbuf))
							{
								stateset->setRenderingHint((crStateSet::RenderingHint)getDetailBin(strbuf));
							}
							//stateset->setRenderingHint(CRCore::crStateSet::TRANSPARENT_BIN);
						}
						else if(strbuf.compare("Add") == 0)
						{
							crBlendFunc *blendFunc = new crBlendFunc(crBlendFunc::ONE,crBlendFunc::ONE);
							stateset->setAttributeAndModes(blendFunc,crStateAttribute::ON);
							if(cfg_script.Get("RenderOrder", strbuf))
							{
								stateset->setRenderingHint((crStateSet::RenderingHint)getDetailBin(strbuf));
							}
						}
						else if(strbuf.compare("Modulate") == 0)
						{
							if(maskClipValue>0.0f)
							{
								crAlphaFunc *alphaFunc = new crAlphaFunc;
								alphaFunc->setFunction(crAlphaFunc::GREATER,maskClipValue);
								stateset->setAttributeAndModes(alphaFunc,crStateAttribute::ON);
							}
							crBlendFunc *blendFunc = new crBlendFunc(crBlendFunc::SRC_ALPHA,crBlendFunc::ONE);
							stateset->setAttributeAndModes(blendFunc,crStateAttribute::ON);
							if(cfg_script.Get("RenderOrder", strbuf))
							{
								stateset->setRenderingHint((crStateSet::RenderingHint)getDetailBin(strbuf));
							}
						}
						else if(strbuf.compare("Mask") == 0)
						{
							if(maskClipValue == 0.0f)
								maskClipValue = 0.3333f;
							crAlphaFunc *alphaFunc = new crAlphaFunc;
							alphaFunc->setFunction(crAlphaFunc::GREATER,maskClipValue);
							stateset->setAttributeAndModes(alphaFunc,crStateAttribute::ON);
							stateset->setRenderingHint(CRCore::crStateSet::ALPHATEST_BIN);
						}
					}

					if(cfg_script.Get("DisableDepthTest", int1))
					{
						if(int1 == 1)
						{
							stateset->setMode(GL_DEPTH_TEST,CRCore::crStateAttribute::OFF);
							stateset->setRenderingHint(CRCore::crStateSet::TRANSPARENT_BIN);
						}
					}
					if(cfg_script.Get("DepthFunc", strbuf))
					{
						if(strbuf.compare("Inherit")!=0)
						{
							crDepth* depth = new crDepth;
							if(strbuf.compare("NEVER") == 0)
							{
								depth->setFunction(crDepth::NEVER);
							}
							else if(strbuf.compare("LESS") == 0)
							{
								depth->setFunction(crDepth::LESS);
							}
							else if(strbuf.compare("EQUAL") == 0)
							{
								depth->setFunction(crDepth::EQUAL);
							}
							else if(strbuf.compare("LEQUAL") == 0)
							{
								depth->setFunction(crDepth::LEQUAL);
							}
							else if(strbuf.compare("GREATER") == 0)
							{
								depth->setFunction(crDepth::GREATER);
							}
							else if(strbuf.compare("NOTEQUAL") == 0)
							{
								depth->setFunction(crDepth::NOTEQUAL);
							}
							else if(strbuf.compare("GEQUAL") == 0)
							{
								depth->setFunction(crDepth::GEQUAL);
							}
							else if(strbuf.compare("ALWAYS") == 0)
							{
								depth->setFunction(crDepth::ALWAYS);
							}
							if(cfg_script.Get("DepthRange",v_i))
							{
								depth->setRange(v_i[0],v_i[1]);
							}
							if(cfg_script.Get("DepthWrite",int1))
							{
								depth->setWriteMask(int1);
							}
							stateset->setAttribute(depth,crStateAttribute::ON);
						}
					}
					if(cfg_script.Get("PolygonMode", strbuf))
					{
						if(strbuf.compare("Point") == 0)
						{
							crPolygonMode* polygonMode = new crPolygonMode;
							polygonMode->setMode(crPolygonMode::FRONT_AND_BACK,crPolygonMode::POINT);
							stateset->setAttributeAndModes(polygonMode,crStateAttribute::ON);
						}
						else if(strbuf.compare("Line") == 0)
						{
							crPolygonMode* polygonMode = new crPolygonMode;
							polygonMode->setMode(crPolygonMode::FRONT_AND_BACK,crPolygonMode::LINE);
							stateset->setAttributeAndModes(polygonMode,crStateAttribute::ON);
						}
					}

					if(cfg_script.Push("Material"))
					{
						creMat = dynamic_cast<crMaterial *>(stateset->getAttribute(crStateAttribute::MATERIAL));
						if(!creMat)
						{
							creMat = new crMaterial;
							stateset->setAttributeAndModes(creMat,crStateAttribute::ON);
						}
						if(cfg_script.Get("AmbientColor",v_i))
						{
							vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
							creMat->setAmbient(crMaterial::FRONT_AND_BACK,vec4);
						}
						if(cfg_script.Get("DiffuseColor",v_i))
						{
							vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
							creMat->setDiffuse(crMaterial::FRONT_AND_BACK,vec4);
						}
						if(cfg_script.Get("SpecularColor",v_i))
						{
							vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
							creMat->setSpecular(crMaterial::FRONT_AND_BACK,vec4);
						}
						if(cfg_script.Get("Shininess",flt1))
						{
							creMat->setShininess(crMaterial::FRONT_AND_BACK,flt1);
						}
						if(cfg_script.Get("EmissiveColor",v_i))
						{
							vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
							creMat->setEmission(crMaterial::FRONT_AND_BACK,vec4);
						}
						if (!cfg_script.Pop())
						{
							//CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter: "<<cfg_script.GetLastError()<<std::endl;
							char gbuf[256];
							sprintf(gbuf,"ReplicantBodyMgr::parseCharacter:%s\n\0",cfg_script.GetLastError().c_str());
							gDebugInfo->debugInfo(CRCore::FATAL,gbuf);
						}
					}
					if(cfg_script.Push("BaseMap"))
					{
						if(cfg_script.Get("BaseMap",strbuf))
						{
							stateset->removeTextureAttribute(TEXTURE_BASEMAP,crStateAttribute::TEXTURE);
							if(strbuf.compare("NULL")!=0)
							{
								texture = CRCore::crBrain::getInstance()->getTexture2D(strbuf);
								if(texture)
								{
									if(CRCore::crBrain::getInstance()->getNeedLoadImage2D(texture))
									{
										texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									}
								}
								else
								{
									texture = new crTexture2D;
									texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									CRCore::crBrain::getInstance()->insertTexture2D(strbuf,texture);
								}
								stateset->setTextureAttributeAndModes(TEXTURE_BASEMAP,texture,crStateAttribute::ON);

								if(cfg_script.Get("AlphaShadowEnable",int1))
								{
									texture->setAlphaShadowEnable(int1);
								}
								if(cfg_script.Get("BlendShadowEnable",int1))
								{
									texture->setBlendShadowEnable(int1);
								}

								if(cfg_script.Get("OpBT",strbuf))
								{
									stateset->setTextureOperate(TEXTURE_BASEMAP,getTextureOperateFromStr(strbuf));
								}
								if(cfg_script.Get("UVLayer",int1))
								{
									stateset->setTextureUV(TEXTURE_BASEMAP,int1);
								}
								if(cfg_script.Get("param4AddBT",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != ao4)
									{
										uniform = stateset->getOrCreateUniform("param4AddBT",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}
								if(cfg_script.Get("param1MultBT",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultBT",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param4MultBT",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != mo4)
									{
										uniform = stateset->getOrCreateUniform("param4MultBT",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}

								if(cfg_script.Get("OpBTA",strbuf))
								{
									stateset->setAlphaOperate(TEXTURE_BASEMAP,getAlphaOperateFromStr(strbuf));
								}
								if(cfg_script.Get("param1AddBTa",flt1))
								{
									if(flt1 != 0.0f)
									{
										uniform = stateset->getOrCreateUniform("param1AddBTa",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param1MultBTa",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultBTa",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}

								if(cfg_script.Get("baseMat",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("baseMat",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}
								if(cfg_script.Get("baseMatA",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("baseMatA",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}

								if(cfg_script.Get("MIN_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("MAG_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("WRAP_MODE",strbuf))
								{
									crTexture::WrapMode wrap = getWrapModeFromStr(strbuf);
									texture->setWrap(crTexture::WRAP_S,wrap);
									texture->setWrap(crTexture::WRAP_T,wrap);
									texture->setWrap(crTexture::WRAP_R,wrap);
								}
							}
						}
						if (!cfg_script.Pop())
						{
							//CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter: "<<cfg_script.GetLastError()<<std::endl;
							char gbuf[256];
							sprintf(gbuf,"ReplicantBodyMgr::parseCharacter:%s\n\0",cfg_script.GetLastError().c_str());
							gDebugInfo->debugInfo(CRCore::FATAL,gbuf);
						}
					}
					if(cfg_script.Push("LightMap"))
					{
						if(cfg_script.Get("LightMap",strbuf))
						{
							crTexture *lightMap = dynamic_cast<crTexture2D *>(stateset->getTextureAttribute(TEXTURE_LIGHTMAP,crStateAttribute::TEXTURE));
							if(lightMap)
								stateset->removeTextureAttribute(TEXTURE_LIGHTMAP,crStateAttribute::TEXTURE);
							if(strbuf.compare("NULL")!=0)
							{
								texture = CRCore::crBrain::getInstance()->getTexture2D(strbuf);
								if(texture)
								{
									if(CRCore::crBrain::getInstance()->getNeedLoadImage2D(texture))
									{
										texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									}
								}
								else
								{
									texture = new crTexture2D;
									texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									CRCore::crBrain::getInstance()->insertTexture2D(strbuf,texture);
								}
								stateset->setTextureAttributeAndModes(TEXTURE_LIGHTMAP,texture,crStateAttribute::ON);

								if(cfg_script.Get("MIN_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("MAG_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("WRAP_MODE",strbuf))
								{
									crTexture::WrapMode wrap = getWrapModeFromStr(strbuf);
									texture->setWrap(crTexture::WRAP_S,wrap);
									texture->setWrap(crTexture::WRAP_T,wrap);
									texture->setWrap(crTexture::WRAP_R,wrap);
								}
							}
						}
						if (!cfg_script.Pop())
						{
							//CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter: "<<cfg_script.GetLastError()<<std::endl;
							char gbuf[256];
							sprintf(gbuf,"ReplicantBodyMgr::parseCharacter:%s\n\0",cfg_script.GetLastError().c_str());
							gDebugInfo->debugInfo(CRCore::FATAL,gbuf);
						}
					}
					if(cfg_script.Push("EnvMap"))
					{
						if(cfg_script.Get("EnvMap",strbuf))
						{
							crTextureCubeMap *envMap = dynamic_cast<crTextureCubeMap *>(stateset->getTextureAttribute(TEXTURE_ENVMAP,crStateAttribute::TEXTURE));
							if(envMap)
								stateset->removeTextureAttribute(TEXTURE_ENVMAP,crStateAttribute::TEXTURE);
							if(strbuf.compare("NULL")!=0)
							{
								crTextureCubeMap *texCube = CRCore::crBrain::getInstance()->getTextureCubeMap(strbuf);
								if(texCube)
								{
									if(CRCore::crBrain::getInstance()->getNeedLoadImageCubeMap(texCube))
									{
										texCube->getImageDataRequestHandler()->requestCubeImageFile(strbuf,texCube);
									}
								}
								else
								{
									texCube = new crTextureCubeMap;
									texCube->getImageDataRequestHandler()->requestCubeImageFile(strbuf,texCube);
									CRCore::crBrain::getInstance()->insertTextureCubeMap(strbuf,texCube);
								}
								stateset->setTextureAttributeAndModes(TEXTURE_ENVMAP,texCube,crStateAttribute::ON);

								if(cfg_script.Get("param4AddENV",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != ao4)
									{
										uniform = stateset->getOrCreateUniform("param4AddENV",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}
								if(cfg_script.Get("param1MultENV",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultENV",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param4MultENV",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != mo4)
									{
										uniform = stateset->getOrCreateUniform("param4MultENV",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}

								if(cfg_script.Get("MIN_FILTER",strbuf))
								{
									texCube->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("MAG_FILTER",strbuf))
								{
									texCube->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("WRAP_MODE",strbuf))
								{
									crTexture::WrapMode wrap = getWrapModeFromStr(strbuf);
									texCube->setWrap(crTexture::WRAP_S,wrap);
									texCube->setWrap(crTexture::WRAP_T,wrap);
									texCube->setWrap(crTexture::WRAP_R,wrap);
								}
							}
						}
						if (!cfg_script.Pop())
						{
							//CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter: "<<cfg_script.GetLastError()<<std::endl;
							char gbuf[256];
							sprintf(gbuf,"ReplicantBodyMgr::parseCharacter:%s\n\0",cfg_script.GetLastError().c_str());
							gDebugInfo->debugInfo(CRCore::FATAL,gbuf);
						}
					}
					if(cfg_script.Push("NormalMap"))
					{
						if(cfg_script.Get("NormalMap",strbuf))
						{
							stateset->removeTextureAttribute(TEXTURE_BUMPMAP,crStateAttribute::TEXTURE);
							if(strbuf.compare("NULL")!=0)
							{
								texture = CRCore::crBrain::getInstance()->getTexture2D(strbuf);
								if(texture)
								{
									if(CRCore::crBrain::getInstance()->getNeedLoadImage2D(texture))
									{
										texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									}
								}
								else
								{
									texture = new crTexture2D;
									texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									CRCore::crBrain::getInstance()->insertTexture2D(strbuf,texture);
								}
								stateset->setTextureAttributeAndModes(TEXTURE_BUMPMAP,texture,crStateAttribute::ON);

								if(cfg_script.Get("UseParallax",int1))
								{
									texture->setUseParallax(int1);
								}

								if(texture->getUseParallax())
								{
									if(cfg_script.Get("parallax",flt1))
									{
										uniform = stateset->getOrCreateUniform("parallax",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}

								if(cfg_script.Get("param3AddNT",v_i))
								{
									vec3.set(v_i[0],v_i[1],v_i[2]);
									if(vec3 != ao3)
									{
										uniform = stateset->getOrCreateUniform("param3AddNT",CRCore::crUniform::FLOAT_VEC3);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec3);
									}
								}
								if(cfg_script.Get("param1MultNT",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultNT",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param3MultNT",v_i))
								{
									vec3.set(v_i[0],v_i[1],v_i[2]);
									if(vec3 != mo3)
									{
										uniform = stateset->getOrCreateUniform("param3MultNT",CRCore::crUniform::FLOAT_VEC3);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec3);
									}
								}

								if(cfg_script.Get("normalMat",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("normalMat",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}

								if(cfg_script.Get("MIN_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("MAG_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("WRAP_MODE",strbuf))
								{
									crTexture::WrapMode wrap = getWrapModeFromStr(strbuf);
									texture->setWrap(crTexture::WRAP_S,wrap);
									texture->setWrap(crTexture::WRAP_T,wrap);
									texture->setWrap(crTexture::WRAP_R,wrap);
								}
							}
						}
						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter: "<<cfg_script.GetLastError()<<std::endl;
					}
					if(cfg_script.Push("DiffuseMap"))
					{
						if(cfg_script.Get("DiffuseMap",strbuf))
						{
							stateset->removeTextureAttribute(TEXTURE_DIFFUSEMAP,crStateAttribute::TEXTURE);
							if(strbuf.compare("NULL")!=0)
							{
								texture = CRCore::crBrain::getInstance()->getTexture2D(strbuf);
								if(texture)
								{
									if(CRCore::crBrain::getInstance()->getNeedLoadImage2D(texture))
									{
										texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									}
								}
								else
								{
									texture = new crTexture2D;
									texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									CRCore::crBrain::getInstance()->insertTexture2D(strbuf,texture);
								}
								stateset->setTextureAttributeAndModes(TEXTURE_DIFFUSEMAP,texture,crStateAttribute::ON);

								if(cfg_script.Get("OpDT",strbuf))
								{
									stateset->setTextureOperate(TEXTURE_DIFFUSEMAP,getTextureOperateFromStr(strbuf));
								}
								if(cfg_script.Get("UVLayer",int1))
								{
									stateset->setTextureUV(TEXTURE_DIFFUSEMAP,int1);
								}
								if(cfg_script.Get("param4AddDT",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != ao4)
									{
										uniform = stateset->getOrCreateUniform("param4AddDT",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}
								if(cfg_script.Get("param1MultDT",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultDT",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param4MultDT",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != mo4)
									{
										uniform = stateset->getOrCreateUniform("param4MultDT",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}

								if(cfg_script.Get("OpDTA",strbuf))
								{
									stateset->setAlphaOperate(TEXTURE_DIFFUSEMAP,getAlphaOperateFromStr(strbuf));
								}
								if(cfg_script.Get("param1AddDTa",flt1))
								{
									if(flt1 != 0.0f)
									{
										uniform = stateset->getOrCreateUniform("param1AddDTa",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param1MultDTa",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultDTa",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}

								if(cfg_script.Get("diffuseMat",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("diffuseMat",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}
								if(cfg_script.Get("diffuseMatA",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("diffuseMatA",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}

								if(cfg_script.Get("MIN_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("MAG_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("WRAP_MODE",strbuf))
								{
									crTexture::WrapMode wrap = getWrapModeFromStr(strbuf);
									texture->setWrap(crTexture::WRAP_S,wrap);
									texture->setWrap(crTexture::WRAP_T,wrap);
									texture->setWrap(crTexture::WRAP_R,wrap);
								}
							}
						}
						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter: "<<cfg_script.GetLastError()<<std::endl;
					}
					if(cfg_script.Push("SpecularMap"))
					{
						if(cfg_script.Get("SpecularMap",strbuf))
						{
							stateset->removeTextureAttribute(TEXTURE_SPECULARMAP,crStateAttribute::TEXTURE);
							if(strbuf.compare("NULL")!=0)
							{
								texture = CRCore::crBrain::getInstance()->getTexture2D(strbuf);
								if(texture)
								{
									if(CRCore::crBrain::getInstance()->getNeedLoadImage2D(texture))
									{
										texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									}
								}
								else
								{
									texture = new crTexture2D;
									texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									CRCore::crBrain::getInstance()->insertTexture2D(strbuf,texture);
								}
								stateset->setTextureAttributeAndModes(TEXTURE_SPECULARMAP,texture,crStateAttribute::ON);

								//if(cfg_script.Get("AlphaShininessEnable",int1))
								//{
								//	texture->setAlphaShininessEnable(int1);
								//}
								if(cfg_script.Get("OpST",strbuf))
								{
									stateset->setTextureOperate(TEXTURE_SPECULARMAP,getTextureOperateFromStr(strbuf));
								}
								if(cfg_script.Get("UVLayer",int1))
								{
									stateset->setTextureUV(TEXTURE_SPECULARMAP,int1);
								}
								if(cfg_script.Get("param4AddST",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != ao4)
									{
										uniform = stateset->getOrCreateUniform("param4AddST",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}
								if(cfg_script.Get("param1MultST",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultST",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param4MultST",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != mo4)
									{
										uniform = stateset->getOrCreateUniform("param4MultST",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}

								if(cfg_script.Get("OpSTA",strbuf))
								{
									stateset->setAlphaOperate(TEXTURE_SPECULARMAP,getAlphaOperateFromStr(strbuf));
								}
								if(cfg_script.Get("param1AddSTa",flt1))
								{
									if(flt1 != 0.0f)
									{
										uniform = stateset->getOrCreateUniform("param1AddSTa",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param1MultSTa",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultSTa",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}

								if(cfg_script.Get("specularMat",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("specularMat",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}
								if(cfg_script.Get("specularMatA",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("specularMatA",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}

								if(cfg_script.Get("MIN_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("MAG_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("WRAP_MODE",strbuf))
								{
									crTexture::WrapMode wrap = getWrapModeFromStr(strbuf);
									texture->setWrap(crTexture::WRAP_S,wrap);
									texture->setWrap(crTexture::WRAP_T,wrap);
									texture->setWrap(crTexture::WRAP_R,wrap);
								}
							}
						}
						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter: "<<cfg_script.GetLastError()<<std::endl;
					}
					if(cfg_script.Push("EmissiveMap"))
					{
						if(cfg_script.Get("EmissiveMap",strbuf))
						{
							stateset->removeTextureAttribute(TEXTURE_EMISSIVEMAP,crStateAttribute::TEXTURE);
							if(strbuf.compare("NULL")!=0)
							{
								texture = CRCore::crBrain::getInstance()->getTexture2D(strbuf);
								if(texture)
								{
									if(CRCore::crBrain::getInstance()->getNeedLoadImage2D(texture))
									{
										texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									}
								}
								else
								{
									texture = new crTexture2D;
									texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									CRCore::crBrain::getInstance()->insertTexture2D(strbuf,texture);
								}
								stateset->setTextureAttributeAndModes(TEXTURE_EMISSIVEMAP,texture,crStateAttribute::ON);

								if(cfg_script.Get("OpET",strbuf))
								{
									stateset->setTextureOperate(TEXTURE_EMISSIVEMAP,getTextureOperateFromStr(strbuf));
								}
								if(cfg_script.Get("UVLayer",int1))
								{
									stateset->setTextureUV(TEXTURE_EMISSIVEMAP,int1);
								}
								if(cfg_script.Get("param4AddET",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != ao4)
									{
										uniform = stateset->getOrCreateUniform("param4AddET",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}
								if(cfg_script.Get("param1MultET",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultET",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param4MultET",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != mo4)
									{
										uniform = stateset->getOrCreateUniform("param4MultET",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}

								if(cfg_script.Get("OpETA",strbuf))
								{
									stateset->setAlphaOperate(TEXTURE_EMISSIVEMAP,getAlphaOperateFromStr(strbuf));
								}
								if(cfg_script.Get("param1AddETa",flt1))
								{
									if(flt1 != 0.0f)
									{
										uniform = stateset->getOrCreateUniform("param1AddETa",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param1MultETa",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultETa",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}

								if(cfg_script.Get("emissiveMat",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("emissiveMat",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}
								if(cfg_script.Get("emissiveMatA",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("emissiveMatA",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}

								if(cfg_script.Get("MIN_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("MAG_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("WRAP_MODE",strbuf))
								{
									crTexture::WrapMode wrap = getWrapModeFromStr(strbuf);
									texture->setWrap(crTexture::WRAP_S,wrap);
									texture->setWrap(crTexture::WRAP_T,wrap);
									texture->setWrap(crTexture::WRAP_R,wrap);
								}
							}
						}
						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter: "<<cfg_script.GetLastError()<<std::endl;
					}
					if(cfg_script.Push("NormalMap2"))
					{
						if(cfg_script.Get("NormalMap2",strbuf))
						{
							stateset->removeTextureAttribute(TEXTURE_NORMALMAP2,crStateAttribute::TEXTURE);
							if(strbuf.compare("NULL")!=0)
							{
								texture = CRCore::crBrain::getInstance()->getTexture2D(strbuf);
								if(texture)
								{
									if(CRCore::crBrain::getInstance()->getNeedLoadImage2D(texture))
									{
										texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									}
								}
								else
								{
									texture = new crTexture2D;
									texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									CRCore::crBrain::getInstance()->insertTexture2D(strbuf,texture);
								}
								stateset->setTextureAttributeAndModes(TEXTURE_NORMALMAP2,texture,crStateAttribute::ON);

								if(cfg_script.Get("OpNT2",strbuf))
								{
									stateset->setTextureOperate(TEXTURE_NORMALMAP2,getTextureOperateFromStr(strbuf));
								}
								if(cfg_script.Get("UVLayer",int1))
								{
									stateset->setTextureUV(TEXTURE_NORMALMAP2,int1);
								}
								if(cfg_script.Get("param3AddNT2",v_i))
								{
									vec3.set(v_i[0],v_i[1],v_i[2]);
									if(vec3 != ao3)
									{
										uniform = stateset->getOrCreateUniform("param3AddNT2",CRCore::crUniform::FLOAT_VEC3);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec3);
									}
								}
								if(cfg_script.Get("param1MultNT2",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultNT2",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param3MultNT2",v_i))
								{
									vec3.set(v_i[0],v_i[1],v_i[2]);
									if(vec3 != mo3)
									{
										uniform = stateset->getOrCreateUniform("param3MultNT2",CRCore::crUniform::FLOAT_VEC3);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec3);
									}
								}

								if(cfg_script.Get("normalMat2",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("normalMat2",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}

								if(cfg_script.Get("MIN_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("MAG_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("WRAP_MODE",strbuf))
								{
									crTexture::WrapMode wrap = getWrapModeFromStr(strbuf);
									texture->setWrap(crTexture::WRAP_S,wrap);
									texture->setWrap(crTexture::WRAP_T,wrap);
									texture->setWrap(crTexture::WRAP_R,wrap);
								}
							}
						}
						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter: "<<cfg_script.GetLastError()<<std::endl;
					}
					if(cfg_script.Push("DiffuseMap2"))
					{
						if(cfg_script.Get("DiffuseMap2",strbuf))
						{
							stateset->removeTextureAttribute(TEXTURE_DIFFUSEMAP2,crStateAttribute::TEXTURE);
							if(strbuf.compare("NULL")!=0)
							{
								texture = CRCore::crBrain::getInstance()->getTexture2D(strbuf);
								if(texture)
								{
									if(CRCore::crBrain::getInstance()->getNeedLoadImage2D(texture))
									{
										texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									}
								}
								else
								{
									texture = new crTexture2D;
									texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									CRCore::crBrain::getInstance()->insertTexture2D(strbuf,texture);
								}
								stateset->setTextureAttributeAndModes(TEXTURE_DIFFUSEMAP2,texture,crStateAttribute::ON);

								if(cfg_script.Get("OpDT2",strbuf))
								{
									stateset->setTextureOperate(TEXTURE_DIFFUSEMAP2,getTextureOperateFromStr(strbuf));
								}
								if(cfg_script.Get("UVLayer",int1))
								{
									stateset->setTextureUV(TEXTURE_DIFFUSEMAP2,int1);
								}
								if(cfg_script.Get("param4AddDT2",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != ao4)
									{
										uniform = stateset->getOrCreateUniform("param4AddDT2",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}
								if(cfg_script.Get("param1MultDT2",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultDT2",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param4MultDT2",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != mo4)
									{
										uniform = stateset->getOrCreateUniform("param4MultDT2",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}

								if(cfg_script.Get("OpDT2A",strbuf))
								{
									stateset->setAlphaOperate(TEXTURE_DIFFUSEMAP2,getAlphaOperateFromStr(strbuf));
								}
								if(cfg_script.Get("param1AddDT2a",flt1))
								{
									if(flt1 != 0.0f)
									{
										uniform = stateset->getOrCreateUniform("param1AddDT2a",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param1MultDT2a",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultDT2a",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}

								if(cfg_script.Get("diffuseMat2",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("diffuseMat2",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}
								if(cfg_script.Get("diffuseMat2A",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("diffuseMat2A",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}

								if(cfg_script.Get("MIN_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("MAG_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("WRAP_MODE",strbuf))
								{
									crTexture::WrapMode wrap = getWrapModeFromStr(strbuf);
									texture->setWrap(crTexture::WRAP_S,wrap);
									texture->setWrap(crTexture::WRAP_T,wrap);
									texture->setWrap(crTexture::WRAP_R,wrap);
								}
							}
						}
						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter: "<<cfg_script.GetLastError()<<std::endl;
					}
					if(cfg_script.Push("SpecularMap2"))
					{
						if(cfg_script.Get("SpecularMap2",strbuf))
						{
							stateset->removeTextureAttribute(TEXTURE_SPECULARMAP2,crStateAttribute::TEXTURE);
							if(strbuf.compare("NULL")!=0)
							{
								texture = CRCore::crBrain::getInstance()->getTexture2D(strbuf);
								if(texture)
								{
									if(CRCore::crBrain::getInstance()->getNeedLoadImage2D(texture))
									{
										texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									}
								}
								else
								{
									texture = new crTexture2D;
									texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									CRCore::crBrain::getInstance()->insertTexture2D(strbuf,texture);
								}
								stateset->setTextureAttributeAndModes(TEXTURE_SPECULARMAP2,texture,crStateAttribute::ON);

								if(cfg_script.Get("OpST2",strbuf))
								{
									stateset->setTextureOperate(TEXTURE_SPECULARMAP2,getTextureOperateFromStr(strbuf));
								}
								if(cfg_script.Get("UVLayer",int1))
								{
									stateset->setTextureUV(TEXTURE_SPECULARMAP2,int1);
								}
								if(cfg_script.Get("param4AddST2",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != ao4)
									{
										uniform = stateset->getOrCreateUniform("param4AddST2",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}
								if(cfg_script.Get("param1MultST2",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultST2",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param4MultST2",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != mo4)
									{
										uniform = stateset->getOrCreateUniform("param4MultST2",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}

								if(cfg_script.Get("OpST2A",strbuf))
								{
									stateset->setAlphaOperate(TEXTURE_SPECULARMAP2,getAlphaOperateFromStr(strbuf));
								}
								if(cfg_script.Get("param1AddST2a",flt1))
								{
									if(flt1 != 0.0f)
									{
										uniform = stateset->getOrCreateUniform("param1AddST2a",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param1MultST2a",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultST2a",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}

								if(cfg_script.Get("specularMat2",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("specularMat2",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}
								if(cfg_script.Get("specularMat2A",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("specularMat2A",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}

								if(cfg_script.Get("MIN_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("MAG_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("WRAP_MODE",strbuf))
								{
									crTexture::WrapMode wrap = getWrapModeFromStr(strbuf);
									texture->setWrap(crTexture::WRAP_S,wrap);
									texture->setWrap(crTexture::WRAP_T,wrap);
									texture->setWrap(crTexture::WRAP_R,wrap);
								}
							}
						}
						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter: "<<cfg_script.GetLastError()<<std::endl;
					}
					if(cfg_script.Push("EmissiveMap2"))
					{
						if(cfg_script.Get("EmissiveMap2",strbuf))
						{
							stateset->removeTextureAttribute(TEXTURE_EMISSIVEMAP2,crStateAttribute::TEXTURE);
							if(strbuf.compare("NULL")!=0)
							{
								texture = CRCore::crBrain::getInstance()->getTexture2D(strbuf);
								if(texture)
								{
									if(CRCore::crBrain::getInstance()->getNeedLoadImage2D(texture))
									{
										texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									}
								}
								else
								{
									texture = new crTexture2D;
									texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									CRCore::crBrain::getInstance()->insertTexture2D(strbuf,texture);
								}
								stateset->setTextureAttributeAndModes(TEXTURE_EMISSIVEMAP2,texture,crStateAttribute::ON);

								if(cfg_script.Get("OpET2",strbuf))
								{
									stateset->setTextureOperate(TEXTURE_EMISSIVEMAP2,getTextureOperateFromStr(strbuf));
								}
								if(cfg_script.Get("UVLayer",int1))
								{
									stateset->setTextureUV(TEXTURE_EMISSIVEMAP2,int1);
								}
								if(cfg_script.Get("param4AddET2",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != ao4)
									{
										uniform = stateset->getOrCreateUniform("param4AddET2",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}
								if(cfg_script.Get("param1MultET2",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultET2",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param4MultET2",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != mo4)
									{
										uniform = stateset->getOrCreateUniform("param4MultET2",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}

								if(cfg_script.Get("OpET2A",strbuf))
								{
									stateset->setAlphaOperate(TEXTURE_EMISSIVEMAP2,getAlphaOperateFromStr(strbuf));
								}
								if(cfg_script.Get("param1AddET2a",flt1))
								{
									if(flt1 != 0.0f)
									{
										uniform = stateset->getOrCreateUniform("param1AddET2a",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param1MultET2a",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultET2a",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}

								if(cfg_script.Get("emissiveMat2",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("emissiveMat2",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}
								if(cfg_script.Get("emissiveMat2A",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("emissiveMat2A",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}

								if(cfg_script.Get("MIN_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("MAG_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("WRAP_MODE",strbuf))
								{
									crTexture::WrapMode wrap = getWrapModeFromStr(strbuf);
									texture->setWrap(crTexture::WRAP_S,wrap);
									texture->setWrap(crTexture::WRAP_T,wrap);
									texture->setWrap(crTexture::WRAP_R,wrap);
								}
							}
						}
						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter: "<<cfg_script.GetLastError()<<std::endl;
					}
					if(cfg_script.Push("NormalMap3"))
					{
						if(cfg_script.Get("NormalMap3",strbuf))
						{
							stateset->removeTextureAttribute(TEXTURE_NORMALMAP3,crStateAttribute::TEXTURE);
							if(strbuf.compare("NULL")!=0)
							{
								texture = CRCore::crBrain::getInstance()->getTexture2D(strbuf);
								if(texture)
								{
									if(CRCore::crBrain::getInstance()->getNeedLoadImage2D(texture))
									{
										texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									}
								}
								else
								{
									texture = new crTexture2D;
									texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									CRCore::crBrain::getInstance()->insertTexture2D(strbuf,texture);
								}
								stateset->setTextureAttributeAndModes(TEXTURE_NORMALMAP3,texture,crStateAttribute::ON);

								if(cfg_script.Get("OpNT3",strbuf))
								{
									stateset->setTextureOperate(TEXTURE_NORMALMAP3,getTextureOperateFromStr(strbuf));
								}
								if(cfg_script.Get("UVLayer",int1))
								{
									stateset->setTextureUV(TEXTURE_NORMALMAP3,int1);
								}
								if(cfg_script.Get("param3AddNT3",v_i))
								{
									vec3.set(v_i[0],v_i[1],v_i[2]);
									if(vec3 != ao3)
									{
										uniform = stateset->getOrCreateUniform("param3AddNT3",CRCore::crUniform::FLOAT_VEC3);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec3);
									}
								}
								if(cfg_script.Get("param1MultNT3",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultNT3",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param3MultNT3",v_i))
								{
									vec3.set(v_i[0],v_i[1],v_i[2]);
									if(vec3 != mo3)
									{
										uniform = stateset->getOrCreateUniform("param3MultNT3",CRCore::crUniform::FLOAT_VEC3);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec3);
									}
								}

								if(cfg_script.Get("normalMat3",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("normalMat3",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}

								if(cfg_script.Get("MIN_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("MAG_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("WRAP_MODE",strbuf))
								{
									crTexture::WrapMode wrap = getWrapModeFromStr(strbuf);
									texture->setWrap(crTexture::WRAP_S,wrap);
									texture->setWrap(crTexture::WRAP_T,wrap);
									texture->setWrap(crTexture::WRAP_R,wrap);
								}
							}
						}
						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter: "<<cfg_script.GetLastError()<<std::endl;
					}
					if(cfg_script.Push("SpecularMap3"))
					{
						if(cfg_script.Get("SpecularMap3",strbuf))
						{
							stateset->removeTextureAttribute(TEXTURE_SPECULARMAP3,crStateAttribute::TEXTURE);
							if(strbuf.compare("NULL")!=0)
							{
								texture = CRCore::crBrain::getInstance()->getTexture2D(strbuf);
								if(texture)
								{
									if(CRCore::crBrain::getInstance()->getNeedLoadImage2D(texture))
									{
										texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									}
								}
								else
								{
									texture = new crTexture2D;
									texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									CRCore::crBrain::getInstance()->insertTexture2D(strbuf,texture);
								}
								stateset->setTextureAttributeAndModes(TEXTURE_SPECULARMAP3,texture,crStateAttribute::ON);

								if(cfg_script.Get("OpST3",strbuf))
								{
									stateset->setTextureOperate(TEXTURE_SPECULARMAP3,getTextureOperateFromStr(strbuf));
								}
								if(cfg_script.Get("UVLayer",int1))
								{
									stateset->setTextureUV(TEXTURE_SPECULARMAP3,int1);
								}
								if(cfg_script.Get("param4AddST3",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != ao4)
									{
										uniform = stateset->getOrCreateUniform("param4AddST3",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}
								if(cfg_script.Get("param1MultST3",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultST3",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param4MultST3",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != mo4)
									{
										uniform = stateset->getOrCreateUniform("param4MultST3",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}

								if(cfg_script.Get("OpST3A",strbuf))
								{
									stateset->setAlphaOperate(TEXTURE_SPECULARMAP3,getAlphaOperateFromStr(strbuf));
								}
								if(cfg_script.Get("param1AddST3a",flt1))
								{
									if(flt1 != 0.0f)
									{
										uniform = stateset->getOrCreateUniform("param1AddST3a",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param1MultST3a",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultST3a",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}

								if(cfg_script.Get("specularMat3",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("specularMat3",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}
								if(cfg_script.Get("specularMat3A",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("specularMat3A",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}

								if(cfg_script.Get("MIN_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("MAG_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("WRAP_MODE",strbuf))
								{
									crTexture::WrapMode wrap = getWrapModeFromStr(strbuf);
									texture->setWrap(crTexture::WRAP_S,wrap);
									texture->setWrap(crTexture::WRAP_T,wrap);
									texture->setWrap(crTexture::WRAP_R,wrap);
								}
							}
						}
						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter: "<<cfg_script.GetLastError()<<std::endl;
					}
					if(cfg_script.Push("EmissiveMap3"))
					{
						if(cfg_script.Get("EmissiveMap3",strbuf))
						{
							stateset->removeTextureAttribute(TEXTURE_EMISSIVEMAP3,crStateAttribute::TEXTURE);
							if(strbuf.compare("NULL")!=0)
							{
								texture = CRCore::crBrain::getInstance()->getTexture2D(strbuf);
								if(texture)
								{
									if(CRCore::crBrain::getInstance()->getNeedLoadImage2D(texture))
									{
										texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									}
								}
								else
								{
									texture = new crTexture2D;
									texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									CRCore::crBrain::getInstance()->insertTexture2D(strbuf,texture);
								}
								stateset->setTextureAttributeAndModes(TEXTURE_EMISSIVEMAP3,texture,crStateAttribute::ON);

								if(cfg_script.Get("OpET3",strbuf))
								{
									stateset->setTextureOperate(TEXTURE_EMISSIVEMAP3,getTextureOperateFromStr(strbuf));
								}
								if(cfg_script.Get("UVLayer",int1))
								{
									stateset->setTextureUV(TEXTURE_EMISSIVEMAP3,int1);
								}
								if(cfg_script.Get("param4AddET3",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != ao4)
									{
										uniform = stateset->getOrCreateUniform("param4AddET3",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}
								if(cfg_script.Get("param1MultET3",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultET3",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param4MultET3",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != mo4)
									{
										uniform = stateset->getOrCreateUniform("param4MultET3",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}

								if(cfg_script.Get("OpET3A",strbuf))
								{
									stateset->setAlphaOperate(TEXTURE_EMISSIVEMAP3,getAlphaOperateFromStr(strbuf));
								}
								if(cfg_script.Get("param1AddET3a",flt1))
								{
									if(flt1 != 0.0f)
									{
										uniform = stateset->getOrCreateUniform("param1AddET3a",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param1MultET3a",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultET3a",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}

								if(cfg_script.Get("emissiveMat3",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("emissiveMat3",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}
								if(cfg_script.Get("emissiveMat3A",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("emissiveMat3A",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}

								if(cfg_script.Get("MIN_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("MAG_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("WRAP_MODE",strbuf))
								{
									crTexture::WrapMode wrap = getWrapModeFromStr(strbuf);
									texture->setWrap(crTexture::WRAP_S,wrap);
									texture->setWrap(crTexture::WRAP_T,wrap);
									texture->setWrap(crTexture::WRAP_R,wrap);
								}
							}
						}
						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter: "<<cfg_script.GetLastError()<<std::endl;
					}
					////////////ExternTexture
					if(cfg_script.Push("UVSMap"))
					{
						if(cfg_script.Get("UVSMap",strbuf))
						{
							crTexture2D *externTex2D = dynamic_cast<crTexture2D *>(stateset->getTextureAttribute(TEXTURE_UVSCRAMBLER,crStateAttribute::TEXTURE));
							if(externTex2D)
							{
								crTexture::ExternTextureID externTextureID = externTex2D->getExternTextureID();
								if(externTextureID == crTexture::UVSCRAMBLER)
									stateset->removeTextureAttribute(TEXTURE_UVSCRAMBLER,crStateAttribute::TEXTURE);
							}
							if(strbuf.compare("NULL")!=0)
							{
								std::string extImageNameID = strbuf + "_UVS";
								texture = CRCore::crBrain::getInstance()->getTexture2D(extImageNameID);
								if(texture)
								{
									texture->setExternTextureID(crTexture::UVSCRAMBLER);
									if(CRCore::crBrain::getInstance()->getNeedLoadImage2D(texture))
									{
										texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									}
								}
								else
								{
									texture = new crTexture2D;
									texture->setExternTextureID(crTexture::UVSCRAMBLER);
									texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									CRCore::crBrain::getInstance()->insertTexture2D(extImageNameID,texture);
								}
								stateset->setTextureAttributeAndModes(TEXTURE_UVSCRAMBLER,texture,crStateAttribute::ON);
								if(cfg_script.Get("UVLayer",int1))
								{
									stateset->setTextureUV(TEXTURE_UVSCRAMBLER,int1);
								}
								if(cfg_script.Get("param4AddUVS",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != ao4)
									{
										uniform = stateset->getOrCreateUniform("param4AddUVS",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}
								if(cfg_script.Get("param1MultUVS",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultUVS",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param4MultUVS",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != mo4)
									{
										uniform = stateset->getOrCreateUniform("param4MultUVS",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}

								if(cfg_script.Get("UVSMat",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("UVSMat",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}

								if(cfg_script.Get("MIN_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("MAG_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("WRAP_MODE",strbuf))
								{
									crTexture::WrapMode wrap = getWrapModeFromStr(strbuf);
									texture->setWrap(crTexture::WRAP_S,wrap);
									texture->setWrap(crTexture::WRAP_T,wrap);
									texture->setWrap(crTexture::WRAP_R,wrap);
								}
							}
						}
						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter: "<<cfg_script.GetLastError()<<std::endl;
					}
					if(cfg_script.Push("CWMap"))
					{
						if(cfg_script.Get("CWMap",strbuf))
						{
							crTexture2D *externTex2D = dynamic_cast<crTexture2D *>(stateset->getTextureAttribute(TEXTURE_COLORWEIGHT,crStateAttribute::TEXTURE));
							if(externTex2D)
							{
								crTexture::ExternTextureID externTextureID = externTex2D->getExternTextureID();
								if(externTextureID == crTexture::COLORWEIGHT)
									stateset->removeTextureAttribute(TEXTURE_COLORWEIGHT,crStateAttribute::TEXTURE);
							}
							if(strbuf.compare("NULL")!=0)
							{
								std::string extImageNameID = strbuf + "_CW";
								texture = CRCore::crBrain::getInstance()->getTexture2D(extImageNameID);
								if(texture)
								{
									texture->setExternTextureID(crTexture::COLORWEIGHT);
									if(CRCore::crBrain::getInstance()->getNeedLoadImage2D(texture))
									{
										texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									}
								}
								else
								{
									texture = new crTexture2D;
									texture->setExternTextureID(crTexture::COLORWEIGHT);
									texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(strbuf,texture));
									CRCore::crBrain::getInstance()->insertTexture2D(extImageNameID,texture);
								}
								stateset->setTextureAttributeAndModes(TEXTURE_COLORWEIGHT,texture,crStateAttribute::ON);
								if(cfg_script.Get("UVLayer",int1))
								{
									stateset->setTextureUV(TEXTURE_COLORWEIGHT,int1);
								}
								if(cfg_script.Get("param4AddCW",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != ao4)
									{
										uniform = stateset->getOrCreateUniform("param4AddCW",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}
								if(cfg_script.Get("param1MultCW",flt1))
								{
									if(flt1 != 1.0f)
									{
										uniform = stateset->getOrCreateUniform("param1MultCW",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(flt1);
									}
								}
								if(cfg_script.Get("param4MultCW",v_i))
								{
									vec4.set(v_i[0],v_i[1],v_i[2],v_i[3]);
									if(vec4 != mo4)
									{
										uniform = stateset->getOrCreateUniform("param4MultCW",CRCore::crUniform::FLOAT_VEC4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec4);
									}
								}
								if(cfg_script.Get("param3MultCWN",v_i))
								{
									vec3.set(v_i[0],v_i[1],v_i[2]);
									if(vec3 != mo3)
									{
										uniform = stateset->getOrCreateUniform("param3MultCWN",CRCore::crUniform::FLOAT_VEC3);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec3);
									}
								}
								if(cfg_script.Get("param3MultCWD",v_i))
								{
									vec3.set(v_i[0],v_i[1],v_i[2]);
									if(vec3 != mo3)
									{
										uniform = stateset->getOrCreateUniform("param3MultCWD",CRCore::crUniform::FLOAT_VEC3);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec3);
									}
								}
								if(cfg_script.Get("param3MultCWDa",v_i))
								{
									vec3.set(v_i[0],v_i[1],v_i[2]);
									if(vec3 != mo3)
									{
										uniform = stateset->getOrCreateUniform("param3MultCWDa",CRCore::crUniform::FLOAT_VEC3);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec3);
									}
								}
								if(cfg_script.Get("param3MultCWS",v_i))
								{
									vec3.set(v_i[0],v_i[1],v_i[2]);
									if(vec3 != mo3)
									{
										uniform = stateset->getOrCreateUniform("param3MultCWS",CRCore::crUniform::FLOAT_VEC3);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec3);
									}
								}
								if(cfg_script.Get("param3MultCWSa",v_i))
								{
									vec3.set(v_i[0],v_i[1],v_i[2]);
									if(vec3 != mo3)
									{
										uniform = stateset->getOrCreateUniform("param3MultCWSa",CRCore::crUniform::FLOAT_VEC3);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec3);
									}
								}
								if(cfg_script.Get("param3MultCWE",v_i))
								{
									vec3.set(v_i[0],v_i[1],v_i[2]);
									if(vec3 != mo3)
									{
										uniform = stateset->getOrCreateUniform("param3MultCWE",CRCore::crUniform::FLOAT_VEC3);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec3);
									}
								}
								if(cfg_script.Get("param3MultCWEa",v_i))
								{
									vec3.set(v_i[0],v_i[1],v_i[2]);
									if(vec3 != mo3)
									{
										uniform = stateset->getOrCreateUniform("param3MultCWEa",CRCore::crUniform::FLOAT_VEC3);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(vec3);
									}
								}
								//if(cfg_script.Get("uvw",int1))
								//{
								//	if(int1 == 0)
								//	{
								//		uniform = stateset->getOrCreateUniform("uvw",CRCore::crUniform::BOOL);
								//		uniform->setDataVariance(crBase::STATIC);
								//		uniform->set(false);
								//	}
								//}
								if(cfg_script.Get("CWMat",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("CWMat",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}
								if(cfg_script.Get("CWMatR",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("CWMatR",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}
								if(cfg_script.Get("CWMatG",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("CWMatG",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}
								if(cfg_script.Get("CWMatB",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("CWMatB",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}
								if(cfg_script.Get("CWMatA",v_i))
								{
									vec2.set(v_i[0],v_i[1]);
									if(vec2 != crVector2(1,1))
									{
										uniform = stateset->getOrCreateUniform("CWMatA",CRCore::crUniform::FLOAT_MAT4);
										uniform->setDataVariance(crBase::STATIC);
										uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
									}
								}

								if(cfg_script.Get("MIN_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("MAG_FILTER",strbuf))
								{
									texture->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(strbuf));
								}
								if(cfg_script.Get("WRAP_MODE",strbuf))
								{
									crTexture::WrapMode wrap = getWrapModeFromStr(strbuf);
									texture->setWrap(crTexture::WRAP_S,wrap);
									texture->setWrap(crTexture::WRAP_T,wrap);
									texture->setWrap(crTexture::WRAP_R,wrap);
								}
							}
						}
						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter: "<<cfg_script.GetLastError()<<std::endl;
					}
					//////////////////////////////////

					CRCore::ref_ptr<CRCore::crStateSet> exist_stateset = CRCore::crBrain::getInstance()->checkStateSetExistAndGet(stateset.get());
					if(exist_stateset.valid())
					{
						stateset = exist_stateset;
					}
					else
						CRCore::crBrain::getInstance()->insertStateSet(stateset.get());
					subMeshDrawable->setStateSet(stateset.get());
				}
				//else
				//{
				//	CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter not find Mesh "<<meshName<<std::endl;
				//}
				if (!cfg_script.Pop())
					CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter: "<<cfg_script.GetLastError()<<std::endl;
			}
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"ReplicantBodyMgr::parseCharacter: "<<cfg_script.GetLastError()<<std::endl;
		}
	}
}
//void CreBodyNode::setEnableIntersect(bool enableIntersect)
//{
//	crCharacterMatterObject::setEnableIntersect(enableIntersect);
//}

//void CreBodyNode::setRotation(float inc)
//{
//    CRCore::crMatrix m;
//	m.makeRotate(inc, CRCore::Z_AXIS);
//    preMult(m);
//}
float CreBodyNode::getScale()
{
	const crMatrix& mat = m_body_scale->getMatrix();
	return mat.getScale()[0];
}
void CreBodyNode::setScale(float factor)
{
    //getBody()->setScale(factor);
    // Scale model according to new scale factor
    CRCore::crMatrix m;
    m.makeIdentity();
    //float scale = getBody()->getScale();
    m.makeScale(factor, factor, factor);
    m_body_scale->setMatrix(m);
}

CRCore::crMatrixTransform *CreBodyNode::getBodyTransform()
{
    return m_body_scale;
}
//
//void CreBodyNode::updateCharacterLod(float sqrDistance)
//{
//	float lodLevel = (2500.0f - sqrDistance) / 2025.0f;
//	////// clamp the value to [0.0, 1.0]
//	if(lodLevel < 0) lodLevel = 0;
//	if(lodLevel > 1.0f) lodLevel = 1.0f;
//
//	getBody()->setLodPoly(lodLevel);
//}
void CreBodyNode::setLodLevel(float lod)
{
	m_lodLevel = CRCore::clampTo(lod,0.0f,1.0f);
}
void CreBodyNode::traverse(CRCore::crNodeVisitor& nv)
{
	//if(nv.getTraverseString().compare("WaterReflectNode") == 0)
	//{
	//	CRPhysics::crCharacterMatterObject::traverse(nv);
	//	return;
	//}
	switch(nv.getVisitorType()) 
	{
	case CRCore::crNodeVisitor::UPDATE_VISITOR:
		{//根据更新后的数据重新设置geomid，保证z最小值不变
/*			//rbody::ActionRequest *action=0;
			//if(m_move_strafeSpeed[0] == 0.0f && m_move_strafeSpeed[1] == 0.0f)
			//{
			//	action = m_body->getActionPrototype( "ACT_STAND" );
			//	if (action) 
			//	{
			//		action->setPrioritized(false);
			//		m_body->executeAction(action, false);
			//	}
			//}
			const CRCore::crBoundingBox &bbox = m_body_scale->getBoundBox();
			//float radius = m_body_scale->getBound().radius();
			//radius -= 0.2f;
			//radius *= radius < 0.8f ? 0.67382f : 0.58f;
			//dGeomSphereSetRadius(m_geomID,radius);
			dGeomBoxSetLengths(m_geomID,bbox.xLength(),bbox.yLength(),bbox.zLength());
			dMass newMass;
			//dMassSetSphereTotal (&newMass,m_mass,radius);
			dMassSetBoxTotal (&newMass,m_mass,bbox.xLength(),bbox.yLength(),bbox.zLength());
			dMassTranslate (&newMass,m_barycenter[0],m_barycenter[1],m_barycenter[2]);
			dBodySetMass (m_bodyID,&newMass);
			//dBodyDisable(m_bodyID);
			m_viewHeight = bbox.zLength() * 0.5f;
			//CRCore::notify(CRCore::ALWAYS)<<"m_viewHeight = "<<m_viewHeight<<std::endl;
			
			//updateWeaponMatrix();
 */       
			//m_body_scale->postMult(crMatrix::translate(-m_body_scale->getBound().center()));
			if(!m_safeReleaseVec.empty())
			{
				SubMeshDrawable *subMeshDrawable;
				std::set<int> deleteMesh;
				std::set<int>::iterator deleteItr;
				for( DrawableVec::iterator itr = m_safeReleaseVec.begin();
					itr != m_safeReleaseVec.end();
					++itr )
				{
					subMeshDrawable = dynamic_cast<SubMeshDrawable *>(itr->get());
					if(subMeshDrawable)
					{
						deleteMesh.insert(subMeshDrawable->getMeshID());
					}
					(*itr)->releaseObjects();
				}
                for( deleteItr = deleteMesh.begin();
					 deleteItr != deleteMesh.end();
					 ++deleteItr )
				{
					m_body->deleteMesh(*deleteItr);
					eraseGeometry(*deleteItr);
					//getGeomContainer()->erase(this,*deleteItr);
				}
				m_safeReleaseVec.clear();
			}
			if(!m_releaseDrawableVec.empty())
			{
				m_safeReleaseVec.swap(m_releaseDrawableVec);
				//CRCore::notify(CRCore::NOTICE)<<"  m_releaseDrawableVec = "<<m_releaseDrawableVec.size()<<std::endl;
				//m_releaseDrawableVec.clear();
			}
			if(!m_requestRemoveDrawableVec.empty())
			{
				m_subMeshDrawableListMutex.lock();
				for( DrawableVec::iterator itr = m_requestRemoveDrawableVec.begin();
					itr != m_requestRemoveDrawableVec.end();
					++itr )
				{
					//m_body_geode->removeDrawable(itr->get());
					m_body_scale->removeChild((*itr)->getParent(0));
					for( crObject::DrawableList::iterator sitr = m_subMeshDrawableList.begin();
						sitr != m_subMeshDrawableList.end();
						)
					{
						if((*sitr)->getPuppetGeometry() == itr->get() || (*sitr)->getPuppetBufGeometry() == itr->get())
						{
							sitr = m_subMeshDrawableList.erase(sitr);
						}
						else
						{
							++sitr;
						}
					}
				}
				m_subMeshDrawableListMutex.unlock();
				m_releaseDrawableVec.swap(m_requestRemoveDrawableVec);
				//CRCore::notify(CRCore::NOTICE)<<"  m_requestRemoveDrawableVec = "<<m_requestRemoveDrawableVec.size()<<std::endl;
				//m_requestRemoveDrawableVec.clear();
			}
			if(m_requestInitDrawables)
			{
				initDrawables();//需要修改
				m_body_scale->setRenderInited(false);
				CRIOManager::crLoadManager::getInstance()->requestCompile(m_body_scale);
			}
			//dirtyBound();
			//float distance = (nv.getEyePoint() - getBound().center()).length2();
			//float lodLevel = (2500.0f - distance) / 2025.0f;
			////// clamp the value to [0.0, 1.0]
			//if(lodLevel < 0.1) lodLevel = 0.1f;
			//if(lodLevel > 1.0f) lodLevel = 1.0f;

			//getBody()->setLodPoly(lodLevel);
			//CRCore::notify(CRCore::ALWAYS)<<"CreBodyNode::traverse UPDATE_VISITOR lodLevel = "<<lodLevel<<std::endl;
		}
		break;
	case CRCore::crNodeVisitor::SWAPBUFFER_VISITOR:
		if(!m_swapVaild)
			return;
		else
			m_swapVaild = false;
		break;
	//case CRCore::crNodeVisitor::CULL_VISITOR://由crCreBodyCullCallback实现
	//	{//do update, when body is in FOV
	//	    m_needUpdate = true;
 //           //update(nv.getTraversalNumber());
	//	}
	//	break;
	//case CRCore::crNodeVisitor::CULL_VISITOR:
	//	{//根据与视点远近设置合理的lod参数
	//		//crCullVisitor *cv = dynamic_cast<crCullVisitor *>(&nv);
	//		//if(cv->getRenderMode()>=crCullVisitor::ShadowMapRender) 
	//		//{
	//		//	crGroup::traverse(nv);
	//		//	return;
	//		//}
	//		//float distance = fabsf(nv.getDistanceToEyePoint(m_matrix.getTrans(),false));
	//		//float lodLevel = (50.0f - distance) / 40.0f;
	//		////// clamp the value to [0.0, 1.0]
	//		//if(lodLevel < 0.1) lodLevel = 0.1f;
	//		//if(lodLevel > 1.0f) lodLevel = 1.0f;
	//		//getBody()->setLodPoly(lodLevel);
	//		////m_lodLevel = lodLevel;
	//		//if(getCameraBind())
	//		//{
	//		//	CRCore::notify(ALWAYS)<<"CULL_VISITOR:distance = "<<distance<<std::endl;
	//		//	CRCore::notify(ALWAYS)<<"CULL_VISITOR:cameraPos = "<<nv.getEyePoint()<<std::endl;
	//		//}
	//		/*if(!m_enableTangentSpace) break;

	//		//crVector3 center = -m_body_scale->getBound().center();
	//		//center[2] = 0.0f;
	//		//m_body_scale->postMult(crMatrix::translate(center));


	//		m_noNeedTangentSpace = distance > 50.0f;
	//		if(!m_noNeedTangentSpace)
	//		{
	//			crVector3 lv = cv->getLookVectorLocal();
	//			crVector3 aa = m_matrix.getTrans() - cv->getEyeLocal();
	//			m_noNeedTangentSpace = (aa.normalize() * lv.normalize() < 0.4f);//设视角为60度
	//		}
 //           */
	//		//SubMeshDrawable *submesh_drawable;
	//		//for( crObject::DrawableList::iterator itr = m_body_geode->getDrawableList().begin();
	//		//	 itr != m_body_geode->getDrawableList().end();
	//		//	 ++itr )
	//		//{
	//		//	submesh_drawable = dynamic_cast<SubMeshDrawable *>(itr->get());
 //  //             if(submesh_drawable)
	//		//	{
	//		//		submesh_drawable->setNoNeedTangentSpaceFlag(noNeedTangentSpace);
	//		//	}
	//		//}
	//	}
	//	break;
	//case CRCore::crNodeVisitor::OPT_PHYSICSCHECK_VISITOR:
	//	{
	//		CRUtil::crOptimizer::crCheckPhysicsVisitor* chkPhysics = dynamic_cast<CRUtil::crOptimizer::crCheckPhysicsVisitor*>(&nv);
	//		if(chkPhysics)
	//		{
	//			m_body_scale->postMult(chkPhysics->topMatrix());
	//		}
	//	}
	//	break;
	}

	CRCore::crGroup::traverse(nv);
}

//void CreBodyNode::_create(dWorldID worldID, dSpaceID spaceID)
//{
////	postMult(crMatrix::translate(m_initPosition[0],m_initPosition[1],m_initPosition[2]));
//
//	dMass m;
//	m_bodyID = dBodyCreate(worldID);
//	crVector3f box(m_bbox.xLength(),m_bbox.yLength(),m_bbox.zLength());
//	dMassSetBox (&m,m_density,box[0],box[1],box[2]);
//	dMassAdjust (&m,m_mass);
//	dMassTranslate(&m,m_barycenter[0],m_barycenter[1],m_barycenter[2]);
//	dBodySetMass (m_bodyID,&m);
//	m_geomID = dCreateBox(spaceID,box[0],box[1],box[2]);
//	dGeomSetBody (m_geomID,m_bodyID);
//	dGeomSetPosition (m_geomID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
//	dBodySetPosition (m_bodyID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
//
//	crOdeWorld::getDynamicGeomMap()[m_geomID] = this;
//
//	crCharacterMatterObject::_create(worldID,spaceID);
//
//	crVector3f center = m_bbox.center();
//	m_viewHeight = m_bbox.zLength() * 0.5f;
//	//CRCore::notify(CRCore::ALWAYS)<<"center = "<<center<<std::endl;
//	m_matrix.setTrans(center);
//	dGeomSetPosition (m_sphereGeomID,center[0],center[1],center[2] + m_viewHeight - 0.55f);
//	crVector3f dir = -getDirection();
//	dGeomRaySet(m_fowardRayGeomID,center[0],center[1],center[2]-m_viewHeight+0.3f,dir[0],dir[1],0.0f);
//	dGeomRaySet(m_groundRayGeomID,center[0],center[1],center[2],0.0f,0.0f,-1.0f);
//
//	if(m_enableIntersect)
//	    dGeomEnable(m_geomID);
//	else
//		dGeomDisable(m_geomID);
//	dBodyDisable(m_bodyID);
//}
//
//crCharacterMatterObject::CollideBody CreBodyNode::getCollideBody(const CRCore::crVector3f& collidePos)
//{
//	//m_body->get
//	//CalBone *bone;
//	//bone->getRotation();
//	return Collide_None;
//}

static void _MakeMatrix(const crVector3 &CT, const CalQuaternion &CQ, CRCore::crMatrix &mat)
{
	float x= CQ.x, y= CQ.y, z= CQ.z, w= CQ.w;
	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
	// calculate coefficients 
	x2 = x + x;
	y2 = y + y;
	z2 = z + z;
	xx = x * x2;
	xy = x * y2;
	xz = x * z2;
	yy = y * y2;
	yz = y * z2;
	zz = z * z2;
	wx = w * x2;
	wy = w * y2;
	wz = w * z2;

	mat(0,0) = 1.0f - (yy + zz);
	mat(0,1) = xy - wz;
	mat(0,2) = xz + wy;
	mat(0,3) = 0.0f;

	mat(1,0) = xy + wz;
	mat(1,1) = 1.0f - (xx + zz);
	mat(1,2) = yz - wx;
	mat(1,3) = 0.0f;

	mat(2,0) = xz - wy;
	mat(2,1) = yz + wx;
	mat(2,2) = 1.0f - (xx + yy);
	mat(2,3) = 0.0f;

	mat(3,0) = CT.x();
	mat(3,1) = CT.y();
	mat(3,2) = CT.z();
	mat(3,3) = 1.0f;
}

void CreBodyNode::getBoneMatrix(const std::string &boneName, CRCore::crMatrix &mat)
{
	//CalCoreModel *cal3d_core_model = m_body->getCoreBody()->getCal3dCoreModel();
	//CalCoreBone *bone;
	//int id = cal3d_core_model->getBoneId(boneName);
    CalModel *model = m_body->getCalModel();
	int boneid	= model->getSkeleton()->getCoreSkeleton()->getCoreBoneId(boneName);
	CalBone *bone = NULL;	
	if(boneid!=-1)
	{
		bone = model->getSkeleton()->getBone(boneid);
		const CalVector &trans = bone->getTranslationAbsolute();
		crVector3 t(trans[0],trans[1],trans[2]);
		const crMatrix &bodyMatrix = m_body_scale->getMatrix();
		t = t * bodyMatrix;
		////const CalQuaternion& rotBoneSpace = bone->getRotationBoneSpace();
		////CRCore::crQuat rot(rotBoneSpace[0],rotBoneSpace[1],rotBoneSpace[2],rotBoneSpace[3]);
		////mat.makeRotate(rot);
		////mat.setTrans(t);
		////float angle = getBody()->getCoreBody()->getRotAngle();
		//crQuat q = bodyMatrix.getRotate();
		////q.makeRotate(CRCore::DegreesToRadians(angle),CRCore::Z_AXIS);
		//CalQuaternion calRot(q[0],q[1],q[2],q[3]);
		//float angle = getBody()->getCoreBody()->getRotAngle();
		//if(angle = 0.0f)
		//{
			_MakeMatrix(t, bone->getRotationBoneSpace()/* * calRot*/,mat);
		//}
		//else
		//{
		//	_MakeMatrix(CRCore::crVector3(0.0f,0.0f,0.0f), bone->getRotationBoneSpace()/* * calRot*/,mat);
		//	mat.preMult(CRCore::crMatrix::rotate(CRCore::DegreesToRadians(angle),CRCore::Z_AXIS));
		//	mat.setTrans(t);
		//}
		//CRCore::notify(CRCore::ALWAYS)<<"getBoneMatrix mat = "<<mat<<std::endl;
	}
}
void CreBodyNode::rotateBoneMatrix(const std::string &boneName, const CRCore::crQuat &rot)
{
	CalModel *model = m_body->getCalModel();
	int boneid	= model->getSkeleton()->getCoreSkeleton()->getCoreBoneId(boneName);
	CalBone *bone = NULL;	
	if(boneid!=-1)
	{
		bone = model->getSkeleton()->getBone(boneid);
		int parentId = bone->getCoreBone()->getParentId();
		CalQuaternion calRot(rot[0],rot[1],rot[2],rot[3]);
		if(parentId != -1)
		{
			CalBone *pParent = model->getSkeleton()->getBone(parentId);
			CalQuaternion parentRotation = pParent->getCoreBone()->getRotation();
			CalQuaternion invertRot = parentRotation;
			invertRot.invert();
			calRot = invertRot * calRot * parentRotation;
		}
		bone->setRotationExtern(calRot);
		needUpdate();
	}
}

std::string CreBodyNode::getCurrentPartMeshName(int partid)
{
    return m_body->getCoreBody()->getCurrentPartMeshName(partid);
}

void CreBodyNode::deleteMesh(const std::string &meshName)
{
	//getGeomContainer()->requestRemoveGeom(this,m_body->getCoreBody()->getCal3dMeshId(meshName));
	requestRemoveGeom(m_body->getCoreBody()->getCal3dMeshId(meshName));
	m_body->detachMesh(meshName);
    //m_body->deleteMesh(meshName);//可能存在线程不安全
}

void CreBodyNode::requestRemoveGeom(CRCore::crGeometry *geom)
{
	//crObject::DrawableList &drawableList = m_body_geode->getDrawableList();
	CRCore::ScopedLock<CRCore::crMutex> lock(m_subMeshDrawableListMutex);
	for( crObject::DrawableList::iterator itr = m_subMeshDrawableList.begin();
		 itr != m_subMeshDrawableList.end();
		 ++itr )
	{
		if((*itr)->getPuppetGeometry() == geom || (*itr)->getPuppetBufGeometry() == geom)
		{
            m_requestRemoveDrawableVec.push_back(itr->get());
		}
	}
}

CRCore::crDrawable *CreBodyNode::getSubMeshDrawable(const std::string &meshName)
{
	//crObject::DrawableList &drawableList = m_body_geode->getDrawableList();
	CRCore::ScopedLock<CRCore::crMutex> lock(m_subMeshDrawableListMutex);
	for( crObject::DrawableList::iterator itr = m_subMeshDrawableList.begin();
		itr != m_subMeshDrawableList.end();
		++itr )
	{
		if((*itr)->getName() == meshName)
		{
			return itr->get();
		}
	}
	return NULL;
}

bool CreBodyNode::findMesh(const std::string &meshName)
{
    return m_body->getCoreBody()->findMesh(meshName);
}
std::string CreBodyNode::getMeshMaterialFileName(int meshid)
{
	return m_body->getCoreBody()->getMeshMaterialFileName(meshid);
}
int CreBodyNode::getCurrentMaterialSize()
{
    return m_body->getCoreBody()->getCal3dCoreModel()->getCoreMaterialCount();
}
void CreBodyNode::addMaterial(const std::string &fileName)
{
	int cal3d_id = m_body->getCoreBody()->getCal3dCoreModel()->loadCoreMaterial(fileName);
	if (cal3d_id == -1)
		throw (config_error("CreBodyNode::addMaterial(): " + CalError::getLastErrorDescription() + " (" + fileName + ")"));
}
int CreBodyNode::addMaterialSet(MaterialSet *material_set)
{
	return m_body->getCoreBody()->addMaterialSet(material_set);
}
void CreBodyNode::attachMesh(const std::string &meshName)
{
	if(m_body->attachMesh(meshName))
	    m_requestInitDrawables = true;
}
void CreBodyNode::addMesh(Mesh *mesh/*,int materialsetid*/)
{
	if(m_body->getCoreBody()->findMesh(mesh->getName()))
	{
		delete mesh;
		return;
	}
	m_body->addMesh(mesh/*,materialsetid*/);
	m_requestInitDrawables = true;
}

void CreBodyNode::checkAndSetBodyMeshSet(const Body::MeshIDSet &meshIDSet)
{
	const Body::MeshIDSet &myMeshSet = getBody()->getBodyMeshSet();
	for( Body::MeshIDSet::const_iterator itr = myMeshSet.begin();
		 itr != myMeshSet.end();
		 ++itr )
	{
		if(meshIDSet.find(*itr)==meshIDSet.end())
		{
			//getGeomContainer()->requestRemoveGeom(this,*itr);
			requestRemoveGeom(*itr);
			getBody()->detachMesh(*itr);
		}
	}
	
	for( Body::MeshIDSet::const_iterator itr = meshIDSet.begin();
		 itr != meshIDSet.end();
		 ++itr )
	{
        if(myMeshSet.find(*itr)==myMeshSet.end())
		{
			getBody()->attachMesh(*itr);
			m_requestInitDrawables = true;
		}
	}
}

void CreBodyNode::deleteAnimation(AnimationBase *anim)
{
	m_body->deleteAnimation(anim);
}

void CreBodyNode::addAnimation(AnimationBase *anim)
{
	CoreBody* coreBody = m_body->getCoreBody();
    AnimationBase *existAnim = coreBody->getAnimation(anim->getName());
	if(existAnim)
	{
		m_body->attachAct(anim->getActName());
		delete anim;
		return;
	}
	std::list<AnimationBase *> act_anim_list;
    coreBody->getAnimationsWithActName(anim->getActName(),act_anim_list);
	for( std::list<AnimationBase *>::iterator itr = act_anim_list.begin();
		 itr != act_anim_list.end();
		 ++itr )
	{//动作替换
        deleteAnimation(*itr);
	}
    coreBody->addAnimation(anim);
	if (anim->getType() == AnimationBase::REAL)
	{
	    m_body->getCalModel()->getMixer()->getAnimationVector().push_back(0);
	}
	m_body->attachAct(anim->getActName());
	coreBody->buildActionPrototypes();
}
void CreBodyNode::setAnimPlaySpeed(float speed)
{
    m_animPlaySpeed = speed;
}
float CreBodyNode::getAnimPlaySpeed() const
{
    return m_animPlaySpeed;
}
void CreBodyNode::update(/*int fn*/)
{//do update, when body is in FOV
	//crObject::DrawableList &drawableList = m_body_geode->getDrawableList();
#if _DEBUG
	try
	{
#endif
	if(m_body.valid())
	{
		SubMeshDrawable *subMeshDrawable;
		CRCore::ScopedLock<CRCore::crMutex> lock(m_subMeshDrawableListMutex);
		for( crObject::DrawableList::iterator itr = m_subMeshDrawableList.begin();
			itr != m_subMeshDrawableList.end();
			++itr )
		{
			subMeshDrawable = dynamic_cast<SubMeshDrawable *>(itr->get());
			if(subMeshDrawable) subMeshDrawable->update(/*fn*/);
		}
		dirtyBound();
		computeBound();
	}
#if _DEBUG
	}
	catch (...)
	{
		//CRCore::notify(CRCore::ALWAYS)<<"subMeshDrawable->update error "<<std::endl;
		char gbuf[256];
		sprintf(gbuf,"subMeshDrawable->update error\n\0");
		gDebugInfo->debugInfo(CRCore::FATAL,gbuf);
	}
#endif
}
void CreBodyNode::setFileName(const std::string &fileName)
{
	m_fileName = fileName;
}
const std::string &CreBodyNode::getFileName() const
{
	return m_fileName;
}
////GeometryContainer
void CreBodyNode::setGeometry(int mesh, int submesh, CRCore::crGeometry *geom)
{
	for( GeomList::iterator itr = m_geomList.begin();
		 itr != m_geomList.end();
		 itr++ ) 
	{
		if ((itr->m_mesh == mesh) && (itr->m_submesh == submesh)) 
		{
			itr->m_geom = geom;
			return;
		}
	}
	m_geomList.push_back(SubMeshGeom(mesh, submesh, geom));
}

CRCore::crGeometry* CreBodyNode::getGeometry(int mesh, int submesh)
{
	for( GeomList::iterator itr = m_geomList.begin();
		itr != m_geomList.end();
		itr++ ) 
	{
		if ((itr->m_mesh == mesh) && (itr->m_submesh == submesh)) 
		{
			return itr->m_geom.get();
		}
	}
	return NULL;
}

void CreBodyNode::requestRemoveGeom(int mesh)
{
	for( GeomList::iterator itr = m_geomList.begin();
		itr != m_geomList.end();
		itr++ ) 
	{
		if (itr->m_mesh == mesh) 
		{
			requestRemoveGeom(itr->m_geom.get());
		}
	}
}

void CreBodyNode::eraseGeometry(int mesh)
{
	for( GeomList::iterator itr = m_geomList.begin();
		 itr != m_geomList.end(); ) 
	{
		if ((itr->m_mesh == mesh)) 
		{
			itr = m_geomList.erase(itr);
		}
		else itr++;
	}
}
RequestResult CreBodyNode::executeAction(ActionRequest *action, bool force)
{
	if (!force)
	{
		if (crDisplaySettings::instance()->getFreezeRender())
			return UNKNOWN;
		CRCore::crCalcNodeParentsMatrixVisitor parentMatrix;
		accept(parentMatrix);
		crBoundingBox bbox = getBoundBox();
		bbox.m_max = bbox.m_max * parentMatrix.getResult();
		bbox.m_min = bbox.m_min * parentMatrix.getResult();
		bbox.correct();
		if (!crBrain::getInstance()->getCameraFrustum().contains(bbox))
			return UNKNOWN;
	}
	return getBody()->executeAction(action, force);
}
bool CreBodyNode::computeBound() const 
{   
  // Instead get bounding box from bones (fast)
  CRCore::crVector3 min, max;
  m_body->getCalModel()->getSkeleton()->getBoneBoundingBox(m_bbox.m_min.ptr(), m_bbox.m_max.ptr());

  if (!m_bbox.valid())
    CRCore::notify(CRCore::WARN) << "******* CreBodyNode::computeBound() problem" << std::endl;
 
  m_boundSphere.init();
  m_boundSphere.expandBy(m_bbox);

  crMatrix l2w;

  m_body_scale->computeLocalToWorldMatrix(l2w,NULL);

  m_bbox.m_max = m_bbox.m_max * l2w;
  m_bbox.m_min = m_bbox.m_min * l2w;

  m_bbox.correct();

  crVector3 xdash = m_boundSphere.m_center;
  xdash.x() += m_boundSphere.m_fRadius;
  xdash = xdash*l2w;

  crVector3 ydash = m_boundSphere.m_center;
  ydash.y() += m_boundSphere.m_fRadius;
  ydash = ydash*l2w;

  crVector3 zdash = m_boundSphere.m_center;
  zdash.z() += m_boundSphere.m_fRadius;
  zdash = zdash*l2w;

  m_boundSphere.m_center = m_boundSphere.m_center*l2w;

  xdash -= m_boundSphere.m_center;
  float len_xdash = xdash.length();

  ydash -= m_boundSphere.m_center;
  float len_ydash = ydash.length();

  zdash -= m_boundSphere.m_center;
  float len_zdash = zdash.length();

  m_boundSphere.m_fRadius = len_xdash;
  if (m_boundSphere.m_fRadius<len_ydash) m_boundSphere.m_fRadius = len_ydash;
  if (m_boundSphere.m_fRadius<len_zdash) m_boundSphere.m_fRadius = len_zdash;

  m_bBoundSphere_computed = true;
  //CRCore::notify(CRCore::WARN) << "CreBodyNode::computeBound m_min "<<m_bbox.m_min<<" m_max "<<m_bbox.m_max<< std::endl;
  //CRCore::notify(CRCore::WARN) << "CreBodyNode::computeBound() "<<m_boundSphere.m_fRadius<< std::endl;

  return true;
}
//
//CreBodyNode::DetailCollide CreBodyNode::getDetailCollide(const CRCore::crVector3f& collidePos, const CRCore::crMatrix &botmatrix)
//{
//    return parseDetailCollide(getCollideBoneName(collidePos,botmatrix));
//}
//
//std::string CreBodyNode::getCollideBoneName(const CRCore::crVector3f& collidePos, const CRCore::crMatrix &botmatrix)
//{
//	CalSkeleton *pCalSkeleton = m_body->getCalModel()->getSkeleton();
//
//	std::vector<CalBone*> &vectorCoreBone = pCalSkeleton->getVectorBone();
//
//	//crVector3f pos = collidePos * m_body_scale->getMatrix();
// //   CalVector p;
//	//p.set(collidePos[0],collidePos[1],collidePos[2]);
//	//bool contain = false;
//	crBoundingBox bbox;
//	float *min = bbox.m_min.ptr(); float *max = bbox.m_max.ptr();
//	crMatrix parentMatrix = m_body_scale->getMatrix() * botmatrix;
//	CalVector p[8];
//	//CRCore::notify(CRCore::WARN) << "CreBodyNode::collidePos "<<collidePos<< std::endl;
//	for(size_t boneId=0;boneId<vectorCoreBone.size();++boneId)
//	{
//		CalBoundingBox & calBoundingBox = vectorCoreBone[boneId]->getBoundingBox();
//		calBoundingBox.computePoints(p);
//		min[0] = max[0] = p[0].x;
//		min[1] = max[1] = p[1].y;
//		min[2] = max[2] = p[2].z;
//		for(int i = 1; i<8; i++)
//		{
//			if (p[i].x > max[0])
//				max[0] = p[i].x;
//			else if (p[i].x < min[0])
//				min[0] = p[i].x;
//
//			if (p[i].y > max[1])
//				max[1] = p[i].y;
//			else if (p[i].y < min[1])
//				min[1] = p[i].y;
//
//			if (p[i].z > max[2])
//				max[2] = p[i].z;
//			else if (p[i].z < min[2])
//				min[2] = p[i].z;
//		}
//        bbox.m_min = bbox.m_min * parentMatrix;
//		bbox.m_max = bbox.m_max * parentMatrix;
//		bbox.correct();
//		//CRCore::notify(CRCore::WARN) <<vectorCoreBone[boneId]->getCoreBone()->getName()<< std::endl;
//		//CRCore::notify(CRCore::WARN) << "CreBodyNode::m_min "<<bbox.m_min<<" m_max "<<bbox.m_max<< std::endl;
//		//contain = false;
//		//for(int i = 0; i<6; i++)
//		//{
//		//	calBoundingBox.plane[i].d = m_body_scale->getMatrix();
//		//	if (calBoundingBox.plane[i].eval(p)<0.0f)
//		//	{
//  //              contain = false;
//		//		break;
//		//	}
//		//	contain = true;
//		//}
//		if(bbox.contains(collidePos))
//		{
//			//CRCore::notify(CRCore::WARN) << "CreBodyNode::contains "<<vectorCoreBone[boneId]->getCoreBone()->getName()<< std::endl;
//			return vectorCoreBone[boneId]->getCoreBone()->getName();
//		}
//	}
//
//	return "NULL";
//}
//
//void CreBodyNode::insertDetailCollideParseMap(const std::string& boneName, const std::string &detailCollide)
//{
//    DetailCollide dc = Collide_None;
//	if(detailCollide.compare("Collide_Head") == 0)
//		dc = Collide_Head;
//	else if(detailCollide.compare("Collide_Body") == 0)
//		dc = Collide_Body;
//	else if(detailCollide.compare("Collide_LArms") == 0)
//		dc = Collide_LArms;
//	else if(detailCollide.compare("Collide_RArms") == 0)
//		dc = Collide_RArms;
//	else if(detailCollide.compare("Collide_LLegs") == 0)
//        dc = Collide_LLegs;
//	else if(detailCollide.compare("Collide_RLegs") == 0)
//		dc = Collide_RLegs;
//	else if(detailCollide.compare("Collide_LFoots") == 0)
//		dc = Collide_LFoots;
//	else if(detailCollide.compare("Collide_RFoots") == 0)
//		dc = Collide_RFoots;
//	m_detailCollideParseMap.insert(std::make_pair(boneName,dc));
//}
//
//CreBodyNode::DetailCollide CreBodyNode::parseDetailCollide(const std::string& boneName)
//{
//	//CRCore::notify(CRCore::WARN) << "CreBodyNode::parseDetailCollide() "<<boneName<< std::endl;
//
//	DetailCollideParseMap::iterator itr = m_detailCollideParseMap.find(boneName);
//	if(itr!=m_detailCollideParseMap.end())
//		return itr->second;
//	return Collide_None;
//}

//void CreBodyNode::updateWeaponMatrix()
//{
//    if(m_currentWeaponMatterObject.valid())
//	{
//        const std::string &weapon_bone_name = m_body->getCoreBody()->getWeaponBoneName();
//		//const std::string &direct_bone_name = m_body->getCoreBody()->getHeadBoneName();
//        CalCoreModel *cal3d_core_model = m_body->getCoreBody()->getCal3dCoreModel();
//		CalCoreBone *weapon_bone;
//		int id = cal3d_core_model->getBoneId(weapon_bone_name);
//		if(id!=-1)
//		    weapon_bone = cal3d_core_model->getCoreSkeleton()->getVectorCoreBone()[id];
//		//CalCoreBone *direct_bone = cal3d_core_model->getCoreSkeleton()->getVectorCoreBone()[cal3d_core_model->getBoneId(direct_bone_name)];
//        //const CalQuaternion &rotation = direct_bone->getRotationAbsolute();
//		const CalVector &trans = weapon_bone->getTranslationAbsolute();
//		//crQuat rot(rotation[0],rotation[1],rotation[2],rotation[3]);
//		crVector3 t(trans[0],trans[1],trans[2]);
//		t = t * getBody()->getScale();
//		t[2] -= m_viewHeight;
//		//const CRCore::crBoundingBox &bbox = m_body_scale->getBoundBox();
//		//m_body_scale->postMult(crMatrix::translate(-m_body_scale->getBound().center()));
//        //t = t * m_body_scale->getMatrix();
//		//CRCore::notify(CRCore::ALWAYS)<<"CalVector rot = "<<rot<<std::endl;
//		//m_currentWeaponMatterObject->setMatrix(/*CRCore::crMatrixd::rotate(rot) * */crMatrix::translate(t)/* * getViewMatrix()*/);
//		m_currentWeaponMatterObject->setMatrix(crMatrix::translate(t) * getViewMatrix());
//	}
//}
//
//void CreBodyNode::dead()
//{
//    rbody::ActionRequest *action =  getBody()->getActionPrototype( "ACT_STAND" );
//	if (action) 
//	{
//		action->setPrioritized(false);
//		getBody()->executeAction(action, false);
//	}
//	//crCharacterMatterObject::dead();
//}

//void CreBodyNode::renascence()
//{
//	rbody::ActionRequest *action =  getBody()->getActionPrototype( "ACT_STAND" );
//	if (action) 
//	{
//		action->setPrioritized(false);
//		getBody()->executeAction(action, false);
//	}
//
//	crCharacterMatterObject::renascence();
//}

    CreBody::CreBody(/*CRPhysics::crCharacterMatterObject *bodyNode*/)
: Body()//, m_bodyObject(bodyNode)
{
}

CreBody::CreBody(/*CRPhysics::crCharacterMatterObject *bodyNode, */const std::string& core_body_name,const std::string& fileName,const Body::MeshIDSet &meshIDSet)
: Body(core_body_name,fileName,meshIDSet)
{
    initInternalActions( core_body_name );
}

    void
CreBody::initInternalActions(const std::string& core_body_name)
{
    static std::map<std::string,bool,std::less<std::string> > core_initiated;

    //Body::create( core_body_name );

    if ( !core_initiated[core_body_name] ) {

  //      ActionRequest *action = new ActionRequest( "ACT_TURN_ANGLE" );
  //      action->addPropertyFloat("angle" );
  //      m_core_body->addActionPrototype( action );

  //      action = new ActionRequest( "ACT_STRAFE" );
  //      action->addPropertyFloat("speed" );
  //      m_core_body->addActionPrototype( action );

  //      action = new ActionRequest( "ACT_BACK" );
  //      action->addPropertyFloat("speed" );
  //      m_core_body->addActionPrototype( action );

		//action = new ActionRequest( "ACT_JUMP" );
		//m_core_body->addActionPrototype( action );

        core_initiated[core_body_name] = true;
        //std::cerr << "ADDING ACTIONS: " << core_body_name << std::endl;
    }
}

RequestResult CreBody::executeAction( ActionRequest *action, bool force )
{
/*	if (action->getName() == "ACT_STAND")
	{
        //stopAction( "ACT_BACK" );
		//stopAction( "ACT_STRAFE" );
		stopAction( "ACT_WALK" );
		stopAction( "ACT_RUN" );
		//stopAction( "ACT_STRUT" );
		//stopAction( "ACT_JOG" );
		//stopAction( "ACT_CRAWL" );
	}
*/
 //   if ( action->getName() == "ACT_WALK"
 //        || action->getName() == "ACT_RUN"
	//	 || action->getName() == "ACT_STRUT"
	//	 || action->getName() == "ACT_JOG" 
	//	 || action->getName() == "ACT_CRAWL")
	//{
	//	stopAction( "ACT_BACK" );
 //   }

 //   if (action->getName() == "ACT_BACK") 
	//{
 //       m_back_speed = action->getPropertyFloat( "speed" );
 //       if ( m_back_speed == 0.0 ) {
 //           return ::rbody::SUCCESS;
 //       }
 //       if ( !(action = getActionPrototype( "ACT_STAND" )) ) 
	//	{
 //           return ::rbody::UNKNOWN;
 //       }
 //       action->setPrioritized( false );
 //   }
	//stopAction( "ACT_STAND" );
	RequestResult rr = Body::executeAction(action, force);
	//delete action;
    return rr;
}

/*----------------------------------------------------------------------------
  Stop action
  ----------------------------------------------------------------------------*/
RequestResult CreBody::stopAction(const std::string& action)
{
    return Body::stopAction(action);
}

bool CreBody::update( float elapsed_seconds )
{
 //   updateMove( elapsed_seconds );

    return Body::update( elapsed_seconds );
}

//void CreBody::updateMove( double time )
//{
//    EXCEPTION_IF_FALSE( m_bodyObject->getMatrix().valid() );
//    m_bodyObject->preMult(
//            CRCore::crMatrix::translate( m_strafe_speed * time, m_back_speed * time, 0 )
//            * CRCore::crMatrix::rotate( m_turn_angle, 0, 0, 1 ) );
//	m_turn_angle = 0.0;
//    EXCEPTION_IF_FALSE( m_bodyObject->getMatrix().valid() );
//}

CreBody::~CreBody()
{
}