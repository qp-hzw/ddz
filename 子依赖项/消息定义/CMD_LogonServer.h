#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#pragma pack(1)

/*
** 简要描述:  登录服 与 其他进程的 消息
** 详细描述:  1. 消息号(主 + 子)     2. 结构体
** 备注说明:  子消息号的含义: 
**            1. 第一个字段: 归属标志,        SUB(接受到的消息)， CMD(发送出去的消息), DBR(向数据库Request)，DBO(数据库返回的消息)
**            2. 第二个字段: 消息流向         L: logon;  G:gameserver;  C:client; CP:correspond;  W:web
**            3. 第三个字段: 主消息号的标志   比如LOGON即为登录模块的消息
**            4. 之后的字段: 子消息号的含义   比如Accounts表示账号登录
**           
**            命名规范
**            1. 结构体的命名:  在消息号的前面加STR, 比如SUB_CL_LOGON_ACCOUNTS 的为 STR_SUB_CL_LOGON_ACCOUNTS
**
**            2. DBO, DBR消息号的命名为:  将SUB替换为DBR或DBO即可
**
**            3. 变量的命名规范: 1) 指针加p  2)归属标志SUB  3)主消息号标志  4)子消息号标志
**
**            4. 函数的命名:  On_归属标志_主消息号子消息号
**
**
*/

/* *********************************************************************************
**      MAIN:1   MDM_LOGON    登录 模块
** *********************************************************************************/
#define MDM_LOGON					1									//主消息

#define SUB_CL_LOGON_ACCOUNTS		1									//帐号登录
#define CMD_LC_LOGON_ACCOUNTS		101									//账号登录

#define SUB_CL_LOGON_REGISTER	    2									//帐号注册（返回的是账号登录）
#define CMD_LC_LOGON_REGISTER		102									//账号注册返回

#define SUB_CL_LOGON_PLATFORM		3									//平台登陆（第三方登录）
#define CMD_LC_LOGON_PLATFORM       103                                 //平台登陆返回

#define CMD_LC_LOGON_REPEAT_LOGON	104									//重复登录

#define CMD_LC_LOGON_LOGON_REWARD   105                                 //登录奖励（老玩家回归 + 节日奖励）

#define CMD_LC_LOGON_UPDATE_NOTIFY	106									//升级提示（登录 + 注册 + 平台登录都会用到）

//////////////////////////////////////////////////////////////////////////////////

//帐号登录  
struct STR_SUB_CL_LOGON_ACCOUNTS
{
	DWORD							dwVersionCheck;						//版本校验（32位 = 代码版本(8位) + 内核版本(8位) + 平台版本(8位) + 子游戏版本(8位)）
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};
//账号登录返回
struct STR_CMD_LC_LOGON_ACCOUNTS
{
	//操作结果
	DWORD                           dwResultCode;                       //结果标识
	//描述信息
	TCHAR							szDescribeString[128];				//描述消息

	/* *****************************    用户信息     ****************************/
	//用户标识
	DWORD							dwUserID;							//用户标识
	//基本属性
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	BYTE							cbGender;							//用户性别
	WORD							wFaceID;							//头像索引
	TCHAR							szMySignature[LEN_MY_SIGNATURE];	//个性签名
	//荣誉属性
	SCORE                           dwUserDiamond;						//用户元宝
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力
	//用户权限
	BYTE                            cbMasterOrder;                      //管理员等级 0表示非管理员
	BYTE							cbMemberOrder;						//会员等级   0表示非管理员
	//SYSTEMTIME						MemberOverDate;						//会员到期时间
	//联系方式
	TCHAR                           szIdentityName[LEN_IDENTITY_NAME];  //真实姓名
	TCHAR                           szIdentityNum[LEN_IDENTITY_NUM];    //身份证号
	TCHAR                           szMobilePhone[LEN_MOBILE_PHONE];    //手机号码
	
	/* *****************************    账号信息     ****************************/
	TCHAR							szLasLogonIp[LEN_IP_ADDR];			//最后登录地址
	SYSTEMTIME						LasLogonDate;						//最后上线时间
	
	/* *****************************    附加数据     ****************************/
	//牌友圈 
	DWORD							dwGroupID;							//社团标识
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字

	//财富信息
	SCORE							lUserScore;							//用户积分
	SCORE							lUserGold;							//用户金币
	SCORE							lUserRoomCard;						//用户房卡
};

//注册帐号
struct STR_SUB_CL_LOGON_REGISTER
{
	DWORD							dwVersionCheck;						//版本校验
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	BYTE							cbGender;							//用户性别
	TCHAR                           strMobilePhone[LEN_MOBILE_PHONE];   //手机号码 可以为null
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};
//账号注册返回
struct STR_CMD_LC_LOGON_REGISTER
{
	//操作结果
	DWORD                           dwResultCode;                       //结果标识
	//描述信息
	TCHAR							szDescribeString[128];				//描述消息
	//用户标识
	DWORD							dwUserID;							//用户标识
	//基本属性
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	BYTE							cbGender;							//用户性别
	WORD							wFaceID;							//头像索引
	TCHAR							szMySignature[LEN_MY_SIGNATURE];	//个性签名
	//荣誉属性
	SCORE                           dwUserDiamond;						//用户元宝
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力
	//用户权限
	BYTE                            cbMasterOrder;                      //管理员等级 0表示非管理员
	BYTE							cbMemberOrder;						//会员等级   0表示非管理员
	//SYSTEMTIME						MemberOverDate;						//会员到期时间
	//联系方式
	TCHAR                           szIdentityName[LEN_IDENTITY_NAME];  //真实姓名
	TCHAR                           szIdentityNum[LEN_IDENTITY_NUM];    //身份证号
	TCHAR                           szMobilePhone[LEN_MOBILE_PHONE];    //手机号码	
	//账号信息
	TCHAR							szLasLogonIp[LEN_IP_ADDR];			//最后登录地址
	//SYSTEMTIME						LasLogonDate;						//最后上线时间
	//牌友圈 
	DWORD							dwGroupID;							//社团标识
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字
	//财富信息
	SCORE							lUserScore;							//用户游戏币
	SCORE							lUserGold;							//用户金币
	SCORE							lOpenRoomCard;						//用户房卡
};

//平台登录  
struct STR_SUB_CL_LOGON_PLATFORM
{
	DWORD							dwVersionCheck;						//版本校验
	TCHAR							szOpenID[33];						//OpenID | tokenID
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	BYTE							cbGender;							//用户性别
	TCHAR							strHeadUrl[LEN_HEAD_URL];			//头像地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};
//平台登录
struct STR_CMD_LC_LOGON_PLATFORM
{
	//操作结果
	DWORD                           dwResultCode;							//结果标识
	//描述信息
	TCHAR							szDescribeString[LEN_MESSAGE_DESCRIBE];	//描述消息
	//用户标识
	DWORD							dwUserID;							//用户标识
	//基本属性
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	BYTE							cbGender;							//用户性别
	TCHAR							szHeadUrl[LEN_HEAD_URL];			//头像地址
	TCHAR							szMySignature[LEN_MY_SIGNATURE];	//个性签名
	//荣誉属性
	SCORE                           dwUserDiamond;						//用户元宝
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力
	//用户权限
	BYTE                            cbMasterOrder;                      //管理员等级 0表示非管理员
	BYTE							cbMemberOrder;						//会员等级   0表示非管理员
	//SYSTEMTIME						MemberOverDate;						//会员到期时间
	//联系方式
	TCHAR                           szIdentityName[LEN_IDENTITY_NAME];  //真实姓名
	TCHAR                           szIdentityNum[LEN_IDENTITY_NUM];    //身份证号
	TCHAR                           szMobilePhone[LEN_MOBILE_PHONE];    //手机号码	
	//账号信息
	TCHAR							szLasLogonIp[LEN_IP_ADDR];			//最后登录地址
	//SYSTEMTIME						LasLogonDate;					//最后上线时间
	//牌友圈 
	DWORD							dwGroupID;							//社团标识
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字
	//财富信息
	SCORE							lUserScore;							//用户游戏币
	SCORE							lUserGold;							//用户金币
	SCORE							lOpenRoomCard;						//用户房卡

	//额外信息. 
	DWORD							dwOffLineGameID;					//断线重连的GameID, 非空表示是断线重连

	//TODO 增加代理ID
};

//重复登录
struct STR_CMD_LC_LOGON_REPEAT_LOGON
{
	TCHAR szDescribe[LEN_MESSAGE_DESCRIBE];                                              //提示信息
};

//升级提示
struct STR_CMD_LC_LOGON_UPDATE_NOTIFY
{
	BYTE							cbUpdateNotify;						//升级提示		0版本皮喷	1-建议升级		2-强制升级
	DWORD							dwCurrentServerVersion;				//当前服务器版本
};

//登录奖励
struct STR_CMD_LC_LOGON_LOGON_REWARD
{
	BYTE	byRewardType;				//奖励类型
	DWORD	dwRewardCount;				//奖励数量
	TCHAR	szDescribe[LEN_MESSAGE_DESCRIBE];			//描述
};

/* *********************************************************************************
**         MAIN:2    MDM_LIST    列表命令（登录成功后主动发送给客户端，客户端无需请求）	TODO 放到后面修改
** *********************************************************************************/
#define MDM_LIST					2									//列表信息

//列表信息返回
#define CMD_LC_LIST_TYPE			101									//类型列表TypeItem
#define CMD_LC_LIST_KIND			102									//种类列表KindItem
#define CMD_LC_LIST_NODE			103									//节点列表NodeItem
#define CMD_LC_LIST_ROOM			104									//房间列表RoomItem

//////////////////////////////////////////////////////////////////////////////////
//【结构体】
//获取在线
struct CMD_GP_GetOnline
{
	WORD							wServerCount;						//房间数目
	DWORD							wOnLineServerID[MAX_SERVER];		//房间标识
};

//类型在线
struct CMD_GP_KindOnline
{
	WORD							wKindCount;							//类型数目
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//类型在线
};

//房间在线
struct CMD_GP_ServerOnline
{
	WORD							wServerCount;						//房间数目
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//房间在线
};


/* *********************************************************************************
**      MAIN:3    MDM_SERVICE    服务		//基本包括所有客户端大厅点击按钮消息
**		说明：1、C(客户端)->L(登陆服)的消息号范围：	1-100
**			  2、L(客户端)->C(登陆服)的消息号范围：	101-200
**			  3、特殊消息号范围：	201-300（服务器主动发送）
** *********************************************************************************/
//【主消息号】
#define MDM_SERVICE								3							//服务

//【子消息号】
#define SUB_CL_SERVICE_USER_FEEDBACK			1							//玩家反馈
#define CMD_LC_SERVICE_USER_FEEDBACK			101							//玩家反馈返回

#define SUB_CL_SERVICE_REFRESH_USER_INFO		2							//刷新用户信息
#define CMD_CL_SERVICE_REFRESH_USER_INFO		102							//刷新用户信息返回

#define SUB_CL_SERVICE_QUERY_ROOM_LIST			3							//查询开房信息列表
#define CMD_LC_SERVICE_QUERY_ROOM_LIST			103							//开房信息返回

#define SUB_CL_SERVICE_MODIFY_PERSONAL_INFO		4							//修改个人资料（所有用户能修改的资料都在这里处理）
#define CMD_LC_SERVICE_MODIFY_PERSONAL_INFO		104							//修改个人资料返回

#define SUB_CL_SERVICE_GET_RICH_LIST			5							//获取富豪榜
#define CMD_LC_SERVICE_GET_RICH_LIST			105							//富豪榜返回

#define SUB_CL_SERVICE_GET_DJ_RECORD_LIST		6							//获取大局录像列表
#define CMD_LC_SERVICE_GET_DJ_RECORD_LIST		106							//获取大局录像列表返回

#define SUB_CL_SERVICE_GET_XJ_RECORD_LIST		7							//获取小局录像列表
#define CMD_LC_SERVICE_GET_XJ_RECORD_LIST		107							//获取小局录像列表返回

#define SUB_CL_SERVICE_ONLINE_REWARD			8							//获取在线奖励（节日奖励或点击领取奖励）
#define CMD_LC_SERVICE_ONLINE_REWARD			108							//获取在线奖励返回

#define SUB_CL_SERVICE_GET_TASK_LIST			9							//获取任务列表			//TODO 任务状态的更新逻辑还没修改
#define CMD_LC_SERVICE_GET_TASK_LIST			109							//获取任务列表返回

#define SUB_CL_SERVICE_GET_TASK_REWARD			10							//领取任务奖励
#define CMD_LC_SERVICE_GET_TASK_REWARD			110							//领取任务奖励返回

#define SUB_CL_SERVICE_REQUEST_LOTTERY			11							//请求抽奖
#define CMD_LC_SERVICE_REQUEST_LOTTERY			111							//请求抽奖返回

#define SUB_CL_SERVICE_GET_RANK_LIST			12							//获取指定排行榜			TOOD 真正的数据是从子游戏获取的，小局结算，但只有今日的数据，没有更新到昨日之类的
#define CMD_LC_SERVICE_GET_RANK_LIST			112							//获取指定排行榜返回

#define SUB_CL_SERVICE_GET_RANK_REWARD			13							//领取排行榜奖励
#define CMD_LC_SERVICE_GET_RANK_REWARD			113							//领取排行榜奖励返回

#define CMD_LC_USER_QUERY_SCORE_INFO			114							//查询（修改）用户金币房卡钻石信息返回(涉及到金币房卡改变的都会用到这个消息)

//////////////////////////////////////////////////////////////////////////////////
//【结构体定义】
//玩家反馈
struct STR_SUB_CL_SERVICE_FEEDBACK
{
	DWORD							dwUserID;								//用户ID
	TCHAR							szFB_Title[LEN_FEEDBACK_TITLE];			//反馈标题
	TCHAR							szFB_Content[LEN_FEEDBACK_CONTENT];		//反馈内容
	TCHAR							szContact[LEN_MOBILE_PHONE];			//电话号码
};
//玩家反馈返回
struct STR_CMD_LC_SERVICE_FEEDBACK
{
	LONG							lResultCode;								//错误代码
	TCHAR							szDescribeString[LEN_MESSAGE_DESCRIBE];		//消息描述
};

//刷新用户信息
struct STR_SUB_CL_SERVICE_REFRESH_INFO
{
	DWORD							dwUserID;							//用户ID
};
//刷新用户信息返回
struct STR_CMD_LC_SERVICE_REFRESH_INFO
{
	DWORD                           dwResultCode;                       //结果标识
	TCHAR							szDescribeString[128];				//描述消息
	DWORD							dwUserID;							//用户标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	BYTE							cbGender;							//用户性别
	WORD							wFaceID;							//头像索引
	TCHAR							szMySignature[LEN_MY_SIGNATURE];	//个性签名
	SCORE                           dwUserDiamond;						//用户元宝
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力
	BYTE                            cbMasterOrder;                      //管理员等级 0表示非管理员
	BYTE							cbMemberOrder;						//会员等级   0表示非管理员
	//SYSTEMTIME					MemberOverDate;						//会员到期时间
	TCHAR                           szIdentityName[LEN_IDENTITY_NAME];  //真实姓名
	TCHAR                           szIdentityNum[LEN_IDENTITY_NUM];    //身份证号
	TCHAR                           szMobilePhone[LEN_MOBILE_PHONE];    //手机号码
	TCHAR							szLasLogonIp[LEN_IP_ADDR];			//最后登录地址
	//SYSTEMTIME						LasLogonDate;						//最后上线时间		TODO 客户端没有最后上线时间，校验是不是等于数据包大小
	DWORD							dwGroupID;							//社团标识
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字
	SCORE							lUserScore;							//用户积分
	SCORE							lUserGold;							//用户金币
	SCORE							lUserRoomCard;						//用户房卡
};

//开房信息列表
struct STR_SUB_CL_SERVICE_QUERY_ROOMLIST
{
	DWORD dwUserID;
};
//开房信息列表返回
struct STR_CMD_LC_SERVICE_QUERY_ROOMLIST
{
	DWORD dwTableID;			//桌子ID
	DWORD dwPassword;			//桌子密码
	DWORD dwCreaterID;			//创建玩家ID
	WORD  wJuShu;				//局数
	BYTE  byMode;				//模式
	BYTE  byZhuangType;			//庄类型
	BYTE  byPlayerCount;		//玩家数
	BYTE  byMaxPlayerCount;		//最大玩家数
	BYTE  byIsStart;			//是否开始
	BYTE  byCost;				//扣卡数
	TCHAR szTime[24];			//创建时间
	DWORD dwUserID[5];			//玩家ID
	TCHAR szNickName[5][LEN_NICKNAME];		//玩家昵称
	DWORD dwScore[5];					//用户积分
};

//修改个人资料	
struct STR_SUB_CL_SERVICE_MODIFY_PERSONAL_INFO
{
	//用户标识
	DWORD							dwUserID;							//用户ID
	TCHAR							szOldLogonPassword[LEN_PASSWORD];	//旧的登陆密码
	//基本信息
	TCHAR							szNewLogonPassword[LEN_PASSWORD];	//新的登陆密码（若无修改，和旧的一样）
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	BYTE							cbGender;							//用户性别	
	TCHAR							szHeadImageUrl[LEN_HEAD_URL];			//头像地址
	TCHAR							szSignature[LEN_MY_SIGNATURE];		//个性签名
	//联系方式
	TCHAR							szRealName[LEN_IDENTITY_NAME];		//真实姓名
	TCHAR							szIDCardNum[LEN_IDENTITY_NUM];		//身份证号
	TCHAR							szPhoneNum[LEN_MOBILE_PHONE];		//绑定手机号
	//附加属性
	DWORD							dwProxyUserID;						//代理用户ID		//TODO 代理ID在数据库中暂时未增加，后面增加
};
//修改个人资料返回
struct STR_CMD_LC_SERVICE_MODIFY_PERSONL_INFO
{
	LONG							lResultCode;						//操作代码
	TCHAR							szDescribeString[128];				//成功消息
};

//获取富豪榜
struct STR_SUB_CL_SERVICE_GET_RICHLIST
{

};
//单个富豪榜结构体
struct tagSingleRichInfo
{
	TCHAR szName[LEN_NICKNAME];				//用户名
	DWORD dwMoney;							//金钱
	TCHAR szWechatAccount[LEN_ACCOUNTS];	//微信账号
	DWORD dwRegistID;						//认证ID
	TCHAR szQQ[LEN_ACCOUNTS];				//QQ号
	TCHAR szMobilePhone[LEN_MOBILE_PHONE];	//手机号
};
//获取富豪榜返回
struct STR_CMD_LC_SERVICE_GET_RICHLIST
{
	BYTE byCount;						//排行榜数量
	tagSingleRichInfo RegalInfoList[15];	//排行列表
};

//获取用户录像列表
struct STR_SUB_CL_SERVICE_GET_DJ_RECORDLIST
{
	DWORD dwUserID;
};
//获取用户录像列表返回
struct STR_CMD_LC_SERVICE_GET_DJ_RECORDLIST
{
	DWORD							dwTableID;			//桌子ID
	DWORD							dwUserID[5];		//玩家ID
	TCHAR							szUserName[5][LEN_NICKNAME];//用户昵称
	BYTE							byGameMode;			//游戏模式
	BYTE							byZhuangType;		//坐庄类型
	WORD							wCurrentJuShu;		//当前局数
	WORD							wTotalJuShu;		//总局数
	TCHAR							szGameTime[30];		//游戏时间
	TCHAR							szTotalScore[50];	//玩家大局总分数
};

//获取小局录像列表
struct STR_SUB_CL_SERVICE_GET_XJ_RECORDLIST
{
	DWORD dwTableID;												//桌子ID
};
//获取小局录像列表返回
struct STR_CMD_LC_SERVICE_GET_XJ_RECORDLIST
{
	WORD							wCurrentJuShu;					//当前局数
	TCHAR							szNickName[5][LEN_NICKNAME];	//用户昵称
	TCHAR							szScore[50];					//玩家小局分数

	//TODO 后面再添加具体的数据
};

//获取在线奖励
struct STR_SUB_CL_SERVICE_ONLINE_REWARD   
{
	DWORD dwUserID;
};
//获取在线奖励返回
struct STR_CMD_LC_SERVICE_ONLINE_REWARD
{
	BYTE  byType;		//奖励类型
	DWORD dwCount;		//奖励数量
};

//获取任务列表
struct STR_SUB_CL_SERVICE_GET_TASKLIST
{
	DWORD	dwUserID;				//用户ID
};
//任务信息
struct tagTaskInfo
{
	WORD	wTaskID;							//任务ID
	BYTE	cbTaskType;							//任务类型 1日常; 2周常; 3月常; 4成就
	BYTE	cbActionType;						//任务动作类型 ，1绑定信息; 2微信分享; 3充值; 4房卡游戏; 5金币游戏	
	DWORD	dwNeedCount;						//任务完成所需的数量, 比如完成5局房卡游戏, 这里就是5	
	TCHAR	szDescribe[128];					//任务描述
	BYTE	byRewardType;						//奖励类型1金币; 2房卡; 3钻石
	DWORD	dwRewardCount;						//奖励数量
	BYTE	cbTaskStatus;						//任务状态 ，1-进行中  1-完成但未领取 2-完成已领取
	DWORD	dwFinishedCount;					//已完成的任务所需数量
};
//获取任务列表返回
struct STR_CMD_LC_SERVICE_GET_TASKLIST
{
	WORD	wTaskID;							//任务ID
	BYTE	cbTaskType;							//任务类型 1日常; 2周常; 3月常; 4成就
	BYTE	cbActionType;						//任务动作类型 ，1绑定信息; 2微信分享; 3充值; 4房卡游戏; 5金币游戏	
	DWORD	dwNeedCount;						//任务完成所需的数量, 比如完成5局房卡游戏, 这里就是5	
	TCHAR	szDescribe[128];					//任务描述
	BYTE	byRewardType;						//奖励类型1金币; 2房卡; 3钻石
	DWORD	dwRewardCount;						//奖励数量
	BYTE	cbTaskStatus;						//任务状态 ，0-进行中  1-完成但未领取 2-完成已领取
	DWORD	dwFinishedCount;					//已完成的任务数量
	//附加信息
	BYTE	cbListFinishMask;					//任务列表发送完成标志 0-未完成 1-完成
};

//领取任务奖励
struct STR_SUB_CL_SERVICE_GET_TASK_REWARD
{
	DWORD	dwUserID;
	WORD	wTaskID;	
};
//领取任务奖励返回
struct STR_CMD_LC_SERVICE_GET_TASK_REWARD				
{
	LONG	lResultCode;												//0-任务完成  其他-任务未完成
	TCHAR	szDescribe[128];											//完成描述，例如 任完成,获得10000金币
};

//获取排行榜列表
struct STR_SUB_CL_SERVICE_GET_RANKLIST
{
	BYTE	byIndex;  //榜单序号,0今日消耗房卡 1今日胜场 2昨日消耗 3昨日胜场 4本周消耗 5本周胜场 6本月消耗 7本月胜场
};
//排行榜信息
struct tagRankInfo
{
	BYTE	byIndex;				//榜单序号
	DWORD	dwUserID;				//用户ID
	DWORD	dwCount;				//消费或胜利数量
	BYTE	byRankNum;				//名次
	BYTE	byReceived;				//已领取
	TCHAR	szNickName[LEN_NICKNAME];//昵称
	TCHAR	szHeadUrl[LEN_HEAD_URL];	//头像
};
//获取排行榜列表返回
struct STR_CMD_LC_SERVICE_GET_RANKLIST
{
	tagRankInfo rankItemInfo;
};

//请求抽奖
struct STR_SUB_CL_SERVICE_REQUEST_LOTTERY
{
	DWORD	dwUserID;				//用户ID
};
//请求抽奖返回
struct STR_CMD_LC_SERVICE_REQUEST_LOTTERY
{
	LONG							lResultCode;						//操作代码
	TCHAR							szDescribeString[128];				//描述消息
	BYTE							byIndex;							//摇奖序号（转盘的分区个数）：0-11	
};

//领取排行榜奖励
struct STR_SUB_CL_SERVICE_GET_RANK_REWARD
{
	BYTE	byIndex;
	DWORD	dwUserID;
};
//领取排行榜奖励返回   
struct STR_CMD_LC_SERVICE_GET_RANK_REWARD				
{
	LONG	lResultCode;												//0-领取成功  其他失败
	TCHAR	szDescribe[128];											//完成描述，例如 领取成功,获得10000金币
};

//查询用户金币房卡信息		TODO 在录像列表时用到，貌似错误，	可以修改为客户端大厅的刷新按钮
struct CMD_GP_QUERY_ScoreInfo
{
	DWORD							dwUserID;							//用户ID
};
//返回用户金币房卡信息
struct STR_CMD_LC_QUERY_SCORE_INFO
{
	LONG							lResultCode;						//操作代码
	TCHAR							szDescribeString[128];				//描述消息
	DWORD							dwUserID;							//用户ID
	LONGLONG						lOpenRoomCard;						//房卡
	LONGLONG						lGold;								//金币
	LONGLONG						lDiamond;							//钻石
};


/* *******************************************************************************
**          MAIN:4      MDM_GROUP   牌友圈
**			说明：1、C(客户端)->L(登陆服)的消息号范围：	1-100
**				  2、L(客户端)->C(登陆服)的消息号范围：	101-200
**				  3、特殊消息号范围：	201-300
** ******************************************************************************/
#define MDM_GROUP						4									//牌友圈消息

#define SUB_CL_GROUP_CREATE_GROUP		1									//创建牌友圈
#define CMD_LC_GROUP_CREATE_GROUP		101									//创建牌友圈成功返回，同时返回了用户牌友圈列表

#define SUB_CL_GROUP_JOIN_GROUP			2									//申请加入牌友圈
#define CMD_LC_GROUP_JOIN_GROUP			102									//申请加入牌友圈返回

#define SUB_CL_GROUP_DISMISS_GROUP		3									//解散牌友圈
#define CMD_LC_GROUP_DISMISS_GROUP		103									//解散牌友圈返回

#define SUB_CL_GROUP_LEAVE_GROUP		4									//退出牌友圈		
#define CMD_LC_GROUP_LEAVE_GROUP		104									//退出牌友圈返回

#define SUB_CL_GROUP_SET_ROOMCARD		5									//设置牌友圈房卡（同时返回主消息好3用户金币房卡信息116和牌友圈列表信息105）
#define CMD_LC_GROUP_SET_ROOMCARD		105									//设置牌友圈房卡返回

#define SUB_CL_GROUP_GET_GROUP_LIST		6									//获取用户牌友圈列表(只有基本信息)
#define CMD_LC_GROUP_GET_GROUP_LIST		106									//获取用户牌友圈列表返回（返回多次该消息，每次返回一个牌友圈的基本信息）

#define SUB_CL_GROUP_GET_MEMBER_LIST	7									//获取指定牌友圈的成员列表
#define CMD_LC_GROUP_GET_MEMBER_LIST	107									//获取指定牌友圈的成员列表返回

#define SUB_CL_GROUP_ALLOW_USER_JOIN	8									//同意加入牌友圈（同时刷新牌友圈列表）
#define CMD_LC_GROUP_ALLOW_USER_JOIN	108									//同意加入牌友圈返回

#define SUB_CL_GROUP_REFUSE_USER_JOIN	9									//拒绝加入牌友圈
#define CMD_LC_GROUP_REFUSE_USER_JOIN	109									//拒绝加入牌友圈返回

#define SUB_CL_GROUP_GET_APPLY_LIST		10									//获取指定牌友圈 申请加入用户列表
#define CMD_LC_GROUP_GET_APPLY_LIST		110									//获取指定牌友圈 申请加入用户列表返回

#define SUB_CL_GROUP_SEND_MSG			11									//发送牌友圈消息（群聊）		TODO 待测试其他用户是否收得到消息
#define CMD_LC_GROUP_SEND_MSG			111									//发送牌友圈消息返回

#define SUB_CL_GROUP_SET_NOTICE			12									//设置牌友圈公告，返回获得牌友圈信息（所有成员）
//#define CMD_LC_GROUP_SET_NOTICE		112									//设置牌友圈公告返回

#define SUB_CL_GROUP_GET_GROUP_INFO		13									//获得指定牌友圈所有信息		TODO 待测试其他用户是否设置成功
#define CMD_LC_GROUP_GET_GROUP_INFO		113									//获得指定牌友圈所有信息返回

#define SUB_CL_GROUP_KICK_USER			14									//踢出成员
#define CMD_LC_GROUP_KICK_USER			114									//踢出成员返回

//////////////////////////////////////////////////////////////////////////////////

//创建牌友圈
struct STR_SUB_CL_GROUP_CREATE_GROUP
{
	TCHAR	szGroupName[LEN_GROUP_NAME];	//牌友圈名称
	DWORD	dwOwnerID;						//圈主ID
	BYTE	GameId;							//游戏ID，5-铁岭麻将   4-阿拉斗牛
};
//创建牌友圈返回
struct STR_CMD_LC_GROUP_CREATE_GROUP
{
	LONG	lResultCode;								//操作代码
	TCHAR	szDescribeString[LEN_MESSAGE_DESCRIBE];		//描述消息
	DWORD	dwGroupID;									//牌友圈ID
	TCHAR	szGroupName[LEN_GROUP_NAME];				//牌友圈名称
	WORD	wCurrentUserCount;							//当前用户数
	WORD	wMaxUserCount;								//最大用户数
};

//申请加入牌友圈
struct STR_SUB_CL_GROUP_JOIN_GROUP
{
	DWORD	dwGroupID;
	DWORD	dwUserID;
};
struct tagJoinGroup
{
	DWORD	dwUserID;	
	TCHAR	szUserName[LEN_NICKNAME];
	DWORD	dwGroupID;
	TCHAR	szHeadUrl[LEN_HEAD_URL];
};
//申请加入牌友圈返回
struct STR_CMD_LC_GROUP_JOIN_GROUP
{
	LONG			lResultCode;								//操作代码
	TCHAR			szDescribeString[LEN_MESSAGE_DESCRIBE];		//描述消息
	tagJoinGroup	applyUserInfo;								//申请人信息
};

//解散牌友圈
struct STR_SUB_CL_GROUP_DISMISS_GROUP
{
	DWORD	dwUserID;				//解散牌友圈的用户ID
	DWORD	dwGroupID;				//牌友圈ID
};
//解散牌友圈返回
struct STR_CMD_LC_GROUP_DISMISS_GROUP
{
	LONG						lResultCode;								//错误代码
	TCHAR						szDescribeString[LEN_MESSAGE_DESCRIBE];		//描述消息
	DWORD						dwGroupID;									//解散牌友圈ID
};

//退出牌友圈
struct STR_SUB_CL_GROUP_LEAVE_GROUP
{
	DWORD				dwUserID;							//用户ID
	DWORD				dwGroupID;							//牌友圈ID

};
//退出牌友圈返回
struct STR_CMD_LC_GROUP_LEAVE_GROUP
{
	LONG				lResultCode;								//错误代码
	TCHAR				szDescribeString[LEN_MESSAGE_DESCRIBE];		//描述消息
	DWORD				dwGroupID;									//牌友圈ID
};

//设置牌友圈房卡
struct STR_SUB_CL_GROUP_SET_ROOMCARD
{
	DWORD	dwGroupID;	//牌友圈ID
	DWORD	dwUserID;	//用户ID
	BYTE	cbPutIn;	//转入0  取回1
	DWORD	dwRoomCard;	//房卡数
};
//设置牌友圈房卡返回
struct STR_CMD_LC_GROUP_SET_ROOMCARD
{
	LONG					lResultCode;								//错误代码
	TCHAR					szDescribeString[LEN_MESSAGE_DESCRIBE];		//描述消息
	DWORD					dwGroupRoomCard;							//牌友圈房卡数
};

//获取用户牌友圈列表
struct STR_SUB_CL_GROUP_GET_GROUP_LIST
{
	DWORD dwUserID;							//用户ID
};
//获取用户牌友圈列表返回
struct STR_CMD_LC_GROUP_GET_GROUP_LIST
{
	DWORD dwGroupID;						//牌友圈ID
	TCHAR szGroupName[LEN_GROUP_NAME];		//牌友圈名称
	WORD  wCurrentUserCount;				//牌友圈当前人数
	WORD  wMaxUserCount;					//牌友圈最大人数
	DWORD dwOwnerID;						//圈主ID
	TCHAR szOwnerHeadUrl[LEN_HEAD_URL];		//圈主头像		
};

//获取指定牌友圈成员列表
struct STR_SUB_CL_GROUP_GET_MEMBER_LIST
{
	DWORD	dwGroupID;
};
//获取指定牌友圈成员列表返回
struct STR_CMD_LC_GROUP_GET_MEMBER_LIST
{
	DWORD dwUserID;							//成员ID
	DWORD dwGroupID;						//牌友圈ID
	TCHAR szNickName[LEN_NICKNAME];			//成员昵称
	TCHAR szHeadUrl[LEN_HEAD_URL];			//成员头像
};

//同意用户加入牌友圈
struct STR_SUB_CL_GROUP_ALLOW_USER_JOIN
{
	DWORD	dwJoinUserID;		//同意加入的用户ID
	DWORD	dwGroupID;			//牌友圈ID
};
//同意用户加入牌友圈返回
struct STR_CMD_LC_GROUP_ALLOW_USER_JOIN
{
	LONG							lResultCode;								//错误代码
	TCHAR							szDescribeString[LEN_MESSAGE_DESCRIBE];		//描述消息
	DWORD							dwJoinUserID;								//同意加入的用户ID
	DWORD							dwGroupID;									//牌友圈ID
};

//拒绝用户加入牌友圈
struct STR_SUB_CL_GROUP_REFUSE_USER_JOIN
{
	DWORD	dwRefuseUserID;								//拒绝加入的用户ID
	DWORD	dwGroupID;									//牌友圈ID
};
//拒绝用户加入牌友圈返回
struct STR_CMD_LC_GROUP_REFUSE_USER_JOIN
{
	TCHAR	szDescribeString[LEN_MESSAGE_DESCRIBE];		//描述消息
};

//获取指定牌友圈的申请加入用户列表
struct STR_SUB_CL_GROUP_GET_APPLY_LIST
{
	DWORD	dwUserID;			//操作用户ID
	DWORD	dwGroupID;			//牌友圈ID
};
//申请加入牌友圈返回
struct STR_CMD_LC_GROUP_GET_APPLY_LIST
{
	LONG			lResultCode;								//操作代码
	TCHAR			szDescribeString[LEN_MESSAGE_DESCRIBE];		//描述消息
	tagJoinGroup	applyUserInfo;								//申请人信息
};

//发送牌友圈消息
struct STR_SUB_CL_GROUP_SEND_MSG
{
	DWORD dwSendUserID;					//发送消息用户ID
	TCHAR szSendUserNick[LEN_NICKNAME];	//发送消息用户昵称
	DWORD dwGroupID;					//牌友圈ID
	BYTE  wMsgID;						//表示自定义消息
	TCHAR szMsg[LEN_GROUP_MESSAGE];		//自定义消息内容
};
//发送牌友圈消返回息
struct STR_CMD_LC_GROUP_SEND_MSG
{
	DWORD dwSendUserID;					//发送消息用户ID
	TCHAR szSendUserNick[LEN_NICKNAME];	//发送消息用户昵称
	DWORD dwGroupID;					//牌友圈ID
	BYTE  wMsgID;						//表示自定义消息
	TCHAR szMsg[LEN_GROUP_MESSAGE];		//自定义消息内容
};

//设置牌友圈公告
struct STR_SUB_CL_GROUP_SET_GROUP_NOTICE
{
	DWORD   dwUserID;					//用户ID
	DWORD	dwGroupID;					//牌友圈ID
	TCHAR	szNotice[LEN_GROUP_NOTICE];				//自公告内容
};
//设置牌友圈公告返回
struct STR_CMD_LC_GROUP_SET_GROUP_NOTICE
{
	LONG			lResultCode;								//错误代码
	TCHAR			szDescribeString[LEN_MESSAGE_DESCRIBE];		//描述消息
};

//获得指定牌友圈信息
struct STR_SUB_CL_GROUP_GET_GROUP_INFO
{
	DWORD   dwGroupID;						//牌友圈ID
};
//获得指定牌友圈信息返回
struct STR_CMD_LC_GROUP_GET_GROUP_INFO
{
	DWORD dwGroupID;						//牌友圈ID
	TCHAR szGroupName[LEN_GROUP_NAME];		//牌友圈名称
	DWORD dwOwnerID;						//圈主ID
	TCHAR szOwnerHeadUrl[LEN_HEAD_URL];		//圈主头像
	WORD  wCurrentUserCount;				//牌友圈当前人数
	WORD  wMaxUserCount;					//牌友圈最大人数
	DWORD dwRoomCard;						//牌友圈房卡
	TCHAR szNotice[LEN_GROUP_NOTICE];		//牌友圈公告
};

//踢出用户  
struct STR_SUB_CL_GROUP_KICK_USER
{
	DWORD dwOperateUserID;		//踢人的用户ID
	DWORD dwKickUserID;			//被踢的用户ID
	DWORD dwGroupID;			//牌友圈ID
};
//踢出用户返回
struct STR_CMD_LC_GROUP_KICK_USER
{
	LONG							lResultCode;								//错误代码
	TCHAR							szDescribeString[LEN_MESSAGE_DESCRIBE];		//描述消息
};

/* *******************************************************************************
**          MAIN:5      MDM_OTHERS   其他消息
** ******************************************************************************/
//【主消息号】
#define MDM_OTHERS								5							//其他消息

//【子消息号】
#define SUB_CL_OTHERS_GIVE_PROPS				1							//赠送道具  //TODO 返回的是登陆奖励311		TODO修改流程
#define CMD_LC_OTHERS_GIVE_PROPS				101							//赠送道具返回

#define SUB_CL_OTHERS_RECHARGE_INFO				2							//充值信息
#define CMD_LC_OTHERS_RECHARGE_INFO				102							//赠送道具返回

#define SUB_CL_OTHERS_EXCHANGE_INFO				3							//兑换信息（钱->道具）
#define CMD_LC_OTHERS_EXCHANGE_INFO				3							//兑换信息返回

//////////////////////////////////////////////////////////////////////////////////
//赠送道具
struct SUB_CL_OTHER_GIVE_PROPS  
{
	DWORD dwSrcUserID;   //扣除用户ID
	DWORD dwDstUserID;	//目标用户ID
	BYTE  byType;		//道具类型
	DWORD dwCount;		//赠送数量
};

//赠送道具返回的是在线奖励 TODO 修改返回流程

//充值信息
struct STR_SUB_CL_OTHER_RECHARGE_INFO
{
	DWORD								dwUserID;							//用户 I D
};
//充值信息返回
struct STR_CMD_LC_OTHER_RECHARGE_INFO
{
	SCORE								dwMinMoney;		//最少充值钱  
	UINT32								dwChangeScale;  //兑换比例
};

//兑换道具
struct STR_SUB_CL_OTHER_EXCHANGE_INFO
{
	DWORD								dwUserID;							//用户 I D
};
//兑换道具返回
struct STR_CMD_LC_OTHER_EXCHANGE_INFO
{
	SCORE								dwMinMoney;		///最少充值钱  
	UINT32								dwChangeScale;  ///兑换比例
	SCORE								dwWithdrawals;  ///待提现RMB
	SCORE								dwBankMoney;	///银行金币
};

//公用操作结果
struct STR_CMD_LC_OTHER_OPERATE_RESULT
{
	LONG							lResultCode;						//错误代码
};
//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif