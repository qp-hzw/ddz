#ifndef CMD_CORRESPOND_HEAD_FILE
#define CMD_CORRESPOND_HEAD_FILE

#include "STR_CMD_Correspond.h"

/*
** 简要描述:  协调服 与 其他进程(登录服, 游戏服, web后台) 消息
** 详细描述:  1. 消息号(主 + 子)     2. 结构体
** 备注说明:  子消息号的含义: 
**            1. 第一个字段: 归属标志,
**				 CPR(其他进程->协调服的消息)， CPO(协调服->其他进程的消息), 
**            2. 第二个字段: 消息流向         
**				 L: logon;  G:gameserver; W:web;  P:协调服
**            3. 第三个字段: 主消息号的标志   比如REGISTER即为注册模块的消息
**            4. 之后的字段: 子消息号的含义   比如GameServer表示游戏服注册
**           
**            命名规范
**            1. 结构体的命名:  在消息号的前面加STR, 比如SUB_CL_LOGON_ACCOUNTS 的为 STR_SUB_CL_LOGON_ACCOUNTS
**
**            2. 变量的命名规范: 1) 指针加p  2)归属标志CPR  3)主消息号标志  4)子消息号标志
**
**            3. 函数的命名:  On_归属标志_主消息号子消息号
**
**
*/

#pragma region MDM_REGISTER 注册模块
//注册命令
#define MDM_REGISTER_CPD			1									//注册模块

//服务注册
#define CPR_REGISTER_SERVER  	    1									//服务注册

//注册结果
#define CPO_REGISTER_SUCESS	        101									//注册完成
#define CPO_REGISTER_FAILURE	    102									//注册失败

#pragma endregion


#pragma region MDM_TRANSFER 中转服务
#define CPD_MDM_TRANSFER			2									//中转服务

#define CPR_GP_CLUB_TABLE_INFO		1									//广播桌子信息
#define CPO_PL_CLUB_TABLE_INFO		101									//广播桌子信息

#define CPR_GP_CLUB_PLAYER_INFO		2									//广播用户信息
#define CPO_PL_CLUB_PLAYER_INFO		102									//广播用户信息

#define CPR_LP_CLUB_TABLE_DISSOLVE	3									//解散桌子
#define CPO_PG_CLUB_TABLE_DISSOLVE	103									//解散桌子

#define CPR_LP_CREATE_TABLE			4									//创建桌子 查询可用的GameID
#define CPO_PL_CREATE_TABLE			104									//创建桌子 查询可用的GameID

#define CPR_LP_OFFLINE_PLAYERQUERY	5									//查询断线玩家 logon->correspond
#define CPR_PG_OFFLINE_PLAYERQUERY	105									//查询断线玩家 correspond ->game
#define CPO_GP_OFFLINE_FINISH		205									//查询断线玩家返回  game->correspond
#define CPO_PL_OFFLINE_FiNISH		305									//查询断线玩家返回  correspond->logon
#pragma endregion 

#endif