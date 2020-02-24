#ifndef STR_CMD_LONGON_HEAD_FILE
#define STR_CMD_LONGON_HEAD_FILE

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

//通用错误提示
struct STR_SUB_CL_COMMON_ERROR
{
	TCHAR szMsg[20];            //通用错误提示
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
	DWORD                           dwResultCode;							//结果标识
	TCHAR							szDescribeString[LEN_MESSAGE_DESCRIBE];	//描述消息

	tagUserInfo                     useInfo;                                //玩家信息 
	DWORD							dwOffLineGameID;					    //断线重连的GameID, 非空表示是断线重连
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

#pragma region MDM_SERVICE 用户命令
//关注, 粉丝 
struct STR_SUB_CL_SERVICE_FLOWER
{
	DWORD   dwTargetID;  //查看对象的useID
};

//关注, 粉丝 
struct STR_CMD_LC_SERVICE_FLOWER_P
{
	DWORD							dwUserID;							//用户ID
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	BYTE							cbGender;							//用户性别
	TCHAR							szHeadUrl[LEN_HEAD_URL];			//头像地址
	DWORD                           dwLoveValue;                        //爱心值
};
//关注, 粉丝 
struct STR_CMD_LC_SERVICE_FLOWER
{
	DWORD   dwFolwerNum;  //我的关注
	DWORD   dwFolwingNum; //我的粉丝
	BYTE    cbIsGuanzhu;  //1显示关注按钮; 其他不显示关注按钮

	STR_CMD_LC_SERVICE_FLOWER_P folwingInfo[5];
};

//关注, 取消关注
struct STR_SUB_CL_SERVICE_FLOWER_ACT
{
	DWORD   dwTargetID;  //查看对象的useID
	BYTE    cbMask;      //1关注, 2取消关注
};
//关注， 取消关注 返回
struct STR_CMD_LC_SERVICE_FLOWER_ACT
{
	BYTE    cbResult;    //0成功， 其他失败
	BYTE    cbMask;      //1关注, 2取消关注
};


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
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	BYTE							cbGender;							//用户性别	
	TCHAR							szHeadImageUrl[LEN_HEAD_URL];		//头像地址
	TCHAR							szSignature[LEN_MY_SIGNATURE];		//个性签名
	//联系方式
	TCHAR							szRealName[LEN_IDENTITY_NAME];		//真实姓名
	TCHAR							szIDCardNum[LEN_IDENTITY_NUM];		//身份证号
	TCHAR							szPhoneNum[LEN_MOBILE_PHONE];		//绑定手机号
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

#pragma region MDM_SHOP 商城道具
//商城购买
struct STR_SUB_CL_SHOP_BUY
{
	DWORD dwGoodsID;							//物品ID
	DWORD dwGoodsNum;                           //物品数量
	BYTE byTreasureType;						//财富类型
};

//商城购买返回
struct STR_CMD_LC_SHOP_BUY_RESULT
{
	BYTE  byResult;        //返回结果 0成功， 其他失败
};

//背包物品查询 返回
struct STR_CMD_LC_BAG_RESULT
{
	DWORD dwGoodsID;		//物品ID
	DWORD dwGoodsNum;		//物品数量
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