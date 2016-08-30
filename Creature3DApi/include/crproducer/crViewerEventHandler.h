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
#ifndef CRRODUCER_CRVIEWEREVENTHANDLER
#define CRPRODUCER_CRVIEWEREVENTHANDLER 1

#include <CRGA/crGUIEventHandler.h>
#include <CRProducer/crViewer.h>

#include <CRGLText/crText.h>

namespace CRProducer {
 
class CRPRODUCER_EXPORT crViewerEventHandler : public CRGA::crGUIEventHandler
{
    public:
    
        crViewerEventHandler(crCameraGroup* cg);
    
        virtual bool handle(const CRGA::crGUIEventAdapter& ea,CRGA::crGUIActionAdapter& aa);

        virtual void accept(CRGA::crGUIEventHandlerVisitor& gehv);

        /** Get the keyboard and mouse usage of this manipulator.*/
        virtual void getUsage(CRCore::crApplicationUsage& usage) const;
        
        crCameraGroup* getCameraGroup() { return m_cg; }
        const crCameraGroup* getCameraGroup() const { return m_cg; }
        
        
        void setWriteNodeFileName(const std::string& filename) { m_writeNodeFileName = filename; }
        const std::string& getWriteNodeFileName() const { return m_writeNodeFileName; }
        
        
        void setDisplayHelp(bool displayHelp) { m_displayHelp = displayHelp; }
        
        bool getDisplayHelp() const { return m_displayHelp; }
		
		void setDisplayLogo(bool displayLogo) { m_displayLogo = displayLogo; }

		bool getDisplayLogo() const { return m_displayLogo; }

		void setDisplayPost(bool displayPost) { m_displayPost = displayPost; }

		bool getDisplayPost() const { return m_displayPost; }

        
        enum FrameStatsMode
        {
            NO_STATS = 0,
            FRAME_RATE = 1,
            CAMERA_STATS = 2
        };
        
        void setFrameStatsMode(FrameStatsMode mode) { m_frameStatsMode = mode; }
        FrameStatsMode getFrameStatsMode() { return m_frameStatsMode; }

		class CRPRODUCER_EXPORT SnapImageDrawCallback : public Producer::Camera::Callback
		{
		public:

	        SnapImageDrawCallback(const std::string& filename):
			    m_filename(filename),
				m_snapImageOnNextFrame(false)
			{
			}

			void setSnapImageOnNextFrame(bool flag) { m_snapImageOnNextFrame = flag; }
			bool getSnapImageOnNextFrame() const { return m_snapImageOnNextFrame; }
             
			void setFileName(const std::string& filename){ m_filename = filename; }

			virtual void operator()( const Producer::Camera & camera);
			static void setIndex( int index ){ m_index = index; }
		protected:

			std::string m_filename;
			bool        m_snapImageOnNextFrame;
			static int m_index;
		};

		class CRPRODUCER_EXPORT StatsAndHelpDrawCallback : public Producer::CameraGroup::StatsHandler, public Producer::Camera::Callback
		{
		public:

		    StatsAndHelpDrawCallback(crViewerEventHandler* veh, unsigned int cameraNumber);

		    virtual void operator() (const Producer::CameraGroup &cg );

			virtual void operator()( const Producer::Camera & camera);

		protected:

			crViewerEventHandler* m_veh;
			unsigned int m_cameraNumber;

			CRCore::ref_ptr<CRCore::crStateSet> m_stateset;
			CRCore::ref_ptr<CRCore::crViewPort> m_viewport;
			CRCore::ref_ptr<CRCore::RefMatrix> m_projection;
			CRCore::ref_ptr<CRCore::RefMatrix> m_modelview;

			// help related methods and data
			void displayHelp();
			void createHelpText();
			void displayLogo();
			void createLogoText();
			void displayPost();

			typedef std::vector< CRCore::ref_ptr<CRGLText::crText> > TextList;
			bool        m_helpInitialized;
			bool        m_logoInitialized;
			TextList    m_descriptionList;
			TextList    m_optionList;
			TextList    m_explanationList;
            TextList    m_logoTextList;
			// stats related methods and data.
			void displayStats();
			void createStatsText();

			typedef std::vector<double> CameraTimes;

			bool                        m_statsInitialized;
			CRCore::ref_ptr<CRGLText::crText> m_frameRateLabelText;
			CRCore::ref_ptr<CRGLText::crText> m_frameRateCounterText;

			CRCore::ref_ptr<CRGLText::crText> m_stateLabelText;
			CRCore::ref_ptr<CRGLText::crText> m_stateCounterText;
			CRCore::ref_ptr<CRGLText::crText> m_drawableLabelText;
			CRCore::ref_ptr<CRGLText::crText> m_drawableCounterText;
			CRCore::ref_ptr<CRGLText::crText> m_vertexLabelText;
			CRCore::ref_ptr<CRGLText::crText> m_vertexCounterText;
			CRCore::ref_ptr<CRGLText::crText> m_primLabelText;
			CRCore::ref_ptr<CRGLText::crText> m_primCounterText;

			TextList                    m_statsLabelList;
			CRCore::ref_ptr<CRGLText::crText> m_frameTimeText;
			//CameraTimes                 m_cullTimes;
			//TextList                    m_cullTimeText;
			CameraTimes                 m_referDrawTimes;
			TextList                    m_referDrawTimeText;
			CameraTimes                 m_drawTimes;
			TextList                    m_drawTimeText;

			// info related methods and data.
			void displayInfo();
			void createInfoText();

			bool                        m_infoInitialized;
			TextList                    m_infoLabelList;
			CRCore::ref_ptr<CRGLText::crText> m_positionText;
			CRCore::ref_ptr<CRGLText::crText> m_orientationText;
			CRCore::ref_ptr<CRGLText::crText> m_speedText;

			Producer::CameraGroup::FrameStats m_fs[2];
			//unsigned int m_index;

		};

        typedef std::vector<SnapImageDrawCallback*> SnapImageDrawCallbackList;
		SnapImageDrawCallbackList &getSnapImageDrawCallbackList(){ return m_snapImageDrawCallbackList; }

		StatsAndHelpDrawCallback *getStatsAndHelpDrawCallback(){ return m_statsAndHelpDrawCallback; }

		void setWriteImageOnNextFrame(bool writeImageOnNextFrame);
		void setWriteImageFileName(const std::string& filename);
		const std::string& getWriteImageFileName() const { return m_writeImageFileName; }

//	    virtual void pick(const CRGA::crGUIEventAdapter& ea);

    protected:
        
        CRProducer::crCameraGroup* m_cg;
        
        std::string m_writeNodeFileName;
        
        bool            m_displayHelp;
		bool            m_displayLogo;
        FrameStatsMode  m_frameStatsMode;
        
        bool            m_firstTimeTogglingFullScreen;
        
        StatsAndHelpDrawCallback* m_statsAndHelpDrawCallback;

        SnapImageDrawCallbackList m_snapImageDrawCallbackList;

	    std::string                 m_writeImageFileName;

		bool m_displayPost;
};

}

#endif
