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
#ifndef CRUTIL_CRCULLVISITOR_H
#define CRUTIL_CRCULLVISITOR_H 1

#include <CRUtil/crExport.h>
#include <CRCore/crNodeVisitor.h>
#include <CRCore/crBoundingSphere.h>
#include <CRCore/crBoundingBox.h>
#include <CRCore/crMatrix.h>
#include <CRCore/crDrawable.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crState.h>
//#include <CRCore/crImpostor.h>
#include <CRCore/crClearNode.h>
#include <CRCore/crCameraNode.h>
#include <CRCore/crRenderToTextureManager.h>
#include <CRCore/crTraverseStringNode.h>
//#include <CRCore/crFilterRenderManager.h>
#include <CRCore/crNotify.h>
#include <CRCore/crSwitch.h>
#include <CRCore/crSequence.h>
#include <CRCore/crDB.h>
#include <CRCore/crEventGroup.h>
#include <CRCore/crMatrixTransform.h>
#include <CRCore/crCullStack.h>
#include <CRCore/crShaderManager.h>
#include <CRCore/crBlendFunc.h>
#include <CRCore/crDepth.h>
#include <CRCore/crStreamBuf.h>

//#include <CRCore/crWaterSimulationNode.h>
#include <CRUtil/crRenderGraph.h>
#include <CRUtil/crRenderStage.h>
#include <CRPhysics\crMatterObject.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRPhysics/crWeaponNode.h>
//#include <CRPhysics\crMatterGroup.h>

#include <map>
#include <vector>

#include <CRCore/crVector3.h>

namespace CRUtil {

/**
 * Basic crNodeVisitor implementation for rendering a scene.
 * This visitor traverses the scene graph, collecting transparent and
 * opaque CRCore::Drawables into a depth sorted transparent bin and a state
 * sorted opaque bin.  The opaque bin is rendered first, and then the
 * transparent bin in rendered in order from the furthest CRCore::crDrawable
 * from the eye to the one nearest the eye. 
 */
class CRUTIL_EXPORT crCullVisitor : public CRCore::crNodeVisitor, public CRCore::crCullStack
{
    public:
    
        typedef CRCore::crMatrix::value_type value_type;
    

        crCullVisitor();
        virtual ~crCullVisitor();

        virtual crCullVisitor* cloneType() const { return new crCullVisitor(); }

        virtual void reset();
        
        inline virtual CRCore::crVector3 getEyePoint() const { return getEyeLocal(); }
        virtual float getDistanceToEyePoint(const CRCore::crVector3& pos, bool withLODScale) const;
        virtual float getDistanceFromEyePoint(const CRCore::crVector3& pos, bool withLODScale) const;

        virtual void apply(CRCore::crNode&);
		virtual void apply(CRCore::crDB& node);
        virtual void apply(CRCore::crObject& node);
        virtual void apply(CRCore::crBillboard& node);
		//virtual void apply(CRCore::crDepthSortedObject& node);
        virtual void apply(CRCore::crLightSource& node);
        virtual void apply(CRCore::crClipNode& node);
		virtual void apply(CRCore::crTexGenNode& node);

        virtual void apply(CRCore::crGroup& node);
        virtual void apply(CRCore::crTransform& node);
		virtual void apply(CRCore::crSequence& node);
		virtual void apply(CRCore::crMatrixSequence& node);
        virtual void apply(CRCore::crProjection& node);
        virtual void apply(CRCore::crSwitch& node);
        virtual void apply(CRCore::crLod& node);
        virtual void apply(CRCore::crClearNode& node);
		virtual void apply(CRCore::crCameraNode& node);
        virtual void apply(CRCore::crOccluderNode& node);
		//virtual void apply(CRCore::crWaterSimulationNode& node);
		virtual void apply(CRCore::crRenderToTextureManager& node);
		virtual void apply(CRCore::crTraverseStringNode& node);

		//virtual void apply(CRPhysics::crMatterGroup& node);
		
		//virtual void apply(CRPhysics::crMatterObject& node);
		//virtual void apply(CRPhysics::crViewMatterObject& node);
		//virtual void apply(CRPhysics::crBulletMatterObject& node);
		virtual void apply(CRPhysics::crWeaponMatterObject& node);


	 //   virtual void apply(CRPhysics::crCaissonMatterObject& node);
		//virtual void apply(CRPhysics::crHpMatterObject& node);
		//virtual void apply(CRPhysics::crArmorMatterObject& node);

		//virtual void apply(CRCore::crFilterRenderManager& node);

  //      virtual void apply(CRCore::crImpostor& node);
	//	virtual void apply(CRCore::crEventGroup& node);

        inline void setClearNode(const CRCore::crClearNode* earthSky) { m_clearNode = earthSky; }
        inline const CRCore::crClearNode* getClearNode() const { return m_clearNode.get(); }


        /** crSwitch the creation of Impostors on or off.
          * Setting active to false forces the crCullVisitor to use the crImpostor
          * crLod children for rendering. Setting active to true forces the
          * crCullVisitor to create the appropriate pre-rendering stages which
          * render to the crImpostorSprite's texture.*/
        //void setImpostorsActive(bool active) { m_impostorActive = active; }
        //
        ///** Get whether impostors are active or not. */
        //bool getImpostorsActive() const { return m_impostorActive; }

        ///** Set the impostor error threshold.
        //  * Used in calculation of whether impostors remain valid.*/
        //void setImpostorPixelErrorThreshold(float numPixels) { m_impostorPixelErrorThreshold=numPixels; }

        ///** Get the impostor error threshold.*/
        //float getImpostorPixelErrorThreshold() const { return m_impostorPixelErrorThreshold; }

        ///** Set whether crImpostorSprite's should be placed in a depth sorted bin for rendering.*/
        //void setDepthSortcrImpostorSprites(bool doDepthSort) { m_depthSortcrImpostorSprites = doDepthSort; }

        ///** Get whether crImpostorSprite's are depth sorted bin for rendering.*/
        //bool setDepthSortcrImpostorSprites() const { return m_depthSortcrImpostorSprites; }

        ///** Set the number of frames that an crImpostorSprite's is kept whilst not being beyond,
        //  * before being recycled.*/
        //void setNumberOfFrameToKeepcrImpostorSprites(int numFrames) { m_numFramesToKeepcrImpostorSprites = numFrames; }

        ///** Get the number of frames that an crImpostorSprite's is kept whilst not being beyond,
        //  * before being recycled.*/
        //int getNumberOfFrameToKeepcrImpostorSprites() const { return m_numFramesToKeepcrImpostorSprites; }
        
        /** Push state set on the current state group.
          * If the state exists in a child state group of the current
          * state group then move the current state group to that child.
          * Otherwise, create a new state group for the state set, add
          * it to the current state group then move the current state
          * group pointer to the new state group.
          */
        inline bool pushStateSet(const CRCore::crStateSet* ss, bool detailCompare = false/*,int &needPopAlphaShadow*/)
		{
			if(!ss || m_renderMode == OnlyGeodeRender) return false;
			unsigned char ssmode = ss->getStateSetMode();
			switch (m_renderMode)
			{
			case ShadowMapRender:
				if(ssmode != CRCore::crStateSet::SS_ShadowMapRender)
					return false;
				break;
			case CollideMapRender:
				if(ssmode != CRCore::crStateSet::SS_CollideMapRender)
					return false;
				break;
			case NRPRender:
				if(ssmode != CRCore::crStateSet::SS_NRPRender)
					return false;
				break;
			default:
				if(ssmode != CRCore::crStateSet::SS_Normal)
					return false;
				break;
			}

			m_currentRenderGraph = m_currentRenderGraph->find_or_insert(ss,detailCompare);
			if (ss->useRenderBinDetails())
			{
				m_currentRenderBin = m_currentRenderBin->find_or_insert(ss->getBinNumber(),ss->getBinName());
			}
			return true;
		}
        
        /** Pop the top state set and hence associated state group.
          * Move the current state group to the parent of the popped
          * state group.
          */
        inline void popStateSet(/*int needPopAlphaShadow*/)
		{
			if (m_currentRenderGraph->m_stateset->useRenderBinDetails())
			{
				m_currentRenderBin = m_currentRenderBin->m_parent;
			}
			m_currentRenderGraph = m_currentRenderGraph->m_parent;
		}
        
        inline void setRenderGraph(crRenderGraph* rg)
        {
            m_rootRenderGraph = rg;
            m_currentRenderGraph = rg;
        }

        inline crRenderGraph* getRootRenderGraph()
        {
            return m_rootRenderGraph.get();
        }

        inline crRenderGraph* getCurrentRenderGraph()
        {
            return m_currentRenderGraph;
        }

        inline void setRenderStage(crRenderStage* rg)
        {
            m_rootRenderStage = rg;
            m_currentRenderBin = rg;
        }

        inline crRenderStage* getRenderStage()
        {
            return m_rootRenderStage.get();
        }

        inline crRenderBin* getCurrentRenderBin()
        {
            return m_currentRenderBin;
        }

        inline void setCurrentRenderBin(crRenderBin* rb)
        {
            m_currentRenderBin = rb;
        }

        inline value_type getCalculatedNearPlane() const { return m_computed_znear; }
        
        inline value_type getCalculatedFarPlane() const { return m_computed_zfar; }

		value_type computeNearestPointInFrustum(const CRCore::crMatrix& matrix, const CRCore::crPolytope::PlaneList& planes,const CRCore::crDrawable& drawable);

	bool updateCalculatedNearFar(const CRCore::crMatrix& matrix,const CRCore::crDrawable& drawable,bool isBillboard=false);// { updateCalculatedNearFar(matrix,drawable.getBound()); }
	bool updateCalculatedNearFar(const CRCore::crMatrix& matrix,const CRCore::crBoundingBox& bb);
    	void updateCalculatedNearFar(const CRCore::crVector3& pos);
		
        /** Add a drawable to current render graph.*/
        inline void addDrawable(const CRCore::crObject *obj,CRCore::crDrawable* drawable,CRCore::RefMatrix* matrix,bool effectByShadow, CRCore::crNode *needUnLockNode = 0);

        /** Add a drawable and depth to current render graph.*/
		inline void addDrawableAndDepth(const CRCore::crObject *obj,CRCore::crDrawable* drawable,CRCore::RefMatrix* matrix,float depth,bool effectByShadow, CRCore::crNode *needUnLockNode = 0);

        /** Add an attribute which is positioned related to the modelview matrix.*/
        inline void addPositionedAttribute(CRCore::RefMatrix* matrix,const CRCore::crStateAttribute* attr);
        inline void addPositionedTextureAttribute(unsigned int textureUnit, CRCore::RefMatrix* matrix,const CRCore::crStateAttribute* attr);

        /** reimplement crCullStack's popProjectionMatrix() adding clamping of the projection matrix to the computed near and far.*/
        virtual void popProjectionMatrix();

        /** crCullVisitor's default clamping of the projection float matrix to computed near and far values.
          * Note, do not call this method directly, use clampProjectionMatrix(..) instead, unless you want to bypass the callback.*/
        virtual bool clampProjectionMatrixImplementation(CRCore::crMatrixf& projection, double znear, double zfar) const;

        /** crCullVisitor's default clamping of the projection double matrix to computed near and far values.
          * Note, do not call this method directly, use clampProjectionMatrix(..) instead, unless you want to bypass the callback.*/
        virtual bool clampProjectionMatrixImplementation(CRCore::crMatrixd& projection, double znear, double zfar) const;

        /** clamp the projection float matrix to computed near and far values, use callback if it exists, otherwise use default CullVisitro implemntation.*/
        inline bool clampProjectionMatrix(CRCore::crMatrixf& projection, value_type znear, value_type zfar) const
        {
            double zn = znear;
            double zf = zfar;
            bool result = false;
            if (m_clampProjectionMatrixCallback.valid()) result = m_clampProjectionMatrixCallback->clampProjectionMatrixImplementation(projection, zn, zf);
            else result = clampProjectionMatrixImplementation(projection, zn, zf);

            if (result)
            {
                znear = zn;
                zfar = zn;
                return true;
            }
            else 
                return false;
        }

        /** clamp the projection double matrix to computed near and far values, use callback if it exists, otherwise use default CullVisitro implemntation.*/
        inline bool clampProjectionMatrix(CRCore::crMatrixd& projection, value_type znear, value_type zfar) const
        {
            double zn = znear;
            double zf = zfar;
            bool result = false;

            if (m_clampProjectionMatrixCallback.valid()) result = m_clampProjectionMatrixCallback->clampProjectionMatrixImplementation(projection, zn, zf);
            else result = clampProjectionMatrixImplementation(projection, zn, zf);

            if (result)
            {
                znear = zn;
                zfar = zn;
                return true;
            }
            else 
                return false;
        }
        

        void setState(CRCore::crState* state);
        CRCore::crState* getState();
        const CRCore::crState* getState() const;

		//void setNotPushStateSet( bool bln ){ m_notPushStateSet = bln; }

		//void setCamera( CRCore::crMatrixTransform *node)
		//{ 
		//	m_viewMatrixBindNode = node;
		//}
		//const CRCore::crMatrixTransform *getCamera()const{ return m_viewMatrixBindNode.get(); }
		//CRCore::crMatrixTransform *getCamera(){ return m_viewMatrixBindNode.get(); }

		//inline void updateHorizontalFov(float hfov){ m_hfov = hfov; }
		//inline void updateVerticalFov(float vfov){ m_vfov = vfov; }
		//inline float getHorizontalFov(){ return m_hfov; }
		//inline float getVerticalFov(){ return m_vfov; }
		//inline void pushDependCullingBuf( CRCore::crGroup *group ){ m_dependCullingBufStack.push(group); }
		//inline void popDependCullingBuf(){ m_dependCullingBufStack.pop(); }
		//inline CRCore::crGroup *topDependCullingBuf(){ return m_dependCullingBufStack.top().get(); }
		//inline void pushShallRender( bool shallrender ){ m_shallRenderStack.push(shallrender); }
		//inline void popShallRender(){ m_shallRenderStack.pop(); }
		//inline bool topShallRender(){ return m_shallRenderStack.top(); }
		virtual void clearExternFrustum();
		virtual void addExternFrustum(const CRCore::crPolytope& frustum);
		virtual void clearExternOccluderFrustum();
		virtual void addExternOccluderFrustum(const CRCore::crPolytope& frustum);

		//inline virtual void beginSpecialCulling(){ m_enableSpecialCulling = true; }
		//inline virtual void endSpecialCulling(){ m_enableSpecialCulling = false; }
		//inline bool specialCulled(const CRCore::crBoundingBox& bb);
		//inline bool specialCulled(const CRCore::crBoundingSphere& bs);
		//inline bool specialCulled(const CRCore::crNode& node)
		//{
		//	return specialCulled(node.getBound());
		//}
   protected:

         /** prevent unwanted copy construction.*/
         crCullVisitor(const crCullVisitor&): CRCore::crNodeVisitor(), CRCore::crCullStack() {}

        /** prevent unwanted copy operator.*/
        crCullVisitor& operator = (const crCullVisitor&) { return *this; }
        
		//CRCore::crStateSet *getOrCreateStdLightEffectStateSet(const CRCore::crObject *obj,const CRCore::crBoundingBox &bbox, const CRCore::RefMatrix* matrix,bool effectByShadow);
		//CRCore::crStateSet *getOrCreateLightEffectStateSet(const CRCore::crObject *obj,const CRCore::crBoundingBox &bbox, const CRCore::RefMatrix* matrix,bool effectByShadow);
		//void createLightEffectStateSetVec(const CRCore::crObject *obj,const CRCore::crBoundingBox &bbox, const CRCore::RefMatrix* matrix,bool effectByShadow);

        inline void handle_cull_callbacks_and_traverse(CRCore::crNode& node)
        {
#if _DEBUG
			try
			{
#endif
			node.doEvent(WCH_CULLVISITOR,MAKEINT64(this,NULL));
            CRCore::crNodeCallback* callback = node.getCullCallback();
            if (callback) (*callback)(&node,this);
            else traverse(node);
#if _DEBUG
			}
			catch (...)
			{
				//CRCore::notify(CRCore::ALWAYS)<<"handle_cull_callbacks_and_traverse error "<<node.getName()<<std::endl;
				char gbuf[256];
				sprintf(gbuf,"handle_cull_callbacks_and_traverse error %s\n\0",node.getName().c_str());
				gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
			}
#endif
        }

        inline void handle_cull_callbacks_and_accept(CRCore::crNode& node,CRCore::crNode* acceptNode)
        {
            node.doEvent(WCH_CULLVISITOR,MAKEINT64(this,NULL));
			CRCore::crNodeCallback* callback = node.getCullCallback();      
            if (callback) (*callback)(&node,this);
            else acceptNode->accept(*this);
        }

        /** create an impostor sprite by setting up a pre-rendering stage
          * to generate the impostor texture. */
 //       CRCore::crImpostorSprite* createcrImpostorSprite(CRCore::crImpostor& node);

        CRCore::ref_ptr<crRenderGraph>                                m_rootRenderGraph;
        crRenderGraph*                                                m_currentRenderGraph;

        CRCore::ref_ptr<crRenderStage>                                m_rootRenderStage;        
        crRenderBin*                                                  m_currentRenderBin;

        value_type               m_computed_znear;
        value_type               m_computed_zfar;
        
		//typedef std::stack<bool> ComputeAlphaShadowStack;
		//ComputeAlphaShadowStack m_computeAlphaShadowStack;
		//typedef std::list< CRCore::ref_ptr<CRCore::crStateSet> > CASStateSetList;
		//CASStateSetList m_casStateSetList;
		//CRCore::ref_ptr<CRCore::crStateSet> m_computeAlphaShadowStateSet;
		CRCore::ref_ptr<CRCore::crStateSet> m_forMultiRenderStateSet;
		CRCore::ref_ptr<CRCore::crStateSet> m_outlinePerStateSet;
		CRCore::ref_ptr<CRCore::crStateSet> m_outlinePostStateSet;
		typedef std::vector< CRCore::ref_ptr<CRCore::crStateSet> > OutlineColorVec;
		OutlineColorVec m_outlineColorVec;
		//CRCore::ref_ptr<CRCore::crStateSet> m_outlineStateSet2;
		//CRCore::ref_ptr<CRCore::crStateSet> m_selectOutlineStateSet;
		/*struct LS3UniformStateSet : public CRCore::Referenced 
		{
			LS3UniformStateSet(CRCore::crLightSource *ls1,CRCore::crLightSource*ls2,CRCore::crLightSource*ls3, bool effectByShadow, int frameNumber = 0):
		        m_ls1(ls1),
				m_ls2(ls2),
				m_ls3(ls3),
				m_effectByShadow(effectByShadow),
				m_frameNumber(frameNumber){}
			CRCore::ref_ptr<CRCore::crLightSource> m_ls1;
            CRCore::ref_ptr<CRCore::crLightSource> m_ls2;
			CRCore::ref_ptr<CRCore::crLightSource> m_ls3;
			CRCore::ref_ptr<CRCore::crStateSet> m_stateset;
			int m_frameNumber;
			bool m_effectByShadow;

			inline bool compare(CRCore::crLightSource *ls1,CRCore::crLightSource*ls2,CRCore::crLightSource*ls3, bool effectByShadow) const 
			{
				return m_ls1 == ls1 && m_ls2 == ls2 && m_ls3 == ls3 && m_effectByShadow == effectByShadow;
			}
			//inline bool operator == (const LS3UniformStateSet & ls_ss) const 
			//{
			//	return ls1 == ls_ss.ls1 && ls2 == ls_ss.ls2 && ls3 == ls_ss.ls3 && m_effectByShadow == ls_ss.m_effectByShadow;;
			//}
		};
		typedef std::vector< CRCore::ref_ptr<LS3UniformStateSet> > LS3UniformStateSetVec;
		LS3UniformStateSetVec m_ls3UniformStateSetVec;

		struct LSUniformStateSet : public CRCore::Referenced 
		{
			LSUniformStateSet(CRCore::crLightSource *ls1,bool effectByShadow, int frameNumber = 0):
		        m_ls1(ls1),
			    m_effectByShadow(effectByShadow),
			    m_frameNumber(frameNumber){}
			CRCore::ref_ptr<CRCore::crLightSource> m_ls1;
			CRCore::ref_ptr<CRCore::crStateSet> m_stateset;
			int m_frameNumber;
			bool m_effectByShadow;
			inline bool compare(CRCore::crLightSource *ls1,bool effectByShadow) const
			{
				return m_ls1 == ls1 && m_effectByShadow == effectByShadow; 
			}
			//inline bool operator == (const LSUniformStateSet & ls_ss) const 
			//{
			//	return m_ls1 == ls_ss.ls1 && m_effectByShadow == ls_ss.m_effectByShadow;;
			//}
		};
		typedef std::vector< CRCore::ref_ptr<LSUniformStateSet> > LSUniformStateSetVec;
		LSUniformStateSetVec m_lsUniformStateSetVec;
		typedef std::vector< CRCore::ref_ptr<CRCore::crStateSet> > StateSetVec;
		StateSetVec m_tempLsUniformStateSetVec;
		StateSetVec m_stdLightEffectStateSetSet;
		//float                                            m_hfov;
		//float                                            m_vfov;
		//typedef std::stack< CRCore::ref_ptr<CRCore::crGroup> >   GroupStack;         
		//GroupStack                                       m_dependCullingBufStack;
		//bool                                             m_enableSpecialCulling;
		//CRCore::crPlane                                  m_specialCullingPlane;
		//typedef std::stack<bool>   BoolStack;         
		//BoolStack                                       m_shallRenderStack;
*/
        CRCore::ref_ptr<const CRCore::crClearNode> m_clearNode;
	/*
        bool    m_impostorActive;
        bool    m_depthSortcrImpostorSprites;
        float   m_impostorPixelErrorThreshold;
        int     m_numFramesToKeepcrImpostorSprites;*/
	
        
	//typedef std::vector< CRCore::ref_ptr<crRenderLeaf> > RenderLeafList;
	//RenderLeafList m_reuseRenderLeafList;
	//unsigned int m_currentReuseRenderLeafIndex;
	
	inline CRCore::ref_ptr<crRenderLeaf> createOrReuseRenderLeaf(CRCore::crDrawable* drawable,CRCore::RefMatrix* projection,CRCore::RefMatrix* matrix, float depth=0.0f,CRCore::crNode *needUnLockNode = 0);
        
//        CRCore::ref_ptr<CRCore::crImpostorSpriteManager>    m_impostorSpriteManager;

        CRCore::ref_ptr<CRCore::crState>                    m_state;

		//bool m_showBoundingBox;

		//bool m_notPushStateSet;

	    CRCore::ref_ptr<CRCore::crMatrixTransform>       m_viewMatrixBindNode;

		struct MatrixPlanesDrawables
		{
			MatrixPlanesDrawables(const CRCore::crMatrix& matrix, const CRCore::crDrawable* drawable, const CRCore::crPolytope& frustum):
		      m_matrix(matrix),
			  m_drawable(drawable)
		    {
			    // create a new list of planes from the active walls of the frustum.
			    CRCore::crPolytope::ClippingMask result_mask = frustum.getResultMask();
			    CRCore::crPolytope::ClippingMask selector_mask = 0x1;
			    for( CRCore::crPolytope::PlaneList::const_iterator itr=frustum.getPlaneList().begin();
				     itr!=frustum.getPlaneList().end();
				     ++itr)
			    {
				    if (result_mask&selector_mask) m_planes.push_back(*itr);
				    selector_mask <<= 1; 
			    }
		    }
		    MatrixPlanesDrawables(const MatrixPlanesDrawables& mpd):
		        m_matrix(mpd.m_matrix),
			    m_drawable(mpd.m_drawable),
			    m_planes(mpd.m_planes) {}

			MatrixPlanesDrawables& operator = (const MatrixPlanesDrawables& mpd)
			{
				m_matrix = mpd.m_matrix;
				m_drawable = mpd.m_drawable;
				m_planes = mpd.m_planes;
				return *this;
			}

			CRCore::crMatrix                 m_matrix;
			const CRCore::crDrawable*        m_drawable;
			CRCore::crPolytope::PlaneList    m_planes;
		};

		typedef std::multimap<value_type, MatrixPlanesDrawables>   DistanceMatrixDrawableMap;
		DistanceMatrixDrawableMap                                  m_nearPlaneCandidateMap;
};

inline void crCullVisitor::addDrawable(const CRCore::crObject *obj,CRCore::crDrawable* drawable,CRCore::RefMatrix* matrix,bool effectByShadow,CRCore::crNode *needUnLockNode)
{
	drawable->resumeRendering();
	//int popAlphaShadowFlg;
	CRCore::crStateSet *drawabless = drawable->getStateSet();
    bool stateset_pushed = false;
	if(m_renderMode == NormalRender || m_renderMode == RTTRender)
	{
		bool olstate1_pushed = false;
		bool drawoutline = m_renderMode == NormalRender && obj->getDrawOutLine()>=0 && (obj->getDrawOutLine()>0 || obj->getOutlineColorMode()>CRCore::crObject::OL_Black);
		if(drawoutline)
		{
			if (m_outlinePerStateSet.valid()) olstate1_pushed = pushStateSet(m_outlinePerStateSet.get());
		}

		effectByShadow &= m_renderMode == NormalRender;
		const std::string &ts = getTraverseString();

		CRCore::crShaderManager::Callback *callback = 0;
		if(!ts.empty())
		{
            callback = CRCore::crShaderManager::getInstance()->getCallback(ts);
		}
		if(!callback)
		{
			stateset_pushed = false;
			if (drawabless) 
				stateset_pushed = pushStateSet(drawabless/*,popAlphaShadowFlg*/);

			if (m_currentRenderGraph->leaves_empty())
				m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
			m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,0.0f,needUnLockNode));

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
			bool doNotNeedDrawableStateSet = (*callback)(obj,drawable,box,0.0f,effectByShadow,statesetVec);
			int size = statesetVec.size();
		    //bool needlight = ts.compare("needlight") == 0;
			if(size==0/* && !needlight*/)
			{
				stateset_pushed = false;
				if (drawabless) 
					stateset_pushed = pushStateSet(drawabless/*,popAlphaShadowFlg*/);

				if (m_currentRenderGraph->leaves_empty())
					m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
				m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,0.0f,needUnLockNode));

				if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);
			}
			else
			{
				//int popAlpha_s,popAlpha_u,popAlpha_multi;
				bool needMultiRender = false;
				//bool morepass = ts.compare("morepass") == 0;
				bool shader_pushed,uniform_pushed,multi_pushed;
			    if(size > 1/* || needlight*/)
				{
					needMultiRender = true;
				}
				CRCore::crStateSet *first_ss;
				CRCore::crStateSet *second_ss;
				for( CRCore::crShaderManager::StateSetVec::iterator itr = statesetVec.begin();
					itr != statesetVec.end(); )
				{
					shader_pushed = false;
					stateset_pushed = false;
					uniform_pushed = false;
					multi_pushed = false;

					first_ss = itr->first.get();
					second_ss = itr->second.get();

					if(++itr == statesetVec.end()/* && morepass*/)
					{
						needMultiRender = false;
						if(doNotNeedDrawableStateSet) drawabless = 0;
					}
					if(needMultiRender)
					{
						multi_pushed = pushStateSet(m_forMultiRenderStateSet.get()/*,popAlpha_multi*/);
					}

					shader_pushed = pushStateSet(first_ss/*,popAlpha_s*/);

					if (drawabless) 
						stateset_pushed = pushStateSet(drawabless/*,popAlphaShadowFlg*/);

					if(second_ss)
						uniform_pushed = pushStateSet(second_ss,true/*,popAlpha_u*/);

					if (m_currentRenderGraph->leaves_empty())
						m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
					m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,0.0f,needUnLockNode));

					if(uniform_pushed) popStateSet(/*popAlpha_u*/);
					if(stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);
					if(shader_pushed) popStateSet(/*popAlpha_s*/);
					if(multi_pushed) popStateSet(/*popAlpha_multi*/);
				}
			}
		}
		if(drawoutline && olstate1_pushed)
		{
			popStateSet();
			bool olstate2_pushed = pushStateSet(m_outlinePostStateSet.get());
			unsigned char colorMode = obj->getOutlineColorMode();
			if(colorMode >= m_outlineColorVec.size())
				colorMode = 0;
			bool olstate3_pushed = pushStateSet(m_outlineColorVec[colorMode].get());

			if (m_currentRenderGraph->leaves_empty())
				m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
			m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,0.0f,needUnLockNode));

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
	else if(m_renderMode == ShadowMapRender)
	{
		CRCore::crShaderManager::StateSetVec statesetVec;
		if(!CRCore::crShaderManager::getInstance()->getShadowShaderStateSetVec(obj,drawable,statesetVec))
			return;

		bool shader_pushed,uniform_pushed;

		shader_pushed = false;
		//stateset_pushed = false;
		uniform_pushed = false;

		shader_pushed = pushStateSet(statesetVec[0].first.get());//push shader

		//if(drawabless) //push drawabless
		//	stateset_pushed = pushStateSet(drawabless);

		if(statesetVec[0].second.valid()) 
			uniform_pushed = pushStateSet(statesetVec[0].second.get()/*,true*/);//push uniforms,可能是导致树叶阴影UV动画实效的原因

		if (m_currentRenderGraph->leaves_empty())
			m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
		m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,0.0f,needUnLockNode));

		if(uniform_pushed) popStateSet();
		//if(stateset_pushed) popStateSet();
		if(shader_pushed) popStateSet();
	}
	else if(m_renderMode == OnlyGeodeRender)
	{
		if (m_currentRenderGraph->leaves_empty())
			m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
		m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,0.0f,needUnLockNode));
	}
	else
	{
		stateset_pushed = false;
		if (drawabless) stateset_pushed = pushStateSet(drawabless/*,popAlphaShadowFlg*/);

		if (m_currentRenderGraph->leaves_empty())
			m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
		m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,0.0f,needUnLockNode));

		if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);
	}
}

/** Add a drawable and depth to current render graph.*/
inline void crCullVisitor::addDrawableAndDepth(const CRCore::crObject *obj,CRCore::crDrawable* drawable,CRCore::RefMatrix* matrix,float depth,bool effectByShadow,CRCore::crNode *needUnLockNode)
{
//	CRCore::notify(CRCore::FATAL)<<"crCullVisitor::addDrawableAndDepth "<<ts.c_str()<<std::endl;
	//int popAlphaShadowFlg;
	//try
	//{
	drawable->resumeRendering();
	bool stateset_pushed = false;
	CRCore::crStateSet *drawabless = drawable->getStateSet();

	if(m_renderMode == NormalRender || m_renderMode == RTTRender)
	{
		bool olstate1_pushed = false;
		bool drawoutline = m_renderMode == NormalRender && obj->getDrawOutLine()>=0 && ((obj->getDrawOutLine()>0 && depth<CRCore::crDisplaySettings::instance()->getOutLineDispDistance()) || obj->getOutlineColorMode()>CRCore::crObject::OL_Black);
		if(drawoutline)
		{
			if (m_outlinePerStateSet.valid()) olstate1_pushed = pushStateSet(m_outlinePerStateSet.get());
		}

		effectByShadow &= m_renderMode == NormalRender;
		const std::string &ts = getTraverseString();
		CRCore::crShaderManager::Callback *callback = 0;
		if(!ts.empty())
		{
			callback = CRCore::crShaderManager::getInstance()->getCallback(ts);
		}
		//if(callback) CRCore::notify(CRCore::ALWAYS)<<"callback "<<ts<< std::endl;
		if(!callback)
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
			m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,depth,needUnLockNode));

			if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);
		}
		else
		{
			CRCore::crBoundingBox box = drawable->getBound();
			if(matrix)
			{
				CRCore::crMatrix mat = *matrix * getCameraViewInverseMatrix();
				box.m_max = box.m_max * mat;
				box.m_min = box.m_min * mat;
				box.correct();
			}
			CRCore::crShaderManager::StateSetVec statesetVec;
			bool doNotNeedDrawableStateSet = (*callback)(obj,drawable,box,depth,effectByShadow,statesetVec);
			int size = statesetVec.size();
			//bool needlight = ts.compare("needlight") == 0;
			if(size==0/* && !needlight*/)
			{
				stateset_pushed = false;
				if (drawabless) stateset_pushed = pushStateSet(drawabless/*,popAlphaShadowFlg*/);

				if (m_currentRenderGraph->leaves_empty())
					m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
				m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,depth,needUnLockNode));

				if (stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);
			}
			else
			{
				//int popAlpha_s,popAlpha_u,popAlpha_multi;
				bool needMultiRender = false;
				//bool morepass = ts.compare("morepass") == 0;
				bool shader_pushed,uniform_pushed,multi_pushed;
				if(size > 1/* || needlight*/)
				{
					needMultiRender = true;
				}
				CRCore::crStateSet *first_ss;
				CRCore::crStateSet *second_ss;
				for( CRCore::crShaderManager::StateSetVec::iterator itr = statesetVec.begin();
					itr != statesetVec.end(); )
				{
					shader_pushed = false;
					stateset_pushed = false;
					uniform_pushed = false;
					multi_pushed = false;

					first_ss = itr->first.get();
                    second_ss = itr->second.get();

					if(++itr == statesetVec.end()/* && morepass*/)
					{
						needMultiRender = false;
						if(doNotNeedDrawableStateSet) drawabless = 0;
					}

					if(needMultiRender)
					{
						multi_pushed = pushStateSet(m_forMultiRenderStateSet.get()/*,popAlpha_multi*/);
						//CRCore::notify(CRCore::WARN) << "crCullVisitor::addDrawableAndDepth multi_pushed = "<<statesetVec.size()<<std::endl;
					}

				    shader_pushed = pushStateSet(first_ss/*,popAlpha_s*/);

					if (drawabless) stateset_pushed = pushStateSet(drawabless/*,popAlphaShadowFlg*/);

					if(second_ss)
					    uniform_pushed = pushStateSet(second_ss,true/*,popAlpha_u*/);

					if (m_currentRenderGraph->leaves_empty())
						m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
					m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,depth,needUnLockNode));

					if(uniform_pushed) popStateSet(/*popAlpha_u*/);
					if(stateset_pushed) popStateSet(/*popAlphaShadowFlg*/);
					if(shader_pushed) popStateSet(/*popAlpha_s*/);
				    if(multi_pushed) popStateSet(/*popAlpha_multi*/);
				}
			}
		}
		if(drawoutline && olstate1_pushed)
		{
			popStateSet();
			bool olstate2_pushed = pushStateSet(m_outlinePostStateSet.get());
			unsigned char colorMode = obj->getOutlineColorMode();
			if(colorMode >= m_outlineColorVec.size())
				colorMode = 0;
			bool olstate3_pushed = pushStateSet(m_outlineColorVec[colorMode].get());

			if (m_currentRenderGraph->leaves_empty())
				m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
			m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,depth,needUnLockNode));

			if (olstate2_pushed) popStateSet();
			if (olstate3_pushed) popStateSet();
		}
	}
	else if(m_renderMode == ShadowMapRender)
	{
		CRCore::crShaderManager::StateSetVec statesetVec;
		if(!CRCore::crShaderManager::getInstance()->getShadowShaderStateSetVec(obj,drawable,statesetVec))
		{
			//CRCore::notify(CRCore::FATAL)<<"getShadowShaderStateSetVec: "<<std::endl;
			return;
		}
		bool shader_pushed,uniform_pushed;

		shader_pushed = false;
		//stateset_pushed = false;
		uniform_pushed = false;

		shader_pushed = pushStateSet(statesetVec[0].first.get());//push shader
		
		//if(drawabless) //push drawabless
		//	stateset_pushed = pushStateSet(drawabless);

		if(statesetVec[0].second.valid()) uniform_pushed = pushStateSet(statesetVec[0].second.get(),false);//push uniforms

		//if(stateset_pushed) CRCore::notify(CRCore::ALWAYS)<<"ShadowMapRender "<<obj->getName()<<std::endl;

		if (m_currentRenderGraph->leaves_empty())
			m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
		m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,depth,needUnLockNode));
		
		if(uniform_pushed) popStateSet();
		//if(stateset_pushed) popStateSet();
		if(shader_pushed) popStateSet();
	}
	else if(m_renderMode == OnlyGeodeRender || m_renderMode == NRPRender)
	{
		if (m_currentRenderGraph->leaves_empty())
			m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
		m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,depth/*0.0f*/,needUnLockNode));
	}
	else
	{
		stateset_pushed = false;
		if (drawabless) stateset_pushed = pushStateSet(drawabless/*,popAlphaShadowFlg*/);

		if (m_currentRenderGraph->leaves_empty())
			m_currentRenderBin->addRenderGraph(m_currentRenderGraph);
		m_currentRenderGraph->addLeaf(createOrReuseRenderLeaf(drawable,m_projectionStack.back().get(),matrix,depth/*0.0f*/,needUnLockNode));

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

/** Add an attribute which is positioned related to the modelview matrix.*/
inline void crCullVisitor::addPositionedAttribute(CRCore::RefMatrix* matrix,const CRCore::crStateAttribute* attr)
{
	//if(!m_shallRenderStack.empty()&&!topShallRender()) return;

    m_currentRenderBin->m_stage->addPositionedAttribute(matrix,attr);
}

inline void crCullVisitor::addPositionedTextureAttribute(unsigned int textureUnit, CRCore::RefMatrix* matrix,const CRCore::crStateAttribute* attr)
{
	//if(!m_shallRenderStack.empty()&&!topShallRender()) return;

	m_currentRenderBin->getStage()->addPositionedTextureAttribute(textureUnit,matrix,attr);
}

inline CRCore::ref_ptr<crRenderLeaf> crCullVisitor::createOrReuseRenderLeaf(CRCore::crDrawable* drawable,CRCore::RefMatrix* projection,CRCore::RefMatrix* matrix, float depth,CRCore::crNode *needUnLockNode)
{
  //  // skip of any already reused renderleaf.
  //  while (m_currentReuseRenderLeafIndex<m_reuseRenderLeafList.size() && 
  //         m_reuseRenderLeafList[m_currentReuseRenderLeafIndex]->referenceCount()>1)
  //  {
		////CRCore::notify(CRCore::WARN)<<"Warning:createOrReuseRenderLeaf() skipping multiply refrenced entry."<< std::endl;
  //      ++m_currentReuseRenderLeafIndex;
  //  }

  //  // if still within list, element must be singularly referenced
  //  // there return it to be reused.
  //  if (m_currentReuseRenderLeafIndex<m_reuseRenderLeafList.size())
  //  {
  //      crRenderLeaf* renderleaf = m_reuseRenderLeafList[m_currentReuseRenderLeafIndex++].get();
  //      renderleaf->set(drawable,projection,matrix,depth,needUnLockNode);
  //      return renderleaf;
  //  }

  // //  otherwise need to create new renderleaf.
  //  CRCore::ref_ptr<crRenderLeaf> renderleaf = new crRenderLeaf(drawable,projection,matrix,depth,needUnLockNode);
  //  m_reuseRenderLeafList.push_back(renderleaf);
  //  ++m_currentReuseRenderLeafIndex;
  //  return renderleaf;
	return new crRenderLeaf(drawable,projection,matrix,depth,needUnLockNode);
}

}

#endif

