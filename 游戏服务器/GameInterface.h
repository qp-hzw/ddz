#ifndef GAME_INTERFACE
#define GAME_INTERFACE

#pragma once
#include <objbase.h>
#include <iostream>
#include "GameCardConfig.h"
#include "GamePlayerConfig.h"
#include "ServerLog.h"

//�߼�����
#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����


// ��Ϸ������������
typedef struct _tagGAME_CARD_CONFIG
{
	CARD_DESCS		game_cards;		 // ��Ϸ���õ������п��Ƽ���
	DWORD			game_cards_sum;	 // ����������
	BYTE			init_card_num;	 // ��ҳ�ʼ����������������������������108-8��/n��
	BYTE			leave_card_num;	 // �۵���Ŀ��8��9�ţ�
}GAME_CARD_CONFIG;

// �������
typedef struct _tagPLAYERS_CONFIG
{
	PLAYER_DESCS	players;				// �������
	DWORD			player_sum;				// ���������󳤶ȣ���ǰ�����Ӧ��С�ڸ���
	CARD_DESC		hand_cards[MAX_CHAIR_COUNT][MAX_CARD_COUNT];		// ������ƣ�2-20���ˣ�ÿ����5����
}PLAYERS_CONFIG;


// ������Ϸ����
typedef struct _tagROOM_CONFIG
{
	DWORD	kind_id;				// ��Ϸ��ʶ��	
	BYTE	game_count;				// ��Ϸ���þ���	
	DWORD	room_cell;				// ����׷�
	BYTE	max_chair_sum;			// �������������
	WORD	fangzhu_id;				// ����id
	BYTE	game_robBank_mode;		// ��Ϸ����ׯģʽ
	BYTE	game_score_mode;		// �÷�ģʽ������ģʽ����ӱ�ģʽ���ӿ�ܻ�ȡ
	BYTE	game_score_times[MAX_GAME_SCORE_TYPES];	// ���ݵ÷�ģʽ���������ļ��л�ȡ�÷ֵı���������Ĵ�С���������͵Ķ������ã�Ŀǰ18��
}ROOM_CONFIG;

// ��Ϸ�в���
typedef struct _tagPLAYING_PARAMETERS
{
	DWORD	game_state;				// ��ǰ��Ϸ״̬
	BYTE	game_cur_count;			// ��Ϸ��ǰ����
	WORD	banker_id;				// ׯ������ID
	DWORD	start_bet;				// ���俪ʼ��ע����
	DWORD	cur_total_bet;			// ��ǰ������ע�ܷ�
	WORD	last_winner_id;			// �ϰ�Ӯ��

	//������
	BYTE	magic_card;				// ���

	//����
	BYTE	sort_right[LEN_SORT_RIGHT];		// ��ɫ����Ȩλ
	BYTE	main_color;						// ���ƻ�ɫ
	BYTE	level_value;					// ������ֵ����2��ʼ��
	BYTE	nt_value;						// ������ֵ����С����
	BYTE	leave_card[MAX_LEAVE_CARD_NUM];	// �۵׿�������
	WORD	liangzhu_user;					// �������
	BYTE	cur_fanzhu_card;				// ��ǰ��������
	WORD	cur_outcard_user;				// ��ǰ�������
	WORD	start_outcard_user;				// һ�������ȿ�ʼ�������
	BYTE	score_card[MAX_SCORE_CARD_NUM];	// �÷��˿�����
	WORD	cur_fanzhu_user;				// ��ǰ�������
	WORD	cur_koudi_user;					// ��ǰ�۵����
	WORD	start_rob_user;					// ���ȿ�ʼ��ׯ���

	BYTE	turn_max_cards[MAX_CARD_COUNT];	// һ�������Ŀ�������
	BYTE	turn_cards_num;					// һ�������Ŀ������ݴ�С
	BYTE	turn_end;						// һ���Ƿ����	0-����	1-δ����

}PLAYING_PARAMETERS;

interface CGameAccess;
interface CGameLogic;

/************************ ��Ϸ����ӿ� ***************************/
interface CGameLogic
{
	// ������Ϸ
	virtual int __stdcall CreateGame( CGameLogic **pGameRule, CGameAccess **pGameAccess ) = NULL;

	// ���ض�ȡ�������ļ�����
	virtual int __stdcall LoadConfigParameter( const wchar_t *_logFile, const BYTE cbGameScoreMode) = NULL;

	//��ʼ������
	virtual int __stdcall Init() = NULL;

	// �����ע
	virtual int __stdcall Wager( WORD wChairID, DWORD bet  ) = NULL;

	// ������ׯģʽ��ָ��ׯ��
	virtual bool __stdcall AppointBanker( WORD &wChairID) = NULL;

	// ����
	virtual int __stdcall Deal() = NULL;

	// ������Ҳ���
	virtual int __stdcall SetUserOperate( WORD wChairID, BYTE byAction, DWORD dwActionScore ) = NULL;

	// �����û�����
	virtual int __stdcall AnsysCardsType() = NULL;

	// ����÷�
	virtual int __stdcall Settle() = NULL;

	// ����С����Ϸ����
	virtual int __stdcall ClearXjGame() = NULL;

	// ��������Ϸ����
	virtual int __stdcall ClearDjGame() = NULL;

	// ����������
	virtual int __stdcall ClearRoom() = NULL;

	//����ɫȨλ�����˿�
	virtual void __stdcall SortCardList(BYTE *cbCardData, BYTE cbCardCount) = NULL;

	//����ɫȨλ���û���������
	virtual void __stdcall SortCardList(const WORD &wChairID, BYTE cbCardCount, BYTE cbSortType) = NULL;

	//����������
	virtual void __stdcall SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType) = NULL;


	//��ÿ��Ƶ�����ȼ�
	virtual BYTE __stdcall GetCardSortOrder(BYTE cbCardData) = NULL;

	//��������Ƿ�������
	virtual bool __stdcall AnalyseUserLiangZhu(const WORD &wChairID, BYTE *cbLiangZhuColor) = NULL;

	//��������Ƿ��ܷ���
	virtual bool __stdcall AnalyseUserFanZhu(const WORD &wChairID, BYTE *cbFanZhuCard) = NULL;

	//�ж���������Ƿ��ܷ���
	virtual bool __stdcall IsUserCanFanZhu(const WORD &wChairID, const BYTE &cbCurFanZhuColor, BYTE *cbFanZhuCard) = NULL;

	//���͵��Ƹ����
	virtual void __stdcall SendLeaveCardToUser(const WORD &wChairID) = NULL;

	//���һ�鿨�Ƶ�������
	virtual BYTE __stdcall GetCardLogicType(const BYTE *cbCardData, BYTE cbCardCount) = NULL;

	//��ÿ��Ƶ��߼���ɫ
	virtual BYTE __stdcall GetCardLogicColor(BYTE cbCardData) = NULL;

	//��ÿ���������߼���ɫ
	virtual BYTE __stdcall GetCardLogicColor(const BYTE *cbCardData, BYTE cbCardCount) = NULL;

	//��ÿ��Ƶ��߼���ֵ
	virtual BYTE __stdcall GetCardLogicValue(BYTE cbCardData) = NULL;

	//���ſ����Ƿ�����
	virtual bool __stdcall IsLineValue(BYTE cbTurnCard, BYTE cbSecondCard) = NULL;

	//һ�ֱ��ƣ��ж�Ӯ��
	virtual WORD __stdcall GetOneRoundWinner() = NULL;

	//����һ����Ӯ�÷�
	virtual WORD __stdcall SettleOneRound(const WORD &wWinner) = NULL;

	//���һ���е����з���
	virtual BYTE __stdcall GetScoreCard() = NULL;

	//��ȡ���������ܷ���
	virtual WORD __stdcall GetAllCardScore(const BYTE *cbCardData, BYTE cbCardCount) = NULL;

	//�Ƚ���ҳ��ƴ�С
	virtual bool __stdcall CompareUserOutCard(const BYTE &cbFirstCardData, const WORD &cbNextCardData) = NULL;

	//����У��
	virtual bool __stdcall EfficacyOutCard(const WORD &wCheckUser, const BYTE *cbOutCardData, const BYTE &cbOutCardCount) = NULL;

	//����ָ����ɫ��ȡ�˿�
	virtual BYTE __stdcall DistillCardByColor(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardColor, BYTE cbResultCard[]) = NULL;

	//��ȡ�˿�
	virtual BYTE __stdcall DistillCardByCount(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbSameCount, tagSameDataInfo &SameDataInfo) = NULL;

	//��ȡ�˿�
	virtual BYTE __stdcall DistillTrackorByCount(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbSameCount, tagTractorDataInfo &TractorDataInfo) = NULL;

	//�ж���������Ľ�����Ŀ
	virtual BYTE __stdcall GetIntersectionCount(const BYTE cbCardData1[], BYTE cbCardCount1, const BYTE cbCardData2[], BYTE cbCardCount2) = NULL;

	//����һ�ֵĿ�ʼ������ң�������ҿ��Գ�����
	virtual bool __stdcall AnalyseOutCardNotify(const WORD &wChairID, const BYTE *OutCardData, 
											   const BYTE &cbOutCardNum, tagOutCardNotify *cbOutCardNotify) = NULL;

	//�ж��û��������Ƿ���ָ����ɫ�ĸ����͵Ŀ���
	virtual bool __stdcall IsHaveSpecificType(const WORD &wChairID, const BYTE &color, const BYTE &cbOutCardType, 
												const BYTE &cbOutCardNum, tagOutCardNotify &cbOutCardNotify) = NULL;

	//���㳭�׵÷�
	virtual SCORE __stdcall CalculateChaodiScore(const BYTE &cbCDcardNum) = NULL;

	//���ݵ÷ּ��������������
	virtual BYTE __stdcall AnalyseUpGrade(const SCORE &score) = NULL;

	//�������Ƚϴ�С
	virtual bool __stdcall AnalysebCompare(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult &AnalyseResult, int dCardType) = NULL;

	//�����������˿�
	virtual void __stdcall AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult &AnalyseResult) = NULL;

	//��������ȡ���ͽ��
	virtual int __stdcall GetType(tagAnalyseResult AnalyseResult,const BYTE cbCardData[],BYTE cbCardCount,const BYTE NoChangeCard[]) = NULL;

	//��������
public:
	//��ÿ��ƻ�ɫ
	BYTE GetCardColor(BYTE cbCardData) {return ((cbCardData&MASK_COLOR)>>4); }
	//��ÿ��Ƶ���
	BYTE GetCardValue(BYTE cbCardData) {return (cbCardData&MASK_VALUE); }
	//�˿�����ת��������
	virtual BYTE __stdcall SwitchToCardData(BYTE cbCardIndex) = NULL;
	//�˿�����ת��������
	virtual BYTE __stdcall SwitchToCardIndex(BYTE cbCardData) = NULL;
	//�˿���������ת����������
	virtual BYTE __stdcall SwitchToCardData(const BYTE cbCardIndex[LEN_MAX_INDEX], BYTE cbCardData[MAX_CARD_COUNT]) = NULL;
	//�˿���������ת������������
	virtual BYTE __stdcall SwitchToCardIndex(const BYTE *cbCardData, BYTE cbCardCount, BYTE *cbCardIndex) = NULL;
	//ɾ���˿�
	virtual bool __stdcall RemoveCard(const BYTE *cbRemoveCard, BYTE cbRemoveCount, BYTE *cbCardData, BYTE cbCardCount) = NULL;
	//�����������ɾ���˿�
	virtual bool __stdcall RemoveCard(const WORD &wChairID, const BYTE *cbRemoveCard, BYTE cbRemoveCount) = NULL;
};

/***************  ��Ϸ��Ϣ��ѯ�ӿ� ***********************/
interface CGameAccess
{
	// ��ѯ��ǰ��Ϸ����ID
	virtual int __stdcall SetKindID(DWORD id) = NULL;

	// ��ѯ��ǰ��Ϸ����ID
	virtual DWORD __stdcall GetKindID() = NULL;

	// ��ѯ�ͻ��˵�����
	virtual int __stdcall GetClientHandCards( WORD wChairID, BYTE *cards, const int &cardsum ) = NULL;

	// ��ѯ���С�ֵ÷�
	virtual SCORE __stdcall GetPlayerSingleScore( WORD wChairID ) = NULL;

	// ��ѯ��ҷ������ܵ÷�
	virtual SCORE __stdcall GetPlayerTotalScore( WORD wChairID ) = NULL;

	// ��ѯ���Ӯ����
	virtual int __stdcall GetWinSum( WORD wChairID, BYTE &winsum ) = NULL;

	// ��ѯ��ǰ����
	virtual BYTE __stdcall GetCurGameCount() = NULL;

	// ���õ�ǰ����
	virtual int __stdcall SetGameCount( BYTE count ) = NULL;

	// ��ѯ�ܾ���
	virtual BYTE __stdcall GetAllCount() = NULL;

	// �����ܾ���
	virtual int __stdcall SetAllCount( BYTE count ) = NULL;

	// ��ѯ����׷�
	virtual DWORD __stdcall GetCellScore() = NULL;

	// ���÷���׷�
	virtual int __stdcall SetCellScore( DWORD score ) = NULL;

	//****************��ѯ/�����û���Ϸ��״̬*********************//

	// ���������ׯ��ׯ
	virtual int __stdcall SetBankerState( WORD wChairID, BYTE care  ) = NULL;

	// ��ȡ�����ׯ״̬
	virtual BYTE __stdcall GetBankerState( WORD wChairID) = NULL;

	// ��ѯ�����ע��ɱ�־
	virtual BYTE __stdcall GetAddScoreState( WORD wChairID) = NULL;

	// ���������ע��ɱ�־
	virtual int __stdcall SetAddScoreState( WORD wChairID, BYTE beted ) = NULL;

	// ��ѯ��Ҷ�����ɱ�־
	virtual BYTE __stdcall GetUserOperateState( WORD wChairID) = NULL;

	// ������Ҷ�����ɱ�־
	virtual int __stdcall SetUserOperateState( WORD wChairID,  BYTE state ) = NULL;

	// ��ѯ���̯��
	virtual BYTE __stdcall GetShowCardState( WORD wChairID ) = NULL;

	// �������̯��
	virtual int __stdcall SetShowCardState( WORD wChairID, BYTE state ) = NULL;

	// ��ѯ���׼��״̬
	virtual BYTE __stdcall GetXjReadyState( WORD wChairID ) = NULL;
	
	// �������׼��
	virtual int __stdcall SetXjReadyState( WORD wChairID, BYTE ready ) = NULL;

	//*****************************end**************************//

	// ��ȡ��ע��
	virtual DWORD __stdcall GetAllBet( WORD wChairID) = NULL;

	// ��ѯ���������
	virtual BYTE __stdcall GetMaxChairCount() = NULL;

	// �������������
	virtual int __stdcall SetMaxChairCount( BYTE count ) = NULL;

	//added by lizhihu 2017.10.20 ������Ϸ�ӱ�ģʽ������ģʽ����ӱ�ģʽ
	virtual int __stdcall SetGameScoreMode(BYTE nMode) = NULL;

	//�����Ϸ�ӱ�ģʽ
	virtual BYTE __stdcall GetGameScoreMode() = NULL;

	// ��ѯ��ǰ�����
	virtual BYTE __stdcall GetCurPlayerCount() = NULL;

	// ��ѯ��ǰ�Թ�����   MaxChair = CurPlayer + CurLooker + voidChair
	virtual BYTE __stdcall GetCurLookerCount() = NULL;

	// ���õ�ǰׯ��
	virtual int __stdcall SetBankerID(const WORD &wChairID) = NULL;

	// ��ѯ��ǰׯ��
	virtual WORD __stdcall GetBankerID() = NULL;

	// ��ȡ�¸����  ��ʱ���ȡ
	virtual WORD __stdcall GetNextUser(const WORD &wChariID) = NULL;

	// ��ȡ�ϸ����  ˳ʱ���ȡ
	virtual WORD __stdcall GetLastUser(const WORD &wChariID) = NULL;

	// ��öԼ�
	virtual WORD __stdcall GetOppositeUser(const WORD &wChariID) = NULL;

	// �����û�״̬���ӷ�������ܻ�ã��û����¡�׼�����Թۡ���Ϸ��
	virtual int __stdcall SetPlayerState( WORD wChairID, DWORD state ) = NULL;

	// ��ѯ�û�״̬���û����¡�׼�����Թۡ���Ϸ��
	virtual DWORD __stdcall GetPlayerState( WORD wChairID) = NULL;
	
	// ������Ϸ״̬
	virtual int __stdcall SetGameState( DWORD state ) = NULL;

	// ��ѯ��Ϸ״̬
	virtual int __stdcall GetGameState( DWORD &state ) = NULL;

	// ���÷������ added by lizhihu 2017.10.17 ʹ�� void* + size ȡ���̶��ṹ��
	virtual bool __stdcall SetRoomRule(VOID * pData, WORD wDataSize) = 0;

	//������Ϸ��ׯģʽ��0-��ׯ		1-��ׯ		2-�̶�ׯ
	virtual int __stdcall SetRobBankMode(BYTE nMode) = NULL;

	//�����Ϸ��ׯģʽ��0-��ׯ		1-��ׯ		2-�̶�ׯ
	virtual BYTE __stdcall GetRobBankMode() = NULL;

	//���������ׯ����
	virtual int __stdcall SetUserRobScore(const WORD &wChairID, BYTE score) = NULL;

	//���������ׯ����
	virtual int __stdcall SetUserRobNum(const WORD &wChairID, BYTE cbNum) = NULL;

	//��������ׯ����
	virtual BYTE __stdcall GetUserRobNum(const WORD &wChairID) = NULL;

	//���÷���
	virtual int __stdcall SetRoomFangzhu(WORD nFangzhu_id) = NULL;

	//��÷���id
	virtual WORD __stdcall GetRoomFangzhu() = NULL;

	//��ѯ�û��ĳ�ʼ������Ŀ
	virtual int _stdcall GetPlayerInitCardNum() = NULL;

	//�����û��ĳ�ʼ������Ŀ
	virtual int _stdcall SetPlayerInitCardNum() = NULL;

	//����Ͼ�Ӯ�����Ӻ�
	virtual WORD _stdcall GetLastGameWinner() = NULL;

	//����Ӯ�����Ӻ�
	virtual int _stdcall SetLastGameWinner(const WORD &wChairID) = NULL;

	//������������ɫ
	virtual int _stdcall SetMainColor(const BYTE &color) = NULL;

	//��ȡ��������ɫ
	virtual BYTE _stdcall GetMainColor() = NULL;

	//�����������
	virtual int _stdcall SetLiangZhuUser(const WORD &wChairID) = NULL;

	//��ȡ�������
	virtual WORD _stdcall GetLiangZhuUser() = NULL;

	//���õ�ǰ����
	virtual int _stdcall SetLevelValue(const BYTE &value) = NULL;

	//��ȡ��ǰ����
	virtual BYTE _stdcall GetLevelValue() = NULL;

	//��õ���
	virtual void _stdcall GetLeaveCard(BYTE *cbLeaveCard, int nLeaveCardSize) = NULL;

	//�����ҵ�ǰ������Ŀ
	virtual BYTE _stdcall GetUserCurCardNum(const WORD &wChairID) = NULL;

	//�����ҵ������е����Ƹ���
	virtual BYTE _stdcall GetUserMainColorNum(const WORD &wChairID) = NULL;

	// ���õ�ǰ�����û�
	virtual int __stdcall SetCurOutCardUser(const WORD &wChairID) = NULL;

	// ��õ�ǰ�����û�
	virtual WORD __stdcall GetCurOutCardUser() = NULL;

	// ����һ�������ȿ�ʼ���Ƶ��û�
	virtual int __stdcall SetStartOutCardUser(const WORD &wChairID) = NULL;

	// ���һ�������ȿ�ʼ���Ƶ��û�
	virtual WORD __stdcall GetStartOutCardUser() = NULL;

	// ������ҳ��ƿ�������
	virtual bool __stdcall SetUserOutCard(const WORD wChairID, const BYTE *cbCardData, const BYTE &cbCardNum) = NULL;

	// �����ҳ��ƿ�������
	virtual BYTE __stdcall GetUserOutCard(const WORD wChairID, BYTE *cbCardData) = NULL;

	// ��õ�����Ŀ
	virtual BYTE __stdcall GetLeaveCardNum() = NULL;

	// ���õ�ǰ��������
	virtual int __stdcall SetCurFanZhuCard(const BYTE &card) = NULL;

	// �����ҵ�ǰ����
	virtual int __stdcall GetUserCurLevel(const WORD &wChairID) = NULL;

	// ���õ�ǰ�������
	virtual int __stdcall SetCurFanzhuUser(const WORD &wChairID) = NULL;

	// ��õ�ǰ�������
	virtual WORD __stdcall GetCurFanzhuUser() = NULL;

	// ���õ�ǰ�۵����
	virtual int __stdcall SetCurKoudiUser(const WORD &wChairID) = NULL;

	// ��õ�ǰ�۵����
	virtual WORD __stdcall GetCurKoudiUser() = NULL;

	// �����ʼ��ׯ���
	virtual int __stdcall SetStartRobUser(const WORD &wChairID) = NULL;

	// ����ʼ��ׯ���
	virtual WORD __stdcall GetStartRobUser() = NULL;

	// ����һ�ֽ���
	virtual int __stdcall SetOneTurnEnd(const BYTE &cbEnd) = NULL;

	// ���һ�ֽ���
	virtual BYTE __stdcall GetOneTurnEnd() = NULL;

	// ����һ�������������
	virtual int __stdcall SetTurnMaxCards(BYTE *cbCardData, BYTE cbCardNum) = NULL;

	// ���һ�������������
	virtual BYTE __stdcall GetTurnMaxCards(BYTE *cbCardData, BYTE &cbCardNum) = NULL;

	// ���������Ŀ
	virtual BYTE __stdcall GetMagicCardNum(const BYTE cbHandCardData[], BYTE cbHandCardCount) = NULL;

	// ����������������Ŀ
	virtual BYTE __stdcall GetMagicCardNum(const WORD &wChairID) = NULL;

	//���õ�ǰ���
	virtual int __stdcall SetMagicCard(const BYTE &cbCard) = NULL;

	//��õ�ǰ���
	virtual BYTE __stdcall GetMagicCard() = NULL;

	//�������ը����Ϣ
	virtual int __stdcall SetUserBoomInfo(const WORD &wChairID, const BYTE &cbBoomType) = NULL;

	//������ը����Ϣ
	virtual BYTE __stdcall GetUserBoomInfo(const WORD &wChairID, tagBoomInfo &strBoomInfo) = NULL;

	
};

#endif