/**********************************************************\

  Auto-generated Creature3D.cpp

  This file contains the auto-generated main plugin object
  implementation for the Creature3D project

\**********************************************************/

#include "Creature3DAPI.h"

#include "Creature3D.h"
#include "PluginWindowWin.h"
///////////////////////////////////////////////////////////////////////////////
/// @fn Creature3D::StaticInitialize()
///
/// @brief  Called from PluginFactory::globalPluginInitialize()
///
/// @see FB::FactoryBase::globalPluginInitialize
///////////////////////////////////////////////////////////////////////////////
void Creature3D::StaticInitialize()
{
    // Place one-time initialization stuff here; As of FireBreath 1.4 this should only
    // be called once per process
}

///////////////////////////////////////////////////////////////////////////////
/// @fn Creature3D::StaticInitialize()
///
/// @brief  Called from PluginFactory::globalPluginDeinitialize()
///
/// @see FB::FactoryBase::globalPluginDeinitialize
///////////////////////////////////////////////////////////////////////////////
void Creature3D::StaticDeinitialize()
{
    // Place one-time deinitialization stuff here. As of FireBreath 1.4 this should
    // always be called just before the plugin library is unloaded
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  Creature3D constructor.  Note that your API is not available
///         at this point, nor the window.  For best results wait to use
///         the JSAPI object until the onPluginReady method is called
///////////////////////////////////////////////////////////////////////////////
Creature3D::Creature3D()
{
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  Creature3D destructor.
///////////////////////////////////////////////////////////////////////////////
Creature3D::~Creature3D()
{
    // This is optional, but if you reset m_api (the shared_ptr to your JSAPI
    // root object) and tell the host to free the retained JSAPI objects then
    // unless you are holding another shared_ptr reference to your JSAPI object
    // they will be released here.
    releaseRootJSAPI();
    m_host->freeRetainedObjects();
}

void Creature3D::onPluginReady()
{
    // When this is called, the BrowserHost is attached, the JSAPI object is
    // created, and we are ready to interact with the page and such.  The
    // PluginWindow may or may not have already fire the AttachedEvent at
    // this point.
}

void Creature3D::shutdown()
{
    // This will be called when it is time for the plugin to shut down;
    // any threads or anything else that may hold a shared_ptr to this
    // object should be released here so that this object can be safely
    // destroyed. This is the last point that shared_from_this and weak_ptr
    // references to this object will be valid
	Creature3DAPI* api = static_cast<Creature3DAPI*>(getRootJSAPI().get());
	if (api)
		api->shut();
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  Creates an instance of the JSAPI object that provides your main
///         Javascript interface.
///
/// Note that m_host is your BrowserHost and shared_ptr returns a
/// FB::PluginCorePtr, which can be used to provide a
/// boost::weak_ptr<Creature3D> for your JSAPI class.
///
/// Be very careful where you hold a shared_ptr to your plugin class from,
/// as it could prevent your plugin class from getting destroyed properly.
///////////////////////////////////////////////////////////////////////////////
FB::JSAPIPtr Creature3D::createJSAPI()
{
    // m_host is the BrowserHost
    return boost::make_shared<Creature3DAPI>(FB::ptr_cast<Creature3D>(shared_from_this()), m_host);
}

bool Creature3D::onMouseDown(FB::MouseDownEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse down at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}

bool Creature3D::onMouseUp(FB::MouseUpEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse up at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}

bool Creature3D::onMouseMove(FB::MouseMoveEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse move at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}
bool Creature3D::onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindow *wnd)
{
    // The window is attached; act appropriately
	FB::PluginWindowWin* pWindow = wnd->get_as<FB::PluginWindowWin>();
	if (pWindow)
	{
		m_hWnd = pWindow->getHWND();
	}
    return false;
}

bool Creature3D::onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindow *)
{
    // The window is about to be detached; act appropriately
	Creature3DAPI* api = static_cast<Creature3DAPI*>(getRootJSAPI().get());
	if (api)
		api->shut();
    return false;
}

