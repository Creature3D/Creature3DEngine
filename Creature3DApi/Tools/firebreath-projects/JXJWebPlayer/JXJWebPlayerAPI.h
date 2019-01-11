/**********************************************************\

  Auto-generated JXJWebPlayerAPI.h

\**********************************************************/

#include <string>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "JXJWebPlayer.h"
#include <map>
#include<wininet.h>

#ifndef H_JXJWebPlayerAPI
#define H_JXJWebPlayerAPI

class JXJWebPlayerAPI : public FB::JSAPIAuto
{
public:
    ////////////////////////////////////////////////////////////////////////////
    /// @fn JXJWebPlayerAPI::JXJWebPlayerAPI(const JXJWebPlayerPtr& plugin, const FB::BrowserHostPtr host)
    ///
    /// @brief  Constructor for your JSAPI object.
    ///         You should register your methods, properties, and events
    ///         that should be accessible to Javascript from here.
    ///
    /// @see FB::JSAPIAuto::registerMethod
    /// @see FB::JSAPIAuto::registerProperty
    /// @see FB::JSAPIAuto::registerEvent
    ////////////////////////////////////////////////////////////////////////////
	JXJWebPlayerAPI::JXJWebPlayerAPI(const JXJWebPlayerPtr& plugin, const FB::BrowserHostPtr& host) :
		m_plugin(plugin), m_host(host),m_thread(NULL),m_creature3d(NULL),m_needRestart(false),m_dialog(NULL),m_downloadThread(NULL),
		m_downloadState(0)

	{
		registerMethod("setParam", make_method(this, &JXJWebPlayerAPI::setParam));
		registerMethod("Init", make_method(this, &JXJWebPlayerAPI::Initialize));

		// Read-write property
		registerProperty("testString",
			make_property(this,
			&JXJWebPlayerAPI::get_testString,
			&JXJWebPlayerAPI::set_testString));

		// Read-only property
		registerProperty("version",
			make_property(this,
			&JXJWebPlayerAPI::get_version));

		m_params.clear();
		//memset(m_szLoginInfo,0,256);
		//m_window = new FB::WinMessageWindow();
	}

    ///////////////////////////////////////////////////////////////////////////////
    /// @fn JXJWebPlayerAPI::~JXJWebPlayerAPI()
    ///
    /// @brief  Destructor.  Remember that this object will not be released until
    ///         the browser is done with it; this will almost definitely be after
    ///         the plugin is released.
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~JXJWebPlayerAPI();

    JXJWebPlayerPtr getPlugin();

    // Read/Write property ${PROPERTY.ident}
    std::string get_testString();
    void set_testString(const std::string& val);

    // Read-only property ${PROPERTY.ident}
    std::string get_version();

	void setParam(std::string key,std::string value);
	std::string getParam(std::string key);
    
    // Event helpers
	FB_JSAPI_EVENT(setParam, 2, (const std::string&, const std::string&));
	FB_JSAPI_EVENT(init, 0, ());

	void Initialize();
	void StartCreature3D(/*HWND hWndParent, HWND hWnd, int x, int y, int width, int height*/);
	bool WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult);
	void WriteLoginInfo2File();	//将登录信息写文件
	void shut();
	void exitGame();
	void needRestart();
	void waitClose();
	FB::BrowserHostPtr getHost(){return m_host;}
	wchar_t* CharToWchar(const char* c) ;
	char* WcharToChar(const wchar_t* wc);
	void Release();
	bool copyFile(const std::string &srcFile, const std::string &destFile);
	void initDialog();
	void showDialog(const char* url);
	void gameLog(int value);
	void download();

	HINTERNET m_internetopenurl;
	
	std::string m_clientPath;
	
    JXJWebPlayerWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;
	std::string m_testString;
	std::map<std::string,std::string> m_params;
    
	bool m_needRestart;
	
	HMODULE m_creature3d;
	HMODULE m_dialog;
	HANDLE m_thread;
	HANDLE m_downloadThread;
	HWND m_hWnd;
	wchar_t* m_wchar;
	char* m_char;
	int m_downloadState;
	HWND m_dlgHwnd;
};

#endif // H_JXJWebPlayerAPI

