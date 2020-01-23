#ifndef STR_CMD_CORRESPOND_HEAD_FILE
#define STR_CMD_CORRESPOND_HEAD_FILE

#pragma pack(1)

#pragma region  MDM_REGISTER 注册模块
//游戏房间
struct tagServerItem
{
	DWORD							dwServerID;							//server id
	DWORD                           dwSocketID;                         //socket id
	BYTE                            byServerType;                       //1.lobbyServer;  2.GameServer
	TCHAR							szServerName[LEN_SERVER];			//
	
	TCHAR							szServerAddr[LEN_IP_ADDR];		    //服务器地址
	WORD							wServerPort;					    //服务器端口
};

//服务类型
enum enServerType
{
	NULL_TYPE = 1,				//无效服务
	GAME_TYPE = 2,				//游戏服
	LOBBY_TYPE = 3,				//登录服
};

//注册失败
struct STR_CPO_PGPL_REGISTER_FAILURE
{
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[128];				//错误消息
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

	TCHAR	szIPAddr[LEN_IP_ADDR];					//ip地址
	DWORD	dwPort;									//ip端口
};
#pragma endregion

#pragma region MDM_TRANSFER 用户
//断线重连
struct tagOfflineUser 
{
	DWORD dwUserID;
	DWORD dwServerID;	
	BYTE  byMask;		//1表示在list中增加用户；  2表示在list中删除用户
};

#pragma endregion

#endif