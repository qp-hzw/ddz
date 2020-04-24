#include "StdAfx.h"
# include "IntelligentAI.h"


//    19.10.24     by  lih

//============================================手牌数据类==============================

//初始化  手牌序列和手牌个数
void  HandCardData::Init(BYTE * CurCardData, BYTE CurCardNum)
{
	if (CurCardData == NULL)
	{
		cout << "CurCardData == NULL" << endl;
		return;
	}

	memset(&value_aHandCardList, 0, sizeof(value_aHandCardList));

	nHandCardCount = CurCardNum;

	for (int i = 0; i < CurCardNum; i++)
	{
		value_aHandCardList[GetCardLogicValue(CurCardData[i])]++;
	}
}

//判断要打出去的牌是否合法
bool  HandCardData::PutCards()
{
	for (list<int>::iterator iter = value_nPutCardList.begin(); iter != value_nPutCardList.end(); iter++)
	{
		int color_nCard = -1;
		if (PutOneCard(*iter, color_nCard))
		{
			color_nPutCardList.push_back(color_nCard);
		}
		else
		{
			return false;
		}
	}

	nHandCardCount -= value_nPutCardList.size();
	return true;
}

//判断打出去的一张牌是否合法
bool  HandCardData::PutOneCard(int value_nCard, int &color_nCard)
{
	bool ret = false;



	//value状态数组处理

	value_aHandCardList[value_nCard]--;

	if (value_aHandCardList[value_nCard] < 0)
	{
		return false;
	}


	//value列表数组处理
	for (list<int>::iterator iter = value_nHandCardList.begin(); iter != value_nHandCardList.end(); iter++)
	{
		if (*iter == value_nCard)
		{
			value_nHandCardList.erase(iter);
			ret = true;
			break;
		}
	}


	// color列表数组处理

	int k = (value_nCard - 3) * 4;      //数值转换

	for (list<int>::iterator iter = color_nHandCardList.begin(); iter != color_nHandCardList.end(); iter++)
	{

		for (int i = k; i < k + 4; i++)
		{
			if (*iter == i)
			{
				color_nCard = i;
				color_nHandCardList.erase(iter);
				return ret;

			}
		}
	}
	return false;
}

//清空出牌序列
void  HandCardData::ClearPutCardList()
{
	memset(&uctPutCardType, 0, sizeof(uctPutCardType));
	uctPutCardType.cgType = cgERROR;
	value_nPutCardList.clear();
	color_nPutCardList.clear();
}




//============================================出牌逻辑类==============================

/*
检查剩余的牌是否只是一手牌
是：  返回手牌类型数据
不是：返回错误类型（cgERROR）
*/
CardGroupData AILogic::ins_SurCardsType(int arr[])
{

	int nCount = 0;
	for (int i = 3; i < 18; i++)
	{
		nCount += arr[i];
	}

	CardGroupData retCardGroupData;
	retCardGroupData.nCount = nCount;


	//单牌类型
	if (nCount == 1)
	{
		//用于验证的变量
		int prov = 0;
		int SumValue = 0;
		for (int i = 3; i < 18; i++)
		{
			if (arr[i] == 1)
			{
				SumValue = i - 10;
				prov++;
				retCardGroupData.nMaxCard = i;
			}
		}
		if (prov == 1)
		{
			retCardGroupData.cgType = cgSINGLE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//对牌类型
	if (nCount == 2)
	{
		//用于验证的变量
		int prov = 0;
		int SumValue = 0;
		int i = 0;
		for (i = 3; i < 16; i++)
		{
			if (arr[i] == 2)
			{
				SumValue = i - 10;
				prov++;
				retCardGroupData.nMaxCard = i;
			}
		}
		if (prov == 1)
		{
			retCardGroupData.cgType = cgDOUBLE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//三条类型
	if (nCount == 3)
	{
		//用于验证的变量
		int prov = 0;
		int SumValue = 0;
		int i = 0;
		for (i = 3; i < 16; i++)
		{
			if (arr[i] == 3)
			{
				SumValue = i - 10;
				prov++;
				retCardGroupData.nMaxCard = i;
			}
		}
		if (prov == 1)
		{
			retCardGroupData.cgType = cgTHREE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//三带一单
	if (nCount == 4)
	{
		//用于验证的变量
		int prov1 = 0;
		int prov2 = 0;
		int SumValue = 0;
		for (int i = 3; i < 18; i++)
		{
			if (arr[i] == 3)
			{
				SumValue = i - 10;
				prov1++;
				retCardGroupData.nMaxCard = i;

			}
			if (arr[i] == 1)
			{
				prov2++;
			}

		}
		if (prov1 == 1 && prov2 == 1)
		{
			retCardGroupData.cgType = cgTHREE_TAKE_ONE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//三带一对
	if (nCount == 5)
	{
		//用于验证的变量
		int prov1 = 0;
		int prov2 = 0;
		int SumValue = 0;
		for (int i = 3; i < 16; i++)
		{
			if (arr[i] == 3)
			{
				SumValue = i - 10;
				prov1++;
				retCardGroupData.nMaxCard = i;

			}
			if (arr[i] == 2)
			{
				prov2++;

			}
		}
		if (prov1 == 1 && prov2 == 1)
		{
			retCardGroupData.cgType = cgTHREE_TAKE_TWO;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//四带两单
	if (nCount == 6)
	{
		//用于验证的变量
		int prov1 = 0;
		int prov2 = 0;
		int SumValue = 0;
		for (int i = 3; i < 18; i++)
		{
			if (arr[i] == 4)
			{
				SumValue = (i - 3) / 2;
				prov1++;
				retCardGroupData.nMaxCard = i;

			}
			if (arr[i] == 1 || arr[i] == 2)
			{
				prov2 += arr[i];
			}
		}

		if (prov1 == 1 && prov2 == 2)
		{
			retCardGroupData.cgType = cgFOUR_TAKE_ONE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//四带两对
	if (nCount == 8)
	{
		//用于验证的变量
		int prov1 = 0;
		int prov2 = 0;
		int SumValue = 0;
		for (int i = 3; i < 16; i++)
		{
			if (arr[i] == 4)
			{
				SumValue = (i - 3) / 2;

				prov1++;
				retCardGroupData.nMaxCard = i;
			}
			if (arr[i] == 2 || arr[i] == 4)
			{
				prov2 += arr[i] / 2;

			}
		}
		//注意这里prov2==4因为四牌也是两个对
		if (prov1 == 1 && prov2 == 4)
		{
			retCardGroupData.cgType = cgFOUR_TAKE_TWO;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//炸弹类型
	if (nCount == 4)
	{
		//用于验证的变量
		int prov = 0;
		int SumValue = 0;
		for (int i = 3; i < 16; i++)
		{
			if (arr[i] == 4)
			{
				SumValue += i - 3 + 7;
				prov++;
				retCardGroupData.nMaxCard = i;
			}
		}
		if (prov == 1)
		{
			retCardGroupData.cgType = cgBOMB_CARD;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//王炸类型
	if (nCount == 2)
	{
		int SumValue = 0;
		if (arr[17] > 0 && arr[16] > 0)
		{
			SumValue = 20;
			retCardGroupData.nMaxCard = 17;
			retCardGroupData.cgType = cgKING_CARD;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//单连类型
	if (nCount >= 5)
	{
		//用于验证的变量
		int prov = 0;
		int SumValue = 0;
		int i;
		for (i = 3; i < 15; i++)
		{
			if (arr[i] == 1)
			{
				prov++;
			}
			else
			{
				if (prov != 0)
				{
					break;
				}

			}
		}
		SumValue = i - 10;

		if (prov == nCount)
		{
			retCardGroupData.nMaxCard = i - 1;
			retCardGroupData.cgType = cgSINGLE_LINE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//对连类型
	if (nCount >= 6)
	{
		//用于验证的变量
		int prov = 0;
		int SumValue = 0;
		int i;
		for (i = 3; i < 15; i++)
		{
			if (arr[i] == 2)
			{
				prov++;
			}
			else
			{
				if (prov != 0)
				{
					break;
				}

			}
		}
		SumValue = i - 10;

		if (prov * 2 == nCount)
		{
			retCardGroupData.nMaxCard = i - 1;
			retCardGroupData.cgType = cgDOUBLE_LINE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//三连类型
	if (nCount >= 6)
	{
		//用于验证的变量
		int prov = 0;

		int SumValue = 0;
		int i;
		for (i = 3; i < 15; i++)
		{
			if (arr[i] == 3)
			{
				prov++;
			}
			else
			{
				if (prov != 0)
				{
					break;
				}

			}
		}
		SumValue = (i - 3) / 2;

		if (prov * 3 == nCount)
		{
			retCardGroupData.nMaxCard = i - 1;
			retCardGroupData.cgType = cgTHREE_LINE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//三带一连类型
	if (nCount >= 8)
	{
		//用于验证的变量
		int prov1 = 0;
		int SumValue = 0;
		int i, j;
		for (i = 3; i < 15; i++)
		{
			if (arr[i] >= 3)
			{
				prov1++;
			}
			else
			{
				if (prov1 != 0)
				{
					break;
				}

			}
		}
		SumValue = (i - 3) / 2;
		if (prov1 * 4 == nCount)
		{
			retCardGroupData.nMaxCard = i - 1;
			retCardGroupData.cgType = cgTHREE_TAKE_ONE_LINE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}

	}
	//三带二连类型
	if (nCount >= 10)
	{
		//用于验证的变量
		int prov1 = 0;
		int prov2 = 0;
		int SumValue = 0;
		int i, j;
		for (i = 3; i < 15; i++)
		{
			if (arr[i] == 3)
			{
				prov1++;
			}
			else
			{
				if (prov1 != 0)
				{
					break;
				}
			}
		}
		for (j = 3; j < 16; j++)
		{
			if (arr[j] == 2 || arr[j] == 4)
			{
				prov2 += arr[j] / 2;
			}
		}
		SumValue = (i - 3) / 2;
		if (prov1 == prov2 && prov1 * 5 == nCount)
		{
			retCardGroupData.nMaxCard = i - 1;
			retCardGroupData.cgType = cgTHREE_TAKE_TWO_LINE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}

	retCardGroupData.cgType = cgERROR;
	return retCardGroupData;
}

/*
检查剩余的牌是否只是一手牌（vector重载）
是：  返回手牌类型数据
不是：返回错误类型（cgERROR）
*/
CardGroupData AILogic::ins_SurCardsType(list<int>mlist)
{
	int arr[18];
	memset(arr, 0, sizeof(arr));
	for (list<int>::iterator iter = mlist.begin(); iter != mlist.end(); iter++)
	{
		arr[*iter]++;
	}
	return ins_SurCardsType(arr);
}


/*
封装好的获取各类牌型组合结构函数
CardGroupType cgType：牌型
int MaxCard：决定大小的牌值
int Count：牌数
返回值：CardGroupData
*/
CardGroupData AILogic::get_GroupData(CardGroupType cgType, int MaxCard, int Count)
{
	CardGroupData uctCardGroupData;

	uctCardGroupData.cgType = cgType;
	uctCardGroupData.nCount = Count;
	uctCardGroupData.nMaxCard = MaxCard;

	//不出牌型
	if (cgType == cgZERO)
		uctCardGroupData.nValue = 0;
	//单牌类型
	else if (cgType == cgSINGLE)
		uctCardGroupData.nValue = MaxCard - 10;
	//对牌类型
	else if (cgType == cgDOUBLE)
		uctCardGroupData.nValue = MaxCard - 10;
	//三条类型
	else if (cgType == cgTHREE)
		uctCardGroupData.nValue = MaxCard - 10;
	//单连类型
	else if (cgType == cgSINGLE_LINE)
		uctCardGroupData.nValue = MaxCard - 10 + 1;
	//对连类型
	else if (cgType == cgDOUBLE_LINE)
		uctCardGroupData.nValue = MaxCard - 10 + 1;
	//三连类型
	else if (cgType == cgTHREE_LINE)
		uctCardGroupData.nValue = (MaxCard - 3 + 1) / 2;
	//三带一单
	else if (cgType == cgTHREE_TAKE_ONE)
		uctCardGroupData.nValue = MaxCard - 10;
	//三带一对
	else if (cgType == cgTHREE_TAKE_TWO)
		uctCardGroupData.nValue = MaxCard - 10;
	//三带一单连
	else if (cgType == cgTHREE_TAKE_ONE_LINE)
		uctCardGroupData.nValue = (MaxCard - 3 + 1) / 2;
	//三带一对连
	else if (cgType == cgTHREE_TAKE_TWO_LINE)
		uctCardGroupData.nValue = (MaxCard - 3 + 1) / 2;
	//四带两单
	else if (cgType == cgFOUR_TAKE_ONE)
		uctCardGroupData.nValue = (MaxCard - 3) / 2;
	//四带两对
	else if (cgType == cgFOUR_TAKE_TWO)
		uctCardGroupData.nValue = (MaxCard - 3) / 2;
	//炸弹类型 
	else if (cgType == cgBOMB_CARD)
		uctCardGroupData.nValue = MaxCard - 3 + 7;
	//王炸类型 
	else if (cgType == cgKING_CARD)
		uctCardGroupData.nValue = 20;
	//错误牌型
	else
		uctCardGroupData.nValue = 0;


	return uctCardGroupData;
}



/*
通过回溯dp的方式获取手牌价值
与get_PutCardList作为交替递归调用
返回：价值结构体HandCardValue
权值的计算规则参考头文件评分逻辑思维
*/

HandCardValue AILogic::get_HandCardValue(HandCardData &clsHandCardData)
{

	//首先清空出牌队列，因为剪枝时是不调用get_PutCardList的
	clsHandCardData.ClearPutCardList();

	HandCardValue uctHandCardValue;
	//出完牌了，其实这种情况只限于手中剩下四带二且被动出牌的情况，因为四带二剪枝做了特殊处理。
	if (clsHandCardData.nHandCardCount == 0)
	{
		uctHandCardValue.SumValue = 0;
		uctHandCardValue.NeedRound = 0;
		return uctHandCardValue;
	}
	//————以下为剪枝：判断是否可以一手出完牌
	CardGroupData uctCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
	//————不到万不得已我们都不会出四带二，都尽量保炸弹
	if (uctCardGroupData.cgType != cgERROR && uctCardGroupData.cgType != cgFOUR_TAKE_ONE && uctCardGroupData.cgType != cgFOUR_TAKE_TWO)
	{
		uctHandCardValue.SumValue = uctCardGroupData.nValue;
		uctHandCardValue.NeedRound = 1;
		return uctHandCardValue;
	}

	//非剪枝操作，即非一手能出完的牌

	/*只是获取出牌的序列，即clsHandCardData.value_nPutCardList及clsHandCardData.uctPutCardType
	其他成员均无改变，也不会调用出牌函数，get_PutCardList返回最优方案*/
	get_PutCardList_2_unlimit(clsHandCardData);

	//要保存当前的clsHandCardData.value_nPutCardList及clsHandCardData.uctPutCardType用于回溯
	CardGroupData NowPutCardType = clsHandCardData.uctPutCardType;
	list<int> NowPutCardList = clsHandCardData.value_nPutCardList;

	if (clsHandCardData.uctPutCardType.cgType == cgERROR)
	{
		cout << "PutCardType ERROR!" << endl;
	}



	//---回溯↓
	for (list<int>::iterator iter = NowPutCardList.begin();
		iter != NowPutCardList.end(); iter++)
	{
		clsHandCardData.value_aHandCardList[*iter]--;
	}
	clsHandCardData.nHandCardCount -= NowPutCardType.nCount;
	//---回溯↑

	HandCardValue tmp_SurValue = get_HandCardValue(clsHandCardData);//递归剩余牌价值

	//---回溯↓	
	for (list<int>::iterator iter = NowPutCardList.begin();
		iter != NowPutCardList.end(); iter++)
	{
		clsHandCardData.value_aHandCardList[*iter]++;
	}
	clsHandCardData.nHandCardCount += NowPutCardType.nCount;
	//---回溯↑

	uctHandCardValue.SumValue = NowPutCardType.nValue + tmp_SurValue.SumValue;
	uctHandCardValue.NeedRound = tmp_SurValue.NeedRound + 1;





	return uctHandCardValue;

}


void AILogic::Put_All_SurCards(GameSituation &clsGameSituation, HandCardData &clsHandCardData, CardGroupData SurCardGroupData)
{
	/*全部出完*/
	for (int i = 0; i < 18; i++)
		for (int j = 0; j < clsHandCardData.value_aHandCardList[i]; j++)
			clsHandCardData.value_nPutCardList.push_back(i);
	/**********/
	clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = SurCardGroupData;
	return;
}

void AILogic::Put_All_SurCards(HandCardData &clsHandCardData, CardGroupData SurCardGroupData)
{
	/*全部出完*/
	for (int i = 0; i < 18; i++)
		for (int j = 0; j < clsHandCardData.value_aHandCardList[i]; j++)
			clsHandCardData.value_nPutCardList.push_back(i);
	/**********/
	clsHandCardData.uctPutCardType = SurCardGroupData;
	return;
}

/*
2.0版本策略  根据场上形势决定当前预打出的手牌——被动出牌
*/
void AILogic::get_PutCardList_2_limit(GameSituation &clsGameSituation, HandCardData &clsHandCardData)
{
	clsHandCardData.ClearPutCardList();


	/*王炸——当前策略只处理王炸作为倒数第二手的优先出牌逻辑，后续版本会在此基础上优化*/
	if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
	{

		clsHandCardData.value_aHandCardList[17] --;
		clsHandCardData.value_aHandCardList[16] --;
		clsHandCardData.nHandCardCount -= 2;
		HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
		clsHandCardData.value_aHandCardList[16] ++;
		clsHandCardData.value_aHandCardList[17] ++;
		clsHandCardData.nHandCardCount += 2;
		if (tmpHandCardValue.NeedRound == 1)
		{
			clsHandCardData.value_nPutCardList.push_back(17);
			clsHandCardData.value_nPutCardList.push_back(16);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
			return;
		}
	}

	//不出牌型，当队友出牌的价值 > 1时 选择不出牌
	if (clsGameSituation.NowOutUser != clsGameSituation.nDiZhuID && clsGameSituation.nCardDroit != clsGameSituation.nDiZhuID)   //我不是地主，最后出牌玩家不是地主
	{
		//判断队友出的手牌的价值 > 1 选择不出牌
		if (clsGameSituation.uctNowCardGroup.nValue > 0)
		{
			clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
			return;
		}
	}

	//错误牌型  不出
	if (clsGameSituation.uctNowCardGroup.cgType == cgERROR)
	{
		clsHandCardData.uctPutCardType = get_GroupData(cgERROR, 0, 0);
		return;
	}
	//不出牌型，在被动出牌策略里也是错误数据 不出
	else if (clsGameSituation.uctNowCardGroup.cgType == cgZERO)
	{
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//单牌类型
	else if (clsGameSituation.uctNowCardGroup.cgType == cgSINGLE)
	{

		//剪枝：如果能出去最后一手牌直接出
		CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
		if (SurCardGroupData.cgType != cgERROR)
		{
			if (SurCardGroupData.cgType == cgSINGLE && SurCardGroupData.nMaxCard > clsGameSituation.uctNowCardGroup.nMaxCard)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
			else if (SurCardGroupData.cgType == cgBOMB_CARD || SurCardGroupData.cgType == cgKING_CARD)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
		}

		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;

		//暂存最佳的牌号
		int BestMaxCard = 0;
		//是否出牌的标志
		bool PutCards = false;

		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard + 1; i < 18; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 0)
			{
				//尝试打出一张牌，估算剩余手牌价值
				clsHandCardData.value_aHandCardList[i]--;
				clsHandCardData.nHandCardCount--;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i]++;
				clsHandCardData.nHandCardCount++;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgSINGLE, BestMaxCard, 1);
			return;
		}

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4 && 
				(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
			{

				//尝试打出炸弹，估算剩余手牌价值,因为炸弹可以参与顺子，不能因为影响顺子而任意出炸
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//如果剩余手牌价值为正，证明出去的几率很大， 那么可以用炸获得先手
					|| tmpHandCardValue.SumValue > 0)
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, BestMaxCard, 4);
			return;
		}

		//王炸
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0 &&
			(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
		{
			//如果剩余手牌价值为正，证明出去的几率很大，那么可以用炸获得先手，王炸20分
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}

		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;

	}
	//对牌类型
	else if (clsGameSituation.uctNowCardGroup.cgType == cgDOUBLE)
	{

		//剪枝：如果能出去最后一手牌直接出
		CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
		if (SurCardGroupData.cgType != cgERROR)
		{
			if (SurCardGroupData.cgType == cgDOUBLE && SurCardGroupData.nMaxCard > clsGameSituation.uctNowCardGroup.nMaxCard)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
			else if (SurCardGroupData.cgType == cgBOMB_CARD || SurCardGroupData.cgType == cgKING_CARD)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
		}
		//-------------------------------------------对牌-------------------------------------------

		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);

		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;

		//暂存最佳的牌号
		int BestMaxCard = 0;
		//是否出牌的标志
		bool PutCards = false;

		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard + 1; i < 18; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 1)
			{
				//尝试打出一对牌，估算剩余手牌价值
				clsHandCardData.value_aHandCardList[i] -= 2;
				clsHandCardData.nHandCardCount -= 2;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 2;
				clsHandCardData.nHandCardCount += 2;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgDOUBLE, BestMaxCard, 2);
			return;
		}


		//-------------------------------------------炸弹-------------------------------------------

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4 &&
				(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
			{

				//尝试打出炸弹，估算剩余手牌价值,因为炸弹可以参与顺子，不能因为影响顺子而任意出炸
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//如果剩余手牌价值为正，证明出去的几率很大， 那么可以用炸获得先手
					|| tmpHandCardValue.SumValue > 0)
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, BestMaxCard, 4);
			return;
		}

		//王炸
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0 &&
			(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
		{
			//如果剩余手牌价值为正，证明出去的几率很大，那么可以用炸获得先手，王炸20分
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}

		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//三牌类型
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE)
	{
		//剪枝：如果能出去最后一手牌直接出
		CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
		if (SurCardGroupData.cgType != cgERROR)
		{
			if (SurCardGroupData.cgType == cgTHREE && SurCardGroupData.nMaxCard > clsGameSituation.uctNowCardGroup.nMaxCard)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
			else if (SurCardGroupData.cgType == cgBOMB_CARD || SurCardGroupData.cgType == cgKING_CARD)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
		}
		//-------------------------------------------三牌-------------------------------------------

		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;

		//暂存最佳的牌号
		int BestMaxCard = 0;
		//是否出牌的标志
		bool PutCards = false;

		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard + 1; i < 18; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				//尝试打出一对牌，估算剩余手牌价值
				clsHandCardData.value_aHandCardList[i] -= 3;
				clsHandCardData.nHandCardCount -= 3;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 3;
				clsHandCardData.nHandCardCount += 3;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgTHREE, BestMaxCard, 3);
			return;
		}


		//-------------------------------------------炸弹-------------------------------------------

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4 &&
				(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
			{

				//尝试打出炸弹，估算剩余手牌价值,因为炸弹可以参与顺子，不能因为影响顺子而任意出炸
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//如果剩余手牌价值为正，证明出去的几率很大， 那么可以用炸获得先手
					|| tmpHandCardValue.SumValue > 0)
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, BestMaxCard, 4);
			return;
		}

		//王炸
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0 &&
			(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
		{
			//如果剩余手牌价值为正，证明出去的几率很大，那么可以用炸获得先手，王炸20分
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}

		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//单连类型
	else if (clsGameSituation.uctNowCardGroup.cgType == cgSINGLE_LINE)
	{
		CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
		if (SurCardGroupData.cgType == cgSINGLE_LINE && SurCardGroupData.nMaxCard > clsGameSituation.uctNowCardGroup.nMaxCard
			&&SurCardGroupData.nCount == clsGameSituation.uctNowCardGroup.nCount)
		{
			Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
			return;
		}

		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;

		//暂存最佳的牌号
		int BestMaxCard = 0;
		//是否出牌的标志
		bool PutCards = false;

		//验证顺子的标志
		int prov = 0;
		//顺子起点
		int start_i = 0;
		//顺子终点
		int end_i = 0;
		//顺子长度
		int length = clsGameSituation.uctNowCardGroup.nCount;

		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard - length + 2; i < 15; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 0)
			{
				prov++;
			}
			else
			{
				prov = 0;
			}
			if (prov >= length)
			{
				end_i = i;
				start_i = i - length + 1;

				for (int j = start_i; j <= end_i; j++)
				{
					clsHandCardData.value_aHandCardList[j] --;
				}
				clsHandCardData.nHandCardCount -= clsGameSituation.uctNowCardGroup.nCount;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				for (int j = start_i; j <= end_i; j++)
				{
					clsHandCardData.value_aHandCardList[j] ++;
				}
				clsHandCardData.nHandCardCount += clsGameSituation.uctNowCardGroup.nCount;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = end_i;
					PutCards = true;
				}

			}
		}

		if (PutCards)
		{
			for (int j = start_i; j <= end_i; j++)
			{
				clsHandCardData.value_nPutCardList.push_back(j);
			}
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgSINGLE_LINE, BestMaxCard, clsGameSituation.uctNowCardGroup.nCount);
			return;
		}

		//-------------------------------------------炸弹-------------------------------------------

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4 &&
				(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
			{

				//尝试打出炸弹，估算剩余手牌价值,因为炸弹可以参与顺子，不能因为影响顺子而任意出炸
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//如果剩余手牌价值为正，证明出去的几率很大， 那么可以用炸获得先手
					|| tmpHandCardValue.SumValue > 0)
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, BestMaxCard, 4);
			return;
		}

		//王炸
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0 &&
			(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
		{
			//如果剩余手牌价值为正，证明出去的几率很大，那么可以用炸获得先手，王炸20分
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}

		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//对连类型
	else if (clsGameSituation.uctNowCardGroup.cgType == cgDOUBLE_LINE)
	{

		//剪枝：如果能出去最后一手牌直接出
		CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
		if (SurCardGroupData.cgType != cgERROR)
		{
			if (SurCardGroupData.cgType == cgDOUBLE_LINE && SurCardGroupData.nMaxCard > clsGameSituation.uctNowCardGroup.nMaxCard
				&&SurCardGroupData.nCount == clsGameSituation.uctNowCardGroup.nCount)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
			else if (SurCardGroupData.cgType == cgBOMB_CARD || SurCardGroupData.cgType == cgKING_CARD)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
		}


		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;

		//暂存最佳的牌号
		int BestMaxCard = 0;
		//是否出牌的标志
		bool PutCards = false;
		//验证顺子的标志
		int prov = 0;
		//顺子起点
		int start_i = 0;
		//顺子终点
		int end_i = 0;
		//顺子长度
		int length = clsGameSituation.uctNowCardGroup.nCount / 2;
		//2与王不参与顺子，从当前已打出的顺子最小牌值+1开始遍历
		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard - length + 2; i < 15; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 1)
			{
				prov++;
			}
			else
			{
				prov = 0;
			}
			if (prov >= length)
			{
				end_i = i;
				start_i = i - length + 1;

				for (int j = start_i; j <= end_i; j++)
				{
					clsHandCardData.value_aHandCardList[j] -= 2;
				}
				clsHandCardData.nHandCardCount -= clsGameSituation.uctNowCardGroup.nCount;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				for (int j = start_i; j <= end_i; j++)
				{
					clsHandCardData.value_aHandCardList[j] += 2;
				}
				clsHandCardData.nHandCardCount += clsGameSituation.uctNowCardGroup.nCount;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = end_i;
					PutCards = true;
				}

			}
		}

		if (PutCards)
		{
			for (int j = start_i; j <= end_i; j++)
			{
				clsHandCardData.value_nPutCardList.push_back(j);
				clsHandCardData.value_nPutCardList.push_back(j);
			}
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgDOUBLE_LINE, BestMaxCard, clsGameSituation.uctNowCardGroup.nCount);
			return;
		}

		//-------------------------------------------炸弹-------------------------------------------

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4 &&
				(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
			{

				//尝试打出炸弹，估算剩余手牌价值,因为炸弹可以参与顺子，不能因为影响顺子而任意出炸
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//如果剩余手牌价值为正，证明出去的几率很大， 那么可以用炸获得先手
					|| tmpHandCardValue.SumValue > 0)
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, BestMaxCard, 4);
			return;
		}

		//王炸
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0 &&
			(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
		{
			//如果剩余手牌价值为正，证明出去的几率很大，那么可以用炸获得先手，王炸20分
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}

		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;

	}
	//三连类型
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE_LINE)
	{

		//剪枝：如果能出去最后一手牌直接出
		CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
		if (SurCardGroupData.cgType != cgERROR)
		{
			if (SurCardGroupData.cgType == cgTHREE_LINE && SurCardGroupData.nMaxCard > clsGameSituation.uctNowCardGroup.nMaxCard
				&&SurCardGroupData.nCount == clsGameSituation.uctNowCardGroup.nCount)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
			else if (SurCardGroupData.cgType == cgBOMB_CARD || SurCardGroupData.cgType == cgKING_CARD)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
		}


		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;

		//暂存最佳的牌号
		int BestMaxCard = 0;
		//是否出牌的标志
		bool PutCards = false;
		//验证顺子的标志
		int prov = 0;
		//顺子起点
		int start_i = 0;
		//顺子终点
		int end_i = 0;
		//顺子长度
		int length = clsGameSituation.uctNowCardGroup.nCount / 3;
		//2与王不参与顺子，从当前已打出的顺子最小牌值+1开始遍历
		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard - length + 2; i < 15; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				prov++;
			}
			else
			{
				prov = 0;
			}
			if (prov >= length)
			{
				end_i = i;
				start_i = i - length + 1;

				for (int j = start_i; j <= end_i; j++)
				{
					clsHandCardData.value_aHandCardList[j] -= 3;
				}
				clsHandCardData.nHandCardCount -= clsGameSituation.uctNowCardGroup.nCount;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				for (int j = start_i; j <= end_i; j++)
				{
					clsHandCardData.value_aHandCardList[j] += 3;
				}
				clsHandCardData.nHandCardCount += clsGameSituation.uctNowCardGroup.nCount;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = end_i;
					PutCards = true;
				}

			}
		}

		if (PutCards)
		{
			for (int j = start_i; j <= end_i; j++)
			{
				clsHandCardData.value_nPutCardList.push_back(j);
				clsHandCardData.value_nPutCardList.push_back(j);
				clsHandCardData.value_nPutCardList.push_back(j);
			}
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgTHREE_LINE, BestMaxCard, clsGameSituation.uctNowCardGroup.nCount);
			return;
		}

		//-------------------------------------------炸弹-------------------------------------------

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4 &&
				(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
			{

				//尝试打出炸弹，估算剩余手牌价值,因为炸弹可以参与顺子，不能因为影响顺子而任意出炸
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//如果剩余手牌价值为正，证明出去的几率很大， 那么可以用炸获得先手
					|| tmpHandCardValue.SumValue > 0)
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, BestMaxCard, 4);
			return;
		}

		//王炸
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0 &&
			(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
		{
			//如果剩余手牌价值为正，证明出去的几率很大，那么可以用炸获得先手，王炸20分
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}

		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//三带一单
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE_TAKE_ONE)
	{
		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);
		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;
		//暂存最佳的牌号
		int BestMaxCard = 0;
		//顺带出去的牌
		int tmp_1 = 0;
		//是否出牌的标志
		bool PutCards = false;
		//三带一
		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard + 1; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				for (int j = 3; j < 18; j++)
				{
					//选出一张以上的牌且不是选择三张的那个牌
					if (clsHandCardData.value_aHandCardList[j] > 0 && j != i)
					{
						clsHandCardData.value_aHandCardList[i] -= 3;
						clsHandCardData.value_aHandCardList[j] -= 1;
						clsHandCardData.nHandCardCount -= 4;
						HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
						clsHandCardData.value_aHandCardList[i] += 3;
						clsHandCardData.value_aHandCardList[j] += 1;
						clsHandCardData.nHandCardCount += 4;
						//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
						if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
						{
							BestHandCardValue = tmpHandCardValue;
							BestMaxCard = i;
							tmp_1 = j;
							PutCards = true;
						}
					}
				}
			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(tmp_1);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgTHREE_TAKE_ONE, BestMaxCard, 4);
			return;
		}

		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//三带一对
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE_TAKE_TWO)
	{

		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);
		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;
		//暂存最佳的牌号
		int BestMaxCard = 0;
		//顺带出去的牌
		int tmp_1 = 0;
		//是否出牌的标志
		bool PutCards = false;

		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard + 1; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				for (int j = 3; j < 16; j++)
				{
					//选出一张以上的牌且不是选择三张的那个牌
					if (clsHandCardData.value_aHandCardList[j] > 1 && j != i)
					{
						clsHandCardData.value_aHandCardList[i] -= 3;
						clsHandCardData.value_aHandCardList[j] -= 2;
						clsHandCardData.nHandCardCount -= 5;
						HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
						clsHandCardData.value_aHandCardList[i] += 3;
						clsHandCardData.value_aHandCardList[j] += 2;
						clsHandCardData.nHandCardCount += 5;
						//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
						if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
						{
							BestHandCardValue = tmpHandCardValue;
							BestMaxCard = i;
							tmp_1 = j;
							PutCards = true;
						}
					}
				}
			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(tmp_1);
			clsHandCardData.value_nPutCardList.push_back(tmp_1);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgTHREE_TAKE_TWO, BestMaxCard, 5);
			return;
		}

		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//三带一单连
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE_TAKE_ONE_LINE)
	{

		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;

		//暂存最佳的牌号
		int BestMaxCard = 0;
		//是否出牌的标志
		bool PutCards = false;
		//验证顺子的标志
		int prov = 0;
		//顺子起点
		int start_i = 0;
		//顺子终点
		int end_i = 0;
		//顺子长度
		int length = clsGameSituation.uctNowCardGroup.nCount / 4;

		int tmp_1 = 0;
		int tmp_2 = 0;
		int tmp_3 = 0;
		int tmp_4 = 0;
		//2与王不参与顺子，从当前已打出的顺子最小牌值+1开始遍历
		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard - length + 2; i < 15; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				prov++;
			}
			else
			{
				prov = 0;
			}
			if (prov >= length)
			{
				end_i = i;
				start_i = i - length + 1;

				for (int j = start_i; j <= end_i; j++)
				{
					clsHandCardData.value_aHandCardList[j] -= 3;
				}
				clsHandCardData.nHandCardCount -= clsGameSituation.uctNowCardGroup.nCount;

				/*本来想做全排列选取带出的牌然后枚举出最高价值的，但考虑到当飞机长度也就是在2-4之间
				所以干脆做三个分支处理算了*/
				//为两连飞机
				if (length == 2)
				{
					for (int j = 3; j < 18; j++)
					{
						if (clsHandCardData.value_aHandCardList[j] > 0)
						{
							clsHandCardData.value_aHandCardList[j] -= 1;
							for (int k = 3; k < 18; k++)
							{
								if (clsHandCardData.value_aHandCardList[k] > 0)
								{
									clsHandCardData.value_aHandCardList[k] -= 1;
									HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
									clsHandCardData.value_aHandCardList[k] += 1;

									//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
									if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
									{
										BestHandCardValue = tmpHandCardValue;
										BestMaxCard = end_i;
										tmp_1 = j;
										tmp_2 = k;
										PutCards = true;
									}
								}
							}
							clsHandCardData.value_aHandCardList[j] += 1;
						}

					}
				}
				//为三连飞机
				if (length == 3)
				{
					for (int j = 3; j < 18; j++)
					{
						if (clsHandCardData.value_aHandCardList[j] > 0)
						{
							clsHandCardData.value_aHandCardList[j] -= 1;
							for (int k = 3; k < 18; k++)
							{
								if (clsHandCardData.value_aHandCardList[k] > 0)
								{
									clsHandCardData.value_aHandCardList[k] -= 1;
									for (int l = 3; l < 18; l++)
									{
										if (clsHandCardData.value_aHandCardList[l] > 0)
										{
											clsHandCardData.value_aHandCardList[l] -= 1;
											HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
											//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
											if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
											{
												BestHandCardValue = tmpHandCardValue;
												BestMaxCard = end_i;
												tmp_1 = j;
												tmp_2 = k;
												tmp_3 = l;
												PutCards = true;
											}
											clsHandCardData.value_aHandCardList[l] += 1;
										}
									}
									clsHandCardData.value_aHandCardList[k] += 1;
								}
							}
							clsHandCardData.value_aHandCardList[j] += 1;
						}


					}
				}
				//为四连飞机
				if (length == 4)
				{
					for (int j = 3; j < 18; j++)
					{
						if (clsHandCardData.value_aHandCardList[j] > 0)
						{
							clsHandCardData.value_aHandCardList[j] -= 1;
							for (int k = 3; k < 18; k++)
							{
								if (clsHandCardData.value_aHandCardList[k] > 0)
								{
									clsHandCardData.value_aHandCardList[k] -= 1;
									for (int l = 3; l < 18; l++)
									{
										if (clsHandCardData.value_aHandCardList[l] > 0)
										{
											clsHandCardData.value_aHandCardList[l] -= 1;
											for (int m = 3; m < 18; m++)
											{
												if (clsHandCardData.value_aHandCardList[m] > 0)
												{
													clsHandCardData.value_aHandCardList[m] -= 1;
													HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
													//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
													if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
													{
														BestHandCardValue = tmpHandCardValue;
														BestMaxCard = end_i;
														tmp_1 = j;
														tmp_2 = k;
														tmp_3 = l;
														tmp_4 = m;
														PutCards = true;
													}
													clsHandCardData.value_aHandCardList[m] += 1;
												}
											}
											clsHandCardData.value_aHandCardList[l] += 1;
										}
									}
									clsHandCardData.value_aHandCardList[k] += 1;
								}
							}
							clsHandCardData.value_aHandCardList[j] += 1;
						}


					}
				}

				for (int j = start_i; j <= end_i; j++)
				{
					clsHandCardData.value_aHandCardList[j] += 3;
				}
				clsHandCardData.nHandCardCount += clsGameSituation.uctNowCardGroup.nCount;
			}
		}

		if (PutCards)
		{
			for (int j = start_i; j <= end_i; j++)
			{
				clsHandCardData.value_nPutCardList.push_back(j);
				clsHandCardData.value_nPutCardList.push_back(j);
				clsHandCardData.value_nPutCardList.push_back(j);
			}

			if (length == 2)
			{
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
			}
			if (length == 3)
			{
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
				clsHandCardData.value_nPutCardList.push_back(tmp_3);

			}
			if (length == 4)
			{
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
				clsHandCardData.value_nPutCardList.push_back(tmp_3);
				clsHandCardData.value_nPutCardList.push_back(tmp_4);
			}

			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgTHREE_TAKE_ONE_LINE, BestMaxCard, clsGameSituation.uctNowCardGroup.nCount);
			return;
		}

		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//三带一对连
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE_TAKE_TWO_LINE)
	{
		int i;

		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;

		//暂存最佳的牌号
		int BestMaxCard = 0;
		//是否出牌的标志
		bool PutCards = false;
		//验证顺子的标志
		int prov = 0;
		//顺子起点
		int start_i = 0;
		//顺子终点
		int end_i = 0;
		//顺子长度
		int length = clsGameSituation.uctNowCardGroup.nCount / 4;

		int tmp_1 = 0;
		int tmp_2 = 0;
		int tmp_3 = 0;
		int tmp_4 = 0;
		//2与王不参与顺子，从当前已打出的顺子最小牌值+1开始遍历
		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard - length + 2; i < 15; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				prov++;
			}
			else
			{
				prov = 0;
			}
			if (prov >= length)
			{
				end_i = i;
				start_i = i - length + 1;

				for (int j = start_i; j <= end_i; j++)
				{
					clsHandCardData.value_aHandCardList[j] -= 3;
				}
				clsHandCardData.nHandCardCount -= clsGameSituation.uctNowCardGroup.nCount;

				/*本来想做全排列选取带出的牌然后枚举出最高价值的，但考虑到当飞机长度也就是在2-4之间
				所以干脆做三个分支处理算了*/
				//为两连飞机
				if (length == 2)
				{
					for (int j = 3; j < 18; j++)
					{
						if (clsHandCardData.value_aHandCardList[j] > 1)
						{
							clsHandCardData.value_aHandCardList[j] -= 2;
							for (int k = 3; k < 18; k++)
							{
								if (clsHandCardData.value_aHandCardList[k] > 1)
								{
									clsHandCardData.value_aHandCardList[k] -= 2;
									HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
									clsHandCardData.value_aHandCardList[k] += 2;

									//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
									if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
									{
										BestHandCardValue = tmpHandCardValue;
										BestMaxCard = end_i;
										tmp_1 = j;
										tmp_2 = k;
										PutCards = true;
									}
								}
							}
							clsHandCardData.value_aHandCardList[j] += 2;
						}

					}
				}
				//为三连飞机
				if (length == 3)
				{
					for (int j = 3; j < 18; j++)
					{
						if (clsHandCardData.value_aHandCardList[j] > 1)
						{
							clsHandCardData.value_aHandCardList[j] -= 2;
							for (int k = 3; k < 18; k++)
							{
								if (clsHandCardData.value_aHandCardList[k] > 1)
								{
									clsHandCardData.value_aHandCardList[k] -= 2;
									for (int l = 3; l < 18; l++)
									{
										if (clsHandCardData.value_aHandCardList[l] > 1)
										{
											clsHandCardData.value_aHandCardList[l] -= 2;
											HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
											//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
											if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
											{
												BestHandCardValue = tmpHandCardValue;
												BestMaxCard = end_i;
												tmp_1 = j;
												tmp_2 = k;
												tmp_3 = l;
												PutCards = true;
											}
											clsHandCardData.value_aHandCardList[l] += 2;
										}
									}
									clsHandCardData.value_aHandCardList[k] += 2;
								}
							}
							clsHandCardData.value_aHandCardList[j] += 2;
						}


					}
				}
				//为四连飞机
				if (length == 4)
				{
					for (int j = 3; j < 18; j++)
					{
						if (clsHandCardData.value_aHandCardList[j] > 1)
						{
							clsHandCardData.value_aHandCardList[j] -= 2;
							for (int k = 3; k < 18; k++)
							{
								if (clsHandCardData.value_aHandCardList[k] > 1)
								{
									clsHandCardData.value_aHandCardList[k] -= 2;
									for (int l = 3; l < 18; l++)
									{
										if (clsHandCardData.value_aHandCardList[l] > 1)
										{
											clsHandCardData.value_aHandCardList[l] -= 2;
											for (int m = 3; m < 18; m++)
											{
												if (clsHandCardData.value_aHandCardList[m] > 1)
												{
													clsHandCardData.value_aHandCardList[m] -= 2;
													HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
													//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
													if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
													{
														BestHandCardValue = tmpHandCardValue;
														BestMaxCard = end_i;
														tmp_1 = j;
														tmp_2 = k;
														tmp_3 = l;
														tmp_4 = m;
														PutCards = true;
													}
													clsHandCardData.value_aHandCardList[m] += 2;
												}
											}
											clsHandCardData.value_aHandCardList[l] += 2;
										}
									}
									clsHandCardData.value_aHandCardList[k] += 2;
								}
							}
							clsHandCardData.value_aHandCardList[j] += 2;
						}


					}
				}

				for (int j = start_i; j <= end_i; j++)
				{
					clsHandCardData.value_aHandCardList[j] += 3;
				}
				clsHandCardData.nHandCardCount += clsGameSituation.uctNowCardGroup.nCount;
			}
		}

		if (PutCards)
		{
			for (int j = start_i; j <= end_i; j++)
			{
				clsHandCardData.value_nPutCardList.push_back(j);
				clsHandCardData.value_nPutCardList.push_back(j);
				clsHandCardData.value_nPutCardList.push_back(j);
			}

			if (length == 2)
			{
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
			}
			if (length == 3)
			{
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
				clsHandCardData.value_nPutCardList.push_back(tmp_3);
				clsHandCardData.value_nPutCardList.push_back(tmp_3);

			}
			if (length == 4)
			{
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
				clsHandCardData.value_nPutCardList.push_back(tmp_3);
				clsHandCardData.value_nPutCardList.push_back(tmp_3);
				clsHandCardData.value_nPutCardList.push_back(tmp_4);
				clsHandCardData.value_nPutCardList.push_back(tmp_4);
			}

			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgTHREE_TAKE_TWO_LINE, BestMaxCard, clsGameSituation.uctNowCardGroup.nCount);
			return;
		}

		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//四带两单
	else if (clsGameSituation.uctNowCardGroup.cgType == cgFOUR_TAKE_ONE)
	{

		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);
		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;
		//暂存最佳的牌号
		int BestMaxCard = 0;
		//顺带出去的牌
		int tmp_1 = 0;
		int tmp_2 = 0;
		//是否出牌的标志
		bool PutCards = false;

		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard + 1; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4)
			{
				for (int j = 3; j < 18; j++)
				{
					//先选出一张以上的牌且不是选择四张的那个牌
					if (clsHandCardData.value_aHandCardList[j] > 0 && j != i)
					{
						//再选出一张以上的牌且不是选择四张的那个牌且不是第一次选的两张内个牌（策略里四带二不允许带一对,还不如炸）
						for (int k = j + 1; k < 18; k++)
						{
							if (clsHandCardData.value_aHandCardList[k] > 0 && k != i)
							{
								clsHandCardData.value_aHandCardList[i] -= 4;
								clsHandCardData.value_aHandCardList[j] -= 1;
								clsHandCardData.value_aHandCardList[k] -= 1;
								clsHandCardData.nHandCardCount -= 6;
								HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
								clsHandCardData.value_aHandCardList[i] += 4;
								clsHandCardData.value_aHandCardList[j] += 1;
								clsHandCardData.value_aHandCardList[k] += 1;
								clsHandCardData.nHandCardCount += 6;

								//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
								if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
								{
									BestHandCardValue = tmpHandCardValue;
									BestMaxCard = i;
									tmp_1 = j;
									tmp_2 = k;
									PutCards = true;
								}
							}
						}
					}
				}
			}
		}

		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(tmp_1);
			clsHandCardData.value_nPutCardList.push_back(tmp_2);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgFOUR_TAKE_ONE, BestMaxCard, 6);
			return;
		}

		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//四带两对
	else if (clsGameSituation.uctNowCardGroup.cgType == cgFOUR_TAKE_TWO)
	{

		//这里是在拍权值较小的情况，且没有选择出良好的四带二牌型，那么也要炸，因为信仰。
		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4 &&
				(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
			{
				clsHandCardData.value_nPutCardList.push_back(i);
				clsHandCardData.value_nPutCardList.push_back(i);
				clsHandCardData.value_nPutCardList.push_back(i);
				clsHandCardData.value_nPutCardList.push_back(i);

				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, i, 4);

				return;
			}
		}
		//王炸
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0 &&
			(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
		{

			clsHandCardData.value_nPutCardList.push_back(17);
			clsHandCardData.value_nPutCardList.push_back(16);

			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);

			return;
		}

		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//炸弹类型 
	else if (clsGameSituation.uctNowCardGroup.cgType == cgBOMB_CARD)
	{

		/*本策略对于这种牌型有炸必炸*/

		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);

		if (BestHandCardValue.SumValue > 14)
		{
			//炸弹——这里直接炸，不考虑拆分后果。因为信仰。
			for (int i = 3; i < 16; i++)
			{
				if (clsHandCardData.value_aHandCardList[i] == 4 && i > clsGameSituation.uctNowCardGroup.nMaxCard &&
					(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
				{
					clsHandCardData.value_nPutCardList.push_back(i);
					clsHandCardData.value_nPutCardList.push_back(i);
					clsHandCardData.value_nPutCardList.push_back(i);
					clsHandCardData.value_nPutCardList.push_back(i);

					clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, i, 4);

					return;
				}
			}
			//王炸
			if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0 &&
				(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
			{

				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);

				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);

				return;
			}
		}

		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//王炸类型 
	else if (clsGameSituation.uctNowCardGroup.cgType == cgKING_CARD)
	{
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//异常处理 不出
	else
	{
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
	}
	return;
}


//主动出牌
void AILogic::get_PutCardList_2_unlimit(HandCardData &clsHandCardData)
{

	clsHandCardData.ClearPutCardList();

	//剪枝：如果能出去最后一手牌直接出
	CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);

	//如果能一次性出去且不是四带二，因为主动出牌若手上剩四带二牌的话可以考虑先打一手然后炸，获得双倍积分
	if (SurCardGroupData.cgType != cgERROR && SurCardGroupData.cgType != cgFOUR_TAKE_ONE && SurCardGroupData.cgType != cgFOUR_TAKE_TWO)
	{
		Put_All_SurCards(clsHandCardData, SurCardGroupData);
		return;
	}

	/*王炸——当前策略只处理王炸作为倒数第二手的优先出牌逻辑，后续版本会在此基础上优化*/
	if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
	{
		clsHandCardData.value_aHandCardList[17] --;
		clsHandCardData.value_aHandCardList[16] --;
		clsHandCardData.nHandCardCount -= 2;
		HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
		clsHandCardData.value_aHandCardList[16] ++;
		clsHandCardData.value_aHandCardList[17] ++;
		clsHandCardData.nHandCardCount += 2;
		if (tmpHandCardValue.NeedRound == 1)
		{
			clsHandCardData.value_nPutCardList.push_back(17);
			clsHandCardData.value_nPutCardList.push_back(16);
			clsHandCardData.uctPutCardType = get_GroupData(cgKING_CARD, 17, 2);
			return;
		}
	}

	//暂存最佳的价值
	HandCardValue BestHandCardValue;
	BestHandCardValue.NeedRound = 20;
	BestHandCardValue.SumValue = MinCardsValue;
	//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
	BestHandCardValue.NeedRound += 1;

	//暂存最佳的组合
	CardGroupData BestCardGroup;

	//带出去的牌
	int tmp_1 = 0;
	int tmp_2 = 0;
	int tmp_3 = 0;
	int tmp_4 = 0;
	//优先处理三牌、飞机等牌
	for (int i = 3; i < 16; i++)
	{
		//2.0版本策略主动出牌不拆分炸弹，朕自己从来就不打四带二，因为浪
		if (clsHandCardData.value_aHandCardList[i] != 4 && i <= 10)
		{
			//出三带一
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				clsHandCardData.value_aHandCardList[i] -= 3;
				for (int j = 3; j < 18; j++)
				{
					if (clsHandCardData.value_aHandCardList[j] > 0)
					{
						clsHandCardData.value_aHandCardList[j] -= 1;
						clsHandCardData.nHandCardCount -= 4;
						HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
						clsHandCardData.value_aHandCardList[j] += 1;
						clsHandCardData.nHandCardCount += 4;
						//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
						if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
						{
							BestHandCardValue = tmpHandCardValue;
							BestCardGroup = get_GroupData(cgTHREE_TAKE_ONE, i, 4);
							tmp_1 = j;
						}
					}
				}
				clsHandCardData.value_aHandCardList[i] += 3;
			}
			//出三带二
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				for (int j = 3; j < 16; j++)
				{
					clsHandCardData.value_aHandCardList[i] -= 3;
					if (clsHandCardData.value_aHandCardList[j] > 1)
					{
						clsHandCardData.value_aHandCardList[j] -= 2;
						clsHandCardData.nHandCardCount -= 5;
						HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
						clsHandCardData.value_aHandCardList[j] += 2;
						clsHandCardData.nHandCardCount += 5;
						//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
						if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
						{
							BestHandCardValue = tmpHandCardValue;
							BestCardGroup = get_GroupData(cgTHREE_TAKE_TWO, i, 5);
							tmp_1 = j;
						}
					}
					clsHandCardData.value_aHandCardList[i] += 3;
				}
			}
			//出四带二单
			if (clsHandCardData.value_aHandCardList[i] > 3)
			{
				//2.0版本策略主动出牌不拆分炸弹，
			}
			//出四带二对
			if (clsHandCardData.value_aHandCardList[i] > 3)
			{
				//2.0版本策略主动出牌不拆分炸弹，
			}
			//出三带一单连
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				int prov = 0;
				for (int j = i; j < 15; j++)
				{
					if (clsHandCardData.value_aHandCardList[j] > 2)
					{
						prov++;
					}
					else
					{
						break;
					}
					//为两连飞机
					if (prov == 2)
					{

						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] -= 3;
						}
						clsHandCardData.nHandCardCount -= prov * 4;
						for (int tmp1 = 3; tmp1 < 18; tmp1++)
						{
							if (clsHandCardData.value_aHandCardList[tmp1] > 0)
							{
								clsHandCardData.value_aHandCardList[tmp1] -= 1;
								for (int tmp2 = tmp1; tmp2 < 18; tmp2++)
								{
									if (clsHandCardData.value_aHandCardList[tmp2] > 0)
									{
										clsHandCardData.value_aHandCardList[tmp2] -= 1;
										HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);

										if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
										{
											BestHandCardValue = tmpHandCardValue;
											BestCardGroup = get_GroupData(cgTHREE_TAKE_ONE_LINE, j, prov * 4);
											tmp_1 = tmp1;
											tmp_2 = tmp2;

										}
										clsHandCardData.value_aHandCardList[tmp2] += 1;
									}
								}
								clsHandCardData.value_aHandCardList[tmp1] += 1;
							}
						}
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] += 3;
						}
						clsHandCardData.nHandCardCount += prov * 4;
					}
					//为三连飞机
					if (prov == 3)
					{
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] -= 3;
						}
						clsHandCardData.nHandCardCount -= prov * 4;
						for (int tmp1 = 3; tmp1 < 18; tmp1++)
						{
							if (clsHandCardData.value_aHandCardList[tmp1] > 0)
							{
								clsHandCardData.value_aHandCardList[tmp1] -= 1;
								for (int tmp2 = tmp1; tmp2 < 18; tmp2++)
								{
									if (clsHandCardData.value_aHandCardList[tmp2] > 0)
									{
										clsHandCardData.value_aHandCardList[tmp2] -= 1;
										for (int tmp3 = tmp2; tmp3 < 18; tmp3++)
										{
											if (clsHandCardData.value_aHandCardList[tmp3] > 0)
											{
												clsHandCardData.value_aHandCardList[tmp3] -= 1;

												HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
												if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
												{

													BestHandCardValue = tmpHandCardValue;
													BestCardGroup = get_GroupData(cgTHREE_TAKE_ONE_LINE, j, prov * 4);
													tmp_1 = tmp1;
													tmp_2 = tmp2;
													tmp_3 = tmp3;

												}
												clsHandCardData.value_aHandCardList[tmp3] += 1;
											}

										}
										clsHandCardData.value_aHandCardList[tmp2] += 1;
									}

								}
								clsHandCardData.value_aHandCardList[tmp1] += 1;
							}
						}
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] += 3;
						}
						clsHandCardData.nHandCardCount += prov * 4;
					}
					//为四连飞机
					if (prov == 4)
					{
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] -= 3;
						}
						clsHandCardData.nHandCardCount -= prov * 4;
						for (int tmp1 = 3; tmp1 < 18; tmp1++)
						{
							if (clsHandCardData.value_aHandCardList[tmp1] > 0)
							{
								clsHandCardData.value_aHandCardList[tmp1] -= 1;
								for (int tmp2 = tmp1; tmp2 < 18; tmp2++)
								{
									if (clsHandCardData.value_aHandCardList[tmp2] > 0)
									{
										clsHandCardData.value_aHandCardList[tmp2] -= 1;
										for (int tmp3 = tmp2; tmp3 < 18; tmp3++)
										{
											if (clsHandCardData.value_aHandCardList[tmp3] > 0)
											{
												clsHandCardData.value_aHandCardList[tmp3] -= 1;
												for (int tmp4 = tmp3; tmp4 < 18; tmp4++)
												{
													if (clsHandCardData.value_aHandCardList[tmp4] > 0)
													{
														clsHandCardData.value_aHandCardList[tmp4] -= 1;
														HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
														if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
														{
															BestHandCardValue = tmpHandCardValue;
															BestCardGroup = get_GroupData(cgTHREE_TAKE_ONE_LINE, j, prov * 4);
															tmp_1 = tmp1;
															tmp_2 = tmp2;
															tmp_3 = tmp3;
															tmp_4 = tmp4;
														}
														clsHandCardData.value_aHandCardList[tmp4] += 1;
													}

												}
												clsHandCardData.value_aHandCardList[tmp3] += 1;
											}

										}
										clsHandCardData.value_aHandCardList[tmp2] += 1;
									}

								}
								clsHandCardData.value_aHandCardList[tmp1] += 1;
							}
						}
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] += 3;
						}
						clsHandCardData.nHandCardCount += prov * 4;
					}
					//若prov==5，则是地主可以直接出去，在剪枝部分已经处理
				}

			}
			//出三带一双连
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				int prov = 0;
				for (int j = i; j < 15; j++)
				{
					if (clsHandCardData.value_aHandCardList[j] > 2)
					{
						prov++;
					}
					else
					{
						break;
					}
					/*本来想做全排列选取带出的牌然后枚举出最高价值的，但考虑到当飞机长度也就是在2-4之间
					所以干脆做三个分支处理算了*/
					//为两连飞机
					if (prov == 2)
					{

						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] -= 3;
						}
						clsHandCardData.nHandCardCount -= prov * 5;
						for (int tmp1 = 3; tmp1 < 16; tmp1++)
						{
							if (clsHandCardData.value_aHandCardList[tmp1] > 1)
							{
								clsHandCardData.value_aHandCardList[tmp1] -= 2;
								for (int tmp2 = tmp1; tmp2 < 16; tmp2++)
								{
									if (clsHandCardData.value_aHandCardList[tmp2] > 1)
									{
										clsHandCardData.value_aHandCardList[tmp2] -= 2;
										HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
										if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
										{
											BestHandCardValue = tmpHandCardValue;
											BestCardGroup = get_GroupData(cgTHREE_TAKE_TWO_LINE, j, prov * 5);
											tmp_1 = tmp1;
											tmp_2 = tmp2;
										}
										clsHandCardData.value_aHandCardList[tmp2] += 2;
									}
								}
								clsHandCardData.value_aHandCardList[tmp1] += 2;
							}
						}
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] += 3;
						}
						clsHandCardData.nHandCardCount += prov * 5;
					}
					//为三连飞机
					if (prov == 3)
					{
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] -= 3;
						}
						clsHandCardData.nHandCardCount -= prov * 5;
						for (int tmp1 = 3; tmp1 < 16; tmp1++)
						{
							if (clsHandCardData.value_aHandCardList[tmp1] > 1)
							{
								clsHandCardData.value_aHandCardList[tmp1] -= 2;
								for (int tmp2 = tmp1; tmp2 < 16; tmp2++)
								{
									if (clsHandCardData.value_aHandCardList[tmp2] > 1)
									{
										clsHandCardData.value_aHandCardList[tmp2] -= 2;
										for (int tmp3 = tmp2; tmp3 < 16; tmp3++)
										{
											if (clsHandCardData.value_aHandCardList[tmp3] > 1)
											{
												clsHandCardData.value_aHandCardList[tmp3] -= 2;
												HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
												if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
												{
													BestHandCardValue = tmpHandCardValue;
													BestCardGroup = get_GroupData(cgTHREE_TAKE_TWO_LINE, j, prov * 5);
													tmp_1 = tmp1;
													tmp_2 = tmp2;
													tmp_3 = tmp3;
												}
												clsHandCardData.value_aHandCardList[tmp3] += 2;
											}

										}
										clsHandCardData.value_aHandCardList[tmp2] += 2;
									}

								}
								clsHandCardData.value_aHandCardList[tmp1] += 2;
							}
						}
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] += 3;
						}
						clsHandCardData.nHandCardCount += prov * 5;
					}
					//若prov==4，则是地主可以直接出去，在剪枝部分已经处理
				}
			}
		}
	}
	//这部分出牌处理放到循环外
	if (BestCardGroup.cgType == cgTHREE_TAKE_ONE)
	{

		clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
		clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
		clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
		clsHandCardData.value_nPutCardList.push_back(tmp_1);
		clsHandCardData.uctPutCardType = BestCardGroup;
		return;
	}
	else if (BestCardGroup.cgType == cgTHREE_TAKE_TWO)
	{
		clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
		clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
		clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
		clsHandCardData.value_nPutCardList.push_back(tmp_1);
		clsHandCardData.value_nPutCardList.push_back(tmp_1);
		clsHandCardData.uctPutCardType = BestCardGroup;
		return;
	}
	else if (BestCardGroup.cgType == cgTHREE_TAKE_ONE_LINE)
	{
		for (int j = BestCardGroup.nMaxCard - (BestCardGroup.nCount / 4) + 1; j <= BestCardGroup.nMaxCard; j++)
		{
			clsHandCardData.value_nPutCardList.push_back(j);
			clsHandCardData.value_nPutCardList.push_back(j);
			clsHandCardData.value_nPutCardList.push_back(j);
		}

		if (BestCardGroup.nCount / 4 == 2)
		{
			clsHandCardData.value_nPutCardList.push_back(tmp_1);
			clsHandCardData.value_nPutCardList.push_back(tmp_2);
		}
		if (BestCardGroup.nCount / 4 == 3)
		{
			clsHandCardData.value_nPutCardList.push_back(tmp_1);
			clsHandCardData.value_nPutCardList.push_back(tmp_2);
			clsHandCardData.value_nPutCardList.push_back(tmp_3);
		}
		if (BestCardGroup.nCount / 4 == 4)
		{
			clsHandCardData.value_nPutCardList.push_back(tmp_1);
			clsHandCardData.value_nPutCardList.push_back(tmp_2);
			clsHandCardData.value_nPutCardList.push_back(tmp_3);
			clsHandCardData.value_nPutCardList.push_back(tmp_4);
		}

		clsHandCardData.uctPutCardType = BestCardGroup;
		return;

	}
	else if (BestCardGroup.cgType == cgTHREE_TAKE_TWO_LINE)
	{
		for (int j = BestCardGroup.nMaxCard - (BestCardGroup.nCount / 5) + 1; j <= BestCardGroup.nMaxCard; j++)
		{
			clsHandCardData.value_nPutCardList.push_back(j);
			clsHandCardData.value_nPutCardList.push_back(j);
			clsHandCardData.value_nPutCardList.push_back(j);
		}
		if (BestCardGroup.nCount / 5 == 2)
		{
			clsHandCardData.value_nPutCardList.push_back(tmp_1);
			clsHandCardData.value_nPutCardList.push_back(tmp_1);
			clsHandCardData.value_nPutCardList.push_back(tmp_2);
			clsHandCardData.value_nPutCardList.push_back(tmp_2);
		}
		if (BestCardGroup.nCount / 5 == 3)
		{
			clsHandCardData.value_nPutCardList.push_back(tmp_1);
			clsHandCardData.value_nPutCardList.push_back(tmp_1);
			clsHandCardData.value_nPutCardList.push_back(tmp_2);
			clsHandCardData.value_nPutCardList.push_back(tmp_2);
			clsHandCardData.value_nPutCardList.push_back(tmp_3);
			clsHandCardData.value_nPutCardList.push_back(tmp_3);
		}
		clsHandCardData.uctPutCardType = BestCardGroup;
		return;
	}


	//次之处理当前价值最低的牌，现在不必再考虑这张牌可能被三牌带出等情况
	for (int i = 3; i < 16; i++)
	{
		if (clsHandCardData.value_aHandCardList[i] != 0 && clsHandCardData.value_aHandCardList[i] != 4)
		{
			//出单牌
			if (clsHandCardData.value_aHandCardList[i] > 0)
			{
				clsHandCardData.value_aHandCardList[i]--;
				clsHandCardData.nHandCardCount--;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i]++;
				clsHandCardData.nHandCardCount++;
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
				{
					BestHandCardValue = tmpHandCardValue;
					BestCardGroup = get_GroupData(cgSINGLE, i, 1);
				}
			}
			//出对牌
			if (clsHandCardData.value_aHandCardList[i] > 1)
			{
				//尝试打出一对牌，估算剩余手牌价值
				clsHandCardData.value_aHandCardList[i] -= 2;
				clsHandCardData.nHandCardCount -= 2;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 2;
				clsHandCardData.nHandCardCount += 2;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
				{
					BestHandCardValue = tmpHandCardValue;
					BestCardGroup = get_GroupData(cgDOUBLE, i, 2);
				}
			}
			//出三牌
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				clsHandCardData.value_aHandCardList[i] -= 3;
				clsHandCardData.nHandCardCount -= 3;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 3;
				clsHandCardData.nHandCardCount += 3;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
				{
					BestHandCardValue = tmpHandCardValue;
					BestCardGroup = get_GroupData(cgTHREE, i, 3);
				}
			}

			//出单顺
			if (clsHandCardData.value_aHandCardList[i] > 0)
			{
				int prov = 0;
				for (int j = i; j < 15; j++)
				{
					if (clsHandCardData.value_aHandCardList[j] > 0)
					{
						prov++;
					}
					else
					{
						break;
					}
					if (prov >= 5)
					{

						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] --;
						}
						clsHandCardData.nHandCardCount -= prov;
						HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] ++;
						}
						clsHandCardData.nHandCardCount += prov;

						//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
						if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
						{
							BestHandCardValue = tmpHandCardValue;
							BestCardGroup = get_GroupData(cgSINGLE_LINE, j, prov);
						}
					}
				}

			}
			//出双顺
			if (clsHandCardData.value_aHandCardList[i] > 1)
			{
				int prov = 0;
				for (int j = i; j < 15; j++)
				{
					if (clsHandCardData.value_aHandCardList[j] > 1)
					{
						prov++;
					}
					else
					{
						break;
					}
					if (prov >= 3)
					{

						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] -= 2;
						}
						clsHandCardData.nHandCardCount -= prov * 2;
						HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] += 2;
						}
						clsHandCardData.nHandCardCount += prov * 2;

						//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
						if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
						{
							BestHandCardValue = tmpHandCardValue;
							BestCardGroup = get_GroupData(cgDOUBLE_LINE, j, prov * 2);
						}
					}
				}
			}
			//出三顺
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				int prov = 0;
				for (int j = i; j < 15; j++)
				{
					if (clsHandCardData.value_aHandCardList[j] > 2)
					{
						prov++;
					}
					else
					{
						break;
					}
					if (prov >= 2)
					{

						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] -= 3;
						}
						clsHandCardData.nHandCardCount -= prov * 3;
						HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] += 3;
						}
						clsHandCardData.nHandCardCount += prov * 3;

						//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
						if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
						{
							BestHandCardValue = tmpHandCardValue;
							BestCardGroup = get_GroupData(cgTHREE_LINE, j, prov * 3);
						}
					}
				}
			}


			if (BestCardGroup.cgType == cgERROR)
			{
				cout << "BestCardGroup.cgType == cgERROR" << endl;
			}
			else if (BestCardGroup.cgType == cgSINGLE)
			{
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.uctPutCardType = BestCardGroup;
			}
			else if (BestCardGroup.cgType == cgDOUBLE)
			{
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.uctPutCardType = BestCardGroup;
			}
			else if (BestCardGroup.cgType == cgTHREE)
			{
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.uctPutCardType = BestCardGroup;
			}
			else if (BestCardGroup.cgType == cgSINGLE_LINE)
			{
				for (int j = BestCardGroup.nMaxCard - BestCardGroup.nCount + 1; j <= BestCardGroup.nMaxCard; j++)
				{
					clsHandCardData.value_nPutCardList.push_back(j);
				}
				clsHandCardData.uctPutCardType = BestCardGroup;
			}
			else if (BestCardGroup.cgType == cgDOUBLE_LINE)
			{
				for (int j = BestCardGroup.nMaxCard - (BestCardGroup.nCount / 2) + 1; j <= BestCardGroup.nMaxCard; j++)
				{
					clsHandCardData.value_nPutCardList.push_back(j);
					clsHandCardData.value_nPutCardList.push_back(j);
				}
				clsHandCardData.uctPutCardType = BestCardGroup;
			}
			else if (BestCardGroup.cgType == cgTHREE_LINE)
			{
				for (int j = BestCardGroup.nMaxCard - (BestCardGroup.nCount / 3) + 1; j <= BestCardGroup.nMaxCard; j++)
				{
					clsHandCardData.value_nPutCardList.push_back(j);
					clsHandCardData.value_nPutCardList.push_back(j);
					clsHandCardData.value_nPutCardList.push_back(j);
				}
				clsHandCardData.uctPutCardType = BestCardGroup;
			}
			else if (BestCardGroup.cgType == cgTHREE_TAKE_ONE)
			{
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.uctPutCardType = BestCardGroup;
			}
			else if (BestCardGroup.cgType == cgTHREE_TAKE_TWO)
			{
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.uctPutCardType = BestCardGroup;
			}
			else if (BestCardGroup.cgType == cgTHREE_TAKE_ONE_LINE)
			{
				for (int j = BestCardGroup.nMaxCard - (BestCardGroup.nCount / 4) + 1; j <= BestCardGroup.nMaxCard; j++)
				{
					clsHandCardData.value_nPutCardList.push_back(j);
					clsHandCardData.value_nPutCardList.push_back(j);
					clsHandCardData.value_nPutCardList.push_back(j);
				}

				if (BestCardGroup.nCount / 4 == 2)
				{
					clsHandCardData.value_nPutCardList.push_back(tmp_1);
					clsHandCardData.value_nPutCardList.push_back(tmp_2);
				}
				if (BestCardGroup.nCount / 4 == 3)
				{
					clsHandCardData.value_nPutCardList.push_back(tmp_1);
					clsHandCardData.value_nPutCardList.push_back(tmp_2);
					clsHandCardData.value_nPutCardList.push_back(tmp_3);
				}
				if (BestCardGroup.nCount / 4 == 4)
				{
					clsHandCardData.value_nPutCardList.push_back(tmp_1);
					clsHandCardData.value_nPutCardList.push_back(tmp_2);
					clsHandCardData.value_nPutCardList.push_back(tmp_3);
					clsHandCardData.value_nPutCardList.push_back(tmp_4);
				}

				clsHandCardData.uctPutCardType = BestCardGroup;
			}
			else if (BestCardGroup.cgType == cgTHREE_TAKE_TWO_LINE)
			{
				for (int j = BestCardGroup.nMaxCard - (BestCardGroup.nCount / 5) + 1; j <= BestCardGroup.nMaxCard; j++)
				{
					clsHandCardData.value_nPutCardList.push_back(j);
					clsHandCardData.value_nPutCardList.push_back(j);
					clsHandCardData.value_nPutCardList.push_back(j);
				}
				if (BestCardGroup.nCount / 5 == 2)
				{
					clsHandCardData.value_nPutCardList.push_back(tmp_1);
					clsHandCardData.value_nPutCardList.push_back(tmp_1);
					clsHandCardData.value_nPutCardList.push_back(tmp_2);
					clsHandCardData.value_nPutCardList.push_back(tmp_2);
				}
				if (BestCardGroup.nCount / 5 == 3)
				{
					clsHandCardData.value_nPutCardList.push_back(tmp_1);
					clsHandCardData.value_nPutCardList.push_back(tmp_1);
					clsHandCardData.value_nPutCardList.push_back(tmp_2);
					clsHandCardData.value_nPutCardList.push_back(tmp_2);
					clsHandCardData.value_nPutCardList.push_back(tmp_3);
					clsHandCardData.value_nPutCardList.push_back(tmp_3);
				}
				clsHandCardData.uctPutCardType = BestCardGroup;
			}
			return;
		}
	}
	//如果没有3-2的非炸牌，则看看有没有单王
	if (clsHandCardData.value_aHandCardList[16] == 1 && clsHandCardData.value_aHandCardList[17] == 0)
	{
		clsHandCardData.value_nPutCardList.push_back(16);
		clsHandCardData.uctPutCardType = get_GroupData(cgSINGLE, 16, 1);
		return;
	}
	if (clsHandCardData.value_aHandCardList[16] == 0 && clsHandCardData.value_aHandCardList[17] == 1)
	{
		clsHandCardData.value_nPutCardList.push_back(17);
		clsHandCardData.uctPutCardType = get_GroupData(cgSINGLE, 17, 1);
		return;
	}
	//单王也没有，出炸弹
	for (int i = 3; i < 16; i++)
	{
		if (clsHandCardData.value_aHandCardList[i] == 4)
		{
			clsHandCardData.value_nPutCardList.push_back(i);
			clsHandCardData.value_nPutCardList.push_back(i);
			clsHandCardData.value_nPutCardList.push_back(i);
			clsHandCardData.value_nPutCardList.push_back(i);

			clsHandCardData.uctPutCardType = get_GroupData(cgBOMB_CARD, i, 4);

			return;
		}
	}

	//异常错误
	clsHandCardData.uctPutCardType = get_GroupData(cgERROR, 0, 0);
	return;

}


//获取叫分函数
int AILogic::LandScore(HandCardData &clsHandCardData)
{
	int SumValue = 0;

	clsHandCardData.uctHandCardValue = get_HandCardValue(clsHandCardData);

	SumValue = clsHandCardData.uctHandCardValue.SumValue;

	if (SumValue < 10)
	{
		return 0;
	}
	else if (SumValue < 15)
	{
		return 1;
	}
	else if (SumValue < 20)
	{
		return 2;
	}
	else
	{
		return 3;
	}
}


//获得卡牌的逻辑数值
BYTE HandCardData::GetCardLogicValue(BYTE cbCardData)
{
	//逻辑数值
	BYTE cbCardColor = (cbCardData & 0xF0) >> 4;
	BYTE cbCardValue = cbCardData & 0x0F;


	//转换数值明显出现错误
	if ((cbCardColor << 4) == 0x40) return cbCardValue + 2;		//大小王   

	return (cbCardValue <= 2) ? (cbCardValue + 13) : cbCardValue;
}