#include "stdafx.h"
#include "SubRuleManager.h"

tagSubGameRule						CSubRuleManager::m_SubRule;			//��������Ϸ����


//��������Ϸ����
void CSubRuleManager::SetSubGameRule(string key_name[20], string key_value[20])
{
	for(int i=0; i<20; i++)
	{
		if(!key_name[i].empty())
		{
			SetRoomRule(m_SubRule, key_name[i], key_value[i]);
		}
	}
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
extern "C" _declspec(dllexport) void SetSubGameRule(string key_name[20], string key_value[20])
{
	CSubRuleManager::SetSubGameRule(key_name, key_value);
}

extern "C" _declspec(dllexport) void GetDescribe(string key_name)
{
	CSubRuleManager::GetDescribe(key_name);
}