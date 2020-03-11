#include "stdafx.h"
#include "SubRuleManager.h"

CSubRuleManager	*					CSubRuleManager::s_instance = NULL;

CSubRuleManager * CSubRuleManager::instance()
{
	if (s_instance == NULL)
	{
		s_instance = new CSubRuleManager();
		memset(&(s_instance->m_SubRule), 0, sizeof(s_instance->m_SubRule));
	}

	return s_instance;
}

//��ʼ��
void CSubRuleManager::Init()
{
	memset(&(s_instance->m_SubRule), 0, sizeof(s_instance->m_SubRule));
}

//�����ֶ�����, Ϊ�ṹ���Ӧ�ֶθ�ֵ
void CSubRuleManager::SetRoomRule( string key_name, string value)
{
	if(key_name == "�׷�")
	{
		m_SubRule.Cellscore = atoi(value.c_str());
	}
	else if(key_name == "ģʽ")
	{
		m_SubRule.GameWanFa = atoi(value.c_str());
	}
	else if(key_name == "��ׯ")
	{
		m_SubRule.GameDiZhu = atoi(value.c_str());
	}
	else if(key_name == "�ⶥ")
	{
		m_SubRule.GameFengDing = atoi(value.c_str());
	}
	else if(key_name == "ϴ��")
	{
		m_SubRule.DontCutCards = atoi(value.c_str());
	}
	else if(key_name == "�ӱ�")
	{
		m_SubRule.AddMultiple = atoi(value.c_str());
	}
	else if(key_name == "����")
	{
		m_SubRule.ShowCards = atoi(value.c_str());
	}
	else if(key_name == "����")
	{
		m_SubRule.BaseCardAddMultiple = atoi(value.c_str());
	}
}

//����Gold������
void CSubRuleManager::SetGoldRule(VOID* roominfo)  //������Լ����������ֶ�
{
	STR_CMD_GC_USER_GOLD_INFO *info = (STR_CMD_GC_USER_GOLD_INFO *)roominfo;
	m_SubRule.Cellscore = info->dwScore;
	m_SubRule.BaseCardAddMultiple = 1;
	m_SubRule.GameWanFa = 0;
	m_SubRule.GameDiZhu = 0;
	m_SubRule.GameFengDing = 0;
	m_SubRule.DontCutCards = 1;
	m_SubRule.AddMultiple = 1;
	m_SubRule.ShowCards = 1;
}

//����match����
void CSubRuleManager::SetMatchRule(VOID *config)
{
	MATCH_CONFIG *cfg = (MATCH_CONFIG *)config;
	m_SubRule.Cellscore = cfg->llBaseScore;
	m_SubRule.BaseCardAddMultiple = 0;
	m_SubRule.GameWanFa = 0;
	m_SubRule.GameDiZhu = 0;
	m_SubRule.GameFengDing = 0;
	m_SubRule.DontCutCards = 0;
	m_SubRule.AddMultiple = 1;
	m_SubRule.ShowCards = 0;
}

//sub��ȡ����Ϸ����
tagSubGameRule CSubRuleManager::GetSubGameRule()
{
	return m_SubRule;
}

extern "C" __declspec(dllexport) VOID * GetSubRuleManager()
{
	//��������
	CSubRuleManager * pSubRoleManger = NULL;
	try
	{
		pSubRoleManger = CSubRuleManager::instance();
		if (pSubRoleManger == NULL)
			throw TEXT("����ʧ��");

		return pSubRoleManger;
	}
	catch (...) {}

	//�������
	SafeDelete(pSubRoleManger);
	return NULL;
}