#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#include "CMD_Club.h"
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

#define CMD_LC_COMMON_ERROR		    0									//通用错误 

#define SUB_CL_LOGON_ACCOUNTS		1									//帐号登录
#define SUB_CL_LOGON_REGISTER	    2									//帐号注册（返回的是登录成功）
#define SUB_CL_LOGON_PLATFORM		3									//平台登陆（第三方登录）

#define CMD_LC_LOGON_ACCOUNTS		101									//登录成功
#define CMD_LC_LOGON_PLATFORM       103                                 //平台登陆返回
#define CMD_LC_LOGON_REPEAT_LOGON	104									//重复登录
#define CMD_LC_LOGON_LOGON_REWARD   105                                 //登录奖励（老玩家回归 + 节日奖励）


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
#define SUB_CL_SERVICE_FLOWER					1							//粉丝, 关注
#define CMD_LC_SERVICE_FLOWER					101							//粉丝, 关注 返回

#define SUB_CL_SERVICE_FLOWER_QUERY				2							//查看关注
#define CMD_LC_SERVICE_FLOWER_QUERY				102							//查看关注 返回
#define CMD_LC_SERVICE_FLOWER_QUERY_FINISH		202							//查看关注 返回

#define SUB_CL_SERVICE_FLOWING_QUERY			3							//查看粉丝
#define CMD_LC_SERVICE_FLOWING_QUERY			103							//查看粉丝 返回
#define CMD_LC_SERVICE_FLOWING_QUERY_FINISH		203							//查看关注 返回

#define SUB_CL_SERVICE_FLOWER_ACT			    4							//关注, 取消关注
#define CMD_LC_SERVICE_FLOWER_ACT			    104							//关注, 取消关注 返回

#define SUB_CL_SERVICE_REFRESH_USER_INFO		5							//刷新用户信息
#define CMD_CL_SERVICE_REFRESH_USER_INFO		105							//刷新用户信息返回

#define SUB_CL_SERVICE_MODIFY_PERSONAL_INFO		12							//修改个人资料（所有用户能修改的资料都在这里处理）
#define CMD_LC_SERVICE_MODIFY_PERSONAL_INFO		112							//修改个人资料返回


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

#define SUB_CL_SERVICE_XJ_RECORD_PLAYBACK		20							//小局录像回放
#define CMD_LC_SERVICE_XJ_RECORD_PLAYBACK		120							//小局录像回放 返回

#define SUB_CL_SERVICE_GET_RANK_LIST			21							//获取指定排行榜			TOOD 真正的数据是从子游戏获取的，小局结算，但只有今日的数据，没有更新到昨日之类的
#define CMD_LC_SERVICE_GET_RANK_LIST			121							//获取指定排行榜返回

#define SUB_CL_SERVICE_GET_RANK_REWARD			22							//领取排行榜奖励
#define CMD_LC_SERVICE_GET_RANK_REWARD			122							//领取排行榜奖励返回

#pragma endregion

#pragma region MDM_SHOP 商城道具
/* *******************************************************************************
**          MAIN:7      MDM_SHOP   商城道具
** ******************************************************************************/
//【主消息号】
#define MDM_SHOP								7							//商城道具

//【子消息号】
#define SUB_CL_SHOP_BUY							1							//商城购买
#define CMD_LC_SHOP_BUY_RESULT					101							//商城购买返回

#define SUB_CL_BAG_QUERY						2							//背包物品查询
#define CMD_LC_BAG_RESULT						102							//背包物品返回
#define CMD_LC_BAG_FINISH						202							//背包物品查询结束

#define SUB_CL_GIFT_GIVE_PROPS					3							//赠送道具
#define CMD_LC_GIFT_GIVE_PROPS					103							//赠送道具返回
#define CMD_LC_GIFT_GIVE_PROPS_SHOW				203							//被赠送人的提醒

#define SUB_CL_OTHERS_RECHARGE_INFO				4							//充值信息
#define CMD_LC_OTHERS_RECHARGE_INFO				104							//充值信息 返回

#define SUB_CL_OTHERS_EXCHANGE_INFO				5							//兑换信息（钱->道具）
#define CMD_LC_OTHERS_EXCHANGE_INFO				105							//兑换信息返回


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

#endif