#ifndef _SUB_RULE_MANAGER_H_
#define _SUB_RULE_MANAGER_H_

# include "stdafx.h"
# include <string>
using std::string;

class CSubRuleManager
{
private:
	static tagSubGameRule			m_SubRule;				//����Ϸ����

private:
	static CSubRuleManager			* s_instance;

	//sub�ӿ�
public:
	//sub��ȡ����Ϸ����
	static tagSubGameRule GetSubGameRule();

	// instance()
	static CSubRuleManager * instance();

	//frame�ӿں���
public:
	//��������Ϸ����
	static void SetSubGameRule(string key_name, string key_value);
	//��ȡ�ֶζ�Ӧ������
	static string GetDescribe(string key_name);

	//��������
public:
	//�����ֶ�����, Ϊ�ṹ���Ӧ�ֶθ�ֵ
	static void SetRoomRule(tagSubGameRule &subrule, string key_name, string value);
};

#endif