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
int CGamePlayerConfig::LoadPlayerDesc(CCfg& ff, PLAYER_TYPE &player_type)
{
	// ��ʼ���������
	int result = 0;

	long id = 0, _sum = 0, _card_sum = 0;
	ff.GetItemValue("GAME", "CardGroupNum", id);
	ff.GetItemValue("GAME", "CardGroupNum", _sum);
	ff.GetItemValue("GAME", "CardGroupNum", _card_sum);

	SetPlayerGroupID(player_type, id); // id
	SetPlayerCardSum(player_type, _sum); // card_sum
	SetPlayerSum(player_type, 0); // ����player_sum

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
