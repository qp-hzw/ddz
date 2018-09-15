#include "StdAfx.h"
#include "GamePlayerConfig.h"

#import "msxml6.dll"
using namespace MSXML2;
#include <time.h>


/**
 * �������е�������� ����� �����������
 * @param		configfile				[in]			�����ļ�
 * @param		pplayers				[out]			���������
 * @param		player_sum				[in-out]		��������鳤��
 * @param		m_plog					[in]			��־�ӿ�
 */
int CGamePlayerConfig::LoadPlayerDesc( const wchar_t *configfile, PLAYER_TYPE &player_type, CServerLog *m_plog )
{
	// �������У��
	if( NULL == configfile || NULL == m_plog )
	{
		// "��������쳣"
		return -1;
	}

	// ��ȡ�����ļ�
	MSXML2::IXMLDOMDocumentPtr pXMLDom;
	HRESULT hr= pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER);
	if (FAILED(hr)) 
	{
		m_plog->LogSingleLine(L"xml����������ʧ��", NULL );
		return -1;
	}

	// ��ʼ���������
	int result = 0;

	try
	{
		pXMLDom->async = VARIANT_FALSE;
		pXMLDom->validateOnParse = VARIANT_FALSE;
		pXMLDom->resolveExternals = VARIANT_FALSE; // ��ʼ����ʶ

		if(pXMLDom->load(configfile) == VARIANT_TRUE)
		{
			MSXML2::IXMLDOMNodeListPtr pnl = pXMLDom->selectNodes(L"//Game"); // �������нڵ�
			if(NULL != pnl)
			{
				for( int i = 0; i < pnl->length; ++i )
				{
					if( 0 == wcsncmp( pnl->item[i]->Getattributes()->getNamedItem(L"name")->text, L"�人����Ȥ������", wcslen(L"�人����Ȥ������")))
					{
						MSXML2::IXMLDOMNodeListPtr pNodes;

						// ���������
						pNodes = pnl->item[i]->selectNodes(L"Players");
						if( NULL != pNodes)	// �������
						{
							for( int j = 0; j < pNodes->length; ++j )
							{
								if( NULL != pNodes->item[j]->selectSingleNode("id"))
								{
									SetPlayerGroupID( player_type, atoi(pNodes->item[j]->selectSingleNode("id")->text)); // id
									if(NULL != pNodes->item[j]->selectSingleNode("card_sum"))
									{
										SetPlayerCardSum( player_type, atoi(pNodes->item[j]->selectSingleNode("card_sum")->text)); // card_sum
										//added by lizhihu ����������������ļ���ȡ���ӿͻ��˴��룬����д��
										SetPlayerSum( player_type, 0); // ����player_sum
									}
									else
									{
										m_plog->LogSingleLine(L"�������card_sum��ǩ����ʧ��", NULL);
										result = -1;
									}
								}
								else
								{
									m_plog->LogSingleLine(L"�������id��ǩ����ʧ��", NULL);
									result = -1;
								}
							}
						}
						else
						{
							m_plog->LogSingleLine(L"�����������̫С", NULL);
							result = -1;
						}
					}
				}
			}
			else
			{
				m_plog->LogSingleLine(L"�ڵ��ȡʧ��", NULL);
				result = -1;
			}
		}
		else
		{
			m_plog->LogSingleLine(L"�����ļ�����ʧ��", NULL);
			result = -1;
		}
	}
	catch(_com_error errorObject)
	{
		m_plog->LogSingleLine(L"�쳣���룺0x%08x", errorObject.Error());
		result = -1;
	}

	return result;
}

/**
 * ����������������鹹�������������
 * @param		dst_player_types	[in-out]	Ŀ�������������
 * @param		player_sum			[in]		����������鳤��
 * @param		src_player_type		[in]		Դ��������鳤��
 */
int CGamePlayerConfig::CreatePlayerDesc( PLAYER_DESCS &dst_player_types, const DWORD player_sum, const PLAYER_TYPE &src_player_type)
{

	// �����������,ÿ�����������Ŀ*���������Ŀ
	int _player_sum = 0;

	// ��ʼ���������
	int result = 0;

	//�������	
	for( DWORD j = 0; j < player_sum; ++j  )
	{
		//������鸳ֵ
		dst_player_types[j].type = src_player_type;
	}

	return result;
}


/**
* �������������ID
*/
void CGamePlayerConfig::SetPlayerGroupID( PLAYER_TYPE &_type, BYTE _id)
{
	_type &= 0x0FFF;
	_type += (_id * 0x1000);
}

/**
* �����������
*/
void CGamePlayerConfig::SetPlayerSum( PLAYER_TYPE &_type, BYTE _sum)
{
	_type &= 0xF0FF;
	_type += (_sum * 0x100);
}

/**
* �������������������
*/
void CGamePlayerConfig::SetPlayerCardSum( PLAYER_TYPE &_type, BYTE _card_sum )
{
	_type &= 0xFF00;
	_type += _card_sum;
}

/**
* ��ȡ���������ID
*/
BYTE CGamePlayerConfig::GetPlayerGroupID( PLAYER_TYPE _type)
{
	return (_type >> 12); //ǰ4λ
}

/**
* ��ȡ���������
*/
BYTE CGamePlayerConfig::GetPlayerSum( PLAYER_TYPE _type)
{
	return ( ( _type >> 8 ) & 0x0F );	//��4λ
}

/**
* ��ȡ���������������
*/
BYTE CGamePlayerConfig::GetPlayerCardSum( PLAYER_TYPE _type )
{
	return ( _type & 0x00FF );			//��8λ
}
