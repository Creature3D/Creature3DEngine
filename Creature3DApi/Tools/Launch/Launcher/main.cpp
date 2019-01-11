#if defined(WIN32) && !defined(__CYGWIN__)
#include <Io.h>
#include <Windows.h>
#include <Winbase.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h> // for _mkdir

//#include <ShlObj.h>

#define mkdir(x,y) _mkdir((x))
#define stat64 _stati64

// set up for windows so acts just like unix access().
#define F_OK 4


#else // unix

#if defined( __APPLE__ ) || defined(__CYGWIN__) || defined(__FreeBSD__)
#define stat64 stat
#endif

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

// set up _S_ISDIR()
#if !defined(S_ISDIR)
#  if defined( _S_IFDIR) && !defined( __S_IFDIR)
#    define __S_IFDIR _S_IFDIR
#  endif
#  define S_ISDIR(mode)    (mode&__S_IFDIR)
#endif
#include <string>
#include <fstream>
#include <time.h>
#include <vector>
#include <fstream>
#include <stack>
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
void findFileInDir(const char* rootDir, std::vector<std::string>& fileNameVec)
{
	std::string rdir = rootDir;
	char fname[1024];
	ZeroMemory(fname, 1024);

	WIN32_FIND_DATA fd;
	ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));

	HANDLE hSearch;

	char filePathName[1024];
	char tmpPath[1024];
	ZeroMemory(filePathName, 1024);
	ZeroMemory(tmpPath, 1024);

	strcpy(filePathName, rootDir);

	bool bSearchFinished = FALSE;

	if( filePathName[strlen(filePathName) -1] != '\\' )
	{
		strcat(filePathName, "\\");
	}
	strcat(filePathName, "*");
	hSearch = FindFirstFile(filePathName, &fd);

	if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		&& strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") )
	{
		strcpy(tmpPath, rootDir);
		strcat(tmpPath, fd.cFileName);
		strcat(tmpPath, "\\");
		findFileInDir(tmpPath, fileNameVec);
	}
	else if( strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") )
	{
		if( rootDir[strlen(rootDir) -1] != '\\' )
			sprintf(fname, "%s\\%s\0",rootDir,fd.cFileName);
		else
			sprintf(fname, "%s%s\0",rootDir,fd.cFileName);
		fileNameVec.push_back(fname);
	}

	while( !bSearchFinished )
	{
		if( FindNextFile(hSearch, &fd) )
		{
			if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") )       
			{
				strcpy(tmpPath, rootDir);
				strcat(tmpPath, fd.cFileName);
				strcat(tmpPath, "\\");
				findFileInDir(tmpPath, fileNameVec);
			}
			else if( strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") )
			{
				if( rootDir[strlen(rootDir) -1] != '\\' )
					sprintf(fname, "%s\\%s\0",rootDir,fd.cFileName);
				else
					sprintf(fname, "%s%s\0",rootDir,fd.cFileName);
				fileNameVec.push_back(fname);
			}
		}
		else
		{
			if( GetLastError() == ERROR_NO_MORE_FILES )
			{
				bSearchFinished = TRUE;
			}
			else
				bSearchFinished = TRUE;
		}
	}
	FindClose(hSearch);
}
bool fileExists(const std::string& filename)
{
	return access( filename.c_str(), F_OK ) == 0;
}
std::string getFilePath(const std::string& fileName)
{
	std::string::size_type slash1 = fileName.find_last_of('/');
	std::string::size_type slash2 = fileName.find_last_of('\\');
	if (slash1==std::string::npos) 
	{
		if (slash2==std::string::npos) return std::string();
		return std::string(fileName,0,slash2);
	}
	if (slash2==std::string::npos) return std::string(fileName,0,slash1);
	return std::string(fileName, 0, slash1>slash2 ?  slash1 : slash2);
}
std::string convertFileNameToUnixStyle(const std::string& fileName)
{
	std::string new_fileName(fileName);

	std::string::size_type slash = 0;
	while( (slash=new_fileName.find_first_of('\\',slash)) != std::string::npos)
	{
		new_fileName[slash]='/';
	}

	return new_fileName;
}
bool makeDirectory( const std::string &path )
{
	if (path.empty())
	{
		return false;
	}

	struct stat64 stbuf;
	if( stat64( path.c_str(), &stbuf ) == 0 )
	{
		if( S_ISDIR(stbuf.st_mode))
			return true;
		else
		{
			return false;
		}
	}

	std::string dir = path;
	std::stack<std::string> paths;
	while( true )
	{
		if( dir.empty() )
			break;

		if( stat64( dir.c_str(), &stbuf ) < 0 )
		{
			switch( errno )
			{
			case ENOENT:
			case ENOTDIR:
				paths.push( dir );
				break;

			default:
				return false;
			}
		}
		dir = getFilePath(std::string(dir));
	}

	while( !paths.empty() )
	{
		std::string dir = paths.top();
#if defined(WIN32)
		if (dir.size() == 2 && dir.c_str()[1] == ':') {
			paths.pop();
			continue;
		}
#endif
		if( mkdir( dir.c_str(), 0755 )< 0 )
		{
			return false;
		} 
		paths.pop();
	}
	return true;
}
bool makeDirectoryForFile( const std::string &path )
{
	return makeDirectory( getFilePath( path ));
}
bool copyFile(const std::string &srcFile, const std::string &destFile)
{
	if(fileExists(destFile))
	{
		std::string _destFile = "_"+destFile;
		makeDirectoryForFile(_destFile);
		if(fileExists(_destFile))
			DeleteFileA(_destFile.c_str());
		MoveFile(destFile.c_str(),_destFile.c_str());
	}
	if(fileExists(srcFile))
	{
		std::string src = convertFileNameToUnixStyle(srcFile);
		if(src.find("./") == 0)
		{
			src.erase(0,2);
		}
		std::string dest = convertFileNameToUnixStyle(destFile);
		if(dest.find("./") == 0)
		{
			dest.erase(0,2);
		}
		if(src.compare(dest) != 0)
		{
			makeDirectoryForFile(destFile);
			std::ofstream out(destFile.c_str(),std::ios::binary|std::ios::out);
			if(!out)
			{
				char buf[128];
				sprintf(buf,"错误:无法更新文件[%s],请确认文件是否被锁定(比如被杀毒软件错误隔离)。\0",destFile.c_str());
				MessageBox(::GetDesktopWindow(),buf,"启动更新",MB_OK);
				return false;
			}
			else
			{
				std::ifstream in(srcFile.c_str(),std::ios::binary|std::ios::in);
				out << in.rdbuf();
				out.close();
				in.close();
				return true;
			}
		}
	}
	return false;
}
int main( int argc, char **argv )
{
	//if( fileExists("reg.ini"))
	//{
	//	DeleteFile("reg.ini");
	//	//注册插件
	//	char buf[256];
	//	memset(buf,0,256);
	//	PROCESS_INFORMATION pi;
	//	STARTUPINFO si; //This is an [in] parameter
	//	ZeroMemory(&si, sizeof(si));
	//	si.cb = sizeof si; //Only compulsory field
	//	si.dwFlags = STARTF_USESHOWWINDOW;
	//	si.wShowWindow = SW_HIDE;
	//	sprintf_s(buf,"regsvr32 /s Bin/Creature3DX.ocx");
	//	if(CreateProcess(NULL, buf, NULL,NULL,FALSE,0,NULL, NULL,&si,&pi))
	//	{
	//		WaitForSingleObject(pi.hProcess,INFINITE);
	//		CloseHandle(pi.hThread);
	//		CloseHandle(pi.hProcess);
	//	}
	//	sprintf_s(buf,"RegisterKeyNP.exe -reg");
	//	if(CreateProcess(NULL, buf, NULL,NULL,FALSE,0,NULL, NULL,&si,&pi))
	//	{
	//		WaitForSingleObject(pi.hProcess,INFINITE);
	//		CloseHandle(pi.hThread);
	//		CloseHandle(pi.hProcess);
	//	}
	//	WinExec("Login.exe",SW_SHOWNORMAL);
	//	return 0;
	//}
	//else if( fileExists("regweb.ini"))
	//{
	//	DeleteFile("regweb.ini");
	//	//注册插件
	//	char buf[256];
	//	memset(buf,0,256);
	//	PROCESS_INFORMATION pi;
	//	STARTUPINFO si; //This is an [in] parameter
	//	ZeroMemory(&si, sizeof(si));
	//	si.cb = sizeof si; //Only compulsory field
	//	si.dwFlags = STARTF_USESHOWWINDOW;
	//	si.wShowWindow = SW_HIDE;
	//	sprintf_s(buf,"regsvr32 /s Bin/creActiveX.ocx");
	//	if(CreateProcess(NULL, buf, NULL,NULL,FALSE,0,NULL, NULL,&si,&pi))
	//	{
	//		WaitForSingleObject(pi.hProcess,INFINITE);
	//		CloseHandle(pi.hThread);
	//		CloseHandle(pi.hProcess);
	//	}
	//	sprintf_s(buf,"RegisterNP.exe -reg");
	//	if(CreateProcess(NULL, buf, NULL,NULL,FALSE,0,NULL, NULL,&si,&pi))
	//	{
	//		WaitForSingleObject(pi.hProcess,INFINITE);
	//		CloseHandle(pi.hThread);
	//		CloseHandle(pi.hProcess);
	//	}
	//}
	//else if( fileExists("qqtest.ini"))
	//{
	//	DeleteFile("qqtest.ini");
	//	//std::string destfile = "D:\\国战三国\\JXJMiniClient.7z";
	//	//makeDirectoryForFile(destfile);
	//	//copyFile("JXJMiniClient.7z",destfile);
	//	//destfile = "D:\\国战三国\\7zr.exe";
	//	//copyFile("7zr.exe",destfile);

	//	std::string file = "JXJMiniClient.7z";
	//	SetCurrentDirectory("D:\\国战三国");
	//	char buf[256];
	//	memset(buf,0,256);
	//	sprintf_s(buf,"7zr.exe x -r %s -y",file.c_str());
	//	PROCESS_INFORMATION pi;
	//	STARTUPINFO si; //This is an [in] parameter
	//	ZeroMemory(&si, sizeof(si));
	//	si.cb = sizeof si; //Only compulsory field
	//	si.dwFlags = STARTF_USESHOWWINDOW;
	//	si.wShowWindow = SW_HIDE;
	//	if(CreateProcess(NULL, buf, NULL,NULL,FALSE,0,NULL, NULL,&si,&pi))
	//	{
	//		WaitForSingleObject(pi.hProcess,INFINITE);
	//		CloseHandle(pi.hThread);
	//		CloseHandle(pi.hProcess);
	//	}
	//	else
	//	{
	//		sprintf_s(buf,"解压缩[%s]出错!\0",file.c_str());
	//		MessageBox(::GetActiveWindow(),buf,"国战三国",MB_OK);
	//		return false;
	//	}

	//	std::string launchfile = "D:\\国战三国\\Launcher.exe";

	//	HRESULT hr = CoInitialize(NULL);
	//	if (SUCCEEDED(hr))
	//	{
	//		IShellLink *pisl;
	//		hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pisl);
	//		if (SUCCEEDED(hr))
	//		{
	//			IPersistFile* pIPF;

	//			//这里是我们要创建快捷方式的原始文件地址
	//			pisl->SetPath(launchfile.c_str());
	//			hr = pisl->QueryInterface(IID_IPersistFile, (void**)&pIPF);
	//			if (SUCCEEDED(hr))
	//			{

	//				/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//				//这里是我们要创建快捷方式的目标地址
	//				TCHAR szDesktopPath[MAX_PATH] = {0}, szPath[MAX_PATH];
	//				SHGetSpecialFolderPath(NULL, szDesktopPath, CSIDL_DESKTOP, FALSE);
	//				wchar_t szBuf[255] = {0};
	//				wsprintfW(szBuf, L"%s/国战三国.lnk", szDesktopPath);

	//				pIPF->Save(szBuf, FALSE);
	//				pIPF->Release();
	//			}
	//			pisl->Release();
	//		}
	//		CoUninitialize();
	//	}

	//	WinExec("Launcher.exe",SW_SHOWNORMAL);
	//	return 0;
	//}

	Sleep(1000);
	HANDLE hMutex = NULL;
	if(argc==1)
	{
		hMutex = CreateMutex(NULL, false, "Creature3D");
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			CloseHandle(hMutex);
			MessageBox(::GetDesktopWindow(),"程序已经在运行中，不能重复启动！","Creature3D Launcher",MB_OK);
			return 0;
		}
	}
	char *loginkey = NULL;
	struct stat buf;
	if(!stat("jxjlogin.dat", &buf ))
	{
		time_t mtime = buf.st_mtime;
		time_t now = time(0);
		if(now - mtime<570)
		{
			std::ifstream fin("jxjlogin.dat",std::ios::in|std::ios::binary);
			if(fin)
			{
				fin.seekg(0, std::ios::end);
				int length = fin.tellg();
				loginkey = new char[length+1];
				memset(loginkey,0,length+1);
				fin.seekg(0, std::ios::beg);
				fin.read(loginkey, length);
				fin.close();
			}
		}
	}
	std::vector<std::string> fileNameVec;
	findFileInDir("jxjpkg\\bin",fileNameVec);
	if(!fileNameVec.empty())
	{
		//system("taskkill /im Jianxingjue.exe /f");
		//system("taskkill /im Jiangxingjue.exe /f");
		PROCESS_INFORMATION pi;
		STARTUPINFO si; //This is an [in] parameter
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof si; //Only compulsory field
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		if(CreateProcess(NULL, "taskkill /im Jiangxingjue.exe /f", NULL,NULL,FALSE,0,NULL, NULL,&si,&pi))
		{
			WaitForSingleObject(pi.hProcess,INFINITE);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
		else
		{
			MessageBox(::GetDesktopWindow(),"错误：无法结束进程Jiangxingjue.exe","启动更新",MB_OK);
		}
		Sleep(1000);
		//system("move /y jxjpkg\\bin\\*.* bin");
		std::string filename;
		for( std::vector<std::string>::iterator itr = fileNameVec.begin();
			 itr != fileNameVec.end();
			 ++itr )
		{
			filename = *itr;
			filename.erase(0,7);
			if(copyFile(*itr,filename))
				DeleteFileA((*itr).c_str());
		}
	}
	if(hMutex)
		CloseHandle(hMutex);

#if 0//限制双开
	PROCESS_INFORMATION pi1;
	STARTUPINFO si1; //This is an [in] parameter
	ZeroMemory(&si1, sizeof(si1));
	si1.cb = sizeof si1; //Only compulsory field
	si1.dwFlags = STARTF_USESHOWWINDOW;
	si1.wShowWindow = SW_HIDE;
	if(CreateProcess(NULL, "taskkill /im Jiangxingjue.exe /f", NULL,NULL,FALSE,0,NULL, NULL,&si1,&pi1))
	{
		WaitForSingleObject(pi1.hProcess,INFINITE);
		CloseHandle(pi1.hThread);
		CloseHandle(pi1.hProcess);
	}
	else
	{
		MessageBox(::GetDesktopWindow(),"错误：无法结束进程Jiangxingjue.exe","禁止双开",MB_OK);
		exit(0);
	}
#endif

	if(1<argc && (strcmp(argv[1],"Robot") == 0||strcmp(argv[1],"RobotNoTexture") == 0))
	{
		int start = atoi(argv[2]);
		int end = atoi(argv[3]);
		int serverid = atoi(argv[4]);//1
		int isp = atoi(argv[5]);//1电信2网通
		char buf[128];
		memset(buf,0,128);
		for(int i = start; i<end; i++)
		{
			sprintf_s(buf,"bin\\Jiangxingjue.exe %s -WebLogin Creature3DRobot$@#|%d|%d|%d",argv[1],i,serverid,isp);//key|playerid|serverid|isp
			WinExec(buf,SW_SHOWNORMAL);
			Sleep(2000);
		}
	}
	else
	{
		if(loginkey)
		{
			char buf[128];
			memset(buf,0,128);
#ifndef _DEBUG
			sprintf_s(buf,"bin\\Jiangxingjue.exe -WebLogin %s",loginkey);
#else
			sprintf_s(buf,"bin\\Creature3Dd.exe -WebLogin %s",loginkey);
#endif
			WinExec(buf,SW_SHOWNORMAL);
		}
		else
		{
			char *web = NULL;
			if(!stat("web.ini", &buf ))
			{
				std::ifstream fin("web.ini",std::ios::in|std::ios::binary);
				if(fin)
				{
					fin.seekg(0, std::ios::end);
					int length = fin.tellg();
					web = new char[length+1];
					memset(web,0,length+1);
					fin.seekg(0, std::ios::beg);
					fin.read(web, length);
					fin.close();
				}
			}
			if(web)
			{
				ShellExecute(::GetActiveWindow(),"open",web,NULL,NULL,SW_SHOWNORMAL);
				delete [] web;
				web = NULL;
			}
			else
			{
#ifndef _DEBUG
				WinExec("bin\\Jiangxingjue.exe",SW_SHOWNORMAL);
#else
				WinExec("bin\\Creature3Dd.exe",SW_SHOWNORMAL);
#endif
			}
		}
	}
	if(loginkey)
	{
		delete [] loginkey;
		loginkey = NULL;
	}
	//ShellExecuteA(::GetDesktopWindow(),"open","Client.cmd",NULL,NULL,SW_SHOWNORMAL);
}