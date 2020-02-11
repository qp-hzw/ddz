#ifndef _SUB_RULE_MANAGER_H_
#define _SUB_RULE_MANAGER_H_

# include "stdafx.h"
# include <string>
using std::string;

//比赛配置
struct MATCH_CONFIG
{
	//阶段信息
	struct STAGE_INFO
	{
		TCHAR	szName[128];	//初赛、半决赛、决赛..
		DWORD	wXJCount;					//小局数
		DWORD	wPlayerCount;				//人数
		DWORD	wJinJi_Rule;				//晋级规则   同桌比较/全桌比较 1,2
		DWORD	wJinJi_Count;				//晋级名额
	};

	//比赛场信息
	DWORD		wMatchID;							//比赛场ID标识
	TCHAR		szTitle[128];			//标题
	TCHAR		szDescribe[128];		//比赛描述
	STAGE_INFO	stage[5];				//阶段信息
	DWORD		wStageSize;							//阶段个数
	DWORD		llBaseScore;						//底分

	//比赛场属性
	DWORD		dwStartTime;						//开始时间 （-1表示人满即开）
	DWORD		dwTimePeriod;						//创建周期
	DWORD		wStartType;							//开始类型  (0-人满开始  1-定时开始)
	DWORD		dwLowestPlayer;						//最少参赛人数
	DWORD		FeeType;							//报名费类型 0-金币 1-砖石 2-门票
	DWORD		llFee;								//报名费
	DWORD		llReword;							//奖励

	//机器人信息
	DWORD		dwRobotCount;						//机器人数量
	DWORD		dwRobotJoinMin;						//机器人最小加入周期
	DWORD		dwRobotJoinMax;						//机器人最大加入周期
};

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
	//设置Gold场规则
	virtual void SetGoldRule(BYTE byType);
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