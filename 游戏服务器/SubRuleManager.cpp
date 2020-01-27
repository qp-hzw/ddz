#include "stdafx.h"
#include "SubRuleManager.h"

tagSubGameRule						CSubRuleManager::m_SubRule;			//��������Ϸ����


//��������Ϸ����
void CSubRuleManager::SetSubGameRule(string key_name, string key_value)
{
	if (!key_name.empty())
		SetRoomRule(m_SubRule, key_name, key_value);
}

//sub��ȡ����Ϸ����
tagSubGameRule CSubRuleManager::GetSubGameRule()
{
	return m_SubRule;
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
void CSubRuleManager::SetRoomRule(tagSubGameRule &subrule, string key_name, string value)
{
	if(key_name == "Cellscore")
	{
		subrule.Cellscore = atoi(value.c_str());
	}
	else if(key_name == "GameWanFa")
	{
		subrule.GameWanFa = atoi(value.c_str());
	}
	else if(key_name == "GameDiZhu")
	{
		subrule.GameDiZhu = atoi(value.c_str());
	}
	else if(key_name == "GameFengDing")
	{
		subrule.GameFengDing = atoi(value.c_str());
	}
	else if(key_name == "DontCutCards")
	{
		subrule.DontCutCards = atoi(value.c_str());
	}
	else if(key_name == "AddMultiple")
	{
		subrule.AddMultiple = atoi(value.c_str());
	}
	else if(key_name == "ShowCards")
	{
		subrule.ShowCards = atoi(value.c_str());
	}
	else if(key_name == "BaseCardAddMultiple")
	{
		subrule.BaseCardAddMultiple = atoi(value.c_str());
	}
}

//��������
extern "C" __declspec(dllexport) VOID * SetSubGameRule(string key_name, string key_value)
{
	std::cout << "key_name: " << key_name << "key_value: "<< key_value <<  std::endl;
	CSubRuleManager::SetSubGameRule(key_name, key_value);
	return NULL;
}

extern "C" __declspec(dllexport) string GetDescribe(string key_name)
{
	std::cout << "key_name: " << key_name << std::endl;
	return CSubRuleManager::GetDescribe(key_name);
}