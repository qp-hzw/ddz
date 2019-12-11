#ifndef CMD_OX_HEAD_FILE
#define CMD_OX_HEAD_FILE
#pragma pack(1)		//1�ֽڶ���
#include <windows.h>
#include "define.h"
#include "cardType.h"
#include <string>


#pragma region ��Ϸ����
//��Ϸ��������Attribute
#define KIND_ID								8												//Kind I D
#define NODE_ID								1												//Node ID
#define GAME_GENRE							(GAME_GENRE_GOLD|GAME_GENRE_MATCH)				//֧������

//���/С������
#define MAX_CHAIR_COUNT						4			//��������� ע: 1.�ͻ��˷��͵ķ�������е���������� Ӧ��С�ڴ���ֵ  2.�������� 3.���Թ���	
#define MIN_CHAIR_COUNT						3			//��ʼ��Ϸ����С������
#pragma endregion

#pragma region ������Ϣ
/* *********************************************************************************
**										CMD������Ϣ��
** *********************************************************************************/
#define CMD_SC_SEND_CARD							1							//����ҷ���
#define CMD_SC_ROB_START							2							//��ׯ��ʼ����������		
#define CMD_SC_ROB_RESULT							3    						//��ׯ���
#define CMD_SC_APPOINT_BANKER						4    						//ָ��ׯ��
#define CMD_SC_USER_SEND_LEAVE_CARD					5							//���͵��ƣ�����������ҷ�����������������
#define CMD_SC_MING_PAI_START						6							//���ƣ����ƣ���ʼ
#define CMD_SC_MING_PAI_RESULT						7							//���ƣ����ƣ����
#define CMD_SC_ADD_SCORE_START						8							//��ע��ʼ���ӱ���
#define CMD_SC_ADD_SCORE_RESULT						9							//��ע������ӱ���
#define	CMD_SC_USER_OUT_CARD_START					10							//���ƿ�ʼ
#define CMD_SC_USER_OUT_CARD_RESULT					11							//���ƽ��
#define CMD_SC_XJ_GAME_END							12							//С����Ϸ����
#define CMD_SC_DJ_GAME_END							13							//�����Ϸ����
#define	CMD_SC_PUBLIC_BET							14							// ��������
#define	CMD_SC_JIAOFEN_START						15							// �����ֿ�ʼ
#define	CMD_SC_JIAOFEN_RESULT						16							// �����ֽ��
#define	CMD_SC_JIPAIQI_START						17							// ������
#define	CMD_SC_FANGJIAN_BET							18							// ���䱶��
#define	CMD_SC_TUO_GUAN								19							//�й�
#define	CMD_SC_RECODE								27							//¼��

#define	CMD_SC_STATUS_FREE							20						// ����--����״̬
#define	CMD_SC_STATUS_OUTCARD						21						// ����--����״̬
#define	CMD_SC_STATUS_XJ_END						22						// ����--С�ֽ���
#define	CMD_SC_STATUS_ROB							23						// ����--��ׯ״̬
#define	CMD_SC_STATUS_ADD_SCORE						24						// ����--��ע״̬
#define	CMD_SC_STATUS_JIAOFEN						25						// �����з�״̬
#define	CMD_SC_RULE									26						//����


///////////////////////////////////////////////////////////////////////////////////
//��������
struct STR_CMD_SC_SEND_CARD
{
	WORD		wChairID;							//�������
	BYTE		cbHandCardData[MAX_CARD_COUNT];		//��������
	BYTE		cbHandCardNum;						//���Ƹ���
	DWORD		CurJuShu;						//��ǰ����
	DWORD		ISMingPaiStart;					//�Ƿ����ƿ�ʼ  1-�� 0-����
	//repeated Card  	   HandCardData;  	        //����¼��ط���������
	DWORD		GoldCoin[MAX_CHAIR_COUNT];				//���
	//DWORD		replay_code = 8;					//�ط���
};
//struct Card				//¼��ط��Ƿ�����ԭ����
//{
//	repeated  CardData = 1;
//			   wChairID = 2;
//}

//��ׯ��ʼ
struct STR_CMD_SC_ROB_BANKER_START
{
	BYTE 		wChairID;					//��ׯ�����
	BYTE		cbType;						//�����ׯ����	0-δ��ʼ	1-�е���	2-������
};

//��ׯ���
struct STR_CMD_SC_ROB_RESULT
{
	WORD		wChairID;					//���Ӻ�
	BYTE		cbType;						//�����ׯ����	0-δ��ʼ	1-�е���	2-������
	BYTE		cbRobState;					//�����ׯ��־	0-��	1-��/��
	BYTE		room_bet[MAX_CHAIR_COUNT];					//���䱶��
};

//�����ֿ�ʼ
struct STR_CMD_SC_JIAOFEN_START
{
	WORD		wChairID;		//�����ֵ����
	BYTE		ActionType;		//�����ƶ�����Ϊ   0001-����  0010-��һ�� 0100-�ж��� 1000-������	
};

//�����ֽ��
struct STR_CMD_SC_JIAOFEN_RESULT
{
	WORD 		wChairID;						//���Ӻ�
	BYTE		Score;							//��ҽ��˶��ٷ�   0 1 2 3
	BYTE		room_bet[MAX_CHAIR_COUNT];		//����ҵķ��䱶��	
};

//ָ��ׯ��
struct STR_CMD_SC_APPOINT_BANKER
{
	WORD		wBanker;					//ׯ�����Ӻ�
};

//����ҷ��͵���
struct STR_CMD_SC_SEND_LEAVE_CARD
{
	WORD		wSendCardUser;						//����ʣ����Ƶ����
	BYTE		cbSortedCard[MAX_CARD_COUNT];		//�������������
	BYTE		cbHandCardNum;						//���Ƹ���
	BYTE		cbMagicCardNum;						//��Ӹ��������ƴ����������򣬿ͻ��˴�0-cbMagicCardNumΪ����ɫ��
	BYTE		cbMagicCard;						//���
	BYTE		cbLeaveCard[MAX_LEAVE_CARD_NUM];	//��������
	DWORD		LeaveCardBet;						//���Ʊ���
};

//��ע(�ӱ�)��ʼ
struct STR_CMD_SC_ADD_SCORE_START
{
	BYTE		cbStart;					//��ע��ʼ��ʶ	0-δ��ʼ	1-��ʼ
};

//��ע���
struct STR_CMD_SC_ADD_SCORE_RESULT
{
	WORD                                wChairID;						//��ע��� 
	WORD                                wAddSocre;						//�ô���ע��ֵ
	DWORD								RoomBet[MAX_CHAIR_COUNT];       //���䱶��
};

//���ƿ�ʼ
struct STR_CMD_SC_MING_PAI_START
{
	WORD                                wChairID;						//���ƿ�ʼ��� 
	BYTE                                cbType;							//��������	3-���ƿ�ʼ����	2-��ʼ��Ϸ����	1-���ƿ�ʼ����
};

//���ƽ��
struct STR_CMD_SC_MING_PAI_RESULT
{
	WORD                                wChairID;						//������� 
	BYTE                                cbIsMing;						//�Ƿ�����	0-������	1-����
	BYTE								cbTimes;						//�ӱ���ı���
	BYTE								cbHandCard[MAX_CARD_COUNT];		//�û�����
	DWORD							    RoomBet[MAX_CHAIR_COUNT];		//���䱶��
};




////�������
//struct STR_CMD_SC_LIANGZHU_RESULT
//{
//	WORD		wLiangzhuUser;						//�������
//	BYTE		cbLZCard;							//��������
//};	
//
////������ʼ
//struct STR_CMD_SC_FANZHU_NOTIFY
//{
//	WORD		wFanzhuUser;						//�������
//	BYTE		cbFanZhuCard[MAX_FANZHU_TYPE_NUM];	//��������
//};	 
//
////�������
//struct STR_CMD_SC_FANZHU_RESULT
//{
//	WORD		wFanzhuUser;						//�������
//	BYTE		cbFanZhuCard;						//��������
//};	 	
//
////�۵׿�ʼ
//struct STR_CMD_SC_SEND_KOUDI_START
//{
//	WORD		wKouDiUser;						//�۵����		
//	BYTE		cbKouDiNum;						//�۵׿�����Ŀ;
//};	
//
////�۵����
//struct STR_CMD_SC_SEND_KOUDI_FINISH
//{
//	WORD		wKouDiUser;						//�۵����
//	BYTE		cbFinishMask;					//0-δ���	1-���
//	BYTE		cbUserHandCard[MAX_CARD_COUNT];	//�۵���ɺ�������������
//	BYTE		cbMainColorNum;					//���Ƹ��������ƴ����������򣬿ͻ��˴�0-cbMainColorNumΪ����ɫ��
//};	
//
////������ʾ
//struct STR_CMD_SC_OUT_CARD_NOTIFY
//{
//	tagOutCardNotify		OutCardNotify;		//������ʾ
//};	


//���ƿ�ʼ
struct STR_CMD_SC_OUT_CARD_START
{
	WORD		wOutCardUser;						//���������
	BYTE		ActionType;							//�����ƶ�����Ϊ   //0001-����  0010-����  0100-��ʾ  1000-Ҫ����
	BYTE		TurnOutCardData[MAX_CARD_COUNT];				//��ǰ������������
	DWORD		TurnOutCardNum;					//��ǰ����������Ŀ     //�Ƿ���Ҫ
};	

//���ƽ��
struct STR_CMD_SC_OUT_CARD_RESULT
{
	BYTE		cbFlag;								//���ı�ʶ	0-��	1-��������
	BYTE		cbSuccess;							//�Ƿ�ɹ�	1-���Ƴɹ�  0-���ʹ���
	WORD		wOutCardUser;						//�������
	BYTE		cbOutCardNum;						//������Ŀ
	BYTE		cbOutCard[MAX_CARD_COUNT];			//��������
	BYTE		cbHandCardNum;						//���Ƹ���
	BYTE		cbCardType;							//���ͣ����ʹ���7����Ϊ�����ͻ�����ʾ���÷�
	SCORE		lBonusScore;						//���÷֣��н�����ֵ
	SCORE		lSingleGameScore[MAX_CHAIR_COUNT];	//��ҵ��ֵ÷�
	BYTE		handcarddata[MAX_CARD_COUNT];       //���������������
	DWORD		RoomBet[MAX_CHAIR_COUNT];			//���䱶��
};	

////һ�ֻ��ֽ���
//struct STR_CMD_SC_ROUND_BALANCE
//{
//	WORD		wRoundWinner;						//����Ӯ��
//	WORD		wCurOutCardUser;					//��ǰ�������
//	LONGLONG	lUserScore[MAX_CHAIR_COUNT];		//������һ���
//	LONGLONG	lRoundScore;						//���ַ��������طַ�Ӯ����0���÷ַ�Ӯ���Ǿ������
//};

//С����Ϸ����
struct STR_CMD_SC_XJ_GAME_END
{
	struct	XJ_END
	{ 
		WORD 		UsrChairID;							//�����λ��
		BYTE		cbLeaveHandCard[MAX_CARD_COUNT];	//���ʣ������
		SCORE		nTotalGameScore;  					//����ܵ÷���
		SCORE		nSingleGameScore;					//��ҵ��ֵ÷�
		SCORE		UserBet;							//��Ҵ˾ֵı���
		SCORE		RoomSocre;							//��ҳ�����׷�
		WORD		BankID;								//ׯ��
		std::string		PlayerName;    					//�������
		DWORD		IsWin;								//����Ƿ�Ӯ��  0-��  1-ʤ
		DWORD		IsEnd;								//����Ƿ����    0-δ����  1-����
	};
	struct XJ_END		xj_end[MAX_CHAIR_COUNT];
	WORD				wchairID;
};

//�����Ϸ����
struct STR_CMD_SC_DJ_GAME_END
{
	WORD							wMaxWinChairID;							// Ӯ��������������Ӻ�
	WORD							wFangzhuID;								// ����ID
	SCORE							lUserScore[MAX_CHAIR_COUNT];			// ����ܵ÷� 
	BYTE                            bWinTime[MAX_CHAIR_COUNT];				// ʤ������
	WORD							BankTime[MAX_CHAIR_COUNT];				//��������

	struct PlayerInfo
	{
		std::string		playerName;    //����
		WORD			playerID;   		//ID
		std::string		szFaceUrl;			//ͷ���
	};

	struct PlayerInfo	playerinfo[MAX_CHAIR_COUNT];      			//�����Ϣ
	SCORE				PlayerBigestBet[MAX_CHAIR_COUNT];			//�������ù��������
	WORD				IsEnd;												//�Ƿ����  0-����  1-δ����
	WORD				Rich_ID[MAX_CHAIR_COUNT];					//�����������Ӻ� �������
};

//��Ϸ���� ��������
struct STR_CMD_SC_PUBLIC_BET
{
	SCORE		RoomBet;			//�׷�
	SCORE		MingPai;			//����
	SCORE		Rob;				//������
	SCORE		LeaveCard;			//����
	SCORE		Boom;				//ը��
	SCORE 		Spring;			//����
	SCORE		AddScore[MAX_CHAIR_COUNT];			//�ӱ�
	SCORE		IUserAScore;			//�ܷ� 
	WORD		Game_State;			//��ǰ��Ϸ״̬
	WORD		bank_ID;			//ׯ��ID
	SCORE		public_bet;			//��������
	WORD		Rule;			//��ǰ��Ϸ����  0-��ׯ  1-�з�
};

//������
struct STR_CMD_SC_JIPAIQI_START
{
	WORD		JiPaiQi[MAX_CARD_COUNT];			//������
};

//���䱶��
struct STR_CMD_SC_FANGJIAN_BET
{
	SCORE	room_bet[MAX_CHAIR_COUNT];
};

//�й�
struct STR_CMD_SC_TUO_GUAN
{
	WORD	tuo_guan;	//��1��ʾ���Զ��й�
};

//rule
struct STR_CMD_SC_RULE
{
	WORD	rule;   // 0-jingdian   1-buxip
};

//¼��ط�
struct STR_CMD_SC_RECODE
{
	std::string		ReplayData;			//¼��ط�����
};

#pragma endregion

#pragma region ������Ϣ
/* *********************************************************************************
**										SUB������Ϣ
** *********************************************************************************/
#define SUB_CS_ROB_BANKER							1							//��ׯ
#define SUB_CS_MING_PAI								2							//����
#define SUB_CS_ADD_SCORE							3							//��ע���ӱ���
#define SUB_CS_OUT_CARD								4							//����
#define SUB_CS_XJ_GAME_READY 						5							//С����Ϸ׼��
#define	SUB_CS_PUBLIC_BET							106							//��ֽ�����������
#define	SUB_CS_JIAO_FEN								107							//�з�
#define	SUB_CS_JIPAIQI								108							//������
#define	SUB_CS_TUO_GUAN								109							//�й�

//#define SUB_CS_LIANG_ZHU							1							//����
//#define SUB_CS_FAN_ZHU								2							//����
//#define SUB_CS_KOU_DI								3							//�۵�
/////////////////////////////////////////////////////////////////////////////////////////////

//��ׯ����
struct STR_SUB_CS_ROB_BANKER
{
	BYTE		cbType;						//�����ׯ����	0-�е���	1-������	
	BYTE		cbRobState;					//��ׯ״̬		0-��	1-��/��	
};

//����������
struct STR_SUB_CS_JIAO_FEN
{
	SCORE		Score;							//��ҽ��˶��ٷ�   0-����  1-1�� 2-2�� 3-3��
};

//�����ע  
struct STR_SUB_CS_ADD_SCORE
{
	DWORD		score;						//��ע��ֵ	0-������ע
};

//�������
struct STR_SUB_CS_MING_PAI
{
	BYTE        cbType;						//��������	2-���ƿ�ʼ����	1-���ƿ�ʼ����
	BYTE		cbIsMing;					//�Ƿ�����	0-������	1-����
	SCORE		OutCard_bet;				//�������Ƶı���
};


////����
//struct STR_SUB_CS_LIANG_ZHU
//{
//	WORD		wLiangZhuUser;		//�������
//	BYTE		cbLiangZhuColor;	//������ɫ
//};
//
////����
//struct STR_SUB_CS_FAN_ZHU
//{
//	BYTE		cbFlag;				//������ʶ	0-��ҷ�������	1-��ҷ���
//	WORD		wFanZhuUser;		//�������
//	BYTE		cbFanZhuCard;		//��������
//};

////�۵�
//struct STR_SUB_CS_KOUDI
//{
//	WORD		wKouDiUser;							//�۵����
//	BYTE		cbKouDiCard[MAX_LEAVE_CARD_NUM];	//�۵׿���
//};

//����
struct STR_SUB_CS_OUT_CARD
{
	BYTE		cbPass;								//��	0-��������	1-��
	WORD		wOutCardUser;						//�������
	BYTE		cbOutCardNum;						//������Ŀ
	BYTE		cbOutCard[MAX_CARD_COUNT];			//��������
};

//С����Ϸ׼��
struct STR_SUB_CS_XJ_USER_READY
{
	WORD		wReadyUser;							//׼�����
	BYTE		cbReady;							//С����Ϸ�û�׼��	1-׼����0-δ׼��
	SCORE		cbBet;								//���ƿ�ʼ����
};

//��Ϸ���� ��������
struct STR_SUB_CS_PUBLIC_BET
{
	WORD		wChairID;			//Ҫ�鿴�����Ӻ�
};

//������
struct STR_SUB_CS_JIPAIQI
{
	WORD tmp;			//��Ҫ��0
};

//�й�
struct STR_SUB_CS_TUO_GUAN
{
	WORD	TuoGuan_state;		//1-�����й�   0-ȡ���й�
};

#pragma endregion

#pragma region ��������
//����״̬
struct STR_CMD_SC_STATUS_FREE
{
	BYTE	GameCount;				//��Ϸ���� 0-���޾�
	BYTE	PlayerCount;			//��Ϸ����
	WORD	CellScore;				//�׷�	  Ĭ��Ϊ1
	DWORD	FangZhu;				//����    �����û�
};

////����״̬
//struct STR_CMD_SC_STATUS_LIANGZHU
//{
//	BYTE	cbHandCard[MAX_CARD_COUNT];			//�������
//	BYTE	cbCardNum;							//�����������
//	BYTE	cbLastTime;							//������ʱ��ʣ��ʱ�䣨�룩
//};
//
////����״̬
//struct STR_CMD_SC_STATUS_FANZHU
//{
//	WORD	wCurFanzhuUser;						//��ǰ�������
//	BYTE	cbFanZhuCard[MAX_FANZHU_TYPE_NUM];	//����Լ��ķ������ƣ���ǰ������Լ���ʱ�����ʾ������ʾ
//	BYTE	cbLastTime;							//������ʱ��ʣ��ʱ�䣨�룩
//};
//
////�۵�״̬
//struct STR_CMD_SC_STATUS_KOUDI
//{
//	WORD		wCurKouDiUser;						//��ǰ�۵����
//	BYTE		cbSortedCard[MAX_CARD_COUNT];		//�������������
//	BYTE		cbHandCardNum;						//���Ƹ���
//	BYTE		cbMainColorNum;						//���Ƹ��������ƴ����������򣬿ͻ��˴�0-cbMainColorNumΪ����ɫ��
//	BYTE		cbMainColor;						//��ǰ����ɫ
//};


//��ׯ״̬
struct STR_CMD_SC_STATUS_ROB
{
	WORD			CurRobUsr;					//��ǰ������ׯ�����
	WORD 			robstate[MAX_CHAIR_COUNT];	//����ҵ���ׯ״̬
	BYTE			HandCardData[MAX_CARD_COUNT];	//��ǰ��ҵ�����
	DWORD			HandCardNum[MAX_CHAIR_COUNT];	//����ҵ����Ƹ���
	WORD			CurJuShu;		//��ǰ�������
	SCORE			room_bet[MAX_CHAIR_COUNT];	//��ǰ���䱶��
	SCORE			All_bet[MAX_CHAIR_COUNT];	//�ܻ���
	SCORE			GoldCoin[MAX_CHAIR_COUNT];	//����ҵĽ��
	DWORD		    replay_code;			//�ط���
};

//�з�״̬
struct STR_CMD_SC_STATUS_JIAOFEN
{
	WORD	   CurRobUsr;            //��ǰ���ڽзֵ����
	WORD	   robstate[MAX_CHAIR_COUNT];     //����ҵĽз�״̬
	BYTE	   HandCardData[MAX_CARD_COUNT];         //��ǰ��ҵ�����
	DWORD	   HandCardNum[MAX_CHAIR_COUNT];  //����ҵ����Ƹ���
	DWORD	   CurJuShu;             //��ǰ�������
	SCORE	   room_bet[MAX_CHAIR_COUNT];     //��ǰ���䱶��
	SCORE	   All_bet[MAX_CHAIR_COUNT];      //�ܻ���
	SCORE	   GoldCoin[MAX_CHAIR_COUNT];	//����ҵĽ��
	DWORD	   replay_code;			//�ط���
};


//��ע״̬
struct STR_CMD_SC_STATUS_ADD_SCORE
{
	WORD		scorestate[MAX_CHAIR_COUNT];	//����ҵ���ע״̬
	BYTE		HandCardData[MAX_CARD_COUNT];	//��ǰ��ҵ�����
	DWORD		HandCardNum[MAX_CHAIR_COUNT];	//����ҵ����Ƹ���
	DWORD		CurJuShu;			             //��ǰ�������
	SCORE 		room_bet[MAX_CHAIR_COUNT];	//��ǰ����ı���
	WORD		bankID;		//ׯ��ID
	SCORE		All_bet[MAX_CHAIR_COUNT];	//�ܻ���
	SCORE 		GoldCoin[MAX_CHAIR_COUNT];	//����ҵĽ��
	DWORD		replay_code;			//�ط���
};

struct player_op_info
{
   WORD					op_type;    //�������� 0-�� 1-����
   BYTE 				op_result[MAX_CARD_COUNT]; //���ƽ��, ������; ֻ��op_type=1ʱ����Ч
   DWORD				op_cardscount;   //��ǰ������
   BYTE					MingPaiCardData[MAX_CARD_COUNT];   //������ҵ�����
};

//����״̬
struct STR_CMD_SC_STATUS_OUTCARD
{
	struct	player_op_info		players_op[MAX_CHAIR_COUNT];     //�����Ϣ
	WORD						outcardid;						//��ǰ�������
	BYTE 						brokenoutcarddata[MAX_CARD_COUNT];					//�Լ�����������
	WORD						CurJuShu;             //��ǰ�������
	SCORE 						room_bet[MAX_CHAIR_COUNT];		//��ǰ����ı���
	WORD						bankID;		//ׯ��ID
	WORD 						MingPaiState[MAX_CHAIR_COUNT];		//����ҵ�����״̬
	BYTE 						LeaveCard[MAX_LEAVE_CARD_NUM];		//����
	SCORE						All_bet[MAX_CHAIR_COUNT];		//�ܻ���
	BYTE						TurnCardData[MAX_CARD_COUNT];	//��ǰ��������
	SCORE 						GoldCoin[MAX_CHAIR_COUNT];	//����ҵĽ��
	WORD						Add_bet[MAX_CHAIR_COUNT];		//����Ƿ�ӱ�
	SCORE						Leave_bet;		//���Ʊ���
	DWORD					    replay_code;			//�ط���
};


//С�ֽ���״̬
struct STR_CMD_SC_STATUS_XJ_END
{
	SCORE 		nSingleGameScore[MAX_CHAIR_COUNT];				//��ҵ��ֵ÷�	
};

//����Ϸ�������
struct STR_CMD_SUB_ROOM_RULE
{
	WORD		Gamejushu;                  //��Ϸ����
	WORD		Cellscore;                   //��Ϸ�׷�(1/2/5)
	WORD		Balancemode;                //���㷽ʽ(0:���ֽ��� 1:��ҽ���)
	WORD		GameChuPai;                 //��Ϸ����(��ʱ-->15�����/25�����/�����Ƴ���)
	WORD		GameWanFa;                  //��Ϸ�淨(�����淨/����淨/������)
	WORD		GameDiZhu;                  //��Ϸ����ģʽ(0-������ 1-������)
	WORD		GameFengDing;               //��Ϸ�ⶥģʽ(32/64/128/���ⶥ)
	WORD		Preventcheat;               //������
	WORD		DontCutCards;               //��ϴ��
	WORD		AddMultiple;               //�ӱ�
	WORD		ShowCards;                 //����
	WORD		BaseCardAddMultiple;       //���Ʒ���
	WORD		GameMode;                  //��Ϸ�÷�ģʽ   0-����ģʽ  1-���ģʽ 
};

////¼��طŽṹ��
//message GAME_REPLAY
//{
//	//����Ϸ�������
//	message MSG_SUB_ROOM_RULE
//	{
//			int32 Gamejushu = 1;                  //��Ϸ����
//		int32 Cellscore = 2;                   //��Ϸ�׷�(1/2/5)
//		int32 Balancemode = 3;                //���㷽ʽ(0:���ֽ��� 1:��ҽ���)
//		int32 GameChuPai = 4;                 //��Ϸ����(��ʱ-->15�����/25�����/�����Ƴ���)
//		int32 GameWanFa = 5;                  //��Ϸ�淨(�����淨/����淨/������)
//		int32 GameDiZhu = 6;                  //��Ϸ����ģʽ(0-������ 1-������)
//		int32 GameFengDing = 7;               //��Ϸ�ⶥģʽ(32/64/128/���ⶥ)
//		int32 Preventcheat = 8;               //������
//		int32 DontCutCards = 9;               //��ϴ��
//		int32 AddMultiple = 10;               //�ӱ�
//		int32 ShowCards = 11;                 //����
//		int32 BaseCardAddMultiple = 12;       //���Ʒ���
//		int32 GameMode = 13;                  //��Ϸ�÷�ģʽ   0-����ģʽ  1-���ģʽ
//	};
//
////�����Ϣ
//message MSG_TABLE_PLAYER_INFO_NOTIFY
//{
//		uint32 messageid = 1;
//		uint64  player_id = 2;                  //���ID
//		string name = 3;                    //�������
//		int64  registerDate = 4;                //ע��ʱ��
//		uint32 sex = 5;                    //�Ա�
//		int32 userstate = 6;                  //���״̬
//		int32 tableposid = 7;                //��λ��
//		int32 isonline = 8;                  //����״̬��1��Ϊ����0��������
//		int64 playergold = 9;                //���
//		string faceurl = 10;                  //ͼ���ַ
//};
//
//
////������Ϣ
//message OPERATE_INFO
//{
//	MSG_M2C_SEND_CARD 			send_card = 1;		//����
//	MSG_M2C_ROB_RESULT 			rob_result = 2;		//��ׯ���
//	MSG_M2C_APPOINT_BANKER 				appoint_banker = 3;	//ָ��ׯ��
//	MSG_M2C_SEND_LEAVE_CARD		send_leave_card = 4;	//���͵���
//	MSG_M2C_MING_PAI_RESULT		mingpai_result = 5;	//���ƽ��
//	MSG_M2C_ADD_SCORE_RESULT		addscore_result = 6;	//�ӱ����
//	MSG_M2C_OUT_CARD_RESULT		outcard_result = 7;	//���ƽ��
//	MSG_M2C_XJ_GAME_END			xj_end = 8;		//С�ֽ���
//	MSG_M2C_DJ_GAME_END				dj_end = 9;		//��ֽ���
//	MSG_M2C_JIAOFEN_RESULT			jiaosanfen_result = 10;	//�����ֽ��	
//};
//
//MSG_SUB_ROOM_RULE    room_info = 1;
//repeated MSG_TABLE_PLAYER_INFO_NOTIFY player_info = 2;
//repeated OPERATE_INFO operate_info = 3;
//};
//
////����ս����¼
//message GAME_SINGLE_RECODE
//{
//	int32		cur_jushu = 1;		//��ǰ����
//	int32		winID = 2;		//Ӯ��
//	repeated int32	single_score = 3;		//���ֵ÷�
//};

//***********************************************************************************//
#pragma endregion

#pragma region ������ -- ������
///////////////////////////////////////////////////////////////////////////////////////////////////
// ������ʹ��, ��ʱδ�õ� added by WangChengQing 2017.9.21 TODONOW �Ժ�Ӧ�������������ӻ����˹���
///////////////////////////////////////////////////////////////////////////////////////////////////
//����������ṹ    --  ������
#define SUB_S_A_ADD_SCORE					(0x0008)									//��ע���
#define SUB_S_A_PLAYER_EXIT				(0x0010)									//�û�ǿ��
#define SUB_S_A_GAME_END					(0x0040)									//��Ϸ����
#define SUB_S_A_OPEN_CARD					(0x0080)									//�û�̯��
#define SUB_S_A_CALL_BANKER				(0x0100)									//�û���ׯ
#define SUB_S_A_CALL						(0x0400)									//�û���ׯ

//���flash ID �� ����ID
struct CMD_S_PlayerAction
{
	WORD wChairID;
	int iActionID;
};
//��� ����ID
struct CMD_C_PlayerAction
{
	int iActionID;
};


//��Ϸ״̬
struct CMD_S_StatusCall
{
	WORD							    	wCallBanker;						//��ׯ�û�
	BYTE							        cbPlayStatus[MAX_CHAIR_COUNT];          //�û�״̬
	BYTE									cbCallStatus[MAX_CHAIR_COUNT];			//��ׯ״̬
	bool									bCallBanker[MAX_CHAIR_COUNT];			//�Ƿ��ׯ
	SCORE									lCellScore;							//��������
	SCORE									lRevenue;
	BYTE									cbTimeLeave;
	SCORE									lPlayerScore[MAX_CHAIR_COUNT];			//��ҷ���
};

struct CMD_S_PlayerCount
{
	BYTE									wPlayerCount;
	SCORE									lUserScore[MAX_CHAIR_COUNT];			//��һ���
};
//
////�û���ׯ
struct CMD_S_CallBanker
{
	WORD							     	wCallBanker;						//��ׯ�û�
	bool							    	bFirstTimes;						//�״ν�ׯ
};

////////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ             --  ������
#define SUB_C_A_CALL_BANKER				1									//�û���ׯ
#define SUB_C_A_ADD_SCORE					2									//�û���ע
#define SUB_C_A_OPEN_CARD					3									//�û�̯��
#define SUB_C_A_AMDIN_COMMAND				5									//����Ա����
#define SUB_C_A_AMDIN_CHANGE_CARD			6									//����Ա����
#define SUB_C_A_READY_MESSAGE				7								    //�ͻ��˷���׼����Ϣ

/////////-----5
struct CMD_C_Admin_ChangeCard
{
	BYTE cbStart;
	BYTE cbEnd;

};

# endif

//lD6-�e?��<p??~?,?B��[k���k�H��ocO��è��KH^���b?�����*؉��J�s�a��?
//Y?.֒���XK�sr�]��GVM?�E8����6�gʝ�Q7��?��pe��