#ifndef _SUB_RULE_MANAGER_H_
#define _SUB_RULE_MANAGER_H_

# include "stdafx.h"
# include <string>
using std::string;

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