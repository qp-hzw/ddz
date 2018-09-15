#include "StdAfx.h"
#include "GamePlayerConfig.h"

#import "msxml6.dll"
using namespace MSXML2;
#include <time.h>


/**
 * 解析所有的玩家类型 ，输出 玩家类型数组
 * @param		configfile				[in]			配置文件
 * @param		pplayers				[out]			玩家组数组
 * @param		player_sum				[in-out]		玩家组数组长度
 * @param		m_plog					[in]			日志接口
 */
int CGamePlayerConfig::LoadPlayerDesc( const wchar_t *configfile, PLAYER_TYPE &player_type, CServerLog *m_plog )
{
	// 输入参数校验
	if( NULL == configfile || NULL == m_plog )
	{
		// "输入参数异常"
		return -1;
	}

	// 读取配置文件
	MSXML2::IXMLDOMDocumentPtr pXMLDom;
	HRESULT hr= pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER);
	if (FAILED(hr)) 
	{
		m_plog->LogSingleLine(L"xml解析器加载失败", NULL );
		return -1;
	}

	// 初始化输出参数
	int result = 0;

	try
	{
		pXMLDom->async = VARIANT_FALSE;
		pXMLDom->validateOnParse = VARIANT_FALSE;
		pXMLDom->resolveExternals = VARIANT_FALSE; // 初始化标识

		if(pXMLDom->load(configfile) == VARIANT_TRUE)
		{
			MSXML2::IXMLDOMNodeListPtr pnl = pXMLDom->selectNodes(L"//Game"); // 加载所有节点
			if(NULL != pnl)
			{
				for( int i = 0; i < pnl->length; ++i )
				{
					if( 0 == wcsncmp( pnl->item[i]->Getattributes()->getNamedItem(L"name")->text, L"武汉四五趣游网络", wcslen(L"武汉四五趣游网络")))
					{
						MSXML2::IXMLDOMNodeListPtr pNodes;

						// 解析玩家组
						pNodes = pnl->item[i]->selectNodes(L"Players");
						if( NULL != pNodes)	// 检查数组
						{
							for( int j = 0; j < pNodes->length; ++j )
							{
								if( NULL != pNodes->item[j]->selectSingleNode("id"))
								{
									SetPlayerGroupID( player_type, atoi(pNodes->item[j]->selectSingleNode("id")->text)); // id
									if(NULL != pNodes->item[j]->selectSingleNode("card_sum"))
									{
										SetPlayerCardSum( player_type, atoi(pNodes->item[j]->selectSingleNode("card_sum")->text)); // card_sum
										//added by lizhihu 玩家人数不从配置文件读取，从客户端传入，这里写死
										SetPlayerSum( player_type, 0); // 解析player_sum
									}
									else
									{
										m_plog->LogSingleLine(L"玩家类型card_sum标签解析失败", NULL);
										result = -1;
									}
								}
								else
								{
									m_plog->LogSingleLine(L"玩家类型id标签解析失败", NULL);
									result = -1;
								}
							}
						}
						else
						{
							m_plog->LogSingleLine(L"玩家类型数组太小", NULL);
							result = -1;
						}
					}
				}
			}
			else
			{
				m_plog->LogSingleLine(L"节点获取失败", NULL);
				result = -1;
			}
		}
		else
		{
			m_plog->LogSingleLine(L"配置文件加载失败", NULL);
			result = -1;
		}
	}
	catch(_com_error errorObject)
	{
		m_plog->LogSingleLine(L"异常代码：0x%08x", errorObject.Error());
		result = -1;
	}

	return result;
}

/**
 * 根据玩家组描述数组构造玩家描述数组
 * @param		dst_player_types	[in-out]	目的玩家类型数组
 * @param		player_sum			[in]		玩家类型数组长度
 * @param		src_player_type		[in]		源玩家组数组长度
 */
int CGamePlayerConfig::CreatePlayerDesc( PLAYER_DESCS &dst_player_types, const DWORD player_sum, const PLAYER_TYPE &src_player_type)
{

	// 计算玩家数量,每种玩家类型数目*玩家类型数目
	int _player_sum = 0;

	// 初始化输出参数
	int result = 0;

	//玩家类型	
	for( DWORD j = 0; j < player_sum; ++j  )
	{
		//玩家数组赋值
		dst_player_types[j].type = src_player_type;
	}

	return result;
}


/**
* 设置玩家类型组ID
*/
void CGamePlayerConfig::SetPlayerGroupID( PLAYER_TYPE &_type, BYTE _id)
{
	_type &= 0x0FFF;
	_type += (_id * 0x1000);
}

/**
* 设置玩家数量
*/
void CGamePlayerConfig::SetPlayerSum( PLAYER_TYPE &_type, BYTE _sum)
{
	_type &= 0xF0FF;
	_type += (_sum * 0x100);
}

/**
* 设置玩家类型手牌数量
*/
void CGamePlayerConfig::SetPlayerCardSum( PLAYER_TYPE &_type, BYTE _card_sum )
{
	_type &= 0xFF00;
	_type += _card_sum;
}

/**
* 提取玩家类型组ID
*/
BYTE CGamePlayerConfig::GetPlayerGroupID( PLAYER_TYPE _type)
{
	return (_type >> 12); //前4位
}

/**
* 提取玩家类数量
*/
BYTE CGamePlayerConfig::GetPlayerSum( PLAYER_TYPE _type)
{
	return ( ( _type >> 8 ) & 0x0F );	//中4位
}

/**
* 提取玩家类型手牌数量
*/
BYTE CGamePlayerConfig::GetPlayerCardSum( PLAYER_TYPE _type )
{
	return ( _type & 0x00FF );			//后8位
}
