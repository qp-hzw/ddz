#ifndef CMD_CLUB_HEAD_FILE
#define CMD_CLUB_HEAD_FILE

#include "STR_CMD_Club.h"

/*
** 工会系统
** 1. 登录服, 游戏服消息号都在这里
** 2. L->logon;  G->Game
*/

/* *******************************************************************************
**          MAIN:6      MDM_CLUB  工会
** ******************************************************************************/
//【主消息号】
#define MDM_CLUB								6							//工会

//【子消息号】
#define CMD_LC_CLUB_LIST_RE						50							//实时刷新工会列表
#define CMD_LC_CLUB_ROOM_RE						51							//实时刷新工会房间列表

#pragma region 工会 创建|加入|列表
#define SUB_CL_CLUB_CREATE_CLUB					1							//创建工会请求
#define CMD_LC_CLUB_CREATE_CLUB					101							//创建工会请求 返回

#define SUB_CL_CLUB_JOIN_CLUB					2							//申请加入工会
#define CMD_LC_CLUB_JOIN_CLUB					102							//申请加入工会返回
#define CMD_LC_CLUB_JOIN_CLUB_BDCAST			202							//申请加入工会广播

#define SUB_CL_CLUB_DISS_CLUB					3							//解散工会
#define CMD_LC_CLUB_DISS_CLUB					103							//解散工会返回

#define SUB_CL_CLUB_ALL_CLUB_INFO_LIST			4							//请求自身工会列表
#define CMD_LC_CLUB_ALL_CLUB_INFO_LIST			104							//返回自身工会列表
#define CMD_LC_CLUB_ALL_INFO_FINISH				204							//返回自身工会列表 结束

#define SUB_CL_CLUB_RANDOM_CLUB_LIST			5							//查询未满员, 非自身随机工会(最大9个)
#define CMD_LC_CLUB_RANDOM_CLUB_LIST			105							//查询未满员非自身随机工会 返回
#define CMD_LC_CLUB_RANDOM_CLUB_LIST_FINISH		205							//查询未满员非自身随机工会 结束
#pragma endregion

#pragma region 工会房间 创建|加入|修改|列表
#define SUB_CL_CLUB_ROOM_LIST					11							//请求该工会房间列表
#define CMD_LC_CLUB_ROOM_LIST					111							//请求该工会房间列表 返回
#define CMD_LC_CLUB_ROOM_LIST_FINISH			211							//请求该工会房间列表 结束

#define SUB_CL_CLUB_ROOM_SETTING				13							//房间设置请求 
#define CMD_LC_CLUB_ROOM_SETTING				113							//房间设置请求 返回

#define SUB_CG_CLUB_ROOM_CREATE					14							//创建房间
#define CMD_LC_CLUB_ROOM_CREATE					114							//创建房间 返回

#define SUB_CL_CLUB_ROOM_DISSOLVE				15							//解散房间请求 
#define CMD_LC_CLUB_ROOM_DISSOLVE				115							//解散房间请求 返回
#pragma endregion

#pragma region  工会桌子 创建|加入|列表
#define SUB_CG_CLUB_TABLE_LIST_TABLE			16							//请求该房间的桌子信息
#define CMD_GC_CLUB_TABLE_LIST_TABLE			116							//请求该房间的桌子信息 返回

#define SUB_CG_CLUB_CREATE_TABLE				17							//创建桌子 
#define CMD_GC_CLUB_CREATE_TABKE				117							//创建桌子 返回

#define SUB_CG_USER_JOIN_TABLE_NO_PASS			19							//#加入桌子,不需要密码, 即快速开始 -- 仅牌友群使用

#define SUB_CL_CLUB_TABLE_DISSOLVE				18							//解散桌子请求 
#define CMD_LC_CLUB_TABLE_DISSOLVE				118							//解散桌子请求 返回
#pragma endregion

#pragma region 工会信息面板
#define SUB_CL_CLUB_NOTICE						21							//牌友圈公告 -- 内部成员可看
#define CMD_LC_CLUB_NOTICE						121							//牌友圈公告 返回
#define CMD_LC_CLUB_NOTICE_BDCAST				221							//牌友圈公告 广播

#define SUB_CL_CLUB_MESSAGE						22							//牌友圈简介 -- 对外部展示的信息
#define CMD_LC_CLUB_MESSAGE						122							//牌友圈简介 返回  注:牌友圈简介 无须广播

#define SUB_CL_CLUB_CONTRIBUTE_FK				23							//贡献房卡 -- 对外部展示的信息
#define CMD_LC_CLUB_CONTRIBUTE_FK				123							//贡献房卡 返回

#define SUB_CL_CLUB_AUTO_AGREE					24							//牌友圈设置 -- 申请加入自动同意
#define CMD_LC_CLUB_AUTO_AGREE					124							//牌友圈设置 返回

#define SUB_CL_CLUB_CHAT						25							//聊天ALL 
#define CMD_LC_CLUB_CHAT						125							//聊天ALL 返回
#define CMD_LC_CLUB_CHAT_BDCAST					225							//聊天ALL 广播

#define SUB_CL_CLUB_MEMBER_MANAGER				31							//进入工会主面板
#define CMD_LC_CLUB_MEMBER_MANAGER				131							//进入工会主面板 返回
#define CMD_LC_CLUB_MEMBER_MANAGER_FINISH		231							//进入工会主面板 结束
#define CMD_LC_CLUB_DATA						331							//工会基本信息

#define SUB_CL_CLUB_INVITE						32							//邀请他入加入牌友圈 
#define CMD_LC_CLUB_INVITE						132							//邀请他人加入牌友圈 返回
#define CMD_LC_CLUB_INVITE_REMIND				232							//被邀请人的提醒

#define SUB_CL_CLUB_INVITE_RESULT				33							//被邀请人的回复
#define CMD_LC_CLUB_INVITE_RESULT				133							//被邀请人的回复 返回

#define SUB_CL_CLUB_APPLICANT_LIST				34							//牌友圈中申请人列表
#define CMD_LC_CLUB_APPLICANT_LIST				134							//牌友圈中申请人列表 返回
#define CMD_LC_CLUB_APPLICANT_LIST_FINISH		234							//牌友圈中申请人列表 结束

#define SUB_CL_CLUB_APPLICANT_RESULT			35							//群主|管理对申请消息的答复(同意|拒绝)
#define CMD_LC_CLUB_APPLICANT_RESULT			135							//群主|管理对申请消息的答复(同意|拒绝)

#define SUB_CL_CLUB_QUIT						36							//用户退出牌友圈
#define CMD_LC_CLUB_QUIT						136							//用户给退出牌友圈 返回

#define SUB_CL_CLUB_APPOINTMENT					37							//职务任免 0提出 1将为一般成员 2设置为管理员 
#define CMD_LC_CLUB_APPOINTMENT					137							//职务任免 返回
#define CMD_LC_CLUB_APPOINTMENT_NOTE			237							//职务任免 提醒

#define SUB_CL_CLUB_INQUERY_LIST				38							//被邀请人查看自己的邀请列表
#define CMD_LC_CLUB_INQUERY_LIST				138							//被邀请人查看自己的邀请列表 返回
#define CMD_LC_CLUB_INQUERY_LIST_FINISH			238							//被邀请人查看自己的邀请列表 结束
#pragma endregion

#pragma region 工会战绩
#define SUB_CL_CLUB_RECORD_LIST					41							//工会战绩统计 查询
#define CMD_LC_CLUB_RECORD_LIST					141							//工会战绩统计 返回
#define CMD_LC_CLUB_RECORD_FINISH				241							//工会战绩统计 结束
#pragma endregion

#endif