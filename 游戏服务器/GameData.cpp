#include "StdAfx.h"
#include <vector>
#include <map>
#include <algorithm>
#include "GameData.h"
#include "SubGameRule.h"
#include "GameConfig.h"

//�˿�����
const BYTE	CGameData::m_cbCardData[FULL_COUNT]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
	0x4E,0x4F,
};

CGameData::CGameData()
{

	//CreateLog( L"SUB_GAMES_LOG.txt" );

	memset( &m_card_config, 0, sizeof m_card_config);
	memset( &m_players_config, 0, sizeof m_players_config);
}


/******************************************************************************/
/*******************************  ��Ϸ����ӿ�ʵ��  *****************************/
/******************************************************************************/
// ������Ϸ
int __stdcall CGameData::CreateGame(CGameLogic **pGameRule, CGameAccess **pGameAccess)
{

	//1. �˴���������GameRule���� -- ������GameData�ĺ���
	//2. GameLogic��GameAccess,CSubGameRule ָ���GameRule����
	CSubGameRule *_game = new CSubGameRule();

	*pGameRule = _game;
	*pGameAccess = _game;

	return 0;
}

//���ض�ȡ�������ļ�����
int __stdcall CGameData::LoadConfigParameter(const wchar_t *_logFile, const BYTE cbGameScoreMode)
{
	int result = -1;

	// ������Ϸ���ơ���ҡ�����
	result = LoadGameCards();		// �������ƣ���m_card_config��ֵ
	if (0 == result)
	{
		result = LoadPlayers();	// ������ң���m_players_config��ֵ
		if (0 == result)
		{
			//��ȡ���ӹ���
			result = LoadGameRule(cbGameScoreMode);	//�������ӹ��򣬸�m_room_config��ֵ

			// ��ʼ��ȫ������
			InitGameData();
		}
		else
		{
			//CLog::Log(log_error, "��������쳣\n");
		}
	}
	else
	{
		//CLog::Log(log_error, "���ÿ����쳣\n");
	}

	return result;
}

// ������Ϸ
int __stdcall CGameData::Init()
{
	//���������ļ�


	//��ʼ����Ϸ���� -- ������ڶ�ȡ�����ļ�֮��
	InitGameData();

	return 0;
}

// ����ܱ���
int __stdcall CGameData::Wager(WORD wChairID, float bet)
{
	//TODO �ӱ�����ͨ�������ļ�����

	//�ⶥ

	m_players_config.players[wChairID].bet *= bet;

	if (m_playing_para.max_room_bet < m_players_config.players[wChairID].bet)
	{
		m_players_config.players[wChairID].bet = m_playing_para.max_room_bet;
		return 0;
	}

	return 0;
}


// ������Ҳ���
int __stdcall CGameData::SetUserOperate(WORD wChairID, BYTE byAction, DWORD dwActionScore)
{
	//������Ҷ���
	m_players_config.players[wChairID].action = byAction;

	switch (byAction)
	{
	case 1:		//��������Ѻע����������ƣ�ֱ����׷�
	{
		m_players_config.players[wChairID].bet = 0;
		m_playing_para.cur_total_bet += m_players_config.players[wChairID].bet;
		printf("����� = %d������ = %d����ע = %d��", wChairID, byAction, m_players_config.players[wChairID].bet);
		break;
	}
	case 2:		//����Ѻ 1����ʼ��ע�׷�
	{
		m_players_config.players[wChairID].bet = dwActionScore;
		m_playing_para.cur_total_bet += m_players_config.players[wChairID].bet;
		printf("����� = %d������ = %d����ע = %d��", wChairID, byAction, m_players_config.players[wChairID].bet);
		break;
	}
	case 3:		//�ԣ�Ѻ��ʼ��ע�׷�
	{
		m_players_config.players[wChairID].bet = m_playing_para.start_bet;
		m_playing_para.cur_total_bet += m_players_config.players[wChairID].bet;
		printf("����� = %d������ = %d����ע = %d��", wChairID, byAction, m_players_config.players[wChairID].bet);
		break;
	}
	case 4:		//�����׷� + �������Ѻע�ۺ�
	{
		m_players_config.players[wChairID].bet = m_playing_para.cur_total_bet;
		m_playing_para.cur_total_bet += m_players_config.players[wChairID].bet;
		printf("����� = %d������ = %d����ע = %d��", wChairID, byAction, m_players_config.players[wChairID].bet);
		break;
	}
	}

	return 0;
}


// ָ��ׯ��
bool __stdcall CGameData::AppointBanker(WORD wChairID)
{
	m_playing_para.appointbanker = wChairID;
	return true;
}

//��õ�ǰָ����ׯ��
WORD __stdcall CGameData::GetAppointBanker()
{
	return m_playing_para.appointbanker;
}

/**
* �ͻ��˷����Ƿ���ׯ
* @param		wChairID		[in-out]		���ӱ��
* @param		type			[in]		��ׯ����		//	0-������	1-�е���
* @param		state			[in]		��ׯ״̬		//  0-��		1-��/������
*/
int __stdcall CGameData::SetBankerState(WORD wChairID, BYTE cbResult)
{
	m_players_config.players[wChairID].bank_state = cbResult;   // //0-����  1-�е���  2-����  3-������

	return 0;
}

// ��ȡ�û���ׯ״̬
BYTE __stdcall CGameData::GetBankerState(WORD wChairID)
{
	return m_players_config.players[wChairID].bank_state;
}

// �����û�����
int __stdcall CGameData::AnsysCardsType()
{
	return 0;
}

/*************************************************
@Description:	ϴ�Ʋ����ƣ��������鲢��������鸳ֵ
@Input:			wChairID			[in-out]		���ӱ��
				force			[in-out]		�Ƿ�ָ���ɹ�
@Output:
@Return:		0
@author & data:	lizhihu 2017.11.2
*************************************************/
int __stdcall CGameData::Deal()
{
	// ����ϴ�Ʒ��Ƶ���ʱ�洢��
	CARD_DESC *cards = new CARD_DESC[m_card_config.game_cards_sum]();

	CARD_DESC *boomCards = new CARD_DESC[m_card_config.game_cards_sum]();
	BYTE	 boomSum = 0;

	//��õ���������һ���Ƶ���������
	CGameCardConfig::OutOrder(cards, m_card_config.game_cards_sum,
		m_card_config.game_cards, m_card_config.game_cards_sum,
		m_room_config.game_score_mode, boomCards, boomSum); // ϴ��

	///*
	BYTE cardssum = m_card_config.game_cards_sum;
	//���Թ̶���
	BYTE tmpcards[54] = {
		0x4E, 0x4F, 0x01, 0x11, 0x21, 0x31, 0x02, 0x12, 0x22, 0x32, 0x03, 0x13, 0x23, 0x33, 0x04, 0x14, 0x24, 0x34, 0x05, 0x15, 0x25, 0x35,
		0x06, 0x16, 0x26, 0x36, 0x07, 0x17, 0x27, 0x37, 0x08, 0x18, 0x28, 0x38, 0x09, 0x19, 0x29, 0x39, 0x0A, 0x1A, 0x2A, 0x3A, 0x0B, 0x1B, 0x2B, 0x3B,
		0x0C, 0x1C, 0x2C, 0x3C, 0x0D, 0x1D, 0x2D, 0x3D,
	};
	memcpy(cards, tmpcards, cardssum * sizeof(BYTE));
	//*/

	//��ÿ۵׿���
	m_playing_para.leave_card[MAX_LEAVE_CARD_NUM];

	//���䳡�ַ�����
	if (0 == m_room_config.game_score_mode)
	{
		//TODONOW �����Ƿ�������
		ZeroMemory(m_playing_para.leave_card, sizeof(m_playing_para.leave_card));
		CopyMemory(m_playing_para.leave_card, &cards[m_card_config.game_cards_sum - m_card_config.leave_card_num],
			m_card_config.leave_card_num * sizeof(BYTE));

		SortCardList(m_playing_para.leave_card, m_card_config.leave_card_num, ST_ORDER);

		//��һ���Ʒַ����������
		const CARD_DESC *card_index = cards;

		//�ַ���
		for (int i = 0; i < m_room_config.max_chair_sum; ++i)
		{
			int nCardNum = m_card_config.init_card_num;
			//�ж��Ƿ�����Ϸ��
			if (USER_PLAYING == m_players_config.players[i].play_state)
			{
				//��ҵ�ǰ������
				m_players_config.players[i].hand_card_num = nCardNum;
				cout << "��Ϸ�����ѳ�ʼ��" << endl;
				//�����������
				memcpy(m_players_config.hand_cards[i], card_index, sizeof(CARD_DESC)*nCardNum);
				cout << "1" << endl;

				//��������
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
	else	//��ϴ��ģʽ
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

		//������ը���Ż�
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
			//�ж��Ƿ�����Ϸ��
			if (USER_PLAYING == m_players_config.players[i].play_state)
			{
				//��ҵ�ǰ������
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

				//��������
				SortCardList(m_players_config.hand_cards[i], nCardNum, ST_ORDER);

			}
		}
	}

	//�ͷ�
	delete[] cards;
	delete[] boomCards;
	cards = NULL;

	return 0;
}

//���㳭�׵÷�
SCORE __stdcall CGameData::CalculateChaodiScore(const BYTE &cbCDcardNum)
{
	//��õ��Ƶ����з���
	WORD wLeaveCardScore = GetAllCardScore(m_playing_para.leave_card, MAX_LEAVE_CARD_NUM);

	//���׷����Ϳ������й�
	SCORE TotalScore = ((cbCDcardNum + 1) / 2)*wLeaveCardScore;

	//��õ÷ַ�
	WORD wScorer = GetNextUser(m_playing_para.appointbanker);

	//С�ֵ÷�����
	m_players_config.players[wScorer].single_score += TotalScore;

	return TotalScore;
}

//���ݵ÷ּ��������������
BYTE __stdcall CGameData::AnalyseUpGrade(const SCORE &score)
{
	BYTE cbLevel = 0;
	//���
	if (SCORE_DA_GUANG == score)
	{
		cbLevel = LEVEL_DA_GUANG;
		//ׯ�ҵĶԼ���ׯ
		m_playing_para.appointbanker = GetOppositeUser(m_playing_para.appointbanker);
	}
	//С��
	if (SCORE_XIAO_GUANG > score)
	{
		cbLevel = LEVEL_XIAO_GUANG;
		//ׯ�ҵĶԼ���ׯ
		m_playing_para.appointbanker = GetOppositeUser(m_playing_para.appointbanker);
	}
	//40-80
	if ((SCORE_XIAO_GUANG <= score) && (SCORE_CHANGE_BANKER > score))
	{
		cbLevel = LEVEL_CHANGE_BANKER;
		//ׯ�ҵĶԼ���ׯ
		m_playing_para.appointbanker = GetOppositeUser(m_playing_para.appointbanker);
	}
	//80-120
	if ((SCORE_CHANGE_BANKER <= score) && (SCORE_UPGRADE > score))
	{
		//��ׯ,ׯ�ҵ���һ����ׯ
		m_playing_para.appointbanker = GetNextUser(m_playing_para.appointbanker);
	}
	//>=120
	if (SCORE_UPGRADE <= score)
	{
		cbLevel = (score - SCORE_UPGRADE) / SCORE_UPGRADE_ONE;

		//��ׯ,ׯ�ҵ���һ����ׯ
		m_playing_para.appointbanker = GetNextUser(m_playing_para.appointbanker);
	}

	//�����ټ�
	m_playing_para.level_value += cbLevel;

	printf("\n���¾���Ϸ ���ȼ� = %d��ׯ�� = %d��\n", m_playing_para.level_value, m_playing_para.appointbanker);

	return cbLevel;
}

//�����Ƚ�     
bool CGameData::AnalysebCompare(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult, int dCardType)
{
	//�����˿�
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

	//������������ͨ��				//��ͨ�����������
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

	//�����ж�
	if (bLaiZiCount != 0 && !SameCard && m_room_config.game_laizi_mode > 0)
	{
		for (BYTE i = 0;i<cbCardCount;i++)
		{
			if (GetCardLogicValueLaiZi(cbCardData[i]) == m_playing_para.magic_card)
			{
				//һ������
				for (BYTE j = 0;j<13;j++)
				{
					bCardDataTemp[i] = m_cbCardData[j];

					//��������
					if (bLaiZiCount >= 2)
					{
						for (BYTE k = i + 1;k<cbCardCount;k++)
						{
							if (GetCardLogicValueLaiZi(cbCardData[k]) == m_playing_para.magic_card)
							{
								for (BYTE z = 0;z<13;z++)
								{
									bCardDataTemp[k] = m_cbCardData[z];

									//��������
									if (bLaiZiCount >= 3)
									{
										for (BYTE g = k + 1;g<cbCardCount;g++)
										{
											if (GetCardLogicValueLaiZi(cbCardData[g]) == m_playing_para.magic_card)
											{
												for (BYTE f = 0;f<13;f++)
												{
													bCardDataTemp[g] = m_cbCardData[f];

													//��������
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

//�����˿�
void CGameData::AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult)
{
	//���ñ���
	BYTE bCardData[MAX_CARD_COUNT];

	memset(bCardData, 0, sizeof(bCardData));
	CopyMemory(bCardData, cbCardData, sizeof(BYTE)*cbCardCount);

	SortCardListNoLaiZi(bCardData, cbCardCount, ST_ORDER);

	//�˿˷���
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		//��������
		BYTE cbSameCount = 1, cbCardValueTemp = 0;
		BYTE cbLogicValue = GetCardLogicValue(bCardData[i]);

		//����ͬ��
		for (BYTE j = i + 1; j < cbCardCount; j++)
		{
			//��ȡ�˿�
			if (GetCardLogicValue(bCardData[j]) != cbLogicValue) break;

			//���ñ���
			cbSameCount++;
		}

		//���ý��
		switch (cbSameCount)
		{
		case 1:		//����
		{
			BYTE cbIndex = AnalyseResult.cbSignedCount++;
			AnalyseResult.cbSignedCardData[cbIndex*cbSameCount] = bCardData[i];
			break;
		}
		case 2:		//����
		{
			BYTE cbIndex = AnalyseResult.cbDoubleCount++;
			AnalyseResult.cbDoubleCardData[cbIndex*cbSameCount] = bCardData[i];
			AnalyseResult.cbDoubleCardData[cbIndex*cbSameCount + 1] = bCardData[i + 1];
			break;
		}
		case 3:		//����
		{
			BYTE cbIndex = AnalyseResult.cbThreeCount++;
			AnalyseResult.cbThreeCardData[cbIndex*cbSameCount] = bCardData[i];
			AnalyseResult.cbThreeCardData[cbIndex*cbSameCount + 1] = bCardData[i + 1];
			AnalyseResult.cbThreeCardData[cbIndex*cbSameCount + 2] = bCardData[i + 2];
			break;
		}
		case 4:		//����
		{
			BYTE cbIndex = AnalyseResult.cbFourCount++;
			AnalyseResult.cbFourCardData[cbIndex*cbSameCount] = bCardData[i];
			AnalyseResult.cbFourCardData[cbIndex*cbSameCount + 1] = bCardData[i + 1];
			AnalyseResult.cbFourCardData[cbIndex*cbSameCount + 2] = bCardData[i + 2];
			AnalyseResult.cbFourCardData[cbIndex*cbSameCount + 3] = bCardData[i + 3];
			break;
		}
		}

		//��������
		i += cbSameCount - 1;
	}


	return;
}

//��ȡ���ͽ��
int CGameData::GetType(tagAnalyseResult AnalyseResult, const BYTE cbCardData[], BYTE cbCardCount, const BYTE NoChangeCard[])
{
	BYTE bLaiZiCount = GetMagicCardNum(NoChangeCard, cbCardCount);

	BYTE bCardData[MAX_CARD_COUNT];
	ZeroMemory(&bCardData, sizeof(bCardData));
	CopyMemory(bCardData, cbCardData, sizeof(BYTE)*cbCardCount);

	//����
	SortCardListNoLaiZi(bCardData, cbCardCount, ST_ORDER);

	//������
	switch (cbCardCount)
	{
	case 0:	//����
	{
		return CT_ERROR;
	}
	case 1: //����
	{
		return CT_SINGLE;
	}
	case 2:	//���ƻ��
	{
		//�����ж�
		if ((bCardData[0] == 0x4F) && (bCardData[1] == 0x4E)) return CT_MISSILE_CARD;
		if (GetCardLogicValue(bCardData[0]) == GetCardLogicValue(bCardData[1])) return CT_DOUBLE;

		return CT_ERROR;
	}
	}

	//�����ж�
	if (AnalyseResult.cbFourCount > 0)
	{
		//�����ж�
		if ((AnalyseResult.cbFourCount == 1) && (cbCardCount == 4) && (bLaiZiCount == 0)) return CT_BOMB_CARD;
		if ((AnalyseResult.cbFourCount == 1) && (bLaiZiCount == 4)) return CT_LAIZI_BOMB;
		if ((AnalyseResult.cbFourCount == 1) && (cbCardCount == 4) && (bLaiZiCount != 0)) return CT_RUAN_BOMB;
		if ((AnalyseResult.cbFourCount == 1) && ((AnalyseResult.cbSignedCount == 2) || (AnalyseResult.cbDoubleCount == 1)) && (cbCardCount == 6)) return CT_FOUR_LINE_TAKE_ONE;
		if ((AnalyseResult.cbFourCount == 1) && (AnalyseResult.cbDoubleCount == 2) && (cbCardCount == 8)) return CT_FOUR_LINE_TAKE_TWO;


		return CT_ERROR;
	}

	//�����ж�
	if (AnalyseResult.cbThreeCount > 0)
	{
		//��������
		if (AnalyseResult.cbThreeCount == 1 && cbCardCount == 3)
			return CT_THREE;

		//�����ж�
		if (AnalyseResult.cbThreeCount > 1)
		{
			//��������
			BYTE bCardData = AnalyseResult.cbThreeCardData[0];
			BYTE cbFirstLogicValue = GetCardLogicValue(bCardData);

			//�������
			if (cbFirstLogicValue >= 15) return CT_ERROR;

			//�����ж�
			for (BYTE i = 1; i < AnalyseResult.cbThreeCount; i++)
			{
				BYTE bCardData = AnalyseResult.cbThreeCardData[i * 3];
				if (cbFirstLogicValue != (GetCardLogicValue(bCardData) + i))
					return CT_ERROR;
			}
		}

		//�����ж�
		if (AnalyseResult.cbThreeCount * 3 == cbCardCount) return CT_THREE_LINE;
		if (AnalyseResult.cbThreeCount * 4 == cbCardCount && (AnalyseResult.cbSignedCount == cbCardCount % 3 || AnalyseResult.cbDoubleCount == cbCardCount % 3 / 2)) return CT_THREE_LINE_TAKE_ONE;
		if ((AnalyseResult.cbThreeCount * 5 == cbCardCount) && (AnalyseResult.cbDoubleCount == AnalyseResult.cbThreeCount)) return CT_THREE_LINE_TAKE_TWO;

		return CT_ERROR;
	}

	//��������
	if (AnalyseResult.cbDoubleCount >= 3)
	{
		//��������
		BYTE bCardData = AnalyseResult.cbDoubleCardData[0];
		BYTE cbFirstLogicValue = GetCardLogicValue(bCardData);

		/*cout << "���Ե�һ�����߼�ֵ��" << (int)cbFirstLogicValue << endl;
		cout << "���һ�����߼�ֵ: " << (int)GetCardLogicValue(AnalyseResult.cbDoubleCardData[AnalyseResult.cbDoubleCount*2 - 1]) << endl;*/

		//�������
		if (cbFirstLogicValue >= 15) return CT_ERROR;

		//�����ж�
		for (BYTE i = 1; i < AnalyseResult.cbDoubleCount; i++)
		{
			BYTE bCardData = AnalyseResult.cbDoubleCardData[i * 2];
			if (cbFirstLogicValue != (GetCardLogicValue(bCardData) + i))
				return CT_ERROR;
		}

		//�����ж�
		if ((AnalyseResult.cbDoubleCount * 2) == cbCardCount)
			return CT_DOUBLE_LINE;

		return CT_ERROR;
	}

	//�����ж�
	if ((AnalyseResult.cbSignedCount >= 5) && (AnalyseResult.cbSignedCount == cbCardCount))
	{
		//��������
		BYTE bCardData = AnalyseResult.cbSignedCardData[0];
		BYTE cbFirstLogicValue = GetCardLogicValue(bCardData);

		//�������
		if (cbFirstLogicValue >= 15) return CT_ERROR;

		//�����ж�
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

//�жϵ�ǰ����Ƿ��ܳ��� 
bool CGameData::JudgePlayerOutCard(WORD wCurOutCardUser)      //lih
{
	//��ø�������������
	BYTE TurnCardNum = m_playing_para.turn_cards_num;
	BYTE TurnCardData[MAX_CARD_COUNT];
	ZeroMemory(&TurnCardData, sizeof(TurnCardData));
	CopyMemory(TurnCardData, m_playing_para.turn_max_cards, sizeof(TurnCardData));

	//��ȡ��ǰ��ҵ���������
	BYTE CurCardNum = m_players_config.players[wCurOutCardUser].hand_card_num;
	BYTE CurCardData[MAX_CARD_COUNT];
	ZeroMemory(&CurCardData, sizeof(CurCardData));

	for (int i = 0; i < CurCardNum; i++)
	{
		CurCardData[i] = m_players_config.hand_cards[wCurOutCardUser][i];
	}

	//���������˿�����
	tagAnalyseResult TurnResult;
	tagAnalyseResult CurResult;

	ZeroMemory(&TurnResult, sizeof(TurnResult));
	ZeroMemory(&CurResult, sizeof(CurResult));

	AnalysebCardData(CurCardData, CurCardNum, CurResult);
	AnalysebCardData(TurnCardData, TurnCardNum, TurnResult);

	//��ȡ������ҵ�����
	int TurnCardType = GetType(TurnResult, TurnCardData, TurnCardNum, TurnCardData);
	TurnCardType = TransListToInt(0, TurnCardType);

	//�Ա��ж�����
	if (TurnCardType == CT_ERROR)	return true;
	if ((TurnCardType & (1 << CT_MISSILE_CARD)) != 0)    return false;    //��ը�ж�

																		  //ը���ж�
	if ((TurnCardType & (1 << CT_BOMB_CARD)) != 0)   //�ϼҳ�ը��
	{
		cout << "ը��" << endl;

		//��ը�ж�
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//��ȡ�ϼҳ�����ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbFourCardData[0]);
		int index = 0;   //������ʶ

		if (CurResult.cbFourCount == 0)    return false;

		for (int i = 0; i < CurResult.cbFourCount; i++)
		{
			if (GetCardLogicValue(CurResult.cbFourCardData[index]) > TurnLogicValue)		return true;
			index += 4;
		}
	}
	else if ((TurnCardType & (1 << CT_SINGLE)) != 0)   //����
	{
		cout << "����" << endl;

		//��ը�ж�
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//ը���ж�
		if (CurResult.cbFourCount != 0)		return true;

		//��ȡ�ϼҳ�����ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbSignedCardData[0]);

		cout << "��ȡ�ϼҳ�����ֵ:" << (int)TurnLogicValue << endl;

		if (GetCardLogicValue(CurCardData[0]) > TurnLogicValue)
			return true;
	}
	else if ((TurnCardType & (1 << CT_DOUBLE)) != 0)   //����
	{
		cout << "����" << endl;

		//��ը�ж�
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//ը���ж�
		if (CurResult.cbFourCount != 0)		return true;

		//��ȡ�ϼҳ�����ֵ
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
	else if ((TurnCardType & (1 << CT_THREE)) != 0)   //����
	{
		cout << "����" << endl;

		//��ը�ж�
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//ը���ж�
		if (CurResult.cbFourCount != 0)		return true;

		//��ȡ�ϼҳ�����ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);

		for (int i = 0; i < CurResult.cbThreeCount; i++)
		{
			int index = 0;
			if (GetCardLogicValue(CurResult.cbThreeCardData[index]) > TurnLogicValue)
				return true;

			index += 3;
		}
	}
	else if ((TurnCardType & (1 << CT_SINGLE_LINE)) != 0)   //˳��
	{
		cout << "˳��" << endl;

		//��ը�ж�
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//ը���ж�
		if (CurResult.cbFourCount != 0)		return true;

		//�Ʋ�������
		if (CurCardNum < 5)		return false;

		//��ȡ�ϼҳ�����ֵ  ���ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbSignedCardData[0]);
		cout << "˳��:��ȡ�ϼҳ�����ֵ:" << (int)TurnLogicValue << endl;

		int flag = 0;

		for (int i = 0; i < CurCardNum; i++)     //����˳������
		{
			//printf("��%d���Ƶ�������־��%d\n", i, flag);
			if (flag != 0)    //�������ñ��ƴ�С
			{
				if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
				{
					continue;
				}
				if (GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //�ж��Ƿ�����
				{
					flag++;

					//��������
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
					if (GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //�ж��Ƿ�����
					{
						flag++;

						//��������
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
	else if ((TurnCardType & (1 << CT_DOUBLE_LINE)) != 0)   //����
	{
		cout << "����" << endl;

		//��ը�ж�
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//ը���ж�
		if (CurResult.cbFourCount != 0)		return true;

		//�Ʋ�������
		if (CurCardNum < 6)		return false;

		//��ȡ�ϼҳ�����ֵ  ���ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbDoubleCardData[0]);
		int flag = 0;   //�ж��Ƿ�������־
		int Numflag = 0;   //�ж������Ƿ���ı�־

		for (int i = 0; i < CurCardNum; i++)     //������������
		{
			if (flag != 0)    //�������ñ��ƴ�С
			{
				if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
				{
					Numflag++;
					continue;
				}
				if ((GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1))
					&& (Numflag > 0) && (GetCardLogicValue(CurCardData[i]) < 15))   //�ж��Ƿ�����
				{
					flag++;

					//��������
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
						&& (Numflag > 0) && (GetCardLogicValue(CurCardData[i]) < 15))   //�ж��Ƿ�����
					{
						flag++;

						//��������
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
	else if ((TurnCardType & (1 << CT_THREE_LINE)) != 0)   //����
	{
		cout << "����" << endl;

		//��ը�ж�
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//ը���ж�
		if (CurResult.cbFourCount != 0)		return true;

		//�Ʋ�������
		if (CurCardNum < 6)		return false;

		//��ȡ�ϼҳ�����ֵ  ���ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);
		int flag = 0;   //�ж��Ƿ�������־
		int Numflag = 0;   //�ж������Ƿ���ı�־

		for (int i = 0; i < CurCardNum; i++)     //������������
		{
			if (flag != 0)    //�������ñ��ƴ�С
			{
				if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
				{
					Numflag++;
					continue;
				}
				if ((GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1))
					&& (Numflag > 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //�ж��Ƿ�����
				{
					flag++;

					//��������
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
						&& (Numflag > 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //�ж��Ƿ�����
					{
						flag++;

						//��������
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
	else if ((TurnCardType & (1 << CT_THREE_LINE_TAKE_ONE)) != 0)   //����һ
	{

		cout << "����һ" << endl;
		//��ը�ж�
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//ը���ж�
		if (CurResult.cbFourCount != 0)		return true;

		//�Ʋ�������
		if (CurCardNum < 4 * TurnResult.cbThreeCount)		return false;

		//��ȡ�ϼҳ�����ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);

		if (TurnResult.cbThreeCount == 1)    //����һ
		{
			int index = 0;
			for (int i = 0; i < CurResult.cbThreeCount; i++)
			{
				if (GetCardLogicValue(CurResult.cbThreeCardData[index]) > TurnLogicValue)
					return true;

				index += 3;
			}
		}
		else		//�ɻ�
		{
			int index = 0;
			int flag = 1;   //�жϷɻ��������ŵı�־
							//�ж������Ƿ�����
			for (int i = 0; i < CurResult.cbThreeCount; i++)
			{
				if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) - 1) == (GetCardLogicValue(CurResult.cbThreeCardData[index + 3])))  //����
				{
					flag++;
				}

				if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) >(TurnLogicValue - TurnResult.cbThreeCount + 2))
					&& (flag == TurnResult.cbThreeCount))   //��С�Ƚ�
					return true;

				index += 3;
			}
		}
	}
	else if ((TurnCardType & (1 << CT_THREE_LINE_TAKE_TWO)) != 0)   //������
	{
		cout << "������" << endl;

		//��ը�ж�
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//ը���ж�
		if (CurResult.cbFourCount != 0)		return true;

		//�Ʋ�������
		if (CurCardNum < 5 * TurnResult.cbThreeCount)		return false;

		//û�ж���
		if (TurnResult.cbThreeCount < 2 && TurnResult.cbDoubleCount == 0)	return false;

		//��ȡ�ϼҳ�����ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);

		if (TurnResult.cbThreeCount == 1)    //������
		{
			int index = 0;
			for (int i = 0; i < CurResult.cbThreeCount; i++)
			{
				if (GetCardLogicValue(CurResult.cbThreeCardData[index]) > TurnLogicValue)
					return true;

				index += 3;
			}
		}
		else		//�ɻ�
		{
			int index = 0;
			int flag = 1;   //�жϷɻ��������ŵı�־
							//�ж������Ƿ�����
			for (int i = 0; i < CurResult.cbThreeCount; i++)
			{
				if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) - 1) == (GetCardLogicValue(CurResult.cbThreeCardData[index + 3])))  //����
				{
					flag++;
				}

				if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) >(TurnLogicValue - TurnResult.cbThreeCount + 2))
					&& (flag == TurnResult.cbThreeCount))   //��С�Ƚ�
					return true;

				index += 3;
			}
		}
	}
	else if ((TurnCardType & (1 << CT_FOUR_LINE_TAKE_ONE)) != 0)   //�Ĵ���
	{
		cout << "�Ĵ���" << endl;

		//��ը�ж�
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//ը���ж�
		if (CurResult.cbFourCount != 0)		return true;

	}
	else if ((TurnCardType & (1 << CT_FOUR_LINE_TAKE_TWO)) != 0)   //�Ĵ�����
	{
		cout << "�Ĵ�����" << endl;

		//��ը�ж�
		if ((GetCardLogicValue(CurCardData[0]) == 17) && (GetCardLogicValue(CurCardData[1]) == 16))		return true;

		//ը���ж�
		if (CurResult.cbFourCount != 0)		return true;

	}

	return false;
}

//��ҵ�һ������  �ж������Գ��ĺ��ʵ���
bool CGameData::AnalysePlayerOutCardFirst(WORD OutCardUsr, BYTE OutCardData[], BYTE *OutCardNum)
{
	cout << "func AnalysePlayerOutCardFirst() Begin::::" << endl;

	//У��
	if (OutCardData == NULL || OutCardUsr == INVALID_CHAIR)
	{
		return false;
	}

	//����ֵ
	BYTE tmpOutCardData[MAX_CARD_COUNT];
	BYTE tmpOutCardNum = 0;
	ZeroMemory(&tmpOutCardData, sizeof(tmpOutCardData));

	//��ȡ��ǰ��ҵ���������
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

	//NoLaiZi������
	SortCardListNoLaiZi(CurCardData, CurCardNum, ST_ORDER);

	//���������˿�����
	tagAnalyseResult CurResult;
	ZeroMemory(&CurResult, sizeof(CurResult));

	AnalysebCardData(CurCardData, CurCardNum, CurResult);

	//�ж��ܳ�����  ���ȳ���С����ֵ  ը������

	cout << "��ʼ���бȽ�" << endl;

	//�����С����Ϊը��
	if (CurResult.cbFourCount != 0)
	{
		BYTE MinData = GetCardLogicValue(CurCardData[CurCardNum - 1]);		//��ȡ��С��
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
	//�Ƚ���С����
	if (CurResult.cbSignedCount != 0 && CurResult.cbDoubleCount != 0 && CurResult.cbThreeCount != 0)
	{
		cout << "1";

		BYTE MinData = GetCardLogicValue(CurCardData[CurCardNum - 1]);		//��ȡ��С��
		cout << "MinData:" << (int)MinData << endl;
		cout << "cbDoubleCardData:" << (int)GetCardLogicValue(CurResult.cbDoubleCardData[CurResult.cbDoubleCount * 2 - 1]) << endl;
		cout << "cbThreeCardData:" << (int)GetCardLogicValue(CurResult.cbThreeCardData[CurResult.cbThreeCount * 3 - 1]) << endl;
		if (MinData == GetCardLogicValue(CurResult.cbSignedCardData[CurResult.cbSignedCount - 1]))		//����
		{
			cout << "1.1";
			tmpOutCardData[0] = CurResult.cbSignedCardData[CurResult.cbSignedCount - 1];
			tmpOutCardNum = 1;
		}
		else if (MinData == GetCardLogicValue(CurResult.cbDoubleCardData[CurResult.cbDoubleCount * 2 - 1]))   //����
		{
			cout << "1.2";
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurResult.cbDoubleCardData[CurResult.cbDoubleCount * 2 - 1 - i];

			tmpOutCardNum = 2;
		}
		else if (MinData == GetCardLogicValue(CurResult.cbThreeCardData[CurResult.cbThreeCount * 3 - 1]))		//����һ
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
		BYTE MinData = GetCardLogicValue(CurCardData[CurCardNum - 1]);		//��ȡ��С��

		if (MinData == GetCardLogicValue(CurResult.cbDoubleCardData[CurResult.cbDoubleCount * 2 - 1]))   //����
		{
			for (int i = 0; i < 2; i++)
				tmpOutCardData[i] = CurResult.cbDoubleCardData[CurResult.cbDoubleCount * 2 - 1 - i];

			tmpOutCardNum = 2;
		}
		else if (MinData == GetCardLogicValue(CurResult.cbThreeCardData[CurResult.cbThreeCount * 3 - 1]))		//������
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
		BYTE MinData = GetCardLogicValue(CurCardData[CurCardNum - 1]);		//��ȡ��С��

		if (MinData == GetCardLogicValue(CurResult.cbSignedCardData[CurResult.cbSignedCount - 1]))		//����
		{
			tmpOutCardData[0] = CurResult.cbSignedCardData[CurResult.cbSignedCount - 1];
			tmpOutCardNum = 1;
		}
		else if (MinData == GetCardLogicValue(CurResult.cbDoubleCardData[CurResult.cbDoubleCount * 2 - 1]))   //����
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
		BYTE MinData = GetCardLogicValue(CurCardData[CurCardNum - 1]);		//��ȡ��С��
		if (MinData == GetCardLogicValue(CurResult.cbSignedCardData[CurResult.cbSignedCount - 1]))		//����
		{
			tmpOutCardData[0] = CurResult.cbSignedCardData[CurResult.cbSignedCount - 1];
			tmpOutCardNum = 1;
		}

		else if (MinData == GetCardLogicValue(CurResult.cbThreeCardData[CurResult.cbThreeCount * 3 - 1]))		//����һ
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

//��ҽ���  �ж������Գ�����
bool CGameData::AnalysePlayerOutCard(WORD OutCardUsr, BYTE OutCardData[], BYTE *OutCardNum)
{
	//����ֵ
	BYTE tmpOutCardData[MAX_CARD_COUNT];
	BYTE tmpOutCardNum = 0;
	ZeroMemory(&tmpOutCardData, sizeof(tmpOutCardData));

	//��ø�������������
	BYTE TurnCardNum = m_playing_para.turn_cards_num;
	BYTE TurnCardData[MAX_CARD_COUNT];
	ZeroMemory(&TurnCardData, sizeof(TurnCardData));
	CopyMemory(TurnCardData, m_playing_para.turn_max_cards, sizeof(TurnCardData));

	//��ȡ��ǰ��ҵ���������
	BYTE CurCardNum = m_players_config.players[OutCardUsr].hand_card_num;
	BYTE CurCardData[MAX_CARD_COUNT];
	ZeroMemory(&CurCardData, sizeof(CurCardData));

	for (int i = 0; i < CurCardNum; i++)
	{
		CurCardData[i] = m_players_config.hand_cards[OutCardUsr][i];
	}

	//���������˿�����
	tagAnalyseResult TurnResult;
	tagAnalyseResult CurResult;

	ZeroMemory(&TurnResult, sizeof(TurnResult));
	ZeroMemory(&CurResult, sizeof(CurResult));

	AnalysebCardData(CurCardData, CurCardNum, CurResult);
	AnalysebCardData(TurnCardData, TurnCardNum, TurnResult);

	//��ȡ������ҵ�����
	int TurnCardType = GetType(TurnResult, TurnCardData, TurnCardNum, TurnCardData);
	TurnCardType = TransListToInt(0, TurnCardType);

	//�Ա��ж�����
	if (TurnCardType == CT_ERROR)	return false;
	if ((TurnCardType & (1 << CT_MISSILE_CARD)) != 0)    return false;    //��ը�ж�

																		  //ը���ж�
	if ((TurnCardType & (1 << CT_BOMB_CARD)) != 0)   //�ϼҳ�ը��
	{
		cout << "ը��" << endl;

		//��ȡ�ϼҳ�����ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbFourCardData[0]);
		int index = 0;   //������ʶ

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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_SINGLE)) != 0)   //����
	{
		cout << "����" << endl;

		//��ȡ�ϼҳ�����ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbSignedCardData[0]);

		cout << "��ȡ�ϼҳ�����ֵ:" << (int)TurnLogicValue << endl;

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

		//ը���ж�
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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_DOUBLE)) != 0)   //����
	{
		cout << "����" << endl;

		//��ȡ�ϼҳ�����ֵ
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

		//ը���ж�
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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_THREE)) != 0)   //����
	{
		cout << "����" << endl;

		//��ȡ�ϼҳ�����ֵ
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

		//ը���ж�
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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_SINGLE_LINE)) != 0)   //˳��
	{
		cout << "˳��" << endl;

		//��ȡ�ϼҳ�����ֵ  ���ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbSignedCardData[0]);
		cout << "˳��:��ȡ�ϼҳ�����ֵ:" << (int)TurnLogicValue << endl;

		int flag = 0;
		int index = 0;

		for (int i = 0; i < CurCardNum; i++)     //����˳������
		{
			printf("��%d���Ƶ�������־��%d\n", i, flag);
			if (flag != 0)    //�������ñ��ƴ�С
			{
				if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
				{
					continue;
				}
				if (GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //�ж��Ƿ�����
				{
					tmpOutCardData[index++] = CurCardData[i];
					tmpOutCardNum++;

					flag++;

					//��������
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
					if (GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //�ж��Ƿ�����
					{
						tmpOutCardData[index++] = CurCardData[i];
						tmpOutCardNum++;

						flag++;

						//��������
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

		//ը���ж�
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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_DOUBLE_LINE)) != 0)   //����
	{
		cout << "����" << endl;


		//��ȡ�ϼҳ�����ֵ  ���ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbDoubleCardData[0]);
		int flag = 0;   //�ж��Ƿ�������־
		int Numflag = 0;   //�ж������Ƿ���ı�־
		int index = 0;

		for (int i = 0; i < CurCardNum; i++)     //������������
		{
			if (flag != 0)    //�������ñ��ƴ�С
			{
				if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
				{
					Numflag++;
					continue;
				}
				if ((GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1))
					&& (Numflag > 0) && (GetCardLogicValue(CurCardData[i]) < 15))   //�ж��Ƿ�����
				{
					tmpOutCardData[index++] = CurCardData[i];		//���Լ�����
					tmpOutCardNum++;

					tmpOutCardData[index++] = CurCardData[i - 1];
					tmpOutCardNum++;

					flag++;

					//��������
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
						&& (Numflag > 0) && (GetCardLogicValue(CurCardData[i]) < 15))   //�ж��Ƿ�����
					{
						tmpOutCardData[index++] = CurCardData[i];		//���Լ�����
						tmpOutCardNum++;

						tmpOutCardData[index++] = CurCardData[i - 1];
						tmpOutCardNum++;

						flag++;

						//��������
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

		//ը���ж�
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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_THREE_LINE)) != 0)   //����
	{
		cout << "����" << endl;

		//��ȡ�ϼҳ�����ֵ  ���ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);
		int flag = 0;   //�ж��Ƿ�������־
		int Numflag = 0;   //�ж������Ƿ���ı�־
		int index = 0;

		for (int i = 0; i < CurCardNum; i++)     //������������
		{
			if (flag != 0)    //�������ñ��ƴ�С
			{
				if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
				{
					Numflag++;
					continue;
				}
				if ((GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1))
					&& (Numflag > 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //�ж��Ƿ�����
				{
					tmpOutCardData[index++] = CurCardData[i];		//����������
					tmpOutCardNum++;

					tmpOutCardData[index++] = CurCardData[i - 1];
					tmpOutCardNum++;

					tmpOutCardData[index++] = CurCardData[i - 2];
					tmpOutCardNum++;

					flag++;

					//��������
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

				if (GetCardLogicValue(CurCardData[i + 1]) != GetCardLogicValue(CurCardData[i]))   //����ͬ���� ��־λ����
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
						&& (Numflag > 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //�ж��Ƿ�����
					{
						tmpOutCardData[index++] = CurCardData[i];		//����������
						tmpOutCardNum++;

						tmpOutCardData[index++] = CurCardData[i - 1];
						tmpOutCardNum++;

						tmpOutCardData[index++] = CurCardData[i - 2];
						tmpOutCardNum++;

						flag++;

						//��������
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

					if (GetCardLogicValue(CurCardData[i + 1]) != GetCardLogicValue(CurCardData[i]))   //����ͬ���� ��־λ����
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

		//ը���ж�
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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_THREE_LINE_TAKE_ONE)) != 0)   //����һ
	{

		cout << "����һ" << endl;


		//��ȡ�ϼҳ�����ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);

		if (CurResult.cbSignedCount >= TurnResult.cbThreeCount)     //û�е��ž�û��Ҫ�������
		{
			if (TurnResult.cbThreeCount == 1)    //����һ
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
			else		//�ɻ�
			{
				int index = 0;
				int flag = 1;   //�жϷɻ��������ŵı�־
								//�ж������Ƿ�����
				for (int i = 0; i < CurResult.cbThreeCount; i++)
				{
					if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) - 1) == (GetCardLogicValue(CurResult.cbThreeCardData[index + 3])))  //����
					{
						flag++;
					}

					if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) >(TurnLogicValue - TurnResult.cbThreeCount + 2))
						&& (flag == TurnResult.cbThreeCount))   //��С�Ƚ�
					{
						index = index - TurnResult.cbThreeCount * 3 + 3;     //��ֵ���±�
						for (int j = 0; j < TurnResult.cbThreeCount * 3; j++)
						{
							tmpOutCardData[j] = CurResult.cbThreeCardData[index++];
						}

						for (int j = 0; j < TurnResult.cbThreeCount; j++)
						{
							tmpOutCardData[TurnResult.cbThreeCount * 3 + j] = CurResult.cbSignedCardData[TurnResult.cbSignedCount - j - 1];   //�ɻ����ĵ�
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

		//ը���ж�
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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_THREE_LINE_TAKE_TWO)) != 0)   //������
	{
		cout << "������" << endl;

		//��ȡ�ϼҳ�����ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);

		if (CurResult.cbDoubleCount >= TurnResult.cbThreeCount)     //û�ж��Ӿ�û��Ҫ�������
		{
			if (TurnResult.cbThreeCount == 1)    //������
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
			else		//�ɻ�
			{
				int index = 0;
				int flag = 1;   //�жϷɻ��������ŵı�־
								//�ж������Ƿ�����
				for (int i = 0; i < CurResult.cbThreeCount; i++)
				{
					if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) - 1) == (GetCardLogicValue(CurResult.cbThreeCardData[index + 3])))  //����
					{
						flag++;
					}

					if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) >(TurnLogicValue - TurnResult.cbThreeCount + 2))
						&& (flag == TurnResult.cbThreeCount))   //��С�Ƚ�
					{
						index = index - TurnResult.cbThreeCount * 3 + 3;     //��ֵ���±�
						for (int j = 0; j < TurnResult.cbThreeCount * 3; j++)
						{
							tmpOutCardData[j] = CurResult.cbThreeCardData[index++];
						}

						for (int j = 0; j < TurnResult.cbThreeCount * 2; j += 2)
						{
							tmpOutCardData[TurnResult.cbThreeCount * 3 + j] = CurResult.cbDoubleCardData[TurnResult.cbDoubleCount * 2 - j - 1];   //�ɻ����Ķ�
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

		//ը���ж�
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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_FOUR_LINE_TAKE_ONE)) != 0)   //�Ĵ���
	{
		cout << "�Ĵ���" << endl;

		//ը���ж�
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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_FOUR_LINE_TAKE_TWO)) != 0)   //�Ĵ�����
	{
		cout << "�Ĵ�����" << endl;

		//ը���ж�
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

		//��ը�ж�
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

//������
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

// ����÷�
int CGameData::Settle()
{
	return 0;
}

// ����С����Ϸ����
int CGameData::ClearXjGame()
{
	cout << "����С������" << endl;

	// ����С���������
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

	//������Ϸ����
	m_playing_para.appointbanker = INVALID_CHAIR;
	m_playing_para.game_state = GS_WK_FREE;
	m_playing_para.cur_total_bet = 0;
	m_playing_para.room_bet = 1;
	m_playing_para.turn_cards_num = 0;   //���������ݳ�ʼ��
	m_playing_para.jiaofen_count = 0;	 //��ʼ���зִ���
	m_playing_para.leave_card_bet = 1;

	//����Ȩλ���
	for (BYTE i = 0; i < LEN_SORT_RIGHT; i++)
		m_playing_para.sort_right[i] = i * COLOR_RIGHT;

	//����ɫ�ͼ������
	//m_playing_para.level_value = VALUE_ERROR;	TODO ���費��Ҫ������������ÿ�μ��ƶ�Ϊ2
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

// ��������Ϸ����
int  CGameData::ClearDjGame()
{
	// �������������
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

	//������Ϸ����
	m_playing_para.liangzhu_user = INVALID_CHAIR;
	m_playing_para.last_winner_id = INVALID_CHAIR;
	m_playing_para.game_state = GS_WK_FREE;
	m_playing_para.cur_fanzhu_card = INVALID_CARD;

	return 0;
}

// ����������
int CGameData::ClearRoom()
{
	if (m_card_config.game_cards != NULL)
		delete[] m_card_config.game_cards;
	if (m_players_config.players != NULL)
		delete[] m_players_config.players;

	return 0;
}

//����ɫȨλ�����˿�  //��������
void CGameData::SortCardList(BYTE *cbCardData, BYTE cbCardCount)
{
	//������������
	BYTE cbSortValue[MAX_CARD_COUNT];
	for (BYTE i = 0; i < cbCardCount; i++)
		cbSortValue[i] = GetCardSortOrder(cbCardData[i]);

	//�������
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
				//����λ��
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

//����ɫȨλ���û���������
void CGameData::SortCardList(const WORD &wChairID, BYTE cbCardCount, BYTE cbSortType)
{
	//��������
	SortCardList(m_players_config.hand_cards[wChairID], cbCardCount, cbSortType);
}

//�����˿�
void CGameData::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
	//��Ŀ����
	if (cbCardCount == 0) return;

	//ת����ֵ
	BYTE cbSortValue[MAX_CARD_COUNT];
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		cbSortValue[i] = GetCardLogicValueLaiZi(cbCardData[i]);
	}

	//�������
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
				//����λ��
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

	//��Ŀ����
	if (cbSortType == ST_COUNT)
	{
		//�����˿�
		BYTE cbIndex = 0;
		tagAnalyseResult AnalyseResult;
		AnalysebCardData(cbCardData, cbCardCount, AnalyseResult);

		//��������
		CopyMemory(&cbCardData[cbIndex], AnalyseResult.cbFourCardData, sizeof(BYTE)*AnalyseResult.cbFourCount * 4);
		cbIndex += AnalyseResult.cbFourCount * 4;

		//��������
		CopyMemory(&cbCardData[cbIndex], AnalyseResult.cbThreeCardData, sizeof(BYTE)*AnalyseResult.cbThreeCount * 3);
		cbIndex += AnalyseResult.cbThreeCount * 3;

		//��������
		CopyMemory(&cbCardData[cbIndex], AnalyseResult.cbDoubleCardData, sizeof(BYTE)*AnalyseResult.cbDoubleCount * 2);
		cbIndex += AnalyseResult.cbDoubleCount * 2;

		//��������
		CopyMemory(&cbCardData[cbIndex], AnalyseResult.cbSignedCardData, sizeof(BYTE)*AnalyseResult.cbSignedCount);
		cbIndex += AnalyseResult.cbSignedCount;
	}

	return;
}

//��ÿ��Ƶ�����ȼ�
BYTE CGameData::GetCardSortOrder(BYTE cbCardData)
{
	//���˵���Ч����
	if (INVALID_CARD == cbCardData)
		return 0;

	//�߼���ֵ
	BYTE cbCardColor = GetCardColor(cbCardData);
	BYTE cbCardValue = GetCardValue(cbCardData);

	//����
	if ((SMALL_JOKER == cbCardData) || (BIG_JOKER == cbCardData))
	{
		BYTE cbSortValueTmp = cbCardValue + 14;
		return (cbSortValueTmp + m_playing_para.sort_right[4]);
	}

	//������Ϊ����ɫ����ڼ��ƣ�����ɫ�任
	BYTE cbSortColor = cbCardColor;	//��ɫ��1��ʼ

									//������ֵΪA��A��������ǰ����������ֵ�任
	BYTE cbSortValue = cbCardValue;
	if (cbCardValue == 1)
		cbSortValue += 13;

	//����Ϊ��ӣ���ӷ�������ߣ���λ���					//��һ�ɲ��������
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

//��ÿ��Ƶ��߼���ֵ
BYTE CGameData::GetCardLogicValue(BYTE cbCardData)
{
	if (0 == cbCardData)
	{
		return 0;
	}

	//�߼���ֵ
	BYTE cbCardColor = GetCardColor(cbCardData);
	BYTE cbCardValue = GetCardValue(cbCardData);

	//ת����ֵ���Գ��ִ���
	if ((cbCardColor << 4) == 0x40) return cbCardValue + 2;		//��С��    

	return (cbCardValue <= 2) ? (cbCardValue + 13) : cbCardValue;
}

//��������Ƿ�������
bool CGameData::AnalyseUserLiangZhu(const WORD &wChairID, BYTE *cbLiangZhuColor)
{
	//У��
	if ((VALUE_ERROR == m_playing_para.level_value) ||
		(NULL == cbLiangZhuColor))
		return false;

	//��ʱ������ɫ����
	std::vector <BYTE> vecLZcolor;

	//�ж�����������Ƿ��е�ǰ����
	for (int i = 0; i < m_card_config.init_card_num; i++)
	{
		BYTE cbCardValue = GetCardValue(m_players_config.hand_cards[wChairID][i]);
		if (cbCardValue == m_playing_para.level_value)
		{
			//ȥ���ظ�������
			std::vector <BYTE>::iterator result = find(vecLZcolor.begin(), vecLZcolor.end(), cbCardValue);
			if (result == vecLZcolor.end())
			{
				BYTE cbCardColor = GetCardColor(m_players_config.hand_cards[wChairID][i]);
				vecLZcolor.push_back(cbCardColor);
			}
		}
	}

	//�����������Ŀ���
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

//��������Ƿ��ܷ�������������λ˳��
bool CGameData::AnalyseUserFanZhu(const WORD &wChairID, BYTE *cbFanZhuCard)
{
	//��ǰ��������
	BYTE cbCurFanzhuCard = m_playing_para.cur_fanzhu_card;
	BYTE cbCurFanZhuColor = GetCardColor(m_playing_para.cur_fanzhu_card);

	//�������ֻ�е�������ҷ���֮����ܷ���
	if (m_playing_para.appointbanker == wChairID)
	{
		//У���Ƿ�����ҷ���
		if (INVALID_CARD != cbCurFanzhuCard)
		{
			return IsUserCanFanZhu(wChairID, cbCurFanZhuColor, cbFanZhuCard);
		}
	}
	else
	{
		//���˷��������ж����м���
		BYTE cbColorIndex = 0;
		//��ǰ����ҷ���,�������ҷ����Ļ�ɫ������ڸû�ɫ
		if (INVALID_CARD != cbCurFanzhuCard)
		{
			cbColorIndex = cbCurFanZhuColor;
		}

		return IsUserCanFanZhu(wChairID, cbColorIndex, cbFanZhuCard);
	}

	return false;
}

//�ж���������Ƿ��ܷ���
bool CGameData::IsUserCanFanZhu(const WORD &wChairID, const BYTE &cbCurFanZhuColor, BYTE *cbFanZhuCard)
{
	//��ҿ�������
	BYTE cbCardIndex[LEN_MAX_INDEX];
	SwitchToCardIndex(m_players_config.hand_cards[wChairID], MAX_CARD_COUNT, cbCardIndex);

	//��ô��ڵ�ǰ����ɫ�ļ�������
	BYTE cbFanzhuTypeNum = 0;
	for (BYTE i = cbCurFanZhuColor; i < MAX_COLOR_TYPE_NUM; i++)
	{
		//����
		BYTE cbTmpCard = ((i << 4) | m_playing_para.level_value);
		//��������
		BYTE cbCurFanzhuIndex = SwitchToCardIndex(cbTmpCard);

		//�Ƿ�������һ���ļ���
		if (2 == cbCardIndex[cbCurFanzhuIndex])
		{
			if (MAX_FANZHU_TYPE_NUM > cbFanzhuTypeNum)
			{
				cbFanZhuCard[cbFanzhuTypeNum] = cbTmpCard;
				cbFanzhuTypeNum++;
			}
		}
	}

	//�ж�����Ƿ���һ��С��
	if (2 == cbCardIndex[SMALL_JOKER_INDEX])
	{
		if (MAX_FANZHU_TYPE_NUM > cbFanzhuTypeNum)
		{
			cbFanZhuCard[cbFanzhuTypeNum] = SwitchToCardData(SMALL_JOKER_INDEX);
			cbFanzhuTypeNum++;
		}
	}
	//�ж�����Ƿ���һ�Դ���
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

//���͵��Ƹ����
void CGameData::SendLeaveCardToUser(const WORD &wChairID)
{
	//�����Ƹ������
	for (int i = 0; i < MAX_LEAVE_CARD_NUM; i++)
	{
		if (INVALID_CARD != m_playing_para.leave_card[i])
		{
			m_players_config.hand_cards[wChairID][i + m_card_config.init_card_num] = m_playing_para.leave_card[i];
			m_players_config.players[wChairID].hand_card_num++;
		}
	}
}

//���һ�鿨�Ƶ�������
int CGameData::GetCardLogicType(const BYTE *cbCardData, BYTE cbCardCount)
{
	//������
	switch (cbCardCount)
	{
	case 0:	//����
	{
		//cout << "0" << endl;
		return CT_ERROR;
	}
	case 1: //����
	{
		//cout << "1" << endl;
		return TransListToInt(0, CT_SINGLE);
	}
	case 2:	//���ƻ��
	{
		//cout << "2" << endl;
		//�����ж�
		if ((cbCardData[0] == 0x4F) && (cbCardData[1] == 0x4E)) return TransListToInt(0, CT_MISSILE_CARD);   //����һ�� 1<<14
		if (GetCardLogicValueLaiZi(cbCardData[0]) == GetCardLogicValueLaiZi(cbCardData[1])) return TransListToInt(0, CT_DOUBLE);

		//��������
		BYTE LaiZiData[MAX_LAIZI_COUNT] = { 0 };	//��ӵ���ͨ�Ƶ�����
		BYTE LaiZiNum = 0;					//��ӵ���ͨ�Ƶ�����

											//����
		SetLaiZiToCommon(LaiZiData, LaiZiNum);

		//�����ж�
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

						cout << "������ӣ�����" << LaiZiData[0] << endl;
						//�����������ͨ��
						SetLaiZiToCommon(LaiZiData, LaiZiNum);
						return TransListToInt(0, CT_DOUBLE);
					}
				}
			}
		}

		//û����� ����
		SetLaiZiToCommon(LaiZiData, LaiZiNum);

		//printf("�������������жϴ���\n");
		return CT_ERROR;
	}
	}

	//�����˿�
	tagAnalyseResult AnalyseResult;
	BYTE bLaiZiCount = GetMagicCardNum(cbCardData, cbCardCount);
	BYTE bCardDataTemp[MAX_CARD_COUNT];
	int dCardType = CT_ERROR;

	//��������
	BYTE LaiZiData[MAX_LAIZI_COUNT] = { 0 };	//��ӵ���ͨ�Ƶ�����
	BYTE LaiZiNum = 0;					//��ӵ���ͨ�Ƶ�����

										//����
	SetLaiZiToCommon(LaiZiData, LaiZiNum);

	memset(bCardDataTemp, 0, sizeof(bCardDataTemp));
	CopyMemory(bCardDataTemp, cbCardData, sizeof(BYTE)*cbCardCount);
	ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));

	bool SameCard = false;

	//������������ͨ��
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

	//�����ж�
	if (bLaiZiCount != 0 && !SameCard && m_room_config.game_laizi_mode > 0)
	{
		for (BYTE i = 0;i<cbCardCount;i++)
		{
			if (GetCardLogicValueLaiZi(cbCardData[i]) == m_playing_para.magic_card)
			{
				cout << "==============һ�����================" << endl;
				//һ������
				for (BYTE j = 0;j<13;j++)
				{
					bCardDataTemp[i] = m_cbCardData[j];

					//��������
					if (bLaiZiCount >= 2)
					{
						for (BYTE k = i + 1;k<cbCardCount;k++)
						{
							if (GetCardLogicValueLaiZi(cbCardData[k]) == m_playing_para.magic_card)
							{
								cout << "==============�������================" << endl;
								for (BYTE z = 0;z<13;z++)
								{
									bCardDataTemp[k] = m_cbCardData[z];

									//��������
									if (bLaiZiCount >= 3)
									{
										for (BYTE g = k + 1;g<cbCardCount;g++)
										{
											if (GetCardLogicValueLaiZi(cbCardData[g]) == m_playing_para.magic_card)
											{
												cout << "==============�������================" << endl;
												for (BYTE f = 0;f<13;f++)
												{
													bCardDataTemp[g] = m_cbCardData[f];

													//��������
													if (bLaiZiCount == 4)
													{
														for (BYTE h = g + 1;h<cbCardCount;h++)
														{
															if (GetCardLogicValueLaiZi(cbCardData[h]) == m_playing_para.magic_card)
															{
																cout << "==============�������================" << endl;
																for (BYTE l = 0;l<13;l++)
																{
																	if (l == 1)continue;
																	bCardDataTemp[h] = m_cbCardData[l];
																	ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));

																	AnalysebCardData(bCardDataTemp, cbCardCount, AnalyseResult);
																	if (GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData) == 0)
																		continue;

																	//��¼���������
																	LaiZiData[0] = GetCardLogicValue(m_cbCardData[j]);
																	LaiZiData[1] = GetCardLogicValue(m_cbCardData[z]);
																	LaiZiData[2] = GetCardLogicValue(m_cbCardData[f]);
																	LaiZiData[3] = GetCardLogicValue(m_cbCardData[l]);

																	if ((dCardType&(1 << GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData))) == 0)
																		dCardType = TransListToInt(dCardType, GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData));

																	//Ŀǰ���� ը��ֱ�ӳ�
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

													//��¼���������
													LaiZiData[0] = GetCardLogicValue(m_cbCardData[j]);
													LaiZiData[1] = GetCardLogicValue(m_cbCardData[z]);
													LaiZiData[2] = GetCardLogicValue(m_cbCardData[f]);

													if ((dCardType&(1 << GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData))) == 0)
														dCardType = TransListToInt(dCardType, GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData));

													//Ŀǰ���� ը��ֱ�ӳ�
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

									//��¼�ڶ������
									cout << "TWOһ��ӣ�����" << (int)GetCardLogicValue(m_cbCardData[j]) << endl;
									cout << "TWO����ӣ�����" << (int)GetCardLogicValue(m_cbCardData[z]) << endl;
									LaiZiData[0] = GetCardLogicValue(m_cbCardData[j]);
									LaiZiData[1] = GetCardLogicValue(m_cbCardData[z]);

									if ((dCardType&(1 << GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData))) == 0)
										dCardType = TransListToInt(dCardType, GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData));

									//Ŀǰ���� ը��ֱ�ӳ�
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

					//��¼��һ�����
					cout << "ONEһ��ӣ�����" << (int)GetCardLogicValue(m_cbCardData[j]) << endl;
					LaiZiData[0] = GetCardLogicValue(m_cbCardData[j]);

					if ((dCardType&(1 << GetType(AnalyseResult, bCardDataTemp, cbCardCount, cbCardData))) == 0)
						dCardType = TransListToInt(dCardType, GetType(AnalyseResult, cbCardData, cbCardCount, cbCardData));

					//Ŀǰ���� ը��ֱ�ӳ�
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

	//��ȡ����
	for (int i = 0; i < MAX_LAIZI_COUNT; i++)
	{
		if (INVALID_CARD != LaiZiData[i])
		{
			cout << (int)LaiZiData[i];
			LaiZiNum++;
		}
	}
	cout << endl;

	//�����������ͨ��
	SetLaiZiToCommon(LaiZiData, LaiZiNum);

	cout << "LaiZiNum::" << (int)LaiZiNum << endl;

	//��ʱ�������¼�ը�����͵��ж�
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

//��ÿ��Ƶ��߼���ɫ
BYTE CGameData::GetCardLogicColor(BYTE cbCardData)
{
	//�߼���ֵ
	BYTE cbCardColor = GetCardColor(cbCardData);
	BYTE cbCardValue = GetCardValue(cbCardData);

	//��ɫ�ж�
	if (cbCardColor == COLOR_MAIN_CARD) return COLOR_MAIN_CARD;
	if (cbCardColor == m_playing_para.main_color) return COLOR_MAIN_CARD;
	if (cbCardColor == m_playing_para.main_color) return COLOR_MAIN_CARD;

	//��ֵ�ж�
	if (cbCardValue == m_playing_para.nt_value) return COLOR_MAIN_CARD;
	if (cbCardValue == m_playing_para.level_value) return COLOR_MAIN_CARD;

	return cbCardColor;
}

//��ÿ���������߼���ɫ
BYTE CGameData::GetCardLogicColor(const BYTE *cbCardData, BYTE cbCardCount)
{
	//��ȡ���������ͷβ��ɫ
	BYTE cbFirstColor = GetCardLogicColor(cbCardData[0]);
	BYTE cbSecondColor = GetCardLogicColor(cbCardData[cbCardCount - 1]);

	//ͷβ��ȣ��򷵻ػ�ɫ������ȣ����ش���
	return ((cbFirstColor == cbSecondColor) ? cbFirstColor : COLOR_ERROR);
}

//���ſ����Ƿ���������һ���Ʊȵڶ��Ŵ�
bool CGameData::IsLineValue(BYTE cbTurnCard, BYTE cbSecondCard)
{
	bool bIsLine = false;

	//��ȡ�����߼���ֵ
	BYTE cbFirstLogicValue = GetCardLogicValue(cbTurnCard);
	BYTE cbSecondLogicValue = GetCardLogicValue(cbSecondCard);

	//������ʵֵ�ͻ�ɫ
	BYTE bFirstValue = GetCardValue(cbTurnCard);
	BYTE bFirstColor = GetCardColor(cbTurnCard);
	BYTE bNextValue = GetCardValue(cbSecondCard);
	BYTE bNextColor = GetCardColor(cbSecondCard);

	//��С��
	if (bFirstValue == 0x0F && bNextValue == 0x0E) bIsLine = true;

	//�������ƣ�Ҳ������������		TODO �ǵ�У��
	if ((m_playing_para.level_value != 2) &&
		(m_playing_para.level_value != 1) &&
		(m_playing_para.level_value != 14) &&
		(m_playing_para.level_value != 15))		//���˵���С���ֵ�ʹ�С��
	{
		if ((bFirstValue = m_playing_para.level_value + 1) &&
			(bNextValue = m_playing_para.level_value - 1))
		{
			bIsLine = true;
		}
	}

	//�����ж�
	if (cbFirstLogicValue == (cbSecondLogicValue + 1)) bIsLine = true;

	//���ˣ����Լ��ƣ����һ�ɫ�����������������
	if ((bFirstValue == m_playing_para.level_value) &&
		(bNextValue == m_playing_para.level_value) &&
		(bFirstColor == bNextColor + 1))
	{
		bIsLine = false;
	}

	return bIsLine;
}

//һ�ֱ��ƣ��ж�Ӯ��
WORD CGameData::GetOneRoundWinner()
{
	//��ʼӮ��Ϊ��һ���������
	WORD wWinner = m_playing_para.start_outcard_user;
	WORD wFirstIndex = wWinner;
	BYTE cbCardType = GetCardLogicType(m_players_config.players[wWinner].out_card_info, m_players_config.players[wWinner].out_card_num);

	//�Ա��˿�
	switch (cbCardType)
	{
	case CT_SINGLE:		//��������
	case CT_SAME_2:		//��������
	case CT_TRACKOR_2:	//��������
	case CT_TRACKOR_3:	//��������
	case CT_TRACKOR_4:	//��������
	case CT_TRACKOR_5:	//��������
	case CT_TRACKOR_6:	//��������
	case CT_TRACKOR_7:	//��������
	case CT_TRACKOR_8:	//��������
	case CT_TRACKOR_9:	//��������
	case CT_TRACKOR_10:	//��������
	case CT_TRACKOR_11:	//��������
	case CT_TRACKOR_12:	//��������
	case CT_TRACKOR_13:	//��������
	{
		//�ӿ�ʼ������ҿ�ʼ�������Ƚ�	
		wFirstIndex = GetNextUser(wFirstIndex);
		while (wFirstIndex != m_playing_para.start_outcard_user)
		{
			//У������
			if (GetCardLogicType(m_players_config.players[wFirstIndex].out_card_info, m_players_config.players[wWinner].out_card_num) == cbCardType)
			{
				//�Ƚϴ�С
				if (CompareUserOutCard(m_players_config.players[wWinner].out_card_info[0], m_players_config.players[wFirstIndex].out_card_info[0]))
				{
					wWinner = wFirstIndex;
				}
			}

			wFirstIndex = GetNextUser(wFirstIndex);		//����¼�
		}

		return wWinner;
	}
	//case CT_THROW_CARD:	//˦������
	//	{
	//		//��ɫ�ж�
	//		if (GetCardLogicColor(cbOutCardData[wWinnerIndex][0])==COLOR_NT) return wFirstIndex;

	//		//��������
	//		tagAnalyseResult WinnerResult;
	//		AnalyseCardData(cbOutCardData[wFirstIndex],cbCardCount,WinnerResult);

	//		//ѭ���Ƚ�
	//		for (WORD i=0;i<GAME_PLAYER-1;i++)
	//		{
	//			//�����ж�
	//			WORD wUserIndex=(wFirstIndex+i+1)%GAME_PLAYER;
	//			if (GetCardLogicColor(cbOutCardData[wUserIndex],cbCardCount)!=COLOR_NT) continue;

	//			//��������
	//			tagAnalyseResult UserResult;
	//			if (RectifyCardWeave(cbOutCardData[wUserIndex],cbCardCount,WinnerResult,UserResult)==false) continue;

	//			//�ԱȽ��
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

//����һ����Ӯ�÷�
WORD CGameData::SettleOneRound(const WORD &wWinner)
{
	//��������ׯ�Һ�ׯ�ҵ��¼�����,ׯ�����طַ����м��ǵ÷ַ�
	WORD score = 0;

	//�ж�Ӯ���ǵ÷ַ������طַ�
	if ((wWinner == GetNextUser(m_playing_para.appointbanker)) ||
		(wWinner == GetLastUser(m_playing_para.appointbanker)))
	{
		//��õ÷ֿ�������
		BYTE cbScoreCardNum = GetScoreCard();

		//����ܷ�
		score = GetAllCardScore(m_playing_para.score_card, cbScoreCardNum);

		//�����мҵ÷֣���������ׯ�ҵ��¼�����
		WORD wBankerNext = GetNextUser(m_playing_para.appointbanker);
		if (INVALID_CHAIR != wBankerNext)
		{
			//m_players_config.players[wBankerNext].single_score += score;      ������������ 5 10 K
			//printf("\n��һ�ֽ��������ֵ÷� = %d����� = %d ���ܷ� = %d��\n", score, wBankerNext, m_players_config.players[wBankerNext].single_score);
		}
	}

	//ׯ��Ӯ����Ҫ�Ƿ�
	return score;
}

//���һ���е����з���
BYTE CGameData::GetScoreCard()
{
	//��������
	BYTE cbPostion = 0;

	//��ø���������ҳ��Ƶķ���
	for (int i = 0; i < m_room_config.max_chair_sum; i++)
	{
		if (USER_PLAYING == m_players_config.players[i].play_state)
		{
			//������ҳ�������
			for (BYTE j = 0; j < m_players_config.players[i].out_card_num; j++)
			{
				//��ȡ��ֵ
				BYTE cbCardValue = GetCardValue(m_players_config.players[i].out_card_info[j]);

				//�ۼ��˿�
				if ((cbCardValue == 5) || (cbCardValue == 10) || (cbCardValue == 13))
				{
					m_playing_para.score_card[cbPostion++] = m_players_config.players[i].out_card_info[j];
				}
			}
		}
	}

	return cbPostion;
}

//��ȡ���������ܷ���
WORD CGameData::GetAllCardScore(const BYTE *cbCardData, BYTE cbCardCount)
{
	//��������
	WORD wCardScore = 0;

	//�˿��ۼ�
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		//��ȡ��ֵ
		BYTE cbCardValue = GetCardValue(cbCardData[i]);

		//�ۼƻ���
		if (cbCardValue == 5)
		{
			wCardScore += 5;
			continue;
		}

		//�ۼƻ���
		if ((cbCardValue == 10) || (cbCardValue == 13))
		{
			wCardScore += 10;
			continue;
		}
	}

	return wCardScore;
}

//�Ƚ���ҳ��ƴ�С�������ǰ��󣬷���true
bool CGameData::CompareUserOutCard(const BYTE &cbFirstCardData, const WORD &cbNextCardData)
{
	//��ȡ��ɫ
	BYTE cbLogicColorNext = GetCardLogicColor(cbNextCardData);
	BYTE cbLogicColorFirst = GetCardLogicColor(cbFirstCardData);

	//��ɫ��ͬ�������ƣ���Աȴ�С
	if ((cbLogicColorNext == COLOR_MAIN_CARD) || (cbLogicColorNext == cbLogicColorFirst))
	{
		//��ȡ��С
		BYTE cbLogicValueNext = GetCardLogicValue(cbNextCardData);
		BYTE cbLogicValueFirst = GetCardLogicValue(cbFirstCardData);

		//��С�ж�
		if (cbLogicValueNext > cbLogicValueFirst)
			return true;
	}

	//��ɫ��ͬ����϶��ǵ�һ����

	return false;
}

//����У��    //�����Ҫ�����ƺ͸������ĳ��ƽ��бȽ� ���Գ�����true
bool CGameData::EfficacyOutCard(const WORD &wCheckUser, const BYTE *cbOutCardData, const BYTE &cbOutCardCount)
{
	//��ø������ĳ�������
	BYTE cbTurnCardNum = m_playing_para.turn_cards_num;
	BYTE cbTurnCard[MAX_CARD_COUNT];
	ZeroMemory(&cbTurnCard, sizeof(cbTurnCard));
	CopyMemory(cbTurnCard, m_playing_para.turn_max_cards, sizeof(cbTurnCard));

	printf("��ø������ĳ�������:");
	for (int i = 0; i < cbTurnCardNum; i++)
	{
		printf("%d ", cbTurnCard[i]);
	}
	cout << endl;

	//��ȡ����
	int cbFirstType = GetCardLogicType(cbTurnCard, cbTurnCardNum);
	int cbNextType = GetCardLogicType(cbOutCardData, cbOutCardCount);

	cout << "cbFirstType:" << cbFirstType << endl;
	cout << "cbNextType:" << cbNextType << endl;


	//�����ж�
	if (cbNextType == CT_ERROR) return false;
	if ((cbFirstType&(1 << CT_MISSILE_CARD)) != 0)  return false;
	if ((cbNextType&(1 << CT_MISSILE_CARD)) != 0) return true;

	//ը���ж�
	if ((cbFirstType&(1 << CT_LAIZI_BOMB)) != 0) { return false; }
	if ((cbNextType&(1 << CT_LAIZI_BOMB)) != 0) { return true; }
	if ((cbFirstType&(1 << CT_BOMB_CARD)) == 0 && (cbNextType&(1 << CT_BOMB_CARD)) != 0) { return true; }
	if ((cbFirstType&(1 << CT_BOMB_CARD)) != 0 && (cbNextType&(1 << CT_BOMB_CARD)) == 0) { return false; }
	if ((cbFirstType&(1 << CT_RUAN_BOMB)) != 0 && (cbNextType&(1 << CT_RUAN_BOMB)) == 0 && (cbNextType&(1 << CT_BOMB_CARD)) == 0) { return false; }
	if ((cbFirstType&(1 << CT_BOMB_CARD)) == 0 && (cbFirstType&(1 << CT_RUAN_BOMB)) == 0 && (cbNextType&(1 << CT_RUAN_BOMB)) != 0) { return true; }


	//�����ж�
	if (((cbFirstType&cbNextType) == 0 || (cbTurnCardNum != cbOutCardCount)) && (cbFirstType&(1 << CT_BOMB_CARD)) == 0 && (cbFirstType&(1 << CT_RUAN_BOMB)) == 0 &&
		(cbNextType&(1 << CT_BOMB_CARD)) == 0 && (cbNextType&(1 << CT_RUAN_BOMB)) == 0) return false;

	//��ʼ�Ա�
	if ((cbNextType&(1 << CT_BOMB_CARD)) != 0)
	{

		//�����˿�
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;

		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_BOMB_CARD))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_BOMB_CARD))return false;

		//��ȡ��ֵ
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbFourCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbFourCardData[0]);

		//�������ֵ
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
		//�����˿�
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;

		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_RUAN_BOMB))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_RUAN_BOMB))return false;

		//��ȡ��ֵ
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbFourCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbFourCardData[0]);

		//�������ֵ
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
		//�����˿�
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;

		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_SINGLE))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_SINGLE))return false;

		//��ȡ��ֵ
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbSignedCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbSignedCardData[0]);

		//�������ֵ
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
		//�����˿�
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;

		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_DOUBLE))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_DOUBLE))return false;

		//��ȡ��ֵ
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbDoubleCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbDoubleCardData[0]);

		//�������ֵ
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
		//�����˿�
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;

		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_THREE))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_THREE))return false;

		//��ȡ��ֵ
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbThreeCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbThreeCardData[0]);

		//�������ֵ
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
		//�����˿�
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;

		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_SINGLE_LINE))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_SINGLE_LINE))return false;

		//��ȡ��ֵ
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbSignedCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbSignedCardData[0]);

		//�������ֵ
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
		//�����˿�
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;

		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_DOUBLE_LINE))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_DOUBLE_LINE))return false;

		//��ȡ��ֵ
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbDoubleCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbDoubleCardData[0]);

		//�������ֵ
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
		//�����˿�
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;

		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_THREE_LINE))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_THREE_LINE))return false;

		//��ȡ��ֵ
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbThreeCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbThreeCardData[0]);

		//�������ֵ
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
		//�����˿�
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;

		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_THREE_LINE_TAKE_ONE))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_THREE_LINE_TAKE_ONE))return false;

		//��ȡ��ֵ
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbThreeCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbThreeCardData[0]);

		//�������ֵ
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
		//�����˿�
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;

		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_THREE_LINE_TAKE_TWO))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_THREE_LINE_TAKE_TWO))return false;

		//��ȡ��ֵ
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbThreeCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbThreeCardData[0]);

		//�������ֵ
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
		//�����˿�
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;
		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_FOUR_LINE_TAKE_ONE))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_FOUR_LINE_TAKE_ONE))return false;

		//��ȡ��ֵ
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbFourCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbFourCardData[0]);

		//�������ֵ
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
		//�����˿�
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;
		ZeroMemory(&NextResult, sizeof(NextResult));
		ZeroMemory(&FirstResult, sizeof(FirstResult));

		if (!AnalysebCompare(cbOutCardData, cbOutCardCount, NextResult, CT_FOUR_LINE_TAKE_TWO))return false;
		if (!AnalysebCompare(cbTurnCard, cbTurnCardNum, FirstResult, CT_FOUR_LINE_TAKE_TWO))return false;

		//��ȡ��ֵ
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbFourCardData[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbFourCardData[0]);

		//�������ֵ
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

//����ָ����ɫ��ȡ�˿�
BYTE CGameData::DistillCardByColor(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardColor, BYTE cbResultCard[])
{
	//��������
	BYTE cbResultCount = 0;

	//��ȡ�˿�
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		//��ɫ�ж�
		if (GetCardLogicColor(cbCardData[i]) == cbCardColor)
		{
			cbResultCard[cbResultCount++] = cbCardData[i];
			continue;
		}

		//��ֹ�ж�
		if (cbResultCount > 0) break;
	}

	return cbResultCount;
}

//������Ŀ��ȡ�˿�
BYTE CGameData::DistillCardByCount(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbSameCount, tagSameDataInfo &SameDataInfo)
{
	//����Ч��
	if ((cbSameCount < 2) || (cbSameCount > MAX_PACK)) return 0;

	//���ñ���
	ZeroMemory(&SameDataInfo, sizeof(SameDataInfo));

	//��ȡ�ж�
	if (cbCardCount < cbSameCount) return 0;

	//��ȡ�˿�
	for (BYTE i = 0; i < cbCardCount - cbSameCount + 1; i++)
	{
		//��Ŀ�ж�
		BYTE j = 0;
		for (j = 1; j < cbSameCount; j++)
		{
			if (cbCardData[i] != cbCardData[i + j]) break;
		}

		//�ɹ�����
		if (j == cbSameCount)
		{
			//ͬ������
			SameDataInfo.cbBlockCount++;

			//�˿�����
			for (BYTE k = 0; k < cbSameCount; k++)
			{
				SameDataInfo.cbSameData[SameDataInfo.cbCardCount++] = cbCardData[i + k];
			}
		}

		//��������
		i += j - 1;
	}

	return SameDataInfo.cbBlockCount;
}

//��ȡ�������˿�
BYTE CGameData::DistillTrackorByCount(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbSameCount, tagTractorDataInfo &TractorDataInfo)
{
	//����Ч��
	if ((cbSameCount < 2) || (cbSameCount > MAX_PACK)) return 0;

	//���ñ���
	ZeroMemory(&TractorDataInfo, sizeof(TractorDataInfo));

	//��ȡͬ��
	tagSameDataInfo SameDataInfo;
	DistillCardByCount(cbCardData, cbCardCount, cbSameCount, SameDataInfo);

	//�ƶ��ж�
	if (SameDataInfo.cbBlockCount < 2) return 0;

	//��������
	BYTE cbResultIndex = 0;
	for (BYTE i = 0; i < SameDataInfo.cbBlockCount - 1; i++)
	{
		//��������
		if (SameDataInfo.cbSameData[i*cbSameCount] == 0) continue;

		//��������
		BYTE cbLineCount = 1;
		BYTE cbTrackorIndex[MAX_TRACKOR] = { i*cbSameCount };

		//�����ж�
		for (BYTE j = i; j < SameDataInfo.cbBlockCount - 1; j++)
		{
			//��������
			if (SameDataInfo.cbSameData[(j + 1)*cbSameCount] == 0) continue;

			//��������
			BYTE cbSecondIndex = (j + 1)*cbSameCount;
			BYTE cbFirstIndex = cbTrackorIndex[cbLineCount - 1];
			BYTE cbFirstCardData = SameDataInfo.cbSameData[cbFirstIndex];
			BYTE cbSecondCardData = SameDataInfo.cbSameData[cbSecondIndex];

			//�����ж�
			if (IsLineValue(cbFirstCardData, cbSecondCardData) == true)
			{
				cbTrackorIndex[cbLineCount++] = cbSecondIndex;
				continue;
			}

			//�������
			if (cbFirstCardData == cbSecondCardData) continue;
			if (GetCardValue(cbSecondCardData) == m_playing_para.nt_value) continue;
			if (GetCardValue(cbSecondCardData) == m_playing_para.level_value) continue;

			break;
		}

		//�ɹ��ж�
		if (cbLineCount >= 2)
		{
			//��󳤶�
			if (cbLineCount > TractorDataInfo.cbTractorMaxLength)
			{
				TractorDataInfo.cbTractorMaxLength = cbLineCount;
				TractorDataInfo.cbTractorMaxIndex = cbResultIndex;
			}

			//���ý��
			TractorDataInfo.cbTractorCount++;
			TractorDataInfo.cbCardCount += cbLineCount * cbSameCount;
			TractorDataInfo.cbTractorData[cbResultIndex++] = cbLineCount;
			TractorDataInfo.cbTractorData[cbResultIndex++] = cbSameCount;

			//��������
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
	//��������
	BYTE cbAnalyseCard[MAX_CARD_COUNT];
	CopyMemory(cbAnalyseCard, cbCardData2, sizeof(BYTE)*cbCardCount2);

	//�����ж�
	BYTE cbIntersectionCount = 0;
	for (BYTE i = 0; i < cbCardCount1; i++)
	{
		for (BYTE j = 0; j < cbCardCount2; j++)
		{
			if (cbCardData1[i] == cbAnalyseCard[j])
			{
				//���ñ���
				cbAnalyseCard[j] = 0;
				cbIntersectionCount++;

				break;
			}
		}
	}

	return cbIntersectionCount;
}

//����һ�ֵĿ�ʼ������ң�������ҿ��Գ�����
bool CGameData::AnalyseOutCardNotify(const WORD &wChairID, const BYTE *OutCardData,
	const BYTE &cbOutCardNum, tagOutCardNotify *cbOutCardNotify)
{


	return true;

}

//�ж��û��������Ƿ���ָ����ɫ�ĸ����͵Ŀ���
bool CGameData::IsHaveSpecificType(const WORD &wChairID, const BYTE &color, const BYTE &cbOutCardType,
	const BYTE &cbOutCardNum, tagOutCardNotify &cbOutCardNotify)
{
	//У��
	if ((CT_ERROR == cbOutCardType) || (0 == cbOutCardNum))
		return false;

	//������Ŀ
	cbOutCardNotify.cbOutCardNum = cbOutCardType;

	//�ж��������Ƿ��иû�ɫ����
	BYTE cbHandCardNum = GetUserCurCardNum(wChairID);
	bool bHaveColor = false;

	//���������ָ����ɫ�Ŀ�������
	std::vector<BYTE> vecColorCard;

	//��������г���ָ����ɫ�Ŀ�������
	std::vector<BYTE> vecNotColorCard;

	//��������
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

	//���иû�ɫ�����ж�����
	if (bHaveColor)
	{
		//�����иû�ɫ�Ŀ�����Ŀ
		BYTE cbColorNum = vecColorCard.size();

		//��������
		if ((CT_SINGLE == cbOutCardType) && (1 == cbOutCardNum))
		{
			//�ܳ�����Ϊ�û�ɫȫ������
			for (int i = 0; i < vecColorCard.size(); i++)
			{
				cbOutCardNotify.cbMustOutCard[i] = vecColorCard[i];
			}
			return true;
		}

		//��������
		if ((CT_SAME_2 == cbOutCardType) && (2 == cbOutCardNum))
		{
			//�û�ɫ���Ʋ���
			if (cbColorNum < cbOutCardNum)
			{
				//������Ŀ���Ϊ���û�ɫ����
				for (int i = 0; i < cbColorNum; i++)
				{
					cbOutCardNotify.cbMustOutCard[i] = vecColorCard[i];
				}

				//��ѡ���Ŀ���Ϊ��������ɫ����
				for (int i = 0; i < vecNotColorCard.size(); i++)
				{
					cbOutCardNotify.cbOptionalCard[i] = vecNotColorCard[i];
				}
			}
			else		//�����㹻�����ж����ƶ���
			{
				//�����Ҹû�ɫ�����ж���
				int nPairNum = 0;
				for (int i = 0; i < cbColorNum; i += 2)
				{
					int nSameNum = count(vecColorCard.begin(), vecColorCard.end(), vecColorCard[i]);
					if (2 == nSameNum)	//�ж��ӣ�ֻ�ܳ�����
					{
						cbOutCardNotify.cbMustOutCard[nPairNum++] = vecColorCard[i];
						cbOutCardNotify.cbMustOutCard[nPairNum++] = vecColorCard[i];
					}
				}

				//û���ӣ�������û�ɫ��������
				if (0 == nPairNum)
				{
					//�ܳ�����Ϊ�û�ɫȫ������
					for (int i = 0; i < cbColorNum; i++)
					{
						cbOutCardNotify.cbMustOutCard[i] = vecColorCard[i];
					}
				}
			}

			return true;
		}

		//���������ͣ����ԣ�
		if ((CT_TRACKOR_2 <= cbOutCardType) && (CT_TRACKOR_13 >= cbOutCardType))
		{
			//�����Ҹû�ɫ�����ж���
			std::vector<BYTE> vecPair;
			for (int i = 0; i < cbColorNum; i += 2)
			{
				int nSameNum = count(vecColorCard.begin(), vecColorCard.end(), vecColorCard[i]);
				if (2 == nSameNum)	//�ж��ӣ�ֻ�ܳ�����
				{
					vecPair.push_back(vecColorCard[i]);
					vecPair.push_back(vecColorCard[i]);
				}
			}
			//���������С
			int nPairNum = vecPair.size();

			//�û�ɫ���Ʋ���
			if (cbColorNum < cbOutCardNum)
			{
				//�ж��ӳ�����
				int nIndex = 0;
				for (int i = 0; i < nPairNum; i++)
				{
					cbOutCardNotify.cbMustOutCard[nIndex++] = vecPair[i];
				}

				//������ӣ��ٳ���û�ɫ���� ������ٳ����⿨��
				for (int i = 0; i < cbColorNum; i++)
				{
					//���˶����еĿ���
					if (vecPair.end() == find(vecPair.begin(), vecPair.end(), vecColorCard[i]))
					{
						cbOutCardNotify.cbMustOutCard[nIndex++] = vecColorCard[i];
					}
				}

				//�ٳ����⿨��
				for (int i = 0; i < vecNotColorCard.size(); i++)
				{
					cbOutCardNotify.cbOptionalCard[i] = vecNotColorCard[i];
				}
			}
			else		//�����㹻�����ж����ƶ���
			{
				//û���ӣ�������û�ɫ��������
				if (0 == nPairNum)
				{
					//�ܳ�����Ϊ�û�ɫȫ������
					for (int i = 0; i < cbColorNum; i++)
					{
						cbOutCardNotify.cbMustOutCard[i] = vecColorCard[i];
					}
				}
				else	//�ж��ӣ����ж���û�������������ж϶����Ƿ��㹻
				{
					//���㹻��Ķ���
					if (nPairNum >= cbOutCardNum)
					{
						//�ж��ǲ���������
						BYTE *cbTmpPair = new BYTE[nPairNum];
						for (int i = 0; i < nPairNum; i++)
							cbTmpPair[i] = vecPair[i];

						//����������Ķ�����
						BYTE cbTrackorPair = cbOutCardNum / 2;

						//ѭ���������ж���,�ж���û��������
						int nStartIndex = 0;
						int nEndIndex = nStartIndex + cbTrackorPair * 2 - 1;
						bool bHaveTrackor = false;
						while (nEndIndex < nPairNum)
						{
							//�ж��ǲ���������						
							if (cbOutCardType == GetCardLogicType(cbTmpPair + nStartIndex, cbOutCardNum))
							{
								bHaveTrackor = true;
								//��ñ�����Ƶ�����
								for (int i = nStartIndex; i < nEndIndex; i++)
								{
									cbOutCardNotify.cbMustOutCard[i] = cbTmpPair[i];
								}
							}
							//���������Ƶ�λ��
							nStartIndex += 2;
							nEndIndex += 2;
						}

						//û��������,����������
						if (!bHaveTrackor)
						{
							for (int i = 0; i < nPairNum; i++)
							{
								cbOutCardNotify.cbMustOutCard[i] = vecPair[i];
							}
						}

						//�ͷ��ڴ�
						delete cbTmpPair;
						return true;
					}
					else	//û���㹻�Ķ��ӣ������� + �û�ɫ�ĵ���
					{
						//�ж��ӳ�����
						int nIndex = 0;
						for (int i = 0; i < nPairNum; i++)
						{
							cbOutCardNotify.cbMustOutCard[nIndex++] = vecPair[i];
						}

						//������ӣ��ٳ���û�ɫ���� 
						nIndex = 0;
						for (int i = 0; i < cbColorNum; i++)
						{
							//���˶����еĿ���
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
	else	//û�û�ɫ����ʲô�����ԣ��û����ƶ����Գ�
	{
		//�������
		for (int i = 0; i < cbHandCardNum; i++)
		{
			cbOutCardNotify.cbOptionalCard[i] = m_players_config.hand_cards[wChairID][i];
		}
	}

	return true;
}

/******************************************************************************/
/*********************************  ��ѯ�ӿ�ʵ��  *******************************/

/**
 * ���������˵���ת�����ͻ��ˣ���ѯ�û�����
 * @param			wChairID			[in]		���ӱ��
 * @param			cards			[out]		���ص����黺����
 * @param			cardsum			[in-out]	���صĻ��������ȣ���BYTE��
 */
int __stdcall CGameData::GetClientHandCards(WORD wChairID, BYTE *cards, const int &cardsum)
{
	//У��
	if (NULL == cards || 0 == cardsum)
	{
		return -1;
	}

	//����ת��
	for (int i = 0; i < cardsum; ++i)
	{
		cards[i] = m_players_config.hand_cards[wChairID][i];
	}

	return 0;
}

// ���õ�ǰ��Ϸ����ID
int __stdcall CGameData::SetKindID(DWORD id)
{
	m_room_config.kind_id = id;
	return 0;
}

// ��ѯ��ǰ��Ϸ����ID
DWORD __stdcall CGameData::GetKindID()
{
	return m_room_config.kind_id;
}

////��ѯÿ�����õ����������
//DWORD __stdcall CGameData::GetGameMaxCardNum()
//{
//	return m_card_config.game_cards_sum;
//}

// ��ѯ���С�ֵ÷�
SCORE __stdcall CGameData::GetPlayerSingleScore(WORD wChairID)
{
	return m_players_config.players[wChairID].single_score;
}

// ������ҷ������ܵ÷�
DWORD __stdcall CGameData::SetPlayerTotalScore(WORD wChairID, SCORE score)
{
	m_players_config.players[wChairID].total_score += score;
	return 0;
}

// ��ѯ��ҷ������ܵ÷�
SCORE __stdcall CGameData::GetPlayerTotalScore(WORD wChairID)
{
	return m_players_config.players[wChairID].total_score;
}

// ��ѯ�����Ϸ��״̬-׼��״̬
BYTE __stdcall CGameData::GetXjReadyState(WORD wChairID)
{
	return m_players_config.players[wChairID].xj_ready_state;

}

// ���������Ϸ��״̬-׼��
int __stdcall CGameData::SetXjReadyState(WORD wChairID, BYTE ready)
{
	m_players_config.players[wChairID].xj_ready_state = ready;
	return 0;
}

//��¼���ׯ�Ҵ���  ��ֽ���
int __stdcall CGameData::SetPlayerBankCount(WORD wChairID)
{
	m_players_config.players[wChairID].bank_count++;
	return 0;
}

//��ȡ���ׯ�Ҵ���  
int __stdcall CGameData::GetPlayerBankCount(WORD wChairID)
{
	return m_players_config.players[wChairID].bank_count;
}

// �������Ӯ����
int __stdcall CGameData::SetWinSum(WORD wChairID, BYTE winsum)
{
	m_players_config.players[wChairID].winsum += winsum;
	return 0;
}

// ��ѯ���Ӯ����
int __stdcall CGameData::GetWinSum(WORD wChairID, BYTE &winsum)
{
	winsum = m_players_config.players[wChairID].winsum;
	return 0;
}

// ��ѯ��ǰ����
BYTE __stdcall CGameData::GetCurGameCount()
{
	return m_playing_para.game_cur_count;
}

// ���õ�ǰ����
int __stdcall CGameData::SetGameCount(BYTE count)
{
	m_playing_para.game_cur_count = count;
	return 0;
}

// ��ѯ�ܾ���
BYTE __stdcall CGameData::GetAllCount()
{
	return m_room_config.game_count;
}

// �����ܾ���
int __stdcall CGameData::SetAllCount(BYTE count)
{
	//У��
	if (count < 1)		//������ ���ù̶�����
	{
		count = 2;
	}

	printf("�ܾ�����%d\n", count);

	m_room_config.game_count = count;
	return 0;
}

// ���÷��䵱ǰ����
int __stdcall CGameData::SetCurRoomBet(SCORE room_bet)
{
	m_playing_para.room_bet *= room_bet;

	return 0;
}

// ��ѯ���䵱ǰ����
SCORE __stdcall CGameData::GetCurRoomBet()
{
	return m_playing_para.room_bet;
}

// ��ѯ����׷�
DWORD __stdcall CGameData::GetCellScore()
{
	return m_room_config.room_cell;
}

// ������ע�׷�
int __stdcall CGameData::SetCellScore(DWORD score)
{
	//У��
	if (score < ROOMRULE_MIN_CELL_SCORE)
	{
		score = ROOMRULE_MIN_CELL_SCORE;
	}

	printf("��ӡ����׷֣�%d\n", score);

	m_room_config.room_cell = score;
	return 0;
}

// ��ѯ�����ע��ɱ�־
BYTE __stdcall CGameData::GetAddScoreState(WORD wChairID)
{
	return m_players_config.players[wChairID].bet_state;
}

// ���������ע��ɱ�־		0-��עδ���		1-��ע���
int __stdcall CGameData::SetAddScoreState(WORD wChairID, BYTE beted)
{
	m_players_config.players[wChairID].bet_state = beted;
	return 0;
}

// ��ѯ���������ɱ�־
BYTE __stdcall CGameData::GetMingPaiState(WORD wChairID)
{
	return m_players_config.players[wChairID].mingpai_state;
}

// �������������ɱ�־		0-����δ���		1-�������
int __stdcall CGameData::SetMingPaiState(WORD wChairID, BYTE state)
{
	m_players_config.players[wChairID].mingpai_state = state;
	return 0;
}

// ��ѯ�����ע��ɱ�־
BYTE __stdcall CGameData::GetUserOperateState(WORD wChairID)
{
	return m_players_config.players[wChairID].action_state;
}

// ���������ע��ɱ�־		0-��עδ���		1-��ע���
int __stdcall CGameData::SetUserOperateState(WORD wChairID, BYTE state)
{
	m_players_config.players[wChairID].action_state = state;
	return 0;
}

// ��ȡ����ܷ�
DWORD __stdcall CGameData::GetAllBet(WORD wChairID)
{
	return  m_players_config.players[wChairID].bet;
}

// ��ѯ��������
BYTE __stdcall CGameData::GetMaxChairCount()
{
	return m_room_config.max_chair_sum;
}

// ������������
int __stdcall CGameData::SetMaxChairCount(BYTE count)
{
	//��ܴ�����������������ܴ��ڴ���
	if (MAX_CHAIR_COUNT < count || count < 1)
	{
		count = MAX_CHAIR_COUNT;
	}
	m_players_config.player_sum = count;
	m_room_config.max_chair_sum = count;
	return 0;
}

// ����һ��������ǳ��ƻ��ǹ�
int __stdcall CGameData::SetTurnOutCardType(const WORD wChairID, DWORD type)
{
	m_playing_para.out_card_type[wChairID] = type;
	return 0;
}

// ��ȡһ��������ǳ��ƻ��ǹ�
DWORD __stdcall CGameData::GetTurnOutCardType(const WORD wChairID)
{
	return m_playing_para.out_card_type[wChairID];
}

//������Ϸ�÷�ģʽ
int __stdcall CGameData::SetGameScoreMode(BYTE nMode)
{
	//У��
	if (GAME_SCORE_MODE_CLASSIC != nMode && GAME_SCORE_MODE_CRAZY != nMode)
	{
		nMode = GAME_SCORE_MODE_CLASSIC;
	}
	m_room_config.game_score_mode = nMode;
	return 0;
}

//�����Ϸ�÷�ģʽ
BYTE __stdcall CGameData::GetGameScoreMode()
{
	return m_room_config.game_score_mode;
}

//������Ϸ��ׯģʽ��0 - ��ׯ		1 - ������
int __stdcall CGameData::SetRobBankMode(BYTE nMode)
{
	//�̶�

	if (ROOMRULE_OPTION_ROBTYPE_FREE > nMode || ROOMRULE_OPTION_ROBTYPE_JIAOSANFEN < nMode)
	{
		nMode = ROOMRULE_OPTION_ROBTYPE_FREE;
	}
	m_room_config.game_robBank_mode = nMode;
	return 0;
}

//��ȡ��ׯģʽ
BYTE __stdcall CGameData::GetRobBankMode()
{
	return m_room_config.game_robBank_mode;
}

// ���õ�ǰ��ׯ����
int __stdcall CGameData::SetCurBankerCount(BYTE count)
{
	m_playing_para.rob_count = count;
	return 0;
}

// ��ȡ��ǰ��ׯ����
BYTE __stdcall CGameData::GetCurBankerCount()
{
	return m_playing_para.rob_count;
}

//���������ׯ����
int __stdcall CGameData::SetUserRobScore(const WORD &wChairID, BYTE score)
{
	m_players_config.players[wChairID].rob_score = score;
	return 0;
}

//���������ׯ����
int __stdcall CGameData::SetUserRobNum(const WORD &wChairID, BYTE cbNum)
{
	m_players_config.players[wChairID].rob_num = cbNum;
	return 0;
}

//��������ׯ����
BYTE __stdcall CGameData::GetUserRobNum(const WORD &wChairID)
{
	return m_players_config.players[wChairID].rob_num;
}

// ��ѯ��ǰ�����(ֻҪ���¾������)
//TODONOW added by WangChengQing 2017.9.22 �����ж� -- �����Ϸ�ڽ�����. ��client������������, serverӦ����ʾ "����ȵ��þֽ����� �ſ�������"  ���ж�Ӧ�������º�������Ӧ
BYTE __stdcall CGameData::GetCurPlayerCount()
{
	// ������Ϸ����
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

// ��ѯ��ǰ�ο���
BYTE __stdcall CGameData::GetCurLookerCount()
{
	BYTE curLookerCount = 0;
	for (BYTE i = 0; i < GetMaxChairCount(); i++)
	{
		if (USER_STANDUP == GetPlayerState(i))	//�������ӵ�״̬���ж�
		{
			curLookerCount++;
		}
	}
	return curLookerCount;
}

// ���õ�ǰׯ��
int __stdcall CGameData::SetBankerID(const WORD &wChairID)
{
	m_playing_para.appointbanker = wChairID;
	return 0;
}

// ��ѯ��ǰׯ��
WORD __stdcall CGameData::GetBankerID()
{
	return m_playing_para.appointbanker;
}


// �����ҵ��¼� -- ��ʱ���ȡ
WORD __stdcall CGameData::GetNextUser(const WORD &wChariID)
{
	if (INVALID_CHAIR == wChariID)
	{
		return INVALID_CHAIR;
	}
	//Ѱ���¼�
	WORD wNextChairID = INVALID_CHAIR;
	int nMaxChairCount = m_room_config.max_chair_sum;

	wNextChairID = (wChariID + 1) % nMaxChairCount;

	//�¼�����Ϸ��
	if (USER_PLAYING == m_players_config.players[wNextChairID].play_state)
	{
		return wNextChairID;
	}
	else
		return GetNextUser(wNextChairID); //���ʧ��,������ȡ��һ��
}

// ��ȡ�ϸ����  ˳ʱ���ȡ
WORD __stdcall CGameData::GetLastUser(const WORD &wChariID)
{
	if (INVALID_CHAIR == wChariID)
	{
		return INVALID_CHAIR;
	}
	//Ѱ���ϼ�
	WORD wLastChairID = INVALID_CHAIR;
	int nMaxChairCount = m_room_config.max_chair_sum;
	wLastChairID = (wChariID + nMaxChairCount - 1) % nMaxChairCount;

	//�¼�����Ϸ��
	if (USER_PLAYING == m_players_config.players[wLastChairID].play_state)
	{
		return wLastChairID;
	}
	else
		return GetLastUser(wLastChairID); //���ʧ��,������ȡ��һ��
}

// ��öԼ�
WORD __stdcall CGameData::GetOppositeUser(const WORD &wChariID)
{
	if ((INVALID_CHAIR == wChariID) ||
		(m_room_config.max_chair_sum != 4))
	{
		return INVALID_CHAIR;
	}
	//Ѱ�ҶԼ�
	WORD wOpposite = INVALID_CHAIR;
	int nMaxChairCount = m_room_config.max_chair_sum;

	wOpposite = (wChariID + 2) % nMaxChairCount;

	//�Լ�����Ϸ��
	if (USER_PLAYING == m_players_config.players[wOpposite].play_state)
	{
		return wOpposite;
	}

	return INVALID_CHAIR;
}

// �����û�״̬���ӷ�������ܻ�ã��û����¡�׼�����Թۡ���Ϸ��
int __stdcall CGameData::SetPlayerState(WORD wChairID, DWORD state)
{
	if (wChairID == INVALID_CHAIR)
	{
		return -1;
	}
	m_players_config.players[wChairID].play_state = state;
	return 0;
}

// ��ѯ�û�״̬���û����¡�׼�����Թۡ���Ϸ��
DWORD __stdcall CGameData::GetPlayerState(WORD wChairID)
{
	return m_players_config.players[wChairID].play_state;
}

// ������Ϸ״̬
int __stdcall CGameData::SetGameStatus(DWORD state)
{
	m_playing_para.game_state = state;
	return 0;
}

// ��ѯ��Ϸ״̬
DWORD __stdcall CGameData::GetGameStatus()
{
	return m_playing_para.game_state;
}

// ��ѯ��Ϸ��״̬-���̯��
BYTE __stdcall CGameData::GetShowCardState(WORD wChairID)
{
	return m_players_config.players[wChairID].showcard_state;

}

// ������Ϸ��״̬-���̯��		0-δ̯��	1-̯��
int __stdcall CGameData::SetShowCardState(WORD wChairID, BYTE state)
{
	m_players_config.players[wChairID].showcard_state = state;
	return 0;
}

/*************************************************
@Description:		���÷�����򣬴ӷ�������ܻ�����ݣ���Ϸid���ܾ�����
					�������������ע�׷֡���ׯģʽ���÷�ģʽ������
@Input:				pData-�ͻ��˴����Ľṹ������
					wDataSize-���ݴ�С
@Output:          ��
@Return:          ��
@author & data:	lizhihu 2017.11.2
*************************************************/
bool __stdcall CGameData::SetRoomRule(tagTableCfg * pRoomRuleOption)
{
	if (NULL == pRoomRuleOption)
	{
		return false;
	}

	//��ʼ��Ϊ0
	ZeroMemory(&m_room_config, sizeof(m_room_config));
	ZeroMemory(&m_players_config, sizeof(m_players_config));
	ZeroMemory(&m_card_config, sizeof(m_card_config));
	ZeroMemory(&m_playing_para, sizeof(m_playing_para));

	//������Ϸ����ID
	SetKindID(KIND_ID);

	//���þ���
	SetAllCount(pRoomRuleOption->com_rule->GameCount);

	//�������Ӻ������Ŀ
	SetMaxChairCount(pRoomRuleOption->com_rule->PlayerCount);

	// ������ע�׷�
	SetCellScore(1);

	//������Ϸ�÷�ģʽ������ģʽ��ϴ��   ---------����Ϸ������趨
	SetGameScoreMode(0);

	//������Ϸ��ׯģʽ��0 - ��ׯ		1 - ������
	SetRobBankMode(0);

	//���÷���
	//SetRoomFangzhu(pRoomRuleOption->com_rule->FangZhu);

	m_card_config.leave_card_num = NORMAL_LEAVE_CARD_NUM;
	m_card_config.init_card_num = NORMAL_HAND_CARD_NUM;

	//���÷ⶥ����
	SetRoomMaxBet(0);

	//������Ϸ���ģʽ  0-����  1-���  2-������
	SetLaiZiMode(0);

	//���õ��Ʒ���   0-������  1-����
	SetISLeaveCardDouble(0);

	//�����Ƿ�����   0-������   1-����
	SetMingPaiMode(0);

	//�����Ƿ�ӱ�   0-���ӱ�   1-�ӱ�
	SetAddBetMode(0);

	return true;
}

//���÷���
int __stdcall CGameData::SetRoomFangzhu(WORD nFangzhu_id)
{
	//У��
	if (nFangzhu_id < 0 || nFangzhu_id > m_players_config.player_sum)
	{
		nFangzhu_id = 0;
	}
	m_room_config.fangzhu_id = nFangzhu_id;
	return 0;
}

//��÷�������ID
WORD __stdcall CGameData::GetRoomFangzhu()
{
	return m_room_config.fangzhu_id;
}

//�õ�һ���û���ʼ������Ŀ
int __stdcall CGameData::GetPlayerInitCardNum()
{
	return m_card_config.init_card_num;
}

//�����û��ĳ�ʼ������Ŀ
int __stdcall CGameData::SetPlayerInitCardNum()
{
	//����������������ҳ�ʼ������
	m_card_config.init_card_num = (m_card_config.game_cards_sum - m_card_config.leave_card_num) / m_players_config.player_sum;

	//������ 17����
	m_card_config.init_card_num = 17;

	return 0;
}

//����Ͼ�Ӯ�����Ӻ�
WORD __stdcall CGameData::GetLastGameWinner()
{
	return m_playing_para.last_winner_id;
}

//����Ͼ�Ӯ�����Ӻ�
int __stdcall CGameData::SetLastGameWinner(const WORD &wChairID)
{
	m_playing_para.last_winner_id = wChairID;
	return 0;
}

//������������ɫ
int __stdcall CGameData::SetMainColor(const BYTE &color)
{
	m_playing_para.main_color = color;
	return 0;
}

//��ȡ��������ɫ
BYTE __stdcall CGameData::GetMainColor()
{
	return m_playing_para.main_color;
}

//�����������
int __stdcall CGameData::SetLiangZhuUser(const WORD &wChairID)
{
	m_playing_para.liangzhu_user = wChairID;
	return 0;
}

//��ȡ�������
WORD __stdcall CGameData::GetLiangZhuUser()
{
	return m_playing_para.liangzhu_user;
}

//���õ�ǰ����
int __stdcall CGameData::SetLevelValue(const BYTE &value)
{
	m_playing_para.level_value = value;
	return 0;
}

//��ȡ��ǰ����
BYTE __stdcall CGameData::GetLevelValue()
{
	return m_playing_para.level_value;
}

//��õ���
void __stdcall CGameData::GetLeaveCard(BYTE* cbLeaveCard, int nLeaveCardSize)
{
	//У��
	if ((NULL == cbLeaveCard) || (nLeaveCardSize != MAX_LEAVE_CARD_NUM))
	{
		return;
	}

	//��õ���
	memcpy(cbLeaveCard, m_playing_para.leave_card, sizeof(BYTE)* MAX_LEAVE_CARD_NUM);
}

//�����ҵ�ǰ������Ŀ
BYTE __stdcall CGameData::GetUserCurCardNum(const WORD &wChairID)
{
	return m_players_config.players[wChairID].hand_card_num;
}

//�����ҵ������е����Ƹ���
BYTE __stdcall CGameData::GetUserMainColorNum(const WORD &wChairID)
{
	//����ɫ����
	BYTE cbMainCount = 0;

	//��������
	for (int i = 0; i < m_players_config.players[wChairID].hand_card_num; i++)
	{
		BYTE cbCardData = m_players_config.hand_cards[wChairID][i];

		if (COLOR_MAIN_CARD == GetCardLogicColor(cbCardData))
			cbMainCount++;
	}

	return cbMainCount;
}

// ���õ�ǰ�����û�
int __stdcall CGameData::SetCurOutCardUser(const WORD &wChairID)
{
	m_playing_para.cur_outcard_user = wChairID;
	return 0;
}

// ��õ�ǰ�����û�
WORD __stdcall CGameData::GetCurOutCardUser()
{
	return m_playing_para.cur_outcard_user;
}

// ����һ�������ȿ�ʼ���Ƶ��û�
int __stdcall CGameData::SetStartOutCardUser(const WORD &wChairID)
{
	m_playing_para.start_outcard_user = wChairID;

	//������ֵĳ�������
	for (int i = 0; i < m_room_config.max_chair_sum; i++)
	{
		m_players_config.players[i].out_card_num = 0;
		ZeroMemory(m_players_config.players[i].out_card_info, sizeof(BYTE)*MAX_CARD_COUNT);
	}

	return 0;
}

// ���һ�������ȿ�ʼ���Ƶ��û�
WORD __stdcall CGameData::GetStartOutCardUser()
{
	return m_playing_para.start_outcard_user;
}


// ����һ���������Ƶ��û�
int __stdcall CGameData::SetLastOutCardUser(const WORD &wChairID)
{
	m_playing_para.last_outcard_user = wChairID;

	return 0;
}

// ���һ���������Ƶ��û�
WORD __stdcall CGameData::GetLastOutCardUser()
{
	return m_playing_para.last_outcard_user;
}

// ������ҳ��ƿ�������
bool __stdcall CGameData::SetUserOutCard(const WORD wChairID, const BYTE *cbCardData, const BYTE &cbCardNum)
{
	//У��
	if ((NULL == cbCardData) ||
		(0 == cbCardNum))
		return false;

	//��������
	memcpy(m_players_config.players[wChairID].out_card_info, cbCardData, sizeof(BYTE)*cbCardNum);
	//���Ƹ���
	m_players_config.players[wChairID].out_card_num = cbCardNum;

	return true;
}

// �����ҳ��ƿ�������
BYTE __stdcall CGameData::GetUserOutCard(const WORD wChairID, BYTE *cbCardData)
{
	BYTE cbCardNum = m_players_config.players[wChairID].out_card_num;

	//У��
	if (NULL == cbCardData)
		return cbCardNum;

	//��������
	memcpy(cbCardData, m_players_config.players[wChairID].out_card_info, sizeof(BYTE)*cbCardNum);

	return cbCardNum;
}

// ��õ�����Ŀ
BYTE __stdcall CGameData::GetLeaveCardNum()
{
	return m_card_config.leave_card_num;
}

// ���õ�ǰ��������
int __stdcall CGameData::SetCurFanZhuCard(const BYTE &card)
{
	m_playing_para.cur_fanzhu_card = card;
	return 0;
}

// �����ҵ�ǰ����
int __stdcall CGameData::GetUserCurLevel(const WORD &wChairID)
{
	return m_players_config.players[wChairID].level;
}

// ���õ�ǰ�������
int __stdcall CGameData::SetCurFanzhuUser(const WORD &wChairID)
{
	m_playing_para.cur_fanzhu_user = wChairID;
	return 0;
}

// ��õ�ǰ�������
WORD __stdcall CGameData::GetCurFanzhuUser()
{
	return m_playing_para.cur_fanzhu_user;
}

// ���õ�ǰ�۵����
int __stdcall CGameData::SetCurKoudiUser(const WORD &wChairID)
{
	m_playing_para.cur_koudi_user = wChairID;
	return 0;
}

// ��õ�ǰ�۵����
WORD __stdcall CGameData::GetCurKoudiUser()
{
	return m_playing_para.cur_koudi_user;
}

// �����ʼ��ׯ���
int __stdcall CGameData::SetStartRobUser(const WORD &wChairID)
{
	m_playing_para.start_rob_user = wChairID;
	return 0;
}

// ����ʼ��ׯ���
WORD __stdcall CGameData::GetStartRobUser()
{
	return m_playing_para.start_rob_user;
}

// ����һ�ֽ���
int __stdcall CGameData::SetOneTurnEnd(const BYTE &cbEnd)
{
	m_playing_para.turn_end = cbEnd;
	return 0;
}

// ���һ�ֽ���
BYTE __stdcall CGameData::GetOneTurnEnd()
{
	return m_playing_para.turn_end;
}

// ����һ�������������
int __stdcall CGameData::SetTurnMaxCards(BYTE *cbCardData, BYTE cbCardNum)
{
	//��������������
	ZeroMemory(&m_playing_para.turn_max_cards, sizeof(m_playing_para.turn_max_cards));

	//���Ƹ���
	m_playing_para.turn_cards_num = cbCardNum;

	//��ֵ
	for (int i = 0; i < cbCardNum && i < MAX_CARD_COUNT; i++)
	{
		m_playing_para.turn_max_cards[i] = cbCardData[i];
	}
	return 0;
}

// ���һ�������������
BYTE __stdcall CGameData::GetTurnMaxCards(BYTE *cbCardData, BYTE &cbCardNum)
{
	//У��
	if (NULL == cbCardData)
		return 0;

	//��ֵ
	cbCardNum = m_playing_para.turn_cards_num;
	for (int i = 0; i < cbCardNum; i++)
	{
		cbCardData[i] = m_playing_para.turn_max_cards[i];
	}
}

// ���������Ŀ
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

// ����������������Ŀ
BYTE __stdcall CGameData::GetMagicCardNum(const WORD &wChairID)
{
	//�������
	BYTE cbHandCardData[MAX_CARD_COUNT];
	memcpy(cbHandCardData, m_players_config.hand_cards[wChairID], sizeof(cbHandCardData));

	return GetMagicCardNum(cbHandCardData, m_players_config.players[wChairID].hand_card_num);
}

//���õ�ǰ���
int __stdcall CGameData::SetMagicCard(const BYTE &cbCard)
{
	m_playing_para.magic_card = ((cbCard >= 14) ? (cbCard - 13) : cbCard) + 0x50;   //����߼�ֵ����ô��
	return 0;
}

//��õ�ǰ���
BYTE __stdcall CGameData::GetMagicCard()
{
	return m_playing_para.magic_card;
}

//�������ը����Ϣ
int __stdcall CGameData::SetUserBoomInfo(const WORD &wChairID, const DWORD cbBoomType)
{
	cout << wChairID << ":ը������:" << (int)cbBoomType << endl;
	//У��
	if (((1 << CT_RUAN_BOMB) > cbBoomType) || ((1 << CT_MISSILE_CARD) < cbBoomType))
		cout << "Error!!!" << endl;

	// ���õ�ǰ���䱶������¼����ұ���
	if (((cbBoomType&(1 << CT_MISSILE_CARD)) != 0))    //��ը
	{
		for (int i = 0; i < m_room_config.max_chair_sum; i++)      //��ը�����������
		{
			if (USER_PLAYING == GetPlayerState(i))
			{
				if (0 < GetLaiZiMode())
					m_players_config.players[i].bet *= HARD_BOOM_TIMES;
				else
					m_players_config.players[i].bet *= RUAN_BOOM_TIMES;
			}

			if (m_players_config.players[i].bet > m_playing_para.max_room_bet)   //�ж��Ƿ�ⶥ
				m_players_config.players[i].bet = m_playing_para.max_room_bet;
		}

		//��ֵ
		BYTE &cbBoomNum = m_players_config.players[wChairID].boom_info.cbBoomNum;
		m_players_config.players[wChairID].boom_info.cbBoomType[cbBoomNum++] = cbBoomType;
	}
	else if (((cbBoomType & (1 << CT_BOMB_CARD)) != 0))    //Ӳը
	{
		for (int i = 0; i < m_room_config.max_chair_sum; i++)      //��ը�����������
		{
			if (USER_PLAYING == GetPlayerState(i))
			{
				if (0 < GetLaiZiMode())
					m_players_config.players[i].bet *= HARD_BOOM_TIMES;
				else
					m_players_config.players[i].bet *= RUAN_BOOM_TIMES;
			}

			if (m_players_config.players[i].bet > m_playing_para.max_room_bet)   //�ж��Ƿ�ⶥ
				m_players_config.players[i].bet = m_playing_para.max_room_bet;
		}

		//��ֵ
		BYTE &cbBoomNum = m_players_config.players[wChairID].boom_info.cbBoomNum;
		m_players_config.players[wChairID].boom_info.cbBoomType[cbBoomNum++] = cbBoomType;
	}
	else if (((cbBoomType & (1 << CT_RUAN_BOMB)) != 0))    //��ը
	{
		for (int i = 0; i < m_room_config.max_chair_sum; i++)      //��ը�����������
		{
			if (USER_PLAYING == GetPlayerState(i) && 0 < GetLaiZiMode())
			{
				m_players_config.players[i].bet *= RUAN_BOOM_TIMES;
			}

			if (m_players_config.players[i].bet > m_playing_para.max_room_bet)   //�ж��Ƿ�ⶥ
				m_players_config.players[i].bet = m_playing_para.max_room_bet;
		}

		//��ֵ
		BYTE &cbBoomNum = m_players_config.players[wChairID].boom_info.cbBoomNum;
		m_players_config.players[wChairID].boom_info.cbBoomType[cbBoomNum++] = cbBoomType;
	}
	else if (((cbBoomType & (1 << CT_LAIZI_BOMB)) != 0))    //���ը��
	{
		for (int i = 0; i < m_room_config.max_chair_sum; i++)      //��ը�����������
		{
			if (USER_PLAYING == GetPlayerState(i) && 0 < GetLaiZiMode())
			{
				m_players_config.players[i].bet *= HARD_BOOM_TIMES;
			}

			if (m_players_config.players[i].bet > m_playing_para.max_room_bet)   //�ж��Ƿ�ⶥ
				m_players_config.players[i].bet = m_playing_para.max_room_bet;
		}

		//��ֵ
		BYTE &cbBoomNum = m_players_config.players[wChairID].boom_info.cbBoomNum;
		m_players_config.players[wChairID].boom_info.cbBoomType[cbBoomNum++] = cbBoomType;
	}

	return 0;
}

//������ը����Ϣ
BYTE __stdcall CGameData::GetUserBoomInfo(const WORD &wChairID, tagBoomInfo &strBoomInfo)
{
	//��ֵ
	strBoomInfo.cbBoomNum = m_players_config.players[wChairID].boom_info.cbBoomNum;
	for (int i = 0; i < MAX_BOOM_NUM; i++)
	{
		strBoomInfo.cbBoomType[i] = m_players_config.players[wChairID].boom_info.cbBoomType[i];
	}

	return strBoomInfo.cbBoomNum;
}

/******************************************************************************/
/*******************************  ��������  *******************************/
/******************************************************************************/
// ������Ϸ���ô�����������
int CGameData::LoadGameCards()
{
	int result = 0;

	//����ȫ�������ļ���������ֵ����Ա����
	if ((NULL != CGameConfig::gComCardPara.group) &&
		(0 != CGameConfig::gComCardPara.groupNum) &&
		(0 != CGameConfig::gComCardPara.game_cards_num))
	{
		//ʵ�ʿ�������
		m_card_config.game_cards_sum = CGameConfig::gComCardPara.game_cards_num;

		//��Ϸ�ڿ���
		m_card_config.game_cards = new CARD_DESC[m_card_config.game_cards_sum];
		for (int i = 0; i < m_card_config.game_cards_sum; i++)
		{
			m_card_config.game_cards[i] = CGameConfig::gComCardPara.game_cards[i];
		}

		//��ҳ�ʼ������
		SetPlayerInitCardNum();
	}
	else
	{
		cout << "���õ�ȫ�ֱ���ERR" << endl;
	}

	return result;
}

// ������Ϸ���ô����������
int CGameData::LoadPlayers()
{
	int result = 0;

	PLAYER_TYPE _play_type = 0;		//һ��word�洢����ҵ�2������card_sum��player_sum

	//��ȫ�������ļ��ж�ȡ�������
	_play_type = CGameConfig::gComPlayerPara.type;

	// ����������ң������Ŀ�̶�Ϊ������������ӷ�������ܻ��
	m_players_config.players = new PLAYER_DESC[m_players_config.player_sum];
	memset(m_players_config.players, 0, sizeof(PLAYER_DESC)*m_players_config.player_sum);

	CGamePlayerConfig::CreatePlayerDesc(m_players_config.players, m_players_config.player_sum, _play_type);

	return result;
}

// ������Ϸ����
int  CGameData::LoadGameRule(const BYTE cbGameScoreMode)
{
	//��÷ֱ���
	if (cbGameScoreMode == GAME_SCORE_MODE_CLASSIC)	//����ģʽ
	{
		memcpy(&m_room_config.game_score_times, &CGameConfig::gComGamePara.game_normal_times, sizeof(m_room_config.game_score_times));
	}
	else if (cbGameScoreMode == GAME_SCORE_MODE_CRAZY)	//���ģʽ
	{
		memcpy(&m_room_config.game_score_times, &CGameConfig::gComGamePara.game_crazy_times, sizeof(m_room_config.game_score_times));
	}

	return 0;
}

// ����ƾ���Ϣ
int CGameData::OutGameInfor()
{
	/*TODOLATER
	//m_plog->LogSingleLine(L"ׯ�ң� %d", m_room_config.appointbanker ); // ���ׯ��ID

	// ����������
	for ( unsigned i = 0 ;i < GetCurPlayerCount() ; ++i)
	{
		wchar_t **_str = new wchar_t* [__GETCARDSUM( m_players_config.players[i].type ) + 1]();
		_str[0] = new wchar_t[64]();
		swprintf_s(_str[0], 64,  L"%s\t%s\t%s\n", L"����", L"��ɫ", L"��ID");
		for ( int j = 1 ;j < __GETCARDSUM( m_players_config.players[i].type ) + 1 ; ++j )
		{
			// ��仺����
			_str[j] = new wchar_t[64]();
			swprintf_s(_str[j], 64,  L"%d\t%d\t%d\n", __GETSCORE( m_players_config.hand_cards[i][j - 1]),  __GETCOLOR( m_players_config.hand_cards[i][j - 1]), __GETGROUPID( m_players_config.hand_cards[i][j - 1] ));
		}
		int *a = NULL;
		// д��־
		//m_plog->LogMutilLine(_str, a, __GETCARDSUM( m_players_config.players[i].type ) + 1);

		// ����
		for ( int j = 0; j < __GETCARDSUM( m_players_config.players[i].type ) + 1; ++ j )
		{
			delete [] _str[j];
			_str[j] = NULL;
		}

		delete [] _str;
		_str = NULL;
	}

	// �����ҵ÷�
	int nPlayerNum = GetCurPlayerCount() + 1;
	wchar_t **_str = new wchar_t* [nPlayerNum]();
	_str[0] = new wchar_t[64]();
	swprintf_s(_str[0], 64,  L"%s\t%s\t%s\t%s\t%s\t%s\t%s\n", L"����", L"��ע", L"����", L"������", L"���ַ�", L"�ܵ÷�", L"����", L"���Type");
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

	// д��־
	int *a = NULL;
	//m_plog->LogMutilLine(_str, a, nPlayerNum);

	for ( unsigned j = 0; j < nPlayerNum; ++ j )
	{
		delete [] _str[j];
		_str[j] = NULL;
	}

	delete [] _str;
	_str = NULL;

	//m_plog->LogSingleLine(L"========================== �����ķָ��� ==========================", NULL);

	return 0;
*/
	return 0;
}

// ��ʼ����Ϸ����
int CGameData::InitGameData()
{
	//m_playing_para.appointbanker = 0;		//��ǰׯ��

	//���뷿��ʱ������Ϊ0
	for (unsigned i = 0; i < m_players_config.player_sum; ++i)
	{
		m_players_config.players[i].total_score = 0;
		m_players_config.players[i].winsum = 0;
		m_players_config.players[i].level = START_LEVEL_VALUE;
		m_players_config.players[i].bet = 1;   //��ʼ�ӱ����Ʊ���Ϊ1
		m_players_config.players[i].bank_count = 0;
		m_players_config.players[i].rob_num = 0;
		m_players_config.players[i].best_bet = 0;  //�������ʼ��Ϊ0
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

	//��ʼ�����䱶��
	m_playing_para.room_bet = 1;
	m_playing_para.turn_cards_num = 0;   //���������ݳ�ʼ��
	m_playing_para.jiaofen_count = 0;	 //��ʼ���зִ���
	m_playing_para.leave_card_bet = 1;
	m_playing_para.game_cur_count = 0;

	//����Ȩλ��ʼ��
	for (BYTE i = 0; i < LEN_SORT_RIGHT; i++)
		m_playing_para.sort_right[i] = i * COLOR_RIGHT;

	//����ɫ�ͼ��Ƴ�ʼ��
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


// ���������ͻ��˿���ת��
int CGameData::CardTrans_S2C(const CARD_DESC &card_s, BYTE &card_c)
{
	//������ͺͻ�ɫ����Ҫ����id
	card_c = card_s & 0xff;		//ȡ�Ͱ�λ

	return 0;
}

// �ͻ��˵�����������ת��
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
			//m_plog->LogSingleLine(L"���ƽ���ʧ��: %d", card_c );
			result = -1;
		}
	}

	return result;

}

//����ת��	(��32���Ƶ�ʹ�����ת��Ϊһ��32λ����)
int CGameData::TransListToInt(int iOldType, int iNewType)
{
	//����У��
	if (iNewType == 0) return 0;

	//��������
	int nList = 0;
	nList |= (1 << iNewType);

	//cout << "ת����һ��32λ����: " <<nList << endl;

	return iOldType | nList;
}

//���Ժ��Ƶĺ���--�߶�
void CGameData::SetCardData_Leo(const WORD &wChairID, CARD_DESC *cards)
{
	BYTE cardtmp[108] = { 0x4f,0x4f,0x22,0x32,0x31,0x31,0x3d,0x3d,0x3c,0x3c,0x3b,0x3b,0x3a,0x3a,0x2c,0x2b,0x29,0x0d,0x17,0x0c,0x0b,0x0b,0x0a,0x09,0x07,
					   0x4e,0x22,0x12,0x12,0x2a,0x29,0x28,0x25,0x24,0x39,0x39,0x37,0x36,0x35,0x34,0x33,0x33,0x1b,0x1a,0x19,0x18,0x17,0x16,0x1d,0x14,
					   0x4e,0x32,0x02,0x2b,0x2a,0x26,0x23,0x23,0x38,0x38,0x37,0x35,0x11,0x1c,0x1b,0x18,0x0c,0x08,0x08,0x07,0x06,0x05,0x05,0x04,0x03,
					   0x21,0x21,0x2d,0x2d,0x2c,0x28,0x27,0x26,0x25,0x24,0x34,0x1d,0x1c,0x1a,0x19,0x16,0x15,0x15,0x13,0x01,0x0d,0x0a,0x06,0x04,0x03,
					   0x13,0x02,0x11,0x14,0x36,0x27,0x09,0x01 };

	//��ֵ
	for (int i = 0; i < 108; i++)
	{
		cards[i] = cardtmp[i];
	}
}

//�˿�����ת��������
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

//�˿�����ת�������� cardIndex:��0-12�� 13-25÷�� 26-38���� 39-51���� 65-66С��������
BYTE __stdcall CGameData::SwitchToCardIndex(BYTE cbCardData)
{
	BYTE card = ((cbCardData&MASK_COLOR) >> 4) * 13 + (cbCardData&MASK_VALUE) - 1;
	return card;	//����λ��ɫ������λ����
}

//�˿���������ת����������
BYTE __stdcall CGameData::SwitchToCardData(const BYTE cbCardIndex[LEN_MAX_INDEX], BYTE cbCardData[MAX_CARD_COUNT])
{
	//ת���˿�
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

//�˿���������ת������������
BYTE __stdcall CGameData::SwitchToCardIndex(const BYTE *cbCardData, BYTE cbCardCount, BYTE *cbCardIndex)
{
	//���ñ���
	ZeroMemory(cbCardIndex, sizeof(BYTE)*LEN_MAX_INDEX);

	//ת���˿�
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}

//ɾ���˿�
bool __stdcall CGameData::RemoveCard(const BYTE *cbRemoveCard, BYTE cbRemoveCount, BYTE *cbCardData, BYTE cbCardCount)
{
	//��������
	if (cbRemoveCount > cbCardCount)
	{
		cout << "��������ʧ��" << endl;
		return false;

	}

	//�������
	BYTE cbDeleteCount = 0;
	BYTE cbTempCardData[MAX_CARD_COUNT];
	if (cbCardCount > MAX_CARD_COUNT)
	{
		cout << "if (cbCardCount > MAX_CARD_COUNT)" << endl;
		return false;
	}
	CopyMemory(cbTempCardData, cbCardData, MAX_CARD_COUNT * sizeof(cbCardData[0]));

	//����Ҫɾ�����˿�����
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

	//���¸�ֵ
	BYTE cbCardPos = 0;
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		if (cbTempCardData[i] != 0)
			cbCardData[cbCardPos++] = cbTempCardData[i];
	}

	//ȥ������ûɾ��������
	for (int i = cbCardPos; i < cbCardCount; i++)
	{
		cbCardData[i] = 0;
	}

	return true;
}

//�����������ɾ���˿�
bool __stdcall CGameData::RemoveCard(const WORD &wChairID, const BYTE *cbRemoveCard, BYTE cbRemoveCount)
{
	//������Ч����
	for (int i = 0; i < cbRemoveCount; i++)
	{
		if (INVALID_CARD == cbRemoveCard[i])
		{
			cbRemoveCount--;
		}
	}

	//��ҵ�ǰ������
	BYTE cbCardCount = GetUserCurCardNum(wChairID);
	//ɾ������
	if (RemoveCard(cbRemoveCard, cbRemoveCount, m_players_config.hand_cards[wChairID], cbCardCount))
	{
		m_players_config.players[wChairID].hand_card_num -= cbRemoveCount;
		return true;
	}
	return false;
}


//���õ�ǰ��ׯ�����
int __stdcall CGameData::SetCurRobUser(const WORD wChairID)
{
	m_playing_para.currobuser = wChairID;
	return 0;
}

//��ȡ��ǰ��ׯ�����
WORD __stdcall CGameData::GetCurRobUser()
{
	return m_playing_para.currobuser;
}

//��¼�����ע
int __stdcall CGameData::SetPlayerAddScore(WORD wChairID, WORD lScore)
{
	if (lScore < 1)
	{
		lScore = 1;
	}

	m_players_config.players[wChairID].addscore_bet = lScore;
	return 0;
}

//��ȡ�����ע
WORD __stdcall CGameData::GetPlayerAddScore(WORD wChairID)
{
	return m_players_config.players[wChairID].addscore_bet;
}

//��¼������Ʊ���
int __stdcall CGameData::SetPlayerMingPaiBet(WORD wChairID, WORD bet)
{
	if (bet < 1)
	{
		bet = 1;
	}

	m_players_config.players[wChairID].mingpai_bet = bet;
	return 0;
}


//��ȡ������Ʊ���
int __stdcall CGameData::GetPlayerMingPaiBet(WORD wChairID)
{
	return m_players_config.players[wChairID].mingpai_bet;
}


//����һ�������һ�ù��������
int __stdcall CGameData::SetDJPlayerBestBet(WORD wChairID, SCORE bet)
{
	m_players_config.players[wChairID].best_bet = bet;
	return 0;
}

//���һ�������һ�ù��������
SCORE __stdcall CGameData::GetDJPlayerBestBet(WORD wChairID)
{
	return  m_players_config.players[wChairID].best_bet;
}

//������Ϸ�ⶥ����
int __stdcall CGameData::SetRoomMaxBet(DWORD Max_Bet)
{
	if (Max_Bet == 0)   //���ⶥ
	{
		m_playing_para.max_room_bet = (1 << 30);
		return 0;
	}

	if (Max_Bet < 32)
		Max_Bet = 32;

	m_playing_para.max_room_bet = Max_Bet;

	return 0;
}

//��ȡ��Ϸ�ⶥ����
DWORD __stdcall CGameData::GetRoomMaxBet()
{

	return m_playing_para.max_room_bet;
}

//��¼����Ƿ�ʼ��Ϸ����  5-��ʼ��Ϸ����  4-��������
int __stdcall CGameData::SetStartMingPai(WORD wChairID, DWORD state)
{
	m_players_config.players[wChairID].start_mingpai = state;
	return 0;
}

//��ȡ����Ƿ�ʼ��Ϸ����
DWORD __stdcall CGameData::GetStartMingPai(WORD wChairID)
{
	return m_players_config.players[wChairID].start_mingpai;
}

//����һ��������¼�зִ��� 
int __stdcall CGameData::SetJiaoFenCount(WORD count)
{
	m_playing_para.jiaofen_count = count;
	return 0;
}

//��ýзִ��� 
WORD __stdcall CGameData::GetJiaoFenCount()
{
	return m_playing_para.jiaofen_count;
}

//��¼��ǰ��ҵĽз�״̬
int __stdcall CGameData::SetCurJiaoFenState(WORD wChairID, WORD state)
{
	m_players_config.players[wChairID].jiaofen_state = state;
	return 0;
}

//��õ�ǰ��ҵĽз�״̬
int __stdcall CGameData::GetCurJiaoFenState(WORD wChairID)
{
	return m_players_config.players[wChairID].jiaofen_state;
}

//���õ�ǰ���ڽзֵ����     
int __stdcall CGameData::SetCurJiaoFenUser(WORD wChairID)
{
	m_playing_para.cur_jiaofen_player = wChairID;
	return 0;
}

//��ȡ��ǰ���ڽзֵ����     
WORD __stdcall CGameData::GetCurJiaoFenUser()
{
	return m_playing_para.cur_jiaofen_player;
}

//�������ҵĸ��˱���
WORD __stdcall CGameData::CleanAllBet()
{
	for (int i = 0; i < GetCurPlayerCount(); i++)
	{
		m_players_config.players[i].bet = 1;
	}
	return 0;
}

//��ȡ����ׯ������
WORD __stdcall CGameData::GetTurnPassCount()
{
	return m_playing_para.turn_pass_count;
}

//���ò���ׯ������
int __stdcall CGameData::SetTurnPassCount(WORD count)
{
	m_playing_para.turn_pass_count = count;
	return 0;
}

//������Ϸ���ģʽ
int __stdcall CGameData::SetLaiZiMode(WORD Mode)
{
	if (Mode > 2 || Mode < 0)
	{
		Mode = 0;
	}

	m_room_config.game_laizi_mode = 0;
	return 0;
}

//��ȡ��Ϸ���ģʽ
int __stdcall CGameData::GetLaiZiMode()
{
	return m_room_config.game_laizi_mode;
}

//��ʼ����ǰ�����Ƹ�������
void __stdcall CGameData::GetTurnMaxCardToRobot(GameSituation &gst, AILogic &m_AILogic)
{
	memset(&(gst.uctNowCardGroup), 0, sizeof(gst.uctNowCardGroup));

	//��ȡ��ǰ�ֵĿ�������  ������
	DWORD m_type = GetCardLogicType(m_playing_para.turn_max_cards, m_playing_para.turn_cards_num);

	//��ֵ
	gst.NowOutUser = m_playing_para.cur_outcard_user;   //��ǰ�����û�
	gst.nCardDroit = m_playing_para.last_outcard_user;    //�������û�
	gst.nDiZhuID = m_playing_para.appointbanker;			//ׯ��

	//����
	int  nValue = 0;								//���Ƶļ�ֵ
	int  nCount = m_playing_para.turn_cards_num;		//���Ƶĸ���
	int  nMaxCard = 0;								//���о�����С����ֵ�����ڶԱ�

	//�����Ƚϻ�ȡ������ֵ�����������Ʋ���gst��ֵ
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
	else if ((1 << CT_THREE_LINE_TAKE_ONE) == m_type)    //����һ�ͷɻ�
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
	else if ((1 << CT_THREE_LINE_TAKE_TWO) == m_type)    //����һ�Ժͷɻ�
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

//��Ҫ�����ƴ��߼�ֵת�����л�ɫ
void __stdcall CGameData::GetCardColorValue(WORD OutCardUser, HandCardData &m_HandCardData, BYTE *OutCardData, BYTE &OutCardNum)
{
	BYTE CurCardData[MAX_CARD_COUNT] = { 0 };
	BYTE CurCardNum = GetUserCurCardNum(OutCardUser);

	GetClientHandCards(OutCardUser, CurCardData, CurCardNum);

	//��־��
	int index = 0;

	//һ��һ�����ȽϽ���
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

	//��OutCard����
	SortCardList(OutCardData, OutCardNum, ST_ORDER);

	return;
}

//��������Ƿ񷭱� ���ѱ���������ҳ���     �ú����ڷ��ƺ����
DWORD __stdcall CGameData::IsDoubleLeaveCard()
{
	//��ȡ���ŵ���
	BYTE LeaveCard[NORMAL_LEAVE_CARD_NUM] = { 0 };

	GetLeaveCard(LeaveCard, NORMAL_LEAVE_CARD_NUM);

	//�жϵ������Ͳ����ñ���
	if (((0x4F == LeaveCard[0] && 0x4E != LeaveCard[1]) || 0x4E == LeaveCard[0]))		m_playing_para.leave_card_bet = 2;
	if (0x4F == LeaveCard[0] && 0x4E == LeaveCard[1])									m_playing_para.leave_card_bet = 4;
	if (GetCardColor(LeaveCard[0]) == GetCardColor(LeaveCard[1]) && GetCardColor(LeaveCard[1]) == GetCardColor(LeaveCard[2]))		//ͬ��˳
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

//��ȡ���Ʒ�������
DWORD __stdcall CGameData::GetLeaveCardBet()
{
	return m_playing_para.leave_card_bet;
}

//�����Ƿ���Ʒ���
int __stdcall CGameData::SetISLeaveCardDouble(WORD Mode)
{
	if (Mode != 0 && Mode != 1)
		Mode = 1;

	m_room_config.game_leave_card = Mode;
	return 0;
}

//��ȡ�����Ƿ񷭱�
WORD __stdcall CGameData::GetISLeaveCardDouble()
{
	return m_room_config.game_leave_card;
}

//�����Ƿ�����			0-������   1-����
int __stdcall CGameData::SetMingPaiMode(WORD Mode)
{
	if (Mode != 0 && Mode != 1)
		Mode = 1;

	m_room_config.game_mingpai = Mode;
	return 0;
}

//��ȡ�Ƿ�����
WORD __stdcall CGameData::GetMingPaiMode()
{
	return m_room_config.game_mingpai;
}

//�����Ƿ�ӱ�       0-���ӱ�  1-�ӱ�
int __stdcall CGameData::SetAddBetMode(WORD Mode)
{
	if (Mode != 0 && Mode != 1)
		Mode = 1;

	m_room_config.game_addbet = Mode;
	return 0;
}

//��ȡ�Ƿ�ӱ�
WORD __stdcall CGameData::GetAddBetMode()
{
	return m_room_config.game_addbet;
}

//��¼����Ƿ������
int __stdcall CGameData::SetSpring(WORD wChairID, WORD spring)
{
	m_players_config.players[wChairID].spring_state += spring;
	return 0;
}

//�ж�����Ƿ���  0-���� 1-��  ���ϱ���
int __stdcall CGameData::JudgeSpring(WORD wChairID)
{
	if (INVALID_CHAIR == m_playing_para.appointbanker)
	{
		return -1;
	}

	int flag = 0;
	if (m_playing_para.appointbanker == wChairID)   //ׯ��Ӯ��
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
	//ũ��Ӯ��
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

//��ȡ����Ƿ���
int __stdcall CGameData::GetSpring(WORD wChairID)
{
	return m_players_config.players[wChairID].spring;
}

//��������й�
int __stdcall CGameData::SetPlayerTuoGuan(WORD wChairID, WORD state)
{
	m_players_config.players[wChairID].player_tuo_guan = state;
	return 0;
}

//��ȡ����й�
int __stdcall CGameData::GetPlayerTuoGuan(WORD wChairID)
{
	return m_players_config.players[wChairID].player_tuo_guan;
}

//�������������ʱ���ƴ���
int __stdcall CGameData::SetPlayerTimeOutNum(WORD wChairID, WORD num)
{
	m_players_config.players[wChairID].out_time_num = num;
	return 0;
}

//��ȡ���������ʱ���ƴ���
int __stdcall CGameData::GetPlayerTimeOutNum(WORD wChairID)
{
	return m_players_config.players[wChairID].out_time_num;
}

//================================================��ӳ��ж�=================================================   add by lih

//����ڰ�����ӵĿ��Ƶ��߼���ֵ
BYTE CGameData::GetCardLogicValueLaiZi(BYTE cbCardData)
{
	if (0 == cbCardData)
	{
		return 0;
	}

	//�߼���ֵ
	BYTE cbCardColor = GetCardColor(cbCardData);
	BYTE cbCardValue = GetCardValue(cbCardData);

	//ת����ֵ���Գ��ִ���
	if ((cbCardColor << 4) == 0x40) return cbCardValue + 2;		//��С��   

	if (m_room_config.game_laizi_mode > 0)
	{
		//��ӷ��������
		if ((m_playing_para.magic_card != 0) && (m_playing_para.magic_card == 16 * 5 + cbCardValue))     //���������
		{
			return (cbCardValue + 16 * 5);
		}
	}

	return (cbCardValue <= 2) ? (cbCardValue + 13) : cbCardValue;

}

//������ӵ��ɵ���ͨ��
int CGameData::SetLaiZiToCommon(BYTE LaiZiData[], BYTE Count)
{
	if (LaiZiData == NULL)
		return false;

	ZeroMemory(&m_playing_para.laizi_transport, sizeof(m_playing_para.laizi_transport));
	memcpy(m_playing_para.laizi_transport, LaiZiData, Count * sizeof(BYTE));

	m_playing_para.transport_count = Count;

	return 0;
}

//��ȡ��ӵ��ɵ���ͨ��
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

//����������е���ӽ�����ֵ�滻
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

//��������������ӵ�����
void CGameData::SortCardListNoLaiZi(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
	//��Ŀ����
	if (cbCardCount == 0) return;

	//ת����ֵ
	BYTE cbSortValue[MAX_CARD_COUNT];
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		cbSortValue[i] = GetCardLogicValue(cbCardData[i]);
	}

	//�������
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
				//����λ��
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

//�ж���ӳ�������Ƿ��ܳ���
bool CGameData::JudgeLaiZiPlayerOutCard(WORD wChairID)
{
	//У��
	if (wChairID < 0 || wChairID > MAX_CHAIR_COUNT)
		return false;

	//��ȡ��ҵ�����
	BYTE HandCardData[MAX_CARD_COUNT];
	BYTE HandCardNum = m_players_config.players[wChairID].hand_card_num;

	ZeroMemory(&HandCardData, sizeof(HandCardData));
	CopyMemory(HandCardData, m_players_config.hand_cards[wChairID], sizeof(BYTE)*HandCardNum);

	//��ȡ�������
	BYTE LaiZiNum = GetMagicCardNum(HandCardData, HandCardNum);

	cout << "LaiZiCount:::" << (int)LaiZiNum << endl;

	//������ӵ���ͨ�Ƶ���ֵ DOLATER
	BYTE ReplaceData[MAX_LAIZI_COUNT] = { 0 };		//��ӵ���ͨ�Ƶ�����
	BYTE ReplaceNum = 0;							//��ӵ���ͨ�Ƶ�����

													//����
	SetLaiZiToCommon(ReplaceData, ReplaceNum);

	//�Ƿ������
	bool LaiFlag = false;
	for (int i = 0; i < HandCardNum; i++)
	{
		if (m_playing_para.magic_card == GetCardLogicValueLaiZi(HandCardData[i]))
			LaiFlag = true;
		else
			LaiFlag = false;
	}

	//������������
	BYTE OutCardData[MAX_CARD_COUNT];
	BYTE OutCardNum;
	ZeroMemory(&OutCardData, sizeof(OutCardData));

	//�滻���鶨��
	BYTE tmpCard[MAX_CARD_COUNT];
	ZeroMemory(&tmpCard, sizeof(tmpCard));
	CopyMemory(tmpCard, HandCardData, sizeof(BYTE)*HandCardNum);

	//�����������ͨ��
	if (!LaiFlag && LaiZiNum != 0)
	{
		for (int a = 0; a < HandCardNum; a++)
		{
			if (m_playing_para.magic_card == GetCardLogicValueLaiZi(HandCardData[a]))
			{
				cout << "==============OneLai================" << endl;
				//һ�����
				for (int b = 0; b < 13; b++)
				{
					//���滻
					tmpCard[a] = m_cbCardData[b] + 0x50;

					//�������
					if (LaiZiNum >= 2)
					{
						for (int c = a + 1; c < HandCardNum; c++)
						{
							if (m_playing_para.magic_card == GetCardLogicValueLaiZi(HandCardData[c]))
							{
								cout << "==============TwoLai================" << endl;
								for (int d = 0; d < 13; d++)
								{
									//���滻
									tmpCard[c] = m_cbCardData[d] + 0x50;

									//�������
									if (LaiZiNum >= 3)
									{
										for (int e = c + 1; e < HandCardNum; e++)
										{
											if (m_playing_para.magic_card == GetCardLogicValueLaiZi(HandCardData[e]))
											{
												cout << "==============ThreeLai================" << endl;
												for (int f = 0; f < 13; f++)
												{
													//���滻
													tmpCard[e] = m_cbCardData[f] + 0x50;

													//�������
													if (LaiZiNum >= 4)
													{
														for (int g = e + 1; g < HandCardNum; g++)
														{
															if (m_playing_para.magic_card == GetCardLogicValueLaiZi(HandCardData[g]))
															{
																cout << "==============FourLai================" << endl;
																for (int h = 0; h < 13; h++)
																{
																	//���滻
																	tmpCard[g] = m_cbCardData[h] + 0x50;

																	//�������  д���������������

																	//�ж��Ƿ�Ҫ����
																	if (LaiZiCompareCard(tmpCard, HandCardNum, OutCardData, &OutCardNum))
																		return true;
																	else
																		continue;
																}
																break;
															}
														}
													}

													//�ж��Ƿ�Ҫ����
													if (LaiZiCompareCard(tmpCard, HandCardNum, OutCardData, &OutCardNum))
														return true;
													else
														continue;
												}
												break;
											}
										}
									}

									//�ж��Ƿ�Ҫ����
									if (LaiZiCompareCard(tmpCard, HandCardNum, OutCardData, &OutCardNum))
										return true;
									else
										continue;
								}
								break;
							}
						}
					}

					//�ж��Ƿ�Ҫ����
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

//��ӣ���ҵ�һ������  �ж������Գ��ĺ��ʵ���
bool CGameData::LaiZiAnalysePlayerOutCardFirst(WORD OutCardUsr, BYTE OutCardData[], BYTE *OutCardNum)
{
	//У��
	if (OutCardUsr < 0 || OutCardUsr > MAX_CHAIR_COUNT || OutCardData == NULL)
		return false;



	return true;
}

//��ӣ���ҽ���
bool CGameData::LaiZiAnalysePlayerOutCard(WORD OutCardUsr, BYTE OutCardData[], BYTE *OutCardNum)
{
	//У��
	if (OutCardUsr < 0 || OutCardUsr > MAX_CHAIR_COUNT || OutCardData == NULL)
		return false;

	//��ȡ��ҵ�����
	BYTE HandCardData[MAX_CARD_COUNT];
	BYTE HandCardNum = m_players_config.players[OutCardUsr].hand_card_num;

	ZeroMemory(&HandCardData, sizeof(HandCardData));
	CopyMemory(HandCardData, m_players_config.hand_cards[OutCardUsr], sizeof(BYTE)*HandCardNum);

	//��ȡ�������
	BYTE LaiZiNum = GetMagicCardNum(HandCardData, HandCardNum);

	cout << "LaiZiCount:::" << (int)LaiZiNum << endl;

	//������ӵ���ͨ�Ƶ���ֵ DOLATER
	BYTE ReplaceData[MAX_LAIZI_COUNT] = { 0 };		//��ӵ���ͨ�Ƶ�����
	BYTE ReplaceNum = 0;							//��ӵ���ͨ�Ƶ�����

													//����
	SetLaiZiToCommon(ReplaceData, ReplaceNum);

	//�Ƿ������
	bool LaiFlag = false;
	for (int i = 0; i < HandCardNum; i++)
	{
		if (m_playing_para.magic_card == GetCardLogicValueLaiZi(HandCardData[i]))
			LaiFlag = true;
		else
			LaiFlag = false;
	}

	//�滻���鶨��
	BYTE tmpCard[MAX_CARD_COUNT];
	ZeroMemory(&tmpCard, sizeof(tmpCard));
	CopyMemory(tmpCard, HandCardData, sizeof(BYTE)*HandCardNum);

	//�����������ͨ��
	if (!LaiFlag && LaiZiNum != 0)
	{
		for (int a = 0; a < HandCardNum; a++)
		{
			if (m_playing_para.magic_card == GetCardLogicValueLaiZi(HandCardData[a]))
			{
				cout << "==============OneLai================" << endl;
				//һ�����
				for (int b = 0; b < 13; b++)
				{
					//���滻
					tmpCard[a] = m_cbCardData[b] + 0x50;

					//�������
					if (LaiZiNum >= 2)
					{
						for (int c = a + 1; c < HandCardNum; c++)
						{
							if (m_playing_para.magic_card == GetCardLogicValueLaiZi(HandCardData[c]))
							{
								cout << "==============TwoLai================" << endl;
								for (int d = 0; d < 13; d++)
								{
									//���滻
									tmpCard[c] = m_cbCardData[d] + 0x50;

									//�������
									if (LaiZiNum >= 3)
									{
										for (int e = c + 1; e < HandCardNum; e++)
										{
											if (m_playing_para.magic_card == GetCardLogicValueLaiZi(HandCardData[e]))
											{
												cout << "==============ThreeLai================" << endl;
												for (int f = 0; f < 13; f++)
												{
													//���滻
													tmpCard[e] = m_cbCardData[f] + 0x50;

													//�������
													if (LaiZiNum >= 4)
													{
														for (int g = e + 1; g < HandCardNum; g++)
														{
															if (m_playing_para.magic_card == GetCardLogicValueLaiZi(HandCardData[g]))
															{
																cout << "==============FourLai================" << endl;
																for (int h = 0; h < 13; h++)
																{
																	//���滻
																	tmpCard[g] = m_cbCardData[h] + 0x50;

																	//�������  д���������������

																	//�ж��Ƿ�Ҫ����
																	if (LaiZiCompareCard(tmpCard, HandCardNum, OutCardData, OutCardNum))
																		return true;
																	else
																		continue;
																}
																break;
															}
														}
													}

													//�ж��Ƿ�Ҫ����
													if (LaiZiCompareCard(tmpCard, HandCardNum, OutCardData, OutCardNum))
														return true;
													else
														continue;
												}
												break;
											}
										}
									}

									//�ж��Ƿ�Ҫ����
									if (LaiZiCompareCard(tmpCard, HandCardNum, OutCardData, OutCardNum))
										return true;
									else
										continue;
								}
								break;
							}
						}
					}

					//�ж��Ƿ�Ҫ����
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

//��ӣ������Ƚ�����
bool CGameData::LaiZiCompareCard(BYTE Card[], BYTE CardCount, BYTE OutCardData[]/*out*/, BYTE *OutCardNum)
{
	//У��
	if (Card == NULL || OutCardData == NULL)
		return false;

	//Card����
	BYTE tmpCard[MAX_CARD_COUNT];
	ZeroMemory(&tmpCard, sizeof(tmpCard));
	CopyMemory(tmpCard, Card, sizeof(BYTE)*CardCount);

	//��������
	SortCardListNoLaiZi(tmpCard, CardCount, ST_ORDER);

	//����ֵ
	BYTE tmpOutCardData[MAX_CARD_COUNT];
	BYTE tmpOutCardNum = 0;
	ZeroMemory(&tmpOutCardData, sizeof(tmpOutCardData));

	//��ø�������������
	BYTE TurnCardNum = m_playing_para.turn_cards_num;
	BYTE TurnCardData[MAX_CARD_COUNT];
	ZeroMemory(&TurnCardData, sizeof(TurnCardData));
	CopyMemory(TurnCardData, m_playing_para.turn_max_cards, sizeof(TurnCardData));

	//��ȡ��ǰ��ҵ���������
	BYTE CurCardNum = CardCount;
	BYTE CurCardData[MAX_CARD_COUNT];
	ZeroMemory(&CurCardData, sizeof(CurCardData));

	for (int i = 0; i < CurCardNum; i++)
	{
		CurCardData[i] = tmpCard[i];
	}

	//���������˿�����
	tagAnalyseResult TurnResult;
	tagAnalyseResult CurResult;

	ZeroMemory(&TurnResult, sizeof(TurnResult));
	ZeroMemory(&CurResult, sizeof(CurResult));

	AnalysebCardData(CurCardData, CurCardNum, CurResult);
	AnalysebCardData(TurnCardData, TurnCardNum, TurnResult);

	//��ȡ������ҵ�����
	int TurnCardType = GetType(TurnResult, TurnCardData, TurnCardNum, TurnCardData);
	TurnCardType = TransListToInt(0, TurnCardType);

	//�Ա��ж�����
	if (TurnCardType == CT_ERROR)	return false;
	if ((TurnCardType & (1 << CT_MISSILE_CARD)) != 0)    return false;    //��ը�ж�

																		  //ը���ж�
	if ((TurnCardType & (1 << CT_BOMB_CARD)) != 0)   //�ϼҳ�ը��
	{
		cout << "ը��" << endl;

		//��ȡ�ϼҳ�����ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbFourCardData[0]);
		int index = 0;   //������ʶ

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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_RUAN_BOMB)) != 0)   //�ϼҳ���ը
	{
		cout << "��ը" << endl;

		//��ȡ�ϼҳ�����ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbFourCardData[0]);
		int index = 0;   //������ʶ

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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_LAIZI_BOMB)) != 0)   //���ը��
	{
		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_SINGLE)) != 0)   //����
	{
		cout << "����" << endl;

		//��ȡ�ϼҳ�����ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbSignedCardData[0]);

		cout << "��ȡ�ϼҳ�����ֵ:" << (int)TurnLogicValue << endl;

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

		//ը���ж�
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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_DOUBLE)) != 0)   //����
	{
		cout << "����" << endl;

		//��ȡ�ϼҳ�����ֵ
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

		//ը���ж�
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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_THREE)) != 0)   //����
	{
		cout << "����" << endl;

		//��ȡ�ϼҳ�����ֵ
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

		//ը���ж�
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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_SINGLE_LINE)) != 0)   //˳��
	{
		cout << "˳��" << endl;

		//��ȡ�ϼҳ�����ֵ  ���ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbSignedCardData[0]);
		cout << "˳��:��ȡ�ϼҳ�����ֵ:" << (int)TurnLogicValue << endl;

		int flag = 0;
		int index = 0;

		for (int i = 0; i < CurCardNum; i++)     //����˳������
		{
			if (flag != 0)    //�������ñ��ƴ�С
			{
				if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
				{
					continue;
				}
				if (GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //�ж��Ƿ�����
				{
					tmpOutCardData[index++] = CurCardData[i];
					tmpOutCardNum++;

					flag++;

					//��������
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
					if (GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //�ж��Ƿ�����
					{
						tmpOutCardData[index++] = CurCardData[i];
						tmpOutCardNum++;

						flag++;

						//��������
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

		//ը���ж�
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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_DOUBLE_LINE)) != 0)   //����
	{
		cout << "����" << endl;


		//��ȡ�ϼҳ�����ֵ  ���ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbDoubleCardData[0]);
		int flag = 0;   //�ж��Ƿ�������־
		int Numflag = 0;   //�ж������Ƿ���ı�־
		int index = 0;

		for (int i = 0; i < CurCardNum; i++)     //������������
		{
			if (flag != 0)    //�������ñ��ƴ�С
			{
				if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
				{
					Numflag++;
					continue;
				}
				if ((GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1))
					&& (Numflag > 0) && (GetCardLogicValue(CurCardData[i]) < 15))   //�ж��Ƿ�����
				{
					tmpOutCardData[index++] = CurCardData[i];		//���Լ�����
					tmpOutCardNum++;

					tmpOutCardData[index++] = CurCardData[i - 1];
					tmpOutCardNum++;

					flag++;

					//��������
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
						&& (Numflag > 0) && (GetCardLogicValue(CurCardData[i]) < 15))   //�ж��Ƿ�����
					{
						tmpOutCardData[index++] = CurCardData[i];		//���Լ�����
						tmpOutCardNum++;

						tmpOutCardData[index++] = CurCardData[i - 1];
						tmpOutCardNum++;

						flag++;

						//��������
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

		//ը���ж�
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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_THREE_LINE)) != 0)   //����
	{
		cout << "����" << endl;

		//��ȡ�ϼҳ�����ֵ  ���ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);
		int flag = 0;   //�ж��Ƿ�������־
		int Numflag = 0;   //�ж������Ƿ���ı�־
		int index = 0;

		for (int i = 0; i < CurCardNum; i++)     //������������
		{
			if (flag != 0)    //�������ñ��ƴ�С
			{
				if (GetCardLogicValue(CurCardData[i + 1]) == GetCardLogicValue(CurCardData[i]))
				{
					Numflag++;
					continue;
				}
				if ((GetCardLogicValue(CurCardData[i + 1]) == (GetCardLogicValue(CurCardData[i]) - 1))
					&& (Numflag > 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //�ж��Ƿ�����
				{
					tmpOutCardData[index++] = CurCardData[i];		//����������
					tmpOutCardNum++;

					tmpOutCardData[index++] = CurCardData[i - 1];
					tmpOutCardNum++;

					tmpOutCardData[index++] = CurCardData[i - 2];
					tmpOutCardNum++;

					flag++;

					//��������
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

				if (GetCardLogicValue(CurCardData[i + 1]) != GetCardLogicValue(CurCardData[i]))   //����ͬ���� ��־λ����
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
						&& (Numflag > 1) && (GetCardLogicValue(CurCardData[i]) < 15))   //�ж��Ƿ�����
					{
						tmpOutCardData[index++] = CurCardData[i];		//����������
						tmpOutCardNum++;

						tmpOutCardData[index++] = CurCardData[i - 1];
						tmpOutCardNum++;

						tmpOutCardData[index++] = CurCardData[i - 2];
						tmpOutCardNum++;

						flag++;

						//��������
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

					if (GetCardLogicValue(CurCardData[i + 1]) != GetCardLogicValue(CurCardData[i]))   //����ͬ���� ��־λ����
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

		//ը���ж�
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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_THREE_LINE_TAKE_ONE)) != 0)   //����һ
	{

		cout << "����һ" << endl;


		//��ȡ�ϼҳ�����ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);

		if (CurResult.cbSignedCount >= TurnResult.cbThreeCount)     //û�е��ž�û��Ҫ�������
		{
			if (TurnResult.cbThreeCount == 1)    //����һ
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
			else		//�ɻ�
			{
				int index = 0;
				int flag = 1;   //�жϷɻ��������ŵı�־
								//�ж������Ƿ�����
				for (int i = 0; i < CurResult.cbThreeCount; i++)
				{
					if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) - 1) == (GetCardLogicValue(CurResult.cbThreeCardData[index + 3])))  //����
					{
						flag++;
					}

					if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) >(TurnLogicValue - TurnResult.cbThreeCount + 2))
						&& (flag == TurnResult.cbThreeCount))   //��С�Ƚ�
					{
						index = index - TurnResult.cbThreeCount * 3 + 3;     //��ֵ���±�
						for (int j = 0; j < TurnResult.cbThreeCount * 3; j++)
						{
							tmpOutCardData[j] = CurResult.cbThreeCardData[index++];
						}

						for (int j = 0; j < TurnResult.cbThreeCount; j++)
						{
							tmpOutCardData[TurnResult.cbThreeCount * 3 + j] = CurResult.cbSignedCardData[TurnResult.cbSignedCount - j - 1];   //�ɻ����ĵ�
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

		//ը���ж�
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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_THREE_LINE_TAKE_TWO)) != 0)   //������
	{
		cout << "������" << endl;

		//��ȡ�ϼҳ�����ֵ
		BYTE TurnLogicValue = GetCardLogicValue(TurnResult.cbThreeCardData[0]);

		if (CurResult.cbDoubleCount >= TurnResult.cbThreeCount)     //û�ж��Ӿ�û��Ҫ�������
		{
			if (TurnResult.cbThreeCount == 1)    //������
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
			else		//�ɻ�
			{
				int index = 0;
				int flag = 1;   //�жϷɻ��������ŵı�־
								//�ж������Ƿ�����
				for (int i = 0; i < CurResult.cbThreeCount; i++)
				{
					if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) - 1) == (GetCardLogicValue(CurResult.cbThreeCardData[index + 3])))  //����
					{
						flag++;
					}

					if ((GetCardLogicValue(CurResult.cbThreeCardData[index]) >(TurnLogicValue - TurnResult.cbThreeCount + 2))
						&& (flag == TurnResult.cbThreeCount))   //��С�Ƚ�
					{
						index = index - TurnResult.cbThreeCount * 3 + 3;     //��ֵ���±�
						for (int j = 0; j < TurnResult.cbThreeCount * 3; j++)
						{
							tmpOutCardData[j] = CurResult.cbThreeCardData[index++];
						}

						for (int j = 0; j < TurnResult.cbThreeCount * 2; j += 2)
						{
							tmpOutCardData[TurnResult.cbThreeCount * 3 + j] = CurResult.cbDoubleCardData[TurnResult.cbDoubleCount * 2 - j - 1];   //�ɻ����Ķ�
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

		//ը���ж�
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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_FOUR_LINE_TAKE_ONE)) != 0)   //�Ĵ���
	{
		cout << "�Ĵ���" << endl;

		//ը���ж�
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

		//��ը�ж�
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
	else if ((TurnCardType & (1 << CT_FOUR_LINE_TAKE_TWO)) != 0)   //�Ĵ�����
	{
		cout << "�Ĵ�����" << endl;

		//ը���ж�
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

		//��ը�ж�
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
//================================================��ӳ��ж�=================================================