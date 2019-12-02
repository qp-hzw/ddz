#include "StdAfx.h"
#include "GameCardConfig.h"
#include "ServerLog.h"

#import "msxml6.dll"
using namespace MSXML2;
#include <time.h>

//用于不洗牌模式的卡牌数组
const BYTE CGameCardConfig::m_CardData[FULL_COUNT] = {
	31,32,33,34,
	41,42,43,44,
	51,52,53,54,
	61,62,63,64,
	71,72,73,74,
	81,82,83,84,
	91,92,93,94,
	101,102,103,104,
	111,112,113,114,
	121,122,123,124,
	131,132,133,134,
	141,142,143,144,
	151,152,153,154,
	165,166
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
int CGameCardConfig::LoadCardsSum( const wchar_t *configfile, DWORD &cards_sum, CServerLog *m_plog )
{
	int result = 0;

	//升级默认两副牌
	cards_sum = 108;

	if( NULL == configfile )
	{
		return -1;
	}

	// 读取配置文件，在VS2012上可运行成功，VS2017 XML解析库已经升级
	MSXML2::IXMLDOMDocumentPtr pXMLDom;
	HRESULT hr= pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER);
	if (FAILED(hr)) 
	{
		if (NULL != m_plog)
			m_plog->LogSingleLine(L"加载XML解析器失败%d", 2);
		return -1;
	}

	try
	{
		pXMLDom->async = VARIANT_FALSE;
		pXMLDom->validateOnParse = VARIANT_FALSE;
		pXMLDom->resolveExternals = VARIANT_FALSE; // 初始化标识

		// 初始化输出参数
		cards_sum = MAX_TOTAL_CARD_NUM;			//默认值

		if(pXMLDom->load(configfile) == VARIANT_TRUE)
		{
			MSXML2::IXMLDOMNodeListPtr pnl = pXMLDom->selectNodes(L"//Game"); // 加载所有节点
			if(NULL != pnl)
			{
				for( int i = 0; i < pnl->length; ++i )
				{
					//获得一副牌的总数
					if( 0 == wcscmp( L"武汉四五趣游网络", pnl->item[i]->Getattributes()->getNamedItem(L"name")->text))
					{
						if( NULL != pnl->item[i]->selectSingleNode(L"CARDSUM"))
						{
							cards_sum = atoi( pnl->item[i]->selectSingleNode(L"CARDSUM")->text);
						}
					}
				}
			}
			else
			{
				if (NULL != m_plog)
					m_plog->LogSingleLine(L"Game节点不存在%d", 2);
				result = -1;
			}
		}
		else
		{
			if (NULL != m_plog)
				m_plog->LogSingleLine(L"配置文件加载失败%d", 2);
			result = -1;
		}
	}
	catch(_com_error errorObject)
	{
		if (NULL != m_plog)
			m_plog->LogSingleLine(L"异常代码：0x%08x", errorObject.Error() );
		result = -1;
	}

	if( result == -1) // 解析失败
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
int CGameCardConfig::LoadGameScoreTimes(const wchar_t *configfile, BYTE game_score_Mode, BYTE *game_score_times, CServerLog *m_plog)
{
	int result = 0;
	memset( game_score_times, 1, sizeof(BYTE)*MAX_GAME_SCORE_TYPES);

	if (NULL == configfile || NULL == game_score_times || NULL == m_plog)
	{
		return -1;
	}

	// 读取配置文件
	MSXML2::IXMLDOMDocumentPtr pXMLDom;
	HRESULT hr = pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER);
	if (FAILED(hr))
	{
		//m_plog->LogSingleLine(L"加载XML解析器失败%d", 2);
		return -1;
	}

	try
	{
		// 初始化标识
		pXMLDom->async = VARIANT_FALSE;
		pXMLDom->validateOnParse = VARIANT_FALSE;
		pXMLDom->resolveExternals = VARIANT_FALSE; 
		
		// 加载所有节点
		if (pXMLDom->load(configfile) == VARIANT_TRUE)
		{
			MSXML2::IXMLDOMNodeListPtr pnl = pXMLDom->selectNodes(L"//Game");
			if (NULL != pnl)
			{					
				for (int i = 0; i < pnl->length; ++i)
				{
					//获得得分加倍模式
					if ( 0 == wcscmp(L"武汉四五趣游网络", pnl->item[i]->Getattributes()->getNamedItem(L"name")->text) )
					{
						MSXML2::IXMLDOMNodeListPtr pNodes;

						// 选出经典模式
						pNodes = pnl->item[i]->selectNodes( L"CAME_SCORE_MODE_CLASSIC" );
						if (NULL != pNodes)
						{
							// 遍历所有的 CAME_SCORE_MODE_CLASSIC
							for (int j = 0; j < pNodes->length; j++)
							{
                                //added by WangChengQing 2017/12/8 使用通用性的数字代替文字
								for (int k = 0; k < MAX_GAME_SCORE_TYPES; k++)
								{
                                    _bstr_t value = 1; //默认为1
								    // 解析得分加倍
                                    wchar_t wchName[32];
                                    swprintf(wchName, 31, L"FF%d", k);
                                    if ( NULL != pNodes->item[j]->selectSingleNode(wchName))
								    {
										value = pNodes->item[j]->selectSingleNode(wchName)->text;
                                    }                                                                       
								    game_score_times[k] = atoi(value);
									
								}
							}
						}
						else
						{
                            //add 考虑之后xml配置文件的通用性， 此处不应该报错，而是给一个默认值1
							for (int i = 0; i < MAX_GAME_SCORE_TYPES; i++)
							{
								game_score_times[i] = 1;
							}
						}

						// 选出疯狂加倍模式
						pNodes = pnl->item[i]->selectNodes( L"CAME_SCORE_MODE_CRAZY" );
						if (NULL != pNodes)
						{
							// 遍历所有的 CAME_SCORE_MODE_CRAZY
							for (int j = 0; j < pNodes->length; j++)
							{
                                //added by WangChengQing 2017/12/8 使用通用性的数字代替文字
								for (int k = 0; k < MAX_GAME_SCORE_TYPES; k++)
								{
                                    _bstr_t value = 1; //默认为1
								    // 解析得分加倍
                                    wchar_t wchName[32];
                                    swprintf(wchName, 31, L"FF%d", k);
                                    if ( NULL != pNodes->item[j]->selectSingleNode(wchName))
								    {
										value = pNodes->item[j]->selectSingleNode(wchName)->text;
                                    }                                                                       
								    game_score_times[k+MAX_GAME_SCORE_TYPES] = atoi(value);
									
								}
							}
						}
						else
						{
                            //add 考虑之后xml配置文件的通用性， 此处不应该报错，而是给一个默认值1
							for (int i = 0; i < MAX_GAME_SCORE_TYPES; i++)
							{
								game_score_times[i] = 1;
							}
						}
					}
					else
					{
						//m_plog->LogSingleLine(L"name节点不存在%s", 2);
						result = -1;
					}
				}
			}
			else
			{
				//m_plog->LogSingleLine(L"Game节点不存在%d", 2);
				result = -1;
			}
		}
		else
		{
			//m_plog->LogSingleLine(L"配置文件加载失败%d", 2);
			result = -1;
		}
	}
	catch (_com_error errorObject)
	{
		//m_plog->LogSingleLine(L"异常代码：0x%08x", errorObject.Error());
		result = -1;
	}

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
int CGameCardConfig::LoadCardGroups(const wchar_t *configfile, CARD_GOURP *groups, DWORD &group_sum, CServerLog *m_plog)
{
	int result = 0;

	if( NULL == configfile || NULL == groups)
	{
		return -1;
	}

	// 读取配置文件
	MSXML2::IXMLDOMDocumentPtr pXMLDom;
	HRESULT hr= pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER);
	if (FAILED(hr)) 
	{
		if ( NULL != m_plog )
			m_plog->LogSingleLine(L"加载XML解析器失败%d", 3);
		
		return -1;
	}

	// 初始化 输出参数
	pXMLDom->async = VARIANT_FALSE;
	pXMLDom->validateOnParse = VARIANT_FALSE;
	pXMLDom->resolveExternals = VARIANT_FALSE; // 初始化标识

	if(pXMLDom->load(configfile) == VARIANT_TRUE)
	{
		MSXML2::IXMLDOMNodeListPtr pnl = pXMLDom->selectNodes(L"//Game"); // 加载所有Game节点
		if(NULL != pnl)
		{
			for( int i = 0; i < pnl->length; ++i )
			{
				if( 0 == wcscmp( pnl->item[i]->Getattributes()->getNamedItem(L"name")->text, L"武汉四五趣游网络") )
				{
					MSXML2::IXMLDOMNodeListPtr pNodes;

					// 解析牌组顺序
					pNodes = pnl->item[i]->selectNodes(L"CARD_GOURP");			// 选出所有的 CARD_GOURP
					if( NULL != pNodes)
					{
						//取最小的卡牌牌组数
						group_sum = (pNodes->length > MAX_CARD_GROUP_NUM) ? MAX_CARD_GROUP_NUM : pNodes->length;

						for( int j = 0; j < group_sum; j++)		// 遍历所有的 CARD_GOURP
						{
							if( NULL != pNodes->item[j]->selectSingleNode(L"color") )	// 解析color
							{
								groups[j].color = atoi( pNodes->item[j]->selectSingleNode(L"color")->text );
								if ( NULL != pNodes->item[j]->selectSingleNode(L"begin") )	// 解析begin
								{
									groups[j].begin = atoi( pNodes->item[j]->selectSingleNode(L"begin")->text ); 
									if ( 0 != groups[j].begin )
									{
										if ( NULL != pNodes->item[j]->selectSingleNode(L"end") )	// 解析end
										{
											groups[j].end = atoi( pNodes->item[j]->selectSingleNode(L"end")->text ); 
											if ( 0 != groups[j].end )
											{
												// 解析花色
												if( NULL != pNodes->item[j]->selectSingleNode(L"sum") )		//解析sum
												{
													groups[j].sum = atoi( pNodes->item[j]->selectSingleNode(L"sum")->text );
												}
												else
												{
													if ( NULL != m_plog )
														m_plog->LogSingleLine(L"牌组: %d sum节点值异常", j);
													result = -1;
												}
											}
										}
										else
										{
											if ( NULL != m_plog )
												m_plog->LogSingleLine(L"牌组: %d end节点异常", j);
											result = -1;
										}
									}
								}
								else
								{
									if ( NULL != m_plog )
										m_plog->LogSingleLine(L"牌组: %d begin节点异常", j);
									result = -1;
								}
							}
							else
							{
								// 组 ID 节点不存在
								if ( NULL != m_plog )
									m_plog->LogSingleLine(L"牌组: %d color节点不存在", j);
								result = -1;
							}

							//异常退出
							if ( -1 == result )
							{
								break;
							}
						}
					}
					else
					{
						if ( NULL != m_plog )
							m_plog->LogSingleLine(L"没找到CARD_GOURP标签%d", 1);
						result = -1;
					}
				}
			}
		}
		else
		{
			if ( NULL != m_plog )
				m_plog->LogSingleLine(L"节点获取失败%d", 1);
			result = -1;
		}
	}
	else
	{
		if ( NULL != m_plog )
			m_plog->LogSingleLine(L"配置文件加载失败%d", 1);
		result = -1;
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
int CGameCardConfig::TransGroup(	const CARD_GOURP *group, 
									CARD_DESCS cards, 
									const DWORD maxlen, 
									DWORD &curIndex,
									CServerLog *m_plog)
{
	// 输入参数校验
	if ( NULL == group ||
		 NULL == cards  ||
		maxlen < 1 ||
		curIndex < 0 ||
		 NULL == m_plog )
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
	for( unsigned i = group->begin; i <= group->end; ++i)	//卡牌点数
	{
		for (unsigned k = 0; k < group->sum; ++k)		//同一张有几张
		{
			// 花色
			if (curIndex < maxlen) // 检查card 缓冲区是否还有空位
			{
				cards[curIndex] = i; //低四位为牌值
				cards[curIndex] += group->color * (0xF+1);// 填充花色 5-8位表示花色
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
									CARD_DESC *pGameCards, DWORD &dwCardSum, CServerLog *m_plog)
{
	//校验
	if (NULL == pGameCards || 0 > nGroupLen )
	{
		return -1;
	}

	// 输出参数初始化
	DWORD curIndex = 0;

	// 遍历转换
	int result = 0;
	for( int i = 0; i < nGroupLen; ++i )
	{
		if( 0 != group[i].sum ) //检查当前组是否为空
		{
			result = TransGroup( &group[i], pGameCards, dwCardSum, curIndex, m_plog ); // 解析单组
			if( 0 != result)
			{
				// 发生异常
				if (NULL != m_plog)
					m_plog->LogSingleLine(L"填充牌组异常", NULL);
				result = -1;
				break;
			}
		}
	}

	// 牌数校验，如果牌数小于最大牌数，则按实际卡牌数算
	if ((curIndex+1) < dwCardSum)
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
									const BYTE nMode)
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
		CopyMemory(tmpCards, m_CardData, sizeof(BYTE)*src_len);

		srand((unsigned)time(NULL));

		if (0 == CreateBuXiPaiArray(tmpCards, src_len))
		{
			for (int i = 0; i < src_len; i++)
			{
				//牌值映射
				dest_cards[i] = MapOfCardValue(tmpCards[i]);
			}
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