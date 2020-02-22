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

	//ׯ��Ӯ��
	if ( wWinner == m_playing_para.appointbanker )
	{

		for (int i = 0; i < m_room_config.max_chair_sum; i++)
		{
			if ( (USER_PLAYING == m_players_config.players[i].play_state) && (i != m_playing_para.appointbanker) )
			{
				SCORE PlayerGameScore = m_players_config.players[i].bet * lGameScore;				//����������ҵķ���  ������

				//�жϷⶥ
				if (PlayerGameScore > m_playing_para.max_room_bet)
					PlayerGameScore = m_playing_para.max_room_bet;

				m_players_config.players[i].single_score -= PlayerGameScore;
				m_players_config.players[m_playing_para.appointbanker].single_score += PlayerGameScore;
			}
		}		
	}
	else	//ׯ������
	{
		for (int i = 0; i < m_room_config.max_chair_sum; i++)
		{
			if ( (USER_PLAYING == m_players_config.players[i].play_state) && (i != m_playing_para.appointbanker) )
			{
				SCORE PlayerGameScore = m_players_config.players[i].bet * lGameScore;				//��������Ӯ�ҵķ���

				//�жϷⶥ
				if (PlayerGameScore > m_playing_para.max_room_bet)
					PlayerGameScore = m_playing_para.max_room_bet;

				m_players_config.players[i].single_score += PlayerGameScore;
				m_players_config.players[m_playing_para.appointbanker].single_score -= PlayerGameScore;
			}
		}
	}

	return 0;
}
