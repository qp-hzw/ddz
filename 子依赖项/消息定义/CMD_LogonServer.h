#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#include "STR_CMD_LogonServer.h"

/*
** 简要描述:  登录服 与 客户端的 消息
** 详细描述:  1. 消息号(主 + 子)     2. 结构体
** 备注说明:  子消息号的含义: 
**            1. 第一个字段: 归属标志,        
**				 SUB(client->server的消息)， CMD(server->client的消息), 
**				 DBR(向数据库Request)，DBO(数据库返回的消息).
**            2. 第二个字段: 消息流向         
**				 C:client;
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

#pragma region MDM_LOGON 登录模块
/* *********************************************************************************
**      MAIN:1   MDM_LOGON    登录 模块
** *********************************************************************************/
#define MDM_LOGON					1									//主消息

#define SUB_CL_LOGON_ACCOUNTS		1									//帐号登录
#define CMD_LC_LOGON_ACCOUNTS		101									//账号登录

#define SUB_CL_LOGON_REGISTER	    2									//帐号注册（返回的是账号登录）

#define SUB_CL_LOGON_PLATFORM		3									//平台登陆（第三方登录）
#define CMD_LC_LOGON_PLATFORM       103                                 //平台登陆返回

#define CMD_LC_LOGON_REPEAT_LOGON	104									//重复登录

#define CMD_LC_LOGON_LOGON_REWARD   105                                 //登录奖励（老玩家回归 + 节日奖励）

#define CMD_LC_LOGON_UPDATE_NOTIFY	106									//升级提示（登录 + 注册 + 平台登录都会用到）


#pragma endregion

#pragma region MDM_LIST 列表命令
/* *********************************************************************************
**         MAIN:2    MDM_LIST    列表命令（登录成功后主动发送给客户端，客户端无需请求)
** *********************************************************************************/
#define MDM_LIST					2									//列表信息

//列表信息返回
#define CMD_LC_LIST_TYPE			101									//类型列表TypeItem
#define CMD_LC_LIST_KIND			102									//种类列表KindItem
#define CMD_LC_LIST_NODE			103									//节点列表NodeItem
#define CMD_LC_LIST_ROOM			104									//房间列表RoomItem

#define CMD_LC_LIST_ROOM_ONLINE		105									//房间人数 TODONOW client需要增加

#pragma endregion

#pragma region MDM_SERVICE 用户服务
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

/* START 老大厅的排行榜, 之后可以删除 */
#define SUB_CL_SERVICE_GET_RICH_LIST			5							//获取富豪榜
#define CMD_LC_SERVICE_GET_RICH_LIST			105							//富豪榜返回
/* END */

/* START 老大厅的战绩, 之后可以删除掉 */
#define SUB_CL_SERVICE_GET_DJ_RECORD_LIST		6							//获取大局录像列表
#define CMD_LC_SERVICE_GET_DJ_RECORD_LIST		106							//获取大局录像列表返回

#define SUB_CL_SERVICE_GET_XJ_RECORD_LIST		7							//获取小局录像列表
#define CMD_LC_SERVICE_GET_XJ_RECORD_LIST		107							//获取小局录像列表返回
/* END */

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

#define SUB_CL_SERVICE_PURE_STANDING_LIST		15							//pure大厅排行版 查询
#define CMD_LC_SERVICE_PURE_STANDING_LIST		115							//pure大厅排行版 返回
#define CMD_LC_SERVICE_PURE_STANDING_FINISH		215							//pure大厅排行版 结束

#define SUB_CL_SERVICE_PURE_RECORD_LIST			16							//大局战绩 查询
#define CMD_LC_SERVICE_PURE_RECORD_LIST			116							//大局战绩 返回
#define CMD_LC_SERVICE_PURE_RECORD_LIST_PINFO	216							//大局战绩玩家信息 返回
#define CMD_LC_SERVICE_PURE_RECORD_FINISH		316							//大局战绩 结束

#define SUB_CL_SERVICE_PURE_XJ_RECORD_LIST		17							//小局战绩 查询
#define CMD_LC_SERVICE_PURE_XJ_RECORD_LIST		117							//小局战绩 返回
#define CMD_LC_SERVICE_PURE_XJ_RECORD_LIST_PINFO 217						//小局战绩玩家信息 返回
#define CMD_LC_SERVICE_PURE_XJ_RECORD_FINISH	317							//小局战绩 结束

#define CMD_LC_SERVICE_MARQUEE					18							//跑马灯消息

#define SUB_CL_SERVICE_GOLD_INFO				19							//请求金币大厅信息
#define CMD_LC_SERVICE_GOLD_INFO				119							//请求金币大厅信息 返回
#define CMD_LC_SERVICE_GOLD_INFO_FINISH			219							//请求金币大厅信息 结束

#define SUB_CL_SERVICE_XJ_RECORD_PLAYBACK		20							//小局录像回放
#define CMD_LC_SERVICE_XJ_RECORD_PLAYBACK		120							//小局录像回放 返回

#define SUB_CL_SERVICE_CUSTOMER_MESSEGE			21							//客服提示消息
#define CMD_LC_SERVICE_CUSTOMER_MESSEGE			121							//客服提示消息 返回

#pragma endregion

#pragma region MDM_CLUB  牌友圈
/* *******************************************************************************
**          MAIN:6      MDM_CLUB  牌友圈(版本2)
** ******************************************************************************/
//【主消息号】
#define MDM_CLUB								6							//牌友圈(版本2)

//【子消息号】
/*************** 牌友圈相关 ***********/
#define SUB_CL_CLUB_CREATE_CLUB					1							//创建牌友圈
#define CMD_LC_CLUB_CREATE_CLUB					101							//创建牌友圈返回

#define SUB_CL_CLUB_JOIN_CLUB					2							//申请加入牌友圈
#define CMD_LC_CLUB_JOIN_CLUB					102							//申请加入牌友圈返回
#define CMD_LC_CLUB_JOIN_CLUB_BDCAST			202							//申请加入牌友圈广播

#define SUB_CL_CLUB_DISS_CLUB					3							//解散牌友圈
#define CMD_LC_CLUB_DISS_CLUB					103							//解散牌友圈返回

#define SUB_CL_CLUB_ALL_CLUB_INFO_LIST			4							//查询自身牌友圈列表
#define CMD_LC_CLUB_ALL_CLUB_INFO_LIST			104							//查询自身牌友圈列表结果
#define CMD_LC_CLUB_ALL_INFO_FINISH				204							//查询自身牌友圈列表完成

#define SUB_CL_CLUB_RANDOM_CLUB_LIST			5							//查询未满员, 非自身随机牌友圈(最大9个)
#define CMD_LC_CLUB_RANDOM_CLUB_LIST			105							//查询未满员非自身随机牌友圈 返回
#define CMD_LC_CLUB_RANDOM_CLUB_LIST_FINISH		205							//查询未满员非自身随机牌友圈 结束

/*************** 牌友圈 房间相关 -- 除了查询, 其他都是游戏服处理 ***********/
#define SUB_CL_CLUB_ROOM_LIST					11							//查询指定牌友圈房间列表
#define CMD_LC_CLUB_ROOM_LIST					111							//查询指定牌友圈房间列表 返回
#define CMD_LC_CLUB_ROOM_LIST_FINISH			211							//查询指定牌友圈房间列表 结束

#define SUB_CL_CLUB_JOIN_ROOM					12							//申请加入房间
#define CMD_LC_CLUB_JOIN_ROOM					112							//申请加入房间 返回
#define CMD_LC_CLUB_TABLE_LIST_TABLE			212							//该房间的桌子信息
#define CMD_LC_CLUB_TABLE_LIST_USER				312							//该房间的玩家信息

#define SUB_CL_CLUB_ROOM_SETTING				13							//房间设置请求 
#define CMD_LC_CLUB_ROOM_SETTING				113							//房间设置请求 返回

#define SUB_CL_CLUB_ROOM_USER_LEAVE				14							//玩家离开房间 

#define SUB_CL_CLUB_ROOM_DISSOLVE				15							//解散房间请求 
#define CMD_LC_CLUB_ROOM_DISSOLVE				115							//解散房间请求 返回

#define CMD_LC_CLUB_LIST_RE						116							//实时刷新工会列表

//TODONOWW 删除房间的时候也需要 实时刷新房间列表
#define CMD_LC_CLUB_ROOM_RE						117							//实时刷新工会房间列表

#define SUB_CL_CLUB_TABLE_DISSOLVE				18							//解散桌子请求 
#define CMD_LC_CLUB_TABLE_DISSOLVE				118							//解散桌子请求 返回

#define SUB_CL_CLUB_ROOM_QUERY_SETTING			19							//请求房房间设置信息
#define CMD_LC_CLUB_ROOM_QUERY_SETTING			119							//请求房房间设置信息 返回

/*************** 牌友圈 工会操作 **********/
#define SUB_CL_CLUB_NOTICE						21							//牌友圈公告 -- 内部成员可看
#define CMD_LC_CLUB_NOTICE						121							//牌友圈公告 返回
#define CMD_LC_CLUB_NOTICE_BDCAST				221							//牌友圈公告 广播

#define SUB_CL_CLUB_MESSAGE						22							//牌友圈简介 -- 对外部展示的信息
#define CMD_LC_CLUB_MESSAGE						122							//牌友圈简介 返回  注:牌友圈简介 无须广播

#define SUB_CL_CLUB_CONTRIBUTE_FK				23							//贡献房卡 -- 对外部展示的信息
#define CMD_LC_CLUB_CONTRIBUTE_FK				123							//贡献房卡 返回

#define SUB_CL_CLUB_AUTO_AGREE					24							//牌友圈设置 -- 申请加入自动同意
#define CMD_LC_CLUB_AUTO_AGREE					124							//牌友圈设置 返回

#define SUB_CL_CLUB_CHAT						25							//聊天ALL 
#define CMD_LC_CLUB_CHAT						125							//聊天ALL 返回
#define CMD_LC_CLUB_CHAT_BDCAST					225							//聊天ALL 广播

#define SUB_CL_CLUB_STICKY_POST					26							//牌友圈工会置顶
#define CMD_LC_CLUB_STICKY_POST					126							//牌友圈工会置顶返回

/*************** 牌友圈 成员操作 **********/
#define SUB_CL_CLUB_MEMBER_MANAGER				31							//请求成员数据
#define CMD_LC_CLUB_MEMBER_MANAGER				131							//请求成员数据 返回
#define CMD_LC_CLUB_MEMBER_MANAGER_FINISH		231							//请求成员数据 结束
#define CMD_LC_CLUB_DATA						331							//工会基本信息

#define SUB_CL_CLUB_INVITE						32							//邀请他入加入牌友圈 
#define CMD_LC_CLUB_INVITE						132							//邀请他人加入牌友圈 返回
#define CMD_LC_CLUB_INVITE_REMIND				232							//被邀请人的提醒

#define SUB_CL_CLUB_INVITE_RESULT				33							//被邀请人的回复
#define CMD_LC_CLUB_INVITE_RESULT				133							//被邀请人的回复 返回

#define SUB_CL_CLUB_APPLICANT_LIST				34							//牌友圈中申请人列表
#define CMD_LC_CLUB_APPLICANT_LIST				134							//牌友圈中申请人列表 返回
#define CMD_LC_CLUB_APPLICANT_LIST_FINISH		234							//牌友圈中申请人列表 结束

#define SUB_CL_CLUB_APPLICANT_RESULT			35							//群主|管理对申请消息的答复(同意|拒绝)
#define CMD_LC_CLUB_APPLICANT_RESULT			135							//群主|管理对申请消息的答复(同意|拒绝)

#define SUB_CL_CLUB_QUIT						36							//用户退出牌友圈
#define CMD_LC_CLUB_QUIT						136							//用户给退出牌友圈 返回

#define SUB_CL_CLUB_APPOINTMENT					37							//职务任免 0提出 1将为一般成员 2设置为管理员 
#define CMD_LC_CLUB_APPOINTMENT					137							//职务任免 返回
#define CMD_LC_CLUB_APPOINTMENT_NOTE			237							//职务任免 提醒

#define SUB_CL_CLUB_INQUERY_LIST				38							//被邀请人查看自己的邀请列表
#define CMD_LC_CLUB_INQUERY_LIST				138							//被邀请人查看自己的邀请列表 返回
#define CMD_LC_CLUB_INQUERY_LIST_FINISH			238							//被邀请人查看自己的邀请列表 结束


/*************** 牌友圈 战绩查看 **********/
#define SUB_CL_CLUB_RECORD_LIST					41							//工会战绩统计 查询
#define CMD_LC_CLUB_RECORD_LIST					141							//工会战绩统计 返回
#define CMD_LC_CLUB_RECORD_FINISH				241							//工会战绩统计 结束

#pragma endregion 

#pragma region MDM_SHOP 商城道具
/* *******************************************************************************
**          MAIN:7      MDM_SHOP   商城道具
** ******************************************************************************/
//【主消息号】
#define MDM_SHOP								7							//商城道具

//【子消息号】
#define SUB_CL_SHOP_QUERY						1							//商城查询 -- 查询商品打折情况
#define CMD_LC_SHOP_QUERY_RESULT				101							//查询返回
#define CMD_LC_SHOP_QUERY_FINISH				201							//查询结束

#define SUB_CL_SHOP_MONEY						2							//(暂未用到)金钱购买道具
#define CMD_LC_SHOP_MONEY_RESULT				102							//(暂未用到)金钱购买道具 返回

#define SUB_CL_SHOP_DIAMOND						3							//钻石购买道具
#define CMD_LC_SHOP_DIAMOND_RESULT				103							//钻石购买道具 返回

#define SUB_CL_BAG_QUERY						4							//背包物品查询
#define CMD_LC_BAG_RESULT						104							//背包物品返回
#define CMD_LC_BAG_FINISH						204							//背包物品查询结束

#pragma endregion 

#pragma region MDM_GIFT 礼物道具

/* *******************************************************************************
**          MAIN:5      MDM_GIFT   礼物道具
** ******************************************************************************/
//【主消息号】
#define MDM_GIFT								5							//礼物道具

//【子消息号】
#define SUB_CL_GIFT_GIVE_PROPS					1							//赠送道具
#define CMD_LC_GIFT_GIVE_PROPS					101							//赠送道具返回
#define CMD_LC_GIFT_GIVE_PROPS_SHOW				201							//被赠送人的提醒

#define SUB_CL_OTHERS_RECHARGE_INFO				2							//充值信息
#define CMD_LC_OTHERS_RECHARGE_INFO				102							//赠送道具返回

#define SUB_CL_OTHERS_EXCHANGE_INFO				3							//兑换信息（钱->道具）
#define CMD_LC_OTHERS_EXCHANGE_INFO				3							//兑换信息返回


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
#pragma endregion

#pragma region MDM_GAME 游戏服相关
#define MDM_GAME								8							//游戏服相关

//client->logon, logon->cpd, cpd->logon, logon->client
#define SUB_CL_GAME_QUERY_GAMEID				1							//创建房间之前, 先查询可用的GameID
#define CMD_LC_GAME_QUERY_GAMEID				101							//创建房间之前, 先查询可用的GameID

#pragma endregion

#pragma pack()

#endif