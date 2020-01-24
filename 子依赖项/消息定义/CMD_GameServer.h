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

//
#define CMD_GC_USER_MODIFY_TREASURE		1									//修改用户财富信息返回
#define CMD_ROOM_RULE					2									//房间信息
#define SUB_GR_USER_STATUS				3									//用户动作
#define SUB_GR_REQUEST_FAILURE			4									//请求失败		大部分失败消息都返回这个消息


//玩家动作	
#define SUB_CG_USER_SITDOWN				10									//*坐下请求（用户加入/创建房间成功后，服务器主动让玩家坐下，不需要请求）
#define SUB_CG_USER_READY				11									//用户准备
#define SUB_CG_USER_STANDUP				12									//#起立请求		TODO 非房主解散房间，发送的是该消息，放在15的处理流程中
#define SUB_GR_USER_CHAIR_REQ			13                                  //*请求更换位置

#define SUB_CG_USER_INVITE_USER			14									//*邀请用户进入桌子
#define CMD_GC_USER_INVITE_USER			15									//*邀请用户进入桌子返回
#define SUB_CG_USER_KICK_USER			16                                   //*踢出用户		TODO 让玩家起立，返回系统消息，踢出用户

//创建|加入 房间
#define SUB_CG_USER_CREATE_ROOM			17									//申请创建房间
#define CMD_GC_USER_GET_ROOM_RULE		117									//房间规则选择, 申请创建房间成功后发送
#define SUB_CG_USER_SET_ROOM_RULE		217									//房间规则设置 返回
#define CMD_GC_USER_ENTER_SUBGAME_ROOM	317									//进入子游戏

#define SUB_CG_USER_JOIN_FK_ROOM		18									//#加入桌子 需要密码
#define CMD_GC_USER_JOIN_ROOM_SUCCESS	118									//#加入成功		加入房卡/金币/牌友圈房间的返回

#define SUB_CG_USER_JOIN_TABLE_NO_PASS	19									//#加入桌子,不需要密码, 即快速开始
//返回的为 2, CMD_GC_USER_JOIN_ROOM_SUCCESS;								//不需要加入桌子返回

#define SUB_CG_USER_JOIN_GOLD_HALL_ROOM	20									//加入大厅金币场桌子
//如果没有空桌, 返回的是 2, CMD_GC_CLUB_CREATE_TABKE
//如果有空桌子, 返回的为 2, CMD_GC_USER_JOIN_ROOM_SUCCESS

#define SUB_CG_CLUB_CREATE_TABLE		21									//#创建桌子 牌友圈特有
#define CMD_GC_CLUB_CREATE_TABKE		121									//#创建桌子 返回


#define SUB_GR_GET_TABLELIST			22									//获取房间列表		TODO 指的是竞技场的已开房间列表
#define SUB_GR_GET_TABLELIST_RESULT		122									//房间列表返回		


//解散房间
#define SUB_RG_USER_ASK_DISMISS			24									//发起申请解散房间
#define CMD_GR_USER_ASK_DISMISS_RESULT	124									//发起申请解散房间 结果
#define	SUB_RG_USER_VOTE_DISMISS		25									//表决解散房间
#define CMD_GR_USER_VOTE_DISMISS		125									//广播房间申请解散
#define	CMD_GR_USER_DISMISS_RESULT		224									//广播表决解散房间结果

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