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

//根据字段名字, 为结构体对应字段赋值
void CSubRuleManager::SetRoomRule( string key_name, string value)
{
	if(key_name == "底分")
	{
		m_SubRule.Cellscore = atoi(value.c_str());
	}
	else if(key_name == "模式")
	{
		m_SubRule.GameWanFa = atoi(value.c_str());
	}
	else if(key_name == "抢庄")
	{
		m_SubRule.GameDiZhu = atoi(value.c_str());
	}
	else if(key_name == "封顶")
	{
		m_SubRule.GameFengDing = atoi(value.c_str());
	}
	else if(key_name == "洗牌")
	{
		m_SubRule.DontCutCards = atoi(value.c_str());
	}
	else if(key_name == "加倍")
	{
		m_SubRule.AddMultiple = atoi(value.c_str());
	}
	else if(key_name == "明牌")
	{
		m_SubRule.ShowCards = atoi(value.c_str());
	}
	else if(key_name == "翻倍")
	{
		m_SubRule.BaseCardAddMultiple = atoi(value.c_str());
	}
}

//设置Gold场规则
void CSubRuleManager::SetGoldRule(VOID* roominfo)  //这里可以加特殊规则的字段
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

//设置match规则
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