#include "StdAfx.h"
#include "Tableframesink.h"
#include "GameServerManager.h"
#include "AndroidUserItemSink.h"////////
#include "GameConfig.h"


//���캯��
CGameServiceManager::CGameServiceManager(void)
{
	//��־
	m_GameServiceAttrib.dwGameID= (KIND_ID<<16) + (NODE_ID<<8);

	//
	m_GameServiceAttrib.wChairCount=MAX_CHAIR_COUNT;
	//m_GameServiceAttrib.wSupporType=GAME_GENRE;

	//�汾��Ϣ
	//m_GameServiceAttrib.dwSubGameVersion = Get_SubGame_Version();
	TCHAR szDllName[32];
	wsprintf(szDllName, TEXT("%d_%d.dll"), KIND_ID, NODE_ID);
	memcpy_s(m_GameServiceAttrib.szDllName, sizeof(m_GameServiceAttrib.szDllName), szDllName, sizeof(m_GameServiceAttrib.szDllName));
	
	return;
}

//��������
CGameServiceManager::~CGameServiceManager(void)
{
}

//�ӿڲ�ѯ
VOID * CGameServiceManager::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameServiceManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameServiceManager,Guid,dwQueryVer);

	return NULL;
}

//������Ϸ��
VOID * CGameServiceManager::CreateTableFrameSink(REFGUID Guid, DWORD dwQueryVer)
{
	//��������
	CTableFrameSink * pTableFrameSink=NULL;
	try
	{
		pTableFrameSink=new CTableFrameSink();
		if (pTableFrameSink==NULL) throw TEXT("����ʧ��");
		void * pObject=pTableFrameSink->QueryInterface(Guid,dwQueryVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pTableFrameSink);

	return NULL;
}

//��ȡ����
bool CGameServiceManager::GetServiceAttrib(tagGameServiceAttrib & GameServiceAttrib)
{
	GameServiceAttrib=m_GameServiceAttrib;
	return true;
}

//�����޸�
bool CGameServiceManager::RectifyParameter(tagGameServiceOption & GameServiceOption)
{
	//Ч�����
	ASSERT(&GameServiceOption!=NULL);
	if (&GameServiceOption==NULL) 
	{
		return false;
	}

	return true;
}

//��ȡ����
bool CGameServiceManager::SaveCustomRule(LPBYTE pcbCustomRule, WORD wCustonSize)
{
	return true;
}
	
//Ĭ������
bool CGameServiceManager::DefaultCustomRule(LPBYTE pcbCustomRule, WORD wCustonSize)
{
	return true;
}

//��������
HWND CGameServiceManager::CreateCustomRule(CWnd * pParentWnd, CRect rcCreate, LPBYTE pcbCustomRule, WORD wCustonSize)
{
	return NULL;
}

//��������
VOID * CGameServiceManager::CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer)
{
	CAndroidUserItemSink * pAndroidUserItemSink=NULL;

	try
	{
		//��������
		pAndroidUserItemSink=new CAndroidUserItemSink();
		if (pAndroidUserItemSink==NULL) throw TEXT("����ʧ��");

		//��ѯ�ӿ�
		void * pObject=pAndroidUserItemSink->QueryInterface(Guid,dwQueryVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");

		return pObject;
	}
	catch (...) {}

	//ɾ������
	SafeDelete(pAndroidUserItemSink);

	return NULL;
}

//��������
VOID * CGameServiceManager::CreateGameDataBaseEngineSink(REFGUID Guid, DWORD dwQueryVer)
{
	return NULL;
}

//////////////////////////////////////////////////////////////////////////

//��ɾ����

//���������� 
//extern "C" __declspec(dllexport) VOID * CreateGameServiceManager(REFGUID Guid, DWORD dwInterfaceVer)
//{
//	//��ȡ�����ļ��ʹ�����־
//	CGameConfig::LoadGameCommonConfig();
//
//	/*static CGameServiceManager GameServiceManager;
//	return GameServiceManager.QueryInterface(Guid,dwInterfaceVer);*/
//
//	CGameServiceManager *pGameServiceManager = new CGameServiceManager();
//	if (pGameServiceManager == NULL)
//		return false;
//	return pGameServiceManager;
//}

/////////////////////////////////////////////////////////////////////////
