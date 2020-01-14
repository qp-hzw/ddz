#ifndef STR_CMD_LONGON_HEAD_FILE
#define STR_CMD_LONGON_HEAD_FILE

#pragma pack(1)

#pragma region 辅助结构体
////桌子上的玩家信息
struct tagClubTableUserInfo
{
	DWORD	dwUserID;									//玩家ID
	TCHAR	szUserName[LEN_NICKNAME];					//玩家名字
	BYTE	bySex;										//性别
	WORD	wLevel;										//等级
	TCHAR	szHeadUrl[LEN_HEAD_URL];							//头像地址

	BYTE	byClubRole;									//玩家职务 0 群主; 1管理; 2一般成员
	BYTE	byClubReputation;							//玩家在工会中的声望等级
};

//在线人数
struct STR_CMD_LC_LIST_ROOM_ONLINE
{
	DWORD	dwServerID;						//房间ID
	DWORD	dwOnlineCount;					//在线
};

#pragma endregion

#pragma region MDM_LOGON 登录模块
//帐号登录  
struct STR_SUB_CL_LOGON_ACCOUNTS
{
	DWORD							dwVersionCheck;						//版本校验（32位 = 代码版本(8位) + 内核版本(8位) + 平台版本(8位) + 子游戏版本(8位)）
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	DWORD							dwProxyID;							//代理ID
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
	DWORD							dwProxyID;							//代理ID
};
//平台登录  
struct STR_SUB_CL_LOGON_PLATFORM
{
	DWORD							dwVersionCheck;						//版本校验
	TCHAR							szOpenID[33];						//OpenID | tokenID
	TCHAR							szUnionID[33];						//UnionID
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	BYTE							cbGender;							//用户性别
	TCHAR							strHeadUrl[LEN_HEAD_URL];			//头像地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	DWORD							dwProxyID;							//代理ID
};

//账号|平台登陆 返回
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
	BYTE							byLevel;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力
	//用户权限
	BYTE                            cbMasterOrder;                      //管理员等级 0表示非管理员
	BYTE							cbMemberOrder;						//会员等级   0表示非管理员
	SYSTEMTIME						MemberOverDate;						//会员到期时间
	TCHAR                           szIdentityName[LEN_IDENTITY_NAME];  //真实姓名
	TCHAR                           szIdentityNum[LEN_IDENTITY_NUM];    //身份证号
	TCHAR                           szMobilePhone[LEN_MOBILE_PHONE];    //手机号码	
	//账号信息
	TCHAR							szLasLogonIp[LEN_IP_ADDR];			//最后登录地址
	SYSTEMTIME						LasLogonDate;						//最后上线时间
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
#pragma endregion

#pragma region MDM_LIST 列表命令
struct STR_SUB_CL_LIST_QUERY_GAMEID
{
	DWORD		kindID; //TODONOW 具体类型 再定
};

struct STR_CMD_LC_LIST_QUERY_GAMEID
{
	DWORD		dwGameID;	//可用的gameID
};
#pragma endregion

#pragma region MDM_SERVICE 用户命令

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
	SYSTEMTIME					MemberOverDate;						//会员到期时间
	TCHAR                           szIdentityName[LEN_IDENTITY_NAME];  //真实姓名
	TCHAR                           szIdentityNum[LEN_IDENTITY_NUM];    //身份证号
	TCHAR                           szMobilePhone[LEN_MOBILE_PHONE];    //手机号码
	TCHAR							szLasLogonIp[LEN_IP_ADDR];			//最后登录地址
	SYSTEMTIME						LasLogonDate;						//最后上线时间		TODO 客户端没有最后上线时间，校验是不是等于数据包大小
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

//pure大厅排行版 查询
struct STR_SUB_CL_SERVICE_PURE_STANDING_LIST
{
	DWORD	dwUserID;				//用户ID
};
//pure大厅排行版 返回
struct STR_CMD_LC_SERVICE_PURE_STANDING_LIST
{
	BYTE	byRanking;					//排名 只展示前200个人的
	DWORD	dwUserID;					//玩家id
	DWORD   dwLoveLiness;				//用户魅力
	TCHAR	szNickName[LEN_NICKNAME];	//用户昵称
	TCHAR	szHeadUrl[LEN_HEAD_URL];	//用户头像
};
//pure大厅排行版 结束
struct STR_CMD_LC_SERVICE_PURE_STANDING_FINISH
{
	BYTE	byMask;						//掩码
	DWORD	dwRanking;					//请求者的排名 只展示前200个人的
	DWORD   dwLoveLiness;				//用户魅力
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

//大局战绩 查询
struct STR_SUB_CL_SERVICE_PURE_RECORD_LIST
{
	DWORD						dwUserID;					//玩家ID
	DWORD						dwClubID;					//牌友圈(版本2)索引, 为0表示是大厅
	BYTE						byMask;						//1房卡场  2金币场
	SYSTEMTIME					tmQueryStartData;			//所要查询的开始日期(最小单位为天)
	//SYSTEMTIME					tmQueryEndData;				//所要查询的截止日期(最小单位为天)
};
//大局战绩 返回
struct STR_CMD_LC_SERVICE_PURE_RECORD_LIST
{
	BYTE						byMask;						//1房卡场  2金币场
	DWORD						dwDrawID;					//大局记录 索引
	TCHAR						szKindName[32];				//游戏名
	DWORD						dwTableID;					//桌子id
	TCHAR						szTime[128];				//开始时间 -- 结束时间  2018.4.16 14:00- 
	//SYSTEMTIME					szStartDataTim;				//开始时间(2018.4.16 12.00-13.55)
	//SYSTEMTIME					szEndDataTim;				//结束时间(2018.4.16 12.00-13.55)
};
//大局战绩 单个玩家信息
struct STR_CMD_LC_SERVICE_PURE_RECORD_LIST_PLAYERINFO
{
	DWORD					 dwDrawID;						//大局记录 索引
	DWORD					 dwUserID;						//玩家ID
	TCHAR					 szNickName[32];				//昵称
	TCHAR					 szHeadUrl[LEN_HEAD_URL];		//头像
	SCORE					 lScore;						//玩家得分
};
//大局战绩 结束
struct STR_CMD_LC_SERVICE_PURE_RECORD_LIST_FINIST
{
	BYTE					byMask;							//结束标志
};

//小局战绩 查询
struct STR_SUB_CL_SERVICE_PURE_XJ_RECORD_LIST
{
	DWORD					 dwDrawID;						//大局记录 索引
};
//小局战绩 返回
struct STR_CMD_LC_SERVICE_PURE_XJ_RECORD_LIST
{
	BYTE					bRoundCount;					//局数
	DWORD					dwRecordID;						//录像ID（唯一标识）
};
//小局战绩 单个玩家信息
struct STR_CMD_LC_SERVICE_PURE_XJ_RECORD_LIST_PLAYERINFO
{
	BYTE					 bRoundCount;					//大局记录 索引
	DWORD					 dwUserID;						//玩家ID
	TCHAR					 szNickName[32];				//昵称
	SCORE					 lScore;						//玩家得分
};
//小局战绩 结束
struct STR_CMD_LC_SERVICE_PURE_XJ_RECORD_LIST_FINISH
{
	BYTE					byMask;						   //结束标志
};

//小局录像回放
struct STR_SUB_CL_SERVICE_XJ_RECORD_PLAYBACK
{
	DWORD					dwRecordID;						//录像ID
};
//小局录像回放返回
struct STR_CMD_LC_SERVICE_XJ_RECORD_PLAYBACK
{
	BYTE					cbfinish;							//发送结束表示	0-未结束	1-结束
	BYTE					cbRecordData[LEN_MAX_RECORD_SIZE/4];	//录像数据
};

//客服提示消息
struct STR_SUB_CL_SERVICE_CUSTOMER_MESSEGE
{
	BYTE					cbMessegeFlag;						//消息标识	0-表示消息提示	>=1表示具体消息请求
};
//客服提示消息 返回
struct STR_CMD_LC_SERVICE_CUSTOMER_MESSEGE
{
	BYTE					cbMessegeFlag;						//消息标识
	TCHAR					szMessege[LEN_MAX_SERVICE_MEG_SIZE];//消息内容
};

//请求金币大厅信息
struct STR_SUB_CL_SERVICE_GOLD_INFO
{
	DWORD					dwUserID;					   //玩家ID
	DWORD					dwModID;					   //游戏模式
	DWORD					dwKindID;					   //游戏的KindID
};
//请求金币大厅信息 返回
struct STR_CMD_LC_SERVICE_GOLD_INFO
{
	BYTE					byGoldMod;						//1.初级   2.中级   3.高级   4.富豪
	DWORD					dwScore;						//底分
	DWORD					dwGold;							//入场金币
	DWORD					dwServiceGold;					//入场费用
};
//请求金币大厅信息 结束
struct STR_CMD_LC_SERVICE_GOLD_INFO_FINISH
{
	BYTE					byMask;						   //结束标志
};
 
#pragma endregion

#pragma region MDM_CLUB  牌友圈(版本2)
#pragma region 1. 牌友圈相关
//创建牌友圈
struct STR_SUB_CL_CLUB_CREATE_CLUB
{
	DWORD	dwUserID;									//玩家ID
	TCHAR	szClubName[LEN_NICKNAME];					//牌友圈名字
	DWORD	dwMajorKindID;								//主打游戏ID
	DWORD	dwMinorKindID[10];							//次要游戏 最多10个

	TCHAR	szNotice[128];								//牌友圈公告
	TCHAR	szMessag[128];								//牌友圈简介
};

//创建牌友圈 返回
struct STR_CMD_LC_CLUB_CREATE_CLUB
{
	LONG	lResultCode;								//0成功  其他失败
	TCHAR	szDescribe[128];							//失败原因

	DWORD	dwClubID;									//牌友圈ID
	BYTE	byClubLevel;								//牌友圈等级
	//后台可以配置 新创建的牌友圈房卡数量
	DWORD	dwFK;										//牌友圈中房卡数量
	//此时的简介和公告都是默认的
};

//申请加入牌友圈
struct STR_SUB_CL_CLUB_JOIN_CLUB
{
	DWORD	dwUserID;									//玩家ID
	DWORD	dwClubID;									//牌友圈ID
	TCHAR	szNote[128];								//加入说明
};

//申请加入牌友圈 返回
struct STR_CMD_LC_CLUB_JOIN_CLUB
{
	LONG	lResultCode;								//0-成功  其他失败
	TCHAR	szDescribe[128];							//失败原因
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
	DWORD	dwUserID;									//玩家ID
	DWORD	dwClubID;									//牌友圈ID
	TCHAR	szNote[128];								//解散说明
};

//解散牌友圈 返回
struct STR_CMD_LC_CLUB_DISS_CLUB
{
	LONG	lResultCode;								//0-成功  其他失败
	TCHAR	szDescribe[128];							//失败原因
};

//请求自身牌友圈列表
struct STR_SUB_CL_CLUB_ALL_CLUB_INFO_LIST
{
	DWORD	dwUserID;									//玩家ID
};

//请求自身牌友圈列表 返回
struct STR_CMD_LC_CLUB_ALL_CLUB_INFO_LIST
{
	DWORD	dwClubID;									//牌友圈ID
	TCHAR	szClubName[LEN_NICKNAME];					//牌友圈名字

	BYTE	byClubRole;									//玩家职务 0群主 1管理 2一般成员
};

//请求牌友圈列表 完成
struct STR_CMD_LC_CLUB_ALL_INFO_FINISH
{
	BYTE	byMask;										//完成标志 1
	DWORD	dwUserID;									//自身ID
};

//请求未满员非自身随机牌友圈(最大9个)
struct STR_SUB_CL_CLUB_RANDOM_CLUB_LIST
{
	DWORD	dwUserID;									//玩家ID
};

//请求未满员非自身随机牌友圈(最大9个) 返回
struct STR_CMD_LC_CLUB_RANDOM_CLUB_LIST
{
	DWORD	dwClubID;									//牌友圈ID
	TCHAR	szClubName[LEN_NICKNAME];					//牌友圈名字
	DWORD	dwMajorKindID;								//主打游戏ID
	DWORD	dwMinorKindID[10];							//次要游戏 最多10个
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

#pragma region 2. 牌友圈房间
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
	DWORD	dwDissolveTime;								//解散时长

	//金币房特有
	DWORD	dwAmount;									//最低额度
	DWORD	dwOwnerPercentage;							//群主提成

	//底注 && 最低身价
	BYTE	byMask;										//1 AA支付;  2大赢家支付
	DWORD	dwDizhu;									//底注
	DWORD	dwGold;										//最低身价
};

//查询特定牌友圈房间列表 完成
struct STR_CMD_LC_CLUB_ROOM_LIST_FINISH
{
	BYTE	byMask;										//完成标志 1
};

//申请加入房间
struct STR_SUB_CL_CLUB_JOIN_ROOM
{
	DWORD	dwUserID;									//玩家ID
	DWORD	dwRoomID;									//房间ID
};

//申请加入房间 返回
struct STR_CMD_LC_CLUB_JOIN_ROOM
{
	DWORD	dwPlayerNum;								//当前房间人数
	LONG	lResultCode;								//0-成功  其他失败
	TCHAR	szDescribe[128];							//失败原因
};

//该房间的tableList
struct STR_CMD_LC_CLUB_TABLE_LIST
{
	DWORD	dwClubRoomID;								//牌友圈房间ID
	DWORD	dwTableID;									//桌子ID
	DWORD	dwClubID;									//俱乐部ID
	
	BYTE	byMask;										//0-不是群主;  1-是群主 (该字段已舍弃)

	BYTE	byDel;										//mask 0无操作;  1删除
	DWORD	TableState;									//0桌子空闲 1桌子已经开局 2人数已满
	DWORD	LockState;									//0桌子需要密码  1桌子不需要密码
	DWORD	CurrentRound;								//当前局数

	DWORD	dwOwnerID;									//群主ID
};

//该房间的userList
struct STR_CMD_LC_CLUB_TABLE_USER_LIST
{
	BYTE	byDel;										//mask 0无操作； 1删除
	DWORD	dwClubRoomID;								//俱乐部房间ID
	DWORD	dwUserID;									//玩家ID
	TCHAR	szUserName[LEN_NICKNAME];					//玩家名字
	BYTE	bySex;										//性别
	WORD	wLevel;										//等级
	TCHAR	szHeadUrl[LEN_HEAD_URL];							//头像地址

	BYTE	byClubRole;									//玩家职务 0 群主; 1管理; 2一般成员
	BYTE	byClubReputation;							//玩家在工会中的声望等级

	DWORD	dwTableID;									//桌子ID
	BYTE	byChairID;									//椅子ID
};


//房间设置请求
struct STR_SUB_CL_CLUB_ROOM_SETTING
{
	DWORD	dwUserID;									//玩家ID
	DWORD	dwRoomID;									//房间ID
	BYTE	byGoldOrFK;									//0金币房; 1房卡房

	//金币房 && 房卡房 通用配置
	bool	bDissolve;									//是否允许解散
	DWORD	dwDissolveTime;								//解散时长

	//金币房特用
	BYTE	byMask;										//1 AA支付;  2大赢家支付

	DWORD	dwAmount;									//最低额度(大赢家收益高于这个 才开始抽水)
	DWORD	dwOwnerPercentage;							//群主提成

	//底注 && 最低身价
	DWORD	dwDizhu;									//底注
	DWORD	dwGold;										//最低身价

	TCHAR	szRoomName[16];								//房间名字
};

//房间设置请求 返回
struct STR_CMD_LC_CLUB_ROOM_SETTING
{
	LONG	lResultCode;								//0-成功  其他失败
	TCHAR	szDescribe[128];							//失败原因

	DWORD	dwRoomID;									//房间ID
	BYTE	byGoldOrFK;									//0金币房; 1房卡房

	//金币房 && 房卡房 通用配置
	BYTE	byDissolve;									//是否允许解散  0是不允许 其他是允许
	DWORD	dwDissolveTime;								//解散时长 （已舍弃 客户端不再使用）

	//金币房特用
	DWORD	dwAmount;									//最低额度
	DWORD	dwOwnerPercentage;							//群主提成

	//底注 && 最低身价
	BYTE	byMask;										//1 AA支付;  2大赢家支付
	DWORD	dwDizhu;									//底注
	DWORD	dwGold;										//最低身价

	TCHAR	szRoomName[16];								//房间名字
};

//请求房间设置
struct STR_SUB_CL_CLUB_ROOM_QUERY_SETTING
{			
	DWORD	dwUserID;									//玩家ID
	DWORD	dwRoomID;									//房间ID
};

//请求房间设置 返回
struct STR_CMD_LC_CLUB_ROOM_QUERY_SETTING
{
	LONG	lResultCode;								//0-成功  其他失败
	TCHAR	szDescribe[128];							//失败原因

	DWORD	dwRoomID;									//房间ID
	BYTE	byGoldOrFK;									//0金币房; 1房卡房

	//金币房 && 房卡房 通用配置
	BYTE	byDissolve;									//是否允许解散 0是不允许 其他是允许
	DWORD	dwDissolveTime;								//解散时长（已舍弃， 客户端不再使用）

	//金币房特用
	DWORD	dwAmount;									//最低额度
	DWORD	dwOwnerPercentage;							//群主提成

	//底注 && 最低身价
	BYTE	byMask;										//1 AA支付;  2大赢家支付
	DWORD	dwDizhu;									//底注
	DWORD	dwGold;										//最低身价

	TCHAR	szRoomName[16];								//房间名字
};

//玩家离开房间
struct STR_SUB_CL_CLUB_ROOM_USER_LEAVE
{
	DWORD	dwUserID;									//玩家ID
	DWORD	dwClubRoomID;								//俱乐部房间ID(离开之前所在的房间ID)
};

//解散房间请求
struct STR_SUB_CL_CLUB_ROOM_DISSOLVE
{
	DWORD	dwUserID;									//玩家ID
	DWORD	dwClubID;									//俱乐部ID
	DWORD	dwClubRoomID;								//房间ID
};

//解散房间请求 返回
struct STR_CMD_LC_CLUB_ROOM_DISSOLVE
{
	LONG	lResultCode;								//0-成功  其他失败
	TCHAR	szDescribe[128];							//失败原因
};

//解散桌子请求
struct STR_SUB_CL_CLUB_TABLE_DISSOLVE
{
	DWORD	dwUserID;									//玩家ID
	DWORD	dwClubID;									//俱乐部ID
	DWORD	dwClubRoomID;								//房间ID
	DWORD	dwTableID;									//桌子ID
};

//解散桌子请求 返回
struct STR_CMD_LC_CLUB_TABLE_DISSOLVE
{
	LONG	lResultCode;								//0-成功  其他失败
	TCHAR	szDescribe[128];							//失败原因

	//下面字段 客户端无需处理, 服务器为简化字段而设置
	BYTE	byMask;										//0-成功  其他失败
	DWORD	dwTableID;									//桌子ID
	DWORD	dwGameID;									//游戏ID
};
#pragma endregion

#pragma region 3.工会操作
//牌友圈公告 -- 内部成员可看
struct STR_SUB_CL_CLUB_NOTICE
{
	DWORD	dwUserID;									//玩家ID
	DWORD	dwClubID;									//牌友圈ID
	TCHAR	szNotice[128];								//牌友圈 公告
};

//牌友圈公告 返回
struct STR_CMD_LC_CLUB_NOTICE
{
	LONG	lResultCode;								//0成功  其他失败
	TCHAR	szDescribe[128];							//失败原因
};

//牌友圈公告 广播
struct STR_CMD_LC_CLUB_NOTICE_BDCAST
{
	DWORD	dwUserID;									//玩家ID
	TCHAR	szNotice[128];								//牌友圈 公告
};

//牌友圈简介
struct STR_SUB_CL_CLUB_MESSAGE
{
	DWORD	dwUserID;									//玩家ID
	DWORD	dwClubID;									//玩家ID
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
	DWORD	dwUserID;									//玩家ID
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
	DWORD	dwUserID;									//玩家ID
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

//牌友圈置顶
struct STR_SUB_CL_CLUB_STICKY_POST
{
	DWORD	dwUserID;									//玩家ID
	DWORD	dwClubID;									//俱乐部ID
	BYTE	cbTopFlag;									//是否置顶	1-置顶	0-取消置顶
};

//牌友圈置顶返回
struct STR_CMD_LC_CLUB_STICKY_POST
{
	LONG	lResultCode;								//0成功  其他失败
};


#pragma endregion

#pragma region 4. 成员操作
//请求成员数据
struct STR_SUB_CL_CLUB_MEMBER_MANAGER
{
	DWORD	dwUserID;									//玩家ID
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

//请求成员数据 完成
struct STR_CMD_LC_CLUB_MEMBER_MANAGER_FINISH
{
	BYTE	byMask;										//完成标志 1
};

 //同时返回的公会基本信息
struct STR_CMD_LC_CLUB_DATA
{
	TCHAR	szClubName[LEN_NICKNAME];					//群名称
	TCHAR	szClubOwnerName[LEN_NICKNAME];				//群主名称
	DWORD	dwClubID;									//群ID
	DWORD	dwClubOwner;								//群主ID
	DWORD	dwMajorKindID;								//主打游戏ID 
	DWORD	dwMinorKindID[10];							//次要游戏 最多10个
	byte	byClubLevel;								//牌友圈等级
	WORD	wMemberNum;									//当前人数
	TCHAR	szNotice[128];								//牌友圈公告
	TCHAR	szMessage[128];								//牌友圈简介
	DWORD	dwFK;										//牌友圈中房卡数量
	BYTE	byApplyNum;									//牌友圈中房卡数量
	BYTE	byAutoAgree;								//新的入群申请数
	BYTE	bySex;										//是否开启自动同意
	WORD	wLevel;										//人物等级
};

//邀请他人加入牌友圈
struct STR_SUB_CL_CLUB_INVITE
{
	DWORD	dwUserID;									//玩家ID
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

//被邀请人的回复
struct STR_SUB_CL_CLUB_INVITE_RESULT
{
	DWORD	dwUserID;									//玩家ID
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
	DWORD	dwUserID;									//玩家ID
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

//被邀请人查看自己的邀请列表
struct STR_SUB_CL_CLUB_INQUERY_LIST
{
	DWORD	dwUserID;									//玩家ID
};

//被邀请人查看自己的邀请列表 返回
struct STR_CMD_LC_CLUB_INQUERY_LIST
{
	DWORD	dwClubID;									//牌友圈ID
	TCHAR	szClubName[LEN_NICKNAME];					//申请玩家昵称
	DWORD	dwMajorKindID;								//主打游戏ID
	DWORD	dwMinorKindID[10];							//次打游戏ID
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
};

//退出牌友圈 
struct STR_SUB_CL_CLUB_QUIT
{
	DWORD	dwClubID;									//牌友圈ID
	DWORD	dwUserID;									//玩家ID
};

//退出牌友圈 返回
struct STR_CMD_LC_CLUB_QUIT
{
	LONG	lResultCode;								//0-购买成功  其他失败
	TCHAR	szDescribe[128];							//失败原因
};

//职务任免 请求
struct STR_SUB_CL_CLUB_APPOINTMENT
{
	DWORD	dwClubID;									//牌友圈ID
	DWORD	dwUserID;									//玩家ID
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

//房间状态
struct STR_CMD_LC_ROOM_STATE
{
	DWORD	dwClubID;									//牌友圈编号
	DWORD	dwStateType;
	DWORD	dwJoinOrLeave;
	DWORD	dwTableState;
	DWORD	dwRoomID;
	DWORD	dwUserID;
	DWORD	dwTableID;
	DWORD	dwChairID;

	DWORD	dwCustomID;
	TCHAR	szHeadUrl[LEN_HEAD_URL];
	TCHAR	szUserName[LEN_NICKNAME];
	TCHAR	szKindName[LEN_KIND];

	DWORD	byPlayerNum;

	BYTE	rule[128];
};
#pragma endregion

#pragma endregion

#pragma region MDM_SHOP 商城道具
//查询商城物品打折情况
struct STR_SUB_CL_SHOP_QUERY
{
	DWORD dwUserID;								//玩家ID
	BYTE byGoodsType;							//商城物品类型 1房卡 2金币 3金币礼物 4积分礼物
};

//查询商城返回
struct STR_CMD_LC_SHOP_QUERY_RESULT
{
	DWORD dwGoodsID;							//商品编号
	TCHAR szGoodsName[LEN_NICKNAME];			//商品名字
	BYTE  byExtraGiftType;						//额外赠送的财富类型1rmb 2金币 3房卡 4积分
	DWORD dwExtraGiftCount;						//额外赠送的财富数量
	BYTE  byMoneyType;							//购买需要的财富类型 1rmb 2金币 3房卡 4积分
	DWORD dwMoneyCount;							//购买需要的财富数量
	BYTE  byDiscount;							//折扣
	DWORD dwLoveLiness;							//增加的魅力值
	TCHAR szDescripe[128];						//商品描述
	TCHAR szUrl[256];							//商品url地址
};

//查询商城结束
struct STR_CMD_LC_SHOP_QUERY_FINISH
{
	BYTE byMask;								//结束标志 1			
};

//钻石购买道具
struct STR_SUB_CL_SHOP_DIAMOND
{
	DWORD dwShopper;							//购买人
	DWORD byGoodsType;							//商品类型
	DWORD dwGoodsID;							//商品编号
	DWORD dwGoodsNum;							//购买商品的数量

	BYTE  byMask;								//0为自己购买;  1为他人代付
	DWORD dwTargetID;							//赠送对象
};

//钻石购买道具 返回
struct STR_CMD_LC_SHOP_DIAMOND_RESULT
{
	LONG	lResultCode;						//0-购买成功  其他失败
	TCHAR	szDescribe[128];					//失败原因
};

//背包物品查询
struct STR_SUB_CL_BAG_QUERY
{
	DWORD dwUserID;				//玩家ID
};

//背包物品查询 返回
struct STR_CMD_LC_BAG_RESULT
{
	DWORD dwGoodsID;		//物品ID
	DWORD dwGoodsNum;		//物品数量
	BYTE  byGoodsType;		//物品类型
};

//背包物品查询 结束
struct STR_CMD_LC_BAG_FINISH
{
	DWORD dwLoveness;		//玩家魅力值
};

#pragma endregion

#pragma region MDM_GIFT 礼物道具
//赠送道具
struct STR_SUB_CL_GIFT_GIVE_PROPS  
{
	DWORD										dwSrcUserID;				//扣除用户ID
	DWORD										dwDstUserID;				//目标用户ID
	DWORD										dwPropsID;					//道具编号
	DWORD										dwCount;					//赠送数量
};

//赠送道具 返回
struct STR_CMD_LC_GIFT_GIVE_PROPS  
{
	DWORD										dwResultCode;				//结果标识
	TCHAR										szDescribeString[128];		//描述消息

	DWORD										dwPropsID;					//道具编号
	DWORD										dwCount;					//道具数量

	DWORD										dwLoveness;					//魅力值
};

//礼物赠送 -- 被赠送人的提醒
struct STR_CMD_LC_GIFT_GIVE_PROPS_SHOW
{
	DWORD							dwTargetID;							//玩家ID
	TCHAR							szDescribeString[128];				//描述消息
};
#pragma endregion

#pragma region MDM_GAME 游戏服相关
//创建房间之前, 先查询可用的GameID
struct STR_SUB_CL_GAME_QUERY_GAMEID
{
	DWORD	dwKindID;		//TODONOW 确认kindID的类型
};

//创建房间之前, 先查询可用的GameID
struct STR_CMD_LC_GAME_QUERY_GAMEID
{
	//操作结果
	DWORD                           dwResultCode;							//结果标识
	TCHAR							szDescribeString[LEN_MESSAGE_DESCRIBE];	//描述消息

	DWORD	dwGameID;				//可用的GameID;
	TCHAR							szIPAddr[LEN_IP_ADDR];					//ip地址
	DWORD							dwPort;									//ip端口

};
#pragma endregion

#endif