#pragma once

//19.10.24    by lih

# include <ctime>
# include <vector>
# include "Stdafx.h"
# include <iostream>
using namespace std;


//手牌权值结构
struct HandCardValue
{
	int SumValue;		//手牌的价值
	int NeedRound;		//需要打几手牌
};

//牌型组合数据结构
struct CardGroupData
{
	//枚举类型
	CardGroupType cgType = cgERROR;
	//该牌的价值
	int  nValue = 0;
	//含牌的个数
	int  nCount = 0;
	//牌中决定大小的牌值，用于对比
	int nMaxCard = 0;
};

//游戏全局类
class GameSituation
{
public:
	//构造函数
	GameSituation()
	{
	}
	//析构函数
	virtual ~GameSituation()
	{
	}

public:


	//地主玩家
	int nDiZhuID = -1;
	//本局叫分
	int nLandScore = 0;

	//当前地主玩家——还未确定
	int nNowDiZhuID = -1;
	//当前本局叫分——还未确定
	int nNowLandScore = 0;

	//三张底牌
	int DiPai[3] = { 0 };
	//已经打出的牌——状态记录，便于一些计算，值域为该index牌对应的数量0~4
	int value_aAllOutCardList[18] = { 0 };
	//三名玩家已经打出的手牌记录
	//int value_aUnitOutCardList[3][18] = { 0 };
	//三名玩家已经剩余手牌个数
	int value_aUnitHandCardCount[3] = { 0 };
	//本局当前底分倍数
	int nMultiple = 0;
	//当前控手对象（用于区分是否可以自身任意出牌以及是否地主已经放弃出牌从而不去管队友）
	int nCardDroit = 0;
	//当前出牌玩家
	int NowOutUser;
	//当前打出牌的类型数据，被动出牌时玩家根据这里做出筛选
	CardGroupData uctNowCardGroup;
	//本局游戏是否结束
	bool Over = false;


};

//手牌数据类
class HandCardData
{


public:
	//构造函数
	HandCardData()
	{
	}
	//析构函数
	virtual ~HandCardData()
	{
	}

public:

	vector <int>		value_nHandCardList;					//手牌序列——无花色，值域3~17
	int					value_aHandCardList[18] = { 0 };				//手牌序列——状态记录，便于一些计算，值域为该index牌对应的数量0~4
	vector <int>		color_nHandCardList;					//手牌序列——有花色，按照从大到小的排列  56，52：大王小王……4~0：红3黑3方3花3
	int					nHandCardCount = 17;							//手牌个数
	int					nGameRole = -1;									//玩家角色地位       0：地主    1：农民——地主下家   2：农民——地主上家
	int					nOwnIndex = -1;									//玩家座位ID 
	CardGroupData		uctPutCardType;						//玩家要打出去的牌类型
	vector <int>		value_nPutCardList;					//要打出去的牌——无花色
	vector <int>		color_nPutCardList;					//要打出去的牌——有花色
	HandCardValue		 uctHandCardValue;


public:

	//要打出的牌序列清空
	void ClearPutCardList();

	//手牌排序，大牌靠前
	void SortAsList(vector <int> &arr);

	//出一张牌，返回操作是否合法
	bool PutOneCard(int value_nCard, int &clear_nCard);

	//出一组牌，返回操作是否合法
	bool PutCards();

	//通过有花色手牌获取无花色手牌（包含两种结构）
	void get_valueHandCardList();

	//初始化
	void Init(BYTE * CurCardData, BYTE CurCardNum);

	//输出所有成员变量，用于测试
	void PrintAll();

	//获得卡牌的逻辑数值
	BYTE GetCardLogicValue(BYTE cbCardData);

};

//出牌逻辑类
class AILogic
{
public:
	//构造函数
	AILogic()
	{

	}

	//析构函数
	~AILogic()
	{

	}

public:

	//检查剩余的牌是否能一手出完   是：返回手牌类型数据   否：返回错误类型
	CardGroupData ins_SurCardsType(int arr[]);

	//vector重载
	CardGroupData ins_SurCardsType(vector<int> list);

	/*
	封装好的获取各类牌型组合结构函数
		CardGroupType cgType：牌型
		int MaxCard：决定大小的牌值
		int Count：牌数
		返回值：CardGroupData
	*/
	CardGroupData get_GroupData(CardGroupType cgType, int MaxCard, int Count);

	/*
	通过回溯dp的方式获取手牌价值
		与get_PutCardList作为交替递归调用
		返回：价值结构体HandCardValue
		权值的计算规则参考头文件评分逻辑思维
	*/
	HandCardValue get_HandCardValue(HandCardData &clsHandCardData);

	//将一手牌全部出掉
	void Put_All_SurCards(GameSituation &clsGameSituation, HandCardData &clsHandCardData, CardGroupData SurCardGroupData);
	void Put_All_SurCards(HandCardData &clsHandCardData, CardGroupData SurCardGroupData);


	// 根据场上形势决定当前预打出的手牌——被动出牌
	void get_PutCardList_2_limit(GameSituation &clsGameSituation, HandCardData &clsHandCardData);

	// 主动出牌
	void get_PutCardList_2_unlimit(HandCardData &clsHandCardData);

	//获取叫分函数
	int LandScore( HandCardData &clsHandCardData);

};