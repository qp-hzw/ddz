#ifndef STR_CMD_GAME_SERVER_HEAD_FILE
#define STR_CMD_GAME_SERVER_HEAD_FILE
#include "../服务器组件/游戏服务器/TableFrameBase.h" //因为改结构体tagTableRule
#pragma pack(1)

#pragma region 辅助宏

#define LEN_PRIVATE_TABLE_RULE		256									//自建房规则数据长度
#define NUM_MAX_GOURP_CREATE_ROOM	10									//牌友圈创建的最大房间数

//其他信息
#define DTP_GR_TABLE_PASSWORD		1									//桌子密码

//用户属性
#define DTP_GR_NICK_NAME			10									//用户昵称
#define DTP_GR_GROUP_NAME			11									//社团名字
#define DTP_GR_UNDER_WRITE			12									//个性签名

//附加信息
#define DTP_GR_USER_NOTE			20									//用户备注
#define DTP_GR_CUSTOM_FACE			21									//自定头像

//////////////////////////////////////////////////////////////////////////////////
//桌子模式
#define TABLE_MODE_FK				0									//房卡模式
#define TABLE_MODE_MATCH			1									//比赛模式
#define TABLE_MODE_GOLD				2									//金币模式
#define TABLE_MODE_FK_GOLD			3									//房卡金币模式
#define TABLE_MODE_CLUB				4									//牌友圈模式

//财富类型
#define TREASURE_FK					1									//房卡
#define TREASURE_GOLD				2									//金币
#define TREASURE_DIAMOND			3									//钻石
#define TREASURE_JF					4									//积分


//请求错误
#define REQUEST_FAILURE_NORMAL		0									//常规原因
#define REQUEST_FAILURE_PASSWORD	3									//密码错误

//////////////////////////////////////////////////////////////////////////////////
#pragma endregion

#pragma region 辅助结构体
//单条房间规则
struct rule_item
{
	TCHAR	                       szHeadName[15];						//房间头
	TCHAR	                       szItemValue[4][10];				    //房间选择值
};
//房间规则
struct rule_arry
{
	rule_item	                   ItemArry[20];						//房间规则
};

#pragma endregion

#pragma region MDM_LOGON 登录模块
//ID 登录
struct STR_SUB_CG_LOGON_USERID
{
	DWORD							dwUserID;									//用户 I D
	TCHAR							szPassword[LEN_MD5];						//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];				//机器序列
	DWORD							dwSubGameVersion;							//子游戏版本
};
//ID 登录返回
struct STR_CMD_GC_LOGON_USERID
{
	LONG							lResultCode;								//错误代码
	TCHAR							szDescribeString[LEN_MESSAGE_DESCRIBE];		//消息描述
	DWORD							dwKindID;									//游戏KindID
};
#pragma endregion

#pragma region MDM_USER 用户命令

//申请创建房间
struct STR_SUB_CG_USER_CREATE_ROOM
{
	BYTE							byGameMode;				//游戏模式 0房卡约局; 1比赛模式; 2金币模式; 3金币约局; 4牌友圈
};

//设置房间规则
struct STR_SUB_CG_USER_SET_ROOM_RULE
{
	BYTE							byGameMode;				//游戏模式 0房卡约局; 1比赛模式; 2金币模式; 3金币约局; 4牌友圈
	BYTE							byChoose[20];			//规则选择
};


//club牌友圈创建桌子
struct STR_SUB_CG_USER_CREATE_TABLE
{
	DWORD							dwUserID;				//用户ID
	DWORD							dwClubRoomID;			//俱乐部房间
	BYTE							byTableMode;			//1.普通桌子  2.私密桌子
};
//club创建房间成功返回
struct STR_CMD_GC_USER_CREATE_TABLE
{
	DWORD							dwPassword;				//房间密码
	WORD							wChairID;				//用户椅子号
	BYTE							byAllGameCount;			//总局数
	BYTE							byGameMode;				//游戏模式 0-经典 1-疯狂加被
	BYTE							byZhuangType;			//坐庄模式 0-抢庄 1-轮庄 2-固定庄
};

//加入桌子 -- 需要密码
struct STR_SUB_CG_USER_JOIN_FK_ROOM
{
	DWORD							dwPassword;		//房间密码
};

//加入桌子 -- 不需要密码
struct STR_SUB_CG_USER_JOIN_TABLE_NO_PASS
{
	DWORD							dwUserID;		//玩家ID
	DWORD							dwClubRoomID;	//俱乐部房间
	double							dLongitude;		//经度
	double							dLatitude;		//纬度
};

//加入桌子 -- 加入大厅金币场桌子
struct STR_SUB_CG_USER_JOIN_GOLD_HALL_ROOM
{
	BYTE							byType;			//1.初级场   2.中级场   3.高级场   4.富豪场
};

//断线重连
struct STR_SUB_CG_USER_RECONNECT_ROOM
{
	//BYTE	cbMask;			//0-不重连 1-重连  
};
//断线重连返回
struct STR_CMD_GC_USER_RECONNECT_ROOM
{
	BYTE retCode;						//返回值，0成功  1失败

	tagTableRule					strTableRule;			//大厅的房间规则

	DWORD dwPassWord;					//房间密码
	WORD  wChairID;						//椅子号
	
	BYTE cbGameCount;            //游戏局数
    BYTE cbPlayerCount;          //玩家人数
    BYTE cbPayType;              //支付方式
    DWORD lSinglePayCost;         //单人支付费用
    BYTE gameMod;                //游戏模式
};

//创建牌友圈房间
struct STR_SUB_CG_USER_CREATE_GROUP_ROOM
{
	DWORD		dwUserID;								//创建房间用户ID
	DWORD		dwGroupID;								//牌友圈ID
	BYTE		rule[LEN_PRIVATE_TABLE_RULE];			//创建房间规则
};
//创建牌友圈房间
struct STR_CMD_GC_USER_CREATE_GROUP_ROOM
{
	DWORD       dwResultCode;							 //结果标识
	TCHAR		szDescribeString[LEN_MESSAGE_DESCRIBE];	 //描述消息
};

//用户起立
struct STR_SUB_CG_USER_STANDUP
{
	WORD							wTableID;							//桌子位置		TODO 客户端如何知道桌子ID，应该只知道桌子密码才对
	WORD							wChairID;							//椅子位置
	BYTE							cbForceLeave;						//强行离开
};

//加入牌友圈房间
struct STR_SUB_CG_USER_JOIN_GROUP_ROOM
{
	DWORD		dwGroupID;			//牌友圈ID
	DWORD		dwUserID;			//用户ID
};

//修改用户财富信息返回
struct STR_CMD_GC_USER_MODIFY_TREASURE
{
	SCORE           lUserDiamond;							 //用户钻石
	SCORE			lUserGold;								 //用户金币
	SCORE			lOpenRoomCard;							 //用户房卡
};

//系统消息通知
struct STR_CMD_GR_FRAME_TREASURE_NO
{
	WORD  wChairID;			 //事件用户椅子号
	TCHAR szDescribe[128];   //消息描述
};

//加入电玩房间
struct CMD_GR_JoinDwRoom
{
	BYTE byDwID;			//电玩ID ，暂时只有骰宝 值=1
	BYTE byMode;			//模式，预留
	BYTE byRule[64];		//预留
};

//加入电玩房成功
struct CMD_GR_Join_DwTable_Success
{
	BYTE byDwID;			//电玩ID ，暂时只有骰宝 值=1
	BYTE byMode;			//模式，预留
	WORD wChairID;			//椅子号
	BYTE byRule[64];		//预留
};

//创建金币房间
struct CMD_GR_Create_Gold_Room
{
	int dwCellScore;		//金币场底分
};

//加入成功
struct CMD_GR_Join_GoldTable_Success
{
	WORD							wChairID;
};

//请求房间列表
struct CMD_GR_GetTableList
{
	BYTE							GameMode;	//游戏模式
	BYTE							GoldRank;	//金币等级
	DWORD							dwUserID;	//用户ID
};

//桌子信息 TOODNOW 这些东西肯定都要重写
struct TableInfo
{
	//房间规则
	DWORD	dwTablePassword;
	DWORD	wCellScore;
	BYTE	byMaxPlayerCount;
	BYTE	byCurrPlayerCount;
	DWORD	dwJoinScore;
	DWORD	dwLeaveScore;
	//游戏规则
	BYTE	ZhangMao;					//长毛
	BYTE	XuanFengGang;				//旋风杠
	BYTE	QiXiaoDui;					//七小对
	BYTE	QingYiSe;					//清一色
	BYTE	KaiHuaGang;					//杠开
};

//房间列表
struct CMD_GR_TableListResult
{
	BYTE		byCount;				//房间数量
	BYTE		GoldRank;				//0初级场 50-100 1高级场 100-500 2富豪场 500-5000
	TableInfo	tableInfoList[20];
};

//抽奖通知
struct CMD_GR_LotteryNotify
{
	BYTE	byType;					//类型
	DWORD	dwTimeLeft;				//延时（秒）
};

//请求摇奖
struct CMD_GR_QueryLottery
{
	DWORD	dwUserID;				//用户ID
	BYTE	byType;					//抽奖方式 0-红包 1-摇奖
};

//抽奖结果
struct CMD_GR_LotteryResult
{
	BYTE	byType;					//红包类型  0-没有抽到 1-兑换码 2-金币  3-房卡 4-钻石 5-奖牌 其他-其他预留
	BYTE    byIndex;				//0-11
	DWORD	dwRewardCount;			//奖励道具数量
	TCHAR	szPacketID[128];				//兑换码
	TCHAR	szDescribe[128];		//描述
};

//续费
struct CMD_GF_XUFEI
{
	WORD							wChairID;
	BYTE							bFangzhu;
};

//旁观请求
struct CMD_GR_UserLookon
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
};

//坐下请求
struct CMD_GR_UserSitDown
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
	TCHAR							szPassword[LEN_PASSWORD];			//桌子密码
};

//邀请用户 
struct CMD_GR_UserInvite
{
	WORD							wTableID;							//桌子号码
	DWORD							dwUserID;							//用户 I D
};

//邀请用户请求 
struct CMD_GR_UserInviteReq
{
	WORD							wTableID;							//桌子号码
	DWORD							dwUserID;							//用户 I D
};

//请求失败
struct CMD_GR_RequestFailure
{
	DWORD							lErrorCode;							//错误代码
	TCHAR							szDescribeString[256];				//描述信息
};

//邀请用户
struct CMD_GR_C_InviteUser
{
	WORD							wTableID;							//桌子号码
	DWORD							dwSendUserID;						//发送用户
};

//邀请用户
struct CMD_GR_S_InviteUser
{
	DWORD							dwTargetUserID;						//目标用户
};

//用户拒绝黑名单坐下
struct CMD_GR_UserRepulseSit
{
	WORD							wTableID;							//桌子号码
	WORD							wChairID;							//椅子位置
	DWORD							dwUserID;							//用户 I D
	DWORD							dwRepulseUserID;					//用户 I D
};

//通知用户
struct CMD_GF_OnlinePlayers
{
	WORD wOnlinePlayers;			//在线人数
};

//用户准备
struct STR_SUB_CG_USER_READY
{
};

struct STR_CMD_ROOM_RULE
{
	tagTableRule common;			 //创建房间 frame通用房间规则
	DWORD TableID;                   //房间号
};

//金币场房间信息 返回
struct STR_CMD_GC_USER_GOLD_INFO
{
	BYTE bGoldMod;// 房间等级 1.初级 2.中级 3.高级 4.富豪

	DWORD dwScore;  //底分
	DWORD dwGold;  //入场金币
};

//////////////////////////////////////////////////////////////////////////////////

//规则标志
#define UR_LIMIT_SAME_IP			0x01								//限制地址
#define UR_LIMIT_WIN_RATE			0x02								//限制胜率
#define UR_LIMIT_FLEE_RATE			0x04								//限制逃率
#define UR_LIMIT_GAME_SCORE			0x08								//限制积分

//用户规则
struct CMD_GR_UserRule
{
	BYTE							cbRuleMask;							//规则掩码
	WORD							wMinWinRate;						//最低胜率
	WORD							wMaxFleeRate;						//最高逃率
	LONG							lMaxGameScore;						//最高分数 
	LONG							lMinGameScore;						//最低分数
};

//请求用户信息
struct CMD_GR_UserInfoReq
{
	DWORD                           dwUserIDReq;                        //请求用户
	WORD							wTablePos;							//桌子位置
};

//请求用户信息
struct CMD_GR_ChairUserInfoReq
{
	WORD							wTableID;							//桌子号码
	WORD							wChairID;							//椅子位置
};

//踢出用户
struct CMD_GR_KickUser
{
	DWORD							dwTargetUserID;						//目标用户
};
#pragma endregion

#pragma region  MDM_GR_MATCH 比赛命令
//比赛人数
struct CMD_GR_Match_Num
{
	DWORD							dwWaitting;							//等待人数
	DWORD							dwTotal;							//开赛人数
	DWORD							dwMatchTotal;						//总人数
};

//赛事信息
struct CMD_GR_Match_Info
{
	TCHAR							szTitle[4][64];						//信息标题
    WORD							wGameCount;							//游戏局数
};

//提示信息
struct CMD_GR_Match_Wait_Tip
{
	SCORE							lScore;								//当前积分
	WORD							wRank;								//当前名次
	WORD							wCurTableRank;						//本桌名次
	WORD							wUserCount;							//当前人数
	WORD							wPlayingTable;						//游戏桌数
	TCHAR							szMatchName[LEN_SERVER];			//比赛名称
};

//比赛结果
struct CMD_GR_MatchResult
{
	TCHAR							szDescribe[256];					//得奖描述
	DWORD							dwGold;								//金币奖励
	DWORD							dwMedal;							//奖牌奖励
	DWORD							dwExperience;						//经验奖励
};

#define MAX_MATCH_DESC				4									//最多描述
//比赛描述
struct CMD_GR_MatchDesc
{
	TCHAR							szTitle[MAX_MATCH_DESC][16];		//信息标题
	TCHAR							szDescribe[MAX_MATCH_DESC][64];		//描述内容
	COLORREF						crTitleColor;						//标题颜色
	COLORREF						crDescribeColor;					//描述颜色
};
#pragma endregion

#pragma region MDM_G_FRAME 框架命令
//游戏配置
struct CMD_GF_GameOption
{
};

//旁观配置
struct CMD_GF_LookonConfig
{
	DWORD							dwUserID;							//用户标识
	BYTE							cbAllowLookon;						//允许旁观
};

//旁观状态
struct CMD_GF_LookonStatus
{
	BYTE							cbAllowLookon;						//允许旁观
};

//游戏环境
struct CMD_GF_GameStatus
{
	BYTE							cbUserAction;							//游戏状态
	tagUserInfo						UserInfo;								//玩家信息
};

//用户聊天
struct STR_SUB_RG_FRAME_CHAT
{
	DWORD							dwUserID;							//用户ID
	DWORD							dwMsgID;							//语音标识
};

//用户聊天
struct STR_CMD_GR_FRAME_CHAT
{
	DWORD							dwUserID;							//信息颜色
	DWORD							dwChairID;							//发送用户
	DWORD							dwMsgID;							//目标用户
};

//用户表情
struct CMD_GF_C_UserExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwTargetUserID;						//目标用户
};

//用户表情
struct CMD_GF_S_UserExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
};

//申请解散房间
struct STR_SUB_RG_FRAME_ASK_DISMISS
{
	
};
//广播申请解散房间
struct STR_CMD_GR_FRMAE_VOTE_DISMISS
{
	BYTE							cbChairID;		//id
	BYTE							cbAgree;		//0-同意  1-不同意
	BYTE							cbApplyChair;	//申请解散者的chair
};

//申请解散房间 结果
struct STR_CMD_GR_FRMAE_ASK_DISMISS_RESULT
{
	LONG							lResultCode;								//错误代码 0-投票解散房间
	TCHAR							szDescribeString[LEN_MESSAGE_DESCRIBE];		//消息描述
};

//表决解散房间
struct STR_SUB_RG_FRAME_VOTE_DISMISS
{
	BYTE					cbAgree;						//是否同意解散		0-不同意	1-同意
};
//广播解散房间结果
struct STR_CMD_GR_FRAME_DISMISS_RESULT
{
	BYTE					cbDismiss;						//0-未解散  1-解散成功（用户起立）	2-解散失败
};

//请求校验用户GPS位置信息
struct STR_SUB_CG_COM_CHECK_USER_GPS
{
	BYTE					cbRequestFlag;					//请求标识	1-请求	0-未请求
};

//校验用户GPS位置信息返回
struct STR_CMD_CG_COM_CHECK_USER_GPS
{
	DWORD					dwFirstUserID;					//第一个玩家
	DWORD					dwNextUserID;					//第二个玩家
	double					fDistance;						//两个玩家的距离
	BYTE					cbIsCloser;						//距离小于（200m）报警	1-报警	0-不报警
};

//请求校验用户IP
struct STR_SUB_CG_COM_CHECK_USER_IP
{
	BYTE					cbRequestFlag;					//请求标识	1-请求	0-未请求
};

//校验用户IP返回
struct STR_CMD_CG_COM_CHECK_USER_IP
{
	DWORD					dwFirstUserID;					//第一个玩家
	DWORD					dwNextUserID;					//第二个玩家
	BYTE					cbISame;						//IP相同	1-相同	0-不同
};

//游戏内聊天
struct STR_SUB_CG_FRAME_USER_CHAT
{
	DWORD dwUserID;				//用户ID
	DWORD dwMsgID;				//0-15:语音标识
};

//游戏内聊天返回
struct STR_CMD_GC_FRAME_USER_CHAT
{
	DWORD dwUserID;				//用户ID
	DWORD dwChairID;			//椅子号
	DWORD dwMsgID;				//0-15:语音标识
};


//断线重连  解散面板状态
struct STR_CMD_GR_FRAME_GAME_DISSMISS
{
	DWORD					dwUserID[MAX_CHAIR];					//用户ID
	BYTE					cbAgree[MAX_CHAIR];						    //是否同意解散		0-不同意	1-同意   2-未表决
};


//消耗道具
struct STR_SUB_CG_EFFECT
{
	BYTE					target_chair;						    //目标玩家 椅子号
	DWORD					dwGoodsID;								//物品ID
};


//消耗道具广播
struct STR_CMD_GC_EFFECT_BRODCAST
{
	BYTE					from_chair;								//from 椅子号
	BYTE					target_chair;						    //to 椅子号
	DWORD					dwGoodsID;								//物品ID
};

#pragma endregion

#pragma pack()

#endif
