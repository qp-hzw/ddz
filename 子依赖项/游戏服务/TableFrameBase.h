#ifndef GAME_SERVICE_HEAD_HEAD_FILE
#define GAME_SERVICE_HEAD_HEAD_FILE

#define GAME_CONCLUDE_CONTINUE  0xFF //大局结束并续费
#define GAME_CONCLUDE_NORMAL    0xFE //正常结束

//游戏状态
#define GAME_STATUS_FREE			0									//空闲状态
#define GAME_STATUS_PLAY			100									//游戏状态	TODO 细看，改成桌子状态

//结束原因
#define GER_NORMAL					0x00								//常规结束
#define GER_DISMISS					0x01								//游戏解散
#define GER_USER_LEAVE				0x02								//用户离开
#define GER_NETWORK_ERROR			0x03								//网络错误


//通用房间规则
struct tagTableRule
{
	/********************************* 大厅使用 ************************************/
	BYTE	GameMode;				//游戏模式 0-房卡模式; 1-竞技模式;  2-金币模式;  3-房卡金币;
	BYTE	RobBankType;			//坐庄模式 0-抢庄（抢注高者坐庄） 1-轮庄（轮流坐庄）2-固定（固定房主坐庄）  3-赢庄（赢的人坐庄） 4-摇骰子定庄（随机庄家）
	BYTE	GameCount;				//游戏局数 0-无限局
	BYTE	GameCountType;			//大局类型 是第一大局, 第二大局, 还是第三大局
	BYTE	PlayerCount;			//玩家数量 0-任意人数可开
	BYTE	cbPayType;				//支付方式，0房主支付、1AA制
	DWORD	lSinglePayCost;			//金币场 入场门票  TODONOW 该字段仅在扣除门票的时候有用，其他的调用均是以前的, 可以删除
	WORD	CellScore;				//*底分	  默认为1 -- 子游戏使用的是该字段
	DWORD	MaxFan;					//*封顶番数 0-不封顶 
	DWORD	LeaveScore;				//*离场分数 0-无限制         -- 已舍弃
	DWORD	JoinScore;				//*入场设定 0-无限制         -- 已舍弃
	BYTE	bRefuseSameIP;			//允许同IP    0-不允许 1-允许
	BYTE	bDistanceIn300;			//允许300米	  0-不许云 1-允许
	double	dLongitude;				//经度	
	double	dLatitude;				//纬度	
	BYTE	bCreateToOther;			//是否为他人开房 0-否 1-是
	DWORD	FangZhu;				//*房主
	BYTE	bAllowStranger;			//允许陌生人加入

	/********************************** 牌友圈相关 ************************************/
	BYTE	byClubCreate;			//0大厅创建 1 牌友圈普通创建  2牌友圈私密创建
	DWORD	dwUserID;				//群主ID
    DWORD	dwClubID;				//牌友群/俱乐部编号
	DWORD	dwKindID;			    //游戏ID
	BYTE	byGoldOrFK;				//(2.金币 1.房卡)

	BYTE	bDissolve;				//是否允许解散 0允许 1不允许
	DWORD	dwDissolveTime;			//解散时长 (分钟)

	//金币房特用
	DWORD	dwAmount;				//最低额度
	DWORD	dwOwnerPercentage;		//群主提成
    
	BYTE	byMask;					//1 AA支付;  2大赢家支付
	DWORD	dwDizhu;				//底注
	//TODONOW 如果是在俱乐部的金币场,这里就是房主设置的; 
	//如果是在大厅的房卡金币场,这里就是系统设置的; 
	//如果是在大厅的金币场, 这里就是系统设置的
    DWORD	dwLevelGold;			//进场的最小身价 

	/************************************ 备用字段 ************************************/
	BYTE byRetain1;
    BYTE byRetain2;
    BYTE byRetain3;
	BYTE byRetain4;

	DWORD szRetain1;
    DWORD szRetain2;
	DWORD szRetain3;
    DWORD szRetain4;
	DWORD szRetain5;
    DWORD szRetain6;
	DWORD szRetain7;
    DWORD szRetain8;
    DWORD szRetain9;
    DWORD szRetain10;
};

/////////////////////////////////////////////////////////////////////
//测试通过用  

//用户接口
interface IServerUserItem
{
//	//属性信息
//public:
//	//用户索引
//	virtual WORD GetBindIndex() = NULL;
//	//用户地址
//	virtual DWORD GetClientAddr() = NULL;
//	//机器标识
//	virtual LPCTSTR GetMachineID() = NULL;
//	//设置断线标志 GameID  TODONOW
//	virtual void SetOfflineGameID(DWORD) = NULL;
//	//获取断线标志 GameID  TODONOW
//	virtual DWORD GetOfflineGameID() = NULL;
//
//	//登录信息
//public:
//	//请求标识
//	virtual DWORD GetDBQuestID() = NULL;
//	//登录时间
//	virtual DWORD GetLogonTime() = NULL;
//	//记录索引
//	virtual DWORD GetInoutIndex() = NULL;
//
//	//用户信息
//public:
//	//用户信息
//	virtual tagUserInfo * GetUserInfo() = NULL;
//	//道具信息
//	virtual tagUserProperty * GetUserProperty() = NULL;
//
//	//属性信息
//public:
//	//用户性别
//	virtual BYTE GetGender() = NULL;
//	//用户标识
//	virtual DWORD GetUserID() = NULL;
//	//用户昵称
//	virtual LPCTSTR GetNickName() = NULL;
//
//	//用户断线之前的状态
//	virtual void SetOldGameStatus(BYTE gamestatus) = NULL;
//
//	//用户断线之前的状态
//	virtual BYTE GetOldGameStatus() = NULL;
//
//	//状态接口
public:
//	//桌子号码
//	virtual WORD GetTableID() = NULL;
//	//椅子号码
	virtual WORD GetChairID() = NULL;
//	//用户状态
//	virtual BYTE GetUserStatus() = NULL;
//
//	//权限信息
//public:
//	//用户权限
//	virtual DWORD GetUserRight() = NULL;
//	//管理权限
//	virtual DWORD GetMasterRight() = NULL;
//
//	//权限信息
//public:
//	//用户权限
//	virtual BYTE GetMemberOrder() = NULL;
//	//管理权限
//	virtual BYTE GetMasterOrder() = NULL;
//
//	//积分信息
//public:
//	//用户积分
//	virtual SCORE GetUserScore() = NULL;
//	//用户成绩
//	virtual SCORE GetUserGrade() = NULL;
//
//	//获得用户房卡
//	virtual SCORE GetUserRoomCard() = NULL;
//	//设置用户房卡
//	virtual void SetUserRoomCard(SCORE lRoomCard) = NULL;
//
//	//获得用户钻石
//	virtual SCORE GetUserDiamond() = NULL;
//	//设置用户钻石
//	virtual void SetUserDiamond(SCORE lDiamod) = NULL;
//
//	//获得用户金币
//	virtual SCORE GetUserGold() = NULL;
//	//设置用户金币
//	virtual void SetUserGold(SCORE lGold) = NULL;
//
//	//用户控制值
//	virtual SCORE GetUserControlScore() = NULL;
//	virtual VOID  SetUserControlScore(SCORE val) = NULL;
//
//
//	//托管信息
//public:
//	//托管积分
//	virtual SCORE GetTrusteeScore() = NULL;
//	//锁定积分
//	virtual SCORE GetFrozenedScore() = NULL;
//
//	//积分信息
//public:
//	//用户胜率
//	virtual WORD GetUserWinRate() = NULL;
//	//用户输率
//	virtual WORD GetUserLostRate() = NULL;
//	//用户和率
//	virtual WORD GetUserDrawRate() = NULL;
//	//用户逃率
//	virtual WORD GetUserFleeRate() = NULL;
//	//游戏局数
//	virtual DWORD GetUserPlayCount() = NULL;
//
//	//效验接口
//public:
//	//对比帐号
//	virtual bool ContrastNickName(LPCTSTR pszNickName) = NULL;
//
//	//游戏状态
//public:
//	//连接状态
//	virtual bool IsClientReady() = NULL;
//	//设置连接
//	virtual VOID SetClientReady(bool bClientReady) = NULL;
//
//	//控制用户
//public:
//	//控制状态
//	virtual bool IsAndroidUser() = NULL;
//	//设置控制
//	virtual VOID SetAndroidUser(bool bbMachineUser) = NULL;
//
//public:
//	//记录接口
//public:
//	//变更判断
//	virtual bool IsVariation() = NULL;
//	//查询记录
//	//virtual bool QueryRecordInfo(tagVariationInfo & UserRecordInfo) = NULL;
//	//提取变更
//	//virtual bool DistillVariation(tagVariationInfo & UserVariationInfo) = NULL;
//
//	//管理接口
//public:
//	//设置状态
//	virtual bool SetUserStatus(BYTE cbUserStatus, WORD wTableID, WORD wChairID, bool bNotify = true) = NULL;
//	//修改权限
//	virtual VOID ModifyUserRight(DWORD dwAddRight, DWORD dwRemoveRight, bool bGameRight = true) = NULL;
//
//	//更新用户财富信息
//	virtual bool ModifyUserTreasure(DWORD dwTableID, BYTE byTableMode, BYTE byRound, SCORE lUserTreasuse, BYTE byWin) = NULL;
//
//	//高级接口
//public:
//	//解除绑定
//	virtual bool DetachBindStatus() = NULL;
//	//银行操作
//	virtual bool ModifyUserInsure(SCORE lScore, SCORE lInsure, SCORE lRevenue) = NULL;
//	//设置参数
//	virtual bool SetUserParameter(DWORD dwClientAddr, WORD wBindIndex, TCHAR szMachineID[LEN_MACHINE_ID], bool bAndroidUser,
//		bool bClientReady, const double &dLongitude, const double &dLatitude) = NULL;
//
//	//手机定义
//public:
//	//手机规则
//	virtual WORD GetMobileUserRule() = NULL;
//	//设置定义
//	virtual VOID SetMobileUserRule(WORD wMobileUserRule) = NULL;
//	//当前分页
//	virtual WORD GetMobileUserDeskPos() = NULL;
//	//当前分页
//	virtual VOID SetMobileUserDeskPos(WORD wMobileUserDeskPos) = NULL;
//	//分页桌数
//	virtual WORD GetMobileUserDeskCount() = NULL;
//	//分页桌数
//	virtual VOID SetMobileUserDeskCount(WORD wMobileUserDeskCount) = NULL;
//
//public:
//	//设置为虚拟用户
//	virtual VOID SetVirtualUser(bool bVirtual) = NULL;
//	//获取是否为虚拟用户
//	virtual bool IsVirtualUser() = NULL;

};
//玩家基础信息 DB use player base info
struct BASE_PLAYERINFO
{
	char					m_szAccID[54 + 1];
	int				m_PlayerID;
	char					m_szPlayerName[54 + 1];
	unsigned int		m_LV;
	unsigned int		m_dwExp;
	//unsigned int		m_goldCoin;
	int		m_goldCoin;
	unsigned int		m_diamond;
	unsigned int		m_vip;
	time_t				m_registerDate;
	char					m_szPassword[54 + 1];
	unsigned int		m_sex;
	char					m_szFaceUrl[54 + 1];
	time_t				m_receiveTime;
	unsigned int      m_RechTimes;
	unsigned int      m_gameNumber;
	unsigned int      m_winNumber;
	unsigned int      m_invitecode;
	char              m_szRegisterAddrip[54 + 1];
	unsigned int      m_VoucherNum;								//带金卷数量
	char              m_szLoginip[54 + 1];
	char              m_szRealName[54 + 1];   //实名
	int					m_PlayerModelID;							//玩家模型ID
	int					m_Permissions;								//权限
	int                 m_creditpoint;                              //比赛信用分
	//int          m_match_score;                                 //比赛积分
};
/////////////////////////////////////////////////////////////////////

//桌子接口
interface ITableFrame : public IUnknownEx
{
public:
	// 1金币; 2私有; 3比赛
	virtual int GameType() = 0;
	//is roboot
	virtual bool IsRobot(WORD wChairID) = 0;
	//get player base info
	virtual BASE_PLAYERINFO GetPlayerBaseInfo(WORD wChairID) = 0;

		//配置参数
public:
	//读取通用房间规则
	virtual VOID* GetCustomRule() = NULL;
	//读取子游戏特有房间规则
	virtual VOID * GetSubGameRule() = NULL;

	//状态接口
public:
	//获取状态
	virtual BYTE GetGameStatus() = NULL;
	//设置状态
	virtual VOID SetGameStatus(BYTE bGameStatus) = NULL;

	//控制接口
public:
	//开始游戏
	virtual bool StartGame() = NULL;

	//写分接口
public:
	//写入录像记录 参数 小局数,数据和长度
	virtual bool WriteRecordInfo(WORD wXJCount, TCHAR strScore[], VOID* pData, DWORD dwDataSize) = NULL;
	//处理小局结束
	virtual bool HandleXJGameEnd(BYTE cbCurGameCount, BYTE cbGameMode, SCORE *lGameScore) = NULL;
	//处理大局结束
	virtual bool HandleDJGameEnd(BYTE cbGameStatus) = NULL;

	//时间接口
public:
	//设置时间
	virtual bool SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter) = NULL;
	//删除时间
	virtual bool KillGameTimer(DWORD dwTimerID) = NULL;
	//获取定时器剩余毫秒数
	virtual DWORD GetTimerLeftTickCount(DWORD dwTimerID) = NULL;

	//网络接口
public:
	//发送数据
	virtual bool SendTableData(WORD wChairID, WORD wSubCmdID) = NULL;
	//发送数据
	virtual bool SendTableData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize) = NULL;

	//功能接口
public:
	//发送场景
	virtual bool SendGameScene(IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) = NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//回调接口
interface ITableFrameSink : public IUnknownEx
{
	//管理接口
public:
	//启动服务
	virtual bool StartService(){return true;}
	//停止服务
	virtual bool ConcludeService(){return true;}

	//管理接口
public:
	//复位接口
	virtual VOID RepositionSink() = NULL;
	//配置接口
	virtual bool Initialization(ITableFrame *pTableFrame) = NULL;

	//查询接口
public:
	//获取KindID	frame交互
	virtual DWORD GetKindIDToFrame() = NULL;

	//游戏事件
public:
	//游戏开始
	virtual bool OnEventGameStart() = NULL;
	//游戏结束
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason) = NULL;
	//发送场景
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem *pIServerUserItem, BYTE cbGameStatus, bool bSendSecret) = NULL;

	//事件接口
public:
	//时间事件
	virtual bool OnTimerMessage(DWORD dwTimerID, WPARAM dwBindParameter) = NULL;

	//网络接口
public:
	//游戏消息
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, WORD wChairID) = NULL;

public:
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser) = NULL;
	//用户起立
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser) = NULL;
	//用户准备
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) = NULL;

};

#endif