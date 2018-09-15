#include "stdafx.h"
#include "GameConfig.h"
#include "ServerLog.h"

#import "msxml6.dll"
using namespace MSXML2;
#include <time.h>

COM_PROFILE_RULE	gComGamePara;		// 游戏配置
COM_CARD_CONFIG		gComCardPara;		// 卡牌配置
COM_PLAYER_CONFIG	gComPlayerPara;		// 玩家配置

CServerLog          *m_plog;			//日志文件
wchar_t             m_cfgFile[128];		//配置文件


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

	// 创建日志
	CServerLog::GetLogInstance( L"SUB_GAMES_LOG.txt", &m_plog );

	// 绑定配置文件
	if(0 == BindCfgFile())
	{
		m_plog->LogSingleLine(L"读取配置文件失败, 未找到KindID", NULL );
		return 1;
	}

	// 创建游戏卡牌和玩家，并获得手牌倍数配置
	result = CreateGameCards();		// 创建卡牌，给pGameRule->m_card_config赋值
	if ( 0 == result )
	{
		result = CreatePlayers(); // 创建玩家，给pGameRule->m_players_config赋值
		if ( 0 != result )
		{
			m_plog->LogSingleLine(L"配置玩家异常", NULL );
		}
	}
	else
	{
		m_plog->LogSingleLine(L"配置卡牌异常", NULL );
	}

	return result;
}

// 根据游戏配置创建卡牌数据
int CGameConfig::CreateGameCards( )
{
	int result = 0;

	//提取组
	result = CGameCardConfig::LoadCardGroups(m_cfgFile, gComCardPara.group, gComCardPara.groupNum, m_plog);  

	if (0 == result)
	{
		// 获取配置中一副纸牌的总数和发到每个用户的牌数
		result = CGameCardConfig::LoadCardsSum(m_cfgFile, gComCardPara.game_cards_num, m_plog); 

		if (0 == result)
		{
			result = CGameCardConfig::TransGroups(gComCardPara.group, gComCardPara.groupNum, gComCardPara.game_cards, gComCardPara.game_cards_num, m_plog);	// 解析牌
			if (0 == result)
			{
				//读取配置文件对应的手牌倍数
				CGameCardConfig::LoadGameScoreTimes(m_cfgFile, GAME_SCORE_MODE_CLASSIC, gComGamePara.game_normal_times, m_plog);

				//获得疯狂加倍倍数
				CGameCardConfig::LoadGameScoreTimes(m_cfgFile, GAME_SCORE_MODE_CRAZY, gComGamePara.game_crazy_times, m_plog);

				if (result == -1)
				{
					m_plog->LogSingleLine(L"解析得分倍数失败", NULL);
				}
			}
		}
		else
		{
			m_plog->LogSingleLine(L"解析牌失败", NULL);
			result = -1;
		}
	}
	else
	{
		m_plog->LogSingleLine(L"提组失败", NULL);
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
	result = CGamePlayerConfig::LoadPlayerDesc(	m_cfgFile, gComPlayerPara.type, m_plog );

	return result;
}

// 绑定配置文件
DWORD CGameConfig::BindCfgFile()
{
	swprintf_s(m_cfgFile, sizeof m_cfgFile / sizeof(wchar_t), L"%ld_%ld.xml", KIND_ID, NODE_ID);

	return KIND_ID;
}