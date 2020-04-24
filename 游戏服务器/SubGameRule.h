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

	// 分析用户牌型
	virtual int __stdcall AnsysCardsType( );

	// 结算得分
	virtual int __stdcall Settle();

//算分
protected:
	//升级算分
	int SettleUpGrade();

	//斗地主算分
	int SettleFightLandLord();
};

#endif