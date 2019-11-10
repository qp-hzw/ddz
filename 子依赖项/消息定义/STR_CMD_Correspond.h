#ifndef STR_CMD_CORRESPOND_HEAD_FILE
#define STR_CMD_CORRESPOND_HEAD_FILE

#pragma pack(1)

#pragma region  MDM_REGISTER 注册模块
//注册登录服
struct STR_CPR_LP_REGISTER_LOGON
{
	TCHAR							szServerAddr[32];					//登陆服务地址
};

//注册游戏服
struct STR_CPR_GP_REGISTER_GAME
{
	DWORD							dwServerID;							//房间索引
	TCHAR							szServerName[LEN_SERVER];			//房间名称

	TCHAR							szGameServerAddr[32];				//游戏服地址
	WORD							wGameServerPort;					//游戏服端口
	
	DWORD							dwSubGameVersion;					//子游戏版本
};

//注册失败
struct STR_CPO_PGPL_REGISTER_FAILURE
{
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[128];				//错误消息
};

#pragma endregion

#pragma region MDM_CPD_LIST 列表命令
//房间人数
struct STR_CPR_GP_LIST_GAME_ONLINE
{
	DWORD							dwOnLineCount;						//在线人数
};

//房间人数
struct STR_CPR_PL_LIST_GAME_ONLINE
{
	DWORD							wServerID;							//房间标识
	DWORD							dwOnLineCount;						//在线人数
};

//房间删除
struct STR_CPO_PL_LIST_REMOVE_GAME
{
	DWORD							dwServerID;							//房间标识
};
#pragma endregion

#pragma region MDM_TRANSFER 中转服务

//登录服通知协调服, 协调服通知游戏服 解散桌子
struct STR_CPR_LP_CLUB_TABLE_DISSOLVE
{
	DWORD	dwTableID;									//桌子ID
	DWORD	dwGameID;									//游戏ID
};

//登录服通知协调服, 协调服通知游戏服 解散桌子
struct STR_CPO_PG_CLUB_TABLE_DISSOLVE
{
	DWORD	dwTableID;									//桌子ID
	DWORD	dwGameID;									//游戏ID
};

//创建桌子 查询可用的GameID
struct STR_CPR_LP_CREATE_TABLE
{
	DWORD	dwKindID;									//TODONOW 确认kindID的类型

	DWORD	dwSocketID;									//客户端SocketID
};

//创建桌子 查询可用的GameID
struct STR_CPO_PL_CREATE_TABLE
{
	//操作结果
	DWORD                           dwResultCode;							//结果标识
	TCHAR							szDescribeString[LEN_MESSAGE_DESCRIBE];	//描述消息

	DWORD	dwGameID;				//可用的GameID;

	DWORD	dwSocketID;									//客户端SocketID
};
#pragma endregion

#pragma region  玩家状态

#pragma endregion 

#pragma region MDM_WEB 后台管理
//跑马灯消息
struct STR_CPR_WP_WEB_MARQUEE
{
	BYTE							byMask;								//标志
};

//聊天频道 -- 系统消息
struct STR_CPR_WP_WEB_SYSTEM_MESSAGE
{
	TCHAR							szMessage[50];						//聊天频道 -- 系统消息
};
#pragma endregion
#pragma pack()

#endif