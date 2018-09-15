#pragma once
// ����ģʽ����־��
class CServerLog
{
private:
	CServerLog();
	~CServerLog();

// д��־
public:
	// дһ����־ģ��
	template<typename T>
	int LogSingleLine(wchar_t *formatstr, T msg, unsigned CodePage = CP_ACP);

	// д������־ģ��
	template<typename T>
	int LogMutilLine( wchar_t *formatstr[], T msg[], unsigned logsum, unsigned CodePage = CP_ACP);

protected:

	// д������־ģ��
	template<typename T>
	int LogOut(wchar_t *formatstr[], T msg[], unsigned logsum, unsigned CodePage);
	
	// ʱ���ת��
	int Stamp2Standard(wchar_t *timebuff, unsigned bufferlen);


// ����ʵ��
public:
	/**
	 * �˺�����ȡ��־ʵ��
	 * @param			wcsLogFile		[in]		��־�ļ�
	 * @param			pplog			[out]		��־ʵ��
	 */
	static int GetLogInstance( const wchar_t *wcsLogFile, CServerLog ** pplog );

	/**
	 * �˺����ͷ�һ����־ʵ��
	 */
	static int Release(CServerLog *pplog);

protected:

	// Ϊ��־ʵ��������־�ļ�Ŀ¼
	int SetLogFile(const wchar_t *wcsLogFile);

protected:
	CServerLog *m_plog;				// ��־ʵ��
	wchar_t		m_plogFile[128];	// ��־�ļ�·��
};

#include <fstream>

using std::ofstream;
using std::ios_base;

// дһ����־ģ��
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
* д������־
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
	// ����У��
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

	// ����ʱ���
	wchar_t datetime[64] = { 0 };
	Stamp2Standard(datetime, sizeof datetime / sizeof(wchar_t));

	// ����־�ļ�
	ofstream ofs(m_plogFile, ios_base::app);
	if (ofs.is_open())
	{
		// д������
		char buff[128] = { 0 };
		wchar_t wcsbuf[128] = { 0 };

		// д��ʱ���
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
		// �ļ���ʧ��
		result = -1;
	}

	return result;
}