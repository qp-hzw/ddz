#include "StdAfx.h"
#include "TableFrameSink.h"
#include "GameServerManager.h"
#include "SubGameRule.h"


// ���캯��
CTableFrameSink::CTableFrameSink()
{
	m_GameLogic = NULL;
	m_GameAccess = NULL;

	m_pITableFrame=NULL;

	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib = NULL;

	m_pRoomRuleOption = NULL;

	memset(m_dPlayerState, USER_NULL, sizeof(DWORD)*MAX_CHAIR_COUNT);
}

// ��������
CTableFrameSink::~CTableFrameSink()
{
	//У��
	if (NULL  !=  m_GameLogic)
	{
		delete m_GameLogic;
		m_GameLogic  = NULL;
		m_GameAccess = NULL; 
	}

	m_pITableFrame = NULL;

	m_pGameServiceOption = NULL;
	m_pGameServiceAttrib = NULL;

	m_pRoomRuleOption = NULL;
}

// �ӿڲ�ѯ--��������Ϣ�汾
VOID * CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

// ��ʼ��
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL)
	{
		return false;
	}
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);	//������׼����ʼ

	//��Ϸ����
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();

	//��������
	m_pRoomRuleOption= (tagGameRoomItem*)m_pITableFrame->GetCustomRule();

	//У��
	if (NULL != m_GameLogic)
	{
		delete m_GameLogic;
		m_GameLogic  = NULL;
		m_GameAccess = NULL; 
	}
	
	return true;
}

// ��λ����
VOID  CTableFrameSink::RepositionSink()
{
	if ( NULL != m_GameAccess )
	{
		m_GameAccess->SetGameState( GAME_STATUS_FREE ); // �ȴ���ʼ
	}
}

// С����Ϸ����
bool CTableFrameSink::XjGameConclude(int nTotalGameCount, int nCurGameCount)
{
	// �����Ŀ
	BYTE _playersum = m_GameAccess->GetMaxChairCount(); 

	// ��������
	STR_CMD_SC_XJ_GAME_END sXJGameEnd;
	ZeroMemory(&sXJGameEnd, sizeof(STR_CMD_SC_XJ_GAME_END));

	//��õ���
	m_GameAccess->GetLeaveCard(sXJGameEnd.cbLeaveCard,MAX_LEAVE_CARD_NUM);
	
	//�����ҵ�����Ϸ�÷�
	for (BYTE i = 0; i < _playersum && m_GameAccess != NULL; ++i)
	{
		if ( USER_PLAYING == m_GameAccess->GetPlayerState(i) )
		{
			//ֱ�ӽ�����ܻ��ַ����ͻ��ˣ�����ͻ��˼���
			sXJGameEnd.nTotalGameScore[i]  = m_GameAccess->GetPlayerTotalScore(i);
			//��ҵ��ֵ÷�
			sXJGameEnd.nSingleGameScore[i] = m_GameAccess->GetPlayerSingleScore(i);	

			printf("\nӮ�ң�%d�����ֵ÷� = %lld���ܵ÷� = %lld\n",i, sXJGameEnd.nSingleGameScore[i], sXJGameEnd.nTotalGameScore[i]);
		}		
	}

	//��һ����Ϸ������۳�����		TODO ������Ҫ��������ģʽ����Ϸ
	BYTE cbCurGameCount = m_GameAccess->GetCurGameCount();
	m_pITableFrame->HandleXJGameEnd(cbCurGameCount, TABLE_MODE_FANG_KA, sXJGameEnd.nSingleGameScore);		//�۳�����

	//С������д�����ݿ�
	WriteGameRecord();

	//���һ�ֲ�����С����Ϸ�������ͻ��˲���ʾС�ֽ���ҳ��
	if ( cbCurGameCount != m_GameAccess->GetAllCount() )
	{
		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_XJ_GAME_END, &sXJGameEnd, sizeof(sXJGameEnd));
		//m_pITableFrame->SendLookonData(INVALID_CHAIR, CMD_SC_XJ_GAME_END, &sXJGameEnd, sizeof(sXJGameEnd));

		printf("\n������С����Ϸ������\n");

		//������Ϸ״̬ΪС�ֽ���״̬
		m_pITableFrame->SetGameStatus(GS_WK_XJ_GAMEEND);

		//����С����Ϸ�������û�׼�����Ķ�ʱ��
		m_pITableFrame->KillGameTimer(IDI_XJGAME_USER_READY);
		m_pITableFrame->SetGameTimer(IDI_XJGAME_USER_READY, IDI_TIME_XJGAME_USER_READY, 1, 0);

		// ������Ϸ����
		m_GameLogic->ClearXjGame();
	}

	return true;
}

// �����Ϸ����
bool CTableFrameSink::DjGameConclude(int nTotalGameCount, int nCurGameCount)
{
	printf("��ֽ���\n");

	// ��������
	STR_CMD_SC_DJ_GAME_END  sDJGameEnd;
	memset(&sDJGameEnd, 0, sizeof(sDJGameEnd));
	sDJGameEnd.wFangzhuID = m_GameAccess->GetRoomFangzhu();

	//Ӯ����
	int nMaxWinNum = 0;
	
	// �����Ŀ
	BYTE _playersum = m_GameAccess->GetCurPlayerCount(); 
	for (BYTE i = 0; i < _playersum; ++i)
	{
		if ( USER_PLAYING == m_GameAccess->GetPlayerState(i) )
		{
			sDJGameEnd.lUserScore[i] = m_GameAccess->GetPlayerTotalScore(i);
			m_GameAccess->GetWinSum(i, sDJGameEnd.bWinTime[i]);

			//��ô�Ӯ�ң����Ӯ������
			if ( sDJGameEnd.bWinTime[i] > nMaxWinNum )
			{
				nMaxWinNum = sDJGameEnd.bWinTime[i];
				sDJGameEnd.wMaxWinChairID = i;
			}
			printf("\n���ID = %d����ҵ÷� = %d��Ӯ���� = %d\n", i, sDJGameEnd.lUserScore[i], sDJGameEnd.bWinTime[i]);
		}
	}

	// �㲥��ֽ�����Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_DJ_GAME_END, &sDJGameEnd, sizeof(sDJGameEnd));
	//m_pITableFrame->SendLookonData(INVALID_CHAIR,CMD_SC_DJ_GAME_END, &Conclude,sizeof(Conclude));

	//������ϷΪ�����С�״̬
	m_pITableFrame->SetGameStatus(GS_WK_FREE);

	//ֱ�ӽ�����Ϸ����Ҫ����
	m_pITableFrame->ConcludeGame(GAME_CONCLUDE_NORMAL);

	// ����������
	m_GameLogic->ClearDjGame();

	//��ֽ���������ڴ� -- ��� ��Ϸ������־λ GAME_CONCLUDE_CONTINUE, ����Ͳ�������ڴ�
	m_GameLogic->ClearRoom();

	return true;
}

// ��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//�ر�̯�ƶ�ʱ��
	m_pITableFrame->KillGameTimer(IDI_OUT_CARD);

	// ��Ϸ��ǰ����
	BYTE cbCurGameCount = m_GameAccess->GetCurGameCount();

	// �ܾ���
	WORD cbAllGameCount = m_GameAccess->GetAllCount();

	// �����Ŀ
	BYTE cbPlayersum = m_GameAccess->GetMaxChairCount(); 

	switch (cbReason)
	{
	case GER_DISMISS:			// ��ɢ����
		{
			printf("\n��ɢ����\n");

			// ���ͽ�ɢ��Ϣ
			STR_CMD_SC_DJ_GAME_END  sGameDismiss;
			memset(&sGameDismiss, 0, sizeof(sGameDismiss));
			for ( BYTE i = 0; i < cbPlayersum; ++i)
			{
				if ( USER_PLAYING == m_GameAccess->GetPlayerState(i) )
				{
					sGameDismiss.lUserScore[i] = m_GameAccess->GetPlayerTotalScore(i);
					m_GameAccess->GetWinSum( i, sGameDismiss.bWinTime[i] );
				}
			}

			//�㲥����
			m_pITableFrame->SendTableData(INVALID_CHAIR,CMD_SC_DJ_GAME_END, &sGameDismiss, sizeof(sGameDismiss));
		
			m_pITableFrame->ConcludeGame(cbReason);

			// ��������
			m_GameLogic->ClearRoom();

			//�ͷ��ڴ�
			delete m_GameLogic;
			m_GameLogic = NULL;
			m_GameAccess = NULL;

			break;
		}
	case GER_NORMAL:			//��������
		{
			// С�ֽ���
			if (cbCurGameCount < cbAllGameCount) 
			{
				XjGameConclude(cbAllGameCount, cbCurGameCount);
			}
			else if (cbCurGameCount == cbAllGameCount)		// ��ֽ���
			{
				XjGameConclude(cbAllGameCount, cbCurGameCount);
				DjGameConclude(cbAllGameCount, cbCurGameCount);
			}
				
			break;
		}
	default:
		break;
	}
	
	return true;
}

// ��������ʱ���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
		case GS_WK_FREE:		//����״̬
		{
			//�����������Ϊ�գ���ȡ��������
			if ( NULL == m_pRoomRuleOption )
			{
				m_pRoomRuleOption = (tagGameRoomItem*)m_pITableFrame->GetCustomRule();
			}

			//����״̬
			STR_CMD_SC_STATUS_FREE StatusFree;
			ZeroMemory( &StatusFree, sizeof( StatusFree ) );

			// ���ñ���
			//StatusFree.GameCount = m_pRoomRuleOption->GameCount;
			//StatusFree.PlayerCount = m_pRoomRuleOption->PlayerCount;
			//StatusFree.CellScore = m_pRoomRuleOption->CellScore;
			//StatusFree.FangZhu = m_pRoomRuleOption->FangZhu;

			// ���ͳ���
			return m_pITableFrame->SendGameScene( pIServerUserItem, &StatusFree, sizeof(StatusFree));
		}
		case GS_WK_FANZHU:		//����״̬
		{
			STR_CMD_SC_STATUS_FANZHU StatusFanzhu;
			ZeroMemory(&StatusFanzhu, sizeof(STR_CMD_SC_STATUS_FANZHU));

			//��������
			StatusFanzhu.wCurFanzhuUser = m_GameAccess->GetCurFanzhuUser();
			m_GameLogic->AnalyseUserFanZhu(wChairID, StatusFanzhu.cbFanZhuCard);
			StatusFanzhu.cbLastTime = m_pITableFrame->GetTimerLeftTickCount(IDI_FAN_ZHU) / 1000;

			// ���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusFanzhu, sizeof(STR_CMD_SC_STATUS_FANZHU));
		}
		case GS_WK_KOUDI:		//�۵�״̬
		{
			STR_CMD_SC_STATUS_KOUDI StatusKoudi;
			ZeroMemory(&StatusKoudi, sizeof(STR_CMD_SC_STATUS_KOUDI));

			//�۵�����
			StatusKoudi.wCurKouDiUser = m_GameAccess->GetCurKoudiUser();
			m_GameAccess->GetClientHandCards(wChairID, StatusKoudi.cbSortedCard, MAX_CARD_COUNT);
			StatusKoudi.cbHandCardNum = m_GameAccess->GetUserCurCardNum(wChairID);		
			StatusKoudi.cbMainColorNum = m_GameAccess->GetUserMainColorNum(wChairID);
			StatusKoudi.cbMainColor = m_GameAccess->GetMainColor();

			// ���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusKoudi, sizeof(STR_CMD_SC_STATUS_KOUDI));
		}
		case GS_WK_OUT_CARD:	//����״̬
		{
			STR_CMD_SC_STATUS_OUTCARD StatusOutCard;
			ZeroMemory(&StatusOutCard, sizeof(STR_CMD_SC_STATUS_OUTCARD));

			//��������
			StatusOutCard.wCurOutCardUser = m_GameAccess->GetCurOutCardUser();
			int nPlayerNum = m_GameAccess->GetMaxChairCount();
			for (int i = 0; i < nPlayerNum; i++)
			{
				if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
				{
					m_GameAccess->GetUserOutCard(i, StatusOutCard.cbOutCard[i]);
				}
			}

			// ���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusOutCard, sizeof(STR_CMD_SC_STATUS_OUTCARD));
		}
		case GS_WK_XJ_GAMEEND:
		{
			STR_CMD_SC_STATUS_XJ_END StatusXJEnd;
			ZeroMemory(&StatusXJEnd, sizeof(STR_CMD_SC_STATUS_XJ_END));

			//�����ҵ�����Ϸ�÷�
			int nPlayerNum = m_GameAccess->GetMaxChairCount();				
			for (BYTE i = 0; i < nPlayerNum && m_GameAccess != NULL; ++i)
			{
				if ( USER_PLAYING == m_GameAccess->GetPlayerState(i) )
				{
					//��ҵ��ֵ÷�
					StatusXJEnd.nSingleGameScore[i] = m_GameAccess->GetPlayerSingleScore(i);	

					printf("\nӮ�ң�%d�����ֵ÷� = %lld\n",i, StatusXJEnd.nSingleGameScore[i]);
				}		
			}

			return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusXJEnd, sizeof(STR_CMD_SC_STATUS_XJ_END));
		}
	default:break;
	}

	return false;
}

// ��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	//У��
	if ( NULL ==  m_GameAccess)
		return false;

	//���̴���
	switch(wTimerID)
	{
		case IDI_ROB_BANKER: // ��ׯ��ʱ��
		{
			m_pITableFrame->KillGameTimer(IDI_ROB_BANKER);

			//û�н�ׯ,Ĭ�ϲ���
			for (WORD i = 0; i < m_GameAccess->GetMaxChairCount() && m_GameAccess != NULL; i++)
			{
				if ( (USER_PLAYING == m_GameAccess->GetPlayerState(i)) 
					&& (0 == m_GameAccess->GetBankerState(i)) )
				{        
					printf("\n��ʱ������Ĭ�ϲ���ׯ\n");
					OnUserCallBanker(i, 0, 0);
				}
			}
			break;
		}
		case IDI_XJGAME_USER_READY:  // С�ֽ����û�׼����ʱ��
		{
			m_pITableFrame->KillGameTimer(IDI_XJGAME_USER_READY);
			printf("\n��������������ʱ���������롾С�ֿ�ʼ��\n");
			//û��׼������Ĭ��ȫ��׼������ʼ��Ϸ
			for (WORD i = 0; i < m_GameAccess->GetMaxChairCount() && m_GameAccess != NULL; i++)
			{
				if ( USER_PLAYING == m_GameAccess->GetPlayerState(i) 
					&& (0 == m_GameAccess->GetXjReadyState(i)) )
				{
					printf("\n��������������ʱ���������ܡ�С�ֿ�ʼ�� ��ң�%d\n", i);
					//�û�׼��
					OnUserXjGameReady(i, 1);
				}
			}
			break;
		}
		case IDI_ROUND_END_DELAY:	//һ�ֽ����ӳٶ�ʱ��
		{
			m_pITableFrame->KillGameTimer(IDI_ROUND_END_DELAY);

			//�ӳٸ���ҷ���һ�ֽ���
			WORD cbOutCardNum = (WORD)wBindParam;

			//һ�ֽ�����֪ͨӮ�ҳ��ƿ�ʼ
			HandleRoundEnd(cbOutCardNum);
			break;
		}
		default:
			return false;
	}
	
	return true;
}

// ��Ϸ��Ϣ����
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//��Ϣ����
	switch (wSubCmdID)
	{
		case SUB_CS_OUT_CARD:		//����
		{
			//Ч������
			if (wDataSize != sizeof(STR_SUB_CS_OUT_CARD) )
				return true;

			//��������
			STR_SUB_CS_OUT_CARD *pOutCard= (STR_SUB_CS_OUT_CARD *)pData;
			if ( pOutCard->wOutCardUser == pIServerUserItem->GetChairID() )
			{
				bool bPass = (pOutCard->cbPass == 1) ? true : false;
				On_Sub_UserOutCard(pOutCard->wOutCardUser, pOutCard->cbOutCard, pOutCard->cbOutCardNum, bPass);
			}			
			break;	
		}
		case SUB_CS_ROB_BANKER:		 //��ׯ����
		{
			//Ч������
			if (wDataSize != sizeof(CMD_C_CallBanker))
			{
				return true;//������Ϣ Ҳ��ʾ����
			}

			//��������
			STR_SUB_CS_ROB_BANKER *pCallBanker = (STR_SUB_CS_ROB_BANKER *)pData;
			OnUserCallBanker(pIServerUserItem->GetChairID(), pCallBanker->cbType, pCallBanker->cbRobState);

			break;
		}
		case SUB_CS_ADD_SCORE:	 //�û���ע
		{
			//У������
			if (wDataSize != sizeof(STR_SUB_CS_ADD_SCORE))
				return true;

			STR_SUB_CS_ADD_SCORE *pAddScore = (STR_SUB_CS_ADD_SCORE *)pData;

			//����
			OnUserAddScore(pIServerUserItem->GetChairID(), pAddScore->score);

			break;
		}
		case SUB_CS_MING_PAI:	//����
		{
			//У������
			if (wDataSize != sizeof(STR_SUB_CS_MING_PAI))
				return true;

			STR_SUB_CS_MING_PAI *pMingPai = (STR_SUB_CS_MING_PAI *)pData;

			On_Sub_UserMingPai(pIServerUserItem->GetChairID(), pMingPai->cbIsMing, pMingPai->cbType);
		}
		case SUB_CS_XJ_GAME_READY:       //���С����׼��
		{
			//У������
			if (wDataSize != sizeof(STR_SUB_CS_XJ_USER_READY))
				return true;

			STR_SUB_CS_XJ_USER_READY *pXjReady = (STR_SUB_CS_XJ_USER_READY *)pData;

			OnUserXjGameReady(pXjReady->wReadyUser, pXjReady->cbReady);
			break;
		}
		default:
			return false;
	}

	return true;
}

// �û�����
bool CTableFrameSink::OnActionUserSitDown( WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser )
{
	if( bLookonUser == false )	
	{		
		m_dPlayerState[wChairID] = USER_SITDOWN;
	}

	return true;

}

// �û�����
bool CTableFrameSink::OnActionUserStandUp( WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if( bLookonUser == false )
		m_dPlayerState[wChairID] = USER_STANDUP;

	return true;
}

// �û�׼��
bool CTableFrameSink::OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{ 
	printf("\n���û���Ϸ״̬ = %d��\n", pIServerUserItem->GetUserStatus());
	if ( pIServerUserItem->GetUserStatus() == USER_SITDOWN )
	{
		//�ӿ�ܻ�ô����Ϸ�û�״̬
		m_dPlayerState[wChairID] = USER_ONREADY;
	}

	return true; 
}

// �����Ϸ��ʼ				
bool CTableFrameSink::OnEventGameStart()
{
	// ������Ϸ��ָ�븳ֵ -- �˴���m_GameAccess��m_GameLogicΪͬһ������  ��m_GameData����ͬһ������
	m_GameData.CreateGame( &m_GameLogic, &m_GameAccess );

	// ��ʼ������-���÷������
	if ( !m_GameAccess->SetRoomRule( m_pRoomRuleOption, sizeof(tagGameRoomItem)) )
		return false;

	// ���ض�ȡ�������ļ������ʹ�����־,�����齫-����ģʽ		
	if ( 0 != m_GameLogic->LoadConfigParameter(L"MJ_GAMES_LOG.txt", GAME_SCORE_MODE_CLASSIC) )
	{
		//δ�ɹ���ʼ��,�˳�
		return false;
	}

	//�����û�׼��״̬������ܴ������û�״̬��������Ϸ����
	int nReadyNum = 0;
	for (int i = 0; i < m_GameAccess->GetMaxChairCount(); i++)
	{
		if ( USER_ONREADY == m_dPlayerState[i] )
		{
			m_GameAccess->SetPlayerState(i, USER_ONREADY);
			nReadyNum++;
		}
	}
	
	//������С������Ϸ��ʼ
	if ( MIN_CHAIR_COUNT <= nReadyNum )	
	{
		StartGame();
	}
	
	return true;
}

// ��ʼ��Ϸ
void CTableFrameSink::StartGame()
{
	printf("\n����ʼ��Ϸ��\n");

	// �������״̬����Ϸ�С�
	BYTE cbPlayerNum = m_GameAccess->GetMaxChairCount();
	for ( BYTE i = 0; i < cbPlayerNum; ++i)
	{	
		//��ֽ���ʱ���û�״̬Ϊ׼����С����Ϸ����ʱ���û�״̬Ϊ��Ϸ�С�����Ҫ��>=
		if( USER_ONREADY <= m_GameAccess->GetPlayerState(i) )
		{
			m_GameAccess->SetPlayerState( i, USER_PLAYING );
		}
	}

	// ��Ϸ������1
	BYTE cbGameCout = m_GameAccess->GetCurGameCount();
	m_GameAccess->SetGameCount( ++cbGameCout );

	//����ҷ���
	HandleDeal();

	//������ׯ����������
	HandleRobBanker();
}

//����������
void CTableFrameSink::HandleDeal()
{
	// ϴ�Ʋ�����
	m_GameLogic->Deal();

	//�������
	BYTE cbMaxChairCount = m_GameAccess->GetMaxChairCount();
	for (WORD i = 0; i < cbMaxChairCount; i++)
	{
		if ( USER_PLAYING == m_GameAccess->GetPlayerState(i) )
		{
			// ���췢������
			STR_CMD_SC_SEND_CARD SendCard;
			ZeroMemory(&SendCard, sizeof(STR_CMD_SC_SEND_CARD));

			//��ֵ	
			SendCard.wChairID = i;	
			int _cardsum = m_GameAccess->GetPlayerInitCardNum();
			m_GameAccess->GetClientHandCards(i, SendCard.cbHandCardData, _cardsum);
			SendCard.cbHandCardNum = m_GameAccess->GetUserCurCardNum(i);

			// ���Ƹ���Ӧ�ͻ��� -- ÿ���ͻ���Ӧ��ֻ���յ��Լ�����
			m_pITableFrame->SendTableData(i, CMD_SC_SEND_CARD, &SendCard, sizeof(STR_CMD_SC_SEND_CARD));
		}
	}
}

//������ׯ
void CTableFrameSink::HandleRobBanker()
{
	// ������Ϸ������״̬��
	m_pITableFrame->SetGameStatus(GS_WK_ROB);

	//���ݷ�������ж���ׯģʽ
	BYTE cbRobBankMode = m_GameAccess->GetRobBankMode();

	//1���̶�ׯ����ׯ��ָ��ׯ�ң�ֱ�ӷ��͡���ע��ʼ��	
	if ( (ROOMRULE_OPTION_ROBTYPE_FIXED == cbRobBankMode) ||
		 (ROOMRULE_OPTION_ROBTYPE_TURN == cbRobBankMode) )
	{
		//�̶�ׯ����ׯ��������
		HandleTurnAndFixRobBanker();
	}

	//2��������ׯ�����͡���ׯ��ʼ��-������ׯ����Ϣ
	if ( ROOMRULE_OPTION_ROBTYPE_FREE == cbRobBankMode )
	{
		//������ׯ��Ϣ��������
		HandleFreeRobBanker();
	}
}

//����̶�ׯ����ׯ
void CTableFrameSink::HandleTurnAndFixRobBanker()
{
	//����ȫ���û���ׯ���
	BYTE cbPlayerNum = m_GameAccess->GetMaxChairCount();
	for (BYTE i = 0; i < cbPlayerNum; ++i)
	{
		if( USER_PLAYING == m_GameAccess->GetPlayerState(i) )
		{
			m_GameAccess->SetBankerState(i, 1);
		}
	}
		
	//ָ��ׯ��
	WORD wBanker = INVALID_CHAIR;
	bool bAppointResult = m_GameLogic->AppointBanker(wBanker);

	//У�� ���ﲻ�˳���Ĭ�Ϸ���Ϊׯ��
	if (INVALID_CHAIR == wBanker || !bAppointResult)
	{
		wBanker = m_GameAccess->GetRoomFangzhu();
	}

	////��������
	//STR_CMD_SC_BET_START sAddScoreStart;
	//ZeroMemory(&sAddScoreStart,sizeof(sAddScoreStart));
	////��ֵ
	//sAddScoreStart.wBanker = wBanker;
	//sAddScoreStart.dwBetCellScore = ROOMRULE_MIN_CELL_SCORE;

	//printf("\n���̶�ׯ����ׯ��ID = %d���׷� = %d\n", sAddScoreStart.wBanker, sAddScoreStart.dwBetCellScore);

	////�㲥����ע��ʼ��
	//m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_BET_START, &sAddScoreStart, sizeof(STR_CMD_SC_BET_START));

	////������Ϸ״̬Ϊ����ע״̬��
	//m_pITableFrame->SetGameStatus(GS_WK_ADDSCORE);


	//// ��ע��ʱ��
	//m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);
	//m_pITableFrame->SetGameTimer(IDI_ADD_SCORE, IDI_TIME_ADD_SCORE, 1, 0);
}

//����������ׯ
void CTableFrameSink::HandleFreeRobBanker()
{
	//��һ�֣��ӷ�����ʼ������ׯ��ʼ��Ϣ	�ڶ��֣����Ͼֵ�Ӯ�ҿ�ʼ������ׯ��Ϣ
	WORD wChairID = m_GameAccess->GetRoomFangzhu();
	if (1 != m_GameAccess->GetCurGameCount() )
	{
		wChairID = m_GameAccess->GetLastGameWinner();
	}
	m_GameAccess->SetStartRobUser(wChairID);
	
	//������ׯ(�е���)��ʼ��Ϣ
	SendRobStart(wChairID, ROB_TYPE_CALL);
}

//������ׯ��ʼ��Ϣ
void CTableFrameSink::SendRobStart(const WORD &wChairID, const BYTE &cbType)
{
	//֪ͨ��һ�������ׯ��ʼ
	STR_CMD_SC_ROB_BANKER_START RobStart;
	ZeroMemory(&RobStart, sizeof(STR_CMD_SC_ROB_BANKER_START));

	//��ֵ
	RobStart.cbType = cbType;

	if ( INVALID_CHAIR != wChairID)
	{
		m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_ROB_START, &RobStart, sizeof(STR_CMD_SC_ROB_BANKER_START));

		//������Ϸ״̬Ϊ����ׯ״̬��
		m_pITableFrame->SetGameStatus(GS_WK_ROB);

		//��ׯ��ʱ��
		m_pITableFrame->KillGameTimer(IDI_ROB_BANKER);
		m_pITableFrame->SetGameTimer(IDI_ROB_BANKER, IDI_TIME_ROB_BANKER, 1, wChairID);
	}
}

// ����ͻ��˷����ġ���ׯ����Ϣ
void CTableFrameSink::OnUserCallBanker(WORD wChairID, BYTE cbType, BYTE cbRobState)
{
	printf("\n�����������������û� = %d, ��ׯ���� = %d, ��ׯ = %d\n", wChairID, cbType, cbRobState);

	//��Ϸ״̬Ч��
	if ( GS_WK_ROB != m_pITableFrame->GetGameStatus() )
		return;

	//������ѡ�������ֱ���˳�������Ҫ���жϸ����
	BYTE cbOldState = m_GameAccess->GetBankerState( wChairID);
	if ( ROB_STATE_PASS == cbOldState )
		return;

	//������ׯ״̬
	m_GameAccess->SetBankerState(wChairID, cbRobState);

	//���ͬ��������������ׯ����
	BYTE cbRobNum = m_GameAccess->GetUserRobNum(wChairID);
	if ( ROB_STATE_AGREE == cbRobState )
	{		
		m_GameAccess->SetUserRobNum(wChairID, ++cbRobNum);
	}
	
	//������ׯ�������
	STR_CMD_SC_ROB_RESULT sRobResult;
	ZeroMemory(&sRobResult, sizeof(STR_CMD_SC_ROB_RESULT));

	//��ֵ
	sRobResult.wChairID  = wChairID;
	sRobResult.cbType = cbType;
	sRobResult.cbRobState = cbRobState;

	//�㲥��ׯ���
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_ROB_RESULT, &sRobResult, sizeof(STR_CMD_SC_ROB_RESULT));

	//��������ζ���ׯ������Ϊׯ��
	if (2 == cbRobNum)
	{
		AllRobBankerOver();
	}
	else	//��ׯ��δ��ɣ���������¸���ҷ�����ׯ��Ϣ
	{
		//�ж��ǲ���������Ҷ�ѡ����ׯ
		BYTE cbMaxChairCount = m_GameAccess->GetMaxChairCount();
		BYTE cbPassNum = 0;
		for (int i = 0; i < cbMaxChairCount; i++)
		{
			if ( (USER_PLAYING == m_GameAccess->GetPlayerState(i)) &&
				 (ROB_STATE_PASS == m_GameAccess->GetBankerState(i)))
			{
				cbPassNum++;
			}
		}

		//�����˷�����ׯ�������·���
		if ( cbPassNum == cbMaxChairCount )
		{
			//�������
			m_GameLogic->ClearXjGame();

			//����ҷ���
			HandleDeal();

			//������ׯ����������
			HandleRobBanker();
		}
		else	//���¸���ҷ�����ׯ��Ϣ
		{
			//����һ����ҷ�����ׯ��ʼ��Ϣ
			WORD wNextUser = m_GameAccess->GetNextUser(wChairID);

			//�ж��¸���ҵ���ׯ״̬
			while ( wNextUser != wChairID )		//��Ҳ��ǡ�����״̬�Ÿ���������������Ϣ
			{
				if ( ROB_STATE_PASS != m_GameAccess->GetBankerState(wNextUser) )
				{
					SendRobStart(wNextUser, ROB_TYPE_ROB);
					break;
				}				
				wNextUser = m_GameAccess->GetNextUser(wNextUser);				
			}
			
		}
	}
}

// ȫ����ׯ���, ������ע״̬
void CTableFrameSink::AllRobBankerOver()
{
	//������Ϸ״̬Ϊ����ע״̬��
	m_pITableFrame->SetGameStatus(GS_WK_ADD_SCORE);

	//�ر���ׯ��ʱ��
	m_pITableFrame->KillGameTimer(IDI_ROB_BANKER);

	//ָ��ׯ��
	WORD wBanker = INVALID_CHAIR;
	bool bAppointResult = m_GameLogic->AppointBanker(wBanker);

	//У�� ���ﲻ�˳���Ĭ�Ϸ���Ϊׯ��
	if (INVALID_CHAIR == wBanker || !bAppointResult)
	{
		wBanker = m_GameAccess->GetRoomFangzhu();
	}

	//����ָ��ׯ����Ϣ
	SendAppointBanker(wBanker);

	//���͵��Ƹ�ׯ��
	HandleSendLeaveCard(wBanker);

	//���ͼ�ע��ʼ���ӱ�����Ϣ
	SendAddScoreStart(INVALID_CHAIR);
}

//�����͵���
void CTableFrameSink::HandleSendLeaveCard(const WORD &wSendCardUser)
{
	printf("\n������� = %d���͵��ơ�\n", wSendCardUser);

	//�����˶����ܷ������������һ�����̣���ׯ�ҷ��͵���
	m_GameLogic->SendLeaveCardToUser(wSendCardUser);

	//��������ҷ���ׯ���õ����Ƶ���Ϣ
	BYTE cbMaxChairCount = m_GameAccess->GetMaxChairCount();
	for (WORD i = 0; i < cbMaxChairCount; i++)
	{
		if ( (USER_PLAYING == m_GameAccess->GetPlayerState(i)) )
		{
			//���͵�����Ϣ
			STR_CMD_SC_SEND_LEAVE_CARD SendCard;
			ZeroMemory(&SendCard, sizeof(STR_CMD_SC_SEND_LEAVE_CARD));

			//��ֵ
			SendCard.wSendCardUser = i;	
			SendCard.cbMagicCard = m_GameAccess->GetMagicCard();

			//���������
			BYTE cbCardNum = m_GameAccess->GetUserCurCardNum(i);

			//��������
			SendCard.cbHandCardNum = m_GameAccess->GetUserCurCardNum(i);
			m_GameLogic->SortCardList(i, SendCard.cbHandCardNum, ST_ORDER);
			m_GameAccess->GetClientHandCards(i, SendCard.cbSortedCard, MAX_CARD_COUNT);	
			
			//����ɫ����
			SendCard.cbMagicCardNum = m_GameAccess->GetMagicCardNum(i);		
			printf("\n���������ƣ���� = %d����Ӹ��� = %d��\n", i, SendCard.cbMagicCardNum);

			// ��������
			m_pITableFrame->SendTableData(i, CMD_SC_USER_SEND_LEAVE_CARD, &SendCard, sizeof(STR_CMD_SC_SEND_LEAVE_CARD));
		}
	}
}

//������ע��ʼ��Ϣ
void CTableFrameSink::SendAddScoreStart(const WORD &wChairID)
{
	//ָ��ׯ�Һ󣬷�����ע��ʼ��Ϣ
	STR_CMD_SC_ADD_SCORE_START sAddScoreStart;
	ZeroMemory(&sAddScoreStart, sizeof(STR_CMD_SC_ADD_SCORE_START));

	//��ֵ		
	sAddScoreStart.cbStart = 1;

	printf("\n�������������㲥����ע��ʼ��\n");

	// �㲥��Ϣ �û���ע��ʼ
	m_pITableFrame->SendTableData(wChairID, CMD_SC_ADD_SCORE_START, &sAddScoreStart, sizeof(STR_CMD_SC_ADD_SCORE_START));
		
	m_pITableFrame->SetGameStatus(GS_WK_ADDSCORE);

	// ��ע��ʱ��
	m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);
	m_pITableFrame->SetGameTimer(IDI_ADD_SCORE, IDI_TIME_ADD_SCORE, 1, 0);
}

// ��ע��ʼ����
void CTableFrameSink::OnUserAddScore( WORD wChairID, SCORE lScore )
{
	printf("\n���������������ա���ע������� = %d����ע = %ld\n", wChairID, lScore);

	//��Ϸ״̬Ч��
	if ( GS_WK_ADDSCORE != m_pITableFrame->GetGameStatus() )
		return;

	//�û�״̬У��
	if( (USER_PLAYING != m_GameAccess->GetPlayerState(wChairID)) &&
		(1 != m_GameAccess->GetAddScoreState(wChairID)) )
		return;

	//�ɶ�μ�ע, ��˴˴��������ע��ɱ��
	m_GameLogic->Wager(wChairID, lScore);

	// ���������ע״̬
	m_GameAccess->SetAddScoreState(wChairID, 1);

	//��������
	STR_CMD_SC_ADD_SCORE_RESULT sAddScoreResult;
	ZeroMemory(&sAddScoreResult, sizeof(STR_CMD_SC_ADD_SCORE_RESULT));
	//��ֵ
	sAddScoreResult.wChairID	 = wChairID;
	sAddScoreResult.wAddSocre	 = lScore;

	printf("\n���������������͡���ע���������� = %d����ע = %d\n",sAddScoreResult.wChairID, sAddScoreResult.wAddSocre);

	//�㲥��ע���
	m_pITableFrame->SendTableData( INVALID_CHAIR, CMD_SC_ADD_SCORE_RESULT, &sAddScoreResult, sizeof(STR_CMD_SC_ADD_SCORE_RESULT) );

	// �����ע��������
	BYTE cbBetedNum = 0;
	BYTE cbPlayerNum = m_GameAccess->GetMaxChairCount();// ��ȡ�����
	for (BYTE i = 0; i < cbPlayerNum; ++i)
	{
		if ( 1 == m_GameAccess->GetAddScoreState(i) )
			cbBetedNum++;
	}

	// ��ע��ɣ�����
	if ( cbBetedNum == m_GameAccess->GetCurPlayerCount() )
	{
		printf("\n��������ע��ɣ���ע���� = %d\n", cbPlayerNum);

		//֪ͨׯ���Ƿ���Ҫ����
		WORD wBanker = m_GameAccess->GetBankerID();
		SendMingPaiStart(wBanker, MING_PAI_TYPE_OUTCARD);
	}

	return;
}

//�������ƿ�ʼ��Ϣ
void CTableFrameSink::SendMingPaiStart(const WORD &wChairID, const BYTE &cbMPType)
{
	//ָ��ׯ�Һ󣬷�����ע��ʼ��Ϣ
	STR_CMD_SC_MING_PAI_START MingPaiStart;
	ZeroMemory(&MingPaiStart, sizeof(STR_CMD_SC_MING_PAI_START));

	//��ֵ		
	MingPaiStart.wChairID = wChairID;
	MingPaiStart.cbType = cbMPType;

	printf("\n������������֪ͨ�û� = %d�����ƿ�ʼ��\n", wChairID);

	// ֪ͨ�û����ƿ�ʼ
	m_pITableFrame->SendTableData(wChairID, CMD_SC_MING_PAI_START, &MingPaiStart, sizeof(STR_CMD_SC_MING_PAI_START));

	// ���ƶ�ʱ��
	m_pITableFrame->KillGameTimer(IDI_MING_PAI);
	m_pITableFrame->SetGameTimer(IDI_MING_PAI, IDI_TIME_MING_PAI, 1, 0);
}

//�����¼�
void CTableFrameSink::On_Sub_UserMingPai(WORD wChairID, const BYTE &cbMPType, BYTE cbFlag)
{
	//�������
	if ( cbFlag == 1 )			
	{
		//������������
		STR_CMD_SC_MING_PAI_RESULT MPResult;
		ZeroMemory(&MPResult, sizeof(STR_CMD_SC_MING_PAI_RESULT));

		//���Ƽӱ�
		DWORD dwBet = m_GameAccess->GetAllBet(wChairID);
		if ( MING_PAI_TYPE_GAMESTART == cbMPType )
		{			
			//�ӱ�
			MPResult.cbTimes = 4;
			m_GameLogic->Wager(wChairID, 3*dwBet);
		}
		else if ( MING_PAI_TYPE_OUTCARD == cbMPType )
		{
			MPResult.cbTimes = 2;
			m_GameLogic->Wager(wChairID, dwBet);
		}

		//��ֵ
		MPResult.wChairID = wChairID;
		MPResult.cbIsMing = cbFlag;
		m_GameAccess->GetClientHandCards(wChairID, MPResult.cbHandCard, MAX_CARD_COUNT);		

		//�㲥���������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_USER_OUT_CARD_RESULT, &MPResult, sizeof(STR_CMD_SC_MING_PAI_RESULT));
	}

	//���ͳ��ƿ�ʼ
	WORD wBanker = m_GameAccess->GetBankerID();
	HandleOutCardStart(wBanker);
}

//������ƿ�ʼ��Ϣ
void CTableFrameSink::HandleOutCardStart(const WORD &wOutCardUser)
{
	//���õ�ǰ�������
	m_GameAccess->SetCurOutCardUser(wOutCardUser);

	//�㲥���ƿ�ʼ����Ϣ
	STR_CMD_SC_OUT_CARD_START OutCardStart;
	ZeroMemory(&OutCardStart, sizeof(STR_CMD_SC_OUT_CARD_START));

	OutCardStart.wOutCardUser = wOutCardUser;

	//�㲥���������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_USER_OUT_CARD_START, &OutCardStart, sizeof(STR_CMD_SC_OUT_CARD_START));

	printf("\n��֪ͨ��� = %d���ƿ�ʼ��\n", OutCardStart);

	//���ó�����Ϸ״̬
	m_pITableFrame->SetGameStatus(GS_WK_OUT_CARD);

	// ���ƶ�ʱ��
	m_pITableFrame->KillGameTimer(IDI_OUT_CARD);
	m_pITableFrame->SetGameTimer(IDI_OUT_CARD, IDI_TIME_OUT_CARD, 1, 0);	
}

//�����¼�
void CTableFrameSink::On_Sub_UserOutCard(WORD wOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum, const bool &bPass)
{
	//��Ϸ״̬Ч��
	if ( GS_WK_OUT_CARD != m_pITableFrame->GetGameStatus() ) 
		return;

	//����У��
	if ((wOutCardUser != m_GameAccess->GetCurOutCardUser())||
		(MAX_CARD_COUNT < cbOutCardNum))
	{
		return;
	}

	// ɾ�����ƶ�ʱ��
	m_pITableFrame->KillGameTimer(IDI_OUT_CARD);

	//��ҹ�
	if (bPass)
	{
		//���������
		HandleOutCardPass(wOutCardUser);
	}
	else
	{
		//�����������
		HandleOutCard(wOutCardUser, cbOutCard, cbOutCardNum);
	}
}

//�����û����ƹ�
void CTableFrameSink::HandleOutCardPass(WORD wOutCardUser)
{
	//��ø���ҵ��¼�
	WORD wNextUser = m_GameAccess->GetNextUser(wOutCardUser);

	//һ�ֿ�ʼ�������
	WORD wStartUser = m_GameAccess->GetStartOutCardUser();

	//У���ǲ����ֵ���ʼ�����
	if ( wNextUser == wStartUser )
	{
		//�ǣ���һ�ֽ���
		HandleRoundEnd(wNextUser);
	}
	else
	{
		//��֪ͨ�¸���ҳ��ƿ�ʼ
		HandleOutCardStart(wNextUser);
	}
}

//�����û�����
void CTableFrameSink::HandleOutCard(WORD wOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum)
{
	//�������һ�������ȿ�ʼ������
	WORD wStartUser = m_GameAccess->GetStartOutCardUser();
	//����
	BYTE cbCardType= m_GameLogic->GetCardLogicType(cbOutCard, cbOutCardNum);

	//��һ���������
	if ( (TURN_END == m_GameAccess->GetOneTurnEnd()) &&
		 (wOutCardUser == wStartUser) )
	{
		//����У��
		if (CT_ERROR == cbCardType)		
		{
			//���ͳ��ƽ��
			SendOutCardResult(wOutCardUser, cbOutCard, cbOutCardNum, cbCardType, OUT_CARD_FAIL, OUT_CARD_NORMAL);

			printf("\n����� = %d����ʧ�ܣ����Ͳ��ԡ�\n", wOutCardUser);
			return;
		}
		else
		{
			m_GameAccess->SetOneTurnEnd(TURN_NOT_END);
		}
	}
	else	//����������
	{
		//У�������Ŀ�Ƿ���ʼ���������ͬ������У��
		if ( (cbOutCardNum != m_GameAccess->GetUserOutCard(wStartUser, NULL)) || 
			 !m_GameLogic->EfficacyOutCard(wOutCardUser, cbOutCard, cbOutCardNum) )			
		{
			//���ͳ��ƽ��
			SendOutCardResult(wOutCardUser, cbOutCard, cbOutCardNum, cbCardType, OUT_CARD_FAIL, OUT_CARD_NORMAL);

			printf("\n����� = %d����ʧ�ܣ����Ͳ��ԡ�\n", wOutCardUser);

			return;
		}
	}

	//��������ɾ������˿�
	if ( !m_GameLogic->RemoveCard(wOutCardUser, cbOutCard, cbOutCardNum) )
		return;

	//���Ƽ�¼
	m_GameAccess->SetUserOutCard(wOutCardUser, cbOutCard, cbOutCardNum);

	//���ֳ���������ݼ�¼
	m_GameAccess->SetTurnMaxCards(cbOutCard, cbOutCardNum);

	//������һ���������
	WORD wNextUser = m_GameAccess->GetNextUser(wOutCardUser);

	//�������ը����Ϣ
	if ( (cbCardType&(1<<CT_BOMB_CARD))!= 0 ||
		(cbCardType&(1<<CT_MISSILE_CARD))!= 0||
		(cbCardType&(1<<CT_LAIZI_BOMB))!= 0 || 
		(cbCardType&(1<<CT_RUAN_BOMB))!= 0 )
	{
		m_GameAccess->SetUserBoomInfo(wOutCardUser, cbCardType);
	}

	//���ͳ��ƽ��
	SendOutCardResult(wOutCardUser, cbOutCard, cbOutCardNum, cbCardType, OUT_CARD_SUCCESS, OUT_CARD_NORMAL);

	//У���ǲ���һ�ֳ������
	if ( wNextUser == m_GameAccess->GetStartOutCardUser() )
	{
		//һ�ֽ���
		m_GameAccess->SetCurOutCardUser(INVALID_CHAIR);
		HandleRoundEnd(wOutCardUser);
	}
	else
	{
		//֪ͨ�¸���ҳ��ƿ�ʼ
		m_GameAccess->SetCurOutCardUser(wNextUser);		
		HandleOutCardStart(wNextUser);
	}
}

//�ж�����Ƿ��ܳ���
bool CTableFrameSink::JudgeUserOutCard(const WORD &wLastOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum)
{
	bool bCanOut = false;

	//�ж���������Ƿ��ܳ���
	WORD wNextUser = m_GameAccess->GetNextUser(wLastOutCardUser);

	//ѭ�������������,ֱ�������
	while ( wNextUser != wLastOutCardUser )
	{
		//����ܳ���
		tagOutCardNotify OutCardNotify[MAX_WEAVE_NUM];
		ZeroMemory(&OutCardNotify, MAX_WEAVE_NUM*sizeof(tagOutCardNotify));

		if ( m_GameLogic->AnalyseOutCardNotify(wNextUser, cbOutCard, cbOutCardNum, OutCardNotify) )
		{
			printf("\n����� = %dҪ���𣬷��ͳ��ƿ�ʼ��\n", wNextUser);

			//֪ͨ����ҳ��ƿ�ʼ
			m_GameAccess->SetCurOutCardUser(wNextUser);
			HandleOutCardStart(wNextUser);
			bCanOut = true;
			break;
		}
		else	//���ͳ��ƽ��
		{
			printf("\n����� = %dҪ���𣬷��ͳ��ƽ����\n", wNextUser);
			SendOutCardResult(wNextUser, cbOutCard, cbOutCardNum, 0, OUT_CARD_FAIL, OUT_CARD_PASS);
		}

		wNextUser = m_GameAccess->GetNextUser(wNextUser);
	}
	
	return bCanOut;
}

//���ͳ��ƽ��
void CTableFrameSink::SendOutCardResult(WORD wOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum, 
										 const BYTE &cbCardType, const BYTE &cbSuccess, const BYTE &cbFlag)
{
	//�����������
	STR_CMD_SC_OUT_CARD_RESULT OutCard;
	ZeroMemory(&OutCard, sizeof(STR_CMD_SC_OUT_CARD_RESULT));

	//��ֵ
	OutCard.wOutCardUser = wOutCardUser;
	OutCard.cbFlag = cbFlag;
	OutCard.cbSuccess = cbSuccess;

	//��������
	if ( 1 == cbFlag )
	{
		if (0 == cbSuccess)
		{
			m_pITableFrame->SendTableData(wOutCardUser, CMD_SC_USER_OUT_CARD_RESULT, &OutCard, sizeof(STR_CMD_SC_OUT_CARD_RESULT));
		}
		else
		{
			//��ֵ					
			OutCard.cbOutCardNum = cbOutCardNum;
			memcpy_s(OutCard.cbOutCard, sizeof(BYTE)*cbOutCardNum, cbOutCard, sizeof(BYTE)*cbOutCardNum);
			OutCard.cbHandCardNum = m_GameAccess->GetUserCurCardNum(wOutCardUser);
			
			//����
			OutCard.cbCardType = cbCardType;

			//�÷�
			BYTE cbPlayerNum = m_GameAccess->GetMaxChairCount();
			for (BYTE i = 0; i < cbPlayerNum; ++i)
			{
				if ( USER_PLAYING == m_GameAccess->GetPlayerState(i) )					
				{
					OutCard.lSingleGameScore[i] = m_GameAccess->GetPlayerSingleScore(i);
					printf("\n����� = %d�ĵ��ֵ÷� = %d��\n", i, OutCard.lSingleGameScore[i]);
				}
			}
		}	
	}
	else	//���Ҫ����
	{
		OutCard.cbFlag = cbFlag;
	}

	//�㲥���ƽ��
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_USER_OUT_CARD_RESULT, &OutCard, sizeof(STR_CMD_SC_OUT_CARD_RESULT));
	printf("\n���㲥��� = %d�ĳ��ƽ����\n", OutCard.wOutCardUser);	
}

//����һ�ֳ��ƽ���
void CTableFrameSink::HandleRoundEnd(const WORD &wWinner)
{		
	//����һ�ֵ÷�
	WORD wScore = m_GameLogic->SettleOneRound(wWinner);

	//����һ�ֽ���
	m_GameAccess->SetOneTurnEnd(TURN_END);

	//���ÿ�ʼ�ĳ�����ң���ո��ֵĳ�������
	if ( 0 != m_GameAccess->GetUserCurCardNum(wWinner) )
	{
		//���ø����ʼ�����û�
		m_GameAccess->SetStartOutCardUser(wWinner);

		//���õ�ǰ�����û�
		m_GameAccess->SetCurOutCardUser(wWinner);

		//֪ͨӮ�ҳ���
		HandleOutCardStart(wWinner);

		printf("\n����һ����ƣ�һ�ֽ�����֪ͨӮ�� = %d���ƿ�ʼ��\n", wWinner);
	}
	else	//��������Ƴ���
	{
		printf("\n����Ϸ������\n");

		//����Ӯ��
		m_GameAccess->SetLastGameWinner(wWinner);

		m_GameAccess->SetCurOutCardUser(INVALID_CHAIR);

		//���
		m_GameLogic->Settle();

		//һ�ֽ�������ǰӮ����ȫ�����꣬��Ϸ����
		OnEventGameConclude(INVALID_CHAIR, NULL, GER_NORMAL);
	}
}

//����ָ��ׯ����Ϣ
void CTableFrameSink::SendAppointBanker(const WORD &wBanker)
{
	//�û�У��
	if ( wBanker >= m_GameAccess->GetMaxChairCount() )
		return;

	//��������
	STR_CMD_SC_APPOINT_BANKER AppointBanker;
	ZeroMemory(&AppointBanker, sizeof(STR_CMD_SC_APPOINT_BANKER));

	//��ֵ
	AppointBanker.wBanker = wBanker;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_APPOINT_BANKER, &AppointBanker, sizeof(STR_CMD_SC_APPOINT_BANKER));
}

//С�ֽ���-�û�׼���¼�
void CTableFrameSink::OnUserXjGameReady(WORD wChairID, BYTE ready)
{
	printf("\n���������������ܡ�С�ֿ�ʼ��\n");
	//��Ϸ״̬Ч��
	if ( GS_WK_XJ_GAMEEND != m_pITableFrame->GetGameStatus() )
	{
		printf("\n������Ϸ״̬ = %d\n");
		return;
	}

	//�û�״̬У��
	if ( USER_PLAYING != m_GameAccess->GetPlayerState(wChairID) || 
		(1 == m_GameAccess->GetXjReadyState(wChairID)) )
	{
		printf("\n�������״̬ = %d\n");
		return;
	}

	// ������Ϸ׼��״̬
	m_GameAccess->SetXjReadyState(wChairID, ready);

	// ����׼������
	BYTE cbPlayerNum = m_GameAccess->GetMaxChairCount();
	BYTE cbReadyNum = 0;

	for (int i = 0; i < cbPlayerNum; ++i)
	{
		if ( 1 == m_GameAccess->GetXjReadyState(i) )
		{
			cbReadyNum++;
		}
	}

	if ( cbReadyNum == m_GameAccess->GetCurPlayerCount() )
	{
		//ɾ��С�ֽ�����ʱ��
		m_pITableFrame->KillGameTimer(IDI_XJGAME_USER_READY);
		
		// ȫ��׼����ɣ���ʼ��Ϸ
		StartGame();
	}
}

/*************************************************
@Description:     ��ʼ��¼�����飬��ø����ݵ���������С
@Input:           ��
@Output:          ��
@Return:          ��
@author & data:	lizhihu 2017.11.2
*************************************************/
void CTableFrameSink::StartRecord()
{
}

/*************************************************
@Description:     д��¼�����ݣ����˶�ʱ��
				 ,��С�ֽ�������ã�������Щ����Ϊ��
@Input:           ��
@Output:          ��
@Return:          ��
@author & data:	lizhihu 2017.11.2
*************************************************/
void CTableFrameSink::WriteGameRecord()
{
	//�����ҵ�����Ϸ�÷�
	int _playersum = m_GameAccess->GetMaxChairCount();
	LONG *lGameScore = new LONG [_playersum];
	for (BYTE i = 0; i < _playersum; ++i)
	{
		if ( USER_PLAYING == m_GameAccess->GetPlayerState(i) )
		{
			lGameScore[i] = m_GameAccess->GetPlayerSingleScore(i);
		}	
	}
	
	//���쵥�ֵ÷��ַ���
	CString str;
	str.Format(TEXT("%d"),lGameScore[0]);
	for(int i=1; i < _playersum; ++i)
	{
		TCHAR szTemp[10];
		_sntprintf(szTemp,10,TEXT(",%d"),lGameScore[i]);
		str.Append(szTemp);
	}

	//�ͷ�
	delete [] lGameScore;
	lGameScore = NULL;

	//���ÿ�ܷ���		//��ʱֻдС�ֵ÷�
	m_pITableFrame->WriteRecordInfo(m_GameAccess->GetCurGameCount(), str.GetBuffer(str.GetLength()), NULL, 0);
}

/*************************************************
@Description:     ����¼���ͷ��ڴ�
@Input:           ��
@Output:          ��
@Return:          ��
@author & data:	lizhihu 2017.11.2
*************************************************/
void CTableFrameSink::CloseRecord()
{
	delete [] m_GameRec.pRecData;
	m_GameRec.pRecData = NULL;
}
