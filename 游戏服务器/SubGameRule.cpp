#include "StdAfx.h"
#include "SubGameRule.h"
#include "GameConfig.h"
#include <time.h>

//轮庄中指定庄家
bool CSubGameRule::AppointBankerTURN( WORD &wChairID )
{
	//默认为无效椅子
	wChairID = INVALID_CHAIR;

 	switch (m_room_config.kind_id)
    {
        case 2 :		//两张牌		TODO 两张牌的轮庄确定是这样？？？
        {
			if ( 1 == m_playing_para.game_cur_count )
			{
				//第一局庄家为房主
				wChairID = m_room_config.fangzhu_id;
			}
			else
			{
				//逆时针下一个位置上的人为庄家
				int nBanker = 0;				
				for (int i = 0; i < m_room_config.max_chair_sum; ++i)
				{
					//寻找下一个玩家
					nBanker = (m_playing_para.banker_id + i + 1) % m_room_config.max_chair_sum;

					//用户是否在游戏中
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
		case 3 : //钓蟹
		{
			if ( 1 == m_playing_para.game_cur_count )
			{
				//第一局庄家为房主
				wChairID = m_room_config.fangzhu_id;	
			}
			else
			{
				//上次赢的人为庄家
				wChairID = m_playing_para.last_winner_id;				
			}
			m_playing_para.banker_id = wChairID;
			return true;
		}
        default:
        {
            m_plog->LogSingleLine(L"轮庄 指定庄家发生错误", NULL);
            break;
        }
    } 
	return false;
}

//抢庄中指定庄家
bool CSubGameRule::AppointBankerFREE(WORD &wChairID)
{
	//默认为无效椅子
	wChairID = INVALID_CHAIR;

	//根据不同游戏处理抢庄
 	switch (m_room_config.kind_id)
    {
        case 2: //两张牌
        {
			//1、第一局固定庄家为房主
			//2、每玩两轮，重新投骰子决定庄家，谁的筛子点数高,谁是庄家；如果最高的有几个, 则在最高者之中随机			
            if(1 == m_playing_para.game_cur_count)
            {
				wChairID = m_room_config.fangzhu_id;
				m_playing_para.banker_id = wChairID;
            }
            else	 
            {
				//每玩两轮，重新决定庄家
				if ( 1 == (m_playing_para.game_cur_count % 2) )
				{
					 int nMaxScore = 0;
					int nRobNum = 0;
					int nRobPlayer[MAX_CHAIR_COUNT];	// 抢庄玩家数组

					//获取最大筛子者的数组
					for (int i = 0; i < GetCurPlayerCount(); ++i)
					{
						if(m_players_config.players[i].rob_score > nMaxScore)
						{
							nMaxScore = m_players_config.players[i].rob_score;
							//删除最大椅子号数组
							memset(nRobPlayer, 0, MAX_CHAIR_COUNT * sizeof(int));
							nRobPlayer[0] = i;
							nRobNum = 1;
						}
						else if(m_players_config.players[i].rob_score == nMaxScore)
						{
							nRobPlayer[nRobNum] = i; //保留最大筛子者的椅子号
							nRobNum ++;
						}
					}

					//从最大筛子者数组中选择庄家
					srand((unsigned)time(NULL));
					BYTE nBanker = rand() % ( nRobNum ); //nRobnum >= 1
					wChairID = nRobPlayer[nBanker];
					m_playing_para.banker_id = wChairID; //TODONOW 该语句可以放在AppointBanker中统一处理
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
            m_plog->LogSingleLine(L"轮庄 指定庄家发生错误", NULL);
            break;
        }
    } 
	return false;
}

// 固定庄处理
bool CSubGameRule::AppointBankerFIXED(WORD &wChairID)
{
	//默认为无效椅子
	wChairID = INVALID_CHAIR;

	//固定庄家为房主
	wChairID = m_room_config.fangzhu_id;
	m_playing_para.banker_id = wChairID;

	return true;
}

////根据不同的抢庄模式指定庄家
//bool __stdcall CSubGameRule::AppointBanker( WORD &wChairID )
//{
//	//校验抢庄模式
//	if ( m_room_config.game_robBank_mode < 0 )
//	{
//		return false;
//	}
//
//	//根据抢庄模式，指定庄家
//	switch ( m_room_config.game_robBank_mode )
//	{
//		case ROOMRULE_OPTION_ROBTYPE_FREE:	// 自由抢庄
//		{			
//			return AppointBankerFREE(wChairID);
//		}
//		case ROOMRULE_OPTION_ROBTYPE_TURN:	// 轮庄 
//		{
//			return AppointBankerTURN(wChairID);
//		}
//		case ROOMRULE_OPTION_ROBTYPE_FIXED: // 固定庄家
//		{
//			return AppointBankerFIXED(wChairID);
//		}
//		default:
//		{
//			m_plog->LogSingleLine(L"抢庄函数错误", NULL);
//			break;
//		}
//	}
//
//	return false;
//}

//分析卡牌类型
int __stdcall CSubGameRule::AnsysCardsType( )
{
	int result = 0;

	switch (m_room_config.kind_id)
	{
		default:
		{
			m_plog->LogSingleLine(L"分析函数中游戏类型错误", NULL);
			break;
		}
	}

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
			m_plog->LogSingleLine(L"结算得分游戏类型不支持", NULL);
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
