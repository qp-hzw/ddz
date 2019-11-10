#ifndef SETTING_HEAD_FILE
#define SETTING_HEAD_FILE

#include "Define.h"
#pragma once

/************************************************************************
** CONFIG  配置文件 
** 备注1: 只能修改对应的版本信息, 非自己的版本即使修改了也没用
          内核只能修改代码版本和内核版本
		  framework只能修改平台版本
		  子游戏只能修改子游戏版本
************************************************************************/

/* 游戏版本信息 */
//代码版本 1-254; 255为IUnknownEx
const BYTE CODE_VERSION = 
/* CODE_VERSION: */      4;

//内核版本 1-254; 255为IUnknownEx
const BYTE KERNEL_VERSION =
/* KERNEL_VERSION: */    5;

/************************************************************************
** FUNCTION  对外接口
************************************************************************/
/*
** 版本获取
*/
//获取代码 && Code版本
static DWORD Get_Kernel_Version()
{
	DWORD dwVersion = (CODE_VERSION<<24) + (KERNEL_VERSION<<16);
	return dwVersion;
}

//获取框架版本
static DWORD Get_Framework_Version(const BYTE PLATFORM_VERSION)
{
	DWORD dwVersion = (CODE_VERSION<<24) + (KERNEL_VERSION<<16) + (PLATFORM_VERSION<<8);
	return dwVersion;
}

//获取子游戏版本
static DWORD Get_SubGame_Version(const BYTE PLATFORM_VERSION, const BYTE SUBGAME_VERSION)
{
	DWORD dwVersion = (CODE_VERSION<<24) + (KERNEL_VERSION<<16) + (PLATFORM_VERSION<<8) + SUBGAME_VERSION;
	return dwVersion;
}

/*
** 版本比较
*/
//内核版本 -- 供内核代码使用
static DWORD INTERFACE_VERSION(BYTE cbCodeVer, BYTE cbKernelVer)	
{ 
	return (DWORD)((cbCodeVer<<24) + (cbKernelVer<<16) );
}

//代码版本
inline BYTE GetCodeVer(DWORD dwVersion){ return ((BYTE *)&dwVersion)[3];}
//内核版本
inline BYTE GetKernelVer(DWORD dwVersion){ return ((BYTE *)&dwVersion)[2];}
//平台版本
inline BYTE GetPlatformVer(DWORD dwVersion){ return ((BYTE *)&dwVersion)[1];}
//子游戏版本
inline BYTE GetSubGameVer(DWORD dwVersion){ return ((BYTE *)&dwVersion)[0];}


//游戏版本比较  范围 kernel与framework
static inline byte Compare_Kernek_Framework(DWORD Kernel_Version, DWORD framework_Version)
{
	if (GetCodeVer(Kernel_Version) != GetCodeVer(framework_Version)) return 1;
	if (GetKernelVer(Kernel_Version) != GetKernelVer(framework_Version)) return 1;

	return 0;// 0 满足要求； 其他值: 不满足要求
}

//游戏版本比较  范围: framework与dll(子游戏)
static inline byte Compare_Dll_Framework(DWORD dll_Version, DWORD framework_Version)
{
	if (GetCodeVer(dll_Version) != GetCodeVer(framework_Version)) return 1;
	if (GetKernelVer(dll_Version) != GetKernelVer(framework_Version)) return 1;
	if (GetPlatformVer(dll_Version) != GetPlatformVer(framework_Version)) return 1;

	return 0;// 0 满足要求； 其他值: 不满足要求
}

//游戏版本比较  范围: client的hall与LogonServer
static inline byte Compate_Hall_LogonServer(DWORD client_Version, DWORD LognServer_Version)
{
	//CODE版本不匹配 直接强制升级
	if (GetCodeVer(LognServer_Version) != GetCodeVer(client_Version)) return 1;

	// kernel版本
	if (GetKernelVer(LognServer_Version) != GetKernelVer(client_Version)) return 1;

	// platform 版本
	if (GetPlatformVer(LognServer_Version) != GetPlatformVer(client_Version)) return 1;

	// 0 满足要求； 1升级
	return 0;
}

//游戏版本比较  范围: client子游戏  和 server子游戏
static inline byte Compate_clientSubGame_ServerSubGame(DWORD client_subGame_Version, DWORD server_subGame_Version)
{
	if (GetCodeVer(client_subGame_Version) != GetCodeVer(server_subGame_Version)) return 2;
	if (GetKernelVer(client_subGame_Version) != GetKernelVer(server_subGame_Version)) return 2;
	if (GetPlatformVer(client_subGame_Version) != GetPlatformVer(server_subGame_Version)) return 2;

	// 0 满足要求； 1 建议升级 ；  2强制升级
	return 0;
}


//游戏版本比较 -- 仅供内核使用
static inline bool InterfaceVersionCompare(DWORD dwQueryVer, DWORD dwInterfaceVer)
{
	//CODE版本不匹配 直接false
	if (GetCodeVer(dwInterfaceVer) != GetCodeVer(dwQueryVer)) return false;
	// kernel 1.查询版本不能大于当下版本  2.内核版本相差2时必须升级
	if (GetKernelVer(dwInterfaceVer) < GetKernelVer(dwQueryVer)) return false;
	if ((GetKernelVer(dwInterfaceVer) - GetKernelVer(dwQueryVer)) >= 2 ) return false;
	// platform 1.查询版本不能大于当下版本  2.平台版本相差2时必须升级
	if (GetPlatformVer(dwInterfaceVer) < GetPlatformVer(dwQueryVer)) return false;
	if ((GetPlatformVer(dwInterfaceVer) - GetPlatformVer(dwQueryVer)) >=2 ) return false;
	// subgame 1.查询版本不能大于当下版本  2.子游戏版本相差3时必须升级
	if (GetSubGameVer(dwInterfaceVer) < GetSubGameVer(dwQueryVer)) return false;
	if ((GetSubGameVer(dwInterfaceVer) - GetSubGameVer(dwQueryVer)) >=3 ) return false;
	
	return true;
}

#define INCLUDE_IMPORT_FILES


#define SUB_INCLUDE_IMPORT_FILES

#endif
