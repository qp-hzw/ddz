#ifndef GAME_RULE_L
#define GAME_RULE_L

#pragma once
#include "GameData.h"

class CSubGameRule : public CGameData
{
public:
	CSubGameRule(void) {}
	~CSubGameRule(void) {}

/** ��Ϸ����ӿ� **/
public:

	// ������ׯģʽ��ָ��ׯ��
	virtual bool __stdcall AppointBanker( WORD &wChairID );

	// �����û�����
	virtual int __stdcall AnsysCardsType( );

	// ����÷�
	virtual int __stdcall Settle();


/** ��ׯ���� **/
protected:
	 // ��ׯ����
    bool AppointBankerTURN(WORD &wChairID);

    // ��ׯ����
    bool AppointBankerFREE(WORD &wChairID);

	// �̶�ׯ����
    bool AppointBankerFIXED(WORD &wChairID);

//���
protected:
	//�������
	int SettleUpGrade();

	//���������
	int SettleFightLandLord();
};

#endif