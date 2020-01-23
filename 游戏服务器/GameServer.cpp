#include "Stdafx.h"
#include <afxdllx.h>

//////////////////////////////////////////////////////////////////////////

static AFX_EXTENSION_MODULE GameServerDLL={NULL,NULL};


//DLL 导出主函数
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(GameServerDLL, hInstance)) return 0;
		new CDynLinkLibrary(GameServerDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(GameServerDLL);
	}
	return 1;
}


/*
//DLL 导出主函数
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(GameServerDLL, hInstance)) return 0;
		new CDynLinkLibrary(GameServerDLL);

		//初始化 COM
		CoInitialize(NULL);

		//初始化 SOCKET
		WSADATA WSAData;
		WORD wVersionRequested = MAKEWORD(2, 2);
		int iErrorCode = WSAStartup(wVersionRequested, &WSAData);
		if (iErrorCode != 0) return 0;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		CoUninitialize();
		AfxTermExtensionModule(GameServerDLL);
	}

	return 1;
}
*/

//////////////////////////////////////////////////////////////////////////
