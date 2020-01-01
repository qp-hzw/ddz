#ifndef TRACE_SERVICE_HEAD_FILE
#define TRACE_SERVICE_HEAD_FILE

//log输出等级
enum enTraceLevel2
{
	log_debug				=0,									//调试消息
	log_warn				=1,									//警告消息
	log_error			    =2,									//错误消息
};

//追踪服务
class  CLog //: public ILog
{
	//函数定义
protected:
	//构造函数
	CLog();
	~CLog();

public:
	//设置日志文件
	static bool Init(const char* szFileName);

public:
	//log日志
	static void Log(enTraceLevel2 TraceLevel, const char* pszFormat, ...);
};

#endif