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

//��ȡ�ֶ� ��Ӧ������
string CSubRuleManager::GetDescribe(string key_name)
{
	string describe;
	if(key_name == "Cellscore")
	{
		describe = "�׷�";
	}
	else if(key_name == "GameWanFa")
	{
		describe = "�淨";
	}
	else if(key_name == "GameDiZhu")
	{
		describe = "�е���ģʽ";
	}
	else if(key_name == "GameFengDing")
	{
		describe = "�ⶥ����";
	}
	else if(key_name == "DontCutCards")
	{
		describe = "��ϴ��";
	}
	else if(key_name == "AddMultiple")
	{
		describe = "�ӱ�";
	}
	else if(key_name == "ShowCards")
	{
		describe = "����";
	}
	else if(key_name == "BaseCardAddMultiple")
	{
		describe = "���Ƽӱ�";
	}

	return describe;
}
//�����ֶ�����, Ϊ�ṹ���Ӧ�ֶθ�ֵ
void CSubRuleManager::SetRoomRule( string key_name, string value)
{
	if(key_name == "Cellscore")
	{
		m_SubRule.Cellscore = atoi(value.c_str());
	}
	else if(key_name == "GameWanFa")
	{
		m_SubRule.GameWanFa = atoi(value.c_str());
	}
	else if(key_name == "GameDiZhu")
	{
		m_SubRule.GameDiZhu = atoi(value.c_str());
	}
	else if(key_name == "GameFengDing")
	{
		m_SubRule.GameFengDing = atoi(value.c_str());
	}
	else if(key_name == "DontCutCards")
	{
		m_SubRule.DontCutCards = atoi(value.c_str());
	}
	else if(key_name == "AddMultiple")
	{
		m_SubRule.AddMultiple = atoi(value.c_str());
	}
	else if(key_name == "ShowCards")
	{
		m_SubRule.ShowCards = atoi(value.c_str());
	}
	else if(key_name == "BaseCardAddMultiple")
	{
		m_SubRule.BaseCardAddMultiple = atoi(value.c_str());
	}
}

//����Gold������
void CSubRuleManager::SetGoldRule(BYTE byType)  //������Լ����������ֶ�
{
	m_SubRule.Cellscore = 1;
	m_SubRule.BaseCardAddMultiple = 1;
	m_SubRule.GameWanFa = 0;
	m_SubRule.GameDiZhu = 0;
	m_SubRule.GameFengDing = 64;
	m_SubRule.DontCutCards = 1;
	m_SubRule.AddMultiple = 0;
	m_SubRule.ShowCards = 0;
}

//����match����
void CSubRuleManager::SetMatchRule(VOID *config)
{
	MATCH_CONFIG *cfg = (MATCH_CONFIG *)config;
	cout << cfg->llBaseScore << endl;
	m_SubRule.Cellscore = cfg->llBaseScore;
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