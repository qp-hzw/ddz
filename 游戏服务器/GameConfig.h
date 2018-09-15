#ifndef GAME_CONFIG_L
#define GAME_CONFIG_L
#pragma once
#include <windows.h>
#include "GamePlayerConfig.h"
#include "GameCardConfig.h"
#include "ServerLog.h"
#include "GameInterface.h"
#include "GameData.h"
#include "SubGameRule.h"

//�������ļ���ȡ���淨����
typedef struct _tagCommonProfilePara
{
	BYTE	game_normal_times[MAX_GAME_SCORE_TYPES];	// ����÷�ģʽ���������ļ��л�ȡ�÷ֵı�����Ŀǰ27��
	BYTE	game_crazy_times[MAX_GAME_SCORE_TYPES];		// ���÷�ģʽ���������ļ��л�ȡ�÷ֵı�����Ŀǰ27��

}COM_PROFILE_RULE;

//ͨ��������ã��������ļ��ж�ȡ
typedef struct _tagCommonPlayerPara
{
	PLAYER_TYPE			type;					// ���TYPE�����飬ÿ��������ĳ���û����������Ŀ��������Ŀ			
}COM_PLAYER_CONFIG;

//ͨ�ÿ������ã��������ļ��ж�ȡ
typedef struct _tagCommonCardPara
{
	CARD_GOURP group[MAX_CARD_GROUP_NUM];		//���Ƶ����飨����÷����
	DWORD groupNum;								//����ĸ���
	CARD_DESC game_cards[MAX_TOTAL_CARD_NUM];	//��Ϸ����
	DWORD game_cards_num;						//ʵ�ʿ�������
}COM_CARD_CONFIG;

//ͨ�������ļ�ȫ�ֱ���

extern  COM_PROFILE_RULE	gComGamePara;		//ͨ�õ���Ϸ���ò������������ļ���ã�
extern	COM_CARD_CONFIG		gComCardPara;		// ��������
extern	COM_PLAYER_CONFIG	gComPlayerPara;		// �������

extern	CServerLog          *m_plog;			//��־�ļ�
extern	wchar_t             m_cfgFile[128];		//�����ļ�


//��dll���ص�ʱ�� ��ȡ�����ļ�
class CGameConfig
{
public:
	CGameConfig(void);
	~CGameConfig(void);

private:
	static int CreateGameCards( );

	static int CreatePlayers( );

	static int CreateLog( const wchar_t *_logFile );

public:
	//�������ļ��л�ȡͨ�õ���Ϸ���ã�����Ϸ��ʼǰ����ȫ�ֱ���
	static int LoadGameCommonConfig();
	//��ȡ�����ļ�����
	static DWORD BindCfgFile();
};
#endif