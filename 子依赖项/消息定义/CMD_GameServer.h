#ifndef CMD_GAME_SERVER_HEAD_FILE
#define CMD_GAME_SERVER_HEAD_FILE

#include "STR_CMD_GameServer.h"

/*
** 简要描述:  游戏服 与 其他进程的 消息
** 详细描述:  1. 消息号(主 + 子)     2. 结构体
** 备注说明:  子消息号的含义: 
**            1. 第一个字段: 归属标志,        SUB(接受到的消息)， CMD(发送出去的消息), DBR(向数据库Request)，DBO(数据库返回的消息)
**            2. 第二个字段: 消息流向         L: logon;  G:gameserver;  C:client; CP:correspond;  W:web  S:服务端子游戏 R:客户端子游戏
**            3. 第三个字段: 主消息号的标志   比如LOGON即为登录模块的消息
**            4. 之后的字段: 子消息号的含义   比如Accounts表示账号登录
**           
**            命名规范
**            1. 结构体的命名:  在消息号的前面加STR, 比如SUB_CG_LOGON_ACCOUNTS 的为 STR_SUB_CG_LOGON_ACCOUNTS
**
**            2. DBO, DBR消息号的命名为:  将SUB替换为DBR或DBO即可
**
**            3. 变量的命名规范: 1) 指针加p  2)归属标志SUB  3)主消息号标志  4)子消息号标志
**
**            4. 函数的命名:  On_归属标志_主消息号子消息号
**
**
*/

#pragma region MDM_LOGON 登录模块
/* *********************************************************************************
**						MAIN:1   MDM_LOGON    登录模块
** *********************************************************************************/
#define MDM_GR_LOGON				1										//登录信息

//登录
#define SUB_CG_LOGON_USERID			1										//ID 登录
#define CMD_GC_LOGON_USERID			101										//ID 登录返回

#pragma endregion

#pragma region MDM_USER 用户命令
/* *********************************************************************************
**							MAIN:2   MDM_USER    用户命令
** *********************************************************************************/
#define MDM_USER						2									//用户信息

//子消息  *表示客户端未用到		#初步整理		
#define SUB_CG_USER_RULE				1									//*用户规则		TODO 貌似是限制用户IP、胜率、逃跑率、积分，应该需要保留
//#define SUB_CG_USER_LOOKON				2									//*旁观请求		TODO 成功无返回，失败返回103请求失败
#define SUB_CG_USER_READY				9									//用户准备
#define SUB_CG_USER_SITDOWN				3									//*坐下请求（用户加入/创建房间成功后，服务器主动让玩家坐下，不需要请求）
#define SUB_CG_USER_STANDUP				4									//#起立请求		TODO 非房主解散房间，发送的是该消息，放在15的处理流程中
#define SUB_CG_USER_INVITE_USER			5									//*邀请用户进入桌子
#define CMD_GC_USER_INVITE_USER			6									//*邀请用户进入桌子返回
#define SUB_CG_USER_REFUSE_SIT  		7									//*拒绝玩家坐下	TODO 让玩家起立，返回系统消息，不让用户坐下
#define SUB_CG_USER_KICK_USER			8                                   //*踢出用户		TODO 让玩家起立，返回系统消息，踢出用户

#define SUB_GR_USER_CHAIR_REQ			10                                  //*请求更换位置
#define SUB_GR_USER_WAIT_DISTRIBUTE		12									//等待分配		TODO 再用户准备后，发送的空包，客户端如何处理了？

#pragma region 房间 && 桌子
#define SUB_CG_USER_CREATE_ROOM			13									//#创建房间
#define CMD_GC_USER_CREATE_ROOM_SUCCESS	121									//#创建成功		TODO 创建房卡房间和牌友圈房间的返回，其实创建房间的成功返回应该都用该消息

#define SUB_CG_CLUB_CREATE_TABLE		15									//#创建桌子 牌友圈特有
#define CMD_GC_CLUB_CREATE_TABKE		115									//#创建桌子 返回

#define SUB_CG_USER_JOIN_FK_ROOM		14									//#加入桌子 需要密码
#define CMD_GC_USER_JOIN_ROOM_SUCCESS	122									//#加入成功		加入房卡/金币/牌友圈房间的返回

#define SUB_CG_USER_JOIN_TABLE_NO_PASS	16									//#加入桌子,不需要密码, 即快速开始
//返回的为 2, CMD_GC_USER_JOIN_ROOM_SUCCESS;								不需要加入桌子返回

#define SUB_CG_USER_JOIN_GOLD_HALL_ROOM	17									//加入大厅金币场桌子
//如果没有空桌, 返回的是 2, CMD_GC_CLUB_CREATE_TABKE
//如果有空桌子, 返回的为 2, CMD_GC_USER_JOIN_ROOM_SUCCESS

#pragma endregion

#define SUB_CG_USER_JOIN_GROUP_ROOM		18									//(可删除)加入牌友圈房间 返回加入房间成功 （老大厅）
#define SUB_CG_USER_JOIN_GOLD_ROOM		19									//(可删除)加入金币房（没有空位置金币房，则先创建） TODO 为了大厅通用性，房间规则由客户端游戏决定

#define SUB_GR_GET_TABLELIST			21									//获取房间列表		TODO 指的是竞技场的已开房间列表
#define SUB_GR_GET_TABLELIST_RESULT		131									//房间列表返回		

#define SUB_GR_LOTTERY_NOTIFY			22									//抽奖通知 CMD TODO 发送的空包
#define SUB_GR_QUERY_LOTTERY			23									//请求抽奖 SUB
#define SUB_GR_LOTTERY_RESULT			132									//抽奖结果 CMD

#define SUB_CG_USER_RECONNECT_ROOM		24									//#断线重连
#define CMD_GC_USER_RECONNECT_ROOM		134									//#断线重连返回

#define SUB_CG_USER_CREATE_GROUP_ROOM	25									//(可删除)#创建牌友圈房间（只有圈主才能创建房间）	TODO 牌友圈放在单独的消息号中
#define CMD_GC_USER_CREATE_GROUP_ROOM	205									//(可删除)#创建牌友圈房间返回

#define SUB_GR_USER_SCORE				101									//用户分数		TODO 子游戏调用写分函数，大厅发送给客户端
#define SUB_GR_USER_STATUS				102									//用户动作
#define CMD_ROOM_RULE					202									//房间信息

#define SUB_GR_REQUEST_FAILURE			103									//请求失败		大部分失败消息都返回这个消息

#define CMD_GC_USER_MODIFY_TREASURE		106									//修改用户财富信息返回

#define SUB_GR_CREATE_OTHER_SUCCESS		129									//*替他人开房成功		TODO 删除，替他人开房成功放在开房成功中，不需要单独处理
#define SUB_GR_JOIN_DWTABLE_SUCCESS		133									//*加入电玩房成功		逻辑处理和其他加入房间不同，貌似是加入成功就开始游戏了
#pragma endregion

#pragma region  MDM_GR_MATCH 比赛命令
/* *********************************************************************************
**						MAIN:7   MDM_GR_MATCH    比赛命令
** *********************************************************************************/
#define MDM_GR_MATCH				7									//比赛命令

#define SUB_GR_MATCH_FEE			400									//报名费用
#define SUB_GR_MATCH_NUM			401									//等待人数
#define SUB_GR_LEAVE_MATCH			402									//退出比赛
#define SUB_GR_MATCH_INFO			403									//比赛信息
#define SUB_GR_MATCH_WAIT_TIP		404									//等待提示
#define SUB_GR_MATCH_RESULT			405									//比赛结果
#define SUB_GR_MATCH_STATUS			406									//比赛状态
#define SUB_GR_MATCH_USER_COUNT		407									//参赛人数
#define SUB_GR_MATCH_DESC			408									//比赛描述

#pragma endregion

#pragma region MDM_G_FRAME 框架命令
/* *********************************************************************************
**							MAIN:100   MDM_G_FRAME    框架命令
** *********************************************************************************/
#define MDM_G_FRAME					100									//框架命令

//房间信息
#define SUB_RG_FRAME_OPTION					1								//游戏配置
#define CMD_GR_FRAME_GAME_OPTION			101								//游戏场景
#define CMD_GR_FRAME_GAME_STATUS			102								//游戏状态
#define CMD_GR_FRAME_GAME_DISSMISS			301							    //解散面板状态

//解散房间
#define SUB_RG_FRAME_ASK_DISMISS			2								//发起申请解散房间
#define CMD_GR_FRMAE_VOTE_DISMISS			103								//广播房间申请解散
#define CMD_GR_FRMAE_ASK_DISMISS_RESULT     202								//发起申请解散房间 结果

#define	SUB_RG_FRAME_VOTE_DISMISS			3								//表决解散房间
#define	CMD_GR_FRAME_DISMISS_RESULT			104								//广播表决解散房间结果

//GPS测距
#define SUB_CG_COM_CHECK_USER_GPS			5								//请求校验用户GPS位置信息
#define CMD_GC_COM_CHECK_USER_GPS			105								//校验用户GPS位置信息返回

//IP校验
#define SUB_CG_COM_CHECK_USER_IP			8								//请求校验用户IP信息
#define CMD_GC_COM_CHECK_USER_IP			108								//校验用户IP信息返回

//用户聊天
#define SUB_RG_FRAME_CHAT					7								//用户聊天
#define CMD_GR_FRAME_CHAT					107								//用户聊天


//下面client没有
#define SUB_GF_LOOKON_CONFIG			803									//旁观配置

#define SUB_GF_XUFEI					805									//*续费消息

#define SUB_GF_USER_EXPRESSION			811									//*用户表情
#define SUB_GF_SOUND					812									//*发送语音

#define CMD_GR_FRAME_TREASURE_NO		105									//*用户财富不足以继续下局游戏时,发送给client 


//游戏信息
#define CMD_GF_LOOKON_STATUS			803									//*旁观状态

//系统消息
#define CMD_GF_SYSTEM_MESSAGE			806									//系统消息

#pragma endregion

#pragma region MDM_GF_GAME 子游戏命令
/* *********************************************************************************
**							MAIN:200   MDM_GF_GAME    子游戏命令
** *********************************************************************************/
#define MDM_GF_GAME					200									//游戏命令

#pragma endregion

#pragma pack()

#endif