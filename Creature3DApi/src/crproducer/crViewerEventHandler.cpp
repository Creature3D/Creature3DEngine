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
//Modified by 吴财华
#include <CRProducer/crViewerEventHandler.h>
#include <CRGA/crAnimationPathManipulator.h>
#include <CRIOManager/crWriteFile.h>

#include <CRCore/crBlendFunc.h>
#include <CRRender/crOglBlendFuncDraw.h>
#include <CRUtil/crStatistics.h>
#include <CRIOManager/crFileNameUtils.h>
#include <algorithm>

#if defined(__sgi)
#include <ctype.h>
#elif defined(__GNUC__) || !defined(WIN32) || defined(__MWERKS__)
#include <cctype>
using std::tolower;
using std::strlen;    
#endif

using namespace CRCore;
using namespace CRProducer;
using namespace CRRender;
using namespace CRUtil;

int crViewerEventHandler::SnapImageDrawCallback::m_index = 1;
void crViewerEventHandler::SnapImageDrawCallback::operator()( const Producer::Camera & camera)
{
    if (!m_snapImageOnNextFrame) return;
    
    int x,y;
    unsigned int width,height;
    camera.getProjectionRectangle(x,y,width,height);

    CRCore::ref_ptr<CRCore::crImage> image = new CRCore::crImage;
    image->readPixels(x,y,width,height,
                        GL_RGB,GL_UNSIGNED_BYTE);
    
    char filename[256];
    sprintf(filename,"output/photos/image_%d.jpg",m_index++);
    m_filename = filename;
    CRIOManager::writeImageFile(*image,m_filename);
    m_snapImageOnNextFrame = false;
}

crViewerEventHandler::StatsAndHelpDrawCallback::StatsAndHelpDrawCallback(crViewerEventHandler* veh, unsigned int cameraNumber):
        m_veh(veh),
        m_cameraNumber(cameraNumber),
        m_helpInitialized(false),
        m_statsInitialized(false),
        m_infoInitialized(false),
		m_logoInitialized(false)
{
	//m_fs.resize(10);
	//m_index = 0;
    
    m_veh->getCameraGroup()->setStatsHandler(this);
    
    m_stateset = new CRCore::crStateSet;
    m_viewport = new CRCore::crViewPort(0,0,1280,1024);
    m_stateset->setAttribute(m_viewport.get());
    crBlendFunc *blendFunc = new CRCore::crBlendFunc;
	blendFunc->setDrawCallback(crOglBlendFuncDraw::getInstance());
    m_stateset->setAttribute(blendFunc);
    m_stateset->setMode(CRCore::crStateAttribute::VRMode(GL_DEPTH_TEST,NULL),CRCore::crStateAttribute::OFF);
    m_stateset->setMode(CRCore::crStateAttribute::VRMode(GL_BLEND,NULL),CRCore::crStateAttribute::ON);
	m_stateset->setMode(CRCore::crStateAttribute::VRMode(GL_MULTISAMPLE_ARB,NULL),CRCore::crStateAttribute::OFF);

    m_projection = new CRCore::RefMatrix(CRCore::crMatrix::ortho2D(0.0,1280,0,1024));
    m_modelview = new CRCore::RefMatrix();

/*     createHelpText();
    createStatsText();
    
    m_helpInitialized = false;*/

}

void crViewerEventHandler::StatsAndHelpDrawCallback::operator() (const Producer::CameraGroup &cg )
{
	m_fs[0] = m_fs[1];
	m_fs[1] = cg.getFrameStats();

    //m_index = (m_index + 1) % m_fs.size();
	//m_fs[m_index] = cg.getFrameStats();     
}

void crViewerEventHandler::StatsAndHelpDrawCallback::operator()( const Producer::Camera & camera)
{
    if (m_veh->getDisplayHelp()==false && m_veh->getDisplayLogo()==false &&
        m_veh->getFrameStatsMode()==crViewerEventHandler::NO_STATS &&!m_veh->getDisplayPost()) return;

    int x,y;
    unsigned int width,height;
    camera.getProjectionRectangle(x,y,width,height);
    m_viewport->setViewport(x,y,width,height);

    crSceneHandler* osh = m_veh->getCameraGroup()->getSceneHandlerList()[m_cameraNumber].get();
    CRUtil::crSceneView* sv = osh->getSceneView();
    CRCore::crState& state = *(sv->getCurrentRenderState());
    
    
    state.applyProjectionMatrix(m_projection.get());
    state.applyModelViewMatrix(m_modelview.get());
    
    state.pushStateSet(m_stateset.get());
    state.apply();

    if (m_veh->getFrameStatsMode()!=crViewerEventHandler::NO_STATS)
    {
		//camera.setInstrumentationMode(true);
        displayStats();
    }
    
    if (m_veh->getDisplayHelp())
    {
        displayHelp();
    }
       
    if (m_veh->getDisplayLogo())
    {
        displayLogo();
    }

	if (m_veh->getDisplayPost())
	{
		displayPost();
	}
    
    state.popStateSet();
    
    //state.apply();

}

void crViewerEventHandler::StatsAndHelpDrawCallback::displayHelp()
{
  /*  if (!m_helpInitialized) createHelpText();

    crSceneHandler* osh = m_veh->getCameraGroup()->getSceneHandlerList()[m_cameraNumber].get();
    CRUtil::crSceneView* sv = osh->getSceneView();


    for(TextList::iterator ditr=m_descriptionList.begin();
        ditr!=m_descriptionList.end();
        ++ditr)
    {
        (*ditr)->draw(*(sv->getState()));
    }

    for(TextList::iterator oitr=m_optionList.begin();
        oitr!=m_optionList.end();
        ++oitr)
    {
        (*oitr)->draw(*(sv->getState()));
    }

    for(TextList::iterator eitr=m_explanationList.begin();
        eitr!=m_explanationList.end();
        ++eitr)
    {
        (*eitr)->draw(*(sv->getState()));
    }*/
}

void crViewerEventHandler::StatsAndHelpDrawCallback::displayPost()
{
	///////绘制鼠标指针，在屏幕正中
	crSceneHandler* osh = m_veh->getCameraGroup()->getSceneHandlerList()[m_cameraNumber].get();
	CRUtil::crSceneView* sv = osh->getSceneView();
	sv->getCurrentRenderState()->applyMode(GL_BLEND,false);
	//glDisable(GL_BLEND);
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	glOrtho( 0, 800, 0, 600, -1.0, 1.0 ); 

	glColor4f( 0.0, 1.0, 0.0, 1.0 );
	glLineWidth(1.0f);
	glBegin( GL_LINES );
	    glVertex2d( 405,300 );//--
		glVertex2d( 402,300 );//

		glVertex2d( 398,300 );//--
		glVertex2d( 395,300 );//

		glVertex2d( 400,295 );//|
		glVertex2d( 400,298 );//

		glVertex2d( 400,302 );//|
		glVertex2d( 400,305 );//
	glEnd();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
}

void crViewerEventHandler::StatsAndHelpDrawCallback::displayLogo()
{
	if (!m_logoInitialized) createLogoText();

	crSceneHandler* osh = m_veh->getCameraGroup()->getSceneHandlerList()[m_cameraNumber].get();
	CRUtil::crSceneView* sv = osh->getSceneView();

	for(TextList::iterator itr=m_logoTextList.begin();
		itr!=m_logoTextList.end();
		++itr)
	{
		(*itr)->draw(*(sv->getCurrentRenderState()));
	}   
 //   
}

void crViewerEventHandler::StatsAndHelpDrawCallback::createLogoText()
{
	float leftPos = 10.0f;
	float characterSize = 20.0f;
	CRCore::crVector3 pos(leftPos,140.0f,0.0f);
	CRCore::crVector4 color(1.0f,1.0f,0.0f,1.0f);

	{
		CRGLText::crText* text = new CRGLText::crText;
		text->setFont("fonts/arial.ttf");
		text->setColor(color);
		text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
		text->setCharacterSize(characterSize);
		text->setPosition(pos);
		text->setAlignment(CRGLText::crText::BASE_LINE);
		text->setText("==========================");
		//text->setText("Copyright(C) 2005 wucaihua, All Rights Reserved. E-mail:caitht@163.com, Msn:caitht@hotmail.com");
		m_logoTextList.push_back(text);

		pos.y()-=characterSize;
        text = new CRGLText::crText;
		text->setFont("fonts/arial.ttf");
		text->setColor(color);
		text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
		text->setCharacterSize(characterSize);
		text->setPosition(pos);
		text->setAlignment(CRGLText::crText::BASE_LINE);
		text->setText("Copyright(C) 2005 wucaihua");
		m_logoTextList.push_back(text);

		pos.y()-=characterSize;
		text = new CRGLText::crText;
		text->setFont("fonts/arial.ttf");
		text->setColor(color);
		text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
		text->setCharacterSize(characterSize);
		text->setPosition(pos);
		text->setAlignment(CRGLText::crText::BASE_LINE);
		text->setText("All Rights Reserved");
		m_logoTextList.push_back(text);

		pos.y()-=characterSize;
		text = new CRGLText::crText;
		text->setFont("fonts/arial.ttf");
		text->setColor(color);
		text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
		text->setCharacterSize(characterSize);
		text->setPosition(pos);
		text->setAlignment(CRGLText::crText::BASE_LINE);
		text->setText("Tel:13185059986");
		m_logoTextList.push_back(text);

		pos.y()-=characterSize;
		text = new CRGLText::crText;
		text->setFont("fonts/arial.ttf");
		text->setColor(color);
		text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
		text->setCharacterSize(characterSize);
		text->setPosition(pos);
		text->setAlignment(CRGLText::crText::BASE_LINE);
		text->setText("E-mail:caitht@163.com");
		m_logoTextList.push_back(text);
		
		pos.y()-=characterSize;
		text = new CRGLText::crText;
		text->setFont("fonts/arial.ttf");
		text->setColor(color);
		text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
		text->setCharacterSize(characterSize);
		text->setPosition(pos);
		text->setAlignment(CRGLText::crText::BASE_LINE);
		text->setText("Msn:caitht@hotmail.com");
		m_logoTextList.push_back(text);

		pos.y()-=characterSize;
		text = new CRGLText::crText;
		text->setFont("fonts/arial.ttf");
		text->setColor(color);
		text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
		text->setCharacterSize(characterSize);
		text->setPosition(pos);
		text->setAlignment(CRGLText::crText::BASE_LINE);
		text->setText("==========================");
		m_logoTextList.push_back(text);
	}
	m_logoInitialized = true;
}

void crViewerEventHandler::StatsAndHelpDrawCallback::createHelpText()
{

    //crCameraGroup* ocg = m_veh->getCameraGroup();
    //if (ocg->getApplicationUsage())
    //{

    //    const CRCore::crApplicationUsage::UsageMap& um = ocg->getApplicationUsage()->getKeyboardMouseBindings();

    //    float maxWidthOfDisplayRegion = 1200.0f;
    //    float bottomOfDescription = 1000.0f;
    //    CRCore::crVector3 posDescription(0.0f,bottomOfDescription,0.0f);
    //    CRCore::crVector4 colorDescription(1.0f,1.0f,0.0f,1.0f);
    //    float characterSize = 20.0f;

    //    if (!(ocg->getApplicationUsage()->getDescription()).empty())
    //    {
    //        CRGLText::crText* text = new CRGLText::crText;
    //        text->setFont("fonts/arial.ttf");
    //        text->setColor(colorDescription);
    //        text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
    //        text->setCharacterSize(characterSize);
    //        text->setPosition(posDescription);
    //        text->setMaximumWidth(maxWidthOfDisplayRegion);
    //        text->setAlignment(CRGLText::crText::BASE_LINE);
    //        text->setText(ocg->getApplicationUsage()->getDescription());

    //        bottomOfDescription = text->getBound().yMin()-characterSize*2.0f;

    //        m_descriptionList.push_back(text);

    //    }

    //    CRCore::crVector3 posOption(0.0f,bottomOfDescription,0.0f);
    //    CRCore::crVector4 colorOption(1.0f,1.0f,0.0f,1.0f);
    //    float maxX = 0.0f;

    //    // create option strings.
    //    CRCore::crApplicationUsage::UsageMap::const_iterator citr;
    //    for(citr=um.begin();
    //        citr!=um.end();
    //        ++citr)
    //    {
    //        CRGLText::crText* text = new CRGLText::crText;
    //        text->setFont("fonts/arial.ttf");
    //        text->setColor(colorOption);
    //        text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
    //        text->setCharacterSize(characterSize);
    //        text->setPosition(posOption);
    //        text->setAlignment(CRGLText::crText::BASE_LINE);
    //        text->setText(citr->first);

    //        if (text->getBound().xMax()>maxX) maxX=text->getBound().xMax();

    //        m_optionList.push_back(text);

    //    }

    //    CRCore::crVector3 posExplanation(maxX+characterSize,bottomOfDescription,0.0f);
    //    CRCore::crVector4 colorExplanation(1.0f,1.0f,0.0f,1.0f);
    //    float maxWidth = maxWidthOfDisplayRegion-maxX;

    //    TextList::iterator oitr;
    //    TextList::iterator eitr;
    //    TextList::iterator ditr;

    //    for(citr=um.begin(), oitr=m_optionList.begin();
    //        citr!=um.end();
    //        ++citr,++oitr)
    //    {
    //        CRGLText::crText* text = new CRGLText::crText;
    //        text->setFont("fonts/arial.ttf");
    //        text->setColor(colorExplanation);
    //        text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
    //        text->setCharacterSize(characterSize);
    //        text->setPosition(posExplanation);
    //        text->setMaximumWidth(maxWidth);
    //        text->setAlignment(CRGLText::crText::BASE_LINE);
    //        text->setText(citr->second);

    //        if (text->getBound().xMax()>maxX) maxX=text->getBound().xMax();

    //        // fix the position of option text to be the same height as the examplanation.
    //        CRCore::crVector3 pos((*oitr)->getPosition());
    //        (*oitr)->setPosition(CRCore::crVector3(pos.x(),posExplanation.y(),pos.z()));

    //        posExplanation.y() = text->getBound().yMin()-characterSize;

    //        m_explanationList.push_back(text);

    //    }

    //  //   compute the boundings of the all the text.
    //    CRCore::crBoundingBox bb;
    //    for(ditr=m_descriptionList.begin();
    //        ditr!=m_descriptionList.end();
    //        ++ditr)
    //    {
    //        bb.expandBy((*ditr)->getBound());
    //    }

    //    for(oitr=m_optionList.begin();
    //        oitr!=m_optionList.end();
    //        ++oitr)
    //    {
    //        bb.expandBy((*oitr)->getBound());
    //    }

    //    for(eitr=m_explanationList.begin();
    //        eitr!=m_explanationList.end();
    //        ++eitr)
    //    {
    //        bb.expandBy((*eitr)->getBound());
    //    }

    //    float totalWidth = bb.xMax()-bb.xMin();
    //    float totalHeight = bb.yMax()-bb.yMin();
    //    float widthMargin = (1280.0f-totalWidth)*0.5f;
    //    float heightMargin = (1024.0f-totalHeight)*0.5f;

    //    CRCore::crVector3 delta(widthMargin-bb.xMin(),heightMargin-bb.yMin(),0.0f);

    //    // shift the text to center it.
    //    for(ditr=m_descriptionList.begin();
    //        ditr!=m_descriptionList.end();
    //        ++ditr)
    //    {
    //        (*ditr)->setPosition((*ditr)->getPosition()+delta);
    //    }

    //    for(oitr=m_optionList.begin();
    //        oitr!=m_optionList.end();
    //        ++oitr)
    //    {
    //        (*oitr)->setPosition((*oitr)->getPosition()+delta);
    //    }

    //    for(eitr=m_explanationList.begin();
    //        eitr!=m_explanationList.end();
    //        ++eitr)
    //    {
    //        (*eitr)->setPosition((*eitr)->getPosition()+delta);
    //    }


    //}
    //m_helpInitialized = true;
}

void crViewerEventHandler::StatsAndHelpDrawCallback::displayStats()
{
    if (!m_statsInitialized) createStatsText();

    crSceneHandler* osh = m_veh->getCameraGroup()->getSceneHandlerList()[m_cameraNumber].get();
    CRUtil::crSceneView* sv = osh->getSceneView();

    // render graphs
    //if (m_veh->getFrameStatsMode()>=crViewerEventHandler::CAMERA_STATS)
    //{

    //    // Set up the Orthographic view
    //    glMatrixMode( GL_PROJECTION );
    //    glPushMatrix();
    //    glLoadIdentity();
    //    glOrtho( -.025, .128, 600.0, -10.0, -1.0, 1.0 ); 


    //    unsigned int lindex = (m_index + 1) % m_fs.size();
    //    Producer::Camera::TimeStamp zero = m_fs[lindex]._startOfFrame;
    //    unsigned int i;
    //    double x1=0.0, x2=0.0, y1=0.0, y2=0.0;
    //    for(unsigned int frame = 0; frame < m_fs.size(); frame++ )
    //    {
	   // Producer::CameraGroup::FrameStats fs = m_fs[(lindex + frame) % m_fs.size()];
	   // y1 = 0.0;
   	//    y2 = y1 + 10;
	   // x1 = fs._startOfUpdate - zero;
	   // x2 = fs._endOfUpdate   - zero;

	   // glBegin( GL_QUADS );

	   // // Draw Update length
 	  //  glColor4f( 0.0, 1.0, 0.0, 0.5 );
	   // glVertex2d( x1, y1);
	   // glVertex2d( x2, y1);
	   // glVertex2d( x2, y2);
	   // glVertex2d( x1, y2);

	   // for( i = 0; i < fs.getNumFrameTimeStampSets(); i++ )
    //        {
	   //     Producer::Camera::FrameTimeStampSet fts = fs.getFrameTimeStampSet(i);
	   //     y1 += 13.0;
	   //     y2 = y1 + 10.0;
	   //     x1 = fts[Producer::Camera::BeginCull] - zero;
	   //     x2 = fts[Producer::Camera::EndCull]   - zero;

 	  //      glColor4f( 0.0, 1.0, 1.0, 0.5 );
	   //     glVertex2d( x1, y1);
	   //     glVertex2d( x2, y1);
	   //     glVertex2d( x2, y2);
	   //     glVertex2d( x1, y2);

	   //     x1 = fts[Producer::Camera::BeginDraw] - zero;
	   //     x2 = fts[Producer::Camera::EndDraw]   - zero;

 	  //      glColor4f( 1.0, 1.0, 0.0, 0.5 );
	   //     glVertex2d( x1, y1);
	   //     glVertex2d( x2, y1);
	   //     glVertex2d( x2, y2);
	   //     glVertex2d( x1, y2);

    //        }
	   // glEnd();

	   // glBegin( GL_LINES );
	   // glColor4f( 1, 1, 1, 0.5 );
	   // glVertex2d( fs._startOfFrame - zero , 0.0 );
    //        y1 = fs.getNumFrameTimeStampSets() * 13.0 + 10.0;
    //        glVertex2d( fs._startOfFrame - zero, y1 );

    //        y1 = 12.5; 
	   // for( i = 0; i < fs.getNumFrameTimeStampSets(); i++ )
    //        {
	   //     y2 = y1 + 11; 
	   //     Producer::Camera::FrameTimeStampSet fts = fs.getFrameTimeStampSet(i);
	   //     Producer::Camera::TimeStamp vsync = fts[Producer::Camera::Vsync];
	   //     double x1 = vsync - zero;
    //            glColor4f( 1.0, 1.0, 0.0, 0.5 );
	   //     glVertex2d( x1, y1 );
    //            glVertex2d( x1, y2 );
 	  //      y1 += 13.0;
  	 //   }
	   // glEnd();	
    //    }

    //    glBegin( GL_LINES );

    //    glColor4f( 1, 1, 1, 0.5 );
    //    for( i = 0; i < 128; i++ )
    //    {
	   // glVertex2d((GLdouble)i*.001, y1);

    //       if( !(i%10) )
	   //    glVertex2d((GLdouble)i*.001, y1 - 5.0);
	   // else if( !(i%5) )
	   //     glVertex2d((GLdouble)i*.001, y1 - 3.0);
	   // else
	   //     glVertex2d((GLdouble)i*.001, y1 - 1.0);
    //    }

    //    glEnd();

    //    glMatrixMode( GL_PROJECTION );
    //    glPopMatrix();
    //    glMatrixMode( GL_MODELVIEW );
    //    
    //}

    // render the text
    if (m_veh->getFrameStatsMode()>=crViewerEventHandler::FRAME_RATE)
    {
        // update and draw the frame rate text.
        
        char tmpText[128];

        m_frameRateLabelText->draw(*(sv->getCurrentRenderState()));

        //if (m_fs.size()>1)
        //{
        //    unsigned int lindex = (m_index + 1) % m_fs.size();
        //    double timeForFrames = (m_fs[m_index]._startOfFrame-m_fs[lindex]._startOfFrame);
        //    double timePerFrame = timeForFrames/(double)(m_fs.size()-1);
        //    sprintf(tmpText,"%4.2f\0",1.0/timePerFrame);
        //    m_frameRateCounterText->setText(tmpText);
        //}
		double timeForFrames = (m_fs[1]._startOfFrame-m_fs[0]._startOfFrame);
		double timePerFrame = timeForFrames;
		sprintf(tmpText,"%4.2f\0",1.0/timePerFrame);
		m_frameRateCounterText->setText(tmpText);

        m_frameRateCounterText->draw(*(sv->getCurrentRenderState()));

        if (m_veh->getFrameStatsMode()>=crViewerEventHandler::CAMERA_STATS)
        {
			double timeForFrames = (m_fs[1]._startOfFrame-m_fs[0]._startOfFrame);
			double timePerFrame = timeForFrames;
			sprintf(tmpText,"%4.2f\0",1000.0 * timePerFrame);
			m_frameTimeText->setText(tmpText);

		    std::fill(m_drawTimes.begin(),m_drawTimes.end(),0.0);
			std::fill(m_referDrawTimes.begin(),m_referDrawTimes.end(),0.0);

			Producer::Camera::FrameTimeStampSet fts0,fts1; 
			for(unsigned int i = 0; i < m_fs[0].getNumFrameTimeStampSets(); i++ )
			{
                fts0 = m_fs[0].getFrameTimeStampSet(i);
                fts1 = m_fs[1].getFrameTimeStampSet(i);
				m_drawTimes[i] += fts1[Producer::Camera::BeginCameraFrame] - fts0[Producer::Camera::BeginCameraFrame];
			    m_referDrawTimes[i] += fts0[Producer::Camera::EndCameraFrame]-fts0[Producer::Camera::BeginCameraFrame];
			}

			m_frameTimeText->draw(*(sv->getCurrentRenderState()));

			TextList::iterator itr;
			CameraTimes::iterator titr;
			for(itr=m_drawTimeText.begin(),titr = m_drawTimes.begin();
				itr!=m_drawTimeText.end() && titr!=m_drawTimes.end();
				++itr,++titr)
			{
				sprintf(tmpText,"%4.2f\0",1000.0*(*titr));
				(*itr)->setText(tmpText);
				(*itr)->draw(*(sv->getCurrentRenderState()));
			}

			for(itr=m_referDrawTimeText.begin(),titr = m_referDrawTimes.begin();
			    itr!=m_referDrawTimeText.end() && titr!=m_referDrawTimes.end();
			    ++itr,++titr)
			{
			    sprintf(tmpText,"%4.2f\0",100.0*(*titr));
			    (*itr)->setText(tmpText);
			    (*itr)->draw(*(sv->getCurrentRenderState()));
			}

            for(itr=m_statsLabelList.begin();
                itr!=m_statsLabelList.end();
                ++itr)
            {
                (*itr)->draw(*(sv->getCurrentRenderState()));
            }

           /* double updateTime = 0.0;*/
     //       std::fill(m_cullTimes.begin(),m_cullTimes.end(),0.0);
     //       std::fill(m_drawTimes.begin(),m_drawTimes.end(),0.0);

	//		Producer::CameraGroup::FrameStats fs;
     //       for(unsigned int frame = 0; frame < m_fs.size(); frame++ )
     //       {
	    //        fs = m_fs[frame];
     ///*           updateTime += (fs._endOfUpdate-fs._startOfUpdate);*/

	    //        for(unsigned int i = 0; i < fs.getNumFrameTimeStampSets(); i++ )
     //           {
	    //            Producer::Camera::FrameTimeStampSet fts = fs.getFrameTimeStampSet(i);

	    //            /*m_cullTimes[i] += fts[Producer::Camera::EndCull]-fts[Producer::Camera::BeginCull];*/
	    //            m_drawTimes[i] += fts[Producer::Camera::EndDraw]-fts[Producer::Camera::BeginDraw];
     //           }
     //       }

      /*      sprintf(tmpText,"%4.2f\0",1000.0*updateTime/(double)m_fs.size());
            m_frameTimeText->setText(tmpText);*/

         /*   m_frameTimeText->draw(*(sv->getState()));*/

           // CameraTimes::iterator titr;
            //for(itr=m_cullTimeText.begin(),titr = m_cullTimes.begin();
            //    itr!=m_cullTimeText.end() && titr!=m_cullTimes.end();
            //    ++itr,++titr)
            //{
            //    sprintf(tmpText,"%4.2f\0",100.0*(*titr)/(double)m_fs.size());
            //    (*itr)->setText(tmpText);
            //    (*itr)->draw(*(sv->getCurrentRenderState()));
            //}
            //for(itr=m_drawTimeText.begin(),titr = m_drawTimes.begin();
            //    itr!=m_drawTimeText.end() && titr!=m_drawTimes.end();
            //    ++itr,++titr)
            //{
            //    sprintf(tmpText,"%4.2f\0",1000.0*(*titr)/(double)m_fs.size());
            //    (*itr)->setText(tmpText);
            //    (*itr)->draw(*(sv->getCurrentRenderState()));
            //}
           
			///////////statistics
			crStatistics *statistics = crStatistics::getInstance();

			m_stateLabelText->draw(*(sv->getCurrentRenderState()));
			sprintf(tmpText,"%d\0",statistics->numStates);
			m_stateCounterText->setText(tmpText);
			m_stateCounterText->draw(*(sv->getCurrentRenderState()));

			m_drawableLabelText->draw(*(sv->getCurrentRenderState()));
			sprintf(tmpText,"%d\0",statistics->numDrawables);
			m_drawableCounterText->setText(tmpText);
			m_drawableCounterText->draw(*(sv->getCurrentRenderState()));

			m_vertexLabelText->draw(*(sv->getCurrentRenderState()));
			sprintf(tmpText,"%d\0",statistics->m_vertexCount);
			m_vertexCounterText->setText(tmpText);
			m_vertexCounterText->draw(*(sv->getCurrentRenderState()));

			m_primLabelText->draw(*(sv->getCurrentRenderState()));
			sprintf(tmpText,"%d\0",statistics->getPrimCount());
			m_primCounterText->setText(tmpText);
			m_primCounterText->draw(*(sv->getCurrentRenderState()));
        }
        
    }

}

void crViewerEventHandler::StatsAndHelpDrawCallback::createStatsText()
{
    m_statsInitialized = true;

    float characterSize = 20.0f;

    CRCore::crVector4 colorFR(1.0f,0.0f,0.0f,1.0f);
    CRCore::crVector4 colorFrameTimes( 0.0f,1.0f,0.0f,1.0f);
    CRCore::crVector4 colorDraw( 0.0f,1.0f,1.0f,1.0f);
    CRCore::crVector4 colorReferDraw( 1.0f,1.0f,0.0f,1.0f);
    
    float leftPos = 10.0f;
    
    CRCore::crVector3 pos(leftPos,1000.0f,0.0f);

    m_frameRateLabelText = new CRGLText::crText;
    m_frameRateLabelText->setFont("fonts/arial.ttf");
    m_frameRateLabelText->setColor(colorFR);
    m_frameRateLabelText->setCharacterSize(characterSize);
    m_frameRateLabelText->setPosition(pos);
    m_frameRateLabelText->setAlignment(CRGLText::crText::BASE_LINE);
    m_frameRateLabelText->setText("Frame Rate: ");

    pos.x() = m_frameRateLabelText->getBound().xMax();

    m_frameRateCounterText = new CRGLText::crText;
    m_frameRateCounterText->setFont("fonts/arial.ttf");
    m_frameRateCounterText->setColor(colorFR);
    m_frameRateCounterText->setCharacterSize(characterSize);
    m_frameRateCounterText->setPosition(pos);
    m_frameRateCounterText->setAlignment(CRGLText::crText::BASE_LINE);
    m_frameRateCounterText->setText("0123456789.");


    pos.x() = leftPos;
    pos.y() -= characterSize;

    {
        CRGLText::crText* text = new CRGLText::crText;
        text->setFont("fonts/arial.ttf");
        text->setColor(colorFrameTimes);
        text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
        text->setCharacterSize(characterSize);
        text->setPosition(pos);
        text->setAlignment(CRGLText::crText::BASE_LINE);
        text->setText("Frame Time: ");

        m_statsLabelList.push_back(text);
        
        pos.x() = text->getBound().xMax();

        m_frameTimeText = new CRGLText::crText;

        m_frameTimeText->setFont("fonts/arial.ttf");
        m_frameTimeText->setColor(colorFrameTimes);
        m_frameTimeText->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
        m_frameTimeText->setCharacterSize(characterSize);
        m_frameTimeText->setPosition(pos);
        m_frameTimeText->setAlignment(CRGLText::crText::BASE_LINE);
        m_frameTimeText->setText("0123456789.");
        
    }

    pos.x() = leftPos;
    pos.y() -= characterSize;

    m_drawTimes.clear();
	m_referDrawTimes.clear();

    crCameraGroup* ocg = m_veh->getCameraGroup();
    Producer::CameraConfig* cfg = ocg->getCameraConfig();
    for (unsigned int i=0;i<cfg->getNumberOfCameras(); ++i )
    {
        pos.x() = leftPos;

        CRGLText::crText* tempText = new CRGLText::crText;
        tempText->setFont("fonts/arial.ttf");
        tempText->setColor(colorDraw);
        tempText->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
        tempText->setCharacterSize(characterSize);
        tempText->setPosition(pos);
        tempText->setAlignment(CRGLText::crText::BASE_LINE);
        tempText->setText("Draw: ");

        m_statsLabelList.push_back(tempText);
        
        pos.x() = tempText->getBound().xMax();

        tempText = new CRGLText::crText;
        tempText->setFont("fonts/arial.ttf");
        tempText->setColor(colorDraw);
        tempText->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
        tempText->setCharacterSize(characterSize);
        tempText->setPosition(pos);
        tempText->setAlignment(CRGLText::crText::BASE_LINE);
        tempText->setText("1000.00");
        
        m_drawTimes.push_back(0.0);
        m_drawTimeText.push_back(tempText);

        pos.x() = tempText->getBound().xMax();

        tempText = new CRGLText::crText;
        tempText->setFont("fonts/arial.ttf");
        tempText->setColor(colorReferDraw);
        tempText->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
        tempText->setCharacterSize(characterSize);
        tempText->setPosition(pos);
        tempText->setAlignment(CRGLText::crText::BASE_LINE);
        tempText->setText("ReferDraw: ");

        m_statsLabelList.push_back(tempText);
        
        pos.x() = tempText->getBound().xMax();

        tempText = new CRGLText::crText;

        tempText->setFont("fonts/arial.ttf");
        tempText->setColor(colorReferDraw);
        tempText->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
        tempText->setCharacterSize(characterSize);
        tempText->setPosition(pos);
        tempText->setAlignment(CRGLText::crText::BASE_LINE);
        tempText->setText("1000.00");
        
		m_referDrawTimes.push_back(0.0);
        m_referDrawTimeText.push_back(tempText);

        pos.y() -= characterSize;
    }

////////////////////////////state
	pos.x() = leftPos;

	m_stateLabelText = new CRGLText::crText;
	m_stateLabelText->setFont("fonts/arial.ttf");
	m_stateLabelText->setColor(colorFR);
	m_stateLabelText->setCharacterSize(characterSize);
	m_stateLabelText->setPosition(pos);
	m_stateLabelText->setAlignment(CRGLText::crText::BASE_LINE);
	m_stateLabelText->setText("Number of state: ");

	pos.x() = m_stateLabelText->getBound().xMax();

	m_stateCounterText = new CRGLText::crText;
	m_stateCounterText->setFont("fonts/arial.ttf");
	m_stateCounterText->setColor(colorFR);
	m_stateCounterText->setCharacterSize(characterSize);
	m_stateCounterText->setPosition(pos);
	m_stateCounterText->setAlignment(CRGLText::crText::BASE_LINE);
	m_stateCounterText->setText("0123456789.");

	pos.x() = leftPos;
	pos.y() -= characterSize;

///////////////////////drawable
	m_drawableLabelText = new CRGLText::crText;
	m_drawableLabelText->setFont("fonts/arial.ttf");
	m_drawableLabelText->setColor(colorFR);
	m_drawableLabelText->setCharacterSize(characterSize);
	m_drawableLabelText->setPosition(pos);
	m_drawableLabelText->setAlignment(CRGLText::crText::BASE_LINE);
	m_drawableLabelText->setText("Number of drawables: ");

	pos.x() = m_drawableLabelText->getBound().xMax();

	m_drawableCounterText = new CRGLText::crText;
	m_drawableCounterText->setFont("fonts/arial.ttf");
	m_drawableCounterText->setColor(colorFR);
	m_drawableCounterText->setCharacterSize(characterSize);
	m_drawableCounterText->setPosition(pos);
	m_drawableCounterText->setAlignment(CRGLText::crText::BASE_LINE);
	m_drawableCounterText->setText("0123456789.");

	pos.x() = leftPos;
	pos.y() -= characterSize;

////////////////////////////m_vertexLabelText
	m_vertexLabelText = new CRGLText::crText;
	m_vertexLabelText->setFont("fonts/arial.ttf");
	m_vertexLabelText->setColor(colorFR);
	m_vertexLabelText->setCharacterSize(characterSize);
	m_vertexLabelText->setPosition(pos);
	m_vertexLabelText->setAlignment(CRGLText::crText::BASE_LINE);
	m_vertexLabelText->setText("Number of vertex: ");

	pos.x() = m_vertexLabelText->getBound().xMax();

	m_vertexCounterText = new CRGLText::crText;
	m_vertexCounterText->setFont("fonts/arial.ttf");
	m_vertexCounterText->setColor(colorFR);
	m_vertexCounterText->setCharacterSize(characterSize);
	m_vertexCounterText->setPosition(pos);
	m_vertexCounterText->setAlignment(CRGLText::crText::BASE_LINE);
	m_vertexCounterText->setText("0123456789.");

	pos.x() = leftPos;
	pos.y() -= characterSize;

	////////////////////////////m_vertexLabelText
	m_primLabelText = new CRGLText::crText;
	m_primLabelText->setFont("fonts/arial.ttf");
	m_primLabelText->setColor(colorFR);
	m_primLabelText->setCharacterSize(characterSize);
	m_primLabelText->setPosition(pos);
	m_primLabelText->setAlignment(CRGLText::crText::BASE_LINE);
	m_primLabelText->setText("Number of Primitive: ");

	pos.x() = m_primLabelText->getBound().xMax();

	m_primCounterText = new CRGLText::crText;
	m_primCounterText->setFont("fonts/arial.ttf");
	m_primCounterText->setColor(colorFR);
	m_primCounterText->setCharacterSize(characterSize);
	m_primCounterText->setPosition(pos);
	m_primCounterText->setAlignment(CRGLText::crText::BASE_LINE);
	m_primCounterText->setText("0123456789.");

	pos.x() = leftPos;
	pos.y() -= characterSize;


}

void crViewerEventHandler::StatsAndHelpDrawCallback::displayInfo()
{
}

void crViewerEventHandler::StatsAndHelpDrawCallback::createInfoText()
{
}

crViewerEventHandler::crViewerEventHandler(crCameraGroup* cg):
    m_cg(cg),
    m_writeNodeFileName("saved_model.cre"),
    m_displayHelp(false),
	m_displayLogo(true),
    m_displayPost(false),
    m_frameStatsMode(NO_STATS),
    m_firstTimeTogglingFullScreen(true)
{
    Producer::CameraConfig* cfg = m_cg->getCameraConfig();
    Producer::Camera *cam = cfg->getCamera(0);
    
    //m_statsAndHelpDrawCallback = new StatsAndHelpDrawCallback(this,0);
   // cam->addPostDrawCallback(m_statsAndHelpDrawCallback);

    if (cfg->getNumberOfCameras()==1)
    {
        SnapImageDrawCallback* snapImageDrawCallback = new SnapImageDrawCallback("saved_image.jpg");
        cam->addPostDrawCallback(snapImageDrawCallback);
        m_snapImageDrawCallbackList.push_back(snapImageDrawCallback);
    }

	char filename[256];// = "output/photos/image_1.jpg";
	int i = 1;
	FILE *stream = NULL;
	while(1)
	{
		sprintf(filename,"output/photos/image_%d.jpg",i);
		stream  = fopen( filename, "r" );
		if(stream)
		{
			fclose(stream);
			i++;
		}
		else
		{
			break;
		}
	}
	SnapImageDrawCallback::setIndex(i);

    //crViewer* viewer = dynamic_cast<crViewer*>(cg);
    //if (viewer) setWriteImageFileName(viewer->getWriteImageFileName());
    //else setWriteImageFileName("saved_image.jpg");

}

void crViewerEventHandler::setWriteImageOnNextFrame(bool writeImageOnNextFrame)
{
	for(SnapImageDrawCallbackList::iterator itr=m_snapImageDrawCallbackList.begin();
		itr!=m_snapImageDrawCallbackList.end();
		++itr)
	{
		(*itr)->setSnapImageOnNextFrame(writeImageOnNextFrame);
	}
}

void crViewerEventHandler::setWriteImageFileName(const std::string& filename)
{
	std::string basename = CRIOManager::getNameLessExtension(filename);
	std::string ext = CRIOManager::getFileExtension(filename);

	unsigned int cameraNum = 0;
	for(SnapImageDrawCallbackList::iterator itr=m_snapImageDrawCallbackList.begin();
		itr!=m_snapImageDrawCallbackList.end();
		++itr, ++cameraNum)
	{
		if (cameraNum==0)
		{
			(*itr)->setFileName(filename);
		}
		else
		{
			std::string name(basename+"_");
			name += ('0'+cameraNum);
			name += '.';
			name += ext;
			(*itr)->setFileName(name);
		}
	}
}

class SearchInfoVisitor : public CRCore::crNodeVisitor
{
public:
	static SearchInfoVisitor* instance()
	{
		static CRCore::ref_ptr<SearchInfoVisitor> s_searchInfoVisitor = new SearchInfoVisitor;
		return s_searchInfoVisitor.get();
	}

	SearchInfoVisitor():
	CRCore::crNodeVisitor(NODE_VISITOR,TRAVERSE_PARENTS){}

	virtual void apply(CRCore::crGroup& node)
	{
		if(node.getDescriptionsInString().find(m_key)!=-1)
			m_result = &node;
		else
			traverse(node);
	}

	void setkey(const std::string &str){ m_key = str; }
	void reset(){ m_result = NULL; m_key.clear(); }
	CRCore::crNode *getResult(){ return m_result.get(); }

protected:
	virtual ~SearchInfoVisitor(){}
	std::string m_key; 
	CRCore::ref_ptr<CRCore::crNode> m_result;
};
#define  g_searchInfoVisitor SearchInfoVisitor::instance()

bool crViewerEventHandler::handle(const CRGA::crGUIEventAdapter& ea,CRGA::crGUIActionAdapter& aa)
{
    if(!m_cg) return false;

    switch(ea.getEventType())
	{
	 //case(CRGA::crGUIEventAdapter::RELEASE):
		//{
		//	//if(ea.getButtonMask()==CRGA::crGUIEventAdapter::LEFT_MOUSE_BUTTON)
		//	{//拾取物体，并显示其属性对话框
  //              pick(ea);
		//	}
		//	return true;
		//}
	case (CRGA::crGUIEventAdapter::KEYDOWN):
		{
			switch( ea.getKey() )
			{
			//case CRGA::crGUIEventAdapter::KEY_F2:
			//	{
			//		crMatrixd matrix = m_cg->getViewMatrix();
			//		crVector3d ep,center,up;
			//		matrix.getLookAt(ep,center,up);
			//		CRCore::ref_ptr<CRCore::crLineSegment> seg = new CRCore::crLineSegment;
			//		//ep[2] -= 1.0f;
			//		seg->set(ep, ep+(center - ep)*10.0f);


			//		CRUtil::crIntersectVisitor iv;

			//		iv.addLineSegment(seg.get());
			//		iv.setTraversalMask(0xfe);
			//		CRCore::crBrain::getInstance()->getCurrentActiveDB()->accept(iv);
			//		
			//		if (iv.hits())
			//		{
			//			CRUtil::crIntersectVisitor::HitList& hitList = iv.getHitList(seg.get());
			//			if (!hitList.empty())
			//			{
			//				if (hitList[0].m_geode.valid())
			//				{//选中一个object
			//					//弹出该object的属性对话框
			//					//hlist[0].m_geode->showBoundingBox(true);

			//					g_searchInfoVisitor->reset();
			//					g_searchInfoVisitor->setkey("Info");

			//					hitList[0].m_geode->accept(*g_searchInfoVisitor);

			//					CRCore::crNode *node = g_searchInfoVisitor->getResult();
			//					if(node)
			//					{
			//						std::string info;
			//						if(CRCore::crArgumentParser::readKeyValue(node->getDescriptionsInString(),"Info",info))
			//							ShellExecute(NULL, NULL, info.c_str(), NULL, NULL, SW_SHOWNORMAL);
			//					}
			//				}
			//			}

			//		}
			//		return true;
			//	}
			case '`' :
				{
					m_frameStatsMode = (FrameStatsMode)((m_frameStatsMode+1)%3);
					if (m_frameStatsMode==NO_STATS)
					{
						m_cg->setInstrumentationMode(false);
						crStatistics::getInstance()->setStat(false);
					}
					else
					{
						m_cg->setInstrumentationMode(true);
						crStatistics::getInstance()->setStat(true);
					}
					return true;
				}
				//case 'v' :
				//{
				//   // m_cg->setBlockOnVsync(!m_cg->getBlockOnVsync());
				//    return true;
				//}

			/*case 'f' :
				{
					Producer::CameraConfig* cfg = m_cg->getCameraConfig();
					for( unsigned int i = 0; i < cfg->getNumberOfCameras(); ++i )
					{
						Producer::Camera *cam = cfg->getCamera(i);
						Producer::RenderSurface* rs = cam->getRenderSurface();

						if (m_firstTimeTogglingFullScreen && rs->isFullScreen())
						{
#ifndef WIN32                    
							rs->useBorder(true);
							rs->setWindowRectangle(240,220,800,600);
#else                        
							rs->fullScreen(false);
							rs->setWindowRectangle(240,220,800,600);
#endif
						}
						else
						{
							rs->fullScreen(!rs->isFullScreen());
						}
					}
					m_firstTimeTogglingFullScreen = false;

					return true;
				}
*/
				//          case 'o' :
				//          {
				///*              CRCore::crNode* node = m_cg->getSceneData();
				//              if (node)
				//              {
				//                  if (CRIOManager::writeNodeFile(*node,m_writeNodeFileName.c_str()))
				//                  {
				//                      std::cout<<"writen nodes to file "<<m_writeNodeFileName<<std::endl;
				//                  }
				//              }*/

				//              return true;
				//          }

			case CRGA::crGUIEventAdapter::KEY_Print :
			case 'O' :
		    case 'o' :
				{
					for(SnapImageDrawCallbackList::iterator itr=m_snapImageDrawCallbackList.begin();
						itr!=m_snapImageDrawCallbackList.end();
						++itr)
					{
						(*itr)->setSnapImageOnNextFrame(true);
					}

					return true;
				}
			case 'p' :
			case 'P' :
				{
                    crViewer *viewer = dynamic_cast<crViewer *>(m_cg);
					if(viewer) viewer->gainCubeImage("gainedCubeImage.jpg");
					return true;
				}
			//case '+' :
			//	{
			//		//_camera->setFusionDistanceRatio(_camera->getFusionDistanceRatio()*1.25f);
			//		return true;
			//	}
			//case '-' :
			//	{
			//		//_camera->setFusionDistanceRatio(_camera->getFusionDistanceRatio()/1.25f);
			//		return true;
			//	}

			//case '*' :
			//case '/' :
			//	{
			//		if (ea.getKey()=='*') m_cg->setLODScale( m_cg->getLODScale() * 1.1f);
			//		else m_cg->setLODScale( m_cg->getLODScale() / 1.1f);
			//		std::cout<<"New LOD Scale = "<<m_cg->getLODScale()<<std::endl;
			//		return true;
			//	}

				//case CRGA::crGUIEventAdapter::KEY_Help :
				//case 'h' :
				//{
				//    setDisplayHelp(!getDisplayHelp());
				//    return true;
				//}
			//case 'Z' :
			//case 'z' :
			//	{
			//		CRProducer::crViewer* viewer = dynamic_cast<CRProducer::crViewer*>(m_cg);
			//		if (viewer)
			//		{
			//		    if (viewer->getRecordingAnimationPath())
			//		    {
			//		        // have already been recording so switch of recording.
			//		        viewer->setRecordingAnimationPath(false);

			//		        CRCore::notify(CRCore::NOTICE) << "finished recording camera animation, press 'Z' to replay."<< std::endl;

			//		        if (viewer->getAnimationPath())
			//		        {
			//		            std::ofstream fout("saved_animation.path");
			//		            viewer->getAnimationPath()->write(fout);
			//		            fout.close();

			//		            CRCore::notify(CRCore::NOTICE) << "Saved camera animation to 'saved_animation.path'"<< std::endl;

			//		        }

			//		    }
			//		    else if (ea.getKey()=='z') 
			//		    {
			//		        viewer->setRecordingAnimationPath(true);
			//		        viewer->setAnimationPath(new CRCore::crAnimationPath());
			//		        viewer->getAnimationPath()->setLoopMode(CRCore::crAnimationPath::LOOP);
			//		        CRCore::notify(CRCore::NOTICE) << "Recording camera animation, press 'z' to finish recording."<< std::endl;
			//		    }

			//		    if (ea.getKey()=='Z')
			//		    {
			//		        CRGA::crAnimationPathManipulator* apm = 0;
			//		        unsigned int apmNo = 0;
			//		        
			//		        CRGA::crKeySwitchMatrixManipulator* kscm = viewer->getKeySwitchMatrixManipulator();
			//		        if (kscm)
			//		        {
			//		            for(apmNo=0;apmNo<kscm->getNumMatrixManipulators() && apm==0;++apmNo)
			//		            {
			//		                apm = dynamic_cast<CRGA::crAnimationPathManipulator*>(kscm->getMatrixManipulator(apmNo));
			//		            }
			//		        }

			//		        if (!apm)
			//		        {
			//		            apm = new CRGA::crAnimationPathManipulator();
			//		            apmNo = viewer->addCameraManipulator(apm);
			//		        }

			//		        apm->setAnimationPath(viewer->getAnimationPath());
			//		        apm->home(ea,aa);

			//		        viewer->selectCameraManipulator(apmNo);
			//		    }

			//		    break;
			//		}
			//		return true;
			//	}
			//case CRGA::crGUIEventAdapter::KEY_KP_Tab :
			//	{
			//		if(ea.getModKeyMask() == CRGA::crGUIEventAdapter::MODKEY_ALT)
			//		{//暂停，释放鼠标
			//		}
			//		return true;
			//	}
			default:
				break;

			}

		}
	}
    return false;

}

//void crViewerEventHandler::pick(const CRGA::crGUIEventAdapter& ea)
//{
//	CRProducer::crViewer* viewer = dynamic_cast<CRProducer::crViewer*>(m_cg);
//	if(viewer)
//	{
//		CRUtil::crIntersectVisitor::HitList hlist;
//
//		if (viewer->computeIntersections(ea.getX(),ea.getY(),hlist))
//		{
//			if(hlist.size()>0)
//			{			
//				if (hlist[0].m_geode.valid())
//				{//选中一个object
//                 //弹出该object的属性对话框
//					//hlist[0].m_geode->showBoundingBox(true);
//					
//					g_searchInfoVisitor->reset();
//					g_searchInfoVisitor->setkey("Info");
//
//					hlist[0].m_geode->accept(*g_searchInfoVisitor);
//
//					CRCore::crNode *node = g_searchInfoVisitor->getResult();
//                    if(node)
//					{
//						std::string info;
//					    if(CRCore::crArgumentParser::readKeyValue(node->getDescriptionsInString(),"Info",info))
//						    ShellExecute(NULL, NULL, info.c_str(), NULL, NULL, 4);
//					}
//				}
//			}
//		}
//	}
//}

void crViewerEventHandler::accept(CRGA::crGUIEventHandlerVisitor& gehv)
{
    gehv.visit(*this);
}

void crViewerEventHandler::getUsage(CRCore::crApplicationUsage& usage) const
{
    usage.addKeyboardMouseBinding("f","Toggle fullscreen");
    usage.addKeyboardMouseBinding("h","Display help");
    usage.addKeyboardMouseBinding("o","Write scene graph to \"saved_model.CRCore\"");
    usage.addKeyboardMouseBinding("O PrtSrn","Write camera images to \"saved_image*.rgb\"");
    usage.addKeyboardMouseBinding("s","Toggle instrumention");
    usage.addKeyboardMouseBinding("v","Toggle block and vsync");
    usage.addKeyboardMouseBinding("z","Start recording camera path.");
    usage.addKeyboardMouseBinding("Z","If recording camera path stop recording camera path, save to \"saved_animation.path\"\nThen start viewing from being on animation path");
//    usage.addKeyboardMouseBinding("+","When in stereo, increase the fusion distance");
//    usage.addKeyboardMouseBinding("-","When in stereo, reduse the fusion distance");
}