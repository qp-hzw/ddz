#include "StdAfx.h"
#include "log.h"
#include <iostream>

FILE  				*m_pstFile = NULL;

CLog::CLog()
{
}

CLog::~CLog()
{
	if(m_pstFile)
	{
		fclose(m_pstFile);
	}
}

//设置输出文件
bool CLog::Init(const char* szFileName)
{
	if(!szFileName)
		return false;

	m_pstFile = fopen(szFileName, "a+");
	if (m_pstFile == NULL)
	{
		return false;
	}

	return true;
}

//log日志
void CLog::Log(enTraceLevel2 TraceLevel, const char* pszFormat, ...)
{
	//if(!m_pstFile) return;
	if(!pszFormat) return;
	
	std::string log_str;
	switch (TraceLevel)
	{
		case log_debug:
			log_str += "[Dbug]";
			break;
		case log_warn:
			log_str += "[Warn]";
			break;
		case log_error:
			log_str += "[Err!]";
			break;
	}

	char psz[128];
	va_list args;
	va_start(args, pszFormat);
	vsprintf(psz, pszFormat, args);
	va_end(args);

	log_str +=  psz;

	std::cout << log_str.c_str() << std::endl;

	//vfprintf(m_pstFile, "%s\n", log_str);
	//fflush(m_pstFile);
}