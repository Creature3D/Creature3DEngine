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
#include <CRCore/crTransform.h>
#include <CRCore/crProjection.h>
#include <CRCore/crObject.h>
#include <CRCore/crLod.h>
#include <CRCore/crBillboard.h>
#include <CRCore/crLightSource.h>
#include <CRCore/crClipNode.h>
#include <CRCore/crTexGenNode.h>
#include <CRCore/crOccluderNode.h>
#include <CRCore/crShapeDrawable.h>
#include <CRCore/crNotify.h>
#include <CRCore/crLineSegment.h>
#include <CRCore/crIntersectVisitor.h>
#include <CRPhysics/crMatterObject.h>
#include <CRCore/crTriangleFunctor.h>
#include <CRUtil/crRenderStage.h>

#include <CRCore/crBlendFunc.h>
#include <CRCore/crLightSourceManager.h>
#include <CRCore/crPolygonOffset.h>
#include <CRCore/crStencil.h>
#include <CRCore/crLineWidth.h>
#include <CRCore/crPolygonMode.h>
#include <CRCore/crCullFace.h>

#include <CRUtil/crCullVisitor.h>

#include <float.h>
#include <algorithm>

#include <CRCore/crTimer.h>

using namespace CRCore;
using namespace CRUtil;

inline float MAX_F(float a, float b)
    { return a>b?a:b; }
inline int EQUAL_F(float a, float b)
    { return a == b || fabsf(a-b) <= MAX_F(fabsf(a),fabsf(b))*1e-3f; }


//class crPrintVisitor : public crNodeVisitor
//{
//
//   public:
//   
//        crPrintVisitor():crNodeVisitor(crNodeVisitor::TRAVERSE_ALL_CHILDREN)
//        {
//            m_indent = 0;
//            m_step = 4;
//        }
//        
//        inline void moveIn() { m_indent += m_step; }
//        inline void moveOut() { m_indent -= m_step; }
//        inline void writeIndent() 
//        {
//            for(int i=0;i<m_indent;++i) std::cout << " ";
//        }
//                
//        virtual void apply(crNode& node)
//        {
//            moveIn();
//            writeIndent(); std::cout << node.className() <<std::endl;
//            traverse(node);
//            moveOut();
//        }
//
//        virtual void apply(crObject& node)        { apply((crNode&)node); }
//        virtual void apply(crBillboard& node)     { apply((crObject&)node); }
//        virtual void apply(crLightSource& node)   { apply((crGroup&)node); }
//        virtual void apply(crClipNode& node)      { apply((crGroup&)node); }
//        //
//        virtual void apply(crGroup& node)         { apply((crNode&)node); }
//        virtual void apply(crTransform& node)     { apply((crGroup&)node); }
//        virtual void apply(crProjection& node)    { apply((crGroup&)node); }
//        virtual void apply(crSwitch& node)        { apply((crGroup&)node); }
//        virtual void apply(crLod& node)           { apply((crGroup&)node); }
//        //virtual void apply(crImpostor& node)      { apply((crLOD&)node); }
//
//   protected:
//    
//        int m_indent;
//        int m_step;
//};
//crCullVisitor::RenderLeafList crCullVisitor::m_reuseRenderLeafList;
//unsigned int crCullVisitor::m_currentReuseRenderLeafIndex = 0;
crCullVisitor::crCullVisitor():
    crNodeVisitor(CULL_VISITOR,TRAVERSE_ACTIVE_CHILDREN),
    m_currentRenderGraph(NULL),
    m_currentRenderBin(NULL),
    m_computed_znear(FLT_MAX),
    m_computed_zfar(-FLT_MAX)//,
//    m_impostorActive(true),
//    m_depthSortImpostorSprites(false),
//    m_impostorPixelErrorThreshold(4.0f),
//    m_numFramesToKeepImpostorSprites(10),
    //m_currentReuseRenderLeafIndex(0)
	//m_showBoundingBox(false)
	//m_enableSpecialCulling(false)
	//m_notPushStateSet(false)
{
//	m_impostorSpriteManager = new ImpostorSpriteManager;

	//m_computeAlphaShadowStack.push(false);
	//m_reuseRenderLeafList.reserve(50);

	m_forMultiRenderStateSet = new CRCore::crStateSet;
	CRCore::crBlendFunc* blendfunc = new CRCore::crBlendFunc(CRCore::crBlendFunc::ONE,CRCore::crBlendFunc::ONE);
	m_forMultiRenderStateSet->setAttributeAndModes(blendfunc,CRCore::crStateAttribute::ON);
	//CRCore::crPolygonOffset *polygonOffset = new CRCore::crPolygonOffset(1.0,1.0);
	//m_forMultiRenderStateSet->setAttributeAndModes(polygonOffset,CRCore::crStateAttribute::ON);
	CRCore::crDepth *depthAttr = new CRCore::crDepth;
	depthAttr->setFunction(CRCore::crDepth::LEQUAL);
	depthAttr->setWriteMask(false);
	m_forMultiRenderStateSet->setAttribute(depthAttr,CRCore::crStateAttribute::ON);
	m_forMultiRenderStateSet->setRenderingHint(crStateSet::MULTIRENDER_BIN);

	const unsigned int Override_On = crStateAttribute::ON/*|crStateAttribute::OVERRIDE*/;
	//const unsigned int Override_Off = crStateAttribute::OFF|crStateAttribute::OVERRIDE;
	m_outlinePerStateSet = new CRCore::crStateSet;
	CRCore::crStencil* stencil = new CRCore::crStencil;
	stencil->setFunction(crStencil::ALWAYS,1,~0u);
	stencil->setOperation(crStencil::KEEP, crStencil::KEEP, crStencil::REPLACE);
	m_outlinePerStateSet->setAttributeAndModes(stencil,Override_On);

	m_outlinePostStateSet = new CRCore::crStateSet;
	stencil = new CRCore::crStencil;
	stencil->setFunction(crStencil::NOTEQUAL,1,~0u);
	stencil->setOperation(crStencil::KEEP, crStencil::KEEP, crStencil::REPLACE);
	m_outlinePostStateSet->setAttributeAndModes(stencil,Override_On);
	crCullFace* cullFace = new crCullFace;
	cullFace->setMode(crCullFace::FRONT);
	m_outlinePostStateSet->setAttributeAndModes(cullFace, Override_On);
	crPolygonMode *polygonMode = new crPolygonMode;
	polygonMode->setMode(crPolygonMode::BACK,crPolygonMode::LINE);
	m_outlinePostStateSet->setAttributeAndModes(polygonMode,Override_On);
	crLineWidth *lineWidth = new crLineWidth;
	lineWidth->setWidth(crDisplaySettings::instance()->getOutLineWidth());
	m_outlinePostStateSet->setAttributeAndModes(lineWidth,Override_On);
	CRCore::crPolygonOffset *polygonOffset = new CRCore::crPolygonOffset(4.0,1.0);
	m_outlinePostStateSet->setAttributeAndModes(polygonOffset,CRCore::crStateAttribute::ON);

	//// outline color/material
	//crMaterial* material = new crMaterial;
	//material->setColorMode(crMaterial::EMISSION);
	//const crMaterial::Face face = crMaterial::FRONT;
	//const crVector3& color = crDisplaySettings::instance()->getOutLineColor();
	//crVector4 color4(color[0],color[1],color[2],1.0f);
	//material->setEmission(face, color4);
	//m_outlineStateSet2->setAttributeAndModes(material,Override_On);
	// disable modes
	//m_outlineStateSet2->setMode(GL_BLEND, Override_Off);
	//m_outlineStateSet2->setTextureMode(0, GL_TEXTURE_1D, Override_Off);
	//m_outlineStateSet2->setTextureMode(0, GL_TEXTURE_2D, Override_Off);
	//m_outlineStateSet2->setTextureMode(0, GL_TEXTURE_3D, Override_Off);
	//m_outlineStateSet2->setRenderingHint(crStateSet::OUTLINE_BIN);
	crStateSet *shader = crShaderManager::getInstance()->getShaderStateSet("outline");
	if(shader) m_outlinePostStateSet->merge(*shader);

	m_outlineColorVec.reserve(crObject::OL_Count);
	for(int i = 0; i<crObject::OL_Count; i++)
		m_outlineColorVec.push_back(new crStateSet);
	crUniform *lineColor = m_outlineColorVec[crObject::OL_Black]->getOrCreateUniform("outline_color",crUniform::FLOAT_VEC3);
	//const crVector3& color = crDisplaySettings::instance()->getOutLineColor();
	lineColor->setDataVariance(crBase::STATIC);
	lineColor->set(crVector3(0.0f,0.0f,0.0f));

	lineColor = m_outlineColorVec[crObject::OL_Red]->getOrCreateUniform("outline_color",crUniform::FLOAT_VEC3);
	lineColor->setDataVariance(crBase::STATIC);
	lineColor->set(crVector3(1.0f,0.0f,0.0f));

	lineColor = m_outlineColorVec[crObject::OL_Green]->getOrCreateUniform("outline_color",crUniform::FLOAT_VEC3);
	lineColor->setDataVariance(crBase::STATIC);
	lineColor->set(crVector3(0.0f,1.0f,0.0f));

	lineColor = m_outlineColorVec[crObject::OL_Blue]->getOrCreateUniform("outline_color",crUniform::FLOAT_VEC3);
	lineColor->setDataVariance(crBase::STATIC);
	lineColor->set(crVector3(0.0f,0.0f,1.0f));

	lineColor = m_outlineColorVec[crObject::OL_Yellow]->getOrCreateUniform("outline_color",crUniform::FLOAT_VEC3);
	lineColor->setDataVariance(crBase::STATIC);
	lineColor->set(crVector3(1.0f,1.0f,0.0f));

	lineColor = m_outlineColorVec[crObject::OL_Edit]->getOrCreateUniform("outline_color",crUniform::FLOAT_VEC3);
	lineColor->setDataVariance(crBase::STATIC);
	lineColor->set(crVector3(0.5f,0.0f,0.0f));
	//material = new crMaterial;
	//material->setColorMode(crMaterial::EMISSION);
	//const crVector3& scolor = crDisplaySettings::instance()->getSelectOutLineColor();
	//color4.set(scolor[0],scolor[1],scolor[2],1.0f);
	//material->setEmission(face, color4);
	//m_selectOutlineStateSet->setAttributeAndModes(material,Override_On);
	// disable modes
	//m_selectOutlineStateSet->setMode(GL_BLEND, Override_Off);
	//m_selectOutlineStateSet->setTextureMode(0, GL_TEXTURE_1D, Override_Off);
	//m_selectOutlineStateSet->setTextureMode(0, GL_TEXTURE_2D, Override_Off);
	//m_selectOutlineStateSet->setTextureMode(0, GL_TEXTURE_3D, Override_Off);
	//m_selectOutlineStateSet->setRenderingHint(crStateSet::OUTLINE_BIN);
}


crCullVisitor::~crCullVisitor()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crCullVisitor()"<< std::endl;
    reset();
}


void crCullVisitor::reset()
{

	//crShaderManager::getInstance()->resetCulling();
    //
    // first unref all referenced objects and then empty the containers.
    //
    
    crCullStack::reset();

    // reset the calculated near far planes.
    m_computed_znear = FLT_MAX;
    m_computed_zfar = -FLT_MAX;
    
    
    CRCore::crVector3 lookVector(0.0,0.0,-1.0);
    
    m_bbCornerFar = (lookVector.x()>=0?1:0) |
                   (lookVector.y()>=0?2:0) |
                   (lookVector.z()>=0?4:0);

    m_bbCornerNear = (~m_bbCornerFar)&7;
    
    // reset the resuse lists.
    //m_currentReuseMatrixIndex = 0;
    //m_currentReuseRenderLeafIndex = 0;
    
    //for(RenderLeafList::iterator itr=m_reuseRenderLeafList.begin();
    //    itr!=m_reuseRenderLeafList.end();
    //    ++itr)
    //{
    //    (*itr)->reset();
    //}
 //   if (m_impostorSpriteManager.valid()) m_impostorSpriteManager->reset();

	//m_showBoundingBox = false;
	//m_notPushStateSet = false;
	//m_renderMode = NormalRender;

	m_nearPlaneCandidateMap.clear();
    //m_enableSpecialCulling = false;

	//while(!m_computeAlphaShadowStack.empty()) m_computeAlphaShadowStack.pop();
	//m_computeAlphaShadowStack.push(false);

	//int size = m_stdLightEffectStateSetSet.size();
	//m_stdLightEffectStateSetSet.clear();
	//m_stdLightEffectStateSetSet.reserve(size);
    ////m_casStateSetList.clear();
}
void crCullVisitor::addDrawable(const CRCore::crObject *obj, CRCore::crDrawable* drawable, CRCore::RefMatrix* matrix, bool effectByShadow, CRCore::crNode *needUnLockNode)
{
	drawable->resumeRendering();
	//int popAlphaShadowFlg;
	CRCore::crStateSet *drawabless = drawable->getStateSet();
	bool stateset_pushed = false;
	if (m_renderMode == NormalRender || m_renderMode == RTTRender || m_renderMode == GIMapRender)
	{
		bool olstate1_pushed = false;
		bool drawoutline = m_renderMode == NormalRender && obj->getDrawOutLine() >= 0 && (obj->getDrawOutLine()>0 || obj->getOutlineColorMode()>CRCore::crObject::OL_Black);
		if (drawoutline)
		{
			if (m_outlinePerStateSet.valid()) olstate1_pushed = pushStateSet(m_outlinePerStateSet.get());
		}

		effectByShadow &= m_renderMode == NormalRender;
		const std::string &ts = getTraverseString();

		CRCore::crShaderManager::Callback *callback = 0;
		if (!ts.empty())
		{
			callback = CRCore::crShaderManager::getInstance()->getCallback(ts);
		}
		if (!callback)
		{
			stateset_pushed = false;
			if (drawabless)
				stateset_pushed = pushStateSet(drawabless/*,popAlphaShadowFlg*/);

			if (m_currentRenderGraph->leaves_empty())
				m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
			m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable, m_projectionStack.back().get(), matrix, 0.0f, needUnLockNode));

			if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);
		}
		else
		{
			CRCore::crMatrix mat = *matrix * getCameraViewInverseMatrix();
			CRCore::crBoundingBox box = drawable->getBound();
			box.m_max = box.m_max * mat;
			box.m_min = box.m_min * mat;

			box.correct();

			CRCore::crShaderManager::StateSetVec statesetVec;
			bool doNotNeedDrawableStateSet = m_renderMode == GIMapRender ? callback->giMapRender(obj, drawable, statesetVec) : (*callback)(obj, drawable, box, 0.0f, effectByShadow, statesetVec);
			int size = statesetVec.size();
			//bool needlight = ts.compare("needlight") == 0;
			if (size == 0/* && !needlight*/)
			{
				stateset_pushed = false;
				if (drawabless)
					stateset_pushed = pushStateSet(drawabless/*,popAlphaShadowFlg*/);

				if (m_currentRenderGraph->leaves_empty())
					m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
				m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable, m_projectionStack.back().get(), matrix, 0.0f, needUnLockNode));

				if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);
			}
			else
			{
				//int popAlpha_s,popAlpha_u,popAlpha_multi;
				bool needMultiRender = false;
				//bool morepass = ts.compare("morepass") == 0;
				bool shader_pushed, uniform_pushed, multi_pushed;
				if (size > 1/* || needlight*/)
				{
					needMultiRender = true;
				}
				CRCore::crStateSet *first_ss;
				CRCore::crStateSet *second_ss;
				for (CRCore::crShaderManager::StateSetVec::iterator itr = statesetVec.begin();
					itr != statesetVec.end(); )
				{
					shader_pushed = false;
					stateset_pushed = false;
					uniform_pushed = false;
					multi_pushed = false;

					first_ss = itr->first.get();
					second_ss = itr->second.get();

					if (++itr == statesetVec.end()/* && morepass*/)
					{
						needMultiRender = false;
						if (doNotNeedDrawableStateSet) drawabless = 0;
					}
					if (needMultiRender)
					{
						multi_pushed = pushStateSet(m_forMultiRenderStateSet.get()/*,popAlpha_multi*/);
					}

					shader_pushed = pushStateSet(first_ss/*,popAlpha_s*/);

					if (drawabless)
						stateset_pushed = pushStateSet(drawabless/*,popAlphaShadowFlg*/);

					if (second_ss)
						uniform_pushed = pushStateSet(second_ss, true/*,popAlpha_u*/);

					if (m_currentRenderGraph->leaves_empty())
						m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
					m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable, m_projectionStack.back().get(), matrix, 0.0f, needUnLockNode));

					if (uniform_pushed) popStateSet(/*popAlpha_u*/);
					if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);
					if (shader_pushed) popStateSet(/*popAlpha_s*/);
					if (multi_pushed) popStateSet(/*popAlpha_multi*/);
				}
			}
		}
		if (drawoutline && olstate1_pushed)
		{
			popStateSet();
			bool olstate2_pushed = pushStateSet(m_outlinePostStateSet.get());
			unsigned char colorMode = obj->getOutlineColorMode();
			if (colorMode >= m_outlineColorVec.size())
				colorMode = 0;
			bool olstate3_pushed = pushStateSet(m_outlineColorVec[colorMode].get());

			if (m_currentRenderGraph->leaves_empty())
				m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
			m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable, m_projectionStack.back().get(), matrix, 0.0f, needUnLockNode));

			if (olstate2_pushed) popStateSet();
			if (olstate3_pushed) popStateSet();
		}
		/*		if(tsstr.compare("0x0")==0)
		{
		ss = getOrCreateStdLightEffectStateSet(obj,drawable->getBound(), matrix,false);
		if(ss) pushStateSet(ss,popAlphaShadowFlg);

		if (m_currentRenderGraph->leaves_empty())
		m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
		m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,0.0f,needUnLockNode));

		if(ss) popStateSet(popAlphaShadowFlg);
		}
		else *//* if(tsstr.compare("0x1")==0 || tsstr.compare("0x4")==0)
		{
		if(tsstr.compare("0x1")==0)
		ss = getOrCreateLightEffectStateSet(obj,drawable->getBound(), matrix,effectByShadow);
		else
		ss = getOrCreateLightEffectStateSet(obj,drawable->getBound(), matrix,false);
		if(ss) pushStateSet(ss,popAlphaShadowFlg);

		if (m_currentRenderGraph->leaves_empty())
		m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
		m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,0.0f,needUnLockNode));

		if(ss) popStateSet(popAlphaShadowFlg);
		}
		else if(tsstr.compare("0x2")==0 || tsstr.compare("0x3")==0)
		{
		if(tsstr.compare("0x2")==0)
		createLightEffectStateSetVec(obj,drawable->getBound(), matrix,effectByShadow);
		else
		createLightEffectStateSetVec(obj,drawable->getBound(), matrix,false);

		//CRCore::notify(CRCore::FATAL)<<"m_tempLsUniformStateSetVec size =  "<<m_tempLsUniformStateSetVec.size()<<" frame = "<<getFrameStamp()->getFrameNumber()<<std::endl;
		for( StateSetVec::iterator itr = m_tempLsUniformStateSetVec.begin();
		itr != m_tempLsUniformStateSetVec.end();
		itr++ )
		{
		pushStateSet(itr->get(),popAlphaShadowFlg);

		if (m_currentRenderGraph->leaves_empty())
		m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
		m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,0.0f,needUnLockNode));

		popStateSet(popAlphaShadowFlg);
		}
		}
		else
		{
		if (m_currentRenderGraph->leaves_empty())
		m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
		m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,0.0f,needUnLockNode));
		}*/
	}
	else if (m_renderMode == ShadowMapRender)
	{
		CRCore::crShaderManager::StateSetVec statesetVec;
		if (!CRCore::crShaderManager::getInstance()->getShadowShaderStateSetVec(obj, drawable, statesetVec))
			return;

		bool shader_pushed, uniform_pushed;

		shader_pushed = false;
		//stateset_pushed = false;
		uniform_pushed = false;

		shader_pushed = pushStateSet(statesetVec[0].first.get());//push shader

																 //if(drawabless) //push drawabless
																 //	stateset_pushed = pushStateSet(drawabless);

		if (statesetVec[0].second.valid())
			uniform_pushed = pushStateSet(statesetVec[0].second.get()/*,true*/);//push uniforms,可能是导致树叶阴影UV动画实效的原因

		if (m_currentRenderGraph->leaves_empty())
			m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
		m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable, m_projectionStack.back().get(), matrix, 0.0f, needUnLockNode));

		if (uniform_pushed) popStateSet();
		//if(stateset_pushed) popStateSet();
		if (shader_pushed) popStateSet();
	}
	else if (m_renderMode == OnlyGeodeRender)
	{
		if (m_currentRenderGraph->leaves_empty())
			m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
		m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable, m_projectionStack.back().get(), matrix, 0.0f, needUnLockNode));
	}
	else
	{
		stateset_pushed = false;
		if (drawabless) stateset_pushed = pushStateSet(drawabless/*,popAlphaShadowFlg*/);

		if (m_currentRenderGraph->leaves_empty())
			m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
		m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable, m_projectionStack.back().get(), matrix, 0.0f, needUnLockNode));

		if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);
	}
}

/** Add a drawable and depth to current render graph.*/
void crCullVisitor::addDrawableAndDepth(const CRCore::crObject *obj, CRCore::crDrawable* drawable, CRCore::RefMatrix* matrix, float depth, bool effectByShadow, CRCore::crNode *needUnLockNode)
{
	//	CRCore::notify(CRCore::FATAL)<<"crCullVisitor::addDrawableAndDepth "<<ts.c_str()<<std::endl;
	//int popAlphaShadowFlg;
	//try
	//{
	drawable->resumeRendering();
	bool stateset_pushed = false;
	CRCore::crStateSet *drawabless = drawable->getStateSet();
	if (m_renderMode == NormalRender || m_renderMode == RTTRender || m_renderMode == GIMapRender)
	{
		bool olstate1_pushed = false;
		bool drawoutline = m_renderMode == NormalRender && obj->getDrawOutLine() >= 0 && ((obj->getDrawOutLine()>0 && depth<CRCore::crDisplaySettings::instance()->getOutLineDispDistance()) || obj->getOutlineColorMode()>CRCore::crObject::OL_Black);
		if (drawoutline)
		{
			if (m_outlinePerStateSet.valid()) olstate1_pushed = pushStateSet(m_outlinePerStateSet.get());
		}

		effectByShadow &= m_renderMode == NormalRender;
		const std::string &ts = getTraverseString();
		CRCore::crShaderManager::Callback *callback = 0;
		if (!ts.empty())
		{
			callback = CRCore::crShaderManager::getInstance()->getCallback(ts);
		}
		//if(callback) CRCore::notify(CRCore::ALWAYS)<<"callback "<<ts<< std::endl;
		if (!callback)
		{
			//if(strcmp(drawable->className(),"crParticleSystem") == 0)
			//{
			//	CRCore::notify(CRCore::ALWAYS)<<"crParticleSystem..."<< std::endl;
			//	CRCore::notify(CRCore::ALWAYS)<<"m_renderMode = "<<m_renderMode<< std::endl;
			//	CRCore::notify(CRCore::ALWAYS)<<"ts = "<<getTraverseString()<< std::endl;
			//	CRCore::notify(CRCore::ALWAYS)<<"drawabless = "<<drawabless<< std::endl;
			//}
			stateset_pushed = false;
			if (drawabless) stateset_pushed = pushStateSet(drawabless/*,popAlphaShadowFlg*/);

			if (m_currentRenderGraph->leaves_empty())
				m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
			m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable, m_projectionStack.back().get(), matrix, depth, needUnLockNode));

			if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);
		}
		else
		{
			CRCore::crBoundingBox box = drawable->getBound();
			if (matrix)
			{
				CRCore::crMatrix mat = *matrix * getCameraViewInverseMatrix();
				box.m_max = box.m_max * mat;
				box.m_min = box.m_min * mat;
				box.correct();
			}
			CRCore::crShaderManager::StateSetVec statesetVec;
			bool doNotNeedDrawableStateSet = m_renderMode == GIMapRender ? callback->giMapRender(obj, drawable, statesetVec) : (*callback)(obj, drawable, box, depth, effectByShadow, statesetVec);
			int size = statesetVec.size();
			//bool needlight = ts.compare("needlight") == 0;
			if (size == 0/* && !needlight*/)
			{
				stateset_pushed = false;
				if (drawabless) stateset_pushed = pushStateSet(drawabless/*,popAlphaShadowFlg*/);

				if (m_currentRenderGraph->leaves_empty())
					m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
				m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable, m_projectionStack.back().get(), matrix, depth, needUnLockNode));

				if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);
			}
			else
			{
				//int popAlpha_s,popAlpha_u,popAlpha_multi;
				bool needMultiRender = false;
				//bool morepass = ts.compare("morepass") == 0;
				bool shader_pushed, uniform_pushed, multi_pushed;
				if (size > 1/* || needlight*/)
				{
					needMultiRender = true;
				}
				CRCore::crStateSet *first_ss;
				CRCore::crStateSet *second_ss;
				for (CRCore::crShaderManager::StateSetVec::iterator itr = statesetVec.begin();
					itr != statesetVec.end(); )
				{
					shader_pushed = false;
					stateset_pushed = false;
					uniform_pushed = false;
					multi_pushed = false;

					first_ss = itr->first.get();
					second_ss = itr->second.get();

					if (++itr == statesetVec.end()/* && morepass*/)
					{
						needMultiRender = false;
						if (doNotNeedDrawableStateSet) drawabless = 0;
					}

					if (needMultiRender)
					{
						multi_pushed = pushStateSet(m_forMultiRenderStateSet.get()/*,popAlpha_multi*/);
						//CRCore::notify(CRCore::WARN) << "crCullVisitor::addDrawableAndDepth multi_pushed = "<<statesetVec.size()<<std::endl;
					}

					shader_pushed = pushStateSet(first_ss/*,popAlpha_s*/);

					if (drawabless) stateset_pushed = pushStateSet(drawabless/*,popAlphaShadowFlg*/);

					if (second_ss)
						uniform_pushed = pushStateSet(second_ss, true/*,popAlpha_u*/);

					if (m_currentRenderGraph->leaves_empty())
						m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
					m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable, m_projectionStack.back().get(), matrix, depth, needUnLockNode));

					if (uniform_pushed) popStateSet(/*popAlpha_u*/);
					if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);
					if (shader_pushed) popStateSet(/*popAlpha_s*/);
					if (multi_pushed) popStateSet(/*popAlpha_multi*/);
				}
			}
		}
		if (drawoutline && olstate1_pushed)
		{
			popStateSet();
			bool olstate2_pushed = pushStateSet(m_outlinePostStateSet.get());
			unsigned char colorMode = obj->getOutlineColorMode();
			if (colorMode >= m_outlineColorVec.size())
				colorMode = 0;
			bool olstate3_pushed = pushStateSet(m_outlineColorVec[colorMode].get());

			if (m_currentRenderGraph->leaves_empty())
				m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
			m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable, m_projectionStack.back().get(), matrix, depth, needUnLockNode));

			if (olstate2_pushed) popStateSet();
			if (olstate3_pushed) popStateSet();
		}
	}
	else if (m_renderMode == ShadowMapRender)
	{
		CRCore::crShaderManager::StateSetVec statesetVec;
		if (!CRCore::crShaderManager::getInstance()->getShadowShaderStateSetVec(obj, drawable, statesetVec))
		{
			//CRCore::notify(CRCore::FATAL)<<"getShadowShaderStateSetVec: "<<std::endl;
			return;
		}
		bool shader_pushed, uniform_pushed;

		shader_pushed = false;
		//stateset_pushed = false;
		uniform_pushed = false;

		shader_pushed = pushStateSet(statesetVec[0].first.get());//push shader

																 //if(drawabless) //push drawabless
																 //	stateset_pushed = pushStateSet(drawabless);

		if (statesetVec[0].second.valid()) uniform_pushed = pushStateSet(statesetVec[0].second.get(), false);//push uniforms

																											 //if(stateset_pushed) CRCore::notify(CRCore::ALWAYS)<<"ShadowMapRender "<<obj->getName()<<std::endl;

		if (m_currentRenderGraph->leaves_empty())
			m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
		m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable, m_projectionStack.back().get(), matrix, depth, needUnLockNode));

		if (uniform_pushed) popStateSet();
		//if(stateset_pushed) popStateSet();
		if (shader_pushed) popStateSet();
	}
	else if (m_renderMode == OnlyGeodeRender || m_renderMode == NRPRender)
	{
		if (m_currentRenderGraph->leaves_empty())
			m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
		m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable, m_projectionStack.back().get(), matrix, depth/*0.0f*/, needUnLockNode));
	}
	else
	{
		stateset_pushed = false;
		if (drawabless) stateset_pushed = pushStateSet(drawabless/*,popAlphaShadowFlg*/);

		if (m_currentRenderGraph->leaves_empty())
			m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
		m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable, m_projectionStack.back().get(), matrix, depth/*0.0f*/, needUnLockNode));

		if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);
	}
	/*	if(m_renderMode == NormalRender)
	{
	int popAlphaShadowFlg;
	const std::string &tsstr = getTraverseString();
	CRCore::crStateSet *ss;
	if(tsstr.compare("0x1")==0 || tsstr.compare("0x4")==0)
	{//受0－3个光源影响的渲染
	if(tsstr.compare("0x1")==0)
	ss = getOrCreateLightEffectStateSet(obj,drawable->getBound(), matrix,effectByShadow);
	else
	ss = getOrCreateLightEffectStateSet(obj,drawable->getBound(), matrix,false);
	if(ss) pushStateSet(ss,popAlphaShadowFlg);

	if (m_currentRenderGraph->leaves_empty())
	m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
	m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,depth,needUnLockNode));

	if(ss) popStateSet(popAlphaShadowFlg);
	}
	else if(tsstr.compare("0x2")==0 || tsstr.compare("0x3")==0)
	{//超过3个光源影响的渲染
	if(tsstr.compare("0x2")==0)
	createLightEffectStateSetVec(obj,drawable->getBound(), matrix,effectByShadow);
	else
	createLightEffectStateSetVec(obj,drawable->getBound(), matrix,false);

	//CRCore::notify(CRCore::FATAL)<<"m_tempLsUniformStateSetVec size =  "<<m_tempLsUniformStateSetVec.size()<<" frame = "<<getFrameStamp()->getFrameNumber()<<std::endl;
	for( StateSetVec::iterator itr = m_tempLsUniformStateSetVec.begin();
	itr != m_tempLsUniformStateSetVec.end();
	itr++ )
	{
	pushStateSet(itr->get(),popAlphaShadowFlg);

	if (m_currentRenderGraph->leaves_empty())
	m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
	m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,depth,needUnLockNode));

	popStateSet(popAlphaShadowFlg);
	}
	}
	else
	{
	if (m_currentRenderGraph->leaves_empty())
	m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
	m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,depth,needUnLockNode));
	}

	}
	else
	{
	if (m_currentRenderGraph->leaves_empty())
	m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
	m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,depth,needUnLockNode));
	}*/

	//}
	//catch (...)
	//{
	//	CRCore::notify(CRCore::FATAL)<<"crCullVisitor::addDrawableAndDepth() error "<<m_renderMode<<getTraverseString()<<std::endl;
	//}
}
void crCullVisitor::handle_cull_callbacks_and_traverse(CRCore::crNode& node)
{
#ifdef _DEBUG
	try
	{
#endif
		node.doEvent(WCH_CULLVISITOR, MAKECREPARAM(this, NULL));
		CRCore::crNodeCallback* callback = node.getCullCallback();
		if (callback) (*callback)(&node, this);
		else traverse(node);
#ifdef _DEBUG
	}
	catch (...)
	{
		//CRCore::notify(CRCore::ALWAYS)<<"handle_cull_callbacks_and_traverse error "<<node.getName()<<std::endl;
		char gbuf[256];
		sprintf(gbuf, "handle_cull_callbacks_and_traverse error %s,%s,%s,code=%d\n\0", node.getName().c_str(), node.className(), node.getParent(0)->className(), GetLastError());
		gDebugInfo->debugInfo(CRCore::ALWAYS, gbuf);

		CRCore::NodePath &nodePath = getNodePath();
		for (int i = 0; i < nodePath.size(); i++)
		{
			sprintf(gbuf, "NodePath %s,%s\n\0", nodePath[i]->getName().c_str(), nodePath[i]->className());
			gDebugInfo->debugInfo(CRCore::ALWAYS, gbuf);
		}
		HANDLE handle = GetCurrentProcess();
		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
		sprintf(gbuf, "内存使用:%d,峰值内存:%d,虚拟内存:%dKB,峰值虚拟内存:%dKB\n\0", pmc.WorkingSetSize / 1024, pmc.PeakWorkingSetSize / 1024, pmc.PagefileUsage / 1024, pmc.PeakPagefileUsage / 1024);
		gDebugInfo->debugInfo(CRCore::ALWAYS, gbuf);
		_asm   int   3   //只是为了让程序崩溃
	}
#endif
}

void crCullVisitor::handle_cull_callbacks_and_accept(CRCore::crNode& node, CRCore::crNode* acceptNode)
{
	node.doEvent(WCH_CULLVISITOR, MAKECREPARAM(this, NULL));
	CRCore::crNodeCallback* callback = node.getCullCallback();
	if (callback) (*callback)(&node, this);
	else acceptNode->accept(*this);
}
void crCullVisitor::setState(CRCore::crState* state) 
{ 
	m_state = state; 
}

CRCore::crState* crCullVisitor::getState() 
{ 
	return m_state.get(); 
}

const CRCore::crState* crCullVisitor::getState() const 
{ 
	return m_state.get(); 
}

float crCullVisitor::getDistanceToEyePoint(const crVector3& pos, bool withLODScale) const
{
    if (withLODScale) return (pos-getEyeLocal()).length()*getLODScale();
    else return (pos-getEyeLocal()).length();
}

inline crCullVisitor::value_type distance(const CRCore::crVector3& coord,const CRCore::crMatrix& matrix)
{
    return -((crCullVisitor::value_type)coord[0]*(crCullVisitor::value_type)matrix(0,2)+(crCullVisitor::value_type)coord[1]*(crCullVisitor::value_type)matrix(1,2)+(crCullVisitor::value_type)coord[2]*(crCullVisitor::value_type)matrix(2,2)+matrix(3,2));
}

float crCullVisitor::getDistanceFromEyePoint(const CRCore::crVector3& pos, bool withLODScale) const
{
    const crMatrix& matrix = *m_modelviewStack.back();
    float dist = distance(pos,matrix);
    
    if (withLODScale) return dist*getLODScale();
    else return dist;
}

void crCullVisitor::popProjectionMatrix()
{
	if (!m_nearPlaneCandidateMap.empty())
	{ 

		// CRCore::Timer_t start_t = CRCore::Timer::instance()->tick();

		// update near from defferred list of drawables
		unsigned int numTests = 0;
		for(DistanceMatrixDrawableMap::iterator itr=m_nearPlaneCandidateMap.begin();
			itr!=m_nearPlaneCandidateMap.end() && itr->first<m_computed_znear;
			++itr)
		{
			++numTests;
			// CRCore::notify(CRCore::WARN)<<"testing computeNearestPointInFrustum with d_near = "<<itr->first<<std::endl;
			value_type d_near = computeNearestPointInFrustum(itr->second.m_matrix, itr->second.m_planes,*(itr->second.m_drawable));
			if (d_near<m_computed_znear)
			{
				m_computed_znear = d_near;
				// CRCore::notify(CRCore::WARN)<<"updating znear to "<<m_computed_znear<<std::endl;
			}
		} 

		// CRCore::Timer_t end_t = CRCore::Timer::instance()->tick();
		// CRCore::notify(CRCore::NOTICE)<<"Took "<<CRCore::Timer::instance()->delta_m(start_t,end_t)<<"ms to test "<<numTests<<" out of "<<m_nearPlaneCandidateMap.size()<<std::endl;
	}
    if (m_computeNearFar && m_computed_zfar>=m_computed_znear)
    {

        // adjust the projection matrix so that it encompases the local coords.
        // so it doesn't cull them out.
        CRCore::crMatrix& projection = *m_projectionStack.back();
		value_type tmp_znear = m_computed_znear;
		value_type tmp_zfar = m_computed_zfar;

        clampProjectionMatrix(projection, tmp_znear, tmp_zfar);
    }

    crCullStack::popProjectionMatrix();
}

template<class matrix_type, class value_type>
bool _clampProjectionMatrix(matrix_type& projection, double znear, double zfar, value_type nearFarRatio)
{
	if (zfar>=znear)
	{

		double epsilon = 1e-6;
		if (fabs(projection(0,3))<epsilon  && fabs(projection(1,3))<epsilon  && fabs(projection(2,3))<epsilon )
		{
			value_type delta_span = (zfar-znear)*0.02;
			if (delta_span<1.0) delta_span = 1.0;
			value_type desired_znear = znear - delta_span;
			value_type desired_zfar = zfar + delta_span;

			// assign the clamped values back to the computed values.
			znear = desired_znear;
			zfar = desired_zfar;

			projection(2,2)=-2.0f/(desired_zfar-desired_znear);
			projection(3,2)=-(desired_zfar+desired_znear)/(desired_zfar-desired_znear);

			return true;

		}
		else
		{
			value_type zfarPushRatio = 1.02;
			value_type znearPullRatio = 0.98;

			//znearPullRatio = 0.99; 

			value_type desired_znear = znear * znearPullRatio;
			value_type desired_zfar = zfar * zfarPushRatio;

			// near plane clamping.
			double min_near_plane = zfar*nearFarRatio;
			if (desired_znear<min_near_plane) desired_znear=min_near_plane;

			// assign the clamped values back to the computed values.
			znear = desired_znear;
			zfar = desired_zfar;

			value_type trans_near_plane = (-desired_znear*projection(2,2)+projection(3,2))/(-desired_znear*projection(2,3)+projection(3,3));
			value_type trans_far_plane = (-desired_zfar*projection(2,2)+projection(3,2))/(-desired_zfar*projection(2,3)+projection(3,3));

			value_type ratio = fabs(2.0/(trans_near_plane-trans_far_plane));
			value_type center = -(trans_near_plane+trans_far_plane)/2.0;

			projection.postMult(CRCore::crMatrix(1.0f,0.0f,0.0f,0.0f,
				0.0f,1.0f,0.0f,0.0f,
				0.0f,0.0f,ratio,0.0f,
				0.0f,0.0f,center*ratio,1.0f));

			return true;
		}
	}
	return false;
}


bool crCullVisitor::clampProjectionMatrixImplementation(CRCore::crMatrixf& projection, double znear, double zfar) const
{
    return _clampProjectionMatrix( projection, znear, zfar, m_nearFarRatio );
}

bool crCullVisitor::clampProjectionMatrixImplementation(CRCore::crMatrixd& projection, double znear, double zfar) const
{
    return _clampProjectionMatrix( projection, znear, zfar, m_nearFarRatio );
}

struct ComputeNearestPointFunctor
{

	ComputeNearestPointFunctor():
		m_planes(0) {}

	void set(crCullVisitor::value_type znear, const CRCore::crMatrix& matrix, const CRCore::crPolytope::PlaneList* planes)
	{
		m_znear = znear;
		m_matrix = matrix;
		m_planes = planes;
	}

	typedef std::pair<float, CRCore::crVector3>  DistancePoint;
	typedef std::vector<DistancePoint>  Polygon;

	crCullVisitor::value_type         m_znear;
	CRCore::crMatrix                     m_matrix;
	const CRCore::crPolytope::PlaneList* m_planes;
	Polygon                         m_polygonOriginal;
	Polygon                         m_polygonNew;

	Polygon                         m_pointCache;

	inline void operator() ( const CRCore::crVector3 &v1, const CRCore::crVector3 &v2, const CRCore::crVector3 &v3, bool)
	{
		crCullVisitor::value_type n1 = distance(v1,m_matrix);
		crCullVisitor::value_type n2 = distance(v2,m_matrix);
		crCullVisitor::value_type n3 = distance(v3,m_matrix);

		// check if triangle is total behind znear, if so disguard
		if (n1 >= m_znear &&
			n2 >= m_znear &&
			n3 >= m_znear)
		{
			//CRCore::notify(CRCore::NOTICE)<<"Triangle totally beyond znear"<<std::endl;
			return;
		}


		if (n1 < 0.0 &&
			n2 < 0.0 &&
			n3 < 0.0)
		{
			// CRCore::notify(CRCore::NOTICE)<<"Triangle totally behind eye point"<<std::endl;
			return;
		}

		// check which planes the points
		CRCore::crPolytope::ClippingMask selector_mask = 0x1;
		CRCore::crPolytope::ClippingMask active_mask = 0x0;

		CRCore::crPolytope::PlaneList::const_iterator pitr;
		for(pitr = m_planes->begin();
			pitr != m_planes->end();
			++pitr)
		{
			const CRCore::crPlane& plane = *pitr;
			float d1 = plane.distance(v1);
			float d2 = plane.distance(v2);
			float d3 = plane.distance(v3);

			unsigned int numOutside = ((d1<0.0)?1:0) + ((d2<0.0)?1:0) + ((d3<0.0)?1:0);
			if (numOutside==3)
			{
				//CRCore::notify(CRCore::NOTICE)<<"Triangle totally outside frustum "<<d1<<"\t"<<d2<<"\t"<<d3<<std::endl;
				return;
			}
			unsigned int numInside = ((d1>=0.0)?1:0) + ((d2>=0.0)?1:0) + ((d3>=0.0)?1:0);
			if (numInside<3)
			{
				active_mask = active_mask | selector_mask;
			}

			//CRCore::notify(CRCore::NOTICE)<<"Triangle ok w.r.t plane "<<d1<<"\t"<<d2<<"\t"<<d3<<std::endl;

			selector_mask <<= 1; 
		}        

		if (active_mask==0)
		{
			m_znear = CRCore::minimum(m_znear,n1);
			m_znear = CRCore::minimum(m_znear,n2);
			m_znear = CRCore::minimum(m_znear,n3);
			// CRCore::notify(CRCore::NOTICE)<<"Triangle all inside frustum "<<n1<<"\t"<<n2<<"\t"<<n3<<" number of plane="<<m_planes->size()<<std::endl;
			return;
		}

		//return;

		// numPartiallyInside>0) so we have a triangle cutting an frustum wall,
		// this means that use brute force methods for deviding up triangle. 

		//CRCore::notify(CRCore::NOTICE)<<"Using brute force method of triangle cutting frustum walls"<<std::endl;
		m_polygonOriginal.clear();
		m_polygonOriginal.push_back(DistancePoint(0,v1));
		m_polygonOriginal.push_back(DistancePoint(0,v2));
		m_polygonOriginal.push_back(DistancePoint(0,v3));

		selector_mask = 0x1;


		for(pitr = m_planes->begin();
			pitr != m_planes->end() && !m_polygonOriginal.empty();
			++pitr)
		{
			if (active_mask & selector_mask)
			{    
				// polygon bisects plane so need to divide it up.
				const CRCore::crPlane& plane = *pitr;
				m_polygonNew.clear();

				// assign the distance from the current plane.
				for(Polygon::iterator polyItr = m_polygonOriginal.begin();
					polyItr != m_polygonOriginal.end();
					++polyItr)
				{
					polyItr->first = plane.distance(polyItr->second);
				}

				// create the new polygon by clamping against the 
				unsigned int psize = m_polygonOriginal.size();

				for(unsigned int ci = 0; ci < psize; ++ci)
				{
					unsigned int ni = (ci+1)%psize;
					bool computeIntersection = false;
					if (m_polygonOriginal[ci].first>=0.0f)
					{
						m_polygonNew.push_back(m_polygonOriginal[ci]);

						if (m_polygonOriginal[ni].first<0.0f) computeIntersection = true;
					}
					else if (m_polygonOriginal[ni].first>0.0f) computeIntersection = true;


					if (computeIntersection)
					{
						// segment intersects with the plane, compute new position.
						float r = m_polygonOriginal[ci].first/(m_polygonOriginal[ci].first-m_polygonOriginal[ni].first);
						m_polygonNew.push_back(DistancePoint(0.0f,m_polygonOriginal[ci].second*(1.0f-r) + m_polygonOriginal[ni].second*r));
					}
				}
				m_polygonOriginal.swap(m_polygonNew);
			}
			selector_mask <<= 1; 
		}

		// now take the nearst points to the eye point.
		for(Polygon::iterator polyItr = m_polygonOriginal.begin();
			polyItr != m_polygonOriginal.end();
			++polyItr)
		{
			crCullVisitor::value_type dist = distance(polyItr->second,m_matrix);
			if (dist < m_znear) 
			{
				m_znear = dist;
				//CRCore::notify(CRCore::NOTICE)<<"Near plane updated "<<m_znear<<std::endl;
			}
		}
	}
};

crCullVisitor::value_type crCullVisitor::computeNearestPointInFrustum(const CRCore::crMatrix& matrix, const CRCore::crPolytope::PlaneList& planes,const CRCore::crDrawable& drawable)
{
	// CRCore::notify(CRCore::WARN)<<"crCullVisitor::computeNearestPointInFrustum("<<getTraversalNumber()<<"\t"<<planes.size()<<std::endl;

	CRCore::crTriangleFunctor<ComputeNearestPointFunctor> cnpf;
	cnpf.set(m_computed_znear, matrix, &planes);

	drawable.accept(cnpf);

	return cnpf.m_znear;
}

bool crCullVisitor::updateCalculatedNearFar(const CRCore::crMatrix& matrix,const CRCore::crBoundingBox& bb)
{
	// efficient computation of near and far, only taking into account the nearest and furthest
	// corners of the bounding box.
	value_type d_near = distance(bb.corner(m_bbCornerNear),matrix);
	value_type d_far = distance(bb.corner(m_bbCornerFar),matrix);

	if (d_near>d_far)
	{
		std::swap(d_near,d_far);
		if ( !EQUAL_F(d_near, d_far) ) 
		{
			//CRCore::notify(CRCore::WARN)<<"Warning: crCullVisitor::updateCalculatedNearFar(.) near>far in range calculation,"<< std::endl;
			//CRCore::notify(CRCore::WARN)<<"         correcting by swapping values d_near="<<d_near<<" dfar="<<d_far<< std::endl;
			char gbuf[256];
			sprintf(gbuf,"Warning: crCullVisitor::updateCalculatedNearFar(.) near>far in range calculation,correcting by swapping values d_near=%f dfar=%f\n\0",d_near,d_far);
			gDebugInfo->debugInfo(CRCore::WARN,gbuf);
		}
	}

	if (d_far<0.0)
	{
		// whole object behind the eye point so disguard
		return false;
	}

	if (d_near<m_computed_znear) m_computed_znear = d_near;
	if (d_far>m_computed_zfar) m_computed_zfar = d_far;

	return true;
}
bool crCullVisitor::updateCalculatedNearFar(const CRCore::crMatrix& matrix,const CRCore::crDrawable& drawable, bool isBillboard)
{
	const CRCore::crBoundingBox& bb = drawable.getBound();

	value_type d_near, d_far;

	if (isBillboard)
	{

#ifdef TIME_BILLBOARD_NEAR_FAR_CALCULATION    
		static unsigned int lastFrameNumber = getTraversalNumber();
		static unsigned int numBillboards = 0;
		static double elapsed_time = 0.0;
		if (lastFrameNumber != getTraversalNumber())
		{
			//CRCore::notify(CRCore::NOTICE)<<"Took "<<elapsed_time<<"ms to test "<<numBillboards<<" billboards"<<std::endl;
			numBillboards = 0;
			elapsed_time = 0.0;
			lastFrameNumber = getTraversalNumber();
		}
		CRCore::Timer_t start_t = CRCore::Timer::instance()->tick();
#endif

		CRCore::crVector3 lookVector(-matrix(0,2),-matrix(1,2),-matrix(2,2));

		unsigned int bbCornerFar = (lookVector.x()>=0?1:0) +
			(lookVector.y()>=0?2:0) +
			(lookVector.z()>=0?4:0);

		unsigned int bbCornerNear = (~bbCornerFar)&7;

		d_near = distance(bb.corner(bbCornerNear),matrix);
		d_far = distance(bb.corner(bbCornerFar),matrix);

		//CRCore::notify(CRCore::NOTICE).precision(15);

		if (false)
		{

			//CRCore::notify(CRCore::NOTICE)<<"TESTING Billboard near/far computation"<<std::endl;

			// CRCore::notify(CRCore::WARN)<<"Checking corners of billboard "<<std::endl;
			// deprecated brute force way, use all corners of the bounding box.
			value_type nd_near, nd_far;
			nd_near = nd_far = distance(bb.corner(0),matrix);
			for(unsigned int i=0;i<8;++i)
			{
				value_type d = distance(bb.corner(i),matrix);
				if (d<nd_near) nd_near = d;
				if (d>nd_far) nd_far = d;
				CRCore::notify(CRCore::NOTICE)<<"\ti="<<i<<"\td="<<d<<std::endl;
			}

			if (nd_near==d_near && nd_far==d_far)
			{
				CRCore::notify(CRCore::NOTICE)<<"\tBillboard near/far computation correct "<<std::endl;
			}
			else
			{
				CRCore::notify(CRCore::NOTICE)<<"\tBillboard near/far computation ERROR\n\t\t"<<d_near<<"\t"<<nd_near
					<<"\n\t\t"<<d_far<<"\t"<<nd_far<<std::endl;
			}

		}

#ifdef TIME_BILLBOARD_NEAR_FAR_CALCULATION    
		CRCore::Timer_t end_t = CRCore::Timer::instance()->tick();

		elapsed_time += CRCore::Timer::instance()->delta_m(start_t,end_t);
		++numBillboards;
#endif
	}
	else
	{
		// efficient computation of near and far, only taking into account the nearest and furthest
		// corners of the bounding box.
		d_near = distance(bb.corner(m_bbCornerNear),matrix);
		d_far = distance(bb.corner(m_bbCornerFar),matrix);
	}

	if (d_near>d_far)
	{
		std::swap(d_near,d_far);
		if ( !EQUAL_F(d_near, d_far) ) 
		{
			//CRCore::notify(CRCore::WARN)<<"Warning: crCullVisitor::updateCalculatedNearFar(.) near>far in range calculation,"<< std::endl;
			//CRCore::notify(CRCore::WARN)<<"         correcting by swapping values d_near="<<d_near<<" dfar="<<d_far<< std::endl;
			char gbuf[256];
			sprintf(gbuf,"Warning: crCullVisitor::updateCalculatedNearFar(.) near>far in range calculation,correcting by swapping values d_near=%f dfar=%f\n\0",d_near,d_far);
			gDebugInfo->debugInfo(CRCore::WARN,gbuf);
		}
	}

	if (d_far<0.0)
	{
		// whole object behind the eye point so discard
		return false;
	}

	if (d_near<m_computed_znear) 
	{
		if (m_computeNearFar==COMPUTE_NEAR_FAR_USING_PRIMITIVES)
		{
			CRCore::crPolytope& frustum = getCurrentCullingSet().getFrustum();
			if (frustum.getCurrentMask() && frustum.getResultMask())
			{
				if (isBillboard)
				{
					// CRCore::notify(CRCore::WARN)<<"Adding billboard into deffered list"<<std::endl;

					CRCore::crPolytope transformed_frustum;
					transformed_frustum.setAndTransformProvidingInverse(getProjectionCullingStack().back().getFrustum(),matrix);

					// insert drawable into the deferred list of drawables which will be handled at the popProjectionMatrix().
					m_nearPlaneCandidateMap.insert(
						DistanceMatrixDrawableMap::value_type(d_near,MatrixPlanesDrawables(matrix,&drawable,transformed_frustum)) );
				} 
				else
				{
					// insert drawable into the deferred list of drawables which will be handled at the popProjectionMatrix().
					m_nearPlaneCandidateMap.insert(
						DistanceMatrixDrawableMap::value_type(d_near,MatrixPlanesDrawables(matrix,&drawable,frustum)) );
				}



				// use the far point if its nearer than current znear as this is a conservative estimate of the znear
				// while the final computation for this drawable is deferred.
				if (d_far<m_computed_znear)
				{
					if (d_far<0.0) CRCore::notify(CRCore::WARN)<<"       1)  sett near with dnear="<<d_near<<"  dfar="<<d_far<< std::endl;
					else m_computed_znear = d_far;
				}

			}
			else
			{

				if (d_near<0.0) CRCore::notify(CRCore::WARN)<<"        2) sett near with d_near="<<d_near<< std::endl;
				else m_computed_znear = d_near;
			}

		}
		else
		{
			//if (d_near<0.0) CRCore::notify(CRCore::WARN)<<"         3) set near with d_near="<<d_near<< std::endl;
			m_computed_znear = d_near;
		}
	}    

	if (d_far>m_computed_zfar) m_computed_zfar = d_far;


	/*
	// deprecated brute force way, use all corners of the bounding box.
	updateCalculatedNearFar(bb.corner(0));
	updateCalculatedNearFar(bb.corner(1));
	updateCalculatedNearFar(bb.corner(2));
	updateCalculatedNearFar(bb.corner(3));
	updateCalculatedNearFar(bb.corner(4));
	updateCalculatedNearFar(bb.corner(5));
	updateCalculatedNearFar(bb.corner(6));
	updateCalculatedNearFar(bb.corner(7));
	*/

	return true;

}
void crCullVisitor::updateCalculatedNearFar(const CRCore::crVector3& pos)
{
	float d;
	if (!m_modelviewStack.empty())
	{
		const CRCore::crMatrix& matrix = *(m_modelviewStack.back());
		d = distance(pos,matrix);
	}
	else
	{
		d = -pos.z();
	}

	if (d<m_computed_znear) 
	{
		m_computed_znear = d;
		if (d<0.0) CRCore::notify(CRCore::WARN)<<"Alerting billboard ="<<d<< std::endl;
	}
	if (d>m_computed_zfar) m_computed_zfar = d;
}   

void crCullVisitor::apply(crNode& node)
{
	node.setNodeMask(node.getNodeMask() | CulledMask);
	if(!node.getVisiable() && m_renderMode != CollideMapRender) return;
	if(m_renderMode == ShadowMapRender&&!node.isCalcShadow()) return;
	if(m_renderMode == GIMapRender &&  !node.isGIMapRenderable()) return;

	if (isCulled(node)) return;
	node.setNodeMask(node.getNodeMask() & ~CulledMask);
 /*   if(!m_dependCullingBufStack.empty())
	{
		topDependCullingBuf()->addChild(&node);
	}*/

    // push the culling mode.
    pushCurrentMask();
    
    // push the node's state.
    crStateSet* node_state = node.getStateSet();
	//int popAlphaShadowFlg;
	bool stateset_pushed = false;
    if (node_state) stateset_pushed = pushStateSet(node_state/*,popAlphaShadowFlg*/);

    handle_cull_callbacks_and_traverse(node);

    // pop the node's state off the geostate stack.    
    if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);
    
    // pop the culling mode.
	popCurrentMask();
}

void crCullVisitor::apply(crDB& node)
{
	if(!node.getVisiable() && m_renderMode != CollideMapRender) return;
	if(m_renderMode == ShadowMapRender&&!node.isCalcShadow()) return;
	if (m_renderMode == GIMapRender &&  !node.isGIMapRenderable()) return;
	//if(!m_showBoundingBox)
	//	m_showBoundingBox = node.isShowBoundingBox();

	crStateSet* node_state = node.getStateSet();
	//int popAlphaShadowFlg;
	bool stateset_pushed = false;
	if (node_state) stateset_pushed = pushStateSet(node_state/*,popAlphaShadowFlg*/);

	handle_cull_callbacks_and_traverse(node);

	if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);

	//if(node.isShowBoundingBox()) m_showBoundingBox = false;
}

void crCullVisitor::apply(crObject& node)
{
	node.setNodeMask(node.getNodeMask() | CulledMask);
	if(!node.getVisiable() && m_renderMode != CollideMapRender) return;
	if(m_renderMode == ShadowMapRender&&!node.isCalcShadow()) return;
	if(m_renderMode == CollideMapRender && !node.isMapCollide()) return;
	if(m_renderMode == GIMapRender &&  !node.isGIMapRenderable()) return;
	//if (m_renderMode == ShadowMapRender2 && node.getBound().radius()<1.0f) return;

	bool culled = isCulled(node);
	//node.physicsCulled(culled);
	if (culled) return;
	node.setNodeMask(node.getNodeMask() & ~CulledMask);

	//if(!m_dependCullingBufStack.empty())
	//{
	//	topDependCullingBuf()->addChild(&node);
	//}

	//if(!m_showBoundingBox)
	//	m_showBoundingBox = node.isShowBoundingBox();

    // push the node's state.
    crStateSet* node_state = node.getStateSet();
	//int popAlphaShadowFlg;
	bool stateset_pushed = false;
    if (node_state) stateset_pushed = pushStateSet(node_state/*,popAlphaShadowFlg*/);

    // traverse any call callbacks and traverse any children.
    handle_cull_callbacks_and_traverse(node);

    RefMatrix& matrix = getModelViewMatrix();
	int numDrawables = node.getNumDrawables();
	//int drawablePopAlphaShadowFlg;
	//std::stack<int> drawablePopAlphaShadowFlgStack;
    
	//bool effectByShadow = true;
	//std::string str;
	//if(crArgumentParser::readKeyValue(node.getDescriptions(),"CalcShadowMode",str) && str.compare("NotEffectByShadow") == 0)
 //       effectByShadow = false;
	bool notUI = getTraverseString().compare("UIRender") != 0;
	ref_ptr<crDrawable> drawable;
    for(unsigned int i=0;i<numDrawables;++i)
    {
        drawable = node.getDrawable(i);
		if(!drawable.valid() || !drawable->getVisiable()) continue;
        const crBoundingBox &bb =drawable->getBound();

		if (node.isCullingActive() && isCulled(bb)) continue;

		if( drawable->getCullCallback() )
        {
            if( drawable->getCullCallback()->cull( this, drawable.get(), m_state.valid()?m_state.get():NULL ) == true )
            continue;
        }

        if (m_computeNearFar && bb.valid()) 
		{
			if (!updateCalculatedNearFar(matrix,*drawable,false)) continue;
		}

		///delete by wch 2007.3.02，
		//crCullingSet::StateFrustumList功能被停用
		//该功能可以采用节点多parent的方式代替
		//drawable的stateset放在addDrawable函数里push
		/*unsigned int numPopStateSetRequired = 0;
		
        // push the geoset's state on the geostate stack.    
        crStateSet* stateset = drawable->getStateSet();
        if (stateset) 
		{
			++numPopStateSetRequired;
			pushStateSet(stateset,drawablePopAlphaShadowFlg);
			drawablePopAlphaShadowFlgStack.push(drawablePopAlphaShadowFlg);
		}

		crCullingSet& cs = getCurrentCullingSet();
		if (!cs.getStateFrustumList().empty())
		{
			CRCore::crCullingSet::StateFrustumList& sfl = cs.getStateFrustumList();
			for(CRCore::crCullingSet::StateFrustumList::iterator itr = sfl.begin();
				itr != sfl.end();
				++itr)
			{
				if (itr->second.contains(bb))
				{
					++numPopStateSetRequired;
					pushStateSet(itr->first.get(),drawablePopAlphaShadowFlg);
					drawablePopAlphaShadowFlgStack.push(drawablePopAlphaShadowFlg);
				}
			}
		}*/
        if (notUI && bb.valid()) 
			addDrawableAndDepth(&node,drawable.get(),&matrix,distance(bb.center(),matrix),node.getEffectByShadow());
	    else
			addDrawableAndDepth(&node,drawable.get(),&matrix,0.0f,node.getEffectByShadow());

		/*for(unsigned int i=0;i< numPopStateSetRequired; ++i)
		{
			popStateSet(drawablePopAlphaShadowFlgStack.top());
			drawablePopAlphaShadowFlgStack.pop();
		}*/
    }

	//if(m_showBoundingBox)
	//{
	//	crDrawable *bboxDrawable = dynamic_cast<crDrawable *>(node.getBoundingBoxDrawable());
	//	crStateSet* ss = bboxDrawable->getStateSet();
 //       int boxPopAlphaShadowFlg;
	//	if (ss) pushStateSet(ss,boxPopAlphaShadowFlg);

 //       const crBoundingBox &bbox =bboxDrawable->getBound();

	//    if (bbox.valid()) addDrawableAndDepth(&node,bboxDrawable,&matrix,distance(bbox.center(),matrix),false);
	//    else addDrawableAndDepth(&node,bboxDrawable,&matrix,0.0f,false);

 //       if (ss) popStateSet(boxPopAlphaShadowFlg);

	//	if(node.isShowBoundingBox()) m_showBoundingBox = false;
	//}

    // pop the node's state off the geostate stack.    
    if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);
}

//void crCullVisitor::apply(crDepthSortedObject& node)
//{
//	if(m_renderMode == ShadowMapRender&&!node.isCalcShadow()) return;
//
//	//int frameNumber = getFrameStamp()->getFrameNumber();
//	//if(frameNumber <= 3 && m_renderMode != StaitcShadowMapRender) return;
//	//else if(frameNumber > 3 && m_renderMode != NormalRender) return;
//
//	if(!node.getVisiable()) return;
//	if (isCulled(node)) return;
//
//	//if(!m_showBoundingBox)
//	//	m_showBoundingBox = node.isShowBoundingBox();
//
//	// push the node's state.
//	crStateSet* node_state = node.getStateSet();
//	int popAlphaShadowFlg;
//	if (node_state) pushStateSet(node_state,popAlphaShadowFlg);
//
//	// traverse any call callbacks and traverse any children.
//	handle_cull_callbacks_and_traverse(node);
//
//    node.setEyeLocal(getEyeLocal());
//
//	RefMatrix& matrix = getModelViewMatrix();
//	int numDrawables = node.getNumDrawables();
//	int drawablePopAlphaShadowFlg;
//	std::stack<int> drawablePopAlphaShadowFlgStack;
//
//	//bool effectByShadow = true;
//	//std::string str;
//	//if(crArgumentParser::readKeyValue(node.getDescriptions(),"CalcShadowMode",str) && str.compare("NotEffectByShadow") == 0)
//	//	effectByShadow = false;
//
//	for(unsigned int i=0;i<numDrawables;++i)
//	{
//		crDrawable* drawable = node.getDrawable(i);
//		if(!drawable->getVisiable()) continue;
//		const crBoundingBox &bb =drawable->getBound();
//
//		if (node.isCullingActive() && isCulled(bb)) continue;
//
//		if( drawable->getCullCallback() )
//		{
//			if( drawable->getCullCallback()->cull( this, drawable, m_state.valid()?m_state.get():NULL ) == true )
//				continue;
//		}
//
//		if (m_computeNearFar && bb.valid()) 
//		{
//			if (!updateCalculatedNearFar(matrix,*drawable,false)) continue;
//		}
//
//		// need to track how push/pops there are, so we can unravel the stack correctly.
//		unsigned int numPopStateSetRequired = 0;
//
//		// push the geoset's state on the geostate stack.    
//		crStateSet* stateset = drawable->getStateSet();
//		if (stateset) 
//		{
//			++numPopStateSetRequired;
//			pushStateSet(stateset,drawablePopAlphaShadowFlg);
//			drawablePopAlphaShadowFlgStack.push(drawablePopAlphaShadowFlg);
//		}
//
//		crCullingSet& cs = getCurrentCullingSet();
//		if (!cs.getStateFrustumList().empty())
//		{
//			CRCore::crCullingSet::StateFrustumList& sfl = cs.getStateFrustumList();
//			for(CRCore::crCullingSet::StateFrustumList::iterator itr = sfl.begin();
//				itr != sfl.end();
//				++itr)
//			{
//				if (itr->second.contains(bb))
//				{
//					++numPopStateSetRequired;
//					pushStateSet(itr->first.get(),drawablePopAlphaShadowFlg);
//					drawablePopAlphaShadowFlgStack.push(drawablePopAlphaShadowFlg);
//				}
//			}
//		}
//
//		node.pushNeedSortIndex(i);
//		if (bb.valid()) addDrawableAndDepth(&node,drawable,&matrix,distance(bb.center(),matrix),node.getEffectByShadow());
//		else addDrawableAndDepth(&node,drawable,&matrix,0.0f,node.getEffectByShadow());
//
//		for(unsigned int i=0;i< numPopStateSetRequired; ++i)
//		{
//			popStateSet(drawablePopAlphaShadowFlgStack.top());
//			drawablePopAlphaShadowFlgStack.pop();
//		}
//	}
//
//	//if(m_showBoundingBox)
//	//{
//	//	crDrawable *bboxDrawable = dynamic_cast<crDrawable *>(node.getBoundingBoxDrawable());
//	//	crStateSet* ss = bboxDrawable->getStateSet();
//	//	int boxPopAlphaShadowFlg;
//	//	if (ss) pushStateSet(ss,boxPopAlphaShadowFlg);
//
//	//	const crBoundingBox &bbox =bboxDrawable->getBound();
//
//	//	if (bbox.valid()) addDrawableAndDepth(&node,bboxDrawable,&matrix,distance(bbox.center(),matrix),false);
//	//	else addDrawableAndDepth(&node,bboxDrawable,&matrix,0.0f,false);
//
//	//	if (ss) popStateSet(boxPopAlphaShadowFlg);
//
//	//	if(node.isShowBoundingBox()) m_showBoundingBox = false;
//	//}
//
//	// pop the node's state off the geostate stack.    
//	if (node_state) popStateSet(popAlphaShadowFlg);
//}
//
void crCullVisitor::apply(crBillboard& node)
{
	node.setNodeMask(node.getNodeMask() | CulledMask);
	if (!node.getVisiable() || m_renderMode == GIMapRender || m_renderMode == CollideMapRender) return;
	if(m_renderMode == ShadowMapRender&&!node.isCalcShadow()) return;

	int frameNumber = crFrameStamp::getInstance()->getFrameNumber();
	if(frameNumber <= 3 && m_renderMode != StaitcShadowMapRender) return;
	else if(frameNumber > 3 && m_renderMode != NormalRender) return;

	if (isCulled(node)) return;
	node.setNodeMask(node.getNodeMask() & ~CulledMask);
	//if(!m_dependCullingBufStack.empty())
	//{
	//	topDependCullingBuf()->addChild(&node);
	//}

	//if(!m_showBoundingBox)
	//	m_showBoundingBox = node.isShowBoundingBox();
  
	// push the node's state.
    crStateSet* node_state = node.getStateSet();
	//int popAlphaShadowFlg;
	bool stateset_pushed = false;
    if (node_state) stateset_pushed = pushStateSet(node_state/*,popAlphaShadowFlg*/);

    // traverse any call callbacks and traverse any children.
    handle_cull_callbacks_and_traverse(node);

	//node.lock();

    const crVector3& eye_local = getEyeLocal();
	////设置billboard的注视轴心为视点后方10米处
	//crVector3 eye_local1 = eye_local;
	//crVector3 lookVector = getLookVectorLocal();
 //   crVector3 dir = lookVector - eye_local;
	//dir.normalize();
	//eye_local1 -= dir * 1.0f;

    RefMatrix& modelview = getModelViewMatrix();
	//RefMatrix* parentMatrix = createOrReuseMatrix(modelview * getCameraViewInverseMatrix());
	//RefMatrix* billboard_matrix;
	int numDrawables = node.getNumDrawables();
	//node.clearIDepthVec();

	//bool effectByShadow = false;
	//std::string str;
	//if(crArgumentParser::readKeyValue(node.getDescriptionsInString(),"CalcShadowMode",str) && str.compare("NotEffectByShadow") == 0)
	//	effectByShadow = false;

    for(unsigned int i=0;i<numDrawables;++i)
    {
       // const crVector3& pos = node.getPos(i);

        //crDrawable* drawable = node.getDrawable(i);
        // need to modify isCulled to handle the billboard offset.
        // if (isCulled(drawable->getBound())) continue;

		//billboard_matrix = createOrReuseMatrix(modelview);
        //billboard_matrix->setTrans(eye_local1);
        node.computeMatrix(modelview,eye_local,i);
        
		//node.makeComposeDrawable(i,*billboard_matrix);

        //float d = distance(pos,modelview);
        //if (m_computeNearFar)
        //{
        //    if (d<m_computed_znear) m_computed_znear = d;
        //    if (d>m_computed_zfar) m_computed_zfar = d;
        //}
  //      
  //      crStateSet* stateset = drawable->getStateSet();
  //      if (stateset) pushStateSet(stateset);

		//addDrawableAndDepth(drawable,billboard_matrix,d);

  //      if (stateset) popStateSet();

    }

	// push the geoset's state on the geostate stack.    
    ref_ptr<crDrawable> bbDrawable = node.getComposeDrawable();
	if(bbDrawable.valid())
	{
		const crBoundingSphere &bs = node.getBound();//bbDrawable->getBound();
		//crMatrix mat;//单位阵
		//if (m_computeNearFar && bb.valid()) updateCalculatedNearFar(mat,*bbDrawable,true);
		//float d = distance(pos,modelview);
		//	if (m_computeNearFar && bb.valid()) updateCalculatedNearFar(mat,*bbDrawable);

		//node.setNeedSort(true);
		if (bs.valid())
		{///避免使用Billboard计算实时光
			addDrawableAndDepth(&node,bbDrawable.get(),0,distance(bs.center(),modelview),false/*,&node*/);
		}
		else 
		{
			addDrawableAndDepth(&node,bbDrawable.get(),0,0.0f,false/*,&node*/);
		}
	}
 //   if(m_showBoundingBox)
	//{
	//	crDrawable *bboxDrawable = dynamic_cast< crDrawable * >(node.getBoundingBoxDrawable());
	//	crStateSet* ss = bboxDrawable->getStateSet();

	//	int boxPopAlphaShadowFlg;
	//	if (ss) pushStateSet(ss,boxPopAlphaShadowFlg);

	//    const crBoundingBox &bbox =bboxDrawable->getBound();
	//    if (bbox.valid()) addDrawableAndDepth(&node,bboxDrawable,&modelview,distance(bbox.center(),modelview),false);
	//    else addDrawableAndDepth(&node,bboxDrawable,&modelview,0.0f,false);

 //       if (ss) popStateSet(boxPopAlphaShadowFlg);

	//	if(node.isShowBoundingBox()) m_showBoundingBox = false;
	//}

    // pop the node's state off the geostate stack.    
    if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);
}


void crCullVisitor::apply(crLightSource& node)
{
	if(m_renderMode >= ShadowMapRender) 
	{
		traverse(node);
		return;
	}
	//crStateSet* node_state = NULL;
	//bool stateset_pushed = false;

	if (node.getReferenceFrame()==CRCore::crLightSource::RELATIVE_TO_PARENTS)
	{
		node.setLightMatrix(getModelViewMatrix() * getCameraViewInverseMatrix());
	}
//		if(node.getEnable() && node.getStdModeEnable())
//		{
///*			if(node.getGlobal()) addPositionedAttribute(0,light);
//			else */if(!isCulled(node))
//			{
//				node_state = node.getStateSet();
//				if (node_state) stateset_pushed = pushStateSet(node_state/*,popAlphaShadowFlg*/);
//                //CRCore::notify(WARN)<<"crCullVisitor::apply(crLightSource& node) getStdModeEnable = "<<light<< std::endl;
//			}
//		}
 
    handle_cull_callbacks_and_traverse(node);

    //// pop the node's state off the geostate stack.
	//if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);
}

void crCullVisitor::apply(CRCore::crRenderToTextureManager& node)
{
	if(!node.getVisiable()) return;
	if(m_renderMode == ShadowMapRender&&!node.isCalcShadow()) return;

	// push the culling mode.
	pushCurrentMask();

	node.setCurrentProjectMatrix(getProjectionMatrix());
	node.setCurrentViewMatrix(getModelViewMatrix());
	//node.setCurrentHorizontalFov(m_hfov);
	//node.setCurrentVerticalFov(m_vfov);

	handle_cull_callbacks_and_traverse(node);

	// pop the culling mode.
	popCurrentMask();
}

void crCullVisitor::apply(CRCore::crTraverseStringNode& node)
{
	node.setNodeMask(node.getNodeMask() | CulledMask);
	if(!node.getVisiable() && m_renderMode != CollideMapRender) return;
	if(m_renderMode == ShadowMapRender&&!node.isCalcShadow()) return;
	if (m_renderMode == GIMapRender &&  !node.isGIMapRenderable()) return;
	//const std::string& name = node.getName();
    //if(m_renderMode > NormalRender && (name.compare("0x2")==0 || name.compare("0x3")==0)) return;
	if(isCulled(node)) return;
	node.setNodeMask(node.getNodeMask() & ~CulledMask);

	pushCurrentMask();
	
	crStateSet* node_state = node.getStateSet();

    /*if(m_renderMode == NormalRender && name.compare("0x0") == 0)
	{//室外，太阳光和太阳补光影响的渲染（补光不计算阴影）
		ref_ptr<crLightSource> ls,ls1;
        node.getGlobalLightSource(ls,ls1);
		crLight *light;
        crVector3 lightPos;
		float spotAttenuation;
		crUniform *uniform;
		if(ls.valid())
		{
			light = dynamic_cast<crLight *>(ls->getLight());
			const CRCore::crVector4& lightPosition = light->getPosition();
			lightPos.set(lightPosition[0],lightPosition[1],lightPosition[2]);
			uniform = node_state->getOrCreateUniform("lightPos",crUniform::FLOAT_VEC3);
		    uniform->set(lightPos);
			//CRCore::notify(CRCore::FATAL)<<"lightPos ="<<lightPos<<std::endl;

			const CRCore::crVector4& lightColor = light->getDiffuse();
			uniform = node_state->getOrCreateUniform("lightColor",crUniform::FLOAT_VEC3);
			uniform->set(crVector3(lightColor[0],lightColor[1],lightColor[2]));

			uniform = node_state->getOrCreateUniform("lightDir",crUniform::FLOAT_VEC3);
			uniform->set(light->getDirection());

			spotAttenuation = 0.0f;
			if(light->getLightType()==crLight::LIGHT_SPOT)
				spotAttenuation = light->getSpotExponent();

			uniform = node_state->getOrCreateUniform("lightAttenuation",crUniform::FLOAT_VEC4);
			uniform->set(crVector4(light->getConstantAttenuation(),light->getLinearAttenuation(),light->getQuadraticAttenuation(),spotAttenuation));

			if(ls->getCalcShadow())
			{
				uniform = node_state->getOrCreateUniform("lightMVP",crUniform::FLOAT_MAT4);
				uniform->set(ls->getLightMapMVP());
				node_state->setTextureAttributeAndModes(TEXTURE_SHADOWMAP,ls->getLightMap(),crStateAttribute::ON);

				uniform = node_state->getOrCreateUniform("cosLightsFov",crUniform::FLOAT);
				uniform->set(ls->getCosLightMapFov());

				uniform = node_state->getOrCreateUniform("scale",crUniform::FLOAT);
				uniform->set(ls->getLightRangeScale());

				uniform = node_state->getOrCreateUniform("calcRelTimeShadow",crUniform::BOOL);
				uniform->set(true);

				//CRCore::notify(CRCore::FATAL)<<"crTraverseStringNode: "<<std::endl;
			}
		}
		if(ls1.valid())
		{
			light = dynamic_cast<crLight *>(ls1->getLight());
			const CRCore::crVector4& lightPosition = light->getPosition();
			lightPos.set(lightPosition[0],lightPosition[1],lightPosition[2]);

			uniform = node_state->getOrCreateUniform("lightPos1",crUniform::FLOAT_VEC3);
			uniform->set(lightPos);
			//CRCore::notify(CRCore::FATAL)<<"lightPos1 ="<<lightPos<<std::endl;

			const CRCore::crVector4& lightColor = light->getDiffuse();
			uniform = node_state->getOrCreateUniform("lightColor1",crUniform::FLOAT_VEC3);
			uniform->set(crVector3(lightColor[0],lightColor[1],lightColor[2]));

			uniform = node_state->getOrCreateUniform("lightDir1",crUniform::FLOAT_VEC3);
			uniform->set(light->getDirection());

			spotAttenuation = 0.0f;
			if(light->getLightType()==crLight::LIGHT_SPOT)
				spotAttenuation = light->getSpotExponent();

			uniform = node_state->getOrCreateUniform("lightAttenuation1",crUniform::FLOAT_VEC4);
			uniform->set(crVector4(light->getConstantAttenuation(),light->getLinearAttenuation(),light->getQuadraticAttenuation(),spotAttenuation));
		}
	}
*/
	//int popAlphaShadowFlg;
	bool stateset_pushed = false;
	if (node_state) stateset_pushed = pushStateSet(node_state/*,popAlphaShadowFlg*/);
	
	handle_cull_callbacks_and_traverse(node);    
	
	if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);

	popCurrentMask();
}

//void crCullVisitor::apply(CRCore::crWaterSimulationNode& node)
//{
//	if(m_renderMode >= ShadowMapRender || !node.getVisiable() || isCulled(node)) 
//	{
//		node.setHasReflectMap(false);
//		return;
//	}
//	//if(!m_dependCullingBufStack.empty())
//	//{
//	//	topDependCullingBuf()->addChild(&node);
//	//}
//
//	//if(!m_showBoundingBox)
//	//	m_showBoundingBox = node.isShowBoundingBox();
//
//	// push the culling mode.
//	pushCurrentMask();
//
//	// push the node's state.
//	crStateSet* node_state = node.getStateSet();
//	int popAlphaShadowFlg;
//	if (node_state) pushStateSet(node_state,popAlphaShadowFlg);
//
//	node.setCurrentProjectMatrix(getProjectionMatrix());//未进行同步
//	node.setCurrentViewMatrix(getModelViewMatrix());//未进行同步
//	//node.setCurrentHorizontalFov(m_hfov);//未进行同步
//	//node.setCurrentVerticalFov(m_vfov);//未进行同步
//    
//	handle_cull_callbacks_and_traverse(node);
//
//	// pop the node's state off the render graph stack.    
//	if (node_state) popStateSet(popAlphaShadowFlg);
//
//	// pop the culling mode.
//	popCurrentMask();
//
//	//if(node.isShowBoundingBox()) m_showBoundingBox = false;
//}
//
//
void crCullVisitor::apply(crTexGenNode& node)
{
	if(!node.getVisiable()) return;
	if(m_renderMode == ShadowMapRender&&!node.isCalcShadow()) return;
	if (m_renderMode == GIMapRender &&  !node.isGIMapRenderable()) return;
	//if(!m_dependCullingBufStack.empty())
	//{
	//	topDependCullingBuf()->addChild(&node);
	//}

	//if(!m_showBoundingBox)
	//	m_showBoundingBox = node.isShowBoundingBox();

	// push the node's state.
	crStateSet* node_state = node.getStateSet();
	//int popAlphaShadowFlg;
	bool stateset_pushed = false;
	if (node_state) stateset_pushed = pushStateSet(node_state/*,popAlphaShadowFlg*/);

	if (node.getReferenceFrame()==CRCore::crTexGenNode::RELATIVE_RF)
	{
		RefMatrix& matrix = getModelViewMatrix();
		addPositionedTextureAttribute(node.getTextureUnit(), &matrix ,node.getTexGen());
	}
	else
	{
		addPositionedTextureAttribute(node.getTextureUnit(), 0 ,node.getTexGen());
	}

	handle_cull_callbacks_and_traverse(node);

	// pop the node's state off the geostate stack.    
	if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);

	//if(node.isShowBoundingBox()) m_showBoundingBox = false;

}

void crCullVisitor::apply(crClipNode& node)
{
	if(!node.getVisiable()) return;
	if(m_renderMode == ShadowMapRender&&!node.isCalcShadow()) return;
	if (m_renderMode == GIMapRender &&  !node.isGIMapRenderable()) return;
	//if(!m_dependCullingBufStack.empty())
	//{
	//	topDependCullingBuf()->addChild(&node);
	//}

	//if(!m_showBoundingBox)
	//	m_showBoundingBox = node.isShowBoundingBox();

	// push the node's state.
    crStateSet* node_state = node.getStateSet();
	//int popAlphaShadowFlg;
	bool stateset_pushed = false;
    if (node_state) stateset_pushed = pushStateSet(node_state/*,popAlphaShadowFlg*/);

    RefMatrix& matrix = getModelViewMatrix();

    const crClipNode::ClipPlaneList& planes = node.getClipPlaneList();
    for(crClipNode::ClipPlaneList::const_iterator itr=planes.begin();
        itr!=planes.end();
        ++itr)
    {
        addPositionedAttribute(&matrix,itr->get());
    }

    handle_cull_callbacks_and_traverse(node);

    // pop the node's state off the geostate stack.    
    if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);

    //if(node.isShowBoundingBox()) m_showBoundingBox = false;
}

void crCullVisitor::apply(crGroup& node)
{
	node.setNodeMask(node.getNodeMask() | CulledMask);
	if(!node.getVisiable() && m_renderMode != CollideMapRender) return;
	if(m_renderMode == ShadowMapRender&&!node.isCalcShadow()) return;
	if(m_renderMode == GIMapRender &&  !node.isGIMapRenderable()) return;

	if (isCulled(node)) return;
	node.setNodeMask(node.getNodeMask() & ~CulledMask);
	//if(!m_dependCullingBufStack.empty())
	//{
	//	topDependCullingBuf()->addChild(&node);
	//}

	//if(!m_showBoundingBox)
	//	m_showBoundingBox = node.isShowBoundingBox();

    // push the culling mode.
    pushCurrentMask();

    // push the node's state.
    crStateSet* node_state = node.getStateSet();
	//int popAlphaShadowFlg;
	bool stateset_pushed = false;
    if (node_state) stateset_pushed = pushStateSet(node_state/*,popAlphaShadowFlg*/);

    handle_cull_callbacks_and_traverse(node);

    // pop the node's state off the render graph stack.    
    if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);

    // pop the culling mode.
    popCurrentMask();

	//if(node.isShowBoundingBox()) m_showBoundingBox = false;
}

//void crCullVisitor::apply(CRPhysics::crMatterObject& node)
//{
//	if(m_renderMode == ShadowMapRender&&!node.isCalcShadow()) return;
//
//	if(!node.getVisiable()) return;
//	bool culled = isCulled(node);
//	node.setEnableIntersect(!culled);
//	if (culled) return;
//
//	pushCurrentMask();
//
//	// push the node's state.
//	crStateSet* node_state = node.getStateSet();
//	//int popAlphaShadowFlg;
//	bool stateset_pushed = false;
//	if (node_state) stateset_pushed = pushStateSet(node_state/*,popAlphaShadowFlg*/);
//
//	ref_ptr<RefMatrix> matrix = createOrReuseMatrix(getModelViewMatrix());
//	node.computeLocalToWorldMatrix(*matrix,this);
//	pushModelViewMatrix(matrix.get());
//
//	handle_cull_callbacks_and_traverse(node);
//
//	popModelViewMatrix();
//
//	// pop the node's state off the render graph stack.    
//	if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);
//
//	// pop the culling mode.
//	popCurrentMask();
//}
//
//void crCullVisitor::apply(CRPhysics::crViewMatterObject& node)
//{
//    apply((crMatrixTransform&) node);
//}
//
//void crCullVisitor::apply(CRPhysics::crBulletMatterObject& node)
//{
//	apply((crMatrixTransform&) node);
//}
//
void crCullVisitor::apply(CRPhysics::crWeaponMatterObject& node)
{
	/*apply((crMatrixTransform&) node);*/
	if(!node.getVisiable()) return;
	if(m_renderMode == ShadowMapRender&&!node.isCalcShadow()) return;
	if (m_renderMode == GIMapRender &&  !node.isGIMapRenderable()) return;

	//CRPhysics::crWeaponMatterObject::WeaponMatrixVec matrixVec = node.getWeaponMatrixVec();
	//if(matrixVec.empty())
		apply((crMatrixTransform&) node);
	//else
	//{
	//	for( CRPhysics::crWeaponMatterObject::WeaponMatrixVec::iterator itr = matrixVec.begin();
	//		itr != matrixVec.end();
	//		++itr )
	//	{
	//		node.setMatrix(itr->first);
	//		apply((crMatrixTransform&) node);
	//	}
	//}
}

void crCullVisitor::apply(CRCore::crSequence& node)
{
	//if(node.isEffectIdle()) return;
    apply((crGroup&) node);
}

void crCullVisitor::apply(CRCore::crMatrixSequence& node)
{
	node.setNodeMask(node.getNodeMask() | CulledMask);
	//CRCore::notify(CRCore::FATAL)<<"crCullVisitor::apply(CRCore::crMatrixSequence& node) "<<node.getName()<<std::endl;
	if(!node.getVisiable() && m_renderMode != CollideMapRender) return;
	if(/*node.isEffectIdle() || */(m_renderMode == ShadowMapRender&&!node.isCalcShadow())) return;
	if (m_renderMode == GIMapRender &&  !node.isGIMapRenderable()) return;

	if (isCulled(node)) return;
	node.setNodeMask(node.getNodeMask() & ~CulledMask);

	pushCurrentMask();

	crStateSet* node_state = node.getStateSet();
	bool stateset_pushed = false;
	if (node_state) stateset_pushed = pushStateSet(node_state/*,popAlphaShadowFlg*/);

	RefMatrix* matrix = createOrReuseMatrix(getModelViewMatrix());
	node.computeLocalToWorldMatrix(*matrix,this);
	pushModelViewMatrix(matrix);

	//CRCore::notify(CRCore::FATAL)<<"crCullVisitor::apply(CRCore::crMatrixSequence& node) "<<node.getName()<<std::endl;

	handle_cull_callbacks_and_traverse(node);

	popModelViewMatrix();

	if (stateset_pushed) popStateSet();

	popCurrentMask();
}

void crCullVisitor::apply(crTransform& node)
{
	node.setNodeMask(node.getNodeMask() | CulledMask);
	if(!node.getVisiable() && m_renderMode != CollideMapRender) return;
	if(m_renderMode == ShadowMapRender&&!node.isCalcShadow()) return;
	if (m_renderMode == GIMapRender &&  !node.isGIMapRenderable()) return;

	if (isCulled(node)) return;
	node.setNodeMask(node.getNodeMask() & ~CulledMask);
	//if(!m_dependCullingBufStack.empty())
	//{
	//	topDependCullingBuf()->addChild(&node);
	//}

	//if(!m_showBoundingBox)
	//	m_showBoundingBox = node.isShowBoundingBox();

	// push the culling mode.
    pushCurrentMask();

    // push the node's state.
    crStateSet* node_state = node.getStateSet();
	//int popAlphaShadowFlg;
	bool stateset_pushed = false;
    if (node_state) stateset_pushed = pushStateSet(node_state/*,popAlphaShadowFlg*/);

	//const crVector3& eye_local = getEyeLocal();
 //   CRCore::notify(CRCore::ALWAYS)<<"crCullVisitor::apply(crTransform& node) eye_local = "<<eye_local<<std::endl;

    RefMatrix* matrix = createOrReuseMatrix(getModelViewMatrix());
    node.computeLocalToWorldMatrix(*matrix,this);
    pushModelViewMatrix(matrix);

	//const crVector3& eye_local2 = getEyeLocal();
 //   CRCore::notify(CRCore::ALWAYS)<<"crCullVisitor::apply(crTransform& node) eye_local2 = "<<eye_local2<<std::endl;

    handle_cull_callbacks_and_traverse(node);

    popModelViewMatrix();

    // pop the node's state off the render graph stack.    
    if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);

    // pop the culling mode.
    popCurrentMask();

	//if(node.isShowBoundingBox()) m_showBoundingBox = false;
}

void crCullVisitor::apply(crProjection& node)
{
	node.setNodeMask(node.getNodeMask() | CulledMask);
	if(!node.getVisiable() && m_renderMode != CollideMapRender) return;
	if(m_renderMode == ShadowMapRender&&!node.isCalcShadow()) return;
	if (m_renderMode == GIMapRender &&  !node.isGIMapRenderable()) return;

	if (isCulled(node)) return;
	node.setNodeMask(node.getNodeMask() & ~CulledMask);
	//if(!m_dependCullingBufStack.empty())
	//{
	//	topDependCullingBuf()->addChild(&node);
	//}
  
	//if(!m_showBoundingBox)
	//	m_showBoundingBox = node.isShowBoundingBox();

    // push the culling mode.
    pushCurrentMask();

    // push the node's state.
    crStateSet* node_state = node.getStateSet();
	//int popAlphaShadowFlg;
	bool stateset_pushed = false;
    if (node_state) stateset_pushed = pushStateSet(node_state/*,popAlphaShadowFlg*/);


    // record previous near and far values.
    float previous_znear = m_computed_znear;
    float previous_zfar = m_computed_zfar;

	// take a copy of the current near plane candidates
	DistanceMatrixDrawableMap  previousNearPlaneCandidateMap;
	previousNearPlaneCandidateMap.swap(m_nearPlaneCandidateMap);

    m_computed_znear = FLT_MAX;
    m_computed_zfar = -FLT_MAX;

    CRCore::RefMatrix* matrix = createOrReuseMatrix(node.getMatrix());
    pushProjectionMatrix(matrix);
	
	if (!isCulled(node))
	{
		handle_cull_callbacks_and_traverse(node);
	}

    popProjectionMatrix();

    m_computed_znear = previous_znear;
    m_computed_zfar = previous_zfar;

	// swap back the near plane candidates
	previousNearPlaneCandidateMap.swap(m_nearPlaneCandidateMap);

    // pop the node's state off the render graph stack.    
    if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);

    // pop the culling mode.
    popCurrentMask();

	//if(node.isShowBoundingBox()) m_showBoundingBox = false;
}

void crCullVisitor::apply(crSwitch& node)
{
	//if(!node.getVisiable()) return;

	//if(m_renderMode == ShadowMapRender&&!node.isCalcShadow()) return;

    apply((crGroup&)node);
}


void crCullVisitor::apply(crLod& node)
{
	node.setNodeMask(node.getNodeMask() | CulledMask);
	if(!node.getVisiable() && m_renderMode != CollideMapRender) return;
	if(m_renderMode == ShadowMapRender&&!node.isCalcShadow()) return;
	if (m_renderMode == GIMapRender &&  !node.isGIMapRenderable()) return;

	if (isCulled(node)) return;
	node.setNodeMask(node.getNodeMask() & ~CulledMask);
	//if(!m_dependCullingBufStack.empty())
	//{
	//	topDependCullingBuf()->addChild(&node);
	//}
   
	//if(!m_showBoundingBox)
	//	m_showBoundingBox = node.isShowBoundingBox();

    // push the culling mode.
    pushCurrentMask();

    // push the node's state.
    crStateSet* node_state = node.getStateSet();
	//int popAlphaShadowFlg;
	bool stateset_pushed = false;
    if (node_state) stateset_pushed = pushStateSet(node_state/*,popAlphaShadowFlg*/);

    handle_cull_callbacks_and_traverse(node);

    // pop the node's state off the render graph stack.    
    if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);

    // pop the culling mode.
    popCurrentMask();

	//if(node.isShowBoundingBox()) m_showBoundingBox = false;
}

void crCullVisitor::apply(CRCore::crClearNode& node)
{
	if(!node.getVisiable() && m_renderMode != CollideMapRender) return;
	if(m_renderMode >= ShadowMapRender) return;

	//if(!m_showBoundingBox)
	//	m_showBoundingBox = node.isShowBoundingBox();

	// simply override the current earth sky.
    setClearNode(&node);

    // push the node's state.
    crStateSet* node_state = node.getStateSet();
	//int popAlphaShadowFlg;
	bool stateset_pushed = false;
    if (node_state) stateset_pushed = pushStateSet(node_state/*,popAlphaShadowFlg*/);

	//ref_ptr<RefMatrix> matrix = createOrReuseMatrix(getModelViewMatrix());
	//
	//crVector3 eyePointLocal = getEyeLocal();
	//matrix->preMult(crMatrix::translate(eyePointLocal));

	//pushModelViewMatrix(matrix.get());

    handle_cull_callbacks_and_traverse(node);

	//popModelViewMatrix();

    // pop the node's state off the render graph stack.    
    if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);

	//if(node.isShowBoundingBox()) m_showBoundingBox = false;

}

void crCullVisitor::apply(CRCore::crCameraNode& camera)
{
	//CRCore::notify(CRCore::NOTICE)<<"crCullVisitor::apply(CRCore::crCameraNode& camera) "<<camera.getName()<<std::endl;
	if(!camera.getShallRender()) return;
	if(!camera.getVisiable()) return;
	if(m_renderMode == ShadowMapRender&&!camera.isCalcShadow()) return;
	//if(getFrameStamp()->getFrameNumber()<=5)
	//	return;
	//if(camera.getName().compare("StaticShadowCamera") == 0 && getFrameStamp()->getFrameNumber() > 3)
	//{
	//	//CRCore::notify(CRCore::FATAL)<<"crCullVisitor::apply StaticShadowCamera"<<std::endl;
	//	camera.setShallRender(false);
	//}
	// set the compute near far mode.
	ComputeNearFarMode saved_compute_near_far_mode = getComputeNearFarMode();
	setComputeNearFarMode( camera.getComputeNearFarMode());

	RenderMode saved_renderMode = getRenderMode();
	setRenderMode( camera.getRenderMode() );
	// push the node's state.
	crStateSet* node_state = camera.getStateSet();
	//int popAlphaShadowFlg;
	bool stateset_pushed = false;
	if (node_state) stateset_pushed = pushStateSet(node_state/*,popAlphaShadowFlg*/);
	
	//pushShallRender(camera.getShallRender());

	CRCore::crMatrixd* projectionMatrix = camera.getProjectionMatrix();
	CRCore::crMatrixd* viewMatrix = camera.getViewMatrix();

	CRCore::RefMatrix& originalModelView = getModelViewMatrix();

	if (camera.getReferenceFrame()==CRCore::crTransform::RELATIVE_TO_ABSOLUTE)
	{
		pushProjectionMatrix(createOrReuseMatrix(*projectionMatrix));
		pushModelViewMatrix(createOrReuseMatrix(*viewMatrix));
        camera.computerViewInverseMatrix();
		pushCameraViewInverseMatrix(createOrReuseMatrix(*camera.getViewInverseMatrix()));
	}
	else if (camera.getTransformOrder()==CRCore::crCameraNode::POST_MULTIPLE)
	{
		pushProjectionMatrix(createOrReuseMatrix(getProjectionMatrix() * *projectionMatrix));
		pushModelViewMatrix(createOrReuseMatrix(getModelViewMatrix() * *viewMatrix));
	}
	else // pre multiple 
	{
		pushProjectionMatrix(createOrReuseMatrix(*projectionMatrix * getProjectionMatrix()));
		pushModelViewMatrix(createOrReuseMatrix(*viewMatrix * getModelViewMatrix()));
	}

	crCullingSet& cs = getCurrentCullingSet();
	cs.setCullingMask(camera.getCullingMode());
	if((camera.getCullingMode()&EXTERN_FRUSTUM_CULLING) && camera.hasExternFrustum())
	{
	    cs.setExternFrustumList(camera.getExternFrustumList());
	}

	if((camera.getCullingMode()&EXTERN_OCCLUSION_CULLING) && camera.hasExternOccluderFrustum())
	{
		cs.setExternOccluderFrustumList(camera.getExternOccluderFrustumList());
	}

	if (camera.getRenderOrder()==CRCore::crCameraNode::NESTED_RENDER)
	{
		handle_cull_callbacks_and_traverse(camera);
	}
	else
	{
		// set up lighting.
		// currently ignore lights in the scene graph itself..
		// will do later.
		CRUtil::crRenderStage* previous_stage = getCurrentRenderBin()->getStage();

		unsigned int contextID = m_state.valid() ? m_state->getContextID() : 0;

		// use render to texture stage.
		// create the render to texture stage.
		CRCore::ref_ptr<CRUtil::crRenderStage> rtts = dynamic_cast<CRUtil::crRenderStage*>(camera.getRenderingCache(contextID));
		if (!rtts.valid())
		{
			//CRCore::ScopedLock<CRCore::crMutex> lock(*(camera.getDataChangeMutex()));
			//rtts = new CRUtil::crOglRenderStage;/////////////////////ogl渲染
			rtts = new CRUtil::crRenderStage;/////////////////////ogl渲染
			rtts->setCameraNode(&camera);
			CRCore::ref_ptr<CRUtil::crRenderStage> rttsbuf = dynamic_cast<CRUtil::crRenderStage*>(camera.getBufRenderingCache(contextID));
			if(rttsbuf.valid())
			{
				//while(rttsbuf->getCameraRequiresSetUp())
				//{
					crThread::sleep(10);
				//}
				rtts->setFrameBufferObject(rttsbuf->getFrameBufferObject());
				rtts->setMultisampleResolveFramebufferObject(rttsbuf->getMultisampleResolveFramebufferObject());
				rtts->setCameraRequiresSetUp(false);
			}
			if (camera.getDrawBuffer() != GL_NONE)
			{
				rtts->setDrawBuffer(camera.getDrawBuffer());
			}
			else
			{
				// inherit draw buffer from above.
				rtts->setDrawBuffer(previous_stage->getDrawBuffer());
			}

			if (camera.getReadBuffer() != GL_NONE)
			{
				rtts->setReadBuffer(camera.getReadBuffer());
			}
			else
			{
				// inherit read buffer from above.
				rtts->setReadBuffer(previous_stage->getReadBuffer());
			}
			camera.setRenderingCache(contextID, rtts.get());
		}
		else
		{
			if(!rtts->getFrameBufferObject())
			{
				CRCore::ref_ptr<CRUtil::crRenderStage> rttsbuf = dynamic_cast<CRUtil::crRenderStage*>(camera.getBufRenderingCache(contextID));
				if(rttsbuf.valid())
				{
					rtts->setFrameBufferObject(rttsbuf->getFrameBufferObject());
					rtts->setMultisampleResolveFramebufferObject(rttsbuf->getMultisampleResolveFramebufferObject());
				}
			}
			// reusing render to texture stage, so need to reset it to empty it from previous frames contents.
			rtts->reset();
		}

		// set up the background color and clear mask.
		rtts->setClearDepth(camera.getClearDepth());
		rtts->setClearAccum(camera.getClearAccum());
		rtts->setClearStencil(camera.getClearStencil());
		rtts->setClearMask(camera.getClearMask());

		rtts->setClearColor(camera.getClearColor());

		// set the color mask.
		CRCore::crColorMask* colorMask = camera.getColorMask()!=0 ? camera.getColorMask() : previous_stage->getColorMask();
		rtts->setColorMask(colorMask);

		// set up the viewport.
		CRCore::crViewPort* viewport = camera.getViewport()!=0 ? camera.getViewport() : previous_stage->getViewport();
		rtts->setViewport( viewport );


		// set up to charge the same PositionalStateContainer is the parent previous stage.
		CRCore::crMatrix inhertiedMVtolocalMV;
		inhertiedMVtolocalMV.invert(originalModelView);
		inhertiedMVtolocalMV.postMult(getModelViewMatrix());
		rtts->setInheritedPositionalStateContainerMatrix(inhertiedMVtolocalMV);
		rtts->setInheritedPositionalStateContainer(previous_stage->getInheritedPositionalStateContainer());//getPositionalStateContainer()

		// record the render bin, to be restored after creation
		// of the render to text
		CRUtil::crRenderBin* previousRenderBin = getCurrentRenderBin();

		// set the current renderbin to be the newly created stage.
		setCurrentRenderBin(rtts.get());

		// set the cull traversal mask of camera node
		CRCore::crNode::NodeMask saved_mask = getCullMask();
		if (camera.getInheritanceMask() & CULL_MASK)
		{
			setTraversalMask(camera.getCullMask());
		}

		// traverse the subgraph
		{
			handle_cull_callbacks_and_traverse(camera);
		}

		// restore the cull traversal mask of camera node
		if (camera.getInheritanceMask() & CULL_MASK)
		{
			setTraversalMask(saved_mask);
		}

		// restore the previous renderbin.
		setCurrentRenderBin(previousRenderBin);


		//if (rtts->getRenderGraphList().size()==0 && rtts->getRenderBinList().size()==0)
		//{
		//	// getting to this point means that all the subgraph has been
		//	// culled by small feature culling or is beyond LOD ranges.
		//}

		// and the render to texture stage to the current stages
		// dependancy list.
		switch(camera.getRenderOrder())
		{
		case CRCore::crCameraNode::PRE_RENDER :
			getCurrentRenderBin()->getStage()->addPreRenderStage(rtts.get());
			break;
		default :
			getCurrentRenderBin()->getStage()->addPostRenderStage(rtts.get());
			break;
		}

	}

	if (camera.getReferenceFrame()==CRCore::crTransform::RELATIVE_TO_ABSOLUTE)
	{
		popCameraViewInverseMatrix();
	}

	// restore the previous model view matrix.
	popModelViewMatrix();

	// restore the previous model view matrix.
	popProjectionMatrix();

	// pop the node's state off the render graph stack.    
	if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);

	// restore the previous compute near far mode
	setComputeNearFarMode(saved_compute_near_far_mode);
	setRenderMode( saved_renderMode );
	//popShallRender();
}

void crCullVisitor::apply(CRCore::crOccluderNode& node)
{
	if (m_renderMode != NormalRender) return;

	//if(!m_showBoundingBox)
	//	m_showBoundingBox = node.isShowBoundingBox();

	// need to check if occlusion node is in the occluder
    // list, if so disable the appropriate ShadowOccluderVolume
    disableAndPushOccludersCurrentMask(m_nodePath);

    if (isCulled(node))
    {
        popOccludersCurrentMask(m_nodePath);
        return;
    }

    // push the culling mode.
    pushCurrentMask();

    // push the node's state.
    crStateSet* node_state = node.getStateSet();
	//int popAlphaShadowFlg;
	bool stateset_pushed = false;
    if (node_state) stateset_pushed = pushStateSet(node_state/*,popAlphaShadowFlg*/);

    handle_cull_callbacks_and_traverse(node);

    // pop the node's state off the render graph stack.    
    if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);

    // pop the culling mode.
    popCurrentMask();

    // pop the current mask for the disabled occluder
    popOccludersCurrentMask(m_nodePath);

	//if(node.isShowBoundingBox()) m_showBoundingBox = false;
}
void crCullVisitor::apply(CRParticle::crParticleEffect& node)
{
	if (m_renderMode == GIMapRender || m_renderMode == CollideMapRender) return;
	apply((crGroup&)node);
}
void crCullVisitor::apply(CRCore::crFilterRenderManager& node)
{
	if (m_renderMode != NormalRender) return;
	apply((crNode&)node);
}
//
void crCullVisitor::clearExternFrustum()
{
	crCullingSet& cs = getCurrentCullingSet();
	cs.clearExternFrustum();
}

void crCullVisitor::addExternFrustum(const CRCore::crPolytope& frustum)
{
	crCullingSet& cs = getCurrentCullingSet();
	cs.setCullingMask(cs.getCullingMask()|EXTERN_FRUSTUM_CULLING);
	cs.clearExternFrustum();
	cs.addExternFrustum(frustum);
}

void crCullVisitor::clearExternOccluderFrustum()
{
	crCullingSet& cs = getCurrentCullingSet();
	cs.clearExternOccluderFrustum();
}

void crCullVisitor::addExternOccluderFrustum(const CRCore::crPolytope& frustum)
{
	crCullingSet& cs = getCurrentCullingSet();
	cs.setCullingMask(cs.getCullingMask()|EXTERN_OCCLUSION_CULLING);
	cs.clearExternOccluderFrustum();
	cs.addExternOccluderFrustum(frustum);
}

/*void crCullVisitor::pushStateSet(const CRCore::crStateSet* ss, int &popAlphaShadowFlg)
{
	popAlphaShadowFlg = -1;
	if(m_renderMode>=ShadowMapRender)
	{
		if(ss->getIsShadowStateSet())
		{
			popAlphaShadowFlg = 1;
			m_computeAlphaShadowStack.push(1);
		}
		else
		{
			const CRCore::crUniform *uniform = ss->getUniform("computeAlphaShadow");
			if(uniform)
			{
				bool value;
				uniform->get(value);
				m_computeAlphaShadowStack.push(value);
				popAlphaShadowFlg = value ? 1 : 0;
			}
			if(!m_computeAlphaShadowStack.top()) return;

			if(!m_computeAlphaShadowStateSet.valid())
			{
				m_computeAlphaShadowStateSet = new crStateSet;
				m_computeAlphaShadowStateSet->setTextureAttributeAndModes(0,const_cast<crStateAttribute *>(ss->getTextureAttribute(0,crStateAttribute::TEXTURE)),crStateAttribute::ON);
				m_computeAlphaShadowStateSet->setAttributeAndModes(const_cast<crStateAttribute *>(ss->getAttribute(crStateAttribute::BLENDFUNC)),crStateAttribute::ON);
				m_computeAlphaShadowStateSet->addUniform(const_cast<crUniform *>(ss->getUniform("computeAlphaShadow")));
				m_computeAlphaShadowStateSet->setRenderingHint(ss->getRenderingHint());
			}
			ss = m_computeAlphaShadowStateSet.get();
		}
	}
	else 
	{
		if(ss->getIsShadowStateSet())
		{
			popAlphaShadowFlg = -2;
			return;
		}
	}
	m_currentRenderGraph = m_currentRenderGraph->find_or_insert(ss);
	if (ss->useRenderBinDetails())
	{
		m_currentRenderBin = m_currentRenderBin->find_or_insert(ss->getBinNumber(),ss->getBinName());
	}
}

void crCullVisitor::popStateSet( int needPopAlphaShadow )
{
	if(m_renderMode>=ShadowMapRender)
	{
		bool temp = m_computeAlphaShadowStack.top();
		if(needPopAlphaShadow >= 0)
		{
            m_computeAlphaShadowStack.pop();
		}
        if(!temp) return;
	}
	else
	{
		if(needPopAlphaShadow == -2)
			return;
	}
	if (m_currentRenderGraph->m_stateset->useRenderBinDetails())
	{
		m_currentRenderBin = m_currentRenderBin->m_parent;
	}
	m_currentRenderGraph = m_currentRenderGraph->m_parent;
}

*/
//CRCore::crStateSet *crCullVisitor::getOrCreateLightEffectStateSet(const CRCore::crObject *obj,const crBoundingBox &bbox, const CRCore::RefMatrix* matrix, bool effectByShadow)
//{
//	crMatrix mat = *matrix * getCameraViewInverseMatrix();
//	crBoundingBox box = bbox;
//	box.m_max = box.m_max * mat;
//	box.m_min = box.m_min * mat;
//
//	box.correct();
//
//	ref_ptr<crLightSource> ls1,ls2,ls3;
//	crLightSourceManager::getInstance()->lock();
//	crLightSourceManager::getInstance()->selectLightByBoundingBox(obj,box,ls1,ls2,ls3,effectByShadow);
//	crLightSourceManager::getInstance()->unlock();
//	if(!ls1.valid()) return NULL;
//	//bool hasSunLight = crLightSourceManager::getInstance()->getHasSunLight();
//	//ref_ptr<LS3UniformStateSet> temp = new LS3UniformStateSet(ls1.get(),ls2.get(),ls3.get(),effectByShadow);	
//	LS3UniformStateSet* newLs3UniformStateSet;
//
//	crStateSet *ss;
//	bool find = false;
//	bool needReSet = false;
//	for( LS3UniformStateSetVec::iterator itr = m_ls3UniformStateSetVec.begin();
//		itr != m_ls3UniformStateSetVec.end();
//		++itr )
//	{
//		if((*itr)->compare(ls1.get(),ls2.get(),ls3.get(),effectByShadow))
//		{
//			ss = (*itr)->m_stateset.get();
//			find = true;
//            needReSet = (*itr)->m_frameNumber != getFrameStamp()->getFrameNumber();
//            if(needReSet)
//			    (*itr)->m_frameNumber = getFrameStamp()->getFrameNumber();
//			break;
//		}
//	}
//    if(!find)
//	{
//		ss = new crStateSet;
//		newLs3UniformStateSet = new LS3UniformStateSet(ls1.get(),ls2.get(),ls3.get(),effectByShadow,getFrameStamp()->getFrameNumber());
//		newLs3UniformStateSet->m_stateset = ss;
//        m_ls3UniformStateSetVec.push_back(newLs3UniformStateSet);
//		needReSet = true;
//	}
//
//    if(needReSet)
//	{
//		crUniform *uniform;
//		//int useLightMapFlg = -1;
//		crLight *light;
//		crVector3 lightPos/*,lightDir*/;
//		crVector3i shadowMapChannels(-1,-1,-1);
//		//crVector2 cosLightsFov;
//		float spotAttenuation;
//		int lightNumber = 0;
//		//crMatrix lightMat;
//		if(ls1.valid())
//		{
//			//CRCore::notify(CRCore::FATAL)<<"getOrCreateLightEffectStateSet ls1 =  "<<ls1->getName()<<std::endl;
//
//			light = dynamic_cast<crLight *>(ls1->getLight());
//			const CRCore::crVector4& lightPosition = light->getPosition();
//			//lightMat = ls1->getLightMatrix();  
//			lightPos.set(lightPosition[0],lightPosition[1],lightPosition[2]);
//			//lightPos = lightPos * lightMat;
//
//			uniform = ss->getOrCreateUniform("lightPos",crUniform::FLOAT_VEC3);
//			uniform->set(lightPos);
//
//			const CRCore::crVector4& lightColor = light->getDiffuse();
//			uniform = ss->getOrCreateUniform("lightColor",crUniform::FLOAT_VEC3);
//			uniform->set(crVector3(lightColor[0],lightColor[1],lightColor[2]));
//
//			//lightMat.setTrans(0.0,0.0,0.0);
//			//lightDir = light->getDirection() * lightMat;
//			//lightDir.normalize();
//			uniform = ss->getOrCreateUniform("lightDir",crUniform::FLOAT_VEC3);
//			uniform->set(light->getDirection());
//
//	        spotAttenuation = 0.0f;
//			if(light->getLightType()==crLight::LIGHT_SPOT)
//				spotAttenuation = light->getSpotExponent();
// 
//			uniform = ss->getOrCreateUniform("lightAttenuation",crUniform::FLOAT_VEC4);
//			uniform->set(crVector4(light->getConstantAttenuation(),light->getLinearAttenuation(),light->getQuadraticAttenuation(),spotAttenuation));
//
//			shadowMapChannels[0] = ls1->getStaticShadowMapChannel();
//
//			if(effectByShadow && ls1->getCalcShadow())
//			{
//				uniform = ss->getOrCreateUniform("lightMVP",crUniform::FLOAT_MAT4);
//				uniform->set(ls1->getLightMapMVP());
//				ss->setTextureAttributeAndModes(TEXTURE_SHADOWMAP,ls1->getLightMap(),crStateAttribute::ON);
//
//				uniform = ss->getOrCreateUniform("cosLightsFov",crUniform::FLOAT);
//				uniform->set(ls1->getCosLightMapFov());
//				
//				uniform = ss->getOrCreateUniform("scale",crUniform::FLOAT);
//				uniform->set(ls1->getLightRangeScale());
//
//				uniform = ss->getOrCreateUniform("calcRelTimeShadow",crUniform::BOOL);
//				uniform->set(true);
//
//				//CRCore::notify(CRCore::ALWAYS)<<"calcRelTimeShadow="<<true<<" cosLightsFov="<<ls1->getCosLightMapFov()<< std::endl;
//			}
//
//			lightNumber++;
//		}
//		
//		if(ls2.valid())
//		{
//			light = dynamic_cast<crLight *>(ls2->getLight());
//			const CRCore::crVector4& lightPosition = light->getPosition();
//			//lightMat = ls2->getLightMatrix();  
//			lightPos.set(lightPosition[0],lightPosition[1],lightPosition[2]);
//			//lightPos = lightPos * lightMat;
//
//			uniform = ss->getOrCreateUniform("lightPos1",crUniform::FLOAT_VEC3);
//			uniform->set(lightPos);
//
//			const CRCore::crVector4& lightColor = light->getDiffuse();
//			uniform = ss->getOrCreateUniform("lightColor1",crUniform::FLOAT_VEC3);
//			uniform->set(crVector3(lightColor[0],lightColor[1],lightColor[2]));
//
//			//lightMat.setTrans(0.0,0.0,0.0);
//			//lightDir = light->getDirection() * lightMat;
//			//lightDir.normalize();
//			uniform = ss->getOrCreateUniform("lightDir1",crUniform::FLOAT_VEC3);
//			uniform->set(light->getDirection());
//
//			spotAttenuation = 0.0f;
//			if(light->getLightType()==crLight::LIGHT_SPOT)
//				spotAttenuation = light->getSpotExponent();
//
//			uniform = ss->getOrCreateUniform("lightAttenuation1",crUniform::FLOAT_VEC4);
//			uniform->set(crVector4(light->getConstantAttenuation(),light->getLinearAttenuation(),light->getQuadraticAttenuation(),spotAttenuation));
//
//			shadowMapChannels[1] = ls2->getStaticShadowMapChannel();
//
//			//if(!hasSunLight && effectByShadow && ls2->getCalcShadow())
//			//{
//			//	useLightMapFlg = 1;
//			//	uniform = ss->getOrCreateUniform("lightMapMVP",crUniform::FLOAT_MAT4);
//			//	uniform->set(ls2->getLightMapMVP());
//			//	ss->setTextureAttributeAndModes(TEXTURE_LIGHTMAP,ls2->getLightMap(),crStateAttribute::ON);
//
//			//	cosLightsFov[1] = cos(ls2->getLightMapFov());
//			//}
//
//			lightNumber++;
//		}
//
//		if(ls3.valid())
//		{
//			light = dynamic_cast<crLight *>(ls3->getLight());
//			const CRCore::crVector4& lightPosition = light->getPosition();
//			//lightMat = ls3->getLightMatrix();  
//			lightPos.set(lightPosition[0],lightPosition[1],lightPosition[2]);
//			//lightPos = lightPos * lightMat;
//
//			uniform = ss->getOrCreateUniform("lightPos2",crUniform::FLOAT_VEC3);
//			uniform->set(lightPos);
//
//			const CRCore::crVector4& lightColor = light->getDiffuse();
//			uniform = ss->getOrCreateUniform("lightColor2",crUniform::FLOAT_VEC3);
//			uniform->set(crVector3(lightColor[0],lightColor[1],lightColor[2]));
//
//			//lightMat.setTrans(0.0,0.0,0.0);
//			//lightDir = light->getDirection() * lightMat;
//			//lightDir.normalize();
//			uniform = ss->getOrCreateUniform("lightDir2",crUniform::FLOAT_VEC3);
//			uniform->set(light->getDirection());
//
//			spotAttenuation = 0.0f;
//			if(light->getLightType()==crLight::LIGHT_SPOT)
//				spotAttenuation = light->getSpotExponent();
//
//			uniform = ss->getOrCreateUniform("lightAttenuation2",crUniform::FLOAT_VEC4);
//			uniform->set(crVector4(light->getConstantAttenuation(),light->getLinearAttenuation(),light->getQuadraticAttenuation(),spotAttenuation));
//			
//			shadowMapChannels[2] = ls3->getStaticShadowMapChannel();
//
//			lightNumber++;
//		}
//
//		//CRCore::notify(CRCore::ALWAYS)<<" lightNumber="<<lightNumber<< std::endl;
//
//		uniform = ss->getOrCreateUniform("lightNumber",crUniform::INT);
//		uniform->set(lightNumber);
//		uniform = ss->getOrCreateUniform("shadowMapChannels",crUniform::INT_VEC3);
//		uniform->set(shadowMapChannels[0],shadowMapChannels[1],shadowMapChannels[2]);
//	}
//	return ss;
//}
//
//void crCullVisitor::createLightEffectStateSetVec(const CRCore::crObject *obj,const CRCore::crBoundingBox &bbox, const CRCore::RefMatrix* matrix,bool effectByShadow)
//{
//	crMatrix mat = *matrix * getCameraViewInverseMatrix();
//	crBoundingBox box = bbox;
//	box.m_max = box.m_max * mat;
//	box.m_min = box.m_min * mat;
//
//	box.correct();
//
//    crLightSourceManager::getInstance()->lock();
//	crLightSourceManager::SelectLightMap &selectLightMap = crLightSourceManager::getInstance()->selectLightByBoundingBox(obj,box);
//	//CRCore::notify(CRCore::FATAL)<<"createLightEffectStateSetVec size =  "<<selectLightMap.size()<<std::endl;
//	m_tempLsUniformStateSetVec.clear();
//	if(selectLightMap.empty()) 
//	{
//		crLightSourceManager::getInstance()->unlock();
//		return;
//	}
//	m_tempLsUniformStateSetVec.reserve(selectLightMap.size());
//
//	crStateSet* ss;
//	crUniform *uniform;
//    crLight *light;
//	crVector3 lightPos,lightDir;
//	float spotAttenuation;
//	int shadowMapChannel = -1;
//	//crMatrix lightMat;
//	bool find;
//	bool needReSet;
//    LSUniformStateSet* newLsUniformStateSet;
//	for( crLightSourceManager::SelectLightMap::iterator itr = selectLightMap.begin();
//		 itr != selectLightMap.end();
//		 ++itr )
//	{
//
//		find = false;
//		needReSet = false;
//		for( LSUniformStateSetVec::iterator luitr = m_lsUniformStateSetVec.begin();
//			luitr != m_lsUniformStateSetVec.end();
//			++luitr )
//		{
//			if((*luitr)->compare(itr->second,effectByShadow))
//			{
//				ss = (*luitr)->m_stateset.get();
//				find = true;
//				needReSet = (*luitr)->m_frameNumber != getFrameStamp()->getFrameNumber();
//				if(needReSet)
//					(*luitr)->m_frameNumber = getFrameStamp()->getFrameNumber();
//				break;
//			}
//		}
//		if(!find)
//		{
//			ss = new crStateSet;
//			newLsUniformStateSet = new LSUniformStateSet(itr->second,effectByShadow,getFrameStamp()->getFrameNumber());
//			newLsUniformStateSet->m_stateset = ss;
//			m_lsUniformStateSetVec.push_back(newLsUniformStateSet);
//			needReSet = true;
//		}
//
//        m_tempLsUniformStateSetVec.push_back(ss);
//        
//		if(needReSet)
//		{
//			light = dynamic_cast<crLight *>(itr->second->getLight());
//			const CRCore::crVector4& lightPosition = light->getPosition();
//			//lightMat = itr->second->getLightMatrix();  
//			lightPos.set(lightPosition[0],lightPosition[1],lightPosition[2]);
//			//lightPos = lightPos * lightMat;
//
//			uniform = ss->getOrCreateUniform("lightPos",crUniform::FLOAT_VEC3);
//			uniform->set(lightPos);
//
//			const CRCore::crVector4& lightColor = light->getDiffuse();
//			uniform = ss->getOrCreateUniform("lightColor",crUniform::FLOAT_VEC3);
//			uniform->set(crVector3(lightColor[0],lightColor[1],lightColor[2]));
//
//			//lightMat.setTrans(0.0,0.0,0.0);
//			//lightDir = light->getDirection() * lightMat;
//			//lightDir.normalize();
//			uniform = ss->getOrCreateUniform("lightDir",crUniform::FLOAT_VEC3);
//			uniform->set(light->getDirection());
//
//			spotAttenuation = 0.0f;
//			if(light->getLightType()==crLight::LIGHT_SPOT)
//				spotAttenuation = light->getSpotExponent();
//
//			uniform = ss->getOrCreateUniform("lightAttenuation",crUniform::FLOAT_VEC4);
//			uniform->set(crVector4(light->getConstantAttenuation(),light->getLinearAttenuation(),light->getQuadraticAttenuation(),spotAttenuation));
//
//			shadowMapChannel = itr->second->getStaticShadowMapChannel();
//			uniform = ss->getOrCreateUniform("shadowMapChannel",crUniform::INT);
//			uniform->set(shadowMapChannel);
//
//			if(effectByShadow)
//			{
//				if(itr->second->getCalcShadow())
//				{
//					uniform = ss->getOrCreateUniform("lightMVP",crUniform::FLOAT_MAT4);
//					uniform->set(itr->second->getLightMapMVP());
//
//					ss->setTextureAttributeAndModes(TEXTURE_SHADOWMAP,itr->second->getLightMap(),crStateAttribute::ON);
//
//					uniform = ss->getOrCreateUniform("cosLightsFov",crUniform::FLOAT);
//					uniform->set(itr->second->getCosLightMapFov());
//
//					uniform = ss->getOrCreateUniform("scale",crUniform::FLOAT);
//					uniform->set(itr->second->getLightRangeScale());
//
//					uniform = ss->getOrCreateUniform("calcRelTimeShadow",crUniform::BOOL);
//					uniform->set(true);
//				}
//				else
//				{
//					uniform = ss->getOrCreateUniform("calcRelTimeShadow",crUniform::BOOL);
//					uniform->set(false);
//				}
//			}
//		}
//	}
//	crLightSourceManager::getInstance()->unlock();
//}
///////////////////////////////////////////
////old
/////////////////
//CRCore::crStateSet * crCullVisitor::getOrCreateStdLightEffectStateSet(const CRCore::crObject *obj,const CRCore::crBoundingBox &bbox, const CRCore::RefMatrix* matrix,bool effectByShadow)
//{
//	crMatrix mat = *matrix * getCameraViewInverseMatrix();
//	crBoundingBox box = bbox;
//	box.m_max = box.m_max * mat;
//	box.m_min = box.m_min * mat;
//
//	box.correct();
//
//	crLightSourceManager::getInstance()->lock();
//	crLightSourceManager::SelectLightMap &selectLightMap = crLightSourceManager::getInstance()->selectLightByBoundingBox(obj,box);
//	if(selectLightMap.empty()) 
//	{
//		crLightSourceManager::getInstance()->unlock();
//		return NULL;
//	}
//
//	ref_ptr<crStateSet> ss = new crStateSet;
//	ref_ptr<crLight> light;
//	int i = 0;
//	//crMatrix lightMat;
//	for( crLightSourceManager::SelectLightMap::iterator itr = selectLightMap.begin();
//		itr != selectLightMap.end();
//		++itr )
//	{
//        if(i<8 && itr->second->getStdModeEnable())
//		{
//            light = dynamic_cast<crLight *>(itr->second->getLight()->clone(CRCore::crCopyOp::SHALLOW_COPY));
//			light->setDrawCallback(itr->second->getLight()->getDrawCallback());
//			light->setLightNum(i);
//			ss->setAttribute(light.get(),crStateAttribute::ON);
//			ss->setMode(GL_LIGHT0 + i,crStateAttribute::ON);
//			//lightMat = itr->second->getLightMatrix();
//			//crVector4 &position = light->getPosition();
//			//position = position * lightMat;
//			//lightMat.setTrans(0.0f,0.0f,0.0f);
//			//crVector3 &direction = light->getDirection();
//			//direction = direction * lightMat;
//			i++;
//		}
//		else if(i >= 8) break;
//	}
//	crLightSourceManager::getInstance()->unlock();
//
//	//crBrain::StateSetSet::iterator ssitr = m_stdLightEffectStateSetSet.find(ss.get());
//	//if(ssitr != m_stdLightEffectStateSetSet.end()) ss = ssitr->get();
//	//else m_stdLightEffectStateSetSet.insert(ss.get());
//    m_stdLightEffectStateSetSet.push_back(ss.get());
//	return ss.get();
//}
//
//bool crCullVisitor::specialCulled(const CRCore::crBoundingBox& bb)
//{
//	int cullmask = m_specialCullingPlane.intersect(bb);
//	if(cullmask > 0) return true;
//	return false;
//}
//bool crCullVisitor::specialCulled(const CRCore::crBoundingSphere& bs)
//{   
//	int cullmask = m_specialCullingPlane.intersect(bs);
//	if(cullmask > 0) return true;
//	return false;
//}


//void crCullVisitor::apply(crImpostor& node)
//{
//	  if(m_renderMode >= ShadowMapRender) return;

//    if (isCulled(node)) return;
//
//    CRCore::crVector3 eyeLocal = getEyeLocal();
//
//    // push the culling mode.
//    pushCurrentMask();
//
//    // push the node's state.
//    crStateSet* node_state = node.getStateSet();
//    if (node_state) pushStateSet(node_state);
//
//    const BoundingSphere& bs = node.getBound();
//    
//    unsigned int contextID = 0;
//    if (m_state.valid()) contextID = m_state->getContextID();
//
//    float distance2 = (eyeLocal-bs.center()).length2();
//    if (!m_impostorActive ||
//        distance2*_LODScale*_LODScale<node.getImpostorThreshold2() ||
//        distance2<bs.radius2()*2.0f)
//    {
//        // outwith the impostor distance threshold therefore simple
//        // traverse the appropriate child of the crLOD.
//        handle_cull_callbacks_and_traverse(node);
//    }
//    else if (_viewportStack.empty())
//    {
//        // need to use impostor but no valid viewport is defined to simply
//        // default to using the crLOD child as above.
//        handle_cull_callbacks_and_traverse(node);
//    }
//    else
//    {    
//
//        // within the impostor distance threshold therefore attempt
//        // to use impostor instead.
//        
//        RefMatrix& matrix = getModelViewMatrix();
//
//        // search for the best fit ImpostorSprite;
//        ImpostorSprite* impostorSprite = node.findBestImpostorSprite(contextID,eyeLocal);
//        
//        if (impostorSprite)
//        {
//            // impostor found, now check to see if it is good enough to use
//            float error = impostorSprite->calcPixelError(getMVPW());
//
//            if (error>m_impostorPixelErrorThreshold)
//            {
//                // chosen impostor sprite pixel error is too great to use
//                // from this eye point, therefore invalidate it.
//                impostorSprite=NULL;
//            }
//        }
//        
//
//// need to think about sprite reuse and support for multiple context's.
//
//        if (impostorSprite==NULL)
//        {
//            // no appropriate sprite has been found therefore need to create
//            // one for use.
//            
//            // create the impostor sprite.
//            impostorSprite = createImpostorSprite(node);
//
//            //if (impostorSprite) impostorSprite->_color.set(0.0f,0.0f,1.0f,1.0f);
//
//        }
//        //else impostorSprite->_color.set(1.0f,1.0f,1.0f,1.0f);
//        
//        if (impostorSprite)
//        {
//            
//            // update frame number to show that impostor is in action.
//            impostorSprite->setLastFrameUsed(getTraversalNumber());
//
//            if (m_computeNearFar) updateCalculatedNearFar(matrix,*impostorSprite);
//
//            crStateSet* stateset = impostorSprite->getStateSet();
//            
//            if (stateset) pushStateSet(stateset);
//            
//            addDrawableAndDepth(impostorSprite,&matrix,distance(node.getCenter(),matrix));
//
//            if (stateset) popStateSet();
//            
//            
//        }
//        else
//        {
//           // no impostor has been selected or created so default to 
//           // traversing the usual crLOD selected child.
//            handle_cull_callbacks_and_traverse(node);
//        }
//                
//    }
//
//    // pop the node's state off the render graph stack.    
//    if (node_state) popStateSet();
//
//    // pop the culling mode.
//    popCurrentMask();
//}
//
//ImpostorSprite* crCullVisitor::createImpostorSprite(crImpostor& node)
//{
//   
//    unsigned int contextID = 0;
//    if (m_state.valid()) contextID = m_state->getContextID();
//
//    // default to true right now, will dertermine if perspective from the
//    // projection matrix...
//    bool isPerspectiveProjection = true;
//
//    const crMatrix& matrix = getModelViewMatrix();
//    const BoundingSphere& bs = node.getBound();
//    CRCore::crVector3 eye_local = getEyeLocal();
//
//    if (!bs.valid())
//    {
//        CRCore::notify(CRCore::WARN) << "bb invalid"<<&node<<std::endl;
//        return NULL;
//    }
//
//
//    crVector3 eye_world(0.0,0.0,0.0);
//    crVector3 center_world = bs.center()*matrix;
//
//    // no appropriate sprite has been found therefore need to create
//    // one for use.
//
//    // create the render to texture stage.
//    ref_ptr<crRenderToTextureStage> rtts = new crRenderToTextureStage;
//
//    // set up lighting.
//    // currently ignore lights in the scene graph itself..
//    // will do later.
//    crRenderStage* previous_stage = m_currentRenderBin->_stage;
//
//    // set up the background color and clear mask.
//    CRCore::crVector4 clear_color = previous_stage->getClearColor();
//    clear_color[3] = 0.0f; // set the alpha to zero.
//    rtts->setClearColor(clear_color);
//    rtts->setClearMask(previous_stage->getClearMask());
//
//    // set up to charge the same RenderStageLighting is the parent previous stage.
//    rtts->setRenderStageLighting(previous_stage->getRenderStageLighting());
//
//
//    // record the render bin, to be restored after creation
//    // of the render to text
//    crRenderBin* previousRenderBin = m_currentRenderBin;
//
//    // set the current renderbin to be the newly created stage.
//    m_currentRenderBin = rtts.get();
//
//    // create quad coords (in local coords)
//
//    crVector3 center_local = bs.center();
//    crVector3 camera_up_local = getUpLocal();
//    crVector3 lv_local = center_local-eye_local;
//
//    float distance_local = lv_local.length();
//    lv_local /= distance_local;
//   
//    crVector3 sv_local = lv_local^camera_up_local;
//    sv_local.normalize();
//    
//    crVector3 up_local = sv_local^lv_local;
//    
//
//    
//    float width = bs.radius();
//    if (isPerspectiveProjection)
//    {
//        // expand the width to account for projection onto sprite.
//        width *= (distance_local/sqrtf(distance_local*distance_local-bs.radius2()));
//    }
//    
//    // scale up and side vectors to sprite width.
//    up_local *= width;
//    sv_local *= width;
//    
//    // create the corners of the sprite.
//    crVector3 c00(center_local - sv_local - up_local);
//    crVector3 c10(center_local + sv_local - up_local);
//    crVector3 c01(center_local - sv_local + up_local);
//    crVector3 c11(center_local + sv_local + up_local);
//    
//// adjust camera left,right,up,down to fit (in world coords)
//
//    crVector3 near_local  ( center_local-lv_local*width );
//    crVector3 far_local   ( center_local+lv_local*width );
//    crVector3 top_local   ( center_local+up_local);
//    crVector3 right_local ( center_local+sv_local);
//    
//    crVector3 near_world = near_local * matrix;
//    crVector3 far_world = far_local * matrix;
//    crVector3 top_world = top_local * matrix;
//    crVector3 right_world = right_local * matrix;
//    
//    float znear = (near_world-eye_world).length();
//    float zfar  = (far_world-eye_world).length();
//        
//    float top   = (top_world-center_world).length();
//    float right = (right_world-center_world).length();
//
//    znear *= 0.9f;
//    zfar *= 1.1f;
//
//    // set up projection.
//    CRCore::RefMatrix* projection = new CRCore::RefMatrix;
//    if (isPerspectiveProjection)
//    {
//        // deal with projection issue move the top and right points
//        // onto the near plane.
//        float ratio = znear/(center_world-eye_world).length();
//        top *= ratio;
//        right *= ratio;
//        projection->makeFrustum(-right,right,-top,top,znear,zfar);
//    }
//    else
//    {
//        projection->makeOrtho(-right,right,-top,top,znear,zfar);
//    }
//
//    pushProjectionMatrix(projection);
//
//    crVector3 rotate_from = bs.center()-eye_local;
//    crVector3 rotate_to   = getLookVectorLocal();
//
//    CRCore::RefMatrix* rotate_matrix = new CRCore::RefMatrix(
//        CRCore::crMatrix::translate(-eye_local)*        
//        CRCore::crMatrix::rotate(rotate_from,rotate_to)*
//        CRCore::crMatrix::translate(eye_local)*
//        getModelViewMatrix());
//
//    // pushing the cull view state will update it so it takes
//    // into account the new camera orientation.
//    pushModelViewMatrix(rotate_matrix);
//
//    crStateSet* localPreRenderState = m_impostorSpriteManager->createOrReuseStateSet();
//
//    pushStateSet(localPreRenderState);
//
//    {
//
//        // traversing the usual crLOD selected child.
//        handle_cull_callbacks_and_traverse(node);
//
//    }
//
//    popStateSet();
//
//    // restore the previous model view matrix.
//    popModelViewMatrix();
//
//    // restore the previous model view matrix.
//    popProjectionMatrix();
//
//    // restore the previous renderbin.
//    m_currentRenderBin = previousRenderBin;
//
//
//
//    if (rtts->_renderGraphList.size()==0 && rtts->_bins.size()==0)
//    {
//        // getting to this point means that all the subgraph has been
//        // culled by small feature culling or is beyond crLOD ranges.
//        return NULL;
//    }
//
//
//
//
//    const CRCore::crViewPort& viewport = *getViewport();
//    
//
//    // calc texture size for eye, bs.
//
//    // convert the corners of the sprite (in world coords) into their
//    // equivilant window coordinates by using the camera's project method.
//    const CRCore::crMatrix& MVPW = getMVPW();
//    crVector3 c00_win = c00 * MVPW;
//    crVector3 c11_win = c11 * MVPW;
//
//// adjust texture size to be nearest power of 2.
//
//    float s  = c11_win.x()-c00_win.x();
//    float t  = c11_win.y()-c00_win.y();
//
//    // may need to reverse sign of width or height if a matrix has
//    // been applied which flips the orientation of this subgraph.
//    if (s<0.0f) s = -s;
//    if (t<0.0f) t = -t;
//
//    // bias value used to assist the rounding up or down of
//    // the texture dimensions to the nearest power of two.
//    // bias near 0.0 will almost always round down.
//    // bias near 1.0 will almost always round up. 
//    float bias = 0.7f;
//
//    float sp2 = logf((float)s)/logf(2.0f);
//    float rounded_sp2 = floorf(sp2+bias);
//    int new_s = (int)(powf(2.0f,rounded_sp2));
//
//    float tp2 = logf((float)t)/logf(2.0f);
//    float rounded_tp2 = floorf(tp2+bias);
//    int new_t = (int)(powf(2.0f,rounded_tp2));
//
//    // if dimension is bigger than window divide it down.    
//    while (new_s>viewport.width()) new_s /= 2;
//
//    // if dimension is bigger than window divide it down.    
//    while (new_t>viewport.height()) new_t /= 2;
//
//
//    // offset the impostor viewport from the center of the main window
//    // viewport as often the edges of the viewport might be obscured by
//    // other windows, which can cause image/reading writing problems.
//    int center_x = viewport.x()+viewport.width()/2;
//    int center_y = viewport.y()+viewport.height()/2;
//
//    crViewPort* new_viewport = new crViewPort;
//    new_viewport->setViewport(center_x-new_s/2,center_y-new_t/2,new_s,new_t);
//    rtts->setViewport(new_viewport);
//    
//    localPreRenderState->setAttribute(new_viewport);
//
//    // create the impostor sprite.
//    ImpostorSprite* impostorSprite = 
//        m_impostorSpriteManager->createOrReuseImpostorSprite(new_s,new_t,getTraversalNumber()-m_numFramesToKeepImpostorSprites);
//
//    if (impostorSprite==NULL)
//    {
//        CRCore::notify(CRCore::WARN)<<"Warning: unable to create required impostor sprite."<<std::endl;
//        return NULL;
//    }
//
//    // update frame number to show that impostor is in action.
//    impostorSprite->setLastFrameUsed(getTraversalNumber());
//
//
//    // have successfully created an impostor sprite so now need to
//    // add it into the impostor.
//    node.addImpostorSprite(contextID,impostorSprite);
//
//    if (m_depthSortImpostorSprites)
//    {
//        // the depth sort bin should probably be user definable,
//        // will look into this later. RO July 2001.
//        crStateSet* stateset = impostorSprite->getStateSet();
//        stateset->setRenderBinDetails(10,"DepthSortedBin");
//    }
//    
//    crTexture2D* texture = impostorSprite->getTexture();
//
//    // update frame number to show that impostor is in action.
//    impostorSprite->setLastFrameUsed(getTraversalNumber());
//
//    crVector3* coords = impostorSprite->getCoords();
//    Vec2* texcoords = impostorSprite->getTexCoords();
//    
//    coords[0] = c01;
//    texcoords[0].set(0.0f,1.0f);
//
//    coords[1] = c00;
//    texcoords[1].set(0.0f,0.0f);
//
//    coords[2] = c10;
//    texcoords[2].set(1.0f,0.0f);
//
//    coords[3] = c11;
//    texcoords[3].set(1.0f,1.0f);
//    
//    impostorSprite->dirtyBound();
//    
//    crVector3* controlcoords = impostorSprite->getControlCoords();
//    
//    if (isPerspectiveProjection)
//    {
//        // deal with projection issue by moving the coorners of the quad
//        // towards the eye point.
//        float ratio = width/(center_local-eye_local).length();
//        float one_minus_ratio = 1.0f-ratio;
//        crVector3 eye_local_ratio = eye_local*ratio;
//        
//        controlcoords[0] = coords[0]*one_minus_ratio + eye_local_ratio;
//        controlcoords[1] = coords[1]*one_minus_ratio + eye_local_ratio;
//        controlcoords[2] = coords[2]*one_minus_ratio + eye_local_ratio;
//        controlcoords[3] = coords[3]*one_minus_ratio + eye_local_ratio;
//    }
//    else
//    {
//        // project the control points forward towards the eyepoint,
//        // but since this an othographics projection this projection is
//        // parallel.
//        crVector3 dv = lv_local*width;
//
//        controlcoords[0] = coords[0]-dv;
//        controlcoords[1] = coords[1]-dv;
//        controlcoords[2] = coords[2]-dv;
//        controlcoords[3] = coords[3]-dv;
//    }
//
//    impostorSprite->setStoredLocalEyePoint(eye_local);
//
//
//    // and the render to texture stage to the current stages
//    // dependancy list.
//    m_currentRenderBin->_stage->addToDependencyList(rtts.get());
//
//    // attach texture to the crRenderToTextureStage.
//    rtts->setTexture(texture);
//
//    // must sort the crRenderToTextureStage so that all leaves are
//    // accounted correctly in all renderbins i.e depth sorted bins.    
//    rtts->sort();
//
//    return impostorSprite;
//
//}
