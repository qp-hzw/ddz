#ifndef CMD_CORRESPOND_HEAD_FILE
#define CMD_CORRESPOND_HEAD_FILE

#include "STR_CMD_Correspond.h"
#pragma pack(1)

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
#define MDM_REGISTER				1									//注册模块

//服务注册
#define CPR_LP_REGISTER_LOGON		1									//注册登录服
#define CPR_GP_REGISTER_GAME		2									//注册游戏服

//注册结果
#define CPO_PGPL_REGISTER_SUCESS	101									//注册完成
#define CPO_PGPL_REGISTER_FAILURE	102									//注册失败

#pragma endregion

#pragma region MDM_CPD_LIST 列表命令
#define MDM_CPD_LIST				2									//服务信息

#define CPR_GP_LIST_GAME_ONLINE		1									//游戏服 在线人数 
#define CPO_PL_LIST_GAME_ONLINE		101									//游戏服 在线人数

#define CPO_PL_LIST_INSERT_GAME		102									//插入游戏服GameID(单个) -- 结构体为tagGameServer
#define CPO_PL_LIST_INSERT_GAME_LIST 103								//插入游戏服GameID(列表) -- 结构体为tagGameServer
#define CPO_PL_LIST_REMOVE_GAME		104									//删除游戏服GameID


//TODONOW added by WangChengQing 2018/9/6 没有看懂是做什么的
#define SUB_CS_S_SERVER_INFO		105									//房间信息
#pragma endregion

#pragma region MDM_TRANSFER 中转服务
#define MDM_TRANSFER				3									//中转服务

#define CPR_GP_CLUB_TABLE_INFO		1									//广播桌子信息
#define CPO_PL_CLUB_TABLE_INFO		101									//广播桌子信息

#define CPR_GP_CLUB_PLAYER_INFO		2									//广播用户信息
#define CPO_PL_CLUB_PLAYER_INFO		102									//广播用户信息

#define CPR_LP_CLUB_TABLE_DISSOLVE	3									//解散桌子
#define CPO_PG_CLUB_TABLE_DISSOLVE	103									//解散桌子

#define CPR_LP_CREATE_TABLE			4									//创建桌子 查询可用的GameID
#define CPO_PL_CREATE_TABLE			104									//创建桌子 查询可用的GameID

#pragma endregion 

#pragma region MDM_CS_USER_COLLECT 用户汇总
//TODONOW 套嵌的有点深 added by WangChengQing 2018/9/6
#define MDM_CS_USER_COLLECT			4									//用户汇总

//用户状态
#define SUB_CS_C_USER_ENTER			1									//用户进入
#define SUB_CS_C_USER_LEAVE			2									//用户离开
#define SUB_CS_C_USER_FINISH		3									//用户完成
#define SUB_CS_C_USER_OFFLINE		4									//用户断线  game->correspond
// TODONOW 登录服没有 主消息号3的处理, 因此这个消息号的主消息号为2 之后修改
#define SUB_CS_C_USER_OFFLINE_B		5									//用户断线  correspond -> logon

//用户状态
#define SUB_CS_S_COLLECT_REQUEST	100									//汇总请求

//////////////////////////////////////////////////////////////////////////////////
//断线重连
struct tagOfflineUser 
{
	DWORD dwUserID;
	DWORD dwServerID;	
	BYTE  byMask;		//1表示在list中增加用户；  2表示在list中删除用户
};

//用户进入
struct CMD_CS_C_UserEnter
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//辅助信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级

	//SCORE							lDiamond;							//钻石
	//SCORE							lGold;								//金币
};

//用户离开
struct CMD_CS_C_UserLeave
{
	DWORD							dwUserID;							//用户标识
};

struct STR_SUB_CS_C_USER_OFFLINE
{
	DWORD dwUserID;
	DWORD dwGameID;
	BYTE  byMask;		//1表示在list中增加用户；  2表示在list中删除用户
};


#pragma endregion

#pragma region MDM_WEB 后台管理
#define MDM_WEB						5									//后台管理

#define CPR_WP_WEB_MARQUEE			1									//跑马灯消息 (WEB --> CPD)
#define CPO_PL_WEB_MARQUEE			101									//跑马灯消息（CPD --> Logon）

#define CPR_WP_WEB_SYSTEM_MESSAGE	2									//聊天频道 -- 系统消息（WEB --> CPD）
#define CPO_PL_WEB_SYSTEM_MESSAGE	102									//聊天频道 -- 系统消息 (CPR --> Logon)

#pragma endregion

#pragma pack()


#endif