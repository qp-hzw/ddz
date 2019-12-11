#ifndef CMD_OX_HEAD_FILE
#define CMD_OX_HEAD_FILE
#pragma pack(1)		//1×Ö½Ú¶ÔÆë
#include <windows.h>
#include "define.h"
#include "cardType.h"
#include <string>


#pragma region ÓÎÏ·ÅäÖÃ
//ÓÎÏ·ÊôÐÔÅäÖÃAttribute
#define KIND_ID								8												//Kind I D
#define NODE_ID								1												//Node ID
#define GAME_GENRE							(GAME_GENRE_GOLD|GAME_GENRE_MATCH)				//Ö§³ÖÀàÐÍ

//×î´ó/Ð¡ÒÎ×ÓÊý
#define MAX_CHAIR_COUNT						4			//×î´óÒÎ×ÓÊý ×¢: 1.¿Í»§¶Ë·¢ËÍµÄ·¿¼ä¹æÔòÖÐµÄ×î´óÒÎ×ÓÊý Ó¦¸ÃÐ¡ÓÚ´ËÊýÖµ  2.º¬»úÆ÷ÈË 3.º¬ÅÔ¹ÛÕß	
#define MIN_CHAIR_COUNT						3			//¿ªÊ¼ÓÎÏ·µÄ×îÐ¡ÒÎ×ÓÊý
#pragma endregion

#pragma region ·¢ËÍÏûÏ¢
/* *********************************************************************************
**										CMD·¢ËÍÏûÏ¢ºÅ
** *********************************************************************************/
#define CMD_SC_SEND_CARD							1							//¸øÍæ¼Ò·¢ÅÆ
#define CMD_SC_ROB_START							2							//ÇÀ×¯¿ªÊ¼£¨ÇÀµØÖ÷£©		
#define CMD_SC_ROB_RESULT							3    						//ÇÀ×¯½á¹û
#define CMD_SC_APPOINT_BANKER						4    						//Ö¸¶¨×¯¼Ò
#define CMD_SC_USER_SEND_LEAVE_CARD					5							//·¢ËÍµ×ÅÆ£¬²¢¸øËùÓÐÍæ¼Ò·¢ËÍÅÅÐòºóµÄÊÖÅÆÊý¾Ý
#define CMD_SC_MING_PAI_START						6							//Ã÷ÅÆ£¨³öÅÆ£©¿ªÊ¼
#define CMD_SC_MING_PAI_RESULT						7							//Ã÷ÅÆ£¨³öÅÆ£©½á¹û
#define CMD_SC_ADD_SCORE_START						8							//¼Ó×¢¿ªÊ¼£¨¼Ó±¶£©
#define CMD_SC_ADD_SCORE_RESULT						9							//¼Ó×¢½á¹û£¨¼Ó±¶£©
#define	CMD_SC_USER_OUT_CARD_START					10							//³öÅÆ¿ªÊ¼
#define CMD_SC_USER_OUT_CARD_RESULT					11							//³öÅÆ½á¹û
#define CMD_SC_XJ_GAME_END							12							//Ð¡¾ÖÓÎÏ·½áÊø
#define CMD_SC_DJ_GAME_END							13							//´ó¾ÖÓÎÏ·½áÊø
#define	CMD_SC_PUBLIC_BET							14							// ¹«¹²±¶Êý
#define	CMD_SC_JIAOFEN_START						15							// ½ÐÈý·Ö¿ªÊ¼
#define	CMD_SC_JIAOFEN_RESULT						16							// ½ÐÈý·Ö½á¹û
#define	CMD_SC_JIPAIQI_START						17							// ¼ÇÅÆÆ÷
#define	CMD_SC_FANGJIAN_BET							18							// ·¿¼ä±¶Êý
#define	CMD_SC_TUO_GUAN								19							//ÍÐ¹Ü
#define	CMD_SC_RECODE								27							//Â¼Ïñ

#define	CMD_SC_STATUS_FREE							20						// ÖØÁ¬--¿ÕÏÐ×´Ì¬
#define	CMD_SC_STATUS_OUTCARD						21						// ÖØÁ¬--³öÅÆ×´Ì¬
#define	CMD_SC_STATUS_XJ_END						22						// ÖØÁª--Ð¡¾Ö½áÊø
#define	CMD_SC_STATUS_ROB							23						// ÖØÁ¬--ÇÀ×¯×´Ì¬
#define	CMD_SC_STATUS_ADD_SCORE						24						// ÖØÁ¬--ÏÂ×¢×´Ì¬
#define	CMD_SC_STATUS_JIAOFEN						25						// ÖØÁ¬½Ð·Ö×´Ì¬
#define	CMD_SC_RULE									26						//¹æÔò


///////////////////////////////////////////////////////////////////////////////////
//·¢ÅÆÃüÁî
struct STR_CMD_SC_SEND_CARD
{
	WORD		wChairID;							//·¢ÅÆÍæ¼Ò
	BYTE		cbHandCardData[MAX_CARD_COUNT];		//ÊÖÅÆÊý¾Ý
	BYTE		cbHandCardNum;						//ÊÖÅÆ¸öÊý
	DWORD		CurJuShu;						//µ±Ç°¾ÖÊý
	DWORD		ISMingPaiStart;					//ÊÇ·ñÃ÷ÅÆ¿ªÊ¼  1-ÊÇ 0-²»ÊÇ
	//repeated Card  	   HandCardData;  	        //¸¨ÖúÂ¼Ïñ»Ø·ÅÊÖÅÆÊý¾Ý
	DWORD		GoldCoin[MAX_CHAIR_COUNT];				//½ð±Ò
	//DWORD		replay_code = 8;					//»Ø·ÅÂë
};
//struct Card				//Â¼Ïñ»Ø·ÅÊÇ·ñ»¹ÊÇÓÃÔ­À´µÄ
//{
//	repeated  CardData = 1;
//			   wChairID = 2;
//}

//ÇÀ×¯¿ªÊ¼
struct STR_CMD_SC_ROB_BANKER_START
{
	BYTE 		wChairID;					//ÇÀ×¯µÄÍæ¼Ò
	BYTE		cbType;						//Íæ¼ÒÇÀ×¯ÀàÐÍ	0-Î´¿ªÊ¼	1-½ÐµØÖ÷	2-ÇÀµØÖ÷
};

//ÇÀ×¯½á¹û
struct STR_CMD_SC_ROB_RESULT
{
	WORD		wChairID;					//ÒÎ×ÓºÅ
	BYTE		cbType;						//Íæ¼ÒÇÀ×¯ÀàÐÍ	0-Î´¿ªÊ¼	1-½ÐµØÖ÷	2-ÇÀµØÖ÷
	BYTE		cbRobState;					//Íæ¼ÒÇÀ×¯±êÖ¾	0-¹ý	1-½Ð/ÇÀ
	BYTE		room_bet[MAX_CHAIR_COUNT];					//·¿¼ä±¶Êý
};

//½ÐÈý·Ö¿ªÊ¼
struct STR_CMD_SC_JIAOFEN_START
{
	WORD		wChairID;		//½ÐÈý·ÖµÄÍæ¼Ò
	BYTE		ActionType;		//¶þ½øÖÆ¶¯×÷ÐÐÎª   0001-²»½Ð  0010-½ÐÒ»·Ö 0100-½Ð¶þ·Ö 1000-½ÐÈý·Ö	
};

//½ÐÈý·Ö½á¹û
struct STR_CMD_SC_JIAOFEN_RESULT
{
	WORD 		wChairID;						//ÒÎ×ÓºÅ
	BYTE		Score;							//Íæ¼Ò½»ÁË¶àÉÙ·Ö   0 1 2 3
	BYTE		room_bet[MAX_CHAIR_COUNT];		//¸÷Íæ¼ÒµÄ·¿¼ä±¶Êý	
};

//Ö¸¶¨×¯¼Ò
struct STR_CMD_SC_APPOINT_BANKER
{
	WORD		wBanker;					//×¯¼ÒÒÎ×ÓºÅ
};

//¸øÍæ¼Ò·¢ËÍµ×ÅÆ
struct STR_CMD_SC_SEND_LEAVE_CARD
{
	WORD		wSendCardUser;						//·¢ËÍÊ£Óàµ×ÅÆµÄÍæ¼Ò
	BYTE		cbSortedCard[MAX_CARD_COUNT];		//Íæ¼ÒÅÅÐòºóµÄÊÖÅÆ
	BYTE		cbHandCardNum;						//ÊÖÅÆ¸öÊý
	BYTE		cbMagicCardNum;						//ñ®×Ó¸öÊý£¨Ö÷ÅÆ´Ó×óÏòÓÒÅÅÐò£¬¿Í»§¶Ë´Ó0-cbMagicCardNumÎªÖ÷»¨É«£©
	BYTE		cbMagicCard;						//ñ®×Ó
	BYTE		cbLeaveCard[MAX_LEAVE_CARD_NUM];	//µ×ÅÆÊý¾Ý
	DWORD		LeaveCardBet;						//µ×ÅÆ±¶Êý
};

//ÏÂ×¢(¼Ó±¶)¿ªÊ¼
struct STR_CMD_SC_ADD_SCORE_START
{
	BYTE		cbStart;					//ÏÂ×¢¿ªÊ¼±êÊ¶	0-Î´¿ªÊ¼	1-¿ªÊ¼
};

//ÏÂ×¢½á¹û
struct STR_CMD_SC_ADD_SCORE_RESULT
{
	WORD                                wChairID;						//ÏÂ×¢Íæ¼Ò 
	WORD                                wAddSocre;						//¸Ã´ÎÏÂ×¢·ÖÖµ
	DWORD								RoomBet[MAX_CHAIR_COUNT];       //·¿¼ä±¶Êý
};

//Ã÷ÅÆ¿ªÊ¼
struct STR_CMD_SC_MING_PAI_START
{
	WORD                                wChairID;						//Ã÷ÅÆ¿ªÊ¼Íæ¼Ò 
	BYTE                                cbType;							//Ã÷ÅÆÀàÐÍ	3-·¢ÅÆ¿ªÊ¼Ã÷ÅÆ	2-¿ªÊ¼ÓÎÏ·Ã÷ÅÆ	1-³öÅÆ¿ªÊ¼Ã÷ÅÆ
};

//Ã÷ÅÆ½á¹û
struct STR_CMD_SC_MING_PAI_RESULT
{
	WORD                                wChairID;						//Ã÷ÅÆÍæ¼Ò 
	BYTE                                cbIsMing;						//ÊÇ·ñÃ÷ÅÆ	0-²»Ã÷ÅÆ	1-Ã÷ÅÆ
	BYTE								cbTimes;						//¼Ó±¶ºóµÄ±¶Êý
	BYTE								cbHandCard[MAX_CARD_COUNT];		//ÓÃ»§ÊÖÅÆ
	DWORD							    RoomBet[MAX_CHAIR_COUNT];		//·¿¼ä±¶Êý
};




////ÁÁÖ÷½á¹û
//struct STR_CMD_SC_LIANGZHU_RESULT
//{
//	WORD		wLiangzhuUser;						//ÁÁÖ÷Íæ¼Ò
//	BYTE		cbLZCard;							//ÁÁÖ÷¿¨ÅÆ
//};	
//
////·´Ö÷¿ªÊ¼
//struct STR_CMD_SC_FANZHU_NOTIFY
//{
//	WORD		wFanzhuUser;						//·´Ö÷Íæ¼Ò
//	BYTE		cbFanZhuCard[MAX_FANZHU_TYPE_NUM];	//·´Ö÷¿¨ÅÆ
//};	 
//
////·´Ö÷½á¹û
//struct STR_CMD_SC_FANZHU_RESULT
//{
//	WORD		wFanzhuUser;						//·´Ö÷Íæ¼Ò
//	BYTE		cbFanZhuCard;						//·´Ö÷¿¨ÅÆ
//};	 	
//
////¿Ûµ×¿ªÊ¼
//struct STR_CMD_SC_SEND_KOUDI_START
//{
//	WORD		wKouDiUser;						//¿Ûµ×Íæ¼Ò		
//	BYTE		cbKouDiNum;						//¿Ûµ×¿¨ÅÆÊýÄ¿;
//};	
//
////¿Ûµ×Íê³É
//struct STR_CMD_SC_SEND_KOUDI_FINISH
//{
//	WORD		wKouDiUser;						//¿Ûµ×Íæ¼Ò
//	BYTE		cbFinishMask;					//0-Î´Íê³É	1-Íê³É
//	BYTE		cbUserHandCard[MAX_CARD_COUNT];	//¿Ûµ×Íê³ÉºóµÄÍæ¼ÒÅÅÐòÊÖÅÆ
//	BYTE		cbMainColorNum;					//Ö÷ÅÆ¸öÊý£¨Ö÷ÅÆ´Ó×óÏòÓÒÅÅÐò£¬¿Í»§¶Ë´Ó0-cbMainColorNumÎªÖ÷»¨É«£©
//};	
//
////³öÅÆÌáÊ¾
//struct STR_CMD_SC_OUT_CARD_NOTIFY
//{
//	tagOutCardNotify		OutCardNotify;		//³öÅÆÌáÊ¾
//};	


//³öÅÆ¿ªÊ¼
struct STR_CMD_SC_OUT_CARD_START
{
	WORD		wOutCardUser;						//³öÅÆÌáÍæ¼Ò
	BYTE		ActionType;							//¶þ½øÖÆ¶¯×÷ÐÐÎª   //0001-³öÅÆ  0010-²»³ö  0100-ÌáÊ¾  1000-Òª²»Æð
	BYTE		TurnOutCardData[MAX_CARD_COUNT];				//µ±Ç°ÂÖ×î´ó³öÅÆÊý¾Ý
	DWORD		TurnOutCardNum;					//µ±Ç°ÂÖ×î´ó³öÅÆÊýÄ¿     //ÊÇ·ñÐèÒª
};	

//³öÅÆ½á¹û
struct STR_CMD_SC_OUT_CARD_RESULT
{
	BYTE		cbFlag;								//¹ýµÄ±êÊ¶	0-¹ý	1-Õý³£³öÅÆ
	BYTE		cbSuccess;							//ÊÇ·ñ³É¹¦	1-³öÅÆ³É¹¦  0-ÅÆÐÍ´íÎó
	WORD		wOutCardUser;						//³öÅÆÍæ¼Ò
	BYTE		cbOutCardNum;						//³öÅÆÊýÄ¿
	BYTE		cbOutCard[MAX_CARD_COUNT];			//³öÅÆÊý¾Ý
	BYTE		cbHandCardNum;						//ÊÖÅÆ¸öÊý
	BYTE		cbCardType;							//ÅÆÐÍ£¬ÅÆÐÍ´óÓÚ7£¬ÔòÎª½±£¬¿Í»§¶ËÏÔÊ¾½±µÃ·Ö
	SCORE		lBonusScore;						//½±µÃ·Ö£¬ÓÐ½±²ÅÓÐÖµ
	SCORE		lSingleGameScore[MAX_CHAIR_COUNT];	//Íæ¼Òµ¥¾ÖµÃ·Ö
	BYTE		handcarddata[MAX_CARD_COUNT];       //³öÅÆÍæ¼ÒÊÖÅÆÊý¾Ý
	DWORD		RoomBet[MAX_CHAIR_COUNT];			//·¿¼ä±¶Êý
};	

////Ò»ÂÖ»ý·Ö½áËã
//struct STR_CMD_SC_ROUND_BALANCE
//{
//	WORD		wRoundWinner;						//¸ÃÂÖÓ®¼Ò
//	WORD		wCurOutCardUser;					//µ±Ç°³öÅÆÍæ¼Ò
//	LONGLONG	lUserScore[MAX_CHAIR_COUNT];		//¸ÃÂÖÍæ¼Ò»ý·Ö
//	LONGLONG	lRoundScore;						//¸ÃÂÖ·ÖÊý£¬ÈôÊØ·Ö·½Ó®ÔòÊÇ0£¬µÃ·Ö·½Ó®ÔòÊÇ¾ßÌå·ÖÊý
//};

//Ð¡¾ÖÓÎÏ·½áÊø
struct STR_CMD_SC_XJ_GAME_END
{
	struct	XJ_END
	{ 
		WORD 		UsrChairID;							//Íæ¼Ò×ùÎ»ºÅ
		BYTE		cbLeaveHandCard[MAX_CARD_COUNT];	//Íæ¼ÒÊ£ÓàÊÖÅÆ
		SCORE		nTotalGameScore;  					//Íæ¼Ò×ÜµÃ·ÖÊý
		SCORE		nSingleGameScore;					//Íæ¼Òµ¥¾ÖµÃ·Ö
		SCORE		UserBet;							//Íæ¼Ò´Ë¾ÖµÄ±¶Êý
		SCORE		RoomSocre;							//½ð±Ò³¡·¿¼äµ×·Ö
		WORD		BankID;								//×¯¼Ò
		std::string		PlayerName;    					//Íæ¼ÒÃû×Ö
		DWORD		IsWin;								//Íæ¼ÒÊÇ·ñÓ®ÁË  0-°Ü  1-Ê¤
		DWORD		IsEnd;								//´ó¾ÖÊÇ·ñ½áÊø    0-Î´½áÊø  1-½áÊø
	};
	struct XJ_END		xj_end[MAX_CHAIR_COUNT];
	WORD				wchairID;
};

//´ó¾ÖÓÎÏ·½áÊø
struct STR_CMD_SC_DJ_GAME_END
{
	WORD							wMaxWinChairID;							// Ó®´ÎÊý×î¶àµÄÍæ¼ÒÒÎ×ÓºÅ
	WORD							wFangzhuID;								// ·¿Ö÷ID
	SCORE							lUserScore[MAX_CHAIR_COUNT];			// Íæ¼Ò×ÜµÃ·Ö 
	BYTE                            bWinTime[MAX_CHAIR_COUNT];				// Ê¤Àû´ÎÊý
	WORD							BankTime[MAX_CHAIR_COUNT];				//µØÖ÷´ÎÊý

	struct PlayerInfo
	{
		std::string		playerName;    //Ãû×Ö
		WORD			playerID;   		//ID
		std::string		szFaceUrl;			//Í·Ïñ¿ò
	};

	struct PlayerInfo	playerinfo[MAX_CHAIR_COUNT];      			//Íæ¼ÒÐÅÏ¢
	SCORE				PlayerBigestBet[MAX_CHAIR_COUNT];			//Íæ¼ÒÔø»ñµÃ¹ýµÄ×î´ó±¶Êý
	WORD				IsEnd;												//ÊÇ·ñ½áÊø  0-½áÊø  1-Î´½áÊø
	WORD				Rich_ID[MAX_CHAIR_COUNT];					//´óÍÁºÀµÄÒÎ×ÓºÅ ÊäµÃ×î¶àµÄ
};

//ÓÎÏ·½áÊø ¹«¹²±¶Êý
struct STR_CMD_SC_PUBLIC_BET
{
	SCORE		RoomBet;			//µ×·Ö
	SCORE		MingPai;			//Ã÷ÅÆ
	SCORE		Rob;				//ÇÀµØÖ÷
	SCORE		LeaveCard;			//µ×ÅÆ
	SCORE		Boom;				//Õ¨µ¯
	SCORE 		Spring;			//´ºÌì
	SCORE		AddScore[MAX_CHAIR_COUNT];			//¼Ó±¶
	SCORE		IUserAScore;			//×Ü·Ö 
	WORD		Game_State;			//µ±Ç°ÓÎÏ·×´Ì¬
	WORD		bank_ID;			//×¯¼ÒID
	SCORE		public_bet;			//¹«¹²±¶Êý
	WORD		Rule;			//µ±Ç°ÓÎÏ·¹æÔò  0-ÇÀ×¯  1-½Ð·Ö
};

//¼ÇÅÆÆ÷
struct STR_CMD_SC_JIPAIQI_START
{
	WORD		JiPaiQi[MAX_CARD_COUNT];			//¼ÇÅÆÆ÷
};

//·¿¼ä±¶Êý
struct STR_CMD_SC_FANGJIAN_BET
{
	SCORE	room_bet[MAX_CHAIR_COUNT];
};

//ÍÐ¹Ü
struct STR_CMD_SC_TUO_GUAN
{
	WORD	tuo_guan;	//´«1±íÊ¾ÒÑ×Ô¶¯ÍÐ¹Ü
};

//rule
struct STR_CMD_SC_RULE
{
	WORD	rule;   // 0-jingdian   1-buxip
};

//Â¼Ïñ»Ø·Å
struct STR_CMD_SC_RECODE
{
	std::string		ReplayData;			//Â¼Ïñ»Ø·ÅÊý¾Ý
};

#pragma endregion

#pragma region ½ÓÊÕÏûÏ¢
/* *********************************************************************************
**										SUB½ÓÊÕÏûÏ¢
** *********************************************************************************/
#define SUB_CS_ROB_BANKER							1							//ÇÀ×¯
#define SUB_CS_MING_PAI								2							//Ã÷ÅÆ
#define SUB_CS_ADD_SCORE							3							//¼Ó×¢£¨¼Ó±¶£©
#define SUB_CS_OUT_CARD								4							//³öÅÆ
#define SUB_CS_XJ_GAME_READY 						5							//Ð¡¾ÖÓÎÏ·×¼±¸
#define	SUB_CS_PUBLIC_BET							106							//´ó¾Ö½áÊø¹«¹²±¶Êý
#define	SUB_CS_JIAO_FEN								107							//½Ð·Ö
#define	SUB_CS_JIPAIQI								108							//¼ÇÅÆÆ÷
#define	SUB_CS_TUO_GUAN								109							//ÍÐ¹Ü

//#define SUB_CS_LIANG_ZHU							1							//ÁÁÖ÷
//#define SUB_CS_FAN_ZHU								2							//·´Ö÷
//#define SUB_CS_KOU_DI								3							//¿Ûµ×
/////////////////////////////////////////////////////////////////////////////////////////////

//ÇÀ×¯ÃüÁî
struct STR_SUB_CS_ROB_BANKER
{
	BYTE		cbType;						//Íæ¼ÒÇÀ×¯ÀàÐÍ	0-½ÐµØÖ÷	1-ÇÀµØÖ÷	
	BYTE		cbRobState;					//ÇÀ×¯×´Ì¬		0-¹ý	1-½Ð/ÇÀ	
};

//½ÐÈý·ÖÃüÁî
struct STR_SUB_CS_JIAO_FEN
{
	SCORE		Score;							//Íæ¼Ò½ÐÁË¶àÉÙ·Ö   0-²»½Ð  1-1·Ö 2-2·Ö 3-3·Ö
};

//Íæ¼ÒÏÂ×¢  
struct STR_SUB_CS_ADD_SCORE
{
	DWORD		score;						//ÏÂ×¢·ÖÖµ	0-´ú±í²»ÏÂ×¢
};

//Íæ¼ÒÃ÷ÅÆ
struct STR_SUB_CS_MING_PAI
{
	BYTE        cbType;						//Ã÷ÅÆÀàÐÍ	2-·¢ÅÆ¿ªÊ¼Ã÷ÅÆ	1-³öÅÆ¿ªÊ¼Ã÷ÅÆ
	BYTE		cbIsMing;					//ÊÇ·ñÃ÷ÅÆ	0-²»Ã÷ÅÆ	1-Ã÷ÅÆ
	SCORE		OutCard_bet;				//·¢ÅÆÃ÷ÅÆµÄ±¶Êý
};


////ÁÁÖ÷
//struct STR_SUB_CS_LIANG_ZHU
//{
//	WORD		wLiangZhuUser;		//ÁÁÖ÷Íæ¼Ò
//	BYTE		cbLiangZhuColor;	//ÁÁÖ÷»¨É«
//};
//
////·´Ö÷
//struct STR_SUB_CS_FAN_ZHU
//{
//	BYTE		cbFlag;				//·´Ö÷±êÊ¶	0-Íæ¼Ò·ÅÆú·´Ö÷	1-Íæ¼Ò·´Ö÷
//	WORD		wFanZhuUser;		//·´Ö÷Íæ¼Ò
//	BYTE		cbFanZhuCard;		//·´Ö÷¿¨ÅÆ
//};

////¿Ûµ×
//struct STR_SUB_CS_KOUDI
//{
//	WORD		wKouDiUser;							//¿Ûµ×Íæ¼Ò
//	BYTE		cbKouDiCard[MAX_LEAVE_CARD_NUM];	//¿Ûµ×¿¨ÅÆ
//};

//³öÅÆ
struct STR_SUB_CS_OUT_CARD
{
	BYTE		cbPass;								//¹ý	0-Õý³£³öÅÆ	1-¹ý
	WORD		wOutCardUser;						//³öÅÆÍæ¼Ò
	BYTE		cbOutCardNum;						//³öÅÆÊýÄ¿
	BYTE		cbOutCard[MAX_CARD_COUNT];			//³öÅÆÊý¾Ý
};

//Ð¡¾ÖÓÎÏ·×¼±¸
struct STR_SUB_CS_XJ_USER_READY
{
	WORD		wReadyUser;							//×¼±¸Íæ¼Ò
	BYTE		cbReady;							//Ð¡¾ÖÓÎÏ·ÓÃ»§×¼±¸	1-×¼±¸£»0-Î´×¼±¸
	SCORE		cbBet;								//Ã÷ÅÆ¿ªÊ¼±¶Êý
};

//ÓÎÏ·½áÊø ¹«¹²±¶Êý
struct STR_SUB_CS_PUBLIC_BET
{
	WORD		wChairID;			//Òª²é¿´µÄÒÎ×ÓºÅ
};

//¼ÇÅÆÆ÷
struct STR_SUB_CS_JIPAIQI
{
	WORD tmp;			//²»Òª´«0
};

//ÍÐ¹Ü
struct STR_SUB_CS_TUO_GUAN
{
	WORD	TuoGuan_state;		//1-ÉèÖÃÍÐ¹Ü   0-È¡ÏûÍÐ¹Ü
};

#pragma endregion

#pragma region ¶ÏÏßÖØÁ¬
//¿ÕÏÐ×´Ì¬
struct STR_CMD_SC_STATUS_FREE
{
	BYTE	GameCount;				//ÓÎÏ·¾ÖÊý 0-ÎÞÏÞ¾Ö
	BYTE	PlayerCount;			//ÓÎÏ·ÈËÊý
	WORD	CellScore;				//µ×·Ö	  Ä¬ÈÏÎª1
	DWORD	FangZhu;				//·¿Ö÷    ´´½¨ÓÃ»§
};

////ÁÁÖ÷×´Ì¬
//struct STR_CMD_SC_STATUS_LIANGZHU
//{
//	BYTE	cbHandCard[MAX_CARD_COUNT];			//Íæ¼ÒÊÖÅÆ
//	BYTE	cbCardNum;							//Íæ¼ÒÊÖÅÆ×ÜÊý
//	BYTE	cbLastTime;							//ÁÁÖ÷¶¨Ê±Æ÷Ê£ÓàÊ±¼ä£¨Ãë£©
//};
//
////·´Ö÷×´Ì¬
//struct STR_CMD_SC_STATUS_FANZHU
//{
//	WORD	wCurFanzhuUser;						//µ±Ç°·´Ö÷Íæ¼Ò
//	BYTE	cbFanZhuCard[MAX_FANZHU_TYPE_NUM];	//Íæ¼Ò×Ô¼ºµÄ·´Ö÷¿¨ÅÆ£¬µ±Ç°Íæ¼ÒÊÇ×Ô¼ºµÄÊ±ºò²ÅÏÔÊ¾·´Ö÷ÌáÊ¾
//	BYTE	cbLastTime;							//·´Ö÷¶¨Ê±Æ÷Ê£ÓàÊ±¼ä£¨Ãë£©
//};
//
////¿Ûµ××´Ì¬
//struct STR_CMD_SC_STATUS_KOUDI
//{
//	WORD		wCurKouDiUser;						//µ±Ç°¿Ûµ×Íæ¼Ò
//	BYTE		cbSortedCard[MAX_CARD_COUNT];		//Íæ¼ÒÅÅÐòºóµÄÊÖÅÆ
//	BYTE		cbHandCardNum;						//ÊÖÅÆ¸öÊý
//	BYTE		cbMainColorNum;						//Ö÷ÅÆ¸öÊý£¨Ö÷ÅÆ´Ó×óÏòÓÒÅÅÐò£¬¿Í»§¶Ë´Ó0-cbMainColorNumÎªÖ÷»¨É«£©
//	BYTE		cbMainColor;						//µ±Ç°Ö÷»¨É«
//};


//ÇÀ×¯×´Ì¬
struct STR_CMD_SC_STATUS_ROB
{
	WORD			CurRobUsr;					//µ±Ç°ÕýÔÚÇÀ×¯µÄÍæ¼Ò
	WORD 			robstate[MAX_CHAIR_COUNT];	//¸÷Íæ¼ÒµÄÇÀ×¯×´Ì¬
	BYTE			HandCardData[MAX_CARD_COUNT];	//µ±Ç°Íæ¼ÒµÄÊÖÅÆ
	DWORD			HandCardNum[MAX_CHAIR_COUNT];	//¸÷Íæ¼ÒµÄÊÖÅÆ¸öÊý
	WORD			CurJuShu;		//µ±Ç°·¿¼ä¾ÖÊý
	SCORE			room_bet[MAX_CHAIR_COUNT];	//µ±Ç°·¿¼ä±¶Êý
	SCORE			All_bet[MAX_CHAIR_COUNT];	//×Ü»ý·Ö
	SCORE			GoldCoin[MAX_CHAIR_COUNT];	//¸÷Íæ¼ÒµÄ½ð±Ò
	DWORD		    replay_code;			//»Ø·ÅÂë
};

//½Ð·Ö×´Ì¬
struct STR_CMD_SC_STATUS_JIAOFEN
{
	WORD	   CurRobUsr;            //µ±Ç°ÕýÔÚ½Ð·ÖµÄÍæ¼Ò
	WORD	   robstate[MAX_CHAIR_COUNT];     //¸÷Íæ¼ÒµÄ½Ð·Ö×´Ì¬
	BYTE	   HandCardData[MAX_CARD_COUNT];         //µ±Ç°Íæ¼ÒµÄÊÖÅÆ
	DWORD	   HandCardNum[MAX_CHAIR_COUNT];  //¸÷Íæ¼ÒµÄÊÖÅÆ¸öÊý
	DWORD	   CurJuShu;             //µ±Ç°·¿¼ä¾ÖÊý
	SCORE	   room_bet[MAX_CHAIR_COUNT];     //µ±Ç°·¿¼ä±¶Êý
	SCORE	   All_bet[MAX_CHAIR_COUNT];      //×Ü»ý·Ö
	SCORE	   GoldCoin[MAX_CHAIR_COUNT];	//¸÷Íæ¼ÒµÄ½ð±Ò
	DWORD	   replay_code;			//»Ø·ÅÂë
};


//ÏÂ×¢×´Ì¬
struct STR_CMD_SC_STATUS_ADD_SCORE
{
	WORD		scorestate[MAX_CHAIR_COUNT];	//¸÷Íæ¼ÒµÄÏÂ×¢×´Ì¬
	BYTE		HandCardData[MAX_CARD_COUNT];	//µ±Ç°Íæ¼ÒµÄÊÖÅÆ
	DWORD		HandCardNum[MAX_CHAIR_COUNT];	//¸÷Íæ¼ÒµÄÊÖÅÆ¸öÊý
	DWORD		CurJuShu;			             //µ±Ç°·¿¼ä¾ÖÊý
	SCORE 		room_bet[MAX_CHAIR_COUNT];	//µ±Ç°·¿¼äµÄ±¶Êý
	WORD		bankID;		//×¯¼ÒID
	SCORE		All_bet[MAX_CHAIR_COUNT];	//×Ü»ý·Ö
	SCORE 		GoldCoin[MAX_CHAIR_COUNT];	//¸÷Íæ¼ÒµÄ½ð±Ò
	DWORD		replay_code;			//»Ø·ÅÂë
};

struct player_op_info
{
   WORD					op_type;    //²Ù×÷ÀàÐÍ 0-¹ý 1-³öÅÆ
   BYTE 				op_result[MAX_CARD_COUNT]; //³öÅÆ½á¹û, ÅÆÊý¾Ý; Ö»ÔÚop_type=1Ê±²ÅÓÐÐ§
   DWORD				op_cardscount;   //µ±Ç°ÅÆÊýÁ¿
   BYTE					MingPaiCardData[MAX_CARD_COUNT];   //Ã÷ÅÆÍæ¼ÒµÄÊÖÅÆ
};

//³öÅÆ×´Ì¬
struct STR_CMD_SC_STATUS_OUTCARD
{
	struct	player_op_info		players_op[MAX_CHAIR_COUNT];     //Íæ¼ÒÐÅÏ¢
	WORD						outcardid;						//µ±Ç°³öÅÆÍæ¼Ò
	BYTE 						brokenoutcarddata[MAX_CARD_COUNT];					//×Ô¼ºµÄÊÖÅÆÊý¾Ý
	WORD						CurJuShu;             //µ±Ç°·¿¼ä¾ÖÊý
	SCORE 						room_bet[MAX_CHAIR_COUNT];		//µ±Ç°·¿¼äµÄ±¶Êý
	WORD						bankID;		//×¯¼ÒID
	WORD 						MingPaiState[MAX_CHAIR_COUNT];		//¸÷Íæ¼ÒµÄÃ÷ÅÆ×´Ì¬
	BYTE 						LeaveCard[MAX_LEAVE_CARD_NUM];		//µ×ÅÆ
	SCORE						All_bet[MAX_CHAIR_COUNT];		//×Ü»ý·Ö
	BYTE						TurnCardData[MAX_CARD_COUNT];	//µ±Ç°ÂÖ×î´ó³öÅÆ
	SCORE 						GoldCoin[MAX_CHAIR_COUNT];	//¸÷Íæ¼ÒµÄ½ð±Ò
	WORD						Add_bet[MAX_CHAIR_COUNT];		//Íæ¼ÒÊÇ·ñ¼Ó±¶
	SCORE						Leave_bet;		//µ×ÅÆ±¶Êý
	DWORD					    replay_code;			//»Ø·ÅÂë
};


//Ð¡¾Ö½áËã×´Ì¬
struct STR_CMD_SC_STATUS_XJ_END
{
	SCORE 		nSingleGameScore[MAX_CHAIR_COUNT];				//Íæ¼Òµ¥¾ÖµÃ·Ö	
};

//×ÓÓÎÏ··¿¼ä¹æÔò
struct STR_CMD_SUB_ROOM_RULE
{
	WORD		Gamejushu;                  //ÓÎÏ·¾ÖÊý
	WORD		Cellscore;                   //ÓÎÏ·µ×·Ö(1/2/5)
	WORD		Balancemode;                //½áËã·½Ê½(0:»ý·Ö½áËã 1:½ð±Ò½áËã)
	WORD		GameChuPai;                 //ÓÎÏ·³öÅÆ(ÏÞÊ±-->15Ãë³öÅÆ/25Ãë³öÅÆ/²»ÏÞÖÆ³öÅÆ)
	WORD		GameWanFa;                  //ÓÎÏ·Íæ·¨(¾­µäÍæ·¨/ñ®×ÓÍæ·¨/ÌìµØñ®×Ó)
	WORD		GameDiZhu;                  //ÓÎÏ·µØÖ÷Ä£Ê½(0-ÇÀµØÖ÷ 1-½ÐÈý·Ö)
	WORD		GameFengDing;               //ÓÎÏ··â¶¥Ä£Ê½(32/64/128/²»·â¶¥)
	WORD		Preventcheat;               //·À×÷±×
	WORD		DontCutCards;               //²»Ï´ÅÆ
	WORD		AddMultiple;               //¼Ó±¶
	WORD		ShowCards;                 //Ã÷ÅÆ
	WORD		BaseCardAddMultiple;       //µ×ÅÆ·­±¶
	WORD		GameMode;                  //ÓÎÏ·µÃ·ÖÄ£Ê½   0-¾­µäÄ£Ê½  1-·è¿ñÄ£Ê½ 
};

////Â¼Ïñ»Ø·Å½á¹¹Ìå
//message GAME_REPLAY
//{
//	//×ÓÓÎÏ··¿¼ä¹æÔò
//	message MSG_SUB_ROOM_RULE
//	{
//			int32 Gamejushu = 1;                  //ÓÎÏ·¾ÖÊý
//		int32 Cellscore = 2;                   //ÓÎÏ·µ×·Ö(1/2/5)
//		int32 Balancemode = 3;                //½áËã·½Ê½(0:»ý·Ö½áËã 1:½ð±Ò½áËã)
//		int32 GameChuPai = 4;                 //ÓÎÏ·³öÅÆ(ÏÞÊ±-->15Ãë³öÅÆ/25Ãë³öÅÆ/²»ÏÞÖÆ³öÅÆ)
//		int32 GameWanFa = 5;                  //ÓÎÏ·Íæ·¨(¾­µäÍæ·¨/ñ®×ÓÍæ·¨/ÌìµØñ®×Ó)
//		int32 GameDiZhu = 6;                  //ÓÎÏ·µØÖ÷Ä£Ê½(0-ÇÀµØÖ÷ 1-½ÐÈý·Ö)
//		int32 GameFengDing = 7;               //ÓÎÏ··â¶¥Ä£Ê½(32/64/128/²»·â¶¥)
//		int32 Preventcheat = 8;               //·À×÷±×
//		int32 DontCutCards = 9;               //²»Ï´ÅÆ
//		int32 AddMultiple = 10;               //¼Ó±¶
//		int32 ShowCards = 11;                 //Ã÷ÅÆ
//		int32 BaseCardAddMultiple = 12;       //µ×ÅÆ·­±¶
//		int32 GameMode = 13;                  //ÓÎÏ·µÃ·ÖÄ£Ê½   0-¾­µäÄ£Ê½  1-·è¿ñÄ£Ê½
//	};
//
////Íæ¼ÒÐÅÏ¢
//message MSG_TABLE_PLAYER_INFO_NOTIFY
//{
//		uint32 messageid = 1;
//		uint64  player_id = 2;                  //Íæ¼ÒID
//		string name = 3;                    //Íæ¼ÒÃû×Ö
//		int64  registerDate = 4;                //×¢²áÊ±¼ä
//		uint32 sex = 5;                    //ÐÔ±ð
//		int32 userstate = 6;                  //Íæ¼Ò×´Ì¬
//		int32 tableposid = 7;                //×ùÎ»ºÅ
//		int32 isonline = 8;                  //ÔÚÏß×´Ì¬£º1£ºÎªÔÚÏß0£º²»ÔÚÏß
//		int64 playergold = 9;                //½ð±Ò
//		string faceurl = 10;                  //Í¼ÏñµØÖ·
//};
//
//
////²Ù×÷ÐÅÏ¢
//message OPERATE_INFO
//{
//	MSG_M2C_SEND_CARD 			send_card = 1;		//·¢ÅÆ
//	MSG_M2C_ROB_RESULT 			rob_result = 2;		//ÇÀ×¯½á¹û
//	MSG_M2C_APPOINT_BANKER 				appoint_banker = 3;	//Ö¸¶¨×¯¼Ò
//	MSG_M2C_SEND_LEAVE_CARD		send_leave_card = 4;	//·¢ËÍµ×ÅÆ
//	MSG_M2C_MING_PAI_RESULT		mingpai_result = 5;	//Ã÷ÅÆ½á¹û
//	MSG_M2C_ADD_SCORE_RESULT		addscore_result = 6;	//¼Ó±¶½á¹û
//	MSG_M2C_OUT_CARD_RESULT		outcard_result = 7;	//³öÅÆ½á¹û
//	MSG_M2C_XJ_GAME_END			xj_end = 8;		//Ð¡¾Ö½áÊø
//	MSG_M2C_DJ_GAME_END				dj_end = 9;		//´ó¾Ö½áÊø
//	MSG_M2C_JIAOFEN_RESULT			jiaosanfen_result = 10;	//½ÐÈý·Ö½á¹û	
//};
//
//MSG_SUB_ROOM_RULE    room_info = 1;
//repeated MSG_TABLE_PLAYER_INFO_NOTIFY player_info = 2;
//repeated OPERATE_INFO operate_info = 3;
//};
//
////µ¥¾ÖÕ½¼¨¼ÍÂ¼
//message GAME_SINGLE_RECODE
//{
//	int32		cur_jushu = 1;		//µ±Ç°¾ÖÊý
//	int32		winID = 2;		//Ó®¼Ò
//	repeated int32	single_score = 3;		//µ¥¾ÖµÃ·Ö
//};

//***********************************************************************************//
#pragma endregion

#pragma region ´ý´¦Àí -- »úÆ÷ÈË
///////////////////////////////////////////////////////////////////////////////////////////////////
// »úÆ÷ÈËÊ¹ÓÃ, ÔÝÊ±Î´ÓÃµ½ added by WangChengQing 2017.9.21 TODONOW ÒÔºóÓ¦¸ÃÔÚÆåÅÆÖÐÔö¼Ó»úÆ÷ÈË¹¦ÄÜ
///////////////////////////////////////////////////////////////////////////////////////////////////
//·þÎñÆ÷ÃüÁî½á¹¹    --  »úÆ÷ÈË
#define SUB_S_A_ADD_SCORE					(0x0008)									//¼Ó×¢½á¹û
#define SUB_S_A_PLAYER_EXIT				(0x0010)									//ÓÃ»§Ç¿ÍË
#define SUB_S_A_GAME_END					(0x0040)									//ÓÎÏ·½áÊø
#define SUB_S_A_OPEN_CARD					(0x0080)									//ÓÃ»§Ì¯ÅÆ
#define SUB_S_A_CALL_BANKER				(0x0100)									//ÓÃ»§½Ð×¯
#define SUB_S_A_CALL						(0x0400)									//ÓÃ»§½Ð×¯

//Íæ¼Òflash ID ºÍ ¶¯×÷ID
struct CMD_S_PlayerAction
{
	WORD wChairID;
	int iActionID;
};
//Íæ¼Ò ¶¯×÷ID
struct CMD_C_PlayerAction
{
	int iActionID;
};


//ÓÎÏ·×´Ì¬
struct CMD_S_StatusCall
{
	WORD							    	wCallBanker;						//½Ð×¯ÓÃ»§
	BYTE							        cbPlayStatus[MAX_CHAIR_COUNT];          //ÓÃ»§×´Ì¬
	BYTE									cbCallStatus[MAX_CHAIR_COUNT];			//½Ð×¯×´Ì¬
	bool									bCallBanker[MAX_CHAIR_COUNT];			//ÊÇ·ñ½Ð×¯
	SCORE									lCellScore;							//»ù´¡»ý·Ö
	SCORE									lRevenue;
	BYTE									cbTimeLeave;
	SCORE									lPlayerScore[MAX_CHAIR_COUNT];			//Íæ¼Ò·ÖÊý
};

struct CMD_S_PlayerCount
{
	BYTE									wPlayerCount;
	SCORE									lUserScore[MAX_CHAIR_COUNT];			//Íæ¼Ò»ý·Ö
};
//
////ÓÃ»§½Ð×¯
struct CMD_S_CallBanker
{
	WORD							     	wCallBanker;						//½Ð×¯ÓÃ»§
	bool							    	bFirstTimes;						//Ê×´Î½Ð×¯
};

////////////////////////////////////////////////////////////////////////////
//¿Í»§¶ËÃüÁî½á¹¹             --  »úÆ÷ÈË
#define SUB_C_A_CALL_BANKER				1									//ÓÃ»§½Ð×¯
#define SUB_C_A_ADD_SCORE					2									//ÓÃ»§¼Ó×¢
#define SUB_C_A_OPEN_CARD					3									//ÓÃ»§Ì¯ÅÆ
#define SUB_C_A_AMDIN_COMMAND				5									//¹ÜÀíÔ±ÃüÁî
#define SUB_C_A_AMDIN_CHANGE_CARD			6									//¹ÜÀíÔ±ÃüÁî
#define SUB_C_A_READY_MESSAGE				7								    //¿Í»§¶Ë·¢ËÍ×¼±¸ÏûÏ¢

/////////-----5
struct CMD_C_Admin_ChangeCard
{
	BYTE cbStart;
	BYTE cbEnd;

};

# endif

//lD6-Åe?—¨<p??~?,?B€Ï[k›ùîkñH£›ocO˜èÃ¨‡ÕKH^áèÖb?ûÉÿ¶‹*Ø‰ªÑJ»sÍa¼ß?
//Y?.Ö’§ÜXKØsr ]œÎGVM?ƒE8Äèü€6šgÊ Q7Íì?ÝÁpe¥¡