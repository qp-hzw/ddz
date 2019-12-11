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
	//m_pITableFrame->SetStartMode(START_MODE_FULL_READY);	//������׼����ʼ

	////��Ϸ����
	//m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	//m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();

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
		m_GameAccess->SetGameStatus( GAME_STATUS_FREE ); // �ȴ���ʼ
	}
}

// С����Ϸ����
bool CTableFrameSink::XjGameConclude(int nTotalGameCount, int nCurGameCount)
{
	// �����Ŀ
	BYTE _playersum = m_GameAccess->GetMaxChairCount();

	// ������ҷ����ڵ��ܵ÷�
	for (BYTE i = 0; i < _playersum && m_GameAccess != NULL; ++i)
	{
		if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
		{
			m_GameAccess->SetPlayerTotalScore(i, m_GameAccess->GetPlayerSingleScore(i));
		}
	}

	//��¼������е�ǰС�ֵĵ÷��ǲ������
	for (BYTE i = 0; i < _playersum && m_GameAccess != NULL; ++i)
	{
		SCORE CurBestBet = m_GameAccess->GetAllBet(i);
		if (m_GameAccess->GetDJPlayerBestBet(i) < CurBestBet)      //��һ�ֵķֱ���һ��С ���滻
		{
			m_GameAccess->SetDJPlayerBestBet(i, CurBestBet);
		}
	}


	// ��������
	SCORE *msg_totalGameScore = new SCORE[_playersum]();
	SCORE *msg_singleGameScore = new SCORE[_playersum]();
	WORD  Winner = m_GameAccess->GetLastGameWinner();   //��ȡӮ��
	WORD  BankID = m_GameLogic->GetAppointBanker();     //��ȡׯ�� 

	//�����ҵ�����Ϸ�÷�
	for (BYTE i = 0; i < _playersum && m_GameAccess != NULL; ++i)
	{
		if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
		{
			//ֱ�ӽ�����ܻ��ַ����ͻ��ˣ�����ͻ��˼���
			msg_totalGameScore[i] = m_GameAccess->GetPlayerTotalScore(i);
			//��ҵ��ֵ÷�
			msg_singleGameScore[i] = m_GameAccess->GetPlayerSingleScore(i);

			//����ǽ�ҳ����ܷ����ó� ��� + �ܷ�
			if (m_pITableFrame->GameType() == 1)
			{
				BASE_PLAYERINFO info = m_pITableFrame->GetPlayerBaseInfo(i);
				msg_totalGameScore[i] += info.m_goldCoin;
				//����Ϊ0
				if (msg_totalGameScore[i] < 0)
				{
					msg_totalGameScore[i] = 0;
				}
			}
		}
	}

	//��ȡ����ũ��ļӱ�������
	DWORD Addbet = 0;
	for (int i = 0; i < _playersum; i++)
	{
		if (i != BankID)
		{
			Addbet += m_GameAccess->GetPlayerAddScore(i);
		}
	}

	//�жϵ�����ũ���Ƿ��������
	if (m_pITableFrame->GameType() == 1)
	{
		SCORE BankScore = 0;
		if (Winner == BankID)		//ׯӮ��
		{
			for (int i = 0; i < _playersum; i++)
			{
				if (i != BankID)
				{
					BASE_PLAYERINFO info = m_pITableFrame->GetPlayerBaseInfo(i);
					if ((int)info.m_goldCoin < (int)(-msg_singleGameScore[i]))
					{
						msg_singleGameScore[i] = (int)(-info.m_goldCoin);
						BankScore += info.m_goldCoin;
					}
					else
					{
						BankScore -= msg_singleGameScore[i];
					}
				}
			}
			//���Ӯ�ý�ҳ����˵���������
			if (BankScore > m_pITableFrame->GetPlayerBaseInfo(BankID).m_goldCoin)
			{
				for (int i = 0; i < _playersum; i++)
				{
					if (i != BankID)
					{
						msg_singleGameScore[i] = (-1)*(int)m_pITableFrame->GetPlayerBaseInfo(BankID).m_goldCoin*
							((float)m_GameAccess->GetPlayerAddScore(i) / (float)Addbet);
					}
				}
				msg_singleGameScore[BankID] = m_pITableFrame->GetPlayerBaseInfo(BankID).m_goldCoin;
			}
			else
			{
				msg_singleGameScore[BankID] = BankScore;
			}
		}
		else			//ׯ����
		{
			BASE_PLAYERINFO info = m_pITableFrame->GetPlayerBaseInfo(BankID);
			if ((int)info.m_goldCoin < (int)(-msg_singleGameScore[BankID]))
			{
				for (int i = 0; i < _playersum; i++)
				{
					if (i != BankID)
					{
						msg_singleGameScore[i] = (int)info.m_goldCoin * ((float)m_GameAccess->GetPlayerAddScore(i) / (float)Addbet);

						//ũ�����ֻ��Ӯ���Ľ����ô����
						if (msg_singleGameScore[i] > m_pITableFrame->GetPlayerBaseInfo(i).m_goldCoin)
						{
							msg_singleGameScore[i] = m_pITableFrame->GetPlayerBaseInfo(i).m_goldCoin;
							BankScore -= msg_singleGameScore[i];
						}
						else
						{
							BankScore -= msg_singleGameScore[i];
						}
						cout << "msg_singleGameScore::::" << msg_singleGameScore[i] << endl;
					}
				}
				msg_singleGameScore[BankID] = BankScore;
				cout << "msg_singleGameScore[BankID]::::" << BankScore << endl;
			}
			else
			{
				for (int i = 0; i < _playersum; i++)
				{
					if (i != BankID && msg_singleGameScore[i] > m_pITableFrame->GetPlayerBaseInfo(i).m_goldCoin)
					{
						msg_singleGameScore[i] = m_pITableFrame->GetPlayerBaseInfo(i).m_goldCoin;
						BankScore -= msg_singleGameScore[i];
					}
				}
				msg_singleGameScore[BankID] = BankScore;
			}
		}
	}

	//��ҳ� �ܷ�=���ַ�
	if (m_pITableFrame->GameType() == 1)
	{
		for (BYTE i = 0; i < _playersum; ++i)
		{
			BASE_PLAYERINFO info = m_pITableFrame->GetPlayerBaseInfo(i);
			if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
			{
				msg_totalGameScore[i] = 0;
				msg_totalGameScore[i] = info.m_goldCoin + msg_singleGameScore[i];
			}
			//����Ϊ0
			if (msg_totalGameScore[i] < 0)
			{
				msg_totalGameScore[i] = 0;
			}
		}
	}

	//֪ͨframe����
	BYTE cbCurGameCount = m_GameAccess->GetCurGameCount();
	std::vector<SCORE> vec_score;

	//д��vector
	for (int i = 0; i < _playersum; i++)
	{
		vec_score.push_back(msg_singleGameScore[i]);
	}

	//֪ͨ���С����Ϸ����
	//m_pITableFrame->OnEventXJGameEnd(cbCurGameCount, vec_score);

	//���һ�ֲ�����С����Ϸ�������ͻ��˲���ʾС�ֽ���ҳ��
	//if (cbCurGameCount != m_GameAccess->GetAllCount())					//lih   ���һ��ҲҪ��ʾС�ֽ��� 

		//���ݹ���	�����������

	for (BYTE j = 0; j < _playersum && m_GameAccess != NULL; j++)
	{
		//��������
		STR_CMD_SC_XJ_GAME_END sXJGameEnd;
		ZeroMemory(&sXJGameEnd, sizeof(STR_CMD_SC_XJ_GAME_END));

		sXJGameEnd.wchairID = j;

		for (BYTE i = 0; i < _playersum && m_GameAccess != NULL; i++)
		{
			//��ȡ��ҵ�ǰ������
			BYTE TmpCards[MAX_CARD_COUNT];
			m_GameAccess->GetClientHandCards(i, TmpCards, MAX_CARD_COUNT);

			//��ȡ�������
			BASE_PLAYERINFO player_info = m_pITableFrame->GetPlayerBaseInfo(i);

			//��ֵ���������Ӻ�
			sXJGameEnd.xj_end[i].nTotalGameScore = msg_totalGameScore[i];      //�ܷ�
			sXJGameEnd.xj_end[i].nSingleGameScore = msg_singleGameScore[i];	//���ַ�
			sXJGameEnd.xj_end[i].UsrChairID = i;								//��λ��
			sXJGameEnd.xj_end[i].RoomSocre = m_GameAccess->GetCellScore();     //�׷�
			sXJGameEnd.xj_end[i].UserBet = m_GameAccess->GetAllBet(i);			//����ұ���
			sXJGameEnd.xj_end[i].BankID = BankID;								//ׯ��
			sXJGameEnd.xj_end[i].PlayerName = player_info.m_szPlayerName;		//����

			//�ж�С���Ƿ����
			if (nTotalGameCount == nCurGameCount)
			{
				sXJGameEnd.xj_end[i].IsEnd = 1;    //1����  0δ����
			}

			//�ж�ʤ������ʧ��
			if (BankID == Winner)   //ׯ��Ӯ��
			{
				if (i == BankID)
				{
					sXJGameEnd.xj_end[i].IsWin = 1;    //�Ƿ�Ӯ
				}
				else
				{
					sXJGameEnd.xj_end[i].IsWin = 0;
				}
			}
			else    //ׯ������
			{
				if (i == BankID)
				{
					sXJGameEnd.xj_end[i].IsWin = 0;
				}
				else
				{
					sXJGameEnd.xj_end[i].IsWin = 1;
				}
			}

			//��ֵʣ������
			for (BYTE k = 0; k < m_GameAccess->GetUserCurCardNum(i); k++)
			{
				sXJGameEnd.xj_end[i].cbLeaveHandCard[k] = TmpCards[k];		//��
			}
		}
		//¼��С�ֽ��
		//AddRecordXJEnd(sXJGameEnd);

		//ѭ��������Ϣ
		m_pITableFrame->SendTableData(j, CMD_SC_XJ_GAME_END, &sXJGameEnd, sizeof(STR_CMD_SC_XJ_GAME_END));
	}

	//С������д�����ݿ�
	//WriteGameRecord(vec_score);

	//�������Ƿ��ǿ�ʼ��Ϸ����
	for (int i = 0; i < _playersum; i++)
	{
		m_GameAccess->SetStartMingPai(i, 1);
	}

	//���͹���������Ϣ
	for (int i = 0; i < _playersum; i++)
	{
		if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
		{
			OnUserPublicBet(i);
		}
	}

	//����Ǳ����� ���С������
	if (3 == m_pITableFrame->GameType())
	{
		m_GameLogic->ClearXjGame();
	}


	//������Ϸ״̬ΪС�ֽ���״̬
	m_GameAccess->SetGameStatus(GS_WK_XJ_GAMEEND);    //���ⱻ���

	delete[]	msg_totalGameScore;
	delete[]	msg_singleGameScore;

	return true;
}

// �����Ϸ����
bool CTableFrameSink::DjGameConclude(int nTotalGameCount, int nCurGameCount)
{
	printf("��ֽ���\n");

	//��Ϸ��û��ʼ״̬�µĽ�ɢ����
	if (GS_WK_FREE == m_GameAccess->GetGameStatus())
	{
		return false;
	}

	//У��
	if (GS_WK_XJ_GAMEEND != m_GameAccess->GetGameStatus())
		return false;

	// �����Ŀ
	BYTE _playersum = m_GameAccess->GetCurPlayerCount();

	//���͹���������Ϣ
	for (int i = 0; i < _playersum; i++)
	{
		if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
		{
			OnUserPublicBet(i);
		}
	}


	// ��������
	SCORE	*msg_userScore = new SCORE[_playersum]();
	BYTE	*msg_winTime = new BYTE[_playersum]();
	BYTE	*msg_bankTime = new BYTE[_playersum]();
	SCORE	*msg_BestBet = new SCORE[_playersum]();
	WORD	*Rich = new WORD[_playersum]();    //������
	WORD msg_wMaxWinChairID;


	//Ӯ����
	int nMaxWinNum = 0;

	for (BYTE i = 0; i < _playersum; ++i)
	{
		if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
		{
			msg_userScore[i] = m_GameAccess->GetPlayerTotalScore(i);    //�ܷ�
			//player_info[i] = m_GameAccess->GetPlayerInfo(i);	//�����Ϣ

			m_GameAccess->GetWinSum(i, msg_winTime[i]);					//Ӯ����
			msg_bankTime[i] = m_GameAccess->GetPlayerBankCount(i);		//��ׯ����
			msg_BestBet[i] = m_GameAccess->GetDJPlayerBestBet(i);		//�����

			//��ô�Ӯ�ң����Ӯ������
			if (msg_winTime[i] > nMaxWinNum)
			{
				nMaxWinNum = msg_winTime[i];
				msg_wMaxWinChairID = i;
			}
		}
	}

	//�жϴ�����
	SCORE Loser = 0;
	WORD chair = 0;
	for (BYTE i = 0; i < _playersum; ++i)
	{
		if (Loser > msg_userScore[i])
		{
			chair = i;
			Loser = msg_userScore[i];
		}
	}
	Rich[chair] = 1;

	//�ж��Ƿ�������������
	for (BYTE i = 0; i < _playersum; ++i)
	{
		if (chair != i && msg_userScore[chair] == msg_userScore[i])
		{
			Rich[i] = 1;
		}
	}

	//��������
	STR_CMD_SC_DJ_GAME_END  sDJGameEnd;
	ZeroMemory(&sDJGameEnd, sizeof(STR_CMD_SC_DJ_GAME_END));

	sDJGameEnd.wFangzhuID = m_GameAccess->GetRoomFangzhu();  //����
	sDJGameEnd.wMaxWinChairID = msg_wMaxWinChairID;			//��Ӯ��

	//�жϴ�־��Ƿ����
	if (nTotalGameCount == nCurGameCount)
	{
		sDJGameEnd.IsEnd = 1;    //1����  0δ����
	}

	for (int i = 0; i < _playersum; i++)
	{
		sDJGameEnd.lUserScore[i] = msg_userScore[i];  //�ܷ�
		sDJGameEnd.bWinTime[i] = msg_winTime[i];     //Ӯ����
		sDJGameEnd.BankTime[i] = msg_bankTime[i];	//��������
		sDJGameEnd.PlayerBigestBet[i] = msg_BestBet[i];	 //��¼�����
		sDJGameEnd.Rich_ID[i] = Rich[i];	 //��¼������

		//�����Ϣ
		auto player_info = m_pITableFrame->GetPlayerBaseInfo(i);
		auto pInfo = sDJGameEnd.playerinfo;

		printf("ID:%d\n", player_info.m_PlayerID);
		cout << player_info.m_szPlayerName << endl;

		pInfo[i].playerID = player_info.m_PlayerID;   //ID 
		pInfo[i].playerName = player_info.m_szPlayerName;   //����
		pInfo[i].szFaceUrl = player_info.m_szFaceUrl;	//ͷ���

	}

	//¼���ֽ��
	//AddRecordDJEnd(sDJGameEnd);

	// �㲥��ֽ�����Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_DJ_GAME_END, &sDJGameEnd, sizeof(STR_CMD_SC_DJ_GAME_END));

	//������ϷΪ�����С�״̬
	m_GameAccess->SetGameStatus(GS_WK_FREE);

	//ֱ�ӽ�����Ϸ����Ҫ����
	//m_pITableFrame->OnEventDJGameEnd(GAME_CONCLUDE_NORMAL);

	delete[]	msg_userScore;
	delete[]	msg_winTime;
	delete[]	msg_bankTime;
	delete[]	msg_BestBet;
	delete[]	Rich;

	// ������Ϸ����
	m_GameLogic->ClearXjGame();

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

			// �����Ŀ
			BYTE _playersum = m_GameAccess->GetCurPlayerCount();

			//��Ϣ����
			SCORE	*msg_userScore = new SCORE[_playersum]();
			BYTE	*msg_winTime = new BYTE[_playersum]();
			BYTE	*msg_bankTime = new BYTE[_playersum]();
			SCORE	*msg_BestBet = new SCORE[_playersum]();
			WORD	*Rich = new WORD[_playersum]();    //������
			WORD msg_wMaxWinChairID;


			//Ӯ����
			int nMaxWinNum = 0;

			for (BYTE i = 0; i < _playersum; ++i)
			{
				if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
				{
					msg_userScore[i] = m_GameAccess->GetPlayerTotalScore(i);    //�ܷ�
					//player_info[i] = m_GameAccess->GetPlayerInfo(i);	//�����Ϣ

					m_GameAccess->GetWinSum(i, msg_winTime[i]);					//Ӯ����
					msg_bankTime[i] = m_GameAccess->GetPlayerBankCount(i);		//��ׯ����
					msg_BestBet[i] = m_GameAccess->GetDJPlayerBestBet(i);		//�����

					//��ô�Ӯ�ң����Ӯ������
					if (msg_winTime[i] > nMaxWinNum)
					{
						nMaxWinNum = msg_winTime[i];
						msg_wMaxWinChairID = i;
					}
				}
			}

			//�жϴ�����
			SCORE Loser = 0;
			WORD chair = 0;
			for (BYTE i = 0; i < _playersum; ++i)
			{
				cout << "1 ";
				if (Loser > msg_userScore[i])
				{
					chair = i;
					Loser = msg_userScore[i];
				}
			}
			Rich[chair] = 1;
			cout << "2 ";
			//�ж��Ƿ�������������
			for (BYTE i = 0; i < _playersum; ++i)
			{
				cout << "3 ";
				if (chair != i && msg_userScore[chair] == msg_userScore[i])
				{
					Rich[i] = 1;
				}
			}

			//����ǵ�һ��  ����ʾ������
			if (1 == cbCurGameCount)
			{
				cout << "4 ";
				for (BYTE i = 0; i < _playersum; ++i)
					Rich[i] = 0;
			}

			//��������
			STR_CMD_SC_DJ_GAME_END  sDJGameEnd;
			ZeroMemory(&sDJGameEnd, sizeof(STR_CMD_SC_DJ_GAME_END));

			sDJGameEnd.wFangzhuID = m_GameAccess->GetRoomFangzhu();  //����
			sDJGameEnd.wMaxWinChairID = msg_wMaxWinChairID;			//��Ӯ��

			//�жϴ�־��Ƿ����
			if (cbAllGameCount == cbCurGameCount)
			{
				sDJGameEnd.IsEnd = 1;    //1����  0δ����
			}

			for (int i = 0; i < _playersum; i++)
			{
				sDJGameEnd.lUserScore[i] = msg_userScore[i];  //�ܷ�
				sDJGameEnd.bWinTime[i] = msg_winTime[i];     //Ӯ����
				sDJGameEnd.BankTime[i] = msg_bankTime[i];	//��������
				sDJGameEnd.PlayerBigestBet[i] = msg_BestBet[i];	 //��¼�����
				sDJGameEnd.Rich_ID[i] = Rich[i];	 //��¼������

				//�����Ϣ
				auto player_info = m_pITableFrame->GetPlayerBaseInfo(i);
				auto pInfo = sDJGameEnd.playerinfo;

				printf("ID:%d\n", player_info.m_PlayerID);
				cout << player_info.m_szPlayerName << endl;

				pInfo[i].playerID = player_info.m_PlayerID;   //ID 
				pInfo[i].playerName = player_info.m_szPlayerName;   //����
				pInfo[i].szFaceUrl = player_info.m_szFaceUrl;	//ͷ���

			}

			//�㲥����
			m_pITableFrame->SendTableData(INVALID_CHAIR,CMD_SC_DJ_GAME_END, &sDJGameEnd, sizeof(STR_CMD_SC_DJ_GAME_END));
		
			//֪ͨframe
			m_pITableFrame->ConcludeGame(cbReason);

			// ��������
			m_GameLogic->ClearRoom();

			//�ͷ��ڴ�
			delete m_GameLogic;
			m_GameLogic = NULL;
			m_GameAccess = NULL;

			delete[]	msg_userScore;
			delete[]	msg_winTime;
			delete[]	msg_bankTime;
			delete[]	msg_BestBet;
			delete[]	Rich;

			break;
		}
	case GER_NORMAL:			//��������
		{
			if (cbCurGameCount == cbAllGameCount)		// ��ֽ���		//��ҳ��ж��������
			{
				XjGameConclude(cbAllGameCount, cbCurGameCount);
				DjGameConclude(cbAllGameCount, cbCurGameCount);
			}
			// С�ֽ���
			else if (cbCurGameCount < cbAllGameCount) 
			{
				XjGameConclude(cbAllGameCount, cbCurGameCount);
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
		case GS_WK_ROB:   //��ׯ״̬
		{
			if (INVALID_CHAIR == wChairID)
			{
				cout << "INVALID_CHAIR == wChairID" << endl;
				return false;
			}

			STR_CMD_SC_STATUS_ROB statusRob;
			ZeroMemory(&statusRob, sizeof(STR_CMD_SC_STATUS_ROB));

			int		nPlayerNum = m_GameAccess->GetCurPlayerCount();
			BYTE	*UserCardNum = new BYTE[nPlayerNum]();
			BYTE	wCardData[MAX_CARD_COUNT];

			for (int i = 0; i < nPlayerNum; i++)
			{
				UserCardNum[i] = m_GameAccess->GetUserCurCardNum(i);
			}

			m_GameAccess->GetClientHandCards(wChairID, wCardData, UserCardNum[wChairID]);

			//��ֵ
			statusRob.CurRobUsr = m_GameAccess->GetCurRobUser();
			statusRob.CurJuShu = m_GameAccess->GetCurGameCount();
			//statusRob.replay_code = m_pITableFrame->GetRoomBaseInfo().replay_code;

			for (int i = 0; i < nPlayerNum; i++)
			{
				statusRob.robstate[i] = m_GameAccess->GetBankerState(i);
				statusRob.HandCardNum[i] = UserCardNum[i];
			}

			for (int i = 0; i < UserCardNum[wChairID]; i++)
			{
				statusRob.HandCardData[i] = wCardData[i];
			}

			for (int i = 0; i < nPlayerNum; i++)
			{
				statusRob.room_bet[i] = m_GameAccess->GetAllBet(i);
				statusRob.All_bet[i] = m_GameAccess->GetPlayerTotalScore(i);   //�ܷ�
			}

			m_pITableFrame->SendTableData(wChairID, CMD_SC_STATUS_ROB, &statusRob, sizeof(STR_CMD_SC_STATUS_ROB));

			delete[] UserCardNum;

			break;
		}

		case GS_WK_ADDSCORE:	//��ע״̬
		{
			if (INVALID_CHAIR == wChairID)
			{
				cout << "INVALID_CHAIR == wChairID" << endl;
				return false;
			}

			STR_CMD_SC_STATUS_ADD_SCORE statusAddStore;
			ZeroMemory(&statusAddStore, sizeof(STR_CMD_SC_STATUS_ADD_SCORE));

			int		nPlayerNum = m_GameAccess->GetCurPlayerCount();
			BYTE	*UserCardNum = new BYTE[nPlayerNum]();
			BYTE	wCardData[MAX_CARD_COUNT];

			for (int i = 0; i < nPlayerNum; i++)
			{
				UserCardNum[i] = m_GameAccess->GetUserCurCardNum(i);
			}

			m_GameAccess->GetClientHandCards(wChairID, wCardData, UserCardNum[wChairID]);

			//��ֵ
			statusAddStore.CurJuShu = m_GameAccess->GetCurGameCount();
			statusAddStore.bankID = m_GameLogic->GetAppointBanker();
//			statusAddStore.replay_code = m_pITableFrame->GetRoomBaseInfo().replay_code;

			for (int i = 0; i < nPlayerNum; i++)
			{
				statusAddStore.HandCardNum[i] = UserCardNum[i];
			}

			for (int i = 0; i < UserCardNum[wChairID]; i++)
			{
				statusAddStore.HandCardData[i] = wCardData[i];
			}

			for (int i = 0; i < nPlayerNum; i++)
			{
				statusAddStore.room_bet[i] = m_GameAccess->GetAllBet(i);
				statusAddStore.All_bet[i] = m_GameAccess->GetPlayerTotalScore(i);   //�ܷ�
			}

			m_pITableFrame->SendTableData(wChairID, CMD_SC_STATUS_ADD_SCORE, &statusAddStore, sizeof(STR_CMD_SC_STATUS_ADD_SCORE));

			delete[] UserCardNum;

			break;
		}

		case GS_WK_OUT_CARD:	//����״̬
		{
			cout << "����״̬��������" << endl;

			if (INVALID_CHAIR == wChairID)
			{
				cout << "INVALID_CHAIR == wChairID" << endl;
				return false;
			}

			//���Ӻ�

			STR_CMD_SC_STATUS_OUTCARD StatusOutCard;
			ZeroMemory(&StatusOutCard, sizeof(STR_CMD_SC_STATUS_OUTCARD)); 

			int nPlayerNum = m_GameAccess->GetCurPlayerCount();
			BYTE (*msg_cbCardData)[MAX_CARD_COUNT] = new BYTE[nPlayerNum][MAX_CARD_COUNT]();	//��¼����
			BYTE (*msg_CardData)[MAX_CARD_COUNT] = new BYTE[nPlayerNum][MAX_CARD_COUNT]();		//��¼������ҵ�����
			BYTE msg_LeaveCardData[MAX_LEAVE_CARD_NUM];   //����
			BYTE msg_TurnCardData[MAX_CARD_COUNT];			//��¼��ǰ�ֵ�������GetTurnMaxCards
			BYTE max_card_count = MAX_CARD_COUNT;

			//����������
			ZeroMemory(&msg_TurnCardData, sizeof(msg_TurnCardData));
			m_GameAccess->GetTurnMaxCards(msg_TurnCardData, max_card_count);

			//����
			m_GameAccess->GetLeaveCard(msg_LeaveCardData, MAX_LEAVE_CARD_NUM);

			//��¼������ҵ�����
			for (int i = 0; i < nPlayerNum; i++)
			{
				if (m_GameAccess->GetPlayerMingPaiBet(i) > 1)
				{
					m_GameAccess->GetClientHandCards(i, msg_CardData[i], m_GameAccess->GetUserCurCardNum(i));
				}
			}

			//��ȡ����
			for (int i = 0; i < nPlayerNum; i++)
			{
				if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
				{
					m_GameAccess->GetUserOutCard(i, msg_cbCardData[i]);
				}
			}

			//������ҵ�����
			BYTE *BrokenCardData = new BYTE[m_GameAccess->GetUserCurCardNum(wChairID)]();
			m_GameAccess->GetClientHandCards(wChairID, BrokenCardData, m_GameAccess->GetUserCurCardNum(wChairID));

			for (int i = 0; i < m_GameAccess->GetUserCurCardNum(wChairID); i++)
			{
				StatusOutCard.brokenoutcarddata[i] = BrokenCardData[i];
			}


			for (int i = 0; i < nPlayerNum; i++)
			{
				auto player_op_info = StatusOutCard.players_op;

				//���ø���ҵ���������
				player_op_info[i].op_cardscount = m_GameAccess->GetUserCurCardNum(i);

				//���ø�����Ƿ���Ƶ�״̬
				player_op_info[i].op_type = m_GameAccess->GetTurnOutCardType(i);

				//����������ҵ�����
				for (int j = 0; j < m_GameAccess->GetUserCurCardNum(i); j++)
				{
					player_op_info[i].MingPaiCardData[j] = msg_CardData[i][j];
				}

				//���ø���ҳ��ƽ��
				for (int j = 0; j < m_GameAccess->GetUserOutCard(i, NULL); j++)
				{
					player_op_info[i].op_result[j] = msg_cbCardData[i][j];
				}
			}

			//���õ�ǰ���Ƶ����
			StatusOutCard.outcardid = m_GameAccess->GetCurOutCardUser();
			StatusOutCard.Leave_bet = m_GameAccess->GetLeaveCardBet();  //���Ʊ���

			StatusOutCard.CurJuShu = m_GameAccess->GetCurGameCount();   //��Ϸ����
			StatusOutCard.bankID = m_GameLogic->GetAppointBanker();   //ׯ��
//			StatusOutCard.replay_code = m_pITableFrame->GetRoomBaseInfo().replay_code;

			for (int i = 0; i < nPlayerNum; i++)
			{
				StatusOutCard.room_bet[i] = m_GameAccess->GetAllBet(i);   //���䱶��
				StatusOutCard.MingPaiState[i] = m_GameAccess->GetPlayerMingPaiBet(i);  //����״̬
				StatusOutCard.All_bet[i] = m_GameAccess->GetPlayerTotalScore(i);   //�ܷ�
				StatusOutCard.Add_bet[i] = m_GameAccess->GetPlayerAddScore(i);				//����Ƿ�ӱ�
			}

			for (int i = 0; i < MAX_LEAVE_CARD_NUM; i++)
			{
				StatusOutCard.LeaveCard[i] = msg_LeaveCardData[i];  //����
			}

			for (int i = 0; i < max_card_count; i++)
			{
				StatusOutCard.TurnCardData[i] = msg_TurnCardData[i];  //��ǰ�������
			}

			// ���ͳ���
			m_pITableFrame->SendTableData(wChairID, CMD_SC_STATUS_OUTCARD, &StatusOutCard, sizeof(STR_CMD_SC_STATUS_OUTCARD));

			delete [] msg_cbCardData;
			delete [] msg_CardData;
			delete []  BrokenCardData;

			break;
		}
		case GS_WK_JIAO_FEN:   //�з�״̬
		{
			if (INVALID_CHAIR == wChairID)
			{
				cout << "INVALID_CHAIR == wChairID" << endl;
				return false;
			}

			STR_CMD_SC_STATUS_JIAOFEN statusJiaofen;
			ZeroMemory(&statusJiaofen, sizeof(STR_CMD_SC_STATUS_JIAOFEN));

			int		nPlayerNum = m_GameAccess->GetMaxChairCount();
			BYTE	*UserCardNum = new BYTE[nPlayerNum]();
			BYTE	wCardData[MAX_CARD_COUNT];

			for (int i = 0; i < nPlayerNum; i++)
			{
				UserCardNum[i] = m_GameAccess->GetUserCurCardNum(i);
			}

			m_GameAccess->GetClientHandCards(wChairID, wCardData, UserCardNum[wChairID]);

			//��ֵ
			statusJiaofen.CurRobUsr = m_GameAccess->GetCurJiaoFenUser();
			statusJiaofen.CurJuShu = m_GameAccess->GetCurGameCount();
//			statusJiaofen.replay_code = m_pITableFrame->GetRoomBaseInfo().replay_code;

			for (int i = 0; i < nPlayerNum; i++)
			{
				statusJiaofen.robstate[i] = m_GameAccess->GetCurJiaoFenState(i);
				statusJiaofen.HandCardNum[i] = UserCardNum[i];
			}

			for (int i = 0; i < UserCardNum[wChairID]; i++)
			{
				statusJiaofen.HandCardData[i] = wCardData[i];
			}

			for (int i = 0; i < nPlayerNum; i++)
			{
				statusJiaofen.room_bet[i] = m_GameAccess->GetAllBet(i);
				statusJiaofen.All_bet[i] = m_GameAccess->GetPlayerTotalScore(i);   //�ܷ�
			}

			m_pITableFrame->SendTableData(wChairID, CMD_SC_STATUS_JIAOFEN, &statusJiaofen, sizeof(STR_CMD_SC_STATUS_JIAOFEN));

			delete[] UserCardNum;

			break;
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

			//״̬У��
			if (GS_WK_ROB != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_ROB != m_GameAccess->GetGameStatus() : err ��" << endl;
				return false;
			}

			//û�н�ׯ,Ĭ�ϲ���   ����ǻ����ˣ������ׯ
			for (WORD i = 0; (m_GameAccess != NULL) && (i < m_GameAccess->GetMaxChairCount()); i++)
			{
				cout << "��ʱ���е�ǰ���Ӻ�:" << (int)i << endl;
				cout << "��ʱ���л�ȡ��ǰ��ׯ����ң�" << (int)m_GameAccess->GetCurRobUser() << endl;

				if ((USER_PLAYING == m_GameAccess->GetPlayerState(i))
					&& (i == m_GameAccess->GetCurRobUser()))
				{
					//Clog::InsertLog("��ʱ������Ĭ�ϲ���ׯ");

					WORD wNextUser = m_GameAccess->GetNextUser(i);

					if (wNextUser < 0 || wNextUser >= m_GameAccess->GetMaxChairCount())
					{
						//Clog::InsertLog("func ��ׯ��ʱ�� err : wChairID < 0 && wChairID >= m_GameAccess->GetMaxChairCount(): chairID:%d", wNextUser);
						return false;
					}

					while (i != wNextUser)		//�ж��ǲ������ǲ���
					{
						if (ROB_STATE_AGREE == m_GameAccess->GetBankerState(wNextUser) || ROB_STATE_AGREE == (m_GameAccess->GetBankerState(wNextUser) + 1))
						{
							//�ж�����ǻ����˵����
							if (m_pITableFrame->IsRobot(i))
							{
								BYTE CurCardData[MAX_CARD_COUNT] = { 0 };
								BYTE CurCardNum = m_GameAccess->GetUserCurCardNum(i);

								m_GameAccess->GetClientHandCards(i, CurCardData, CurCardNum);

								//��ʼ����������������
								m_HandCardData[i].Init(CurCardData, CurCardNum);

								//���������ж�����������
								int flag = m_AILogic.LandScore(m_HandCardData[i]);

								if (flag > 1)
								{
									OnUserCallBanker(i, 1, 1);	//������������
								}
								else
								{
									OnUserCallBanker(i, 1, 0);	//������������
								}

								break;
							}
							else
							{
								OnUserCallBanker(i, 1, 0);	//����
								break;
							}
						}

						wNextUser = m_GameAccess->GetNextUser(wNextUser);
					}

					if (i == wNextUser)   //����
					{
						//�ж�����ǻ����˵����
						if (m_pITableFrame->IsRobot(i))
						{
							BYTE CurCardData[MAX_CARD_COUNT] = { 0 };
							BYTE CurCardNum = m_GameAccess->GetUserCurCardNum(i);

							m_GameAccess->GetClientHandCards(i, CurCardData, CurCardNum);

							//��ʼ����������������
							m_HandCardData[i].Init(CurCardData, CurCardNum);

							//���������ж�����������
							int flag = m_AILogic.LandScore(m_HandCardData[i]);

							if (flag > 1)
							{
								OnUserCallBanker(i, 0, 1);	//������������
							}
							else
							{
								OnUserCallBanker(i, 0, 0);	//������������
							}

							break;
						}
						else
						{
							OnUserCallBanker(i, 0, 0);
							break;
						}
					}
					break;
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
		case IDI_ADD_SCORE:		//��ע��ʱ��
		{
			m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);

			//״̬У��
			if (GS_WK_ADDSCORE != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_ADDSCORE != m_GameAccess->GetGameStatus() : err ��" << endl;
				return false;
			}

			//û����ע  Ĭ�ϲ��� 
			for (WORD i = 0; i < m_GameAccess->GetMaxChairCount() && m_GameAccess != NULL; i++)
			{
				if (USER_PLAYING == m_GameAccess->GetPlayerState(i)
					/*&& (0 == m_GameAccess->GetXjReadyState(i))*/
					&& (0 == m_GameAccess->GetAddScoreState(i)))
				{
					printf("��ʱ������Ĭ�ϲ���ע\n");
					//Clog::InsertLog("��ʱ������Ĭ�ϲ���ע %d\n", i);

					BYTE CurCardData[MAX_CARD_COUNT] = { 0 };
					BYTE CurCardNum = m_GameAccess->GetUserCurCardNum(i);

					m_GameAccess->GetClientHandCards(i, CurCardData, CurCardNum);

					//�����˸���������ע
					//��ʼ����������������
					m_HandCardData[i].Init(CurCardData, CurCardNum);

					//���������ж��²���ע
					int flag = m_AILogic.LandScore(m_HandCardData[i]);

					if (m_pITableFrame->IsRobot(i) && 3 == flag)
						OnUserAddScore(i, 2);
					else
						OnUserAddScore(i, 1);
				}

			}

			break;
		}
		case IDI_MING_PAI:		//���ƶ�ʱ��
		{
			m_pITableFrame->KillGameTimer(IDI_MING_PAI);

			//û������ Ĭ�ϲ�����    ������ҲĬ�ϲ�����
			for (WORD i = 0; i < m_GameAccess->GetMaxChairCount() && m_GameAccess != NULL; i++)
			{
				if (USER_PLAYING == m_GameAccess->GetPlayerState(i)
					/*&& (0 == m_GameAccess->GetXjReadyState(i))*/
					&& (0 == m_GameAccess->GetMingPaiState(i)))
				{
					printf("��ʱ������Ĭ�ϲ�����\n");
					//Clog::InsertLog("��ʱ������Ĭ�ϲ����� %d\n", i);

					//����������Ϣ
					On_Sub_UserMingPai(i, 0, 0);
				}
			}

			break;
		}
		case IDI_OUT_CARD:		//���ƶ�ʱ��
		{
			m_pITableFrame->KillGameTimer(IDI_OUT_CARD);

			WORD OutCardUser = m_GameAccess->GetCurOutCardUser();
			if (OutCardUser == INVALID_CHAIR)
			{
				return false;
			}

			//״̬У��
			if (GS_WK_OUT_CARD != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_OUT_CARD != m_GameAccess->GetGameStatus() : err ���й�" << endl;
				return false;
			}

			//�������������ζ�ʱ����ʱ ��ô�������ó��й�״̬
			if (!(m_pITableFrame->IsRobot(OutCardUser)))
			{
				WORD TimeOutNum = m_GameAccess->GetPlayerTimeOutNum(OutCardUser);
				m_GameAccess->SetPlayerTimeOutNum(OutCardUser, ++TimeOutNum);

				if (2 <= TimeOutNum)
				{
					//���ø����Ϊ�й�
					m_GameAccess->SetPlayerTuoGuan(OutCardUser, 1);

					//���ͻ��˷����й���Ϣ
					STR_CMD_SC_TUO_GUAN  tuoguan;
					ZeroMemory(&tuoguan, sizeof(STR_CMD_SC_TUO_GUAN));

					tuoguan.tuo_guan = 1;

					//����
					m_pITableFrame->SendTableData(OutCardUser, CMD_SC_TUO_GUAN, &tuoguan, sizeof(STR_CMD_SC_TUO_GUAN));

					//��������ֵ
					//m_pITableFrame->PlayerAutoOp(OutCardUser);
				}
			}



			//���ǵ�һ������  
			if (1 == m_GameAccess->GetOneTurnEnd())
			{
				BYTE OutCardData[MAX_CARD_COUNT];
				BYTE OutCardNum;

				ZeroMemory(&OutCardData, sizeof(OutCardData));

				//�жϻ����˵����
				if (m_pITableFrame->IsRobot(OutCardUser))
				{
					BYTE CurCardData[MAX_CARD_COUNT] = { 0 };
					BYTE CurCardNum = m_GameAccess->GetUserCurCardNum(OutCardUser);

					m_GameAccess->GetClientHandCards(OutCardUser, CurCardData, CurCardNum);

					//��ʼ����������������
					m_HandCardData[OutCardUser].Init(CurCardData, CurCardNum);

					//��ʼ����ǰ������
					GameSituation m_GameSituation;
					m_GameLogic->GetTurnMaxCardToRobot(m_GameSituation, m_AILogic);

					//���õ����˱������ƺ���
					m_AILogic.get_PutCardList_2_limit(m_GameSituation, m_HandCardData[OutCardUser]);

					cout << "OutCard:";
					for (vector<int>::iterator it = m_HandCardData[OutCardUser].value_nPutCardList.begin(); it < m_HandCardData[OutCardUser].value_nPutCardList.end(); it++)
					{
						cout << ' ' << (int)*it;
					}
					cout << endl;

					//�жϳ�������
					if (m_HandCardData[OutCardUser].uctPutCardType.cgType == cgERROR || m_HandCardData[OutCardUser].uctPutCardType.cgType == cgZERO)
					{
						On_Sub_UserOutCard(OutCardUser, NULL, 0, 1);   //���ƹ�
					}
					else
					{
						//��Ҫ�����ƴ��߼�ֵת�����л�ɫ
						m_GameLogic->GetCardColorValue(OutCardUser, m_HandCardData[OutCardUser], OutCardData, OutCardNum);

						On_Sub_UserOutCard(OutCardUser, OutCardData, OutCardNum, 0);     //����
					}
				}
				else
				{
					//�ж� ��ҿ��Գ�����
					if (m_GameLogic->AnalysePlayerOutCard(OutCardUser, OutCardData, &OutCardNum))
					{
						cout << "OutCard::::";
						for (int i = 0; i < OutCardNum; i++)
						{
							printf("%d ", OutCardData[i]);
						}
						cout << endl;

						if (!m_GameLogic->EfficacyOutCard(OutCardUser, OutCardData, OutCardNum))
						{
							cout << "�ж� ��ҿ��Գ����� err" << endl;
							//Clog::InsertLog("func �ж� ��ҿ��Գ����� err : У��ʧ��");
							return false;
						}
						On_Sub_UserOutCard(OutCardUser, OutCardData, OutCardNum, 0);
					}
					else
						On_Sub_UserOutCard(OutCardUser, NULL, 0, 1);   //���ƹ�	
				}
			}
			//�ǵ�һ������  
			else
			{

				BYTE OutCardData[MAX_CARD_COUNT];
				BYTE OutCardNum;

				ZeroMemory(&OutCardData, sizeof(OutCardData));

				if (m_pITableFrame->IsRobot(OutCardUser))
				{
					BYTE CurCardData[MAX_CARD_COUNT] = { 0 };
					BYTE CurCardNum = m_GameAccess->GetUserCurCardNum(OutCardUser);

					m_GameAccess->GetClientHandCards(OutCardUser, CurCardData, CurCardNum);

					//��ʼ����������������
					m_HandCardData[OutCardUser].Init(CurCardData, CurCardNum);

					//���õ������������ƺ���
					m_AILogic.get_PutCardList_2_unlimit(m_HandCardData[OutCardUser]);

					cout << endl << "OutCard:::::::";
					for (vector<int>::iterator it = m_HandCardData[OutCardUser].value_nPutCardList.begin(); it < m_HandCardData[OutCardUser].value_nPutCardList.end(); it++)
					{
						cout << ' ' << *it;
					}
					cout << endl << endl;

					//��Ҫ�����ƴ��߼�ֵת�����л�ɫ

					m_GameLogic->GetCardColorValue(OutCardUser, m_HandCardData[OutCardUser], OutCardData, OutCardNum);
					if (CT_ERROR == m_GameLogic->GetCardLogicType(OutCardData, OutCardNum))
					{
						//Clog::InsertLog("CT_ERROR == m_GameLogic->GetCardLogicType!!!!!!!!!\n");
						//��������˳��Ƴ��ִ���
						BYTE UOutCardData[MAX_CARD_COUNT];
						BYTE UOutCardNum;
						ZeroMemory(&UOutCardData, sizeof(UOutCardData));
						//�ж� ��һ����ҿ��Գ�����
						m_GameLogic->AnalysePlayerOutCardFirst(OutCardUser, UOutCardData, &UOutCardNum);

						//ִ�г�����Ϣ
						On_Sub_UserOutCard(OutCardUser, UOutCardData, UOutCardNum, 0);
					}
					else
					{
						//ִ�г�����Ϣ
						On_Sub_UserOutCard(OutCardUser, OutCardData, OutCardNum, 0);
					}
				}
				else

				{
					//�ж� ��һ����ҿ��Գ�����
					m_GameLogic->AnalysePlayerOutCardFirst(OutCardUser, OutCardData, &OutCardNum);

					//ִ�г�����Ϣ
					On_Sub_UserOutCard(OutCardUser, OutCardData, OutCardNum, 0);
				}

			}

			break;
		}
		case IDI_ROB_JiaoFen:	//�зֶ�ʱ��
		{
			m_pITableFrame->KillGameTimer(IDI_ROB_JiaoFen);

			//״̬У��
			if (GS_WK_JIAO_FEN != m_GameAccess->GetGameStatus())
			{
				cout << "��ʱ����GS_WK_JIAO_FEN != m_GameAccess->GetGameStatus() : err" << endl;
				return false;
			}

			cout << "�зֶ�ʱ����" << endl;

			//���Ĭ�ϲ���  ����������з�
			for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
			{
				if (i == m_GameAccess->GetCurJiaoFenUser() && USER_PLAYING == m_GameAccess->GetPlayerState(i))
				{
					//���������
					if (m_pITableFrame->IsRobot(i))
					{
						WORD m_Score = JIAOFEN_START;
						for (int j = 0; j < m_GameAccess->GetCurPlayerCount(); j++)
						{
							if ((0 != m_GameAccess->GetCurJiaoFenState(j))
								&& (10 != m_GameAccess->GetCurJiaoFenState(j)))    //Ĭ��״̬�Ͳ���״̬
							{
								m_Score -= 1 << (m_GameAccess->GetCurJiaoFenState(j));
							}
						}

						if (JIAOFEN_START == m_Score)  //���ֽз����
						{
							OnUserJiaoFen(i, (rand() % 4));
							break;
						}
						else if (13 == m_Score)
						{
							int tmp[3] = { 0, 2, 3 };
							OnUserJiaoFen(i, tmp[rand() % 3]);
							break;
						}
						else if (11 == m_Score)
						{
							int tmp[3] = { 0, 1, 3 };
							OnUserJiaoFen(i, tmp[rand() % 3]);
							break;
						}
						else if (5 == m_Score)
						{
							int tmp[2] = { 0, 2 };
							OnUserJiaoFen(i, tmp[rand() % 2]);
							break;
						}
						else if (9 == m_Score)
						{
							int tmp[2] = { 0, 3 };
							OnUserJiaoFen(i, tmp[rand() % 2]);
							break;
						}
						else if (3 == m_Score)
						{
							int tmp[2] = { 0, 1 };
							OnUserJiaoFen(i, tmp[rand() % 2]);
							break;
						}
					}
					else
					{
						OnUserJiaoFen(i, 0);
						break;
					}
				}
			}

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

			//״̬У��
			if (GS_WK_OUT_CARD != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_OUT_CARD != m_GameAccess->GetGameStatus() : err" << endl;
				return false;
			}

			//��������
			STR_SUB_CS_OUT_CARD *pOutCard= (STR_SUB_CS_OUT_CARD *)pData;

			//����û������� �������������ʱ����
			m_GameAccess->SetPlayerTuoGuan(pOutCard->wOutCardUser, 0);

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
			if (wDataSize != sizeof(STR_SUB_CS_ROB_BANKER))
			{
				return true;//������Ϣ Ҳ��ʾ����
			}

			//״̬У��
			if (GS_WK_ROB != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_ROB != m_GameAccess->GetGameStatus() : err" << endl;
				return false;
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

			//״̬У��
			if (GS_WK_ADDSCORE != m_GameAccess->GetGameStatus())
			{
				cout << "(GS_WK_ADDSCORE != m_GameAccess->GetGameStatus() : err" << endl;
				return false;
			}

			STR_SUB_CS_ADD_SCORE *pAddScore = (STR_SUB_CS_ADD_SCORE *)pData;

			//����
			OnUserAddScore(pIServerUserItem->GetChairID(), pAddScore->score);

			break;
		}
		case SUB_CS_MING_PAI:	//����
		{
			//У��
			if (1 != m_GameAccess->GetPlayerMingPaiBet(pIServerUserItem->GetChairID()))
			{
				printf("��ң�%d �Ƿ������ƣ�%d\n", pIServerUserItem->GetChairID(), m_GameAccess->GetMingPaiState(pIServerUserItem->GetChairID()));
				cout << "����Ѿ��������� : err" << endl;
				return false;
			}

			//У������
			if (wDataSize != sizeof(STR_SUB_CS_MING_PAI))
				return true;

			STR_SUB_CS_MING_PAI *pMingPai = (STR_SUB_CS_MING_PAI *)pData;

			On_Sub_UserMingPai(pIServerUserItem->GetChairID(), pMingPai->cbIsMing, pMingPai->cbType, pMingPai->OutCard_bet);
		}
		case SUB_CS_XJ_GAME_READY:       //���С����׼��
		{
			//У������
			if (wDataSize != sizeof(STR_SUB_CS_XJ_USER_READY))
				return true;

			STR_SUB_CS_XJ_USER_READY *pXjReady = (STR_SUB_CS_XJ_USER_READY *)pData;

			//��¼����Ƿ�ʼ��Ϸ����
			printf("��ң�%d �Ƿ�ʼ��Ϸ����:%d\n", pXjReady->wReadyUser, pXjReady->cbBet);
			m_GameAccess->SetStartMingPai(pXjReady->wReadyUser, pXjReady->cbBet);


			OnUserXjGameReady(pXjReady->wReadyUser, pXjReady->cbReady);
			break;
		}
		case SUB_CS_PUBLIC_BET:		//��Ϸ�����Ĺ���������Ϣ
		{
			//У������
			if (wDataSize != sizeof(STR_SUB_CS_PUBLIC_BET))
				return true;

			//״̬У��
			if (GS_WK_FREE == m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_FREE == m_GameAccess->GetGameStatus() : err" << endl;
				return false;
			}

			STR_SUB_CS_PUBLIC_BET *pPublicBet = (STR_SUB_CS_PUBLIC_BET *)pData;

			OnUserPublicBet(pIServerUserItem->GetChairID());

			break;
		}
		case SUB_CS_JIAO_FEN:		//�з�
		{
			//У������
			if (wDataSize != sizeof(STR_SUB_CS_JIAO_FEN))
				return true;

			//״̬У��
			if (GS_WK_JIAO_FEN != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_JIAO_FEN != m_GameAccess->GetGameStatus() : err" << endl;
				return false;
			}

			STR_SUB_CS_JIAO_FEN *pJiaoFen = (STR_SUB_CS_JIAO_FEN *)pData;

			OnUserJiaoFen(pIServerUserItem->GetChairID(), pJiaoFen->Score);

			break;
		}
		case SUB_CS_JIPAIQI:		//������
		{
			//У������
			if (wDataSize != sizeof(STR_SUB_CS_JIPAIQI))
				return true;

			//״̬У��
			if (GS_WK_OUT_CARD != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_OUT_CARD != m_GameAccess->GetGameStatus() : err" << endl;
				return false;
			}

			STR_SUB_CS_JIPAIQI *pJiPaiQi = (STR_SUB_CS_JIPAIQI *)pData;

			OnUserJiPaiQi(pIServerUserItem->GetChairID());

			break;
		}
		case SUB_CS_TUO_GUAN:		//�й�
		{
			//У������
			if (wDataSize != sizeof(STR_SUB_CS_TUO_GUAN))
				return true;

			//״̬У��
			if (GS_WK_OUT_CARD != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_OUT_CARD != m_GameAccess->GetGameStatus() : err ���й�" << endl;
				return false;
			}

			STR_SUB_CS_TUO_GUAN *pTuoGuan = (STR_SUB_CS_TUO_GUAN *)pData;


			//���ø���ҵ��й�״̬
			m_GameAccess->SetPlayerTuoGuan(pIServerUserItem->GetChairID(), pTuoGuan->TuoGuan_state);

			//���ó��ƶ�ʱ��
			if (1 == pTuoGuan->TuoGuan_state)
			{
				m_pITableFrame->KillGameTimer(IDI_OUT_CARD);
				m_pITableFrame->SetGameTimer(IDI_OUT_CARD, IDI_TIME_OUT_CARD*0.05, 0, 0);
			}
			//�ؿͻ����й���Ϣ
			STR_CMD_SC_TUO_GUAN  tuoguan;
			ZeroMemory(&tuoguan, sizeof(STR_CMD_SC_TUO_GUAN));

			tuoguan.tuo_guan = pTuoGuan->TuoGuan_state;

			//����
			m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), CMD_SC_TUO_GUAN, &tuoguan, sizeof(STR_CMD_SC_TUO_GUAN));
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
	//printf("\n���û���Ϸ״̬ = %d��\n", pIServerUserItem->GetUserStatus());
	//if ( pIServerUserItem->GetUserStatus() == USER_SITDOWN )
	if (m_dPlayerState[wChairID] == USER_SITDOWN)
	{
		//�ӿ�ܻ�ô����Ϸ�û�״̬
		m_dPlayerState[wChairID] = USER_ONREADY;
	}

	//�����û�׼��״̬������ܴ������û�״̬��������Ϸ����
	int nReadyNum = 0;
	for (int i = 0; i < m_GameAccess->GetMaxChairCount(); i++)
	{
		if (USER_ONREADY == m_dPlayerState[i])
		{
			m_GameAccess->SetPlayerState(i, USER_ONREADY);
			nReadyNum++;
		}
	}

	//������С������Ϸ��ʼ
	if (MIN_CHAIR_COUNT <= nReadyNum)
	{
		m_pITableFrame->StartGame();
	}

	return true; 
}

// �����Ϸ��ʼ				
bool CTableFrameSink::OnEventGameStart()
{
	// ������Ϸ��ָ�븳ֵ -- �˴���m_GameAccess��m_GameLogicΪͬһ������  ��m_GameData����ͬһ������
	m_GameData.CreateGame( &m_GameLogic, &m_GameAccess );

	// ��ʼ������-���÷������
	if ( !m_GameAccess->SetRoomRule( m_pRoomRuleOption ))
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

	//�²��ԣ�����ʱ��ѡ���Ƿ����ƺͿ�ʼ��Ϸ����
	//�������ﲻ��ׯ
}

//����������
void CTableFrameSink::HandleDeal()
{
	// ϴ�Ʋ�����
	m_GameLogic->Deal();		//ϴ���㷨����ϴ�ƺͲ�ϴ�����ֲ���

	// ����ұ������ϴ����ĵ׷�  �Ժ���Ը� 

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

			//��ǰ�������Ƿ����ƿ�ʼ����� ���ط���---�������

			// ���Ƹ���Ӧ�ͻ��� -- ÿ���ͻ���Ӧ��ֻ���յ��Լ�����
			m_pITableFrame->SendTableData(i, CMD_SC_SEND_CARD, &SendCard, sizeof(STR_CMD_SC_SEND_CARD));
		}
	}

	//�Է���¼�� recode

	// ���ݹ����ж��Ƿ�Ҫ�����������
	if (1 == m_GameAccess->GetMingPaiMode())
	{
		for (int i = 0; i < cbMaxChairCount; i++)
		{
			//�ж�����Ƿ�ʼ��Ϸ������
			printf("��ң�%d �Ƿ�ʼ��Ϸ����:%d\n", i, m_GameAccess->GetStartMingPai(i));
			if (5 == m_GameAccess->GetStartMingPai(i))
			{
				On_Sub_UserMingPai(i, MING_PAI_TYPE_GAMESTART, 1);
			}
			else
			{
				SendMingPaiStart(i, MING_PAI_TYPE_DEAL);    //�������ƿ�ʼ��Ϣ
			}
		}
	}
	//����ֱ�ӿ�ʼ��ׯ
	else
	{
		HandleRobBanker();
	}
}

//������ׯ
void CTableFrameSink::HandleRobBanker()
{
	//ɾ�����ƶ�ʱ��
	m_pITableFrame->KillGameTimer(IDI_MING_PAI);

	// ������Ϸ������״̬��
	m_pITableFrame->SetGameStatus(GS_WK_ROB);

	//���ݷ�������ж���ׯģʽ
	BYTE cbRobBankMode = m_GameAccess->GetRobBankMode();   //�����

	//����һ�ֵ���ҷ��ͼ�����
	for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)			//�����
	{
		OnUserJiPaiQi(i);
	}

	//���ͷ��䱶����Ϣ   //�����

	//1��������	
	if (ROOMRULE_OPTION_ROBTYPE_JIAOSANFEN == cbRobBankMode)
	{
		//�����ִ�������
		HandleJiaoFenRobBanker();
	}

	//2��������ׯ�����͡���ׯ��ʼ��-������ׯ����Ϣ
	if (ROOMRULE_OPTION_ROBTYPE_FREE == cbRobBankMode)       //��������ׯ����ׯ
	{
		//������ׯ��Ϣ��������
		HandleFreeRobBanker();
	}
}

//���������
void CTableFrameSink::HandleJiaoFenRobBanker()
{
	//���һ���˳���  ������
	WORD wChairID = rand() % m_GameAccess->GetCurPlayerCount();

	//���ǵ�һ�� ���Ͼ�Ӯ�ҽз�
	if (1 != m_GameAccess->GetCurGameCount())
	{
		wChairID = m_GameAccess->GetLastGameWinner();
	}

	//���ͽ����ֿ�ʼ��Ϣ
	SendJiaoFenStart(wChairID, JIAOFEN_START);
}	  

//���ͽ����ֿ�ʼ��Ϣ
void CTableFrameSink::SendJiaoFenStart(WORD wChairID, const BYTE &ActionType)
{
	//������Ϣ
	STR_CMD_SC_JIAOFEN_START  JiaofenStart;
	ZeroMemory(&JiaofenStart, sizeof(STR_CMD_SC_JIAOFEN_START));

	//��ֵ
	JiaofenStart.wChairID = wChairID;
	JiaofenStart.ActionType = ActionType;

	//���õ�ǰ���ڽзֵ����     
	m_GameAccess->SetCurJiaoFenUser(wChairID);

	if (INVALID_CHAIR != wChairID)
	{
		//������Ϣ  �㲥
		m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_JIAOFEN_START, &JiaofenStart, sizeof(STR_CMD_SC_JIAOFEN_START));

		//������Ϸ״̬
		m_GameAccess->SetGameStatus(GS_WK_JIAO_FEN);

		//���ý����ֶ�ʱ��  
		m_pITableFrame->KillGameTimer(IDI_ROB_JiaoFen);

		//�жϻ�����
		if (m_pITableFrame->IsRobot(wChairID))     //����� �����˽ӿڻ�û���
		{
			WORD index = (rand() % 4) + 1;
			m_pITableFrame->SetGameTimer(IDI_ROB_JiaoFen, IDI_TIME_ROB_JiaoFen*0.1*index, 0, 0);
		}
		else
		{
			m_pITableFrame->SetGameTimer(IDI_ROB_JiaoFen, IDI_TIME_ROB_JiaoFen, 0, 0);
		}
	}
}

//����ͻ��˷����Ľ�������Ϣ
void CTableFrameSink::OnUserJiaoFen(WORD wChairID, BYTE Score)
{
	//״̬У��
	if (GS_WK_JIAO_FEN != m_GameAccess->GetGameStatus())
	{
		cout << "�з�״̬У��ʧ��" << endl;
		return;
	}

	//ɱ����ʱ��
	m_pITableFrame->KillGameTimer(IDI_ROB_JiaoFen);

	//������Ϣ
	STR_CMD_SC_JIAOFEN_RESULT JiaofenResult;
	ZeroMemory(&JiaofenResult, sizeof(STR_CMD_SC_JIAOFEN_RESULT));

	//��ֵ
	JiaofenResult.wChairID = wChairID;
	JiaofenResult.Score = Score;

	for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
	{
		JiaofenResult.room_bet[i] = m_GameAccess->GetAllBet(i);
	}

	////¼��зֽ��					//¼��ط�  �����
	//AddRecordJiaoSanFen(JiaofenResult);

	//�㲥�зֽ��
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_JIAOFEN_RESULT, &JiaofenResult, sizeof(STR_CMD_SC_JIAOFEN_RESULT));

	//����һ��������¼�зִ��� 
	WORD JiaoNum = m_GameAccess->GetJiaoFenCount();
	m_GameAccess->SetJiaoFenCount(++JiaoNum);
	cout << "�зִ�����" << JiaoNum << endl;

	//��¼��ǰ��ҵĽз�״̬
	m_GameAccess->SetCurJiaoFenState(wChairID, Score);

	//������ֱ�ӽ�����  ������Ϊ����
	if (3 == Score)
	{
		//��ÿ����ҳ��Ͻзֱ���
		for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
		{
			m_GameLogic->Wager(i, Score);
		}

		//ָ����Ϊׯ��
		m_GameLogic->AppointBanker(wChairID);

		//�з���� ������ע״̬
		AllRobBankerOver();
	}
	else
	{
		//��¼�ж��ٸ����е����
		BYTE cbMaxChairCount = m_GameAccess->GetMaxChairCount();
		BYTE cbPassNum = 0;
		for (int i = 0; i < cbMaxChairCount; i++)
		{
			if ((USER_PLAYING == m_GameAccess->GetPlayerState(i)) &&
				(0 == m_GameAccess->GetCurJiaoFenState(i)))    //
			{
				cbPassNum++;
			}
		}

		cout << "���е��������" << (int)cbPassNum << endl;

		//��������˲��� ���·���
		if (3 == cbPassNum)
		{
			//���ֶ����� ���һ����ҵ���
			WORD TurnPassCount = m_GameAccess->GetTurnPassCount();
			m_GameAccess->SetTurnPassCount(++TurnPassCount);

			if (3 == TurnPassCount)
			{
				WORD banker = rand() % cbPassNum;
				//ָ����Ϊׯ��
				m_GameLogic->AppointBanker(banker);

				AllRobBankerOver();

				return;
			}

			//��ռ�¼�Ľзִ���
			m_GameAccess->SetJiaoFenCount(0);

			//�����ҵĽз�״̬  �����ҵ�����״̬  ������Ʊ���
			for (int i = 0; i < cbMaxChairCount; i++)
			{
				m_GameAccess->SetCurJiaoFenState(i, 10);
				m_GameAccess->SetMingPaiState(i, 0);
				m_GameAccess->SetPlayerMingPaiBet(i, 1);
			}

			//��ձ���
			m_GameAccess->CleanAllBet();

			//����ҷ���
			HandleDeal();

			//sleep(4);

			//������ׯ���з֣�
			HandleRobBanker();
		}

		else
		{
			//�����жϽз��Ƿ��Ѿ��й������� ���������ýз��������Ϊ����
			if (3 == JiaoNum)
			{
				WORD m_Score = m_GameAccess->GetCurJiaoFenState(0);
				WORD Banker = 0;
				for (int i = 1; i < m_GameAccess->GetCurPlayerCount(); i++)
				{
					if ((USER_PLAYING == m_GameAccess->GetPlayerState(i)) &&
						(m_Score < m_GameAccess->GetCurJiaoFenState(i)))
					{
						m_Score = m_GameAccess->GetCurJiaoFenState(i);
						Banker = i;
					}
				}

				//��ÿ����ҳ������зֱ���
				for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
				{
					m_GameLogic->Wager(i, m_Score);
				}

				//ָ����Ϊׯ��
				m_GameLogic->AppointBanker(Banker);

				AllRobBankerOver();
			}
			else		//���� ���¸���ҷ��ͽз���Ϣ
			{
				WORD m_Score = JIAOFEN_START;    //1111
				for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
				{
					if ((USER_PLAYING == m_GameAccess->GetPlayerState(i)) &&
						(0 != m_GameAccess->GetCurJiaoFenState(i))
						&& (10 != m_GameAccess->GetCurJiaoFenState(i)))    //Ĭ��״̬�Ͳ���״̬
					{
						m_Score -= 1 << (m_GameAccess->GetCurJiaoFenState(i));
					}
				}

				cout << "���Ķ����Ʋ�����" << m_Score << endl;

				//���¸���ҷ��ͽз���Ϣ
				SendJiaoFenStart(m_GameAccess->GetNextUser(wChairID), m_Score);
			}
		}


	}
}

//����������ׯ
void CTableFrameSink::HandleFreeRobBanker()
{
	//��һ�֣��ӷ�����ʼ������ׯ��ʼ��Ϣ	�ڶ��֣����Ͼֵ�Ӯ�ҿ�ʼ������ׯ��Ϣ

	//��ʼ��Ĭ�ϵ������ׯ״̬
	for (WORD i = 0; i < m_GameAccess->GetMaxChairCount(); i++)   //10��Ĭ��״̬ 
	{
		m_GameAccess->SetBankerState(i, 10, 10);
		//printf("�����ׯ״̬��%d\n", m_GameAccess->GetBankerState(i));
	}

	WORD wChairID = m_GameAccess->GetRoomFangzhu();
	if (1 != m_GameAccess->GetCurGameCount())
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
	RobStart.wChairID = wChairID;

	//���õ�ǰ������ׯ�����
	m_GameAccess->SetCurRobUser(wChairID);
	printf("��ǰ������ׯ����ң�%d\n", wChairID);

	if (INVALID_CHAIR != wChairID)
	{
		m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_ROB_START, &RobStart, sizeof(STR_CMD_SC_ROB_BANKER_START));

		//������Ϸ״̬Ϊ����ׯ״̬��
		m_GameAccess->SetGameStatus(GS_WK_ROB);

		//��ׯ��ʱ��
		m_pITableFrame->KillGameTimer(IDI_ROB_BANKER);

		//�ж��ǲ��ǻ�����
		if (m_pITableFrame->IsRobot(wChairID))     //�����˽ӿڴ����
		{
			WORD index = (rand() % 3) + 1;
			m_pITableFrame->SetGameTimer(IDI_ROB_BANKER, IDI_TIME_ROB_BANKER*0.1*index, 0, 0);
		}
		else
		{
			m_pITableFrame->SetGameTimer(IDI_ROB_BANKER, IDI_TIME_ROB_BANKER, 0, 0);
		}
	}
}

// ����ͻ��˷����ġ���ׯ����Ϣ
void CTableFrameSink::OnUserCallBanker(WORD wChairID, BYTE cbType, BYTE cbRobState)
{
	printf("\n�����������������û� = %d, ��ׯ���� = %d, ��ׯ = %d\n", wChairID, cbType, cbRobState);

	//��������÷��䱶������ׯ����
	if (1 == cbRobState)
	{
		m_GameAccess->SetCurRoomBet(2);
		for (int i = 0; i < m_GameAccess->GetMaxChairCount(); i++)
		{
			if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
			{
				m_GameLogic->Wager(i, 2);      //���������  ��ÿ����ұ�����2
			}
		}
	}

	//�رն�ʱ��
	m_pITableFrame->KillGameTimer(IDI_ROB_BANKER);

	//��Ϸ״̬Ч��
	if (GS_WK_ROB != m_GameAccess->GetGameStatus())
		return;

	//������ׯ�������
	STR_CMD_SC_ROB_RESULT sRobResult;
	ZeroMemory(&sRobResult, sizeof(STR_CMD_SC_ROB_RESULT));

	//��ֵ 
	sRobResult.wChairID  = wChairID;	
	sRobResult.cbType = cbType;			
	sRobResult.cbRobState = cbRobState;	

	for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
	{
		sRobResult.room_bet[i] = m_GameAccess->GetAllBet(i);
	}

	//��ׯ¼��  //�����
	//AddRecordRobResult(sRobResult);

	//�㲥��ׯ���
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_ROB_RESULT, &sRobResult, sizeof(STR_CMD_SC_ROB_RESULT));

	//����һ��������¼��ׯ����      Ϊ���жϹ��������������
	BYTE robnum = m_GameAccess->GetCurBankerCount();
	m_GameAccess->SetCurBankerCount(++robnum);
	printf("��¼��ׯ����:%d\n", robnum);

	//������ҵ�ǰ��ׯ״̬
	m_GameAccess->SetBankerState(wChairID, cbType, cbRobState);

	//���ͬ��������������ׯ����
	BYTE cbRobNum = m_GameAccess->GetUserRobNum(wChairID);
	if (ROB_STATE_AGREE == cbRobState + 1)
	{
		m_GameAccess->SetUserRobNum(wChairID, ++cbRobNum);
	}
	printf("�����ׯ���� %d\n", cbRobNum);

	//��������ζ���ׯ������Ϊׯ��
	if (2 == cbRobNum)
	{
		//ָ����Ϊׯ��
		m_GameLogic->AppointBanker(wChairID);

		AllRobBankerOver();
	}
	else	//��ׯ��δ��ɣ���������¸���ҷ�����ׯ��Ϣ
	{
		//�ж��ǲ���������Ҷ�ѡ����ׯ
		BYTE cbMaxChairCount = m_GameAccess->GetMaxChairCount();
		BYTE cbPassNum = 0;
		for (int i = 0; i < cbMaxChairCount; i++)
		{
			if ((USER_PLAYING == m_GameAccess->GetPlayerState(i)) &&
				(ROB_STATE_PASS == (m_GameAccess->GetBankerState(i) + 1)))    //m_GameAccess->GetBankerState(i)��ֵ  0-��	1-�е���2-������ ROB_STATE_PASS Ϊ1
			{
				cbPassNum++;
			}
		}
		printf("����ׯ�������:%d\n", cbPassNum);

		//�����˷�����ׯ�������·���
		if (cbPassNum == cbMaxChairCount)
		{
			cout << "�����˷�����ׯ" << endl;

			//���ֶ����� ���һ����ҵ���
			WORD TurnPassCount = m_GameAccess->GetTurnPassCount();
			m_GameAccess->SetTurnPassCount(++TurnPassCount);

			if (3 == TurnPassCount)
			{
				WORD banker = rand() % cbPassNum;
				//ָ����Ϊׯ��
				m_GameLogic->AppointBanker(banker);

				AllRobBankerOver();

				return;
			}

			//�������
			//m_GameLogic->ClearXjGame();

			//��ռ�¼����ׯ����
			m_GameAccess->SetCurBankerCount(0);

			//�����ҵ�����״̬ //�����ҵ����Ʊ���
			for (int i = 0; i < cbMaxChairCount; i++)
			{
				m_GameAccess->SetMingPaiState(i, 0);
				m_GameAccess->SetPlayerMingPaiBet(i, 1);
			}



			//��ձ���
			m_GameAccess->CleanAllBet();

			//����ҷ���
			HandleDeal();

			//sleep(4);

			//������ׯ����������
			HandleRobBanker();
		}
		else	//���¸���ҷ�����ׯ��Ϣ
		{
			//����һ����ҷ�����ׯ��ʼ��Ϣ
			WORD wNextUser = m_GameAccess->GetNextUser(wChairID);
			WORD wLastUser = m_GameAccess->GetLastUser(wChairID);

			//�ж��¸���ҵ���ׯ״̬
			while (wNextUser != wChairID)		//�¸���Ҳ��ǡ�����״̬�Ÿ���������������Ϣ
			{
				if (robnum == 4)   //���������ֻ���ĸ��غ�
				{
					if (ROB_STATE_PASS != (m_GameAccess->GetBankerState(wChairID) + 1))    //���Ļغ�������������Ϊ����
					{
						//ָ����Ϊׯ��
						m_GameLogic->AppointBanker(wChairID);

						AllRobBankerOver();
						break;
					}
					else				//���������ׯ���Ϊׯ
					{
						for (int i = (wChairID - cbMaxChairCount - 1); i < wChairID; i++)
						{
							WORD chairid = (i > 0) ? i % cbMaxChairCount : (-i) % cbMaxChairCount;
							if ((USER_PLAYING == m_GameAccess->GetPlayerState(chairid)) &&
								ROB_STATE_PASS != (m_GameAccess->GetBankerState(chairid) + 1))
							{
								//ָ����Ϊׯ��
								m_GameLogic->AppointBanker(chairid);

								AllRobBankerOver();
								break;
							}
						}
						break;
					}
				}

				else if ((int)cbPassNum == 2 && robnum == 2)
				{
					SendRobStart(wNextUser, ROB_TYPE_CALL);
					break;
				}

				else if (ROB_STATE_PASS != (m_GameAccess->GetBankerState(wNextUser) + 1)
					&& (int)cbPassNum != 2)   //������ͬ�� 914
				{
					if ((int)cbPassNum == 1 && robnum == 1)
					{
						SendRobStart(wNextUser, ROB_TYPE_CALL);    //�е�������
						break;
					}
					else
					{
						SendRobStart(wNextUser, ROB_TYPE_ROB);
						break;
					}
				}
				wNextUser = m_GameAccess->GetNextUser(wNextUser);
			}


			//ǰ������Ҷ����� 
			if (wNextUser == wChairID)
			{
				//Ŀǰ��������Ҷ����� ��ȡ��ǰ��ׯ�����  ������Ϊ����
				BYTE cbMaxChairCount = m_GameAccess->GetMaxChairCount();
				WORD cbRobUser;
				for (int i = 0; i < cbMaxChairCount; i++)
				{
					if (ROB_STATE_PASS != (m_GameAccess->GetBankerState(i) + 1))    //m_GameAccess->GetBankerState(i)��ֵ  0-��	1-�е���2-������ ROB_STATE_PASS Ϊ1
					{
						cbRobUser = i;
					}
				}

				//ָ��ׯ��
				m_GameLogic->AppointBanker(cbRobUser);
				AllRobBankerOver();
			}
		}
	}
}

// ȫ����ׯ���, ������ע״̬
void CTableFrameSink::AllRobBankerOver()
{
	//������Ϸ״̬Ϊ����ע״̬��
	m_GameAccess->SetGameStatus(GS_WK_ADDSCORE);

	//�ر���ׯ��ʱ��
	m_pITableFrame->KillGameTimer(IDI_ROB_BANKER);

	//�����ׯ����
	m_GameAccess->SetCurBankerCount(0);

	//ָ��ׯ��
	WORD wBanker = m_GameLogic->GetAppointBanker();
	printf("��ȡ��ǰָ����ׯ�ң�%d", m_GameLogic->GetAppointBanker());

	//У�� ���ﲻ�˳���Ĭ�Ϸ���Ϊׯ��
	if (INVALID_CHAIR == wBanker /*|| !bappointresult*/)
	{
		wBanker = m_GameAccess->GetRoomFangzhu();
	}

	//��¼�����ׯ����  ��ֽ���
	m_GameAccess->SetPlayerBankCount(wBanker);

	//����ָ��ׯ����Ϣ
	SendAppointBanker(wBanker);

	//���͵��Ƹ�ׯ��
	HandleSendLeaveCard(wBanker);

	cout << "���͵���over" << endl;

	//���õ�һ���ʼ�����û�
	m_GameAccess->SetStartOutCardUser(wBanker);

	//���ݹ����Ƿ��ͼ�ע��ʼ���ӱ�����Ϣ
	if (1 == m_GameAccess->GetAddBetMode())
	{
		SendAddScoreStart(INVALID_CHAIR);
	}
	//����ֱ�ӿ�ʼ����
	else
	{
		HandleOutCardStart(wBanker);
	}

	//���͵�ǰ���䱶���������						//�ǵýпͻ��˼��������Ϣ
	STR_CMD_SC_FANGJIAN_BET  fangjian_bet;
	ZeroMemory(&fangjian_bet, sizeof(STR_CMD_SC_FANGJIAN_BET));

	//����������ũ��ı���  ����ũ����ϵ����ı���
	for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
	{
		fangjian_bet.room_bet[i] = m_GameAccess->GetAllBet(i);
	}

	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_FANGJIAN_BET, &fangjian_bet, sizeof(STR_CMD_SC_FANGJIAN_BET));
}

//�����͵���
void CTableFrameSink::HandleSendLeaveCard(const WORD &wSendCardUser)
{
	printf("\n������� = %d���͵��ơ�\n", wSendCardUser);

	//���ݹ����ж��Ƿ���Ƽӱ�
	if (1 == m_GameAccess->GetISLeaveCardDouble())
	{
		m_GameLogic->IsDoubleLeaveCard();
	}

	//�����˶����ܷ������������һ�����̣���ׯ�ҷ��͵���
	m_GameLogic->SendLeaveCardToUser(wSendCardUser);

	//��������ҷ���ׯ���õ����Ƶ���Ϣ
	BYTE cbMaxChairCount = m_GameAccess->GetMaxChairCount();

	//���͵�ǰ���䱶���������						
	STR_CMD_SC_FANGJIAN_BET  fangjian_bet;
	ZeroMemory(&fangjian_bet, sizeof(STR_CMD_SC_FANGJIAN_BET));

	//����������ũ��ı���  ����ũ����ϵ����ı���
	for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
	{
		fangjian_bet.room_bet[i] = m_GameAccess->GetAllBet(i);
	}

	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_FANGJIAN_BET, &fangjian_bet, sizeof(STR_CMD_SC_FANGJIAN_BET));

	//���͵���
	for (WORD i = 0; i < cbMaxChairCount; i++)
	{
		if ((USER_PLAYING == m_GameAccess->GetPlayerState(i)))
		{
			//��ȡ����
			BYTE leavecards[MAX_LEAVE_CARD_NUM];
			m_GameAccess->GetLeaveCard(leavecards, MAX_LEAVE_CARD_NUM);

			//���͵�����Ϣ
			STR_CMD_SC_SEND_LEAVE_CARD SendLeaveCard;
			ZeroMemory(&SendLeaveCard, sizeof(STR_CMD_SC_SEND_LEAVE_CARD));	

			//��ֵ����
			cout << "��ӡҪ���͵ĵ���" << endl;
			for (int i = 0; i < MAX_LEAVE_CARD_NUM; i++)
			{
				SendLeaveCard.cbLeaveCard[i] = leavecards[i];
				printf("%d ", leavecards[i]);
			}
			cout << endl;

			BYTE tmpcards[MAX_CARD_COUNT];

			//��ֵ
			SendLeaveCard.cbMagicCard = m_GameAccess->GetMagicCard();	// Seeqings modify
			SendLeaveCard.wSendCardUser = i;

			//���������
			BYTE cbCardNum = m_GameAccess->GetUserCurCardNum(i);

			//��������
			SendLeaveCard.cbHandCardNum = cbCardNum;

			m_GameLogic->SortCardList(i, cbCardNum, ST_ORDER); 
			m_GameAccess->GetClientHandCards(i, tmpcards, MAX_CARD_COUNT);

			for (int j = 0; j < cbCardNum; j++)
			{
				SendLeaveCard.cbSortedCard[i] = tmpcards[j];
			}

			//����ɫ����
			SendLeaveCard.cbMagicCardNum = m_GameAccess->GetMagicCardNum(i);
			SendLeaveCard.LeaveCardBet = m_GameAccess->GetLeaveCardBet();

			//����¼��
			//if (i == m_GameLogic->GetAppointBanker())    //�����
			//	AddRecordSendLeaveCard(SendLeaveCard);

			// ��������
			m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_USER_SEND_LEAVE_CARD, &SendLeaveCard, sizeof(STR_CMD_SC_SEND_LEAVE_CARD));
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
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_ADD_SCORE_START, &sAddScoreStart, sizeof(STR_CMD_SC_ADD_SCORE_START));
		
	//����״̬
	m_pITableFrame->SetGameStatus(GS_WK_ADDSCORE);

	// ��ע��ʱ��
	m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);

	//�жϻ�����
	if (m_pITableFrame->IsRobot(wChairID))
	{
		WORD index = (rand() % 3) + 1;
		m_pITableFrame->SetGameTimer(IDI_ADD_SCORE_ROBOT, IDI_TIME_ADD_SCORE_ROBOT*index, 0, 0);
	}
	else
	{
		m_pITableFrame->SetGameTimer(IDI_ADD_SCORE, IDI_TIME_ADD_SCORE, 0, 0);
	}
}

// ��ע��ʼ����
void CTableFrameSink::OnUserAddScore( WORD wChairID, SCORE lScore )
{
	printf("\n���������������ա���ע������� = %d����ע = %ld\n", wChairID, lScore);

	//��Ϸ״̬Ч��
	if (GS_WK_ADDSCORE != m_GameAccess->GetGameStatus())   //���趨Ϊ��ע״̬
	{
		cout << "-1" << endl;
		return;
	}


	//�û�״̬У��
	if ((USER_PLAYING != m_GameAccess->GetPlayerState(wChairID))
		&& (1 != m_GameAccess->GetAddScoreState(wChairID)))
	{
		cout << "��ϷЧ��ʧ��" << endl;
		return;
	}

	////ɾ����ע��ʱ��
	//m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);   //ɾ��һ�ξ�ȫɾ����  Ҫ���ں���ɾ

	//�ɶ�μ�ע, ��˴˴��������ע��ɱ��
	m_GameLogic->Wager(wChairID, lScore);

	//����������ũ��ı���		//��ũ����ϵ����ļӱ�
	if (wChairID != m_GameLogic->GetAppointBanker())
	{
		m_GameLogic->Wager(m_GameLogic->GetAppointBanker(), lScore);
	}
	else
	{
		for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
		{
			if (i != m_GameLogic->GetAppointBanker())
				m_GameLogic->Wager(i, lScore);
		}
	}

	//��¼�����ע
	m_GameAccess->SetPlayerAddScore(wChairID, lScore);

	// ���������ע״̬
	m_GameAccess->SetAddScoreState(wChairID, 1);

	// ���õ�ǰ���䱶��
	m_GameAccess->SetCurRoomBet(lScore);

	//��������
	STR_CMD_SC_ADD_SCORE_RESULT sAddScoreResult;
	ZeroMemory(&sAddScoreResult, sizeof(STR_CMD_SC_ADD_SCORE_RESULT)); // Seeqings modify

	//��ֵ
	sAddScoreResult.wChairID	 = wChairID;	
	sAddScoreResult.wAddSocre	 = lScore;		

	for (int i = 0; i < m_GameAccess->GetMaxChairCount(); i++)
	{
		if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
			sAddScoreResult.RoomBet[i] = m_GameAccess->GetAllBet(i);
	}

	//¼��ӱ����		//�����
	//AddRecordAddScoreResult(sAddScoreResult);

	//�㲥��ע���
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_ADD_SCORE_RESULT, &sAddScoreResult, sizeof(STR_CMD_SC_ADD_SCORE_RESULT));

	// �����ע��������
	BYTE cbBetedNum = 0;
	BYTE cbPlayerNum = m_GameAccess->GetMaxChairCount();// ��ȡ�����
	for (BYTE i = 0; i < cbPlayerNum; ++i)
	{
		if (1 == m_GameAccess->GetAddScoreState(i))
			cbBetedNum++;
	}

	// ��ע��ɣ�����
	printf("��ע����������%d\n", cbBetedNum);   //����
	if (cbBetedNum == m_GameAccess->GetCurPlayerCount())
	{
		//����������ũ��ı���
		BYTE bet = 0;
		for (BYTE i = 0; i < cbPlayerNum; ++i)
		{
			if (i != m_GameLogic->GetAppointBanker())
				bet += m_GameAccess->GetPlayerAddScore(i);
		}
		if (bet == 2)
		{
			m_GameLogic->Wager(m_GameLogic->GetAppointBanker(), 2);
		}
		else if (bet == 3)
		{
			m_GameLogic->Wager(m_GameLogic->GetAppointBanker(), 1.5);
		}

		//���ͱ���
		STR_CMD_SC_FANGJIAN_BET  fangjian_bet;
		ZeroMemory(&fangjian_bet, sizeof(STR_CMD_SC_FANGJIAN_BET));

		for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
		{
			fangjian_bet.room_bet[i] = m_GameAccess->GetAllBet(i);
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_FANGJIAN_BET, &fangjian_bet, sizeof(STR_CMD_SC_FANGJIAN_BET));


		//֪ͨׯ�ҳ���
		cout << "ȫ����ע��� ������ƿ�ʼ��Ϣ" << endl;

		WORD wBanker = m_GameLogic->GetAppointBanker();

		//���õ�һ���ʼ�����û�
		m_GameAccess->SetStartOutCardUser(wBanker);

		//��ׯ�ҷ����Ƿ����Ƶ�����
		if (1 == m_GameAccess->GetPlayerMingPaiBet(wBanker) && 1 == m_GameAccess->GetMingPaiMode())
		{
			SendMingPaiStart(wBanker, MING_PAI_TYPE_OUTCARD);
		}

		//��ׯ�ҷ��ͳ�����Ϣ
		HandleOutCardStart(wBanker);
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
void CTableFrameSink::On_Sub_UserMingPai(WORD wChairID, const BYTE &cbMPType, BYTE cbFlag, BYTE FlushCardBet)     //��������	2-��ʼ��Ϸ����	1-���ƿ�ʼ����  3-��������
{																												//�Ƿ�����	0-������	1-����    //�������Ƶı���  4 - 3 - 2
	//��������������
	m_GameAccess->SetMingPaiState(wChairID, 1);

	//�������
	if (cbFlag == 1)
	{
		//������������
		STR_CMD_SC_MING_PAI_RESULT MPResult;
		ZeroMemory(&MPResult, sizeof(STR_CMD_SC_MING_PAI_RESULT));	

		//���Ƽӱ�
		if (MING_PAI_TYPE_GAMESTART == cbMPType)	//��ʼ��Ϸ����
		{
			//�ӱ�
			MPResult.cbTimes = 5;

			// ���õ�ǰ���䱶��
			m_GameAccess->SetCurRoomBet(5);

			//��¼������Ʊ���
			m_GameAccess->SetPlayerMingPaiBet(wChairID, 5);
		}
		else if (MING_PAI_TYPE_OUTCARD == cbMPType)   //��������
		{
			MPResult.cbTimes = 2;	

			// ���õ�ǰ���䱶��
			m_GameAccess->SetCurRoomBet(2);

			//��¼������Ʊ���
			m_GameAccess->SetPlayerMingPaiBet(wChairID, 2);
		}
		else if (MING_PAI_TYPE_DEAL == cbMPType)   //��������
		{
			cout << "FlushCardBet:" << FlushCardBet << endl;

			MPResult.cbTimes = FlushCardBet;	

			// ���õ�ǰ���䱶��
			m_GameAccess->SetCurRoomBet(FlushCardBet);

			//��¼������Ʊ���
			m_GameAccess->SetPlayerMingPaiBet(wChairID, FlushCardBet);

			//��¼�������ģʽ
			m_GameAccess->SetStartMingPai(wChairID, 4);
		}

		//��ֵ
		MPResult.wChairID = wChairID;
		MPResult.cbIsMing = cbFlag;	

		for (int i = 0; i < m_GameAccess->GetMaxChairCount(); i++)
		{
			if (USER_PLAYING == m_GameAccess->GetPlayerState(i) &&
				INVALID_CHAIR == m_GameLogic->GetAppointBanker())
			{
				MPResult.RoomBet[i] = m_GameAccess->GetPlayerMingPaiBet(i);
			}
			else
			{
				MPResult.RoomBet[i] = m_GameAccess->GetAllBet(i);
			}
		}

		BYTE msg_cbHandCard[MAX_CARD_COUNT];

		m_GameAccess->GetClientHandCards(wChairID, msg_cbHandCard, MAX_CARD_COUNT);

		for (int i = 0; i < m_GameAccess->GetUserCurCardNum(wChairID); i++)
		{
			MPResult.cbHandCard[i] = msg_cbHandCard[i];
		}

		//���ƽ��¼��			//�����
		//AddRecordMingPaiResult(MPResult);

		//�㲥���������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_MING_PAI_RESULT, &MPResult, sizeof(STR_CMD_SC_MING_PAI_RESULT));
	}

	//����������������
	BYTE cbmingpaiNum = 0;
	BYTE cbPlayerNum = m_GameAccess->GetMaxChairCount();// ��ȡ�����
	for (BYTE i = 0; i < cbPlayerNum; ++i)
	{
		if (1 == m_GameAccess->GetMingPaiState(i))
			cbmingpaiNum++;
	}
	printf("��������������%d\n", (int)cbmingpaiNum);

	//���ȫ��������� ���ͳ��ƿ�ʼ

	if ((cbmingpaiNum == m_GameAccess->GetCurPlayerCount()))
	{

		if (MING_PAI_TYPE_OUTCARD == cbMPType)
		{
			//������Ҽ����������Ʊ���
			WORD mingbet = 1;
			for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
			{
				if (i != wChairID && mingbet < m_GameAccess->GetPlayerMingPaiBet(i))
				{
					mingbet = m_GameAccess->GetPlayerMingPaiBet(i);
				}
			}
			cout << "mingbet: " << mingbet << endl;
			if (1 == mingbet)
			{
				for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
				{
					m_GameLogic->Wager(i, 2);
				}
			}

		}
		else if (GS_WK_OUT_CARD != m_GameAccess->GetGameStatus())
		{
			cout << "ȫ��������� ������ׯ��ʼ��Ϣ" << endl;

			//������Ҽ����������Ʊ���
			WORD mingbet = 0;
			for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
			{
				if (mingbet < m_GameAccess->GetPlayerMingPaiBet(i))
				{
					mingbet = m_GameAccess->GetPlayerMingPaiBet(i);
				}
			}
			for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
			{
				m_GameLogic->Wager(i, mingbet);
			}

			//������ׯ
			HandleRobBanker();
		}
	}
}

//������ƿ�ʼ��Ϣ
void CTableFrameSink::HandleOutCardStart(const WORD &wOutCardUser)
{
	//ɾ����ע��ʱ��
	m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);

	//���õ�ǰ�������
	m_GameAccess->SetCurOutCardUser(wOutCardUser);

	//��ȡ��ǰ�����������ݺ���Ŀ
	BYTE TurnCardNum = 0;
	BYTE TurnCardData[MAX_CARD_COUNT];
	ZeroMemory(&TurnCardData, sizeof(TurnCardData));

	m_GameAccess->GetTurnMaxCards(TurnCardData, TurnCardNum);

	//�㲥���ƿ�ʼ����Ϣ
	STR_CMD_SC_OUT_CARD_START OutCardStart;
	ZeroMemory(&OutCardStart, sizeof(STR_CMD_SC_OUT_CARD_START));	

	OutCardStart.wOutCardUser = wOutCardUser;	

	//��ǰ�������ݸ�ֵ
	OutCardStart.TurnOutCardNum = TurnCardNum;
	for (int i = 0; i < TurnCardNum; i++)
	{
		OutCardStart.TurnOutCardData[i] = TurnCardData[i];
	}

	//�жϵ�ǰ���ƻغ��Ƿ����
	printf("�жϵ�ǰ�غ��Ƿ����: %d(0-����	1-δ����)\n", m_GameAccess->GetOneTurnEnd());         //0001-����  0010-����  0100-��ʾ  1000-Ҫ����
	if (0 == m_GameAccess->GetOneTurnEnd())
	{
		OutCardStart.ActionType = 1;  //��һ������   ��ʾ���ƺ���ʾ��ť         0101  
	}
	else
	{
		//�ж��ǲ���Ҫ����
		if (m_GameLogic->JudgePlayerOutCard(wOutCardUser))
		{
			cout << "����" << endl;
			OutCardStart.ActionType = 1 + (1 << 1) + (1 << 2);    //0111
		}
		else
		{
			cout << "Ҫ����" << endl;
			OutCardStart.ActionType = 1 << 3;    //1000
		}
	}

	//�㲥���ƿ�ʼ��Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_USER_OUT_CARD_START, &OutCardStart, sizeof(STR_CMD_SC_OUT_CARD_START));

	//���ó�����Ϸ״̬
	m_GameAccess->SetGameStatus(GS_WK_OUT_CARD);

	// ���ƶ�ʱ��
	m_pITableFrame->KillGameTimer(IDI_OUT_CARD);
	//�ж��Ƿ��ǻ�����
	if (m_pITableFrame->IsRobot(wOutCardUser))
	{
		WORD index = (rand() % 4) + 1;
		m_pITableFrame->SetGameTimer(IDI_OUT_CARD, IDI_TIME_OUT_CARD*0.05*index, 0, 0);
	}
	else
	{
		//�ж��Ƿ��й�
		if (1 == m_GameAccess->GetPlayerTuoGuan(wOutCardUser))
		{
			m_pITableFrame->SetGameTimer(IDI_OUT_CARD, IDI_TIME_OUT_CARD*0.05, 0 , 0);
		}
		else
		{
			m_pITableFrame->SetGameTimer(IDI_OUT_CARD, IDI_TIME_OUT_CARD, 0, 0);
		}
	}
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

	// ɾ�����ƶ�ʱ��
	m_pITableFrame->KillGameTimer(IDI_OUT_CARD);

	//�������Ϊ���ƹ�	
	m_GameAccess->SetTurnOutCardType(wOutCardUser, 0);

	//���һ�������Ƶ����
	WORD wLastUser = m_GameAccess->GetLastOutCardUser();

	//У���ǲ����ֵ���ʼ�����
	if ( wNextUser == wLastUser)
	{
		//�ȷ��ͳ��ƽ�������ͻ���
		SendOutCardResult(wOutCardUser, NULL, 0, 0, OUT_CARD_FAIL, OUT_CARD_PASS);

		//�ǣ���һ�ֽ���
		HandleRoundEnd(wNextUser);
	}
	else
	{
		//�ȷ��ͳ��ƽ�������ͻ���
		SendOutCardResult(wOutCardUser, NULL, 0, 0, OUT_CARD_FAIL, OUT_CARD_PASS);

		//��֪ͨ�¸���ҳ��ƿ�ʼ
		HandleOutCardStart(wNextUser);
	}
}

//�����û�����
void CTableFrameSink::HandleOutCard(WORD wOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum)
{
	//�������һ�������ȿ�ʼ������
	WORD wStartUser = m_GameAccess->GetStartOutCardUser();

	//��ô������������
	WORD wLastUser = m_GameAccess->GetLastOutCardUser();

	//����
	int cbCardType = m_GameLogic->GetCardLogicType(cbOutCard, cbOutCardNum);    //ʹ��BYTE����Խ��

	//��һ���������
	if ((TURN_END == m_GameAccess->GetOneTurnEnd()) &&
		(wOutCardUser == wStartUser))
	{
		cout << "��һ�����Ƶ���ң����ͣ�" << (int)cbCardType << endl;
		//����У��
		if (CT_ERROR == cbCardType)		  //���Ŀ���
		{
			cout << "��һ��������ҳ��Ƹ�ʽ����:CT_ERROR == cbCardType" << endl;
			//���ͳ��ƽ��
			SendOutCardResult(wOutCardUser, cbOutCard, cbOutCardNum, cbCardType, OUT_CARD_FAIL, OUT_CARD_NORMAL);  //����ʧ��

			return;
		}
		else
		{
			m_GameAccess->SetOneTurnEnd(TURN_NOT_END);
		}
	}
	else	//����������
	{
		cout << "������Ƶ����" << endl;
		//У�������Ŀ�Ƿ�ʹ��������������ͬ������У��
		if ( /*(cbOutCardNum != m_GameAccess->GetUserOutCard(wLastUser, NULL)) || */
			!m_GameLogic->EfficacyOutCard(wOutCardUser, cbOutCard, cbOutCardNum))
		{
			cout << "������Ƶ���ҵ����ʹ���:" << endl;
			//printf("��ǰ���Ƶ���Ŀ��%d���ϸ���ҳ��Ƶ���Ŀ��%d\n", cbOutCardNum, m_GameAccess->GetUserOutCard(wLastUser, NULL));
			printf("����У�鷵��ֵ��%d\n", m_GameLogic->EfficacyOutCard(wOutCardUser, cbOutCard, cbOutCardNum));

			//���ͳ��ƽ��
			SendOutCardResult(wOutCardUser, cbOutCard, cbOutCardNum, cbCardType, OUT_CARD_FAIL, OUT_CARD_NORMAL);

			return;
		}
	}

	// ɾ�����ƶ�ʱ��
	m_pITableFrame->KillGameTimer(IDI_OUT_CARD);

	//��������ɾ������˿�
	if (!m_GameLogic->RemoveCard(wOutCardUser, cbOutCard, cbOutCardNum))
	{
		printf("��������ɾ������˿�ʧ��\n");
		return;
	}

	//�����ж�������¼
	m_GameAccess->SetSpring(wOutCardUser, 1);

	//���Ƽ�¼
	m_GameAccess->SetUserOutCard(wOutCardUser, cbOutCard, cbOutCardNum);

	//���ֳ���������ݼ�¼
	m_GameAccess->SetTurnMaxCards(cbOutCard, cbOutCardNum);

	//�������Ϊ����	
	m_GameAccess->SetTurnOutCardType(wOutCardUser, 1);

	//���õ�ǰ�������Ϊ�������������
	m_GameAccess->SetLastOutCardUser(wOutCardUser);

	//������һ���������
	WORD wNextUser = m_GameAccess->GetNextUser(wOutCardUser);

	//�������ը����Ϣ
	if ((cbCardType&(1 << CT_BOMB_CARD)) != 0 ||
		(cbCardType&(1 << CT_MISSILE_CARD)) != 0 ||
		(cbCardType&(1 << CT_LAIZI_BOMB)) != 0 /*||
		(cbCardType&(1 << CT_RUAN_BOMB)) != 0*/)
	{
		cout << "����ը����Ϣ" << endl;
		m_GameAccess->SetUserBoomInfo(wOutCardUser, cbCardType);
	}

	//���ͳ��ƽ��
	SendOutCardResult(wOutCardUser, cbOutCard, cbOutCardNum, cbCardType, OUT_CARD_SUCCESS, OUT_CARD_NORMAL);

	//У���ǲ���һ�ֳ������      //Ӧ�ü�⵱ǰ��ҵ����ǲ��ǳ�����
//	if ( wNextUser == m_GameAccess->GetStartOutCardUser() )
	//printf("��ҵ�ǰ�������� : %d\n", m_GameAccess->GetUserCurCardNum(wOutCardUser));
	if (m_GameAccess->GetUserCurCardNum(wOutCardUser) == 0)
	{
		cout << "һ�ֽ���" << endl;
		//һ�ֽ���
		m_GameAccess->SetCurOutCardUser(INVALID_CHAIR);
		HandleRoundEnd(wOutCardUser);
	}
	else
	{
		cout << "֪ͨ�¸���ҳ���" << endl;
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

	//��ȡ��ǰ���������������������
	BYTE cbCurCardCount = m_GameAccess->GetUserCurCardNum(wOutCardUser);
	BYTE *cbCurCardData = new BYTE[cbCurCardCount]();
	m_GameAccess->GetClientHandCards(wOutCardUser, cbCurCardData, cbCurCardCount);

	//��ֵ
	OutCard.wOutCardUser = wOutCardUser;
	OutCard.cbFlag = cbFlag;
	OutCard.cbSuccess = cbSuccess;
	OutCard.cbHandCardNum = cbCurCardCount;

	for (int i = 0; i < m_GameAccess->GetMaxChairCount(); i++)
	{
		if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
		{
			OutCard.RoomBet[i] = m_GameAccess->GetAllBet(i);

			//�Ѽ����������ͻ��˻���
			OnUserJiPaiQi(i);
		}
	}

	//��ֵ��ǰ��������
	for (int i = 0; i < cbCurCardCount; i++)
	{
		OutCard.handcarddata[i] = cbCurCardData[i];
	}


	//��������
	if (1 == cbFlag)
	{
		if (0 == cbSuccess)
		{
			m_pITableFrame->SendTableData(wOutCardUser, CMD_SC_USER_OUT_CARD_RESULT, &OutCard, sizeof(STR_CMD_SC_OUT_CARD_RESULT));
		}
		else
		{
			//��ֵ					
			OutCard.cbOutCardNum = cbOutCardNum;	
			memcpy(OutCard.cbOutCard, cbOutCard, sizeof(BYTE)*cbOutCardNum);	
			OutCard.cbHandCardNum = m_GameAccess->GetUserCurCardNum(wOutCardUser);	
			OutCard.cbCardType = cbCardType;	//����

			//�÷�
			BYTE cbPlayerNum = m_GameAccess->GetMaxChairCount();
			for (BYTE i = 0; i < cbPlayerNum; ++i)
			{
				if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
				{
					OutCard.lSingleGameScore[i] = m_GameAccess->GetPlayerSingleScore(i);
				}
			}
		}
	}
	else	//���Ҫ����
	{
		OutCard.cbFlag = cbFlag;	
	}

	//¼����ƽ��
	//AddRecordOutCardResult(OutCard);

	//�㲥���ƽ��
	m_pITableFrame->SendTableData(wOutCardUser, CMD_SC_USER_OUT_CARD_RESULT, &OutCard, sizeof(STR_CMD_SC_OUT_CARD_RESULT));
	printf("\n���㲥��� = %d�ĳ��ƽ����\n", OutCard.wOutCardUser);	

	delete[] cbCurCardData;
}

//����һ�ֳ��ƽ���
void CTableFrameSink::HandleRoundEnd(const WORD &wWinner)
{		
	//����һ�ֵ÷�
	WORD wScore = m_GameLogic->SettleOneRound(wWinner);

	//����һ�ֽ���
	m_GameAccess->SetOneTurnEnd(TURN_END);

	//�������ǳ��ƻ��ǹ�
	for (int i = 0; i < 3; i++)
	{
		m_GameAccess->SetTurnOutCardType(i, 99);
	}

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

		//��¼Ӯ��
		m_GameAccess->SetWinSum(wWinner, 1);

		m_GameAccess->SetCurOutCardUser(INVALID_CHAIR);

		//�ж�����Ƿ���
		m_GameAccess->JudgeSpring(wWinner);

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

	//¼��ָ��ׯ��
	//AddRecordAppointBanker(AppointBanker);

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

//��Ϸ�����Ļ�ȡ����������Ϣ
void CTableFrameSink::OnUserPublicBet(WORD wChairID)
{
	//���Ӻ�У��
	if (wChairID < 0 || wChairID >= m_GameAccess->GetMaxChairCount())
	{
		//Clog::InsertLog("func OnUserPublicBet err : wChairID < 0 && wChairID >= m_GameAccess->GetMaxChairCount(): chairID:%d", wChairID);
		return;
	}

	//������Ϣ
	STR_CMD_SC_PUBLIC_BET public_bet;
	ZeroMemory(&public_bet, sizeof(STR_CMD_SC_PUBLIC_BET));

	WORD _playernum = m_GameAccess->GetCurPlayerCount();
	WORD LeaveCard_Bet = 1; //���Ʊ���
	WORD Spring_Bet = 1;	//���챶��
	WORD Mingpai_Bet = 1;  //���Ʊ���
	WORD Boom_Num = 0;   //ը������Ŀ
	WORD Boom_Bet = 1;	 //ը���ܱ���
	WORD Rob_Num = 0;    //��ׯ����
	WORD Rob_bet = 1;    //��ׯ����
	WORD public_Bet = 0;  //��������
	tagBoomInfo  boominfo;  //ը����Ϣ
	WORD Rule = m_GameAccess->GetRobBankMode();  //��ȡ��Ϸ����
	cout << "����" << Rule << endl;

	//��ֵ
	public_bet.RoomBet = 5;	//�׷�
	public_bet.IUserAScore = m_GameAccess->GetAllBet(wChairID);   //�ܷ�
	public_bet.Game_State = m_GameAccess->GetGameStatus();		//��Ϸ״̬

	//���Ʊ���
	LeaveCard_Bet = m_GameAccess->GetLeaveCardBet();
	public_bet.LeaveCard = LeaveCard_Bet;

	//���챶��
	for (int i = 0; i < _playernum; i++)
	{
		WORD flag = m_GameAccess->GetSpring(i);
		if (1 == flag)
		{
			Spring_Bet = 2;
		}
	}
	public_bet.Spring = Spring_Bet;


	if (m_GameLogic->GetAppointBanker() < 0 || m_GameLogic->GetAppointBanker() > m_GameAccess->GetMaxChairCount())	//�ж��Ƿ������˵���
		public_bet.bank_ID = -1;    //ׯ��ID
	else
		public_bet.bank_ID = m_GameLogic->GetAppointBanker();    //ׯ��ID

	//�ӱ�
	for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
	{
		cout << "����ҵļӱ���Ϣ" << (int)m_GameAccess->GetPlayerAddScore(i) << endl;
		public_bet.AddScore[i] = m_GameAccess->GetPlayerAddScore(i);    //����ҵļӱ���Ϣ
		Boom_Num += m_GameAccess->GetUserBoomInfo(i, boominfo);			//��ȡը������
		Rob_Num += m_GameAccess->GetUserRobNum(i);						//��ȡ�ܹ���ׯ����
	}

	//�ɹ������ж���ׯ���ֵı���
	if (Rule == 0)
	{
		for (int i = 0; i < Rob_Num; i++)    //������ׯ�ı���
		{
			Rob_bet *= 2;    // 2��4�η���
		}
	}
	else		//�зֱ�����������һ��
	{
		for (int i = 0; i < _playernum; i++)
		{
			if (10 != m_GameAccess->GetCurJiaoFenState(i) && Rob_bet < m_GameAccess->GetCurJiaoFenState(i))
			{
				Rob_bet = m_GameAccess->GetCurJiaoFenState(i);
			}
		}

		if (Rob_bet == 0)
		{
			Rob_bet = 1;
		}
	}

	for (int i = 0; i < Boom_Num; i++)
	{
		Boom_Bet *= RUAN_BOOM_TIMES;
	}

	public_bet.Boom = Boom_Bet;
	public_bet.Rob = Rob_bet;	//��������з�
	public_bet.Rule = Rule;

	//���Ʊ���ȡ������һ��
	Mingpai_Bet = 0;
	for (int i = 0; i < _playernum; i++)
	{
		if (Mingpai_Bet < m_GameAccess->GetPlayerMingPaiBet(i))
		{
			Mingpai_Bet = m_GameAccess->GetPlayerMingPaiBet(i);
		}
	}

	public_bet.MingPai = Mingpai_Bet;

	//��������
	if (Boom_Num == 0)
		if (Mingpai_Bet == 0)
			public_Bet = 15 * Rob_bet * LeaveCard_Bet* Spring_Bet;  //�������� = ��ʼ*����*������*����*ը��*����
		else
			public_Bet = 15 * Rob_bet * Mingpai_Bet * LeaveCard_Bet * Spring_Bet;
	else
		if (Mingpai_Bet == 0)
			public_Bet = 15 * Rob_bet  * Boom_Bet * LeaveCard_Bet * Spring_Bet;  //�������� = ��ʼ*����*������*����*ը��8����
		else
			public_Bet = 15 * Rob_bet * Mingpai_Bet * Boom_Bet * LeaveCard_Bet * Spring_Bet;

	//�ж��Ƿ�ⶥ
	if (m_GameAccess->GetRoomMaxBet() < public_Bet)
	{
		public_bet.public_bet = m_GameAccess->GetRoomMaxBet();
	}
	else
	{
		public_bet.public_bet = public_Bet;
	}

	printf("rob_bet: %d  Mingpai_Bet: %d  public_Bet:%d\n", Rob_bet, Mingpai_Bet, public_Bet);

	m_pITableFrame->SendTableData(wChairID, CMD_SC_PUBLIC_BET, &public_bet, sizeof(STR_CMD_SC_PUBLIC_BET));
}

//����ͻ��˷����ļ�������Ϣ
void CTableFrameSink::OnUserJiPaiQi(WORD wChairID)
{
	//״̬У��
	if (GS_WK_FREE == m_GameAccess->GetGameStatus())
	{
		cout << "OnUserJiPaiQi : err" << endl;
		return;
	}

	//������Ϣ
	STR_CMD_SC_JIPAIQI_START  JiPaiQi;
	ZeroMemory(&JiPaiQi, sizeof(STR_CMD_SC_JIPAIQI_START));

	//�������
	BYTE CardREC[15];
	tagCardRecorder	CardRec;
	ZeroMemory(&CardREC, sizeof(CardREC));
	ZeroMemory(&CardRec, sizeof(CardRec));

	//��ø���ҵĿ�����Ŀ
	m_GameLogic->CardRecorder(wChairID, CardRec);

	CardREC[0] = CardRec.ACount;
	CardREC[1] = CardRec.TwoCount;
	CardREC[2] = CardRec.ThreeCount;
	CardREC[3] = CardRec.FourCount;
	CardREC[4] = CardRec.FiveCount;
	CardREC[5] = CardRec.SixCount;
	CardREC[6] = CardRec.SevenCount;
	CardREC[7] = CardRec.EightCount;
	CardREC[8] = CardRec.NineCount;
	CardREC[9] = CardRec.TenCount;
	CardREC[10] = CardRec.JCount;
	CardREC[11] = CardRec.QCount;
	CardREC[12] = CardRec.KCount;
	CardREC[13] = CardRec.SmallJokerCount;
	CardREC[14] = CardRec.BigJokerCount;

	for (int i = 0; i < 15; i++)
	{
		JiPaiQi.JiPaiQi[i] = CardREC[i];
	}

	m_pITableFrame->SendTableData(wChairID, CMD_SC_JIPAIQI_START, &JiPaiQi, sizeof(STR_CMD_SC_JIPAIQI_START));
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

///*************************************************
//@Description:     ����¼���ͷ��ڴ�
//@Input:           ��
//@Output:          ��
//@Return: lD#�XkQ?�v��0Մ9��E?/@��b�z��?�bj?����F?p_��HK5?2?os+�c�n-?$? q��w?�xl�I��??�S��b�ϊ����^bcVI�H��?ǧ"p8����ǿ�w?=Ӳ�[�W�݈ጂ2Va��D��.�а@??���6��p�x��z
//`t?�oPT<~0M9��
//g

////////////////////////////////////////////////////////////////////////////////////
//��������

extern "C" __declspec(dllexport) VOID * CreateTableFrameSink()
{
	CTableFrameSink *pTableFrameSink = NULL;
	try
	{
		pTableFrameSink = new CTableFrameSink();
		if (pTableFrameSink == NULL)
		{
			throw TEXT("����ʧ��");
		}

		return pTableFrameSink;
	}
	catch (...) {}

	//�������
	SafeDelete(pTableFrameSink);
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////