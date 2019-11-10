#ifndef GAME_LLLL
#define GAME_LLLL

#pragma once

//////////////////////////////////////////////////////////////////////////////////

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0502
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0502
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0502
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0400
#endif

#define _ATL_ATTRIBUTES
#define _AFX_ALL_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

//////////////////////////////////////////////////////////////////////////////////

//MFC �ļ�
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>

/***ȫ���ļ�***/
//�����ļ�
#include "../��������/ȫ�ֶ���/Macro.h"
#include "../��������/ȫ�ֶ���/Define.h"

//�ṹ�ļ�
#include "../��������/ȫ�ֶ���/Struct_Frame.h"
#include "../��������/ȫ�ֶ���/Packet.h"
//#include "../��������/ȫ�ֶ���/Property.h"

//ģ���ļ�
#include "../��������/ȫ�ֶ���/Array.h"
#include "../��������/ȫ�ֶ���/Module.h"
#include "../��������/ȫ�ֶ���/PacketAide.h"
//#include "../��������/ȫ�ֶ���/ServerRule.h"
//#include "../��������/ȫ�ֶ���/RightDefine.h"

#include "..\��������\ȫ�ֶ���\Version.h"
#include "..\��������\��Ϸ����\GameServiceHead.h"

#include "..\��Ϣ����\CMD_SubGame.h"

#endif