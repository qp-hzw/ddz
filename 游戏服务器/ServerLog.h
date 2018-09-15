#pragma once
// 单例模式的日志类
class CServerLog
{
private:
	CServerLog();
	~CServerLog();

// 写日志
public:
	// 写一条日志模板
	template<typename T>
	int LogSingleLine(wchar_t *formatstr, T msg, unsigned CodePage = CP_ACP);

	// 写多条日志模板
	template<typename T>
	int LogMutilLine( wchar_t *formatstr[], T msg[], unsigned logsum, unsigned CodePage = CP_ACP);

protected:

	// 写多条日志模板
	template<typename T>
	int LogOut(wchar_t *formatstr[], T msg[], unsigned logsum, unsigned CodePage);
	
	// 时间戳转换
	int Stamp2Standard(wchar_t *timebuff, unsigned bufferlen);


// 单例实现
public:
	/**
	 * 此函数获取日志实例
	 * @param			wcsLogFile		[in]		日志文件
	 * @param			pplog			[out]		日志实例
	 */
	static int GetLogInstance( const wchar_t *wcsLogFile, CServerLog ** pplog );

	/**
	 * 此函数释放一个日志实例
	 */
	static int Release(CServerLog *pplog);

protected:

	// 为日志实例设置日志文件目录
	int SetLogFile(const wchar_t *wcsLogFile);

protected:
	CServerLog *m_plog;				// 日志实例
	wchar_t		m_plogFile[128];	// 日志文件路径
};

#include <fstream>

using std::ofstream;
using std::ios_base;

// 写一条日志模板
template<typename T>
int CServerLog::LogSingleLine( wchar_t *formatstr, T msg, unsigned CodePage)
{
	wchar_t *_formatstr[1];
	_formatstr[0] = formatstr;

	T _msg[1];
	_msg[0] = msg;
	return LogOut(_formatstr, _msg, 1, CodePage);
	
}

/**
* 写多条日志
*/
template<typename T>
int CServerLog::LogMutilLine(wchar_t *formatstr[], T msg[], unsigned logsum, unsigned CodePage)
{

	return LogOut(formatstr, msg, logsum, CodePage);
}


template<typename T>
int CServerLog::LogOut(wchar_t *formatstr[], T msg[], unsigned logsum, unsigned CodePage)
{
	int result = 0;
	// 参数校验
	if (NULL == formatstr)
	{
		result = -1;
	}
	else
	{
		for (unsigned i = 0; i < logsum; ++i)
		{
			if (NULL == formatstr[i] ||
				wcslen(formatstr[i]) > 128)
			{
				result = -1;
				break;
			}
		}
	}

	if (0 != result)
	{
		return result;
	}

	// 生成时间戳
	wchar_t datetime[64] = { 0 };
	Stamp2Standard(datetime, sizeof datetime / sizeof(wchar_t));

	// 打开日志文件
	ofstream ofs(m_plogFile, ios_base::app);
	if (ofs.is_open())
	{
		// 写缓冲区
		char buff[128] = { 0 };
		wchar_t wcsbuf[128] = { 0 };

		// 写入时间戳
		WideCharToMultiByte(CodePage, NULL, datetime, -1, buff, sizeof buff, NULL, NULL);
		ofs.write(buff, strlen(buff));

		for (unsigned i = 0; i < logsum; ++i)
		{
			if ( NULL != msg )
			{
				swprintf_s(wcsbuf, sizeof wcsbuf / sizeof(wchar_t), formatstr[i], msg[i]);
			}
			else 
			{
				wcscpy_s( wcsbuf, sizeof wcsbuf / sizeof(wchar_t), formatstr[i] );
			}
			
			WideCharToMultiByte(CodePage, NULL, wcsbuf, -1, buff, sizeof buff, NULL, NULL);
			strcat_s(buff, sizeof buff, "\n");
			ofs.write(buff, strlen(buff));
		}

		ofs.clear();
		ofs.close();
	}
	else
	{
		// 文件打开失败
		result = -1;
	}

	return result;
}