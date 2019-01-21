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
#include <CRPhysics/crMatterObject.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRPhysics/crWeaponNode.h>
#include <CRParticle/crParticleEffect.h>
//#include <CRPhysics/crMatterGroup.h>

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
		virtual void apply(CRParticle::crParticleEffect& node);

	 //   virtual void apply(CRPhysics::crCaissonMatterObject& node);
		//virtual void apply(CRPhysics::crHpMatterObject& node);
		//virtual void apply(CRPhysics::crArmorMatterObject& node);

		virtual void apply(CRCore::crFilterRenderManager& node);

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
			//try
			//{
				m_currentRenderGraph = m_currentRenderGraph->find_or_insert(ss, detailCompare);
				if (ss->useRenderBinDetails())
				{
					m_currentRenderBin = m_currentRenderBin->find_or_insert(ss->getBinNumber(), ss->getBinName());
				}
			//}
			//catch (...)
			//{
			//	char gbuf[256];
			//	sprintf(gbuf, "pushStateSet error %d\n\0",GetLastError());
			//	gDebugInfo->debugInfo(CRCore::ALWAYS, gbuf);
			//}
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
#ifdef _DEBUG
			try
			{
#endif
			node.doEvent(WCH_CULLVISITOR,MAKEINT64(this,NULL));
            CRCore::crNodeCallback* callback = node.getCullCallback();
            if (callback) (*callback)(&node,this);
            else traverse(node);
#ifdef _DEBUG
			}
			catch (...)
			{
				//CRCore::notify(CRCore::ALWAYS)<<"handle_cull_callbacks_and_traverse error "<<node.getName()<<std::endl;
				char gbuf[256];
				sprintf(gbuf,"handle_cull_callbacks_and_traverse error %s,%s,%s,code=%d\n\0",node.getName().c_str(), node.className(),node.getParent(0)->className(), GetLastError());
				gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);

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

