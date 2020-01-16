#pragma once
#include <windows.h>
#include "cfg.h"
typedef		WORD	PLAYER_TYPE;  // �����������

#define __GETCARDSUM( _type )		CGamePlayerConfig::GetPlayerCardSum( _type ) // ��ȡ�����������

// �������
typedef struct _tagPLAYER_DESC
{
	PLAYER_TYPE			type;				// ���TYPE�����飬ÿ��������ĳ���û����͵�id�������Ŀ��������Ŀ
	BYTE				card_type;			// ��������ID
	int					times;				// ���Ʊ���
	DWORD				bet;				// �����ע����
	BYTE				bank_state;			// �����ׯ		0-��	1-�е���	2-������
	BYTE				rob_num;			// �����ׯ����
	DWORD               rob_score;          // �����ׯ�ķ����� ����"������"�е�ɸ�ӵ���
	DWORD				play_state;			// ���״̬
	BYTE				showcard_state;		// ̯��״̬			0 δ��ɣ�	1 ���
	BYTE				xj_ready_state;		// С�����׼��			0 δ׼����	1 ׼��
	BYTE				bet_state;			// �����ע״̬		0 δ��ɣ�	1 ���
	BYTE				mingpai_state;		// �������״̬		0 δ��ɣ�	1 ���
	BYTE				action_state;		// ��Ҷ���״̬		0 δ���,   1 ���
	BYTE				action;				// ��Ҷ���			1-�� 2-�� 3-�� 4-��
	SCORE				single_score;		// ���ֵ÷�
	SCORE				total_score;		// ����ܵ÷�
	BYTE				winsum;				// Ӯ����
	DWORD				bank_count;			// ��ׯ�Ĵ���
	WORD				addscore_bet;		// ��ע�ı���
	WORD				mingpai_bet;		// ���Ƶı���
	SCORE				best_bet;			// ���һ����л�ù��������
	BASE_PLAYERINFO		player_info;				// ��¼�����Ϣ
	DWORD				start_mingpai;		//�Ƿ�ʼ��Ϸ����
	WORD				jiaofen_state;		//��¼��ҽеķ�
	WORD				spring_state;		//��¼�Ƿ������
	WORD				spring;				//��¼�Ƿ���
	WORD				player_tuo_guan;	//����Ƿ��й�   0-���й�
	WORD				out_time_num;		//��ҳ��Ƴ�ʱ����

	BYTE				hand_card_num;						// ��ǰ������
	BYTE				out_card_num;						// ��Ҹ��ֵĳ�����Ŀ	
	BYTE				out_card_info[MAX_CARD_COUNT];		// ��Ҹ��ֳ��Ŀ�������
	BYTE				level;				//��Ҽ�������2��ʼ��

	//������
	BYTE				out_card_pass;		//�����Ƿ��	0-��������	1-��
	tagBoomInfo			boom_info;			//ը����Ϣ

}PLAYER_DESC, *PLAYER_DESCS;

class CGamePlayerConfig
{
public:
	CGamePlayerConfig() {}
	~CGamePlayerConfig(void) {}


	// �����������
public:

	/**
	 * �������е������ ����� ���������
	 * @param		configfile				[in]			�˿��� ����
	 * @param		ppplayers				[out]			�˿��� ����
	 * @param		player_sum				[in-out]		�˿��� ����
	 * @param		m_plog					[in]			��־�ӿ�
	 */
	static int LoadPlayerDesc(CCfg& ff, PLAYER_TYPE &player_type);

	/**
	 * ����������������鹹�������������
	 * @param		dst_player_types	in]			Ŀ�����������
	 * @param		player_sum			[in-out]	��������鳤��
	 * @param		src_player_type		[in]		Դ�������
	 * @param		m_plog				[in]		��������鳤��
	 */
	static int CreatePlayerDesc( PLAYER_DESCS &dst_player_types, const DWORD player_sum, const PLAYER_TYPE &src_player_type );

public:
	/**
	 * �������������ID
	 */
	static void SetPlayerGroupID( PLAYER_TYPE &_type, BYTE _id); 
		
	/**
	 * ������������� 
	 *	TODONOW  added by WangChengQing
	 *  1. ���ݴ�framework����   2. û������palytypeID, ���Ե��������ļ��е��������ID����
	 */
	static void SetPlayerSum( PLAYER_TYPE &_type, BYTE _sum); 

	/**
	 * �������������������
	 */
	static void SetPlayerCardSum( PLAYER_TYPE &_type, BYTE _card_sum );

	/**
	 * ��ȡ���������ID
	 */
	static BYTE GetPlayerGroupID( PLAYER_TYPE _type); 

	/**
	 * ��ȡ���������
	 */
	static BYTE GetPlayerSum( PLAYER_TYPE _type); 

	/**
	 * ��ȡ���������������
	 */
	static BYTE GetPlayerCardSum( PLAYER_TYPE _type );
};