#include "stdafx.h"
#include "SubRuleManager.h"

tagSubGameRule						CSubRuleManager::m_SubRule;			//房间子游戏规则


//设置子游戏规则
void CSubRuleManager::SetSubGameRule(string key_name, string key_value)
{
	if (!key_name.empty())
		SetRoomRule(m_SubRule, key_name, key_value);
}

//sub获取子游戏规则
tagSubGameRule CSubRuleManager::GetSubGameRule()
{
	return m_SubRule;
}

//获取字段 对应的描述
string CSubRuleManager::GetDescribe(string key_name)
{
	string describe;
	if(key_name == "Cellscore")
	{
		describe = "底分";
	}
	else if(key_name == "GameWanFa")
	{
		describe = "玩法";
	}
	else if(key_name == "GameDiZhu")
	{
		describe = "叫地主模式";
	}
	else if(key_name == "GameFengDing")
	{
		describe = "封顶倍数";
	}
	else if(key_name == "DontCutCards")
	{
		describe = "不洗牌";
	}
	else if(key_name == "AddMultiple")
	{
		describe = "加倍";
	}
	else if(key_name == "ShowCards")
	{
		describe = "明牌";
	}
	else if(key_name == "BaseCardAddMultiple")
	{
		describe = "底牌加倍";
	}

	return describe;
}

//根据字段名字, 为结构体对应字段赋值
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

//导出定义
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