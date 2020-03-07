#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE
#pragma once

#include "Stdafx.h"
#include "GameData.h"
#include "IntelligentAI.h"

enum GameRecIndex
{
	chariID = 0,				//�����Ӻš���ʼ�ֽ��������ݴ�С����
	nickName,					//���ǳơ���ʼ�ֽ��������ݴ�С����
	robResult,					//����ׯ�������ʼ�ֽ��������ݴ�С����
	betStart,					//����ע��ʼ��ʼ�ֽ��������ݴ�С����
	betResult,					//����ע�������ʼ�ֽ��������ݴ�С����		
	sendCard,					//�����ơ���ʼ�ֽ��������ݴ�С����
	showCard,					//��̯�ơ���ʼ�ֽ��������ݴ�С����
	xjGameEnd					//��С�ֽ�������ʼ�ֽ��������ݴ�С����
};

//С����Ϸ��������Ϸ¼��ṹ��
//(����ID + ����ǳ� + ��ׯ��� + ��ע��� + ̯��)*MAX_CHAIR_COUNT + (��ע��ʼ + ���� + С�ֽ���)*1 + ��ʱ��*3
struct GameRecord
{
	BYTE *pRecData;			// ��Ϸ¼������	//pRecData�Ĵ�С =  nEachGameSize
	int nRecOffset[8];		// ��Ϸ¼��������������������¼������,��������ǳƴӵ�25���ֽڿ�ʼ
	int nStructSize[8];		// ÿ������¼��Ľṹ�����ݴ�С
	int nEachGameSize;		// ÿ����Ϸ�����ݴ�С
	time_t time_start;		// ĳ����ʱ����ʼ��ʱ��
	time_t time_end;		// ĳ����ʱ��������ʱ��
};


//��Ϸ������
class CTableFrameSink : public ITableFrameSink
{
	//�������
protected:
	//�����ദ����������Ϸͨ�õĲ���
	CGameData						m_GameData;								//��Ϸ�߼�����
	CGameLogic						* m_GameLogic;							//��Ϸ�߼��ӿ�
	CGameAccess						* m_GameAccess;							//��Ϸ��ѯ�ӿ�

	ITableFrame						* m_pITableFrame;						//��ܽӿ�

	tagTableCfg						* m_pRoomRuleOption;					//�������

	GameRecord						m_GameRec;								//��Ϸ¼�����ݽṹ��

	AILogic							m_AILogic;							    //AI�����߼���
	HandCardData					m_HandCardData[MAX_CHAIR_COUNT];						//AI����������

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//����ӿ�
public:
	//��ʼ��
	virtual bool Initialization(ITableFrame *pTableFrame, tagTableRule *comRule);

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChairID);

	//�¼��ӿ�
public:
	// ��ʱ���¼�
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, WORD wChairID);

	//�йܽӿ�
public:
	//�й�
	virtual bool PlayerTuoGuan(WORD wChairID);
	//ȡ���й�
	virtual bool PlayerCancelTuoGuan(WORD wChairID);

	//��Ӧclient�¼�
protected:
	//�����¼�
	void OnUserLiangZhu(WORD wChairID, BYTE cbCardColor);
	//�����¼�
	void OnUserFanZhu(WORD wChairID, BYTE cbCard, BYTE cbFanzhuFlag);
	//�۵��¼�
	void OnUserKouDi(WORD wChairID, BYTE cbLeaveCard[MAX_LEAVE_CARD_NUM]);
	//�����¼�
	void On_Sub_UserOutCard(WORD wOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum, const bool &bPass);

	//��ׯ�¼�
	void OnUserCallBanker(WORD wChairID, BYTE cbResult);
	//��ע�¼�
	void OnUserAddScore(WORD wChairID, WORD wTYPE);
	//�����¼�
	void On_Sub_UserMingPai(WORD wChairID, const BYTE &cbMPType, BYTE cbFlag, BYTE FlushCardBet = 1);

	//��Ϸ�����Ļ�ȡ����������Ϣ
	void OnUserPublicBet(WORD wChairID);

	//����ͻ��˷����ļ�������Ϣ
	void OnUserJiPaiQi(WORD wChairID);

	//��Ӧclientʱ��� �ڲ�����
protected:
	// ȫ����ׯ���
	void AllRobBankerOver();
	// С����Ϸ����
	bool XjGameConclude(int nTotalGameCount, int nCurGameCount);

	//��������
protected:
	//��ʼ��Ϸ
	void StartGame();
	//��ʼ¼��
	void StartRecord();
	//����¼��
	void CloseRecord();
	//����������
	void HandleDeal();
	//�����͵���
	void HandleSendLeaveCard(const WORD &wSendCardUser);
	//������ƿ�ʼ��Ϣ
	void HandleOutCardStart(const WORD &wOutCardUser);
	//�����û���������
	void HandleOutCard(WORD wOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum);
	//�����û����ƹ�
	void HandleOutCardPass(WORD wOutCardUser);
	//����һ�ֳ��ƽ���
	void HandleRoundEnd(const WORD &wWinner);
	//���������
	void HandleJiaoFenRobBanker();
	//���ͽ����ֿ�ʼ��Ϣ
	void SendJiaoFenStart(WORD wChairID, const BYTE &ActionType);
	//����ͻ��˷����Ľ�������Ϣ
	void OnUserJiaoFen(WORD wChairID, BYTE Score);
	//������ׯ
	void HandleRobBanker();
	//����������ׯ
	void HandleFreeRobBanker();
	//������ׯ��ʼ��Ϣ
	void SendRobStart(const WORD &wChairID, const BYTE &cbType);
	//����ָ��ׯ����Ϣ
	void SendAppointBanker(const WORD &wBanker);
	//������ע��ʼ��Ϣ
	void SendAddScoreStart(const WORD &wChairID);
	//�������ƿ�ʼ��Ϣ
	void SendMingPaiStart(const WORD &wChairID, const BYTE &cbMPType);
	//���ͳ��ƽ��
	void SendOutCardResult(WORD wOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum, 
						const int &cbCardType, const BYTE &cbSuccess, const BYTE &cbFlag);
	//�ж�����Ƿ��ܳ���
	bool JudgeUserOutCard(const WORD &wLastOutCardUser, BYTE *cbOutCard, BYTE cbOutCardNum);
};

#endif
