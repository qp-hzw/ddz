#ifndef _SUB_RULE_MANAGER_H_
#define _SUB_RULE_MANAGER_H_

# include "stdafx.h"
# include <string>
using std::string;

class CSubRuleManager
{
public:
	static tagSubGameRule			m_SubRule;				//子游戏规则

	//sub接口
public:
	//sub获取子游戏规则
	static tagSubGameRule GetSubGameRule();

	//frame接口函数
public:
	//设置子游戏规则
	static void SetSubGameRule(string key_name[20], string key_value[20]);
	//获取字段对应的描述
	static string GetDescribe(string key_name);

	//辅助函数
public:
	//根据字段名字, 为结构体对应字段赋值
	static void SetRoomRule(tagSubGameRule &subrule, string key_name, string value);
};

#endif