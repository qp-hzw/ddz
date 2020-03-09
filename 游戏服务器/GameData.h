#ifndef GAME_DATA_L
#define GAME_DATA_L

#include "GameInterface.h"

class CGameData : public CGameLogic, public CGameAccess
{
public:

	CGameData();
	~CGameData() {}

// 游戏规则接口
public:
	// 创建游戏
	virtual int __stdcall CreateGame(CGameLogic **pGameRule, CGameAccess **pGameAccess);

	// 加载读取的配置文件参数
	virtual int __stdcall LoadConfigParameter(const wchar_t *_logFile, const BYTE cbGameScoreMode);

	//初始化数据
	virtual int __stdcall Init();

	// 玩家下注
	virtual int __stdcall Wager(WORD wChairID, float bet);

	// 根据抢庄模式，指定庄家
	virtual bool __stdcall AppointBanker(WORD wChairID);

	//获得当前指定的庄家
	virtual WORD __stdcall GetAppointBanker();

	// 发牌
	virtual int __stdcall Deal();

	// 设置玩家操作
	virtual int __stdcall SetUserOperate(WORD wChairID, BYTE byAction, DWORD dwActionScore);

	// 分析用户牌型
	virtual int __stdcall AnsysCardsType();

	// 结算得分
	virtual int __stdcall Settle();

	// 清理小局游戏数据
	virtual int __stdcall ClearXjGame();

	// 清理大局游戏数据
	virtual int __stdcall ClearDjGame();

	// 清理房间数据
	virtual int __stdcall ClearRoom();

	//按花色权位排序扑克
	virtual void __stdcall SortCardList(BYTE *cbCardData, BYTE cbCardCount);

	//按花色权位给用户手牌排序
	virtual void __stdcall SortCardList(const WORD &wChairID, BYTE cbCardCount, BYTE cbSortType);

	//斗地主排序
	virtual void __stdcall SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);


	//获得卡牌的排序等级
	virtual BYTE __stdcall GetCardSortOrder(BYTE cbCardData);

	//分析玩家是否能亮主
	virtual bool __stdcall AnalyseUserLiangZhu(const WORD &wChairID, BYTE *cbLiangZhuColor);

	//分析玩家是否能反主
	virtual bool __stdcall AnalyseUserFanZhu(const WORD &wChairID, BYTE *cbFanZhuCard);

	//判断玩家手牌是否能反主
	virtual bool __stdcall IsUserCanFanZhu(const WORD &wChairID, const BYTE &cbCurFanZhuColor, BYTE *cbFanZhuCard);

	//发送底牌给玩家
	virtual void __stdcall SendLeaveCardToUser(const WORD &wChairID);

	//获得一组卡牌的牌类型
	virtual int __stdcall GetCardLogicType(const BYTE *cbCardData, BYTE cbCardCount);

	//获得卡牌的逻辑花色
	virtual BYTE __stdcall GetCardLogicColor(BYTE cbCardData);

	//获得卡牌数组的逻辑花色
	virtual BYTE __stdcall GetCardLogicColor(const BYTE *cbCardData, BYTE cbCardCount);

	//获得卡牌的逻辑数值
	virtual BYTE __stdcall GetCardLogicValue(BYTE cbCardData);

	//两张卡牌是否相连
	virtual bool __stdcall IsLineValue(BYTE cbTurnCard, BYTE cbSecondCard);

	//一轮比牌，判断赢家
	virtual WORD __stdcall GetOneRoundWinner();

	//计算一轮输赢得分
	virtual WORD __stdcall SettleOneRound(const WORD &wWinner);

	//获得一轮中的所有分牌
	virtual BYTE __stdcall GetScoreCard();

	//获取卡牌数组总分数
	virtual WORD __stdcall GetAllCardScore(const BYTE *cbCardData, BYTE cbCardCount);

	//比较玩家出牌大小
	virtual bool __stdcall CompareUserOutCard(const BYTE &cbFirstCardData, const WORD &cbNextCardData);

	//出牌校验
	virtual bool __stdcall EfficacyOutCard(const WORD &wCheckUser, const BYTE *cbOutCardData, const BYTE &cbOutCardCount);

	//根据指定花色提取扑克
	virtual BYTE __stdcall DistillCardByColor(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardColor, BYTE cbResultCard[]);

	//提取扑克
	virtual BYTE __stdcall DistillCardByCount(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbSameCount, tagSameDataInfo &SameDataInfo);

	//提取扑克
	virtual BYTE __stdcall DistillTrackorByCount(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbSameCount, tagTractorDataInfo &TractorDataInfo);

	//判断两个数组的交集数目
	virtual BYTE __stdcall GetIntersectionCount(const BYTE cbCardData1[], BYTE cbCardCount1, const BYTE cbCardData2[], BYTE cbCardCount2);

	//根据一轮的开始出牌玩家，分析玩家可以出的牌
	virtual bool __stdcall AnalyseOutCardNotify(const WORD &wChairID, const BYTE *OutCardData,
		const BYTE &cbOutCardNum, tagOutCardNotify *cbOutCardNotify);

	//判断用户手牌中是否有指定花色的该类型的卡牌
	virtual bool __stdcall IsHaveSpecificType(const WORD &wChairID, const BYTE &color, const BYTE &cbOutCardType,
		const BYTE &cbOutCardNum, tagOutCardNotify &cbOutCardNotify);

	//计算抄底得分
	virtual SCORE __stdcall CalculateChaodiScore(const BYTE &cbCDcardNum);

	//根据得分计算玩家升级级数
	virtual BYTE __stdcall AnalyseUpGrade(const SCORE &score);

	//斗地主比较大小
	virtual bool __stdcall AnalysebCompare(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult &AnalyseResult, int dCardType);

	//斗地主分析扑克
	virtual void __stdcall AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult &AnalyseResult);

	//斗地主获取类型结果
	virtual int __stdcall GetType(tagAnalyseResult AnalyseResult, const BYTE cbCardData[], BYTE cbCardCount, const BYTE NoChangeCard[]);

	//判断当前玩家是否能出牌 
	virtual bool __stdcall JudgePlayerOutCard(WORD wCurOutCardUser);

	//玩家第一个出牌  判断他可以出的合适的牌
	virtual bool __stdcall AnalysePlayerOutCardFirst(WORD OutCardUsr, BYTE OutCardData[], BYTE *OutCardNum);

	//玩家接牌  判断他可以出的牌
	virtual bool __stdcall AnalysePlayerOutCard(WORD OutCardUsr, BYTE OutCardData[], BYTE *OutCardNum);

	//记牌器 返回除当前玩家外其他玩家各手牌的数量
	virtual bool __stdcall CardRecorder(WORD wChairID, tagCardRecorder &Recorder);

	//初始化当前轮手牌给机器人
	virtual void __stdcall GetTurnMaxCardToRobot(GameSituation &gst, AILogic &m_AILogic);

	//将要出的牌从逻辑值转换成有花色
	virtual void __stdcall GetCardColorValue(WORD OutCardUser, HandCardData &m_HandCardData, BYTE *OutCardData, BYTE &OutCardNum);

	//计算底牌是否翻倍 并返回倍数
	virtual DWORD __stdcall IsDoubleLeaveCard();

	//获取出牌玩家动作行为
	virtual BYTE __stdcall GetOutCardActionType(WORD wChairID);

	//================================================癞子场判断=================================================

	//获得于包含癞子的卡牌的逻辑数值
	virtual BYTE GetCardLogicValueLaiZi(BYTE cbCardData);

	//将玩家手牌中的癞子进行牌值替换
	virtual bool ReplaceLaiZiCard(WORD wChairID, BYTE cbCardNum);

	//斗地主不考虑癞子的排序
	virtual void SortCardListNoLaiZi(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);

	//判断癞子场的玩家是否能出牌
	virtual bool JudgeLaiZiPlayerOutCard(WORD wChairID);

	//癞子：玩家第一个出牌  判断他可以出的合适的牌
	virtual bool LaiZiAnalysePlayerOutCardFirst(WORD OutCardUsr, BYTE OutCardData[], BYTE *OutCardNum);

	//癞子：玩家接牌
	virtual bool LaiZiAnalysePlayerOutCard(WORD OutCardUsr, BYTE OutCardData[], BYTE *OutCardNum);

	//癞子：分析比较手牌
	virtual bool LaiZiCompareCard(BYTE Card[], BYTE CardCount, BYTE OutCardData[]/*out*/, BYTE *OutCardNum);

	//辅助函数
	//扑克转换
	virtual BYTE __stdcall SwitchToCardData(BYTE cbCardIndex);

	//扑克转换
	virtual BYTE __stdcall SwitchToCardIndex(BYTE cbCardData);

	//扑克转换
	virtual BYTE __stdcall SwitchToCardData(const BYTE cbCardIndex[LEN_MAX_INDEX], BYTE cbCardData[MAX_CARD_COUNT]);

	//扑克转换
	virtual BYTE __stdcall SwitchToCardIndex(const BYTE *cbCardData, BYTE cbCardCount, BYTE *cbCardIndex);

	//删除扑克
	virtual bool __stdcall RemoveCard(const BYTE *cbRemoveCard, BYTE cbRemoveCount, BYTE *cbCardData, BYTE cbCardCount);

	//从玩家手牌中删除扑克
	virtual bool __stdcall RemoveCard(const WORD &wChairID, const BYTE *cbRemoveCard, BYTE cbRemoveCount);

// 信息查询接口CGameAccess
public:
	// 查询当前游戏类型ID
	virtual int __stdcall SetKindID(DWORD id);

	// 查询当前游戏类型ID
	virtual DWORD __stdcall GetKindID();

	// 查询客户端的牌组
	virtual int __stdcall GetClientHandCards(WORD wChairID, BYTE *cards, const int &cardsum);

	// 查询玩家小局得分
	virtual SCORE __stdcall  GetPlayerSingleScore(WORD wChairID);

	// 查询玩家房间内总得分
	virtual SCORE __stdcall GetPlayerTotalScore(WORD wChairID);

	//记录玩家庄家次数  大局结算
	virtual int __stdcall SetPlayerBankCount(WORD wChairID);

	//获取玩家庄家次数  
	virtual int __stdcall GetPlayerBankCount(WORD wChairID);

	// 设置玩家赢局数
	virtual int __stdcall SetWinSum(WORD wChairID, BYTE winsum);

	// 查询玩家赢局数
	virtual int __stdcall GetWinSum(WORD wChairID, BYTE &winsum);

	// 查询当前局数
	virtual BYTE __stdcall GetCurGameCount();

	// 设置当前局数
	virtual int __stdcall SetGameCount(BYTE count);

	// 查询总局数
	virtual BYTE __stdcall GetAllCount();

	// 设置总局数
	virtual int __stdcall SetAllCount(BYTE count);

	// 查询房间底分
	virtual DWORD __stdcall GetCellScore();

	// 设置玩家房间内总得分
	virtual DWORD __stdcall SetPlayerTotalScore(WORD wChairID, SCORE score);

	// 设置房间底分
	virtual int __stdcall SetCellScore(DWORD score);

	//****************查询/设置用户游戏中状态*********************//

	// 设置当前抢庄次数
	virtual int __stdcall SetCurBankerCount(BYTE count);

	// 获取当前抢庄次数
	virtual BYTE __stdcall GetCurBankerCount();

	// 设置玩家抢庄做庄
	virtual int __stdcall SetBankerState(WORD wChairID, BYTE cbResult);

	// 获取玩家抢庄状态
	virtual BYTE __stdcall GetBankerState(WORD wChairID);

	// 查询玩家下注完成标志
	virtual BYTE __stdcall GetAddScoreState(WORD wChairID);

	// 设置玩家下注完成标志
	virtual int __stdcall SetAddScoreState(WORD wChairID, BYTE beted);

	// 查询玩家明牌完成标志
	virtual BYTE __stdcall GetMingPaiState(WORD wChairID);

	// 设置玩家明牌完成标志
	virtual int __stdcall SetMingPaiState(WORD wChairID, BYTE state);

	// 查询玩家动作完成标志
	virtual BYTE __stdcall GetUserOperateState(WORD wChairID);

	// 设置玩家动作完成标志
	virtual int __stdcall SetUserOperateState(WORD wChairID, BYTE state);

	// 查询玩家摊牌
	virtual BYTE __stdcall GetShowCardState(WORD wChairID);

	// 设置玩家摊牌
	virtual int __stdcall SetShowCardState(WORD wChairID, BYTE state);

	// 查询玩家准备状态
	virtual BYTE __stdcall GetXjReadyState(WORD wChairID);

	// 设置玩家准备
	virtual int __stdcall SetXjReadyState(WORD wChairID, BYTE ready);

	//*****************************end**************************//

	// 获取下注数
	virtual DWORD __stdcall GetAllBet(WORD wChairID);

	// 查询最大椅子数
	virtual BYTE __stdcall GetMaxChairCount();

	// 设置最大椅子数
	virtual int __stdcall SetMaxChairCount(BYTE count);

	//added by lizhihu 2017.10.20 设置游戏加倍模式：经典模式或疯狂加倍模式
	virtual int __stdcall SetGameScoreMode(BYTE nMode);

	//获得游戏加倍模式
	virtual BYTE __stdcall GetGameScoreMode();

	// 查询当前玩家数
	virtual BYTE __stdcall GetCurPlayerCount();

	// 查询当前旁观者数   MaxChair = CurPlayer + CurLooker + voidChair
	virtual BYTE __stdcall GetCurLookerCount();

	// 设置当前庄家
	virtual int __stdcall SetBankerID(const WORD &wChairID);

	// 查询当前庄家
	virtual WORD __stdcall GetBankerID();

	// 获取下个玩家  逆时针获取
	virtual WORD __stdcall GetNextUser(const WORD &wChariID);

	// 获取上个玩家  顺时针获取
	virtual WORD __stdcall GetLastUser(const WORD &wChariID);

	// 获得对家
	virtual WORD __stdcall GetOppositeUser(const WORD &wChariID);

	// 设置用户状态，从服务器框架获得，用户坐下、准备、旁观、游戏中
	virtual int __stdcall  SetPlayerState(WORD wChairID, DWORD state);

	// 查询用户状态，用户坐下、准备、旁观、游戏中
	virtual DWORD __stdcall GetPlayerState(WORD wChairID);

	// 设置游戏房间状态
	virtual int __stdcall SetGameStatus(DWORD state);

	// 查询游戏状态
	virtual DWORD __stdcall GetGameStatus();

	// 设置房间规则 added by lizhihu 2017.10.17 使用 void* + size 取代固定结构体
	virtual bool __stdcall SetRoomRule(tagTableCfg * pRoomRuleOption);

	//设置游戏抢庄模式：0-抢庄		1-轮庄		2-固定庄
	virtual int __stdcall SetRobBankMode(BYTE nMode);

	//获得游戏抢庄模式：0-抢庄		1-轮庄		2-固定庄
	virtual BYTE __stdcall GetRobBankMode();

	//设置玩家抢庄分数
	virtual int __stdcall SetUserRobScore(const WORD &wChairID, BYTE score);

	//设置玩家抢庄次数
	virtual int __stdcall SetUserRobNum(const WORD &wChairID, BYTE cbNum);

	//获得玩家抢庄次数
	virtual BYTE __stdcall GetUserRobNum(const WORD &wChairID);

	//设置房主
	virtual int __stdcall SetRoomFangzhu(WORD nFangzhu_id);

	//获得房主id
	virtual WORD __stdcall GetRoomFangzhu();

	//查询用户的初始卡牌数目
	virtual int  __stdcall GetPlayerInitCardNum();

	//设置用户的初始卡牌数目
	virtual int  __stdcall SetPlayerInitCardNum();

	//获得上局赢家椅子号
	virtual WORD  __stdcall GetLastGameWinner();

	//设置赢家椅子号
	virtual int  __stdcall SetLastGameWinner(const WORD &wChairID);

	//设置升级主花色
	virtual int __stdcall SetMainColor(const BYTE &color);

	//获取升级主花色
	virtual BYTE  __stdcall GetMainColor();

	//设置亮主玩家
	virtual int  __stdcall SetLiangZhuUser(const WORD &wChairID);

	//获取亮主玩家
	virtual WORD  __stdcall GetLiangZhuUser();

	//设置当前级牌
	virtual int  __stdcall SetLevelValue(const BYTE &value);

	//获取当前级牌
	virtual BYTE  __stdcall GetLevelValue();

	//获得底牌
	virtual void  __stdcall GetLeaveCard(BYTE *cbLeaveCard, int nLeaveCardSize);

	//获得玩家当前卡牌数目
	virtual BYTE  __stdcall GetUserCurCardNum(const WORD &wChairID);

	//获得玩家当手牌中的主牌个数
	virtual BYTE  __stdcall GetUserMainColorNum(const WORD &wChairID);

	// 设置当前出牌用户
	virtual int __stdcall SetCurOutCardUser(const WORD &wChairID);

	// 获得当前出牌用户
	virtual WORD __stdcall GetCurOutCardUser();

	// 设置一轮中最先开始出牌的用户
	virtual int __stdcall SetStartOutCardUser(const WORD &wChairID);

	// 获得一轮中最先开始出牌的用户
	virtual WORD __stdcall GetStartOutCardUser();

	// 设置一轮中最后出牌的用户
	virtual int __stdcall SetLastOutCardUser(const WORD &wChairID);  //by hao

	// 获得一轮中最后出牌的用户
	virtual WORD __stdcall GetLastOutCardUser();

	// 设置玩家出牌卡牌数据
	virtual bool __stdcall SetUserOutCard(const WORD wChairID, const BYTE *cbCardData, const BYTE &cbCardNum);

	// 获得玩家出牌卡牌数据
	virtual BYTE __stdcall GetUserOutCard(const WORD wChairID, BYTE *cbCardData);

	// 获得底牌数目
	virtual BYTE __stdcall GetLeaveCardNum();

	// 设置当前反主卡牌
	virtual int __stdcall SetCurFanZhuCard(const BYTE &card);

	// 获得玩家当前级数
	virtual int __stdcall GetUserCurLevel(const WORD &wChairID);

	// 设置当前反主玩家
	virtual int __stdcall SetCurFanzhuUser(const WORD &wChairID);

	// 获得当前反主玩家
	virtual WORD __stdcall GetCurFanzhuUser();

	// 设置当前扣底玩家
	virtual int __stdcall SetCurKoudiUser(const WORD &wChairID);

	// 获得当前扣底玩家
	virtual WORD __stdcall GetCurKoudiUser();

	// 设置最开始抢庄玩家
	virtual int __stdcall SetStartRobUser(const WORD &wChairID);

	// 获得最开始抢庄玩家
	virtual WORD __stdcall GetStartRobUser();

	// 设置一轮中玩家是出牌还是过		
	virtual int __stdcall SetTurnOutCardType(const WORD wChairID, DWORD type);    //0- 过   1-出牌

	// 获取一轮中玩家是出牌还是过
	virtual DWORD __stdcall GetTurnOutCardType(const WORD wChairID);

	// 设置一轮结束
	virtual int __stdcall SetOneTurnEnd(const BYTE &cbEnd);

	// 获得一轮结束
	virtual BYTE __stdcall GetOneTurnEnd();

	// 设置一轮中最大卡牌数据
	virtual int __stdcall SetTurnMaxCards(BYTE *cbCardData, BYTE cbCardNum);

	// 获得一轮中最大卡牌数据
	virtual BYTE __stdcall GetTurnMaxCards(BYTE *cbCardData, BYTE &cbCardNum);

	// 获得赖子数目
	virtual BYTE __stdcall GetMagicCardNum(const BYTE cbHandCardData[], BYTE cbHandCardCount);

	// 获得玩家手牌赖子数目
	virtual BYTE __stdcall GetMagicCardNum(const WORD &wChairID);

	//设置当前癞子
	virtual int __stdcall SetMagicCard(const BYTE &cbCard);

	//获得当前癞子
	virtual BYTE __stdcall GetMagicCard();

	//设置玩家炸弹信息
	virtual int __stdcall SetUserBoomInfo(const WORD &wChairID, const DWORD cbBoomType);

	//获得玩家炸弹信息
	virtual BYTE __stdcall GetUserBoomInfo(const WORD &wChairID, tagBoomInfo &strBoomInfo);

	//设置当前抢庄的玩家
	virtual int __stdcall SetCurRobUser(const WORD wChairID);

	//获取当前抢庄的玩家
	virtual WORD __stdcall GetCurRobUser();

	//记录玩家下注
	virtual int __stdcall SetPlayerAddScore(WORD wChairID, WORD lScore);

	//获取玩家下注
	virtual WORD __stdcall GetPlayerAddScore(WORD wChairID);

	//记录玩家明牌倍数
	virtual int __stdcall SetPlayerMingPaiBet(WORD wChairID, WORD bet);

	//获取玩家明牌倍数
	virtual int __stdcall GetPlayerMingPaiBet(WORD wChairID);

	//设置一大局中玩家获得过的最大倍数
	virtual int __stdcall SetDJPlayerBestBet(WORD wChairID, SCORE bet);

	//获得一大局中玩家获得过的最大倍数
	virtual SCORE __stdcall GetDJPlayerBestBet(WORD wChairID);

	//设置游戏封顶倍数
	virtual int __stdcall SetRoomMaxBet(DWORD Max_Bet);

	//获取游戏封顶倍数
	virtual DWORD __stdcall GetRoomMaxBet();

	//记录玩家是否开始游戏明牌  5-开始游戏明牌
	virtual int __stdcall SetStartMingPai(WORD wChairID, DWORD state);

	//获取玩家是否开始游戏明牌
	virtual DWORD __stdcall GetStartMingPai(WORD wChairID);

	//设置一个参数记录叫分次数 
	virtual int __stdcall SetJiaoFenCount(WORD count);

	//获得叫分次数 
	virtual WORD __stdcall GetJiaoFenCount();

	//记录当前玩家的叫分状态
	virtual int __stdcall SetCurJiaoFenState(WORD wChairID, WORD state);

	//获得当前玩家的叫分状态
	virtual int __stdcall GetCurJiaoFenState(WORD wChairID);

	//设置当前正在叫分的玩家     
	virtual int __stdcall SetCurJiaoFenUser(WORD wChairID);

	//获取当前正在叫分的玩家     
	virtual WORD __stdcall GetCurJiaoFenUser();

	//清除各玩家的个人倍数
	virtual WORD __stdcall CleanAllBet();

	//获取不抢庄的轮数
	virtual WORD __stdcall GetTurnPassCount();

	//设置不抢庄的轮数
	virtual int __stdcall SetTurnPassCount(WORD count);

	//设置游戏癞子模式
	virtual int __stdcall SetLaiZiMode(WORD Mode);

	//获取游戏癞子模式
	virtual int __stdcall GetLaiZiMode();

	//获取底牌翻倍倍数
	virtual DWORD __stdcall GetLeaveCardBet();

	//设置是否底牌翻倍
	virtual int __stdcall SetISLeaveCardDouble(WORD Mode);

	//获取底牌是否翻倍
	virtual WORD __stdcall GetISLeaveCardDouble();

	//设置是否明牌
	virtual int __stdcall SetMingPaiMode(WORD Mode);

	//获取是否明牌
	virtual WORD __stdcall GetMingPaiMode();

	//设置是否加倍
	virtual int __stdcall SetAddBetMode(WORD Mode);

	//获取是否加倍
	virtual WORD __stdcall GetAddBetMode();

	//记录玩家是否出过牌
	virtual int __stdcall SetSpring(WORD wChairID, WORD spring);

	//判断玩家是否春天  0-不是 1-是
	virtual int __stdcall JudgeSpring(WORD wChairID);

	//获取玩家是否春天
	virtual int __stdcall GetSpring(WORD wChairID);

	//设置玩家托管
	virtual int __stdcall SetPlayerTuoGuan(WORD wChairID, WORD state);

	//获取玩家托管
	virtual int __stdcall GetPlayerTuoGuan(WORD wChairID);

	//设置玩家连续超时出牌次数
	virtual int __stdcall SetPlayerTimeOutNum(WORD wChairID, WORD num);

	//获取玩家连续超时出牌次数
	virtual int __stdcall GetPlayerTimeOutNum(WORD wChairID);

	//================================================癞子场判断=================================================

	//设置癞子当成的普通牌
	virtual int SetLaiZiToCommon(BYTE LaiZiData[], BYTE Count);

	//获取癞子当成的普通牌
	virtual int GetLaiZiToCommon(BYTE LaiZiData[], BYTE &Count);

protected:
	// 根据游戏配置加载卡牌数据
	int LoadGameCards( );

	// 根据游戏配置加载玩家数据
	int LoadPlayers( );

	// 加载置游戏规则
	int LoadGameRule(const BYTE cbGameScoreMode);

	// 单例创建日志
	int CreateLog( const wchar_t *_logFile );

	// 输出牌局信息
	int OutGameInfor();

	// 初始化全局数据
	int InitGameData( );

	void SetCardData_Leo(const WORD &wChairID, CARD_DESC *cards);

// 工具函数
protected:

	// 卡牌数据转换
	virtual int CardTrans_S2C( const CARD_DESC &card_s, BYTE &card_c ); // 服务端转客户端
	virtual int CardTrans_C2S( CARD_DESC &card_s, const BYTE &card_c ); // 客户端转服务器

	int TransListToInt(int iOldType,int iNewType);

public:

	GAME_CARD_CONFIG	m_card_config;		// 卡牌配置
	PLAYERS_CONFIG		m_players_config;	// 玩家配置
	ROOM_CONFIG			m_room_config;		// 游戏配置	
	PLAYING_PARAMETERS	m_playing_para;		// 游戏中参数
	CServerLog          *m_plog;			// 日志
	wchar_t             m_cfgFile[128];		// 配置文件

public:
	static const BYTE	m_cbCardData[FULL_COUNT];			//原始扑克数据
	
};

#endif