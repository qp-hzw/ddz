#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE
#pragma once

#include "Stdafx.h"
#include "GameData.h"

enum GameRecIndex
{
	chariID = 0,				//【椅子号】开始字节数、数据大小索引
	nickName,					//【昵称】开始字节数、数据大小索引
	robResult,					//【抢庄结果】开始字节数、数据大小索引
	betStart,					//【下注开始】始字节数、数据大小索引
	betResult,					//【下注结果】开始字节数、数据大小索引		
	sendCard,					//【发牌】开始字节数、数据大小索引
	showCard,					//【摊牌】开始字节数、数据大小索引
	xjGameEnd					//【小局结束】开始字节数、数据大小索引
};

//小局游戏结束，游戏录像结构体
//(椅子ID + 玩家昵称 + 抢庄结果 + 下注结果 + 摊牌)*MAX_CHAIR_COUNT + (下注开始 + 发牌 + 小局结束)*1 + 定时器*3
struct GameRecord
{
	BYTE *pRecData;			// 游戏录像数据	//pRecData的大小 =  nEachGameSize
	int nRecOffset[8];		// 游戏录像数组索引，用来索引录像数据,例如玩家昵称从第25个字节开始
	int nStructSize[8];		// 每个存入录像的结构体数据大小
	int nEachGameSize;		// 每局游戏的数据大小
	time_t time_start;		// 某个计时器开始的时间
	time_t time_end;		// 某个定时器结束的时间
};


//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//组件变量
protected:
	//三个类处理棋牌子游戏通用的部分
	CGameData						m_GameData;								//游戏逻辑管理
	CGameLogic						* m_GameLogic;							//游戏逻辑接口
	CGameAccess						* m_GameAccess;							//游戏查询接口

	ITableFrame						* m_pITableFrame;						//框架接口
	tagGameServiceOption            * m_pGameServiceOption;					//游戏配置
	tagGameServiceAttrib            * m_pGameServiceAttrib;					//游戏属性

	tagTableCfg						* m_pRoomRuleOption;					//房间规则

	DWORD							m_dPlayerState[MAX_CHAIR_COUNT];		//框架传来的玩家游戏状态

	GameRecord						m_GameRec;								//游戏录像数据结构体

	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	//初始化
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual VOID RepositionSink();

	//游戏事件
public:
	//游戏开始
	virtual bool OnEventGameStart();
	//游戏结束
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//事件接口
public:
	// 定时器事件
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//游戏消息处理
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息处理
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem) { return false; }
	//数据事件
	virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize) { return false; }

	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){ return 0;}
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){ return 0;}
	//查询服务费
	virtual bool QueryBuckleServiceCharge(WORD wChairID){ return true;}

	//事件接口
public:
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){ return false;}
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){ return false;}
	//设置积分
	virtual void SetGameBaseScore(LONG lBaseScore) {}

	//用户事件，框架传入
public:
	//用户断线
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户重入
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起立
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户准备
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);
	//用户下注
	virtual bool OnActionUserPour(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }


	//响应client事件
protected:
	//亮主事件
	void OnUserLiangZhu(WORD wChairID, BYTE cbCardColor);
	//反主事件
	void OnUserFanZhu(WORD wChairID, BYTE cbCard, BYTE cbFanzhuFlag);
	//扣底事件
	void OnUserKouDi(WORD wChairID, BYTE cbLeaveCard[MAX_LEAVE_CARD_NUM]);
	//出牌事件
	void On_Sub_UserOutCard(WORD wOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum, const bool &bPass);

	//抢庄事件
	void OnUserCallBanker(WORD wChairID, BYTE cbType, BYTE cbRobState);
	//下注事件
	void OnUserAddScore(WORD wChairID, SCORE lScore);
	//明牌事件
	void On_Sub_UserMingPai(WORD wChairID, const BYTE &cbMPType, BYTE cbFlag);

	//小局结算准备
	void OnUserXjGameReady(WORD wChairID, BYTE ready);

	//响应client时间的 内部函数
protected:
	// 全部抢庄完成
	void AllRobBankerOver();
	// 小局游戏结束
	bool XjGameConclude(int nTotalGameCount, int nCurGameCount);
	// 大局游戏结束
	bool DjGameConclude(int nTotalGameCount, int nCurGameCount);

	//辅助函数
protected:
	//开始游戏
	void StartGame();
	//开始录像
	void StartRecord();
	//写入录像数据，除了定时器
	void WriteGameRecord();
	//结束录像
	void CloseRecord();
	//处理发牌流程
	void HandleDeal();
	//处理发送底牌
	void HandleSendLeaveCard(const WORD &wSendCardUser);
	//处理出牌开始消息
	void HandleOutCardStart(const WORD &wOutCardUser);
	//处理用户正常出牌
	void HandleOutCard(WORD wOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum);
	//处理用户出牌过
	void HandleOutCardPass(WORD wOutCardUser);
	//处理一轮出牌结束
	void HandleRoundEnd(const WORD &wWinner);
	//处理抢庄
	void HandleRobBanker();
	//处理固定庄和轮庄
	void HandleTurnAndFixRobBanker();
	//处理自由抢庄
	void HandleFreeRobBanker();
	//发送抢庄开始消息
	void SendRobStart(const WORD &wChairID, const BYTE &cbType);
	//发送指定庄家消息
	void SendAppointBanker(const WORD &wBanker);
	//发送下注开始消息
	void SendAddScoreStart(const WORD &wChairID);
	//发送明牌开始消息
	void SendMingPaiStart(const WORD &wChairID, const BYTE &cbMPType);
	//发送出牌结果
	void SendOutCardResult(WORD wOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum, 
						const BYTE &cbCardType, const BYTE &cbSuccess, const BYTE &cbFlag);
	//判断玩家是否能出牌
	bool JudgeUserOutCard(const WORD &wLastOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum);
};

#endif
