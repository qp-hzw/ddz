#include "StdAfx.h"
#include <vector>
#include <map>
#include <algorithm>
#include "GameData.h"
#include "SubGameRule.h"
#include "GameConfig.h"

//扑克数据
const BYTE	CGameData::m_cbCardData[FULL_COUNT]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
	0x4E,0x4F,
};

CGameData::CGameData()
{

	//CreateLog( L"SUB_GAMES_LOG.txt" );

	memset( &m_card_config, 0, sizeof m_card_config);
	memset( &m_players_config, 0, sizeof m_players_config);
}


/******************************************************************************/
/*******************************  游戏规则接口实现  *****************************/
/******************************************************************************/
// 创建游戏
int __stdcall CGameData::CreateGame(CGameLogic **pGameRule, CGameAccess **pGameAccess)
{

	//1. 此处创建的是GameRule对象 -- 重载了GameData的函数
	//2. GameLogic和GameAccess,CSubGameRule 指向该GameRule对象
	CSubGameRule *_game = new CSubGameRule();

	*pGameRule = _game;
	*pGameAccess = _game;

	return 0;
}

//加载读取的配置文件参数
int __stdcall CGameData::LoadConfigParameter(const wchar_t *_logFile, const BYTE cbGameScoreMode)
{
	int result = -1;

	// 创建游戏卡牌、玩家、规则
	result = LoadGameCards();		// 创建卡牌，给m_card_config赋值
	if (0 == result)
	{
		result = LoadPlayers();	// 创建玩家，给m_players_config赋值
		if (0 == result)
		{
			//读取桌子规则
			result = LoadGameRule(cbGameScoreMode);	//创建桌子规则，给m_room_config赋值

			// 初始化全局数据
			InitGameData();
		}
		else
		{
			//CLog::Log(log_error, "配置玩家异常\n");
		}
	}
	else
	{
		//CLog::Log(log_error, "配置卡牌异常\n");
	}

	return result;
}

// 创建游戏
int __stdcall CGameData::Init()
{
	//加载配置文件


	//初始化游戏数据 -- 必须放在读取配置文件之后
	InitGameData();

	return 0;
}

// 玩家总倍数
int __stdcall CGameData::Wager(WORD wChairID, float bet)
{
	//TODO 加倍倍数通过配置文件配置

	//封顶

	m_players_config.players[wChairID].bet *= bet;

	if (m_playing_para.max_room_bet < m_players_config.players[wChairID].bet)
	{
		m_players_config.players[wChairID].bet = m_playing_para.max_room_bet;
		return 0;
	}

	return 0;
}


// 设置玩家操作
int __stdcall CGameData::SetUserOperate(WORD wChairID, BYTE byAction, DWORD dwActionScore)
{
	//设置玩家动作
	m_players_config.players[wChairID].action = byAction;

	switch (byAction)
	{
	case 1:		//过：放弃押注，不参与比牌，直接输底分
	{
		m_players_config.players[wChairID].bet = 0;
		m_playing_para.cur_total_bet += m_players_config.players[wChairID].bet;
		printf("【玩家 = %d，动作 = %d，下注 = %d】", wChairID, byAction, m_players_config.players[wChairID].bet);
		break;
	}
	case 2:		//碰：押 1至开始下注底分
	{
		m_players_config.players[wChairID].bet = dwActionScore;
		m_playing_para.cur_total_bet += m_players_config.players[wChairID].bet;
		printf("【玩家 = %d，动作 = %d，下注 = %d】", wChairID, byAction, m_players_config.players[wChairID].bet);
		break;
	}
	case 3:		//吃：押开始下注底分
	{
		m_players_config.players[wChairID].bet = m_playing_para.start_bet;
		m_playing_para.cur_total_bet += m_players_config.players[wChairID].bet;
		printf("【玩家 = %d，动作 = %d，下注 = %d】", wChairID, byAction, m_players_config.players[wChairID].bet);
		break;
	}
	case 4:		//带：底分 + 所有玩家押注综合
	{
		m_players_config.players[wChairID].bet = m_playing_para.cur_total_bet;
		m_playing_para.cur_total_bet += m_players_config.players[wChairID].bet;
		printf("【玩家 = %d，动作 = %d，下注 = %d】", wChairID, byAction, m_players_config.players[wChairID].bet);
		break;
	}
	}

	return 0;
}


// 指定庄家
bool __stdcall CGameData::AppointBanker(WORD wChairID)
{
	m_playing_para.appointbanker = wChairID;
	return true;
}

//获得当前指定的庄家
WORD __stdcall CGameData::GetAppointBanker()
{
	return m_playing_para.appointbanker;
}

/**
* 客户端返回是否做庄
* @param		wChairID		[in-out]		椅子编号
* @param		type			[in]		抢庄类型		//	0-抢地主	1-叫地主
* @param		state			[in]		抢庄状态		//  0-过		1-叫/抢地主
*/
int __stdcall CGameData::SetBankerState(WORD wChairID, BYTE cbResult)
{
	m_players_config.players[wChairID].bank_state = cbResult;   // //0-不叫  1-叫地主  2-不抢  3-抢地主

	return 0;
}

// 获取用户叫庄状态
BYTE __stdcall CGameData::GetBankerState(WORD wChairID)
{
	return m_players_config.players[wChairID].bank_state;
}

// 分析用户牌型
int __stdcall CGameData::AnsysCardsType()
{
	return 0;
}

/*************************************************
@Description:	洗牌并发牌，乱序牌组并给玩家牌组赋值
@Input:			wChairID			[in-out]		椅子编号
				force			[in-out]		是否指定成功
@Output:
@Return:		0
@author & data:	lizhihu 2017.11.2
*************************************************/
int __stdcall CGameData::Deal()
{
	// 用于洗牌发牌的零时存储区
	CARD_DESC *cards = new CARD_DESC[m_card_config.game_cards_sum]();

	CARD_DESC *boomCards = new CARD_DESC[m_card_config.game_cards_sum]();
	BYTE	 boomSum = 0;

	//获得的牌数组是一副牌的乱序数组
	CGameCardConfig::OutOrder(cards, m_card_config.game_cards_sum,
		m_card_config.game_cards, m_card_config.game_cards_sum,
		m_room_config.game_score_mode, boomCards, boomSum); // 洗牌

	///*
	BYTE cardssum = m_card_config.game_cards_sum;
	//测试固定牌
	BYTE tmpcards[54] = {
		0x4E, 0x4F, 0x01, 0x11, 0x21, 0x31, 0x02, 0x12, 0x22, 0x32, 0x03, 0x13, 0x23, 0x33, 0x04, 0x14, 0x24, 0x34, 0x05, 0x15, 0x25, 0x35,
		0x06, 0x16, 0x26, 0x36, 0x07, 0x17, 0x27, 0x37, 0x08, 0x18, 0x28, 0x38, 0x09, 0x19, 0x29, 0x39, 0x0A, 0x1A, 0x2A, 0x3A, 0x0B, 0x1B, 0x2B, 0x3B,
		0x0C, 0x1C, 0x2C, 0x3C, 0x0D, 0x1D, 0x2D, 0x3D,
	};
	memcpy(cards, tmpcards, cardssum * sizeof(BYTE));
	//*/

	//获得扣底卡牌
	m_playing_para.leave_card[MAX_LEAVE_CARD_NUM];

	//经典场分发手牌
	if (0 == m_room_config.game_score_mode)
	{
		//TODONOW 看下是否有问题
		ZeroMemory(m_playing_para.leave_card, sizeof(m_playing_para.leave_card));
		CopyMemory(m_playing_para.leave_card, &cards[m_card_config.game_cards_sum - m_card_config.leave_card_num],
			m_card_config.leave_card_num * sizeof(BYTE));

		SortCardList(m_playing_para.leave_card, m_card_config.leave_card_num, ST_ORDER);

		//将一副牌分发给所有玩家
		const CARD_DESC *card_index = cards;

		//分发牌
		for (int i = 0; i < m_room_config.max_chair_sum; ++i)
		{
			int nCardNum = m_card_config.init_card_num;
			//判断是否在游戏中
			if (USER_PLAYING == m_players_config.players[i].play_state)
			{
				//玩家当前手牌数
				m_players_config.players[i].hand_card_num = nCardNum;
				cout << "游戏手牌已初始化" << endl;
				//获得乱序手牌
				memcpy(m_players_config.hand_cards[i], card_index, sizeof(CARD_DESC)*nCardNum);
				cout << "1" << endl;

				//手牌排序
				SortCardList(m_players_config.hand_cards[i], nCardNum, ST_ORDER);
				cout << "2" << endl;

				for (int j = 0; j < nCardNum; j++)
				{
					cout << (int)m_players_config.hand_cards[i][j] << " ";
				}
				cout << endl;

				card_index += nCardNum;
			}
		}
	}
	else	//不洗牌模式
	{
		srand((unsigned)time(NULL));

		BYTE card_sum = m_card_config.game_cards_sum - boomSum * 4;
		CARD_DESC *card_index = cards;

		ZeroMemory(m_playing_para.leave_card, sizeof(m_playing_para.leave_card));
		CopyMemory(m_playing_para.leave_card, &cards[card_sum - m_card_config.leave_card_num],
			m_card_config.leave_card_num * sizeof(BYTE));

		SortCardList(m_playing_para.leave_card, m_card_config.leave_card_num, ST_ORDER);

		int cardflag = 0;
		int boomflag = 0;
		int index[3] = { 0 };
		index[0] = rand() % boomSum;
		if (index[0] > 3) index[0] = 3;
		index[1] = rand() % (boomSum - index[0]);
		if (index[1] > 3) index[1] = 3;
		index[2] = boomSum - index[1] - index[0];

		//机器人炸弹优化
		/*for (int i = 0; i < 3; i++)
		{
			if (m_pGameRoomFrame->IsRobot(i))
			{
				if (0 < index[(i + 1) % 3])
				{
					index[i]++;
					index[(i + 1) % 3]--;
				}
			}
		}*/

		for (int i = 0; i < m_room_config.max_chair_sum; ++i)
		{
			int nCardNum = m_card_config.init_card_num;
			//判断是否在游戏中
			if (USER_PLAYING == m_players_config.players[i].play_state)
			{
				//玩家当前手牌数
				m_players_config.players[i].hand_card_num = nCardNum;

				int flag = 0;
				for (int j = 0; j < nCardNum - index[i] * 4; j++)
				{
					m_players_config.hand_cards[i][flag++] = card_index[cardflag++];
				}

				for (int j = 0; j < index[i] * 4; j++)
				{
					m_players_config.hand_cards[i][flag++] = boomCards[boomflag++];
				}

				//手牌排序
				SortCardList(m_players_config.hand_cards[i], nCardNum, ST_ORDER);

			}
		}
	}

	//释放
	delete[] cards;
	delete[] boomCards;
	cards = NULL;

	return 0;
}

//计算抄底得分
SCORE __stdcall CGameData::CalculateChaodiScore(const BYTE &cbCDcardNum)
{
	//获得底牌的所有分数
	WORD wLeaveCardScore = GetAllCardScore(m_playing_para.leave_card, MAX_LEAVE_CARD_NUM);

	//抄底分数和卡牌数有关
	SCORE TotalScore = ((cbCDcardNum + 1) / 2)*wLeaveCardScore;

	//获得得分方
	WORD wScorer = GetNextUser(m_playing_para.appointbanker);

	//小局得分增加
	m_players_config.players[wScorer].single_score += TotalScore;

	return TotalScore;
}

//根据得分计算玩家升级级数
BYTE __stdcall CGameData::AnalyseUpGrade(const SCORE &score)
{
	BYTE cbLevel = 0;
	//大光
	if (SCORE_DA_GUANG == score)
	{
		cbLevel = LEVEL_DA_GUANG;
		//庄家的对家坐庄
		m_playing_para.appointbanker = GetOppositeUser(m_playing_para.appointbanker);
	}
	//小光
	if (SCORE_XIAO_GUANG > score)
	{
		cbLevel = LEVEL_XIAO_GUANG;
		//庄家的对家坐庄
		m_playing_para.appointbanker = GetOppositeUser(m_playing_para.appointbanker);
	}
	//40-80
	if ((SCORE_XIAO_GUANG <= score) && (SCORE_CHANGE_BANKER > score))
	{
		cbLevel = LEVEL_CHANGE_BANKER;
		//庄家的对家坐庄
		m_playing_para.appointbanker = GetOppositeUser(m_playing_para.appointbanker);
	}
	//80-120
	if ((SCORE_CHANGE_BANKER <= score) && (SCORE_UPGRADE > score))
	{
		//换庄,庄家的下一家坐庄
		m_playing_para.appointbanker = GetNextUser(m_playing_para.appointbanker);
	}
	//>=120
	if (SCORE_UPGRADE <= score)
	{
		cbLevel = (score - SCORE_UPGRADE) / SCORE_UPGRADE_ONE;

		//换庄,庄家的下一家坐庄
		m_playing_para.appointbanker = GetNextUser(m_playing_para.appointbanker);
	}

	//升多少级
	m_playing_para.level_value += cbLevel;

	printf("\n【下局游戏 ：等级 = %d，庄家 = %d】\n", m_playing_para.level_value, m_playing_para.appointbanker);

	return cbLevel;
}

//分析比较     
bool CGameData::AnalysebCompare(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult, int dCardType)
{
	//分析扑克
	BYTE bLaiZiCount = GetMagicCardNum(cbCardData, cbCardCount);
	BYTE bCardDataTemp[MAX_CARD_COUNT];
	int dCardTypeTemp = CT_ERROR;
	tagAnalyseResult  AnalyseResultTemp;
	dCardType = 1 << dCardType;

	memset(bCardDataTemp, 0, sizeof(bCardDataTemp));
	CopyMemory(bCardDataTemp, cbCardData, sizeof(BYTE)*cbCardCount);
	ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));
	ZeroMemory(&AnalyseResultTemp, sizeof(AnalyseResultTemp));

	bool SameCard = false;

	//都是赖子做普通牌				//普通场不考虑癞子
	if (m_room_config.game_laizi_mode > 0)
	{

		for (BYTE i = 0; i < cbCardCount; i++)
		{
			if (GetCardLogicValueLaiZi(cbCardData[i]) == m_playing_para.magic_card)
				SameCard = true;
			else
			{
				SameCard = false;
				break;
			}
		}
	}

	//赖子判断
	if (bLaiZiCount != 0 && !SameCard && m_room_config.game_laizi_mode > 0)
	{
		for (BYTE i = 0;i<cbCardCount;i++)
		{
			if (GetCardLogicValueLaiZi(cbCardData[i]) == m_playing_para.magic_card)
			{
				//一张赖子
				for (BYTE j = 0;j<13;j++)
				{
					bCardDataTemp[i] = m_cbCardData[j];

					//两张赖子
					if (bLaiZiCount >= 2)
					{
						for (BYTE k = i + 1;k<cbCardCount;k++)
						{
							if (GetCardLogicValueLaiZi(cbCardData[k]) == m_playing_para.magic_card)
							{
								for (BYTE z = 0;z<13;z++)
								{
									bCardDataTemp[k] = m_cbCardData[z];

									//三张赖子
									if (bLaiZiCount >= 3)
									{
										for (BYTE g = k + 1;g<cbCardCount;g++)
										{
											if (GetCardLogicValueLaiZi(cbCardData[g]) == m_playing_para.magic_card)
											{
												for (BYTE f = 0;f<13;f++)
												{
													bCardDataTemp[g] = m_cbCardData[f];

													//四张赖子
													if (bLaiZiCount == 4)
													{
														for (BYTE h = g + 1;h<cbCardCount;h++)
														{
															if (GetCardLogicValueLaiZi(cbCardData[h]) == m_playing_para.magic_card)
															{
																for (BYTE l = 0;l<13;l++)
																{
																	bCardDataTemp[h] = m_cbCardData[l];

																	ZeroMemory(&AnalyseResultTemp, sizeof(AnalyseResultTemp));

																	AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResultTemp);
																	if (GetType(AnalyseResultTemp, bCardDataTemp, cbCardCount, cbCardData) == 0)
																		continue;
																	if ((dCardType&(GetType(AnalyseResultTemp, bCardDataTemp, cbCardCount, cbCardData))) != 0)
																		AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResult);
																}
															}
															else
															{
																ZeroMemory(&AnalyseResultTemp, sizeof(AnalyseResultTemp));

																AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResultTemp);
																if (GetType(AnalyseResultTemp, bCardDataTemp, cbCardCount, cbCardData) == 0)
																	continue;

																if ((dCardType&(1 << GetType(AnalyseResultTemp, bCardDataTemp, cbCardCount, cbCardData))) != 0)
																	AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResult);
															}
														}
													}
													ZeroMemory(&AnalyseResultTemp, sizeof(AnalyseResultTemp));

													AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResultTemp);
													if (GetType(AnalyseResultTemp, bCardDataTemp, cbCardCount, cbCardData) == 0)
														continue;

													if ((dCardType&(1 << GetType(AnalyseResultTemp, bCardDataTemp, cbCardCount, cbCardData))) != 0)
														AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResult);
												}
											}
											else
											{
												ZeroMemory(&AnalyseResultTemp, sizeof(AnalyseResultTemp));

												AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResultTemp);
												if (GetType(AnalyseResultTemp, bCardDataTemp, cbCardCount, cbCardData) == 0)
													continue;

												if ((dCardType&(1 << GetType(AnalyseResultTemp, bCardDataTemp, cbCardCount, cbCardData))) != 0)
													AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResult);
											}
										}
									}
									ZeroMemory(&AnalyseResultTemp, sizeof(AnalyseResultTemp));

									AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResultTemp);
									if (GetType(AnalyseResultTemp, bCardDataTemp, cbCardCount, cbCardData) == 0)
										continue;

									if ((dCardType&(1 << GetType(AnalyseResultTemp, bCardDataTemp, cbCardCount, cbCardData))) != 0)
										AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResult);
								}
							}
							else
							{
								ZeroMemory(&AnalyseResultTemp, sizeof(AnalyseResultTemp));

								AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResultTemp);
								if (GetType(AnalyseResultTemp, bCardDataTemp, cbCardCount, cbCardData) == 0)
									continue;

								if ((dCardType&(1 << GetType(AnalyseResultTemp, bCardDataTemp, cbCardCount, cbCardData))) != 0)
									AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResult);
							}
						}
					}
					ZeroMemory(&AnalyseResultTemp, sizeof(AnalyseResultTemp));

					AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResultTemp);
					if (GetType(AnalyseResultTemp, bCardDataTemp, cbCardCount, cbCardData) == 0)
						continue;

					if ((dCardType&(1 << GetType(AnalyseResultTemp, bCardDataTemp, cbCardCount, cbCardData))) != 0)
						AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResult);
				}
			}
			else
			{
				ZeroMemory(&AnalyseResultTemp, sizeof(AnalyseResultTemp));

				AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResultTemp);
				if (GetType(AnalyseResultTemp, bCardDataTemp, cbCardCount, cbCardData) == 0)
					continue;

				if ((dCardType&(1 << GetType(AnalyseResultTemp, bCardDataTemp, cbCardCount, cbCardData))) != 0)
					AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResult);
			}
		}
	}
	else
	{
		ZeroMemory(&AnalyseResultTemp, sizeof(AnalyseResultTemp));

		AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResultTemp);

		if ((dCardType&(1 << GetType(AnalyseResultTemp, cbCardData, cbCardCount, cbCardData))) != 0)
			AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResult);
	}

	return true;
}

//分析扑克
void CGameData::AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult)
{
	//设置变量
	BYTE bCardData[MAX_CARD_COUNT];

	memset(bCardData, 0, sizeof(bCardData));
	CopyMemory(bCardData, cbCardData, sizeof(BYTE)*cbCardCount);

	SortCardListNoLaiZi(bCardData, cbCardCount, ST_ORDER);

	//扑克分析
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		//变量定义
		BYTE cbSameCount = 1, cbCardValueTemp = 0;
		BYTE cbLogicValue = GetCardLogicValue(bCardData[i]);

		//搜索同牌
		for (BYTE j = i + 1; j < cbCardCount; j++)
		{
			//获取扑克
			if (GetCardLogicValue(bCardData[j]) != cbLogicValue) break;

			//设置变量
			cbSameCount++;
		}

		//设置结果
		switch (cbSameCount)
		{
		case 1:		//单张
		{
			BYTE cbIndex = AnalyseResult.cbSignedCount++;
			AnalyseResult.cbSignedCardData[cbIndex*cbSameCount] = bCardData[i];
			break;
		}
		case 2:		//两张
		{
			BYTE cbIndex = AnalyseResult.cbDoubleCount++;
			AnalyseResult.cbDoubleCardData[cbIndex*cbSameCount] = bCardData[i];
			AnalyseResult.cbDoubleCardData[cbIndex*cbSameCount + 1] = bCardData[i + 1];
			break;
		}
		case 3:		//三张
		{
			BYTE cbIndex = AnalyseResult.cbThreeCount++;
			AnalyseResult.cbThreeCardData[cbIndex*cbSameCount] = bCardData[i];
			AnalyseResult.cbThreeCardData[cbIndex*cbSameCount + 1] = bCardData[i + 1];
			AnalyseResult.cbThreeCardData[cbIndex*cbSameCount + 2] = bCardData[i + 2];
			break;
		}
		case 4:		//四张
		{
			BYTE cbIndex = AnalyseResult.cbFourCount++;
			AnalyseResult.cbFourCardData[cbIndex*cbSameCount] = bCardData[i];
			AnalyseResult.cbFourCardData[cbIndex*cbSameCount + 1] = bCardData[i + 1];
			AnalyseResult.cbFourCardData[cbIndex*cbSameCount + 2] = bCardData[i + 2];
			AnalyseResult.cbFourCardData[cbIndex*cbSameCount + 3] = bCardData[i + 3];
			break;
		}
		}

		//设置索引
		i += cbSameCount - 1;
	}


	return;
}

//获取类型结果
int CGameData::GetType(tagAnalyseResult AnalyseResult, const BYTE cbCardData[], BYTE cbCardCount, const BYTE NoChangeCard[])
{
	BYTE bLaiZiCount = GetMagicCardNum(NoChangeCard, cbCardCount);

	BYTE bCardData[MAX_CARD_COUNT];
	ZeroMemory(&bCardData, sizeof(bCardData));
	CopyMemory(bCardData, cbCardData, sizeof(BYTE)*cbCardCount);

	//排序
	SortCardListNoLaiZi(bCardData, cbCardCount, ST_ORDER);

	//简单牌型
	switch (cbCardCount)
	{
	case 0:	//空牌
	{
		return CT_ERROR;
	}
	case 1: //单牌
	{
		return CT_SINGLE;
	}
	case 2:	//对牌火箭
	{
		//牌型判断
		if ((bCardData[0] == 0x4F) && (bCardData[1] == 0x4E)) return CT_MISSILE_CARD;
		if (GetCardLogicValue(bCardData[0]) == GetCardLogicValue(bCardData[1])) return CT_DOUBLE;

		return CT_ERROR;
	}
	}

	//四牌判断
	if (AnalyseResult.cbFourCount > 0)
	{
		//牌型判断
		if ((AnalyseResult.cbFourCount == 1) && (cbCardCount == 4) && (bLaiZiCount == 0)) return CT_BOMB_CARD;
		if ((AnalyseResult.cbFourCount == 1) && (bLaiZiCount == 4)) return CT_LAIZI_BOMB;
		if ((AnalyseResult.cbFourCount == 1) && (cbCardCount == 4) && (bLaiZiCount != 0)) return CT_RUAN_BOMB;
		if ((AnalyseResult.cbFourCount == 1) && ((AnalyseResult.cbSignedCount == 2) || (AnalyseResult.cbDoubleCount == 1)) && (cbCardCount == 6)) return CT_FOUR_LINE_TAKE_ONE;
		if ((AnalyseResult.cbFourCount == 1) && (AnalyseResult.cbDoubleCount == 2) && (cbCardCount == 8)) return CT_FOUR_LINE_TAKE_TWO;


		return CT_ERROR;
	}

	//三牌判断
	if (AnalyseResult.cbThreeCount > 0)
	{
		//三条类型
		if (AnalyseResult.cbThreeCount == 1 && cbCardCount == 3)
			return CT_THREE;

		//连牌判断
		if (AnalyseResult.cbThreeCount > 1)
		{
			//变量定义
			BYTE bCardData = AnalyseResult.cbThreeCardData[0];
			BYTE cbFirstLogicValue = GetCardLogicValue(bCardData);

			//错误过虑
			if (cbFirstLogicValue >= 15) return CT_ERROR;

			//连牌判断
			for (BYTE i = 1; i < AnalyseResult.cbThreeCount; i++)
			{
				BYTE bCardData = AnalyseResult.cbThreeCardData[i * 3];
				if (cbFirstLogicValue != (GetCardLogicValue(bCardData) + i))
					return CT_ERROR;
			}
		}

		//牌形判断
		if (AnalyseResult.cbThreeCount * 3 == cbCardCount) return CT_THREE_LINE;
		if (AnalyseResult.cbThreeCount * 4 == cbCardCount && (AnalyseResult.cbSignedCount == cbCardCount % 3 || AnalyseResult.cbDoubleCount == cbCardCount % 3 / 2)) return CT_THREE_LINE_TAKE_ONE;
		if ((AnalyseResult.cbThreeCount * 5 == cbCardCount) && (AnalyseResult.cbDoubleCount == AnalyseResult.cbThreeCount)) return CT_THREE_LINE_TAKE_TWO;

		return CT_ERROR;
	}

	//两张类型
	if (AnalyseResult.cbDoubleCount >= 3)
	{
		//变量定义
		BYTE bCardData = AnalyseResult.cbDoubleCardData[0];
		BYTE cbFirstLogicValue = GetCardLogicValue(bCardData);

		/*cout << "连对第一张牌逻辑值：" << (int)cbFirstLogicValue << endl;
		cout << "最后一张拍逻辑值: " << (int)GetCardLogicValue(AnalyseResult.cbDoubleCardData[AnalyseResult.cbDoubleCount*2 - 1]) << endl;*/

		//错误过虑
		if (cbFirstLogicValue >= 15) return CT_ERROR;

		//连牌判断
		for (BYTE i = 1; i < AnalyseResult.cbDoubleCount; i++)
		{
			BYTE bCardData = AnalyseResult.cbDoubleCardData[i * 2];
			if (cbFirstLogicValue != (GetCardLogicValue(bCardData) + i))
				return CT_ERROR;
		}

		//二连判断
		if ((AnalyseResult.cbDoubleCount * 2) == cbCardCount)
			return CT_DOUBLE_LINE;

		return CT_ERROR;
	}

	//单张判断
	if ((AnalyseResult.cbSignedCount >= 5) && (AnalyseResult.cbSignedCount == cbCardCount))
	{
		//变量定义
		BYTE bCardData = AnalyseResult.cbSignedCardData[0];
		BYTE cbFirstLogicValue = GetCardLogicValue(bCardData);

		//错误过虑
		if (cbFirstLogicValue >= 15) return CT_ERROR;

		//连牌判断
		for (BYTE i = 1; i < AnalyseResult.cbSignedCount; i++)
		{
			BYTE bCardData = AnalyseResult.cbSignedCardData[i];
			if (cbFirstLogicValue != (GetCardLogicValue(bCardData) + i))
				return CT_ERROR;
		}

		return CT_SINGLE_LINE;
	}

	return CT_ERROR;
}

//判断当前玩家是否能出牌 
bool CGameData::JudgePlayerOutCard(WORD wCurOutCardUser)      //lih
{
	//获得该轮最大出牌数据
	BYTE TurnCardNum = m_playing_para.turn_cards_num;
	BYTE TurnCardData[MAX_CARD_COUNT];
	ZeroMemory(&TurnCardData, sizeof(TurnCardData));
	CopyMemory(TurnCardData, m_playing_para.turn_max_cards, sizeof(TurnCardData));

	//获取当前玩家的手牌数据
	BYTE CurCardNum = m_players_config.players[wCurOutCardUser].hand_card_num;
	BYTE CurCardData[MAX_CARD_COUNT];
	ZeroMemory(&CurCardData, sizeof(CurCardData));

	for (int i = 0; i < CurCardNum; i++)
	{
		CurCardData[i] = m_players_config.hand_cards[wCurOutCardUser][i];
	}

	//分析两个扑克牌型
	tagAnalyseResult TurnResult;
	tagAnalyseResult CurResult;

	ZeroMemory(&TurnResult, sizeof(TurnResult));
	ZeroMemory(&CurResult, sizeof(CurResult));

	AnalysebCardData(CurCardData, CurCardNum, CurResult);
	AnalysebCardData(TurnCardData, TurnCardNum, TurnResult);

	//获取出牌玩家的牌型
	int TurnCardType = GetType(TurnResult, TurnCardData, TurnCardNum, TurnCardData);
	TurnCardType = TransListToInt(0, TurnCardType);

	//对比判断牌型
	if (TurnCardType == CT_ERROR)	return true;
	if ((TurnCardType & (1 << CT_MISSILE_CARD)) != 0)    return false;    //王炸判断

																		  //炸弹判断
	if ((TurnCardType & (1 << CT_BOMB_CARD)) != 0)   //上家出炸弹
	{
		cout << "炸弹" << endl;

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//获取上家出牌数值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbFourCardData[0]);
		int index = 0;   //辅助标识

		if (CurResult.cbFourCount == 0)    return false;

		for (int i = 0; i < CurResult.cbFourCount; i++)
		{
			if (GetCardLogicValue(CurResult.cbFourCardData[index]) > TurnLogicValue)		return true;
			index += 4;
		}
	}
	else if ((TurnCardType & (1 << CT_SINGLE)) != 0)   //单张
	{
		cout << "单张" << endl;

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//炸弹判断
		if (CurResult.cbFourCount != 0)		return true;

		//获取上家出牌数值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbSignedCardData[0]);

		cout << "获取上家出牌数值:" << (int)TurnLogicValue << endl;

		if (GetCardLogicValue(CurCardData[0]) > TurnLogicValue)
			return true;
	}
	else if ((TurnCardType & (1 << CT_DOUBLE)) != 0)   //对子
	{
		cout << "对子" << endl;

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//炸弹判断
		if (CurResult.cbFourCount != 0)		return true;

		//获取上家出牌数值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbDoubleCardData[0]);

		for (int i = 0; i < CurResult.cbDoubleCount; i++)
		{
			int index = 0;
			if (GetCardLogicValue(CurResult.cbDoubleCardData[index]) > TurnLogicValue)
				return true;

			index += 2;
		}
		for (int i = 0; i < CurResult.cbThreeCount; i++)
		{
			int index = 0;
			if (GetCardLogicValue(CurResult.cbThreeCardData[index]) > TurnLogicValue)
				return true;

			index += 3;
		}
	}
	else if ((TurnCardType & (1 << CT_THREE)) != 0)   //三张
	{
		cout << "三张" << endl;

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//炸弹判断
		if (CurResult.cbFourCount != 0)		return true;

		//获取上家出牌数值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);

		for (int i = 0; i < CurResult.cbThreeCount; i++)
		{
			int index = 0;
			if (GetCardLogicValue(CurResult.cbThreeCardData[index]) > TurnLogicValue)
				return true;

			index += 3;
		}
	}
	else if ((TurnCardType & (1 << CT_SINGLE_LINE)) != 0)   //顺子
	{
		cout << "顺子" << endl;

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//炸弹判断
		if (CurResult.cbFourCount != 0)		return true;

		//牌不足五张
		if (CurCardNum < 5)		return false;

		//获取上家出牌数值  最大值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbSignedCardData[0]);
		cout << "顺子:获取上家出牌数值:" << (int)TurnLogicValue << endl;

		int flag = 0;

		for (int i = 0; i < CurCardNum; i++)     //计算顺子张数
		{
			//printf("第%d张牌的连续标志：%d\n", i, flag);
			if (flag != 0)    //连续不用比牌大小
			{
				if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
				{
					continue;
				}
				if (GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //判断是否连续
				{
					flag++;

					//跳出条件
					if (flag == TurnResult.cbSignedCount - 1)
						break;
				}
				else
					flag = 0;
			}
			else
			{
				if (GetCardLogicValue(CurCardData[i]) > TurnLogicValue)
				{
					if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
					{
						continue;
					}
					if (GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //判断是否连续
					{
						flag++;

						//跳出条件
						if (flag == TurnResult.cbSignedCount - 1)
							break;
					}
					else
						flag = 0;
				}
			}
		}

		if (flag >= TurnResult.cbSignedCount - 1)
			return true;
	}
	else if ((TurnCardType & (1 << CT_DOUBLE_LINE)) != 0)   //连对
	{
		cout << "连对" << endl;

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//炸弹判断
		if (CurResult.cbFourCount != 0)		return true;

		//牌不足六张
		if (CurCardNum < 6)		return false;

		//获取上家出牌数值  最大值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbDoubleCardData[0]);
		int flag = 0;   //判断是否连续标志
		int Numflag = 0;   //判断牌数是否达标的标志

		for (int i = 0; i < CurCardNum; i++)     //计算连对张数
		{
			if (flag != 0)    //连续不用比牌大小
			{
				if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
				{
					Numflag++;
					continue;
				}
				if ((GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1))
					&& (Numflag > 0) && (GetCardLogicValue(CurCardData[i]) < 15))   //判断是否连续
				{
					flag++;

					//跳出条件
					if (flag == TurnResult.cbDoubleCount)
						break;
				}
				else
					flag = 0;

				if (GetCardLogicValue(CurCardData[i + 1]) != GetCardLogicValue(CurCardData[i]))
				{
					Numflag = 0;
				}
			}
			else
			{
				if (GetCardLogicValue(CurCardData[i]) > TurnLogicValue)
				{
					if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
					{
						Numflag++;
						continue;
					}
					if (GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1)
						&& (Numflag > 0) && (GetCardLogicValue(CurCardData[i]) < 15))   //判断是否连续
					{
						flag++;

						//跳出条件
						if (flag == TurnResult.cbDoubleCount)
							break;
					}
					else
						flag = 0;

					if (GetCardLogicValue(CurCardData[i + 1]) != GetCardLogicValue(CurCardData[i]))
					{
						Numflag = 0;
					}
				}
			}
		}

		if (flag >= TurnResult.cbDoubleCount)
			return true;
	}
	else if ((TurnCardType & (1 << CT_THREE_LINE)) != 0)   //三连
	{
		cout << "三连" << endl;

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//炸弹判断
		if (CurResult.cbFourCount != 0)		return true;

		//牌不足六张
		if (CurCardNum < 6)		return false;

		//获取上家出牌数值  最大值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);
		int flag = 0;   //判断是否连续标志
		int Numflag = 0;   //判断牌数是否达标的标志

		for (int i = 0; i < CurCardNum; i++)     //计算连对张数
		{
			if (flag != 0)    //连续不用比牌大小
			{
				if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
				{
					Numflag++;
					continue;
				}
				if ((GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1))
					&& (Numflag > 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //判断是否连续
				{
					flag++;

					//跳出条件
					if (flag == TurnResult.cbThreeCount)
						break;
				}
				else
					flag = 0;

				if (GetCardLogicValue(CurCardData[i + 1]) != GetCardLogicValue(CurCardData[i]))
				{
					Numflag = 0;
				}
			}
			else
			{
				if (GetCardLogicValue(CurCardData[i]) > TurnLogicValue)
				{
					if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
					{
						Numflag++;
						continue;
					}
					if (GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1)
						&& (Numflag > 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //判断是否连续
					{
						flag++;

						//跳出条件
						if (flag == TurnResult.cbThreeCount)
							break;
					}
					else
						flag = 0;

					if (GetCardLogicValue(CurCardData[i + 1]) != GetCardLogicValue(CurCardData[i]))
					{
						Numflag = 0;
					}
				}
			}
		}

		if (flag >= TurnResult.cbThreeCount)
			return true;
	}
	else if ((TurnCardType & (1 << CT_THREE_LINE_TAKE_ONE)) != 0)   //三代一
	{

		cout << "三代一" << endl;
		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//炸弹判断
		if (CurResult.cbFourCount != 0)		return true;

		//牌不够四张
		if (CurCardNum < 4 * TurnResult.cbThreeCount)		return false;

		//获取上家出牌数值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);

		if (TurnResult.cbThreeCount == 1)    //三代一
		{
			int index = 0;
			for (int i = 0; i < CurResult.cbThreeCount; i++)
			{
				if (GetCardLogicValue(CurResult.cbThreeCardData[index]) > TurnLogicValue)
					return true;

				index += 3;
			}
		}
		else		//飞机
		{
			int index = 0;
			int flag = 1;   //判断飞机几个三张的标志
							//判断三个是否连续
			for (int i = 0; i < CurResult.cbThreeCount; i++)
			{
				if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) - 1) == (GetCardLogicValue(CurResult.cbThreeCardData[index + 3])))  //连续
				{
					flag++;
				}

				if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) >(TurnLogicValue - TurnResult.cbThreeCount + 2))
					&& (flag == TurnResult.cbThreeCount))   //大小比较
					return true;

				index += 3;
			}
		}
	}
	else if ((TurnCardType & (1 << CT_THREE_LINE_TAKE_TWO)) != 0)   //三代二
	{
		cout << "三代二" << endl;

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//炸弹判断
		if (CurResult.cbFourCount != 0)		return true;

		//牌不够五张
		if (CurCardNum < 5 * TurnResult.cbThreeCount)		return false;

		//没有对子
		if (TurnResult.cbThreeCount < 2 && TurnResult.cbDoubleCount == 0)	return false;

		//获取上家出牌数值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);

		if (TurnResult.cbThreeCount == 1)    //三代二
		{
			int index = 0;
			for (int i = 0; i < CurResult.cbThreeCount; i++)
			{
				if (GetCardLogicValue(CurResult.cbThreeCardData[index]) > TurnLogicValue)
					return true;

				index += 3;
			}
		}
		else		//飞机
		{
			int index = 0;
			int flag = 1;   //判断飞机几个三张的标志
							//判断三个是否连续
			for (int i = 0; i < CurResult.cbThreeCount; i++)
			{
				if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) - 1) == (GetCardLogicValue(CurResult.cbThreeCardData[index + 3])))  //连续
				{
					flag++;
				}

				if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) >(TurnLogicValue - TurnResult.cbThreeCount + 2))
					&& (flag == TurnResult.cbThreeCount))   //大小比较
					return true;

				index += 3;
			}
		}
	}
	else if ((TurnCardType & (1 << CT_FOUR_LINE_TAKE_ONE)) != 0)   //四代二
	{
		cout << "四代二" << endl;

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//炸弹判断
		if (CurResult.cbFourCount != 0)		return true;

	}
	else if ((TurnCardType & (1 << CT_FOUR_LINE_TAKE_TWO)) != 0)   //四代二对
	{
		cout << "四代二对" << endl;

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//炸弹判断
		if (CurResult.cbFourCount != 0)		return true;

	}

	return false;
}

//玩家第一个出牌  判断他可以出的合适的牌
bool CGameData::AnalysePlayerOutCardFirst(WORD OutCardUsr, BYTE OutCardData[], BYTE *OutCardNum)
{
	cout << "func AnalysePlayerOutCardFirst() Begin::::" << endl;

	//校验
	if (OutCardData == NULL || OutCardUsr == INVALID_CHAIR)
	{
		return false;
	}

	//构建值
	BYTE tmpOutCardData[MAX_CARD_COUNT];
	BYTE tmpOutCardNum = 0;
	ZeroMemory(&tmpOutCardData, sizeof(tmpOutCardData));

	//获取当前玩家的手牌数据
	BYTE CurCardNum = m_players_config.players[OutCardUsr].hand_card_num;
	BYTE CurCardData[MAX_CARD_COUNT];
	ZeroMemory(&CurCardData, sizeof(CurCardData));

	if (CurCardNum > MAX_CARD_COUNT)
	{
		return false;
	}

	for (int i = 0; i < CurCardNum; i++)
	{
		CurCardData[i] = m_players_config.hand_cards[OutCardUsr][i];
	}

	//NoLaiZi的排序
	SortCardListNoLaiZi(CurCardData, CurCardNum, ST_ORDER);

	//分析两个扑克牌型
	tagAnalyseResult CurResult;
	ZeroMemory(&CurResult, sizeof(CurResult));

	AnalysebCardData(CurCardData, CurCardNum, CurResult);

	//判断能出的牌  优先出最小的牌值  炸弹最后出

	cout << "开始进行比较" << endl;

	//如果最小的牌为炸弹
	if (CurResult.cbFourCount != 0)
	{
		BYTE MinData = GetCardLogicValue(CurCardData[CurCardNum - 1]);		//获取最小牌
		WORD Fourcount = CurResult.cbFourCount;

		while (MinData == CurResult.cbFourCardData[Fourcount * 4 - 1])
		{
			CurCardNum -= 4;
			Fourcount--;

			if (0 == CurCardNum)
				break;

			MinData = GetCardLogicValue(CurCardData[CurCardNum - 1]);
		}
	}
	//比较最小牌型
	if (CurResult.cbSignedCount != 0 && CurResult.cbDoubleCount != 0 && CurResult.cbThreeCount != 0)
	{
		cout << "1";

		BYTE MinData = GetCardLogicValue(CurCardData[CurCardNum - 1]);		//获取最小牌
		cout << "MinData:" << (int)MinData << endl;
		cout << "cbDoubleCardData:" << (int)GetCardLogicValue(CurResult.cbDoubleCardData[CurResult.cbDoubleCount * 2 - 1]) << endl;
		cout << "cbThreeCardData:" << (int)GetCardLogicValue(CurResult.cbThreeCardData[CurResult.cbThreeCount * 3 - 1]) << endl;
		if (MinData == GetCardLogicValue(CurResult.cbSignedCardData[CurResult.cbSignedCount - 1]))		//单排
		{
			cout << "1.1";
			tmpOutCardData[0] = CurResult.cbSignedCardData[CurResult.cbSignedCount - 1];
			tmpOutCardNum = 1;
		}
		else if (MinData == GetCardLogicValue(CurResult.cbDoubleCardData[CurResult.cbDoubleCount * 2 - 1]))   //对子
		{
			cout << "1.2";
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurResult.cbDoubleCardData[CurResult.cbDoubleCount * 2 - 1 - i];

			tmpOutCardNum = 2;
		}
		else if (MinData == GetCardLogicValue(CurResult.cbThreeCardData[CurResult.cbThreeCount * 3 - 1]))		//三代一
		{
			cout << "1.3";
			for (int i = 0; i < 3; i++)
				tmpOutCardData[i] = CurResult.cbThreeCardData[CurResult.cbThreeCount * 3 - 1 - i];

			tmpOutCardData[3] = CurResult.cbSignedCardData[CurResult.cbSignedCount - 1];
			tmpOutCardNum = 4;
		}

		memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
		*OutCardNum = tmpOutCardNum;
	}
	else if (CurResult.cbDoubleCount != 0 && CurResult.cbThreeCount != 0)
	{
		cout << "2";
		BYTE MinData = GetCardLogicValue(CurCardData[CurCardNum - 1]);		//获取最小牌

		if (MinData == GetCardLogicValue(CurResult.cbDoubleCardData[CurResult.cbDoubleCount * 2 - 1]))   //对子
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurResult.cbDoubleCardData[CurResult.cbDoubleCount * 2 - 1 - i];

			tmpOutCardNum = 2;
		}
		else if (MinData == GetCardLogicValue(CurResult.cbThreeCardData[CurResult.cbThreeCount * 3 - 1]))		//三代二
		{
			for (int i = 0; i < 3; i++)
				tmpOutCardData[i] = CurResult.cbThreeCardData[CurResult.cbThreeCount * 3 - 1 - i];

			tmpOutCardData[3] = CurResult.cbDoubleCardData[CurResult.cbDoubleCount * 2 - 1];
			tmpOutCardData[4] = CurResult.cbDoubleCardData[CurResult.cbDoubleCount * 2 - 2];
			tmpOutCardNum = 5;
		}

		memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
		*OutCardNum = tmpOutCardNum;
	}
	else if (CurResult.cbSignedCount != 0 && CurResult.cbDoubleCount != 0)
	{
		cout << "3";
		BYTE MinData = GetCardLogicValue(CurCardData[CurCardNum - 1]);		//获取最小牌

		if (MinData == GetCardLogicValue(CurResult.cbSignedCardData[CurResult.cbSignedCount - 1]))		//单排
		{
			tmpOutCardData[0] = CurResult.cbSignedCardData[CurResult.cbSignedCount - 1];
			tmpOutCardNum = 1;
		}
		else if (MinData == GetCardLogicValue(CurResult.cbDoubleCardData[CurResult.cbDoubleCount * 2 - 1]))   //对子
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurResult.cbDoubleCardData[CurResult.cbDoubleCount * 2 - 1 - i];

			tmpOutCardNum = 2;
		}

		memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
		*OutCardNum = tmpOutCardNum;
	}
	else if (CurResult.cbSignedCount != 0 && CurResult.cbThreeCount != 0)
	{
		cout << "4";
		BYTE MinData = GetCardLogicValue(CurCardData[CurCardNum - 1]);		//获取最小牌
		if (MinData == GetCardLogicValue(CurResult.cbSignedCardData[CurResult.cbSignedCount - 1]))		//单排
		{
			tmpOutCardData[0] = CurResult.cbSignedCardData[CurResult.cbSignedCount - 1];
			tmpOutCardNum = 1;
		}

		else if (MinData == GetCardLogicValue(CurResult.cbThreeCardData[CurResult.cbThreeCount * 3 - 1]))		//三代一
		{
			for (int i = 0; i < 3; i++)
				tmpOutCardData[i] = CurResult.cbThreeCardData[CurResult.cbThreeCount * 3 - 1 - i];

			tmpOutCardData[3] = CurResult.cbSignedCardData[CurResult.cbSignedCount - 1];
			tmpOutCardNum = 4;
		}

		memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
		*OutCardNum = tmpOutCardNum;
	}
	else if (CurResult.cbDoubleCount != 0)
	{

		for (int i = 0; i < 2; i++)
			tmpOutCardData[i] = CurResult.cbDoubleCardData[CurResult.cbDoubleCount * 2 - 1 - i];

		tmpOutCardNum = 2;

		memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
		*OutCardNum = tmpOutCardNum;
	}
	else if (CurResult.cbSignedCount != 0)
	{

		tmpOutCardData[0] = CurResult.cbSignedCardData[CurResult.cbSignedCount - 1];
		tmpOutCardNum = 1;

		memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
		*OutCardNum = tmpOutCardNum;

	}
	else if (CurResult.cbThreeCount != 0)
	{

		for (int i = 0; i < 3; i++)
			tmpOutCardData[i] = CurResult.cbThreeCardData[CurResult.cbThreeCount * 3 - 1 - i];

		tmpOutCardNum = 3;


		memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
		*OutCardNum = tmpOutCardNum;

	}
	else if (CurResult.cbFourCount != 0)
	{
		for (int i = 0; i < 4; i++)
			tmpOutCardData[i] = CurResult.cbFourCardData[CurResult.cbFourCount * 4 - 1 - i];

		tmpOutCardNum = 4;

		memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
		*OutCardNum = tmpOutCardNum;
	}

	return true;
}

//玩家接牌  判断他可以出的牌
bool CGameData::AnalysePlayerOutCard(WORD OutCardUsr, BYTE OutCardData[], BYTE *OutCardNum)
{
	//构建值
	BYTE tmpOutCardData[MAX_CARD_COUNT];
	BYTE tmpOutCardNum = 0;
	ZeroMemory(&tmpOutCardData, sizeof(tmpOutCardData));

	//获得该轮最大出牌数据
	BYTE TurnCardNum = m_playing_para.turn_cards_num;
	BYTE TurnCardData[MAX_CARD_COUNT];
	ZeroMemory(&TurnCardData, sizeof(TurnCardData));
	CopyMemory(TurnCardData, m_playing_para.turn_max_cards, sizeof(TurnCardData));

	//获取当前玩家的手牌数据
	BYTE CurCardNum = m_players_config.players[OutCardUsr].hand_card_num;
	BYTE CurCardData[MAX_CARD_COUNT];
	ZeroMemory(&CurCardData, sizeof(CurCardData));

	for (int i = 0; i < CurCardNum; i++)
	{
		CurCardData[i] = m_players_config.hand_cards[OutCardUsr][i];
	}

	//分析两个扑克牌型
	tagAnalyseResult TurnResult;
	tagAnalyseResult CurResult;

	ZeroMemory(&TurnResult, sizeof(TurnResult));
	ZeroMemory(&CurResult, sizeof(CurResult));

	AnalysebCardData(CurCardData, CurCardNum, CurResult);
	AnalysebCardData(TurnCardData, TurnCardNum, TurnResult);

	//获取出牌玩家的牌型
	int TurnCardType = GetType(TurnResult, TurnCardData, TurnCardNum, TurnCardData);
	TurnCardType = TransListToInt(0, TurnCardType);

	//对比判断牌型
	if (TurnCardType == CT_ERROR)	return false;
	if ((TurnCardType & (1 << CT_MISSILE_CARD)) != 0)    return false;    //王炸判断

																		  //炸弹判断
	if ((TurnCardType & (1 << CT_BOMB_CARD)) != 0)   //上家出炸弹
	{
		cout << "炸弹" << endl;

		//获取上家出牌数值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbFourCardData[0]);
		int index = 0;   //辅助标识

						 //if (CurResult.cbFourCount == 0)    return false;
		for (int i = 0; i < CurResult.cbFourCount; i++)
		{
			if (GetCardLogicValue(CurResult.cbFourCardData[index]) > TurnLogicValue)
			{
				for (int i = 0; i < 4; i++)
					tmpOutCardData[i] = CurResult.cbFourCardData[index++];
				tmpOutCardNum = 4;

				memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
				*OutCardNum = tmpOutCardNum;

				return true;
			}

			index += 4;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
			{
				tmpOutCardData[i] = CurCardData[i];
			}
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}


	}
	else if ((TurnCardType & (1 << CT_SINGLE)) != 0)   //单张
	{
		cout << "单张" << endl;

		//获取上家出牌数值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbSignedCardData[0]);

		cout << "获取上家出牌数值:" << (int)TurnLogicValue << endl;

		int index = (CurResult.cbSignedCount - 1);
		for (int i = 0; i < CurResult.cbSignedCount; i++)
		{
			if (GetCardLogicValue(CurResult.cbSignedCardData[index]) > TurnLogicValue)
			{
				for (int i = 0; i < 2; i++)
					tmpOutCardData[i] = CurResult.cbSignedCardData[index++];
				tmpOutCardNum = 1;

				//cout << " tmpOutCardData[i]: " << tmpOutCardData[i] << endl;

				memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
				*OutCardNum = tmpOutCardNum;

				return true;
			}

			index--;
		}

		for (int i = CurCardNum - 1; i > 0; i--)
		{
			if (GetCardLogicValue(CurCardData[i]) > TurnLogicValue)
			{
				tmpOutCardData[0] = CurCardData[i];
				tmpOutCardNum = 1;

				memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
				*OutCardNum = tmpOutCardNum;

				return true;
			}
		}

		//炸弹判断
		if (CurResult.cbFourCount != 0)
		{
			int index = 0;
			for (int i = (CurResult.cbFourCount - 1) * 4; i < CurResult.cbFourCount * 4; i++)
			{
				tmpOutCardData[index++] = CurResult.cbFourCardData[i];
			}
			tmpOutCardNum = 4;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurCardData[i];
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_DOUBLE)) != 0)   //对子
	{
		cout << "对子" << endl;

		//获取上家出牌数值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbDoubleCardData[0]);

		int index = (CurResult.cbDoubleCount - 1) * 2;
		for (int i = 0; i < CurResult.cbDoubleCount; i++)
		{
			if (GetCardLogicValue(CurResult.cbDoubleCardData[index]) > TurnLogicValue)
			{
				for (int i = 0; i < 2; i++)
					tmpOutCardData[i] = CurResult.cbDoubleCardData[index++];
				tmpOutCardNum = 2;

				//cout << " tmpOutCardData[i]: " << tmpOutCardData[i] << endl;

				memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
				*OutCardNum = tmpOutCardNum;

				return true;
			}

			index -= 2;
		}

		index = (CurResult.cbThreeCount - 1) * 3;
		for (int i = 0; i < CurResult.cbThreeCount; i++)
		{
			if (GetCardLogicValue(CurResult.cbThreeCardData[index]) > TurnLogicValue)
			{
				for (int i = 0; i < 2; i++)
					tmpOutCardData[i] = CurResult.cbThreeCardData[index++];
				tmpOutCardNum = 2;

				memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
				*OutCardNum = tmpOutCardNum;

				return true;
			}

			index -= 3;
		}

		//炸弹判断
		if (CurResult.cbFourCount != 0)
		{
			int index = 0;
			for (int i = (CurResult.cbFourCount - 1) * 4; i < CurResult.cbFourCount * 4; i++)
			{
				tmpOutCardData[index++] = CurResult.cbFourCardData[i];
			}
			tmpOutCardNum = 4;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurCardData[i];
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_THREE)) != 0)   //三张
	{
		cout << "三张" << endl;

		//获取上家出牌数值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);

		int index = (CurResult.cbThreeCount - 1) * 3;
		for (int i = 0; i < CurResult.cbThreeCount; i++)
		{
			if (GetCardLogicValue(CurResult.cbThreeCardData[index]) > TurnLogicValue)
			{
				for (int i = 0; i < 3; i++)
					tmpOutCardData[i] = CurResult.cbThreeCardData[index++];
				tmpOutCardNum = 3;

				memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
				*OutCardNum = tmpOutCardNum;

				return true;
			}

			index -= 3;
		}

		//炸弹判断
		if (CurResult.cbFourCount != 0)
		{
			int index = 0;
			for (int i = (CurResult.cbFourCount - 1) * 4; i < CurResult.cbFourCount * 4; i++)
			{
				tmpOutCardData[index++] = CurResult.cbFourCardData[i];
			}
			tmpOutCardNum = 4;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurCardData[i];
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_SINGLE_LINE)) != 0)   //顺子
	{
		cout << "顺子" << endl;

		//获取上家出牌数值  最大值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbSignedCardData[0]);
		cout << "顺子:获取上家出牌数值:" << (int)TurnLogicValue << endl;

		int flag = 0;
		int index = 0;

		for (int i = 0; i < CurCardNum; i++)     //计算顺子张数
		{
			printf("第%d张牌的连续标志：%d\n", i, flag);
			if (flag != 0)    //连续不用比牌大小
			{
				if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
				{
					continue;
				}
				if (GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //判断是否连续
				{
					tmpOutCardData[index++] = CurCardData[i];
					tmpOutCardNum++;

					flag++;

					//跳出条件
					if (flag == TurnResult.cbSignedCount - 1)
					{
						tmpOutCardData[index] = CurCardData[i + 1];
						tmpOutCardNum++;
						break;
					}
				}
				else
				{
					ZeroMemory(&tmpOutCardData, sizeof(tmpOutCardData));
					tmpOutCardNum = 0;

					index = 0;
					flag = 0;
				}
			}
			else
			{
				if (GetCardLogicValue(CurCardData[i]) > TurnLogicValue)
				{
					if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
					{
						continue;
					}
					if (GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //判断是否连续
					{
						tmpOutCardData[index++] = CurCardData[i];
						tmpOutCardNum++;

						flag++;

						//跳出条件
						if (flag == TurnResult.cbSignedCount - 1)
							break;
					}
					else
					{
						ZeroMemory(&tmpOutCardData, sizeof(tmpOutCardData));
						tmpOutCardNum = 0;

						index = 0;
						flag = 0;
					}
				}
			}
		}

		if (flag >= TurnResult.cbSignedCount - 1)
		{
			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//炸弹判断
		if (CurResult.cbFourCount != 0)
		{
			int index = 0;
			for (int i = (CurResult.cbFourCount - 1) * 4; i < CurResult.cbFourCount * 4; i++)
			{
				tmpOutCardData[index++] = CurResult.cbFourCardData[i];
			}
			tmpOutCardNum = 4;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurCardData[i];
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_DOUBLE_LINE)) != 0)   //连对
	{
		cout << "连对" << endl;


		//获取上家出牌数值  最大值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbDoubleCardData[0]);
		int flag = 0;   //判断是否连续标志
		int Numflag = 0;   //判断牌数是否达标的标志
		int index = 0;

		for (int i = 0; i < CurCardNum; i++)     //计算连对张数
		{
			if (flag != 0)    //连续不用比牌大小
			{
				if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
				{
					Numflag++;
					continue;
				}
				if ((GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1))
					&& (Numflag > 0) && (GetCardLogicValue(CurCardData[i]) < 15))   //判断是否连续
				{
					tmpOutCardData[index++] = CurCardData[i];		//连对加两次
					tmpOutCardNum++;

					tmpOutCardData[index++] = CurCardData[i - 1];
					tmpOutCardNum++;

					flag++;

					//跳出条件
					if (flag == TurnResult.cbDoubleCount)
						break;
				}
				else
				{
					ZeroMemory(&tmpOutCardData, sizeof(tmpOutCardData));
					tmpOutCardNum = 0;

					index = 0;
					flag = 0;
				}

				if (GetCardLogicValue(CurCardData[i + 1]) != GetCardLogicValue(CurCardData[i]))
				{
					Numflag = 0;
				}
			}
			else
			{
				if (GetCardLogicValue(CurCardData[i]) > TurnLogicValue)
				{
					if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
					{
						Numflag++;
						continue;
					}
					if (GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1)
						&& (Numflag > 0) && (GetCardLogicValue(CurCardData[i]) < 15))   //判断是否连续
					{
						tmpOutCardData[index++] = CurCardData[i];		//连对加两次
						tmpOutCardNum++;

						tmpOutCardData[index++] = CurCardData[i - 1];
						tmpOutCardNum++;

						flag++;

						//跳出条件
						if (flag == TurnResult.cbDoubleCount)
							break;
					}
					else
					{
						ZeroMemory(&tmpOutCardData, sizeof(tmpOutCardData));
						tmpOutCardNum = 0;

						index = 0;
						flag = 0;
					}

					if (GetCardLogicValue(CurCardData[i + 1]) != GetCardLogicValue(CurCardData[i]))
					{
						Numflag = 0;
					}
				}
			}
		}

		if (flag >= TurnResult.cbDoubleCount)
		{
			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//炸弹判断
		if (CurResult.cbFourCount != 0)
		{
			int index = 0;
			for (int i = (CurResult.cbFourCount - 1) * 4; i < CurResult.cbFourCount * 4; i++)
			{
				tmpOutCardData[index++] = CurResult.cbFourCardData[i];
			}
			tmpOutCardNum = 4;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurCardData[i];
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_THREE_LINE)) != 0)   //三连
	{
		cout << "三连" << endl;

		//获取上家出牌数值  最大值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);
		int flag = 0;   //判断是否连续标志
		int Numflag = 0;   //判断牌数是否达标的标志
		int index = 0;

		for (int i = 0; i < CurCardNum; i++)     //计算连对张数
		{
			if (flag != 0)    //连续不用比牌大小
			{
				if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
				{
					Numflag++;
					continue;
				}
				if ((GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1))
					&& (Numflag > 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //判断是否连续
				{
					tmpOutCardData[index++] = CurCardData[i];		//三连加三次
					tmpOutCardNum++;

					tmpOutCardData[index++] = CurCardData[i - 1];
					tmpOutCardNum++;

					tmpOutCardData[index++] = CurCardData[i - 2];
					tmpOutCardNum++;

					flag++;

					//跳出条件
					if (flag == TurnResult.cbThreeCount)
						break;
				}
				else
				{
					ZeroMemory(&tmpOutCardData, sizeof(tmpOutCardData));
					tmpOutCardNum = 0;

					index = 0;
					flag = 0;
				}

				if (GetCardLogicValue(CurCardData[i + 1]) != GetCardLogicValue(CurCardData[i]))   //不是同牌了 标志位制零
				{
					Numflag = 0;
				}
			}
			else
			{
				if (GetCardLogicValue(CurCardData[i]) > TurnLogicValue)
				{
					if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
					{
						Numflag++;
						continue;
					}
					if (GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1)
						&& (Numflag > 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //判断是否连续
					{
						tmpOutCardData[index++] = CurCardData[i];		//三连加三次
						tmpOutCardNum++;

						tmpOutCardData[index++] = CurCardData[i - 1];
						tmpOutCardNum++;

						tmpOutCardData[index++] = CurCardData[i - 2];
						tmpOutCardNum++;

						flag++;

						//跳出条件
						if (flag == TurnResult.cbThreeCount)
							break;
					}
					else
					{
						ZeroMemory(&tmpOutCardData, sizeof(tmpOutCardData));
						tmpOutCardNum = 0;

						index = 0;
						flag = 0;
					}

					if (GetCardLogicValue(CurCardData[i + 1]) != GetCardLogicValue(CurCardData[i]))   //不是同牌了 标志位制零
					{
						Numflag = 0;
					}
				}
			}
		}

		if (flag >= TurnResult.cbThreeCount)
		{
			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//炸弹判断
		if (CurResult.cbFourCount != 0)
		{
			int index = 0;
			for (int i = (CurResult.cbFourCount - 1) * 4; i < CurResult.cbFourCount * 4; i++)
			{
				tmpOutCardData[index++] = CurResult.cbFourCardData[i];
			}
			tmpOutCardNum = 4;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurCardData[i];
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_THREE_LINE_TAKE_ONE)) != 0)   //三代一
	{

		cout << "三代一" << endl;


		//获取上家出牌数值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);

		if (CurResult.cbSignedCount >= TurnResult.cbThreeCount)     //没有单张就没必要出这个牌
		{
			if (TurnResult.cbThreeCount == 1)    //三代一
			{
				int index = 0;
				for (int i = 0; i < CurResult.cbThreeCount; i++)
				{
					if (GetCardLogicValue(CurResult.cbThreeCardData[index]) > TurnLogicValue)
					{
						for (int j = 0; j < 3; j++)
						{
							tmpOutCardData[j] = CurResult.cbThreeCardData[j];
						}
						tmpOutCardData[3] = CurResult.cbSignedCardData[TurnResult.cbSignedCount - 1];
						tmpOutCardNum = 4;

						memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
						*OutCardNum = tmpOutCardNum;

						return true;
					}

					index += 3;
				}
			}
			else		//飞机
			{
				int index = 0;
				int flag = 1;   //判断飞机几个三张的标志
								//判断三个是否连续
				for (int i = 0; i < CurResult.cbThreeCount; i++)
				{
					if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) - 1) == (GetCardLogicValue(CurResult.cbThreeCardData[index + 3])))  //连续
					{
						flag++;
					}

					if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) >(TurnLogicValue - TurnResult.cbThreeCount + 2))
						&& (flag == TurnResult.cbThreeCount))   //大小比较
					{
						index = index - TurnResult.cbThreeCount * 3 + 3;     //赋值的下标
						for (int j = 0; j < TurnResult.cbThreeCount * 3; j++)
						{
							tmpOutCardData[j] = CurResult.cbThreeCardData[index++];
						}

						for (int j = 0; j < TurnResult.cbThreeCount; j++)
						{
							tmpOutCardData[TurnResult.cbThreeCount * 3 + j] = CurResult.cbSignedCardData[TurnResult.cbSignedCount - j - 1];   //飞机带的单
						}

						tmpOutCardNum = TurnResult.cbThreeCount * 4;

						memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
						*OutCardNum = tmpOutCardNum;

						return true;
					}

					index += 3;
				}
			}
		}

		//炸弹判断
		if (CurResult.cbFourCount != 0)
		{
			int index = 0;
			for (int i = (CurResult.cbFourCount - 1) * 4; i < CurResult.cbFourCount * 4; i++)
			{
				tmpOutCardData[index++] = CurResult.cbFourCardData[i];
			}
			tmpOutCardNum = 4;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurCardData[i];
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_THREE_LINE_TAKE_TWO)) != 0)   //三代二
	{
		cout << "三代二" << endl;

		//获取上家出牌数值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);

		if (CurResult.cbDoubleCount >= TurnResult.cbThreeCount)     //没有对子就没必要出这个牌
		{
			if (TurnResult.cbThreeCount == 1)    //三代二
			{
				int index = 0;
				for (int i = 0; i < CurResult.cbThreeCount; i++)
				{
					if (GetCardLogicValue(CurResult.cbThreeCardData[index]) > TurnLogicValue)
					{
						for (int j = 0; j < 3; j++)
						{
							tmpOutCardData[j] = CurResult.cbThreeCardData[j];
						}
						tmpOutCardData[3] = CurResult.cbDoubleCardData[TurnResult.cbDoubleCount * 2 - 1];
						tmpOutCardData[4] = CurResult.cbDoubleCardData[TurnResult.cbDoubleCount * 2 - 2];
						tmpOutCardNum = 5;

						memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
						*OutCardNum = tmpOutCardNum;

						return true;
					}

					index += 3;
				}
			}
			else		//飞机
			{
				int index = 0;
				int flag = 1;   //判断飞机几个三张的标志
								//判断三个是否连续
				for (int i = 0; i < CurResult.cbThreeCount; i++)
				{
					if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) - 1) == (GetCardLogicValue(CurResult.cbThreeCardData[index + 3])))  //连续
					{
						flag++;
					}

					if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) >(TurnLogicValue - TurnResult.cbThreeCount + 2))
						&& (flag == TurnResult.cbThreeCount))   //大小比较
					{
						index = index - TurnResult.cbThreeCount * 3 + 3;     //赋值的下标
						for (int j = 0; j < TurnResult.cbThreeCount * 3; j++)
						{
							tmpOutCardData[j] = CurResult.cbThreeCardData[index++];
						}

						for (int j = 0; j < TurnResult.cbThreeCount * 2; j += 2)
						{
							tmpOutCardData[TurnResult.cbThreeCount * 3 + j] = CurResult.cbDoubleCardData[TurnResult.cbDoubleCount * 2 - j - 1];   //飞机带的对
							tmpOutCardData[TurnResult.cbThreeCount * 3 + j + 1] = CurResult.cbDoubleCardData[TurnResult.cbDoubleCount * 2 - j - 1];
						}

						tmpOutCardNum = TurnResult.cbThreeCount * 5;

						memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
						*OutCardNum = tmpOutCardNum;

						return true;
					}

					index += 3;
				}
			}
		}

		//炸弹判断
		if (CurResult.cbFourCount != 0)
		{
			int index = 0;
			for (int i = (CurResult.cbFourCount - 1) * 4; i < CurResult.cbFourCount * 4; i++)
			{
				tmpOutCardData[index++] = CurResult.cbFourCardData[i];
			}
			tmpOutCardNum = 4;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurCardData[i];
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_FOUR_LINE_TAKE_ONE)) != 0)   //四代二
	{
		cout << "四代二" << endl;

		//炸弹判断
		if (CurResult.cbFourCount != 0)
		{
			int index = 0;
			for (int i = (CurResult.cbFourCount - 1) * 4; i < CurResult.cbFourCount * 4; i++)
			{
				tmpOutCardData[index++] = CurResult.cbFourCardData[i];
			}
			tmpOutCardNum = 4;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurCardData[i];
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_FOUR_LINE_TAKE_TWO)) != 0)   //四代二对
	{
		cout << "四代二对" << endl;

		//炸弹判断
		if (CurResult.cbFourCount != 0)
		{
			int index = 0;
			for (int i = (CurResult.cbFourCount - 1) * 4; i < CurResult.cbFourCount * 4; i++)
			{
				tmpOutCardData[index++] = CurResult.cbFourCardData[i];
			}
			tmpOutCardNum = 4;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurCardData[i];
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

	}

	return false;

}

//记牌器
bool CGameData::CardRecorder(WORD wChairID, tagCardRecorder &Recorder)
{

	int _playernum = GetCurPlayerCount();

	tagCardRecorder tagRec;
	ZeroMemory(&tagRec, sizeof(tagRec));

	for (int i = 0; i < _playernum; i++)
	{
		if (wChairID != i)
		{
			for (int j = 0; j < m_players_config.players[i].hand_card_num; j++)
			{
				if (3 == GetCardLogicValue(m_players_config.hand_cards[i][j]))		tagRec.ThreeCount++;
				if (4 == GetCardLogicValue(m_players_config.hand_cards[i][j]))		tagRec.FourCount++;
				if (5 == GetCardLogicValue(m_players_config.hand_cards[i][j]))		tagRec.FiveCount++;
				if (6 == GetCardLogicValue(m_players_config.hand_cards[i][j]))		tagRec.SixCount++;
				if (7 == GetCardLogicValue(m_players_config.hand_cards[i][j]))		tagRec.SevenCount++;
				if (8 == GetCardLogicValue(m_players_config.hand_cards[i][j]))		tagRec.EightCount++;
				if (9 == GetCardLogicValue(m_players_config.hand_cards[i][j]))		tagRec.NineCount++;
				if (10 == GetCardLogicValue(m_players_config.hand_cards[i][j]))		tagRec.TenCount++;
				if (11 == GetCardLogicValue(m_players_config.hand_cards[i][j]))		tagRec.JCount++;
				if (12 == GetCardLogicValue(m_players_config.hand_cards[i][j]))		tagRec.QCount++;
				if (13 == GetCardLogicValue(m_players_config.hand_cards[i][j]))		tagRec.KCount++;
				if (14 == GetCardLogicValue(m_players_config.hand_cards[i][j]))		tagRec.ACount++;
				if (15 == GetCardLogicValue(m_players_config.hand_cards[i][j]))		tagRec.TwoCount++;
				if (16 == GetCardLogicValue(m_players_config.hand_cards[i][j]))		tagRec.SmallJokerCount++;
				if (17 == GetCardLogicValue(m_players_config.hand_cards[i][j]))		tagRec.BigJokerCount++;
			}
			cout << endl;
		}
	}

	memcpy(&Recorder, &tagRec, sizeof(Recorder));

	return true;
}

// 结算得分
int CGameData::Settle()
{
	return 0;
}

// 清理小局游戏数据
int CGameData::ClearXjGame()
{
	cout << "清理小局数据" << endl;

	// 清理小局玩家数据
	memset(m_players_config.hand_cards, 0, sizeof(m_players_config.hand_cards));
	for (unsigned i = 0; i < m_players_config.player_sum; ++i)
	{

		m_players_config.players[i].bank_state = 0;
		m_players_config.players[i].bet = 1;
		m_players_config.players[i].bet_state = 0;

		m_players_config.players[i].single_score = 0;
		m_players_config.players[i].showcard_state = 0;
		m_players_config.players[i].times = 0;
		m_players_config.players[i].xj_ready_state = 0;
		m_players_config.players[i].action_state = 0;
		m_players_config.players[i].action = 0;
		m_players_config.players[i].rob_num = 0;
		m_players_config.players[i].rob_score = 0;
		m_players_config.players[i].mingpai_state = 0;
		m_players_config.players[i].bet_state = 0;
		m_players_config.players[i].mingpai_bet = 1;
		m_players_config.players[i].addscore_bet = 1;
		m_players_config.players[i].boom_info.cbBoomNum = 0;
		m_players_config.players[i].jiaofen_state = 10;
		m_players_config.players[i].spring = 0;
		m_players_config.players[i].spring_state = 0;
		m_players_config.players[i].player_tuo_guan = 0;
		m_players_config.players[i].out_time_num = 0;
	}

	//清理游戏参数
	m_playing_para.appointbanker = INVALID_CHAIR;
	m_playing_para.game_state = GS_WK_FREE;
	m_playing_para.cur_total_bet = 0;
	m_playing_para.room_bet = 1;
	m_playing_para.turn_cards_num = 0;   //最大出牌数据初始化
	m_playing_para.jiaofen_count = 0;	 //初始化叫分次数
	m_playing_para.leave_card_bet = 1;

	//排序权位清空
	for (BYTE i = 0; i < LEN_SORT_RIGHT; i++)
		m_playing_para.sort_right[i] = i * COLOR_RIGHT;

	//主花色和级牌清空
	//m_playing_para.level_value = VALUE_ERROR;	TODO 看需不需要升级，不升级每次级牌都为2
	m_playing_para.main_color = COLOR_ERROR;
	ZeroMemory(m_playing_para.leave_card, sizeof(m_playing_para.leave_card));
	m_playing_para.liangzhu_user = INVALID_CHAIR;
	m_playing_para.cur_fanzhu_card = INVALID_CARD;
	m_playing_para.start_outcard_user = INVALID_CHAIR;
	ZeroMemory(m_playing_para.score_card, sizeof(m_playing_para.score_card));
	m_playing_para.cur_fanzhu_user = INVALID_CHAIR;
	m_playing_para.cur_koudi_user = INVALID_CHAIR;
	m_playing_para.start_rob_user = INVALID_CHAIR;
	m_playing_para.magic_card = INVALID_CARD;

	return 0;
}

// 清理大局游戏数据
int  CGameData::ClearDjGame()
{
	// 清理大局玩家数据
	memset(m_players_config.hand_cards, 0, sizeof(m_players_config.hand_cards));
	for (unsigned i = 0; i < m_players_config.player_sum; ++i)
	{

		m_players_config.players[i].bank_state = 0;
		m_players_config.players[i].bet = 1;
		m_players_config.players[i].bet_state = 0;

		m_players_config.players[i].single_score = 0;
		m_players_config.players[i].showcard_state = 0;
		m_players_config.players[i].times = 0;
		m_players_config.players[i].xj_ready_state = 0;
		m_players_config.players[i].card_type = 0;
		m_players_config.players[i].total_score = 0;
		m_players_config.players[i].winsum = 0;
		m_players_config.players[i].best_bet = 0;
		m_players_config.players[i].bank_count = 0;
		//m_players_config.players[i].play_state		= 0;
	}

	//清理游戏参数
	m_playing_para.liangzhu_user = INVALID_CHAIR;
	m_playing_para.last_winner_id = INVALID_CHAIR;
	m_playing_para.game_state = GS_WK_FREE;
	m_playing_para.cur_fanzhu_card = INVALID_CARD;

	return 0;
}

// 清理房间数据
int CGameData::ClearRoom()
{
	if (m_card_config.game_cards != NULL)
		delete[] m_card_config.game_cards;
	if (m_players_config.players != NULL)
		delete[] m_players_config.players;

	return 0;
}

//按花色权位排序扑克  //垃圾函数
void CGameData::SortCardList(BYTE *cbCardData, BYTE cbCardCount)
{
	//排序索引数组
	BYTE cbSortValue[MAX_CARD_COUNT];
	for (BYTE i = 0; i < cbCardCount; i++)
		cbSortValue[i] = GetCardSortOrder(cbCardData[i]);

	//排序操作
	bool bSorted = true;
	BYTE bTempData, bLast = cbCardCount - 1;
	do
	{
		bSorted = true;
		for (BYTE i = 0; i < bLast; i++)
		{
			if ((cbSortValue[i] < cbSortValue[i + 1]) ||
				((cbSortValue[i] == cbSortValue[i + 1]) && (cbCardData[i] < cbCardData[i + 1])))
			{
				//交换位置
				bTempData = cbCardData[i];
				cbCardData[i] = cbCardData[i + 1];
				cbCardData[i + 1] = bTempData;
				bTempData = cbSortValue[i];
				cbSortValue[i] = cbSortValue[i + 1];
				cbSortValue[i + 1] = bTempData;
				bSorted = false;
			}
		}
		bLast--;
	} while (bSorted == false);
}

//按花色权位给用户手牌排序
void CGameData::SortCardList(const WORD &wChairID, BYTE cbCardCount, BYTE cbSortType)
{
	//手牌排序
	SortCardList(m_players_config.hand_cards[wChairID], cbCardCount, cbSortType);
}

//排列扑克
void CGameData::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
	//数目过虑
	if (cbCardCount == 0) return;

	//转换数值
	BYTE cbSortValue[MAX_CARD_COUNT];
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		cbSortValue[i] = GetCardLogicValueLaiZi(cbCardData[i]);
	}

	//排序操作
	bool bSorted = true;
	BYTE cbThreeCount, cbLast = cbCardCount - 1;
	do
	{
		bSorted = true;
		for (BYTE i = 0; i < cbLast; i++)
		{
			if ((cbSortValue[i] < cbSortValue[i + 1]) ||
				((cbSortValue[i] == cbSortValue[i + 1]) && (cbCardData[i] < cbCardData[i + 1])))
			{
				//交换位置
				cbThreeCount = cbCardData[i];
				cbCardData[i] = cbCardData[i + 1];
				cbCardData[i + 1] = cbThreeCount;
				cbThreeCount = cbSortValue[i];
				cbSortValue[i] = cbSortValue[i + 1];
				cbSortValue[i + 1] = cbThreeCount;
				bSorted = false;
			}
		}
		cbLast--;
	} while (bSorted == false);

	//数目排序
	if (cbSortType == ST_COUNT)
	{
		//分析扑克
		BYTE cbIndex = 0;
		tagAnalyseResult AnalyseResult;
		AnalysebCardData(cbCardData, cbCardCount, AnalyseResult);

		//拷贝四牌
		CopyMemory(&cbCardData[cbIndex], AnalyseResult.cbFourCardData, sizeof(BYTE)*AnalyseResult.cbFourCount * 4);
		cbIndex += AnalyseResult.cbFourCount * 4;

		//拷贝三牌
		CopyMemory(&cbCardData[cbIndex], AnalyseResult.cbThreeCardData, sizeof(BYTE)*AnalyseResult.cbThreeCount * 3);
		cbIndex += AnalyseResult.cbThreeCount * 3;

		//拷贝对牌
		CopyMemory(&cbCardData[cbIndex], AnalyseResult.cbDoubleCardData, sizeof(BYTE)*AnalyseResult.cbDoubleCount * 2);
		cbIndex += AnalyseResult.cbDoubleCount * 2;

		//拷贝单牌
		CopyMemory(&cbCardData[cbIndex], AnalyseResult.cbSignedCardData, sizeof(BYTE)*AnalyseResult.cbSignedCount);
		cbIndex += AnalyseResult.cbSignedCount;
	}

	return;
}

//获得卡牌的排序等级
BYTE CGameData::GetCardSortOrder(BYTE cbCardData)
{
	//过滤掉无效卡牌
	if (INVALID_CARD == cbCardData)
		return 0;

	//逻辑数值
	BYTE cbCardColor = GetCardColor(cbCardData);
	BYTE cbCardValue = GetCardValue(cbCardData);

	//鬼牌
	if ((SMALL_JOKER == cbCardData) || (BIG_JOKER == cbCardData))
	{
		BYTE cbSortValueTmp = cbCardValue + 14;
		return (cbSortValueTmp + m_playing_para.sort_right[4]);
	}

	//若卡牌为主花色或等于级牌，排序花色变换
	BYTE cbSortColor = cbCardColor;	//花色从1开始

									//若卡牌值为A，A的排序在前，则排序数值变换
	BYTE cbSortValue = cbCardValue;
	if (cbCardValue == 1)
		cbSortValue += 13;

	//卡牌为癞子，癞子放在最左边，排位最高					//先一律不考虑癞子
	if (m_room_config.game_laizi_mode > 0)
	{
		if (cbCardValue == m_playing_para.magic_card)
		{
			cbSortValue += 14;
			cbSortColor = m_playing_para.sort_right[4];
		}
	}

	return (cbSortValue + m_playing_para.sort_right[cbSortColor]);
}

//获得卡牌的逻辑数值
BYTE CGameData::GetCardLogicValue(BYTE cbCardData)
{
	if (0 == cbCardData)
	{
		return 0;
	}

	//逻辑数值
	BYTE cbCardColor = GetCardColor(cbCardData);
	BYTE cbCardValue = GetCardValue(cbCardData);

	//转换数值明显出现错误
	if ((cbCardColor << 4) == 0x40) return cbCardValue + 2;		//大小王    

	return (cbCardValue <= 2) ? (cbCardValue + 13) : cbCardValue;
}

//分析玩家是否能亮主
bool CGameData::AnalyseUserLiangZhu(const WORD &wChairID, BYTE *cbLiangZhuColor)
{
	//校验
	if ((VALUE_ERROR == m_playing_para.level_value) ||
		(NULL == cbLiangZhuColor))
		return false;

	//临时亮主花色数组
	std::vector <BYTE> vecLZcolor;

	//判断玩家手牌中是否有当前级牌
	for (int i = 0; i < m_card_config.init_card_num; i++)
	{
		BYTE cbCardValue = GetCardValue(m_players_config.hand_cards[wChairID][i]);
		if (cbCardValue == m_playing_para.level_value)
		{
			//去除重复的数据
			std::vector <BYTE>::iterator result = find(vecLZcolor.begin(), vecLZcolor.end(), cbCardValue);
			if (result == vecLZcolor.end())
			{
				BYTE cbCardColor = GetCardColor(m_players_config.hand_cards[wChairID][i]);
				vecLZcolor.push_back(cbCardColor);
			}
		}
	}

	//返回能亮主的卡牌
	int nLiangzhuNum = vecLZcolor.size();
	if (0 != nLiangzhuNum)
	{
		for (int i = 0; i < nLiangzhuNum && i < MAX_COLOR_TYPE_NUM; i++)
		{
			cbLiangZhuColor[i] = vecLZcolor[i];
		}
		return true;
	}

	return false;
}

//分析玩家是否能反主（反主按座位顺序）
bool CGameData::AnalyseUserFanZhu(const WORD &wChairID, BYTE *cbFanZhuCard)
{
	//当前反主卡牌
	BYTE cbCurFanzhuCard = m_playing_para.cur_fanzhu_card;
	BYTE cbCurFanZhuColor = GetCardColor(m_playing_para.cur_fanzhu_card);

	//亮主玩家只有等其他玩家反主之后才能反主
	if (m_playing_para.appointbanker == wChairID)
	{
		//校验是否有玩家反主
		if (INVALID_CARD != cbCurFanzhuCard)
		{
			return IsUserCanFanZhu(wChairID, cbCurFanZhuColor, cbFanZhuCard);
		}
	}
	else
	{
		//无人反主，则判断所有级牌
		BYTE cbColorIndex = 0;
		//当前有玩家反主,则后续玩家反主的花色必须大于该花色
		if (INVALID_CARD != cbCurFanzhuCard)
		{
			cbColorIndex = cbCurFanZhuColor;
		}

		return IsUserCanFanZhu(wChairID, cbColorIndex, cbFanZhuCard);
	}

	return false;
}

//判断玩家手牌是否能反主
bool CGameData::IsUserCanFanZhu(const WORD &wChairID, const BYTE &cbCurFanZhuColor, BYTE *cbFanZhuCard)
{
	//玩家卡牌索引
	BYTE cbCardIndex[LEN_MAX_INDEX];
	SwitchToCardIndex(m_players_config.hand_cards[wChairID], MAX_CARD_COUNT, cbCardIndex);

	//获得大于当前主花色的级牌索引
	BYTE cbFanzhuTypeNum = 0;
	for (BYTE i = cbCurFanZhuColor; i < MAX_COLOR_TYPE_NUM; i++)
	{
		//级牌
		BYTE cbTmpCard = ((i << 4) | m_playing_para.level_value);
		//级牌索引
		BYTE cbCurFanzhuIndex = SwitchToCardIndex(cbTmpCard);

		//是否有两张一样的级牌
		if (2 == cbCardIndex[cbCurFanzhuIndex])
		{
			if (MAX_FANZHU_TYPE_NUM > cbFanzhuTypeNum)
			{
				cbFanZhuCard[cbFanzhuTypeNum] = cbTmpCard;
				cbFanzhuTypeNum++;
			}
		}
	}

	//判断玩家是否有一对小王
	if (2 == cbCardIndex[SMALL_JOKER_INDEX])
	{
		if (MAX_FANZHU_TYPE_NUM > cbFanzhuTypeNum)
		{
			cbFanZhuCard[cbFanzhuTypeNum] = SwitchToCardData(SMALL_JOKER_INDEX);
			cbFanzhuTypeNum++;
		}
	}
	//判断玩家是否有一对大王
	if (2 == cbCardIndex[BIG_JOKER_INDEX])
	{
		if (MAX_FANZHU_TYPE_NUM > cbFanzhuTypeNum)
		{
			cbFanZhuCard[cbFanzhuTypeNum] = SwitchToCardData(BIG_JOKER_INDEX);
			cbFanzhuTypeNum++;
		}
	}

	return (cbFanzhuTypeNum > 0);
}

//发送底牌给玩家
void CGameData::SendLeaveCardToUser(const WORD &wChairID)
{
	//将底牌赋给玩家
	for (int i = 0; i < MAX_LEAVE_CARD_NUM; i++)
	{
		if (INVALID_CARD != m_playing_para.leave_card[i])
		{
			m_players_config.hand_cards[wChairID][i + m_card_config.init_card_num] = m_playing_para.leave_card[i];
			m_players_config.players[wChairID].hand_card_num++;
		}
	}
}

//获得一组卡牌的牌类型
int CGameData::GetCardLogicType(const BYTE *cbCardData, BYTE cbCardCount)
{
	//简单牌型
	switch (cbCardCount)
	{
	case 0:	//空牌
	{
		//cout << "0" << endl;
		return CT_ERROR;
	}
	case 1: //单牌
	{
		//cout << "1" << endl;
		return TransListToInt(0, CT_SINGLE);
	}
	case 2:	//对牌火箭
	{
		//cout << "2" << endl;
		//牌型判断
		if ((cbCardData[0] == 0x4F) && (cbCardData[1] == 0x4E)) return TransListToInt(0, CT_MISSILE_CARD);   //返回一个 1<<14
		if (GetCardLogicValueLaiZi(cbCardData[0]) == GetCardLogicValueLaiZi(cbCardData[1])) return TransListToInt(0, CT_DOUBLE);

		//参数定义
		BYTE LaiZiData[MAX_LAIZI_COUNT] = { 0 };	//癞子当普通牌的数据
		BYTE LaiZiNum = 0;					//癞子当普通牌的数量

											//清零
		SetLaiZiToCommon(LaiZiData, LaiZiNum);

		//赖子判断
		if (m_room_config.game_laizi_mode > 0)
		{
			if (GetCardLogicValueLaiZi(cbCardData[0]) != GetCardLogicValueLaiZi(cbCardData[1]))
			{
				if (GetCardLogicValueLaiZi(cbCardData[0]) == m_playing_para.magic_card || GetCardLogicValueLaiZi(cbCardData[1]) == m_playing_para.magic_card)
				{
					if ((cbCardData[0] != BIG_JOKER) && (cbCardData[0] != SMALL_JOKER) && (cbCardData[1] != BIG_JOKER) && (cbCardData[1] != SMALL_JOKER))
					{
						LaiZiNum++;

						if (GetCardLogicValueLaiZi(cbCardData[0]) == m_playing_para.magic_card)    LaiZiData[0] = GetCardLogicValue(cbCardData[1]);
						if (GetCardLogicValueLaiZi(cbCardData[1]) == m_playing_para.magic_card)    LaiZiData[0] = GetCardLogicValue(cbCardData[0]);

						cout << "对牌癞子：：：" << LaiZiData[0] << endl;
						//设置癞子做普通牌
						SetLaiZiToCommon(LaiZiData, LaiZiNum);
						return TransListToInt(0, CT_DOUBLE);
					}
				}
			}
		}

		//没有癞子 置零
		SetLaiZiToCommon(LaiZiData, LaiZiNum);

		//printf("对拍类型牌型判断错误\n");
		return CT_ERROR;
	}
	}

	//分析扑克
	tagAnalyseResult AnalyseResult;
	BYTE bLaiZiCount = GetMagicCardNum(cbCardData, cbCardCount);
	BYTE bCardDataTemp[MAX_CARD_COUNT];
	int dCardType = CT_ERROR;

	//参数定义
	BYTE LaiZiData[MAX_LAIZI_COUNT] = { 0 };	//癞子当普通牌的数据
	BYTE LaiZiNum = 0;					//癞子当普通牌的数量

										//清零
	SetLaiZiToCommon(LaiZiData, LaiZiNum);

	memset(bCardDataTemp, 0, sizeof(bCardDataTemp));
	CopyMemory(bCardDataTemp, cbCardData, sizeof(BYTE)*cbCardCount);
	ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));

	bool SameCard = false;

	//都是赖子做普通牌
	if (m_room_config.game_laizi_mode > 0)
	{
		for (BYTE i = 0; i < cbCardCount; i++)
		{
			if (GetCardLogicValueLaiZi(cbCardData[i]) == m_playing_para.magic_card)
				SameCard = true;
			else
			{
				SameCard = false;
				break;
			}
		}
	}

	//赖子判断
	if (bLaiZiCount != 0 && !SameCard && m_room_config.game_laizi_mode > 0)
	{
		for (BYTE i = 0;i<cbCardCount;i++)
		{
			if (GetCardLogicValueLaiZi(cbCardData[i]) == m_playing_para.magic_card)
			{
				cout << "==============一张癞子================" << endl;
				//一张赖子
				for (BYTE j = 0;j<13;j++)
				{
					bCardDataTemp[i] = m_cbCardData[j];

					//两张赖子
					if (bLaiZiCount >= 2)
					{
						for (BYTE k = i + 1;k<cbCardCount;k++)
						{
							if (GetCardLogicValueLaiZi(cbCardData[k]) == m_playing_para.magic_card)
							{
								cout << "==============两张癞子================" << endl;
								for (BYTE z = 0;z<13;z++)
								{
									bCardDataTemp[k] = m_cbCardData[z];

									//三张赖子
									if (bLaiZiCount >= 3)
									{
										for (BYTE g = k + 1;g<cbCardCount;g++)
										{
											if (GetCardLogicValueLaiZi(cbCardData[g]) == m_playing_para.magic_card)
											{
												cout << "==============三张癞子================" << endl;
												for (BYTE f = 0;f<13;f++)
												{
													bCardDataTemp[g] = m_cbCardData[f];

													//四张赖子
													if (bLaiZiCount == 4)
													{
														for (BYTE h = g + 1;h<cbCardCount;h++)
														{
															if (GetCardLogicValueLaiZi(cbCardData[h]) == m_playing_para.magic_card)
															{
																cout << "==============四张癞子================" << endl;
																for (BYTE l = 0;l<13;l++)
																{
																	if (l == 1)continue;
																	bCardDataTemp[h] = m_cbCardData[l];
																	ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));

																	AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResult);
																	if (GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData) == 0)
																		continue;

																	//记录第四张癞子
																	LaiZiData[0] = GetCardLogicValue(m_cbCardData[j]);
																	LaiZiData[1] = GetCardLogicValue(m_cbCardData[z]);
																	LaiZiData[2] = GetCardLogicValue(m_cbCardData[f]);
																	LaiZiData[3] = GetCardLogicValue(m_cbCardData[l]);

																	if ((dCardType&(1 << GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData))) == 0)
																		dCardType = TransListToInt(dCardType, GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData));

																	//目前策略 炸弹直接出
																	if (((1 << CT_RUAN_BOMB)&dCardType) != 0)
																		break;
																}
															}
															else
															{
																ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));

																AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResult);
																if (GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData) == 0)
																	continue;

																if ((dCardType&(1 << GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData))) == 0)
																	dCardType = TransListToInt(dCardType, GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData));
															}
														}
													}
													ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));

													AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResult);
													if (GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData) == 0)
														continue;

													//记录第三张癞子
													LaiZiData[0] = GetCardLogicValue(m_cbCardData[j]);
													LaiZiData[1] = GetCardLogicValue(m_cbCardData[z]);
													LaiZiData[2] = GetCardLogicValue(m_cbCardData[f]);

													if ((dCardType&(1 << GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData))) == 0)
														dCardType = TransListToInt(dCardType, GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData));

													//目前策略 炸弹直接出
													if (((1 << CT_RUAN_BOMB)&dCardType) != 0)
														break;
												}

												break;
											}
											else
											{
												ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));

												AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResult);
												if (GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData) == 0)
													continue;

												if ((dCardType&(1 << GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData))) == 0)
													dCardType = TransListToInt(dCardType, GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData));
											}
										}
									}
									ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));

									AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResult);
									if (GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData) == 0)
										continue;

									//记录第二张癞子
									cout << "TWO一癞子：：：" << (int)GetCardLogicValue(m_cbCardData[j]) << endl;
									cout << "TWO二癞子：：：" << (int)GetCardLogicValue(m_cbCardData[z]) << endl;
									LaiZiData[0] = GetCardLogicValue(m_cbCardData[j]);
									LaiZiData[1] = GetCardLogicValue(m_cbCardData[z]);

									if ((dCardType&(1 << GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData))) == 0)
										dCardType = TransListToInt(dCardType, GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData));

									//目前策略 炸弹直接出
									if (((1 << CT_RUAN_BOMB)&dCardType) != 0)
										break;
								}

								break;
							}
							else
							{
								ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));

								AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResult);
								if (GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData) == 0)
									continue;

								if ((dCardType&(1 << GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData))) == 0)
									dCardType = TransListToInt(dCardType, GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData));
							}
						}
					}
					ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));

					AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResult);
					if (GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData) == 0)
						continue;

					//记录第一张癞子
					cout << "ONE一癞子：：：" << (int)GetCardLogicValue(m_cbCardData[j]) << endl;
					LaiZiData[0] = GetCardLogicValue(m_cbCardData[j]);

					if ((dCardType&(1 << GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData))) == 0)
						dCardType = TransListToInt(dCardType, GetType(AnalyseResult, cbCardData, cbCardCount, cbCardData));

					//目前策略 炸弹直接出
					if (((1 << CT_RUAN_BOMB)&dCardType) != 0)
						break;
				}

				break;
			}
			else
			{
				ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));

				AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResult);
				if (GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData) == 0)
					continue;

				if ((dCardType&(1 << GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData))) == 0)
					dCardType = TransListToInt(dCardType, GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData));
			}
		}
	}
	else
	{
		ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));

		AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResult);

		if ((dCardType&(1 << GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData))) == 0)
			dCardType = TransListToInt(dCardType, GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData));
	}

	//获取数量
	for (int i = 0; i < MAX_LAIZI_COUNT; i++)
	{
		if (INVALID_CARD != LaiZiData[i])
		{
			cout << (int)LaiZiData[i];
			LaiZiNum++;
		}
	}
	cout << endl;

	//设置癞子做普通牌
	SetLaiZiToCommon(LaiZiData, LaiZiNum);

	cout << "LaiZiNum::" << (int)LaiZiNum << endl;

	//定时器出牌事件炸弹类型的判断
	if (dCardType&(1 << CT_BOMB_CARD) != 0)
	{
		int index = 0;
		for (int i = 0; i < cbCardCount; i++)
		{
			if (cbCardData[i] > 0x50)
				index++;
		}

		if (index > 0)   dCardType = (1 << CT_RUAN_BOMB);
		if (index == 4)  dCardType = (1 << CT_LAIZI_BOMB);
	}

	return dCardType;

}

//获得卡牌的逻辑花色
BYTE CGameData::GetCardLogicColor(BYTE cbCardData)
{
	//逻辑数值
	BYTE cbCardColor = GetCardColor(cbCardData);
	BYTE cbCardValue = GetCardValue(cbCardData);

	//花色判断
	if (cbCardColor == COLOR_MAIN_CARD) return COLOR_MAIN_CARD;
	if (cbCardColor == m_playing_para.main_color) return COLOR_MAIN_CARD;
	if (cbCardColor == m_playing_para.main_color) return COLOR_MAIN_CARD;

	//数值判断
	if (cbCardValue == m_playing_para.nt_value) return COLOR_MAIN_CARD;
	if (cbCardValue == m_playing_para.level_value) return COLOR_MAIN_CARD;

	return cbCardColor;
}

//获得卡牌数组的逻辑花色
BYTE CGameData::GetCardLogicColor(const BYTE *cbCardData, BYTE cbCardCount)
{
	//获取卡牌数组的头尾花色
	BYTE cbFirstColor = GetCardLogicColor(cbCardData[0]);
	BYTE cbSecondColor = GetCardLogicColor(cbCardData[cbCardCount - 1]);

	//头尾相等，则返回花色；不相等，返回错误
	return ((cbFirstColor == cbSecondColor) ? cbFirstColor : COLOR_ERROR);
}

//两张卡牌是否相连（第一张牌比第二张大）
bool CGameData::IsLineValue(BYTE cbTurnCard, BYTE cbSecondCard)
{
	bool bIsLine = false;

	//获取卡牌逻辑数值
	BYTE cbFirstLogicValue = GetCardLogicValue(cbTurnCard);
	BYTE cbSecondLogicValue = GetCardLogicValue(cbSecondCard);

	//卡牌真实值和花色
	BYTE bFirstValue = GetCardValue(cbTurnCard);
	BYTE bFirstColor = GetCardColor(cbTurnCard);
	BYTE bNextValue = GetCardValue(cbSecondCard);
	BYTE bNextColor = GetCardColor(cbSecondCard);

	//大小王
	if (bFirstValue == 0x0F && bNextValue == 0x0E) bIsLine = true;

	//跳过级牌，也可以是拖拉机		TODO 记得校验
	if ((m_playing_para.level_value != 2) &&
		(m_playing_para.level_value != 1) &&
		(m_playing_para.level_value != 14) &&
		(m_playing_para.level_value != 15))		//过滤掉最小最大值和大小王
	{
		if ((bFirstValue = m_playing_para.level_value + 1) &&
			(bNextValue = m_playing_para.level_value - 1))
		{
			bIsLine = true;
		}
	}

	//相连判断
	if (cbFirstLogicValue == (cbSecondLogicValue + 1)) bIsLine = true;

	//过滤：两对级牌，并且花色相连算拖拉机的情况
	if ((bFirstValue == m_playing_para.level_value) &&
		(bNextValue == m_playing_para.level_value) &&
		(bFirstColor == bNextColor + 1))
	{
		bIsLine = false;
	}

	return bIsLine;
}

//一轮比牌，判断赢家
WORD CGameData::GetOneRoundWinner()
{
	//初始赢家为第一个出牌玩家
	WORD wWinner = m_playing_para.start_outcard_user;
	WORD wFirstIndex = wWinner;
	BYTE cbCardType = GetCardLogicType(m_players_config.players[wWinner].out_card_info, m_players_config.players[wWinner].out_card_num);

	//对比扑克
	switch (cbCardType)
	{
	case CT_SINGLE:		//单牌类型
	case CT_SAME_2:		//对牌类型
	case CT_TRACKOR_2:	//拖拉机型
	case CT_TRACKOR_3:	//拖拉机型
	case CT_TRACKOR_4:	//拖拉机型
	case CT_TRACKOR_5:	//拖拉机型
	case CT_TRACKOR_6:	//拖拉机型
	case CT_TRACKOR_7:	//拖拉机型
	case CT_TRACKOR_8:	//拖拉机型
	case CT_TRACKOR_9:	//拖拉机型
	case CT_TRACKOR_10:	//拖拉机型
	case CT_TRACKOR_11:	//拖拉机型
	case CT_TRACKOR_12:	//拖拉机型
	case CT_TRACKOR_13:	//拖拉机型
	{
		//从开始出牌玩家开始，两两比较	
		wFirstIndex = GetNextUser(wFirstIndex);
		while (wFirstIndex != m_playing_para.start_outcard_user)
		{
			//校验牌型
			if (GetCardLogicType(m_players_config.players[wFirstIndex].out_card_info, m_players_config.players[wWinner].out_card_num) == cbCardType)
			{
				//比较大小
				if (CompareUserOutCard(m_players_config.players[wWinner].out_card_info[0], m_players_config.players[wFirstIndex].out_card_info[0]))
				{
					wWinner = wFirstIndex;
				}
			}

			wFirstIndex = GetNextUser(wFirstIndex);		//获得下家
		}

		return wWinner;
	}
	//case CT_THROW_CARD:	//甩牌类型
	//	{
	//		//花色判断
	//		if (GetCardLogicColor(cbOutCardData[wWinnerIndex][0])==COLOR_NT) return wFirstIndex;

	//		//分析牌型
	//		tagAnalyseResult WinnerResult;
	//		AnalyseCardData(cbOutCardData[wFirstIndex],cbCardCount,WinnerResult);

	//		//循环比较
	//		for (WORD i=0;i<GAME_PLAYER-1;i++)
	//		{
	//			//规则判断
	//			WORD wUserIndex=(wFirstIndex+i+1)%GAME_PLAYER;
	//			if (GetCardLogicColor(cbOutCardData[wUserIndex],cbCardCount)!=COLOR_NT) continue;

	//			//构造牌型
	//			tagAnalyseResult UserResult;
	//			if (RectifyCardWeave(cbOutCardData[wUserIndex],cbCardCount,WinnerResult,UserResult)==false) continue;

	//			//对比结果
	//			if (CompareCardResult(WinnerResult,UserResult)==true)
	//			{
	//				wWinnerIndex=wUserIndex;
	//				CopyMemory(&WinnerResult,&UserResult,sizeof(WinnerResult));
	//			}
	//		}

	//		return wWinnerIndex;
	//	}
	}

	return m_playing_para.start_outcard_user;
}

//计算一轮输赢得分
WORD CGameData::SettleOneRound(const WORD &wWinner)
{
	//分数存在庄家和庄家的下家身上,庄家是守分方，闲家是得分方
	WORD score = 0;

	//判断赢家是得分方还是守分方
	if ((wWinner == GetNextUser(m_playing_para.appointbanker)) ||
		(wWinner == GetLastUser(m_playing_para.appointbanker)))
	{
		//获得得分卡牌数组
		BYTE cbScoreCardNum = GetScoreCard();

		//获得总分
		score = GetAllCardScore(m_playing_para.score_card, cbScoreCardNum);

		//计算闲家得分，分数记在庄家的下家身上
		WORD wBankerNext = GetNextUser(m_playing_para.appointbanker);
		if (INVALID_CHAIR != wBankerNext)
		{
			//m_players_config.players[wBankerNext].single_score += score;      斗地主不存在 5 10 K
			//printf("\n【一轮结束，该轮得分 = %d，玩家 = %d 的总分 = %d】\n", score, wBankerNext, m_players_config.players[wBankerNext].single_score);
		}
	}

	//庄家赢不需要记分
	return score;
}

//获得一轮中的所有分牌
BYTE CGameData::GetScoreCard()
{
	//变量定义
	BYTE cbPostion = 0;

	//获得该轮所有玩家出牌的分牌
	for (int i = 0; i < m_room_config.max_chair_sum; i++)
	{
		if (USER_PLAYING == m_players_config.players[i].play_state)
		{
			//遍历玩家出牌数据
			for (BYTE j = 0; j < m_players_config.players[i].out_card_num; j++)
			{
				//获取数值
				BYTE cbCardValue = GetCardValue(m_players_config.players[i].out_card_info[j]);

				//累计扑克
				if ((cbCardValue == 5) || (cbCardValue == 10) || (cbCardValue == 13))
				{
					m_playing_para.score_card[cbPostion++] = m_players_config.players[i].out_card_info[j];
				}
			}
		}
	}

	return cbPostion;
}

//获取卡牌数组总分数
WORD CGameData::GetAllCardScore(const BYTE *cbCardData, BYTE cbCardCount)
{
	//变量定义
	WORD wCardScore = 0;

	//扑克累计
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		//获取数值
		BYTE cbCardValue = GetCardValue(cbCardData[i]);

		//累计积分
		if (cbCardValue == 5)
		{
			wCardScore += 5;
			continue;
		}

		//累计积分
		if ((cbCardValue == 10) || (cbCardValue == 13))
		{
			wCardScore += 10;
			continue;
		}
	}

	return wCardScore;
}

//比较玩家出牌大小，后面比前面大，返回true
bool CGameData::CompareUserOutCard(const BYTE &cbFirstCardData, const WORD &cbNextCardData)
{
	//获取花色
	BYTE cbLogicColorNext = GetCardLogicColor(cbNextCardData);
	BYTE cbLogicColorFirst = GetCardLogicColor(cbFirstCardData);

	//花色相同或都是主牌，则对比大小
	if ((cbLogicColorNext == COLOR_MAIN_CARD) || (cbLogicColorNext == cbLogicColorFirst))
	{
		//获取大小
		BYTE cbLogicValueNext = GetCardLogicValue(cbNextCardData);
		BYTE cbLogicValueFirst = GetCardLogicValue(cbFirstCardData);

		//大小判断
		if (cbLogicValueNext > cbLogicValueFirst)
			return true;
	}

	//花色不同，则肯定是第一个大

	return false;
}

//出牌校验    //将玩家要出的牌和该轮最大的出牌进行比较 可以出返回true
bool CGameData::EfficacyOutCard(const WORD &wCheckUser, const BYTE *cbOutCardData, const BYTE &cbOutCardCount)
{
	//获得该轮最大的出牌数据
	BYTE cbTurnCardNum = m_playing_para.turn_cards_num;
	BYTE cbTurnCard[MAX_CARD_COUNT];
	ZeroMemory(&cbTurnCard, sizeof(cbTurnCard));
	CopyMemory(cbTurnCard, m_playing_para.turn_max_cards, sizeof(cbTurnCard));

	printf("获得该轮最大的出牌数据:");
	for (int i = 0; i < cbTurnCardNum; i++)
	{
		printf("%d ", cbTurnCard[i]);
	}
	cout << endl;

	//获取类型
	int cbFirstType = GetCardLogicType(cbTurnCard, cbTurnCardNum);
	int cbNextType = GetCardLogicType(cbOutCardData, cbOutCardCount);

	cout << "cbFirstType:" << cbFirstType << endl;
	cout << "cbNextType:" << cbNextType << endl;


	//类型判断
	if (cbNextType == CT_ERROR) return false;
	if ((cbFirstType&(1 << CT_MISSILE_CARD)) != 0)  return false;
	if ((cbNextType&(1 << CT_MISSILE_CARD)) != 0) return true;

	//炸弹判断
	if ((cbFirstType&(1 << CT_LAIZI_BOMB)) != 0) { return false; }
	if ((cbNextType&(1 << CT_LAIZI_BOMB)) != 0) { return true; }
	if ((cbFirstType&(1 << CT_BOMB_CARD)) == 0 && (cbNextType&(1 << CT_BOMB_CARD)) != 0) { return true; }
	if ((cbFirstType&(1 << CT_BOMB_CARD)) != 0 && (cbNextType&(1 << CT_BOMB_CARD)) == 0) { return false; }
	if ((cbFirstType&(1 << CT_RUAN_BOMB)) != 0 && (cbNextType&(1 << CT_RUAN_BOMB)) == 0 && (cbNextType&(1 << CT_BOMB_CARD)) == 0) { return false; }
	if ((cbFirstType&(1 << CT_BOMB_CARD)) == 0 && (cbFirstType&(1 << CT_RUAN_BOMB)) == 0 && (cbNextType&(1 << CT_RUAN_BOMB)) != 0) { return true; }


	//规则判断
	if (((cbFirstType&cbNextType) == 0 || (cbTurnCardNum != cbOutCardCount)) && (cbFirstType&(1 << CT_BOMB_CARD)) == 0 && (cbFirstType&(1 << CT_RUAN_BOMB)) == 0 &&
		(cbNextType&(1 << CT_BOMB_CARD)) == 0 && (cbNextType&(1 << CT_RUAN_BOMB)) == 0) return false;

	//开始对比
	if ((cbNextType&(1 << CT_BOMB_CARD)) != 0)
	{

		//分析扑克
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;

		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_BOMB_CARD))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_BOMB_CARD))return false;

		//获取数值
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbFourCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbFourCardData[0]);

		//出牌最大值
		for (BYTE i = 0; i < FirstResult.cbFourCount; i++)
		{
			if (cbFirstLogicValue < GetCardLogicValue(NextResult.cbFourCardData[i]))
				cbFirstLogicValue = GetCardLogicValue(FirstResult.cbFourCardData[i]);
		}
		cout << endl;
		for (BYTE i = 0; i < NextResult.cbFourCount; i++)
		{
			cbNextLogicValue = GetCardLogicValue(NextResult.cbFourCardData[i]);


			if (cbNextLogicValue > cbFirstLogicValue)
				return true;
		}
	}
	else if ((cbNextType&(1 << CT_RUAN_BOMB)) != 0)
	{
		//分析扑克
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;

		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_RUAN_BOMB))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_RUAN_BOMB))return false;

		//获取数值
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbFourCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbFourCardData[0]);

		//出牌最大值
		for (BYTE i = 0; i < FirstResult.cbFourCount; i++)
		{
			if (cbFirstLogicValue < GetCardLogicValue(NextResult.cbFourCardData[i]))
				cbFirstLogicValue = GetCardLogicValue(FirstResult.cbFourCardData[i]);
		}
		for (BYTE i = 0; i < NextResult.cbFourCount; i++)
		{
			cbNextLogicValue = GetCardLogicValue(NextResult.cbFourCardData[i]);

			if (cbNextLogicValue > cbFirstLogicValue)
				return true;
		}
	}
	else if ((cbNextType&(1 << CT_SINGLE)) != 0 && cbTurnCardNum == cbOutCardCount)
	{
		//分析扑克
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;

		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_SINGLE))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_SINGLE))return false;

		//获取数值
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbSignedCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbSignedCardData[0]);

		//出牌最大值
		for (BYTE i = 0; i < FirstResult.cbSignedCount; i++)
		{
			if (cbFirstLogicValue < GetCardLogicValue(NextResult.cbSignedCardData[i]))
				cbFirstLogicValue = GetCardLogicValue(FirstResult.cbSignedCardData[i]);
		}
		for (BYTE i = 0; i < NextResult.cbSignedCount; i++)
		{
			cbNextLogicValue = GetCardLogicValue(NextResult.cbSignedCardData[i]);

			if (cbNextLogicValue > cbFirstLogicValue)
				return true;
		}
	}
	else if ((cbNextType&(1 << CT_DOUBLE)) != 0 && cbTurnCardNum == cbOutCardCount)
	{
		//分析扑克
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;

		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_DOUBLE))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_DOUBLE))return false;

		//获取数值
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbDoubleCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbDoubleCardData[0]);

		//出牌最大值
		for (BYTE i = 0; i < FirstResult.cbDoubleCount; i++)
		{
			if (cbFirstLogicValue < GetCardLogicValue(NextResult.cbDoubleCardData[i]))
				cbFirstLogicValue = GetCardLogicValue(FirstResult.cbDoubleCardData[i]);
		}
		for (BYTE i = 0; i < NextResult.cbDoubleCount; i++)
		{
			cbNextLogicValue = GetCardLogicValue(NextResult.cbDoubleCardData[i]);

			if (cbNextLogicValue > cbFirstLogicValue)
				return true;
		}
	}
	else if ((cbNextType&(1 << CT_THREE)) != 0 && cbTurnCardNum == cbOutCardCount)
	{
		//分析扑克
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;

		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_THREE))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_THREE))return false;

		//获取数值
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbThreeCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbThreeCardData[0]);

		//出牌最大值
		for (BYTE i = 0; i < FirstResult.cbThreeCount; i++)
		{
			if (cbFirstLogicValue < GetCardLogicValue(NextResult.cbThreeCardData[i]))
				cbFirstLogicValue = GetCardLogicValue(FirstResult.cbThreeCardData[i]);
		}
		for (BYTE i = 0; i < NextResult.cbThreeCount; i++)
		{
			cbNextLogicValue = GetCardLogicValue(NextResult.cbThreeCardData[i]);

			if (cbNextLogicValue > cbFirstLogicValue)
				return true;
		}
	}
	else if ((cbNextType&(1 << CT_SINGLE_LINE)) != 0 && cbTurnCardNum == cbOutCardCount)
	{
		//分析扑克
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;

		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_SINGLE_LINE))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_SINGLE_LINE))return false;

		//获取数值
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbSignedCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbSignedCardData[0]);

		//出牌最大值
		for (BYTE i = 0; i < FirstResult.cbSignedCount; i++)
		{
			if (cbFirstLogicValue < GetCardLogicValue(NextResult.cbSignedCardData[i]))
				cbFirstLogicValue = GetCardLogicValue(FirstResult.cbSignedCardData[i]);
		}
		for (BYTE i = 0; i < NextResult.cbSignedCount; i++)
		{
			cbNextLogicValue = GetCardLogicValue(NextResult.cbSignedCardData[i]);

			if (cbNextLogicValue > cbFirstLogicValue)
				return true;
		}
	}
	else if ((cbNextType&(1 << CT_DOUBLE_LINE)) != 0 && cbTurnCardNum == cbOutCardCount)
	{
		//分析扑克
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;

		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_DOUBLE_LINE))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_DOUBLE_LINE))return false;

		//获取数值
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbDoubleCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbDoubleCardData[0]);

		//出牌最大值
		for (BYTE i = 0; i < FirstResult.cbDoubleCount; i++)
		{
			if (cbFirstLogicValue < GetCardLogicValue(NextResult.cbDoubleCardData[i]))
				cbFirstLogicValue = GetCardLogicValue(FirstResult.cbDoubleCardData[i]);
		}
		for (BYTE i = 0; i < NextResult.cbDoubleCount; i++)
		{
			cbNextLogicValue = GetCardLogicValue(NextResult.cbDoubleCardData[i]);

			if (cbNextLogicValue > cbFirstLogicValue)
				return true;
		}
	}
	else if ((cbNextType&(1 << CT_THREE_LINE)) != 0 && cbTurnCardNum == cbOutCardCount)
	{
		//分析扑克
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;

		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_THREE_LINE))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_THREE_LINE))return false;

		//获取数值
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbThreeCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbThreeCardData[0]);

		//出牌最大值
		for (BYTE i = 0; i < FirstResult.cbThreeCount; i++)
		{
			if (cbFirstLogicValue < GetCardLogicValue(NextResult.cbThreeCardData[i]))
				cbFirstLogicValue = GetCardLogicValue(FirstResult.cbThreeCardData[i]);
		}
		for (BYTE i = 0; i < NextResult.cbThreeCount; i++)
		{
			cbNextLogicValue = GetCardLogicValue(NextResult.cbThreeCardData[i]);

			if (cbNextLogicValue > cbFirstLogicValue)
				return true;
		}
	}
	else if ((cbNextType&(1 << CT_THREE_LINE_TAKE_ONE)) != 0 && cbTurnCardNum == cbOutCardCount)
	{
		//分析扑克
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;

		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_THREE_LINE_TAKE_ONE))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_THREE_LINE_TAKE_ONE))return false;

		//获取数值
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbThreeCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbThreeCardData[0]);

		//出牌最大值
		for (BYTE i = 0; i < FirstResult.cbThreeCount; i++)
		{
			if (cbFirstLogicValue < GetCardLogicValue(NextResult.cbThreeCardData[i * 3]))
				cbFirstLogicValue = GetCardLogicValue(FirstResult.cbThreeCardData[i * 3]);
		}
		for (BYTE i = 0; i < NextResult.cbThreeCount; i++)
		{
			cbNextLogicValue = GetCardLogicValue(NextResult.cbThreeCardData[i * 3]);

			if (cbNextLogicValue > cbFirstLogicValue)
				return true;
		}
	}
	else if ((cbNextType&(1 << CT_THREE_LINE_TAKE_TWO)) != 0 && cbTurnCardNum == cbOutCardCount)
	{
		//分析扑克
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;

		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_THREE_LINE_TAKE_TWO))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_THREE_LINE_TAKE_TWO))return false;

		//获取数值
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbThreeCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbThreeCardData[0]);

		//出牌最大值
		for (BYTE i = 0; i < FirstResult.cbThreeCount; i++)
		{
			if (cbFirstLogicValue < GetCardLogicValue(NextResult.cbThreeCardData[i * 3]))
				cbFirstLogicValue = GetCardLogicValue(FirstResult.cbThreeCardData[i * 3]);
		}
		for (BYTE i = 0; i < NextResult.cbThreeCount; i++)
		{
			cbNextLogicValue = GetCardLogicValue(NextResult.cbThreeCardData[i * 3]);

			if (cbNextLogicValue > cbFirstLogicValue)
				return true;
		}
	}
	else if ((cbNextType&(1 << CT_FOUR_LINE_TAKE_ONE)) != 0 && cbTurnCardNum == cbOutCardCount)
	{
		//分析扑克
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;
		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_FOUR_LINE_TAKE_ONE))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_FOUR_LINE_TAKE_ONE))return false;

		//获取数值
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbFourCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbFourCardData[0]);

		//出牌最大值
		for (BYTE i = 0; i < FirstResult.cbFourCount; i++)
		{
			if (cbFirstLogicValue < GetCardLogicValue(NextResult.cbFourCardData[i * 4]))
				cbFirstLogicValue = GetCardLogicValue(FirstResult.cbFourCardData[i * 4]);
		}
		for (BYTE i = 0; i < NextResult.cbFourCount; i++)
		{
			cbNextLogicValue = GetCardLogicValue(NextResult.cbFourCardData[i * 4]);

			if (cbNextLogicValue > cbFirstLogicValue)
				return true;
		}
	}
	else if ((cbNextType&(1 << CT_FOUR_LINE_TAKE_TWO)) != 0 && cbTurnCardNum == cbOutCardCount)
	{
		//分析扑克
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;
		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_FOUR_LINE_TAKE_TWO))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_FOUR_LINE_TAKE_TWO))return false;

		//获取数值
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbFourCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbFourCardData[0]);

		//出牌最大值
		for (BYTE i = 0; i < FirstResult.cbFourCount; i++)
		{
			if (cbFirstLogicValue < GetCardLogicValue(NextResult.cbFourCardData[i * 4]))
				cbFirstLogicValue = GetCardLogicValue(FirstResult.cbFourCardData[i * 4]);
		}
		for (BYTE i = 0; i < NextResult.cbFourCount; i++)
		{
			cbNextLogicValue = GetCardLogicValue(NextResult.cbFourCardData[i * 4]);

			if (cbNextLogicValue > cbFirstLogicValue)
				return true;
		}
	}

	return false;
}

//根据指定花色提取扑克
BYTE CGameData::DistillCardByColor(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardColor, BYTE cbResultCard[])
{
	//变量定义
	BYTE cbResultCount = 0;

	//提取扑克
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		//花色判断
		if (GetCardLogicColor(cbCardData[i]) == cbCardColor)
		{
			cbResultCard[cbResultCount++] = cbCardData[i];
			continue;
		}

		//终止判断
		if (cbResultCount > 0) break;
	}

	return cbResultCount;
}

//根据数目提取扑克
BYTE CGameData::DistillCardByCount(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbSameCount, tagSameDataInfo &SameDataInfo)
{
	//参数效验
	if ((cbSameCount < 2) || (cbSameCount > MAX_PACK)) return 0;

	//设置变量
	ZeroMemory(&SameDataInfo, sizeof(SameDataInfo));

	//提取判断
	if (cbCardCount < cbSameCount) return 0;

	//提取扑克
	for (BYTE i = 0; i < cbCardCount - cbSameCount + 1; i++)
	{
		//数目判断
		BYTE j = 0;
		for (j = 1; j < cbSameCount; j++)
		{
			if (cbCardData[i] != cbCardData[i + j]) break;
		}

		//成功处理
		if (j == cbSameCount)
		{
			//同牌属性
			SameDataInfo.cbBlockCount++;

			//扑克数据
			for (BYTE k = 0; k < cbSameCount; k++)
			{
				SameDataInfo.cbSameData[SameDataInfo.cbCardCount++] = cbCardData[i + k];
			}
		}

		//递增处理
		i += j - 1;
	}

	return SameDataInfo.cbBlockCount;
}

//提取拖拉机扑克
BYTE CGameData::DistillTrackorByCount(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbSameCount, tagTractorDataInfo &TractorDataInfo)
{
	//参数效验
	if ((cbSameCount < 2) || (cbSameCount > MAX_PACK)) return 0;

	//设置变量
	ZeroMemory(&TractorDataInfo, sizeof(TractorDataInfo));

	//提取同牌
	tagSameDataInfo SameDataInfo;
	DistillCardByCount(cbCardData, cbCardCount, cbSameCount, SameDataInfo);

	//牌段判断
	if (SameDataInfo.cbBlockCount < 2) return 0;

	//分析处理
	BYTE cbResultIndex = 0;
	for (BYTE i = 0; i < SameDataInfo.cbBlockCount - 1; i++)
	{
		//分析过滤
		if (SameDataInfo.cbSameData[i*cbSameCount] == 0) continue;

		//变量定义
		BYTE cbLineCount = 1;
		BYTE cbTrackorIndex[MAX_TRACKOR] = { i*cbSameCount };

		//连牌判断
		for (BYTE j = i; j < SameDataInfo.cbBlockCount - 1; j++)
		{
			//分析过滤
			if (SameDataInfo.cbSameData[(j + 1)*cbSameCount] == 0) continue;

			//变量定义
			BYTE cbSecondIndex = (j + 1)*cbSameCount;
			BYTE cbFirstIndex = cbTrackorIndex[cbLineCount - 1];
			BYTE cbFirstCardData = SameDataInfo.cbSameData[cbFirstIndex];
			BYTE cbSecondCardData = SameDataInfo.cbSameData[cbSecondIndex];

			//连牌判断
			if (IsLineValue(cbFirstCardData, cbSecondCardData) == true)
			{
				cbTrackorIndex[cbLineCount++] = cbSecondIndex;
				continue;
			}

			//副主间隔
			if (cbFirstCardData == cbSecondCardData) continue;
			if (GetCardValue(cbSecondCardData) == m_playing_para.nt_value) continue;
			if (GetCardValue(cbSecondCardData) == m_playing_para.level_value) continue;

			break;
		}

		//成功判断
		if (cbLineCount >= 2)
		{
			//最大长度
			if (cbLineCount > TractorDataInfo.cbTractorMaxLength)
			{
				TractorDataInfo.cbTractorMaxLength = cbLineCount;
				TractorDataInfo.cbTractorMaxIndex = cbResultIndex;
			}

			//设置结果
			TractorDataInfo.cbTractorCount++;
			TractorDataInfo.cbCardCount += cbLineCount * cbSameCount;
			TractorDataInfo.cbTractorData[cbResultIndex++] = cbLineCount;
			TractorDataInfo.cbTractorData[cbResultIndex++] = cbSameCount;

			//设置数据
			for (BYTE k = 0; k < cbLineCount; k++)
			{
				BYTE cbIndex = cbTrackorIndex[k];
				for (BYTE l = 0; l < cbSameCount; l++)
				{
					TractorDataInfo.cbTractorData[cbResultIndex++] = SameDataInfo.cbSameData[cbIndex + l];
					SameDataInfo.cbSameData[cbIndex + l] = 0;
				}
			}
		}
	}

	return TractorDataInfo.cbTractorCount;
}

BYTE CGameData::GetIntersectionCount(const BYTE cbCardData1[], BYTE cbCardCount1, const BYTE cbCardData2[], BYTE cbCardCount2)
{
	//变量定义
	BYTE cbAnalyseCard[MAX_CARD_COUNT];
	CopyMemory(cbAnalyseCard, cbCardData2, sizeof(BYTE)*cbCardCount2);

	//分析判断
	BYTE cbIntersectionCount = 0;
	for (BYTE i = 0; i < cbCardCount1; i++)
	{
		for (BYTE j = 0; j < cbCardCount2; j++)
		{
			if (cbCardData1[i] == cbAnalyseCard[j])
			{
				//设置变量
				cbAnalyseCard[j] = 0;
				cbIntersectionCount++;

				break;
			}
		}
	}

	return cbIntersectionCount;
}

//根据一轮的开始出牌玩家，分析玩家可以出的牌
bool CGameData::AnalyseOutCardNotify(const WORD &wChairID, const BYTE *OutCardData,
	const BYTE &cbOutCardNum, tagOutCardNotify *cbOutCardNotify)
{


	return true;

}

//判断用户手牌中是否有指定花色的该类型的卡牌
bool CGameData::IsHaveSpecificType(const WORD &wChairID, const BYTE &color, const BYTE &cbOutCardType,
	const BYTE &cbOutCardNum, tagOutCardNotify &cbOutCardNotify)
{
	//校验
	if ((CT_ERROR == cbOutCardType) || (0 == cbOutCardNum))
		return false;

	//出牌数目
	cbOutCardNotify.cbOutCardNum = cbOutCardType;

	//判断手牌中是否有该花色的牌
	BYTE cbHandCardNum = GetUserCurCardNum(wChairID);
	bool bHaveColor = false;

	//玩家手牌中指定花色的卡牌数组
	std::vector<BYTE> vecColorCard;

	//玩家手牌中除了指定花色的卡牌数据
	std::vector<BYTE> vecNotColorCard;

	//遍历手牌
	for (int i = 0; i < cbHandCardNum; i++)
	{
		BYTE cbCardColor = GetCardLogicColor(m_players_config.hand_cards[wChairID][i]);
		if (color == cbCardColor)
		{
			bHaveColor = true;
			vecColorCard.push_back(m_players_config.hand_cards[wChairID][i]);
		}
		else
		{
			vecNotColorCard.push_back(m_players_config.hand_cards[wChairID][i]);
		}
	}

	//若有该花色，再判断类型
	if (bHaveColor)
	{
		//手牌中该花色的卡牌数目
		BYTE cbColorNum = vecColorCard.size();

		//单牌类型
		if ((CT_SINGLE == cbOutCardType) && (1 == cbOutCardNum))
		{
			//能出的牌为该花色全部的牌
			for (int i = 0; i < vecColorCard.size(); i++)
			{
				cbOutCardNotify.cbMustOutCard[i] = vecColorCard[i];
			}
			return true;
		}

		//对子类型
		if ((CT_SAME_2 == cbOutCardType) && (2 == cbOutCardNum))
		{
			//该花色卡牌不足
			if (cbColorNum < cbOutCardNum)
			{
				//必须出的卡牌为：该花色的牌
				for (int i = 0; i < cbColorNum; i++)
				{
					cbOutCardNotify.cbMustOutCard[i] = vecColorCard[i];
				}

				//可选出的卡牌为：其他花色卡牌
				for (int i = 0; i < vecNotColorCard.size(); i++)
				{
					cbOutCardNotify.cbOptionalCard[i] = vecNotColorCard[i];
				}
			}
			else		//卡牌足够，则判断手牌对子
			{
				//获得玩家该花色的所有对子
				int nPairNum = 0;
				for (int i = 0; i < cbColorNum; i += 2)
				{
					int nSameNum = count(vecColorCard.begin(), vecColorCard.end(), vecColorCard[i]);
					if (2 == nSameNum)	//有对子，只能出对子
					{
						cbOutCardNotify.cbMustOutCard[nPairNum++] = vecColorCard[i];
						cbOutCardNotify.cbMustOutCard[nPairNum++] = vecColorCard[i];
					}
				}

				//没对子，必须出该花色的其他牌
				if (0 == nPairNum)
				{
					//能出的牌为该花色全部的牌
					for (int i = 0; i < cbColorNum; i++)
					{
						cbOutCardNotify.cbMustOutCard[i] = vecColorCard[i];
					}
				}
			}

			return true;
		}

		//拖拉机类型（连对）
		if ((CT_TRACKOR_2 <= cbOutCardType) && (CT_TRACKOR_13 >= cbOutCardType))
		{
			//获得玩家该花色的所有对子
			std::vector<BYTE> vecPair;
			for (int i = 0; i < cbColorNum; i += 2)
			{
				int nSameNum = count(vecColorCard.begin(), vecColorCard.end(), vecColorCard[i]);
				if (2 == nSameNum)	//有对子，只能出对子
				{
					vecPair.push_back(vecColorCard[i]);
					vecPair.push_back(vecColorCard[i]);
				}
			}
			//对子数组大小
			int nPairNum = vecPair.size();

			//该花色卡牌不足
			if (cbColorNum < cbOutCardNum)
			{
				//有对子出对子
				int nIndex = 0;
				for (int i = 0; i < nPairNum; i++)
				{
					cbOutCardNotify.cbMustOutCard[nIndex++] = vecPair[i];
				}

				//出完对子，再出完该花色的牌 ，最后再出任意卡牌
				for (int i = 0; i < cbColorNum; i++)
				{
					//过滤对子中的卡牌
					if (vecPair.end() == find(vecPair.begin(), vecPair.end(), vecColorCard[i]))
					{
						cbOutCardNotify.cbMustOutCard[nIndex++] = vecColorCard[i];
					}
				}

				//再出任意卡牌
				for (int i = 0; i < vecNotColorCard.size(); i++)
				{
					cbOutCardNotify.cbOptionalCard[i] = vecNotColorCard[i];
				}
			}
			else		//卡牌足够，则判断手牌对子
			{
				//没对子，必须出该花色的其他牌
				if (0 == nPairNum)
				{
					//能出的牌为该花色全部的牌
					for (int i = 0; i < cbColorNum; i++)
					{
						cbOutCardNotify.cbMustOutCard[i] = vecColorCard[i];
					}
				}
				else	//有对子，先判断有没有拖拉机，再判断对子是否足够
				{
					//有足够多的对子
					if (nPairNum >= cbOutCardNum)
					{
						//判断是不是拖拉机
						BYTE *cbTmpPair = new BYTE[nPairNum];
						for (int i = 0; i < nPairNum; i++)
							cbTmpPair[i] = vecPair[i];

						//拖拉机所需的对子数
						BYTE cbTrackorPair = cbOutCardNum / 2;

						//循环遍历所有对子,判断有没有拖拉机
						int nStartIndex = 0;
						int nEndIndex = nStartIndex + cbTrackorPair * 2 - 1;
						bool bHaveTrackor = false;
						while (nEndIndex < nPairNum)
						{
							//判断是不是拖拉机						
							if (cbOutCardType == GetCardLogicType(cbTmpPair + nStartIndex, cbOutCardNum))
							{
								bHaveTrackor = true;
								//获得必须出牌的数据
								for (int i = nStartIndex; i < nEndIndex; i++)
								{
									cbOutCardNotify.cbMustOutCard[i] = cbTmpPair[i];
								}
							}
							//后移两张牌的位置
							nStartIndex += 2;
							nEndIndex += 2;
						}

						//没有拖拉机,则必须出对子
						if (!bHaveTrackor)
						{
							for (int i = 0; i < nPairNum; i++)
							{
								cbOutCardNotify.cbMustOutCard[i] = vecPair[i];
							}
						}

						//释放内存
						delete cbTmpPair;
						return true;
					}
					else	//没有足够的对子，出对子 + 该花色的单牌
					{
						//有对子出对子
						int nIndex = 0;
						for (int i = 0; i < nPairNum; i++)
						{
							cbOutCardNotify.cbMustOutCard[nIndex++] = vecPair[i];
						}

						//出完对子，再出完该花色的牌 
						nIndex = 0;
						for (int i = 0; i < cbColorNum; i++)
						{
							//过滤对子中的卡牌
							if (vecPair.end() == find(vecPair.begin(), vecPair.end(), vecColorCard[i]))
							{
								cbOutCardNotify.cbMustOutCard[nIndex++] = vecColorCard[i];
							}
						}

					}
				}
			}

			return true;
		}
	}
	else	//没该花色，出什么都可以，用户手牌都可以出
	{
		//获得手牌
		for (int i = 0; i < cbHandCardNum; i++)
		{
			cbOutCardNotify.cbOptionalCard[i] = m_players_config.hand_cards[wChairID][i];
		}
	}

	return true;
}

/******************************************************************************/
/*********************************  查询接口实现  *******************************/

/**
 * 将服务器端的牌转换到客户端，查询用户牌组
 * @param			wChairID			[in]		椅子编号
 * @param			cards			[out]		返回的牌组缓冲区
 * @param			cardsum			[in-out]	返回的缓冲区长度，按BYTE算
 */
int __stdcall CGameData::GetClientHandCards(WORD wChairID, BYTE *cards, const int &cardsum)
{
	//校验
	if (NULL == cards || 0 == cardsum)
	{
		return -1;
	}

	//卡牌转换
	for (int i = 0; i < cardsum; ++i)
	{
		cards[i] = m_players_config.hand_cards[wChairID][i];
	}

	return 0;
}

// 设置当前游戏类型ID
int __stdcall CGameData::SetKindID(DWORD id)
{
	m_room_config.kind_id = id;
	return 0;
}

// 查询当前游戏类型ID
DWORD __stdcall CGameData::GetKindID()
{
	return m_room_config.kind_id;
}

////查询每个人拿到的最大牌数
//DWORD __stdcall CGameData::GetGameMaxCardNum()
//{
//	return m_card_config.game_cards_sum;
//}

// 查询玩家小局得分
SCORE __stdcall CGameData::GetPlayerSingleScore(WORD wChairID)
{
	return m_players_config.players[wChairID].single_score;
}

// 设置玩家房间内总得分
DWORD __stdcall CGameData::SetPlayerTotalScore(WORD wChairID, SCORE score)
{
	m_players_config.players[wChairID].total_score += score;
	return 0;
}

// 查询玩家房间内总得分
SCORE __stdcall CGameData::GetPlayerTotalScore(WORD wChairID)
{
	return m_players_config.players[wChairID].total_score;
}

// 查询玩家游戏中状态-准备状态
BYTE __stdcall CGameData::GetXjReadyState(WORD wChairID)
{
	return m_players_config.players[wChairID].xj_ready_state;

}

// 设置玩家游戏中状态-准备
int __stdcall CGameData::SetXjReadyState(WORD wChairID, BYTE ready)
{
	m_players_config.players[wChairID].xj_ready_state = ready;
	return 0;
}

//记录玩家庄家次数  大局结算
int __stdcall CGameData::SetPlayerBankCount(WORD wChairID)
{
	m_players_config.players[wChairID].bank_count++;
	return 0;
}

//获取玩家庄家次数  
int __stdcall CGameData::GetPlayerBankCount(WORD wChairID)
{
	return m_players_config.players[wChairID].bank_count;
}

// 设置玩家赢局数
int __stdcall CGameData::SetWinSum(WORD wChairID, BYTE winsum)
{
	m_players_config.players[wChairID].winsum += winsum;
	return 0;
}

// 查询玩家赢局数
int __stdcall CGameData::GetWinSum(WORD wChairID, BYTE &winsum)
{
	winsum = m_players_config.players[wChairID].winsum;
	return 0;
}

// 查询当前局数
BYTE __stdcall CGameData::GetCurGameCount()
{
	return m_playing_para.game_cur_count;
}

// 设置当前局数
int __stdcall CGameData::SetGameCount(BYTE count)
{
	m_playing_para.game_cur_count = count;
	return 0;
}

// 查询总局数
BYTE __stdcall CGameData::GetAllCount()
{
	return m_room_config.game_count;
}

// 设置总局数
int __stdcall CGameData::SetAllCount(BYTE count)
{
	//校验
	if (count < 1)		//测试用 设置固定局数
	{
		count = 2;
	}

	printf("总局数：%d\n", count);

	m_room_config.game_count = count;
	return 0;
}

// 设置房间当前倍数
int __stdcall CGameData::SetCurRoomBet(SCORE room_bet)
{
	m_playing_para.room_bet *= room_bet;

	return 0;
}

// 查询房间当前倍数
SCORE __stdcall CGameData::GetCurRoomBet()
{
	return m_playing_para.room_bet;
}

// 查询房间底分
DWORD __stdcall CGameData::GetCellScore()
{
	return m_room_config.room_cell;
}

// 设置下注底分
int __stdcall CGameData::SetCellScore(DWORD score)
{
	//校验
	if (score < ROOMRULE_MIN_CELL_SCORE)
	{
		score = ROOMRULE_MIN_CELL_SCORE;
	}

	printf("打印房间底分：%d\n", score);

	m_room_config.room_cell = score;
	return 0;
}

// 查询玩家下注完成标志
BYTE __stdcall CGameData::GetAddScoreState(WORD wChairID)
{
	return m_players_config.players[wChairID].bet_state;
}

// 设置玩家下注完成标志		0-下注未完成		1-下注完成
int __stdcall CGameData::SetAddScoreState(WORD wChairID, BYTE beted)
{
	m_players_config.players[wChairID].bet_state = beted;
	return 0;
}

// 查询玩家明牌完成标志
BYTE __stdcall CGameData::GetMingPaiState(WORD wChairID)
{
	return m_players_config.players[wChairID].mingpai_state;
}

// 设置玩家明牌完成标志		0-明牌未完成		1-明牌完成
int __stdcall CGameData::SetMingPaiState(WORD wChairID, BYTE state)
{
	m_players_config.players[wChairID].mingpai_state = state;
	return 0;
}

// 查询玩家下注完成标志
BYTE __stdcall CGameData::GetUserOperateState(WORD wChairID)
{
	return m_players_config.players[wChairID].action_state;
}

// 设置玩家下注完成标志		0-下注未完成		1-下注完成
int __stdcall CGameData::SetUserOperateState(WORD wChairID, BYTE state)
{
	m_players_config.players[wChairID].action_state = state;
	return 0;
}

// 获取玩家总分
DWORD __stdcall CGameData::GetAllBet(WORD wChairID)
{
	return  m_players_config.players[wChairID].bet;
}

// 查询椅子总数
BYTE __stdcall CGameData::GetMaxChairCount()
{
	return m_room_config.max_chair_sum;
}

// 设置椅子总数
int __stdcall CGameData::SetMaxChairCount(BYTE count)
{
	//框架传来的最大椅子数不能大于此数
	if (MAX_CHAIR_COUNT < count || count < 1)
	{
		count = MAX_CHAIR_COUNT;
	}
	m_players_config.player_sum = count;
	m_room_config.max_chair_sum = count;
	return 0;
}

// 设置一轮中玩家是出牌还是过
int __stdcall CGameData::SetTurnOutCardType(const WORD wChairID, DWORD type)
{
	m_playing_para.out_card_type[wChairID] = type;
	return 0;
}

// 获取一轮中玩家是出牌还是过
DWORD __stdcall CGameData::GetTurnOutCardType(const WORD wChairID)
{
	return m_playing_para.out_card_type[wChairID];
}

//设置游戏得分模式
int __stdcall CGameData::SetGameScoreMode(BYTE nMode)
{
	//校验
	if (GAME_SCORE_MODE_CLASSIC != nMode && GAME_SCORE_MODE_CRAZY != nMode)
	{
		nMode = GAME_SCORE_MODE_CLASSIC;
	}
	m_room_config.game_score_mode = nMode;
	return 0;
}

//获得游戏得分模式
BYTE __stdcall CGameData::GetGameScoreMode()
{
	return m_room_config.game_score_mode;
}

//设置游戏抢庄模式：0 - 抢庄		1 - 叫三分
int __stdcall CGameData::SetRobBankMode(BYTE nMode)
{
	//固定

	if (ROOMRULE_OPTION_ROBTYPE_FREE > nMode || ROOMRULE_OPTION_ROBTYPE_JIAOSANFEN < nMode)
	{
		nMode = ROOMRULE_OPTION_ROBTYPE_FREE;
	}
	m_room_config.game_robBank_mode = nMode;
	return 0;
}

//获取抢庄模式
BYTE __stdcall CGameData::GetRobBankMode()
{
	return m_room_config.game_robBank_mode;
}

// 设置当前抢庄次数
int __stdcall CGameData::SetCurBankerCount(BYTE count)
{
	m_playing_para.rob_count = count;
	return 0;
}

// 获取当前抢庄次数
BYTE __stdcall CGameData::GetCurBankerCount()
{
	return m_playing_para.rob_count;
}

//设置玩家抢庄分数
int __stdcall CGameData::SetUserRobScore(const WORD &wChairID, BYTE score)
{
	m_players_config.players[wChairID].rob_score = score;
	return 0;
}

//设置玩家抢庄次数
int __stdcall CGameData::SetUserRobNum(const WORD &wChairID, BYTE cbNum)
{
	m_players_config.players[wChairID].rob_num = cbNum;
	return 0;
}

//获得玩家抢庄次数
BYTE __stdcall CGameData::GetUserRobNum(const WORD &wChairID)
{
	return m_players_config.players[wChairID].rob_num;
}

// 查询当前玩家数(只要坐下就是玩家)
//TODONOW added by WangChengQing 2017.9.22 增加判断 -- 如果游戏在进行中. 若client发送坐下请求, server应该提示 "必须等到该局结束后 才可以坐下"  此判断应该在坐下函数中响应
BYTE __stdcall CGameData::GetCurPlayerCount()
{
	// 计算游戏人数
	BYTE cbPlayerNum = 0;

	for (int i = 0; i < m_room_config.max_chair_sum; ++i)
	{
		if (USER_PLAYING == GetPlayerState(i))
		{
			cbPlayerNum++;
		}
	}

	return cbPlayerNum;
}

// 查询当前游客数
BYTE __stdcall CGameData::GetCurLookerCount()
{
	BYTE curLookerCount = 0;
	for (BYTE i = 0; i < GetMaxChairCount(); i++)
	{
		if (USER_STANDUP == GetPlayerState(i))	//根据椅子的状态来判断
		{
			curLookerCount++;
		}
	}
	return curLookerCount;
}

// 设置当前庄家
int __stdcall CGameData::SetBankerID(const WORD &wChairID)
{
	m_playing_para.appointbanker = wChairID;
	return 0;
}

// 查询当前庄家
WORD __stdcall CGameData::GetBankerID()
{
	return m_playing_para.appointbanker;
}


// 获得玩家的下家 -- 逆时针获取
WORD __stdcall CGameData::GetNextUser(const WORD &wChariID)
{
	if (INVALID_CHAIR == wChariID)
	{
		return INVALID_CHAIR;
	}
	//寻找下家
	WORD wNextChairID = INVALID_CHAIR;
	int nMaxChairCount = m_room_config.max_chair_sum;

	wNextChairID = (wChariID + 1) % nMaxChairCount;

	//下家在游戏中
	if (USER_PLAYING == m_players_config.players[wNextChairID].play_state)
	{
		return wNextChairID;
	}
	else
		return GetNextUser(wNextChairID); //如果失败,继续获取下一个
}

// 获取上个玩家  顺时针获取
WORD __stdcall CGameData::GetLastUser(const WORD &wChariID)
{
	if (INVALID_CHAIR == wChariID)
	{
		return INVALID_CHAIR;
	}
	//寻找上家
	WORD wLastChairID = INVALID_CHAIR;
	int nMaxChairCount = m_room_config.max_chair_sum;
	wLastChairID = (wChariID + nMaxChairCount - 1) % nMaxChairCount;

	//下家在游戏中
	if (USER_PLAYING == m_players_config.players[wLastChairID].play_state)
	{
		return wLastChairID;
	}
	else
		return GetLastUser(wLastChairID); //如果失败,继续获取下一个
}

// 获得对家
WORD __stdcall CGameData::GetOppositeUser(const WORD &wChariID)
{
	if ((INVALID_CHAIR == wChariID) ||
		(m_room_config.max_chair_sum != 4))
	{
		return INVALID_CHAIR;
	}
	//寻找对家
	WORD wOpposite = INVALID_CHAIR;
	int nMaxChairCount = m_room_config.max_chair_sum;

	wOpposite = (wChariID + 2) % nMaxChairCount;

	//对家在游戏中
	if (USER_PLAYING == m_players_config.players[wOpposite].play_state)
	{
		return wOpposite;
	}

	return INVALID_CHAIR;
}

// 设置用户状态，从服务器框架获得，用户坐下、准备、旁观、游戏中
int __stdcall CGameData::SetPlayerState(WORD wChairID, DWORD state)
{
	if (wChairID == INVALID_CHAIR)
	{
		return -1;
	}
	m_players_config.players[wChairID].play_state = state;
	return 0;
}

// 查询用户状态，用户坐下、准备、旁观、游戏中
DWORD __stdcall CGameData::GetPlayerState(WORD wChairID)
{
	return m_players_config.players[wChairID].play_state;
}

// 设置游戏状态
int __stdcall CGameData::SetGameStatus(DWORD state)
{
	m_playing_para.game_state = state;
	return 0;
}

// 查询游戏状态
DWORD __stdcall CGameData::GetGameStatus()
{
	return m_playing_para.game_state;
}

// 查询游戏中状态-玩家摊牌
BYTE __stdcall CGameData::GetShowCardState(WORD wChairID)
{
	return m_players_config.players[wChairID].showcard_state;

}

// 设置游戏中状态-玩家摊牌		0-未摊牌	1-摊牌
int __stdcall CGameData::SetShowCardState(WORD wChairID, BYTE state)
{
	m_players_config.players[wChairID].showcard_state = state;
	return 0;
}

/*************************************************
@Description:		设置房间规则，从服务器框架获得数据：游戏id、总局数、
					最大椅子数、下注底分、抢庄模式、得分模式、房主
@Input:				pData-客户端传来的结构体数据
					wDataSize-数据大小
@Output:          无
@Return:          无
@author & data:	lizhihu 2017.11.2
*************************************************/
bool __stdcall CGameData::SetRoomRule(tagTableCfg * pRoomRuleOption)
{
	if (NULL == pRoomRuleOption)
	{
		return false;
	}

	//初始化为0
	ZeroMemory(&m_room_config, sizeof(m_room_config));
	ZeroMemory(&m_players_config, sizeof(m_players_config));
	ZeroMemory(&m_card_config, sizeof(m_card_config));
	ZeroMemory(&m_playing_para, sizeof(m_playing_para));

	//设置游戏类型ID
	SetKindID(KIND_ID);

	//设置局数
	SetAllCount(pRoomRuleOption->com_rule->GameCount);

	//设置椅子和玩家数目
	SetMaxChairCount(pRoomRuleOption->com_rule->PlayerCount);

	// 设置下注底分
	SetCellScore(1);

	//设置游戏得分模式，经典模式或不洗牌   ---------子游戏规则待设定
	SetGameScoreMode(0);

	//设置游戏抢庄模式：0 - 抢庄		1 - 叫三分
	SetRobBankMode(0);

	//设置房主
	//SetRoomFangzhu(pRoomRuleOption->com_rule->FangZhu);

	m_card_config.leave_card_num = NORMAL_LEAVE_CARD_NUM;
	m_card_config.init_card_num = NORMAL_HAND_CARD_NUM;

	//设置封顶倍数
	SetRoomMaxBet(0);

	//设置游戏癞子模式  0-经典  1-癞子  2-天地癞子
	SetLaiZiMode(0);

	//设置底牌翻倍   0-不翻倍  1-翻倍
	SetISLeaveCardDouble(0);

	//设置是否明牌   0-不明牌   1-明牌
	SetMingPaiMode(0);

	//设置是否加倍   0-不加倍   1-加倍
	SetAddBetMode(0);

	return true;
}

//设置房主
int __stdcall CGameData::SetRoomFangzhu(WORD nFangzhu_id)
{
	//校验
	if (nFangzhu_id < 0 || nFangzhu_id > m_players_config.player_sum)
	{
		nFangzhu_id = 0;
	}
	m_room_config.fangzhu_id = nFangzhu_id;
	return 0;
}

//获得房主椅子ID
WORD __stdcall CGameData::GetRoomFangzhu()
{
	return m_room_config.fangzhu_id;
}

//得到一个用户初始卡牌数目
int __stdcall CGameData::GetPlayerInitCardNum()
{
	return m_card_config.init_card_num;
}

//设置用户的初始卡牌数目
int __stdcall CGameData::SetPlayerInitCardNum()
{
	//根据人数，设置玩家初始手牌数
	m_card_config.init_card_num = (m_card_config.game_cards_sum - m_card_config.leave_card_num) / m_players_config.player_sum;

	//测试用 17张牌
	m_card_config.init_card_num = 17;

	return 0;
}

//获得上局赢家椅子号
WORD __stdcall CGameData::GetLastGameWinner()
{
	return m_playing_para.last_winner_id;
}

//获得上局赢家椅子号
int __stdcall CGameData::SetLastGameWinner(const WORD &wChairID)
{
	m_playing_para.last_winner_id = wChairID;
	return 0;
}

//设置升级主花色
int __stdcall CGameData::SetMainColor(const BYTE &color)
{
	m_playing_para.main_color = color;
	return 0;
}

//获取升级主花色
BYTE __stdcall CGameData::GetMainColor()
{
	return m_playing_para.main_color;
}

//设置亮主玩家
int __stdcall CGameData::SetLiangZhuUser(const WORD &wChairID)
{
	m_playing_para.liangzhu_user = wChairID;
	return 0;
}

//获取亮主玩家
WORD __stdcall CGameData::GetLiangZhuUser()
{
	return m_playing_para.liangzhu_user;
}

//设置当前级牌
int __stdcall CGameData::SetLevelValue(const BYTE &value)
{
	m_playing_para.level_value = value;
	return 0;
}

//获取当前级牌
BYTE __stdcall CGameData::GetLevelValue()
{
	return m_playing_para.level_value;
}

//获得底牌
void __stdcall CGameData::GetLeaveCard(BYTE* cbLeaveCard, int nLeaveCardSize)
{
	//校验
	if ((NULL == cbLeaveCard) || (nLeaveCardSize != MAX_LEAVE_CARD_NUM))
	{
		return;
	}

	//获得底牌
	memcpy(cbLeaveCard, m_playing_para.leave_card, sizeof(BYTE)* MAX_LEAVE_CARD_NUM);
}

//获得玩家当前卡牌数目
BYTE __stdcall CGameData::GetUserCurCardNum(const WORD &wChairID)
{
	return m_players_config.players[wChairID].hand_card_num;
}

//获得玩家当手牌中的主牌个数
BYTE __stdcall CGameData::GetUserMainColorNum(const WORD &wChairID)
{
	//主花色个数
	BYTE cbMainCount = 0;

	//遍历手牌
	for (int i = 0; i < m_players_config.players[wChairID].hand_card_num; i++)
	{
		BYTE cbCardData = m_players_config.hand_cards[wChairID][i];

		if (COLOR_MAIN_CARD == GetCardLogicColor(cbCardData))
			cbMainCount++;
	}

	return cbMainCount;
}

// 设置当前出牌用户
int __stdcall CGameData::SetCurOutCardUser(const WORD &wChairID)
{
	m_playing_para.cur_outcard_user = wChairID;
	return 0;
}

// 获得当前出牌用户
WORD __stdcall CGameData::GetCurOutCardUser()
{
	return m_playing_para.cur_outcard_user;
}

// 设置一轮中最先开始出牌的用户
int __stdcall CGameData::SetStartOutCardUser(const WORD &wChairID)
{
	m_playing_para.start_outcard_user = wChairID;

	//清空上轮的出牌数据
	for (int i = 0; i < m_room_config.max_chair_sum; i++)
	{
		m_players_config.players[i].out_card_num = 0;
		ZeroMemory(m_players_config.players[i].out_card_info, sizeof(BYTE)*MAX_CARD_COUNT);
	}

	return 0;
}

// 获得一轮中最先开始出牌的用户
WORD __stdcall CGameData::GetStartOutCardUser()
{
	return m_playing_para.start_outcard_user;
}


// 设置一轮中最后出牌的用户
int __stdcall CGameData::SetLastOutCardUser(const WORD &wChairID)
{
	m_playing_para.last_outcard_user = wChairID;

	return 0;
}

// 获得一轮中最后出牌的用户
WORD __stdcall CGameData::GetLastOutCardUser()
{
	return m_playing_para.last_outcard_user;
}

// 设置玩家出牌卡牌数据
bool __stdcall CGameData::SetUserOutCard(const WORD wChairID, const BYTE *cbCardData, const BYTE &cbCardNum)
{
	//校验
	if ((NULL == cbCardData) ||
		(0 == cbCardNum))
		return false;

	//出牌数据
	memcpy(m_players_config.players[wChairID].out_card_info, cbCardData, sizeof(BYTE)*cbCardNum);
	//出牌个数
	m_players_config.players[wChairID].out_card_num = cbCardNum;

	return true;
}

// 获得玩家出牌卡牌数据
BYTE __stdcall CGameData::GetUserOutCard(const WORD wChairID, BYTE *cbCardData)
{
	BYTE cbCardNum = m_players_config.players[wChairID].out_card_num;

	//校验
	if (NULL == cbCardData)
		return cbCardNum;

	//出牌数据
	memcpy(cbCardData, m_players_config.players[wChairID].out_card_info, sizeof(BYTE)*cbCardNum);

	return cbCardNum;
}

// 获得底牌数目
BYTE __stdcall CGameData::GetLeaveCardNum()
{
	return m_card_config.leave_card_num;
}

// 设置当前反主卡牌
int __stdcall CGameData::SetCurFanZhuCard(const BYTE &card)
{
	m_playing_para.cur_fanzhu_card = card;
	return 0;
}

// 获得玩家当前级数
int __stdcall CGameData::GetUserCurLevel(const WORD &wChairID)
{
	return m_players_config.players[wChairID].level;
}

// 设置当前反主玩家
int __stdcall CGameData::SetCurFanzhuUser(const WORD &wChairID)
{
	m_playing_para.cur_fanzhu_user = wChairID;
	return 0;
}

// 获得当前反主玩家
WORD __stdcall CGameData::GetCurFanzhuUser()
{
	return m_playing_para.cur_fanzhu_user;
}

// 设置当前扣底玩家
int __stdcall CGameData::SetCurKoudiUser(const WORD &wChairID)
{
	m_playing_para.cur_koudi_user = wChairID;
	return 0;
}

// 获得当前扣底玩家
WORD __stdcall CGameData::GetCurKoudiUser()
{
	return m_playing_para.cur_koudi_user;
}

// 设置最开始抢庄玩家
int __stdcall CGameData::SetStartRobUser(const WORD &wChairID)
{
	m_playing_para.start_rob_user = wChairID;
	return 0;
}

// 获得最开始抢庄玩家
WORD __stdcall CGameData::GetStartRobUser()
{
	return m_playing_para.start_rob_user;
}

// 设置一轮结束
int __stdcall CGameData::SetOneTurnEnd(const BYTE &cbEnd)
{
	m_playing_para.turn_end = cbEnd;
	return 0;
}

// 获得一轮结束
BYTE __stdcall CGameData::GetOneTurnEnd()
{
	return m_playing_para.turn_end;
}

// 设置一轮中最大卡牌数据
int __stdcall CGameData::SetTurnMaxCards(BYTE *cbCardData, BYTE cbCardNum)
{
	//先清除保存的数据
	ZeroMemory(&m_playing_para.turn_max_cards, sizeof(m_playing_para.turn_max_cards));

	//卡牌个数
	m_playing_para.turn_cards_num = cbCardNum;

	//赋值
	for (int i = 0; i < cbCardNum && i < MAX_CARD_COUNT; i++)
	{
		m_playing_para.turn_max_cards[i] = cbCardData[i];
	}
	return 0;
}

// 获得一轮中最大卡牌数据
BYTE __stdcall CGameData::GetTurnMaxCards(BYTE *cbCardData, BYTE &cbCardNum)
{
	//校验
	if (NULL == cbCardData)
		return 0;

	//赋值
	cbCardNum = m_playing_para.turn_cards_num;
	for (int i = 0; i < cbCardNum; i++)
	{
		cbCardData[i] = m_playing_para.turn_max_cards[i];
	}
}

// 获得赖子数目
BYTE __stdcall CGameData::GetMagicCardNum(const BYTE cbHandCardData[], BYTE cbHandCardCount)
{
	BYTE bLaiZiCount = 0;
	for (BYTE i = 0; i < cbHandCardCount; i++)
	{
		if (cbHandCardData[i] == INVALID_CARD)
			continue;

		if (GetCardLogicValue(cbHandCardData[i]) == m_playing_para.magic_card)
			bLaiZiCount++;
	}

	return bLaiZiCount;
}

// 获得玩家手牌赖子数目
BYTE __stdcall CGameData::GetMagicCardNum(const WORD &wChairID)
{
	//玩家手牌
	BYTE cbHandCardData[MAX_CARD_COUNT];
	memcpy(cbHandCardData, m_players_config.hand_cards[wChairID], sizeof(cbHandCardData));

	return GetMagicCardNum(cbHandCardData, m_players_config.players[wChairID].hand_card_num);
}

//设置当前癞子
int __stdcall CGameData::SetMagicCard(const BYTE &cbCard)
{
	m_playing_para.magic_card = ((cbCard >= 14) ? (cbCard - 13) : cbCard) + 0x50;   //癞子逻辑值有这么大
	return 0;
}

//获得当前癞子
BYTE __stdcall CGameData::GetMagicCard()
{
	return m_playing_para.magic_card;
}

//设置玩家炸弹信息
int __stdcall CGameData::SetUserBoomInfo(const WORD &wChairID, const DWORD cbBoomType)
{
	cout << wChairID << ":炸弹类型:" << (int)cbBoomType << endl;
	//校验
	if (((1 << CT_RUAN_BOMB) > cbBoomType) || ((1 << CT_MISSILE_CARD) < cbBoomType))
		cout << "Error!!!" << endl;

	// 设置当前房间倍数并记录各玩家倍数
	if (((cbBoomType&(1 << CT_MISSILE_CARD)) != 0))    //王炸
	{
		for (int i = 0; i < m_room_config.max_chair_sum; i++)      //把炸弹倍数添加上
		{
			if (USER_PLAYING == GetPlayerState(i))
			{
				if (0 < GetLaiZiMode())
					m_players_config.players[i].bet *= HARD_BOOM_TIMES;
				else
					m_players_config.players[i].bet *= RUAN_BOOM_TIMES;
			}

			if (m_players_config.players[i].bet > m_playing_para.max_room_bet)   //判断是否封顶
				m_players_config.players[i].bet = m_playing_para.max_room_bet;
		}

		//赋值
		BYTE &cbBoomNum = m_players_config.players[wChairID].boom_info.cbBoomNum;
		m_players_config.players[wChairID].boom_info.cbBoomType[cbBoomNum++] = cbBoomType;
	}
	else if (((cbBoomType & (1 << CT_BOMB_CARD)) != 0))    //硬炸
	{
		for (int i = 0; i < m_room_config.max_chair_sum; i++)      //把炸弹倍数添加上
		{
			if (USER_PLAYING == GetPlayerState(i))
			{
				if (0 < GetLaiZiMode())
					m_players_config.players[i].bet *= HARD_BOOM_TIMES;
				else
					m_players_config.players[i].bet *= RUAN_BOOM_TIMES;
			}

			if (m_players_config.players[i].bet > m_playing_para.max_room_bet)   //判断是否封顶
				m_players_config.players[i].bet = m_playing_para.max_room_bet;
		}

		//赋值
		BYTE &cbBoomNum = m_players_config.players[wChairID].boom_info.cbBoomNum;
		m_players_config.players[wChairID].boom_info.cbBoomType[cbBoomNum++] = cbBoomType;
	}
	else if (((cbBoomType & (1 << CT_RUAN_BOMB)) != 0))    //软炸
	{
		for (int i = 0; i < m_room_config.max_chair_sum; i++)      //把炸弹倍数添加上
		{
			if (USER_PLAYING == GetPlayerState(i) && 0 < GetLaiZiMode())
			{
				m_players_config.players[i].bet *= RUAN_BOOM_TIMES;
			}

			if (m_players_config.players[i].bet > m_playing_para.max_room_bet)   //判断是否封顶
				m_players_config.players[i].bet = m_playing_para.max_room_bet;
		}

		//赋值
		BYTE &cbBoomNum = m_players_config.players[wChairID].boom_info.cbBoomNum;
		m_players_config.players[wChairID].boom_info.cbBoomType[cbBoomNum++] = cbBoomType;
	}
	else if (((cbBoomType & (1 << CT_LAIZI_BOMB)) != 0))    //癞子炸弹
	{
		for (int i = 0; i < m_room_config.max_chair_sum; i++)      //把炸弹倍数添加上
		{
			if (USER_PLAYING == GetPlayerState(i) && 0 < GetLaiZiMode())
			{
				m_players_config.players[i].bet *= HARD_BOOM_TIMES;
			}

			if (m_players_config.players[i].bet > m_playing_para.max_room_bet)   //判断是否封顶
				m_players_config.players[i].bet = m_playing_para.max_room_bet;
		}

		//赋值
		BYTE &cbBoomNum = m_players_config.players[wChairID].boom_info.cbBoomNum;
		m_players_config.players[wChairID].boom_info.cbBoomType[cbBoomNum++] = cbBoomType;
	}

	return 0;
}

//获得玩家炸弹信息
BYTE __stdcall CGameData::GetUserBoomInfo(const WORD &wChairID, tagBoomInfo &strBoomInfo)
{
	//赋值
	strBoomInfo.cbBoomNum = m_players_config.players[wChairID].boom_info.cbBoomNum;
	for (int i = 0; i < MAX_BOOM_NUM; i++)
	{
		strBoomInfo.cbBoomType[i] = m_players_config.players[wChairID].boom_info.cbBoomType[i];
	}

	return strBoomInfo.cbBoomNum;
}

/******************************************************************************/
/*******************************  辅助函数  *******************************/
/******************************************************************************/
// 根据游戏配置创建卡牌数据
int CGameData::LoadGameCards()
{
	int result = 0;

	//根据全局配置文件变量，赋值给成员变量
	if ((NULL != CGameConfig::gComCardPara.group) &&
		(0 != CGameConfig::gComCardPara.groupNum) &&
		(0 != CGameConfig::gComCardPara.game_cards_num))
	{
		//实际卡牌总数
		m_card_config.game_cards_sum = CGameConfig::gComCardPara.game_cards_num;

		//游戏内卡牌
		m_card_config.game_cards = new CARD_DESC[m_card_config.game_cards_sum];
		for (int i = 0; i < m_card_config.game_cards_sum; i++)
		{
			m_card_config.game_cards[i] = CGameConfig::gComCardPara.game_cards[i];
		}

		//玩家初始手牌数
		SetPlayerInitCardNum();
	}
	else
	{
		cout << "配置的全局变量ERR" << endl;
	}

	return result;
}

// 根据游戏配置创建玩家数据
int CGameData::LoadPlayers()
{
	int result = 0;

	PLAYER_TYPE _play_type = 0;		//一个word存储了玩家的2个数据card_sum，player_sum

	//从全局配置文件中读取玩家类型
	_play_type = CGameConfig::gComPlayerPara.type;

	// 创建所有玩家，玩家数目固定为最大椅子数，从服务器框架获得
	m_players_config.players = new PLAYER_DESC[m_players_config.player_sum];
	memset(m_players_config.players, 0, sizeof(PLAYER_DESC)*m_players_config.player_sum);

	CGamePlayerConfig::CreatePlayerDesc(m_players_config.players, m_players_config.player_sum, _play_type);

	return result;
}

// 配置游戏规则
int  CGameData::LoadGameRule(const BYTE cbGameScoreMode)
{
	//或得分倍数
	if (cbGameScoreMode == GAME_SCORE_MODE_CLASSIC)	//经典模式
	{
		memcpy(&m_room_config.game_score_times, &CGameConfig::gComGamePara.game_normal_times, sizeof(m_room_config.game_score_times));
	}
	else if (cbGameScoreMode == GAME_SCORE_MODE_CRAZY)	//疯狂模式
	{
		memcpy(&m_room_config.game_score_times, &CGameConfig::gComGamePara.game_crazy_times, sizeof(m_room_config.game_score_times));
	}

	return 0;
}

// 输出牌局信息
int CGameData::OutGameInfor()
{
	/*TODOLATER
	//m_plog->LogSingleLine(L"庄家： %d", m_room_config.appointbanker ); // 输出庄家ID

	// 输出玩家手牌
	for ( unsigned i = 0 ;i < GetCurPlayerCount() ; ++i)
	{
		wchar_t **_str = new wchar_t* [__GETCARDSUM( m_players_config.players[i].type ) + 1]();
		_str[0] = new wchar_t[64]();
		swprintf_s(_str[0], 64,  L"%s\t%s\t%s\n", L"点数", L"花色", L"组ID");
		for ( int j = 1 ;j < __GETCARDSUM( m_players_config.players[i].type ) + 1 ; ++j )
		{
			// 填充缓冲区
			_str[j] = new wchar_t[64]();
			swprintf_s(_str[j], 64,  L"%d\t%d\t%d\n", __GETSCORE( m_players_config.hand_cards[i][j - 1]),  __GETCOLOR( m_players_config.hand_cards[i][j - 1]), __GETGROUPID( m_players_config.hand_cards[i][j - 1] ));
		}
		int *a = NULL;
		// 写日志
		//m_plog->LogMutilLine(_str, a, __GETCARDSUM( m_players_config.players[i].type ) + 1);

		// 回收
		for ( int j = 0; j < __GETCARDSUM( m_players_config.players[i].type ) + 1; ++ j )
		{
			delete [] _str[j];
			_str[j] = NULL;
		}

		delete [] _str;
		_str = NULL;
	}

	// 输出玩家得分
	int nPlayerNum = GetCurPlayerCount() + 1;
	wchar_t **_str = new wchar_t* [nPlayerNum]();
	_str[0] = new wchar_t[64]();
	swprintf_s(_str[0], 64,  L"%s\t%s\t%s\t%s\t%s\t%s\t%s\n", L"点数", L"下注", L"牌型", L"公牌数", L"单局分", L"总得分", L"倍数", L"玩家Type");
	for( unsigned i = 1; i < nPlayerNum; ++i)
	{
		_str[i] = new wchar_t[64]();
		swprintf_s(_str[i], 64, L"%d\t%d\t%d\t%d\t%d\n",
								m_players_config.players[i - 1].bet,
								m_players_config.players[i - 1].card_type,
								m_players_config.players[i - 1].single_score,
								m_players_config.players[i - 1].total_score,
								m_players_config.players[i - 1].times,
								m_players_config.players[i - 1].type);
	}

	// 写日志
	int *a = NULL;
	//m_plog->LogMutilLine(_str, a, nPlayerNum);

	for ( unsigned j = 0; j < nPlayerNum; ++ j )
	{
		delete [] _str[j];
		_str[j] = NULL;
	}

	delete [] _str;
	_str = NULL;

	//m_plog->LogSingleLine(L"========================== 华丽的分割线 ==========================", NULL);

	return 0;
*/
	return 0;
}

// 初始化游戏数据
int CGameData::InitGameData()
{
	//m_playing_para.appointbanker = 0;		//当前庄家

	//进入房间时，积分为0
	for (unsigned i = 0; i < m_players_config.player_sum; ++i)
	{
		m_players_config.players[i].total_score = 0;
		m_players_config.players[i].winsum = 0;
		m_players_config.players[i].level = START_LEVEL_VALUE;
		m_players_config.players[i].bet = 1;   //初始加倍明牌倍数为1
		m_players_config.players[i].bank_count = 0;
		m_players_config.players[i].rob_num = 0;
		m_players_config.players[i].best_bet = 0;  //最大倍数初始化为0
		m_players_config.players[i].addscore_bet = 1;
		m_players_config.players[i].mingpai_bet = 1;
		m_players_config.players[i].start_mingpai = 1;
		m_players_config.players[i].mingpai_state = 0;
		m_players_config.players[i].jiaofen_state = 10;
		m_players_config.players[i].spring = 0;
		m_players_config.players[i].spring_state = 0;
		m_players_config.players[i].player_tuo_guan = 0;
		m_players_config.players[i].out_time_num = 0;
	}

	//初始化房间倍数
	m_playing_para.room_bet = 1;
	m_playing_para.turn_cards_num = 0;   //最大出牌数据初始化
	m_playing_para.jiaofen_count = 0;	 //初始化叫分次数
	m_playing_para.leave_card_bet = 1;
	m_playing_para.game_cur_count = 0;

	//排序权位初始化
	for (BYTE i = 0; i < LEN_SORT_RIGHT; i++)
		m_playing_para.sort_right[i] = i * COLOR_RIGHT;

	//主花色和级牌初始化
	m_playing_para.level_value = START_LEVEL_VALUE;
	m_playing_para.main_color = COLOR_ERROR;
	m_playing_para.appointbanker = INVALID_CHAIR;
	m_playing_para.liangzhu_user = INVALID_CHAIR;
	m_playing_para.nt_value = VALUE_ERROR;
	m_playing_para.cur_fanzhu_user = INVALID_CHAIR;
	m_playing_para.cur_koudi_user = INVALID_CHAIR;
	m_playing_para.start_rob_user = INVALID_CHAIR;

	return 0;
}


// 服务器到客户端卡牌转换
int CGameData::CardTrans_S2C(const CARD_DESC &card_s, BYTE &card_c)
{
	//获得牌型和花色，不要牌组id
	card_c = card_s & 0xff;		//取低八位

	return 0;
}

// 客户端到服务器卡牌转换
int CGameData::CardTrans_C2S(CARD_DESC &card_s, const BYTE &card_c)
{
	int result = 0;
	BYTE _score = (card_c & 0x0F);
	BYTE _color = ((card_c & 0xF0) >> 4);

	CARD_DESC _tmp_card = (_score + (_color << 8));

	for (unsigned i = 0; i < m_card_config.game_cards_sum; ++i)
	{
		if ((m_card_config.game_cards[i] & 0x0FFF) == _tmp_card)
		{
			card_s = m_card_config.game_cards[i];
			break;
		}
		else if ((m_card_config.game_cards_sum - 1) == i)
		{
			//m_plog->LogSingleLine(L"卡牌解析失败: %d", card_c );
			result = -1;
		}
	}

	return result;

}

//牌组转换	(将32张牌的使用情况转化为一个32位整型)
int CGameData::TransListToInt(int iOldType, int iNewType)
{
	//数据校验
	if (iNewType == 0) return 0;

	//变量定义
	int nList = 0;
	nList |= (1 << iNewType);

	//cout << "转化的一个32位整型: " <<nList << endl;

	return iOldType | nList;
}

//测试胡牌的函数--七对
void CGameData::SetCardData_Leo(const WORD &wChairID, CARD_DESC *cards)
{
	BYTE cardtmp[108] = { 0x4f,0x4f,0x22,0x32,0x31,0x31,0x3d,0x3d,0x3c,0x3c,0x3b,0x3b,0x3a,0x3a,0x2c,0x2b,0x29,0x0d,0x17,0x0c,0x0b,0x0b,0x0a,0x09,0x07,
					   0x4e,0x22,0x12,0x12,0x2a,0x29,0x28,0x25,0x24,0x39,0x39,0x37,0x36,0x35,0x34,0x33,0x33,0x1b,0x1a,0x19,0x18,0x17,0x16,0x1d,0x14,
					   0x4e,0x32,0x02,0x2b,0x2a,0x26,0x23,0x23,0x38,0x38,0x37,0x35,0x11,0x1c,0x1b,0x18,0x0c,0x08,0x08,0x07,0x06,0x05,0x05,0x04,0x03,
					   0x21,0x21,0x2d,0x2d,0x2c,0x28,0x27,0x26,0x25,0x24,0x34,0x1d,0x1c,0x1a,0x19,0x16,0x15,0x15,0x13,0x01,0x0d,0x0a,0x06,0x04,0x03,
					   0x13,0x02,0x11,0x14,0x36,0x27,0x09,0x01 };

	//赋值
	for (int i = 0; i < 108; i++)
	{
		cards[i] = cardtmp[i];
	}
}

//扑克索引转换成数据
BYTE __stdcall CGameData::SwitchToCardData(BYTE cbCardIndex)
{
	if (cbCardIndex == SMALL_JOKER_INDEX)
	{
		return SMALL_JOKER;
	}
	else if (cbCardIndex == BIG_JOKER_INDEX)
	{
		return BIG_JOKER;
	}
	else
		return ((cbCardIndex / 13) << 4) | (cbCardIndex % 13 + 1);
}

//扑克数据转换成索引 cardIndex:（0-12方 13-25梅花 26-38红桃 39-51黑桃 65-66小王大王）
BYTE __stdcall CGameData::SwitchToCardIndex(BYTE cbCardData)
{
	BYTE card = ((cbCardData&MASK_COLOR) >> 4) * 13 + (cbCardData&MASK_VALUE) - 1;
	return card;	//高四位花色，低四位点数
}

//扑克索引数据转换数据数组
BYTE __stdcall CGameData::SwitchToCardData(const BYTE cbCardIndex[LEN_MAX_INDEX], BYTE cbCardData[MAX_CARD_COUNT])
{
	//转换扑克
	BYTE cbPosition = 0;

	for (BYTE i = 0; i < LEN_MAX_INDEX; i++)
	{
		if (cbCardIndex[i] != 0)
		{
			for (BYTE j = 0; j < cbCardIndex[i]; j++)
			{
				cbCardData[cbPosition++] = SwitchToCardData(i);
			}
		}
	}

	return cbPosition;
}

//扑克数据数组转换成索引数组
BYTE __stdcall CGameData::SwitchToCardIndex(const BYTE *cbCardData, BYTE cbCardCount, BYTE *cbCardIndex)
{
	//设置变量
	ZeroMemory(cbCardIndex, sizeof(BYTE)*LEN_MAX_INDEX);

	//转换扑克
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}

//删除扑克
bool __stdcall CGameData::RemoveCard(const BYTE *cbRemoveCard, BYTE cbRemoveCount, BYTE *cbCardData, BYTE cbCardCount)
{
	//检验数据
	if (cbRemoveCount > cbCardCount)
	{
		cout << "检验数据失败" << endl;
		return false;

	}

	//定义变量
	BYTE cbDeleteCount = 0;
	BYTE cbTempCardData[MAX_CARD_COUNT];
	if (cbCardCount > MAX_CARD_COUNT)
	{
		cout << "if (cbCardCount > MAX_CARD_COUNT)" << endl;
		return false;
	}
	CopyMemory(cbTempCardData, cbCardData, MAX_CARD_COUNT * sizeof(cbCardData[0]));

	//将需要删除的扑克置零
	for (BYTE i = 0; i < cbRemoveCount; i++)
	{
		for (BYTE j = 0; j < cbCardCount; j++)
		{
			if (cbRemoveCard[i] == cbTempCardData[j])
			{
				cbDeleteCount++;
				cbTempCardData[j] = 0;
				break;
			}
		}
	}
	if (cbDeleteCount != cbRemoveCount)
	{
		printf("cbDeleteCount != cbRemoveCount\n");
		return false;
	}

	//重新赋值
	BYTE cbCardPos = 0;
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		if (cbTempCardData[i] != 0)
			cbCardData[cbCardPos++] = cbTempCardData[i];
	}

	//去除后面没删除的数据
	for (int i = cbCardPos; i < cbCardCount; i++)
	{
		cbCardData[i] = 0;
	}

	return true;
}

//从玩家手牌中删除扑克
bool __stdcall CGameData::RemoveCard(const WORD &wChairID, const BYTE *cbRemoveCard, BYTE cbRemoveCount)
{
	//过滤无效卡牌
	for (int i = 0; i < cbRemoveCount; i++)
	{
		if (INVALID_CARD == cbRemoveCard[i])
		{
			cbRemoveCount--;
		}
	}

	//玩家当前手牌数
	BYTE cbCardCount = GetUserCurCardNum(wChairID);
	//删除卡牌
	if (RemoveCard(cbRemoveCard, cbRemoveCount, m_players_config.hand_cards[wChairID], cbCardCount))
	{
		m_players_config.players[wChairID].hand_card_num -= cbRemoveCount;
		return true;
	}
	return false;
}


//设置当前抢庄的玩家
int __stdcall CGameData::SetCurRobUser(const WORD wChairID)
{
	m_playing_para.currobuser = wChairID;
	return 0;
}

//获取当前抢庄的玩家
WORD __stdcall CGameData::GetCurRobUser()
{
	return m_playing_para.currobuser;
}

//记录玩家下注
int __stdcall CGameData::SetPlayerAddScore(WORD wChairID, WORD lScore)
{
	if (lScore < 1)
	{
		lScore = 1;
	}

	m_players_config.players[wChairID].addscore_bet = lScore;
	return 0;
}

//获取玩家下注
WORD __stdcall CGameData::GetPlayerAddScore(WORD wChairID)
{
	return m_players_config.players[wChairID].addscore_bet;
}

//记录玩家明牌倍数
int __stdcall CGameData::SetPlayerMingPaiBet(WORD wChairID, WORD bet)
{
	if (bet < 1)
	{
		bet = 1;
	}

	m_players_config.players[wChairID].mingpai_bet = bet;
	return 0;
}


//获取玩家明牌倍数
int __stdcall CGameData::GetPlayerMingPaiBet(WORD wChairID)
{
	return m_players_config.players[wChairID].mingpai_bet;
}


//设置一大局中玩家获得过的最大倍数
int __stdcall CGameData::SetDJPlayerBestBet(WORD wChairID, SCORE bet)
{
	m_players_config.players[wChairID].best_bet = bet;
	return 0;
}

//获得一大局中玩家获得过的最大倍数
SCORE __stdcall CGameData::GetDJPlayerBestBet(WORD wChairID)
{
	return  m_players_config.players[wChairID].best_bet;
}

//设置游戏封顶倍数
int __stdcall CGameData::SetRoomMaxBet(DWORD Max_Bet)
{
	if (Max_Bet == 0)   //不封顶
	{
		m_playing_para.max_room_bet = (1 << 30);
		return 0;
	}

	if (Max_Bet < 32)
		Max_Bet = 32;

	m_playing_para.max_room_bet = Max_Bet;

	return 0;
}

//获取游戏封顶倍数
DWORD __stdcall CGameData::GetRoomMaxBet()
{

	return m_playing_para.max_room_bet;
}

//记录玩家是否开始游戏明牌  5-开始游戏明牌  4-发牌明牌
int __stdcall CGameData::SetStartMingPai(WORD wChairID, DWORD state)
{
	m_players_config.players[wChairID].start_mingpai = state;
	return 0;
}

//获取玩家是否开始游戏明牌
DWORD __stdcall CGameData::GetStartMingPai(WORD wChairID)
{
	return m_players_config.players[wChairID].start_mingpai;
}

//设置一个参数记录叫分次数 
int __stdcall CGameData::SetJiaoFenCount(WORD count)
{
	m_playing_para.jiaofen_count = count;
	return 0;
}

//获得叫分次数 
WORD __stdcall CGameData::GetJiaoFenCount()
{
	return m_playing_para.jiaofen_count;
}

//记录当前玩家的叫分状态
int __stdcall CGameData::SetCurJiaoFenState(WORD wChairID, WORD state)
{
	m_players_config.players[wChairID].jiaofen_state = state;
	return 0;
}

//获得当前玩家的叫分状态
int __stdcall CGameData::GetCurJiaoFenState(WORD wChairID)
{
	return m_players_config.players[wChairID].jiaofen_state;
}

//设置当前正在叫分的玩家     
int __stdcall CGameData::SetCurJiaoFenUser(WORD wChairID)
{
	m_playing_para.cur_jiaofen_player = wChairID;
	return 0;
}

//获取当前正在叫分的玩家     
WORD __stdcall CGameData::GetCurJiaoFenUser()
{
	return m_playing_para.cur_jiaofen_player;
}

//清除各玩家的个人倍数
WORD __stdcall CGameData::CleanAllBet()
{
	for (int i = 0; i < GetCurPlayerCount(); i++)
	{
		m_players_config.players[i].bet = 1;
	}
	return 0;
}

//获取不抢庄的轮数
WORD __stdcall CGameData::GetTurnPassCount()
{
	return m_playing_para.turn_pass_count;
}

//设置不抢庄的轮数
int __stdcall CGameData::SetTurnPassCount(WORD count)
{
	m_playing_para.turn_pass_count = count;
	return 0;
}

//设置游戏癞子模式
int __stdcall CGameData::SetLaiZiMode(WORD Mode)
{
	if (Mode > 2 || Mode < 0)
	{
		Mode = 0;
	}

	m_room_config.game_laizi_mode = 0;
	return 0;
}

//获取游戏癞子模式
int __stdcall CGameData::GetLaiZiMode()
{
	return m_room_config.game_laizi_mode;
}

//初始化当前轮手牌给机器人
void __stdcall CGameData::GetTurnMaxCardToRobot(GameSituation &gst, AILogic &m_AILogic)
{
	memset(&(gst.uctNowCardGroup), 0, sizeof(gst.uctNowCardGroup));

	//获取当前轮的卡牌类型  二进制
	DWORD m_type = GetCardLogicType(m_playing_para.turn_max_cards, m_playing_para.turn_cards_num);

	//赋值
	gst.NowOutUser = m_playing_para.cur_outcard_user;   //当前出牌用户
	gst.nCardDroit = m_playing_para.last_outcard_user;    //最后出牌用户
	gst.nDiZhuID = m_playing_para.appointbanker;			//庄家

	//定义
	int  nValue = 0;								//该牌的价值
	int  nCount = m_playing_para.turn_cards_num;		//含牌的个数
	int  nMaxCard = 0;								//牌中决定大小的牌值，用于对比

	//分析比较获取决定牌值的最大的那张牌并给gst赋值
	if ((1 << CT_SINGLE) == m_type)
	{
		gst.uctNowCardGroup = m_AILogic.get_GroupData(cgSINGLE, GetCardLogicValue(m_playing_para.turn_max_cards[0]), nCount);
	}
	else if ((1 << CT_DOUBLE) == m_type)
	{
		gst.uctNowCardGroup = m_AILogic.get_GroupData(cgDOUBLE, GetCardLogicValue(m_playing_para.turn_max_cards[0]), nCount);
	}
	else if ((1 << CT_THREE) == m_type)
	{
		gst.uctNowCardGroup = m_AILogic.get_GroupData(cgTHREE, GetCardLogicValue(m_playing_para.turn_max_cards[0]), nCount);
	}
	else if ((1 << CT_SINGLE_LINE) == m_type)
	{
		gst.uctNowCardGroup = m_AILogic.get_GroupData(cgSINGLE_LINE, GetCardLogicValue(m_playing_para.turn_max_cards[0]), nCount);
	}
	else if ((1 << CT_DOUBLE_LINE) == m_type)
	{
		gst.uctNowCardGroup = m_AILogic.get_GroupData(cgDOUBLE_LINE, GetCardLogicValue(m_playing_para.turn_max_cards[0]), nCount);
	}
	else if ((1 << CT_THREE_LINE) == m_type)
	{
		gst.uctNowCardGroup = m_AILogic.get_GroupData(cgTHREE_LINE, GetCardLogicValue(m_playing_para.turn_max_cards[0]), nCount);
	}
	else if ((1 << CT_THREE_LINE_TAKE_ONE) == m_type)    //三代一和飞机
	{
		if (nCount == 4)
		{
			gst.uctNowCardGroup = m_AILogic.get_GroupData(cgTHREE_TAKE_ONE, GetCardLogicValue(m_playing_para.turn_max_cards[0]), nCount);
		}
		else
		{
			gst.uctNowCardGroup = m_AILogic.get_GroupData(cgTHREE_TAKE_ONE_LINE, GetCardLogicValue(m_playing_para.turn_max_cards[0]), nCount);
		}
	}
	else if ((1 << CT_THREE_LINE_TAKE_TWO) == m_type)    //三带一对和飞机
	{
		if (nCount == 5)
		{
			gst.uctNowCardGroup = m_AILogic.get_GroupData(cgTHREE_TAKE_TWO, GetCardLogicValue(m_playing_para.turn_max_cards[0]), nCount);
		}
		else
		{
			gst.uctNowCardGroup = m_AILogic.get_GroupData(cgTHREE_TAKE_TWO_LINE, GetCardLogicValue(m_playing_para.turn_max_cards[0]), nCount);
		}
	}
	else if ((1 << CT_FOUR_LINE_TAKE_ONE) == m_type)
	{
		gst.uctNowCardGroup = m_AILogic.get_GroupData(cgFOUR_TAKE_ONE, GetCardLogicValue(m_playing_para.turn_max_cards[0]), nCount);
	}
	else if ((1 << CT_FOUR_LINE_TAKE_TWO) == m_type)
	{
		gst.uctNowCardGroup = m_AILogic.get_GroupData(cgFOUR_TAKE_TWO, GetCardLogicValue(m_playing_para.turn_max_cards[0]), nCount);
	}
	else if ((1 << CT_RUAN_BOMB) == m_type)
	{
		gst.uctNowCardGroup = m_AILogic.get_GroupData(cgBOMB_CARD, GetCardLogicValue(m_playing_para.turn_max_cards[0]), nCount);
	}
	else if ((1 << CT_BOMB_CARD) == m_type)
	{
		gst.uctNowCardGroup = m_AILogic.get_GroupData(cgBOMB_CARD, GetCardLogicValue(m_playing_para.turn_max_cards[0]), nCount);
	}
	else if ((1 << CT_LAIZI_BOMB) == m_type)
	{
		gst.uctNowCardGroup = m_AILogic.get_GroupData(cgBOMB_CARD, GetCardLogicValue(m_playing_para.turn_max_cards[0]), nCount);
	}
	else if ((1 << CT_MISSILE_CARD) == m_type)
	{
		gst.uctNowCardGroup = m_AILogic.get_GroupData(cgKING_CARD, GetCardLogicValue(m_playing_para.turn_max_cards[0]), nCount);
	}


	return;
}

//将要出的牌从逻辑值转换成有花色
void __stdcall CGameData::GetCardColorValue(WORD OutCardUser, HandCardData &m_HandCardData, BYTE *OutCardData, BYTE &OutCardNum)
{
	BYTE CurCardData[MAX_CARD_COUNT] = { 0 };
	BYTE CurCardNum = GetUserCurCardNum(OutCardUser);

	GetClientHandCards(OutCardUser, CurCardData, CurCardNum);

	//标志量
	int index = 0;

	//一个一个作比较交换
	for (vector<int>::iterator it = m_HandCardData.value_nPutCardList.begin(); it < m_HandCardData.value_nPutCardList.end(); it++)
	{
		for (int i = 0; i < CurCardNum; i++)
		{
			if (*it == GetCardLogicValue(CurCardData[i]))
			{
				OutCardData[index++] = CurCardData[i];
				CurCardData[i] = 0;
				break;
			}
		}
	}

	OutCardNum = index;

	//给OutCard排序
	SortCardList(OutCardData, OutCardNum, ST_ORDER);

	return;
}

//计算底牌是否翻倍 并把倍数给各玩家乘上     该函数在发牌后调用
DWORD __stdcall CGameData::IsDoubleLeaveCard()
{
	//获取三张底牌
	BYTE LeaveCard[NORMAL_LEAVE_CARD_NUM] = { 0 };

	GetLeaveCard(LeaveCard, NORMAL_LEAVE_CARD_NUM);

	//判断底牌牌型并设置倍数
	if (((0x4F == LeaveCard[0] && 0x4E != LeaveCard[1]) || 0x4E == LeaveCard[0]))		m_playing_para.leave_card_bet = 2;
	if (0x4F == LeaveCard[0] && 0x4E == LeaveCard[1])									m_playing_para.leave_card_bet = 4;
	if (GetCardColor(LeaveCard[0]) == GetCardColor(LeaveCard[1]) && GetCardColor(LeaveCard[1]) == GetCardColor(LeaveCard[2]))		//同花顺
	{
		if (15 != GetCardLogicValue(LeaveCard[0]) &&
			(GetCardLogicValue(LeaveCard[0]) == (GetCardLogicValue(LeaveCard[1]) + 1) && (GetCardLogicValue(LeaveCard[1]) + 1) == (GetCardLogicValue(LeaveCard[2]) + 2)))
		{
			m_playing_para.leave_card_bet = 4;
		}
		else
		{
			m_playing_para.leave_card_bet = 3;
		}
	}
	if (15 != GetCardLogicValue(LeaveCard[0]) &&
		(GetCardLogicValue(LeaveCard[0]) == (GetCardLogicValue(LeaveCard[1]) + 1) && (GetCardLogicValue(LeaveCard[1]) + 1) == (GetCardLogicValue(LeaveCard[2]) + 2)))
	{
		m_playing_para.leave_card_bet = 3;
	}
	if (GetCardLogicValue(LeaveCard[0]) == GetCardLogicValue(LeaveCard[1]) && GetCardLogicValue(LeaveCard[1]) == GetCardLogicValue(LeaveCard[2]))
	{
		m_playing_para.leave_card_bet = 4;
	}

	cout << "leave_card_bet:" << m_playing_para.leave_card_bet << endl;

	for (int i = 0; i < GetCurPlayerCount(); i++)
	{
		m_players_config.players[i].bet *= m_playing_para.leave_card_bet;
	}

	return 0;
}

//获取底牌翻倍倍数
DWORD __stdcall CGameData::GetLeaveCardBet()
{
	return m_playing_para.leave_card_bet;
}

//设置是否底牌翻倍
int __stdcall CGameData::SetISLeaveCardDouble(WORD Mode)
{
	if (Mode != 0 && Mode != 1)
		Mode = 1;

	m_room_config.game_leave_card = Mode;
	return 0;
}

//获取底牌是否翻倍
WORD __stdcall CGameData::GetISLeaveCardDouble()
{
	return m_room_config.game_leave_card;
}

//设置是否明牌			0-不明牌   1-明牌
int __stdcall CGameData::SetMingPaiMode(WORD Mode)
{
	if (Mode != 0 && Mode != 1)
		Mode = 1;

	m_room_config.game_mingpai = Mode;
	return 0;
}

//获取是否明牌
WORD __stdcall CGameData::GetMingPaiMode()
{
	return m_room_config.game_mingpai;
}

//设置是否加倍       0-不加倍  1-加倍
int __stdcall CGameData::SetAddBetMode(WORD Mode)
{
	if (Mode != 0 && Mode != 1)
		Mode = 1;

	m_room_config.game_addbet = Mode;
	return 0;
}

//获取是否加倍
WORD __stdcall CGameData::GetAddBetMode()
{
	return m_room_config.game_addbet;
}

//记录玩家是否出过牌
int __stdcall CGameData::SetSpring(WORD wChairID, WORD spring)
{
	m_players_config.players[wChairID].spring_state += spring;
	return 0;
}

//判断玩家是否春天  0-不是 1-是  乘上倍数
int __stdcall CGameData::JudgeSpring(WORD wChairID)
{
	if (INVALID_CHAIR == m_playing_para.appointbanker)
	{
		return -1;
	}

	int flag = 0;
	if (m_playing_para.appointbanker == wChairID)   //庄家赢了
	{
		for (int i = 0; i < GetCurPlayerCount(); i++)
		{
			if (i != m_playing_para.appointbanker &&
				0 == m_players_config.players[i].spring_state)
			{
				flag++;
			}
		}

		if (2 == flag)
		{
			for (int i = 0; i < GetCurPlayerCount(); i++)
			{
				Wager(i, 2);
			}

			m_players_config.players[wChairID].spring = 1;

			return 1;
		}
		else
		{
			m_players_config.players[wChairID].spring = 0;

			return 0;
		}
	}
	//农民赢了
	else
	{
		if (1 == m_players_config.players[m_playing_para.appointbanker].spring_state)
		{
			for (int i = 0; i < GetCurPlayerCount(); i++)
			{
				Wager(i, 2);

				if (i != m_playing_para.appointbanker)
				{
					m_players_config.players[i].spring = 1;

				}
			}

			return 1;
		}
		else
		{
			for (int i = 0; i < GetCurPlayerCount(); i++)
			{
				if (i != m_playing_para.appointbanker)
				{
					m_players_config.players[i].spring = 0;
				}
			}
			return 0;
		}
	}
}

//获取玩家是否春天
int __stdcall CGameData::GetSpring(WORD wChairID)
{
	return m_players_config.players[wChairID].spring;
}

//设置玩家托管
int __stdcall CGameData::SetPlayerTuoGuan(WORD wChairID, WORD state)
{
	m_players_config.players[wChairID].player_tuo_guan = state;
	return 0;
}

//获取玩家托管
int __stdcall CGameData::GetPlayerTuoGuan(WORD wChairID)
{
	return m_players_config.players[wChairID].player_tuo_guan;
}

//设置玩家连续超时出牌次数
int __stdcall CGameData::SetPlayerTimeOutNum(WORD wChairID, WORD num)
{
	m_players_config.players[wChairID].out_time_num = num;
	return 0;
}

//获取玩家连续超时出牌次数
int __stdcall CGameData::GetPlayerTimeOutNum(WORD wChairID)
{
	return m_players_config.players[wChairID].out_time_num;
}

//================================================癞子场判断=================================================   add by lih

//获得于包含癞子的卡牌的逻辑数值
BYTE CGameData::GetCardLogicValueLaiZi(BYTE cbCardData)
{
	if (0 == cbCardData)
	{
		return 0;
	}

	//逻辑数值
	BYTE cbCardColor = GetCardColor(cbCardData);
	BYTE cbCardValue = GetCardValue(cbCardData);

	//转换数值明显出现错误
	if ((cbCardColor << 4) == 0x40) return cbCardValue + 2;		//大小王   

	if (m_room_config.game_laizi_mode > 0)
	{
		//癞子放在最左边
		if ((m_playing_para.magic_card != 0) && (m_playing_para.magic_card == 16 * 5 + cbCardValue))     //不考虑癞子
		{
			return (cbCardValue + 16 * 5);
		}
	}

	return (cbCardValue <= 2) ? (cbCardValue + 13) : cbCardValue;

}

//设置癞子当成的普通牌
int CGameData::SetLaiZiToCommon(BYTE LaiZiData[], BYTE Count)
{
	if (LaiZiData == NULL)
		return false;

	ZeroMemory(&m_playing_para.laizi_transport, sizeof(m_playing_para.laizi_transport));
	memcpy(m_playing_para.laizi_transport, LaiZiData, Count * sizeof(BYTE));

	m_playing_para.transport_count = Count;

	return 0;
}

//获取癞子当成的普通牌
int CGameData::GetLaiZiToCommon(BYTE LaiZiData[], BYTE &Count)
{
	if (LaiZiData == NULL)
		return false;

	Count = m_playing_para.transport_count;

	for (int i = 0; i < Count; i++)
	{
		LaiZiData[i] = m_playing_para.laizi_transport[i];
	}

	return 0;
}

//将玩家手牌中的癞子进行牌值替换
bool CGameData::ReplaceLaiZiCard(WORD wChairID, BYTE cbCardNum)
{
	if (0 == m_room_config.game_laizi_mode)
		return false;

	for (int i = 0; i < MAX_LAIZI_COUNT; i++)
	{
		if (m_playing_para.magic_card == GetCardLogicValueLaiZi(m_players_config.hand_cards[wChairID][i]))
		{
			m_players_config.hand_cards[wChairID][i] = m_playing_para.magic_card;
		}
	}

	return true;
}

//斗地主不考虑癞子的排序
void CGameData::SortCardListNoLaiZi(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
	//数目过虑
	if (cbCardCount == 0) return;

	//转换数值
	BYTE cbSortValue[MAX_CARD_COUNT];
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		cbSortValue[i] = GetCardLogicValue(cbCardData[i]);
	}

	//排序操作
	bool bSorted = true;
	BYTE cbThreeCount, cbLast = cbCardCount - 1;
	do
	{
		bSorted = true;
		for (BYTE i = 0; i < cbLast; i++)
		{
			if ((cbSortValue[i] < cbSortValue[i + 1]) ||
				((cbSortValue[i] == cbSortValue[i + 1]) && (cbCardData[i] < cbCardData[i + 1])))
			{
				//交换位置
				cbThreeCount = cbCardData[i];
				cbCardData[i] = cbCardData[i + 1];
				cbCardData[i + 1] = cbThreeCount;
				cbThreeCount = cbSortValue[i];
				cbSortValue[i] = cbSortValue[i + 1];
				cbSortValue[i + 1] = cbThreeCount;
				bSorted = false;
			}
		}
		cbLast--;
	} while (bSorted == false);

	return;
}

//判断癞子场的玩家是否能出牌
bool CGameData::JudgeLaiZiPlayerOutCard(WORD wChairID)
{
	//校验
	if (wChairID < 0 || wChairID > MAX_CHAIR_COUNT)
		return false;

	//获取玩家的手牌
	BYTE HandCardData[MAX_CARD_COUNT];
	BYTE HandCardNum = m_players_config.players[wChairID].hand_card_num;

	ZeroMemory(&HandCardData, sizeof(HandCardData));
	CopyMemory(HandCardData, m_players_config.hand_cards[wChairID], sizeof(BYTE)*HandCardNum);

	//获取癞子数量
	BYTE LaiZiNum = GetMagicCardNum(HandCardData, HandCardNum);

	cout << "LaiZiCount:::" << (int)LaiZiNum << endl;

	//定义癞子当普通牌的牌值 DOLATER
	BYTE ReplaceData[MAX_LAIZI_COUNT] = { 0 };		//癞子当普通牌的数据
	BYTE ReplaceNum = 0;							//癞子当普通牌的数量

													//清零
	SetLaiZiToCommon(ReplaceData, ReplaceNum);

	//是否都是癞子
	bool LaiFlag = false;
	for (int i = 0; i < HandCardNum; i++)
	{
		if (m_playing_para.magic_card == GetCardLogicValueLaiZi(HandCardData[i]))
			LaiFlag = true;
		else
			LaiFlag = false;
	}

	//辅助参数定义
	BYTE OutCardData[MAX_CARD_COUNT];
	BYTE OutCardNum;
	ZeroMemory(&OutCardData, sizeof(OutCardData));

	//替换数组定义
	BYTE tmpCard[MAX_CARD_COUNT];
	ZeroMemory(&tmpCard, sizeof(tmpCard));
	CopyMemory(tmpCard, HandCardData, sizeof(BYTE)*HandCardNum);

	//当癞子牌做普通牌
	if (!LaiFlag && LaiZiNum != 0)
	{
		for (int a = 0; a < HandCardNum; a++)
		{
			if (m_playing_para.magic_card == GetCardLogicValueLaiZi(HandCardData[a]))
			{
				cout << "==============OneLai================" << endl;
				//一张癞子
				for (int b = 0; b < 13; b++)
				{
					//牌替换
					tmpCard[a] = m_cbCardData[b] + 0x50;

					//两张癞子
					if (LaiZiNum >= 2)
					{
						for (int c = a + 1; c < HandCardNum; c++)
						{
							if (m_playing_para.magic_card == GetCardLogicValueLaiZi(HandCardData[c]))
							{
								cout << "==============TwoLai================" << endl;
								for (int d = 0; d < 13; d++)
								{
									//牌替换
									tmpCard[c] = m_cbCardData[d] + 0x50;

									//三张癞子
									if (LaiZiNum >= 3)
									{
										for (int e = c + 1; e < HandCardNum; e++)
										{
											if (m_playing_para.magic_card == GetCardLogicValueLaiZi(HandCardData[e]))
											{
												cout << "==============ThreeLai================" << endl;
												for (int f = 0; f < 13; f++)
												{
													//牌替换
													tmpCard[e] = m_cbCardData[f] + 0x50;

													//四张癞子
													if (LaiZiNum >= 4)
													{
														for (int g = e + 1; g < HandCardNum; g++)
														{
															if (m_playing_para.magic_card == GetCardLogicValueLaiZi(HandCardData[g]))
															{
																cout << "==============FourLai================" << endl;
																for (int h = 0; h < 13; h++)
																{
																	//牌替换
																	tmpCard[g] = m_cbCardData[h] + 0x50;

																	//五张癞子  写天地癞子清在这里加

																	//判断是否要的起
																	if (LaiZiCompareCard(tmpCard, HandCardNum, OutCardData, &OutCardNum))
																		return true;
																	else
																		continue;
																}
																break;
															}
														}
													}

													//判断是否要的起
													if (LaiZiCompareCard(tmpCard, HandCardNum, OutCardData, &OutCardNum))
														return true;
													else
														continue;
												}
												break;
											}
										}
									}

									//判断是否要的起
									if (LaiZiCompareCard(tmpCard, HandCardNum, OutCardData, &OutCardNum))
										return true;
									else
										continue;
								}
								break;
							}
						}
					}

					//判断是否要的起
					if (LaiZiCompareCard(tmpCard, HandCardNum, OutCardData, &OutCardNum))
						return true;
					else
						continue;
				}
				break;
			}
		}
	}
	else
	{
		if (LaiZiCompareCard(HandCardData, HandCardNum, OutCardData, &OutCardNum))
			return true;
		else
			return false;
	}

	return false;
}

//癞子：玩家第一个出牌  判断他可以出的合适的牌
bool CGameData::LaiZiAnalysePlayerOutCardFirst(WORD OutCardUsr, BYTE OutCardData[], BYTE *OutCardNum)
{
	//校验
	if (OutCardUsr < 0 || OutCardUsr > MAX_CHAIR_COUNT || OutCardData == NULL)
		return false;



	return true;
}

//癞子：玩家接牌
bool CGameData::LaiZiAnalysePlayerOutCard(WORD OutCardUsr, BYTE OutCardData[], BYTE *OutCardNum)
{
	//校验
	if (OutCardUsr < 0 || OutCardUsr > MAX_CHAIR_COUNT || OutCardData == NULL)
		return false;

	//获取玩家的手牌
	BYTE HandCardData[MAX_CARD_COUNT];
	BYTE HandCardNum = m_players_config.players[OutCardUsr].hand_card_num;

	ZeroMemory(&HandCardData, sizeof(HandCardData));
	CopyMemory(HandCardData, m_players_config.hand_cards[OutCardUsr], sizeof(BYTE)*HandCardNum);

	//获取癞子数量
	BYTE LaiZiNum = GetMagicCardNum(HandCardData, HandCardNum);

	cout << "LaiZiCount:::" << (int)LaiZiNum << endl;

	//定义癞子当普通牌的牌值 DOLATER
	BYTE ReplaceData[MAX_LAIZI_COUNT] = { 0 };		//癞子当普通牌的数据
	BYTE ReplaceNum = 0;							//癞子当普通牌的数量

													//清零
	SetLaiZiToCommon(ReplaceData, ReplaceNum);

	//是否都是癞子
	bool LaiFlag = false;
	for (int i = 0; i < HandCardNum; i++)
	{
		if (m_playing_para.magic_card == GetCardLogicValueLaiZi(HandCardData[i]))
			LaiFlag = true;
		else
			LaiFlag = false;
	}

	//替换数组定义
	BYTE tmpCard[MAX_CARD_COUNT];
	ZeroMemory(&tmpCard, sizeof(tmpCard));
	CopyMemory(tmpCard, HandCardData, sizeof(BYTE)*HandCardNum);

	//当癞子牌做普通牌
	if (!LaiFlag && LaiZiNum != 0)
	{
		for (int a = 0; a < HandCardNum; a++)
		{
			if (m_playing_para.magic_card == GetCardLogicValueLaiZi(HandCardData[a]))
			{
				cout << "==============OneLai================" << endl;
				//一张癞子
				for (int b = 0; b < 13; b++)
				{
					//牌替换
					tmpCard[a] = m_cbCardData[b] + 0x50;

					//两张癞子
					if (LaiZiNum >= 2)
					{
						for (int c = a + 1; c < HandCardNum; c++)
						{
							if (m_playing_para.magic_card == GetCardLogicValueLaiZi(HandCardData[c]))
							{
								cout << "==============TwoLai================" << endl;
								for (int d = 0; d < 13; d++)
								{
									//牌替换
									tmpCard[c] = m_cbCardData[d] + 0x50;

									//三张癞子
									if (LaiZiNum >= 3)
									{
										for (int e = c + 1; e < HandCardNum; e++)
										{
											if (m_playing_para.magic_card == GetCardLogicValueLaiZi(HandCardData[e]))
											{
												cout << "==============ThreeLai================" << endl;
												for (int f = 0; f < 13; f++)
												{
													//牌替换
													tmpCard[e] = m_cbCardData[f] + 0x50;

													//四张癞子
													if (LaiZiNum >= 4)
													{
														for (int g = e + 1; g < HandCardNum; g++)
														{
															if (m_playing_para.magic_card == GetCardLogicValueLaiZi(HandCardData[g]))
															{
																cout << "==============FourLai================" << endl;
																for (int h = 0; h < 13; h++)
																{
																	//牌替换
																	tmpCard[g] = m_cbCardData[h] + 0x50;

																	//五张癞子  写天地癞子清在这里加

																	//判断是否要的起
																	if (LaiZiCompareCard(tmpCard, HandCardNum, OutCardData, OutCardNum))
																		return true;
																	else
																		continue;
																}
																break;
															}
														}
													}

													//判断是否要的起
													if (LaiZiCompareCard(tmpCard, HandCardNum, OutCardData, OutCardNum))
														return true;
													else
														continue;
												}
												break;
											}
										}
									}

									//判断是否要的起
									if (LaiZiCompareCard(tmpCard, HandCardNum, OutCardData, OutCardNum))
										return true;
									else
										continue;
								}
								break;
							}
						}
					}

					//判断是否要的起
					if (LaiZiCompareCard(tmpCard, HandCardNum, OutCardData, OutCardNum))
						return true;
					else
						continue;
				}
				break;
			}
		}
	}
	else
	{
		if (LaiZiCompareCard(HandCardData, HandCardNum, OutCardData, OutCardNum))
			return true;
		else
			return false;
	}

	return false;
}

//癞子：分析比较手牌
bool CGameData::LaiZiCompareCard(BYTE Card[], BYTE CardCount, BYTE OutCardData[]/*out*/, BYTE *OutCardNum)
{
	//校验
	if (Card == NULL || OutCardData == NULL)
		return false;

	//Card数组
	BYTE tmpCard[MAX_CARD_COUNT];
	ZeroMemory(&tmpCard, sizeof(tmpCard));
	CopyMemory(tmpCard, Card, sizeof(BYTE)*CardCount);

	//卡牌排序
	SortCardListNoLaiZi(tmpCard, CardCount, ST_ORDER);

	//构建值
	BYTE tmpOutCardData[MAX_CARD_COUNT];
	BYTE tmpOutCardNum = 0;
	ZeroMemory(&tmpOutCardData, sizeof(tmpOutCardData));

	//获得该轮最大出牌数据
	BYTE TurnCardNum = m_playing_para.turn_cards_num;
	BYTE TurnCardData[MAX_CARD_COUNT];
	ZeroMemory(&TurnCardData, sizeof(TurnCardData));
	CopyMemory(TurnCardData, m_playing_para.turn_max_cards, sizeof(TurnCardData));

	//获取当前玩家的手牌数据
	BYTE CurCardNum = CardCount;
	BYTE CurCardData[MAX_CARD_COUNT];
	ZeroMemory(&CurCardData, sizeof(CurCardData));

	for (int i = 0; i < CurCardNum; i++)
	{
		CurCardData[i] = tmpCard[i];
	}

	//分析两个扑克牌型
	tagAnalyseResult TurnResult;
	tagAnalyseResult CurResult;

	ZeroMemory(&TurnResult, sizeof(TurnResult));
	ZeroMemory(&CurResult, sizeof(CurResult));

	AnalysebCardData(CurCardData, CurCardNum, CurResult);
	AnalysebCardData(TurnCardData, TurnCardNum, TurnResult);

	//获取出牌玩家的牌型
	int TurnCardType = GetType(TurnResult, TurnCardData, TurnCardNum, TurnCardData);
	TurnCardType = TransListToInt(0, TurnCardType);

	//对比判断牌型
	if (TurnCardType == CT_ERROR)	return false;
	if ((TurnCardType & (1 << CT_MISSILE_CARD)) != 0)    return false;    //王炸判断

																		  //炸弹判断
	if ((TurnCardType & (1 << CT_BOMB_CARD)) != 0)   //上家出炸弹
	{
		cout << "炸弹" << endl;

		//获取上家出牌数值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbFourCardData[0]);
		int index = 0;   //辅助标识

						 //if (CurResult.cbFourCount == 0)    return false;
		for (int i = 0; i < CurResult.cbFourCount; i++)
		{
			bool Ruanboom = false;
			int  LaiZiFlag = 0;
			for (int k = 0; k < 4; k++)
			{
				if (0x50 < CurResult.cbFourCardData[index + k])
				{
					Ruanboom = true;
					LaiZiFlag++;
				}
			}

			if ((GetCardLogicValue(CurResult.cbFourCardData[index]) > TurnLogicValue && !Ruanboom) || 4 == LaiZiFlag)
			{
				for (int j = 0; j < 4; j++)
					tmpOutCardData[j] = CurResult.cbFourCardData[index++];
				tmpOutCardNum = 4;

				memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
				*OutCardNum = tmpOutCardNum;

				return true;
			}

			index += 4;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
			{
				tmpOutCardData[i] = CurCardData[i];
			}
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_RUAN_BOMB)) != 0)   //上家出软炸
	{
		cout << "软炸" << endl;

		//获取上家出牌数值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbFourCardData[0]);
		int index = 0;   //辅助标识

						 //if (CurResult.cbFourCount == 0)    return false;
		for (int i = 0; i < CurResult.cbFourCount; i++)
		{
			bool Ruanboom = false;
			for (int k = 0; k < 4; k++)
			{
				if (0x50 < CurResult.cbFourCardData[index + k])
					Ruanboom = true;
			}

			if ((Ruanboom && GetCardLogicValue(CurResult.cbFourCardData[index]) > TurnLogicValue) || !Ruanboom)
			{
				for (int j = 0; j < 4; j++)
					tmpOutCardData[j] = CurResult.cbFourCardData[index++];
				tmpOutCardNum = 4;

				memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
				*OutCardNum = tmpOutCardNum;

				return true;
			}

			index += 4;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
			{
				tmpOutCardData[i] = CurCardData[i];
			}
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_LAIZI_BOMB)) != 0)   //癞子炸弹
	{
		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
			{
				tmpOutCardData[i] = CurCardData[i];
			}
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_SINGLE)) != 0)   //单张
	{
		cout << "单张" << endl;

		//获取上家出牌数值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbSignedCardData[0]);

		cout << "获取上家出牌数值:" << (int)TurnLogicValue << endl;

		int index = (CurResult.cbSignedCount - 1);
		for (int i = 0; i < CurResult.cbSignedCount; i++)
		{
			if (GetCardLogicValue(CurResult.cbSignedCardData[index]) > TurnLogicValue)
			{
				for (int i = 0; i < 2; i++)
					tmpOutCardData[i] = CurResult.cbSignedCardData[index++];
				tmpOutCardNum = 1;

				//cout << " tmpOutCardData[i]: " << tmpOutCardData[i] << endl;

				memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
				*OutCardNum = tmpOutCardNum;

				return true;
			}

			index--;
		}

		for (int i = CurCardNum - 1; i > 0; i--)
		{
			if (GetCardLogicValue(CurCardData[i]) > TurnLogicValue)
			{
				tmpOutCardData[0] = CurCardData[i];
				tmpOutCardNum = 1;

				memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
				*OutCardNum = tmpOutCardNum;

				return true;
			}
		}

		//炸弹判断
		if (CurResult.cbFourCount != 0)
		{
			int index = 0;
			for (int i = (CurResult.cbFourCount - 1) * 4; i < CurResult.cbFourCount * 4; i++)
			{
				tmpOutCardData[index++] = CurResult.cbFourCardData[i];
			}
			tmpOutCardNum = 4;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurCardData[i];
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_DOUBLE)) != 0)   //对子
	{
		cout << "对子" << endl;

		//获取上家出牌数值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbDoubleCardData[0]);

		int index = (CurResult.cbDoubleCount - 1) * 2;
		for (int i = 0; i < CurResult.cbDoubleCount; i++)
		{
			if (GetCardLogicValue(CurResult.cbDoubleCardData[index]) > TurnLogicValue)
			{
				for (int i = 0; i < 2; i++)
					tmpOutCardData[i] = CurResult.cbDoubleCardData[index++];
				tmpOutCardNum = 2;

				//cout << " tmpOutCardData[i]: " << tmpOutCardData[i] << endl;

				memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
				*OutCardNum = tmpOutCardNum;

				return true;
			}

			index -= 2;
		}

		index = (CurResult.cbThreeCount - 1) * 3;
		for (int i = 0; i < CurResult.cbThreeCount; i++)
		{
			if (GetCardLogicValue(CurResult.cbThreeCardData[index]) > TurnLogicValue)
			{
				for (int i = 0; i < 2; i++)
					tmpOutCardData[i] = CurResult.cbThreeCardData[index++];
				tmpOutCardNum = 2;

				memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
				*OutCardNum = tmpOutCardNum;

				return true;
			}

			index -= 3;
		}

		//炸弹判断
		if (CurResult.cbFourCount != 0)
		{
			int index = 0;
			for (int i = (CurResult.cbFourCount - 1) * 4; i < CurResult.cbFourCount * 4; i++)
			{
				tmpOutCardData[index++] = CurResult.cbFourCardData[i];
			}
			tmpOutCardNum = 4;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurCardData[i];
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_THREE)) != 0)   //三张
	{
		cout << "三张" << endl;

		//获取上家出牌数值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);

		int index = (CurResult.cbThreeCount - 1) * 3;
		for (int i = 0; i < CurResult.cbThreeCount; i++)
		{
			if (GetCardLogicValue(CurResult.cbThreeCardData[index]) > TurnLogicValue)
			{
				for (int i = 0; i < 3; i++)
					tmpOutCardData[i] = CurResult.cbThreeCardData[index++];
				tmpOutCardNum = 3;

				memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
				*OutCardNum = tmpOutCardNum;

				return true;
			}

			index -= 3;
		}

		//炸弹判断
		if (CurResult.cbFourCount != 0)
		{
			int index = 0;
			for (int i = (CurResult.cbFourCount - 1) * 4; i < CurResult.cbFourCount * 4; i++)
			{
				tmpOutCardData[index++] = CurResult.cbFourCardData[i];
			}
			tmpOutCardNum = 4;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurCardData[i];
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_SINGLE_LINE)) != 0)   //顺子
	{
		cout << "顺子" << endl;

		//获取上家出牌数值  最大值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbSignedCardData[0]);
		cout << "顺子:获取上家出牌数值:" << (int)TurnLogicValue << endl;

		int flag = 0;
		int index = 0;

		for (int i = 0; i < CurCardNum; i++)     //计算顺子张数
		{
			if (flag != 0)    //连续不用比牌大小
			{
				if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
				{
					continue;
				}
				if (GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //判断是否连续
				{
					tmpOutCardData[index++] = CurCardData[i];
					tmpOutCardNum++;

					flag++;

					//跳出条件
					if (flag == TurnResult.cbSignedCount - 1)
					{
						tmpOutCardData[index] = CurCardData[i + 1];
						tmpOutCardNum++;
						break;
					}
				}
				else
				{
					ZeroMemory(&tmpOutCardData, sizeof(tmpOutCardData));
					tmpOutCardNum = 0;

					index = 0;
					flag = 0;
				}
			}
			else
			{
				if (GetCardLogicValue(CurCardData[i]) > TurnLogicValue)
				{
					if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
					{
						continue;
					}
					if (GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //判断是否连续
					{
						tmpOutCardData[index++] = CurCardData[i];
						tmpOutCardNum++;

						flag++;

						//跳出条件
						if (flag == TurnResult.cbSignedCount - 1)
							break;
					}
					else
					{
						ZeroMemory(&tmpOutCardData, sizeof(tmpOutCardData));
						tmpOutCardNum = 0;

						index = 0;
						flag = 0;
					}
				}
			}
		}

		if (flag >= TurnResult.cbSignedCount - 1)
		{
			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//炸弹判断
		if (CurResult.cbFourCount != 0)
		{
			int index = 0;
			for (int i = (CurResult.cbFourCount - 1) * 4; i < CurResult.cbFourCount * 4; i++)
			{
				tmpOutCardData[index++] = CurResult.cbFourCardData[i];
			}
			tmpOutCardNum = 4;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurCardData[i];
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_DOUBLE_LINE)) != 0)   //连对
	{
		cout << "连对" << endl;


		//获取上家出牌数值  最大值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbDoubleCardData[0]);
		int flag = 0;   //判断是否连续标志
		int Numflag = 0;   //判断牌数是否达标的标志
		int index = 0;

		for (int i = 0; i < CurCardNum; i++)     //计算连对张数
		{
			if (flag != 0)    //连续不用比牌大小
			{
				if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
				{
					Numflag++;
					continue;
				}
				if ((GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1))
					&& (Numflag > 0) && (GetCardLogicValue(CurCardData[i]) < 15))   //判断是否连续
				{
					tmpOutCardData[index++] = CurCardData[i];		//连对加两次
					tmpOutCardNum++;

					tmpOutCardData[index++] = CurCardData[i - 1];
					tmpOutCardNum++;

					flag++;

					//跳出条件
					if (flag == TurnResult.cbDoubleCount)
						break;
				}
				else
				{
					ZeroMemory(&tmpOutCardData, sizeof(tmpOutCardData));
					tmpOutCardNum = 0;

					index = 0;
					flag = 0;
				}

				if (GetCardLogicValue(CurCardData[i + 1]) != GetCardLogicValue(CurCardData[i]))
				{
					Numflag = 0;
				}
			}
			else
			{
				if (GetCardLogicValue(CurCardData[i]) > TurnLogicValue)
				{
					if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
					{
						Numflag++;
						continue;
					}
					if (GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1)
						&& (Numflag > 0) && (GetCardLogicValue(CurCardData[i]) < 15))   //判断是否连续
					{
						tmpOutCardData[index++] = CurCardData[i];		//连对加两次
						tmpOutCardNum++;

						tmpOutCardData[index++] = CurCardData[i - 1];
						tmpOutCardNum++;

						flag++;

						//跳出条件
						if (flag == TurnResult.cbDoubleCount)
							break;
					}
					else
					{
						ZeroMemory(&tmpOutCardData, sizeof(tmpOutCardData));
						tmpOutCardNum = 0;

						index = 0;
						flag = 0;
					}

					if (GetCardLogicValue(CurCardData[i + 1]) != GetCardLogicValue(CurCardData[i]))
					{
						Numflag = 0;
					}
				}
			}
		}

		if (flag >= TurnResult.cbDoubleCount)
		{
			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//炸弹判断
		if (CurResult.cbFourCount != 0)
		{
			int index = 0;
			for (int i = (CurResult.cbFourCount - 1) * 4; i < CurResult.cbFourCount * 4; i++)
			{
				tmpOutCardData[index++] = CurResult.cbFourCardData[i];
			}
			tmpOutCardNum = 4;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurCardData[i];
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_THREE_LINE)) != 0)   //三连
	{
		cout << "三连" << endl;

		//获取上家出牌数值  最大值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);
		int flag = 0;   //判断是否连续标志
		int Numflag = 0;   //判断牌数是否达标的标志
		int index = 0;

		for (int i = 0; i < CurCardNum; i++)     //计算连对张数
		{
			if (flag != 0)    //连续不用比牌大小
			{
				if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
				{
					Numflag++;
					continue;
				}
				if ((GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1))
					&& (Numflag > 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //判断是否连续
				{
					tmpOutCardData[index++] = CurCardData[i];		//三连加三次
					tmpOutCardNum++;

					tmpOutCardData[index++] = CurCardData[i - 1];
					tmpOutCardNum++;

					tmpOutCardData[index++] = CurCardData[i - 2];
					tmpOutCardNum++;

					flag++;

					//跳出条件
					if (flag == TurnResult.cbThreeCount)
						break;
				}
				else
				{
					ZeroMemory(&tmpOutCardData, sizeof(tmpOutCardData));
					tmpOutCardNum = 0;

					index = 0;
					flag = 0;
				}

				if (GetCardLogicValue(CurCardData[i + 1]) != GetCardLogicValue(CurCardData[i]))   //不是同牌了 标志位制零
				{
					Numflag = 0;
				}
			}
			else
			{
				if (GetCardLogicValue(CurCardData[i]) > TurnLogicValue)
				{
					if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
					{
						Numflag++;
						continue;
					}
					if (GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1)
						&& (Numflag > 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //判断是否连续
					{
						tmpOutCardData[index++] = CurCardData[i];		//三连加三次
						tmpOutCardNum++;

						tmpOutCardData[index++] = CurCardData[i - 1];
						tmpOutCardNum++;

						tmpOutCardData[index++] = CurCardData[i - 2];
						tmpOutCardNum++;

						flag++;

						//跳出条件
						if (flag == TurnResult.cbThreeCount)
							break;
					}
					else
					{
						ZeroMemory(&tmpOutCardData, sizeof(tmpOutCardData));
						tmpOutCardNum = 0;

						index = 0;
						flag = 0;
					}

					if (GetCardLogicValue(CurCardData[i + 1]) != GetCardLogicValue(CurCardData[i]))   //不是同牌了 标志位制零
					{
						Numflag = 0;
					}
				}
			}
		}

		if (flag >= TurnResult.cbThreeCount)
		{
			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//炸弹判断
		if (CurResult.cbFourCount != 0)
		{
			int index = 0;
			for (int i = (CurResult.cbFourCount - 1) * 4; i < CurResult.cbFourCount * 4; i++)
			{
				tmpOutCardData[index++] = CurResult.cbFourCardData[i];
			}
			tmpOutCardNum = 4;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurCardData[i];
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_THREE_LINE_TAKE_ONE)) != 0)   //三代一
	{

		cout << "三代一" << endl;


		//获取上家出牌数值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);

		if (CurResult.cbSignedCount >= TurnResult.cbThreeCount)     //没有单张就没必要出这个牌
		{
			if (TurnResult.cbThreeCount == 1)    //三代一
			{
				int index = 0;
				for (int i = 0; i < CurResult.cbThreeCount; i++)
				{
					if (GetCardLogicValue(CurResult.cbThreeCardData[index]) > TurnLogicValue)
					{
						for (int j = 0; j < 3; j++)
						{
							tmpOutCardData[j] = CurResult.cbThreeCardData[j];
						}
						tmpOutCardData[3] = CurResult.cbSignedCardData[TurnResult.cbSignedCount - 1];
						tmpOutCardNum = 4;

						memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
						*OutCardNum = tmpOutCardNum;

						return true;
					}

					index += 3;
				}
			}
			else		//飞机
			{
				int index = 0;
				int flag = 1;   //判断飞机几个三张的标志
								//判断三个是否连续
				for (int i = 0; i < CurResult.cbThreeCount; i++)
				{
					if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) - 1) == (GetCardLogicValue(CurResult.cbThreeCardData[index + 3])))  //连续
					{
						flag++;
					}

					if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) >(TurnLogicValue - TurnResult.cbThreeCount + 2))
						&& (flag == TurnResult.cbThreeCount))   //大小比较
					{
						index = index - TurnResult.cbThreeCount * 3 + 3;     //赋值的下标
						for (int j = 0; j < TurnResult.cbThreeCount * 3; j++)
						{
							tmpOutCardData[j] = CurResult.cbThreeCardData[index++];
						}

						for (int j = 0; j < TurnResult.cbThreeCount; j++)
						{
							tmpOutCardData[TurnResult.cbThreeCount * 3 + j] = CurResult.cbSignedCardData[TurnResult.cbSignedCount - j - 1];   //飞机带的单
						}

						tmpOutCardNum = TurnResult.cbThreeCount * 4;

						memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
						*OutCardNum = tmpOutCardNum;

						return true;
					}

					index += 3;
				}
			}
		}

		//炸弹判断
		if (CurResult.cbFourCount != 0)
		{
			int index = 0;
			for (int i = (CurResult.cbFourCount - 1) * 4; i < CurResult.cbFourCount * 4; i++)
			{
				tmpOutCardData[index++] = CurResult.cbFourCardData[i];
			}
			tmpOutCardNum = 4;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurCardData[i];
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_THREE_LINE_TAKE_TWO)) != 0)   //三代二
	{
		cout << "三代二" << endl;

		//获取上家出牌数值
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);

		if (CurResult.cbDoubleCount >= TurnResult.cbThreeCount)     //没有对子就没必要出这个牌
		{
			if (TurnResult.cbThreeCount == 1)    //三代二
			{
				int index = 0;
				for (int i = 0; i < CurResult.cbThreeCount; i++)
				{
					if (GetCardLogicValue(CurResult.cbThreeCardData[index]) > TurnLogicValue)
					{
						for (int j = 0; j < 3; j++)
						{
							tmpOutCardData[j] = CurResult.cbThreeCardData[j];
						}
						tmpOutCardData[3] = CurResult.cbDoubleCardData[TurnResult.cbDoubleCount * 2 - 1];
						tmpOutCardData[4] = CurResult.cbDoubleCardData[TurnResult.cbDoubleCount * 2 - 2];
						tmpOutCardNum = 5;

						memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
						*OutCardNum = tmpOutCardNum;

						return true;
					}

					index += 3;
				}
			}
			else		//飞机
			{
				int index = 0;
				int flag = 1;   //判断飞机几个三张的标志
								//判断三个是否连续
				for (int i = 0; i < CurResult.cbThreeCount; i++)
				{
					if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) - 1) == (GetCardLogicValue(CurResult.cbThreeCardData[index + 3])))  //连续
					{
						flag++;
					}

					if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) >(TurnLogicValue - TurnResult.cbThreeCount + 2))
						&& (flag == TurnResult.cbThreeCount))   //大小比较
					{
						index = index - TurnResult.cbThreeCount * 3 + 3;     //赋值的下标
						for (int j = 0; j < TurnResult.cbThreeCount * 3; j++)
						{
							tmpOutCardData[j] = CurResult.cbThreeCardData[index++];
						}

						for (int j = 0; j < TurnResult.cbThreeCount * 2; j += 2)
						{
							tmpOutCardData[TurnResult.cbThreeCount * 3 + j] = CurResult.cbDoubleCardData[TurnResult.cbDoubleCount * 2 - j - 1];   //飞机带的对
							tmpOutCardData[TurnResult.cbThreeCount * 3 + j + 1] = CurResult.cbDoubleCardData[TurnResult.cbDoubleCount * 2 - j - 1];
						}

						tmpOutCardNum = TurnResult.cbThreeCount * 5;

						memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
						*OutCardNum = tmpOutCardNum;

						return true;
					}

					index += 3;
				}
			}
		}

		//炸弹判断
		if (CurResult.cbFourCount != 0)
		{
			int index = 0;
			for (int i = (CurResult.cbFourCount - 1) * 4; i < CurResult.cbFourCount * 4; i++)
			{
				tmpOutCardData[index++] = CurResult.cbFourCardData[i];
			}
			tmpOutCardNum = 4;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurCardData[i];
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_FOUR_LINE_TAKE_ONE)) != 0)   //四代二
	{
		cout << "四代二" << endl;

		//炸弹判断
		if (CurResult.cbFourCount != 0)
		{
			int index = 0;
			for (int i = (CurResult.cbFourCount - 1) * 4; i < CurResult.cbFourCount * 4; i++)
			{
				tmpOutCardData[index++] = CurResult.cbFourCardData[i];
			}
			tmpOutCardNum = 4;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurCardData[i];
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}
	}
	else if ((TurnCardType & (1 << CT_FOUR_LINE_TAKE_TWO)) != 0)   //四代二对
	{
		cout << "四代二对" << endl;

		//炸弹判断
		if (CurResult.cbFourCount != 0)
		{
			int index = 0;
			for (int i = (CurResult.cbFourCount - 1) * 4; i < CurResult.cbFourCount * 4; i++)
			{
				tmpOutCardData[index++] = CurResult.cbFourCardData[i];
			}
			tmpOutCardNum = 4;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

		//王炸判断
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurCardData[i];
			tmpOutCardNum = 2;

			memcpy(OutCardData, tmpOutCardData, tmpOutCardNum*(sizeof(BYTE)));
			*OutCardNum = tmpOutCardNum;

			return true;
		}

	}

	return false;

}
//================================================癞子场判断=================================================