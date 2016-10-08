/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
//Modified by Wucaihua
#include <CRUtil/crSceneView.h>
#include <CRUtil/crUpdateVisitor.h>
//#include <CRUtil/crSetTextureCallBack.h>
#include <CRCore/crTimer.h>
#include <CRCore/crNotify.h>
#include <CRCore/crDB.h>
#include <CRCore/crTexture.h>
#include <CRCore/crFilterRenderManager.h>
#include <CRCore/crStatistics.h>
#include <CRUtil/crRenderInitVisitor.h>
#include <CRPhysics/crPhysicsInitVisitor.h>
#include <CRPhysics/crPhysicsUpdateVisitor.h>

#include <CRCore/crShapeDrawable.h>
//#include <CRCore/crWaterSimulationNode.h>
//#include <CRCore/crDepthSortedObject.h>
#include <CRCore/crRenderToTextureManager.h>
#include <CRCore/crFilterRenderManager.h>
#include <CRCore/crShaderManager.h>
#include <CRCore/crEnvironmentManager.h>
#include <CRPhysics/crOdeWorld.h>
//#include <CRUtil/crAISystemUpdater.h>
#include <rbody/Creature3D/crCharacterSystemUpdater.h>
#include <CRParticle/crParticleSystemUpdater.h>

#include <CRCore/crMultisample.h>
#include <CRCore/crTexMat.h>
#include <CRCore/crBlendColor.h>
#include <CRCore/crBlendEquation.h>
#include <CRCore/crClampColor.h>
#include <CRCore/crFrontFace.h>
#include <CRCore/crStencil.h>
#include <CRCore/crTexEnvFilter.h>
#include <CRCore/crPolygonOffset.h>
#include <iterator>
//#define ShowTime
//#ifdef ShowTime
//#include <CRText/crText.h>
//#endif

using namespace CRCore;
using namespace CRUtil;

//CRCore::ref_ptr<crSceneView::NetWorkUpdateCallback> crSceneView::m_netWorkUpdateCallback;
//CRCore::ref_ptr<crSceneView::SceneManagerUpdater> crSceneView::s_sceneManagerUpdater;
crSceneView::ExternUpdaterVec crSceneView::s_externUpdaterVec;
crSceneView::crSceneView(crDisplaySettings* ds)
{
    m_displaySettings = ds;

    m_fusionDistanceMode = PROPORTIONAL_TO_SCREEN_DISTANCE;
    m_fusionDistanceValue = 1.0f;

    m_lightingMode=NO_SCENEVIEW_LIGHT;
        
	m_camera = new crCameraNode;
	//m_camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT/* | GL_STENCIL_BUFFER_BIT*/);
	m_camera->setViewport(new crViewPort);
	if(crDisplaySettings::instance()->getRunMode() == 0)
		m_camera->setClearColor(CRCore::crVector4(0.27f,0.37f,0.47f, 1.0f)/*CRCore::crVector4(0.2f, 0.2f, 0.4f, 1.0f)*/);
	else
		m_camera->setClearColor(CRCore::crVector4(0.0f, 0.0f, 0.0f, 1.0f)/*CRCore::crVector4(0.2f, 0.2f, 0.4f, 1.0f)*/);
	m_camera->setName("SceneRender");
//#define ZperPass
//#ifdef ZperPass
	if(crDisplaySettings::instance()->getUseZperPass())
		m_camera->setClearMask(GL_NONE/*GL_COLOR_BUFFER_BIT*/);
//#endif
    m_initCalled = false;

    m_drawBufferMode = GL_BACK;

    m_requiresFlush = true;

    m_activeUniforms = DEFAULT_UNIFORMS;
	//m_previousFrameTime = 0;

	m_redrawInterlacedStereoStencilMask = true;
	m_interlacedStereoStencilWidth = 0;
	m_interlacedStereoStencilHeight = 0;

    m_useRenderDoubleBuf = false;
	m_swapBufMutexLocked = false;
	//m_lastDrawTick = 0L;
	//m_svDisplaySettings = new crSceneView::SVDisplaySettings(this);
}


crSceneView::~crSceneView()
{
	unlockSwapBuffer();
}

void crSceneView::setCamera(CRCore::crCameraNode* camera)
{
	if (camera)
	{
		m_camera = camera;
		m_camera->setName("SceneRender");
	}
	else
	{
		CRCore::notify(CRCore::NOTICE)<<"Warning: attempt to assign a NULL camera to SceneView not permitted."<<std::endl;
	}
}

CRCore::crCameraNode* crSceneView::getCamera() 
{ 
	return m_camera.get(); 
}

const CRCore::crCameraNode* crSceneView::getCamera() const 
{ 
	return m_camera.get(); 
}

void crSceneView::setSceneData(CRCore::crNode* node)
{
	// take a temporary reference to node to prevent the possibility
	// of it getting deleted when when we do the camera clear of children. 
	CRCore::ref_ptr<CRCore::crNode> temporaryRefernce = node;

	// remove pre existing children
	m_camera->removeChild(0, m_camera->getNumChildren());

	// add the new one in.
	m_camera->addChild(node);
}

unsigned int crSceneView::getNumSceneData() const 
{ 
	return m_camera->getNumChildren(); 
}

void crSceneView::setViewport(CRCore::crViewPort* viewport) 
{ 
	m_camera->setViewport(viewport); 
}

void crSceneView::setViewport(int x,int y,int width,int height) 
{ 
	m_camera->setViewport(x,y,width,height); 
}

void crSceneView::getViewport(int& x,int& y,int& width,int& height) const
{
	if (m_camera->getViewport()!=0) 
		m_camera->getViewport()->getViewport(x,y,width,height);
	else
		x = y = width = height = 0;
}

void crSceneView::setDisplaySettings(CRCore::crDisplaySettings* vs)
{ 
	m_displaySettings = vs; 
}

const CRCore::crDisplaySettings* crSceneView::getDisplaySettings() const 
{ 
	return m_displaySettings.get(); 
}

CRCore::crDisplaySettings* crSceneView::getDisplaySettings()
{ 
	return m_displaySettings.get(); 
}

void crSceneView::setClearColor(const CRCore::crVector4& color)
{ 
	m_camera->setClearColor(color); 
}

const CRCore::crVector4& crSceneView::getClearColor() const 
{
	return m_camera->getClearColor(); 
}

void crSceneView::setRedrawInterlacedStereoStencilMask(bool flag) 
{ 
	m_redrawInterlacedStereoStencilMask = flag; 
}

bool crSceneView::getRedrawInterlacedStereoStencilMask() const 
{ 
	return m_redrawInterlacedStereoStencilMask; 
}

void crSceneView::setGlobalStateSet(CRCore::crStateSet* state) 
{ 
	m_globalStateSet = state; 
}

CRCore::crStateSet* crSceneView::getGlobalStateSet() 
{
	return m_globalStateSet.get(); 
}

const CRCore::crStateSet* crSceneView::getGlobalStateSet() const 
{ 
	return m_globalStateSet.get(); 
}

void crSceneView::setLocalStateSet(CRCore::crStateSet* state) 
{
	m_localStateSet = state; 
}

CRCore::crStateSet* crSceneView::getLocalStateSet()
{ 
	return m_localStateSet.get(); 
}

const CRCore::crStateSet* crSceneView::getLocalStateSet() const 
{ 
	return m_localStateSet.get(); 
}

void crSceneView::setActiveUniforms(int activeUniforms) 
{ 
	m_activeUniforms = activeUniforms; 
}

int crSceneView::getActiveUniforms() const 
{ 
	return m_activeUniforms; 
}

void crSceneView::setLightingMode(crSceneView::LightingMode mode) 
{ 
	m_lightingMode=mode; 
}

crSceneView::LightingMode crSceneView::getLightingMode() const 
{ 
	return m_lightingMode; 
}

void crSceneView::setLight(CRCore::crLight* light)
{ 
	m_light = light; 
}

CRCore::crLight* crSceneView::getLight() 
{ 
	return m_light.get(); 
}

const CRCore::crLight* crSceneView::getLight() const 
{ 
	return m_light.get(); 
}

void crSceneView::setState(CRCore::crState* state)
{
    m_state = state;
}

CRCore::crState* crSceneView::getState()
{
    return m_state.get();
}

const CRCore::crState* crSceneView::getState() const
{
    return m_state.get();
}

CRCore::crMatrixd* crSceneView::getBufProjectionMatrix() 
{ 
	return m_camera->getBufProjectionMatrix(); 
}

const CRCore::crMatrixd* crSceneView::getBufProjectionMatrix() const 
{
	return m_camera->getBufProjectionMatrix(); 
}

CRCore::crMatrixd* crSceneView::getBufViewMatrix() 
{ 
	return m_camera->getBufViewMatrix(); 
}

const CRCore::crMatrixd* crSceneView::getBufViewMatrix() const 
{ 
	return m_camera->getBufViewMatrix(); 
}

CRCore::crMatrixd* crSceneView::getViewInverseMatrix() 
{ 
	return m_camera->getViewInverseMatrix(); 
}

const CRCore::crMatrixd* crSceneView::getViewInverseMatrix() const 
{ 
	return m_camera->getViewInverseMatrix(); 
}

CRCore::crMatrixd* crSceneView::getBufViewInverseMatrix() 
{ 
	return m_camera->getBufViewInverseMatrix(); 
}

const CRCore::crMatrixd* crSceneView::getBufViewInverseMatrix() const
{ 
	return m_camera->getBufViewInverseMatrix(); 
}

void crSceneView::setDrawBufferMode( unsigned int drawBufferMode )
{
	m_drawBufferMode = drawBufferMode;
}

unsigned int crSceneView::getDrawBufferMode() const
{
	return m_drawBufferMode;
}

void crSceneView::setInitVisitor(CRCore::crNodeVisitor* av) 
{ 
	m_initVisitor = av; 
}

CRCore::crNodeVisitor* crSceneView::getInitVisitor() 
{ 
	return m_initVisitor.get(); 
}

const CRCore::crNodeVisitor* crSceneView::getInitVisitor() const 
{ 
	return m_initVisitor.get(); 
}

void crSceneView::setPhysicsInitVisitor(CRCore::crNodeVisitor* pv)
{
	m_physicsInitVisitor = pv; 
}

CRCore::crNodeVisitor* crSceneView::getPhysicsInitVisitor() 
{ 
	return m_physicsInitVisitor.get(); 
}

const CRCore::crNodeVisitor* crSceneView::getPhysicsInitVisitor() const 
{ 
	return m_physicsInitVisitor.get(); 
}

void crSceneView::setUpdateVisitor(CRCore::crNodeVisitor* av) 
{
	m_updateVisitor = av; 
}

CRCore::crNodeVisitor* crSceneView::getUpdateVisitor() 
{ 
	return m_updateVisitor.get(); 
}

const CRCore::crNodeVisitor* crSceneView::getUpdateVisitor() const 
{ 
	return m_updateVisitor.get(); 
}

void crSceneView::setCullVisitor(CRUtil::crCullVisitor* cv) { m_cullVisitor = cv; }
CRUtil::crCullVisitor* crSceneView::getCullVisitor() { return m_cullVisitor.get(); }
const CRUtil::crCullVisitor* crSceneView::getCullVisitor() const { return m_cullVisitor.get(); }

void crSceneView::setCullVisitorLeft(CRUtil::crCullVisitor* cv) { m_cullVisitorLeft = cv; }
CRUtil::crCullVisitor* crSceneView::getCullVisitorLeft() { return m_cullVisitorLeft.get(); }
const CRUtil::crCullVisitor* crSceneView::getCullVisitorLeft() const { return m_cullVisitorLeft.get(); }

void crSceneView::setCullVisitorRight(CRUtil::crCullVisitor* cv) { m_cullVisitorRight = cv; }
CRUtil::crCullVisitor* crSceneView::getCullVisitorRight() { return m_cullVisitorRight.get(); }
const CRUtil::crCullVisitor* crSceneView::getCullVisitorRight() const { return m_cullVisitorRight.get(); }

void crSceneView::setCollectOccludersVisitor(crCollectOccludersVisitor* cov) { m_collectOccludersVisistor = cov; }
crCollectOccludersVisitor* crSceneView::getCollectOccludersVisitor() { return m_collectOccludersVisistor.get(); }
const crCollectOccludersVisitor* crSceneView::getCollectOccludersVisitor() const { return m_collectOccludersVisistor.get(); }

void crSceneView::setRenderGraph(CRUtil::crRenderGraph* rg) { m_rendergraph = rg; }
CRUtil::crRenderGraph* crSceneView::getRenderGraph() { return m_rendergraph.get(); }
const CRUtil::crRenderGraph* crSceneView::getRenderGraph() const { return m_rendergraph.get(); }

void crSceneView::setRenderGraphLeft(CRUtil::crRenderGraph* rg) { m_rendergraphLeft = rg; }
CRUtil::crRenderGraph* crSceneView::getRenderGraphLeft() { return m_rendergraphLeft.get(); }
const CRUtil::crRenderGraph* crSceneView::getRenderGraphLeft() const { return m_rendergraphLeft.get(); }

void crSceneView::setRenderGraphRight(CRUtil::crRenderGraph* rg) { m_rendergraphRight = rg; }
CRUtil::crRenderGraph* crSceneView::getRenderGraphRight() { return m_rendergraphRight.get(); }
const CRUtil::crRenderGraph* crSceneView::getRenderGraphRight() const { return m_rendergraphRight.get(); }

void crSceneView::setRenderStage(CRUtil::crRenderStage* rs) { m_renderStage = rs; }
CRUtil::crRenderStage* crSceneView::getRenderStage() { return m_renderStage.get(); }
const CRUtil::crRenderStage* crSceneView::getRenderStage() const { return m_renderStage.get(); }

void crSceneView::setRenderStageLeft(CRUtil::crRenderStage* rs) { m_renderStageLeft = rs; }
CRUtil::crRenderStage* crSceneView::getRenderStageLeft() { return m_renderStageLeft.get(); }
const CRUtil::crRenderStage* crSceneView::getRenderStageLeft() const { return m_renderStageLeft.get(); }

void crSceneView::setRenderStageRight(CRUtil::crRenderStage* rs) { m_renderStageRight = rs; }
CRUtil::crRenderStage* crSceneView::getRenderStageRight() { return m_renderStageRight.get(); }
const CRUtil::crRenderStage* crSceneView::getRenderStageRight() const { return m_renderStageRight.get(); }

crSceneView::FusionDistanceMode crSceneView::getFusionDistanceMode() const { return m_fusionDistanceMode; }
float crSceneView::getFusionDistanceValue() const { return m_fusionDistanceValue; }

void crSceneView::setComputeStereoMatricesCallback(ComputeStereoMatricesCallback* callback) { m_computeStereoMatricesCallback=callback; }
crSceneView::ComputeStereoMatricesCallback* crSceneView::getComputeStereoMatricesCallback() { return m_computeStereoMatricesCallback.get(); }
const crSceneView::ComputeStereoMatricesCallback* crSceneView::getComputeStereoMatricesCallback() const { return m_computeStereoMatricesCallback.get(); }

void crSceneView::setUseRenderDoubleBuffer( bool useRenderDoubleBuf ){ m_useRenderDoubleBuf = useRenderDoubleBuf; }
bool crSceneView::getUseRenderDoubleBuffer()const{ return m_useRenderDoubleBuf; }

void crSceneView::init()
{
#if _DEBUG
	try
	{
#endif
		m_state->initializeExtensionProcs();
		flushAllDeletedObjectsImp(m_state->getContextID());
		if (m_camera.valid() && m_initVisitor.valid())
		{
			m_camera->setRenderInited(false);
			m_initVisitor->reset();
			//m_initVisitor->setFrameStamp(m_frameStamp.get());

			crRenderInitVisitor* dlv = dynamic_cast<crRenderInitVisitor*>(m_initVisitor.get());
			if (dlv) 
			{
				dlv->setState(m_state.get());
				//dlv->setMode(dlv->getMode()/*|crRenderInitVisitor::SWITCH_ON_VERTEX_BUFFER_OBJECTS|crRenderInitVisitor::SWITCH_OFF_DISPLAY_LISTS*/);//关闭VBO渲染
				dlv->setMode(/*crRenderInitVisitor::DEFAULT*/crRenderInitVisitor::NONE);
				dlv->initExtern();
			}
			//if (m_frameStamp.valid())
			//{
				m_initVisitor->setTraversalNumber(crFrameStamp::getInstance()->getFrameNumber());
			//}
			//glClearColor( m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
			//glClearDepth( 1.0f );
			//glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
			//glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
			//glDepthMask(GL_FALSE);
			//glDepthFunc(GL_NEVER);
			m_camera->accept(*m_initVisitor.get());
			if(dlv) dlv->setMode(crRenderInitVisitor::NONE);//程序运行过程中由crDatabasePager负责编译
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crSceneView::init() "<< std::endl;

		if(m_physicsInitVisitor.valid())
		{
			CRCore::crBrain::getInstance()->getCurrentActiveDB()->accept(*m_physicsInitVisitor);
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crSceneView::init()1 "<< std::endl;

		m_initCalled = true;
#if _DEBUG
	}
	catch (...)
	{
		CRCore::notify(CRCore::FATAL)<<"crSceneView::init() error"<<std::endl;
	}
#endif
}
bool crSceneView::doubleThreadRenderInit()
{
	m_useRenderDoubleBuf = true;

	//if(!m_state_buf.valid()) m_state_buf =  new crState;
	//m_state_buf->setContextID(m_state->getContextID());
	//m_state_buf->setFrameStamp(m_frameStamp.get());
	//m_state_buf = m_state;

	//if(!m_cullVisitor_buf.valid()) m_cullVisitor_buf = new crCullVisitor;
	m_rendergraph_buf = new crRenderGraph;
	//if(!m_renderStage_buf.valid()) m_renderStage_buf = new crOglRenderStage;
	m_renderStage_buf = new crRenderStage;

	//if(!m_projectionMatrix_buf.valid()) 
	//{
	//	m_projectionMatrix_buf = new CRCore::crMatrixd;
	//	m_projectionMatrix_buf->makePerspective(50.0f,1.4f,1.0f,10000.0f);
	//}

	//if(!m_viewMatrix_buf.valid()) 
	//{
	//	m_viewMatrix_buf = new CRCore::crMatrixd;
	//	m_viewMatrix_buf->makeIdentity();
	//}
	m_renderStage_buf->setClearMask(m_camera->getClearMask());
	return true;
}
//#define ResetRenderFunc(func) func::reset();
void crSceneView::flushAllDeletedObjects()
{
	m_requiresFlush = false;

	unsigned int contextID = m_state->getContextID();
	m_state->reset();
	//if(m_state_buf.valid()) m_state_buf->reset();

	if(m_cullVisitor.valid()) m_cullVisitor->setDatabaseRequestHandler(0);
	//if(m_cullVisitor_buf.valid()) m_cullVisitor_buf->setDatabaseRequestHandler(0);
	if(m_cullVisitorLeft.valid()) m_cullVisitorLeft->setDatabaseRequestHandler(0);
	//if(m_cullVisitorLeft_buf.valid()) m_cullVisitorLeft_buf->setDatabaseRequestHandler(0);
	if(m_cullVisitorRight.valid()) m_cullVisitorRight->setDatabaseRequestHandler(0);
	//if(m_cullVisitorRight_buf.valid()) m_cullVisitorRight_buf->setDatabaseRequestHandler(0);

	//crRenderGraph::clearReuseRenderGraph();

	//double availableTime = 500.0f;
	//double currentTime = m_state->getFrameStamp()?m_state->getFrameStamp()->getReferenceTime():0.0;
	s_externUpdaterVec.clear();
	m_camera->removeChild(0,m_camera->getNumChildren());

	flushAllDeletedObjectsImp(contextID);

	m_initCalled = false;
	m_drawBufferMode = GL_BACK;
	m_requiresFlush = true;
	m_activeUniforms = DEFAULT_UNIFORMS;
	m_redrawInterlacedStereoStencilMask = true;
	m_interlacedStereoStencilWidth = 0;
	m_interlacedStereoStencilHeight = 0;
	
	unlockSwapBuffer();

	m_physicsInitVisitor = NULL;
	m_physicsUpdateVisitor = NULL;
	
	//s_externUpdaterVec.clear();
	//crBrain::getInstance()->accept(*CRPhysics::crPhysicsReleaseVisitor::getInstance());
	//crBrain::getInstance()->clear();
	//crAttachRenderFuncCallback::getInstance()->clear();
	//m_camera->removeChild(0,m_camera->getNumChildren());
	//CRRender::clearExtensions();
	//ResetRenderFunc(crAlphaFunc);
	//ResetRenderFunc(crBlendColor);
	//ResetRenderFunc(crBlendEquation);
	//ResetRenderFunc(crBlendFunc);
	//ResetRenderFunc(crClampColor);
	//ResetRenderFunc(crDepth);
	//ResetRenderFunc(crFrontFace);
	//ResetRenderFunc(crLightModel);
	//ResetRenderFunc(crMaterial);
	//ResetRenderFunc(crMultisample);
	//ResetRenderFunc(crPolygonMode);
	//ResetRenderFunc(crPolygonOffset);
	//ResetRenderFunc(crShadeModel);
	//ResetRenderFunc(crStencil);
	//ResetRenderFunc(crTexEnv);
	//ResetRenderFunc(crTexEnvFilter);
	//ResetRenderFunc(crViewPort);
}
void crSceneView::unlockSwapBuffer()
{
	if(m_swapBufMutexLocked)
	{
		m_swapBufMutexLocked = false;
		m_swapBufMutex.unlock();
	}
}
void crSceneView::flushAllDeletedObjectsImp(unsigned int contextID)
{
	crFrameBufferObject::flushAllDeletedFrameBufferObjects(contextID);
	crRenderBuffer::flushAllDeletedRenderBuffers(contextID);
	crTexture::flushAllDeletedTextureObjects(contextID);
	crDrawable::flushAllDeletedDisplayLists(contextID);
	crDrawable::flushAllDeletedVertexBufferObjects(contextID);
	//crOglVertexProgramDraw::flushDeletedVertexProgramObjects(contextID,currentTime,availableTime);
	//crOglFragmentProgramDraw::flushDeletedFragmentProgramObjects(contextID,currentTime,availableTime);
	crProgram::flushAllDeletedGlPrograms(contextID);
	crShader::flushAllDeletedGlShaders(contextID);
	crBufferObject::flushAllDeletedBufferObjects(contextID);
}
void crSceneView::flushDeletedObjects(double& availableTime)
{
#if 0
	m_requiresFlush = false;

	double currentTime = crFrameStamp::getInstance()->getReferenceTime();
	unsigned int contextID = m_state->getContextID();

	crFrameBufferObject::flushDeletedFrameBufferObjects(contextID,currentTime,availableTime);
	crRenderBuffer::flushDeletedRenderBuffers(contextID,currentTime,availableTime);
	crTexture::flushDeletedTextureObjects(contextID,currentTime,availableTime);
	crDrawable::flushDeletedDisplayLists(contextID,availableTime);
	crDrawable::flushDeletedVertexBufferObjects(contextID,currentTime,availableTime);
	//crOglVertexProgramDraw::flushDeletedVertexProgramObjects(contextID,currentTime,availableTime);
	//crOglFragmentProgramDraw::flushDeletedFragmentProgramObjects(contextID,currentTime,availableTime);
	crProgram::flushDeletedGlPrograms(contextID,currentTime,availableTime);
	crShader::flushDeletedGlShaders(contextID,currentTime,availableTime);
	crBufferObject::flushDeletedBufferObjects(contextID,currentTime,availableTime);
#endif
}

void crSceneView::setAbortRenderingPtr(bool *Ptr)
{
	m_state->setAbortRenderingPtr(Ptr); 
	//if(m_state_buf.valid()) 
	//	m_state_buf->setAbortRenderingPtr(Ptr);
}

//void crSceneView::setFrameStamp(CRCore::crFrameStamp* fs)
//{ 
//	m_frameStamp = fs; 
//	m_state->setFrameStamp(fs); 
//	//if(m_state_buf.valid())
//	//{
//	//	m_state_buf->setFrameStamp(fs);
//	//	m_state_buf->setContextID(m_state->getContextID());
//	//	m_state_buf->setGraphicsContext(m_state->getGraphicsContext());
//	//}
//}

void crSceneView::update()
{
	//if (!m_initCalled) init();

	//CRCore::Timer_t start_tick = 0;
	//CRCore::Timer_t end_tick = 0;
	//if(crStatistics::getInstance()->getStat())
	//{
	//	start_tick = CRCore::Timer::instance()->tick();
	//}

	if (m_camera.valid() && m_updateVisitor.valid())
	{
    	m_updateVisitor->reset();
		m_camera->computerViewInverseMatrix();
		crMatrixd* viewInverseMatrix = m_camera->getViewInverseMatrix();
		CRUtil::crUpdateVisitor *updateVisitor = dynamic_cast<CRUtil::crUpdateVisitor *>(m_updateVisitor.get());
		crVector3 cameraPosition((*viewInverseMatrix)(3,0),(*viewInverseMatrix)(3,1),(*viewInverseMatrix)(3,2));
		updateVisitor->setEyePoint(cameraPosition);
		CRCore::crBrain::getInstance()->setCameraPosition(cameraPosition);
        updateVisitor->setViewMatrix(m_camera->getViewMatrix());
		updateVisitor->setProjectionMatrix(m_camera->getProjectionMatrix());

		//m_updateVisitor->setFrameStamp(m_frameStamp.get());

		// use the frame number for the traversal number.
		//if (m_frameStamp.valid())
		//{
			m_updateVisitor->setTraversalNumber(crFrameStamp::getInstance()->getFrameNumber());
		//}
		//CRCore::notify(CRCore::ALWAYS)<<m_frameStamp->getFrameNumber()<<"crSceneView::update(): m_position = "<<m_updateVisitor->getEyePoint()<<std::endl;

		//crBrain::getInstance()->updateKeyboardMouse(*m_updateVisitor);

		//if(s_sceneManagerUpdater.valid())
		//	(*s_sceneManagerUpdater)();

   //     if(m_netWorkUpdateCallback.valid())
			//(*m_netWorkUpdateCallback)();

		//if(m_aiUpdateVisitor.valid())
		//{
		//	//CRCore::notify(CRCore::ALWAYS)<<"m_aiUpdateVisitor 00"<<std::endl;
		//	m_aiUpdateVisitor->setFrameStamp(m_frameStamp.get());
		//	crBrain::getInstance()->getAITreeHead()->accept(*m_aiUpdateVisitor);
		//	//CRCore::notify(CRCore::ALWAYS)<<"m_aiUpdateVisitor 22"<<std::endl;
		//}
        
		//m_futureMatrix->m_matrix
		//m_matrix->cameraMatrix
		//ode->m_matrix_buf
		//m_matrix_buf->m_futureMatrix/m_matrix
		//CRParticle::crParticleSystemUpdater::getInstance()->updateBlock();
		//rbody::crCharacterSystemUpdater::getInstance()->updateBlock();
		float dt = crFrameStamp::getInstance()->getFrameInterval();
#if _DEBUG
		try
		{
#endif
		crBrain::getInstance()->update(dt);
		crBrain::getInstance()->doEvent(WCH_UPDATE,MAKEINT64(&dt,m_updateVisitor.get()));
#if _DEBUG
		}
		catch (...)
		{
			CRCore::notify(CRCore::ALWAYS)<<"crSceneView Brain Update error "<<std::endl;
		}
#endif
#if _DEBUG
		try
		{
#endif
		m_camera->accept(*m_updateVisitor);
#if _DEBUG
		}
		catch (...)
		{
			CRCore::notify(CRCore::ALWAYS)<<"crSceneView::m_updateVisitor error "<<std::endl;
		}
#endif
#if _DEBUG
		try
		{
#endif
		for( ExternUpdaterVec::iterator itr = s_externUpdaterVec.begin();
			itr != s_externUpdaterVec.end();
			++itr )
		{
			(*(itr->get()))(dt);
		}
#if _DEBUG
		}
		catch (...)
		{
			CRCore::notify(CRCore::ALWAYS)<<"crSceneView::ExternUpdater error "<<std::endl;
		}
#endif

		//m_futureMatrix->m_matrix;
		//do m_futureMatrix
		if(m_physicsUpdateVisitor.valid())
		{
			//m_camera->accept(*m_physicsUpdateVisitor);
			//m_physicsUpdateVisitor->setFrameStamp(m_frameStamp.get());
			crBrain::getInstance()->getCurrentActiveDB()->accept(*m_physicsUpdateVisitor);
			//update setMatterObjectMatrix->m_futureMatrix/m_matrix
			//m_futureMatrix/m_matrix->ode
			//m_futureMatrix/m_matrix->m_matrix_buf
		}

		//CRPhysics::crPhysicsSystemUpdater::getInstance()->frameBlock();//20140131 deleted
		//CRCore::notify(CRCore::ALWAYS)<<"crPhysicsSystemUpdater::getInstance()->frameBlock()"<<std::endl;

		//crAISystemUpdater::getInstance()->frameBlock();
		//CRCore::notify(CRCore::ALWAYS)<<"crAISystemUpdater::getInstance()->frameBlock()"<<std::endl;

		//if(m_aiUpdateVisitor.valid())
		//{
		//	//CRCore::notify(CRCore::ALWAYS)<<"m_aiUpdateVisitor 00"<<std::endl;
		//	m_aiUpdateVisitor->setFrameStamp(m_frameStamp.get());
		//	crBrain::getInstance()->getAITreeHead()->accept(*m_aiUpdateVisitor);
		//	//CRCore::notify(CRCore::ALWAYS)<<"m_aiUpdateVisitor 22"<<std::endl;
		//}

		//if(CRNet::crNetManager::getInstance()->getNetType()!=CRNet::crNetManager::None)
		//{
		//	CRNet::crNetManager::getInstance()->send();
		//	CRNet::crNetManager::getInstance()->get_response();
		//}
		// now force a recompute of the bounding volume while we are still in
		// the read/write app phase, this should prevent the need to recompute
		// the bounding volumes from within the cull traversal which may be
		// multi-threaded.
		m_camera->getBound();
	}
	//if(crStatistics::getInstance()->getStat())
	//{
	//	end_tick = CRCore::Timer::instance()->tick();
	//	float updtime = CRCore::Timer::instance()->delta_m(start_tick,end_tick);
	//	crStatistics::getInstance()->setUpdTime(updtime);
	//	//CRText::crText *updtimeText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("updTimeText"));
	//	//updtimeText->setText(crArgumentParser::appFtoa(updtime));
	//}
}

CRCore::crMatrixd crSceneView::computeLeftEyeProjectionImplementation(const CRCore::crMatrixd& projection) const
{
	double iod = m_displaySettings->getEyeSeparation();
	double sd = m_displaySettings->getScreenDistance();
	double scale_x = 1.0;
	double scale_y = 1.0;

	if (m_displaySettings->getSplitStereoAutoAjustAspectRatio())
	{
		switch(m_displaySettings->getStereoMode())
		{
		case(CRCore::crDisplaySettings::HORIZONTAL_SPLIT):
			scale_x = 2.0;
			break;
		case(CRCore::crDisplaySettings::VERTICAL_SPLIT):
			scale_y = 2.0;
			break;
		default:
			break;
		}
	}

	if (m_displaySettings->getDisplayType()==CRCore::crDisplaySettings::HEAD_MOUNTED_DISPLAY)
	{
		// head mounted display has the same projection matrix for left and right eyes.
		return CRCore::crMatrixd::scale(scale_x,scale_y,1.0) * projection;
	}
	else
	{
		// all other display types assume working like a projected power wall
		// need to shjear projection matrix to account for asymetric frustum due to eye offset.
		return CRCore::crMatrixd(1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			iod/(2.0*sd),0.0,1.0,0.0,
			0.0,0.0,0.0,1.0) *
			CRCore::crMatrixd::scale(scale_x,scale_y,1.0) *
			projection;
	}
}

CRCore::crMatrixd crSceneView::computeLeftEyeViewImplementation(const CRCore::crMatrixd& view) const
{
	double fusionDistance = m_displaySettings->getScreenDistance();
	switch(m_fusionDistanceMode)
	{
	case(USE_FUSION_DISTANCE_VALUE):
		fusionDistance = m_fusionDistanceValue;
		break;
	case(PROPORTIONAL_TO_SCREEN_DISTANCE):
		fusionDistance *= m_fusionDistanceValue;
		break;
	}

	double iod = m_displaySettings->getEyeSeparation();
	double sd = m_displaySettings->getScreenDistance();
	double es = 0.5f*iod*(fusionDistance/sd);

	return view *
		CRCore::crMatrixd(1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		es,0.0,0.0,1.0);
}

CRCore::crMatrixd crSceneView::computeRightEyeProjectionImplementation(const CRCore::crMatrixd& projection) const
{
	double iod = m_displaySettings->getEyeSeparation();
	double sd = m_displaySettings->getScreenDistance();
	double scale_x = 1.0;
	double scale_y = 1.0;

	if (m_displaySettings->getSplitStereoAutoAjustAspectRatio())
	{
		switch(m_displaySettings->getStereoMode())
		{
		case(CRCore::crDisplaySettings::HORIZONTAL_SPLIT):
			scale_x = 2.0;
			break;
		case(CRCore::crDisplaySettings::VERTICAL_SPLIT):
			scale_y = 2.0;
			break;
		default:
			break;
		}
	}

	if (m_displaySettings->getDisplayType()==CRCore::crDisplaySettings::HEAD_MOUNTED_DISPLAY)
	{
		// head mounted display has the same projection matrix for left and right eyes.
		return CRCore::crMatrixd::scale(scale_x,scale_y,1.0) * projection;
	}
	else
	{
		// all other display types assume working like a projected power wall
		// need to shjear projection matrix to account for asymetric frustum due to eye offset.
		return CRCore::crMatrixd(1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			-iod/(2.0*sd),0.0,1.0,0.0,
			0.0,0.0,0.0,1.0) *
			CRCore::crMatrixd::scale(scale_x,scale_y,1.0) *
			projection;
	}
}

CRCore::crMatrixd crSceneView::computeRightEyeViewImplementation(const CRCore::crMatrixd& view) const
{
	float fusionDistance = m_displaySettings->getScreenDistance();
	switch(m_fusionDistanceMode)
	{
	case(USE_FUSION_DISTANCE_VALUE):
		fusionDistance = m_fusionDistanceValue;
		break;
	case(PROPORTIONAL_TO_SCREEN_DISTANCE):
		fusionDistance *= m_fusionDistanceValue;
		break;
	}

	double iod = m_displaySettings->getEyeSeparation();
	double sd = m_displaySettings->getScreenDistance();
	double es = 0.5*iod*(fusionDistance/sd);

	return view *
		CRCore::crMatrixd(1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		-es,0.0,0.0,1.0);
}

void crSceneView::cull()
{//注：在立体渲染模式时逆矩阵信息会出现不正确，需要调整
	CRCore::Timer_t start_tick = 0;
	CRCore::Timer_t end_tick = 0;
	if(crStatistics::getInstance()->getStat())
	{
		start_tick = CRCore::Timer::instance()->tick();
	}

    //m_state->setFrameStamp(m_frameStamp.get());
    m_state->setDisplaySettings(m_displaySettings.get());

	crRenderLeaf *previous = NULL;
	crMatrixd* projectionMatrix = m_camera->getProjectionMatrix();
	crMatrixd* viewMatrix = m_camera->getViewMatrix();
	//CRCore::notify(ALWAYS)<<"crSceneView::cull:projectionMatrix = "<<*projectionMatrix<<std::endl;
	//CRCore::notify(ALWAYS)<<"crSceneView::cull:viewMatrix = "<<*viewMatrix<<std::endl;
	//m_camera->computerViewInverseMatrix();
	crMatrixd* viewInverseMatrix = m_camera->getViewInverseMatrix();
    //CRCore::notify(ALWAYS)<<"crSceneView::cull:viewInverseMatrix = "<<*viewInverseMatrix<<std::endl;
#if _DEBUG
	try
	{
#endif
	if (m_displaySettings.valid() && m_displaySettings->getStereo()) 
    {

        if (m_displaySettings->getStereoMode()==CRCore::crDisplaySettings::LEFT_EYE)
        {
            // set up the left eye.
            m_cullVisitor->setTraversalMask(m_cullMaskLeft);

            cullStage(computeLeftEyeProjection(*projectionMatrix),computeLeftEyeView(*viewMatrix),*viewInverseMatrix,m_cullVisitor.get(),m_rendergraph.get(),m_renderStage.get());

            if (m_cullVisitor->getComputeNearFarMode()!=CRUtil::crCullVisitor::DO_NOT_COMPUTE_NEAR_FAR)
            {
                crCullVisitor::value_type zNear = m_cullVisitor->getCalculatedNearPlane();
                crCullVisitor::value_type zFar = m_cullVisitor->getCalculatedFarPlane();
                m_cullVisitor->clampProjectionMatrix(*projectionMatrix,zNear,zFar);
            }
			//m_renderStage.get()->createRenderDataList(*m_state, previous );
        }
        else if (m_displaySettings->getStereoMode()==CRCore::crDisplaySettings::RIGHT_EYE)
        {
            // set up the right eye.
            m_cullVisitor->setTraversalMask(m_cullMaskRight);

            cullStage(computeRightEyeProjection(*projectionMatrix),computeRightEyeView(*viewMatrix),*viewInverseMatrix,m_cullVisitor.get(),m_rendergraph.get(),m_renderStage.get());

            if (m_cullVisitor->getComputeNearFarMode()!=CRUtil::crCullVisitor::DO_NOT_COMPUTE_NEAR_FAR)
            {
                crCullVisitor::value_type zNear = m_cullVisitor->getCalculatedNearPlane();
                crCullVisitor::value_type zFar = m_cullVisitor->getCalculatedFarPlane();
                m_cullVisitor->clampProjectionMatrix(*projectionMatrix,zNear,zFar);
            }
			//m_renderStage.get()->createRenderDataList(*m_state, previous );
        }
        else
        {
			if (!m_cullVisitorLeft.valid()) m_cullVisitorLeft = dynamic_cast<crCullVisitor*>(m_cullVisitor->cloneType());
			if (!m_rendergraphLeft.valid()) m_rendergraphLeft = dynamic_cast<crRenderGraph*>(m_rendergraph->cloneType());
			if (!m_renderStageLeft.valid()) m_renderStageLeft = dynamic_cast<crRenderStage*>(m_renderStage->clone(CRCore::crCopyOp::DEEP_COPY_ALL));
			
			if (!m_cullVisitorRight.valid()) m_cullVisitorRight = dynamic_cast<crCullVisitor*>(m_cullVisitor->cloneType());
			if (!m_rendergraphRight.valid()) m_rendergraphRight = dynamic_cast<crRenderGraph*>(m_rendergraph->cloneType());
			if (!m_renderStageRight.valid()) m_renderStageRight = dynamic_cast<crRenderStage*>(m_renderStage->clone(CRCore::crCopyOp::DEEP_COPY_ALL));

			if(m_useRenderDoubleBuf)
			{
				//if (!m_cullVisitorLeft_buf.valid()) m_cullVisitorLeft_buf = dynamic_cast<crCullVisitor*>(m_cullVisitor->cloneType());
				if (!m_rendergraphLeft_buf.valid()) m_rendergraphLeft_buf = dynamic_cast<crRenderGraph*>(m_rendergraph->cloneType());
				if (!m_renderStageLeft_buf.valid()) m_renderStageLeft_buf = dynamic_cast<crRenderStage*>(m_renderStage->clone(CRCore::crCopyOp::DEEP_COPY_ALL));

				//if (!m_cullVisitorRight_buf.valid()) m_cullVisitorRight_buf = dynamic_cast<crCullVisitor*>(m_cullVisitor->cloneType());		
				if (!m_rendergraphRight_buf.valid()) m_rendergraphRight_buf = dynamic_cast<crRenderGraph*>(m_rendergraph->cloneType());
				if (!m_renderStageRight_buf.valid()) m_renderStageRight_buf = dynamic_cast<crRenderStage*>(m_renderStage->clone(CRCore::crCopyOp::DEEP_COPY_ALL));
			}

			// set up the left eye.
            m_cullVisitorLeft->setDatabaseRequestHandler(m_cullVisitor->getDatabaseRequestHandler());
            m_cullVisitorLeft->setClampProjectionMatrixCallback(m_cullVisitor->getClampProjectionMatrixCallback());
            m_cullVisitorLeft->setTraversalMask(m_cullMaskLeft);
            // set up the right eye.
            m_cullVisitorRight->setDatabaseRequestHandler(m_cullVisitor->getDatabaseRequestHandler());
            m_cullVisitorRight->setClampProjectionMatrixCallback(m_cullVisitor->getClampProjectionMatrixCallback());
            m_cullVisitorRight->setTraversalMask(m_cullMaskRight);

			cullStage(computeLeftEyeProjection(*projectionMatrix),computeLeftEyeView(*viewMatrix),*viewInverseMatrix,m_cullVisitorLeft.get(),m_rendergraphLeft.get(),m_renderStageLeft.get());
			cullStage(computeRightEyeProjection(*projectionMatrix),computeRightEyeView(*viewMatrix),*viewInverseMatrix,m_cullVisitorRight.get(),m_rendergraphRight.get(),m_renderStageRight.get());

            if (m_cullVisitorLeft->getComputeNearFarMode()!=CRUtil::crCullVisitor::DO_NOT_COMPUTE_NEAR_FAR &&
                m_cullVisitorRight->getComputeNearFarMode()!=CRUtil::crCullVisitor::DO_NOT_COMPUTE_NEAR_FAR)
            {
                crCullVisitor::value_type zNear = CRCore::minimum(m_cullVisitorLeft->getCalculatedNearPlane(),m_cullVisitorRight->getCalculatedNearPlane());
                crCullVisitor::value_type zFar =  CRCore::maximum(m_cullVisitorLeft->getCalculatedFarPlane(),m_cullVisitorRight->getCalculatedFarPlane());
                m_cullVisitor->clampProjectionMatrix(*projectionMatrix,zNear,zFar);
            }

			//m_renderStageLeft.get()->createRenderDataList(*m_state, previous );
			//m_renderStageRight.get()->createRenderDataList(*m_state, previous );
        }
    }
    else
    {
        m_cullVisitor->setTraversalMask(m_cullMask);
        cullStage(*projectionMatrix,*viewMatrix,*viewInverseMatrix,m_cullVisitor.get(),m_rendergraph.get(),m_renderStage.get());
        if (m_cullVisitor->getComputeNearFarMode()!=CRUtil::crCullVisitor::DO_NOT_COMPUTE_NEAR_FAR)
        {
            crCullVisitor::value_type zNear = m_cullVisitor->getCalculatedNearPlane();
            crCullVisitor::value_type zFar = m_cullVisitor->getCalculatedFarPlane();
			m_cullVisitor->clampProjectionMatrix(*projectionMatrix,zNear,zFar);
        }

        //创建RenderDataList
		//m_renderStage.get()->createRenderDataList(*m_state, previous );
    }
	//updateUniforms();
#if _DEBUG
	}
	catch (...)
	{
		CRCore::notify(CRCore::ALWAYS)<<"crSceneView::cull error "<<std::endl;
	}
#endif
	if(crStatistics::getInstance()->getStat())
	{
		end_tick = CRCore::Timer::instance()->tick();
		float culltime = CRCore::Timer::instance()->delta_m(start_tick,end_tick);
		crStatistics::getInstance()->setCullTime(culltime);
		//CRText::crText *culltimeText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("cullTimeText"));
		//culltimeText->setText(crArgumentParser::appFtoa(culltime));
	}
	if(m_useRenderDoubleBuf) swapBuffers();

	//CRPhysics::crPhysicsSystemUpdater::getInstance()->frameBlock();//20140131 add
	//if(crDisplaySettings::instance()->getRunMode() < crDisplaySettings::Robot)
	//{
	//	CRParticle::crParticleSystemUpdater::getInstance()->frameBlock();
	//	rbody::crCharacterSystemUpdater::getInstance()->frameBlock();
	//}
	//////crAISystemUpdater::getInstance()->setFrameStamp(m_frameStamp.get());
	//////crAISystemUpdater::getInstance()->frameBlock();
}

void crSceneView::cullStage(const CRCore::crMatrixd& projection,const CRCore::crMatrixd& modelview,const CRCore::crMatrixd& inverseview,CRUtil::crCullVisitor* cullVisitor, CRUtil::crRenderGraph* rendergraph, CRUtil::crRenderStage* renderStage)
{
	if (!m_camera || !getViewport()) return;

    //if (!m_initCalled) init();

    CRCore::ref_ptr<RefMatrix> proj = new CRCore::RefMatrix(projection);
    CRCore::ref_ptr<RefMatrix> mv = new CRCore::RefMatrix(modelview);
	CRCore::ref_ptr<RefMatrix> mvinverse = new CRCore::RefMatrix(inverseview);

    // collect any occluder in the view frustum.        
	if (m_camera->containsOccluderNodes())
	{
		//CRCore::notify(CRCore::WARN)<<"Warning: containsOccluderNodes "<<m_camera->getNumChildrenWithOccluderNodes()<<std::endl;
        if (!m_collectOccludersVisistor) m_collectOccludersVisistor = new crCollectOccludersVisitor;
		
		m_collectOccludersVisistor->inheritCullSettings(*this);

		//m_collectOccludersVisistor->setCullSettings(*this);
        
		m_collectOccludersVisistor->reset();
        
        //m_collectOccludersVisistor->setFrameStamp(m_frameStamp.get());

        // use the frame number for the traversal number.
        //if (m_frameStamp.valid())
		//{
            m_collectOccludersVisistor->setTraversalNumber(crFrameStamp::getInstance()->getFrameNumber());
		//}

	    m_collectOccludersVisistor->pushViewport(getViewport());
        m_collectOccludersVisistor->pushProjectionMatrix(proj.get());
        m_collectOccludersVisistor->pushModelViewMatrix(mv.get());

        // traverse the scene graph to search for occluder in there new positions.
		for(unsigned int i=0; i< m_camera->getNumChildren(); ++i)
		{
			m_camera->getChild(i)->accept(*m_collectOccludersVisistor);
		}
        //m_sceneData->accept(*m_collectOccludersVisistor);

        m_collectOccludersVisistor->popModelViewMatrix();
        m_collectOccludersVisistor->popProjectionMatrix();
        m_collectOccludersVisistor->popViewport();
       
        // sort the occluder from largest occluder volume to smallest.
        m_collectOccludersVisistor->removeOccludedOccluders();
       
        //CRCore::notify(CRCore::INFO) << "finished searching for occluder - found "<<m_collectOccludersVisistor->getCollectedOccluderSet().size()<<std::endl;
          
        cullVisitor->getOccluderList().clear();
        std::copy(m_collectOccludersVisistor->getCollectedOccluderSet().begin(),m_collectOccludersVisistor->getCollectedOccluderSet().end(), std::back_insert_iterator<crCullStack::OccluderList>(cullVisitor->getOccluderList()));
	}

	cullVisitor->reset();
    crShaderManager::getInstance()->resetCulling();
    //cullVisitor->setFrameStamp(m_frameStamp.get());

    // use the frame number for the traversal number.
    //if (m_frameStamp.valid())
    //{
         cullVisitor->setTraversalNumber(crFrameStamp::getInstance()->getFrameNumber());	
    //}

	cullVisitor->inheritCullSettings(*this);
    //cullVisitor->setCullSettings(*this);
    cullVisitor->setClearNode(NULL); // reset earth sky on each frame.

	cullVisitor->setRenderGraph(rendergraph);
    cullVisitor->setRenderStage(renderStage);

	//if(m_useRenderDoubleBuf)
	//{
	//	cullVisitor->setState( m_state_buf.get() );
	//}
	//else
	//{
        cullVisitor->setState( m_state.get() );
	//}


    renderStage->reset();

    // comment out reset of rendergraph since clean is more efficient.
    rendergraph->reset();
	//crRenderGraph::resetReuseRenderGraph();

    // use clean of the rendergraph rather than reset, as it is able to
    // reuse the structure on the rendergraph in the next frame. This
    // achieves a certain amount of frame cohereancy of memory allocation.
    //rendergraph->clean();


    renderStage->setViewport(getViewport());
    renderStage->setClearColor(getClearColor());

    //switch(m_lightingMode)
    //{
    //case(HEADLIGHT):
    //    if (m_light.valid()) renderStage->addPositionedAttribute(NULL,m_light.get());
    //    //else CRCore::notify(CRCore::WARN)<<"Warning: no CRCore::Light attached to ogUtil::crSceneView to provide head light.*/"<<std::endl;
    //    break;
    //case(SKY_LIGHT):
    //    if (m_light.valid()) renderStage->addPositionedAttribute(mv.get(),m_light.get());
    //    //else CRCore::notify(CRCore::WARN)<<"Warning: no CRCore::Light attached to ogUtil::crSceneView to provide sky light.*/"<<std::endl;
    //    break;
    //case(NO_SCENEVIEW_LIGHT):
    //    break;
    //}  

	//int globalPopAlphaShadowFlg;
	//int localPopAlphaShadowFlg;
	bool global_pushed = false;
	bool local_pushed = false;

    if (m_globalStateSet.valid()) 
	{
		global_pushed = cullVisitor->pushStateSet(m_globalStateSet.get()/*,globalPopAlphaShadowFlg*/);
	}
    if (m_localStateSet.valid()) 
	{
		local_pushed = cullVisitor->pushStateSet(m_localStateSet.get()/*,localPopAlphaShadowFlg*/);
	}


    cullVisitor->pushViewport(getViewport());
    cullVisitor->pushProjectionMatrix(proj.get());
    cullVisitor->pushModelViewMatrix(mv.get());
    cullVisitor->pushCameraViewInverseMatrix(mvinverse.get());
	CRCore::crBrain::getInstance()->setCameraFrustum(cullVisitor->getCurrentCullingSet().getFrustum());
    // traverse the scene graph to generate the rendergraph.

#if _DEBUG
	try
	{
#endif
	//cullVisitor->setTraverseString("NoLight");//默认渲染方式
	unsigned int count = m_camera->getNumChildren();
	for(unsigned int childNo=0;
		childNo<count;
		++childNo)
	{
		m_camera->getChild(childNo)->accept(*cullVisitor);
	}
#if _DEBUG
	}
	catch (...)
	{
		CRCore::notify(CRCore::ALWAYS)<<"crSceneView::cullvisitor error "<<std::endl;
	}
#endif

	cullVisitor->popCameraViewInverseMatrix();
    cullVisitor->popModelViewMatrix();
    cullVisitor->popProjectionMatrix();
    cullVisitor->popViewport();

    if (local_pushed) 
	{
		cullVisitor->popStateSet(/*globalPopAlphaShadowFlg*/);
	}
    if (global_pushed) 
	{
		cullVisitor->popStateSet(/*localPopAlphaShadowFlg*/);
	}
    
    const CRCore::crClearNode* clearNode = cullVisitor->getClearNode();
    if (clearNode)
    {
        if (clearNode->getRequiresClear())
        {
            renderStage->setClearColor(clearNode->getClearColor());
            renderStage->setClearMask(clearNode->getClearMask());
         }
        else
        {
            // we have an earth sky implementation to do the work for use
            // so we don't need to clear.
            renderStage->setClearMask(0);
        }
    }

	renderStage->sort();
    rendergraph->prune();

    // prune out any empty crRenderGraph children.
    // note, this would be not required if the rendergraph had been
    // reset at the start of each frame (see top of this method) but
    // a clean has been used instead to try to minimize the amount of
    // allocation and deleteing of the crRenderGraph nodes.
	//renderStage->prune();

	//if(m_enableHint) renderStage->setSortMode(crRenderBin::SORT_BACK_TO_FRONT);
}

void crSceneView::releaseAllObjects()
{
	//try{
    if (!m_camera) return;

	//if(m_rendergraph.valid())
	//{
	//	m_rendergraph->reset();
	//}
	//if(m_rendergraph_buf.valid())
	//{
	//	m_rendergraph_buf->reset();
	//}
	//if(m_rendergraphLeft.valid())
	//{
	//	m_rendergraphLeft->reset();
	//}
	//if(m_rendergraphLeft_buf.valid())
	//{
	//	m_rendergraphLeft_buf->reset();
	//}
	//if(m_rendergraphRight.valid())
	//{
	//	m_rendergraphRight->reset();
	//}
	//if(m_rendergraphRight_buf.valid())
	//{
	//	m_rendergraphRight_buf->reset();
	//}
	if(m_renderStage.valid())
	{
		m_renderStage->releaseObjects(m_state.get());
		//m_renderStage->reset();
	}
	if(m_renderStage_buf.valid()) 
	{
		m_renderStage_buf->releaseObjects(m_state.get());
		//m_renderStage_buf->reset();
	}
	if(m_renderStageLeft.valid())
	{
		m_renderStageLeft->releaseObjects(m_state.get());
		//m_renderStageLeft->reset();
	}
	if(m_renderStageLeft_buf.valid())
	{
		m_renderStageLeft_buf->releaseObjects(m_state.get());
		//m_renderStageLeft_buf->reset();
	}
	if(m_renderStageRight.valid())
	{
		m_renderStageRight->releaseObjects(m_state.get());
		//m_renderStageRight->reset();
	}
	if(m_renderStageRight_buf.valid())
	{
		m_renderStageRight_buf->releaseObjects(m_state.get());
		//m_renderStageRight_buf->reset();
	}
	if(m_collectOccludersVisistor.valid())
		m_collectOccludersVisistor->reset();
	//crRenderGraph::resetReuseRenderGraph();
	//crRenderGraph::clearReuseRenderGraph();
	crShaderManager::getInstance()->resetCulling();

	crShaderManager::getInstance()->releaseObjects(m_state.get());
	crEnvironmentManager::getInstance()->releaseObjects(m_state.get());
    m_camera->releaseObjects(m_state.get());
	m_globalStateSet->releaseObjects(m_state.get());
	m_localStateSet->releaseObjects(m_state.get());
	crBrain::getInstance()->releaseObjects(m_state.get());
	//}
	//catch (...) {
	//	CRCore::notify(CRCore::FATAL)<<"crSceneView::releaseAllObjects: "<<std::endl;
	//}
 //   m_swapBufMutex.unlock();
	//swapBuffers();
	//m_camera->releaseObjects(m_state.get());
}

bool crSceneView::projectWindowIntoObject(const CRCore::crVector3& window,CRCore::crVector3& object) const
{
    CRCore::crMatrix inverseMVPW;
    inverseMVPW.invert(computeMVPW());
    
    object = window*inverseMVPW;
    
    return true;
}


/** Calculate, via glUnProject, the object coordinates of a window x,y
    when projected onto the near and far planes.
    Note, current implementation requires that crSceneView::draw() has been previously called
    for projectWindowIntoObject to produce valid values.  As per OpenGL
    windows coordinates are calculated relative to the bottom left of the window.*/
bool crSceneView::projectWindowXYIntoObject(int x,int y,CRCore::crVector3& near_point,CRCore::crVector3& far_point) const
{
    CRCore::crMatrix inverseMVPW;
    inverseMVPW.invert(computeMVPW());
    
    near_point = CRCore::crVector3(x,y,0.0f)*inverseMVPW;
    far_point = CRCore::crVector3(x,y,1.0f)*inverseMVPW;
        
    return true;
}


/** Calculate, via glProject, the object coordinates of a window.
    Note, current implementation requires that crSceneView::draw() has been previously called
    for projectWindowIntoObject to produce valid values.  As per OpenGL
    windows coordinates are calculated relative to the bottom left of the window.*/
bool crSceneView::projectObjectIntoWindow(const CRCore::crVector3& object,CRCore::crVector3& window) const
{
    window = object*computeMVPW();
    return true;
}

const CRCore::crMatrix crSceneView::computeMVPW() const
{
    CRCore::crMatrix matrix( (*getViewMatrix()) * (*getProjectionMatrix()));
        
    if (getViewport())
        matrix.postMult(getViewport()->computeWindowMatrix());
    else
        CRCore::notify(CRCore::WARN)<<"CRCore::crMatrix crSceneView::computeMVPW() - error no viewport attached to crSceneView, coords will be computed inccorectly."<<std::endl;

    return matrix;
}

void crSceneView::setProjectionMatrixAsOrtho(double left, double right,
                                           double bottom, double top,
                                           double zNear, double zFar)
{
    setProjectionMatrix(CRCore::crMatrixd::ortho(left, right,
                                           bottom, top,
                                           zNear, zFar));
}                                           

void crSceneView::setProjectionMatrixAsOrtho2D(double left, double right,
                                             double bottom, double top)
{
    setProjectionMatrix(CRCore::crMatrixd::ortho2D(left, right,
                                             bottom, top));
}

void crSceneView::setProjectionMatrixAsFrustum(double left, double right,
                                             double bottom, double top,
                                             double zNear, double zFar)
{
    setProjectionMatrix(CRCore::crMatrixd::frustum(left, right,
                                             bottom, top,
                                             zNear, zFar));
}

void crSceneView::setProjectionMatrixAsPerspective(double fovy,double aspectRatio,
                                                 double zNear, double zFar)
{
    setProjectionMatrix(CRCore::crMatrixd::perspective(fovy,aspectRatio,
                                                 zNear, zFar));
}                                      

bool crSceneView::getProjectionMatrixAsOrtho(double& left, double& right,
                                           double& bottom, double& top,
                                           double& zNear, double& zFar)
{
    return getProjectionMatrix()->getOrtho(left, right,
                                       bottom, top,
                                       zNear, zFar);
}

bool crSceneView::getProjectionMatrixAsFrustum(double& left, double& right,
                                             double& bottom, double& top,
                                             double& zNear, double& zFar)
{
    return getProjectionMatrix()->getFrustum(left, right,
                                         bottom, top,
                                         zNear, zFar);
}                                  

bool crSceneView::getProjectionMatrixAsPerspective(double& fovy,double& aspectRatio,
                                                 double& zNear, double& zFar)
{
    return getProjectionMatrix()->getPerspective(fovy, aspectRatio, zNear, zFar);
}                                                 

void crSceneView::setViewMatrixAsLookAt(const crVector3d& eye,const crVector3d& center,const crVector3d& up)
{
    setViewMatrix(CRCore::crMatrixd::lookAt(eye,center,up));
}

void crSceneView::getViewMatrixAsLookAt(crVector3d& eye,crVector3d& center,crVector3d& up,float lookDistance)
{
    getViewMatrix()->getLookAt(eye,center,up,lookDistance);
}

class SwapBufferVisitor : public CRCore::crNodeVisitor
{
public:

	SwapBufferVisitor():
	  m_frameNumber(0),
	  CRCore::crNodeVisitor(SWAPBUFFER_VISITOR,/*TRAVERSE_ALL_CHILDREN*/TRAVERSE_ACTIVE_CHILDREN)
	{
		//setTraversalMask(~CulledMask);
	}

	  virtual ~SwapBufferVisitor(){}
	  inline void setFrameNumber(int frameNumber){ m_frameNumber = frameNumber; }
	  virtual void apply(CRCore::crNode& node)
	  {
		  if(node.getVisiable2() && node.getSwapFrameNumber() != m_frameNumber)
		  {
			  node.swapBuffers(m_frameNumber);
			  traverse(node);
		  }
	  }
protected:
	int m_frameNumber;
};
static SwapBufferVisitor s_swapBufferVisitor;

void crSceneView::swapBuffers()
{
	//float dt = m_frameStamp->getFrameInterval();
	//if(dt<0.01f)
	//	CRCore::crThread::sleep((0.01f - dt));
	//CRCore::crThread::sleep(1);//释放CPU，降低CPU的占用率

//注：crSceneHandler对当前视锥矩阵进行更新操作-－>cullVisitor使用当前被更新的矩阵进行剪裁，使用当前的
//crRenderGraph、crRenderStage和crState-->swapBuffers()-->drawVisitor对buf进行绘制
	//CRCore::Timer_t time1 = CRCore::Timer::instance()->tick();
	m_swapBufMutex.lock();
	m_swapBufMutexLocked = true;
	//CRCore::Timer_t time2 = CRCore::Timer::instance()->tick();
	//CRCore::notify(CRCore::ALWAYS)<<"Upd_Cull Wait Time = "<<CRCore::Timer::instance()->delta_s(time1,time2)<<std::endl;
	//const GL2Extensions* extensions = GL2Extensions::Get(m_state->getContextID(),false);
	//if(extensions && extensions->isGlslSupported())
	CRCore::Timer_t start_tick,end_tick;
	if(crStatistics::getInstance()->getStat())
	{
		start_tick = CRCore::Timer::instance()->tick();
	}
	
	updateUniforms();
	//ref_ptr<crCullVisitor> cullVisitor;
	ref_ptr<crRenderGraph> rg;
    ref_ptr<crRenderStage> rs;
	//ref_ptr<crState> state;
	ref_ptr<crMatrixd> projectionMatrix;
	ref_ptr<crMatrixd> viewMatrix;


	//cullVisitor = m_cullVisitor;
	//m_cullVisitor = m_cullVisitor_buf;
	//m_cullVisitor_buf = cullVisitor;

	rg = m_rendergraph;
    m_rendergraph = m_rendergraph_buf;
    m_rendergraph_buf = rg;

	rs = m_renderStage;
	m_renderStage = m_renderStage_buf;
	m_renderStage_buf = rs;

	//state = m_state;
	//m_state = m_state_buf;
 //   m_state_buf = state;

	int frameNumber = crFrameStamp::getInstance()->getFrameNumber();
    if (m_state->getAbortRendering())
		crFrameStamp::getInstance()->setFrameNumber(++frameNumber);

	s_swapBufferVisitor.setFrameNumber(frameNumber);
	//if (m_frameStamp.valid())
	//{
	//	s_swapBufferVisitor.setTraversalNumber(m_frameStamp->getFrameNumber());
	//}
    m_camera->accept(s_swapBufferVisitor);

	//crRenderToTextureManager::getInstance()->swapBuffers();
	//crFilterRenderManager::getInstance()->swapBuffers();
	crShaderManager::getInstance()->swapBuffers(frameNumber);

	//crShaderManager::getInstance()->resetCulling();
	m_globalStateSet->swapBuffers(frameNumber);
	m_localStateSet->swapBuffers(frameNumber);

////left_right bufs

	//////left
	//cullVisitor = m_cullVisitorLeft;
	//m_cullVisitorLeft = m_cullVisitorLeft_buf;
	//m_cullVisitorLeft_buf = cullVisitor;

	rg = m_rendergraphLeft;
	m_rendergraphLeft = m_rendergraphLeft_buf;
	m_rendergraphLeft_buf = rg;

	rs = m_renderStageLeft;
	m_renderStageLeft = m_renderStageLeft_buf;
	m_renderStageLeft_buf = rs;

	//////right
	//cullVisitor = m_cullVisitorRight;
	//m_cullVisitorRight = m_cullVisitorRight_buf;
	//m_cullVisitorRight_buf = cullVisitor;

	rg = m_rendergraphRight;
	m_rendergraphRight = m_rendergraphRight_buf;
	m_rendergraphRight_buf = rg;

	rs = m_renderStageRight;
	m_renderStageRight = m_renderStageRight_buf;
	m_renderStageRight_buf = rs;

	if(crStatistics::getInstance()->getStat())
	{
		end_tick = CRCore::Timer::instance()->tick();
		float ms = CRCore::Timer::instance()->delta_m(start_tick,end_tick);
		crStatistics::getInstance()->setSwapTime(ms);
	}
}

void crSceneView::setContextID( int id )
{
    if(m_state.valid())
		m_state->setContextID(id);
	//if(m_state_buf.valid())
	//    m_state_buf->setContextID(id);
}

void crSceneView::updateUniforms()
{//从m_localStateSet 改为 m_globalStateSet

	//CRCore::notify(CRCore::FATAL)<<"crSceneView::updateUniforms()"<<std::endl;
//try
//{

	if (!m_globalStateSet)
	{
		m_globalStateSet = new CRCore::crStateSet;
	}

	if (!m_globalStateSet) return;

	CRCore::crUniform* uniform;

	if ((m_activeUniforms & FRAME_NUMBER_UNIFORM))
	{
		uniform = m_globalStateSet->getOrCreateUniform("CRE_FrameNumber",CRCore::crUniform::INT);
		uniform->setDataVariance(crBase::STATIC);
		uniform->set(crFrameStamp::getInstance()->getFrameNumber());        
	}

	if ((m_activeUniforms & FRAME_TIME_UNIFORM))
	{
		uniform = m_globalStateSet->getOrCreateUniform("CRE_FrameTime",CRCore::crUniform::FLOAT);
		uniform->setDataVariance(crBase::STATIC);
		uniform->set(static_cast<float>(crFrameStamp::getInstance()->getReferenceTime()));
	}

	if ((m_activeUniforms & DELTA_FRAME_TIME_UNIFORM))
	{
		//float delta_frame_time = (m_previousFrameTime != 0.0) ? /*static_cast<float>*/(m_frameStamp->getReferenceTime()-m_previousFrameTime) : 0.0f;
		//m_previousFrameTime = m_frameStamp->getReferenceTime();

		uniform = m_globalStateSet->getOrCreateUniform("CRE_DeltaFrameTime",CRCore::crUniform::FLOAT);
		uniform->setDataVariance(crBase::STATIC);
		uniform->set(crFrameStamp::getInstance()->getFrameInterval());
	}

	if (m_activeUniforms & VIEW_MATRIX_UNIFORM)
	{
		uniform = m_globalStateSet->getOrCreateUniform("CRE_ViewMatrix",CRCore::crUniform::FLOAT_MAT4);
		uniform->setDataVariance(crBase::STATIC);
		uniform->set(*m_camera->getViewMatrix());      
	}

	CRCore::crMatrixd* viewInverseMatrix = m_camera->getViewInverseMatrix();

	if (m_activeUniforms & INVERSE_VIEW_MATRIX_UNIFORM)
	{
		uniform = m_globalStateSet->getOrCreateUniform("CRE_InverseViewMatrix",CRCore::crUniform::FLOAT_MAT4);
		uniform->setDataVariance(crBase::STATIC);
		uniform->set(*viewInverseMatrix);      
	}

	if (m_activeUniforms & CAMERAPOS_UNIFORM)
	{
		uniform = m_globalStateSet->getOrCreateUniform("CRE_CameraPos",CRCore::crUniform::FLOAT_VEC3);
		uniform->setDataVariance(crBase::STATIC);
		uniform->set(CRCore::crVector3((*viewInverseMatrix)(3,0),(*viewInverseMatrix)(3,1),(*viewInverseMatrix)(3,2)));
		//CRCore::notify(ALWAYS)<<"crSceneView::cull:CAMERAPOS_UNIFORM = "<<CRCore::crVector3((*viewInverseMatrix)(3,0),(*viewInverseMatrix)(3,1),(*viewInverseMatrix)(3,2))<<std::endl;
	}

	//crUpdateUniformVisitor *visitor = dynamic_cast<crUpdateUniformVisitor *>(m_updateUniformVisitor.get());
	//visitor->setSceneViewPort(m_camera->getViewport());
	//m_camera->accept(*visitor);

//}
//catch (...)
//{
//	CRCore::notify(CRCore::FATAL)<<"crSceneView::updateUniforms() error"<<std::endl;
//}
}

#include <Driver/GLDrv/crSceneViewDrv.h>