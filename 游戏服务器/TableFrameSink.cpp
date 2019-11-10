#include "StdAfx.h"
#include "TableFrameSink.h"
#include "GameServerManager.h"
#include "SubGameRule.h"


// 构造函数
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

	m_pGameServiceOption = NULL;
	m_pGameServiceAttrib = NULL;

	m_pRoomRuleOption = NULL;
}

// 接口查询--检测相关信息版本
VOID * CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

// 初始化
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL)
	{
		return false;
	}
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);	//所有人准备开始

	//游戏配置
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();

	//房卡配置
	m_pRoomRuleOption= (tagGameRoomItem*)m_pITableFrame->GetCustomRule();

	//校验
	if (NULL != m_GameLogic)
	{
		delete m_GameLogic;
		m_GameLogic  = NULL;
		m_GameAccess = NULL; 
	}
	
	return true;
}

// 复位桌子
VOID  CTableFrameSink::RepositionSink()
{
	if ( NULL != m_GameAccess )
	{
		m_GameAccess->SetGameState( GAME_STATUS_FREE ); // 等待开始
	}
}

// 小局游戏结束
bool CTableFrameSink::XjGameConclude(int nTotalGameCount, int nCurGameCount)
{
	// 玩家数目
	BYTE _playersum = m_GameAccess->GetMaxChairCount(); 

	// 构建数据
	STR_CMD_SC_XJ_GAME_END sXJGameEnd;
	ZeroMemory(&sXJGameEnd, sizeof(STR_CMD_SC_XJ_GAME_END));

	//获得底牌
	m_GameAccess->GetLeaveCard(sXJGameEnd.cbLeaveCard,MAX_LEAVE_CARD_NUM);
	
	//获得玩家单局游戏得分
	for (BYTE i = 0; i < _playersum && m_GameAccess != NULL; ++i)
	{
		if ( USER_PLAYING == m_GameAccess->GetPlayerState(i) )
		{
			//直接将玩家总积分发给客户端，避免客户端计算
			sXJGameEnd.nTotalGameScore[i]  = m_GameAccess->GetPlayerTotalScore(i);
			//玩家单局得分
			sXJGameEnd.nSingleGameScore[i] = m_GameAccess->GetPlayerSingleScore(i);	

			printf("\n赢家：%d，单局得分 = %lld，总得分 = %lld\n",i, sXJGameEnd.nSingleGameScore[i], sXJGameEnd.nTotalGameScore[i]);
		}		
	}

	//第一局游戏结束后扣除房卡		TODO 可能需要增加其他模式的游戏
	BYTE cbCurGameCount = m_GameAccess->GetCurGameCount();
	m_pITableFrame->HandleXJGameEnd(cbCurGameCount, TABLE_MODE_FANG_KA, sXJGameEnd.nSingleGameScore);		//扣除房卡

	//小局数据写入数据库
	WriteGameRecord();

	//最后一局不发送小局游戏结束，客户端不显示小局结算页面
	if ( cbCurGameCount != m_GameAccess->GetAllCount() )
	{
		//发送信息
		m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_XJ_GAME_END, &sXJGameEnd, sizeof(sXJGameEnd));
		//m_pITableFrame->SendLookonData(INVALID_CHAIR, CMD_SC_XJ_GAME_END, &sXJGameEnd, sizeof(sXJGameEnd));

		printf("\n【发送小局游戏结束】\n");

		//设置游戏状态为小局结束状态
		m_pITableFrame->SetGameStatus(GS_WK_XJ_GAMEEND);

		//设置小局游戏继续（用户准备）的定时器
		m_pITableFrame->KillGameTimer(IDI_XJGAME_USER_READY);
		m_pITableFrame->SetGameTimer(IDI_XJGAME_USER_READY, IDI_TIME_XJGAME_USER_READY, 1, 0);

		// 清理游戏数据
		m_GameLogic->ClearXjGame();
	}

	return true;
}

// 大局游戏结束
bool CTableFrameSink::DjGameConclude(int nTotalGameCount, int nCurGameCount)
{
	printf("大局结束\n");

	// 构造数据
	STR_CMD_SC_DJ_GAME_END  sDJGameEnd;
	memset(&sDJGameEnd, 0, sizeof(sDJGameEnd));
	sDJGameEnd.wFangzhuID = m_GameAccess->GetRoomFangzhu();

	//赢次数
	int nMaxWinNum = 0;
	
	// 玩家数目
	BYTE _playersum = m_GameAccess->GetCurPlayerCount(); 
	for (BYTE i = 0; i < _playersum; ++i)
	{
		if ( USER_PLAYING == m_GameAccess->GetPlayerState(i) )
		{
			sDJGameEnd.lUserScore[i] = m_GameAccess->GetPlayerTotalScore(i);
			m_GameAccess->GetWinSum(i, sDJGameEnd.bWinTime[i]);

			//获得大赢家（最大赢次数）
			if ( sDJGameEnd.bWinTime[i] > nMaxWinNum )
			{
				nMaxWinNum = sDJGameEnd.bWinTime[i];
				sDJGameEnd.wMaxWinChairID = i;
			}
			printf("\n玩家ID = %d，玩家得分 = %d，赢局数 = %d\n", i, sDJGameEnd.lUserScore[i], sDJGameEnd.bWinTime[i]);
		}
	}

	// 广播大局结束消息
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_DJ_GAME_END, &sDJGameEnd, sizeof(sDJGameEnd));
	//m_pITableFrame->SendLookonData(INVALID_CHAIR,CMD_SC_DJ_GAME_END, &Conclude,sizeof(Conclude));

	//设置游戏为【空闲】状态
	m_pITableFrame->SetGameStatus(GS_WK_FREE);

	//直接结束游戏，不要续费
	m_pITableFrame->ConcludeGame(GAME_CONCLUDE_NORMAL);

	// 清理大局数据
	m_GameLogic->ClearDjGame();

	//大局结束，清空内存 -- 如果 游戏结束标志位 GAME_CONCLUDE_CONTINUE, 这里就不能清空内存
	m_GameLogic->ClearRoom();

	return true;
}

// 游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//关闭摊牌定时器
	m_pITableFrame->KillGameTimer(IDI_OUT_CARD);

	// 游戏当前局数
	BYTE cbCurGameCount = m_GameAccess->GetCurGameCount();

	// 总局数
	WORD cbAllGameCount = m_GameAccess->GetAllCount();

	// 玩家数目
	BYTE cbPlayersum = m_GameAccess->GetMaxChairCount(); 

	switch (cbReason)
	{
	case GER_DISMISS:			// 解散房间
		{
			printf("\n解散房间\n");

			// 发送解散消息
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

			//广播数据
			m_pITableFrame->SendTableData(INVALID_CHAIR,CMD_SC_DJ_GAME_END, &sGameDismiss, sizeof(sGameDismiss));
		
			m_pITableFrame->ConcludeGame(cbReason);

			// 清理数据
			m_GameLogic->ClearRoom();

			//释放内存
			delete m_GameLogic;
			m_GameLogic = NULL;
			m_GameAccess = NULL;

			break;
		}
	case GER_NORMAL:			//正常结束
		{
			// 小局结束
			if (cbCurGameCount < cbAllGameCount) 
			{
				XjGameConclude(cbAllGameCount, cbCurGameCount);
			}
			else if (cbCurGameCount == cbAllGameCount)		// 大局结束
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

// 断线重连时发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
		case GS_WK_FREE:		//空闲状态
		{
			//如果房间配置为空，获取房间配置
			if ( NULL == m_pRoomRuleOption )
			{
				m_pRoomRuleOption = (tagGameRoomItem*)m_pITableFrame->GetCustomRule();
			}

			//空闲状态
			STR_CMD_SC_STATUS_FREE StatusFree;
			ZeroMemory( &StatusFree, sizeof( StatusFree ) );

			// 设置变量
			//StatusFree.GameCount = m_pRoomRuleOption->GameCount;
			//StatusFree.PlayerCount = m_pRoomRuleOption->PlayerCount;
			//StatusFree.CellScore = m_pRoomRuleOption->CellScore;
			//StatusFree.FangZhu = m_pRoomRuleOption->FangZhu;

			// 发送场景
			return m_pITableFrame->SendGameScene( pIServerUserItem, &StatusFree, sizeof(StatusFree));
		}
		case GS_WK_FANZHU:		//反主状态
		{
			STR_CMD_SC_STATUS_FANZHU StatusFanzhu;
			ZeroMemory(&StatusFanzhu, sizeof(STR_CMD_SC_STATUS_FANZHU));

			//反主数据
			StatusFanzhu.wCurFanzhuUser = m_GameAccess->GetCurFanzhuUser();
			m_GameLogic->AnalyseUserFanZhu(wChairID, StatusFanzhu.cbFanZhuCard);
			StatusFanzhu.cbLastTime = m_pITableFrame->GetTimerLeftTickCount(IDI_FAN_ZHU) / 1000;

			// 发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusFanzhu, sizeof(STR_CMD_SC_STATUS_FANZHU));
		}
		case GS_WK_KOUDI:		//扣底状态
		{
			STR_CMD_SC_STATUS_KOUDI StatusKoudi;
			ZeroMemory(&StatusKoudi, sizeof(STR_CMD_SC_STATUS_KOUDI));

			//扣底数据
			StatusKoudi.wCurKouDiUser = m_GameAccess->GetCurKoudiUser();
			m_GameAccess->GetClientHandCards(wChairID, StatusKoudi.cbSortedCard, MAX_CARD_COUNT);
			StatusKoudi.cbHandCardNum = m_GameAccess->GetUserCurCardNum(wChairID);		
			StatusKoudi.cbMainColorNum = m_GameAccess->GetUserMainColorNum(wChairID);
			StatusKoudi.cbMainColor = m_GameAccess->GetMainColor();

			// 发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusKoudi, sizeof(STR_CMD_SC_STATUS_KOUDI));
		}
		case GS_WK_OUT_CARD:	//出牌状态
		{
			STR_CMD_SC_STATUS_OUTCARD StatusOutCard;
			ZeroMemory(&StatusOutCard, sizeof(STR_CMD_SC_STATUS_OUTCARD));

			//出牌数据
			StatusOutCard.wCurOutCardUser = m_GameAccess->GetCurOutCardUser();
			int nPlayerNum = m_GameAccess->GetMaxChairCount();
			for (int i = 0; i < nPlayerNum; i++)
			{
				if (USER_PLAYING == m_GameAccess->GetPlayerState(i))
				{
					m_GameAccess->GetUserOutCard(i, StatusOutCard.cbOutCard[i]);
				}
			}

			// 发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusOutCard, sizeof(STR_CMD_SC_STATUS_OUTCARD));
		}
		case GS_WK_XJ_GAMEEND:
		{
			STR_CMD_SC_STATUS_XJ_END StatusXJEnd;
			ZeroMemory(&StatusXJEnd, sizeof(STR_CMD_SC_STATUS_XJ_END));

			//获得玩家单局游戏得分
			int nPlayerNum = m_GameAccess->GetMaxChairCount();				
			for (BYTE i = 0; i < nPlayerNum && m_GameAccess != NULL; ++i)
			{
				if ( USER_PLAYING == m_GameAccess->GetPlayerState(i) )
				{
					//玩家单局得分
					StatusXJEnd.nSingleGameScore[i] = m_GameAccess->GetPlayerSingleScore(i);	

					printf("\n赢家：%d，单局得分 = %lld\n",i, StatusXJEnd.nSingleGameScore[i]);
				}		
			}

			return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusXJEnd, sizeof(STR_CMD_SC_STATUS_XJ_END));
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

			//没有叫庄,默认不叫
			for (WORD i = 0; i < m_GameAccess->GetMaxChairCount() && m_GameAccess != NULL; i++)
			{
				if ( (USER_PLAYING == m_GameAccess->GetPlayerState(i)) 
					&& (0 == m_GameAccess->GetBankerState(i)) )
				{        
					printf("\n定时器到：默认不抢庄\n");
					OnUserCallBanker(i, 0, 0);
				}
			}
			break;
		}
		case IDI_XJGAME_USER_READY:  // 小局结束用户准备定时器
		{
			m_pITableFrame->KillGameTimer(IDI_XJGAME_USER_READY);
			printf("\n【服务器】：定时器到，进入【小局开始】\n");
			//没有准备，则默认全部准备，开始游戏
			for (WORD i = 0; i < m_GameAccess->GetMaxChairCount() && m_GameAccess != NULL; i++)
			{
				if ( USER_PLAYING == m_GameAccess->GetPlayerState(i) 
					&& (0 == m_GameAccess->GetXjReadyState(i)) )
				{
					printf("\n【服务器】：定时器到，接受【小局开始】 玩家：%d\n", i);
					//用户准备
					OnUserXjGameReady(i, 1);
				}
			}
			break;
		}
		case IDI_ROUND_END_DELAY:	//一轮结束延迟定时器
		{
			m_pITableFrame->KillGameTimer(IDI_ROUND_END_DELAY);

			//延迟给玩家发送一轮结束
			WORD cbOutCardNum = (WORD)wBindParam;

			//一轮结束，通知赢家出牌开始
			HandleRoundEnd(cbOutCardNum);
			break;
		}
		default:
			return false;
	}
	
	return true;
}

// 游戏消息处理
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//消息处理
	switch (wSubCmdID)
	{
		case SUB_CS_OUT_CARD:		//出牌
		{
			//效验数据
			if (wDataSize != sizeof(STR_SUB_CS_OUT_CARD) )
				return true;

			//变量定义
			STR_SUB_CS_OUT_CARD *pOutCard= (STR_SUB_CS_OUT_CARD *)pData;
			if ( pOutCard->wOutCardUser == pIServerUserItem->GetChairID() )
			{
				bool bPass = (pOutCard->cbPass == 1) ? true : false;
				On_Sub_UserOutCard(pOutCard->wOutCardUser, pOutCard->cbOutCard, pOutCard->cbOutCardNum, bPass);
			}			
			break;	
		}
		case SUB_CS_ROB_BANKER:		 //抢庄命令
		{
			//效验数据
			if (wDataSize != sizeof(CMD_C_CallBanker))
			{
				return true;//舍弃消息 也表示处理
			}

			//变量定义
			STR_SUB_CS_ROB_BANKER *pCallBanker = (STR_SUB_CS_ROB_BANKER *)pData;
			OnUserCallBanker(pIServerUserItem->GetChairID(), pCallBanker->cbType, pCallBanker->cbRobState);

			break;
		}
		case SUB_CS_ADD_SCORE:	 //用户下注
		{
			//校验数据
			if (wDataSize != sizeof(STR_SUB_CS_ADD_SCORE))
				return true;

			STR_SUB_CS_ADD_SCORE *pAddScore = (STR_SUB_CS_ADD_SCORE *)pData;

			//处理
			OnUserAddScore(pIServerUserItem->GetChairID(), pAddScore->score);

			break;
		}
		case SUB_CS_MING_PAI:	//明牌
		{
			//校验数据
			if (wDataSize != sizeof(STR_SUB_CS_MING_PAI))
				return true;

			STR_SUB_CS_MING_PAI *pMingPai = (STR_SUB_CS_MING_PAI *)pData;

			On_Sub_UserMingPai(pIServerUserItem->GetChairID(), pMingPai->cbIsMing, pMingPai->cbType);
		}
		case SUB_CS_XJ_GAME_READY:       //玩家小结算准备
		{
			//校验数据
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

// 用户坐下
bool CTableFrameSink::OnActionUserSitDown( WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser )
{
	if( bLookonUser == false )	
	{		
		m_dPlayerState[wChairID] = USER_SITDOWN;
	}

	return true;

}

// 用户起立
bool CTableFrameSink::OnActionUserStandUp( WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if( bLookonUser == false )
		m_dPlayerState[wChairID] = USER_STANDUP;

	return true;
}

// 用户准备
bool CTableFrameSink::OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{ 
	printf("\n【用户游戏状态 = %d】\n", pIServerUserItem->GetUserStatus());
	if ( pIServerUserItem->GetUserStatus() == USER_SITDOWN )
	{
		//从框架获得大局游戏用户状态
		m_dPlayerState[wChairID] = USER_ONREADY;
	}

	return true; 
}

// 大局游戏开始				
bool CTableFrameSink::OnEventGameStart()
{
	// 创建游戏，指针赋值 -- 此处的m_GameAccess与m_GameLogic为同一个对象，  与m_GameData不是同一个对象
	m_GameData.CreateGame( &m_GameLogic, &m_GameAccess );

	// 初始化房间-设置房间规则
	if ( !m_GameAccess->SetRoomRule( m_pRoomRuleOption, sizeof(tagGameRoomItem)) )
		return false;

	// 加载读取的配置文件参数和创建日志,铁岭麻将-经典模式		
	if ( 0 != m_GameLogic->LoadConfigParameter(L"MJ_GAMES_LOG.txt", GAME_SCORE_MODE_CLASSIC) )
	{
		//未成功初始化,退出
		return false;
	}

	//设置用户准备状态，将框架传来的用户状态付给子游戏变量
	int nReadyNum = 0;
	for (int i = 0; i < m_GameAccess->GetMaxChairCount(); i++)
	{
		if ( USER_ONREADY == m_dPlayerState[i] )
		{
			m_GameAccess->SetPlayerState(i, USER_ONREADY);
			nReadyNum++;
		}
	}
	
	//大于最小人数游戏开始
	if ( MIN_CHAIR_COUNT <= nReadyNum )	
	{
		StartGame();
	}
	
	return true;
}

// 开始游戏
void CTableFrameSink::StartGame()
{
	printf("\n【开始游戏】\n");

	// 设置玩家状态【游戏中】
	BYTE cbPlayerNum = m_GameAccess->GetMaxChairCount();
	for ( BYTE i = 0; i < cbPlayerNum; ++i)
	{	
		//大局结束时，用户状态为准备；小局游戏结束时，用户状态为游戏中。所以要用>=
		if( USER_ONREADY <= m_GameAccess->GetPlayerState(i) )
		{
			m_GameAccess->SetPlayerState( i, USER_PLAYING );
		}
	}

	// 游戏局数加1
	BYTE cbGameCout = m_GameAccess->GetCurGameCount();
	m_GameAccess->SetGameCount( ++cbGameCout );

	//给玩家发牌
	HandleDeal();

	//处理抢庄（抢地主）
	HandleRobBanker();
}

//处理发牌流程
void CTableFrameSink::HandleDeal()
{
	// 洗牌并发牌
	m_GameLogic->Deal();

	//遍历玩家
	BYTE cbMaxChairCount = m_GameAccess->GetMaxChairCount();
	for (WORD i = 0; i < cbMaxChairCount; i++)
	{
		if ( USER_PLAYING == m_GameAccess->GetPlayerState(i) )
		{
			// 构造发牌数据
			STR_CMD_SC_SEND_CARD SendCard;
			ZeroMemory(&SendCard, sizeof(STR_CMD_SC_SEND_CARD));

			//赋值	
			SendCard.wChairID = i;	
			int _cardsum = m_GameAccess->GetPlayerInitCardNum();
			m_GameAccess->GetClientHandCards(i, SendCard.cbHandCardData, _cardsum);
			SendCard.cbHandCardNum = m_GameAccess->GetUserCurCardNum(i);

			// 发牌给对应客户端 -- 每个客户端应该只能收到自己的牌
			m_pITableFrame->SendTableData(i, CMD_SC_SEND_CARD, &SendCard, sizeof(STR_CMD_SC_SEND_CARD));
		}
	}
}

//处理抢庄
void CTableFrameSink::HandleRobBanker()
{
	// 设置游戏【亮主状态】
	m_pITableFrame->SetGameStatus(GS_WK_ROB);

	//根据房间规则判断抢庄模式
	BYTE cbRobBankMode = m_GameAccess->GetRobBankMode();

	//1、固定庄和轮庄：指定庄家，直接发送【下注开始】	
	if ( (ROOMRULE_OPTION_ROBTYPE_FIXED == cbRobBankMode) ||
		 (ROOMRULE_OPTION_ROBTYPE_TURN == cbRobBankMode) )
	{
		//固定庄和轮庄处理流程
		HandleTurnAndFixRobBanker();
	}

	//2、自由抢庄：发送【抢庄开始】-处理【抢庄】消息
	if ( ROOMRULE_OPTION_ROBTYPE_FREE == cbRobBankMode )
	{
		//自由抢庄消息处理流程
		HandleFreeRobBanker();
	}
}

//处理固定庄和轮庄
void CTableFrameSink::HandleTurnAndFixRobBanker()
{
	//设置全部用户抢庄完成
	BYTE cbPlayerNum = m_GameAccess->GetMaxChairCount();
	for (BYTE i = 0; i < cbPlayerNum; ++i)
	{
		if( USER_PLAYING == m_GameAccess->GetPlayerState(i) )
		{
			m_GameAccess->SetBankerState(i, 1);
		}
	}
		
	//指定庄家
	WORD wBanker = INVALID_CHAIR;
	bool bAppointResult = m_GameLogic->AppointBanker(wBanker);

	//校验 这里不退出，默认房主为庄家
	if (INVALID_CHAIR == wBanker || !bAppointResult)
	{
		wBanker = m_GameAccess->GetRoomFangzhu();
	}

	////构建数据
	//STR_CMD_SC_BET_START sAddScoreStart;
	//ZeroMemory(&sAddScoreStart,sizeof(sAddScoreStart));
	////赋值
	//sAddScoreStart.wBanker = wBanker;
	//sAddScoreStart.dwBetCellScore = ROOMRULE_MIN_CELL_SCORE;

	//printf("\n【固定庄】：庄家ID = %d，底分 = %d\n", sAddScoreStart.wBanker, sAddScoreStart.dwBetCellScore);

	////广播【下注开始】
	//m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_BET_START, &sAddScoreStart, sizeof(STR_CMD_SC_BET_START));

	////设置游戏状态为【下注状态】
	//m_pITableFrame->SetGameStatus(GS_WK_ADDSCORE);


	//// 下注定时器
	//m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);
	//m_pITableFrame->SetGameTimer(IDI_ADD_SCORE, IDI_TIME_ADD_SCORE, 1, 0);
}

//处理自由抢庄
void CTableFrameSink::HandleFreeRobBanker()
{
	//第一局：从房主开始发送抢庄开始消息	第二局：从上局的赢家开始发送抢庄消息
	WORD wChairID = m_GameAccess->GetRoomFangzhu();
	if (1 != m_GameAccess->GetCurGameCount() )
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
	//通知第一个玩家抢庄开始
	STR_CMD_SC_ROB_BANKER_START RobStart;
	ZeroMemory(&RobStart, sizeof(STR_CMD_SC_ROB_BANKER_START));

	//赋值
	RobStart.cbType = cbType;

	if ( INVALID_CHAIR != wChairID)
	{
		m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_ROB_START, &RobStart, sizeof(STR_CMD_SC_ROB_BANKER_START));

		//设置游戏状态为【抢庄状态】
		m_pITableFrame->SetGameStatus(GS_WK_ROB);

		//抢庄定时器
		m_pITableFrame->KillGameTimer(IDI_ROB_BANKER);
		m_pITableFrame->SetGameTimer(IDI_ROB_BANKER, IDI_TIME_ROB_BANKER, 1, wChairID);
	}
}

// 处理客户端发来的【抢庄】消息
void CTableFrameSink::OnUserCallBanker(WORD wChairID, BYTE cbType, BYTE cbRobState)
{
	printf("\n【服务器】：接收用户 = %d, 抢庄类型 = %d, 抢庄 = %d\n", wChairID, cbType, cbRobState);

	//游戏状态效验
	if ( GS_WK_ROB != m_pITableFrame->GetGameStatus() )
		return;

	//如果玩家选择过，则直接退出，不需要再判断该玩家
	BYTE cbOldState = m_GameAccess->GetBankerState( wChairID);
	if ( ROB_STATE_PASS == cbOldState )
		return;

	//设置抢庄状态
	m_GameAccess->SetBankerState(wChairID, cbRobState);

	//玩家同意抢，则设置抢庄次数
	BYTE cbRobNum = m_GameAccess->GetUserRobNum(wChairID);
	if ( ROB_STATE_AGREE == cbRobState )
	{		
		m_GameAccess->SetUserRobNum(wChairID, ++cbRobNum);
	}
	
	//构造抢庄结果数据
	STR_CMD_SC_ROB_RESULT sRobResult;
	ZeroMemory(&sRobResult, sizeof(STR_CMD_SC_ROB_RESULT));

	//赋值
	sRobResult.wChairID  = wChairID;
	sRobResult.cbType = cbType;
	sRobResult.cbRobState = cbRobState;

	//广播抢庄结果
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_ROB_RESULT, &sRobResult, sizeof(STR_CMD_SC_ROB_RESULT));

	//若玩家两次都抢庄，则他为庄家
	if (2 == cbRobNum)
	{
		AllRobBankerOver();
	}
	else	//抢庄还未完成，则继续给下个玩家发送抢庄消息
	{
		//判断是不是所有玩家都选择不抢庄
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

		//所有人放弃抢庄，则重新发牌
		if ( cbPassNum == cbMaxChairCount )
		{
			//清空数据
			m_GameLogic->ClearXjGame();

			//给玩家发牌
			HandleDeal();

			//处理抢庄（抢地主）
			HandleRobBanker();
		}
		else	//给下个玩家发送抢庄消息
		{
			//给下一个玩家发送抢庄开始消息
			WORD wNextUser = m_GameAccess->GetNextUser(wChairID);

			//判断下个玩家的抢庄状态
			while ( wNextUser != wChairID )		//玩家不是“过”状态才给他发送抢地主消息
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

// 全部抢庄完成, 进入下注状态
void CTableFrameSink::AllRobBankerOver()
{
	//设置游戏状态为【下注状态】
	m_pITableFrame->SetGameStatus(GS_WK_ADD_SCORE);

	//关闭抢庄定时器
	m_pITableFrame->KillGameTimer(IDI_ROB_BANKER);

	//指定庄家
	WORD wBanker = INVALID_CHAIR;
	bool bAppointResult = m_GameLogic->AppointBanker(wBanker);

	//校验 这里不退出，默认房主为庄家
	if (INVALID_CHAIR == wBanker || !bAppointResult)
	{
		wBanker = m_GameAccess->GetRoomFangzhu();
	}

	//发送指定庄家消息
	SendAppointBanker(wBanker);

	//发送底牌给庄家
	HandleSendLeaveCard(wBanker);

	//发送加注开始（加倍）消息
	SendAddScoreStart(INVALID_CHAIR);
}

//处理发送底牌
void CTableFrameSink::HandleSendLeaveCard(const WORD &wSendCardUser)
{
	printf("\n【给玩家 = %d发送底牌】\n", wSendCardUser);

	//所有人都不能反主，则进入下一个流程，给庄家发送底牌
	m_GameLogic->SendLeaveCardToUser(wSendCardUser);

	//给其他玩家发送庄家拿到底牌的消息
	BYTE cbMaxChairCount = m_GameAccess->GetMaxChairCount();
	for (WORD i = 0; i < cbMaxChairCount; i++)
	{
		if ( (USER_PLAYING == m_GameAccess->GetPlayerState(i)) )
		{
			//发送底牌消息
			STR_CMD_SC_SEND_LEAVE_CARD SendCard;
			ZeroMemory(&SendCard, sizeof(STR_CMD_SC_SEND_LEAVE_CARD));

			//赋值
			SendCard.wSendCardUser = i;	
			SendCard.cbMagicCard = m_GameAccess->GetMagicCard();

			//玩家手牌数
			BYTE cbCardNum = m_GameAccess->GetUserCurCardNum(i);

			//卡牌排序
			SendCard.cbHandCardNum = m_GameAccess->GetUserCurCardNum(i);
			m_GameLogic->SortCardList(i, SendCard.cbHandCardNum, ST_ORDER);
			m_GameAccess->GetClientHandCards(i, SendCard.cbSortedCard, MAX_CARD_COUNT);	
			
			//主花色个数
			SendCard.cbMagicCardNum = m_GameAccess->GetMagicCardNum(i);		
			printf("\n【发生底牌，玩家 = %d，癞子个数 = %d】\n", i, SendCard.cbMagicCardNum);

			// 发送数据
			m_pITableFrame->SendTableData(i, CMD_SC_USER_SEND_LEAVE_CARD, &SendCard, sizeof(STR_CMD_SC_SEND_LEAVE_CARD));
		}
	}
}

//发送下注开始消息
void CTableFrameSink::SendAddScoreStart(const WORD &wChairID)
{
	//指定庄家后，发送下注开始消息
	STR_CMD_SC_ADD_SCORE_START sAddScoreStart;
	ZeroMemory(&sAddScoreStart, sizeof(STR_CMD_SC_ADD_SCORE_START));

	//赋值		
	sAddScoreStart.cbStart = 1;

	printf("\n【服务器】：广播【下注开始】\n");

	// 广播消息 用户下注开始
	m_pITableFrame->SendTableData(wChairID, CMD_SC_ADD_SCORE_START, &sAddScoreStart, sizeof(STR_CMD_SC_ADD_SCORE_START));
		
	m_pITableFrame->SetGameStatus(GS_WK_ADDSCORE);

	// 下注定时器
	m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);
	m_pITableFrame->SetGameTimer(IDI_ADD_SCORE, IDI_TIME_ADD_SCORE, 1, 0);
}

// 下注开始返回
void CTableFrameSink::OnUserAddScore( WORD wChairID, SCORE lScore )
{
	printf("\n【服务器】：接收【下注】，玩家 = %d，下注 = %ld\n", wChairID, lScore);

	//游戏状态效验
	if ( GS_WK_ADDSCORE != m_pITableFrame->GetGameStatus() )
		return;

	//用户状态校验
	if( (USER_PLAYING != m_GameAccess->GetPlayerState(wChairID)) &&
		(1 != m_GameAccess->GetAddScoreState(wChairID)) )
		return;

	//可多次加注, 因此此处不处理加注完成标记
	m_GameLogic->Wager(wChairID, lScore);

	// 设置玩家下注状态
	m_GameAccess->SetAddScoreState(wChairID, 1);

	//构造数据
	STR_CMD_SC_ADD_SCORE_RESULT sAddScoreResult;
	ZeroMemory(&sAddScoreResult, sizeof(STR_CMD_SC_ADD_SCORE_RESULT));
	//赋值
	sAddScoreResult.wChairID	 = wChairID;
	sAddScoreResult.wAddSocre	 = lScore;

	printf("\n【服务器】：发送【下注结果】，玩家 = %d，下注 = %d\n",sAddScoreResult.wChairID, sAddScoreResult.wAddSocre);

	//广播下注结果
	m_pITableFrame->SendTableData( INVALID_CHAIR, CMD_SC_ADD_SCORE_RESULT, &sAddScoreResult, sizeof(STR_CMD_SC_ADD_SCORE_RESULT) );

	// 计算加注完成玩家数
	BYTE cbBetedNum = 0;
	BYTE cbPlayerNum = m_GameAccess->GetMaxChairCount();// 获取玩家数
	for (BYTE i = 0; i < cbPlayerNum; ++i)
	{
		if ( 1 == m_GameAccess->GetAddScoreState(i) )
			cbBetedNum++;
	}

	// 加注完成，发牌
	if ( cbBetedNum == m_GameAccess->GetCurPlayerCount() )
	{
		printf("\n所有人下注完成：下注人数 = %d\n", cbPlayerNum);

		//通知庄家是否需要明牌
		WORD wBanker = m_GameAccess->GetBankerID();
		SendMingPaiStart(wBanker, MING_PAI_TYPE_OUTCARD);
	}

	return;
}

//发送明牌开始消息
void CTableFrameSink::SendMingPaiStart(const WORD &wChairID, const BYTE &cbMPType)
{
	//指定庄家后，发送下注开始消息
	STR_CMD_SC_MING_PAI_START MingPaiStart;
	ZeroMemory(&MingPaiStart, sizeof(STR_CMD_SC_MING_PAI_START));

	//赋值		
	MingPaiStart.wChairID = wChairID;
	MingPaiStart.cbType = cbMPType;

	printf("\n【服务器】：通知用户 = %d【明牌开始】\n", wChairID);

	// 通知用户明牌开始
	m_pITableFrame->SendTableData(wChairID, CMD_SC_MING_PAI_START, &MingPaiStart, sizeof(STR_CMD_SC_MING_PAI_START));

	// 明牌定时器
	m_pITableFrame->KillGameTimer(IDI_MING_PAI);
	m_pITableFrame->SetGameTimer(IDI_MING_PAI, IDI_TIME_MING_PAI, 1, 0);
}

//明牌事件
void CTableFrameSink::On_Sub_UserMingPai(WORD wChairID, const BYTE &cbMPType, BYTE cbFlag)
{
	//玩家明牌
	if ( cbFlag == 1 )			
	{
		//构造明牌数据
		STR_CMD_SC_MING_PAI_RESULT MPResult;
		ZeroMemory(&MPResult, sizeof(STR_CMD_SC_MING_PAI_RESULT));

		//明牌加倍
		DWORD dwBet = m_GameAccess->GetAllBet(wChairID);
		if ( MING_PAI_TYPE_GAMESTART == cbMPType )
		{			
			//加倍
			MPResult.cbTimes = 4;
			m_GameLogic->Wager(wChairID, 3*dwBet);
		}
		else if ( MING_PAI_TYPE_OUTCARD == cbMPType )
		{
			MPResult.cbTimes = 2;
			m_GameLogic->Wager(wChairID, dwBet);
		}

		//赋值
		MPResult.wChairID = wChairID;
		MPResult.cbIsMing = cbFlag;
		m_GameAccess->GetClientHandCards(wChairID, MPResult.cbHandCard, MAX_CARD_COUNT);		

		//广播玩家明牌消息
		m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_USER_OUT_CARD_RESULT, &MPResult, sizeof(STR_CMD_SC_MING_PAI_RESULT));
	}

	//发送出牌开始
	WORD wBanker = m_GameAccess->GetBankerID();
	HandleOutCardStart(wBanker);
}

//处理出牌开始消息
void CTableFrameSink::HandleOutCardStart(const WORD &wOutCardUser)
{
	//设置当前出牌玩家
	m_GameAccess->SetCurOutCardUser(wOutCardUser);

	//广播出牌开始的消息
	STR_CMD_SC_OUT_CARD_START OutCardStart;
	ZeroMemory(&OutCardStart, sizeof(STR_CMD_SC_OUT_CARD_START));

	OutCardStart.wOutCardUser = wOutCardUser;

	//广播出牌完成消息
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_USER_OUT_CARD_START, &OutCardStart, sizeof(STR_CMD_SC_OUT_CARD_START));

	printf("\n【通知玩家 = %d出牌开始】\n", OutCardStart);

	//设置出牌游戏状态
	m_pITableFrame->SetGameStatus(GS_WK_OUT_CARD);

	// 出牌定时器
	m_pITableFrame->KillGameTimer(IDI_OUT_CARD);
	m_pITableFrame->SetGameTimer(IDI_OUT_CARD, IDI_TIME_OUT_CARD, 1, 0);	
}

//出牌事件
void CTableFrameSink::On_Sub_UserOutCard(WORD wOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum, const bool &bPass)
{
	//游戏状态效验
	if ( GS_WK_OUT_CARD != m_pITableFrame->GetGameStatus() ) 
		return;

	//数据校验
	if ((wOutCardUser != m_GameAccess->GetCurOutCardUser())||
		(MAX_CARD_COUNT < cbOutCardNum))
	{
		return;
	}

	// 删除出牌定时器
	m_pITableFrame->KillGameTimer(IDI_OUT_CARD);

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

	//一轮开始出牌玩家
	WORD wStartUser = m_GameAccess->GetStartOutCardUser();

	//校验是不是轮到开始玩家了
	if ( wNextUser == wStartUser )
	{
		//是，则一轮结束
		HandleRoundEnd(wNextUser);
	}
	else
	{
		//否，通知下个玩家出牌开始
		HandleOutCardStart(wNextUser);
	}
}

//处理用户出牌
void CTableFrameSink::HandleOutCard(WORD wOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum)
{
	//若玩家是一轮中最先开始出牌者
	WORD wStartUser = m_GameAccess->GetStartOutCardUser();
	//牌型
	BYTE cbCardType= m_GameLogic->GetCardLogicType(cbOutCard, cbOutCardNum);

	//第一个出牌玩家
	if ( (TURN_END == m_GameAccess->GetOneTurnEnd()) &&
		 (wOutCardUser == wStartUser) )
	{
		//牌型校验
		if (CT_ERROR == cbCardType)		
		{
			//发送出牌结果
			SendOutCardResult(wOutCardUser, cbOutCard, cbOutCardNum, cbCardType, OUT_CARD_FAIL, OUT_CARD_NORMAL);

			printf("\n【玩家 = %d出牌失败，牌型不对】\n", wOutCardUser);
			return;
		}
		else
		{
			m_GameAccess->SetOneTurnEnd(TURN_NOT_END);
		}
	}
	else	//后面出牌玩家
	{
		//校验出牌数目是否和最开始出牌玩家相同，出牌校验
		if ( (cbOutCardNum != m_GameAccess->GetUserOutCard(wStartUser, NULL)) || 
			 !m_GameLogic->EfficacyOutCard(wOutCardUser, cbOutCard, cbOutCardNum) )			
		{
			//发送出牌结果
			SendOutCardResult(wOutCardUser, cbOutCard, cbOutCardNum, cbCardType, OUT_CARD_FAIL, OUT_CARD_NORMAL);

			printf("\n【玩家 = %d出牌失败，牌型不对】\n", wOutCardUser);

			return;
		}
	}

	//从手牌中删除玩家扑克
	if ( !m_GameLogic->RemoveCard(wOutCardUser, cbOutCard, cbOutCardNum) )
		return;

	//出牌记录
	m_GameAccess->SetUserOutCard(wOutCardUser, cbOutCard, cbOutCardNum);

	//该轮出牌最大数据记录
	m_GameAccess->SetTurnMaxCards(cbOutCard, cbOutCardNum);

	//设置下一个出牌玩家
	WORD wNextUser = m_GameAccess->GetNextUser(wOutCardUser);

	//设置玩家炸弹信息
	if ( (cbCardType&(1<<CT_BOMB_CARD))!= 0 ||
		(cbCardType&(1<<CT_MISSILE_CARD))!= 0||
		(cbCardType&(1<<CT_LAIZI_BOMB))!= 0 || 
		(cbCardType&(1<<CT_RUAN_BOMB))!= 0 )
	{
		m_GameAccess->SetUserBoomInfo(wOutCardUser, cbCardType);
	}

	//发送出牌结果
	SendOutCardResult(wOutCardUser, cbOutCard, cbOutCardNum, cbCardType, OUT_CARD_SUCCESS, OUT_CARD_NORMAL);

	//校验是不是一轮出牌完成
	if ( wNextUser == m_GameAccess->GetStartOutCardUser() )
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
	bool bCanOut = false;

	//判断其他玩家是否能出牌
	WORD wNextUser = m_GameAccess->GetNextUser(wLastOutCardUser);

	//循环遍历所有玩家,直到该玩家
	while ( wNextUser != wLastOutCardUser )
	{
		//玩家能出牌
		tagOutCardNotify OutCardNotify[MAX_WEAVE_NUM];
		ZeroMemory(&OutCardNotify, MAX_WEAVE_NUM*sizeof(tagOutCardNotify));

		if ( m_GameLogic->AnalyseOutCardNotify(wNextUser, cbOutCard, cbOutCardNum, OutCardNotify) )
		{
			printf("\n【玩家 = %d要的起，发送出牌开始】\n", wNextUser);

			//通知该玩家出牌开始
			m_GameAccess->SetCurOutCardUser(wNextUser);
			HandleOutCardStart(wNextUser);
			bCanOut = true;
			break;
		}
		else	//发送出牌结果
		{
			printf("\n【玩家 = %d要不起，发送出牌结果】\n", wNextUser);
			SendOutCardResult(wNextUser, cbOutCard, cbOutCardNum, 0, OUT_CARD_FAIL, OUT_CARD_PASS);
		}

		wNextUser = m_GameAccess->GetNextUser(wNextUser);
	}
	
	return bCanOut;
}

//发送出牌结果
void CTableFrameSink::SendOutCardResult(WORD wOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum, 
										 const BYTE &cbCardType, const BYTE &cbSuccess, const BYTE &cbFlag)
{
	//构造出牌数据
	STR_CMD_SC_OUT_CARD_RESULT OutCard;
	ZeroMemory(&OutCard, sizeof(STR_CMD_SC_OUT_CARD_RESULT));

	//赋值
	OutCard.wOutCardUser = wOutCardUser;
	OutCard.cbFlag = cbFlag;
	OutCard.cbSuccess = cbSuccess;

	//正常出牌
	if ( 1 == cbFlag )
	{
		if (0 == cbSuccess)
		{
			m_pITableFrame->SendTableData(wOutCardUser, CMD_SC_USER_OUT_CARD_RESULT, &OutCard, sizeof(STR_CMD_SC_OUT_CARD_RESULT));
		}
		else
		{
			//赋值					
			OutCard.cbOutCardNum = cbOutCardNum;
			memcpy_s(OutCard.cbOutCard, sizeof(BYTE)*cbOutCardNum, cbOutCard, sizeof(BYTE)*cbOutCardNum);
			OutCard.cbHandCardNum = m_GameAccess->GetUserCurCardNum(wOutCardUser);
			
			//牌型
			OutCard.cbCardType = cbCardType;

			//得分
			BYTE cbPlayerNum = m_GameAccess->GetMaxChairCount();
			for (BYTE i = 0; i < cbPlayerNum; ++i)
			{
				if ( USER_PLAYING == m_GameAccess->GetPlayerState(i) )					
				{
					OutCard.lSingleGameScore[i] = m_GameAccess->GetPlayerSingleScore(i);
					printf("\n【玩家 = %d的单局得分 = %d】\n", i, OutCard.lSingleGameScore[i]);
				}
			}
		}	
	}
	else	//玩家要不起
	{
		OutCard.cbFlag = cbFlag;
	}

	//广播出牌结果
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_USER_OUT_CARD_RESULT, &OutCard, sizeof(STR_CMD_SC_OUT_CARD_RESULT));
	printf("\n【广播玩家 = %d的出牌结果】\n", OutCard.wOutCardUser);	
}

//处理一轮出牌结束
void CTableFrameSink::HandleRoundEnd(const WORD &wWinner)
{		
	//计算一轮得分
	WORD wScore = m_GameLogic->SettleOneRound(wWinner);

	//设置一轮结束
	m_GameAccess->SetOneTurnEnd(TURN_END);

	//设置开始的出牌玩家，清空该轮的出牌数据
	if ( 0 != m_GameAccess->GetUserCurCardNum(wWinner) )
	{
		//设置该轮最开始出牌用户
		m_GameAccess->SetStartOutCardUser(wWinner);

		//设置当前出牌用户
		m_GameAccess->SetCurOutCardUser(wWinner);

		//通知赢家出牌
		HandleOutCardStart(wWinner);

		printf("\n【玩家还有牌，一轮结束，通知赢家 = %d出牌开始】\n", wWinner);
	}
	else	//玩家所有牌出完
	{
		printf("\n【游戏结束】\n");

		//设置赢家
		m_GameAccess->SetLastGameWinner(wWinner);

		m_GameAccess->SetCurOutCardUser(INVALID_CHAIR);

		//算分
		m_GameLogic->Settle();

		//一轮结束，当前赢家牌全部出完，游戏结束
		OnEventGameConclude(INVALID_CHAIR, NULL, GER_NORMAL);
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

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR, CMD_SC_APPOINT_BANKER, &AppointBanker, sizeof(STR_CMD_SC_APPOINT_BANKER));
}

//小局结束-用户准备事件
void CTableFrameSink::OnUserXjGameReady(WORD wChairID, BYTE ready)
{
	printf("\n【服务器】：接受【小局开始】\n");
	//游戏状态效验
	if ( GS_WK_XJ_GAMEEND != m_pITableFrame->GetGameStatus() )
	{
		printf("\n错误游戏状态 = %d\n");
		return;
	}

	//用户状态校验
	if ( USER_PLAYING != m_GameAccess->GetPlayerState(wChairID) || 
		(1 == m_GameAccess->GetXjReadyState(wChairID)) )
	{
		printf("\n错误玩家状态 = %d\n");
		return;
	}

	// 设置游戏准备状态
	m_GameAccess->SetXjReadyState(wChairID, ready);

	// 计算准备人数
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
		//删除小局结束定时器
		m_pITableFrame->KillGameTimer(IDI_XJGAME_USER_READY);
		
		// 全体准备完成，开始游戏
		StartGame();
	}
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

/*************************************************
@Description:     写入录像数据，除了定时器
				 ,在小局结束后调用，否则有些数据为空
@Input:           无
@Output:          无
@Return:          无
@author & data:	lizhihu 2017.11.2
*************************************************/
void CTableFrameSink::WriteGameRecord()
{
	//获得玩家单局游戏得分
	int _playersum = m_GameAccess->GetMaxChairCount();
	LONG *lGameScore = new LONG [_playersum];
	for (BYTE i = 0; i < _playersum; ++i)
	{
		if ( USER_PLAYING == m_GameAccess->GetPlayerState(i) )
		{
			lGameScore[i] = m_GameAccess->GetPlayerSingleScore(i);
		}	
	}
	
	//构造单局得分字符串
	CString str;
	str.Format(TEXT("%d"),lGameScore[0]);
	for(int i=1; i < _playersum; ++i)
	{
		TCHAR szTemp[10];
		_sntprintf(szTemp,10,TEXT(",%d"),lGameScore[i]);
		str.Append(szTemp);
	}

	//释放
	delete [] lGameScore;
	lGameScore = NULL;

	//调用框架方法		//暂时只写小局得分
	m_pITableFrame->WriteRecordInfo(m_GameAccess->GetCurGameCount(), str.GetBuffer(str.GetLength()), NULL, 0);
}

/*************************************************
@Description:     结束录像，释放内存
@Input:           无
@Output:          无
@Return:          无
@author & data:	lizhihu 2017.11.2
*************************************************/
void CTableFrameSink::CloseRecord()
{
	delete [] m_GameRec.pRecData;
	m_GameRec.pRecData = NULL;
}
