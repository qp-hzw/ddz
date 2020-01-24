#ifndef STRUCT_HEAD_FILE
#define STRUCT_HEAD_FILE

#pragma pack(1)
#include "Define.h"

//////////////////////////////////////////////////////////////////////////////////
//用户信息


//用户道具
struct tagUserProperty
{
	//道具使用
	struct tagUsePropertyInfo
	{
		WORD                            wPropertyCount;                     //道具数目
		WORD                            dwValidNum;						    //有效数字
		DWORD                           dwEffectTime;                       //生效时间
	};

	WORD                            wPropertyUseMark;                   //道具标示
	tagUsePropertyInfo              PropertyInfo[MAX_PT_MARK];			//使用信息   
};


//用户信息
struct tagUserInfo
{
	//基本属性
	DWORD							dwUserID;							//用户 I D
	DWORD							wTableID;						//桌子索引
	WORD							wChairID;							//椅子索引
	BYTE							cbUserStatus;						//用户状态

	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	DWORD							dwGroupID;							//社团 I D
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字
	TCHAR							szUnderWrite[LEN_MY_SIGNATURE];		//个性签名

	//头像信息
	DWORD							dwCustomID;							//自定标识
	TCHAR							szHeadUrl[LEN_HEAD_URL];			//头像地址

	//用户资料
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级

	

	//积分信息
	SCORE							lScore;								//用户分数
	SCORE							lGrade;								//用户成绩
	SCORE							lControlScore;						//用户控制值
	SCORE							lOpenRoomCard;						//房卡数量
	SCORE							lDiamond;							//钻石
	SCORE							lGold;								//金币

	//游戏信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//用户经验
	LONG							lLoveLiness;						//用户魅力

	//GPS added by lizhihu TODO 暂时放在这里，后面统一
	double							dLongitude;							//经度
	double							dLatitude;							//纬度

	//登录信息
	DWORD							dwLogonTime;						//登录时间
	DWORD							dwInoutIndex;						//进出标识

	//连接信息
	WORD							wBindIndex;							//绑定索引
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识

	//附加变量
	DWORD							dwUserRight;						//用户权限
	DWORD							dwMasterRight;						//管理权限
	SCORE							lRestrictScore;						//限制积分

	//辅助变量
	bool							bAndroidUser;						//机器用户
	TCHAR							szPassword[LEN_MD5];				//桌子密码
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