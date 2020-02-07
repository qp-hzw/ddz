#ifndef STRUCT_HEAD_FILE
#define STRUCT_HEAD_FILE

#pragma pack(1)
#include "Define.h"

//////////////////////////////////////////////////////////////////////////////////
//用户信息


//用户道具
struct tagUserProperty
{
	DWORD       dwPropID;		  //item id
	DWORD       dwPropCount;      //item count;
};


//用户信息
struct tagUserInfo
{
	//基本属性
	DWORD							dwUserID;							//用户ID
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	BYTE							cbGender;							//用户性别
	TCHAR							szHeadUrl[LEN_HEAD_URL];			//头像地址
	TCHAR							szUnderWrite[LEN_MY_SIGNATURE];		//个性签名

	//社团信息
	DWORD							dwGroupID;							//社团 ID
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字

	//用户等级
	BYTE							cbMemberOrder;						//会员等级
	BYTE							dwLevel;                            //经验等级   
	DWORD							dwExperience;						//经验数值

	//用户财富
	SCORE							lOpenRoomCard;						//房卡数量
	SCORE							lGold;								//金币
	SCORE							lDiamond;							//钻石
	
	//游戏信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数

	/* 附加数据 游戏动态数据 */
	DWORD							wTableID;							//桌子索引
	WORD							wChairID;							//椅子索引
	BYTE							cbUserStatus;						//用户状态
	SCORE							lScore;								//积分
};

//////////////////////////////////////////////////////////////////////////////////

//游戏记录
struct tagGameScoreRecord
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	WORD							wChairID;							//椅子号码
	BYTE							cbAndroid;							//机器标志

	//成绩信息
	SCORE							lScore;								//用户分数
	SCORE							lGrade;								//用户成绩
	SCORE							lRevenue;							//游戏税收
	SCORE							lControlScore;						//用户控制值
	SCORE							lChoushui;							//抽水
	DWORD							dwChoushuiType;						//抽水类型

	//用户信息
	DWORD							dwDBQuestID;						//请求标识
	DWORD							dwInoutIndex;						//记录索引

	//附加信息
	DWORD							dwUserMemal;						//奖牌数目
	DWORD							dwPlayTimeCount;					//游戏时长
};

#endif