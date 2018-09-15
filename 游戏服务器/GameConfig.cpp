#include "stdafx.h"
#include "GameConfig.h"
#include "ServerLog.h"

#import "msxml6.dll"
using namespace MSXML2;
#include <time.h>

COM_PROFILE_RULE	gComGamePara;		// ��Ϸ����
COM_CARD_CONFIG		gComCardPara;		// ��������
COM_PLAYER_CONFIG	gComPlayerPara;		// �������

CServerLog          *m_plog;			//��־�ļ�
wchar_t             m_cfgFile[128];		//�����ļ�


CGameConfig::CGameConfig(void)
{

}

CGameConfig::~CGameConfig(void)
{

}


//�������ļ��л�ȡͨ�õ���Ϸ���ã�����Ϸ��ʼǰ����ȫ�ֱ���
int CGameConfig::LoadGameCommonConfig()
{
	int result = 0;

	// ������־
	CServerLog::GetLogInstance( L"SUB_GAMES_LOG.txt", &m_plog );

	// �������ļ�
	if(0 == BindCfgFile())
	{
		m_plog->LogSingleLine(L"��ȡ�����ļ�ʧ��, δ�ҵ�KindID", NULL );
		return 1;
	}

	// ������Ϸ���ƺ���ң���������Ʊ�������
	result = CreateGameCards();		// �������ƣ���pGameRule->m_card_config��ֵ
	if ( 0 == result )
	{
		result = CreatePlayers(); // ������ң���pGameRule->m_players_config��ֵ
		if ( 0 != result )
		{
			m_plog->LogSingleLine(L"��������쳣", NULL );
		}
	}
	else
	{
		m_plog->LogSingleLine(L"���ÿ����쳣", NULL );
	}

	return result;
}

// ������Ϸ���ô�����������
int CGameConfig::CreateGameCards( )
{
	int result = 0;

	//��ȡ��
	result = CGameCardConfig::LoadCardGroups(m_cfgFile, gComCardPara.group, gComCardPara.groupNum, m_plog);  

	if (0 == result)
	{
		// ��ȡ������һ��ֽ�Ƶ������ͷ���ÿ���û�������
		result = CGameCardConfig::LoadCardsSum(m_cfgFile, gComCardPara.game_cards_num, m_plog); 

		if (0 == result)
		{
			result = CGameCardConfig::TransGroups(gComCardPara.group, gComCardPara.groupNum, gComCardPara.game_cards, gComCardPara.game_cards_num, m_plog);	// ������
			if (0 == result)
			{
				//��ȡ�����ļ���Ӧ�����Ʊ���
				CGameCardConfig::LoadGameScoreTimes(m_cfgFile, GAME_SCORE_MODE_CLASSIC, gComGamePara.game_normal_times, m_plog);

				//��÷��ӱ�����
				CGameCardConfig::LoadGameScoreTimes(m_cfgFile, GAME_SCORE_MODE_CRAZY, gComGamePara.game_crazy_times, m_plog);

				if (result == -1)
				{
					m_plog->LogSingleLine(L"�����÷ֱ���ʧ��", NULL);
				}
			}
		}
		else
		{
			m_plog->LogSingleLine(L"������ʧ��", NULL);
			result = -1;
		}
	}
	else
	{
		m_plog->LogSingleLine(L"����ʧ��", NULL);
		result = -1;
	}

	return result;
}

// ������Ϸ���ô����������
int CGameConfig::CreatePlayers()
{
	int result = 0;

	gComPlayerPara.type = 0;

	// �������ļ��н������������
	//PLAYER_TYPES��һ��word�洢����ҵ���������ID,card_sum��player_sum
	result = CGamePlayerConfig::LoadPlayerDesc(	m_cfgFile, gComPlayerPara.type, m_plog );

	return result;
}

// �������ļ�
DWORD CGameConfig::BindCfgFile()
{
	swprintf_s(m_cfgFile, sizeof m_cfgFile / sizeof(wchar_t), L"%ld_%ld.xml", KIND_ID, NODE_ID);

	return KIND_ID;
}