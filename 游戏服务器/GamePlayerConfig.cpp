#include "StdAfx.h"
#include "GamePlayerConfig.h"

#import "msxml6.dll"
using namespace MSXML2;
#include <time.h>


/**
 * 解析所有的玩家类型 ，输出 玩家类型数组
 * @param		configfile				[in]			配置文件
 * @param		pplayers				[out]			玩家组数组
 * @param		player_sum				[in-out]		玩家组数组长度
 * @param		m_plog					[in]			日志接口
 */
int CGamePlayerConfig::LoadPlayerDesc(CCfg& ff, PLAYER_TYPE &player_type)
{
	// 初始化输出参数
	int result = 0;

	long id = 0, _sum = 0, _card_sum = 0;
	ff.GetItemValue("GAME", "CardGroupNum", id);
	ff.GetItemValue("GAME", "CardGroupNum", _sum);
	ff.GetItemValue("GAME", "CardGroupNum", _card_sum);

	SetPlayerGroupID(player_type, id); // id
	SetPlayerCardSum(player_type, _sum); // card_sum
	SetPlayerSum(player_type, 0); // 解析player_sum

	return result;
}

/**
 * 根据玩家组描述数组构造玩家描述数组
 * @param		dst_player_types	[in-out]	目的玩家类型数组
 * @param		player_sum			[in]		玩家类型数组长度
 * @param		src_player_type		[in]		源玩家组数组长度
 */
int CGamePlayerConfig::CreatePlayerDesc( PLAYER_DESCS &dst_player_types, const DWORD player_sum, const PLAYER_TYPE &src_player_type)
{

	// 计算玩家数量,每种玩家类型数目*玩家类型数目
	int _player_sum = 0;

	// 初始化输出参数
	int result = 0;

	//玩家类型	
	for( DWORD j = 0; j < player_sum; ++j  )
	{
		//玩家数组赋值
		dst_player_types[j].type = src_player_type;
	}

	return result;
}


/**
* 设置玩家类型组ID
*/
void CGamePlayerConfig::SetPlayerGroupID( PLAYER_TYPE &_type, BYTE _id)
{
	_type &= 0x0FFF;
	_type += (_id * 0x1000);
}

/**
* 设置玩家数量
*/
void CGamePlayerConfig::SetPlayerSum( PLAYER_TYPE &_type, BYTE _sum)
{
	_type &= 0xF0FF;
	_type += (_sum * 0x100);
}

/**
* 设置玩家类型手牌数量
*/
void CGamePlayerConfig::SetPlayerCardSum( PLAYER_TYPE &_type, BYTE _card_sum )
{
	_type &= 0xFF00;
	_type += _card_sum;
}

/**
* 提取玩家类型组ID
*/
BYTE CGamePlayerConfig::GetPlayerGroupID( PLAYER_TYPE _type)
{
	return (_type >> 12); //前4位
}

/**
* 提取玩家类数量
*/
BYTE CGamePlayerConfig::GetPlayerSum( PLAYER_TYPE _type)
{
	return ( ( _type >> 8 ) & 0x0F );	//中4位
}

/**
* 提取玩家类型手牌数量
*/
BYTE CGamePlayerConfig::GetPlayerCardSum( PLAYER_TYPE _type )
{
	return ( _type & 0x00FF );			//后8位
}
