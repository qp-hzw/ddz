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

//初始化
void CSubRuleManager::Init()
{
	memset(&(s_instance->m_SubRule), 0, sizeof(s_instance->m_SubRule));
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

//设置Gold场规则
void CSubRuleManager::SetGoldRule(BYTE byType)  //这里可以加特殊规则的字段
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

//设置match规则
void CSubRuleManager::SetMatchRule(VOID *config)
{
	MATCH_CONFIG *cfg = (MATCH_CONFIG *)config;
	cout << cfg->llBaseScore << endl;
	m_SubRule.Cellscore = cfg->llBaseScore;
}

//sub获取子游戏规则
tagSubGameRule CSubRuleManager::GetSubGameRule()
{
	return m_SubRule;
}

extern "C" __declspec(dllexport) VOID * GetSubRuleManager()
{
	//建立对象
	CSubRuleManager * pSubRoleManger = NULL;
	try
	{
		pSubRoleManger = CSubRuleManager::instance();
		if (pSubRoleManger == NULL)
			throw TEXT("创建失败");

		return pSubRoleManger;
	}
	catch (...) {}

	//清理对象
	SafeDelete(pSubRoleManger);
	return NULL;
}