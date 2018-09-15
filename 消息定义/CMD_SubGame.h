#ifndef CMD_OX_HEAD_FILE
#define CMD_OX_HEAD_FILE
#pragma pack(1)		//1字节对齐
#include <windows.h>
#include "define.h"
#include "cardType.h"


#pragma region 游戏配置
//游戏属性配置Attribute
#define KIND_ID								4												//Kind I D
#define NODE_ID								1												//Node ID
#define GAME_GENRE							(GAME_GENRE_GOLD|GAME_GENRE_MATCH)				//支持类型

//最大/小椅子数
#define MAX_CHAIR_COUNT						4			//最大椅子数 注: 1.客户端发送的房间规则中的最大椅子数 应该小于此数值  2.含机器人 3.含旁观者	
#define MIN_CHAIR_COUNT						3			//开始游戏的最小椅子数
#pragma endregion

#pragma region 发送消息
/* *********************************************************************************
**										CMD发送消息号
** *********************************************************************************/
#define CMD_SC_SEND_CARD							1							//给玩家发牌
#define CMD_SC_ROB_START							2							//抢庄开始（抢地主）		
#define CMD_SC_ROB_RESULT							3    						//抢庄结果
#define CMD_SC_APPOINT_BANKER						4    						//指定庄家
#define CMD_SC_USER_SEND_LEAVE_CARD					5							//发送底牌，并给所有玩家发送排序后的手牌数据
#define CMD_SC_MING_PAI_START						6							//明牌（出牌）开始
#define CMD_SC_MING_PAI_RESULT						7							//明牌（出牌）结果
#define CMD_SC_ADD_SCORE_START						8							//加注开始（加倍）
#define CMD_SC_ADD_SCORE_RESULT						9							//加注结果（加倍）
#define	CMD_SC_USER_OUT_CARD_START					10							//出牌开始
#define CMD_SC_USER_OUT_CARD_RESULT					11							//出牌结果
#define CMD_SC_XJ_GAME_END							12							//小局游戏结束
#define CMD_SC_DJ_GAME_END							13							//大局游戏结束


#define CMD_SC_USER_LIANGZHU_RESULT					3							//玩家亮主结果
#define CMD_SC_USER_FANZHU_NOTIFY					4							//玩家反主提示
#define CMD_SC_USER_FANZHU_RESULT					5							//玩家反主结果
#define CMD_SC_USER_KOUDI_START						7							//扣底开始
#define CMD_SC_USER_KOUDI_FINISH					8							//扣底完成（给玩家发排序后的手牌）

#define CMD_SC_USER_OUT_CARD_NOTIFY					11							//出牌提示（限制出哪些牌）


///////////////////////////////////////////////////////////////////////////////////
//发牌命令
struct STR_CMD_SC_SEND_CARD
{
	WORD		wChairID;							//发牌玩家
	BYTE		cbHandCardData[MAX_CARD_COUNT];		//手牌数据
	BYTE		cbHandCardNum;						//手牌个数
};

//抢庄开始
struct STR_CMD_SC_ROB_BANKER_START
{
	BYTE		cbType;						//玩家抢庄类型	0-未开始	1-叫地主	2-抢地主
};

//抢庄结果
struct STR_CMD_SC_ROB_RESULT
{
	WORD		wChairID;					//椅子号
	BYTE		cbType;						//玩家抢庄类型	0-未开始	1-叫地主	2-抢地主
	BYTE		cbRobState;					//玩家抢庄标志	0-过	1-叫/抢

};

//指定庄家
struct STR_CMD_SC_APPOINT_BANKER
{
	WORD		wBanker;					//庄家椅子号
};

//给玩家发送底牌
struct STR_CMD_SC_SEND_LEAVE_CARD
{
	WORD		wSendCardUser;						//发送剩余底牌的玩家
	BYTE		cbSortedCard[MAX_CARD_COUNT];		//玩家排序后的手牌
	BYTE		cbHandCardNum;						//手牌个数
	BYTE		cbMagicCardNum;						//癞子个数（主牌从左向右排序，客户端从0-cbMagicCardNum为主花色）
	BYTE		cbMagicCard;						//癞子
};

//下注(加倍)开始
struct STR_CMD_SC_ADD_SCORE_START
{
	BYTE		cbStart;					//下注开始标识	0-未开始	1-开始
};

//下注结果
struct STR_CMD_SC_ADD_SCORE_RESULT
{
	WORD                                wChairID;						//下注玩家 
	WORD                                wAddSocre;						//该次下注分值
};

//明牌开始
struct STR_CMD_SC_MING_PAI_START
{
	WORD                                wChairID;						//明牌开始玩家 
	BYTE                                cbType;							//明牌类型	2-发牌开始明牌	1-出牌开始明牌
};

//明牌结果
struct STR_CMD_SC_MING_PAI_RESULT
{
	WORD                                wChairID;						//明牌玩家 
	BYTE                                cbIsMing;						//是否明牌	0-不明牌	1-明牌
	BYTE								cbTimes;						//加倍后的倍数
	BYTE								cbHandCard[MAX_CARD_COUNT];		//用户手牌
};




//亮主结果
struct STR_CMD_SC_LIANGZHU_RESULT
{
	WORD		wLiangzhuUser;						//亮主玩家
	BYTE		cbLZCard;							//亮主卡牌
};	

//反主开始
struct STR_CMD_SC_FANZHU_NOTIFY
{
	WORD		wFanzhuUser;						//反主玩家
	BYTE		cbFanZhuCard[MAX_FANZHU_TYPE_NUM];	//反主卡牌
};	 

//反主结果
struct STR_CMD_SC_FANZHU_RESULT
{
	WORD		wFanzhuUser;						//反主玩家
	BYTE		cbFanZhuCard;						//反主卡牌
};	 	

//扣底开始
struct STR_CMD_SC_SEND_KOUDI_START
{
	WORD		wKouDiUser;						//扣底玩家		
	BYTE		cbKouDiNum;						//扣底卡牌数目;
};	

//扣底完成
struct STR_CMD_SC_SEND_KOUDI_FINISH
{
	WORD		wKouDiUser;						//扣底玩家
	BYTE		cbFinishMask;					//0-未完成	1-完成
	BYTE		cbUserHandCard[MAX_CARD_COUNT];	//扣底完成后的玩家排序手牌
	BYTE		cbMainColorNum;					//主牌个数（主牌从左向右排序，客户端从0-cbMainColorNum为主花色）
};	

//出牌提示
struct STR_CMD_SC_OUT_CARD_NOTIFY
{
	tagOutCardNotify		OutCardNotify;		//出牌提示
};	

//出牌开始
struct STR_CMD_SC_OUT_CARD_START
{
	WORD		wOutCardUser;					//出牌提玩家
};	

//出牌结果
struct STR_CMD_SC_OUT_CARD_RESULT
{
	BYTE		cbFlag;								//过的标识	0-过	1-正常出牌
	BYTE		cbSuccess;							//是否成功	1-出牌成功  0-牌型错误
	WORD		wOutCardUser;						//出牌玩家
	BYTE		cbOutCardNum;						//出牌数目
	BYTE		cbOutCard[MAX_CARD_COUNT];			//出牌数据
	BYTE		cbHandCardNum;						//手牌个数
	BYTE		cbCardType;							//牌型，牌型大于7，则为奖，客户端显示奖得分
	SCORE		lBonusScore;						//奖得分，有奖才有值
	SCORE		lSingleGameScore[MAX_CHAIR_COUNT];	//玩家单局得分
};	

//一轮积分结算
struct STR_CMD_SC_ROUND_BALANCE
{
	WORD		wRoundWinner;						//该轮赢家
	WORD		wCurOutCardUser;					//当前出牌玩家
	LONGLONG	lUserScore[MAX_CHAIR_COUNT];		//该轮玩家积分
	LONGLONG	lRoundScore;						//该轮分数，若守分方赢则是0，得分方赢则是具体分数
};	

//小局游戏结束
struct STR_CMD_SC_XJ_GAME_END
{
	BYTE	cbLeaveCard[MAX_LEAVE_CARD_NUM];			//底牌	
	SCORE	nTotalGameScore[MAX_CHAIR_COUNT];			//玩家总得分数		//所有的得分都用LONGLONG，都是8个字节
	SCORE	nSingleGameScore[MAX_CHAIR_COUNT];			//玩家单局得分
};

//大局游戏结束
struct STR_CMD_SC_DJ_GAME_END
{
	WORD							wMaxWinChairID;							// 赢次数最多的玩家椅子号
	WORD							wFangzhuID;								// 房主ID
	SCORE							lUserScore[MAX_CHAIR_COUNT];			// 玩家总得分 
	BYTE                            bWinTime[MAX_CHAIR_COUNT];				// 胜利次数
};

#pragma endregion

#pragma region 接收消息
/* *********************************************************************************
**										SUB接收消息
** *********************************************************************************/
#define SUB_CS_ROB_BANKER							1							//抢庄
#define SUB_CS_MING_PAI								2							//明牌
#define SUB_CS_ADD_SCORE							3							//加注（加倍）
#define SUB_CS_OUT_CARD								4							//出牌
#define SUB_CS_XJ_GAME_READY 						5							//小局游戏准备

#define SUB_CS_LIANG_ZHU							1							//亮主
#define SUB_CS_FAN_ZHU								2							//反主
#define SUB_CS_KOU_DI								3							//扣底
/////////////////////////////////////////////////////////////////////////////////////////////

//抢庄命令
struct STR_SUB_CS_ROB_BANKER
{
	BYTE		cbType;						//玩家抢庄类型	0-叫地主	1-抢地主	
	BYTE		cbRobState;					//抢庄状态		0-过	1-叫/抢	
};

//玩家下注  
struct STR_SUB_CS_ADD_SCORE
{
	DWORD		score;						//下注分值	0-代表不下注
};

//玩家明牌
struct STR_SUB_CS_MING_PAI
{
	BYTE        cbType;						//明牌类型	2-发牌开始明牌	1-出牌开始明牌
	BYTE		cbIsMing;					//是否明牌	0-不明牌	1-明牌
};


//亮主
struct STR_SUB_CS_LIANG_ZHU
{
	WORD		wLiangZhuUser;		//亮主玩家
	BYTE		cbLiangZhuColor;	//亮主花色
};

//反主
struct STR_SUB_CS_FAN_ZHU
{
	BYTE		cbFlag;				//反主标识	0-玩家放弃反主	1-玩家反主
	WORD		wFanZhuUser;		//反主玩家
	BYTE		cbFanZhuCard;		//反主卡牌
};

//扣底
struct STR_SUB_CS_KOUDI
{
	WORD		wKouDiUser;							//扣底玩家
	BYTE		cbKouDiCard[MAX_LEAVE_CARD_NUM];	//扣底卡牌
};

//出牌
struct STR_SUB_CS_OUT_CARD
{
	BYTE		cbPass;								//过	0-正常出牌	1-过
	WORD		wOutCardUser;						//出牌玩家
	BYTE		cbOutCardNum;						//出牌数目
	BYTE		cbOutCard[MAX_CARD_COUNT];			//出牌数据
};

//小局游戏准备
struct STR_SUB_CS_XJ_USER_READY
{
	WORD		wReadyUser;							//准备玩家
	BYTE		cbReady;							//小局游戏用户准备	1-准备；0-未准备
};

#pragma endregion

#pragma region 断线重连
//空闲状态
struct STR_CMD_SC_STATUS_FREE
{
	BYTE	GameCount;				//游戏局数 0-无限局
	BYTE	PlayerCount;			//游戏人数
	WORD	CellScore;				//底分	  默认为1
	DWORD	FangZhu;				//房主    创建用户
};

//亮主状态
struct STR_CMD_SC_STATUS_LIANGZHU
{
	BYTE	cbHandCard[MAX_CARD_COUNT];			//玩家手牌
	BYTE	cbCardNum;							//玩家手牌总数
	BYTE	cbLastTime;							//亮主定时器剩余时间（秒）
};

//反主状态
struct STR_CMD_SC_STATUS_FANZHU
{
	WORD	wCurFanzhuUser;						//当前反主玩家
	BYTE	cbFanZhuCard[MAX_FANZHU_TYPE_NUM];	//玩家自己的反主卡牌，当前玩家是自己的时候才显示反主提示
	BYTE	cbLastTime;							//反主定时器剩余时间（秒）
};

//扣底状态
struct STR_CMD_SC_STATUS_KOUDI
{
	WORD		wCurKouDiUser;						//当前扣底玩家
	BYTE		cbSortedCard[MAX_CARD_COUNT];		//玩家排序后的手牌
	BYTE		cbHandCardNum;						//手牌个数
	BYTE		cbMainColorNum;						//主牌个数（主牌从左向右排序，客户端从0-cbMainColorNum为主花色）
	BYTE		cbMainColor;						//当前主花色
};

//出牌状态
struct STR_CMD_SC_STATUS_OUTCARD
{		
	WORD		wCurOutCardUser;							//当前出牌玩家
	BYTE		cbOutCard[MAX_CHAIR_COUNT][MAX_CARD_COUNT];	//玩家已经出的牌	
};

//小局结算状态
struct STR_CMD_SC_STATUS_XJ_END
{		
	SCORE	nSingleGameScore[MAX_CHAIR_COUNT];				//玩家单局得分	
};


//***********************************************************************************//
#pragma endregion

#pragma region 待处理 -- 机器人
///////////////////////////////////////////////////////////////////////////////////////////////////
// 机器人使用, 暂时未用到 added by WangChengQing 2017.9.21 TODONOW 以后应该在棋牌中增加机器人功能
///////////////////////////////////////////////////////////////////////////////////////////////////
//服务器命令结构    --  机器人
#define SUB_S_A_ADD_SCORE					(0x0008)									//加注结果
#define SUB_S_A_PLAYER_EXIT				(0x0010)									//用户强退
#define SUB_S_A_GAME_END					(0x0040)									//游戏结束
#define SUB_S_A_OPEN_CARD					(0x0080)									//用户摊牌
#define SUB_S_A_CALL_BANKER				(0x0100)									//用户叫庄
#define SUB_S_A_CALL						(0x0400)									//用户叫庄

//玩家flash ID 和 动作ID
struct CMD_S_PlayerAction
{
	WORD wChairID;
	int iActionID;
};
//玩家 动作ID
struct CMD_C_PlayerAction
{
	int iActionID;
};


//游戏状态
struct CMD_S_StatusCall
{
	WORD							    	wCallBanker;						//叫庄用户
	BYTE							        cbPlayStatus[MAX_CHAIR_COUNT];          //用户状态
	BYTE									cbCallStatus[MAX_CHAIR_COUNT];			//叫庄状态
	bool									bCallBanker[MAX_CHAIR_COUNT];			//是否叫庄
	SCORE									lCellScore;							//基础积分
	SCORE									lRevenue;
	BYTE									cbTimeLeave;
	SCORE									lPlayerScore[MAX_CHAIR_COUNT];			//玩家分数
};

struct CMD_S_PlayerCount
{
	BYTE									wPlayerCount;
	SCORE									lUserScore[MAX_CHAIR_COUNT];			//玩家积分
};
//
////用户叫庄
struct CMD_S_CallBanker
{
	WORD							     	wCallBanker;						//叫庄用户
	bool							    	bFirstTimes;						//首次叫庄
};

////////////////////////////////////////////////////////////////////////////
//客户端命令结构             --  机器人
#define SUB_C_A_CALL_BANKER				1									//用户叫庄
#define SUB_C_A_ADD_SCORE					2									//用户加注
#define SUB_C_A_OPEN_CARD					3									//用户摊牌
#define SUB_C_A_AMDIN_COMMAND				5									//管理员命令
#define SUB_C_A_AMDIN_CHANGE_CARD			6									//管理员命令
#define SUB_C_A_READY_MESSAGE				7								    //客户端发送准备消息

/////////-----5
struct CMD_C_Admin_ChangeCard
{
	BYTE cbStart;
	BYTE cbEnd;

};

//用户叫庄
struct CMD_C_CallBanker
{
	BYTE							    	bBanker;							//做庄标志
};
#pragma endregion
#pragma pack()

#endif
