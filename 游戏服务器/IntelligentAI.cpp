#include "StdAfx.h"
# include "IntelligentAI.h"


//    19.10.24     by  lih

//============================================����������==============================

//��ʼ��  �������к����Ƹ���
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

//�ж�Ҫ���ȥ�����Ƿ�Ϸ�
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

//�жϴ��ȥ��һ�����Ƿ�Ϸ�
bool  HandCardData::PutOneCard(int value_nCard, int &color_nCard)
{
	bool ret = false;



	//value״̬���鴦��

	value_aHandCardList[value_nCard]--;

	if (value_aHandCardList[value_nCard] < 0)
	{
		return false;
	}


	//value�б����鴦��
	for (vector<int>::iterator iter = value_nHandCardList.begin(); iter != value_nHandCardList.end(); iter++)
	{
		if (*iter == value_nCard)
		{
			value_nHandCardList.erase(iter);
			ret = true;
			break;
		}
	}


	// color�б����鴦��

	int k = (value_nCard - 3) * 4;      //��ֵת��

	for (vector<int>::iterator iter = color_nHandCardList.begin(); iter != color_nHandCardList.end(); iter++)
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

//��ճ�������
void  HandCardData::ClearPutCardList()
{
	memset(&uctPutCardType, 0, sizeof(uctPutCardType));
	uctPutCardType.cgType = cgERROR;
	value_nPutCardList.clear();
	color_nPutCardList.clear();
}




//============================================�����߼���==============================

/*
���ʣ������Ƿ�ֻ��һ����
�ǣ�  ����������������
���ǣ����ش������ͣ�cgERROR��
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


	//��������
	if (nCount == 1)
	{
		//������֤�ı���
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
	//��������
	if (nCount == 2)
	{
		//������֤�ı���
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
	//��������
	if (nCount == 3)
	{
		//������֤�ı���
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
	//����һ��
	if (nCount == 4)
	{
		//������֤�ı���
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
	//����һ��
	if (nCount == 5)
	{
		//������֤�ı���
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
	//�Ĵ�����
	if (nCount == 6)
	{
		//������֤�ı���
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
	//�Ĵ�����
	if (nCount == 8)
	{
		//������֤�ı���
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
		//ע������prov2==4��Ϊ����Ҳ��������
		if (prov1 == 1 && prov2 == 4)
		{
			retCardGroupData.cgType = cgFOUR_TAKE_TWO;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//ը������
	if (nCount == 4)
	{
		//������֤�ı���
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
	//��ը����
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
	//��������
	if (nCount >= 5)
	{
		//������֤�ı���
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
	//��������
	if (nCount >= 6)
	{
		//������֤�ı���
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
	//��������
	if (nCount >= 6)
	{
		//������֤�ı���
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
	//����һ������
	if (nCount >= 8)
	{
		//������֤�ı���
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
	//������������
	if (nCount >= 10)
	{
		//������֤�ı���
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
���ʣ������Ƿ�ֻ��һ���ƣ�vector���أ�
�ǣ�  ����������������
���ǣ����ش������ͣ�cgERROR��
*/
CardGroupData AILogic::ins_SurCardsType(vector<int>list)
{
	int arr[18];
	memset(arr, 0, sizeof(arr));
	for (vector<int>::iterator iter = list.begin(); iter != list.end(); iter++)
	{
		arr[*iter]++;
	}
	return ins_SurCardsType(arr);
}


/*
��װ�õĻ�ȡ����������Ͻṹ����
CardGroupType cgType������
int MaxCard��������С����ֵ
int Count������
����ֵ��CardGroupData
*/
CardGroupData AILogic::get_GroupData(CardGroupType cgType, int MaxCard, int Count)
{
	CardGroupData uctCardGroupData;

	uctCardGroupData.cgType = cgType;
	uctCardGroupData.nCount = Count;
	uctCardGroupData.nMaxCard = MaxCard;

	//��������
	if (cgType == cgZERO)
		uctCardGroupData.nValue = 0;
	//��������
	else if (cgType == cgSINGLE)
		uctCardGroupData.nValue = MaxCard - 10;
	//��������
	else if (cgType == cgDOUBLE)
		uctCardGroupData.nValue = MaxCard - 10;
	//��������
	else if (cgType == cgTHREE)
		uctCardGroupData.nValue = MaxCard - 10;
	//��������
	else if (cgType == cgSINGLE_LINE)
		uctCardGroupData.nValue = MaxCard - 10 + 1;
	//��������
	else if (cgType == cgDOUBLE_LINE)
		uctCardGroupData.nValue = MaxCard - 10 + 1;
	//��������
	else if (cgType == cgTHREE_LINE)
		uctCardGroupData.nValue = (MaxCard - 3 + 1) / 2;
	//����һ��
	else if (cgType == cgTHREE_TAKE_ONE)
		uctCardGroupData.nValue = MaxCard - 10;
	//����һ��
	else if (cgType == cgTHREE_TAKE_TWO)
		uctCardGroupData.nValue = MaxCard - 10;
	//����һ����
	else if (cgType == cgTHREE_TAKE_ONE_LINE)
		uctCardGroupData.nValue = (MaxCard - 3 + 1) / 2;
	//����һ����
	else if (cgType == cgTHREE_TAKE_TWO_LINE)
		uctCardGroupData.nValue = (MaxCard - 3 + 1) / 2;
	//�Ĵ�����
	else if (cgType == cgFOUR_TAKE_ONE)
		uctCardGroupData.nValue = (MaxCard - 3) / 2;
	//�Ĵ�����
	else if (cgType == cgFOUR_TAKE_TWO)
		uctCardGroupData.nValue = (MaxCard - 3) / 2;
	//ը������ 
	else if (cgType == cgBOMB_CARD)
		uctCardGroupData.nValue = MaxCard - 3 + 7;
	//��ը���� 
	else if (cgType == cgKING_CARD)
		uctCardGroupData.nValue = 20;
	//��������
	else
		uctCardGroupData.nValue = 0;


	return uctCardGroupData;
}



/*
ͨ������dp�ķ�ʽ��ȡ���Ƽ�ֵ
��get_PutCardList��Ϊ����ݹ����
���أ���ֵ�ṹ��HandCardValue
Ȩֵ�ļ������ο�ͷ�ļ������߼�˼ά
*/

HandCardValue AILogic::get_HandCardValue(HandCardData &clsHandCardData)
{

	//������ճ��ƶ��У���Ϊ��֦ʱ�ǲ�����get_PutCardList��
	clsHandCardData.ClearPutCardList();

	HandCardValue uctHandCardValue;
	//�������ˣ���ʵ�������ֻ��������ʣ���Ĵ����ұ������Ƶ��������Ϊ�Ĵ�����֦�������⴦��
	if (clsHandCardData.nHandCardCount == 0)
	{
		uctHandCardValue.SumValue = 0;
		uctHandCardValue.NeedRound = 0;
		return uctHandCardValue;
	}
	//������������Ϊ��֦���ж��Ƿ����һ�ֳ�����
	CardGroupData uctCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
	//�������������򲻵������Ƕ�������Ĵ�������������ը��
	if (uctCardGroupData.cgType != cgERROR && uctCardGroupData.cgType != cgFOUR_TAKE_ONE && uctCardGroupData.cgType != cgFOUR_TAKE_TWO)
	{
		uctHandCardValue.SumValue = uctCardGroupData.nValue;
		uctHandCardValue.NeedRound = 1;
		return uctHandCardValue;
	}

	//�Ǽ�֦����������һ���ܳ������

	/*ֻ�ǻ�ȡ���Ƶ����У���clsHandCardData.value_nPutCardList��clsHandCardData.uctPutCardType
	������Ա���޸ı䣬Ҳ������ó��ƺ�����get_PutCardList�������ŷ���*/
	get_PutCardList_2_unlimit(clsHandCardData);

	//Ҫ���浱ǰ��clsHandCardData.value_nPutCardList��clsHandCardData.uctPutCardType���ڻ���
	CardGroupData NowPutCardType = clsHandCardData.uctPutCardType;
	list<int> NowPutCardList = clsHandCardData.value_nPutCardList;

	if (clsHandCardData.uctPutCardType.cgType == cgERROR)
	{
		cout << "PutCardType ERROR!" << endl;
	}



	//---���ݡ�
	for (list<int>::iterator iter = NowPutCardList.begin();
		iter != NowPutCardList.end(); iter++)
	{
		clsHandCardData.value_aHandCardList[*iter]--;
	}
	clsHandCardData.nHandCardCount -= NowPutCardType.nCount;
	//---���ݡ�

	HandCardValue tmp_SurValue = get_HandCardValue(clsHandCardData);//�ݹ�ʣ���Ƽ�ֵ

	//---���ݡ�	
	for (list<int>::iterator iter = NowPutCardList.begin();
		iter != NowPutCardList.end(); iter++)
	{
		clsHandCardData.value_aHandCardList[*iter]++;
	}
	clsHandCardData.nHandCardCount += NowPutCardType.nCount;
	//---���ݡ�

	uctHandCardValue.SumValue = NowPutCardType.nValue + tmp_SurValue.SumValue;
	uctHandCardValue.NeedRound = tmp_SurValue.NeedRound + 1;





	return uctHandCardValue;

}


void AILogic::Put_All_SurCards(GameSituation &clsGameSituation, HandCardData &clsHandCardData, CardGroupData SurCardGroupData)
{
	/*ȫ������*/
	for (int i = 0; i < 18; i++)
		for (int j = 0; j < clsHandCardData.value_aHandCardList[i]; j++)
			clsHandCardData.value_nPutCardList.push_back(i);
	/**********/
	clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = SurCardGroupData;
	return;
}

void AILogic::Put_All_SurCards(HandCardData &clsHandCardData, CardGroupData SurCardGroupData)
{
	/*ȫ������*/
	for (int i = 0; i < 18; i++)
		for (int j = 0; j < clsHandCardData.value_aHandCardList[i]; j++)
			clsHandCardData.value_nPutCardList.push_back(i);
	/**********/
	clsHandCardData.uctPutCardType = SurCardGroupData;
	return;
}

/*
2.0�汾����  ���ݳ������ƾ�����ǰԤ��������ơ�����������
*/
void AILogic::get_PutCardList_2_limit(GameSituation &clsGameSituation, HandCardData &clsHandCardData)
{
	clsHandCardData.ClearPutCardList();


	/*��ը������ǰ����ֻ������ը��Ϊ�����ڶ��ֵ����ȳ����߼��������汾���ڴ˻������Ż�*/
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

	//�������ͣ������ѳ��Ƶļ�ֵ > 1ʱ ѡ�񲻳���
	if (clsGameSituation.NowOutUser != clsGameSituation.nDiZhuID && clsGameSituation.nCardDroit != clsGameSituation.nDiZhuID)   //�Ҳ��ǵ�������������Ҳ��ǵ���
	{
		cout << "clsGameSituation.uctNowCardGroup.nValue::" << clsGameSituation.uctNowCardGroup.nValue << endl;
		//�ж϶��ѳ������Ƶļ�ֵ > 1 ѡ�񲻳���
		if (clsGameSituation.uctNowCardGroup.nValue > 0)
		{
			cout << "buchupai" << endl;
			clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
			return;
		}
	}

	//��������  ����
	if (clsGameSituation.uctNowCardGroup.cgType == cgERROR)
	{
		clsHandCardData.uctPutCardType = get_GroupData(cgERROR, 0, 0);
		return;
	}
	//�������ͣ��ڱ������Ʋ�����Ҳ�Ǵ������� ����
	else if (clsGameSituation.uctNowCardGroup.cgType == cgZERO)
	{
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//��������
	else if (clsGameSituation.uctNowCardGroup.cgType == cgSINGLE)
	{

		//��֦������ܳ�ȥ���һ����ֱ�ӳ�
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

		//�ݴ���ѵļ�ֵ
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//������Ϊ�����ƵĻ����ö���һ���ִΣ�����һ�֣�Ȩֵ����7�����ں����ĶԱȲο���
		BestHandCardValue.NeedRound += 1;

		//�ݴ���ѵ��ƺ�
		int BestMaxCard = 0;
		//�Ƿ���Ƶı�־
		bool PutCards = false;

		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard + 1; i < 18; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 0)
			{
				//���Դ��һ���ƣ�����ʣ�����Ƽ�ֵ
				clsHandCardData.value_aHandCardList[i]--;
				clsHandCardData.nHandCardCount--;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i]++;
				clsHandCardData.nHandCardCount++;

				//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
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

				//���Դ��ը��������ʣ�����Ƽ�ֵ,��Ϊը�����Բ���˳�ӣ�������ΪӰ��˳�Ӷ������ը
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//���ʣ�����Ƽ�ֵΪ����֤����ȥ�ļ��ʺܴ� ��ô������ը�������
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

		//��ը
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0 &&
			(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
		{
			//���ʣ�����Ƽ�ֵΪ����֤����ȥ�ļ��ʺܴ���ô������ը������֣���ը20��
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}

		//�ܲ���
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;

	}
	//��������
	else if (clsGameSituation.uctNowCardGroup.cgType == cgDOUBLE)
	{

		//��֦������ܳ�ȥ���һ����ֱ�ӳ�
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
		//-------------------------------------------����-------------------------------------------

		//�ݴ���ѵļ�ֵ
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);

		//������Ϊ�����ƵĻ����ö���һ���ִΣ�����һ�֣�Ȩֵ����7�����ں����ĶԱȲο���
		BestHandCardValue.NeedRound += 1;

		//�ݴ���ѵ��ƺ�
		int BestMaxCard = 0;
		//�Ƿ���Ƶı�־
		bool PutCards = false;

		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard + 1; i < 18; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 1)
			{
				//���Դ��һ���ƣ�����ʣ�����Ƽ�ֵ
				clsHandCardData.value_aHandCardList[i] -= 2;
				clsHandCardData.nHandCardCount -= 2;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 2;
				clsHandCardData.nHandCardCount += 2;

				//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
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


		//-------------------------------------------ը��-------------------------------------------

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4 &&
				(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
			{

				//���Դ��ը��������ʣ�����Ƽ�ֵ,��Ϊը�����Բ���˳�ӣ�������ΪӰ��˳�Ӷ������ը
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//���ʣ�����Ƽ�ֵΪ����֤����ȥ�ļ��ʺܴ� ��ô������ը�������
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

		//��ը
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0 &&
			(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
		{
			//���ʣ�����Ƽ�ֵΪ����֤����ȥ�ļ��ʺܴ���ô������ը������֣���ը20��
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}

		//�ܲ���
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//��������
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE)
	{
		//��֦������ܳ�ȥ���һ����ֱ�ӳ�
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
		//-------------------------------------------����-------------------------------------------

		//�ݴ���ѵļ�ֵ
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//������Ϊ�����ƵĻ����ö���һ���ִΣ�����һ�֣�Ȩֵ����7�����ں����ĶԱȲο���
		BestHandCardValue.NeedRound += 1;

		//�ݴ���ѵ��ƺ�
		int BestMaxCard = 0;
		//�Ƿ���Ƶı�־
		bool PutCards = false;

		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard + 1; i < 18; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				//���Դ��һ���ƣ�����ʣ�����Ƽ�ֵ
				clsHandCardData.value_aHandCardList[i] -= 3;
				clsHandCardData.nHandCardCount -= 3;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 3;
				clsHandCardData.nHandCardCount += 3;

				//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
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


		//-------------------------------------------ը��-------------------------------------------

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4 &&
				(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
			{

				//���Դ��ը��������ʣ�����Ƽ�ֵ,��Ϊը�����Բ���˳�ӣ�������ΪӰ��˳�Ӷ������ը
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//���ʣ�����Ƽ�ֵΪ����֤����ȥ�ļ��ʺܴ� ��ô������ը�������
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

		//��ը
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0 &&
			(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
		{
			//���ʣ�����Ƽ�ֵΪ����֤����ȥ�ļ��ʺܴ���ô������ը������֣���ը20��
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}

		//�ܲ���
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//��������
	else if (clsGameSituation.uctNowCardGroup.cgType == cgSINGLE_LINE)
	{
		CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
		if (SurCardGroupData.cgType == cgSINGLE_LINE && SurCardGroupData.nMaxCard > clsGameSituation.uctNowCardGroup.nMaxCard
			&&SurCardGroupData.nCount == clsGameSituation.uctNowCardGroup.nCount)
		{
			Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
			return;
		}

		//�ݴ���ѵļ�ֵ
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//������Ϊ�����ƵĻ����ö���һ���ִΣ�����һ�֣�Ȩֵ����7�����ں����ĶԱȲο���
		BestHandCardValue.NeedRound += 1;

		//�ݴ���ѵ��ƺ�
		int BestMaxCard = 0;
		//�Ƿ���Ƶı�־
		bool PutCards = false;

		//��֤˳�ӵı�־
		int prov = 0;
		//˳�����
		int start_i = 0;
		//˳���յ�
		int end_i = 0;
		//˳�ӳ���
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

				//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
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

		//-------------------------------------------ը��-------------------------------------------

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4 &&
				(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
			{

				//���Դ��ը��������ʣ�����Ƽ�ֵ,��Ϊը�����Բ���˳�ӣ�������ΪӰ��˳�Ӷ������ը
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//���ʣ�����Ƽ�ֵΪ����֤����ȥ�ļ��ʺܴ� ��ô������ը�������
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

		//��ը
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0 &&
			(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
		{
			//���ʣ�����Ƽ�ֵΪ����֤����ȥ�ļ��ʺܴ���ô������ը������֣���ը20��
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}

		//�ܲ���
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//��������
	else if (clsGameSituation.uctNowCardGroup.cgType == cgDOUBLE_LINE)
	{

		//��֦������ܳ�ȥ���һ����ֱ�ӳ�
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


		//�ݴ���ѵļ�ֵ
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//������Ϊ�����ƵĻ����ö���һ���ִΣ�����һ�֣�Ȩֵ����7�����ں����ĶԱȲο���
		BestHandCardValue.NeedRound += 1;

		//�ݴ���ѵ��ƺ�
		int BestMaxCard = 0;
		//�Ƿ���Ƶı�־
		bool PutCards = false;
		//��֤˳�ӵı�־
		int prov = 0;
		//˳�����
		int start_i = 0;
		//˳���յ�
		int end_i = 0;
		//˳�ӳ���
		int length = clsGameSituation.uctNowCardGroup.nCount / 2;
		//2����������˳�ӣ��ӵ�ǰ�Ѵ����˳����С��ֵ+1��ʼ����
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

				//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
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

		//-------------------------------------------ը��-------------------------------------------

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4 &&
				(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
			{

				//���Դ��ը��������ʣ�����Ƽ�ֵ,��Ϊը�����Բ���˳�ӣ�������ΪӰ��˳�Ӷ������ը
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//���ʣ�����Ƽ�ֵΪ����֤����ȥ�ļ��ʺܴ� ��ô������ը�������
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

		//��ը
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0 &&
			(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
		{
			//���ʣ�����Ƽ�ֵΪ����֤����ȥ�ļ��ʺܴ���ô������ը������֣���ը20��
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}

		//�ܲ���
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;

	}
	//��������
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE_LINE)
	{

		//��֦������ܳ�ȥ���һ����ֱ�ӳ�
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


		//�ݴ���ѵļ�ֵ
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//������Ϊ�����ƵĻ����ö���һ���ִΣ�����һ�֣�Ȩֵ����7�����ں����ĶԱȲο���
		BestHandCardValue.NeedRound += 1;

		//�ݴ���ѵ��ƺ�
		int BestMaxCard = 0;
		//�Ƿ���Ƶı�־
		bool PutCards = false;
		//��֤˳�ӵı�־
		int prov = 0;
		//˳�����
		int start_i = 0;
		//˳���յ�
		int end_i = 0;
		//˳�ӳ���
		int length = clsGameSituation.uctNowCardGroup.nCount / 3;
		//2����������˳�ӣ��ӵ�ǰ�Ѵ����˳����С��ֵ+1��ʼ����
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

				//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
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

		//-------------------------------------------ը��-------------------------------------------

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4 &&
				(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
			{

				//���Դ��ը��������ʣ�����Ƽ�ֵ,��Ϊը�����Բ���˳�ӣ�������ΪӰ��˳�Ӷ������ը
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//���ʣ�����Ƽ�ֵΪ����֤����ȥ�ļ��ʺܴ� ��ô������ը�������
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

		//��ը
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0 &&
			(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
		{
			//���ʣ�����Ƽ�ֵΪ����֤����ȥ�ļ��ʺܴ���ô������ը������֣���ը20��
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}

		//�ܲ���
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//����һ��
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE_TAKE_ONE)
	{
		//�ݴ���ѵļ�ֵ
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);
		//������Ϊ�����ƵĻ����ö���һ���ִΣ�����һ�֣�Ȩֵ����7�����ں����ĶԱȲο���
		BestHandCardValue.NeedRound += 1;
		//�ݴ���ѵ��ƺ�
		int BestMaxCard = 0;
		//˳����ȥ����
		int tmp_1 = 0;
		//�Ƿ���Ƶı�־
		bool PutCards = false;
		//����һ
		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard + 1; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				for (int j = 3; j < 18; j++)
				{
					//ѡ��һ�����ϵ����Ҳ���ѡ�����ŵ��Ǹ���
					if (clsHandCardData.value_aHandCardList[j] > 0 && j != i)
					{
						clsHandCardData.value_aHandCardList[i] -= 3;
						clsHandCardData.value_aHandCardList[j] -= 1;
						clsHandCardData.nHandCardCount -= 4;
						HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
						clsHandCardData.value_aHandCardList[i] += 3;
						clsHandCardData.value_aHandCardList[j] += 1;
						clsHandCardData.nHandCardCount += 4;
						//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
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

		//�ܲ���
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//����һ��
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE_TAKE_TWO)
	{

		//�ݴ���ѵļ�ֵ
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);
		//������Ϊ�����ƵĻ����ö���һ���ִΣ�����һ�֣�Ȩֵ����7�����ں����ĶԱȲο���
		BestHandCardValue.NeedRound += 1;
		//�ݴ���ѵ��ƺ�
		int BestMaxCard = 0;
		//˳����ȥ����
		int tmp_1 = 0;
		//�Ƿ���Ƶı�־
		bool PutCards = false;

		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard + 1; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				for (int j = 3; j < 16; j++)
				{
					//ѡ��һ�����ϵ����Ҳ���ѡ�����ŵ��Ǹ���
					if (clsHandCardData.value_aHandCardList[j] > 1 && j != i)
					{
						clsHandCardData.value_aHandCardList[i] -= 3;
						clsHandCardData.value_aHandCardList[j] -= 2;
						clsHandCardData.nHandCardCount -= 5;
						HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
						clsHandCardData.value_aHandCardList[i] += 3;
						clsHandCardData.value_aHandCardList[j] += 2;
						clsHandCardData.nHandCardCount += 5;
						//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
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

		//�ܲ���
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//����һ����
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE_TAKE_ONE_LINE)
	{

		//�ݴ���ѵļ�ֵ
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//������Ϊ�����ƵĻ����ö���һ���ִΣ�����һ�֣�Ȩֵ����7�����ں����ĶԱȲο���
		BestHandCardValue.NeedRound += 1;

		//�ݴ���ѵ��ƺ�
		int BestMaxCard = 0;
		//�Ƿ���Ƶı�־
		bool PutCards = false;
		//��֤˳�ӵı�־
		int prov = 0;
		//˳�����
		int start_i = 0;
		//˳���յ�
		int end_i = 0;
		//˳�ӳ���
		int length = clsGameSituation.uctNowCardGroup.nCount / 4;

		int tmp_1 = 0;
		int tmp_2 = 0;
		int tmp_3 = 0;
		int tmp_4 = 0;
		//2����������˳�ӣ��ӵ�ǰ�Ѵ����˳����С��ֵ+1��ʼ����
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

				/*��������ȫ����ѡȡ��������Ȼ��ö�ٳ���߼�ֵ�ģ������ǵ����ɻ�����Ҳ������2-4֮��
				���Ըɴ���������֧��������*/
				//Ϊ�����ɻ�
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

									//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
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
				//Ϊ�����ɻ�
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
											//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
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
				//Ϊ�����ɻ�
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
													//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
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

		//�ܲ���
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//����һ����
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE_TAKE_TWO_LINE)
	{
		int i;

		//�ݴ���ѵļ�ֵ
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//������Ϊ�����ƵĻ����ö���һ���ִΣ�����һ�֣�Ȩֵ����7�����ں����ĶԱȲο���
		BestHandCardValue.NeedRound += 1;

		//�ݴ���ѵ��ƺ�
		int BestMaxCard = 0;
		//�Ƿ���Ƶı�־
		bool PutCards = false;
		//��֤˳�ӵı�־
		int prov = 0;
		//˳�����
		int start_i = 0;
		//˳���յ�
		int end_i = 0;
		//˳�ӳ���
		int length = clsGameSituation.uctNowCardGroup.nCount / 4;

		int tmp_1 = 0;
		int tmp_2 = 0;
		int tmp_3 = 0;
		int tmp_4 = 0;
		//2����������˳�ӣ��ӵ�ǰ�Ѵ����˳����С��ֵ+1��ʼ����
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

				/*��������ȫ����ѡȡ��������Ȼ��ö�ٳ���߼�ֵ�ģ������ǵ����ɻ�����Ҳ������2-4֮��
				���Ըɴ���������֧��������*/
				//Ϊ�����ɻ�
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

									//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
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
				//Ϊ�����ɻ�
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
											//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
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
				//Ϊ�����ɻ�
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
													//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
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

		//�ܲ���
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//�Ĵ�����
	else if (clsGameSituation.uctNowCardGroup.cgType == cgFOUR_TAKE_ONE)
	{

		//�ݴ���ѵļ�ֵ
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);
		//������Ϊ�����ƵĻ����ö���һ���ִΣ�����һ�֣�Ȩֵ����7�����ں����ĶԱȲο���
		BestHandCardValue.NeedRound += 1;
		//�ݴ���ѵ��ƺ�
		int BestMaxCard = 0;
		//˳����ȥ����
		int tmp_1 = 0;
		int tmp_2 = 0;
		//�Ƿ���Ƶı�־
		bool PutCards = false;

		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard + 1; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4)
			{
				for (int j = 3; j < 18; j++)
				{
					//��ѡ��һ�����ϵ����Ҳ���ѡ�����ŵ��Ǹ���
					if (clsHandCardData.value_aHandCardList[j] > 0 && j != i)
					{
						//��ѡ��һ�����ϵ����Ҳ���ѡ�����ŵ��Ǹ����Ҳ��ǵ�һ��ѡ�������ڸ��ƣ��������Ĵ����������һ��,������ը��
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

								//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
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

		//�ܲ���
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//�Ĵ�����
	else if (clsGameSituation.uctNowCardGroup.cgType == cgFOUR_TAKE_TWO)
	{

		//����������Ȩֵ��С���������û��ѡ������õ��Ĵ������ͣ���ôҲҪը����Ϊ������
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
		//��ը
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0 &&
			(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
		{

			clsHandCardData.value_nPutCardList.push_back(17);
			clsHandCardData.value_nPutCardList.push_back(16);

			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);

			return;
		}

		//�ܲ���
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//ը������ 
	else if (clsGameSituation.uctNowCardGroup.cgType == cgBOMB_CARD)
	{

		/*�����Զ�������������ը��ը*/

		//�ݴ���ѵļ�ֵ
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);

		if (BestHandCardValue.SumValue > 14)
		{
			//ը����������ֱ��ը�������ǲ�ֺ������Ϊ������
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
			//��ը
			if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0 &&
				(clsGameSituation.NowOutUser == clsGameSituation.nDiZhuID || clsGameSituation.nCardDroit == clsGameSituation.nDiZhuID))
			{

				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);

				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);

				return;
			}
		}

		//�ܲ���
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//��ը���� 
	else if (clsGameSituation.uctNowCardGroup.cgType == cgKING_CARD)
	{
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//�쳣���� ����
	else
	{
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
	}
	return;
}


//��������
void AILogic::get_PutCardList_2_unlimit(HandCardData &clsHandCardData)
{

	clsHandCardData.ClearPutCardList();

	//��֦������ܳ�ȥ���һ����ֱ�ӳ�
	CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);

	//�����һ���Գ�ȥ�Ҳ����Ĵ�������Ϊ��������������ʣ�Ĵ����ƵĻ����Կ����ȴ�һ��Ȼ��ը�����˫������
	if (SurCardGroupData.cgType != cgERROR && SurCardGroupData.cgType != cgFOUR_TAKE_ONE && SurCardGroupData.cgType != cgFOUR_TAKE_TWO)
	{
		Put_All_SurCards(clsHandCardData, SurCardGroupData);
		return;
	}

	/*��ը������ǰ����ֻ������ը��Ϊ�����ڶ��ֵ����ȳ����߼��������汾���ڴ˻������Ż�*/
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

	//�ݴ���ѵļ�ֵ
	HandCardValue BestHandCardValue;
	BestHandCardValue.NeedRound = 20;
	BestHandCardValue.SumValue = MinCardsValue;
	//������Ϊ�����ƵĻ����ö���һ���ִΣ�����һ�֣�Ȩֵ����7�����ں����ĶԱȲο���
	BestHandCardValue.NeedRound += 1;

	//�ݴ���ѵ����
	CardGroupData BestCardGroup;

	//����ȥ����
	int tmp_1 = 0;
	int tmp_2 = 0;
	int tmp_3 = 0;
	int tmp_4 = 0;
	//���ȴ������ơ��ɻ�����
	for (int i = 3; i < 16; i++)
	{
		//2.0�汾�����������Ʋ����ը�������Լ������Ͳ����Ĵ�������Ϊ��
		if (clsHandCardData.value_aHandCardList[i] != 4 && i <= 10)
		{
			//������һ
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
						//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
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
			//��������
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
						//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
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
			//���Ĵ�����
			if (clsHandCardData.value_aHandCardList[i] > 3)
			{
				//2.0�汾�����������Ʋ����ը����
			}
			//���Ĵ�����
			if (clsHandCardData.value_aHandCardList[i] > 3)
			{
				//2.0�汾�����������Ʋ����ը����
			}
			//������һ����
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
					//Ϊ�����ɻ�
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
					//Ϊ�����ɻ�
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
					//Ϊ�����ɻ�
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
					//��prov==5�����ǵ�������ֱ�ӳ�ȥ���ڼ�֦�����Ѿ�����
				}

			}
			//������һ˫��
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
					/*��������ȫ����ѡȡ��������Ȼ��ö�ٳ���߼�ֵ�ģ������ǵ����ɻ�����Ҳ������2-4֮��
					���Ըɴ���������֧��������*/
					//Ϊ�����ɻ�
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
					//Ϊ�����ɻ�
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
					//��prov==4�����ǵ�������ֱ�ӳ�ȥ���ڼ�֦�����Ѿ�����
				}
			}
		}
	}
	//�ⲿ�ֳ��ƴ���ŵ�ѭ����
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


	//��֮����ǰ��ֵ��͵��ƣ����ڲ����ٿ��������ƿ��ܱ����ƴ��������
	for (int i = 3; i < 16; i++)
	{
		if (clsHandCardData.value_aHandCardList[i] != 0 && clsHandCardData.value_aHandCardList[i] != 4)
		{
			//������
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
			//������
			if (clsHandCardData.value_aHandCardList[i] > 1)
			{
				//���Դ��һ���ƣ�����ʣ�����Ƽ�ֵ
				clsHandCardData.value_aHandCardList[i] -= 2;
				clsHandCardData.nHandCardCount -= 2;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 2;
				clsHandCardData.nHandCardCount += 2;

				//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
				{
					BestHandCardValue = tmpHandCardValue;
					BestCardGroup = get_GroupData(cgDOUBLE, i, 2);
				}
			}
			//������
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				clsHandCardData.value_aHandCardList[i] -= 3;
				clsHandCardData.nHandCardCount -= 3;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 3;
				clsHandCardData.nHandCardCount += 3;

				//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
				{
					BestHandCardValue = tmpHandCardValue;
					BestCardGroup = get_GroupData(cgTHREE, i, 3);
				}
			}

			//����˳
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

						//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
						if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
						{
							BestHandCardValue = tmpHandCardValue;
							BestCardGroup = get_GroupData(cgSINGLE_LINE, j, prov);
						}
					}
				}

			}
			//��˫˳
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

						//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
						if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
						{
							BestHandCardValue = tmpHandCardValue;
							BestCardGroup = get_GroupData(cgDOUBLE_LINE, j, prov * 2);
						}
					}
				}
			}
			//����˳
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

						//ѡȡ��Ȩֵ-�ִ�*7ֵ��ߵĲ���  ��Ϊ������Ϊʣ���������Ҫn�ο��ֵĻ�����ܳ��꣬���ִ����ͺܴ���ը���� ����-7�ļ�ֵҲ��Ϊ��
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
	//���û��3-2�ķ�ը�ƣ��򿴿���û�е���
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
	//����Ҳû�У���ը��
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

	//�쳣����
	clsHandCardData.uctPutCardType = get_GroupData(cgERROR, 0, 0);
	return;

}


//��ȡ�зֺ���
int AILogic::LandScore(HandCardData &clsHandCardData)
{
	int SumValue = 0;

	clsHandCardData.uctHandCardValue = get_HandCardValue(clsHandCardData);

	SumValue = clsHandCardData.uctHandCardValue.SumValue;

	cout << "SumValue is :" << SumValue << ",";

	cout << "NeedRound is :" << clsHandCardData.uctHandCardValue.NeedRound << endl;

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


//��ÿ��Ƶ��߼���ֵ
BYTE HandCardData::GetCardLogicValue(BYTE cbCardData)
{
	//�߼���ֵ
	BYTE cbCardColor = (cbCardData & 0xF0) >> 4;
	BYTE cbCardValue = cbCardData & 0x0F;


	//ת����ֵ���Գ��ִ���
	if ((cbCardColor << 4) == 0x40) return cbCardValue + 2;		//��С��   

	return (cbCardValue <= 2) ? (cbCardValue + 13) : cbCardValue;
}