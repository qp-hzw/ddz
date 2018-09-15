#ifndef GAME_DATA_L
#define GAME_DATA_L

#include "GameInterface.h"

class CGameData : public CGameLogic, public CGameAccess
{
public:

	CGameData();
	~CGameData() {}

// ��Ϸ����ӿ�
public:
	// ������Ϸ
	virtual int __stdcall CreateGame( CGameLogic **pGameRule, CGameAccess **pGameAccess );

	// ���ض�ȡ�������ļ�����
	virtual int __stdcall LoadConfigParameter( const wchar_t *_logFile, const BYTE cbGameScoreMode);

	//��ʼ������
	virtual int __stdcall Init();

	// �����ע
	virtual int __stdcall Wager( WORD wChairID, DWORD bet );

	// ָ��ׯ��
	virtual bool __stdcall AppointBanker(WORD &wChairID);

	// ϴ�Ʋ�����
	virtual int __stdcall Deal( );

	// ������Ҳ���
	virtual int __stdcall SetUserOperate( WORD wChairID, BYTE byAction, DWORD dwActionScore );

	// �����û�����
	virtual int __stdcall AnsysCardsType();

	// ����÷�
	virtual int __stdcall Settle();

	// ����С����Ϸ����
	virtual int __stdcall ClearXjGame() ;

	// ��������Ϸ����
	virtual int __stdcall ClearDjGame() ;

	// ����������
	virtual int __stdcall ClearRoom() ;

	//����ɫȨλ�����˿�
	virtual void __stdcall SortCardList(BYTE *cbCardData, BYTE cbCardCount);

	//����ɫȨλ���û���������
	virtual void __stdcall SortCardList(const WORD &wChairID, BYTE cbCardCount, BYTE cbSortType);

	//����������
	virtual void __stdcall SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);

	//��ÿ��Ƶ�����ȼ�
	virtual BYTE __stdcall GetCardSortOrder(BYTE cbCardData);

	//��������Ƿ�������
	virtual bool __stdcall AnalyseUserLiangZhu(const WORD &wChairID, BYTE *cbLiangZhuColor);

	//��������Ƿ��ܷ���
	virtual bool __stdcall AnalyseUserFanZhu(const WORD &wChairID, BYTE *cbFanZhuCard);

	//�ж���������Ƿ��ܷ���
	virtual bool __stdcall IsUserCanFanZhu(const WORD &wChairID, const BYTE &cbCurFanZhuColor, BYTE *cbFanZhuCard);

	//���͵��Ƹ����
	virtual void __stdcall SendLeaveCardToUser(const WORD &wChairID);

	//���һ�鿨�Ƶ�������
	virtual BYTE __stdcall GetCardLogicType(const BYTE *cbCardData, BYTE cbCardCount);

	//��ÿ��Ƶ��߼���ɫ
	virtual BYTE __stdcall GetCardLogicColor(BYTE cbCardData);

	//��ÿ���������߼���ɫ
	virtual BYTE __stdcall GetCardLogicColor(const BYTE *cbCardData, BYTE cbCardCount);

	//��ÿ��Ƶ��߼���ֵ
	virtual BYTE __stdcall GetCardLogicValue(BYTE cbCardData);

	//���ſ����Ƿ�����
	virtual bool __stdcall IsLineValue(BYTE cbTurnCard, BYTE cbSecondCard);

	//һ�ֱ��ƣ��ж�Ӯ��
	virtual WORD __stdcall GetOneRoundWinner();

	//����һ����Ӯ�÷�
	virtual WORD __stdcall SettleOneRound(const WORD &wWinner);

	//���һ���е����з���
	virtual BYTE __stdcall GetScoreCard();

	//��ȡ���������ܷ���
	virtual WORD __stdcall GetAllCardScore(const BYTE *cbCardData, BYTE cbCardCount);

	//�Ƚ���ҳ��ƴ�С
	virtual bool __stdcall CompareUserOutCard(const BYTE &cbFirstCardData, const WORD &cbNextCardData);

	//����У��
	virtual bool __stdcall EfficacyOutCard(const WORD &wCheckUser, const BYTE *cbOutCardData, const BYTE &cbOutCardCount);

	//����ָ����ɫ��ȡ�˿�
	virtual BYTE __stdcall DistillCardByColor(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardColor, BYTE cbResultCard[]);

	//��ȡ�˿�
	virtual BYTE __stdcall DistillCardByCount(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbSameCount, tagSameDataInfo &SameDataInfo);

	//��ȡ�˿�
	virtual BYTE __stdcall DistillTrackorByCount(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbSameCount, tagTractorDataInfo &TractorDataInfo);

	//�ж���������Ľ�����Ŀ
	virtual BYTE __stdcall GetIntersectionCount(const BYTE cbCardData1[], BYTE cbCardCount1, const BYTE cbCardData2[], BYTE cbCardCount2);

	//����һ�ֵĿ�ʼ������ң�������ҿ��Գ�����
	virtual bool __stdcall AnalyseOutCardNotify(const WORD &wChairID, const BYTE *OutCardData, 
											   const BYTE &cbOutCardNum, tagOutCardNotify *cbOutCardNotify);

	//�ж��û��������Ƿ���ָ����ɫ�ĸ����͵Ŀ���
	virtual bool __stdcall IsHaveSpecificType(const WORD &wChairID, const BYTE &color, const BYTE &cbOutCardType, 
												const BYTE &cbOutCardNum, tagOutCardNotify &cbOutCardNotify);

	//���㳭�׵÷�
	virtual SCORE __stdcall CalculateChaodiScore(const BYTE &cbCDcardNum);

	//���ݵ÷ּ��������������
	virtual BYTE __stdcall AnalyseUpGrade(const SCORE &score);

	//�������Ƚϴ�С
	virtual bool __stdcall AnalysebCompare(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult &AnalyseResult, int dCardType);

	//�����������˿�
	virtual void __stdcall AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult &AnalyseResult);

	//��������ȡ���ͽ��
	virtual int __stdcall GetType(tagAnalyseResult AnalyseResult,const BYTE cbCardData[],BYTE cbCardCount,const BYTE NoChangeCard[]);


	//��������
	//�˿�ת��
	virtual BYTE __stdcall SwitchToCardData(BYTE cbCardIndex);

	//�˿�ת��
	virtual BYTE __stdcall SwitchToCardIndex(BYTE cbCardData);

	//�˿�ת��
	virtual BYTE __stdcall SwitchToCardData(const BYTE cbCardIndex[LEN_MAX_INDEX], BYTE cbCardData[MAX_CARD_COUNT]);

	//�˿�ת��
	virtual BYTE __stdcall SwitchToCardIndex(const BYTE *cbCardData, BYTE cbCardCount, BYTE *cbCardIndex);

	//ɾ���˿�
	virtual bool __stdcall RemoveCard(const BYTE *cbRemoveCard, BYTE cbRemoveCount, BYTE *cbCardData, BYTE cbCardCount);

	//�����������ɾ���˿�
	virtual bool __stdcall RemoveCard(const WORD &wChairID, const BYTE *cbRemoveCard, BYTE cbRemoveCount);

// ��Ϣ��ѯ�ӿ�CGameAccess
public:
	// ��ѯ��ǰ��Ϸ����ID
	virtual int __stdcall SetKindID(DWORD id);

	// ��ѯ��ǰ��Ϸ����  
	virtual DWORD __stdcall GetKindID();

	//added by lizhihu 2017.10.18
	//��ѯ��Ϸ����-�������Ŀ
	virtual DWORD __stdcall GetGameMaxCardNum();

	// ���������˵���ת�����ͻ��ˣ���ѯ�û�����
	virtual int __stdcall GetClientHandCards( WORD wChairID, BYTE *cards, const int &cardsum );

	// ��ѯ���С�ֵ÷�
	virtual SCORE __stdcall GetPlayerSingleScore( WORD wChairID );

	// ��ѯ��ҷ������ܵ÷�
	virtual SCORE __stdcall GetPlayerTotalScore( WORD wChairID );


	//****************��ѯ/�����û���Ϸ��״̬*********************//

	// ��ȡ�û���ׯ״̬
	virtual BYTE __stdcall GetBankerState( WORD wChairID);

	//�����û���ׯ��ׯ
	virtual int __stdcall SetBankerState( WORD wChairID, BYTE care  );

	// ��ѯ���׼��״̬
	virtual BYTE __stdcall GetXjReadyState( WORD wChairID ) ;
	
	// �������׼��
	virtual int __stdcall SetXjReadyState( WORD wChairID, BYTE ready );

	// ��ѯ�����ע��ɱ�־
	virtual BYTE __stdcall GetAddScoreState( WORD wChairID );

	// ���������ע��ɱ�־		0-��עδ���		1-��ע���
	virtual int __stdcall SetAddScoreState( WORD wChairID, BYTE beted );

	// ��ѯ��Ҷ�����ɱ�־
	virtual BYTE __stdcall GetUserOperateState( WORD wChairID);

	// ������Ҷ�����ɱ�־    	0-����δ���		1-�������
	virtual int __stdcall SetUserOperateState( WORD wChairID,  BYTE state );

	// ��ѯ���̯��
	virtual BYTE __stdcall GetShowCardState(WORD wChairID);

	// �������̯��		0-δ̯��	1-̯��
	virtual int __stdcall SetShowCardState( WORD wChairID, BYTE state ) ;
	//*****************************end**************************//

	// ��ѯ���Ӯ����
	virtual int __stdcall GetWinSum( WORD wChairID, BYTE &winsum );

	// ��ѯ��ǰ����
	virtual BYTE __stdcall GetCurGameCount() ;

	// ���õ�ǰ����
	virtual int __stdcall SetGameCount( BYTE count ) ;

	// ��ѯ�ܾ���
	virtual BYTE __stdcall GetAllCount();

	// �����ܾ���
	virtual int __stdcall SetAllCount( BYTE count );

	// ��ѯ����׷�
	virtual DWORD __stdcall GetCellScore();

	// ������ע�׷�
	virtual int __stdcall SetCellScore( DWORD score );

	// ��ȡ��ע��
	virtual DWORD __stdcall GetAllBet( WORD wChairID);

	// ��ѯ���������
	virtual BYTE __stdcall GetMaxChairCount();

	// ������������
	virtual int __stdcall SetMaxChairCount( BYTE count );

	//added by lizhihu 2017.10.20 ������Ϸ�ӱ�ģʽ������ģʽ����ӱ�ģʽ
	virtual int __stdcall SetGameScoreMode(BYTE nMode);

	//�����Ϸ�ӱ�ģʽ������ģʽ����ӱ�ģʽ
	virtual BYTE __stdcall GetGameScoreMode();

	//added by lizhihu 2017.10.21 ������Ϸ��ׯģʽ��0-��ׯ		1-��ׯ		2-�̶�ׯ
	virtual int __stdcall SetRobBankMode(BYTE nMode);

	//��ȡ��Ϸ��ׯģʽ��0-��ׯ		1-��ׯ		2-�̶�ׯ
	virtual BYTE __stdcall GetRobBankMode();

	//���������ׯ����
	virtual int __stdcall SetUserRobScore(const WORD &wChairID, BYTE score);

	//���������ׯ����
	virtual int __stdcall SetUserRobNum(const WORD &wChairID, BYTE cbNum);

	//��������ׯ����
	virtual BYTE __stdcall GetUserRobNum(const WORD &wChairID);

	// ��ѯ��ǰ�����
	virtual BYTE __stdcall GetCurPlayerCount();

	// ��ѯ��ǰ�Թ�����   MaxChair = CurPlayer + CurLooker + voidChair
	virtual BYTE __stdcall GetCurLookerCount();

	// ���õ�ǰׯ��
	virtual int __stdcall SetBankerID(const WORD &wChairID);

	// ��ѯ��ǰׯ��
	virtual WORD __stdcall GetBankerID();

	// ��ȡ�¸����  ��ʱ���ȡ
	virtual WORD __stdcall GetNextUser(const WORD &wChariID);

	// ��ȡ�ϸ����  ˳ʱ���ȡ
	virtual WORD __stdcall GetLastUser(const WORD &wChariID);

	// ��öԼ�
	virtual WORD __stdcall GetOppositeUser(const WORD &wChariID);

	// �����û�״̬���ӷ�������ܻ�ã��û����¡�׼�����Թۡ���Ϸ��
	virtual int __stdcall SetPlayerState( WORD wChairID, DWORD state );

	// ��ѯ�û�״̬���û����¡�׼�����Թۡ���Ϸ��
	virtual DWORD __stdcall GetPlayerState( WORD wChairID);
	
	// ������Ϸ״̬
	virtual int __stdcall SetGameState( DWORD state );

	// ��ѯ��Ϸ״̬
	virtual int __stdcall GetGameState( DWORD &state );

	// ���÷�����򣬴ӷ�������ܻ������,������Ϸid����ׯģʽ���÷ּӱ�ģʽ�ȵ�
	virtual bool __stdcall SetRoomRule(VOID * pData, WORD wDataSize);

	//���÷���
	virtual int __stdcall SetRoomFangzhu(WORD nFangzhu_id);

	//��÷���id
	virtual WORD __stdcall GetRoomFangzhu();

	//��ѯ�û��ĳ�ʼ������Ŀ
	virtual int _stdcall GetPlayerInitCardNum();

	//�����û��ĳ�ʼ������Ŀ
	virtual int _stdcall SetPlayerInitCardNum();

	//����Ͼ�Ӯ�����Ӻ�
	virtual WORD _stdcall GetLastGameWinner();

	//����Ӯ�����Ӻ�
	virtual int _stdcall SetLastGameWinner(const WORD &wChairID);

	//������������ɫ
	virtual int _stdcall SetMainColor(const BYTE &color);

	//��ȡ��������ɫ
	virtual BYTE _stdcall GetMainColor();

	//�����������
	virtual int _stdcall SetLiangZhuUser(const WORD &wChairID);

	//��ȡ�������
	virtual WORD _stdcall GetLiangZhuUser();

	//���õ�ǰ����
	virtual int _stdcall SetLevelValue(const BYTE &value);

	//��ȡ��ǰ����
	virtual BYTE _stdcall GetLevelValue();

	//��õ���
	virtual void _stdcall GetLeaveCard(BYTE *cbLeaveCard, int nLeaveCardSize);

	//�����ҵ�ǰ������Ŀ
	virtual BYTE _stdcall GetUserCurCardNum(const WORD &wChairID);

	//�����ҵ������е����Ƹ���
	virtual BYTE _stdcall GetUserMainColorNum(const WORD &wChairID);

	// ���õ�ǰ�����û�
	virtual int __stdcall SetCurOutCardUser(const WORD &wChairID);

	// ��õ�ǰ�����û�
	virtual WORD __stdcall GetCurOutCardUser();

	// ����һ�������ȿ�ʼ���Ƶ��û�
	virtual int __stdcall SetStartOutCardUser(const WORD &wChairID);

	// ���һ�������ȿ�ʼ���Ƶ��û�
	virtual WORD __stdcall GetStartOutCardUser();

	// ������ҳ��ƿ�������
	virtual bool __stdcall SetUserOutCard(const WORD wChairID, const BYTE *cbCardData, const BYTE &cbCardNum);

	// �����ҳ��ƿ�������
	virtual BYTE __stdcall GetUserOutCard(const WORD wChairID, BYTE *cbCardData);

	// ��õ�����Ŀ
	virtual BYTE __stdcall GetLeaveCardNum();

	// ���õ�ǰ��������
	virtual int __stdcall SetCurFanZhuCard(const BYTE &card);

	// �����ҵ�ǰ����
	virtual int __stdcall GetUserCurLevel(const WORD &wChairID);

	// ���õ�ǰ�������
	virtual int __stdcall SetCurFanzhuUser(const WORD &wChairID);

	// ��õ�ǰ�������
	virtual WORD __stdcall GetCurFanzhuUser();

	// ���õ�ǰ�۵����
	virtual int __stdcall SetCurKoudiUser(const WORD &wChairID);

	// ��õ�ǰ�۵����
	virtual WORD __stdcall GetCurKoudiUser();

	// �����ʼ��ׯ���
	virtual int __stdcall SetStartRobUser(const WORD &wChairID);

	// ����ʼ��ׯ���
	virtual WORD __stdcall GetStartRobUser();

	// ����һ�ֽ���
	virtual int __stdcall SetOneTurnEnd(const BYTE &cbEnd);

	// ���һ�ֽ���
	virtual BYTE __stdcall GetOneTurnEnd();

	// ����һ�������������
	virtual int __stdcall SetTurnMaxCards(BYTE *cbCardData, BYTE cbCardNum);

	// ���һ�������������
	virtual BYTE __stdcall GetTurnMaxCards(BYTE *cbCardData, BYTE &cbCardNum);

	// ���������Ŀ
	virtual BYTE __stdcall GetMagicCardNum(const BYTE cbHandCardData[], BYTE cbHandCardCount);

	// ����������������Ŀ
	virtual BYTE __stdcall GetMagicCardNum(const WORD &wChairID);

	//���õ�ǰ���
	virtual int __stdcall SetMagicCard(const BYTE &cbCard);

	//��õ�ǰ���
	virtual BYTE __stdcall GetMagicCard();

	//�������ը����Ϣ
	virtual int __stdcall SetUserBoomInfo(const WORD &wChairID, const BYTE &cbBoomType);

	//������ը����Ϣ
	virtual BYTE __stdcall GetUserBoomInfo(const WORD &wChairID, tagBoomInfo &strBoomInfo);


protected:
	// ������Ϸ���ü��ؿ�������
	int LoadGameCards( );

	// ������Ϸ���ü����������
	int LoadPlayers( );

	// ��������Ϸ����
	int LoadGameRule(const BYTE cbGameScoreMode);

	// ����������־
	int CreateLog( const wchar_t *_logFile );

	// ����ƾ���Ϣ
	int OutGameInfor();

	// ��ʼ��ȫ������
	int InitGameData( );

	void SetCardData_Leo(const WORD &wChairID, CARD_DESC *cards);

// ���ߺ���
protected:

	// ��������ת��
	virtual int CardTrans_S2C( const CARD_DESC &card_s, BYTE &card_c ); // �����ת�ͻ���
	virtual int CardTrans_C2S( CARD_DESC &card_s, const BYTE &card_c ); // �ͻ���ת������

	int TransListToInt(int iOldType,int iNewType);

public:

	GAME_CARD_CONFIG	m_card_config;		// ��������
	PLAYERS_CONFIG		m_players_config;	// �������
	ROOM_CONFIG			m_room_config;		// ��Ϸ����	
	PLAYING_PARAMETERS	m_playing_para;		// ��Ϸ�в���
	CServerLog          *m_plog;			// ��־
	wchar_t             m_cfgFile[128];		// �����ļ�

	DWORD				m_dPlayerState[MAX_CHAIR_COUNT];		//�����Ϸ���״̬���ڳ�ʼ����Ϸ֮ǰ���õ�

protected:
	static const BYTE	m_cbCardData[FULL_COUNT];			//ԭʼ�˿�����
	
};

#endif