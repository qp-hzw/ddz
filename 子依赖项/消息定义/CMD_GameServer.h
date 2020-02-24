#ifndef CMD_GAME_SERVER_HEAD_FILE
#define CMD_GAME_SERVER_HEAD_FILE

#include "STR_CMD_GameServer.h"
#include "CMD_Club.h"

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

#define CMD_GC_COMMON_ERROR		    0									    //通用错误 
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
#define SUB_GR_REQUEST_FAILURE			4									//请求失败		大部分失败消息都返回这个消息


//玩家动作	
#define SUB_CG_USER_SITDOWN				10									//*坐下请求（用户加入/创建房间成功后，服务器主动让玩家坐下，不需要请求）
#define SUB_CG_USER_READY				11									//用户准备
#define SUB_CG_USER_STANDUP				12									//#起立请求		TODO 非房主解散房间，发送的是该消息，放在15的处理流程中
#define SUB_GR_USER_CHAIR_REQ			13                                  //*请求更换位置

#define SUB_CG_USER_INVITE_USER			14									//*邀请用户进入桌子
#define CMD_GC_USER_INVITE_USER			15									//*邀请用户进入桌子返回
#define SUB_CG_USER_KICK_USER			16                                   //*踢出用户		TODO 让玩家起立，返回系统消息，踢出用户

#pragma region 创建| 加入 房间
//房卡场, 房卡金币场
#define SUB_CG_USER_CREATE_ROOM			17									//申请创建房间
#define CMD_GC_USER_GET_ROOM_RULE		117									//房间规则选择, 申请创建房间成功后发送
#define SUB_CG_USER_SET_ROOM_RULE		217									//房间规则设置 返回
#define CMD_GC_USER_ENTER_SUBGAME_ROOM	317									//进入子游戏

#define SUB_CG_USER_JOIN_FK_ROOM		18									//加入桌子 需要密码

//金币场
#define SUB_CG_USER_JOIN_GOLD_HALL_ROOM	20									//加入金币大厅 金币场桌子
#pragma endregion

#define SUB_GR_GET_TABLELIST			22									//获取房间列表		TODO 指的是竞技场的已开房间列表
#define SUB_GR_GET_TABLELIST_RESULT		122									//房间列表返回		

//解散房间
#define SUB_RG_USER_ASK_DISMISS			24									//发起申请解散房间
#define CMD_GR_USER_ASK_DISMISS_RESULT	124									//发起申请解散房间 结果
#define	SUB_RG_USER_VOTE_DISMISS		224									//表决解散房间
#define CMD_GR_USER_VOTE_DISMISS		324									//广播房间申请解散
#define	CMD_GR_USER_DISMISS_RESULT		424									//广播表决解散房间结果

#define SUB_CG_USER_GOLD_INFO		    25									//请求金币大厅信息
#define CMD_GC_USER_GOLD_INFO		    125									//请求金币大厅信息 返回
#define CMD_GC_USER_GOLD_INFO_FINISH	225									//请求金币大厅信息 结束

#pragma endregion

#pragma region  MDM_GR_MATCH 比赛命令
/* *********************************************************************************
**						MAIN:7   MDM_GR_MATCH    比赛命令
** *********************************************************************************/
#define MDM_GR_MATCH				7									//比赛命令

#define SUB_CG_MATCH_INFO			1									//请求比赛场信息
#define CMD_GC_MATCH_INFO			101									//请求比赛场信息  返回

#define SUB_CG_MATCH_APPLY			2									//比赛场报名
#define CMD_GC_MATCH_APPLY			102									//比赛场报名	返回
#define SUB_CG_MATCH_UNAPPLY		3									//玩家取消报名
#define CMD_GC_MATCH_UNAPPLY		103									//玩家取消报名  返回

#define CMD_GC_MATCH_START			4									//比赛开始
#define CMD_GC_MATCH_CANCEL			5									//比赛取消

#define SUB_CG_MATCH_QUERY_PLAYER   6									//请求信息 人数时间
#define CMD_GC_MATCH_QUERY_PLAYER   106									//请求信息 人数时间  返回

#define SUB_CG_MATCH_RANKING		7									//更新排名  所有人
#define CMD_GC_MATCH_RANKING		107									//更新排名  所有人  返回

#define SUB_CG_MATCH_RANKING_MY		8									//更新排名  自己
#define CMD_GC_MATCH_RANKING_MY		108									//更新排名  自己  返回

#define CMD_GC_MATCH_RESULT_JINJI	9									//比赛阶段结果 玩家晋级
#define CMD_GC_MATCH_RESULT_TAOTAI	10									//比赛阶段结果 玩家淘汰

#define CMD_GC_MATCH_JUESAI_RECODE	11									//决赛
#define CMD_GC_MATCH_WAIT_COUNT		12									//等待桌数消息

#pragma endregion

#pragma region MDM_G_FRAME 框架命令
/* *********************************************************************************
**							MAIN:100   MDM_G_FRAME    框架命令
** *********************************************************************************/
#define MDM_G_FRAME					100									//框架命令

//房间信息
#define SUB_RG_ROOM_RULE					1								//请求房间规则
#define CMD_GR_ROOM_RULE					101								//房间规则
#define SUB_RG_PLAYER_INFO					201								//请求用户信息
#define CMD_GR_USER_STATUS					301								//用户动作结果
#define CMD_GR_USER_STATUS_FINISH			401								//请求用户信息完毕
#define SUB_RG_FRAME_GAME_OPTION			501								//请求游戏场景
#define CMD_GR_FRAME_GAME_OPTION			601								//请求用户场景返回

#define CMD_GR_FRAME_GAME_STATUS			102								//游戏状态
#define CMD_GR_FRAME_GAME_DISSMISS			701							    //解散面板状态

//GPS测距
#define SUB_CG_COM_CHECK_USER_GPS			5								//请求校验用户GPS位置信息
#define CMD_GC_COM_CHECK_USER_GPS			105								//校验用户GPS位置信息返回

//IP校验
#define SUB_CG_COM_CHECK_USER_IP			8								//请求校验用户IP信息
#define CMD_GC_COM_CHECK_USER_IP			108								//校验用户IP信息返回

//用户聊天
#define SUB_RG_FRAME_CHAT					7								//用户聊天
#define CMD_GR_FRAME_CHAT					107								//用户聊天


//道具消耗
#define SUB_CG_EFFECT						9								//道具消耗 -- 子游戏中扔鸡蛋, 鲜花, 炸弹
#define CMD_GC_EFFECT_RESULT				109								//操作失败
#define CMD_GC_EFFECT_BRODCAST				209								//操作广播

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

#endif