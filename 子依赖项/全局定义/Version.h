#ifndef SETTING_HEAD_FILE
#define SETTING_HEAD_FILE

#include "Define.h"
#pragma once

/************************************************************************
** CONFIG  �����ļ� 
** ��ע1: ֻ���޸Ķ�Ӧ�İ汾��Ϣ, ���Լ��İ汾��ʹ�޸���Ҳû��
          �ں�ֻ���޸Ĵ���汾���ں˰汾
		  frameworkֻ���޸�ƽ̨�汾
		  ����Ϸֻ���޸�����Ϸ�汾
************************************************************************/

/* ��Ϸ�汾��Ϣ */
//����汾 1-254; 255ΪIUnknownEx
const BYTE CODE_VERSION = 
/* CODE_VERSION: */      4;

//�ں˰汾 1-254; 255ΪIUnknownEx
const BYTE KERNEL_VERSION =
/* KERNEL_VERSION: */    5;

/************************************************************************
** FUNCTION  ����ӿ�
************************************************************************/
/*
** �汾��ȡ
*/
//��ȡ���� && Code�汾
static DWORD Get_Kernel_Version()
{
	DWORD dwVersion = (CODE_VERSION<<24) + (KERNEL_VERSION<<16);
	return dwVersion;
}

//��ȡ��ܰ汾
static DWORD Get_Framework_Version(const BYTE PLATFORM_VERSION)
{
	DWORD dwVersion = (CODE_VERSION<<24) + (KERNEL_VERSION<<16) + (PLATFORM_VERSION<<8);
	return dwVersion;
}

//��ȡ����Ϸ�汾
static DWORD Get_SubGame_Version(const BYTE PLATFORM_VERSION, const BYTE SUBGAME_VERSION)
{
	DWORD dwVersion = (CODE_VERSION<<24) + (KERNEL_VERSION<<16) + (PLATFORM_VERSION<<8) + SUBGAME_VERSION;
	return dwVersion;
}

/*
** �汾�Ƚ�
*/
//�ں˰汾 -- ���ں˴���ʹ��
static DWORD INTERFACE_VERSION(BYTE cbCodeVer, BYTE cbKernelVer)	
{ 
	return (DWORD)((cbCodeVer<<24) + (cbKernelVer<<16) );
}

//����汾
inline BYTE GetCodeVer(DWORD dwVersion){ return ((BYTE *)&dwVersion)[3];}
//�ں˰汾
inline BYTE GetKernelVer(DWORD dwVersion){ return ((BYTE *)&dwVersion)[2];}
//ƽ̨�汾
inline BYTE GetPlatformVer(DWORD dwVersion){ return ((BYTE *)&dwVersion)[1];}
//����Ϸ�汾
inline BYTE GetSubGameVer(DWORD dwVersion){ return ((BYTE *)&dwVersion)[0];}


//��Ϸ�汾�Ƚ�  ��Χ kernel��framework
static inline byte Compare_Kernek_Framework(DWORD Kernel_Version, DWORD framework_Version)
{
	if (GetCodeVer(Kernel_Version) != GetCodeVer(framework_Version)) return 1;
	if (GetKernelVer(Kernel_Version) != GetKernelVer(framework_Version)) return 1;

	return 0;// 0 ����Ҫ�� ����ֵ: ������Ҫ��
}

//��Ϸ�汾�Ƚ�  ��Χ: framework��dll(����Ϸ)
static inline byte Compare_Dll_Framework(DWORD dll_Version, DWORD framework_Version)
{
	if (GetCodeVer(dll_Version) != GetCodeVer(framework_Version)) return 1;
	if (GetKernelVer(dll_Version) != GetKernelVer(framework_Version)) return 1;
	if (GetPlatformVer(dll_Version) != GetPlatformVer(framework_Version)) return 1;

	return 0;// 0 ����Ҫ�� ����ֵ: ������Ҫ��
}

//��Ϸ�汾�Ƚ�  ��Χ: client��hall��LogonServer
static inline byte Compate_Hall_LogonServer(DWORD client_Version, DWORD LognServer_Version)
{
	//CODE�汾��ƥ�� ֱ��ǿ������
	if (GetCodeVer(LognServer_Version) != GetCodeVer(client_Version)) return 1;

	// kernel�汾
	if (GetKernelVer(LognServer_Version) != GetKernelVer(client_Version)) return 1;

	// platform �汾
	if (GetPlatformVer(LognServer_Version) != GetPlatformVer(client_Version)) return 1;

	// 0 ����Ҫ�� 1����
	return 0;
}

//��Ϸ�汾�Ƚ�  ��Χ: client����Ϸ  �� server����Ϸ
static inline byte Compate_clientSubGame_ServerSubGame(DWORD client_subGame_Version, DWORD server_subGame_Version)
{
	if (GetCodeVer(client_subGame_Version) != GetCodeVer(server_subGame_Version)) return 2;
	if (GetKernelVer(client_subGame_Version) != GetKernelVer(server_subGame_Version)) return 2;
	if (GetPlatformVer(client_subGame_Version) != GetPlatformVer(server_subGame_Version)) return 2;

	// 0 ����Ҫ�� 1 �������� ��  2ǿ������
	return 0;
}


//��Ϸ�汾�Ƚ� -- �����ں�ʹ��
static inline bool InterfaceVersionCompare(DWORD dwQueryVer, DWORD dwInterfaceVer)
{
	//CODE�汾��ƥ�� ֱ��false
	if (GetCodeVer(dwInterfaceVer) != GetCodeVer(dwQueryVer)) return false;
	// kernel 1.��ѯ�汾���ܴ��ڵ��°汾  2.�ں˰汾���2ʱ��������
	if (GetKernelVer(dwInterfaceVer) < GetKernelVer(dwQueryVer)) return false;
	if ((GetKernelVer(dwInterfaceVer) - GetKernelVer(dwQueryVer)) >= 2 ) return false;
	// platform 1.��ѯ�汾���ܴ��ڵ��°汾  2.ƽ̨�汾���2ʱ��������
	if (GetPlatformVer(dwInterfaceVer) < GetPlatformVer(dwQueryVer)) return false;
	if ((GetPlatformVer(dwInterfaceVer) - GetPlatformVer(dwQueryVer)) >=2 ) return false;
	// subgame 1.��ѯ�汾���ܴ��ڵ��°汾  2.����Ϸ�汾���3ʱ��������
	if (GetSubGameVer(dwInterfaceVer) < GetSubGameVer(dwQueryVer)) return false;
	if ((GetSubGameVer(dwInterfaceVer) - GetSubGameVer(dwQueryVer)) >=3 ) return false;
	
	return true;
}

#define INCLUDE_IMPORT_FILES


#define SUB_INCLUDE_IMPORT_FILES

#endif
