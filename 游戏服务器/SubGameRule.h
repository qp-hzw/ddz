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

	// �����û�����
	virtual int __stdcall AnsysCardsType( );

	// ����÷�
	virtual int __stdcall Settle();

//���
protected:
	//�������
	int SettleUpGrade();

	//���������
	int SettleFightLandLord();
};

#endif