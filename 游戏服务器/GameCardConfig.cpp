#include "StdAfx.h"
#include "GameCardConfig.h"

#import "msxml6.dll"
using namespace MSXML2;
#include <time.h>

//���ڲ�ϴ��ģʽ�Ŀ�������
const BYTE CGameCardConfig::m_CardData[FULL_COUNT] = {
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
	0x4E,0x4F,
};


//�������͵���ֵӳ�亯��
BYTE    CGameCardConfig::MapOfCardValue(BYTE CardData)
{
	if (0 == CardData)
	{
		return 0;
	}

	//�ж�A��2�ʹ�С�������
	if (CardData > 140 && CardData < 160)
	{
		return ((CardData % 10 - 1) * 16 + (CardData / 10) - 13);
	}

	if (CardData > 160)
	{
		return (CardData == 165 ? 0x4E : 0x4F);
	}

	//�� ��ֵ+��ɫ   ==>  ��ɫ+��ֵ
	return ((CardData % 10 - 1) * 16 + CardData / 10);
}

/**
 * ��ȡһ���Ƶ�����
 * ʹ�� xml �����ļ� ����һ���Ƶ�����
 * @param  configfile				[in]			�����ļ�
 * @param  cards_sum				[in-out]		�˿���Ŀ
 * @param  m_plog						[in]			��־��¼
 * return					0				����
 *							-1				�쳣
 */
int CGameCardConfig::LoadCardsSum(CConfigFile&ff, DWORD &cards_sum)
{
	int result = 0;

	//����Ĭ��������
	cards_sum = 108;

	// ��ʼ���������
	cards_sum = MAX_TOTAL_CARD_NUM;			//Ĭ��ֵ

	ff.GetItemValue("GAME", "CardSum", cards_sum);
	if (cards_sum == 0)
	{
		CLog::Log(log_error, "��ȡʧ��");
		result = -1;
	}

	if (result == -1) // ����ʧ��
	{
		cards_sum = 0;
	}

	return result;
}


/**
* ��ȡ��ţ��Ϸ�ĵ÷ּӱ�ģʽ
* ʹ�� xml �����ļ� ������Ϸ�ĵ÷ּӱ�ģʽ
* @param  configfile					[in]			�����ļ�
* @param  game_score_Mode				[in]			��Ϸ�÷�ģʽ���������ģʽ
* @param  game_score_times				[in-out]		��Ϸ�÷ּӱ�����
* @param  m_plog						[in]			��־��¼
* return					0				����
*							-1				�쳣
*/
int CGameCardConfig::LoadGameScoreTimes(CConfigFile &ff, BYTE game_score_Mode, BYTE *game_score_times)
{
	int result = 0;
	memset(game_score_times, 1, sizeof(BYTE)*MAX_GAME_SCORE_TYPES);

	if (NULL == game_score_times)
	{
		return -1;
	}


	// ѡ������ģʽ
	BYTE type_sum = 0;
	ff.GetItemValue("CAME_SCORE_MODE_CLASSIC", "sum", type_sum);

	type_sum = (type_sum > MAX_GAME_SCORE_TYPES) ? MAX_GAME_SCORE_TYPES : type_sum;
	if (type_sum == 0)
	{
		CLog::Log(log_error, "��ȡ �÷�ģʽʧ��");
		return -2;
	}

	for (int j = 0; j < type_sum; j++)
	{
		char sz[20];
		sprintf(sz, "FF_%d", j);
		ff.GetItemValue("CAME_SCORE_MODE_CLASSIC", sz, game_score_times[j]);
	}

	// ѡ�����ӱ�ģʽ
	// ��ʱ����Ҫ����

	return result;
}



/**
 * ����� 
 * ����XML �� ��������ڵ���Ϣ 
 * @param  configfile				[in]			�����ļ�
 * @param  ppgroups					[out]			�˿�����
 * @param  group_sum				[in-out]		��ʵ������
 * @m_plog							[in]			��־��¼
 * return							 0				����
 *							        -1				�쳣
 */
int CGameCardConfig::LoadCardGroups(CConfigFile &ff, CARD_GOURP *groups, DWORD &group_sum)
{
	int result = 0;

	if (NULL == groups)
	{
		return -1;
	}

	BYTE card_group_num = 0;
	ff.GetItemValue("GAME", "CardGroupNum", card_group_num);

	cout << "card_group_num: " << (int)card_group_num << endl;

	//ȡ��С�Ŀ���������
	group_sum = (card_group_num > MAX_CARD_GROUP_NUM) ? MAX_CARD_GROUP_NUM : card_group_num;

	if (group_sum <= 0)
	{
		CLog::Log(log_error, "��ȡ�����ļ�, card group numʧ��");
		return -1;
	}

	for (int j = 0; j < group_sum; j++)	 // �������е� CARD_GOURP
	{
		char psz[20];
		sprintf(psz, "CARD_GROUP_%d", j);
		ff.GetItemValue(psz, "color", groups[j].color);
		ff.GetItemValue(psz, "begin", groups[j].begin);
		ff.GetItemValue(psz, "end", groups[j].end);
		ff.GetItemValue(psz, "sum", groups[j].sum);
	}

	return result;
}

/**
 * ��ָ����������ʶ������ת��Ϊֽ�������е�����
 * �˺������˿��б����ҳ��������� group ������ֽ�Ʋ���䵽 cards �����У� ������ִ����ɺ� cards �����еĳ�Ա��
 * @param		group					[in]			�˿���
 * @param		card_ranks				[in]			�˿�����
 * @param		card_ranks_sum			[in]			�˿�������Ŀ
 * @param		cards					[in-out]		�˿�����
 * @param		cardslen				[in]			�˿����鳤��
 * @param		curlen					[in-out]		�Ѿ����ĳ��ȣ� ע�⣬�˲����Ὣ����ֵ������ȥ��ֵ���
 *														���Դ˲��������ֵ������ֵ�й�
 */
int CGameCardConfig::TransGroup(const CARD_GOURP *group,
	CARD_DESCS cards,
	const DWORD maxlen,
	DWORD &curIndex)

{
	// �������У��
	if (NULL == group ||
		NULL == cards ||
		maxlen < 1 ||
		curIndex < 0)

	{
		return -1;
	}

	int result = 0;

	//У������
	if (group->begin < 1 || group->begin >15
		|| group->end < 1 || group->end  >15
		|| group->begin > group->end)
	{
		return -1;
	}

	// ����ֽ���б��ҳ����з�����������
	for (unsigned i = group->begin; i <= group->end; ++i)	//���Ƶ���
	{
		for (unsigned k = 0; k < group->sum; ++k)		//ͬһ���м���
		{
			// ��ɫ
			if (curIndex < maxlen) // ���card �������Ƿ��п�λ
			{
				cards[curIndex] = i; //����λΪ��ֵ
				cards[curIndex] += group->color * (0xF + 1);// ��仨ɫ 5-8λ��ʾ��ɫ
				cards[curIndex] += group->id * (0xF + 1) * (0xF + 1); //9-12λ��ʾid
				++(curIndex);
			}
			else
			{
				// ���
				result = -1;
			}
		}
	}

	return result;
}

/**
 * ��ȡ������
 * �˺������˿��б����ҳ��������� group ������ֽ�Ʋ���䵽 cards �����У� ������ִ����ɺ� cards �����еĳ�Ա��
 * @param		groups					[in]			�˿��� ����
 * @param		groups_len				[in]			�˿��� ������󳤶ȣ�ע�⣬����Ҫ��������ʵ�ʳ�Ա����
 * @param		card_ranks				[in]			�˿�����
 * @param		card_ranks_sum			[in]			�˿�������Ŀ
 * @param		ppcards					[out]			�˿�����
 * @param		cardslen				[in-out]		�˿����鳤��,������ⳤ�ȣ�����ʵ�ʳ���
 */
int CGameCardConfig::TransGroups(const CARD_GOURP group[MAX_CARD_GROUP_NUM], const int &nGroupLen,
	CARD_DESC *pGameCards, DWORD &dwCardSum)
{
	//У��
	if (NULL == pGameCards || 0 > nGroupLen)
	{
		return -1;
	}

	// ���������ʼ��
	DWORD curIndex = 0;

	// ����ת��
	int result = 0;
	for (int i = 0; i < nGroupLen; ++i)
	{
		if (0 != group[i].sum) //��鵱ǰ���Ƿ�Ϊ��
		{
			result = TransGroup(&group[i], pGameCards, dwCardSum, curIndex); // ��������
			if (0 != result)
			{
				// �����쳣
				//if (NULL != m_plog)
				//TODOLATER
				//m_plog->LogSingleLine(L"��������쳣", NULL);
				result = -1;
				break;
			}
		}
	}

	// ����У�飬�������С�������������ʵ�ʿ�������
	if ((curIndex + 1) < dwCardSum)
	{
		dwCardSum = curIndex;
	}

	return result;
}


/**
 * ����һ����������
 * �˺���������һ��arlen ���ȵ��������飨0��src_len-1������䵽array��0��src_len-1λ��
 * @param			randarray			[in-out]		����	
 * @param			src_len				[in]			���鳤��	
 * @param			m_plog				[in]			��־	
 */
int CGameCardConfig::CreateOutOrderArray( DWORD randarray[], const int src_len )
{
	// ����У��
	if( NULL == randarray )
	{

		return -1;
	}

	// ����һ����������
	memset( randarray, 0, src_len * sizeof( DWORD ));
	for( int i = 0; i < src_len ; i++ )
	{
		// ����һ��0����src_len - i - 1 ��֮��������
		srand((unsigned)time(NULL));
		int randsum = rand()%( src_len - i );

		// �� i ��� randsum ����λ���� 0 ��ʼ �㣩
		int index = 0; // ��λ����
		for( int j = 0; j < src_len; j++)
		{
			if( 0 == randarray[j] )
			{
				// �ж��Ƿ�����Ҫ��λ��
				if( index == randsum )
				{
					randarray[j] = i;
					break;
				}

				++index;
			}
		}
	}

	return 0;
}

////////////////////////////////////////////
//��ϴ��ģʽ��ϴ���㷨       by  lih
int CGameCardConfig::CreateBuXiPaiArray(BYTE randarray[], const int src_len)
{
	cout << "func CreateBuXiPaiArray begin" << endl;

	// ����У��
	if (NULL == randarray)
	{
		return -1;
	}

	BYTE count = 0;
	BYTE index[2] = { 0 };

	//��������±꽻��15��   //���Ʋ��� �ĳ�7��
	while (count < 7)
	{
		index[0] = rand() % src_len;
		index[1] = rand() % src_len;

		if (index[0] != index[1])
		{
			BYTE tmpSwapCard = randarray[index[0]];
			randarray[index[0]] = randarray[index[1]];
			randarray[index[1]] = tmpSwapCard;

			count++;
		}
	}



	cout << "func CreateBuXiPaiArray end" << endl;

	return 0;
}

bool CGameCardConfig::flushcard(BYTE card[], int &cardsum, BYTE boomcard[], int &boomsum)
{
	bool cardflag[13] = { 0 };
	srand((unsigned)time(NULL));

	BYTE tmpcard[54] = { 0 };
	cardsum = 54;
	memcpy(tmpcard, m_CardData, sizeof(BYTE) * 54);

	BYTE tmpboom[54] = { 0 };
	int boomcount = 0;

	//���2~7��ը��
	int boomNum = (rand() % 6) + 2;

	//���ȡը��
	int index = 0;
	for (int i = 0; i < boomNum; i++)
	{
		int flag = rand() % 13;
		if (true == cardflag[flag])
			continue;

		cardflag[flag] = true;
		boomcount++;
		cardsum -= 4;

		for (int j = 0; j < 54; j++)
		{
			if (flag + 1 == (tmpcard[j] & 0x0f))
			{
				tmpboom[index++] = tmpcard[j];
				tmpcard[j] = 0;
			}
		}
	}

	//ϴ��
	int iSend = 0;
	BYTE flushcard[54] = { 0 };
	do
	{
		int station = rand() % (54 - iSend);
		flushcard[iSend] = tmpcard[station];
		iSend++;
		tmpcard[station] = tmpcard[54 - iSend];
	} while (iSend < 54);

	//ȥ����Ч����
	index = 0;
	for (int i = 0; i < 54; i++)
	{
		if (flushcard[i] != 0)
		{
			card[index++] = flushcard[i];
		}
	}

	if (index != cardsum)  return false;

	memcpy(boomcard, tmpboom, sizeof(BYTE)*boomcount * 4);
	boomsum = boomcount;

	return true;
}

////////////////////////////////////////////

/** 
 * ������������, ϴ��
 * ÿ�ֿ�ʼʱ, ���Ƶ�˳�����
 * @param		dest_cards			[in-out]		Ŀ�����飬 ����������
 * @param		dest_len			[in]			Ŀ�����鳤��
 * @param		src_cards			[in]			Դ�����飬 �������ļ����ɵ���Ϸ����
 * @param		src_len				[in]			Դ�������� ע�⣬�����ݱ����������ʵ�ʳ���
 */
int CGameCardConfig::OutOrder(		CARD_DESCS dest_cards, 
									const DWORD dest_len, 
									const CARD_DESCS source_cards, 
									const DWORD src_len, 
									const BYTE nMode,
									CARD_DESCS boom_cards,
									BYTE &boomsum)
{
	// У���������
	if( NULL == dest_cards	||
		NULL == source_cards ||
		dest_len < src_len  )
	{
		return -1;
	}

	int result = 0;

	if (GAME_SCORE_MODE_CLASSIC == nMode)
	{
		// ����һ����������
		DWORD *randarray = new DWORD[src_len](); // ���򻺳���

		if (0 == CreateOutOrderArray(randarray, src_len))
		{
			for (unsigned i = 0; i < src_len; ++i)
			{
				dest_cards[i] = source_cards[randarray[i]];
			}
		}
		else
		{
			// �������д���
			result = -1;
		}

		delete[] randarray;
	}
	else
	{
		// ����һ����ϴ��ģʽ����������
		BYTE *tmpCards = new BYTE[src_len]();
		int tmpNum = 0;
		BYTE *BoomCards = new BYTE[src_len]();
		int BoomNum = 0;

		if (flushcard(tmpCards, tmpNum, BoomCards, BoomNum))
		{
			memcpy(dest_cards, tmpCards, sizeof(BYTE)*tmpNum);
			memcpy(boom_cards, BoomCards, sizeof(BYTE)*BoomNum * 4);
			boomsum = BoomNum;
		}
		else
		{
			result = -1;
		}
	}

	return result;
}

/**
 * �Ƚ������ƴ�С
 * @param			self			[in]		��	
 * @param			rival			[in]		��	
 * @param			flag			[in-out]	���أ� ��self ��, �򷵻�TRUE, ���򷵻�FLASE	
 */
int CGameCardConfig::CompareCards( const CARD_DESC *self, const CARD_DESC *rival, BOOL *flag)
{
	if ( NULL == flag)
	{
		return 0;
	}

	BYTE _self_score = GetCardScore( *self );
	BYTE _self_color = GetCardColor( *self );
	BYTE _rival_score = GetCardScore( *rival );
	BYTE _rival_color = GetCardColor( *rival );

	BOOL _flag = TRUE;
	if ( _self_score > _rival_score )
	{
		_flag = TRUE;
	}
	else if ( _self_score == _rival_score )
	{
		if ( _self_color > _rival_color )
		{
			_flag = TRUE;
		}
		else
		{
			_flag = FALSE;
		}
	}
	else
	{
		_flag = FALSE;
	}

	*flag = _flag;
	return 0;
}

/**
 * ������
 * �Ȱ������ţ�Ȼ�󰴻�ɫ��, ��ʽΪ��С����
 * @param			cards				[in-out]		����	
 * @param			cardslen			[in]			���鳤��	
 */
int CGameCardConfig::OrderCards( CARD_DESCS cards, const DWORD cardslen )
{
	// ����У��

	// ����
	int result = 0;

	CARD_DESC tmp_cards;

	for ( unsigned i = 0; i < cardslen - 1; ++i )
	{
		for( unsigned j = 0; j < cardslen - 1 - i; ++j) 
		{
			BOOL flag = TRUE;
			CompareCards( &cards[j], &cards[j+1], &flag);

			if( TRUE == flag )
			{
				tmp_cards	= cards[j];
				cards[j]	= cards[j+1];
				cards[j+1]	= tmp_cards;
			}
		}
	}

	return result;
}


/**
 * ��ȡ���Ƶ���
 */
BYTE CGameCardConfig::GetCardScore( CARD_DESC card )
{
	return (card & 0x000F); //ȡ0-4λ
}

/**
* ��ȡ���ƻ�ɫ
*/
BYTE CGameCardConfig::GetCardColor(CARD_DESC card)
{
	return (card & 0x00F0 ) >> 4; //ȡ4-8λ << 4
}

/**
* ��ȡ������
*/
BYTE CGameCardConfig::GetCardGroup( CARD_DESC card )
{
	return (card & 0x0F00) >> 8; //ȡ8-12λ
}