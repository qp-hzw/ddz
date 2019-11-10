#ifndef STRUCT_HEAD_FILE
#define STRUCT_HEAD_FILE

#pragma pack(1)

#pragma region 游戏列表  -- 来源: 数据库
//TypeItem
struct tagGameType
{
	WORD							wTypeID;							//类型索引
	TCHAR							szTypeName[LEN_TYPE];				//种类名字
};

//KindItem
struct tagGameKind
{
	WORD							wKindID;							//类型索引
	WORD							wTypeID;							//类型索引
	TCHAR							szKindName[LEN_KIND];				//游戏名字
};

//NodeItem
struct tagGameNode
{
	WORD							wKindID;							//名称索引
	WORD							wNodeID;							//节点索引
	TCHAR							szNodeName[LEN_NODE];				//节点名称
};

//GameItem
struct tagGameGame
{
	DWORD							wGameID;							//模块标识
	TCHAR							szGameName[LEN_KIND];				//游戏名字
	TCHAR							szGameServerAddr[LEN_DB_ADDR];		//游戏服地址
	TCHAR							szDLLName[LEN_DLL];					//DLL名字

	//非数据库, 程序中使用
	//TODONOW 此字段是根据szDLLName来判断当前是否有dll. 如果不放在此处, 每次都需要使用函数来get一下
	DWORD							dwNativeVersion;					//校验Dll是否存在
};

//RoomItem
struct tagGameRoomItem
{
	//索引变量
	DWORD							dwServerID;							//房间标识
	TCHAR							szServerName[LEN_SERVER];			//房间名称

	WORD							wGameServerPort;					//游戏服端口

	//非数据库, 程序中使用
	//TODONOW DLLName此字段其实是存在Database中的GameItem中的 如果不放在此处, 程序中就无法加载dll
	//        dwNativeVersion原因是 不需要每次校验dll是否存在
	TCHAR							szDLLName[LEN_DLL];					//DLL名字
	DWORD							dwNativeVersion;					//校验Dll是否存在
	//GameItem表中读取
	TCHAR							szGameServerAddr[LEN_DB_ADDR];		//游戏服地址
};

#pragma endregion

#pragma region 游戏服, 协调服, 登录服, 客户端 统一数据
//游戏房间 -- 发给client的
struct tagGameServer
{
	DWORD							wServerID;							//房间索引
	TCHAR							szServerName[LEN_SERVER];			//房间名称
	
	TCHAR							szGameServerAddr[LEN_IP_ADDR];		//游戏服地址
	WORD							wGameServerPort;					//游戏服端口

	DWORD							dwSubGameVersion;					//子游戏版本信息

	DWORD							dwOnlineCount;						//在线人数统计

	BYTE							byMask;								//增加删除标志 0为增加 1位删除
};

#pragma endregion

//在线信息
struct tagOnLineInfoGame
{
	DWORD							dwGameID;							//类型标识
	DWORD							dwOnLineCount;						//在线人数
};

//////////////////////////////////////////////////////////////////////////////////
//用户信息

//桌子状态
struct tagTableStatus
{
	BYTE							cbTableLock;						//锁定标志
	BYTE							cbPlayStatus;						//游戏标志
};

//用户状态
struct tagUserStatus
{
	WORD							wTableID;							//桌子索引
	WORD							wChairID;							//椅子位置
	BYTE							cbUserStatus;						//用户状态
};

//用户属性
struct tagUserAttrib
{
	BYTE							cbCompanion;						//用户关系
};

//用户积分
struct tagUserScore
{
	//积分信息
	SCORE							lScore;								//用户分数
	SCORE							lGrade;								//用户成绩
	SCORE							lInsure;							//用户银行
	SCORE							lOpenRoomCard;						//用户房卡

	//输赢信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数

	//全局信息
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//用户经验
	LONG							lLoveLiness;						//用户魅力
};

//用户积分
struct tagMobileUserScore
{
	//积分信息
	SCORE							lScore;								//用户分数

	//输赢信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数

	//全局信息
	DWORD							dwExperience;						//用户经验
};

//道具使用
struct tagUsePropertyInfo
{
	WORD                            wPropertyCount;                     //道具数目
	WORD                            dwValidNum;						    //有效数字
	DWORD                           dwEffectTime;                       //生效时间
};

//用户道具
struct tagUserProperty
{
	WORD                            wPropertyUseMark;                   //道具标示
	tagUsePropertyInfo              PropertyInfo[MAX_PT_MARK];			//使用信息   
};

//道具包裹
struct tagPropertyPackage
{
	WORD                            wTrumpetCount;                     //小喇叭数
	WORD                            wTyphonCount;                      //大喇叭数
};

//用户信息
struct tagUserInfo
{
	//基本属性
	DWORD							dwUserID;							//用户 I D
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

	//用户状态
	WORD							wTableID;							//桌子索引
	WORD							wChairID;							//椅子索引
	BYTE							cbUserStatus;						//用户状态

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
};

//////////////////////////////////////////////////////////////////////////////////

//广场子项
struct tagGamePlaza
{
	WORD							wPlazaID;							//广场标识
	TCHAR							szServerAddr[32];					//服务地址
};

//级别子项
struct tagLevelItem
{
	LONG							lLevelScore;						//级别积分
	TCHAR							szLevelName[16];					//级别描述
};

//会员子项
struct tagMemberItem
{
	BYTE							cbMemberOrder;						//等级标识
	TCHAR							szMemberName[16];					//等级名字
};

//管理子项
struct tagMasterItem
{
	BYTE							cbMasterOrder;						//等级标识
	TCHAR							szMasterName[16];					//等级名字
};

//列表子项
struct tagColumnItem
{
	BYTE							cbColumnWidth;						//列表宽度
	BYTE							cbDataDescribe;						//字段类型
	TCHAR							szColumnName[16];					//列表名字
};

//地址信息
struct tagAddressInfo
{
	TCHAR							szAddress[32];						//服务地址
};

//授权信息
struct tagLicenseInfo
{
	TCHAR						szGuestName[30];						//客户名(不能有空格)
	TCHAR						szExpire[20];							//过期时间
	TCHAR						szCode[MAX_PATH];						//授权码
	TCHAR						szMac[20];								//MAC
	TCHAR						szSystemInstall[MAX_PATH];				//
	TCHAR						szTimes[20];							//
	TCHAR						szCPUID[MAX_PATH];							//
};

//房间配置
struct tagServerOptionInfo
{
	//挂接属性
	WORD							wKindID;							//挂接类型
	WORD							wNodeID;							//挂接节点
	WORD							wSortID;							//排列标识

	//房间属性
	TCHAR							szServerName[LEN_SERVER];			//房间名称
};
//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif