#ifndef TABLE_FRAME_BASE_H
#define TABLE_FRAME_BASE_H

#include <string>
#include <google/protobuf/message.h>
using namespace google::protobuf;

#define GAME_CONCLUDE_CONTINUE  0xFF //大局结束并续费
#define GAME_CONCLUDE_NORMAL    0xFE //正常结束

//游戏状态
#define GAME_STATUS_FREE			0									//空闲状态
#define GAME_STATUS_PLAY			100									//游戏状态	TODO 细看，改成桌子状态

//结束原因
#define GER_NORMAL					0x00								//常规结束
#define GER_DISMISS					0x01								//游戏解散
#define GER_USER_LEAVE				0x02								//用户离开
#define GER_NETWORK_ERROR			0x03								//网络错误


/////////////////////////////////////////////////////////////////////

//桌子接口
class ITableFrame : public IUnknownEx
{
public:
	//游戏模式 0-房卡模式; 1-竞技模式;  2-金币模式;  3-房卡金币;
	virtual int GameType() = 0;
	//is roboot
	virtual bool IsRobot(WORD wChairID) = 0;

	//流程接口
public:
	//处理小局结束
	virtual bool HandleXJGameEnd(BYTE cbCurGameCount, WORD* wIdentity, SCORE* lGameScore) = NULL;
	//处理大局结束
	virtual bool HandleDJGameEnd(BYTE cbGameStatus) = NULL;

	//时间接口
public:
	//设置时间
	virtual bool SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter) = NULL;
	//删除时间
	virtual bool KillGameTimer(DWORD dwTimerID) = NULL;
	//获取定时器剩余毫秒数
	virtual DWORD GetTimerLeftTickCount(DWORD dwTimerID) = NULL;

	//网络接口
public:
	//发送数据 --protobuf协议
	virtual bool SendTableData(WORD wChairID, WORD wSubCmdID, google::protobuf::Message* pData, WORD wMainCmdID = 200) = NULL;
	//发送场景
	virtual bool SendGameScene(WORD wChairID, google::protobuf::Message* pData) = NULL;

	//托管接口
public:
	//发送托管
	virtual bool SendPlayerTuoGuan(WORD wChairID) = NULL;
	//取消托管
	virtual bool SendPlayerCancelTuoGaun(WORD wChairID) = NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//子游戏接口
class ITableFrameSink : public IUnknownEx
{
	//管理接口
public:
	//启动服务
	virtual bool StartService() { return true; }
	//停止服务
	virtual bool ConcludeService() { return true; }

	//管理接口
public:
	//配置接口
	virtual bool Initialization(ITableFrame* pTableFrame, tagTableRule* comRule) = NULL;

	//游戏事件
public:
	//游戏开始
	virtual bool OnEventGameStart() = NULL;
	//游戏结束
	virtual bool OnEventGameConclude(BYTE cbReason) = NULL;
	//发送场景
	virtual bool OnEventSendGameScene(WORD wChairID) = NULL;

	//事件接口
public:
	//时间事件
	virtual bool OnTimerMessage(DWORD dwTimerID, WPARAM dwBindParameter) = NULL;
	//游戏消息
	virtual bool OnGameMessage(WORD wSubCmdID, void* pData, WORD wDataSize, WORD wChairID) = NULL;

	//托管接口
public:
	//托管
	virtual bool PlayerTuoGuan(WORD wChairID) = NULL;
	//取消托管
	virtual bool PlayerCancelTuoGuan(WORD wChairID) = NULL;
};

//子游戏规则读取
class ISubRoomRuleManager : public IUnknownEx
{
	//管理接口
public:
	//启动服务
	virtual bool StartService() { return true; }
	//停止服务
	virtual bool ConcludeService() { return true; }

	//
public:
	//根据字段名字, 为结构体对应字段赋值
	virtual void SetRoomRule(std::string key, std::string value) = NULL;
	//
	virtual void Init() = NULL;
	//设置Gold场规则
	virtual void SetGoldRule(VOID* roominfo) = NULL;
	//设置match规则
	virtual void SetMatchRule(VOID* config) = NULL;
};

#endif