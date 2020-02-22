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

	//庄家赢了
	if ( wWinner == m_playing_para.appointbanker )
	{

		for (int i = 0; i < m_room_config.max_chair_sum; i++)
		{
			if ( (USER_PLAYING == m_players_config.players[i].play_state) && (i != m_playing_para.appointbanker) )
			{
				SCORE PlayerGameScore = m_players_config.players[i].bet * lGameScore;				//计算两个输家的分数  地主分

				//判断封顶
				if (PlayerGameScore > m_playing_para.max_room_bet)
					PlayerGameScore = m_playing_para.max_room_bet;

				m_players_config.players[i].single_score -= PlayerGameScore;
				m_players_config.players[m_playing_para.appointbanker].single_score += PlayerGameScore;
			}
		}		
	}
	else	//庄家输了
	{
		for (int i = 0; i < m_room_config.max_chair_sum; i++)
		{
			if ( (USER_PLAYING == m_players_config.players[i].play_state) && (i != m_playing_para.appointbanker) )
			{
				SCORE PlayerGameScore = m_players_config.players[i].bet * lGameScore;				//计算两个赢家的分数

				//判断封顶
				if (PlayerGameScore > m_playing_para.max_room_bet)
					PlayerGameScore = m_playing_para.max_room_bet;

				m_players_config.players[i].single_score += PlayerGameScore;
				m_players_config.players[m_playing_para.appointbanker].single_score -= PlayerGameScore;
			}
		}
	}

	return 0;
}
