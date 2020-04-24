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
	//根据字段名字, 为结构体对应字段赋值
	virtual void SetRoomRule(string key, string value);
	//初始化
	virtual void Init();
	//设置Gold场规则
	virtual void SetGoldRule(VOID* roominfo);
	//设置match规则
	virtual void SetMatchRule(VOID *config);

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