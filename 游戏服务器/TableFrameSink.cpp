#include "StdAfx.h"
#include "TableFrameSink.h"
#include "GameServerManager.h"
#include "SubGameRule.h"


// ¹¹Ôìº¯Êı
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

// Îö¹¹º¯Êı
CTableFrameSink::~CTableFrameSink()
{
	//Ğ£Ñé
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

// ½Ó¿Ú²éÑ¯--¼ì²âÏà¹ØĞÅÏ¢°æ±¾
VOID * CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

// ³õÊ¼»¯
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//²éÑ¯½Ó¿Ú
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL)
	{
		return false;
	}
	//m_pITableFrame->SetStartMode(START_MODE_FULL_READY);	//ËùÓĞÈË×¼±¸¿ªÊ¼

	////ÓÎÏ·ÅäÖÃ
	//m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	//m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();

	//·¿¿¨ÅäÖÃ
	m_pRoomRuleOption= (tagGameRoomItem*)m_pITableFrame->GetCustomRule();

	//Ğ£Ñé
	if (NULL != m_GameLogic)
	{
		delete m_GameLogic;
		m_GameLogic  = NULL;
		m_GameAccess = NULL; 
	}
	
	return true;
}

// ¸´Î»×À×Ó
VOID  CTableFrameSink::RepositionSink()
{
	if ( NULL != m_GameAccess )
	{
		m_GameAccess->SetGameStatus( GAME_STATUS_FREE ); // µÈ´ı¿ªÊ¼
	}
}

// Ğ¡¾ÖÓÎÏ·½áÊø
bool CTableFrameSink::XjGameConclude(int nTotalGameCount, int nCurGameCount)
{
	// Íæ¼ÒÊıÄ¿
	BYTE _playersum = m_GameAccess->GetMaxChairCount();

	// ¼ÆËãÍæ¼Ò·¿¼äÄÚµÄ×ÜµÃ·Ö
	for (BYTE i = 0; i < _playersum && m_GameAccess != NULL; ++i)
	{
		if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
		{
			m_GameAccess->SetPlayerTotalScore(i, m_GameAccess->GetPlayerSingleScore(i));
		}
	}

	//¼ÇÂ¼Õû´ó¾ÖÖĞµ±Ç°Ğ¡¾ÖµÄµÃ·ÖÊÇ²»ÊÇ×î´ó
	for (BYTE i = 0; i < _playersum && m_GameAccess != NULL; ++i)
	{
		SCORE CurBestBet = m_GameAccess->GetAllBet(i);
		if (m_GameAccess->GetDJPlayerBestBet(i) < CurBestBet)      //ÉÏÒ»¾ÖµÄ·Ö±ÈÕâÒ»¾ÖĞ¡ ¾ÍÌæ»»
		{
			m_GameAccess->SetDJPlayerBestBet(i, CurBestBet);
		}
	}


	// ¹¹½¨Êı¾İ
	SCORE *msg_totalGameScore = new SCORE[_playersum]();
	SCORE *msg_singleGameScore = new SCORE[_playersum]();
	WORD  Winner = m_GameAccess->GetLastGameWinner();   //»ñÈ¡Ó®¼Ò
	WORD  BankID = m_GameLogic->GetAppointBanker();     //»ñÈ¡×¯¼Ò 

	//»ñµÃÍæ¼Òµ¥¾ÖÓÎÏ·µÃ·Ö
	for (BYTE i = 0; i < _playersum && m_GameAccess != NULL; ++i)
	{
		if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
		{
			//Ö±½Ó½«Íæ¼Ò×Ü»ı·Ö·¢¸ø¿Í»§¶Ë£¬±ÜÃâ¿Í»§¶Ë¼ÆËã
			msg_totalGameScore[i] = m_GameAccess->GetPlayerTotalScore(i);
			//Íæ¼Òµ¥¾ÖµÃ·Ö
			msg_singleGameScore[i] = m_GameAccess->GetPlayerSingleScore(i);

			//Èç¹ûÊÇ½ğ±Ò³¡£¬×Ü·ÖÉèÖÃ³É ½ğ±Ò + ×Ü·Ö
			if (m_pITableFrame->GameType() == 1)
			{
				BASE_PLAYERINFO info = m_pITableFrame->GetPlayerBaseInfo(i);
				msg_totalGameScore[i] += info.m_goldCoin;
				//²»ÄÜÎª0
				if (msg_totalGameScore[i] < 0)
				{
					msg_totalGameScore[i] = 0;
				}
			}
		}
	}

	//»ñÈ¡Á½¸öÅ©ÃñµÄ¼Ó±¶±¶ÊıºÍ
	DWORD Addbet = 0;
	for (int i = 0; i < _playersum; i++)
	{
		if (i != BankID)
		{
			Addbet += m_GameAccess->GetPlayerAddScore(i);
		}
	}

	//ÅĞ¶ÏµØÖ÷»òÅ©ÃñÊÇ·ñ½ğ±ÒÊä¸ÉÁË
	if (m_pITableFrame->GameType() == 1)
	{
		SCORE BankScore = 0;
		if (Winner == BankID)		//×¯Ó®ÁË
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
			//Èç¹ûÓ®µÃ½ğ±Ò³¬¹ıÁËµØÖ÷±¾Éí½ğ±Ò
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
		else			//×¯ÊäÁË
		{
			BASE_PLAYERINFO info = m_pITableFrame->GetPlayerBaseInfo(BankID);
			if ((int)info.m_goldCoin < (int)(-msg_singleGameScore[BankID]))
			{
				for (int i = 0; i < _playersum; i++)
				{
					if (i != BankID)
					{
						msg_singleGameScore[i] = (int)info.m_goldCoin * ((float)m_GameAccess->GetPlayerAddScore(i) / (float)Addbet);

						//Å©Ãñ×î¶àÖ»ÄÜÓ®ËûµÄ½ğ±ÒÄÇÃ´¶à½ğ±Ò
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

	//½ğ±Ò³¡ ×Ü·Ö=µ¥¾Ö·Ö
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
			//²»ÄÜÎª0
			if (msg_totalGameScore[i] < 0)
			{
				msg_totalGameScore[i] = 0;
			}
		}
	}

	//Í¨Öªframe´¦Àí
	BYTE cbCurGameCount = m_GameAccess->GetCurGameCount();
	std::vector<SCORE> vec_score;

	//Ğ´Èëvector
	for (int i = 0; i < _playersum; i++)
	{
		vec_score.push_back(msg_singleGameScore[i]);
	}

	//Í¨Öª¿ò¼ÜĞ¡¾ÖÓÎÏ·½áÊø
	//m_pITableFrame->OnEventXJGameEnd(cbCurGameCount, vec_score);

	//×îºóÒ»¾Ö²»·¢ËÍĞ¡¾ÖÓÎÏ·½áÊø£¬¿Í»§¶Ë²»ÏÔÊ¾Ğ¡¾Ö½áËãÒ³Ãæ
	//if (cbCurGameCount != m_GameAccess->GetAllCount())					//lih   ×îºóÒ»¾ÖÒ²ÒªÏÔÊ¾Ğ¡¾Ö½áËã 

		//Êı¾İ¹¹Ôì	Èı¸öÍæ¼ÒÈı·İ

	for (BYTE j = 0; j < _playersum && m_GameAccess != NULL; j++)
	{
		//¹¹ÔìÊı¾İ
		STR_CMD_SC_XJ_GAME_END sXJGameEnd;
		ZeroMemory(&sXJGameEnd, sizeof(STR_CMD_SC_XJ_GAME_END));

		sXJGameEnd.wchairID = j;

		for (BYTE i = 0; i < _playersum && m_GameAccess != NULL; i++)
		{
			//»ñÈ¡Íæ¼Òµ±Ç°µÄÊÖÅÆ
			BYTE TmpCards[MAX_CARD_COUNT];
			m_GameAccess->GetClientHandCards(i, TmpCards, MAX_CARD_COUNT);

			//»ñÈ¡Íæ¼ÒÃû×Ö
			BASE_PLAYERINFO player_info = m_pITableFrame->GetPlayerBaseInfo(i);

			//¸³Öµ·ÖÊıºÍÒÎ×ÓºÅ
			sXJGameEnd.xj_end[i].nTotalGameScore = msg_totalGameScore[i];      //×Ü·Ö
			sXJGameEnd.xj_end[i].nSingleGameScore = msg_singleGameScore[i];	//µ¥¾Ö·Ö
			sXJGameEnd.xj_end[i].UsrChairID = i;								//×ùÎ»ºÅ
			sXJGameEnd.xj_end[i].RoomSocre = m_GameAccess->GetCellScore();     //µ×·Ö
			sXJGameEnd.xj_end[i].UserBet = m_GameAccess->GetAllBet(i);			//¸÷Íæ¼Ò±¶Êı
			sXJGameEnd.xj_end[i].BankID = BankID;								//×¯¼Ò
			sXJGameEnd.xj_end[i].PlayerName = player_info.m_szPlayerName;		//Ãû×Ö

			//ÅĞ¶ÏĞ¡¾ÖÊÇ·ñ½áÊø
			if (nTotalGameCount == nCurGameCount)
			{
				sXJGameEnd.xj_end[i].IsEnd = 1;    //1½áÊø  0Î´½áÊø
			}

			//ÅĞ¶ÏÊ¤Àû»¹ÊÇÊ§°Ü
			if (BankID == Winner)   //×¯¼ÒÓ®ÁË
			{
				if (i == BankID)
				{
					sXJGameEnd.xj_end[i].IsWin = 1;    //ÊÇ·ñÓ®
				}
				else
				{
					sXJGameEnd.xj_end[i].IsWin = 0;
				}
			}
			else    //×¯¼ÒÊäÁË
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

			//¸³ÖµÊ£ÓàÊÖÅÆ
			for (BYTE k = 0; k < m_GameAccess->GetUserCurCardNum(i); k++)
			{
				sXJGameEnd.xj_end[i].cbLeaveHandCard[k] = TmpCards[k];		//ÅÆ
			}
		}
		//Â¼ÏñĞ¡¾Ö½á¹û
		//AddRecordXJEnd(sXJGameEnd);

		//Ñ­»··¢ËÍĞÅÏ¢
		m_pITableFrame->SendTableData(j, CMD_SC_XJ_GAME_END, &sXJGameEnd, sizeof(STR_CMD_SC_XJ_GAME_END));
	}

	//Ğ¡¾ÖÊı¾İĞ´ÈëÊı¾İ¿â
	//WriteGameRecord(vec_score);

	//Çå¿ÕÍæ¼ÒÊÇ·ñÊÇ¿ªÊ¼ÓÎÏ·Ã÷ÅÆ
	for (int i = 0; i < _playersum; i++)
	{
		m_GameAccess->SetStartMingPai(i, 1);
	}

	//·¢ËÍ¹«¹²±¶ÊıĞÅÏ¢
	for (int i = 0; i < _playersum; i++)
	{
		if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
		{
			OnUserPublicBet(i);
		}
	}

	//Èç¹ûÊÇ±ÈÈü³¡ Çå¿ÕĞ¡¾ÖÊı¾İ
	if (3 == m_pITableFrame->GameType())
	{
		m_GameLogic->ClearXjGame();
	}


	//ÉèÖÃÓÎÏ·×´Ì¬ÎªĞ¡¾Ö½áÊø×´Ì¬
	m_GameAccess->SetGameStatus(GS_WK_XJ_GAMEEND);    //ÒÔÃâ±»Çå¿Õ

	delete[]	msg_totalGameScore;
	delete[]	msg_singleGameScore;

	return true;
}

// ´ó¾ÖÓÎÏ·½áÊø
bool CTableFrameSink::DjGameConclude(int nTotalGameCount, int nCurGameCount)
{
	printf("´ó¾Ö½áÊø\n");

	//ÓÎÏ·»¹Ã»¿ªÊ¼×´Ì¬ÏÂµÄ½âÉ¢·¿¼ä
	if (GS_WK_FREE == m_GameAccess->GetGameStatus())
	{
		return false;
	}

	//Ğ£Ñé
	if (GS_WK_XJ_GAMEEND != m_GameAccess->GetGameStatus())
		return false;

	// Íæ¼ÒÊıÄ¿
	BYTE _playersum = m_GameAccess->GetCurPlayerCount();

	//·¢ËÍ¹«¹²±¶ÊıĞÅÏ¢
	for (int i = 0; i < _playersum; i++)
	{
		if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
		{
			OnUserPublicBet(i);
		}
	}


	// ¹¹ÔìÊı¾İ
	SCORE	*msg_userScore = new SCORE[_playersum]();
	BYTE	*msg_winTime = new BYTE[_playersum]();
	BYTE	*msg_bankTime = new BYTE[_playersum]();
	SCORE	*msg_BestBet = new SCORE[_playersum]();
	WORD	*Rich = new WORD[_playersum]();    //´óÍÁºÀ
	WORD msg_wMaxWinChairID;


	//Ó®´ÎÊı
	int nMaxWinNum = 0;

	for (BYTE i = 0; i < _playersum; ++i)
	{
		if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
		{
			msg_userScore[i] = m_GameAccess->GetPlayerTotalScore(i);    //×Ü·Ö
			//player_info[i] = m_GameAccess->GetPlayerInfo(i);	//Íæ¼ÒĞÅÏ¢

			m_GameAccess->GetWinSum(i, msg_winTime[i]);					//Ó®´ÎÊı
			msg_bankTime[i] = m_GameAccess->GetPlayerBankCount(i);		//×ö×¯´ÎÊı
			msg_BestBet[i] = m_GameAccess->GetDJPlayerBestBet(i);		//×î´ó±¶Êı

			//»ñµÃ´óÓ®¼Ò£¨×î´óÓ®´ÎÊı£©
			if (msg_winTime[i] > nMaxWinNum)
			{
				nMaxWinNum = msg_winTime[i];
				msg_wMaxWinChairID = i;
			}
		}
	}

	//ÅĞ¶Ï´óÍÁºÀ
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

	//ÅĞ¶ÏÊÇ·ñÓĞÁ½¸ö´óÍÁºÀ
	for (BYTE i = 0; i < _playersum; ++i)
	{
		if (chair != i && msg_userScore[chair] == msg_userScore[i])
		{
			Rich[i] = 1;
		}
	}

	//¹¹ÔìÊı¾İ
	STR_CMD_SC_DJ_GAME_END  sDJGameEnd;
	ZeroMemory(&sDJGameEnd, sizeof(STR_CMD_SC_DJ_GAME_END));

	sDJGameEnd.wFangzhuID = m_GameAccess->GetRoomFangzhu();  //·¿Ö÷
	sDJGameEnd.wMaxWinChairID = msg_wMaxWinChairID;			//´óÓ®¼Ò

	//ÅĞ¶Ï´ó¾Ö¾ÖÊÇ·ñ½áÊø
	if (nTotalGameCount == nCurGameCount)
	{
		sDJGameEnd.IsEnd = 1;    //1½áÊø  0Î´½áÊø
	}

	for (int i = 0; i < _playersum; i++)
	{
		sDJGameEnd.lUserScore[i] = msg_userScore[i];  //×Ü·Ö
		sDJGameEnd.bWinTime[i] = msg_winTime[i];     //Ó®´ÎÊı
		sDJGameEnd.BankTime[i] = msg_bankTime[i];	//µØÖ÷´ÎÊı
		sDJGameEnd.PlayerBigestBet[i] = msg_BestBet[i];	 //¼ÇÂ¼×î´ó±¶Êı
		sDJGameEnd.Rich_ID[i] = Rich[i];	 //¼ÇÂ¼´óÍÁºÀ

		//Íæ¼ÒĞÅÏ¢
		auto player_info = m_pITableFrame->GetPlayerBaseInfo(i);
		auto pInfo = sDJGameEnd.playerinfo;

		printf("ID:%d\n", player_info.m_PlayerID);
		cout << player_info.m_szPlayerName << endl;

		pInfo[i].playerID = player_info.m_PlayerID;   //ID 
		pInfo[i].playerName = player_info.m_szPlayerName;   //Ãû×Ö
		pInfo[i].szFaceUrl = player_info.m_szFaceUrl;	//Í·Ïñ¿ò

	}

	//Â¼Ïñ´ó¾Ö½á¹û
	//AddRecordDJEnd(sDJGameEnd);

	// ¹ã²¥´ó¾Ö½áÊøÏûÏ¢
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_DJ_GAME_END, &sDJGameEnd, sizeof(STR_CMD_SC_DJ_GAME_END));

	//ÉèÖÃÓÎÏ·Îª¡¾¿ÕÏĞ¡¿×´Ì¬
	m_GameAccess->SetGameStatus(GS_WK_FREE);

	//Ö±½Ó½áÊøÓÎÏ·£¬²»ÒªĞø·Ñ
	//m_pITableFrame->OnEventDJGameEnd(GAME_CONCLUDE_NORMAL);

	delete[]	msg_userScore;
	delete[]	msg_winTime;
	delete[]	msg_bankTime;
	delete[]	msg_BestBet;
	delete[]	Rich;

	// ÇåÀíÓÎÏ·Êı¾İ
	m_GameLogic->ClearXjGame();

	// ÇåÀí´ó¾ÖÊı¾İ
	m_GameLogic->ClearDjGame();

	//´ó¾Ö½áÊø£¬Çå¿ÕÄÚ´æ -- Èç¹û ÓÎÏ·½áÊø±êÖ¾Î» GAME_CONCLUDE_CONTINUE, ÕâÀï¾Í²»ÄÜÇå¿ÕÄÚ´æ
	m_GameLogic->ClearRoom();

	return true;
}

// ÓÎÏ·½áÊø
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//¹Ø±ÕÌ¯ÅÆ¶¨Ê±Æ÷
	m_pITableFrame->KillGameTimer(IDI_OUT_CARD);

	// ÓÎÏ·µ±Ç°¾ÖÊı
	BYTE cbCurGameCount = m_GameAccess->GetCurGameCount();

	// ×Ü¾ÖÊı
	WORD cbAllGameCount = m_GameAccess->GetAllCount();

	// Íæ¼ÒÊıÄ¿
	BYTE cbPlayersum = m_GameAccess->GetMaxChairCount(); 

	switch (cbReason)
	{
	case GER_DISMISS:			// ½âÉ¢·¿¼ä
		{
			printf("\n½âÉ¢·¿¼ä\n");

			// Íæ¼ÒÊıÄ¿
			BYTE _playersum = m_GameAccess->GetCurPlayerCount();

			//ÏûÏ¢¹¹Ôì
			SCORE	*msg_userScore = new SCORE[_playersum]();
			BYTE	*msg_winTime = new BYTE[_playersum]();
			BYTE	*msg_bankTime = new BYTE[_playersum]();
			SCORE	*msg_BestBet = new SCORE[_playersum]();
			WORD	*Rich = new WORD[_playersum]();    //´óÍÁºÀ
			WORD msg_wMaxWinChairID;


			//Ó®´ÎÊı
			int nMaxWinNum = 0;

			for (BYTE i = 0; i < _playersum; ++i)
			{
				if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
				{
					msg_userScore[i] = m_GameAccess->GetPlayerTotalScore(i);    //×Ü·Ö
					//player_info[i] = m_GameAccess->GetPlayerInfo(i);	//Íæ¼ÒĞÅÏ¢

					m_GameAccess->GetWinSum(i, msg_winTime[i]);					//Ó®´ÎÊı
					msg_bankTime[i] = m_GameAccess->GetPlayerBankCount(i);		//×ö×¯´ÎÊı
					msg_BestBet[i] = m_GameAccess->GetDJPlayerBestBet(i);		//×î´ó±¶Êı

					//»ñµÃ´óÓ®¼Ò£¨×î´óÓ®´ÎÊı£©
					if (msg_winTime[i] > nMaxWinNum)
					{
						nMaxWinNum = msg_winTime[i];
						msg_wMaxWinChairID = i;
					}
				}
			}

			//ÅĞ¶Ï´óÍÁºÀ
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
			//ÅĞ¶ÏÊÇ·ñÓĞÁ½¸ö´óÍÁºÀ
			for (BYTE i = 0; i < _playersum; ++i)
			{
				cout << "3 ";
				if (chair != i && msg_userScore[chair] == msg_userScore[i])
				{
					Rich[i] = 1;
				}
			}

			//Èç¹ûÊÇµÚÒ»¾Ö  ²»ÏÔÊ¾´óÍÁºÀ
			if (1 == cbCurGameCount)
			{
				cout << "4 ";
				for (BYTE i = 0; i < _playersum; ++i)
					Rich[i] = 0;
			}

			//¹¹ÔìÊı¾İ
			STR_CMD_SC_DJ_GAME_END  sDJGameEnd;
			ZeroMemory(&sDJGameEnd, sizeof(STR_CMD_SC_DJ_GAME_END));

			sDJGameEnd.wFangzhuID = m_GameAccess->GetRoomFangzhu();  //·¿Ö÷
			sDJGameEnd.wMaxWinChairID = msg_wMaxWinChairID;			//´óÓ®¼Ò

			//ÅĞ¶Ï´ó¾Ö¾ÖÊÇ·ñ½áÊø
			if (cbAllGameCount == cbCurGameCount)
			{
				sDJGameEnd.IsEnd = 1;    //1½áÊø  0Î´½áÊø
			}

			for (int i = 0; i < _playersum; i++)
			{
				sDJGameEnd.lUserScore[i] = msg_userScore[i];  //×Ü·Ö
				sDJGameEnd.bWinTime[i] = msg_winTime[i];     //Ó®´ÎÊı
				sDJGameEnd.BankTime[i] = msg_bankTime[i];	//µØÖ÷´ÎÊı
				sDJGameEnd.PlayerBigestBet[i] = msg_BestBet[i];	 //¼ÇÂ¼×î´ó±¶Êı
				sDJGameEnd.Rich_ID[i] = Rich[i];	 //¼ÇÂ¼´óÍÁºÀ

				//Íæ¼ÒĞÅÏ¢
				auto player_info = m_pITableFrame->GetPlayerBaseInfo(i);
				auto pInfo = sDJGameEnd.playerinfo;

				printf("ID:%d\n", player_info.m_PlayerID);
				cout << player_info.m_szPlayerName << endl;

				pInfo[i].playerID = player_info.m_PlayerID;   //ID 
				pInfo[i].playerName = player_info.m_szPlayerName;   //Ãû×Ö
				pInfo[i].szFaceUrl = player_info.m_szFaceUrl;	//Í·Ïñ¿ò

			}

			//¹ã²¥Êı¾İ
			m_pITableFrame->SendTableData(INVALID_CHAIR,CMD_SC_DJ_GAME_END, &sDJGameEnd, sizeof(STR_CMD_SC_DJ_GAME_END));
		
			//Í¨Öªframe
			m_pITableFrame->ConcludeGame(cbReason);

			// ÇåÀíÊı¾İ
			m_GameLogic->ClearRoom();

			//ÊÍ·ÅÄÚ´æ
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
	case GER_NORMAL:			//Õı³£½áÊø
		{
			if (cbCurGameCount == cbAllGameCount)		// ´ó¾Ö½áÊø		//½ğ±Ò³¡ÅĞ¶ÏÕâÀï¼ÓÉÏ
			{
				XjGameConclude(cbAllGameCount, cbCurGameCount);
				DjGameConclude(cbAllGameCount, cbCurGameCount);
			}
			// Ğ¡¾Ö½áÊø
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

// ¶ÏÏßÖØÁ¬Ê±·¢ËÍ³¡¾°
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
		case GS_WK_FREE:		//¿ÕÏĞ×´Ì¬
		{
			//Èç¹û·¿¼äÅäÖÃÎª¿Õ£¬»ñÈ¡·¿¼äÅäÖÃ
			if ( NULL == m_pRoomRuleOption )
			{
				m_pRoomRuleOption = (tagGameRoomItem*)m_pITableFrame->GetCustomRule();
			}

			//¿ÕÏĞ×´Ì¬
			STR_CMD_SC_STATUS_FREE StatusFree;
			ZeroMemory( &StatusFree, sizeof( StatusFree ) );

			// ÉèÖÃ±äÁ¿
			//StatusFree.GameCount = m_pRoomRuleOption->GameCount;
			//StatusFree.PlayerCount = m_pRoomRuleOption->PlayerCount;
			//StatusFree.CellScore = m_pRoomRuleOption->CellScore;
			//StatusFree.FangZhu = m_pRoomRuleOption->FangZhu;

			// ·¢ËÍ³¡¾°
			return m_pITableFrame->SendGameScene( pIServerUserItem, &StatusFree, sizeof(StatusFree));
		}
		case GS_WK_ROB:   //ÇÀ×¯×´Ì¬
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

			//¸³Öµ
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
				statusRob.All_bet[i] = m_GameAccess->GetPlayerTotalScore(i);   //×Ü·Ö
			}

			m_pITableFrame->SendTableData(wChairID, CMD_SC_STATUS_ROB, &statusRob, sizeof(STR_CMD_SC_STATUS_ROB));

			delete[] UserCardNum;

			break;
		}

		case GS_WK_ADDSCORE:	//ÏÂ×¢×´Ì¬
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

			//¸³Öµ
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
				statusAddStore.All_bet[i] = m_GameAccess->GetPlayerTotalScore(i);   //×Ü·Ö
			}

			m_pITableFrame->SendTableData(wChairID, CMD_SC_STATUS_ADD_SCORE, &statusAddStore, sizeof(STR_CMD_SC_STATUS_ADD_SCORE));

			delete[] UserCardNum;

			break;
		}

		case GS_WK_OUT_CARD:	//³öÅÆ×´Ì¬
		{
			cout << "³öÅÆ×´Ì¬¶ÏÏßÖØÁ¬" << endl;

			if (INVALID_CHAIR == wChairID)
			{
				cout << "INVALID_CHAIR == wChairID" << endl;
				return false;
			}

			//ÒÎ×ÓºÅ

			STR_CMD_SC_STATUS_OUTCARD StatusOutCard;
			ZeroMemory(&StatusOutCard, sizeof(STR_CMD_SC_STATUS_OUTCARD)); 

			int nPlayerNum = m_GameAccess->GetCurPlayerCount();
			BYTE (*msg_cbCardData)[MAX_CARD_COUNT] = new BYTE[nPlayerNum][MAX_CARD_COUNT]();	//¼ÇÂ¼³öÅÆ
			BYTE (*msg_CardData)[MAX_CARD_COUNT] = new BYTE[nPlayerNum][MAX_CARD_COUNT]();		//¼ÇÂ¼ÃûÅÆÍæ¼ÒµÄÊÖÅÆ
			BYTE msg_LeaveCardData[MAX_LEAVE_CARD_NUM];   //µ×ÅÆ
			BYTE msg_TurnCardData[MAX_CARD_COUNT];			//¼ÇÂ¼µ±Ç°ÂÖµÄ×î´ó³öÅÆGetTurnMaxCards
			BYTE max_card_count = MAX_CARD_COUNT;

			//×î´ó³öÅÆÊıÁ¿
			ZeroMemory(&msg_TurnCardData, sizeof(msg_TurnCardData));
			m_GameAccess->GetTurnMaxCards(msg_TurnCardData, max_card_count);

			//µ×ÅÆ
			m_GameAccess->GetLeaveCard(msg_LeaveCardData, MAX_LEAVE_CARD_NUM);

			//¼ÇÂ¼ÃûÅÆÍæ¼ÒµÄÊÖÅÆ
			for (int i = 0; i < nPlayerNum; i++)
			{
				if (m_GameAccess->GetPlayerMingPaiBet(i) > 1)
				{
					m_GameAccess->GetClientHandCards(i, msg_CardData[i], m_GameAccess->GetUserCurCardNum(i));
				}
			}

			//»ñÈ¡ÊÖÅÆ
			for (int i = 0; i < nPlayerNum; i++)
			{
				if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
				{
					m_GameAccess->GetUserOutCard(i, msg_cbCardData[i]);
				}
			}

			//¶ÏÏßÍæ¼ÒµÄÊÖÅÆ
			BYTE *BrokenCardData = new BYTE[m_GameAccess->GetUserCurCardNum(wChairID)]();
			m_GameAccess->GetClientHandCards(wChairID, BrokenCardData, m_GameAccess->GetUserCurCardNum(wChairID));

			for (int i = 0; i < m_GameAccess->GetUserCurCardNum(wChairID); i++)
			{
				StatusOutCard.brokenoutcarddata[i] = BrokenCardData[i];
			}


			for (int i = 0; i < nPlayerNum; i++)
			{
				auto player_op_info = StatusOutCard.players_op;

				//ÉèÖÃ¸÷Íæ¼ÒµÄÊÖÅÆÊıÁ¿
				player_op_info[i].op_cardscount = m_GameAccess->GetUserCurCardNum(i);

				//ÉèÖÃ¸÷Íæ¼ÒÊÇ·ñ³öÅÆµÄ×´Ì¬
				player_op_info[i].op_type = m_GameAccess->GetTurnOutCardType(i);

				//ÉèÖÃÃûÅÆÍæ¼ÒµÄÊÖÅÆ
				for (int j = 0; j < m_GameAccess->GetUserCurCardNum(i); j++)
				{
					player_op_info[i].MingPaiCardData[j] = msg_CardData[i][j];
				}

				//ÉèÖÃ¸÷Íæ¼Ò³öÅÆ½á¹û
				for (int j = 0; j < m_GameAccess->GetUserOutCard(i, NULL); j++)
				{
					player_op_info[i].op_result[j] = msg_cbCardData[i][j];
				}
			}

			//ÉèÖÃµ±Ç°³öÅÆµÄÍæ¼Ò
			StatusOutCard.outcardid = m_GameAccess->GetCurOutCardUser();
			StatusOutCard.Leave_bet = m_GameAccess->GetLeaveCardBet();  //µ×ÅÆ±¶Êı

			StatusOutCard.CurJuShu = m_GameAccess->GetCurGameCount();   //ÓÎÏ·¾ÖÊı
			StatusOutCard.bankID = m_GameLogic->GetAppointBanker();   //×¯¼Ò
//			StatusOutCard.replay_code = m_pITableFrame->GetRoomBaseInfo().replay_code;

			for (int i = 0; i < nPlayerNum; i++)
			{
				StatusOutCard.room_bet[i] = m_GameAccess->GetAllBet(i);   //·¿¼ä±¶Êı
				StatusOutCard.MingPaiState[i] = m_GameAccess->GetPlayerMingPaiBet(i);  //Ã÷ÅÆ×´Ì¬
				StatusOutCard.All_bet[i] = m_GameAccess->GetPlayerTotalScore(i);   //×Ü·Ö
				StatusOutCard.Add_bet[i] = m_GameAccess->GetPlayerAddScore(i);				//Íæ¼ÒÊÇ·ñ¼Ó±¶
			}

			for (int i = 0; i < MAX_LEAVE_CARD_NUM; i++)
			{
				StatusOutCard.LeaveCard[i] = msg_LeaveCardData[i];  //µ×ÅÆ
			}

			for (int i = 0; i < max_card_count; i++)
			{
				StatusOutCard.TurnCardData[i] = msg_TurnCardData[i];  //µ±Ç°ÂÖ×î´ó¿¨ÅÆ
			}

			// ·¢ËÍ³¡¾°
			m_pITableFrame->SendTableData(wChairID, CMD_SC_STATUS_OUTCARD, &StatusOutCard, sizeof(STR_CMD_SC_STATUS_OUTCARD));

			delete [] msg_cbCardData;
			delete [] msg_CardData;
			delete []  BrokenCardData;

			break;
		}
		case GS_WK_JIAO_FEN:   //½Ğ·Ö×´Ì¬
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

			//¸³Öµ
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
				statusJiaofen.All_bet[i] = m_GameAccess->GetPlayerTotalScore(i);   //×Ü·Ö
			}

			m_pITableFrame->SendTableData(wChairID, CMD_SC_STATUS_JIAOFEN, &statusJiaofen, sizeof(STR_CMD_SC_STATUS_JIAOFEN));

			delete[] UserCardNum;

			break;
		}
		case GS_WK_XJ_GAMEEND:
		{
			STR_CMD_SC_STATUS_XJ_END StatusXJEnd;
			ZeroMemory(&StatusXJEnd, sizeof(STR_CMD_SC_STATUS_XJ_END));

			//»ñµÃÍæ¼Òµ¥¾ÖÓÎÏ·µÃ·Ö
			int nPlayerNum = m_GameAccess->GetMaxChairCount();				
			for (BYTE i = 0; i < nPlayerNum && m_GameAccess != NULL; ++i)
			{
				if ( USER_PLAYING == m_GameAccess->GetPlayerState(i) )
				{
					//Íæ¼Òµ¥¾ÖµÃ·Ö
					StatusXJEnd.nSingleGameScore[i] = m_GameAccess->GetPlayerSingleScore(i);	

					printf("\nÓ®¼Ò£º%d£¬µ¥¾ÖµÃ·Ö = %lld\n",i, StatusXJEnd.nSingleGameScore[i]);
				}		
			}

			return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusXJEnd, sizeof(STR_CMD_SC_STATUS_XJ_END));
		}
	default:break;
	}

	return false;
}

// ¶¨Ê±Æ÷ÊÂ¼ş
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	//Ğ£Ñé
	if ( NULL ==  m_GameAccess)
		return false;

	//Á÷³Ì´¦Àí
	switch(wTimerID)
	{
		case IDI_ROB_BANKER: // ÇÀ×¯¶¨Ê±Æ÷
		{
			m_pITableFrame->KillGameTimer(IDI_ROB_BANKER);

			//×´Ì¬Ğ£Ñé
			if (GS_WK_ROB != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_ROB != m_GameAccess->GetGameStatus() : err £º" << endl;
				return false;
			}

			//Ã»ÓĞ½Ğ×¯,Ä¬ÈÏ²»½Ğ   Èç¹ûÊÇ»úÆ÷ÈË£¬Ëæ»ú½Ğ×¯
			for (WORD i = 0; (m_GameAccess != NULL) && (i < m_GameAccess->GetMaxChairCount()); i++)
			{
				cout << "¶¨Ê±Æ÷ÖĞµ±Ç°ÒÎ×ÓºÅ:" << (int)i << endl;
				cout << "¶¨Ê±Æ÷ÖĞ»ñÈ¡µ±Ç°ÇÀ×¯µÄÍæ¼Ò£º" << (int)m_GameAccess->GetCurRobUser() << endl;

				if ((USER_PLAYING == m_GameAccess->GetPlayerState(i))
					&& (i == m_GameAccess->GetCurRobUser()))
				{
					//Clog::InsertLog("¶¨Ê±Æ÷µ½£ºÄ¬ÈÏ²»ÇÀ×¯");

					WORD wNextUser = m_GameAccess->GetNextUser(i);

					if (wNextUser < 0 || wNextUser >= m_GameAccess->GetMaxChairCount())
					{
						//Clog::InsertLog("func ÇÀ×¯¶¨Ê±Æ÷ err : wChairID < 0 && wChairID >= m_GameAccess->GetMaxChairCount(): chairID:%d", wNextUser);
						return false;
					}

					while (i != wNextUser)		//ÅĞ¶ÏÊÇ²»ÇÀ»¹ÊÇ²»½Ğ
					{
						if (ROB_STATE_AGREE == m_GameAccess->GetBankerState(wNextUser) || ROB_STATE_AGREE == (m_GameAccess->GetBankerState(wNextUser) + 1))
						{
							//ÅĞ¶ÏÍæ¼ÒÊÇ»úÆ÷ÈËµÄÇé¿ö
							if (m_pITableFrame->IsRobot(i))
							{
								BYTE CurCardData[MAX_CARD_COUNT] = { 0 };
								BYTE CurCardNum = m_GameAccess->GetUserCurCardNum(i);

								m_GameAccess->GetClientHandCards(i, CurCardData, CurCardNum);

								//³õÊ¼»¯»úÆ÷ÈËÊÖÅÆÊı¾İ
								m_HandCardData[i].Init(CurCardData, CurCardNum);

								//¸ù¾İÊÖÅÆÅĞ¶ÏÇÀ²»ÇÀµØÖ÷
								int flag = m_AILogic.LandScore(m_HandCardData[i]);

								if (flag > 1)
								{
									OnUserCallBanker(i, 1, 1);	//²»ÇÀ»òÇÀµØÖ÷
								}
								else
								{
									OnUserCallBanker(i, 1, 0);	//²»ÇÀ»òÇÀµØÖ÷
								}

								break;
							}
							else
							{
								OnUserCallBanker(i, 1, 0);	//²»ÇÀ
								break;
							}
						}

						wNextUser = m_GameAccess->GetNextUser(wNextUser);
					}

					if (i == wNextUser)   //²»½Ğ
					{
						//ÅĞ¶ÏÍæ¼ÒÊÇ»úÆ÷ÈËµÄÇé¿ö
						if (m_pITableFrame->IsRobot(i))
						{
							BYTE CurCardData[MAX_CARD_COUNT] = { 0 };
							BYTE CurCardNum = m_GameAccess->GetUserCurCardNum(i);

							m_GameAccess->GetClientHandCards(i, CurCardData, CurCardNum);

							//³õÊ¼»¯»úÆ÷ÈËÊÖÅÆÊı¾İ
							m_HandCardData[i].Init(CurCardData, CurCardNum);

							//¸ù¾İÊÖÅÆÅĞ¶ÏÇÀ²»ÇÀµØÖ÷
							int flag = m_AILogic.LandScore(m_HandCardData[i]);

							if (flag > 1)
							{
								OnUserCallBanker(i, 0, 1);	//²»ÇÀ»òÇÀµØÖ÷
							}
							else
							{
								OnUserCallBanker(i, 0, 0);	//²»ÇÀ»òÇÀµØÖ÷
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
		case IDI_XJGAME_USER_READY:  // Ğ¡¾Ö½áÊøÓÃ»§×¼±¸¶¨Ê±Æ÷
		{
			m_pITableFrame->KillGameTimer(IDI_XJGAME_USER_READY);
			printf("\n¡¾·şÎñÆ÷¡¿£º¶¨Ê±Æ÷µ½£¬½øÈë¡¾Ğ¡¾Ö¿ªÊ¼¡¿\n");
			//Ã»ÓĞ×¼±¸£¬ÔòÄ¬ÈÏÈ«²¿×¼±¸£¬¿ªÊ¼ÓÎÏ·
			for (WORD i = 0; i < m_GameAccess->GetMaxChairCount() && m_GameAccess != NULL; i++)
			{
				if ( USER_PLAYING == m_GameAccess->GetPlayerState(i) 
					&& (0 == m_GameAccess->GetXjReadyState(i)) )
				{
					printf("\n¡¾·şÎñÆ÷¡¿£º¶¨Ê±Æ÷µ½£¬½ÓÊÜ¡¾Ğ¡¾Ö¿ªÊ¼¡¿ Íæ¼Ò£º%d\n", i);
					//ÓÃ»§×¼±¸
					OnUserXjGameReady(i, 1);
				}
			}
			break;
		}
		case IDI_ROUND_END_DELAY:	//Ò»ÂÖ½áÊøÑÓ³Ù¶¨Ê±Æ÷
		{
			m_pITableFrame->KillGameTimer(IDI_ROUND_END_DELAY);

			//ÑÓ³Ù¸øÍæ¼Ò·¢ËÍÒ»ÂÖ½áÊø
			WORD cbOutCardNum = (WORD)wBindParam;

			//Ò»ÂÖ½áÊø£¬Í¨ÖªÓ®¼Ò³öÅÆ¿ªÊ¼
			HandleRoundEnd(cbOutCardNum);
			break;
		}
		case IDI_ADD_SCORE:		//ÏÂ×¢¶¨Ê±Æ÷
		{
			m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);

			//×´Ì¬Ğ£Ñé
			if (GS_WK_ADDSCORE != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_ADDSCORE != m_GameAccess->GetGameStatus() : err £º" << endl;
				return false;
			}

			//Ã»ÓĞÏÂ×¢  Ä¬ÈÏ²»ÏÂ 
			for (WORD i = 0; i < m_GameAccess->GetMaxChairCount() && m_GameAccess != NULL; i++)
			{
				if (USER_PLAYING == m_GameAccess->GetPlayerState(i)
					/*&& (0 == m_GameAccess->GetXjReadyState(i))*/
					&& (0 == m_GameAccess->GetAddScoreState(i)))
				{
					printf("¶¨Ê±Æ÷µ½£¬Ä¬ÈÏ²»ÏÂ×¢\n");
					//Clog::InsertLog("¶¨Ê±Æ÷µ½£ºÄ¬ÈÏ²»ÏÂ×¢ %d\n", i);

					BYTE CurCardData[MAX_CARD_COUNT] = { 0 };
					BYTE CurCardNum = m_GameAccess->GetUserCurCardNum(i);

					m_GameAccess->GetClientHandCards(i, CurCardData, CurCardNum);

					//»úÆ÷ÈË¸ù¾İÊÖÅÆÏÂ×¢
					//³õÊ¼»¯»úÆ÷ÈËÊÖÅÆÊı¾İ
					m_HandCardData[i].Init(CurCardData, CurCardNum);

					//¸ù¾İÊÖÅÆÅĞ¶ÏÏÂ²»ÏÂ×¢
					int flag = m_AILogic.LandScore(m_HandCardData[i]);

					if (m_pITableFrame->IsRobot(i) && 3 == flag)
						OnUserAddScore(i, 2);
					else
						OnUserAddScore(i, 1);
				}

			}

			break;
		}
		case IDI_MING_PAI:		//Ã÷ÅÆ¶¨Ê±Æ÷
		{
			m_pITableFrame->KillGameTimer(IDI_MING_PAI);

			//Ã»ÓĞÃ÷ÅÆ Ä¬ÈÏ²»Ã÷ÅÆ    »úÆ÷ÈËÒ²Ä¬ÈÏ²»Ã÷ÅÆ
			for (WORD i = 0; i < m_GameAccess->GetMaxChairCount() && m_GameAccess != NULL; i++)
			{
				if (USER_PLAYING == m_GameAccess->GetPlayerState(i)
					/*&& (0 == m_GameAccess->GetXjReadyState(i))*/
					&& (0 == m_GameAccess->GetMingPaiState(i)))
				{
					printf("¶¨Ê±Æ÷µ½£¬Ä¬ÈÏ²»Ã÷ÅÆ\n");
					//Clog::InsertLog("¶¨Ê±Æ÷µ½£ºÄ¬ÈÏ²»Ã÷ÅÆ %d\n", i);

					//´¦Àí²»Ã÷ÅÆÏûÏ¢
					On_Sub_UserMingPai(i, 0, 0);
				}
			}

			break;
		}
		case IDI_OUT_CARD:		//³öÅÆ¶¨Ê±Æ÷
		{
			m_pITableFrame->KillGameTimer(IDI_OUT_CARD);

			WORD OutCardUser = m_GameAccess->GetCurOutCardUser();
			if (OutCardUser == INVALID_CHAIR)
			{
				return false;
			}

			//×´Ì¬Ğ£Ñé
			if (GS_WK_OUT_CARD != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_OUT_CARD != m_GameAccess->GetGameStatus() : err £ºÍĞ¹Ü" << endl;
				return false;
			}

			//Èç¹ûÍæ¼ÒÁ¬ĞøÈı´Î¶¨Ê±Æ÷³¬Ê± ÄÇÃ´½«ËûÉèÖÃ³ÉÍĞ¹Ü×´Ì¬
			if (!(m_pITableFrame->IsRobot(OutCardUser)))
			{
				WORD TimeOutNum = m_GameAccess->GetPlayerTimeOutNum(OutCardUser);
				m_GameAccess->SetPlayerTimeOutNum(OutCardUser, ++TimeOutNum);

				if (2 <= TimeOutNum)
				{
					//ÉèÖÃ¸ÃÍæ¼ÒÎªÍĞ¹Ü
					m_GameAccess->SetPlayerTuoGuan(OutCardUser, 1);

					//¸ø¿Í»§¶Ë·¢ËÍÍĞ¹ÜÏûÏ¢
					STR_CMD_SC_TUO_GUAN  tuoguan;
					ZeroMemory(&tuoguan, sizeof(STR_CMD_SC_TUO_GUAN));

					tuoguan.tuo_guan = 1;

					//µ¥·¢
					m_pITableFrame->SendTableData(OutCardUser, CMD_SC_TUO_GUAN, &tuoguan, sizeof(STR_CMD_SC_TUO_GUAN));

					//´«µİĞÅÓÃÖµ
					//m_pITableFrame->PlayerAutoOp(OutCardUser);
				}
			}



			//²»ÊÇµÚÒ»¸ö³öÅÆ  
			if (1 == m_GameAccess->GetOneTurnEnd())
			{
				BYTE OutCardData[MAX_CARD_COUNT];
				BYTE OutCardNum;

				ZeroMemory(&OutCardData, sizeof(OutCardData));

				//ÅĞ¶Ï»úÆ÷ÈËµÄÇé¿ö
				if (m_pITableFrame->IsRobot(OutCardUser))
				{
					BYTE CurCardData[MAX_CARD_COUNT] = { 0 };
					BYTE CurCardNum = m_GameAccess->GetUserCurCardNum(OutCardUser);

					m_GameAccess->GetClientHandCards(OutCardUser, CurCardData, CurCardNum);

					//³õÊ¼»¯»úÆ÷ÈËÊÖÅÆÊı¾İ
					m_HandCardData[OutCardUser].Init(CurCardData, CurCardNum);

					//³õÊ¼»¯µ±Ç°ÂÖÊÖÅÆ
					GameSituation m_GameSituation;
					m_GameLogic->GetTurnMaxCardToRobot(m_GameSituation, m_AILogic);

					//µ÷ÓÃµçÄÔÈË±»¶¯³öÅÆº¯Êı
					m_AILogic.get_PutCardList_2_limit(m_GameSituation, m_HandCardData[OutCardUser]);

					cout << "OutCard:";
					for (vector<int>::iterator it = m_HandCardData[OutCardUser].value_nPutCardList.begin(); it < m_HandCardData[OutCardUser].value_nPutCardList.end(); it++)
					{
						cout << ' ' << (int)*it;
					}
					cout << endl;

					//ÅĞ¶Ï³ö²»³öÅÆ
					if (m_HandCardData[OutCardUser].uctPutCardType.cgType == cgERROR || m_HandCardData[OutCardUser].uctPutCardType.cgType == cgZERO)
					{
						On_Sub_UserOutCard(OutCardUser, NULL, 0, 1);   //³öÅÆ¹ı
					}
					else
					{
						//½«Òª³öµÄÅÆ´ÓÂß¼­Öµ×ª»»³ÉÓĞ»¨É«
						m_GameLogic->GetCardColorValue(OutCardUser, m_HandCardData[OutCardUser], OutCardData, OutCardNum);

						On_Sub_UserOutCard(OutCardUser, OutCardData, OutCardNum, 0);     //³öÅÆ
					}
				}
				else
				{
					//ÅĞ¶Ï Íæ¼Ò¿ÉÒÔ³öµÄÅÆ
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
							cout << "ÅĞ¶Ï Íæ¼Ò¿ÉÒÔ³öµÄÅÆ err" << endl;
							//Clog::InsertLog("func ÅĞ¶Ï Íæ¼Ò¿ÉÒÔ³öµÄÅÆ err : Ğ£ÑéÊ§°Ü");
							return false;
						}
						On_Sub_UserOutCard(OutCardUser, OutCardData, OutCardNum, 0);
					}
					else
						On_Sub_UserOutCard(OutCardUser, NULL, 0, 1);   //³öÅÆ¹ı	
				}
			}
			//ÊÇµÚÒ»¸ö³öÅÆ  
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

					//³õÊ¼»¯»úÆ÷ÈËÊÖÅÆÊı¾İ
					m_HandCardData[OutCardUser].Init(CurCardData, CurCardNum);

					//µ÷ÓÃµçÄÔÈËÖ÷¶¯³öÅÆº¯Êı
					m_AILogic.get_PutCardList_2_unlimit(m_HandCardData[OutCardUser]);

					cout << endl << "OutCard:::::::";
					for (vector<int>::iterator it = m_HandCardData[OutCardUser].value_nPutCardList.begin(); it < m_HandCardData[OutCardUser].value_nPutCardList.end(); it++)
					{
						cout << ' ' << *it;
					}
					cout << endl << endl;

					//½«Òª³öµÄÅÆ´ÓÂß¼­Öµ×ª»»³ÉÓĞ»¨É«

					m_GameLogic->GetCardColorValue(OutCardUser, m_HandCardData[OutCardUser], OutCardData, OutCardNum);
					if (CT_ERROR == m_GameLogic->GetCardLogicType(OutCardData, OutCardNum))
					{
						//Clog::InsertLog("CT_ERROR == m_GameLogic->GetCardLogicType!!!!!!!!!\n");
						//Èç¹û»úÆ÷ÈË³öÅÆ³öÏÖ´íÎó
						BYTE UOutCardData[MAX_CARD_COUNT];
						BYTE UOutCardNum;
						ZeroMemory(&UOutCardData, sizeof(UOutCardData));
						//ÅĞ¶Ï µÚÒ»¸öÍæ¼Ò¿ÉÒÔ³öµÄÅÆ
						m_GameLogic->AnalysePlayerOutCardFirst(OutCardUser, UOutCardData, &UOutCardNum);

						//Ö´ĞĞ³öÅÆÏûÏ¢
						On_Sub_UserOutCard(OutCardUser, UOutCardData, UOutCardNum, 0);
					}
					else
					{
						//Ö´ĞĞ³öÅÆÏûÏ¢
						On_Sub_UserOutCard(OutCardUser, OutCardData, OutCardNum, 0);
					}
				}
				else

				{
					//ÅĞ¶Ï µÚÒ»¸öÍæ¼Ò¿ÉÒÔ³öµÄÅÆ
					m_GameLogic->AnalysePlayerOutCardFirst(OutCardUser, OutCardData, &OutCardNum);

					//Ö´ĞĞ³öÅÆÏûÏ¢
					On_Sub_UserOutCard(OutCardUser, OutCardData, OutCardNum, 0);
				}

			}

			break;
		}
		case IDI_ROB_JiaoFen:	//½Ğ·Ö¶¨Ê±Æ÷
		{
			m_pITableFrame->KillGameTimer(IDI_ROB_JiaoFen);

			//×´Ì¬Ğ£Ñé
			if (GS_WK_JIAO_FEN != m_GameAccess->GetGameStatus())
			{
				cout << "¶¨Ê±Æ÷£ºGS_WK_JIAO_FEN != m_GameAccess->GetGameStatus() : err" << endl;
				return false;
			}

			cout << "½Ğ·Ö¶¨Ê±Æ÷µ½" << endl;

			//Íæ¼ÒÄ¬ÈÏ²»½Ğ  »úÆ÷ÈËËæ»ú½Ğ·Ö
			for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
			{
				if (i == m_GameAccess->GetCurJiaoFenUser() && USER_PLAYING == m_GameAccess->GetPlayerState(i))
				{
					//»úÆ÷ÈËÇé¿ö
					if (m_pITableFrame->IsRobot(i))
					{
						WORD m_Score = JIAOFEN_START;
						for (int j = 0; j < m_GameAccess->GetCurPlayerCount(); j++)
						{
							if ((0 != m_GameAccess->GetCurJiaoFenState(j))
								&& (10 != m_GameAccess->GetCurJiaoFenState(j)))    //Ä¬ÈÏ×´Ì¬ºÍ²»½Ğ×´Ì¬
							{
								m_Score -= 1 << (m_GameAccess->GetCurJiaoFenState(j));
							}
						}

						if (JIAOFEN_START == m_Score)  //ÁùÖÖ½Ğ·ÖÇé¿ö
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

// ÓÎÏ·ÏûÏ¢´¦Àí
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//ÏûÏ¢´¦Àí
	switch (wSubCmdID)
	{
		case SUB_CS_OUT_CARD:		//³öÅÆ
		{
			//Ğ§ÑéÊı¾İ
			if (wDataSize != sizeof(STR_SUB_CS_OUT_CARD) )
				return true;

			//×´Ì¬Ğ£Ñé
			if (GS_WK_OUT_CARD != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_OUT_CARD != m_GameAccess->GetGameStatus() : err" << endl;
				return false;
			}

			//±äÁ¿¶¨Òå
			STR_SUB_CS_OUT_CARD *pOutCard= (STR_SUB_CS_OUT_CARD *)pData;

			//Èç¹ûÓÃ»§³öÅÆÁË Çå³ıËûµÄÁ¬Ğø³¬Ê±´ÎÊı
			m_GameAccess->SetPlayerTuoGuan(pOutCard->wOutCardUser, 0);

			if ( pOutCard->wOutCardUser == pIServerUserItem->GetChairID() )
			{
				bool bPass = (pOutCard->cbPass == 1) ? true : false;
				On_Sub_UserOutCard(pOutCard->wOutCardUser, pOutCard->cbOutCard, pOutCard->cbOutCardNum, bPass);
			}			
			break;	
		}
		case SUB_CS_ROB_BANKER:		 //ÇÀ×¯ÃüÁî
		{
			//Ğ§ÑéÊı¾İ
			if (wDataSize != sizeof(STR_SUB_CS_ROB_BANKER))
			{
				return true;//ÉáÆúÏûÏ¢ Ò²±íÊ¾´¦Àí
			}

			//×´Ì¬Ğ£Ñé
			if (GS_WK_ROB != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_ROB != m_GameAccess->GetGameStatus() : err" << endl;
				return false;
			}

			//±äÁ¿¶¨Òå
			STR_SUB_CS_ROB_BANKER *pCallBanker = (STR_SUB_CS_ROB_BANKER *)pData;
			OnUserCallBanker(pIServerUserItem->GetChairID(), pCallBanker->cbType, pCallBanker->cbRobState);

			break;
		}
		case SUB_CS_ADD_SCORE:	 //ÓÃ»§ÏÂ×¢
		{
			//Ğ£ÑéÊı¾İ
			if (wDataSize != sizeof(STR_SUB_CS_ADD_SCORE))
				return true;

			//×´Ì¬Ğ£Ñé
			if (GS_WK_ADDSCORE != m_GameAccess->GetGameStatus())
			{
				cout << "(GS_WK_ADDSCORE != m_GameAccess->GetGameStatus() : err" << endl;
				return false;
			}

			STR_SUB_CS_ADD_SCORE *pAddScore = (STR_SUB_CS_ADD_SCORE *)pData;

			//´¦Àí
			OnUserAddScore(pIServerUserItem->GetChairID(), pAddScore->score);

			break;
		}
		case SUB_CS_MING_PAI:	//Ã÷ÅÆ
		{
			//Ğ£Ñé
			if (1 != m_GameAccess->GetPlayerMingPaiBet(pIServerUserItem->GetChairID()))
			{
				printf("Íæ¼Ò£º%d ÊÇ·ñÒÑÃ÷ÅÆ£º%d\n", pIServerUserItem->GetChairID(), m_GameAccess->GetMingPaiState(pIServerUserItem->GetChairID()));
				cout << "Íæ¼ÒÒÑ¾­Ã÷¹ıÅÆÁË : err" << endl;
				return false;
			}

			//Ğ£ÑéÊı¾İ
			if (wDataSize != sizeof(STR_SUB_CS_MING_PAI))
				return true;

			STR_SUB_CS_MING_PAI *pMingPai = (STR_SUB_CS_MING_PAI *)pData;

			On_Sub_UserMingPai(pIServerUserItem->GetChairID(), pMingPai->cbIsMing, pMingPai->cbType, pMingPai->OutCard_bet);
		}
		case SUB_CS_XJ_GAME_READY:       //Íæ¼ÒĞ¡½áËã×¼±¸
		{
			//Ğ£ÑéÊı¾İ
			if (wDataSize != sizeof(STR_SUB_CS_XJ_USER_READY))
				return true;

			STR_SUB_CS_XJ_USER_READY *pXjReady = (STR_SUB_CS_XJ_USER_READY *)pData;

			//¼ÇÂ¼Íæ¼ÒÊÇ·ñ¿ªÊ¼ÓÎÏ·Ã÷ÅÆ
			printf("Íæ¼Ò£º%d ÊÇ·ñ¿ªÊ¼ÓÎÏ·Ã÷ÅÆ:%d\n", pXjReady->wReadyUser, pXjReady->cbBet);
			m_GameAccess->SetStartMingPai(pXjReady->wReadyUser, pXjReady->cbBet);


			OnUserXjGameReady(pXjReady->wReadyUser, pXjReady->cbReady);
			break;
		}
		case SUB_CS_PUBLIC_BET:		//ÓÎÏ·½áÊøµÄ¹«¹²±¶ÊıĞÅÏ¢
		{
			//Ğ£ÑéÊı¾İ
			if (wDataSize != sizeof(STR_SUB_CS_PUBLIC_BET))
				return true;

			//×´Ì¬Ğ£Ñé
			if (GS_WK_FREE == m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_FREE == m_GameAccess->GetGameStatus() : err" << endl;
				return false;
			}

			STR_SUB_CS_PUBLIC_BET *pPublicBet = (STR_SUB_CS_PUBLIC_BET *)pData;

			OnUserPublicBet(pIServerUserItem->GetChairID());

			break;
		}
		case SUB_CS_JIAO_FEN:		//½Ğ·Ö
		{
			//Ğ£ÑéÊı¾İ
			if (wDataSize != sizeof(STR_SUB_CS_JIAO_FEN))
				return true;

			//×´Ì¬Ğ£Ñé
			if (GS_WK_JIAO_FEN != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_JIAO_FEN != m_GameAccess->GetGameStatus() : err" << endl;
				return false;
			}

			STR_SUB_CS_JIAO_FEN *pJiaoFen = (STR_SUB_CS_JIAO_FEN *)pData;

			OnUserJiaoFen(pIServerUserItem->GetChairID(), pJiaoFen->Score);

			break;
		}
		case SUB_CS_JIPAIQI:		//¼ÇÅÆÆ÷
		{
			//Ğ£ÑéÊı¾İ
			if (wDataSize != sizeof(STR_SUB_CS_JIPAIQI))
				return true;

			//×´Ì¬Ğ£Ñé
			if (GS_WK_OUT_CARD != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_OUT_CARD != m_GameAccess->GetGameStatus() : err" << endl;
				return false;
			}

			STR_SUB_CS_JIPAIQI *pJiPaiQi = (STR_SUB_CS_JIPAIQI *)pData;

			OnUserJiPaiQi(pIServerUserItem->GetChairID());

			break;
		}
		case SUB_CS_TUO_GUAN:		//ÍĞ¹Ü
		{
			//Ğ£ÑéÊı¾İ
			if (wDataSize != sizeof(STR_SUB_CS_TUO_GUAN))
				return true;

			//×´Ì¬Ğ£Ñé
			if (GS_WK_OUT_CARD != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_OUT_CARD != m_GameAccess->GetGameStatus() : err £ºÍĞ¹Ü" << endl;
				return false;
			}

			STR_SUB_CS_TUO_GUAN *pTuoGuan = (STR_SUB_CS_TUO_GUAN *)pData;


			//ÉèÖÃ¸ÃÍæ¼ÒµÄÍĞ¹Ü×´Ì¬
			m_GameAccess->SetPlayerTuoGuan(pIServerUserItem->GetChairID(), pTuoGuan->TuoGuan_state);

			//ÉèÖÃ³öÅÆ¶¨Ê±Æ÷
			if (1 == pTuoGuan->TuoGuan_state)
			{
				m_pITableFrame->KillGameTimer(IDI_OUT_CARD);
				m_pITableFrame->SetGameTimer(IDI_OUT_CARD, IDI_TIME_OUT_CARD*0.05, 0, 0);
			}
			//»Ø¿Í»§¶ËÍĞ¹ÜÏûÏ¢
			STR_CMD_SC_TUO_GUAN  tuoguan;
			ZeroMemory(&tuoguan, sizeof(STR_CMD_SC_TUO_GUAN));

			tuoguan.tuo_guan = pTuoGuan->TuoGuan_state;

			//µ¥·¢
			m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), CMD_SC_TUO_GUAN, &tuoguan, sizeof(STR_CMD_SC_TUO_GUAN));
		}
		default:
			return false;
	}

	return true;
}

// ÓÃ»§×øÏÂ
bool CTableFrameSink::OnActionUserSitDown( WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser )
{
	if( bLookonUser == false )	
	{		
		m_dPlayerState[wChairID] = USER_SITDOWN;
	}

	return true;

}

// ÓÃ»§ÆğÁ¢
bool CTableFrameSink::OnActionUserStandUp( WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if( bLookonUser == false )
		m_dPlayerState[wChairID] = USER_STANDUP;

	return true;
}

// ÓÃ»§×¼±¸
bool CTableFrameSink::OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{ 
	//printf("\n¡¾ÓÃ»§ÓÎÏ·×´Ì¬ = %d¡¿\n", pIServerUserItem->GetUserStatus());
	//if ( pIServerUserItem->GetUserStatus() == USER_SITDOWN )
	if (m_dPlayerState[wChairID] == USER_SITDOWN)
	{
		//´Ó¿ò¼Ü»ñµÃ´ó¾ÖÓÎÏ·ÓÃ»§×´Ì¬
		m_dPlayerState[wChairID] = USER_ONREADY;
	}

	//ÉèÖÃÓÃ»§×¼±¸×´Ì¬£¬½«¿ò¼Ü´«À´µÄÓÃ»§×´Ì¬¸¶¸ø×ÓÓÎÏ·±äÁ¿
	int nReadyNum = 0;
	for (int i = 0; i < m_GameAccess->GetMaxChairCount(); i++)
	{
		if (USER_ONREADY == m_dPlayerState[i])
		{
			m_GameAccess->SetPlayerState(i, USER_ONREADY);
			nReadyNum++;
		}
	}

	//´óÓÚ×îĞ¡ÈËÊıÓÎÏ·¿ªÊ¼
	if (MIN_CHAIR_COUNT <= nReadyNum)
	{
		m_pITableFrame->StartGame();
	}

	return true; 
}

// ´ó¾ÖÓÎÏ·¿ªÊ¼				
bool CTableFrameSink::OnEventGameStart()
{
	// ´´½¨ÓÎÏ·£¬Ö¸Õë¸³Öµ -- ´Ë´¦µÄm_GameAccessÓëm_GameLogicÎªÍ¬Ò»¸ö¶ÔÏó£¬  Óëm_GameData²»ÊÇÍ¬Ò»¸ö¶ÔÏó
	m_GameData.CreateGame( &m_GameLogic, &m_GameAccess );

	// ³õÊ¼»¯·¿¼ä-ÉèÖÃ·¿¼ä¹æÔò
	if ( !m_GameAccess->SetRoomRule( m_pRoomRuleOption ))
		return false;

	// ¼ÓÔØ¶ÁÈ¡µÄÅäÖÃÎÄ¼ş²ÎÊıºÍ´´½¨ÈÕÖ¾,ÌúÁëÂé½«-¾­µäÄ£Ê½		
	if ( 0 != m_GameLogic->LoadConfigParameter(L"MJ_GAMES_LOG.txt", GAME_SCORE_MODE_CLASSIC) )
	{
		//Î´³É¹¦³õÊ¼»¯,ÍË³ö
		return false;
	}

	//ÉèÖÃÓÃ»§×¼±¸×´Ì¬£¬½«¿ò¼Ü´«À´µÄÓÃ»§×´Ì¬¸¶¸ø×ÓÓÎÏ·±äÁ¿
	int nReadyNum = 0;
	for (int i = 0; i < m_GameAccess->GetMaxChairCount(); i++)
	{
		if ( USER_ONREADY == m_dPlayerState[i] )
		{
			m_GameAccess->SetPlayerState(i, USER_ONREADY);
			nReadyNum++;
		}
	}
	
	//´óÓÚ×îĞ¡ÈËÊıÓÎÏ·¿ªÊ¼
	if ( MIN_CHAIR_COUNT <= nReadyNum )	
	{
		StartGame();
	}
	
	return true;
}

// ¿ªÊ¼ÓÎÏ·
void CTableFrameSink::StartGame()
{
	printf("\n¡¾¿ªÊ¼ÓÎÏ·¡¿\n");

	// ÉèÖÃÍæ¼Ò×´Ì¬¡¾ÓÎÏ·ÖĞ¡¿
	BYTE cbPlayerNum = m_GameAccess->GetMaxChairCount();
	for ( BYTE i = 0; i < cbPlayerNum; ++i)
	{	
		//´ó¾Ö½áÊøÊ±£¬ÓÃ»§×´Ì¬Îª×¼±¸£»Ğ¡¾ÖÓÎÏ·½áÊøÊ±£¬ÓÃ»§×´Ì¬ÎªÓÎÏ·ÖĞ¡£ËùÒÔÒªÓÃ>=
		if( USER_ONREADY <= m_GameAccess->GetPlayerState(i) )
		{
			m_GameAccess->SetPlayerState( i, USER_PLAYING );
		}
	}

	// ÓÎÏ·¾ÖÊı¼Ó1
	BYTE cbGameCout = m_GameAccess->GetCurGameCount();
	m_GameAccess->SetGameCount( ++cbGameCout );

	//¸øÍæ¼Ò·¢ÅÆ
	HandleDeal();

	//ĞÂ²ßÂÔ£º·¢ÅÆÊ±ºòÑ¡ÔñÊÇ·ñÃ÷ÅÆºÍ¿ªÊ¼ÓÎÏ·Ã÷ÅÆ
	//ËùÒÔÕâÀï²»ÇÀ×¯
}

//´¦Àí·¢ÅÆÁ÷³Ì
void CTableFrameSink::HandleDeal()
{
	// Ï´ÅÆ²¢·¢ÅÆ
	m_GameLogic->Deal();		//Ï´ÅÆËã·¨Ôö¼ÓÏ´ÅÆºÍ²»Ï´ÅÆÁ½ÖÖ²ßÂÔ

	// ¸øÍæ¼Ò±¶Êı³ËÉÏ´«À´µÄµ×·Ö  ÒÔºóµ÷ÊÔ¸Ä 

	//±éÀúÍæ¼Ò
	BYTE cbMaxChairCount = m_GameAccess->GetMaxChairCount();
	for (WORD i = 0; i < cbMaxChairCount; i++)
	{
		if ( USER_PLAYING == m_GameAccess->GetPlayerState(i) )
		{
			// ¹¹Ôì·¢ÅÆÊı¾İ
			STR_CMD_SC_SEND_CARD SendCard;
			ZeroMemory(&SendCard, sizeof(STR_CMD_SC_SEND_CARD));

			//¸³Öµ	
			SendCard.wChairID = i;	
			int _cardsum = m_GameAccess->GetPlayerInitCardNum();
			m_GameAccess->GetClientHandCards(i, SendCard.cbHandCardData, _cardsum);
			SendCard.cbHandCardNum = m_GameAccess->GetUserCurCardNum(i);

			//µ±Ç°¾ÖÊı£¬ÊÇ·ñÃ÷ÅÆ¿ªÊ¼£¬½ğ±Ò £¬»Ø·ÅÂë---ÖØÁ¬Ïà¹Ø

			// ·¢ÅÆ¸ø¶ÔÓ¦¿Í»§¶Ë -- Ã¿¸ö¿Í»§¶ËÓ¦¸ÃÖ»ÄÜÊÕµ½×Ô¼ºµÄÅÆ
			m_pITableFrame->SendTableData(i, CMD_SC_SEND_CARD, &SendCard, sizeof(STR_CMD_SC_SEND_CARD));
		}
	}

	//¶Ô·¢ÅÆÂ¼Ïñ recode

	// ¸ù¾İ¹æÔòÅĞ¶ÏÊÇ·ñÒªÃ÷ÅÆÕâ¸ö¹¦ÄÜ
	if (1 == m_GameAccess->GetMingPaiMode())
	{
		for (int i = 0; i < cbMaxChairCount; i++)
		{
			//ÅĞ¶ÏÍæ¼ÒÊÇ·ñ¿ªÊ¼ÓÎÏ·¾ÍÃ÷ÅÆ
			printf("Íæ¼Ò£º%d ÊÇ·ñ¿ªÊ¼ÓÎÏ·Ã÷ÅÆ:%d\n", i, m_GameAccess->GetStartMingPai(i));
			if (5 == m_GameAccess->GetStartMingPai(i))
			{
				On_Sub_UserMingPai(i, MING_PAI_TYPE_GAMESTART, 1);
			}
			else
			{
				SendMingPaiStart(i, MING_PAI_TYPE_DEAL);    //·¢ËÍÃ÷ÅÆ¿ªÊ¼ÏûÏ¢
			}
		}
	}
	//·ñÔòÖ±½Ó¿ªÊ¼ÇÀ×¯
	else
	{
		HandleRobBanker();
	}
}

//´¦ÀíÇÀ×¯
void CTableFrameSink::HandleRobBanker()
{
	//É¾³ıÃ÷ÅÆ¶¨Ê±Æ÷
	m_pITableFrame->KillGameTimer(IDI_MING_PAI);

	// ÉèÖÃÓÎÏ·¡¾ÁÁÖ÷×´Ì¬¡¿
	m_pITableFrame->SetGameStatus(GS_WK_ROB);

	//¸ù¾İ·¿¼ä¹æÔòÅĞ¶ÏÇÀ×¯Ä£Ê½
	BYTE cbRobBankMode = m_GameAccess->GetRobBankMode();   //´ıÌí¼Ó

	//¸øµÚÒ»ÂÖµÄÍæ¼Ò·¢ËÍ¼ÇÅÆÆ÷
	for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)			//´ıÌí¼Ó
	{
		OnUserJiPaiQi(i);
	}

	//·¢ËÍ·¿¼ä±¶ÊıĞÅÏ¢   //´ıÌí¼Ó

	//1¡¢½ĞÈı·Ö	
	if (ROOMRULE_OPTION_ROBTYPE_JIAOSANFEN == cbRobBankMode)
	{
		//½ĞÈı·Ö´¦ÀíÁ÷³Ì
		HandleJiaoFenRobBanker();
	}

	//2¡¢×ÔÓÉÇÀ×¯£º·¢ËÍ¡¾ÇÀ×¯¿ªÊ¼¡¿-´¦Àí¡¾ÇÀ×¯¡¿ÏûÏ¢
	if (ROOMRULE_OPTION_ROBTYPE_FREE == cbRobBankMode)       //²âÊÔ×ÔÓÉ×¯ºÍÂÖ×¯
	{
		//×ÔÓÉÇÀ×¯ÏûÏ¢´¦ÀíÁ÷³Ì
		HandleFreeRobBanker();
	}
}

//´¦Àí½ĞÈı·Ö
void CTableFrameSink::HandleJiaoFenRobBanker()
{
	//Ëæ»úÒ»¸öÈË³öÀ´  ½ĞÈı·Ö
	WORD wChairID = rand() % m_GameAccess->GetCurPlayerCount();

	//²»ÊÇµÚÒ»¾Ö ÈÃÉÏ¾ÖÓ®¼Ò½Ğ·Ö
	if (1 != m_GameAccess->GetCurGameCount())
	{
		wChairID = m_GameAccess->GetLastGameWinner();
	}

	//·¢ËÍ½ĞÈı·Ö¿ªÊ¼ÏûÏ¢
	SendJiaoFenStart(wChairID, JIAOFEN_START);
}	  

//·¢ËÍ½ĞÈı·Ö¿ªÊ¼ÏûÏ¢
void CTableFrameSink::SendJiaoFenStart(WORD wChairID, const BYTE &ActionType)
{
	//¹¹½¨ÏûÏ¢
	STR_CMD_SC_JIAOFEN_START  JiaofenStart;
	ZeroMemory(&JiaofenStart, sizeof(STR_CMD_SC_JIAOFEN_START));

	//¸³Öµ
	JiaofenStart.wChairID = wChairID;
	JiaofenStart.ActionType = ActionType;

	//ÉèÖÃµ±Ç°ÕıÔÚ½Ğ·ÖµÄÍæ¼Ò     
	m_GameAccess->SetCurJiaoFenUser(wChairID);

	if (INVALID_CHAIR != wChairID)
	{
		//·¢ËÍÏûÏ¢  ¹ã²¥
		m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_JIAOFEN_START, &JiaofenStart, sizeof(STR_CMD_SC_JIAOFEN_START));

		//ÉèÖÃÓÎÏ·×´Ì¬
		m_GameAccess->SetGameStatus(GS_WK_JIAO_FEN);

		//ÉèÖÃ½ĞÈı·Ö¶¨Ê±Æ÷  
		m_pITableFrame->KillGameTimer(IDI_ROB_JiaoFen);

		//ÅĞ¶Ï»úÆ÷ÈË
		if (m_pITableFrame->IsRobot(wChairID))     //´ıÌí¼Ó »úÆ÷ÈË½Ó¿Ú»¹Ã»Ìí¼Ó
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

//´¦Àí¿Í»§¶Ë·¢À´µÄ½ĞÈı·ÖÏûÏ¢
void CTableFrameSink::OnUserJiaoFen(WORD wChairID, BYTE Score)
{
	//×´Ì¬Ğ£Ñé
	if (GS_WK_JIAO_FEN != m_GameAccess->GetGameStatus())
	{
		cout << "½Ğ·Ö×´Ì¬Ğ£ÑéÊ§°Ü" << endl;
		return;
	}

	//É±ËÀ¶¨Ê±Æ÷
	m_pITableFrame->KillGameTimer(IDI_ROB_JiaoFen);

	//¹¹ÔìÏûÏ¢
	STR_CMD_SC_JIAOFEN_RESULT JiaofenResult;
	ZeroMemory(&JiaofenResult, sizeof(STR_CMD_SC_JIAOFEN_RESULT));

	//¸³Öµ
	JiaofenResult.wChairID = wChairID;
	JiaofenResult.Score = Score;

	for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
	{
		JiaofenResult.room_bet[i] = m_GameAccess->GetAllBet(i);
	}

	////Â¼Ïñ½Ğ·Ö½á¹û					//Â¼Ïñ»Ø·Å  ´ıÌí¼Ó
	//AddRecordJiaoSanFen(JiaofenResult);

	//¹ã²¥½Ğ·Ö½á¹û
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_JIAOFEN_RESULT, &JiaofenResult, sizeof(STR_CMD_SC_JIAOFEN_RESULT));

	//ÉèÖÃÒ»¸ö²ÎÊı¼ÇÂ¼½Ğ·Ö´ÎÊı 
	WORD JiaoNum = m_GameAccess->GetJiaoFenCount();
	m_GameAccess->SetJiaoFenCount(++JiaoNum);
	cout << "½Ğ·Ö´ÎÊı£º" << JiaoNum << endl;

	//¼ÇÂ¼µ±Ç°Íæ¼ÒµÄ½Ğ·Ö×´Ì¬
	m_GameAccess->SetCurJiaoFenState(wChairID, Score);

	//Èç¹ûÍæ¼ÒÖ±½Ó½ĞÈı·Ö  ÉèÖÃËûÎªµØÖ÷
	if (3 == Score)
	{
		//¸øÃ¿¸öÍæ¼Ò³ËÉÏ½Ğ·Ö±¶Êı
		for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
		{
			m_GameLogic->Wager(i, Score);
		}

		//Ö¸¶¨ËûÎª×¯¼Ò
		m_GameLogic->AppointBanker(wChairID);

		//½Ğ·ÖÍê³É ½øÈëÏÂ×¢×´Ì¬
		AllRobBankerOver();
	}
	else
	{
		//¼ÇÂ¼ÓĞ¶àÉÙ¸ö²»½ĞµÄÍæ¼Ò
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

		cout << "²»½ĞµÄÍæ¼ÒÊı£º" << (int)cbPassNum << endl;

		//Èç¹ûËùÓĞÈË²»½Ğ ÖØĞÂ·¢ÅÆ
		if (3 == cbPassNum)
		{
			//ÈıÂÖ¶¼²»½Ğ Ëæ»úÒ»¸öÍæ¼ÒµØÖ÷
			WORD TurnPassCount = m_GameAccess->GetTurnPassCount();
			m_GameAccess->SetTurnPassCount(++TurnPassCount);

			if (3 == TurnPassCount)
			{
				WORD banker = rand() % cbPassNum;
				//Ö¸¶¨ËûÎª×¯¼Ò
				m_GameLogic->AppointBanker(banker);

				AllRobBankerOver();

				return;
			}

			//Çå¿Õ¼ÇÂ¼µÄ½Ğ·Ö´ÎÊı
			m_GameAccess->SetJiaoFenCount(0);

			//Çå¿ÕÍæ¼ÒµÄ½Ğ·Ö×´Ì¬  Çå¿ÕÍæ¼ÒµÄÃ÷ÅÆ×´Ì¬  Çå¿ÕÃ÷ÅÆ±¶Êı
			for (int i = 0; i < cbMaxChairCount; i++)
			{
				m_GameAccess->SetCurJiaoFenState(i, 10);
				m_GameAccess->SetMingPaiState(i, 0);
				m_GameAccess->SetPlayerMingPaiBet(i, 1);
			}

			//Çå¿Õ±¶Êı
			m_GameAccess->CleanAllBet();

			//¸øÍæ¼Ò·¢ÅÆ
			HandleDeal();

			//sleep(4);

			//´¦ÀíÇÀ×¯£¨½Ğ·Ö£©
			HandleRobBanker();
		}

		else
		{
			//·ñÔòÅĞ¶Ï½Ğ·ÖÊÇ·ñÒÑ¾­½Ğ¹ıÁËÈı´Î ÓĞÈı´ÎÉèÖÃ½Ğ·Ö×î´óµÄÍæ¼ÒÎªµØÖ÷
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

				//¸øÃ¿¸öÍæ¼Ò³ËÉÏ×î´ó½Ğ·Ö±¶Êı
				for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
				{
					m_GameLogic->Wager(i, m_Score);
				}

				//Ö¸¶¨ËûÎª×¯¼Ò
				m_GameLogic->AppointBanker(Banker);

				AllRobBankerOver();
			}
			else		//·ñÔò ¸øÏÂ¸öÍæ¼Ò·¢ËÍ½Ğ·ÖĞÅÏ¢
			{
				WORD m_Score = JIAOFEN_START;    //1111
				for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
				{
					if ((USER_PLAYING == m_GameAccess->GetPlayerState(i)) &&
						(0 != m_GameAccess->GetCurJiaoFenState(i))
						&& (10 != m_GameAccess->GetCurJiaoFenState(i)))    //Ä¬ÈÏ×´Ì¬ºÍ²»½Ğ×´Ì¬
					{
						m_Score -= 1 << (m_GameAccess->GetCurJiaoFenState(i));
					}
				}

				cout << "´«µÄ¶ş½øÖÆ²ÎÊı£º" << m_Score << endl;

				//¸øÏÂ¸öÍæ¼Ò·¢ËÍ½Ğ·ÖÏûÏ¢
				SendJiaoFenStart(m_GameAccess->GetNextUser(wChairID), m_Score);
			}
		}


	}
}

//´¦Àí×ÔÓÉÇÀ×¯
void CTableFrameSink::HandleFreeRobBanker()
{
	//µÚÒ»¾Ö£º´Ó·¿Ö÷¿ªÊ¼·¢ËÍÇÀ×¯¿ªÊ¼ÏûÏ¢	µÚ¶ş¾Ö£º´ÓÉÏ¾ÖµÄÓ®¼Ò¿ªÊ¼·¢ËÍÇÀ×¯ÏûÏ¢

	//³õÊ¼»¯Ä¬ÈÏµÄÍæ¼ÒÇÀ×¯×´Ì¬
	for (WORD i = 0; i < m_GameAccess->GetMaxChairCount(); i++)   //10ÊÇÄ¬ÈÏ×´Ì¬ 
	{
		m_GameAccess->SetBankerState(i, 10, 10);
		//printf("Íæ¼ÒÇÀ×¯×´Ì¬£º%d\n", m_GameAccess->GetBankerState(i));
	}

	WORD wChairID = m_GameAccess->GetRoomFangzhu();
	if (1 != m_GameAccess->GetCurGameCount())
	{
		wChairID = m_GameAccess->GetLastGameWinner();
	}
	m_GameAccess->SetStartRobUser(wChairID);

	//·¢ËÍÇÀ×¯(½ĞµØÖ÷)¿ªÊ¼ÏûÏ¢
	SendRobStart(wChairID, ROB_TYPE_CALL);
}

//·¢ËÍÇÀ×¯¿ªÊ¼ÏûÏ¢
void CTableFrameSink::SendRobStart(const WORD &wChairID, const BYTE &cbType)
{
	//Í¨ÖªµÚÒ»¸öÍæ¼ÒÇÀ×¯¿ªÊ¼
	STR_CMD_SC_ROB_BANKER_START RobStart;
	ZeroMemory(&RobStart, sizeof(STR_CMD_SC_ROB_BANKER_START));

	//¸³Öµ
	RobStart.cbType = cbType;
	RobStart.wChairID = wChairID;

	//ÉèÖÃµ±Ç°ÕıÔÚÇÀ×¯µÄÍæ¼Ò
	m_GameAccess->SetCurRobUser(wChairID);
	printf("µ±Ç°ÕıÔÚÇÀ×¯µÄÍæ¼Ò£º%d\n", wChairID);

	if (INVALID_CHAIR != wChairID)
	{
		m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_ROB_START, &RobStart, sizeof(STR_CMD_SC_ROB_BANKER_START));

		//ÉèÖÃÓÎÏ·×´Ì¬Îª¡¾ÇÀ×¯×´Ì¬¡¿
		m_GameAccess->SetGameStatus(GS_WK_ROB);

		//ÇÀ×¯¶¨Ê±Æ÷
		m_pITableFrame->KillGameTimer(IDI_ROB_BANKER);

		//ÅĞ¶ÏÊÇ²»ÊÇ»úÆ÷ÈË
		if (m_pITableFrame->IsRobot(wChairID))     //»úÆ÷ÈË½Ó¿Ú´ıÌí¼Ó
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

// ´¦Àí¿Í»§¶Ë·¢À´µÄ¡¾ÇÀ×¯¡¿ÏûÏ¢
void CTableFrameSink::OnUserCallBanker(WORD wChairID, BYTE cbType, BYTE cbRobState)
{
	printf("\n¡¾·şÎñÆ÷¡¿£º½ÓÊÕÓÃ»§ = %d, ÇÀ×¯ÀàĞÍ = %d, ÇÀ×¯ = %d\n", wChairID, cbType, cbRobState);

	//Èç¹ûÇÀÉèÖÃ·¿¼ä±¶ÊıºÍÇÀ×¯±¶Êı
	if (1 == cbRobState)
	{
		m_GameAccess->SetCurRoomBet(2);
		for (int i = 0; i < m_GameAccess->GetMaxChairCount(); i++)
		{
			if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
			{
				m_GameLogic->Wager(i, 2);      //Èç¹ûÇÀµØÖ÷  ¸øÃ¿¸öÍæ¼Ò±¶Êı³Ë2
			}
		}
	}

	//¹Ø±Õ¶¨Ê±Æ÷
	m_pITableFrame->KillGameTimer(IDI_ROB_BANKER);

	//ÓÎÏ·×´Ì¬Ğ§Ñé
	if (GS_WK_ROB != m_GameAccess->GetGameStatus())
		return;

	//¹¹ÔìÇÀ×¯½á¹ûÊı¾İ
	STR_CMD_SC_ROB_RESULT sRobResult;
	ZeroMemory(&sRobResult, sizeof(STR_CMD_SC_ROB_RESULT));

	//¸³Öµ 
	sRobResult.wChairID  = wChairID;	
	sRobResult.cbType = cbType;			
	sRobResult.cbRobState = cbRobState;	

	for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
	{
		sRobResult.room_bet[i] = m_GameAccess->GetAllBet(i);
	}

	//ÇÀ×¯Â¼Ïñ  //´ıÌí¼Ó
	//AddRecordRobResult(sRobResult);

	//¹ã²¥ÇÀ×¯½á¹û
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_ROB_RESULT, &sRobResult, sizeof(STR_CMD_SC_ROB_RESULT));

	//ÉèÖÃÒ»¸ö²ÎÊı¼ÇÂ¼ÇÀ×¯´ÎÊı      ÎªÁËÅĞ¶Ï¹ı¹ıÇÀµØÖ÷µÄÇé¿ö
	BYTE robnum = m_GameAccess->GetCurBankerCount();
	m_GameAccess->SetCurBankerCount(++robnum);
	printf("¼ÇÂ¼ÇÀ×¯´ÎÊı:%d\n", robnum);

	//ÉèÖÃÍæ¼Òµ±Ç°ÇÀ×¯×´Ì¬
	m_GameAccess->SetBankerState(wChairID, cbType, cbRobState);

	//Íæ¼ÒÍ¬ÒâÇÀ£¬ÔòÉèÖÃÇÀ×¯´ÎÊı
	BYTE cbRobNum = m_GameAccess->GetUserRobNum(wChairID);
	if (ROB_STATE_AGREE == cbRobState + 1)
	{
		m_GameAccess->SetUserRobNum(wChairID, ++cbRobNum);
	}
	printf("Íæ¼ÒÇÀ×¯´ÎÊı %d\n", cbRobNum);

	//ÈôÍæ¼ÒÁ½´Î¶¼ÇÀ×¯£¬ÔòËûÎª×¯¼Ò
	if (2 == cbRobNum)
	{
		//Ö¸¶¨ËûÎª×¯¼Ò
		m_GameLogic->AppointBanker(wChairID);

		AllRobBankerOver();
	}
	else	//ÇÀ×¯»¹Î´Íê³É£¬Ôò¼ÌĞø¸øÏÂ¸öÍæ¼Ò·¢ËÍÇÀ×¯ÏûÏ¢
	{
		//ÅĞ¶ÏÊÇ²»ÊÇËùÓĞÍæ¼Ò¶¼Ñ¡Ôñ²»ÇÀ×¯
		BYTE cbMaxChairCount = m_GameAccess->GetMaxChairCount();
		BYTE cbPassNum = 0;
		for (int i = 0; i < cbMaxChairCount; i++)
		{
			if ((USER_PLAYING == m_GameAccess->GetPlayerState(i)) &&
				(ROB_STATE_PASS == (m_GameAccess->GetBankerState(i) + 1)))    //m_GameAccess->GetBankerState(i)µÄÖµ  0-¹ı	1-½ĞµØÖ÷2-ÇÀµØÖ÷ ROB_STATE_PASS Îª1
			{
				cbPassNum++;
			}
		}
		printf("²»ÇÀ×¯µÄÍæ¼ÒÊı:%d\n", cbPassNum);

		//ËùÓĞÈË·ÅÆúÇÀ×¯£¬ÔòÖØĞÂ·¢ÅÆ
		if (cbPassNum == cbMaxChairCount)
		{
			cout << "ËùÓĞÈË·ÅÆúÇÀ×¯" << endl;

			//ÈıÂÖ¶¼²»½Ğ Ëæ»úÒ»¸öÍæ¼ÒµØÖ÷
			WORD TurnPassCount = m_GameAccess->GetTurnPassCount();
			m_GameAccess->SetTurnPassCount(++TurnPassCount);

			if (3 == TurnPassCount)
			{
				WORD banker = rand() % cbPassNum;
				//Ö¸¶¨ËûÎª×¯¼Ò
				m_GameLogic->AppointBanker(banker);

				AllRobBankerOver();

				return;
			}

			//Çå¿ÕÊı¾İ
			//m_GameLogic->ClearXjGame();

			//Çå¿Õ¼ÇÂ¼µÄÇÀ×¯´ÎÊı
			m_GameAccess->SetCurBankerCount(0);

			//Çå¿ÕÍæ¼ÒµÄÃ÷ÅÆ×´Ì¬ //Çå¿ÕÍæ¼ÒµÄÃ÷ÅÆ±¶Êı
			for (int i = 0; i < cbMaxChairCount; i++)
			{
				m_GameAccess->SetMingPaiState(i, 0);
				m_GameAccess->SetPlayerMingPaiBet(i, 1);
			}



			//Çå¿Õ±¶Êı
			m_GameAccess->CleanAllBet();

			//¸øÍæ¼Ò·¢ÅÆ
			HandleDeal();

			//sleep(4);

			//´¦ÀíÇÀ×¯£¨ÇÀµØÖ÷£©
			HandleRobBanker();
		}
		else	//¸øÏÂ¸öÍæ¼Ò·¢ËÍÇÀ×¯ÏûÏ¢
		{
			//¸øÏÂÒ»¸öÍæ¼Ò·¢ËÍÇÀ×¯¿ªÊ¼ÏûÏ¢
			WORD wNextUser = m_GameAccess->GetNextUser(wChairID);
			WORD wLastUser = m_GameAccess->GetLastUser(wChairID);

			//ÅĞ¶ÏÏÂ¸öÍæ¼ÒµÄÇÀ×¯×´Ì¬
			while (wNextUser != wChairID)		//ÏÂ¸öÍæ¼Ò²»ÊÇ¡°¹ı¡±×´Ì¬²Å¸øËû·¢ËÍÇÀµØÖ÷ÏûÏ¢
			{
				if (robnum == 4)   //ÇÀµØÖ÷×î¶àÖ»ÄÜËÄ¸ö»ØºÏ
				{
					if (ROB_STATE_PASS != (m_GameAccess->GetBankerState(wChairID) + 1))    //µÚËÄ»ØºÏÇÀµØÖ÷¾ÍÊÇËûÎªµØÖ÷
					{
						//Ö¸¶¨ËûÎª×¯¼Ò
						m_GameLogic->AppointBanker(wChairID);

						AllRobBankerOver();
						break;
					}
					else				//ÉèÖÃ×îºóÇÀ×¯Íæ¼ÒÎª×¯
					{
						for (int i = (wChairID - cbMaxChairCount - 1); i < wChairID; i++)
						{
							WORD chairid = (i > 0) ? i % cbMaxChairCount : (-i) % cbMaxChairCount;
							if ((USER_PLAYING == m_GameAccess->GetPlayerState(chairid)) &&
								ROB_STATE_PASS != (m_GameAccess->GetBankerState(chairid) + 1))
							{
								//Ö¸¶¨ËûÎª×¯¼Ò
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
					&& (int)cbPassNum != 2)   //ºÍÉÏÃæÍ¬Àí 914
				{
					if ((int)cbPassNum == 1 && robnum == 1)
					{
						SendRobStart(wNextUser, ROB_TYPE_CALL);    //½ĞµØÖ÷ÀàĞÍ
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


			//Ç°Á½¸öÍæ¼Ò¶¼²»ÇÀ 
			if (wNextUser == wChairID)
			{
				//Ä¿Ç°ÓĞÁ½¸öÍæ¼Ò¶¼²»ÇÀ »ñÈ¡µ±Ç°ÇÀ×¯µÄÍæ¼Ò  ÉèÖÃËûÎªµØÖ÷
				BYTE cbMaxChairCount = m_GameAccess->GetMaxChairCount();
				WORD cbRobUser;
				for (int i = 0; i < cbMaxChairCount; i++)
				{
					if (ROB_STATE_PASS != (m_GameAccess->GetBankerState(i) + 1))    //m_GameAccess->GetBankerState(i)µÄÖµ  0-¹ı	1-½ĞµØÖ÷2-ÇÀµØÖ÷ ROB_STATE_PASS Îª1
					{
						cbRobUser = i;
					}
				}

				//Ö¸¶¨×¯¼Ò
				m_GameLogic->AppointBanker(cbRobUser);
				AllRobBankerOver();
			}
		}
	}
}

// È«²¿ÇÀ×¯Íê³É, ½øÈëÏÂ×¢×´Ì¬
void CTableFrameSink::AllRobBankerOver()
{
	//ÉèÖÃÓÎÏ·×´Ì¬Îª¡¾ÏÂ×¢×´Ì¬¡¿
	m_GameAccess->SetGameStatus(GS_WK_ADDSCORE);

	//¹Ø±ÕÇÀ×¯¶¨Ê±Æ÷
	m_pITableFrame->KillGameTimer(IDI_ROB_BANKER);

	//Çå¿ÕÇÀ×¯´ÎÊı
	m_GameAccess->SetCurBankerCount(0);

	//Ö¸¶¨×¯¼Ò
	WORD wBanker = m_GameLogic->GetAppointBanker();
	printf("»ñÈ¡µ±Ç°Ö¸¶¨µÄ×¯¼Ò£º%d", m_GameLogic->GetAppointBanker());

	//Ğ£Ñé ÕâÀï²»ÍË³ö£¬Ä¬ÈÏ·¿Ö÷Îª×¯¼Ò
	if (INVALID_CHAIR == wBanker /*|| !bappointresult*/)
	{
		wBanker = m_GameAccess->GetRoomFangzhu();
	}

	//¼ÇÂ¼Íæ¼ÒÇÀ×¯´ÎÊı  ´ó¾Ö½áËã
	m_GameAccess->SetPlayerBankCount(wBanker);

	//·¢ËÍÖ¸¶¨×¯¼ÒÏûÏ¢
	SendAppointBanker(wBanker);

	//·¢ËÍµ×ÅÆ¸ø×¯¼Ò
	HandleSendLeaveCard(wBanker);

	cout << "·¢ËÍµ×ÅÆover" << endl;

	//ÉèÖÃµÚÒ»ÂÖ×î¿ªÊ¼³öÅÆÓÃ»§
	m_GameAccess->SetStartOutCardUser(wBanker);

	//¸ù¾İ¹æÔòÊÇ·ñ·¢ËÍ¼Ó×¢¿ªÊ¼£¨¼Ó±¶£©ÏûÏ¢
	if (1 == m_GameAccess->GetAddBetMode())
	{
		SendAddScoreStart(INVALID_CHAIR);
	}
	//·ñÔòÖ±½Ó¿ªÊ¼³öÅÆ
	else
	{
		HandleOutCardStart(wBanker);
	}

	//·¢ËÍµ±Ç°·¿¼ä±¶Êı¸ø¸÷Íæ¼Ò						//¼ÇµÃ½Ğ¿Í»§¶Ë¼ÓÉÏÕâ¸öÏûÏ¢
	STR_CMD_SC_FANGJIAN_BET  fangjian_bet;
	ZeroMemory(&fangjian_bet, sizeof(STR_CMD_SC_FANGJIAN_BET));

	//¸øµØÖ÷¼ÓÉÏÅ©ÃñµÄ±¶Êı  ²¢¸øÅ©Ãñ¼ÓÉÏµØÖ÷µÄ±¶Êı
	for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
	{
		fangjian_bet.room_bet[i] = m_GameAccess->GetAllBet(i);
	}

	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_FANGJIAN_BET, &fangjian_bet, sizeof(STR_CMD_SC_FANGJIAN_BET));
}

//´¦Àí·¢ËÍµ×ÅÆ
void CTableFrameSink::HandleSendLeaveCard(const WORD &wSendCardUser)
{
	printf("\n¡¾¸øÍæ¼Ò = %d·¢ËÍµ×ÅÆ¡¿\n", wSendCardUser);

	//¸ù¾İ¹æÔòÅĞ¶ÏÊÇ·ñµ×ÅÆ¼Ó±¶
	if (1 == m_GameAccess->GetISLeaveCardDouble())
	{
		m_GameLogic->IsDoubleLeaveCard();
	}

	//ËùÓĞÈË¶¼²»ÄÜ·´Ö÷£¬Ôò½øÈëÏÂÒ»¸öÁ÷³Ì£¬¸ø×¯¼Ò·¢ËÍµ×ÅÆ
	m_GameLogic->SendLeaveCardToUser(wSendCardUser);

	//¸øÆäËûÍæ¼Ò·¢ËÍ×¯¼ÒÄÃµ½µ×ÅÆµÄÏûÏ¢
	BYTE cbMaxChairCount = m_GameAccess->GetMaxChairCount();

	//·¢ËÍµ±Ç°·¿¼ä±¶Êı¸ø¸÷Íæ¼Ò						
	STR_CMD_SC_FANGJIAN_BET  fangjian_bet;
	ZeroMemory(&fangjian_bet, sizeof(STR_CMD_SC_FANGJIAN_BET));

	//¸øµØÖ÷¼ÓÉÏÅ©ÃñµÄ±¶Êı  ²¢¸øÅ©Ãñ¼ÓÉÏµØÖ÷µÄ±¶Êı
	for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
	{
		fangjian_bet.room_bet[i] = m_GameAccess->GetAllBet(i);
	}

	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_FANGJIAN_BET, &fangjian_bet, sizeof(STR_CMD_SC_FANGJIAN_BET));

	//·¢ËÍµ×ÅÆ
	for (WORD i = 0; i < cbMaxChairCount; i++)
	{
		if ((USER_PLAYING == m_GameAccess->GetPlayerState(i)))
		{
			//»ñÈ¡µ×ÅÆ
			BYTE leavecards[MAX_LEAVE_CARD_NUM];
			m_GameAccess->GetLeaveCard(leavecards, MAX_LEAVE_CARD_NUM);

			//·¢ËÍµ×ÅÆÏûÏ¢
			STR_CMD_SC_SEND_LEAVE_CARD SendLeaveCard;
			ZeroMemory(&SendLeaveCard, sizeof(STR_CMD_SC_SEND_LEAVE_CARD));	

			//¸³Öµµ×ÅÆ
			cout << "´òÓ¡Òª·¢ËÍµÄµ×ÅÆ" << endl;
			for (int i = 0; i < MAX_LEAVE_CARD_NUM; i++)
			{
				SendLeaveCard.cbLeaveCard[i] = leavecards[i];
				printf("%d ", leavecards[i]);
			}
			cout << endl;

			BYTE tmpcards[MAX_CARD_COUNT];

			//¸³Öµ
			SendLeaveCard.cbMagicCard = m_GameAccess->GetMagicCard();	// Seeqings modify
			SendLeaveCard.wSendCardUser = i;

			//Íæ¼ÒÊÖÅÆÊı
			BYTE cbCardNum = m_GameAccess->GetUserCurCardNum(i);

			//¿¨ÅÆÅÅĞò
			SendLeaveCard.cbHandCardNum = cbCardNum;

			m_GameLogic->SortCardList(i, cbCardNum, ST_ORDER); 
			m_GameAccess->GetClientHandCards(i, tmpcards, MAX_CARD_COUNT);

			for (int j = 0; j < cbCardNum; j++)
			{
				SendLeaveCard.cbSortedCard[i] = tmpcards[j];
			}

			//Ö÷»¨É«¸öÊı
			SendLeaveCard.cbMagicCardNum = m_GameAccess->GetMagicCardNum(i);
			SendLeaveCard.LeaveCardBet = m_GameAccess->GetLeaveCardBet();

			//µ×ÅÆÂ¼Ïñ
			//if (i == m_GameLogic->GetAppointBanker())    //´ıÌí¼Ó
			//	AddRecordSendLeaveCard(SendLeaveCard);

			// ·¢ËÍÊı¾İ
			m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_USER_SEND_LEAVE_CARD, &SendLeaveCard, sizeof(STR_CMD_SC_SEND_LEAVE_CARD));
		}
	}
}

//·¢ËÍÏÂ×¢¿ªÊ¼ÏûÏ¢
void CTableFrameSink::SendAddScoreStart(const WORD &wChairID)
{
	//Ö¸¶¨×¯¼Òºó£¬·¢ËÍÏÂ×¢¿ªÊ¼ÏûÏ¢
	STR_CMD_SC_ADD_SCORE_START sAddScoreStart;
	ZeroMemory(&sAddScoreStart, sizeof(STR_CMD_SC_ADD_SCORE_START));

	//¸³Öµ		
	sAddScoreStart.cbStart = 1;

	printf("\n¡¾·şÎñÆ÷¡¿£º¹ã²¥¡¾ÏÂ×¢¿ªÊ¼¡¿\n");

	// ¹ã²¥ÏûÏ¢ ÓÃ»§ÏÂ×¢¿ªÊ¼
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_ADD_SCORE_START, &sAddScoreStart, sizeof(STR_CMD_SC_ADD_SCORE_START));
		
	//·¿¼ä×´Ì¬
	m_pITableFrame->SetGameStatus(GS_WK_ADDSCORE);

	// ÏÂ×¢¶¨Ê±Æ÷
	m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);

	//ÅĞ¶Ï»úÆ÷ÈË
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

// ÏÂ×¢¿ªÊ¼·µ»Ø
void CTableFrameSink::OnUserAddScore( WORD wChairID, SCORE lScore )
{
	printf("\n¡¾·şÎñÆ÷¡¿£º½ÓÊÕ¡¾ÏÂ×¢¡¿£¬Íæ¼Ò = %d£¬ÏÂ×¢ = %ld\n", wChairID, lScore);

	//ÓÎÏ·×´Ì¬Ğ§Ñé
	if (GS_WK_ADDSCORE != m_GameAccess->GetGameStatus())   //ÒÑÉè¶¨ÎªÏÂ×¢×´Ì¬
	{
		cout << "-1" << endl;
		return;
	}


	//ÓÃ»§×´Ì¬Ğ£Ñé
	if ((USER_PLAYING != m_GameAccess->GetPlayerState(wChairID))
		&& (1 != m_GameAccess->GetAddScoreState(wChairID)))
	{
		cout << "ÓÎÏ·Ğ§ÑéÊ§°Ü" << endl;
		return;
	}

	////É¾³ıÏÂ×¢¶¨Ê±Æ÷
	//m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);   //É¾µôÒ»´Î¾ÍÈ«É¾µôÁË  Òª·ÅÔÚºóÃæÉ¾

	//¿É¶à´Î¼Ó×¢, Òò´Ë´Ë´¦²»´¦Àí¼Ó×¢Íê³É±ê¼Ç
	m_GameLogic->Wager(wChairID, lScore);

	//¸øµØÖ÷¼ÓÉÏÅ©ÃñµÄ±¶Êı		//¸øÅ©Ãñ¼ÓÉÏµØÖ÷µÄ¼Ó±¶
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

	//¼ÇÂ¼Íæ¼ÒÏÂ×¢
	m_GameAccess->SetPlayerAddScore(wChairID, lScore);

	// ÉèÖÃÍæ¼ÒÏÂ×¢×´Ì¬
	m_GameAccess->SetAddScoreState(wChairID, 1);

	// ÉèÖÃµ±Ç°·¿¼ä±¶Êı
	m_GameAccess->SetCurRoomBet(lScore);

	//¹¹ÔìÊı¾İ
	STR_CMD_SC_ADD_SCORE_RESULT sAddScoreResult;
	ZeroMemory(&sAddScoreResult, sizeof(STR_CMD_SC_ADD_SCORE_RESULT)); // Seeqings modify

	//¸³Öµ
	sAddScoreResult.wChairID	 = wChairID;	
	sAddScoreResult.wAddSocre	 = lScore;		

	for (int i = 0; i < m_GameAccess->GetMaxChairCount(); i++)
	{
		if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
			sAddScoreResult.RoomBet[i] = m_GameAccess->GetAllBet(i);
	}

	//Â¼Ïñ¼Ó±¶½á¹û		//´ıÌí¼Ó
	//AddRecordAddScoreResult(sAddScoreResult);

	//¹ã²¥ÏÂ×¢½á¹û
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_ADD_SCORE_RESULT, &sAddScoreResult, sizeof(STR_CMD_SC_ADD_SCORE_RESULT));

	// ¼ÆËã¼Ó×¢Íê³ÉÍæ¼ÒÊı
	BYTE cbBetedNum = 0;
	BYTE cbPlayerNum = m_GameAccess->GetMaxChairCount();// »ñÈ¡Íæ¼ÒÊı
	for (BYTE i = 0; i < cbPlayerNum; ++i)
	{
		if (1 == m_GameAccess->GetAddScoreState(i))
			cbBetedNum++;
	}

	// ¼Ó×¢Íê³É£¬·¢ÅÆ
	printf("¼Ó×¢Íê³ÉÍæ¼ÒÊı£º%d\n", cbBetedNum);   //²âÊÔ
	if (cbBetedNum == m_GameAccess->GetCurPlayerCount())
	{
		//¸øµØÖ÷¼ÓÉÏÅ©ÃñµÄ±¶Êı
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

		//·¢ËÍ±¶Êı
		STR_CMD_SC_FANGJIAN_BET  fangjian_bet;
		ZeroMemory(&fangjian_bet, sizeof(STR_CMD_SC_FANGJIAN_BET));

		for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
		{
			fangjian_bet.room_bet[i] = m_GameAccess->GetAllBet(i);
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_FANGJIAN_BET, &fangjian_bet, sizeof(STR_CMD_SC_FANGJIAN_BET));


		//Í¨Öª×¯¼Ò³öÅÆ
		cout << "È«²¿ÏÂ×¢Íê³É ´¦Àí³öÅÆ¿ªÊ¼ÏûÏ¢" << endl;

		WORD wBanker = m_GameLogic->GetAppointBanker();

		//ÉèÖÃµÚÒ»ÂÖ×î¿ªÊ¼³öÅÆÓÃ»§
		m_GameAccess->SetStartOutCardUser(wBanker);

		//¸ø×¯¼Ò·¢ËÍÊÇ·ñÃ÷ÅÆµÄÃüÁî
		if (1 == m_GameAccess->GetPlayerMingPaiBet(wBanker) && 1 == m_GameAccess->GetMingPaiMode())
		{
			SendMingPaiStart(wBanker, MING_PAI_TYPE_OUTCARD);
		}

		//¸ø×¯¼Ò·¢ËÍ³öÅÆÏûÏ¢
		HandleOutCardStart(wBanker);
	}

	return;
}

//·¢ËÍÃ÷ÅÆ¿ªÊ¼ÏûÏ¢
void CTableFrameSink::SendMingPaiStart(const WORD &wChairID, const BYTE &cbMPType)
{
	//Ö¸¶¨×¯¼Òºó£¬·¢ËÍÏÂ×¢¿ªÊ¼ÏûÏ¢
	STR_CMD_SC_MING_PAI_START MingPaiStart;
	ZeroMemory(&MingPaiStart, sizeof(STR_CMD_SC_MING_PAI_START));

	//¸³Öµ		
	MingPaiStart.wChairID = wChairID;
	MingPaiStart.cbType = cbMPType;

	printf("\n¡¾·şÎñÆ÷¡¿£ºÍ¨ÖªÓÃ»§ = %d¡¾Ã÷ÅÆ¿ªÊ¼¡¿\n", wChairID);

	// Í¨ÖªÓÃ»§Ã÷ÅÆ¿ªÊ¼
	m_pITableFrame->SendTableData(wChairID, CMD_SC_MING_PAI_START, &MingPaiStart, sizeof(STR_CMD_SC_MING_PAI_START));

	// Ã÷ÅÆ¶¨Ê±Æ÷
	m_pITableFrame->KillGameTimer(IDI_MING_PAI);
	m_pITableFrame->SetGameTimer(IDI_MING_PAI, IDI_TIME_MING_PAI, 1, 0);
}

//Ã÷ÅÆÊÂ¼ş
void CTableFrameSink::On_Sub_UserMingPai(WORD wChairID, const BYTE &cbMPType, BYTE cbFlag, BYTE FlushCardBet)     //Ã÷ÅÆÀàĞÍ	2-¿ªÊ¼ÓÎÏ·Ã÷ÅÆ	1-³öÅÆ¿ªÊ¼Ã÷ÅÆ  3-·¢ÅÆÃ÷ÅÆ
{																												//ÊÇ·ñÃ÷ÅÆ	0-²»Ã÷ÅÆ	1-Ã÷ÅÆ    //·¢ÅÆÃ÷ÅÆµÄ±¶Êı  4 - 3 - 2
	//ÉèÖÃÍæ¼ÒÃ÷ÅÆÍê³É
	m_GameAccess->SetMingPaiState(wChairID, 1);

	//Íæ¼ÒÃ÷ÅÆ
	if (cbFlag == 1)
	{
		//¹¹ÔìÃ÷ÅÆÊı¾İ
		STR_CMD_SC_MING_PAI_RESULT MPResult;
		ZeroMemory(&MPResult, sizeof(STR_CMD_SC_MING_PAI_RESULT));	

		//Ã÷ÅÆ¼Ó±¶
		if (MING_PAI_TYPE_GAMESTART == cbMPType)	//¿ªÊ¼ÓÎÏ·Ã÷ÅÆ
		{
			//¼Ó±¶
			MPResult.cbTimes = 5;

			// ÉèÖÃµ±Ç°·¿¼ä±¶Êı
			m_GameAccess->SetCurRoomBet(5);

			//¼ÇÂ¼Íæ¼ÒÃ÷ÅÆ±¶Êı
			m_GameAccess->SetPlayerMingPaiBet(wChairID, 5);
		}
		else if (MING_PAI_TYPE_OUTCARD == cbMPType)   //³öÅÆÃûÅÆ
		{
			MPResult.cbTimes = 2;	

			// ÉèÖÃµ±Ç°·¿¼ä±¶Êı
			m_GameAccess->SetCurRoomBet(2);

			//¼ÇÂ¼Íæ¼ÒÃ÷ÅÆ±¶Êı
			m_GameAccess->SetPlayerMingPaiBet(wChairID, 2);
		}
		else if (MING_PAI_TYPE_DEAL == cbMPType)   //·¢ÅÆÃ÷ÅÆ
		{
			cout << "FlushCardBet:" << FlushCardBet << endl;

			MPResult.cbTimes = FlushCardBet;	

			// ÉèÖÃµ±Ç°·¿¼ä±¶Êı
			m_GameAccess->SetCurRoomBet(FlushCardBet);

			//¼ÇÂ¼Íæ¼ÒÃ÷ÅÆ±¶Êı
			m_GameAccess->SetPlayerMingPaiBet(wChairID, FlushCardBet);

			//¼ÇÂ¼Íæ¼ÒÃûÅÆÄ£Ê½
			m_GameAccess->SetStartMingPai(wChairID, 4);
		}

		//¸³Öµ
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

		//ÃûÅÆ½á¹ûÂ¼Ïñ			//´ıÌí¼Ó
		//AddRecordMingPaiResult(MPResult);

		//¹ã²¥Íæ¼ÒÃ÷ÅÆÏûÏ¢
		m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_MING_PAI_RESULT, &MPResult, sizeof(STR_CMD_SC_MING_PAI_RESULT));
	}

	//¼ÆËãÃ÷ÅÆÍê³ÉÍæ¼ÒÊı
	BYTE cbmingpaiNum = 0;
	BYTE cbPlayerNum = m_GameAccess->GetMaxChairCount();// »ñÈ¡Íæ¼ÒÊı
	for (BYTE i = 0; i < cbPlayerNum; ++i)
	{
		if (1 == m_GameAccess->GetMingPaiState(i))
			cbmingpaiNum++;
	}
	printf("Ã÷ÅÆÍê³ÉÍæ¼ÒÊı£º%d\n", (int)cbmingpaiNum);

	//Èç¹ûÈ«²¿Ã÷ÅÆÍê³É ·¢ËÍ³öÅÆ¿ªÊ¼

	if ((cbmingpaiNum == m_GameAccess->GetCurPlayerCount()))
	{

		if (MING_PAI_TYPE_OUTCARD == cbMPType)
		{
			//¸ø¸÷Íæ¼Ò¼ÓÉÏ×î´óµÄÃ÷ÅÆ±¶Êı
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
			cout << "È«²¿Ã÷ÅÆÍê³É ´¦ÀíÇÀ×¯¿ªÊ¼ÏûÏ¢" << endl;

			//¸ø¸÷Íæ¼Ò¼ÓÉÏ×î´óµÄÃ÷ÅÆ±¶Êı
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

			//´¦ÀíÇÀ×¯
			HandleRobBanker();
		}
	}
}

//´¦Àí³öÅÆ¿ªÊ¼ÏûÏ¢
void CTableFrameSink::HandleOutCardStart(const WORD &wOutCardUser)
{
	//É¾³ıÏÂ×¢¶¨Ê±Æ÷
	m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);

	//ÉèÖÃµ±Ç°³öÅÆÍæ¼Ò
	m_GameAccess->SetCurOutCardUser(wOutCardUser);

	//»ñÈ¡µ±Ç°ÂÖ×î´ó³öÅÆÊı¾İºÍÊıÄ¿
	BYTE TurnCardNum = 0;
	BYTE TurnCardData[MAX_CARD_COUNT];
	ZeroMemory(&TurnCardData, sizeof(TurnCardData));

	m_GameAccess->GetTurnMaxCards(TurnCardData, TurnCardNum);

	//¹ã²¥³öÅÆ¿ªÊ¼µÄÏûÏ¢
	STR_CMD_SC_OUT_CARD_START OutCardStart;
	ZeroMemory(&OutCardStart, sizeof(STR_CMD_SC_OUT_CARD_START));	

	OutCardStart.wOutCardUser = wOutCardUser;	

	//µ±Ç°ÂÖÅÆÊı¾İ¸³Öµ
	OutCardStart.TurnOutCardNum = TurnCardNum;
	for (int i = 0; i < TurnCardNum; i++)
	{
		OutCardStart.TurnOutCardData[i] = TurnCardData[i];
	}

	//ÅĞ¶Ïµ±Ç°³öÅÆ»ØºÏÊÇ·ñ½áÊø
	printf("ÅĞ¶Ïµ±Ç°»ØºÏÊÇ·ñ½áÊø: %d(0-½áÊø	1-Î´½áÊø)\n", m_GameAccess->GetOneTurnEnd());         //0001-³öÅÆ  0010-²»³ö  0100-ÌáÊ¾  1000-Òª²»Æğ
	if (0 == m_GameAccess->GetOneTurnEnd())
	{
		OutCardStart.ActionType = 1;  //µÚÒ»¸ö³öÅÆ   ÏÔÊ¾³öÅÆºÍÌáÊ¾°´Å¥         0101  
	}
	else
	{
		//ÅĞ¶ÏÊÇ²»ÊÇÒª²»Æğ
		if (m_GameLogic->JudgePlayerOutCard(wOutCardUser))
		{
			cout << "³öÅÆ" << endl;
			OutCardStart.ActionType = 1 + (1 << 1) + (1 << 2);    //0111
		}
		else
		{
			cout << "Òª²»Æğ" << endl;
			OutCardStart.ActionType = 1 << 3;    //1000
		}
	}

	//¹ã²¥³öÅÆ¿ªÊ¼ÏûÏ¢
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_USER_OUT_CARD_START, &OutCardStart, sizeof(STR_CMD_SC_OUT_CARD_START));

	//ÉèÖÃ³öÅÆÓÎÏ·×´Ì¬
	m_GameAccess->SetGameStatus(GS_WK_OUT_CARD);

	// ³öÅÆ¶¨Ê±Æ÷
	m_pITableFrame->KillGameTimer(IDI_OUT_CARD);
	//ÅĞ¶ÏÊÇ·ñÊÇ»úÆ÷ÈË
	if (m_pITableFrame->IsRobot(wOutCardUser))
	{
		WORD index = (rand() % 4) + 1;
		m_pITableFrame->SetGameTimer(IDI_OUT_CARD, IDI_TIME_OUT_CARD*0.05*index, 0, 0);
	}
	else
	{
		//ÅĞ¶ÏÊÇ·ñÍĞ¹Ü
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

//³öÅÆÊÂ¼ş
void CTableFrameSink::On_Sub_UserOutCard(WORD wOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum, const bool &bPass)
{
	//ÓÎÏ·×´Ì¬Ğ§Ñé
	if ( GS_WK_OUT_CARD != m_pITableFrame->GetGameStatus() ) 
		return;

	//Êı¾İĞ£Ñé
	if ((wOutCardUser != m_GameAccess->GetCurOutCardUser())||
		(MAX_CARD_COUNT < cbOutCardNum))
	{
		return;
	}

	//Íæ¼Ò¹ı
	if (bPass)
	{
		//´¦Àí¹ıÁ÷³Ì
		HandleOutCardPass(wOutCardUser);
	}
	else
	{
		//´¦Àí³öÅÆÁ÷³Ì
		HandleOutCard(wOutCardUser, cbOutCard, cbOutCardNum);
	}
}

//´¦ÀíÓÃ»§³öÅÆ¹ı
void CTableFrameSink::HandleOutCardPass(WORD wOutCardUser)
{
	//»ñµÃ¸ÃÍæ¼ÒµÄÏÂ¼Ò
	WORD wNextUser = m_GameAccess->GetNextUser(wOutCardUser);

	// É¾³ı³öÅÆ¶¨Ê±Æ÷
	m_pITableFrame->KillGameTimer(IDI_OUT_CARD);

	//ÉèÖÃÍæ¼ÒÎª³öÅÆ¹ı	
	m_GameAccess->SetTurnOutCardType(wOutCardUser, 0);

	//»ñµÃÒ»ÂÖ×îºó³öÅÆµÄÍæ¼Ò
	WORD wLastUser = m_GameAccess->GetLastOutCardUser();

	//Ğ£ÑéÊÇ²»ÊÇÂÖµ½¿ªÊ¼Íæ¼ÒÁË
	if ( wNextUser == wLastUser)
	{
		//ÏÈ·¢ËÍ³öÅÆ½á¹û°ü¸ø¿Í»§¶Ë
		SendOutCardResult(wOutCardUser, NULL, 0, 0, OUT_CARD_FAIL, OUT_CARD_PASS);

		//ÊÇ£¬ÔòÒ»ÂÖ½áÊø
		HandleRoundEnd(wNextUser);
	}
	else
	{
		//ÏÈ·¢ËÍ³öÅÆ½á¹û°ü¸ø¿Í»§¶Ë
		SendOutCardResult(wOutCardUser, NULL, 0, 0, OUT_CARD_FAIL, OUT_CARD_PASS);

		//·ñ£¬Í¨ÖªÏÂ¸öÍæ¼Ò³öÅÆ¿ªÊ¼
		HandleOutCardStart(wNextUser);
	}
}

//´¦ÀíÓÃ»§³öÅÆ
void CTableFrameSink::HandleOutCard(WORD wOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum)
{
	//ÈôÍæ¼ÒÊÇÒ»ÂÖÖĞ×îÏÈ¿ªÊ¼³öÅÆÕß
	WORD wStartUser = m_GameAccess->GetStartOutCardUser();

	//»ñµÃ´ËÂÖ×îºó³öÅÆÍæ¼Ò
	WORD wLastUser = m_GameAccess->GetLastOutCardUser();

	//ÅÆĞÍ
	int cbCardType = m_GameLogic->GetCardLogicType(cbOutCard, cbOutCardNum);    //Ê¹ÓÃBYTEµ¼ÖÂÔ½½ç

	//µÚÒ»¸ö³öÅÆÍæ¼Ò
	if ((TURN_END == m_GameAccess->GetOneTurnEnd()) &&
		(wOutCardUser == wStartUser))
	{
		cout << "µÚÒ»¸ö³öÅÆµÄÍæ¼Ò£¬ÅÆĞÍ£º" << (int)cbCardType << endl;
		//ÅÆĞÍĞ£Ñé
		if (CT_ERROR == cbCardType)		  //³öµÄ¿ÕÅÆ
		{
			cout << "µÚÒ»¸ö³öÅÆÍæ¼Ò³öÅÆ¸ñÊ½´íÎó:CT_ERROR == cbCardType" << endl;
			//·¢ËÍ³öÅÆ½á¹û
			SendOutCardResult(wOutCardUser, cbOutCard, cbOutCardNum, cbCardType, OUT_CARD_FAIL, OUT_CARD_NORMAL);  //³öÅÆÊ§°Ü

			return;
		}
		else
		{
			m_GameAccess->SetOneTurnEnd(TURN_NOT_END);
		}
	}
	else	//ºóÃæ³öÅÆÍæ¼Ò
	{
		cout << "ºóÃæ³öÅÆµÄÍæ¼Ò" << endl;
		//Ğ£Ñé³öÅÆÊıÄ¿ÊÇ·ñºÍ´ËÂÖ×îºó³öÅÆÍæ¼ÒÏàÍ¬£¬³öÅÆĞ£Ñé
		if ( /*(cbOutCardNum != m_GameAccess->GetUserOutCard(wLastUser, NULL)) || */
			!m_GameLogic->EfficacyOutCard(wOutCardUser, cbOutCard, cbOutCardNum))
		{
			cout << "ºóÃæ³öÅÆµÄÍæ¼ÒµÄÅÆĞÍ´íÎó:" << endl;
			//printf("µ±Ç°³öÅÆµÄÊıÄ¿£º%d£¬ÉÏ¸öÍæ¼Ò³öÅÆµÄÊıÄ¿£º%d\n", cbOutCardNum, m_GameAccess->GetUserOutCard(wLastUser, NULL));
			printf("³öÅÆĞ£Ñé·µ»ØÖµ£º%d\n", m_GameLogic->EfficacyOutCard(wOutCardUser, cbOutCard, cbOutCardNum));

			//·¢ËÍ³öÅÆ½á¹û
			SendOutCardResult(wOutCardUser, cbOutCard, cbOutCardNum, cbCardType, OUT_CARD_FAIL, OUT_CARD_NORMAL);

			return;
		}
	}

	// É¾³ı³öÅÆ¶¨Ê±Æ÷
	m_pITableFrame->KillGameTimer(IDI_OUT_CARD);

	//´ÓÊÖÅÆÖĞÉ¾³ıÍæ¼ÒÆË¿Ë
	if (!m_GameLogic->RemoveCard(wOutCardUser, cbOutCard, cbOutCardNum))
	{
		printf("´ÓÊÖÅÆÖĞÉ¾³ıÍæ¼ÒÆË¿ËÊ§°Ü\n");
		return;
	}

	//´ºÌìÅĞ¶ÏÌõ¼ş¼ÇÂ¼
	m_GameAccess->SetSpring(wOutCardUser, 1);

	//³öÅÆ¼ÇÂ¼
	m_GameAccess->SetUserOutCard(wOutCardUser, cbOutCard, cbOutCardNum);

	//¸ÃÂÖ³öÅÆ×î´óÊı¾İ¼ÇÂ¼
	m_GameAccess->SetTurnMaxCards(cbOutCard, cbOutCardNum);

	//ÉèÖÃÍæ¼ÒÎª³öÅÆ	
	m_GameAccess->SetTurnOutCardType(wOutCardUser, 1);

	//ÉèÖÃµ±Ç°³öÅÆÍæ¼ÒÎª´ËÂÖ×îºó³öÅÆÍæ¼Ò
	m_GameAccess->SetLastOutCardUser(wOutCardUser);

	//ÉèÖÃÏÂÒ»¸ö³öÅÆÍæ¼Ò
	WORD wNextUser = m_GameAccess->GetNextUser(wOutCardUser);

	//ÉèÖÃÍæ¼ÒÕ¨µ¯ĞÅÏ¢
	if ((cbCardType&(1 << CT_BOMB_CARD)) != 0 ||
		(cbCardType&(1 << CT_MISSILE_CARD)) != 0 ||
		(cbCardType&(1 << CT_LAIZI_BOMB)) != 0 /*||
		(cbCardType&(1 << CT_RUAN_BOMB)) != 0*/)
	{
		cout << "ÉèÖÃÕ¨µ¯ĞÅÏ¢" << endl;
		m_GameAccess->SetUserBoomInfo(wOutCardUser, cbCardType);
	}

	//·¢ËÍ³öÅÆ½á¹û
	SendOutCardResult(wOutCardUser, cbOutCard, cbOutCardNum, cbCardType, OUT_CARD_SUCCESS, OUT_CARD_NORMAL);

	//Ğ£ÑéÊÇ²»ÊÇÒ»ÂÖ³öÅÆÍê³É      //Ó¦¸Ã¼ì²âµ±Ç°Íæ¼ÒµÄÅÆÊÇ²»ÊÇ³öÍêÁË
//	if ( wNextUser == m_GameAccess->GetStartOutCardUser() )
	//printf("Íæ¼Òµ±Ç°ÊÖÅÆÊıÁ¿ : %d\n", m_GameAccess->GetUserCurCardNum(wOutCardUser));
	if (m_GameAccess->GetUserCurCardNum(wOutCardUser) == 0)
	{
		cout << "Ò»ÂÖ½áÊø" << endl;
		//Ò»ÂÖ½áÊø
		m_GameAccess->SetCurOutCardUser(INVALID_CHAIR);
		HandleRoundEnd(wOutCardUser);
	}
	else
	{
		cout << "Í¨ÖªÏÂ¸öÍæ¼Ò³öÅÆ" << endl;
		//Í¨ÖªÏÂ¸öÍæ¼Ò³öÅÆ¿ªÊ¼
		m_GameAccess->SetCurOutCardUser(wNextUser);
		HandleOutCardStart(wNextUser);
	}
}

//ÅĞ¶ÏÍæ¼ÒÊÇ·ñÄÜ³öÅÆ
bool CTableFrameSink::JudgeUserOutCard(const WORD &wLastOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum)
{
	bool bCanOut = false;

	//ÅĞ¶ÏÆäËûÍæ¼ÒÊÇ·ñÄÜ³öÅÆ
	WORD wNextUser = m_GameAccess->GetNextUser(wLastOutCardUser);

	//Ñ­»·±éÀúËùÓĞÍæ¼Ò,Ö±µ½¸ÃÍæ¼Ò
	while ( wNextUser != wLastOutCardUser )
	{
		//Íæ¼ÒÄÜ³öÅÆ
		tagOutCardNotify OutCardNotify[MAX_WEAVE_NUM];
		ZeroMemory(&OutCardNotify, MAX_WEAVE_NUM*sizeof(tagOutCardNotify));

		if ( m_GameLogic->AnalyseOutCardNotify(wNextUser, cbOutCard, cbOutCardNum, OutCardNotify) )
		{
			printf("\n¡¾Íæ¼Ò = %dÒªµÄÆğ£¬·¢ËÍ³öÅÆ¿ªÊ¼¡¿\n", wNextUser);

			//Í¨Öª¸ÃÍæ¼Ò³öÅÆ¿ªÊ¼
			m_GameAccess->SetCurOutCardUser(wNextUser);
			HandleOutCardStart(wNextUser);
			bCanOut = true;
			break;
		}
		else	//·¢ËÍ³öÅÆ½á¹û
		{
			printf("\n¡¾Íæ¼Ò = %dÒª²»Æğ£¬·¢ËÍ³öÅÆ½á¹û¡¿\n", wNextUser);
			SendOutCardResult(wNextUser, cbOutCard, cbOutCardNum, 0, OUT_CARD_FAIL, OUT_CARD_PASS);
		}

		wNextUser = m_GameAccess->GetNextUser(wNextUser);
	}
	
	return bCanOut;
}

//·¢ËÍ³öÅÆ½á¹û
void CTableFrameSink::SendOutCardResult(WORD wOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum, 
										 const BYTE &cbCardType, const BYTE &cbSuccess, const BYTE &cbFlag)
{
	//¹¹Ôì³öÅÆÊı¾İ
	STR_CMD_SC_OUT_CARD_RESULT OutCard;
	ZeroMemory(&OutCard, sizeof(STR_CMD_SC_OUT_CARD_RESULT));

	//»ñÈ¡µ±Ç°Íæ¼ÒÊÖÅÆÊıÁ¿ºÍÊÖÅÆÊı¾İ
	BYTE cbCurCardCount = m_GameAccess->GetUserCurCardNum(wOutCardUser);
	BYTE *cbCurCardData = new BYTE[cbCurCardCount]();
	m_GameAccess->GetClientHandCards(wOutCardUser, cbCurCardData, cbCurCardCount);

	//¸³Öµ
	OutCard.wOutCardUser = wOutCardUser;
	OutCard.cbFlag = cbFlag;
	OutCard.cbSuccess = cbSuccess;
	OutCard.cbHandCardNum = cbCurCardCount;

	for (int i = 0; i < m_GameAccess->GetMaxChairCount(); i++)
	{
		if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
		{
			OutCard.RoomBet[i] = m_GameAccess->GetAllBet(i);

			//°Ñ¼ÇÅÆÆ÷·¢¸ø¿Í»§¶Ë»º´æ
			OnUserJiPaiQi(i);
		}
	}

	//¸³Öµµ±Ç°ÊÖÅÆÊı¾İ
	for (int i = 0; i < cbCurCardCount; i++)
	{
		OutCard.handcarddata[i] = cbCurCardData[i];
	}


	//Õı³£³öÅÆ
	if (1 == cbFlag)
	{
		if (0 == cbSuccess)
		{
			m_pITableFrame->SendTableData(wOutCardUser, CMD_SC_USER_OUT_CARD_RESULT, &OutCard, sizeof(STR_CMD_SC_OUT_CARD_RESULT));
		}
		else
		{
			//¸³Öµ					
			OutCard.cbOutCardNum = cbOutCardNum;	
			memcpy(OutCard.cbOutCard, cbOutCard, sizeof(BYTE)*cbOutCardNum);	
			OutCard.cbHandCardNum = m_GameAccess->GetUserCurCardNum(wOutCardUser);	
			OutCard.cbCardType = cbCardType;	//ÅÆĞÍ

			//µÃ·Ö
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
	else	//Íæ¼ÒÒª²»Æğ
	{
		OutCard.cbFlag = cbFlag;	
	}

	//Â¼Ïñ³öÅÆ½á¹û
	//AddRecordOutCardResult(OutCard);

	//¹ã²¥³öÅÆ½á¹û
	m_pITableFrame->SendTableData(wOutCardUser, CMD_SC_USER_OUT_CARD_RESULT, &OutCard, sizeof(STR_CMD_SC_OUT_CARD_RESULT));
	printf("\n¡¾¹ã²¥Íæ¼Ò = %dµÄ³öÅÆ½á¹û¡¿\n", OutCard.wOutCardUser);	

	delete[] cbCurCardData;
}

//´¦ÀíÒ»ÂÖ³öÅÆ½áÊø
void CTableFrameSink::HandleRoundEnd(const WORD &wWinner)
{		
	//¼ÆËãÒ»ÂÖµÃ·Ö
	WORD wScore = m_GameLogic->SettleOneRound(wWinner);

	//ÉèÖÃÒ»ÂÖ½áÊø
	m_GameAccess->SetOneTurnEnd(TURN_END);

	//Çå¿ÕÍæ¼ÒÊÇ³öÅÆ»¹ÊÇ¹ı
	for (int i = 0; i < 3; i++)
	{
		m_GameAccess->SetTurnOutCardType(i, 99);
	}

	//ÉèÖÃ¿ªÊ¼µÄ³öÅÆÍæ¼Ò£¬Çå¿Õ¸ÃÂÖµÄ³öÅÆÊı¾İ
	if ( 0 != m_GameAccess->GetUserCurCardNum(wWinner) )
	{
		//ÉèÖÃ¸ÃÂÖ×î¿ªÊ¼³öÅÆÓÃ»§
		m_GameAccess->SetStartOutCardUser(wWinner);

		//ÉèÖÃµ±Ç°³öÅÆÓÃ»§
		m_GameAccess->SetCurOutCardUser(wWinner);

		//Í¨ÖªÓ®¼Ò³öÅÆ
		HandleOutCardStart(wWinner);

		printf("\n¡¾Íæ¼Ò»¹ÓĞÅÆ£¬Ò»ÂÖ½áÊø£¬Í¨ÖªÓ®¼Ò = %d³öÅÆ¿ªÊ¼¡¿\n", wWinner);
	}
	else	//Íæ¼ÒËùÓĞÅÆ³öÍê
	{
		printf("\n¡¾ÓÎÏ·½áÊø¡¿\n");

		//ÉèÖÃÓ®¼Ò
		m_GameAccess->SetLastGameWinner(wWinner);

		//¼ÇÂ¼Ó®¼Ò
		m_GameAccess->SetWinSum(wWinner, 1);

		m_GameAccess->SetCurOutCardUser(INVALID_CHAIR);

		//ÅĞ¶ÏÍæ¼ÒÊÇ·ñ´ºÌì
		m_GameAccess->JudgeSpring(wWinner);

		//Ëã·Ö
		m_GameLogic->Settle();

		//Ò»ÂÖ½áÊø£¬µ±Ç°Ó®¼ÒÅÆÈ«²¿³öÍê£¬ÓÎÏ·½áÊø
		OnEventGameConclude(INVALID_CHAIR, NULL, GER_NORMAL);
	}
}

//·¢ËÍÖ¸¶¨×¯¼ÒÏûÏ¢
void CTableFrameSink::SendAppointBanker(const WORD &wBanker)
{
	//ÓÃ»§Ğ£Ñé
	if ( wBanker >= m_GameAccess->GetMaxChairCount() )
		return;

	//¹¹ÔìÊı¾İ
	STR_CMD_SC_APPOINT_BANKER AppointBanker;
	ZeroMemory(&AppointBanker, sizeof(STR_CMD_SC_APPOINT_BANKER));

	//¸³Öµ
	AppointBanker.wBanker = wBanker;

	//Â¼ÏñÖ¸¶¨×¯¼Ò
	//AddRecordAppointBanker(AppointBanker);

	//·¢ËÍÊı¾İ
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_APPOINT_BANKER, &AppointBanker, sizeof(STR_CMD_SC_APPOINT_BANKER));
}

//Ğ¡¾Ö½áÊø-ÓÃ»§×¼±¸ÊÂ¼ş
void CTableFrameSink::OnUserXjGameReady(WORD wChairID, BYTE ready)
{
	printf("\n¡¾·şÎñÆ÷¡¿£º½ÓÊÜ¡¾Ğ¡¾Ö¿ªÊ¼¡¿\n");
	//ÓÎÏ·×´Ì¬Ğ§Ñé
	if ( GS_WK_XJ_GAMEEND != m_pITableFrame->GetGameStatus() )
	{
		printf("\n´íÎóÓÎÏ·×´Ì¬ = %d\n");
		return;
	}

	//ÓÃ»§×´Ì¬Ğ£Ñé
	if ( USER_PLAYING != m_GameAccess->GetPlayerState(wChairID) || 
		(1 == m_GameAccess->GetXjReadyState(wChairID)) )
	{
		printf("\n´íÎóÍæ¼Ò×´Ì¬ = %d\n");
		return;
	}

	// ÉèÖÃÓÎÏ·×¼±¸×´Ì¬
	m_GameAccess->SetXjReadyState(wChairID, ready);

	// ¼ÆËã×¼±¸ÈËÊı
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
		//É¾³ıĞ¡¾Ö½áÊø¶¨Ê±Æ÷
		m_pITableFrame->KillGameTimer(IDI_XJGAME_USER_READY);
		
		// È«Ìå×¼±¸Íê³É£¬¿ªÊ¼ÓÎÏ·
		StartGame();
	}
}

//ÓÎÏ·½áÊøµÄ»ñÈ¡¹«¹²±¶ÊıĞÅÏ¢
void CTableFrameSink::OnUserPublicBet(WORD wChairID)
{
	//ÒÎ×ÓºÅĞ£Ñé
	if (wChairID < 0 || wChairID >= m_GameAccess->GetMaxChairCount())
	{
		//Clog::InsertLog("func OnUserPublicBet err : wChairID < 0 && wChairID >= m_GameAccess->GetMaxChairCount(): chairID:%d", wChairID);
		return;
	}

	//¹¹ÔìĞÅÏ¢
	STR_CMD_SC_PUBLIC_BET public_bet;
	ZeroMemory(&public_bet, sizeof(STR_CMD_SC_PUBLIC_BET));

	WORD _playernum = m_GameAccess->GetCurPlayerCount();
	WORD LeaveCard_Bet = 1; //µ×ÅÆ±¶Êı
	WORD Spring_Bet = 1;	//´ºÌì±¶Êı
	WORD Mingpai_Bet = 1;  //Ã÷ÅÆ±¶Êı
	WORD Boom_Num = 0;   //Õ¨µ¯×ÜÊıÄ¿
	WORD Boom_Bet = 1;	 //Õ¨µ¯×Ü±¶Êı
	WORD Rob_Num = 0;    //ÇÀ×¯´ÎÊı
	WORD Rob_bet = 1;    //ÇÀ×¯±¶Êı
	WORD public_Bet = 0;  //¹«¹²±¶Êı
	tagBoomInfo  boominfo;  //Õ¨µ¯ĞÅÏ¢
	WORD Rule = m_GameAccess->GetRobBankMode();  //»ñÈ¡ÓÎÏ·¹æÔò
	cout << "¹æÔò£º" << Rule << endl;

	//¸³Öµ
	public_bet.RoomBet = 5;	//µ×·Ö
	public_bet.IUserAScore = m_GameAccess->GetAllBet(wChairID);   //×Ü·Ö
	public_bet.Game_State = m_GameAccess->GetGameStatus();		//ÓÎÏ·×´Ì¬

	//µ×ÅÆ±¶Êı
	LeaveCard_Bet = m_GameAccess->GetLeaveCardBet();
	public_bet.LeaveCard = LeaveCard_Bet;

	//´ºÌì±¶Êı
	for (int i = 0; i < _playernum; i++)
	{
		WORD flag = m_GameAccess->GetSpring(i);
		if (1 == flag)
		{
			Spring_Bet = 2;
		}
	}
	public_bet.Spring = Spring_Bet;


	if (m_GameLogic->GetAppointBanker() < 0 || m_GameLogic->GetAppointBanker() > m_GameAccess->GetMaxChairCount())	//ÅĞ¶ÏÊÇ·ñÇÀÍêÁËµØÖ÷
		public_bet.bank_ID = -1;    //×¯¼ÒID
	else
		public_bet.bank_ID = m_GameLogic->GetAppointBanker();    //×¯¼ÒID

	//¼Ó±¶
	for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
	{
		cout << "¸÷Íæ¼ÒµÄ¼Ó±¶ĞÅÏ¢" << (int)m_GameAccess->GetPlayerAddScore(i) << endl;
		public_bet.AddScore[i] = m_GameAccess->GetPlayerAddScore(i);    //¸÷Íæ¼ÒµÄ¼Ó±¶ĞÅÏ¢
		Boom_Num += m_GameAccess->GetUserBoomInfo(i, boominfo);			//»ñÈ¡Õ¨µ¯×ÜÊı
		Rob_Num += m_GameAccess->GetUserRobNum(i);						//»ñÈ¡×Ü¹²ÇÀ×¯´ÎÊı
	}

	//ÓÉ¹æÔòÀ´ÅĞ¶ÏÇÀ×¯²¿·ÖµÄ±¶Êı
	if (Rule == 0)
	{
		for (int i = 0; i < Rob_Num; i++)    //¼ÆËãÇÀ×¯µÄ±¶Êı
		{
			Rob_bet *= 2;    // 2µÄ4´Î·½±¶
		}
	}
	else		//½Ğ·Ö±¶ÊıËã×î´óµÄÄÇÒ»¸ö
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
	public_bet.Rob = Rob_bet;	//ÇÀµØÖ÷»ò½Ğ·Ö
	public_bet.Rule = Rule;

	//Ã÷ÅÆ±¶ÊıÈ¡×î´óµÄÄÇÒ»¸ö
	Mingpai_Bet = 0;
	for (int i = 0; i < _playernum; i++)
	{
		if (Mingpai_Bet < m_GameAccess->GetPlayerMingPaiBet(i))
		{
			Mingpai_Bet = m_GameAccess->GetPlayerMingPaiBet(i);
		}
	}

	public_bet.MingPai = Mingpai_Bet;

	//¹«¹²±¶Êı
	if (Boom_Num == 0)
		if (Mingpai_Bet == 0)
			public_Bet = 15 * Rob_bet * LeaveCard_Bet* Spring_Bet;  //¹«¹²±¶Êı = ³õÊ¼*Ã÷ÅÆ*ÇÀµØÖ÷*µ×ÅÆ*Õ¨µ¯*´ºÌì
		else
			public_Bet = 15 * Rob_bet * Mingpai_Bet * LeaveCard_Bet * Spring_Bet;
	else
		if (Mingpai_Bet == 0)
			public_Bet = 15 * Rob_bet  * Boom_Bet * LeaveCard_Bet * Spring_Bet;  //¹«¹²±¶Êı = ³õÊ¼*Ã÷ÅÆ*ÇÀµØÖ÷*µ×ÅÆ*Õ¨µ¯8´ºÌì
		else
			public_Bet = 15 * Rob_bet * Mingpai_Bet * Boom_Bet * LeaveCard_Bet * Spring_Bet;

	//ÅĞ¶ÏÊÇ·ñ·â¶¥
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

//´¦Àí¿Í»§¶Ë·¢À´µÄ¼ÇÅÆÆ÷ÏûÏ¢
void CTableFrameSink::OnUserJiPaiQi(WORD wChairID)
{
	//×´Ì¬Ğ£Ñé
	if (GS_WK_FREE == m_GameAccess->GetGameStatus())
	{
		cout << "OnUserJiPaiQi : err" << endl;
		return;
	}

	//¹¹½¨ÏûÏ¢
	STR_CMD_SC_JIPAIQI_START  JiPaiQi;
	ZeroMemory(&JiPaiQi, sizeof(STR_CMD_SC_JIPAIQI_START));

	//¶¨Òå±äÁ¿
	BYTE CardREC[15];
	tagCardRecorder	CardRec;
	ZeroMemory(&CardREC, sizeof(CardREC));
	ZeroMemory(&CardRec, sizeof(CardRec));

	//»ñµÃ¸÷Íæ¼ÒµÄ¿¨ÅÆÊıÄ¿
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
@Description:     ³õÊ¼»¯Â¼ÏñÊı×é£¬»ñµÃ¸öÊı¾İµÄË÷Òı¼°´óĞ¡
@Input:           ÎŞ
@Output:          ÎŞ
@Return:          ÎŞ
@author & data:	lizhihu 2017.11.2
*************************************************/
void CTableFrameSink::StartRecord()
{
}

/*************************************************
@Description:     Ğ´ÈëÂ¼ÏñÊı¾İ£¬³ıÁË¶¨Ê±Æ÷
				 ,ÔÚĞ¡¾Ö½áÊøºóµ÷ÓÃ£¬·ñÔòÓĞĞ©Êı¾İÎª¿Õ
@Input:           ÎŞ
@Output:          ÎŞ
@Return:          ÎŞ
@author & data:	lizhihu 2017.11.2
*************************************************/
void CTableFrameSink::WriteGameRecord()
{
	//»ñµÃÍæ¼Òµ¥¾ÖÓÎÏ·µÃ·Ö
	int _playersum = m_GameAccess->GetMaxChairCount();
	LONG *lGameScore = new LONG [_playersum];
	for (BYTE i = 0; i < _playersum; ++i)
	{
		if ( USER_PLAYING == m_GameAccess->GetPlayerState(i) )
		{
			lGameScore[i] = m_GameAccess->GetPlayerSingleScore(i);
		}	
	}
	
	//¹¹Ôìµ¥¾ÖµÃ·Ö×Ö·û´®
	CString str;
	str.Format(TEXT("%d"),lGameScore[0]);
	for(int i=1; i < _playersum; ++i)
	{
		TCHAR szTemp[10];
		_sntprintf(szTemp,10,TEXT(",%d"),lGameScore[i]);
		str.Append(szTemp);
	}

	//ÊÍ·Å
	delete [] lGameScore;
	lGameScore = NULL;

	//µ÷ÓÃ¿ò¼Ü·½·¨		//ÔİÊ±Ö»Ğ´Ğ¡¾ÖµÃ·Ö
	m_pITableFrame->WriteRecordInfo(m_GameAccess->GetCurGameCount(), str.GetBuffer(str.GetLength()), NULL, 0);
}

///*************************************************
//@Description:     ½áÊøÂ¼Ïñ£¬ÊÍ·ÅÄÚ´æ
//@Input:           ÎŞ
//@Output:          ÎŞ
//@Return: lD#ûXkQ?ävĞİ0Õ„9Œ®E?/@ ¿bözş™?øbj?““‹‰F?p_½ç´HK5?2?os+Ácn-?$? qÃúw?æxlšIÔú??ªS¯ŞbˆÏŠ£Ïõâ¡^bcVI™H“Â?Ç§"p8İÒÅÀÇ¿ªw?=Ó²ñ[±WºİˆáŒ‚2Va“êDÑÏ.ŒĞ°@??ÿ½Ú6¶‘pÅxüÅz
//`t?ŒoPT<~0M9¾÷
//g

////////////////////////////////////////////////////////////////////////////////////
//µ¼³ö¶¨Òå

extern "C" __declspec(dllexport) VOID * CreateTableFrameSink()
{
	CTableFrameSink *pTableFrameSink = NULL;
	try
	{
		pTableFrameSink = new CTableFrameSink();
		if (pTableFrameSink == NULL)
		{
			throw TEXT("´´½¨Ê§°Ü");
		}

		return pTableFrameSink;
	}
	catch (...) {}

	//ÇåÀí¶ÔÏó
	SafeDelete(pTableFrameSink);
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////