#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//辅助时间
#define TIME_LESS					1									//最少时间

//游戏时间
#define TIME_USER_CALL_BANKER		2									//叫庄定时器
#define TIME_USER_START_GAME		3									//开始定时器
#define TIME_USER_ADD_SCORE			3									//下注定时器
#define TIME_USER_OPEN_CARD			12									//摊牌定时器

//游戏时间
#define IDI_START_GAME			(IDI_ANDROID_ITEM_SINK+0)			//开始时间
#define IDI_CALL_BANKER			(IDI_ANDROID_ITEM_SINK+1)			//叫庄时间
#define IDI_USER_ADD_SCORE		(IDI_ANDROID_ITEM_SINK+2)			//下注时间
#define IDI_OPEN_CARD			(IDI_ANDROID_ITEM_SINK+3)			//开牌时间
#define IDI_BANKER						105							//机器人存取款

#define IDI_ANDROID_ITEM_SINK		500////////////////

//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_lTurnMaxScore = 0;
	m_lDiZhu = 0;
	// memset( &m_HandCardData, 0, sizeof m_HandCardData);

	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));///////

	//接口变量
	m_pIAndroidUserItem=NULL;

	return;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
void * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//初始接口
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//重置接口
bool CAndroidUserItemSink::RepositionSink()
{
	m_lTurnMaxScore = 0;
	// memset( &m_HandCardData, 0, sizeof m_HandCardData);
	memset( &m_cbPlayStatus, 0, sizeof m_cbPlayStatus);

	return true;
}

//时间消息
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//开始定时器
		{
			CMD_C_PlayerAction pPlayerAction;
			//pPlayerAction.iActionID = E_INGAME;  TODONOW 这里是什么？？？

			//发送准备
			m_pIAndroidUserItem->SendUserReady(NULL,0);

			return true;
		}
	case IDI_CALL_BANKER:		//叫庄定时
		{
			//设置变量
			CMD_C_CallBanker CallBanker;
			CallBanker.bBanker =rand()%2;
			if(m_pIAndroidUserItem->GetMeUserItem()->GetUserScore() < (m_cbUserCount -1)*m_lDiZhu*30)
			{
				CallBanker.bBanker = 0;
			}

			//发送信息
			m_pIAndroidUserItem->SendSocketData(SUB_C_A_CALL_BANKER,&CallBanker,sizeof(CallBanker));

			return true;
		}
	case IDI_OPEN_CARD:			//开牌定时
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

//游戏消息
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_A_CALL:
		{
			return true;
		}
	case SUB_S_A_CALL_BANKER:	//用户叫庄
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_PlayerCount)) return false;
			CMD_S_PlayerCount * pPlayerCount=(CMD_S_PlayerCount *)pData;

			
			m_cbUserCount=pPlayerCount->wPlayerCount;
			//叫庄时间
			UINT nElapse=rand()%(3)+2;
			m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER,nElapse);
			return true;

		}
		/*
	case CMD_RC_BET:	//下注开始
		{
			//消息处理
			return OnSubGameStart(pData,wDataSize);
		}
		*/
	case SUB_S_A_ADD_SCORE:	//用户下注
		{
			//消息处理
			return OnSubAddScore(pData,wDataSize);
		}
	case CMD_SC_SEND_CARD:	//发牌消息
		{
			//消息处理
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_A_OPEN_CARD:	//用户摊牌
		{
			//消息处理
			return OnSubOpenCard(pData,wDataSize);
		}
	case SUB_S_A_PLAYER_EXIT:	//用户强退
		{
			//消息处理
			return OnSubPlayerExit(pData,wDataSize);
		}
	case SUB_S_A_GAME_END:	//游戏结束
		{
			//消息处理
			return OnSubGameEnd(pData,wDataSize);
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//游戏消息
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//场景消息
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(STR_CMD_SC_STATUS_FREE));
			if (wDataSize!=sizeof(STR_CMD_SC_STATUS_FREE)) return false;

			//消息处理
			STR_CMD_SC_STATUS_FREE * pStatusFree=(STR_CMD_SC_STATUS_FREE *)pData;

			//开始时间
			UINT nElapse=rand()%(3)+TIME_LESS;
			m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

			//银行操作
			BankOperate(2);
			m_lDiZhu = pStatusFree->CellScore;

			//下底注
			// memcpy(m_lTurnMaxScore, pStatusFree->lCellScore, sizeof(m_lTurnMaxScore));

			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//用户进入
void CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户离开
void CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户积分
void CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户状态
void CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户段位
void CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户叫庄
bool CAndroidUserItemSink::OnSubCallBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_CallBanker)) return false;
	CMD_S_CallBanker * pCallBanker=(CMD_S_CallBanker *)pBuffer;

	//始叫用户
	if(pCallBanker->wCallBanker==m_pIAndroidUserItem->GetChairID())
	{

		//叫庄时间
		UINT nElapse=rand()%(3)+1;
		m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER,nElapse);
	}
	return true;
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	////效验数据
	//if (wDataSize!=sizeof(STR_CMD_SC_BET_START)) return false;
	//STR_CMD_SC_BET_START * pGameStart=(STR_CMD_SC_BET_START *)pBuffer;

	return true;
}

//用户下注
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//效验数据

	return true;
}

//发牌消息
bool CAndroidUserItemSink::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(STR_CMD_SC_SEND_CARD)) return false;
	STR_CMD_SC_SEND_CARD * pSendCard=(STR_CMD_SC_SEND_CARD *)pBuffer;

	//设置数据
	WORD wMeChiarID=m_pIAndroidUserItem->GetChairID();
	// memcpy_s(&m_HandCardData, sizeof m_HandCardData, &pSendCard->cbCardData[wMeChiarID],  sizeof m_HandCardData);

	////for(BYTE i=0;i<MAX_CHAIR_COUNT;i++)
	////{
	////	m_cbPlayStatus[i]=pGameStart->cbPlayStatus[i];
	////}

	//多人游戏延时倍数////////////


	//开牌时间
	UINT nElapse=rand()%(3)+m_cbUserCount*2;////////
	m_pIAndroidUserItem->SetGameTimer(IDI_OPEN_CARD,nElapse);

	return true;
}
//银行操作
void CAndroidUserItemSink::BankOperate(BYTE cbType)
{
	IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
	if(pUserItem->GetUserStatus()>=US_SIT)
	{
		if(cbType==1)
		{
			CString strInfo;
			strInfo.Format(TEXT("大厅：状态不对，不执行存取款"));
			// NcaTextOut(strInfo);
			return;
		}
	}

	//变量定义
	SCORE lRobotScore = pUserItem->GetUserScore();

	{
		CString strInfo;
		strInfo.Format(TEXT("[%s] 分数(%I64d)"),pUserItem->GetNickName(),lRobotScore);

		if (lRobotScore > m_pIAndroidUserItem->GetAndroidParameter()->lMaxTakeScore)
		{
			CString strInfo1;
			strInfo1.Format(TEXT("满足存款条件(%I64d)"),m_pIAndroidUserItem->GetAndroidParameter()->lMaxTakeScore);
			strInfo+=strInfo1;

			// NcaTextOut(strInfo);
		}
		else if (lRobotScore < m_pIAndroidUserItem->GetAndroidParameter()->lMinTakeScore)
		{
			CString strInfo1;
			strInfo1.Format(TEXT("满足取款条件(%I64d)"),m_pIAndroidUserItem->GetAndroidParameter()->lMinTakeScore);
			strInfo+=strInfo1;

			// NcaTextOut(strInfo);
		}

		//判断存取
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
			strInfo.Format(TEXT("[%s] 执行存款：存款前金币(%I64d)，存款后金币(%I64d)"),pUserItem->GetNickName(),lRobotScore,lRobotNewScore);

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
			strInfo.Format(TEXT("[%s] 执行取款：取款前金币(%I64d)，取款后金币(%I64d)"),pUserItem->GetNickName(),lRobotScore,lRobotNewScore);

			// NcaTextOut(strInfo);
		}
	}
}
//用户摊牌
bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	/*if (wDataSize!=sizeof(CMD_S_Open_Card)) return false;
	CMD_S_Open_Card * pOpenCard=(CMD_S_Open_Card *)pBuffer;*/

	return true;
}

//用户强退
bool CAndroidUserItemSink::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	/*if (wDataSize!=sizeof(CMD_S_PlayerExit)) return false;
	CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;*/

	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	if (wDataSize!=sizeof(STR_CMD_SC_XJ_GAME_END)) return false;
	STR_CMD_SC_XJ_GAME_END * pGameEnd=(STR_CMD_SC_XJ_GAME_END *)pBuffer;
	//删除定时器
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_BANKER);
	m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
	m_pIAndroidUserItem->KillGameTimer(IDI_OPEN_CARD);
	//开始时间
	UINT nElapse=rand()%(3)+7+m_cbUserCount;
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

	UINT nTime = rand()%nElapse + 1;
	m_pIAndroidUserItem->SetGameTimer(IDI_BANKER,nTime);

	//清理变量
	m_lTurnMaxScore = 0;
	// memset( &m_HandCardData, 0, sizeof m_HandCardData);

	return true;
}

//////////////////////////////////////////////////////////////////////////
