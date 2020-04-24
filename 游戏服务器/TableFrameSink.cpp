#include "StdAfx.h"
#include "TableFrameSink.h"
#include "SubGameRule.h"
#include "GameConfig.h"
#include "SubRuleManager.h"

// 构造函数
CTableFrameSink::CTableFrameSink()
{
	m_GameLogic = NULL;
	m_GameAccess = NULL;

	m_pITableFrame=NULL;

	m_pRoomRuleOption = new tagTableCfg;

}

// 析构函数
CTableFrameSink::~CTableFrameSink()
{
	//校验
	if (NULL  !=  m_GameLogic)
	{
		delete m_GameLogic;
		m_GameLogic  = NULL;
		m_GameAccess = NULL; 
	}

	m_pITableFrame = NULL;

	delete m_pRoomRuleOption;
	m_pRoomRuleOption = NULL;
}

// 初始化
bool CTableFrameSink::Initialization(ITableFrame *pTableFrame, tagTableRule *comRule)
{
	//查询接口
	if (pTableFrame == NULL)
	{
		return false;
	}

	m_pITableFrame = pTableFrame;
	//m_pITableFrame->SetStartMode(START_MODE_FULL_READY);	//所有人准备开始

	//规则配置
	m_pRoomRuleOption->com_rule = comRule;

	CopyMemory(&(m_pRoomRuleOption->sub_rule), &(CSubRuleManager::instance()->GetSubGameRule()), sizeof(tagSubGameRule));

	//校验
	if (NULL != m_GameLogic)
	{
		delete m_GameLogic;
		m_GameLogic  = NULL;
		m_GameAccess = NULL; 
	}

	// 创建游戏，指针赋值 -- 此处的m_GameAccess与m_GameLogic为同一个对象，  与m_GameData不是同一个对象
	m_GameData.CreateGame(&m_GameLogic, &m_GameAccess);
	if (m_GameLogic == NULL || m_GameAccess == NULL)
	{
		return false;
	}

	//给状态设初值
	m_GameAccess->SetGameStatus(GS_WK_FREE);

	// 初始化房间-设置房间规则
	if (!m_GameAccess->SetRoomRule(m_pRoomRuleOption))
		return false;

	// 加载读取的配置文件参数和创建日志,铁岭麻将-经典模式		
	if (0 != m_GameLogic->LoadConfigParameter(L"MJ_GAMES_LOG.txt", GAME_SCORE_MODE_CLASSIC))
	{
		//未成功初始化,退出
		return false;
	}
	
	return true;
}

// 小局游戏结束
bool CTableFrameSink::XjGameConclude(int nTotalGameCount, int nCurGameCount)
{
	CLog::Log(log_debug, "小局结束");

	// 玩家数目
	BYTE _playersum = m_GameAccess->GetMaxChairCount();

	// 计算玩家房间内的总得分
	for (BYTE i = 0; i < _playersum && m_GameAccess != NULL; ++i)
	{
		if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
		{
			m_GameAccess->SetPlayerTotalScore(i, m_GameAccess->GetPlayerSingleScore(i));
		}
	}

	//记录整大局中当前小局的得分是不是最大
	for (BYTE i = 0; i < _playersum && m_GameAccess != NULL; ++i)
	{
		SCORE CurBestBet = m_GameAccess->GetAllBet(i);
		if (m_GameAccess->GetDJPlayerBestBet(i) < CurBestBet)      //上一局的分比这一局小 就替换
		{
			m_GameAccess->SetDJPlayerBestBet(i, CurBestBet);
		}
	}


	// 构建数据
	SCORE msg_totalGameScore[MAX_CHAIR_COUNT] = {'\0'};
	SCORE msg_singleGameScore[MAX_CHAIR_COUNT] = {'\0'};
	WORD  msg_Identity[MAX_CHAIR_COUNT] = {'\0'};
	WORD  Winner = m_GameAccess->GetLastGameWinner();   //获取赢家
	WORD  BankID = m_GameLogic->GetAppointBanker();     //获取庄家 

	//获得玩家单局游戏得分
	for (BYTE i = 0; i < _playersum && m_GameAccess != NULL; ++i)
	{
		if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
		{
			//直接将玩家总积分发给客户端，避免客户端计算
			msg_totalGameScore[i] = m_GameAccess->GetPlayerTotalScore(i);
			//玩家单局得分
			msg_singleGameScore[i] = m_GameAccess->GetPlayerSingleScore(i);

			CLog::Log(log_debug, "chair: %d, singleGameScore: %d", i, msg_singleGameScore[i]);
		}
	}

	//获取玩家身份
	for (int i = 0; i < _playersum; i++)
	{
		if (i == BankID)
			msg_Identity[i] = 1;
		else
			msg_Identity[i] = 0;
	}

	//获取两个农民的加倍倍数和
	DWORD Addbet = 0;
	for (int i = 0; i < _playersum; i++)
	{
		if (i != BankID)
		{
			Addbet += m_GameAccess->GetPlayerAddScore(i);
		}
	}

	//通知frame处理
	BYTE cbCurGameCount = m_GameAccess->GetCurGameCount();

	//数据构造	三个玩家三份
	for (BYTE j = 0; j < _playersum && m_GameAccess != NULL; j++)
	{
		//构造数据
		STR_CMD_SC_XJ_GAME_END sXJGameEnd;
		ZeroMemory(&sXJGameEnd, sizeof(STR_CMD_SC_XJ_GAME_END));

		//sXJGameEnd.wchairID = j;
		if (Winner == BankID)
		{
			if (j == BankID)	sXJGameEnd.wIsWin = 1;
			else	sXJGameEnd.wIsWin = 0;
		}
		else
		{
			if (j == BankID)	sXJGameEnd.wIsWin = 0;
			else	sXJGameEnd.wIsWin = 1;
		}

		for (BYTE i = 0; i < _playersum && m_GameAccess != NULL; i++)
		{
			//获取玩家手牌数量
			BYTE TmpCardsCount = m_GameAccess->GetUserCurCardNum(i);

			//获取玩家当前的手牌
			m_GameAccess->GetClientHandCards(i, sXJGameEnd.cbLeaveHandCard[i], TmpCardsCount);

			//赋值
			sXJGameEnd.cbLeaveCardCount[i] = TmpCardsCount;
			sXJGameEnd.nSingleGameScore[i] = msg_singleGameScore[i];
			sXJGameEnd.nUserBet[i] = m_GameAccess->GetAllBet(i);

		}

		//循环发送信息
		m_pITableFrame->SendTableData(j, CMD_SC_XJ_GAME_END, &sXJGameEnd, sizeof(STR_CMD_SC_XJ_GAME_END));
	}

	//通知框架小局游戏结束
	m_pITableFrame->HandleXJGameEnd(cbCurGameCount, msg_Identity, msg_singleGameScore);

	//发送公共倍数信息
	for (int i = 0; i < _playersum; i++)
	{
		if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
		{
			OnUserPublicBet(i);
		}
	}

	m_GameLogic->ClearXjGame();

	//设置游戏状态为小局结束状态
	m_GameAccess->SetGameStatus(GS_WK_XJ_GAMEEND);    //以免被清空

	return true;
}

// 游戏结束
bool CTableFrameSink::OnEventGameConclude(BYTE cbReason)
{
	//关闭摊牌定时器
	m_pITableFrame->KillGameTimer(IDI_OUT_CARD);

	// 游戏当前局数
	WORD cbCurGameCount = m_GameAccess->GetCurGameCount();

	// 总局数
	WORD cbAllGameCount = m_GameAccess->GetAllCount();

	// 玩家数目
	BYTE cbPlayersum = m_GameAccess->GetMaxChairCount(); 

	switch (cbReason)
	{
	case GER_DISMISS:			// 解散房间
		{
			printf("\n解散房间\n");

			//结束游戏
			m_pITableFrame->HandleDJGameEnd(GAME_CONCLUDE_NORMAL);
		
			break;
		}
	case GER_NORMAL:			//正常结束
		{
			if (cbCurGameCount == cbAllGameCount || 2 == m_pITableFrame->GameType())		// 大局结束		//金币场判断这里加上
			{
				XjGameConclude(cbAllGameCount, cbCurGameCount);
				m_pITableFrame->HandleDJGameEnd(GAME_CONCLUDE_NORMAL);
			}
			// 小局结束
			else if (cbCurGameCount < cbAllGameCount) 
			{
				XjGameConclude(cbAllGameCount, cbCurGameCount);
				return true;
			}
				
			break;
		}
	default:
		break;
	}
	
	//设置游戏为【空闲】状态
	m_GameAccess->SetGameStatus(GS_WK_FREE);

	// 清理游戏数据
	m_GameLogic->ClearXjGame();

	// 清理大局数据
	m_GameLogic->ClearDjGame();

	//大局结束，清空内存 -- 如果 游戏结束标志位 GAME_CONCLUDE_CONTINUE, 这里就不能清空内存
	m_GameLogic->ClearRoom();
	
	return true;
}

// 断线重连时发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID)
{
	switch (m_GameAccess->GetGameStatus())
	{
		case GS_WK_FREE:		//空闲状态
		{
			//空闲状态
			STR_CMD_SC_STATUS_FREE StatusFree;
			ZeroMemory( &StatusFree, sizeof( StatusFree ) );

			// 设置变量
			//StatusFree.GameCount = m_pRoomRuleOption->com_rule->GameCount;
			//StatusFree.PlayerCount = m_pRoomRuleOption->com_rule->PlayerCount;
			//StatusFree.CellScore = m_pRoomRuleOption->sub_rule.Cellscore;
			//StatusFree.FangZhu = m_pRoomRuleOption->com_rule->FangZhu;

			// 发送场景
			m_pITableFrame->SendGameScene( wChairID, &StatusFree, sizeof(StatusFree));

			break;
		}
		case GS_WK_ROB:   //抢庄状态
		{
			if (INVALID_CHAIR == wChairID)
			{
				cout << "INVALID_CHAIR == wChairID" << endl;
				return false;
			}

			//校验
			if (m_GameAccess == NULL)
				return false;

			STR_CMD_SC_STATUS_ROB statusRob;
			ZeroMemory(&statusRob, sizeof(STR_CMD_SC_STATUS_ROB));

			int		nPlayerNum = m_GameAccess->GetCurPlayerCount();
			BYTE	UserCardNum[MAX_CHAIR_COUNT] = {'\0'};
			BYTE	wCardData[MAX_CARD_COUNT] = {'\0'};

			for (int i = 0; i < nPlayerNum; i++)
			{
				UserCardNum[i] = m_GameAccess->GetUserCurCardNum(i);
			}

			m_GameAccess->GetClientHandCards(wChairID, wCardData, UserCardNum[wChairID]);

			//赋值
			statusRob.CurRobUsr = m_GameAccess->GetCurRobUser();
			statusRob.CurJuShu = m_GameAccess->GetCurGameCount();
			//statusRob.replay_code = m_pITableFrame->GetRoomBaseInfo().replay_code;
			statusRob.room_bet = m_GameAccess->GetAllBet(wChairID);
			statusRob.GameStatue = m_GameAccess->GetGameStatus();

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
				statusRob.PlayerScore[i] = m_GameAccess->GetPlayerTotalScore(i);   //总分
			}

			//m_pITableFrame->SendTableData(wChairID, CMD_SC_STATUS_ROB, &statusRob, sizeof(STR_CMD_SC_STATUS_ROB));
			// 发送场景
			m_pITableFrame->SendGameScene( wChairID, &statusRob, sizeof(STR_CMD_SC_STATUS_ROB));

			break;
		}

		case GS_WK_ADDSCORE:	//下注状态
		{
			if (INVALID_CHAIR == wChairID)
			{
				cout << "INVALID_CHAIR == wChairID" << endl;
				return false;
			}

			STR_CMD_SC_STATUS_ADD_SCORE statusAddStore;
			ZeroMemory(&statusAddStore, sizeof(STR_CMD_SC_STATUS_ADD_SCORE));

			int		nPlayerNum = m_GameAccess->GetCurPlayerCount();
			BYTE	UserCardNum[MAX_CHAIR_COUNT] = {'\0'};
			BYTE	wCardData[MAX_CARD_COUNT] = {'\0'};

			for (int i = 0; i < nPlayerNum; i++)
			{
				UserCardNum[i] = m_GameAccess->GetUserCurCardNum(i);
			}

			m_GameAccess->GetClientHandCards(wChairID, wCardData, UserCardNum[wChairID]);

			//赋值
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
				statusAddStore.All_bet[i] = m_GameAccess->GetPlayerTotalScore(i);   //总分
			}

			//m_pITableFrame->SendTableData(wChairID, CMD_SC_STATUS_ADD_SCORE, &statusAddStore, sizeof(STR_CMD_SC_STATUS_ADD_SCORE));

			break;
		}

		case GS_WK_OUT_CARD:	//出牌状态
		{
			cout << "出牌状态断线重连" << endl;

			if (INVALID_CHAIR == wChairID)
			{
				cout << "INVALID_CHAIR == wChairID" << endl;
				return false;
			}

			//如果用户重连上了 清除他的连续超时次数
			m_GameAccess->SetPlayerTuoGuan(wChairID, 0);

			//构造数据
			STR_CMD_SC_STATUS_OUTCARD StatusOutCard;
			ZeroMemory(&StatusOutCard, sizeof(STR_CMD_SC_STATUS_OUTCARD)); 

			int nPlayerNum = m_GameAccess->GetCurPlayerCount();
			BYTE msg_cbCardData[MAX_CHAIR_COUNT][MAX_CARD_COUNT] = {'\0'};	//记录出牌
			BYTE msg_CardData[MAX_CHAIR_COUNT][MAX_CARD_COUNT] = {'\0'};		//记录名牌玩家的手牌
			BYTE msg_LeaveCardData[MAX_LEAVE_CARD_NUM] = {'\0'};   //底牌
			BYTE msg_TurnCardData[MAX_CARD_COUNT] = {'\0'};			//记录当前轮的最大出牌GetTurnMaxCards
			BYTE max_card_count = MAX_CARD_COUNT;

			//最大出牌数量
			ZeroMemory(&msg_TurnCardData, sizeof(msg_TurnCardData));
			m_GameAccess->GetTurnMaxCards(msg_TurnCardData, max_card_count);

			//底牌
			m_GameAccess->GetLeaveCard(msg_LeaveCardData, MAX_LEAVE_CARD_NUM);

			//记录名牌玩家的手牌
			for (int i = 0; i < nPlayerNum; i++)
			{
				if (m_GameAccess->GetPlayerMingPaiBet(i) > 1)
				{
					m_GameAccess->GetClientHandCards(i, msg_CardData[i], m_GameAccess->GetUserCurCardNum(i));
				}
			}

			//获取手牌
			for (int i = 0; i < nPlayerNum; i++)
			{
				if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
				{
					m_GameAccess->GetUserOutCard(i, msg_cbCardData[i]);
				}
			}

			//断线玩家的手牌
			m_GameAccess->GetClientHandCards(wChairID, StatusOutCard.brokenoutcarddata, m_GameAccess->GetUserCurCardNum(wChairID));

			for (int i = 0; i < nPlayerNum; i++)
			{
				auto player_op_info = StatusOutCard.players_op;

				//名牌状态
				if (m_GameAccess->GetPlayerMingPaiBet(i) > 1)
					player_op_info[i].op_isMingpai = 1;

				//设置各玩家的手牌数量
				m_GameAccess->GetClientHandCards(i, player_op_info[i].op_HandCardData, m_GameAccess->GetUserCurCardNum(i));
				player_op_info[i].op_cardscount = m_GameAccess->GetUserCurCardNum(i);

				//设置各玩家是否出牌的状态
				player_op_info[i].op_type = m_GameAccess->GetTurnOutCardType(i);
				player_op_info[i].op_outCardCount = m_GameAccess->GetUserOutCard(i, NULL);

				//设置名牌玩家的手牌
				/*for (int j = 0; j < m_GameAccess->GetUserCurCardNum(i); j++)
				{
					player_op_info[i].MingPaiCardData[j] = msg_CardData[i][j];
				}*/

				//设置各玩家出牌结果
				for (int j = 0; j < m_GameAccess->GetUserOutCard(i, NULL); j++)
				{
					player_op_info[i].op_result[j] = msg_cbCardData[i][j];
				}
			}

			//设置当前出牌的玩家
			StatusOutCard.outcardid = m_GameAccess->GetCurOutCardUser();
			StatusOutCard.Leave_bet = m_GameAccess->GetLeaveCardBet();  //底牌倍数

			StatusOutCard.CurJuShu = m_GameAccess->GetCurGameCount();   //游戏局数
			StatusOutCard.bankID = m_GameLogic->GetAppointBanker();   //庄家
			StatusOutCard.room_bet = m_GameAccess->GetAllBet(wChairID);   //房间倍数
			StatusOutCard.IsTurnEnd = m_GameAccess->GetOneTurnEnd();
			StatusOutCard.ActionType = m_GameLogic->GetOutCardActionType(wChairID);
			StatusOutCard.GameStatue = m_GameAccess->GetGameStatus();
			StatusOutCard.cbMagicCard = m_GameAccess->GetMagicCard();

			for (int i = 0; i < nPlayerNum; i++)
			{
				StatusOutCard.PlayerScore[i] = m_GameAccess->GetPlayerTotalScore(i);   //总分
				//StatusOutCard.Add_bet[i] = m_GameAccess->GetPlayerAddScore(i);				//玩家是否加倍
			}

			for (int i = 0; i < MAX_LEAVE_CARD_NUM; i++)
			{
				StatusOutCard.LeaveCard[i] = msg_LeaveCardData[i];  //底牌
			}

			//for (int i = 0; i < max_card_count; i++)
			//{
			//	StatusOutCard.TurnCardData[i] = msg_TurnCardData[i];  //当前轮最大卡牌
			//}

			// 发送场景
			m_pITableFrame->SendGameScene( wChairID, &StatusOutCard, sizeof(STR_CMD_SC_STATUS_OUTCARD));

			break;
		}
		case GS_WK_JIAO_FEN:   //叫分状态
		{
			if (INVALID_CHAIR == wChairID)
			{
				cout << "INVALID_CHAIR == wChairID" << endl;
				return false;
			}

			STR_CMD_SC_STATUS_JIAOFEN statusJiaofen;
			ZeroMemory(&statusJiaofen, sizeof(STR_CMD_SC_STATUS_JIAOFEN));

			int		nPlayerNum = m_GameAccess->GetMaxChairCount();
			BYTE	UserCardNum[MAX_CHAIR_COUNT] = {'\0'};
			BYTE	wCardData[MAX_CARD_COUNT] = {'\0'};

			for (int i = 0; i < nPlayerNum; i++)
			{
				UserCardNum[i] = m_GameAccess->GetUserCurCardNum(i);
			}

			m_GameAccess->GetClientHandCards(wChairID, wCardData, UserCardNum[wChairID]);

			//赋值
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
				statusJiaofen.All_bet[i] = m_GameAccess->GetPlayerTotalScore(i);   //总分
			}

			//m_pITableFrame->SendTableData(wChairID, CMD_SC_STATUS_JIAOFEN, &statusJiaofen, sizeof(STR_CMD_SC_STATUS_JIAOFEN));

			break;
		}
		case GS_WK_XJ_GAMEEND:
		{
			STR_CMD_SC_STATUS_XJ_END StatusXJEnd;
			ZeroMemory(&StatusXJEnd, sizeof(STR_CMD_SC_STATUS_XJ_END));

			//获得玩家单局游戏得分
			int nPlayerNum = m_GameAccess->GetMaxChairCount();				

			//sXJGameEnd.wchairID = j;
			if (m_GameAccess->GetLastGameWinner() == m_GameAccess->GetBankerID())
			{
				if (wChairID == m_GameAccess->GetBankerID())
					StatusXJEnd.XjGameEnd.wIsWin = 1;
				else
					StatusXJEnd.XjGameEnd.wIsWin = 0;
			}
			else
			{
				if (wChairID == m_GameAccess->GetBankerID())
					StatusXJEnd.XjGameEnd.wIsWin = 0;
				else
					StatusXJEnd.XjGameEnd.wIsWin = 1;
			}

			StatusXJEnd.Banker = m_GameAccess->GetBankerID();
			StatusXJEnd.GameCount = m_GameAccess->GetCurGameCount();
			m_GameAccess->GetLeaveCard(StatusXJEnd.LeaveCard, MAX_LEAVE_CARD_NUM);
			StatusXJEnd.GameStatue = m_GameAccess->GetGameStatus();

			for (BYTE i = 0; i < nPlayerNum && m_GameAccess != NULL; i++)
			{
				//获取玩家手牌数量
				BYTE TmpCardsCount = m_GameAccess->GetUserCurCardNum(i);

				//获取玩家当前的手牌
				m_GameAccess->GetClientHandCards(i, StatusXJEnd.XjGameEnd.cbLeaveHandCard[i], TmpCardsCount);

				//赋值
				StatusXJEnd.XjGameEnd.cbLeaveCardCount[i] = TmpCardsCount;
				StatusXJEnd.XjGameEnd.nSingleGameScore[i] = m_GameAccess->GetPlayerSingleScore(i);
				StatusXJEnd.XjGameEnd.nUserBet[i] = m_GameAccess->GetAllBet(i);
			}

			// 发送场景
			m_pITableFrame->SendGameScene( wChairID, &StatusXJEnd, sizeof(STR_CMD_SC_STATUS_XJ_END));

			break;
		}
		default:break;
	}

	return false;
}

// 定时器事件
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	//校验
	if ( NULL ==  m_GameAccess)
		return false;

	//流程处理
	switch(wTimerID)
	{
		case IDI_ROB_BANKER: // 抢庄定时器
		{
			m_pITableFrame->KillGameTimer(IDI_ROB_BANKER);

			//状态校验
			if (GS_WK_ROB != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_ROB != m_GameAccess->GetGameStatus() : err ：" << endl;
				return false;
			}

			//没有叫庄,默认不叫   如果是机器人，随机叫庄
			for (WORD i = 0; (m_GameAccess != NULL) && (i < m_GameAccess->GetMaxChairCount()); i++)
			{
				if ((USER_PLAYING == m_GameAccess->GetPlayerState(i))
					&& (i == m_GameAccess->GetCurRobUser()))
				{
					//CLog::Log(log_debug, "定时器到：默认不抢庄");

					WORD wNextUser = m_GameAccess->GetNextUser(i);

					if (wNextUser < 0 || wNextUser >= m_GameAccess->GetMaxChairCount())
					{
						//CLog::Log(log_error, "func 抢庄定时器 err : wChairID < 0 && wChairID >= m_GameAccess->GetMaxChairCount(): chairID:%d", wNextUser);
						return false;
					}

					while (i != wNextUser)		//判断是不抢还是不叫
					{
						if (ROB_TYPE_CALL == m_GameAccess->GetBankerState(wNextUser) || ROB_TYPE_ROB == (m_GameAccess->GetBankerState(wNextUser) + 1))
						{
							//判断玩家是机器人的情况
							if (m_pITableFrame->IsRobot(i))
							{
								BYTE CurCardData[MAX_CARD_COUNT] = { 0 };
								BYTE CurCardNum = m_GameAccess->GetUserCurCardNum(i);

								m_GameAccess->GetClientHandCards(i, CurCardData, CurCardNum);

								//初始化机器人手牌数据
								m_HandCardData[i].Init(CurCardData, CurCardNum);

								//根据手牌判断抢不抢地主
								int flag = m_AILogic.LandScore(m_HandCardData[i]);

								if (flag > 1)
								{
									OnUserCallBanker(i, ROB_TYPE_ROB);	//不抢或抢地主
								}
								else
								{
									OnUserCallBanker(i, ROB_STATE_BUQIANG);	//不抢或抢地主
								}

								break;
							}
							else
							{
								OnUserCallBanker(i, ROB_STATE_BUQIANG);	//不抢
								break;
							}
						}

						wNextUser = m_GameAccess->GetNextUser(wNextUser);
					}

					if (i == wNextUser)   //不叫
					{
						//判断玩家是机器人的情况
						if (m_pITableFrame->IsRobot(i))
						{
							BYTE CurCardData[MAX_CARD_COUNT] = { 0 };
							BYTE CurCardNum = m_GameAccess->GetUserCurCardNum(i);

							m_GameAccess->GetClientHandCards(i, CurCardData, CurCardNum);

							//初始化机器人手牌数据
							m_HandCardData[i].Init(CurCardData, CurCardNum);

							//根据手牌判断抢不抢地主
							int flag = m_AILogic.LandScore(m_HandCardData[i]);

							if (flag > 1)
							{
								OnUserCallBanker(i, ROB_TYPE_CALL);	//不抢或抢地主
							}
							else
							{
								OnUserCallBanker(i, ROB_STATE_BUJIAO);	//不抢或抢地主
							}

							break;
						}
						else
						{
							OnUserCallBanker(i, ROB_STATE_BUJIAO);
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case IDI_ADD_SCORE:		//下注定时器
		{
			m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);

			//状态校验
			if (GS_WK_ADDSCORE != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_ADDSCORE != m_GameAccess->GetGameStatus() : err ：" << endl;
				return false;
			}

			//没有下注  默认不下 
			for (WORD i = 0; i < m_GameAccess->GetMaxChairCount() && m_GameAccess != NULL; i++)
			{
				if (USER_PLAYING == m_GameAccess->GetPlayerState(i)
					/*&& (0 == m_GameAccess->GetXjReadyState(i))*/
					&& (0 == m_GameAccess->GetAddScoreState(i)))
				{
					printf("定时器到，默认不下注\n");
					//CLog::Log(log_error, "定时器到：默认不下注 %d\n", i);

					BYTE CurCardData[MAX_CARD_COUNT] = { 0 };
					BYTE CurCardNum = m_GameAccess->GetUserCurCardNum(i);

					m_GameAccess->GetClientHandCards(i, CurCardData, CurCardNum);

					//机器人根据手牌下注
					//初始化机器人手牌数据
					m_HandCardData[i].Init(CurCardData, CurCardNum);

					//根据手牌判断下不下注
					int flag = m_AILogic.LandScore(m_HandCardData[i]);

					if (m_pITableFrame->IsRobot(i) && 3 == flag)
						OnUserAddScore(i, 2);
					else
						OnUserAddScore(i, 0);
				}

			}

			break;
		}
		case IDI_MING_PAI:		//明牌定时器
		{
			m_pITableFrame->KillGameTimer(IDI_MING_PAI);

			//没有明牌 默认不明牌    机器人也默认不明牌
			for (WORD i = 0; i < m_GameAccess->GetMaxChairCount() && m_GameAccess != NULL; i++)
			{
				if (USER_PLAYING == m_GameAccess->GetPlayerState(i)
					/*&& (0 == m_GameAccess->GetXjReadyState(i))*/
					&& (0 == m_GameAccess->GetMingPaiState(i)))
				{
					printf("定时器到，默认不明牌\n");
					//CLog::Log(log_error, "定时器到：默认不明牌 %d\n", i);

					//处理不明牌消息
					On_Sub_UserMingPai(i, 0, 0);
					//On_Sub_UserMingPai(i, MING_PAI_TYPE_DEAL, 1);
				}
			}

			break;
		}
		case IDI_OUT_CARD:		//出牌定时器
		{
			m_pITableFrame->KillGameTimer(IDI_OUT_CARD);

			CLog::Log(log_debug, "Timer OUT_CARD func begin!!!");

			WORD OutCardUser = m_GameAccess->GetCurOutCardUser();
			if (OutCardUser == INVALID_CHAIR)
			{
				return false;
			}

			//状态校验
			if (GS_WK_OUT_CARD != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_OUT_CARD != m_GameAccess->GetGameStatus() : err ：托管" << endl;
				return false;
			}

			//如果玩家连续三次定时器超时 那么将他设置成托管状态
			if (!(m_pITableFrame->IsRobot(OutCardUser)))
			{
				WORD TimeOutNum = m_GameAccess->GetPlayerTimeOutNum(OutCardUser);
				m_GameAccess->SetPlayerTimeOutNum(OutCardUser, ++TimeOutNum);

				if (2 <= TimeOutNum)
				{
					//设置该玩家为托管
					m_GameAccess->SetPlayerTuoGuan(OutCardUser, 1);

					//发送托管
					m_pITableFrame->SendPlayerTuoGuan(OutCardUser);
				}
			}

			//不是第一个出牌  
			if (1 == m_GameAccess->GetOneTurnEnd())
			{
				BYTE OutCardData[MAX_CARD_COUNT] = {'\0'};
				BYTE OutCardNum;

				ZeroMemory(&OutCardData, sizeof(OutCardData));

				//判断机器人的情况
				if (m_pITableFrame->IsRobot(OutCardUser))
				{
					CLog::Log(log_debug, "Turn Not End Robot!! One");

					BYTE CurCardData[MAX_CARD_COUNT] = { 0 };
					BYTE CurCardNum = m_GameAccess->GetUserCurCardNum(OutCardUser);

					m_GameAccess->GetClientHandCards(OutCardUser, CurCardData, CurCardNum);

					//初始化机器人手牌数据
					m_HandCardData[OutCardUser].Init(CurCardData, CurCardNum);
					CLog::Log(log_debug, "Turn Not End Robot!! Two");
					//初始化当前轮手牌
					GameSituation m_GameSituation;
					m_GameLogic->GetTurnMaxCardToRobot(m_GameSituation, m_AILogic);

					//调用电脑人被动出牌函数
					m_AILogic.get_PutCardList_2_limit(m_GameSituation, m_HandCardData[OutCardUser]);
					CLog::Log(log_debug, "Turn Not End Robot!! Three");

					//判断出不出牌
					if (m_HandCardData[OutCardUser].uctPutCardType.cgType == cgERROR || m_HandCardData[OutCardUser].uctPutCardType.cgType == cgZERO)
					{
						On_Sub_UserOutCard(OutCardUser, NULL, 0, 1);   //出牌过
						break;
					}
					else
					{
						CLog::Log(log_debug, "Turn Not End Robot!! Four");

						//将要出的牌从逻辑值转换成有花色
						m_GameLogic->GetCardColorValue(OutCardUser, m_HandCardData[OutCardUser], OutCardData, OutCardNum);

						On_Sub_UserOutCard(OutCardUser, OutCardData, OutCardNum, 0);     //出牌
						break;
					}
				}
				else
				{
					//判断 玩家可以出的牌
					if (m_GameLogic->AnalysePlayerOutCard(OutCardUser, OutCardData, &OutCardNum))
					{
						if (!m_GameLogic->EfficacyOutCard(OutCardUser, OutCardData, OutCardNum))
						{
							cout << "判断 玩家可以出的牌 err" << endl;
							//CLog::Log(log_error, "func 判断 玩家可以出的牌 err : 校验失败");
							return false;
						}
						On_Sub_UserOutCard(OutCardUser, OutCardData, OutCardNum, 0);
						break;
					}
					else
						{
							On_Sub_UserOutCard(OutCardUser, NULL, 0, 1);   //出牌过	
							break;
						}
				}
			}
			//是第一个出牌  
			else
			{

				BYTE OutCardData[MAX_CARD_COUNT] = {'\0'};
				BYTE OutCardNum;

				ZeroMemory(&OutCardData, sizeof(OutCardData));

				if (m_pITableFrame->IsRobot(OutCardUser))
				{
					CLog::Log(log_debug, "Turn End Robot!! One");

					BYTE CurCardData[MAX_CARD_COUNT] = { 0 };
					BYTE CurCardNum = m_GameAccess->GetUserCurCardNum(OutCardUser);

					m_GameAccess->GetClientHandCards(OutCardUser, CurCardData, CurCardNum);

					//初始化机器人手牌数据
					m_HandCardData[OutCardUser].Init(CurCardData, CurCardNum);
					CLog::Log(log_debug, "Turn End Robot!! Two");
					//调用电脑人主动出牌函数
					m_AILogic.get_PutCardList_2_unlimit(m_HandCardData[OutCardUser]);

					CLog::Log(log_debug, "Turn End Robot!! Three");

					//将要出的牌从逻辑值转换成有花色
					m_GameLogic->GetCardColorValue(OutCardUser, m_HandCardData[OutCardUser], OutCardData, OutCardNum);
					if (CT_ERROR == m_GameLogic->GetCardLogicType(OutCardData, OutCardNum))
					{
						CLog::Log(log_debug, "Turn End Robot!! Four");
						//CLog::Log(log_error, "CT_ERROR == m_GameLogic->GetCardLogicType!!!!!!!!!\n");
						//如果机器人出牌出现错误
						BYTE UOutCardData[MAX_CARD_COUNT] = {'\0'};
						BYTE UOutCardNum;
						ZeroMemory(&UOutCardData, sizeof(UOutCardData));
						//判断 第一个玩家可以出的牌
						m_GameLogic->AnalysePlayerOutCardFirst(OutCardUser, UOutCardData, &UOutCardNum);

						//执行出牌消息
						On_Sub_UserOutCard(OutCardUser, UOutCardData, UOutCardNum, 0);
						break;
					}
					else
					{
						//执行出牌消息
						On_Sub_UserOutCard(OutCardUser, OutCardData, OutCardNum, 0);
						break;
					}
				}
				else

				{
					//判断 第一个玩家可以出的牌
					m_GameLogic->AnalysePlayerOutCardFirst(OutCardUser, OutCardData, &OutCardNum);

					//执行出牌消息
					On_Sub_UserOutCard(OutCardUser, OutCardData, OutCardNum, 0);
					break;

					On_Sub_UserOutCard(OutCardUser, NULL, 0, 1);   //出牌过	
							break;
				}

			}

			break;
		}
		case IDI_ROB_JiaoFen:	//叫分定时器
		{
			m_pITableFrame->KillGameTimer(IDI_ROB_JiaoFen);

			//状态校验
			if (GS_WK_JIAO_FEN != m_GameAccess->GetGameStatus())
			{
				cout << "定时器：GS_WK_JIAO_FEN != m_GameAccess->GetGameStatus() : err" << endl;
				return false;
			}

			cout << "叫分定时器到" << endl;

			//玩家默认不叫  机器人随机叫分
			for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
			{
				if (i == m_GameAccess->GetCurJiaoFenUser() && USER_PLAYING == m_GameAccess->GetPlayerState(i))
				{
					//机器人情况
					if (m_pITableFrame->IsRobot(i))
					{
						WORD m_Score = JIAOFEN_START;
						for (int j = 0; j < m_GameAccess->GetCurPlayerCount(); j++)
						{
							if ((0 != m_GameAccess->GetCurJiaoFenState(j))
								&& (10 != m_GameAccess->GetCurJiaoFenState(j)))    //默认状态和不叫状态
							{
								m_Score -= 1 << (m_GameAccess->GetCurJiaoFenState(j));
							}
						}

						if (JIAOFEN_START == m_Score)  //六种叫分情况
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
		case IDI_ROB_START:		//开始抢庄定时器
			{
				m_pITableFrame->KillGameTimer(IDI_ROB_START);

				if (m_GameAccess->GetGameStatus() != GS_WK_FREE && m_GameAccess->GetGameStatus() != GS_WK_XJ_GAMEEND &&
					m_GameAccess->GetGameStatus() != GS_WK_ROB)
					return false;

				HandleRobBanker();
			}
		case IDI_OUTCARD_START:		//开始出牌定时器
			{
				m_pITableFrame->KillGameTimer(IDI_OUTCARD_START);

				if (m_GameAccess->GetGameStatus() != GS_WK_ADDSCORE)
					return false;

				HandleOutCardStart(m_GameAccess->GetBankerID());
			}
		default:
			return false;
	}
	
	return true;
}

// 游戏消息处理
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, WORD wChairID)
{
	//校验
	if (pData == NULL)
		return false;

	//消息处理
	switch (wSubCmdID)
	{
		case SUB_CS_OUT_CARD:		//出牌
		{
			//效验数据
			if (wDataSize != sizeof(STR_SUB_CS_OUT_CARD) )
				return true;

			//状态校验
			if (GS_WK_OUT_CARD != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_OUT_CARD != m_GameAccess->GetGameStatus() : err" << endl;
				return false;
			}

			//变量定义
			STR_SUB_CS_OUT_CARD *pOutCard= (STR_SUB_CS_OUT_CARD *)pData;

			//如果用户出牌了 清除他的连续超时次数
			m_GameAccess->SetPlayerTuoGuan(wChairID, 0);

			bool bPass = (pOutCard->cbPass == 1) ? true : false;
			On_Sub_UserOutCard(wChairID, pOutCard->cbOutCard, pOutCard->cbOutCardNum, bPass);

			break;	
		}
		case SUB_CS_ROB_BANKER:		 //抢庄命令
		{
			//效验数据
			if (wDataSize != sizeof(STR_SUB_CS_ROB_BANKER))
			{
				return true;//舍弃消息 也表示处理
			}

			//状态校验
			if (GS_WK_ROB != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_ROB != m_GameAccess->GetGameStatus() : err" << endl;
				return false;
			}

			//变量定义
			STR_SUB_CS_ROB_BANKER *pCallBanker = (STR_SUB_CS_ROB_BANKER *)pData;
			OnUserCallBanker(wChairID, pCallBanker->cbResult);

			break;
		}
		case SUB_CS_ADD_SCORE:	 //用户下注
		{
			//校验数据
			if (wDataSize != sizeof(STR_SUB_CS_ADD_SCORE))
				return true;

			//状态校验
			if (GS_WK_ADDSCORE != m_GameAccess->GetGameStatus())
			{
				cout << "(GS_WK_ADDSCORE != m_GameAccess->GetGameStatus() : err" << endl;
				return false;
			}

			STR_SUB_CS_ADD_SCORE *pAddScore = (STR_SUB_CS_ADD_SCORE *)pData;

			//处理
			OnUserAddScore(wChairID, pAddScore->action);

			break;
		}
		case SUB_CS_MING_PAI:	//明牌
		{
			//校验
			if (1 != m_GameAccess->GetPlayerMingPaiBet(wChairID))
			{
				printf("玩家：%d 是否已明牌：%d\n", wChairID, m_GameAccess->GetMingPaiState(wChairID));
				cout << "玩家已经明过牌了 : err" << endl;
				return false;
			}

			//校验数据
			if (wDataSize != sizeof(STR_SUB_CS_MING_PAI))
				return true;

			STR_SUB_CS_MING_PAI *pMingPai = (STR_SUB_CS_MING_PAI *)pData;

			//开始游戏明牌情况
			if (pMingPai->cbType == MING_PAI_TYPE_GAMESTART)
			{
				m_GameAccess->SetPlayerMingPaiBet(wChairID, 5);
				break;
			}

			On_Sub_UserMingPai(wChairID, pMingPai->cbType, 1, pMingPai->OutCard_stage);
			break;
		}
		case SUB_CS_PUBLIC_BET:		//游戏结束的公共倍数信息
		{
			//校验数据
			if (wDataSize != sizeof(STR_SUB_CS_PUBLIC_BET))
				return true;

			//状态校验
			if (GS_WK_FREE == m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_FREE == m_GameAccess->GetGameStatus() : err" << endl;
				return false;
			}

			STR_SUB_CS_PUBLIC_BET *pPublicBet = (STR_SUB_CS_PUBLIC_BET *)pData;

			OnUserPublicBet(wChairID);

			break;
		}
		case SUB_CS_JIAO_FEN:		//叫分
		{
			//校验数据
			if (wDataSize != sizeof(STR_SUB_CS_JIAO_FEN))
				return true;

			//状态校验
			if (GS_WK_JIAO_FEN != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_JIAO_FEN != m_GameAccess->GetGameStatus() : err" << endl;
				return false;
			}

			STR_SUB_CS_JIAO_FEN *pJiaoFen = (STR_SUB_CS_JIAO_FEN *)pData;

			OnUserJiaoFen(wChairID, pJiaoFen->Score);

			break;
		}
		case SUB_CS_JIPAIQI:		//记牌器
		{
			//校验数据
			if (wDataSize != sizeof(STR_SUB_CS_JIPAIQI))
				return true;

			//状态校验
			if (GS_WK_OUT_CARD != m_GameAccess->GetGameStatus())
			{
				cout << "GS_WK_OUT_CARD != m_GameAccess->GetGameStatus() : err" << endl;
				return false;
			}

			STR_SUB_CS_JIPAIQI *pJiPaiQi = (STR_SUB_CS_JIPAIQI *)pData;

			OnUserJiPaiQi(wChairID);

			break;
		}
		default:
			return false;
	}

	return true;
}

// 大局游戏开始				
bool CTableFrameSink::OnEventGameStart()
{
	//开始游戏
	StartGame();
	
	return true;
}

// 开始游戏
void CTableFrameSink::StartGame()
{
	CLog::Log(log_debug, "\n【开始游戏】\n");

	// 设置玩家状态【游戏中】
	BYTE cbPlayerNum = m_GameAccess->GetMaxChairCount();
	for ( BYTE i = 0; i < cbPlayerNum; ++i)
	{	
		m_GameAccess->SetPlayerState( i, USER_PLAYING );

		//设置底分
		m_GameLogic->Wager(i, m_GameAccess->GetCellScore());
	}

	// 游戏局数加1
	BYTE cbGameCout = m_GameAccess->GetCurGameCount();
	m_GameAccess->SetGameCount( ++cbGameCout );

	//给玩家发牌
	HandleDeal();
}

//处理发牌流程
void CTableFrameSink::HandleDeal()
{
	// 洗牌并发牌
	m_GameLogic->Deal();		//洗牌算法增加洗牌和不洗牌两种策略

	// 构造发牌数据
	STR_CMD_SC_SEND_CARD SendCard;

	//遍历玩家
	BYTE cbMaxChairCount = m_GameAccess->GetMaxChairCount();
	for (WORD i = 0; i < cbMaxChairCount; i++)
	{
		if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
		{
			ZeroMemory(&SendCard, sizeof(STR_CMD_SC_SEND_CARD));

			//赋值	
			SendCard.wChairID = i;
			SendCard.CurJuShu = m_GameAccess->GetCurGameCount();

			CLog::Log(log_debug, "ChairID: %d, CardNum: %d", i, m_GameAccess->GetUserCurCardNum(i));

			//设置是否明牌开始
			if (5 == m_GameAccess->GetPlayerMingPaiBet(i))
			{
				SendCard.ISMingPaiStart = 1;
			}
			else
			{
				SendCard.ISMingPaiStart = 0;
			}

			//设置手牌数据  
			int _cardsum = m_GameAccess->GetPlayerInitCardNum();
			m_GameAccess->GetClientHandCards(i, SendCard.cbHandCardData, _cardsum);

			printf("ChairID: %d Card:", i);
			for (int j = 0; j < _cardsum; j++)
			{
				printf(" %d", SendCard.cbHandCardData[j]);
			}
			printf("\n");

			//设置各卡牌的数量
			for (int j = 0; j < cbMaxChairCount; j++)
			{
				SendCard.cbHandCardNum[j] = m_GameAccess->GetUserCurCardNum(j);
			}

			// 发牌给对应客户端 -- 每个客户端应该只能收到自己的牌
			m_pITableFrame->SendTableData(i, CMD_SC_SEND_CARD, &SendCard, sizeof(STR_CMD_SC_SEND_CARD));
		}
	}

	// 根据规则判断是否要明牌这个功能
	if (1 == m_GameAccess->GetMingPaiMode())
	{
		for (int i = 0; i < cbMaxChairCount; i++)
		{
			//判断玩家是否开始游戏就明牌
			printf("玩家：%d 是否开始游戏明牌:%d\n", i, m_GameAccess->GetPlayerMingPaiBet(i));
			if (5 == m_GameAccess->GetPlayerMingPaiBet(i))
			{
				On_Sub_UserMingPai(i, MING_PAI_TYPE_GAMESTART, 1);
			}
			else
			{
				SendMingPaiStart(i, MING_PAI_TYPE_DEAL);    //发送明牌开始消息
			}
		}

		//设置抢庄开始定时器
		m_pITableFrame->KillGameTimer(IDI_ROB_START);
		m_pITableFrame->SetGameTimer(IDI_ROB_START, IDI_TIME_ROB_START, 1, 0);
	}
	//否则直接开始抢庄
	else
	{
		//设置抢庄开始定时器
		m_pITableFrame->KillGameTimer(IDI_ROB_START);
		m_pITableFrame->SetGameTimer(IDI_ROB_START, IDI_TIME_ROB_START, 1, 0);
	}
}

//处理抢庄
void CTableFrameSink::HandleRobBanker()
{
	//删除明牌定时器
	m_pITableFrame->KillGameTimer(IDI_MING_PAI);

	//根据房间规则判断抢庄模式
	BYTE cbRobBankMode = m_GameAccess->GetRobBankMode();   

	////给第一轮的玩家发送记牌器
	//for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
	//{
	//	OnUserJiPaiQi(i);
	//}

	////发送房间倍数信息
	//STR_CMD_SC_FANGJIAN_BET fangjian_bet;
	//ZeroMemory(&fangjian_bet, sizeof(fangjian_bet));

	//for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
	//{
	//	fangjian_bet.room_bet[i] = m_GameAccess->GetAllBet(i);
	//}

	//m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_FANGJIAN_BET, &fangjian_bet, sizeof(STR_CMD_SC_FANGJIAN_BET));

	//1、叫三分	
	if (ROOMRULE_OPTION_ROBTYPE_JIAOSANFEN == cbRobBankMode)
	{
		//叫三分处理流程
		HandleJiaoFenRobBanker();
	}

	//2、自由抢庄：发送【抢庄开始】-处理【抢庄】消息
	if (ROOMRULE_OPTION_ROBTYPE_FREE == cbRobBankMode)       //测试自由庄和轮庄
	{
		//自由抢庄消息处理流程
		HandleFreeRobBanker();
	}
}

//处理叫三分
void CTableFrameSink::HandleJiaoFenRobBanker()
{
	//随机一个人出来  叫三分
	WORD wChairID = rand() % m_GameAccess->GetCurPlayerCount();

	//不是第一局 让上局赢家叫分
	if (1 != m_GameAccess->GetCurGameCount())
	{
		wChairID = m_GameAccess->GetLastGameWinner();
	}

	//发送叫三分开始消息
	SendJiaoFenStart(wChairID, JIAOFEN_START);
}	  

//发送叫三分开始消息
void CTableFrameSink::SendJiaoFenStart(WORD wChairID, const BYTE &ActionType)
{
	//构建消息
	STR_CMD_SC_JIAOFEN_START  JiaofenStart;
	ZeroMemory(&JiaofenStart, sizeof(STR_CMD_SC_JIAOFEN_START));

	//赋值
	JiaofenStart.wChairID = wChairID;
	JiaofenStart.ActionType = ActionType;

	//设置当前正在叫分的玩家     
	m_GameAccess->SetCurJiaoFenUser(wChairID);

	if (INVALID_CHAIR != wChairID)
	{
		//发送消息  广播
		m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_JIAOFEN_START, &JiaofenStart, sizeof(STR_CMD_SC_JIAOFEN_START));

		//设置游戏状态
		m_GameAccess->SetGameStatus(GS_WK_JIAO_FEN);

		//设置叫三分定时器  
		m_pITableFrame->KillGameTimer(IDI_ROB_JiaoFen);

		//判断机器人
		if (m_pITableFrame->IsRobot(wChairID))     //待添加 机器人接口还没添加
		{
			WORD index = (rand() % 4) + 1;
			m_pITableFrame->SetGameTimer(IDI_ROB_JiaoFen, IDI_TIME_ROB_JiaoFen*0.1*index, 1, 0);
		}
		else
		{
			m_pITableFrame->SetGameTimer(IDI_ROB_JiaoFen, IDI_TIME_ROB_JiaoFen, 1, 0);
		}
	}
}

//处理客户端发来的叫三分消息
void CTableFrameSink::OnUserJiaoFen(WORD wChairID, BYTE Score)
{
	//状态校验
	if (GS_WK_JIAO_FEN != m_GameAccess->GetGameStatus())
	{
		cout << "叫分状态校验失败" << endl;
		return;
	}

	//杀死定时器
	m_pITableFrame->KillGameTimer(IDI_ROB_JiaoFen);

	//构造消息
	STR_CMD_SC_JIAOFEN_RESULT JiaofenResult;
	ZeroMemory(&JiaofenResult, sizeof(STR_CMD_SC_JIAOFEN_RESULT));

	//赋值
	JiaofenResult.wChairID = wChairID;
	JiaofenResult.Score = Score;

	for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
	{
		JiaofenResult.room_bet[i] = m_GameAccess->GetAllBet(i);
	}

	////录像叫分结果					//录像回放  待添加
	//AddRecordJiaoSanFen(JiaofenResult);

	//广播叫分结果
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_JIAOFEN_RESULT, &JiaofenResult, sizeof(STR_CMD_SC_JIAOFEN_RESULT));

	//设置一个参数记录叫分次数 
	WORD JiaoNum = m_GameAccess->GetJiaoFenCount();
	m_GameAccess->SetJiaoFenCount(++JiaoNum);

	//记录当前玩家的叫分状态
	m_GameAccess->SetCurJiaoFenState(wChairID, Score);

	//如果玩家直接叫三分  设置他为地主
	if (3 == Score)
	{
		//给每个玩家乘上叫分倍数
		for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
		{
			m_GameLogic->Wager(i, Score);
		}

		//指定他为庄家
		m_GameLogic->AppointBanker(wChairID);

		//叫分完成 进入下注状态
		AllRobBankerOver();
	}
	else
	{
		//记录有多少个不叫的玩家
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


		//如果所有人不叫 重新发牌
		if (3 == cbPassNum)
		{
			//三轮都不叫 随机一个玩家地主
			WORD TurnPassCount = m_GameAccess->GetTurnPassCount();
			m_GameAccess->SetTurnPassCount(++TurnPassCount);

			if (3 == TurnPassCount)
			{
				WORD banker = rand() % cbPassNum;
				//指定他为庄家
				m_GameLogic->AppointBanker(banker);

				AllRobBankerOver();

				return;
			}

			//清空记录的叫分次数
			m_GameAccess->SetJiaoFenCount(0);

			//清空玩家的叫分状态  清空玩家的明牌状态  清空明牌倍数
			for (int i = 0; i < cbMaxChairCount; i++)
			{
				m_GameAccess->SetCurJiaoFenState(i, 10);
				m_GameAccess->SetMingPaiState(i, 0);
				m_GameAccess->SetPlayerMingPaiBet(i, 1);
			}

			//清空倍数
			m_GameAccess->CleanAllBet();

			//给玩家发牌
			HandleDeal();

			//sleep(4);

			//处理抢庄（叫分）
			HandleRobBanker();
		}

		else
		{
			//否则判断叫分是否已经叫过了三次 有三次设置叫分最大的玩家为地主
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

				//给每个玩家乘上最大叫分倍数
				for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
				{
					m_GameLogic->Wager(i, m_Score);
				}

				//指定他为庄家
				m_GameLogic->AppointBanker(Banker);

				AllRobBankerOver();
			}
			else		//否则 给下个玩家发送叫分信息
			{
				WORD m_Score = JIAOFEN_START;    //1111
				for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
				{
					if ((USER_PLAYING == m_GameAccess->GetPlayerState(i)) &&
						(0 != m_GameAccess->GetCurJiaoFenState(i))
						&& (10 != m_GameAccess->GetCurJiaoFenState(i)))    //默认状态和不叫状态
					{
						m_Score -= 1 << (m_GameAccess->GetCurJiaoFenState(i));
					}
				}


				//给下个玩家发送叫分消息
				SendJiaoFenStart(m_GameAccess->GetNextUser(wChairID), m_Score);
			}
		}


	}
}

//处理自由抢庄
void CTableFrameSink::HandleFreeRobBanker()
{
	//第一局：从房主开始发送抢庄开始消息	第二局：从上局的赢家开始发送抢庄消息
	WORD wChairID = rand()%m_GameAccess->GetMaxChairCount();
	if (1 != m_GameAccess->GetCurGameCount())
	{
		wChairID = m_GameAccess->GetLastGameWinner();
	}
	m_GameAccess->SetStartRobUser(wChairID);

	//发送抢庄(叫地主)开始消息
	SendRobStart(wChairID, ROB_TYPE_CALL);
}

//发送抢庄开始消息
void CTableFrameSink::SendRobStart(const WORD &wChairID, const BYTE &cbType)
{
	//赋值
	//RobStart.cbType = cbType;
	//RobStart.wChairID = wChairID;

	//设置当前正在抢庄的玩家
	m_GameAccess->SetCurRobUser(wChairID);

	if (INVALID_CHAIR != wChairID)
	{
		STR_CMD_SC_ROB_BANKER_START info;
		if (cbType == ROB_TYPE_CALL)
			info.cbType = 0;
		else
			info.cbType = 1;

		m_pITableFrame->SendTableData(wChairID, CMD_SC_ROB_START, &info, sizeof(STR_CMD_SC_ROB_BANKER_START));

		//设置游戏状态为【抢庄状态】
		m_GameAccess->SetGameStatus(GS_WK_ROB);

		//抢庄定时器
		m_pITableFrame->KillGameTimer(IDI_ROB_BANKER);

		//判断是不是机器人
		if (m_pITableFrame->IsRobot(wChairID))     //机器人接口待添加
		{
			WORD index = (rand() % 3) + 1;
			m_pITableFrame->SetGameTimer(IDI_ROB_BANKER, IDI_TIME_ROB_BANKER*0.1*index, 1, 0);
		}
		else
		{
			//判断是否托管
			if (1 == m_GameAccess->GetPlayerTuoGuan(wChairID))
			{
				m_pITableFrame->SetGameTimer(IDI_ROB_BANKER, IDI_TIME_ROB_BANKER*0.1, 1 , 0);
			}
			else
			{
				m_pITableFrame->SetGameTimer(IDI_ROB_BANKER, IDI_TIME_ROB_BANKER, 1, 0);
			}
		}
	}
}

// 处理客户端发来的【抢庄】消息
void CTableFrameSink::OnUserCallBanker(WORD wChairID, BYTE cbResult)	//1-不叫  2-叫地主  3-不抢  4-抢地主
{
	//校验
	if (m_GameAccess->GetCurRobUser() != wChairID)
		return;

	CLog::Log(log_debug, "【服务器】：接收用户 = %d, 抢庄类型 = %d", wChairID, cbResult);

	//判断玩家叫抢
	int flag = 0;
	for (int i = 0; i < m_GameAccess->GetMaxChairCount(); i++)
	{
		if (ROB_TYPE_CALL == m_GameAccess->GetBankerState(i) || ROB_TYPE_ROB == m_GameAccess->GetBankerState(i))
		{
			flag = 1;
		}
	}

	if (1 == flag)
	{
		if (cbResult == 1)
			cbResult = ROB_STATE_BUQIANG;
		else
			cbResult = ROB_TYPE_ROB;
	}
	else
	{
		if (cbResult == 1)
			cbResult = ROB_STATE_BUJIAO;
		else
			cbResult = ROB_TYPE_CALL;
	}

	//如果抢设置抢庄倍数
	if (ROB_TYPE_ROB == cbResult)
	{
		for (int i = 0; i < m_GameAccess->GetMaxChairCount(); i++)
		{
			if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
			{
				m_GameLogic->Wager(i, 2);      //如果抢地主  给每个玩家倍数乘2
			}
		}
	}

	//关闭定时器
	m_pITableFrame->KillGameTimer(IDI_ROB_BANKER);

	//游戏状态效验
	if (GS_WK_ROB != m_GameAccess->GetGameStatus())
		return;

	//构造抢庄结果数据
	STR_CMD_SC_ROB_RESULT sRobResult;
	ZeroMemory(&sRobResult, sizeof(STR_CMD_SC_ROB_RESULT));

	//赋值 
	sRobResult.wChairID  = wChairID;	
	sRobResult.cbResult = cbResult;	

	for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
	{
		sRobResult.room_bet[i] = m_GameAccess->GetAllBet(i);
	}

	//抢庄录像  //待添加
	//AddRecordRobResult(sRobResult);

	//广播抢庄结果
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_ROB_RESULT, &sRobResult, sizeof(STR_CMD_SC_ROB_RESULT));

	////////////////////////////////////////////////////////////////////////
	//两人斗地主测试用 
	if (2 == m_GameAccess->GetMaxChairCount())
	{
		//CLog::Log(log_debug, "cbResult: %d", cbResult);
		if (cbResult == ROB_TYPE_CALL || cbResult == ROB_TYPE_ROB)
		{
			m_GameLogic->AppointBanker(wChairID);
			AllRobBankerOver();
		}
		else
		{
			WORD wNextUser = m_GameAccess->GetNextUser(wChairID);
			SendRobStart(wNextUser, ROB_TYPE_CALL);    //叫地主类型
		}
		return;
	}
	////////////////////////////////////////////////////////////////////////

	//设置一个参数记录抢庄次数      为了判断过过抢地主的情况
	BYTE robnum = m_GameAccess->GetCurBankerCount();
	m_GameAccess->SetCurBankerCount(++robnum);
	printf("记录抢庄次数:%d\n", robnum);

	//设置玩家当前抢庄状态
	m_GameAccess->SetBankerState(wChairID, cbResult);

	//玩家同意抢，则设置抢庄次数
	BYTE cbRobNum = m_GameAccess->GetUserRobNum(wChairID);
	if (ROB_TYPE_CALL == cbResult || ROB_TYPE_ROB == cbResult)
	{
		m_GameAccess->SetUserRobNum(wChairID, ++cbRobNum);
	}

	//若玩家两次都抢庄，则他为庄家
	if (2 == cbRobNum)
	{
		//指定他为庄家
		m_GameLogic->AppointBanker(wChairID);

		AllRobBankerOver();
	}
	else	//抢庄还未完成，则继续给下个玩家发送抢庄消息
	{
		//判断是不是所有玩家都选择不抢庄
		BYTE cbMaxChairCount = m_GameAccess->GetMaxChairCount();
		BYTE cbPassNum = 0;
		for (int i = 0; i < cbMaxChairCount; i++)
		{
			if ((USER_PLAYING == m_GameAccess->GetPlayerState(i)) &&
				((ROB_STATE_BUJIAO == m_GameAccess->GetBankerState(i)) ||
				(ROB_STATE_BUQIANG == m_GameAccess->GetBankerState(i))))     //1-不叫  2-叫地主  3-不抢  4-抢地主
			{
				cbPassNum++;
			}
		}

		//所有人放弃抢庄，则重新发牌
		if (cbPassNum == cbMaxChairCount)
		{
			cout << "所有人放弃抢庄" << endl;

			//三轮都不叫 随机一个玩家地主
			WORD TurnPassCount = m_GameAccess->GetTurnPassCount();
			m_GameAccess->SetTurnPassCount(++TurnPassCount);

			if (3 == TurnPassCount)
			{
				WORD banker = rand() % cbPassNum;
				//指定他为庄家
				m_GameLogic->AppointBanker(banker);

				AllRobBankerOver();

				return;
			}

			//清空记录的抢庄各状态
			m_GameAccess->SetCurBankerCount(0);

			//清空玩家的明牌状态抢庄各状态
			for (int i = 0; i < cbMaxChairCount; i++)
			{
				m_GameAccess->SetMingPaiState(i, 0);
				m_GameAccess->SetPlayerMingPaiBet(i, 1);
				m_GameAccess->SetBankerState(i, 0);
				m_GameAccess->SetUserRobNum(i, 0);
			}

			//清空倍数
			m_GameAccess->CleanAllBet();

			//给玩家发牌
			HandleDeal();

			//设置抢庄开始定时器
			m_pITableFrame->KillGameTimer(IDI_ROB_START);
			m_pITableFrame->SetGameTimer(IDI_ROB_START, IDI_TIME_ROB_START, 1, 0);
		}
		else	//给下个玩家发送抢庄消息
		{
			//给下一个玩家发送抢庄开始消息
			WORD wNextUser = m_GameAccess->GetNextUser(wChairID);
			WORD wLastUser = m_GameAccess->GetLastUser(wChairID);

			//判断下个玩家的抢庄状态
			while (wNextUser != wChairID)		//下个玩家不是“过”状态才给他发送抢地主消息
			{
				if (robnum == 4)   //抢地主最多只能四个回合
				{
					if (ROB_TYPE_ROB == m_GameAccess->GetBankerState(wChairID))    //第四回合抢地主就是他为地主
					{
						//指定他为庄家
						m_GameLogic->AppointBanker(wChairID);

						AllRobBankerOver();
						break;
					}
					else				//设置最后抢庄玩家为庄
					{
						for (int i = (wChairID + cbMaxChairCount - 1); i > wChairID; i--)
						{
							WORD chairid = (i > 0) ? i % cbMaxChairCount : (-i) % cbMaxChairCount;
							if ((USER_PLAYING == m_GameAccess->GetPlayerState(chairid)) &&
								ROB_TYPE_ROB == m_GameAccess->GetBankerState(chairid))
							{
								//指定他为庄家
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

				else if ((ROB_TYPE_ROB == m_GameAccess->GetBankerState(wNextUser)
					|| ROB_TYPE_CALL == m_GameAccess->GetBankerState(wNextUser)
					|| 0 == m_GameAccess->GetBankerState(wNextUser))	//初始状态
					&& (int)cbPassNum != 2)   //和上面同理 914
				{
					if ((int)cbPassNum == 1 && robnum == 1)
					{
						SendRobStart(wNextUser, ROB_TYPE_CALL);    //叫地主类型
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


			//前两个玩家都不抢 
			if (wNextUser == wChairID)
			{
				//目前有两个玩家都不抢 获取当前抢庄的玩家  设置他为地主
				BYTE cbMaxChairCount = m_GameAccess->GetMaxChairCount();
				WORD cbRobUser;
				for (int i = 0; i < cbMaxChairCount; i++)
				{
					if (ROB_TYPE_ROB == m_GameAccess->GetBankerState(i)
						|| ROB_TYPE_CALL == m_GameAccess->GetBankerState(i))  //1-不叫  2-叫地主  3-不抢  4-抢地主
					{
						cbRobUser = i;
					}
				}

				//指定庄家
				m_GameLogic->AppointBanker(cbRobUser);
				AllRobBankerOver();
			}
		}
	}
}

// 全部抢庄完成, 进入下注状态
void CTableFrameSink::AllRobBankerOver()
{
	//设置游戏状态为【下注状态】
	m_GameAccess->SetGameStatus(GS_WK_ADDSCORE);

	//关闭抢庄定时器
	m_pITableFrame->KillGameTimer(IDI_ROB_BANKER);

	//清空抢庄次数
	m_GameAccess->SetCurBankerCount(0);

	//指定庄家
	WORD wBanker = m_GameLogic->GetAppointBanker();
	printf("获取当前指定的庄家：%d", m_GameLogic->GetAppointBanker());

	//校验 这里不退出，默认房主为庄家
	if (INVALID_CHAIR == wBanker /*|| !bappointresult*/)
	{
		wBanker = m_GameAccess->GetRoomFangzhu();
	}

	//记录玩家抢庄次数  大局结算
	m_GameAccess->SetPlayerBankCount(wBanker);

	//发送指定庄家消息
	SendAppointBanker(wBanker);

	//发送底牌给庄家
	HandleSendLeaveCard(wBanker);

	cout << "发送底牌over" << endl;

	//设置第一轮最开始出牌用户
	m_GameAccess->SetStartOutCardUser(wBanker);

	//根据规则是否发送加注开始（加倍）消息
	if (1 == m_GameAccess->GetAddBetMode())
	{
		SendAddScoreStart(INVALID_CHAIR);
	}
	//否则直接开始出牌
	else
	{
		HandleOutCardStart(wBanker);
	}

	//发送当前房间倍数给各玩家						//记得叫客户端加上这个消息
	STR_CMD_SC_FANGJIAN_BET  fangjian_bet;
	ZeroMemory(&fangjian_bet, sizeof(STR_CMD_SC_FANGJIAN_BET));

	for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
	{
		fangjian_bet.room_bet[i] = m_GameAccess->GetAllBet(i);
	}

	//m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_FANGJIAN_BET, &fangjian_bet, sizeof(STR_CMD_SC_FANGJIAN_BET));
}

//处理发送底牌
void CTableFrameSink::HandleSendLeaveCard(const WORD &wSendCardUser)
{
	CLog::Log(log_debug, "【给玩家 = %d发送底牌】", wSendCardUser);

	//根据规则判断是否底牌加倍
	if (1 == m_GameAccess->GetISLeaveCardDouble())
	{
		m_GameLogic->IsDoubleLeaveCard();
	}

	//给庄家发送底牌
	m_GameLogic->SendLeaveCardToUser(wSendCardUser);

	//设置癞子,随机一张牌非双王的牌来作为癞子
	if (0 < m_GameAccess->GetLaiZiMode())
	{
		DWORD index = rand() % (15 - 2);
		BYTE LaiZi = m_GameLogic->GetCardLogicValue(CGameData::m_cbCardData[index]);
		m_GameAccess->SetMagicCard(LaiZi);
		CLog::Log(log_debug, "LAIZI: %d", LaiZi);
	}

	//给其他玩家发送庄家拿到底牌的消息
	BYTE cbMaxChairCount = m_GameAccess->GetMaxChairCount();

	//发送当前房间倍数给各玩家						
	STR_CMD_SC_FANGJIAN_BET  fangjian_bet;
	ZeroMemory(&fangjian_bet, sizeof(STR_CMD_SC_FANGJIAN_BET));

	//给地主加上农民的倍数  并给农民加上地主的倍数
	for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
	{
		fangjian_bet.room_bet[i] = m_GameAccess->GetAllBet(i);
	}

	//m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_FANGJIAN_BET, &fangjian_bet, sizeof(STR_CMD_SC_FANGJIAN_BET));

	//发送底牌
	for (WORD i = 0; i < cbMaxChairCount; i++)
	{
		if ((USER_PLAYING == m_GameAccess->GetPlayerState(i)))
		{
			//获取底牌
			BYTE leavecards[MAX_LEAVE_CARD_NUM];
			m_GameAccess->GetLeaveCard(leavecards, MAX_LEAVE_CARD_NUM);

			//发送底牌消息
			STR_CMD_SC_SEND_LEAVE_CARD SendLeaveCard;
			ZeroMemory(&SendLeaveCard, sizeof(STR_CMD_SC_SEND_LEAVE_CARD));	

			//赋值底牌
			for (int j = 0; j < MAX_LEAVE_CARD_NUM; j++)
			{
				SendLeaveCard.cbLeaveCard[j] = leavecards[j];
			}

			BYTE tmpcards[MAX_CARD_COUNT] = {'\0'};
			
			//玩家手牌数
			BYTE cbCardNum = m_GameAccess->GetUserCurCardNum(i);

			//排序手牌
			m_GameLogic->SortCardList(i, cbCardNum, ST_ORDER); 

			//将玩家手牌中的癞子进行牌值替换
			m_GameLogic->ReplaceLaiZiCard(i, cbCardNum);

			//赋值
			SendLeaveCard.cbMagicCard = m_GameAccess->GetMagicCard();	// Seeqings modify

			for (int j = 0; j < cbMaxChairCount; j++)
			{
				SendLeaveCard.cbHandCardNum[j] = m_GameAccess->GetUserCurCardNum(j);
			}

			//获取手牌
			m_GameAccess->GetClientHandCards(i, tmpcards, cbCardNum);

			for (int j = 0; j < cbCardNum; j++)
			{
				SendLeaveCard.cbSortedCard[j] = tmpcards[j];
				cout << " " << (int)tmpcards[j];
			}
			cout << endl;

			//获取癞子数目
			SendLeaveCard.LeaveCardBet = m_GameAccess->GetLeaveCardBet();

			// 发送数据
			m_pITableFrame->SendTableData(i, CMD_SC_USER_SEND_LEAVE_CARD, &SendLeaveCard, sizeof(STR_CMD_SC_SEND_LEAVE_CARD));
		}
	}
}

//发送下注开始消息
void CTableFrameSink::SendAddScoreStart(const WORD &wChairID)
{
	CLog::Log(log_debug, "【服务器】：广播【下注开始】");

	// 广播消息 用户下注开始
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_ADD_SCORE_START, NULL, 0);
		
	// 下注定时器
	m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);
	m_pITableFrame->SetGameTimer(IDI_ADD_SCORE, IDI_TIME_ADD_SCORE, 1, 0);
}

// 下注开始返回
void CTableFrameSink::OnUserAddScore( WORD wChairID, WORD wType )
{
	//超加3倍
	SCORE lScore = (wType == 0) ? 1 : (wType == 1 ? 2 : 3);
	CLog::Log(log_debug, "【服务器】：接收【下注】，玩家 = %d，下注 = %ld", wChairID, lScore);

	//游戏状态效验
	if (GS_WK_ADDSCORE != m_GameAccess->GetGameStatus())   //已设定为下注状态
	{
		return;
	}

	//用户状态校验
	if ((USER_PLAYING != m_GameAccess->GetPlayerState(wChairID))
		&& (1 != m_GameAccess->GetAddScoreState(wChairID)))
	{
		cout << "游戏效验失败" << endl;
		return;
	}

	//可多次加注, 因此此处不处理加注完成标记
	m_GameLogic->Wager(wChairID, lScore);

	//给地主加上农民的倍数		//给农民加上地主的加倍
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

	//记录玩家下注
	m_GameAccess->SetPlayerAddScore(wChairID, lScore);

	// 设置玩家下注状态
	m_GameAccess->SetAddScoreState(wChairID, 1);

	//构造数据
	STR_CMD_SC_ADD_SCORE_RESULT sAddScoreResult;
	ZeroMemory(&sAddScoreResult, sizeof(STR_CMD_SC_ADD_SCORE_RESULT)); // Seeqings modify

	//赋值
	sAddScoreResult.wChairID	 = wChairID;	
	sAddScoreResult.byAction	 = wType;		

	for (int i = 0; i < m_GameAccess->GetMaxChairCount(); i++)
	{
		if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
			sAddScoreResult.RoomBet[i] = m_GameAccess->GetAllBet(i);
	}

	//广播下注结果
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_ADD_SCORE_RESULT, &sAddScoreResult, sizeof(STR_CMD_SC_ADD_SCORE_RESULT));

	// 计算加注完成玩家数
	BYTE cbBetedNum = 0;
	BYTE cbPlayerNum = m_GameAccess->GetMaxChairCount();// 获取玩家数
	for (BYTE i = 0; i < cbPlayerNum; ++i)
	{
		if (1 == m_GameAccess->GetAddScoreState(i))
			cbBetedNum++;
	}

	// 加注完成，发牌
	printf("加注完成玩家数：%d\n", cbBetedNum);   //测试
	if (cbBetedNum == m_GameAccess->GetCurPlayerCount())
	{
		//发送倍数
		STR_CMD_SC_FANGJIAN_BET  fangjian_bet;
		ZeroMemory(&fangjian_bet, sizeof(STR_CMD_SC_FANGJIAN_BET));

		for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
		{
			fangjian_bet.room_bet[i] = m_GameAccess->GetAllBet(i);
		}
		//m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_FANGJIAN_BET, &fangjian_bet, sizeof(STR_CMD_SC_FANGJIAN_BET));


		//通知庄家出牌
		cout << "全部下注完成 处理出牌开始消息" << endl;

		WORD wBanker = m_GameLogic->GetAppointBanker();

		//设置第一轮最开始出牌用户
		m_GameAccess->SetStartOutCardUser(wBanker);

		//给庄家发送是否明牌的命令
		if (1 == m_GameAccess->GetPlayerMingPaiBet(wBanker) && 1 == m_GameAccess->GetMingPaiMode())
		{
			SendMingPaiStart(wBanker, MING_PAI_TYPE_OUTCARD);
		}

		// 下注定时器
		m_pITableFrame->KillGameTimer(IDI_OUTCARD_START);
		m_pITableFrame->SetGameTimer(IDI_OUTCARD_START, IDI_TIME_OUTCARD_START, 1, 0);
	}

	return;
}

//发送明牌开始消息
void CTableFrameSink::SendMingPaiStart(const WORD &wChairID, const BYTE &cbMPType)
{
	CLog::Log(log_debug, "【服务器】：通知用户 = %d【明牌开始】", wChairID);

	STR_CMD_SC_MING_PAI_START cmd;
	cmd.cbType = cbMPType;

	// 通知用户明牌开始
	m_pITableFrame->SendTableData(wChairID, CMD_SC_MING_PAI_START, &cmd, sizeof(STR_CMD_SC_MING_PAI_START));

	// 明牌定时器
	m_pITableFrame->KillGameTimer(IDI_MING_PAI);
	m_pITableFrame->SetGameTimer(IDI_MING_PAI, IDI_TIME_MING_PAI, 1, 0);
}

//明牌事件
void CTableFrameSink::On_Sub_UserMingPai(WORD wChairID, const BYTE &cbMPType, BYTE cbFlag, BYTE FlushCardBet)     //明牌类型	2-开始游戏明牌	1-出牌开始明牌  3-发牌明牌
{																												//是否明牌	0-不明牌	1-明牌    //发牌明牌的倍数  4 - 3 - 2
	//设置玩家明牌完成
	m_GameAccess->SetMingPaiState(wChairID, 1);

	//玩家明牌
	if (cbFlag == 1)
	{
		//构造明牌数据
		STR_CMD_SC_MING_PAI_RESULT MPResult;
		ZeroMemory(&MPResult, sizeof(STR_CMD_SC_MING_PAI_RESULT));	

		//明牌加倍
		if (MING_PAI_TYPE_GAMESTART == cbMPType)	//开始游戏明牌
		{
			//加倍
			MPResult.cbTimes = 5;
		}
		else if (MING_PAI_TYPE_OUTCARD == cbMPType)   //出牌名牌
		{
			MPResult.cbTimes = 2;	

			//记录玩家明牌倍数
			m_GameAccess->SetPlayerMingPaiBet(wChairID, 2);
		}
		else if (MING_PAI_TYPE_DEAL == cbMPType)   //发牌明牌
		{
			MPResult.cbTimes = (FlushCardBet == 1) ? 4 : (FlushCardBet == 2 ? 3 : 2);	

			//记录玩家明牌倍数
			m_GameAccess->SetPlayerMingPaiBet(wChairID, MPResult.cbTimes);
		}

		//赋值
		MPResult.wChairID = wChairID;

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

		BYTE msg_cbHandCard[MAX_CARD_COUNT] = {'\0'};

		m_GameAccess->GetClientHandCards(wChairID, msg_cbHandCard, MAX_CARD_COUNT);

		for (int i = 0; i < m_GameAccess->GetUserCurCardNum(wChairID); i++)
		{
			MPResult.cbHandCard[i] = msg_cbHandCard[i];
		}

		//广播玩家明牌消息
		m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_MING_PAI_RESULT, &MPResult, sizeof(STR_CMD_SC_MING_PAI_RESULT));
	}

	//计算明牌完成玩家数
	BYTE cbmingpaiNum = 0;
	BYTE cbPlayerNum = m_GameAccess->GetMaxChairCount();// 获取玩家数
	for (BYTE i = 0; i < cbPlayerNum; ++i)
	{
		if (1 == m_GameAccess->GetMingPaiState(i))
			cbmingpaiNum++;
	}
	printf("明牌完成玩家数：%d\n", (int)cbmingpaiNum);

	if ((cbmingpaiNum == m_GameAccess->GetCurPlayerCount()))
	{
		if (MING_PAI_TYPE_OUTCARD == cbMPType)
		{
			//给各玩家加上最大的明牌倍数
			WORD mingbet = 1;
			for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
			{
				if (i != wChairID && mingbet < m_GameAccess->GetPlayerMingPaiBet(i))
				{
					mingbet = m_GameAccess->GetPlayerMingPaiBet(i);
				}
			}
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
			cout << "全部明牌完成 处理抢庄开始消息" << endl;

			//给各玩家加上最大的明牌倍数
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
		}
	}
}

//处理出牌开始消息
void CTableFrameSink::HandleOutCardStart(const WORD &wOutCardUser)
{
	//删除下注定时器
	m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);

	//设置当前出牌玩家
	m_GameAccess->SetCurOutCardUser(wOutCardUser);

	//获取当前轮最大出牌数据和数目
	BYTE TurnCardNum = 0;
	BYTE TurnCardData[MAX_CARD_COUNT] = {'\0'};
	ZeroMemory(&TurnCardData, sizeof(TurnCardData));

	m_GameAccess->GetTurnMaxCards(TurnCardData, TurnCardNum);

	//广播出牌开始的消息
	STR_CMD_SC_OUT_CARD_START OutCardStart;
	ZeroMemory(&OutCardStart, sizeof(STR_CMD_SC_OUT_CARD_START));	

	OutCardStart.wOutCardUser = wOutCardUser;	

	//当前轮牌数据赋值
	/*
	OutCardStart.TurnOutCardNum = TurnCardNum;
	for (int i = 0; i < TurnCardNum; i++)
	{
		OutCardStart.TurnOutCardData[i] = TurnCardData[i];
	}*/

	//判断当前出牌回合是否结束
	CLog::Log(log_debug, "判断当前回合是否结束: %d(0-结束	1-未结束)", m_GameAccess->GetOneTurnEnd());         //0001-出牌  0010-不出  0100-提示  1000-要不起
	OutCardStart.ActionType = m_GameLogic->GetOutCardActionType(wOutCardUser);

	//广播出牌开始消息
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_USER_OUT_CARD_START, &OutCardStart, sizeof(STR_CMD_SC_OUT_CARD_START));

	//设置出牌游戏状态
	m_GameAccess->SetGameStatus(GS_WK_OUT_CARD);

	// 出牌定时器
	m_pITableFrame->KillGameTimer(IDI_OUT_CARD);
	//判断是否是机器人
	if (m_pITableFrame->IsRobot(wOutCardUser))
	{
		WORD index = (rand() % 4) + 1;
		m_pITableFrame->SetGameTimer(IDI_OUT_CARD, IDI_TIME_OUT_CARD*0.05*index, 1, 0);
	}
	else
	{
		//判断是否托管
		if (1 == m_GameAccess->GetPlayerTuoGuan(wOutCardUser))
		{
			m_pITableFrame->SetGameTimer(IDI_OUT_CARD, IDI_TIME_OUT_CARD*0.05, 1 , 0);
		}
		else
		{
			m_pITableFrame->SetGameTimer(IDI_OUT_CARD, IDI_TIME_OUT_CARD, 1, 0);
		}
	}
}

//出牌事件
void CTableFrameSink::On_Sub_UserOutCard(WORD wOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum, const bool &bPass)
{
	//游戏状态效验
	if ( GS_WK_OUT_CARD != m_GameAccess->GetGameStatus() ) 
		return;

	//数据校验
	if ((wOutCardUser != m_GameAccess->GetCurOutCardUser())||
		(MAX_CARD_COUNT < cbOutCardNum))
	{
		return;
	}

	//玩家过
	if (bPass)
	{
		//处理过流程
		HandleOutCardPass(wOutCardUser);
	}
	else
	{
		//处理出牌流程
		HandleOutCard(wOutCardUser, cbOutCard, cbOutCardNum);
	}
}

//处理用户出牌过
void CTableFrameSink::HandleOutCardPass(WORD wOutCardUser)
{
	//获得该玩家的下家
	WORD wNextUser = m_GameAccess->GetNextUser(wOutCardUser);

	// 删除出牌定时器
	m_pITableFrame->KillGameTimer(IDI_OUT_CARD);

	//设置玩家为出牌过	
	m_GameAccess->SetTurnOutCardType(wOutCardUser, 0);

	//获得一轮最后出牌的玩家
	WORD wLastUser = m_GameAccess->GetLastOutCardUser();

	//校验是不是轮到开始玩家了
	if ( wNextUser == wLastUser)
	{
		//先发送出牌结果包给客户端
		SendOutCardResult(wOutCardUser, NULL, 0, 0, OUT_CARD_SUCCESS, OUT_CARD_PASS);

		//是，则一轮结束
		HandleRoundEnd(wNextUser);
	}
	else
	{
		//先发送出牌结果包给客户端
		SendOutCardResult(wOutCardUser, NULL, 0, 0, OUT_CARD_SUCCESS, OUT_CARD_PASS);

		//否，通知下个玩家出牌开始
		HandleOutCardStart(wNextUser);
	}
}

//处理用户出牌
void CTableFrameSink::HandleOutCard(WORD wOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum)
{
	//校验 是否是当前玩家出牌
	if (wOutCardUser != m_GameAccess->GetCurOutCardUser())
		return;

	for (int i = 0; i < cbOutCardNum; i++)
		printf(" %d", cbOutCard[i]);
	cout << endl;

	//若玩家是一轮中最先开始出牌者
	WORD wStartUser = m_GameAccess->GetStartOutCardUser();

	//获得此轮最后出牌玩家
	WORD wLastUser = m_GameAccess->GetLastOutCardUser();

	//牌型
	int cbCardType = m_GameLogic->GetCardLogicType(cbOutCard, cbOutCardNum);    //使用BYTE导致越界

	//第一个出牌玩家
	if ((TURN_END == m_GameAccess->GetOneTurnEnd()) &&
		(wOutCardUser == wStartUser))
	{
		//牌型校验
		if (CT_ERROR == cbCardType)		  //出的空牌
		{
			//发送出牌结果
			SendOutCardResult(wOutCardUser, cbOutCard, cbOutCardNum, cbCardType, OUT_CARD_FAIL, OUT_CARD_NORMAL);  //出牌失败

			//牌型错误当前玩家继续出牌
			HandleOutCardStart(wOutCardUser);
			return;
		}
		else
		{
			m_GameAccess->SetOneTurnEnd(TURN_NOT_END);
		}
	}
	else	//后面出牌玩家
	{
		//校验出牌数目是否和此轮最后出牌玩家相同，出牌校验
		if ( /*(cbOutCardNum != m_GameAccess->GetUserOutCard(wLastUser, NULL)) || */
			!m_GameLogic->EfficacyOutCard(wOutCardUser, cbOutCard, cbOutCardNum))
		{
			//发送出牌结果
			SendOutCardResult(wOutCardUser, cbOutCard, cbOutCardNum, cbCardType, OUT_CARD_FAIL, OUT_CARD_NORMAL);

			//牌型错误当前玩家继续出牌
			HandleOutCardStart(wOutCardUser);
			return;
		}
	}

	// 删除出牌定时器
	m_pITableFrame->KillGameTimer(IDI_OUT_CARD);

	//从手牌中删除玩家扑克
	if (!m_GameLogic->RemoveCard(wOutCardUser, cbOutCard, cbOutCardNum))
	{
		return;
	}

	//春天判断条件记录
	m_GameAccess->SetSpring(wOutCardUser, 1);

	//癞子场癞子代替普通牌
	if (0 < m_GameAccess->GetLaiZiMode())
	{
		BYTE LaiZiData[MAX_LAIZI_COUNT] = { 0 };
		BYTE LaiZiCount;

		m_GameAccess->GetLaiZiToCommon(LaiZiData, LaiZiCount);

		int index = 0;
		for (int i = 0; i < cbOutCardNum; i++)
		{
			if (m_GameAccess->GetMagicCard() == m_GameLogic->GetCardLogicValueLaiZi(cbOutCard[i]) && INVALID_CARD != LaiZiData[index])
			{
				cbOutCard[i] = ((LaiZiData[index] >= 14) ? (LaiZiData[index] - 13) : LaiZiData[index]) + 16 * 5;
				index++;
			}
		}
		cout << endl;

		//将换好癞子的出牌排序
		m_GameLogic->SortCardListNoLaiZi(cbOutCard, cbOutCardNum, ST_ORDER);
	}

	//出牌记录
	m_GameAccess->SetUserOutCard(wOutCardUser, cbOutCard, cbOutCardNum);

	//该轮出牌最大数据记录
	m_GameAccess->SetTurnMaxCards(cbOutCard, cbOutCardNum);

	//设置玩家为出牌	
	m_GameAccess->SetTurnOutCardType(wOutCardUser, 1);

	//设置当前出牌玩家为此轮最后出牌玩家
	m_GameAccess->SetLastOutCardUser(wOutCardUser);

	//设置下一个出牌玩家
	WORD wNextUser = m_GameAccess->GetNextUser(wOutCardUser);

	//设置玩家炸弹信息
	if ((cbCardType&(1 << CT_BOMB_CARD)) != 0 ||
		(cbCardType&(1 << CT_MISSILE_CARD)) != 0 ||
		(cbCardType&(1 << CT_LAIZI_BOMB)) != 0 ||
		(cbCardType&(1 << CT_RUAN_BOMB)) != 0)
	{
		m_GameAccess->SetUserBoomInfo(wOutCardUser, cbCardType);
	}

	//发送出牌结果
	SendOutCardResult(wOutCardUser, cbOutCard, cbOutCardNum, cbCardType, OUT_CARD_SUCCESS, OUT_CARD_NORMAL);

	if (m_GameAccess->GetUserCurCardNum(wOutCardUser) == 0)
	{
		//一轮结束
		m_GameAccess->SetCurOutCardUser(INVALID_CHAIR);
		HandleRoundEnd(wOutCardUser);
	}
	else
	{
		//通知下个玩家出牌开始
		m_GameAccess->SetCurOutCardUser(wNextUser);
		HandleOutCardStart(wNextUser);
	}
}

//判断玩家是否能出牌
bool CTableFrameSink::JudgeUserOutCard(const WORD &wLastOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum)
{
	return true;
}

//发送出牌结果
void CTableFrameSink::SendOutCardResult(WORD wOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum, 
										 const int &cbCardType, const BYTE &cbSuccess, const BYTE &cbFlag)
{
	//构造出牌数据
	STR_CMD_SC_OUT_CARD_RESULT OutCard;
	ZeroMemory(&OutCard, sizeof(STR_CMD_SC_OUT_CARD_RESULT));

	//获取当前玩家手牌数量和手牌数据
	BYTE cbCurCardCount = m_GameAccess->GetUserCurCardNum(wOutCardUser);
	BYTE cbCurCardData[MAX_CARD_COUNT] = {'\0'};
	m_GameAccess->GetClientHandCards(wOutCardUser, cbCurCardData, cbCurCardCount);

	//赋值
	OutCard.wOutCardUser = wOutCardUser;
	OutCard.cbFlag = cbFlag;
	OutCard.cbSuccess = cbSuccess;
	OutCard.cbHandCardNum = cbCurCardCount;
	OutCard.cbIsOneTurnEnd = m_GameAccess->GetOneTurnEnd();
	if (m_GameAccess->GetPlayerMingPaiBet(wOutCardUser) > 1)
		OutCard.cbIsmingpai = 1;

	for (int i = 0; i < m_GameAccess->GetMaxChairCount(); i++)
	{
		if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
		{
			OutCard.RoomBet[i] = m_GameAccess->GetAllBet(i);

			//把记牌器发给客户端缓存
			//OnUserJiPaiQi(i);
		}
	}

	//赋值当前手牌数据
	for (int i = 0; i < cbCurCardCount; i++)
	{
		OutCard.handcarddata[i] = cbCurCardData[i];
	}


	//正常出牌
	if (1 == cbFlag)
	{
		if (0 == cbSuccess)
		{
			m_pITableFrame->SendTableData(wOutCardUser, CMD_SC_USER_OUT_CARD_RESULT, &OutCard, sizeof(STR_CMD_SC_OUT_CARD_RESULT));
			return;
		}
		else
		{
			//赋值					
			OutCard.cbOutCardNum = cbOutCardNum;	
			memcpy(OutCard.cbOutCard, cbOutCard, sizeof(BYTE)*cbOutCardNum);	
			OutCard.cbHandCardNum = m_GameAccess->GetUserCurCardNum(wOutCardUser);	
			
			for (int i = 14; i >= 0; i--)
			{
				if ((cbCardType & (1 << i)) != 0)
				{
					if (i == CT_THREE_LINE_TAKE_ONE || i == CT_THREE_LINE_TAKE_TWO)
					{
						if (cbOutCardNum > 5)	
						{
							OutCard.cbCardType = CT_FEIJI;
							break;
						}
					}

					OutCard.cbCardType = i;	//牌型
					CLog::Log(log_debug, "牌型: %d", OutCard.cbCardType);
					break;
				}
			}

			//CLog::Log(log_debug, "cbCardType2: %d", OutCard.cbCardType);

			//得分
		/*	BYTE cbPlayerNum = m_GameAccess->GetMaxChairCount();
			for (BYTE i = 0; i < cbPlayerNum; ++i)
			{
				if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
				{
					OutCard.lSingleGameScore[i] = m_GameAccess->GetPlayerSingleScore(i);
				}
			}*/
		}
	}
	else	//玩家要不起
	{
		OutCard.cbFlag = cbFlag;	
	}

	//录像出牌结果
	//AddRecordOutCardResult(OutCard);
	//广播出牌结果
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_USER_OUT_CARD_RESULT, &OutCard, sizeof(STR_CMD_SC_OUT_CARD_RESULT));
	CLog::Log(log_debug, "【广播玩家 = %d的出牌结果】", OutCard.wOutCardUser);	

}

//处理一轮出牌结束
void CTableFrameSink::HandleRoundEnd(const WORD &wWinner)
{		
	//计算一轮得分
	WORD wScore = m_GameLogic->SettleOneRound(wWinner);

	//设置一轮结束
	m_GameAccess->SetOneTurnEnd(TURN_END);

	//清空玩家是出牌还是过
	for (int i = 0; i < 3; i++)
	{
		m_GameAccess->SetTurnOutCardType(i, 99);
	}

	//设置开始的出牌玩家，清空该轮的出牌数据
	if ( 0 != m_GameAccess->GetUserCurCardNum(wWinner) )
	{
		//设置该轮最开始出牌用户
		m_GameAccess->SetStartOutCardUser(wWinner);

		//设置当前出牌用户
		m_GameAccess->SetCurOutCardUser(wWinner);

		//通知赢家出牌
		HandleOutCardStart(wWinner);

		CLog::Log(log_debug, "【玩家还有牌，一轮结束，通知赢家 = %d出牌开始】", wWinner);
	}
	else	//玩家所有牌出完
	{
		CLog::Log(log_debug, "【游戏结束】");

		//设置赢家
		m_GameAccess->SetLastGameWinner(wWinner);

		//记录赢家
		m_GameAccess->SetWinSum(wWinner, 1);

		m_GameAccess->SetCurOutCardUser(INVALID_CHAIR);

		//判断玩家是否春天
		m_GameAccess->JudgeSpring(wWinner);

		//算分
		m_GameLogic->Settle();

		//一轮结束，当前赢家牌全部出完，游戏结束
		OnEventGameConclude(GER_NORMAL);
	}
}

//发送指定庄家消息
void CTableFrameSink::SendAppointBanker(const WORD &wBanker)
{
	//用户校验
	if ( wBanker >= m_GameAccess->GetMaxChairCount() )
		return;

	//构造数据
	STR_CMD_SC_APPOINT_BANKER AppointBanker;
	ZeroMemory(&AppointBanker, sizeof(STR_CMD_SC_APPOINT_BANKER));

	//赋值
	AppointBanker.wBanker = wBanker;

	//录像指定庄家
	//AddRecordAppointBanker(AppointBanker);

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_APPOINT_BANKER, &AppointBanker, sizeof(STR_CMD_SC_APPOINT_BANKER));
}

//游戏结束的获取公共倍数信息
void CTableFrameSink::OnUserPublicBet(WORD wChairID)
{
	//椅子号校验
	if (wChairID < 0 || wChairID >= m_GameAccess->GetMaxChairCount())
	{
		//CLog::Log(log_error, "func OnUserPublicBet err : wChairID < 0 && wChairID >= m_GameAccess->GetMaxChairCount(): chairID:%d", wChairID);
		return;
	}

	//构造信息
	STR_CMD_SC_PUBLIC_BET public_bet;
	ZeroMemory(&public_bet, sizeof(STR_CMD_SC_PUBLIC_BET));

	WORD _playernum = m_GameAccess->GetCurPlayerCount();
	WORD LeaveCard_Bet = 1; //底牌倍数
	WORD Spring_Bet = 1;	//春天倍数
	WORD Mingpai_Bet = 1;  //明牌倍数
	WORD Boom_Num = 0;   //炸弹总数目
	WORD Boom_Bet = 1;	 //炸弹总倍数
	WORD Rob_Num = 0;    //抢庄次数
	WORD Rob_bet = 1;    //抢庄倍数
	WORD public_Bet = 0;  //公共倍数
	WORD cell_score = m_GameAccess->GetCellScore();
	tagBoomInfo  boominfo;  //炸弹信息
	WORD Rule = m_GameAccess->GetRobBankMode();  //获取游戏规则

	//赋值
	public_bet.RoomBet = 1;	//底分
	public_bet.IUserAScore = m_GameAccess->GetAllBet(wChairID);   //总分
	public_bet.Game_State = m_GameAccess->GetGameStatus();		//游戏状态

	//底牌倍数
	LeaveCard_Bet = m_GameAccess->GetLeaveCardBet();
	public_bet.LeaveCard = LeaveCard_Bet;

	//春天倍数
	for (int i = 0; i < _playernum; i++)
	{
		WORD flag = m_GameAccess->GetSpring(i);
		if (1 == flag)
		{
			Spring_Bet = 2;
		}
	}
	public_bet.Spring = Spring_Bet;


	if (m_GameLogic->GetAppointBanker() < 0 || m_GameLogic->GetAppointBanker() > m_GameAccess->GetMaxChairCount())	//判断是否抢完了地主
		public_bet.bank_ID = -1;    //庄家ID
	else
		public_bet.bank_ID = m_GameLogic->GetAppointBanker();    //庄家ID

	//加倍
	for (int i = 0; i < m_GameAccess->GetCurPlayerCount(); i++)
	{
		public_bet.AddScore[i] = m_GameAccess->GetPlayerAddScore(i);    //各玩家的加倍信息
		Boom_Num += m_GameAccess->GetUserBoomInfo(i, boominfo);			//获取炸弹总数
		Rob_Num += m_GameAccess->GetUserRobNum(i);						//获取总共抢庄次数
	}

	//由规则来判断抢庄部分的倍数
	if (Rule == 0)
	{
		for (int i = 0; i < Rob_Num - 1; i++)    //计算抢庄的倍数
		{
			Rob_bet *= 2;    // 2的4次方倍
		}
	}
	else		//叫分倍数算最大的那一个
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
	public_bet.Rob = Rob_bet;	//抢地主或叫分
	public_bet.Rule = Rule;

	//明牌倍数取最大的那一个
	Mingpai_Bet = 0;
	for (int i = 0; i < _playernum; i++)
	{
		if (Mingpai_Bet < m_GameAccess->GetPlayerMingPaiBet(i))
		{
			Mingpai_Bet = m_GameAccess->GetPlayerMingPaiBet(i);
		}
	}

	public_bet.MingPai = Mingpai_Bet;

	//公共倍数
	if (Boom_Num == 0)
		if (Mingpai_Bet == 0)
			public_Bet = cell_score *Rob_bet * LeaveCard_Bet* Spring_Bet;  //公共倍数 = 初始*明牌*抢地主*底牌*炸弹*春天
		else
			public_Bet = cell_score *Rob_bet * Mingpai_Bet * LeaveCard_Bet * Spring_Bet;
	else
		if (Mingpai_Bet == 0)
			public_Bet = cell_score *Rob_bet  * Boom_Bet * LeaveCard_Bet * Spring_Bet;  //公共倍数 = 初始*明牌*抢地主*底牌*炸弹8春天
		else
			public_Bet = cell_score *Rob_bet * Mingpai_Bet * Boom_Bet * LeaveCard_Bet * Spring_Bet;

	//判断是否封顶
	if (m_GameAccess->GetRoomMaxBet() < public_Bet)
	{
		public_bet.public_bet = m_GameAccess->GetRoomMaxBet();
	}
	else
	{
		public_bet.public_bet = public_Bet;
	}

	CLog::Log(log_debug, "cell: %d rob: %d  Boom: %d  Mingpai: %d LeaveCard: %d public:%d\n",cell_score, Rob_bet, Boom_Bet, Mingpai_Bet, LeaveCard_Bet, public_Bet);

	//m_pITableFrame->SendTableData(wChairID, CMD_SC_PUBLIC_BET, &public_bet, sizeof(STR_CMD_SC_PUBLIC_BET));
}

//处理客户端发来的记牌器消息
void CTableFrameSink::OnUserJiPaiQi(WORD wChairID)
{
	//状态校验
	if (GS_WK_FREE == m_GameAccess->GetGameStatus())
	{
		cout << "OnUserJiPaiQi : err" << endl;
		return;
	}

	//构建消息
	STR_CMD_SC_JIPAIQI_START  JiPaiQi;
	ZeroMemory(&JiPaiQi, sizeof(STR_CMD_SC_JIPAIQI_START));

	//定义变量
	BYTE CardREC[15];
	tagCardRecorder	CardRec;
	ZeroMemory(&CardREC, sizeof(CardREC));
	ZeroMemory(&CardRec, sizeof(CardRec));

	//获得各玩家的卡牌数目
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

//托管
bool CTableFrameSink::PlayerTuoGuan(WORD wChairID)
{
	////状态校验
	//if (GS_WK_OUT_CARD != m_GameAccess->GetGameStatus())
	//{
	//	cout << "GS_WK_OUT_CARD != m_GameAccess->GetGameStatus() : err ：托管" << endl;
	//	return false;
	//}

	if (wChairID > m_GameAccess->GetMaxChairCount())
		return false;

	//设置托管
	m_GameAccess->SetPlayerTuoGuan(wChairID, 1);

	//设置抢庄定时器
	if (m_GameAccess->GetCurRobUser() == wChairID && GS_WK_ROB == m_GameAccess->GetGameStatus())
	{
		m_pITableFrame->KillGameTimer(IDI_ROB_BANKER);
		m_pITableFrame->SetGameTimer(IDI_ROB_BANKER, IDI_TIME_ROB_BANKER*0.1, 1 , 0);
	}

	//设置出牌定时器
	if (m_GameAccess->GetCurOutCardUser() == wChairID && GS_WK_OUT_CARD == m_GameAccess->GetGameStatus())
	{
		m_pITableFrame->KillGameTimer(IDI_OUT_CARD);
		m_pITableFrame->SetGameTimer(IDI_OUT_CARD, IDI_TIME_OUT_CARD*0.05, 1, 0);
	}

	return true;
}

//取消托管
bool CTableFrameSink::PlayerCancelTuoGuan(WORD wChairID)
{
	if (wChairID > m_GameAccess->GetMaxChairCount())
		return false;

	//取消托管
	m_GameAccess->SetPlayerTuoGuan(wChairID, 0);

	return true;
}

/*************************************************
@Description:     初始化录像数组，获得个数据的索引及大小
@Input:           无
@Output:          无
@Return:          无
@author & data:	lizhihu 2017.11.2
*************************************************/
void CTableFrameSink::StartRecord()
{
}

///*************************************************
//@Description:     结束录像，释放内存
//@Input:           无
//@Output:          无
//@Return: lD#鸛kQ?鋠休0談9尞E?/@牽b鰖?鴅j?摀媺F?p_界碒K5?2?os+羉巒-?$? q铭w?鎥l欼扎??猄b埾姡硝狻^bcVI橦撀?千"p8菀爬强獁?=硬馵盬狠堘寕2Va撽D严.屝癅??节6稇p舩z
//`t?宱PT<~0M9诀
//g

////////////////////////////////////////////////////////////////////////////////////
//导出定义

extern "C" __declspec(dllexport) VOID * CreateTableFrameSink()
{
	CTableFrameSink *pTableFrameSink = NULL;
	try
	{
		pTableFrameSink = new CTableFrameSink();
		if (pTableFrameSink == NULL)
		{
			throw TEXT("创建失败");
		}

		return pTableFrameSink;
	}
	catch (...) {}

	//清理对象
	SafeDelete(pTableFrameSink);
	return NULL;
}

extern "C" __declspec(dllexport) VOID  FreeTableFrameSink(VOID *p)
{
	CTableFrameSink *pTableFrameSink = (CTableFrameSink *)p;
	CLog::Log(log_debug, "pTableFrameSink: %d", pTableFrameSink);
	if (pTableFrameSink != NULL)
	{
		SafeDelete(pTableFrameSink);
	}
}

//读取配置文件
extern "C" __declspec(dllexport) DWORD ReadSubCfgConfig()
{
	//从配置文件读取游戏配置
	int ret = CGameConfig::LoadGameCommonConfig();
	if (ret != 0)
		return 0;

	return KIND_ID;
}

////////////////////////////////////////////////////////////////////////////////////