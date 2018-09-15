#ifndef GAME_INTERFACE
#define GAME_INTERFACE

#pragma once
#include <objbase.h>
#include <iostream>
#include "GameCardConfig.h"
#include "GamePlayerConfig.h"
#include "ServerLog.h"

//逻辑掩码
#define	MASK_COLOR					0xF0								//花色掩码
#define	MASK_VALUE					0x0F								//数值掩码


// 游戏棋牌配置描述
typedef struct _tagGAME_CARD_CONFIG
{
	CARD_DESCS		game_cards;		 // 游戏内用到的所有卡牌集合
	DWORD			game_cards_sum;	 // 卡牌总张数
	BYTE			init_card_num;	 // 玩家初始手牌数，升级根据人数决定（（108-8）/n）
	BYTE			leave_card_num;	 // 扣底数目（8或9张）
}GAME_CARD_CONFIG;

// 局内玩家
typedef struct _tagPLAYERS_CONFIG
{
	PLAYER_DESCS	players;				// 玩家数组
	DWORD			player_sum;				// 玩家数组最大长度，当前玩家数应该小于该数
	CARD_DESC		hand_cards[MAX_CHAIR_COUNT][MAX_CARD_COUNT];		// 玩家手牌，2-20个人，每个人5张牌
}PLAYERS_CONFIG;


// 房间游戏配置
typedef struct _tagROOM_CONFIG
{
	DWORD	kind_id;				// 游戏标识符	
	BYTE	game_count;				// 游戏配置局数	
	DWORD	room_cell;				// 房间底分
	BYTE	max_chair_sum;			// 房间最大椅子数
	WORD	fangzhu_id;				// 房主id
	BYTE	game_robBank_mode;		// 游戏的抢庄模式
	BYTE	game_score_mode;		// 得分模式：经典模式或疯狂加倍模式，从框架获取
	BYTE	game_score_times[MAX_GAME_SCORE_TYPES];	// 根据得分模式，从配置文件中获取得分的倍数，数组的大小根据有牌型的多少设置，目前18种
}ROOM_CONFIG;

// 游戏中参数
typedef struct _tagPLAYING_PARAMETERS
{
	DWORD	game_state;				// 当前游戏状态
	BYTE	game_cur_count;			// 游戏当前局数
	WORD	banker_id;				// 庄家椅子ID
	DWORD	start_bet;				// 房间开始下注分数
	DWORD	cur_total_bet;			// 当前房间下注总分
	WORD	last_winner_id;			// 上把赢家

	//斗地主
	BYTE	magic_card;				// 癞子

	//升级
	BYTE	sort_right[LEN_SORT_RIGHT];		// 花色排序权位
	BYTE	main_color;						// 主牌花色
	BYTE	level_value;					// 级牌数值（从2开始）
	BYTE	nt_value;						// 常主数值（大小王）
	BYTE	leave_card[MAX_LEAVE_CARD_NUM];	// 扣底卡牌数组
	WORD	liangzhu_user;					// 亮主玩家
	BYTE	cur_fanzhu_card;				// 当前反主卡牌
	WORD	cur_outcard_user;				// 当前出牌玩家
	WORD	start_outcard_user;				// 一轮中最先开始出牌玩家
	BYTE	score_card[MAX_SCORE_CARD_NUM];	// 得分扑克数据
	WORD	cur_fanzhu_user;				// 当前反主玩家
	WORD	cur_koudi_user;					// 当前扣底玩家
	WORD	start_rob_user;					// 最先开始抢庄玩家

	BYTE	turn_max_cards[MAX_CARD_COUNT];	// 一轮中最大的卡牌数据
	BYTE	turn_cards_num;					// 一轮中最大的卡牌数据大小
	BYTE	turn_end;						// 一轮是否结束	0-结束	1-未结束

}PLAYING_PARAMETERS;

interface CGameAccess;
interface CGameLogic;

/************************ 游戏规则接口 ***************************/
interface CGameLogic
{
	// 创建游戏
	virtual int __stdcall CreateGame( CGameLogic **pGameRule, CGameAccess **pGameAccess ) = NULL;

	// 加载读取的配置文件参数
	virtual int __stdcall LoadConfigParameter( const wchar_t *_logFile, const BYTE cbGameScoreMode) = NULL;

	//初始化数据
	virtual int __stdcall Init() = NULL;

	// 玩家下注
	virtual int __stdcall Wager( WORD wChairID, DWORD bet  ) = NULL;

	// 根据抢庄模式，指定庄家
	virtual bool __stdcall AppointBanker( WORD &wChairID) = NULL;

	// 发牌
	virtual int __stdcall Deal() = NULL;

	// 设置玩家操作
	virtual int __stdcall SetUserOperate( WORD wChairID, BYTE byAction, DWORD dwActionScore ) = NULL;

	// 分析用户牌型
	virtual int __stdcall AnsysCardsType() = NULL;

	// 结算得分
	virtual int __stdcall Settle() = NULL;

	// 清理小局游戏数据
	virtual int __stdcall ClearXjGame() = NULL;

	// 清理大局游戏数据
	virtual int __stdcall ClearDjGame() = NULL;

	// 清理房间数据
	virtual int __stdcall ClearRoom() = NULL;

	//按花色权位排序扑克
	virtual void __stdcall SortCardList(BYTE *cbCardData, BYTE cbCardCount) = NULL;

	//按花色权位给用户手牌排序
	virtual void __stdcall SortCardList(const WORD &wChairID, BYTE cbCardCount, BYTE cbSortType) = NULL;

	//斗地主排序
	virtual void __stdcall SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType) = NULL;


	//获得卡牌的排序等级
	virtual BYTE __stdcall GetCardSortOrder(BYTE cbCardData) = NULL;

	//分析玩家是否能亮主
	virtual bool __stdcall AnalyseUserLiangZhu(const WORD &wChairID, BYTE *cbLiangZhuColor) = NULL;

	//分析玩家是否能反主
	virtual bool __stdcall AnalyseUserFanZhu(const WORD &wChairID, BYTE *cbFanZhuCard) = NULL;

	//判断玩家手牌是否能反主
	virtual bool __stdcall IsUserCanFanZhu(const WORD &wChairID, const BYTE &cbCurFanZhuColor, BYTE *cbFanZhuCard) = NULL;

	//发送底牌给玩家
	virtual void __stdcall SendLeaveCardToUser(const WORD &wChairID) = NULL;

	//获得一组卡牌的牌类型
	virtual BYTE __stdcall GetCardLogicType(const BYTE *cbCardData, BYTE cbCardCount) = NULL;

	//获得卡牌的逻辑花色
	virtual BYTE __stdcall GetCardLogicColor(BYTE cbCardData) = NULL;

	//获得卡牌数组的逻辑花色
	virtual BYTE __stdcall GetCardLogicColor(const BYTE *cbCardData, BYTE cbCardCount) = NULL;

	//获得卡牌的逻辑数值
	virtual BYTE __stdcall GetCardLogicValue(BYTE cbCardData) = NULL;

	//两张卡牌是否相连
	virtual bool __stdcall IsLineValue(BYTE cbTurnCard, BYTE cbSecondCard) = NULL;

	//一轮比牌，判断赢家
	virtual WORD __stdcall GetOneRoundWinner() = NULL;

	//计算一轮输赢得分
	virtual WORD __stdcall SettleOneRound(const WORD &wWinner) = NULL;

	//获得一轮中的所有分牌
	virtual BYTE __stdcall GetScoreCard() = NULL;

	//获取卡牌数组总分数
	virtual WORD __stdcall GetAllCardScore(const BYTE *cbCardData, BYTE cbCardCount) = NULL;

	//比较玩家出牌大小
	virtual bool __stdcall CompareUserOutCard(const BYTE &cbFirstCardData, const WORD &cbNextCardData) = NULL;

	//出牌校验
	virtual bool __stdcall EfficacyOutCard(const WORD &wCheckUser, const BYTE *cbOutCardData, const BYTE &cbOutCardCount) = NULL;

	//根据指定花色提取扑克
	virtual BYTE __stdcall DistillCardByColor(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardColor, BYTE cbResultCard[]) = NULL;

	//提取扑克
	virtual BYTE __stdcall DistillCardByCount(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbSameCount, tagSameDataInfo &SameDataInfo) = NULL;

	//提取扑克
	virtual BYTE __stdcall DistillTrackorByCount(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbSameCount, tagTractorDataInfo &TractorDataInfo) = NULL;

	//判断两个数组的交集数目
	virtual BYTE __stdcall GetIntersectionCount(const BYTE cbCardData1[], BYTE cbCardCount1, const BYTE cbCardData2[], BYTE cbCardCount2) = NULL;

	//根据一轮的开始出牌玩家，分析玩家可以出的牌
	virtual bool __stdcall AnalyseOutCardNotify(const WORD &wChairID, const BYTE *OutCardData, 
											   const BYTE &cbOutCardNum, tagOutCardNotify *cbOutCardNotify) = NULL;

	//判断用户手牌中是否有指定花色的该类型的卡牌
	virtual bool __stdcall IsHaveSpecificType(const WORD &wChairID, const BYTE &color, const BYTE &cbOutCardType, 
												const BYTE &cbOutCardNum, tagOutCardNotify &cbOutCardNotify) = NULL;

	//计算抄底得分
	virtual SCORE __stdcall CalculateChaodiScore(const BYTE &cbCDcardNum) = NULL;

	//根据得分计算玩家升级级数
	virtual BYTE __stdcall AnalyseUpGrade(const SCORE &score) = NULL;

	//斗地主比较大小
	virtual bool __stdcall AnalysebCompare(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult &AnalyseResult, int dCardType) = NULL;

	//斗地主分析扑克
	virtual void __stdcall AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult &AnalyseResult) = NULL;

	//斗地主获取类型结果
	virtual int __stdcall GetType(tagAnalyseResult AnalyseResult,const BYTE cbCardData[],BYTE cbCardCount,const BYTE NoChangeCard[]) = NULL;

	//辅助函数
public:
	//获得卡牌花色
	BYTE GetCardColor(BYTE cbCardData) {return ((cbCardData&MASK_COLOR)>>4); }
	//获得卡牌点数
	BYTE GetCardValue(BYTE cbCardData) {return (cbCardData&MASK_VALUE); }
	//扑克索引转换成数据
	virtual BYTE __stdcall SwitchToCardData(BYTE cbCardIndex) = NULL;
	//扑克数据转换成索引
	virtual BYTE __stdcall SwitchToCardIndex(BYTE cbCardData) = NULL;
	//扑克索引数据转换数据数组
	virtual BYTE __stdcall SwitchToCardData(const BYTE cbCardIndex[LEN_MAX_INDEX], BYTE cbCardData[MAX_CARD_COUNT]) = NULL;
	//扑克数据数组转换成索引数组
	virtual BYTE __stdcall SwitchToCardIndex(const BYTE *cbCardData, BYTE cbCardCount, BYTE *cbCardIndex) = NULL;
	//删除扑克
	virtual bool __stdcall RemoveCard(const BYTE *cbRemoveCard, BYTE cbRemoveCount, BYTE *cbCardData, BYTE cbCardCount) = NULL;
	//从玩家手牌中删除扑克
	virtual bool __stdcall RemoveCard(const WORD &wChairID, const BYTE *cbRemoveCard, BYTE cbRemoveCount) = NULL;
};

/***************  游戏信息查询接口 ***********************/
interface CGameAccess
{
	// 查询当前游戏类型ID
	virtual int __stdcall SetKindID(DWORD id) = NULL;

	// 查询当前游戏类型ID
	virtual DWORD __stdcall GetKindID() = NULL;

	// 查询客户端的牌组
	virtual int __stdcall GetClientHandCards( WORD wChairID, BYTE *cards, const int &cardsum ) = NULL;

	// 查询玩家小局得分
	virtual SCORE __stdcall GetPlayerSingleScore( WORD wChairID ) = NULL;

	// 查询玩家房间内总得分
	virtual SCORE __stdcall GetPlayerTotalScore( WORD wChairID ) = NULL;

	// 查询玩家赢局数
	virtual int __stdcall GetWinSum( WORD wChairID, BYTE &winsum ) = NULL;

	// 查询当前局数
	virtual BYTE __stdcall GetCurGameCount() = NULL;

	// 设置当前局数
	virtual int __stdcall SetGameCount( BYTE count ) = NULL;

	// 查询总局数
	virtual BYTE __stdcall GetAllCount() = NULL;

	// 设置总局数
	virtual int __stdcall SetAllCount( BYTE count ) = NULL;

	// 查询房间底分
	virtual DWORD __stdcall GetCellScore() = NULL;

	// 设置房间底分
	virtual int __stdcall SetCellScore( DWORD score ) = NULL;

	//****************查询/设置用户游戏中状态*********************//

	// 设置玩家抢庄做庄
	virtual int __stdcall SetBankerState( WORD wChairID, BYTE care  ) = NULL;

	// 获取玩家抢庄状态
	virtual BYTE __stdcall GetBankerState( WORD wChairID) = NULL;

	// 查询玩家下注完成标志
	virtual BYTE __stdcall GetAddScoreState( WORD wChairID) = NULL;

	// 设置玩家下注完成标志
	virtual int __stdcall SetAddScoreState( WORD wChairID, BYTE beted ) = NULL;

	// 查询玩家动作完成标志
	virtual BYTE __stdcall GetUserOperateState( WORD wChairID) = NULL;

	// 设置玩家动作完成标志
	virtual int __stdcall SetUserOperateState( WORD wChairID,  BYTE state ) = NULL;

	// 查询玩家摊牌
	virtual BYTE __stdcall GetShowCardState( WORD wChairID ) = NULL;

	// 设置玩家摊牌
	virtual int __stdcall SetShowCardState( WORD wChairID, BYTE state ) = NULL;

	// 查询玩家准备状态
	virtual BYTE __stdcall GetXjReadyState( WORD wChairID ) = NULL;
	
	// 设置玩家准备
	virtual int __stdcall SetXjReadyState( WORD wChairID, BYTE ready ) = NULL;

	//*****************************end**************************//

	// 获取下注数
	virtual DWORD __stdcall GetAllBet( WORD wChairID) = NULL;

	// 查询最大椅子数
	virtual BYTE __stdcall GetMaxChairCount() = NULL;

	// 设置最大椅子数
	virtual int __stdcall SetMaxChairCount( BYTE count ) = NULL;

	//added by lizhihu 2017.10.20 设置游戏加倍模式：经典模式或疯狂加倍模式
	virtual int __stdcall SetGameScoreMode(BYTE nMode) = NULL;

	//获得游戏加倍模式
	virtual BYTE __stdcall GetGameScoreMode() = NULL;

	// 查询当前玩家数
	virtual BYTE __stdcall GetCurPlayerCount() = NULL;

	// 查询当前旁观者数   MaxChair = CurPlayer + CurLooker + voidChair
	virtual BYTE __stdcall GetCurLookerCount() = NULL;

	// 设置当前庄家
	virtual int __stdcall SetBankerID(const WORD &wChairID) = NULL;

	// 查询当前庄家
	virtual WORD __stdcall GetBankerID() = NULL;

	// 获取下个玩家  逆时针获取
	virtual WORD __stdcall GetNextUser(const WORD &wChariID) = NULL;

	// 获取上个玩家  顺时针获取
	virtual WORD __stdcall GetLastUser(const WORD &wChariID) = NULL;

	// 获得对家
	virtual WORD __stdcall GetOppositeUser(const WORD &wChariID) = NULL;

	// 设置用户状态，从服务器框架获得，用户坐下、准备、旁观、游戏中
	virtual int __stdcall SetPlayerState( WORD wChairID, DWORD state ) = NULL;

	// 查询用户状态，用户坐下、准备、旁观、游戏中
	virtual DWORD __stdcall GetPlayerState( WORD wChairID) = NULL;
	
	// 设置游戏状态
	virtual int __stdcall SetGameState( DWORD state ) = NULL;

	// 查询游戏状态
	virtual int __stdcall GetGameState( DWORD &state ) = NULL;

	// 设置房间规则 added by lizhihu 2017.10.17 使用 void* + size 取代固定结构体
	virtual bool __stdcall SetRoomRule(VOID * pData, WORD wDataSize) = 0;

	//设置游戏抢庄模式：0-抢庄		1-轮庄		2-固定庄
	virtual int __stdcall SetRobBankMode(BYTE nMode) = NULL;

	//获得游戏抢庄模式：0-抢庄		1-轮庄		2-固定庄
	virtual BYTE __stdcall GetRobBankMode() = NULL;

	//设置玩家抢庄分数
	virtual int __stdcall SetUserRobScore(const WORD &wChairID, BYTE score) = NULL;

	//设置玩家抢庄次数
	virtual int __stdcall SetUserRobNum(const WORD &wChairID, BYTE cbNum) = NULL;

	//获得玩家抢庄次数
	virtual BYTE __stdcall GetUserRobNum(const WORD &wChairID) = NULL;

	//设置房主
	virtual int __stdcall SetRoomFangzhu(WORD nFangzhu_id) = NULL;

	//获得房主id
	virtual WORD __stdcall GetRoomFangzhu() = NULL;

	//查询用户的初始卡牌数目
	virtual int _stdcall GetPlayerInitCardNum() = NULL;

	//设置用户的初始卡牌数目
	virtual int _stdcall SetPlayerInitCardNum() = NULL;

	//获得上局赢家椅子号
	virtual WORD _stdcall GetLastGameWinner() = NULL;

	//设置赢家椅子号
	virtual int _stdcall SetLastGameWinner(const WORD &wChairID) = NULL;

	//设置升级主花色
	virtual int _stdcall SetMainColor(const BYTE &color) = NULL;

	//获取升级主花色
	virtual BYTE _stdcall GetMainColor() = NULL;

	//设置亮主玩家
	virtual int _stdcall SetLiangZhuUser(const WORD &wChairID) = NULL;

	//获取亮主玩家
	virtual WORD _stdcall GetLiangZhuUser() = NULL;

	//设置当前级牌
	virtual int _stdcall SetLevelValue(const BYTE &value) = NULL;

	//获取当前级牌
	virtual BYTE _stdcall GetLevelValue() = NULL;

	//获得底牌
	virtual void _stdcall GetLeaveCard(BYTE *cbLeaveCard, int nLeaveCardSize) = NULL;

	//获得玩家当前卡牌数目
	virtual BYTE _stdcall GetUserCurCardNum(const WORD &wChairID) = NULL;

	//获得玩家当手牌中的主牌个数
	virtual BYTE _stdcall GetUserMainColorNum(const WORD &wChairID) = NULL;

	// 设置当前出牌用户
	virtual int __stdcall SetCurOutCardUser(const WORD &wChairID) = NULL;

	// 获得当前出牌用户
	virtual WORD __stdcall GetCurOutCardUser() = NULL;

	// 设置一轮中最先开始出牌的用户
	virtual int __stdcall SetStartOutCardUser(const WORD &wChairID) = NULL;

	// 获得一轮中最先开始出牌的用户
	virtual WORD __stdcall GetStartOutCardUser() = NULL;

	// 设置玩家出牌卡牌数据
	virtual bool __stdcall SetUserOutCard(const WORD wChairID, const BYTE *cbCardData, const BYTE &cbCardNum) = NULL;

	// 获得玩家出牌卡牌数据
	virtual BYTE __stdcall GetUserOutCard(const WORD wChairID, BYTE *cbCardData) = NULL;

	// 获得底牌数目
	virtual BYTE __stdcall GetLeaveCardNum() = NULL;

	// 设置当前反主卡牌
	virtual int __stdcall SetCurFanZhuCard(const BYTE &card) = NULL;

	// 获得玩家当前级数
	virtual int __stdcall GetUserCurLevel(const WORD &wChairID) = NULL;

	// 设置当前反主玩家
	virtual int __stdcall SetCurFanzhuUser(const WORD &wChairID) = NULL;

	// 获得当前反主玩家
	virtual WORD __stdcall GetCurFanzhuUser() = NULL;

	// 设置当前扣底玩家
	virtual int __stdcall SetCurKoudiUser(const WORD &wChairID) = NULL;

	// 获得当前扣底玩家
	virtual WORD __stdcall GetCurKoudiUser() = NULL;

	// 设置最开始抢庄玩家
	virtual int __stdcall SetStartRobUser(const WORD &wChairID) = NULL;

	// 获得最开始抢庄玩家
	virtual WORD __stdcall GetStartRobUser() = NULL;

	// 设置一轮结束
	virtual int __stdcall SetOneTurnEnd(const BYTE &cbEnd) = NULL;

	// 获得一轮结束
	virtual BYTE __stdcall GetOneTurnEnd() = NULL;

	// 设置一轮中最大卡牌数据
	virtual int __stdcall SetTurnMaxCards(BYTE *cbCardData, BYTE cbCardNum) = NULL;

	// 获得一轮中最大卡牌数据
	virtual BYTE __stdcall GetTurnMaxCards(BYTE *cbCardData, BYTE &cbCardNum) = NULL;

	// 获得赖子数目
	virtual BYTE __stdcall GetMagicCardNum(const BYTE cbHandCardData[], BYTE cbHandCardCount) = NULL;

	// 获得玩家手牌赖子数目
	virtual BYTE __stdcall GetMagicCardNum(const WORD &wChairID) = NULL;

	//设置当前癞子
	virtual int __stdcall SetMagicCard(const BYTE &cbCard) = NULL;

	//获得当前癞子
	virtual BYTE __stdcall GetMagicCard() = NULL;

	//设置玩家炸弹信息
	virtual int __stdcall SetUserBoomInfo(const WORD &wChairID, const BYTE &cbBoomType) = NULL;

	//获得玩家炸弹信息
	virtual BYTE __stdcall GetUserBoomInfo(const WORD &wChairID, tagBoomInfo &strBoomInfo) = NULL;

	
};

#endif