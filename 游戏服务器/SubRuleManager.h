#ifndef _SUB_RULE_MANAGER_H_
#define _SUB_RULE_MANAGER_H_

# include "stdafx.h"
# include <string>
using std::string;

class CSubRuleManager
{
public:
	static tagSubGameRule			m_SubRule;				//����Ϸ����

	//sub�ӿ�
public:
	//sub��ȡ����Ϸ����
	static tagSubGameRule GetSubGameRule();

	//frame�ӿں���
public:
	//��������Ϸ����
	static void SetSubGameRule(string key_name[20], string key_value[20]);
	//��ȡ�ֶζ�Ӧ������
	static string GetDescribe(string key_name);

	//��������
public:
	//�����ֶ�����, Ϊ�ṹ���Ӧ�ֶθ�ֵ
	static void SetRoomRule(tagSubGameRule &subrule, string key_name, string value);
};

#endif