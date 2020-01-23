#include "stdafx.h"
#include "GameConfig.h"

#include <time.h>

std::string      CGameConfig::cfg_file;             // 配置文件

COM_PROFILE_RULE	CGameConfig::gComGamePara ;	    // 游戏配置
COM_CARD_CONFIG		CGameConfig::gComCardPara;		// 卡牌配置
COM_PLAYER_CONFIG	CGameConfig::gComPlayerPara;    // 玩家配置
 

CGameConfig::CGameConfig(void)
{

}

CGameConfig::~CGameConfig(void)
{

}


//从配置文件中获取通用的游戏配置，在游戏开始前赋给全局变量
int CGameConfig::LoadGameCommonConfig()
{
	int result = 0;

	// 绑定配置文件
	if (0 == BindCfgFile())
	{
		return -1;
	}

	//打开文件
	result = CCfg::Instance()->OpenFile(cfg_file.c_str());
	if(result != 0)
	{
		cout << result << endl;
		return - 2;
	}

	// 创建游戏卡牌和玩家，并获得手牌倍数配置
	result = CreateGameCards();		// 创建卡牌，给pGameRule->m_card_config赋值
	if (0 == result)
	{
		result = CreatePlayers(); // 创建玩家，给pGameRule->m_players_config赋值
		if (0 != result)
		{
			////CLog::Log(log_error, "配置玩家异常");
		}
	}
	else
	{
		////CLog::Log(log_error, "配置卡牌异常");
	}

	//关闭文件
	CCfg::Instance()->CloseFile();

	return result;
}

// 根据游戏配置创建卡牌数据
int CGameConfig::CreateGameCards()
{
	int result = 0;

	//提取组
	result = CGameCardConfig::LoadCardGroups(gComCardPara.group, gComCardPara.groupNum);

	if (0 == result)
	{
		// 获取配置中一副纸牌的总数和发到每个用户的牌数
		result = CGameCardConfig::LoadCardsSum(gComCardPara.game_cards_num);
		if (0 == result)
		{
			result = CGameCardConfig::TransGroups(gComCardPara.group, gComCardPara.groupNum, gComCardPara.game_cards, gComCardPara.game_cards_num);	// 解析牌
			if (0 == result)
			{
				//读取配置文件对应的手牌倍数
				CGameCardConfig::LoadGameScoreTimes(GAME_SCORE_MODE_CLASSIC, gComGamePara.game_normal_times);
				//获得疯狂加倍倍数
				CGameCardConfig::LoadGameScoreTimes(GAME_SCORE_MODE_CRAZY, gComGamePara.game_crazy_times);
			}
			if (result == -1)
			{
				////CLog::Log(log_error, "解析得分倍数失败");
			}
		}
		else
		{
			////CLog::Log(log_error, "解析牌失败");
			result = -1;
		}
	}
	else
	{
		////CLog::Log(log_error, "提组失败");
		result = -1;
	}
	return result;
}

// 根据游戏配置创建玩家数据
int CGameConfig::CreatePlayers()
{
	int result = 0;

	gComPlayerPara.type = 0;

	// 从配置文件中解析玩家组数组
	//PLAYER_TYPES用一个word存储了玩家的三个数据ID,card_sum，player_sum
	result = CGamePlayerConfig::LoadPlayerDesc(gComPlayerPara.type);

	return result;

}

// 绑定配置文件
DWORD CGameConfig::BindCfgFile()
{
	//cfg_file = "./subgame/"
	//	+ std::to_string(KIND_ID)
	//	+ ".cfg";

	cfg_file = std::to_string(KIND_ID)+ ".cfg";

	return KIND_ID;
}