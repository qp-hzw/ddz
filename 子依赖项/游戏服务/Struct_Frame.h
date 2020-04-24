#ifndef STRUCT_HEAD_FILE
#define STRUCT_HEAD_FILE
#pragma pack(1)
#include "Define.h"

//通用房间规则
struct tagTableRule
{
	/********************************* 大厅使用 ************************************/
	BYTE	GameMode;				//游戏模式 0房卡约局; 1比赛模式; 2金币模式; 3金币约局; 4牌友圈
	BYTE    GameRoomLevel;          //房间等级 （金币场专用）

	BYTE	GameCount;				//游戏局数 0-无限局
	BYTE	PlayerCount;			//玩家数量 0-任意人数可开

	BYTE	cbPayType;				//支付方式，0房主支付、1AA制 -- 房卡模式才需要

	BYTE	bRefuseSameIP;			//允许同IP    0-不允许 1-允许
	BYTE	bDistanceIn300;			//允许300米	  0-不许云 1-允许
	BYTE	bAllowStranger;			//允许陌生人加入
};

//工会房间规则
struct tagClubRoomRule
{
	DWORD	dwClubID;				//工会
	DWORD	dwRoomID;				//房间
	DWORD   dwPasswd;               //密码

	BYTE	byGoldOrFK;				//(2.金币 1.房卡)

	BYTE	bDissolve;				//是否允许解散 0允许 1不允许

	//金币房特用
	DWORD	dwAmount;				//最低额度
	DWORD	dwOwnerPercentage;		//群主提成

	BYTE	byMask;					//1 AA支付;  2大赢家支付
	DWORD	dwDizhu;				//底注
};

//单个比赛场配置
struct MATCH_Rule
{
	//比赛场信息
	DWORD		llBaseScore;						//底分
};

//用户道具 -- 与client保持一致
struct tagUserProperty
{
	DWORD       dwPropID;		  //item id
	DWORD       dwPropCount;      //item count;
};

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

//金币场房间信息 返回
struct GOLD_RULE
{
	BYTE bGoldMod;// 房间等级 1.初级 2.中级 3.高级 4.富豪

	DWORD dwScore;  //底分
	DWORD dwGold;  //入场金币
};

#endif