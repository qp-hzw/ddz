#include "StdAfx.h"
#include "GameCardConfig.h"

#import "msxml6.dll"
using namespace MSXML2;
#include <time.h>

//用于不洗牌模式的卡牌数组
const BYTE CGameCardConfig::m_CardData[FULL_COUNT] = {
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
	0x4E,0x4F,
};


//两种牌型的牌值映射函数
BYTE    CGameCardConfig::MapOfCardValue(BYTE CardData)
{
	if (0 == CardData)
	{
		return 0;
	}

	//判断A和2和大小王的情况
	if (CardData > 140 && CardData < 160)
	{
		return ((CardData % 10 - 1) * 16 + (CardData / 10) - 13);
	}

	if (CardData > 160)
	{
		return (CardData == 165 ? 0x4E : 0x4F);
	}

	//从 牌值+花色   ==>  花色+牌值
	return ((CardData % 10 - 1) * 16 + CardData / 10);
}

/**
 * 获取一副牌的数量
 * 使用 xml 配置文件 解析一副牌的数量
 * @param  configfile				[in]			配置文件
 * @param  cards_sum				[in-out]		扑克数目
 * @param  m_plog						[in]			日志记录
 * return					0				正常
 *							-1				异常
 */
int CGameCardConfig::LoadCardsSum(CConfigFile&ff, DWORD &cards_sum)
{
	int result = 0;

	//升级默认两副牌
	cards_sum = 108;

	// 初始化输出参数
	cards_sum = MAX_TOTAL_CARD_NUM;			//默认值

	ff.GetItemValue("GAME", "CardSum", cards_sum);
	if (cards_sum == 0)
	{
		CLog::Log(log_error, "获取失败");
		result = -1;
	}

	if (result == -1) // 解析失败
	{
		cards_sum = 0;
	}

	return result;
}


/**
* 获取斗牛游戏的得分加倍模式
* 使用 xml 配置文件 解析游戏的得分加倍模式
* @param  configfile					[in]			配置文件
* @param  game_score_Mode				[in]			游戏得分模式：经典或疯狂模式
* @param  game_score_times				[in-out]		游戏得分加倍数组
* @param  m_plog						[in]			日志记录
* return					0				正常
*							-1				异常
*/
int CGameCardConfig::LoadGameScoreTimes(CConfigFile &ff, BYTE game_score_Mode, BYTE *game_score_times)
{
	int result = 0;
	memset(game_score_times, 1, sizeof(BYTE)*MAX_GAME_SCORE_TYPES);

	if (NULL == game_score_times)
	{
		return -1;
	}


	// 选出经典模式
	BYTE type_sum = 0;
	ff.GetItemValue("CAME_SCORE_MODE_CLASSIC", "sum", type_sum);

	type_sum = (type_sum > MAX_GAME_SCORE_TYPES) ? MAX_GAME_SCORE_TYPES : type_sum;
	if (type_sum == 0)
	{
		CLog::Log(log_error, "读取 得分模式失败");
		return -2;
	}

	for (int j = 0; j < type_sum; j++)
	{
		char sz[20];
		sprintf(sz, "FF_%d", j);
		ff.GetItemValue("CAME_SCORE_MODE_CLASSIC", sz, game_score_times[j]);
	}

	// 选出疯狂加倍模式
	// 暂时不需要增加

	return result;
}



/**
 * 填充组 
 * 解析XML 中 所有牌组节点信息 
 * @param  configfile				[in]			配置文件
 * @param  ppgroups					[out]			扑克配置
 * @param  group_sum				[in-out]		组实际数量
 * @m_plog							[in]			日志记录
 * return							 0				正常
 *							        -1				异常
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

	//取最小的卡牌牌组数
	group_sum = (card_group_num > MAX_CARD_GROUP_NUM) ? MAX_CARD_GROUP_NUM : card_group_num;

	if (group_sum <= 0)
	{
		CLog::Log(log_error, "读取配置文件, card group num失败");
		return -1;
	}

	for (int j = 0; j < group_sum; j++)	 // 遍历所有的 CARD_GOURP
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
 * 将指定的组所标识的牌组转换为纸牌数组中的数据
 * 此函数从扑克列表中找出所有满足 group 条件的纸牌并填充到 cards 数组中， 并返回执行完成后 cards 数组中的成员数
 * @param		group					[in]			扑克组
 * @param		card_ranks				[in]			扑克配置
 * @param		card_ranks_sum			[in]			扑克排列数目
 * @param		cards					[in-out]		扑克数组
 * @param		cardslen				[in]			扑克数组长度
 * @param		curlen					[in-out]		已经填充的长度， 注意，此参数会将现有值与填充进去的值相加
 *														所以此参数的输出值与输入值有关
 */
int CGameCardConfig::TransGroup(const CARD_GOURP *group,
	CARD_DESCS cards,
	const DWORD maxlen,
	DWORD &curIndex)

{
	// 输入参数校验
	if (NULL == group ||
		NULL == cards ||
		maxlen < 1 ||
		curIndex < 0)

	{
		return -1;
	}

	int result = 0;

	//校验数据
	if (group->begin < 1 || group->begin >15
		|| group->end < 1 || group->end  >15
		|| group->begin > group->end)
	{
		return -1;
	}

	// 遍历纸牌列表，找出所有符合条件的牌
	for (unsigned i = group->begin; i <= group->end; ++i)	//卡牌点数
	{
		for (unsigned k = 0; k < group->sum; ++k)		//同一张有几张
		{
			// 花色
			if (curIndex < maxlen) // 检查card 缓冲区是否还有空位
			{
				cards[curIndex] = i; //低四位为牌值
				cards[curIndex] += group->color * (0xF + 1);// 填充花色 5-8位表示花色
				cards[curIndex] += group->id * (0xF + 1) * (0xF + 1); //9-12位表示id
				++(curIndex);
			}
			else
			{
				// 溢出
				result = -1;
			}
		}
	}

	return result;
}

/**
 * 提取所有组
 * 此函数从扑克列表中找出所有满足 group 条件的纸牌并填充到 cards 数组中， 并返回执行完成后 cards 数组中的成员数
 * @param		groups					[in]			扑克组 数组
 * @param		groups_len				[in]			扑克组 数组最大长度，注意，不需要是数组中实际成员个数
 * @param		card_ranks				[in]			扑克配置
 * @param		card_ranks_sum			[in]			扑克排列数目
 * @param		ppcards					[out]			扑克数组
 * @param		cardslen				[in-out]		扑克数组长度,输入估测长度，返回实际长度
 */
int CGameCardConfig::TransGroups(const CARD_GOURP group[MAX_CARD_GROUP_NUM], const int &nGroupLen,
	CARD_DESC *pGameCards, DWORD &dwCardSum)
{
	//校验
	if (NULL == pGameCards || 0 > nGroupLen)
	{
		return -1;
	}

	// 输出参数初始化
	DWORD curIndex = 0;

	// 遍历转换
	int result = 0;
	for (int i = 0; i < nGroupLen; ++i)
	{
		if (0 != group[i].sum) //检查当前组是否为空
		{
			result = TransGroup(&group[i], pGameCards, dwCardSum, curIndex); // 解析单组
			if (0 != result)
			{
				// 发生异常
				//if (NULL != m_plog)
				//TODOLATER
				//m_plog->LogSingleLine(L"填充牌组异常", NULL);
				result = -1;
				break;
			}
		}
	}

	// 牌数校验，如果牌数小于最大牌数，则按实际卡牌数算
	if ((curIndex + 1) < dwCardSum)
	{
		dwCardSum = curIndex;
	}

	return result;
}


/**
 * 生成一个乱序数组
 * 此函数会生成一个arlen 长度的乱序数组（0到src_len-1）并填充到array的0到src_len-1位置
 * @param			randarray			[in-out]		数组	
 * @param			src_len				[in]			数组长度	
 * @param			m_plog				[in]			日志	
 */
int CGameCardConfig::CreateOutOrderArray( DWORD randarray[], const int src_len )
{
	// 参数校验
	if( NULL == randarray )
	{

		return -1;
	}

	// 生成一个乱序数组
	memset( randarray, 0, src_len * sizeof( DWORD ));
	for( int i = 0; i < src_len ; i++ )
	{
		// 生成一个0到（src_len - i - 1 ）之间的随机数
		srand((unsigned)time(NULL));
		int randsum = rand()%( src_len - i );

		// 将 i 填到第 randsum 个空位（从 0 开始 算）
		int index = 0; // 空位个数
		for( int j = 0; j < src_len; j++)
		{
			if( 0 == randarray[j] )
			{
				// 判断是否是需要的位置
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
//不洗牌模式的洗牌算法       by  lih
int CGameCardConfig::CreateBuXiPaiArray(BYTE randarray[], const int src_len)
{
	cout << "func CreateBuXiPaiArray begin" << endl;

	// 参数校验
	if (NULL == randarray)
	{
		return -1;
	}

	BYTE count = 0;
	BYTE index[2] = { 0 };

	//随机两个下标交换15次   //嫌牌不好 改成7次
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

	//随机2~7个炸弹
	int boomNum = (rand() % 6) + 2;

	//随机取炸弹
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

	//洗牌
	int iSend = 0;
	BYTE flushcard[54] = { 0 };
	do
	{
		int station = rand() % (54 - iSend);
		flushcard[iSend] = tmpcard[station];
		iSend++;
		tmpcard[station] = tmpcard[54 - iSend];
	} while (iSend < 54);

	//去除无效卡牌
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
 * 乱序排列数组, 洗牌
 * 每局开始时, 将牌的顺序打乱
 * @param		dest_cards			[in-out]		目的数组， 乱序后的数组
 * @param		dest_len			[in]			目的数组长度
 * @param		src_cards			[in]			源牌数组， 由配置文件生成的游戏牌组
 * @param		src_len				[in]			源牌数量， 注意，此数据必须是牌组的实际长度
 */
int CGameCardConfig::OutOrder(		CARD_DESCS dest_cards, 
									const DWORD dest_len, 
									const CARD_DESCS source_cards, 
									const DWORD src_len, 
									const BYTE nMode,
									CARD_DESCS boom_cards,
									BYTE &boomsum)
{
	// 校验输入参数
	if( NULL == dest_cards	||
		NULL == source_cards ||
		dest_len < src_len  )
	{
		return -1;
	}

	int result = 0;

	if (GAME_SCORE_MODE_CLASSIC == nMode)
	{
		// 生成一个乱序数组
		DWORD *randarray = new DWORD[src_len](); // 乱序缓冲区

		if (0 == CreateOutOrderArray(randarray, src_len))
		{
			for (unsigned i = 0; i < src_len; ++i)
			{
				dest_cards[i] = source_cards[randarray[i]];
			}
		}
		else
		{
			// 乱序排列错误
			result = -1;
		}

		delete[] randarray;
	}
	else
	{
		// 生成一个不洗牌模式的乱序数组
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
 * 比较两张牌大小
 * @param			self			[in]		牌	
 * @param			rival			[in]		牌	
 * @param			flag			[in-out]	返回， 若self 大, 则返回TRUE, 否则返回FLASE	
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
 * 牌排序
 * 先按点数排，然后按花色排, 方式为从小到大
 * @param			cards				[in-out]		牌组	
 * @param			cardslen			[in]			牌组长度	
 */
int CGameCardConfig::OrderCards( CARD_DESCS cards, const DWORD cardslen )
{
	// 输入校验

	// 排序
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
 * 提取卡牌点数
 */
BYTE CGameCardConfig::GetCardScore( CARD_DESC card )
{
	return (card & 0x000F); //取0-4位
}

/**
* 提取卡牌花色
*/
BYTE CGameCardConfig::GetCardColor(CARD_DESC card)
{
	return (card & 0x00F0 ) >> 4; //取4-8位 << 4
}

/**
* 提取卡牌组
*/
BYTE CGameCardConfig::GetCardGroup( CARD_DESC card )
{
	return (card & 0x0F00) >> 8; //取8-12位
}