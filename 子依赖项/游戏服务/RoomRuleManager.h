#ifndef ROOM_RULE_MANAGER
#define ROOM_RULE_MANAGER
#include <string>
#include "Stdafx.h"
using std::string;


//桌子接口
class ISubRoomRuleManager : public IUnknownEx
{
	//管理接口
public:
	//启动服务
	virtual bool StartService(){return true;}
	//停止服务
	virtual bool ConcludeService(){return true;}

	//
public:
	//获取字段 对应的描述
	virtual string GetDescribe(string key_name)= NULL;
	//根据字段名字, 为结构体对应字段赋值
	virtual void SetRoomRule(string key, string value)= NULL;
	//
	virtual void Init() = NULL;
};

/*
class RoomRuleManager
{
private:
	static RoomRuleManager*         _instance;  
	ISubRoomRuleManager*            m_SubRoomRuleManager;
	rule_arry						m_rule_arry;						//房间规则
	BYTE							m_frame_rule_count;					//frame房间规则 条数
	BYTE							m_subgame_rule_count;				//子游戏房间规则 条数

public:
	static  RoomRuleManager* Instance();

public:
	//获取房间配置选项
	rule_arry GetRoomRuleSetting();
	//获取房间规则
	void GetRoomRule(tagTableRule& roomrule, byte value[20]);

public:
	//读取子游戏房间配置文件
	void ReadSubGameRoomRule(int kindid);

	//辅助函数
private:
	//获取字段 对应的描述
	virtual string GetDescribe(string key_name);
	//根据字段名字, 为结构体对应字段赋值
	virtual void SetRoomRule(tagTableRule &roomrule, string key, byte value);

	//根据字段名字, 找到对应的字段值
	string GetRoomValByKey(string key, byte value);
};
*/
#endif