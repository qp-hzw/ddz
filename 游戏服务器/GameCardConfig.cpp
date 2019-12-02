#include "StdAfx.h"
#include "GameCardConfig.h"
#include "ServerLog.h"

#import "msxml6.dll"
using namespace MSXML2;
#include <time.h>

//���ڲ�ϴ��ģʽ�Ŀ�������
const BYTE CGameCardConfig::m_CardData[FULL_COUNT] = {
	31,32,33,34,
	41,42,43,44,
	51,52,53,54,
	61,62,63,64,
	71,72,73,74,
	81,82,83,84,
	91,92,93,94,
	101,102,103,104,
	111,112,113,114,
	121,122,123,124,
	131,132,133,134,
	141,142,143,144,
	151,152,153,154,
	165,166
};


//�������͵���ֵӳ�亯��
BYTE    CGameCardConfig::MapOfCardValue(BYTE CardData)
{
	if (0 == CardData)
	{
		return 0;
	}

	//�ж�A��2�ʹ�С�������
	if (CardData > 140 && CardData < 160)
	{
		return ((CardData % 10 - 1) * 16 + (CardData / 10) - 13);
	}

	if (CardData > 160)
	{
		return (CardData == 165 ? 0x4E : 0x4F);
	}

	//�� ��ֵ+��ɫ   ==>  ��ɫ+��ֵ
	return ((CardData % 10 - 1) * 16 + CardData / 10);
}

/**
 * ��ȡһ���Ƶ�����
 * ʹ�� xml �����ļ� ����һ���Ƶ�����
 * @param  configfile				[in]			�����ļ�
 * @param  cards_sum				[in-out]		�˿���Ŀ
 * @param  m_plog						[in]			��־��¼
 * return					0				����
 *							-1				�쳣
 */
int CGameCardConfig::LoadCardsSum( const wchar_t *configfile, DWORD &cards_sum, CServerLog *m_plog )
{
	int result = 0;

	//����Ĭ��������
	cards_sum = 108;

	if( NULL == configfile )
	{
		return -1;
	}

	// ��ȡ�����ļ�����VS2012�Ͽ����гɹ���VS2017 XML�������Ѿ�����
	MSXML2::IXMLDOMDocumentPtr pXMLDom;
	HRESULT hr= pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER);
	if (FAILED(hr)) 
	{
		if (NULL != m_plog)
			m_plog->LogSingleLine(L"����XML������ʧ��%d", 2);
		return -1;
	}

	try
	{
		pXMLDom->async = VARIANT_FALSE;
		pXMLDom->validateOnParse = VARIANT_FALSE;
		pXMLDom->resolveExternals = VARIANT_FALSE; // ��ʼ����ʶ

		// ��ʼ���������
		cards_sum = MAX_TOTAL_CARD_NUM;			//Ĭ��ֵ

		if(pXMLDom->load(configfile) == VARIANT_TRUE)
		{
			MSXML2::IXMLDOMNodeListPtr pnl = pXMLDom->selectNodes(L"//Game"); // �������нڵ�
			if(NULL != pnl)
			{
				for( int i = 0; i < pnl->length; ++i )
				{
					//���һ���Ƶ�����
					if( 0 == wcscmp( L"�人����Ȥ������", pnl->item[i]->Getattributes()->getNamedItem(L"name")->text))
					{
						if( NULL != pnl->item[i]->selectSingleNode(L"CARDSUM"))
						{
							cards_sum = atoi( pnl->item[i]->selectSingleNode(L"CARDSUM")->text);
						}
					}
				}
			}
			else
			{
				if (NULL != m_plog)
					m_plog->LogSingleLine(L"Game�ڵ㲻����%d", 2);
				result = -1;
			}
		}
		else
		{
			if (NULL != m_plog)
				m_plog->LogSingleLine(L"�����ļ�����ʧ��%d", 2);
			result = -1;
		}
	}
	catch(_com_error errorObject)
	{
		if (NULL != m_plog)
			m_plog->LogSingleLine(L"�쳣���룺0x%08x", errorObject.Error() );
		result = -1;
	}

	if( result == -1) // ����ʧ��
	{
		cards_sum = 0;
	}

	return result;
}


/**
* ��ȡ��ţ��Ϸ�ĵ÷ּӱ�ģʽ
* ʹ�� xml �����ļ� ������Ϸ�ĵ÷ּӱ�ģʽ
* @param  configfile					[in]			�����ļ�
* @param  game_score_Mode				[in]			��Ϸ�÷�ģʽ���������ģʽ
* @param  game_score_times				[in-out]		��Ϸ�÷ּӱ�����
* @param  m_plog						[in]			��־��¼
* return					0				����
*							-1				�쳣
*/
int CGameCardConfig::LoadGameScoreTimes(const wchar_t *configfile, BYTE game_score_Mode, BYTE *game_score_times, CServerLog *m_plog)
{
	int result = 0;
	memset( game_score_times, 1, sizeof(BYTE)*MAX_GAME_SCORE_TYPES);

	if (NULL == configfile || NULL == game_score_times || NULL == m_plog)
	{
		return -1;
	}

	// ��ȡ�����ļ�
	MSXML2::IXMLDOMDocumentPtr pXMLDom;
	HRESULT hr = pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER);
	if (FAILED(hr))
	{
		//m_plog->LogSingleLine(L"����XML������ʧ��%d", 2);
		return -1;
	}

	try
	{
		// ��ʼ����ʶ
		pXMLDom->async = VARIANT_FALSE;
		pXMLDom->validateOnParse = VARIANT_FALSE;
		pXMLDom->resolveExternals = VARIANT_FALSE; 
		
		// �������нڵ�
		if (pXMLDom->load(configfile) == VARIANT_TRUE)
		{
			MSXML2::IXMLDOMNodeListPtr pnl = pXMLDom->selectNodes(L"//Game");
			if (NULL != pnl)
			{					
				for (int i = 0; i < pnl->length; ++i)
				{
					//��õ÷ּӱ�ģʽ
					if ( 0 == wcscmp(L"�人����Ȥ������", pnl->item[i]->Getattributes()->getNamedItem(L"name")->text) )
					{
						MSXML2::IXMLDOMNodeListPtr pNodes;

						// ѡ������ģʽ
						pNodes = pnl->item[i]->selectNodes( L"CAME_SCORE_MODE_CLASSIC" );
						if (NULL != pNodes)
						{
							// �������е� CAME_SCORE_MODE_CLASSIC
							for (int j = 0; j < pNodes->length; j++)
							{
                                //added by WangChengQing 2017/12/8 ʹ��ͨ���Ե����ִ�������
								for (int k = 0; k < MAX_GAME_SCORE_TYPES; k++)
								{
                                    _bstr_t value = 1; //Ĭ��Ϊ1
								    // �����÷ּӱ�
                                    wchar_t wchName[32];
                                    swprintf(wchName, 31, L"FF%d", k);
                                    if ( NULL != pNodes->item[j]->selectSingleNode(wchName))
								    {
										value = pNodes->item[j]->selectSingleNode(wchName)->text;
                                    }                                                                       
								    game_score_times[k] = atoi(value);
									
								}
							}
						}
						else
						{
                            //add ����֮��xml�����ļ���ͨ���ԣ� �˴���Ӧ�ñ������Ǹ�һ��Ĭ��ֵ1
							for (int i = 0; i < MAX_GAME_SCORE_TYPES; i++)
							{
								game_score_times[i] = 1;
							}
						}

						// ѡ�����ӱ�ģʽ
						pNodes = pnl->item[i]->selectNodes( L"CAME_SCORE_MODE_CRAZY" );
						if (NULL != pNodes)
						{
							// �������е� CAME_SCORE_MODE_CRAZY
							for (int j = 0; j < pNodes->length; j++)
							{
                                //added by WangChengQing 2017/12/8 ʹ��ͨ���Ե����ִ�������
								for (int k = 0; k < MAX_GAME_SCORE_TYPES; k++)
								{
                                    _bstr_t value = 1; //Ĭ��Ϊ1
								    // �����÷ּӱ�
                                    wchar_t wchName[32];
                                    swprintf(wchName, 31, L"FF%d", k);
                                    if ( NULL != pNodes->item[j]->selectSingleNode(wchName))
								    {
										value = pNodes->item[j]->selectSingleNode(wchName)->text;
                                    }                                                                       
								    game_score_times[k+MAX_GAME_SCORE_TYPES] = atoi(value);
									
								}
							}
						}
						else
						{
                            //add ����֮��xml�����ļ���ͨ���ԣ� �˴���Ӧ�ñ������Ǹ�һ��Ĭ��ֵ1
							for (int i = 0; i < MAX_GAME_SCORE_TYPES; i++)
							{
								game_score_times[i] = 1;
							}
						}
					}
					else
					{
						//m_plog->LogSingleLine(L"name�ڵ㲻����%s", 2);
						result = -1;
					}
				}
			}
			else
			{
				//m_plog->LogSingleLine(L"Game�ڵ㲻����%d", 2);
				result = -1;
			}
		}
		else
		{
			//m_plog->LogSingleLine(L"�����ļ�����ʧ��%d", 2);
			result = -1;
		}
	}
	catch (_com_error errorObject)
	{
		//m_plog->LogSingleLine(L"�쳣���룺0x%08x", errorObject.Error());
		result = -1;
	}

	return result;
}



/**
 * ����� 
 * ����XML �� ��������ڵ���Ϣ 
 * @param  configfile				[in]			�����ļ�
 * @param  ppgroups					[out]			�˿�����
 * @param  group_sum				[in-out]		��ʵ������
 * @m_plog							[in]			��־��¼
 * return							 0				����
 *							        -1				�쳣
 */
int CGameCardConfig::LoadCardGroups(const wchar_t *configfile, CARD_GOURP *groups, DWORD &group_sum, CServerLog *m_plog)
{
	int result = 0;

	if( NULL == configfile || NULL == groups)
	{
		return -1;
	}

	// ��ȡ�����ļ�
	MSXML2::IXMLDOMDocumentPtr pXMLDom;
	HRESULT hr= pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER);
	if (FAILED(hr)) 
	{
		if ( NULL != m_plog )
			m_plog->LogSingleLine(L"����XML������ʧ��%d", 3);
		
		return -1;
	}

	// ��ʼ�� �������
	pXMLDom->async = VARIANT_FALSE;
	pXMLDom->validateOnParse = VARIANT_FALSE;
	pXMLDom->resolveExternals = VARIANT_FALSE; // ��ʼ����ʶ

	if(pXMLDom->load(configfile) == VARIANT_TRUE)
	{
		MSXML2::IXMLDOMNodeListPtr pnl = pXMLDom->selectNodes(L"//Game"); // ��������Game�ڵ�
		if(NULL != pnl)
		{
			for( int i = 0; i < pnl->length; ++i )
			{
				if( 0 == wcscmp( pnl->item[i]->Getattributes()->getNamedItem(L"name")->text, L"�人����Ȥ������") )
				{
					MSXML2::IXMLDOMNodeListPtr pNodes;

					// ��������˳��
					pNodes = pnl->item[i]->selectNodes(L"CARD_GOURP");			// ѡ�����е� CARD_GOURP
					if( NULL != pNodes)
					{
						//ȡ��С�Ŀ���������
						group_sum = (pNodes->length > MAX_CARD_GROUP_NUM) ? MAX_CARD_GROUP_NUM : pNodes->length;

						for( int j = 0; j < group_sum; j++)		// �������е� CARD_GOURP
						{
							if( NULL != pNodes->item[j]->selectSingleNode(L"color") )	// ����color
							{
								groups[j].color = atoi( pNodes->item[j]->selectSingleNode(L"color")->text );
								if ( NULL != pNodes->item[j]->selectSingleNode(L"begin") )	// ����begin
								{
									groups[j].begin = atoi( pNodes->item[j]->selectSingleNode(L"begin")->text ); 
									if ( 0 != groups[j].begin )
									{
										if ( NULL != pNodes->item[j]->selectSingleNode(L"end") )	// ����end
										{
											groups[j].end = atoi( pNodes->item[j]->selectSingleNode(L"end")->text ); 
											if ( 0 != groups[j].end )
											{
												// ������ɫ
												if( NULL != pNodes->item[j]->selectSingleNode(L"sum") )		//����sum
												{
													groups[j].sum = atoi( pNodes->item[j]->selectSingleNode(L"sum")->text );
												}
												else
												{
													if ( NULL != m_plog )
														m_plog->LogSingleLine(L"����: %d sum�ڵ�ֵ�쳣", j);
													result = -1;
												}
											}
										}
										else
										{
											if ( NULL != m_plog )
												m_plog->LogSingleLine(L"����: %d end�ڵ��쳣", j);
											result = -1;
										}
									}
								}
								else
								{
									if ( NULL != m_plog )
										m_plog->LogSingleLine(L"����: %d begin�ڵ��쳣", j);
									result = -1;
								}
							}
							else
							{
								// �� ID �ڵ㲻����
								if ( NULL != m_plog )
									m_plog->LogSingleLine(L"����: %d color�ڵ㲻����", j);
								result = -1;
							}

							//�쳣�˳�
							if ( -1 == result )
							{
								break;
							}
						}
					}
					else
					{
						if ( NULL != m_plog )
							m_plog->LogSingleLine(L"û�ҵ�CARD_GOURP��ǩ%d", 1);
						result = -1;
					}
				}
			}
		}
		else
		{
			if ( NULL != m_plog )
				m_plog->LogSingleLine(L"�ڵ��ȡʧ��%d", 1);
			result = -1;
		}
	}
	else
	{
		if ( NULL != m_plog )
			m_plog->LogSingleLine(L"�����ļ�����ʧ��%d", 1);
		result = -1;
	}

	return result;
}

/**
 * ��ָ����������ʶ������ת��Ϊֽ�������е�����
 * �˺������˿��б����ҳ��������� group ������ֽ�Ʋ���䵽 cards �����У� ������ִ����ɺ� cards �����еĳ�Ա��
 * @param		group					[in]			�˿���
 * @param		card_ranks				[in]			�˿�����
 * @param		card_ranks_sum			[in]			�˿�������Ŀ
 * @param		cards					[in-out]		�˿�����
 * @param		cardslen				[in]			�˿����鳤��
 * @param		curlen					[in-out]		�Ѿ����ĳ��ȣ� ע�⣬�˲����Ὣ����ֵ������ȥ��ֵ���
 *														���Դ˲��������ֵ������ֵ�й�
 */
int CGameCardConfig::TransGroup(	const CARD_GOURP *group, 
									CARD_DESCS cards, 
									const DWORD maxlen, 
									DWORD &curIndex,
									CServerLog *m_plog)
{
	// �������У��
	if ( NULL == group ||
		 NULL == cards  ||
		maxlen < 1 ||
		curIndex < 0 ||
		 NULL == m_plog )
	{
		return -1;
	}

	int result = 0;

	//У������
	if (group->begin < 1 || group->begin >15
		|| group->end < 1 || group->end  >15
		|| group->begin > group->end)
	{
		return -1;
	}

	// ����ֽ���б��ҳ����з�����������
	for( unsigned i = group->begin; i <= group->end; ++i)	//���Ƶ���
	{
		for (unsigned k = 0; k < group->sum; ++k)		//ͬһ���м���
		{
			// ��ɫ
			if (curIndex < maxlen) // ���card �������Ƿ��п�λ
			{
				cards[curIndex] = i; //����λΪ��ֵ
				cards[curIndex] += group->color * (0xF+1);// ��仨ɫ 5-8λ��ʾ��ɫ
				cards[curIndex] += group->id * (0xF + 1) * (0xF + 1); //9-12λ��ʾid
				++(curIndex);
			}
			else
			{
				// ���
				result = -1;
			}	
		}	
	}

	return result;
}

/**
 * ��ȡ������
 * �˺������˿��б����ҳ��������� group ������ֽ�Ʋ���䵽 cards �����У� ������ִ����ɺ� cards �����еĳ�Ա��
 * @param		groups					[in]			�˿��� ����
 * @param		groups_len				[in]			�˿��� ������󳤶ȣ�ע�⣬����Ҫ��������ʵ�ʳ�Ա����
 * @param		card_ranks				[in]			�˿�����
 * @param		card_ranks_sum			[in]			�˿�������Ŀ
 * @param		ppcards					[out]			�˿�����
 * @param		cardslen				[in-out]		�˿����鳤��,������ⳤ�ȣ�����ʵ�ʳ���
 */
int CGameCardConfig::TransGroups(const CARD_GOURP group[MAX_CARD_GROUP_NUM], const int &nGroupLen,
									CARD_DESC *pGameCards, DWORD &dwCardSum, CServerLog *m_plog)
{
	//У��
	if (NULL == pGameCards || 0 > nGroupLen )
	{
		return -1;
	}

	// ���������ʼ��
	DWORD curIndex = 0;

	// ����ת��
	int result = 0;
	for( int i = 0; i < nGroupLen; ++i )
	{
		if( 0 != group[i].sum ) //��鵱ǰ���Ƿ�Ϊ��
		{
			result = TransGroup( &group[i], pGameCards, dwCardSum, curIndex, m_plog ); // ��������
			if( 0 != result)
			{
				// �����쳣
				if (NULL != m_plog)
					m_plog->LogSingleLine(L"��������쳣", NULL);
				result = -1;
				break;
			}
		}
	}

	// ����У�飬�������С�������������ʵ�ʿ�������
	if ((curIndex+1) < dwCardSum)
	{
		dwCardSum = curIndex;
	}

	return result;
}


/**
 * ����һ����������
 * �˺���������һ��arlen ���ȵ��������飨0��src_len-1������䵽array��0��src_len-1λ��
 * @param			randarray			[in-out]		����	
 * @param			src_len				[in]			���鳤��	
 * @param			m_plog				[in]			��־	
 */
int CGameCardConfig::CreateOutOrderArray( DWORD randarray[], const int src_len )
{
	// ����У��
	if( NULL == randarray )
	{

		return -1;
	}

	// ����һ����������
	memset( randarray, 0, src_len * sizeof( DWORD ));
	for( int i = 0; i < src_len ; i++ )
	{
		// ����һ��0����src_len - i - 1 ��֮��������
		srand((unsigned)time(NULL));
		int randsum = rand()%( src_len - i );

		// �� i ��� randsum ����λ���� 0 ��ʼ �㣩
		int index = 0; // ��λ����
		for( int j = 0; j < src_len; j++)
		{
			if( 0 == randarray[j] )
			{
				// �ж��Ƿ�����Ҫ��λ��
				if( index == randsum )
				{
					randarray[j] = i;
					break;
				}

				++index;
			}
		}
	}

	return 0;
}

////////////////////////////////////////////
//��ϴ��ģʽ��ϴ���㷨       by  lih
int CGameCardConfig::CreateBuXiPaiArray(BYTE randarray[], const int src_len)
{
	cout << "func CreateBuXiPaiArray begin" << endl;

	// ����У��
	if (NULL == randarray)
	{
		return -1;
	}

	BYTE count = 0;
	BYTE index[2] = { 0 };

	//��������±꽻��15��   //���Ʋ��� �ĳ�7��
	while (count < 7)
	{
		index[0] = rand() % src_len;
		index[1] = rand() % src_len;

		if (index[0] != index[1])
		{
			BYTE tmpSwapCard = randarray[index[0]];
			randarray[index[0]] = randarray[index[1]];
			randarray[index[1]] = tmpSwapCard;

			count++;
		}
	}



	cout << "func CreateBuXiPaiArray end" << endl;

	return 0;
}
////////////////////////////////////////////

/** 
 * ������������, ϴ��
 * ÿ�ֿ�ʼʱ, ���Ƶ�˳�����
 * @param		dest_cards			[in-out]		Ŀ�����飬 ����������
 * @param		dest_len			[in]			Ŀ�����鳤��
 * @param		src_cards			[in]			Դ�����飬 �������ļ����ɵ���Ϸ����
 * @param		src_len				[in]			Դ�������� ע�⣬�����ݱ����������ʵ�ʳ���
 */
int CGameCardConfig::OutOrder(		CARD_DESCS dest_cards, 
									const DWORD dest_len, 
									const CARD_DESCS source_cards, 
									const DWORD src_len, 
									const BYTE nMode)
{
	// У���������
	if( NULL == dest_cards	||
		NULL == source_cards ||
		dest_len < src_len  )
	{
		return -1;
	}

	int result = 0;

	if (GAME_SCORE_MODE_CLASSIC == nMode)
	{
		// ����һ����������
		DWORD *randarray = new DWORD[src_len](); // ���򻺳���

		if (0 == CreateOutOrderArray(randarray, src_len))
		{
			for (unsigned i = 0; i < src_len; ++i)
			{
				dest_cards[i] = source_cards[randarray[i]];
			}
		}
		else
		{
			// �������д���
			result = -1;
		}

		delete[] randarray;
	}
	else
	{
		// ����һ����ϴ��ģʽ����������
		BYTE *tmpCards = new BYTE[src_len]();
		CopyMemory(tmpCards, m_CardData, sizeof(BYTE)*src_len);

		srand((unsigned)time(NULL));

		if (0 == CreateBuXiPaiArray(tmpCards, src_len))
		{
			for (int i = 0; i < src_len; i++)
			{
				//��ֵӳ��
				dest_cards[i] = MapOfCardValue(tmpCards[i]);
			}
		}
		else
		{
			result = -1;
		}
	}

	return result;
}

/**
 * �Ƚ������ƴ�С
 * @param			self			[in]		��	
 * @param			rival			[in]		��	
 * @param			flag			[in-out]	���أ� ��self ��, �򷵻�TRUE, ���򷵻�FLASE	
 */
int CGameCardConfig::CompareCards( const CARD_DESC *self, const CARD_DESC *rival, BOOL *flag)
{
	if ( NULL == flag)
	{
		return 0;
	}

	BYTE _self_score = GetCardScore( *self );
	BYTE _self_color = GetCardColor( *self );
	BYTE _rival_score = GetCardScore( *rival );
	BYTE _rival_color = GetCardColor( *rival );

	BOOL _flag = TRUE;
	if ( _self_score > _rival_score )
	{
		_flag = TRUE;
	}
	else if ( _self_score == _rival_score )
	{
		if ( _self_color > _rival_color )
		{
			_flag = TRUE;
		}
		else
		{
			_flag = FALSE;
		}
	}
	else
	{
		_flag = FALSE;
	}

	*flag = _flag;
	return 0;
}

/**
 * ������
 * �Ȱ������ţ�Ȼ�󰴻�ɫ��, ��ʽΪ��С����
 * @param			cards				[in-out]		����	
 * @param			cardslen			[in]			���鳤��	
 */
int CGameCardConfig::OrderCards( CARD_DESCS cards, const DWORD cardslen )
{
	// ����У��

	// ����
	int result = 0;

	CARD_DESC tmp_cards;

	for ( unsigned i = 0; i < cardslen - 1; ++i )
	{
		for( unsigned j = 0; j < cardslen - 1 - i; ++j) 
		{
			BOOL flag = TRUE;
			CompareCards( &cards[j], &cards[j+1], &flag);

			if( TRUE == flag )
			{
				tmp_cards	= cards[j];
				cards[j]	= cards[j+1];
				cards[j+1]	= tmp_cards;
			}
		}
	}

	return result;
}


/**
 * ��ȡ���Ƶ���
 */
BYTE CGameCardConfig::GetCardScore( CARD_DESC card )
{
	return (card & 0x000F); //ȡ0-4λ
}

/**
* ��ȡ���ƻ�ɫ
*/
BYTE CGameCardConfig::GetCardColor(CARD_DESC card)
{
	return (card & 0x00F0 ) >> 4; //ȡ4-8λ << 4
}

/**
* ��ȡ������
*/
BYTE CGameCardConfig::GetCardGroup( CARD_DESC card )
{
	return (card & 0x0F00) >> 8; //ȡ8-12λ
}