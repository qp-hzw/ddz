#ifndef SERVICE_CORE_HEAD_FILE
#define SERVICE_CORE_HEAD_FILE

//MFC �ļ�
#include <Afxmt.h>
#include <AtlBase.h>

//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef SERVICE_CORE_CLASS
	#ifdef  SERVICE_CORE_DLL
		#define SERVICE_CORE_CLASS _declspec(dllexport)
	#else
		#define SERVICE_CORE_CLASS _declspec(dllimport)
	#endif
#endif

#endif