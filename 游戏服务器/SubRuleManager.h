#ifndef _SUB_RULE_MANAGER_H_
#define _SUB_RULE_MANAGER_H_

# include "stdafx.h"
# include <string>
using std::string;

class CSubRuleManager : ISubRoomRuleManager
{
private:
	static CSubRuleManager			* s_instance;
	tagSubGameRule			m_SubRule;				//子游戏规则

	//管理接口
public:
	//启动服务
	virtual bool StartService(){return true;}
	//停止服务
	virtual bool ConcludeService(){return true;}

public:
	static CSubRuleManager * instance();

	//frame接口
public:
	//获取字段 对应的描述
	virtual string GetDescribe(string key_name);
	//根据字段名字, 为结构体对应字段赋值
	virtual void SetRoomRule(string key, string value);
	//初始化
	virtual void Init();

	//sub接口
public:
	//sub获取子游戏规则
	tagSubGameRule GetSubGameRule();

	//辅助函数
public:
	//根据字段名字, 为结构体对应字段赋值
	void SetRoomRule(tagSubGameRule &subrule, string key_name, string value);
};

#endif