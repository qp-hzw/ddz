
#pragma once
#include <windows.h>
#include "cfg.h"
typedef BYTE *CARD_SCOR_RANK,	CARD_SCOR;	// 纸牌点数排序描述
typedef BYTE *CARD_COLOR_RANK,	CARD_COLOR;	// 纸牌花色排序描述
typedef BYTE *CARD_DESCS,		CARD_DESC;	// 纸牌描述

#define __GETSCORE( A )			CGameCardConfig::GetCardScore( A )		// 获取单张手牌点数
#define __GETGROUPID( A )		CGameCardConfig::GetCardGroup( A )		// 获取单张手牌所属组ID
#define __GETCOLOR( A )			CGameCardConfig::GetCardColor( A )		// 获取单张手牌花色

#define MAX_CARD_GROUP_NUM			20				//最大的卡牌牌组数目
#define MAX_TOTAL_CARD_NUM			108				//默认的卡牌总数

// 牌组描述
typedef struct _tagCARD_GOURP
{
	//BYTE			id;			// 组标识符
	//CARD_SCOR		begin;		// 开始牌
	//CARD_SCOR		end;		// 结尾牌
	//CARD_COLOR_RANK	colors;		// 花色组合 1方块; 2梅花; 3红桃; 4黑桃
	//BYTE			clrsum;		// 花色数量
	//BYTE			sum;		// 数量
	BYTE			id;			// 组标识符
	BYTE			color;		// 花色
	CARD_SCOR		begin;		// 开始牌
	CARD_SCOR		end;		// 结尾牌
	BYTE			sum;		// 同一张牌有几张
}CARD_GOURP, *CARD_GOURPS;

class CServerLog;

class CGameCardConfig
{
public:
	CGameCardConfig(void) {}
	~CGameCardConfig(void) {}


// 解析纸牌数据
public:
/**
 * 获取一副牌的估计数量
 * 使用 xml 配置文件 解析一副牌的数量
 * @param  configfile					[in]			配置文件
 * @param  cards_sum					[in-out]		扑克总数目
 * @param  m_plog						[in]			日志记录
 * return					0				正常
 *							-1				异常
 */
static int LoadCardsSum(DWORD &cards_sum);


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
static int LoadGameScoreTimes(BYTE game_score_Mode, BYTE *game_score_times);


/**
 * 填充组 
 * 解析XML 中 所有牌组节点信息 
 * @param  configfile				[in]			配置文件
 * @param  ppgroups					[out]			扑克配置
 * @param  group_sum				[in-out]		组实际数量
 * @m_plog					[in]			日志记录
 * return					0				正常
 *							-1				异常
 */
static int LoadCardGroups(CARD_GOURP *groups, DWORD &group_sum);

/**
 * 提取所有组
 * 此函数从扑克列表中找出所有满足 group 条件的纸牌并填充到 cards 数组中， 并返回执行完成后 cards 数组中的成员数
 * @param		groups					[in]			扑克组 数组
 * @param		groups_len				[in]			扑克组 数组最大长度，注意，不需要是数组中实际成员个数
 * @param		card_rank				[in]			扑克点数排列
 * @param		card_ranks_sum			[in]			扑克点数排列数目
 * @param		ppcards					[out]			扑克数组
 * @param		cardslen				[in-out]		扑克数组长度,输入估测长度，返回实际长度
 */
static int TransGroups(const CARD_GOURP group[MAX_CARD_GROUP_NUM], const int &nGroupLen,
										CARD_DESC *pGameCards, DWORD &dwCardSum);

// 工具类函数
public:

	/** 
	 * 乱序排列数组, 洗牌
	 * 每局开始时, 将牌的顺序打乱
	 * @param		dest_cards			[in-out]		目的数组， 乱序后的数组
	 * @param		dest_len			[in]			目的数组长度
	 * @param		src_cards			[in]			源牌数组， 由配置文件生成的游戏牌组
	 * @param		src_len				[in]			源牌数量， 注意，此数据必须是牌组的实际长度
	 */
	static int OutOrder(CARD_DESCS dest_cards,
									const DWORD dest_len,
									const CARD_DESCS source_cards,
									const DWORD src_len,
									const BYTE nMode,
									CARD_DESCS boom_cards,
									BYTE &boomsum);

// 
public:

	/**
	 * 将指定的组所标识的牌组转换为纸牌数组中的数据
	 * 此函数从扑克列表中找出所有满足 group 条件的纸牌并填充到 cards 数组中， 并返回执行完成后 cards 数组中的成员数
	 * @param		group					[in]			扑克组
	 * @param		card_ranks				[in]			扑克排列
	 * @param		card_ranks_sum			[in]			扑克排列数目
	 * @param		cards					[in-out]		扑克数组
	 * @param		cardslen				[in]			扑克数组长度
	 * @param		curlen					[in-out]		已经填充的长度， 注意，此参数会将现有值与填充进去的值相加
	 *														所以此参数的输出值与输入值有关
	 */
	static int TransGroup(const CARD_GOURP *group,
										CARD_DESCS cards,
										const DWORD maxlen,
										DWORD &curIndex);

	/**
	 * 生成一个乱序数组
	 * 此函数会生成一个arlen 长度的乱序数组（0到src_len-1）并填充到array的0到src_len-1位置
	 * @param			randarray			[in-out]		数组	
	 * @param			src_len				[in]			数组长度	
	 * @param			m_plog				[in]			日志	
	 */
	static int CreateOutOrderArray( DWORD randarray[], const int src_len );

	/**
	 * 比较两张牌大小
	 * @param			self			[in]		牌	
	 * @param			rival			[in]		牌	
	 * @param			flag			[in-out]	返回， 若self 大, 则返回TRUE, 否则返回FLASE	
	 */
	static int CompareCards( const CARD_DESC *self, const CARD_DESC *rival, BOOL *flag);

	////////////////////////////////////////////////////////
	//不洗牌模式的洗牌算法       by  lih
	static int CreateBuXiPaiArray(BYTE randarray[], const int src_len);
	/////////////////////////////////////////////////////////

	/**
	 * 牌排序
	 * 先按点数排，然后按花色排
	 * @param			cards				[in-out]		牌组	
	 * @param			cardslen			[in]			牌组长度	
	 * @param			card_ranks			[in]			点数排序	
	 * @param			card_ranks_sum		[in]			点数排序长度	
	 * @param			color_ranks			[in]			花色排序	
	 * @param			color_ranks_sum		[in]			花色排序长度	
	 */
	static int OrderCards(	CARD_DESCS cards, 
							const DWORD cardslen );

	/**
	 * 提取卡牌点数
	 */
	static BYTE GetCardScore( CARD_DESC card );

	/**
	 * 提取卡牌花色
	 */
	static BYTE GetCardColor(CARD_DESC card);

	/**
	 * 提取卡牌组
	 */
	static BYTE GetCardGroup( CARD_DESC card );

	//不洗牌模式的卡牌数局索引值
	static const BYTE	m_CardData[FULL_COUNT];			//原始扑克数据

	//两种牌型的牌值映射函数
	static BYTE   MapOfCardValue(BYTE CardData);

	static bool flushcard(BYTE card[], int &cardsum, BYTE boomcard[], int &boomsum);
};

