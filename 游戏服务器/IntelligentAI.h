#pragma once

//19.10.24    by lih

# include <ctime>
# include <vector>
# include <list>
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
	CardGroupType cgType;
	//���Ƶļ�ֵ
	int  nValue;
	//���Ƶĸ���
	int  nCount ;
	//���о�����С����ֵ�����ڶԱ�
	int nMaxCard ;
};

//��Ϸȫ����
class GameSituation
{
public:
	//���캯��
	GameSituation()
	{
			//�������
	 nDiZhuID = -1;
	//���ֽз�
	 nLandScore = 0;

	//��ǰ������ҡ�����δȷ��
	 nNowDiZhuID = -1;
	//��ǰ���ֽз֡�����δȷ��
	 nNowLandScore = 0;

	//���ֵ�ǰ�׷ֱ���
	 nMultiple = 0;
	//��ǰ���ֶ������������Ƿ����������������Լ��Ƿ�����Ѿ��������ƴӶ���ȥ�ܶ��ѣ�
	 nCardDroit = 0;

	}
	//��������
	virtual ~GameSituation()
	{
	}

public:
	//�������
	int nDiZhuID;
	//���ֽз�
	int nLandScore;

	//��ǰ������ҡ�����δȷ��
	int nNowDiZhuID;
	//��ǰ���ֽз֡�����δȷ��
	int nNowLandScore ;

	//���ŵ���
	int DiPai[3] ;
	//�Ѿ�������ơ���״̬��¼������һЩ���㣬ֵ��Ϊ��index�ƶ�Ӧ������0~4
	int value_aAllOutCardList[18] ;
	//��������Ѿ���������Ƽ�¼
	//int value_aUnitOutCardList[3][18] = { 0 };
	//��������Ѿ�ʣ�����Ƹ���
	int value_aUnitHandCardCount[3] ;
	//���ֵ�ǰ�׷ֱ���
	int nMultiple ;
	//��ǰ���ֶ������������Ƿ����������������Լ��Ƿ�����Ѿ��������ƴӶ���ȥ�ܶ��ѣ�
	int nCardDroit ;
	//��ǰ�������
	int NowOutUser;
	//��ǰ����Ƶ��������ݣ���������ʱ��Ҹ�����������ɸѡ
	CardGroupData uctNowCardGroup;
	//������Ϸ�Ƿ����
	bool Over ;


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

	list <int>		value_nHandCardList;					//�������С����޻�ɫ��ֵ��3~17
	int					value_aHandCardList[18] ;				//�������С���״̬��¼������һЩ���㣬ֵ��Ϊ��index�ƶ�Ӧ������0~4
	list <int>		color_nHandCardList;					//�������С����л�ɫ�����մӴ�С������  56��52������С������4~0����3��3��3��3
	int					nHandCardCount ;							//���Ƹ���
	int					nGameRole ;									//��ҽ�ɫ��λ       0������    1��ũ�񡪡������¼�   2��ũ�񡪡������ϼ�
	int					nOwnIndex ;									//�����λID 
	CardGroupData		uctPutCardType;						//���Ҫ���ȥ��������
	list <int>			value_nPutCardList;					//Ҫ���ȥ���ơ����޻�ɫ
	list <int>		color_nPutCardList;					//Ҫ���ȥ���ơ����л�ɫ
	HandCardValue		 uctHandCardValue;


public:

	//Ҫ��������������
	void ClearPutCardList();

	//�������򣬴��ƿ�ǰ
	void SortAsList(list <int> &arr);

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
	CardGroupData ins_SurCardsType(list<int> mlist);

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