#include "stdafx.h"
#include "SubRuleManager.h"

tagSubGameRule						CSubRuleManager::m_SubRule;			//房间子游戏规则


//设置子游戏规则
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
extern "C" _declspec(dllexport) void SetSubGameRule(string key_name[20], string key_value[20])
{
	CSubRuleManager::SetSubGameRule(key_name, key_value);
}

extern "C" _declspec(dllexport) void GetDescribe(string key_name)
{
	CSubRuleManager::GetDescribe(key_name);
}