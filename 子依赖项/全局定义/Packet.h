#ifndef PACKET_HEAD_FILE
#define PACKET_HEAD_FILE

//内核命令
#define MDM_KN_COMMAND				0									//内核命令
#define SUB_KN_DETECT_SOCKET		1									//检测命令
//#define SUB_KN_VALIDATE_SOCKET		2									//验证命令


//////////////////////////////////////////////////////////////////////////////////
//网络定义
#define SOCKET_TCP_BUFFER			1024*4								//网络缓冲
#define SOCKET_TCP_VER				0x77                                //


//网络内核
struct TCP_Info
{
	BYTE							cbDataKind;							//数据类型
	BYTE							cbCheckCode;						//效验字段
	WORD							wPacketSize;						//数据大小
};

//网络命令
struct TCP_Command
{
	WORD							wMainCmdID;							//主命令码
	WORD							wSubCmdID;							//子命令码
};

//网络包头
struct TCP_Head
{
	TCP_Info						TCPInfo;							//基础结构
	TCP_Command						CommandInfo;						//命令信息
};

#endif
