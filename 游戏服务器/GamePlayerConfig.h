#pragma once
#include <windows.h>
#include "cfg.h"
typedef		WORD	PLAYER_TYPE;  // 玩家类型描述

#define __GETCARDSUM( _type )		CGamePlayerConfig::GetPlayerCardSum( _type ) // 提取玩家手牌数量

// 玩家描述
typedef struct _tagPLAYER_DESC
{
	PLAYER_TYPE			type;				// 玩家TYPE，数组，每个数代表某个用户类型的id、玩家数目、手牌数目
	BYTE				card_type;			// 手牌牌型ID
	int					times;				// 手牌倍数
	DWORD				bet;				// 玩家下注分数
	BYTE				bank_state;			// 玩家抢庄		0-过	1-叫地主	2-抢地主
	BYTE				rob_num;			// 玩家抢庄次数
	DWORD               rob_score;          // 玩家抢庄的分数， 例如"两张牌"中的筛子点数
	DWORD				play_state;			// 玩家状态
	BYTE				showcard_state;		// 摊牌状态			0 未完成，	1 完成
	BYTE				xj_ready_state;		// 小局玩家准备			0 未准备，	1 准备
	BYTE				bet_state;			// 玩家下注状态		0 未完成，	1 完成
	BYTE				mingpai_state;		// 玩家明牌状态		0 未完成，	1 完成
	BYTE				action_state;		// 玩家动作状态		0 未完成,   1 完成
	BYTE				action;				// 玩家动作			1-过 2-碰 3-吃 4-带
	SCORE				single_score;		// 单局得分
	SCORE				total_score;		// 玩家总得分
	BYTE				winsum;				// 赢局数
	DWORD				bank_count;			// 做庄的次数
	WORD				addscore_bet;		// 下注的倍数
	WORD				mingpai_bet;		// 明牌的倍数
	SCORE				best_bet;			// 玩家一大局中获得过的最大倍数
	BASE_PLAYERINFO		player_info;				// 记录玩家信息
	DWORD				start_mingpai;		//是否开始游戏明牌
	WORD				jiaofen_state;		//记录玩家叫的分
	WORD				spring_state;		//记录是否出过牌
	WORD				spring;				//记录是否春天
	WORD				player_tuo_guan;	//玩家是否托管   0-不托管
	WORD				out_time_num;		//玩家出牌超时次数

	BYTE				hand_card_num;						// 当前手牌数
	BYTE				out_card_num;						// 玩家该轮的出牌数目	
	BYTE				out_card_info[MAX_CARD_COUNT];		// 玩家该轮出的卡牌数据
	BYTE				level;				//玩家级数（从2开始）

	//斗地主
	BYTE				out_card_pass;		//出牌是否过	0-正常出牌	1-过
	tagBoomInfo			boom_info;			//炸弹信息

}PLAYER_DESC, *PLAYER_DESCS;

class CGamePlayerConfig
{
public:
	CGamePlayerConfig() {}
	~CGamePlayerConfig(void) {}


	// 解析玩家数据
public:

	/**
	 * 解析所有的玩家组 ，输出 玩家组数组
	 * @param		configfile				[in]			扑克组 数组
	 * @param		ppplayers				[out]			扑克组 数组
	 * @param		player_sum				[in-out]		扑克组 数组
	 * @param		m_plog					[in]			日志接口
	 */
	static int LoadPlayerDesc(CCfg& ff, PLAYER_TYPE &player_type);

	/**
	 * 根据玩家组描述数组构造玩家描述数组
	 * @param		dst_player_types	in]			目的玩家组数组
	 * @param		player_sum			[in-out]	玩家组数组长度
	 * @param		src_player_type		[in]		源玩家类型
	 * @param		m_plog				[in]		玩家组数组长度
	 */
	static int CreatePlayerDesc( PLAYER_DESCS &dst_player_types, const DWORD player_sum, const PLAYER_TYPE &src_player_type );

public:
	/**
	 * 设置玩家类型组ID
	 */
	static void SetPlayerGroupID( PLAYER_TYPE &_type, BYTE _id); 
		
	/**
	 * 设置玩家类数量 
	 *	TODONOW  added by WangChengQing
	 *  1. 数据从framework传入   2. 没有区分palytypeID, 所以导致配置文件中的玩家类型ID无用
	 */
	static void SetPlayerSum( PLAYER_TYPE &_type, BYTE _sum); 

	/**
	 * 设置玩家类型手牌数量
	 */
	static void SetPlayerCardSum( PLAYER_TYPE &_type, BYTE _card_sum );

	/**
	 * 获取玩家类型组ID
	 */
	static BYTE GetPlayerGroupID( PLAYER_TYPE _type); 

	/**
	 * 获取玩家类数量
	 */
	static BYTE GetPlayerSum( PLAYER_TYPE _type); 

	/**
	 * 获取玩家类型手牌数量
	 */
	static BYTE GetPlayerCardSum( PLAYER_TYPE _type );
};