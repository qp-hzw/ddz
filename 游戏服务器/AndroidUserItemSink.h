#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameInterface.h"


//游戏对话框
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//加注信息
protected:
	SCORE							m_lDiZhu;								//最大下注
	SCORE							m_lTurnMaxScore;						//最大下注
	BYTE							m_cbUserCount;
	//用户扑克
protected:
	// PlayerCards						m_HandCardData;						//用户手牌							

	BYTE							m_cbPlayStatus[MAX_CHAIR_COUNT];			//游戏状态//////////////

	TCHAR							m_szRoomName[32];					    //配置房间

	//控件变量
public:
	IAndroidUserItem *				m_pIAndroidUserItem;					//用户接口
	
	//函数定义
public:
	//构造函数
	CAndroidUserItemSink();
	//析构函数
	virtual ~CAndroidUserItemSink();

private:
	//银行操作
	void BankOperate(BYTE cbType);

	//基础接口
public:
	//释放对象
	virtual VOID Release() { }
	//是否有效
	virtual bool IsValid() { return AfxIsValidAddress(this,sizeof(CAndroidUserItemSink))?true:false; }
	//接口查询
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//控制接口
public:
	//初始接口
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//重置接口
	virtual bool RepositionSink();

	//游戏事件
public:
	//时间消息
	virtual bool OnEventTimer(UINT nTimerID);
	//游戏消息
	virtual bool OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//游戏消息
	virtual bool OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//场景消息
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize);

	//用户事件
public:
	//用户进入
	virtual void OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户离开
	virtual void OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户积分
	virtual void OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户状态
	virtual void OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户段位
	virtual void OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);

	//消息处理
protected:
	//用户叫庄
	bool OnSubCallBanker(const void * pBuffer, WORD wDataSize);
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户加注
	bool OnSubAddScore(const void * pBuffer, WORD wDataSize);
	//发牌消息
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//用户摊牌
	bool OnSubOpenCard(const void * pBuffer, WORD wDataSize);
	//用户强退
	bool OnSubPlayerExit(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////

#endif
