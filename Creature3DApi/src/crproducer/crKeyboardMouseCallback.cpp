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
#include <CRCore/crMath.h>
#include <CRCore/crNotify.h>
#include <CRCore/crWchMsg.h>
#include <CRCore/crBrain.h>
#include <CRProducer/crKeyboardMouseCallback.h>

#include <float.h>

using namespace CRProducer;

void crKeyboardMouseCallback::mouseScroll( Producer::KeyboardMouseCallback::ScrollingMotion sm )
{
    CRCore::ref_ptr<crEventAdapter> event = createEventAdapter();

    m_eventQueueMutex.lock();
    event->adaptMouseScroll(getTime(), sm);
    m_eventQueue.push_back(event);
    m_eventQueueMutex.unlock();
}

void crKeyboardMouseCallback::buttonPress( float mx, float my, unsigned int mbutton ) 
{
    m_mx = mx;
    m_my = my;
    m_mbutton |= (1<<(mbutton-1));
    
    CRCore::ref_ptr<crEventAdapter> event = createEventAdapter();
    
    m_eventQueueMutex.lock();
    event->adaptButtonPress(getTime(),mx,my,mbutton);
    m_eventQueue.push_back(event);
    m_eventQueueMutex.unlock();
}

void crKeyboardMouseCallback::buttonRelease( float mx, float my, unsigned int mbutton ) 
{
    m_mx = mx;
    m_my = my;
    m_mbutton &= ~(1<<(mbutton-1));
    
    CRCore::ref_ptr<crEventAdapter> event = createEventAdapter();
    
    m_eventQueueMutex.lock();
    event->adaptButtonRelease(getTime(),mx,my,mbutton);
    m_eventQueue.push_back(event);
    m_eventQueueMutex.unlock();
}

void crKeyboardMouseCallback::doubleButtonPress( float mx, float my, unsigned int mbutton ) 
{
    m_mx = mx;
    m_my = my;
    m_mbutton |= (1<<(mbutton-1));
    
    CRCore::ref_ptr<crEventAdapter> event = createEventAdapter();
    
    m_eventQueueMutex.lock();
    event->adaptDoubleButtonPress(getTime(),mx,my,mbutton);
    m_eventQueue.push_back(event);
    m_eventQueueMutex.unlock();
}

void crKeyboardMouseCallback::keyPress( Producer::KeyCharacter key )
{
    CRCore::ref_ptr<crEventAdapter> event = createEventAdapter();

    m_eventQueueMutex.lock();
    event->adaptKeyPress(getTime(),key);
    m_eventQueue.push_back(event);
    m_eventQueueMutex.unlock();

	// check against adapted key symbol.    
	if (m_escapeKeySetsDone && 
		event->getKey()==crGUIEventAdapter::KEY_Escape) m_done = true;
}

void crKeyboardMouseCallback::keyRelease( Producer::KeyCharacter key )
{
    CRCore::ref_ptr<crEventAdapter> event = createEventAdapter();
    
    m_eventQueueMutex.lock();
    event->adaptKeyRelease(getTime(),key);
    m_eventQueue.push_back(event);
    m_eventQueueMutex.unlock();

	//if(event->getKey()==CRGA::crGUIEventAdapter::KEY_Alt_L||event->getKey()==CRGA::crGUIEventAdapter::KEY_Alt_R)
	//{
	//	m_alt_pressed = 0.0;
	//}
}

void crKeyboardMouseCallback::specialKeyPress( Producer::KeyCharacter key )
{
    CRCore::ref_ptr<crEventAdapter> event = createEventAdapter();

    m_eventQueueMutex.lock();
    event->adaptKeyPress(getTime(),key);
    m_eventQueue.push_back(event);
    m_eventQueueMutex.unlock();

	//if(event->getKey()==CRGA::crGUIEventAdapter::KEY_Alt_L||event->getKey()==CRGA::crGUIEventAdapter::KEY_Alt_R)
	//{
	//	m_alt_pressed = event->time();
	//	CRCore::notify(CRCore::FATAL)<<"specialKeyPress alt = "<<m_alt_pressed<<std::endl;
	//}
	//else if(event->getKey()==CRGA::crGUIEventAdapter::KEY_Tab||event->getKey()==CRGA::crGUIEventAdapter::KEY_KP_Tab)
	//{
	//	if(m_alt_pressed > 0.0 && m_alt_pressed < event->time())
	//	{
	//		CRCore::notify(CRCore::FATAL)<<"alt + tab"<<std::endl;
	//	}
	//	CRCore::notify(CRCore::FATAL)<<"specialKeyPress Tab = "<<event->time()<<std::endl;
	//}

	// check against adapted key symbol.    
	//if (m_escapeKeySetsDone && 
	//	event->getKey()==CRGA::crGUIEventAdapter::KEY_Escape) m_done = true;
}

void crKeyboardMouseCallback::specialKeyRelease( Producer::KeyCharacter key )
{
    CRCore::ref_ptr<crEventAdapter> event = createEventAdapter();
    
    m_eventQueueMutex.lock();
    event->adaptKeyRelease(getTime(),key);
    m_eventQueue.push_back(event);
    m_eventQueueMutex.unlock();

	//if(event->getKey()==CRGA::crGUIEventAdapter::KEY_Alt_L||event->getKey()==CRGA::crGUIEventAdapter::KEY_Alt_R)
	//{
	//	m_alt_pressed = 0.0;
	//}
}

void crKeyboardMouseCallback::inputChar( wchar_t c ) 
{
	CRCore::ref_ptr<crEventAdapter> event = createEventAdapter();

	m_eventQueueMutex.lock();
	event->adaptInputChar(getTime(),c);
	m_eventQueue.push_back(event);
	m_eventQueueMutex.unlock();

}

void crKeyboardMouseCallback::windowConfig( int x, int y, unsigned int width, unsigned int height )
{
    CRCore::ref_ptr<crEventAdapter> event = createEventAdapter();
    
    m_eventQueueMutex.lock();
    event->adaptResize(getTime(), x, y, x+width, y+height );
    m_eventQueue.push_back(event);
    m_eventQueueMutex.unlock();
}

//void crKeyboardMouseCallback::windowFocus( int focus, Producer::Window transferwin )
//{
//	CRCore::ref_ptr<crEventAdapter> event = createEventAdapter();
//
//	m_eventQueueMutex.lock();
//	event->windowFocus(getTime(), focus, (__int32)transferwin);
//	m_eventQueue.push_back(event);
//	m_eventQueueMutex.unlock();
//}

void crKeyboardMouseCallback::mouseMotion( float mx, float my) 
{
    m_mx = mx;
    m_my = my;
    
    CRCore::ref_ptr<crEventAdapter> event = createEventAdapter();
    
    m_eventQueueMutex.lock();
    event->adaptMouseMotion(getTime(),mx,my);
    m_eventQueue.push_back(event);
    m_eventQueueMutex.unlock();

	//CRCore::notify(CRCore::FATAL)<<"mouseMotion(): mx = "<<mx<<" my="<<my<<std::endl;
}

void crKeyboardMouseCallback::passiveMouseMotion( float mx, float my) 
{
    m_mx = mx;
    m_my = my;
    
    CRCore::ref_ptr<crEventAdapter> event = createEventAdapter();
    
    m_eventQueueMutex.lock();
    event->adaptMouseMotion(getTime(),mx,my);
    m_eventQueue.push_back(event);
    m_eventQueueMutex.unlock();

	//CRCore::notify(CRCore::FATAL)<<"passiveMouseMotion(): mx = "<<mx<<" my="<<my<<std::endl;
}

void crKeyboardMouseCallback::mouseWarp( float mx, float my) 
{
    m_mx = mx;
    m_my = my;
    
    CRCore::ref_ptr<crEventAdapter> event = createEventAdapter();
    
    m_eventQueueMutex.lock();
    m_eventQueue.push_back(event);
    m_eventQueueMutex.unlock();

}

double crKeyboardMouseCallback::getEventQueue(EventQueue& queue)
{
    double swapTime;

    queue.clear();
    m_eventQueueMutex.lock();
    m_eventQueue.swap(queue);
    swapTime = getTime();
	m_eventQueueMutex.unlock();
    
    return swapTime;
}

double crKeyboardMouseCallback::copyEventQueue(EventQueue& queue)
{
	double swapTime;

	queue.clear();
	m_eventQueueMutex.lock();
	queue = m_eventQueue;
	swapTime = getTime();
	m_eventQueueMutex.unlock();

	return swapTime;
}

double crKeyboardMouseCallback::setEventQueue(EventQueue& queue)
{
	double swapTime;

	m_eventQueueMutex.lock();
	m_eventQueue = queue;
	swapTime = getTime();
	m_eventQueueMutex.unlock();

	return swapTime;
}

double crKeyboardMouseCallback::appendEventQueue(EventQueue& queue)
{
	double swapTime;

	m_eventQueueMutex.lock();
	m_eventQueue.insert(m_eventQueue.end(),queue.begin(),queue.end());
	swapTime = getTime();
	m_eventQueueMutex.unlock();

	return swapTime;
}

crEventAdapter* crKeyboardMouseCallback::createEventAdapter()
{
    crEventAdapter* ea = new crEventAdapter;

    Producer::InputArea* ia = m_keyboardMouse->getInputArea();
    Producer::RenderSurface* rs = m_keyboardMouse->getRenderSurface();
    if (ia)
    {
    
        float minX = FLT_MAX;
        float minY = FLT_MAX;
        float maxX = -FLT_MAX;
        float maxY = -FLT_MAX;
        //int numInputRectangle = ia->getNumInputRectangle();
        int numRenderSurfaces = ia->getNumRenderSurfaces();
        for (int i=0;i<numRenderSurfaces;++i)
        {
            const Producer::RenderSurface::InputRectangle &ir = 
                ia->getRenderSurface(i)->getInputRectangle();

            minX = CRCore::minimum(minX,ir.left());
            minX = CRCore::minimum(minX,ir.left()+ir.width());
            
            minY = CRCore::minimum(minY,ir.bottom());
            minY = CRCore::minimum(minY,ir.bottom()+ir.height());

            maxX = CRCore::maximum(maxX,ir.left());
            maxX = CRCore::maximum(maxX,ir.left()+ir.width());
            
            maxY = CRCore::maximum(maxY,ir.bottom());
            maxY = CRCore::maximum(maxY,ir.bottom()+ir.height());
        }
        ea->setWindowSize(minX,minY,maxX,maxY);
    }
    else if (rs)
    {
        //ea->setWindowSize(-1.0f,-1.0f,1.0f,1.0f);
        
        const Producer::RenderSurface::InputRectangle &ir = rs->getInputRectangle();

        float minX = CRCore::minimum(ir.left(),ir.left()+ir.width());
        float maxX = CRCore::maximum(ir.left(),ir.left()+ir.width());
        float minY = CRCore::minimum(ir.bottom(),ir.bottom()+ir.height());
        float maxY = CRCore::maximum(ir.bottom(),ir.bottom()+ir.height());

        ea->setWindowSize(minX,minY,maxX,maxY);
    }
    
    return ea;
}

void crKeyboardMouseCallback::shutdown(int flg)
{
	CRCore::crBrain::getInstance()->doEvent(MAKEINT64(WCH_EXITGAME,flg));
    //m_done = true;
    //m_keyboardMouse->cancel();
}

void crKeyboardMouseCallback::clear()
{
	m_eventQueueMutex.lock();
	m_eventQueue.clear();
	m_eventQueueMutex.unlock();
}
void crKeyboardMouseCallback::windowRestored(int flg)
{
	CRCore::crBrain::getInstance()->doEvent(MAKEINT64(WCH_WINRESTORED,flg));
}