#include "stdafx.h"
#include "GameConfig.h"

#include <time.h>

std::string      CGameConfig::cfg_file;             // �����ļ�

COM_PROFILE_RULE	CGameConfig::gComGamePara ;	    // ��Ϸ����
COM_CARD_CONFIG		CGameConfig::gComCardPara;		// ��������
COM_PLAYER_CONFIG	CGameConfig::gComPlayerPara;    // �������
 

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

	// �������ļ�
	if (0 == BindCfgFile())
	{
		return -1;
	}

	//���ļ�
	result = CCfg::Instance()->OpenFile(cfg_file.c_str());
	if(result != 0)
	{
		cout << result << endl;
		return - 2;
	}

	// ������Ϸ���ƺ���ң���������Ʊ�������
	result = CreateGameCards();		// �������ƣ���pGameRule->m_card_config��ֵ
	if (0 == result)
	{
		result = CreatePlayers(); // ������ң���pGameRule->m_players_config��ֵ
		if (0 != result)
		{
			////CLog::Log(log_error, "��������쳣");
		}
	}
	else
	{
		////CLog::Log(log_error, "���ÿ����쳣");
	}

	//�ر��ļ�
	CCfg::Instance()->CloseFile();

	return result;
}

// ������Ϸ���ô�����������
int CGameConfig::CreateGameCards()
{
	int result = 0;

	//��ȡ��
	result = CGameCardConfig::LoadCardGroups(gComCardPara.group, gComCardPara.groupNum);

	if (0 == result)
	{
		// ��ȡ������һ��ֽ�Ƶ������ͷ���ÿ���û�������
		result = CGameCardConfig::LoadCardsSum(gComCardPara.game_cards_num);
		if (0 == result)
		{
			result = CGameCardConfig::TransGroups(gComCardPara.group, gComCardPara.groupNum, gComCardPara.game_cards, gComCardPara.game_cards_num);	// ������
			if (0 == result)
			{
				//��ȡ�����ļ���Ӧ�����Ʊ���
				CGameCardConfig::LoadGameScoreTimes(GAME_SCORE_MODE_CLASSIC, gComGamePara.game_normal_times);
				//��÷��ӱ�����
				CGameCardConfig::LoadGameScoreTimes(GAME_SCORE_MODE_CRAZY, gComGamePara.game_crazy_times);
			}
			if (result == -1)
			{
				////CLog::Log(log_error, "�����÷ֱ���ʧ��");
			}
		}
		else
		{
			////CLog::Log(log_error, "������ʧ��");
			result = -1;
		}
	}
	else
	{
		////CLog::Log(log_error, "����ʧ��");
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
	result = CGamePlayerConfig::LoadPlayerDesc(gComPlayerPara.type);

	return result;

}

// �������ļ�
DWORD CGameConfig::BindCfgFile()
{
	//cfg_file = "./subgame/"
	//	+ std::to_string(KIND_ID)
	//	+ ".cfg";

	cfg_file = std::to_string(KIND_ID)+ ".cfg";

	return KIND_ID;
}