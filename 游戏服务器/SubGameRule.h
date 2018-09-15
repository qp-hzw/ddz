#ifndef GAME_RULE_L
#define GAME_RULE_L

#pragma once
#include "GameData.h"

class CSubGameRule : public CGameData
{
public:
	CSubGameRule(void) {}
	~CSubGameRule(void) {}

/** 游戏规则接口 **/
public:

	// 根据抢庄模式，指定庄家
	virtual bool __stdcall AppointBanker( WORD &wChairID );

	// 分析用户牌型
	virtual int __stdcall AnsysCardsType( );

	// 结算得分
	virtual int __stdcall Settle();


/** 抢庄处理 **/
protected:
	 // 轮庄处理
    bool AppointBankerTURN(WORD &wChairID);

    // 抢庄处理
    bool AppointBankerFREE(WORD &wChairID);

	// 固定庄处理
    bool AppointBankerFIXED(WORD &wChairID);

//算分
protected:
	//升级算分
	int SettleUpGrade();

	//斗地主算分
	int SettleFightLandLord();
};

#endif