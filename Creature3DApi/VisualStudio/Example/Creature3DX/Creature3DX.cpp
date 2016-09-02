// Creature3DX.cpp : CCreature3DXApp 和 DLL 注册的实现。

#include "stdafx.h"
#include "Creature3DX.h"
#include "comcat.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CCreature3DXApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x25A06DDE, 0xB9A0, 0x47B9, { 0x90, 0x3B, 0x16, 0x6F, 0xBD, 0x6, 0x44, 0x49 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

const CATID CATID_SafeForScripting     =
{0x7dd95801,0x9882,0x11cf,{0x9f,0xa9,0x00,0xaa,0x00,0x6c,0x42,0xc4}};
const CATID CATID_SafeForInitializing  =
{0x7dd95802,0x9882,0x11cf,{0x9f,0xa9,0x00,0xaa,0x00,0x6c,0x42,0xc4}};

const GUID CDECL BASED_CODE _ctlid =
{ 0x5d2ee22e, 0xecfe, 0x440a,{ 0x8d, 0x48, 0x86, 0xed, 0xda, 0x43, 0x39, 0x95 } };
// CCreature3DXApp::InitInstance - DLL 初始化

BOOL CCreature3DXApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: 在此添加您自己的模块初始化代码。
	}

	return bInit;
}



// CCreature3DXApp::ExitInstance - DLL 终止

int CCreature3DXApp::ExitInstance()
{
	// TODO: 在此添加您自己的模块终止代码。

	return COleControlModule::ExitInstance();
}

//  创建组件种类     
HRESULT CreateComponentCategory(CATID catid, WCHAR *  catDescription) 
{    
	ICatRegister *pcr  =  NULL ;    
	HRESULT hr  =  S_OK ;    
	hr  =  CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, ( void ** ) & pcr);    
	if  (FAILED(hr))  return  hr;    
	//  Make sure the HKCR\Component Categories\{..catid...}    
	//  key is registered.     
	CATEGORYINFO catinfo;    
	catinfo.catid  =  catid;    
	catinfo.lcid  =   0x0409;  //  english    
	//  Make sure the provided description is not too long.    
	//  Only copy the first 127 characters if it is.     
	int  len  =  wcslen(catDescription);    
	if  (len > 127 ) len  =   127 ;    
	wcsncpy(catinfo.szDescription, catDescription, len);    
	//  Make sure the description is null terminated.     
	catinfo.szDescription[len] = '\0';    
	hr  =  pcr -> RegisterCategories( 1 ,  & catinfo);    
	pcr -> Release();    
	return  hr;
}  
//  注册组件种类     
HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{    
	//  Register your component categories information.     
	ICatRegister *  pcr  =  NULL ;    
	HRESULT hr  =  S_OK ;    
	hr  =  CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, ( void ** ) & pcr);    
	if  (SUCCEEDED(hr)) {    
		//  Register this category as being "implemented" by the class.     
		CATID rgcatid[ 1 ];    
		rgcatid[ 0 ]  =  catid;    
		hr  =  pcr -> RegisterClassImplCategories(clsid,  1 , rgcatid);    
	}    
	if  (pcr  !=  NULL) pcr -> Release();    
	return  hr;    
}    
//  卸载组件种类     
HRESULT UnRegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{    
	ICatRegister *  pcr  =  NULL ;    
	HRESULT hr  =  S_OK ;    
	hr  =  CoCreateInstance(CLSID_StdComponentCategoriesMgr,    
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, ( void ** ) & pcr);    
	if  (SUCCEEDED(hr)) {    
		//  Unregister this category as being "implemented" by the class.     
		CATID rgcatid[ 1 ] ;    
		rgcatid[ 0 ]  =  catid;    
		hr  =  pcr -> UnRegisterClassImplCategories(clsid,  1 , rgcatid);    
	}    
	if  (pcr  !=  NULL) pcr -> Release();    
	return  hr;    
}

// DllRegisterServer - 将项添加到系统注册表

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	if (FAILED( CreateComponentCategory(
		CATID_SafeForScripting,
		L"Controls that are safely scriptable") ))
		return ResultFromScode(SELFREG_E_CLASS);

	if (FAILED( CreateComponentCategory(
		CATID_SafeForInitializing,
		L"Controls safely initializable from persistent data") ))
		return ResultFromScode(SELFREG_E_CLASS);

	if (FAILED( RegisterCLSIDInCategory(
		_ctlid, CATID_SafeForScripting) ))
		return ResultFromScode(SELFREG_E_CLASS);

	if (FAILED( RegisterCLSIDInCategory(
		_ctlid, CATID_SafeForInitializing) ))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;

	//AFX_MANAGE_STATE(_afxModuleAddrThis);

	//if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
	//	return ResultFromScode(SELFREG_E_TYPELIB);

	//if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
	//	return ResultFromScode(SELFREG_E_CLASS);

	//return NOERROR;
}



// DllUnregisterServer - 将项从系统注册表中移除

STDAPI DllUnregisterServer(void)
{
	//HRESULT hr;    
	//AFX_MANAGE_STATE(_afxModuleAddrThis);    
	//if  ( ! AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))    
	//	return  ResultFromScode(SELFREG_E_TYPELIB);    
	//if  ( ! COleObjectFactoryEx::UpdateRegistryAll(FALSE))    
	//	return  ResultFromScode(SELFREG_E_CLASS);    
	////  删除控件初始化安全入口.     
	//hr = UnRegisterCLSIDInCategory(BASED_CODE _tlid , CATID_SafeForInitializing);    
	//if  (FAILED(hr))  return  hr;    
	////  删除控件脚本安全入口     
	//hr = UnRegisterCLSIDInCategory(BASED_CODE _tlid , CATID_SafeForScripting);    
	//if  (FAILED(hr))  return  hr;    
	//return  NOERROR;
	////AFX_MANAGE_STATE(_afxModuleAddrThis);

	////if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
	////	return ResultFromScode(SELFREG_E_TYPELIB);

	////if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
	////	return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}