#include "StdAfx.h"
#include "SubGameRule.h"
#include "GameConfig.h"
#include <time.h>

//��ׯ��ָ��ׯ��
bool CSubGameRule::AppointBankerTURN( WORD &wChairID )
{
	//Ĭ��Ϊ��Ч����
	wChairID = INVALID_CHAIR;

 	switch (m_room_config.kind_id)
    {
        case 2 :		//������		TODO �����Ƶ���ׯȷ��������������
        {
			if ( 1 == m_playing_para.game_cur_count )
			{
				//��һ��ׯ��Ϊ����
				wChairID = m_room_config.fangzhu_id;
			}
			else
			{
				//��ʱ����һ��λ���ϵ���Ϊׯ��
				int nBanker = 0;				
				for (int i = 0; i < m_room_config.max_chair_sum; ++i)
				{
					//Ѱ����һ�����
					nBanker = (m_playing_para.banker_id + i + 1) % m_room_config.max_chair_sum;

					//�û��Ƿ�����Ϸ��
					if ( USER_PLAYING == m_players_config.players[nBanker].play_state )
					{
						wChairID = nBanker;
						break;
					}
				}					
			}
			m_playing_para.banker_id = wChairID;
			return true;
        }
		case 3 : //��з
		{
			if ( 1 == m_playing_para.game_cur_count )
			{
				//��һ��ׯ��Ϊ����
				wChairID = m_room_config.fangzhu_id;	
			}
			else
			{
				//�ϴ�Ӯ����Ϊׯ��
				wChairID = m_playing_para.last_winner_id;				
			}
			m_playing_para.banker_id = wChairID;
			return true;
		}
        default:
        {
            m_plog->LogSingleLine(L"��ׯ ָ��ׯ�ҷ�������", NULL);
            break;
        }
    } 
	return false;
}

//��ׯ��ָ��ׯ��
bool CSubGameRule::AppointBankerFREE(WORD &wChairID)
{
	//Ĭ��Ϊ��Ч����
	wChairID = INVALID_CHAIR;

	//���ݲ�ͬ��Ϸ������ׯ
 	switch (m_room_config.kind_id)
    {
        case 2: //������
        {
			//1����һ�̶ֹ�ׯ��Ϊ����
			//2��ÿ�����֣�����Ͷ���Ӿ���ׯ�ң�˭��ɸ�ӵ�����,˭��ׯ�ң������ߵ��м���, ���������֮�����			
            if(1 == m_playing_para.game_cur_count)
            {
				wChairID = m_room_config.fangzhu_id;
				m_playing_para.banker_id = wChairID;
            }
            else	 
            {
				//ÿ�����֣����¾���ׯ��
				if ( 1 == (m_playing_para.game_cur_count % 2) )
				{
					 int nMaxScore = 0;
					int nRobNum = 0;
					int nRobPlayer[MAX_CHAIR_COUNT];	// ��ׯ�������

					//��ȡ���ɸ���ߵ�����
					for (int i = 0; i < GetCurPlayerCount(); ++i)
					{
						if(m_players_config.players[i].rob_score > nMaxScore)
						{
							nMaxScore = m_players_config.players[i].rob_score;
							//ɾ��������Ӻ�����
							memset(nRobPlayer, 0, MAX_CHAIR_COUNT * sizeof(int));
							nRobPlayer[0] = i;
							nRobNum = 1;
						}
						else if(m_players_config.players[i].rob_score == nMaxScore)
						{
							nRobPlayer[nRobNum] = i; //�������ɸ���ߵ����Ӻ�
							nRobNum ++;
						}
					}

					//�����ɸ����������ѡ��ׯ��
					srand((unsigned)time(NULL));
					BYTE nBanker = rand() % ( nRobNum ); //nRobnum >= 1
					wChairID = nRobPlayer[nBanker];
					m_playing_para.banker_id = wChairID; //TODONOW �������Է���AppointBanker��ͳһ����
				}
				else
				{
					wChairID = m_playing_para.banker_id;
					m_playing_para.banker_id = wChairID;
				}
               
            }
            return true;
		}
        default:
        {
            m_plog->LogSingleLine(L"��ׯ ָ��ׯ�ҷ�������", NULL);
            break;
        }
    } 
	return false;
}

// �̶�ׯ����
bool CSubGameRule::AppointBankerFIXED(WORD &wChairID)
{
	//Ĭ��Ϊ��Ч����
	wChairID = INVALID_CHAIR;

	//�̶�ׯ��Ϊ����
	wChairID = m_room_config.fangzhu_id;
	m_playing_para.banker_id = wChairID;

	return true;
}

////���ݲ�ͬ����ׯģʽָ��ׯ��
//bool __stdcall CSubGameRule::AppointBanker( WORD &wChairID )
//{
//	//У����ׯģʽ
//	if ( m_room_config.game_robBank_mode < 0 )
//	{
//		return false;
//	}
//
//	//������ׯģʽ��ָ��ׯ��
//	switch ( m_room_config.game_robBank_mode )
//	{
//		case ROOMRULE_OPTION_ROBTYPE_FREE:	// ������ׯ
//		{			
//			return AppointBankerFREE(wChairID);
//		}
//		case ROOMRULE_OPTION_ROBTYPE_TURN:	// ��ׯ 
//		{
//			return AppointBankerTURN(wChairID);
//		}
//		case ROOMRULE_OPTION_ROBTYPE_FIXED: // �̶�ׯ��
//		{
//			return AppointBankerFIXED(wChairID);
//		}
//		default:
//		{
//			m_plog->LogSingleLine(L"��ׯ��������", NULL);
//			break;
//		}
//	}
//
//	return false;
//}

//������������
int __stdcall CSubGameRule::AnsysCardsType( )
{
	int result = 0;

	switch (m_room_config.kind_id)
	{
		default:
		{
			m_plog->LogSingleLine(L"������������Ϸ���ʹ���", NULL);
			break;
		}
	}

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
			m_plog->LogSingleLine(L"����÷���Ϸ���Ͳ�֧��", NULL);
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
