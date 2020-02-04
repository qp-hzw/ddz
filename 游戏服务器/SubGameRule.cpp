#include "StdAfx.h"
#include "SubGameRule.h"
#include "GameConfig.h"
#include <time.h>

//分析卡牌类型
int __stdcall CSubGameRule::AnsysCardsType( )
{
	int result = 0;

	return result;
}

//结算得分和输赢局数
int __stdcall CSubGameRule::Settle()
{
	int result = 0;

	switch (m_room_config.kind_id)
	{
		case KIND_ID:	//升级
		{
			SettleFightLandLord();
			break;
		}
		default:
		{
			//CLog::Log(log_error, "结算得分游戏类型不支持");
			break;
		}
	}

	// 输出牌局信息
	OutGameInfor();

	return result;
}

//计算升级得分
int CSubGameRule::SettleUpGrade()
{
	//获得得分方该局的总得分(庄家的下家和该玩家的对家)
	WORD wScorerOne = GetNextUser(m_playing_para.banker_id);
	WORD wScorerTwo = GetLastUser(m_playing_para.banker_id);

	//获得得分方的总分
	SCORE SingleScore = m_players_config.players[wScorerOne].single_score;

	//计算得分方的得分
	m_players_config.players[wScorerOne].total_score += SingleScore;
	
	//计算升级得级数
	BYTE cbLevel = AnalyseUpGrade(SingleScore);	

	//TODO 输赢次数，看清空需不需要记录，升级貌似没必要
	if (0 <= cbLevel)
	{
		//胜场数加1
		m_players_config.players[wScorerOne].winsum += 1;

		//玩家升级
		m_players_config.players[wScorerOne].level += cbLevel;
	}

	if (INVALID_CHAIR != wScorerTwo)
	{
		m_players_config.players[wScorerTwo].single_score = m_players_config.players[wScorerOne].single_score;
		m_players_config.players[wScorerTwo].total_score = m_players_config.players[wScorerOne].total_score;
		m_players_config.players[wScorerTwo].winsum = m_players_config.players[wScorerOne].winsum;
		m_players_config.players[wScorerTwo].level = m_players_config.players[wScorerOne].level;
	}

	return 0;
}

//斗地主算分
int CSubGameRule::SettleFightLandLord()
{
	//获得赢家
	WORD wWinner = m_playing_para.last_winner_id;

	//获得游戏所有得分
	SCORE lGameScore = m_room_config.room_cell;

	//获得炸弹倍数
	for (int i = 0; i < m_room_config.max_chair_sum; i++)
	{
		if ( USER_PLAYING == m_players_config.players[i].play_state )
		{
			for (int j = 0; j < m_players_config.players[i].boom_info.cbBoomNum; j++)
			{
				BYTE bCardType = m_players_config.players[i].boom_info.cbBoomType[j];
				if ( (bCardType&(1<<CT_BOMB_CARD)) != 0 ||(bCardType&(1<<CT_MISSILE_CARD)) != 0||(bCardType&(1<<CT_LAIZI_BOMB))!= 0  )
				{
					lGameScore *= HARD_BOOM_TIMES;
				}
				else if((bCardType&(1<<CT_RUAN_BOMB))!= 0)
				{
					lGameScore *= RUAN_BOOM_TIMES;
				}
			}
		}
	}

	//庄家赢了
	if ( wWinner == m_playing_para.banker_id )
	{
		for (int i = 0; i < m_room_config.max_chair_sum; i++)
		{
			if ( (USER_PLAYING == m_players_config.players[i].play_state) && (i != m_playing_para.banker_id) )
			{
				m_players_config.players[i].single_score -= lGameScore;
				m_players_config.players[m_playing_para.banker_id].single_score += lGameScore;
			}
		}		
	}
	else	//庄家输了
	{
		for (int i = 0; i < m_room_config.max_chair_sum; i++)
		{
			if ( (USER_PLAYING == m_players_config.players[i].play_state) && (i != m_playing_para.banker_id) )
			{
				m_players_config.players[i].single_score += lGameScore;
				m_players_config.players[m_playing_para.banker_id].single_score -= lGameScore;
			}
		}
	}

	return 0;
}
