
#pragma once
#include <windows.h>
#include "cfg.h"
typedef BYTE *CARD_SCOR_RANK,	CARD_SCOR;	// ֽ�Ƶ�����������
typedef BYTE *CARD_COLOR_RANK,	CARD_COLOR;	// ֽ�ƻ�ɫ��������
typedef BYTE *CARD_DESCS,		CARD_DESC;	// ֽ������

#define __GETSCORE( A )			CGameCardConfig::GetCardScore( A )		// ��ȡ�������Ƶ���
#define __GETGROUPID( A )		CGameCardConfig::GetCardGroup( A )		// ��ȡ��������������ID
#define __GETCOLOR( A )			CGameCardConfig::GetCardColor( A )		// ��ȡ�������ƻ�ɫ

#define MAX_CARD_GROUP_NUM			20				//���Ŀ���������Ŀ
#define MAX_TOTAL_CARD_NUM			108				//Ĭ�ϵĿ�������

// ��������
typedef struct _tagCARD_GOURP
{
	//BYTE			id;			// ���ʶ��
	//CARD_SCOR		begin;		// ��ʼ��
	//CARD_SCOR		end;		// ��β��
	//CARD_COLOR_RANK	colors;		// ��ɫ��� 1����; 2÷��; 3����; 4����
	//BYTE			clrsum;		// ��ɫ����
	//BYTE			sum;		// ����
	BYTE			id;			// ���ʶ��
	BYTE			color;		// ��ɫ
	CARD_SCOR		begin;		// ��ʼ��
	CARD_SCOR		end;		// ��β��
	BYTE			sum;		// ͬһ�����м���
}CARD_GOURP, *CARD_GOURPS;

class CServerLog;

class CGameCardConfig
{
public:
	CGameCardConfig(void) {}
	~CGameCardConfig(void) {}


// ����ֽ������
public:
/**
 * ��ȡһ���ƵĹ�������
 * ʹ�� xml �����ļ� ����һ���Ƶ�����
 * @param  configfile					[in]			�����ļ�
 * @param  cards_sum					[in-out]		�˿�����Ŀ
 * @param  m_plog						[in]			��־��¼
 * return					0				����
 *							-1				�쳣
 */
static int LoadCardsSum(DWORD &cards_sum);


/**
* ��ȡ��ţ��Ϸ�ĵ÷ּӱ�ģʽ
* ʹ�� xml �����ļ� ������Ϸ�ĵ÷ּӱ�ģʽ
* @param  configfile					[in]			�����ļ�
* @param  game_score_Mode				[in]			��Ϸ�÷�ģʽ���������ģʽ
* @param  game_score_times				[in-out]		��Ϸ�÷ּӱ�����
* @param  m_plog						[in]			��־��¼
* return					0				����
*							-1				�쳣
*/
static int LoadGameScoreTimes(BYTE game_score_Mode, BYTE *game_score_times);


/**
 * ����� 
 * ����XML �� ��������ڵ���Ϣ 
 * @param  configfile				[in]			�����ļ�
 * @param  ppgroups					[out]			�˿�����
 * @param  group_sum				[in-out]		��ʵ������
 * @m_plog					[in]			��־��¼
 * return					0				����
 *							-1				�쳣
 */
static int LoadCardGroups(CARD_GOURP *groups, DWORD &group_sum);

/**
 * ��ȡ������
 * �˺������˿��б����ҳ��������� group ������ֽ�Ʋ���䵽 cards �����У� ������ִ����ɺ� cards �����еĳ�Ա��
 * @param		groups					[in]			�˿��� ����
 * @param		groups_len				[in]			�˿��� ������󳤶ȣ�ע�⣬����Ҫ��������ʵ�ʳ�Ա����
 * @param		card_rank				[in]			�˿˵�������
 * @param		card_ranks_sum			[in]			�˿˵���������Ŀ
 * @param		ppcards					[out]			�˿�����
 * @param		cardslen				[in-out]		�˿����鳤��,������ⳤ�ȣ�����ʵ�ʳ���
 */
static int TransGroups(const CARD_GOURP group[MAX_CARD_GROUP_NUM], const int &nGroupLen,
										CARD_DESC *pGameCards, DWORD &dwCardSum);

// �����ຯ��
public:

	/** 
	 * ������������, ϴ��
	 * ÿ�ֿ�ʼʱ, ���Ƶ�˳�����
	 * @param		dest_cards			[in-out]		Ŀ�����飬 ����������
	 * @param		dest_len			[in]			Ŀ�����鳤��
	 * @param		src_cards			[in]			Դ�����飬 �������ļ����ɵ���Ϸ����
	 * @param		src_len				[in]			Դ�������� ע�⣬�����ݱ����������ʵ�ʳ���
	 */
	static int OutOrder(CARD_DESCS dest_cards,
									const DWORD dest_len,
									const CARD_DESCS source_cards,
									const DWORD src_len,
									const BYTE nMode,
									CARD_DESCS boom_cards,
									BYTE &boomsum);

// 
public:

	/**
	 * ��ָ����������ʶ������ת��Ϊֽ�������е�����
	 * �˺������˿��б����ҳ��������� group ������ֽ�Ʋ���䵽 cards �����У� ������ִ����ɺ� cards �����еĳ�Ա��
	 * @param		group					[in]			�˿���
	 * @param		card_ranks				[in]			�˿�����
	 * @param		card_ranks_sum			[in]			�˿�������Ŀ
	 * @param		cards					[in-out]		�˿�����
	 * @param		cardslen				[in]			�˿����鳤��
	 * @param		curlen					[in-out]		�Ѿ����ĳ��ȣ� ע�⣬�˲����Ὣ����ֵ������ȥ��ֵ���
	 *														���Դ˲��������ֵ������ֵ�й�
	 */
	static int TransGroup(const CARD_GOURP *group,
										CARD_DESCS cards,
										const DWORD maxlen,
										DWORD &curIndex);

	/**
	 * ����һ����������
	 * �˺���������һ��arlen ���ȵ��������飨0��src_len-1������䵽array��0��src_len-1λ��
	 * @param			randarray			[in-out]		����	
	 * @param			src_len				[in]			���鳤��	
	 * @param			m_plog				[in]			��־	
	 */
	static int CreateOutOrderArray( DWORD randarray[], const int src_len );

	/**
	 * �Ƚ������ƴ�С
	 * @param			self			[in]		��	
	 * @param			rival			[in]		��	
	 * @param			flag			[in-out]	���أ� ��self ��, �򷵻�TRUE, ���򷵻�FLASE	
	 */
	static int CompareCards( const CARD_DESC *self, const CARD_DESC *rival, BOOL *flag);

	////////////////////////////////////////////////////////
	//��ϴ��ģʽ��ϴ���㷨       by  lih
	static int CreateBuXiPaiArray(BYTE randarray[], const int src_len);
	/////////////////////////////////////////////////////////

	/**
	 * ������
	 * �Ȱ������ţ�Ȼ�󰴻�ɫ��
	 * @param			cards				[in-out]		����	
	 * @param			cardslen			[in]			���鳤��	
	 * @param			card_ranks			[in]			��������	
	 * @param			card_ranks_sum		[in]			�������򳤶�	
	 * @param			color_ranks			[in]			��ɫ����	
	 * @param			color_ranks_sum		[in]			��ɫ���򳤶�	
	 */
	static int OrderCards(	CARD_DESCS cards, 
							const DWORD cardslen );

	/**
	 * ��ȡ���Ƶ���
	 */
	static BYTE GetCardScore( CARD_DESC card );

	/**
	 * ��ȡ���ƻ�ɫ
	 */
	static BYTE GetCardColor(CARD_DESC card);

	/**
	 * ��ȡ������
	 */
	static BYTE GetCardGroup( CARD_DESC card );

	//��ϴ��ģʽ�Ŀ�����������ֵ
	static const BYTE	m_CardData[FULL_COUNT];			//ԭʼ�˿�����

	//�������͵���ֵӳ�亯��
	static BYTE   MapOfCardValue(BYTE CardData);

	static bool flushcard(BYTE card[], int &cardsum, BYTE boomcard[], int &boomsum);
};

