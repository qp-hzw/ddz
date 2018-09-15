#ifndef CMD_CORRESPOND_HEAD_FILE
#define CMD_CORRESPOND_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//注册命令

#define MDM_CS_REGISTER				1									//服务注册

//服务注册
#define SUB_CS_C_REGISTER_PLAZA		100									//注册广场
#define SUB_CS_C_REGISTER_SERVER	101									//注册房间

#define SUB_CS_S_CHECK_LICENSE		300									//检查授权
#define SUB_CS_S_CHECK_LICENSE_RES	301									//授权返回


//注册结果
#define SUB_CS_S_REGISTER_FAILURE	200									//注册失败

//////////////////////////////////////////////////////////////////////////////////

//注册广场
struct CMD_CS_C_RegisterPlaza
{
	TCHAR							szServerAddr[32];					//登陆服务地址		//lee：只是用来做校验的，证明登陆服的存在
	TCHAR							szServerName[LEN_SERVER];			//登陆服务器名
};

//注册游戏
struct CMD_CS_C_RegisterServer
{
	WORD							wKindID;							//名称索引
	WORD							wNodeID;							//节点索引
	WORD							wSortID;							//排序索引
	DWORD							dwServerID;							//房间索引
	WORD							wGameServerPort;					//房间端口		//改为游戏服端口
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwFullCount;						//满员人数
	TCHAR							szGameServerAddr[32];				//服务地址		//改为游戏服地址
	TCHAR							szServerName[LEN_SERVER];			//房间名称

	//积分信息
	SCORE							lServerScore;						//单元积分
	SCORE							lMinServerScore;					//需要积分
};

//注册失败
struct CMD_CS_S_RegisterFailure
{
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[128];				//错误消息
};


//授权请求
struct CMD_CS_S_CheckLicense
{
	TCHAR							szApp[128];							//
	TCHAR							szToken[128];						//
	LONG							time;
};

//授权结果
struct CMD_CS_S_CheckLicense_Res
{
	TCHAR							szToken[128];						//
	WORD							result;
};

//////////////////////////////////////////////////////////////////////////////////
//服务信息

#define MDM_CS_SERVICE_INFO			2									//服务信息

//房间信息
#define SUB_CS_C_SERVER_ONLINE		1									//房间人数
#define SUB_CS_C_SERVER_MODIFY		2									//房间修改

//广场命令
#define SUB_CS_S_PLAZA_INFO			100									//广场信息
#define SUB_CS_S_PLAZA_INSERT		101									//广场列表
#define SUB_CS_S_PLAZA_REMOVE		103									//广场删除
#define SUB_CS_S_PLAZA_FINISH		104									//广场完成

//房间命令
#define SUB_CS_S_SERVER_INFO		110									//房间信息
#define SUB_CS_S_SERVER_ONLINE		111									//房间人数
#define SUB_CS_S_SERVER_INSERT		112									//房间列表
#define SUB_CS_S_SERVER_MODIFY		113									//房间修改
#define SUB_CS_S_SERVER_REMOVE		114									//房间删除
#define SUB_CS_S_SERVER_FINISH		115									//房间完成

//////////////////////////////////////////////////////////////////////////////////

//房间数据
struct CMD_CS_C_ServerModify
{
	WORD							wSortID;							//排序索引
	WORD							wKindID;							//类型索引
	WORD							wNodeID;							//节点索引
	WORD							wServerPort;						//房间端口
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwFullCount;						//满员人数
	TCHAR							szServerAddr[32];					//服务地址
	TCHAR							szServerName[LEN_SERVER];			//房间名称

	//积分信息
	SCORE							lServerScore;						//单元积分
	SCORE							lMinServerScore;					//需要积分
};

//房间人数
struct CMD_CS_C_ServerOnLine
{
	DWORD							dwOnLineCount;						//在线人数
};

//////////////////////////////////////////////////////////////////////////////////

//广场删除
struct CMD_CS_S_PlazaRemove
{
	DWORD							dwPlazaID;							//广场标识
};

//房间人数
struct CMD_CS_S_ServerOnLine
{
	DWORD							wServerID;							//房间标识
	DWORD							dwOnLineCount;						//在线人数
};

//房间修改
struct CMD_CS_S_ServerModify
{
	WORD							wSortID;							//排序索引
	WORD							wKindID;							//类型索引
	WORD							wNodeID;							//节点索引
	DWORD							wServerID;							//房间标识
	WORD							wServerPort;						//房间端口
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwFullCount;						//满员人数
	TCHAR							szServerAddr[32];					//服务地址
	TCHAR							szServerName[LEN_SERVER];			//房间名称

	//积分信息
	SCORE							lServerScore;						//单元积分
	SCORE							lMinServerScore;					//需要积分
};

//房间删除
struct CMD_CS_S_ServerRemove
{
	DWORD							wServerID;							//房间标识
};

//////////////////////////////////////////////////////////////////////////////////
//用户汇总

#define MDM_CS_USER_COLLECT			3									//用户汇总

//用户状态
#define SUB_CS_C_USER_ENTER			1									//用户进入
#define SUB_CS_C_USER_LEAVE			2									//用户离开
#define SUB_CS_C_USER_FINISH		3									//用户完成
#define SUB_CS_C_USER_OFFLINE		4									//用户断线  game->correspond
// TODONOW 登录服没有 主消息号3的处理, 因此这个消息号的主消息号为2 之后修改
#define SUB_CS_C_USER_OFFLINE_B		5									//用户断线  correspond -> (game && logon)

//用户状态
#define SUB_CS_S_COLLECT_REQUEST	100									//汇总请求

//////////////////////////////////////////////////////////////////////////////////

//用户进入
struct CMD_CS_C_UserEnter
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//辅助信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级

	//SCORE							lDiamond;							//钻石
	//SCORE							lGold;								//金币
};

//用户离开
struct CMD_CS_C_UserLeave
{
	DWORD							dwUserID;							//用户标识
};

struct STR_SUB_CS_C_USER_OFFLINE
{
	DWORD dwUserID;
	DWORD dwGameID;
};

//////////////////////////////////////////////////////////////////////////////////
//远程服务
#define MDM_CS_REMOTE_SERVICE		4									//远程服务

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//管理服务

#define MDM_CS_MANAGER_SERVICE		5									//管理服务

#define SUB_CS_C_SYSTEM_MESSAGE 	1									//系统消息
#define SUB_CS_C_PROPERTY_TRUMPET 	2									//喇叭消息
#define SUB_CS_C_MARQUEE_MESSAGE	3									//跑马灯消息yang
#define SUB_CS_C_PRIZE_POOL_DATA    4	                                //彩金数据
#define SUB_CS_C_PRIZE_POOL_REWARD 	5	                                //彩金中奖

#define SUB_CS_S_SYSTEM_MESSAGE 	100									//系统消息
#define SUB_CS_S_PROPERTY_TRUMPET  	200									//喇叭消息
#define SUB_CS_S_MARQUEE_MESSAGE	300									//跑马灯消息yang
#define SUB_CS_S_PRIZE_POOL_DATA    400	                                //彩金数据
#define SUB_CS_S_PRIZE_POOL_REWARD  500	                                //彩金中奖

//发送喇叭
struct CMD_CS_S_SendTrumpet
{
	WORD                           wPropertyIndex;                      //道具索引 
	DWORD                          dwSendUserID;                         //用户 I D
	DWORD                          TrumpetColor;                        //喇叭颜色
	TCHAR                          szSendNickName[32];				    //玩家昵称 
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //喇叭内容
};

//跑马灯消息yang
struct  CMD_CS_C_SendMarquee
{
	WORD							MsgType;										//消息类型
	WORD							MsgID;											//消息ID(选择预设)
	TCHAR							szMessage[LEN_USER_CHAT];						//消息

};

//消息类型yang
enum MarqueeMessageType
{
	MarType_SYS	=	 1	,	//系统
	MarType_Game		=	 2	,	//中大奖
};

#define  WinScore 2000000  //单局赢分
//消息ID
enum MarqueeMessageID
{
	SystemID = 1,				//系统
	WinID = 2,					//中大奖
	KillID = 3,					//击中大鱼
	PrizePoolID = 4,            //彩金跑马灯
};
//跑马灯消息yang
struct  CMD_CS_MarqueeMessage
{
	LONGLONG						StartTime;							//开始播放时间
	INT								MsgNumberID;                        //跑马灯消息ID
	WORD							MsgType;							//消息类型
	WORD							MsgID;								//消息ID(选择预设)
	WORD							MsgFlag;							//显示位置
	WORD							m_wMarqueeMsgPlayCount;				//消息播放次数
	WORD							m_wMarqueeMsgApartTime;				//消息播放间隔时间
	WORD							MsgPlayTime;						//消息播放时间
	TCHAR							szMessage[LEN_USER_CHAT];			//消息
};

//彩金数据配置
struct CMD_CS_C_PrizePoolData
{
	WORD                            wGameID;                             //游戏ID
	WORD                            wRoomID;                             //房间ID
	LONGLONG                        lAllPrizePool;                       //总彩金池
};
//彩金中奖
struct CMD_CS_C_PrizePoolReward
{
	WORD                            wGameID;                             //游戏ID
	WORD                            wRoomID;                             //房间ID
	WORD                            wTableID;                            //中奖玩家桌子ID
	WORD                            wChairID;                            //中奖玩家ID
	LONGLONG                        lRewardGold;                         //中奖金币
};

//房间彩金
struct CMD_CS_RoomPrizePool
{
	WORD                            wGameID;                             //游戏ID
	WORD                            wRoomID;                             //房间ID
	WORD                            wTableID;                            //桌子ID
	LONGLONG                        lPrizePool;                          //彩金池
	BYTE                            bIsGamePrize;                        //是否开总彩金
	BYTE                            bIsRoomPrize;                        //是否开房间彩金
	BYTE                            bIsSendMessage;                      //是否发送跑马灯
};
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//后台管理
#define MDM_CS_WEB_SERVICE	300											//管理服务

#define SUB_CS_WEB_CONTROL_INFO			26								//控制输赢
#define	SUB_CS_WEB_FROZEN_ACCOUNT		25								//账号冻结
#define	SUB_CS_WEB_UPORDOWNPOINT		27								//上下分
#define SUB_CS_WEB_GAME_LEVEL_CONTROL	28								//游戏难度控制
#define SUB_CS_WEB_CONTROL_ROOM_VALUE	29								//控制房间值
#define SUB_CS_WEB_MARQUEE_MANAGER		30								//系统发布跑马灯消息
#define	SUB_CS_WEB_ROOM_CONF_NOTIFY		31								//库存修改
#define SUB_CS_WEB_CANCLE_MARQUEE		32								//取消系统发布跑马灯

//控制输赢yang
struct CMD_CS_WEB_ControlInfo
{
	DWORD						dwUserID;		//用户ID
	SCORE						dwControlScore;	//控制分数
};
struct CMD_CS_WEB_ControlInfoEx
{
	DWORD						dwUserID;		//用户ID
	SCORE						dwControlScore;	//控制分数
	TCHAR						Token[32];		//安全密钥
};

//账号冻结yang
struct CMD_CS_WEB_FrozenAccount
{
	DWORD						dwUserID;		//用户ID
};
struct CMD_CS_WEB_FrozenAccountEx
{
	DWORD						dwUserID;		//用户ID
	TCHAR						Token[32];		//安全密钥
};

//上下分yang
struct CMD_CS_WEB_UpOrDownScore
{
	DWORD						dwUserID;		//用户ID
	SCORE						dwChangePoint;	//改变分数
};
struct CMD_CS_WEB_UpOrDownScoreEx
{
	DWORD						dwUserID;		//用户ID
	SCORE						dwChangePoint;	//改变分数
	TCHAR						Token[32];		//安全密钥

};

//控制游戏难易yang
struct CMD_CS_WEB_ControlGameLevel
{
	DWORD						dwRoomID;		//用户ID
	DWORD						dwControlLevel;	//难度
};
struct CMD_CS_WEB_ControlGameLevelEx
{
	DWORD						dwRoomID;		//用户ID
	DWORD						dwControlLevel;	//难度
	TCHAR						Token[32];		//安全密钥
};

//控制房间值yang
struct CMD_CS_WEB_ControlRoomValue
{
	DWORD						dwRoomID;		//房间ID
	SCORE						dwValue;
};
struct CMD_CS_WEB_ControlRoomValueEx
{
	DWORD						dwRoomID;		//房间ID
	SCORE						dwValue;
	TCHAR						Token[32];		//安全密钥
};

//系统发布跑马灯消息
struct CMD_CS_WEB_MarqueeManagerEx
{
	INT								MsgNumberID;                        //跑马灯消息ID
	WORD							MsgID;								//消息类型
	LONGLONG						StartTime;							//开始播放时间
	WORD							m_wMarqueeMsgPlayCount;				//消息播放次数
	WORD							m_wMarqueeMsgApartTime;				//消息播放间隔时间
	TCHAR							szMessage[LEN_USER_CHAT];			//消息
	TCHAR							Token[32];							//安全密钥
};

//库存修改yang
struct  CMD_CS_WEB_RoomConfNotify
{
	DWORD						dwGameID;
	DWORD						dwRoomID;
	DWORD						OperationType;							//消息类型
};
struct  CMD_CS_WEB_RoomConfNotifyEx
{
	DWORD						dwGameID;
	DWORD						dwRoomID;
	DWORD						OperationType;							//消息类型
	TCHAR						Token[32];		//安全密钥
};

//取消系统发布跑马灯
struct CMD_CS_WEB_CancleMarqueeMsg
{
	INT							dwSystemMsgID;						//每一条消息ID
};

struct CMD_CS_WEB_CancleMarqueeMsgEx
{
	INT							dwSystemMsgID;						//每一条消息ID
	TCHAR						Token[32];		//安全密钥
};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()


#endif