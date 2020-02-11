#ifndef _SUB_RULE_MANAGER_H_
#define _SUB_RULE_MANAGER_H_

# include "stdafx.h"
# include <string>
using std::string;

//��������
struct MATCH_CONFIG
{
	//�׶���Ϣ
	struct STAGE_INFO
	{
		TCHAR	szName[128];	//�����������������..
		DWORD	wXJCount;					//С����
		DWORD	wPlayerCount;				//����
		DWORD	wJinJi_Rule;				//��������   ͬ���Ƚ�/ȫ���Ƚ� 1,2
		DWORD	wJinJi_Count;				//��������
	};

	//��������Ϣ
	DWORD		wMatchID;							//������ID��ʶ
	TCHAR		szTitle[128];			//����
	TCHAR		szDescribe[128];		//��������
	STAGE_INFO	stage[5];				//�׶���Ϣ
	DWORD		wStageSize;							//�׶θ���
	DWORD		llBaseScore;						//�׷�

	//����������
	DWORD		dwStartTime;						//��ʼʱ�� ��-1��ʾ����������
	DWORD		dwTimePeriod;						//��������
	DWORD		wStartType;							//��ʼ����  (0-������ʼ  1-��ʱ��ʼ)
	DWORD		dwLowestPlayer;						//���ٲ�������
	DWORD		FeeType;							//���������� 0-��� 1-שʯ 2-��Ʊ
	DWORD		llFee;								//������
	DWORD		llReword;							//����

	//��������Ϣ
	DWORD		dwRobotCount;						//����������
	DWORD		dwRobotJoinMin;						//��������С��������
	DWORD		dwRobotJoinMax;						//����������������
};

class CSubRuleManager : ISubRoomRuleManager
{
private:
	static CSubRuleManager			* s_instance;
	tagSubGameRule			m_SubRule;				//����Ϸ����

	//����ӿ�
public:
	//��������
	virtual bool StartService(){return true;}
	//ֹͣ����
	virtual bool ConcludeService(){return true;}

public:
	static CSubRuleManager * instance();

	//frame�ӿ�
public:
	//��ȡ�ֶ� ��Ӧ������
	virtual string GetDescribe(string key_name);
	//�����ֶ�����, Ϊ�ṹ���Ӧ�ֶθ�ֵ
	virtual void SetRoomRule(string key, string value);
	//��ʼ��
	virtual void Init();
	//����Gold������
	virtual void SetGoldRule(BYTE byType);
	//����match����
	virtual void SetMatchRule(VOID *config);

	//sub�ӿ�
public:
	//sub��ȡ����Ϸ����
	tagSubGameRule GetSubGameRule();

	//��������
public:
	//�����ֶ�����, Ϊ�ṹ���Ӧ�ֶθ�ֵ
	void SetRoomRule(tagSubGameRule &subrule, string key_name, string value);
};

#endif