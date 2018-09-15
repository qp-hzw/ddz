#include "StdAfx.h"
#include "ServerLog.h"
#include <windows.h>
#include <time.h>

CServerLog::CServerLog()
{
	memset(m_plogFile, 0, sizeof m_plogFile);
	m_plog = NULL;
}

CServerLog::~CServerLog()
{
	if (NULL != m_plog)
	{
		memset(m_plogFile, 0, sizeof m_plogFile);
		delete m_plog;
		m_plog = NULL;
	}
}

/**
 * 时间戳转换
 * @param		timebuff		[in]		输出字符串缓冲区
 * @param		bufferlen		[in]		缓冲区大小
 */
int CServerLog::Stamp2Standard(wchar_t *timebuff, unsigned bufferlen)
{
	// 校验入参
	if (	NULL == timebuff ||
			bufferlen > 128)
	{
		return -1;
	}

	time_t tt = time(NULL); 
	struct tm t;
	errno_t err = localtime_s(&t, &tt);// 获取系统当前时间戳
	if (0 == err)
	{
		swprintf_s(	timebuff,
					bufferlen,
					L"\n%d-%02d-%02d %02d:%02d:%02d\n",
					t.tm_year + 1900,
					t.tm_mon + 1,
					t.tm_mday,
					t.tm_hour,
					t.tm_min,
					t.tm_sec);
	}

	return 0;
}

/**
* 此函数获取日志实例
* @param			wcsLogFile		[in]		日志文件
* @param			pplog			[out]		日志文件
*/
int CServerLog::GetLogInstance(const wchar_t *wcsLogFile, CServerLog ** pplog)
{
	// 入参校验
	if (NULL == wcsLogFile ||
		wcslen(wcsLogFile) < 1 ||
		wcslen(wcsLogFile) > 127 ||
		NULL == pplog)
	{
		return  -1;
	}

	int result = 0;
	CServerLog *m_plog = new CServerLog();
	if (NULL != m_plog)
	{
		result = m_plog->SetLogFile(wcsLogFile);
		if (0 == result)
		{
			*pplog = m_plog;
			result = 0;
		}
		else
		{
			result = -1;
		}		
	}
	else
	{
		result = -1;
	}

	return result;
}

/**
* 此函数释放一个日志实例
*/
int CServerLog::Release(CServerLog *m_plog )
{
	if (NULL != m_plog)
	{
		delete m_plog;
		m_plog = NULL;
	}
	
	return 0;
}

int CServerLog::SetLogFile(const wchar_t *wcsLogFile)
{
	if (NULL == wcsLogFile ||
		wcslen(wcsLogFile) < 1 ||
		wcslen(wcsLogFile) > 127)
	{
		return  -1;
	}

	wcscat_s(m_plogFile, sizeof m_plogFile / sizeof (wchar_t), wcsLogFile );

	return 0;
}
