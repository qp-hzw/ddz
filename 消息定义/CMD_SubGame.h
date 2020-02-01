#ifndef CMD_OX_HEAD_FILE
#define CMD_OX_HEAD_FILE
#pragma pack(1)		//1字节对齐
#include <windows.h>
#include "define.h"
#include "cardType.h"
#include <string>

#pragma region 游戏配置
//游戏属性配置Attribute
#define KIND_ID								9												//Kind I D
#define NODE_ID								1												//Node ID
#define GAME_GENRE							(GAME_GENRE_GOLD|GAME_GENRE_MATCH)				//支持类型

//最大/小椅子数
#define MAX_CHAIR_COUNT						5			//最大椅子数 注: 1.客户端发送的房间规则中的最大椅子数 应该小于此数值  2.含机器人 3.含旁观者	
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
#define	CMD_SC_PUBLIC_BET							14							// 公共倍数
#define	CMD_SC_JIAOFEN_START						15							// 叫三分开始
#define	CMD_SC_JIAOFEN_RESULT						16							// 叫三分结果
#define	CMD_SC_JIPAIQI_START						17							// 记牌器
#define	CMD_SC_FANGJIAN_BET							18							// 房间倍数
#define	CMD_SC_TUO_GUAN								19							//托管
#define	CMD_SC_RECODE								27							//录像

//一下目前不需要
#define	CMD_SC_STATUS_FREE							20						// 重连--空闲状态
#define	CMD_SC_STATUS_OUTCARD						21						// 重连--出牌状态
#define	CMD_SC_STATUS_XJ_END						22						// 重联--小局结束
#define	CMD_SC_STATUS_ROB							23						// 重连--抢庄状态
#define	CMD_SC_STATUS_ADD_SCORE						24						// 重连--下注状态
#define	CMD_SC_STATUS_JIAOFEN						25						// 重连叫分状态
#define	CMD_SC_RULE									26						//规则


///////////////////////////////////////////////////////////////////////////////////
//发牌命令
struct STR_CMD_SC_SEND_CARD
{
	WORD		wChairID;							//发牌玩家
	BYTE		cbHandCardData[MAX_CARD_COUNT];		//手牌数据
	BYTE		cbHandCardNum[MAX_CHAIR_COUNT];						//手牌个数
	DWORD		CurJuShu;						//当前局数
	DWORD		ISMingPaiStart;					//是否明牌开始  1-是 0-不是
	//repeated Card  	   HandCardData;  	        //辅助录像回放手牌数据
	DWORD		GoldCoin[MAX_CHAIR_COUNT];				//金币
	//DWORD		replay_code = 8;					//回放码
};
//struct Card				//录像回放是否还是用原来的
//{
//	repeated  CardData = 1;
//			   wChairID = 2;
//}

//抢庄开始
struct STR_CMD_SC_ROB_BANKER_START
{
};

//抢庄结果
struct STR_CMD_SC_ROB_RESULT
{
	WORD		wChairID;					//椅子号
	//BYTE		cbType;						//玩家抢庄类型	0-未开始	1-叫地主	2-抢地主
	//BYTE		cbRobState;					//玩家抢庄标志	0-过	1-叫/抢
	BYTE			cbResult;					//1-不叫  2-叫地主  3-不抢  4-抢地主
	DWORD	room_bet[MAX_CHAIR_COUNT];					//房间倍数
};

//叫三分开始
struct STR_CMD_SC_JIAOFEN_START
{
	WORD		wChairID;		//叫三分的玩家
	BYTE		ActionType;		//二进制动作行为   0001-不叫  0010-叫一分 0100-叫二分 1000-叫三分	
};

//叫三分结果
struct STR_CMD_SC_JIAOFEN_RESULT
{
	WORD 		wChairID;						//椅子号
	BYTE		Score;							//玩家交了多少分   0 1 2 3
	BYTE		room_bet[MAX_CHAIR_COUNT];		//各玩家的房间倍数	
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
	BYTE		cbSortedCard[MAX_CARD_COUNT];		//玩家排序后的手
	BYTE		cbHandCardNum;						//手牌个数
	BYTE		cbMagicCardNum;						//癞子个数（主牌从左向右排序，客户端从0-cbMagicCardNum为主花色）
	BYTE		cbMagicCard;						//癞子
	BYTE		cbLeaveCard[MAX_LEAVE_CARD_NUM];	//底牌数据
	DWORD		LeaveCardBet;						//底牌倍数
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
	DWORD								RoomBet[MAX_CHAIR_COUNT];       //房间倍数
};

//明牌开始
struct STR_CMD_SC_MING_PAI_START
{
	WORD                                wChairID;						//明牌开始玩家 
	BYTE                                cbType;							//明牌类型	3-发牌开始明牌	2-开始游戏明牌	1-出牌开始明牌
};

//明牌结果
struct STR_CMD_SC_MING_PAI_RESULT
{
	WORD                                wChairID;						//明牌玩家 
	BYTE                                cbIsMing;						//是否明牌	0-不明牌	1-明牌
	BYTE								cbTimes;						//加倍后的倍数
	BYTE								cbHandCard[MAX_CARD_COUNT];		//用户手牌
	DWORD							    RoomBet[MAX_CHAIR_COUNT];		//房间倍数
};




////亮主结果
//struct STR_CMD_SC_LIANGZHU_RESULT
//{
//	WORD		wLiangzhuUser;						//亮主玩家
//	BYTE		cbLZCard;							//亮主卡牌
//};	
//
////反主开始
//struct STR_CMD_SC_FANZHU_NOTIFY
//{
//	WORD		wFanzhuUser;						//反主玩家
//	BYTE		cbFanZhuCard[MAX_FANZHU_TYPE_NUM];	//反主卡牌
//};	 
//
////反主结果
//struct STR_CMD_SC_FANZHU_RESULT
//{
//	WORD		wFanzhuUser;						//反主玩家
//	BYTE		cbFanZhuCard;						//反主卡牌
//};	 	
//
////扣底开始
//struct STR_CMD_SC_SEND_KOUDI_START
//{
//	WORD		wKouDiUser;						//扣底玩家		
//	BYTE		cbKouDiNum;						//扣底卡牌数目;
//};	
//
////扣底完成
//struct STR_CMD_SC_SEND_KOUDI_FINISH
//{
//	WORD		wKouDiUser;						//扣底玩家
//	BYTE		cbFinishMask;					//0-未完成	1-完成
//	BYTE		cbUserHandCard[MAX_CARD_COUNT];	//扣底完成后的玩家排序手牌
//	BYTE		cbMainColorNum;					//主牌个数（主牌从左向右排序，客户端从0-cbMainColorNum为主花色）
//};	
//
////出牌提示
//struct STR_CMD_SC_OUT_CARD_NOTIFY
//{
//	tagOutCardNotify		OutCardNotify;		//出牌提示
//};	


//出牌开始
struct STR_CMD_SC_OUT_CARD_START
{
	WORD		wOutCardUser;						//出牌的玩家
	BYTE		ActionType;							//二进制动作行为   //0001-出牌  0010-不出  0100-提示  1000-要不起
	//BYTE		TurnOutCardData[MAX_CARD_COUNT];				//当前轮最大出牌数据
	//DWORD		TurnOutCardNum;					//当前轮最大出牌数目     //是否需要
};	

//出牌结果
struct STR_CMD_SC_OUT_CARD_RESULT
{
	BYTE		cbFlag;								//过的标识	0-过	1-正常出牌
	BYTE		cbSuccess;							//是否成功	1-出牌成功  0-牌型错误
	WORD		wOutCardUser;						//出牌玩家
	BYTE		cbOutCardNum;						//出牌数目	
	BYTE		cbOutCard[MAX_CARD_COUNT];			//出牌数据
	BYTE		handcarddata[MAX_CARD_COUNT];       //出牌玩家手牌数据
	BYTE		cbHandCardNum;						//手牌个数
	BYTE		cbCardType;							//牌型，牌型大于7，则为奖，客户端显示奖得分
	//SCORE		lSingleGameScore[MAX_CHAIR_COUNT];	//玩家单局得分

	DWORD		RoomBet[MAX_CHAIR_COUNT];			//房间倍数
	BYTE		cbIsOneTurnEnd;						//0-一轮结束 1-一轮未结束
};	

////一轮积分结算
//struct STR_CMD_SC_ROUND_BALANCE
//{
//	WORD		wRoundWinner;						//该轮赢家
//	WORD		wCurOutCardUser;					//当前出牌玩家
//	LONGLONG	lUserScore[MAX_CHAIR_COUNT];		//该轮玩家积分
//	LONGLONG	lRoundScore;						//该轮分数，若守分方赢则是0，得分方赢则是具体分数
//};

//小局游戏结束
struct STR_CMD_SC_XJ_GAME_END
{
	/*
	struct	XJ_END
	{ 
		WORD 		UsrChairID;							//玩家座位号
		BYTE		cbLeaveHandCard[MAX_CARD_COUNT];	//玩家剩余手牌
		SCORE		nTotalGameScore;  					//玩家总得分数
		SCORE		nSingleGameScore;					//玩家单局得分
		SCORE		UserBet;							//玩家此局的倍数
		SCORE		RoomSocre;							//金币场房间底分
		WORD		BankID;								//庄家
		std::string		PlayerName;    					//玩家名字
		DWORD		IsWin;								//玩家是否赢了  0-败  1-胜
		DWORD		IsEnd;								//大局是否结束    0-未结束  1-结束
	};
	struct XJ_END		xj_end[MAX_CHAIR_COUNT];
	WORD				wchairID;
	*/

	SCORE		nSingleGameScore[MAX_CHAIR_COUNT];				//玩家单局得分
	DWORD		nUserBet[MAX_CHAIR_COUNT];								//玩家此局的倍数
	BYTE			cbLeaveHandCard[MAX_CHAIR_COUNT][MAX_CARD_COUNT];			//玩家剩余手牌
	BYTE			cbLeaveCardCount[MAX_CHAIR_COUNT];												//玩家剩余手牌数量
	WORD		wWinChair;																//赢家
};

//大局游戏结束
struct STR_CMD_SC_DJ_GAME_END
{
	WORD							wMaxWinChairID;							// 赢次数最多的玩家椅子号
	WORD							wFangzhuID;								// 房主ID
	SCORE							lUserScore[MAX_CHAIR_COUNT];			// 玩家总得分 
	//BYTE                            bWinTime[MAX_CHAIR_COUNT];				// 胜利次数
	//WORD							BankTime[MAX_CHAIR_COUNT];				//地主次数

	//struct PlayerInfo
	//{
	//	std::string		playerName;    //名字
	//	WORD			playerID;   		//ID
	//	std::string		szFaceUrl;			//头像框
	//};

	//struct PlayerInfo	playerinfo[MAX_CHAIR_COUNT];      			//玩家信息
	//SCORE				PlayerBigestBet[MAX_CHAIR_COUNT];			//玩家曾获得过的最大倍数
	//WORD				IsEnd;												//是否结束  0-结束  1-未结束
	//WORD				Rich_ID[MAX_CHAIR_COUNT];					//大土豪的椅子号 输得最多的
};

//游戏结束 公共倍数
struct STR_CMD_SC_PUBLIC_BET
{
	SCORE		RoomBet;			//底分
	SCORE		MingPai;			//明牌
	SCORE		Rob;				//抢地主
	SCORE		LeaveCard;			//底牌
	SCORE		Boom;				//炸弹
	SCORE 		Spring;			//春天
	SCORE		AddScore[MAX_CHAIR_COUNT];			//加倍
	SCORE		IUserAScore;			//总分 
	WORD		Game_State;			//当前游戏状态
	WORD		bank_ID;			//庄家ID
	SCORE		public_bet;			//公共倍数
	WORD		Rule;			//当前游戏规则  0-抢庄  1-叫分
};

//记牌器
struct STR_CMD_SC_JIPAIQI_START
{
	WORD		JiPaiQi[MAX_CARD_COUNT];			//记牌器
};

//房间倍数
struct STR_CMD_SC_FANGJIAN_BET
{
	SCORE	room_bet[MAX_CHAIR_COUNT];
};

//托管
struct STR_CMD_SC_TUO_GUAN
{
	WORD	tuo_guan;	//传1表示已自动托管
};

//rule
struct STR_CMD_SC_RULE
{
	WORD	rule;   // 0-jingdian   1-buxip
};

//录像回放
struct STR_CMD_SC_RECODE
{
	std::string		ReplayData;			//录像回放数据
};

#pragma endregion

#pragma region 接收消息
/* *********************************************************************************
**										SUB接收消息
** *********************************************************************************/
#define SUB_CS_ROB_BANKER							101							//抢庄
#define SUB_CS_MING_PAI								102							//明牌
#define SUB_CS_ADD_SCORE							103							//加注（加倍）
#define SUB_CS_OUT_CARD								104							//出牌
#define SUB_CS_XJ_GAME_READY 						105							//小局游戏准备
#define	SUB_CS_PUBLIC_BET							106							//大局结束公共倍数
#define	SUB_CS_JIAO_FEN								107							//叫分
#define	SUB_CS_JIPAIQI								108							//记牌器
#define	SUB_CS_TUO_GUAN								109							//托管

//#define SUB_CS_LIANG_ZHU							1							//亮主
//#define SUB_CS_FAN_ZHU								2							//反主
//#define SUB_CS_KOU_DI								3							//扣底
/////////////////////////////////////////////////////////////////////////////////////////////

//抢庄命令
struct STR_SUB_CS_ROB_BANKER
{
	BYTE			cbResult;					// 2叫,抢 | 1不叫,不抢
};

//叫三分命令
struct STR_SUB_CS_JIAO_FEN
{
	SCORE		Score;							//玩家叫了多少分   0-不叫  1-1分 2-2分 3-3分
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
	SCORE		OutCard_bet;				//发牌明牌的倍数
};


////亮主
//struct STR_SUB_CS_LIANG_ZHU
//{
//	WORD		wLiangZhuUser;		//亮主玩家
//	BYTE		cbLiangZhuColor;	//亮主花色
//};
//
////反主
//struct STR_SUB_CS_FAN_ZHU
//{
//	BYTE		cbFlag;				//反主标识	0-玩家放弃反主	1-玩家反主
//	WORD		wFanZhuUser;		//反主玩家
//	BYTE		cbFanZhuCard;		//反主卡牌
//};

////扣底
//struct STR_SUB_CS_KOUDI
//{
//	WORD		wKouDiUser;							//扣底玩家
//	BYTE		cbKouDiCard[MAX_LEAVE_CARD_NUM];	//扣底卡牌
//};

//出牌
struct STR_SUB_CS_OUT_CARD
{
	BYTE		cbPass;								//过	0-正常出牌	1-过
	//WORD		wOutCardUser;						//出牌玩家
	BYTE		cbOutCardNum;						//出牌数目
	BYTE		cbOutCard[MAX_CARD_COUNT];			//出牌数据
};

//小局游戏准备
struct STR_SUB_CS_XJ_USER_READY
{
	WORD		wReadyUser;							//准备玩家
	BYTE		cbReady;							//小局游戏用户准备	1-准备；0-未准备
	SCORE		cbBet;								//明牌开始倍数
};

//游戏结束 公共倍数
struct STR_SUB_CS_PUBLIC_BET
{
	WORD		wChairID;			//要查看的椅子号
};

//记牌器
struct STR_SUB_CS_JIPAIQI
{
	WORD tmp;			//不要传0
};

//托管
struct STR_SUB_CS_TUO_GUAN
{
	WORD	TuoGuan_state;		//1-设置托管   0-取消托管
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

////亮主状态
//struct STR_CMD_SC_STATUS_LIANGZHU
//{
//	BYTE	cbHandCard[MAX_CARD_COUNT];			//玩家手牌
//	BYTE	cbCardNum;							//玩家手牌总数
//	BYTE	cbLastTime;							//亮主定时器剩余时间（秒）
//};
//
////反主状态
//struct STR_CMD_SC_STATUS_FANZHU
//{
//	WORD	wCurFanzhuUser;						//当前反主玩家
//	BYTE	cbFanZhuCard[MAX_FANZHU_TYPE_NUM];	//玩家自己的反主卡牌，当前玩家是自己的时候才显示反主提示
//	BYTE	cbLastTime;							//反主定时器剩余时间（秒）
//};
//
////扣底状态
//struct STR_CMD_SC_STATUS_KOUDI
//{
//	WORD		wCurKouDiUser;						//当前扣底玩家
//	BYTE		cbSortedCard[MAX_CARD_COUNT];		//玩家排序后的手牌
//	BYTE		cbHandCardNum;						//手牌个数
//	BYTE		cbMainColorNum;						//主牌个数（主牌从左向右排序，客户端从0-cbMainColorNum为主花色）
//	BYTE		cbMainColor;						//当前主花色
//};


//抢庄状态
struct STR_CMD_SC_STATUS_ROB
{
	BYTE			CurRobUsr;					//当前正在抢庄的玩家
	BYTE 			robstate[MAX_CHAIR_COUNT];	//各玩家的抢庄状态
	BYTE			HandCardData[MAX_CARD_COUNT];	//当前玩家的手牌
	BYTE			HandCardNum[MAX_CHAIR_COUNT];	//各玩家的手牌个数
	BYTE			CurJuShu;		//当前房间局数
	SCORE			room_bet;	//当前房间倍数
	SCORE			PlayerScore[MAX_CHAIR_COUNT];	//总积分
	//SCORE			GoldCoin[MAX_CHAIR_COUNT];	//各玩家的金币
	//DWORD		    replay_code;			//回放码
};

//叫分状态
struct STR_CMD_SC_STATUS_JIAOFEN
{
	WORD	   CurRobUsr;            //当前正在叫分的玩家
	WORD	   robstate[MAX_CHAIR_COUNT];     //各玩家的叫分状态
	BYTE	   HandCardData[MAX_CARD_COUNT];         //当前玩家的手牌
	DWORD	   HandCardNum[MAX_CHAIR_COUNT];  //各玩家的手牌个数
	DWORD	   CurJuShu;             //当前房间局数
	SCORE	   room_bet[MAX_CHAIR_COUNT];     //当前房间倍数
	SCORE	   All_bet[MAX_CHAIR_COUNT];      //总积分
	SCORE	   GoldCoin[MAX_CHAIR_COUNT];	//各玩家的金币
	DWORD	   replay_code;			//回放码
};


//下注状态
struct STR_CMD_SC_STATUS_ADD_SCORE
{
	WORD		scorestate[MAX_CHAIR_COUNT];	//各玩家的下注状态
	BYTE		HandCardData[MAX_CARD_COUNT];	//当前玩家的手牌
	DWORD		HandCardNum[MAX_CHAIR_COUNT];	//各玩家的手牌个数
	DWORD		CurJuShu;			             //当前房间局数
	SCORE 		room_bet[MAX_CHAIR_COUNT];	//当前房间的倍数
	WORD		bankID;		//庄家ID
	SCORE		All_bet[MAX_CHAIR_COUNT];	//总积分
	SCORE 		GoldCoin[MAX_CHAIR_COUNT];	//各玩家的金币
	DWORD		replay_code;			//回放码
};

struct player_op_info
{
   DWORD					op_type;    //操作类型 0-过 1-出牌
   BYTE 				op_result[MAX_CARD_COUNT]; //出牌结果, 牌数据; 只在op_type=1时才有效
   DWORD				op_cardscount;   //当前牌数量
   //BYTE					MingPaiCardData[MAX_CARD_COUNT];   //明牌玩家的手牌
   BYTE					op_outCardCount;				//出牌数量
};

//出牌状态
struct STR_CMD_SC_STATUS_OUTCARD
{
	struct	player_op_info		players_op[MAX_CHAIR_COUNT];     //玩家信息
	WORD						outcardid;						//当前出牌玩家
	BYTE 						brokenoutcarddata[MAX_CARD_COUNT];					//自己的手牌数据
	BYTE						CurJuShu;             //当前房间局数
	SCORE 						room_bet;		//当前房间的倍数
	WORD						bankID;		//庄家ID
	//WORD 						MingPaiState[MAX_CHAIR_COUNT];		//各玩家的明牌状态
	BYTE 						LeaveCard[MAX_LEAVE_CARD_NUM];		//底牌
	SCORE						PlayerScore[MAX_CHAIR_COUNT];		//总积分
	//BYTE						TurnCardData[MAX_CARD_COUNT];	//当前轮最大出牌
	//SCORE 						GoldCoin[MAX_CHAIR_COUNT];	//各玩家的金币
	//WORD						Add_bet[MAX_CHAIR_COUNT];		//玩家是否加倍
	SCORE						Leave_bet;		//底牌倍数
	BYTE						IsTurnEnd;			//是否是第一个出牌的玩家
	BYTE						ActionType;			//出牌二进制动作行为
	//DWORD					    replay_code;			//回放码
};


//小局结算状态
struct STR_CMD_SC_STATUS_XJ_END
{
	STR_CMD_SC_XJ_GAME_END	XjGameEnd;
	WORD					Banker;		//当前地主
	BYTE					GameCount;	//当前局数
	BYTE					LeaveCard[MAX_LEAVE_CARD_NUM];
};

//子游戏房间规则
struct STR_CMD_SUB_ROOM_RULE
{
	WORD		Gamejushu;                  //游戏局数
	WORD		Cellscore;                   //游戏底分(1/2/5)
	WORD		Balancemode;                //结算方式(0:积分结算 1:金币结算)
	WORD		GameChuPai;                 //游戏出牌(限时-->15秒出牌/25秒出牌/不限制出牌)
	WORD		GameWanFa;                  //游戏玩法(经典玩法/癞子玩法/天地癞子)
	WORD		GameDiZhu;                  //游戏地主模式(0-抢地主 1-叫三分)
	WORD		GameFengDing;               //游戏封顶模式(32/64/128/不封顶)
	WORD		Preventcheat;               //防作弊
	WORD		DontCutCards;               //不洗牌
	WORD		AddMultiple;               //加倍
	WORD		ShowCards;                 //明牌
	WORD		BaseCardAddMultiple;       //底牌翻倍
	WORD		GameMode;                  //游戏得分模式   0-经典模式  1-疯狂模式 
};

////录像回放结构体
//message GAME_REPLAY
//{
//	//子游戏房间规则
//	message MSG_SUB_ROOM_RULE
//	{
//			int32 Gamejushu = 1;                  //游戏局数
//		int32 Cellscore = 2;                   //游戏底分(1/2/5)
//		int32 Balancemode = 3;                //结算方式(0:积分结算 1:金币结算)
//		int32 GameChuPai = 4;                 //游戏出牌(限时-->15秒出牌/25秒出牌/不限制出牌)
//		int32 GameWanFa = 5;                  //游戏玩法(经典玩法/癞子玩法/天地癞子)
//		int32 GameDiZhu = 6;                  //游戏地主模式(0-抢地主 1-叫三分)
//		int32 GameFengDing = 7;               //游戏封顶模式(32/64/128/不封顶)
//		int32 Preventcheat = 8;               //防作弊
//		int32 DontCutCards = 9;               //不洗牌
//		int32 AddMultiple = 10;               //加倍
//		int32 ShowCards = 11;                 //明牌
//		int32 BaseCardAddMultiple = 12;       //底牌翻倍
//		int32 GameMode = 13;                  //游戏得分模式   0-经典模式  1-疯狂模式
//	};
//
////玩家信息
//message MSG_TABLE_PLAYER_INFO_NOTIFY
//{
//		uint32 messageid = 1;
//		uint64  player_id = 2;                  //玩家ID
//		string name = 3;                    //玩家名字
//		int64  registerDate = 4;                //注册时间
//		uint32 sex = 5;                    //性别
//		int32 userstate = 6;                  //玩家状态
//		int32 tableposid = 7;                //座位号
//		int32 isonline = 8;                  //在线状态：1：为在线0：不在线
//		int64 playergold = 9;                //金币
//		string faceurl = 10;                  //图像地址
//};
//
//
////操作信息
//message OPERATE_INFO
//{
//	MSG_M2C_SEND_CARD 			send_card = 1;		//发牌
//	MSG_M2C_ROB_RESULT 			rob_result = 2;		//抢庄结果
//	MSG_M2C_APPOINT_BANKER 				appoint_banker = 3;	//指定庄家
//	MSG_M2C_SEND_LEAVE_CARD		send_leave_card = 4;	//发送底牌
//	MSG_M2C_MING_PAI_RESULT		mingpai_result = 5;	//明牌结果
//	MSG_M2C_ADD_SCORE_RESULT		addscore_result = 6;	//加倍结果
//	MSG_M2C_OUT_CARD_RESULT		outcard_result = 7;	//出牌结果
//	MSG_M2C_XJ_GAME_END			xj_end = 8;		//小局结束
//	MSG_M2C_DJ_GAME_END				dj_end = 9;		//大局结束
//	MSG_M2C_JIAOFEN_RESULT			jiaosanfen_result = 10;	//叫三分结果	
//};
//
//MSG_SUB_ROOM_RULE    room_info = 1;
//repeated MSG_TABLE_PLAYER_INFO_NOTIFY player_info = 2;
//repeated OPERATE_INFO operate_info = 3;
//};
//
////单局战绩纪录
//message GAME_SINGLE_RECODE
//{
//	int32		cur_jushu = 1;		//当前局数
//	int32		winID = 2;		//赢家
//	repeated int32	single_score = 3;		//单局得分
//};

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

//子游戏房间规则
struct tagSubGameRule
{
	SCORE		Cellscore;                  //游戏底分(1/2/5)
	//WORD		Balancemode;                //结算方式(0:积分结算 1:金币结算)
	//WORD		GameChuPai;                 //游戏出牌(限时-->15秒出牌/25秒出牌/不限制出牌)
	WORD		GameWanFa;                  //游戏玩法(经典玩法/癞子玩法/天地癞子) 0 1 2 
	WORD		GameDiZhu;                  //游戏地主模式(0-抢地主 1-叫三分)
	SCORE		GameFengDing;               //游戏封顶模式(32/64/128/不封顶)
	//WORD		Preventcheat;               //防作弊
	WORD		DontCutCards;			    //不洗牌		 0-洗牌 1-不洗牌
	WORD		AddMultiple;                //是否加倍	 0-不加倍 1-加倍
	WORD		ShowCards;                  //是否明牌	 0-不名牌 1-明牌
	WORD		BaseCardAddMultiple;        //底牌是否翻倍 0-不翻倍 1-翻倍
};

//房间规则
struct tagTableCfg
{
	tagTableRule			*com_rule;		//通用房间规则
	tagSubGameRule			sub_rule;		//子游戏特有规则
};

# endif