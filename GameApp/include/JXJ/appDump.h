#pragma  once
#pragma comment( lib, "DbgHelp" )
#pragma comment( lib, "wininet.lib" )
#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crReadFile.h>
#include <CRIOManager/crWriteFile.h>
#include <crIOManager/crLoadManager.h>
#include <Windows.h>
#include <WinInet.h>
#include <Dbghelp.h>
#include <time.h>
#include <stdio.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRCore/crDisplaySettings.h>
#include <CRCore/crStreamBuf.h>
#include <crgl/glu.h>
#include <CRNet/crNetContainer.h>
#include <CRNetApp/appNetCallbacks.h>
#include <CRNet/crNetWorkUpdater.h>

void getFtpInfo(std::string & ftpHost,
				std::string & ftpUserName,
				std::string & ftpPassword)
{
	std::string file = "Client.cfg";
	rcfg::ConfigScript cfg_script;
	if(!CRIOManager::crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		//CRCore::notify(CRCore::FATAL)<<"crInitDownloadMethod(): file open error,"<<file<<std::endl;
		char gbuf[256];
		sprintf(gbuf,"crInitDownloadMethod(): file open error,%s\n\0",file.c_str());
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		return;
	}
	cfg_script.Get("FtpHost", ftpHost);
	cfg_script.Get("UserName", ftpUserName);
	cfg_script.Get("Password", ftpPassword);
//	MessageBox(NULL,ftpHost.c_str(),NULL,MB_OK);
//	MessageBox(NULL,ftpUserName.c_str(),NULL,MB_OK);
//	MessageBox(NULL,ftpPassword.c_str(),NULL,MB_OK);
	cfg_script.Close();
}
BOOL UpLoadBugFile(std::string & szfilepathName)
{
	std::string remoteDir = std::string("dump//");
	int length = szfilepathName.length();
	//int pos = szfilepathName.rfind('\\');			
	std::string dumpFile =remoteDir+szfilepathName+ std::string(TEXT(".dmp"));
	std::string textFile =remoteDir+szfilepathName+ std::string(TEXT(".txt"));
	std::string gametextFile =remoteDir+szfilepathName+ std::string(TEXT("creature3d.txt"));
	std::string bmpFile =remoteDir+szfilepathName+ std::string(TEXT(".jpg"));
	std::string localDumpFile = szfilepathName+std::string(TEXT(".dmp"));
	std::string localTextFile = szfilepathName+std::string(TEXT(".txt"));
	std::string localGameTextFile = szfilepathName+std::string(TEXT("d.txt"));
	std::string localbmpFile = szfilepathName+std::string(TEXT("d.jpg"));
	HINTERNET hInternet = InternetOpen(NULL,INTERNET_OPEN_TYPE_DIRECT,NULL,NULL,0);
	if(hInternet != NULL)
	{
		std::string ftpAdd,ftpUserName,ftpPassword;
		getFtpInfo(ftpAdd,ftpUserName,ftpPassword);
		HINTERNET ftpInet = InternetConnect(hInternet,ftpAdd.c_str(),INTERNET_DEFAULT_FTP_PORT,ftpUserName.c_str(),\
			ftpPassword.c_str(),INTERNET_SERVICE_FTP,INTERNET_FLAG_EXISTING_CONNECT | INTERNET_NO_CALLBACK,0);
		if(ftpInet!=NULL)
		{
			//MessageBox(NULL,localDumpFile.c_str(),localDumpFile.c_str(),MB_OK);
			FtpPutFile(ftpInet,localDumpFile.c_str(),dumpFile.c_str(),FTP_TRANSFER_TYPE_BINARY,0);			
			//MessageBox(NULL,localTextFile.c_str(),localTextFile.c_str(),MB_OK);
			FtpPutFile(ftpInet,localTextFile.c_str(),textFile.c_str(),FTP_TRANSFER_TYPE_BINARY,0);
			//MessageBox(NULL,localGameTextFile.c_str(),localGameTextFile.c_str(),MB_OK);
			FtpPutFile(ftpInet,localGameTextFile.c_str(),gametextFile.c_str(),FTP_TRANSFER_TYPE_BINARY,0);
			FtpPutFile(ftpInet,localbmpFile.c_str(),bmpFile.c_str(),FTP_TRANSFER_TYPE_BINARY,0);
		}
		InternetCloseHandle(ftpInet);
	}
	InternetCloseHandle(hInternet);

	//delete file
	DeleteFile(localDumpFile.c_str());
	DeleteFile(localTextFile.c_str());
	DeleteFile(localGameTextFile.c_str());
	DeleteFile(localbmpFile.c_str());
	return TRUE;
}

BOOL SaveBitmapToFile(LPCTSTR lpszFilePath,HDC hdc ,HBITMAP hBm)
{
	//  定义位图文件表头
	BITMAPFILEHEADER bmfh;
	//  定义位图信息表头
	BITMAPINFOHEADER bmih;

	//  调色板长度
	int nColorLen = 0;
	//  调色表大小
	DWORD dwRgbQuadSize = 0;
	//  位图大小
	DWORD dwBmSize = 0;
	//  分配内存的指针
	HGLOBAL    hMem = NULL;
	LPBITMAPINFOHEADER     lpbi;
	BITMAP bm;
	HANDLE hFile = NULL;

	DWORD dwWritten;

	GetObject(hBm, sizeof(BITMAP), &bm);
	
	int size, wordsPerScan;
	wordsPerScan=(bm.bmWidth*3+3)/4; // rounds up to next 32 bit boundary
	size=4*bm.bmHeight*wordsPerScan; // rounded to 4bytes * number of scan lines

	bmih.biSize    = sizeof(BITMAPINFOHEADER);    // 本结构所占的字节
	bmih.biWidth    = bm.bmWidth;            // 位图宽
	bmih.biHeight    = bm.bmHeight;            // 位图高
	bmih.biPlanes    = 1;
	bmih.biBitCount        = 24;    // 每一图素的位数
	bmih.biCompression    = 0;            // 
	bmih.biSizeImage        = size;  //  位图大小
	bmih.biXPelsPerMeter    = 1000;
	bmih.biYPelsPerMeter    = 1000;
	bmih.biClrUsed        = 0;
	bmih.biClrImportant    = 0;

	//  计算位图图素数据区大小 
	dwBmSize = 4 * ((bm.bmWidth * bmih.biBitCount + 31) / 32) * bm.bmHeight;

	//  如果图素位 <= 8bit，则有调色板
	if (bmih.biBitCount <= 8)
	{
		nColorLen = (1 << bm.bmBitsPixel);
	}

	//  计算调色板大小
	dwRgbQuadSize = nColorLen * sizeof(RGBQUAD);

	//  分配内存
	hMem = GlobalAlloc(GHND, dwBmSize + dwRgbQuadSize + sizeof(BITMAPINFOHEADER));

	if (NULL == hMem)
	{
		return FALSE;
	}

	//  锁定内存
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hMem);

	//  将bmih中的内容写入分配的内存中
	*lpbi = bmih;

	  //  将位图中的数据以bits的形式放入lpData中。
	GetDIBits(hdc, hBm, 0, (DWORD)bmih.biHeight, (LPSTR)lpbi+sizeof(BITMAPINFOHEADER)+dwRgbQuadSize, (BITMAPINFO *)lpbi, (DWORD)DIB_RGB_COLORS);

	bmfh.bfType = 0x4D42;  // 位图文件类型：BM
	bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwRgbQuadSize + dwBmSize;  // 位图大小
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfOffBits    = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwRgbQuadSize;  // 位图数据与文件头部的偏移量

	//  把上面的数据写入文件中

	hFile = CreateFile(lpszFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		  return FALSE;
	}

	//  写入位图文件头
	WriteFile(hFile, (LPSTR)&bmfh, sizeof(BITMAPFILEHEADER), (DWORD *)&dwWritten, NULL);
	//  写入位图数据
	WriteFile(hFile, (LPBITMAPINFOHEADER)lpbi, bmfh.bfSize - sizeof(BITMAPFILEHEADER), (DWORD *)&dwWritten, NULL);

	GlobalFree(hMem);
	CloseHandle(hFile);

	return TRUE;
}

LONG WINAPI JXJExceptionInfo(_EXCEPTION_POINTERS *ExceptionInfo)
{
	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 
	char temp[128];
	char tempfilename[128];
	char tempgamefileName[128];
	char tempbmpfileName[128];

	sprintf_s(temp,"%4d%02d%02d%02d%02d%02d%03d\0",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wMinute,sys.wMilliseconds);
	sprintf_s(tempfilename,"%s.dmp\0",temp);
	HANDLE dumpFile = CreateFile(tempfilename,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	
	char filePath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,filePath);
	char exeFilePath[2*MAX_PATH];
	//const char *exePath = "reportbugs\\reportbugs.exe";
	sprintf_s(exeFilePath,"%s\0",temp);

	MINIDUMP_EXCEPTION_INFORMATION dumpExceptioninfo;
	dumpExceptioninfo.ExceptionPointers = ExceptionInfo;
	dumpExceptioninfo.ThreadId = GetCurrentThreadId();
	dumpExceptioninfo.ClientPointers = TRUE;

	BOOL isWriteSucess = MiniDumpWriteDump(GetCurrentProcess(),GetCurrentProcessId(),dumpFile,MiniDumpNormal,&dumpExceptioninfo,NULL,NULL);
	CloseHandle(dumpFile);
	if(CRNetApp::crGlobalHandle::isClient())
	{
		//关闭网络线程
		CRNet::crNetWorkReconnecter::getInstance()->clear();
		CRNet::crNetWorkUpdater::getInstance()->clear();
		CRNet::crNetBridge::shutdownGame();
		CRNet::crNetContainer::getInstance()->clear();

		if(CRNetApp::crGlobalHandle::getRunProtectHandle())
		{
			CloseHandle(CRNetApp::crGlobalHandle::getRunProtectHandle());
			CRNetApp::crGlobalHandle::setRunProtectHandle(NULL);
		}
		sprintf_s(tempfilename,"%s.txt\0",temp);
		CRIOManager::copyFile("glinfo.txt",tempfilename);

		//char gbuf[256];
		//GLenum errorNo = glGetError();
		//sprintf(gbuf,"OpenGL error %s\n\0",gluErrorString(errorNo).c_str());
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);

		gDebugInfo->writeToFile();

		//WinExec(exeFilePath,SW_NORMAL);	
		//
		sprintf_s(tempgamefileName,"%sd.txt\0",temp);
		CRIOManager::copyFile("creature3d.txt",tempgamefileName);
			//截屏
		sprintf_s(tempbmpfileName,"%sd.bmp\0",temp);
		HDC hdc = GetDC(NULL);

		static int screenx = GetSystemMetrics(SM_CXSCREEN);
		static int screeny = GetSystemMetrics(SM_CYSCREEN);

		HBITMAP hBitmap = CreateCompatibleBitmap(hdc, screenx, screeny);
		HDC hdcMem = CreateCompatibleDC(hdc);

		SelectObject(hdcMem, hBitmap);
		BitBlt(hdcMem, 0, 0, screenx, screeny, hdc, 0, 0, SRCCOPY);
		SaveBitmapToFile(tempbmpfileName,hdc,hBitmap);

		ReleaseDC(NULL, hdcMem);
		ReleaseDC(NULL, hdc);

		CRCore::ref_ptr<CRCore::crImage> image = CRIOManager::readImageFile(tempbmpfileName);
		if (image.valid())
		{
			std::string fileName = CRCore::crArgumentParser::getFileNameEliminateExt(tempbmpfileName) + ".jpg";
			CRIOManager::writeImageFile(*image,fileName);
		}
		DeleteFile(tempbmpfileName);

		UpLoadBugFile(std::string(exeFilePath));
		//::Sleep(1000);
		WinExec(CRNetApp::crGlobalHandle::argvstr().c_str(),SW_NORMAL);
		TerminateProcess(GetCurrentProcess(),1);
	}
	else
	{
		CRNet::crNetConductor *dbServer = CRNet::crNetContainer::getInstance()->getNetConductor(CRNetApp::DBServer);
		if(dbServer)
		{
			CRNetApp::crDBServerCallback *callback = dynamic_cast<CRNetApp::crDBServerCallback *>(dbServer->getNetDataManager()->getNetCallback());
			if(callback)
			{
				callback->serverShutdown();
			}
		}
		//MessageBox(GetDesktopWindow(),CRNetApp::crGlobalHandle::argvstr().c_str(),"Creature3D",MB_OK);
		::Sleep(3000);
		WinExec(CRNetApp::crGlobalHandle::argvstr().c_str(),SW_NORMAL);
	}
	return EXCEPTION_EXECUTE_HANDLER;
}
