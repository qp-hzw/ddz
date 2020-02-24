#ifndef STR_CMD_CLUB_HEAD_FILE
#define STR_CMD_CLUB_HEAD_FILE

#pragma pack(1)

//MFC 文件
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxDisp.h>
#include <AfxDtctl.h>
#include "../全局定义/Define.h"

//实时刷新工会列表
struct STR_CMD_LC_CLUB_LIST_RE
{
	BYTE	byMask;					//无实际意义
};

//实时刷新工会房间列表
struct STR_CMD_LC_CLUB_ROOM_RE
{
	DWORD	dwClubID;				//俱乐部ID
};

#pragma region 工会 创建|加入|列表
//创建牌友圈
struct STR_SUB_CL_CLUB_CREATE_CLUB
{
	TCHAR	szClubName[LEN_NICKNAME];					//牌友圈名字
	TCHAR	szMessag[128];								//牌友圈简介
};

//创建牌友圈 返回
struct STR_CMD_LC_CLUB_CREATE_CLUB
{
	LONG	lResultCode;								//0成功  其他失败
};

//申请加入牌友圈
struct STR_SUB_CL_CLUB_JOIN_CLUB
{
	DWORD	dwClubID;									//牌友圈ID
};

//申请加入牌友圈 返回
struct STR_CMD_LC_CLUB_JOIN_CLUB
{
	LONG	lResultCode;								//0-成功  其他失败
};
//申请加入牌友圈 广播
struct STR_SUB_CL_CLUB_JOIN_CLUB_BDCAST
{
	DWORD	dwClubID;									//牌友圈ID
	DWORD	dwApplicantNum;								//申请人数
};

//解散牌友圈
struct STR_SUB_CL_CLUB_DISS_CLUB
{
	DWORD	dwClubID;									//牌友圈ID
};

//解散牌友圈 返回
struct STR_CMD_LC_CLUB_DISS_CLUB
{
	LONG	lResultCode;								//0-成功  其他失败
	TCHAR	szDescribe[128];							//失败原因
};

//请求自身牌友圈列表 返回
struct STR_CMD_LC_CLUB_ALL_CLUB_INFO_LIST
{
	DWORD	dwClubID;									//牌友圈ID
	TCHAR	szClubName[LEN_NICKNAME];					//牌友圈名字
	BYTE	byClubRole;									//玩家职务 0群主 1管理 2一般成员
};

//请求未满员非自身随机牌友圈(最大9个) 返回
struct STR_CMD_LC_CLUB_RANDOM_CLUB_LIST
{
	DWORD	dwClubID;									//牌友圈ID
	TCHAR	szClubName[LEN_NICKNAME];					//牌友圈名字
	BYTE	byClubLevel;								//牌友圈等级
	WORD	wMemberNum;									//当前人数
	TCHAR	szNotice[128];								//牌友圈 公告
	TCHAR	szMessage[128];								//牌友圈 简介
	DWORD	dwClubOwner;								//群主ID
	TCHAR	szClubOwnerName[LEN_NICKNAME];				//群主名字
};

//请求未满员非自身随机牌友圈(最大9个) 结束
struct STR_CMD_LC_CLUB_RANDOM_CLUB_LIST_FINISH
{
	BYTE	byMask;										//结束标志 1
};
#pragma endregion

#pragma region 工会房间 创建|加入|修改|列表
//查询特定牌友圈房间列表
struct STR_SUB_CL_CLUB_ROOM_LIST
{
	DWORD	dwClubID;									//牌友圈ID
};

//查询特定牌友圈房间列表 返回
struct STR_CMD_LC_CLUB_ROOM_LIST
{
	DWORD	dwRoomID;									//牌友圈房间ID
	TCHAR	szRoomName[16];								//club房间名字
	DWORD	dwKindID;									//游戏索引 -- 游戏服地址
	BYTE	byGoldOrFK;									//房间类型 1房卡场  2金币场
	TCHAR	szKindName[LEN_KIND];						//游戏名字
	WORD	wPlayerNum;									//该房间内当前玩家数
	TCHAR	szRoomRule[128];							//房间规则说明
	BYTE	byAllRound;									//游戏总局数
	BYTE	byChairNum;									//椅子数目

	//金币房 && 房卡房通用
	BYTE	bDissolve;									//是否允许解散  0允许  1不允许

	//金币房特有
	DWORD	dwAmount;									//最低额度
	DWORD	dwOwnerPercentage;							//群主提成

	//底注 && 最低身价
	BYTE	byMask;										//1 AA支付;  2大赢家支付
	DWORD	dwDizhu;									//底注
	DWORD	dwGold;										//最低身价, 低于此值, 不抽税收
};

//房间设置请求
struct STR_SUB_CL_CLUB_ROOM_SETTING
{
	DWORD	dwRoomID;									//房间ID
	TCHAR	szRoomName[16];								//房间名字
	DWORD   dwClubID;									//俱乐部ID
	DWORD   dwKindID;                                   //kindID

	BYTE	byGoldOrFK;									//1房卡房 2金币房
	//金币房 && 房卡房 通用配置
	bool	bDissolve;									//是否允许解散

	//金币房特用
	BYTE	byMask;										//1 AA支付;  2大赢家支付

	DWORD	dwAmount;									//最低额度(大赢家收益高于这个 才开始抽水)
	DWORD	dwOwnerPercentage;							//群主提成

	//底注 && 最低身价
	DWORD	dwDizhu;									//底注
	DWORD	dwGold;										//最低身价
};

//房间设置请求 返回
struct STR_CMD_LC_CLUB_ROOM_SETTING
{
	LONG	lResultCode;								//0-成功  其他失败
};

//解散房间请求
struct STR_SUB_CL_CLUB_ROOM_DISSOLVE
{
	DWORD	dwClubRoomID;								//房间ID
};

//解散房间请求 返回
struct STR_CMD_LC_CLUB_ROOM_DISSOLVE
{
	LONG	lResultCode;								//0-成功  其他失败
	TCHAR	szDescribe[128];							//失败原因
};

//申请加入房间
struct STR_SUB_CL_CLUB_JOIN_ROOM
{
	DWORD	dwRoomID;									//房间ID
};

//申请加入房间 返回
struct STR_CMD_LC_CLUB_JOIN_ROOM
{
	LONG	lResultCode;								//0-成功  其他失败
};

//桌子上的 userList
struct STR_CMD_LC_CLUB_TABLE_USER_LIST
{
	DWORD	dwUserID;									//玩家ID
	TCHAR	szUserName[LEN_NICKNAME];					//玩家名字
	BYTE	bySex;										//性别
	TCHAR	szHeadUrl[LEN_HEAD_URL];					//头像地址
	WORD	wLevel;										//等级
	
	BYTE	byClubRole;									//玩家职务 0 群主; 1管理; 2一般成员
	BYTE	byClubReputation;							//玩家在工会中的声望等级

	BYTE	byChairID;									//椅子ID
};

//该房间的tableList
struct STR_CMD_LC_CLUB_TABLE_LIST
{
	BYTE	byDel;										//mask 0无操作;  1删除

	DWORD	dwClubID;									//工会ID
	DWORD	dwRoomID;									//工会房间ID
	DWORD	dwTableID;									//桌子ID

	DWORD	TableState;									//0桌子空闲 1桌子已经开局
	DWORD	LockState;									//0桌子需要密码  1桌子不需要密码
	DWORD	CurrentRound;								//当前局数
	DWORD	AllRound;									//总局数
	DWORD	ChairCount;									//椅子数目

	STR_CMD_LC_CLUB_TABLE_USER_LIST player_info[6];     //玩家信息
};

//创建房间
struct STR_SUB_CL_CLUB_ROOM_CREATE
{
	//房间设置
	STR_SUB_CL_CLUB_ROOM_SETTING setting;

    //房间规则
	BYTE							byChoose[20];			//规则选择
};

//创建房间 返回
struct STR_CMD_LC_CLUB_ROOM_CREATE
{
	LONG	lResultCode;								//0-成功  其他失败
};
#pragma endregion

#pragma region 工会桌子 创建|加入|列表//解散桌子请求
//请求该房间的桌子信息
struct STR_SUB_CG_CLUB_TABLE_LIST_TABLE
{
	DWORD	dwClubID;								//club id 
	DWORD	dwRoomID;								//room id
};

//创建桌子
struct STR_SUB_CG_CLUB_CREATE_TABLE
{
	DWORD	dwClubID;								//club id 
	DWORD	dwRoomID;								//room id
	BYTE	byType;									//1.普通桌子 2.私密桌子
	DWORD	dwPasswd;								//私密桌子 密码
};

//解散桌子请求
struct STR_SUB_CL_CLUB_TABLE_DISSOLVE
{
	DWORD	dwClubRoomID;								//room id 
	DWORD	dwTableID;									//table id
};
//解散桌子请求 返回
struct STR_CMD_LC_CLUB_TABLE_DISSOLVE
{
	LONG	lResultCode;								//错误码
	TCHAR	szDescribe[128];							//描述信息
};

#pragma endregion

#pragma region 工会信息面板
//牌友圈公告 -- 内部成员可看
struct STR_SUB_CL_CLUB_NOTICE
{
	DWORD	dwClubID;									//牌友圈ID
	TCHAR	szNotice[128];								//牌友圈 公告
};

//牌友圈公告 返回
struct STR_CMD_LC_CLUB_NOTICE
{
	LONG	lResultCode;								//0成功  其他失败
	TCHAR	szDescribe[128];							//失败原因
};


//牌友圈简介
struct STR_SUB_CL_CLUB_MESSAGE
{
	DWORD	dwClubID;									//工会ID
	TCHAR	szMessage[128];								//牌友圈 简介
};

//牌友圈简介 返回
struct STR_CMD_LC_CLUB_MESSAGE
{
	LONG	lResultCode;								//0成功  其他失败
	TCHAR	szDescribe[128];							//失败原因
};

//贡献房卡
struct STR_SUB_CL_CLUB_CONTRIBUTE_FK
{
	DWORD	dwClubID;									//俱乐部ID
	DWORD	dwFK;										//贡献的房卡数量
};

//贡献房卡 返回
struct STR_CMD_LC_CLUB_CONTRIBUTE_FK
{
	LONG	lResultCode;								//0成功  其他失败
	TCHAR	szDescribe[128];							//失败原因

	DWORD	dwClubFK;									//当前牌友圈中房卡数量
};

//牌友圈设置
struct STR_SUB_CL_CLUB_AUTO_AGREE
{
	DWORD	dwClubID;									//俱乐部ID
	BYTE	byAutoAgree;									//是否开启自动同意 0同意
	//下面为过滤条件 比如10级以上的女性才允许加入等等 -- 后面会用
	BYTE	bySex;										//0不区分; 允许1男; 2女
	WORD	wLevel;										//人物等级
};

//牌友圈设置 返回
struct STR_CMD_LC_CLUB_AUTO_AGREE
{
	LONG	lResultCode;								//0成功  其他失败
	TCHAR	szDescribe[128];							//失败原因
};

//请求成员数据
struct STR_SUB_CL_CLUB_MEMBER_MANAGER
{
	DWORD	dwClubID;									//牌友圈ID
};

//请求成员数据 返回
struct STR_CMD_LC_CLUB_MEMBER_MANAGER
{
	DWORD	dwUserID;									//玩家ID
	TCHAR	szUserName[LEN_NICKNAME];					//玩家名字
	BYTE	byClubRole;									//玩家职务 0 群主; 1管理; 2一般成员
	BYTE	byUserLevel;								//玩家等级
	BYTE	byClubReputation;							//玩家在工会中的声望等级

	BYTE	byUserState;								//玩家状态  0在线 1离线 2游戏中
	TCHAR	szHeadUrl[LEN_HEAD_URL];							//头像地址
};

 //同时返回的公会基本信息
struct STR_CMD_LC_CLUB_DATA
{
	TCHAR	szClubName[LEN_NICKNAME];					//群名称
	TCHAR	szClubOwnerName[LEN_NICKNAME];				//群主名称
	DWORD	dwClubID;									//群ID
	DWORD	dwClubOwner;								//群主ID
	byte	byClubLevel;								//牌友圈等级
	WORD	wMemberNum;									//当前人数
	TCHAR	szNotice[128];								//牌友圈公告
	TCHAR	szMessage[128];								//牌友圈简介
	DWORD	dwFK;										//牌友圈中房卡数量
	BYTE	byApplyNum;									//新的入群申请数
	BYTE	byAutoAgree;								//是否开启自动同意
	BYTE	bySex;										//0不区分; 允许1男; 2女
	WORD	wLevel;										//加入等级
};

//邀请他人加入牌友圈
struct STR_SUB_CL_CLUB_INVITE
{
	DWORD	dwClubID;									//牌友圈ID
	DWORD	dwTagID;									//目标玩家ID
};

//邀请他人加入牌友圈 返回
struct STR_CMD_LC_CLUB_INVITE
{
	LONG	lResultCode;								//0-成功  其他失败
	TCHAR	szDescribe[128];							//失败原因
};

//被邀请人的提醒
struct STR_CMD_LC_CLUB_INVITE_REMIND
{
	DWORD	dwTagID;									//目标ID
};

//被邀请人查看自己的邀请列表 返回
struct STR_CMD_LC_CLUB_INQUERY_LIST
{
	DWORD	dwClubID;									//牌友圈ID
	TCHAR	szClubName[LEN_NICKNAME];					//牌友群名称
	BYTE	byClubLevel;								//牌友圈等级
	WORD	wMemberNum;									//当前人数
	TCHAR	szNotice[128];								//牌友圈 公告
	TCHAR	szMessage[128];								//牌友圈 简介
	DWORD	dwClubOwner;								//群主ID
	TCHAR	szClubOwnerName[LEN_NICKNAME];				//群主昵称
};

//被邀请人查看自己的邀请列表 结束
struct STR_CMD_LC_CLUB_INQUERY_LIST_FINISH
{
	BYTE	byMask;										//结束标志 1
};

//被邀请人的回复
struct STR_SUB_CL_CLUB_INVITE_RESULT
{
	DWORD	dwClubID;									//牌友圈ID
	byte	byAgree;									//0同意; 1不同意
};

//被邀请人的回复 返回
struct STR_CMD_LC_CLUB_INVITE_RESULT
{
	LONG	lResultCode;								//0-成功  其他失败
	TCHAR	szDescribe[128];							//失败原因
};

//申请人列表
struct STR_SUB_CL_CLUB_APPLICANT_LIST
{
	DWORD	dwClubID;									//牌友圈ID
};

// 申请人列表 返回
struct STR_CMD_LC_CLUB_APPLICANT_LIST
{
	DWORD	dwUserID;									//申请玩家ID
	TCHAR	szUserName[LEN_NICKNAME];					//申请玩家昵称
	BYTE	byUserSex;									//玩家性别
	BYTE	byUserLevel;									//玩家等级
	TCHAR	szNote[128];								//加入说明

	TCHAR	szHeadUrl[LEN_HEAD_URL];							//头像地址
};

//申请人列表 结束
struct STR_CMD_LC_CLUB_APPLICANT_LIST_FINISH
{
	BYTE	byMask;										//结束标志 1
};

//群主|管理 
struct STR_SUB_CL_CLUB_APPLICANT_RESULT
{
	DWORD	dwUserID;									//玩家ID
	DWORD	dwCludID;									//俱乐部ID
	DWORD	dwResult;									//0同意 其他拒绝
	DWORD	dwTagID;									//申请人ID
};

//群主|管理  返回
struct STR_CMD_LC_CLUB_APPLICANT_RESULT
{
	LONG	lResultCode;								//0-成功  其他失败
	TCHAR	szDescribe[128];							//失败原因

	DWORD dwTargetID;									//目标玩家ID
};

//退出牌友圈 
struct STR_SUB_CL_CLUB_QUIT
{
	DWORD	dwClubID;									//牌友圈ID
};

//退出牌友圈 返回
struct STR_CMD_LC_CLUB_QUIT
{
	LONG	lResultCode;								//0-购买成功  其他失败
};

//职务任免 请求
struct STR_SUB_CL_CLUB_APPOINTMENT
{
	DWORD	dwClubID;									//牌友圈ID
	DWORD	dwTagID;									//目标玩家ID
	BYTE	byAction;									//0为踢出用户; 1降为平民; 2升为管理员
};

//职务任免 返回
struct STR_CMD_LC_CLUB_APPOINTMENT
{
	LONG	lResultCode;								//错误码
	TCHAR	szDescribe[128];							//描述信息
};

//职务任免 提醒
struct STR_CMD_LC_CLUB_APPOINTMENT_NOTE
{
	DWORD	dwUserID;									//目标玩家ID
	BYTE	byAction;									//0踢出  1设为普通成员 2管理员
	TCHAR	szDescribe[128];							//描述信息
};

//牌友圈聊天
struct STR_SUB_CL_CLUB_CHAT
{
	DWORD	dwUserID;									//玩家ID
	DWORD	dwTagUserID;								//私密时候发送的玩家的ID
	BYTE	byChatMode;									//0.俱乐部聊天  1.世界聊天   2.系统聊天    3.私聊
	DWORD	dwClubID;									//俱乐部ID
	TCHAR	szChat[128];								//聊天文字
};
//牌友圈聊天 返回
struct STR_CMD_LC_CLUB_CHAT
{
	BYTE	byChatMode;									//0.俱乐部聊天  1.世界聊天   2.系统聊天    3.私聊
	LONG	lResultCode;								//0-购买成功  其他失败
	TCHAR	szDescribe[128];							//失败原因
};
//牌友圈聊天 广播
struct STR_CMD_LC_CLUB_CHAT_BDCAST
{
	BYTE	byChatMode;									//0.俱乐部聊天  1.世界聊天   2.系统聊天    3.私聊

	DWORD	dwClubID;									//俱乐部ID
	DWORD	dwUserID;									//发言玩家ID
	DWORD	dwTagID;									//目标玩家ID

	TCHAR	szUserNickName[32];							//发言玩家昵称
	TCHAR	szHeadUrl[256];								//发言玩家头像地址
	TCHAR	szChat[128];								//发言玩家聊天文字
};

#pragma endregion

#pragma region 5. 战绩查看
//工会战绩统计 查询
struct STR_SUB_CL_CLUB_RECORD_LIST
{
	DWORD						dwUserID;					//玩家ID
	DWORD						dwClubID;					//牌友圈(版本2)索引, 为0表示是大厅
	SYSTEMTIME					tmQueryStartData;			//所要查询的开始日期(最小单位为天)
	SYSTEMTIME					tmQueryEndData;				//所要查询的截止日期(最小单位为天)
};
//工会战绩统计 返回
struct STR_CMD_LC_CLUB_RECORD_LIST
{
	BYTE						byMask;						//1房卡场  2金币场

	DWORD						dwUserID;					//玩家ID
	TCHAR						szNickName[32];				//昵称
	TCHAR						szHeadUrl[LEN_HEAD_URL];	//头像

	DWORD						dwAllCount;					//总场次
	SCORE						lAllScore;					//玩家总场分数

	DWORD						dwWinCount;					//大赢家场次
	SCORE						lWinScore;					//玩家大赢家分数
};

//工会战绩统计 结束
struct STR_CMD_LC_CLUB_RECORD_FINISH
{
	BYTE						byMask;						//结束标志
};

#pragma endregion

#pragma region 待删除
//创建牌友圈房间
struct STR_SUB_CL_CLUB_CREATE_CLUB_ROOM_APPLY
{
	DWORD	dwUserID;									//玩家ID
	DWORD	dwClubID;
	DWORD	dwKindID;									//游戏类型索引
};

//创建牌友圈房间 返回
struct STR_CMD_LC_CLUB_CREATE_CLUB_ROOM_APPLY
{
	LONG	lResultCode;								//0-购买成功  其他失败
	TCHAR	szDescribe[128];							//失败原因
};

//牌友圈创建房间 返回
struct STR_CMD_LC_CLUB_CREATE_ROOM_INFO
{
	LONG	lResultCode;								//0-购买成功  其他失败
	TCHAR	szDescribe[128];							//失败原因
	DWORD	dwRoomID;									//房间ID
};

//申请加入桌子
struct STR_SUB_CL_CLUB_JOIN_TABLE
{
	DWORD	dwUserID;									//玩家ID
	int		JoinType;									//加入类型
	DWORD	dwTableID;									//桌子ID
	DWORD	dwChairID;									//椅子ID
	DWORD	dwCreatePassword;							//创建密码房密码
	DWORD	dwJoinPassword;								//加入密码房密码
};

//申请加入桌子 返回
struct STR_CMD_LC_CLUB_JOIN_TABLE
{
	LONG	lResultCode;								//0-购买成功  其他失败
	TCHAR	szDescribe[128];							//失败原因
	DWORD	dwTableID;									//桌子ID
	DWORD	dwChairID;									//椅子ID
};

#pragma endregion

#endif