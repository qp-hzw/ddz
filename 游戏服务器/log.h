#ifndef TRACE_SERVICE_HEAD_FILE
#define TRACE_SERVICE_HEAD_FILE

//log����ȼ�
enum enTraceLevel2
{
	log_debug				=0,									//������Ϣ
	log_warn				=1,									//������Ϣ
	log_error			    =2,									//������Ϣ
};

//׷�ٷ���
class  CLog //: public ILog
{
	//��������
protected:
	//���캯��
	CLog();
	~CLog();

public:
	//������־�ļ�
	static bool Init(const char* szFileName);

public:
	//log��־
	static void Log(enTraceLevel2 TraceLevel, const char* pszFormat, ...);
};

#endif