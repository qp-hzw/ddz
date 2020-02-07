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

//MFC 文件
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>

/***全局文件***/
//定义文件
#include "../子依赖项/全局定义/Macro.h"

//结构文件
#include "../子依赖项/游戏服务/Struct_Frame.h"
#include "../子依赖项/游戏服务/Define.h"
#include "../子依赖项/游戏服务/TableFrameBase.h"
#include "../子依赖项/全局定义/Packet.h"

//模板文件
#include "../子依赖项/全局定义/Array.h"
#include "../子依赖项/服务核心/log.h"
#include "../子依赖项/服务核心/cfg.h"
#include "..\消息定义\CMD_SubGame.h"


//链接代码
#pragma comment (lib,"../子依赖项/链接库/ServiceCore.lib")
#pragma comment (lib,"../子依赖项/链接库/KernelEngine.lib")
# include <iostream>
using namespace std;

#endif