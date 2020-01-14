#pragma once

#include <Afxmt.h>
#include <Comutil.h>
#include <ICrsint.h>
#include <Process.h>
#include <Wininet.h>
#include <WinSock2.h>
#include "..\�������\ServiceCoreHead.h"


//////////////////////////////////////////////////////////////////////////////////
//ADO ����

#import "MSADO15.DLL" rename_namespace("ADOCG") rename("EOF","EndOfFile")
using namespace ADOCG;

typedef _com_error						CComError;						//COM ����
typedef _variant_t						CDBVarValue;					//���ݿ���ֵ

//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef KERNEL_ENGINE_CLASS
	#ifdef  KERNEL_ENGINE_DLL
		#define KERNEL_ENGINE_CLASS _declspec(dllexport)
	#else
		#define KERNEL_ENGINE_CLASS _declspec(dllimport)
	#endif
#endif


//////////////////////////////////////////////////////////////////////////////////
//ϵͳ����
#define TIMES_INFINITY				            DWORD(-1)			        //���޴��� -- ��ʱ����ʹ��

//��������
#define TIME_CELL					200									//ʱ�䵥Ԫ
#define MAX_ASYNCHRONISM_DATA		16384								//�첽����


//////////////////////////////////////////////////////////////////////////////////
//�¼�����

//�¼���ʶ
#define EVENT_TIMER					0x0001								//ʱ���¼�
#define EVENT_DATABASE				0x0003								//���ݿ��¼�

//�����¼�
#define EVENT_TCP_SOCKET_READ		0x0004								//��ȡ�¼�
#define EVENT_TCP_SOCKET_SHUT		0x0005								//�ر��¼�
#define EVENT_TCP_SOCKET_LINK		0x0006								//�����¼�

//�����¼�
#define EVENT_TCP_NETWORK_ACCEPT	0x0007								//Ӧ���¼�
#define EVENT_TCP_NETWORK_READ		0x0008								//��ȡ�¼�
#define EVENT_TCP_NETWORK_SHUT		0x0009								//�ر��¼�

//�¼�����
#define EVENT_MASK_KERNEL			0x00FF								//�ں��¼�

//////////////////////////////////////////////////////////////////////////////////

//�����¼�
struct NTY_ControlEvent
{
	WORD							wControlID;							//���Ʊ�ʶ
};

//��ʱ���¼�
struct NTY_TimerEvent
{
	DWORD							dwTimerID;							//ʱ���ʶ
	WPARAM							dwBindParameter;					//�󶨲���
};

//���ݿ��¼�
struct NTY_DataBaseEvent
{
	WORD							wRequestID;							//�����ʶ
	DWORD							dwContextID;						//�����ʶ
};

//��ȡ�¼�
struct NTY_TCPSocketReadEvent
{
	WORD							wDataSize;							//���ݴ�С
	WORD							wServiceID;							//�����ʶ
	TCP_Command						Command;							//������Ϣ
};

//�ر��¼�
struct NTY_TCPSocketShutEvent
{
	WORD							wServiceID;							//�����ʶ
	BYTE							cbShutReason;						//�ر�ԭ��
};

//�����¼�
struct NTY_TCPSocketLinkEvent
{
	INT								nErrorCode;							//�������
	WORD							wServiceID;							//�����ʶ
};

//Ӧ���¼�
struct NTY_TCPNetworkAcceptEvent
{
	DWORD							dwSocketID;							//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//��ȡ�¼�
struct NTY_TCPNetworkReadEvent
{
	WORD							wDataSize;							//���ݴ�С
	DWORD							dwSocketID;							//�����ʶ
	TCP_Command						Command;							//������Ϣ
};

//�ر��¼�
struct NTY_TCPNetworkShutEvent
{
	DWORD							dwSocketID;							//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	DWORD							dwActiveTime;						//����ʱ��
};

//////////////////////////////////////////////////////////////////////////////////
//���ݿ�ӿ�
interface IDataBase : public IUnknownEx
{
	//���ӽӿ�
public:
	//�������ݿ�
	virtual bool Connect(BYTE byMask)=NULL;
	 
	//�������� -- ����ӿ�
public:
	//���ò���
	virtual VOID ResetParameter()=NULL;
	//��ȡ����
	virtual VOID GetParameter(LPCTSTR pszItem, CDBVarValue & DBVarValue)= NULL;
	//��ȡ����
	virtual VOID GetParameter(LPCTSTR pszItem, LPSTR pszString, UINT uSize)= NULL;
	//��ȡ����
	virtual VOID GetParameter(LPCTSTR pszItem, LPWSTR pszString, UINT uSize)= NULL;

	//�������
public:
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, INT nValue, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, UINT uValue, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, LONG lValue, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, LONGLONG lValue, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, BYTE cbValue, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, WORD wValue, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, DWORD dwValue, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, FLOAT fValue, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, DOUBLE dValue, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, LPCSTR pszString, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, LPCWSTR pszString, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//�������
	virtual VOID AddParameter(LPCTSTR pszItem, SYSTEMTIME & SystemTime, ParameterDirectionEnum ParameterDirection=adParamInput)= NULL;
	//�������
	virtual VOID AddParameterOutput(LPCTSTR pszItem, LPSTR pszString, UINT uSize, ParameterDirectionEnum ParameterDirection=adParamInputOutput)= NULL;
	//�������
	virtual VOID AddParameterOutput(LPCTSTR pszItem, LPWSTR pszString, UINT uSize, ParameterDirectionEnum ParameterDirection=adParamInputOutput)= NULL;
  
	//ִ�нӿ� -- ����ӿ�
public:
	//�洢����
	virtual LONG ExecuteProcess(LPCTSTR pszSPName, bool bRecordset)= NULL;

	//��¼�ӿ�
public:
	//�л���¼
	virtual VOID NextRecordset()=NULL;
	//�رռ�¼
	virtual VOID CloseRecordset()=NULL;
	//�󶨶���
	virtual VOID BindToRecordset(CADORecordBinding * pBind)=NULL;

	//�����ƶ�
	virtual VOID MoveToNext()=NULL;
	//�Ƶ���ͷ
	virtual VOID MoveToFirst()=NULL;
	//�Ƿ����
	virtual bool IsRecordsetEnd()=NULL;
	//��ȡ��Ŀ
	virtual LONG GetRecordCount()=NULL;
	//������ֵ
	virtual LONG GetReturnValue()=NULL;
	//��ȡ����
	virtual VOID GetRecordsetValue(LPCTSTR pszItem, CDBVarValue & DBVarValue)=NULL;


	 //��ȡDB��¼�е�value -- ����ӿ�
public:
	//��ȡ����
	virtual INT GetValue_INT(LPCTSTR pszItem)= NULL;
	//��ȡ����
	virtual UINT GetValue_UINT(LPCTSTR pszItem)= NULL;
	//��ȡ����
	virtual LONG GetValue_LONG(LPCTSTR pszItem)= NULL;
	//��ȡ����
	virtual BYTE GetValue_BYTE(LPCTSTR pszItem)= NULL;
	//��ȡ����
	virtual WORD GetValue_WORD(LPCTSTR pszItem)= NULL;
	//��ȡ����
	virtual DWORD GetValue_DWORD(LPCTSTR pszItem)= NULL;
	//��ȡ����
	virtual FLOAT GetValue_FLOAT(LPCTSTR pszItem)= NULL;
	//��ȡ����
	virtual DOUBLE GetValue_DOUBLE(LPCTSTR pszItem)= NULL;
	//��ȡ����
	virtual LONGLONG GetValue_LONGLONG(LPCTSTR pszItem)= NULL;
	//��ȡ����
	virtual VOID GetValue_SystemTime(LPCTSTR pszItem, SYSTEMTIME & SystemTime)= NULL;
	//��ȡ�ַ�
	virtual VOID GetValue_String(LPCTSTR pszItem, LPSTR pszString, UINT uMaxCount)= NULL;
	//��ȡ�ַ�
	virtual VOID GetValue_String(LPCTSTR pszItem, LPWSTR pszString, UINT uMaxCount) = NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//���ݿ⹳��
interface IDataBaseEngineSink : public IUnknownEx
{
	//ϵͳ�¼�
public:
	//�����¼�
	virtual bool OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)=NULL;
	//ֹͣ�¼�
	virtual bool OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx)=NULL;

	//�ں��¼�
public:
	//�����¼�
	virtual bool OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//��������
interface ITCPNetworkEngine : public IUnknownEx
{
	//���ýӿ�
public:
	//���ò���
	virtual bool SetServiceParameter(WORD wServicePort)=NULL;
	
	//���ͽӿ�
public:
	//���ͺ���
	virtual bool SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//���ͺ���
	virtual bool SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//��������
	virtual bool SendDataBatch(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;

	//���ƽӿ�
public:
	//�ر�����
	virtual bool CloseSocket(DWORD dwSocketID)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//����ӿ�
interface ITCPSocketEngine : public IUnknownEx
{
	//���ýӿ�
public:
	//���ú���
	virtual bool SetServiceID(WORD wServiceID)=NULL;

	//���ܽӿ�
public:
	//������
	virtual bool Connect(DWORD dwServerIP, WORD wPort)=NULL;
	//������
	virtual bool Connect(LPCTSTR szServerIP, WORD wPort)=NULL;
	//���ͺ���
	virtual bool SendData(WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//���ͺ���
	virtual bool SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
};


//////////////////////////////////////////////////////////////////////////////////
//��ʱ������
interface ITimerEngine : public IUnknownEx
{
	//���ܽӿ�
public:
	//���ö�ʱ��
	virtual bool SetTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter)=NULL; 
	//ɾ����ʱ��
	virtual bool KillTimer(DWORD dwTimerID)=NULL;
	//ɾ����ʱ��
	virtual bool KillAllTimer()=NULL;
	//��ȡ��ʱ��ʣ��ʱ�䣨���룩
	virtual DWORD GetTimerLeftTickCount(DWORD dwTimerID) = NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//��������
interface IAttemperEngine : public IUnknownEx
{
	//ע��ص�
public:
	//ע��Attemper�ص�
	virtual bool SetAttemperEngineSink(IUnknownEx * pIUnknownEx)=NULL;
	//ע��DB�ص�
	virtual bool SetDataBaseEngineSink(IUnknownEx * pIUnknownEx)=NULL;

	//DB�ӿ�
public:
	virtual bool PostDataBaseRequest(WORD wRequestID, DWORD dwScoketID, VOID * pData, WORD wDataSize)=NULL;

};

//////////////////////////////////////////////////////////////////////////////////
//���ȹ���
interface IAttemperEngineSink : public IUnknownEx
{
	//�첽�ӿ�
public:
	//�����¼�
	virtual bool OnAttemperEngineStart(IUnknownEx * pIUnknownEx)=NULL;
	//ֹͣ�¼�
	virtual bool OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)=NULL;

	//��ʱ���¼�
public:
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)=NULL;

	//DB�¼�
public:
	virtual bool OnEventDataBaseResult(WORD wRequestID, DWORD dwScoketID, VOID * pData, WORD wDataSize) = NULL;

	//socket::client
public:
	//�����¼�
	virtual bool OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)=NULL;
	//�ر��¼�
	virtual bool OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)=NULL;
	//��ȡ�¼�
	virtual bool OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)=NULL;

	//socket::server
public:
	//Ӧ���¼�
	virtual bool OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)=NULL;
	//�ر��¼�
	virtual bool OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)=NULL;
	//��ȡ�¼�
	virtual bool OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)=NULL;
};
