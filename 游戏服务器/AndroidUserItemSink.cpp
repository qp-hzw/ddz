#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS					1									//����ʱ��

//��Ϸʱ��
#define TIME_USER_CALL_BANKER		2									//��ׯ��ʱ��
#define TIME_USER_START_GAME		3									//��ʼ��ʱ��
#define TIME_USER_ADD_SCORE			3									//��ע��ʱ��
#define TIME_USER_OPEN_CARD			12									//̯�ƶ�ʱ��

//��Ϸʱ��
#define IDI_START_GAME			(IDI_ANDROID_ITEM_SINK+0)			//��ʼʱ��
#define IDI_CALL_BANKER			(IDI_ANDROID_ITEM_SINK+1)			//��ׯʱ��
#define IDI_USER_ADD_SCORE		(IDI_ANDROID_ITEM_SINK+2)			//��עʱ��
#define IDI_OPEN_CARD			(IDI_ANDROID_ITEM_SINK+3)			//����ʱ��
#define IDI_BANKER						105							//�����˴�ȡ��

#define IDI_ANDROID_ITEM_SINK		500////////////////

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_lTurnMaxScore = 0;
	m_lDiZhu = 0;
	// memset( &m_HandCardData, 0, sizeof m_HandCardData);

	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));///////

	//�ӿڱ���
	m_pIAndroidUserItem=NULL;

	return;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
void * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ�ӿ�
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//���ýӿ�
bool CAndroidUserItemSink::RepositionSink()
{
	m_lTurnMaxScore = 0;
	// memset( &m_HandCardData, 0, sizeof m_HandCardData);
	memset( &m_cbPlayStatus, 0, sizeof m_cbPlayStatus);

	return true;
}

//ʱ����Ϣ
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//��ʼ��ʱ��
		{
			CMD_C_PlayerAction pPlayerAction;
			//pPlayerAction.iActionID = E_INGAME;  TODONOW ������ʲô������

			//����׼��
			m_pIAndroidUserItem->SendUserReady(NULL,0);

			return true;
		}
	case IDI_CALL_BANKER:		//��ׯ��ʱ
		{
			//���ñ���
			CMD_C_CallBanker CallBanker;
			CallBanker.bBanker =rand()%2;
			if(m_pIAndroidUserItem->GetMeUserItem()->GetUserScore() < (m_cbUserCount -1)*m_lDiZhu*30)
			{
				CallBanker.bBanker = 0;
			}

			//������Ϣ
			m_pIAndroidUserItem->SendSocketData(SUB_C_A_CALL_BANKER,&CallBanker,sizeof(CallBanker));

			return true;
		}
	case IDI_OPEN_CARD:			//���ƶ�ʱ
		{
			m_pIAndroidUserItem->SendSocketData(SUB_C_A_OPEN_CARD,0,0);
			return true;	
		}
	case IDI_BANKER:
		{
			BankOperate(2);
			return true;
		}

	}

	return false;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_A_CALL:
		{
			return true;
		}
	case SUB_S_A_CALL_BANKER:	//�û���ׯ
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_PlayerCount)) return false;
			CMD_S_PlayerCount * pPlayerCount=(CMD_S_PlayerCount *)pData;

			
			m_cbUserCount=pPlayerCount->wPlayerCount;
			//��ׯʱ��
			UINT nElapse=rand()%(3)+2;
			m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER,nElapse);
			return true;

		}
		/*
	case CMD_RC_BET:	//��ע��ʼ
		{
			//��Ϣ����
			return OnSubGameStart(pData,wDataSize);
		}
		*/
	case SUB_S_A_ADD_SCORE:	//�û���ע
		{
			//��Ϣ����
			return OnSubAddScore(pData,wDataSize);
		}
	case CMD_SC_SEND_CARD:	//������Ϣ
		{
			//��Ϣ����
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_A_OPEN_CARD:	//�û�̯��
		{
			//��Ϣ����
			return OnSubOpenCard(pData,wDataSize);
		}
	case SUB_S_A_PLAYER_EXIT:	//�û�ǿ��
		{
			//��Ϣ����
			return OnSubPlayerExit(pData,wDataSize);
		}
	case SUB_S_A_GAME_END:	//��Ϸ����
		{
			//��Ϣ����
			return OnSubGameEnd(pData,wDataSize);
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(STR_CMD_SC_STATUS_FREE));
			if (wDataSize!=sizeof(STR_CMD_SC_STATUS_FREE)) return false;

			//��Ϣ����
			STR_CMD_SC_STATUS_FREE * pStatusFree=(STR_CMD_SC_STATUS_FREE *)pData;

			//��ʼʱ��
			UINT nElapse=rand()%(3)+TIME_LESS;
			m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

			//���в���
			BankOperate(2);
			m_lDiZhu = pStatusFree->CellScore;

			//�µ�ע
			// memcpy(m_lTurnMaxScore, pStatusFree->lCellScore, sizeof(m_lTurnMaxScore));

			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//�û�����
void CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
void CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
void CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
void CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û���λ
void CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û���ׯ
bool CAndroidUserItemSink::OnSubCallBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_CallBanker)) return false;
	CMD_S_CallBanker * pCallBanker=(CMD_S_CallBanker *)pBuffer;

	//ʼ���û�
	if(pCallBanker->wCallBanker==m_pIAndroidUserItem->GetChairID())
	{

		//��ׯʱ��
		UINT nElapse=rand()%(3)+1;
		m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER,nElapse);
	}
	return true;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	////Ч������
	//if (wDataSize!=sizeof(STR_CMD_SC_BET_START)) return false;
	//STR_CMD_SC_BET_START * pGameStart=(STR_CMD_SC_BET_START *)pBuffer;

	return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//Ч������

	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(STR_CMD_SC_SEND_CARD)) return false;
	STR_CMD_SC_SEND_CARD * pSendCard=(STR_CMD_SC_SEND_CARD *)pBuffer;

	//��������
	WORD wMeChiarID=m_pIAndroidUserItem->GetChairID();
	// memcpy_s(&m_HandCardData, sizeof m_HandCardData, &pSendCard->cbCardData[wMeChiarID],  sizeof m_HandCardData);

	////for(BYTE i=0;i<MAX_CHAIR_COUNT;i++)
	////{
	////	m_cbPlayStatus[i]=pGameStart->cbPlayStatus[i];
	////}

	//������Ϸ��ʱ����////////////


	//����ʱ��
	UINT nElapse=rand()%(3)+m_cbUserCount*2;////////
	m_pIAndroidUserItem->SetGameTimer(IDI_OPEN_CARD,nElapse);

	return true;
}
//���в���
void CAndroidUserItemSink::BankOperate(BYTE cbType)
{
	IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
	if(pUserItem->GetUserStatus()>=US_SIT)
	{
		if(cbType==1)
		{
			CString strInfo;
			strInfo.Format(TEXT("������״̬���ԣ���ִ�д�ȡ��"));
			// NcaTextOut(strInfo);
			return;
		}
	}

	//��������
	SCORE lRobotScore = pUserItem->GetUserScore();

	{
		CString strInfo;
		strInfo.Format(TEXT("[%s] ����(%I64d)"),pUserItem->GetNickName(),lRobotScore);

		if (lRobotScore > m_pIAndroidUserItem->GetAndroidParameter()->lMaxTakeScore)
		{
			CString strInfo1;
			strInfo1.Format(TEXT("����������(%I64d)"),m_pIAndroidUserItem->GetAndroidParameter()->lMaxTakeScore);
			strInfo+=strInfo1;

			// NcaTextOut(strInfo);
		}
		else if (lRobotScore < m_pIAndroidUserItem->GetAndroidParameter()->lMinTakeScore)
		{
			CString strInfo1;
			strInfo1.Format(TEXT("����ȡ������(%I64d)"),m_pIAndroidUserItem->GetAndroidParameter()->lMinTakeScore);
			strInfo+=strInfo1;

			// NcaTextOut(strInfo);
		}

		//�жϴ�ȡ
		if (lRobotScore >m_pIAndroidUserItem->GetAndroidParameter()->lMaxTakeScore)
		{			
			SCORE lSaveScore=0L;
			lSaveScore = lRobotScore - m_pIAndroidUserItem->GetAndroidParameter()->lMaxTakeScore;

			if (lSaveScore > 0) 
			{
				lSaveScore += (rand()%(int)(m_pIAndroidUserItem->GetAndroidParameter()->lMaxTakeScore - m_pIAndroidUserItem->GetAndroidParameter()->lMinTakeScore));
				m_pIAndroidUserItem->PerformSaveScore(lSaveScore);
			}
			SCORE lRobotNewScore = pUserItem->GetUserScore();

			CString strInfo;
			strInfo.Format(TEXT("[%s] ִ�д����ǰ���(%I64d)��������(%I64d)"),pUserItem->GetNickName(),lRobotScore,lRobotNewScore);

			// NcaTextOut(strInfo);
		}
		else if (lRobotScore < m_pIAndroidUserItem->GetAndroidParameter()->lMinTakeScore)
		{
			CString strInfo;
			SCORE lScore=m_pIAndroidUserItem->GetAndroidParameter()->lMinTakeScore-lRobotScore;
			if (lScore > 0)
			{
				lScore += (rand()%(int)(m_pIAndroidUserItem->GetAndroidParameter()->lMaxTakeScore - m_pIAndroidUserItem->GetAndroidParameter()->lMinTakeScore));
				m_pIAndroidUserItem->PerformTakeScore(lScore);
			}

			SCORE lRobotNewScore = pUserItem->GetUserScore();
			strInfo.Format(TEXT("[%s] ִ��ȡ�ȡ��ǰ���(%I64d)��ȡ�����(%I64d)"),pUserItem->GetNickName(),lRobotScore,lRobotNewScore);

			// NcaTextOut(strInfo);
		}
	}
}
//�û�̯��
bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	/*if (wDataSize!=sizeof(CMD_S_Open_Card)) return false;
	CMD_S_Open_Card * pOpenCard=(CMD_S_Open_Card *)pBuffer;*/

	return true;
}

//�û�ǿ��
bool CAndroidUserItemSink::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	/*if (wDataSize!=sizeof(CMD_S_PlayerExit)) return false;
	CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;*/

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	if (wDataSize!=sizeof(STR_CMD_SC_XJ_GAME_END)) return false;
	STR_CMD_SC_XJ_GAME_END * pGameEnd=(STR_CMD_SC_XJ_GAME_END *)pBuffer;
	//ɾ����ʱ��
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_BANKER);
	m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
	m_pIAndroidUserItem->KillGameTimer(IDI_OPEN_CARD);
	//��ʼʱ��
	UINT nElapse=rand()%(3)+7+m_cbUserCount;
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

	UINT nTime = rand()%nElapse + 1;
	m_pIAndroidUserItem->SetGameTimer(IDI_BANKER,nTime);

	//�������
	m_lTurnMaxScore = 0;
	// memset( &m_HandCardData, 0, sizeof m_HandCardData);

	return true;
}

//////////////////////////////////////////////////////////////////////////
