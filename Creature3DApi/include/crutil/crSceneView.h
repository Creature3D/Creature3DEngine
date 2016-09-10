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
#ifndef CRUTIL_CRSCENEVIEW_H
#define CRUTIL_CRSCENEVIEW_H 1

#include <CRUtil/crExport.h>
#include <CRCore/crNode.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crLight.h>
#include <CRCore/crFrameStamp.h>
#include <CRCore/crDisplaySettings.h>
#include <CRCore/crViewPort.h>
#include <CRCore/crAlphaFunc.h>
#include <CRCore/crLightModel.h>
#include <CRCore/crDepth.h>
#include <CRCore/crPolygonMode.h>
#include <CRCore/crHint.h>
#include <CRCore/crTexEnv.h>
#include <CRCore/crBlendFunc.h>
#include <CRCore/crCullFace.h>
#include <CRCore/crState.h>
#include <CRCore/crShadeModel.h>
#include <CRCore/crCullSettings.h>
#include <CRCore/crMatrixTransform.h>

#include <CRUtil/crCullVisitor.h>
#include <CRUtil/crCollectOccludersVisitor.h>

#include <CRCore/thread/crMutex.h>

namespace CRUtil {

/**
 * crSceneView is literally a view of a scene, encapsulating the
 * camera (modelview+projection matrices), global state, lights and the scene itself.  Provides
 * methods for setting up the view and rendering it.
*/
class CRUTIL_EXPORT crSceneView : public CRCore::Referenced, public CRCore::crCullSettings
{
    public:

		enum Options
		{
			NO_SCENEVIEW_LIGHT = 0x00,
			HEADLIGHT = 0x01,
			SKY_LIGHT = 0x02,
			COMPILE_GLOBJECTS_AT_INIT = 0x04,
			USEPHYSICS = 0x08,
			USEAI = 0x10,
			STANDARD_SETTINGS = NO_SCENEVIEW_LIGHT |
			                    COMPILE_GLOBJECTS_AT_INIT                                
		};


        /** Construct a default scene view.*/
        crSceneView(CRCore::crDisplaySettings* ds=NULL);

		virtual ~crSceneView();

        /** Set scene view to use default global state, light, camera
         *  and render visitor.
        */
		void setDefaults(unsigned int options = STANDARD_SETTINGS);

		/** Set the camera used to represent the camera view of this SceneView.*/
		void setCamera(CRCore::crCameraNode* camera);

		/** Get the camera used to represent the camera view of this SceneView.*/
		CRCore::crCameraNode* getCamera();

		/** Get the const camera used to represent the camera view of this SceneView.*/
		const CRCore::crCameraNode* getCamera() const;

        /** Set the data which to view. The data will typically be
         *  an CRCore::Scene but can be any CRCore::crNode type.
         */
        void setSceneData(CRCore::crNode* node);

		/** Get the scene data to view. The data will typically be
		*  an osg::Scene but can be any osg::Node type.
		*/
		inline CRCore::crNode* getSceneData(unsigned int childNo=0) { return (m_camera->getNumChildren()>childNo) ? m_camera->getChild(childNo) : 0; }

		/** Get the const scene data which to view. The data will typically be
		*  an osg::Scene but can be any osg::Node type.
		*/
		inline const CRCore::crNode* getSceneData(unsigned int childNo=0) const { return (m_camera->getNumChildren()>childNo) ? m_camera->getChild(childNo) : 0; }

		/** Get the number of scene data subgraphs added to the SceneView's camera.*/
		unsigned int getNumSceneData() const;

		void setViewport(CRCore::crViewPort* viewport);
		void setViewport(int x,int y,int width,int height);

		inline CRCore::crViewPort* crSceneView::getViewport() 
		{ 
			return (m_camera->getViewport()!=0) ? m_camera->getViewport() : 0; 
		}
		inline const CRCore::crViewPort* crSceneView::getViewport() const 
		{ 
			return (m_camera->getViewport()!=0) ? m_camera->getViewport() : 0; 
		}
		void getViewport(int& x,int& y,int& width,int& height) const;

        void setDisplaySettings(CRCore::crDisplaySettings* vs);
        
        const CRCore::crDisplaySettings* getDisplaySettings() const;
        CRCore::crDisplaySettings* getDisplaySettings();

		void setClearColor(const CRCore::crVector4& color);
		const CRCore::crVector4& getClearColor() const;

		/** Mannually set the redraw interlaced stereo stencil mask request flag to control whether to redraw the stencil buffer on the next frame.*/  
		void setRedrawInterlacedStereoStencilMask(bool flag);
		/** Get the redraw interlaced stereo stencil mask request flag.*/  
		bool getRedrawInterlacedStereoStencilMask() const;

        void setGlobalStateSet(CRCore::crStateSet* state);
        CRCore::crStateSet* getGlobalStateSet();
        const CRCore::crStateSet* getGlobalStateSet() const;

        void setLocalStateSet(CRCore::crStateSet* state);
        CRCore::crStateSet* getLocalStateSet();
        const CRCore::crStateSet* getLocalStateSet() const;

		enum ActiveUniforms
		{
			FRAME_NUMBER_UNIFORM            = 0x1,
			FRAME_TIME_UNIFORM              = 0x2,
			DELTA_FRAME_TIME_UNIFORM        = 0x4,
			VIEW_MATRIX_UNIFORM             = 0x8,
			INVERSE_VIEW_MATRIX_UNIFORM     = 0x16,
			CAMERAPOS_UNIFORM               = 0x64,
			DEFAULT_UNIFORMS                = /*FRAME_TIME_UNIFORM |*/
			                                  INVERSE_VIEW_MATRIX_UNIFORM |
											  CAMERAPOS_UNIFORM,
			ALL_UNIFORMS                    = 0xFFFFFFFF
		};

		void setActiveUniforms(int activeUniforms);
		int getActiveUniforms() const;

        typedef Options LightingMode;
        void setLightingMode(LightingMode mode);
        LightingMode getLightingMode() const;

        void setLight(CRCore::crLight* light);
        CRCore::crLight* getLight();
        const CRCore::crLight* getLight() const;
        
        void setState(CRCore::crState* state);
		CRCore::crState* getState();
        const CRCore::crState* getState() const;

		inline CRCore::crState* getCurrentRenderState() { return /*m_useRenderDoubleBuf?m_state_buf.get():*/m_state.get(); }
		inline const CRCore::crState* getCurrentRenderState() const { return /*m_useRenderDoubleBuf?m_state_buf.get():*/m_state.get(); }

		void setContextID( int id );

        /** Set the projection matrix. Can be thought of as setting the lens of a camera. */
        inline void setProjectionMatrix(const CRCore::crMatrixf& matrix) { m_camera->setProjectionMatrix(matrix); }

        /** Set the projection matrix. Can be thought of as setting the lens of a camera. */
        inline void setProjectionMatrix(const CRCore::crMatrixd& matrix) { m_camera->setProjectionMatrix(matrix); }

        /** Set to a orthographic projection. See OpenGL glOrtho for documentation further details.*/
        void setProjectionMatrixAsOrtho(double left, double right,
                                        double bottom, double top,
                                        double zNear, double zFar);

        /** Set to a 2D orthographic projection. See OpenGL glOrtho2D documentation for further details.*/
        void setProjectionMatrixAsOrtho2D(double left, double right,
                                          double bottom, double top);

        /** Set to a perspective projection. See OpenGL glFrustum documentation for further details.*/
        void setProjectionMatrixAsFrustum(double left, double right,
                                          double bottom, double top,
                                          double zNear, double zFar);

        /** Create a symmetrical perspective projection, See OpenGL gluPerspective documentation for further details.
          * Aspect ratio is defined as width/height.*/
        void setProjectionMatrixAsPerspective(double fovy,double aspectRatio,
                                              double zNear, double zFar);

        /** Get the projection matrix.*/
        inline CRCore::crMatrixd* getProjectionMatrix() { return m_camera->getProjectionMatrix(); }

        /** Get the const projection matrix.*/
        inline const CRCore::crMatrixd* getProjectionMatrix() const { return m_camera->getProjectionMatrix(); }

		CRCore::crMatrixd* getBufProjectionMatrix();
		const CRCore::crMatrixd* getBufProjectionMatrix() const;

        /** Get the othorgraphic settings of the orthographic projection matrix. 
          * Returns false if matrix is not an orthographic matrix, where parameter values are undefined.*/
        bool getProjectionMatrixAsOrtho(double& left, double& right,
                                        double& bottom, double& top,
                                        double& zNear, double& zFar);

        /** Get the frustum setting of a perspective projection matrix.
          * Returns false if matrix is not a perspective matrix, where parameter values are undefined.*/
        bool getProjectionMatrixAsFrustum(double& left, double& right,
                                          double& bottom, double& top,
                                          double& zNear, double& zFar);

        /** Get the frustum setting of a symetric perspective projection matrix.
          * Returns false if matrix is not a perspective matrix, where parameter values are undefined. 
          * Note, if matrix is not a symetric perspective matrix then the shear will be lost.
          * Asymetric metrices occur when stereo, power walls, caves and reality center display are used.
          * In these configuration one should use the AsFrustum method instead.*/
        bool getProjectionMatrixAsPerspective(double& fovy,double& aspectRatio,
                                              double& zNear, double& zFar);


        /** Set the view matrix. Can be thought of as setting the position of the world relative to the camera in camera coordinates. */
        inline void setViewMatrix(const CRCore::crMatrixf& matrix) { m_camera->setViewMatrix(matrix); }
        
        /** Set the view matrix. Can be thought of as setting the position of the world relative to the camera in camera coordinates. */
        inline void setViewMatrix(const CRCore::crMatrixd& matrix) { m_camera->setViewMatrix(matrix); }

        /** Set to the position and orientation of view matrix, using the same convention as gluLookAt. */
        void setViewMatrixAsLookAt(const CRCore::crVector3d& eye,const CRCore::crVector3d& center,const CRCore::crVector3d& up);

        /** Get the view matrix. */
        inline CRCore::crMatrixd* getViewMatrix() { return m_camera->getViewMatrix(); }

        /** Get the const view matrix. */
        inline const CRCore::crMatrixd* getViewMatrix() const { return m_camera->getViewMatrix(); }

		CRCore::crMatrixd* getBufViewMatrix();
		const CRCore::crMatrixd* getBufViewMatrix() const;

		CRCore::crMatrixd* getViewInverseMatrix();
		const CRCore::crMatrixd* getViewInverseMatrix() const;
		CRCore::crMatrixd* getBufViewInverseMatrix();
		const CRCore::crMatrixd* getBufViewInverseMatrix() const;

        void getViewMatrixAsLookAt(CRCore::crVector3d& eye,CRCore::crVector3d& center,CRCore::crVector3d& up,float lookDistance=1.0f);

        void setDrawBufferMode( unsigned int drawBufferMode );
        unsigned int getDrawBufferMode() const;
   
        void setInitVisitor(CRCore::crNodeVisitor* av);
        CRCore::crNodeVisitor* getInitVisitor();
        const CRCore::crNodeVisitor* getInitVisitor() const;

		void setPhysicsInitVisitor(CRCore::crNodeVisitor* pv);
		CRCore::crNodeVisitor* getPhysicsInitVisitor();
		const CRCore::crNodeVisitor* getPhysicsInitVisitor() const;

		void setUpdateVisitor(CRCore::crNodeVisitor* av);
        CRCore::crNodeVisitor* getUpdateVisitor();
        const CRCore::crNodeVisitor* getUpdateVisitor() const;

        void setCullVisitor(CRUtil::crCullVisitor* cv);
        CRUtil::crCullVisitor* getCullVisitor();
        const CRUtil::crCullVisitor* getCullVisitor() const;

        void setCullVisitorLeft(CRUtil::crCullVisitor* cv);
        CRUtil::crCullVisitor* getCullVisitorLeft();
        const CRUtil::crCullVisitor* getCullVisitorLeft() const;

        void setCullVisitorRight(CRUtil::crCullVisitor* cv);
        CRUtil::crCullVisitor* getCullVisitorRight();
        const CRUtil::crCullVisitor* getCullVisitorRight() const;
        
        void setCollectOccludersVisitor(crCollectOccludersVisitor* cov);
        crCollectOccludersVisitor* getCollectOccludersVisitor();
        const crCollectOccludersVisitor* getCollectOccludersVisitor() const;

        void setRenderGraph(CRUtil::crRenderGraph* rg);
        CRUtil::crRenderGraph* getRenderGraph();
        const CRUtil::crRenderGraph* getRenderGraph() const;

        void setRenderGraphLeft(CRUtil::crRenderGraph* rg);
        CRUtil::crRenderGraph* getRenderGraphLeft();
        const CRUtil::crRenderGraph* getRenderGraphLeft() const;

        void setRenderGraphRight(CRUtil::crRenderGraph* rg);
        CRUtil::crRenderGraph* getRenderGraphRight();
        const CRUtil::crRenderGraph* getRenderGraphRight() const;

        void setRenderStage(CRUtil::crRenderStage* rs);
        CRUtil::crRenderStage* getRenderStage();
        const CRUtil::crRenderStage* getRenderStage() const;

        void setRenderStageLeft(CRUtil::crRenderStage* rs);
        CRUtil::crRenderStage* getRenderStageLeft();
        const CRUtil::crRenderStage* getRenderStageLeft() const;

        void setRenderStageRight(CRUtil::crRenderStage* rs);
        CRUtil::crRenderStage* getRenderStageRight();
        const CRUtil::crRenderStage* getRenderStageRight() const;

        /** FusionDistanceMode is used only when working in stereo.*/
        enum FusionDistanceMode
        {
            /** Use fusion distance from the value set on the crSceneView.*/
            USE_FUSION_DISTANCE_VALUE,
            /** Compute the fusion distance by multiplying the screen distance by the  fusion distance value.*/
            PROPORTIONAL_TO_SCREEN_DISTANCE
        };

        /** Set the FusionDistanceMode and Value. Note, is used only when working in stereo.*/
        inline void setFusionDistance(FusionDistanceMode mode,float value=1.0f)
        {
            m_fusionDistanceMode = mode;
            m_fusionDistanceValue = value;
        }

        /** Get the FusionDistanceMode.*/
        FusionDistanceMode getFusionDistanceMode() const;
        /** Get the FusionDistanceValue. Note, only used for USE_FUSION_DISTANCE_VALUE & PROPORTIONAL_TO_SCREEN_DISTANCE modes.*/
        float getFusionDistanceValue() const;

        /** Calculate, via glUnProject, the object coordinates of a window point.
            Note, current implementation requires that crSceneView::draw() has been previously called
            for projectWindowIntoObject to produce valid values.  Consistent with OpenGL
            windows coordinates are calculated relative to the bottom left of the window.
            Returns true on successful projection.
        */
        bool projectWindowIntoObject(const CRCore::crVector3& window,CRCore::crVector3& object) const;

        /** Calculate, via glUnProject, the object coordinates of a window x,y
            when projected onto the near and far planes.
            Note, current implementation requires that crSceneView::draw() has been previously called
            for projectWindowIntoObject to produce valid values.  Consistent with OpenGL
            windows coordinates are calculated relative to the bottom left of the window.
            Returns true on successful projection.
        */
        bool projectWindowXYIntoObject(int x,int y,CRCore::crVector3& near_point,CRCore::crVector3& far_point) const;

        /** Calculate, via glProject, the object coordinates of a window.
            Note, current implementation requires that crSceneView::draw() has been previously called
            for projectWindowIntoObject to produce valid values.  Consistent with OpenGL
            windows coordinates are calculated relative to the bottom left of the window,
            whereas as window API's normally have the top left as the origin,
            so you may need to pass in (mouseX,window_height-mouseY,...).
            Returns true on successful projection.
        */
        bool projectObjectIntoWindow(const CRCore::crVector3& object,CRCore::crVector3& window) const;


		void setAbortRenderingPtr(bool *Ptr);

        ///** Set the frame stamp for the current frame.*/
        //void setFrameStamp(CRCore::crFrameStamp* fs);

        ///** Set the frame stamp for the current frame.*/
        //inline const CRCore::crFrameStamp* getFrameStamp() const { return m_frameStamp.get(); }

        /** callback for overidding the default method for compute the offset projection and view matrices.*/
        struct ComputeStereoMatricesCallback : public CRCore::Referenced
        {
            virtual CRCore::crMatrixd computeLeftEyeProjection(const CRCore::crMatrixd& projection) const = 0;
            virtual CRCore::crMatrixd computeLeftEyeView(const CRCore::crMatrixd& view) const = 0;

            virtual CRCore::crMatrixd computeRightEyeProjection(const CRCore::crMatrixd& projection) const = 0;
            virtual CRCore::crMatrixd computeRightEyeView(const CRCore::crMatrixd& view) const = 0;
        };
        
        void setComputeStereoMatricesCallback(ComputeStereoMatricesCallback* callback);
        ComputeStereoMatricesCallback* getComputeStereoMatricesCallback();
        const ComputeStereoMatricesCallback* getComputeStereoMatricesCallback() const;

        inline CRCore::crMatrixd computeLeftEyeProjection(const CRCore::crMatrixd& projection)  const
        {
            if (m_computeStereoMatricesCallback.valid()) return m_computeStereoMatricesCallback->computeLeftEyeProjection(projection);
            else return computeLeftEyeProjectionImplementation(projection);
        }

        inline CRCore::crMatrixd computeLeftEyeView(const CRCore::crMatrixd& view) const
        {
            if (m_computeStereoMatricesCallback.valid()) return m_computeStereoMatricesCallback->computeLeftEyeView(view);
            else return computeLeftEyeViewImplementation(view);
        }
        
        inline CRCore::crMatrixd computeRightEyeProjection(const CRCore::crMatrixd& projection)  const
        {
            if (m_computeStereoMatricesCallback.valid()) return m_computeStereoMatricesCallback->computeRightEyeProjection(projection);
            else return computeRightEyeProjectionImplementation(projection);
        }

        inline CRCore::crMatrixd computeRightEyeView(const CRCore::crMatrixd& view) const
        {
            if (m_computeStereoMatricesCallback.valid()) return m_computeStereoMatricesCallback->computeRightEyeView(view);
            else return computeRightEyeViewImplementation(view);
        }

        CRCore::crMatrixd computeLeftEyeProjectionImplementation(const CRCore::crMatrixd& projection) const;
        CRCore::crMatrixd computeLeftEyeViewImplementation(const CRCore::crMatrixd& view) const;

        CRCore::crMatrixd computeRightEyeProjectionImplementation(const CRCore::crMatrixd& projection) const;
        CRCore::crMatrixd computeRightEyeViewImplementation(const CRCore::crMatrixd& view) const;


        /** Do init traversal of attached scene graph using Init crNodeVisitor.
          * The init traversal is called once for each crSceneView, and should
          * be used to compile display list, texture objects intialize data
          * not otherwise intializaed during scene graph loading. Note, is
          * called automatically by update&cull if it hasn't already been called
          * elsewhere. Also init() should only ever be called within a valid
          * graphics context.*/
        void init();
		inline bool isInited(){return m_initCalled;}
        //做数据更新，场景图变动在这里是安全的，物理、智能、粒子等系统在此处同步
        void update();

        //做数据剪裁，可以做部分数据更新工作，但是场景图变动在这里是不安全的（如：节点的增加、删除、替换、crSwitch节点访问流的变动等操作）
		//当有无法避免在cull流里对场景图造成变动的，可以通过NodeMutex进行同步。
        void cull();

        //绘制
		void draw();
        
		void releaseAllObjects();
		void flushAllDeletedObjects();
		void flushDeletedObjects(double& availableTime/*,CRCore::crState *state*/);
       
		void setUseRenderDoubleBuffer( bool useRenderDoubleBuf );
        bool getUseRenderDoubleBuffer()const;

		bool doubleThreadRenderInit();

		//struct NetWorkUpdateCallback : public CRCore::Referenced
		//{
		//	NetWorkUpdateCallback() {}
  //          virtual void operator()(void)=0;
		//};
		//static void registerNetWorkUpdateCallback(NetWorkUpdateCallback* nc) { m_netWorkUpdateCallback=nc;}

		//struct SceneManagerUpdater : public CRCore::Referenced
		//{
		//	SceneManagerUpdater() {}
		//    virtual void operator()(void)=0;
		//};
		//static void registerSceneManagerUpdater(SceneManagerUpdater* updater) { s_sceneManagerUpdater=updater;}
		struct ExternUpdater : public CRCore::Referenced
		{
			ExternUpdater() {}
		    virtual void operator()(float dt)=0;
		};
		static void registerExternUpdater(ExternUpdater* updater) { s_externUpdaterVec.push_back(updater); }
		typedef std::vector< CRCore::ref_ptr<ExternUpdater> > ExternUpdaterVec;
		void unlockSwapBuffer();
    protected:
		void swapBuffers();
		void updateUniforms();
		void flushAllDeletedObjectsImp(unsigned int contextID);
        /** Do cull traversal of attached scene graph using Cull crNodeVisitor.*/
        void cullStage(const CRCore::crMatrixd& projection,const CRCore::crMatrixd& modelview,const CRCore::crMatrixd& inverseview,CRUtil::crCullVisitor* cullVisitor, CRUtil::crRenderGraph* rendergraph, CRUtil::crRenderStage* renderStage);

        const CRCore::crMatrix computeMVPW() const;

		void clearArea(int x,int y,int width,int height,const CRCore::crVector4& color);

        CRCore::ref_ptr<CRCore::crStateSet>              m_globalStateSet;
        CRCore::ref_ptr<CRCore::crStateSet>              m_localStateSet;
        CRCore::ref_ptr<CRCore::crLight>                 m_light;

        CRCore::ref_ptr<CRCore::crDisplaySettings>       m_displaySettings;
        CRCore::ref_ptr<CRCore::crState>                 m_state;
		//CRCore::ref_ptr<CRCore::crState>                 m_state_buf;

        bool                                             m_initCalled;
        CRCore::ref_ptr<CRCore::crNodeVisitor>           m_initVisitor;
		CRCore::ref_ptr<CRCore::crNodeVisitor>           m_physicsInitVisitor;
        CRCore::ref_ptr<CRCore::crNodeVisitor>           m_updateVisitor;
		CRCore::ref_ptr<CRCore::crNodeVisitor>           m_physicsUpdateVisitor;
	    CRCore::ref_ptr<CRCore::crNodeVisitor>           m_aiUpdateVisitor;
		//CRCore::ref_ptr<CRCore::crNodeVisitor>           m_updateUniformVisitor;

		CRCore::ref_ptr<CRUtil::crCullVisitor>           m_cullVisitor;
        CRCore::ref_ptr<CRUtil::crRenderGraph>           m_rendergraph;
        CRCore::ref_ptr<CRUtil::crRenderStage>           m_renderStage;
		//CRCore::ref_ptr<CRUtil::crCullVisitor>           m_cullVisitor_buf;
		CRCore::ref_ptr<CRUtil::crRenderGraph>           m_rendergraph_buf;
		CRCore::ref_ptr<CRUtil::crRenderStage>           m_renderStage_buf;

        CRCore::ref_ptr<ComputeStereoMatricesCallback>   m_computeStereoMatricesCallback;

        CRCore::ref_ptr<CRUtil::crCullVisitor>           m_cullVisitorLeft;
        CRCore::ref_ptr<CRUtil::crRenderGraph>           m_rendergraphLeft;
        CRCore::ref_ptr<CRUtil::crRenderStage>           m_renderStageLeft;
		//CRCore::ref_ptr<CRUtil::crCullVisitor>           m_cullVisitorLeft_buf;
		CRCore::ref_ptr<CRUtil::crRenderGraph>           m_rendergraphLeft_buf;
		CRCore::ref_ptr<CRUtil::crRenderStage>           m_renderStageLeft_buf;

        CRCore::ref_ptr<CRUtil::crCullVisitor>           m_cullVisitorRight;
        CRCore::ref_ptr<CRUtil::crRenderGraph>           m_rendergraphRight;
        CRCore::ref_ptr<CRUtil::crRenderStage>           m_renderStageRight;
		//CRCore::ref_ptr<CRUtil::crCullVisitor>           m_cullVisitorRight_buf;
		CRCore::ref_ptr<CRUtil::crRenderGraph>           m_rendergraphRight_buf;
		CRCore::ref_ptr<CRUtil::crRenderStage>           m_renderStageRight_buf;

        CRCore::ref_ptr<crCollectOccludersVisitor>       m_collectOccludersVisistor;
        
        //CRCore::ref_ptr<CRCore::crFrameStamp>            m_frameStamp;

		CRCore::ref_ptr<CRCore::crCameraNode>            m_camera;

        CRCore::crVector4f                               m_clearColor;

        FusionDistanceMode                               m_fusionDistanceMode;
        float                                            m_fusionDistanceValue;

        LightingMode                                     m_lightingMode;
        
		unsigned int                                   m_drawBufferMode;
        
        bool                                             m_requiresFlush;
        bool                                             m_useRenderDoubleBuf;
		CRCore::crMutex                               m_swapBufMutex;
		bool                                          m_swapBufMutexLocked;

		int                                              m_activeUniforms;        
		//double                                           m_previousFrameTime;
		
		bool                                        m_redrawInterlacedStereoStencilMask;
		int                                         m_interlacedStereoStencilWidth;
		int                                         m_interlacedStereoStencilHeight;

		//static CRCore::ref_ptr<NetWorkUpdateCallback> m_netWorkUpdateCallback;
		//static CRCore::ref_ptr<SceneManagerUpdater> s_sceneManagerUpdater;
		static ExternUpdaterVec s_externUpdaterVec;
		//CRCore::Timer_t m_lastDrawTick;
};
}
#endif