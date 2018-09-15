#ifndef CMD_OX_HEAD_FILE
#define CMD_OX_HEAD_FILE
#pragma pack(1)		//1�ֽڶ���
#include <windows.h>
#include "define.h"
#include "cardType.h"


#pragma region ��Ϸ����
//��Ϸ��������Attribute
#define KIND_ID								4												//Kind I D
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


#define CMD_SC_USER_LIANGZHU_RESULT					3							//����������
#define CMD_SC_USER_FANZHU_NOTIFY					4							//��ҷ�����ʾ
#define CMD_SC_USER_FANZHU_RESULT					5							//��ҷ������
#define CMD_SC_USER_KOUDI_START						7							//�۵׿�ʼ
#define CMD_SC_USER_KOUDI_FINISH					8							//�۵���ɣ�����ҷ����������ƣ�

#define CMD_SC_USER_OUT_CARD_NOTIFY					11							//������ʾ�����Ƴ���Щ�ƣ�


///////////////////////////////////////////////////////////////////////////////////
//��������
struct STR_CMD_SC_SEND_CARD
{
	WORD		wChairID;							//�������
	BYTE		cbHandCardData[MAX_CARD_COUNT];		//��������
	BYTE		cbHandCardNum;						//���Ƹ���
};

//��ׯ��ʼ
struct STR_CMD_SC_ROB_BANKER_START
{
	BYTE		cbType;						//�����ׯ����	0-δ��ʼ	1-�е���	2-������
};

//��ׯ���
struct STR_CMD_SC_ROB_RESULT
{
	WORD		wChairID;					//���Ӻ�
	BYTE		cbType;						//�����ׯ����	0-δ��ʼ	1-�е���	2-������
	BYTE		cbRobState;					//�����ׯ��־	0-��	1-��/��

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
};

//���ƿ�ʼ
struct STR_CMD_SC_MING_PAI_START
{
	WORD                                wChairID;						//���ƿ�ʼ��� 
	BYTE                                cbType;							//��������	2-���ƿ�ʼ����	1-���ƿ�ʼ����
};

//���ƽ��
struct STR_CMD_SC_MING_PAI_RESULT
{
	WORD                                wChairID;						//������� 
	BYTE                                cbIsMing;						//�Ƿ�����	0-������	1-����
	BYTE								cbTimes;						//�ӱ���ı���
	BYTE								cbHandCard[MAX_CARD_COUNT];		//�û�����
};




//�������
struct STR_CMD_SC_LIANGZHU_RESULT
{
	WORD		wLiangzhuUser;						//�������
	BYTE		cbLZCard;							//��������
};	

//������ʼ
struct STR_CMD_SC_FANZHU_NOTIFY
{
	WORD		wFanzhuUser;						//�������
	BYTE		cbFanZhuCard[MAX_FANZHU_TYPE_NUM];	//��������
};	 

//�������
struct STR_CMD_SC_FANZHU_RESULT
{
	WORD		wFanzhuUser;						//�������
	BYTE		cbFanZhuCard;						//��������
};	 	

//�۵׿�ʼ
struct STR_CMD_SC_SEND_KOUDI_START
{
	WORD		wKouDiUser;						//�۵����		
	BYTE		cbKouDiNum;						//�۵׿�����Ŀ;
};	

//�۵����
struct STR_CMD_SC_SEND_KOUDI_FINISH
{
	WORD		wKouDiUser;						//�۵����
	BYTE		cbFinishMask;					//0-δ���	1-���
	BYTE		cbUserHandCard[MAX_CARD_COUNT];	//�۵���ɺ�������������
	BYTE		cbMainColorNum;					//���Ƹ��������ƴ����������򣬿ͻ��˴�0-cbMainColorNumΪ����ɫ��
};	

//������ʾ
struct STR_CMD_SC_OUT_CARD_NOTIFY
{
	tagOutCardNotify		OutCardNotify;		//������ʾ
};	

//���ƿ�ʼ
struct STR_CMD_SC_OUT_CARD_START
{
	WORD		wOutCardUser;					//���������
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
};	

//һ�ֻ��ֽ���
struct STR_CMD_SC_ROUND_BALANCE
{
	WORD		wRoundWinner;						//����Ӯ��
	WORD		wCurOutCardUser;					//��ǰ�������
	LONGLONG	lUserScore[MAX_CHAIR_COUNT];		//������һ���
	LONGLONG	lRoundScore;						//���ַ��������طַ�Ӯ����0���÷ַ�Ӯ���Ǿ������
};	

//С����Ϸ����
struct STR_CMD_SC_XJ_GAME_END
{
	BYTE	cbLeaveCard[MAX_LEAVE_CARD_NUM];			//����	
	SCORE	nTotalGameScore[MAX_CHAIR_COUNT];			//����ܵ÷���		//���еĵ÷ֶ���LONGLONG������8���ֽ�
	SCORE	nSingleGameScore[MAX_CHAIR_COUNT];			//��ҵ��ֵ÷�
};

//�����Ϸ����
struct STR_CMD_SC_DJ_GAME_END
{
	WORD							wMaxWinChairID;							// Ӯ��������������Ӻ�
	WORD							wFangzhuID;								// ����ID
	SCORE							lUserScore[MAX_CHAIR_COUNT];			// ����ܵ÷� 
	BYTE                            bWinTime[MAX_CHAIR_COUNT];				// ʤ������
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

#define SUB_CS_LIANG_ZHU							1							//����
#define SUB_CS_FAN_ZHU								2							//����
#define SUB_CS_KOU_DI								3							//�۵�
/////////////////////////////////////////////////////////////////////////////////////////////

//��ׯ����
struct STR_SUB_CS_ROB_BANKER
{
	BYTE		cbType;						//�����ׯ����	0-�е���	1-������	
	BYTE		cbRobState;					//��ׯ״̬		0-��	1-��/��	
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
};


//����
struct STR_SUB_CS_LIANG_ZHU
{
	WORD		wLiangZhuUser;		//�������
	BYTE		cbLiangZhuColor;	//������ɫ
};

//����
struct STR_SUB_CS_FAN_ZHU
{
	BYTE		cbFlag;				//������ʶ	0-��ҷ�������	1-��ҷ���
	WORD		wFanZhuUser;		//�������
	BYTE		cbFanZhuCard;		//��������
};

//�۵�
struct STR_SUB_CS_KOUDI
{
	WORD		wKouDiUser;							//�۵����
	BYTE		cbKouDiCard[MAX_LEAVE_CARD_NUM];	//�۵׿���
};

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

//����״̬
struct STR_CMD_SC_STATUS_LIANGZHU
{
	BYTE	cbHandCard[MAX_CARD_COUNT];			//�������
	BYTE	cbCardNum;							//�����������
	BYTE	cbLastTime;							//������ʱ��ʣ��ʱ�䣨�룩
};

//����״̬
struct STR_CMD_SC_STATUS_FANZHU
{
	WORD	wCurFanzhuUser;						//��ǰ�������
	BYTE	cbFanZhuCard[MAX_FANZHU_TYPE_NUM];	//����Լ��ķ������ƣ���ǰ������Լ���ʱ�����ʾ������ʾ
	BYTE	cbLastTime;							//������ʱ��ʣ��ʱ�䣨�룩
};

//�۵�״̬
struct STR_CMD_SC_STATUS_KOUDI
{
	WORD		wCurKouDiUser;						//��ǰ�۵����
	BYTE		cbSortedCard[MAX_CARD_COUNT];		//�������������
	BYTE		cbHandCardNum;						//���Ƹ���
	BYTE		cbMainColorNum;						//���Ƹ��������ƴ����������򣬿ͻ��˴�0-cbMainColorNumΪ����ɫ��
	BYTE		cbMainColor;						//��ǰ����ɫ
};

//����״̬
struct STR_CMD_SC_STATUS_OUTCARD
{		
	WORD		wCurOutCardUser;							//��ǰ�������
	BYTE		cbOutCard[MAX_CHAIR_COUNT][MAX_CARD_COUNT];	//����Ѿ�������	
};

//С�ֽ���״̬
struct STR_CMD_SC_STATUS_XJ_END
{		
	SCORE	nSingleGameScore[MAX_CHAIR_COUNT];				//��ҵ��ֵ÷�	
};


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

//�û���ׯ
struct CMD_C_CallBanker
{
	BYTE							    	bBanker;							//��ׯ��־
};
#pragma endregion
#pragma pack()

#endif
