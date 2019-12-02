#pragma once

//19.10.24    by lih

# include <ctime>
# include <vector>
# include "Stdafx.h"
# include <iostream>
using namespace std;


//����Ȩֵ�ṹ
struct HandCardValue
{
	int SumValue;		//���Ƶļ�ֵ
	int NeedRound;		//��Ҫ������
};

//����������ݽṹ
struct CardGroupData
{
	//ö������
	CardGroupType cgType = cgERROR;
	//���Ƶļ�ֵ
	int  nValue = 0;
	//���Ƶĸ���
	int  nCount = 0;
	//���о�����С����ֵ�����ڶԱ�
	int nMaxCard = 0;
};

//��Ϸȫ����
class GameSituation
{
public:
	//���캯��
	GameSituation()
	{
	}
	//��������
	virtual ~GameSituation()
	{
	}

public:


	//�������
	int nDiZhuID = -1;
	//���ֽз�
	int nLandScore = 0;

	//��ǰ������ҡ�����δȷ��
	int nNowDiZhuID = -1;
	//��ǰ���ֽз֡�����δȷ��
	int nNowLandScore = 0;

	//���ŵ���
	int DiPai[3] = { 0 };
	//�Ѿ�������ơ���״̬��¼������һЩ���㣬ֵ��Ϊ��index�ƶ�Ӧ������0~4
	int value_aAllOutCardList[18] = { 0 };
	//��������Ѿ���������Ƽ�¼
	//int value_aUnitOutCardList[3][18] = { 0 };
	//��������Ѿ�ʣ�����Ƹ���
	int value_aUnitHandCardCount[3] = { 0 };
	//���ֵ�ǰ�׷ֱ���
	int nMultiple = 0;
	//��ǰ���ֶ������������Ƿ����������������Լ��Ƿ�����Ѿ��������ƴӶ���ȥ�ܶ��ѣ�
	int nCardDroit = 0;
	//��ǰ�������
	int NowOutUser;
	//��ǰ����Ƶ��������ݣ���������ʱ��Ҹ�����������ɸѡ
	CardGroupData uctNowCardGroup;
	//������Ϸ�Ƿ����
	bool Over = false;


};

//����������
class HandCardData
{


public:
	//���캯��
	HandCardData()
	{
	}
	//��������
	virtual ~HandCardData()
	{
	}

public:

	vector <int>		value_nHandCardList;					//�������С����޻�ɫ��ֵ��3~17
	int					value_aHandCardList[18] = { 0 };				//�������С���״̬��¼������һЩ���㣬ֵ��Ϊ��index�ƶ�Ӧ������0~4
	vector <int>		color_nHandCardList;					//�������С����л�ɫ�����մӴ�С������  56��52������С������4~0����3��3��3��3
	int					nHandCardCount = 17;							//���Ƹ���
	int					nGameRole = -1;									//��ҽ�ɫ��λ       0������    1��ũ�񡪡������¼�   2��ũ�񡪡������ϼ�
	int					nOwnIndex = -1;									//�����λID 
	CardGroupData		uctPutCardType;						//���Ҫ���ȥ��������
	vector <int>		value_nPutCardList;					//Ҫ���ȥ���ơ����޻�ɫ
	vector <int>		color_nPutCardList;					//Ҫ���ȥ���ơ����л�ɫ
	HandCardValue		 uctHandCardValue;


public:

	//Ҫ��������������
	void ClearPutCardList();

	//�������򣬴��ƿ�ǰ
	void SortAsList(vector <int> &arr);

	//��һ���ƣ����ز����Ƿ�Ϸ�
	bool PutOneCard(int value_nCard, int &clear_nCard);

	//��һ���ƣ����ز����Ƿ�Ϸ�
	bool PutCards();

	//ͨ���л�ɫ���ƻ�ȡ�޻�ɫ���ƣ��������ֽṹ��
	void get_valueHandCardList();

	//��ʼ��
	void Init(BYTE * CurCardData, BYTE CurCardNum);

	//������г�Ա���������ڲ���
	void PrintAll();

	//��ÿ��Ƶ��߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);

};

//�����߼���
class AILogic
{
public:
	//���캯��
	AILogic()
	{

	}

	//��������
	~AILogic()
	{

	}

public:

	//���ʣ������Ƿ���һ�ֳ���   �ǣ�����������������   �񣺷��ش�������
	CardGroupData ins_SurCardsType(int arr[]);

	//vector����
	CardGroupData ins_SurCardsType(vector<int> list);

	/*
	��װ�õĻ�ȡ����������Ͻṹ����
		CardGroupType cgType������
		int MaxCard��������С����ֵ
		int Count������
		����ֵ��CardGroupData
	*/
	CardGroupData get_GroupData(CardGroupType cgType, int MaxCard, int Count);

	/*
	ͨ������dp�ķ�ʽ��ȡ���Ƽ�ֵ
		��get_PutCardList��Ϊ����ݹ����
		���أ���ֵ�ṹ��HandCardValue
		Ȩֵ�ļ������ο�ͷ�ļ������߼�˼ά
	*/
	HandCardValue get_HandCardValue(HandCardData &clsHandCardData);

	//��һ����ȫ������
	void Put_All_SurCards(GameSituation &clsGameSituation, HandCardData &clsHandCardData, CardGroupData SurCardGroupData);
	void Put_All_SurCards(HandCardData &clsHandCardData, CardGroupData SurCardGroupData);


	// ���ݳ������ƾ�����ǰԤ��������ơ�����������
	void get_PutCardList_2_limit(GameSituation &clsGameSituation, HandCardData &clsHandCardData);

	// ��������
	void get_PutCardList_2_unlimit(HandCardData &clsHandCardData);

	//��ȡ�зֺ���
	int LandScore( HandCardData &clsHandCardData);

};