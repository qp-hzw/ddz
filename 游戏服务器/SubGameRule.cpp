#include "StdAfx.h"
#include "SubGameRule.h"
#include "GameConfig.h"
#include <time.h>

//������������
int __stdcall CSubGameRule::AnsysCardsType( )
{
	int result = 0;

	return result;
}

//����÷ֺ���Ӯ����
int __stdcall CSubGameRule::Settle()
{
	int result = 0;

	switch (m_room_config.kind_id)
	{
		case KIND_ID:	//����
		{
			SettleFightLandLord();
			break;
		}
		default:
		{
			//CLog::Log(log_error, "����÷���Ϸ���Ͳ�֧��");
			break;
		}
	}

	// ����ƾ���Ϣ
	OutGameInfor();

	return result;
}

//���������÷�
int CSubGameRule::SettleUpGrade()
{
	//��õ÷ַ��þֵ��ܵ÷�(ׯ�ҵ��¼Һ͸���ҵĶԼ�)
	WORD wScorerOne = GetNextUser(m_playing_para.banker_id);
	WORD wScorerTwo = GetLastUser(m_playing_para.banker_id);

	//��õ÷ַ����ܷ�
	SCORE SingleScore = m_players_config.players[wScorerOne].single_score;

	//����÷ַ��ĵ÷�
	m_players_config.players[wScorerOne].total_score += SingleScore;
	
	//���������ü���
	BYTE cbLevel = AnalyseUpGrade(SingleScore);	

	//TODO ��Ӯ������������費��Ҫ��¼������ò��û��Ҫ
	if (0 <= cbLevel)
	{
		//ʤ������1
		m_players_config.players[wScorerOne].winsum += 1;

		//�������
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

//���������
int CSubGameRule::SettleFightLandLord()
{
	//���Ӯ��
	WORD wWinner = m_playing_para.last_winner_id;

	//�����Ϸ���е÷�
	SCORE lGameScore = m_room_config.room_cell;

	//���ը������
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

	//ׯ��Ӯ��
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
	else	//ׯ������
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
