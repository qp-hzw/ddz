#ifndef CMD_GAME_SERVER_HEAD_FILE
#define CMD_GAME_SERVER_HEAD_FILE

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
//TODO 暂时放在这里，后面可以单独用文件存储
//桌子模式
#define TABLE_MODE_FK				0									//房卡模式
#define TABLE_MODE_JJC				1									//竞技场模式
#define TABLE_MODE_GOLD				2									//金币模式
#define TABLE_MODE_GROUP			3									//牌友圈房间模式
#define TABLE_MODE_DW				4									//电玩模式

//请求错误
#define REQUEST_FAILURE_NORMAL		0									//常规原因
#define REQUEST_FAILURE_NOGOLD		1									//金币不足
#define REQUEST_FAILURE_NOSCORE		2									//积分不足
#define REQUEST_FAILURE_PASSWORD	3									//密码错误
#define REQUEST_FAILURE_END			4									//游戏结束
#define REQUEST_FAILURE_NOROOMCARD	5									//钻石不足

//////////////////////////////////////////////////////////////////////////////////
#pragma endregion

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

#pragma region 主消息号1
/* *********************************************************************************
**						MAIN:1   MDM_LOGON    登录 模块
** *********************************************************************************/
#define MDM_GR_LOGON				1									//登录信息

//登录
#define SUB_CG_LOGON_USERID			1									//ID 登录
#define CMD_GC_LOGON_USERID			101									//ID 登录返回

#define CMD_GC_LOGON_GET_USER_INFO	102									//获得用户基本信息	USERID登录游戏服成功之后会发送给客户端

//////////////////////////////////////////////////////////////////////////////////

//ID 登录
struct STR_SUB_CG_LOGON_USERID
{
	DWORD							dwUserID;									//用户 I D
	TCHAR							szPassword[LEN_MD5];						//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];				//机器序列
	double							dLongitude;									//登录游戏服时GPS经度
	double							dLatitude;									//登录游戏服时GPS纬度
};
//ID 登录返回
struct STR_CMD_GC_LOGON_USERID
{
	LONG							lResultCode;								//错误代码
	TCHAR							szDescribeString[LEN_MESSAGE_DESCRIBE];		//消息描述
	DWORD							dwOffLineGameID;							//断线重连的游戏ID, 非0表示是断线重连
};
#pragma endregion

#pragma region 主消息号199
/* *********************************************************************************
**						MAIN:199   MDM_CONFIG    配置命令
** *********************************************************************************/
#define MDM_CONFIG					199									//配置信息 TODONOW待删除

#define CMD_GC_CONFIG_ROOM			101									//房间配置		登录成功游戏服返回给客户端
#define CMD_GC_CONFIG_PROPERTY		102									//道具配置		登录成功游戏服返回给客户端

//////////////////////////////////////////////////////////////////////////////////

//房间配置
struct STR_CMD_GC_CONFIG_ROOM
{
	//房间属性
	WORD							wTableCount;						//桌子数目
	WORD							wChairCount;						//椅子数目

	//房间配置
	WORD							wServerType;						//房间类型
	DWORD							dwServerRule;						//房间规则
};

//道具配置
struct STR_CMD_GC_CONFIG_PROPERTY
{
	BYTE							cbPropertyCount;					//道具数目
	tagPropertyInfo					PropertyInfo[MAX_PROPERTY];			//道具描述
};
#pragma endregion

#pragma region 主消息号2
/* *********************************************************************************
**							MAIN:2   MDM_USER    用户命令
** *********************************************************************************/
#define MDM_USER						2									//用户信息

//子消息  *表示客户端未用到		#初步整理		
#define SUB_CG_USER_RULE				1									//*用户规则		TODO 貌似是限制用户IP、胜率、逃跑率、积分，应该需要保留
#define SUB_CG_USER_LOOKON				2									//*旁观请求		TODO 成功无返回，失败返回103请求失败
#define SUB_CG_USER_SITDOWN				3									//*坐下请求（用户加入/创建房间成功后，服务器主动让玩家坐下，不需要请求）
#define SUB_CG_USER_STANDUP				4									//#起立请求		TODO 非房主解散房间，发送的是该消息，放在15的处理流程中
#define SUB_CG_USER_INVITE_USER			5									//*邀请用户进入桌子
#define CMD_GC_USER_INVITE_USER			6									//*邀请用户进入桌子返回
#define SUB_CG_USER_REFUSE_SIT  		7									//*拒绝玩家坐下	TODO 让玩家起立，返回系统消息，不让用户坐下
#define SUB_CG_USER_KICK_USER			8                                   //*踢出用户		TODO 让玩家起立，返回系统消息，踢出用户

#define SUB_GR_USER_CHAIR_REQ			10                                  //*请求更换位置
#define SUB_GR_USER_WAIT_DISTRIBUTE		12									//等待分配		TODO 再用户准备后，发送的空包，客户端如何处理了？

#define SUB_CG_USER_CREATE_FK_ROOM		13									//#创建房卡房间
#define CMD_GC_USER_CREATE_ROOM_SUCCESS	121									//#创建成功		TODO 创建房卡房间和牌友圈房间的返回，其实创建房间的成功返回应该都用该消息

#define SUB_CG_USER_JOIN_FK_ROOM		14									//#加入房卡房间
#define CMD_GC_USER_JOIN_ROOM_SUCCESS	122									//#加入成功		加入房卡/金币/牌友圈房间的返回

#define SUB_CG_USER_JOIN_GROUP_ROOM		18									//#加入牌友圈房间 返回加入房间成功
#define SUB_CG_USER_JOIN_GOLD_ROOM		19									//#加入金币房（没有空位置金币房，则先创建） TODO 为了大厅通用性，房间规则由客户端游戏决定
#define SUB_GR_JOIN_DW_ROOM				20									//*加入电玩场（骰宝）	

#define SUB_GR_GET_TABLELIST			21									//获取房间列表		TODO 指的是竞技场的已开房间列表
#define SUB_GR_GET_TABLELIST_RESULT		131									//房间列表返回		

#define SUB_GR_LOTTERY_NOTIFY			22									//抽奖通知 CMD TODO 发送的空包
#define SUB_GR_QUERY_LOTTERY			23									//请求抽奖 SUB
#define SUB_GR_LOTTERY_RESULT			132									//抽奖结果 CMD

#define SUB_CG_USER_RECONNECT_ROOM		24									//#断线重连
#define CMD_GC_USER_RECONNECT_ROOM		134									//#断线重连返回

#define SUB_CG_USER_CREATE_GROUP_ROOM	25									//#创建牌友圈房间（只有圈主才能创建房间）	TODO 牌友圈放在单独的消息号中
#define CMD_GC_USER_CREATE_GROUP_ROOM	205									//#创建牌友圈房间返回

#define SUB_GR_USER_SCORE				101									//用户分数		TODO 子游戏调用写分函数，大厅发送给客户端
#define SUB_GR_USER_STATUS				102									//用户状态		TODO 设置用户状态时，会调用OnEventUserItemStatus，它会发送该消息给客户端
#define SUB_GR_REQUEST_FAILURE			103									//请求失败		大部分失败消息都返回这个消息
#define SUB_GR_NOTIFY_MSG				105									//*通知消息
#define CMD_GC_USER_MODIFY_TREASURE		106									//修改用户财富信息返回

#define SUB_GR_CREATE_OTHER_SUCCESS		129									//*替他人开房成功		TODO 删除，替他人开房成功放在开房成功中，不需要单独处理
#define SUB_GR_JOIN_DWTABLE_SUCCESS		133									//*加入电玩房成功		逻辑处理和其他加入房间不同，貌似是加入成功就开始游戏了

#define CMD_GR_TABLE_INFO				306									//*更新开房信息		TODO 需要删除，与主消息号为4的子消息号同名
#define SUB_GR_TABLE_CHAT				308									//*桌子聊天			TODO SUB和CMD的主消息号不一样，需要修改

#define SUB_GF_ONLINE_PLAYERS			313									//*在线人数

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//创建房卡房间
struct STR_SUB_CG_USER_CREATE_FK_ROOM
{
	BYTE rule[LEN_PRIVATE_TABLE_RULE];						//创建房间规则
};
//创建房间成功返回
struct STR_CMD_GC_USER_CREATE_ROOM_SUCCESS
{
	DWORD							dwPassword;				//房间密码
	WORD							wChairID;				//用户椅子号
	BYTE							byAllGameCount;			//总局数
	BYTE							byGameMode;				//游戏模式 0-经典 1-疯狂加被
	BYTE							byZhuangType;			//坐庄模式 0-抢庄 1-轮庄 2-固定庄
};

//加入房卡房间
struct STR_SUB_CG_USER_JOIN_FK_ROOM
{
	DWORD							dwPassword;		//房间密码
	double							dLongitude;		//经度
	double							dLatitude;		//纬度
};
//加入房间成功返回
struct STR_CMD_GC_USER_JOIN_ROOM_SUCCESS
{
	DWORD							dwRoomID;
	WORD							wChairID;
	WORD							wPlayerCount;
	BYTE							byAllGameCount;			//总局数
	BYTE							byGameMode;				//麻将：0-房卡模式  1-竞技模式 2-金币模式；  游戏模式 0-经典 1-疯狂加倍
	BYTE							byZhuangType;			//坐庄模式 0-抢庄 1-轮庄 2-固定庄
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
struct CMD_GR_NotifyMsg
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

//游戏服 主消息号 MDM_USER 3，副消息号SUB_GR_TABLE_CHAT 308   收发一致
struct CMD_GR_TableChat
{
	DWORD dwUserID;				//用户ID
	DWORD dwChairID;			//用户椅子号
	DWORD dwTableID;			//房间号
	DWORD dwMsgID;				//0-99 自带文字，100-199 自带表情，255,自定义文字
	TCHAR szMsg[256];			//自定义文字内容
};

//加入电玩房成功
struct CMD_GR_Join_DwTable_Success
{
	BYTE byDwID;			//电玩ID ，暂时只有骰宝 值=1
	BYTE byMode;			//模式，预留
	WORD wChairID;			//椅子号
	BYTE byRule[64];		//预留
};

//房间规则
struct tagTableCfg
{
	//通用属性 
	BYTE	GameMode;				//游戏模式 *牛牛：0-牛牛经典模式 1-牛牛疯狂加倍   *麻将：0-房卡模式  1-竞技模式 2-金币模式 
	BYTE	RobBankType;			//坐庄模式 0-抢庄（抢注高者坐庄） 1-轮庄（轮流坐庄）2-固定（固定房主坐庄）  3-赢庄（赢的人坐庄） 4-摇骰子定庄（随机庄家）
	BYTE	GameCount;				//游戏局数 0-无限局
	BYTE	PlayerCount;			//玩家数量 0-任意人数可开
	BYTE	cbPayType;				//支付方式，0房主支付、1AA制
	DWORD	lSinglePayCost;			//AA每个人需要的房卡或房主支付需要的总房卡
	WORD	CellScore;				//底分	  默认为1
	DWORD	MaxFan;					//封顶番数 0-不封顶 
	DWORD	LeaveScore;				//离场分数 0-无限制
	DWORD	JoinScore;				//入场设定 0-无限制  
	BYTE	bRefuseSameIP;			//允许同IP    0-不允许 1-允许
	BYTE	bDistanceIn300;			//允许300米	  0-不许云 1-允许
	double	dLongitude;				//经度	
	double	dLatitude;				//纬度	
	BYTE	bCreateToOther;			//是否为他人开房 0-否 1-是
	DWORD	FangZhu;				//房主    创建用户的ID
	BYTE	bAllowStranger;			//允许陌生人加入

	//子游戏特有规则
	BYTE	ZhangMao;					//长毛
	BYTE	XuanFengGang;				//旋风杠
	BYTE	QiXiaoDui;					//七小对
	BYTE	QingYiSe;					//清一色
	BYTE	KaiHuaGang;					//杠开
	BYTE	Other0;
	BYTE	Other1;
	BYTE	Other2;
	BYTE	Other3;
	BYTE	Other4;
	BYTE	Other5;
	BYTE	Other6;
	BYTE	Other7;
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

//用户分数
struct CMD_GR_UserScore
{
	DWORD							dwUserID;							//用户标识
	tagUserScore					UserScore;							//积分信息
};

//用户分数
struct CMD_GR_MobileUserScore
{
	DWORD							dwUserID;							//用户标识
	tagMobileUserScore				UserScore;							//积分信息
};

//用户状态
struct CMD_GR_UserStatus
{
	DWORD							dwUserID;							//用户标识
	tagUserStatus					UserStatus;							//用户状态
};

//请求失败
struct CMD_GR_RequestFailure
{
	LONG							lErrorCode;							//错误代码
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
#pragma endregion

#pragma region 主消息号4
/* *********************************************************************************
**						 MAIN:3   MDM_GR_STATUS    状态命令
** *********************************************************************************/
#define MDM_GR_STATUS				3									//状态信息

#define CMD_GR_TABLE_INFO			100									//桌子信息
#define CMD_GR_TABLE_STATUS			101									//桌子状态

//////////////////////////////////////////////////////////////////////////////////

//桌子信息
struct CMD_GR_TableInfo
{
	WORD							wTableCount;						//桌子数目
	tagTableStatus					TableStatusArray[512];				//桌子状态
};

//桌子状态
struct CMD_GR_TableStatus
{
	WORD							wTableID;							//桌子号码
	tagTableStatus					TableStatus;						//桌子状态
};
#pragma endregion

#pragma region 主消息号6
/* *********************************************************************************
**					 MAIN:6   MDM_GR_MANAGE    管理命令
** *********************************************************************************/
#define MDM_GR_MANAGE				6									//管理命令

#define SUB_GR_SEND_WARNING			1									//发送警告
#define SUB_GR_SEND_MESSAGE			2									//发送消息
#define SUB_GR_LOOK_USER_IP			3									//查看地址
#define SUB_GR_KILL_USER			4									//踢出用户
#define SUB_GR_LIMIT_ACCOUNS		5									//禁用帐户
#define SUB_GR_SET_USER_RIGHT		6									//权限设置

//房间设置
#define SUB_GR_QUERY_OPTION			7									//查询设置
#define SUB_GR_OPTION_SERVER		8									//房间设置
#define SUB_GR_OPTION_CURRENT		9									//当前设置

#define SUB_GR_LIMIT_USER_CHAT		10									//限制聊天

#define SUB_GR_KICK_ALL_USER		11									//踢出用户
#define SUB_GR_DISMISSGAME		    12									//解散游戏

//////////////////////////////////////////////////////////////////////////////////

//发送警告
struct CMD_GR_SendWarning
{
	WORD							wChatLength;						//信息长度
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szWarningMessage[LEN_USER_CHAT];	//警告消息
};

//系统消息
struct CMD_GR_SendMessage
{
	BYTE							cbGame;								//游戏消息
	BYTE							cbRoom;								//游戏消息
	BYTE							cbAllRoom;							//游戏消息
	WORD							wChatLength;						//信息长度
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//系统消息
};

//查看地址
struct CMD_GR_LookUserIP
{
	DWORD							dwTargetUserID;						//目标用户
};

//踢出用户
struct CMD_GR_KickUser
{
	DWORD							dwTargetUserID;						//目标用户
};

//禁用帐户
struct CMD_GR_LimitAccounts
{
	DWORD							dwTargetUserID;						//目标用户
};

//权限设置
struct CMD_GR_SetUserRight
{
	//目标用户
	DWORD							dwTargetUserID;						//目标用户

	//绑定变量
	BYTE							cbGameRight;						//帐号权限
	BYTE							cbAccountsRight;					//帐号权限

	//权限变化
	BYTE							cbLimitRoomChat;					//大厅聊天
	BYTE							cbLimitGameChat;					//游戏聊天
	BYTE							cbLimitPlayGame;					//游戏权限
	BYTE							cbLimitSendWisper;					//发送消息
	BYTE							cbLimitLookonGame;					//旁观权限
};

//房间设置
struct CMD_GR_OptionCurrent
{
	DWORD							dwRuleMask;							//规则掩码
	tagServerOptionInfo				ServerOptionInfo;					//房间配置
};

//房间设置
struct CMD_GR_ServerOption
{
	tagServerOptionInfo				ServerOptionInfo;					//房间配置
};

//踢出所有用户
struct CMD_GR_KickAllUser
{
	TCHAR							szKickMessage[LEN_USER_CHAT];		//踢出提示
};

//解散游戏
struct CMD_GR_DismissGame
{
	WORD							wDismissTableNum;		            //解散桌号
};
//////////////////////////////////////////////////////////////////////////////////

//设置标志
#define OSF_ROOM_CHAT				1									//大厅聊天
#define OSF_GAME_CHAT				2									//游戏聊天
#define OSF_ROOM_WISPER				3									//大厅私聊
#define OSF_ENTER_TABLE				4									//进入游戏
#define OSF_ENTER_SERVER			5									//进入房间
#define OSF_SEND_BUGLE				12									//发送喇叭

//房间设置
struct CMD_GR_OptionServer
{
	BYTE							cbOptionFlags;						//设置标志
	BYTE							cbOptionValue;						//设置标志
};

//限制聊天
struct CMD_GR_LimitUserChat
{
	DWORD							dwTargetUserID;						//目标用户
	BYTE							cbLimitFlags;						//限制标志
	BYTE							cbLimitValue;						//限制与否
};
#pragma endregion

#pragma region 主消息号7
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
////////////////////////////////////////////////////////////////////////////////////
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

#pragma region 主消息号8
/* *********************************************************************************
**							MAIN:8   MDM_PROPERTY    道具命令
** *********************************************************************************/
#define MDM_PROPERTY					8								

#define SUB_GR_PROPERTY_BUY				300									//*购买道具
#define SUB_GR_PROPERTY_SUCCESS			301									//*道具成功
#define SUB_GR_PROPERTY_FAILURE			302									//*道具失败
#define SUB_GR_PROPERTY_MESSAGE			303                                 //*道具消息
#define SUB_GR_PROPERTY_EFFECT			304                                 //*道具效应
#define SUB_GR_PROPERTY_TRUMPET			305                                 //*喇叭消息
#define SUB_GR_QUERY_USER_PROP			309									//*查询用户道具
#define SUB_GR_USER_PROP_RESULT			310									//*查询用户道具返回
#define SUB_GR_MODIFY_USER_PROP			311									//*增加\减少用户道具
#define SUB_GR_MODIFY_PROP_RESULT		312									//*修改道具结果
///////////////////////////////////////////////////////////////////////////////////////

//购买道具
struct CMD_GR_C_PropertyBuy
{
	BYTE                            cbRequestArea;						//请求范围
	BYTE							cbConsumeScore;						//积分消费
	WORD							wItemCount;							//购买数目
	WORD							wPropertyIndex;						//道具索引	
	DWORD							dwTargetUserID;						//使用对象
};

//道具成功
struct CMD_GR_S_PropertySuccess
{
	BYTE                            cbRequestArea;						//使用环境
	WORD							wItemCount;							//购买数目
	WORD							wPropertyIndex;						//道具索引
	DWORD							dwSourceUserID;						//目标对象
	DWORD							dwTargetUserID;						//使用对象
};

//道具失败
struct CMD_GR_PropertyFailure
{
	WORD                            wRequestArea;                       //请求区域
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[256];				//描述信息
};

//道具消息
struct CMD_GR_S_PropertyMessage
{
	//道具信息
	WORD                            wPropertyIndex;                     //道具索引
	WORD                            wPropertyCount;                     //道具数目
	DWORD                           dwSourceUserID;                     //目标对象
	DWORD                           dwTargerUserID;                     //使用对象
};


//道具效应
struct CMD_GR_S_PropertyEffect
{
	DWORD                           wUserID;					        //用 户I D
	BYTE							cbMemberOrder;						//会员等级
};

//发送喇叭
struct CMD_GR_C_SendTrumpet
{
	BYTE                           cbRequestArea;                        //请求范围 
	WORD                           wPropertyIndex;                      //道具索引 
	DWORD                          TrumpetColor;                        //喇叭颜色
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //喇叭内容
};

//发送喇叭
struct CMD_GR_S_SendTrumpet
{
	WORD                           wPropertyIndex;                      //道具索引 
	DWORD                          dwSendUserID;                         //用户 I D
	DWORD                          TrumpetColor;                        //喇叭颜色
	TCHAR                          szSendNickName[32];				    //玩家昵称 
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //喇叭内容
};

//查询用户道具
struct CMD_GR_QueryUserProp
{
	DWORD dwUserID;
};

//查询返回
struct CMD_GR_UserPropResult
{
	DWORD dwUserID;			//用户ID
	WORD  wPropID[5];		//道具ID
	LONGLONG lPropCount[5];	//道具数量
};

//增加\减少用户道具
struct CMD_GR_ModifyUserProp
{
	BYTE	bAdd;		//0表示减少，>0表示增加
	DWORD	dwUserID;
	WORD	wPropID;
	LONGLONG lPropCount;
};

//修改道具结果
struct CMD_GR_ModifyPropResult
{
	WORD	wErrCode;		//错误码
	DWORD	dwUserID;		//用户ID
	WORD	wPropID;		//道具ID
	LONGLONG lPropCount;	//操作后的道具数量
	TCHAR	szDescribe[128]; //描述
};
#pragma endregion

#pragma region 主消息号100
/* *********************************************************************************
**							MAIN:100   MDM_G_FRAME    框架命令
** *********************************************************************************/
#define MDM_G_FRAME					100									//框架命令

//房间信息
#define SUB_RG_FRAME_OPTION					1								//游戏配置
#define CMD_GR_FRAME_GAME_OPTION			101								//游戏场景
#define CMD_GR_FRAME_GAME_STATUS			102								//游戏状态

//解散房间
#define SUB_RG_FRAME_ASK_DISMISS			2								//申请解散房间
#define	SUB_RG_FRAME_VOTE_DISMISS			3								//表决解散房间

#define CMD_GR_FRMAE_VOTE_DISMISS			103								//表决是否解散房间
#define	CMD_GR_FRAME_DISMISS_RESULT			104								//解散房间结果

//GPS测距
#define CMD_GC_COM_CHECK_USER_GPS			105								//校验用户GPS位置信息返回



//下面都是待整理 client有, 但没有整理
#define SUB_GF_USER_READY				4									//用户准备
#define SUB_GF_USER_CHAT				5									//用户聊天

#define CMD_GF_SYSTEM_MESSAGE			106									//系统消息


//下面client没有
#define SUB_GF_LOOKON_CONFIG			803									//旁观配置
#define SUB_GF_HEADURL					804									//*头像地址
#define SUB_GF_XUFEI					805									//*续费消息

#define SUB_GF_USER_EXPRESSION			811									//*用户表情
#define SUB_GF_SOUND					812									//*发送语音
#define SUB_GF_HEAD						813									//*发送头像

//游戏信息
#define CMD_GF_LOOKON_STATUS			803									//*旁观状态

//系统消息
#define CMD_GF_SYSTEM_MESSAGE			806									//系统消息
#define SUB_GF_ACTION_MESSAGE			901									//动作消息
#define SUB_GF_MARQUEE_MESSAGE			902									//跑马灯消息

//////////////////////////////////////////////////////////////////////////////////////

//游戏配置
struct CMD_GF_GameOption
{
	BYTE							cbAllowLookon;						//旁观标志
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwClientVersion;					//游戏版本
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
	BYTE							cbGameStatus;						//游戏状态
	BYTE							cbAllowLookon;						//旁观标志
};

//头像地址
struct CMD_GF_C_HeadUrl
{
	TCHAR							szHeadUrl[LEN_HEAD_URL];
};

//头像地址
struct CMD_GF_S_HeadUrl
{
	WORD							wChairID;
	TCHAR							szHeadUrl[LEN_HEAD_URL];
};


//用户聊天
struct CMD_GF_C_UserChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//用户聊天
struct CMD_GF_S_UserChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
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
	DWORD					dwApplyUserID;					//申请解散用户ID
	BYTE					cbAgree;						//是否同意解散		0-不同意	1-同意
};
//表决是否解散房间
struct STR_CMD_GR_FRMAE_VOTE_DISMISS
{
	DWORD					dwVoteUser[5];					//需要投票解散的玩家	//客户端目前最多显示5个人
	TCHAR					szApplyUserNick[LEN_NICKNAME];	//申请解散用户昵称
};

//表决解散房间
struct STR_SUB_RG_FRAME_VOTE_DISMISS
{
	DWORD					dwVoteUserID;					//表决解散房间用户ID
	BYTE					cbAgree;						//是否同意解散		0-不同意	1-同意
};
//解散房间结果
struct STR_CMD_GR_FRAME_DISMISS_RESULT
{
	BYTE					cbDismiss;						//0-未解散  1-解散成功（用户起立）	2-解散失败
	DWORD					dwVoteUserID;					//表决解散房间用户ID
	TCHAR					szVoteUserNick[LEN_NICKNAME];	//申请解散用户昵称
	BYTE					cbAgree;						//是否同意解散
};

//校验用户GPS位置信息返回
struct STR_CMD_CG_COM_CHECK_USER_GPS
{
	DWORD					dwFirstUserID;					//第一个玩家
	DWORD					dwNextUserID;					//第二个玩家
	double					fDistance;						//两个玩家的距离
};
#pragma endregion


#pragma region 主消息号200
/* *********************************************************************************
**							MAIN:200   MDM_GF_GAME    子游戏命令
** *********************************************************************************/
#define MDM_GF_GAME					200									//游戏命令

#pragma endregion

#pragma pack()

#endif