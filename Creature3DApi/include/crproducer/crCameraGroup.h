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
#ifndef CRPRODUCER_CRCAMERAGROUP_H
#define CRPRODUCER_CRCAMERAGROUP_H 1


#include <CRProducer/Producer/CameraGroup>

#include <CRCore/crTimer.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crGroup.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crFrameStamp.h>
#include <CRCore/crDisplaySettings.h>
#include <CRCore/crMatrixd.h>
#include <CRCore/crCullSettings.h>
#include <CRProducer/crGraphicsContextImplementation.h>
#include <CRUtil/crSceneView.h>

namespace CRProducer {
class crSceneHandler;
class CRPRODUCER_EXPORT crCameraGroup : public Producer::CameraGroup
{
    public :

        crCameraGroup();
        crCameraGroup(Producer::CameraConfig *cfg);

        virtual ~crCameraGroup();

		typedef std::vector< CRCore::ref_ptr<crGraphicsContextImplementation> > GraphicsContextList;

		void setGraphicsContextList(GraphicsContextList& gcList);
		GraphicsContextList& getGraphicsContextList();
		const GraphicsContextList& getGraphicsContextList() const;

        typedef std::vector < Producer::ref_ptr<CRProducer::crSceneHandler> > SceneHandlerList;
        inline SceneHandlerList& getSceneHandlerList() { return m_shvec;}
        inline const SceneHandlerList& getSceneHandlerList() const { return m_shvec;}
		inline CRProducer::crSceneHandler* getDefaultSceneHandler() { return m_shvec.empty()?NULL:m_shvec[0].get();}

        void setSceneData( CRCore::crNode *scene );
        inline CRCore::crNode *getSceneData() { return m_scene_data.get(); }
        inline const CRCore::crNode *getSceneData() const { return m_scene_data.get(); }

        void setSceneDecorator( CRCore::crGroup* decorator);
        CRCore::crGroup* getSceneDecorator();
        const CRCore::crGroup* getSceneDecorator() const;

        inline CRCore::crNode* getTopMostSceneData()
		{
			if (m_scene_decorator.valid())
				return m_scene_decorator.get();
			else
				return m_scene_data.get(); 
		}
        inline const CRCore::crNode* getTopMostSceneData() const
		{
			if (m_scene_decorator.valid())
				return m_scene_decorator.get();
			else
				return m_scene_data.get(); 
		}

		/** Update internal structures w.r.t updated scene data.*/
		virtual void updatedSceneData();
        
        void setDisplaySettings( CRCore::crDisplaySettings *ds );
        CRCore::crDisplaySettings *getDisplaySettings();
        const CRCore::crDisplaySettings *getDisplaySettings() const;
        
		void setCullSettings( const CRCore::crCullSettings& cs);
		CRCore::crCullSettings& getCullSettings();
		const CRCore::crCullSettings& getCullSettings() const;
        
        //void setFrameStamp( CRCore::crFrameStamp* fs );
        //inline CRCore::crFrameStamp *getFrameStamp() { return m_frameStamp.get(); }

        void setGlobalStateSet( CRCore::crStateSet *sset );
        CRCore::crStateSet *getGlobalStateSet();
        const CRCore::crStateSet *getGlobalStateSet() const;

        void setClearColor( const CRCore::crVector4& clearColor );
        CRCore::crVector4f& getClearColor();
        const CRCore::crVector4f& getClearColor() const;

        void setLODScale( float scale );
        float getLODScale() const;

        void setFusionDistance( CRUtil::crSceneView::FusionDistanceMode mode,float value=1.0f);
		/** Set the options to set up SceneView with, see osgUtil::SceneView::Options for available options.*/
		void setRealizeSceneViewOptions(unsigned int options);
		unsigned int getRealizeSceneViewOptions();

        /** RealizeCallback class one should override to provide an the implemention of realize callbacks.
          * Note, this callback overrides the normal call to crSceneHandler::init() so it become the your
          * responisibility to call this within your callback if required, it is a safe assumption to
          * always call crSceneHandler::init() within your callback..*/
        class CRPRODUCER_EXPORT RealizeCallback : public CRCore::Referenced
        {
        public:
            virtual void operator()( crCameraGroup& cg, crSceneHandler& sh, const Producer::RenderSurface & rs) = 0;
            
        protected:
            virtual ~RealizeCallback() {}
        };

        /** Set the realize callback to use when once the render surfaces are realized.*/
        void setRealizeCallback( RealizeCallback* cb);

        /** Get the realize callback.*/
        RealizeCallback* getRealizeCallback();
        
        /** Get the const realize callback.*/
        const RealizeCallback* getRealizeCallback() const;

        void advance();

        /** Set the threading model and then call realize().*/
        virtual bool realize(ThreadingModel thread_model,Producer::Window parentWnd,Producer::Window renderWnd);

        /** Realize the render surfaces (OpenGL graphics) and various threads, and call any realize callbacks.*/
        virtual bool realize(Producer::Window parentWnd,Producer::Window renderWnd);

        /** Set the model view matrix of the camera group,
          * by individually set all the camera groups's camera.*/
        virtual void setView(const CRCore::crMatrixd& matrix);
        
        /** Get the model view martrix of the camera group,
          * taking its value for camera 0.*/
        CRCore::crMatrixd getViewMatrix() const;
        CRCore::crMatrixd getProjectionMatrix();

        virtual void sync();

        /** Dispatch the cull_update  for each of the Camera's for this frame.*/
		inline virtual void cull_update()
		{
			//CRCore::crNode* node = getTopMostSceneData();
			//if (node) node->getBound();
			CameraGroup::cull_update();
			//CRCore::crBrain::getInstance()->gameLoop(0);
			//m_frameStamp->setFrameNumber(m_frameNumber++);
			//m_frameStamp->setReferenceTime(m_timer.delta_s(m_start_tick,m_timer.tick()));
			//m_frameStamp->update();
		}
		/** Dispatch the draw  for each of the Camera's for this frame.*/
		inline virtual void draw()
		{
			//CRCore::crBrain::getInstance()->gameLoop(1);
			CameraGroup::draw();
		}
		/** Dispatch the cull and draw for each of the Camera's for this frame.*/
        virtual void frame();

		virtual void cleanup_frame();
		//inline float getUIMouseOffset()const { return m_uiMouseOffset; }
		inline bool isInited(){ return m_initialized; }
    protected :

        void setUpSceneViewsWithData();

        CRCore::ref_ptr<CRCore::crNode>               m_scene_data;
        CRCore::ref_ptr<CRCore::crGroup>              m_scene_decorator;

        //CRCore::ref_ptr<CRCore::crStateSet>           m_global_stateset;
        CRCore::crVector4f                             m_clear_color;
       // float                                         m_LODScale;
        
		CRUtil::crSceneView::FusionDistanceMode       m_fusionDistanceMode;
        float                                         m_fusionDistanceValue;
         
		unsigned int                                  m_realizeSceneViewOptions;

		GraphicsContextList                           m_gcList;
		SceneHandlerList                              m_shvec;
        
        CRCore::ref_ptr<RealizeCallback>              m_realizeCallback;

		CRCore::crCullSettings                        m_cullSettings;
        
        CRCore::ref_ptr<CRCore::crDisplaySettings>    m_ds;
        bool                                          m_initialized;

        //unsigned int                                  m_frameNumber;
        CRCore::Timer                                 m_timer;
        CRCore::Timer_t                               m_start_tick;
        //CRCore::ref_ptr<CRCore::crFrameStamp>         m_frameStamp;

        void _init();
		//float                                         m_uiMouseOffset;
};

}

#endif
