#pragma once

#include <Afxmt.h>
#include <Comutil.h>
#include <ICrsint.h>
#include <Process.h>
#include <Wininet.h>
#include <WinSock2.h>
#include "..\服务核心\ServiceCoreHead.h"


//////////////////////////////////////////////////////////////////////////////////
//ADO 定义

#import "MSADO15.DLL" rename_namespace("ADOCG") rename("EOF","EndOfFile")
using namespace ADOCG;

typedef _com_error						CComError;						//COM 错误
typedef _variant_t						CDBVarValue;					//数据库数值

//////////////////////////////////////////////////////////////////////////////////
//导出定义

//导出定义
#ifndef KERNEL_ENGINE_CLASS
	#ifdef  KERNEL_ENGINE_DLL
		#define KERNEL_ENGINE_CLASS _declspec(dllexport)
	#else
		#define KERNEL_ENGINE_CLASS _declspec(dllimport)
	#endif
#endif


//////////////////////////////////////////////////////////////////////////////////
//系统常量
#define TIMES_INFINITY				            DWORD(-1)			        //无限次数 -- 定时器中使用

//常量定义
#define TIME_CELL					200									//时间单元
#define MAX_ASYNCHRONISM_DATA		16384								//异步数据


//////////////////////////////////////////////////////////////////////////////////
//事件定义

//事件标识
#define EVENT_TIMER					0x0001								//时间事件
#define EVENT_DATABASE				0x0003								//数据库事件

//网络事件
#define EVENT_TCP_SOCKET_READ		0x0004								//读取事件
#define EVENT_TCP_SOCKET_SHUT		0x0005								//关闭事件
#define EVENT_TCP_SOCKET_LINK		0x0006								//连接事件

//网络事件
#define EVENT_TCP_NETWORK_ACCEPT	0x0007								//应答事件
#define EVENT_TCP_NETWORK_READ		0x0008								//读取事件
#define EVENT_TCP_NETWORK_SHUT		0x0009								//关闭事件

//事件掩码
#define EVENT_MASK_KERNEL			0x00FF								//内核事件

//////////////////////////////////////////////////////////////////////////////////

//控制事件
struct NTY_ControlEvent
{
	WORD							wControlID;							//控制标识
};

//定时器事件
struct NTY_TimerEvent
{
	DWORD							dwTimerID;							//时间标识
	WPARAM							dwBindParameter;					//绑定参数
};

//数据库事件
struct NTY_DataBaseEvent
{
	WORD							wRequestID;							//请求标识
	DWORD							dwContextID;						//对象标识
};

//读取事件
struct NTY_TCPSocketReadEvent
{
	WORD							wDataSize;							//数据大小
	WORD							wServiceID;							//服务标识
	TCP_Command						Command;							//命令信息
};

//关闭事件
struct NTY_TCPSocketShutEvent
{
	WORD							wServiceID;							//服务标识
	BYTE							cbShutReason;						//关闭原因
};

//连接事件
struct NTY_TCPSocketLinkEvent
{
	INT								nErrorCode;							//错误代码
	WORD							wServiceID;							//服务标识
};

//应答事件
struct NTY_TCPNetworkAcceptEvent
{
	DWORD							dwSocketID;							//网络标识
	DWORD							dwClientAddr;						//连接地址
};

//读取事件
struct NTY_TCPNetworkReadEvent
{
	WORD							wDataSize;							//数据大小
	DWORD							dwSocketID;							//网络标识
	TCP_Command						Command;							//命令信息
};

//关闭事件
struct NTY_TCPNetworkShutEvent
{
	DWORD							dwSocketID;							//网络标识
	DWORD							dwClientAddr;						//连接地址
	DWORD							dwActiveTime;						//连接时间
};

//////////////////////////////////////////////////////////////////////////////////
//数据库接口
interface IDataBase : public IUnknownEx
{
	//连接接口
public:
	//连接数据库
	virtual bool Connect(BYTE byMask)=NULL;
	 
	//参数函数 -- 对外接口
public:
	//重置参数
	virtual VOID ResetParameter()=NULL;
	//获取参数
	virtual VOID GetParameter(LPCTSTR pszItem, CDBVarValue & DBVarValue)= NULL;
	//获取参数
	virtual VOID GetParameter(LPCTSTR pszItem, LPSTR pszString, UINT uSize)= NULL;
	//获取参数
	virtual VOID GetParameter(LPCTSTR pszItem, LPWSTR pszString, UINT uSize)= NULL;

	//插入参数
public:
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, INT nValue, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, UINT uValue, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, LONG lValue, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, LONGLONG lValue, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, BYTE cbValue, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, WORD wValue, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, DWORD dwValue, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, FLOAT fValue, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, DOUBLE dValue, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, LPCSTR pszString, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, LPCWSTR pszString, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszItem, SYSTEMTIME & SystemTime, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//插入参数
	virtual VOID AddParameterOutput(LPCTSTR pszItem, LPSTR pszString, UINT uSize, ParameterDirectionEnum ParameterDirection=adParamInputOutput)= NULL;
	//插入参数
	virtual VOID AddParameterOutput(LPCTSTR pszItem, LPWSTR pszString, UINT uSize, ParameterDirectionEnum ParameterDirection=adParamInputOutput)= NULL;
  
	//执行接口 -- 对外接口
public:
	//存储过程
	virtual LONG ExecuteProcess(LPCTSTR pszSPName, bool bRecordset)= NULL;

	//记录接口
public:
	//切换记录
	virtual VOID NextRecordset()=NULL;
	//关闭记录
	virtual VOID CloseRecordset()=NULL;
	//绑定对象
	virtual VOID BindToRecordset(CADORecordBinding * pBind)=NULL;

	//往下移动
	virtual VOID MoveToNext()=NULL;
	//移到开头
	virtual VOID MoveToFirst()=NULL;
	//是否结束
	virtual bool IsRecordsetEnd()=NULL;
	//获取数目
	virtual LONG GetRecordCount()=NULL;
	//返回数值
	virtual LONG GetReturnValue()=NULL;
	//获取数据
	virtual VOID GetRecordsetValue(LPCTSTR pszItem, CDBVarValue & DBVarValue)=NULL;


	 //获取DB记录中的value -- 对外接口
public:
	//获取数据
	virtual INT GetValue_INT(LPCTSTR pszItem)= NULL;
	//获取数据
	virtual UINT GetValue_UINT(LPCTSTR pszItem)= NULL;
	//获取数据
	virtual LONG GetValue_LONG(LPCTSTR pszItem)= NULL;
	//获取数据
	virtual BYTE GetValue_BYTE(LPCTSTR pszItem)= NULL;
	//获取数据
	virtual WORD GetValue_WORD(LPCTSTR pszItem)= NULL;
	//获取数据
	virtual DWORD GetValue_DWORD(LPCTSTR pszItem)= NULL;
	//获取数据
	virtual FLOAT GetValue_FLOAT(LPCTSTR pszItem)= NULL;
	//获取数据
	virtual DOUBLE GetValue_DOUBLE(LPCTSTR pszItem)= NULL;
	//获取数据
	virtual LONGLONG GetValue_LONGLONG(LPCTSTR pszItem)= NULL;
	//获取数据
	virtual VOID GetValue_SystemTime(LPCTSTR pszItem, SYSTEMTIME & SystemTime)= NULL;
	//获取字符
	virtual VOID GetValue_String(LPCTSTR pszItem, LPSTR pszString, UINT uMaxCount)= NULL;
	//获取字符
	virtual VOID GetValue_String(LPCTSTR pszItem, LPWSTR pszString, UINT uMaxCount) = NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//数据库钩子
interface IDataBaseEngineSink : public IUnknownEx
{
	//系统事件
public:
	//启动事件
	virtual bool OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)=NULL;
	//停止事件
	virtual bool OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx)=NULL;

	//内核事件
public:
	//请求事件
	virtual bool OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//网络引擎
interface ITCPNetworkEngine : public IUnknownEx
{
	//配置接口
public:
	//设置参数
	virtual bool SetServiceParameter(WORD wServicePort)=NULL;
	
	//发送接口
public:
	//发送函数
	virtual bool SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//发送函数
	virtual bool SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//批量发送
	virtual bool SendDataBatch(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;

	//控制接口
public:
	//关闭连接
	virtual bool CloseSocket(DWORD dwSocketID)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//网络接口
interface ITCPSocketEngine : public IUnknownEx
{
	//配置接口
public:
	//配置函数
	virtual bool SetServiceID(WORD wServiceID)=NULL;

	//功能接口
public:
	//打开连接
	virtual bool Connect(DWORD dwServerIP, WORD wPort)=NULL;
	//打开连接
	virtual bool Connect(LPCTSTR szServerIP, WORD wPort)=NULL;
	//发送函数
	virtual bool SendData(WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//发送函数
	virtual bool SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
};


//////////////////////////////////////////////////////////////////////////////////
//定时器引擎
interface ITimerEngine : public IUnknownEx
{
	//功能接口
public:
	//设置定时器
	virtual bool SetTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter)=NULL; 
	//删除定时器
	virtual bool KillTimer(DWORD dwTimerID)=NULL;
	//删除定时器
	virtual bool KillAllTimer()=NULL;
	//获取定时器剩余时间（毫秒）
	virtual DWORD GetTimerLeftTickCount(DWORD dwTimerID) = NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//调度引擎
interface IAttemperEngine : public IUnknownEx
{
	//注册回调
public:
	//注册Attemper回调
	virtual bool SetAttemperEngineSink(IUnknownEx * pIUnknownEx)=NULL;
	//注册DB回调
	virtual bool SetDataBaseEngineSink(IUnknownEx * pIUnknownEx)=NULL;

	//DB接口
public:
	virtual bool PostDataBaseRequest(WORD wRequestID, DWORD dwScoketID, VOID * pData, WORD wDataSize)=NULL;

};

//////////////////////////////////////////////////////////////////////////////////
//调度钩子
interface IAttemperEngineSink : public IUnknownEx
{
	//异步接口
public:
	//启动事件
	virtual bool OnAttemperEngineStart(IUnknownEx * pIUnknownEx)=NULL;
	//停止事件
	virtual bool OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)=NULL;

	//定时器事件
public:
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)=NULL;

	//DB事件
public:
	virtual bool OnEventDataBaseResult(WORD wRequestID, DWORD dwScoketID, VOID * pData, WORD wDataSize) = NULL;

	//socket::client
public:
	//连接事件
	virtual bool OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)=NULL;
	//关闭事件
	virtual bool OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)=NULL;
	//读取事件
	virtual bool OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)=NULL;

	//socket::server
public:
	//应答事件
	virtual bool OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)=NULL;
	//关闭事件
	virtual bool OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)=NULL;
	//读取事件
	virtual bool OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)=NULL;
};
